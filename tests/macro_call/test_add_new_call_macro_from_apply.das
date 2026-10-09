options gen2
require dastest/testing_boost
require _mint_macro

// [mints] fails this file's compile unless every annotation its apply registers is found in the compiling module
[mints]
def answer : int => 41

[test]
def test_a_call_macro_registered_from_a_function_annotation(t : T?) {
    t |> equal(answer() + answer_one(), 42)
}
