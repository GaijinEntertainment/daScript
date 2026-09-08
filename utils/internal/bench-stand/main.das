options gen2
options indenting = 4

require bench_suite
require bench_runner
require bench_history
require daslib/clargs
require daslib/fio
require daslib/json_boost
require daslib/strings_boost
require strings

//! The daslang benchmark stand - the tool half of the nightly. Two verbs, both run from the
//! repo root with the tree's own binary:
//!   daslang utils/internal/bench-stand/main.das -- run --meta meta.json --out runs/<id>.json
//!   daslang utils/internal/bench-stand/main.das -- report --runs runs/ --out-data site/data.json --out-summary site/summary.md
//! The box-side driver is `nightly.sh`; the exit codes are stated where they are produced.

[CommandLineArgs]
struct Config {
    @clarg_positional
    @clarg_doc = "run | report"
    verb : string

    @clarg_doc = "Repo root (default: .)"
    root : string

    @clarg_doc = "suite.json to use (default: the one beside this tool)"
    suite : string

    @clarg_doc = "run: daslang binary the interp and jit lanes use (default: bin/daslang under --root)"
    bin : string

    @clarg_doc = "run: binary the aot lane uses - daslang with the benchmarks' AOT stubs linked in (default: bin/test_aot under --root)"
    aot_bin : string

    @clarg_doc = "run: JSON with run_id, started, commit, machine, build - written by nightly.sh"
    meta : string

    @clarg_doc = "run: where to write the run record"
    out : string

    @clarg_doc = "run: only files whose path contains this substring"
    filter : string

    @clarg_doc = "run: comma-separated lanes to run (default: suite.json lanes)"
    lanes : string

    @clarg_doc = "run: benchmark nothing - write the record of a night that could not run the suite, with this reason (non-empty), and exit 1"
    failed : string

    @clarg_doc = "run: repeats per file (default: suite.json repeat)"
    repeat : int

    @clarg_doc = "report: directory of run records"
    runs : string

    @clarg_doc = "report: where to write data.json"
    out_data : string

    @clarg_doc = "report: where to write summary.md"
    out_summary : string

    @clarg_doc = "report: ISO timestamp stamped into data.json (default: now, UTC)"
    generated : string

    @clarg_doc = "report: repository URL for commit links"
    repo_url : string

    @clarg_doc = "report: site URL the summary links to"
    site_url : string

