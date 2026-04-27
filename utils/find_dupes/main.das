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

    @clarg_doc = "Path to baseline JSON (an --export-functions output) — B1 mode: tag records whose canonical is not in the baseline as candidates"
    baseline : string

    @clarg_doc = "Strict baseline: also drop clusters whose canonical exists in the baseline (only fully-new clusters survive)"
    baseline_strict : bool

    @clarg_doc = "B2 mode candidate path(s) — file or directory; functions extracted from these are tagged as candidates and the report is filtered to those involving them. Repeatable."
    against : array<string>

    @clarg_doc = "Read newline-delimited candidate paths from stdin (B2 mode, complements --against)"
    against_from_stdin : bool

    @clarg_doc = "Exit with non-zero code when the post-filter report contains any clusters or pairs (CI gate)"
    check : bool

    @clarg_doc = "Force the flat clusters/pairs writer even when --against is active (default in --against mode is the per-candidate writer)"
    flat : bool

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


// Read newline-delimited paths from stdin. Used by --against-from-stdin
// to plug into shell pipelines: `git diff --name-only master |
// daslang utils/find_dupes/main.das -- --against-from-stdin
// --import-functions corpus.json`. Skips blank lines.
def read_stdin_paths(var paths : array<string>) {
    let f = fstdin()
    if (f == null) {
        return
    }
    while (!feof(f)) {
        let line = fgets(f) |> strip
        if (!empty(line)) {
            paths |> push(line)
        }
    }
}


// Expand each `--against` argument (file or directory) into a normalized
// set of `.das` file paths. Reuses scan_das_files so the same skip
// rules (`builtin.das`, debugger/profiler) apply.
def resolve_against_files(against : array<string>; from_stdin : bool;
                          var out_set : table<string; void?>) {
    var raw : array<string>
    for (p in against) {
        raw |> push(p)
    }
    if (from_stdin) {
        read_stdin_paths(raw)
    }
    var cache : table<string; void?>
    var files : array<string>
    for (p in raw) {
        scan_das_files(p, files, cache)
    }
    for (f in files) {
        let n = normalize_path(f)
        if (!key_exists(out_set, n)) {
            out_set |> insert(n, null)
        }
    }
}


// Mark in-place: any record whose file is in `against_files` becomes a
// candidate. Idempotent; safe to call after compile or after import.
def mark_records_as_candidates(var records : array<FuncRecord>;
                               against_files : table<string; void?>) {
    for (r in records) {
        if (key_exists(against_files, normalize_path(r.ref.file))) {
            r.is_candidate = true
        }
    }
}


// Walk records and tag any whose canonical is NOT in the baseline as a
// candidate. Additive — does not clear existing candidate flags from
// --against.
def tag_baseline_new_canonicals(var records : array<FuncRecord>;
                                baseline_canonicals : table<string; void?>) {
    for (r in records) {
        if (r.is_candidate) {
            continue
        }
        if (!key_exists(baseline_canonicals, r.canonical)) {
            r.is_candidate = true
        }
    }
}


