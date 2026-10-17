// A keyword in a name position reports the reserved word, not bison's
// bare token dump alone.
options gen2
options no_aot     // invokes the compiler at runtime (compile_file) - not AOT-linkable

require daslib/ast
require strings
require dastest/testing_boost public
require _compile_issues



[test]
def test_reserved_word_as_parameter_name(t : T?) {
    t |> run("a keyword as a parameter name is named as the reserved word it is") @(t : T?) {
        let issues = compile_issues("{get_das_root()}/tests/typer_errors/_reserved_word_name.das")
        t |> success(find(issues, "unexpected class") >= 0,
            "the parser's own token dump stays: {issues}")
        t |> success(find(issues, "'class' is a reserved word and cannot be used as a name") >= 0,
            "and the note names the word")
    }
}

[test]
def test_identifier_out_of_place_draws_no_note(t : T?) {
    t |> run("an identifier out of place is not called a reserved word") @(t : T?) {
        let issues = compile_issues("{get_das_root()}/tests/typer_errors/_unexpected_name.das")
        t |> success(find(issues, "unexpected name") >= 0,
            "the parser's token dump names the identifier token: {issues}")
        t |> success(find(issues, "reserved word") < 0, "and no note follows it")
    }
}
