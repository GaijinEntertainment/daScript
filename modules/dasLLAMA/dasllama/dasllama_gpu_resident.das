options gen2
options _comment_hygiene = true

module dasllama_gpu_resident shared public

// The whole-model GPU RESIDENCY rail: bake the device layout offline, upload a model's stacks to
// the tier, and drive decode/prefill entirely on device. It reaches the engine only through
// contracts dasllama_common already publishes (the decode/prefill/batch override registries) and
// the device SPI in dasllama_gpu_tier — no device module is required here, so this file compiles
// on every box. "vulkan" is the tier string it registers under, not a dependency.

require dasllama/dasllama_common
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_kqformat
require dasllama/dasllama_math
require dasllama/dasllama_rope        // build_rope_row_packed — the one source for every resident rope row
require dasllama/dasllama_gemm_schema // Q8_BLOCK_ELEMS / KQ_SUPERBLOCK_ELEMS — device block sizing
require llvm/daslib/f16_cvt   // f32_to_f16 — the trim pass re-packs the emb scale plane
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
    wq |> resize(e.wq_bytes)
    ws |> resize(e.ws_bytes)
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
    if (t.cls_q8) {
        // mblob-style 34B interleaved blocks over the emb region (dequant_embq_row's layout)
        let nb = vocab * dim / 32l
        t.embq |> resize(nb * 34l)
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
        unsafe {
            if (t.emb_fmt == KqFmt.k4) {
                t.embq |> resize(nsb * K4_QSB)
                t.embs |> resize(nsb * K4_SSB)
                memcpy(addr<void?>(t.embq[0]), addr<void?>(t.k4q[sb0 * K4_QSB]), nsb * K4_QSB)
                memcpy(addr<void?>(t.embs[0]), addr<void?>(t.k4s[sb0 * K4_SSB]), nsb * K4_SSB)
            } elif (t.emb_fmt == KqFmt.k5) {
                t.embq |> resize(nsb * K5_QSB)
                t.embs |> resize(nsb * K5_SSB)
                memcpy(addr<void?>(t.embq[0]), addr<void?>(t.k5q[sb0 * K5_QSB]), nsb * K5_QSB)
                memcpy(addr<void?>(t.embs[0]), addr<void?>(t.k5s[sb0 * K5_SSB]), nsb * K5_SSB)
            } elif (t.emb_fmt == KqFmt.k6) {
                t.embq |> resize(nsb * K6_QSB)
                t.embs |> resize(nsb * K6_SSB)
                memcpy(addr<void?>(t.embq[0]), addr<void?>(t.k6q[sb0 * K6_QSB]), nsb * K6_QSB)
                memcpy(addr<void?>(t.embs[0]), addr<void?>(t.k6s[sb0 * K6_SSB]), nsb * K6_SSB)
            } elif (t.emb_fmt == KqFmt.q40) {
                t.embq |> resize(nsb * Q40_QSB)
                t.embs |> resize(nsb * Q40_SSB)
                memcpy(addr<void?>(t.embq[0]), addr<void?>(t.q40q[sb0 * Q40_QSB]), nsb * Q40_QSB)
                memcpy(addr<void?>(t.embs[0]), addr<void?>(t.q40s[sb0 * Q40_SSB]), nsb * Q40_SSB)
            } else {
                to_log(LOG_WARNING, "dasLLAMA trim: emb fmt '{t.emb_fmt}' has no kq plane pair - declining\n")
                return false
            }
        }
    }   // fp32 embeddings ride fblob, which the trim keeps
    t.planes_trimmed = true
    delete t.qblob
    delete t.qscales
    delete t.qscales16
    delete t.wblob
    delete t.bf16blob
    delete t.k4q
    delete t.k4s
    delete t.k5q
    delete t.k5s
    delete t.k6q
    delete t.k6s
    delete t.q40q
    delete t.q40s
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
    headroom_bytes : int64  //!< desktop slack the auto arm leaves unfilled (0 when VRAM is pinned)
    budget_bytes : int64
    fits : bool
    reason : string       //!< "" when it fits; otherwise why, with a remedy where one exists
}

let private RDEC_PF_ROWS = 512l                    // mirrors PF_WINDOW (dasllama_vulkan_prefill)
let private RDEC_VRAM_HEADROOM = 2_147_483_648l    // auto-arm slack past the tier's desktop reserve:
                                                   // filling weights + KV to the cap brim measured a
                                                   // 12x WDDM-demotion collapse on a live desktop
let private RDEC_MISC_BYTES = 67_108_864l          // decode smalls + batch act planes + metas

// the driver's device allocations past weights + KV — the prefill window buffers dominate.
// An estimate, deliberately a touch high; the flat term carries the decode/batch smalls.
def private rdec_scratch_bytes(t : Model) : int64 {
    let c = t.config
    let qd = layer_qd(c, 0l)
    let hid = layer_hidden(t, 0l)
    let wide = max(max(c.dim, qd), hid)
    return RDEC_PF_ROWS * (16l * c.dim + 2l * wide + 10l * qd + 12l * layer_kv_dim(c, 0l)
        + 8l * hid + 4l * layer_head_size(c, 0l)) + c.vocab_size * 4l + RDEC_MISC_BYTES
}

