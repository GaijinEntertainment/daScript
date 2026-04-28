options gen2
options rtti
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public
require daslib/fio
require daslib/strings_convert
require daslib/result


// `export_corpus` shells out to `daslang utils/detect-dupe/main.das`
// (daslang's `require` grammar can't accept hyphens in path components,
// so the in-process pipeline isn't reachable from this tool). The MCP
// envelope returns the corpus path and subprocess output for diagnostics
// — the corpus body itself is not embedded (potentially many MB).

struct ExportCorpusEnvelope {
    exit_code : int
    out : string
    stdout : string                  //!< captured subprocess output
}


def public do_export_corpus(paths : string; out : string;
                            min_tokens_str : string) : string {
    let exe = get_daslang_exe()
    if (empty(exe)) {
        return make_tool_result("Cannot determine daslang executable path", true)
    }
    if (empty(out)) {
        return make_tool_result("export_corpus: missing required 'out' parameter (output corpus.json path)", true)
    }
    if (empty(paths)) {
        return make_tool_result("export_corpus: missing required 'paths' parameter (one or more .das files/dirs/globs)", true)
    }
    let detect_dupe_main = "{get_das_root()}/utils/detect-dupe/main.das"
    let detect_dupe_st = stat(detect_dupe_main)
    if (!detect_dupe_st.is_valid || !detect_dupe_st.is_reg) {
        return make_tool_result("export_corpus: utils/detect-dupe/main.das not found at {detect_dupe_main}", true)
    }
    var seeds : array<string>
    parse_file_list(paths, seeds)
    if (empty(seeds)) {
        return make_tool_result("export_corpus: no paths matched: {paths}", true)
    }
    let resolved_out = resolve_path(out)
    var cmd = "\"{exe}\" \"{detect_dupe_main}\" -- --export-functions \"{resolved_out}\""
    for (s in seeds) {
        cmd += " -p \"{resolve_path(s)}\""
    }
    if (!empty(min_tokens_str)) {
        let parsed = try_to_int(min_tokens_str)
        if (is_err(parsed)) {
            return make_tool_result("export_corpus: 'min_tokens' must be a non-negative integer (got '{min_tokens_str}')", true)
        }
        let n = unwrap(parsed)
        if (n < 0) {
            return make_tool_result("export_corpus: 'min_tokens' must be >= 0 (got {n})", true)
        }
        cmd += " --min-tokens {n}"
    }
    var output : string
    let exit_code = run_and_capture(cmd, output, 0.0)
    if (exit_code != 0) {
        return make_tool_result("export_corpus: detect-dupe failed (exit code {exit_code}):\n{output}", true)
    }
    let out_st = stat(resolved_out)
    if (!out_st.is_valid || !out_st.is_reg) {
        return make_tool_result("export_corpus: detect-dupe completed but corpus missing: {resolved_out}\nSubprocess output:\n{output}", true)
    }
    let envelope = ExportCorpusEnvelope(
        exit_code = exit_code,
        out = resolved_out,
        stdout = output)
    return make_tool_result(sprint_json(envelope, false), false)
}
