options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

require dasllama/dasllama_transformer
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_load
require dasllama/dasllama_image   // nolint:STYLE030,LINT019 — load_model_cached, used only inside the das_metal static_if half
require dasllama/dasllama_env
require ?das_metal dasllama/dasllama_metal_common
require ?das_metal dasllama/dasllama_metal_prefill
require ../performance/profile_common.das
require daslib/jobque_boost
require daslib/fio

def measure_one(tr : Model; npos : int64) {
    var s <- make_run_state(tr.config)
    var toks : array<int64>
    toks |> reserve(npos)
    for (i in range64(npos)) {
        toks |> push(100l + i % 5000l)
    }
    eval_(tr, s, toks)
    delete toks
    delete s
}

[export]
def main {
    let env = g_env_harness.models_dir
    let mdir = empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
    let path = path_join(mdir, "Qwen3-8B-BF16.gguf")
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_mode(MetalMode.required)
        var tr <- load_model_cached(path, QuantMode.q8)
        tr.config.seq_len = 2048l
        apply_box_profile_runtime()
        if (!tune_gate()) {
            panic("census: tune_gate refused")
        }
        with_job_que() {
            setup_dasllama_jobque_()
            measure_one(tr, 512l)   // warm
            metal_kernel_coverage_reset()
            measure_one(tr, 512l)   // the counted forward
            var cov <- metal_kernel_coverage()
            var names <- [for (name in keys(cov)); name]
            names |> sort()
            for (name in names) {
                if (cov[name] > 0l) {
                    print("census {name} = {cov[name]}\n")
                }
            }
            delete names
            delete cov
        }
        delete tr
    } else {
        print("census: no das_metal\n")
    }
}