//! Size `t` for the resident driver at `seq_cap` context. Declines (fits = false) carry a reason:
//! the commonest support question on this tier is "why did it decline", so the answer is the API.
def resident_plan(t : Model; seq_cap : int64; kdt, vdt : KVDtype) : ResidentPlan {
    let c = t.config
    var p = ResidentPlan(budget_bytes = moe_gpu_weight_budget())
    if (p.budget_bytes <= 0l) {
        p.reason = "no GPU tier armed"
        return p
    }
    if (c.n_expert > 0l) {
        p.reason = "MoE - the per-op offload tier serves routed experts, not the resident driver"
        return p
    }
    // KV is reserved BEFORE weights and never grows: on a discrete card it competes directly with
    // them, and evicting weights to grow it would mean re-uploading gigabytes.
    var krow = 0l
    var vrow = 0l
    for (l in range64(c.n_layers)) {
        krow += kv_row_bytes(kdt, layer_kv_dim(c, l))
        vrow += kv_row_bytes(vdt, layer_kv_dim(c, l))
    }
    p.kv_bytes = seq_cap * (krow + vrow)
    p.scratch_bytes = rdec_scratch_bytes(t)
    // a pinned VRAM_MB is the user's contract — fill it; the auto arm keeps desktop slack
    p.headroom_bytes = gpu_want_vram_mb() > 0l ? 0l : RDEC_VRAM_HEADROOM
    // every 2D plane a dense forward reads: the attention quad, the FFN triple, the classifier
    for (l in range64(c.n_layers)) {
        let qd = layer_qd(c, l)
        let kvd = layer_kv_dim(c, l)
        let hid = layer_hidden(t, l)
        if (t.wq_offs[l] >= 0l) {
            p.weight_bytes += moe_gpu_plane_bytes(c.dim, qd * (c.q_gated ? 2l : 1l), int(fmt_at(t.wq_fmt, l)))
        }
        if (t.wk_offs[l] >= 0l) {
            p.weight_bytes += moe_gpu_plane_bytes(c.dim, kvd, int(fmt_at(t.wk_fmt, l)))
        }
        if (t.wv_offs[l] >= 0l) {
            p.weight_bytes += moe_gpu_plane_bytes(c.dim, kvd, int(fmt_at(t.wv_fmt, l)))
        }
        if (t.wo_offs[l] >= 0l) {
            p.weight_bytes += moe_gpu_plane_bytes(qd, c.dim, int(fmt_at(t.wo_fmt, l)))
        }
        if (t.w1_offs[l] >= 0l) {
            p.weight_bytes += moe_gpu_plane_bytes(c.dim, hid, int(fmt_at(t.w1_fmt, l)))
            p.weight_bytes += moe_gpu_plane_bytes(c.dim, hid, int(fmt_at(t.w3_fmt, l)))
            p.weight_bytes += moe_gpu_plane_bytes(hid, c.dim, int(fmt_at(t.w2_fmt, l)))
        }
    }
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    p.weight_bytes += moe_gpu_plane_bytes(c.dim, c.vocab_size, int(cls_fmt))
    let total = p.weight_bytes + p.kv_bytes + p.scratch_bytes
    let usable = p.budget_bytes - p.headroom_bytes
    p.fits = total <= usable
    if (!p.fits) {
        // a shorter context is the remedy that actually works — weights are fixed, KV is not
        let kv_fit = max(usable - p.weight_bytes - p.scratch_bytes, 0l)
        let cap_fit = (krow + vrow) > 0l ? kv_fit / (krow + vrow) : 0l
        p.reason = (cap_fit >= 512l
            ? "needs {total >> 20l} MB of {usable >> 20l} MB at ctx {seq_cap} - would fit at ctx {cap_fit}"
            : "needs {total >> 20l} MB of {usable >> 20l} MB (weights alone {p.weight_bytes >> 20l} MB)")
    }
    return p
}

// ===== the resident whole-stack decode driver (dense models that fully fit) =====

var private g_rdec_active = false       // this loaded model runs the resident decode driver
var private g_rdec_cnt = 0l             // cached positions in the device KV mirror (built as we decode)
var private g_rdec_cap = 0l             // mirror context the driver armed at (<= model ctx when VRAM-capped)
var private g_rdec_gen = 0l             // mirror ownership generation (bumped per resident prefill)
var private g_rdec_kvdt = KVDtype.f16   // the ARMED mirror codec — sessions must match to be served
var private g_resident_prefill_allowed = true   // multi-stream schedulers pin this off (see set_resident_prefill_allowed)

//! Allow/forbid the whole-prefill resident override. A multi-stream scheduler (dasllama-server)
//! pins it OFF: the single shared mirror + chunked prefill would leave device-only KV that a
//! second stream's steal strands. Single-session tools keep the fast path (default on).
def set_resident_prefill_allowed(allowed : bool) {
    g_resident_prefill_allowed = allowed
}

// Bulk-hydrate the session's host KV from the device mirror rows [0, g_rdec_cnt) — the rail that
// keeps host KV authoritative at rest after device-resident prefills/decodes.
def private rdec_hydrate_host(t : Model; var s : Session) {
    let c = t.config
    for (l in range64(c.n_layers)) {
        rdec_read_kv_bulk(l, kv_layer_kbase(s, l, c.seq_len), kv_layer_vbase(s, l, c.seq_len), g_rdec_cnt)
    }
    s.rdec_hydrated = true
    to_log(LOG_INFO, "dasLLAMA: resident KV hydrated to host ({g_rdec_cnt} positions x {c.n_layers} layers)\n")
}
let private RDEC_MIN_CTX = 4096l        // smallest ctx worth arming a capped mirror at

// the auto-cap floor, env-overridable: DASLLAMA_GPU_MIN_CTX lets a short-context session arm a
// model whose capped mirror lands under the default floor (e.g. a 3B on an 8GB card fits at 3429)
def private rdec_min_ctx : int64 {
    let v = g_env_gpu.gpu_min_ctx   // unset or non-numeric keeps the default floor
    return v > 0l ? max(v, 256l) : RDEC_MIN_CTX
}

