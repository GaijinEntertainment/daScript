options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_common
require dasllama/dasllama_math   // parallel_argmax + setup_dasllama_jobque_
require dasllama/dasllama_image   // load_model_cached — the .dlim rail
require dasllama/dasllama_tokenizer
require ?das_metal dasllama/dasllama_metal_common   // finish_pending_step — land the one-deep pipelined step before reading logits
require ?das_metal dasllama/dasllama_metal_decode   // the metal spec-eval override registration
require ?das_metal dasllama/dasllama_metal_prefill
require ../performance/profile_common.das   // apply_box_profile_runtime
require daslib/jobque_boost
require daslib/module_path
require daslib/strings_boost
require daslib/clargs
require daslib/fio
require math

// The sequential-vs-verify parity probe: does the B=2 speculative verify reproduce the plain
// single-position step it stands in for? The metal single-stream decode is one-deep pipelined
// (metal_decode_forward lands the PREVIOUS step and launches the current one), so a lockstep
// A/B read of s.logits would compare a stale row. The probe decouples into two passes over ONE
// token stream instead:
//   pass 1 (spec): run mtp_spec_eval, record every committed token AND the verify's row logits
//                  (row 0 = the truth for tok@pos; row 1 = the accepted draft's continuation);
//   pass 2 (plain): replay the SAME committed tokens on a plain session, finish_pending_step
//                  after each forward so s.logits is landed, and compare per position.
// KV content matches by construction (identical tokens), so any delta is the verify path's own:
// batched-B=2 kernels vs single-row decode kernels. A drift that flips an argmax changes emitted
// tokens; drift below the flip line still eats acceptance silently, so both the flip count and
// the logit distance are reported, each flip tagged with the plain top-2 margin it crossed (a
// near-tie flip is benign; a wide-margin flip is a kernel defect).
//   bin/daslang -jit modules/dasLLAMA/harness/mtp_parity_probe.das -- -m <model.gguf>

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path (a NextN-headed file)"
    model : string

    @clarg_doc = "Prompt corpus (segments separated by lines of two percent signs; default: benchmarks/data/wikitext2_prompts.txt)"
    prompts : string = ""

    @clarg_short = "k"
    @clarg_doc = "Prompts to run, each on fresh sessions (default: 4)"
    nprompts : int = 4

    @clarg_short = "p"
    @clarg_doc = "Prefill length per prompt (default: 128)"
    plen : int = 128

    @clarg_short = "n"
    @clarg_doc = "Speculative rounds per prompt (default: 48)"
    rounds : int = 48

    @clarg_short = "q"
    @clarg_doc = "Weight quant mode (default: q8)"
    quant : QuantMode = QuantMode.q8

    @clarg_doc = "Negative control: pass 1 also steps plainly (no speculation) - every compared row must read 0.0"
    control : bool

    @clarg_doc = "Print every flip as it happens (position, ids, margin, drift)"
    verbose : bool

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

struct RowStats {
    rows : int64
    flips : int64
    max_abs : float           // the largest |plain - verify| over every row and vocab entry
    sum_max_abs : double      // per-row max |delta|, summed (mean = / rows)
    sum_mean_abs : double     // per-row mean |delta|, summed
    flip_margin_max : float   // widest plain top-2 margin among the flipped rows
    flip_margin_sum : double
    b_lt_1e3 : int64          // per-row max |delta| histogram
    b_lt_1e2 : int64
    b_lt_1e1 : int64
    b_ge_1e1 : int64
}

// A recorded verify row: its position, which row (0 or 1), and where its logits start in the store.
struct Rec {
    pos : int64
    row : int64
    off : int64
}

// plain top-2 margin: top1 - top2 of the plain logits (the tie distance an argmax flip crossed)
def private top2_margin(a : array<float>; n : int64) : float {
    var m1 = -3.4e38
    var m2 = -3.4e38
    for (i in range64(n)) {
        let v = a[i]
        if (v > m1) {
            m2 = m1
            m1 = v
        } elif (v > m2) {
            m2 = v
        }
    }
    return m1 - m2
}

