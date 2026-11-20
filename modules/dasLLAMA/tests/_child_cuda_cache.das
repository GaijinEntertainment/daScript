options gen2
options _dasllama_internal = true

module _child_cuda_cache shared

require daslib/fio   // stat: the cubin a load reads
require ?cuda dasllama/dasllama_cuda_classes   // nolint:STYLE030,LINT019 - the kernel texts and the home, inside the dasCUDA half

// The fixture test_cuda_tower_front.das and test_cuda_kernels.das spawn: DASLLAMA_CUDA_CACHE_DIR is read at context init,
// so the folder a run uses is a child's. `dir` prints the folder the home's loads and prewarm use; `arm` arms the home
// and loads every kernel, twice over that folder, each arm's prewarm counts and the cubins found at the paths a load
// reads, one record each.

def private dir_case() {
    static_if (typeinfo builtin_module_exists(cuda)) {
        print("CACHE_DIR {cud_cache_dir()}\n")
    }
}

def private arm_case() {
    static_if (typeinfo builtin_module_exists(cuda)) {
        for (arm in range(2)) {
            if (!cud_init()) {
                print("ARM none\n")
                return
            }
            var inscope refused <- cud_ensure_all()
            let r = cud_prewarm_report()
            var rows = 0
            var hits = 0
            for (k in cud_kernel_texts()) {
                continue if (!cud_has_target(g_cud.sm, k.is_arch_specific))
                rows++
                let path = cubin_cache_path(cud_cache_dir(), k.text, cuda_text_compile_options(g_cud.sm, k.text))
                hits += cubin_is_hit(stat(path)) ? 1 : 0
            }
            print("ARM {arm} rows={rows} hits={hits} compiled={r.compiled} cached={r.cached} failed={r.failed} refused={length(refused)}\n")
            cud_shutdown()
        }
    }
}

//! `args`: `dir` or `arm`
def public child_cuda_cache(args : array<string>) {
    let what = empty(args) ? "" : args[0]
    if (what == "dir") {
        dir_case()
    } elif (what == "arm") {
        arm_case()
    }
    print("ROOT_DONE\n")
}
