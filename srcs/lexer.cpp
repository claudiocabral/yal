#include <string_view>
#include <vector>
#include <tokens.h>
#include <lexer.h>
#include <cstdio>
#include <keywords.h>

using akura::Token;
using akura::TokenId;
using TokenList = std::vector<Token>;
using Predicate = bool (*)(char);

struct FileContext {
    int line = 1;
    int column = 1;
    std::string_view text;
};

static inline bool is_whitespace(char c) { return c == ' ' || c == '\t'; }
static inline bool is_newline(char c) { return c == '\n'; }
static inline bool is_digit(char c) { return c >= '0' && c <= '9'; }
static inline bool is_alphabetic(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
static inline bool is_alphanumeric(char c) { return is_digit(c) || is_alphabetic(c) || c == '_'; }
static inline bool is_identifier_character(char c) { return is_alphanumeric(c) || c == '_'; }

void print_tokens(TokenList & tokens)
{
    for (auto & token : tokens) {
        printf("%d:%d:\tTokenId: %s\n\tValue: %.*s\n",
                token.line,
                token.column,
                akura::token_name(token.id),
                static_cast<int>(token.value.size()),
                token.value.data()
                );
    }
}

void emit_token(FileContext & context, TokenList & tokens, TokenId id, int size)
{
    auto begin = context.text.begin();
    auto end = context.text.end();
    tokens.emplace_back(
            std::string_view(begin, size),
            id,
            context.line,
            context.column
            );
    begin += size;
    context.text = std::string_view(begin, end - begin);
    context.column += size;
}

template <class Predicate>
auto find_token_end(FileContext & context, Predicate && predicate)
{
    auto begin = context.text.begin();
    auto it = context.text.begin() + 1;
    auto end = context.text.end();
    while (it != end && predicate(*it))
    {
        ++it;
    }
    auto token = std::string_view(begin, it - begin);
    begin += token.size();
    context.text = std::string_view(begin, end - begin);
    context.column += token.size();
    return token;
}

void lex_identifier(FileContext & context, TokenList & tokens)
{
    auto column = context.column;
    auto token = find_token_end(context, is_identifier_character);
    auto keyword = akura::keywords.find(token);
    auto id = keyword != akura::keywords.end()
        ? keyword->second
        : TokenId::identifier;
    tokens.emplace_back(token, id, context.line, column);
}

void lex_number(FileContext & context, TokenList & tokens)
{
    auto column = context.column;
    auto token = find_token_end(context, is_identifier_character);
    if (context.text.size() != 0 && context.text[0] != '.')
    {
        tokens.emplace_back(token, TokenId::integer, context.line, column);
        return;
    }
    auto remainder = find_token_end(context, is_identifier_character);
    auto result = std::string_view(token.begin(), token.size() + remainder.size());
    tokens.emplace_back(result, TokenId::floating_point, context.line, column);
}

void lex_multiline_comment(FileContext & context, TokenList & tokens)
{
    auto level = 1;
    constexpr int16_t open = ('/' << 8) | ('*');
    constexpr int16_t close = ('*' << 8) | ('/');
    auto begin = context.text.begin() + 2;
    auto end = context.text.end();
    while (begin < end && begin + 1 < end)
    {
        int16_t token = begin[0] << 8 | begin[1];
        if (token == open)
        {
            begin += 2;
            context.column += 2;
            ++level;
        }
        else if (token == close)
        {
            begin += 2;
            context.column += 2;
            --level;
            if (level == 0)
                break;
        }
        else if (begin[0] == '\n')
        {
            context.column = 1;
            ++context.line;
            ++begin;
        }
        else
        {
            ++begin;
            ++context.column;
        }

    }
    auto old_begin = context.text.begin();
    context.text.remove_prefix(begin - old_begin);
}

void lex_forward_slash(FileContext & context, TokenList & tokens)
{
    auto column = context.column;
    auto line = context.line;
    auto begin = context.text.begin() + 1;
    auto end = context.text.end();
    if (begin == end)
        return ;
    if (*begin == '/')
    {
        ++begin;
        while (begin != end && *begin != '\n')
        {
            ++begin;
        }
        begin += begin != end;
        context.column = 1;
        ++context.line;
        auto old_begin = context.text.begin();
        context.text.remove_prefix(begin - old_begin);
    }
    else if (*begin == '*')
    {
        lex_multiline_comment(context, tokens);
    }
    else
    {
        emit_token(context, tokens, TokenId::forward_slash, 1);
    }
}

void lex_unkwon(FileContext & context)
{
    printf("%d:%d: Unkwon token: '%c'\n",
            context.line,
            context.column,
            context.text.front()
          );
    ++context.column; context.text.remove_prefix(1);
}




std::vector<akura::Token> lex(std::string_view text) {
    std::vector<Token> tokens;
    FileContext context{ 1, 1, text };
    while (true)
    {
        if (context.text.size() == 0)
        {
            emit_token(context, tokens, TokenId::end, 0);
            return tokens;
        }
        auto c = context.text.front();
        if (is_whitespace(c)) { ++context.column; context.text.remove_prefix(1); }
        else if (is_newline(c)) { ++context.line; context.column = 1; context.text.remove_prefix(1); }
        else if (is_alphabetic(c)) { lex_identifier(context, tokens); }
        else if (is_digit(c)) { lex_number(context, tokens); }
        else if (c == '/') { lex_forward_slash(context, tokens); }
        else if (c == '\'') { emit_token(context, tokens, TokenId::single_quote, 1); }
        else if (c == '\\') { emit_token(context, tokens, TokenId::backwards_slash, 1); }
        else if (c == '"') { emit_token(context, tokens, TokenId::double_quote, 1); }
        else if (c == '.') { emit_token(context, tokens, TokenId::operator_dot, 1); }
        else if (c == '(') { emit_token(context, tokens, TokenId::open_parens, 1); }
        else if (c == ')') { emit_token(context, tokens, TokenId::close_parens, 1); }
        else if (c == '{') { emit_token(context, tokens, TokenId::open_curly_braces, 1); }
        else if (c == '}') { emit_token(context, tokens, TokenId::close_curly_braces, 1); }
        else if (c == '[') { emit_token(context, tokens, TokenId::open_square_braces, 1); }
        else if (c == ']') { emit_token(context, tokens, TokenId::close_square_braces, 1); }
        else if (c == '*') { emit_token(context, tokens, TokenId::operator_star, 1); }
        else if (c == ':') { emit_token(context, tokens, TokenId::colon, 1); }
        else if (c == ';') { emit_token(context, tokens, TokenId::semicolon, 1); }
        else { lex_unkwon(context); }
    }
    tokens.clear();
    return tokens;
}
