options gen2
options _dasllama_internal = true

module _parity_cells shared public

//! the parity cells - tied classifier rows, the model_specs rows' frozen streams, the layer cuts' streams - on a loaded model;
//! none loads one

require dastest/testing_boost public
require dasllama/dasllama   // the public facade - encode, the arch registrations
require dasllama/dasllama_math   // with_dasllama_jobque_, active_kernel_backend
require dasllama/dasllama_env   // g_env_engine.pin_backend: a parity row's form's backend pin
require daslib/f16_cvt   // f16_to_f32 - the embedding rows read the live scale plane
require daslib/jobque_boost
require math
require _model_tier   // cut_within, eyeball_text, with_pinned_kernels, live_maxabs
require _compares   // argmax_of, INFINITE_DIFF, mismatch_exact
require _resident_feed   // feed_alone
require _cpu_gpu_model   // with_ctx_cut, cpu_cells_form: a parity row's form and its context
require ../performance/model_specs.das   // spec_by_file, counting_prompt: the cut rows' full files and prompts

// The parity cells on a loaded model, beside `test_parity.das`'s model_specs rows: the tied Q8 classifier's embedding rows
// against the fp32 load's table (SmolLM2-135M, the smallest tied model: two loads, the compare after both), and the layer
// cuts - a model file holding a carrier's first blocks - fed the reference's greedy stream after the full file's parity
// prompt (gpt-oss-4L, gemma-4-12B-12L, gemma-4-26B-A4B-6L, Qwen1.5-MoE-A2.7B-4L, Qwen3.6-35B-A3B-4L). A cut loads as its full
// file's parity row loads it (`parity_row_load` in `test_parity.das`: the row's K-quant native knob around the load, its load
// mode, the context cut to 2048).

let private EMBEDDING_ROW_TOKENS = fixed_array(0l, 1l, 1000l, -1l)   //! the compared rows' tokens, -1 the vocabulary's last

//! `m`'s embedding rows of EMBEDDING_ROW_TOKENS as its load reads them: with `from_q8` the q8 plane at emb_q8_off times its
//! live scale (wscale_f16 frees the f32 one; the widen is the exact disk value, so a bit-match claim is unchanged), else the fp32
//! table in fblob where the load kept it - none where it did not
def embedding_rows(m : Model; from_q8 : bool) : array<float> {
    var rows : array<float>
    let dim = m.config.dim
    for (tk in EMBEDDING_ROW_TOKENS) {
        let token = tk < 0l ? m.config.vocab_size - 1l : tk
        for (i in range64(dim)) {
            if (from_q8) {
                let el = m.emb_q8_off + token * dim + i
                let sc = m.wscale_f16 ? f16_to_f32(uint(m.qscales16[el / 32l])) : m.qscales[el / 32l]
                rows |> push(float(m.qblob[el]) * sc)
            } elif (m.tok_emb_off >= 0l) {
                rows |> push(m.fblob[m.tok_emb_off + token * dim + i])
            }
        }
    }
    return <- rows
}

//! one load's tied embedding rows: whether the load took the cls_q8 path, and its `embedding_rows` - the fp32 table off an
//! fp32 load, the q8 plane times its live scale off a q8 load
struct TiedClsRows {
    cls_q8 : bool
    rows : array<float>
}

//! `m`'s tied rows as its load reads them (`TiedClsRows`)
def tied_cls_rows(m : Model) : TiedClsRows => TiedClsRows(cls_q8 = m.cls_q8, rows <- embedding_rows(m, m.cls_q8))

//! The cls_q8 invariant on SmolLM2-135M's two loads' rows (`tied_cls_rows`): the fp32 load keeps the fblob table, the tied
//! Q8_0 load drops it for the classifier's disk quants (rows dequant on demand from the linear transcode at emb_q8_off), and
//! those rows are bit-identical to the fp32 table
def tied_cls_q8_rows_cell(t : T?; f32, q8 : TiedClsRows) {
    unsafe {
        t |> run("tied Q8 load: cls_q8 embedding rows bit-match the fp32 table") @capture(& f32, & q8)(t : T ?) {
            t |> success(!f32.cls_q8, "fp32 load keeps the fblob table")
            t |> success(q8.cls_q8, "tied Q8_0 load takes the cls_q8 path")
            let bad = mismatch_exact(q8.rows, f32.rows)
            t |> success(bad == 0, "embedding rows bit-identical ({bad} mismatches)")
        }
    }
}

