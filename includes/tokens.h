#pragma once

#include <array> 
#include <string_view> 
#include <nasty_macros.h>

namespace akura {
        AKURA_ENUM_ARRAY(TokenId, tokenDescriptors,
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
                operator_star,
                comma_separator,
                function_declaration,
                type_declaration,
                return_statement,
                semicolon,
                colon,
                integer,
                floating_point,
                forward_slash,
                backwards_slash,
                identifier,
                keyword_s64,
                keyword_s32,
                keyword_s16,
                keyword_s8,
                keyword_u64,
                keyword_u32,
                keyword_u16,
                keyword_u8,
                keyword_f64,
                keyword_f32,
                keyword_if,
                keyword_for,
                keyword_match,
                keyword_static,
                keyword_import,
                keyword_export,
                end);
    struct Token {
        std::string_view value = "";
        TokenId id = TokenId::end;
        int line = -1;
        int column = -1;
        Token() = default;
        Token(const Token &) = default;
        Token(Token &&) = default;
        Token & operator=(const Token &) = default;
        Token & operator=(Token &&) = default;
        Token(std::string_view value, TokenId id, int line, int column) :
            value(value),
            id(id),
            line(line),
            column(column) {}
    };
    constexpr static inline auto & token_name(TokenId id) {
        return tokenDescriptors[static_cast<int>(id)];
    }
    std::array<std::string_view, 3> get_line(std::string_view text, int column);
}
