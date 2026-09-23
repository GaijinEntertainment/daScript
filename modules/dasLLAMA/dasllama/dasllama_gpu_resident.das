options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_gpu_resident shared public

// The whole-model GPU RESIDENCY rail: bake the device layout offline, upload a model's stacks to
// the tier, and drive decode/prefill entirely on device. It reaches the engine only through
// contracts dasllama_common already publishes (the decode/prefill/batch override registries) and
// the device SPI in dasllama_gpu_tier — no device module is required here, so this file compiles
// on every box. "vulkan" is the tier string it registers under, not a dependency.

require dasllama/dasllama_lint public
require dasllama/dasllama_common
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_kqformat
require dasllama/dasllama_math
require dasllama/dasllama_rope        // build_rope_row_packed — the one source for every resident rope row
require dasllama/dasllama_gguf        // kq_disk_bytes - the file bytes a device plane is measured against
require daslib/f16_cvt   // f32_to_f16 — the trim pass re-packs the emb scale plane
require daslib/enum_trait
require dasllama/dasllama_par
require dasllama/dasllama_env
require math

//! The upload-walk stage a baked gather belongs to — set by the walk, stamped per entry.
enum VkBakeRole {
    none
    cls
    dense
    shexp
    dn
    attn
    qkv
    expert          //!< resident expert stack (VRAM)
    expert_stream   //!< streamed expert stack (pinned mirror; CPU planes stay — decode runs there)
    arena           //!< resident-driver arena plane (dense models)
}

enum private VkBakeMode {
    off
    collect
    slice
}

var private g_vk_bake = VkBakeMode.off
var private g_vk_bake_blob : array<uint8>
var private g_vk_bake_plan : array<VkPlanEntry>
var private g_vk_bake_cursor = 0l
var private g_vk_bake_role = VkBakeRole.none

// P2 imported mirrors: the mapped-blob window of each sliced gather, keyed woff*8+fmt (the
// marks convention) — the stream-mirror path imports these instead of copying to pinned RAM
struct private SliceWindow {
    wq : void?
    ws : void?
    wq_bytes : int64
    ws_bytes : int64
}

var private g_vk_slice_windows : table<int64; SliceWindow>

def private vk_slice_window_provider(woff : int64; fmt : int) : MoeGpuSliceWindow {
    let k = woff * 8l + int64(fmt)
    if (!key_exists(g_vk_slice_windows, k)) {
        return default<MoeGpuSliceWindow>
    }
    unsafe {
        let w & = g_vk_slice_windows[k]
        return (wq = w.wq, ws = w.ws, wqb = w.wq_bytes, wsb = w.ws_bytes)
    }
}

//! The upload walk declares its current stage here (collect stamps it, slice verifies it).
def vulkan_bake_role(r : VkBakeRole) {
    g_vk_bake_role = r
}

//! Arm collection for the next model load's GPU walk (the vulkan-flavor bake).
def vulkan_bake_begin {
    g_vk_bake = VkBakeMode.collect
    g_vk_bake_role = VkBakeRole.none
    g_vk_bake_blob |> clear()
    g_vk_bake_plan |> clear()
}

//! Move the collected planes into the model and disarm; false = nothing collected (no GPU walk ran).
def vulkan_bake_take(var t : Model) : bool {
    g_vk_bake = VkBakeMode.off
    if (empty(g_vk_bake_plan)) {
        delete g_vk_bake_blob
        return false
    }
    t.vkblob <- g_vk_bake_blob
    t.vkplan <- g_vk_bake_plan
    return true
}

//! Arm slicing for a mapped vulkan-flavor load: the walk reads t.vkblob instead of gathering.
def vulkan_bake_slice_begin {
    g_vk_bake = VkBakeMode.slice
    g_vk_bake_role = VkBakeRole.none
    g_vk_bake_cursor = 0l
    g_vk_slice_windows |> clear()
}

//! Disarm after a successful flavor load; a partially-consumed plan means the walk diverged
//! (the slicer already declined to live gathers — correct, just slower) and is worth a rebake.
def vulkan_bake_slice_end(t : Model) {
    if (g_vk_bake != VkBakeMode.off && g_vk_bake_cursor != long_length(t.vkplan)) {
        to_log(LOG_WARNING, "dasLLAMA vulkan image: the GPU walk consumed {g_vk_bake_cursor} of {length(t.vkplan)} baked gathers - the image is stale for this run's shape, consider rebaking\n")
    }
    g_vk_bake = VkBakeMode.off
}

//! Disarm without verification (a flavor-load attempt that never ran the walk).
def vulkan_bake_disarm {
    g_vk_bake = VkBakeMode.off
}

def private vk_bake_append(a : array<uint8>) : int64 {
    // 4KB-aligned starts (import windows); long_ forms — the blob crosses the int32 array rail
    let off = (long_length(g_vk_bake_blob) + 4095l) / 4096l * 4096l
    let abytes = long_length(a)
    let need = off + abytes
    // growth capped at 2GB steps past 4GB — doubling a multi-GB blob wastes up to half its size
    if (long_capacity(g_vk_bake_blob) < need) {
        let cap = long_capacity(g_vk_bake_blob)
        let grown = cap < 4_294_967_296l ? cap * 2l : cap + 2_147_483_648l
        g_vk_bake_blob |> reserve(max(need, grown))
    }
    g_vk_bake_blob |> resize(need)
    if (abytes > 0l) {
        unsafe {
            memcpy(addr<void?>(g_vk_bake_blob[off]), addr<void?>(a[0]), abytes)
        }
    }
    return off
}

// Called AFTER the tier accepted the upload/placement (a refused gather must not enter the
// plan — the slice path would replay the refusal's absence and diverge).
def private vk_bake_collect(fmt : int; woff, n, rows : int64; wq, ws : array<uint8>) {
    if (g_vk_bake != VkBakeMode.collect) {
        return
    }
    var e = VkPlanEntry(fmt = fmt, role = int(g_vk_bake_role), woff = woff, n = n, rows = rows,
                        wq_bytes = long_length(wq), ws_bytes = long_length(ws))
    e.wq_off = vk_bake_append(wq)
    e.ws_off = vk_bake_append(ws)
    g_vk_bake_plan |> push(e)
}

def private vk_bake_slice(t : Model; fmt : int; woff, n, rows : int64;
                          var wq : array<uint8>; var ws : array<uint8>) : bool {
    if (g_vk_bake != VkBakeMode.slice) {
        return false
    }
    // the bake records only ACCEPTED gathers — serve a would-refuse mismatch EMPTY (the refusal is pure arithmetic), hold the cursor: the walk re-refuses, rolls back, and re-syncs with the plan
    if (g_vk_bake_cursor >= long_length(t.vkplan)) {
        if (!moe_gpu_would_accept(n, rows, fmt, g_vk_bake_role == VkBakeRole.expert_stream)) {
            wq |> resize(0)
            ws |> resize(0)
            return true
        }
        to_log(LOG_WARNING, "dasLLAMA vulkan image: the GPU walk overran the baked plan at gather {g_vk_bake_cursor} - regathering live from here\n")
        g_vk_bake = VkBakeMode.off
        return false
    }
    let e = t.vkplan[int(g_vk_bake_cursor)]
    if (e.fmt != fmt || e.woff != woff || e.n != n || e.rows != rows || e.role != int(g_vk_bake_role)) {
        if (!moe_gpu_would_accept(n, rows, fmt, g_vk_bake_role == VkBakeRole.expert_stream)) {
            wq |> resize(0)
            ws |> resize(0)
            return true
        }
        to_log(LOG_WARNING, "dasLLAMA vulkan image: baked gather {g_vk_bake_cursor} is (fmt {e.fmt} role {e.role} woff {e.woff} {e.n}x{e.rows}), the walk wants (fmt {fmt} role {int(g_vk_bake_role)} woff {woff} {n}x{rows}) - regathering live from here\n")
        g_vk_bake = VkBakeMode.off
        return false
    }
    g_vk_bake_cursor++
    // record the mapped window (P2: the stream-mirror path imports it instead of copying)
    unsafe {
        g_vk_slice_windows |> insert(e.woff * 8l + int64(fmt), SliceWindow(
            wq = e.wq_bytes > 0l ? addr<void?>(t.vkblob[e.wq_off]) : null,
            ws = e.ws_bytes > 0l ? addr<void?>(t.vkblob[e.ws_off]) : null,
            wq_bytes = e.wq_bytes, ws_bytes = e.ws_bytes))
    }
    // int-indexed destinations: a >2GB plane would resize truncated and then memcpy past it
    assert(e.wq_bytes <= 2_147_483_647l && e.ws_bytes <= 2_147_483_647l,
        "vulkan image: a baked plane exceeds the int-indexed array cap")
    wq |> grow_resize(e.wq_bytes)
    ws |> grow_resize(e.ws_bytes)
    unsafe {
        // int64 blob indexing — wq_off crosses INT32_MAX on multi-GB bakes
        if (e.wq_bytes > 0l) {
            memcpy(addr(wq[0]), addr<void?>(t.vkblob[e.wq_off]), e.wq_bytes)
        }
        if (e.ws_bytes > 0l) {
            memcpy(addr(ws[0]), addr<void?>(t.vkblob[e.ws_off]), e.ws_bytes)
        }
    }
    return true
}

def private moe_gpu_gather_stack(t : Model; woff : int64; n, rows, slice_rows : int64; mr, kgroup, wbias : int64;
                                 var wq : array<uint8>; var ws : array<uint8>) {
    if (vk_bake_slice(t, 0, woff, n, rows, wq, ws)) {
        return
    }
    if (t.planes_trimmed) {
        panic("dasLLAMA: planes_trimmed image - live gather impossible (baked plan diverged, or a re-gather was requested); rebake without --trim or match the image's config")
    }
    layout_gather_stack(t, woff, n, rows, slice_rows, mr, kgroup, wbias, wq, ws)
}

def private moe_gpu_gather_stack_kq(t : Model; fmt : KqFmt; woff : int64; n, rows, slice_rows : int64;
                                    repacked : bool; mr : int64; var wq : array<uint8>; var ws : array<uint8>) {
    if (vk_bake_slice(t, int(fmt), woff, n, rows, wq, ws)) {
        return
    }
    if (t.planes_trimmed) {
        panic("dasLLAMA: planes_trimmed image - live gather impossible (baked plan diverged, or a re-gather was requested); rebake without --trim or match the image's config")
    }
    layout_gather_stack_kq(t, fmt, woff, n, rows, slice_rows, repacked, mr, wq, ws)
}

def private moe_gpu_gather_stack_q51(t : Model; woff : int64; n, rows, slice_rows : int64;
                                     var wq : array<uint8>; var ws : array<uint8>) {
    if (vk_bake_slice(t, int(KqFmt.q51), woff, n, rows, wq, ws)) {
        return
    }
    if (t.planes_trimmed) {
        panic("dasLLAMA: planes_trimmed image - live gather impossible (baked plan diverged, or a re-gather was requested); rebake without --trim or match the image's config")
    }
    let qmr = active_q51_layout_mr()   //! the q51 expert stacks sit in the active backend's grp<mr> layout (repack_q51_stacks), disk order at mr 1
    layout_gather_stack_q51(t, woff, n, rows, slice_rows, qmr > 1l, qmr, wq, ws)
}

def private moe_gpu_gather_stack_mx4(t : Model; woff : int64; n, rows, slice_rows : int64;
                                     var wq : array<uint8>; var ws : array<uint8>) {
    if (vk_bake_slice(t, int(KqFmt.mx4), woff, n, rows, wq, ws)) {
        return
    }
    if (t.planes_trimmed) {
        panic("dasLLAMA: planes_trimmed image - live gather impossible (baked plan diverged, or a re-gather was requested); rebake without --trim or match the image's config")
    }
    //! the mx4 stacks ride the Q8 layout's grp<mr> interleave when the CPU repacked them (repack_mx4_stacks under the q8q8 perm), disk order otherwise
    layout_gather_stack_mx4(t, woff, n, rows, slice_rows, t.mx4_repacked, active_q8_layout_mr(), wq, ws)
}

//! an expert stack's device format tag: the native-MXFP4 planes carry no per-tensor tag (the loader keeps `experts_mx4`), so the tag is mx4 for every routed stack of such a model
def private rdec_efmt(t : Model; tags : array<KqFmt>; l : int64) : KqFmt => t.experts_mx4 ? KqFmt.mx4 : fmt_at(tags, l)

//! P3 trim (generate-then-free): pack the emb/cls region into embq/embs, mark the model
//! trimmed, and FREE the big CPU weight families — the save then writes them as empty planes.
//! Arena-only plans qualify (pure resident-driver dense); false = declined, nothing changed.
def trim_model_planes(var t : Model) : bool {
    if (empty(t.vkplan) || t.config.n_layer_nextn > 0l) {
        return false
    }
    for (e in t.vkplan) {
        if (e.role != int(VkBakeRole.arena)) {
            to_log(LOG_WARNING, "dasLLAMA trim: baked plan carries non-arena role {e.role} - only pure resident-driver dense models trim; declining\n")
            return false
        }
    }
    let dim = t.config.dim
    let vocab = t.config.vocab_size
    if (t.emb_q8) {
        // mblob-style 34B interleaved blocks over the emb region (dequant_embq_row's layout)
        let nb = vocab * dim / 32l
        t.embq |> reserve_resize(nb * 34l)   // vocab-scaled: ~600 MB at a 248k-vocab 27B, one grow
        unsafe {
            for (b in range64(nb)) {
                let off = t.emb_q8_off + b * 32l
                var bp = addr(t.embq[b * 34l])
                let sc = t.wscale_f16 ? uint(t.qscales16[off / 32l]) : f32_to_f16(t.qscales[off / 32l])
                bp[0] = uint8(sc & 0xFFu)
                bp[1] = uint8((sc >> 8u) & 0xFFu)
                memcpy(addr<void?>(t.embq[b * 34l + 2l]), addr<void?>(t.qblob[off]), 32l)
            }
        }
    } elif (cls_kq(t)) {
        // the region's plane pair verbatim (repack layout preserved), rebased to superblock 0
        let nsb = vocab * (dim / 256l)
        let sb0 = t.wcls_off / 256l
        let f = t.emb_fmt
        if (!kq_sb(f)) {
            to_log(LOG_WARNING, "dasLLAMA trim: emb fmt '{f}' has no kq plane pair - declining\n")
            return false
        }
        let qsb = kq_qsb(f)
        let ssb = kq_ssb(f)
        t.embq |> reserve_resize(nsb * qsb)
        t.embs |> reserve_resize(nsb * ssb)
        unsafe {
            memcpy(addr<void?>(t.embq[0]), addr<void?>(t.kq[int(f)].q[sb0 * qsb]), nsb * qsb)
            memcpy(addr<void?>(t.embs[0]), addr<void?>(t.kq[int(f)].s[sb0 * ssb]), nsb * ssb)
        }
    }   // fp32 embeddings ride fblob, which the trim keeps
    t.planes_trimmed = true
    delete t.qblob
    delete t.qscales
    delete t.qscales16
    delete t.wblob
    delete t.bf16blob
    for (p in t.kq) {
        delete p.q
        delete p.s
    }
    delete t.q51q
    delete t.q51s
    delete t.mxq
    delete t.mxs
    delete t.q4blob
    delete t.q4scales
    return true
}

// ===== GPU MoE tier upload (post-prepare, loader-agnostic) =====


// a plane the dense rail can serve: a format the tier has kernels for, repacked if native-kq
def private dense_plane_ok(t : Model; f : KqFmt) : bool => (f == KqFmt.q8 || kq_sb(f)) && kq_servable(t, f)

// layer l carries a parallel-dense shared expert whose whole w1/w3/w2 triple the tier can serve
def private dense_shexp_uploadable(t : Model; l : int64) : bool {
    if (!t.config.moe_dense_shexp || !gpu_want_dense_shexp() || long_length(t.w1_offs) <= l
            || t.w1_offs[l] < 0l || t.w2_offs[l] < 0l || t.w3_offs[l] < 0l) {
        return false
    }
    return (dense_plane_ok(t, fmt_at(t.w1_fmt, l)) && dense_plane_ok(t, fmt_at(t.w2_fmt, l))
        && dense_plane_ok(t, fmt_at(t.w3_fmt, l)))
}


// Gather one stack through its format's reader (q8 planes or native kq planes) and hand it to
// the tier's upload hook with the format tag.
def private moe_gpu_gather_upload(t : Model; f : KqFmt; woff : int64; n, rows, slice_rows : int64;
                                  mr, kgroup, wbias : int64; var wq : array<uint8>; var ws : array<uint8>;
                                  stream : bool = false) : bool {
    if (f == KqFmt.q8) {
        moe_gpu_gather_stack(t, woff, n, rows, slice_rows, mr, kgroup, wbias, wq, ws)
    } elif (f == KqFmt.q51) {
        moe_gpu_gather_stack_q51(t, woff, n, rows, slice_rows, wq, ws)
    } elif (f == KqFmt.mx4) {
        moe_gpu_gather_stack_mx4(t, woff, n, rows, slice_rows, wq, ws)
    } else {
        let kmr = kq_active_mr(t, f)
        moe_gpu_gather_stack_kq(t, f, woff, n, rows, slice_rows, t.kq_repacked, kmr, wq, ws)
    }
    let ok = moe_gpu_upload_stack(wq, ws, woff, n, rows, int(f), stream)
    if (ok) {
        vk_bake_collect(int(f), woff, n, rows, wq, ws)
    }
    return ok
}

//! What a whole-model resident load would cost on the device. The resident driver is all-or-nothing
//! by construction — a half-resident whole-stack driver cannot run at all — so this is the decision,
//! and it is computed from Model metadata BEFORE a single byte uploads.
struct ResidentPlan {
    weight_bytes : int64
    kv_bytes : int64
    scratch_bytes : int64   //!< the driver's own device scratch (prefill window + decode smalls)
    headroom_bytes : int64  //!< desktop slack the auto arm leaves unfilled (0 when VRAM is pinned, or when the OS budget answers)
    budget_bytes : int64
    usable_bytes : int64    //!< what the plan may fill: the budget minus the headroom, capped by the room the OS reports when it answers and no pin overrides
    others_bytes : int64    //!< dedicated video memory every other process holds, when the OS answers (0 otherwise) - named in a decline
    emb_f32_bytes : int64   //!< the raw f32 embd table the gather would upload - counted only when it rides the headroom left after weights + KV (0 = the CPU embed serves)
    ple_tbl_bytes : int64   //!< the per-layer-embedding token table (gemma-4 E-series, q8) the window chain would gather on device - counted the same way, after the embd table (0 = the host gathers)
    fits : bool
    memory : bool         //!< the decline is the card's room at the session's context, not a feature the driver lacks
    reason : string       //!< "" when it fits; otherwise why, with a remedy where one exists
}

let private RDEC_PF_ROWS = 512l                    // mirrors PF_WINDOW (dasllama_vulkan_prefill)
let private RDEC_VRAM_HEADROOM = 2_147_483_648l    // auto-arm slack past the tier's desktop reserve:
                                                   // filling weights + KV to the cap brim measured a
                                                   // 12x WDDM-demotion collapse on a live desktop
let private RDEC_VRAM_HEADROOM_PCT = 27l
let private RDEC_OS_RESERVE = 1_073_741_824l
let private RDEC_MISC_BYTES = 67_108_864l          // decode smalls + batch act planes + metas

//! Layer `l` routes through expert stacks: past the dense lead, with its expert planes present.
def layer_is_moe(t : Model; l : int64) : bool => (t.config.n_expert > 0l && l >= t.config.n_layer_dense_lead
    && l < long_length(t.we1_offs) && t.we1_offs[l] >= 0l && t.we3_offs[l] >= 0l && t.we2_offs[l] >= 0l)

//! The width of layer `l`'s dense FFN triple on the resident driver: the shared expert's on an
//! MoE layer (0 = the layer has none; gemma-4's parallel dense shared expert is the layer's own FFN
//! triple at the FFN width), the layer's FFN width otherwise.
def resident_dense_width(t : Model; l : int64) : int64 => (layer_is_moe(t, l) && !t.config.moe_dense_shexp) ? t.config.n_ff_shexp : layer_hidden(t, l)

