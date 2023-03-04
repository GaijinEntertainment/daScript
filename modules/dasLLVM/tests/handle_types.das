require dastest/testing_boost

require llvm/llvm_jit
require jit

require daslib/math_boost
require UnitTest

[jit]
def handle_fun(var a:float4x4)
    return inverse(a)

[jit]
def handler_ref_op1(a:float4x4)
    var c : float4x4
    c = -a
    return c

[jit]
def handler_ref_op2(a,b:float4x4)
    var c : float4x4
    c = a * b
    return c

[jit]
def handler_val(var num:int&)
    var iid = make_invalid_id()
    num = int(iid)
    var iid2 = EntityId(13)
    return iid2

[test]
def handle_ref_op2 ( t:T? )
    t |> run("handle val") <| @ ( t : T? )
        t |> success(is_jit_function(@@handler_val))
        var num = 0
        var iid = handler_val(num)
        t |> equal(13, int(iid))
        t |> equal(-1, num)
    t |> run("handle op1") <| @ ( t : T? )
        t |> success(is_jit_function(@@handler_ref_op1))
        var a, b, c : float4x4
        identity(a)
        b = handler_ref_op1(a)
        identity(c)
        t |> equal(-c, b)
    t |> run("handle op2") <| @ ( t : T? )
        t |> success(is_jit_function(@@handler_ref_op2))
        var a, b, c, d : float4x4
        identity(a)
        identity(b)
        c = handler_ref_op2(a,b)
        identity(d)
        t |> equal(d, c)
    t |> run("handle fun") <| @ ( t : T? )
        t |> success(is_jit_function(@@handle_fun))
        var a : float4x4
        identity(a)
        a[1][1] = 2.0
        a[2][2] = 4.0
        a[3][0] = 1.0
        a[3][1] = 2.0
        a[3][2] = 4.0
        var b = handle_fun(a)
        var c = inverse(a)
        t |> equal(c, b)
