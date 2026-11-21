options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (qwen2)
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require daslib/fio
require dasllama/dasllama_math   // with_dasllama_jobque_: the compares after the block decode off the queue
require dasllama/dasllama_kv_dtype   // KVDtype: the codec forms' mirror asks
require dasllama/dasllama_gpu_resident   // gpu_kv_want_pin: the ask a codec form loaded under
require _model_tier   // models_dir() + model_available() presence/size-tier gate
require _resident_feed   // Held, held_within, held_pf_window, dense_planes_tagged
require _kv_codec_cells   // the codec cells' halves, the per-op cells
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms, DenseFeeds
require _resident_regions   // the regions cells' collections, REGIONS_RIG_CTX

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // each block's CPU form, and the by-design passes, prefill on the CPU
}

// The qwen2 family on the whole-model driver under DASLLAMA_GPU=1, on the Vulkan home or under DASLLAMA_CUDA=1 on the CUDA
// home (the CUDA pass keeps the file by its `resident_qwen2` token). Each model file is ONE block (`with_cpu_and_gpu_model`):
// its forms load once each and the CPU form once a mirror codec, the cells collect what the compares read and the compares
// follow the block.
// - Qwen2.5-0.5B-Instruct Q8_0 (`test_gpu_resident_qwen2`, the smallest carrier the driver takes): the q/k/v projection bias
//   folds into the rope stage on the device, held by the forced-feed logits-tolerance form - the same fixed tokens through
//   the device and the CPU form, the prefill logits and every step's within the bar, the one-step-off control past it; a
//   dropped or misplaced bias moves the logits far past the bar (the bias is a large share of a 0.5B's k rows). On the
//   Vulkan home one window and two; the q8_0 K/V mirror (the Q8_0 one of the three weight formats a codec's parity owes) in
//   the forced-feed form on its own bar, the distance form with the prompt on the CPU rails and the stored-rows form; a tq4
//   ask served at q8_0 by the serving rule. On both homes the regions rig on the default codec and on a q8_0 mirror: one
//   region's handover, two regions' interleaved and batched steps, continuations, device-home sessions, the parked claim,
//   the guards, the reports, the device-mode, paged and pooled schedulers, the pin and the gap and cap passes, and the
//   route-off load's decline. On the CUDA home the forced feed over one window and two with the driver's counters.
// - The IQ4_NL mint (`-local`, `../performance/model_specs.das`'s recipe, `test_gpu_resident_qwen2_iq4nl32`): its 896-wide
//   rows sit off the superblock lattice, so every dense plane serves off the per-32 iq4nl32 planes - the window's GEMMs on the
//   s tile on the f16 feed and the per-32 batch tile off it, the steps on the per-32 GEMVs; the token table demotes to q8.
//   The forced feed over one window and two on the Vulkan home, one window on the CUDA home.
// - Qwen2.5-1.5B-Instruct IQ3_XS (`test_gpu_perop_iq`): the per-op rails on the grid format, the Vulkan home's.
// Every test skips off the JIT, without DASLLAMA_GPU=1 on a device the driver serves, without the file under models_dir(),
// or where the plan does not fit the card; a codec form is a held skip where DASLLAMA_VK_KV32=1 arms an f32 mirror over its
// codec; the two-window feeds where the prompt window is not PF_WINDOW_ROWS rows. Under DASLLAMA_CUDA=1 the Vulkan home's
// cells - the bias and iq4nl32 feeds' Vulkan form, the q8_0 mirror's feed, distance and stored rows, the tq4 ask, the per-op
// rails - give way to the CUDA feeds.

