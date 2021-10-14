options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module ast_block_to_loop shared public

require daslib/ast_boost

class B2LVisitor : AstVisitor
    inClosure:int
    failOnReturn : bool
    replaceReturnWithContinue : bool
    def B2LVisitor(fOnR,rRetWC:bool)
        failOnReturn = fOnR
        replaceReturnWithContinue = rRetWC
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
                macro_error(compiling_program(),expr.at,"return is not allowed inside this macros's block")
            elif replaceReturnWithContinue
                return <- new [[ExprContinue() at=expr.at]]
        return expr
    def override visitExprBreak(expr:smart_ptr<ExprBreak>) : ExpressionPtr
        macro_error(compiling_program(),expr.at,"break is not allowed inside this macros's block")
        return expr
    def override visitExprContinue(expr:smart_ptr<ExprContinue>) : ExpressionPtr
        macro_error(compiling_program(),expr.at,"continue is not allowed inside this macros's block")
        return expr

[macro_function]
def public convert_block_to_loop(var blk:smart_ptr<Expression>; failOnReturn,replaceReturnWithContinue:bool )
    var astVisitor = new B2LVisitor(failOnReturn,replaceReturnWithContinue)
    var astVisitorAdapter <- make_visitor(*astVisitor)
    visit(blk, astVisitorAdapter)
    astVisitorAdapter := null
    unsafe
        delete astVisitor
