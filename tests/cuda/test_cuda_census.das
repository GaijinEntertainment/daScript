options gen2
options indenting = 4

// Census gate: the union of construct kinds the emitter records across every fixture must equal
// the declared set, in both directions. Text has no disassembler, so the census is recorded at
// emit time and published through each kernel's `{name}_census` companion.

require dastest/testing_boost public
require _cuda_common
require daslib/strings_boost

[test]
def test_cuda_census(t : T?) {
    t |> run("emitter census == declared set (all fixtures)") <| @(t : T?) {
        var present : table<string>
        var censuses <- all_cuda_censuses()
        for (cen in censuses) {
            for (kind in split(cen, "\n")) {
                present |> insert(kind)
            }
        }
        delete censuses
        var declared <- declared_cuda_census()
        for (kind in keys(present)) {
            t |> success(key_exists(declared, kind), "emitted `{kind}` is in the declared set")
        }
        for (kind in keys(declared)) {
            t |> success(key_exists(present, kind), "declared `{kind}` is actually emitted")
        }
        delete present
        delete declared
    }
}
