require dastest/testing_boost public

require llvm/llvm_jit
require daslib/fuzzer
require daslib/faker


[jit_llvm]
def test_min2 ( a, b : int )
    return a < b ? a : b

[jit_llvm]
def test_min3 ( a, b, c : int )
    if a < b
        return a < c ? a : c
    else
        return b < c ? b : c

[jit_llvm]
def add_jit (a,b)
    return a + b

def test_add( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> add_jit) )
    t |> strictEqual ( a + b, add_jit(a,b) )

[test]
def test_apply_variant ( t:T? )
    t |> run("add") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            t |> test_add ( fake |> random_int,  fake |> random_int )
            t |> test_add ( fake |> random_uint, fake |> random_uint )
            t |> test_add ( fake |> random_float, fake |> random_float )
            t |> test_add ( fake |> random_double, fake |> random_double )