//! The dense planes' width the driver sizes at: the dense triple's, or the expert width where no
//! layer carries a dense triple (nothing dispatches the dense planes then; they only exist).
def private resident_hidden(t : Model) : int64 {
    var w = 0l
    for (l in range64(t.config.n_layers)) {   //! a model's dense width differs by layer (gemma-4 E2B: 6144 below layer 15, 12288 above)
        w = max(w, resident_dense_width(t, l))
    }
    return w > 0l ? w : t.config.n_ff_exp
}

//! the shared expert's plane of `kind` (1 gate, 3 up, 2 down) on layer l: the K-quant plane where the
//! loader kept one, else the q8 transcode the CPU chain reads
def private resident_shexp_plane(t : Model; l : int64; kind : int) : tuple<f : KqFmt; off : int64> {
    let she = t.config.dim * t.config.n_ff_shexp
    if (kind == 1) {
        if (l < long_length(t.wshk1_offs) && t.wshk1_offs[l] >= 0l) return (f = fmt_at(t.wsh1_fmt, l), off = t.wshk1_offs[l])
        return (f = KqFmt.q8, off = t.wsh1_off + l * she)
    }
    if (kind == 3) {
        if (l < long_length(t.wshk3_offs) && t.wshk3_offs[l] >= 0l) return (f = fmt_at(t.wsh3_fmt, l), off = t.wshk3_offs[l])
        return (f = KqFmt.q8, off = t.wsh3_off + l * she)
    }
    if (l < long_length(t.wshk2_offs) && t.wshk2_offs[l] >= 0l) return (f = fmt_at(t.wsh2_fmt, l), off = t.wshk2_offs[l])
    return (f = KqFmt.q8, off = t.wsh2_off + l * she)
}

//! the deltanet geometry of the driver's recurrent layers; the zero record on a model with none
def private rdec_dn_geom(t : Model) : RdecDnGeom {
    let c = t.config
    for (l in range64(c.n_layers)) {
        if (layer_is_recurrent(c, l)) {
            return RdecDnGeom(cd = dn_conv_dim(c), kd = dn_key_dim(c), di = c.ssm_d_inner, nvh = c.ssm_dt_rank,
                              nkh = c.ssm_n_group, ds = c.ssm_d_state, dconv = c.ssm_d_conv, ba_f32 = t.dn_ba_f32)
        }
    }
    return RdecDnGeom()
}

//! the device planes the resident driver places, one row per plane (its row length, row count and format): the plan sums
//! their bytes and the upload reserves and places them from the same list, so the two cannot drift; `ple_tbl` adds the
//! token table the plan admitted
def private resident_planes(t : Model; dng : RdecDnGeom; ple_tbl : bool) : array<tuple<n : int64; rows : int64; f : KqFmt>> {
    let c = t.config
    let dim = c.dim
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    var planes : array<tuple<n : int64; rows : int64; f : KqFmt>>
    planes |> reserve(c.n_layers * 11l + 1l)
    for (l in range64(c.n_layers)) {
        let lf = resident_layer_fmts(t, l)
        if (layer_is_moe(t, l)) {
            planes |> push((n = dim, rows = c.n_expert * c.n_ff_exp, f = rdec_efmt(t, t.we1_fmt, l)))
            planes |> push((n = dim, rows = c.n_expert * c.n_ff_exp, f = rdec_efmt(t, t.we3_fmt, l)))
            planes |> push((n = c.n_ff_exp, rows = c.n_expert * dim, f = rdec_efmt(t, t.we2_fmt, l)))
            if (c.moe_dense_shexp) {
                let hid_l = layer_hidden(t, l)
                planes |> push((n = dim, rows = hid_l, f = lf.f1))
                planes |> push((n = dim, rows = hid_l, f = lf.f3))
                planes |> push((n = hid_l, rows = dim, f = lf.f2))
            } elif (c.n_ff_shexp > 0l) {
                planes |> push((n = dim, rows = c.n_ff_shexp, f = resident_shexp_plane(t, l, 1).f))
                planes |> push((n = dim, rows = c.n_ff_shexp, f = resident_shexp_plane(t, l, 3).f))
                planes |> push((n = c.n_ff_shexp, rows = dim, f = resident_shexp_plane(t, l, 2).f))
            }
        }
        if (layer_is_recurrent(c, l)) {
            planes |> push((n = dim, rows = dng.cd, f = fmt_at(t.dnqkv_fmt, l)))
            planes |> push((n = dim, rows = dng.di, f = fmt_at(t.dngate_fmt, l)))
            planes |> push((n = dng.di, rows = dim, f = fmt_at(t.dnout_fmt, l)))
            if (!dng.ba_f32) {
                planes |> push((n = dim, rows = dng.nvh, f = KqFmt.q8))
                planes |> push((n = dim, rows = dng.nvh, f = KqFmt.q8))
            }
        } else {
            let qd_l = layer_qd(c, l)
            let kvd_l = layer_kv_dim(c, l)
            planes |> push((n = dim, rows = c.q_gated ? 2l * qd_l : qd_l, f = lf.fq))
            if (t.kv_src[l] == l) {
                planes |> push((n = dim, rows = kvd_l, f = lf.fk))
                if (!resident_v_from_k(c, l)) {
                    planes |> push((n = dim, rows = kvd_l, f = lf.fv))
                }
            }
            planes |> push((n = qd_l, rows = dim, f = lf.fo))
        }
        if (!layer_is_moe(t, l)) {
            let hid_l = resident_dense_width(t, l)
            planes |> push((n = dim, rows = hid_l, f = lf.f1))
            planes |> push((n = dim, rows = hid_l, f = lf.f3))
            planes |> push((n = hid_l, rows = dim, f = lf.f2))
        }
        if (has_ple(c)) {   //! the ple gate and proj planes are q8 whatever the file's format
            planes |> push((n = dim, rows = c.n_embd_per_layer, f = KqFmt.q8))
            planes |> push((n = c.n_embd_per_layer, rows = dim, f = KqFmt.q8))
        }
    }
    planes |> push((n = dim, rows = c.vocab_size, f = cls_fmt))
    if (ple_tbl) {
        planes |> push((n = c.n_layers * c.n_embd_per_layer, rows = c.vocab_size, f = KqFmt.q8))
    }
    return <- planes
}

//! the recurrent layers' f32 beta / alpha rows on the f32 arm: device rows beside the planes, no arena block
def private resident_dn_ba_f32_bytes(t : Model) : int64 {
    let c = t.config
    if (!t.dn_ba_f32) {
        return 0l
    }
    var n = 0l
    for (l in range64(c.n_layers)) {
        if (layer_is_recurrent(c, l)) {
            n++
        }
    }
    return n * 2l * c.ssm_dt_rank * c.dim * 4l
}

//! the router planes: every MoE layer's f32 rows, the shared expert's gate row beside them when it is gated, and the f16 mirror the cm2 route's window GEMM reads (counted whatever the route, the plan's other terms run high too)
def private resident_router_bytes(c : Config; n_moe : int64) : int64 => (
    n_moe * (c.n_expert + (c.n_ff_shexp > 0l && c.moe_shexp_gated ? 1l : 0l)) * c.dim * 6l)

// the driver's device allocations past weights + KV — the prefill window buffers dominate.
// An estimate, deliberately a touch high; the flat term carries the decode/batch smalls.
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]
def private rdec_scratch_bytes(t : Model) : int64 {
    let c = t.config
    var qd = 0l
    var kvd = 0l
    var hs = 0l
    for (l in range64(c.n_layers)) {
        continue if (layer_is_recurrent(c, l))
        qd = max(qd, layer_qd(c, l))
        kvd = max(kvd, layer_kv_dim(c, l))
        hs = max(hs, layer_head_size(c, l))
    }
    let hid = resident_hidden(t)
    let wide = max(max(c.dim, qd), hid)
    var moe = 0l
    if (c.n_expert > 0l) {
        let nk = RDEC_PF_ROWS * c.n_expert_used
        let nfe = c.n_ff_exp
        moe = ((nk + TILE_READ_SLACK) * (c.dim + nfe) * 2l + nk * (2l * nfe + c.dim) * 4l
            + RDEC_PF_ROWS * (c.n_expert + 1l) * 4l + nk * 12l)
    }
    let nb = max(g_rdec_regions_want, 1l)   //! the N-row command's planes hold a row a region: the logits and the attention pieces scale with it
    let pieces = hs > 0l ? (qd / hs) * RDEC_ATTN_SPLIT_CAP * (hs + 2l) * 4l : 0l
    return RDEC_PF_ROWS * (16l * c.dim + 2l * wide + 10l * qd + 12l * kvd
        + 8l * hid + 4l * resident_cos_elems(c) + 4l * hs) + (c.vocab_size * 4l + pieces) * nb + RDEC_MISC_BYTES + moe
}

//! The auto arm's headroom where no OS answers: the WDDM share on Windows (the demotion collapse
//! the constant's comment measures), the fixed floor elsewhere - a Linux driver refuses an
//! allocation past the card out loud instead of demoting it.
def plan_headroom(budget_bytes : int64; wddm : bool) : int64 => wddm ? max(RDEC_VRAM_HEADROOM, budget_bytes * RDEC_VRAM_HEADROOM_PCT / 100l) : RDEC_VRAM_HEADROOM

//! The room a plan may fill: the tier's cap less its headroom, capped by the OS's room when the OS
//! answers - its per-process budget less what the other processes hold on the adapter and the reserve.
//! A pinned cap takes no headroom and reads no room.
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]
def plan_room(budget_bytes : int64; pinned : bool; os : tuple<budget : int64; usage : int64; adapter_dedicated : int64; adapter_shared : int64>) : tuple<headroom : int64; usable : int64; others : int64> {
    let answered = !pinned && os.budget > 0l
    let headroom = (pinned || answered) ? 0l : plan_headroom(budget_bytes, get_platform_name() == "windows")
    var usable = budget_bytes - headroom
    var others = 0l
    if (answered) {
        others = max(os.adapter_dedicated - os.usage, 0l)
        usable = min(usable, os.budget - others - RDEC_OS_RESERVE)
    }
    return (headroom = headroom, usable = usable, others = others)
}

def private fill_plan_room(var p : ResidentPlan) {
    let pinned = gpu_want_vram_mb() > 0l
    let room = plan_room(p.budget_bytes, pinned, pinned ? (budget = 0l, usage = 0l, adapter_dedicated = 0l, adapter_shared = 0l) : moe_gpu_os_memory())
    p.headroom_bytes = room.headroom
    p.usable_bytes = room.usable
    p.others_bytes = room.others
}

//! One region's context: the regions share each mirror side's one binding, so `bctx` - the positions that binding holds, 0 = unknown - divides by their count.
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]
def resident_region_ctx(seq_cap, bctx, regions : int64) : int64 => bctx > 0l ? min(seq_cap, bctx / max(regions, 1l)) : seq_cap

//! the positions one mirror side's binding holds under the device's range cap; 0 = no cap known, or no attention layer
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]
def resident_binding_ctx(t : Model; dt : KVDtype; bcap : int64) : int64 {
    let side_row_bytes = resident_kv_row_bytes(t, dt, dt).krow
    return (bcap > 0l && side_row_bytes > 0l) ? bcap / side_row_bytes : 0l
}

//! one position's K and V mirror bytes over the layers that hold rows of their own (a shared-KV layer reads its donor's)
def private resident_kv_row_bytes(t : Model; kdt, vdt : KVDtype) : tuple<krow : int64; vrow : int64> {
    let c = t.config
    var krow = 0l
    var vrow = 0l
    for (l in range64(c.n_layers)) {
        continue if (t.kv_src[l] != l)
        krow += kv_row_bytes(kdt, layer_kv_dim(c, l))
        vrow += kv_row_bytes(vdt, layer_kv_dim(c, l))
    }
    return (krow = krow, vrow = vrow)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]
def resident_plan(t : Model; seq_cap : int64; kdt, vdt : KVDtype) : ResidentPlan {
    let c = t.config
    var p = ResidentPlan(budget_bytes = moe_gpu_weight_budget_())
    if (p.budget_bytes <= 0l) {
        p.reason = "no GPU tier armed"
        return p
    }
    if (c.n_expert > 0l && !rdec_moe_installed()) {
        p.reason = "MoE - the tier installed no MoE seats for the whole-model driver; the per-op offload tier serves routed experts"
        return p
    }
    let (krow, vrow) = resident_kv_row_bytes(t, kdt, vdt)
    p.kv_bytes = g_rdec_regions_want * seq_cap * (krow + vrow)
    p.scratch_bytes = rdec_scratch_bytes(t)
    fill_plan_room(p)
    var n_moe = 0l
    for (l in range64(c.n_layers)) {
        if (layer_is_moe(t, l)) {
            n_moe++
        }
    }
    var inscope planes <- resident_planes(t, rdec_dn_geom(t), false)   //! the list the upload places: the plan's bytes are its planes' bytes
    for (pl in planes) {
        p.weight_bytes += moe_gpu_plane_bytes(pl.n, pl.rows, int(pl.f))
    }
    p.weight_bytes += resident_dn_ba_f32_bytes(t)
    p.weight_bytes += resident_router_bytes(c, n_moe)
    let total = p.weight_bytes + p.kv_bytes + p.scratch_bytes
    let usable = p.usable_bytes
    let emb_want = g_env_vulkan.vk_gpu_embed && rdec_prefill_ids_installed() && !t.cls_q8 && !cls_kq(t) && !t.planes_trimmed
    let emb_bytes = emb_want ? rdec_emb_f32_bytes(c.vocab_size, c.dim) : 0l
    p.emb_f32_bytes = total + emb_bytes <= usable ? emb_bytes : 0l
    p.weight_bytes += p.emb_f32_bytes
    let ple_tbl_want = has_ple(c) && t.ple_emb_fmt == KqFmt.q8 && g_env_vulkan.vk_gpu_embed && rdec_prefill_ids_installed() && rdec_ple_installed()
    let ple_tbl_bytes = ple_tbl_want ? moe_gpu_plane_bytes(c.n_layers * c.n_embd_per_layer, c.vocab_size, int(KqFmt.q8)) : 0l
    p.ple_tbl_bytes = total + p.emb_f32_bytes + ple_tbl_bytes <= usable ? ple_tbl_bytes : 0l
    p.weight_bytes += p.ple_tbl_bytes
    p.fits = total <= usable
    if (!p.fits) {
        p.memory = true
        // a shorter context is the remedy that actually works — weights are fixed, KV is not
        let kv_fit = max(usable - p.weight_bytes - p.scratch_bytes, 0l)
        let cap_fit = (krow + vrow) > 0l ? kv_fit / (krow + vrow) : 0l
        var room = ""
        if (p.others_bytes > 0l) {
            room = " - other processes hold {p.others_bytes >> 20l} MB of the card"
        } elif (p.headroom_bytes > 0l) {
            room = " - the OS reported no room, so {p.headroom_bytes >> 20l} MB stands as headroom; DASLLAMA_GPU_VRAM_MB=<MiB> pins the cap and reads no room"
        }
        p.reason = (cap_fit >= 512l
            ? "needs {total >> 20l} MB of {usable >> 20l} MB at ctx {seq_cap} - would fit at ctx {cap_fit}{room}"
            : "needs {total >> 20l} MB of {usable >> 20l} MB (weights alone {p.weight_bytes >> 20l} MB){room}")
    }
    return p
}

// ===== the resident whole-stack decode driver (dense models that fully fit) =====

var private g_rdec_active = false       // this loaded model runs the resident decode driver
//! the one writer of the flag: the pool's spin window follows the driver's arm and drop
def private rdec_set_active(on : bool) {
    if (g_rdec_active == on) return
    g_rdec_active = on
    set_dispatch_gpu_served(on)
}
var private g_rdec_emb_gpu = false
var private g_rdec_cap = 0l             // a mirror region's context, as the driver armed (<= model ctx when VRAM-capped)
var private g_rdec_gen_seq = 0l

//! One mirror region: a session's whole K/V history on the device.
struct RdecRegion {
    cnt : int64     //!< cached positions (built as the owner prefills and decodes)
    gen : int64     //!< the owning claim's generation; a session whose `rdec_gen` differs owns none of these rows (0 = never claimed)
    used : int64    //!< the tick of its last bind: a session with no region takes the least recently used one
    pinned : bool   //!< a live device-home session owns the rows, which exist nowhere else: no other session takes this region
}

var private g_rdec_regions : array<RdecRegion>
var private g_rdec_tick = 0l
var private g_rdec_regions_want = 1l

//! Ask the resident driver for `n` mirror regions on the models this process loads next: `n` sessions then keep their K/V on the device at once.
def set_gpu_resident_regions_(n : int64) {
    g_rdec_regions_want = max(n, 1l)
}

//! The mirror region `s` prefills and decodes in, or -1 when every region holds a live device-home session and the call must pass.
def private rdec_bind_region(var s : Session) : int64 {
    var r = s.rdec_region
    let n = long_length(g_rdec_regions)
    if (!rdec_owns(s, r)) {
        r = -1l
        for (i in range64(n)) {
            continue if (g_rdec_regions[i].pinned)
            if (r < 0l || g_rdec_regions[i].used < g_rdec_regions[r].used) {
                r = i
            }
        }
        if (r < 0l) {
            rdec_pass_once(RdecPass.busy, n, 0l)
            return -1l
        }
        s.rdec_region = r
    }
    g_rdec_regions[r].used = ++g_rdec_tick
    rdec_select_region(r)
    return r
}

def private rdec_owns(s : Session; r : int64) : bool
    => s.rdec_gen != 0l && r >= 0l && r < long_length(g_rdec_regions) && g_rdec_regions[r].gen == s.rdec_gen

def private rdec_claim(var s : Session; r, cnt : int64) {
    g_rdec_regions[r].cnt = cnt
    g_rdec_regions[r].gen = ++g_rdec_gen_seq
    g_rdec_regions[r].pinned = s.device_kv
    s.rdec_gen = g_rdec_gen_seq
    s.rdec_region = r
}
var private g_rdec_kvdt = KVDtype.f16   // the ARMED mirror codec — sessions must match to be served
var private g_resident_prefill_allowed = true   // multi-stream schedulers pin this off (see set_resident_prefill_allowed_)

//! Allow/forbid the whole-prefill resident override for HOST-CACHED sessions; a device-home session prefills on the device whatever the pin says.
def set_resident_prefill_allowed_(allowed : bool) {
    g_resident_prefill_allowed = allowed
}

// Bulk-hydrate the session's host KV from the live rows of the mirror region it owns — the rail that
// keeps host KV authoritative at rest after device-resident prefills/decodes.
[cold_path]   // the continuation/steal/drop rail, not the steady-state token path
def private rdec_hydrate_host(t : Model; var s : Session) {
    let c = t.config
    rdec_select_region(s.rdec_region)
    let cnt = g_rdec_regions[s.rdec_region].cnt
    for (l in range64(c.n_layers)) {
        continue if (layer_is_recurrent(c, l) || t.kv_src[l] != l)   //! a shared-KV layer reads its donor's slab, gathered once through the donor
        rdec_read_kv_bulk(l, kv_layer_kbase(s, l, c.seq_len), kv_layer_vbase(s, l, c.seq_len), cnt)
    }
    s.rdec_host_current = true
    to_log(LOG_INFO, "dasLLAMA: resident KV hydrated to host ({cnt} positions x {c.n_layers} layers)\n")
}

//! only hydration saves them from a steal, a CPU-rails continuation or a drop (a device-home session has no host cache to fill)
def private rdec_has_device_only_rows(s : Session) : bool {
    return !s.device_kv && rdec_owns(s, s.rdec_region) && g_rdec_regions[s.rdec_region].cnt > 0l && !s.rdec_host_current
}
let private RDEC_MIN_CTX = 4096l        // smallest ctx worth arming a capped mirror at
let private RDEC_ATTN_SPLIT_CAP = 16l   //! the key splits the plan budgets the attention partials for: the most the driver ever picks, whatever the card's SM count

