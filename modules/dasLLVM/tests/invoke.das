require dastest/testing_boost

require llvm/llvm_jit

var g_A = 0
var g_B = 0.

def call_function ( a:int; b:float )
    g_A = a
    g_B = b
    return 13

[jit]
def test_invoke_call(fun:function<(a:int;b:float):int>)
    return invoke(fun, 1, 2.0)

[test]
def test_ref ( t:T? )
    t |> run("invoke call") <| @ ( t : T? )
        t |> success(is_jit_function(@@test_invoke_call))
        g_A = 0
        g_B = 0.
        t |> equal(13, test_invoke_call(@@call_function))
        t |> equal(1, g_A)
        t |> equal(2.0, g_B)