    @clarg_doc = "report: exit 2 when the latest run regressed"
    fail_on_regression : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private iso_now() : string {
    return "{iso8601_now()}"
}

def private write_text_file(path, text : string) : bool {
    if (!fwrite(path, text)) {
        to_log(LOG_ERROR, "cannot write {path}\n")
        return false
    }
    return true
}

def private suite_path_of(cfg : Config) : string {
    return empty(cfg.suite) ? path_join(get_das_root(), "utils/internal/bench-stand/suite.json") : cfg.suite
}

def private read_meta(path : string; var rec : RunRecord&) : bool {
    var meta = RunMeta()
    return false if (!sscan_json(fread(path), meta) || empty(meta.run_id))
    rec.run_id = meta.run_id
    rec.started = meta.started
    rec.commit = meta.commit
    rec.machine = meta.machine
    rec.build = meta.build
    return true
}

//! The binary a lane measures with: the AOT lane needs its own, the other two share daslang.
def private lane_binary(lane, bin, aot_bin : string) : string {
    return lane == "aot" ? aot_bin : bin
}

def private probe_lanes(var rec : RunRecord&; lanes : array<string>; bin, aot_bin : string) {
    for (lane in lanes) {
        if (lane == "jit") {
            let why = probe_jit(bin)
            rec.lanes[lane] = empty(why) ? "ok" : why
        } elif (lane == "aot") {
            let why = probe_aot(aot_bin)
            rec.lanes[lane] = empty(why) ? "ok" : why
        } else {
            rec.lanes[lane] = "ok"
        }
        to_log(LOG_INFO, "lane {lane}: {rec.lanes[lane]}\n")
    }
}

def private matches_filter(f : BenchFile; filter : string) : bool {
    return empty(filter) || find(f.path, filter) >= 0
}

def private verb_run(cfg : Config; suite : SuiteConfig) : int {
    if (empty(cfg.meta) || empty(cfg.out)) {
        to_log(LOG_ERROR, "bench-stand run: --meta and --out are required\n")
        return 1
    }
    let root = empty(cfg.root) ? "." : cfg.root
    let bin = empty(cfg.bin) ? path_join(root, "bin/daslang") : cfg.bin
    let aot_bin = empty(cfg.aot_bin) ? path_join(root, "bin/test_aot") : cfg.aot_bin
    var rec = RunRecord()
    if (!read_meta(cfg.meta, rec)) {
        to_log(LOG_ERROR, "bench-stand run: --meta {cfg.meta} does not parse or has no run_id\n")
        return 1
    }
    if (!empty(cfg.failed)) {
        mark_failed_run(rec, iso_now(), cfg.failed)
        return 1 if (!write_text_file(cfg.out, sprint_json(rec, true)))
        to_log(LOG_ERROR, "bench-stand run: {rec.status} - {cfg.failed}; record {cfg.out}\n")
        return 1
    }
    var error = ""
    let all_files <- discover_files(root, suite, error)
    if (!empty(error)) {
        to_log(LOG_ERROR, "bench-stand run: {error}\n")
        return 1
    }
    let lanes <- empty(cfg.lanes) ? clone_to_move(suite.lanes) : split(cfg.lanes, ",")
    probe_lanes(rec, lanes, bin, aot_bin)
    let repeat = cfg.repeat > 0 ? cfg.repeat : suite.repeat
    let t0 = ref_time_ticks()
    let failed = benchmark_every_file(rec, all_files, cfg.filter, lanes, bin, aot_bin, root, repeat)
    rec.seconds = double(get_time_usec(t0)) / 1000000.0lf
    rec.finished = iso_now()
    rec.status = derive_status(rec)
    return 1 if (!write_text_file(cfg.out, sprint_json(rec, true)))
    let ok = rec.status == RunStatus.ok
    to_log(ok ? LOG_INFO : LOG_ERROR, "bench-stand run: {rec.status} - {length(rec.files)} file runs, {failed} failed, {rec.seconds:.0f} s; record {cfg.out}\n")
    return ok ? 0 : 1
}

//! Runs every file in every runnable lane into `rec.files`, one progress line each; returns how
//! many of those runs failed. A lane the probe marked unavailable contributes no file.
def private benchmark_every_file(var rec : RunRecord&; files : array<BenchFile>; filter : string;
                                 lanes : array<string>; bin, aot_bin, root : string; repeat : int) : int {
    var limits = RunLimits(repeat = repeat)
    var matching = 0
    for (f in files) {
        matching++ if (matches_filter(f, filter))
    }
    let total = matching * length(lanes)
    var done = 0
    var failed = 0
    rec.files |> reserve(total)
    for (f in files) {
        continue if (!matches_filter(f, filter))
        for (lane in lanes) {
            done++
            continue if (rec.lanes[lane] != "ok")
            limits.timeout_seconds = f.timeout_seconds
            limits.memory_limit_mb = f.memory_limit_mb
            var res <- run_bench_file(lane_binary(lane, bin, aot_bin), root, f, lane, limits)
            failed++ if (is_failure(res.status))
            to_log(is_failure(res.status) ? LOG_ERROR : LOG_INFO,
                "[{done}/{total}] {f.path} {lane}: {res.status} {res.seconds:.1f}s {length(res.samples)} arms{empty(res.message) ? "" : " - " + res.message}\n")
            rec.files |> emplace(res)
        }
    }
    return failed
}

def private verb_report(cfg : Config; suite : SuiteConfig) : int {
    if (empty(cfg.runs) || empty(cfg.out_data) || empty(cfg.out_summary)) {
        to_log(LOG_ERROR, "bench-stand report: --runs, --out-data and --out-summary are required\n")
        return 1
    }
    var errors : array<string>
    let runs <- load_runs(cfg.runs, errors)
    for (e in errors) {
        to_log(LOG_WARNING, "bench-stand report: {e}\n")
    }
    let ds <- build_dataset(runs, suite, empty(cfg.generated) ? iso_now() : cfg.generated, cfg.repo_url)
    return 1 if (!write_text_file(cfg.out_data, sprint_json(ds, false)))
    let md = render_summary(ds, cfg.site_url)
    return 1 if (!write_text_file(cfg.out_summary, md))
    to_log(LOG_INFO, "bench-stand report: {length(runs)} runs, {length(ds.series)} series, {length(ds.changes)} changes -> {cfg.out_data}, {cfg.out_summary}\n")
    return summary_exit_code(ds, cfg.fail_on_regression)
}

[export]
def main() : int {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "error: {r |> unwrap_err}\n")
        print_help(get_command_info(type<Config>), "bench-stand")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help || (cfg.verb != "run" && cfg.verb != "report")) {
        print_help(get_command_info(type<Config>), "bench-stand")
        return cfg.help ? 0 : 1
    }
    var error = ""
    let suite <- load_suite_config(suite_path_of(cfg), error)
    if (!empty(error)) {
        to_log(LOG_ERROR, "bench-stand: {error}\n")
        return 1
    }
    return cfg.verb == "run" ? verb_run(cfg, suite) : verb_report(cfg, suite)
}
