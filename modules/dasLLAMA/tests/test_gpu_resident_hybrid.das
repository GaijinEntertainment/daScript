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
def private pin_window_rows() {
    set_pf_window_rows_pin(HYBRID_WINDOW)
}
require daslib/jobque_boost
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
require _resident_regions   // the regions cells, REGIONS_RIG_CTX, short_control, continue_at
require _kv_codec_cells   // codec_env_why, codec_distances_within: the gated carrier under a block-codec mirror
require _mtp_resident_cells   // the NextN head's cells, one body for every discrete home

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
//   device-home pin's outsider.
// - Q4_K_M and the mixed twin (`..._kq`, `..._kq_q8out`): the K-quant deltanet planes kept in their file formats, the feed
//   over one and two windows at the K-quant bar with the routing witness (on the f16 feed the qkv/z pair on the k4 cm2 tiles)
//   and the f32-chain witness on the Vulkan home, the CUDA home's feeds at its K-quant bar; on the Q4_K_M twin a prompt's
//   second half continued in place and from an adopted checkpoint against the whole prompt, and a 5000-token prompt at the
//   wide window and at a 5120-row window against the CPU chain.
// - The NextN-headed twin (`..._mtp`): the carry the driver lands, the draft through the resident head, the same-slab verify,
//   the rejected verify's rollback, the head's prompt warm, six rounds against the CPU round and the gates around the round
//   (`_mtp_resident_cells.das`), the depth-8 verify and its depth-1 control, the sampled selection at depth 2, the head kept
//   off the device, the head over q8_0 and tq4 mirrors, the peel with speculation off, the pool's openings under a
//   speculating scheduler, and on the CUDA home the trunk's forced feed.
// - On the CUDA home alone: the NVFP4 twin's deltanet sites on the FP4 leaf and pinned off MMQ on the cm2 tiles (`..._nv4`),
//   and the 4B shape (`..._4b`, the `qwen35-4b` category).
// Every model test skips interpreted, without its file, or without the armed tier; a form whose plan does not fit the card
// skips loudly. The K-quant cells skip on the f32 mirrors (`DASLLAMA_VK_KV32=1`), and so do the block-codec forms and the
// codec pass arm; the window cells skip under DASLLAMA_VK_KV32=1 (the f32 mirror passes every f16 session as codec); the
// two-window K-quant feed and the two-window head warm skip where the prompt window is not the file's 512 rows. Under
// DASLLAMA_CUDA=1 the Vulkan home's own cells (the four-length and K-quant feeds' census witnesses, the f32-chain witness,
// the window cells) give way to the CUDA home's, and the NVFP4 leaf cells skip on a card below the FP4 rung (`FP4_RUNG_SM`).
// Under DASLLAMA_DEVICE_POOL=0 the pool cells hold the off rail or skip. The regions rig's no-cache scheduler cell skips its
// cache-on control on every file here: a recurrent model's claim keeps its state at its last row only, so an opening that ends
// at the prompt boundary adopts nothing.

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

//! one forced feed of a hybrid file: its prompt's length, the device's rows, the CPU form's and - for a K-quant carrier on the
//! Vulkan home - the file's f32 chain's, and on the Vulkan home the Q8_K requants and k4 cm2 tiles the prefill alone dispatched
struct HybridFeed {
    n : int64
    gpu : FeedRows
    cpu : FeedRows
    f32 : FeedRows
    rq : int64 = -1l
    tiles : int64 = -1l
}

//! what a hybrid file's feeds collect: the planes the file carries, the CUDA home's ask, the default device form beside its
//! CPU form (`main`), the K-quant planes and the f16 feed's admission as the device load read them (`carrier`), every feed, and
//! the CUDA home's counters with their control (`cuda_counters`)
struct HybridRows {
    planes : HybridPlanes
    cuda : bool
    main : MainPair
    carrier : Held
    f16_feed : bool
    nrec : int64
    nrec_ba_kq : int64
    feeds : array<HybridFeed>
    cuda_counters : Held
}

