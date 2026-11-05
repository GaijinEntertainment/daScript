#pragma once

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"

namespace das {

    Function * findBuiltinOperator ( const ModuleLibrary & library, const char * opName, TypeDecl * t0, TypeDecl * t1 = nullptr );
    ExpressionPtr makeTypedConst ( const LineInfo & at, TypeDecl * type, vec4f value );
    ExprOp1 * makeTypedOp1 ( const LineInfo & at, const char * op, Function * func, Expression * sub, TypeDecl * type );
    ExprVar * makeLocalRead ( const LineInfo & at, Variable * var );
    TypeDecl * inferLetInitType ( TypeDecl * autoT, TypeDecl * initT, bool viaClone, bool relaxedPointerConst );
    bool markSafeTableLookupInit ( Expression * init );
    Expression * makeTypeWitnessZero ( const LineInfo & at, TypeDecl * witnessT );
    void markUnderClone ( Expression * left );
    Expression * makeValue ( Expression * e );
    ExprOp1 * makeNot ( const LineInfo & at, Function * notFn, Expression * cond );
}
