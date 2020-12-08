#pragma once
#include <string_view>
#include <frozen/string.h>
#include <frozen/unordered_set.h>
#include <frozen/unordered_map.h>
#include <tokens.h>

namespace akura {
    using namespace std::literals;
    static constexpr auto keywords = frozen::make_unordered_map<frozen::string, akura::TokenId>({
            { "s64", TokenId::keyword_s64 },
            { "s32", TokenId::keyword_s32 },
            { "s16", TokenId::keyword_s16 },
            { "s8",  TokenId::keyword_s8 },
            { "u64", TokenId::keyword_u64 },
            { "u32", TokenId::keyword_u32 },
            { "u16", TokenId::keyword_u16 },
            { "u8",  TokenId::keyword_u8 },
            { "f64", TokenId::keyword_f64 },
            { "f32", TokenId::keyword_f32 },
            { "if",  TokenId::keyword_if },
            { "for", TokenId::keyword_for },
            { "end", TokenId::end }
        });
}
