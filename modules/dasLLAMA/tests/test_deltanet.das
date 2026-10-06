options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model/sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // CPU-correctness test — prefills on the CPU by design
}
require daslib/jobque_boost
require daslib/fio
require daslib/rtti   // this_context().last_exception - the refused loads' text
require dasllama/dasllama_math  // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // the deltanet chain and decode-step route pins
require _model_tier   // models_dir() + model_available() presence/size-tier gate

// Chunked-vs-recurrent deltanet prefill equivalence on Qwen3.5-0.8B. The 20-token parity fixture
// covers only a single partial chunk, so this probe prefills 150 synthetic tokens — two full
// 64-token chunks plus a partial tail — exercising chunk boundaries, partial-chunk math, and the
// batched-conv history handoff. The two forms are NOT bit-identical (chunk FP reassociation;
// upstream accepts the same divergence between its autoregressive and chunking paths), so the
// gate is the forced-feed logits-tolerance form: the prefill logits within a bar of each other,
// then the recurrent arm's greedy token fed to BOTH arms for 8 steps with the logits compared
// within the bar at every step — which also proves the conv/S state the chunked prefill leaves
// behind feeds the recurrent decode correctly. The claim is the CPU chunk algebra, so the cell
// pins the GPU tier's deltanet chain and decode step off along with the vector activation forms.
// The YaRN cell loads the same 0.8B under the override kinds: every model cell here skips off the
// interpreter and off a missing Qwen3.5-0.8B-Q8_0.gguf.

let private DN_LOGIT_BAR_REL = 0.04   // of the step's max |logit|; the two forms measure ~0.015 of it apart

[test]
def test_deltanet_chunked_prefill(t : T?) {
    t |> run("deltanet chunked prefill == recurrent (Qwen3.5-0.8B, multi-chunk)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        var tr <- load_gguf(path, QuantMode.q8)
        if (tr.config.seq_len > 2048l) {   // KV RAM cap, same as the parity fixtures
            tr.config.seq_len = 2048l
        }
        let c = tr.config
        let prev_dn = get_deltanet_prefill_mode()
        // pin the scalar activation forms: this gate isolates the CHUNK algebra, and the two forms
        // are only reassociation-apart — exp4's ~2ulp is enough to flip a near-tie greedy pick and
        // derail the id lockstep (the exp4 paths are parity-gated by the frozen fixtures instead)
        let prev_vec = get_act_vec()
        set_act_vec(false)
        let prev_dn_route = moe_gpu_dn_route()
        let prev_dnd_route = moe_gpu_dnd_route()
        set_moe_gpu_dn_route(false)
        set_moe_gpu_dnd_route(false)
        with_dasllama_jobque_() {
            let n = 150l
            var prompt <- [for (i in range64(n)); 1000l + (i * 37l) % 5000l]

            set_deltanet_prefill_mode(DnPrefillMode.recurrent)
            var s1 <- make_run_state(c)
            forward_prefill(tr, s1, prompt, n, 0l)
            var ref <- [for (i in range64(c.vocab_size)); s1.logits[i]]

            set_deltanet_prefill_mode(DnPrefillMode.chunked)
            var s2 <- make_run_state(c)
            forward_prefill(tr, s2, prompt, n, 0l)
            var got <- [for (i in range64(c.vocab_size)); s2.logits[i]]

            var tok = argmax_of(ref)
            let prefill_diff = logits_maxdiff(got, ref)
            let prefill_bar = DN_LOGIT_BAR_REL * logits_maxabs(ref)
            to_log(LOG_INFO, "prefill: argmax recurrent {tok} ({ref[tok]}), chunked {argmax_of(got)} ({got[argmax_of(got)]}); logits maxdiff {prefill_diff} (bar {prefill_bar})\n")
            t |> success(prefill_diff <= prefill_bar, "prefill logits: chunked within {prefill_bar} of recurrent (maxdiff {prefill_diff})")
            delete ref
            delete got

            var prev_l1 : array<float>
            for (st in range64(8l)) {
                forward(tr, s1, tok, n + st)
                forward(tr, s2, tok, n + st)
                var inscope l1 <- [for (i in range64(c.vocab_size)); s1.logits[i]]
                var inscope l2 <- [for (i in range64(c.vocab_size)); s2.logits[i]]
                let step_diff = logits_maxdiff(l2, l1)
                let step_bar = DN_LOGIT_BAR_REL * logits_maxabs(l1)
                let a1 = argmax_of(l1)
                let a2 = argmax_of(l2)
                to_log(LOG_INFO, "step {st}: fed {tok}; argmax recurrent {a1} ({l1[a1]}), chunked {a2} ({l2[a2]}); logits maxdiff {step_diff} (bar {step_bar})\n")
                t |> success(step_diff <= step_bar, "step {st} logits: chunked within {step_bar} of recurrent (maxdiff {step_diff})")
                if (st > 0l) {   // the control: the previous step's recurrent logits are a disconnected mechanism the bar must RED
                    let off_diff = logits_maxdiff(l2, prev_l1)
                    t |> success(off_diff > step_bar, "step {st} control: the bar discriminates a one-step-off compare (maxdiff {off_diff} vs bar {step_bar})")
                }
                tok = a1
                delete prev_l1
                prev_l1 <- l1
                delete l2
            }
            delete prev_l1
            delete prompt
            delete s1
            delete s2
        }
        set_moe_gpu_dn_route(prev_dn_route)
        set_moe_gpu_dnd_route(prev_dnd_route)
        set_act_vec(prev_vec)
        set_deltanet_prefill_mode(prev_dn)
        delete tr
    }
}

