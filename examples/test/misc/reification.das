require daslib/templates_boost
require daslib/ast_boost
require daslib/strings

[export]
def main
    print("hello, world\n")
    let bus = "bus_variable_name"
    let car = "car_variable_name"
    let cll = "call_me_maybe"
    let bar = "barbar"
    var a1 = 1
    var a2 = a1 + 1
    var a3 = a1 + 13
    var expr <- quote(length(bus))
    var qqblk : array<ExpressionPtr>
    for i in range(3)
        qqblk |> emplace <| qmacro(print("{$v(i)}\n"))
    var arguments <- [{ExpressionPtr quote(a1); quote(a2)}]
    var aargs <- [{ExpressionPtr quote(a1); quote(a2); quote(a3)}]
    var subtype <- typeinfo(ast_typedecl type<int>)
    var blk <- qmacro <|
        let t = $i(bus)
        let q = $v([[auto a1,car]])
        let lb = $e(expr)
        print("{q}{t}{lb}\n")
        $b(qqblk)
        somefunnycall(1,$a(arguments),2)
        var aaa <- [[auto $a(aargs)]]
        var a : $t(subtype)
        $c(cll)(1,2,$a(arguments),3)
        foo.$f(bar) = 13
        foo?.$f("{bar}`bar") = 14
        foo.$f(bar).$f(bar) = 15
        foo as $f(bar) = 13
    print(describe(blk))
    blk := null
    expr := null
    delete qqblk
    delete arguments
    delete aargs
    subtype := null
