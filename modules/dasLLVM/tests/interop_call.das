options gen2
require dastest/testing_boost

require jit

[jit]
def print_me(foo : int) {
    return sprint(foo, print_flags.singleLine)
}

[test]
def test_ref(t : T?) {
    t |> run("interop call") <| @(t : T?) {
        t |> success(is_jit_function(@@print_me))
        t |> equal(print_me(13), "int const = 13")
    }
}