//! llama.cpp's greedy continuations of the prompt on Qwen3.5-0.8B-Q8_0 (-ngl 0, --temp 0), plain and under
//! `--rope-scaling yarn --rope-scale 4 --yarn-orig-ctx 32768`, logged beside ours; the greedy text is a near-tie
//! discriminator on a model this small (llama.cpp itself parts at the second token between --yarn-orig-ctx
//! 32768 and 262144), so the fold is gated on its numbers and the texts are read, never asserted
let private YARN_PROMPT = "Once upon a time, there was a"
let private YARN_PLAIN_HEAD = " very special kind of animal called a \"puppy\"."
let private YARN_LCPP_HEAD = " young boy named Leo. Leo was a very curious and imaginative child."

//! the fold spelled out in fp64: pair j of a `rot`-wide rotation at `pos`, its frequency divided by the YaRN divisor
def private fold_angle(pos, rot, j : int64; theta, ff : float) : double
    => double(pos) * (1.0lf / pow(double(theta), double(2l * j) / double(rot))) / double(ff)

//! one load's rope-scaling outcome - the rejections and the linear / none kinds, cheap at the config stage
def private load_outcome(path : string; o : RopeScalingOverride) : string {
    set_rope_scaling_override(o)
    var why = ""
    try {
        var m <- load_model_(path, QuantMode.q8)
        why = "loaded: freq_scale {m.config.rope_freq_scale} factors {long_length(m.rope_freqs)} mscale {m.config.rope_mscale}"
        delete m
    } recover {
        why = this_context().last_exception |> rtrim
    }
    return why
}

def private greedy_text(tr : Model; steps : int) : string {
    var prompt <- encode_(tr, YARN_PROMPT, true, false)
    var ids <- greedy_gen(tr, prompt, steps)
    let out = decode_(tr, ids)
    delete ids
    delete prompt
    return out
}

//! the logits of one decode step after the prompt's prefill, under the table or the direct rope path
def private step_logits(tr : Model; tabled : bool) : array<float> {
    var out : array<float>
    let prev = get_rope_table_mode()
    set_rope_table_mode(tabled)
    with_dasllama_jobque_() {
        var s = make_run_state(tr.config)
        var prompt <- encode_(tr, YARN_PROMPT, true, false)
        let n = long_length(prompt)
        forward_prefill(tr, s, prompt, n, 0l)
        forward(tr, s, argmax_of(s.logits), n)
        out <- [for (i in range64(tr.config.vocab_size)); s.logits[i]]
        delete prompt
        delete s
    }
    set_rope_table_mode(prev)
    return <- out
}

