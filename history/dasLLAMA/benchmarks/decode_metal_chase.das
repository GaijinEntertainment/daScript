options gen2
options stack = 65536   // dasLLAMA program-root convention (options stack does not unify up from libs)
options _jit_fast_math = true   // ggml-parity FP laxity — the CPU prefill/sampling half only

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_math   // setup_dasllama_jobque_ + pin_kernel_backend + parallel_argmax
require ?das_metal dasllama/dasllama_metal_llama   // nolint:STYLE030 — GPU driver half, Apple builds only
require daslib/jobque_boost
require daslib/clargs
require daslib/fio   // nolint:STYLE030 — used only inside the Apple static_if half
require math   // nolint:STYLE030 — used only inside the Apple static_if half
require daslib/strings_boost   // nolint:STYLE030 — used only inside the Apple static_if half

// The Metal DECODE chase harness — the emission arc's P3 attribution + A/B rail. Runs the same
// greedy decode window repeatedly with the resident GPU driver switched between knockout arms
// (interleaved across reps, so box noise hits every arm equally) and prints the driver's per-stage
// split next to the wall t/s:
//   full      the production path (GPU logits) — the headline arm
//   nologits  RETIRED: blob models force GPU logits (no CPU classifier planes) and every chase
//             model is blob-form — the arm is skipped loudly if requested
//   noattn    DASLLAMA_METAL_DECODE_SKIP=attn semantics: nologits minus single-query attention
//   noew      minus rms/rope/kv-store/add/swiglu (the elementwise set)
//   nogemv    minus every layer GEMV — attention + elementwise only
//   nomoert   minus the ROUTED expert GEMVs only (router/select/combine + dense-shared stay live)
//   nomoesh   minus the dense-shared branch GEMVs only (g4 shared FFN / qwen35moe shexp)
//   noqkv     minus the attention input GEMVs only (wq/wk/wv or the fused QKV dispatch)
//   nowo      minus the attention output projection GEMV only
//   nofuse    the R1 epilogue-fusion kernel OFF (separate norm/add dispatches) — concurrent +
//             graph stay ON, so full-vs-nofuse is the clean fused-vs-unfused A/B (bar/step + t/s)
// Knockout arms compute GARBAGE tokens by design; timing is shape-bound, not data-bound. Each
// window is preceded by one unclocked step that absorbs the mirror's rewind re-upload (windows
// restart at the same depth, so pos < watermark fires every time) and the first-touch region
// uploads.
//
//   DASLLAMA_PIN_BACKEND=portable bin/daslang -jit modules/dasLLAMA/benchmarks/decode_metal_chase.das -- \
//       -m <model.gguf> [-p 8] [--prefill2 512] [-n 32] [-r 3] [--arms full,noattn]
//
// (the backend defaults to `portable` here even without the env pin — the decode gate needs
// row-major weights + the f32 scale plane, both pinned before load)

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string

    @clarg_short = "p"
    @clarg_doc = "First (shallow) window start depth (default: 8)"
    prefill : int = 8

    @clarg_doc = "Second, deeper window start depth in the same process — 0 to skip (default: 512)"
    prefill2 : int = 512

    @clarg_short = "n"
    @clarg_doc = "Timed decode tokens per window (default: 32)"
    ngen : int = 32

    @clarg_short = "r"
    @clarg_doc = "Interleaved repetitions of the arm set (default: 3)"
    reps : int = 3

    @clarg_doc = "Comma list of arms to run (default: full,noattn,noew,nogemv)"
    arms : string = "full,noattn,noew,nogemv"

    @clarg_doc = "KV cache dtype (default: f16 — the emission arc's batch default)"
    kv : KVDtype = KVDtype.f16

    @clarg_doc = "Weight-scale plane A/B: 1 = f16 halfwords (llama.cpp byte parity, less stream), 0 = f32, -1 = keep default (f16)"
    wscale_f16 : int = -1

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// best window of an arm at one depth, per-step figures in us
struct private ArmBest {
    tps : double
    setup : double
    encode : double
    wait : double
    gpu : double
    rb : double
    tail : double   // wall - driver stages: CPU sampling + forward() overhead
}

