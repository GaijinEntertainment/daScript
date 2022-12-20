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
