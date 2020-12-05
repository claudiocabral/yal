#include <catch2/catch.hpp>
#include <lexer.h>

using namespace std::literals;

TEST_CASE("lex", "[lex]")
{
    constexpr auto file = 
        "/*\n"
        "import stdc.stdio {\n"
        "  whatever = puts,\n"
        "  printf,\n"
        "}\n"
        "*/\n"
        "\n"
        "function increment(value) {\n"
        "  return value + 1;\n"
        "}\n"
        "\n"
        "function entrypoint {\n"
        "  a := s64(7);\n"
        "  return a.increment();\n"
        "}"
        ""sv;
    lex(file);
    REQUIRE(1 == 1);
}

