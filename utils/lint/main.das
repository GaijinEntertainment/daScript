options gen2
options gc
options persistent_heap     // heap_collect requires it; declared rather than inherited from the host's defaults

require daslib/lint
require daslib/perf_lint
require daslib/style_lint
require daslib/lint_config
require daslib/ast_boost
require daslib/fio
require daslib/clargs
require daslib/json
require daslib/json_boost
require math
require strings
require daslib/strings_boost
require daslib/rtti
require ../common/parallel_workers.das
require ../common/arch_citations.das

let JSON_PREFIX = "##lint##\n"

struct LintResult {
    file : string
    count : int
    issues : array<LintIssue>
    failed : bool
    compile_errors : string
    skip_reason : string
    excluded : int      // findings dropped by the .lint_config [paths] exclude policy
}

[CommandLineArgs]
struct Config {
    @clarg_short = "q"
    @clarg_doc = "Suppress PASS lines and progress messages"
    quiet : bool

    @clarg_doc = "Do not print source lines under warnings"
    silent : bool

    @clarg_doc = "Enable STYLE014/STYLE015 comment-length checks"
    comment_hygiene : bool

    @clarg_doc = "Lint fixture-named files too (underscore-led names containing 'fixture'; for tests exercising this tool)"
    lint_fixtures : bool

    @clarg_doc = "Report findings in paths the .lint_config [paths] exclude policy would drop"
    lint_excluded_paths : bool

    @clarg_doc = "Only run paranoid lint (skip perf and style)"
    paranoid_only : bool

    @clarg_doc = "Only run performance lint (skip paranoid and style)"
    perf_only : bool

    @clarg_doc = "Only run style lint (skip paranoid and perf)"
    style_only : bool

    @clarg_short = "j"
    @clarg_doc = "Parallel workers (0 = auto = hardware threads). 1 = serial. Below 16 files, stays serial regardless."
    workers : int

    @clarg_doc = "Worker mode: read newline-delimited file paths and emit one JSON-wrapped LintResult per file on stdout. Internal — used by parallel driver. Skips positional args, banner, summary."
    paths_from : string

    @clarg_short = "d"
    @clarg_doc = "Disable rule(s). Repeatable or comma-separated. Codes only (e.g. PERF001,STYLE024). On overlap with --enable, --disable wins."
    disable : array<string>