var private g_rdec_ctx_max = 0l   //! the in-process cap on the mirror's positions (0 = none): the test rigs pin 32768, a server keeps the model's context

//! Cap the resident driver's KV mirror at `n` positions for the models this process loads next (0 lifts the pin):
//! the in-process form of DASLLAMA_GPU_CTX_MAX. A program serving long contexts leaves it alone.
def set_gpu_ctx_max_(n : int64) {
    g_rdec_ctx_max = max(n, 0l)
}

// the arming floor: DASLLAMA_GPU_MIN_CTX lowers it for a short-context session (a 3B on an 8GB card fits at 3429), and a
// context pin under it (set_gpu_ctx_max in process, else DASLLAMA_GPU_CTX_MAX) lowers it to the pin, never under 256
def private rdec_min_ctx : int64 {
    let pin = g_rdec_ctx_max > 0l ? g_rdec_ctx_max : g_env_gpu.gpu_ctx_max
    return resident_arm_floor(g_env_gpu.gpu_min_ctx, pin)
}

//! a caller's own context pin (`set_gpu_ctx_max`, DASLLAMA_GPU_CTX_MAX) under the floor wins: the pin IS the shape it serves
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]
def resident_arm_floor(env_floor, pin : int64) : int64 {
    let floor = env_floor > 0l ? max(env_floor, 256l) : RDEC_MIN_CTX
    return pin > 0l && pin < floor ? max(pin, 256l) : floor
}

// gather one prepared plane into device-layout bytes and place it in its format's arena; returns
// the arena block index. Same gather the per-op tier uses (moe_gpu_gather_stack / _kq), then place
// instead of upload_stack.
def private resident_place(t : Model; f : KqFmt; woff : int64; n, rows : int64;
                           mr, kgroup, wbias : int64; slice_rows : int64 = -1l) : int64 {
    var wq : array<uint8>
    var ws : array<uint8>
    let srows = slice_rows > 0l ? slice_rows : rows
    if (f == KqFmt.q8) {
        moe_gpu_gather_stack(t, woff, n, rows, srows, mr, kgroup, wbias, wq, ws)
    } elif (f == KqFmt.q51) {
        moe_gpu_gather_stack_q51(t, woff, n, rows, srows, wq, ws)
    } elif (f == KqFmt.mx4) {
        moe_gpu_gather_stack_mx4(t, woff, n, rows, srows, wq, ws)
    } else {
        let kmr = kq_active_mr(t, f)
        moe_gpu_gather_stack_kq(t, f, woff, n, rows, srows, t.kq_repacked, kmr, wq, ws)
    }
    let blk = rdec_place(wq, ws, n, rows, int(f))
    if (blk >= 0l) {
        vk_bake_collect(int(f), woff, n, rows, wq, ws)
    }
    delete wq
    delete ws
    return blk
}

let private RDEC_PLE_TBL_PIECE_ROWS = 65_536l   //! a token-table piece: 65536 rows of the E4B's 10752 stay under the host gather's 2 GB array

// a plane's activation-quant form: 0 = Q8_0 per-32, 1 = Q8_K superblock
def private rdec_quant_class(f : KqFmt) : int => kq_sb(f) ? 1 : 0

def private rdec_dn_fmt_ok(f : KqFmt; n : int64) : bool => (f == KqFmt.q8 || kq_sb(f)) && kq_rows_ok(n, f)

def private resident_layer_fmts(t : Model; l : int64) : tuple<fq : KqFmt; fk : KqFmt; fv : KqFmt; fo : KqFmt; f1 : KqFmt; f3 : KqFmt; f2 : KqFmt> {
    return (fq = fmt_at(t.wq_fmt, l), fk = fmt_at(t.wk_fmt, l), fv = fmt_at(t.wv_fmt, l),
            fo = fmt_at(t.wo_fmt, l), f1 = fmt_at(t.w1_fmt, l), f3 = fmt_at(t.w3_fmt, l),
            f2 = fmt_at(t.w2_fmt, l))
}

var private g_rdec_decline_reason : string   //! the last decline's text, for a test that skips on a memory decline and fails on any other
var private g_rdec_memory_decline = false

//! The whole-model driver's decline line: the reason, and that the per-op rails serve instead.
[cold_path]
def rdec_decline_say(reason : string; memory : bool = false) {
    g_rdec_decline_reason = reason
    g_rdec_memory_decline = memory
    to_log(LOG_WARNING, "dasLLAMA: resident driver declined - {reason}; the per-op rails serve\n")
}

//! why the whole-model driver does not serve the last loaded model ("" when it armed, or no tier was asked for): its decline, or why it was never attempted
def gpu_resident_decline_() : string => g_rdec_decline_reason

//! true when the last decline was the residency plan not fitting the card - the remedy is a shorter context or a smaller file, not a driver fix
def moe_gpu_resident_memory_decline() : bool => g_rdec_memory_decline

[cold_path]
def private rdec_decline(reason : string) : bool {
    rdec_decline_say(reason)
    moe_gpu_drop_model_state()   //! whatever the upload placed before declining leaves the device: the per-op rails start clean, no later model inherits a half-upload
    return false
}

def private name_list_add(var names : string&; on : bool; name : string) {
    if (on) {
        names = empty(names) ? name : "{names}, {name}"
    }
}

//! The model features the whole-model driver has no arm for, as a comma-separated list; "" when
//! it serves every feature the config carries. The list is the decline reason a user reads.
def resident_unserved_features(c : Config) : string {
    var names = ""
    name_list_add(names, c.attn_qkv_bias && !rdec_bias_installed(), "attention qkv bias")
    name_list_add(names, c.attn_sinks && !rdec_sinks_installed(), "attention sinks")
    name_list_add(names, c.attn_sinks && rdec_sinks_installed() && c.head_size != 64l, "attention sinks at head size {c.head_size} (the sink stamps serve 64)")
    name_list_add(names, c.attn_sinks && rdec_sinks_installed() && c.head_size == 64l && c.head_size_swa != 0l && c.head_size_swa != 64l,
        "attention sinks at head size {c.head_size_swa} on the sliding layers (the sink stamps serve 64)")
    name_list_add(names, c.attn_sinks && rdec_sinks_installed() && (c.q_gated || c.attn_logit_softcap != 0.0),
        "attention sinks under gated attention or a logit softcap (the sink stamps serve the plain flash tile)")
    name_list_add(names, c.moe_dense_shexp && !rdec_moe_installed(), "dense shared expert")
    name_list_add(names, has_ple(c) && !rdec_ple_installed(), "per-layer embeddings")
    name_list_add(names, c.q_gated && !c.qk_norm, "gated attention without qk-norm")
    name_list_add(names, c.rope_dim > 0l && c.rope_dim != c.head_size && !c.qk_norm, "partial rotary ({c.rope_dim} of {c.head_size}) without qk-norm")
    if (c.n_expert > 0l) {
        name_list_add(names, c.moe_gate != MoeGate.softmax && c.moe_gate != MoeGate.softmax_weight, "a {c.moe_gate} router")
        name_list_add(names, c.moe_exp_probs, "an expert selection bias")
        name_list_add(names, c.n_expert > 256l, "{c.n_expert} experts (the select kernels reach 256)")
        name_list_add(names, c.n_expert_used > 64l, "{c.n_expert_used} routed experts (the slot metas hold 64)")
    }
    return names
}

