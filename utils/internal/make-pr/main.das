options gen2

// make-pr - the mechanical gates of PR preparation that preflight's CI-mirror charter
// does not carry (skills/internal/make_pr.md keeps the policy and the judgment steps).
// Gates in order: sync, review-md, dupes, ast-verify (conditional), jit-smoke (conditional),
// then preflight --full unless --no-preflight. Exit codes: 0 all green (judgment checklist
// printed), 2 a gate failed (named), 1 environment/API failure.

require strings
require daslib/fio
require daslib/clargs
require daslib/strings_boost

[CommandLineArgs]
struct Config {
    @clarg_doc = "Diff base ref"
    base : string = "origin/master"

    @clarg_doc = "Run a single gate: sync | review-md | dupes | ast-verify | jit-smoke"
    only : string

    @clarg_doc = "Skip a gate by name (repeatable)"
    skip : array<string>

    @clarg_doc = "Do not chain preflight --full after the gates"
    no_preflight : bool

    @clarg_doc = "Path to the daslang binary (default: newest repo-local)"
    daslang : string
}

var g_cfg = Config()

def private run(argv : array<string>; var out : string&; timeout = 600.0) : int {
    return run_and_capture(argv, out, timeout)
}

def private git_lines(args : array<string>) : array<string> {
    var argv <- ["git"]
    argv |> push_from(args)
    var out = ""
    run(argv, out)
    var lines : array<string>
    for (raw in split(out, "\n")) {
        let line = strip(replace(raw, "\r", ""))
        if (!empty(line)) {
            lines |> push(line)
        }
    }
    return <- lines
}

def private find_daslang() : string {
    if (!empty(g_cfg.daslang)) {
        return fexist(g_cfg.daslang) ? get_full_file_name(g_cfg.daslang) : ""
    }
    var best = ""
    var best_mtime = 0l
    for (c in ["bin/daslang", "bin/daslang.exe", "bin/Release/daslang.exe",
               "bin/RelWithDebInfo/daslang.exe", "build/daslang", "build/bin/daslang"]) {
        if (fexist(c)) {
            let st = stat(c)
            if (st.is_valid && int64(st.mtime) > best_mtime) {
                best_mtime = int64(st.mtime)
                best = c
            }
        }
    }
    return empty(best) ? "" : get_full_file_name(best)
}

def private changed_files(pattern : string = "") : array<string> {
    var args <- ["diff", "--name-only", "{g_cfg.base}..HEAD"]
    if (!empty(pattern)) {
        args |> push("--")
        args |> push(pattern)
    }
    return <- git_lines(args)
}

def private gate_sync() : bool {
    var out = ""
    if (run(["git", "fetch", "origin", "master"], out, 120.0) != 0) {
        to_log(LOG_ERROR, "sync: git fetch failed\n{out}\n")
        return false
    }
    let behind = git_lines(["rev-list", "--count", "HEAD..origin/master"])
    if (!empty(behind) && behind[0] != "0") {
        to_log(LOG_ERROR, "sync: branch is {behind[0]} commits behind origin/master - rebase before anything else (git rebase origin/master); squashing before this rebase bakes other PRs into your commit\n")
        return false
    }
    let files = changed_files()
    to_log(LOG_INFO, "sync: up to date with origin/master; PR set = {length(files)} files:\n")
    for (f in files) {
        to_log(LOG_INFO, "  {f}\n")
    }
    to_log(LOG_INFO, "sync: verify every file above is one you edited - a stray file from a merged PR means the rebase went wrong\n")
    return true
}

def private gate_review_md(daslang : string) : bool {
    var out = ""
    let code = run([daslang, "utils/internal/review-md/main.das", "--", "--base", g_cfg.base], out, 1800.0)
    to_log(LOG_INFO, out)
    if (code != 0) {
        to_log(LOG_ERROR, "review-md: a discovered REVIEW.das gate is red - fail-fix before agents launch\n")
        return false
    }
    return true
}

def private touched_scopes() : array<string> {
    var seen : table<string>
    var scopes : array<string>
    for (f in changed_files("*.das")) {
        let parts <- split(to_generic_path(f), "/")
        if (length(parts) >= 2) {
            let scope = "{parts[0]}/{parts[1]}"
            if (!key_exists(seen, scope)) {
                seen |> insert(scope)
                scopes |> push(scope)
            }
        }
    }
    return <- scopes
}

