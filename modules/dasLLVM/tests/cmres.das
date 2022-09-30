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

[jit]
def cmres_var_init_via_call
    var a = cmres_alias()
    return a

[jit]
def cmres_copy_local ( which:bool )
    var a : int[10]
    var b : int[10]
    a = cmres_alias()
    return which ? a : b

[test]
def test_cmres ( t:T? )
    t |> run("cmres via copy") <| @ ( t : T? )
        t |> success ( is_jit_function(@@cmres_via_copy) )
        var x : int[10]
        var b = cmres_via_copy(x)
        for xx in x
            t |> equal(13, xx)
        for bb in b
            t  |> equal(13, bb)
    t |> run("cmres alias") <| @ ( t : T? )
        t |> success ( is_jit_function(@@cmres_alias) )
        var x = cmres_alias()
        for xx,i in x,range(100500)
            t |> equal(i, xx)
    t |> run("cmres var init via call") <| @ ( t : T? )
        t |> success ( is_jit_function(@@cmres_var_init_via_call) )
        var x = cmres_var_init_via_call()
        for xx,i in x,range(100500)
            t |> equal(i, xx)
    t |> run("cmres copy local") <| @ ( t : T? )
        t |> success ( is_jit_function(@@cmres_copy_local) )
        var x = cmres_copy_local(true)
        for xx,i in x,range(100500)
            t |> equal(i, xx)