//! The decline reason for a MoE layer's shared expert triple - its formats on the dense rail, one
//! feed form for gate and up, the down plane's block multiple - or "" when the driver serves it.
def private resident_shexp_decline(t : Model; l : int64) : string {
    let c = t.config
    if (c.moe_dense_shexp) {
        let lf = resident_layer_fmts(t, l)
        if (l >= long_length(t.w1_offs) || t.w1_offs[l] < 0l || t.w3_offs[l] < 0l || t.w2_offs[l] < 0l) {
            return "layer {l} lacks its dense shared expert's planes"
        }
        if (!dense_plane_ok(t, lf.f1) || !dense_plane_ok(t, lf.f3) || !dense_plane_ok(t, lf.f2)) {
            return "layer {l}'s dense shared expert carries a format ({int(lf.f1)}/{int(lf.f3)}/{int(lf.f2)}) the dense rail does not serve"
        }
        if (rdec_quant_class(lf.f1) != rdec_quant_class(lf.f3)) {
            return "layer {l}'s dense shared expert mixes activation-quant forms ({int(lf.f1)}/{int(lf.f3)})"
        }
        return ""
    }
    if (c.n_ff_shexp <= 0l) return ""
    let s1 = resident_shexp_plane(t, l, 1).f
    let s3 = resident_shexp_plane(t, l, 3).f
    let s2 = resident_shexp_plane(t, l, 2).f
    if (!dense_plane_ok(t, s1) || !dense_plane_ok(t, s3) || !dense_plane_ok(t, s2)) {
        return "layer {l}'s shared expert carries a format ({int(s1)}/{int(s3)}/{int(s2)}) the dense rail does not serve"
    }
    if (rdec_quant_class(s1) != rdec_quant_class(s3)) {
        return "layer {l}'s shared expert gate and up planes mix activation-quant forms ({int(s1)}/{int(s3)}) - one feed serves both"
    }
    if (!kq_rows_ok(c.n_ff_shexp, s2)) {
        return "layer {l}'s shared width {c.n_ff_shexp} is not a block multiple of its down plane's format {int(s2)}"
    }
    return ""
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-decode-attn-split")]
//! Why layer `l` keeps the whole-model driver off the model - "" when the driver serves the layer's
//! shape (its planes placed, its own K/V, a head the attention kernels stage).
def resident_layer_decline(t : Model; l : int64) : string {
    let c = t.config
    if (layer_is_moe(t, l)) {
        let fe1 = rdec_efmt(t, t.we1_fmt, l)
        let fe3 = rdec_efmt(t, t.we3_fmt, l)
        let fe2 = rdec_efmt(t, t.we2_fmt, l)
        if (c.moe_exps_bias && kq_sb(fe2)) {
            return "layer {l}'s biased expert stacks feed a K-quant down plane ({int(fe2)}) - the biased act has no Q8_K form"
        }
        if (!rdec_moe_ok(int(fe1), int(fe3), int(fe2))) {
            return "layer {l}'s expert formats {int(fe1)}/{int(fe3)}/{int(fe2)} are outside the resident MoE block's tile family (the f16-fed cm2 tiles, or the KHR tile in mm mode)"
        }
        if (kq_sb(fe1) != kq_sb(fe3)) {
            return "layer {l}'s expert gate and up stacks mix activation-quant forms ({int(fe1)}/{int(fe3)}) - one feed serves both"
        }
        if (!kq_rows_ok(c.n_ff_exp, fe2)) {
            return "layer {l}'s expert width {c.n_ff_exp} is not a block multiple of its down plane's format {int(fe2)}"
        }
        let shexp_why = resident_shexp_decline(t, l)
        if (!empty(shexp_why)) return shexp_why
    } elif (c.n_expert > 0l && l >= c.n_layer_dense_lead) {
        return "layer {l} lacks an expert plane"
    }
    if (layer_is_recurrent(c, l)) {
        if (l >= long_length(t.dnqkv_offs) || t.dnqkv_offs[l] < 0l || t.dngate_offs[l] < 0l || t.dnout_offs[l] < 0l) {
            return "layer {l} lacks a deltanet plane"
        }
        let fqkv = fmt_at(t.dnqkv_fmt, l)
        let fz = fmt_at(t.dngate_fmt, l)
        if (!rdec_dn_fmt_ok(fqkv, c.dim) || !rdec_dn_fmt_ok(fz, c.dim)) {
            return "layer {l}'s deltanet qkv/z planes carry a format ({int(fqkv)}/{int(fz)}) the resident GEMVs do not serve at width {c.dim}"
        }
        if (rdec_quant_class(fqkv) != rdec_quant_class(fz)) {
            return "layer {l}'s deltanet qkv and z planes mix activation-quant forms ({int(fqkv)}/{int(fz)}) - one x feed serves both"
        }
        let fout = fmt_at(t.dnout_fmt, l)
        if (!rdec_dn_fmt_ok(fout, c.ssm_d_inner)) {
            return "layer {l}'s deltanet out plane carries a format ({int(fout)}) the resident GEMVs do not serve at width {c.ssm_d_inner}"
        }
        if (!t.dn_ba_f32 && (t.dnbeta_offs[l] < 0l || t.dnalpha_offs[l] < 0l)) return "layer {l} lacks its beta/alpha planes"
        if (!rdec_dn_installed()) return "layer {l} is recurrent and the tier installed no deltanet seats for the whole-model driver"
        return ""
    }
    let kvsh = t.kv_src[l] != l   //! a shared-KV layer projects Q only and reads its donor's rows (the donor sits below it)
    if (kvsh && t.kv_src[l] > l) return "layer {l} shares layer {t.kv_src[l]}'s K/V, a layer above it"
    if (t.wq_offs[l] < 0l || (!kvsh && t.wk_offs[l] < 0l)) return "layer {l} lacks a q/k plane"
    if (!kvsh && t.wv_offs[l] < 0l && !resident_v_from_k(c, l)) return "layer {l} lacks a v plane"
    let hs = layer_head_size(c, l)
    if (hs % 32l != 0l || (hs > 256l && hs != 512l)) return "layer {l} head size {hs} (the attention kernels take 32-multiples up to 256, and 512)"
    return ""
}

//! layer `l` has no v plane and takes V off its k row (gemma-4's global layers)
def resident_v_from_k(c : Config; l : int64) : bool => l < 64l && ((c.v_from_k_mask >> uint64(l)) & 1ul) != 0ul

def private resident_upload_dn_ba(t : Model; n_recurrent : int64) {
    let c = t.config
    let dim = c.dim
    let nvh = c.ssm_dt_rank
    var rows : array<float>
    rows |> reserve_resize(n_recurrent * 2l * nvh * dim)
    var at = 0l
    for (l in range64(c.n_layers)) {
        continue if (!layer_is_recurrent(c, l))
        copy_floats(t.fblob, t.dn_betaf_off + l * dim * nvh, rows, at, nvh * dim)
        at += nvh * dim
        copy_floats(t.fblob, t.dn_alphaf_off + l * dim * nvh, rows, at, nvh * dim)
        at += nvh * dim
    }
    rdec_upload_dn_ba(rows)
    delete rows
}

def private resident_bias_off(c : Config; l : int64) : int64 {
    var off = 0l
    for (i in range64(l)) {
        off += layer_qd(c, i) + 2l * layer_kv_dim(c, i)
    }
    return off
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-attn-planes")]
def private resident_upload_bias(t : Model) {
    let c = t.config
    var rows : array<float>
    rows |> reserve_resize(resident_bias_off(c, c.n_layers))
    for (l in range64(c.n_layers)) {
        continue if (layer_is_recurrent(c, l))
        let qd = layer_qd(c, l)
        let kvd = layer_kv_dim(c, l)
        let off = resident_bias_off(c, l)
        copy_floats(t.bq, l * qd, rows, off, qd)
        copy_floats(t.bk, l * kvd, rows, off + qd, kvd)
        copy_floats(t.bv, l * kvd, rows, off + qd + kvd, kvd)
    }
    rdec_upload_bias(rows)
    delete rows
}

def private resident_upload_sinks(t : Model) : bool {
    let c = t.config
    var rows : array<float>
    rows |> reserve_resize(c.n_layers * c.n_heads)
    copy_floats(t.fblob, t.sinks_off, rows, 0l, c.n_layers * c.n_heads)
    let ok = rdec_upload_sinks(rows)
    delete rows
    return ok
}

//! the global rope class's rotary width: the config's head size (the global class's), or rope_dim on a partial-rope model
def private resident_rot(c : Config) : int64 => c.rope_dim > 0l ? c.rope_dim : c.head_size

//! the sliding class's rotary width: its own head size (a partial-rope model has one class)
def private resident_rot_swa(c : Config) : int64 => c.head_size_swa > 0l ? c.head_size_swa : c.head_size

//! a position's cos/sin plane for the resident kernels: the global class's row (cos[half] + sin[half]),
//! then the sliding class's where the two rope differently (`has_dual_rope`); a layer reads its class's row at its `cbase`
def private resident_cos_elems(c : Config) : int64 => resident_rot(c) + (has_dual_rope(c) ? resident_rot_swa(c) : 0l)

//! one attention layer's geometry record for the driver; `mir_base` is the caller's running mirror element count
def private resident_layer_geom(t : Model; l, mir_base : int64) : RdecLayerGeom {
    let c = t.config
    let sliding = layer_is_sliding(c, l)
    let swa_row = sliding && has_dual_rope(c)
    let vfk = resident_v_from_k(c, l)
    return RdecLayerGeom(hs = layer_head_size(c, l), qd = layer_qd(c, l), kvd = layer_kv_dim(c, l),
        hid = resident_dense_width(t, l), rot = swa_row ? resident_rot_swa(c) : resident_rot(c),
        window = sliding ? c.sliding_window : 0l, mir_base = mir_base,
        cbase = swa_row ? resident_rot(c) : 0l, boff = resident_bias_off(c, l),
        pre_post = c.pre_post_norm, gelu = c.ffn_act == FfnAct.gelu,
        vfk = vfk, vnorm = c.v_norm && !vfk,
        out_scale = c.layer_out_scale ? t.fblob[t.out_scale_off + l] : 1.0,
        kv_shared = t.kv_src[l] != l, attn_cap = c.attn_logit_softcap,
        moe_g4 = c.moe_dense_shexp && layer_is_moe(t, l))
}

struct private ImageRow {
    name : string
    plane_count : int64
    file_bytes : int64
    device_bytes : int64
}

//! The device-side twin of the load report: every plane the whole-model driver uploads, per
//! format, as bytes in the file's own blocks against bytes in the device planes, biggest overhead
//! first, then the KV mirror and scratch the plan adds at this context.
def private log_resident_image(planes : array<tuple<n : int64; rows : int64; f : KqFmt>>; plan : ResidentPlan; seq_cap : int64) {
    var rows : table<int; ImageRow>
    var file_total = 0l
    var device_total = 0l
    for (pl in planes) {
        let fi = int(pl.f)
        var r = rows[fi]
        r.name = "{pl.f}"
        r.plane_count++
        r.file_bytes += kq_disk_bytes(pl.f, pl.n * pl.rows)
        r.device_bytes += moe_gpu_plane_bytes(pl.n, pl.rows, fi)
        rows[fi] = r
        file_total += kq_disk_bytes(pl.f, pl.n * pl.rows)
        device_total += moe_gpu_plane_bytes(pl.n, pl.rows, fi)
    }
    var sorted : array<ImageRow>
    for (v in values(rows)) {
        sorted |> emplace(ImageRow(name = clone_string(v.name), plane_count = v.plane_count, file_bytes = v.file_bytes, device_bytes = v.device_bytes))
    }
    sorted |> sort() $(a, b) => a.device_bytes - a.file_bytes > b.device_bytes - b.file_bytes
    to_log(LOG_INFO, "dasLLAMA: resident image: {file_total >> 20l} MB in the file's blocks -> {device_total >> 20l} MB on device ({mb_delta(device_total, file_total)}) over {long_length(planes)} planes; KV mirror {plan.kv_bytes >> 20l} MB at ctx {seq_cap}, scratch {plan.scratch_bytes >> 20l} MB\n")
    for (r in sorted) {
        to_log(LOG_INFO, "dasLLAMA: resident image:   {r.name} x{r.plane_count}: {r.file_bytes >> 20l} MB -> {r.device_bytes >> 20l} MB ({mb_delta(r.device_bytes, r.file_bytes)})\n")
    }
    delete sorted
    delete rows
}

//! Reserve arenas + place every dense plane + wire the resident decode driver. Returns false (and
//! leaves the driver unarmed) if the model is not a servable dense shape or does not fit at seq_cap;
//! every decline logs its reason through `rdec_decline`.
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def resident_upload(t : Model; seq_cap : int64; rkdt : KVDtype; plan : ResidentPlan) : bool {   // nolint:STYLE037,STYLE038 — all-or-nothing arm: one reject per unsupported family feature, no seam
    rdec_set_active(false)
    g_rdec_emb_gpu = false
    rdec_ple_proj_set(false)
    g_rdec_pass_said |> clear()
    set_gpu_cpu_pass_reasons([for (k in type<RdecPass>); "{k}"], [for (k in type<RdecPass>); rdec_pass_words(k)])
    vulkan_bake_role(VkBakeRole.arena)
    if (!moe_gpu_resident_installed()) return rdec_decline("the tier installed no resident-driver seams")
    if (t.quant != QuantMode.q8) return rdec_decline("weights are not in the q8 serving mode ({t.quant})")
    if (t.config.n_expert > 0l && !rdec_moe_installed()) return rdec_decline("routed experts ({t.config.n_expert}) - the tier installed no MoE seats; the per-op tier's expert rails serve")
    // the driver is all-or-nothing across prefill AND decode (the KV lives in the device mirror
    // only), so a pin away from it declines BEFORE any upload — the per-op rails take over
    let pin = g_env_engine.pin_prefill
    if (!empty(pin) && pin != "vulkan") return rdec_decline("prefill is pinned to '{pin}' (DASLLAMA_PIN_PREFILL)")
    if (!empty(active_prefill_override())) return rdec_decline("the '{active_prefill_override()}' prefill override is active")
    let c = t.config
    let feats = resident_unserved_features(c)
    if (!empty(feats)) return rdec_decline("no arm for: {feats}")
    var n_attn = 0l
    var l_attn0 = -1l
    for (l in range64(c.n_layers)) {
        if (!layer_is_recurrent(c, l)) {
            n_attn++
            if (l_attn0 < 0l) {
                l_attn0 = l
            }
        }
    }
    if (n_attn == 0l) return rdec_decline("no attention layer (the whole-model driver serves attention or hybrid stacks)")
    let n_recurrent = c.n_layers - n_attn
    let dim = c.dim
    let neox = c.rope_neox
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    // the class maxima size the driver's scratch; each layer's own geometry rides its registration
    var hs = 0l
    var qd = 0l
    var kvd = 0l
    var mir_elems = 0l
    for (l in range64(c.n_layers)) {
        let why = resident_layer_decline(t, l)
        if (!empty(why)) return rdec_decline(why)
        continue if (layer_is_recurrent(c, l))
        hs = max(hs, layer_head_size(c, l))
        qd = max(qd, layer_qd(c, l))
        kvd = max(kvd, layer_kv_dim(c, l))
        if (t.kv_src[l] == l) {
            mir_elems += seq_cap * layer_kv_dim(c, l)
        }
    }
    let cos_elems = resident_cos_elems(c)
    let dng = rdec_dn_geom(t)
    // tally arena blocks per format
    var need : table<int; int64>
    var planes <- resident_planes(t, dng, plan.ple_tbl_bytes > 0l)
    var n_moe = 0l
    for (l in range64(c.n_layers)) {
        if (layer_is_moe(t, l)) {
            n_moe++
        }
    }
    for (pl in planes) {
        if (!kq_rows_ok(pl.n, pl.f)) {   // unplaceable row: the whole driver declines (all-or-nothing)
            let why = "a fmt {int(pl.f)} plane's row length {pl.n} is not a block multiple"   // pl refers into planes: read it before the delete
            delete planes
            delete need
            return rdec_decline(why)
        }
        let fi = int(pl.f)
        need[fi] = (need?[fi] ?? 0l) + kq_blocks_for(pl.n, pl.rows, pl.f)
    }
    log_resident_image(planes, plan, seq_cap)
    delete planes
    var reserve_ok = true
    for (fmt, blocks in keys(need), values(need)) {
        if (!rdec_reserve(fmt, blocks)) {
            to_log(LOG_WARNING, "dasLLAMA: resident driver - arena reserve failed (fmt {fmt}, {blocks} blocks)\n")
            reserve_ok = false
        }
    }
    delete need
    if (!reserve_ok) return rdec_decline("an arena reserve failed (the formats above)")
    let repacked = kernel_backend_needs_repack(active_kernel_backend())
    let mr = repacked ? active_q8_layout_mr() : 1l
    let kg = repacked ? active_q8_kgroup() : 4l
    let wb = repacked ? active_q8_wbias() : 0l
    let rot = resident_rot(c)
    if (!rdec_prepare(c.n_layers, n_attn, dim, qd, kvd, hs, c.n_heads, resident_hidden(t), c.vocab_size, seq_cap, neox,
                      c.norm_eps, c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(hs)), c.qk_norm,
                      rkdt == KVDtype.f16, c.q_gated, rot, dng, mir_elems, cos_elems, g_rdec_regions_want)) {
        return rdec_decline("the device prepare declined (a kernel class or the mirror range - the tier's line above)")
    }
    rdec_note_weight_bytes(plan.weight_bytes)
    let sh_gated = c.n_ff_shexp > 0l && c.moe_shexp_gated
    if (n_moe > 0l && !resident_prepare_moe(t, n_moe, sh_gated)) {
        return rdec_decline("the MoE seats declined (the tier's line above)")
    }
    if (n_recurrent > 0l && dng.ba_f32) {
        resident_upload_dn_ba(t, n_recurrent)
    }
    if (c.attn_qkv_bias) {
        resident_upload_bias(t)
    }
    if (c.attn_sinks && !resident_upload_sinks(t)) {
        return rdec_decline("the attention sinks found no flash arm on the device (the tier's line above)")
    }
    // place + wire each layer. One quantized X image feeds each consumer group (qkv shares one,
    // gate/up another), so a group must agree on its quant form — mixed groups decline
    var mir_idx = 0l
    var mir_base = 0l
    var rec_idx = 0l
    // per layer: its mirror base and slot (a shared-KV layer records its donor's), for the layers above it that share
    var mir_bases : array<int64>
    var mir_slots : array<int64>
    mir_bases |> reserve(c.n_layers)
    mir_bases |> resize(c.n_layers)
    mir_slots |> reserve(c.n_layers)
    mir_slots |> resize(c.n_layers)
    let ple = has_ple(c) ? c.n_embd_per_layer : 0l
    if (ple > 0l && !rdec_set_ple(ple)) return rdec_decline("the per-layer-embedding width {ple} is not a 32-multiple")
    var ple_tbl_placed = false
    if (plan.ple_tbl_bytes > 0l) {
        let all = c.n_layers * ple
        var bpt0 = -1l
        var contiguous = true
        var row = 0l
        while (row < c.vocab_size && contiguous) {
            let prows = min(RDEC_PLE_TBL_PIECE_ROWS, c.vocab_size - row)
            let bpt = resident_place(t, KqFmt.q8, t.ple_emb_off + row * all, all, prows, 1l, 4l, 0l)   //! the table sits linear in the q8 plane, outside the weights' repack
            if (row == 0l) {
                bpt0 = bpt
            }
            contiguous = bpt >= 0l && bpt == bpt0 + kq_blocks_for(all, row, KqFmt.q8)
            row += prows
        }
        ple_tbl_placed = contiguous && rdec_set_ple_table(bpt0)
        to_log(LOG_INFO, "dasLLAMA: resident driver - the per-layer-embedding token table {ple_tbl_placed ? "gathers on device" : "stays on the CPU (the placement or the gather rail declined)"}\n")
    }
    var router_slot = 0l
    for (l in range64(c.n_layers)) {
        var lf = resident_layer_fmts(t, l)
        let moe = layer_is_moe(t, l)
        let hid_l = resident_dense_width(t, l)
        var b1 = -1l
        var b3 = -1l
        var b2 = -1l
        if (moe) {
            //! a layer with no shared expert keeps -1 dense blocks and takes the experts' feed form
            if (c.moe_dense_shexp) {
                if (rdec_quant_class(lf.f1) != rdec_quant_class(lf.f3)) {
                    return rdec_decline("layer {l} mixes activation-quant forms (the dense shared expert {int(lf.f1)}/{int(lf.f3)})")
                }
                b1 = resident_place(t, lf.f1, t.w1_offs[l], dim, hid_l, mr, kg, wb)
                b3 = resident_place(t, lf.f3, t.w3_offs[l], dim, hid_l, mr, kg, wb)
                b2 = resident_place(t, lf.f2, t.w2_offs[l], hid_l, dim, mr, kg, wb)
                if (b1 < 0l || b3 < 0l || b2 < 0l) return rdec_decline("arena placement failed at layer {l} (the dense shared expert)")
            } elif (c.n_ff_shexp > 0l) {
                let p1 = resident_shexp_plane(t, l, 1)
                let p3 = resident_shexp_plane(t, l, 3)
                let p2 = resident_shexp_plane(t, l, 2)
                lf.f1 = p1.f
                lf.f3 = p3.f
                lf.f2 = p2.f
                b1 = resident_place(t, p1.f, p1.off, dim, c.n_ff_shexp, mr, kg, wb)
                b3 = resident_place(t, p3.f, p3.off, dim, c.n_ff_shexp, mr, kg, wb)
                b2 = resident_place(t, p2.f, p2.off, c.n_ff_shexp, dim, mr, kg, wb)
                if (b1 < 0l || b3 < 0l || b2 < 0l) return rdec_decline("arena placement failed at layer {l} (the shared expert)")
            } else {
                lf.f1 = rdec_efmt(t, t.we1_fmt, l)
                lf.f3 = rdec_efmt(t, t.we3_fmt, l)
                lf.f2 = rdec_efmt(t, t.we2_fmt, l)
            }
        } else {
            if (rdec_quant_class(lf.f1) != rdec_quant_class(lf.f3)) {
                return rdec_decline("layer {l} mixes activation-quant forms (ffn {int(lf.f1)}/{int(lf.f3)})")
            }
            b1 = resident_place(t, lf.f1, t.w1_offs[l], dim, hid_l, mr, kg, wb)
            b3 = resident_place(t, lf.f3, t.w3_offs[l], dim, hid_l, mr, kg, wb)
            b2 = resident_place(t, lf.f2, t.w2_offs[l], hid_l, dim, mr, kg, wb)
            if (b1 < 0l || b3 < 0l || b2 < 0l) return rdec_decline("arena placement failed at layer {l}")
        }
        if (layer_is_recurrent(c, l)) {
            let fqkv = fmt_at(t.dnqkv_fmt, l)
            let fz = fmt_at(t.dngate_fmt, l)
            let fout = fmt_at(t.dnout_fmt, l)
            let bqkv = resident_place(t, fqkv, t.dnqkv_offs[l], dim, dng.cd, mr, kg, wb)
            let bz = resident_place(t, fz, t.dngate_offs[l], dim, dng.di, mr, kg, wb)
            let bout = resident_place(t, fout, t.dnout_offs[l], dng.di, dim, mr, kg, wb)
            let bbeta = dng.ba_f32 ? -1l : resident_place(t, KqFmt.q8, t.dnbeta_offs[l], dim, dng.nvh, mr, kg, wb)
            let balpha = dng.ba_f32 ? -1l : resident_place(t, KqFmt.q8, t.dnalpha_offs[l], dim, dng.nvh, mr, kg, wb)
            if (bqkv < 0l || bz < 0l || bout < 0l || (!dng.ba_f32 && (bbeta < 0l || balpha < 0l))) {
                return rdec_decline("arena placement failed at recurrent layer {l}")
            }
            unsafe {
                rdec_set_dn_layer(l, bqkv, bz, bout, bbeta, balpha, b1, b3, b2, rec_idx * 2l * dng.nvh * dim,
                    int(fqkv), int(fz), int(fout), int(lf.f1), int(lf.f3), int(lf.f2),
                    addr<float const?>(t.fblob[t.dn_conv_off + l * c.ssm_d_conv * dng.cd]),
                    addr<float const?>(t.fblob[t.dn_norm_off + l * dng.ds]),
                    addr<float const?>(t.fblob[t.dn_a_off + l * dng.nvh]),
                    addr<float const?>(t.fblob[t.dn_dt_off + l * dng.nvh]))
            }
            rec_idx++
            if (moe) {
                let eb = resident_place_experts(t, l, mr, kg, wb)
                if (eb.be1 < 0l || eb.be3 < 0l || eb.be2 < 0l) return rdec_decline("arena placement failed at recurrent layer {l} (the expert stacks)")
                rdec_set_moe_experts(l, router_slot, eb.be1, eb.be3, eb.be2, int(rdec_efmt(t, t.we1_fmt, l)), int(rdec_efmt(t, t.we3_fmt, l)), int(rdec_efmt(t, t.we2_fmt, l)))
                router_slot++
            }
            continue
        }
        //! a shared-KV layer attends over its donor's mirror rows: its record carries the donor's base and no mirror slot of its own
        let kvsh = t.kv_src[l] != l
        let g = resident_layer_geom(t, l, kvsh ? mir_bases[t.kv_src[l]] : mir_base)
        mir_bases[l] = g.mir_base
        mir_slots[l] = kvsh ? mir_slots[t.kv_src[l]] : mir_idx
        let cq = rdec_quant_class(lf.fq)
        if (!kvsh && (cq != rdec_quant_class(lf.fk) || (!g.vfk && cq != rdec_quant_class(lf.fv)))) {
            return rdec_decline("layer {l} mixes activation-quant forms (qkv {int(lf.fq)}/{int(lf.fk)}/{int(lf.fv)})")
        }
        let bq = resident_place(t, lf.fq, t.wq_offs[l], dim, c.q_gated ? 2l * g.qd : g.qd, mr, kg, wb)
        let bk = kvsh ? -1l : resident_place(t, lf.fk, t.wk_offs[l], dim, g.kvd, mr, kg, wb)
        let bv = (g.vfk || kvsh) ? -1l : resident_place(t, lf.fv, t.wv_offs[l], dim, g.kvd, mr, kg, wb)
        let bo = resident_place(t, lf.fo, t.wo_offs[l], g.qd, dim, mr, kg, wb)
        // a placement can fail (arena exhausted); fail closed to the CPU loop rather than wire a -1 block
        if (bq < 0l || (!kvsh && bk < 0l) || (!g.vfk && !kvsh && bv < 0l) || bo < 0l) return rdec_decline("arena placement failed at layer {l}")
        if (moe) {
            let eb = resident_place_experts(t, l, mr, kg, wb)
            if (eb.be1 < 0l || eb.be3 < 0l || eb.be2 < 0l) return rdec_decline("arena placement failed at layer {l} (the expert stacks)")
            rdec_set_moe_layer(l, mir_idx, router_slot, bq, bk, bv, bo, b1, b3, b2, eb.be1, eb.be3, eb.be2,
                int(lf.fq), int(lf.fk), int(lf.fv), int(lf.fo), int(lf.f1), int(lf.f3), int(lf.f2),
                int(rdec_efmt(t, t.we1_fmt, l)), int(rdec_efmt(t, t.we3_fmt, l)), int(rdec_efmt(t, t.we2_fmt, l)), g)
            router_slot++
        } else {
            rdec_set_layer(l, mir_slots[l], bq, bk, bv, bo, b1, b3, b2,
                int(lf.fq), int(lf.fk), int(lf.fv), int(lf.fo), int(lf.f1), int(lf.f3), int(lf.f2), g)
        }
        if (ple > 0l) {
            let bpg = resident_place(t, KqFmt.q8, t.ple_gate_off + l * dim * ple, dim, ple, mr, kg, wb)
            let bpp = resident_place(t, KqFmt.q8, t.ple_proj_off + l * ple * dim, ple, dim, mr, kg, wb)
            if (bpg < 0l || bpp < 0l) return rdec_decline("arena placement failed at layer {l} (the per-layer-embedding planes)")
            rdec_set_ple_layer(l, bpg, bpp, int(KqFmt.q8))
        }
        if (!kvsh) {
            mir_idx++
            mir_base += seq_cap * g.kvd
        }
    }
    if (ple > 0l) {   //! the pre-step's projection on device where the driver takes it (the cm2 f16 GEMM, or the KHR twin off cm2); the CPU pre-step serves otherwise
        let all = c.n_layers * ple
        var w : array<float>
        w |> reserve(all * dim)   // E4B: 27.5M floats - the guard wants the final size reserved first
        w |> resize(all * dim)
        if (t.ple_model_bf16) {
            for (i in range64(all * dim)) {
                w[i] = unsafe(reinterpret<float>(uint(t.bf16blob[t.ple_model_off + i]) << 16u))
            }
        } else {
            for (i in range64(all * dim)) {
                w[i] = t.fblob[t.ple_model_off + i]
            }
        }
        var pnorm <- [for (i in range64(ple)); t.fblob[t.ple_pnorm_off + i]]
        let on = rdec_set_ple_proj(w, pnorm, all)
        to_log(LOG_INFO, "dasLLAMA: resident driver: the per-layer-embedding pre-step's projection {on ? "runs on device" : "stays on the CPU"}\n")
        delete w
        delete pnorm
    }
    let bcls = resident_place(t, cls_fmt, t.wcls_off, dim, c.vocab_size, mr, kg, wb)
    if (bcls < 0l) return rdec_decline("classifier placement failed")
    rdec_set_cls(bcls, int(cls_fmt), c.final_logit_softcap, t.suppress)
    //! a per-layer-embedding model keeps the CPU embed unless its token table sits on the device too: the pre-step wants both gathers in one place
    let emb_q8_want = (g_env_vulkan.vk_gpu_embed && rdec_prefill_ids_installed() && (!has_ple(c) || (ple_tbl_placed && rdec_ple_proj_on()))
        && c.shared_weights && t.cls_q8 && cls_fmt == KqFmt.q8)
    let emb_f32_want = (g_env_vulkan.vk_gpu_embed && rdec_prefill_ids_installed() && (!has_ple(c) || (ple_tbl_placed && rdec_ple_proj_on()))
        && !t.emb_q8 && !cls_kq(t) && !t.planes_trimmed   // an untied q8 table has no f32 copy to upload: the CPU embed loop serves it
        && plan.emb_f32_bytes > 0l)   // the plan counted it at this context
    var emb_q8_placed = false
    var emb_f32_placed = false
    if (emb_q8_want) {
        emb_q8_placed = rdec_set_emb(bcls)
        if (emb_q8_placed) {
            to_log(LOG_INFO, "dasLLAMA: resident driver - embed gather on device (tied cls plane; DASLLAMA_VK_GPU_EMBED=0 keeps the CPU embed)\n")
        } else {
            to_log(LOG_WARNING, "dasLLAMA: resident driver - the embed-gather rail declined, the CPU embed serves\n")
        }
    } elif (emb_f32_want) {
        emb_f32_placed = rdec_upload_emb_f32(t.fblob, t.tok_emb_off, c.vocab_size, dim)
        if (emb_f32_placed) {
            to_log(LOG_INFO, "dasLLAMA: resident driver - embed gather on device (f32 plane; DASLLAMA_VK_GPU_EMBED=0 keeps the CPU embed)\n")
        } else {
            to_log(LOG_WARNING, "dasLLAMA: resident driver - the f32 embed table stays on the CPU (over the cap, the budget, or the device range)\n")
        }
    }
    g_rdec_emb_gpu = emb_q8_placed || emb_f32_placed
    //! past the layers' RDEC_NORM_ROWS rows: rms_final, then qk_norm's per-head [rms_q, rms_k] x L at the widest class's stride
    var norms : array<float>
    let norms_len = c.n_layers * RDEC_NORM_ROWS * dim + dim + (c.qk_norm ? c.n_layers * 2l * hs : 0l)
    norms |> reserve(norms_len)
    norms |> resize(norms_len)
    for (l in range64(c.n_layers)) {
        let lb = l * RDEC_NORM_ROWS * dim
        for (i in range64(dim)) {
            norms[lb + i] = t.fblob[t.rms_att_off + l * dim + i]
            norms[lb + dim + i] = t.fblob[t.rms_ffn_off + l * dim + i]
        }
        if (c.pre_post_norm) {
            for (i in range64(dim)) {
                norms[lb + 2l * dim + i] = t.fblob[t.rms_post_att_off + l * dim + i]
                norms[lb + 3l * dim + i] = t.fblob[t.rms_post_ffn_off + l * dim + i]
            }
        }
        if (has_ple(c)) {
            for (i in range64(dim)) {
                norms[lb + 4l * dim + i] = t.fblob[t.ple_post_off + l * dim + i]
            }
        }
        if (c.moe_dense_shexp && layer_is_moe(t, l)) {   //! the router's learned input scale rides the norms plane as a weight row, over sqrt(dim)
            let inv_d = 1.0 / sqrt(float(dim))
            for (i in range64(dim)) {
                norms[lb + 5l * dim + i] = t.fblob[t.moe_router_scale_off + l * dim + i] * inv_d
                norms[lb + 6l * dim + i] = t.fblob[t.rms_pre_ffn2_off + l * dim + i]
                norms[lb + 7l * dim + i] = t.fblob[t.rms_post_ffn1_off + l * dim + i]
                norms[lb + 8l * dim + i] = t.fblob[t.rms_post_ffn2_off + l * dim + i]
            }
        }
        if (c.attn_out_bias) {   //! the attention output bias rides the norms plane as a row the residual step adds to the wo GEMV's row
            for (i in range64(dim)) {
                norms[lb + 9l * dim + i] = t.bo[l * dim + i]
            }
        }
    }
    for (i in range64(dim)) {
        norms[c.n_layers * RDEC_NORM_ROWS * dim + i] = t.fblob[t.rms_final_off + i]
    }
    if (c.qk_norm) {
        let qkw = c.n_layers * RDEC_NORM_ROWS * dim + dim
        for (l in range64(c.n_layers)) {
            continue if (layer_is_recurrent(c, l))   // no q/k norm rows on a recurrent layer; its slots stay zero
            for (i in range64(layer_head_size(c, l))) {
                norms[qkw + l * 2l * hs + i] = t.fblob[t.rms_q_offs[l] + i]
                if (t.kv_src[l] == l) {   //! a shared-KV layer norms no K of its own; its k row stays zero
                    norms[qkw + (l * 2l + 1l) * hs + i] = t.fblob[t.rms_k_offs[l] + i]
                }
            }
        }
    }
    rdec_upload_norms(norms, c.attn_out_bias)
    delete norms
    rdec_set_active(true)
    g_rdec_decline_reason = ""
    g_rdec_memory_decline = false
    g_rdec_regions |> clear()
    g_rdec_regions |> resize(g_rdec_regions_want)
    g_rdec_cap = seq_cap
    g_rdec_cos_elems = cos_elems
    g_rdec_kvdt = rkdt
    let kvname = rkdt == KVDtype.f16 ? "f16" : "f32"
    // prefill + decode select together — the device prefill fills the mirror the decode reads
    select_prefill_override("vulkan")
    select_decode_override("vulkan")
    let regions = g_rdec_regions_want > 1l ? " x {g_rdec_regions_want} regions (set_gpu_resident_regions in process: each region takes a share of the mirror's context)" : ""
    to_log(LOG_INFO, "dasLLAMA: resident driver armed ({c.n_layers} layers, dim {dim}, ctx {seq_cap}{regions}, kv {kvname}, device prefill + decode)\n")
    return true
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def private resident_prepare_moe(t : Model; n_moe : int64; sh_gated : bool) : bool {
    let c = t.config
    let dim = c.dim
    let ne = c.n_expert
    let ner = ne + (sh_gated ? 1l : 0l)
    var router : array<float>
    router |> reserve(n_moe * ner * dim)
    router |> resize(n_moe * ner * dim)
    var li = 0l
    for (l in range64(c.n_layers)) {
        continue if (!layer_is_moe(t, l))
        unsafe {
            memcpy(addr<void?>(router[li * ner * dim]), addr<void?>(t.fblob[t.router_off + l * ne * dim]), ne * dim * 4l)
            if (sh_gated) {
                memcpy(addr<void?>(router[li * ner * dim + ne * dim]), addr<void?>(t.fblob[t.shexp_gate_off + l * dim]), dim * 4l)
            }
        }
        li++
    }
    var dscale : array<float>
    if (c.moe_dense_shexp) {
        dscale |> reserve_resize(n_moe * ne)
        var di = 0l
        for (l in range64(c.n_layers)) {
            continue if (!layer_is_moe(t, l))
            copy_floats(t.fblob, t.moe_down_scale_off + l * ne, dscale, di * ne, ne)
            di++
        }
    }
    var rbias : array<float>
    var gbias : array<float>
    var ubias : array<float>
    var dbias : array<float>
    let nfe = c.n_ff_exp
    if (c.moe_router_bias) {
        rbias |> reserve_resize(n_moe * ne)
    }
    if (c.moe_exps_bias) {
        gbias |> reserve_resize(n_moe * ne * nfe)
        ubias |> reserve_resize(n_moe * ne * nfe)
        dbias |> reserve_resize(n_moe * ne * dim)
    }
    var bi = 0l
    for (l in range64(c.n_layers)) {
        continue if (!layer_is_moe(t, l))
        if (c.moe_router_bias) {
            copy_floats(t.fblob, t.router_b_off + l * ne, rbias, bi * ne, ne)
        }
        if (c.moe_exps_bias) {
            copy_floats(t.fblob, t.web1_off + l * ne * nfe, gbias, bi * ne * nfe, ne * nfe)
            copy_floats(t.fblob, t.web3_off + l * ne * nfe, ubias, bi * ne * nfe, ne * nfe)
            copy_floats(t.fblob, t.web2_off + l * ne * dim, dbias, bi * ne * dim, ne * dim)
        }
        bi++
    }
    let ok = rdec_prepare_moe(ne, c.n_expert_used, nfe, c.n_ff_shexp, n_moe, sh_gated, c.norm_topk_prob || c.moe_gate == MoeGate.softmax_weight,
        c.ffn_act == FfnAct.gelu, c.ffn_act == FfnAct.swiglu_oai, c.expert_weights_scale, router, dscale, rbias, gbias, ubias, dbias)
    delete router
    delete dscale
    delete rbias
    delete gbias
    delete ubias
    delete dbias
    return ok
}

//! the expert triple of an MoE layer into the arena, sliced per expert as the per-op walk gathers it
def private resident_place_experts(t : Model; l : int64; mr, kg, wb : int64) : tuple<be1 : int64; be3 : int64; be2 : int64> {
    let c = t.config
    let ne = c.n_expert
    let nfe = c.n_ff_exp
    let be1 = resident_place(t, rdec_efmt(t, t.we1_fmt, l), t.we1_offs[l], c.dim, ne * nfe, mr, kg, wb, nfe)
    let be3 = be1 < 0l ? -1l : resident_place(t, rdec_efmt(t, t.we3_fmt, l), t.we3_offs[l], c.dim, ne * nfe, mr, kg, wb, nfe)
    let be2 = be3 < 0l ? -1l : resident_place(t, rdec_efmt(t, t.we2_fmt, l), t.we2_offs[l], nfe, ne * c.dim, mr, kg, wb, c.dim)
    return (be1 = be1, be3 = be3, be2 = be2)
}

//! One position's cos/sin plane for the resident device kernels (`build_rope_row_packed` is the one
//! source for every resident-override rope row): the global class's row, then the sliding class's where
//! the two rope differently - its own theta, position scale and head size; the p-RoPE factors only where the arch applies them
def private rdec_rope_rows(t : Model; pos : int64; var dst : array<float>; base : int64) {
    let c = t.config
    let rot = resident_rot(c)
    build_rope_row_packed(dst, base, pos, rot, c.rope_freq_base, c.rope_freq_scale, c.rope_mscale, t.rope_freqs)
    if (has_dual_rope(c)) {
        let no_freqs : array<float>
        build_rope_row_packed(dst, base + rot, pos, resident_rot_swa(c), c.rope_freq_base_swa, c.rope_freq_scale_swa, c.rope_mscale,
            c.rope_ff_global_only ? no_freqs : t.rope_freqs)
    }
}

//! a prompt's rope rows, one cos/sin plane per position at `delta` past its index, across the job threads: each row is its own work
def private resident_prefill_rope_rows(t : Model; delta, npos, ce : int64; var cb : array<float>) {
    unsafe {   // the rows land in disjoint ranges of the caller's buffer, one position each; the jobs take the model and the buffer by address
        let tp = addr(t)
        var cbp = addr(cb)
        maybe_parallel_for(npos >= 64l, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            for (p in range(rb, re)) {
                rdec_rope_rows(*tp, int64(p) + delta, *cbp, int64(p) * ce)
            }
        }
    }
}

//! the gather half of the per-layer-embedding pre-step into the scratch rows, timed (us) for the profile line
def private resident_prefill_ple_rows(t : Model; tokens : array<int64>; npos : int64) : int {
    let need = npos * t.config.n_layers * t.config.n_embd_per_layer
    g_rdec_ple_rows |> reserve(need)   // a long prompt's rows pass the unreserved-growth guard
    g_rdec_ple_rows |> resize(need)
    let ts_gather = ref_time_ticks()
    ple_gather_rows(t, tokens, npos, g_rdec_ple_rows)
    return get_time_usec(ts_gather)
}

var private g_rdec_cos_elems = 0l   //! a position's cos/sin plane width on the armed driver (`resident_cos_elems`)
// the N-row command's gathered rows: the residuals, the rope rows, each row's position, count and region, and where its logits land
var private @scratch @exact_size g_rdec_x_n : array<float>
var private @scratch @exact_size g_rdec_cos_n : array<float>
var private @scratch @do_not_delete g_rdec_lrows : array<float?>   //! borrowed rows into the sessions' logits
var private @scratch g_rdec_picks : array<int64>   //! the rows' device picks (-1 on a row that landed its logits)
var private g_rdec_picks_served = 0l
//! the rows whose argmax the device landed alone this process: a cell reads the difference across its steps
def public rdec_picks_served() : int64 => g_rdec_picks_served

//! a landed pick into the session: the next sample_ returns it (a row that landed its logits, -1, leaves the sampler to them)
def private rdec_land_pick(var s : Session; pick : int64) {
    if (pick >= 0l) {
        verify(!s.pick_ready, "dasLLAMA resident: a session's landed pick was never sampled before its next step - engine bug")
        s.pick_tok = pick
        s.pick_ready = true
        g_rdec_picks_served++
    }
}

//! the picks the first `n` rows landed, taken back: the step is declined after them, and the CPU rails' redo lands every row's logits
def private rdec_unland_picks(var sessions : array<Session?>; n : int64) {
    for (i in range64(n)) {
        var s & = unsafe(*sessions[i])
        if (s.pick_ready) {
            s.pick_ready = false
            g_rdec_picks_served--
        }
    }
}
var private @scratch g_rdec_poss : array<int64>
var private @scratch g_rdec_cnts : array<int64>
var private @scratch g_rdec_regs : array<int64>

// per-token scratch rows (capacity-retained; the driver is single-resident by the arbiter)
var private @scratch g_rdec_x : array<float>
var private @scratch g_rdec_cossin : array<float>
var private @scratch g_rdec_tmp : array<float>

// why a resident override handed a call back to the CPU rails; each kind is said once per armed model
enum private RdecPass {
    codec
    paged
    gap
    cap
    superseded
    continuation
    span
    pinned_off
    rewind
    busy
}

var private g_rdec_pass_said : table<int>

def private kvdt_name(v : int64) : string {
    if (v == int64(int(KVDtype.f32))) return "f32"
    if (v == int64(int(KVDtype.f16))) return "f16"
    if (v == int64(int(KVDtype.q8_0))) return "q8_0"
    if (v == int64(int(KVDtype.tq4))) return "tq4"
    return "codec {v}"
}

var private g_rdec_last_pass : tuple<kind : RdecPass; a : int64; b : int64>   //! the pass just counted, for the device-home panic's text
var private g_rdec_pass_seq = 0l   //! bumped per counted pass: the device-home guard tells a pass of its own call from one left by an earlier call

[cold_path]
def private rdec_pass_once(kind : RdecPass; a, b : int64) {
    note_gpu_cpu_pass(int(kind))
    g_rdec_last_pass = (kind = kind, a = a, b = b)
    g_rdec_pass_seq++
    if (key_exists(g_rdec_pass_said, int(kind))) {
        return
    }
    g_rdec_pass_said |> insert(int(kind))
    to_log(LOG_INFO, "dasLLAMA: resident override passed a call to the CPU rails - {rdec_pass_text(kind, a, b)} (said once per reason)\n")
}

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]
def private rdec_device_home_guard(s : Session; done : bool; what : string; seq0 : int64) {
    if (done || !s.device_kv) {
        return
    }
    let why = (g_rdec_pass_seq != seq0 ? rdec_pass_text(g_rdec_last_pass.kind, g_rdec_last_pass.a, g_rdec_last_pass.b)
        : "the whole-model driver is not armed for this model")
    panic("dasLLAMA resident {what}: a device-home session was handed to the CPU rails - {why}")
}

//! why a call went to the CPU, for a reader who never saw the engine: what a status page prints beside the count
[cold_path]
def private rdec_pass_words(kind : RdecPass) : string {
    if (kind == RdecPass.codec) return "the stream's cache format is not the one the GPU holds"
    if (kind == RdecPass.paged) return "the stream keeps its cache in pages on the host, which the GPU path does not read"
    if (kind == RdecPass.gap) return "the GPU's copy of the stream's cache was behind the stream"
    if (kind == RdecPass.cap) return "the stream ran past the context the GPU holds"
    if (kind == RdecPass.superseded) return "another stream took the GPU's copy of the cache"
    if (kind == RdecPass.continuation) return "a prompt continued a conversation the GPU prefills only from its start"
    if (kind == RdecPass.span) return "the prompt carried an image or audio span"
    if (kind == RdecPass.rewind) return "the stream stepped back in a conversation the GPU only steps forward"
    if (kind == RdecPass.busy) return "every GPU cache slot was held by a live stream"
    if (kind == RdecPass.pinned_off) return "a prompt was read on the CPU: the server keeps host-cached streams off the GPU's prompt path"
    return "{kind}"
}

[cold_path]
def private rdec_pass_text(kind : RdecPass; a, b : int64) : string {
    var why = ""
    if (kind == RdecPass.codec) {
        why = "the session's K/V codec {kvdt_name(a)}/{kvdt_name(b)} is not the armed mirror codec {kvdt_name(int64(int(g_rdec_kvdt)))} (a session made for the CPU rails)"
    } elif (kind == RdecPass.paged) {
        why = "the session uses a paged K/V cache; the mirror serves the flat cache only"
    } elif (kind == RdecPass.gap) {
        why = "position {a} is past the mirror's {b} live rows"
    } elif (kind == RdecPass.cap) {
        why = "position {a} reaches the mirror's context capacity {b}"
    } elif (kind == RdecPass.superseded) {
        why = "another session's prefill took this session's mirror region"
    } elif (kind == RdecPass.continuation) {
        why = "the prefill continues an existing history from position {a}"
    } elif (kind == RdecPass.span) {
        why = "the eval carries a non-causal span"
    } elif (kind == RdecPass.rewind) {
        why = "position {a} is not the deltanet state's next position {b} (the recurrent state is forward-only)"
    } elif (kind == RdecPass.busy) {
        why = "all {a} mirror regions hold a live device-home session"
    } else {
        why = "the resident prefill is pinned off (a multi-stream scheduler owns the sessions)"
    }
    return why
}

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private rdec_take_mirror(t : Model; var s : Session; r, pos : int64) {
    let c = t.config
    if (s.device_kv) {   //! a device-home session has nothing on the host to bring up: it claims an empty region
        if (pos != 0l) {
            panic("dasLLAMA resident driver: a device-home session at position {pos} holds no mirror region - more live device-home sessions than the driver has regions ({long_length(g_rdec_regions)})")
        }
        rdec_claim(s, r, 0l)
        return
    }
    for (l in range64(c.n_layers)) {
        continue if (layer_is_recurrent(c, l) || t.kv_src[l] != l)
        rdec_sync_kv(l, kv_layer_kbase(s, l, c.seq_len), kv_layer_vbase(s, l, c.seq_len), pos)
    }
    rdec_claim(s, r, pos)
    s.rdec_host_current = true   // the host still holds everything the mirror does
}

//! a row's recurrent state into its selected region's slots before the step: the state from zero at position 0, its position advanced
def private rdec_dn_row_own(var s : Session; c : Config; pos : int64) {
    if (pos == 0l) {
        dn_reset(s)
    }
    s.dn_pos = pos + 1l
    rdec_dn_own_all(s, c.n_layers)
}

[hot_path]
def private rdec_dn_own_all(var s : Session; n_layers : int64) {
    for (l in range64(n_layers)) {
        continue if (s.dn_state_prefix[l] < 0l)
        unsafe {
            rdec_dn_own(l, addr(s.dn_state[s.dn_state_prefix[l]]), addr(s.dn_conv_state[s.dn_conv_prefix[l]]))
        }
    }
}

var private @scratch g_rdec_one_tok : array<int64>   //! the token's id as the gather's one-row list

// the resident whole-stack decode override (registered as "vulkan") — the mirror builds as positions arrive
[hot_path]   // per token
def private vulkan_resident_decode(t : Model; var s : Session; token, pos : int64) : bool {
    let seq0 = g_rdec_pass_seq
    let done = vulkan_resident_decode_go(t, s, token, pos)
    rdec_device_home_guard(s, done, "decode", seq0)
    if (!done && s.ple_gpu_pending) {   //! the gate took the pre-step and the driver then declined: the CPU pre-step runs late
        ple_pre_decode(t, s, token)
        s.ple_gpu_pending = false
    }
    return done
}

def private vulkan_resident_decode_go(t : Model; var s : Session; token, pos : int64) : bool {
    if (!g_rdec_active) {
        return false
    }
    if (s.kv_dtype_k != g_rdec_kvdt || s.kv_dtype_v != g_rdec_kvdt) {
        rdec_pass_once(RdecPass.codec, int64(int(s.kv_dtype_k)), int64(int(s.kv_dtype_v)))
        return false
    }
    if (s.kv_pool != null) {
        rdec_pass_once(RdecPass.paged, 0l, 0l)
        return false
    }
    if (pos + 1l > g_rdec_cap) {
        return rdec_pass_hydrated(t, s, RdecPass.cap, pos, g_rdec_cap)
    }
    let c = t.config
    let r = rdec_bind_region(s)
    if (r < 0l) {
        return false
    }
    if (!rdec_owns(s, r) || pos > g_rdec_regions[r].cnt) {
        if (rdec_owns(s, r)) {
            rdec_pass_once(RdecPass.gap, pos, g_rdec_regions[r].cnt)   // our own region short of pos: nothing on the host past cnt either
            return false
        }
        if (s.rdec_gen != 0l && !s.rdec_host_current) {
            panic("dasLLAMA resident decode: session's mirror rows were superseded before hydration - engine bug (the steal path must hydrate first)")
        }
        rdec_take_mirror(t, s, r, pos)
    }
    if (c.recr_mask != 0ul) {
        if (pos == 0l) {
            dn_reset(s)
        } elif (pos != s.dn_pos) {
            rdec_pass_once(RdecPass.rewind, pos, s.dn_pos)
            return false
        }
        s.dn_pos = pos + 1l
        rdec_dn_own_all(s, c.n_layers)
    }
    let dim = c.dim
    g_rdec_x |> resize(dim)
    embed_row(t, token, unsafe(addr(g_rdec_x[0])))
    if (c.embed_scale != 1.0) {
        scale_inplace(unsafe(addr(g_rdec_x[0])), c.embed_scale, dim)
    }
    g_rdec_cossin |> resize(g_rdec_cos_elems)
    rdec_rope_rows(t, pos + s.rope_pos_delta, g_rdec_cossin, 0l)   // rope at the delta'd position; KV stays at pos
    let cnt = pos + 1l
    if (s.ple_gpu_pending) {
        g_rdec_one_tok |> resize(1l)
        g_rdec_one_tok[0] = token
        resident_prefill_ple_rows(t, g_rdec_one_tok, 1l)
        rdec_land_pick(s, rdec_token(g_rdec_x, g_rdec_cossin, g_rdec_ple_rows, pos, cnt, s.pick_asked, s.logits))
        s.ple_gpu_pending = false
    } else {
        rdec_land_pick(s, rdec_token(g_rdec_x, g_rdec_cossin, s.ple_inp, pos, cnt, s.pick_asked, s.logits))   //! forward() ran the PLE pre-step into s.ple_inp before the override; empty elsewhere
    }
    g_rdec_regions[r].cnt = pos + 1l
    s.rdec_host_current = false   // the mirror advanced past any earlier hydration
    decode_override_logits_done()   // logits produced on device
    return true
}

var private @scratch g_rdec_ple_rows : array<float>   //! the gather half of the per-layer-embedding pre-step, position-major, when the driver finishes it on device

//! ONE gate for both PLE pre-step seats, taken when the armed driver holds the projection:
//! forward_prefill's, where the token ids land in the session's stash and the prefill override gathers the table rows,
//! and the decode seat's, where the token command gathers the step's own row on device.
[unused_argument(s, npos, start_pos)]
def private vulkan_ple_gate(t : Model; s : Session; npos, start_pos : int64) : bool
    => g_rdec_active && has_ple(t.config) && rdec_ple_proj_on()

// the "vulkan" whole-prefill override — host KV hydrates back on demand, so the CPU rails always take over from a valid authority
[hot_path, arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]   // per prefill call
def private vulkan_resident_prefill(t : Model; var s : Session; npos, start_pos : int64) : bool {
    let seq0 = g_rdec_pass_seq
    let done = vulkan_resident_prefill_go(t, s, npos, start_pos)
    rdec_device_home_guard(s, done, "prefill", seq0)
    if (!done && s.ple_gpu_pending) {   //! the gate took the pre-step and the driver then declined: the CPU pre-step runs late, off the stashed ids
        ple_pre_prefill(t, s, s.ple_gpu_tokens, npos)
        s.ple_gpu_pending = false
    }
    return done
}

//! Always false, the override's answer: the CPU rails take the call, so the session's device-only rows come down first.
def private rdec_pass_hydrated(t : Model; var s : Session; kind : RdecPass; a, b : int64) : bool {
    if (rdec_has_device_only_rows(s)) {
        rdec_hydrate_host(t, s)
    }
    rdec_pass_once(kind, a, b)
    return false
}

//! The region a prefill of `s` from `start_pos` lands in, or -1 where the call passes.
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]
def private rdec_prefill_region(t : Model; var s : Session; start_pos : int64) : int64 {
    let r = rdec_bind_region(s)
    if (start_pos == 0l || r < 0l) {
        return r
    }
    let owns = rdec_owns(s, r)
    if (owns && g_rdec_regions[r].cnt >= start_pos) {
        g_rdec_regions[r].cnt = start_pos   // rows past the start are dead: the windows rewrite them
        return r
    }
    if (owns && !s.rdec_host_current) {
        rdec_pass_hydrated(t, s, RdecPass.gap, start_pos, g_rdec_regions[r].cnt)   // our own region short of the start: the CPU takes the call over the rows the region does hold
        return -1l
    }
    if (!owns && s.rdec_gen != 0l && !s.rdec_host_current) {
        panic("dasLLAMA resident prefill: session's mirror rows were superseded before hydration - engine bug (the steal path must hydrate first)")
    }
    rdec_take_mirror(t, s, r, start_pos)
    return r
}

