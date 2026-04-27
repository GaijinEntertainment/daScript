options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public
require ../../find_dupes/pipeline.das
require ../../find_dupes/cluster.das
require ../../find_dupes/exchange.das
require ../../find_dupes/report.das
require ../../find_dupes/canonical.das
require strings
require math
require daslib/strings_boost


def parse_float_default(s : string; default_val : float) : float {
    if (empty(s)) {
        return default_val
    }
    return to_float(s)
}


def parse_int_default(s : string; default_val : int) : int {
    if (empty(s)) {
        return default_val
    }
    return to_int(s)
}


// MCP entry point: load a corpus and report functions in `paths` that
// resemble (or duplicate) functions already in the corpus. Output is
// the per-candidate JSON shape: one entry per candidate function with
// its top-N exact-cluster siblings and fuzzy partners.
//
// stdio transport reserves stdout for JSON-RPC, so every helper called
// here must run in quiet mode — `read_import(..., quiet=true)` and
// `compile_and_collect(..., quiet=true)`. Failure counts surface only
// via the returned envelope.
def public do_find_duplicates(paths : string; corpus : string;
                              threshold_str : string;
                              min_tokens_str : string;
                              top_str : string;
                              project : string) : string {
    if (empty(corpus)) {
        return make_tool_result("find_duplicates: missing required 'corpus' parameter (path to a corpus.json produced by `find_dupes --export-functions`)", true)
    }
    if (empty(paths)) {
        return make_tool_result("find_duplicates: missing required 'paths' parameter (one or more .das files; comma- or newline-delimited, or a glob)", true)
    }
    let threshold = parse_float_default(threshold_str, 0.7f)
    let min_tokens = parse_int_default(min_tokens_str, 8)
    let top = parse_int_default(top_str, 5)
    if (threshold < 0.0f || threshold > 1.0f) {
        return make_tool_result("find_duplicates: threshold must be in 0..1, got {threshold}", true)
    }
    if (min_tokens < 0) {
        return make_tool_result("find_duplicates: min_tokens must be >= 0, got {min_tokens}", true)
    }
    if (top < 0) {
        return make_tool_result("find_duplicates: top must be >= 0, got {top}", true)
    }

    // `parse_file_list` only splits on commas and globs, so normalize
    // newlines first — supports `git diff --name-only ... | …` style input
    // that the README documents.
    let paths_normalized = paths |> replace("\n", ",")
    var files : array<string>
    parse_file_list(paths_normalized, files)
    if (empty(files)) {
        return make_tool_result("find_duplicates: no files matched: {paths}", true)
    }
    // Build the candidate file set (normalized for comparison) AND keep a
    // sorted list for deterministic compile order — table key iteration is
    // unordered, which would make compile order (and per-candidate report
    // order) non-reproducible across runs.
    var against_files : table<string; void?>
    var against_files_sorted : array<string>
    for (f in files) {
        let n = normalize_path(f)
        if (!key_exists(against_files, n)) {
            against_files |> insert(n, null)
            against_files_sorted |> push(n)
        }
    }
    sort(against_files_sorted)

    // Load the corpus. want_sigs=true because we need MinHash signatures
    // for fuzzy matching — both sides of every comparison must have one.
    var records : array<FuncRecord>
    var ierr : string
    if (!read_import(corpus, records, true, min_tokens, ierr, true)) {
        return make_tool_result("find_duplicates: {ierr}", true)
    }
    let corpus_total = length(records)
    let dropped = drop_records_under_files(records, against_files)

    // Compile the candidate files in-process and tag the freshly-pushed
    // records as candidates. Iterate the sorted list, not the table.
    var seen_loc : table<string; void?>
    var n_failed = 0
    var n_compiled = 0
    var failed_files : array<string>
    for (af in against_files_sorted) {
        let before = length(records)
        if (!compile_and_collect(af, records, seen_loc, false, false, true, min_tokens, true, project)) {
            n_failed++
            failed_files |> push(af)
            continue
        }
        for (i in range(before, length(records))) {
            records[i].is_candidate = true
        }
        n_compiled++
    }

    let cand_refs <- collect_candidate_refs(records)
    if (empty(cand_refs)) {
        let msg = (n_failed > 0
            ? "find_duplicates: no candidate functions extracted (compile failed for {n_failed} file(s); first: {failed_files[0]})"
            : "find_duplicates: no candidate functions extracted from {paths}")
        return make_tool_result(msg, true)
    }

    var clusters <- exact_buckets(records, min_tokens, true)
    sort_clusters(clusters)
    var pairs <- fuzzy_pairs(records, threshold, min_tokens, true)
    sort_pairs(pairs)

    var rep : Report
    rep.summary.functions_analyzed = length(records)
    rep.summary.threshold = threshold
    rep.summary.min_tokens = min_tokens
    rep.exact_clusters <- clusters
    rep.fuzzy_pairs <- pairs
    rep.summary.exact_clusters = length(rep.exact_clusters)
    rep.summary.fuzzy_pairs = length(rep.fuzzy_pairs)

    // Wrap the per-candidate report in a small envelope so consumers can
    // see corpus stats and compile-failure counts without parsing stdout.
    let pcr_body = write_per_candidate_report(rep, cand_refs, top)
    let envelope = build_string() $(var w) {
        w |> write("\{")
        w |> write("\"corpus_records\": {corpus_total}, ")
        w |> write("\"corpus_dropped_for_override\": {dropped}, ")
        w |> write("\"candidate_files_compiled\": {n_compiled}, ")
        w |> write("\"candidate_files_failed\": {n_failed}, ")
        w |> write("\"candidate_functions\": {length(cand_refs)}, ")
        w |> write("\"exact_clusters_kept\": {rep.summary.exact_clusters}, ")
        w |> write("\"fuzzy_pairs_kept\": {rep.summary.fuzzy_pairs}, ")
        w |> write("\"report\": ")
        w |> write(pcr_body)
        w |> write("\}")
    }
    return make_tool_result(envelope, n_failed > 0)
}
