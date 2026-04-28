options gen2
options rtti
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public
require daslib/fio


// `detect_duplicates` shells out to `daslang utils/detect-dupe/main.das`
// because daslang's `require` grammar can't accept hyphens in path
// components. Per-call subprocess overhead is acceptable for an
// interactive MCP tool — and matches how `find_dupe` / `judge_duplicates`
// already invoke their respective utilities.

struct DetectDuplicatesEnvelope {
    exit_code : int
    @embed report : string           //!< raw per-candidate JSON from --json
    stdout : string                  //!< captured subprocess output (diagnostics)
}


def public do_detect_duplicates(paths : string; corpus : string;
                                threshold_str : string;
                                min_tokens_str : string;
                                top_str : string;
                                keep_str : string;
                                project : string) : string {
    let exe = get_daslang_exe()
    if (empty(exe)) {
        return make_tool_result("Cannot determine daslang executable path", true)
    }
    if (empty(corpus)) {
        return make_tool_result("detect_duplicates: missing required 'corpus' parameter (path to a corpus.json produced by the `export_corpus` MCP tool or `detect-dupe --export-functions`)", true)
    }
    if (empty(paths)) {
        return make_tool_result("detect_duplicates: missing required 'paths' parameter (one or more .das files; comma- or newline-delimited, or a glob)", true)
    }
    let detect_dupe_main = "{get_das_root()}/utils/detect-dupe/main.das"
    let detect_dupe_st = stat(detect_dupe_main)
    if (!detect_dupe_st.is_valid || !detect_dupe_st.is_reg) {
        return make_tool_result("detect_duplicates: utils/detect-dupe/main.das not found at {detect_dupe_main}", true)
    }
    var files : array<string>
    parse_file_list(paths, files)
    if (empty(files)) {
        return make_tool_result("detect_duplicates: no files matched: {paths}", true)
    }
    let resolved_corpus = resolve_path(corpus)
    let corpus_st = stat(resolved_corpus)
    if (!corpus_st.is_valid || !corpus_st.is_reg) {
        return make_tool_result("detect_duplicates: corpus file not found: {resolved_corpus}", true)
    }
    let json_out = "{get_das_root()}/_mcp_detect_duplicates_{ref_time_ticks()}.json"
    var cmd = "\"{exe}\" \"{detect_dupe_main}\" -- --import-functions \"{resolved_corpus}\" --json \"{json_out}\""
    for (f in files) {
        cmd += " --against \"{resolve_path(f)}\""
    }
    if (!empty(threshold_str)) {
        cmd += " -t {threshold_str}"
    }
    if (!empty(min_tokens_str)) {
        cmd += " --min-tokens {min_tokens_str}"
    }
    if (!empty(top_str)) {
        cmd += " -n {top_str}"
    }
    if (!empty(keep_str)) {
        for (k in split_by_chars(keep_str, ", \t\n")) {
            if (!empty(k)) {
                cmd += " -k {k}"
            }
        }
    }
    var output : string
    let exit_code = run_and_capture(cmd, output, 0.0)
    if (exit_code != 0) {
        remove(json_out)
        return make_tool_result("detect-dupe failed (exit code {exit_code}):\n{output}", true)
    }
    let json_st = stat(json_out)
    if (!json_st.is_valid || !json_st.is_reg) {
        return make_tool_result("detect-dupe completed but JSON report missing: {json_out}\nSubprocess output:\n{output}", true)
    }
    var envelope : DetectDuplicatesEnvelope
    envelope.exit_code = exit_code
    envelope.report := fread(json_out)
    envelope.stdout := output
    remove(json_out)
    return make_tool_result(sprint_json(envelope, false), false)
}
