options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require daslib/clargs
require daslib/fio
require profile_common
require daslib/tune             // [tune_policy]: provisioning only — never auto-tune
require llvm/daslib/llvm_tune   // the framework the annotation forwards to, and the visibility the auto-policy pass needs

// Merge every per-box record store (performance/records/<box>.json) into the files the site
// renders: site/files/dasllama/bench_records.json (the full merge) and bench_cells.json (its
// first-paint projection, see SiteModel). Boxes never collide on the run key (box is part
// of it), so the merge is a pure upsert; run after any gen_bench_records sweep, commit all three.
// Hand-authored annotations (records/annotations.json) merge LAST — they live outside the sweep
// output so a re-profile can never wipe an explanation.
//   bin/daslang modules/dasLLAMA/performance/gen_site_records.das [-- --verify]

let RECORDS_DIR = "modules/dasLLAMA/performance/records"
let SITE_PATH = "site/files/dasllama/bench_records.json"
let SITE_CELLS_PATH = "site/files/dasllama/bench_cells.json"

//! What dasllama.html renders before a visitor opens a receipt: the merge minus the receipt
//! fields — the command, its environment, the tune stamps, the exec format, the reference
//! version line, the input hashes, the full hardware line, an ASR row's transcript. Every key
//! here holds what `bench_records.json` holds at the same path; the page fetches that full
//! store once, on the first receipt opened, and never for a first paint.
struct SiteTest {
    @optional tok_s : double
    @optional ms : double
    @optional audio_s : double
}

struct SiteFile {
    role : string
    name : string
}

struct SiteHardware {
    cpu : string
}

struct SiteRun {
    engine : string
    backend : string
    flavor : string
    box : string
    threads : int
    date : string
    hardware : SiteHardware
    tests : table<string; SiteTest>
    @optional workload : string
    @optional source : string
    @optional sha : string
    @optional version : string
    @optional files : array<SiteFile>
    @optional comment : string
    @optional void_clips : string
}

struct SiteModel {
    gguf : string
    runs : array<SiteRun>
    @optional arch : string
    @optional size_bytes : int64
    @optional note : string
}

//! The projection of the whole merge (`SiteModel`), as the text `bench_cells.json` holds: the
//! full store's text re-read into the Site structs, which is what drops every key they do not
//! declare - the structs are the projection's only spelling.
def site_cells_json(models : array<BenchModel>) : string {
    var cells : array<SiteModel>
    // a re-read that fails or loses a model is the Site structs drifting from the merge's
    // shape: a loud stop, never an empty projection the drift gate would then pin as correct
    if (!sscan_json(sprint_json(models, false), cells) || length(cells) != length(models)) {
        panic("gen_site_records: the Site structs no longer read the merge ({length(cells)} of {length(models)} models) - realign SiteModel with BenchModel")
    }
    let text = sprint_json(cells, true)
    delete cells
    return text
}

//! Every file the site merge writes, with its text - one list for the writer, `--verify`, and
//! the drift gate, so the set cannot drift between them.
def site_outputs(models : array<BenchModel>) : array<tuple<path : string; text : string>> {
    return <- [(path = SITE_PATH, text = sprint_json(models, true)), (path = SITE_CELLS_PATH, text = site_cells_json(models))]
}

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
    let outputs <- site_outputs(merged)
    if (cfg.verify) {
        for (o in outputs) {
            if (fread(o.path) != o.text) {
                to_log(LOG_ERROR, "gen_site_records: {o.path} DRIFTED from the record stores - a records commit skipped the site merge; rerun gen_site_records and commit all three\n")
                return 1
            }
        }
        let total = length(merged)
        to_log(LOG_INFO, "gen_site_records: verify OK - {SITE_PATH} and {SITE_CELLS_PATH} match the {total}-model merge\n")
        return 0
    }
    // both or neither reported: a merge that landed in one file and not the other is drift
    for (o in outputs) {
        if (!write_records_text(o.path, o.text)) {
            return 1
        }
    }
    let total = length(merged)
    to_log(LOG_INFO, "gen_site_records: wrote {SITE_PATH} and {SITE_CELLS_PATH} - {total} model(s)\n")
    return 0
}
