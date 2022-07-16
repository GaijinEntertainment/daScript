require daslib/templates_boost
require daslib/ast_boost
require daslib/strings
require daslib/defer

def test_fun_arg
    var foo <- [{VariablePtr
        new [[Variable() name:="v1", _type<-qmacro_type(type<int>)]];
        new [[Variable() name:="v2", _type<-qmacro_type(type<float>), init<-qmacro(1.2)]]
    }]; defer_delete(foo)
    var fun <- qmacro_function("add") <| $ ( a: int; $a(foo); b : int )
        return a + b
    print("\nFUNCTION ARGUMENTS ARE:\n{describe(fun)}\n")
    delete fun

def test_var
    var tt <- typeinfo(ast_typedecl type<float>)
    var vvar <- qmacro_variable("foo",type<$t(tt)?>); defer_delete(vvar)
    print("\nVARIABLE IS:\n{describe(vvar)}\n")

def test_type
    var argT <- typeinfo(ast_typedecl type<int>)
    var resT <- qmacro_type(type<$t(argT)???>); defer_delete(resT)
    print("\nTYPE IS:\n{describe(resT)}\n")

def test_fun
    var argT <- typeinfo(ast_typedecl type<int???>)
    var fun <- qmacro_function("add") <| $ ( a:$t(argT); b:float )
        return $i("a") + b
    print("\nFUNCTION REIFICATION IS:\n{describe(fun)}\n")
    delete fun

def test_regular
    let bus = "bus_variable_name"
    let car = "car_variable_name"
    let cll = "call_me_maybe"
    let bar = "barbar"
    var a1 = 1
    var a2 = a1 + 1
    var a3 = a1 + 13
    var expr <- quote(length(bus)); defer_delete(expr)
    var qqblk : array<ExpressionPtr>; defer_delete(qqblk)
    for i in range(3)
        qqblk |> emplace <| qmacro(print("{$v(i)}\n"))
    var arguments <- [{ExpressionPtr quote(a1); quote(a2)}]; defer_delete(arguments)
    var aargs <- [{ExpressionPtr quote(a1); quote(a2); quote(a3)}]; defer_delete(aargs)
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
    delete blk

[export]
def main
    test_regular()
    test_fun()
    test_type()
    test_var()
    test_fun_arg()

