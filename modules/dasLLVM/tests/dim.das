require dastest/testing_boost

require llvm/llvm_jit

[jit]
def makeAll13 ( var a:int[10] )
    for i in range(10)
        a[i] = 13

[jit]
def addAll ( var a : int[10] )
    var c = 0
    for i in range(10)
        c += a[i]
    return c

[test]
def test_dim ( t:T? )
    t |> run("array []") <| @ ( t : T? )
        var x : int[10]
        t |> success ( is_jit_function(@@makeAll13) )
        t |> success ( is_jit_function(@@addAll) )
        makeAll13(x)
        for i in x
            t |> equal(i, 13)
        t |> equal(addAll(x), 130)
