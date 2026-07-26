options gen2
options stack = 65536   // dasLLAMA program-root convention (options stack does not unify up from libs)
options _jit_fast_math = true   // ggml-parity FP laxity (non-bit-exact, ~+10%); tests stay bit-exact

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require daslib/jobque_boost
require daslib/clargs
require daslib/fio
require math
require dasllama/dasllama_math  // pin_kernel_backend / active_kernel_backend (engine-level, not re-exported)

// Per-op DECODE profile: prefill a synthetic prompt to depth P, then greedily decode N tokens with
// the forward-section profiler windowed to just the decode loop. Prints the measured per-bucket
// time split next to a config-derived per-token weight/KV traffic table, so the time share of each
// section can be checked against its theoretical bandwidth share (the traffic table is what says
// whether a weight-format change — e.g. native MXFP4 experts — can pay).
//   bin/daslang -jit modules/dasLLAMA/benchmarks/decode_prof.das -- -m <model.gguf> [-p 256] [-n 64]

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string

    @clarg_short = "p"
    @clarg_doc = "Prefill depth before the decode window (default: 8)"
    prefill : int = 8

    @clarg_doc = "Second, deeper window in the same process — 0 to skip (default: 512)"
    prefill2 : int = 512

    @clarg_short = "n"
    @clarg_doc = "Decode tokens profiled (default: 64)"
    ngen : int = 64

    @clarg_short = "q"
    @clarg_doc = "Weight quant mode (default: q8)"
    quant : QuantMode = QuantMode.q8

    @clarg_doc = "KV cache dtype (default: f32)"
    kv : KVDtype = KVDtype.f32

    @clarg_doc = "Page the KV cache through a KVPool instead of the flat slabs"
    paged : bool

    @clarg_doc = "Positions per page when --paged (default: 64)"
    page_rows : int = 64

    @clarg_doc = "Override target_chunk_work (MACs per dispatch chunk; default 1 = thread everything; 1000000 = the old coarse threshold; -1 = keep default)"
    target_work : int = -1

    @clarg_doc = "Override the GEMV lane cap (0 = uncapped, -1 = keep default)"
    gemv_cap : int = -1

    @clarg_doc = "Override the batch lane cap (0 = uncapped, -1 = keep default)"
    batch_cap : int = -1

    @clarg_doc = "Override the quantized-matmul oversplit factor (chunks per hw job; -1 = keep default)"
    chunks : int = -1

    @clarg_doc = "Team dispatch: workers self-serve chunks off one published atomic instead of per-chunk fifo jobs"
    team : bool

    @clarg_doc = "Save a per-lane jobque event trace of the FIRST decode window to this path (Perfetto/chrome-tracing JSON)"
    trace : string

    @clarg_doc = "GEMV chunks per lane (fine-grain self-serve tail; default 8; 1 = the old equal split; -1 = keep default)"
    fine : int = -1

    @clarg_doc = "Weight-scale plane A/B: 1 = convert to f16 at load (llama.cpp byte parity), 0 = keep f32, -1 = keep default"
    wscale_f16 : int = -1

    @clarg_doc = "Native K-quant planes A/B: 1 = keep Q4_K/Q5_K/Q6_K packed at load, 0 = q8-requant fallback, -1 = keep default"
    kquant_native : int = -1

    @clarg_doc = "Q5_0 -> k5 re-encode A/B: 1 = re-quantize Q5_0 into the k5 plane at load (LOSSY, 9.0 -> 5.625 bits), 0 = bit-exact Q5_0 -> Q8, -1 = keep default"
    q50_native : int = -1

    @clarg_doc = "Native Q5_1 plane tier A/B: 1 = per-32 q51 planes (expert stacks), 0 = q8-requant fallback (exact), -1 = keep default"
    q51_native : int = -1

    @clarg_doc = "Noisy diagnostics: the loader reports backend, format tags and plane bytes (also DASLLAMA_NOISY=1)"
    noisy : bool

    @clarg_doc = "A/B: size chunk counts to workers only (the pre-fix split), not workers + caller lane"
    legacy_lanes : bool

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private build_prompt(n : int64) : array<int64> {
    return <- [for (i in range64(n)); 1000l + (i % 5000l)]
}

def private greedy_top(s : Session; vocab : int64) : int64 {
    return parallel_argmax(s.logits, vocab)   // team-scanned; bit-identical to the serial loop
}

// Bytes read per weight for a quant mode (quants + block scales at 1 per 32 — f32, or raw
// binary16 on a wscale_f16 model's q8 plane).
def private wbytes(q : QuantMode; wscale16 : bool = false) : double {
    if (q == QuantMode.q8) {
        return 1.0lf + (wscale16 ? 2.0lf : 4.0lf) / 32.0lf
    }
    if (q == QuantMode.q4_0) {
        return 0.5lf + 4.0lf / 32.0lf
    }
    return 4.0lf
}

