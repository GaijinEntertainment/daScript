#pragma once

#include "daScript/ast/ast.h"
#include "daScript/simulate/simulate.h"

namespace das {
    DAS_API SimNode * simulateExpression ( Context & context, Expression * expr );
    DAS_API SimNode * trySimulateExpression ( Context & context, const Expression * expr, uint32_t extraOffset, const TypeDeclPtr & r2vType );
    DAS_API SimNode * GetR2V ( Context & context, const LineInfo & at, const TypeDeclPtr & type, SimNode * expr );

    struct SimulateHooks {
        SimNode * (*simulateExpression) ( Context & context, Expression * expr );
        SimNode * (*trySimulateExpression) ( Context & context, const Expression * expr, uint32_t extraOffset, const TypeDeclPtr & r2vType );
        bool (*simulateProgram) ( Program * program, Context & context, TextWriter & logs, StackAllocator * sharedStack );
        void (*makeMacroModule) ( Program * program, TextWriter & logs );
        SimNode * (*makeFunctionSimNode) ( Function * fn, Context & context, const vector<ExpressionPtr> & arguments );
    };
    DAS_API extern SimulateHooks g_simulateHooks;
}