    @clarg_short = "e"
    @clarg_doc = "Enable rule(s) — whitelist. If used, only listed rules run (subject to --disable). Repeatable or comma-separated."
    enable : array<string>

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

var private source_text_cache : table<string; string>

// Same shape the *_collect string overloads emit: pass prefix + message + LineInfo::describe location.
def private format_issue(issue : LintIssue) : string {
    let kind = (issue.code |> starts_with("PERF") ? "performance warning: "
        : (issue.code |> starts_with("STYLE") ? "style warning: " : ""))
    let loc = empty(issue.file) ? "" : "{issue.file}:{issue.line}:{issue.column}"
    return "{kind}{issue.message} at {loc}"
}

def private source_line_at(file : string; lineno : int) : string {
    if (!source_text_cache |> key_exists(file)) {
        source_text_cache[file] = fread(file)
    }
    let text = source_text_cache[file]
    return "" if (empty(text))
    var start = 0
    for (_ in range(lineno - 1)) {
        let nl = find(text, "\n", start)
        return "" if (nl < 0)
        start = nl + 1
    }
    let fin = find(text, "\n", start)
    let line = fin < 0 ? slice(text, start) : slice(text, start, fin)
    return line |> strip
}

def write_deduped(var writer : StringBuilderWriter; issues : array<LintIssue>; prefix : string = "  "; show_source : bool = true) {
    var counts : table<string; int>
    var order : array<int>  // index of first occurrence per unique message
    for (i in range(length(issues))) {
        let msg = format_issue(issues[i])
        if (key_exists(counts, msg)) {
            counts[msg]++
        } else {
            counts[msg] = 1
            order |> push(i)
        }
    }
    for (i in order) {
        let msg = format_issue(issues[i])
        let n = counts[msg]
        if (n > 1) {
            writer |> write("{prefix}{msg} (x{n})\n")
        } else {
            writer |> write("{prefix}{msg}\n")
        }
        continue if (!show_source || empty(issues[i].file) || issues[i].line <= 0)
        let src = source_line_at(issues[i].file, issues[i].line)
        if (!empty(src)) {
            writer |> write("{prefix}  > {src}\n")
        }
    }
}

def is_skip_file(name : string) : bool {
    return name == "builtin.das"
}

def is_skip_dir(name : string) : bool {
    return name |> starts_with(".")
}

// A CMake build tree is not source, whatever it is named: FetchContent checkouts under it
// carry .das and .md the tree never wrote
def is_build_tree(path : string) : bool {
    return stat("{path}/CMakeCache.txt").is_valid
}

def scan_das_files(path : string; var files : array<string>; var cache : table<string; void?>; var skipped : int&) {
    let st = stat(path)
    return if (!st.is_valid)
    if (st.is_reg) {
        if (path |> ends_with(".das") && !cache |> key_exists(path)) {
            // extract filename from path
            let last_slash = max(rfind(path, "/"), rfind(path, "\\"))
            let fname = last_slash >= 0 ? slice(path, last_slash + 1) : path
            if (!is_skip_file(fname)) {
                cache |> insert(path, null)
                files |> push(path)
            } else {
                skipped++
            }
        }
        return
    }
    return if (!st.is_dir)
    fio::dir(path) $(name) {
        return if (name == "." || name == ".." || name |> starts_with("_") || is_skip_dir(name))
        let full = "{path}/{name}"
        let fst = stat(full)
        return if (!fst.is_valid)
        if (fst.is_dir) {
            return if (is_build_tree(full))
            scan_das_files(full, files, cache, skipped)
        } elif (fst.is_reg && name |> ends_with(".das") && !cache |> key_exists(full)) {
            if (is_skip_file(name)) {
                skipped++
            } else {
                cache |> insert(full, null)
                files |> push(full)
            }
        }
    }
}

// LINT027's cap on one rule document, in lines.
let RULE_DOC_MAX_LINES = 300

// The .md basenames a `[docs] rule_docs_only = true` folder may carry: the folder's
// checklists (REVIEW.md and its routed companions), its architecture doc (and companion
// splits), and the rulings sidecar. Everything else is a ledger, a plan or a note.
def is_rule_doc_name(name : string) : bool {
    return (name == "REVIEW.md" || name == "ARCHITECTURE.md" || name == "LAWS.md"
        || name |> starts_with("REVIEW_") || name |> starts_with("ARCHITECTURE_"))
}

// A rule runs unless --disable names it, or --enable put the run in whitelist mode without it.
def rule_is_on(code : string; disabled_codes, enabled_codes : table<string>) : bool {
    return !key_exists(disabled_codes, code) && (empty(enabled_codes) || key_exists(enabled_codes, code))
}

// Directories under `path` the two tagged-or-present folder rules answer for: a folder whose
// OWN .lint_config sets rule_docs_only feeds LINT025, and any folder holding a rule document
// feeds LINT027's line gate. Folder property, no cascade - each directory answers for itself.
// Skip rules mirror scan_das_files so all passes see one tree.
def scan_docs_folders(path : string; want_rule_docs, want_doc_lines : bool;
                      var folders, line_folders : array<string>; var seen : table<string; void?>) {
    let st = stat(path)
    return if (!st.is_valid || !st.is_dir || seen |> key_exists(path))
    seen |> insert(path, null)
    if (want_rule_docs && rule_docs_only_at("{path}/.lint_config")) {
        folders |> push(path)
    }
    if (want_doc_lines && folder_holds_rule_doc(path)) {
        line_folders |> push(path)
    }
    fio::dir(path) $(name) {
        return if (name == "." || name == ".." || name |> starts_with("_") || is_skip_dir(name))
        let full = "{path}/{name}"
        let fst = stat(full)
        if (fst.is_valid && fst.is_dir && !is_build_tree(full)) {
            scan_docs_folders(full, want_rule_docs, want_doc_lines, folders, line_folders, seen)
        }
    }
}

// One walk of the run's scan roots for the two folder-keyed rules - the tree is walked once
// however many of them are on, and not at all when none is.
def collect_docs_folders(roots : array<string>; want_rule_docs, want_doc_lines : bool;
                         var folders, line_folders : array<string>&) {
    return if (!want_rule_docs && !want_doc_lines)
    var seen : table<string; void?>
    for (root in roots) {
        scan_docs_folders(root, want_rule_docs, want_doc_lines, folders, line_folders, seen)
    }
    folders |> sort
    line_folders |> sort
}

// A folder holds a rule document when any of its own (non-recursive) .md files is one
def folder_holds_rule_doc(path : string) : bool {
    var docs : array<string>
    collect_folder_mds(path, true, docs)
    return !empty(docs)
}

// The .md files in `folder` (non-recursive) on one side of `is_rule_doc_name`: the strays
// LINT025 reports, or the rule documents LINT027 sizes. LAWS.md is append-only ruling
// provenance, never read end to end, so it stays out of the rule-document side.
def collect_folder_mds(folder : string; rule_docs : bool; var out : array<string>) {
    fio::dir(folder) $(name) {
        return if (name == "." || name == ".." || !(name |> ends_with(".md"))
                   || is_rule_doc_name(name) != rule_docs
                   || (rule_docs && name == "LAWS.md"))
        let full = "{folder}/{name}"
        let fst = stat(full)
        if (fst.is_valid && fst.is_reg) {
            out |> push(full)
        }
    }
}

// Folder-docs pass: one sweep per invocation over the folders the walk found armed.
// A ledger or plan parked beside shipped code is read by nobody and goes stale unnoticed,
// so a tagged folder admits rule documents only. Returns the finding count.
def run_rule_docs_pass(folders : array<string>) : int {
    var found = 0
    for (folder in folders) {
        var strays : array<string>
        collect_folder_mds(folder, false, strays)
        strays |> sort
        for (md in strays) {
            print("{md}: LINT025: stray .md beside shipped code - rule documents only (REVIEW*.md, ARCHITECTURE*.md, LAWS.md); ledgers and plans live at the module root or plans/ (repo root)\n")
            found ++
        }
    }
    return found
}

// Newline count, plus the last line when the file does not end in one.
def count_text_lines(text : string) : int {
    return 0 if (empty(text))
    var lines = 0
    var from = 0
    while (true) {
        let at = find(text, "\n", from)
        break if (at < 0)
        lines ++
        from = at + 1
    }
    return from < length(text) ? lines + 1 : lines
}

// Folder-docs pass: a rule document nobody reads end to end has stopped being a contract, so a
// tagged folder's checklists and architecture docs are capped. The fix is always a companion
// split, never a trim - every criterion survives. Returns the finding count.
def run_doc_lines_pass(folders : array<string>) : int {
    var found = 0
    for (folder in folders) {
        var docs : array<string>
        collect_folder_mds(folder, true, docs)
        docs |> sort
        for (md in docs) {
            let lines = count_text_lines(fread(md))
            continue if (lines <= RULE_DOC_MAX_LINES)
            print("{md}: LINT027: rule document exceeds {RULE_DOC_MAX_LINES} lines ({lines}) - split it: ARCHITECTURE_<CONCERN>.md companions for architecture docs, routed REVIEW_<KIND>.md companions for checklists\n")
            found ++
        }
    }
    return found
}

// Forward direction: one finding per citation the shared scanner
// (`utils/common/arch_citations.das`) cannot resolve, in that scanner's words.
def private report_arch_citation(file : string; c : ArchCitation; var docs : ArchDocs) : int {
    let reason = citation_failure_reason(docs, file, c)
    return 0 if (empty(reason))
    print("{file}:{c.line}: LINT026: [arch] citation does not resolve - {reason} (\"{c.raw}\")\n")
    return 1
}

// Both halves of the citation check in one walk: the .md files that declare anchors and the
// .das files that may cite them. Same pruning as scan_das_files; `seen` folds nested roots.
def private scan_arch_folder(path : string; var mds, dases : array<string>; var seen : table<string; void?>) {
    return if (seen |> key_exists(path))
    seen |> insert(path, null)
    fio::dir(path) $(name) {
        return if (name == "." || name == ".." || name |> starts_with("_") || is_skip_dir(name))
        let full = "{path}/{name}"
        let fst = stat(full)
        return if (!fst.is_valid)
        if (fst.is_dir) {
            return if (is_build_tree(full))
            scan_arch_folder(full, mds, dases, seen)
        } elif (fst.is_reg && name |> ends_with(".md")) {
            mds |> push(full)
        } elif (fst.is_reg && name |> ends_with(".das") && !is_skip_file(name)) {
            dases |> push(full)
        }
    }
}

// [arch] citation pass, both directions off one read of each file: forward reports every
// citation that does not resolve, reverse every anchor no citation names - an anchor is a
// promise that some code answers for the section, so an uncited one is either code that forgot
// to say so or a section that was never anyone's contract. The roots are read as ONE set (a
// daslib citer satisfies a module's anchor), each .md parsed once through the shared index.
// A folder pass like LINT025 - once per invocation, not once per file.
def run_arch_pass(roots : array<string>) : int {
    var mds : array<string>
    var dases : array<string>
    var seen : table<string; void?>
    for (root in roots) {
        scan_arch_folder(root, mds, dases, seen)
    }
    mds |> sort
    dases |> sort
    var found = 0
    var docs : ArchDocs
    var cited : table<string>
    for (f in dases) {
        let text = fread(f)
        continue if (empty(text) || find(text, "arch") < 0)
        for (c in collect_arch_citations(f, text)) {
            found += report_arch_citation(f, c, docs)
            continue if (empty(c.anchor))
            cited |> insert("{c.doc}#{c.anchor}")
        }
    }
    for (md in mds) {
        let doc = arch_path(md)
        each_arch_anchor(docs, doc) $(a) {
            return if (cited |> key_exists("{doc}#{a.name}"))
            print("{md}:{a.line}: LINT026: anchor \{#{a.name}\} has no [arch] citation - cite it from the owning code, or strip the anchor to demote the section to narrative\n")
            found ++
        }
    }
    return found
}

// Validates a rule code: ^(LINT|PERF|STYLE)\d{3}$. Returns true if shape matches.
def is_valid_rule_code(code : string) : bool {
    let n = length(code)
    return false if (n < 7 || n > 8)
    var prefix_len = 0
    if (code |> starts_with("LINT") || code |> starts_with("PERF")) {
        prefix_len = 4
    } elif (code |> starts_with("STYLE")) {
        prefix_len = 5
    } else {
        return false
    }
    let digits = slice(code, prefix_len)
    return false if (length(digits) != 3)
    var all_digits = true
    peek_data(digits) $(d) {
        for (c in d) {
            if (!is_number(int(c))) {
                all_digits = false
            }
        }
    }
    return all_digits
}

// Comma-split, strip, uppercase, validate. On bad token returns the bad token
// (caller emits error + exits); on success returns "" and dest is populated.
def normalize_rule_list(raw : array<string>; var dest : table<string>) : string {
    for (entry in raw) {
        let parts <- entry |> split(",")
        for (p in parts) {
            let token = p |> strip |> to_upper
            continue if (empty(token))
            return token if (!is_valid_rule_code(token))
            dest |> insert(token)
        }
    }
    return ""
}

def set_skip_reason(file : string; lint_fixtures : bool; var result : LintResult) : bool {
    let content_skip = lint_file_skip_reason(file)
    if (!empty(content_skip)) {
        result.skip_reason = content_skip
        return true
    }
    // the lint tool's own rule fixtures violate rules BY DESIGN (warning-expectation
    // files carry no `expect` directive, so the content skip above misses them).
    // --lint-fixtures is how this tool's OWN tests ask to see those findings, so it
    // lifts both arms; normalize() emits the platform's preferred separator.
    if (!lint_fixtures && (normalize(file) |> replace("\\", "/") |> find("utils/lint/tests/") >= 0
            || is_lint_fixture_name(base_name(file)))) {
        result.skip_reason = "(lint rule fixture)"
        return true
    }
    return false
}

def lint_file(file : string; run_paranoid, run_perf, run_style, comment_hygiene, lint_fixtures, lint_excluded_paths : bool;
              disabled_codes, enabled_codes : table<string>; defer_stale : bool) : LintResult {
    var result = LintResult(file = file)
    if (set_skip_reason(file, lint_fixtures, result)) {
        return <- result
    }
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.ignore_shared_modules = true
            cop.export_all = true
            cop.lint_check = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(file, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    let issue_str = string(issues)
                    // Environment classes downgrade to SKIP only off the FIRST diagnostic's header
                    // line: a genuine environment failure aborts the compile before any other error,
                    // while a whole-string substring match would let one env line mask real errors
                    // (or match the literal echoed from an erroring source line).
                    var first_diag = ""
                    let err_start = find(issue_str, "error[")
                    if (err_start >= 0) {
                        // the first diagnostic's BLOCK, not just its header: the [extern] dlopen
                        // miss puts "can't load library" on the message's second line
                        first_diag = slice(issue_str, err_start, min(err_start + 240, length(issue_str)))
                    }
                    // error[20605] = missing prerequisite (e.g. daspkg-installed `anthropic/anthropic`) —
                    // out-of-tree as far as a stock daslang build can tell.
                    if (find(first_diag, "error[20605]") >= 0) {
                        result.skip_reason = "(missing prerequisite — install daspkg deps to lint)"
                        result.compile_errors = issue_str
                        return
                    }
                    // A [extern] dlopen miss (e.g. dasSMT without libz3) is the environment, not the
                    // code — skip with a visible reason so nightly coverage counting still sees it.
                    if (find(first_diag, "can't load library") >= 0) {
                        result.skip_reason = "(native library unavailable — install it to lint this file)"
                        result.compile_errors = issue_str
                        return
                    }
                    result.failed = true
                    result.compile_errors = issue_str
                    return
                }
                if (run_paranoid) {
                    var paranoid_issues : array<LintIssue>
                    result.count += paranoid_collect_issues(program, paranoid_issues, disabled_codes, enabled_codes)
                    result.issues |> push_from(paranoid_issues)
                }
                if (run_perf) {
                    var perf_issues : array<LintIssue>
                    result.count += perf_lint_collect_issues(program, perf_issues, disabled_codes, enabled_codes)
                    result.issues |> push_from(perf_issues)
                }
                if (run_style) {
                    var style_issues : array<LintIssue>
                    result.count += style_lint_collect_issues(program, style_issues, disabled_codes, enabled_codes, comment_hygiene)
                    result.issues |> push_from(style_issues)
                }
            }
        }
    }
    // LINT019 (stale nolint) needs every pass's consumption record - a partial run
    // under-consumes and would report live directives as dead
    if (!defer_stale && run_paranoid && run_perf && run_style && !result.failed && empty(result.skip_reason)) {
        let src = fread(file)
        if (!empty(src)) {
            var stale : array<LintIssue>
            result.count += stale_nolint_collect(file, src, disabled_codes, enabled_codes, stale)
            result.issues |> push_from(stale)
        }
    }
    if (!lint_excluded_paths) {
        apply_path_excludes(result)
    }
    return <- result
}

