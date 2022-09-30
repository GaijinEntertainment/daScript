require dastest/testing_boost

require llvm/llvm_jit

[jit]
def cmres_via_copy ( var a:int[10] )
    for i in range(10)
        a[i] = 13
    return a

[jit]
def cmres_alias
    var a:int[10]
    for i in range(10)
        a[i] = i
    return a


[test]
def test_cmres ( t:T? )
    t |> run("cmres via copy") <| @ ( t : T? )
        t |> success ( is_jit_function(@@cmres_via_copy) )
        var x : int[10]
        var b = cmres_via_copy(x)
        for xx in x
            t |> equal(xx, 13)
        for bb in b
            t  |> equal(bb, 13)
    t |> run("cmres alias") <| @ ( t : T? )
        t |> success ( is_jit_function(@@cmres_alias) )
        var x = cmres_alias()
        for xx,i in x,range(100500)
            t |> equal(xx, i)