def private vulkan_resident_prefill_go(t : Model; var s : Session; npos, start_pos : int64) : bool {
    if (!g_rdec_active) {
        return false
    }
    if (!g_resident_prefill_allowed && !s.device_kv) {
        rdec_pass_once(RdecPass.pinned_off, 0l, 0l)
        return false
    }
    if (s.kv_dtype_k != g_rdec_kvdt || s.kv_dtype_v != g_rdec_kvdt) {
        rdec_pass_once(RdecPass.codec, int64(int(s.kv_dtype_k)), int64(int(s.kv_dtype_v)))
        return false
    }
    if (s.kv_pool != null) {
        rdec_pass_once(RdecPass.paged, 0l, 0l)
        return false
    }
    if (s.attn_uniform_end != 0l) {
        rdec_pass_once(RdecPass.span, 0l, 0l)
        return false
    }
    let c = t.config
    if (start_pos != 0l && c.recr_mask != 0ul) {
        return rdec_pass_hydrated(t, s, RdecPass.continuation, start_pos, 0l)
    }
    if (start_pos + npos > g_rdec_cap) {
        return rdec_pass_hydrated(t, s, RdecPass.cap, start_pos + npos, g_rdec_cap)
    }
    let r = rdec_prefill_region(t, s, start_pos)
    if (r < 0l) {
        return false
    }
    if (start_pos == 0l && c.recr_mask != 0ul) {
        dn_reset(s)
    }
    let ce = g_rdec_cos_elems
    g_rdec_tmp |> resize(npos * ce)   // the rope-plane batch borrows the scratch row
    var cb & = g_rdec_tmp
    let ts_rope = ref_time_ticks()
    resident_prefill_rope_rows(t, s.rope_pos_delta + start_pos, npos, ce, cb)
    let us_rope = get_time_usec(ts_rope)
    var us_gather = 0
    let ts_run = ref_time_ticks()
    if (s.embed_gpu_pending) {
        rdec_prefill_ids(s.embed_gpu_tokens, c.embed_scale, cb, npos, start_pos, s.logits)
        s.embed_gpu_pending = false
        s.ple_gpu_pending = false   //! the ids form gathers the side rows on device too: no late CPU pre-step
    } else {
        if (s.ple_gpu_pending) {
            us_gather = resident_prefill_ple_rows(t, s.ple_gpu_tokens, npos)
            rdec_prefill(s.x_b, cb, g_rdec_ple_rows, true, npos, start_pos, s.logits)
            s.ple_gpu_pending = false
        } else {
            rdec_prefill(s.x_b, cb, s.ple_inp, false, npos, start_pos, s.logits)   //! forward_prefill ran the PLE pre-step into s.ple_inp (position-major) before the override; empty elsewhere
        }
    }
    if (g_env_gpu.gpu_prof) {
        to_log(LOG_INFO, "dasLLAMA: resident prefill host: {npos} rows, rope rows {us_rope} us, ple gather {us_gather} us, the driver's run {get_time_usec(ts_run) - us_gather} us\n")   // nolint:PERF026,PERF028 — the profile flag's report leg
    }
    if (c.recr_mask != 0ul) {
        for (l in range64(c.n_layers)) {
            continue if (s.dn_state_prefix[l] < 0l)
            unsafe {
                rdec_dn_prefill_own(l, addr(s.dn_state[s.dn_state_prefix[l]]), addr(s.dn_conv_state[s.dn_conv_prefix[l]]))
            }
        }
        s.dn_pos = npos
    }
    if (start_pos == 0l) {
        rdec_claim(s, r, npos)
    } else {
        g_rdec_regions[r].cnt = start_pos + npos
    }
    s.rdec_host_current = false
    g_rdec_prefills++
    prefill_override_logits_done()
    return true
}

