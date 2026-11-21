options gen2
options stack = 524288   // the test file is the program root under dastest; the gpt-oss forward (mx4 experts, sinks, biases) overflows the default budget
options persistent_heap   // + explicit deletes below: each arm frees its model/session before the next loads, so multi-GB weights never accumulate in the process
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — public-path tests exercise the API through it (engine + arch registrations + chat)
require dasllama/dasllama_transformer
require dasllama/dasllama_common
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_moe
require daslib/f16_cvt   // f16_to_f32 — the tied-cls arm reads the live scale plane
require dasllama/dasllama_math   // pin_kernel_backend / kernel_backend_pin_refusal / active_kernel_backend — the facade wraps none of them
require dasllama/dasllama_env    // g_env_engine.pin_backend
require daslib/defer   // the backend pin is restored on every path out of a pinned cell
require daslib/jobque_boost
require daslib/fio
require strings   // find: the refusals' names
require _model_tier   // models_dir() + model_available() presence/size-tier gate (see the file's header)
require _resident_feed
require ../performance/model_specs.das   // the model-set table — parity evidence lives there as DATA
require ../harness/_parity_dump.das

// Frozen parity gates (token for token; a `prefix_bar` carrier as a half-span prefix) for the arches stories15M (test_forward.das) doesn't cover.
// Evidence is DATA in model_specs.das — pinned ids (or a text prompt encoded here) + the arms it was
//   frozen under; regenerate with harness/parity.sh. The board's --parity pregate reads the same rows.
// Models are gitignored multi-GB files: absent = clean skip (a real local regression, a no-op in CI).
// Every model cell skips off the JIT, without its file, past the
// size tier (`model_available`), or where DASLLAMA_PIN_BACKEND names
// a backend this box cannot pin; the layer-cut cells skip too where
// the run's family filter leaves their family out, the dump round
// trip without the armed resident tier.

def private with_parity_backend(t : T?; blk : block<() : void>) {
    let name = g_env_engine.pin_backend
    if (empty(name)) {
        invoke(blk)
        return
    }
    let why = kernel_backend_pin_refusal(name)
    if (!empty(why)) {
        t |> skip("DASLLAMA_PIN_BACKEND='{name}' cannot be pinned on this box: {why}")
        return
    }
    let seat = kernel_backend_seat()
    defer() {
        restore_kernel_backend(seat)
    }
    pin_kernel_backend(name)
    invoke(blk)
}

