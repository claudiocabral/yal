#pragma once

#include <vector>
#include <tokens.h>

namespace akura {
    using AbstractSyntaxTree = int;
    AbstractSyntaxTree parse(std::vector<Token> &tokens);
};
