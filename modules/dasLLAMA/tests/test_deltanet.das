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
require dasllama/dasllama_math  // setup_dasllama_jobque_ (the engine-standard jobque config)
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

let private DN_LOGIT_BAR_REL = 0.04   // of the step's max |logit|; the two forms measure ~0.015 of it apart

def private argmax_of(v : array<float>) : int64 {
    var best = -1e30
    var bi = 0l
    for (i in range64(long_length(v))) {
        if (v[i] > best) {
            best = v[i]
            bi = i
        }
    }
    return bi
}

[test]
def test_deltanet_chunked_prefill(t : T?) {
    t |> run("deltanet chunked prefill == recurrent (Qwen3.5-0.8B, multi-chunk)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
        if (!model_available(t, path)) return
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
        with_job_que() {
            setup_dasllama_jobque_()
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
                var l1 <- [for (i in range64(c.vocab_size)); s1.logits[i]]
                var l2 <- [for (i in range64(c.vocab_size)); s2.logits[i]]
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
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let prev = moe_gpu_dn_state_hooks()
        set_moe_gpu_dn_state_hooks(prev.flush, prev.inval, @@counting_release)
        g_release_calls = 0
        let c = small_deltanet_config()
        var s <- make_run_state(c)
        var lo = 0ul
        unsafe {
            lo = intptr(addr(s.dn_state[0]))
        }
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
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
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
