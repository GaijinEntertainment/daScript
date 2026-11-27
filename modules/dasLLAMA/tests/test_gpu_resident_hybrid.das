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
    allow_cpu_prefill()   // each block's CPU form, and the by-design passes, prefill on the CPU
}

//! every test pins the window before its first load: the plan and the window chain read it, and
//! an [init] pin would reach every batch-mate's context too
//! the pin it replaced answered
def private pin_window_rows() : int64 {
    let was = pf_window_rows_pin()
    set_pf_window_rows_pin(HYBRID_WINDOW)
    return was
}
require daslib/jobque_boost
require daslib/defer   // defer: each test puts back the window pin it found
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_sampling
require dasllama/dasllama_gpu_tier   // reset_gpu_cpu_passes: the passes the by-design cells leave
require dasllama/dasllama_env   // set_pf_window_rows_pin: the file's 512-row window; PF_ROWS_WIDE
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the hybrid
require dasllama/dasllama_kv_dtype   // KVDtype: the codec forms' mirror asks
require ?vulkan dasllama/dasllama_vulkan_common   // the resolved coopmat mode and the kernel census, DN_WINDOW, where the module exists
require ?vulkan dasllama/dasllama_vulkan_prefill   // pf_f16_feed: the feed admission the driver acts on
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_rdec_head_embed_gathered: the head's rows gathered off their ids
require ?cuda dasllama/dasllama_cuda_decode   // CUDA_MMQ_WINNERS / CUDA_MMQ_OFF: the NVFP4 cells' named routes
require ?cuda _cuda_feed   // fp4_rung_why, cuda_mmq_name: the CUDA home's FP4 cells
require _model_tier   // models_dir() + model_available() presence/size-tier gate
require _resident_feed
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms, FeedRows
require _parity_cells   // the parity cells the block's parity knob reaches (`_::`): the files' model_specs parity rows on their blocks
require _resident_regions   // the regions cells, REGIONS_RIG_CTX, short_control, continue_at
require _kv_codec_cells   // codec_env_why, codec_distances_within: the gated carrier under a block-codec mirror
require _mtp_resident_cells   // the NextN head's cells, one body for every discrete home
require _segment_cells   // recurrent_segments_cell, SEG_WINDOW: the segmented window's cells
require _mtp_cpu_cells   // mtp_cpu_cells, mtp_q4_0_cell: the -MTP- file's CPU forms' own cells
require _chat_cells   // chat_qwen35_cells: the Q8_0 file's uncached CPU form's cells
require _deltanet_cells   // deltanet_chunked_prefill_cell: the Q8_0 file's CPU form's chunked-prefill cell
require _scheduler_cells   // the -MTP- file's speculative scheduler cells, on its CPU form and its scheduler form
require _batch_decode_cells   // batch_decode_hybrid_cells: the Q8_0 file's portable-pinned CPU form
require _kquant_cells   // kq_nv4_load_layout_cell: the NVFP4 file's K-quant-native portable form
require _gpu_tier_armed   // gpu_tier_why: the -MTP- file's plain scheduler cell runs on its device form where the tier arms
require _metal_batched_cells   // metal_qwen35_cells, metal_qwen35_mtp_cells: the Q8_0 and -MTP- files' Metal forms' cells
require _metal_matrix_cells   // the support matrix's hybrid row on the Q8_0 file's Metal form
require _metal_decode_cells   // the mixed sites' batched decode on the q8out file's Metal form
require _metal_mtp_cells   // the Metal MTP rail's arms on the -MTP- files' served images
require _kernel_coverage_cells   // the coverage zoo's and the Vulkan census's qwen35 rows
require _compares   // argmax_of: the parity dump's rows decoded
require ../harness/_parity_dump.das   // the parity dump round trip on the -MTP- file's forms

// The whole-model resident driver on the deltanet hybrid Qwen3.5-0.8B under DASLLAMA_GPU=1, on the Vulkan home or under
// DASLLAMA_CUDA=1 on the CUDA home: the resident window chain prefills the prompt (the recurrent layers through the conv +
// chunked scan over their device state, the gated partial-rope attention over the mirror), then the resident decode steps
// every layer - the recurrent ones through the fused deltanet step - as one submit per token. Each model file is ONE block
// (`with_cpu_and_gpu_model`): its forms load once each and the CPU form once a mirror codec, the cells collect what the
// compares read and the compares follow the block. The GPU chains and the CPU chain are not bit-identical (GPU exp / f32
// accumulation order), so a compare against the CPU chain is the forced-feed logits-tolerance form: the same fixed tokens
// fed to both, the logits within a bar of the CPU chain after the prefill and at every step, and the one-step-off control
// that must exceed it.
// - Q8_0 (`test_gpu_resident_hybrid_q8`): on the Vulkan home the forced feed over four prompt lengths - one window, two
//   windows (the conv tail and the state carry across the window seam), one past the window, and two tokens (both conv-tail
//   edges: the last window shorter than the taps); on the CUDA home its own feeds over one window and two with the driver's
//   counters (every position served, the window chains, the captured graph, the unsplit form) and their control; on both a
//   q8_0 mirror's feed and a tq4 mirror's distance form under the gated attention; at one region the greedy chain, the short
//   window's peel and the region a host-cached session loses; at two regions on the 2048-row window the rig's cells - two
//   sessions' recurrent state slots stepped interleaved and batched, device-home sessions, continuations from the region's
//   live state, a recurrent parked claim, the device-mode, paged and pooled schedulers - and with the planes on the host the
//   device-home pin's outsider; at two regions on the SEG_WINDOW-row window the segmented window's cells; with the planes on
//   the host two deltanet streams through the scheduler, the device dropped for the control last (`_scheduler_cells.das`); on
//   its portable-pinned CPU form, on every box, batched decode against sequential (`_batch_decode_cells.das`).
// - On each of the Q8_0, Q4_K_M, mixed-twin and 4B files' parity row CPU form (the row's load mode and K-quant knob under
//   DASLLAMA_PIN_BACKEND's pin), on every box, the row's frozen greedy ids and its chunked-prefill arm (`parity_spec_cell`);
//   the Q4_K_M row's deltanet planes K-quant native.
// - Q4_K_M and the mixed twin (`..._kq`, `..._kq_q8out`): the K-quant deltanet planes kept in their file formats, the feed
//   over one and two windows at the K-quant bar with the routing witness (on the f16 feed the qkv/z pair on the k4 cm2 tiles)
//   and the f32-chain witness on the Vulkan home, the CUDA home's feeds at its K-quant bar; on the Q4_K_M twin a prompt's
//   second half continued in place and from an adopted checkpoint against the whole prompt, and a 5000-token prompt at the
//   wide window and at a 5120-row window against the CPU chain.
// - The NextN-headed twin (`..._mtp`): the carry the driver lands, the draft through the resident head, the same-slab verify,
//   the rejected verify's rollback, the head's prompt warm, six rounds against the CPU round and the gates around the round
//   (`_mtp_resident_cells.das`), the depth-8 verify and its depth-1 control, the sampled selection at depth 2, the head kept
//   off the device, the head over q8_0 and tq4 mirrors, the peel with speculation off, the pool's openings under a
//   speculating scheduler, the segmented window's cells first on the two-region form, and on the CUDA home the trunk's
//   forced feed; on its q8 and q4_0 CPU forms, on every box, the file's CPU cells (`_mtp_cpu_cells.das`) and the speculative
//   scheduler's; the self-speculative streams against the plain scheduler on the driver's own regions where the tier arms, on
//   the q8 CPU form where it does not.
// - The NVFP4 twin (`..._nv4`): on the CUDA home alone its deltanet sites on the FP4 leaf and pinned off MMQ on the cm2 tiles;
//   on its K-quant-native portable-pinned CPU form, on every box, the planes' load layout (`_kquant_cells.das`).
// - On the CUDA home alone the 4B shape's forced feed (`..._4b`, the `qwen35-4b` category).
// - On the Q8_0 and -MTP- files' Metal forms (`metal_form`, the blob twin) the Metal batched driver's cells
//   (`_metal_batched_cells.das`): the batched row, the paged, segmented, evicted and released deltanet sessions, the greedy
//   chain and the pre-encode rail on Q8_0; the NextN carry, the speculative rounds, the pick and the prefix checkpoints on
//   -MTP-. The form is one skipped cell on a build without das_metal; a cell skips under a family filter without `qwen35`.
// Every model test skips interpreted or without its file; without the armed tier each device form skips loudly as a cell of
// its own while the CPU-cells forms run; a form whose plan does not fit the card skips loudly; the NVFP4 twin's device forms
// skip as one loud cell without DASLLAMA_CUDA=1 or on a build with no CUDA module, and so do the 4B file's forced feed and the
// -MTP- file's head-warm fault form without DASLLAMA_CUDA=1; a
// parity row's form where the box cannot take DASLLAMA_PIN_BACKEND's pin. The K-quant cells skip on the f32 mirrors (`DASLLAMA_VK_KV32=1`), and so do the block-codec forms and the
// codec pass arm; the window cells skip under DASLLAMA_VK_KV32=1 (the f32 mirror passes every f16 session as codec); the
// two-window K-quant feed and the two-window head warm skip where the prompt window is not the file's 512 rows. Under
// DASLLAMA_CUDA=1 the Vulkan home's own cells (the four-length and K-quant feeds' census witnesses, the f32-chain witness,
// the window cells) give way to the CUDA home's, and the NVFP4 leaf cells skip on a card below the FP4 rung (`FP4_RUNG_SM`).
// Under DASLLAMA_DEVICE_POOL=0 the pool cells hold the off rail or skip. The segmented window's capture cell skips under
// DASLLAMA_GPU_DN_SNAPS=1: a region then keeps one checkpoint row, and the cell plants two. The regions rig's no-cache scheduler cell skips its
// cache-on control on every file here: a recurrent model's claim keeps its state at its last row only, so an opening that ends
// at the prompt boundary adopts nothing.

