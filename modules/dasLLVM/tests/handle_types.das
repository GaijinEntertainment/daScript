require dastest/testing_boost

require llvm/llvm_jit
require jit

require daslib/math_boost

[jit]
def handler_ref_op2(a,b:float4x4)
    var c : float4x4
    c = a * b
    return c

[test]
def handle_ref_op2 ( t:T? )
    t |> run("handle op2") <| @ ( t : T? )
        t |> success(is_jit_function(@@handler_ref_op2))
        var a, b, c, d : float4x4
        identity(a)
        identity(b)
        c = handler_ref_op2(a,b)
        identity(d)
        t |> equal(d, c)
