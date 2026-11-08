options gen2

require dastest/testing_boost public
require _msl_common
require daslib/strings_boost

// Census gate (gate B): the union of construct kinds the emitter records across EVERY fixture
// must equal the declared supported set, in both directions. Text has no disassembler (dasSpirv
// re-derives its census from the blob; we cannot), so the census is recorded at emit time and
// published via each kernel's `{name}_census` companion global.
[test(description = "The set of construct kinds the emitter records across all fixtures equals the declared set, in both directions.")]
def test_msl_census(t : T?) {
    t |> run("emitter census == declared set (all fixtures)") <| @(t : T?) {
        var present : table<string>
        var censuses <- all_msl_censuses()
        for (cen in censuses) {
            for (kind in split(cen, "\n")) {
                present |> insert(kind)
            }
        }
        delete censuses
        var declared <- declared_msl_census()
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
