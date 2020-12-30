#pragma once

#include <string_view>
#include <tokens.h>
#include <channel.h>

using TokenList = std::vector<akura::Token>;

void lex(std::string_view text, std::string_view filename, TokenList & tokens);
