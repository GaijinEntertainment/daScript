options gen2

require strings
require daslib/strings_boost
require daslib/fio
require daslib/clargs
require ../../common/parallel_workers.das
require classify

// The release-audit batch runner: points at an EXTRACTED release bundle and proves it
// sane with the bundle's own binary — no repo, no build tree. Phase `compile` is the
// floor: every example/tutorial .das in the bundle compiles (-compile-only, per-file
// timeout). The process doc is README.md here.

[CommandLineArgs]
struct Config {
    @clarg_doc = "Extracted release bundle root (contains bin/, examples/, tutorials/)"
    bundle : string

    @clarg_doc = "Seconds allowed per file"
    timeout : float = 60.0

    @clarg_doc = "Worker processes (0 = auto)"
    jobs : int = 0

    @clarg_doc = "Expected-failures list (default: expected_compile.txt beside the tool)"
    expected : string

    @clarg_doc = "Internal: run as a chunk worker over this paths file"
    paths_from : string

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def bundle_binary(bundle : string) : string {
    for (name in ["bin/daslang.exe", "bin/daslang"]) {
        let p = path_join(bundle, name)
        if (fexist(p)) {
            return get_full_file_name(p)
        }
    }
    return ""
}

//! Every example/tutorial .das in the bundle, bundle-relative with forward slashes.
def collect_targets(bundle : string) : array<string> {
    var out : array<string>
    var roots <- ["examples", "tutorials"]
    dir(path_join(bundle, "modules")) $(name) {
        return if (name == "." || name == "..")
        for (sub in ["examples", "tutorials"]) {
            let rel = "modules/{name}/{sub}"
            if (stat(path_join(bundle, rel)).is_dir) {
                roots |> push(rel)
            }
        }
    }
    for (root in roots) {
        let base = path_join(bundle, root)
        continue if (!stat(base).is_dir)
        dir_rec(base) $(name, is_dir) {
            return if (is_dir || extension(name) != ".das")
            out |> push("{root}/{to_generic_path(name)}")
        }
    }
    out |> sort
    return <- out
}

def run_worker(cfg : Config) : int {
    let exe = bundle_binary(cfg.bundle)
    if (empty(exe)) {
        to_log(LOG_ERROR, "test-release: no daslang binary under {cfg.bundle}/bin\n")
        return 2
    }
    if (!chdir(cfg.bundle)) {
        to_log(LOG_ERROR, "test-release: cannot chdir to {cfg.bundle}\n")
        return 2
    }
    let listing = fread(cfg.paths_from)
    for (raw in split(listing, "\n")) {
        let f = strip(raw)
        continue if (empty(f))
        let t0 = ref_time_ticks()
        var out : string
        let rc = run_and_capture([exe, "-compile-only", f], out, cfg.timeout)
        let sec = float(get_time_usec(t0)) / 1000000.0
        print("{verdict_line(f, rc, sec, cfg.timeout, out)}\n")
    }
    return 0
}

def run_parent(cfg : Config) : int {
    let bundle = get_full_file_name(cfg.bundle)
    if (empty(bundle_binary(bundle))) {
        to_log(LOG_ERROR, "test-release: no daslang binary under {cfg.bundle}/bin\n")
        return 2
    }
    var inscope targets <- collect_targets(bundle)
    if (empty(targets)) {
        to_log(LOG_ERROR, "test-release: no example/tutorial .das found under {bundle}\n")
        return 2
    }
    // internal tool: runs from the repo root (README), so the default list is repo-relative
    var inscope expected <- load_expected(!empty(cfg.expected) ? cfg.expected : "utils/internal/test-release/expected_compile.txt")
    to_log(LOG_INFO, "test-release compile: {length(targets)} targets, {length(expected)} expected failures on file\n")

    let workers = compute_worker_count(length(targets), cfg.jobs)
    var inscope chunks <- chunk_files(targets, workers)
    var err : string
    var argvs : array<array<string>>
    argvs |> reserve(length(chunks))
    var tmpfiles : array<string>
    tmpfiles |> reserve(length(chunks))
    for (chunk in chunks) {
        let tmp = create_temp_file("test_release_chunk", ".txt", err)
        let body = build_string() $(var writer) {
            for (f in chunk) {
                writer |> write(f)
                writer |> write("\n")
            }
        }
        fwrite(tmp, body)
        tmpfiles |> push(tmp)
        var argv <- spawn_argv_prefix()
        argv |> push("--")
        argv |> push("--bundle")
        argv |> push(bundle)
        argv |> push("--timeout")
        argv |> push("{cfg.timeout}")
        argv |> push("--paths-from")
        argv |> push(tmp)
        argvs |> emplace(argv)
    }
    var inscope results <- run_chunk_workers(argvs)
    for (tmp in tmpfiles) {
        remove(tmp)
    }
    var verdicts : array<string>
    for (r in results) {
        if (r.exit_code != 0) {
            to_log(LOG_ERROR, "test-release: worker {r.index} died rc={r.exit_code}\n{r.stdout}")
            return 2
        }
        for (ln in split(r.stdout, "\n")) {
            let s = strip(replace(ln, "\r", ""))
            if (!empty(s)) {
                verdicts |> push(s)
            }
        }
    }
    var inscope report <- summarize(verdicts, expected)
    for (ln in report.lines) {
        print("{ln}\n")
    }
    print("test-release compile: {report.ok_count} ok, {report.expected_count} expected-fail, {report.unexpected_count} UNEXPECTED, {report.stale_count} stale-expected, {report.timeout_count} timeout\n")
    return report.unexpected_count + report.timeout_count + report.stale_count > 0 ? 1 : 0
}

[export]
def main() : int {
    var args_r <- parse_args(type<Config>)
    if (args_r |> is_err) {
        to_log(LOG_ERROR, "error: {args_r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Config>), "test-release")
        return 1
    }
    let cfg <- args_r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "test-release")
        return 0
    }
    if (empty(cfg.bundle)) {
        to_log(LOG_ERROR, "test-release: --bundle is required\n")
        return 1
    }
    return !empty(cfg.paths_from) ? run_worker(cfg) : run_parent(cfg)
}
