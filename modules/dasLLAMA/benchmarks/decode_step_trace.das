options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _jit_fast_math = true   // ggml-parity FP laxity — the CPU prefill/sampling half only

require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_math   // setup_dasllama_jobque_ + pin_kernel_backend + parallel_argmax
require dasllama/dasllama_layout   // convert_model_to_metal_blob — the blob-flavor transform
require ?das_metal dasllama/dasllama_metal_decode   // GPU driver half, Apple builds only
require daslib/jobque_boost
require daslib/clargs
require daslib/fio   // nolint:STYLE030,LINT019 — used only inside the Apple static_if half
require math   // nolint:STYLE030,LINT019 — used only inside the Apple static_if half
require strings   // nolint:STYLE030,LINT019 — used only inside the Apple static_if half
require dasllama/dasllama_env   // nolint:STYLE029,LINT019 — the read sites sit in the apple static_if half; the linux lint flavor sees the require as unused

// The per-step decode TRACE probe — the spec-chain bimodality diagnostic. Runs ONE long greedy
// decode with the driver's per-step trace armed and prints per-`bucket` summaries: wall / gpu /
// gap (GPU idle before the step's cb) / sched / wait per step + the serving-mode mix, optionally
// dumping the raw per-step TSV. One run samples ONE mode — the bimodal mode locks per process,
// so rep by re-running the command and compare the whole-run lines.
//
//   bin/daslang -jit modules/dasLLAMA/benchmarks/decode_step_trace.das -- \
//       -m <model.gguf> [-p 0] [-n 512] [--spec -1] [--bucket 32] [-o trace.tsv]

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string

    @clarg_short = "p"
    @clarg_doc = "Prefill depth before the traced decode (default: 0 — tg-style from position 0)"
    prefill : int = 0

    @clarg_short = "n"
    @clarg_doc = "Traced greedy decode tokens (default: 512)"
    ngen : int = 512

    @clarg_doc = "Spec-chain mode: -1 adaptive, 0 off, 1 forced (default: -1; DASLLAMA_METAL_SPEC also applies)"
    spec : int = -1

    @clarg_doc = "Summary bucket width in steps (default: 32)"
    bucket : int = 32

    @clarg_short = "o"
    @clarg_doc = "Raw per-step TSV output path (default: none)"
    output : string = ""

    @clarg_doc = "KV cache dtype (default: f16)"
    kv : KVDtype = KVDtype.f16

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private build_prompt(n : int64) : array<int64> {
    return <- [for (i in range64(n)); 1000l + (i % 5000l)]
}

def private greedy_top(s : Session; vocab : int64) : int64 {
    return parallel_argmax(s.logits, vocab)
}

def private run_trace(cfg) {   // untyped param = generic — non-Apple builds never compile the Metal-typed body
    if (empty(g_env_engine.pin_backend)) {
        pin_kernel_backend("portable")
    } else {
        pin_kernel_backend(g_env_engine.pin_backend)
    }
    if (!select_decode_override("metal")) {
        print("metal decode override not registered on this build\n")
        return
    }
    if (select_prefill_override("metal")) {
        print("prefill override: metal\n")
    }
    let p = int64(cfg.prefill)
    let n = max(int64(cfg.ngen), 1l)
    print("loading {cfg.model}\n")
    var t <- load_gguf(cfg.model, QuantMode.q8)
    if (!convert_model_to_metal_blob(t)) {   // the drivers are blob-only
        print("metal-blob transform declined — the drivers cannot serve this load\n")
        delete t
        return
    }
    let ctx_cap = max(2048l, p + n + 16l)
    t.config.seq_len = min(t.config.seq_len, ctx_cap)   // cap the KV allocation
    let c = t.config
    print("config: dim={c.dim} layers={c.n_layers} heads={c.n_heads}/{c.n_kv_heads} vocab={c.vocab_size}\n")
    print("active backend: {active_kernel_backend()}  kv: {cfg.kv}  spec: {cfg.spec}\n")
    set_metal_decode_speculate(cfg.spec)
    var s <- make_run_state(c, cfg.kv, cfg.kv)
    with_job_que() {
        setup_dasllama_jobque_()
        // warmup: JIT both paths + first-touch the resident weight regions
        let warm <- build_prompt(8l)
        for (wp in range64(8l)) {
            forward(t, s, warm[wp], wp)
        }
        forward_prefill(t, s, warm, 8l, 0l)
        var tok = 1l
        var pos = 0l
        if (p > 0l) {
            let prompt <- build_prompt(p)
            forward_prefill(t, s, prompt, p, 0l)
            tok = greedy_top(s, c.vocab_size)
            pos = p
        }
        metal_decode_stage_reset()
        metal_decode_trace_arm(n + 8l)   // headroom: a spec mispredict retires twice at one pos
        let t0 = ref_time_ticks()
        for (i in range64(n)) {
            forward(t, s, tok, pos + i)
            tok = greedy_top(s, c.vocab_size)
        }
        let us = get_time_usec(t0)
        var tr <- metal_decode_trace_take()
        let tps = double(n) / (double(us) * 1.0e-6lf)
        print("RUN: {n} tok @ depth {pos} in {us}us = {tps} t/s  (traced {length(tr)} gpu-served steps)\n")
        report_buckets(tr, int64(cfg.bucket))
        if (!empty(cfg.output)) {
            dump_tsv(tr, cfg.output)
        }
        metal_decode_stage_report()
        let sp = metal_decode_spec_stats()
        let ds = metal_decode_stats()
        print("driver totals: {ds.decodes} served, {ds.declines} declined, spec {sp.hits} hits / {sp.misses} misses\n")
        delete tr
    }
}

