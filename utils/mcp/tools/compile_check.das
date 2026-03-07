options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

def compile_check_single(file : string; project : string = "") : string {
    return compile_and_simulate(file, project) <| $(program; issues) {
        var warnings = ""
        if (!empty(issues)) {
            warnings = "Warnings:\n{issues}\n"
        }
        var funcs = build_string() <| $(var writer) {
            write_function_list(writer, program)
        }
        return "{warnings}Compilation OK.\n{funcs}"
    }
}

def do_compile_check(file : string; project : string = "") : string {
    var files : array<string>
    parse_file_list(file, files)
    if (empty(files)) {
        return make_tool_result("no files matched: {file}", true)
    }
    // Single file — original behavior with function list
    if (length(files) == 1) {
        return compile_check_single(files[0], project)
    }
    // Batch mode — compile each file, collect results
    var passed = 0
    var failed = 0
    var output = build_string() <| $(var w) {
        for (f in files) {
            let result = compile_check_single(f, project)
            let is_error = find(result, "\"isError\":true") >= 0
            if (is_error) {
                failed++
                // Extract error text from JSON result
                let text_start = find(result, "\"text\":\"")
                if (text_start >= 0) {
                    let content_start = text_start + 8
                    let text_end = find(result, "\"", content_start)
                    if (text_end >= 0) {
                        write(w, "FAIL {f}\n")
                    } else {
                        write(w, "FAIL {f}\n")
                    }
                } else {
                    write(w, "FAIL {f}\n")
                }
            } else {
                passed++
                write(w, "PASS {f}\n")
            }
        }
        write(w, "\n{passed + failed} files, {passed} passed, {failed} failed\n")
    }
    return make_tool_result(output, failed > 0)
}
