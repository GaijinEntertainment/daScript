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
    #define DAS_WEAK_STUB __declspec(selectany)
#else
    #define DAS_WEAK_STUB __attribute__((weak))
#endif

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

// simulate()
SIMULATE_STUB(ExprAddr)
SIMULATE_STUB(ExprArrayComprehension)
SIMULATE_STUB(ExprAscend)
SIMULATE_STUB(ExprAssert)
SIMULATE_STUB(ExprAssume)
SIMULATE_STUB(ExprAsVariant)
SIMULATE_STUB(ExprAt)
SIMULATE_STUB(ExprBlock)
SIMULATE_STUB(ExprBreak)
SIMULATE_STUB(ExprCall)
SIMULATE_STUB(ExprCast)
SIMULATE_STUB(ExprClone)
SIMULATE_STUB(ExprConst)
SIMULATE_STUB(ExprConstEnumeration)
SIMULATE_STUB(ExprConstString)
SIMULATE_STUB(ExprContinue)
SIMULATE_STUB(ExprCopy)
SIMULATE_STUB(ExprDebug)
SIMULATE_STUB(ExprDelete)
SIMULATE_STUB(ExprErase)
SIMULATE_STUB(ExprField)
SIMULATE_STUB(ExprFind)
SIMULATE_STUB(ExprFor)
SIMULATE_STUB(ExprGoto)
SIMULATE_STUB(ExprIfThenElse)
SIMULATE_STUB(ExprInvoke)
SIMULATE_STUB(ExprIs)
SIMULATE_STUB(ExprIsVariant)
SIMULATE_STUB(ExprKeyExists)
SIMULATE_STUB(ExprLabel)
SIMULATE_STUB(ExprLet)
SIMULATE_STUB(ExprMakeArray)
SIMULATE_STUB(ExprMakeBlock)
SIMULATE_STUB(ExprMakeGenerator)
SIMULATE_STUB(ExprMakeStruct)
SIMULATE_STUB(ExprMakeTuple)
SIMULATE_STUB(ExprMakeVariant)
SIMULATE_STUB(ExprMemZero)
SIMULATE_STUB(ExprMove)
SIMULATE_STUB(ExprNamedCall)
SIMULATE_STUB(ExprNew)
SIMULATE_STUB(ExprNullCoalescing)
SIMULATE_STUB(ExprOp1)
SIMULATE_STUB(ExprOp2)
SIMULATE_STUB(ExprOp3)
SIMULATE_STUB(ExprPtr2Ref)
SIMULATE_STUB(ExprQuote)
SIMULATE_STUB(ExprReader)
SIMULATE_STUB(ExprRef2Ptr)
SIMULATE_STUB(ExprRef2Value)
SIMULATE_STUB(ExprReturn)
SIMULATE_STUB(ExprSafeAsVariant)
SIMULATE_STUB(ExprSafeAt)
SIMULATE_STUB(ExprSafeField)
SIMULATE_STUB(ExprSetInsert)
SIMULATE_STUB(ExprStaticAssert)
SIMULATE_STUB(ExprStringBuilder)
SIMULATE_STUB(ExprSwizzle)
SIMULATE_STUB(ExprTag)
SIMULATE_STUB(ExprTryCatch)
SIMULATE_STUB(ExprTypeDecl)
SIMULATE_STUB(ExprTypeInfo)
SIMULATE_STUB(ExprUnsafe)
SIMULATE_STUB(ExprVar)
SIMULATE_STUB(ExprWhile)
SIMULATE_STUB(ExprWith)
SIMULATE_STUB(ExprYield)

// trySimulate()
TRY_SIMULATE_STUB(Expression)
TRY_SIMULATE_STUB(ExprAt)
TRY_SIMULATE_STUB(ExprCast)
TRY_SIMULATE_STUB(ExprField)
TRY_SIMULATE_STUB(ExprSafeAt)
TRY_SIMULATE_STUB(ExprSafeField)
TRY_SIMULATE_STUB(ExprSwizzle)
TRY_SIMULATE_STUB(ExprVar)

