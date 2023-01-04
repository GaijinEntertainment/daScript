options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module collect_finally shared private

require daslib/ast_boost
require daslib/templates_boost

[macro]
class ColletFinally : AstVisitor
    [[do_not_delete]] blocks : table<ExprBlock?>
    def override preVisitExprBlock(blk:smart_ptr<ExprBlock>) : void
        if length(blk.finalList) > 0
            blocks |> insert(get_ptr(blk))
    def override canVisitMakeBlockBody(expr:smart_ptr<ExprMakeBlock>) : bool
        return false

[macro_function]
def public collect_finally(expr:ExpressionPtr)
    var astVisitor = new ColletFinally()
    var adapter <- make_visitor(*astVisitor)
    visit(expr, adapter)
    var res <- [{for k in keys(astVisitor.blocks); k }]
    adapter := null
    unsafe
        delete astVisitor
    return <- res