// .lint_config [paths] excludes filter findings at REPORT time only - the compile
// still ran (generic instantiation + nolint consumption stay whole-tree honest)
def apply_path_excludes(var result : LintResult) {
    return if (empty(result.issues))
    var kept : array<LintIssue>
    for (issue in result.issues) {
        if (is_lint_path_excluded(empty(issue.file) ? result.file : issue.file)) {
            result.excluded ++
        } else {
            kept |> push(issue)
        }
    }
    if (result.excluded > 0) {
        delete result.issues
        result.issues <- kept
        result.count = length(result.issues)
    }
}

// Print a single LintResult in the user-facing format. Shared by serial and
// parallel paths; per-file output matches, though serial mode appends its
// LINT019 findings as a trailing block while parallel reports them inline.
def print_result(result : LintResult; quiet, silent : bool) {
    if (!empty(result.skip_reason)) {
        if (!quiet) {
            print("SKIP {result.file} {result.skip_reason}\n")
        }
        return
    }
    if (result.failed) {
        print("FAIL {result.file}\n")
        if (!empty(result.compile_errors)) {
            print("  {result.compile_errors}\n")
        }
    } elif (result.count > 0) {
        print("WARN {result.file} ({result.count})\n")
        print(build_string() $(var w) {
            write_deduped(w, result.issues, "  ", !silent)
        })
    } elif (!quiet) {
        print("PASS {result.file}\n")
    }
}

