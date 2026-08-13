options gen2
options indenting = 4
options persistent_heap

// preflight — run CI's gates locally before pushing. Gate ↔ CI-lane mapping
// and the manual commands this tool automates: skills/preflight.md.
//
// Fast tier (default): format --verify, lint on changed .das, clang
// frontend (syntax-only) pass on changed C++ — escalates to a full
// src+tests-cpp sweep when a header changed, since a header edit can break
// template instantiation in TUs the diff never touched. --full adds the
// untracked-files gate, dasgen freshness, the CI-only-das compile sweep, the
// six doc gates, tests-cpp (ctest -L small), interpreter/JIT/AOT suites, and
// the sequence smoke.
//
// Cross-platform: Windows / macOS / Linux+WSL. Subprocesses go through
// popen_argv (no shell, no quoting trap), with one exception: the sequence
// gate runs CI's own smoke scripts under pwsh/bash by design. A gate whose
// host tool or module is missing reports SKIP with an install/rebuild hint,
// never a silent pass.

require daslib/clargs
require daslib/fio
require daslib/strings_boost
require strings
require ../common/parallel_workers.das
require ../common/git_signature.das
require ../../dastest/worker_policy.das
require config


[CommandLineArgs]
struct Config {
    @clarg_doc = "Run the full tier (adds untracked, dasgen, ci-das, docs, tests-cpp, interp/JIT/AOT suites, sequence smoke)"
    full : bool

    @clarg_doc = "Diff base for changed-file detection (default: origin/master)"
    base : string = "origin/master"

    @clarg_doc = "Comma-separated gate subset to run (see --list-gates); overrides tier selection"
    only : string

    @clarg_doc = "Comma-separated gates to skip"
    skip : string

    @clarg_name = "daslang"
    @clarg_doc = "daslang binary for child invocations (default: DASLANG env, then bin/, build/ probe)"
    daslang_bin : string

    @clarg_doc = "CMake build directory (default: build)"
    build_dir : string = "build"

    @clarg_doc = "clang / clang-cl binary for the C++ syntax pass (default: probe PATH)"
    clang : string

    @clarg_short = "j"
    @clarg_doc = "Parallel width for the C++ syntax pass; 0 = hardware threads"
    jobs : int

    @clarg_doc = "Per-user preflight TOML (default: platform config directory)"
    config_file : string

    @clarg_doc = "JIT isolated workers; -1 = config/default, 0 = automatic, positive = explicit"
    jit_jobs : int = -1

    @clarg_doc = "JIT per-file wall-time guard in seconds; -1 = config/default, 0 = disabled"
    jit_max_file_time : float = -1.0

    @clarg_doc = "Stop at the first failing gate"
    fail_fast : bool

    @clarg_doc = "List gates with tier and description, then exit"
    list_gates : bool

