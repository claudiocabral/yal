#pragma once

namespace akura {
    enum class AstNodeId {
        module,
        function_declaration,
        type_declaration,
        expression,
        statement,
        if_statement,
        match_statement,
        for_statement,
        variable_declaration,
        variable_assingment,
        end
    };
}
