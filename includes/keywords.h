#pragma once
#include <string_view>
#include <frozen/string.h>
#include <frozen/unordered_set.h>
#include <frozen/unordered_map.h>
#include <tokens.h>

namespace frozen {
template <> struct elsa<std::string_view> {
  constexpr std::size_t operator()(std::string_view value) const {
    std::size_t d = 5381;
    for (std::size_t i = 0; i < value.size(); ++i)
      d = d * 33 + value[i];
    return d;
  }
  // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
  // With the lowest bits removed, based on experimental setup.
  constexpr std::size_t operator()(std::string_view value, std::size_t seed) const {
    std::size_t d =  (0x811c9dc5 ^ seed) * 0x01000193;
    for (std::size_t i = 0; i < value.size(); ++i)
      d = (d ^ value[i]) * 0x01000193;
    return d >> 8 ;
  }
};
}

template <class Key, class Type, size_t size>
constexpr auto make_reverse_map(const frozen::unordered_map<Type, Key, size> & map) {
    std::pair<Key, Type> arr[size];
    int i = 0;
    for (auto elem : map) {
        arr[i].first = elem.second;
        arr[i].second = elem.first;
        ++i;
    }
    return frozen::make_unordered_map(arr);
}

namespace akura {
    static constexpr auto keywords = frozen::make_unordered_map<std::string_view, akura::TokenId>({
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
            { "match", TokenId::keyword_match },
            { "static", TokenId::keyword_static },
            { "import", TokenId::keyword_import },
            { "export", TokenId::keyword_export },
            { "end", TokenId::end }
        });
    constexpr auto reverse_keywords = make_reverse_map(keywords);
}