//! the K-quant bar on the f16 feed and mirror: CPU-vs-device rounding sits flat at 0.39-0.57 of a ~12.5 max logit on the RTX PRO
//! 4500 (the Q8 file 0.19-0.36) and 0.45-0.79 of a 10.9-15.6 max logit on the RTX 5060 Ti, with no growth and the one-step-off
//! control well past it; the device's distance from the file's f32 chain is held beside it (`f32_chain_witness`)
let private DN_LOGIT_BAR_REL_KQ_F16_FEED = 0.08
//! ... on a device whose compiler rounds a shader at a time (`home_rounds_alike`): on the Radeon 8060S the same rows read
//! under the bar above but for one step of the one-window cell - a row whose top two sit close on the f32 chain, the pick
//! flipped; the device's summed distance from the f32 chain reads level with the CPU chain's, the one-step-off control
//! well past both
let private DN_LOGIT_BAR_REL_KQ_F16_FEED_APART = 0.12
//! the K-quant bar on the quant feed (no coopmat tile serves the planes, the prefill's activations ride Q8_K): the mixed twin's two-window cell under `DASLLAMA_COOPMAT=sdot4` on the RTX 5060 Ti reads 0.084 of an 11.6 max logit at one step, argmax unchanged, the rest under 0.05
let private DN_LOGIT_BAR_REL_KQ_QUANT_FEED = 0.10
//! the mixed twin's bar on the f16 feed: its CPU chain quantizes the activations twice a recurrent layer (Q8_K for qkv/z, Q8_0
//! for out), so the device and the CPU chain round on either side of the file's f32 chain - one step reads 0.91 of a 12.3 max
//! logit apart (the device 0.63 and the CPU chain 0.50 from the f32 row, the device's pick the f32 chain's), the rest 0.42-0.60
let private DN_LOGIT_BAR_REL_KQ_Q8OUT = 0.08
//! the K-quant twins' bar on the CUDA home off the f16 feed: the window's K-quant sites read the int8 MMQ leaves' Q8_0 feed
let private DN_LOGIT_BAR_REL_KQ_CUDA = 0.10
//! the FP4 leaf's W4A4 bar on the hybrid: the file's rows read 0.139 (one window) and 0.134 (two) of the step's max logit on the
//! leaf and 0.040 / 0.030 on the cm2 tiles (the control arm, at the K-quant bar), the tightest one-step-off control at 0.45 (RTX
//! 5060 Ti, `-jit`, the MMQ pin on) - the extra is the e2m1 feed's rounding, which the recurrence carries into every step
let private DN_LOGIT_BAR_REL_FP4 = 0.20

//! the gated carrier under a block codec: the q8_0 mirror against the CPU chain on q8_0 sessions reads 0.21-0.31 of a
//! 10.7-12.3 max logit (the f16 mirror's own reading); under tq4 the device's summed distance from the f16 chain reads 1.08
//! of the CPU tq4 chain's own (9.2 against 8.5, the f16 rows one step off 9.2 times it) and the two tq4 chains 0.63 of it
//! apart (the CPU tq4 rows one step off 9.3 times it)
let private GATED_Q8KV_BAR_REL = 0.06
let private GATED_CODEC_DIST_REL = 1.15
let private GATED_CODEC_PAIR_REL = 0.8

let private HYBRID_FED_STEPS = 8l   //! every hybrid feed's fed steps: the forced-feed form's, the CUDA home's and the codec cells'
let private HYBRID_BATCH_CTX = 64l   //! the batched-decode cells' sessions' context

//! the plane formats a fixture's deltanet layers carry: all Q8_0, all K-quant, or K-quant qkv/z beside a Q8_0 out plane
enum HybridPlanes {
    q8
    kq
    kq_q8out
}

//! the f16-fed window tile dispatches of format `f` so far: the cm2 l, m and s columns, and the KHR tile a device in mm mode
//! serves in their place (`census` counts it under the stamp its slot serves); NO_CENSUS with no vulkan module
def private kq_cm2_tiles_census(f : KqFmt) : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let stem = kq_tile_stem("{f}")
        return census("{stem}_cm2l_cls_spv") + census("{stem}_cm2m_cls_spv") + census("{stem}_cm2s_cls_spv") + census("{stem}_khr_cls_spv")
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

//! the Q8_K requants and k4 cm2 tiles one Vulkan feed's prefill alone dispatched, and the counts before it
struct HybridCensus {
    rq0 : int64
    tiles0 : int64
    rq : int64 = -1l
    tiles : int64 = -1l
}

//! what a hybrid file's feeds collect: the planes the file carries, the K-quant planes and the f16 feed's admission as the device
//! load read them (`carrier`), the feeds on the file's window - the CUDA home's counters and their control held in theirs - and
//! each Vulkan feed's prefill census
struct HybridRows {
    planes : HybridPlanes
    carrier : Held
    f16_feed : bool
    nrec : int64
    nrec_ba_kq : int64
    feeds : DenseFeeds
    census : array<HybridCensus>
}

//! the feeds a hybrid file runs on the armed home over the file's window: `vulkan` there, `cuda` under DASLLAMA_CUDA=1; a
//! two-window length past the window the K-quant feeds gate on is held as a skip where the prompt window is not the file's
//! why a K-quant carrier's f32 form, the device's same-or-better
//! witness, is left out: the CUDA home is asked; "" where it is not
def private f32_witness_why() : string
    => vulkan_home_why("the f32 chain's witness is the Vulkan home's cell")

def private hybrid_feeds(var r : HybridRows; planes : HybridPlanes; vulkan, cuda : array<int64>) {
    r.planes = planes
    r.feeds.cuda = gpu_want_cuda()
    r.feeds.window = HYBRID_WINDOW
    r.feeds.steps = HYBRID_FED_STEPS
    for (n in r.feeds.cuda ? cuda : vulkan) {
        if (n > HYBRID_WINDOW && planes != HybridPlanes.q8 && !held_pf_window(r.carrier, HYBRID_WINDOW)) {
            continue
        }
        r.feeds.prompts |> push(n)
    }
}

//! the K-quant carrier's planes as the device load kept them: the deltanet qkv/z planes in their K-quant file formats (the out
//! plane Q8_0 on the mixed twin, K-quant too on the other), and the recurrent layers whose beta or alpha plane is K-quant
def private hybrid_kq_planes(var r : HybridRows; m : Model) {
    let q8out = r.planes == HybridPlanes.kq_q8out
    var native = 0l
    for (l in range64(m.config.n_layers)) {
        continue if (!layer_is_recurrent(m.config, l))
        r.nrec++
        if (kq_sb(int(fmt_at(m.dnbeta_fmt, l))) || kq_sb(int(fmt_at(m.dnalpha_fmt, l)))) {
            r.nrec_ba_kq++   // the window requantizes the row to Q8_K for these on the f16 feed too
        }
        if (fmt_at(m.dnqkv_fmt, l) != KqFmt.q8 && fmt_at(m.dngate_fmt, l) != KqFmt.q8
                && ((fmt_at(m.dnout_fmt, l) == KqFmt.q8) == q8out)) {
            native++
        }
    }
    let out_form = q8out ? "the out plane Q8_0" : "the out plane too"
    r.carrier |> held_fact(native > 0l, "the loader kept the deltanet qkv/z planes in their K-quant file formats, {out_form} ({native} recurrent layers)")
    r.f16_feed = f16_feed_admits_kq()
}

//! one form's share of a hybrid file's feeds (`dense_feeds_rows`), the Vulkan feeds' prefill census read; ahead of the default
//! device form's feeds the CUDA home's family witness or the K-quant planes; the side the form is
def private hybrid_rows(var r : HybridRows; m : Model; form : ModelForm) : MainSide {
    if (device_default(form) && !r.feeds.cuda && r.planes != HybridPlanes.q8) {
        hybrid_kq_planes(r, m)
    }
    return dense_feeds_rows(r.feeds, m, form, r.feeds.cuda ? "qwen35" : "") $(i, after) {
        if (!after) {
            let rq0 = census("cls_q8k_rq_spv")
            let tiles0 = kq_cm2_tiles_census(KqFmt.k4)
            r.census |> push(HybridCensus(rq0 = rq0, tiles0 = tiles0))
        } else {
            r.census[i].rq = census_since("cls_q8k_rq_spv", r.census[i].rq0)
            r.census[i].tiles = census_delta(r.census[i].tiles0, kq_cm2_tiles_census(KqFmt.k4))
        }
    }
}

//! the K-quant routing witness on one Vulkan feed of an `n`-token prompt: off the f16 feed the x route requants once per
//! recurrent layer per window; on it only the K-quant beta / alpha planes' GEMMs read a Q8_K row (one a recurrent layer a
//! window), the qkv / z pair the f16 rows on the K-quant cm2 tiles
def private hybrid_census_within(t : T?; r : HybridRows; n : int64; f : HybridCensus) {
    return if (f.rq == NO_CENSUS)
    let windows = prompt_windows(n, HYBRID_WINDOW)
    to_log(LOG_INFO, "prefill ({n} tokens, {windows} windows) [{tile_arm_tag()}]: {f.rq} Q8_K requants over {r.nrec} recurrent layers ({r.nrec_ba_kq} with K-quant beta / alpha planes), {f.tiles} k4 cm2 tiles, f16 feed admits kq {r.f16_feed}\n")
    if (r.f16_feed) {
        t |> success(f.tiles > 0l, "on the f16 feed the K-quant qkv/z pair's GEMMs ride the k4 window tiles - the cm2 columns, or the KHR tile in mm mode ({f.tiles} dispatches) [{tile_arm_tag()}]")
        t |> success(f.rq <= windows * r.nrec_ba_kq + 1l, "on the f16 feed the prefill requantized the row to Q8_K for the K-quant beta / alpha planes alone, once a recurrent layer a window, and never for the qkv/z pair ({f.rq} Q8_K requants, {r.nrec_ba_kq} such layers, {windows} windows, the classifier's one) [{tile_arm_tag()}]")
    } else {
        t |> equal(f.tiles, 0l, "off the f16 feed no k4 window tile serves the pair [{tile_arm_tag()}]")
        t |> success(f.rq >= windows * r.nrec, "off the f16 feed the K-quant qkv/z pair reads the Q8_K form ({f.rq} Q8_K requants, {r.nrec} recurrent layers, {windows} windows) [{tile_arm_tag()}]")
    }
}

