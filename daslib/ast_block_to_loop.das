options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module ast_block_to_loop shared public

require daslib/ast_boost

class B2LVisitor : AstVisitor
    inClosure : int
    failOnReturn : bool
    replaceReturnWithContinue : bool
    requireContinueCond : bool
    def B2LVisitor(fOnR,rRetWC,rCC:bool)
        failOnReturn = fOnR
        replaceReturnWithContinue = rRetWC
        requireContinueCond = rCC
    def override preVisitExprBlock(blk:smart_ptr<ExprBlock>) : void
        if blk.blockFlags.isClosure
            inClosure ++
    def override visitExprBlock(blk:smart_ptr<ExprBlock>) : ExpressionPtr
        if blk.blockFlags.isClosure
            inClosure --
        return blk
    def override visitExprReturn(expr:smart_ptr<ExprReturn>) : ExpressionPtr
        if inClosure==0
            if failOnReturn
                return make_static_assert_false("return is not allowed inside this macros's block", expr.at)
            elif replaceReturnWithContinue
                if expr.subexpr != null
                    return  <- new [[ExprIfThenElse() at=expr.at,
                        cond <- clone_expression(expr.subexpr),
                        if_true <- new [[ExprReturn() at=expr.at,
                                subexpr <- new [[ExprConstBool() at=expr.at, value:=true]]
                            ]],
                        if_false <- new [[ExprContinue() at=expr.at]]
                    ]]
                else
                    if requireContinueCond
                        macro_error(compiling_program(),expr.at,"expecting return boolean (continue or stop)")
                    else
                        return <- new [[ExprContinue() at=expr.at]]
        return expr
    def override visitExprBreak(expr:smart_ptr<ExprBreak>) : ExpressionPtr
        return make_static_assert_false("break is not allowed inside this macros's block", expr.at)
    def override visitExprContinue(expr:smart_ptr<ExprContinue>) : ExpressionPtr
        return make_static_assert_false("continue is not allowed inside this macros's block", expr.at)

[macro_function]
def public convert_block_to_loop(var blk:smart_ptr<Expression>; failOnReturn,replaceReturnWithContinue,requireContinueCond:bool )
    var astVisitor = new B2LVisitor(failOnReturn,replaceReturnWithContinue,requireContinueCond)
    var astVisitorAdapter <- make_visitor(*astVisitor)
    visit(blk, astVisitorAdapter)
    astVisitorAdapter := null
    unsafe
        delete astVisitor