//! of the step's max |logit|: the 0.5B's CPU-vs-device rounding sits flat at 0.30-0.50 of a 9.5-18 max logit
//! at every step, while a dropped bias lands whole logits off - the hybrid file's K-quant bar; the CUDA home holds it too
let private QWEN2_LOGIT_BAR_REL = 0.06
//! the q8_0 mirror's bar: both chains quantize each query to Q8 for their int8 dot - the device the query it computed, the CPU
//! its own - and two roundings of two nearby queries part by whole quant steps on a share of the elements, on top of the f16
//! reading: 0.42-0.89 of a 9.4-18 max logit, the one-step-off controls 4.7-11
let private Q8KV_BAR_REL = 0.10
//! the codec bars hold ratios of the rows' summed largest differences:
let private CODEC_DIST_REL = 1.25   //! the device's distance from the f16 chain, of the CPU codec chain's own: reads 0.95-1.01, the f16 rows one step off 1.6-3.7
let private CODEC_PAIR_REL = 0.8    //! the two codec chains' distance from each other, of the CPU codec chain's from f16: reads 0.20 (q8_0) to 0.61 (tq4), the CPU codec rows one step off 1.5-3.7
//! layer 0's stored bytes against the CPU cache's: the K rows (the bias carries them) and the steps' V rows (one Q8_0 feed on both
//! chains) differ at 0.002-0.01 of their bytes; the prefill's V rows at 0.39 - its GEMM reads f16 rows where the CPU reads Q8_0
//! blocks - against 0.97 for a row held to the next one (the control)
let private ROWS_BARS = StoredRowsBars(k = 0.02, v_step = 0.05, v_prefill_of_ctrl = 0.6)
//! the regions' tolerance cells under q8_0: the window chain's attention reads the f32 query over the f16 shadow where the token
//! command's reads its q8_0 blocks, so the one-call and the two-call forms part by the query's rounding - reads 0.82 of an
//! 11.4 max logit at the 42-token prompt cut at 40, the prompt one token short the control past it
let private Q8KV_SPLIT_BAR_REL = 0.10
//! the iq4nl32 carrier's bar: its dense planes serve off the per-32 planes, the window's GEMMs on the s tile's f16 feed, and the
//! 0.5B's rows read 0.69 of a 10.9 max logit on the RTX PRO 4500 (0.063), past the file's 0.06; the CUDA home holds it too
let private IQ4NL32_BAR_REL = 0.08


//! the q/k/v bias the qwen2 arm exists for, held off the device form's load
def private bias_held(var c : DenseFeeds; m : Model) {
    c.carrier |> held_fact(m.config.attn_qkv_bias, "the qwen2 carries the q/k/v projection bias the arm exists for")
    c.carrier |> held_equal(m.arch, "qwen2", "the file is a qwen2-family model")
}

//! the q8_0 mirror's cells on the 0.5B: the forced feeds' device rows (the q8_0 form) and the tq4 ask's (the serving rule's
//! form), the distance form's device rows with the prompt on the CPU rails (the q8_0 form with the planes on the host), layer
//! 0's stored rows on both sides, and the CPU form's q8_0 feeds and f16 rows; each `_ran` says its device form collected
struct Qwen2CodecRows {
    feeds : array<int64>
    gpu : array<FeedRows>
    ran : bool
    down : FeedRows
    down_ran : bool
    sides : CodecSides
    cpu : array<FeedRows>
    f16 : FeedRows
    h : Held
}

//! what the 0.5B Q8_0 file's forms collect: the default form's feeds, the q8_0 mirror's cells, the rig's cells on the default
//! codec's and the q8_0 mirror's regions forms
struct Qwen2Rows {
    feeds : DenseFeeds
    codec : Qwen2CodecRows
    regions : Held
    q8_regions : Held
}

//! the 0.5B Q8_0 file's forms: the default (the feeds); one region (the handover), two regions (the rig, the paged and pooled
//! schedulers), the same with the planes on the host (the pin and the gap pass), at the capped context (the cap pass) and
//! the per-op form (the decline reason); under a q8_0 mirror (a codec the environment keeps off held as a skip) on the
//! Vulkan home the plain form (the forced feeds, the stored rows), the planes on the host (the prompt on the CPU rails) and
//! the tq4 ask (the serving rule), on both homes the regions forms again and the per-op form asking the codec
def private qwen2_forms(var r : Qwen2Rows) : array<ModelForm> {
    let cuda = gpu_want_cuda()
    var forms <- [device_form(), device_form(regions = 1l, ctx = REGIONS_RIG_CTX)]
    forms |> push_from(regions_rig_forms())
    let why = codec_env_why(KVDtype.q8_0)
    if (!empty(why)) {
        r.codec.h |> held_skip("the q8_0 mirror's cells and the tq4 ask: {why}")
        return <- forms
    }
    if (!cuda) {
        forms |> push(device_form(kv = KVDtype.q8_0))
        forms |> push(device_form(kv = KVDtype.q8_0, host = true))
        forms |> push(device_form(kv = KVDtype.tq4))
    }
    forms |> push_from(regions_rig_forms(KVDtype.q8_0))
    return <- forms
}

