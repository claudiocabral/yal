#pragma once

#include <array> 
#include <string_view> 

namespace akura {
    //using namespace std::literals;
    enum class TokenId {
        import,
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
        end
    };
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
    //constexpr std::array<std::string_view, static_cast<size_t>(TokenId::end)> TokenDescriptors;
}
