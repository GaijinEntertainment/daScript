options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: each test frees its model/session before the next loads, so multi-GB weights never accumulate in the process

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — engine + arch registrations (incl. forward_mtp / generate_mtp_greedy)

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // CPU-correctness test — prefills on the CPU by design
}
require dasllama/dasllama_math   // parallel_argmax (engine-level, not re-exported by the facade)
require daslib/jobque_boost
require daslib/fio
require _model_tier   // models_dir() + model_available() presence/size-tier gate (see the file's header)

// MTP/NextN self-speculative decode gates, on the qwen35-family -MTP- GGUFs (the files ship one
// extra trunk-shaped draft block at layer index n_layers plus nextn.{eh_proj,enorm,hnorm,...}).
// Three layers of gate, mirroring how the feature was proven in:
//   1. loader: nextn extras load, and with spec OFF the trunk continues the fixture EXACTLY like
//      the plain (non-MTP) file — trunk equality was proven via llama.cpp simple_ids on both files;
//   2. output-invariance: generate_mtp_greedy() == generate() token-for-token — greedy self-spec
//      changes wall time, never tokens (accept path reads batch-kernel logits, so this holds only
//      under the PINNED bit-exact kernels, same reasoning as test_parity's parity_gen_ok);
//   3. rollback: a poisoned 2-row verify batch + mtp_state_restore replays plain decode exactly
//      (the deltanet conv+S state is the only thing a rejected draft leaves behind — KV rows get
//      overwritten by construction).
// The GGUFs are gitignored multi-GB files, so each case skips cleanly when its model is absent.

// Qwen3.5-0.8B word-counting fixture: "Count from one to twenty: one, two, three," and its greedy
// 40-token continuation (" four, five, ... twenty.\nCount from one to twenty"). Frozen from
// llama.cpp simple_ids on the PLAIN Qwen3.5-0.8B-Q8_0 file; simple_ids on the -MTP- file produced
// the identical PROMPT+GEN ids, so this single fixture gates the loader AND trunk equality of the
// MTP conversion. Confident list momentum — near-tie-free under the pinned kernels.
let MTP_PROMPT <- [2427l, 494l, 799l, 310l, 16570l, 25l, 799l, 11l, 1330l, 11l, 2250l, 11l]
let MTP_WANT <- [2943l, 11l, 4097l, 11l, 4590l, 11l, 7840l, 11l, 7810l, 11l, 11292l, 11l, 5600l, 11l,
                 42744l, 11l, 28279l, 11l, 58614l, 11l, 60163l, 11l, 35442l, 11l, 56091l, 11l, 78592l,
                 11l, 59942l, 11l, 90600l, 11l, 16570l, 13l, 198l, 2427l, 494l, 799l, 310l, 16570l]

def private same(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (x, y in a, b) {
        if (x != y) {
            return false
        }
    }
    return true
}

// Pin the bit-exact kernel modes around `body` and restore the previous modes — classic prefill
// attention (not flash), scalar libm activations (not exp4), per-token recurrent deltanet prefill
// (not chunked). Token-exact gates must not ride near-tie kernel variance; see parity_gen_ok.
def private with_pinned_kernels(body : block) {
    let prev_attn = get_prefill_attn_mode()
    set_prefill_attn_mode(AttnPrefillMode.classic)
    let prev_act = get_act_vec()
    set_act_vec(false)
    let prev_dn = get_deltanet_prefill_mode()
    set_deltanet_prefill_mode(DnPrefillMode.recurrent)
    invoke(body)
    set_prefill_attn_mode(prev_attn)
    set_act_vec(prev_act)
    set_deltanet_prefill_mode(prev_dn)
}

// Plain greedy reference over one fresh session (generate()'s echo contract: prompt[1..] then the
// continuation).
def private gen_plain(tr : Model; prompt : array<int64>; total : int) : array<int64> {
    var out : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(tr.config)
        out <- generate(tr, s, prompt, total)
        delete s
    }
    return <- out
}

// Depth-1 self-spec greedy over one fresh session; accept telemetry lands in nd/na.
def private gen_spec(tr : Model; prompt : array<int64>; total : int; var nd, na : int64&) : array<int64> {
    var out : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(tr.config)
        out <- generate_mtp_greedy(tr, s, prompt, int64(total), nd, na)
        delete s
    }
    return <- out
}

// Spec-off pinned generate-and-compare (parity_gen_ok's shape). Helpers like this stay plain
// functions on purpose: they JIT-compile, so the deep forward chain runs on the native stack —
// calling the engine from the [test] run-lambda's interpreted frame overflows its 16K das stack.
def private specoff_gen_ok(tr : Model; prompt, want : array<int64>) : bool {
    var ok = false
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(tr.config)
        let ids <- generate(tr, s, prompt, length(prompt) + length(want))
        let echo = length(prompt) - 1
        let got <- [for (i in range(length(want))); ids[echo + i]; where echo + i < length(ids)]
        ok = same(got, want)
        delete s
    }
    return ok
}