def private argmax_store(store : array<float>; off, n : int64) : int64 {
    var best = 0l
    var bv = store[off]
    for (i in range64(n)) {
        if (store[off + i] > bv) {
            bv = store[off + i]
            best = i
        }
    }
    return best
}

def private compare_row(plain, store : array<float>; soff, n : int64; var st : RowStats; tag : string; pos : int64; verbose : bool) {
    var mx = 0.0
    var sum = 0.0lf
    for (i in range64(n)) {
        let d = abs(plain[i] - store[soff + i])
        mx = max(mx, d)
        sum += double(d)
    }
    st.rows++
    st.max_abs = max(st.max_abs, mx)
    st.sum_max_abs += double(mx)
    st.sum_mean_abs += sum / double(n)
    if (mx < 1e-3) {
        st.b_lt_1e3++
    } elif (mx < 1e-2) {
        st.b_lt_1e2++
    } elif (mx < 1e-1) {
        st.b_lt_1e1++
    } else {
        st.b_ge_1e1++
    }
    let ap = parallel_argmax(plain, n)
    let av = argmax_store(store, soff, n)
    if (ap != av) {
        st.flips++
        let mg = top2_margin(plain, n)
        st.flip_margin_max = max(st.flip_margin_max, mg)
        st.flip_margin_sum += double(mg)
        if (verbose) {
            print("  FLIP {tag} pos={pos}: plain argmax {ap} vs verify {av} | plain top-2 margin {mg:.4f} | max|delta| {mx:.4f}\n")
        }
    }
}

def private report(st : RowStats; tag : string) {
    if (st.rows == 0l) {
        print("{tag}: no rows compared\n")
        return
    }
    let r = double(st.rows)
    print("{tag}: rows {st.rows} | argmax flips {st.flips} ({double(st.flips) * 100.0lf / r:.2f}%) | max|delta| {st.max_abs:.4f} (per-row mean {st.sum_max_abs / r:.4f}) | mean|delta| {st.sum_mean_abs / r:.5f}\n")
    print("{tag}:   per-row max|delta| histogram: <1e-3 {st.b_lt_1e3} | <1e-2 {st.b_lt_1e2} | <1e-1 {st.b_lt_1e1} | >=1e-1 {st.b_ge_1e1}\n")
    if (st.flips > 0l) {
        print("{tag}:   flips crossed a plain top-2 margin of at most {st.flip_margin_max:.4f} (mean {st.flip_margin_sum / double(st.flips):.4f})\n")
    }
}

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

// the metal decode is one-deep pipelined: land the in-flight step so s.logits is this step's
def private land() {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        finish_pending_step()
    }
}

def private record(var recs : array<Rec>; var store : array<float>; src : array<float>; pos, row, vocab : int64) {
    recs |> push(Rec(pos = pos, row = row, off = long_length(store)))
    store |> reserve(long_length(store) + vocab)
    for (i in range64(vocab)) {
        store |> push(src[i])
    }
}

// Pass 1: run the speculative rail, recording the committed token stream (`seq`, one token per
// consumed position from plen onward) and every verify row's logits into the flat `store`.
def private spec_pass(t : Model; ids : array<int64>; plen, rounds : int64; control : bool;
                      var seq : array<int64>; var recs : array<Rec>; var store : array<float>;
                      var drafted, accepted, cold : int64&) {
    let c = t.config
    let vocab = c.vocab_size
    var pre <- [for (i in range64(plen)); ids[i]]
    var b <- make_run_state(c, KVDtype.f16, KVDtype.f16)
    forward_prefill(t, b, pre, plen, 0l)
    land()
    var tok = parallel_argmax(b.logits, vocab)
    var pos = plen
    for (_r in range64(rounds)) {
        if (pos + 3l >= c.seq_len) {
            break
        }
        let nd0 = b.mtp_drafted
        var acc = -1l
        var ok = false
        if (control) {   // no speculation: a plain step, recorded like an always-reject round
            forward(t, b, tok, pos)
        } else {
            ok = mtp_spec_eval(t, b, tok, acc)
        }
        land()   // the cold path (and the control) leave a pipelined step in flight
        seq |> push(tok)
        if (!control && b.mtp_drafted == nd0) {
            cold++
        } elif (!control) {
            drafted++
        }
        // row 0 = the truth for tok@pos: b.mtp_logits on accept, promoted to b.logits otherwise
        record(recs, store, ok ? b.mtp_logits : b.logits, pos, 0l, vocab)
        if (ok) {
            accepted++
            seq |> push(acc)
            record(recs, store, b.logits, pos + 1l, 1l, vocab)   // row 1 = the accepted draft's continuation
            pos += 2l
        } else {
            pos++
        }
        tok = parallel_argmax(b.logits, vocab)
    }
    delete b
    delete pre
}