// Bytes read per weight for one tensor: its native K-quant plane pair (quant + scale planes per
// 256-weight superblock) when kq-tagged, else the model-wide q8 figure.
def private kq_wbytes(f : KqFmt; q8wb : double) : double {
    if (f == KqFmt.k4) {
        return (128.0lf + 20.0lf) / 256.0lf   // nibbles + kq v2 scale rows (16B disk block + pad)
    }
    if (f == KqFmt.k5) {
        return (160.0lf + 20.0lf) / 256.0lf   // nibbles + high-bit plane + kq v2 scale rows
    }
    if (f == KqFmt.k6) {
        return (192.0lf + 18.0lf) / 256.0lf   // ql + qh + native int8 sub-scales + f16 d
    }
    if (f == KqFmt.q40) {
        return (128.0lf + 16.0lf) / 256.0lf   // nibbles + 8 x f16 d
    }
    if (f == KqFmt.q51) {
        return (20.0lf + 4.0lf) / 32.0lf   // per 32-block: nibbles + qh, f16 d + m
    }
    return q8wb
}

def private fmt_or_q8(a : array<KqFmt>; l : int64) : KqFmt => empty(a) ? KqFmt.q8 : a[l]

def private mb(v : double) : double => v / (1024.0lf * 1024.0lf)

// Config-derived weight + KV bytes READ per decoded token, split by section, evaluated at context
// depth `pos` (attention KV traffic grows with position; everything else is position-free).
def private print_traffic_table(t : Model; pos : int64; kvdt : KVDtype) {
    let c = t.config
    let wb = wbytes(t.quant, t.wscale_f16)
    let dim = c.dim
    var attn = 0.0lf
    var kv_read = 0.0lf
    for (l in range64(c.n_layers)) {
        let qd = layer_qd(c, l)
        let kvd = layer_kv_dim(c, l)
        attn += double(dim * qd) * kq_wbytes(fmt_or_q8(t.wq_fmt, l), wb)
        attn += double(dim * kvd) * kq_wbytes(fmt_or_q8(t.wk_fmt, l), wb)
        if (t.wv_offs[l] >= 0l) {
            attn += double(dim * kvd) * kq_wbytes(fmt_or_q8(t.wv_fmt, l), wb)
        }
        attn += double(qd * dim) * kq_wbytes(fmt_or_q8(t.wo_fmt, l), wb)
        let cnt = layer_is_sliding(c, l) ? min(pos, c.sliding_window) : pos
        kv_read += double(cnt * kv_row_bytes(kvdt, kvd)) * 2.0lf   // row bytes: codec-exact (q8_0 incl. block scales)
    }
    var ffn = 0.0lf
    var moe = 0.0lf
    var router = 0.0lf
    var shexp = 0.0lf
    if (c.moe) {
        router = double(c.n_layers * dim * c.n_expert) * 4.0lf   // router weights stay fp32
        for (l in range64(c.n_layers)) {
            let epk = double(c.n_expert_used * dim * c.n_ff_exp)   // elements per (kind, token)
            if (t.experts_mx4) {
                moe += epk * 3.0lf * (17.0lf / 32.0lf)   // native MXFP4: nibbles + E8M0 per 32
            } else {
                moe += epk * (kq_wbytes(fmt_or_q8(t.we1_fmt, l), wb)
                    + kq_wbytes(fmt_or_q8(t.we2_fmt, l), wb)
                    + kq_wbytes(fmt_or_q8(t.we3_fmt, l), wb))
            }
        }
        if (c.n_ff_shexp > 0l) {
            shexp = double(c.n_layers * 3l * dim * c.n_ff_shexp) * wb   // wsh* stay on the q8 rail
        }
        if (c.moe_dense_shexp) {   // gemma4 dense parallel shared expert rides w1/w2/w3
            for (l in range64(c.n_layers)) {
                shexp += double(dim * layer_hidden(t, l)) * (kq_wbytes(fmt_or_q8(t.w1_fmt, l), wb)
                    + kq_wbytes(fmt_or_q8(t.w2_fmt, l), wb)
                    + kq_wbytes(fmt_or_q8(t.w3_fmt, l), wb))
            }
        }
    } else {
        for (l in range64(c.n_layers)) {
            ffn += double(dim * layer_hidden(t, l)) * (kq_wbytes(fmt_or_q8(t.w1_fmt, l), wb)
                + kq_wbytes(fmt_or_q8(t.w2_fmt, l), wb)
                + kq_wbytes(fmt_or_q8(t.w3_fmt, l), wb))
        }
    }
    var clsb = wb   // untied q8 default
    if (cls_kq(t)) {
        clsb = kq_wbytes(t.emb_fmt, wb)
    } elif (c.shared_weights) {
        clsb = t.cls_q8 ? wb : 4.0lf
    } elif (t.wcls_fmt != KqFmt.q8) {
        clsb = kq_wbytes(t.wcls_fmt, wb)
    }
    let cls = double(dim * c.vocab_size) * clsb
    let embed = double(dim) * (t.cls_q8 ? wbytes(QuantMode.q8, t.wscale_f16) : (cls_kq(t) ? kq_wbytes(t.emb_fmt, wb) : 4.0lf))
    let total = attn + kv_read + ffn + moe + router + shexp + cls + embed
    print("--- per-token traffic at ctx depth {pos} (config-derived, MB read/token) ---\n")
    print("  attn weights: {mb(attn)} ({100.0lf * attn / total}%)\n")
    print("  kv cache:     {mb(kv_read)} ({100.0lf * kv_read / total}%)\n")
    if (c.moe) {
        print("  moe experts:  {mb(moe)} ({100.0lf * moe / total}%)\n")
        print("  moe router:   {mb(router)} ({100.0lf * router / total}%)\n")
        if (shexp > 0.0lf) {
            print("  moe shexp:    {mb(shexp)} ({100.0lf * shexp / total}%)\n")
        }
    } else {
        print("  ffn weights:  {mb(ffn)} ({100.0lf * ffn / total}%)\n")
    }
    print("  classifier:   {mb(cls)} ({100.0lf * cls / total}%)\n")
    print("  TOTAL:        {mb(total)} MB/token -> at 100 GB/s ~= {102400.0lf / mb(total)} tok/s ceiling\n")
}

