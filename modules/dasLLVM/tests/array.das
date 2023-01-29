require dastest/testing_boost

require llvm/llvm_jit
require jit

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

[jit]
def resizeLocked ( var a : array<int> )
    for _a,i in a,count()
        if i==3
            a |> resize(20)

[jit]
def outOfRange ( var a : array<int> )
    return a[13]

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
    t |> run("out of range") <| @ ( t : T? )
        t |> success ( is_jit_function(@@outOfRange) )
        var x : array<int>
        resize(x,10)
        var failed = false
        try
            var r = outOfRange(x)
            t |> success(r==123456) // code never gets here
        recover
            failed = true
        t |> success(failed, "out of range caused panic")
    t |> run("resize locked") <| @ ( t : T? )
        t |> success ( is_jit_function(@@resizeLocked) )
        var x : array<int>
        resize(x,10)
        var failed = false
        try
            resizeLocked(x)
        recover
            failed = true
        t |> success(failed, "resize caused panic")
