options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen35)
require dasllama/dasllama   // create_session, moe_gpu_drop_model

let private HYBRID_WINDOW = 512l   //! the file's prompt window: the two-window cells read the seam, not the window's width, and a 600-token prompt holds it

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm (the model dropped off the device) prefills on the CPU by design
    set_pf_window_rows_pin(HYBRID_WINDOW)   //! before every load of the file: the plan and the window chain read it
}
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_sampling
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_env   // set_pf_window_rows_pin: the file's 512-row window
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the hybrid
require ?vulkan dasllama/dasllama_vulkan_common   // the resolved coopmat mode and the kernel census, where the module exists
require ?vulkan dasllama/dasllama_vulkan_prefill   // pf_f16_feed: the feed admission the driver acts on
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_rdec_head_embed_gathered: the head's rows gathered off their ids
require _model_tier   // models_dir() + model_available() presence/size-tier gate
require _resident_feed
require _kv_codec_cells   // the K/V codec cells' rig: the gated carrier under a block-codec mirror
require _mtp_resident_cells   // the NextN head's cells, one body for every discrete home

// The whole-model resident driver on a deltanet hybrid under DASLLAMA_GPU=1: the resident window
// chain prefills the prompt (the recurrent layers through the conv + chunked scan over their device
// state, the gated partial-rope attention over the mirror), then the resident decode steps every
// layer - the recurrent ones through the fused deltanet step - as one submit per token. The GPU
// chains and the CPU chain are not bit-identical (GPU exp / f32 accumulation order), so the gate is
// the forced-feed logits-tolerance form: the same fixed tokens fed to both arms, the logits within
// a bar of the CPU chain after the prefill and at every step, and the one-step-off control that
// must exceed it. Three fixtures: the Q8_0 file over four prompt lengths - one window, two windows
// (the conv tail and the state carry across the window seam), one past the window, and two tokens
// (both conv-tail edges: the last window shorter than the taps); the K-quant twin (Q4_K_M) over one
// and two windows; and the mixed twin (Q4_K_M with the deltanet out plane at Q8_0) over the same
// two, whose recurrent layers take a K-quant x feed and a Q8_0 o feed - the routing witness below
// holds that the x feed rode the coopmat tiles. The NextN-headed twin (the -MTP- file) holds the
// speculative carry the driver lands - the prefill's last row and every step's post-final-norm
// hidden as `mtp_h`, its watermark as `mtp_h_pos1` - one row at a time and through the N-row command,
// and the NextN draft through the head the driver homes against the CPU `forward_mtp` on one token,
// carry and row, then the round's drafts with the draft seat unset and put back, and the round's
// same-slab verify rows within twice the deltanet bar of one-row steps of the same tokens, the argmax the same, accepted and rejected,
// and the head's prompt warm on the window chain: the host residual rows poisoned before a resident
// prefill leave the draft bit for bit, the head's warmed K rows against the CPU warm's within twice
// the deltanet bar over one window and over two, and six speculative rounds against the CPU round from
// one prompt. The gates around the round: speculation off lands no carry, a NaN carry panics the draft
// by row, the pass arms (codec, unhomed, gap) hand the round to the CPU rails by name and the CPU verify
// completes over the recurrent state the CPU rails hold, a rewind passes the verify by name and the CPU
// rails then refuse it by name, and a head kept off the device passes the verify as verify_rows with
// the resident prefill declining the CPU verify's window as verify_window.
// Every model cell skips interpreted, without its model file, or without the armed tier; the
// K-quant cells and the codec pass arm skip on the f32 mirrors (`DASLLAMA_VK_KV32=1`) too.

