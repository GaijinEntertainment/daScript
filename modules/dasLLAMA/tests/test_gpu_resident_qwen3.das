options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen3)
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require dasllama/dasllama_kv_dtype   // KVDtype: the tq4 forms' mirror ask
require dasllama/dasllama_math   // with_dasllama_jobque_: the compares after the block decode off the queue
require ?cuda _cuda_feed   // LOGIT_BAR_REL: the CUDA home's dense bar
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms, FeedRows, the CUDA feed collector, the gather cell's halves
require _resident_regions   // the regions cells' collections, REGIONS_RIG_CTX, the fuse bisect's bits
require _resident_feed   // Held, held_within, held_pf_window
require _kv_codec_cells   // the tq4 distance and stored-rows halves, the per-op cells

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // each block's CPU form, and the by-design passes, prefill on the CPU
}

// The dense qwen3 family on the whole-model driver under DASLLAMA_GPU=1, on the Vulkan home or under DASLLAMA_CUDA=1 on the
// CUDA home. Each model file is ONE block (`with_cpu_and_gpu_model`): its forms load once each and the CPU form once a mirror
// codec, the cells collect what the compares read and the compares follow the block.
// - Qwen3-0.6B Q8_0 (`test_gpu_resident_qwen3`, the q/k-norm carrier, no attention bias): at two regions the rig's step
//   cells - the interleaved single steps, the single steps past the wide position on the wide twin, a batched step before
//   any one-row step, the batched steps at one window and past the unsplit position (the N-row token command's q/k norm in
//   the form the one-row command takes, bit for bit against the sessions alone), and the window's fused q/k norm, rope and
//   store pass against its split pair; DASLLAMA_VK_FUSE_BISECT one load a bit within the split bar of the unsplit load;
//   under a tq4 K/V mirror (a Q/K/V-biased Qwen2.5 serves a tq4 ask at q8_0, so it cannot carry these) the distance form at
//   one window and two and with the prompt on the CPU rails, layer 0's stored rows against the CPU cache's, and every cell of
//   the rig on the rotated 4-bit codec (the staged stores and the rotate-and-quantize pass at one row and at N rows, the
//   decode attention's tq4 stamps, the prefill's attention over the f16 shadow, the rows a pass brings down as the CPU
//   cache's own bytes; the tolerance cells hold the rig's tq4 bars); with the whole-model route off the per-op rails under
//   tq4 and q8_0 sessions, and the load's decline reason; on the CUDA home the forced feed over one window and two, the
//   fused CuQknRopeKv pass, with the driver's counters and their control.
// - Qwen3-4B-Instruct-2507: the Q8_0 file's CUDA forced feed at its 2560-wide rows over 36 layers
//   (`test_gpu_resident_qwen3_4b`, `--category qwen3-4b`, `../TEST_CATEGORIES.md`), and the Q4_K_M file - a format carrier,
//   in every run - its tied K-quant table gathered on the device beside its per-op rails under tq4 and q8_0 sessions, the
//   K-quant format of a codec's parity (`test_gpu_resident_qwen3_4b_kq`).
// Every test skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, without the file under models_dir(),
// or where the plan does not fit the card. The tq4 forms skip where DASLLAMA_VK_KV32=1 arms an f32 mirror over the codec;
// the two-window feeds where the prompt window is not PF_WINDOW_ROWS rows. Under DASLLAMA_CUDA=1 the fuse bisect, the fused
// q/k norm, the tq4 distance and stored-rows cells, the per-op cells and the 4B Q4_K_M file - the Vulkan home's - give way
// to the CUDA feeds.