// per-bucket means over the trace; wall = finish-to-finish (the first step of the run has no
// predecessor and is skipped), gap = this cb's GPUStart minus the previous cb's GPUEnd
def private report_buckets(tr; bucket : int64) {   // untyped param = generic — non-Apple builds never compile the Metal-typed body
    let nt = long_length(tr)
    if (nt < 2l) {
        print("trace too short for buckets\n")
        return
    }
    print("bucket  steps  wall_ms  gpu_ms  gap_ms  sched_ms  wait_ms  slow/poked/spec\n")
    var b0 = 0l
    while (b0 < nt) {
        let b1 = min(b0 + bucket, nt)
        var wall = 0.0lf
        var nwall = 0l
        var gpu = 0.0lf
        var gap = 0.0lf
        var sched = 0.0lf
        var wait = 0.0lf
        var nslow = 0l
        var npoked = 0l
        var nspec = 0l
        for (i in range64(b0, b1)) {
            if (i > 0l) {
                wall += double(tr[i].finish_us - tr[i - 1l].finish_us) * 1.0e-3lf
                gap += (tr[i].gs - tr[i - 1l].ge) * 1000.0lf
                nwall++
            }
            gpu += (tr[i].ge - tr[i].gs) * 1000.0lf
            sched += (tr[i].ke - tr[i].ks) * 1000.0lf
            wait += double(tr[i].wait_us) * 1.0e-3lf
            if (tr[i].mode == 0l) {
                nslow++
            } elif (tr[i].mode == 1l) {
                npoked++
            } else {
                nspec++
            }
        }
        let cnt = double(b1 - b0)
        let cw = double(max(nwall, 1l))
        print("{b0 / bucket}\t{b1 - b0}\t{wall / cw:.3f}\t{gpu / cnt:.3f}\t{gap / cw:.3f}\t{sched / cnt:.3f}\t{wait / cnt:.3f}\t{nslow}/{npoked}/{nspec}\n")
        b0 = b1
    }
}

def private dump_tsv(tr; path : string) {   // untyped param = generic — non-Apple builds never compile the Metal-typed body
    fopen(path, "wb") $(f) {
        if (f == null) {
            print("cannot open {path}\n")
            return
        }
        fwrite(f, "step\tpos\tmode\tcommit_us\tfinish_us\twait_us\tks\tke\tgs\tge\n")
        var i = 0
        for (r in tr) {
            fwrite(f, "{i}\t{r.pos}\t{r.mode}\t{r.commit_us}\t{r.finish_us}\t{r.wait_us}\t{r.ks}\t{r.ke}\t{r.gs}\t{r.ge}\n")
            i++
        }
    }
    print("raw trace -> {path}\n")
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
        print_help(get_command_info(type<Args>), "decode_step_trace")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "decode_step_trace")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        run_trace(cfg)
    } else {
        print("das_metal is not built into this binary\n")
    }
}