//! a hybrid feed's bar on the armed home: the deltanet bar on the Q8_0 planes; on the K-quant planes the CUDA home's MMQ feed
//! bar, else the f16 feed's (the mixed twin's own) or the quant feed's by the driver's admission
def private hybrid_bar(r : HybridRows) : float {
    return _mtp_resident_cells::DN_LOGIT_BAR_REL if (r.planes == HybridPlanes.q8)
    return DN_LOGIT_BAR_REL_KQ_CUDA if (r.feeds.cuda)
    return DN_LOGIT_BAR_REL_KQ_QUANT_FEED if (!r.f16_feed)
    return r.planes == HybridPlanes.kq_q8out ? DN_LOGIT_BAR_REL_KQ_Q8OUT : (home_rounds_alike() ? DN_LOGIT_BAR_REL_KQ_F16_FEED : DN_LOGIT_BAR_REL_KQ_F16_FEED_APART)
}

//! a hybrid file's feeds held after its block: each one's device rows against the CPU form's at the home's bar, on the Vulkan
//! home the K-quant routing witness and, on a K-quant carrier, the device no further from the file's f32 chain than the CPU
//! chain; the K-quant planes' and the CUDA counters' reads
def private hybrid_rows_within(t : T?; tag : string; r : HybridRows) {
    return if (!r.feeds.main.ran)
    let kq = r.planes != HybridPlanes.q8
    held_within(t, r.carrier, "{tag}: ", false)
    held_within(t, r.feeds.carrier, "{tag} cuda: ", false)
    with_dasllama_jobque_() {
        for (f, i in r.feeds.gpu, count()) {
            let n = r.feeds.prompts[i]
            let windows = prompt_windows(n, HYBRID_WINDOW)
            let home = r.feeds.cuda ? "the CUDA driver" : "the resident driver"
            unsafe {
                t |> run("{tag}: a {n}-token prompt ({windows} window{windows > 1l ? "s" : ""}) and {HYBRID_FED_STEPS} fed steps on {home} against the CPU chain ({cuda_knob()})") @capture(& r, & f)(t : T ?) {
                    if (kq && !r.feeds.cuda) {
                        hybrid_census_within(t, r, n, r.census[i])
                    }
                    return if (!twin_collected(t, i, length(r.feeds.cpu), ""))
                    feed_rows_within(t, "", f, r.feeds.cpu[i], hybrid_bar(r))
                    if (kq && !r.feeds.cuda) {   // the K-quant carriers' same-or-better witness: the device no further from the file's f32 chain than the CPU q8 chain
                        f32_chain_witness(t, "{n} tokens against the f32 chain [{tile_arm_tag()}]", f.rows, r.feeds.cpu[i].rows,
                                r.feeds.f32[i].rows, F32_DIST_REL) $(id) {
                            return feed_piece(f, r.feeds.cpu[i], id)
                        }
                    }
                }
            }
        }
    }
}

//! the gated attention's block-codec cells: the q8_0 mirror's forced feed against the CPU chain on q8_0 sessions, and the tq4
//! mirror's distance form - the device's rows, the CPU tq4 chain's and the CPU f16 chain's; each `_ran` says its device form
//! collected
struct HybridCodecRows {
    q8_gpu : FeedRows
    q8_cpu : FeedRows
    q8_ran : bool
    tq4_gpu : FeedRows
    tq4_cpu : FeedRows
    tq4_f16 : FeedRows
    tq4_ran : bool
}

def private hybrid_codec_rows(var c : HybridCodecRows; m : Model; form : ModelForm) {
    if (form.on_gpu && form.kv == KVDtype.q8_0) {
        c.q8_gpu <- resident_feed_rows(m, form, 40l, CODEC_FED_STEPS)
        c.q8_ran = true
    } elif (form.on_gpu) {
        c.tq4_gpu <- resident_feed_rows(m, form, 40l, CODEC_FED_STEPS)
        c.tq4_ran = true
    } elif (form.kv == KVDtype.q8_0) {
        c.q8_cpu <- resident_feed_rows(m, form, 40l, CODEC_FED_STEPS)
    } elif (form.kv == KVDtype.tq4) {
        c.tq4_cpu <- resident_feed_rows(m, form, 40l, CODEC_FED_STEPS)
        var f16 = form
        f16.kv = KVDtype.f16
        c.tq4_f16 <- resident_feed_rows(m, f16, 40l, CODEC_FED_STEPS)
    }
}

def private hybrid_codec_within(t : T?; c : HybridCodecRows) {
    if (c.q8_ran) {
        unsafe {
            t |> run("Qwen3.5-0.8B: a q8_0 K/V mirror under the gated attention - resident prefill + decode vs the CPU chain on q8_0 sessions ({cuda_knob()})") @capture(& c)(t : T ?) {
                feed_rows_within(t, "q8_0 mirror: ", c.q8_gpu, c.q8_cpu, GATED_Q8KV_BAR_REL)
            }
        }
    }
    if (c.tq4_ran) {
        unsafe {
            t |> run("Qwen3.5-0.8B: a tq4 K/V mirror under the gated attention - the un-rotation applies the head gate - against the CPU chain's tq4 and f16 rows ({cuda_knob()})") @capture(& c)(t : T ?) {
                t |> equal(c.tq4_gpu.prefills, 1l, "the resident window chain served the prompt")
                codec_distances_within(t, "tq4, 40 tokens", c.tq4_gpu.rows, c.tq4_cpu.rows, c.tq4_f16.rows, 0, GATED_CODEC_DIST_REL,
                        GATED_CODEC_PAIR_REL) $(id) {
                    return feed_piece(c.tq4_gpu, c.tq4_cpu, id)
                }
            }
        }
    }
}

//! what the Q8_0 file's forms collect: the feeds, the block-codec cells, the greedy chain on the one-region form, the rig's
//! cells on the two-region form, and the device-home pin's outsider on the two-region form with the planes on the host
struct HybridQ8Rows {
    feed : HybridRows
    codec : HybridCodecRows
    chain : Held
    regions : Held
    home_pin : Held
    matrix : MatrixHybridKept   //! the support matrix's row, its twin's side
}

//! the segmented window's cells (`recurrent_segments_cell`) on a two-region load on the SEG_WINDOW-row window as their own cell
def private recurrent_segments_on(t : T?; var m : Model; title : string) {
    unsafe {
        t |> run(title) @capture(& m)(t : T ?) {
            recurrent_segments_cell(t, m)
        }
    }
}

//! one form's collection of the Q8_0 file's cells into `r`; the one- and two-region forms' own cells run on their load
def private hybrid_q8_rows(t : T?; var r : HybridQ8Rows; var m : Model; form : ModelForm) {
    if (form.on_metal) {
        matrix_hybrid_metal(t, m, form, matrix_hybrid_qwen35_0_8b(), r.matrix)
        coverage_zoo_metal(t, m, form, cov_zoo_c9_qwen35())
        metal_qwen35_cells(t, m)
    } elif (form.drops_device) {
        device_drop_cell(t, form) {
            scheduler_batching_deltanet_gpu_cell(t, m)
        }
    } elif (vk_census_form_is(form)) {
        coverage_vk_model(t, m, form, cov_vk_c60_qwen35())
    } elif (!form.on_gpu && form.backend == "portable") {
        with_ctx_cut(m, HYBRID_BATCH_CTX) {
            batch_decode_hybrid_cells(t, m)
        }
        matrix_hybrid_cpu(t, m, form, matrix_hybrid_qwen35_0_8b(), r.matrix)
    } elif (form.pf_window_rows == SEG_WINDOW) {
        recurrent_segments_on(t, m, "On the Qwen3.5-0.8B hybrid segmented windows - a tail under the conv taps, tails past one window - hold the split bar of solo windows, and checkpoint stops captured inside one call adopt and continue at their rows")
    } elif (form.on_gpu && form.regions == 1l) {
        greedy_chain_rows(r.chain, m, "Qwen3.5-0.8B-Q8_0.gguf")
        unsafe {
            t |> run("Qwen3.5-0.8B: a short tail peels through the token commands, the recurrent rows in order in the region's slot, and reads what the one-row steps read") @capture(& m)(t : T ?) {
                recurrent_peel_cell(t, m, SPLIT_BAR_REL)
            }
        }
        regions_taken_cell(t, m)
    } elif (form.on_gpu && form.regions == 2l && form.host) {
        regions_device_home_pin_rows(r.home_pin, m)
        reset_gpu_cpu_passes()   //! the pin cell's outsider passes to the CPU by design: the form's own witness reads the calls after it
    } elif (form.on_gpu && form.regions == 2l) {
        regions_interleaved_rows(r.regions, m)
        regions_batched_rows(r.regions, m)
        regions_batched_first_rows(r.regions, m)
        regions_device_home_rows(r.regions, m)
        regions_continues_cell(t, m, SPLIT_BAR_REL)
        regions_park_adopt_recurrent_cell(t, m)
        regions_scheduler_no_cache_rows(r.regions, m)   //! the scheduler's cells last: their streams leave parked claims behind;
        regions_scheduler_rows(r.regions, m)   //! the no-cache request first, its control adopting off a region no turn holds
        regions_scheduler_chunks_cell(t, m)
        regions_scheduler_handoff_rows(r.regions, m)
        regions_scheduler_paged_cell(t, m)
        regions_checkpoint_adopt_cell(t, m)
        regions_pool_cell(t, m)
        regions_context_file_cell(t, m)
    } elif (form.on_gpu && form.kv_asked) {
        hybrid_codec_rows(r.codec, m, form)
    } elif (!form.on_gpu && form.uncached) {
        chat_qwen35_cells(t, m)
    } else {
        hybrid_rows(r.feed, m, form)
        if (!form.on_gpu) {
            hybrid_codec_rows(r.codec, m, form)
        }
        if (!form.on_gpu && form.alone) {
            deltanet_chunked_prefill_cell(t, m)
        }
    }
}

