options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_blocks
require dasllama/dasllama_load
require dasllama/dasllama_image   // nolint:STYLE030,LINT019 — load_model_cached, used only inside the das_metal static_if half
require dasllama/dasllama_env
require ?das_metal dasllama/dasllama_metal_prefill
require ../performance/profile_common.das   // tune_gate + apply_box_profile_runtime — the standing pre-measure stack
require daslib/jobque_boost
require daslib/fio
require math

// The IMAGE-TURN attribution probe: one loaded model, npos=321 (the coco turn shape), three
// workloads — plain tokens, a narrow embd quantum with the mrope span, and the ds-wide
// quantum — crossed with the DASLLAMA_METAL_PREFILL_SKIP knockout arms, all in ONE process.
// Splits the image-turn tax over text into its (mrope+span) and (ds-wide) halves, then
// attributes whichever half carries it across stage families. Direction-grade, output GARBAGE
// on the skip arms by contract.

let NPOS = 321l
let SPAN_LO = 4l
let SPAN_HI = 304l   // 300 media rows, the 20x15 coco grid
let GRID_X = 20l
let GRID_Y = 15l
let REPS = 3

def measure_tokens(tr : Model; reps : int) : double {
    var best = 1.0e30lf
    for (_r in range(reps)) {
        var s <- make_run_state(tr.config)
        var toks : array<int64>
        toks |> reserve(NPOS)
        for (i in range64(NPOS)) {
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

def measure_span(tr : Model; wide : bool; reps : int) : double {
    let dim = tr.config.dim
    let stride = wide ? (1l + tr.config.n_deepstack) * dim : dim
    var best = 1.0e30lf
    for (_r in range(reps)) {
        var s <- make_run_state(tr.config)
        var embd : array<float>
        embd |> resize(NPOS * stride)
        for (i in range64(NPOS * stride)) {
            embd[i] = 0.01 * sin(float(i % 1024l))
        }
        let t0 = ref_time_ticks()
        eval_embd_span_mrope_(tr, s, embd, NPOS, SPAN_LO, SPAN_HI, GRID_X, GRID_Y)
        let ms = double(get_time_usec(t0)) / 1000.0lf
        if (ms < best) {
            best = ms
        }
        delete embd
        delete s
    }
    return best
}

[export]
def main {
    let env = g_env_harness.models_dir
    let mdir = empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
    let mname = get_env_variable("PROBE_MODEL")
    let path = path_join(mdir, empty(mname) ? "Qwen3VL-8B-Instruct-Q8_0.gguf" : mname)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_mode(MetalMode.required)   // before load — pins the metal serving, same as the bench's --ngl leg
        var tr <- load_model_cached(path, QuantMode.q8)
        if (!tr.metal_blob) {
            panic("image_turn_probe: the mapped flavor is not metal_blob - wrong leg")
        }
        tr.config.seq_len = 2048l
        apply_box_profile_runtime()
        if (!tune_gate()) {   // never measure on fallback winners
            panic("image_turn_probe: tune_gate refused - point DAS_TUNE_MANIFEST at a complete sidecar")
        }
        with_job_que() {
            setup_dasllama_jobque_()
            measure_tokens(tr, 1)   // warm: JIT + first touch + schedule compile
            for (arm in ["", "gemm", "attn", "ew", "nongemm"]) {
                set_metal_prefill_skip(arm)
                let t = measure_tokens(tr, REPS)
                let nsp = measure_span(tr, false, REPS)
                let wsp = measure_span(tr, true, REPS)
                let tag = empty(arm) ? "full" : arm
                print("PROBE {tag}: tokens {t} ms | narrow-span {nsp} ms | wide-span {wsp} ms | narrow-tax {nsp - t} | wide-tax {wsp - t}\n")
            }
            set_metal_prefill_skip("")
        }
        delete tr
    } else {
        print("image_turn_probe: no das_metal on this build - nothing to probe\n")
    }
}
