options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _jit_fast_math = true   // ggml-parity FP laxity — matches how llama.cpp is benched
options _dasllama_internal = true

require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_math   // setup_dasllama_jobque_ + apply_box_profile_runtime (engine-level, not re-exported)
require dasllama/dasllama_image   // load_model_cached — the .dlim rail, without the full facade's compile weight
require ?das_accelerate dasllama/dasllama_math_accelerate   // the --accel leg; Apple-only C++ module
require ../performance/profile_common.das   // profile_threads + affinity_on — the standing per-box methodology
require daslib/jobque_boost
require daslib/jobque_profile   // JOBQUE_PROFILING_ENABLED — the tool refuses on a build without the marker rail (its bucket report needs it)
require daslib/clargs
require math
require dasllama/dasllama_env

// Per-bucket decode profile — the position-0 detector for decode-side chases: one measurement
// window of -n single-token forwards at depth --depth, the engine's permanent prof_add buckets
// reset after prefill+warmup so the report covers EXACTLY the timed window. The window mirrors
// lcpp_bench's tg row (same forward calls, same synth ids), so the printed pace is directly
// comparable to a tg cell; diff the buckets against a dasprof-patched llama-bench
// (harness/ggml_op_profile.patch, -p 0 -n <n> -r 1 --no-warmup) with its per-node tax corrected.

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string

    @clarg_short = "n"
    @clarg_doc = "Decode tokens in the measurement window (default: 128, the tg128 shape)"
    ngen : int = 128

    @clarg_doc = "Batched prefill depth before the window — the window decodes at positions depth..depth+n (default: 8; 0 = decode from an empty cache like llama-bench tg)"
    depth : int = 8

    @clarg_short = "t"
    @clarg_doc = "Worker threads (default: the box profile; DAS_JOBQUE_THREADS still wins)"
    threads : int = 0

    @clarg_short = "q"
    @clarg_doc = "Weight quant mode (default: q8)"
    quant : QuantMode = QuantMode.q8

    @clarg_doc = "Arm the accelerate (+AMX/SME) float-batch tier, the lcpp_bench --accel mirror — profiles the 'accel' flavor's decode"
    accel : bool

    @clarg_name = "no-fused"
    @clarg_doc = "Disable the fused decode chains (set_fused_decode(false)) — the chain-vs-per-op A/B lever"
    no_fused : bool

    @clarg_doc = "Save a jobque lane trace of the measurement window to this path (perfetto JSON; adds ring overhead — don't trust the pace of a traced run)"
    trace : string

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// deterministic synthetic ids (vocab-safe low range, shared convention with lcpp_bench)
def private synth_id(i, seed : int64) : int64 => 1000l + ((i * 17l + seed * 131l) % 5000l)

[export]
def main() : int {   // nolint:STYLE038 — flat profiler driver: arg/env/tier handling, then the one measurement window
    if (!jit_enabled()) {
        print("This profiler is JIT-only - run with: daslang -jit <file>\n")
        return 1
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "decode_prof")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "decode_prof")
        return 0
    }
    allow_cpu_prefill()   // CPU-protocol profile by intent — disarm the Metal-build tripwire
    let ngen = max(int64(cfg.ngen), 1l)
    let depth = max(int64(cfg.depth), 0l)
    let t_env = g_env_core.jobque_threads
    var threads = cfg.threads > 0 ? cfg.threads : (t_env ?? profile_threads())
    if (is_some(t_env) && unwrap(t_env) != threads) {
        threads = unwrap(t_env)
        print("warning: DAS_JOBQUE_THREADS overrides -t - running {threads} lanes\n")
    } elif (is_none(t_env)) {
        set_jobque_threads_cap(max(threads - 1, 1))
    }
    let pinned = affinity_on() && is_none(g_env_core.jobque_affinity)
    if (pinned) {
        set_jobque_affinity(2)   // 2 = hard mask (QoS classification on darwin), the llama-bench --cpu-strict mirror
    }
    apply_box_profile_runtime()   // the tuned per-box stack — what load_model users get
    if (cfg.no_fused) {
        set_fused_decode(false)
    }
    if (cfg.accel) {
        static_if (typeinfo builtin_module_exists(das_accelerate)) {
            dasllama_accelerate_enable(32l)   // the shipped +AMX defaults: min ntok 32, work floor 512 MMAC
        } else {
            print("--accel requested but das_accelerate is not built on this platform - no accel tier to profile\n")
            return 1
        }
    }
    if (!JOBQUE_PROFILING_ENABLED) {
        print("decode_prof needs the marker rail compiled in - rerun with JOBQUE_PROFILING=1 (or --jobque-profiling)\n")
        return 1
    }
    print("loading {cfg.model}\n")
    with_job_que() {
        setup_dasllama_jobque_()
        var t <- load_model_cached(cfg.model, cfg.quant)
        t.config.seq_len = min(t.config.seq_len, depth + ngen + 8l)   // cap the KV allocation
        let c = t.config
        print("config: dim={c.dim} layers={c.n_layers} vocab={c.vocab_size} | backend {active_kernel_backend()} (batch {active_batch_backend()}) | t{threads} affinity {pinned ? "hard" : "env/off"} | fused {get_fused_decode()} | depth {depth} window {ngen}\n")
        var s <- make_run_state(c)
        if (depth > 0l) {
            let prompt <- [for (i in range64(depth)); synth_id(i, 999l)]
            forward_prefill(t, s, prompt, depth, 0l)
        }
        forward(t, s, synth_id(0l, 999l), depth)   // warmup token — untimed, outside the window
        if (!empty(cfg.trace)) {
            jobque_trace_start(1 << 21)   // decode makes ~30k chunk events/token — size for a ~32-token window
            set_trace_tags(true)
        }
        forward_profile_reset()
        let t0 = ref_time_ticks()
        for (i in range64(ngen)) {
            forward(t, s, synth_id(i, 1l), depth + 1l + i)
        }
        let us = get_time_usec(t0)
        if (!empty(cfg.trace)) {
            set_trace_tags(false)
            jobque_trace_stop()
            print(jobque_trace_save(cfg.trace) ? "jobque trace saved: {cfg.trace}\n" : "jobque trace: nothing recorded\n")
        }
        forward_profile_report()
        let pace = double(ngen) * 1.0e6lf / double(us)
        print("WINDOW depth={depth + 1l} n={ngen} wall_us={us} us_per_token={int64(us) / ngen} pace={pace:.2f} tok/s\n")
        delete s
        delete t
    }
    return 0
}