//! the Q8_0 file's forms: the default (the feeds), the q8_0 and tq4 mirrors (a codec the environment keeps off held as a skip),
//! one region (the greedy chain, the peel, the region taken), two regions on the rig's 2048-row window (its cells read prompts
//! sized against it), the same with the planes on the host (the pin cell), two regions on the SEG_WINDOW-row window (the
//! segmented window's cells), the planes on the host with the device dropped last (the two deltanet streams' scheduler cell,
//! its control off the tier), and the q8, uncached and portable-pinned CPU forms whose cells stand alone (the chunked prefill,
//! the chat cells, batched decode against sequential)
def private hybrid_q8_forms(var r : HybridQ8Rows) : array<ModelForm> {
    var forms <- [device_form(), device_form(regions = 1l, ctx = REGIONS_RIG_CTX),
        device_form(regions = 2l, ctx = REGIONS_RIG_CTX, pf_window_rows = PF_WINDOW_ROWS),
        device_form(regions = 2l, host = true, ctx = REGIONS_RIG_CTX, pf_window_rows = PF_WINDOW_ROWS),
        device_form(regions = 2l, ctx = REGIONS_RIG_CTX, pf_window_rows = SEG_WINDOW),
        device_form(host = true, drops_device = true)]
    codec_forms(forms, r.feed.carrier, [KVDtype.q8_0, KVDtype.tq4], device_form()) $(kv) => "the {kv} mirror under the gated attention"
    if (vk_census_asked()) {
        forms |> push(vk_census_form())
    }
    forms |> push(cpu_cells_form())
    forms |> push(cpu_cells_form(uncached = true))
    forms |> push(cpu_cells_form(backend = "portable"))
    forms |> push(metal_form(ctx = KERNEL_COVERAGE_CTX))   //! the zoo's: the other cells cut down
    return <- forms
}

[test(description = "On Qwen3.5-0.8B Q8_0 the resident driver holds the CPU chain within the deltanet bar on forced feeds over one window, two, one past the window and a two-token prompt on the Vulkan home and over one window and two with every position served on the CUDA home; under the gated attention a q8_0 K/V mirror holds the CPU q8_0 chain within 6 percent and a tq4 mirror sits no further than 1.15 times the CPU tq4 chain's distance from the f16 rows; at one region the greedy chain's steps reproduce the plain steps bit for bit, a short tail peels within the split bar of one-row steps and a host-cached session losing its region keeps its device-only rows; at two regions two sessions' recurrent state slots step interleaved, batched and batched first as each reads alone, device-home sessions step as a host-cached session alone, a prefill continued past zero starts from the region's live state within the split bar, a recurrent parked claim hands its rows and state over, and the device-mode, paged and pooled schedulers emit the host-cached schedulers' tokens; on the 512-row window segmented windows - a tail under the conv taps, tails past one window - hold the split bar of solo windows, and checkpoint stops captured inside one call adopt and continue at their rows; on the q8 CPU form, on every box, a 150-token chunked deltanet prefill holds 4 percent of the recurrent one over 8 forced-feed steps, on the uncached CPU form the xml_function template's turns render and the Hermes tool markers are single vocab ids, and on the portable-pinned CPU form, on every box, every batched step runs on the CPU stack and matches sequential logits, KV, deltanet state and conv history within 1e-6 under f32 and f16 KV and decoding below the deltanet position panics; with the planes on the host two deltanet streams decoded turn about match their solo generate references token for token, share decode steps, record fused deltanet steps on the device and pass nothing to the CPU rails; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token with its chunked-prefill arm; on the Metal home's blob twin, one load on a das_metal build: on the deltanet hybrid Qwen3.5-0.8B Q8, the batched row serves four and nine streams through the device driver, with exactly one recurrent scan dispatch per recurrent layer per device call; on Qwen3.5-0.8B Q8, one and two greedy streams of 40 reply tokens on 64-row pages emit the flat sessions' tokens exactly, both sides decoding on Metal, with a one-token reply as the step floor's control; on Qwen3.5-0.8B Q8, two sessions' prompt tails prefilled as one segmented window leave each its own recurrent state: 24 device greedy steps per session equal its single-window run, and the streams differ; on Qwen3.5-0.8B Q8, a deltanet session whose recurrent mirror is evicted mid-stream rebuilds it once from its stash and steps on the device, its 12 greedy tokens those of an unevicted session; on Qwen3.5-0.8B Q8, a released deltanet session's mirror and stash leave the pool, and more than twice the mirror cap's live sessions stepping in turn decode on the device, each emitting its own solo tokens; on Qwen3.5-0.8B Q8, the single-row greedy chain over 24 device steps leaves tokens and every step's logits bit-identical to the plain run, greedy-fed and fed off the pick every third step; on the deltanet hybrid Qwen3.5-0.8B Q8, four greedy streams emit the same tokens with batched steps pre-encoded under the step before as encoded at call, and retiring a parked step leaks no Metal object; on the same twin the support matrix's hybrid row - single-row steps a codec within the batch bar, the paths' engage, the metal-overrides generate on each codec token for token the portable CPU form's, the continuation within 4.0 - and the coverage zoo's qwen35 row; on the Vulkan home's default form the coverage census's DeltaNet row; each form loads once")]
def test_gpu_resident_hybrid_q8(t : T?) {
    let window_was = pin_window_rows()
    defer() {
        set_pf_window_rows_pin(window_was)
    }
    return if (!family_on(t, "qwen35"))
    var r : HybridQ8Rows
    hybrid_feeds(r.feed, HybridPlanes.q8, [40l, HYBRID_WINDOW + 88l, HYBRID_WINDOW + 1l, 2l], [40l, HYBRID_WINDOW + 88l])
    var forms <- hybrid_q8_forms(r)
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-0.8B-Q8_0.gguf", CARRY_RIG_CTX, forms, parity = true) $(var m, form) {
            hybrid_q8_rows(t, r, m, form)
        }
    }
    if (ran) {
        hybrid_rows_within(t, "Qwen3.5-0.8B", r.feed)
        with_dasllama_jobque_() {   //! the compares decode and reduce rows off the engine's queue
            hybrid_codec_within(t, r.codec)
        }
        held_within(t, r.chain, "Qwen3.5-0.8B, the greedy chain: ", false)
        held_within(t, r.regions, "Qwen3.5-0.8B Q8_0, two regions: ", false)
        held_within(t, r.home_pin, "Qwen3.5-0.8B Q8_0, two regions, the planes on the host: ", false)
    }
    matrix_hybrid_verdict(t, matrix_hybrid_qwen35_0_8b(), r.matrix)
    delete r
    delete forms
}

let private CONT_BAR_REL = 0.05         //! the same device chain cut at a different place, the one-token-off control its teeth
let private HALF = 48l                  //! the continuation's first half
let private LONG_PROMPT = 5000l         //! past DN_WINDOW: the cut has rows to cut
let private PAST_WINDOW = 5120l         //! past DN_WINDOW: two deltanet chunks a window were the cap absent (the cell asserts it passes)
let private LONG_BAR_REL = 0.10         //! the device chain against the CPU's on a plain hybrid: the arms read 3%; a window past the planes read 30-72%

//! why the window cells skip on this run: the CUDA home is asked (the cells are the Vulkan home's), or DASLLAMA_VK_KV32=1 arms
//! the f32 mirror, which passes every f16 session as codec; "" where they run
def private window_cells_why() : string {
    return "the CUDA home is asked: the continuation and long-window cells are the Vulkan home's" if (gpu_want_cuda())
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return "DASLLAMA_VK_KV32=1: the f32 mirror passes every f16 session as codec" if (g_env_vulkan.vk_kv32 != 0l)
    }
    return ""
}

//! a prompt continued on the default form: its second half as a continuation window from the first half's position (the
//! regions' `continue_at`), and the same second half from a checkpoint a fresh session adopts - each against the whole prompt
//! through one window, the prompt one token short as the control
def private continuation_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("Qwen3.5-0.8B: a prompt continued from its position on the device, and from an adopted checkpoint, against one window") @capture(& tr)(t : T ?) {
            t |> success(tr.config.recr_mask != 0ul, "the carrier is the hybrid with the recurrent state a continuation carries")
            continue_at(t, tr, 2l * HALF, HALF, CONT_BAR_REL)
            var prompt <- resident_prompt(2l * HALF)
            var short_logits <- short_control(tr, prompt, 2l * HALF)
            var whole <- prefill_row(tr, prompt, 2l * HALF, mirror_kv_dtype())
            var adopted <- adopted_row(t, tr, prompt, HALF)
            let bar = row_bar(whole, CONT_BAR_REL)
            control_past(t, "the whole prompt's last row", whole, short_logits, bar)
            row_within_say(t, tr, "the continuation from the adopted checkpoint against the whole prompt", adopted, whole, bar)
            delete adopted
            delete whole
            delete short_logits
            delete prompt
        }
    }
}

//! a long prompt through the whole-model driver on a recurrent model at one window form: the window chain cuts a recurrent
//! model's window at DN_WINDOW rows, the rows the deltanet smalls planes and the scan workspace hold. The device's last row,
//! its reads held; the CPU form's row and the one-token-short control are the block's
struct LongPromptRow {
    window : int64
    ran : bool
    h : Held
    dev : array<float>
}

def private long_row(var l : LongPromptRow; tr : Model; window : int64) {
    l.window = window
    l.h |> held_equal(pf_window_rows(), window, "the pin reached the driver's window")
    l.h |> held_fact(tr.config.recr_mask != 0ul, "the carrier is the hybrid whose recurrent window the cap bounds")
    var prompt <- resident_prompt(LONG_PROMPT)
    let served0 = gpu_resident_prefills_()
    var sd <- create_session(tr, mirror_kv_dtype())
    forward_prefill(tr, sd, prompt, LONG_PROMPT, 0l)
    l.h |> held_equal(gpu_resident_prefills_(), served0 + 1l, "the window chain served the prompt")
    l.h |> held_served()
    l.dev <- [for (x in sd.logits); x]
    l.ran = true
    delete sd
    delete prompt
}

//! what the Q4_K_M file's forms collect: the feeds, the long prompt at the wide window and past it, the CPU chain's last row of
//! the long prompt and of the prompt one token short, and every logged id decoded
struct HybridKqRows {
    feed : HybridRows
    wide : LongPromptRow
    past : LongPromptRow
    cpu_row : array<float>
    cpu_short : array<float>
    pieces : table<int64; string>
}