//! no logit bar holds a 4-bit mirror on a 0.6B, so the codec bars hold ratios of the rows' summed largest differences: the
//! device's distance from the f16 chain, of the CPU codec chain's own, reads 0.95-1.01 (the tq4 rows 1.17 on the RTX PRO 4500,
//! driver 595), the f16 rows one step off 1.6-3.7
let private TQ4_DIST_REL = 1.25
//! the two codec chains part further on Qwen3-0.6B than on the biased qwen2 carrier: 0.82 of the CPU codec chain's distance from
//! f16 against its 0.61 (the llama files read 1.0-1.1)
let private TQ4_PAIR_REL = 1.0
//! layer 0's stored bytes against the CPU cache's: the q/k norm before the rope lands the device's normed k on the 4-bit quant's
//! edge on 0.049 of layer 0's K bytes; the steps' V rows at 0.002-0.01 (one Q8_0 feed on both chains); the prefill's V rows at
//! 0.12 - its GEMM reads f16 rows where the CPU reads Q8_0 blocks - against 0.97 for a row held to the next one (the control)
let private TQ4_ROWS_BARS = StoredRowsBars(k = 0.08, v_step = 0.05, v_prefill_of_ctrl = 0.6)
//! the per-op rails: layer 0's cache bytes, the device arm's against the CPU chain's - one input, so they agree but for a quant
//! on a rounding edge (0 of the bytes under tq4, 2e-5 under q8_0). The device's summed distance from the f16 chain reads 2.3
//! against the CPU codec chain's 48.7 where the device prefilled and 38.4 against 42.6 over rows the CPU quantized; the f16
//! rows one step off sum to 80-95
let private QWEN3_06B_PEROP_BARS = PeropBars(rows = 0.05, dist_rel = 1.0)
//! the 4B Q4_K_M's per-op rails: the stored bytes differ from the CPU chain's at 0 to 4e-5 of them; the device's summed distance
//! from the f16 chain reads 0.28 of the CPU codec chain's own under tq4 (3.3 against 12.0) and 0.80 under q8_0 (3.3 against 4.1),
//! the f16 rows one step off 5.7 and 16 times it
let private QWEN3_4B_PEROP_BARS = PeropBars(rows = 0.05, dist_rel = 1.0)
//! the Qwen3-4B Q4_K_M chain's device-vs-CPU distance: its K-quant sites read 0.38-0.62 of a 7.3-11.3 max logit (0.073 at its
//! widest step, the argmax the CPU's at every row) - the chain's rounding: the gathered rows are the class lane decode
//! (`test_vkd_emb_gather_family`); the one-step-off control reads 5.4 and past
let private QWEN_KQ_BAR_REL = 0.10


//! the CUDA home's forced feeds of a qwen3 file, one window and two (`dense_feeds`), the file's family held
def private qwen3_cuda_rows(var c : DenseFeeds; m : Model; form : ModelForm) {
    if (dense_feeds_rows(c, m, form) == MainSide.device) {
        c.carrier |> held_equal(m.arch, "qwen3", "the file is a qwen3-family model")
    }
}

//! the CUDA feeds' compares after the block at the CUDA home's dense bar
[unused_argument(file, c)]   // without the cuda module no feed ran
def private qwen3_cuda_within(t : T?; file : string; c : DenseFeeds) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        dense_feeds_within(t, file, c, LOGIT_BAR_REL)
    }
}

//! the tq4 mirror's distance form and stored rows on Qwen3-0.6B: the prompts, the device's rows at each (the plain tq4 form)
//! and with the prompt on the CPU rails (the tq4 form with the planes on the host), layer 0's stored rows on both sides, and the
//! CPU form's rows at tq4 and at f16 for each prompt; each `_ran` says its device form collected
struct Qwen3CodecRows {
    feeds : array<int64>
    gpu : array<array<array<float>>>
    ran : bool
    sides : CodecSides
    cpu : array<FeedRows>
    f16 : array<FeedRows>
    h : Held
}

//! what the Qwen3-0.6B file's forms collect: the CUDA feeds, the two-region form's step cells, the fuse bisect's base rows and
//! its bits' readings, the tq4 codec cells, and the rig's cells on the tq4 two-region forms
struct Qwen3Rows {
    feeds : DenseFeeds
    regions : Held
    bisect_base : array<array<float>>
    bisect_ran : bool
    bisect : Held
    codec : Qwen3CodecRows
    tq4_regions : Held
    perop : PeropRun
}

//! the per-op cells on Qwen3-0.6B, a form each: a tq4 session above the device prefill's floor and under it, a q8_0 session
def private qwen3_06b_perop_cells() : array<PeropCell> => <- [PeropCell(n = 40l, kv = KVDtype.tq4, bars = QWEN3_06B_PEROP_BARS),
    PeropCell(n = 20l, kv = KVDtype.tq4, bars = QWEN3_06B_PEROP_BARS), PeropCell(n = 40l, kv = KVDtype.q8_0, bars = QWEN3_06B_PEROP_BARS)]

