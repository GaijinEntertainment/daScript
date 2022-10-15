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

[jit]
def outOfRange ( var a : int[10]; index:int )
    return a[index]

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
    t |> run("out of range") <| @ ( t : T? )
        t |> success ( is_jit_function(@@outOfRange) )
        var x :int[10]
        var failed = false
        try
            var r = outOfRange(x,13)
            t |> success(r==123456) // code never gets here
        recover
            failed = true
        t |> success(failed, "out of range caused panic")