    @clarg_short = "v"
    @clarg_doc = "Print child output for passing gates too"
    verbose : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

enum GateStatus {
    Pass
    Fail
    Skip
}

struct GateResult {
    name : string
    status : GateStatus
    seconds : double
    detail : string                 // one-line summary: skip reason / failure hint
    output : string                 // child output, printed on FAIL or --verbose
}

struct PreflightCtx {
    daslang : string
    build_dir : string              // "" when no configured build tree found
    multi_config : bool
    config : string                 // Release / Debug / RelWithDebInfo
    clang : string                  // "" when not found
    clang_is_cl : bool
    base : string
    changed_das : array<string>
    changed_cpp : array<string>
    changed_hdr : int
    jobs : int
    jit_jobs : int
    jit_max_file_time : float
    verbose : bool
}

// ===== subprocess + small helpers =====

def run_argv(args : array<string>; timeout : float) : tuple<rc : int; out : string> {
    var output : string
    let rc = unsafe(popen_argv(args, timeout) $(f) {
        if (f != null) {
            output = fread(f)
        }
    })
    return (rc = rc, out = output)
}

def run_argv(args : array<string>) : tuple<rc : int; out : string> {
    return run_argv(args, 0.0)
}

// A dynamically-linked artifact spawned outside the daslang bin dir (the exe rail's
// temp-path das-lint) resolves libDaScriptDyn only through the loader's environment —
// prepend the bin dir so spawned children inherit it.
def prepend_loader_search_path(bin_dir : string) {
    let platform = get_platform_name()
    let vname = platform == "windows" ? "PATH" : (platform == "darwin" ? "DYLD_LIBRARY_PATH" : "LD_LIBRARY_PATH")
    let sep = platform == "windows" ? ";" : ":"
    let cur = get_env_variable(vname)
    set_env_variable(vname, empty(cur) ? bin_dir : "{bin_dir}{sep}{cur}")
}

def tool_available(exe, probe_flag : string) : bool {
    let r = run_argv([exe, probe_flag], 30.0)
    return r.rc == 0
}

def tool_available(exe : string) : bool {
    return tool_available(exe, "--version")
}

def seconds_since(t0 : int64) : double {
    return double(get_time_usec(t0)) / 1000000.0lf
}

def split_csv(s : string) : array<string> {
    var out : array<string>
    for (part in split(s, ",")) {
        let p = strip(part)
        if (!empty(p)) {
            out |> push(p)
        }
    }
    return <- out
}

def temp_dir_or_cwd() : string {
    var err : string
    let td = temp_directory(err)
    return empty(td) ? "." : td
}

// unique per run — the system temp dir is shared across checkouts, so a
// fixed name lets concurrent preflights clobber each other's files
def unique_temp_path(prefix, suffix : string) : string {
    return path_join(temp_dir_or_cwd(), "{prefix}_{ref_time_ticks()}{suffix}")
}

def non_empty_lines(s : string) : array<string> {
    var out : array<string>
    for (raw in split(s, "\n")) {
        let line = strip(replace(raw, "\r", ""))
        if (!empty(line)) {
            out |> push(line)
        }
    }
    return <- out
}

// ===== environment discovery =====

def find_daslang(cli_path : string) : string {
    if (!empty(cli_path)) {
        return fexist(cli_path) ? cli_path : ""
    }
    // Repo-local binaries FIRST. daslang derives its module root from the executable's location,
    // so a DASLANG env var pointing at ANOTHER checkout silently resolves in-tree modules
    // (spirv/, ...) against THAT tree — any PR changing them then fails child gates with
    // phantom errors (the worktree trap). The env var is only a fallback for treeless setups.
    // Among the local candidates the NEWEST wins: a tree often carries several flavors (the MSVC
    // bin/Release binary next to an MCP-setup bin/daslang stub), and a stale stub paired with
    // newer-relinked runtime DLLs fails the try/recover-heavy suites with phantom errors.
    let candidates = [
        "bin/daslang", "bin/daslang.exe",
        "bin/Release/daslang.exe", "bin/RelWithDebInfo/daslang.exe", "bin/Debug/daslang.exe",
        "build/daslang", "build/bin/daslang"
    ]
    var best = ""
    var best_mtime = 0l
    for (c in candidates) {
        if (fexist(c)) {
            let mt = stat_mtime_or_zero(c)
            if (mt > best_mtime) {
                best_mtime = mt
                best = c
            }
        }
    }
    if (!empty(best)) {
        // absolutize: child gates spawn this via popen_argv, whose CreateProcess does not
        // resolve a bare relative exe path the way a shell does
        return get_full_file_name(best)
    }
    if (has_env_variable("DASLANG")) {
        let env = get_env_variable("DASLANG")
        if (fexist(env)) {
            to_log(LOG_WARNING, "preflight: no repo-local daslang found; falling back to DASLANG={env} — a binary from another checkout resolves in-tree modules against THAT tree\n")
            return env
        }
    }
    return ""
}

def detect_build_config(daslang : string) : string {
    let generic = to_generic_path(daslang)
    if (find(generic, "/Debug/") >= 0) return "Debug"
    if (find(generic, "/RelWithDebInfo/") >= 0) return "RelWithDebInfo"
    return "Release"
}

def is_multi_config(build_dir : string) : bool {
    let cache = fread(path_join(build_dir, "CMakeCache.txt"))
    for (line in split(cache, "\n")) {
        if (starts_with(line, "CMAKE_GENERATOR:INTERNAL=")) {
            return find(line, "Visual Studio") >= 0 || find(line, "Xcode") >= 0 || find(line, "Multi-Config") >= 0
        }
    }
    return false
}

def find_vs_clang_cl() : string {
    // CI's clang-cl lane builds with the VS ClangCL toolset, i.e. the
    // VS-bundled clang — the most CI-faithful binary on a Windows box.
    // (A newer standalone LLVM emits diagnostics CI never sees: clang 22
    // rejects vecmath volatile-union passing that VS clang 19 accepts.)
    var pf86 = "C:/Program Files (x86)"
    if (has_env_variable("ProgramFiles(x86)")) {
        pf86 = get_env_variable("ProgramFiles(x86)")
    }
    let vswhere = path_join(pf86, "Microsoft Visual Studio/Installer/vswhere.exe")
    if (fexist(vswhere)) {
        let r = run_argv([vswhere, "-latest", "-products", "*", "-find", "VC\\Tools\\Llvm\\x64\\bin\\clang-cl.exe"], 60.0)
        if (r.rc == 0) {
            let lines <- non_empty_lines(r.out)
            if (!empty(lines) && fexist(lines[0])) return lines[0]
        }
    }
    return ""
}

def find_clang(cli_path, build_dir : string) : tuple<exe : string; is_cl : bool> {
    if (!empty(cli_path)) {
        let is_cl = find(base_name(cli_path), "clang-cl") >= 0
        return (exe = tool_available(cli_path) ? cli_path : "", is_cl = is_cl)
    }
    if (get_platform_name() == "windows") {
        let vs_clang = find_vs_clang_cl()
        if (!empty(vs_clang) && tool_available(vs_clang)) return (exe = vs_clang, is_cl = true)
        if (tool_available("clang-cl")) return (exe = "clang-cl", is_cl = true)
        // the prebuilt LLVM package dasLLVM fetches ships clang-cl next to
        // LLVM.dll — present on any Windows box that built with dasLLVM,
        // and /Zs needs no linker so the package's missing lld doesn't matter.
        // It must still carry clang's RESOURCE HEADERS (lib/clang/<ver>/include):
        // without them clang-cl falls back to MSVC's intrinsic headers, where
        // __m128 is not a clang vector type, and every vecmath TU false-fails
        // on __builtin_shufflevector — a header-less package (r4 ships only the
        // three exes) is a missing tool, not a code failure.
        if (!empty(build_dir)) {
            let bundled_dir = path_join(build_dir, "_deps/das_llvm_shared_lib-src")
            let bundled = path_join(bundled_dir, "clang-cl.exe")
            if (fexist(bundled) && tool_available(bundled) && stat(path_join(bundled_dir, "lib/clang")).is_valid) {
                return (exe = bundled, is_cl = true)
            }
        }
        if (tool_available("clang")) return (exe = "clang", is_cl = false)
    } else {
        if (tool_available("clang++")) return (exe = "clang++", is_cl = false)
        if (tool_available("clang")) return (exe = "clang", is_cl = false)
    }
    return (exe = "", is_cl = false)
}

// sphinx-build is often OFF PATH on dev boxes: pip --user puts it in
// ~/Library/Python/<ver>/bin (mac) / ~/.local/bin (linux). PATH wins when present.
def find_sphinx_build() : string {
    if (tool_available("sphinx-build")) return "sphinx-build"
    if (!has_env_variable("HOME")) return ""
    let home = get_env_variable("HOME")
    var candidates : array<string>
    if (get_platform_name() == "darwin") {
        expand_glob("{home}/Library/Python/*/bin/sphinx-build", candidates)
    }
    candidates |> push("{home}/.local/bin/sphinx-build")
    for (p in candidates) {
        if (fexist(p) && tool_available(p)) return p
    }
    return ""
}

// TUs the clang frontend pass can compile standalone: core + tests-cpp.
// Module/util TUs need external or build-generated headers; CI's clang-cl
// lane covers those.
def in_core_scope(path : string) : bool {
    let g = to_generic_path(path)
    return starts_with(g, "src/") || starts_with(g, "include/") || starts_with(g, "tests-cpp/")
}

def collect_changed_files(base : string; var das_files, cpp_files : array<string>&; var hdr_count : int&) {
    var seen : table<string>
    let probe = run_argv(["git", "rev-parse", "--verify", "--quiet", base])
    var ranges : array<string>
    if (probe.rc == 0) {
        ranges |> push("{base}...HEAD")
    } else {
        to_log(LOG_WARNING, "preflight: diff base '{base}' not found; only uncommitted changes are considered\n")
    }
    ranges |> push("HEAD")          // staged + unstaged vs HEAD
    for (r in ranges) {
        let d = run_argv(["git", "diff", "--name-only", "--diff-filter=ACMR", r])
        continue if (d.rc != 0)
        for (f in non_empty_lines(d.out)) {
            continue if (key_exists(seen, f))
            seen |> insert(f)
            continue if (!fexist(f))
            let ext = extension(f)
            if (ext == ".das") {
                das_files |> push(f)
            } elif (ext == ".cpp" || ext == ".cc") {
                cpp_files |> push(f)
            } elif (ext == ".h" || ext == ".hpp" || ext == ".inc") {
                // core-scope only — the sole consumer is the cpp-syntax
                // full-sweep trigger, and module headers don't ripple into
                // core TUs (modules depend on core, not the other way)
                if (in_core_scope(f)) {
                    hdr_count ++
                }
            }
        }
    }
}

// ===== gate runners =====

// PR-time hygiene: leftover session goo must not ride into or linger past a PR.
def gate_untracked() : GateResult {
    let t0 = ref_time_ticks()
    let ls = run_argv(["git", "ls-files", "--others", "--exclude-standard"])
    if (ls.rc != 0) {
        return GateResult(name = "untracked", status = GateStatus.Fail, seconds = seconds_since(t0),
            detail = "git ls-files failed", output = ls.out)
    }
    var inscope files <- untracked_from_ls(ls.out)
    if (empty(files)) {
        return GateResult(name = "untracked", status = GateStatus.Pass, seconds = seconds_since(t0))
    }
    return GateResult(name = "untracked", status = GateStatus.Fail, seconds = seconds_since(t0),
        detail = "{length(files)} untracked file(s) — commit, delete, or ignore each (.gitignore pattern, or .git/info/exclude for box-local)",
        output = ls.out)
}

def gate_format(ctx : PreflightCtx) : GateResult {
    let t0 = ref_time_ticks()
    // verify TRACKED files only (CI's own scope), so gitignored scratch that
    // never reaches CI can't fail the gate
    let ls = run_argv(["git", "ls-files", "--cached", "--", "*.das"])
    if (ls.rc != 0) {
        return GateResult(name = "format", status = GateStatus.Fail, seconds = seconds_since(t0),
            detail = "git ls-files failed", output = ls.out)
    }
    let list_file = unique_temp_path("preflight_fmt_list", ".txt")
    if (!fwrite(list_file, ls.out)) {
        return GateResult(name = "format", status = GateStatus.Fail, seconds = seconds_since(t0),
            detail = "cannot write {list_file}")
    }
    let r = run_argv([ctx.daslang, "utils/das-fmt/dasfmt.das", "--", "--files-from", list_file, "--verify"])
    remove(list_file)
    return GateResult(name = "format", status = r.rc == 0 ? GateStatus.Pass : GateStatus.Fail,
        seconds = seconds_since(t0),
        detail = r.rc == 0 ? "" : "unformatted files — run the MCP format_file tool (or dasfmt without --verify)",
        output = r.out)
}

// CI's lint verdict is rendered ONLY by the linux lane (extended_checks gates the step on
// matrix.target == 'linux'), in the linux MODULE WORLD (no dasMetal/dasVulkan — platform
// static_if halves compile out, so their requires/args read unused there and nowhere else),
// and TWICE (interp, then the -exe compiled das-lint, which resolves modules differently).
// A host-flavor pass alone is structurally blind to both — so this gate runs three rails.
def gate_lint(ctx : PreflightCtx) : GateResult {
    let t0 = ref_time_ticks()
    if (empty(ctx.changed_das)) {
        return GateResult(name = "lint", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "no .das files changed vs {ctx.base}")
    }
    var files <- [for (f in ctx.changed_das); f]
    var host_args <- [ctx.daslang, "utils/lint/main.das", "--"]
    host_args |> push_from(files)
    host_args |> push("--quiet")
    let host = run_argv(host_args)
    if (host.rc != 0) {
        return GateResult(name = "lint", status = GateStatus.Fail, seconds = seconds_since(t0),
            detail = "host-flavor lint warnings — fix or nolint each (see skills/make_pr.md §1)", output = host.out)
    }
    var mirror_args <- [ctx.daslang, "--disable-module", "dasMetal", "--disable-module", "dasVulkan",
                        "utils/lint/main.das", "--"]
    mirror_args |> push_from(files)
    mirror_args |> push("--quiet")
    let mirror = run_argv(mirror_args)
    if (mirror.rc != 0) {
        return GateResult(name = "lint", status = GateStatus.Fail, seconds = seconds_since(t0),
            detail = "LINUX-lane lint warnings (host pass was clean — a platform static_if half hides the symbol here; nolint:...,LINT019 both-worlds spelling, see skills/preflight.md)", output = mirror.out)
    }
    // the exe rail: CI compiles utils/lint into a standalone das-lint and lints AGAIN — the
    // -exe world resolves modules differently (dasAudio et al are not baked in). `-exe`
    // appends .exe on every platform (CI's LINUX lane invokes $BIN/das-lint.exe verbatim)
    let lint_exe_base = unique_temp_path("preflight_das_lint", "")
    let build = run_argv([ctx.daslang, "-exe", "-output", lint_exe_base, "utils/lint/main.das"])
    if (build.rc != 0) {
        remove("{lint_exe_base}.exe")
        remove("{lint_exe_base}.o")
        return GateResult(name = "lint", status = GateStatus.Fail, seconds = seconds_since(t0),
            detail = "das-lint -exe build failed — the CI exe rail would fail the same way", output = build.out)
    }
    // the -exe artifact is dynamically linked (libDaScriptDyn); from the temp path the
    // loader resolves it only via the environment — point it at the daslang bin dir
    prepend_loader_search_path(dir_name(ctx.daslang))
    var exe_args <- ["{lint_exe_base}.exe"]
    exe_args |> push_from(files)
    exe_args |> push("--quiet")
    let exe = run_argv(exe_args)
    remove("{lint_exe_base}.exe")
    remove("{lint_exe_base}.o")
    if (exe.rc != 0) {
        return GateResult(name = "lint", status = GateStatus.Fail, seconds = seconds_since(t0),
            detail = "das-lint.exe rail warnings (interp rails were clean — the -exe module world differs)", output = exe.out)
    }
    return GateResult(name = "lint", status = GateStatus.Pass, seconds = seconds_since(t0),
        detail = "{length(files)} file(s) clean on all three rails (host, linux-mirror, exe)")
}

def gate_cpp_syntax(ctx : PreflightCtx) : GateResult { // nolint:STYLE038
    let t0 = ref_time_ticks()
    if (empty(ctx.changed_cpp) && ctx.changed_hdr == 0) {
        return GateResult(name = "cpp-syntax", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "no C++ changed vs {ctx.base}")
    }
    if (empty(ctx.clang)) {
        return GateResult(name = "cpp-syntax", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "clang not found — install the VS ClangCL component (CI-faithful) or LLVM clang-cl on PATH, or pass --clang <path>; the dasLLVM prebuilt's clang-cl has no resource headers and is skipped")
    }
    var in_scope : array<string>
    var skipped : array<string>
    var sweep_note = ""
    if (ctx.changed_hdr > 0) {
        // a changed header can break template instantiation in any TU that
        // includes it — frontend-sweep ALL core TUs instead of just the
        // changed ones (no codegen, so the full sweep stays in seconds)
        let ls = run_argv(["git", "ls-files", "--", "src", "tests-cpp"])
        if (ls.rc != 0) {
            return GateResult(name = "cpp-syntax", status = GateStatus.Fail, seconds = seconds_since(t0),
                detail = "git ls-files failed (rc={ls.rc}) — cannot enumerate TUs for the header-change sweep", output = ls.out)
        }
        for (f in non_empty_lines(ls.out)) {
            let ext = extension(f)
            continue if ((ext != ".cpp" && ext != ".cc") || starts_with(f, "tests-cpp/3rdparty/"))
            in_scope |> push(f)
        }
        sweep_note = "{ctx.changed_hdr} changed header(s) → full sweep: "
        for (f in ctx.changed_cpp) {
            if (!in_core_scope(f)) {
                skipped |> push(f)
            }
        }
    } else {
        for (f in ctx.changed_cpp) {
            if (in_core_scope(f)) {
                in_scope |> push(f)
            } else {
                skipped |> push(f)
            }
        }
    }
    if (empty(in_scope)) {
        return GateResult(name = "cpp-syntax", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "changed C++ is outside src/include/tests-cpp ({join(skipped, ", ")}) — needs module/external deps; no local clang gate compiles it, CI's clang-cl lane does")
    }
    var flags : array<string>
    if (ctx.clang_is_cl) {
        flags <- ["/Zs", "/EHsc", "/std:c++17", "/W0"]   // nolint:PERF030 — fresh empty target, branch-initialized
    } else {
        flags <- ["-fsyntax-only", "-std=c++17", "-w"]   // nolint:PERF030 — fresh empty target, branch-initialized
    }
    var includes <- ["include", "3rdparty/fmt/include", "3rdparty/uriparser/include", "tests-cpp/3rdparty"]
    if (!empty(ctx.build_dir)) {
        // configure-time generated headers (modules/external_*.inc) — without
        // this root, TUs like src/simulate/fs_file_info.cpp false-fail
        includes |> push(path_join(ctx.build_dir, "include"))
    }
    let workers = compute_worker_count(length(in_scope), ctx.jobs)
    let chunks <- chunk_files(in_scope, workers)
    var argvs : array<array<string>>
    argvs |> reserve(length(chunks))
    for (chunk in chunks) {
        var av : array<string>
        av |> reserve(1 + length(flags) + length(includes) + length(chunk))
        av |> push(ctx.clang)
        av |> push_from(flags)
        for (inc in includes) {
            av |> push("-I{inc}")
        }
        av |> push_from(chunk)
        argvs |> emplace(av)
    }
    var results <- run_chunk_workers(argvs)
    var failed = false
    for (r in results) {
        if (r.exit_code != 0) {
            failed = true
        }
    }
    let output = build_string() $(w) {
        for (r in results) {
            if (!empty(r.stdout)) {
                w |> write(r.stdout)
                w |> write("\n")
            }
        }
    }
    var detail = "{sweep_note}{length(in_scope)} file(s) via {ctx.clang}"
    if (!empty(skipped)) {
        detail = "{detail}; out of scope: {join(skipped, ", ")}"
    }
    return GateResult(name = "cpp-syntax", status = failed ? GateStatus.Fail : GateStatus.Pass,
        seconds = seconds_since(t0), detail = detail, output = output)
}

def gate_dasgen(ctx : PreflightCtx) : GateResult {
    let t0 = ref_time_ticks()
    let r = run_argv([ctx.daslang, "utils/dasgen/gen_bind.das"])
    if (r.rc != 0) {
        return GateResult(name = "dasgen", status = GateStatus.Fail, seconds = seconds_since(t0),
            detail = "gen_bind.das failed", output = r.out)
    }
    let d = run_argv(["git", "diff", "--exit-code", "--stat", "--", "include/daScript/builtin/"])
    return GateResult(name = "dasgen", status = d.rc == 0 ? GateStatus.Pass : GateStatus.Fail,
        seconds = seconds_since(t0),
        detail = d.rc == 0 ? "" : "generated .inc files out of date — commit the regenerated files",
        output = d.out)
}

// ===== docs gates (mirror doc.yml) =====

def skip_gates(names : array<string>; reason : string; var results : array<GateResult>&) {
    results |> reserve(length(results) + length(names))
    for (g in names) {
        results |> emplace(GateResult(name = g, status = GateStatus.Skip, detail = reason))
    }
}

def skip_sphinx_gates(reason : string; var results : array<GateResult>&) {
    skip_gates(["docs/sphinx-html"], reason, results)
}

def docs_skip_all(reason : string; var results : array<GateResult>&) {
    skip_gates(["docs/das2rst", "docs/imgui2rst", "docs/stubs", "docs/uncategorized", "docs/untracked", "docs/sphinx-html"], reason, results)
}

def gate_docs(ctx : PreflightCtx; var results : array<GateResult>&) { // nolint:STYLE037,STYLE038 — the doc.yml gates in CI order; splitting hides the sequence
    var t0 = ref_time_ticks()
    // gate 1: das2rst runs clean (positional handmade-doc validation)
    let r1 = run_argv([ctx.daslang, "-documentation", "doc/reflections/das2rst.das"])
    if (r1.rc != 0 && find(r1.out, "missing prerequisite") >= 0) {
        docs_skip_all("daslang lacks HV/PUGIXML — rebuild with -DDAS_HV_DISABLED=OFF -DDAS_PUGIXML_DISABLED=OFF", results)
        return
    }
    let tuned_docs = find(r1.out, "llvm_tune:") >= 0
    let das2rst_ok = r1.rc == 0 && !tuned_docs
    results |> emplace(GateResult(name = "docs/das2rst", status = das2rst_ok ? GateStatus.Pass : GateStatus.Fail,
        seconds = seconds_since(t0),
        detail = (das2rst_ok ? "" : (tuned_docs
            ? "documentation compile entered llvm_tune — keep -documentation/is_building_documentation wired"
            : "handmade-doc validation panic — fix and re-run; CI stops at the FIRST panic, more may follow")),
        output = r1.out))

    // gate 1b: imgui2rst — dasImgui's stdlib pages (doc.yml runs it right after
    // das2rst; without them sphinx -W fails on the generated imgui toctree refs)
    t0 = ref_time_ticks()
    let r1b = run_argv([ctx.daslang, "modules/dasImgui/utils/imgui2rst.das"])
    let imgui_pages_ok = r1b.rc == 0
    if (!imgui_pages_ok && find(r1b.out, "missing prerequisite") >= 0) {
        results |> emplace(GateResult(name = "docs/imgui2rst", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "daslang lacks dasImgui — rebuild with -DDAS_IMGUI_DISABLED=OFF", output = r1b.out))
    } else {
        results |> emplace(GateResult(name = "docs/imgui2rst", status = imgui_pages_ok ? GateStatus.Pass : GateStatus.Fail,
            seconds = seconds_since(t0),
            detail = imgui_pages_ok ? "" : "imgui2rst failed — the generated imgui stdlib pages are stale/missing",
            output = r1b.out))
    }

    // gate 2: no `// stub` in handmade docs
    t0 = ref_time_ticks()
    var stubs : array<string>
    dir("doc/source/stdlib/handmade") $(name) {
        return if (extension(name) != ".rst")
        let p = path_join("doc/source/stdlib/handmade", name)
        if (find(fread(p), "// stub") >= 0) {
            stubs |> push(p)
        }
    }
    results |> emplace(GateResult(name = "docs/stubs", status = empty(stubs) ? GateStatus.Pass : GateStatus.Fail,
        seconds = seconds_since(t0),
        detail = empty(stubs) ? "" : "stub docs need real text (skills/make_pr.md §4c)",
        output = join(stubs, "\n")))

    // gate 3: no Uncategorized sections in generated docs
    t0 = ref_time_ticks()
    var uncategorized : array<string>
    dir("doc/source/stdlib/generated") $(name) {
        return if (extension(name) != ".rst")
        let p = path_join("doc/source/stdlib/generated", name)
        for (line in split(fread(p), "\n")) {
            if (strip(replace(line, "\r", "")) == "Uncategorized") {
                uncategorized |> push(p)
                break
            }
        }
    }
    results |> emplace(GateResult(name = "docs/uncategorized", status = empty(uncategorized) ? GateStatus.Pass : GateStatus.Fail,
        seconds = seconds_since(t0),
        detail = empty(uncategorized) ? "" : "add the function(s) to a group_by_regex in doc/reflections/das2rst.das",
        output = join(uncategorized, "\n")))

    // gate 4: no untracked generated RST
    t0 = ref_time_ticks()
    let r4 = run_argv(["git", "ls-files", "--others", "--exclude-standard", "doc/source/stdlib/"])
    let untracked <- non_empty_lines(r4.out)
    let untracked_ok = r4.rc == 0 && empty(untracked)
    results |> emplace(GateResult(name = "docs/untracked", status = untracked_ok ? GateStatus.Pass : GateStatus.Fail,
        seconds = seconds_since(t0),
        detail = untracked_ok ? "" : (r4.rc != 0 ? "git ls-files failed (rc={r4.rc})" : "das2rst generated new files — git add them"),
        output = r4.rc != 0 ? r4.out : join(untracked, "\n")))

    // final gate: sphinx -W html. Delete the doctree cache first — CI builds
    // from a fresh clone, a stale local cache hides warnings.
    let sphinx = find_sphinx_build()
    if (empty(sphinx)) {
        skip_sphinx_gates("sphinx-build not found (PATH, ~/Library/Python/*/bin, ~/.local/bin) — pip install -r doc/requirements.txt", results)
        return
    }
    if (!imgui_pages_ok) {
        skip_sphinx_gates("generated imgui pages unavailable (see docs/imgui2rst) — sphinx -W would red on their toctree refs", results)
        return
    }
    rmdir_rec("doc/sphinx-build")
    if (stat("doc/sphinx-build").is_valid) {
        // running sphinx against a cache we failed to delete would be an
        // untrustworthy PASS — refuse instead
        skip_sphinx_gates("cannot delete doc/sphinx-build (locked?) — remove it and re-run", results)
        return
    }

    // doc.yml stages the docs-assets recordings before sphinx. Stage once per
    // tree — any staged .mp4 counts, so no per-run re-download.
    var staged_recordings = false
    if (stat("doc/source/_static/tutorials").is_valid) {
        dir("doc/source/_static/tutorials") $(name) {
            if (extension(name) == ".mp4") {
                staged_recordings = true
            }
        }
    }
    if (!staged_recordings) {
        let _ = mkdir("doc/source/_static/tutorials")
        let rf = run_argv(["gh", "release", "download", "docs-assets", "--repo", "GaijinEntertainment/daScript",
            "--pattern", "*.mp4", "--dir", "doc/source/_static/tutorials", "--clobber"], 600.0)
        staged_recordings = rf.rc == 0
    }

    t0 = ref_time_ticks()
    let r5 = run_argv([sphinx, "-W", "--keep-going", "-b", "html", "-d", "doc/sphinx-build", "doc/source", "build/site"])
    let html_ok = r5.rc == 0
    // A missing `.. video::` target is a -W warning like any other, so name the
    // unstaged recordings when they are the likely cause rather than leaving the
    // reader to decode a toctree error.
    results |> emplace(GateResult(name = "docs/sphinx-html", status = html_ok ? GateStatus.Pass : GateStatus.Fail,
        seconds = seconds_since(t0),
        detail = html_ok ? "" : (staged_recordings ? "html builder warnings (warnings-as-errors)"
            : "html builder warnings — tutorial recordings are NOT staged (gh auth login, then utils/docs_assets/fetch)"),
        output = r5.out))
}

// ===== CI-only das compile sweep =====

def gate_ci_das(ctx : PreflightCtx) : GateResult {
    let t0 = ref_time_ticks()
    let list_path = "utils/preflight/ci_only_das.txt"
    let list_text = fread(list_path)
    if (empty(list_text)) {
        return GateResult(name = "ci-das", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "{list_path} not found or empty")
    }
    var checked = 0
    var skipped_mods : table<string>
    var fail_parts : array<string>
    for (raw_line in split(list_text, "\n")) {
        let line = strip(replace(raw_line, "\r", ""))
        continue if (empty(line) || starts_with(line, "#"))
        let parts <- split(line, "|")
        continue if (length(parts) < 3)
        let pattern = strip(parts[0])
        let allow_skip <- split_csv(parts[1])
        let hint = strip(parts[2])
        var files : array<string>
        expand_glob(pattern, files)
        if (empty(files)) {
            fail_parts |> push("{pattern}: no files matched (stale ci_only_das.txt entry?)\n")
            continue
        }
        for (f in files) {
            let r = run_argv([ctx.daslang, "-compile-only", f], 300.0)
            if (r.rc == 0) {
                checked ++
                continue
            }
            // a missing NATIVE module from the entry's allow-skip list means the
            // local binary was built without it — environment gap, not a failure
            var module_gap = ""
            for (m in allow_skip) {
                if (find(r.out, "missing prerequisite '{m}'") >= 0 || find(r.out, "module {m} not found") >= 0) {
                    module_gap = m
                    break
                }
            }
            if (!empty(module_gap)) {
                if (!key_exists(skipped_mods, "{module_gap} ({hint})")) {
                    skipped_mods |> insert("{module_gap} ({hint})")
                }
            } else {
                fail_parts |> push("=== {f} ===\n{r.out}\n")
            }
        }
    }
    var detail = "{checked} file(s) compile-checked"
    if (!empty(skipped_mods)) {
        let mods <- [for (k in keys(skipped_mods)); k]
        let mods_text = join(mods, "; ")
        detail = "{detail}; skipped entries needing: {mods_text}"
    }
    if (!empty(fail_parts)) {
        return GateResult(name = "ci-das", status = GateStatus.Fail, seconds = seconds_since(t0),
            detail = "compile errors in CI-only das surface", output = join(fail_parts, ""))
    }
    return GateResult(name = "ci-das", status = checked > 0 ? GateStatus.Pass : GateStatus.Skip,
        seconds = seconds_since(t0), detail = detail)
}

// ===== test suites =====

def run_test_gate(name : string; args : array<string>; fail_hint : string) : GateResult {
    let t0 = ref_time_ticks()
    let r = run_argv(args, 3600.0)
    return GateResult(name = name, status = r.rc == 0 ? GateStatus.Pass : GateStatus.Fail,
        seconds = seconds_since(t0), detail = r.rc == 0 ? "" : fail_hint, output = r.out)
}

// build-gate failure triage. LNK1104 on bin outputs = a live daslang process
// holds the file (make_pr §3). The unavoidable case: a DLL-flavor daslang
// import-links libDaScriptDyn*.dll, so the daslang HOSTING preflight pins
// them and no child build can ever relink — SKIP with a two-step hint
// (CI's static daslang doesn't have this; fresh targets skip the relink).
def classify_build_fail(gate, base : string; r : tuple<rc : int; out : string>; daslang : string; t0 : int64) : GateResult {
    var detail = base
    var status = GateStatus.Fail
    if (find(r.out, "LNK1104") >= 0) {
        if (fexist(path_join(dir_name(daslang), "libDaScriptDyn_runtime.dll"))) {
            status = GateStatus.Skip
            detail = "DLL-flavor daslang pins libDaScriptDyn*.dll, so preflight's own host blocks the relink — build the gate's targets from a plain shell once, then re-run (fresh targets skip the relink)"
        } else {
            detail = "{base} — LNK1104: a running daslang/daslang-live/MCP process is locking build outputs; stop it and re-run"
        }
    }
    return GateResult(name = gate, status = status, seconds = seconds_since(t0), detail = detail, output = r.out)
}

def gate_tests_interp(ctx : PreflightCtx) : GateResult {
    return run_test_gate("tests-interp",
        [ctx.daslang, "dastest/dastest.das", "--", "--failures-only", "--max-file-time", "30", "--timeout", "1800", "--test", "tests"],
        "interpreter suite failures")
}

def gate_tests_jit(ctx : PreflightCtx) : GateResult {
    let t0 = ref_time_ticks()
    // probe: a JIT no-op run tells us whether dasLLVM is in this binary
    let probe_file = unique_temp_path("preflight_jit_probe", ".das")
    if (!fwrite(probe_file, "options gen2\n[export]\ndef main() \{\n    print(\"ok\\n\")\n\}\n")) {
        return GateResult(name = "tests-jit", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "cannot write JIT probe file {probe_file}")
    }
    let probe = run_argv([ctx.daslang, "-jit", probe_file], 600.0)
    remove(probe_file)
    if (probe.rc != 0) {
        return GateResult(name = "tests-jit", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "JIT probe failed (see output) — most common cause: daslang built without dasLLVM (-DDAS_LLVM_DISABLED=OFF)", output = probe.out)
    }
    // Isolated-parallel like CI's run_tests_jit (~3x faster than sequential, identical
    // pass/fail set; workers mint the dll cache themselves). Timeout matches CI's 1800.
    return run_test_gate("tests-jit",
        [ctx.daslang, "dastest/dastest.das", "-jit", "--", "--jit-opt-level=3", "--failures-only",
            "--max-file-time", "{ctx.jit_max_file_time}", "--isolated-mode",
            "--isolated-mode-threads", "{ctx.jit_jobs}", "--batch", "4",
            "--timeout", "1800", "--test", "tests"],
        "JIT suite failures")
}

def gate_tests_cpp(ctx : PreflightCtx) : GateResult {
    let t0 = ref_time_ticks()
    if (empty(ctx.build_dir)) {
        return GateResult(name = "tests-cpp", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "no configured build dir — cmake -B build first")
    }
    var args <- ["ctest", "--test-dir", ctx.build_dir]
    if (ctx.multi_config) {
        args |> push("--build-config")
        args |> push(ctx.config)
    }
    args |> push("-L")
    args |> push("small")
    args |> push("--no-tests=error")        // unbuilt tests-cpp-small must FAIL, not 0-tests-pass
    args |> push("--output-on-failure")
    return run_test_gate("tests-cpp", args, "ctest -L small failures (build tests-cpp-small target if missing)")
}

def gate_tests_aot(ctx : PreflightCtx) : GateResult {
    let t0 = ref_time_ticks()
    if (empty(ctx.build_dir)) {
        return GateResult(name = "tests-aot", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "no configured build dir — cmake -B build first")
    }
    let b = run_argv(["cmake", "--build", ctx.build_dir, "--config", ctx.config, "--target", "test_aot", "--parallel"])
    if (b.rc != 0) {
        // root CMakeLists gates tests/aot out on DAS_AOT_EXAMPLES_DISABLED and
        // 32-bit Windows — an absent target is a config choice, not a failure.
        // Unknown-target spellings: MSBuild / Ninja / Make.
        for (pat in ["MSB1009", "unknown target", "No rule to make target"]) {
            if (find(b.out, pat) >= 0) {
                return GateResult(name = "tests-aot", status = GateStatus.Skip, seconds = seconds_since(t0),
                    detail = "test_aot target not in this build config (DAS_AOT_EXAMPLES_DISABLED, or 32-bit Windows) — the AOT suite runs on 64-bit CI lanes", output = b.out)
            }
        }
        return classify_build_fail("tests-aot",
            "test_aot build failed (new test dirs need tests/aot/CMakeLists.txt registration)", b, ctx.daslang, t0)
    }
    var test_aot = path_join(dir_name(ctx.daslang), "test_aot")
    if (!fexist(test_aot) && fexist("{test_aot}.exe")) {
        test_aot = "{test_aot}.exe"
    }
    if (!fexist(test_aot)) {
        return GateResult(name = "tests-aot", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "test_aot binary not found next to daslang ({test_aot})")
    }
    return run_test_gate("tests-aot",
        [test_aot, "-use-aot", "dastest/dastest.das", "--", "--use-aot", "--failures-only", "--max-file-time", "30", "--timeout", "1800", "--test", "tests"],
        "AOT suite failures (error[50101] → skills/aot_hash_desync_debugging.md)")
}

def gate_sequence(ctx : PreflightCtx) : GateResult {
    let t0 = ref_time_ticks()
    if (empty(ctx.build_dir)) {
        return GateResult(name = "sequence", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "no configured build dir — cmake -B build first")
    }
    let b = run_argv(["cmake", "--build", ctx.build_dir, "--config", ctx.config, "--target",
        "dasModuleGlfw", "dasModuleLiveHost", "dasModuleHV", "dasModuleAudio",
        "dasModulePUGIXML", "dasModuleStbImage", "--parallel"])
    if (b.rc != 0) {
        let lower = to_lower(b.out)
        if (find(lower, "unknown target") >= 0 || find(lower, "no rule to make target") >= 0 || find(lower, "does not exist") >= 0) {
            return GateResult(name = "sequence", status = GateStatus.Skip, seconds = seconds_since(t0),
                detail = "module targets missing — configure with release modules ON (ci/release_modules.txt)", output = b.out)
        }
        return classify_build_fail("sequence", "module build failed", b, ctx.daslang, t0)
    }
    let cwd = getcwd()
    // both smoke scripts honor $BIN (multi-config layouts put daslang in
    // bin/<Config>); daslang has no setenv, so inject via the shell
    let bin_dir = dir_name(ctx.daslang)
    var args : array<string>
    if (get_platform_name() == "windows") {
        let shell = tool_available("pwsh") ? "pwsh" : "powershell"
        // single-quoted PS literals: escape embedded quotes by doubling
        let bin_dir_ps = replace(bin_dir, "'", "''")
        let cwd_ps = replace(cwd, "'", "''")
        let cmd = "$env:BIN = '{bin_dir_ps}'; & 'examples/games/sequence/ci_smoke_test.ps1' '{cwd_ps}'; exit $LASTEXITCODE"
        args <- [shell, "-NoProfile", "-Command", cmd]   // nolint:PERF030 — fresh empty target, branch-initialized
    } else {
        args <- ["env", "BIN={bin_dir}", "bash", "examples/games/sequence/ci_smoke_test.sh", cwd]   // nolint:PERF030 — fresh empty target, branch-initialized
        if (get_platform_name() == "linux" && !has_env_variable("DISPLAY")) {
            // xvfb-run is a wrapper script: -h is its documented flag; --version
            // support varies by distro version
            if (tool_available("xvfb-run", "-h")) {
                var wrapped <- ["xvfb-run", "-a"]
                wrapped |> push_from(args)
                delete args
                args <- wrapped   // nolint:PERF030 — old args deleted above
            } else {
                return GateResult(name = "sequence", status = GateStatus.Skip, seconds = seconds_since(t0),
                    detail = "headless linux without xvfb-run — apt-get install xvfb")
            }
        }
    }
    return run_test_gate("sequence", args, "sequence smoke failed (examples/games/sequence/ci_smoke_test.*)")
}

def gate_imgui(ctx : PreflightCtx) : GateResult {
    let t0 = ref_time_ticks()
    if (empty(ctx.build_dir)) {
        return GateResult(name = "imgui", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "no configured build dir — cmake -B build first")
    }
    let b = run_argv(["cmake", "--build", ctx.build_dir, "--config", ctx.config, "--target",
        "daslang-live", "dasModuleLiveHost", "dasModuleGlfw", "dasModuleHV", "dasModuleClipboard",
        "dasModulePUGIXML", "dasModuleStbImage", "dasModuleImgui", "imguiApp", "imguiAppHeadless", "--parallel"])
    if (b.rc != 0) {
        let lower = to_lower(b.out)
        if (find(lower, "unknown target") >= 0 || find(lower, "no rule to make target") >= 0 || find(lower, "does not exist") >= 0) {
            return GateResult(name = "imgui", status = GateStatus.Skip, seconds = seconds_since(t0),
                detail = "imgui module targets missing — configure with DAS_IMGUI_DISABLED=OFF (needs dasGlfw + dasClipboard on)", output = b.out)
        }
        return classify_build_fail("imgui", "imgui module build failed", b, ctx.daslang, t0)
    }
    // Direct folder targeting bypasses the tests/.das_test nightly gate by
    // design — no --imgui flag needed here (that opt-in is for full sweeps).
    return run_test_gate("imgui",
        [ctx.daslang, "dastest/dastest.das", "--", "--test", "modules/dasImgui/tests", "--headless",
         "--isolated-mode", "--isolated-mode-threads", "4", "--timeout", "600", "--failures-only",
         "--exclude", "glfw_synth", "--exclude", "key_hud", "--exclude", "embedded_terminal"],
        "dasImgui playwright suite failed (nightly_imgui.yml is its only CI lane — this local gate is the only pre-merge check)")
}

// ===== orchestration =====

struct GateInfo {
    name : string
    full_only : bool
    doc : string
}

def gate_table() : array<GateInfo> {
    return <- [
        GateInfo(name = "untracked", full_only = true, doc = "no untracked files at PR time — commit, delete, or ignore each"),
        GateInfo(name = "format", full_only = false, doc = "formatter --verify on tracked .das (mirrors CI)"),
        GateInfo(name = "lint", full_only = false, doc = "lint changed .das on three rails (host, linux-mirror, -exe), zero warnings"),
        GateInfo(name = "cpp-syntax", full_only = false, doc = "clang frontend pass on changed C++; header change → full src+tests-cpp sweep"),
        GateInfo(name = "dasgen", full_only = true, doc = "gen_bind.das freshness vs include/daScript/builtin/"),
        GateInfo(name = "ci-das", full_only = true, doc = "compile-only sweep of CI-only das surface (ci_only_das.txt)"),
        GateInfo(name = "docs", full_only = true, doc = "the six doc.yml gates (das2rst, imgui2rst, stubs, uncategorized, untracked, sphinx html)"),
        GateInfo(name = "tests-cpp", full_only = true, doc = "ctest -L small"),
        GateInfo(name = "tests-interp", full_only = true, doc = "full interpreter suite"),
        GateInfo(name = "tests-jit", full_only = true, doc = "full JIT suite (skips when dasLLVM absent)"),
        GateInfo(name = "tests-aot", full_only = true, doc = "build test_aot + full AOT suite"),
        GateInfo(name = "sequence", full_only = true, doc = "sequence game release smoke (only pre-merge compile of GLFW-gated das)"),
        GateInfo(name = "imgui", full_only = true, doc = "dasImgui playwright suite (nightly-only CI — this local mirror is the only pre-merge gate for it)")
    ]
}

def want_gate(cfg : Config; info : GateInfo) : bool {
    let only_list <- split_csv(cfg.only)
    let skip_list <- split_csv(cfg.skip)
    for (s in skip_list) {
        if (s == info.name) return false
    }
    if (!empty(only_list)) {
        for (o in only_list) {
            if (o == info.name) return true
        }
        return false
    }
    return cfg.full || !info.full_only
}

def status_tag(s : GateStatus) : string {
    if (s == GateStatus.Pass) return "PASS"
    if (s == GateStatus.Fail) return "FAIL"
    return "SKIP"
}

def report_gate(r : GateResult; verbose : bool) {
    let secs = fmt(":.1f", float(r.seconds))
    var timing = ""
    if (r.seconds > 0.05lf) {
        timing = " ({secs}s)"
    }
    var detail = ""
    if (!empty(r.detail)) {
        detail = " — {r.detail}"
    }
    let level = r.status == GateStatus.Fail ? LOG_ERROR : LOG_INFO
    let tag = status_tag(r.status)
    to_log(level, "[{tag}] {r.name}{timing}{detail}\n")
    if ((r.status == GateStatus.Fail || verbose) && !empty(r.output)) {
        to_log(level, "{r.output}\n")
    }
}

[export]
def main() : int { // nolint:STYLE037,STYLE038 — the gate loop + CLI surface; the phases ARE the readability
    var args_r <- parse_args(type<Config>)
    if (args_r |> is_err) {
        to_log(LOG_ERROR, "error: {args_r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Config>), "preflight")
        return 1
    }
    let cfg <- args_r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "preflight")
        return 0
    }
    if (cfg.list_gates) {
        for (info in gate_table()) {
            to_log(LOG_INFO, "{info.name}\t{info.full_only ? "full" : "fast"}\t{info.doc}\n")
        }
        return 0
    }
    if (!fexist("dastest/dastest.das") || !fexist("CMakeLists.txt")) {
        to_log(LOG_ERROR, "preflight must run from the daslang repo root (dastest/dastest.das not found in cwd)\n")
        return 1
    }

