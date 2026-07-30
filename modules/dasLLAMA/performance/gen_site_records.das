options gen2
options stack = 65536   // dasLLAMA program-root convention (options stack does not unify up from libs)

require profile_common
require daslib/fio
require strings

// Merge every per-box record store (performance/records/<box>.json) into the one file the site
// renders: site/files/dasllama/bench_records.json. Boxes never collide on the run key (box is part
// of it), so the merge is a pure upsert; run after any gen_bench_records sweep, commit both.
// Hand-authored annotations (records/annotations.json) merge LAST — they live outside the sweep
// output so a re-profile can never wipe an explanation.
//   bin/daslang modules/dasLLAMA/performance/gen_site_records.das

let RECORDS_DIR = "modules/dasLLAMA/performance/records"
let SITE_PATH = "site/files/dasllama/bench_records.json"

def private list_stores() : array<string> {
    var out : array<string>
    dir(RECORDS_DIR) $(fn) {
        if (ends_with(fn, ".json") && !starts_with(fn, "_")) {
            out |> push("{RECORDS_DIR}/{fn}")
        }
    }
    sort(out)
    return <- out
}

[export]
def main : int {
    var merged : array<BenchModel>
    var stores <- list_stores()
    if (empty(stores)) {
        to_log(LOG_ERROR, "gen_site_records: no stores under {RECORDS_DIR} — run gen_bench_records first\n")
        return 1
    }
    for (path in stores) {
        var models : array<BenchModel>
        if (!read_bench_records(path, models)) {
            to_log(LOG_ERROR, "gen_site_records: cannot parse {path}\n")
            return 1
        }
        let n = length(models)
        to_log(LOG_INFO, "gen_site_records: {path} — {n} model(s)\n")
        for (m in models) {
            upsert_bench_model(merged, m)
        }
    }
    var annos : array<BenchAnnotation>
    if (read_annotations(annos)) {
        apply_annotations(merged, annos)
        let n = length(annos)
        to_log(LOG_INFO, "gen_site_records: applied {n} annotation(s)\n")
    }
    write_bench_records(SITE_PATH, merged)
    let total = length(merged)
    to_log(LOG_INFO, "gen_site_records: wrote {SITE_PATH} — {total} model(s)\n")
    return 0
}
