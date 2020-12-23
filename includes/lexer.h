#pragma once

#include <string_view>
#include <tokens.h>

using TokenList = std::vector<akura::Token>;

void lex(std::string_view text, TokenList & tokens);
