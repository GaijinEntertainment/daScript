options gen2
options log = true

require ast
require daslib/ast_boost
require daslib/templates_boost

[export]
def test {

    var inscope expr <- quote() <| $() {
        x ++
        print("{x}\n")
    }

    print(describe(expr))

    var inscope rules : Template
    rules |> replaceVariable("x") <| new ExprVar(name := "y")
    rules |> renameVariable("x", "y")

    apply_template(rules, expr.at, expr)

    print(describe(expr))

    return true
}
