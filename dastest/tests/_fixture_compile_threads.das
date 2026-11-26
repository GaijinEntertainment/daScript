options gen2

require dastest/testing_boost public
require _fixture_compile_threads_macro

//! a fixture for uncategorized.das: compiles only under
//! --compile-threads 3

[test, three_compile_threads]
def test_compiled_on_three_threads(t : T?) {
    t |> success(true, "the compile ran with compile_threads 3")
}