def private gate_dupes(daslang : string) : bool {
    let das_files <- changed_files("*.das")
    if (empty(das_files)) {
        to_log(LOG_INFO, "dupes: no .das changes - skipped\n")
        return true
    }
    let scopes <- touched_scopes()
    if (empty(scopes)) {
        to_log(LOG_INFO, "dupes: no scoped dirs - skipped\n")
        return true
    }
    var err = ""
    let tmp = create_temp_directory("make_pr_dupes", err)
    if (empty(tmp)) {
        to_log(LOG_ERROR, "dupes: temp dir failed: {err}\n")
        return false
    }
    let corpus = path_join(tmp, "corpus.json")
    var argv <- [daslang, "utils/detect-dupe/main.das", "--"]
    argv |> reserve(length(argv) + length(scopes) * 2 + 2)
    for (s in scopes) {
        argv |> push("-p")
        argv |> push(s)
    }
    argv |> push("--export-functions")
    argv |> push(corpus)
    var out = ""
    if (run(argv, out, 1800.0) != 0) {
        to_log(LOG_ERROR, "dupes: corpus export failed\n{out}\n")
        return false
    }
    var against <- [daslang, "utils/detect-dupe/main.das", "--", "--import-functions", corpus]
    against |> reserve(length(against) + length(das_files) * 2)
    for (f in das_files) {
        against |> push("--against")
        against |> push(f)
    }
    var report = ""
    run(against, report, 1800.0)
    to_log(LOG_INFO, "dupes: scope = {join(scopes, ", ")}\n{report}\n")
    to_log(LOG_INFO, "dupes: TRIAGE IS YOURS - exact match means reuse/justify/extract (skills/internal/detect_dupe.md)\n")
    return true
}

def private diff_touches(patterns : array<string>) : bool {
    for (f in changed_files()) {
        let g = to_generic_path(f)
        for (p in patterns) {
            if (find(g, p) >= 0) {
                return true
            }
        }
    }
    return false
}

def private gate_ast_verify(daslang : string) : bool {
    if (!diff_touches(["daslib/", "src/ast/", "utils/"])) {
        to_log(LOG_INFO, "ast-verify: diff touches no macro/AST surface - skipped\n")
        return true
    }
    let das_files <- changed_files("*.das")
    if (empty(das_files)) {
        to_log(LOG_INFO, "ast-verify: no .das changes - skipped\n")
        return true
    }
    var argv <- [daslang, "--ast-verify-batch", "-compile-only"]
    argv |> push_from(das_files)
    var out = ""
    run(argv, out, 1800.0)
    if (find(out, "AST verify") >= 0) {
        to_log(LOG_ERROR, "ast-verify: reports found - a report is a bug in whatever built the node; plain --ast-verify on ONE file names the pass (skills/das_macros.md)\n{out}\n")
        return false
    }
    to_log(LOG_INFO, "ast-verify: clean over {length(das_files)} changed files\n")
    return true
}

def private gate_jit_smoke(daslang : string) : bool {
    if (!diff_touches(["modules/dasLLVM/", "include/daScript/simulate/", "src/simulate/"])) {
        to_log(LOG_INFO, "jit-smoke: diff touches no JIT surface - skipped\n")
        return true
    }
    for (test in ["tests/jit_tests/array.das", "tests/decs/test_bulk_create.das"]) {
        var out = ""
        run([daslang, "-jit", test], out, 600.0)
        if (find(to_lower(out), "verifier") >= 0 || find(out, "Both operands") >= 0) {
            to_log(LOG_ERROR, "jit-smoke: verifier error in {test}\n{out}\n")
            return false
        }
    }
    to_log(LOG_INFO, "jit-smoke: no verifier errors (clang-cl link failures are local-only and ignored)\n")
    return true
}

def private print_judgment_checklist() {
    to_log(LOG_INFO, "\nall mechanical gates green. STILL YOURS (skills/internal/make_pr.md):\n")
    to_log(LOG_INFO, "  - agent fan-out: review-md-auditor per checklist, tdd-auditor, style-hygiene-auditor, woodpecker\n")
    to_log(LOG_INFO, "  - workaround audit: read the whole diff, surface fix-vs-workaround, ASK\n")
    to_log(LOG_INFO, "  - dupe triage of the report above\n")
    to_log(LOG_INFO, "  - docs substeps when publics/RST changed; .md stop; PR body template; babysit after open\n")
}

def private want(gate : string) : bool {
    if (!empty(g_cfg.only)) {
        return g_cfg.only == gate
    }
    for (s in g_cfg.skip) {
        if (s == gate) {
            return false
        }
    }
    return true
}

[export]
def main() : int {
    let rc = parse_args_with_help(g_cfg, "make-pr")
    return rc if (rc >= 0)
    let daslang = find_daslang()
    if (empty(daslang)) {
        to_log(LOG_ERROR, "make-pr: no repo-local daslang binary found\n")
        return 1
    }
    if ((want("sync") && !gate_sync()) ||
            (want("review-md") && !gate_review_md(daslang)) ||
            (want("dupes") && !gate_dupes(daslang)) ||
            (want("ast-verify") && !gate_ast_verify(daslang)) ||
            (want("jit-smoke") && !gate_jit_smoke(daslang))) {
        return 2
    }
    if (empty(g_cfg.only) && !g_cfg.no_preflight) {
        var out = ""
        let code = run([daslang, "utils/internal/preflight/main.das", "--", "--full"], out, 7200.0)
        to_log(LOG_INFO, out)
        if (code != 0) {
            to_log(LOG_ERROR, "preflight --full failed - fix each, validate with the TARGETED gate, never a second full run\n")
            return 2
        }
    }
    if (empty(g_cfg.only)) {
        print_judgment_checklist()
    }
    return 0
}