//! the K-quant bar on the f16 feed and mirror: CPU-vs-device rounding sits flat at 0.39-0.57 of a ~12.5 max logit on the RTX PRO
//! 4500 (the Q8 file 0.19-0.36) and 0.45-0.79 of a 10.9-15.6 max logit on the RTX 5060 Ti, with no growth and the one-step-off
//! control well past it; the device's distance from the file's f32 chain is held beside it (`f32_chain_witness`)
let private DN_LOGIT_BAR_REL_KQ_F16_FEED = 0.08
//! the K-quant bar on the quant feed (no coopmat tile serves the planes, the prefill's activations ride Q8_K): the mixed twin's two-window cell under `DASLLAMA_COOPMAT=sdot4` on the RTX 5060 Ti reads 0.084 of an 11.6 max logit at one step, argmax unchanged, the rest under 0.05
let private DN_LOGIT_BAR_REL_KQ_QUANT_FEED = 0.10
//! the mixed twin's bar on the f16 feed: its CPU chain quantizes the activations twice a recurrent layer (Q8_K for qkv/z, Q8_0
//! for out), so the device and the CPU chain round on either side of the file's f32 chain - one step reads 0.91 of a 12.3 max
//! logit apart (the device 0.63 and the CPU chain 0.50 from the f32 row, the device's pick the f32 chain's), the rest 0.42-0.60
let private DN_LOGIT_BAR_REL_KQ_Q8OUT = 0.08

//! the plane formats a fixture's deltanet layers carry: all Q8_0, all K-quant, or K-quant qkv/z beside a Q8_0 out plane
enum HybridPlanes {
    q8
    kq
    kq_q8out
}

//! the K-quant cm2 tile dispatches of format `f` so far (the l, m and s columns summed); NO_CENSUS with no vulkan module
def private kq_cm2_tiles_census(f : KqFmt) : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let stem = kq_tile_stem("{f}")
        return census("{stem}_cm2l_cls_spv") + census("{stem}_cm2m_cls_spv") + census("{stem}_cm2s_cls_spv")
    } else {
        return NO_CENSUS
    }
}

//! the driver's own answer: does the prefill's f16 feed admit a K-quant plane on the armed coopmat mode
def private f16_feed_admits_kq() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return pf_f16_feed(int(KqFmt.k4))
    } else {
        return false
    }
}

