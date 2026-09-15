options gen2
options indenting = 4
options no_aot // compiles fixtures through the real compiler at runtime

require dastest/testing_boost
require daslib/ast
require daslib/ast_boost
require strings

def fixture(name : string) : string {
    return "{get_das_root()}/tests/fold_rules/{name}"
}

def compile_fixture(name : string; blk : block<(ok : bool; issues : string) : void>) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.export_all = true
            compile_file(fixture(name), access, unsafe(addr(mg)), cop) $(ok; _program; issues) {
                invoke(blk, ok, "{issues}")
            }
        }
    }
}

[test]
def test_a_non_monotonic_rule_is_reported(t : T?) {
    t |> run("the retry cap names the expression it could not settle") @@(t : T?) {
        compile_fixture("_fixture_cycle.das") $(ok; issues) {
            t |> success(!ok, "the fixture must fail to compile")
            t |> success(find(issues, "do not converge", 0) >= 0,
                "expected a convergence diagnostic, got: {issues}")
        }
    }
}

[test]
def test_an_untypeable_rewrite_is_reported(t : T?) {
    t |> run("the engine refuses a replacement it cannot type") @@(t : T?) {
        compile_fixture("_fixture_untypeable.das") $(ok; issues) {
            t |> success(!ok, "the fixture must fail to compile")
            t |> success(find(issues, "cannot type", 0) >= 0,
                "expected a typing diagnostic, got: {issues}")
            t |> success(find(issues, "no_such_builtin", 0) >= 0,
                "the diagnostic must name the offending call, got: {issues}")
        }
    }
}
