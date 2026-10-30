options gen2
options no_aot

require dastest/testing_boost public
require daslib/ast
require daslib/rtti

let FAILED_RETURN_SOURCE = ("options gen2\n"
    + "def make_one : int[1] \{\n    var a : int[1]\n    return a\n\}\n"
    + "[export]\ndef f => fixed_array(make_one()).[4]\n")

[test]
def test_return_of_a_failed_subexpression(t : T?) {
    using() $(var cop : CodeOfPolicies) {
        cop.threadlock_context = true
        compile("failed_return", FAILED_RETURN_SOURCE, cop) $(ok; program; issues) {
            t |> success(ok, "the program compiles: {issues}")
            return if (!ok)
            simulate(program) $(sok; _ctx; _serrors) {
                t |> success(!sok)
                let codes <- [for (err in program.errors); err.cerr]
                t |> equal(codes, [CompilationError.exceeds_array])
            }
        }
    }
}
