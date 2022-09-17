require dastest/testing_boost

require llvm/llvm_jit
require daslib/fuzzer
require daslib/faker

require daslib/constant_expression

[jit_llvm, sideeffects]
def test_min ( a, b : int )
    return a < b ? a : b

[jit_llvm, sideeffects]
def test_min ( a, b, c : int )
    if a < b
        return a < c ? a : c
    else
        return b < c ? b : c

[test]
def test_phi ( t:T? )

    t |> success ( is_jit_function(@@<(a:int;b:int):bool> test_min) )
    t |> success ( is_jit_function(@@<(a:int;b:int;c:int):bool> test_min) )

    t |> equal(1, test_min(1,2))
    t |> equal(1, test_min(2,1))
    t |> equal(1, test_min(1,2,3))
    t |> equal(1, test_min(1,3,2))
    t |> equal(1, test_min(2,1,3))
    t |> equal(1, test_min(2,3,1))
    t |> equal(1, test_min(3,1,2))
    t |> equal(1, test_min(3,2,1))

    t |> run("min2") <| @ ( t : T? )
        var fake <- Faker()
        var a = fake |> random_int
        var b = fake |> random_int
        t |> equal(test_min(a,b), a<b ? a : b)

    t |> run("min3") <| @ ( t : T? )
        var fake <- Faker()
        var a = fake |> random_int
        var b = fake |> random_int
        var c = fake |> random_int
        t |> equal(test_min(a,b,c), a<b ? (a<c ? a : c) : (b<c ? b : c))


[export]
def main
    print("min(1,2) = {test_min(1,2)}\n")
    print("min(2,1) = {test_min(2,1)}\n")

    print("min(1,2,3) = {test_min(1,2,3)}\n")
    print("min(1,3,2) = {test_min(1,3,2)}\n")
    print("min(2,1,3) = {test_min(2,1,3)}\n")
    print("min(2,3,1) = {test_min(2,3,1)}\n")
    print("min(3,1,2) = {test_min(3,1,2)}\n")
    print("min(3,2,1) = {test_min(3,2,1)}\n")