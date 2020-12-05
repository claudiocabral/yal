#pragma once

#include <array> 
#include <string_view> 
#include <nasty_macros.h>


#define AKURA_TOKENS(...) \
    enum class TokenId { __VA_ARGS__}; \
    constexpr auto tokenDescriptors = std::array { STRINGIFY_MANY(__VA_ARGS__)}; \
    static_assert(tokenDescriptors.size() != 1, ""); 

namespace akura {
    //using namespace std::literals;
    //enum class TokenId {
        AKURA_TOKENS(import,
        open_parens,
        close_parens,
        open_curly_braces,
        close_curly_braces,
        open_square_braces,
        close_square_braces,
        open_multiline_comment,
        close_multiline_comment,
        single_line_comment,
        double_quote,
        single_quote,
        operator_dot,
        operator_equals,
        operator_colon,
        operator_colon_equals,
        operator_plus,
        operator_minus,
        comma_separator,
        function_declaration,
        type_declaration,
        word,
        return_statement,
        semicolon,
        number,
        forward_slash,
        backwards_slash,
        end);
    //};
    //constexpr std::array<std::string_view, static_cast<size_t>(TokenId::end)> TokenDescriptors;
    struct Token {
        std::string_view value;
        // std::string_view filename?;
        TokenId id;
        int line;
        int column;
        Token(std::string_view value, TokenId id, int line, int column) :
            value(value),
            id(id),
            line(line),
            column(column) {}
    };
    constexpr static inline auto & token_name(TokenId id) {
        return tokenDescriptors[static_cast<int>(id)];
    }
}
