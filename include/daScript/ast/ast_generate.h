#pragma once

#include "daScript/ast/ast.h"

namespace das {
    /*
     def STRUCT_NAME
        let t : STRUCT_TYPE
        t.field1 = init1
        t.field2 = init2
        return t
     */
    FunctionPtr makeConstructor ( Structure * str );

    /*
        delete var
     */
    ExpressionPtr makeDelete ( const VariablePtr & var );

    /*
        a->b(args) is short for invoke(a.b, a, args)
     */
    ExprInvoke * makeInvokeMethod ( const LineInfo & at, Expression * a, const string & b );

    /*
     struct __lambda_at_line_xxx
         __lambda = @lambda_fn
        capture_field_1
        capture_field_2
     */
    StructurePtr generateLambdaStruct ( ExprBlock * block, const set<VariablePtr> & capt );

    /*
        lambda function, i.e.
         def __lambda_function_at_line_xxx(THIS:__lambda_at_line_xxx;...block_args...)
            with THIS
                ...block_body...
     */
    FunctionPtr generateLambdaFunction ( ExprBlock * block, const StructurePtr & ls );

    /*
         [[__lambda_at_line_xxx THIS=@__lambda_function_at_line_xxx; ba1=ba1; ba2=ba2; ... ]]
     */
    ExpressionPtr generateLambdaMakeStruct ( const StructurePtr & ls, const FunctionPtr & lf, const set<VariablePtr> & capt );
}

