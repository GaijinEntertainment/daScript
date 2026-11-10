#pragma once

#include "daScript/ast/ast.h"

namespace das {

    struct ExprInvoke;

    // src/ast/ARCHITECTURE.md#whole-program-facts
    struct WholeProgramFacts {
        das_hash_map<const ExprInvoke *, Function *> devirtualizedCalls;
        Function * devirtualizedCall ( const ExprInvoke * expr ) const {
            auto it = devirtualizedCalls.find(expr);
            return it != devirtualizedCalls.end() ? it->second : nullptr;
        }
    };

    void analyzeWholeProgram ( Program * program );

}
