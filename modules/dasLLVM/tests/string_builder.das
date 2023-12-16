require dastest/testing_boost

require jit

require strings

[jit]
def abc_jit ( a:int; b:float; c:string )
    return "a = {a} b = {b} c = {c}"

[test]
def test_ref ( t:T? )
    t |> run("string builder") <| @ ( t : T? )
        t |> success(is_jit_function(@@abc_jit))
        t |> equal(abc_jit(1,2.,"3"), "a = 1 b = 2.000000000 c = 3")


