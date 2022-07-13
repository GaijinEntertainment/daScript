require daslib/templates_boost
require daslib/ast_boost
require daslib/strings

def test_fun
    var argT <- clone_type <| typeinfo(ast_typedecl type<int???>)
    var fun <- qmacro_function("add") <| $ ( a:$t(argT); b:float )
        return $i("a") + b
    print("\nFUNCTION REIFICATION IS:\n{describe(fun)}\n")

def test_regular
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
        if foo is $f(bar)
            print("hello!\n")
        let qq = @@$c(cll)
        let $i(bus) = "busbus"
    print(describe(blk))
    blk := null
    expr := null
    delete qqblk
    delete arguments
    delete aargs
    subtype := null

[export]
def main
    test_regular()
    test_fun()
