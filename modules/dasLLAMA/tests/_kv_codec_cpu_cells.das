options gen2
options _dasllama_internal = true

module _kv_codec_cpu_cells shared public

//! the K/V codec's CPU cells on a model its caller loaded; no entry loads one

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_math   // the fused knob + the backend pin's name
require dasllama/dasllama_core   // create_session / create_kv_pool: the codec a carrier's session and pool serve
require daslib/jobque_boost
require _model_tier   // kv_rows_differ
require _compares   // logits_maxdiff
require _cpu_gpu_model   // with_ctx_cut: the cells' cut contexts

// The K/V codec's CPU cells on a loaded model: the q8_0 cache's teacher-forced logits and its fused-vs-standard decode, the
// q8_0 cache's classic prefill against per-token decode (SmolLM2-135M, the small head_size-64 carrier - stories15M's 48 is
// rejected by the create-time 32-block check), and the step-down rule a session, a pool and a chat share (Qwen2.5-0.5B's
// Q/K/V bias takes tq4 to q8_0; bias-free Qwen3-0.6B keeps tq4). Their models' blocks run them.

let private FORCED_STEPS = 24   //! the teacher-forced stream's decode steps

//! teacher-forced per-step logits under one KV config: the SAME deterministic token stream into a fresh session, all
//! FORCED_STEPS logit rows out - trajectory-compounding-free comparisons
def private forced_logits(tr : Model; kdt, vdt : KVDtype; var out : array<float>) {
    let c = tr.config
    out |> resize(int64(FORCED_STEPS) * c.vocab_size)
    var s <- make_run_state(c, kdt, vdt)
    var pos = 0l
    for (st in range(FORCED_STEPS)) {
        forward(tr, s, int64(st * 37 % 100 + 2), pos)
        pos++
        let base = int64(st) * c.vocab_size
        for (i in range64(c.vocab_size)) {
            out[base + i] = s.logits[i]
        }
    }
    delete s
}

//! On SmolLM2-135M `tr`, loaded and run on its form's portable pin (`cpu_cells_form(backend = "portable")`: the rows core every
//! host shares - the fused gate needs one, and the load lays the q8 rows out for it), its context cut to 128: teacher-forced
//! logits over q8_0 and q8_0-K/f16-V caches within 4.0 of the f16 cache's, and fused and standard decode under q8_0 bit for bit
def kv_q8_decode_cell(t : T?; var tr : Model) {
    with_ctx_cut(tr, 128l) {
        unsafe {
            t |> run("q8_0 KV: teacher-forced logits track f16, fused==std bit-exact under q8 (needs SmolLM2-135M)") @capture(& tr)(t : T ?) {
                t |> equal(kernel_backend_pin(), "portable", "the cell runs on its form's portable pin")
                with_threaded_jobque() {
                    var lg16, lg8, lg8m, lg8f : array<float>
                    set_fused_decode(false)
                    forced_logits(tr, KVDtype.f16, KVDtype.f16, lg16)
                    forced_logits(tr, KVDtype.q8_0, KVDtype.q8_0, lg8)
                    forced_logits(tr, KVDtype.q8_0, KVDtype.f16, lg8m)   // K quantized, V half
                    set_fused_decode(true)
                    forced_logits(tr, KVDtype.q8_0, KVDtype.q8_0, lg8f)
                    // q8 vs f16, teacher-forced: per-step logits stay within the structural bound (greedy
                    // trajectories are NOT compared — on a 135M model a near-tie argmax flips under f16
                    // noise too, and one flip compounds the whole stream). Measured here: q8-vs-f32 noise
                    // is the same order as f16-vs-f32 (~0.4-1.8 worst per step).
                    let w8 = logits_maxdiff(lg16, lg8)
                    t |> success(w8 < 4.0, "q8/q8-vs-f16 teacher-forced logits within the structural bound (worst |diff| = {w8})")
                    let w8m = logits_maxdiff(lg16, lg8m)
                    t |> success(w8m < 4.0, "q8-K/f16-V-vs-f16 within the structural bound (worst |diff| = {w8m})")
                    // fused vs std, BOTH under q8: identical kernels + identical query quants -> bit-exact
                    var lg_bad = 0
                    for (i in range(length(lg8))) {
                        if (lg8[i] != lg8f[i]) {
                            lg_bad++
                        }
                    }
                    t |> equal(0, lg_bad)
                    delete lg16
                    delete lg8
                    delete lg8m
                    delete lg8f
                }
            }
        }
    }
}

