options gen2
options _dasllama_internal = true   // the escape under test: with it, a direct engine require compiles

require dastest/testing_boost public
require dasllama/dasllama_version

[test]
def test_lint_escape(t : T?) {
    t |> run("options _dasllama_internal admits a direct engine require") @(t : T?) {
        t |> success(DASLLAMA_VERSION >= 4, "engine version constant readable through the direct require")
    }
}