//! `row`'s argmax decoded into `pieces`, where the row ran
def private keep_argmax_piece(var pieces : table<int64; string>; m : Model; row : array<float>) {
    return if (empty(row))
    keep_piece(pieces, m, argmax_of(row))
}

def private hybrid_kq_rows(t : T?; var r : HybridKqRows; var m : Model; form : ModelForm) {
    if (form.on_gpu && form.pf_window_rows == PF_ROWS_WIDE) {
        long_row(r.wide, m, PF_ROWS_WIDE)
        return
    }
    if (form.on_gpu && form.pf_window_rows == PAST_WINDOW) {
        long_row(r.past, m, PAST_WINDOW)
        return
    }
    let side = hybrid_rows(r.feed, m, form)
    if (side == MainSide.device && empty(window_cells_why())) {
        continuation_cell(t, m)
    } elif (side == MainSide.cpu && (r.wide.ran || r.past.ran)) {
        var prompt <- resident_prompt(LONG_PROMPT)
        r.cpu_row <- prefill_row(m, prompt, LONG_PROMPT, form.kv)
        r.cpu_short <- short_control(m, prompt, LONG_PROMPT, form.kv)
        keep_argmax_piece(r.pieces, m, r.cpu_row)
        keep_argmax_piece(r.pieces, m, r.wide.dev)
        keep_argmax_piece(r.pieces, m, r.past.dev)
        delete prompt
    }
}

//! the long prompt at one window form against the CPU chain's last row, the one-token-short prompt the control
def private long_row_within(t : T?; r : HybridKqRows; l : LongPromptRow; name : string) {
    return if (!l.ran)
    unsafe {
        t |> run(name) @capture(& r, & l)(t : T ?) {
            held_within(t, l.h)
            t |> success(!empty(r.cpu_row) && !empty(r.cpu_short), "the CPU form ran the long prompt's rows")
            return if (empty(r.cpu_row) || empty(r.cpu_short))
            let bar = row_bar(r.cpu_row, LONG_BAR_REL)
            control_past(t, "the prompt's last row", r.cpu_row, r.cpu_short, bar)
            let pg = argmax_of(l.dev)
            let pw = argmax_of(r.cpu_row)
            to_log(LOG_INFO, "the device's last row at a {l.window}-row window: argmax {pg} '{r.pieces?[pg] ?? ""}' ({l.dev[pg]}) against {pw} '{r.pieces?[pw] ?? ""}' ({r.cpu_row[pw]})\n")
            row_within(t, "the device's last row at a {l.window}-row window against the CPU chain", l.dev, r.cpu_row, bar)
        }
    }
}

[test(description = "Qwen3.5-0.8B-Q4_K_M keeps its deltanet planes K-quant and holds the CPU chain within the K-quant bar over one and two windows - on the Vulkan home the f16 feed riding the k4 cm2 tiles, no further from the f32 chain than the CPU; on the CUDA home within 10 percent with every position served; on the Vulkan home a prompt's second half continued in place and from an adopted checkpoint lands within 5 percent of the whole prompt in one window while the prompt one token short lands past it, and a 5000-token prompt lands its last row within 10 percent of the CPU chain's at the wide window equal to DN_WINDOW and at a 5120-row window the recurrent cut bounds; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token with its chunked-prefill arm and its deltanet planes K-quant native; each form loads once")]
def test_gpu_resident_hybrid_kq(t : T?) {
    let window_was = pin_window_rows()
    defer() {
        set_pf_window_rows_pin(window_was)
    }
    return if (!family_on(t, "qwen35"))
    var r : HybridKqRows
    var forms : array<ModelForm>
    if (mirror_kv_dtype() == KVDtype.f32) {
        skip_cell(t, "the K-quant bar is calibrated on the f16 mirror the tier arms; the f32 A/B mirror (DASLLAMA_VK_KV32=1) is no gate for it")
    } else {
        hybrid_kq_forms(t, forms, r)
    }
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-0.8B-Q4_K_M.gguf", REGIONS_RIG_CTX, forms, parity = true) $(var m, form) {
            hybrid_kq_rows(t, r, m, form)
        }
    }
    if (ran) {
        hybrid_rows_within(t, "Qwen3.5-0.8B-Q4_K_M", r.feed)
        with_dasllama_jobque_() {   //! the compares decode and reduce rows off the engine's queue
            long_row_within(t, r, r.wide, "Qwen3.5-0.8B: a {LONG_PROMPT}-token prompt at the wide prompt window against the CPU chain - the recurrent window uncut at DN_WINDOW")
            long_row_within(t, r, r.past, "Qwen3.5-0.8B: a {LONG_PROMPT}-token prompt at a {PAST_WINDOW}-row prompt window against the CPU chain - the recurrent window cut at DN_WINDOW")
        }
    }
    delete r
    delete forms
}

//! the Q4_K_M file's K-quant forms pushed onto `forms`: the default (the feeds), the f32 chain's witness off the CUDA home, the
//! wide and the past-DN_WINDOW prompt windows where the window cells run (`window_cells_why`, a held skip where they do not)
def private hybrid_kq_forms(t : T?; var forms : array<ModelForm>; var r : HybridKqRows) {
    hybrid_feeds(r.feed, HybridPlanes.kq, [40l, HYBRID_WINDOW + 88l], [40l, HYBRID_WINDOW + 88l])
    forms |> push(device_form())
    let why = window_cells_why()
    forms_or_skips(t, forms, "Qwen3.5-0.8B-Q4_K_M.gguf", [f32_form()], f32_witness_why())
    if (empty(why)) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            r.feed.carrier |> held_equal(PF_ROWS_WIDE, DN_WINDOW, "the wide window the driver picks first is one deltanet chunk ({PF_ROWS_WIDE} rows, DN_WINDOW {DN_WINDOW}): the window runs uncut")
            r.feed.carrier |> held_fact(PAST_WINDOW > DN_WINDOW, "the window asked passes DN_WINDOW ({PAST_WINDOW} over {DN_WINDOW}): the cut is what the cell reads")
        }
        forms |> push(device_form(pf_window_rows = PF_ROWS_WIDE))
        forms |> push(device_form(pf_window_rows = PAST_WINDOW))
    } else {
        r.feed.carrier |> held_skip("the window cells: {why}")
    }
}

[test(description = "The mixed twin with K-quant deltanet qkv/z and a Q8_0 out plane holds the CPU chain within its bar over one and two windows on the Vulkan home, the qkv/z feed on the k4 tiles, no further from the f32 chain than the CPU, and over one window within 10 percent with every position served on the CUDA home; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token with its chunked-prefill arm; on the Metal home's blob twin, one load on a das_metal build, a recurrent layer mixing a Q8_0 out plane with K-quant qkv batched at 3, 9 and 12 rows token for token the single-row step with no decline; each form loads once")]
def test_gpu_resident_hybrid_kq_q8out(t : T?) {
    let window_was = pin_window_rows()
    defer() {
        set_pf_window_rows_pin(window_was)
    }
    return if (!family_on(t, "qwen35"))
    var r : HybridRows
    var forms : array<ModelForm>
    if (mirror_kv_dtype() == KVDtype.f32) {
        skip_cell(t, "the K-quant bar is calibrated on the f16 mirror the tier arms; the f32 A/B mirror (DASLLAMA_VK_KV32=1) is no gate for it")
    } else {
        hybrid_feeds(r, HybridPlanes.kq_q8out, [40l, HYBRID_WINDOW + 88l], [40l])
        forms |> push(device_form())
        forms_or_skips(t, forms, "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", [f32_form()], f32_witness_why())
    }
    forms |> push(metal_form(ctx = DECODE_CTX_QWEN35_08B_Q8OUT))
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", CARRY_RIG_CTX, forms, parity = true) $(var m, form) {
            if (form.on_metal) {
                decode_mixed_sites_qwen35_08b_metal(t, m, form)
            } else {
                hybrid_rows(r, m, form)
            }
        }
    }
    if (ran) {
        hybrid_rows_within(t, "Qwen3.5-0.8B-Q4_K_M-q8out", r)
    }
    delete r
    delete forms
}

//! what the -MTP- file's forms collect: the head cells' device halves and their CPU halves - the carry, the batched carry, the
//! head warm and the rounds on the default codec and again over a q8_0 mirror, the head kept off the device - and on the CUDA
//! home the trunk's forced feed, its counters and their control; `main_kv` the codec the one-region form armed
struct HybridMtpRows {
    cuda : bool
    main_ran : bool
    main_kv : KVDtype
    carry : MtpCarryRows
    batched : MtpBatchedRows
    warms : array<MtpWarmRows>
    rounds : MtpRoundsRows
    q8_ran : bool
    q8_warms : array<MtpWarmRows>
    q8_rounds : MtpRoundsRows
    head_off : MtpHeadOffRows
    trunk : FeedRows
    trunk_cpu : FeedRows
    cuda_counters : Held
    dump : DumpKept
}

let private DUMP_BAR_REL = 0.20   //! the parity dump's row bar: the regions files'

//! what the parity dump round trip's device form hands its CPU form and
//! the compares after the block: the device arm's dump read back off
//! its file, the CPU arm's replayed on its tape and read back, what the
//! replay saw of the tape, and every row's argmax decoded
struct DumpKept {
    dev_ran : bool
    cpu_ran : bool
    kv : KVDtype
    back : ParityDump
    cpu_back : ParityDump
    tape_seen : int64
    tape_miss : int64
    pieces : table<int64; string>
}

