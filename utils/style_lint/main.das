options gen2

require daslib/style_lint
require daslib/ast_boost
require daslib/fio
require strings
require daslib/strings_boost

[export]
def main() {
    let args <- get_command_line_arguments()
    let sep = find_index(args, "--") + 1
    if (sep <= 0 || sep >= length(args)) {
        print("Usage: daslang utils/style_lint/main.das -- file1.das [file2.das ...] [--quiet] [--postfix-conditionals]\n")
        print("\nStyle lint checker for daslang files.\n")
        print("Detects non-idiomatic patterns and suggests improvements.\n")
        print("\nRules:\n")
        print("  STYLE001  unnecessary <| pipe before block argument\n")
        print("  STYLE002  <| pipe before parameterless block (remove pipe and $())\n")
        print("  STYLE003  redundant $() on parameterless block argument\n")
        print("  STYLE005  single-statement if can use postfix conditional (--postfix-conditionals)\n")
        print("  STYLE006  string(__rtti) comparison should use `is` operator\n")
        print("  STYLE010  if (true) should be a bare block\n")
        unsafe {
            fio::exit(1)
        }
        return
    }
    var quiet = false
    var postfix = false
    var files : array<string>
    files |> reserve(length(args) - sep)
    for (i in range(sep, length(args))) {
        if (args[i] == "--quiet") {
            quiet = true
        } elif (args[i] == "--postfix-conditionals") {
            postfix = true
        } else {
            files |> push(args[i])
        }
    }
    if (length(files) == 0) {
        print("Error: no files specified\n")
        unsafe {
            fio::exit(1)
        }
        return
    }
    var total_warnings = 0
    var total_files = 0
    var failed_files = 0
    for (file in files) {
        if (!quiet) {
            print("checking {file}...\n")
        }
        total_files++
        {
            var inscope access <- make_file_access("")
            using() $(var mg : ModuleGroup) {
                using() $(var cop : CodeOfPolicies) {
                    cop.ignore_shared_modules = true
                    cop.export_all = true
                    cop.completion = true
                    cop.no_optimizations = true
                    cop.no_infer_time_folding = true
                    compile_file(file, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                        if (!ok) {
                            let issues_str = string(issues)
                            print("  COMPILE ERROR:\n{issues_str}\n")
                            failed_files++
                        } else {
                            total_warnings += style_lint(program, false, postfix)
                        }
                    }
                }
            }
        }
    }
    if (!quiet || total_warnings > 0 || failed_files > 0) {
        print("\n--- Summary ---\n")
        print("Files checked: {total_files}\n")
        if (failed_files > 0) {
            print("Compile errors: {failed_files}\n")
        }
        print("Style warnings: {total_warnings}\n")
    }
    unsafe {
        fio::exit(failed_files > 0 ? 1 : 0)
    }
}
