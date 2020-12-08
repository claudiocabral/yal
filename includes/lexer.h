#pragma once

#include <string_view>
#include <tokens.h>

std::vector<akura::Token> lex(std::string_view text);
