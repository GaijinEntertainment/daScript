options gen2

// make-pr - the mechanical gates of PR preparation that preflight's CI-mirror charter
// does not carry (skills/internal/make_pr.md keeps the policy and the judgment steps).
// Exit codes: 0 all green (judgment checklist printed), 2 a gate failed (named),
// 1 environment/API failure.

require strings
require daslib/fio
require daslib/clargs
require daslib/defer
require daslib/strings_boost
require daslib/json_boost
require gates

[CommandLineArgs]
struct Config {
    @clarg_doc = "Diff base ref"
    base : string = "origin/master"

    @clarg_doc = "Run a single gate: sync | review-md | stamp-reach | dupes | ast-verify | jit-smoke"
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
    let best = pick_newest(["bin/daslang", "bin/daslang.exe", "bin/Release/daslang.exe",
                            "bin/RelWithDebInfo/daslang.exe", "build/daslang", "build/bin/daslang"])
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

// A record store's das rows and a tune sidecar's provenance stamp engine commits; a rebase
// orphans every stamp minted on the pre-rebase branch, and the row then names bytes no
// reader can check out. This runs where the hazard appears - right after sync's rebase, on
// a dev box's full clone - so REVIEW.das stays spawn-free (its corpus rules are JSON-pure).
// Ref-engine rows stamp upstream commits and are not checked. Only stamps the diff ADDS are
// checked: historical rows legitimately carry shas their own PR's squash-merge orphaned.
def private gate_stamp_reach() : bool {
    let added = build_string() $(var writer) {
        for (f in changed_files("*/performance/records/*.json")) {
            var out = ""
            run(["git", "diff", "{g_cfg.base}..HEAD", "--", f], out)
            for (line in split(out, "\n")) {
                if ((line |> starts_with("+")) && !(line |> starts_with("+++"))) {
                    writer |> write(line)
                    writer |> write("\n")
                }
            }
        }
    }
    var stamps : array<string>
    for (f in changed_files("*/performance/records/*.json")) {
        continue if (!fexist(f) || (f |> ends_with("annotations.json")))
        let text = fread(f)
        continue if (empty(text))
        var jerr = ""
        var doc = read_json(text, jerr)
        continue if (doc == null)
        if (f |> find(".tune.") >= 0) {
            let sha = "{doc?["provenance"]?["engine_sha"] ?? ""}"
            if (!empty(sha) && (added |> find(sha) >= 0) && stamps |> find_index(sha) < 0) {
                stamps |> push(sha)
            }
        } elif (doc.value is _array) {
            for (model in doc.value as _array) {
                let runs = model?["runs"]
                continue if (runs == null || !(runs.value is _array))
                for (r in runs.value as _array) {
                    continue if ("{r?["engine"] ?? ""}" != "das")
                    let sha = "{r?["sha"] ?? ""}"
                    if (!empty(sha) && (added |> find(sha) >= 0) && stamps |> find_index(sha) < 0) {
                        stamps |> push(sha)
                    }
                }
            }
        }
        unsafe {
            delete doc
        }
    }
    var ok = true
    for (sha in stamps) {
        var out = ""
        if (run(["git", "cat-file", "-e", "{sha}^\{commit\}"], out) != 0) {
            to_log(LOG_ERROR, "stamp-reach: '{sha}' names no commit in this repo - re-mint or re-stamp (modules/dasLLAMA/performance/REVIEW.md)\n")
            ok = false
        } elif (run(["git", "merge-base", "--is-ancestor", sha, "HEAD"], out) != 0) {
            to_log(LOG_ERROR, "stamp-reach: '{sha}' is not reachable from HEAD - the rebase orphaned it; re-mint or re-stamp (modules/dasLLAMA/performance/REVIEW.md)\n")
            ok = false
        }
    }
    if (ok) {
        to_log(LOG_INFO, "stamp-reach: {length(stamps)} engine stamp(s) in the changed record stores all reachable from HEAD\n")
    }
    return ok
}

def private touched_scopes() : array<string> {
    return <- scopes_of(changed_files("*.das"))
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
    defer() {
        var rmerr = ""
        rmdir_rec(tmp, rmerr)
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
    if (run(against, report, 1800.0) != 0) {
        to_log(LOG_ERROR, "dupes: --against run failed\n{report}\n")
        return false
    }
    to_log(LOG_INFO, "dupes: scope = {join(scopes, ", ")}\n{report}\n")
    to_log(LOG_INFO, "dupes: TRIAGE IS YOURS - exact match means reuse/justify/extract (skills/internal/detect_dupe.md)\n")
    return true
}

def private diff_touches(patterns : array<string>) : bool {
    return any_path_matches(changed_files(), patterns)
}

def private gate_ast_verify(daslang : string) : bool {
    if (!diff_touches(["daslib/", "src/ast/", "utils/"])) {
        to_log(LOG_INFO, "ast-verify: diff touches no macro/AST surface - skipped\n")
        return true
    }
    var das_files <- changed_files("*.das")
    var subject = "{length(das_files)} changed files"
    if (empty(das_files)) {
        das_files |> push("tests/jit_tests/array.das")
        das_files |> push("tests/decs/test_bulk_create.das")
        subject = "the macro-heavy smoke set (C++-only AST change)"
    }
    var argv <- [daslang, "--ast-verify-batch", "-compile-only"]
    argv |> push_from(das_files)
    var out = ""
    let rc = run(argv, out, 1800.0)
    if (find(out, "AST verify") >= 0) {
        to_log(LOG_ERROR, "ast-verify: reports found - a report is a bug in whatever built the node; plain --ast-verify on ONE file names the pass (skills/das_macros.md)\n{out}\n")
        return false
    }
    if (rc != 0 && find(out, "error[") < 0) {
        to_log(LOG_ERROR, "ast-verify: verifier crashed, timed out, or failed to launch (exit {rc}) - an incomplete run is not a pass\n{out}\n")
        return false
    }
    to_log(LOG_INFO, "ast-verify: clean over {subject}\n")
    return true
}

def private gate_jit_smoke(daslang : string) : bool {
    if (!diff_touches(["modules/dasLLVM/", "include/daScript/simulate/", "src/simulate/"])) {
        to_log(LOG_INFO, "jit-smoke: diff touches no JIT surface - skipped\n")
        return true
    }
    for (test in ["tests/jit_tests/array.das", "tests/decs/test_bulk_create.das"]) {
        var out = ""
        let rc = run([daslang, "dastest/dastest.das", "-jit", "--", "--jit-opt-level=3", "--failures-only", "--test", test], out, 600.0)
        if (find(to_lower(out), "verifier") >= 0 || find(out, "Both operands") >= 0) {
            to_log(LOG_ERROR, "jit-smoke: verifier error in {test}\n{out}\n")
            return false
        }
        if (rc != 0 && find(out, "not executable") < 0) {
            to_log(LOG_ERROR, "jit-smoke: {test} exited {rc} - a crashed or failed run is red (only the local clang-cl 'program not executable' link-discovery failure is ignored)\n{out}\n")
            return false
        }
    }
    to_log(LOG_INFO, "jit-smoke: smoke files pass under JIT, no verifier errors (clang-cl link failures are local-only and ignored)\n")
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
    return want_gate(g_cfg.only, g_cfg.skip, gate)
}

[export]
def main() : int {
    let rc = parse_args_with_help(g_cfg, "make-pr")
    return rc if (rc >= 0)
    let bad <- unknown_gate_names(g_cfg.only, g_cfg.skip)
    if (!empty(bad)) {
        to_log(LOG_ERROR, "make-pr: unknown gate name(s) {join(bad, ", ")} (gates: {join(GATE_NAMES, " ")})\n")
        return 1
    }
    let daslang = find_daslang()
    if (empty(daslang)) {
        if (!empty(g_cfg.daslang)) {
            to_log(LOG_ERROR, "make-pr: --daslang '{g_cfg.daslang}' does not exist\n")
        } else {
            to_log(LOG_ERROR, "make-pr: no repo-local daslang binary found\n")
        }
        return 1
    }
    // the default chain: sync, stamp-reach, jit-smoke, then preflight --full. review-md and ast-verify are
    // preflight's fast-tier gates (one pass per gate); dupes is an advisory report off the critical path -
    // each still runs by name through --only
    let named = !empty(g_cfg.only)
    if ((want("sync") && !gate_sync()) ||
            (named && want("review-md") && !gate_review_md(daslang)) ||
            (want("stamp-reach") && !gate_stamp_reach()) ||
            (named && want("dupes") && !gate_dupes(daslang)) ||
            (named && want("ast-verify") && !gate_ast_verify(daslang)) ||
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
