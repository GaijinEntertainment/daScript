options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require daslib/clargs
require daslib/fio
require profile_common
require llvm/daslib/llvm_tune   // [tune_policy]: provisioning only — never auto-tune

// Merge every per-box record store (performance/records/<box>.json) into the one file the site
// renders: site/files/dasllama/bench_records.json. Boxes never collide on the run key (box is part
// of it), so the merge is a pure upsert; run after any gen_bench_records sweep, commit both.
// Hand-authored annotations (records/annotations.json) merge LAST — they live outside the sweep
// output so a re-profile can never wipe an explanation.
//   bin/daslang modules/dasLLAMA/performance/gen_site_records.das [-- --verify]

let RECORDS_DIR = "modules/dasLLAMA/performance/records"
let SITE_PATH = "site/files/dasllama/bench_records.json"

[CommandLineArgs]
struct GenSiteArgs {
    @clarg_doc = "Compare the regenerated merge against the committed site file and fail on drift - touch nothing (the records-vs-site gate)"
    verify : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

//! The whole site merge in memory: every record store upserted, annotations applied last —
//! the one flow the writer, --verify, and the drift-gate test all share.
def merge_site_records(var merged : array<BenchModel>) : bool {
    var stores <- list_record_stores()
    if (empty(stores)) {
        to_log(LOG_ERROR, "gen_site_records: no stores under {RECORDS_DIR} - run gen_bench_records first\n")
        return false
    }
    for (path in stores) {
        var models : array<BenchModel>
        if (!read_bench_records(path, models)) {
            to_log(LOG_ERROR, "gen_site_records: cannot parse {path}\n")
            return false
        }
        let n = length(models)
        to_log(LOG_INFO, "gen_site_records: {path} - {n} model(s)\n")
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
    return true
}

[export, tune_policy(missing = "fallback")]
def main : int {
    var r <- parse_args(type<GenSiteArgs>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "gen_site_records: {r |> unwrap_err}\n")
        print_help(get_command_info(type<GenSiteArgs>), "gen_site_records")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<GenSiteArgs>), "gen_site_records")
        return 0
    }
    var merged : array<BenchModel>
    if (!merge_site_records(merged)) {
        return 1
    }
    if (cfg.verify) {
        let want = sprint_json(merged, true)
        let got = fread(SITE_PATH)
        if (got != want) {
            to_log(LOG_ERROR, "gen_site_records: {SITE_PATH} DRIFTED from the record stores - a records commit skipped the site merge; rerun gen_site_records and commit both\n")
            return 1
        }
        let total = length(merged)
        to_log(LOG_INFO, "gen_site_records: verify OK - {SITE_PATH} matches the {total}-model merge\n")
        return 0
    }
    write_bench_records(SITE_PATH, merged)
    let total = length(merged)
    to_log(LOG_INFO, "gen_site_records: wrote {SITE_PATH} - {total} model(s)\n")
    return 0
}