    if (cfg.jit_jobs < -1) {
        to_log(LOG_ERROR, "--jit-jobs must be -1 (config/default), 0 (automatic), or positive\n")
        return 1
    }
    if (!valid_jit_max_file_time_override(cfg.jit_max_file_time)) {
        to_log(LOG_ERROR, "--jit-max-file-time must be -1 (config/default) or non-negative\n")
        return 1
    }

    let settings_path = empty(cfg.config_file) ? default_preflight_config_path() : cfg.config_file
    var settings_error : string
    let settings = load_preflight_settings(settings_path, settings_error)
    if (!empty(settings_error)) {
        to_log(LOG_ERROR, "invalid preflight config {settings_path}: {settings_error}\n")
        return 1
    }
    let automatic_jit_jobs = default_jit_isolated_threads(get_total_hw_threads())
    let configured_jit_jobs = settings.jit_threads > 0 ? settings.jit_threads : automatic_jit_jobs
    let jit_jobs = (cfg.jit_jobs < 0
        ? configured_jit_jobs
        : (cfg.jit_jobs == 0 ? automatic_jit_jobs : cfg.jit_jobs))
    let jit_max_file_time = (cfg.jit_max_file_time < 0.0
        ? settings.jit_max_file_time
        : cfg.jit_max_file_time)

