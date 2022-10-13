require dastest/testing_boost

require llvm/llvm_jit

[jit]
def jit_not ( a : bool )
    return !a

[jit]
def jit_xorxor ( a,b : bool )
    return a ^^ b

[jit]
def jit_set_xorxor ( var a : bool; b : bool )
    a ^^= b
    return a

[test]
def test_bool_arithmetics ( t:T? )
    t |> run("!") <| @ ( t : T? )
        t |> success ( is_jit_function(@@jit_not) )
        t |> equal ( true, jit_not(false) )
        t |> equal ( false, jit_not(true) )
    t |> run("^^ ^^=") <| @ ( t : T? )
        t |> success ( is_jit_function(@@jit_xorxor) )
        t |> equal ( false, jit_xorxor(false,false) )
        t |> equal ( true,  jit_xorxor(true, false) )
        t |> equal ( true,  jit_xorxor(false,true) )
        t |> equal ( false, jit_xorxor(true, true) )
        t |> success ( is_jit_function(@@jit_set_xorxor) )
        t |> equal ( false, jit_set_xorxor(false,false) )
        t |> equal ( true,  jit_set_xorxor(true, false) )
        t |> equal ( true,  jit_set_xorxor(false,true) )
        t |> equal ( false, jit_set_xorxor(true, true) )
