options gen2
options _dasllama_internal = true   // the escape under test: with it, a direct engine require compiles
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_version

[test]
def test_lint_escape(t : T?) {
    t |> run("options _dasllama_internal admits a direct engine require") @(t : T?) {
        t |> success(DASLLAMA_RELEASE >= 4, "engine version constant readable through the direct require")
    }
}