// Ground truth: pure plain greedy decode for `n` positions from the same prefill. The spec
// committed stream (seq from pass 1) must equal this token-for-token if the metal verify is a
// faithful stand-in for plain decode; the first mismatch position and the count are the honest
// "does speculation change the output" answer.
def private plain_greedy(t : Model; ids : array<int64>; plen, n : int64) : array<int64> {
    let c = t.config
    let vocab = c.vocab_size
    var pre <- [for (i in range64(plen)); ids[i]]
    var s <- make_run_state(c, KVDtype.f16, KVDtype.f16)
    forward_prefill(t, s, pre, plen, 0l)
    land()
    var out : array<int64>
    out |> reserve(n)
    var tok = parallel_argmax(s.logits, vocab)
    var pos = plen
    for (_i in range64(n)) {
        out |> push(tok)
        forward(t, s, tok, pos)
        land()
        tok = parallel_argmax(s.logits, vocab)
        pos++
    }
    delete s
    delete pre
    return <- out
}

// Pass 2: replay the committed tokens on a plain session, landing each step, and compare every
// recorded position against the stored verify logits.
def private plain_pass(t : Model; ids : array<int64>; plen : int64; seq : array<int64>;
                       recs : array<Rec>; store : array<float>; var st0, st1 : RowStats; verbose : bool) {
    let c = t.config
    let vocab = c.vocab_size
    var pre <- [for (i in range64(plen)); ids[i]]
    var a <- make_run_state(c, KVDtype.f16, KVDtype.f16)
    forward_prefill(t, a, pre, plen, 0l)
    var ri = 0
    var pos = plen
    for (tok in seq) {
        forward(t, a, tok, pos)
        land()
        while (ri < length(recs) && recs[ri].pos == pos) {
            let rc = recs[ri]
            if (rc.row == 0l) {
                compare_row(a.logits, store, rc.off, vocab, st0, "row0", pos, verbose)
            } else {
                compare_row(a.logits, store, rc.off, vocab, st1, "row1", pos, verbose)
            }
            ri++
        }
        pos++
    }
    delete a
    delete pre
}

// One prompt: the spec pass, the plain replay, and the token-stream diff against pure plain greedy.
def private run_prompt(t : Model; tk : Tokenizer; ids : array<int64>; plen, rounds : int64; cfg : Args;
                       var st0, st1 : RowStats; var drafted, accepted, cold : int64&;
                       var seq_tokens, seq_mism, seq_first_div : int64&) {
        var seq : array<int64>
        var recs : array<Rec>
        var store : array<float>
        spec_pass(t, ids, plen, rounds, cfg.control, seq, recs, store, drafted, accepted, cold)
        plain_pass(t, ids, plen, seq, recs, store, st0, st1, cfg.verbose)
        if (!cfg.control) {
            var ref <- plain_greedy(t, ids, plen, long_length(seq))
            var first = -1l
            var mism = 0l
            for (i in range64(min(long_length(seq), long_length(ref)))) {
                if (seq[i] != ref[i]) {
                    mism++
                    if (first < 0l) {
                        first = i
                    }
                }
            }
            if (cfg.verbose) {
                var a16 : array<int64>
                var b16 : array<int64>
                a16 |> reserve(16l)
                b16 |> reserve(16l)
                for (i in range64(min(16l, min(long_length(seq), long_length(ref))))) {
                    a16 |> push(seq[i])
                    b16 |> push(ref[i])
                }
                print("  spec  seq[0..16) = {a16}\n  plain ref[0..16) = {b16}\n  spec text : {decode(tk, a16)}\n  plain text: {decode(tk, b16)}\n")
                delete a16
                delete b16
            }
            seq_tokens += long_length(seq)
            seq_mism += mism
            if (first >= 0l && seq_first_div < 0l) {
                seq_first_div = first
            }
            delete ref
        }
        delete seq
        delete recs
        delete store
}