// Read a newline-delimited paths file (blanks and `#` comments skipped).
def read_paths_from(path : string; var out : array<string>) : bool {
    let text = fread(path)
    return false if (empty(text))
    let lines <- text |> split("\n")
    for (line in lines) {
        let trimmed = line |> strip
        continue if (empty(trimmed) || trimmed |> starts_with("#"))
        out |> push(trimmed)
    }
    return true
}

// Each compile_file leaves AST/module residue only the GC can reclaim - on a
// whole-tree run that grows to gigabytes. Collect every N files (needs `options gc`).
let COLLECT_EVERY = 100

def maybe_collect(var since_collect : int&) {
    since_collect ++
    if (since_collect >= COLLECT_EVERY) {
        since_collect = 0
        // a JIT'd context cannot collect - the run stays grow-only there
        if (!jit_enabled()) {
            unsafe(heap_collect(true, false))
        }
    }
}

// Worker entry: lint every file in `paths`, emit one JSON-wrapped
// LintResult per file between `##lint##\n` markers. Caller (driver)
// parses the stream and aggregates.
def run_worker(paths : array<string>; run_paranoid, run_perf, run_style, comment_hygiene, lint_fixtures, lint_excluded_paths : bool;
               disabled_codes, enabled_codes : table<string>) {
    var since_collect = 0
    for (f in paths) {
        let result = lint_file(f, run_paranoid, run_perf, run_style, comment_hygiene, lint_fixtures, lint_excluded_paths, disabled_codes, enabled_codes, defer_stale = false)
        print("{JSON_PREFIX}{write_json(JV(result))}\n{JSON_PREFIX}")
        maybe_collect(since_collect)
    }
}

