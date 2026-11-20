options gen2

require daslib/ast_boost
require daslib/fio
require daslib/module_path
require daslib/rtti
require llvm/daslib/llvm_env
require llvm/daslib/llvm_jit_sweep
require _llvm_cache_common

[export]
def main() {
    let client = "{get_this_module_dir()}/_llvm_compile_only_client.das"
    for (round in range(2)) {
        compile_for_jit(client, false) $(var program) {
            simulate(program) $(sok, context, _serrors) {
                if (!sok) return
                let cfg : JitCliOptions
                let r = run_jit(get_ptr(program), get_ptr(context), cfg)
                print("ROUND {round} ok={r.ok}\n")
            }
        }
        if (round == 0) {
            var inscope recorded : array<string>
            dir(g_env_jit.jit_manifest) $(name) {
                if (name != "." && name != "..") {
                    recorded |> push(name)
                }
            }
            for (name in recorded) {
                remove(path_join(g_env_jit.jit_manifest, name))
            }
            print("EMPTIED {length(recorded)}\n")
        }
    }
    jit_manifest_record(path_join(g_env_jit.jit_manifest, "pinned_output"), "")
    print("KEYLESS DONE\n")
}