// One measured window: greedily decode n tokens from context depth p (the cache is already filled
// to p), with the profiler reset to cover exactly this loop. Returns the chained last token.
def private profile_window(t : Model; var s : Session; p, n : int64; tok0 : int64) : int64 {
    var tok = tok0
    forward_profile_reset()
    let t0 = ref_time_ticks()
    for (i in range64(n)) {
        trace_marker("token", int(i))   // unit boundary for the timeline viewer (no-op untraced)
        forward(t, s, tok, p + i)
        tok = greedy_top(s, t.config.vocab_size)
    }
    let us = get_time_usec(t0)
    let tps = double(n) / (double(us) * 1.0e-6lf)
    print("decode: {n} tokens from ctx {p} in {us}us = {tps} tok/s\n\n")
    forward_profile_report()
    print("\n")
    print_traffic_table(t, p + n / 2l, s.kv_dtype_k)
    return tok
}

def private run_profile(var t : Model; p, n, p2 : int64; team : bool; kvdt : KVDtype; paged : bool; page_rows : int64; trace : string) {
    let c = t.config
    var pool <- create_kv_pool_(c, page_rows, kvdt, kvdt)   // empty (no groups) unless --paged
    var s : Session
    if (paged) {
        s <- make_run_state_paged(c, pool)
        print("paged KV: page_rows={page_rows}\n")
    } else {
        s <- make_run_state(c, kvdt, kvdt)
    }
    with_job_que() {
        setup_dasllama_jobque_()   // pooled forks + batched dispatch + spin window + join poll (the engine standard)
        if (team) {   // needs the live job que — must sit inside with_job_que
            set_jobque_team_mode(true)
            print("team dispatch: on\n")
        }
        // warmup — JIT both paths (the real prefill below overwrites these cache positions)
        let warm <- build_prompt(8l)
        for (wp in range64(8l)) {
            forward(t, s, warm[wp], wp)
        }
        forward_prefill(t, s, warm, 8l, 0l)
        // prefill to depth p, then window the profiler to just the decode loop
        let prompt <- build_prompt(p)
        forward_prefill(t, s, prompt, p, 0l)
        var tok = greedy_top(s, c.vocab_size)
        if (!empty(trace)) {
            jobque_trace_start(1 << 21)   // decode makes ~30k chunk events/token — size for a ~32-token window
            set_trace_tags(true)
        }
        tok = profile_window(t, s, p, n, tok)
        if (!empty(trace)) {
            set_trace_tags(false)
            jobque_trace_stop()
            if (jobque_trace_save(trace)) {
                print("jobque trace saved: {trace}\n")
            } else {
                print("jobque trace: nothing recorded\n")
            }
        }
        if (p2 > p + n) {   // second window, deeper into the context, off the same load
            let fill = p2 - (p + n)
            let more <- build_prompt(fill)
            forward_prefill(t, s, more, fill, p + n)
            tok = greedy_top(s, c.vocab_size)
            print("\n")
            profile_window(t, s, p2, n, tok)
        }
    }
}

