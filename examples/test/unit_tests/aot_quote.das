options gen2

require ast
require daslib/templates_boost

def test_quote() {
    unsafe {
        var expr : ExpressionPtr <- quote() <| $() {
            x ++;
            print("{x}\n");
        }
        let str_expr : string = describe(expr);

        var rules : Template
        rules |> replaceVariable("x") <| new ExprVar(name := "y")
        rules |> renameVariable("x", "y")

        apply_template(rules, expr.at, expr)

        return (str_expr, describe(expr))
    }
}


[export]
def test {
    let (orig, modified) = test_quote();
    assert(orig == "$():auto
\{
\tx++;
\tprint(string_builder(x, \"\\n\"));
\}")
    assert(modified == "$():auto
\{
\ty++;
\tprint(string_builder(y, \"\\n\"));
\}")
    return true
}