    var ctx = PreflightCtx(base = cfg.base, jobs = cfg.jobs,
                           jit_jobs = jit_jobs,
                           jit_max_file_time = jit_max_file_time,
                           verbose = cfg.verbose)
    ctx.daslang = find_daslang(cfg.daslang_bin)
    if (empty(ctx.daslang)) {
        to_log(LOG_ERROR, "daslang binary not found — build it (cmake --build build --target daslang) or pass --daslang / set DASLANG\n")
        return 1
    }
    ctx.config = detect_build_config(ctx.daslang)
    // A complete full run executes every interpreter/JIT/AOT file. Debug
    // is useful for focused diagnosis, but turns that matrix into a tens-of-
    // minutes trap and does not represent CI performance. Subset runs remain
    // available with --only/--skip when Debug coverage is intentional.
    if (cfg.full && empty(cfg.only) && empty(cfg.skip) && ctx.config != "Release") {
        to_log(LOG_ERROR,
            "full preflight requires a Release daslang; got {ctx.config} at {ctx.daslang}\n" +
            "rebuild Release first (on Windows, stop a locking utils/mcp/main.das host; its watcher restarts it)\n")
        return 1
    }
    if (fexist(path_join(cfg.build_dir, "CMakeCache.txt"))) {
        ctx.build_dir = cfg.build_dir
        ctx.multi_config = is_multi_config(cfg.build_dir)
    }
    let clang_info = find_clang(cfg.clang, ctx.build_dir)
    ctx.clang = clang_info.exe
    ctx.clang_is_cl = clang_info.is_cl
    collect_changed_files(cfg.base, ctx.changed_das, ctx.changed_cpp, ctx.changed_hdr)

