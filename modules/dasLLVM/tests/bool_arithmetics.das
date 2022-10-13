require dastest/testing_boost

require llvm/llvm_jit

def jit_not ( a : bool )
    return !a

[test]
def test_bool_arithmetics ( t:T? )
    t |> run("!") <| @ ( t : T? )
        t |> equal ( true, jit_not(false) )
        t |> equal ( false, jit_not(true) )
