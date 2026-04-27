options gen2
options rtti
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public
require ../../find_dupes/pipeline.das
require ../../find_dupes/cluster.das
require ../../find_dupes/exchange.das
require strings


struct ExportCorpusEnvelope {
    out : string
    files_scanned : int
    files_skipped : int
    records_written : int
}


def parse_int_default(s : string; default_val : int) : int {
    if (empty(s)) {
        return default_val
    }
    return to_int(s)
}


// MCP entry point: scan the given paths, compile each .das file in
// process, and write a corpus.json that `find_duplicates` can later
// consume. This is the MCP-side replacement for the CLI's
// `find_dupes --export-functions <out>` so the whole workflow stays
// inside the MCP server (no shelling out to daslang).
//
// Args:
//   paths     comma- or newline-delimited list of files/dirs/globs
//   out       output corpus.json path
//   min_tokens minimum token count per record (default 0 — keep all,
//              matches the CLI default for export mode where it's a
//              raw dump of the corpus and downstream callers re-filter)
//   project   optional .das_project for module resolution
//
// stdio is reserved for JSON-RPC, so `compile_and_collect` runs with
// quiet=true and we report counts via the returned envelope.
def public do_export_corpus(paths : string; out : string;
                            min_tokens_str : string;
                            project : string) : string {
    if (empty(out)) {
        return make_tool_result("export_corpus: missing required 'out' parameter (output corpus.json path)", true)
    }
    if (empty(paths)) {
        return make_tool_result("export_corpus: missing required 'paths' parameter (one or more .das files/dirs/globs)", true)
    }
    let min_tokens = parse_int_default(min_tokens_str, 0)
    if (min_tokens < 0) {
        return make_tool_result("export_corpus: min_tokens must be >= 0, got {min_tokens}", true)
    }

    var seeds : array<string>
    parse_file_list(paths, seeds)
    if (empty(seeds)) {
        return make_tool_result("export_corpus: no paths matched: {paths}", true)
    }

    // scan_das_files handles dirs, files, and globs uniformly and applies
    // the daslib-skip filter (debugger/profiler agents would crash on
    // re-install).
    var files : array<string>
    var cache : table<string; void?>
    for (s in seeds) {
        scan_das_files(s, files, cache)
    }
    if (empty(files)) {
        return make_tool_result("export_corpus: no .das files found under: {paths}", true)
    }
    sort(files)

    var records : array<FuncRecord>
    var seen_loc : table<string; void?>
    var n_failed = 0
    var n_skipped = 0
    var failed_files : array<string>
    for (f in files) {
        if (has_expect_directive(f)) {
            n_skipped++
            continue
        }
        // want_sigs=false: export captures the raw token stream; MinHash
        // sigs are recomputed on import, so signing here would just be
        // discarded work. Mirrors the CLI's export-mode behavior.
        if (!compile_and_collect(f, records, seen_loc, false, false, false, min_tokens, true, project)) {
            n_failed++
            failed_files |> push(f)
        }
    }

    if (n_failed > 0) {
        // Matches CLI behavior: refuse to write a partial corpus.
        // Sharding scripts that merge exports rely on this signal.
        let first_fail = empty(failed_files) ? "" : failed_files[0]
        return make_tool_result("export_corpus: {n_failed} compile failure(s); refusing to write partial export (first: {first_fail})", true)
    }

    if (!write_export(records, out)) {
        return make_tool_result("export_corpus: could not write export to {out}", true)
    }

    let envelope = ExportCorpusEnvelope(
        out = out,
        files_scanned = length(files),
        files_skipped = n_skipped,
        records_written = length(records))
    return make_tool_result(sprint_json(envelope, false), false)
}
