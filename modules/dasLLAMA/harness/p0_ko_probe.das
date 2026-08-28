options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_load
require dasllama/dasllama_image   // nolint:STYLE030,LINT019 — load_model_cached, used only inside the das_metal static_if half
require dasllama/dasllama_env
require ?das_metal dasllama/dasllama_metal_prefill
require ../performance/profile_common.das   // tune_gate + apply_box_profile_runtime — the standing pre-measure stack
require daslib/jobque_boost
require daslib/fio

// Phase-0 KO attribution on Qwen3-8B (BF16 file, q8 lane): interleaved base/family knockout
// at npos=512, best-of-3, base re-measured before every family (drift-guarded). Adds the new
// act_cvt arm (dense activation converts) beside devw_cvt (weight staging). Direction-grade;
// output is GARBAGE while a family is skipped.

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
    let path = path_join(mdir, "Qwen3-8B-BF16.gguf")
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_mode(MetalMode.required)   // before load — pins the metal serving, same as the bench's --ngl leg
        var tr <- load_model_cached(path, QuantMode.q8)
        if (!tr.metal_blob) {
            panic("p0: the mapped flavor is not metal_blob - wrong leg")
        }
        tr.config.seq_len = 2048l
        apply_box_profile_runtime()
        if (!tune_gate()) {   // never measure on fallback winners
            panic("p0: tune_gate refused - tune first (or point DAS_TUNE_MANIFEST at a complete sidecar)")
        }
        with_job_que() {
            setup_dasllama_jobque_()
            measure_one(tr, 512l, 1)   // warm: JIT + blob upload + first graphs
            let npos = 512l
            //! "nofuse" matches no skip family: every kernel encodes with full math, so this arm
            //! is the null control (base-as-candidate) - and any empty-skip-gated fused route's
            //! on/off A/B, output VALID either way
            for (fam in ["nofuse", "gemm", "attn", "ew", "act_cvt", "devw_cvt", "nongemm"]) {
                let base = measure_one(tr, npos, 3)
                set_metal_prefill_skip(fam)
                let ms = measure_one(tr, npos, 3)
                set_metal_prefill_skip("")
                print("p0 ko npos={npos} base={base} skip={fam} ms={ms} delta_ms={base - ms} share={(base - ms) / base} (output GARBAGE - timing attribution only)\n")
            }
            let base_post = measure_one(tr, npos, 3)
            print("p0 ko base(post)={base_post}ms\n")
        }
        delete tr
    } else {
        print("p0: no das_metal on this build - nothing to measure for {path}\n")
    }
}
