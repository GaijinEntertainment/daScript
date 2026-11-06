#pragma once

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"

namespace das {

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4100)
#elif defined(__EDG__)
#pragma diag_suppress 826
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

#define DAS_CONST_VISIT_EXPR_LIST(X) \
    X(ExprCallMacro) \
    X(ExprUnsafe) \
    X(ExprReader) \
    X(ExprLabel) \
    X(ExprGoto) \
    X(ExprRef2Value) \
    X(ExprRef2Ptr) \
    X(ExprPtr2Ref) \
    X(ExprAddr) \
    X(ExprNullCoalescing) \
    X(ExprAssert) \
    X(ExprStaticAssert) \
    X(ExprQuote) \
    X(ExprDebug) \
    X(ExprInvoke) \
    X(ExprErase) \
    X(ExprSetInsert) \
    X(ExprFind) \
    X(ExprKeyExists) \
    X(ExprAscend) \
    X(ExprCast) \
    X(ExprNew) \
    X(ExprDelete) \
    X(ExprAt) \
    X(ExprSafeAt) \
    X(ExprBlock) \
    X(ExprVar) \
    X(ExprSwizzle) \
    X(ExprField) \
    X(ExprSafeField) \
    X(ExprIsVariant) \
    X(ExprAsVariant) \
    X(ExprSafeAsVariant) \
    X(ExprOp1) \
    X(ExprOp2) \
    X(ExprOp3) \
    X(ExprCopy) \
    X(ExprMove) \
    X(ExprClone) \
    X(ExprTryCatch) \
    X(ExprReturn) \
    X(ExprYield) \
    X(ExprBreak) \
    X(ExprContinue) \
    X(ExprConst) \
    X(ExprFakeContext) \
    X(ExprFakeLineInfo) \
    X(ExprConstPtr) \
    X(ExprConstEnumeration) \
    X(ExprConstBitfield) \
    X(ExprConstInt8) \
    X(ExprConstInt16) \
    X(ExprConstInt64) \
    X(ExprConstInt) \
    X(ExprConstInt2) \
    X(ExprConstInt3) \
    X(ExprConstInt4) \
    X(ExprConstUInt8) \
    X(ExprConstUInt16) \
    X(ExprConstUInt64) \
    X(ExprConstUInt) \
    X(ExprConstUInt2) \
    X(ExprConstUInt3) \
    X(ExprConstUInt4) \
    X(ExprConstRange) \
    X(ExprConstURange) \
    X(ExprConstRange64) \
    X(ExprConstURange64) \
    X(ExprConstBool) \
    X(ExprConstFloat) \
    X(ExprConstFloat16) \
    X(ExprConstFloat2) \
    X(ExprConstFloat3) \
    X(ExprConstFloat4) \
    X(ExprConstString) \
    X(ExprConstDouble) \
    X(ExprLet) \
    X(ExprFor) \
    X(ExprLooksLikeCall) \
    X(ExprMakeBlock) \
    X(ExprMakeGenerator) \
    X(ExprTypeInfo) \
    X(ExprIs) \
    X(ExprCall) \
    X(ExprNamedCall) \
    X(ExprIfThenElse) \
    X(ExprWith) \
    X(ExprAssume) \
    X(ExprWhile) \
    X(ExprMakeStruct) \
    X(ExprMakeVariant) \
    X(ExprMakeArray) \
    X(ExprMakeTuple) \
    X(ExprArrayComprehension) \
    X(ExprMemZero) \
    X(ExprTypeDecl) \
    X(ExprTag)

    class ConstVisitor {
    protected:
        virtual ~ConstVisitor() {}
    public:
        virtual bool canVisitIfSubexpr ( const ExprIfThenElse * ) { return true; }
        virtual bool canVisitExpr ( const ExprTypeInfo * expr, const Expression * subexpr ) { return true; }
        virtual bool canVisitMakeStructure ( const ExprMakeStruct * expr ) { return true; }
        virtual bool canVisitMakeStructureBlock ( const ExprMakeStruct * expr, const Expression * blk ) { return true; }
        virtual bool canVisitMakeStructureBody ( const ExprMakeStruct * expr ) { return true; }
        virtual bool canVisitQuoteSubexpression ( const ExprQuote * ) { return false; }
        virtual bool canVisitWithAliasSubexpression ( const ExprAssume * ) { return false; }
        virtual bool canVisitMakeBlockBody ( const ExprMakeBlock * expr ) { return true; }
        virtual bool canVisitTryCatchBody ( const ExprTryCatch * expr ) { return true; }
        virtual bool canVisitMakeArray ( const ExprMakeArray * expr ) { return true; }
        virtual bool canVisitMakeArrayIndex ( const ExprMakeArray * expr, int index, const Expression * init, bool lastIndex ) { return true; }
        virtual bool canVisitCall ( const ExprCall * expr ) { return true; }
        virtual bool canVisitNamedCall ( const ExprNamedCall * expr ) { return true; }
        virtual bool canVisitLooksLikeCall ( const ExprLooksLikeCall * expr ) { return true; }
        virtual bool canVisitLabel ( const ExprLabel * ) { return true; }
        virtual bool canVisitReader ( const ExprReader * ) { return true; }
        virtual void preVisit ( const TypeDecl * td ) {}
        virtual void visit ( const TypeDecl * td ) {}
        virtual void preVisitExpression ( const Expression * expr ) {}
        virtual void visitExpression ( const Expression * expr ) {}
        virtual void preVisitBlockArgument ( const ExprBlock * block, const Variable * var, bool lastArg ) {}
        virtual void visitBlockArgument ( const ExprBlock * block, const Variable * var, bool lastArg ) {}
        virtual void preVisitBlockArgumentInit ( const ExprBlock * block, const Variable * var, const Expression * init ) {}
        virtual void visitBlockArgumentInit ( const ExprBlock * block, const Variable * var, const Expression * init ) {}
        virtual void preVisitBlockExpression ( const ExprBlock * block, const Expression * expr ) {}
        virtual void visitBlockExpression ( const ExprBlock * block, const Expression * expr ) {}
        virtual void preVisitBlockFinal ( const ExprBlock * block ) {}
        virtual void visitBlockFinal ( const ExprBlock * block ) {}
        virtual void preVisitBlockFinalExpression ( const ExprBlock * block, const Expression * expr ) {}
        virtual void visitBlockFinalExpression ( const ExprBlock * block, const Expression * expr ) {}
        virtual void preVisitLet ( const ExprLet * let, const Variable * var, bool last ) {}
        virtual void visitLet ( const ExprLet * let, const Variable * var, bool last ) {}
        virtual void preVisitLetInit ( const ExprLet * let, const Variable * var, const Expression * init ) {}
        virtual void visitLetInit ( const ExprLet * let, const Variable * var, const Expression * init ) {}
        virtual void preVisit ( const ExprStringBuilder * expr ) { preVisitExpression(expr); }
        virtual void preVisitStringBuilderElement ( const ExprStringBuilder * sb, const Expression * expr, bool last ) {}
        virtual void visitStringBuilderElement ( const ExprStringBuilder * sb, const Expression * expr, bool last ) {}
        virtual void visit ( const ExprStringBuilder * expr ) { visitExpression(expr); }
        virtual void preVisitNewArg ( const ExprNew * call, const Expression * arg, bool last ) {}
        virtual void visitNewArg ( const ExprNew * call, const Expression * arg, bool last ) {}
        virtual void preVisitNamedCallArg ( const ExprNamedCall * call, const MakeFieldDecl * arg, bool last ) {}
        virtual void visitNamedCallArg ( const ExprNamedCall * call, const MakeFieldDecl * arg, bool last ) {}
        virtual void preVisitCallArg ( const ExprCall * call, const Expression * arg, bool last ) {}
        virtual void visitCallArg ( const ExprCall * call, const Expression * arg, bool last ) {}
        virtual bool canVisitLooksLikeCallArg ( const ExprLooksLikeCall * call, const Expression * arg, bool last ) { return true; }
        virtual void preVisitLooksLikeCallArg ( const ExprLooksLikeCall * call, const Expression * arg, bool last ) {}
        virtual void visitLooksLikeCallArg ( const ExprLooksLikeCall * call, const Expression * arg, bool last ) {}
        virtual void preVisitNullCoaelescingDefault ( const ExprNullCoalescing *, const Expression * ) {}
        virtual void preVisitTagValue ( const ExprTag *, const Expression * ) {}
        virtual void preVisitAtIndex ( const ExprAt *, const Expression * ) {}
        virtual void preVisitSafeAtIndex ( const ExprSafeAt *, const Expression * ) {}
        virtual void preVisitType ( const ExprIs *, const TypeDecl * ) {}
        virtual void preVisitRight ( const ExprOp2 *, const Expression * ) {}
        virtual void preVisitLeft  ( const ExprOp3 *, const Expression * ) {}
        virtual void preVisitRight ( const ExprOp3 *, const Expression * ) {}
        virtual bool isRightFirst ( const ExprCopy * ) { return false; }
        virtual void preVisitRight ( const ExprCopy *, const Expression * ) {}
        virtual bool isRightFirst ( const ExprMove * ) { return false; }
        virtual void preVisitRight ( const ExprMove *, const Expression * ) {}
        virtual bool isRightFirst ( const ExprClone * ) { return false; }
        virtual void preVisitRight ( const ExprClone *, const Expression * ) {}
        virtual void preVisitWithBody ( const ExprWith *, const Expression * ) {}
        virtual void preVisitWhileBody ( const ExprWhile *, const Expression * ) {}
        virtual void preVisitCatch ( const ExprTryCatch *, const Expression * ) {}
        virtual void preVisitIfBlock ( const ExprIfThenElse *, const Expression * ) {}
        virtual void preVisitElseBlock ( const ExprIfThenElse *, const Expression * ) {}
        virtual void preVisitFor ( const ExprFor * expr, const Variable * var, bool last ) {}
        virtual void visitFor ( const ExprFor * expr, const Variable * var, bool last ) {}
        virtual void preVisitForStack ( const ExprFor * ) {}
        virtual void preVisitForBody ( const ExprFor *, const Expression * ) {}
        virtual void preVisitForSource ( const ExprFor *, const Expression *, bool ) {}
        virtual void visitForSource ( const ExprFor *, const Expression *, bool ) {}
        virtual void preVisitMakeVariantField ( const ExprMakeVariant * expr, int index, const MakeFieldDecl * decl, bool lastField ) {}
        virtual void visitMakeVariantField ( const ExprMakeVariant * expr, int index, const MakeFieldDecl * decl, bool lastField ) {}
        virtual void preVisitMakeStructureIndex ( const ExprMakeStruct * expr, int index, bool lastIndex ) {}
        virtual void visitMakeStructureIndex ( const ExprMakeStruct * expr, int index, bool lastField ) {}
        virtual void preVisitMakeStructureField ( const ExprMakeStruct * expr, int index, const MakeFieldDecl * decl, bool lastField ) {}
        virtual void visitMakeStructureField ( const ExprMakeStruct * expr, int index, const MakeFieldDecl * decl, bool lastField ) {}
        virtual void preVisitMakeStructureBlock ( const ExprMakeStruct * expr, const Expression * blk ) {}
        virtual void visitMakeStructureBlock ( const ExprMakeStruct * expr, const Expression * blk ) {}
        virtual void preVisitMakeArrayIndex ( const ExprMakeArray * expr, int index, const Expression * init, bool lastIndex ) {}
        virtual void visitMakeArrayIndex ( const ExprMakeArray * expr, int index, const Expression * init, bool lastField ) {}
        virtual void preVisitMakeTupleIndex ( const ExprMakeTuple * expr, int index, const Expression * init, bool lastIndex ) {}
        virtual void visitMakeTupleIndex ( const ExprMakeTuple * expr, int index, const Expression * init, bool lastField ) {}
        virtual void preVisitArrayComprehensionSubexpr ( const ExprArrayComprehension * expr, const Expression * subexpr ) {}
        virtual void preVisitArrayComprehensionWhere ( const ExprArrayComprehension * expr, const Expression * where ) {}
        virtual void preVisitDeleteSizeExpression ( const ExprDelete *, const Expression * ) {}
#define CONST_VISIT_EXPR(ExprType) \
        virtual void preVisit ( const ExprType * that ) { preVisitExpression(that); } \
        virtual void visit ( const ExprType * that ) { visitExpression(that); }
        DAS_CONST_VISIT_EXPR_LIST(CONST_VISIT_EXPR)
#undef CONST_VISIT_EXPR
    };
