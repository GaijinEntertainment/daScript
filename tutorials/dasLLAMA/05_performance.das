options gen2
options persistent_heap        // models are loaded and freed per section (tutorial 04)
options _jit_fast_math = true  // relaxed FP for the JIT'd kernels (non-bit-exact, ~+10%) — the parity tests stay bit-exact

require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require strings
require math

// Tutorial dasLLAMA-05: Performance
//
// This tutorial covers:
//   - Why model programs always run with -jit (and _jit_fast_math)
//   - Threading the kernels: with_job_que + set_jobque_fork_pool
//   - Quantization: fp32 vs q8 vs q4 — memory and speed
//   - Reading stats(): prefill vs generation, time to first token
//
// Prerequisites: tutorial 01.
//
// Run: daslang.exe -jit tutorials/dasLLAMA/05_performance.das -- <path/to/model.gguf>

// A prompt long enough that prefill throughput means something.
let PROMPT = ("The history of computing begins long before electronics. Mechanical aids to " +
    "calculation appeared in antiquity, and by the nineteenth century engineers were designing " +
    "programmable machines. The twentieth century turned those designs into electronic reality.")
let N_GEN = 32l
let CTX = 1024l

// Resolve the model path: last command-line argument ending in .gguf, else $DASLLAMA_MODEL.
def model_path() : string {
    let cmd <- get_command_line_arguments()
    if (!empty(cmd) && ends_with(cmd[length(cmd) - 1], ".gguf")) {
        return cmd[length(cmd) - 1]
    }
    return get_env_variable("DASLLAMA_MODEL")
}

def mb(bytes : int64) : int {
    return int(bytes / 1000000l)
}

// Generate N_GEN tokens greedily and report the session's stats. Two numbers matter:
// prefill t/s (the prompt runs as ONE batched forward — compute-bound, fast) and gen t/s
// (one token per forward — memory-bandwidth-bound, slower). ttft = prefill + first forward.
def bench_gen(m : Model; tag : string) {
    var s = create_session(m)
    let prompt <- encode(m, PROMPT)
    generate(m, s, prompt, SamplingParams(), N_GEN) $(_id, _piece) => true
    let st = stats(s)
    print("{tag}prefill {int(st.prefill_tps)} t/s | gen {int(st.gen_tps)} t/s | ttft {int(st.ttft_s * 1000.0lf)}ms\n")
    delete s
}

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Threads: the job queue
// ──────────────────────────────────────────────────────────────────────────
//
// The matmul kernels split their rows across the job queue — they REQUIRE one
// (running model code outside with_job_que panics). set_jobque_fork_pool(true, true)
// pools the per-job fork contexts: the kernels are pure data-parallel jobs, so their
// contexts can be reused and skip re-init.
//
// The worker count is fixed when the queue is created — the DAS_JOBQUE_THREADS
// environment variable overrides the default (which is deliberately conservative).
// To see what threading buys, re-run this tutorial with DAS_JOBQUE_THREADS=1 and
// compare; the win grows with model size, since tiny models have tiny matmuls.

def section_threads(m : Model) {
    with_job_que() {
        set_jobque_fork_pool(true, true)
        print("job queue: {get_total_hw_jobs()} hardware jobs (override with DAS_JOBQUE_THREADS=N)\n")
        bench_gen(m, "threaded: ")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Quantization: memory vs fidelity
// ──────────────────────────────────────────────────────────────────────────
//
// QuantMode picks the in-memory weight representation, whatever the GGUF stores:
//   fp32 — the token-exact reference (4 bytes/weight); what the test suite validates
//   q8   — int8 blocks (~1 byte/weight), the fast everyday choice
//   q4   — 4-bit blocks, half of q8 again — but currently served by a scalar kernel
//          with no batched prefill, so it's the smallest footprint, not the fastest
// Generation is bandwidth-bound, so between fp32 and q8, smaller is also faster.

def section_quant(path : string; mode : QuantMode; tag : string) {
    var m <- load_model(path, mode)
    m.config.seq_len = min(m.config.seq_len, CTX)
    print("{tag}: {mb(int64(heap_bytes_allocated()))} MB resident | ")
    with_job_que() {
        set_jobque_fork_pool(true, true)
        bench_gen(m, "")
    }
    delete m
}

[export]
def main() {
    let path = model_path()
    if (empty(path)) {
        print("usage: daslang -jit tutorials/dasLLAMA/05_performance.das -- <model.gguf>\n")
        return
    }

    // ──────────────────────────────────────────────────────────────────────
    // Section 0 — Always -jit
    // ──────────────────────────────────────────────────────────────────────
    //
    // The kernels are daslang code; the LLVM JIT compiles them to native SIMD.
    // Interpreted inference is orders of magnitude slower — never benchmark it.
    // `options _jit_fast_math` (top of file) additionally allows the JIT to relax
    // FP ordering in these kernels, matching how llama.cpp compiles its own.

    if (!jit_enabled()) {
        print("NOTE: running interpreted — re-run with -jit; every number below will be far too slow\n")
    }

    var m <- load_model(path, QuantMode.q8)
    m.config.seq_len = min(m.config.seq_len, CTX)
    section_threads(m)
    delete m

    print("\n")
    section_quant(path, QuantMode.fp32, "fp32")
    section_quant(path, QuantMode.q8, "q8  ")
    section_quant(path, QuantMode.q4, "q4  ")

    // Where to go deeper: modules/dasLLAMA/tune_for_this_box.md covers kernel tuning
    // (token-block size, unrolls, the [dasllama_grid] tuner) when you want to squeeze
    // a specific machine.
}
