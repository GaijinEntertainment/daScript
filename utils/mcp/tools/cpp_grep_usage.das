options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require cpp_common public
require daslib/strings_boost public
require strings public

// `do_cpp_grep_usage` — parse-aware C++ identifier search via `ast-grep run -p`.
//
// Returns every textual occurrence of `symbol` in C++ files (excluding strings/comments).
// `directory`: optional — repo-relative or absolute; empty -> default cpp dirs.
// `context_lines_str`: reserved (currently unused; ast-grep already returns the matched line).
// `glob_filter`: optional --globs filter (gitignore syntax; `!pattern` for exclusion).
def do_cpp_grep_usage(symbol, directory, context_lines_str, glob_filter : string) : string {
    if (empty(symbol)) {
        return make_tool_result("'symbol' argument is required", true)
    }
    let sg = get_ast_grep_cmd()
    if (empty(sg)) {
        return make_tool_result("ast-grep not available", true)
    }
    let das_root = get_das_root()
    let search_dirs <- cpp_resolve_search_dirs(directory)
    var args <- [sg, "run", "-p", symbol, "-l", "cpp", "--json"]
    cpp_push_glob_args(args, search_dirs, glob_filter)
    args |> reserve(length(args) + length(search_dirs))
    for (d in search_dirs) {
        args |> push(d)
    }
    var raw_json : string
    let rc = run_and_capture(args, raw_json, 60.0)
    if (rc != 0 && empty(raw_json)) {
        return make_tool_result("ast-grep failed (exit {rc})", true)
    }
    if (empty(raw_json) || raw_json == "[]") {
        return make_tool_result("No matches found for '{symbol}'")
    }
    var parse_err : string
    var parsed = read_json(raw_json, parse_err)
    if (parsed == null) {
        let truncated = length(raw_json) > 500 ? slice(raw_json, 0, 500) : raw_json
        return make_tool_result("Failed to parse ast-grep output: {parse_err}\nRaw: {truncated}", true)
    }
    if (!(parsed.value is _array)) {
        unsafe { delete parsed; }
        return make_tool_result("Unexpected ast-grep output format", true)
    }
    // group matches by file
    var by_file : table<string; array<string>>
    var file_order : array<string>
    unsafe {
        let matches & = parsed.value as _array
        for (m in matches) {
            let file_path = get_json_string(m, "file")
            if (empty(file_path)) {
                continue
            }
            let rng = (m.value as _object)?["range"] ?? null
            let start = rng != null ? ((rng.value as _object)?["start"] ?? null) : null
            let line_num = start != null ? get_json_int(start, "line") + 1 : 0
            let lines_val = (m.value as _object)?["lines"] ?? null
            var context = ""
            if (lines_val != null && (lines_val.value is _string)) {
                context = strip(lines_val.value as _string)
            }
            let display_path = make_relative_path(file_path, das_root)
            let entry = "  {line_num}: {context}"
            if (!key_exists(by_file, display_path)) {
                file_order |> push(display_path)
            }
            by_file[display_path] |> push(entry)
        }
    }
    unsafe { delete parsed; }
    var match_count = 0
    for (fp in file_order) {
        match_count += length(by_file[fp])
    }
    let total = length(by_file)
    if (match_count == 0) {
        return make_tool_result("No matches found for '{symbol}'")
    }
    let output = build_string() $(var w) {
        w |> write("Found {match_count} match")
        if (match_count != 1) {
            w |> write("es")
        }
        w |> write(" for '{symbol}' in {total} file")
        if (total != 1) {
            w |> write("s")
        }
        w |> write("\n\n")
        for (fp in file_order) {
            w |> write("{fp}:\n")
            for (entry in by_file[fp]) {
                w |> write("{entry}\n")
            }
            w |> write("\n")
        }
    }
    return make_tool_result(output)
}