// cls_q8 invariant: a tied Q8 load of a Q8_0 embedding drops the fp32 table (the classifier
// matmuls the disk quants; rows dequant on demand from the linear transcode at emb_q8_off) — and
// those rows must be bit-identical to the fp32 table a fp32 load decodes from the same disk data.
// SmolLM2-135M is the smallest tied model.
[test(description = "A tied Q8 load of SmolLM2-135M takes the cls_q8 path, and its dequantized embedding rows for four tokens are bit-identical to the fp32 table an fp32 load decodes")]
def test_parity_tied_cls_q8_rows(t : T?) {
    t |> run("tied Q8 load: cls_q8 embedding rows bit-match the fp32 table") @(t : T?) {
        let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        return if (!jit_model_available(t, path))
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

// emb_q8 invariant, the untied half: a Q8 load of a Q8_0 embedding with a SEPARATE output.weight drops the
// fp32 table too (rows dequant from the linear transcode at emb_q8_off, bit-identical to the fp32 decode);
// without it an untied model carries vocab x dim floats, 1.5 GB on a 152k-vocab 4B. stories15M is the smallest untied Q8_0 file.
[test(description = "An untied Q8 load of stories15M drops the fp32 embedding table for the emb_q8 plane, whose rows and `embed_row` output are bit-identical to the fp32 load's table")]
def test_parity_untied_emb_q8_rows(t : T?) {
    t |> run("untied Q8 load: emb_q8 embedding rows bit-match the fp32 table, no fp32 table kept") @(t : T?) {
        let path = path_join(models_dir(), "stories15M-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        var t32 <- load_gguf(path, QuantMode.fp32)
        var t8 <- load_gguf(path, QuantMode.q8)
        t |> success(!t32.emb_q8 && t32.tok_emb_off >= 0l, "fp32 load keeps the fblob table")
        t |> success(!t8.config.shared_weights, "the fixture is untied (output.weight on disk)")
        t |> success(!t8.cls_q8, "untied: the classifier flag stays off")
        t |> success(t8.emb_q8, "untied Q8_0 load takes the emb_q8 path")
        t |> success(t8.tok_emb_off < 0l, "no fp32 table in fblob")
        t |> success(t8.emb_q8_off != t8.wcls_off, "the embedding copy is not the classifier plane")
        let dim = t8.config.dim
        var bad = 0l
        for (token in [0l, 1l, 1000l, t8.config.vocab_size - 1l]) {
            for (i in range64(dim)) {
                let el = t8.emb_q8_off + token * dim + i
                let want = t32.fblob[t32.tok_emb_off + token * dim + i]
                let sc = t8.wscale_f16 ? f16_to_f32(uint(t8.qscales16[el / 32l])) : t8.qscales[el / 32l]
                if (float(t8.qblob[el]) * sc != want) {
                    bad++
                }
            }
        }
        t |> success(bad == 0l, "embedding rows bit-identical ({bad} mismatches)")
        // the row reader the forward pass uses agrees with the raw plane arithmetic
        var inscope row : array<float>
        row |> resize(dim)
        embed_row(t8, 1000l, unsafe(addr(row[0])))
        var bad_row = 0l
        for (i in range64(dim)) {
            if (row[i] != t32.fblob[t32.tok_emb_off + 1000l * dim + i]) {
                bad_row++
            }
        }
        t |> success(bad_row == 0l, "embed_row reads the same values ({bad_row} mismatches)")
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

// The half-span prefix bar on one live stream, with the loosened check's control on the same stream: the fixture
// with an id flipped inside its first half must red the bar.
def private prefix_bar_cell(t : T?; tr : Model; prompt, want : array<int64>; what : string) {
    let n = length(want)
    var got <- parity_gen_ids(tr, prompt, n)
    to_log(LOG_INFO, "eyeball want: {eyeball_text(tr, want)}\n")
    to_log(LOG_INFO, "eyeball got:  {eyeball_text(tr, got)}\n")
    let same = prefix_match_len(got, want)
    t |> success(length(got) == n && fixture_ok(true, same, int64(n)), "{what} ({same} of {n} ids match)")
    var flipped := want
    let k = n / 4
    flipped[k] ^= 1l
    t |> success(!fixture_ok(true, prefix_match_len(got, flipped), int64(n)),
        "{what}: control - the fixture with id {k} of {n} flipped reds the bar")
    delete flipped
    delete got
}

//! `path` loaded as the parity row `spec` loads its file: under the
//! row's K-quant native knob, put back after the load, in its load
//! mode, the K/V context cut to 2048 - only about prompt plus
//! generation positions are used, the K/V stride changes no computed
//! token, and Llama-3's native 131072 would burn ~8GB of K/V RAM
def private parity_row_load(path : string; spec : ModelSpec) : Model {
    let prev_kq = get_kquant_native()
    set_kquant_native(spec.parity.kq_native)
    var tr <- load_model_(path, spec.parity.quant)
    set_kquant_native(prev_kq)
    tr.config.seq_len = min(tr.config.seq_len, 2048l)
    return <- tr
}

//! the parity row's prompt: its pinned ids, or its text encoded
def private parity_prompt(tr : Model; spec : ModelSpec) : array<int64> {
    return <- (empty(spec.parity.prompt_text) ? clone_to_move(spec.parity.prompt)
        : encode(tr, spec.parity.prompt_text, true, false))
}

// One spec's arms off ONE model load. load_model_ (not load_gguf) so the tokenizer rides along:
// text-form prompts encode here, and every compare logs its decoded-text eyeball lines.
[cold_path]
def private parity_spec_arms(t : T?; spec : ModelSpec; path : string) {
    let prev_grouped = get_moe_grouped_prefill()
    let prev_fused = get_moe_fused_expert_gemms()
    defer() {
        set_moe_grouped_prefill(prev_grouped)
        set_moe_fused_expert_gemms(prev_fused)
    }
    // the main arm runs on the levers' defaults, whatever an earlier cell left
    set_moe_grouped_prefill(true)
    set_moe_fused_expert_gemms(true)
    var tr <- parity_row_load(path, spec)
    let be = " [backend {active_kernel_backend()}]"   // read AFTER the load: that is where the pin resolves
    var prompt <- parity_prompt(tr, spec)
    if (spec.parity.require_dn_kq) {
        t |> success(dn_kq_engaged(tr.dnqkv_fmt) > 0l && dn_kq_engaged(tr.dngate_fmt) > 0l && dn_kq_engaged(tr.dnout_fmt) > 0l,
            "{spec.file} dn planes kq-native (not demoted)")
    }
    let pb = spec.parity.prefix_bar
    let form = pb ? "half-span prefix" : "token-for-token"
    if (pb) {
        prefix_bar_cell(t, tr, prompt, spec.parity.gen, "{spec.file} {form}{be}")
    } else {
        t |> success(parity_gen_ok(tr, prompt, spec.parity.gen, [eyeball = true]), "{spec.file} {form}{be}")
    }
    if (spec.parity.dn_chunked) {
        t |> success(parity_gen_ok(tr, prompt, spec.parity.gen, [dn_chunked = true, eyeball = true, prefix_bar = pb]),
            "{spec.file} chunked-prefill {form}{be}")
    }
    if (spec.parity.moe_reference) {
        set_moe_grouped_prefill(false)
        t |> success(parity_gen_ok(tr, prompt, spec.parity.gen, [eyeball = true, prefix_bar = pb]),
            "{spec.file} reference-prefill {form}{be}")
        set_moe_grouped_prefill(true)
    }
    if (spec.parity.moe_per_expert) {
        set_moe_fused_expert_gemms(false)
        t |> success(parity_gen_ok(tr, prompt, spec.parity.gen, [eyeball = true, prefix_bar = pb]),
            "{spec.file} per-expert-prefill {form}{be}")
        set_moe_fused_expert_gemms(true)
    }
    delete prompt
    delete tr
}

[test(description = "Every model_specs row with a pinned parity fixture generates its frozen greedy ids token for token (or to the half-span prefix bar, with a flipped-id control), including its chunked, reference and per-expert prefill arms")]
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
            return if (!jit_only(t))
            var inscope spec <- spec_by_file(file)
            let path = path_join(models_dir(), spec.file)
            if (!model_available(t, path)) return
            with_parity_backend(t) {
                parity_spec_arms(t, spec, path)
            }
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
[test(description = "gpt-oss-20b on one Q8 load reproduces its frozen greedy ids token for token on the short spec fixture and on a 449-token counting prompt that engages the 128-token sliding window")]
def test_parity_gptoss_20b(t : T?) {
    t |> run("gpt-oss forward (gpt-oss-20b native-MXFP4 experts, sinks + YaRN + MoE biases; short + window-engaged)") @(t : T?) {
        return if (!jit_only(t))
        var inscope spec <- spec_by_file("gpt-oss-20b-mxfp4.gguf")
        let path = path_join(models_dir(), spec.file)
        if (!model_available(t, path)) return
        with_parity_backend(t) {
            var tr <- load_model_(path, QuantMode.q8)
            if (tr.config.seq_len > 2048l) {
                tr.config.seq_len = 2048l
            }
            let be = " [backend {active_kernel_backend()}]"
            t |> success(parity_gen_ok(tr, spec.parity.prompt, spec.parity.gen, [eyeball = true]), "gpt-oss-20b token-for-token{be}")
            let long_prompt <- encode(tr, counting_prompt(150), true, false)
            t |> success(parity_gen_ok(tr, long_prompt, GPTOSS_LONG_GEN, [eyeball = true]), "gpt-oss-20b window-engaged token-for-token{be}")
            delete tr
        }
    }
}

//! gpt-oss-4L's greedy streams under the reference: after the 20b fixture's short prompt, and after the 449-token
//! counting prompt that engages the sliding window
let private GPTOSS_CUT_REF <- [34380l, 52444l, 47914l, 198854l, 109598l, 15255l, 3386l, 5885l, 28978l, 16286l,
                               26230l, 175455l, 9355l, 8439l, 40815l, 5436l, 48038l, 21406l, 4186l, 24724l,
                               6257l, 6257l, 716l, 104735l, 1512l, 104735l, 168333l, 18023l, 43933l, 15255l,
                               3386l, 471l, 9869l, 103895l, 23532l, 42842l, 150384l, 18269l, 26760l, 16962l]
let private GPTOSS_CUT_LONG_REF <- [32764l, 65816l, 155519l, 1662l, 20545l, 3055l, 4570l, 6486l, 80677l, 23042l,
                                    31965l, 44603l, 3817l, 7583l, 2562l, 2562l, 26760l, 7462l, 47914l, 3817l,
                                    26760l, 105712l, 7462l, 47914l, 80677l, 23042l, 43583l, 7462l, 26760l, 3079l,
                                    1369l, 35769l, 5325l, 30796l, 26760l, 2881l, 189793l, 12346l, 51718l, 4396l]

//! How far under a row's top logit a reference id may sit, as a
//! fraction of the row's largest logit: the first blocks of a file
//! write no confident text, the reference's pick and ours trading
//! places inside 0.8% of the top logit on gpt-oss-4L (the 26B's cut
//! picks every reference id), where a path that broke moves a row by
//! whole logits
let private CUT_GAP_REL = 0.02

//! `prompt` prefilled and the reference's stream `ids` fed after it:
//! every id within `bar` (of the row's largest logit) under its row's
//! top logit, a NaN gap the worst, and the stream one step late - the
//! control, every gap it reads finite - outside that on all but one
//! row in `late_per`; the reference's text, our picks', the two worst
//! gaps and the late stream's gaps sorted logged
def private forced_stream_within(t : T?; tr : Model; prompt, ids : array<int64>; bar : float;
                                 what : string; late_per : int = 4) {
    with_pinned_kernels() {
        var inscope rows <- feed_alone(tr, KVDtype.f32, prompt, ids)
        let n = length(ids)
        var inscope picks <- [for (i in range(n)); argmax_of(rows[i])]
        to_log(LOG_INFO, "{what}: the reference's text {eyeball_text(tr, ids)}\n")
        to_log(LOG_INFO, "{what}: our picks' text {eyeball_text(tr, picks)}\n")
        var worst = 0.0
        var second = 0.0
        var lates : array<float>
        var late = 0
        var late_unread = 0
        var nearest_late = INFINITE_DIFF
        for (i in range(n)) {
            let top = rows[i][picks[i]]
            let scale = live_maxabs(rows[i])
            let gap = (top - rows[i][ids[i]]) / scale
            let g = is_nan(gap) ? INFINITE_DIFF : gap
            if (g > worst) {
                second = worst
                worst = g
            } elif (g > second) {
                second = g
            }
            continue if (i + 1 >= n || ids[i + 1] == ids[i])
            let late_gap = (top - rows[i][ids[i + 1]]) / scale
            if (!is_finite(late_gap)) {
                late_unread++
                continue
            }
            nearest_late = min(nearest_late, late_gap)
            lates |> push(late_gap)
            if (late_gap <= bar) {
                late++
            }
        }
        to_log(LOG_INFO, "{what}: the worst gap {worst} of the top logit, {late} of {n - 1} late " +
            "ids inside the bar, the nearest late id {nearest_late}\n")
        sort(lates)
        to_log(LOG_INFO, "{what}: the second-worst gap {second}; the late stream's gaps sorted {lates}\n")
        t |> success(worst <= bar,
            "{what}: every reference id within {bar} of its row's top logit (worst {worst})")
        t |> equal(late_unread, 0, "{what}: control - every late id's gap reads finite")
        t |> success(late * late_per < n,
            "{what}: control - the stream one step late sits inside the bar on {late} of " +
            "{n - 1} rows")
    }
}

//! the parity row `full`'s file cut to its first `blocks` blocks
//! (`cut`): loaded as that row loads its file, its block count read
//! off the load, then `body` on it with the row and the row's prompt,
//! inside the engine's job queue
def private cut_stream(t : T?; cut, full : string; blocks : int64;
                       body : block<(tr : Model; spec : ModelSpec; prompt : array<int64>) : void>) {
    let path = path_join(models_dir(), cut)
    return if (!model_available(t, path))
    with_parity_backend(t) {
        var inscope spec <- spec_by_file(full)
        var tr <- parity_row_load(path, spec)
        cut_within(t, "{cut}: ", tr, blocks)
        with_dasllama_jobque_() {
            var inscope prompt <- parity_prompt(tr, spec)
            invoke(body, tr, spec, prompt)
        }
        delete tr
    }
}

[test(description = "gpt-oss-4L, the 20b's first four blocks, fed the reference's greedy stream keeps every reference id within 2% of its row's top logit on the short prompt and on the 449-token one that engages the sliding window, the stream one step late outside")]
def test_parity_gptoss_cut(t : T?) {
    t |> run("gpt-oss forward on the four-block cut (native-MXFP4 experts, sinks + YaRN + MoE biases; short + window-engaged), the reference's ids fed") @(t : T?) {
        return if (!family_on(t, "gptoss") || !jit_only(t))
        let cut = "gpt-oss-4L-mxfp4-local.gguf"
        cut_stream(t, cut, "gpt-oss-20b-mxfp4.gguf", 4l) $(tr, _spec, prompt) {
            forced_stream_within(t, tr, prompt, GPTOSS_CUT_REF, CUT_GAP_REL,
                "gpt-oss-4L, the short prompt")
            var inscope long_prompt <- encode(tr, counting_prompt(150), true, false)
            t |> equal(length(long_prompt), 449,
                "the counting prompt encodes to the reference's 449 ids")
            forced_stream_within(t, tr, long_prompt, GPTOSS_CUT_LONG_REF, CUT_GAP_REL,
                "gpt-oss-4L, the window engaged")
        }
    }
}

//! the gemma-4 cuts' greedy streams under the reference after the full files' fixture prompt, the 1..320 count (1492 ids,
//! past the 12B's and the 26B's sliding windows)
let private GEMMA4_12B_CUT_REF <- [1132l, 237027l, 102398l, 163234l, 594l, 118731l, 594l, 109397l, 75800l, 3244l,
                                   255999l, 32424l, 255999l, 32424l, 255999l, 32424l, 255999l, 32424l, 255999l, 32424l,
                                   255999l, 32424l, 255999l, 32424l, 255999l, 32424l, 255999l, 32424l, 255999l, 32424l,
                                   255999l, 32424l, 255999l, 32424l, 255999l, 32424l, 255999l, 32424l, 255999l, 103469l]
let private GEMMA4_26B_CUT_REF <- [233605l, 236963l, 71032l, 237763l, 238031l, 237372l, 26691l, 190786l, 26691l, 28469l,
                                   1663l, 28469l, 1663l, 237469l, 50727l, 38298l, 224246l, 1363l, 106905l, 61771l,
                                   50357l, 28469l, 53585l, 3785l, 1852l, 1852l, 233089l, 59475l, 236835l, 7473l,
                                   238215l, 234358l, 176418l, 237049l, 238215l, 234358l, 237323l, 241075l, 61771l, 36841l]
let private GEMMA4_CUT_PROMPT_IDS = 1492   //! the reference's count of the fixture prompt's ids
//! the 12B cut's bar: the reference's ids sit up to 0.017 of the
//! top logit under ours, the nearest late id 0.50, and the global
//! layers' p-RoPE applied to every layer reads 0.046
let private GEMMA4_12B_CUT_GAP_REL = 0.03

//! a gemma-4 cut fed its reference stream after its full file's
//! fixture prompt
def private gemma4_cut_stream(t : T?; cut, full : string; blocks : int64; ref : array<int64>;
                              bar : float) {
    cut_stream(t, cut, full, blocks) $(tr, _spec, prompt) {
        t |> equal(length(prompt), GEMMA4_CUT_PROMPT_IDS,
            "the fixture prompt encodes to the reference's {GEMMA4_CUT_PROMPT_IDS} ids")
        forced_stream_within(t, tr, prompt, ref, bar, cut)
    }
}

[test(description = "gemma-4-12B-12L, the 12B's first twelve blocks, fed the reference's greedy stream after the 1492-id count keeps every reference id within 3% of its row's top logit, the stream one step late outside")]
def test_parity_gemma4_12b_cut(t : T?) {
    t |> run("the dense gemma-4 forward on the twelve-block cut (two global layers, the window engaged), the reference's ids fed") @(t : T?) {
        return if (!family_on(t, "gemma4") || !jit_only(t))
        gemma4_cut_stream(t, "gemma-4-12B-12L-Q4_K_M-local.gguf",
            "gemma-4-12B-it-Q4_K_M.gguf", 12l,
            GEMMA4_12B_CUT_REF, GEMMA4_12B_CUT_GAP_REL)
    }
}

[test(description = "gemma-4-26B-A4B-6L, the 26B's first six blocks, fed the reference's greedy stream after the 1492-id count keeps every reference id within 2% of its row's top logit, the stream one step late outside")]
def test_parity_gemma4_26b_cut(t : T?) {
    t |> run("the routed gemma-4 forward on the six-block cut (one global layer, the window engaged), the reference's ids fed") @(t : T?) {
        return if (!family_on(t, "gemma4moe") || !jit_only(t))
        gemma4_cut_stream(t, "gemma-4-26B-A4B-6L-Q4_K_M-local.gguf",
            "gemma-4-26B-A4B-it-Q4_K_M.gguf", 6l,
            GEMMA4_26B_CUT_REF, CUT_GAP_REL)
    }
}


//! the Qwen1.5-MoE-A2.7B-4L Q8_0 cut's greedy stream under the
//! reference after the qwen counting fixture (`1, 2, 3, 4, 5, 6, 7,`):
//! the worst reference id reads 0.0083 of the top logit under ours on
//! the M5's CPU chain, the nearest late id 0.31 - the cut's bar `CUT_GAP_REL`
let private QWEN2MOE_CUT_REF <- [220l, 101371l, 112781l, 108339l, 108339l, 102895l, 99710l, 44851l, 104362l, 101353l,
                                 9931l, 31628l, 34794l, 141536l, 102423l, 99829l, 58158l, 119519l, 76111l, 122559l,
                                 56273l, 119261l, 87364l, 52774l, 37160l, 113202l, 111846l, 107698l, 22211l, 117724l,
                                 103712l, 89831l, 105211l, 28330l, 101175l, 100273l, 96922l, 117166l, 1395l, 93994l]

[test(description = "Qwen1.5-MoE-A2.7B-4L Q8_0, the carrier's first four blocks, fed the reference's greedy stream after the counting fixture keeps every reference id within 2% of its row's top logit, the stream one step late outside")]
def test_parity_qwen2moe_cut(t : T?) {
    t |> run("the routed qwen2moe forward on the four-block cut (60 experts, 4 routed, the gated shared expert), the reference's ids fed") @(t : T?) {
        return if (!family_on(t, "qwen2moe") || !jit_only(t))
        cut_stream(t, "Qwen1.5-MoE-A2.7B-4L-Q8_0-local.gguf", "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf", 4l) $(tr, _spec, prompt) {
            forced_stream_within(t, tr, prompt, QWEN2MOE_CUT_REF, CUT_GAP_REL, "Qwen1.5-MoE-A2.7B-4L Q8_0")
        }
    }
}

//! the Qwen3.6-35B-A3B-4L UD-Q4_K_M cut's greedy stream under the
//! reference after the qwen counting fixture
let private QWEN35MOE_CUT_REF <- [4831l, 150481l, 97148l, 97148l, 97148l, 520l, 78931l, 165676l, 162576l, 45560l,
                                  138170l, 50831l, 1313l, 72227l, 128702l, 97389l, 10167l, 160370l, 1320l, 174222l,
                                  7346l, 72l, 39787l, 80151l, 12181l, 38012l, 391l, 391l, 391l, 53206l,
                                  41669l, 245136l, 5262l, 39231l, 245739l, 74475l, 90496l, 19483l, 4096l, 112516l]

//! the 35B cut's bar, twice its measured worst: the reference's ids
//! sit up to 0.0171 of the top logit under ours on the M5's CPU chain
//! (the next 0.0096), and one of the 39 late ids sits inside it (a
//! repeat of our own pick, gap 0; the next 0.076) - the control holds
//! at most one late id in eight inside the bar
let private QWEN35MOE_CUT_GAP_REL = 0.035
let private QWEN35MOE_CUT_LATE_PER = 8

[test(description = "Qwen3.6-35B-A3B-4L UD-Q4_K_M, the 35B's first four blocks, fed the reference's greedy stream after the counting fixture keeps every reference id within 3.5% of its row's top logit, the stream one step late outside on all but one row in eight")]
def test_parity_qwen35moe_cut(t : T?) {
    t |> run("the deltanet hybrid MoE forward on the four-block cut (three recurrent layers, one attention layer, 256 experts, 8 routed, the gated shared expert), the reference's ids fed") @(t : T?) {
        return if (!family_on(t, "qwen35moe") || !jit_only(t))
        cut_stream(t, "Qwen3.6-35B-A3B-4L-UD-Q4_K_M-local.gguf", "Qwen3.6-35B-A3B-UD-Q4_K_M.gguf", 4l) $(tr, _spec, prompt) {
            forced_stream_within(t, tr, prompt, QWEN35MOE_CUT_REF, QWEN35MOE_CUT_GAP_REL,
                "Qwen3.6-35B-A3B-4L UD-Q4_K_M", QWEN35MOE_CUT_LATE_PER)
        }
    }
}

// The parity dump round trip on Qwen3.5-0.8B-MTP: the device arm's forced-feed run to a file, the CPU arm's with the device
// arm's prompt, feed and routing tape replayed off that file, and the two files compared under the regions files' bar - the
// in-process parity the stocked suite runs, through the files a model past the box has to use. A poisoned row reds the compare,
// a reference with another feed is refused. Skips: no Qwen3.5-0.8B-MTP file under the models dir, the GPU tier not armed
// (DASLLAMA_GPU=1 on a device that serves the whole-model driver), not `-jit`.

let private MTP_FILE = "Qwen3.5-0.8B-MTP-Q8_0.gguf"
let private BAR_REL = 0.20   //! the regions files' row bar

//! a file at `name` under the test's temp dir holding the two header words `magic` and `version` and nothing after
def private header_file(name : string; magic, version : int64) : string {
    let path = path_join(test_temp_dir(), name)
    fopen(path, "wb") $(f) {
        fwrite(f, magic)
        fwrite(f, version)
    }
    return path
}

[test(description = "`parity_dump_refusal` names a foreign magic word, a different version word and a missing file as refusals, and accepts a header carrying this build's magic and version")]
def test_parity_dump_refuses_foreign_files(t0 : T?) {
    t0 |> run("a file with another magic word is no parity dump, one with another version word is another version's, and this build's header words are read: each refused by name, model-free") @(t : T?) {
        let foreign = header_file("foreign.pd", 1313821514l, PARITY_DUMP_VERSION)   // "JSON" as a little-endian word
        let old = header_file("old.pd", PARITY_DUMP_MAGIC, PARITY_DUMP_VERSION + 1l)
        let ours = header_file("ours.pd", PARITY_DUMP_MAGIC, PARITY_DUMP_VERSION)
        let why_foreign = parity_dump_refusal(foreign)
        let why_old = parity_dump_refusal(old)
        t |> success(find(why_foreign, "is not a parity dump") >= 0, "another magic word is refused by name ({why_foreign})")
        t |> success(find(why_old, "is another version's") >= 0, "another version word is refused by name ({why_old})")
        t |> equal(parity_dump_refusal(ours), "", "this build's header words are read")
        t |> success(find(parity_dump_refusal(path_join(test_temp_dir(), "missing.pd")), "cannot read") >= 0, "a missing file is refused by name")
    }
}

[test(description = "On Qwen3.5-0.8B-MTP the device-arm dump reads back bit-exact, the CPU arm replayed on its prompt, feed and tape matches every row within the 0.20 relative bar, a one-row poison reds only that row, and a changed feed is refused")]
def test_parity_dump_round_trip(t : T?) {
    t |> run("Qwen3.5-0.8B-MTP: the device arm dumped, the CPU arm dumped on the device arm's tape, the files within the bar; a poisoned row reds, another feed is refused") @(t : T?) {
        let path = path_join(models_dir(), MTP_FILE)
        if (!resident_ready(t, path)) return
        let dev_path = path_join(test_temp_dir(), "parity_dev.pd")
        let cpu_path = path_join(test_temp_dir(), "parity_cpu.pd")
        var tr <- load_model_(path, QuantMode.q8)
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the carrier")
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(40l)
            let none : array<int64>
            var dev <- parity_dump_run(tr, MTP_FILE, mirror_kv_dtype(), prompt, none, 6l, none)
            t |> equal(dev.arm, "device", "the first arm is the device's")
            t |> equal(length(dev.rows), 7, "the prefill's row and six fed steps")
            t |> equal(length(dev.feed), 6, "six fed tokens, the run's own greedy continuation")
            t |> equal(int(moe_pick_tape_mode().mode), int(MoePickTape.off), "the tape is off after the run")
            parity_dump_write(dev_path, dev)
            var back <- parity_dump_read(dev_path)
            t |> success(back.arm == dev.arm && back.vocab == dev.vocab && back.picks_per_select == dev.picks_per_select && ids_equal(back.prompt, dev.prompt) && ids_equal(back.feed, dev.feed) && ids_equal(back.tape, dev.tape), "the dump reads back as written")
            t |> equal(logits_maxdiff(back.rows[3], dev.rows[3]), 0.0, "... its rows bit for bit")
            moe_gpu_drop_model()
            var cpu <- parity_dump_run(tr, MTP_FILE, mirror_kv_dtype(), back.prompt, back.feed, 0l, back.tape)
            t |> equal(cpu.arm, "cpu", "the second arm is the CPU chain's")
            // the carrier is dense: its tape is empty and the replay sees no select; a routed carrier's replay counts every select
            t |> success(dev.picks_per_select == 0l ? cpu.tape_seen == 0l && empty(back.tape) : cpu.tape_seen > 0l, "the CPU arm replayed the device arm's routing ({cpu.tape_seen} selects, {cpu.tape_miss} own picks off the tape, {dev.picks_per_select} picks a select)")
            parity_dump_write(cpu_path, cpu)
            var cpu_back <- parity_dump_read(cpu_path)
            var r <- parity_dump_compare(back, cpu_back, BAR_REL)
            for (row in r.rows) {
                to_log(LOG_INFO, "parity dump row {row.row}: maxdiff {row.diff} (bar {row.bar}) one-step-off {row.ctrl}, argmax device {row.argmax_a} '{decode_(tr, [row.argmax_a])}' against cpu {row.argmax_b} '{decode_(tr, [row.argmax_b])}'\n")
            }
            t |> success(r.ok && empty(r.why), "the device dump is within the bar of the CPU dump on every row, every one-step-off control past it ({r.why})")
            back.rows[2][17] = back.rows[2][17] + 3.0 * r.rows[2].bar
            var poisoned <- parity_dump_compare(back, cpu_back, BAR_REL)
            t |> success(!poisoned.ok && !poisoned.rows[2].ok && poisoned.rows[3].ok, "a poisoned row (one logit three bars off) reds its compare alone")
            cpu_back.feed[0] = cpu_back.feed[0] + 1l
            var refused <- parity_dump_compare(back, cpu_back, BAR_REL)
            t |> success(!refused.ok && !empty(refused.why), "a reference with another feed is refused ({refused.why})")
            delete refused
            delete poisoned
            delete r
            delete cpu_back
            delete cpu
            delete back
            delete dev
            delete prompt
        }
        delete tr
    }
}