// gather one prepared plane into device-layout bytes and place it in its format's arena; returns
// the arena block index. Same gather the per-op tier uses (moe_gpu_gather_stack / _kq), then place
// instead of upload_stack.
def private resident_place(t : Model; f : KqFmt; woff : int64; n, rows : int64;
                           mr, kgroup, wbias : int64) : int64 {
    var wq : array<uint8>
    var ws : array<uint8>
    if (f == KqFmt.q8) {
        moe_gpu_gather_stack(t, woff, n, rows, rows, mr, kgroup, wbias, wq, ws)
    } else {
        let kmr = kq_active_mr(t, f)
        moe_gpu_gather_stack_kq(t, f, woff, n, rows, rows, t.kq_repacked, kmr, wq, ws)
    }
    let blk = rdec_place(wq, ws, n, rows, int(f))
    if (blk >= 0l) {
        vk_bake_collect(int(f), woff, n, rows, wq, ws)
    }
    delete wq
    delete ws
    return blk
}

def private rdec_blocks_for(n, rows : int64; f : KqFmt) : int64 => (
    rows * (n / (f == KqFmt.q8 ? Q8_BLOCK_ELEMS : KQ_SUPERBLOCK_ELEMS)))

// row-length servability twin of the tier's arena_rows_ok (this module is backend-agnostic)
def private rdec_rows_ok(n : int64; f : KqFmt) : bool => (
    n % (f == KqFmt.q8 ? Q8_BLOCK_ELEMS : KQ_SUPERBLOCK_ELEMS) == 0l)

// a plane's activation-quant form: 0 = Q8_0 per-32, 1 = Q8_K superblock
def private rdec_quant_class(f : KqFmt) : int => kq_sb(f) ? 1 : 0

def private resident_layer_fmts(t : Model; l : int64) : tuple<fq : KqFmt; fk : KqFmt; fv : KqFmt; fo : KqFmt; f1 : KqFmt; f3 : KqFmt; f2 : KqFmt> {
    return (fq = fmt_at(t.wq_fmt, l), fk = fmt_at(t.wk_fmt, l), fv = fmt_at(t.wv_fmt, l),
            fo = fmt_at(t.wo_fmt, l), f1 = fmt_at(t.w1_fmt, l), f3 = fmt_at(t.w3_fmt, l),
            f2 = fmt_at(t.w2_fmt, l))
}