[export]
def main() {
    if (!jit_enabled()) {
        print("This probe is JIT-only - run with: daslang -jit <file>\n")
        return
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "mtp_parity_probe")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "mtp_parity_probe")
        return
    }
    let plen = max(int64(cfg.plen), 8l)
    let rounds = max(int64(cfg.rounds), 1l)
    let corpus = empty(cfg.prompts) ? path_join(get_this_module_dir(), "../benchmarks/data/wikitext2_prompts.txt") : cfg.prompts
    var texts <- load_corpus(corpus)
    if (empty(texts)) {
        print("error: no prompts in {corpus}\n")
        return
    }
    apply_box_profile_runtime()
    set_metal_mode(MetalMode.required)   // before load — the probe is about the METAL verify
    set_mtp_spec(true)                   // before load — the prefill warms the draft head
    let tk <- load_tokenizer_auto(cfg.model)
    print("loading {cfg.model}\n")
    with_job_que() {
        setup_dasllama_jobque_()
        var t <- load_model_cached(cfg.model, cfg.quant)
        if (!t.metal_blob) {
            panic("mtp_parity_probe: the mapped flavor is not metal_blob - the probe targets the metal verify")
        }
        if (t.config.n_layer_nextn <= 0l) {
            panic("mtp_parity_probe: {cfg.model} has no NextN draft head")
        }
        t.config.seq_len = min(t.config.seq_len, plen + 2l * rounds + 16l)
        let c = t.config
        let mode = cfg.control ? " | CONTROL" : ""
        print("config: dim={c.dim} layers={c.n_layers} vocab={c.vocab_size} nextn={c.n_layer_nextn} | prefill {plen} + {rounds} rounds x {cfg.nprompts} prompts{mode}\n")
        var st0 = RowStats()
        var st1 = RowStats()
        var drafted = 0l
        var accepted = 0l
        var cold = 0l
        var used = 0
        var seq_tokens = 0l
        var seq_mism = 0l
        var seq_first_div = -1l
        for (tx in texts) {
            if (used >= cfg.nprompts) {
                break
            }
            var ids <- encode(tk, tx, false)
            if (long_length(ids) < plen) {
                delete ids
                continue
            }
            used++
            run_prompt(t, tk, ids, plen, rounds, cfg, st0, st1, drafted, accepted, cold, seq_tokens, seq_mism, seq_first_div)
            delete ids
        }
        let rate = drafted > 0l ? double(accepted) * 100.0lf / double(drafted) : 0.0lf
        print("prompts {used} | rounds drafted {drafted}, accepted {accepted} ({rate:.1f}%), cold {cold}\n")
        if (!cfg.control) {
            let pct = seq_tokens > 0l ? double(seq_mism) * 100.0lf / double(seq_tokens) : 0.0lf
            print("TOKEN DIVERGENCE (spec committed vs plain greedy): {seq_mism}/{seq_tokens} tokens differ ({pct:.2f}%), first at offset {seq_first_div}\n")
        }
        if (cfg.control) {
            report(st0, "control (spec-off pass vs plain replay)")
        } else {
            report(st0, "row0 (verify truth vs plain step)")
            report(st1, "row1 (accepted draft vs plain step)")
        }
        delete t
    }
    delete texts
}
