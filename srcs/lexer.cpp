#include <string_view>
#include <vector>
#include <tokens.h>
#include <lexer.h>
#include <cstdio>

using akura::Token;
using akura::TokenId;
using TokenList = std::vector<Token>;
using Predicate = bool (*)(char);

struct FileContext {
    int line = 1;
    int column = 0;
    std::string_view text;
};

static inline bool is_whitespace(char c) { return c == ' ' || c == '\t'; }
static inline bool is_newline(char c) { return c == '\n'; }
static inline bool is_forward_slash(char c) { return c == '/'; }
static inline bool is_digit(char c) { return c >= '0' && c <= '9'; }
static inline bool is_alphabetic(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

void print_tokens(TokenList & tokens)
{
    for (auto & token : tokens) {
        printf("%d:%d:\n\tTokenId: %d\n\tValue: %.*s\n",
                token.line,
                token.column,
                static_cast<int>(token.id),
                static_cast<int>(token.value.size()),
                token.value.data()
                );
    }
}

template <TokenId id, Predicate predicate>
void lex_sequence(FileContext & context, TokenList & tokens)
{
    context.column += 1;
    auto begin = context.text.begin();
    auto it = context.text.begin() + 1;
    auto end = context.text.end();
    while (it != end && predicate(*it))
    {
        ++it;
        ++context.column;
    }
    tokens.emplace_back(
            std::string_view(begin, std::distance(begin, it)),
            id,
            context.line,
            context.column
            );
    context.text = std::string_view(it, end - it);
}

bool lex(std::string_view text) {
    std::vector<Token> tokens;
    FileContext context{ 1, 0, text };
    while (true)
    {
        if (context.text.size() == 0)
        {
            print_tokens(tokens);
            return true;
        }
        auto c = context.text.front();
        if (is_whitespace(c)) { ++context.column; context.text.remove_prefix(1); }
        else if (is_newline(c)) { ++context.line; context.column = 0; context.text.remove_prefix(1); }
        else if (is_alphabetic(c)) { lex_sequence<TokenId::word, is_alphabetic>(context, tokens); }
        else if (is_digit(c)) { lex_sequence<TokenId::number, is_digit>(context, tokens); }
        else { ++context.column; context.text.remove_prefix(1); }
    }
    printf("error\n");
    return false;
}