//! one form's share of the q8_0 mirror's cells; the ask a kv-asked form's load took tells the tq4 ask from the q8_0 one,
//! since the serving rule arms both at q8_0
def private qwen2_codec_rows(t : T?; var c : Qwen2CodecRows; m : Model; form : ModelForm) {
    if (form.on_gpu && gpu_kv_want_pin().value == KVDtype.tq4) {
        c.h |> held_equal(form.kv, KVDtype.q8_0, "the tq4 ask armed the q8_0 mirror by the serving rule")
        c.h |> held_equal(kv_codec_for(m.config, KVDtype.tq4), KVDtype.q8_0, "and a session's codec steps down the same way")
        c.down <- resident_feed_rows(m, form, 40l, CODEC_FED_STEPS)
        c.down_ran = true
    } elif (form.on_gpu && form.host) {
        codec_up_rows(t, c.sides, m, KVDtype.q8_0)
    } elif (form.on_gpu) {
        c.gpu |> reserve(length(c.feeds))
        for (n in c.feeds) {
            c.gpu |> emplace <| resident_feed_rows(m, form, n, CODEC_FED_STEPS)
        }
        codec_stored_device_rows(t, c.sides, m, KVDtype.q8_0)
        c.ran = true
    } elif (form.kv == KVDtype.q8_0 && (c.ran || c.sides.up_ran || c.down_ran)) {
        c.cpu |> reserve(length(c.feeds))
        for (n in c.feeds) {
            c.cpu |> emplace <| resident_feed_rows(m, form, n, CODEC_FED_STEPS)
        }
        var f16 = form
        f16.kv = KVDtype.f16
        c.f16 <- resident_feed_rows(m, f16, 40l, CODEC_FED_STEPS)
        if (c.ran) {
            c.sides.stored_host <- codec_stored_host(m, KVDtype.q8_0)
        }
    }
}

//! the q8_0 mirror's compares after the block: the forced feeds on q8_0 sessions, the tq4 ask's feed at the q8_0 bar, the
//! distance form with the prompt on the CPU rails (its prompt's row out of the sums), and layer 0's stored rows
def private qwen2_codec_within(t : T?; c : Qwen2CodecRows) {
    held_within(t, c.h, "Qwen2.5-0.5B q8_0: ", false)
    return if (empty(c.cpu))
    if (c.ran) {
        for (f, i in c.gpu, count()) {
            feed_rows_within(t, "Qwen2.5-0.5B q8_0 mirror, {f.nprompt} tokens: ", f, c.cpu[i], Q8KV_BAR_REL)
        }
        codec_stored_cell(t, "Qwen2.5-0.5B: the q8_0 mirror's stored rows against the CPU cache's, layer 0", KVDtype.q8_0,
            c.sides, ROWS_BARS)
    }
    if (c.down_ran) {
        feed_rows_within(t, "Qwen2.5-0.5B tq4 asked, q8_0 served: ", c.down, c.cpu[0], Q8KV_BAR_REL)
    }
    if (c.sides.up_ran) {
        unsafe {
            t |> run("Qwen2.5-0.5B: the device steps over q8_0 rows the CPU prefill wrote") @capture(& c)(t : T ?) {
                codec_distances_within(t, "q8_0, 40 tokens, the prompt on the CPU rails", c.sides.up, c.cpu[0].rows, c.f16.rows, 1,
                        CODEC_DIST_REL, CODEC_PAIR_REL) $(id) {
                    return feed_piece(c.cpu[0], c.f16, id)
                }
            }
        }
    }
}

//! one form's collection of the 0.5B Q8_0 file's cells into `r`; the regions forms' and the per-op forms' own cells run on
//! their load
def private qwen2_rows(t : T?; var r : Qwen2Rows; var m : Model; form : ModelForm) {
    if (form.perop) {
        regions_route_off_cell(t, m)
    } elif (form.on_gpu && form.regions == 1l) {
        regions_taken_cell(t, m)
    } elif (form.on_gpu && form.regions == 2l && form.kv_asked) {
        regions_rig_form_rows(t, r.q8_regions, m, form, Q8KV_SPLIT_BAR_REL)
    } elif (form.on_gpu && form.regions == 2l) {
        regions_rig_form_rows(t, r.regions, m, form, SPLIT_BAR_REL)
        if (!form.host) {
            regions_scheduler_paged_cell(t, m)   //! the paged and pooled schedulers after the rig's: their streams leave parked claims
            regions_pool_cell(t, m)
        }
    } elif (form.on_gpu && form.kv_asked) {
        qwen2_codec_rows(t, r.codec, m, form)
    } else {
        if (dense_feeds_rows(r.feeds, m, form) == MainSide.device) {
            bias_held(r.feeds, m)
        }
        if (!form.on_gpu) {
            qwen2_codec_rows(t, r.codec, m, form)
        }
    }
}

