options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public
require daslib/perf_lint
require strings

def do_perf_lint_single(file : string; project : string = "") : string {
    return compile_only(file, true, true, project) <| $(program; issues) {
        var compile_warnings = ""
        if (!empty(issues)) {
            compile_warnings = "Compilation warnings:\n{issues}\n"
        }
        var warnings : array<string>
        let count = perf_lint_collect(program, warnings)
        if (count == 0) {
            return "{compile_warnings}No performance warnings."
        }
        return build_string() <| $(var w) {
            if (!empty(compile_warnings)) {
                w |> write(compile_warnings)
            }
            w |> write("{count} performance warning(s):\n")
            write_deduped(w, warnings)
        }
    }
}

struct PerfLintFileResult {
    file : string
    count : int
    warnings : array<string>
    failed : bool
}

def do_perf_lint_file(file : string; project : string = "") : PerfLintFileResult {
    var result = PerfLintFileResult(file = file)
    var inscope access <- make_file_access(project)
    using() <| $(var mg : ModuleGroup) {
        using() <| $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.ignore_shared_modules = true
            cop.export_all = true
            cop.no_optimizations = true
            compile_file(file, access, unsafe(addr(mg)), cop) <| $(ok; program; issues) {
                if (!ok) {
                    result.failed = true
                } else {
                    result.count = perf_lint_collect(program, result.warnings)
                }
            }
        }
    }
    return <- result
}

def do_perf_lint(file : string; project : string = "") : string {
    var files : array<string>
    parse_file_list(file, files)
    if (empty(files)) {
        return make_tool_result("no files matched: {file}", true)
    }
    if (length(files) == 1) {
        return do_perf_lint_single(files[0], project)
    }
    // batch mode
    var total_warnings = 0
    var total_errors = 0
    var output = build_string() <| $(var w) {
        for (f in files) {
            let result = do_perf_lint_file(f, project)
            if (result.failed) {
                total_errors++
                w |> write("FAIL {f}\n")
            } elif (result.count > 0) {
                total_warnings += result.count
                w |> write("WARN {f} ({result.count})\n")
                write_deduped(w, result.warnings)
            } else {
                w |> write("PASS {f}\n")
            }
        }
        w |> write("\n{length(files)} files, {total_warnings} warning(s), {total_errors} error(s)\n")
    }
    return make_tool_result(output, total_errors > 0)
}
