options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module lint shared private

require daslib/ast_boost

[lint_macro]
class LintEverything : AstPassMacro
    def override apply ( prog:ProgramPtr; mod:Module? ) : bool
        paranoid(prog, true)
        return true

class LintVisitor : AstVisitor
    astVisitorAdapter : smart_ptr<ast::VisitorAdapter>
    exprForReturn : array<uint64>
    compile_time_errors : bool
    def LintVisitor()
        pass
    def lint_error ( text:string; at:LineInfo ) : void
        if compile_time_errors
            compiling_program() |> macro_error(at, text)
        else
            error("{text} at {describe(at)}\n")
    def override preVisitExprBlock(blk:smart_ptr<ExprBlock>) : void
        exprForReturn |> push(0ul)
    def override visitExprBlock(blk:smart_ptr<ExprBlock>) : ExpressionPtr
        exprForReturn |> pop()
        return blk
    def override visitExprBlockExpression(blk:smart_ptr<ExprBlock>;expr:ExpressionPtr) : ExpressionPtr
        let lb = exprForReturn |> back()
        if lb != 0ul
            let eb = intptr(get_ptr(expr))
            if lb != eb
                self->lint_error("unreachable code after return", expr.at)
        return expr
    def override preVisitExprReturn(expr:smart_ptr<ExprReturn>) : void
        exprForReturn |> pop()
        exprForReturn |> push(intptr(get_ptr(expr)))

def public paranoid ( prog:ProgramPtr; compile_time_errors : bool )
    var astVisitor = new [[LintVisitor() compile_time_errors=compile_time_errors]]
    unsafe
        astVisitor.astVisitorAdapter <- make_visitor(*astVisitor)
    visit ( prog, astVisitor.astVisitorAdapter )
    astVisitor.astVisitorAdapter := null
    unsafe
        delete astVisitor
