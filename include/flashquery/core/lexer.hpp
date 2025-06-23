#ifndef FLASHQUERY_CORE_LEXER_HPP
#define FLASHQUERY_CORE_LEXER_HPP

#include <flashquery/core/arena.hpp>
#include <string_view>

namespace flashquery {

enum class TokenType {
    TAG_OPEN,
    TAG_CLOSE,
};

struct Token {
    TokenType type; 
    std::string_view key;
    std::string_view value;

    Token(const TokenType &t, const std::string_view &k, const std::string_view &v) noexcept : type(t), key(k), value(v)
    {
    }
};

struct TokenArray {
    Token *arr;
    std::size_t len;
};

class Lexer {
public:
    explicit Lexer(Arena &arena, const std::string_view &html);
    bool begin();

    inline const TokenArray &get_tokens() const
    {
        return this->tokens;
    }

private:
    /* states */
    bool tag_open; 

    Arena &_arena;
    std::string_view html_data;
    
    TokenArray tokens;

    void add_token(const TokenType &type, const std::string_view &k = "", const std::string_view &v = "");
};

} // namespace flashquery


#endif // FLASHQUERY_CORE_LEXER_HPP