// Worker argv: spawn prefix + `--paths-from <chunk>` + the driver's pass/filter flags.
def build_worker_argv(spawn_prefix : array<string>; chunk_path : string; cfg : Config) : array<string> {
    var argv : array<string>
    argv |> reserve(length(spawn_prefix) + 14)
    argv |> push_from(spawn_prefix)
    argv |> push("--")
    argv |> push("--paths-from")
    argv |> push(chunk_path)
    if (cfg.quiet) {
        argv |> push("--quiet")
    }
    if (cfg.comment_hygiene) {
        argv |> push("--comment-hygiene")
    }
    if (cfg.lint_fixtures) {
        argv |> push("--lint-fixtures")
    }
    if (cfg.lint_excluded_paths) {
        argv |> push("--lint-excluded-paths")
    }
    if (cfg.paranoid_only) {
        argv |> push("--paranoid-only")
    }
    if (cfg.perf_only) {
        argv |> push("--perf-only")
    }
    if (cfg.style_only) {
        argv |> push("--style-only")
    }
    if (!empty(cfg.disable)) {
        argv |> push("--disable")
        argv |> push(cfg.disable |> join(","))
    }
    if (!empty(cfg.enable)) {
        argv |> push("--enable")
        argv |> push(cfg.enable |> join(","))
    }
    return <- argv
}

