require dastest/testing_boost

require llvm/llvm_jit

var g_A = 0
var g_B = 0.

struct Foo
    a : int
    b : float

[jit]
def test_invoke_block_jit(lmb:block<(a:int;b:float):int>)
    return invoke(lmb, 1, 2.0)

def test_invoke_block(lmb:block<(a:int;b:float):int>)
    return invoke(lmb, 1, 2.0)

def test_invoke_block_refval(lmb:block<(a:int;b:float):Foo>)
    return invoke(lmb, 1, 2.0)

[jit]
def test_make_block
    var local_C = 123
    let res = test_invoke_block <| $(a:int;b:float):int
        g_A = a
        g_B = b
        local_C = 456
        return 13
    return [[auto res, local_C]]

[jit]
def test_make_block_jit_invoke
    var local_C = 123
    let res = test_invoke_block_jit <| $(a:int;b:float):int
        g_A = a
        g_B = b
        local_C = 456
        return 13
    return [[auto res, local_C]]

[jit]
def test_make_block_return_refval
    let res = test_invoke_block_refval <| $(a:int;b:float):Foo
        g_A = a
        g_B = b
        return [[Foo a=a, b=b]]
    return res

[test]
def test_block ( t:T? )
    t |> run("make block") <| @ ( t : T? )
        verify(is_jit_function(@@test_make_block))
        g_A = 0
        g_B = 0.
        let res = test_make_block()
        t |> equal ( 13, res._0 )
        t |> equal ( 456, res._1 )
        t |> equal ( 1, g_A )
        t |> equal ( 2., g_B )
    t |> run("make block, invoke with jit") <| @ ( t : T? )
        verify(is_jit_function(@@test_make_block_jit_invoke))
        g_A = 0
        g_B = 0.
        let res = test_make_block_jit_invoke()
        t |> equal ( 13, res._0 )
        t |> equal ( 456, res._1 )
        t |> equal ( 1, g_A )
        t |> equal ( 2., g_B )
    t |> run("make block return refval") <| @ ( t : T? )
        verify(is_jit_function(@@test_make_block_return_refval))
        g_A = 0
        g_B = 0.
        let res = test_make_block_return_refval()
        t |> equal ( 1, res.a )
        t |> equal ( 2., res.b )
        t |> equal ( 1, g_A )
        t |> equal ( 2., g_B )