//! the parity row's prompt: its pinned ids, or its text encoded
def parity_prompt(tr : Model; spec : ModelSpec) : array<int64> {
    return <- (empty(spec.parity.prompt_text) ? clone_to_move(spec.parity.prompt)
        : encode(tr, spec.parity.prompt_text, true, false))
}

//! the context a parity row loads at (`parity_row_load` in `test_parity.das`): only about prompt plus generation positions are
//! used, the K/V stride changes no computed token
let PARITY_ROW_CTX = 2048l

//! the model_specs parity rows whose files' blocks run them (`parity_spec_cell`): `test_parity_specs` walks every other row
let PARITY_BLOCK_ROWS = fixed_array("Qwen2.5-0.5B-Instruct-Q8_0.gguf", "Qwen2.5-1.5B-Instruct-IQ3_XS.gguf",
    "Qwen3-0.6B-Q8_0.gguf", "Qwen3-4B-Instruct-2507-Q8_0.gguf",
    "Qwen3.5-0.8B-Q8_0.gguf", "Qwen3.5-0.8B-Q4_K_M.gguf", "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", "Qwen3.5-4B-Q8_0.gguf",
    "gemma-2-2b-it-Q8_0.gguf", "gemma-3-1b-it-Q8_0.gguf", "gemma-3-4b-it-Q8_0.gguf",
    "Llama-3.2-1B-Instruct-Q8_0.gguf", "Llama-3.2-1B-Instruct-Q4_0-local.gguf", "Llama-3.2-1B-Instruct-Q4_K_M.gguf",
    "Llama-3.2-1B-Instruct-IQ4_XS.gguf", "Llama-3.2-1B-Instruct-IQ4_NL-local.gguf", "Llama-3.2-1B-Instruct-NVFP4-local.gguf",
    "gemma-4-E2B-it-Q8_0.gguf", "gemma-4-E2B-it-Q4_K_M.gguf", "gemma-4-E2B-it-Q4_0.gguf", "gemma-4-12B-it-Q8_0.gguf",
    "gemma-4-26B-A4B-it-Q4_K_M.gguf", "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf", "gemma-4-26B_q4_0-it.gguf",
    "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", "Qwen3.8-27B-Q4_K_M.gguf",
    "Qwen3.8-Flash-Next-GGUF/UD-IQ4_XS/Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf",
    "Llama-3.2-1B-Instruct-Q5_K_M.gguf", "Llama-3.2-1B-Instruct-IQ2_XXS-local.gguf",
    "Llama-3.2-1B-Instruct-Q3_K_L.gguf", "Llama-3.2-1B-Instruct-IQ3_M.gguf",
    "Llama-3.2-1B-Instruct-IQ3_XXS-local.gguf", "Llama-3.2-1B-Instruct-Q2_K-local.gguf",
    "Llama-3.2-1B-Instruct-IQ2_XS-local.gguf", "Llama-3.2-1B-Instruct.i1-IQ3_XXS.gguf",
    "Qwen2.5-1.5B-Instruct-Q8_0.gguf", "Qwen3-4B-Instruct-2507-Q4_K_M.gguf",
    "Qwen3-4B-Instruct-2507-Q5_K_M.gguf", "Qwen3.6-27B-MTP-Q4_K_M.gguf",
    "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf", "Qwen3.6-35B-A3B-UD-Q4_K_M.gguf",
    "Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf", "Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf",
    "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf", "Qwen3.6-35B-A3B-UD-IQ2_XXS.gguf",
    "Qwen3-Coder-Next-Q4_K_M.gguf",
    "gemma-2-2b-it-Q4_K_M.gguf", "gemma-3-12b-it-Q4_K_M.gguf", "gemma-4-12B-it-Q4_K_M.gguf",
    "gemma-4-26B-A4B-it-UD-IQ4_XS.gguf")

//! the deltanet-plane engage count for one plane kind: a silently-demoted tag would otherwise pass the parity green on the q8
//! fallback rail
def private dn_kq_engaged(fmts : array<KqFmt>) : int64 {
    var n = 0l
    for (f in fmts) {
        if (f != KqFmt.q8) {
            n++
        }
    }
    return n
}

//! The half-span prefix bar on one live stream, with the loosened check's control on the same stream: the fixture with an id
//! flipped inside its first half must red the bar
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