def private build_prompt(n : int64) : array<int64> {
    return <- [for (i in range64(n)); 1000l + (i % 5000l)]
}

def private greedy_top(s : Session; vocab : int64) : int64 {
    return parallel_argmax(s.logits, vocab)
}

def private apply_arm(arm) {   // untyped param = generic — non-Apple builds never compile the Metal-typed body
    set_metal_decode_skip(arm == "noattn" ? "attn" : (arm == "noew" ? "ew" : (arm == "nogemv" ? "gemv" :
        (arm == "nomoert" ? "moe_rt" : (arm == "nomoesh" ? "moe_sh" : (arm == "noqkv" ? "qkv" :
        (arm == "nowo" ? "wo" : "")))))))
    set_metal_decode_logits_cpu(arm == "nologits")
    set_metal_decode_fuse(arm == "nofuse" ? 0 : -1)   // A/B the R1 epilogue-fusion kernel (concurrent+graph stay on)
}

// one timed window: unclocked pre-step at `depth` (absorbs the rewind re-upload), then n greedy
// steps at depth+1..depth+n with the driver's stage timers windowed to exactly this loop
def private run_window(t; var s : Session; depth, n : int64; tok0 : int64; arm : string; var best : ArmBest&) : int64 {
    apply_arm(arm)
    forward(t, s, tok0, depth)
    var tok = greedy_top(s, t.config.vocab_size)
    metal_decode_stage_reset()
    let d0 = metal_dispatch_calls()
    let h0 = metal_hazard_stats()
    let t0 = ref_time_ticks()
    for (i in range64(n)) {
        forward(t, s, tok, depth + 1l + i)
        tok = greedy_top(s, t.config.vocab_size)
    }
    let us = get_time_usec(t0)
    let disp = metal_dispatch_calls() - d0
    let hz = metal_hazard_stats()
    let bar = hz.barriers - h0.barriers
    apply_arm("full")   // restore before anything else runs
    let st = metal_decode_stage_stats()
    let tps = double(n) / (double(us) * 1.0e-6lf)
    let served = st.steps
    let fn = float(max(served, 1l))
    var w = ArmBest(tps = tps,
        setup = double(float(st.setup_us) / fn), encode = double(float(st.encode_us) / fn),
        wait = double(float(st.wait_us) / fn), gpu = st.gpu_us / double(max(served, 1l)),
        rb = double(float(st.readback_us) / fn))
    w.tail = double(us) / double(n) - w.setup - w.encode - w.wait - w.rb
    let miss = served != n ? "  ⚠ served {served}/{n}" : ""
    print("  {arm} @{depth}: {n} tok in {us}us = {tps} t/s | per-step setup {w.setup} encode {w.encode} wait {w.wait} (gpu {w.gpu}) rb {w.rb} tail {w.tail} us  disp {disp / n}/step bar {bar / n}/step{miss}\n")
    if (w.tps > best.tps) {
        best = w
    }
    return tok
}

def private chase_depth(t; var s : Session; arms : array<string>; depth, n : int64; reps : int; tok0 : int64) : int64 {
    print("=== depth {depth}: {reps} reps x {length(arms)} arms, {n} timed tokens/window ===\n")
    var best : table<string; ArmBest>
    var tok = tok0
    for (rep in range(reps)) {
        print(" rep {rep}\n")
        for (arm in arms) {
            tok = run_window(t, s, depth, n, tok, arm, best[arm])
        }
    }
    print("--- best per arm @{depth} (per-step us) ---\n")
    for (arm in arms) {
        let b = best[arm]
        print("  {arm}: {b.tps} t/s | setup {b.setup} encode {b.encode} wait {b.wait} (gpu {b.gpu}) rb {b.rb} tail {b.tail}\n")
    }
    return tok
}

