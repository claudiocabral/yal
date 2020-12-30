#pragma once
#include <unordered_map>
#include <string>
#include <ast_nodes.h>

template <class T> using Map = std::unordered_map<std::string, T>;
namespace akura {
    struct Module {
        struct Type {
            Map<int> members;
            int size;
        };
        struct Function {
            std::vector<Type> argument_list;
            std::vector<Type> return_list;
            std::vector<Expression> body;
        };
        struct Variable {
            Type type;
        };
        struct Identifier {
            union IdentifierPointer {
                void * unknown;
                Type * type;
                Function * function;
                Variable * variable;
                Module * module;
            };
            enum IdentifierTag {
                Unknown = 0,
                Type = 1,
                Function = 2,
                Variable = 3,
                Module = 4,
                end,
                size = end,
            };
            IdentifierPointer data = { nullptr };
            template <class T>
            Identifier & operator=(T *t) {
                if constexpr (std::is_same_v<T, Type>)
                    data.type = t | IdentifierTag::Type;
                else if constexpr (std::is_same_v<T, Function>)
                    data.function = t | IdentifierTag::Function;
                else if constexpr (std::is_same_v<T, Variable>)
                    data.variable = t | IdentifierTag::Variable;
                else if constexpr (std::is_same_v<T, Module>)
                    data.module = t | IdentifierTag::Module;
                else
                    static_assert(
                            !std::is_same_v<T, T>,
                            "T should be one of Type, Function, Variable or Module"
                            );

            }
        };
        struct Import {
            Map<Identifier> identifiers;
        };
        struct Export {
            Identifier value;
        };
        Map <Function> functions;
        Map <Type> types;
        Map <Variable> variables;
        Map <Import> imports;
        Map <Export> exports;
    };
}