//! the Qwen3-0.6B file's forms: two regions (the step cells), on the CUDA home the default (the feeds); on the Vulkan home the
//! fuse bisect's load a bit (no bit first, the base) and the per-op form; under a tq4 mirror (a codec the environment keeps off
//! held as a skip) on the Vulkan home the plain form (the distance form and the stored rows) and the planes on the host (the
//! prompt on the CPU rails), on both homes two regions (the rig), the same with the planes on the host (the pin and the gap
//! pass), at the capped context (the cap pass), and the per-op form asking the codec (the decline reason)
def private qwen3_forms(var r : Qwen3Rows) : array<ModelForm> {
    let cuda = gpu_want_cuda()
    var forms <- [device_form(regions = 2l, ctx = REGIONS_RIG_CTX)]
    if (cuda) {
        forms |> push(device_form())
    } else {
        forms |> reserve(length(forms) + length(FUSE_BISECT_BITS) + 2)
        forms |> push(device_form(regions = 2l, ctx = REGIONS_RIG_CTX, fuse_bisect = 0l))
        for (bit in FUSE_BISECT_BITS) {
            forms |> push(device_form(regions = 2l, ctx = REGIONS_RIG_CTX, fuse_bisect = bit))
        }
        r.perop.cells <- qwen3_06b_perop_cells()
        forms |> push_from(perop_forms(r.perop))
    }
    let why = codec_env_why(KVDtype.tq4)
    if (!empty(why)) {
        r.codec.h |> held_skip("the tq4 mirror's cells: {why}")
        return <- forms
    }
    if (!cuda) {
        forms |> push(device_form(kv = KVDtype.tq4))
        forms |> push(device_form(kv = KVDtype.tq4, host = true))
    }
    forms |> push_from(regions_rig_forms(KVDtype.tq4))
    return <- forms
}

//! one form's share of the tq4 codec cells: the device's rows on the tq4 forms, the CPU form's at tq4 and f16 on its tq4 run
def private qwen3_codec_rows(t : T?; var c : Qwen3CodecRows; m : Model; form : ModelForm) {
    if (form.on_gpu && form.host) {
        codec_up_rows(t, c.sides, m, KVDtype.tq4)
    } elif (form.on_gpu) {
        c.gpu |> reserve(length(c.feeds))
        for (n in c.feeds) {
            c.gpu |> emplace <| codec_device_rows(t, m, n, KVDtype.tq4, false)
        }
        codec_stored_device_rows(t, c.sides, m, KVDtype.tq4)
        c.ran = true
    } elif (form.kv == KVDtype.tq4 && (c.ran || c.sides.up_ran)) {
        var f16 = form
        f16.kv = KVDtype.f16
        c.cpu |> reserve(length(c.feeds))
        c.f16 |> reserve(length(c.feeds))
        for (n in c.feeds) {
            c.cpu |> emplace <| resident_feed_rows(m, form, n, CODEC_FED_STEPS)
            c.f16 |> emplace <| resident_feed_rows(m, f16, n, CODEC_FED_STEPS)
        }
        if (c.ran) {
            c.sides.stored_host <- codec_stored_host(m, KVDtype.tq4)
        }
    }
}