[test]
def test_deltanet_yarn_override(t : T?) {
    t |> run("the YaRN override on the partial-rotary 0.8B: the fold lands in the rope row, both rope paths take it, the plain load stays llama.cpp's") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        let prev = rope_scaling_override()
        set_rope_scaling_override(RopeScalingOverride())
        var plain <- load_model_(path, QuantMode.q8)   // the engine entry with the tokenizer; load_gguf leaves it out
        plain.config.seq_len = min(plain.config.seq_len, 2048l)
        t |> success(empty(plain.rope_freqs) && plain.config.rope_freq_scale == 1.0, "the file carries no rope scaling of its own")
        to_log(LOG_INFO, "plain: ours{greedy_text(plain, 20)} | llama.cpp ...{YARN_PLAIN_HEAD}\n")
        var plain_step <- step_logits(plain, true)
        delete plain
        // the rejections and the other kinds, at the config stage of a load
        let bogus = load_outcome(path, RopeScalingOverride(kind = "bogus"))
        t |> success(find(bogus, "is not none | linear | yarn") >= 0, "an unknown kind is refused by name: {bogus}")
        let unscaled = load_outcome(path, RopeScalingOverride(kind = "yarn"))
        t |> success(find(unscaled, "YaRN needs a scaling factor") >= 0, "yarn on a file with no factor and no --rope-scale is refused: {unscaled}")
        let linear = load_outcome(path, RopeScalingOverride(kind = "linear", scale = 2.0))
        t |> equal(linear, "loaded: freq_scale 0.5 factors 0 mscale 1", "linear x2 halves the position and folds nothing else")
        let none = load_outcome(path, RopeScalingOverride(kind = "none"))
        t |> equal(none, "loaded: freq_scale 1 factors 0 mscale 1", "none leaves a key-less file as it is")
        set_rope_scaling_override(RopeScalingOverride(kind = "yarn", scale = 4.0, orig_ctx = 32768l))
        var yarn <- load_model_(path, QuantMode.q8)
        yarn.config.seq_len = min(yarn.config.seq_len, 2048l)
        let c = yarn.config
        let rot = c.rope_dim
        let mscale = 1.0 + 0.1 * log(4.0)
        t |> equal(long_length(yarn.rope_freqs), rot / 2l, "the YaRN divisors run over the rotated span")
        t |> success(abs(c.rope_mscale - mscale) < 1.0e-6, "the magnitude correction lands in rope_mscale ({c.rope_mscale})")
        var want_ff <- yarn_rope_freqs(rot, c.rope_freq_base, 4.0, 32.0, 1.0, 32768l)
        t |> equal(logits_maxdiff(yarn.rope_freqs, want_ff), 0.0, "the divisors are the band over the OVERRIDE's original context, not the file's")
        // the row the decode drivers read, against the fold spelled out: cos(pos * theta^(-2j/rot) / ff_j) * mscale
        var ct, st : array<float>
        let pos = 777l
        build_decode_rope_row(yarn, pos, ct, st)
        var want_c <- [for (j in range64(rot / 2l)); float(cos(fold_angle(pos, rot, j, c.rope_freq_base, yarn.rope_freqs[j])) * double(mscale))]
        var want_s <- [for (j in range64(rot / 2l)); float(sin(fold_angle(pos, rot, j, c.rope_freq_base, yarn.rope_freqs[j])) * double(mscale))]
        let row_bar = 2.0e-3
        let worst = max(logits_maxdiff(ct, want_c), logits_maxdiff(st, want_s))
        t |> success(worst < row_bar, "the decode rope row folds the divisors and the magnitude (maxdiff {worst})")
        var ct_poison <- poisoned_copy(want_c, 0, 0.01)
        t |> success(logits_maxdiff(ct, ct_poison) >= row_bar, "the row bar reds a +0.01 poison on one expected element ({logits_maxdiff(ct, ct_poison)})")
        var yarn_tab <- step_logits(yarn, true)
        var yarn_direct <- step_logits(yarn, false)
        let bar = DN_LOGIT_BAR_REL * logits_maxabs(yarn_tab)
        let paths_diff = logits_maxdiff(yarn_tab, yarn_direct)
        let a_tab = argmax_of(yarn_tab)
        let a_dir = argmax_of(yarn_direct)
        let a_plain = argmax_of(plain_step)
        to_log(LOG_INFO, "yarn step: table argmax {a_tab} '{piece_(yarn, a_tab)}' ({yarn_tab[a_tab]}), direct argmax {a_dir} '{piece_(yarn, a_dir)}' ({yarn_direct[a_dir]}), plain argmax {a_plain} '{piece_(yarn, a_plain)}' ({plain_step[a_plain]}); paths maxdiff {paths_diff}, fold maxdiff {logits_maxdiff(yarn_tab, plain_step)}, bar {bar}\n")
        t |> success(paths_diff <= bar, "the table and the direct rope paths of the attention agree under the fold (maxdiff {paths_diff}, bar {bar})")
        var step_poison <- poisoned_copy(yarn_direct, 0, 2.0 * bar)
        t |> success(logits_maxdiff(yarn_tab, step_poison) > bar, "the bar reds a step one element off by twice the bar ({logits_maxdiff(yarn_tab, step_poison)})")
        let fold_diff = logits_maxdiff(yarn_tab, plain_step)
        let noise = 10.0 * max(paths_diff, 1.0e-3)   //! a fold that did nothing leaves the two loads apart by the paths' own rounding alone
        t |> success(fold_diff > noise, "the fold moves the step's logits past the paths' own noise (maxdiff {fold_diff} vs {noise})")
        to_log(LOG_INFO, "yarn x4 over 32768: ours{greedy_text(yarn, 20)} | llama.cpp ...{YARN_LCPP_HEAD}\n")
        delete yarn_tab
        delete yarn_direct
        delete step_poison
        delete plain_step
        delete want_ff
        delete want_c
        delete want_s
        delete ct_poison
        delete ct
        delete st
        delete yarn
        set_rope_scaling_override(prev)
    }
}