// prefill `n` tokens + 8 fed steps on the resident driver, then the same on the all-CPU chain; the
// prefill logits and every step's within the deltanet bar, the one-step-off control past it.
[cold_path]
def private hybrid_forced_feed(t : T?; n : int64; file : string; planes : HybridPlanes) {
    let kq = planes != HybridPlanes.q8
    let q8out = planes == HybridPlanes.kq_q8out
    let path = path_join(models_dir(), file)
    if (!resident_ready(t, path)) return
    if (kq && mirror_kv_dtype() == KVDtype.f32) {
        t |> skip("the K-quant bar is calibrated on the f16 mirror the tier arms; the f32 A/B mirror (DASLLAMA_VK_KV32=1) is no gate for it")
        return
    }
    var tr <- load_model_(path, QuantMode.q8)
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the hybrid (the load log names any decline)")
    var nrec = 0l
    var nrec_ba_kq = 0l   // the recurrent layers whose beta or alpha plane is K-quant: the window requantizes the row to Q8_K for them on the f16 feed too
    if (kq) {
        var native = 0l
        for (l in range64(tr.config.n_layers)) {
            if (!layer_is_recurrent(tr.config, l)) continue
            nrec++
            if (kq_sb(int(fmt_at(tr.dnbeta_fmt, l))) || kq_sb(int(fmt_at(tr.dnalpha_fmt, l)))) {
                nrec_ba_kq++
            }
            if (fmt_at(tr.dnqkv_fmt, l) != KqFmt.q8 && fmt_at(tr.dngate_fmt, l) != KqFmt.q8
                    && ((fmt_at(tr.dnout_fmt, l) == KqFmt.q8) == q8out)) {
                native++
            }
        }
        let out_form = q8out ? "the out plane Q8_0" : "the out plane too"
        t |> success(native > 0l, "the loader kept the deltanet qkv/z planes in their K-quant file formats, {out_form} ({native} recurrent layers)")
    }
    with_dasllama_jobque_() {
        var prompt <- resident_prompt(n)
        var toks <- resident_feed(8l)
        let rq0 = census("cls_q8k_rq_spv")
        let tiles0 = kq_cm2_tiles_census(KqFmt.k4)
        var prefill_q8k_rq = NO_CENSUS
        var prefill_k4_tiles = NO_CENSUS
        var fed <- resident_then_cpu_feed(t, tr, prompt, toks, $ {
            prefill_q8k_rq = rq0 == NO_CENSUS ? NO_CENSUS : census("cls_q8k_rq_spv") - rq0
            prefill_k4_tiles = tiles0 == NO_CENSUS ? NO_CENSUS : kq_cm2_tiles_census(KqFmt.k4) - tiles0
        }) $ {}
        if (kq && prefill_q8k_rq != NO_CENSUS) {
            // off the f16 feed the x route requants once per recurrent layer per window; on it only the K-quant beta / alpha planes' GEMMs read a Q8_K row (one a recurrent layer a window), the qkv / z pair the f16 rows on the K-quant cm2 tiles
            let windows = (n + HYBRID_WINDOW - 1l) / HYBRID_WINDOW
            to_log(LOG_INFO, "prefill ({n} tokens, {windows} windows) [{tile_arm_tag()}]: {prefill_q8k_rq} Q8_K requants over {nrec} recurrent layers ({nrec_ba_kq} with K-quant beta / alpha planes), {prefill_k4_tiles} k4 cm2 tiles, f16 feed admits kq {f16_feed_admits_kq()}\n")
            if (f16_feed_admits_kq()) {
                t |> success(prefill_k4_tiles > 0l, "on the f16 feed the K-quant qkv/z pair's GEMMs ride the k4 cm2 tiles ({prefill_k4_tiles} dispatches) [{tile_arm_tag()}]")
                t |> success(prefill_q8k_rq <= windows * nrec_ba_kq + 1l, "on the f16 feed the prefill requantized the row to Q8_K for the K-quant beta / alpha planes alone, once a recurrent layer a window, and never for the qkv/z pair ({prefill_q8k_rq} Q8_K requants, {nrec_ba_kq} such layers, {windows} windows, the classifier's one) [{tile_arm_tag()}]")
            } else {
                t |> equal(prefill_k4_tiles, 0l, "off the f16 feed no k4 cm2 tile serves the pair [{tile_arm_tag()}]")
                t |> success(prefill_q8k_rq >= windows * nrec, "off the f16 feed the K-quant qkv/z pair reads the Q8_K form ({prefill_q8k_rq} Q8_K requants, {nrec} recurrent layers, {windows} windows) [{tile_arm_tag()}]")
            }
        }
        let f16_rel = q8out ? DN_LOGIT_BAR_REL_KQ_Q8OUT : DN_LOGIT_BAR_REL_KQ_F16_FEED
        let rel = kq ? (f16_feed_admits_kq() ? f16_rel : DN_LOGIT_BAR_REL_KQ_QUANT_FEED) : DN_LOGIT_BAR_REL
        feed_steps_within(t, tr, "", n, fed.gpu, fed.cpu, toks, rel)
        if (kq) {   // the K-quant carriers' same-or-better witness: the device no further from the file's f32 chain than the CPU q8 chain
            var f32_rows <- f32_chain_rows(path, prompt, toks)
            f32_chain_witness(t, tr, "{n} tokens against the f32 chain [{tile_arm_tag()}]", fed, f32_rows)
            delete f32_rows
        }
        delete fed
        delete prompt
        delete toks
    }
    delete tr
}

[test]
def test_gpu_resident_hybrid_decode(t : T?) {
    t |> run("Qwen3.5-0.8B: resident prefill + decode with deltanet layers on device vs the CPU chain, one window") @(t : T?) {
        hybrid_forced_feed(t, 40l, "Qwen3.5-0.8B-Q8_0.gguf", HybridPlanes.q8)
    }
}

