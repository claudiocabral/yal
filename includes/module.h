#pragma once
#include <unordered_map>
#include <string>

namespace akura {
    struct Module {
        template <class T> using Map = std::unordered_map<std::string, T>;
        struct Type {
            Map<Type *> members;
            int size;
        };
        struct Function {
            std::vector<Type> argument_list;
            std::vector<Type> return_list;
        };
        struct Variable {
            Type type;
        };
        struct Export {
            union {
                Type * type;
                Function * function;
                Variable * variable;
            } value;
        };
        Map <Function> functions;
        Map <Type> types;
        Map <Variable> variables;
        Map <Export> exports;
    };
}
