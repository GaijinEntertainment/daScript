options gen2
options stack = 65536   // dasLLAMA program-root convention (options stack does not unify up from libs)
options _jit_fast_math = true   // ggml-parity FP laxity (non-bit-exact, ~+10%); tests stay bit-exact

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require daslib/jobque_boost
require daslib/fio
require strings
require dasllama/dasllama_math  // setup_dasllama_jobque_ + the runtime knob setters (engine-level, not re-exported)

// Generation-speed profile for a Llama-3 GGUF (Q8). Prefills a short prompt (untimed),
// then times N greedy generation steps — the batch-1 gemv path, matching llama.cpp's `tg`.
// Tokenizer-independent (forward speed doesn't depend on content), so no BPE vocab needed.
//   bin/daslang -jit modules/dasLLAMA/dasllama_llama3_perf.das -- <model.gguf>

let DEFAULT_MODEL = "/Users/borisbatkin/Work/llama.cpp/models/Meta-Llama-3.1-8B-Instruct-Q8_0.gguf"
let N_TOKENS = 64

// matmul FLOPs per generated token: per layer Q/K/V/O + SwiGLU w1/w2/w3, plus the classifier.
def flops_per_token(c : Config) : int64 {
    let per_layer = 4l * c.dim * c.dim + 4l * c.dim * c.kv_dim + 6l * c.dim * c.hidden_dim
    return c.n_layers * per_layer + 2l * c.vocab_size * c.dim
}

def argmax_logits(s : Session; vocab : int64) : int64 {
    var best = 0l
    var bv = s.logits[0]
    for (i in range64(vocab)) {
        if (s.logits[i] > bv) {
            bv = s.logits[i]
            best = i
        }
    }
    return best
}