//! Reserve arenas + place every dense plane + wire the resident decode driver. Returns false (and
//! leaves the driver unarmed) if the model is not a servable dense shape or does not fit at seq_cap.
def resident_upload(t : Model; seq_cap : int64; rkdt : KVDtype) : bool {   // nolint:STYLE037,STYLE038 — all-or-nothing arm: one reject per unsupported family feature, no seam
    g_rdec_active = false
    vulkan_bake_role(VkBakeRole.arena)
    if (!moe_gpu_resident_installed() || t.quant != QuantMode.q8 || t.config.n_expert > 0l) {
        return false
    }
    // the driver is all-or-nothing across prefill AND decode (the KV lives in the device mirror
    // only), so a pin away from it declines BEFORE any upload — the per-op rails take over
    let pin = g_env_engine.pin_prefill
    if ((!empty(pin) && pin != "vulkan") || !empty(active_prefill_override())) {
        return false
    }
    let c = t.config
    // the plain llama std shape + qk-norm (qwen3) — no bias, sinks, gated attention (its wq is
    // 2x qd rows, and it rode the old qk_norm decline), shared-KV, softcap, or partial rotary.
    // These decline cleanly and fall back to the CPU loop.
    if (c.attn_qkv_bias || c.attn_out_bias || c.v_norm || c.attn_sinks || c.moe_dense_shexp
            || c.n_layer_nextn > 0l || c.q_gated || c.attn_logit_softcap != 0.0
            || c.final_logit_softcap != 0.0 || c.pre_post_norm
            || (c.rope_dim > 0l && c.rope_dim != c.head_size)) {
        return false
    }
    let dim = c.dim
    let hid0 = layer_hidden(t, 0l)
    let hs = layer_head_size(c, 0l)
    let qd = layer_qd(c, 0l)
    let kvd = layer_kv_dim(c, 0l)
    // the prefill Q-tile attention kernel's geometry (da_attn_b: 32-dim lanes, hs-strided staging)
    if (hs % 32l != 0l || hs > 256l) {
        return false
    }
    let neox = c.rope_neox
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    // reject non-uniform per-layer geometry (the recorder assumes fixed dims across layers)
    for (l in range64(c.n_layers)) {
        if (layer_hidden(t, l) != hid0 || layer_head_size(c, l) != hs || layer_kv_dim(c, l) != kvd
                || t.kv_src[l] != l || t.wq_offs[l] < 0l || t.wk_offs[l] < 0l || t.wv_offs[l] < 0l
                || layer_is_recurrent(c, l) || layer_is_sliding(c, l)) {
            return false
        }
    }
    // tally arena blocks per format
    var need : table<int; int64>
    var planes : array<tuple<n : int64; rows : int64; f : KqFmt>>
    planes |> reserve(c.n_layers * 7l + 1l)
    for (l in range64(c.n_layers)) {
        let lf = resident_layer_fmts(t, l)
        planes |> push((n = dim, rows = qd, f = lf.fq))
        planes |> push((n = dim, rows = kvd, f = lf.fk))
        planes |> push((n = dim, rows = kvd, f = lf.fv))
        planes |> push((n = qd, rows = dim, f = lf.fo))
        planes |> push((n = dim, rows = hid0, f = lf.f1))
        planes |> push((n = dim, rows = hid0, f = lf.f3))
        planes |> push((n = hid0, rows = dim, f = lf.f2))
    }
    planes |> push((n = dim, rows = c.vocab_size, f = cls_fmt))
    for (pl in planes) {
        if (!rdec_rows_ok(pl.n, pl.f)) {   // unplaceable row: the whole driver declines (all-or-nothing)
            to_log(LOG_WARNING, "dasLLAMA: resident driver declined - a fmt {int(pl.f)} plane's row length {pl.n} is not a block multiple\n")
            delete planes
            delete need
            return false
        }
        let fi = int(pl.f)
        need[fi] = (need?[fi] ?? 0l) + rdec_blocks_for(pl.n, pl.rows, pl.f)
    }
    delete planes
    var reserve_ok = true
    for (fmt, blocks in keys(need), values(need)) {
        if (!rdec_reserve(fmt, blocks)) {
            to_log(LOG_WARNING, "dasLLAMA: resident driver - arena reserve failed (fmt {fmt}, {blocks} blocks)\n")
            reserve_ok = false
        }
    }
    delete need
    if (!reserve_ok) {
        return false
    }
    let repacked = kernel_backend_needs_repack(active_kernel_backend())
    let mr = repacked ? active_q8_layout_mr() : 1l
    let kg = repacked ? active_q8_kgroup() : 4l
    let wb = repacked ? active_q8_wbias() : 0l
    if (!rdec_prepare(c.n_layers, dim, qd, kvd, hs, c.n_heads, hid0, c.vocab_size, seq_cap, neox,
                      c.norm_eps, c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(hs)), c.qk_norm,
                      rkdt == KVDtype.f16)) {
        return false
    }
    // place + wire each layer. One quantized X image feeds each consumer group (qkv shares one,
    // gate/up another), so a group must agree on its quant form — mixed groups decline
    for (l in range64(c.n_layers)) {
        let lf = resident_layer_fmts(t, l)
        let cq = rdec_quant_class(lf.fq)
        if (cq != rdec_quant_class(lf.fk) || cq != rdec_quant_class(lf.fv)
                || rdec_quant_class(lf.f1) != rdec_quant_class(lf.f3)) {
            to_log(LOG_WARNING, "dasLLAMA: resident driver declined - layer {l} mixes activation-quant forms (qkv {int(lf.fq)}/{int(lf.fk)}/{int(lf.fv)}, ffn {int(lf.f1)}/{int(lf.f3)})\n")
            return false
        }
        let bq = resident_place(t, lf.fq, t.wq_offs[l], dim, qd, mr, kg, wb)
        let bk = resident_place(t, lf.fk, t.wk_offs[l], dim, kvd, mr, kg, wb)
        let bv = resident_place(t, lf.fv, t.wv_offs[l], dim, kvd, mr, kg, wb)
        let bo = resident_place(t, lf.fo, t.wo_offs[l], qd, dim, mr, kg, wb)
        let b1 = resident_place(t, lf.f1, t.w1_offs[l], dim, hid0, mr, kg, wb)
        let b3 = resident_place(t, lf.f3, t.w3_offs[l], dim, hid0, mr, kg, wb)
        let b2 = resident_place(t, lf.f2, t.w2_offs[l], hid0, dim, mr, kg, wb)
        // a placement can fail (arena exhausted); fail closed to the CPU loop rather than wire a -1 block
        if (bq < 0l || bk < 0l || bv < 0l || bo < 0l || b1 < 0l || b3 < 0l || b2 < 0l) {
            to_log(LOG_WARNING, "dasLLAMA: resident driver - arena placement failed at layer {l}, declining\n")
            return false
        }
        rdec_set_layer(l, bq, bk, bv, bo, b1, b3, b2,
            int(lf.fq), int(lf.fk), int(lf.fv), int(lf.fo), int(lf.f1), int(lf.f3), int(lf.f2))
    }
    let bcls = resident_place(t, cls_fmt, t.wcls_off, dim, c.vocab_size, mr, kg, wb)
    if (bcls < 0l) {
        to_log(LOG_WARNING, "dasLLAMA: resident driver - classifier placement failed, declining\n")
        return false
    }
    rdec_set_cls(bcls, int(cls_fmt))
    // upload the norm rows: [rms_att[l], rms_ffn[l]] x L, then rms_final; qk_norm appends the
    // per-head [rms_q[l], rms_k[l]] x L rows (hs floats each) past the fixed layout
    var norms : array<float>
    norms |> resize(c.n_layers * 2l * dim + dim + (c.qk_norm ? c.n_layers * 2l * hs : 0l))
    for (l in range64(c.n_layers)) {
        for (i in range64(dim)) {
            norms[l * 2l * dim + i] = t.fblob[t.rms_att_off + l * dim + i]
            norms[(l * 2l + 1l) * dim + i] = t.fblob[t.rms_ffn_off + l * dim + i]
        }
    }
    for (i in range64(dim)) {
        norms[c.n_layers * 2l * dim + i] = t.fblob[t.rms_final_off + i]
    }
    if (c.qk_norm) {
        let qkw = c.n_layers * 2l * dim + dim
        for (l in range64(c.n_layers)) {
            for (i in range64(hs)) {
                norms[qkw + l * 2l * hs + i] = t.fblob[t.rms_q_offs[l] + i]
                norms[qkw + (l * 2l + 1l) * hs + i] = t.fblob[t.rms_k_offs[l] + i]
            }
        }
    }
    rdec_upload_norms(norms)
    delete norms
    g_rdec_active = true
    g_rdec_cnt = 0l
    g_rdec_cap = seq_cap
    g_rdec_kvdt = rkdt
    let kvname = rkdt == KVDtype.f16 ? "f16" : "f32"
    // prefill + decode select together — the device prefill fills the mirror the decode reads
    select_prefill_override("vulkan")
    select_decode_override("vulkan")
    to_log(LOG_INFO, "dasLLAMA: resident driver armed ({c.n_layers} layers, dim {dim}, ctx {seq_cap}, kv {kvname}, device prefill + decode)\n")
    return true
}

