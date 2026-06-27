options gen2
options indenting = 4
options persistent_heap

// preflight — run CI's gates locally before pushing. Gate ↔ CI-lane mapping
// and the manual commands this tool automates: skills/preflight.md.
//
// Fast tier (default): format --verify, lint on changed .das, clang
// frontend (syntax-only) pass on changed C++ — escalates to a full
// src+tests-cpp sweep when a header changed, since a header edit can break
// template instantiation in TUs the diff never touched. --full adds dasgen
// freshness, the CI-only-das compile sweep, the seven doc gates, tests-cpp
// (ctest -L small), interpreter/JIT/AOT suites, and the sequence smoke.
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


[CommandLineArgs]
struct Config {
    @clarg_doc = "Run the full tier (adds dasgen, ci-das, docs, tests-cpp, interp/JIT/AOT suites, sequence smoke)"
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
    verbose : bool
}

// ===== subprocess + small helpers =====

def run_argv(args : array<string>; timeout : float) : tuple<rc : int; out : string> {
    var output : string
    let rc = unsafe(popen_argv(args, timeout) $(f) {
        if (f != null) {
            output := unsafe(fread_to_eof(f))
        }
    })
    return (rc = rc, out = output)
}

def run_argv(args : array<string>) : tuple<rc : int; out : string> {
    return run_argv(args, 0.0)
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
    if (has_env_variable("DASLANG")) {
        let env = get_env_variable("DASLANG")
        if (fexist(env)) return env
    }
    let candidates = [
        "bin/daslang", "bin/daslang.exe",
        "bin/Release/daslang.exe", "bin/RelWithDebInfo/daslang.exe", "bin/Debug/daslang.exe",
        "build/daslang", "build/bin/daslang"
    ]
    for (c in candidates) {
        if (fexist(c)) return c
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
        // and /Zs needs no linker so the package's missing lld doesn't matter
        if (!empty(build_dir)) {
            let bundled = path_join(build_dir, "_deps/das_llvm_shared_lib-src/clang-cl.exe")
            if (fexist(bundled) && tool_available(bundled)) return (exe = bundled, is_cl = true)
        }
        if (tool_available("clang")) return (exe = "clang", is_cl = false)
    } else {
        if (tool_available("clang++")) return (exe = "clang++", is_cl = false)
        if (tool_available("clang")) return (exe = "clang", is_cl = false)
    }
    return (exe = "", is_cl = false)
}

// sphinx-build / latexmk are often OFF PATH on dev boxes: pip --user puts
// sphinx in ~/Library/Python/<ver>/bin (mac) / ~/.local/bin (linux); BasicTeX
// puts latexmk in /Library/TeX/texbin (mac). PATH wins when present.
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

def find_latexmk() : string {
    if (tool_available("latexmk")) return "latexmk"
    let mac_tex = "/Library/TeX/texbin/latexmk"
    if (get_platform_name() == "darwin" && fexist(mac_tex) && tool_available(mac_tex)) return mac_tex
    return ""
}

