require dastest/testing_boost
require llvm/llvm_jit


[jit_llvm]
def range_for ( value:int )
    var i = 0
    for _ in range(0,value)
        i ++
    return i

[jit_llvm]
def range_for_range ( value:range )
    var i = 0
    for _ in value
        i ++
    return i

[jit_llvm]
def range_for_range_extra ( value:range )
    var i = 0
    for a,b in value,range(13,100500)
        i ++
    return i


[jit_llvm]
def range_for_with_break ( value,max_value:int )
    var i = 0
    for _ in range(0,value)
        if i >= max_value
            break
        i ++
    return i

[jit_llvm]
def range_for_with_continue ( value,max_value:int )
    var i = 0
    var j = 0
    for _ in range(0,value)
        i ++
        if i >= max_value
            continue
        j ++
    return j

[jit_llvm]
def range_for_with_finally ( value:int )
    var i = 0
    var tf = 0
    for _ in range(0,value)
        i ++
    finally
        tf ++
    return tf

[test]
def test_while_loop ( t:T? )

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