// One position's packed rope row for the resident device kernels (see build_rope_row_packed —
// the one source for every resident-override rope row).
[unused_argument(half)]
def private rdec_rope_row(t : Model; pos, hs, half : int64; var dst : array<float>; base : int64) {
    let c = t.config
    build_rope_row_packed(dst, base, pos, hs, c.rope_freq_base, c.rope_freq_scale, c.rope_mscale, t.rope_freqs)
}

//! The resident whole-stack decode override (registered as "vulkan"): embeds the token, builds this
//! position's rope row, runs the whole forward on device, produces s.logits. Decode-from-scratch
//! for now — the mirror builds as positions arrive (prefix-KV sync is a follow-up).
def private vulkan_resident_decode(t : Model; var s : Session; token, pos : int64) : bool {
    // continuity guard: a gap in the mirror means an earlier position took the CPU path — decline too;
    // codec guard: hydration byte-copies mirror rows into the session cache, so codecs must match
    if (!g_rdec_active || pos > g_rdec_cnt
            || s.kv_dtype_k != g_rdec_kvdt || s.kv_dtype_v != g_rdec_kvdt || s.kv_pool != null) {
        return false
    }
    // ownership guard: the single mirror holds ONE session's rows (superseded => host has ours)
    if (s.rdec_gen != g_rdec_gen) {
        if (s.rdec_gen != 0l && !s.rdec_hydrated) {
            panic("dasLLAMA resident decode: session's mirror rows were superseded before hydration - engine bug (the steal path must hydrate first)")
        }
        return false
    }
    if (pos + 1l > g_rdec_cap) {
        // capacity exhausted: hydrate host KV from the mirror, then the CPU rails take over
        rdec_hydrate_host(t, s)
        return false
    }
    let c = t.config
    let dim = c.dim
    let hs = layer_head_size(c, 0l)
    var x : array<float>
    x |> resize(dim)
    unsafe {
        embed_row(t, token, addr(x[0]))
    }
    if (c.embed_scale != 1.0) {
        scale_inplace(unsafe(addr(x[0])), c.embed_scale, dim)
    }
    // this position's rope row: cos[half] then sin[half]
    let half = hs / 2l
    var cossin : array<float>
    cossin |> resize(hs)
    rdec_rope_row(t, pos, hs, half, cossin, 0l)
    let cnt = pos + 1l
    rdec_token(x, cossin, pos, cnt, s.logits)
    delete x
    delete cossin
    g_rdec_cnt = pos + 1l   // mirror continuity: rows [0, pos] are now live
    s.rdec_hydrated = false   // the mirror advanced past any earlier hydration
    decode_override_logits_done()   // logits produced on device
    return true
}

// The "vulkan" whole-prefill override (PrefillOverrideFn): one-submit device prefill via the
// resident driver. The mirror gets the rows; host KV is hydrated back on demand (continuation,
// cap exhaustion) so the CPU rails always take over from a valid host authority.
def private vulkan_resident_prefill(t : Model; var s : Session; npos, start_pos : int64) : bool {
    // gates: armed + embedder-allowed + a flat cache in the ARMED mirror codec (rows byte-copy;
    // a codec-mismatched or paged session falls to the CPU rails)
    if (!g_rdec_active || !g_resident_prefill_allowed
            || s.kv_dtype_k != g_rdec_kvdt || s.kv_dtype_v != g_rdec_kvdt || s.kv_pool != null) {
        return false
    }
    if (start_pos != 0l) {
        // continuation over OUR device-held history: hydrate host KV, then decline to the CPU loop
        if (g_rdec_cnt > 0l && s.rdec_gen == g_rdec_gen && !s.rdec_hydrated) {
            rdec_hydrate_host(t, s)
        }
        return false
    }
    if (npos > g_rdec_cap) {
        return false
    }
    // a DIFFERENT session's re-prefill steals the single mirror — fine for serial flows; a stranded owner's decode fails LOUD via the ownership guard
    let c = t.config
    let hs = layer_head_size(c, 0l)
    let half = hs / 2l
    var cb : array<float>
    cb |> resize(npos * hs)
    for (p in range64(npos)) {
        rdec_rope_row(t, p, hs, half, cb, p * hs)
    }
    rdec_prefill(s.x_b, cb, npos, s.logits)
    delete cb
    g_rdec_cnt = npos   // the mirror now holds [0, npos) — the decode's continuity witness
    g_rdec_gen ++
    s.rdec_gen = g_rdec_gen
    s.rdec_hydrated = false
    prefill_override_logits_done()
    return true
}

