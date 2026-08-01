options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _jit_fast_math = true   // production prefill config (prefill_perf parity)

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_math  // setup_dasllama_jobque_ + apply_box_profile_runtime
require llvm/daslib/llvm_tune   // [tune_policy]: the confirm arm measures WHAT THE MANIFEST SAYS — never self-tune
require daslib/jobque_boost
require strings
require math

// The tune e2e confirm child — gen_tune_probe's tune-mode confirm pass spawns this twice
// (DAS_TUNE_MANIFEST = the challenger manifest vs empty = the per-ISA fallback) so a
// microbench winner has to prove itself in the production regime (dispatch, lanes, DRAM,
// box_profile knobs) before it may displace the fallback in the manifest. One line of
// output matters: `CONFIRM_PP: <tok/s>` (best of PP_REPS batched prefills of PP_TOKENS
// synthetic ids). The `llvm_tune: q8q8_tile_gen <- ...` stamp line this child prints at
// compile is the parent's consistency check that the arm stamped what it intended.
//   usage: daslang -jit tune_confirm_prefill.das -- <model-q8.gguf>

let PP_TOKENS = 512l
let PP_REPS = 3

// fallback: this child measures the manifest handed to it (or the fallback stamps when the
// arm passes an empty manifest) — an incomplete sidecar must NOT recurse into a nested tune
// inside a confirm arm (each arm burned a full hidden tune before this)
[export, tune_policy(missing = "fallback")]
def main {
    allow_cpu_prefill()   // CPU prefill tuning by definition — disarm the Metal-build tripwire
    if (!jit_enabled()) {
        print("CONFIRM_PP: FAIL (run with -jit)\n")
        return
    }
    var model = ""
    let args = get_command_line_arguments()
    if (!empty(args) && ends_with(args[length(args) - 1], ".gguf")) {
        model = args[length(args) - 1]
    }
    if (empty(model)) {
        print("CONFIRM_PP: FAIL (usage: tune_confirm_prefill.das -- <model-q8.gguf>)\n")
        return
    }
    apply_box_profile_runtime()   // the box's tuned runtime stack — confirm in the regime users run
    var t <- load_gguf(model, QuantMode.q8)
    t.config.seq_len = min(t.config.seq_len, PP_TOKENS + 16l)   // cap the KV allocation
    let c = t.config
    var s = make_run_state(c)
    let prompt <- [for (i in range64(PP_TOKENS)); 1000l + (i % 5000l)]
    with_job_que() {
        setup_dasllama_jobque_()
        forward_prefill(t, s, prompt, 8l, 0l)   // warmup: JIT + first-touch paging
        var best = 0.0lf
        for (_r in range(PP_REPS)) {
            let t0 = ref_time_ticks()
            forward_prefill(t, s, prompt, PP_TOKENS, 0l)
            let us = get_time_usec(t0)
            let tps = double(PP_TOKENS) / (double(us) * 1.0e-6lf)
            if (tps > best) {
                best = tps
            }
        }
        print("CONFIRM_PP: {best}\n")
    }
}
