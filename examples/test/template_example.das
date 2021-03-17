options log = true

require ast
require daslib/ast_boost
require daslib/templates

[export]
def test

    var expr <- quote() <|
        x ++
        print("{x}\n")

    print(describe(expr))

    var rules = new Template()
    // rules->replaceVariable("x") <| ExpressionPtr(new [[ExprVar() name:="y"]])
    rules->renameVariable("x", "y")

    var res_expr <- apply_template(rules, expr)

    print(describe(res_expr))

    return true