//! the dump round trip's device half on the whole-model driver: the
//! device arm's forced-feed run - the prompt, its own six-token greedy
//! continuation - written to a file and read back as written
def private dump_device_rows(t : T?; var k : DumpKept; var m : Model) {
    unsafe {   //! the cell reads the block's model by reference
        let title = "Qwen3.5-0.8B-MTP: the device arm's parity dump, written and read back"
        t |> run(title) @capture(& m, & k)(t : T ?) {
            t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the carrier")
            let dev_path = path_join(test_temp_dir(), "parity_dev.pd")
            with_dasllama_jobque_() {
                var inscope prompt <- resident_prompt(40l)
                let none : array<int64>
                var inscope dev <- parity_dump_run(m, MTP_CARRY_FILE, mirror_kv_dtype(), prompt,
                    none, 6l, none)
                t |> equal(dev.arm, "device", "the first arm is the device's")
                t |> equal(length(dev.rows), 7, "the prefill's row and six fed steps")
                t |> equal(length(dev.feed), 6, "six fed tokens, the run's own greedy continuation")
                t |> equal(int(moe_pick_tape_mode().mode), int(MoePickTape.off),
                    "the tape is off after the run")
                parity_dump_write(dev_path, dev)
                k.back <- parity_dump_read(dev_path)
                let header = (k.back.arm == dev.arm && k.back.vocab == dev.vocab
                    && k.back.picks_per_select == dev.picks_per_select)
                let ids = (ids_equal(k.back.prompt, dev.prompt) && ids_equal(k.back.feed, dev.feed)
                    && ids_equal(k.back.tape, dev.tape))
                t |> success(header && ids, "the dump reads back as written")
                t |> equal(logits_maxdiff(k.back.rows[3], dev.rows[3]), 0.0,
                    "... its rows bit for bit")
            }
            k.kv = mirror_kv_dtype()
            k.dev_ran = true
        }
    }
}

//! the dump round trip's CPU half, once, on the q8 CPU form: the CPU
//! arm replayed on the device dump's prompt, feed and routing tape off
//! its file, written and read back; every row's argmax decoded
def private dump_cpu_rows(var k : DumpKept; var m : Model) {
    return if (!k.dev_ran || k.cpu_ran)
    k.cpu_ran = true
    let cpu_path = path_join(test_temp_dir(), "parity_cpu.pd")
    with_dasllama_jobque_() {
        var inscope cpu <- parity_dump_run(m, MTP_CARRY_FILE, k.kv, k.back.prompt, k.back.feed,
            0l, k.back.tape)
        k.tape_seen = cpu.tape_seen
        k.tape_miss = cpu.tape_miss
        parity_dump_write(cpu_path, cpu)
        k.cpu_back <- parity_dump_read(cpu_path)
    }
    row_pieces(k.pieces, k.back.rows, m)
    row_pieces(k.pieces, k.cpu_back.rows, m)
}

//! every row's argmax decoded into `pieces`, keyed by its id
def private row_pieces(var pieces : table<int64; string>; rows : array<array<float>>; m : Model) {
    for (row in rows) {
        let id = argmax_of(row)
        pieces[id] = decode_(m, [id])
    }
}

//! the dump round trip's compares after the block: the CPU arm on the
//! device arm's routing, the two dumps within the bar on every row, a
//! poisoned row red on its own, a reference with another feed refused
def private dump_within(t : T?; var k : DumpKept) {
    if (k.dev_ran && !k.cpu_ran) {
        t |> failure(("{MTP_CARRY_FILE}: the device arm's parity dump has no CPU replay - the q8 " +
            "CPU form ran none"))
    }
    return if (!k.cpu_ran)
    t |> equal(k.cpu_back.arm, "cpu", "the second arm is the CPU chain's")
    //! the carrier is dense: its tape is empty and the replay sees no
    //! select; a routed carrier's replay counts every select
    let dense = k.back.picks_per_select == 0l
    t |> success(dense ? k.tape_seen == 0l && empty(k.back.tape) : k.tape_seen > 0l,
        ("the CPU arm replayed the device arm's routing ({k.tape_seen} selects, {k.tape_miss} " +
        "own picks off the tape, {k.back.picks_per_select} picks a select)"))
    var inscope r <- parity_dump_compare(k.back, k.cpu_back, DUMP_BAR_REL)
    for (row in r.rows) {
        let dev_piece = k.pieces?[row.argmax_a] ?? ""
        let cpu_piece = k.pieces?[row.argmax_b] ?? ""
        to_log(LOG_INFO, ("parity dump row {row.row}: maxdiff {row.diff} (bar {row.bar}) " +
            "one-step-off {row.ctrl}, argmax device {row.argmax_a} '{dev_piece}' against cpu " +
            "{row.argmax_b} '{cpu_piece}'\n"))
    }
    t |> success(r.ok && empty(r.why), ("the device dump is within the bar of the CPU dump on " +
        "every row, every one-step-off control past it ({r.why})"))
    k.back.rows[2][17] = k.back.rows[2][17] + 3.0 * r.rows[2].bar
    var inscope poisoned <- parity_dump_compare(k.back, k.cpu_back, DUMP_BAR_REL)
    t |> success(!poisoned.ok && !poisoned.rows[2].ok && poisoned.rows[3].ok,
        "a poisoned row (one logit three bars off) reds its compare alone")
    k.cpu_back.feed[0] = k.cpu_back.feed[0] + 1l
    var inscope refused <- parity_dump_compare(k.back, k.cpu_back, DUMP_BAR_REL)
    t |> success(!refused.ok && !empty(refused.why),
        "a reference with another feed is refused ({refused.why})")
}

//! the one-region form's cells, in the order a load serves them: every served cell first, then the cells whose calls pass to
//! the CPU by design (the pass arms, the rewind) and the passes they leave cleared, then the suppressed id's re-arm, and the
//! NaN carry's panic last
def private hybrid_mtp_main_rows(t : T?; var r : HybridMtpRows; var m : Model; form : ModelForm) {
    r.main_ran = true
    r.main_kv = form.kv
    mtp_carry_rows(r.carry, m)
    mtp_carry_gate_cell(t, m)
    mtp_draft_cell(t, m)
    mtp_draft_upload_cell(t, m)
    mtp_draft_unwarmed_cell(t, m)
    mtp_verify_cell(t, m)
    mtp_verify_reject_cell(t, m)
    mtp_verify_reject_asked_cell(t, m)
    mtp_xb_poison_cell(t, m)
    mtp_head_warm_rows(r.warms, m)
    if (r.cuda) {
        r.cuda_counters |> held_fact(!mtp_head_warm_declined(), "control: the head-warm fault seat lifted, the next load's warm does not decline")
    }
    mtp_rounds_rows(r.rounds, m)
    unsafe {
        t |> run("Qwen3.5-0.8B-MTP: the same on the NextN-headed carrier with speculation off, whose peel warms no head row; with it on, a short tail takes the chain, which warms the head's rows") @capture(& m)(t : T ?) {
            mtp_cell_served(t) $ {
                let spec0 = get_mtp_spec()
                set_mtp_spec(false)
                recurrent_peel_cell(t, m, SPLIT_BAR_REL)
                set_mtp_spec(true)
                assert_short_window_takes_chain(t, m, "a NextN-headed model under speculation")
                set_mtp_spec(spec0)
            }
        }
    }
    if (r.cuda) {   //! the CUDA home's trunk with the head unused: one window and eight plain steps
        r.cuda_counters |> held_equal(m.arch, "qwen35", "the file is a qwen35-family model")
        r.trunk <- cuda_feed_rows(r.cuda_counters, m, form, 40l, 1l)
    }
    mtp_pass_arms_cell(t, m)
    mtp_rewind_refused_cell(t, m)
    reset_gpu_cpu_passes()   //! the arms pass to the CPU by design: the form's own witness reads the calls after them
    mtp_draft_suppressed_cell(t, m)
    mtp_nonfinite_cell(t, m)
}

let private MTP_SCHED_CTX = 512l   //! the speculative scheduler cells' sessions' context: the scheduler form's, the CPU form's cut

//! the speculative scheduler's cells on the q8 CPU form, its device state dropped: the stop close and the length cap at
//! MTP_SCHED_CTX, the self-speculative streams against the plain scheduler where the GPU tier is not armed (where it is, the
//! plain device form runs them), the checkpoint stops inside one window
def private mtp_scheduler_cpu_cells(t : T?; var m : Model) {
    with_ctx_cut(m, MTP_SCHED_CTX) {
        test_scheduler_mtp_stop_close(t, m)
        test_scheduler_mtp_length_cap(t, m)
    }
    if (!empty(gpu_tier_why())) {
        scheduler_mtp_cell(t, m)
    }
    scheduler_capture_stops_cell(t, m)
}

//! the -MTP- file's Metal cells: the MTP rail's arms on the served
//! image, the batched cells on the blob twin
def private hybrid_mtp_metal_rows(t : T?; var m : Model; form : ModelForm) {
    if (form.metal_image) {
        mtp_parity_metal(t, m, form, mtp_parity_qwen35_0_8b())
    } else {
        metal_qwen35_mtp_cells(t, m)
    }
}

