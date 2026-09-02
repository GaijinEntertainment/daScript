options gen2
require dastest/testing_boost

require UnitTest

[test]
def test_string_arg_never_null(t : T?) {
    t |> run("a null string arrives as empty") @(t : T?) {
        let nul_str : string
        assert(test_string_arg_length(nul_str) == 0l)
    }
    t |> run("an empty literal arrives as empty") @(t : T?) {
        let empty_literal = ""
        assert(test_string_arg_length(empty_literal) == 0l)
    }
    t |> run("a non-empty string is untouched") @(t : T?) {
        assert(test_string_arg_length("abc") == 3l)
    }
}
