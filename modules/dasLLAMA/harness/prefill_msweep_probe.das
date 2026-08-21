options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_load
require dasllama/dasllama_image   // load_model_cached — the .dlim rail, the bench's own metal leg
require dasllama/dasllama_env
require ?das_metal dasllama/dasllama_metal_prefill
require daslib/jobque_boost
require daslib/fio

// The Metal prefill M-curve on ONE loaded model: per-npos best-of ms/tok with stage stats,
// then the DASLLAMA_METAL_PREFILL_SKIP knockouts at a small and a large M. Direction-grade —
// it found the 64-row M-pad staircase (npos=160 billed as 192) that the ceil32 pad removed.

def measure_one(tr : Model; npos : int64; reps : int) : double {
    var best = 1.0e30lf
    for (_r in range(reps)) {
        var s <- make_run_state(tr.config)
        var toks : array<int64>
        toks |> reserve(npos)
        for (i in range64(npos)) {
            toks |> push(100l + i % 5000l)
        }
        let t0 = ref_time_ticks()
        eval_(tr, s, toks)
        let ms = double(get_time_usec(t0)) / 1000.0lf
        if (ms < best) {
            best = ms
        }
        delete toks
        delete s
    }
    return best
}

[export]
def main {
    let env = g_env_harness.models_dir
    let mdir = empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
    let path = path_join(mdir, "gemma-4-12B-it-Q4_K_M.gguf")
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_mode(MetalMode.required)   // before load — pins the metal serving, same as the bench's --ngl leg
        var tr <- load_model_cached(path, QuantMode.q8)
        if (!tr.metal_blob) {
            panic("msweep: the mapped flavor is not metal_blob - wrong leg")
        }
        tr.config.seq_len = 2048l
        with_job_que() {
            setup_dasllama_jobque_()
            measure_one(tr, 512l, 1)   // warm: JIT + blob upload + first graphs
            let p0 = metal_prefill_stats()
            for (npos in [512l, 384l, 256l, 192l, 160l, 128l, 96l, 64l]) {
                metal_prefill_stage_reset()
                let best = measure_one(tr, npos, 3)
                let ss = metal_prefill_stage_stats()
                print("msweep npos={npos} best_ms={best} ms_tok={best / double(npos)} stage(prefills={ss.prefills} setup={ss.setup_us} enc={ss.encode_us} wait={ss.wait_us} gpu={ss.gpu_us} rb={ss.readback_us})\n")
            }
            let p1 = metal_prefill_stats()
            print("msweep serve check: prefills +{p1.prefills - p0.prefills}, declines +{p1.declines - p0.declines}\n")
            for (npos in [160l, 512l]) {
                let base = measure_one(tr, npos, 3)
                print("knockout npos={npos} base={base}ms\n")
                for (fam in ["attn", "gemm", "ew", "nongemm"]) {
                    set_metal_prefill_skip(fam)
                    let ms = measure_one(tr, npos, 3)
                    set_metal_prefill_skip("")
                    print("knockout npos={npos} skip={fam} ms={ms} share={(base - ms) / base}\n")
                }
            }
        }
        delete tr
    } else {
        print("msweep: no das_metal on this build\n")
    }
}
