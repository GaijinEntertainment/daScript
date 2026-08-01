options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _jit_fast_math = true   // ggml-parity FP laxity — matches how llama.cpp is benched

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_math   // setup_dasllama_jobque_ + apply_box_profile_runtime (engine-level, not re-exported)
require dasllama/dasllama_image   // load_model_cached — the .dlim rail, without the full facade's compile weight
require dasllama/dasllama_tokenizer
require ../performance/profile_common.das   // profile_threads + affinity_on — the standing per-box methodology
require daslib/jobque_boost
require daslib/module_path
require daslib/strings_boost
require daslib/clargs
require daslib/fio
require math

// Measures `b` — the marginal cost of one extra row in a batched verify — which is the single
// quantity deciding whether ANY speculative decoding scheme can pay on a model. Each arm advances
// the same number of positions through forward_prefill at a different batch width, over REAL
// corpus tokens (routing realism matters on MoE), and reports per-position cost against plain
// single-token decode. b = T(2)/T(1) - 1 in per-step terms: ~0 means extra rows ride the same
// weight stream and speculation can pay; ~1 means an extra row costs a whole step and it cannot.
// The width-1 arm is the control — same work as decode, different code path — so a slow width-1
// indicts the prefill path itself rather than batching.
//   bin/daslang -jit modules/dasLLAMA/benchmarks/verify_batch_probe.das -- -m <model.gguf>

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string

    @clarg_doc = "Prompt corpus (segments separated by lines of two percent signs; default: data/wikitext2_prompts.txt beside this bench)"
    prompts : string = ""

    @clarg_short = "p"
    @clarg_doc = "Prefill length before the timed window (default: 128)"
    plen : int = 128

    @clarg_short = "n"
    @clarg_doc = "Positions advanced per width arm (default: 64)"
    npos : int = 64

    @clarg_short = "q"
    @clarg_doc = "Weight quant mode (default: q8)"
    quant : QuantMode = QuantMode.q8

    @clarg_doc = "Dump the forward_profile section buckets per arm (untimed extra window)"
    prof : bool

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let WARM_STEPS = 6l

def private load_corpus(path : string) : array<string> {
    var texts : array<string>
    fopen(path, "rb") $(f) {
        if (f == null) {
            panic("can't open prompt corpus {path}")
        }
        for (p in split(fread(f), "%%")) {
            let tp = strip(p)
            if (!empty(tp) && !starts_with(tp, "#")) {
                texts |> push(clone_string(tp))
            }
        }
    }
    return <- texts
}