// Driver: chunk files, write per-chunk paths-from tempfiles, spawn N
// workers via parallel_workers::run_chunk_workers, parse JSON-marker
// blocks back into LintResult records. Returns the flat result list
// (caller sorts).
def run_parallel_lint(files : array<string>; cfg : Config) : array<LintResult> {
    var results : array<LintResult>
    let n = parallel_workers::compute_worker_count(length(files), cfg.workers)
    let chunks <- parallel_workers::chunk_files(files, n)
    let n_chunks = length(chunks)

    let spawn_prefix <- parallel_workers::spawn_argv_prefix()
    if (length(spawn_prefix) < 2) {
        // fail hard: returning empty results here would print a clean summary and exit 0 with nothing linted
        panic("--workers requires `daslang main.das` invocation ({length(spawn_prefix)} argv entries)")
    }
    let tag = "{ref_time_ticks()}"
    var tmperr : string
    let tmpdir = temp_directory(tmperr)
    if (!empty(tmperr)) {
        print("error: cannot resolve temp directory: {tmperr}\n")
        return <- results
    }

    var chunk_paths : array<string>
    chunk_paths |> reserve(n_chunks)
    var argvs : array<array<string>>
    argvs |> reserve(n_chunks)
    for (k in range(n_chunks)) {
        let cp = "{tmpdir}/lint-paths-{tag}-{k}.txt"
        // Push the path BEFORE attempting to write so the cleanup loop
        // sweeps it even on a partial-write or future fopen-semantic
        // change where fopen creates the file but `fw` becomes null
        // before our write block runs. Extra remove() of a non-existent
        // file is a no-op.
        chunk_paths |> push(cp)
        var write_ok = false
        fopen(cp, "wb") $(fw) {
            if (fw == null) return
            write_ok = true
            for (entry in chunks[k]) {
                fw |> fprint("{entry}\n")
            }
        }
        if (!write_ok) {
            for (p in chunk_paths) {
                remove(p)
            }
            print("error: could not write chunk file {cp}\n")
            return <- results
        }
        var argv <- build_worker_argv(spawn_prefix, cp, cfg)
        argvs |> emplace(argv)
    }

    if (!cfg.quiet) {
        print("Linting {length(files)} file(s) across {n_chunks} worker(s)...\n\n")
    }

    let chunk_results <- parallel_workers::run_chunk_workers(argvs)
    for (cr in chunk_results) {
        parse_worker_output(cr, results)
    }

    for (p in chunk_paths) {
        remove(p)
    }

    return <- results
}

// Parse one worker's stdout — alternating JSON_PREFIX-delimited blocks — into LintResult records.
def parse_worker_output(cr : parallel_workers::ChunkResult; var results : array<LintResult>) : void {
    if (cr.exit_code != 0) {
        print("error: worker {cr.index} exited with code {cr.exit_code}\n")
    }
    // Normalize CRLF → LF: on Windows the popen pipe text-mode reader
    // translates child `\n` to `\r\n`, which prevents `split(JSON_PREFIX)`
    // (which ends in `\n`) from finding the markers. Strip the trailing
    // `\r` from each split-on-LF segment, then re-join with LF.
    let normalized = cr.stdout |> split("\r\n") |> join("\n")
    let pieces <- normalized |> split(JSON_PREFIX)
    // Stream emits <PREFIX><json><PREFIX> per file. Splitting on the
    // marker yields odd-indexed pieces as JSON, even-indexed as
    // outside-marker text (compile_file chatter, blank). Indexing
    // by parity is robust to pre/post-marker stdout from the child.
    for (i in range(length(pieces))) {
        continue if (i % 2 == 0)
        let piece = pieces[i]
        continue if (empty(piece |> strip))
        var jerr : string
        let jv = piece |> read_json(jerr)
        if (jv == null) {
            print("error: worker {cr.index} emitted invalid JSON: {jerr}\n")
            continue
        }
        let lr <- from_JV(jv, type<LintResult>)
        results |> push_clone(lr)
    }
}