//! one form's collection of the -MTP- file's cells into `r`
def private hybrid_mtp_rows(t : T?; var r : HybridMtpRows; var m : Model; form : ModelForm) {
    if (form.on_metal) {
        hybrid_mtp_metal_rows(t, m, form)
        return
    }
    if (!form.on_gpu && form.quant == QuantMode.q4_0) {
        mtp_q4_0_cell(t, m)
        return
    }
    if (!form.on_gpu) {
        if (r.main_ran && form.kv == r.main_kv) {
            mtp_carry_cpu_rows(r.carry, m, form.kv)
            mtp_carry_batched_cpu_rows(r.batched, m, form.kv)
            mtp_head_warm_cpu_rows(r.warms, m, form.kv)
            mtp_rounds_cpu_rows(r.rounds, m, form.kv)
            mtp_head_off_cpu_rows(r.head_off, m, form.kv)
            if (r.cuda) {
                with_cpu_counters_still(r.cuda_counters, true) {
                    r.trunk_cpu <- resident_feed_rows(m, form, 40l, DENSE_FED_STEPS)
                }
            }
        }
        if (r.q8_ran && form.kv == KVDtype.q8_0) {
            mtp_head_warm_cpu_rows(r.q8_warms, m, form.kv)
            mtp_rounds_cpu_rows(r.q8_rounds, m, form.kv)
        }
        if (form.alone) {
            mtp_cpu_cells(t, m)
            mtp_scheduler_cpu_cells(t, m)
            dump_cpu_rows(r.dump, m)
        }
        return
    }
    mtp_carrier_held(t, m)
    if (form.cuda_head_warm_fault) {
        mtp_head_warm_declined_cell(t, m)
    } elif (form.regions == 0l) {
        scheduler_mtp_cell(t, m)
        dump_device_rows(t, r.dump, m)
    } elif (form.ctx == MTP_SCHED_CTX) {
        test_scheduler_mtp_resident_picks(t, m)
        test_scheduler_mtp_device_home(t, m)
    } elif (form.head_off) {
        mtp_head_off_rows(r.head_off, m)
        reset_gpu_cpu_passes()   //! the head-off round passes as verify_rows and verify_window by design
    } elif (form.block_rounds) {
        mtp_block_rounds_cell(t, m)
    } elif (form.mtp_depth == 8l) {
        mtp_depth8_cell(t, m)
    } elif (form.mtp_depth == 1l) {
        mtp_depth1_cell(t, m)
    } elif (form.mtp_depth == 2l) {
        mtp_sampled_selection_cell(t, m)
    } elif (form.kv_asked && form.kv == KVDtype.q8_0) {
        r.q8_ran = true
        unsafe {
            t |> run("the NextN head over a q8_0 mirror: the draft, the verify, the warmed head rows, six rounds") @capture(& m)(t : T ?) {
                mtp_draft_cell(t, m)
                mtp_verify_cell(t, m)
            }
        }
        mtp_head_warm_rows(r.q8_warms, m)
        mtp_rounds_rows(r.q8_rounds, m)
    } elif (form.kv_asked) {
        unsafe {
            t |> run("the NextN head over a tq4 mirror: the verify and the forced reject against the driver's own one-row steps") @capture(& m)(t : T ?) {
                mtp_verify_cell(t, m)
                mtp_verify_reject_cell(t, m)
            }
        }
    } elif (form.regions == 2l) {
        recurrent_segments_on(t, m, "On the NextN-headed Qwen3.5-0.8B-MTP hybrid segmented windows hold the split bar of solo windows, and checkpoint stops captured inside one call adopt and continue at their rows")
        mtp_carry_batched_rows(r.batched, m)
        mtp_verify_asked_cell(t, m)
        regions_pool_head_cell(t, m, MTP_CARRY_FILE)
        regions_pool_mtp_cell(t, m, MTP_CARRY_FILE)   //! the speculating scheduler last: its streams leave parked claims behind
    } else {
        hybrid_mtp_main_rows(t, r, m, form)
    }
}

//! the -MTP- file's forms: the device forms, every one with the planes on the host (the head cells run the CPU head on the
//! model) - one region, two on the rig's context, the head kept off the device, the depth-8 verify and its depth-1 control,
//! depth 2 for the sampled selection, the q8_0 and tq4 mirrors (a codec the environment keeps off held as a skip), one region
//! on MTP_SCHED_CTX for the resident scheduler cells, whose device-home streams leave parked claims and whose pass read is the
//! form's whole history, and the driver's own regions for the self-speculative streams against the plain scheduler, whose
//! census reads no tick of theirs a device step with the tier installed - and the q8 and q4_0 CPU forms whose cells stand
//! alone (`_mtp_cpu_cells.das`, the speculative scheduler's CPU cells)
def private hybrid_mtp_forms(t : T?; var r : HybridMtpRows) : array<ModelForm> {
    var forms : array<ModelForm>
    if (r.cuda) {
        forms |> push(device_form(regions = 1l, host = true, cuda_head_warm_fault = true))
    } else {
        skip_cell(t, "{MTP_CARRY_FILE}, the head-warm fault form: the run arms no CUDA home "
            + "(DASLLAMA_CUDA=1), whose seat the form pins")
    }
    forms |> push_from([device_form(regions = 1l, host = true), device_form(regions = 2l, host = true, ctx = REGIONS_RIG_CTX),
        device_form(regions = 1l, host = true, head_off = true), device_form(regions = 1l, host = true, mtp_depth = 8l),
        device_form(regions = 1l, host = true, mtp_depth = 1l), device_form(regions = 1l, host = true, mtp_depth = 2l),
        device_form(regions = 1l, host = true, block_rounds = true)])
    codec_forms(forms, r.cuda_counters, [KVDtype.q8_0, KVDtype.tq4], device_form(regions = 1l, host = true)) $(kv) => "the NextN head over a {kv} mirror"
    forms |> push(device_form(regions = 1l, host = true, ctx = MTP_SCHED_CTX))
    forms |> push(device_form(host = true))
    forms |> push(cpu_cells_form())
    forms |> push(cpu_cells_form(QuantMode.q4_0))
    forms |> push(metal_form(ctx = METAL_NEXTN_CTX))
    forms |> push(metal_form(image = true, ctx = METAL_MTP_CELLS_CTX))
    return <- forms
}

[test(description = "On Qwen3.5-0.8B-MTP the resident prefill and every step land the post-norm hidden as the speculative carry at its row's watermark within the deltanet bar of the CPU chain's, one row at a time and through the N-row command, and with speculation off no carry lands; a NextN draft through the resident head matches the CPU forward_mtp, uploads past the device's rows, pins a suppressed pick, and is bit-identical despite NaN-poisoned host residual rows; the round's verify rows sit within twice the deltanet bar of one-row steps, a forced reject rolls back on the device, the head's warmed K rows sit within an eighth of the row's max of the CPU warm's over one and two windows, and six rounds match the CPU rounds up to near-ties; the depth-8 knob prepares nine verify rows (depth 1 two) and its walk emits the plain decode, a seeded sampled stream at depth 2 decodes the rows-landed stream; a NaN carry panics the draft by row, the pass arms hand the round to the CPU rails by name, a rewind is refused, a head kept off the device passes the verify as verify_rows; the head holds its bars over q8_0 and tq4 mirrors; the peel with speculation off; pooled openings carry the draft head's rows; on the CUDA home the trunk's forced feed holds 4 percent of the CPU chain with every position served; at two regions segmented windows hold the split bar of solo windows, and checkpoint stops captured inside one call adopt and continue at their rows; on the q8 CPU form, on every box, the NextN extras load and the spec-off trunk continues the plain file's fixture, self-speculative greedy decode emits plain decode's tokens on the counting and prose prompts, the k-deep round at depths 2, 3 and 4 decodes the plain stream with depth 4 accepting more than depth 1, a poisoned verify and a forced-reject round roll back to plain decode, the continuation prefill's seam row is written exactly where no seat serves it, and a prefill driver serving the verify window panics the round naming it; on the q4_0 CPU form self-speculative decode emits plain decode's counting tokens; on the Metal home's blob twin, one load on a das_metal build: on Qwen3.5-0.8B-MTP Q8 with speculation off, the batched row serves through the device driver, and after plain batched steps each stream's NextN carry trails its position by at most one; on Qwen3.5-0.8B-MTP Q8 with self-speculation on, the batched row refuses by name (each stream's round runs alone), reads no rate, and counts no device or CPU batched step; on Qwen3.5-0.8B-MTP Q8, a depth-2 greedy MTP walk emits the depth-1 walk's tokens over 24 reply tokens while drafting more than it and accepting some drafts; on Qwen3.5-0.8B-MTP Q8, a greedy scheduler stream lands its device pick at most once a token and emits what `generate` emits off the host argmax, while a non-argmax sampler lands no pick; on Qwen3.5-0.8B-MTP Q8, three self-speculative streams admitted together emit each solo stream's tokens, every tick stepping each through its own round, with draft and accept counts equal to the solo sums; on Qwen3.5-0.8B-MTP Q8, three speculating streams, the last on a prompt past 200 tokens across 64-row chunk boundaries, emit each solo stream's tokens with every tick stepping their own rounds; on Qwen3.5-0.8B-MTP Q8, a self-speculative stream of 150 reply tokens on 64-row pages, spanning three pages, emits the flat stream's tokens with drafts accepted on both sides; on Qwen3.5-0.8B-MTP Q8, a third prompt on a shared opening attaches the checkpoint the second left and replies as an uncached run at the same cut, and a marked stop inside a later window is captured; on Qwen3.5-0.8B-MTP Q8, a request marked with its stable opening leaves that checkpoint at once, which the next prompt attaches and replies with as uncached, while an unmarked request leaves none there; on Qwen3.5-0.8B-MTP Q8, a session attached at an off-boundary checkpoint lands the donor's logits bit for bit after the same tail, while the checkpoint with its recurrent state zeroed lands 5% of the largest logit away or more; on Qwen3.5-0.8B-MTP Q8, a checkpoint captured at a marked stop inside one prefill window, attached and fed the tail, lands within 0.2% of an uncached prefill's largest logit, and zeroing its recurrent state misses by 5% or more; on Qwen3.5-0.8B-MTP Q8, a conversation's next turn attaches the finished turn's checkpoint, prompt plus 40 tokens, and continues the count token for token against an uncached 80-token stream; on the Metal home's served image, one load on a das_metal build, the Metal MTP rail's verify rows within 0.5 of the plain GPU step on forced feed with a one-step-off control, and speculative free-run emitting the plain run's tokens exactly; on the driver's own regions the device arm's parity dump reads back bit-exact, the CPU arm replayed on its prompt, feed and tape off the q8 CPU form matches every row within the 0.20 relative bar, a one-row poison reds only that row, and a changed feed is refused; each form loads once")]
def test_gpu_resident_hybrid_mtp(t : T?) {
    let window_was = pin_window_rows()
    defer() {
        set_pf_window_rows_pin(window_was)
    }
    return if (!family_on(t, "qwen35"))
    var r = HybridMtpRows(cuda = gpu_want_cuda())
    var forms <- hybrid_mtp_forms(t, r)
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-0.8B-MTP-Q8_0.gguf", CARRY_RIG_CTX, forms) $(m, form) {
            hybrid_mtp_rows(t, r, m, form)
        }
    }
    if (ran) {
        with_dasllama_jobque_() {   //! the compares decode and reduce rows off the engine's queue
            held_within(t, r.cuda_counters, "Qwen3.5-0.8B-MTP: ", false)
            mtp_carry_within(t, r.carry)
            mtp_carry_batched_within(t, r.batched)
            mtp_head_warm_within(t, "", r.warms)
            mtp_rounds_within(t, "", r.rounds)
            mtp_head_warm_within(t, "the NextN head over a q8_0 mirror - ", r.q8_warms)
            mtp_rounds_within(t, "the NextN head over a q8_0 mirror - ", r.q8_rounds)
            mtp_head_off_within(t, r.head_off)
            if (r.cuda && r.main_ran) {
                feed_rows_within(t, "{MTP_CARRY_FILE} cuda, the trunk with the head unused, 40 tokens: ", r.trunk, r.trunk_cpu, _mtp_resident_cells::DN_LOGIT_BAR_REL)
            }
        }
    }
    with_dasllama_jobque_() {   //! the compares reduce rows on the engine's queue
        dump_within(t, r.dump)
    }
    delete r
    delete forms
}

