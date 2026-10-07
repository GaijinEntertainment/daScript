options gen2
options indenting = 4

require daslib/rtti
require daslib/ast

//! compiles the file named after `--` through compile_file, under the module cache when an
//! argument is `cache`, unoptimized when one is `noopt` and fastcall-capped when one is `depth`;
//! the per-module compile-time log (a served module's line reads `cache read took`) is printed
//! for the test to read
[export]
def main {
    let args <- get_command_line_arguments()
    let sep = find_index(args, "--")
    if (sep < 0 || sep + 1 >= length(args)) {
        print("USAGE: mc_cf_drv.das -- <file.das> [cache] [noopt] [depth]\n")
        return
    }
    let target = args[sep + 1]
    var cached = false
    var noopt = false
    var depth = false
    for (i in range(sep + 2, length(args))) {
        cached ||= args[i] == "cache"
        noopt ||= args[i] == "noopt"
        depth ||= args[i] == "depth"
    }
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.module_cache = cached
            cop.no_optimizations = noopt
            cop.max_fast_call_depth = depth ? 64u : 0u
            cop.log_module_compile_time = true
            compile_file(target, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                print("{issues}\n")
                print(ok && program != null ? "MC_CF_COMPILED\n" : "MC_CF_FAILED\n")
            }
        }
    }
}
