#include <flashquery/core/arena.hpp>
#include <flashquery/core/lexer.hpp>

#include <iostream>
#include <cassert>

int main(void)
{
    flashquery::Arena arena(1024);
    flashquery::Lexer lexer(arena, "<html></html>");

    bool r = lexer.begin();
    assert(r == true);

    const flashquery::TokenArray &tokens = lexer.get_tokens();
    std::cout << "Total tokens: " << tokens.len << std::endl;

    for (std::size_t i = 0; i < tokens.len; i++) {
        std::cout << "token_name=" << flashquery::token_to_str(tokens.arr[i].type) << " key=" << tokens.arr[i].key << " value=" << tokens.arr[i].value << std::endl;
    }

    return 0;
}