// Model-free: the deltanet session-state sizing at scale — 27B geometry puts the S state at
// 48 recurrent layers x 48 v-heads x 128x128 = 144 MB in ONE grow, past the 64 MB
// max_unreserved_size guard; a widened-conv twin drives the conv history past it the same way.
def private big_deltanet_config(d_conv : int64) : Config {
    var c = Config(
        dim = 64l, hidden_dim = 64l, n_layers = 64l,
        n_heads = 4l, n_kv_heads = 1l, head_size = 16l, kv_dim = 16l,
        vocab_size = 64l, seq_len = 16l,
        hybrid_deltanet = true, q_gated = true,
        ssm_d_conv = d_conv, ssm_d_state = 128l, ssm_n_group = 16l,
        ssm_dt_rank = 48l, ssm_d_inner = 48l * 128l)
    for (l in range64(c.n_layers)) {
        if (((l + 1l) % 4l) != 0l) {   // the 27B interleave: every 4th layer is full attention
            c.recr_mask |= 1ul << uint64(l)
        }
    }
    return c
}

// Model-free: the session's deltanet ownership token and the release seam it drives through the
// tier's registry - a counting hook installed through set_moe_gpu_dn_state_hooks sees one
// release per position-zero reset, per speculative rollback, and per delete, each carrying the
// session's own state range.
def private small_deltanet_config() : Config {
    var c = Config(
        dim = 64l, hidden_dim = 64l, n_layers = 4l,
        n_heads = 4l, n_kv_heads = 1l, head_size = 16l, kv_dim = 16l,
        vocab_size = 64l, seq_len = 16l,
        hybrid_deltanet = true, q_gated = true,
        ssm_d_conv = 4l, ssm_d_state = 16l, ssm_n_group = 2l,
        ssm_dt_rank = 4l, ssm_d_inner = 4l * 16l)
    for (l in range64(3l)) {
        c.recr_mask |= 1ul << uint64(l)
    }
    return c
}

var private g_release_calls = 0
var private g_release_lo = 0ul
var private g_release_hi = 0ul

def private counting_release(state_lo : uint64; state_hi : uint64) {
    g_release_calls++
    g_release_lo = state_lo
    g_release_hi = state_hi
}

[test]
def test_deltanet_owner_release(t : T?) {
    t |> run("the session's deltanet owner token releases its own state range on reset, rollback and delete") @(t : T?) {
        return if (!jit_only(t))
        let prev = moe_gpu_dn_state_hooks()
        set_moe_gpu_dn_state_hooks(prev.flush, prev.inval, @@counting_release)
        g_release_calls = 0
        let c = small_deltanet_config()
        var s <- make_run_state(c)
        var lo = 0ul
        lo = intptr(unsafe(addr(s.dn_state[0])))
        let hi = lo + uint64(long_length(s.dn_state) * 4l)
        t |> equal(s.dn_owner.state_lo, lo, "the token's low bound is the state's host address")
        t |> equal(s.dn_owner.state_hi, hi, "the token's high bound is the state's end")
        dn_reset(s)
        t |> equal(g_release_calls, 1, "a position-zero reset releases once")
        t |> equal(g_release_lo, lo)
        t |> equal(g_release_hi, hi)
        mtp_state_restore(s)
        t |> equal(g_release_calls, 2, "a speculative rollback releases once")
        t |> equal(g_release_lo, lo)
        delete s
        t |> equal(g_release_calls, 3, "the session's delete releases once, through the token's finalizer")
        t |> equal(g_release_lo, lo)
        t |> equal(g_release_hi, hi)
        set_moe_gpu_dn_state_hooks(prev.flush, prev.inval, prev.release)
    }
}

[test]
def test_deltanet_state_sizing(t : T?) {
    t |> run("deltanet session state at 27B geometry clears max_unreserved_size") @(t : T?) {
        return if (!jit_only(t))
        let c = big_deltanet_config(4l)
        var s <- make_run_state(c)
        t |> equal(long_length(s.dn_state), 48l * 48l * 128l * 128l)
        delete s
        // conv-history twin: d_conv 64 puts dn_conv_state at 63 x conv_dim x 48 = ~124 MB
        let cw = big_deltanet_config(64l)
        var sw <- make_run_state(cw)
        t |> equal(long_length(sw.dn_conv_state), 63l * dn_conv_dim(cw) * 48l)
        delete sw
    }
}