[test]
def test_gpu_resident_hybrid_two_windows(t : T?) {
    t |> run("Qwen3.5-0.8B: the resident prefill across two windows (the conv tail and state carry the seam)") @(t : T?) {
        hybrid_forced_feed(t, HYBRID_WINDOW + 88l, "Qwen3.5-0.8B-Q8_0.gguf", HybridPlanes.q8)
    }
}

// the conv-tail edge: a prompt one past the window (the last window would carry one row, fewer
// than the three conv taps - the chain keeps it at the taps by shortening the window before), and
// a prompt shorter than the taps (one window, the ring's leading rows zero)
[test]
def test_gpu_resident_hybrid_short_windows(t : T?) {
    t |> run("Qwen3.5-0.8B: a prompt one past the window - the last window still carries the conv taps") @(t : T?) {
        hybrid_forced_feed(t, HYBRID_WINDOW + 1l, "Qwen3.5-0.8B-Q8_0.gguf", HybridPlanes.q8)
    }
    t |> run("Qwen3.5-0.8B: a two-token prompt - one window shorter than the conv taps") @(t : T?) {
        hybrid_forced_feed(t, 2l, "Qwen3.5-0.8B-Q8_0.gguf", HybridPlanes.q8)
    }
}

[test]
def test_gpu_resident_hybrid_greedy_chain(t : T?) {
    t |> run("Qwen3.5-0.8B: the greedy chain's steps are the plain steps bit for bit, hits on the pick and misses off it") @(t : T?) {
        greedy_chain_cell(t, path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf"))
    }
}

[test]
def test_gpu_resident_hybrid_kq(t : T?) {
    t |> run("Qwen3.5-0.8B-Q4_K_M: K-quant deltanet planes on the resident driver vs the CPU chain, one window") @(t : T?) {
        hybrid_forced_feed(t, 40l, "Qwen3.5-0.8B-Q4_K_M.gguf", HybridPlanes.kq)
    }
    t |> run("Qwen3.5-0.8B-Q4_K_M: the same across two windows") @(t : T?) {
        return if (!assert_pf_window(t, HYBRID_WINDOW))
        hybrid_forced_feed(t, HYBRID_WINDOW + 88l, "Qwen3.5-0.8B-Q4_K_M.gguf", HybridPlanes.kq)
    }
}

[test]
def test_gpu_resident_hybrid_kq_q8out(t : T?) {
    t |> run("Qwen3.5-0.8B-Q4_K_M-q8out: K-quant qkv/z with a Q8_0 out plane on the resident driver vs the CPU chain, one window") @(t : T?) {
        hybrid_forced_feed(t, 40l, "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", HybridPlanes.kq_q8out)
    }
    t |> run("Qwen3.5-0.8B-Q4_K_M-q8out: the same across two windows") @(t : T?) {
        return if (!assert_pf_window(t, HYBRID_WINDOW))
        hybrid_forced_feed(t, HYBRID_WINDOW + 88l, "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", HybridPlanes.kq_q8out)
    }
}

//! the gated carrier under a block codec: the q8_0 mirror against the CPU chain on q8_0 sessions reads 0.21-0.31 of a
//! 10.7-12.3 max logit (the f16 mirror's own reading); under tq4 the device's summed distance from the f16 chain reads 1.08
//! of the CPU tq4 chain's own (9.2 against 8.5, the f16 rows one step off 9.2 times it) and the two tq4 chains 0.63 of it
//! apart (the CPU tq4 rows one step off 9.3 times it)
let private GATED_Q8KV_BAR_REL = 0.06
let private GATED_CODEC_DIST_REL = 1.15
let private GATED_CODEC_PAIR_REL = 0.8

[test]
def test_gpu_resident_hybrid_gated_block_codecs(t : T?) {
    let file = "Qwen3.5-0.8B-Q8_0.gguf"
    t |> run("Qwen3.5-0.8B: a q8_0 K/V mirror under the gated attention - resident prefill + decode vs the CPU chain on q8_0 sessions") @(t : T?) {
        cell_codec_forced_feed(t, file, 40l, KVDtype.q8_0, GATED_Q8KV_BAR_REL)
    }
    t |> run("Qwen3.5-0.8B: a tq4 K/V mirror under the gated attention - the un-rotation applies the head gate - against the CPU chain's tq4 and f16 rows") @(t : T?) {
        cell_codec_distance(t, file, 40l, KVDtype.tq4, false, GATED_CODEC_DIST_REL, GATED_CODEC_PAIR_REL)
    }
}


[test]
def test_gpu_resident_hybrid_sampled_selection(t : T?) {
    mtp_sampled_selection_cell(t)
}

[test]
def test_gpu_resident_hybrid_peel(t : T?) {
    t |> run("Qwen3.5-0.8B: a short tail peels through the token commands, the recurrent rows in order in the region's slot, and reads what the one-row steps read") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
        if (!resident_ready(t, path)) return
        var tr <- load_resident(path, 1l)
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the hybrid (the load log names any decline)")
        with_dasllama_jobque_() {
            recurrent_peel_cell(t, tr, SPLIT_BAR_REL)
        }
        delete tr
    }
    t |> run("Qwen3.5-0.8B-MTP: the same on the NextN-headed carrier with speculation off, whose peel warms no head row; with it on, a short tail takes the chain, which warms the head's rows") @(t : T?) {
        with_mtp_carrier(t, 1l) $(var tr) {
            let spec0 = get_mtp_spec()
            set_mtp_spec(false)
            recurrent_peel_cell(t, tr, SPLIT_BAR_REL)
            set_mtp_spec(true)
            assert_short_window_takes_chain(t, tr, "a NextN-headed model under speculation")
            set_mtp_spec(spec0)
        }
    }
}

