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
    TAG_ATTR_KEY,
    TAG_ATTR_VALUE,
    TAG_ATTR,
    TEXT,
};

inline static const char *token_to_str(const TokenType &type) {
    switch (type) {
        case TokenType::TAG_OPEN: return "TAG_OPEN";
        case TokenType::TAG_CLOSE: return "TAG_CLOSE";
        case TokenType::TAG_SLASH: return "TAG_SLASH";
        case TokenType::TAG_NAME: return "TAG_NAME";
        case TokenType::TAG_ATTR_KEY: return "TAG_ATTR_KEY";
        case TokenType::TAG_ATTR_VALUE: return "TAG_ATTR_VALUE";
        case TokenType::TAG_ATTR: return "TAG_ATTR";
        case TokenType::TEXT: return "TEXT";
        default: return "UNKNOWN";
    }
}

struct Token {
    TokenType type; 
    std::string_view element;

    Token(const TokenType &t, const std::string_view &elem) noexcept : type(t), element(elem)
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
    bool kv_attr;
    bool enforce;

    Arena &_arena;
    std::string_view html_data;
    std::size_t index;
    const char *current_token;
    
    TokenArray tokens;

    void add_token(const TokenType &type, const std::string_view &elem = "");

    inline std::string_view get_text();
    void feed_text(const TokenType &type);

    void process_tag();

    void reset();
};

} // namespace flashquery


#endif // FLASHQUERY_CORE_LEXER_HPP
