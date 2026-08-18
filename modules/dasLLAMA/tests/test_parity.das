options gen2
options persistent_heap   // + explicit deletes below: each arm frees its model/session before the next loads, so multi-GB weights never accumulate in the process
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — public-path tests exercise the API through it (engine + arch registrations + chat)
require llvm/daslib/f16_cvt   // f16_to_f32 — the tied-cls arm reads the live scale plane
require daslib/jobque_boost
require daslib/fio
require _model_tier   // models_dir() + model_available() presence/size-tier gate (see the file's header)
require ../performance/model_specs.das   // the model-set table — parity evidence lives there as DATA

// Frozen token-for-token parity gates for the arches stories15M (test_forward.das) doesn't cover.
// Evidence is DATA in model_specs.das — pinned ids (or a text prompt encoded here) + the arms it was
//   frozen under; regenerate with harness/parity.sh. The board's --parity pregate reads the same rows.
// Models are gitignored multi-GB files: absent = clean skip (a real local regression, a no-op in CI).

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

// The pinned-kernel generate-and-compare core: greedily continue `prompt` on a loaded model and
// return whether the continuation matches `want` token-for-token. Kept free of T? so the [test]'s
// run()-lambda captures no array by move.
def private parity_gen_ok(tr : Model; prompt, want : array<int64>; dn_chunked : bool = false; eyeball : bool = false) : bool {
    let n_gen = length(want)
    var ok = false
    // Token-exact oracle gate => run the BIT-EXACT classic prefill attention, not the (default) flash:
    // flash's online-softmax FP reorder is not bit-exact and can flip a greedy token under the
    // interpreter (whose FP differs from JIT, where the fixtures were frozen). Flash staying within
    // tolerance is what test_flash covers. Restore the prior mode so we don't leak global state.
    let prev_attn = get_prefill_attn_mode()
    set_prefill_attn_mode(AttnPrefillMode.classic)
    // ...and the scalar libm silu/gelu, not the default exp4 gate (~2 ulp) — same token-exact-oracle
    // reasoning as the attention pin above. exp4 staying within tolerance is covered by test_silu.
    let prev_act = get_act_vec()
    set_act_vec(false)
    // ...and the per-token recurrent deltanet prefill (the form the hybrid fixtures were frozen
    // in), not the (default) chunked — same reasoning again; chunked gets its own explicit arms.
    let prev_dn = get_deltanet_prefill_mode()
    set_deltanet_prefill_mode(dn_chunked ? DnPrefillMode.chunked : DnPrefillMode.recurrent)
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(tr.config)
        let ids <- generate(tr, s, prompt, length(prompt) + n_gen)
        // generate echoes prompt[1..] then the continuation, so the first generated token is at
        // index length(prompt)-1 (the same slice harness/parity.das compares)
        let echo = length(prompt) - 1
        let got <- [for (i in range(n_gen)); ids[echo + i]; where echo + i < length(ids)]
        ok = same(got, want)
        if (eyeball) {   // opt-in: needs a tokenizer-bearing load (load_model_, not load_gguf)
            to_log(LOG_INFO, "eyeball want: {eyeball_text(tr, want)}\n")
            to_log(LOG_INFO, "eyeball got:  {eyeball_text(tr, got)}\n")
        }
        delete s
    }
    set_prefill_attn_mode(prev_attn)
    set_act_vec(prev_act)
    set_deltanet_prefill_mode(prev_dn)
    return ok
}

// cls_q8 invariant: a tied Q8 load of a Q8_0 embedding drops the fp32 table (the classifier
// matmuls the disk quants; rows dequant on demand from the linear transcode at emb_q8_off) — and
// those rows must be bit-identical to the fp32 table a fp32 load decodes from the same disk data.
// SmolLM2-135M is the smallest tied model.
[test]
def test_parity_tied_cls_q8_rows(t : T?) {
    t |> run("tied Q8 load: cls_q8 embedding rows bit-match the fp32 table") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!model_available(t, path)) return
        var t32 <- load_gguf(path, QuantMode.fp32)
        var t8 <- load_gguf(path, QuantMode.q8)
        t |> success(!t32.cls_q8, "fp32 load keeps the fblob table")
        t |> success(t8.cls_q8, "tied Q8_0 load takes the cls_q8 path")
        let dim = t8.config.dim
        var bad = 0l
        for (token in [0l, 1l, 1000l, t8.config.vocab_size - 1l]) {
            for (i in range64(dim)) {
                let el = t8.emb_q8_off + token * dim + i
                let want = t32.fblob[t32.tok_emb_off + token * dim + i]
                // read whichever scale plane is live (wscale_f16 frees the f32 one; the widen
                // is the exact disk value, so the bit-match claim is unchanged)
                let sc = t8.wscale_f16 ? f16_to_f32(uint(t8.qscales16[el / 32l])) : t8.qscales[el / 32l]
                if (float(t8.qblob[el]) * sc != want) {
                    bad++
                }
            }
        }
        t |> success(bad == 0l, "embedding rows bit-identical ({bad} mismatches)")
        delete t32
        delete t8
    }
}

// the deltanet-plane engage count for one plane kind — a silently-demoted tag would otherwise
// pass the parity green on the q8 fallback rail
def private dn_kq_engaged(fmts : array<KqFmt>) : int64 {
    var n = 0l
    for (f in fmts) {
        if (f != KqFmt.q8) {
            n++
        }
    }
    return n
}

