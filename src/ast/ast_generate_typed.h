#pragma once

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"
#include "daScript/ast/ast_visitor.h"

namespace das {

    Function * findBuiltinOperator ( const ModuleLibrary & library, const char * opName, TypeDecl * t0, TypeDecl * t1 = nullptr );
    ExpressionPtr makeTypedConst ( const LineInfo & at, TypeDecl * type, vec4f value );
    ExprOp1 * makeTypedOp1 ( const LineInfo & at, const char * op, Function * func, Expression * sub, TypeDecl * type );
    ExprVar * makeLocalRead ( const LineInfo & at, Variable * var );
    TypeDecl * inferLetInitType ( TypeDecl * autoT, TypeDecl * initT, bool viaClone, bool relaxedPointerConst );
    bool markSafeTableLookupInit ( Expression * init );
    Expression * makeTypeWitnessZero ( const LineInfo & at, TypeDecl * witnessT );
    TypeDecl * makeStructCastType ( TypeDecl * subexprT, TypeDecl * castT );
    ExprCall * makeTypedCall ( const LineInfo & at, Function * func, vector<ExpressionPtr> && args );
    void markUnderClone ( Expression * left );
    void markScopesEarlyOut ( const vector<ExprBlock *> & scopes );
    void resetBlockFlow ( ExprBlock * block );
    void setLetVisibility ( ExprLet * let, ExprBlock * scope );
    void setLetEarlyOut ( ExprLet * let, bool hasEarlyOut );
    Expression * makeValue ( Expression * e );
    ExprConstBool * makeConstBool ( const LineInfo & at, bool value );
    ExprOp1 * makeNot ( const LineInfo & at, Function * notFn, Expression * cond );
    ExprIfThenElse * makeIf ( const LineInfo & at, Expression * cond, Expression * ifTrue, Expression * ifFalse );
    ExprCopy * makeCopy ( const LineInfo & at, Expression * left, Expression * right );
    ExprMove * makeMove ( const LineInfo & at, Expression * left, Expression * right );

    // replace one specific node (by identity) inside a statement
    class ReplaceNode : public Visitor {
    public:
        ReplaceNode ( Expression * w, const ExpressionPtr & r ) : what(w), with(r) {}
        Expression * what = nullptr;
        ExpressionPtr with = nullptr;
        bool done = false;
    protected:
        virtual ExpressionPtr visitExpression ( Expression * expr ) override {
            if ( expr==what ) { done = true; return with; }
            return Visitor::visitExpression(expr);
        }
    };
}
