options gen2
options persistent_heap

require daslib/clargs
require daslib/ast
require daslib/ast_boost
require daslib/fio
require daslib/strings_boost
require math
require strings
require canonical
require minhash
require cluster
require report


[CommandLineArgs]
struct Config {
    @clarg_short = "p"
    @clarg_doc = "Path(s) to scan (file or directory). Repeatable."
    path : array<string>

    @clarg_short = "t"
    @clarg_doc = "Fuzzy similarity threshold 0..1 (default 0.7)"
    threshold : float = 0.7f

    @clarg_short = "n"
    @clarg_doc = "Top N entries shown in stdout summary (default 20)"
    top : int = 20

    @clarg_doc = "Path to JSON report (optional)"
    json : string

    @clarg_short = "x"
    @clarg_doc = "Skip fuzzy MinHash pass"
    no_fuzzy : bool

    @clarg_doc = "Min token count per function for inclusion (default 8)"
    min_tokens : int = 8

    @clarg_short = "v"
    @clarg_doc = "Per-file progress to stdout"
    verbose : bool

    @clarg_short = "L"
    @clarg_doc = "Lambda-only mode: skip top-level functions, keep only lambdas (e.g. dastest run() bodies)"
    lambdas_only : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}


def is_daslib_skip(path : string; basename : string) : bool {
    // `daslib/debugger.das` and `daslib/profiler.das` install thread-local
    // agents at compile time; the second install in the same process throws
    // and aborts the scan. Match by daslib/ path suffix, not bare basename
    // — a project's own `myapp/debugger.das` should not be silently skipped.
    let normalized = path |> replace("\\", "/")
    let prefix = "daslib/{basename}"
    return normalized == prefix || normalized |> ends_with("/{prefix}")
}


def is_skip_file(path : string; basename : string) : bool {
    if (basename == "builtin.das") {
        return true
    }
    return (is_daslib_skip(path, "debugger.das")
        || is_daslib_skip(path, "profiler.das"))
}


def is_skip_dir(name : string) : bool {
    return name |> starts_with(".")
}


def has_expect_directive(file : string) : bool {
    let text = fread(file)
    if (empty(text)) {
        return false
    }
    let lines <- text |> split("\n")
    for (line in lines) {
        let trimmed = line |> strip
        if (trimmed |> starts_with("expect ")) {
            return true
        }
    }
    return false
}


def scan_das_files(path : string; var files : array<string>; var cache : table<string; void?>) {
    let st = stat(path)
    if (!st.is_valid) {
        return
    }
    if (st.is_reg) {
        if (path |> ends_with(".das") && !cache |> key_exists(path)) {
            let last_slash = max(rfind(path, "/"), rfind(path, "\\"))
            let fname = last_slash >= 0 ? slice(path, last_slash + 1) : path
            if (!is_skip_file(path, fname)) {
                cache |> insert(path, null)
                files |> push(path)
            }
        }
        return
    }
    if (!st.is_dir) {
        return
    }
    fio::dir(path) $(name) {
        if (name == "." || name == "..") {
            return
        }
        if (name |> starts_with("_") || is_skip_dir(name)) {
            return
        }
        let full = "{path}/{name}"
        let fst = stat(full)
        if (!fst.is_valid) {
            return
        }
        if (fst.is_dir) {
            scan_das_files(full, files, cache)
        } elif (fst.is_reg && name |> ends_with(".das") && !is_skip_file(full, name) && !cache |> key_exists(full)) {
            cache |> insert(full, null)
            files |> push(full)
        }
    }
}


// Walk one compiled program and append a FuncRecord per user
// function (lambdas included; only builtIn / generated skipped).
//
// Generic-template functions (e.g. dastest's `equal<T>`) get reified
// into every caller's module, so without filtering we see hundreds of
// copies of the same library helpers. The file-source filter drops
// any function whose `at.fileInfo` points outside the file we just
// compiled, leaving only what was actually written in that file.
def collect_from_program(program : ProgramPtr; source_file : string; var records : array<FuncRecord>; var seen_loc : table<string; void?>; lambdas_only : bool; want_sigs : bool; min_tokens : int) {
    let thisMod = get_this_module(program)
    for_each_function(thisMod, "") $(func) {
        if (func.flags.builtIn) {
            return
        }
        // Lambdas are emitted as `generated` synthesized functions
        // (`__lambda_function_at_line_xxx`). Default mode skips them via
        // the generated filter (the dispatcher already references them
        // via ADDR). Lambda-only mode lets them through and drops the
        // top-level functions instead.
        if (lambdas_only) {
            if (!func.flags._lambda) {
                return
            }
        } else {
            if (func.flags.generated) {
                return
            }
        }
        let fn_file = (func.at.fileInfo != null) ? string(func.at.fileInfo.name) : ""
        if (fn_file != source_file) {
            return
        }
        // Generic templates get reified per type (e.g. `test_conversions`
        // reified for int8, uint16, uint, uint64). Each reification is a
        // distinct FunctionPtr but they all point at the same source
        // (file, line). Keep the first; the rest are reported as
        // duplicates against themselves which is just noise.
        let loc = "{fn_file}:{int(func.at.line)}"
        if (key_exists(seen_loc, loc)) {
            return
        }
        seen_loc |> insert(loc, null)
        let canon = canonicalize_function(func)
        if (empty(canon)) {
            return
        }
        let tokens <- tokenize_canonical(canon)
        var rec = FuncRecord()
        rec.ref.file = fn_file
        rec.ref.name = string(func.name)
        rec.ref.line = int(func.at.line)
        rec.ref.is_lambda = func.flags._lambda
        rec.canonical := canon
        rec.token_count = length(tokens)
        // Skip signature work when fuzzy is disabled or when the function
        // is below the noise floor — fuzzy_pairs filters those out anyway.
        if (want_sigs && rec.token_count >= min_tokens) {
            rec.sig <- minhash_signature(tokens)
        }
        records |> emplace(rec)
    }
}


