options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

// one-off instrument (deleted after the session): the per-kernel dispatch census of a REAL
// E2B pp512 / pp2048 forward + decode steps — the work-order intersection Boris asked for

require dasllama/dasllama
require dasllama/dasllama_layout
require dasllama/dasllama_math
require ?das_metal dasllama/dasllama_metal_common
require ?das_metal dasllama/dasllama_metal_decode
require ?das_metal metal/das_metal_boost
require daslib/jobque_boost
require daslib/fio
require strings
require math

def private long_prompt(n : int) : string {
    return build_string() $(var w) {
        for (i in range(1, n + 1)) {
            w |> write(i)
            w |> write(i < n ? ", " : ",")
        }
    }
}

def private window(tr : Model; tag : string; nums, steps : int) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_kernel_coverage_reset()
        var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
        let p <- encode(tr, long_prompt(nums), true, false)
        print("=== {tag}: prompt tokens = {length(p)}\n")
        eval_(tr, s, p)
        for (_i in range(steps)) {
            var one <- [42l]
            eval_(tr, s, one)
            delete one
        }
        var cov <- metal_kernel_coverage()
        var names <- [for (name in keys(cov)); name]
        names |> sort()
        for (name in names) {
            if (cov[name] > 0l) {
                print("  {tag} {name} = {cov[name]}\n")
            }
        }
        delete names
        delete cov
        delete s
    }
}

[export]
def main {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_wscale_f16(false)
        pin_kernel_backend("portable")
        var tr <- load_model_("/Users/borisbatkin/Work/llama.cpp/models/Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", QuantMode.q8)
        if (!convert_model_to_metal_blob(tr)) {
            print("blob transform failed\n")
            return
        }
        tr.config.seq_len = min(tr.config.seq_len, 4096l)
        select_prefill_override("metal")
        select_decode_override("metal")
        with_job_que() {
            setup_dasllama_jobque()
            window(tr, "pp512", 190, 0)
            window(tr, "pp2048", 780, 0)
            var ss : array<Session>
            ss |> reserve(2)
            var sess : array<Session?>
            var curs : array<int64>
            for (i in range(2)) {
                ss |> emplace <| make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
                let pb <- encode(tr, long_prompt(30 + i), true, false)
                eval_(tr, ss[i], pb)
                unsafe {
                    sess |> push(addr(ss[i]))
                }
                curs |> push(42l)
            }
            var ws <- create_batch_workspace_(tr)
            eval_batch_(tr, ws, sess, curs)
            print("batch step ok\n")
            sess |> clear()
            unsafe {
                delete ws
                delete sess
            }
            delete curs
            delete ss
        }
        metal_decode_shutdown()
        delete tr
    }
}
