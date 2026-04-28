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

    @clarg_doc = "Path to baseline JSON (an --export-functions output) — B1 mode: tag records whose member identity (file:line:name) is not in the baseline as candidates"
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

    @clarg_short = "k"
    @clarg_doc = "Pattern name to KEEP despite default skip (repeatable). Special value 'all' disables pattern filtering entirely. Known patterns: 'dispatch'."
    keep : array<string>

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}


// Read newline-delimited paths from stdin. Used by --against-from-stdin
// to plug into shell pipelines: `git diff --name-only master |
// daslang utils/detect-dupe/main.das -- --against-from-stdin
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
    raw |> reserve(length(against))
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


// Walk records and tag any whose member identity (file:line:name) is
// NOT in the baseline as a candidate. Member-level rather than
// canonical-level so "growth" is caught: a new copy of an
// already-tracked canonical at a new location surfaces as a candidate
// (and the cluster containing it survives the filter), while unchanged
// records stay invisible. Additive — does not clear existing candidate
// flags from --against.
def tag_baseline_new_members(var records : array<FuncRecord>;
                             baseline_members : table<string; void?>) {
    for (r in records) {
        if (r.is_candidate) {
            continue
        }
        // Path-normalize so a Windows-recorded baseline (`a\b.das`) and a
        // Unix-runner record (`a/b.das`) collide instead of mass-tagging
        // every file as new.
        let mid = "{normalize_path(r.ref.file)}:{r.ref.line}:{r.ref.name}"
        if (!key_exists(baseline_members, mid)) {
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
        print_help(get_command_info(type<Config>), "detect-dupe")
        return
    }
    if (err != "") {
        to_log(LOG_ERROR, "error: {err}\n\n")
        print_help(get_command_info(type<Config>), "detect-dupe")
        return
    }
    let importing = !empty(cfg.import_functions)
    let exporting = !empty(cfg.export_functions)
    let has_against = length(cfg.against) > 0 || cfg.against_from_stdin
    let has_baseline = !empty(cfg.baseline)
    if (importing && length(cfg.path) > 0) {
        to_log(LOG_ERROR, "error: --import-functions and --path are mutually exclusive\n")
        return
    }
    if (importing && exporting) {
        to_log(LOG_ERROR, "error: --import-functions and --export-functions are mutually exclusive (transcode is a no-op)\n")
        return
    }
    if (exporting && (has_against || has_baseline)) {
        to_log(LOG_ERROR, "error: --export-functions is incompatible with --against / --baseline (export captures the raw corpus, not a filtered view)\n")
        return
    }
    if (cfg.baseline_strict && !has_baseline) {
        to_log(LOG_ERROR, "error: --baseline-strict requires --baseline\n")
        return
    }
    if (!importing && !has_against && length(cfg.path) == 0) {
        to_log(LOG_ERROR, "error: at least one --path is required (or pass --import-functions, or --against)\n\n")
        print_help(get_command_info(type<Config>), "detect-dupe")
        return
    }
    if (cfg.threshold < 0.0f || cfg.threshold > 1.0f) {
        to_log(LOG_ERROR, "error: --threshold must be in 0..1, got {cfg.threshold}\n")
        return
    }
    if (cfg.top < 0) {
        to_log(LOG_ERROR, "error: --top must be >= 0, got {cfg.top}\n")
        return
    }
    if (cfg.min_tokens < 0) {
        to_log(LOG_ERROR, "error: --min-tokens must be >= 0, got {cfg.min_tokens}\n")
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
            to_log(LOG_ERROR, "error: --against (or stdin) yielded no .das files\n")
            return
        }
    }

    var records : array<FuncRecord>
    var files_scanned = 0
    var seen_loc : table<string; void?>

    if (importing) {
        var ierr : string
        if (!read_import(cfg.import_functions, records, want_sigs, min_tokens, ierr)) {
            to_log(LOG_ERROR, "error: {ierr}\n")
            return
        }
        to_log(LOG_INFO, "loaded {length(records)} function record(s) from {cfg.import_functions}\n")
        if (has_against) {
            let dropped = drop_records_under_files(records, against_files)
            if (dropped > 0) {
                to_log(LOG_INFO, "dropped {dropped} corpus record(s) overridden by --against paths\n")
            }
        }
    } else {
        var files : array<string>
        var cache : table<string; void?>
        for (p in cfg.path) {
            scan_das_files(p, files, cache)
        }
        if (length(files) == 0 && !has_against) {
            to_log(LOG_ERROR, "error: no .das files found under given paths\n")
            return
        }
        if (length(files) > 0) {
            to_log(LOG_INFO, "scanning {length(files)} file(s)...\n")
        }

        var n_failed = 0
        var n_skipped = 0
        for (i in range(length(files))) {
            let file = files[i]
            if (cfg.verbose) {
                to_log(LOG_INFO, "[{i + 1}/{length(files)}] {file}\n")
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
        to_log(LOG_INFO, "collected {length(records)} function record(s); {n_failed} compile failure(s), {n_skipped} skipped (expect-directive)\n")

        if (exporting) {
            // In export mode, treat compile failures as fatal —
            // sharding scripts that merge multiple exports must be able
            // to detect partial-failure runs, same reason write
            // failures panic below. Best-effort export would silently
            // ship an incomplete corpus.
            if (n_failed > 0) {
                to_log(LOG_ERROR, "error: {n_failed} compile failure(s); refusing to write partial export\n")
                panic("detect-dupe export failed: {n_failed} compile failure(s)")
            }
            if (write_export(records, cfg.export_functions)) {
                to_log(LOG_INFO, "wrote {length(records)} function(s) to {cfg.export_functions}\n")
                return
            }
            // Use `panic` to surface a non-zero exit code so scripts
            // sharding compile-then-merge across machines can detect
            // partial-failure runs. The `error:` prefix matches the
            // format used elsewhere for usage errors in this CLI.
            to_log(LOG_ERROR, "error: could not write export to {cfg.export_functions}\n")
            panic("detect-dupe export failed")
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
        // Sort the candidate file list so compile-and-report order is
        // deterministic across runs — `keys(table)` iteration order is
        // unspecified, which would make the per-candidate output (and the
        // exit-code-driven CI behavior of `--check`) non-reproducible.
        var against_files_sorted : array<string>
        for (k in keys(against_files)) {
            against_files_sorted |> push(k)
        }
        sort(against_files_sorted)
        for (af in against_files_sorted) {
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
            to_log(LOG_INFO, "--against: compiled {n_against_compiled}, failed {n_against_failed}, skipped {n_against_skipped}\n")
        }
    }

    // B1 wiring: tag records whose member identity (file:line:name)
    // isn't in the baseline. The canonical set is also loaded for
    // --baseline-strict, which post-filters clusters whose canonical
    // already existed in baseline (so only "brand-new canonical"
    // clusters survive, not "existing canonical, new member" growth).
    var baseline_canonicals : table<string; void?>
    var baseline_members : table<string; void?>
    if (has_baseline) {
        var berr : string
        if (!load_baseline_canonicals(cfg.baseline, baseline_canonicals, baseline_members, berr)) {
            to_log(LOG_ERROR, "error: {berr}\n")
            return
        }
        to_log(LOG_INFO, "loaded {length(baseline_canonicals)} baseline canonical(s), {length(baseline_members)} member(s) from {cfg.baseline}\n")
        tag_baseline_new_members(records, baseline_members)
    }

    // Pattern filter — drop functions that match a known "boring" shape
    // (e.g. dastest dispatch outer functions whose body is N >= 2
    // identical statement chunks). Default is "skip all known patterns";
    // `--keep <name>` opts a specific pattern back in, `--keep all`
    // disables filtering entirely. Sits before clustering so dropped
    // records don't inflate cluster sizes or generate fuzzy pairs.
    var keep_all = false
    var kept_set : table<string; void?>
    for (k in cfg.keep) {
        if (k == "all") {
            keep_all = true
            continue
        }
        if (!key_exists(kept_set, k)) {
            kept_set |> insert(k, null)
        }
    }
    let skip_counts <- apply_pattern_filter(records, kept_set, keep_all, cfg.verbose)
    if (length(skip_counts) > 0) {
        var skip_pairs : array<tuple<string; int>>
        for (k, v in keys(skip_counts), values(skip_counts)) {
            skip_pairs |> push((k, v))
        }
        sort(skip_pairs) <| $(a, b) => a._0 < b._0
        var parts : array<string>
        for (kv in skip_pairs) {
            parts |> push("{kv._1} {kv._0}")
        }
        let joined = parts |> join(", ")
        to_log(LOG_INFO, "patterns skipped: {joined} (--keep <name>|all to include)\n")
    }

    let filter_candidates = has_against || has_baseline
    to_log(LOG_INFO, "clustering exact matches...\n")
    var clusters <- exact_buckets(records, min_tokens, filter_candidates)
    if (cfg.baseline_strict) {
        let before_strict = length(clusters)
        strict_drop_baseline_clusters(clusters, baseline_canonicals)
        let dropped_strict = before_strict - length(clusters)
        if (dropped_strict > 0) {
            to_log(LOG_INFO, "--baseline-strict: dropped {dropped_strict} cluster(s) whose canonical was already in baseline\n")
        }
    }
    sort_clusters(clusters)
    var pairs : array<FuzzyPair>
    if (!cfg.no_fuzzy) {
        to_log(LOG_INFO, "scanning {length(records)} records for fuzzy near-duplicates (threshold {threshold})...\n")
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
        // `top` applies to both writers — same flag, no implicit
        // mode-dependent default. Per-candidate mode used to halve the
        // value behind the user's back, but that misfired when the user
        // explicitly passed `--top 20` and silently downgraded it to 5.
        print_per_candidate_summary(rep, cand_refs, top)
    } else {
        print_summary(rep, top)
    }
    if (!empty(cfg.json)) {
        var json_ok = false
        if (use_per_candidate) {
            let cand_refs <- collect_candidate_refs(records)
            let body = write_per_candidate_report(rep, cand_refs, top)
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
            to_log(LOG_INFO, "\nJSON report written to {cfg.json}\n")
        } else {
            to_log(LOG_ERROR, "\nERROR: could not write JSON report to {cfg.json}\n")
        }
    }

    if (cfg.check && (length(rep.exact_clusters) > 0 || length(rep.fuzzy_pairs) > 0)) {
        // --check turns the post-filter report into a CI gate. Use
        // `panic` to surface a non-zero exit code (same pattern as the
        // export-failure path above).
        panic("detect-dupe --check failed: {length(rep.exact_clusters)} cluster(s), {length(rep.fuzzy_pairs)} pair(s)")
    }
}
