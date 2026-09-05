options gen2
options indenting = 4

require dastest/testing_boost public

require ../config.das

[test]
def test_has_path_under(t : T?) {
    let prefix = "modules/dasLLAMA/"
    t |> success(has_path_under(["src/ast/ast_parse.cpp", "modules/dasLLAMA/tests/run.das"], prefix), "a path under the prefix arms the gate")
    t |> success(!has_path_under(["modules/dasLLVM/daslib/llvm_jit.das", "doc/modules/dasLLAMA/x.rst"], prefix), "a look-alike elsewhere in the path does not")
    let none : array<string>
    t |> success(!has_path_under(none, prefix), "an empty diff does not")
    t |> success(has_path_under(["modules/dasLLAMA/REVIEW.md\r"], prefix), "a CR-terminated line still counts")
}
