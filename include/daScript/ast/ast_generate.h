#pragma once

#include "daScript/ast/ast.h"

namespace das {

    // ExprVar, where name matches
    bool isExpressionVariable(const ExpressionPtr & expr, const string & name);         // only var
    bool isExpressionVariableDeref(const ExpressionPtr & expr, const string & name);    // r2v(var) or var
    // ExprConstPtr, where value is nullptr
    bool isExpressionNull(const ExpressionPtr & expr);

    // make sure generated code contains line information etc
    void verifyGenerated ( const ExpressionPtr & expr );

    /*
     def STRUCT_NAME
        return [[STRUCT_NAME field1=init1, field2=init2, ...]]
     */
    FunctionPtr makeConstructor ( Structure * str );

    /*
     def clone(a,b:STRUCT_NAME)
        a.f1 := b.f1
        a.f2 := b.f2
        ...
    */
    FunctionPtr makeClone ( Structure * str );

    /*
        delete var
     */
    ExpressionPtr makeDelete ( const VariablePtr & var );

    /*
        a->b(args) is short for invoke(a.b, a, args)
     */
    struct ExprInvoke;
    ExprInvoke * makeInvokeMethod ( const LineInfo & at, Expression * a, const string & b );

    /*
     struct __lambda_at_line_xxx
         __lambda = @lambda_fn
        (yield : int)
        capture_field_1
        capture_field_2
     */
    StructurePtr generateLambdaStruct ( const string & lambdaName, ExprBlock * block,
                                       const das_set<VariablePtr> & capt, bool needYield = false );

    /*
        lambda function, i.e.
         def __lambda_function_at_line_xxx(THIS:__lambda_at_line_xxx;...block_args...)
            with THIS
                ...block_body...
     */
    FunctionPtr generateLambdaFunction ( const string & lambdaName, ExprBlock * block,
                                        const StructurePtr & ls, bool needYield, bool isUnsafe );

    /*
         [[__lambda_at_line_xxx THIS=@__lambda_function_at_line_xxx; ba1=ba1; ba2=ba2; ... ]]
     */
    ExpressionPtr generateLambdaMakeStruct ( const StructurePtr & ls, const FunctionPtr & lf,
                                            const das_set<VariablePtr> & capt, const LineInfo & at );

    /*
         array comprehension [[ for x in src; x_expr; where x_expr ]]
         invoke() <| $()
             let temp : Array<expr->subexpr->type>
             for .....
                 if where ....
                     push(temp, subexpr)
             return temp
    */
    struct ExprArrayComprehension;
    ExpressionPtr generateComprehension ( ExprArrayComprehension * expr );

    /*
        replace reference with pointer
            var blah & = ....
            blah
        onto
            ...
            deref(blah)
     */
    void replaceRef2Ptr ( const ExpressionPtr & expr, const string & name );

    /*
        give variables in the scope of 'expr' block unique names
        only for the top-level block
     */
    void giveBlockVariablesUniqueNames  ( const ExpressionPtr & expr );

    /*
        replace break and continue of a particular loop
        with 'goto label bg' and 'goto label cg' accordingly
     */
    void replaceBreakAndContinue ( Expression * expr, int32_t bg, int32_t cg );

    /*
        replace
            yield A
        with
            result = A
            __yield = X
            return true
            label X
     */
    struct ExprYield;
    ExpressionPtr generateYield( ExprYield * expr, const FunctionPtr & func );

    /*
        replace
            let a = b
            let c
        with
            this.a = b
            memzero(c)
        if variable is & - swap to pointer
     */
    struct ExprLet;
    ExpressionPtr replaceGeneratorLet ( ExprLet * expr, const FunctionPtr & func, ExprBlock * scope );

    /*
        replace
            if cond
                if_true
            else
                if_false
        with
            if ( !cond ) goto else_label;
            if_true;
            goto end_label;
            else_label:
            if_false;
            end_label:

        replace
            if cond
                if_true
        with
            if ( !cond ) goto end_label;
            if_true;
            end_label:

     */
    struct ExprIfThenElse;
    ExpressionPtr replaceGeneratorIfThenElse ( ExprIfThenElse * expr, const FunctionPtr & func );

    /*
    replace
        while cond
            body
        finally
            finally
    with
        label beginloop                 continue -> goto beginloop
        if ! cond goto endloop          break -> goto endloop
        body
        goto beginloop
        label endloop
        finally
    */
    struct ExprWhile;
    ExpressionPtr replaceGeneratorWhile ( ExprWhile * expr, const FunctionPtr & func );

    /*
    replace
        for it0.. in src0..
            body
         finally
             finally
        with
            loop = true                                             continue -> goto midloop
            loop &= _builtin_iterator_first(it0,src0)               break -> goto endloop
            label beginloop
            if ! loop goto endloop
            body
            label midloop
            loop &= _builtin_iterator_next(it0,src0)
            goto beginloop
            label endloop
            finally
            _builtin_itersator_close(it0,src0) ...

     */
    struct ExprFor;
    ExpressionPtr replaceGeneratorFor ( ExprFor * expr, const FunctionPtr & func );
}

