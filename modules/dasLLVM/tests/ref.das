require dastest/testing_boost

require llvm/llvm_jit

[jit]
def inc_local_ref ( var a : int& )
    unsafe
        var b & = a
        return b++

[jit]
def inc_arg_ref ( var a : int& )
    return a++

[test]
def test_ref ( t:T? )
    t |> run("local ref") <| @ ( t : T? )
        var a = 1
        let b = inc_local_ref(a)
        t |> equal(a, 2)
        t |> equal(b, 1)
        t |> success(is_jit_function(@@inc_local_ref))
    t |> run("arg ref") <| @ ( t : T? )
        var a = 1
        let b = inc_arg_ref(a)
        t |> equal(a, 2)
        t |> equal(b, 1)
        t |> success(is_jit_function(@@inc_arg_ref))
