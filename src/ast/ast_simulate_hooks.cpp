#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"
#include "daScript/ast/ast_simulate.h"
#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/simulate.h"

namespace das
{
    // fusion function pointers (defined here in main lib, set by fusion lib)
    void (*g_fusionContextFn) ( Context & context, TextWriter & logs, bool enableFusion ) = nullptr;
    void (*g_resetFusionEngineFn) ( bool orphan ) = nullptr;

    // src/ast/ARCHITECTURE.md#simulate-compiler-lib
    SimulateHooks g_simulateHooks;

    SimNode * simulateExpression ( Context & context, Expression * expr ) {
        DAS_ASSERTF(g_simulateHooks.simulateExpression, "compiler library not loaded");
        return g_simulateHooks.simulateExpression(context, expr);
    }

    SimNode * trySimulateExpression ( Context & context, const Expression * expr, uint32_t extraOffset, const TypeDeclPtr & r2vType ) {
        DAS_ASSERTF(g_simulateHooks.trySimulateExpression, "compiler library not loaded");
        return g_simulateHooks.trySimulateExpression(context, expr, extraOffset, r2vType);
    }

    bool Program::simulate ( Context & context, TextWriter & logs, StackAllocator * sharedStack ) {
        DAS_ASSERTF(g_simulateHooks.simulateProgram, "compiler library not loaded");
        return g_simulateHooks.simulateProgram(this, context, logs, sharedStack);
    }

    void Program::makeMacroModule ( TextWriter & logs ) {
        DAS_ASSERTF(g_simulateHooks.makeMacroModule, "compiler library not loaded");
        g_simulateHooks.makeMacroModule(this, logs);
    }

    SimNode * GetR2V ( Context & context, const LineInfo & at, const TypeDeclPtr & type, SimNode * expr ) {
        if ( type->isRefType() ) {
            return expr;
        } else {
            return context.code->makeValueNode<SimNode_Ref2Value>(type->getR2VType(), at, expr);
        }
    }

    SimNode * Function::makeSimNode ( Context & context, const vector<ExpressionPtr> & arguments ) {
        DAS_ASSERTF(g_simulateHooks.makeFunctionSimNode, "compiler library not loaded");
        return g_simulateHooks.makeFunctionSimNode(this, context, arguments);
    }

    bool ExprInvoke::isCopyOrMove() const {
        auto blockT = arguments[0]->type;
        return blockT->firstType && blockT->firstType->isRefType() && !blockT->firstType->ref;
    }
}
