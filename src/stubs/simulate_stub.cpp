// Stub implementations for Expression::simulate() and related virtual methods.
// These exist only in the runtime lib where simulate is never called.
// If called, they abort — the real implementations live in libDaScript.
// Free-function stubs are marked weak so that the real symbols from libDaScript
// take precedence when both libraries are loaded in the same process.

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/ast/ast_typefactory.h"
#include "daScript/simulate/simulate_nodes.h"
#include "daScript/simulate/runtime_string_delete.h"

#if defined(_MSC_VER)
    #define DAS_WEAK_STUB
#else
    #define DAS_WEAK_STUB __attribute__((weak))
#endif

// Generic stub: panics with function name, returns default-constructed value (or nothing for void).
#define DAS_DEFINE_STUB(ret, name, ...) \
    DAS_WEAK_STUB ret name(__VA_ARGS__) { \
        DAS_FATAL_ERROR(#name " called on runtime stub"); \
        if constexpr (!is_void_v<ret>) { return {}; } \
    }

#define DAS_DEFINE_VOID_STUB(name, ...) \
    DAS_WEAK_STUB void name(__VA_ARGS__) { \
        DAS_FATAL_ERROR(#name " called on runtime stub"); \
    }

#define SIMULATE_STUB(Klass) \
    DAS_WEAK_STUB SimNode * Klass::simulate(Context &) const { \
        DAS_FATAL_ERROR("simulate() called on stub " #Klass); \
        return nullptr; \
    }

#define TRY_SIMULATE_STUB(Klass) \
    DAS_WEAK_STUB SimNode * Klass::trySimulate(Context &, uint32_t, const TypeDeclPtr &) const { \
        DAS_FATAL_ERROR("trySimulate() called on stub " #Klass); \
        return nullptr; \
    }

#define SIMULATE_LOCAL_STUB(Klass) \
    DAS_WEAK_STUB vector<SimNode *> Klass::simulateLocal(Context &) const { \
        DAS_FATAL_ERROR("simulateLocal() called on stub " #Klass); \
        return {nullptr}; \
    }

#define SET_REF_SP_STUB(Klass) \
    DAS_WEAK_STUB void Klass::setRefSp(bool, bool, uint32_t, uint32_t) { \
        DAS_FATAL_ERROR("setRefSp() called on stub " #Klass); \
    }

#define VISIT_STUB(Klass) \
    DAS_WEAK_STUB SimNode * Klass::visit(SimVisitor &) { \
        DAS_FATAL_ERROR("visit() called on stub " #Klass); \
        return nullptr; \
    }

#define EVAL_STUB(Klass) \
    DAS_WEAK_STUB vec4f Klass::eval(Context &) { \
        DAS_FATAL_ERROR("eval() called on stub " #Klass); \
        return v_zero(); \
    }

#define COPY_NODE_STUB(Klass) \
    DAS_WEAK_STUB SimNode * Klass::copyNode(Context &, NodeAllocator *) { \
        DAS_FATAL_ERROR("copyNode() called on stub " #Klass); \
        return nullptr; \
    }

namespace das {

// setRefSp()
SET_REF_SP_STUB(ExprMakeLocal)
SET_REF_SP_STUB(ExprMakeArray)
SET_REF_SP_STUB(ExprMakeStruct)
SET_REF_SP_STUB(ExprMakeTuple)
SET_REF_SP_STUB(ExprMakeVariant)

// ---- SimNode base virtual methods ----
COPY_NODE_STUB(SimNode)
VISIT_STUB(SimNode)
DAS_DEFINE_STUB(char *, SimNode::evalPtr, Context &)
DAS_DEFINE_STUB(bool, SimNode::evalBool, Context &)
DAS_DEFINE_STUB(float, SimNode::evalFloat, Context &)
DAS_DEFINE_STUB(double, SimNode::evalDouble, Context &)
DAS_DEFINE_STUB(int32_t, SimNode::evalInt, Context &)
DAS_DEFINE_STUB(uint32_t, SimNode::evalUInt, Context &)
DAS_DEFINE_STUB(int64_t, SimNode::evalInt64, Context &)
DAS_DEFINE_STUB(uint64_t, SimNode::evalUInt64, Context &)

// ---- SimNode_WithErrorMessage ----
COPY_NODE_STUB(SimNode_WithErrorMessage)

// ---- SimNode_CallBase ----
COPY_NODE_STUB(SimNode_CallBase)
DAS_DEFINE_VOID_STUB(SimNode_CallBase::visitCall, SimVisitor &)

// ---- SimNode_CopyRefValue ----
VISIT_STUB(SimNode_CopyRefValue)
EVAL_STUB(SimNode_CopyRefValue)

// ---- SimNode_TypeInfo ----
VISIT_STUB(SimNode_TypeInfo)

// ---- SimNode_ClosureBlock ----
VISIT_STUB(SimNode_ClosureBlock)
EVAL_STUB(SimNode_ClosureBlock)

// ---- SimNode_Final ----
COPY_NODE_STUB(SimNode_Final)
VISIT_STUB(SimNode_Final)

// ---- SimNode_Block ----
COPY_NODE_STUB(SimNode_Block)
VISIT_STUB(SimNode_Block)
EVAL_STUB(SimNode_Block)

// ---- SimNode_BlockNF ----
EVAL_STUB(SimNode_BlockNF)

// ---- SimNode_BlockWithLabels ----
VISIT_STUB(SimNode_BlockWithLabels)
EVAL_STUB(SimNode_BlockWithLabels)

// ---- SimNode_ForBase ----
COPY_NODE_STUB(SimNode_ForBase)

// ---- SimNode_KeepAlive ----
#if DAS_ENABLE_KEEPALIVE
VISIT_STUB(SimNode_KeepAlive)
#endif

// ---- SimNode_At ----
VISIT_STUB(SimNode_At)

// ---- SimNode_SafeAt ----
VISIT_STUB(SimNode_SafeAt)

// ---- SimNode_Delete ----
VISIT_STUB(SimNode_Delete)

// ---- SimNode_Ptr2Ref ----
VISIT_STUB(SimNode_Ptr2Ref)

// ---- SimNode_Zero ----
VISIT_STUB(SimNode_Zero)

// ---- SimNode_Op1 ----
DAS_DEFINE_STUB(SimNode *, SimNode_Op1::visitOp1, SimVisitor &, const char *, int, const string &)

// ---- SimNode_Op2 ----
DAS_DEFINE_STUB(SimNode *, SimNode_Op2::visitOp2, SimVisitor &, const char *, int, const string &)

// ---- SimNode_CallBase::visitOp ----
DAS_DEFINE_STUB(SimNode *, SimNode_CallBase::visitOp1, SimVisitor &, const char *, int, const char *)
DAS_DEFINE_STUB(SimNode *, SimNode_CallBase::visitOp2, SimVisitor &, const char *, int, const char *)
DAS_DEFINE_STUB(SimNode *, SimNode_CallBase::visitOp3, SimVisitor &, const char *, int, const char *)

// ---- SimVisitor ----
DAS_DEFINE_VOID_STUB(SimVisitor::sub, SimNode **, uint32_t, const char *)

// ---- Program stubs ----
DAS_DEFINE_STUB(bool, Program::optimizationRefFolding, int32_t)
DAS_DEFINE_STUB(bool, Program::optimizationConstFolding, int32_t)
DAS_DEFINE_STUB(bool, Program::optimizationBlockFolding, int32_t)
DAS_DEFINE_STUB(bool, Program::optimizationCondFolding, int32_t)
DAS_DEFINE_STUB(bool, Program::optimizationUnused, TextWriter &, int32_t)
DAS_DEFINE_STUB(bool, Program::simulate, Context &, TextWriter &, StackAllocator *)
DAS_DEFINE_VOID_STUB(Program::checkSideEffects)
DAS_DEFINE_VOID_STUB(Program::makeMacroModule, TextWriter &)

// ---- ExprInvoke ----
DAS_WEAK_STUB bool ExprInvoke::isCopyOrMove() const {
    DAS_FATAL_ERROR("ExprInvoke::isCopyOrMove called on runtime stub");
    return {};
}

// ---- Function::makeSimNode ----
DAS_DEFINE_STUB(SimNode *, Function::makeSimNode, Context &, const vector<ExpressionPtr> &)

// ---- resetFusionEngine ----
DAS_DEFINE_VOID_STUB(resetFusionEngine)

// ---- compileDaScript / parseDaScript ----
DAS_DEFINE_STUB(ProgramPtr, compileDaScript, const string &, const smart_ptr<FileAccess> &, TextWriter &, ModuleGroup &, CodeOfPolicies)
DAS_DEFINE_STUB(ProgramPtr, parseDaScript, const string &, const string &, const smart_ptr<FileAccess> &, TextWriter &, ModuleGroup &, bool, bool, CodeOfPolicies)

// ---- typeFactory stubs (defined in module_builtin_rtti.cpp, needed by dynamic modules using Context&) ----
DAS_DEFINE_VOID_STUB(dapiStackWalk,  StackWalkerPtr walker, Context & context, const LineInfo & at )

// ---- SimFunction::getLineInfo ----
DAS_WEAK_STUB const LineInfo * SimFunction::getLineInfo() const {
    return code ? &code->debugInfo : nullptr;
}

} // namespace das

//workaround compiler bug in MSVC 32 bit
#if defined(_MSC_VER) && !defined(__clang__) && INTPTR_MAX == INT32_MAX
VECTORCALL vec4i v_ldu_ptr(const void * a) {return v_seti_x((int32_t)a);}
#endif