[export]
def main() {
    if (!jit_enabled()) {
        print("This profiler is JIT-only — run with: daslang -jit <file>\n")
        return
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "decode_prof")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "decode_prof")
        return
    }
    let p = max(int64(cfg.prefill), 8l)
    let n = max(int64(cfg.ngen), 1l)
    let p2 = int64(cfg.prefill2)
    // backend pin for A/B on one binary (e.g. portable vs x64-gen) — must land BEFORE
    // load_gguf: select_matmul_backend_for_load honors the pin at load, which decides repack
    if (has_env_variable("DASLLAMA_PIN_BACKEND")) {
        pin_kernel_backend(get_env_variable("DASLLAMA_PIN_BACKEND"))
    }
    // the wscale rail is a LOAD-TIME convert — the flag must land before load_gguf
    if (cfg.wscale_f16 >= 0) {
        set_wscale_f16(cfg.wscale_f16 != 0)
        print("wscale_f16 override: {cfg.wscale_f16 != 0}\n")
    }
    // native K-quant planes are a LOAD-TIME layout decision — same rule
    if (cfg.kquant_native >= 0) {
        set_kquant_native(cfg.kquant_native != 0)
        print("kquant_native override: {cfg.kquant_native != 0}\n")
    }
    if (cfg.q50_native >= 0) {
        set_kq_q50_native(cfg.q50_native != 0)
        print("q50_native override: {cfg.q50_native != 0}\n")
    }
    if (cfg.q51_native >= 0) {
        set_kq_q51_native(cfg.q51_native != 0)
        print("q51_native override: {cfg.q51_native != 0}\n")
    }
    if (cfg.noisy) {
        set_dasllama_noisy(true)
    }
    // the resident whole-decode override (dasMetal emission arc): claims the ENTIRE layer stack
    // per token; needs the f32 scale plane + a row-major backend (like DASLLAMA_PIN_PREFILL)
    if (has_env_variable("DASLLAMA_PIN_DECODE")) {
        let dv = get_env_variable("DASLLAMA_PIN_DECODE")
        if (dv == "metal") {
            // measurement runs are MetalMode.required — a CPU fallback fails the run
            set_metal_mode(MetalMode.required)
            print("decode override: metal, MetalMode.required — CPU fallback fails the run\n")
        } elif (!select_decode_override(dv)) {
            print("decode override `{dv}` not registered on this build\n")
        } else {
            print("decode override: {dv}\n")
        }
    }
    print("loading {cfg.model}\n")
    var t <- load_gguf(cfg.model, cfg.quant)
    let ctx_cap = max(2048l, max(p, p2) + n + 8l)
    t.config.seq_len = min(t.config.seq_len, ctx_cap)   // cap the KV allocation
    let c = t.config
    print("config: dim={c.dim} layers={c.n_layers} heads={c.n_heads}/{c.n_kv_heads} vocab={c.vocab_size} moe={c.moe}\n")
    print("active backend: {active_kernel_backend()}\n")
    if (cfg.target_work >= 1) {
        set_target_chunk_work(int64(cfg.target_work))
        print("target_chunk_work override: {cfg.target_work}\n")
    }
    if (cfg.gemv_cap >= 0) {
        set_gemv_lane_cap(cfg.gemv_cap)
        print("gemv_lane_cap override: {cfg.gemv_cap}\n")
    }
    if (cfg.batch_cap >= 0) {
        set_batch_lane_cap(cfg.batch_cap)
        print("batch_lane_cap override: {cfg.batch_cap}\n")
    }
    if (cfg.chunks >= 1) {
        set_q8_batch_chunks_per_job(cfg.chunks)
        print("q8_batch_chunks_per_job override: {cfg.chunks}\n")
    }
    if (cfg.legacy_lanes) {
        set_dispatch_caller_lane(false)
        print("dispatch lanes: workers only (legacy)\n")
    }
    if (cfg.fine >= 1) {
        set_gemv_chunks_per_lane(cfg.fine)
        print("gemv_chunks_per_lane override: {cfg.fine}\n")
    }
    // A/B the fused per-block decode chains (default on; 0 = per-op dispatches) — same rail
    // as emission_bench
    if (has_env_variable("DASLLAMA_FUSED_DECODE")) {
        set_fused_decode(get_env_variable("DASLLAMA_FUSED_DECODE") != "0")
        print("fused decode override: {get_fused_decode()}\n")
    }
    if (cfg.kv != KVDtype.f32) {
        print("kv cache dtype: {cfg.kv}\n")
    }
    run_profile(t, p, n, p2, cfg.team, cfg.kv, cfg.paged, int64(cfg.page_rows), cfg.trace)
}