//! One parity row's arms on its loaded model `tr` (loaded as `parity_row_load` loads it, the tokenizer riding along: text-form
//! prompts encode here, and every compare logs its decoded-text eyeball lines): the frozen greedy ids token for token, or to
//! the half-span prefix bar with its flipped-id control, then the row's chunked, reference and per-expert prefill arms; the
//! main arm on the MoE levers' defaults, whatever an earlier cell left, and the levers put back
[cold_path]
def parity_row_arms(t : T?; spec : ModelSpec; tr : Model) {
    let prev_grouped = get_moe_grouped_prefill()
    let prev_fused = get_moe_fused_expert_gemms()
    set_moe_grouped_prefill(true)
    set_moe_fused_expert_gemms(true)
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
    set_moe_grouped_prefill(prev_grouped)
    set_moe_fused_expert_gemms(prev_fused)
}

//! the CPU form the parity row of `file` loads on, as `parity_row_load` loads it: the row's load mode and K-quant native knob,
//! the environment's backend pin (none where DASLLAMA_PIN_BACKEND is unset)
def parity_spec_form(file : string) : ModelForm {
    var inscope spec <- spec_by_file(file)
    return cpu_cells_form(spec.parity.quant, backend = g_env_engine.pin_backend,
        kq_native = spec.parity.kq_native ? KqNative.on : KqNative.off)
}

//! whether the pass `form` of a block is the parity row `file`'s load (`parity_spec_form`), on its pass that stands alone
def private on_parity_form(form : ModelForm; file : string) : bool => alone_pass_of(form, parity_spec_form(file))

//! The parity row of `file` on the pass `form` of its file's block, where `form` is the row's load and its cells stand alone
//! (`parity_spec_form`): the row's arms at the row's context, the cell `test_parity_specs` names for every row it walks
def parity_spec_cell(t : T?; var m : Model; form : ModelForm; file : string) {
    return if (!on_parity_form(form, file))
    var inscope spec <- spec_by_file(file)
    unsafe {   //! the cell reads the block's model by reference
        t |> run("pinned-greedy parity: {file}") @capture(& m, & spec)(t : T ?) {
            t |> success(has_value(PARITY_BLOCK_ROWS, file), "{file}: PARITY_BLOCK_ROWS names the row, so the spec walk leaves it to its block")
            with_ctx_cut(m, PARITY_ROW_CTX) {
                parity_row_arms(t, spec, m)
            }
        }
    }
}

//! whether the other cells of a block over `forms` see its pass `form`: every pass but the parity row `file`'s own load, which
//! they see where another CPU form of `forms` is that load too
def parity_spec_spares(form : ModelForm; forms : array<ModelForm>; file : string) : bool {
    let pf = parity_spec_form(file)
    return true if (form.on_gpu || !same_cpu_load(form, pf))
    return length([for (g in forms); g; where !g.on_gpu && same_cpu_load(g, pf)]) > 1
}

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