// latexmk logs run thousands of lines; the actionable error sits at the end.
def last_lines(s : string; n : int) : string {
    let lines <- split(s, "\n")
    let total = length(lines)
    if (total <= n) return s
    var keep : array<string>
    keep |> reserve(n)
    for (i in range(total - n, total)) {
        keep |> push(lines[i])
    }
    return join(keep, "\n")
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

def gate_format(ctx : PreflightCtx) : GateResult {
    let t0 = ref_time_ticks()
    // mirror .githooks/pre-push: verify TRACKED files only, so gitignored
    // scratch that never reaches CI can't fail the gate
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

def gate_lint(ctx : PreflightCtx) : GateResult {
    let t0 = ref_time_ticks()
    if (empty(ctx.changed_das)) {
        return GateResult(name = "lint", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "no .das files changed vs {ctx.base}")
    }
    var args <- [ctx.daslang, "utils/lint/main.das", "--"]
    args |> push_from(ctx.changed_das)
    args |> push("--quiet")
    let r = run_argv(args)
    return GateResult(name = "lint", status = r.rc == 0 ? GateStatus.Pass : GateStatus.Fail,
        seconds = seconds_since(t0),
        detail = r.rc == 0 ? "{length(ctx.changed_das)} file(s) clean" : "lint warnings — fix or nolint each (see skills/make_pr.md §1)",
        output = r.out)
}

def gate_cpp_syntax(ctx : PreflightCtx) : GateResult {
    let t0 = ref_time_ticks()
    if (empty(ctx.changed_cpp) && ctx.changed_hdr == 0) {
        return GateResult(name = "cpp-syntax", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "no C++ changed vs {ctx.base}")
    }
    if (empty(ctx.clang)) {
        return GateResult(name = "cpp-syntax", status = GateStatus.Skip, seconds = seconds_since(t0),
            detail = "clang not found — install LLVM (clang-cl/clang on PATH) or pass --clang <path>")
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
        flags <- ["/Zs", "/EHsc", "/std:c++17", "/W0"]
    } else {
        flags <- ["-fsyntax-only", "-std=c++17", "-w"]
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
    let r = run_argv([ctx.daslang, "dasgen/gen_bind.das"])
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

// ===== docs gates (mirror doc.yml 1-6) =====

def skip_gates(names : array<string>; reason : string; var results : array<GateResult>&) {
    results |> reserve(length(results) + length(names))
    for (g in names) {
        results |> emplace(GateResult(name = g, status = GateStatus.Skip, detail = reason))
    }
}

def skip_sphinx_gates(reason : string; var results : array<GateResult>&) {
    skip_gates(["docs/sphinx-latex", "docs/sphinx-html", "docs/pdflatex"], reason, results)
}

def docs_skip_all(reason : string; var results : array<GateResult>&) {
    skip_gates(["docs/das2rst", "docs/stubs", "docs/uncategorized", "docs/untracked", "docs/sphinx-latex", "docs/sphinx-html", "docs/pdflatex"], reason, results)
}

def gate_docs(ctx : PreflightCtx; var results : array<GateResult>&) {
    var t0 = ref_time_ticks()
    // gate 1: das2rst runs clean (positional handmade-doc validation)
    let r1 = run_argv([ctx.daslang, "doc/reflections/das2rst.das"])
    if (r1.rc != 0 && find(r1.out, "missing prerequisite") >= 0) {
        docs_skip_all("daslang lacks HV/PUGIXML — rebuild with -DDAS_HV_DISABLED=OFF -DDAS_PUGIXML_DISABLED=OFF", results)
        return
    }
    results |> emplace(GateResult(name = "docs/das2rst", status = r1.rc == 0 ? GateStatus.Pass : GateStatus.Fail,
        seconds = seconds_since(t0),
        detail = r1.rc == 0 ? "" : "handmade-doc validation panic — fix and re-run; CI stops at the FIRST panic, more may follow",
        output = r1.out))

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

    // gates 5+6: sphinx -W, latex then html. Delete the doctree cache first —
    // CI builds from a fresh clone, a stale local cache hides warnings.
    let sphinx = find_sphinx_build()
    if (empty(sphinx)) {
        skip_sphinx_gates("sphinx-build not found (PATH, ~/Library/Python/*/bin, ~/.local/bin) — pip install -r doc/requirements.txt", results)
        return
    }
    rmdir_rec("doc/sphinx-build")
    if (stat("doc/sphinx-build").is_valid) {
        // running sphinx against a cache we failed to delete would be an
        // untrustworthy PASS — refuse instead
        skip_sphinx_gates("cannot delete doc/sphinx-build (locked?) — remove it and re-run", results)
        return
    }
    t0 = ref_time_ticks()
    let r5 = run_argv([sphinx, "-W", "--keep-going", "-b", "latex", "-d", "doc/sphinx-build", "doc/source", "build/latex"])
    results |> emplace(GateResult(name = "docs/sphinx-latex", status = r5.rc == 0 ? GateStatus.Pass : GateStatus.Fail,
        seconds = seconds_since(t0), detail = r5.rc == 0 ? "" : "latex builder warnings (warnings-as-errors)", output = r5.out))
    t0 = ref_time_ticks()
    let r6 = run_argv([sphinx, "-W", "--keep-going", "-b", "html", "-d", "doc/sphinx-build", "doc/source", "build/site"])
    results |> emplace(GateResult(name = "docs/sphinx-html", status = r6.rc == 0 ? GateStatus.Pass : GateStatus.Fail,
        seconds = seconds_since(t0), detail = r6.rc == 0 ? "" : "html builder warnings (warnings-as-errors)", output = r6.out))

    // gate 7: compile both PDFs like CI's latex-action does (latexmk; -cd runs in
    // build/latex so the sphinx .sty files resolve). CI marks these steps
    // continue-on-error, so an undeclared unicode char ships broken release PDFs
    // silently — locally it is a FAIL with a one-line remedy.
    if (r5.rc != 0) {
        results |> emplace(GateResult(name = "docs/pdflatex", status = GateStatus.Skip,
            detail = "sphinx-latex failed — no .tex sources to compile"))
        return
    }
    let latexmk = find_latexmk()
    if (empty(latexmk)) {
        results |> emplace(GateResult(name = "docs/pdflatex", status = GateStatus.Skip,
            detail = "latexmk not found (PATH, /Library/TeX/texbin) — brew install --cask basictex (mac) / texlive (linux)"))
        return
    }
    t0 = ref_time_ticks()
    for (root in ["daslangstdlib.tex", "daslang.tex"]) {
        let rl = run_argv([latexmk, "-pdf", "-interaction=nonstopmode", "-halt-on-error", "-cd", "build/latex/{root}"], 900.0)
        if (rl.rc != 0) {
            results |> emplace(GateResult(name = "docs/pdflatex", status = GateStatus.Fail, seconds = seconds_since(t0),
                detail = "{root}: pdflatex rejected the docs — new unicode char? add \\DeclareUnicodeCharacter to doc/source/conf.py 'preamble'",
                output = last_lines(rl.out, 60)))
            return
        }
    }
    results |> emplace(GateResult(name = "docs/pdflatex", status = GateStatus.Pass, seconds = seconds_since(t0)))
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
        [ctx.daslang, "dastest/dastest.das", "--", "--failures-only", "--timeout", "900", "--test", "tests"],
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
    return run_test_gate("tests-jit",
        [ctx.daslang, "dastest/dastest.das", "-jit", "--", "--jit-opt-level=3", "--failures-only", "--timeout", "900", "--test", "tests"],
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
        [test_aot, "-use-aot", "dastest/dastest.das", "--", "--use-aot", "--failures-only", "--timeout", "900", "--test", "tests"],
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
        args <- [shell, "-NoProfile", "-Command", cmd]
    } else {
        args <- ["env", "BIN={bin_dir}", "bash", "examples/games/sequence/ci_smoke_test.sh", cwd]
        if (get_platform_name() == "linux" && !has_env_variable("DISPLAY")) {
            // xvfb-run is a wrapper script: -h is its documented flag; --version
            // support varies by distro version
            if (tool_available("xvfb-run", "-h")) {
                var wrapped <- ["xvfb-run", "-a"]
                wrapped |> push_from(args)
                args <- wrapped
            } else {
                return GateResult(name = "sequence", status = GateStatus.Skip, seconds = seconds_since(t0),
                    detail = "headless linux without xvfb-run — apt-get install xvfb")
            }
        }
    }
    return run_test_gate("sequence", args, "sequence smoke failed (examples/games/sequence/ci_smoke_test.*)")
}

// ===== orchestration =====

struct GateInfo {
    name : string
    full_only : bool
    doc : string
}

def gate_table() : array<GateInfo> {
    return <- [
        GateInfo(name = "format", full_only = false, doc = "formatter --verify on tracked .das (mirrors pre-push + CI)"),
        GateInfo(name = "lint", full_only = false, doc = "lint changed .das, zero warnings"),
        GateInfo(name = "cpp-syntax", full_only = false, doc = "clang frontend pass on changed C++; header change → full src+tests-cpp sweep"),
        GateInfo(name = "dasgen", full_only = true, doc = "gen_bind.das freshness vs include/daScript/builtin/"),
        GateInfo(name = "ci-das", full_only = true, doc = "compile-only sweep of CI-only das surface (ci_only_das.txt)"),
        GateInfo(name = "docs", full_only = true, doc = "the seven doc.yml gates (das2rst, stubs, uncategorized, untracked, sphinx ×2, pdflatex)"),
        GateInfo(name = "tests-cpp", full_only = true, doc = "ctest -L small"),
        GateInfo(name = "tests-interp", full_only = true, doc = "full interpreter suite"),
        GateInfo(name = "tests-jit", full_only = true, doc = "full JIT suite (skips when dasLLVM absent)"),
        GateInfo(name = "tests-aot", full_only = true, doc = "build test_aot + full AOT suite"),
        GateInfo(name = "sequence", full_only = true, doc = "sequence game release smoke (only pre-merge compile of GLFW-gated das)")
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

// ===== preflight token (pre-push gate) =====

// On a clean, complete full pass, mint a token the pre-push hook checks. Bound
// to the HEAD sha so any later commit/amend goes stale; written via
// `git rev-parse --git-path` so each worktree keeps its own. Threat model is
// "forgot to preflight", not a hostile actor — it's a forcing function, not a
// security boundary (`git push --no-verify` is the documented WIP escape, CI
// is the real wall).

def git_line(args : array<string>) : string {
    let r = run_argv(args)
    return r.rc == 0 ? strip(replace(r.out, "\r", "")) : ""
}

def token_path() : string {
    return git_line(["git", "rev-parse", "--git-path", "preflight-token"])
}

def emit_preflight_token() {
    let head = git_line(["git", "rev-parse", "HEAD"])
    if (empty(head)) {
        to_log(LOG_WARNING, "preflight: could not resolve HEAD — no token written\n")
        return
    }
    // The tree we validated must equal HEAD: git push ships the commit, not the
    // working tree, so a dirty tree would certify code that isn't being pushed.
    // (untracked files are ignored — they never reach CI.)
    if (run_argv(["git", "diff", "--quiet", "HEAD"]).rc != 0) {
        to_log(LOG_WARNING, "preflight: working tree differs from HEAD — no token written (commit, then re-run --full)\n")
        return
    }
    let path = token_path()
    if (empty(path)) {
        to_log(LOG_WARNING, "preflight: could not resolve token path — no token written\n")
        return
    }
    if (!fwrite(path, "head={head}\nmode=full\nresult=pass\n")) {
        to_log(LOG_WARNING, "preflight: could not write token at {path}\n")
        return
    }
    to_log(LOG_INFO, "preflight: full-pass token written for {head} — git push allowed\n")
}

// A failed (or now-stale) complete full run must not leave a prior pass token
// behind for a bare `git push` to ride on.
def clear_preflight_token() {
    let path = token_path()
    if (!empty(path) && fexist(path)) {
        remove(path)
    }
}

[export]
def main() : int {
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

    var ctx = PreflightCtx(base = cfg.base, jobs = cfg.jobs, verbose = cfg.verbose)
    ctx.daslang = find_daslang(cfg.daslang_bin)
    if (empty(ctx.daslang)) {
        to_log(LOG_ERROR, "daslang binary not found — build it (cmake --build build --target daslang) or pass --daslang / set DASLANG\n")
        return 1
    }
    ctx.config = detect_build_config(ctx.daslang)
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
            if (info.name == "format") {
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

    // Mint the pre-push token only after a complete full run (no --only/--skip
    // subsetting) that passed. A failed complete full run clears any stale
    // token. Fast tier and subset runs leave a prior full token untouched.
    if (cfg.full && empty(cfg.only) && empty(cfg.skip)) {
        if (n_fail == 0) {
            emit_preflight_token()
        } else {
            clear_preflight_token()
        }
    }
    return n_fail > 0 ? 1 : 0
}
