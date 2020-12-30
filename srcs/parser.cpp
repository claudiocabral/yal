#include <lexer.h>
#include <parser.h>
#include <module.h>
#include <frozen/unordered_map.h>

namespace akura {
struct ParseContext {
    std::vector<akura::Token> const &tokens;
    akura::Token & current_token;
    int i = 0;
    akura::Module module;
    std::string_view filename;
};

using TokenHandler = bool (*) (ParseContext &context);

static auto & peek_next_token(ParseContext &context) {
    return context.tokens[context.i];
}

static auto advance_token(ParseContext &context) {
    context.current_token = context.tokens[context.i];
    ++context.i;
}


static auto & get_next_token(ParseContext &context) {
    auto & token = peek_next_token(context);
    advance_token(context);
    return token;
}

static auto rewind_tokens(ParseContext &context, int number_of_tokens = 1) {
    context.i -= number_of_tokens;
    context.current_token = context.tokens[context.i - 1];
}


static void report_parser_error(ParseContext & context, std::string_view message)
{
    auto & token = peek_next_token(context);
    auto line = akura::get_line(token.value, token.column);
    auto size = static_cast<int>(
            line[0].size()
            + line[1].size()
            + line[2].size()
            );
    fprintf(stderr, "%.*s,%d:%d: Error: %.*s.\n%.*s%.*s%.*s\n%*s\n",
            static_cast<int>(context.filename.size()),
            context.filename.data(),
            token.line,
            token.column,
            static_cast<int>(message.size()),
            message.data(),
            static_cast<int>(line[0].size()),
            line[0].data(),
            static_cast<int>(line[1].size()),
            line[1].data(),
            static_cast<int>(line[2].size()),
            line[2].data(),
            size,
            "^"
           );
    exit(1);
}

bool keyword_import(ParseContext &context) {
    auto & token = get_next_token(context);
    if (token.id != akura::TokenId::identifier) {
        rewind_tokens(context);
        report_parser_error(context, "Expected identifier or '{' after keyword import");
    }
    auto result = context.module.imports.insert(
            std::pair{ token.value, akura::Module::Import{} }
            );
    return true;
}

bool identifier(ParseContext &context) {
    auto begin = context.current_token;
    return true;
}

bool keyword_static(ParseContext &context) {
    return true;
}

bool end(ParseContext &context) {
    return false;
}

static constexpr auto token_handlers = frozen::make_unordered_map<akura::TokenId, TokenHandler>({
        { akura::TokenId::identifier, identifier },
        { akura::TokenId::keyword_import, keyword_import },
        { akura::TokenId::keyword_static, keyword_static },
        { akura::TokenId::end, end },
});

akura::AbstractSyntaxTree parse(TokenList const &tokens, std::string_view filename)
{
    akura::Token dummy{};
    akura::AbstractSyntaxTree tree{};
    auto context = ParseContext{ tokens, dummy, 0, akura::Module{}, filename};
    while (true)
    {
        auto & token = get_next_token(context);
        auto it = token_handlers.find(token.id);
        if (it == token_handlers.end())
            return tree; // error
        if (it->second(context))
            return tree; // done
    }
    return tree; // never reached
}
}
