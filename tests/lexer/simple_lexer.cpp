#include <catch2/catch.hpp>
#include <task_system/task_system.h>
#include <lexer.h>

using namespace std::literals;

TEST_CASE("Lex Numbers", "[lex]")
{
    constexpr std::string_view numbers =
        "1\n"
        "1.0\n"
        "912376921791.0128390180\n"
        "98765432101234567890123456789\n"
        "";
    auto expected_tokens = std::array {
        akura::Token { "1", akura::TokenId::integer, 1, 1},
        akura::Token { "1.0", akura::TokenId::floating_point, 2, 1},
        akura::Token { "912376921791.0128390180", akura::TokenId::floating_point, 3, 1},
        akura::Token { "98765432101234567890123456789", akura::TokenId::integer, 4, 1},
        akura::Token { "", akura::TokenId::end, 5, 1}
    };
    auto tokens = lex(numbers);
    REQUIRE(tokens.size() == expected_tokens.size());
    auto i = GENERATE(range(0, 5));
    CHECK(tokens[i].value == expected_tokens[i].value);
    CHECK(tokens[i].id == expected_tokens[i].id);
    CHECK(tokens[i].line == expected_tokens[i].line);
    CHECK(tokens[i].column == expected_tokens[i].column);
}

TEST_CASE("Lex symbols", "[lex]")
{
    constexpr std::string_view symbols =
        "\\\"'.(){}[]*/:;"
        "";
    auto expected_tokens = std::array {
        akura::Token { "\\", akura::TokenId::backwards_slash, 1, 1},
        akura::Token { "\"", akura::TokenId::double_quote, 1, 2},
        akura::Token { "'", akura::TokenId::single_quote, 1, 3},
        akura::Token { ".", akura::TokenId::operator_dot, 1, 4},
        akura::Token { "(", akura::TokenId::open_parens, 1, 5},
        akura::Token { ")", akura::TokenId::close_parens, 1, 6},
        akura::Token { "{", akura::TokenId::open_curly_braces, 1, 7},
        akura::Token { "}", akura::TokenId::close_curly_braces, 1, 8},
        akura::Token { "[", akura::TokenId::open_square_braces, 1, 9},
        akura::Token { "]", akura::TokenId::close_square_braces, 1, 10},
        akura::Token { "*", akura::TokenId::operator_star, 1, 11},
        akura::Token { "/", akura::TokenId::forward_slash, 1, 12},
        akura::Token { ":", akura::TokenId::colon, 1, 13},
        akura::Token { ";", akura::TokenId::semicolon, 1, 14},
        akura::Token { "", akura::TokenId::end, 1, 15}
    };
    auto tokens = lex(symbols);
    REQUIRE(tokens.size() == expected_tokens.size());
    auto size = tokens.size();
    auto i = GENERATE_COPY(range<int>(0, size));
    CHECK(tokens[i].value == expected_tokens[i].value);
    CHECK(tokens[i].id == expected_tokens[i].id);
    CHECK(tokens[i].line == expected_tokens[i].line);
    CHECK(tokens[i].column == expected_tokens[i].column);
}

TEST_CASE("Lex comments", "[lex]")
{
    constexpr std::string_view symbols =
        "abc // abc 123\n"
        "def // abc 123\n"
        "/* /**/*/\n"
        "z\n"
        "";
    auto expected_tokens = std::array {
        akura::Token { "abc", akura::TokenId::identifier, 1, 1},
        akura::Token { "def", akura::TokenId::identifier, 2, 1},
        akura::Token { "z", akura::TokenId::identifier, 4, 1},
        akura::Token { "", akura::TokenId::end, 5, 1},
    };
    auto tokens = lex(symbols);
    REQUIRE(tokens.size() == expected_tokens.size());
    auto size = tokens.size();
    auto i = GENERATE_COPY(range<int>(0, size));
    CHECK(tokens[i].value == expected_tokens[i].value);
    CHECK(tokens[i].id == expected_tokens[i].id);
    CHECK(tokens[i].line == expected_tokens[i].line);
    CHECK(tokens[i].column == expected_tokens[i].column);
}
