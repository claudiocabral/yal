#include <parser.h>
#include <module.h>
#include <frozen/unordered_map.h>

struct ParseContext {
    std::vector<akura::Token> const &tokens;
    int current_token = 0;
};

using TokenHandler = bool (*) (ParseContext &context);

auto & get_next_token(ParseContext &context)
{
    auto & token = context.tokens[context.current_token]; 
    ++context.current_token;
    return token;
}


bool keyword_import(ParseContext &context) {
    return true;
}

bool end(ParseContext &context) {
    return false;
}

static constexpr auto token_handlers = frozen::make_unordered_map<akura::TokenId, TokenHandler>({
        { akura::TokenId::identifier, end },
        { akura::TokenId::keyword_import, end },
        { akura::TokenId::keyword_static, end },
        { akura::TokenId::single_line_comment, end },
        { akura::TokenId::end, end },
});

akura::AbstractSyntaxTree parse(std::vector<akura::Token> const &tokens)
{
    auto context = ParseContext{ tokens, 0 };
    while (true)
    {
        auto & token = get_next_token(context);
        auto it = token_handlers.find(token.id);
        if (it == token_handlers.end())
            return -1;
        if (it->second(context))
            return 0;
    }
    return 0;
}
