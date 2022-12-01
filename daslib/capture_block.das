options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module capture_block shared private

require daslib/ast_boost
require daslib/templates_boost

[macro]
class CaptureBlock : AstVisitor
    [[do_not_delete]] scope : table<Variable?>
    [[do_not_delete]] capture : table<Variable?; ExprVar?>
    def override preVisitExprVar(expr:smart_ptr<ExprVar>) : void
        if !(expr.varFlags.local || expr.varFlags.argument || expr.varFlags._block)
            return
        let vptr = get_ptr(expr.variable)
        if !(scope |> key_exists(vptr))
            capture[vptr] = get_ptr(expr)
    def override preVisitExprLetVariable(expr:smart_ptr<ExprLet>;arg:VariablePtr;lastArg:bool) : void
        scope |> insert(get_ptr(arg))
    def override preVisitExprForVariable(expr:smart_ptr<ExprFor>;svar:VariablePtr;last:bool) : void
        scope |> insert(get_ptr(svar))
    def override preVisitExprBlockArgument(blk:smart_ptr<ExprBlock>;arg:VariablePtr;lastArg:bool): void
        scope |> insert(get_ptr(arg))

struct public CapturedVariable
    variable : Variable?
    expression : ExprVar?

[macro_function]
def public capture_block(expr:ExpressionPtr) : array<CapturedVariable>
    var astVisitor = new CaptureBlock()
    var adapter <- make_visitor(*astVisitor)
    visit(expr, adapter)
    var res <- [{for k,v in keys(astVisitor.capture),values(astVisitor.capture); [[CapturedVariable variable=k,expression=v]] }]
    adapter := null
    unsafe
        delete astVisitor
    return <- res