// the resident batch-decode override — each session SEQUENTIALLY through the single-session chain, in its own mirror region
[hot_path]   // per batch-decode step (every session's token)
def private vulkan_resident_batch_decode(t : Model; var ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool {
    if (!g_rdec_active) {
        return false
    }
    let seq0 = g_rdec_pass_seq
    let done = rdec_batch_rows_served(t, ws, sessions, nrows) && vulkan_resident_batch_decode_go(t, ws, sessions, nrows)
    for (i in range64(nrows)) {
        rdec_device_home_guard(*sessions[i], done, "batch decode", seq0)
    }
    return done
}

//! all rows or none: one row the driver cannot serve passes the whole step
def private rdec_batch_rows_served(t : Model; ws : BatchWorkspace; sessions : array<Session?>; nrows : int64) : bool {
    let c = t.config
    for (i in range64(nrows)) {
        var s = sessions[i]
        if (s.kv_dtype_k != g_rdec_kvdt || s.kv_dtype_v != g_rdec_kvdt) {
            rdec_pass_once(RdecPass.codec, int64(int(s.kv_dtype_k)), int64(int(s.kv_dtype_v)))
            return false
        }
        if (s.kv_pool != null) {
            rdec_pass_once(RdecPass.paged, 0l, 0l)
            return false
        }
        if (ws.positions[i] + 1l > g_rdec_cap) {
            rdec_pass_once(RdecPass.cap, ws.positions[i], g_rdec_cap)
            return false
        }
        if (c.recr_mask != 0ul && ws.positions[i] != 0l && ws.positions[i] != s.dn_pos) {
            rdec_pass_once(RdecPass.rewind, ws.positions[i], s.dn_pos)
            return false
        }
    }
    return true
}

//! the side-input rows of `n` tokens for the token command, into `g_rdec_ple_rows`: the table rows gathered on device where
//! the device projects, else the pre-step's finished rows from `src_off` of the workspace's rows (position-major either way)
def private rdec_ple_side_rows(t : Model; tokens : array<int64>; n : int64; ws : BatchWorkspace; src_off : int64) {
    if (rdec_ple_proj_on()) {
        resident_prefill_ple_rows(t, tokens, n)
        return
    }
    let all = t.config.n_layers * t.config.n_embd_per_layer * n
    g_rdec_ple_rows |> reserve(all)
    g_rdec_ple_rows |> resize(all)
    copy_floats(ws.scr.ple_inp, src_off, g_rdec_ple_rows, 0l, all)
}

//! row `i`'s side input for the token command: the table row gathered on device, else row `i` of the pre-step `eval_batch` already ran
def private rdec_batch_ple_row(t : Model; ws : BatchWorkspace; i : int64) {
    g_rdec_one_tok |> resize(1l)
    g_rdec_one_tok[0] = ws.row_tokens[i]
    rdec_ple_side_rows(t, g_rdec_one_tok, 1l, ws, i * t.config.n_layers * t.config.n_embd_per_layer)
}

//! the step's rows through the N-row token command, one weight pass for all; false when the rows' regions do not serve
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def private rdec_batch_rows_at_once(t : Model; ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool {
    let c = t.config
    let dim = c.dim
    g_rdec_x_n |> grow_resize(dim * nrows)
    g_rdec_cos_n |> grow_resize(g_rdec_cos_elems * nrows)
    let ple_raw = rdec_ple_proj_on()
    if (has_ple(c)) {
        rdec_ple_side_rows(t, ws.row_tokens, nrows, ws, 0l)
    }
    g_rdec_lrows |> resize(nrows)
    g_rdec_poss |> resize(nrows)
    g_rdec_cnts |> resize(nrows)
    g_rdec_regs |> resize(nrows)
    let ts_prep = prof_ticks()
    for (i in range64(nrows)) {
        var s & = unsafe(*sessions[i])
        let pos = ws.positions[i]
        let r = rdec_bind_region(s)
        if (r < 0l) {
            return false
        }
        for (j in range64(i)) {
            if (g_rdec_regs[j] == r) {   //! two rows one region: the row-at-a-time loop resyncs between them
                return false
            }
        }
        if (!rdec_owns(s, r) || g_rdec_regions[r].cnt != pos) {
            if (!rdec_owns(s, r) && s.rdec_gen != 0l && !s.rdec_host_current) {
                panic("dasLLAMA resident batch decode: row {i}'s mirror rows were superseded before hydration (region {r}, gen {s.rdec_gen}, pos {pos}) - engine bug (the steal path must hydrate first)")
            }
            rdec_take_mirror(t, s, r, pos)
        }
        if (c.recr_mask != 0ul) {
            rdec_dn_row_own(s, c, pos)
        }
        copy_floats(ws.scr.x_b, i * dim, g_rdec_x_n, i * dim, dim)
        rdec_rope_rows(t, ws.rope_positions[i], g_rdec_cos_n, i * g_rdec_cos_elems)
        g_rdec_poss[i] = pos
        g_rdec_cnts[i] = pos + 1l
        g_rdec_regs[i] = r
        g_rdec_lrows[i] = null
        if (!s.pick_asked) {
            g_rdec_lrows[i] = unsafe(addr(s.logits[0]))
        }
    }
    g_rdec_picks |> resize(nrows)
    prof_add("nrow_prep", ts_prep)
    let ts_gpu = prof_ticks()
    rdec_token_n(g_rdec_x_n, g_rdec_cos_n, g_rdec_ple_rows, ple_raw, g_rdec_poss, g_rdec_cnts, g_rdec_regs, nrows, g_rdec_lrows, g_rdec_picks)
    prof_add("nrow_gpu", ts_gpu)
    let ts_land = prof_ticks()
    for (i in range64(nrows)) {
        var s & = unsafe(*sessions[i])
        rdec_land_pick(s, g_rdec_picks[i])
        let pos = g_rdec_poss[i]
        let r = g_rdec_regs[i]
        if (!s.device_kv) {
            rdec_select_region(r)   //! the readback reads the selected region: this row's, bound above
            for (l in range64(c.n_layers)) {
                continue if (layer_is_recurrent(c, l) || t.kv_src[l] != l)
                let kvd = layer_kv_dim(c, l)
                let krowb = kv_row_bytes(s.kv_dtype_k, kvd)
                let vrowb = kv_row_bytes(s.kv_dtype_v, kvd)
                unsafe {
                    rdec_read_kv(l, pos, kv_layer_kbase(s, l, c.seq_len) + pos * krowb,
                        kv_layer_vbase(s, l, c.seq_len) + pos * vrowb)
                }
            }
        }
        g_rdec_regions[r].cnt = pos + 1l
        s.rdec_host_current = !s.device_kv
    }
    prof_add("nrow_land", ts_land)
    batch_decode_override_logits_done()
    return true
}

def private vulkan_resident_batch_decode_go(t : Model; ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool {
    let c = t.config
    let dim = c.dim
    let ple = has_ple(c)
    if (!ple) {
        g_rdec_ple_rows |> clear()
    }
    if (dasllama_noisy()) {
        to_log(LOG_INFO, "dasLLAMA: resident batch decode engaged ({nrows} streams)\n")   // nolint:PERF026,PERF028 — noisy-gated diagnostics
    }
    if (nrows > 1l && rdec_token_n_rows() >= nrows && rdec_batch_rows_at_once(t, ws, sessions, nrows)) {
        return true
    }
    g_rdec_x |> resize(dim)
    g_rdec_cossin |> resize(g_rdec_cos_elems)
    g_rdec_tmp |> resize(c.vocab_size)
    for (i in range64(nrows)) {
        var s & = unsafe(*sessions[i])
        let pos = ws.positions[i]
        let r = rdec_bind_region(s)
        if (r < 0l) {
            rdec_unland_picks(sessions, i)
            return false   //! the CPU rails redo the whole step: rows already stepped here rewrite the same position, and their regions resync from the host
        }
        if (!rdec_owns(s, r) || g_rdec_regions[r].cnt != pos) {
            if (!rdec_owns(s, r) && s.rdec_gen != 0l && !s.rdec_host_current) {
                panic("dasLLAMA resident row-at-a-time decode: row {i}'s mirror rows were superseded before hydration (region {r}, gen {s.rdec_gen}, pos {pos}) - engine bug (the steal path must hydrate first)")
            }
            rdec_take_mirror(t, s, r, pos)
        }
        if (c.recr_mask != 0ul) {
            rdec_dn_row_own(s, c, pos)
        }
        // this row's embedded residual is already in ws.scr.x_b; its rope row for pos
        for (k in range64(dim)) {
            g_rdec_x[int(k)] = ws.scr.x_b[i * dim + k]
        }
        rdec_rope_rows(t, ws.rope_positions[i], g_rdec_cossin, 0l)
        if (ple) {
            rdec_batch_ple_row(t, ws, i)
        }
        let picked = rdec_token(g_rdec_x, g_rdec_cossin, g_rdec_ple_rows, pos, pos + 1l, s.pick_asked, g_rdec_tmp)
        for (l in range64(c.n_layers)) {
            continue if (s.device_kv || layer_is_recurrent(c, l) || t.kv_src[l] != l)
            let kvd = layer_kv_dim(c, l)
            let krowb = kv_row_bytes(s.kv_dtype_k, kvd)
            let vrowb = kv_row_bytes(s.kv_dtype_v, kvd)
            unsafe {
                rdec_read_kv(l, pos, kv_layer_kbase(s, l, c.seq_len) + pos * krowb,
                    kv_layer_vbase(s, l, c.seq_len) + pos * vrowb)
            }
        }
        g_rdec_regions[r].cnt = pos + 1l
        s.rdec_host_current = !s.device_kv
        if (picked >= 0l) {
            rdec_land_pick(s, picked)
        } else {
            copy_floats(g_rdec_tmp, 0l, s.logits, 0l, c.vocab_size)
        }
    }
    batch_decode_override_logits_done()
    return true
}

[hot_path, arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]   // per prefill quantum - reached only through the engine's registered probe
def private vulkan_embed_gpu_gate(t : Model; s : Session; npos, start_pos : int64) : bool {
    return (active_prefill_override() == "vulkan" && g_rdec_active && (g_resident_prefill_allowed || s.device_kv) && g_rdec_emb_gpu
        && (start_pos == 0l || t.config.recr_mask == 0ul) && start_pos + npos <= g_rdec_cap
        && s.kv_dtype_k == g_rdec_kvdt && s.kv_dtype_v == g_rdec_kvdt && s.kv_pool == null
        && s.attn_uniform_end == 0l && empty(s.mrope_pos) && !s.ds_active)
}

[init, arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def resident_driver_register() {
    register_embed_gpu_gate(@@vulkan_embed_gpu_gate)
    register_decode_override("vulkan", @@vulkan_resident_decode)
    register_batch_decode_override("vulkan", @@vulkan_resident_batch_decode)
    register_prefill_override("vulkan", @@vulkan_resident_prefill)
    register_ple_gpu_gate(@@vulkan_ple_gate)
    register_ple_gpu_decode_gate(@@vulkan_ple_gate)   //! the token command gathers the row on device too
    register_prefill_override_split_span("vulkan")
    register_gpu_upload_resident(@@moe_gpu_upload_resident)
    set_moe_gpu_slice_window_fn(@@vk_slice_window_provider)
}

// ===== multi-model GPU slot switch (marks swap + whole-model drop/re-arm) =====

//! One model's complete per-model tier state for a multi-model host: the loader-contract marks
//! plus the resident driver's activation. Save after the model's load (or arm), restore before
//! stepping that model — two models' offset-keyed marks must never be installed together.
struct GpuModelMarks {
    marks : MoeGpuMarks <- MoeGpuMarks()
    rdec_active : bool
    rdec_regions : array<RdecRegion>   //! every mirror region's rows and owner, so a model stepped out and back finds its sessions where they were
    rdec_cap : int64
    rdec_kvdt : KVDtype
    rdec_emb_gpu : bool
    rdec_ple_proj : bool   //! the E-series pre-step's projection sits on the device (`forward()` skips its CPU form)
}

//! A fresh no-model GpuModelMarks — the facade's ctor-shaped re-export delegates here
//! (a generated constructor is not callable through a facade typedef).
def gpu_model_marks_init_() : GpuModelMarks {
    return <- GpuModelMarks()
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#gpu-slot-marks")]
def moe_gpu_model_marks_save_(var st : GpuModelMarks) {
    moe_gpu_marks_save(st.marks)
    st.rdec_active = g_rdec_active
    delete st.rdec_regions
    st.rdec_regions <- g_rdec_regions
    st.rdec_cap = g_rdec_cap
    st.rdec_kvdt = g_rdec_kvdt
    st.rdec_emb_gpu = g_rdec_emb_gpu
    st.rdec_ple_proj = rdec_ple_proj_on()
    rdec_set_active(false)
    g_rdec_cap = 0l
    g_rdec_emb_gpu = false
    rdec_ple_proj_set(false)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#gpu-slot-marks")]
def moe_gpu_model_marks_restore_(var st : GpuModelMarks) {
    moe_gpu_marks_restore(st.marks)
    rdec_set_active(st.rdec_active)
    delete g_rdec_regions
    g_rdec_regions <- st.rdec_regions
    g_rdec_cap = st.rdec_cap
    g_rdec_kvdt = st.rdec_kvdt
    g_rdec_emb_gpu = st.rdec_emb_gpu
    rdec_ple_proj_set(st.rdec_ple_proj)
    st.rdec_active = false
    st.rdec_cap = 0l
    st.rdec_emb_gpu = false
    st.rdec_ple_proj = false
}

//! How many device-home sessions the INSTALLED model serves at once: the armed driver's region count, 0 when the driver is not armed.
def gpu_device_sessions_() : int64 => g_rdec_active ? long_length(g_rdec_regions) : 0l

//! A device-home session of `t` prefills from a position past zero - false for a recurrent model, whose prompt prefills whole, from zero.
def gpu_device_prefill_continues_(t : Model) : bool => t.config.recr_mask == 0ul

//! A finished device-home session lets go of its region and answers the claim its rows stay under (0 = none), which dies when another session takes the region.
def gpu_device_kv_park_(var s : Session) : int64 {
    let claim = s.rdec_gen
    if (rdec_owns(s, s.rdec_region)) {
        g_rdec_regions[s.rdec_region].pinned = false
    }
    s.rdec_gen = 0l
    return claim
}

//! Hand the rows `[0, npos)` a kept `claim` holds to the fresh device-home session `s` under a claim of its own; false when the region was taken or holds fewer rows.
def gpu_device_kv_adopt_(var s : Session; claim, npos : int64) : bool {
    if (!g_rdec_active || claim == 0l) {
        return false
    }
    for (r in range64(long_length(g_rdec_regions))) {
        continue if (g_rdec_regions[r].gen != claim)
        if (g_rdec_regions[r].cnt < npos) {
            return false
        }
        rdec_claim(s, r, npos)
        s.rdec_host_current = false
        s.n_past = npos
        return true
    }
    return false
}

//! The positions one device-home session holds: the armed driver's per-region context.
def gpu_device_session_ctx_() : int64 => g_rdec_cap

//! The rows the device prefills in one window: a host that prefills a device-home session in chunks makes them at least this long.
def gpu_device_prefill_window_() : int64 => RDEC_PF_ROWS

//! The K/V codec a device-home session carries: the armed driver's mirror codec.
def gpu_device_session_dtype_() : KVDtype => g_rdec_kvdt

//! Is the resident whole-stack decode driver active for the INSTALLED model? (The arm-outcome
//! discriminator status surfaces read: resident beats rails beats cpu.)
def moe_gpu_resident_active() : bool => g_rdec_active

var private g_rdec_prefills = 0l

//! How many prompts the resident window chain has prefilled in this process (a test's served witness)
def moe_gpu_resident_prefills() : int64 => g_rdec_prefills

//! Capture the INSTALLED model's tier state into `st` (see ``moe_gpu_model_marks_save_``) and
//! classify what it had: "gpu:resident" | "gpu:rails" | "cpu". The load-time half of a
//! multi-model host's slot bookkeeping.
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#gpu-slot-marks")]
def gpu_slot_capture_(var st : GpuModelMarks) : string {
    let was_resident = g_rdec_active
    moe_gpu_model_marks_save_(st)
    return was_resident ? "gpu:resident" : (moe_gpu_marks_armed(st.marks) ? "gpu:rails" : "cpu")
}

//! Re-arm the INSTALLED model onto the GPU tier (marks already restored): want, arm, resident
//! upload — through the bake-slice path when `t` is a mapped vulkan-flavor image (a live
//! re-gather on a planes-trimmed image panics). Returns "gpu:resident" | "gpu:rails" | "cpu".
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#gpu-slot-marks")]
def gpu_slot_rearm_(want : GpuTierWant; t : Model) : string {
    set_gpu_tier_want_(want)
    moe_gpu_tier_arm_()
    if (!empty(t.vkplan)) {
        vulkan_bake_slice_begin()
        moe_gpu_upload_resident(t)
        vulkan_bake_slice_end(t)
    } else {
        moe_gpu_upload_resident(t)
    }
    return g_rdec_active ? "gpu:resident" : (gpu_tier_status_().vram_bytes > 0l ? "gpu:rails" : "cpu")
}

//! Hydrate `s`'s host KV from the device mirror when `s` owns live mirror rows (no-op
//! otherwise). Call over every live session of the outgoing model before ``moe_gpu_drop_model_``
//! — the drop destroys the mirror, and an unhydrated owner's rows exist nowhere else.
def moe_gpu_hydrate_session_(t : Model; var s : Session) {
    // codec gate mirrors the overrides: hydration byte-copies mirror rows, so a codec-mismatched
    // session must never receive them (the gen match alone is ambiguous at generation 0)
    if (g_rdec_active && rdec_has_device_only_rows(s)
            && s.kv_dtype_k == g_rdec_kvdt && s.kv_dtype_v == g_rdec_kvdt && s.kv_pool == null) {
        rdec_hydrate_host(t, s)
    }
}

//! Drop the INSTALLED model's whole GPU state (every model-owned device object + the routing
//! marks); the model keeps serving on the CPU rails, and re-arm = the same three calls a load
//! runs. Caller guarantees quiescence: no step in flight, sessions hydrated first.
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#gpu-slot-marks")]
def moe_gpu_drop_model_() {
    if (!moe_gpu_tier_installed()) {
        return
    }
    moe_gpu_dn_invalidate()
    rdec_set_active(false)
    reset_gpu_cpu_passes()
    g_rdec_regions |> clear()
    g_rdec_cap = 0l
    g_rdec_emb_gpu = false
    rdec_ple_proj_set(false)
    // resident_upload selected the "vulkan" overrides; deselect so (a) a later re-arm's
    // resident_upload passes its no-active-override gate (the warm re-arm decline), and (b) a
    // dropped model's prefill/decode take the plain CPU path without consulting dead state
    if (active_prefill_override() == "vulkan") {
        select_prefill_override("")
    }
    if (active_decode_override() == "vulkan") {
        select_decode_override("")
    }
    if (active_decode_override() == "vulkan_moe_span") {
        select_decode_override("")
    }
    moe_gpu_drop_model_state()
    set_moe_gpu_stream_need(0l)
    set_moe_gpu_dat_need(0l)
    // loader-contract reset — the same clean slate moe_gpu_upload_resident starts from
    set_moe_gpu_layer_count(0l)
    set_moe_gpu_from_layer(1000000l)
    set_moe_gpu_stream_from(1000000l)
    set_moe_gpu_total_layers(0l)
    set_moe_gpu_cls_off(-1l)
    reset_moe_gpu_dense_offs()
    reset_moe_gpu_dn_layers()
    reset_moe_gpu_attn_layers()
    reset_moe_gpu_shexp_layers()
    reset_moe_gpu_qkv_layers()
    set_moe_gpu_model_support("")
}

//! The upload rail's first step: a model still installed on the tier (its marks armed, or the
//! resident driver active) is dropped before this load uploads, with the load's own MoE layer
//! request - the one mark the drop's reset would zero - carried across.
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#gpu-slot-marks")]
def moe_gpu_drop_installed_before_load(n_layers : int64) {
    if (!moe_gpu_tier_installed() || !(g_rdec_active || moe_gpu_installed_marks_armed())) {
        return
    }
    to_log(LOG_INFO, "dasLLAMA: the GPU tier still holds a model - dropping its device state before loading this one ({n_layers} layers)\n")
    let want_layers = moe_gpu_layer_count()
    moe_gpu_drop_model_()
    set_moe_gpu_layer_count(want_layers)
}

struct private WalkSkips {
    skipped : int64
    first_why : string
}

def private walk_skip(var w : WalkSkips; l : int64; why : string) {
    if (w.skipped == 0l) {
        w.first_why = "layer {l}: {why}"
    }
    w.skipped++
}

[cold_path]
def private walk_say(w : WalkSkips; rail : string) {
    if (w.skipped > 0l) {
        to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: the {rail} leaves {w.skipped} layer(s) on the CPU (first - {w.first_why})\n")
    }
}

[cold_path]
def private walk_budget_say(rail : string; l, layers : int64) {
    to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: the {rail} stopped at layer {l} - VRAM budget; layers {l}..{layers - 1l} stay on the CPU\n")
}

//! The attention features the per-op attention-quad chain has no arm for, comma-separated; "".
def attn_chain_unserved_features(c : Config) : string {
    var names = ""
    name_list_add(names, c.v_norm, "v-norm")
    name_list_add(names, c.attn_sinks, "attention sinks")
    name_list_add(names, c.attn_logit_softcap != 0.0, "attention logit softcap")
    name_list_add(names, c.pre_post_norm, "pre/post norms")
    name_list_add(names, c.attn_out_bias, "attention output bias")
    name_list_add(names, !(c.rope_neox || (c.rope_dim > 0l && c.rope_dim != c.head_size)), "interleaved (non-NEOX) rope")   //! a partial rotary is NEOX per the loader, so it passes unroped
    return names
}

[cold_path]
def private dense_ffn_cpu_say(t : Model) {
    let c = t.config
    var bytes = 0l
    for (l in range64(c.n_layers)) {
        continue if (t.w1_offs[l] < 0l)
        let hid = layer_hidden(t, l)
        bytes += moe_gpu_plane_bytes(c.dim, hid, int(fmt_at(t.w1_fmt, l)))
        bytes += moe_gpu_plane_bytes(c.dim, hid, int(fmt_at(t.w3_fmt, l)))
        bytes += moe_gpu_plane_bytes(hid, c.dim, int(fmt_at(t.w2_fmt, l)))
    }
    to_log(LOG_WARNING, "dasLLAMA: GPU MoE tier: the dense FFN of {c.n_layers} layers ({bytes >> 20l} MB) stays on the CPU - the per-op tier has no dense-FFN rail, only the whole-model resident driver serves it\n")
}

// Upload the offloaded layers' expert stacks to the GPU tier from the PREPARED planes. Called at
// the end of every load rail so DASLLAMA_GPU_MOE_LAYERS behaves identically however the model
// arrived. Layers go resident from the end while the tier's VRAM budget holds.
[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span"), arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#gpu-slot-marks")]
def moe_gpu_upload_resident(t : Model) {   // nolint:STYLE037,STYLE038 — seven sequential rail walks sharing one rollback cursor; splitting them hides the partial-group unwind
    moe_gpu_drop_installed_before_load(t.config.n_layers)
    reset_gpu_cpu_passes()
    set_moe_gpu_from_layer(t.config.n_layers)   // nothing resident until an upload lands
    set_moe_gpu_stream_from(t.config.n_layers)
    set_moe_gpu_total_layers(t.config.n_layers)
    set_moe_gpu_cls_off(-1l)
    reset_moe_gpu_dense_offs()
    reset_moe_gpu_dn_layers()
    reset_moe_gpu_attn_layers()
    reset_moe_gpu_shexp_layers()
    reset_moe_gpu_qkv_layers()
    set_moe_gpu_model_support("")
    // -1 = auto: ask for every layer and let the walk's own budget stops place the split (the
    // resident loop drops a partial triple and breaks, the streamed loop does the same on the
    // pinned-mirror budget) — that is what makes auto degrade on a smaller card instead of failing
    let nstream_want = gpu_want_moe_stream()
    let nstream = nstream_want < 0l ? t.config.n_layers : nstream_want
    let dn_want = gpu_want_dn() || gpu_want_dnd()   // the decode step rides the same dn uploads
    let at_want = gpu_want_attn()
    // layer count tests == 0, NOT <= 0: -1 is the auto sentinel and must read as a REQUEST
    set_moe_gpu_stream_need(0l)   // both needs are THIS model's: a load that carves nothing leaves nothing behind
    set_moe_gpu_dat_need(0l)
    if (!moe_gpu_tier_installed()
            || (moe_gpu_layer_count() == 0l && nstream <= 0l && !dn_want && !at_want
                && !gpu_want_shexp() && !gpu_want_qkv() && !gpu_want_cls())) {
        return   // tier absent or nothing requested
    }
    // models that fully fit, MoE included: the whole-stack resident driver is tried FIRST - it
    // subsumes every rail below; a decline falls through
    let resident_try = (moe_gpu_resident_installed() && gpu_resident_requested() && gpu_want_resident()
        && (t.config.n_expert == 0l || rdec_moe_installed()))
    // the mirror codec: f16 rows by default (native fa feed, ~2x plannable ctx);
    // DASLLAMA_VK_KV32=1 arms f32 mirrors instead (A/B; then only f32 sessions serve)
    let rkdt = g_env_vulkan.vk_kv32 != 0l ? KVDtype.f32 : KVDtype.f16
    var seq_cap = t.config.seq_len
    let ctx_max = g_rdec_ctx_max > 0l ? g_rdec_ctx_max : g_env_gpu.gpu_ctx_max
    if (ctx_max > 0l && ctx_max < seq_cap) {   // a mirror sized to a 131072-token model max is 8 GB on a 4B: the rigs pin 32768
        seq_cap = max(ctx_max, rdec_min_ctx())
        to_log(LOG_INFO, "dasLLAMA: resident driver - KV mirror capped at {seq_cap} positions (DASLLAMA_GPU_CTX_MAX, or set_gpu_ctx_max in process; 0 lifts the cap)\n")
    }
    var plan : ResidentPlan
    if (resident_try) {
        let bctx = resident_binding_ctx(t, rkdt, moe_gpu_binding_cap(g_rdec_regions_want))
        seq_cap = resident_region_ctx(seq_cap, bctx, g_rdec_regions_want)
        plan = resident_plan(t, seq_cap, rkdt, rkdt)
        if (!plan.fits && plan.weight_bytes > 0l && plan.kv_bytes > 0l) {
            // weights are fixed, the KV mirror is not: retry at the context that fits (the plan's
            // own remedy), as long as it stays a usable window. The driver guards decode past the cap.
            //! seven eighths of the room: a mirror sized to the last byte fails on a heap the previous model fragmented
            let fit_ctx = (max(plan.usable_bytes - plan.weight_bytes - plan.scratch_bytes, 0l) / 8l * 7l) / (plan.kv_bytes / seq_cap)
            if (fit_ctx >= rdec_min_ctx()) {
                seq_cap = fit_ctx
                plan = resident_plan(t, seq_cap, rkdt, rkdt)
            }
        }
    }
    let resident_fits = resident_try && plan.fits
    // size the tier's streamed-slot reserve from THIS model's largest expert layer — a fixed
    // reserve wastes VRAM or asserts on big-expert geometries (GLM Air, Qwen1.5-MoE); must
    // precede the first upload (the tier carves the reserve at its lazy init)
    if (t.config.n_expert > 0l && !resident_fits) {
        var sgu = 0l
        var sdn = 0l
        for (sl in range64(t.config.n_layers + t.config.n_layer_nextn)) {
            if (sl < t.config.n_layer_dense_lead || long_length(t.we1_offs) <= sl
                    || t.we1_offs[sl] < 0l || t.we2_offs[sl] < 0l || t.we3_offs[sl] < 0l) {
                continue
            }
            let sne = t.config.n_expert
            let snfe = t.config.n_ff_exp
            sgu = max(sgu, moe_gpu_plane_bytes(t.config.dim, sne * snfe, int(rdec_efmt(t, t.we1_fmt, sl))))
            sgu = max(sgu, moe_gpu_plane_bytes(t.config.dim, sne * snfe, int(rdec_efmt(t, t.we3_fmt, sl))))
            sdn = max(sdn, moe_gpu_plane_bytes(snfe, sne * t.config.dim, int(rdec_efmt(t, t.we2_fmt, sl))))
        }
        set_moe_gpu_stream_need(sgu > 0l ? 2l * sgu + sdn : 0l)
    }
    if (at_want && !resident_fits) {   // the decode block's mirrors for any model whose quads ride the tier, the span's router planes for a MoE one
        set_moe_gpu_dat_need(t.config.n_layers * DAT_MIRROR_ROWS * layer_kv_dim(t.config, 0l) * 4l
            + (t.config.n_expert > 0l ? t.config.n_layers * t.config.n_expert * t.config.dim * 4l : 0l))
    }
    if (resident_try) {
        if (plan.fits) {
            if (resident_upload(t, seq_cap, rkdt, plan)) {
                return
            }
            if (t.config.n_expert > 0l) {
                to_log(LOG_WARNING, "dasLLAMA: the per-op rails serve this MoE without the streamed slot or the decode mirrors the fitting plan forwent - layers the budget stops keep the CPU\n")
            }
        } else {
            rdec_decline_say(empty(plan.reason) ? "the residency plan does not fit" : plan.reason, plan.memory)
        }
    } elif (moe_gpu_resident_installed() && !gpu_want_resident()) {
        g_rdec_decline_reason = "not attempted: the per-op rails were asked for (DASLLAMA_GPU_RESIDENT=0)"
        to_log(LOG_INFO, "dasLLAMA: resident driver not attempted - the per-op rails were asked for (DASLLAMA_GPU_RESIDENT=0, or set_gpu_resident_route(false) in process)\n")
    } elif (moe_gpu_resident_installed() && t.config.n_expert > 0l && !rdec_moe_installed()) {
        g_rdec_decline_reason = "not attempted: the tier installed no seats for routed experts"
        to_log(LOG_INFO, "dasLLAMA: resident driver not attempted - the tier installed no MoE seats for its {t.config.n_expert} routed experts; the per-op tier's expert rails serve\n")
    } elif (moe_gpu_resident_installed() && t.config.n_expert == 0l) {
        g_rdec_decline_reason = "not attempted: nothing asked for the whole-model driver"
        to_log(LOG_INFO, "dasLLAMA: resident driver not attempted - nothing asked for it: DASLLAMA_GPU is not set and the want carries neither auto_tier nor resident (the individual DASLLAMA_GPU_* rails alone are armed)\n")
    }
    // capability guard: the tier needs q8 serving mode — record WHY for the status surface when it
    // is armed but this model cannot ride it. Dense archs pass: every non-expert rail (cls, dense
    // planes, attn quads, qkv) keys on wq/wk/wv/wo/wcls offsets and never on experts.
    var reason = ""
    if (t.quant != QuantMode.q8) {
        reason = "GPU tier needs q8 serving mode"
    } elif (t.experts_mx4) {
        reason = "mx4 expert planes are not tier-servable"
    } elif (t.config.moe_exps_bias) {
        reason = "biased expert stacks are not tier-servable"
    }
    if (!empty(reason)) {
        set_moe_gpu_model_support(reason)
        to_log(LOG_WARNING, "dasLLAMA: GPU tier armed but idle - {reason}\n")
        return
    }
    let ts_gpu = ref_time_ticks()
    let layers = t.config.n_layers
    let dim = t.config.dim
    let ne = t.config.n_expert
    let nfe = t.config.n_ff_exp
    let want_raw = moe_gpu_layer_count()
    // a dense arch has no expert stacks at all — we*_offs are never populated (they are built only
    // under n_expert > 0), so the expert + streamed walks below MUST resolve to zero iterations
    let has_experts = ne > 0l
    let want = has_experts ? (want_raw < 0l ? layers : min(want_raw, layers)) : 0l   // -1 = auto: fill the budget
    set_moe_gpu_layer_count(want)   // publish the resolved count — downstream status reads it
    let repacked = kernel_backend_needs_repack(active_kernel_backend())
    let mr = repacked ? active_q8_layout_mr() : 1l
    let kgroup = repacked ? active_q8_kgroup() : 4l
    let wbias = repacked ? active_q8_wbias() : 0l
    var from_l = layers
    var wq : array<uint8>
    var ws : array<uint8>
    // classifier plane first — the board's best ms-per-GB region (one whole-vocab GEMV serves
    // every token). One slice: the repack groups the plane globally, tail rows disk-order.
    // DASLLAMA_GPU_CLS=0 keeps it on the CPU (A/B lever).
    let cls_fmt = t.config.shared_weights ? t.emb_fmt : t.wcls_fmt
    let cls_servable = cls_fmt == KqFmt.q8 ? (t.cls_q8 || !t.config.shared_weights) : kq_sb(cls_fmt)
    vulkan_bake_role(VkBakeRole.cls)
    if (cls_servable && gpu_want_cls_upload()
            && moe_gpu_gather_upload(t, cls_fmt, t.wcls_off, dim, t.config.vocab_size, t.config.vocab_size, mr, kgroup, wbias, wq, ws)) {
        set_moe_gpu_cls_off(t.wcls_off)
    }
    // dense (attention-side) planes second — DASLLAMA_GPU_DENSE=1 / gpu_dense opt-in. Arch-agnostic:
    // recurrent layers carry the deltanet triple, attention layers the q/o pair (k/v stay CPU — small).
    // Layer-ascending; each successful upload marks its plane, a budget stop ends the walk.
    vulkan_bake_role(VkBakeRole.dense)
    if (gpu_want_dense()) {
        var dense_marked = 0l
        var dense_stop = false
        var dense_skips : WalkSkips
        for (l in range64(layers)) {
            var uploads : array<tuple<f : KqFmt; woff : int64; n : int64; rows : int64>>
            if (layer_is_recurrent(t.config, l)) {
                // kq-tagged dn planes (CPU-rail loads) are not tier-servable — the dn chain reads q8
                if (t.dnqkv_offs[l] < 0l || t.dngate_offs[l] < 0l || t.dnout_offs[l] < 0l) {
                    walk_skip(dense_skips, l, "recurrent, no deltanet planes")
                } elif (!dn_layer_q8(t, l)) {
                    walk_skip(dense_skips, l, "recurrent, kq-tagged deltanet planes (CPU rail)")
                } else {
                    uploads |> push((f = KqFmt.q8, woff = t.dnqkv_offs[l], n = dim, rows = dn_conv_dim(t.config)))
                    uploads |> push((f = KqFmt.q8, woff = t.dngate_offs[l], n = dim, rows = t.config.ssm_d_inner))
                    uploads |> push((f = KqFmt.q8, woff = t.dnout_offs[l], n = t.config.ssm_d_inner, rows = dim))
                }
            } elif (gpu_want_dense_attn()) {
                let qd = layer_qd(t.config, l)
                let fq = fmt_at(t.wq_fmt, l)
                let fo = fmt_at(t.wo_fmt, l)
                if (t.wq_offs[l] < 0l || t.wo_offs[l] < 0l) {
                    walk_skip(dense_skips, l, "no q/o planes")
                } elif (!((fq == KqFmt.q8 || kq_sb(fq)) && (fo == KqFmt.q8 || kq_sb(fo))
                        && kq_servable(t, fq) && kq_servable(t, fo))) {
                    walk_skip(dense_skips, l, "q/o formats {int(fq)}/{int(fo)} not tier-servable")
                } else {
                    uploads |> push((f = fq, woff = t.wq_offs[l], n = dim, rows = t.config.q_gated ? 2l * qd : qd))
                    uploads |> push((f = fo, woff = t.wo_offs[l], n = qd, rows = dim))
                }
            }
            // gemma-4's parallel dense shared expert triple — opt-in, measured a wash (see the gate's doc)
            if (dense_shexp_uploadable(t, l)) {
                let hid = empty(t.ffn_w) ? t.config.hidden_dim : t.ffn_w[l]
                uploads |> push((f = fmt_at(t.w1_fmt, l), woff = t.w1_offs[l], n = dim, rows = hid))
                uploads |> push((f = fmt_at(t.w3_fmt, l), woff = t.w3_offs[l], n = dim, rows = hid))
                uploads |> push((f = fmt_at(t.w2_fmt, l), woff = t.w2_offs[l], n = hid, rows = dim))
            }
            for (u in uploads) {
                if (!moe_gpu_gather_upload(t, u.f, u.woff, u.n, u.rows, u.rows, mr, kgroup, wbias, wq, ws)) {
                    dense_stop = true   // VRAM budget hit — later planes keep the CPU dense path
                    walk_budget_say("dense rail", l, layers)
                    break
                }
                set_moe_gpu_dense_off(int(u.f), u.woff)
                dense_marked++
            }
            delete uploads
            break if (dense_stop)
        }
        if (dense_marked > 0l) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: {dense_marked} dense planes resident\n")
        }
        walk_say(dense_skips, "dense rail")
    }
    // shared-expert triples — on with the tier (DASLLAMA_GPU_SHEXP overrides). Dense (every token,
    // every layer, one fixed weight set) so residency carries no miss term; q8 only (the repack
    // registers them fmt 0).
    vulkan_bake_role(VkBakeRole.shexp)
    if (gpu_want_shexp() && t.config.n_ff_shexp > 0l && !t.config.moe_dense_shexp) {
        let nsh = t.config.n_ff_shexp
        let she = dim * nsh
        var sh_marked = 0l
        for (l in range64(layers)) {
            var uploads <- [(woff = t.wsh1_off + l * she, n = dim, rows = nsh),
                            (woff = t.wsh3_off + l * she, n = dim, rows = nsh),
                            (woff = t.wsh2_off + l * she, n = nsh, rows = dim)]
            var ok = true
            var done = 0
            for (u in uploads) {
                if (!moe_gpu_gather_upload(t, KqFmt.q8, u.woff, u.n, u.rows, u.rows, mr, kgroup, wbias, wq, ws)) {
                    ok = false
                    break
                }
                done++
            }
            if (!ok) {
                moe_gpu_drop_stacks(done, false)   // partial triple rolls back (the resident-walk rule)
                delete uploads
                break
            }
            //! the shexp mark, not set_moe_gpu_dense_off: the shared triple rides the expert chain, not the dense rail
            set_moe_gpu_shexp_layer(l)
            sh_marked++
            delete uploads
        }
        if (sh_marked > 0l) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: shared experts of {sh_marked} layers resident\n")
        }
    }
    // deltanet triples third — DASLLAMA_GPU_DN=1 opt-in: each recurrent layer's qkv/z-gate/
    // out-proj planes + the layer mark that routes its whole prefill attention through the
    // device chain. Layer-ascending; a budget stop leaves later layers CPU, a partial triple rolls back.
    vulkan_bake_role(VkBakeRole.dn)
    if (dn_want) {
        var dn_marked = 0l
        var dn_stop = false
        var dn_skips : WalkSkips
        for (l in range64(layers)) {
            continue if (!layer_is_recurrent(t.config, l))
            if (t.dnqkv_offs[l] < 0l || t.dngate_offs[l] < 0l || t.dnout_offs[l] < 0l) {
                walk_skip(dn_skips, l, "no deltanet planes")
                continue
            }
            if (!dn_layer_q8(t, l)) {
                walk_skip(dn_skips, l, "kq-tagged deltanet planes (CPU rail)")
                continue
            }
            // the alias needs the WHOLE triple: the dense walk marks per-plane and its budget stop
            // can land mid-triple — a lone dnqkv mark would route the serve into a non-resident panic
            if (moe_gpu_dense_on_gpu(int(KqFmt.q8), t.dnqkv_offs[l])
                    && moe_gpu_dense_on_gpu(int(KqFmt.q8), t.dngate_offs[l])
                    && moe_gpu_dense_on_gpu(int(KqFmt.q8), t.dnout_offs[l])) {
                set_moe_gpu_dn_layer(l, t.dnqkv_offs[l])
                dn_marked++
                continue
            }
            let cd = dn_conv_dim(t.config)
            let di = t.config.ssm_d_inner
            var got = 0
            if (moe_gpu_gather_upload(t, KqFmt.q8, t.dnqkv_offs[l], dim, cd, cd, mr, kgroup, wbias, wq, ws)) {
                got++
            }
            if (got == 1 && moe_gpu_gather_upload(t, KqFmt.q8, t.dngate_offs[l], dim, di, di, mr, kgroup, wbias, wq, ws)) {
                got++
            }
            if (got == 2 && moe_gpu_gather_upload(t, KqFmt.q8, t.dnout_offs[l], di, dim, dim, mr, kgroup, wbias, wq, ws)) {
                got++
            }
            if (got < 3) {
                if (got > 0) {
                    moe_gpu_drop_stacks(got, false)
                }
                dn_stop = true
                walk_budget_say("deltanet rail", l, layers)
            }
            break if (dn_stop)
            set_moe_gpu_dn_layer(l, t.dnqkv_offs[l])
            dn_marked++
        }
        if (dn_marked > 0l) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: deltanet triples of {dn_marked} layers resident\n")
        }
        walk_say(dn_skips, "deltanet rail")
    }
    // attention quads fourth — DASLLAMA_GPU_ATTN=1 opt-in: each servable full-attention layer's
    // q|gate/k/v/wo planes + the layer mark that routes its whole prefill attention through the
    // device chain. Layer-ascending; a budget stop rolls back the partial quad.
    vulkan_bake_role(VkBakeRole.attn)
    if (at_want) {
        let c = t.config
        let arch_ok = empty(attn_chain_unserved_features(c))
        var at_marked = 0l
        var at_stop = false
        var at_skips : WalkSkips
        if (!arch_ok) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: the attention-quad rail declined - no arm for: {attn_chain_unserved_features(c)}\n")
        }
        for (l in range64(layers)) {
            break if (!arch_ok)
            continue if (layer_is_recurrent(c, l))
            if (layer_is_sliding(c, l)) {
                walk_skip(at_skips, l, "sliding window")
                continue
            }
            if (t.kv_src[l] != l) {
                walk_skip(at_skips, l, "shares layer {t.kv_src[l]}'s K/V")
                continue
            }
            if (t.wq_offs[l] < 0l || t.wk_offs[l] < 0l || t.wv_offs[l] < 0l || t.wo_offs[l] < 0l) {
                walk_skip(at_skips, l, "lacks a q/k/v/o plane")
                continue
            }
            let hs = layer_head_size(c, l)
            let kvd = layer_kv_dim(c, l)
            let qd = c.n_heads * hs
            let rot = c.rope_dim > 0l ? c.rope_dim : hs
            let fq = fmt_at(t.wq_fmt, l)
            let fk = fmt_at(t.wk_fmt, l)
            let fv = fmt_at(t.wv_fmt, l)
            let fo = fmt_at(t.wo_fmt, l)
            // q/k/v share the chain's one activation image — format class must agree; wo is free
            let kq_cls = fq != KqFmt.q8
            var fmts_ok = (fk != KqFmt.q8) == kq_cls && (fv != KqFmt.q8) == kq_cls
            for (f in [fq, fk, fv, fo]) {
                fmts_ok = fmts_ok && (f == KqFmt.q8 || kq_sb(f)) && kq_servable(t, f)
            }
            if (!fmts_ok) {
                walk_skip(at_skips, l, "q/k/v/o formats {int(fq)}/{int(fk)}/{int(fv)}/{int(fo)} not tier-servable as one chain")
                continue
            }
            if (!(hs == 128l || hs == 256l) || qd > 8192l || kvd > 2048l
                    || c.n_heads > 64l || rot % 2l != 0l || rot > hs || kvd % 32l != 0l
                    || qd % (fo != KqFmt.q8 ? 256l : 32l) != 0l || dim % 32l != 0l) {
                walk_skip(at_skips, l, "geometry outside the chain (hs {hs}, qd {qd}, kvd {kvd}, heads {c.n_heads}, rot {rot})")
                continue
            }
            var uploads : array<tuple<f : KqFmt; woff : int64; n : int64; rows : int64>>
            if (!moe_gpu_dense_on_gpu(int(fq), t.wq_offs[l])) {
                uploads |> push((f = fq, woff = t.wq_offs[l], n = dim, rows = c.q_gated ? 2l * qd : qd))
            }
            uploads |> push((f = fk, woff = t.wk_offs[l], n = dim, rows = kvd))
            uploads |> push((f = fv, woff = t.wv_offs[l], n = dim, rows = kvd))
            if (!moe_gpu_dense_on_gpu(int(fo), t.wo_offs[l])) {
                uploads |> push((f = fo, woff = t.wo_offs[l], n = qd, rows = dim))
            }
            var got = 0
            for (u in uploads) {
                if (!moe_gpu_gather_upload(t, u.f, u.woff, u.n, u.rows, u.rows, mr, kgroup, wbias, wq, ws)) {
                    at_stop = true
                    break
                }
                got++
            }
            delete uploads
            if (at_stop) {
                if (got > 0) {
                    moe_gpu_drop_stacks(got, false)
                }
                walk_budget_say("attention-quad rail", l, layers)
                break
            }
            set_moe_gpu_attn_layer(l)
            at_marked++
        }
        if (at_marked > 0l) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: attention quads of {at_marked} layers resident\n")
        }
        walk_say(at_skips, "attention-quad rail")
    }
    // attention q/k/v triples — DASLLAMA_GPU_QKV=1 opt-in. LAST of the dense rails ON PURPOSE: it
    // is the lowest-value one and an earlier section exhausting stacks/VRAM starves those behind it
    // (a 224-stack cap once cut 6 DN layers = -14% tg). Pays only where GPU reads beat CPU reads.
    vulkan_bake_role(VkBakeRole.qkv)
    if (gpu_want_qkv()) {
        var qkv_marked = 0l
        var qkv_skips : WalkSkips
        for (l in range64(layers)) {
            continue if (layer_is_recurrent(t.config, l) || t.kv_src[l] != l || moe_gpu_attn_on_gpu(l))
            let qd = layer_qd(t.config, l)
            let kv = layer_kv_dim(t.config, l)
            let fq = fmt_at(t.wq_fmt, l)
            let fk = fmt_at(t.wk_fmt, l)
            let fv = fmt_at(t.wv_fmt, l)
            // one shared activation image ⇒ the three planes must agree on quant CLASS
            let cls_ok = (fq == KqFmt.q8) == (fk == KqFmt.q8) && (fq == KqFmt.q8) == (fv == KqFmt.q8)
            if (t.wq_offs[l] < 0l || t.wk_offs[l] < 0l || t.wv_offs[l] < 0l || !cls_ok
                    || !kq_servable(t, fq) || !kq_servable(t, fk) || !kq_servable(t, fv)) {
                walk_skip(qkv_skips, l, "q/k/v formats {int(fq)}/{int(fk)}/{int(fv)} not tier-servable as one image")
                continue
            }
            var uploads <- [(f = fq, woff = t.wq_offs[l], n = dim, rows = t.config.q_gated ? 2l * qd : qd),
                            (f = fk, woff = t.wk_offs[l], n = dim, rows = kv),
                            (f = fv, woff = t.wv_offs[l], n = dim, rows = kv)]
            var done = 0
            var ok = true
            for (u in uploads) {
                if (!moe_gpu_gather_upload(t, u.f, u.woff, u.n, u.rows, u.rows, mr, kgroup, wbias, wq, ws)) {
                    ok = false
                    break
                }
                done++
            }
            delete uploads
            if (!ok) {
                moe_gpu_drop_stacks(done, false)   // partial triple rolls back
                walk_budget_say("q/k/v rail", l, layers)
                break
            }
            set_moe_gpu_qkv_layer(l)
            qkv_marked++
        }
        if (qkv_marked > 0l) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: q/k/v of {qkv_marked} attention layers resident\n")
        }
        walk_say(qkv_skips, "q/k/v rail")
    }
    vulkan_bake_role(VkBakeRole.expert)
    for (i in range64(want)) {
        let l = layers - 1l - i
        let f1 = rdec_efmt(t, t.we1_fmt, l)
        let f2 = rdec_efmt(t, t.we2_fmt, l)
        let f3 = rdec_efmt(t, t.we3_fmt, l)
        // gate/up must share a form (one activation image); down is free. The Q4_K_M MoE
        // recipe bumps ffn_down_exps to Q5_0/Q8_0, so k4/k4/q8 is the COMMON shape (glm4moe,
        // gemma). The walk stops on refusal so the resident set stays contiguous from the end.
        let gu_ok = ((f1 == KqFmt.q8) == (f3 == KqFmt.q8)
            && (f1 == KqFmt.q8 || (kq_sb(f1) && kq_sb(f3))))
        let dn_ok = f2 == KqFmt.q8 || kq_sb(f2)
        break if (l < t.config.n_layer_dense_lead || t.we1_offs[l] < 0l)
        if (!(gu_ok && dn_ok)) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: the expert walk stopped at layer {l} - gate/up/down formats {int(f1)}/{int(f3)}/{int(f2)} cannot share the tier's activation image; layers 0..{l} stay on the CPU\n")
            break
        }
        var got = 0
        if (moe_gpu_gather_upload(t, f1, t.we1_offs[l], dim, ne * nfe, nfe, mr, kgroup, wbias, wq, ws)) {
            got++
        }
        if (got == 1 && moe_gpu_gather_upload(t, f3, t.we3_offs[l], dim, ne * nfe, nfe, mr, kgroup, wbias, wq, ws)) {
            got++
        }
        if (got == 2 && moe_gpu_gather_upload(t, f2, t.we2_offs[l], nfe, ne * dim, dim, mr, kgroup, wbias, wq, ws)) {
            got++
        }
        if (got < 3) {
            // VRAM budget hit — this and lower layers stay on CPU. A partial triple MUST roll
            // back: a dangling gate/up plane hijacks the tier's residency check when this layer
            // re-enters as streamed (or CPU prefill probes it) — non-resident-region panic
            if (got > 0) {
                moe_gpu_drop_stacks(got, false)
            }
            break
        }
        from_l = l
    }
    // streamed tail: DASLLAMA_GPU_MOE_STREAM=N marks the next N layers below the resident set for
    // prefill streaming — device-layout planes gathered into pinned host mirrors the tier DMAs per
    // prefill. CPU planes stay (decode runs them); the walk is contiguous descending.
    vulkan_bake_role(VkBakeRole.expert_stream)
    var sfrom = from_l
    for (i in range64(has_experts ? max(nstream, 0l) : 0l)) {
        let l = from_l - 1l - i
        if (l < t.config.n_layer_dense_lead || t.we1_offs[l] < 0l) {
            break
        }
        let f1 = rdec_efmt(t, t.we1_fmt, l)
        let f2 = rdec_efmt(t, t.we2_fmt, l)
        let f3 = rdec_efmt(t, t.we3_fmt, l)
        // same rule as the resident walk: gate/up share one activation image, down is free
        let gu_ok = ((f1 == KqFmt.q8) == (f3 == KqFmt.q8)
            && (f1 == KqFmt.q8 || (kq_sb(f1) && kq_sb(f3))))
        if (!(gu_ok && (f2 == KqFmt.q8 || kq_sb(f2)))) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: the streamed walk stopped at layer {l} - gate/up/down formats {int(f1)}/{int(f3)}/{int(f2)} cannot share the tier's activation image; layers 0..{l} stay on the CPU\n")
            break
        }
        var got = 0
        if (moe_gpu_gather_upload(t, f1, t.we1_offs[l], dim, ne * nfe, nfe, mr, kgroup, wbias, wq, ws, [stream = true])) {
            got++
        }
        if (got == 1 && moe_gpu_gather_upload(t, f3, t.we3_offs[l], dim, ne * nfe, nfe, mr, kgroup, wbias, wq, ws, [stream = true])) {
            got++
        }
        if (got == 2 && moe_gpu_gather_upload(t, f2, t.we2_offs[l], nfe, ne * dim, dim, mr, kgroup, wbias, wq, ws, [stream = true])) {
            got++
        }
        if (got < 3) {
            // pinned-mirror budget hit — this and lower layers stay CPU-only. Roll back a
            // partial triple: the stream registry's group arithmetic assumes whole gate/up/down
            // triples, and a phantom gate would match the group scan at dispatch
            if (got > 0) {
                moe_gpu_drop_stacks(got, true)
            }
            break
        }
        sfrom = l
    }
    delete wq
    delete ws
    set_moe_gpu_from_layer(from_l)
    set_moe_gpu_stream_from(sfrom)
    if (from_l < layers || moe_gpu_cls_on_gpu(t.wcls_off)) {
        let cls_note = moe_gpu_cls_on_gpu(t.wcls_off) ? "classifier + " : ""
        let stream_note = sfrom < from_l ? ", [{sfrom}..{from_l}) streamed" : ""
        to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: {cls_note}expert stacks of layers [{from_l}..{layers}) resident{stream_note}, {get_time_usec(ts_gpu) / 1000} ms\n")
        if (moe_gpu_span_ok()) {
            select_decode_override("vulkan_moe_span")
        }
    }
    if (!has_experts) {
        dense_ffn_cpu_say(t)
    }
}