// simulateLocal()
SIMULATE_LOCAL_STUB(ExprMakeLocal)
SIMULATE_LOCAL_STUB(ExprMakeArray)
SIMULATE_LOCAL_STUB(ExprMakeStruct)
SIMULATE_LOCAL_STUB(ExprMakeTuple)
SIMULATE_LOCAL_STUB(ExprMakeVariant)

// setRefSp()
SET_REF_SP_STUB(ExprMakeLocal)
SET_REF_SP_STUB(ExprMakeArray)
SET_REF_SP_STUB(ExprMakeStruct)
SET_REF_SP_STUB(ExprMakeTuple)
SET_REF_SP_STUB(ExprMakeVariant)

// ---- SimNode base virtual methods ----
COPY_NODE_STUB(SimNode)
VISIT_STUB(SimNode)
DAS_WEAK_STUB char * SimNode::evalPtr(Context &) {
    DAS_FATAL_ERROR("SimNode::evalPtr() called on stub");
    return nullptr;
}
DAS_WEAK_STUB bool SimNode::evalBool(Context &) {
    DAS_FATAL_ERROR("SimNode::evalBool() called on stub");
    return false;
}
DAS_WEAK_STUB float SimNode::evalFloat(Context &) {
    DAS_FATAL_ERROR("SimNode::evalFloat() called on stub");
    return 0.0f;
}
DAS_WEAK_STUB double SimNode::evalDouble(Context &) {
    DAS_FATAL_ERROR("SimNode::evalDouble() called on stub");
    return 0.0;
}
DAS_WEAK_STUB int32_t SimNode::evalInt(Context &) {
    DAS_FATAL_ERROR("SimNode::evalInt() called on stub");
    return 0;
}
DAS_WEAK_STUB uint32_t SimNode::evalUInt(Context &) {
    DAS_FATAL_ERROR("SimNode::evalUInt() called on stub");
    return 0;
}
DAS_WEAK_STUB int64_t SimNode::evalInt64(Context &) {
    DAS_FATAL_ERROR("SimNode::evalInt64() called on stub");
    return 0;
}
DAS_WEAK_STUB uint64_t SimNode::evalUInt64(Context &) {
    DAS_FATAL_ERROR("SimNode::evalUInt64() called on stub");
    return 0;
}

// ---- SimNode_WithErrorMessage ----
COPY_NODE_STUB(SimNode_WithErrorMessage)

// ---- SimNode_CallBase ----
COPY_NODE_STUB(SimNode_CallBase)
DAS_WEAK_STUB void SimNode_CallBase::visitCall(SimVisitor &) {
    DAS_FATAL_ERROR("SimNode_CallBase::visitCall() called on stub");
}

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

// ---- SimNode_Delete ----
VISIT_STUB(SimNode_Delete)

// ---- SimNode_Ptr2Ref ----
VISIT_STUB(SimNode_Ptr2Ref)

// ---- SimNode_Zero ----
VISIT_STUB(SimNode_Zero)

// ---- SimNode_Op1 ----
SimNode * SimNode_Op1::visitOp1(SimVisitor &, const char *, int, const string &) {
    DAS_FATAL_ERROR("SimNode_Op1::visitOp1() called on stub");
    return nullptr;
}

// ---- SimNode_Op2 ----
SimNode * SimNode_Op2::visitOp2(SimVisitor &, const char *, int, const string &) {
    DAS_FATAL_ERROR("SimNode_Op2::visitOp2() called on stub");
    return nullptr;
}

// ---- SimNode_CallBase::visitOp ----
SimNode * SimNode_CallBase::visitOp1(SimVisitor &, const char *, int, const char *) {
    DAS_FATAL_ERROR("SimNode_CallBase::visitOp1() called on stub");
    return nullptr;
}
SimNode * SimNode_CallBase::visitOp2(SimVisitor &, const char *, int, const char *) {
    DAS_FATAL_ERROR("SimNode_CallBase::visitOp2() called on stub");
    return nullptr;
}
SimNode * SimNode_CallBase::visitOp3(SimVisitor &, const char *, int, const char *) {
    DAS_FATAL_ERROR("SimNode_CallBase::visitOp3() called on stub");
    return nullptr;
}

