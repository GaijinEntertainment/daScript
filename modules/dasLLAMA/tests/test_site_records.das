options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require daslib/fio
require ../performance/gen_site_records.das   // merge_site_records — file-relative require, explicit .das (pays the engine compile, like test_tok_seed)
require ../performance/profile_common.das   // BenchModel — the merge's row type

// The records-vs-site drift gate: site/files/dasllama/bench_records.json is a COMMITTED merge
// of performance/records/*.json (daslang.io/dasllama.html renders from it), and forgetting to
// regenerate it after a records commit ships stale numbers to the live page. This cell IS the
// tool's --verify mode: regenerate in memory, byte-compare against the committed file.

[test]
def test_site_records_current(t : T?) {
    t |> run("the committed site merge matches the record stores byte for byte") @(tt : T?) {
        if (!stat("modules/dasLLAMA/performance/records").is_valid) {
            tt |> skip("not at the repo root (the record stores not visible) - the gate runs from the tree")
            return
        }
        if (!stat("site/files/dasllama/bench_records.json").is_valid) {
            // the committed merge is a tracked file - its absence is a broken or renamed rail, never a skip
            tt |> failure("site/files/dasllama/bench_records.json is missing - the site merge rail moved or was deleted; update gen_site_records + this gate together")
            return
        }
        var merged : array<BenchModel>
        if (!merge_site_records(merged)) {
            tt |> failure("the record stores under performance/records/ would not merge - see the log")
            return
        }
        let want = sprint_json(merged, true)
        let got = fread("site/files/dasllama/bench_records.json")
        if (got != want) {
            tt |> failure("site/files/dasllama/bench_records.json drifted from the record stores - a records commit skipped the site merge; run gen_site_records and commit both")
        }
        delete merged
    }
}
