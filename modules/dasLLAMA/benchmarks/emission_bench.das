options gen2
options _jit_fast_math = true   // ggml-parity FP laxity — matches how llama.cpp is benched
options persistent_heap = true
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require daslib/jobque_boost
require daslib/clargs
require daslib/fio   // has_env_variable / get_env_variable (the DASLLAMA_FUSED_DECODE A/B override)
require math

// EMISSION baseline — the realistic CPU-inference shape: many independent SHORT prompts, each a
// fresh session (the agent/assistant workload), measuring what a user feels per request:
//   ttft     = prefill + the first generated token (ms)
//   emit t/s = the steady generation rate over the remaining tokens
// Reported as mean/min/max across prompts (fresh KV each), greedy by default (-topk for the
// sampled variant — sampling cost is part of the emission bill at small vocab-heavy models).
//   bin/daslang -jit modules/dasLLAMA/benchmarks/emission_bench.das -- -m <model.gguf>
// Anchor: llama-bench -m <model.gguf> -ngl 0 -p <plen> -n <ngen> (pp = ttft side, tg = emit side).

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string

    @clarg_doc = "Independent short prompts, fresh session each (default: 8)"
    nprompts : int = 8

    @clarg_short = "p"
    @clarg_doc = "Prompt length, tokens (default: 32)"
    plen : int = 32

    @clarg_short = "n"
    @clarg_doc = "Tokens emitted per prompt (default: 64)"
    ngen : int = 64

    @clarg_short = "q"
    @clarg_doc = "Weight quant mode (default: q8)"
    quant : QuantMode = QuantMode.q8

    @clarg_doc = "top-k sampling instead of greedy (0 = greedy; e.g. 40)"
    topk : int = 0

    @clarg_doc = "Team dispatch: workers self-serve chunks off one published atomic instead of per-chunk fifo jobs"
    team : bool

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// deterministic synthetic prompt ids (vocab-safe low range; per-prompt seed varies the content —
// the same convention as decode_prof, so runs are comparable across models and time)
def private build_prompt(n : int64; seed : int64) : array<int64> {
    return <- [for (i in range64(n)); 1000l + ((i * 17l + seed * 131l) % 5000l)]
}

def private greedy_top(s : Session; vocab : int64) : int64 {
    return parallel_argmax(s.logits, vocab)   // team-scanned; bit-identical to the serial loop
}

[export]
def main() {
    if (!jit_enabled()) {
        print("This benchmark is JIT-only — run with: daslang -jit <file>\n")
        return
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "emission_bench")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "emission_bench")
        return
    }
    let np = max(cfg.nprompts, 1)
    let plen = max(int64(cfg.plen), 1l)
    let ngen = max(int64(cfg.ngen), 2l)
    apply_box_profile_runtime()   // the tuned per-box stack (backend/batch pins, grains) — what load_model users get
    if (has_env_variable("DASLLAMA_PIN_BACKEND")) {   // A/B rail, same as prefill_perf/llama3_perf: pin BEFORE load
        pin_kernel_backend(get_env_variable("DASLLAMA_PIN_BACKEND"))
        print("backend pin: {get_env_variable("DASLLAMA_PIN_BACKEND")}\n")
    }
    print("loading {cfg.model}\n")
    var t <- load_gguf(cfg.model, cfg.quant)
    print("backend: {active_kernel_backend()} (batch: {active_batch_backend()})\n")
    t.config.seq_len = min(t.config.seq_len, plen + ngen + 8l)   // cap the KV allocation
    let c = t.config
    let mode = cfg.topk > 0 ? "top-k {cfg.topk}" : "greedy"
    print("config: dim={c.dim} layers={c.n_layers} vocab={c.vocab_size} moe={c.moe} | {np} prompts x {plen} tok, emit {ngen}, {mode}\n")
    var params = SamplingParams()
    if (cfg.topk > 0) {
        params.top_k = int64(cfg.topk)
        params.temp = 0.8
    }
    with_job_que() {
        setup_dasllama_jobque_()
        if (cfg.team) {
            set_jobque_team_mode(true)
            print("team dispatch: on\n")
        }
        // A/B the fused per-block decode chains (default on; 0 = per-op dispatches)
        if (has_env_variable("DASLLAMA_FUSED_DECODE")) {
            set_fused_decode(get_env_variable("DASLLAMA_FUSED_DECODE") != "0")
            print("fused decode override: {get_fused_decode()}\n")
        }
        // A/B the 2-D batch chunk grid over the profile's pin (0 = 1-D, 1 = fine, 2 = aligned)
        if (has_env_variable("DASLLAMA_BATCH_GRID_2D")) {
            set_batch_grid_2d(to_int(get_env_variable("DASLLAMA_BATCH_GRID_2D")))
            print("batch grid 2d override: {get_batch_grid_2d()}\n")
        }
        // one throwaway warmup prompt: JIT compilation + first-touch paging stay out of prompt 0
        {
            var s <- make_run_state(c)
            let warm <- build_prompt(plen, 999l)
            eval_(t, s, warm)
            forward(t, s, warm[plen - 1l], plen - 1l)
            delete s
        }
        var ttft_ms : array<double>
        var emit_tps : array<double>
        for (pi in range(np)) {
            var s <- make_run_state(c)
            let prompt <- build_prompt(plen, int64(pi))
            let t0 = ref_time_ticks()
            eval_(t, s, prompt)
            forward(t, s, prompt[plen - 1l], plen - 1l)
            var tok = cfg.topk > 0 ? sample_(s, params) : greedy_top(s, c.vocab_size)
            let ttft_us = get_time_usec(t0)
            let t1 = ref_time_ticks()
            for (i in range64(ngen - 1l)) {
                forward(t, s, tok, plen + i)
                tok = cfg.topk > 0 ? sample_(s, params) : greedy_top(s, c.vocab_size)
            }
            let emit_us = get_time_usec(t1)
            ttft_ms |> push(double(ttft_us) / 1000.0lf)
            emit_tps |> push(double(ngen - 1l) * 1.0e6lf / double(emit_us))
            delete s
        }
        var tmin = ttft_ms[0]
        var tmax = ttft_ms[0]
        var tsum = 0.0lf
        var emin = emit_tps[0]
        var emax = emit_tps[0]
        var esum = 0.0lf
        for (i in range(np)) {
            tmin = min(tmin, ttft_ms[i])
            tmax = max(tmax, ttft_ms[i])
            tsum += ttft_ms[i]
            emin = min(emin, emit_tps[i])
            emax = max(emax, emit_tps[i])
            esum += emit_tps[i]
        }
        print("ttft   ms : mean {tsum / double(np)}  min {tmin}  max {tmax}\n")
        print("emit  t/s : mean {esum / double(np)}  min {emin}  max {emax}\n")
    }
    delete t
}
