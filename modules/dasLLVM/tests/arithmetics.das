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

def add (a,b)
    return a + b

[jit_llvm]
def add_jit (a,b)
    return a + b

[test]
def test_apply_variant ( t:T? )
    t |> run("add") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            let left = fake |> random_int
            let right = fake |> random_int
            let expected = add(left,right)
            let actual = add_jit(left,right)
            t |> equal(expected, actual)
