require dastest/testing_boost

require llvm/llvm_jit

[jit]
def makeAll13 ( var a:array<int> )
    for i in range(length(a))
        a[i] = 13

[jit]
def addAll ( var a : array<int> )
    var c = 0
    for i in range(length(a))
        c += a[i]
    return c

[test]
def test_array ( t:T? )
    t |> run("array []") <| @ ( t : T? )
        var x : array<int>
        resize(x,10)
        t |> success ( is_jit_function(@@makeAll13) )
        t |> success ( is_jit_function(@@addAll) )
        makeAll13(x)
        for i in x
            t |> equal(i, 13)
        t |> equal(addAll(x), 130)