//! the tq4 codec cells' compares after the block: each prompt's distance form, the distance form with the prompt on the CPU
//! rails (its prompt's row out of the sums), and layer 0's stored rows
def private qwen3_codec_within(t : T?; c : Qwen3CodecRows) {
    held_within(t, c.h, "Qwen3-0.6B tq4: ", false)
    return if (empty(c.cpu))
    for (n, i in c.feeds, count()) {
        if (c.ran) {
            unsafe {
                t |> run("Qwen3-0.6B: the tq4 K/V mirror - resident prefill + decode over {n} tokens against the CPU chain's tq4 and f16 rows") @capture(& c)(t : T ?) {
                    codec_distances_within(t, "tq4, {n} tokens", c.gpu[i], c.cpu[i].rows, c.f16[i].rows, 0, TQ4_DIST_REL,
                            TQ4_PAIR_REL) $(id) {
                        return feed_piece(c.cpu[i], c.f16[i], id)
                    }
                }
            }
        }
        if (c.sides.up_ran && n == 40l) {
            unsafe {
                t |> run("Qwen3-0.6B: the device steps over tq4 rows the CPU prefill wrote") @capture(& c)(t : T ?) {
                    codec_distances_within(t, "tq4, 40 tokens, the prompt on the CPU rails", c.sides.up, c.cpu[i].rows, c.f16[i].rows, 1,
                            TQ4_DIST_REL, TQ4_PAIR_REL) $(id) {
                        return feed_piece(c.cpu[i], c.f16[i], id)
                    }
                }
            }
        }
    }
    if (c.ran) {
        codec_stored_cell(t, "Qwen3-0.6B: the tq4 mirror's stored rows against the CPU cache's, layer 0", KVDtype.tq4, c.sides,
            TQ4_ROWS_BARS)
    }
}

//! one form's collection of the Qwen3-0.6B file's cells into `r`; the per-op forms', the regions forms' and the fuse bisect's
//! own cells run on their load
def private qwen3_rows(t : T?; var r : Qwen3Rows; var m : Model; form : ModelForm) {
    if (form.perop) {
        if (form.kv_asked) {
            regions_route_off_cell(t, m)
        } else {
            perop_next(t, r.perop, m)
        }
    } elif (form.on_gpu && form.fuse_bisect == 0l) {
        r.bisect_base <- regions_bisect_base(m)
        r.bisect_ran = true
    } elif (form.on_gpu && form.fuse_bisect > 0l) {
        if (r.bisect_ran) {
            regions_bisect_rows(r.bisect, m, form.fuse_bisect, r.bisect_base)
        } else {
            r.bisect |> held_skip("fuse bisect bit {form.fuse_bisect}: the base form with no bit split did not run")
        }
    } elif (form.on_gpu && form.regions == 2l && form.kv_asked) {
        regions_rig_form_rows(t, r.tq4_regions, m, form, SPLIT_BAR_REL)
    } elif (form.on_gpu && form.regions == 2l) {
        regions_interleaved_rows(r.regions, m)
        regions_wide_rows(r.regions, m)
        regions_batched_first_rows(r.regions, m)
        regions_batched_rows(r.regions, m)
        regions_split_rows(r.regions, m)
        let qkn_why = vulkan_home_why("the window's fused q/k norm is the Vulkan driver's lever")
        if (!empty(qkn_why)) {
            r.regions |> held_skip(qkn_why)
        } else {
            regions_window_qkn_rows(r.regions, m)
        }
    } elif (form.on_gpu && form.kv_asked) {
        qwen3_codec_rows(t, r.codec, m, form)
    } else {
        qwen3_cuda_rows(r.feeds, m, form)
        if (!form.on_gpu) {
            qwen3_codec_rows(t, r.codec, m, form)
        }
    }
}