[test]
def test_kernel_census_by_name(t : T?) {
    t |> run("the kernel census refuses a name no kernel seeded") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            var tripped = false
            try {
                vk_kernel_coverage_of("no_such_kernel_spv")
            } recover {
                tripped = true
            }
            t |> success(tripped, "an unseeded census name panics, so a misspelt key cannot read as a zero count")
            t |> equal(vk_kernel_coverage_of("cls_q8k_rq_spv") >= 0l, true, "a seeded class kernel answers by its SPIR-V name, device or not")
        } else {
            t |> skip("no vulkan module in this build")
        }
    }
}

[test]
def test_row_bar_nan_reference(t : T?) {
    let rel = 0.04
    let cpu <- [1.0, -4.0, 2.0, 0.5]
    var gpu := cpu
    gpu[0] += 0.1
    var nan_cpu := cpu
    nan_cpu[2] = unsafe(reinterpret<float>(NAN_BITS))
    t |> equal(row_bar(cpu, rel), rel * 4.0, "the bar is rel of the row's largest |logit|")
    t |> success(logits_maxdiff(gpu, cpu) <= row_bar(cpu, rel), "a device row within the bar of a finite reference holds it")
    t |> equal(row_bar(nan_cpu, rel), rel * 4.0, "a NaN in the reference row scales no bar - the finite elements set it")
    t |> success(!(logits_maxdiff(gpu, nan_cpu) <= row_bar(nan_cpu, rel)), "a NaN in the reference row reads past the bar")
    t |> success(logits_maxdiff(gpu, nan_cpu) <= rel * logits_maxabs(nan_cpu), "control: the NaN-scaled bar is infinite and the same row holds it")
}

//! the asserts `rows_within_control` fails on a probe over the one device row `g` against the reference `c`, `off` its row one off
def private rows_bar_fails(g, c, off : array<float>) : int {
    return probe_fails() $(p) {
        row_within_ctrl(p, "probe", g, c, off, 0.04, "row")
    }
}

//! the asserts `tq4_within` fails on a probe over `g` against `c`, `off` its row one off
def private tq4_bar_fails(g, c, off : array<float>) : int {
    return probe_fails() $(p) {
        tq4_within(p, g, c, off, "probe")
    }
}

