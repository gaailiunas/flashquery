#include <flashquery/core/lexer.hpp>
#include <string_view>

flashquery::Lexer::Lexer(Arena &arena, const std::string_view &html)
    : _arena(arena), html_data(html), index(0), tokens{0}, tag_open(false), text_len(0),
    tag_name(false), kv_attr(false)
{
}

bool flashquery::Lexer::begin()
{
    for (; this->index < this->html_data.length(); this->index++) {
        if (this->tag_open) {
            if (this->html_data[this->index] == '>') {
                if (this->text_len > 0) {
                    // TODO: make a function of this portion as it is repetitive to the next else if statement
                    if (!this->tag_name) {
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
                this->tag_open = false;
                this->add_token(TokenType::TAG_CLOSE);
                continue;
            }
            else if (this->html_data[this->index] == ' ') {
                if (!this->tag_name) {
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
                continue;
            }
            else if (this->html_data[this->index] == '=') {
                this->kv_attr = true;
                this->feed_text(TokenType::TAG_ATTR_KEY);
                continue;
            }
        }
        else {
            if (this->html_data[this->index] == '<') {
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

void flashquery::Lexer::feed_text(const TokenType &type)
{
    this->add_token(type, std::string_view(this->html_data.data() - this->text_len + this->index, this->text_len));
    this->text_len = 0;
}

void flashquery::Lexer::reset()
{
    this->tag_name = false;
    this->kv_attr = false;
}