// CLI --disable/--enable → code tables, layered with repo policy (default-off rules + .lint_config).
// Prints the error and returns false on an invalid rule code.
def resolve_rule_filters(cfg : Config; var disabled_codes : table<string>&; var enabled_codes : table<string>&) : bool {
    let dis_bad = normalize_rule_list(cfg.disable, disabled_codes)
    if (!empty(dis_bad)) {
        print("error: invalid rule code \"{dis_bad}\" in --disable (expected LINT|PERF|STYLE + 3 digits)\n")
        return false
    }
    let en_bad = normalize_rule_list(cfg.enable, enabled_codes)
    if (!empty(en_bad)) {
        print("error: invalid rule code \"{en_bad}\" in --enable (expected LINT|PERF|STYLE + 3 digits)\n")
        return false
    }
    // Layer repo-wide policy on top of CLI flags: default-off rules + .lint_config.
    // Skipped under --enable (whitelist mode) so the user's explicit pick wins.
    if (empty(enabled_codes)) {
        seed_default_disabled(disabled_codes)
        load_lint_config(disabled_codes)
    }
    return true
}

// Driver-mode positionals → sorted .das file list, plus the folder-docs pass's scan roots
// (`dirs`: directory positionals, and each file positional's folder). Returns a process
// exit code, or -1 to proceed.
def collect_input_files(var files : array<string>&; var dirs : array<string>&) : int {
    let cli_args <- get_user_args()
    let paths <- [for (arg in cli_args); arg; where !starts_with(arg, "-")]
    if (empty(paths)) {
        print("Error: no files or directories specified\n\n")
        return print_lint_help(1)
    }
    var cache : table<string; void?>
    var skipped = 0
    for (p in paths) {
        // absolutize: module identity is spelling-sensitive — a relative root file registers
        // same-dir requires under a relative key while module-mount-routed requires register
        // absolute, tripping error 20605 "several modules with the same name" (e.g. dasPEG)
        let full = get_full_file_name(p)
        let target = empty(full) ? p : replace(full, "\\", "/")
        let st = stat(target)
        if (st.is_valid && st.is_dir) {
            dirs |> push(target)
        } elif (st.is_valid) {
            // a file positional arms its folder too - the PR lint shape passes changed
            // files one by one, and LINT025 must not go silent there
            dirs |> push(dir_name(target))
        }
        scan_das_files(target, files, cache, skipped)
    }
    if (empty(files) && empty(dirs)) {
        // All inputs were intentionally skipped (e.g. builtin.das) -- success.
        // Distinguishes a clean "nothing to lint" from a genuine "no .das at the
        // given path", which still errors. A directory root proceeds even with no
        // .das under it: the folder-docs pass still owes its verdict.
        return 0 if (skipped > 0)
        print("Error: no .das files found\n")
        return 1
    }
    // Sort so output is deterministic across runs (filesystem traversal
    // order varies by FS) and matches parallel mode byte-for-byte.
    files |> sort
    return -1
}

// Stale scan LAST, over the whole run's consumption ledger: a directive is consumed
// by ANY compile in the run, including files sorted after it. Worker mode still
// scans per-file - its ledger is per-process.
def stale_scan_at_end(stale_eligible : array<string>; cfg : Config; disabled_codes, enabled_codes : table<string>;
                      var total_issues : int&; var total_errors : int&; var total_skipped : int&; var total_excluded : int&) : void {
    // the collector no-ops per file when LINT019 is off - skip the whole fread loop then
    return if (key_exists(disabled_codes, "LINT019")
                    || (!empty(enabled_codes) && !key_exists(enabled_codes, "LINT019")))
    for (file in stale_eligible) {
        let src = fread(file)
        continue if (empty(src))
        var result = LintResult(file = file)
        var stale : array<LintIssue>
        result.count = stale_nolint_collect(file, src, disabled_codes, enabled_codes, stale)
        result.issues <- stale
        if (!cfg.lint_excluded_paths) {
            apply_path_excludes(result)
        }
        if (result.count > 0 || result.excluded > 0) {
            // count == 0 with excluded > 0 still tallies, but a print would emit a
            // second PASS line for a file the main pass already reported
            if (result.count > 0) {
                print_result(result, cfg.quiet, cfg.silent)
            }
            tally_result(result, total_issues, total_errors, total_skipped, total_excluded)
        }
    }
}

def tally_result(result : LintResult; var issues : int&; var errors : int&; var skipped : int&; var excluded : int&) : void {
    excluded += result.excluded
    if (!empty(result.skip_reason)) {
        skipped++
    } elif (result.failed) {
        errors++
    } elif (result.count > 0) {
        issues += result.count
    }
}

