options gen2

require strings
require math
require daslib/strings_boost
require daslib/fio
require daslib/clargs
require ../../common/parallel_workers.das
require classify
require utils_phase

// Release-audit batch runner over an EXTRACTED bundle: the bundle's own binary, no repo, no build tree.
// compile: every example/tutorial .das compiles (-compile-only, per-file timeout).
// utils:   every shipped tool run from the bundle (utils_phase.das). Process: README.md.

// the tool runs from the repo root (README), so the default lists are repo-relative
let private EXPECTED_DIR = "utils/internal/test-release"

[CommandLineArgs]
struct Config {
    @clarg_doc = "Extracted release bundle root (contains bin/, examples/, tutorials/)"
    bundle : string

    @clarg_doc = "Seconds allowed per file"
    timeout : float = 60.0

    @clarg_doc = "Worker processes (0 = auto)"
    jobs : int = 0

    @clarg_doc = "compile phase expected-failures list (default: expected_compile.txt beside the tool)"
    expected_compile : string

    @clarg_doc = "Which phases to run: compile, utils, or all"
    phase : string = "all"

    @clarg_doc = "utils phase: also run the network rows (daspkg installs; mutates the bundle's example dirs)"
    network : bool

    @clarg_doc = "utils phase: run only rows whose name contains this"
    only : string

    @clarg_doc = "utils phase expected-failures list (default: expected_utils.txt beside the tool)"
    expected_utils : string

    @clarg_doc = "Internal: run as a chunk worker over this paths file"
    paths_from : string

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def bundle_daslang(bundle : string) : string {
    let p = bundle_exe(bundle, "daslang")
    return fexist(p) ? p : ""
}

// every example/tutorial .das in the bundle, bundle-relative with forward slashes
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

// nearest *.das_project at or above `f` (bundle-relative), "" when none
def project_for(f : string; var cache : table<string; string>) : string {
    var d = dir_name(f)
    while (!empty(d) && d != ".") {
        let hit = cache ?[d] ?? "?"
        if (hit != "?") {
            return hit
        }
        var found = ""
        dir(d) $(name) {
            if (empty(found) && extension(name) == ".das_project") {
                found = "{d}/{name}"
            }
        }
        cache[d] = found
        if (!empty(found)) {
            return found
        }
        d = dir_name(d)
    }
    return ""
}

def run_worker(cfg : Config) : int {
    let exe = bundle_daslang(cfg.bundle)
    if (empty(exe)) {
        to_log(LOG_ERROR, "test-release: no daslang binary under {cfg.bundle}/bin\n")
        return 2
    }
    if (!chdir(cfg.bundle)) {
        to_log(LOG_ERROR, "test-release: cannot chdir to {cfg.bundle}\n")
        return 2
    }
    let listing = fread(cfg.paths_from)
    var inscope proj_cache : table<string; string>
    for (raw in split(listing, "\n")) {
        let f = strip(raw)
        continue if (empty(f))
        let t0 = ref_time_ticks()
        var out : string
        var rc = run_and_capture([exe, "-compile-only", f], out, cfg.timeout)
        if (rc != 0) {
            // a file that fails bare may be a project-file example (engine-alias
            // resolvers etc.) — retry the way its README runs it. Never the other
            // way around: sandbox-lesson projects would break bystander siblings.
            let proj = project_for(f, proj_cache)
            if (!empty(proj)) {
                var proj_out : string
                let proj_rc = run_and_capture([exe, "-compile-only", f, "-project", proj], proj_out, cfg.timeout)
                if (proj_rc == 0) {
                    rc = 0
                }
            }
        }
        let sec = float(get_time_usec(t0)) / 1000000.0
        print("{verdict_line(f, rc, sec, cfg.timeout, out)}\n")
    }
    return 0
}

def run_utils_phase(cfg : Config; bundle : string) : int {
    var inscope expected <- load_expected(!empty(cfg.expected_utils) ? cfg.expected_utils : path_join(EXPECTED_DIR, "expected_utils.txt"))
    to_log(LOG_INFO, "test-release utils: {cfg.network ? "network rows on" : "network rows off"}, {length(expected)} expected failures on file\n")
    let daslang = bundle_daslang(bundle)
    if (cfg.network && get_platform_name() == "windows" && length(bundle) > 60) {
        to_log(LOG_WARNING, "test-release: bundle path is {length(bundle)} chars - git clones and native package builds under it can hit MAX_PATH; extract to a short path (e.g. C:/dl) for the network rows
")
    }
    if (!chdir(bundle)) {
        to_log(LOG_ERROR, "test-release: cannot chdir to {bundle}\n")
        return 2
    }
    var inscope verdicts <- run_utils_checks(bundle, daslang, cfg.network, cfg.only)
    var inscope report <- summarize(verdicts, expected)
    for (ln in report.lines) {
        print("{ln}\n")
    }
    print("test-release utils: {report.ok_count} ok, {report.expected_count} expected-fail, {report.unexpected_count} UNEXPECTED, {report.stale_count} stale-expected, {report.timeout_count} timeout\n")
    return report.unexpected_count + report.timeout_count + report.stale_count > 0 ? 1 : 0
}

def run_parent(cfg : Config) : int {
    let bundle = get_full_file_name(cfg.bundle)
    if (empty(bundle_daslang(bundle))) {
        to_log(LOG_ERROR, "test-release: no daslang binary under {cfg.bundle}/bin\n")
        return 2
    }
    if (cfg.phase != "compile" && cfg.phase != "utils" && cfg.phase != "all") {
        to_log(LOG_ERROR, "test-release: --phase must be compile, utils, or all\n")
        return 2
    }
    // compile first: the utils phase (with --network) installs into the example dirs
    var rc = 0
    if (cfg.phase != "utils") {
        rc = max(rc, run_compile_phase(cfg, bundle))
    }
    if (cfg.phase != "compile") {
        rc = max(rc, run_utils_phase(cfg, bundle))
    }
    return rc
}

def run_compile_phase(cfg : Config; bundle : string) : int {
    var inscope targets <- collect_targets(bundle)
    if (empty(targets)) {
        to_log(LOG_ERROR, "test-release: no example/tutorial .das found under {bundle}\n")
        return 2
    }
    var inscope expected <- load_expected(!empty(cfg.expected_compile) ? cfg.expected_compile : path_join(EXPECTED_DIR, "expected_compile.txt"))
    to_log(LOG_INFO, "test-release compile: {length(targets)} targets, {length(expected)} expected failures on file\n")

    let workers = compute_worker_count(length(targets), cfg.jobs)
    var inscope chunks <- chunk_files(targets, workers)
    var argvs : array<array<string>>
    argvs |> reserve(length(chunks))
    var tmpfiles : array<string>
    tmpfiles |> reserve(length(chunks))
    for (chunk in chunks) {
        var err : string
        let tmp = create_temp_file("test_release_chunk", ".txt", err)
        if (empty(tmp)) {
            to_log(LOG_ERROR, "test-release: cannot create a chunk file: {err}\n")
            return 2
        }
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
