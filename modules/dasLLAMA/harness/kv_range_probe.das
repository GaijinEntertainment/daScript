options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/clargs
require math

// KV value-range probe — the f16-safety proof step (kv_cache_compression.md §3): prefill a prompt
// under the f32 codec, then walk each layer's cache slab and report per-layer K/V min / max /
// abs-max plus a log2 |value| histogram and the headroom vs the f16 max (65504). Post-norm KV is
// expected in ~±[0.001..few hundred]; anything within a few orders of 65504 confirms the f16 codec
// is near-lossless for that model. Run on Gemma (worst known: no QK-norm on K) AND Qwen3 (best:
// QK-norm) before trusting a default flip.
//   bin/daslang -jit modules/dasLLAMA/harness/kv_range_probe.das -- -m <model.gguf> [-p 512]

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path (or llama2.c .bin checkpoint)"
    model : string

    @clarg_short = "p"
    @clarg_doc = "Prefill depth to fill the cache with (default: 512)"
    prefill : int = 512

    @clarg_short = "q"
    @clarg_doc = "Weight quant mode (default: q8)"
    quant : QuantMode = QuantMode.q8

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// log2 histogram bucket for |v|: bucket = clamp(floor(log2 |v|), -24..16) + 24; zeros go below.
let HIST_LO = -24
let HIST_HI = 16
let HIST_N = HIST_HI - HIST_LO + 2   // +1 span, +1 zero bucket at index 0

def private hist_bucket(v : float) : int {
    let a = abs(v)
    if (a == 0.0) {
        return 0
    }
    let b = clamp(int(floor(log2(a))), HIST_LO, HIST_HI)
    return b - HIST_LO + 1
}

struct private Range {
    vmin : float = 3.4e38
    vmax : float = -3.4e38
    amax : float
    n : int64
}

def private fold(var r : Range; v : float) {
    r.vmin = min(r.vmin, v)
    r.vmax = max(r.vmax, v)
    r.amax = max(r.amax, abs(v))
    r.n++
}

def private report_layer(l : int64; tag : string; r : Range) {
    let headroom = r.amax > 0.0 ? 65504.0 / r.amax : 0.0
    print("  L{l} {tag}: min {r.vmin}  max {r.vmax}  absmax {r.amax}  headroom x{headroom}\n")
}

[export]
def main() {
    allow_cpu_prefill()   // CPU harness by intent — disarm the Metal-build tripwire
    if (!jit_enabled()) {
        print("This probe is JIT-only — run with: daslang -jit <file>\n")
        return
    }
    var res <- parse_args(type<Args>)
    if (res |> is_err) {
        print("error: {res |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "kv_range_probe")
        return
    }
    let cfg <- res |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "kv_range_probe")
        return
    }
    let p = max(int64(cfg.prefill), 8l)
    print("loading {cfg.model}\n")
    var t <- (ends_with(cfg.model, ".bin") ? load_checkpoint(cfg.model) : load_gguf(cfg.model, cfg.quant))
    if (ends_with(cfg.model, ".bin") && cfg.quant == QuantMode.q8) {
        quantize_weights(t)
    }
    t.config.seq_len = min(t.config.seq_len, p + 8l)   // cap the KV allocation
    let c = t.config
    print("config: dim={c.dim} layers={c.n_layers} heads={c.n_heads}/{c.n_kv_heads} qk_norm={c.qk_norm}\n")
    var s <- make_run_state(c)   // f32 codec: the cache holds the exact values the probe reads
    with_job_que() {
        setup_dasllama_jobque_()
        let prompt <- [for (i in range64(p)); 1000l + (i % 5000l)]
        forward_prefill(t, s, prompt, p, 0l)
    }
    // walk each layer's slab: seq_len rows of layer_kv_dim, but only the p filled positions
    var khist : array<int64>
    var vhist : array<int64>
    khist |> resize(HIST_N)
    vhist |> resize(HIST_N)
    var kall = Range()
    var vall = Range()
    print("--- per-layer K/V ranges over {p} cached positions ---\n")
    unsafe {
        for (l in range64(c.n_layers)) {
            if (layer_kv_source(c, l) != l) {
                continue   // shared-KV layer: aliases the source slab, nothing of its own
            }
            let kvd = layer_kv_dim(c, l)
            var kcp = reinterpret<float const?>(addr(s.key_cache[0]) + kv_base_k(s, l, c.seq_len))
            var vcp = reinterpret<float const?>(addr(s.value_cache[0]) + kv_base_v(s, l, c.seq_len))
            var kr = Range()
            var vr = Range()
            for (i in range64(p * kvd)) {
                fold(kr, kcp[i])
                khist[hist_bucket(kcp[i])]++
                fold(vr, vcp[i])
                vhist[hist_bucket(vcp[i])]++
            }
            report_layer(l, "K", kr)
            report_layer(l, "V", vr)
            fold(kall, kr.vmin)
            fold(kall, kr.vmax)
            kall.amax = max(kall.amax, kr.amax)
            fold(vall, vr.vmin)
            fold(vall, vr.vmax)
            vall.amax = max(vall.amax, vr.amax)
        }
    }
    print("--- global: K absmax {kall.amax} (headroom x{65504.0 / max(kall.amax, 1e-30)}), V absmax {vall.amax} (headroom x{65504.0 / max(vall.amax, 1e-30)}) ---\n")
    print("--- log2 |value| histogram (bucket floor(log2|v|), {HIST_LO}..{HIST_HI}; z = exact zero) ---\n")
    var ktot = 0l
    for (n in khist) {
        ktot += n
    }
    for (b in range(HIST_N)) {
        if (khist[b] == 0l && vhist[b] == 0l) {
            continue
        }
        let lbl = b == 0 ? "  z" : "{b - 1 + HIST_LO}"
        print("  2^{lbl}: K {khist[b]} ({100.0lf * double(khist[b]) / double(ktot)}%)  V {vhist[b]}\n")
    }
    let f16_ok = kall.amax < 65504.0 && vall.amax < 65504.0
    print(f16_ok
        ? "VERDICT: every K/V value is inside the finite f16 range — the f16 codec stores are clamp-free on this model.\n"
        : "VERDICT: values EXCEED f16 max — the store clamp will engage; investigate before defaulting to f16.\n")
    delete khist
    delete vhist
    delete s
    delete t
}
