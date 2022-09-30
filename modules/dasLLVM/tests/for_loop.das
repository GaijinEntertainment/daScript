require dastest/testing_boost
require llvm/llvm_jit


[jit]
def range_for ( value:int )
    var i = 0
    for _ in range(0,value)
        i ++
    return i

[jit]
def range_for_range ( value:range )
    var i = 0
    for _ in value
        i ++
    return i

[jit]
def range_for_range_extra ( value:range )
    var i = 0
    for a,b in value,range(13,100500)
        i ++
    return i


[jit]
def range_for_with_break ( value,max_value:int )
    var i = 0
    for _ in range(0,value)
        if i >= max_value
            break
        i ++
    return i

[jit]
def range_for_with_continue ( value,max_value:int )
    var i = 0
    var j = 0
    for _ in range(0,value)
        i ++
        if i >= max_value
            continue
        j ++
    return j

[jit]
def range_for_with_finally ( value:int )
    var i = 0
    var tf = 0
    for _ in range(0,value)
        i ++
    finally
        tf ++
    return tf

[jit]
def dim_for ( var a:int[10] )
    for j in a
        j = 13

[jit]
def dim_and_range_for ( var a:int[10] )
    for i,j in range(5),a
        j = i

[test]
def test_for_loop ( t:T? )

    t |> run("range for") <| @ ( t : T? )
        t |> success ( is_jit_function ( @@range_for ) )
        t |> success ( is_jit_function ( @@range_for_range ) )
        t |> success ( is_jit_function ( @@range_for_range_extra ) )
        t |> success ( is_jit_function ( @@range_for_with_break ) )
        t |> success ( is_jit_function ( @@range_for_with_continue ) )
        t |> success ( is_jit_function ( @@range_for_with_finally ) )
        t |> equal ( range_for(10), 10 )
        t |> equal ( range_for_range(range(100,110)), 10 )
        t |> equal ( range_for_range_extra(range(100,110)), 10 )
        t |> equal ( range_for_with_break(10,5), 5 )
        t |> equal ( range_for_with_continue(10,5), 4 )
        t |> equal ( range_for_with_finally(10), 1 )
    t |> run("dim for") <| @ ( t : T? )
        t |> success ( is_jit_function ( @@dim_for ) )
        t |> success ( is_jit_function ( @@dim_and_range_for ) )
        var a : int[10]
        dim_for(a)
        for i in a
            t |> equal ( i, 13 )
        dim_and_range_for(a)
        for i,j in a,range(10)
            t |> equal ( i, j<5 ? j : 13 )