//! the feeds a hybrid file runs on the armed home: `vulkan` there, `cuda` under DASLLAMA_CUDA=1; a two-window length past the
//! window the K-quant feeds gate on is held as a skip where the prompt window is not the file's
def private hybrid_feeds(var r : HybridRows; planes : HybridPlanes; vulkan, cuda : array<int64>) {
    r.planes = planes
    r.cuda = gpu_want_cuda()
    for (n in r.cuda ? cuda : vulkan) {
        if (n > HYBRID_WINDOW && planes != HybridPlanes.q8 && !held_pf_window(r.carrier, HYBRID_WINDOW)) {
            continue
        }
        r.feeds |> emplace(HybridFeed(n = n))
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

//! one form's share of a hybrid file's feeds: the default device form's rows - on the Vulkan home with the prefill's census
//! read, on the CUDA home under the driver's counters - the CPU form's on the default form's codec, the first under the CUDA
//! counters' control, and the f32 form's
def private hybrid_rows(var r : HybridRows; m : Model; form : ModelForm) {
    let side = main_side(r.main, form)
    let kq = r.planes != HybridPlanes.q8
    if (side == MainSide.device) {
        if (r.cuda) {
            r.cuda_counters |> held_equal(m.arch, "qwen35", "the file is a qwen35-family model")
        } elif (kq) {
            hybrid_kq_planes(r, m)
        }
        for (f in r.feeds) {
            if (r.cuda) {
                f.gpu <- cuda_feed_rows(r.cuda_counters, m, form, f.n, prompt_windows(f.n, HYBRID_WINDOW))
                continue
            }
            var inscope prompt <- resident_prompt(f.n)
            var inscope toks <- resident_feed(HYBRID_FED_STEPS)
            let rq0 = census("cls_q8k_rq_spv")
            let tiles0 = kq_cm2_tiles_census(KqFmt.k4)
            f.gpu <- feed_rows(m, form, prompt, toks) $ {
                f.rq = census_since("cls_q8k_rq_spv", rq0)
                f.tiles = census_delta(tiles0, kq_cm2_tiles_census(KqFmt.k4))
            }
        }
    } elif (side == MainSide.cpu) {
        for (f, i in r.feeds, count()) {
            with_cpu_counters_still(r.cuda_counters, r.cuda && i == 0) {
                f.cpu <- resident_feed_rows(m, form, f.n, HYBRID_FED_STEPS)
            }
        }
    } elif (!form.on_gpu && form.quant == QuantMode.fp32) {
        for (f in r.feeds) {
            f.f32 <- resident_feed_rows(m, form, f.n, HYBRID_FED_STEPS)
        }
    }
}

//! the K-quant routing witness on one Vulkan feed: off the f16 feed the x route requants once per recurrent layer per window; on
//! it only the K-quant beta / alpha planes' GEMMs read a Q8_K row (one a recurrent layer a window), the qkv / z pair the f16 rows
//! on the K-quant cm2 tiles
def private hybrid_census_within(t : T?; r : HybridRows; f : HybridFeed) {
    return if (f.rq == NO_CENSUS)
    let windows = prompt_windows(f.n, HYBRID_WINDOW)
    to_log(LOG_INFO, "prefill ({f.n} tokens, {windows} windows) [{tile_arm_tag()}]: {f.rq} Q8_K requants over {r.nrec} recurrent layers ({r.nrec_ba_kq} with K-quant beta / alpha planes), {f.tiles} k4 cm2 tiles, f16 feed admits kq {r.f16_feed}\n")
    if (r.f16_feed) {
        t |> success(f.tiles > 0l, "on the f16 feed the K-quant qkv/z pair's GEMMs ride the k4 cm2 tiles ({f.tiles} dispatches) [{tile_arm_tag()}]")
        t |> success(f.rq <= windows * r.nrec_ba_kq + 1l, "on the f16 feed the prefill requantized the row to Q8_K for the K-quant beta / alpha planes alone, once a recurrent layer a window, and never for the qkv/z pair ({f.rq} Q8_K requants, {r.nrec_ba_kq} such layers, {windows} windows, the classifier's one) [{tile_arm_tag()}]")
    } else {
        t |> equal(f.tiles, 0l, "off the f16 feed no k4 cm2 tile serves the pair [{tile_arm_tag()}]")
        t |> success(f.rq >= windows * r.nrec, "off the f16 feed the K-quant qkv/z pair reads the Q8_K form ({f.rq} Q8_K requants, {r.nrec} recurrent layers, {windows} windows) [{tile_arm_tag()}]")
    }
}

//! a hybrid feed's bar on the armed home: the deltanet bar on the Q8_0 planes; on the K-quant planes the CUDA home's MMQ feed
//! bar, else the f16 feed's (the mixed twin's own) or the quant feed's by the driver's admission
def private hybrid_bar(r : HybridRows) : float {
    return DN_LOGIT_BAR_REL if (r.planes == HybridPlanes.q8)
    return DN_LOGIT_BAR_REL_KQ_CUDA if (r.cuda)
    return DN_LOGIT_BAR_REL_KQ_QUANT_FEED if (!r.f16_feed)
    return r.planes == HybridPlanes.kq_q8out ? DN_LOGIT_BAR_REL_KQ_Q8OUT : DN_LOGIT_BAR_REL_KQ_F16_FEED
}

//! a hybrid file's feeds held after its block: each one's device rows against the CPU form's at the home's bar, on the Vulkan
//! home the K-quant routing witness and, on a K-quant carrier, the device no further from the file's f32 chain than the CPU
//! chain; the K-quant planes' and the CUDA counters' reads
def private hybrid_rows_within(t : T?; tag : string; r : HybridRows) {
    return if (!r.main.ran)
    let kq = r.planes != HybridPlanes.q8
    held_within(t, r.carrier, "{tag}: ", false)
    held_within(t, r.cuda_counters, "{tag} cuda: ", false)
    with_dasllama_jobque_() {
        for (f in r.feeds) {
            let windows = prompt_windows(f.n, HYBRID_WINDOW)
            let home = r.cuda ? "the CUDA driver" : "the resident driver"
            unsafe {
                t |> run("{tag}: a {f.n}-token prompt ({windows} window{windows > 1l ? "s" : ""}) and {HYBRID_FED_STEPS} fed steps on {home} against the CPU chain ({cuda_knob()})") @capture(& r, & f)(t : T ?) {
                    if (kq && !r.cuda) {
                        hybrid_census_within(t, r, f)
                    }
                    feed_rows_within(t, "", f.gpu, f.cpu, hybrid_bar(r))
                    if (kq && !r.cuda) {   // the K-quant carriers' same-or-better witness: the device no further from the file's f32 chain than the CPU q8 chain
                        f32_chain_witness(t, "{f.n} tokens against the f32 chain [{tile_arm_tag()}]", f.gpu.rows, f.cpu.rows, f.f32.rows,
                                F32_DIST_REL) $(id) {
                            return feed_piece(f.gpu, f.cpu, id)
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
}

//! one form's collection of the Q8_0 file's cells into `r`; the one- and two-region forms' own cells run on their load
def private hybrid_q8_rows(t : T?; var r : HybridQ8Rows; var m : Model; form : ModelForm) {
    if (form.on_gpu && form.regions == 1l) {
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
    } elif (form.on_gpu && form.kv_asked) {
        hybrid_codec_rows(r.codec, m, form)
    } else {
        hybrid_rows(r.feed, m, form)
        if (!form.on_gpu) {
            hybrid_codec_rows(r.codec, m, form)
        }
    }
}

//! the Q8_0 file's forms: the default (the feeds), the q8_0 and tq4 mirrors (a codec the environment keeps off held as a skip),
//! one region (the greedy chain, the peel, the region taken), two regions on the rig's 2048-row window (its cells read prompts
//! sized against it) and the same with the planes on the host (the pin cell)
def private hybrid_q8_forms(var r : HybridQ8Rows) : array<ModelForm> {
    var forms <- [device_form(), device_form(regions = 1l, ctx = REGIONS_RIG_CTX),
        device_form(regions = 2l, ctx = REGIONS_RIG_CTX, pf_window_rows = PF_WINDOW_ROWS),
        device_form(regions = 2l, host = true, ctx = REGIONS_RIG_CTX, pf_window_rows = PF_WINDOW_ROWS)]
    codec_forms(forms, r.feed.carrier, [KVDtype.q8_0, KVDtype.tq4], device_form()) $(kv) => "the {kv} mirror under the gated attention"
    return <- forms
}

[test(description = "On Qwen3.5-0.8B Q8_0 the resident driver holds the CPU chain within the deltanet bar on forced feeds over one window, two, one past the window and a two-token prompt on the Vulkan home and over one window and two with every position served on the CUDA home; under the gated attention a q8_0 K/V mirror holds the CPU q8_0 chain within 6 percent and a tq4 mirror sits no further than 1.15 times the CPU tq4 chain's distance from the f16 rows; at one region the greedy chain's steps reproduce the plain steps bit for bit, a short tail peels within the split bar of one-row steps and a host-cached session losing its region keeps its device-only rows; at two regions two sessions' recurrent state slots step interleaved, batched and batched first as each reads alone, device-home sessions step as a host-cached session alone, a prefill continued past zero starts from the region's live state within the split bar, a recurrent parked claim hands its rows and state over, and the device-mode, paged and pooled schedulers emit the host-cached schedulers' tokens; each form loads once")]
def test_gpu_resident_hybrid_q8(t : T?) {
    pin_window_rows()
    return if (!family_on(t, "qwen35"))
    var r : HybridQ8Rows
    hybrid_feeds(r.feed, HybridPlanes.q8, [40l, HYBRID_WINDOW + 88l, HYBRID_WINDOW + 1l, 2l], [40l, HYBRID_WINDOW + 88l])
    var forms <- hybrid_q8_forms(r)
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-0.8B-Q8_0.gguf", CARRY_RIG_CTX, forms) $(m, form) {
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
    let side = main_side(r.feed.main, form)
    hybrid_rows(r.feed, m, form)
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

[test(description = "Qwen3.5-0.8B-Q4_K_M keeps its deltanet planes K-quant and holds the CPU chain within the K-quant bar over one and two windows - on the Vulkan home the f16 feed riding the k4 cm2 tiles, no further from the f32 chain than the CPU; on the CUDA home within 10 percent with every position served; on the Vulkan home a prompt's second half continued in place and from an adopted checkpoint lands within 5 percent of the whole prompt in one window while the prompt one token short lands past it, and a 5000-token prompt lands its last row within 10 percent of the CPU chain's at the wide window equal to DN_WINDOW and at a 5120-row window the recurrent cut bounds; each form loads once")]
def test_gpu_resident_hybrid_kq(t : T?) {
    pin_window_rows()
    return if (!family_on(t, "qwen35"))
    if (mirror_kv_dtype() == KVDtype.f32) {
        t |> skip("the K-quant bar is calibrated on the f16 mirror the tier arms; the f32 A/B mirror (DASLLAMA_VK_KV32=1) is no gate for it")
        return
    }
    var r : HybridKqRows
    hybrid_feeds(r.feed, HybridPlanes.kq, [40l, HYBRID_WINDOW + 88l], [40l, HYBRID_WINDOW + 88l])
    var forms <- [device_form()]
    let why = window_cells_why()
    if (!r.feed.cuda) {
        forms |> push(f32_form())
    }
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
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-0.8B-Q4_K_M.gguf", REGIONS_RIG_CTX, forms) $(m, form) {
            hybrid_kq_rows(t, r, m, form)
        }
    }
    if (ran) {
        hybrid_rows_within(t, "Qwen3.5-0.8B-Q4_K_M", r.feed)
        long_row_within(t, r, r.wide, "Qwen3.5-0.8B: a {LONG_PROMPT}-token prompt at the wide prompt window against the CPU chain - the recurrent window uncut at DN_WINDOW")
        long_row_within(t, r, r.past, "Qwen3.5-0.8B: a {LONG_PROMPT}-token prompt at a {PAST_WINDOW}-row prompt window against the CPU chain - the recurrent window cut at DN_WINDOW")
    }
    delete r
    delete forms
}

[test(description = "The mixed twin with K-quant deltanet qkv/z and a Q8_0 out plane holds the CPU chain within its bar over one and two windows on the Vulkan home, the qkv/z feed on the k4 tiles, no further from the f32 chain than the CPU, and over one window within 10 percent with every position served on the CUDA home; each form loads once")]
def test_gpu_resident_hybrid_kq_q8out(t : T?) {
    pin_window_rows()
    return if (!family_on(t, "qwen35"))
    if (mirror_kv_dtype() == KVDtype.f32) {
        t |> skip("the K-quant bar is calibrated on the f16 mirror the tier arms; the f32 A/B mirror (DASLLAMA_VK_KV32=1) is no gate for it")
        return
    }
    var r : HybridRows
    hybrid_feeds(r, HybridPlanes.kq_q8out, [40l, HYBRID_WINDOW + 88l], [40l])
    var forms <- [device_form()]
    if (!r.cuda) {
        forms |> push(f32_form())
    }
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-0.8B-Q4_K_M-q8out.gguf", CARRY_RIG_CTX, forms) $(m, form) {
            hybrid_rows(r, m, form)
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

//! one form's collection of the -MTP- file's cells into `r`
def private hybrid_mtp_rows(t : T?; var r : HybridMtpRows; var m : Model; form : ModelForm) {
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
        return
    }
    mtp_carrier_held(t, m)
    if (form.head_off) {
        mtp_head_off_rows(r.head_off, m)
        reset_gpu_cpu_passes()   //! the head-off round passes as verify_rows and verify_window by design
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
        mtp_carry_batched_rows(r.batched, m)
        mtp_verify_asked_cell(t, m)
        regions_pool_head_cell(t, m, MTP_CARRY_FILE)
        regions_pool_mtp_cell(t, m, MTP_CARRY_FILE)   //! the speculating scheduler last: its streams leave parked claims behind
    } else {
        hybrid_mtp_main_rows(t, r, m, form)
    }
}

//! the -MTP- file's forms, every one with the planes on the host (the head cells run the CPU head on the model): one region,
//! two on the rig's context, the head kept off the device, the depth-8 verify and its depth-1 control, depth 2 for the sampled
//! selection, and the q8_0 and tq4 mirrors (a codec the environment keeps off held as a skip)
def private hybrid_mtp_forms(var r : HybridMtpRows) : array<ModelForm> {
    var forms <- [device_form(regions = 1l, host = true), device_form(regions = 2l, host = true, ctx = REGIONS_RIG_CTX),
        device_form(regions = 1l, host = true, head_off = true), device_form(regions = 1l, host = true, mtp_depth = 8l),
        device_form(regions = 1l, host = true, mtp_depth = 1l), device_form(regions = 1l, host = true, mtp_depth = 2l)]
    codec_forms(forms, r.cuda_counters, [KVDtype.q8_0, KVDtype.tq4], device_form(regions = 1l, host = true)) $(kv) => "the NextN head over a {kv} mirror"
    return <- forms
}

[test(description = "On Qwen3.5-0.8B-MTP the resident prefill and every step land the post-norm hidden as the speculative carry at its row's watermark within the deltanet bar of the CPU chain's, one row at a time and through the N-row command, and with speculation off no carry lands; a NextN draft through the resident head matches the CPU forward_mtp, uploads past the device's rows, pins a suppressed pick, and is bit-identical despite NaN-poisoned host residual rows; the round's verify rows sit within twice the deltanet bar of one-row steps, a forced reject rolls back on the device, the head's warmed K rows sit within twice the deltanet bar of the CPU warm's over one and two windows, and six rounds match the CPU rounds up to near-ties; the depth-8 knob prepares nine verify rows (depth 1 two) and its walk emits the plain decode, a seeded sampled stream at depth 2 decodes the rows-landed stream; a NaN carry panics the draft by row, the pass arms hand the round to the CPU rails by name, a rewind is refused, a head kept off the device passes the verify as verify_rows; the head holds its bars over q8_0 and tq4 mirrors; the peel with speculation off; pooled openings carry the draft head's rows; on the CUDA home the trunk's forced feed holds 4 percent of the CPU chain with every position served; each form loads once")]
def test_gpu_resident_hybrid_mtp(t : T?) {
    pin_window_rows()
    return if (!family_on(t, "qwen35"))
    var r = HybridMtpRows(cuda = gpu_want_cuda())
    var forms <- hybrid_mtp_forms(r)
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
                feed_rows_within(t, "{MTP_CARRY_FILE} cuda, the trunk with the head unused, 40 tokens: ", r.trunk, r.trunk_cpu, DN_LOGIT_BAR_REL)
            }
        }
    }
    delete r
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
    off : FeedRows
    off_ran : bool
}

def private cuda_only_rows(var r : CudaOnlyRows; m : Model; form : ModelForm) {
    let side = main_side(r.main, form)
    if (form.on_gpu && form.mmq_off) {
        r.off <- cuda_feed_rows(r.carrier, m, form, 40l, 1l)
        r.off_ran = true
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

[test(description = "On the CUDA driver the NVFP4 Qwen3.5-0.8B twin's deltanet sites on the FP4 tensor leaf keep forced-feed logits within 20 percent of max logit of the CPU chain over one and two windows, and pinned off MMQ the cm2 tiles hold the 10 percent bar; each form loads once")]
def test_gpu_resident_hybrid_nv4(t : T?) {
    pin_window_rows()
    return if (!family_on(t, "qwen35") || !cuda_home_asked(t))
    static_if (typeinfo builtin_module_exists(cuda)) {
        var r : CudaOnlyRows
        r.skip = fp4_rung_why()
        var forms : array<ModelForm>
        if (empty(r.skip)) {
            r.prompts <- [40l, HYBRID_WINDOW + 88l]
            forms |> push(device_form())
        } else {
            r.carrier |> held_skip("the FP4 leaf's cells: {r.skip}")
            r.prompts |> push(40l)   //! the MMQ-off form's CPU side
        }
        forms |> push(device_form(mmq_off = true))
        var ran = false
        with_cuda_mmq_winners() {
            ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-0.8B-NVFP4-MTP.gguf", CARRY_RIG_CTX, forms) $(m, form) {
                cuda_only_rows(r, m, form)
            }
        }
        if (ran) {
            held_within(t, r.carrier, "Qwen3.5-0.8B-NVFP4-MTP: ", false)
            with_dasllama_jobque_() {
                for (f, i in r.feeds, count()) {
                    let what = "Qwen3.5-0.8B-NVFP4-MTP.gguf cuda, {f.nprompt} tokens under set_cuda_mmq_pin({cuda_mmq_name(CUDA_MMQ_WINNERS)}), the nv4 deltanet sites on the FP4 tensor leaf under the planes' factors: "
                    continue if (!twin_collected(t, i, length(r.cpu), what))
                    feed_rows_within(t, what, f, r.cpu[i], DN_LOGIT_BAR_REL_FP4)
                }
                let off = "Qwen3.5-0.8B-NVFP4-MTP.gguf cuda, 40 tokens under set_cuda_mmq_pin({cuda_mmq_name(CUDA_MMQ_OFF)}) - the cm2 tiles over f16 activations hold the K-quant bar (the FP4 arm's control): "
                if (r.off_ran && twin_collected(t, 0, length(r.cpu), off)) {
                    feed_rows_within(t, off, r.off, r.cpu[0], DN_LOGIT_BAR_REL_KQ_CUDA)
                }
            }
        }
        delete r
        delete forms
    }
}

[test(description = "On the CUDA driver the deltanet hybrid Qwen3.5-4B Q8_0 keeps one-window prefill and eight fed-step logits within 4 percent of max logit of the CPU chain; each form loads once")]
def test_gpu_resident_hybrid_4b(t : T?) {
    pin_window_rows()
    return if (!family_on(t, "qwen35") || !cuda_home_asked(t))
    var r : CudaOnlyRows
    r.prompts |> push(40l)
    var ran = false
    with_cuda_mmq_winners() {
        ran = with_cpu_and_gpu_model(t, "llm", "Qwen3.5-4B-Q8_0.gguf", CARRY_RIG_CTX, [device_form()]) $(m, form) {
            cuda_only_rows(r, m, form)
        }
    }
    if (ran) {
        held_within(t, r.carrier, "Qwen3.5-4B: ", false)
        with_dasllama_jobque_() {
            let what = "Qwen3.5-4B-Q8_0.gguf cuda, 40 tokens: "
            t |> equal(length(r.feeds), 1, "{what}the default form collected its one feed")
            if (!empty(r.feeds) && twin_collected(t, 0, length(r.cpu), what)) {
                feed_rows_within(t, what, r.feeds[0], r.cpu[0], DN_LOGIT_BAR_REL)
            }
        }
    }
    delete r
}

[test(description = "The Vulkan kernel census panics on a name no kernel seeded, so a misspelt key cannot read as a zero count, while a seeded class kernel answers by its SPIR-V name")]
def test_kernel_census_by_name(t : T?) {
    pin_window_rows()
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
    pin_window_rows()
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
    pin_window_rows()
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
