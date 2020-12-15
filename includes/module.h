#pragma once
#include <unordered_map>
#include <string>

template <class T>
struct Alias { 
    explicit Alias(int value) : value(value) {};
    int value;
};

struct TypeIndexTag{};

using TypeIndex = Alias<TypeIndexTag>;

template <class T> using Map = std::unordered_map<std::string, T>;

namespace akura {
    struct Module {
        struct Type {
            Map<TypeIndex> members;
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
