require dastest/testing_boost

require llvm/llvm_jit
require jit

require daslib/math_boost
require UnitTest

[jit]
def test_point3_array_index( t:T?; arr:Point3Array)
    assert(arr |> length==10)
    for i in range(10)
        let p = arr[i]
        t |> equal(1.0, p.x)
        t |> equal(2.0, p.y)
        t |> equal(3.0, p.z)

[jit]
def test_point3_array_each( t:T?; arr:Point3Array)
    assert(arr |> length==10)
    for p in arr
        t |> equal(1.0, p.x)
        t |> equal(2.0, p.y)
        t |> equal(3.0, p.z)

[jit]
def test_point3_array_block( t:T?)
    testPoint3Array() <| $(arr)
        assert(arr |> length==10)
        for p in arr
            t |> equal(1.0, p.x)
            t |> equal(2.0, p.y)
            t |> equal(3.0, p.z)

[test]
def test_handle_type ( t:T? )
    t |> run("[]") <| @ ( t : T? )
        t |> success(is_jit_function(@@test_point3_array_index))
        testPoint3Array() <| $(arr)
            test_point3_array_index(t, arr)
    t |> run("each") <| @ ( t : T? )
        t |> success(is_jit_function(@@test_point3_array_each))
        testPoint3Array() <| $(arr)
            test_point3_array_each(t, arr)
    t |> run("invoke block") <| @ ( t : T? )
        t |> success(is_jit_function(@@test_point3_array_block))
        test_point3_array_block(t)