[test(description = "On Qwen3-0.6B Q8_0 over two mirror regions two sessions stepped interleaved, past the wide position on the wide twin, batched first, batched at one window and past the unsplit position each read what that session reads alone, and the window's fused q/k norm, rope and store pass reads within the split bar of the split pair; every DASLLAMA_VK_FUSE_BISECT bit serves one-row logits within the split bar of the unsplit load and the split-pair rows batch bit for bit; a tq4 K/V mirror sits within 1.25 times the CPU tq4 chain's distance from the f16 rows over one and two windows and over rows the CPU prefill wrote, its layer 0 stored rows match the CPU cache's within the byte bars, and the rig's cells - interleaved, continued, batched, device-home, parked, guarded, reported, scheduled, the pin and the gap and cap passes - hold the tq4 bars; on the per-op rails tq4 and q8_0 sessions keep layer 0's cache bytes within 5 percent of the CPU chain's and the device no further from f16 than the CPU codec chain, and the route-off load reports never attempted; on the CUDA driver the fused CuQknRopeKv pass keeps forced-feed logits within 6 percent of the CPU chain over one and two windows; each form loads once")]
def test_gpu_resident_qwen3(t : T?) {
    return if (!family_on(t, "qwen3"))
    var r : Qwen3Rows
    if (gpu_want_cuda()) {
        dense_feeds(r.feeds, [40l, TWO_WINDOWS])
    } elif (held_pf_window(r.codec.h)) {
        r.codec.feeds <- [40l, TWO_WINDOWS]
    } else {
        r.codec.feeds <- [40l]
    }
    var forms <- qwen3_forms(r)
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3-0.6B-Q8_0.gguf", REGIONS_RIG_CTX, forms) $(m, form) {
            qwen3_rows(t, r, m, form)
        }
    }
    if (ran) {
        qwen3_cuda_within(t, "Qwen3-0.6B-Q8_0.gguf", r.feeds)
        held_within(t, r.regions, "Qwen3-0.6B Q8_0, two regions: ", false)
        held_within(t, r.bisect, "Qwen3-0.6B Q8_0, the fuse bisect: ", false)
        held_within(t, r.tq4_regions, "Qwen3-0.6B Q8_0, two regions, a tq4 mirror: ", false)
        with_dasllama_jobque_() {
            qwen3_codec_within(t, r.codec)
        }
    }
    delete r
    delete forms
}

[test(description = "On the CUDA whole-model driver Qwen3-4B Q8_0, its 2560-wide rows over 36 layers, keeps forced-feed logits within 6 percent of max logit of the CPU chain over one and two windows with the fused q/k-norm rope pass; each form loads once")]
def test_gpu_resident_qwen3_4b(t : T?) {
    return if (!family_on(t, "qwen3") || !cuda_home_asked(t))
    var c : DenseFeeds
    dense_feeds(c, [40l, TWO_WINDOWS])
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3-4B-Instruct-2507-Q8_0.gguf", REGIONS_RIG_CTX, [device_form()]) $(m, form) {
            qwen3_cuda_rows(c, m, form)
        }
    }
    if (ran) {
        qwen3_cuda_within(t, "Qwen3-4B-Instruct-2507-Q8_0.gguf", c)
    }
    delete c
}

let private GATHER_CTX = 4096l   //! the gather form's context: the cells' prompts are one window

[test(description = "On the whole-model driver Qwen3-4B Q4_K_M's tied K-quant table is gathered on the device by the window and every fed step off its token id, with forced-feed steps within 10 percent of max logit of the CPU chain; on the per-op rails tq4 and q8_0 sessions keep layer 0's cache bytes within 5 percent of the CPU chain's and the device no further from the f16 chain than the CPU codec chain; each form loads once")]
def test_gpu_resident_qwen3_4b_kq(t : T?) {
    return if (!family_on(t, "qwen3") || !vulkan_home_asked(t, "the table gather and the per-op cells are the Vulkan home's"))
    let file = "Qwen3-4B-Instruct-2507-Q4_K_M.gguf"
    var g : EmbGather
    var run = PeropRun(cells <- [PeropCell(n = 40l, kv = KVDtype.tq4, bars = QWEN3_4B_PEROP_BARS),
        PeropCell(n = 40l, kv = KVDtype.q8_0, bars = QWEN3_4B_PEROP_BARS)])
    var forms <- perop_forms(run)
    let arm = arm_why("vk-gather")
    if (empty(arm)) {
        forms |> push(device_form(regions = 1l))
    } else {
        g.h |> held_skip("{file}: the tied K-quant table's gather: {arm}")
    }
    let ran = with_cpu_and_gpu_model(t, "llm", "Qwen3-4B-Instruct-2507-Q4_K_M.gguf", GATHER_CTX, forms) $(m, form) {
        if (form.perop) {
            perop_next(t, run, m)
        } elif (form.on_gpu) {
            emb_gather_rows(g, m, form, file)
        } elif (g.ran) {
            g.cpu <- resident_feed_rows(m, form, GATHER_PROMPT, GATHER_STEPS)
        }
    }
    if (ran) {
        if (!g.ran) {
            held_within(t, g.h, "", false)
        }
        with_dasllama_jobque_() {
            emb_gather_within(t, g, file, QWEN_KQ_BAR_REL)
        }
    }
    delete g
    delete forms
    delete run
}