[test(description = "On Qwen3.5-4B-MTP Q8_0, on the Metal home's served image, one load on a das_metal build, the Metal MTP rail's verify rows stay within 0.5 of the plain GPU step on forced feed at depths 1, 2 and 4, and speculative free-run emits the plain run's tokens exactly; the form loads once")]
def test_gpu_resident_hybrid_4b_mtp(t : T?) {
    return if (!family_on(t, "qwen35"))
    var forms <- [metal_form(image = true, ctx = METAL_MTP_CELLS_CTX)]
    with_cpu_and_gpu_model(t, "llm", "Qwen3.5-4B-MTP-Q8_0.gguf", METAL_MTP_CELLS_CTX,
            forms) $(var m, form) {
        mtp_parity_metal(t, m, form, mtp_parity_qwen35_4b())
    }
    delete forms
}

//! the CUDA home's feeds of a file the Vulkan home serves no cell of: the default device form's feeds - each under the
//! driver's counters - beside the CPU form's, the first CPU feed under the counters' control; `skip` says the default form's
//! cells do not run on this card, `off` holds the MMQ-off form's one-window feed
struct CudaOnlyRows {
    prompts : array<int64>
    skip : string
    main : MainPair
    carrier : Held
    feeds : array<FeedRows>
    cpu : array<FeedRows>
    off : MmqOffFeed
}

def private cuda_only_rows(var r : CudaOnlyRows; m : Model; form : ModelForm) {
    let side = main_side(r.main, form)
    if (form.on_gpu && form.mmq_off) {
        mmq_off_rows(r.off, r.carrier, m, form)
    } elif (side == MainSide.device) {
        r.carrier |> held_equal(m.arch, "qwen35", "the file is a qwen35-family model")
        r.feeds |> reserve(length(r.prompts))
        for (n in r.prompts) {
            r.feeds |> emplace <| cuda_feed_rows(r.carrier, m, form, n, prompt_windows(n, HYBRID_WINDOW))
        }
    } elif (!form.on_gpu) {
        for (n, i in r.prompts, count()) {
            with_cpu_counters_still(r.carrier, i == 0) {
                r.cpu |> emplace <| resident_feed_rows(m, form, n, DENSE_FED_STEPS)
            }
        }
    }
}

//! the NVFP4 file's device forms where the CUDA home is asked on a CUDA build - the FP4 leaf's default form on a card past the
//! FP4 rung (its cells' skip held in `r` otherwise) and the MMQ-off form - else one loud skip naming them
def private nv4_device_forms(t : T?; var r : CudaOnlyRows; var forms : array<ModelForm>) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        let why = cuda_home_why()
        if (!empty(why)) {
            skip_cell(t, "{NV4_FILE}, the FP4 leaf's and the MMQ-off device forms: {why}")
            return
        }
        r.skip = fp4_rung_why()
        if (empty(r.skip)) {
            r.prompts <- [40l, HYBRID_WINDOW + 88l]
            forms |> push(device_form())
        } else {
            r.carrier |> held_skip("the FP4 leaf's cells: {r.skip}")
            r.prompts |> push(40l)   //! the MMQ-off form's CPU side
        }
        forms |> push(device_form(mmq_off = true))
    } else {
        skip_cell(t, "{NV4_FILE}, the FP4 leaf's and the MMQ-off device forms: no CUDA module on this build")
    }
}

//! the NVFP4 file's feeds held after its block: each FP4-leaf feed at the FP4 bar, the MMQ-off feed at the K-quant bar
[unused_argument(t, r)]   // without the cuda module no feed ran
def private nv4_within(t : T?; r : CudaOnlyRows) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        held_within(t, r.carrier, "Qwen3.5-0.8B-NVFP4-MTP: ", false)
        with_dasllama_jobque_() {
            feeds_within(t, "Qwen3.5-0.8B-NVFP4-MTP.gguf cuda, ", r.feeds, r.cpu, DN_LOGIT_BAR_REL_FP4,
                " under set_cuda_mmq_pin({cuda_mmq_name(CUDA_MMQ_WINNERS)}), the nv4 deltanet sites on the FP4 tensor leaf under the planes' factors")
            let off = "Qwen3.5-0.8B-NVFP4-MTP.gguf cuda, 40 tokens under set_cuda_mmq_pin({cuda_mmq_name(CUDA_MMQ_OFF)}) - the cm2 tiles over f16 activations hold the K-quant bar (the FP4 arm's control): "
            mmq_off_within(t, r.off, r.cpu, off, off, DN_LOGIT_BAR_REL_KQ_CUDA)
        }
    }
}

let private NV4_FILE = "Qwen3.5-0.8B-NVFP4-MTP.gguf"

[test(description = "On the CUDA driver the NVFP4 Qwen3.5-0.8B twin's deltanet sites on the FP4 tensor leaf keep forced-feed logits within 20 percent of max logit of the CPU chain over one and two windows, and pinned off MMQ the cm2 tiles hold the 10 percent bar; on its K-quant-native portable-pinned CPU form, on every box, the load keeps the nv4 planes disk-order, tags blk.0.ffn_down nv4, lands every NVFP4 plane's per-tensor scale on the site table at the file's value, places the down plane bit for bit the file dequant, and the site scale reaches the CPU matmul's epilogue; each form loads once")]
def test_gpu_resident_hybrid_nv4(t : T?) {
    let window_was = pin_window_rows()
    defer() {
        set_pf_window_rows_pin(window_was)
    }
    return if (!family_on(t, "qwen35"))
    var r : CudaOnlyRows
    var forms : array<ModelForm>
    nv4_device_forms(t, r, forms)
    forms |> push(cpu_cells_form(backend = "portable", kq_native = KqNative.on))
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-0.8B-NVFP4-MTP.gguf", CARRY_RIG_CTX, forms) $(m, form) {
            if (form.kq_native == KqNative.on) {
                unsafe {
                    t |> run("nv4 load tags the NVFP4 planes, reads their per-tensor scales and places them at their cursor offsets (Qwen3.5-0.8B NVFP4)") @capture(& m)(t : T ?) {
                        kq_nv4_load_layout_cell(t, m)
                    }
                }
            } else {
                cuda_only_rows(r, m, form)
            }
        }
    }
    if (ran) {
        nv4_within(t, r)
    }
    delete r
    delete forms
}

[test(description = "On the CUDA driver the deltanet hybrid Qwen3.5-4B Q8_0 keeps one-window prefill and eight fed-step logits within 4 percent of max logit of the CPU chain; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token with its chunked-prefill arm; each form loads once")]
def test_gpu_resident_hybrid_4b(t : T?) {
    let window_was = pin_window_rows()
    defer() {
        set_pf_window_rows_pin(window_was)
    }
    return if (!family_on(t, "qwen35"))
    var r : CudaOnlyRows
    var forms : array<ModelForm>
    let why = cuda_home_why()
    if (empty(why)) {
        r.prompts |> push(40l)
        forms |> push(device_form())
    } else {
        skip_cell(t, "Qwen3.5-4B-Q8_0.gguf: the CUDA forced feed: {why}")
    }
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-4B-Q8_0.gguf", CARRY_RIG_CTX, forms, parity = true) $(var m, form) {
            cuda_only_rows(r, m, form)
        }
    }
    if (ran) {
        held_within(t, r.carrier, "Qwen3.5-4B: ", false)
        with_dasllama_jobque_() {
            let what = "Qwen3.5-4B-Q8_0.gguf cuda, 40 tokens: "
            t |> equal(length(r.feeds), 1, "{what}the default form collected its one feed")
            if (!empty(r.feeds) && twin_collected(t, 0, length(r.cpu), what)) {
                feed_rows_within(t, what, r.feeds[0], r.cpu[0], _mtp_resident_cells::DN_LOGIT_BAR_REL)
            }
        }
    }
    delete r
    delete forms
}

[test(description = "The Vulkan kernel census panics on a name no kernel seeded, so a misspelt key cannot read as a zero count, while a seeded class kernel answers by its SPIR-V name")]
def test_kernel_census_by_name(t : T?) {
    let window_was = pin_window_rows()
    defer() {
        set_pf_window_rows_pin(window_was)
    }
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

[test(description = "row_bar scales with the finite elements of the reference row, so a NaN in the reference reads past the bar instead of widening it to infinity, the NaN-scaled bar the control")]
def test_row_bar_nan_reference(t : T?) {
    let window_was = pin_window_rows()
    defer() {
        set_pf_window_rows_pin(window_was)
    }
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
[test(description = "On synthetic rows rows_within_control and tq4_within red a NaN in the device or reference row, also with no control row, while clean rows hold; the tq4 bound passes 0.9 and reds 1.1 of the row one off's distance")]
def test_row_bars_red_a_nan(t : T?) {
    let window_was = pin_window_rows()
    defer() {
        set_pf_window_rows_pin(window_was)
    }
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
    t |> equal(tq4_bar_fails(gpu, cpu, off), 0, "control: tq4_within holds a device row nearer the reference than the row one off")
    t |> success(tq4_bar_fails(nan_gpu, cpu, off) > 0, "tq4_within reds a NaN in the device row")
    t |> success(tq4_bar_fails(gpu, nan_cpu, off) > 0, "tq4_within reds a NaN in the reference row")
    // the bound has teeth at the control: the row one off sits 4.0 from the reference and from both device rows, so a device
    // row 0.9 of that from the reference holds and one 1.1 of it away reds
    var near := cpu
    near[0] += 0.9 * 4.0
    var far := cpu
    far[0] += 1.1 * 4.0
    t |> equal(tq4_bar_fails(near, cpu, off), 0, "control: tq4_within holds a device row at 0.9 of the row one off's distance")
    t |> success(tq4_bar_fails(far, cpu, off) > 0, "tq4_within reds a device row at 1.1 of the row one off's distance - no nearer than that row")
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
