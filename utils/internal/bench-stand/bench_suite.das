options gen2
options indenting = 4

module bench_suite public

require daslib/fio
require daslib/json_boost

//! Per-file knobs from `suite.json` `files`; a zero value means "inherit the suite default".
//! `skip` takes the file out of every lane, `skip_lanes` out of the ones it names - a file the
//! AOT emitter cannot handle still measures in the interpreter.
struct FileOverride {
    @optional skip : string
    @optional skip_lanes : table<string; string>
    @optional timeout_seconds : int
    @optional memory_limit_mb : int
}

//! Field initializers are the defaults a missing key keeps.
struct SuiteConfig {
    root : string = "benchmarks"
    exclude : array<string>
    lanes : array<string>
    repeat : int = 3
    timeout_seconds : int = 900
    memory_limit_mb : int = 4096
    regression_threshold : double = 0.10lf
    noise_multiplier : double = 3.0lf
    baseline_runs : int = 7
    files : table<string; FileOverride>
    lane_excludes : table<string; table<string; string>>
}

//! One benchmark file the stand runs. `path` is relative to the suite root with `/` separators,
//! `id` is `path` without the extension, `group` is its directory. A non-empty `skip` is the reason
//! the file is listed but not run.
struct BenchFile {
    path : string
    id : string
    group : string
    timeout_seconds : int
    memory_limit_mb : int
    skip : string
    skip_lanes : table<string; string>
}

//! Why this file does not run in this lane, empty when it does. A whole-file skip answers for
//! every lane; a lane skip answers only for its own.
def skip_reason(f : BenchFile; lane : string) : string {
    return f.skip if (!empty(f.skip))
    return f.skip_lanes?[lane] ?? ""
}

//! Load `suite.json`. Fails closed: any missing, unparseable or out-of-range field sets `error`
//! and returns the defaults, so a typo cannot silently shrink the suite. `exclude` takes a file
//! out of the tree; `lane_excludes` takes it out of one lane, keyed by glob and carrying the
//! reason the site shows every night.
def load_suite_config(path : string; var error : string&) : SuiteConfig {
    error = ""
    var cfg = SuiteConfig()
    if (!stat(path).is_reg) {
        error = "suite config not found: {path}"
        return <- cfg
    }
    if (!sscan_json(fread(path), cfg)) {
        error = "suite config does not parse as JSON: {path}"
        return <- cfg
    }
    if (empty(cfg.lanes)) {
        error = "suite config lists no lanes: {path}"
    } elif (cfg.repeat <= 0 || cfg.timeout_seconds <= 0 || cfg.memory_limit_mb <= 0) {
        error = "suite config needs positive repeat, timeout_seconds and memory_limit_mb: {path}"
    } elif (cfg.baseline_runs < 3) {
        error = "suite config needs baseline_runs >= 3: {path}"
    }
    for (lane in keys(cfg.lane_excludes)) {
        if (find_index(cfg.lanes, lane) < 0) {
            error = "suite config excludes files from lane \"{lane}\", which it does not run: {path}"
        }
    }
    for (lane in cfg.lanes) {
        if (lane != "interp" && lane != "jit" && lane != "aot") {
            error = "suite config names an unknown lane \"{lane}\" (interp, jit or aot): {path}"
        }
    }
    return <- cfg
}

def private is_excluded(rel : string; excludes : array<string>) : bool {
    for (pattern in excludes) {
        return true if (match_glob(pattern, rel))
    }
    return false
}

//! Every `.das` file under `<repo_root>/<cfg.root>` that no `exclude` glob matches, sorted by
//! path, with the per-file overrides applied. A `files` override naming a path that does not
//! exist is reported through `error` so a renamed benchmark cannot silently lose its skip.
def discover_files(repo_root : string; cfg : SuiteConfig; var error : string&) : array<BenchFile> {
    error = ""
    let root = path_join(repo_root, cfg.root)
    var rels : array<string>
    dir_rec(root) $(name, is_dir) {
        let rel = to_generic_path(name)
        return if (is_dir || extension(rel) != ".das" || is_excluded(rel, cfg.exclude))
        rels |> push(rel)
    }
    sort(rels)
    var out : array<BenchFile>
    out |> reserve(length(rels))
    for (rel in rels) {
        var f = BenchFile(path = rel, id = stem_path(rel), group = group_of(rel),
            timeout_seconds = cfg.timeout_seconds, memory_limit_mb = cfg.memory_limit_mb)
        for (lane in keys(cfg.lane_excludes)) {
            cfg.lane_excludes |> get(lane) $(globs) {
                for (pattern in keys(globs)) {
                    continue if (!match_glob(pattern, rel))
                    f.skip_lanes[lane] = globs?[pattern] ?? ""
                }
            }
        }
        cfg.files |> get(rel) $(ov) {
            f.skip = ov.skip
            for (lane in keys(ov.skip_lanes)) {
                f.skip_lanes[lane] = ov.skip_lanes?[lane] ?? ""
            }
            if (ov.timeout_seconds > 0) {
                f.timeout_seconds = ov.timeout_seconds
            }
            if (ov.memory_limit_mb > 0) {
                f.memory_limit_mb = ov.memory_limit_mb
            }
        }
        out |> emplace(f)
    }
    for (k in keys(cfg.files)) {
        if (!stat(path_join(root, k)).is_reg) {
            error = "suite config overrides \"{k}\", which is not a file under {root}"
        }
    }
    return <- out
}

//! `core/hash/test02.das` -> `core/hash`; a file at the root has group `root`.
def group_of(rel : string) : string {
    let d = to_generic_path(dir_name(rel))
    return (empty(d) || d == ".") ? "root" : d
}

//! `core/hash/test02.das` -> `core/hash/test02`.
def stem_path(rel : string) : string {
    let d = to_generic_path(dir_name(rel))
    let s = stem(rel)
    return (empty(d) || d == ".") ? s : "{d}/{s}"
}
