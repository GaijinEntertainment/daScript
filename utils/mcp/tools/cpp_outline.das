options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require cpp_common public
require daslib/strings_boost public
require strings public

// `do_cpp_outline` — list top-level declarations in C++ files.
//
// Either `file` (single absolute or repo-relative path) or `glob_filter`
// (e.g. "src/builtin/*.cpp") must be set. With both, the glob filters within
// the file's directory.
//
// Output groups entries by file; within each file entries are sorted by line.
def do_cpp_outline(file, glob_filter : string) : string {
    if (empty(file) && empty(glob_filter)) {
        return make_tool_result("'file' or 'glob_filter' argument is required", true)
    }
    var search_dirs : array<string>
    if (!empty(file)) {
        search_dirs |> push(resolve_path(file))
    } else {
        search_dirs <- cpp_default_search_dirs()
    }
    var err : string
    var entries <- cpp_run_scan(search_dirs, glob_filter, err)
    if (!empty(err)) {
        return make_tool_result(err, true)
    }
    if (empty(entries)) {
        return make_tool_result("No C++ declarations found")
    }
    // group by file, preserve discovery order
    var by_file : table<string; array<int>>
    var file_order : array<string>
    file_order |> reserve(length(entries))
    for (i in range(length(entries))) {
        let fp = entries[i].file
        if (!key_exists(by_file, fp)) {
            file_order |> push(fp)
        }
        by_file[fp] |> push(i) // nolint:PERF006
    }
    // sort within each file by line
    for (fp in file_order) {
        sort(by_file[fp]) $(a, b) {
            return entries[a].line < entries[b].line
        }
    }
    let multi_file = length(file_order) > 1
    let output = build_string() $(var w) {
        for (fp_idx in range(length(file_order))) {
            let fp = file_order[fp_idx]
            if (multi_file) {
                w |> write("{fp}:\n")
            }
            for (idx in by_file[fp]) {
                let e & = unsafe(entries[idx])
                let display = empty(e.qualified) ? e.text : "{e.qualified}"
                w |> write("  {e.line}: [{e.kind}] {display}\n")
            }
            if (multi_file && fp_idx < length(file_order) - 1) {
                w |> write("\n")
            }
        }
    }
    return make_tool_result(output)
}