// Folder policy, not file policy: the three folder rules answer for the run's scan ROOTS rather
// than its file list, so they run once per invocation, in both driver modes. Returns the
// combined finding count.
def run_folder_passes(docs_roots : array<string>; disabled_codes, enabled_codes : table<string>) : int {
    let want_rule_docs = rule_is_on("LINT025", disabled_codes, enabled_codes)
    let want_arch = rule_is_on("LINT026", disabled_codes, enabled_codes)
    let want_doc_lines = rule_is_on("LINT027", disabled_codes, enabled_codes)
    var rule_docs_folders : array<string>
    var line_folders : array<string>
    collect_docs_folders(docs_roots, want_rule_docs, want_doc_lines, rule_docs_folders, line_folders)
    var found = 0
    if (want_rule_docs) {
        found += run_rule_docs_pass(rule_docs_folders)
    }
    if (want_arch) {
        found += run_arch_pass(docs_roots)
    }
    if (want_doc_lines) {
        found += run_doc_lines_pass(line_folders)
    }
    return found
}

// Prints the summary block; returns the process exit code (0 clean, 1 errors, 2 issues).
def print_summary(files_count, total_issues, total_errors, total_skipped, total_excluded : int) : int {
    print("\n--- Summary ---\n")
    let skip_part = total_skipped > 0 ? ", {total_skipped} skipped" : ""
    let excl_part = total_excluded > 0 ? ", {total_excluded} excluded by path policy" : ""
    print("{files_count} files, {total_issues} issue(s), {total_errors} error(s){skip_part}{excl_part}\n")
    if (total_errors > 0) {
        return 1
    } elif (total_issues > 0) {
        return 2
    } else {
        return 0
    }
}

// --help / bad-argv banner. Returns the process exit code so main stays a dispatcher.
def print_lint_help(rc : int) : int {
    if (rc == 0) {
        print("Unified lint checker for daslang files.\n")
        print("Runs paranoid, performance, and style lint passes with recursive directory scanning.\n\n")
    }
    print_help(get_command_info(type<Config>), "utils/lint/main.das")
    if (rc == 0) {
        print("\nPositional arguments: one or more files or directories to lint.\n")
    }
    return rc
}

[export]
def main() : int {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        return print_lint_help(1)
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        return print_lint_help(0)
    }
    let any_only = cfg.paranoid_only || cfg.perf_only || cfg.style_only
    let run_paranoid = !any_only || cfg.paranoid_only
    let run_perf = !any_only || cfg.perf_only
    let run_style = !any_only || cfg.style_only

    var disabled_codes : table<string>
    var enabled_codes : table<string>
    if (!resolve_rule_filters(cfg, disabled_codes, enabled_codes)) return 1

    // Worker mode: lint a pre-resolved list from a paths-from file and emit
    // JSON-wrapped results. No banner, no summary, no exit-code derivation.
    if (!empty(cfg.paths_from)) {
        var paths : array<string>
        if (!read_paths_from(cfg.paths_from, paths)) {
            print("error: cannot read paths-from file: {cfg.paths_from}\n")
            return 1
        }
        run_worker(paths, run_paranoid, run_perf, run_style, cfg.comment_hygiene, cfg.lint_fixtures, cfg.lint_excluded_paths, disabled_codes, enabled_codes)
        return 0
    }

    // Driver mode: scan positional paths, collect file list.
    var files : array<string>
    var docs_roots : array<string>
    let collect_rc = collect_input_files(files, docs_roots)
    if (collect_rc >= 0) return collect_rc

    var total_issues = 0
    var total_errors = 0
    var total_skipped = 0
    var total_excluded = 0

    if (parallel_workers::should_parallelize(length(files), cfg.workers)) {
        var results <- run_parallel_lint(files, cfg)
        // Sort by file path so output is deterministic across worker counts.
        sort(results, $(a, b : LintResult) => a.file < b.file)
        for (result in results) {
            print_result(result, cfg.quiet, cfg.silent)
            tally_result(result, total_issues, total_errors, total_skipped, total_excluded)
        }
    } else {
        if (!cfg.quiet) {
            print("Linting {length(files)} file(s)...\n\n")
        }
        var stale_eligible : array<string>
        var since_collect = 0
        for (file in files) {
            if (!cfg.quiet) {
                print("checking {file}...\n")
            }
            let result = lint_file(file, run_paranoid, run_perf, run_style, cfg.comment_hygiene, cfg.lint_fixtures, cfg.lint_excluded_paths, disabled_codes, enabled_codes, defer_stale = true)
            if (!result.failed && empty(result.skip_reason)) {
                stale_eligible |> push(file)
            }
            print_result(result, cfg.quiet, cfg.silent)
            tally_result(result, total_issues, total_errors, total_skipped, total_excluded)
            maybe_collect(since_collect)
        }
        if (run_paranoid && run_perf && run_style) {
            stale_scan_at_end(stale_eligible, cfg, disabled_codes, enabled_codes,
                total_issues, total_errors, total_skipped, total_excluded)
        }
    }

    total_issues += run_folder_passes(docs_roots, disabled_codes, enabled_codes)

    return print_summary(length(files), total_issues, total_errors, total_skipped, total_excluded)
}