//! the run `title`: with a `kdt` KV cache, a 16-token classic prefill on `tr` leaves the logits and both cache planes
//! bit-identical to decoding the same tokens one at a time, dispatch forced threaded
def prefill_vs_decode_cell(t : T?; tr : Model; title : string; kdt : KVDtype) {
    unsafe {
        t |> run(title) @capture(& tr)(t : T ?) {
            let prev_mode = get_prefill_attn_mode()
            with_threaded_jobque() {
                set_prefill_attn_mode(AttnPrefillMode.classic)
                let c = tr.config
                var prompt <- [for (i in range(16)); int64(i % 50 + 2)]
                let n = long_length(prompt)
                var sp <- make_run_state(c, kdt, kdt)
                forward_batch(tr, sp, prompt, n, 0l)
                var sd <- make_run_state(c, kdt, kdt)
                var pos = 0l
                for (tok in prompt) {
                    forward(tr, sd, tok, pos)
                    pos++
                }
                var lg_bad = 0
                for (i in range64(c.vocab_size)) {
                    if (sp.logits[i] != sd.logits[i]) {
                        lg_bad++
                    }
                }
                t |> equal(0, lg_bad)
                let kv_bad = kv_rows_differ(sp, sd, c, n)
                t |> equal(kv_bad.k + kv_bad.v, 0l, "both cache planes bit-exact over the {n} rows written")
                delete sp
                delete sd
                delete prompt
            }
            set_prefill_attn_mode(prev_mode)
        }
    }
}

//! On SmolLM2-135M `tr`, its context cut to 64: the prefill-vs-decode cell under a q8_0 KV cache
def kv_q8_prefill_vs_decode_cell(t : T?; var tr : Model) {
    with_ctx_cut(tr, 64l) {
        prefill_vs_decode_cell(t, tr, "q8_0 KV: classic prefill == per-token decode, logits + cache bit-exact (needs SmolLM2-135M)", KVDtype.q8_0)
    }
}

//! On Qwen2.5-0.5B `m`, whose attention carries a Q/K/V bias: a tq4 ask serves at q8_0 in a session, a pool and a chat, and
//! q8_0 asked stays q8_0
def kv_codec_step_down_cell(t : T?; m : Model) {
    t |> success(m.config.attn_qkv_bias, "Qwen2.5 carries the Q/K/V bias the rule reads")
    var s <- create_session(m, KVDtype.tq4)
    t |> equal(s.kv_dtype_k, KVDtype.q8_0, "a tq4 session on Qwen2.5 serves at q8_0")
    t |> equal(s.kv_dtype_v, KVDtype.q8_0, "both planes")
    var s8 <- create_session(m, KVDtype.q8_0)
    t |> equal(s8.kv_dtype_k, KVDtype.q8_0, "q8_0 asked stays q8_0")
    var pool <- create_kv_pool(m, 64l, KVDtype.tq4)
    t |> equal(pool.kdt, KVDtype.q8_0, "a tq4 pool on Qwen2.5 pages at q8_0")
    delete pool
    var chat <- create_chat(m, "", 8l, KVDtype.tq4)
    t |> equal(chat.session.kv_dtype_k, KVDtype.q8_0, "a tq4 chat on Qwen2.5 serves at q8_0: the rule sits in the shared constructor every path takes")
    delete chat
    delete s8
    delete s
}

//! the step-down rule's control on bias-free Qwen3-0.6B `m`: a tq4 session keeps tq4
def kv_codec_keeps_tq4_cell(t : T?; m : Model) {
    var s3 <- create_session(m, KVDtype.tq4)
    t |> equal(s3.kv_dtype_k, KVDtype.tq4, "control: Qwen3, no bias, keeps tq4")
    delete s3
}