// ---- SimVisitor ----
void SimVisitor::sub(SimNode **, uint32_t, const char *) {
    DAS_FATAL_ERROR("SimVisitor::sub() called on stub");
}

// ---- PassVisitor ----
DAS_WEAK_STUB void PassVisitor::preVisitProgram(Program *) {}
DAS_WEAK_STUB void PassVisitor::visitProgram(Program *) {}
DAS_WEAK_STUB void PassVisitor::reportFolding() {}
DAS_WEAK_STUB void PassVisitor::preVisit(Function *) {}
DAS_WEAK_STUB FunctionPtr PassVisitor::visit(Function * fn) { return fn; }

// ---- Program stubs ----
DAS_WEAK_STUB bool Program::optimizationRefFolding(int32_t) { return false; }
DAS_WEAK_STUB bool Program::optimizationConstFolding(int32_t) { return false; }
DAS_WEAK_STUB bool Program::optimizationBlockFolding(int32_t) { return false; }
DAS_WEAK_STUB bool Program::optimizationCondFolding(int32_t) { return false; }
DAS_WEAK_STUB bool Program::optimizationUnused(TextWriter &, int32_t) { return false; }
DAS_WEAK_STUB bool Program::simulate(Context &, TextWriter &, StackAllocator *) { return false; }
DAS_WEAK_STUB void Program::checkSideEffects() {}
DAS_WEAK_STUB void Program::makeMacroModule(TextWriter &) {}

// ---- ExprInvoke ----
DAS_WEAK_STUB bool ExprInvoke::isCopyOrMove() const { return false; }

// ---- Function::makeSimNode ----
DAS_WEAK_STUB SimNode * Function::makeSimNode(Context &, const vector<ExpressionPtr> &) {
    DAS_FATAL_ERROR("makeSimNode() called on stub Function");
    return nullptr;
}

// ---- resetFusionEngine ----
DAS_WEAK_STUB void resetFusionEngine() {}

// ---- getSemanticHash ----
DAS_WEAK_STUB uint64_t getSemanticHash(SimNode *, Context *) { return 0; }

// ---- describeCppType ----
DAS_WEAK_STUB string describeCppType(const smart_ptr_raw<TypeDecl> &, CpptSubstitureRef, CpptSkipRef, CpptSkipConst, CpptRedundantConst, ChooseSmartPtr) { return ""; }

// ---- compileDaScript / parseDaScript ----
DAS_WEAK_STUB ProgramPtr compileDaScript(const string &, const smart_ptr<FileAccess> &, TextWriter &, ModuleGroup &, CodeOfPolicies) { return nullptr; }
DAS_WEAK_STUB ProgramPtr parseDaScript(const string &, const string &, const smart_ptr<FileAccess> &, TextWriter &, ModuleGroup &, bool, bool, CodeOfPolicies) { return nullptr; }

// ---- ExprRef2Value::GetR2V ----
DAS_WEAK_STUB SimNode * ExprRef2Value::GetR2V ( Context &, const LineInfo &, const TypeDeclPtr &, SimNode * ) {
    DAS_FATAL_ERROR("ExprRef2Value::GetR2V called on stub");
    return nullptr;
}

// ---- SimFunction::getLineInfo ----
DAS_WEAK_STUB const LineInfo * SimFunction::getLineInfo() const {
    return code ? &code->debugInfo : nullptr;
}

} // namespace das

//workaround compiler bug in MSVC 32 bit
#if defined(_MSC_VER) && !defined(__clang__) && INTPTR_MAX == INT32_MAX
VECTORCALL vec4i v_ldu_ptr(const void * a) {return v_seti_x((int32_t)a);}
#endif