    let tier = cfg.full ? "full" : "fast"
    let n_das = length(ctx.changed_das)
    let n_cpp = length(ctx.changed_cpp)
    to_log(LOG_INFO, "preflight: {tier} tier; daslang={ctx.daslang}; base={cfg.base}; {n_das} das / {n_cpp} cpp changed\n")
    let settings_label = fexist(settings_path) ? settings_path : "built-in defaults"
    to_log(LOG_INFO, "preflight: JIT workers={ctx.jit_jobs}; max-file-time={ctx.jit_max_file_time}s; settings={settings_label}\n")

    var results : array<GateResult>
    let t_all = ref_time_ticks()
    for (info in gate_table()) {
        continue if (!want_gate(cfg, info))
        to_log(LOG_INFO, "[RUN ] {info.name}\n")
        if (info.name == "docs") {
            var doc_results : array<GateResult>
            gate_docs(ctx, doc_results)
            for (r in doc_results) {
                report_gate(r, cfg.verbose)
            }
            results |> push_from(doc_results)
        } else {
            var r = GateResult(name = info.name)
            if (info.name == "untracked") {
                r <- gate_untracked()
            } elif (info.name == "format") {
                r <- gate_format(ctx)
            } elif (info.name == "lint") {
                r <- gate_lint(ctx)
            } elif (info.name == "cpp-syntax") {
                r <- gate_cpp_syntax(ctx)
            } elif (info.name == "dasgen") {
                r <- gate_dasgen(ctx)
            } elif (info.name == "ci-das") {
                r <- gate_ci_das(ctx)
            } elif (info.name == "tests-cpp") {
                r <- gate_tests_cpp(ctx)
            } elif (info.name == "tests-interp") {
                r <- gate_tests_interp(ctx)
            } elif (info.name == "tests-jit") {
                r <- gate_tests_jit(ctx)
            } elif (info.name == "tests-aot") {
                r <- gate_tests_aot(ctx)
            } elif (info.name == "sequence") {
                r <- gate_sequence(ctx)
            } elif (info.name == "imgui") {
                r <- gate_imgui(ctx)
            }
            report_gate(r, cfg.verbose)
            results |> emplace(r)
        }
        var any_fail = false
        for (r in results) {
            if (r.status == GateStatus.Fail) {
                any_fail = true
                break
            }
        }
        break if (cfg.fail_fast && any_fail)
    }

    var n_pass = 0
    var n_fail = 0
    var n_skip = 0
    for (r in results) {
        if (r.status == GateStatus.Pass) {
            n_pass ++
        } elif (r.status == GateStatus.Fail) {
            n_fail ++
        } else {
            n_skip ++
        }
    }
    let total_s = fmt(":.1f", float(seconds_since(t_all)))
    to_log(n_fail > 0 ? LOG_ERROR : LOG_INFO,
        "\npreflight: {n_pass} passed, {n_fail} failed, {n_skip} skipped in {total_s}s\n")
    for (r in results) {
        if (r.status != GateStatus.Pass) {
            let tag = status_tag(r.status)
            var detail = ""
            if (!empty(r.detail)) {
                detail = " — {r.detail}"
            }
            to_log(r.status == GateStatus.Fail ? LOG_ERROR : LOG_INFO, "  [{tag}] {r.name}{detail}\n")
        }
    }

    return n_fail > 0 ? 1 : 0
}
