require dastest/testing_boost

require llvm/llvm_jit
require jit

[sideeffects,jit]
def test_const_r64
    return range64(1,2)

[sideeffects,jit]
def test_const_ur64
    return urange64(3,4)

[test]
def test_ref ( t:T? )
    t |> run("constants") <| @ ( t : T? )
        t |> equal ( range64(1,2), test_const_r64() )
        t |> equal ( urange64(3,4), test_const_ur64() )