#if defined(_MSC_VER)
#pragma warning(pop)
#elif defined(__EDG__)
#pragma diag_default 826
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(__clang__)
#pragma clang diagnostic pop
#endif

    template <typename TT>
    void ExprLikeCall<TT>::visit ( ConstVisitor & vis ) const {
        vis.preVisit(static_cast<const TT *>(this));
        ExprLooksLikeCall::visit(vis);
        vis.visit(static_cast<const TT *>(this));
    }

    template <typename It, typename SimNodeT, bool first>
    void ExprTableKeysOrValues<It,SimNodeT,first>::visit ( ConstVisitor & vis ) const {
        vis.preVisit(static_cast<const It *>(this));
        ExprLooksLikeCall::visit(vis);
        vis.visit(static_cast<const It *>(this));
    }

    template <typename It, typename SimNodeT>
    void ExprArrayCallWithSizeOrIndex<It,SimNodeT>::visit ( ConstVisitor & vis ) const {
        vis.preVisit(static_cast<const It *>(this));
        ExprLooksLikeCall::visit(vis);
        vis.visit(static_cast<const It *>(this));
    }

    template <typename TT, typename ExprConstExt>
    void ExprConstT<TT,ExprConstExt>::visit ( ConstVisitor & vis ) const {
        vis.preVisit((const ExprConst *)this);
        vis.preVisit((const ExprConstExt *)this);
        vis.visit((const ExprConstExt *)this);
        vis.visit((const ExprConst *)this);
    }
}
