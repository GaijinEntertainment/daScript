options gen2

require daslib/lint
require daslib/perf_lint
require daslib/style_lint
require daslib/ast_boost
require daslib/fio
require math
require strings
require daslib/strings_boost

struct LintResult {
    file : string
    count : int
    errors : array<string>
    failed : bool
    compile_errors : string
}

def write_deduped(var writer : StringBuilderWriter; messages : array<string>; prefix : string = "  ") {
    var counts : table<string; int>
    var order : array<string>
    for (msg in messages) {
        if (key_exists(counts, msg)) {
            counts[msg]++
        } else {
            counts[msg] = 1
            order |> push(msg)
        }
    }
    for (msg in order) {
        let n = counts[msg]
        if (n > 1) {
            writer |> write("{prefix}{msg} (x{n})\n")
        } else {
            writer |> write("{prefix}{msg}\n")
        }
    }
}

def is_skip_file(name : string) : bool {
    return name == "builtin.das"
}

def is_skip_dir(name : string) : bool {
    return name |> starts_with(".")
}

def has_expect_directive(file : string) : bool {
    // Files with `expect <code>:<count>` declare intentional compile errors
    // for dastest. Lint cannot evaluate them — the program never reaches
    // the AST stage paranoid/perf/style passes need. Skip them.
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
            // extract filename from path
            let last_slash = max(rfind(path, "/"), rfind(path, "\\"))
            let fname = last_slash >= 0 ? slice(path, last_slash + 1) : path
            if (!is_skip_file(fname)) {
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
        } elif (fst.is_reg && name |> ends_with(".das") && !is_skip_file(name) && !cache |> key_exists(full)) {
            cache |> insert(full, null)
            files |> push(full)
        }
    }
}

def lint_file(file : string; run_paranoid, run_perf, run_style, postfix_conditionals : bool) : LintResult {
    var result = LintResult(file = file)
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.ignore_shared_modules = true
            cop.export_all = true
            cop.lint_check = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(file, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    result.failed = true
                    result.compile_errors = string(issues)
                    return
                }
                if (run_paranoid) {
                    var paranoid_issues : array<string>
                    result.count += paranoid_collect(program, paranoid_issues)
                    for (w in paranoid_issues) {
                        result.errors |> push(w)
                    }
                }
                if (run_perf) {
                    var perf_issues : array<string>
                    result.count += perf_lint_collect(program, perf_issues)
                    for (w in perf_issues) {
                        result.errors |> push(w)
                    }
                }
                if (run_style) {
                    var style_issues : array<string>
                    result.count += style_lint_collect(program, style_issues, postfix_conditionals)
                    for (w in style_issues) {
                        result.errors |> push(w)
                    }
                }
            }
        }
    }
    return <- result
}

[export]
def main() {
    let args <- get_command_line_arguments()
    let sep = find_index(args, "--") + 1
    if (sep <= 0 || sep >= length(args)) {
        print("Usage: daslang utils/lint/main.das -- <files-or-dirs...> [options]\n")
        print("\nUnified lint checker for daslang files.\n")
        print("Runs regular (paranoid), performance, and style lint passes.\n")
        print("Supports recursive directory scanning.\n")
        print("\nOptions:\n")
        print("  --quiet                  suppress PASS lines and progress\n")
        print("  --postfix-conditionals   enable STYLE005 check\n")
        print("  --paranoid-only          only run regular lint\n")
        print("  --perf-only              only run perf lint\n")
        print("  --style-only             only run style lint\n")
        return 1
    }
    var quiet = false
    var postfix = false
    var only_paranoid = false
    var only_perf = false
    var only_style = false
    var paths : array<string>
    for (i in range(sep, length(args))) {
        if (args[i] == "--quiet") {
            quiet = true
        } elif (args[i] == "--postfix-conditionals") {
            postfix = true
        } elif (args[i] == "--paranoid-only") {
            only_paranoid = true
        } elif (args[i] == "--perf-only") {
            only_perf = true
        } elif (args[i] == "--style-only") {
            only_style = true
        } else {
            paths |> push(args[i])
        }
    }
    if (length(paths) == 0) {
        print("Error: no files or directories specified\n")
        return 1
    }
    let any_only = only_paranoid || only_perf || only_style
    let run_paranoid = !any_only || only_paranoid
    let run_perf = !any_only || only_perf
    let run_style = !any_only || only_style
    // collect files
    var files : array<string>
    var cache : table<string; void?>
    for (p in paths) {
        scan_das_files(p, files, cache)
    }
    if (length(files) == 0) {
        print("Error: no .das files found\n")
        return 1
    }
    if (!quiet) {
        print("Linting {length(files)} file(s)...\n\n")
    }
    var total_issues = 0
    var total_errors = 0
    var total_skipped = 0
    for (file in files) {
        if (!quiet) {
            print("checking {file}...\n")
        }
        if (has_expect_directive(file)) {
            total_skipped++
            if (!quiet) {
                print("SKIP {file} (intentional compile errors via `expect`)\n")
            }
            continue
        }
        let result = lint_file(file, run_paranoid, run_perf, run_style, postfix)
        if (result.failed) {
            total_errors++
            print("FAIL {file}\n")
            if (!empty(result.compile_errors)) {
                print("  {result.compile_errors}\n")
            }
        } elif (result.count > 0) {
            total_issues += result.count
            print("WARN {file} ({result.count})\n")
            print(build_string() $(var w) {
                write_deduped(w, result.errors)
            })
        } elif (!quiet) {
            print("PASS {file}\n")
        }
    }
    print("\n--- Summary ---\n")
    if (total_skipped > 0) {
        print("{length(files)} files, {total_issues} issue(s), {total_errors} error(s), {total_skipped} skipped\n")
    } else {
        print("{length(files)} files, {total_issues} issue(s), {total_errors} error(s)\n")
    }
    if (total_errors > 0) {
        return 1
    } elif (total_issues > 0) {
        return 2
    } else {
        return 0
    }
}