def private run_chase(cfg) {   // untyped param = generic — non-Apple builds never compile the Metal-typed body
    // both knobs must land BEFORE load_gguf: the pin decides repack at load, the scale-plane
    // flag decides the load-time f16 convert (the driver carries GEMV twins for both planes)
    if (!has_env_variable("DASLLAMA_PIN_BACKEND")) {
        pin_kernel_backend("portable")
    } else {
        pin_kernel_backend(get_env_variable("DASLLAMA_PIN_BACKEND"))
    }
    if (cfg.wscale_f16 >= 0) {
        set_wscale_f16(cfg.wscale_f16 != 0)
        print("wscale_f16 override: {cfg.wscale_f16 != 0}\n")
    }
    if (!select_decode_override("metal")) {
        print("metal decode override not registered on this build\n")
        return
    }
    if (select_prefill_override("metal")) {   // fast deep-window fill; declines fall back to CPU
        print("prefill override: metal\n")
    }
    // blob models FORCE GPU logits (no CPU classifier planes), and the blob transform below makes
    // every chase model blob-form — a `nologits` arm would silently measure `full`. Drop it loudly.
    var arms : array<string>
    for (a in split(cfg.arms, ",")) {
        if (a == "nologits") {
            print("nologits arm skipped: blob models force GPU logits — the arm would measure `full`\n")
        } else {
            arms |> push(a)
        }
    }
    let p = max(int64(cfg.prefill), 8l)
    let p2 = int64(cfg.prefill2)
    let n = max(int64(cfg.ngen), 1l)
    print("loading {cfg.model}\n")
    var t <- load_gguf(cfg.model, QuantMode.q8)
    if (!convert_model_to_metal_blob(t)) {   // the drivers are blob-only
        print("metal-blob transform declined — the drivers cannot serve this load\n")
        delete t
        return
    }
    let ctx_cap = max(2048l, max(p, p2) + n + 16l)
    t.config.seq_len = min(t.config.seq_len, ctx_cap)   // cap the KV allocation
    let c = t.config
    print("config: dim={c.dim} layers={c.n_layers} heads={c.n_heads}/{c.n_kv_heads} vocab={c.vocab_size}\n")
    print("active backend: {active_kernel_backend()}  kv: {cfg.kv}\n")
    var s <- make_run_state(c, cfg.kv, cfg.kv)
    with_job_que() {
        setup_dasllama_jobque_()
        // warmup: JIT both paths + first-touch the resident weight regions (3.4GB of memcpys)
        let warm <- build_prompt(8l)
        for (wp in range64(8l)) {
            forward(t, s, warm[wp], wp)
        }
        forward_prefill(t, s, warm, 8l, 0l)
        let prompt <- build_prompt(p)
        forward_prefill(t, s, prompt, p, 0l)
        var tok = greedy_top(s, c.vocab_size)
        tok = chase_depth(t, s, arms, p, n, cfg.reps, tok)
        if (p2 > p + n + 1l) {
            let fill = p2 - (p + n + 1l)   // the shallow matrix consumed rows [p, p+n]
            let more <- build_prompt(fill)
            forward_prefill(t, s, more, fill, p + n + 1l)
            tok = greedy_top(s, c.vocab_size)
            print("\n")
            chase_depth(t, s, arms, p2, n, cfg.reps, tok)
        }
        metal_decode_stage_report()
        let ds = metal_decode_stats()
        let sp = metal_decode_spec_stats()
        print("driver totals: {ds.decodes} served, {ds.declines} declined, spec {sp.hits} hits / {sp.misses} misses\n")
    }
}

[export]
def main() {
    if (!jit_enabled()) {
        print("This harness is JIT-only — run with: daslang -jit <file>\n")
        return
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "decode_metal_chase")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "decode_metal_chase")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        run_chase(cfg)
    } else {
        print("das_metal is not built into this binary\n")
    }
}