[export]
def main {
    if (!jit_enabled()) {
        print("This benchmark is JIT-only — run with: daslang -jit <file>\n")
        return
    }
    var path = DEFAULT_MODEL
    let args = get_command_line_arguments()
    if (!empty(args) && ends_with(args[length(args) - 1], ".gguf")) {
        path = args[length(args) - 1]
    }
    // backend pin for A/B on one binary (e.g. portable row-major vs x64-gen) — must land
    // BEFORE load_gguf: select_matmul_backend_for_load honors the pin at load, which decides repack
    if (has_env_variable("DASLLAMA_PIN_BACKEND")) {
        pin_kernel_backend(get_env_variable("DASLLAMA_PIN_BACKEND"))
    }
    if (has_env_variable("DASLLAMA_PIN_BATCH_BACKEND")) {
        select_batch_backend(get_env_variable("DASLLAMA_PIN_BATCH_BACKEND"))
    }
    print("loading {path}\n")
    let t <- load_gguf(path, QuantMode.q8)
    let c = t.config
    print("config: dim={c.dim} hidden={c.hidden_dim} layers={c.n_layers} heads={c.n_heads}/{c.n_kv_heads} vocab={c.vocab_size}\n")
    print("active backend: {active_kernel_backend()}\n")

    var s <- make_run_state(c)
    // synthetic seed ids — forward speed is content-independent (see header), so keep them small
    // enough to index any real model vocab (ids 1-5; the smallest we bench is Phi-3.5 at 32k),
    // matching prefill_perf's shape-agnostic ids; the generated continuation is argmax over the
    // full vocab and is always a valid id
    let prompt = [1l, 2l, 3l, 4l, 5l]
    with_job_que() {
        setup_dasllama_jobque_()
        // spin-window override for tuning A/Bs (0 disables the spin)
        if (has_env_variable("DAS_WORKER_SPIN_US")) {
            let spin_us = to_int(get_env_variable("DAS_WORKER_SPIN_US"))
            set_jobque_worker_spin(spin_us)
            print("worker spin override: {spin_us}us\n")
        }
        // join poll-level override (ggml denomination 0..100; setup default 50, 0 = park at join)
        if (has_env_variable("DAS_JOBQUE_JOIN_SPIN")) {
            let join_spin = to_int(get_env_variable("DAS_JOBQUE_JOIN_SPIN"))
            set_jobque_join_spin(join_spin)
            print("join spin override: level {join_spin}\n")
        }
        // A/B the matmul min-grains (rows per chunk floor); 0 = clamp off
        if (has_env_variable("DASLLAMA_MIN_CHUNK_ROWS")) {
            let mg = to_int(get_env_variable("DASLLAMA_MIN_CHUNK_ROWS"))
            set_matmul_min_chunk_rows(mg)
            print("min chunk rows override: {mg}\n")
        }
        if (has_env_variable("DASLLAMA_GEMV_WAVE_FILL")) {
            set_gemv_wave_fill(get_env_variable("DASLLAMA_GEMV_WAVE_FILL") != "0")
        }
        if (has_env_variable("DASLLAMA_MIN_CHUNK_ROWS_GEMV")) {
            let mgv = to_int(get_env_variable("DASLLAMA_MIN_CHUNK_ROWS_GEMV"))
            set_matmul_min_chunk_rows_gemv(mgv)
            print("gemv min chunk rows override: {mgv}\n")
        }
        // A/B the fused per-block decode chains (default on; 0 = per-op dispatches)
        if (has_env_variable("DASLLAMA_FUSED_DECODE")) {
            set_fused_decode(get_env_variable("DASLLAMA_FUSED_DECODE") != "0")
            print("fused decode override: {get_fused_decode()}\n")
        }
        // work-target override — also the crude chain selector (attn chain gates at dim*(qd+2kv),
        // ffn at dim*hidden; the fuse needs lanes_for_work(gate work) > 1, i.e. work >= 2*target)
        if (has_env_variable("DASLLAMA_TARGET_CHUNK_WORK")) {
            let tcw = to_int(get_env_variable("DASLLAMA_TARGET_CHUNK_WORK"))
            set_target_chunk_work(int64(tcw))
            print("target chunk work override: {tcw}\n")
        }
        if (has_env_variable("DASLLAMA_GEMV_LANE_CAP")) {
            let glc = to_int(get_env_variable("DASLLAMA_GEMV_LANE_CAP"))
            set_gemv_lane_cap(glc)
            print("gemv lane cap override: {glc}\n")
        }
        if (has_env_variable("DASLLAMA_BATCH_LANE_CAP")) {
            let blc = to_int(get_env_variable("DASLLAMA_BATCH_LANE_CAP"))
            set_batch_lane_cap(blc)
            print("batch lane cap override: {blc}\n")
        }
        if (has_env_variable("DASLLAMA_BATCH_GRID_2D")) {
            let bg2 = to_int(get_env_variable("DASLLAMA_BATCH_GRID_2D"))
            set_batch_grid_2d(bg2)
            print("batch grid 2d override: {bg2}\n")
        }
        if (has_env_variable("DASLLAMA_WORKER_LIMIT")) {
            let wl = to_int(get_env_variable("DASLLAMA_WORKER_LIMIT"))
            set_jobque_worker_limit(wl)   // direct: we are inside the que already; 0 = caller-only, negative = unlimited (jobque semantics)
            print("worker limit override: {wl}\n")
        }
        // prefill (untimed)
        var pos = 0l
        for (tk in prompt) {
            forward(t, s, tk, pos)
            pos++
        }
        var token = argmax_logits(s, c.vocab_size)
        // timed generation; per-section decode profile over exactly this window
        forward_profile_reset()
        let t0 = ref_time_ticks()
        for (_i in range(N_TOKENS)) {
            forward(t, s, token, pos)
            pos++
            token = argmax_logits(s, c.vocab_size)
        }
        let us = get_time_usec(t0)
        let tps = double(N_TOKENS) / (double(us) * 1.0e-6lf)
        let gf = double(int64(N_TOKENS) * flops_per_token(c)) / (double(us) * 1000.0lf)
        print("gen Q8: {N_TOKENS} tok in {us} us -> {tps} tok/s ({gf} GFLOP/s)\n")
        forward_profile_report()   // the decode per-section table (set against llama.cpp's GGML_OP_PROFILE dump)
        // In-process fused-decode A/B: DASLLAMA_FUSED_AB=<pairs> alternates fused/per-op windows
        // of N_TOKENS in ONE process (the campaign's interleaved-cells method — box drift between
        // separate processes swamps a ~10% delta). Window order flips per pair (ABBA), so the
        // KV-depth growth across the run cancels out of the paired deltas.
        if (has_env_variable("DASLLAMA_FUSED_AB")) {
            let pairs = to_int(get_env_variable("DASLLAMA_FUSED_AB"))
            var sum_on = 0.0lf
            var sum_off = 0.0lf
            for (p in range(pairs)) {
                let first_on = (p % 2) == 0
                for (w in range(2)) {
                    let on = (w == 0) == first_on
                    set_fused_decode(on)
                    let w0 = ref_time_ticks()
                    for (_i in range(N_TOKENS)) {
                        forward(t, s, token, pos)
                        pos++
                        token = argmax_logits(s, c.vocab_size)
                    }
                    let wus = get_time_usec(w0)
                    let wtps = double(N_TOKENS) / (double(wus) * 1.0e-6lf)
                    print("fused_ab pair {p} {on ? "ON " : "OFF"}: {wtps} tok/s\n")
                    if (on) { sum_on += wtps; } else { sum_off += wtps; }
                }
            }
            let mon = sum_on / double(pairs)
            let moff = sum_off / double(pairs)
            print("fused_ab mean: ON {mon} vs OFF {moff} tok/s ({100.0lf * (mon - moff) / moff}%)\n")
        }
    }
}
