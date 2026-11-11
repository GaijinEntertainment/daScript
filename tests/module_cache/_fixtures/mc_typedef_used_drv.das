options gen2
options indenting = 4

require daslib/rtti
require daslib/ast

//! compiles the file named after `--` under the module cache and
//! prints whether it names a typedef of the module named next;
//! the compile-time log shows which modules the cache served
[export]
def main {
    let args <- get_command_line_arguments()
    let sep = find_index(args, "--")
    if (sep < 0 || sep + 2 >= length(args)) {
        print("USAGE: mc_typedef_used_drv.das -- <file.das> <module>\n")
        return
    }
    let target = args[sep + 1]
    let asked = args[sep + 2]
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.module_cache = true
            cop.log_module_compile_time = true
            compile_file(target, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                print("{issues}\n")
                if (!ok || program == null) {
                    print("MC_TYPEDEF_USED_FAILED\n")
                    return
                }
                var used = false
                program_for_each_module(program) $(mod) {
                    used ||= mod.name == asked && is_typedef_used(program, mod)
                }
                print("MC_TYPEDEF_USED {asked}={used}\n")
            }
        }
    }
}
