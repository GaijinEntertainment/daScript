options gen2

require daslib/lint
require daslib/perf_lint
require daslib/style_lint
require daslib/ast_boost
require daslib/fio
require daslib/clargs
require daslib/json
require daslib/json_boost
require math
require strings
require daslib/strings_boost
require ../common/parallel_workers.das

let JSON_PREFIX = "##lint##\n"

struct LintResult {
    file : string
    count : int
    errors : array<string>
    failed : bool
    compile_errors : string
    skip_reason : string
}

[CommandLineArgs]
struct Config {
    @clarg_short = "q"
    @clarg_doc = "Suppress PASS lines and progress messages"
    quiet : bool

    @clarg_doc = "Enable STYLE014/STYLE015 comment-length checks"
    comment_hygiene : bool

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

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def write_deduped(var writer : StringBuilderWriter; messages : array<string>; prefix : string = "  ") {
    var counts : table<string; int>
    var order : array<string>
    for (msg in messages) {
        if (key_exists(counts, msg)) {
            counts[msg]++
        } else {
            counts[msg] = 1
            order |> push(msg)
        }
    }
    for (msg in order) {
        let n = counts[msg]
        if (n > 1) {
            writer |> write("{prefix}{msg} (x{n})\n")
        } else {
            writer |> write("{prefix}{msg}\n")
        }
    }
}

def is_skip_file(name : string) : bool {
    return name == "builtin.das"
}

def is_skip_dir(name : string) : bool {
    return name |> starts_with(".")
}

def has_expect_directive(file : string) : bool {
    // Files with `expect <code>:<count>` declare intentional compile errors
    // for dastest. Lint cannot evaluate them — the program never reaches
    // the AST stage paranoid/perf/style passes need. Skip them.
    let text = fread(file)
    return false if (empty(text))
    let lines <- text |> split("\n")
    for (line in lines) {
        let trimmed = line |> strip
        return true if (trimmed |> starts_with("expect "))
    }
    return false
}

def scan_das_files(path : string; var files : array<string>; var cache : table<string; void?>) {
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
            scan_das_files(full, files, cache)
        } elif (fst.is_reg && name |> ends_with(".das") && !is_skip_file(name) && !cache |> key_exists(full)) {
            cache |> insert(full, null)
            files |> push(full)
        }
    }
}

def lint_file(file : string; run_paranoid, run_perf, run_style, comment_hygiene : bool) : LintResult {
    var result = LintResult(file = file)
    if (has_expect_directive(file)) {
        result.skip_reason = "(intentional compile errors via `expect`)"
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
                    // Files that can't resolve a require (e.g. daspkg-installed `anthropic/anthropic`)
                    // are out-of-tree as far as a stock daslang build can tell — treat as skip, not fail.
                    if (find(issue_str, "missing prerequisite") >= 0) {
                        result.skip_reason = "(missing prerequisite — install daspkg deps to lint)"
                        result.compile_errors = issue_str
                        return
                    }
                    result.failed = true
                    result.compile_errors = issue_str
                    return
                }
                if (run_paranoid) {
                    var paranoid_issues : array<string>
                    result.count += paranoid_collect(program, paranoid_issues)
                    for (w in paranoid_issues) {
                        result.errors |> push(w)
                    }
                }
                if (run_perf) {
                    var perf_issues : array<string>
                    result.count += perf_lint_collect(program, perf_issues)
                    for (w in perf_issues) {
                        result.errors |> push(w)
                    }
                }
                if (run_style) {
                    var style_issues : array<string>
                    result.count += style_lint_collect(program, style_issues, comment_hygiene)
                    for (w in style_issues) {
                        result.errors |> push(w)
                    }
                }
            }
        }
    }
    return <- result
}

