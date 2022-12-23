options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module collect_labels shared private

require daslib/ast_boost
require daslib/templates_boost

[macro]
class ColletLabels : AstVisitor
    labels : table<int>
    def override preVisitExprLabel(expr:smart_ptr<ExprLabel>) : void
        labels |> insert(expr.labelName)

[macro_function]
def public collect_labels(expr:ExpressionPtr)
    var astVisitor = new ColletLabels()
    var adapter <- make_visitor(*astVisitor)
    visit(expr, adapter)
    var res <- [{for k in keys(astVisitor.labels); k }]
    adapter := null
    unsafe
        delete astVisitor
    return <- res
