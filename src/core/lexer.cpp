#include <cstddef>
#include <cstdlib>
#include <flashquery/core/lexer.hpp>
#include <string_view>

flashquery::Lexer::Lexer(Arena &arena, const std::string_view &html)
    : _arena(arena), html_data(html), tokens{0}, tag_open(false)
{
}

bool flashquery::Lexer::begin()
{
    for (std::size_t i = 0; i < this->html_data.length(); i++) {

        if (!this->tag_open) {
            if (this->html_data[i] == '<') {
                this->tag_open = true;
                this->add_token(TokenType::TAG_OPEN);
            }
        }
        else {
            if (this->html_data[i] == '>') {
                this->tag_open = false;
                this->add_token(TokenType::TAG_CLOSE);
            }
        }
    }

    return true;
}


void flashquery::Lexer::add_token(const TokenType &type, const std::string_view &k, const std::string_view &v)
{
    Token *t = this->_arena.alloc<Token>(type, k, v);
    this->tokens.arr = t - this->tokens.len; // incase reallocation occurs and base memory addr changes
    this->tokens.len++;
}