//! Resident batch-decode override: processes each session SEQUENTIALLY through the single-session
//! decode chain (resync its KV into the mirror, decode, write K/V back, scatter logits).
//! Correct multi-stream decode; declines to CPU for codec-mismatched or paged sessions.
def private vulkan_resident_batch_decode(t : Model; var ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool {
    if (!g_rdec_active) {
        return false
    }
    let c = t.config
    let dim = c.dim
    let hs = layer_head_size(c, 0l)
    let half = hs / 2l
    let kvd = layer_kv_dim(c, 0l)
    // require the shape the mirror sync can handle: a flat cache in the armed mirror codec
    for (i in range64(nrows)) {
        var s = sessions[i]
        if (s.kv_dtype_k != g_rdec_kvdt || s.kv_dtype_v != g_rdec_kvdt || s.kv_pool != null) {
            return false
        }
    }
    if (dasllama_noisy()) {
        to_log(LOG_INFO, "dasLLAMA: resident batch decode engaged ({nrows} streams)\n")
    }
    var x : array<float>
    x |> resize(dim)
    var cossin : array<float>
    cossin |> resize(hs)
    var tmp : array<float>
    tmp |> resize(c.vocab_size)
    for (i in range64(nrows)) {
        var s & = unsafe(*sessions[i])
        let pos = ws.positions[i]
        // resync this session's KV history [0, pos) into the mirror (same codec — byte rows)
        for (l in range64(c.n_layers)) {
            rdec_sync_kv(l, kv_layer_kbase(s, l, c.seq_len), kv_layer_vbase(s, l, c.seq_len), pos)
        }
        // this row's embedded residual is already in ws.scr.x_b; its rope row for pos
        for (k in range64(dim)) {
            x[int(k)] = ws.scr.x_b[i * dim + k]
        }
        rdec_rope_row(t, pos, hs, half, cossin, 0l)
        rdec_token(x, cossin, pos, pos + 1l, tmp)
        // write the new token's K/V back to this session's cache at pos, then scatter logits
        let krowb = kv_row_bytes(s.kv_dtype_k, kvd)
        let vrowb = kv_row_bytes(s.kv_dtype_v, kvd)
        for (l in range64(c.n_layers)) {
            unsafe {
                rdec_read_kv(l, pos, kv_layer_kbase(s, l, c.seq_len) + pos * krowb,
                    kv_layer_vbase(s, l, c.seq_len) + pos * vrowb)
            }
        }
        copy_floats(tmp, 0l, s.logits, 0l, c.vocab_size)
    }
    delete x
    delete cossin
    delete tmp
    batch_decode_override_logits_done()
    return true
}

[init]
def resident_driver_register() {
    register_decode_override("vulkan", @@vulkan_resident_decode)
    register_batch_decode_override("vulkan", @@vulkan_resident_batch_decode)
    register_prefill_override("vulkan", @@vulkan_resident_prefill)
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
    rdec_cnt : int64
    rdec_cap : int64
    rdec_kvdt : KVDtype
}

//! Move the INSTALLED per-model tier state out into `st`; the globals read as no-model after.
def moe_gpu_model_marks_save(var st : GpuModelMarks) {
    moe_gpu_marks_save(st.marks)
    st.rdec_active = g_rdec_active
    st.rdec_cnt = g_rdec_cnt
    st.rdec_cap = g_rdec_cap
    st.rdec_kvdt = g_rdec_kvdt
    g_rdec_active = false
    g_rdec_cnt = 0l
    g_rdec_cap = 0l
}

//! Install `st` as the per-model tier state (the save's inverse; `st` reads as no-model after).
def moe_gpu_model_marks_restore(var st : GpuModelMarks) {
    moe_gpu_marks_restore(st.marks)
    g_rdec_active = st.rdec_active
    g_rdec_cnt = st.rdec_cnt
    g_rdec_cap = st.rdec_cap
    g_rdec_kvdt = st.rdec_kvdt
    st.rdec_active = false
    st.rdec_cnt = 0l
    st.rdec_cap = 0l
}

//! Is the resident whole-stack decode driver active for the INSTALLED model? (The arm-outcome
//! discriminator status surfaces read: resident beats rails beats cpu.)
def moe_gpu_resident_active() : bool => g_rdec_active

//! Hydrate `s`'s host KV from the device mirror when `s` owns live mirror rows (no-op
//! otherwise). Call over every live session of the outgoing model before ``moe_gpu_drop_model``
//! — the drop destroys the mirror, and an unhydrated owner's rows exist nowhere else.
def moe_gpu_hydrate_session(t : Model; var s : Session) {
    if (g_rdec_active && g_rdec_cnt > 0l && s.rdec_gen == g_rdec_gen && !s.rdec_hydrated) {
        rdec_hydrate_host(t, s)
    }
}

//! Drop the INSTALLED model's whole GPU state (every model-owned device object + the routing
//! marks); the model keeps serving on the CPU rails, and re-arm = the same three calls a load
//! runs. Caller guarantees quiescence: no step in flight, sessions hydrated first.
def moe_gpu_drop_model() {
    if (!moe_gpu_tier_installed()) {
        return
    }
    moe_gpu_dn_invalidate()
    g_rdec_active = false
    g_rdec_cnt = 0l
    g_rdec_cap = 0l
    // resident_upload selected the "vulkan" overrides; deselect so (a) a later re-arm's
    // resident_upload passes its no-active-override gate (the warm re-arm decline), and (b) a
    // dropped model's prefill/decode take the plain CPU path without consulting dead state
    if (active_prefill_override() == "vulkan") {
        select_prefill_override("")
    }
    if (active_decode_override() == "vulkan") {
        select_decode_override("")
    }
    moe_gpu_drop_model_state()
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

// Upload the offloaded layers' expert stacks to the GPU tier from the PREPARED planes. Called at
// the end of every load rail so DASLLAMA_GPU_MOE_LAYERS behaves identically however the model
// arrived. Layers go resident from the end while the tier's VRAM budget holds.
def moe_gpu_upload_resident(t : Model) {   // nolint:STYLE037,STYLE038 — seven sequential rail walks sharing one rollback cursor; splitting them hides the partial-group unwind
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
    if (!moe_gpu_tier_installed()
            || (moe_gpu_layer_count() == 0l && nstream <= 0l && !dn_want && !at_want
                && !gpu_want_shexp() && !gpu_want_qkv() && !gpu_want_cls())) {
        return   // tier absent or nothing requested
    }
    // size the tier's streamed-slot reserve from THIS model's largest expert layer — a fixed
    // reserve wastes VRAM or asserts on big-expert geometries (GLM Air, Qwen1.5-MoE); must
    // precede the first upload (the tier carves the reserve at its lazy init)
    if (t.config.n_expert > 0l) {
        var sgu = 0l
        var sdn = 0l
        for (sl in range64(t.config.n_layers + t.config.n_layer_nextn)) {
            if (sl < t.config.n_layer_dense_lead || long_length(t.we1_offs) <= sl
                    || t.we1_offs[sl] < 0l || t.we2_offs[sl] < 0l || t.we3_offs[sl] < 0l) {
                continue
            }
            let sne = t.config.n_expert
            let snfe = t.config.n_ff_exp
            sgu = max(sgu, moe_gpu_plane_bytes(t.config.dim, sne * snfe, int(fmt_at(t.we1_fmt, sl))))
            sgu = max(sgu, moe_gpu_plane_bytes(t.config.dim, sne * snfe, int(fmt_at(t.we3_fmt, sl))))
            sdn = max(sdn, moe_gpu_plane_bytes(snfe, sne * t.config.dim, int(fmt_at(t.we2_fmt, sl))))
        }
        set_moe_gpu_stream_need(sgu > 0l ? 2l * sgu + sdn : 0l)
    }
    // dense models that fully fit: try the whole-stack resident driver FIRST — it subsumes every
    // rail below (all planes + classifier resident, one-submit device prefill + mirror decode).
    // Any decline (shape, fit, placement) falls through to the per-op rails unchanged.
    if (moe_gpu_resident_installed() && gpu_want_auto() && t.config.n_expert == 0l) {
        var seq_cap = t.config.seq_len
        // the mirror codec: f16 rows by default (native fa feed, ~2x plannable ctx);
        // DASLLAMA_VK_KV32=1 arms f32 mirrors instead (A/B; then only f32 sessions serve)
        let rkdt = g_env_vulkan.vk_kv32 != 0l ? KVDtype.f32 : KVDtype.f16
        // each side of the KV mirror binds as ONE SSBO range — the device's max binding
        // range caps the armable ctx (past it every mirror read is undefined, not slow)
        let bcap = moe_gpu_binding_cap()
        let mrow = t.config.n_layers * kv_row_bytes(rkdt, layer_kv_dim(t.config, 0l))
        if (bcap > 0l && mrow > 0l) {
            seq_cap = min(seq_cap, bcap / mrow)
        }
        var plan = resident_plan(t, seq_cap, rkdt, rkdt)
        if (!plan.fits && plan.weight_bytes > 0l && plan.kv_bytes > 0l) {
            // weights are fixed, the KV mirror is not: retry at the context that fits (the plan's
            // own remedy), as long as it stays a usable window. The driver guards decode past the cap.
            let fit_ctx = (max(plan.budget_bytes - plan.headroom_bytes - plan.weight_bytes
                - plan.scratch_bytes, 0l)) / (plan.kv_bytes / seq_cap)
            if (fit_ctx >= rdec_min_ctx()) {
                seq_cap = fit_ctx
                plan = resident_plan(t, seq_cap, rkdt, rkdt)
            }
        }
        if (plan.fits) {
            if (resident_upload(t, seq_cap, rkdt)) {
                return
            }
        } elif (!empty(plan.reason)) {
            to_log(LOG_INFO, "dasLLAMA: resident driver declined - {plan.reason}\n")
        }
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
        for (l in range64(layers)) {
            var uploads : array<tuple<f : KqFmt; woff : int64; n : int64; rows : int64>>
            if (layer_is_recurrent(t.config, l)) {
                // kq-tagged dn planes (CPU-rail loads) are not tier-servable — the dn chain reads q8
                if (t.dnqkv_offs[l] >= 0l && t.dngate_offs[l] >= 0l && t.dnout_offs[l] >= 0l
                        && dn_layer_q8(t, l)) {
                    uploads |> push((f = KqFmt.q8, woff = t.dnqkv_offs[l], n = dim, rows = dn_conv_dim(t.config)))
                    uploads |> push((f = KqFmt.q8, woff = t.dngate_offs[l], n = dim, rows = t.config.ssm_d_inner))
                    uploads |> push((f = KqFmt.q8, woff = t.dnout_offs[l], n = t.config.ssm_d_inner, rows = dim))
                }
            } else {
                let qd = layer_qd(t.config, l)
                let fq = fmt_at(t.wq_fmt, l)
                let fo = fmt_at(t.wo_fmt, l)
                if (gpu_want_dense_attn() && t.wq_offs[l] >= 0l && t.wo_offs[l] >= 0l
                        && (fq == KqFmt.q8 || kq_sb(fq)) && (fo == KqFmt.q8 || kq_sb(fo))
                        && kq_servable(t, fq) && kq_servable(t, fo)) {
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
    }
    // shared-expert triples — DASLLAMA_GPU_SHEXP=1 opt-in. Dense (every token, every layer, one
    // fixed weight set) so residency carries no miss term; q8 only (the repack registers them fmt 0).
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
            // deliberately NOT set_moe_gpu_dense_off: that would also route the PREFILL shexp GEMMs
            // onto the dense rail (a separately measured wash) and stop this being a decode-only lever
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
        for (l in range64(layers)) {
            if (!layer_is_recurrent(t.config, l)
                    || t.dnqkv_offs[l] < 0l || t.dngate_offs[l] < 0l || t.dnout_offs[l] < 0l
                    || !dn_layer_q8(t, l)) {   // kq-tagged dn planes stay on the CPU rail
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
            }
            break if (dn_stop)
            set_moe_gpu_dn_layer(l, t.dnqkv_offs[l])
            dn_marked++
        }
        if (dn_marked > 0l) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: deltanet triples of {dn_marked} layers resident\n")
        }
    }
    // attention quads fourth — DASLLAMA_GPU_ATTN=1 opt-in: each servable full-attention layer's
    // q|gate/k/v/wo planes + the layer mark that routes its whole prefill attention through the
    // device chain. Layer-ascending; a budget stop rolls back the partial quad.
    vulkan_bake_role(VkBakeRole.attn)
    if (at_want) {
        let c = t.config
        // NEOX rope only (partial rotary implies NEOX per the loader); the chain has no bias /
        // v-norm / sinks / softcap / post-norm arms
        let arch_ok = (!c.attn_qkv_bias && !c.v_norm && !c.attn_sinks && c.attn_logit_softcap == 0.0
            && !c.pre_post_norm && !c.attn_out_bias
            && (c.rope_neox || (c.rope_dim > 0l && c.rope_dim != c.head_size)))
        var at_marked = 0l
        var at_stop = false
        for (l in range64(layers)) {
            break if (!arch_ok)
            continue if (layer_is_recurrent(c, l) || layer_is_sliding(c, l) || t.kv_src[l] != l
                || t.wq_offs[l] < 0l || t.wk_offs[l] < 0l || t.wv_offs[l] < 0l || t.wo_offs[l] < 0l)
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
            continue if (!fmts_ok || !(hs == 128l || hs == 256l) || qd > 8192l || kvd > 1024l
                || c.n_heads > 64l || rot % 2l != 0l || rot > hs || kvd % 32l != 0l
                || qd % (fo != KqFmt.q8 ? 256l : 32l) != 0l || dim % 32l != 0l)
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
                break
            }
            set_moe_gpu_attn_layer(l)
            at_marked++
        }
        if (at_marked > 0l) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: attention quads of {at_marked} layers resident\n")
        }
    }
    // attention q/k/v triples — DASLLAMA_GPU_QKV=1 opt-in. LAST of the dense rails ON PURPOSE: it
    // is the lowest-value one and an earlier section exhausting stacks/VRAM starves those behind it
    // (a 224-stack cap once cut 6 DN layers = -14% tg). Pays only where GPU reads beat CPU reads.
    vulkan_bake_role(VkBakeRole.qkv)
    if (gpu_want_qkv()) {
        var qkv_marked = 0l
        for (l in range64(layers)) {
            continue if (layer_is_recurrent(t.config, l) || t.kv_src[l] != l || moe_gpu_attn_on_gpu(l))
            let qd = layer_qd(t.config, l)
            let kv = layer_kv_dim(t.config, l)
            let fq = fmt_at(t.wq_fmt, l)
            let fk = fmt_at(t.wk_fmt, l)
            let fv = fmt_at(t.wv_fmt, l)
            // one shared activation image ⇒ the three planes must agree on quant CLASS
            let cls_ok = (fq == KqFmt.q8) == (fk == KqFmt.q8) && (fq == KqFmt.q8) == (fv == KqFmt.q8)
            continue if (t.wq_offs[l] < 0l || t.wk_offs[l] < 0l || t.wv_offs[l] < 0l || !cls_ok
                || !kq_servable(t, fq) || !kq_servable(t, fk) || !kq_servable(t, fv))
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
                break
            }
            set_moe_gpu_qkv_layer(l)
            qkv_marked++
        }
        if (qkv_marked > 0l) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: q/k/v of {qkv_marked} attention layers resident\n")
        }
    }
    vulkan_bake_role(VkBakeRole.expert)
    for (i in range64(want)) {
        let l = layers - 1l - i
        let f1 = fmt_at(t.we1_fmt, l)
        let f2 = fmt_at(t.we2_fmt, l)
        let f3 = fmt_at(t.we3_fmt, l)
        // gate/up must share a form (one activation image); down is free. llama.cpp's Q4_K_M MoE
        // recipe bumps ffn_down_exps to Q5_0/Q8_0, so k4/k4/q8 is the COMMON shape (glm4moe,
        // gemma). The walk stops on refusal so the resident set stays contiguous from the end.
        let gu_ok = ((f1 == KqFmt.q8) == (f3 == KqFmt.q8)
            && (f1 == KqFmt.q8 || (kq_sb(f1) && kq_sb(f3))))
        let dn_ok = f2 == KqFmt.q8 || kq_sb(f2)
        if (l < t.config.n_layer_dense_lead || t.we1_offs[l] < 0l || !(gu_ok && dn_ok)) {
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
        let f1 = fmt_at(t.we1_fmt, l)
        let f2 = fmt_at(t.we2_fmt, l)
        let f3 = fmt_at(t.we3_fmt, l)
        // same rule as the resident walk: gate/up share one activation image, down is free
        let gu_ok = ((f1 == KqFmt.q8) == (f3 == KqFmt.q8)
            && (f1 == KqFmt.q8 || (kq_sb(f1) && kq_sb(f3))))
        if (!(gu_ok && (f2 == KqFmt.q8 || kq_sb(f2)))) {
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
    }
}