// The forced-reject rollback sequence (see test_mtp_reject_rollback for the WHY): returns the
// tokens decoded after the poisoned-verify rollback (first = the prefill argmax) and fills
// `want` with the plain-decode reference slice they must equal. Free of T? like specoff_gen_ok —
// a T? param keeps the helper off the JIT, and the engine chain overflows the interpreter stack.
def private reject_rollback_ids(tr : Model; n_check : int64; var want : array<int64>) : array<int64> {
    var got : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        let np = long_length(MTP_PROMPT)
        // plain-decode reference (echo convention: ids[np-1] is the first generated token)
        var s1 = make_run_state(tr.config)
        let ref_ids <- generate(tr, s1, MTP_PROMPT, int(np + n_check + 1l))
        delete s1
        delete want
        want <- [for (i in range64(n_check + 1l)); ref_ids[np - 1l + i]]   // nolint:PERF030 — released on the line above
        var s = make_run_state(tr.config)
        forward_prefill(tr, s, MTP_PROMPT, np, 0l)
        let tok = parallel_argmax(s.logits, tr.config.vocab_size)
        got |> push(tok)
        mtp_state_snapshot(s)
        var vbatch <- [tok, 42l]   // 42 = arbitrary garbage draft
        forward_prefill(tr, s, vbatch, 2l, np)
        mtp_state_restore(s)
        forward(tr, s, tok, np)   // re-advance through the accepted token alone
        var pos = np + 1l
        for (_i in range64(n_check)) {
            let next = parallel_argmax(s.logits, tr.config.vocab_size)
            got |> push(next)
            forward(tr, s, next, pos)
            pos++
        }
        delete s
        delete vbatch
    }
    return <- got
}

// Run the plain-vs-spec invariance pair on a loaded model and assert token-for-token equality
// plus sane accept telemetry. `tag` distinguishes the arms in the report.
def private invariance_ok(t : T?; tr : Model; prompt : array<int64>; n_gen : int; tag : string) {
    let total = length(prompt) + n_gen
    let ref_ids <- gen_plain(tr, prompt, total)
    var nd = 0l
    var na = 0l
    let spec_ids <- gen_spec(tr, prompt, total, nd, na)
    t |> success(same(ref_ids, spec_ids), "{tag}: spec decode token-for-token vs plain")
    t |> success(nd > 0l, "{tag}: the draft head actually drafted ({nd} drafts)")
}

[test]
def test_mtp_loader(t : T?) {
    t |> run("MTP/NextN loader (Qwen3.5-0.8B-MTP Q8): nextn extras + spec-off trunk parity") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
        if (!model_available(t, path)) return
        var tr <- load_gguf(path, QuantMode.q8)
        // block_count INCLUDES the draft block: n_layers stays the trunk count, nextn rides extra
        t |> equal(tr.config.n_layer_nextn, 1l)
        t |> equal(tr.config.n_layers, 24l)
        t |> success(tr.mtp_ehproj_off >= 0l && tr.mtp_enorm_off >= 0l && tr.mtp_hnorm_off >= 0l,
                     "nextn eh_proj/enorm/hnorm loaded")
        t |> success(tr.mtp_headnorm_off >= 0l, "nextn shared_head_norm loaded (qwen35 MTP files ship it)")
        tr.config.seq_len = 2048l
        // spec OFF: the -MTP- file's trunk must continue the fixture exactly like the plain file
        with_pinned_kernels() {
            t |> success(specoff_gen_ok(tr, MTP_PROMPT, MTP_WANT), "spec-off token-for-token vs the plain-file oracle")
        }
        delete tr
    }
}

[test]
def test_mtp_output_invariance(t : T?) {
    t |> run("MTP self-spec greedy decode is output-invariant (Qwen3.5-0.8B-MTP Q8)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
        if (!model_available(t, path)) return
        var tr <- load_model_(path, QuantMode.q8)   // load_model: the prose arm encodes in-test
        tr.config.seq_len = 2048l
        with_pinned_kernels() {
            // the counting fixture is accept-heavy: exercises the 2-row batched accept path + backfill
            t |> invariance_ok(tr, MTP_PROMPT, 48, "counting")
            // prose is reject-heavier: exercises rollback + single-token re-advance mid-loop
            let prose <- encode(tr, "Once upon a time, there was a", true, false)
            t |> invariance_ok(tr, prose, 40, "prose")
        }
        delete tr
    }
}

[test]
def test_mtp_reject_rollback(t : T?) {
    t |> run("MTP rollback: poisoned 2-row verify + restore replays plain decode exactly") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
        if (!model_available(t, path)) return
        var tr <- load_gguf(path, QuantMode.q8)
        tr.config.seq_len = 2048l
        // manual reject flow, exactly as generate_mtp_greedy's reject arm runs it: the draft is
        // a fixed garbage token, so the verify batch advances the deltanet state past a WRONG
        // row; restore + one plain forward must leave the state bit-identical to never having
        // seen the poisoned batch — every subsequent plain-decode token must match a reference
        // run. (Truth vs draft isn't even compared — an unconditional reject is the hardest
        // case, and the KV row the poisoned batch wrote at np+1 gets overwritten by the next
        // decode, by construction.)
        with_pinned_kernels() {
            var want : array<int64>
            let got <- reject_rollback_ids(tr, 6l, want)
            t |> success(same(got, want), "post-rollback decode token-for-token vs plain")
        }
        delete tr
    }
}