//! the resident rigs' row bars on synthetic rows, run on a probe handle: a NaN in the device row or the reference row reds
//! `rows_within_control` and `tq4_within`, the clean rows hold both (the control)
[test]
def test_row_bars_red_a_nan(t : T?) {
    let cpu <- [1.0, -4.0, 2.0, 0.5]
    let off <- [3.0, -1.0, -2.0, 4.5]
    var gpu := cpu
    gpu[0] += 0.05
    var nan_gpu := gpu
    nan_gpu[1] = unsafe(reinterpret<float>(NAN_BITS))
    var nan_cpu := cpu
    nan_cpu[2] = unsafe(reinterpret<float>(NAN_BITS))
    t |> equal(rows_bar_fails(gpu, cpu, off), 0, "control: rows_within_control holds a device row within the bar, its row one off past it")
    t |> success(rows_bar_fails(nan_gpu, cpu, off) > 0, "rows_within_control reds a NaN in the device row")
    t |> success(rows_bar_fails(gpu, nan_cpu, off) > 0, "rows_within_control reds a NaN in the reference row")
    t |> equal(tq4_bar_fails(gpu, cpu, off), 0, "control: tq4_within holds a device row nearer the reference than its share of the row one off")
    t |> success(tq4_bar_fails(nan_gpu, cpu, off) > 0, "tq4_within reds a NaN in the device row")
    t |> success(tq4_bar_fails(gpu, nan_cpu, off) > 0, "tq4_within reds a NaN in the reference row")
    //! the share's bound has teeth at TQ4_CTRL_SHARE: the row one off sits 4.0 from the reference, so a device row 0.8 of that
    //! away holds and one 0.9 of it away reds
    var near := cpu
    near[0] += 0.8 * 4.0
    var far := cpu
    far[0] += 0.9 * 4.0
    t |> equal(tq4_bar_fails(near, cpu, off), 0, "control: tq4_within holds a device row at 0.8 of the one-off row's distance")
    t |> success(tq4_bar_fails(far, cpu, off) > 0, "tq4_within reds a device row at 0.9 of the one-off row's distance - past TQ4_CTRL_SHARE")
    //! a row with no control row (the hybrid carry's row 0): the NaN reference still reds through the bar alone
    var gr <- [ <- clone_to_move(gpu)]
    var cr <- [ <- clone_to_move(nan_cpu)]
    let alone = probe_fails() $(p) {
        rows_within_control(p, "probe", gr, cr, 0.04, -1l, 1.0) $(_r) => "row"
    }
    t |> success(alone > 0, "rows_within_control reds a NaN in the reference row where no control row exists")
    delete gr
    delete cr
}

[test]
def test_gpu_resident_hybrid_mtp_depth8(t : T?) {
    mtp_depth8_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_carry(t : T?) {
    mtp_carry_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_carry_batched(t : T?) {
    mtp_carry_batched_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_carry_gate(t : T?) {
    mtp_carry_gate_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_draft(t : T?) {
    mtp_draft_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_draft_upload(t : T?) {
    mtp_draft_upload_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_draft_suppressed(t : T?) {
    mtp_draft_suppressed_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_verify(t : T?) {
    mtp_verify_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_verify_asked(t : T?) {
    mtp_verify_asked_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_verify_reject(t : T?) {
    mtp_verify_reject_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_verify_reject_asked(t : T?) {
    mtp_verify_reject_asked_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_xb_poison(t : T?) {
    mtp_xb_poison_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_head_warm(t : T?) {
    mtp_head_warm_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_rounds(t : T?) {
    mtp_rounds_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_nonfinite(t : T?) {
    mtp_nonfinite_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_pass_arms(t : T?) {
    mtp_pass_arms_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_rewind_refused(t : T?) {
    mtp_rewind_refused_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_head_off(t : T?) {
    mtp_head_off_cell(t)
}

[test]
def test_gpu_resident_hybrid_mtp_block_codec(t : T?) {
    mtp_block_codec_cell(t)
}