// In strict baseline mode, drop clusters whose canonical existed in the
// baseline (we only want fully-new clusters, not growth on existing
// ones). Pairs are unaffected: they have no per-pair baseline equivalent.
def strict_drop_baseline_clusters(var clusters : array<Cluster>;
                                  baseline_canonicals : table<string; void?>) {
    var kept : array<Cluster>
    kept |> reserve(length(clusters))
    for (c in clusters) {
        if (key_exists(baseline_canonicals, c.canonical_sample)) {
            continue
        }
        kept |> push_clone(c)
    }
    clusters <- kept
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
    let has_against = length(cfg.against) > 0 || cfg.against_from_stdin
    let has_baseline = !empty(cfg.baseline)
    if (importing && length(cfg.path) > 0) {
        print("error: --import-functions and --path are mutually exclusive\n")
        return
    }
    if (importing && exporting) {
        print("error: --import-functions and --export-functions are mutually exclusive (transcode is a no-op)\n")
        return
    }
    if (exporting && (has_against || has_baseline)) {
        print("error: --export-functions is incompatible with --against / --baseline (export captures the raw corpus, not a filtered view)\n")
        return
    }
    if (cfg.baseline_strict && !has_baseline) {
        print("error: --baseline-strict requires --baseline\n")
        return
    }
    if (!importing && !has_against && length(cfg.path) == 0) {
        print("error: at least one --path is required (or pass --import-functions, or --against)\n\n")
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

    // Resolve --against paths first (B2 mode) so we can drop overridden
    // corpus records and tag candidates as we go.
    var against_files : table<string; void?>
    if (has_against) {
        resolve_against_files(cfg.against, cfg.against_from_stdin, against_files)
        if (length(against_files) == 0) {
            print("error: --against (or stdin) yielded no .das files\n")
            return
        }
    }

    var records : array<FuncRecord>
    var files_scanned = 0
    var seen_loc : table<string; void?>

    if (importing) {
        var ierr : string
        if (!read_import(cfg.import_functions, records, want_sigs, min_tokens, ierr)) {
            print("error: {ierr}\n")
            return
        }
        print("loaded {length(records)} function record(s) from {cfg.import_functions}\n")
        if (has_against) {
            let dropped = drop_records_under_files(records, against_files)
            if (dropped > 0) {
                print("dropped {dropped} corpus record(s) overridden by --against paths\n")
            }
        }
    } else {
        var files : array<string>
        var cache : table<string; void?>
        for (p in cfg.path) {
            scan_das_files(p, files, cache)
        }
        if (length(files) == 0 && !has_against) {
            print("error: no .das files found under given paths\n")
            return
        }
        if (length(files) > 0) {
            print("scanning {length(files)} file(s)...\n")
        }

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

    // B2 wiring: tag any already-extracted records whose file is in the
    // candidate set (covers `-p` overlap), then compile any candidate
    // files not yet seen and tag those too.
    if (has_against) {
        mark_records_as_candidates(records, against_files)
        var n_against_failed = 0
        var n_against_skipped = 0
        var n_against_compiled = 0
        for (af in keys(against_files)) {
            // Skip files already extracted (either via `-p` or, post-drop,
            // we already removed any that came from the corpus). Re-scan
            // current records to detect coverage.
            var already_have = false
            for (r in records) {
                if (normalize_path(r.ref.file) == af) {
                    already_have = true
                    break
                }
            }
            if (already_have) {
                continue
            }
            if (has_expect_directive(af)) {
                n_against_skipped++
                continue
            }
            let before = length(records)
            if (!compile_and_collect(af, records, seen_loc, cfg.verbose, cfg.lambdas_only, want_sigs, min_tokens)) {
                n_against_failed++
                continue
            }
            // Tag the freshly-pushed records as candidates.
            for (i in range(before, length(records))) {
                records[i].is_candidate = true
            }
            n_against_compiled++
        }
        if (n_against_compiled > 0 || n_against_failed > 0 || n_against_skipped > 0) {
            print("--against: compiled {n_against_compiled}, failed {n_against_failed}, skipped {n_against_skipped}\n")
        }
    }

    // B1 wiring: tag records whose canonical isn't in the baseline.
    var baseline_canonicals : table<string; void?>
    if (has_baseline) {
        var berr : string
        if (!load_baseline_canonicals(cfg.baseline, baseline_canonicals, berr)) {
            print("error: {berr}\n")
            return
        }
        print("loaded {length(baseline_canonicals)} baseline canonical(s) from {cfg.baseline}\n")
        tag_baseline_new_canonicals(records, baseline_canonicals)
    }

    let filter_candidates = has_against || has_baseline
    print("clustering exact matches...\n")
    var clusters <- exact_buckets(records, min_tokens, filter_candidates)
    if (cfg.baseline_strict) {
        let before_strict = length(clusters)
        strict_drop_baseline_clusters(clusters, baseline_canonicals)
        let dropped_strict = before_strict - length(clusters)
        if (dropped_strict > 0) {
            print("--baseline-strict: dropped {dropped_strict} cluster(s) whose canonical was already in baseline\n")
        }
    }
    sort_clusters(clusters)
    var pairs : array<FuzzyPair>
    if (!cfg.no_fuzzy) {
        print("scanning {length(records)} records for fuzzy near-duplicates (threshold {threshold})...\n")
        pairs <- fuzzy_pairs(records, threshold, min_tokens, filter_candidates)
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

    let use_per_candidate = has_against && !cfg.flat
    if (use_per_candidate) {
        let cand_refs <- collect_candidate_refs(records)
        // Default to a tighter top in per-candidate mode (matches what the
        // MCP tool exposes); user can still override with --top.
        let per_top = cfg.top == 20 ? 5 : cfg.top
        print_per_candidate_summary(rep, cand_refs, per_top)
    } else {
        print_summary(rep, top)
    }
    if (!empty(cfg.json)) {
        var json_ok = false
        if (use_per_candidate) {
            let cand_refs <- collect_candidate_refs(records)
            let per_top = cfg.top == 20 ? 5 : cfg.top
            let body = write_per_candidate_report(rep, cand_refs, per_top)
            fopen(cfg.json, "wb") $(fw) {
                if (fw == null) {
                    return
                }
                json_ok = true
                fw |> fprint(body)
            }
        } else {
            json_ok = write_json_report(rep, cfg.json)
        }
        if (json_ok) {
            print("\nJSON report written to {cfg.json}\n")
        } else {
            print("\nERROR: could not write JSON report to {cfg.json}\n")
        }
    }

    if (cfg.check && (length(rep.exact_clusters) > 0 || length(rep.fuzzy_pairs) > 0)) {
        // --check turns the post-filter report into a CI gate. Use
        // `panic` to surface a non-zero exit code (same pattern as the
        // export-failure path above).
        panic("find_dupes --check failed: {length(rep.exact_clusters)} cluster(s), {length(rep.fuzzy_pairs)} pair(s)")
    }
}
