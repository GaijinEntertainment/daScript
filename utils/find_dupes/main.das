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
require exchange
require pipeline


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

    @clarg_doc = "Write all extracted functions to JSON and exit before clustering"
    export_functions : string

    @clarg_doc = "Load functions from a JSON file (produced by --export-functions) instead of compiling"
    import_functions : string

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
    let normalized = normalize_path(path)
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
    let importing = !empty(cfg.import_functions)
    let exporting = !empty(cfg.export_functions)
    if (importing && length(cfg.path) > 0) {
        print("error: --import-functions and --path are mutually exclusive\n")
        return
    }
    if (importing && exporting) {
        print("error: --import-functions and --export-functions are mutually exclusive (transcode is a no-op)\n")
        return
    }
    if (!importing && length(cfg.path) == 0) {
        print("error: at least one --path is required (or pass --import-functions)\n\n")
        print_help(get_command_info(type<Config>), "find_dupes")
        return
    }
    if (cfg.threshold < 0.0f || cfg.threshold > 1.0f) {
        print("error: --threshold must be in 0..1, got {cfg.threshold}\n")
        return
    }
    if (cfg.top < 0) {
        print("error: --top must be >= 0, got {cfg.top}\n")
        return
    }
    if (cfg.min_tokens < 0) {
        print("error: --min-tokens must be >= 0, got {cfg.min_tokens}\n")
        return
    }
    // Defaults live in the Config struct field initializers — clargs
    // respects them when the flag is omitted, so 0 is now a legitimate
    // user-supplied value (e.g. --min-tokens 0 to include all functions).
    let threshold = cfg.threshold
    let top = cfg.top
    let min_tokens = cfg.min_tokens
    // Skip MinHash signing in export-only mode: signatures aren't
    // written to the export and clustering is skipped, so the work
    // would be discarded. Saves O(tokens × 64) per function on big
    // shard runs.
    let want_sigs = !cfg.no_fuzzy && !exporting

    var records : array<FuncRecord>
    var files_scanned = 0

    if (importing) {
        var ierr : string
        if (!read_import(cfg.import_functions, records, want_sigs, min_tokens, ierr)) {
            print("error: {ierr}\n")
            return
        }
        print("loaded {length(records)} function record(s) from {cfg.import_functions}\n")
    } else {
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
        files_scanned = length(files)
        print("collected {length(records)} function record(s); {n_failed} compile failure(s), {n_skipped} skipped (expect-directive)\n")

        if (exporting) {
            // In export mode, treat compile failures as fatal —
            // sharding scripts that merge multiple exports must be able
            // to detect partial-failure runs, same reason write
            // failures panic below. Best-effort export would silently
            // ship an incomplete corpus.
            if (n_failed > 0) {
                print("error: {n_failed} compile failure(s); refusing to write partial export\n")
                panic("find_dupes export failed: {n_failed} compile failure(s)")
            }
            if (write_export(records, cfg.export_functions)) {
                print("wrote {length(records)} function(s) to {cfg.export_functions}\n")
                return
            }
            // Use `panic` to surface a non-zero exit code so scripts
            // sharding compile-then-merge across machines can detect
            // partial-failure runs. The `error:` prefix matches the
            // format used elsewhere for usage errors in this CLI.
            print("error: could not write export to {cfg.export_functions}\n")
            panic("find_dupes export failed")
        }
    }

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
    rep.summary.files_scanned = files_scanned
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