// Print a single LintResult in the user-facing format. Shared by serial
// and parallel paths so output is byte-identical regardless of mode.
def print_result(result : LintResult; quiet : bool) {
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
            write_deduped(w, result.errors)
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

// Worker entry: lint every file in `paths`, emit one JSON-wrapped
// LintResult per file between `##lint##\n` markers. Caller (driver)
// parses the stream and aggregates.
def run_worker(paths : array<string>; run_paranoid, run_perf, run_style, comment_hygiene : bool) {
    for (f in paths) {
        let result = lint_file(f, run_paranoid, run_perf, run_style, comment_hygiene)
        print("{JSON_PREFIX}{write_json(JV(result))}\n{JSON_PREFIX}")
    }
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

    let raw_args <- get_command_line_arguments()
    if (length(raw_args) < 2) {
        print("error: --workers requires `daslang main.das` invocation (raw_args has {length(raw_args)} entries)\n")
        return <- results
    }
    let exe = raw_args[0]
    let script = raw_args[1]
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
        var argv : array<string>
        argv |> reserve(12)
        argv |> push(exe)
        argv |> push(script)
        argv |> push("--")
        argv |> push("--paths-from")
        argv |> push(cp)
        if (cfg.quiet) {
            argv |> push("--quiet")
        }
        if (cfg.comment_hygiene) {
            argv |> push("--comment-hygiene")
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
        argvs |> emplace(argv)
    }

    if (!cfg.quiet) {
        print("Linting {length(files)} file(s) across {n_chunks} worker(s)...\n\n")
    }

    let chunk_results <- parallel_workers::run_chunk_workers(argvs)

    // Parse each worker's stdout: alternating JSON_PREFIX-delimited blocks.
    for (cr in chunk_results) {
        if (cr.exit_code != 0) {
            print("error: worker {cr.index} exited with code {cr.exit_code}\n")
        }
        let pieces <- cr.stdout |> split(JSON_PREFIX)
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

    for (p in chunk_paths) {
        remove(p)
    }

    return <- results
}

[export]
def main() : int {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Config>), "utils/lint/main.das")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print("Unified lint checker for daslang files.\n")
        print("Runs paranoid, performance, and style lint passes with recursive directory scanning.\n\n")
        print_help(get_command_info(type<Config>), "utils/lint/main.das")
        print("\nPositional arguments: one or more files or directories to lint.\n")
        return 0
    }
    let any_only = cfg.paranoid_only || cfg.perf_only || cfg.style_only
    let run_paranoid = !any_only || cfg.paranoid_only
    let run_perf = !any_only || cfg.perf_only
    let run_style = !any_only || cfg.style_only

    // Worker mode: lint a pre-resolved list from a paths-from file and emit
    // JSON-wrapped results. No banner, no summary, no exit-code derivation.
    if (!empty(cfg.paths_from)) {
        var paths : array<string>
        if (!read_paths_from(cfg.paths_from, paths)) {
            print("error: cannot read paths-from file: {cfg.paths_from}\n")
            return 1
        }
        run_worker(paths, run_paranoid, run_perf, run_style, cfg.comment_hygiene)
        return 0
    }

    // Driver mode: scan positional paths, collect file list.
    let cli_args <- get_user_args()
    var paths : array<string>
    for (arg in cli_args) {
        if (!starts_with(arg, "-")) {
            paths |> push(arg)
        }
    }
    if (empty(paths)) {
        print("Error: no files or directories specified\n\n")
        print_help(get_command_info(type<Config>), "utils/lint/main.das")
        return 1
    }
    var files : array<string>
    var cache : table<string; void?>
    for (p in paths) {
        scan_das_files(p, files, cache)
    }
    if (empty(files)) {
        print("Error: no .das files found\n")
        return 1
    }
    // Sort so output is deterministic across runs (filesystem traversal
    // order varies by FS) and matches parallel mode byte-for-byte.
    files |> sort

    var total_issues = 0
    var total_errors = 0
    var total_skipped = 0

    if (parallel_workers::should_parallelize(length(files), cfg.workers)) {
        var results <- run_parallel_lint(files, cfg)
        // Sort by file path so output is deterministic across worker counts.
        sort(results, $(a, b : LintResult) => a.file < b.file)
        for (result in results) {
            print_result(result, cfg.quiet)
            if (!empty(result.skip_reason)) {
                total_skipped++
            } elif (result.failed) {
                total_errors++
            } elif (result.count > 0) {
                total_issues += result.count
            }
        }
    } else {
        if (!cfg.quiet) {
            print("Linting {length(files)} file(s)...\n\n")
        }
        for (file in files) {
            if (!cfg.quiet) {
                print("checking {file}...\n")
            }
            let result = lint_file(file, run_paranoid, run_perf, run_style, cfg.comment_hygiene)
            print_result(result, cfg.quiet)
            if (!empty(result.skip_reason)) {
                total_skipped++
            } elif (result.failed) {
                total_errors++
            } elif (result.count > 0) {
                total_issues += result.count
            }
        }
    }

    print("\n--- Summary ---\n")
    if (total_skipped > 0) {
        print("{length(files)} files, {total_issues} issue(s), {total_errors} error(s), {total_skipped} skipped\n")
    } else {
        print("{length(files)} files, {total_issues} issue(s), {total_errors} error(s)\n")
    }
    if (total_errors > 0) {
        return 1
    } elif (total_issues > 0) {
        return 2
    } else {
        return 0
    }
}
