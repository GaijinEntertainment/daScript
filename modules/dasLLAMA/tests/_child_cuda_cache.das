options gen2
options _dasllama_internal = true

module _child_cuda_cache shared

require daslib/fio   // stat: the cubin a load reads
require ?cuda dasllama/dasllama_cuda_classes   // nolint:STYLE030,LINT019 - the kernel texts and the home, inside the dasCUDA half

// The fixture test_cuda_tower_front.das and test_cuda_kernels.das spawn: DASLLAMA_CUDA_CACHE_DIR is read at context init,
// so the folder a run uses is a child's. `dir` prints the folder the home's loads and prewarm use; `arm` arms the home
// and loads every kernel, twice over that folder, each arm's prewarm counts and the cubins found at the paths a load
// reads, one record each. The arm itself (`cuda_warm`) is the pod fleet's prewarm step too (../harness/cuda_prewarm.das).

//! one arm of the CUDA home with every kernel the program links loaded: the arm's prewarm counts and the kernels whose
//! ensure failed, or why the home did not arm (`why`, "" once armed)
struct CudaWarm {
    why : string
    compiled : int
    cached : int
    failed : int
    wall_ms : int64
    refused : array<string>
}

//! the home armed and every kernel the program links loaded (`cud_ensure_all`); the home stays armed for the caller,
//! which ends it with `cuda_warm_end`
def public cuda_warm() : CudaWarm {
    var w = CudaWarm(why = "the build has no CUDA module")
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (!cud_init()) {
            w.why = (!cuda_available() ? "no CUDA driver on this box"
                : !cuda_nvrtc_available() ? "the driver loads but NVRTC does not" : "the CUDA home has no device context")
            return <- w
        }
        w.why = ""
        w.refused <- cud_ensure_all()
        let r = cud_prewarm_report()
        w.compiled = r.compiled
        w.cached = r.cached
        w.failed = r.failed
        w.wall_ms = r.wall_ms
    }
    return <- w
}

//! the home `cuda_warm` armed, shut down
def public cuda_warm_end() {
    static_if (typeinfo builtin_module_exists(cuda)) {
        cud_shutdown()
    }
}

//! the prewarm step's one line: its counts and seconds, or why the home did not arm
def public cuda_warm_line(w : CudaWarm) : string {
    return "PREWARM none: {w.why}" if (!empty(w.why))
    let secs = fmt(":.1f", double(w.wall_ms) / 1000.0lf)
    return "PREWARM compiled={w.compiled} cached={w.cached} failed={w.failed} refused={length(w.refused)} in {secs} s"
}

//! the prewarm step's exit code: 1 where a kernel did not compile or load, else 0 - a home that does not arm too
def public cuda_warm_exit(w : CudaWarm) : int => (w.failed > 0 || !empty(w.refused)) ? 1 : 0

def private dir_case() {
    static_if (typeinfo builtin_module_exists(cuda)) {
        print("CACHE_DIR {cud_cache_dir()}\n")
    }
}

def private arm_case() {
    static_if (typeinfo builtin_module_exists(cuda)) {
        for (arm in range(2)) {
            var inscope w <- cuda_warm()
            if (!empty(w.why)) {
                print("ARM none\n")
                return
            }
            var rows = 0
            var hits = 0
            for (k in cud_kernel_texts()) {
                continue if (!cud_has_target(g_cud.sm, k.is_arch_specific))
                rows++
                let path = cubin_cache_path(cud_cache_dir(), k.text, cuda_text_compile_options(g_cud.sm, k.text))
                hits += cubin_is_hit(stat(path)) ? 1 : 0
            }
            print("ARM {arm} rows={rows} hits={hits} compiled={w.compiled} cached={w.cached} failed={w.failed} refused={length(w.refused)}\n")
            cuda_warm_end()
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