[export]
def main() {
    allow_cpu_prefill()   // CPU-protocol probe by intent — disarm the Metal-build tripwire
    if (!jit_enabled()) {
        print("This probe is JIT-only — run with: daslang -jit <file>\n")
        return
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "verify_batch_probe")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "verify_batch_probe")
        return
    }
    let plen = max(int64(cfg.plen), 1l)
    let want = max(int64(cfg.npos), 8l)
    let corpus = empty(cfg.prompts) ? path_join(get_this_module_dir(), "data/wikitext2_prompts.txt") : cfg.prompts
    var texts <- load_corpus(corpus)
    if (empty(texts)) {
        print("error: no prompts in {corpus}\n")
        return
    }
    let threads = has_env_variable("DAS_JOBQUE_THREADS") ? to_int(get_env_variable("DAS_JOBQUE_THREADS")) : profile_threads()
    if (!has_env_variable("DAS_JOBQUE_THREADS")) {
        set_jobque_threads_cap(max(threads - 1, 1))
    }
    let pinned = affinity_on() && !has_env_variable("DAS_JOBQUE_AFFINITY")
    if (pinned) {
        set_jobque_affinity(2)
    }
    apply_box_profile_runtime()
    set_mtp_spec(false)   // the draft head must not ride along — this probe times the trunk alone
    var tk <- load_tokenizer_auto(cfg.model)
    print("loading {cfg.model}\n")
    with_job_que() {
        setup_dasllama_jobque_()
        var t <- load_model_cached(cfg.model, cfg.quant)
        // one long real-token stream: routing on a MoE is content-dependent, so synthetic ids
        // would understate the union cost the batched arms are here to expose
        var ids : array<int64>
        for (tx in texts) {
            var seg <- encode(tk, tx, false)
            ids |> push_from(seg)
            delete seg
        }
        let need = plen + WARM_STEPS * 8l + want + 16l
        if (long_length(ids) < need) {
            print("error: corpus tokenizes to {long_length(ids)} ids, need {need} — pass a longer --prompts\n")
            delete ids
            delete t
            return
        }
        t.config.seq_len = min(t.config.seq_len, need + 8l)
        let c = t.config
        print("config: dim={c.dim} layers={c.n_layers} experts={c.n_expert} k={c.n_expert_used} | backend {active_kernel_backend()} (batch {active_batch_backend()}) | t{threads} affinity {pinned ? "hard" : "env/off"} | prefill {plen} + {want} positions per arm\n")
        var pre : array<int64>
        pre |> reserve(plen)
        for (i in range64(plen)) {
            pre |> push(ids[i])
        }
        // control: plain single-token decode, the cost every speculative scheme is measured against
        var decode_us = 0.0lf
        {
            var s <- make_run_state(c)
            forward_prefill(t, s, pre, plen, 0l)
            var pos = plen
            for (_w in range64(WARM_STEPS)) {
                forward(t, s, ids[pos], pos)
                pos++
            }
            if (cfg.prof) {
                forward_profile_reset()
            }
            let t0 = ref_time_ticks()
            for (_i in range64(want)) {
                forward(t, s, ids[pos], pos)
                pos++
            }
            decode_us = double(get_time_usec(t0)) / double(want)
            if (cfg.prof) {
                print("--- forward_profile: decode ---\n")
                forward_profile_report()
            }
            delete s
        }
        print("decode (forward)      : {decode_us:.0f} us/position  (baseline)\n")
        let widths = fixed_array(1l, 2l, 4l, 8l)
        for (w in widths) {
            var s <- make_run_state(c)
            forward_prefill(t, s, pre, plen, 0l)
            var batch : array<int64>
            batch |> resize(w)
            var pos = plen
            for (_w in range64(WARM_STEPS)) {
                for (j in range64(w)) {
                    batch[j] = ids[pos + j]
                }
                forward_prefill(t, s, batch, w, pos)
                pos += w
            }
            let steps = max(want / w, 1l)
            if (cfg.prof && w == 2l) {
                forward_profile_reset()
            }
            let t0 = ref_time_ticks()
            for (_i in range64(steps)) {
                for (j in range64(w)) {
                    batch[j] = ids[pos + j]
                }
                forward_prefill(t, s, batch, w, pos)
                pos += w
            }
            let us = double(get_time_usec(t0))
            if (cfg.prof && w == 2l) {
                print("--- forward_profile: width 2 ---\n")
                forward_profile_report()
            }
            let per_step = us / double(steps)
            let per_pos = per_step / double(w)
            // b = marginal cost of one extra row, in units of a plain decode step
            let b = w > 1l ? (per_step / decode_us - 1.0lf) / double(w - 1l) : per_step / decode_us - 1.0lf
            let btag = w > 1l ? "b={b:.2f}" : "path overhead {b * 100.0lf:.0f}%"
            print("prefill width {w}       : {per_step:.0f} us/step  {per_pos:.0f} us/position  ({per_step / decode_us:.2f}x decode)  {btag}\n")
            delete batch
            delete s
        }
        print("\nb ~ 0 -> extra verify rows are ~free, speculation pays; b ~ 1 -> an extra row costs a whole step, no scheme pays\n")
        delete pre
        delete ids
        delete t
    }
    delete tk
    delete texts
}
