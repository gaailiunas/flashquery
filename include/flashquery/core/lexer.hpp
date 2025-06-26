#ifndef FLASHQUERY_CORE_LEXER_HPP
#define FLASHQUERY_CORE_LEXER_HPP

#include <flashquery/core/arena.hpp>
#include <string_view>

namespace flashquery {

enum class TokenType {
    TAG_OPEN,
    TAG_CLOSE,
    TAG_SLASH,
    TAG_NAME,
};

inline static const char *token_to_str(const TokenType &type) {
    switch (type) {
        case TokenType::TAG_OPEN: return "TAG_OPEN";
        case TokenType::TAG_CLOSE: return "TAG_CLOSE";
        case TokenType::TAG_SLASH: return "TAG_SLASH";
        case TokenType::TAG_NAME: return "TAG_NAME";
        default: return "UNKNOWN";
    }
}

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
    std::size_t text_len;
    bool tag_name;

    Arena &_arena;
    std::string_view html_data;
    
    TokenArray tokens;

    void add_token(const TokenType &type, const std::string_view &k = "", const std::string_view &v = "");
};

} // namespace flashquery


#endif // FLASHQUERY_CORE_LEXER_HPP