[test(description = "On Qwen2.5-0.5B Q8_0 with the q/k/v bias folded into the device rope stage the resident chain holds the CPU chain's logits within 6 percent over one and two windows plus eight steps, on the CUDA driver with every position served; a q8_0 K/V mirror holds the CPU chain on q8_0 sessions within 10 percent, its steps over rows a CPU prefill wrote sit within 1.25 times the CPU codec chain's distance from the f16 rows, and its layer 0 stored rows match the CPU cache's within the byte bars; a tq4 ask arms the q8_0 mirror by the serving rule and holds the q8_0 bar; on the default codec and on a q8_0 mirror the regions rig's cells - a region handed over, interleaved, continued, batched, device-home, parked, guarded, reported, scheduled, paged and pooled, the pin and the gap and cap passes - read what each session reads alone, and the route-off load reports never attempted; each form loads once")]
def test_gpu_resident_qwen2(t : T?) {
    return if (!family_on(t, "qwen2"))
    var r : Qwen2Rows
    dense_feeds(r.feeds, [40l, TWO_WINDOWS])
    r.codec.feeds <- window_prompts(r.codec.h)
    var forms <- qwen2_forms(r)
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen2.5-0.5B-Instruct-Q8_0.gguf", REGIONS_RIG_CTX, forms) $(m, form) {
            qwen2_rows(t, r, m, form)
        }
    }
    if (ran) {
        dense_feeds_within(t, "Qwen2.5-0.5B-Instruct-Q8_0.gguf", r.feeds, QWEN2_LOGIT_BAR_REL)
        held_within(t, r.regions, "Qwen2.5-0.5B Q8_0, regions: ", false)
        held_within(t, r.q8_regions, "Qwen2.5-0.5B Q8_0, regions, a q8_0 mirror: ", false)
        with_dasllama_jobque_() {
            qwen2_codec_within(t, r.codec)
        }
    }
    delete r
    delete forms
}

let private FEED_CTX = 4096l   //! a feeds-only block's context: its prompts sit under two windows

[test(description = "The Qwen2.5-0.5B IQ4_NL mint serves every dense plane off the per-32 iq4nl32 planes and holds the CPU chain's logits within 8 percent over one and two windows plus eight steps, on the CUDA driver over one window with every position served; each form loads once")]
def test_gpu_resident_qwen2_iq4nl32(t : T?) {
    return if (!family_on(t, "qwen2"))
    var c : DenseFeeds
    dense_feeds(c, gpu_want_cuda() ? [40l] : [40l, TWO_WINDOWS])
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf", FEED_CTX, [device_form()]) $(m, form) {
            if (dense_feeds_rows(c, m, form) == MainSide.device) {
                bias_held(c, m)
                if (!c.cuda) {
                    dense_planes_tagged(t, m, path_join(models_dir(), "Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf"), KqFmt.iq4nl32)
                }
            }
        }
    }
    if (ran) {
        dense_feeds_within(t, "Qwen2.5-0.5B-Instruct-IQ4_NL-local.gguf", c, IQ4NL32_BAR_REL)
    }
    delete c
}

// The per-op rails under a block-codec session on a grid-format carrier (Qwen2.5-1.5B-Instruct IQ3_XS, `DASLLAMA_GPU=1`, the
// whole-model route pinned off), one of the three weight formats a codec's parity owes (Q8_0, a K-quant, a grid format): a tq4
// ask and a q8_0 session at a 40-token prompt, a per-op load each with no CPU form - each cell's CPU chains run on its own load
// once the device state is dropped. Skips where the tier took no attention layer of the model.

//! the device's summed distance from the f16 chain, as a ratio of the CPU codec chain's own: reads 0.79 (8.6 against 11.0), the
//! f16 rows one step off 6.2 times it. The serving rule on the biased carrier: a tq4 ask serves at q8_0 on both chains (the
//! carrier's K bias outliers would set every 4-bit block's scale), so the session holds the q8_0 bars, and the cache's rows are
//! the CPU's q8_0 rows
let private QWEN25_IQ_Q8_BARS = PeropBars(rows = 0.05, dist_rel = 1.0)

[test(description = "On the per-op rails with Qwen2.5-1.5B IQ3_XS, a tq4 ask served at q8_0 by the serving rule and a q8_0 session each keep layer 0's cache bytes within 5 percent of the CPU chain's and the device no further from the f16 chain than the CPU codec chain; the form loads once")]
def test_gpu_perop_iq(t : T?) {
    return if (!family_on(t, "qwen2") || !vulkan_home_asked(t, "the per-op rails' cells are the Vulkan home's"))
    var run = PeropRun(cells <- [PeropCell(n = 40l, kv = KVDtype.tq4, bars = QWEN25_IQ_Q8_BARS),
        PeropCell(n = 40l, kv = KVDtype.q8_0, bars = QWEN25_IQ_Q8_BARS)])
    let forms <- perop_forms(run)
    with_cpu_and_gpu_model(t, "llm", "Qwen2.5-1.5B-Instruct-IQ3_XS.gguf", FEED_CTX, forms, false) $(m, _form) {
        perop_next(t, run, m)
    }
    delete run
}
