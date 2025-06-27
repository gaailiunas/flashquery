#include <cstring>
#include <flashquery/core/lexer.hpp>
#include <string_view>

#include <iostream>

flashquery::Lexer::Lexer(Arena &arena, const std::string_view &html)
    : _arena(arena), html_data(html), index(0), current_token(nullptr), tokens{0}, tag_open(false), text_len(0),
    tag_name(false), kv_attr(false), enforce(false)
{
}

bool flashquery::Lexer::begin()
{
    for (; this->index < this->html_data.length(); this->index++) {
        if (this->tag_open) {
            if (this->html_data[this->index] == '>') {
                this->process_tag();
                this->tag_open = false;
                this->add_token(TokenType::TAG_CLOSE);
                continue;
            }
            else if (this->html_data[this->index] == ' ') {
                this->process_tag(); 
                continue;
            }
            else if (this->html_data[this->index] == '=') {

                // TODO: check if the next char is quotes, then begin treating everything until next quote is encountered

                this->kv_attr = true;
                this->feed_text(TokenType::TAG_ATTR_KEY);
                continue;
            }
        }
        else {
            if (this->html_data[this->index] == '<') {
                if (this->enforce) {
                    // are the next chars /script> or /style>
                  
                    // TODO: make enforcing more elegant with nonhardcoded values
                    if (this->index + 7 < this->html_data.length())
                    {
                        // /style>
                        bool ok = this->html_data[this->index + 1] == '/' && 
                            this->html_data[this->index + 2] == 's' &&
                            this->html_data[this->index + 3] == 't' &&
                            this->html_data[this->index + 4] == 'y' &&
                            this->html_data[this->index + 5] == 'l' &&
                            this->html_data[this->index + 6] == 'e' &&
                            this->html_data[this->index + 7] == '>';

                        if (!ok) {
                            if (this->index + 8 < this->html_data.length())
                            {
                                // /script>
                                ok = this->html_data[this->index + 1] == '/' && 
                                    this->html_data[this->index + 2] == 's' &&
                                    this->html_data[this->index + 3] == 'c' &&
                                    this->html_data[this->index + 4] == 'r' &&
                                    this->html_data[this->index + 5] == 'i' &&
                                    this->html_data[this->index + 6] == 'p' &&
                                    this->html_data[this->index + 7] == 't' &&
                                    this->html_data[this->index + 8] == '>';

                                if (ok) {
                                    std::cout << "script closing" << std::endl;

                                    this->feed_text(TokenType::TEXT);

                                    this->add_token(TokenType::TAG_OPEN);
                                    this->add_token(TokenType::TAG_SLASH);
                                    this->add_token(TokenType::TAG_NAME, std::string_view(this->html_data.data() + this->index + 2, 6));
                                    this->add_token(TokenType::TAG_CLOSE);

                                    this->index += 8;
                                    this->enforce = false;
                                }
                            }
                        }
                        else {
                            std::cout << "style closing down" << std::endl;

                            this->feed_text(TokenType::TEXT);

                            this->add_token(TokenType::TAG_OPEN);
                            this->add_token(TokenType::TAG_SLASH);
                            this->add_token(TokenType::TAG_NAME, std::string_view(this->html_data.data() + this->index + 2, 5));
                            this->add_token(TokenType::TAG_CLOSE);

                            this->index += 7;
                            this->enforce = false;
                        }

                    }

                    continue;
                }

                this->feed_text(TokenType::TEXT);
                this->tag_open = true;
                this->reset();

                this->add_token(TokenType::TAG_OPEN);
                if (this->index + 1 < this->html_data.length()) {
                    if (this->html_data[this->index + 1] == '/') {
                        this->add_token(TokenType::TAG_SLASH);
                        this->index++;
                    }
                }
                continue;
            }
        }

        this->text_len++;
    }
    return true;
}


void flashquery::Lexer::add_token(const TokenType &type, const std::string_view &elem)
{
    Token *t = this->_arena.alloc<Token>(type, elem);
    this->tokens.arr = t - this->tokens.len; // incase reallocation occurs and base memory addr changes
    this->tokens.len++;
}

std::string_view flashquery::Lexer::get_text()
{
    return std::string_view(this->html_data.data() - this->text_len + this->index, this->text_len);
}

void flashquery::Lexer::feed_text(const TokenType &type)
{
    if (this->text_len > 0) {
        this->add_token(type, this->get_text());
        this->text_len = 0;
    }
}

void flashquery::Lexer::process_tag()
{
    if (!this->tag_name) {
        this->current_token = this->html_data.data() - this->text_len + this->index;

        // check for "script" and "style"
        if (this->text_len == 6 && std::memcmp(this->current_token, "script", 6) == 0) {
            std::cout << "enforcing script" << std::endl;
            this->enforce = true;
        }
        else if (this->text_len == 5 && std::memcmp(this->current_token, "style", 5) == 0) {
            std::cout << "enforcing style" << std::endl;
            this->enforce = true;
        }

        this->feed_text(TokenType::TAG_NAME);
        this->tag_name = true;
    }
    else if (this->kv_attr) {
        this->feed_text(TokenType::TAG_ATTR_VALUE);
        this->kv_attr = false;
    }
    else {
        this->feed_text(TokenType::TAG_ATTR);
    }
}

void flashquery::Lexer::reset()
{
    this->tag_name = false;
    this->kv_attr = false;
}