//! `body` on the cut `cut` of the parity row `full`'s file, loaded in `tr`, with the row's prompt, inside the engine's job
//! queue, once its block count `blocks` is read off the load
def private with_cut_prompt(t : T?; tr : Model; cut, full : string; blocks : int64; body : block<(prompt : array<int64>) : void>) {
    var inscope spec <- spec_by_file(full)
    cut_within(t, "{cut}: ", tr, blocks)
    with_dasllama_jobque_() {
        var inscope prompt <- parity_prompt(tr, spec)
        invoke(body, prompt)
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

//! gpt-oss-4L `tr`, the 20b's first four blocks, fed the reference's greedy stream keeps every reference id within 2% of its
//! row's top logit on the short prompt and on the 449-token one that engages the sliding window, the stream one step late
//! outside
def parity_gptoss_cut_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("gpt-oss forward on the four-block cut (native-MXFP4 experts, sinks + YaRN + MoE biases; short + window-engaged), the reference's ids fed") @capture(& tr)(t : T ?) {
            with_cut_prompt(t, tr, "gpt-oss-4L-mxfp4-local.gguf", "gpt-oss-20b-mxfp4.gguf", 4l) $(prompt) {
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

//! the cut `cut` in `tr` (its full file `full`'s first `blocks` blocks) fed its reference stream `ref` after the full file's
//! parity prompt, every reference id within `bar` and the late control on all but one row in `late_per`, logged under `what`;
//! `prompt_ids` past 0 the reference's count of the prompt's ids, asserted
def private cut_cell(t : T?; tr : Model; title, cut, full, what : string; blocks : int64; ref : array<int64>; bar : float;
                     late_per : int = 4; prompt_ids : int = -1) {
    unsafe {
        t |> run(title) @capture(& tr, & ref)(t : T ?) {
            with_cut_prompt(t, tr, cut, full, blocks) $(prompt) {
                if (prompt_ids > 0) {
                    t |> equal(length(prompt), prompt_ids,
                        "the fixture prompt encodes to the reference's {prompt_ids} ids")
                }
                forced_stream_within(t, tr, prompt, ref, bar, what, late_per)
            }
        }
    }
}

//! gemma-4-12B-12L `tr`, the 12B's first twelve blocks, fed the reference's greedy stream after the 1492-id count keeps every
//! reference id within 3% of its row's top logit, the stream one step late outside
def parity_gemma4_12b_cut_cell(t : T?; tr : Model) {
    cut_cell(t, tr, "the dense gemma-4 forward on the twelve-block cut (two global layers, the window engaged), the reference's ids fed",
        "gemma-4-12B-12L-Q4_K_M-local.gguf", "gemma-4-12B-it-Q4_K_M.gguf", "gemma-4-12B-12L-Q4_K_M-local.gguf", 12l,
        GEMMA4_12B_CUT_REF, GEMMA4_12B_CUT_GAP_REL, 4, GEMMA4_CUT_PROMPT_IDS)
}

//! gemma-4-26B-A4B-6L `tr`, the 26B's first six blocks, fed the reference's greedy stream after the 1492-id count keeps every
//! reference id within 2% of its row's top logit, the stream one step late outside
def parity_gemma4_26b_cut_cell(t : T?; tr : Model) {
    cut_cell(t, tr, "the routed gemma-4 forward on the six-block cut (one global layer, the window engaged), the reference's ids fed",
        "gemma-4-26B-A4B-6L-Q4_K_M-local.gguf", "gemma-4-26B-A4B-it-Q4_K_M.gguf", "gemma-4-26B-A4B-6L-Q4_K_M-local.gguf", 6l,
        GEMMA4_26B_CUT_REF, CUT_GAP_REL, 4, GEMMA4_CUT_PROMPT_IDS)
}

//! the Qwen1.5-MoE-A2.7B-4L Q8_0 cut's greedy stream under the
//! reference after the qwen counting fixture (`1, 2, 3, 4, 5, 6, 7,`):
//! the worst reference id reads 0.0083 of the top logit under ours on
//! the M5's CPU chain, the nearest late id 0.31 - the cut's bar `CUT_GAP_REL`
let private QWEN2MOE_CUT_REF <- [220l, 101371l, 112781l, 108339l, 108339l, 102895l, 99710l, 44851l, 104362l, 101353l,
                                 9931l, 31628l, 34794l, 141536l, 102423l, 99829l, 58158l, 119519l, 76111l, 122559l,
                                 56273l, 119261l, 87364l, 52774l, 37160l, 113202l, 111846l, 107698l, 22211l, 117724l,
                                 103712l, 89831l, 105211l, 28330l, 101175l, 100273l, 96922l, 117166l, 1395l, 93994l]

//! Qwen1.5-MoE-A2.7B-4L Q8_0 `tr`, the carrier's first four blocks, fed the reference's greedy stream after the counting
//! fixture keeps every reference id within 2% of its row's top logit, the stream one step late outside
def parity_qwen2moe_cut_cell(t : T?; tr : Model) {
    cut_cell(t, tr, "the routed qwen2moe forward on the four-block cut (60 experts, 4 routed, the gated shared expert), the reference's ids fed",
        "Qwen1.5-MoE-A2.7B-4L-Q8_0-local.gguf", "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf", "Qwen1.5-MoE-A2.7B-4L Q8_0", 4l,
        QWEN2MOE_CUT_REF, CUT_GAP_REL)
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

//! Qwen3.6-35B-A3B-4L UD-Q4_K_M `tr`, the 35B's first four blocks, fed the reference's greedy stream after the counting
//! fixture keeps every reference id within 3.5% of its row's top logit, the stream one step late outside on all but one row in
//! eight
def parity_qwen35moe_cut_cell(t : T?; tr : Model) {
    cut_cell(t, tr, "the deltanet hybrid MoE forward on the four-block cut (three recurrent layers, one attention layer, 256 experts, 8 routed, the gated shared expert), the reference's ids fed",
        "Qwen3.6-35B-A3B-4L-UD-Q4_K_M-local.gguf", "Qwen3.6-35B-A3B-UD-Q4_K_M.gguf", "Qwen3.6-35B-A3B-4L UD-Q4_K_M", 4l,
        QWEN35MOE_CUT_REF, QWEN35MOE_CUT_GAP_REL, QWEN35MOE_CUT_LATE_PER)
}
