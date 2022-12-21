require dastest/testing_boost

require llvm/llvm_jit


[jit,sideeffects]
def jit_assert(expr:bool)
    assert(expr,"we fail")

[test]
def test_assert ( t:T? )
    t |> success(is_jit_function(@@jit_assert))
    var fail = false
    try
        jit_assert(true)
    recover
        fail = true
    t |> success(!fail)
    try
        jit_assert(false)
    recover
        fail = true
    t |> success(fail)

[jit,sideeffects]
def debug_me(value)
    debug(value,"value=")

struct Foo
    a : int
    b : float

[test]
def test_debug ( t:T? )
    t |> success(is_jit_function(@@<(arg:int):void>debug_me))
    debug_me(13)
    t |> success(is_jit_function(@@<(arg:Foo):void>debug_me))
    debug_me([[Foo a=1,b=2.0]])


