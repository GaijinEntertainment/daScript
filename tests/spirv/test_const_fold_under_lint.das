options gen2
options indenting = 4

// Lint compiles with folding off, so a const global keeps the initializer as written. The emitter
// still needs its value -- for a constant `let` it lowers to an OpConstant, and for a named layout
// index it decorates with. Both must ask the compiler what such an expression is worth instead of
// taking only the shapes that are already constant, or every shader-bearing module is unlintable.

require dastest/testing_boost public
require daslib/ast

def fixture_path() : string {
    return "{get_das_root()}/tests/spirv/_lint_fold_fixture.das"
}

[test]
def test_shader_compiles_with_folding_off(t : T?) {
    t |> run("a shader module compiles under the lint policies") <| @(t : T?) {
        var inscope access <- make_file_access("")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.export_all = true
                cop.lint_check = true
                cop.no_optimizations = true
                cop.no_infer_time_folding = true
                compile_file(fixture_path(), access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                    t |> success(ok, "the fixture did not compile: {issues}")
                    t |> success(program != null, "no program")
                }
            }
        }
    }
}