// Qwen3.6-27B-MTP Q4_K_M — the 64-trunk-layer carrier: its draft block sits at layer index 64, the
// exact index where the pre-fix per-layer masks wrapped (mask >> 64 == mask >> 0 on x86) — this
// arm regresses that whole bug class (layer_is_recurrent / layer_is_sliding / wv_offs guards)
// besides being the model the feature is FOR (1.39x decode when frozen). Large tier (~16 GB):
// DASLLAMA_PARITY_FULL=1.
[test]
def test_mtp_invariance_27b(t : T?) {
    t |> run("MTP self-spec output-invariance (Qwen3.6-27B-MTP Q4_K_M, draft block at layer 64)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "Qwen3.6-27B-MTP-Q4_K_M.gguf")
        if (!model_available(t, path)) return
        var tr <- load_gguf(path, QuantMode.q8)
        t |> equal(tr.config.n_layer_nextn, 1l)
        t |> equal(tr.config.n_layers, 64l)
        tr.config.seq_len = 2048l
        with_pinned_kernels() {
            t |> invariance_ok(tr, MTP_PROMPT, 48, "counting-27b")
        }
        delete tr
    }
}

// GLM-4.5-Air Q4_K_M — the only NON-RECURRENT MTP model we have, so this is the sole arm covering
// mtp_spec_eval's `recr_mask == 0` reject branch (a rejected draft consumes the verify's own row-0
// logits + h0 instead of paying a second full forward; every qwen35 MTP file is a deltanet hybrid
// and takes the restore-and-re-advance branch instead). It is also the first arch combining
// attn_qkv_bias with a NextN block — the shape where the qkv/out bias arrays were sized by the
// TRUNK layer count, so blk.<n_layers>.attn_{q,k,v}.bias never loaded and the draft read past the
// end of t.bq. And glm4moe ships nextn.embed_tokens/shared_head_head as copies of the trunk's own
// tables, which the loader shape-checks rather than disabling the head over.
// Large tier (~73 GB, 2 shards): DASLLAMA_PARITY_FULL=1.
[test]
def test_mtp_invariance_glm(t : T?) {
    t |> run("MTP self-spec output-invariance (GLM-4.5-Air Q4_K_M, non-recurrent reject path)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "glm45air/Q4_K_M/GLM-4.5-Air-Q4_K_M-00001-of-00002.gguf")
        if (!model_available(t, path)) return
        var tr <- load_model_(path, QuantMode.q8)   // load_model: this arm encodes its prompt in-test
        t |> equal(tr.config.n_layer_nextn, 1l)    // head ENABLED, not disabled over the table copies
        t |> equal(tr.config.n_layers, 46l)        // block_count 47 - nextn 1
        // the two properties that make this arm load-bearing — asserted so a future file losing
        // either tells us the coverage moved instead of silently passing
        t |> equal(tr.config.recr_mask, 0ul)
        t |> success(tr.config.attn_qkv_bias, "glm4moe carries qkv bias (the layers_ld sizing case)")
        tr.config.seq_len = 2048l
        with_pinned_kernels() {
            // A CODE prompt on purpose. Strict token-identity is only meaningful on a NEAR-TIE-FREE
            // fixture: on K-quant planes the accept path reads BATCH-kernel logits (mm_gemm/mm_b_f)
            // while plain decode reads GEMV (mm_kq), and those differ in reduction order — a
            // legitimate FP difference that `with_pinned_kernels` does NOT cover (it pins attention
            // mode, activations and deltanet, not the matmul family). At Q4_K that delta is far
            // below the model's own quantization error, so on tie-dense PROSE the two paths pick
            // differently at ties and identity fails for no defect (measured: prose diverges every
            // run, this fixture is bit-identical at 95% accept). Code is strongly determined ⇒ no
            // ties to break ⇒ identity is a real assertion. Same reasoning as the 27B arm's
            // deliberately near-tie-free counting fixture.
            // FOLLOW-UP: a margin-gated arm can cover prose — assert that any divergence sits below
            // the quantization noise floor; mtp_verify_logits already computes the margin.
            let code <- encode(tr, "def fibonacci(n):", true, false)
            let total = length(code) + 40
            var nd = 0l
            var na = 0l
            let ref_ids <- gen_plain(tr, code, total)
            let spec_ids <- gen_spec(tr, code, total, nd, na)
            t |> success(same(ref_ids, spec_ids), "code-glm: spec decode token-for-token vs plain")
            t |> success(nd > 0l, "code-glm: the draft head actually drafted ({nd} drafts)")
            // a 100%-accept run would leave the reject branch untouched and the arm would prove
            // nothing about it — so require that at least one draft was actually rejected
            t |> success(na < nd, "code-glm: {nd - na} of {nd} drafts rejected — reject branch covered")
        }
        delete tr
    }
}