// One spec's arms off ONE model load. load_model_ (not load_gguf) so the tokenizer rides along:
// text-form prompts encode here, and every compare logs its decoded-text eyeball lines.
def private parity_spec_arms(t : T?; spec : ModelSpec; path : string) {
    let prev_kq = get_kquant_native()
    if (spec.parity.kq_native) {
        set_kquant_native(true)
    }
    var tr <- load_model_(path, spec.parity.quant)
    if (spec.parity.kq_native) {
        set_kquant_native(prev_kq)
    }
    // Cap the KV context: only ~(prompt + gen) positions are used, and the KV stride doesn't
    // change the computed tokens — Llama-3's native 131072 would just burn ~8GB of KV RAM.
    if (tr.config.seq_len > 2048l) {
        tr.config.seq_len = 2048l
    }
    var prompt <- (empty(spec.parity.prompt_text) ? clone_to_move(spec.parity.prompt)
        : encode(tr, spec.parity.prompt_text, true, false))
    if (spec.parity.require_dn_kq) {
        t |> success(dn_kq_engaged(tr.dnqkv_fmt) > 0l && dn_kq_engaged(tr.dngate_fmt) > 0l && dn_kq_engaged(tr.dnout_fmt) > 0l,
            "{spec.file} dn planes kq-native (not demoted)")
    }
    t |> success(parity_gen_ok(tr, prompt, spec.parity.gen, [eyeball = true]), "{spec.file} token-for-token")
    if (spec.parity.dn_chunked) {
        t |> success(parity_gen_ok(tr, prompt, spec.parity.gen, [dn_chunked = true, eyeball = true]),
            "{spec.file} chunked-prefill token-for-token")
    }
    if (spec.parity.moe_reference) {
        set_moe_grouped_prefill(false)
        t |> success(parity_gen_ok(tr, prompt, spec.parity.gen, [eyeball = true]),
            "{spec.file} reference-prefill token-for-token")
        set_moe_grouped_prefill(true)
    }
    if (spec.parity.moe_per_expert) {
        set_moe_fused_expert_gemms(false)
        t |> success(parity_gen_ok(tr, prompt, spec.parity.gen, [eyeball = true]),
            "{spec.file} per-expert-prefill token-for-token")
        set_moe_fused_expert_gemms(true)
    }
    delete prompt
    delete tr
}

[test]
def test_parity_specs(t : T?) {
    var files : array<string>
    var all <- model_specs()
    for (s in all) {
        // gpt-oss runs through its own shared-load arm below (both fixtures on ONE load — the
        // ~13GB native-mx4 resident is the suite's biggest; loading twice OOMs a 64GB box)
        if (!empty(s.parity.gen) && s.file != "gpt-oss-20b-mxfp4.gguf") {
            files |> push(s.file)
        }
    }
    delete all
    for (file in files) {
        t |> run("pinned-greedy parity: {file}") @(t : T?) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            var inscope spec <- spec_by_file(file)
            let path = path_join(models_dir(), spec.file)
            if (!model_available(t, path)) return
            parity_spec_arms(t, spec, path)
        }
    }
    delete files
}

// The second gpt-oss fixture, window-engaged: "1, 2, …, 150," (449 tokens ≫ the 128-token window,
// so sliding layers genuinely clip — kstart > 0) ENCODED IN-TEST through the gpt-4o pre-tokenizer
// (verified id-for-id against the oracle's tokenization), gating the tokenizer AND the forward,
// same pattern as the gemma4 fixture. Test-local on purpose: the table carries the model's short
// counting fixture (the board pregate reads that one); this is the suite's extra window arm.
let GPTOSS_LONG_GEN <- [220l, 17395l, 11l, 220l, 17336l, 11l, 220l, 17773l, 11l, 220l,
                        19145l, 11l, 220l, 17385l, 11l, 220l, 18369l, 11l, 220l, 19676l,
                        11l, 220l, 19548l, 11l, 220l, 19218l, 11l, 220l, 9444l, 11l,
                        220l, 18881l, 11l, 220l, 18694l, 11l, 220l, 17666l, 11l, 220l]


// gpt-oss-20b (arch "gpt-oss", native-MXFP4 expert planes — attn/cls Q8) — the wave-5 stack:
// per-head attention sinks, YaRN rope (factor 32 / 4096), alternating 128-token SWA,
// softmax-after-top-k routing with router + per-expert + QKV + output-projection biases,
// clamped SwiGLU. Both fixtures share ONE model load.
[test]
def test_parity_gptoss_20b(t : T?) {
    t |> run("gpt-oss forward (gpt-oss-20b native-MXFP4 experts, sinks + YaRN + MoE biases; short + window-engaged)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        var inscope spec <- spec_by_file("gpt-oss-20b-mxfp4.gguf")
        let path = path_join(models_dir(), spec.file)
        if (!model_available(t, path)) return
        var tr <- load_model_(path, QuantMode.q8)
        if (tr.config.seq_len > 2048l) {
            tr.config.seq_len = 2048l
        }
        t |> success(parity_gen_ok(tr, spec.parity.prompt, spec.parity.gen, [eyeball = true]), "gpt-oss-20b token-for-token")
        let long_prompt <- encode(tr, counting_prompt(150), true, false)
        t |> success(parity_gen_ok(tr, long_prompt, GPTOSS_LONG_GEN, [eyeball = true]), "gpt-oss-20b window-engaged token-for-token")
        delete tr
    }
}