def compile_and_collect(file : string; var records : array<FuncRecord>; var seen_loc : table<string; void?>; verbose : bool; lambdas_only : bool; want_sigs : bool; min_tokens : int) : bool {
    var inscope access <- make_file_access("")
    var ok_compile = true
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.ignore_shared_modules = true
            cop.export_all = true
            // Leave optimisations and infer-time folding ON: dastest macros
            // (e.g. unroll) need constant folding to compile, and folded-equal
            // expressions are exactly the kind of duplicate we want to catch.
            compile_file(file, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    ok_compile = false
                    if (verbose) {
                        print("FAIL {file}\n  {issues}\n")
                    } else {
                        print("FAIL {file}\n")
                    }
                    return
                }
                collect_from_program(program, file, records, seen_loc, lambdas_only, want_sigs, min_tokens)
            }
        }
    }
    return ok_compile
}


[export]
def main() {
    var cfg = Config()
    let err = parse_args(cfg)
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "find_dupes")
        return
    }
    if (err != "") {
        print("error: {err}\n\n")
        print_help(get_command_info(type<Config>), "find_dupes")
        return
    }
    if (length(cfg.path) == 0) {
        print("error: at least one --path is required\n\n")
        print_help(get_command_info(type<Config>), "find_dupes")
        return
    }
    // Defaults live in the Config struct field initializers — clargs
    // respects them when the flag is omitted, so 0 is now a legitimate
    // user-supplied value (e.g. --min-tokens 0 to include all functions).
    let threshold = cfg.threshold
    let top = cfg.top
    let min_tokens = cfg.min_tokens
    let want_sigs = !cfg.no_fuzzy

    var files : array<string>
    var cache : table<string; void?>
    for (p in cfg.path) {
        scan_das_files(p, files, cache)
    }
    if (length(files) == 0) {
        print("error: no .das files found under given paths\n")
        return
    }
    print("scanning {length(files)} file(s)...\n")

    var records : array<FuncRecord>
    var seen_loc : table<string; void?>
    var n_failed = 0
    var n_skipped = 0
    for (i in range(length(files))) {
        let file = files[i]
        if (cfg.verbose) {
            print("[{i + 1}/{length(files)}] {file}\n")
        }
        if (has_expect_directive(file)) {
            n_skipped++
            continue
        }
        if (!compile_and_collect(file, records, seen_loc, cfg.verbose, cfg.lambdas_only, want_sigs, min_tokens)) {
            n_failed++
        }
    }
    print("collected {length(records)} function record(s); {n_failed} compile failure(s), {n_skipped} skipped (expect-directive)\n")

    print("clustering exact matches...\n")
    var clusters <- exact_buckets(records, min_tokens)
    sort_clusters(clusters)
    var pairs : array<FuzzyPair>
    if (!cfg.no_fuzzy) {
        print("scanning {length(records)} records for fuzzy near-duplicates (threshold {threshold})...\n")
        pairs <- fuzzy_pairs(records, threshold, min_tokens)
        sort_pairs(pairs)
    }

    var rep : Report
    rep.summary.files_scanned = length(files)
    rep.summary.functions_analyzed = length(records)
    rep.summary.threshold = threshold
    rep.summary.min_tokens = min_tokens
    rep.exact_clusters <- clusters
    rep.fuzzy_pairs <- pairs
    rep.summary.exact_clusters = length(rep.exact_clusters)
    rep.summary.fuzzy_pairs = length(rep.fuzzy_pairs)

    print_summary(rep, top)
    if (!empty(cfg.json)) {
        if (write_json_report(rep, cfg.json)) {
            print("\nJSON report written to {cfg.json}\n")
        } else {
            print("\nERROR: could not write JSON report to {cfg.json}\n")
        }
    }
}
