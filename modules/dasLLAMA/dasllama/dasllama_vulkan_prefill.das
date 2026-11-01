options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_prefill shared public
options _cyclomatic_complexity = 0   // batched record/setup walks — split queued for the dedup phase
options _function_length = 0   // same

require dasllama/dasllama_lint public
require dasllama/dasllama_vulkan_common   // g_gpu/g_rd state, plumbing, hazard rail, fill metas
require dasllama/dasllama_vulkan_classes   // the class-kernel rail (generated set_/enc_ + gemv_cls_*)
require dasllama/dasllama_common   // cap_ring_rows - the capture's n-gram ring rows, the Metal window's walk too
require dasllama/dasllama_vulkan_decode   // vk_moe_dn_step — the dn chain's npos==1 arm routes through the decode step
require dasllama/dasllama_kqformat   // KqFmt — the arena fmt slots are its ordinals
require dasllama/dasllama_gemm_schema   // Q8_BLOCK_ELEMS - the window chain's hyper-connection scale planes
require dasllama/dasllama_env   // g_env_vulkan — the vk_kv_merge / vk_overlap / vk_fuse A/B hatches
require dasllama/dasllama_gpu_tier   // moe_gpu_stream_need — the stream-slot carve's source of truth
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_math
require dasllama/dasllama_gpu_math   // ACT_*, act_code: the act word the act classes read
require math

// The Vulkan PREFILL + batch driver (dasllama_metal_prefill's twin): the resident batched
// prefill (pf_setup / vk_rdec_prefill), the arena FFN/GEMM/GEMV seams, the per-format batched
// GEMM chains (FFN batch + dense), deltanet + attention prefill dispatch, and the streamed
// pinned-mirror rail.
//
// STREAMED layers (DASLLAMA_GPU_MOE_STREAM=N, the N layers below the resident set): the loader
// keeps their device-layout planes in pinned host mirrors; per prefill each layer's stacks DMA
// into one of two device slots whose pseudo-stacks re-base onto the layer, so the batch rail
// runs verbatim. After a layer's fenced chain, the NEXT layer's copy submits UNFENCED and rides
// the GPU-idle window while the CPU runs attention — the following chain's transfer->compute
// barrier orders it (in-order queue). Decode of streamed layers stays on the CPU planes.

// ===== the resident prefill driver (batched, npos rows, one submit) =====
// Batch-kernel twin of vk_rdec_token; only the LAST row's logits are produced (reuses the decode
// tail). PF_ROLES prefill sets/layer + the buffers are built once; metas fill per call (npos varies).

let private PF_ROLES = 20l   // quant_xb q k v rope attn quant_at wo addr_ffn quant_xb2 gate up actrq down addr_next qkn ple_x ple_gate ple_act ple_proj

var private @scratch g_pf_ids_stage : array<uint>

//! whether the prefill's f16 feed admits a plane of format `f` on the armed coopmat mode - the tests read the same answer the driver acts on
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice")]
def public pf_f16_feed(f : int) : bool {
    if (g_gpu.coopmat_mode == COOPMAT_CM2) {
        return f == int(KqFmt.q8) || kq_block32(f) || kq_sb(f)
    }
    return g_gpu.coopmat_mode == COOPMAT_MM && (kq_sb(f) || f == int(KqFmt.q8) || kq_block32(f)) && khr_kq_tile_on()
}

[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice")]
def private pf_qkv6(l : int64) : bool => qkv6_of(g_rd.layers[l])
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice")]
def private qkv6_of(L : RLayer) : bool => pf_f16_feed(L.fq) && pf_f16_feed(L.fk) && pf_f16_feed(L.fv)
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice")]
def private pf_gu6(l : int64) : bool => g_env_vulkan.vk_f16_ffn && pf_f16_feed(g_rd.layers[l].f1) && pf_f16_feed(g_rd.layers[l].f3)
//! the down GEMM's feed: the hidden rows as f16 where the tile admits the plane and the FFN lever leaves the group on it (off it the rows quantize to the Q8 blocks the CPU chain uses)
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice")]
def private pf_dn6(l : int64) : bool => g_env_vulkan.vk_f16_ffn && pf_f16_feed(g_rd.layers[l].f2)
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice"), arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
def private pf_dnx6(l : int64) : bool => pf_f16_feed(g_rd.layers[l].fqkv) && pf_f16_feed(g_rd.layers[l].fz)
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice"), arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
def private pf_dno6(l : int64) : bool => pf_f16_feed(g_rd.layers[l].fout)

[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
def private pf_qkv_feed_fused(l : int64) : bool => !g_rd.layers[l].recurrent && g_env_vulkan.vk_fuse && (pf_qkv6(l) || !kq_sb(g_rd.layers[l].fq))

let PF_CHUNK_MAX = 8l   // the overlap ramp doubles 1,2,4 then holds here; the cmd ring is sized from the cap

//! layers one submit of the window chain holds at most: DASLLAMA_VK_CHUNK where set, else one on an integrated GPU (a Linux
//! amdgpu resets the device under a job past its watchdog, which an iGPU's 8-layer chunk reaches on a long prompt) and the ramp's
//! hold on a discrete one
def pf_chunk_cap : int64 {
    let e = g_env_vulkan.vk_chunk
    if (e > 0l) return min(e, PF_CHUNK_MAX)
    return g_gpu.integrated ? 1l : PF_CHUNK_MAX
}

var private @scratch g_pf_segs : array<RdecPfSeg>    //! the call's segments (`RdecPfSeg`), rows over the whole call; a one-session call is one segment at row 0
var private @scratch g_pf_wsegs : array<RdecPfSeg>   //! the parts of them inside the window being recorded, `row0` window-local, `npos` the part's rows

//! the segment parts a window `[w0, w0 + wlen)` holds, into `g_pf_wsegs`
def private pf_window_segs(w0, wlen : int64) {
    rdec_window_segs(g_pf_segs, w0, wlen, g_pf_wsegs)   // nolint:PERF026 — g_pf_wsegs is the @scratch window list, cleared through the shared helper
}

let PF_CAP_WORDS = 256l   //! the capture records a window holds (two words a capture)

var private @scratch g_pf_caps : array<RdecPfCap>         //! the call's captures (`vk_rdec_prefill_caps`), segment-relative rows
var private @scratch g_pf_wcaps : array<tuple<n : int64; rec : int64>>   //! each window part's captures: their count and first record word
var private @scratch g_pf_cap_words : array<uint>

def vk_rdec_prefill_caps(caps : array<RdecPfCap>) {
    g_pf_caps := caps
    return if (empty(caps) || g_rd == null || !g_rd.hybrid)
    if (g_rd.snap_dev == 0ul) {
        rd_snap_alloc()
    }
    if (g_rd.pf_ready && g_rd.pf_snap_bound != g_rd.snap_dev) {   //! a checkpoint save may have made the rows after the sets were built
        for (l in range64(g_rd.n_layers)) {
            if (g_rd.layers[l].recurrent) {
                pf_dn_layer_sets(l, g_rd.dim, g_rd.dn_cd, g_rd.dn_di)
            }
        }
    }
}

def private pf_window_caps(w0 : int64) {
    g_pf_wcaps |> clear()
    g_pf_wcaps |> reserve(length(g_pf_wsegs))
    g_pf_cap_words |> clear()
    g_pf_cap_words |> reserve(PF_CAP_WORDS)
    for (ws in g_pf_wsegs) {
        let sg = g_pf_segs[ws.seg]
        let part_row0_in_seg = w0 + ws.row0 - sg.row0
        var n = 0l
        let rec = long_length(g_pf_cap_words)
        for (cap in g_pf_caps) {
            continue if (cap.seg != ws.seg || cap.row <= part_row0_in_seg || cap.row >= part_row0_in_seg + ws.npos)
            g_pf_cap_words |> push(uint(cap.row - part_row0_in_seg))
            g_pf_cap_words |> push(uint(((ws.region * int64(RDEC_DN_SNAPS) + cap.slot) * g_rd.snap_row_bytes) / 4l))
            n++
        }
        g_pf_wcaps |> push((n = n, rec = rec))
    }
    if (long_length(g_pf_cap_words) > PF_CAP_WORDS) {
        panic("vk prefill: {long_length(g_pf_cap_words) / 2l} checkpoint captures in one window, past the {PF_CAP_WORDS / 2l} the records hold")
    }
    if (!empty(g_pf_cap_words)) {
        upload_region_at(g_rd.pf_cap_dev, 0l, unsafe(addr<uint8 const?>(g_pf_cap_words[0])), long_length(g_pf_cap_words) * 4l)
    }
}

def private pf_caps_done {
    for (cap in g_pf_caps) {
        let sg = g_pf_segs[cap.seg]
        g_rd.snap_par[sg.region * int64(RDEC_DN_SNAPS) + cap.slot] = 0
    }
    g_pf_caps |> clear()
}

//! the captures name the dropped driver's regions and checkpoint rows: the next model's first call hands its own
def private pf_caps_forget {
    g_pf_caps |> clear()
    g_pf_wcaps |> clear()
    g_pf_cap_words |> clear()
}

[init]
def private pf_register_drop_hooks {
    register_vk_drop_hook(@@pf_caps_forget)
}

//! the slot of the segment whose last row is window row `row` of the window at `w0`, else -1
def private pf_seg_ending_at(w0, row : int64) : int64 => rdec_seg_ending_at(g_pf_segs, w0, row)

[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
def private pf_kv_merged(l, dim, kvd : int64) : bool {
    if (!g_env_vulkan.vk_kv_merge || g_rd.layers[l].fq != int(KqFmt.q8)
            || g_rd.layers[l].fk != int(KqFmt.q8) || g_rd.layers[l].fv != int(KqFmt.q8)) {
        return false
    }
    return g_rd.layers[l].bv == g_rd.layers[l].bk + kvd * (dim / 32l)
}

//! Test hook: false runs every layer's FFN over the whole window (the slice's A/B arm)
var g_pf_ffn_slice = true

//! The first row of a layer's FFN dispatches: the last layer's, on the f16-fed cm2 route, starts 32 rows below the window's end
[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
def pf_ffn_slice_row0(l, n_layers, wlen : int64; f16_gate, f16_down : bool) : int64 =>
    g_pf_ffn_slice && g_env_vulkan.vk_ffn_slice && l + 1l == n_layers && f16_gate && f16_down ? max(wlen - 32l, 0l) : 0l

//! the h128 coopmat twin serves a 128-head layer where the cm2 flash tiles do not
def private pf_fa128s : bool => !(cm2_fa_on() && vk_fa_on() && rd_pf_reads_f16()) && h128_twin_on()

//! a window's rows from `w0`: the last window keeps at least the conv taps, so only window 0 can be shorter, and its history is zero
def private pf_window_len(npos, w0 : int64) : int64 {
    let taps = max(g_rd.dn_dconv - 1l, 0l)
    var wlen = min(g_rd.pf_np, npos - w0)
    if (g_rd.hybrid) {   //! a recurrent window is one deltanet chunk: the smalls planes (beta, g) and the scan workspace hold DN_WINDOW rows
        wlen = min(wlen, DN_WINDOW)
    }
    let rem = npos - w0 - wlen
    if (rem > 0l && rem < taps) {
        wlen -= taps - rem
    }
    assert(wlen >= taps || w0 == 0l, "vk prefill: a later window shorter than the conv taps")
    return wlen
}

def private pf_warm_cos(cos_batch : array<float>; w0, wlen : int64) {
    let hcarry = w0 > 0l ? 1l : 0l
    let nhead = wlen - 1l + hcarry
    if (nhead > 0l) {
        let ce = g_rd.cos_elems
        upload_region_at(g_rd.head_pf_hcos, 0l, unsafe(addr<uint8 const?>(cos_batch[int((w0 - hcarry) * ce)])), nhead * ce * 4l)
    }
}

def private pf_cls_enc(raw : VkCommandBuffer; var h : VkHaz) {
    var pcc = GemvArgs(n = uint(g_rd.dim), d = uint(g_rd.vocab), nreg = 1u, ybase = 0u)
    gemv_enc(g_rd.cls_fmt_marker, raw, h, g_rd.cls_set, pcc, g_rd.vocab)
    if (rd_cls_epi_on()) {
        rd_enc_cls_epi(raw, h)
    }
}

//! A cm2 role's tile column and split pick; a region starting above row 0, and an expert-only plane, never splits
[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-b32-window-gemm")]
def pf_split_pick(fmt : int; d, rows, n, row0, sib_tiles : int64) : tuple<tc : int64; nsplit : int64; ksplit : int64> {
    if (row0 != 0l || cm2_s_only(fmt)) {
        return (tc = batch_tile_edges(fmt, d, rows, true).tc, nsplit = 1l, ksplit = 0l)
    }
    return cm2_gemm_pick(d, rows, n, sib_tiles, g_rd.pf_splitk_bytes)
}

// one prefill GEMM role on the class rail: schedule fill + a per-(role, variant) cached set +
// BatchArgs push. kq formats take the kq tile family, else the q8 router (whose variant moves
// with the dispatched row count — the last window is shorter, the last layer's FFN is sliced).
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
def private pf_gemm_enc(raw : VkCommandBuffer; var h : VkHaz; idx : int; fmt : int;
                        wq, ws, ab, sb, yb : uint64; wqn, wsn, an, sn, yn : int64;
                        abit, ybit : uint; n, d, blk, rows : int64; f16feed : bool; sib_tiles : int64 = 0l; row0 : int64 = 0l; ts : bool = true) {
    // the tile pick and the split pick — computed once, fed to fill AND enc
    var nsplit = 1l
    var ksplit = 0l
    var cm2_tc = 0l
    if (f16feed) {
        let sp = pf_split_pick(fmt, d, rows, n, row0, sib_tiles)
        cm2_tc = sp.tc
        nsplit = sp.nsplit
        ksplit = sp.ksplit
    }
    let groups = int64(fill_arena_batch_sched(g_rd.pf_meta[idx], d, blk, rows, fmt, f16feed, nsplit, row0, cm2_tc))
    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u, ksplit = nsplit > 1l ? uint(ksplit) : 0u)
    if (f16feed) {
        // native fmt-0 decode-in-load; a split pick reroutes the GEMM into the scratch planes
        let split = nsplit > 1l
        if (split) {
            verify(nsplit * rows * d * 4l <= g_rd.pf_splitk_bytes, "vk prefill: split-k planes exceed the scratch bound")
        }
        let yob = split ? g_rd.pf_splitk : yb
        let yon = split ? g_rd.pf_splitk_bytes : yn
        let yog = split ? VHZ_SK : ybit
        let key = idx * 16 + (cm2_tc == 256l ? (split ? 9 : 8) : (cm2_tc == 128l ? (split ? 11 : 10) : (split ? 14 : 13)))
        var gset = cached(g_rd.pf_cls_gemm, key) $ {
            verify(cm2_cls_ensure(fmt, cm2_tc), "vk prefill: the cm2 tile class rail must engage")
            return cm2_cls_set(fmt, cm2_tc,
                fixed_array(wq, ws, g_rd.pf_meta[idx].buf, ab, yob),
                fixed_array(wqn, wsn, BATCH_META_BYTES, an, yon),
                fixed_array(0u, 0u, 0u, abit, yog))
        }
        cm2_cls_enc(fmt, cm2_tc, raw, h, gset, pc, groups)
        if (split) {
            pf_splitk_reduce_enc(raw, h, idx, yb, yn, ybit, rows * d, nsplit)
        }
    } elif (kq_sb(fmt) || blk32_rail(fmt)) {
        // the role's format is static, so the slot never collides with the q8 router's variants
        let q40cm = fmt == int(KqFmt.q40) && g_gpu.coopmat_mode == COOPMAT_F16
        var kset = cached(g_rd.pf_cls_gemm, idx * 16 + 6) $ {
            verify(kq_batch_cls_ensure(fmt, q40cm), "vk prefill: the kq batch class rail must engage")
            return kq_batch_cls_set_for(fmt, q40cm,
                fixed_array(wq, ws, g_rd.pf_meta[idx].buf, ab, sb, yb),
                fixed_array(wqn, wsn, BATCH_META_BYTES, an, sn, yn),
                fixed_array(0u, 0u, 0u, abit, abit, ybit))
        }
        kq_batch_cls_enc_for(fmt, q40cm, raw, h, kset, pc, groups)
    } else {
        let v = q8_batch_cls_variant(q8_batch_tile_for(d, rows), q8_batch_aligned(d, rows))
        var qset = cached(g_rd.pf_cls_gemm, idx * 16 + v) $ {
            verify(q8_batch_cls_ensure(v), "vk prefill: the q8 batch class rail must engage")
            return q8_batch_cls_set(v,
                fixed_array(wq, ws, g_rd.pf_meta[idx].buf, ab, sb, yb),
                fixed_array(wqn, wsn, BATCH_META_BYTES, an, sn, yn),
                fixed_array(0u, 0u, 0u, abit, abit, ybit))
        }
        q8_batch_cls_enc(v, raw, h, qset, pc, groups)
    }
    if (ts) {   // the head warm's GEMMs stamp nothing: the profile's role table counts the layers' stamps and the tail's
        pfq_ts(raw)
    }
}

//! the small f16 GEMM route serves: the shape admits it and the split-k scratch holds a whole window's partial planes at the router's row
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def private pf_f16g_on() : bool => (g_rd.pf_splitk != 0ul && f16g_shape_ok(g_rd.dim)
    && F16G_SPLIT * pf_window_rows() * g_rd.ner * 4l <= g_rd.pf_splitk_bytes)

//! one small f16 GEMM: y[rows x d] at ybase = x[rows x n] w[d x n]^T over the weight rows at woff, through the split-k planes
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def private pf_f16g_enc(raw : VkCommandBuffer; var h : VkHaz; var gset, rset : VkDescriptorSet; n, d, rows, woff, ybase : int64) {
    verify(F16G_SPLIT * rows * d * 4l <= g_rd.pf_splitk_bytes, "vk prefill: the f16 GEMM's partial planes exceed the split-k scratch")
    var pc = F16GemmArgs(n = uint(n), d = uint(d), rows = uint(rows), woff = uint(woff), ybase = 0u, ystride = uint(d), ksplit = uint(n / F16G_SPLIT))
    enc_f16_gemm_cm2_cls(raw, h, gset, pc, f16_gemm_wgs(false, rows, d) * F16G_SPLIT)   //! the split-k route is cm2-only (`f16g_shape_ok`), so the cm2 builder and its grid times the split
    splitk_reduce_enc(raw, h, rset, rows * d, F16G_SPLIT, ybase)
}

def private pf_feed_enc(raw : VkCommandBuffer; var h : VkHaz; var fset : VkDescriptorSet; f16, kq : bool; inbase, elems : int64) {
    if (f16) {
        var pc = gk_f16_rows_args(uint(elems))
        enc_tower_clamp_cvt_cls(raw, h, fset, pc, rq_wgs(false, elems))
    } else {
        rq_enc(raw, h, fset, kq, inbase, elems)
    }
}

def private pf_feed_planes(f16 : bool; f16buf, qbuf, sbuf : uint64; width : int64) : tuple<q : uint64; s : uint64; qn : int64; sn : int64> {
    let np = g_rd.pf_np
    return (f16 ? (q = f16buf, s = g_rd.dummy, qn = np * width * 2l, sn = 256l)
        : (q = qbuf, s = sbuf, qn = np * width, sn = np * (width / 32l) * 4l))
}

def private pf_feed_set(f16 : bool; src : uint64; sbit : uint; f16buf, qbuf, sbuf : uint64; width : int64; obit : uint) : VkDescriptorSet {
    let srcb = g_rd.pf_np * width * 4l
    let p = pf_feed_planes(f16, f16buf, qbuf, sbuf, width)
    return (f16 ? set_tower_clamp_cvt_cls(fixed_array(src, p.q), fixed_array(srcb, p.qn), fixed_array(sbit, obit))
        : set_rq_cls(fixed_array(src, p.q, p.s), fixed_array(srcb, p.qn, p.sn), fixed_array(sbit, obit, obit)))
}

//! one recurrent layer's prefill window sets; the o feed sets (`pf_dnof_set` / `pf_dnrq_set`) are shared by every recurrent layer
[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
def private pf_dn_layer_sets(l, dim, cd, di : int64) {
    var L & = unsafe(g_rd.layers[l])
    let np = g_rd.pf_np
    let dn0 & = unsafe(L.dn[0])   //! every region's slot shares the layer's two buffers; slot 0 names them, the window's pushes name the selected region's
    let smalls_dev = dn0.smalls_dev
    let smalls_bytes = dn0.smalls_bytes
    L.pf_dn_conv_set = set_dn_conv_cls(
        fixed_array(g_rd.pf_dnqkv, g_rd.pf_dnconv, smalls_dev),
        fixed_array(np * cd * 4l, np * cd * 4l, smalls_bytes),
        fixed_array(VHZ_DNP, VHZ_DNC, VHZ_DNSM))
    let snap_dev = g_rd.snap_dev != 0ul ? g_rd.snap_dev : smalls_dev
    g_rd.pf_snap_bound = g_rd.snap_dev
    let snap_bytes = g_rd.snap_dev != 0ul ? g_rd.snap_row_bytes * g_rd.regions * int64(RDEC_DN_SNAPS) : smalls_bytes
    if (!vk_range_ok(snap_bytes)) {   //! the scan and the cap tail bind the checkpoint rows whole
        panic("dasLLAMA vulkan prefill: the checkpoint rows ({snap_bytes} bytes: {g_rd.regions} regions x {RDEC_DN_SNAPS} rows of {g_rd.snap_row_bytes}) pass the device's storage range {vk_max_storage_range()}")
    }
    L.pf_dn_scan_set = set_dn_scan_cls(
        fixed_array(g_rd.pf_dnconv, dn0.state_dev, smalls_dev, g_gpu.dn_ws_dev, g_rd.pf_cap_dev, snap_dev),
        fixed_array(np * cd * 4l, dn0.slot_state_bytes * long_length(L.dn), smalls_bytes, DN_WS_BYTES, PF_CAP_WORDS * 4l, snap_bytes),
        fixed_array(VHZ_DNC, VHZ_DNST, VHZ_DNSM, VHZ_DNWS, 0u, VHZ_DNSM))
    L.pf_dn_captail_set = set_dn_cap_tail_cls(
        fixed_array(g_rd.pf_dnqkv, smalls_dev, g_rd.pf_cap_dev, snap_dev),
        fixed_array(np * cd * 4l, smalls_bytes, PF_CAP_WORDS * 4l, snap_bytes),
        fixed_array(VHZ_DNP, VHZ_DNSM, 0u, VHZ_DNSM))
    let p3_bufs = fixed_array(g_rd.pf_dnz, g_gpu.dn_ws_dev, smalls_dev)
    let p3_bytes = fixed_array(np * di * 4l, DN_WS_BYTES, smalls_bytes)
    let p3_bits = fixed_array(VHZ_DNZ, VHZ_DNWS, VHZ_DNSM)
    L.pf_dn_scan_p3_set = g_rd.dn_zsig ? set_dn_scan_p3_sig_cls(p3_bufs, p3_bytes, p3_bits) : set_dn_scan_p3_cls(p3_bufs, p3_bytes, p3_bits)
    L.pf_dn_tail_set = set_dn_tail_cls(
        fixed_array(g_rd.pf_dnqkv, smalls_dev),
        fixed_array(np * cd * 4l, smalls_bytes),
        fixed_array(VHZ_DNP, VHZ_DNSM))
    if (g_rd.dn_ba_f32) {
        L.pf_dn_ba_set = set_dn_ba_cls(
            fixed_array(g_rd.dn_ba_dev, g_rd.pf_xb, smalls_dev),
            fixed_array(g_rd.dn_ba_bytes, np * dim * 4l, smalls_bytes),
            fixed_array(0u, VHZ_XB, VHZ_DNSM))
    }
    if (pf_dnx6(l)) {
        L.pf_dn_cvt_set = set_tower_clamp_cvt_cls(fixed_array(g_rd.pf_xb, g_rd.pf_xf),
            fixed_array(np * dim * 4l, np * dim * 2l), fixed_array(VHZ_XB, VHZ_XQ))
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
def private pf_dn_zero_region(raw : VkCommandBuffer; var h : VkHaz; r : int64) {
    vhz_dep(raw, h, 0u, VHZ_DNST | VHZ_DNSM, true)
    for (L in g_rd.layers) {
        if (L.recurrent) {
            let st & = unsafe(L.dn[r])
            cmd_fill_zero(raw, st.state_dev, st.state_byte_off, st.slot_state_bytes)
            cmd_fill_zero(raw, st.smalls_dev, st.hist_off * 4l, st.ring_image_bytes)
        }
    }
}

//! the window call's recurrent entry, a segment at a time: a segment from position zero sends its region's foreign slots home and
//! enters on ring image 0, which its first window zero-fills; a continuation enters on its region's parity, the slots the host bound
[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
def private pf_dn_begin {
    for (sg in g_pf_segs) {
        if (sg.pos0 == 0l) {
            vk_rdec_dn_flush_owners(sg.region)
            g_rd.dn_parity[sg.region] = 0
        }
        g_rd.pf_dn_cur[sg.region] = g_rd.dn_parity[sg.region]
    }
}

//! the window's parts that open their segment at position zero start their regions' state and ring from zero
def private pf_dn_zero_fresh(raw : VkCommandBuffer; var h : VkHaz) {
    for (ws in g_pf_wsegs) {
        if (ws.pos0 == 0l) {
            pf_dn_zero_region(raw, h, ws.region)
        }
    }
}

//! a window's parts each wrote their tail into the image they did not read: their regions read that one next
def private pf_dn_window_done {
    for (ws in g_pf_wsegs) {
        g_rd.pf_dn_cur[ws.region] = 1 - g_rd.pf_dn_cur[ws.region]
    }
}

//! the call's segments' regions name the image their last window's tail landed in
def private pf_dn_end {
    for (sg in g_pf_segs) {
        g_rd.dn_parity[sg.region] = g_rd.pf_dn_cur[sg.region]
    }
}

//! the per-op tier's window over one state: the conv over the history in the layout `hist_mode` names, the scan, the gated out-norm
def private dn_window_enc(raw : VkCommandBuffer; var h : VkHaz; var conv_set, scan_set, p3_set : VkDescriptorSet; rows, cd, kd, nvh, nkh, ds, di, dconv : int64;
                          hist_mode, hist_off, state_base : uint; sig : bool; eps : float) {
    var pcc = GkDnConvArgs(npos = uint(rows), cd = uint(cd), kd = uint(kd), ds = uint(ds), dconv = uint(dconv),
        hist_mode = hist_mode, taps_off = uint(DN_SM_TAPS), hist_off = hist_off,
        eps = eps, qscale = 1.0 / sqrt(float(ds)))
    enc_dn_conv_cls(raw, h, conv_set, pcc, dn_conv_wgs(cd, rows))
    pfq_ts(raw)
    var pcs = DnScanArgs(npos = uint(rows), cd = uint(cd), kd = uint(kd), nvh = uint(nvh), nkh = uint(nkh), ds = uint(ds),
        beta_off = uint(DN_SM_BETA), g_off = uint(DN_SM_G), a_off = uint(DN_SM_A), dt_off = uint(DN_SM_DT),
        o_off = uint(DN_O_OFF), di = uint(di), region_state_base = state_base)
    enc_dn_scan_cls(raw, h, scan_set, pcs, dn_scan_wgs(nvh, ds))
    pfq_ts(raw)
    var pc3 = GkDnGateArgs(nvh = uint(nvh), di = uint(di), ds = uint(ds), o_off = uint(DN_O_OFF),
        wnorm_off = uint(DN_SM_WNORM), eps = eps)
    if (sig) {
        enc_dn_scan_p3_sig_cls(raw, h, p3_set, pc3, rows)
    } else {
        enc_dn_scan_p3_cls(raw, h, p3_set, pc3, rows)
    }
    pfq_ts(raw)
}

//! every window part's recurrent conv, scan and ring tail over its own region's slot - the part's rows `row0` on, its history the
//! ring image its region entered the window on, its tail the other image - then the gated out-norm over the whole window
[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
def private dn_window_parts_enc(raw : VkCommandBuffer; var h : VkHaz; var L : RLayer; wlen, cd, kd, nvh, nkh, ds, di, dconv : int64; sig : bool; eps : float) {
    let taps = dconv - 1l
    for (ws in g_pf_wsegs) {
        assert(ws.row0 + ws.npos <= DN_WINDOW, "dasLLAMA vulkan prefill: a recurrent window part past DN_WINDOW rows - the smalls planes and the scan workspace hold that many")
        let st & = unsafe(L.dn[ws.region])
        let img = int(st.ring_image_bytes / 4l)
        let cur = g_rd.pf_dn_cur[ws.region]
        var pcc = GkDnConvArgs(npos = uint(ws.npos), cd = uint(cd), kd = uint(kd), ds = uint(ds), dconv = uint(dconv),
            hist_mode = 1u, taps_off = uint(DN_SM_TAPS), hist_off = uint(int(st.hist_off) + cur * img),
            eps = eps, qscale = 1.0 / sqrt(float(ds)), row0 = uint(ws.row0))
        enc_dn_conv_cls(raw, h, L.pf_dn_conv_set, pcc, dn_conv_wgs(cd, ws.npos))
    }
    pfq_ts(raw)
    for (ws, wi in g_pf_wsegs, count()) {
        let st & = unsafe(L.dn[ws.region])
        let wc = g_pf_wcaps[wi]
        var pcs = DnScanArgs(npos = uint(ws.npos), cd = uint(cd), kd = uint(kd), nvh = uint(nvh), nkh = uint(nkh), ds = uint(ds),
            beta_off = uint(DN_SM_BETA), g_off = uint(DN_SM_G), a_off = uint(DN_SM_A), dt_off = uint(DN_SM_DT),
            o_off = uint(DN_O_OFF), di = uint(di), region_state_base = uint(st.state_byte_off / 4l), row0 = uint(ws.row0),
            cap_n = uint(wc.n), cap_rec = uint(wc.rec), cap_state_off = uint(L.snap_off / 4l))
        enc_dn_scan_cls(raw, h, L.pf_dn_scan_set, pcs, dn_scan_wgs(nvh, ds))
        if (wc.n > 0l) {
            let img = int(st.ring_image_bytes / 4l)
            let cur = g_rd.pf_dn_cur[ws.region]
            var pcx = DnCapTailArgs(cd = uint(cd), taps = uint(taps), row0 = uint(ws.row0), entry_off = uint(int(st.hist_off) + cur * img),
                cap_rec = uint(wc.rec), cap_hist_off = uint((L.snap_off + st.slot_state_bytes) / 4l))
            enc_dn_cap_tail_cls(raw, h, L.pf_dn_captail_set, pcx, dn_cap_tail_wgs(cd, taps) * wc.n)
        }
    }
    pfq_ts(raw)
    var pc3 = GkDnGateArgs(nvh = uint(nvh), di = uint(di), ds = uint(ds), o_off = uint(DN_O_OFF),
        wnorm_off = uint(DN_SM_WNORM), eps = eps)
    if (sig) {
        enc_dn_scan_p3_sig_cls(raw, h, L.pf_dn_scan_p3_set, pc3, wlen)
    } else {
        enc_dn_scan_p3_cls(raw, h, L.pf_dn_scan_p3_set, pc3, wlen)
    }
    pfq_ts(raw)
    vhz_dep(raw, h, VHZ_DNP, VHZ_DNSM, false)
    for (ws in g_pf_wsegs) {
        let st & = unsafe(L.dn[ws.region])
        let img = int(st.ring_image_bytes / 4l)
        let cur = g_rd.pf_dn_cur[ws.region]
        //! a part shorter than the taps takes the ring's leading rows off the image it entered on
        var pct = DnTailArgs(cd = uint(cd), taps = uint(taps), row0 = uint(ws.row0 + max(ws.npos - taps, 0l)), hist_off = uint(int(st.hist_off) + (1 - cur) * img),
            old_rows = uint(max(taps - ws.npos, 0l)), old_off = uint(int(st.hist_off) + cur * img))
        enc_dn_tail_cls(raw, h, L.pf_dn_tail_set, pct, (cd * taps + int64(WG_X) - 1l) / int64(WG_X))
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private rd_pf_recurrent(raw : VkCommandBuffer; var h : VkHaz; l : int64; b : int; wlen : int64) {
    var L & = unsafe(g_rd.layers[l])
    let dim = g_rd.dim
    let cd = g_rd.dn_cd
    let di = g_rd.dn_di
    let nvh = g_rd.dn_nvh
    let ds = g_rd.dn_ds
    let dconv = g_rd.dn_dconv
    let np = g_rd.pf_np
    let eps = g_rd.eps
    let x6 = pf_dnx6(l)
    let xq8k = !x6 && kq_sb(L.fqkv)
    if (x6) {
        var pcc0 = gk_f16_rows_args(uint(wlen * dim))
        enc_tower_clamp_cvt_cls(raw, h, L.pf_dn_cvt_set, pcc0, (wlen * dim / 4l + 255l) / 256l)
    }
    let ba_q = !g_rd.dn_ba_f32
    var feed_kq = xq8k   //! the quant feed's activation class in pf_xq / pf_xs: the qkv plane's off the f16 feed, else the beta plane's
    if (!x6 || ba_q) {
        feed_kq = x6 ? kq_sb(L.fbeta) : xq8k
        rq_enc(raw, h, g_rd.pf_sets[b + 0], feed_kq, 0l, wlen * dim)
    }
    pfq_ts(raw)
    let xf = pf_feed_planes(x6, g_rd.pf_xf, g_rd.pf_xq, g_rd.pf_xs, dim)
    let pqkv = arena_planes(L.fqkv, L.bqkv)
    pf_gemm_enc(raw, h, b + 1, L.fqkv, pqkv.wq, pqkv.ws, xf.q, xf.s, g_rd.pf_dnqkv,
        pqkv.wqb, pqkv.wsb, xf.qn, xf.sn, np * cd * 4l, VHZ_XQ, VHZ_DNP, dim, cd, L.bqkv, wlen, x6, cm2_tiles(di, wlen))
    let pz = arena_planes(L.fz, L.bz)
    pf_gemm_enc(raw, h, b + 2, L.fz, pz.wq, pz.ws, xf.q, xf.s, g_rd.pf_dnz,
        pz.wqb, pz.wsb, xf.qn, xf.sn, np * di * 4l, VHZ_XQ, VHZ_DNZ, dim, di, L.bz, wlen, x6, cm2_tiles(cd, wlen))
    if (g_rd.dn_ba_f32) {
        var pcb = DnBaArgs(npos = uint(wlen), dim = uint(dim), nvh = uint(nvh), woff = uint(L.ba_off), xstride = uint(dim),
            obase_b = uint(DN_SM_BETA), obase_g = uint(DN_SM_G), ostride = uint(nvh))
        enc_dn_ba_cls(raw, h, L.pf_dn_ba_set, pcb, dn_ba_wgs(wlen, nvh))
        pfq_ts(raw)
    } else {
        if (kq_sb(L.fbeta) != feed_kq) {
            rq_enc(raw, h, g_rd.pf_sets[b + 0], kq_sb(L.fbeta), 0l, wlen * dim)
        }
        let pb = arena_planes(L.fbeta, L.bbeta)
        pf_gemm_enc(raw, h, b + 3, L.fbeta, pb.wq, pb.ws, g_rd.pf_xq, g_rd.pf_xs, g_rd.pf_dnbg,
            pb.wqb, pb.wsb, np * dim, np * (dim / 32l) * 4l, np * nvh * 4l, VHZ_XQ, VHZ_DNBG, dim, nvh, L.bbeta, wlen, false)
        vhz_dep(raw, h, VHZ_DNBG, VHZ_DNSM, true)
        cmd_copy_range(raw, g_rd.pf_dnbg, 0l, L.dn[0].smalls_dev, DN_SM_BETA * 4l, wlen * nvh * 4l)
        if (kq_sb(L.falpha) != kq_sb(L.fbeta)) {
            rq_enc(raw, h, g_rd.pf_sets[b + 0], kq_sb(L.falpha), 0l, wlen * dim)
        }
        let pa = arena_planes(L.falpha, L.balpha)
        pf_gemm_enc(raw, h, b + 9, L.falpha, pa.wq, pa.ws, g_rd.pf_xq, g_rd.pf_xs, g_rd.pf_dnbg,
            pa.wqb, pa.wsb, np * dim, np * (dim / 32l) * 4l, np * nvh * 4l, VHZ_XQ, VHZ_DNBG, dim, nvh, L.balpha, wlen, false)
        vhz_dep(raw, h, VHZ_DNBG, VHZ_DNSM, true)
        cmd_copy_range(raw, g_rd.pf_dnbg, 0l, L.dn[0].smalls_dev, DN_SM_G * 4l, wlen * nvh * 4l)
    }
    dn_window_parts_enc(raw, h, L, wlen, cd, g_rd.dn_kd, nvh, g_rd.dn_nkh, ds, di, dconv, g_rd.dn_zsig, eps)
    pfq_ts(raw)
    let o6 = pf_dno6(l)
    pf_feed_enc(raw, h, o6 ? g_rd.pf_dnof_set : g_rd.pf_dnrq_set, o6, kq_sb(L.fout), DN_O_OFF, wlen * di)
    pfq_ts(raw)
    let pout = arena_planes(L.fout, L.bout)
    pf_gemm_enc(raw, h, b + 7, L.fout, pout.wq, pout.ws, o6 ? g_rd.pf_dnof : g_gpu.hq_dev, o6 ? g_rd.dummy : g_gpu.hs_dev, g_rd.pf_xb2,
        pout.wqb, pout.wsb, o6 ? np * di * 2l : BATCH_HQ_BYTES, o6 ? 256l : BATCH_HS_BYTES, np * dim * 4l, VHZ_HQ, VHZ_XB2, di, dim, L.bout, wlen, o6)
}

// the prefill profile's stamps per layer: the attention head's roles (+ qkn), or the recurrent head's
// (rq_x qkv z [beta alpha | ba] conv scan onorm tail rq_o out), then the shared FFN tail's seven; an MoE
// layer adds the routed block's eight and drops the dense block's four when it has no shared expert
def private pf_roles_per_layer(L : RLayer) : int64 {
    var n = L.recurrent ? 7l + (g_rd.dn_ba_f32 ? 10l : 11l) : (g_rd.qk_norm ? 16l : 15l)
    if (L.ple) {
        n += 4l   //! the per-layer-embedding branch: the residual + requant, the gate GEMM, the act, the proj GEMM
    }
    if (L.moe) {
        n += (L.experts_host ? 1l : PF_MOE_ROLES) - (has_dense_triple(L) ? 0l : 4l) + (L.moe_g4 ? 2l : 0l)   //! the gemma-4 block stamps its router norm and its routed-feed norm; a host layer stamps its router alone
    }
    return n
}

//! the routed block's stamps of an MoE layer's tail past ar1 / rq_f at `m0`: the shared expert's four when the layer has one, then the block's eight (ten on the gemma-4 block, whose two norms sit around topk) and ar2
def private pf_moe_tail_text(acc : double[26]; m0 : int; shexp, g4 : bool) : string {
    let s = shexp ? "sh_gate {int(acc[m0])} sh_up {int(acc[m0 + 1])} sh_act {int(acc[m0 + 2])} sh_down {int(acc[m0 + 3])} " : ""
    let r = m0 + (shexp ? 4 : 0)
    let rt = g4 ? "rt_norm {int(acc[r])} router {int(acc[r + 1])} topk {int(acc[r + 2])} pre2_norm {int(acc[r + 3])} " : "router {int(acc[r])} topk {int(acc[r + 1])} "
    let e = r + (g4 ? 4 : 2)
    return "{s}{rt}sched {int(acc[e])} gather {int(acc[e + 1])} e_gate {int(acc[e + 2])} e_up {int(acc[e + 3])} e_act {int(acc[e + 4])} e_down {int(acc[e + 5])} ar2+comb {int(acc[e + 6])}"
}

let private PF_MOE_ROLES = 8l   //! router topk sched gather e_gate e_up e_act e_down (the combine rides the layer's ar2 stamp)

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def vk_rdec_moe_ok(f1, f3, f2 : int) : bool {
    if (!vk_moe_init()) {
        return false
    }
    if (pf_moe_tiles()) {
        return pf_f16_feed(f1) && pf_f16_feed(f3) && pf_f16_feed(f2)
    }
    return pf_q8_tile_ok(f1) && pf_q8_tile_ok(f3) && pf_q8_tile_ok(f2)   //! the Q8-fed chain
}

//! the routed block's f16-fed tiles serve on this device: the cm2 tiles on a coopmat2 device in cm2 mode, the KHR tile in mm mode
def private pf_moe_tiles : bool => (g_gpu.coopmat_mode == COOPMAT_CM2 && g_gpu.has_coopmat2) || (g_gpu.coopmat_mode == COOPMAT_MM && khr_kq_tile_on())

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-moe-q8-chain")]
def private pf_q8_tile_ok(f : int) : bool => f == int(KqFmt.q8) || kq_sb(f) || blk32_rail(f)

//! the Q8-fed chain's expert tile set over [w, ws, schedule, xq, xs, y]
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-moe-q8-chain")]
def private pf_q8_tile_set(fmt : int; bufs : uint64 const[6]; sizes : int64 const[6]; gbits : uint const[6]) : VkDescriptorSet {
    if (fmt == int(KqFmt.q8)) {
        return set_q8_batch_cls(bufs, sizes, gbits)
    }
    return kq_batch_cls_set_for(fmt, false, bufs, sizes, gbits)
}

//! ... and its dispatch
def private pf_q8_tile_enc(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : BatchArgs; groups : int64) {
    if (fmt == int(KqFmt.q8)) {
        enc_q8_batch_cls(raw, h, set_, pc, groups)
    } else {
        kq_batch_cls_enc_for(fmt, false, raw, h, set_, pc, groups)
    }
}

def private pf_moe_bounds(nk : int64) : tuple<gu : int64; dn : int64; mgu : int64; mdn : int64> => moe_sched_bounds(g_rd.ne, g_rd.nfe, g_rd.dim, nk, g_rd.pf_moe_q8)

//! the window chain's MoE block buffers and class rails, once per residency
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block"), arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain")]
def private pf_moe_setup {
    let np = g_rd.pf_np
    let dim = g_rd.dim
    let nfe = g_rd.nfe
    let nk = np * g_rd.k
    g_rd.pf_nk = nk
    var any_dev = false
    for (L in g_rd.layers) {
        any_dev = any_dev || (L.moe && !L.experts_host)
    }
    if (g_rd.experts_host) {
        verify(ensure_router_gemm_cls() && (g_rd.hc || ensure_cls_ar_comb() && ensure_cls_ar_comb_f16_b()), "vk prefill: the router and host-combine class rails must engage on a live device")
        g_rd.pf_mlog = make_device_buf(np * g_rd.ner * 4l)
        pf_host_sum_ensure()
        g_rd.pf_ident_dev = make_ramp_buf(np, 0l)
        g_rd.pf_ones_dev = make_device_buf(np * 4l)
        upload_fill(g_rd.pf_ones_dev, 0l, np * 4l, F32_ONE_WORD)
        return if (!any_dev)
    }
    g_rd.pf_moe_q8 = !pf_moe_tiles()
    let q8c = g_rd.pf_moe_q8
    verify(ensure_router_gemm_cls() && topk_rows_ensure(g_rd.ne) && moe_sched_ensure(g_rd.ne) && ensure_cls_ar_comb() && ensure_cls_ar_comb_f16_b()
        && (q8c ? (ensure_moe_gather_f32_cls() && ensure_cls_dn_rq() && ensure_cls_q8k_rq() && ensure_q8_actrq_cls() && ensure_q8k_actrq_cls()
            && ensure_q8_batch_cls() && (!g_rd.exps_bias || ensure_q8_actrq_b_cls()))
            : (ensure_moe_gather_f16_cls() && ensure_actf16_cls() && (!g_rd.exps_bias || ensure_actf16_b_cls()))),
        "vk prefill: the MoE block's class rails must engage on a live device")
    let bounds = pf_moe_bounds(nk)
    assert((MOE_SCHED_MAP_OFF + max(bounds.gu + bounds.mgu, bounds.dn + bounds.mdn)) * 4l <= BATCH_META_BYTES, "dasLLAMA vulkan tier: the MoE block's schedule exceeds its plane")
    assert(max(bounds.gu, bounds.dn) <= 65535l, "dasLLAMA vulkan tier: the MoE block's s dispatch outgrows the schedule's 16-bit workgroup count")
    if (!g_rd.experts_host) {
        g_rd.pf_mlog = make_device_buf(np * g_rd.ner * 4l)
    }
    g_rd.pf_midx = make_device_buf(nk * 4l)
    g_rd.pf_mw = make_device_buf(nk * 4l)
    g_rd.pf_minv = make_device_buf(nk * 4l)
    g_rd.pf_bex = make_device_buf(nk * 4l)
    g_rd.pf_msch1 = make_device_buf(BATCH_META_BYTES)
    g_rd.pf_msch3 = make_device_buf(BATCH_META_BYTES)
    g_rd.pf_msch2 = make_device_buf(BATCH_META_BYTES)
    if (!q8c) {
        g_rd.pf_mxg = make_device_buf((nk + TILE_READ_SLACK) * dim * 2l)
        g_rd.pf_mhf = make_device_buf((nk + TILE_READ_SLACK) * nfe * 2l)
    }
    g_rd.pf_my1 = make_device_buf(nk * nfe * 4l)
    g_rd.pf_my2 = make_device_buf(nk * nfe * 4l)
    g_rd.pf_mdn = make_device_buf(nk * dim * 4l)
    g_rd.pf_midx_host = make_host_buf(nk * 4l, false, [cached = true])
    if (q8c) {
        g_rd.pf_mxg32 = make_device_buf(nk * dim * 4l)
        g_rd.pf_mgq = make_device_buf((nk + TILE_READ_SLACK) * dim)
        g_rd.pf_mgs = make_device_buf((nk + TILE_READ_SLACK) * (dim / 32l) * 4l)
        assert(nk * nfe <= BATCH_HQ_BYTES, "dasLLAMA vulkan tier: the Q8-fed chain's hidden rows exceed the act's quant plane")
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block"), arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command"), arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain")]
def private pf_moe_layer_sets(l : int64) {
    var L & = unsafe(g_rd.layers[l])
    let np = g_rd.pf_np
    let dim = g_rd.dim
    let nfe = g_rd.nfe
    let nk = g_rd.pf_nk
    let ner = g_rd.ner
    let tcs = int64(SCHED_S_ROWS)
    let tce = CM2_TC_E
    let xg_bytes = (nk + TILE_READ_SLACK) * dim * 2l
    let y_bytes = nk * nfe * 4l
    let hf_bytes = (nk + TILE_READ_SLACK) * nfe * 2l
    let dn_bytes = nk * dim * 4l
    L.pf_router_set = set_router_gemm_cls(fixed_array(g_rd.router_dev, g_rd.pf_xb, g_rd.pf_mlog),
        fixed_array(g_rd.router_bytes, np * dim * 4l, np * ner * 4l), fixed_array(0u, VHZ_XB, VHZ_MRT))
    if (L.experts_host) {
        if (!g_rd.hc && L.moe_g4) {   //! the host's sums land as each row's one routed row of weight one
            verify(ensure_cls_ar_comb_g4() && ensure_cls_ar_comb_g4_f16_b(), "vk prefill: a host layer's gemma-4 combine class rails must engage on a live device")
            L.pf_arcomb_set = set_cls_ar_comb_g4(fixed_array(g_rd.pf_x, g_rd.pf_ident_dev, g_rd.pf_ones_dev, g_rd.pf_hacc_dev, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xb),
                fixed_array(np * dim * 4l, np * 4l, np * 4l, np * dim * 4l, np * dim * 4l, g_rd.norms_bytes, np * dim * 4l),
                fixed_array(VHZ_X, 0u, 0u, VHZ_MDN, VHZ_FFO, 0u, VHZ_XB))
            L.pf_arcomb_f16_set = set_cls_ar_comb_g4_f16_b(fixed_array(g_rd.pf_x, g_rd.pf_ident_dev, g_rd.pf_ones_dev, g_rd.pf_hacc_dev, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xf),
                fixed_array(np * dim * 4l, np * 4l, np * 4l, np * dim * 4l, np * dim * 4l, g_rd.norms_bytes, np * dim * 2l),
                fixed_array(VHZ_X, 0u, 0u, VHZ_MDN, VHZ_FFO, 0u, VHZ_XQ))
        } elif (!g_rd.hc) {
            L.pf_arcomb_set = set_cls_ar_comb(fixed_array(g_rd.pf_x, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xb, g_rd.pf_ident_dev, g_rd.pf_ones_dev, g_rd.pf_hacc_dev, g_rd.pf_mlog, g_rd.moe_db_dev, g_rd.pf_ident_dev),
                fixed_array(np * dim * 4l, np * dim * 4l, g_rd.norms_bytes, np * dim * 4l, np * 4l, np * 4l, np * dim * 4l, np * ner * 4l, g_rd.moe_db_bytes, np * 4l),
                fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB, 0u, 0u, VHZ_MDN, VHZ_MRT, 0u, 0u))
            L.pf_arcomb_f16_set = set_cls_ar_comb_f16_b(fixed_array(g_rd.pf_x, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xf, g_rd.pf_ident_dev, g_rd.pf_ones_dev, g_rd.pf_hacc_dev, g_rd.pf_mlog, g_rd.moe_db_dev, g_rd.pf_ident_dev),
                fixed_array(np * dim * 4l, np * dim * 4l, g_rd.norms_bytes, np * dim * 2l, np * 4l, np * 4l, np * dim * 4l, np * ner * 4l, g_rd.moe_db_bytes, np * 4l),
                fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XQ, 0u, 0u, VHZ_MDN, VHZ_MRT, 0u, 0u))
        }
        return
    }
    let pe1 = arena_planes(L.fe1, L.be1)
    let pe3 = arena_planes(L.fe3, L.be3)
    let pe2 = arena_planes(L.fe2, L.be2)
    if (g_rd.pf_moe_q8) {
        pf_moe_q8_layer_sets(l, pe1, pe3, pe2)
    } else {
        verify(cm2_cls_ensure(L.fe1, tcs) && cm2_cls_ensure(L.fe3, tcs) && cm2_cls_ensure(L.fe2, tcs)
            && cm2_cls_ensure(L.fe1, tce) && cm2_cls_ensure(L.fe3, tce) && cm2_cls_ensure(L.fe2, tce), "vk prefill: the expert tile class rails must engage")
    }
    if (pf_f16g_on()) {
        verify(f16_gemm_ensure(false) && ensure_splitk_reduce_cls() && ensure_tower_clamp_cvt_cls(), "vk prefill: the router's f16 GEMM rails must engage")
        L.pf_router_cvt_set = set_tower_clamp_cvt_cls(fixed_array(g_rd.pf_xb, g_rd.pf_xf),
            fixed_array(np * dim * 4l, np * dim * 2l), fixed_array(VHZ_XB, VHZ_XQ))
        L.pf_f16g_router_set = f16_gemm_set(false, fixed_array(g_rd.router16_dev, g_rd.pf_xf, g_rd.pf_splitk),
            fixed_array(g_rd.router16_bytes, np * dim * 2l, g_rd.pf_splitk_bytes), fixed_array(0u, VHZ_XQ, VHZ_SK))
        L.pf_f16g_router_red_set = set_splitk_reduce_cls(fixed_array(g_rd.pf_splitk, g_rd.pf_mlog),
            fixed_array(g_rd.pf_splitk_bytes, np * ner * 4l), fixed_array(VHZ_SK, VHZ_MRT))
    }
    L.pf_topk_set = set_topk_rows_cls(fixed_array(g_rd.pf_mlog, g_rd.pf_mw, g_rd.moe_ds_dev, g_rd.moe_rb_dev, g_rd.pf_midx),
        fixed_array(np * ner * 4l, nk * 4l, g_rd.moe_ds_bytes, g_rd.moe_rb_bytes, nk * 4l), fixed_array(VHZ_MRT, VHZ_MRT, 0u, 0u, VHZ_MRT))
    L.pf_sched_set = set_moe_sched_cls(fixed_array(g_rd.pf_midx, g_rd.pf_minv, g_rd.pf_msch1, g_rd.pf_msch3, g_rd.pf_msch2, g_rd.pf_bex),
        fixed_array(nk * 4l, nk * 4l, BATCH_META_BYTES, BATCH_META_BYTES, BATCH_META_BYTES, nk * 4l),
        fixed_array(VHZ_MRT, VHZ_MRT, VHZ_MRT, VHZ_MRT, VHZ_MRT, VHZ_MRT))
    if (!g_rd.pf_moe_q8) {
        L.pf_gather_set = set_moe_gather_f16_cls(fixed_array(g_rd.pf_xb, g_rd.pf_minv, g_rd.pf_mxg),
            fixed_array(np * dim * 4l, nk * 4l, xg_bytes), fixed_array(VHZ_XB, VHZ_MRT, VHZ_MXG))
        L.pf_e1_set = cm2_cls_set(L.fe1, tcs, fixed_array(pe1.wq, pe1.ws, g_rd.pf_msch1, g_rd.pf_mxg, g_rd.pf_my1),
            fixed_array(pe1.wqb, pe1.wsb, BATCH_META_BYTES, xg_bytes, y_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_MXG, VHZ_GATE))
        L.pf_e1m_set = cm2_cls_set(L.fe1, tce, fixed_array(pe1.wq, pe1.ws, g_rd.pf_msch1, g_rd.pf_mxg, g_rd.pf_my1),
            fixed_array(pe1.wqb, pe1.wsb, BATCH_META_BYTES, xg_bytes, y_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_MXG, VHZ_GATE_M))
        L.pf_e3_set = cm2_cls_set(L.fe3, tcs, fixed_array(pe3.wq, pe3.ws, g_rd.pf_msch3, g_rd.pf_mxg, g_rd.pf_my2),
            fixed_array(pe3.wqb, pe3.wsb, BATCH_META_BYTES, xg_bytes, y_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_MXG, VHZ_UP))
        L.pf_e3m_set = cm2_cls_set(L.fe3, tce, fixed_array(pe3.wq, pe3.ws, g_rd.pf_msch3, g_rd.pf_mxg, g_rd.pf_my2),
            fixed_array(pe3.wqb, pe3.wsb, BATCH_META_BYTES, xg_bytes, y_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_MXG, VHZ_UP_M))
        if (g_rd.exps_bias) {   //! the biased act: each bucket row's expert (the schedule's map) names its gate and up bias rows
            L.pf_eact_set = set_actf16_b_cls(fixed_array(g_rd.pf_my1, g_rd.pf_my2, g_rd.pf_mhf, g_rd.moe_gb_dev, g_rd.moe_ub_dev, g_rd.pf_bex),
                fixed_array(y_bytes, y_bytes, hf_bytes, g_rd.moe_gub_bytes, g_rd.moe_gub_bytes, nk * 4l), fixed_array(VHZ_GATE | VHZ_GATE_M, VHZ_UP | VHZ_UP_M, VHZ_HQ, 0u, 0u, VHZ_MRT))
        } else {
            L.pf_eact_set = set_actf16_cls(fixed_array(g_rd.pf_my1, g_rd.pf_my2, g_rd.pf_mhf),
                fixed_array(y_bytes, y_bytes, hf_bytes), fixed_array(VHZ_GATE | VHZ_GATE_M, VHZ_UP | VHZ_UP_M, VHZ_HQ))
        }
        L.pf_e2_set = cm2_cls_set(L.fe2, tcs, fixed_array(pe2.wq, pe2.ws, g_rd.pf_msch2, g_rd.pf_mhf, g_rd.pf_mdn),
            fixed_array(pe2.wqb, pe2.wsb, BATCH_META_BYTES, hf_bytes, dn_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_HQ, VHZ_MDN))
        L.pf_e2m_set = cm2_cls_set(L.fe2, tce, fixed_array(pe2.wq, pe2.ws, g_rd.pf_msch2, g_rd.pf_mhf, g_rd.pf_mdn),
            fixed_array(pe2.wqb, pe2.wsb, BATCH_META_BYTES, hf_bytes, dn_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_HQ, VHZ_MDN_M))
    }
    if (L.moe_g4) {
        verify(ensure_cls_ar_comb_g4() && ensure_cls_ar_comb_g4_f16_b(), "vk prefill: the gemma-4 combine class rails must engage on a live device")
        L.pf_arcomb_set = set_cls_ar_comb_g4(fixed_array(g_rd.pf_x, g_rd.pf_minv, g_rd.pf_mw, g_rd.pf_mdn, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xb),
            fixed_array(np * dim * 4l, nk * 4l, nk * 4l, dn_bytes, np * dim * 4l, g_rd.norms_bytes, np * dim * 4l),
            fixed_array(VHZ_X, VHZ_MRT, VHZ_MRT, VHZ_MDN | VHZ_MDN_M, VHZ_FFO, 0u, VHZ_XB))
        L.pf_arcomb_f16_set = set_cls_ar_comb_g4_f16_b(fixed_array(g_rd.pf_x, g_rd.pf_minv, g_rd.pf_mw, g_rd.pf_mdn, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xf),
            fixed_array(np * dim * 4l, nk * 4l, nk * 4l, dn_bytes, np * dim * 4l, g_rd.norms_bytes, np * dim * 2l),
            fixed_array(VHZ_X, VHZ_MRT, VHZ_MRT, VHZ_MDN | VHZ_MDN_M, VHZ_FFO, 0u, VHZ_XQ))
        return
    }
    L.pf_arcomb_set = set_cls_ar_comb(fixed_array(g_rd.pf_x, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xb, g_rd.pf_minv, g_rd.pf_mw, g_rd.pf_mdn, g_rd.pf_mlog, g_rd.moe_db_dev, g_rd.pf_midx),
        fixed_array(np * dim * 4l, np * dim * 4l, g_rd.norms_bytes, np * dim * 4l, nk * 4l, nk * 4l, dn_bytes, np * ner * 4l, g_rd.moe_db_bytes, nk * 4l),
        fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB, VHZ_MRT, VHZ_MRT, VHZ_MDN | VHZ_MDN_M, VHZ_MRT, 0u, VHZ_MRT))
    L.pf_arcomb_f16_set = set_cls_ar_comb_f16_b(fixed_array(g_rd.pf_x, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xf, g_rd.pf_minv, g_rd.pf_mw, g_rd.pf_mdn, g_rd.pf_mlog, g_rd.moe_db_dev, g_rd.pf_midx),
        fixed_array(np * dim * 4l, np * dim * 4l, g_rd.norms_bytes, np * dim * 2l, nk * 4l, nk * 4l, dn_bytes, np * ner * 4l, g_rd.moe_db_bytes, nk * 4l),
        fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XQ, VHZ_MRT, VHZ_MRT, VHZ_MDN | VHZ_MDN_M, VHZ_MRT, 0u, VHZ_MRT))
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-moe-q8-chain")]
def private pf_moe_q8_layer_sets(l : int64; pe1, pe3, pe2 : tuple<wq : uint64; ws : uint64; wqb : int64; wsb : int64>) {
    var L & = unsafe(g_rd.layers[l])
    let np = g_rd.pf_np
    let dim = g_rd.dim
    let nfe = g_rd.nfe
    let nk = g_rd.pf_nk
    let gq = (nk + TILE_READ_SLACK) * dim
    let gs = (nk + TILE_READ_SLACK) * (dim / 32l) * 4l
    let y_bytes = nk * nfe * 4l
    verify(kq_batch_cls_ensure(L.fe1, false) || L.fe1 == int(KqFmt.q8), "vk prefill: the Q8-fed chain's gate tile must engage")
    verify(kq_batch_cls_ensure(L.fe3, false) || L.fe3 == int(KqFmt.q8), "vk prefill: the Q8-fed chain's up tile must engage")
    verify(kq_batch_cls_ensure(L.fe2, false) || L.fe2 == int(KqFmt.q8), "vk prefill: the Q8-fed chain's down tile must engage")
    L.pf_gather_set = set_moe_gather_f32_cls(fixed_array(g_rd.pf_xb, g_rd.pf_minv, g_rd.pf_mxg32),
        fixed_array(np * dim * 4l, nk * 4l, nk * dim * 4l), fixed_array(VHZ_XB, VHZ_MRT, VHZ_MXG))
    L.pf_grq_set = set_rq_cls(fixed_array(g_rd.pf_mxg32, g_rd.pf_mgq, g_rd.pf_mgs), fixed_array(nk * dim * 4l, gq, gs),
        fixed_array(VHZ_MXG, VHZ_MXG, VHZ_MXG))
    L.pf_e1_set = pf_q8_tile_set(L.fe1, fixed_array(pe1.wq, pe1.ws, g_rd.pf_msch1, g_rd.pf_mgq, g_rd.pf_mgs, g_rd.pf_my1),
        fixed_array(pe1.wqb, pe1.wsb, BATCH_META_BYTES, gq, gs, y_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_MXG, VHZ_MXG, VHZ_GATE))
    L.pf_e3_set = pf_q8_tile_set(L.fe3, fixed_array(pe3.wq, pe3.ws, g_rd.pf_msch3, g_rd.pf_mgq, g_rd.pf_mgs, g_rd.pf_my2),
        fixed_array(pe3.wqb, pe3.wsb, BATCH_META_BYTES, gq, gs, y_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_MXG, VHZ_MXG, VHZ_UP))
    if (g_rd.exps_bias) {   //! the biased act: each bucket row's expert (the schedule's map) names its gate and up bias rows
        L.pf_eact_set = set_q8_actrq_b_cls(fixed_array(g_rd.pf_my1, g_rd.pf_my2, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.moe_gb_dev, g_rd.moe_ub_dev, g_rd.pf_bex),
            fixed_array(y_bytes, y_bytes, BATCH_HQ_BYTES, BATCH_HS_BYTES, g_rd.moe_gub_bytes, g_rd.moe_gub_bytes, nk * 4l),
            fixed_array(VHZ_GATE, VHZ_UP, VHZ_HQ, VHZ_HQ, 0u, 0u, VHZ_MRT))
    } else {
        L.pf_eact_set = set_actrq_cls(fixed_array(g_rd.pf_my1, g_rd.pf_my2, g_gpu.hq_dev, g_gpu.hs_dev),
            fixed_array(y_bytes, y_bytes, BATCH_HQ_BYTES, BATCH_HS_BYTES), fixed_array(VHZ_GATE, VHZ_UP, VHZ_HQ, VHZ_HQ))
    }
    L.pf_e2_set = pf_q8_tile_set(L.fe2, fixed_array(pe2.wq, pe2.ws, g_rd.pf_msch2, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.pf_mdn),
        fixed_array(pe2.wqb, pe2.wsb, BATCH_META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, nk * dim * 4l), fixed_array(0u, 0u, VHZ_MRT, VHZ_HQ, VHZ_HQ, VHZ_MDN))
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block"), arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain")]
def private pf_moe_router(raw : VkCommandBuffer; var h : VkHaz; l, wlen : int64) {
    var L & = unsafe(g_rd.layers[l])
    let dim = g_rd.dim
    let ner = g_rd.ner
    let addrms_set = int(l * PF_ROLES) + 8
    if (L.moe_g4) {   //! the router's input: a weightless rms of x under the router scale over sqrt(dim)
        var pcn = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = uint(rd_norm_off(l, RD_NORM_ROUTER)), eps = g_rd.eps, ascale = 1.0)
        enc_cls_ar(raw, h, g_rd.pf_sets[addrms_set], pcn, wlen)
        pfq_ts(raw)
    }
    if (pf_f16g_on() && !L.experts_host) {
        var pcc = gk_f16_rows_args(uint(wlen * dim))
        enc_tower_clamp_cvt_cls(raw, h, L.pf_router_cvt_set, pcc, (wlen * dim / 4l + 255l) / 256l)
        pf_f16g_enc(raw, h, L.pf_f16g_router_set, L.pf_f16g_router_red_set, dim, ner, wlen, L.router_slot * ner * dim, 0l)
    } else {
        router_gemm_enc(raw, h, L.pf_router_set, wlen, ner, dim, L.router_slot * ner * dim)
    }
    pfq_ts(raw)
}

def private pf_host_sum_ensure {
    return if (g_rd.pf_hacc_dev != 0ul)
    let xbytes = g_rd.pf_np * g_rd.dim * 4l
    g_rd.pf_hacc_dev = make_device_buf(xbytes)
    g_rd.pf_hx_host = make_host_buf(xbytes, false, [cached = true])
    g_rd.pf_hlog_host = make_host_buf(g_rd.pf_np * g_rd.ner * 4l, false, [cached = true])
    g_rd.pf_hacc_host = make_host_buf(xbytes, false, [cached = true])
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain")]
def private pf_host_cut(var raw : VkCommandBuffer&; var next : VkCommandBuffer; var h : VkHaz; l, wlen, row0 : int64; experts : RdecHostExpertsRowsFn) {
    let dim = g_rd.dim
    host_land(raw, h, g_rd.pf_xb, g_rd.pf_hx_host, g_rd.pf_mlog, g_rd.pf_hlog_host, wlen)
    cmd_close(raw)
    submit_wait(raw)
    unsafe {
        invoke(experts, l, reinterpret<float const?>(g_rd.pf_hx_host.mapped), reinterpret<float const?>(g_rd.pf_hlog_host.mapped),
            reinterpret<float?>(g_rd.pf_hacc_host.mapped), 0l, wlen)
    }
    raw = next
    h = cmd_open(raw, true)
    vhz_dep(raw, h, 0u, VHZ_MDN, true)
    cmd_copy_range(raw, g_rd.pf_hacc_host.buf, row0 * dim * 4l, g_rd.pf_hacc_dev, row0 * dim * 4l, (wlen - row0) * dim * 4l)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-moe-q8-chain")]
def private pf_moe_q8_experts(raw : VkCommandBuffer; var h : VkHaz; l, nk : int64; bounds : tuple<gu : int64; dn : int64; mgu : int64; mdn : int64>) {
    var L & = unsafe(g_rd.layers[l])
    let dim = g_rd.dim
    let nfe = g_rd.nfe
    let ne = uint(g_rd.ne)
    rq_enc(raw, h, L.pf_grq_set, kq_sb(L.fe1), 0l, nk * dim)
    pfq_ts(raw)
    var pc1 = BatchArgs(n = uint(dim), d = uint(nfe), map_off = uint(MOE_SCHED_MAP_OFF), ksplit = 0u)
    pf_q8_tile_enc(L.fe1, raw, h, L.pf_e1_set, pc1, bounds.gu)
    pfq_ts(raw)
    pf_q8_tile_enc(L.fe3, raw, h, L.pf_e3_set, pc1, bounds.gu)
    pfq_ts(raw)
    var pca = ActArgs(nelem = uint(nk * nfe), act = act_code(g_rd.act_oai, g_rd.is_gelu), bias_on = g_rd.exps_bias ? 1u : 0u,
        gboff = uint(L.router_slot) * ne * uint(nfe), brow = uint(nfe))
    if (g_rd.exps_bias) {
        pca.nblk = uint(nk * nfe / 32l)
        enc_q8_actrq_b_cls(raw, h, L.pf_eact_set, pca, rq_wgs(false, nk * nfe))
    } else {
        actrq_enc(raw, h, L.pf_eact_set, kq_sb(L.fe2), pca, nk * nfe)
    }
    pfq_ts(raw)
    var pc2 = BatchArgs(n = uint(nfe), d = uint(dim), map_off = uint(MOE_SCHED_MAP_OFF), ksplit = 0u)
    pf_q8_tile_enc(L.fe2, raw, h, L.pf_e2_set, pc2, bounds.dn)
    pfq_ts(raw)
}

//! the gemma-4 routed feed: x under the routed branch's pre norm into pf_xb, the rows the gather (or the host) reads
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-gemma4-moe-block")]
def private pf_moe_g4_feed(raw : VkCommandBuffer; var h : VkHaz; l, wlen : int64) {
    var pcn = rd_g4_pre2_args(l)
    enc_cls_ar(raw, h, g_rd.pf_sets[int(l * PF_ROLES) + 8], pcn, wlen)
    pfq_ts(raw)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def private pf_moe_block(raw : VkCommandBuffer; var h : VkHaz; l, wlen : int64) {
    var L & = unsafe(g_rd.layers[l])
    let dim = g_rd.dim
    let ne = g_rd.ne
    let k = g_rd.k
    let nfe = g_rd.nfe
    let nk = wlen * k
    pf_moe_router(raw, h, l, wlen)
    var pct = gk_topk_routing(ne, k, g_rd.norm_topk, g_rd.wscale, g_rd.ner, g_rd.router_bias, uint(L.router_slot * ne),
        L.moe_g4 ? uint(L.router_slot * ne) : GK_NO_DSCALE)
    topk_rows_enc(ne, raw, h, L.pf_topk_set, pct, wlen)
    pfq_ts(raw)
    if (L.moe_g4) {
        pf_moe_g4_feed(raw, h, l, wlen)
    }
    let bounds = pf_moe_bounds(nk)
    let tcs = int64(SCHED_S_ROWS)
    let tce = CM2_TC_E
    let map_m_gu = uint(MOE_SCHED_MAP_OFF + bounds.gu)   //! the m dispatch's maps follow the s dispatch's
    let map_m_dn = uint(MOE_SCHED_MAP_OFF + bounds.dn)
    let wcol = g_rd.pf_moe_q8 ? 32u : 128u   //! the expert tiles' output columns: the sdot4 tile's 32, the cm2 and KHR tiles' 128
    var pcs = gk_sched_args(nk, ne, nfe, dim, wcol, g_rd.pf_moe_q8, bounds, arena_local_blk(L.be1), arena_local_blk(L.be3), arena_local_blk(L.be2), L.fe1, L.fe3, L.fe2)
    moe_sched_enc(ne, raw, h, L.pf_sched_set, pcs)
    pfq_ts(raw)
    var pcg = GatherArgs(n = uint(dim), k = uint(k), r0 = 0u, r1 = uint(nk))
    if (g_rd.pf_moe_q8) {
        enc_moe_gather_f32_cls(raw, h, L.pf_gather_set, pcg, nk)
        pfq_ts(raw)
        pf_moe_q8_experts(raw, h, l, nk, bounds)
        return
    }
    enc_moe_gather_f16_cls(raw, h, L.pf_gather_set, pcg, nk)
    pfq_ts(raw)
    var pc1 = BatchArgs(n = uint(dim), d = uint(nfe), map_off = uint(MOE_SCHED_MAP_OFF), ksplit = 0u)
    var pc1m = BatchArgs(n = uint(dim), d = uint(nfe), map_off = map_m_gu, ksplit = 0u)
    cm2_cls_enc(L.fe1, tce, raw, h, L.pf_e1m_set, pc1m, bounds.mgu)
    cm2_cls_enc(L.fe1, tcs, raw, h, L.pf_e1_set, pc1, bounds.gu)
    pfq_ts(raw)
    cm2_cls_enc(L.fe3, tce, raw, h, L.pf_e3m_set, pc1m, bounds.mgu)
    cm2_cls_enc(L.fe3, tcs, raw, h, L.pf_e3_set, pc1, bounds.gu)
    pfq_ts(raw)
    var pca = ActArgs(nelem = uint(nk * nfe), act = act_code(g_rd.act_oai, g_rd.is_gelu), nblk = uint(nk * nfe / 32l), elem0 = 0u,
        bias_on = g_rd.exps_bias ? 1u : 0u, gboff = uint(L.router_slot * ne * nfe), brow = uint(nfe))
    if (g_rd.exps_bias) {
        enc_actf16_b_cls(raw, h, L.pf_eact_set, pca, (nk * nfe / 4l + 255l) / 256l)
    } else {
        enc_actf16_cls(raw, h, L.pf_eact_set, pca, (nk * nfe / 4l + 255l) / 256l)
    }
    pfq_ts(raw)
    var pc2 = BatchArgs(n = uint(nfe), d = uint(dim), map_off = uint(MOE_SCHED_MAP_OFF), ksplit = 0u)
    var pc2m = BatchArgs(n = uint(nfe), d = uint(dim), map_off = map_m_dn, ksplit = 0u)
    cm2_cls_enc(L.fe2, tce, raw, h, L.pf_e2m_set, pc2m, bounds.mdn)
    cm2_cls_enc(L.fe2, tcs, raw, h, L.pf_e2_set, pc2, bounds.dn)
    pfq_ts(raw)
}

//! the next layer's fused Q8 head skips its own requant, so this step encodes it.
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
//! The per-layer-embedding branch over a window's rows: gelu(gate . x) x side[p][l] -> proj -> pf_ffnout (the next-norm step adds it under the ple post norm)
def private pf_ple_block(raw : VkCommandBuffer; var h : VkHaz; l : int64; b : int; wlen, np : int64) {
    let dim = g_rd.dim
    let ple = g_rd.ple
    let all = uint(g_rd.n_layers * ple)   //! the side rows' position stride
    let L & = unsafe(g_rd.layers[l])
    let woff = l * ple * dim   //! the layer's first element in either f32 plane
    //! every step runs the whole window, the FFN's last-layer row slice included: the branch is 256 wide, and the sliced rows' results go unread
    let x6 = pf_ple_plane_f16(L.bpg, L.fpg)   //! the gate's feed: the residual rows as f16 (the tile pick and split-k serve the 256-wide GEMM) else their quants
    if (L.bpg < 0l && !x6) {   //! the file's f32 rows read the stream as it is, on the f32 tile
        pfq_ts(raw)
        var pcg = pf_ple_f32_args(wlen, dim, ple, woff)
        enc_dn_ba_cls(raw, h, g_rd.pf_sets[b + 17], pcg, dn_ba_wgs(wlen, ple / 2l))
        pfq_ts(raw)
    } else {
        pf_feed_enc(raw, h, g_rd.pf_sets[b + 16], x6, kq_sb(L.fpg) && !x6, 0l, wlen * dim)
        pfq_ts(raw)
        let xf = pf_feed_planes(x6, g_rd.pf_xf, g_rd.pf_xq, g_rd.pf_xs, dim)
        if (L.bpg < 0l) {
            pf_f32_gemm_enc(raw, h, b + 17, g_rd.ple_gf32_dev, g_rd.ple_gf32_bytes, xf.q, xf.qn, VHZ_XQ, g_rd.pf_gate, np * ple * 4l, VHZ_GATE, dim, ple, woff, wlen)
        } else {
            let pg = arena_planes(L.fpg, L.bpg)
            pf_gemm_enc(raw, h, b + 17, L.fpg, pg.wq, pg.ws, xf.q, xf.s, g_rd.pf_gate,
                pg.wqb, pg.wsb, xf.qn, xf.sn, np * ple * 4l, VHZ_XQ, VHZ_GATE, dim, ple, L.bpg, wlen, x6)
        }
    }
    let p6 = pf_ple_plane_f16(L.bpp, L.fpp)   //! the projection's feed: the act rows as f16 where the projection's tile reads them, else their quants or f32 rows
    if (L.bpp < 0l && !p6) {   //! the act's f32 rows (in the up plane, free here) feed the f32 projection
        var pcf = GkPleActArgs(nelem = uint(wlen * ple), sbase = uint(l * ple), sstride = all, slen = uint(ple))
        enc_ple_act_f32_cls(raw, h, g_rd.pf_sets[b + 18], pcf, (wlen * ple + 255l) / 256l)
        pfq_ts(raw)
        var pcp = pf_ple_f32_args(wlen, ple, dim, woff)
        enc_dn_ba_cls(raw, h, g_rd.pf_sets[b + 19], pcp, dn_ba_wgs(wlen, dim / 2l))
        pfq_ts(raw)
        return
    }
    var pca = ActArgs(nelem = uint(wlen * ple), act = ACT_GELU, ubase = uint(l * ple), ustride = all, ulen = uint(ple))
    if (p6) {
        enc_actf16_cls(raw, h, g_rd.pf_sets[b + 18], pca, (wlen * ple / 4l + 255l) / 256l)
    } else {
        actrq_enc(raw, h, g_rd.pf_sets[b + 18], kq_sb(L.fpp), pca, wlen * ple)
    }
    pfq_ts(raw)
    let pf = pf_feed_planes(p6, g_rd.pf_hf, g_gpu.hq_dev, g_gpu.hs_dev, ple)
    if (L.bpp < 0l) {
        pf_f32_gemm_enc(raw, h, b + 19, g_rd.ple_pf32_dev, g_rd.ple_pf32_bytes, pf.q, pf.qn, VHZ_HQ, g_rd.pf_ffnout, np * dim * 4l, VHZ_FFO, ple, dim, woff, wlen)
        return
    }
    let pp = arena_planes(L.fpp, L.bpp)
    pf_gemm_enc(raw, h, b + 19, L.fpp, pp.wq, pp.ws, pf.q, pf.s, g_rd.pf_ffnout,
        pp.wqb, pp.wsb, pf.qn, pf.sn, np * dim * 4l, VHZ_HQ, VHZ_FFO, ple, dim, L.bpp, wlen, p6)
}

//! the file's f32 per-layer-embedding rows ride the staged KHR tile where the device has it (`DASLLAMA_VK_PLE_F32_TILE` holds
//! them on the scalar f32 tile)
[arch(at="../ARCHITECTURE_GPU_VULKAN_KHR.md#f32-rows-staged-tile")]
def private pf_ple_f32_staged : bool => ple_f32_tile_ok(g_rd.dim, g_rd.ple, g_env_vulkan.vk_ple_f32_tile)

//! the staged tile serves f32 rows of a model dim `dim` and a ple width `ple`: both GEMMs' K on its 32-deep k step, so no 16-wide
//! row read reaches past the exactly sized plane; else the scalar f32 tile
[arch(at="../ARCHITECTURE_GPU_VULKAN_KHR.md#f32-rows-staged-tile")]
def public ple_f32_tile_ok(dim, ple : int64; knob : bool) : bool
    => knob && khr_kq_tile_on() && dim % 32l == 0l && ple % 32l == 0l

//! a per-layer-embedding plane at arena block `blk` in format `fmt` reads its window rows as f16: a plane the f16 feed admits, or
//! the file's f32 rows (block -1) on the staged tile
def private pf_ple_plane_f16(blk : int64; fmt : int) : bool => blk < 0l ? pf_ple_f32_staged() : pf_f16_feed(fmt)

//! the k chunks a staged f32-row GEMM of K `n` takes: the most, up to eight, `cm2_chunk` admits, each a dependent window chain's hop
//! cut to its deepest chunk's latency (one under 512)
[arch(at="../ARCHITECTURE_GPU_VULKAN_KHR.md#f32-rows-staged-tile")]
def pf_f32_split(n, rows, d, scratch_bytes : int64) : int64 => cm2_split_fit(n, n / 256l, rows * d * 4l, scratch_bytes)

//! one f32-row GEMM on the staged KHR tile: y[rows x d] = x[rows x n] w^T over the plane's `d` rows of `n` from element `woff`,
//! the f16 rows at `xf`; one region at row 0 and the tile's one column, over `pf_f32_split`'s k chunks into the split-k planes and
//! their reduce
[arch(at="../ARCHITECTURE_GPU_VULKAN_KHR.md#f32-rows-staged-tile")]
def private pf_f32_gemm_enc(raw : VkCommandBuffer; var h : VkHaz; idx : int; w : uint64; wn : int64; xf : uint64; xn : int64; xbit : uint;
                            y : uint64; yn : int64; ybit : uint; n, d, woff, rows : int64) {
    let nsplit = pf_f32_split(n, rows, d, g_rd.pf_splitk_bytes)
    let split = nsplit > 1l
    if (split) {
        verify(nsplit * rows * d * 4l <= g_rd.pf_splitk_bytes, "vk prefill: the staged f32 tile's split-k planes exceed the scratch bound")
    }
    let groups = int64(fill_arena_batch_sched(g_rd.pf_meta[idx], d, woff / 4l, rows, int(KqFmt.q8), true, nsplit, 0l, 128l))
    var pc = BatchArgs(n = uint(n), d = uint(d), ksplit = split ? uint(cm2_chunk(n, nsplit)) : 0u)
    var s = cached(g_rd.pf_cls_gemm, idx * 16 + 15) $ {
        verify(ensure_f32_batch_khr_cls(), "vk prefill: the staged f32 tile's class rail must engage")
        return set_f32_batch_khr_cls(fixed_array(w, g_rd.pf_meta[idx].buf, xf, split ? g_rd.pf_splitk : y),
            fixed_array(wn, BATCH_META_BYTES, xn, split ? g_rd.pf_splitk_bytes : yn), fixed_array(0u, 0u, xbit, split ? VHZ_SK : ybit))
    }
    enc_f32_batch_khr_cls(raw, h, s, pc, groups)
    if (split) {
        pf_splitk_reduce_enc(raw, h, idx, y, yn, ybit, rows * d, nsplit)
    }
    pfq_ts(raw)
}

//! a split GEMM role's reduce: the `nsplit` partial planes in the split-k scratch summed into `y`, the role's cached reduce set
def private pf_splitk_reduce_enc(raw : VkCommandBuffer; var h : VkHaz; idx : int; y : uint64; yn : int64; ybit : uint; nelem, nsplit : int64) {
    var rset = cached(g_rd.pf_cls_gemm, idx * 16 + 12) $ {
        verify(ensure_splitk_reduce_cls(), "vk prefill: the split-k reduce class rail must engage")
        return set_splitk_reduce_cls(fixed_array(g_rd.pf_splitk, y), fixed_array(g_rd.pf_splitk_bytes, yn), fixed_array(VHZ_SK, ybit))
    }
    splitk_reduce_enc(raw, h, rset, nelem, nsplit)
}

//! the f32 tile (`DnBaGemm`) as a plain GEMM: `d` rows of `n` at `woff` over the window's rows into y[p * d + e] - its two output
//! halves laid end to end
def private pf_ple_f32_args(wlen, n, d, woff : int64) : DnBaArgs
    => DnBaArgs(npos = uint(wlen), dim = uint(n), nvh = uint(d / 2l), woff = uint(woff), xstride = uint(n), obase_b = 0u,
        obase_g = uint(d / 2l), ostride = uint(d))

[arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain")]
def private pf_moe_addr_next(raw : VkCommandBuffer; var h : VkHaz; l, wlen, nxt : int64) {
    var L & = unsafe(g_rd.layers[l])
    let dim = g_rd.dim
    var pcc = rd_ar_comb_args(L, nxt)
    var pcg = rd_g4_comb_args(L, l, nxt, false)
    let next_fused = l + 1l < g_rd.n_layers && pf_qkv_feed_fused(l + 1l)
    if (next_fused && pf_qkv6(l + 1l)) {
        if (L.moe_g4) {
            enc_cls_ar_comb_g4_f16_b(raw, h, L.pf_arcomb_f16_set, pcg, wlen)
        } else {
            enc_cls_ar_comb_f16_b(raw, h, L.pf_arcomb_f16_set, pcc, wlen)
        }
        return
    }
    if (L.moe_g4) {
        enc_cls_ar_comb_g4(raw, h, L.pf_arcomb_set, pcg, wlen)
    } else {
        enc_cls_ar_comb(raw, h, L.pf_arcomb_set, pcc, wlen)
    }
    if (next_fused) {   //! the Q8 feed's requant, the next layer's own quant_xb set
        let nb = int((l + 1l) * PF_ROLES)
        rq_enc(raw, h, g_rd.pf_sets[nb], kq_sb(g_rd.layers[l + 1l].fq), 0l, wlen * dim)
    }
}

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain"), arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs"), arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-tq4-basis")]
def private pf_setup {
    if (g_rd.pf_ready) {
        return
    }
    let np = pf_window_rows()
    g_rd.pf_np = np
    if (g_env_vulkan.vk_f16_ffn) {
        to_log(LOG_INFO, "dasLLAMA: resident prefill - the FFN GEMMs take the f16 feed where the tile admits the plane (DASLLAMA_VK_F16_FFN=0 keeps the Q8 blocks)\n")
    }
    let dim = g_rd.dim
    let qd = g_rd.qd
    let kvd = g_rd.kv_dim
    let hid = g_rd.hidden
    let wide = max(max(dim, qd), hid)
    g_rd.pf_x = make_call_buf(np * dim * 4l)
    g_rd.pf_xb = make_device_buf(np * dim * 4l)
    g_rd.pf_xq = make_device_buf(np * wide)
    g_rd.pf_xs = make_device_buf(np * (wide / 32l) * 4l)
    let qrow = g_rd.q_gated ? 2l * qd : qd
    g_rd.pf_q = make_device_buf(np * qrow * 4l)
    g_rd.pf_kv = make_device_buf(np * 2l * kvd * 4l)
    if (rd_kv_block_codec()) {
        g_rd.pf_kst = make_device_buf(np * kvd * 4l)
        g_rd.pf_vst = make_device_buf(np * kvd * 4l)
    }
    g_rd.pf_v = make_device_buf(np * kvd * 4l)   // split-path v GEMM output (copied into pf_kv's v half); idle when the merged k|v GEMM serves the layer
    g_rd.pf_attn = make_device_buf(np * qd * 4l)
    g_rd.pf_aq = make_device_buf(np * qd)
    g_rd.pf_as = make_device_buf(np * (qd / 32l) * 4l)
    g_rd.pf_gate = make_device_buf(np * hid * 4l)
    g_rd.pf_up = make_device_buf(np * hid * 4l)
    g_rd.pf_ffnout = make_device_buf(np * dim * 4l)
    g_rd.pf_xb2 = make_device_buf(np * dim * 4l)
    g_rd.pf_cos = make_call_buf(np * g_rd.cos_elems * 4l)
    if (g_rd.ple > 0l) {   //! the window's per-layer-embedding side rows, position-major [np x (layers x ple)]
        let all = g_rd.n_layers * g_rd.ple
        g_rd.pf_ple = make_call_buf(np * all * 4l)
        if (g_rd.ple_gpu) {
            // the pre-step on device: the projected plane (the f16 tile over the window's panel, else the router GEMV), then the finish
            g_rd.pf_plep = make_device_buf(np * all * 4l)
            if (g_rd.ple_w16_dev != 0ul) {
                let ple_khr = g_gpu.coopmat_mode != COOPMAT_CM2
                verify(ensure_tower_clamp_cvt_cls() && ensure_ple_bf16_cvt_cls() && f16_gemm_ensure(ple_khr) && ensure_ple_finish_cls(),
                    "vk prefill: the per-layer-embedding pre-step's rails must engage")
                g_rd.pf_plexf = make_device_buf(np * dim * 2l)
                g_rd.pf_ple_cvt_set = set_tower_clamp_cvt_cls(fixed_array(g_rd.pf_x, g_rd.pf_plexf),
                    fixed_array(np * dim * 4l, np * dim * 2l), fixed_array(VHZ_X, VHZ_SK))
                let wbufs = fixed_array(g_rd.ple_wp_dev, g_rd.ple_w16_dev)
                let wsizes = fixed_array(g_rd.ple_wp_bytes, g_rd.ple_w16_bytes)
                let wbits = fixed_array(0u, VHZ_SK)
                g_rd.pf_ple_wcvt_set = g_rd.ple_wp_bf16 ? set_ple_bf16_cvt_cls(wbufs, wsizes, wbits) : set_tower_clamp_cvt_cls(wbufs, wsizes, wbits)
                let gbufs = fixed_array(g_rd.ple_w16_dev, g_rd.pf_plexf, g_rd.pf_plep)
                let gsizes = fixed_array(g_rd.ple_w16_bytes, np * dim * 2l, np * all * 4l)
                let ggbits = fixed_array(VHZ_SK, VHZ_SK, VHZ_MRT)
                g_rd.pf_ple_gemm_set = f16_gemm_set(ple_khr, gbufs, gsizes, ggbits)
            } else {
                let gbufs = fixed_array(g_rd.ple_wp_dev, g_rd.pf_x, g_rd.pf_plep, g_rd.ple_pnorm_dev)
                let gsizes = fixed_array(g_rd.ple_wp_bytes, np * dim * 4l, np * all * 4l, g_rd.ple * 4l)
                let ggbits = fixed_array(0u, VHZ_X, VHZ_MRT, 0u)
                g_rd.pf_ple_gemm_set = g_rd.ple_wp_bf16 ? ple_proj_bf16_set(gbufs, gsizes, ggbits) : set_router_gemv_cls(gbufs, gsizes, ggbits)
            }
            //! the finish over [np x layers] slices of ple: side = (gathered + pnorm . rms(projected)) / sqrt(2)
            g_rd.pf_ple_ar_set = set_ple_finish_cls(fixed_array(g_rd.pf_plep, g_rd.ple_pnorm_dev, g_rd.pf_ple),
                fixed_array(np * all * 4l, g_rd.ple * 4l, np * all * 4l), fixed_array(VHZ_MRT, 0u, VHZ_COS))
        }
    }
    g_rd.pf_logits_host = make_host_buf(g_rd.regions * g_rd.vocab * 4l, false, [cached = true])
    g_rd.pf_cmd = alloc_cmd()
    if (g_env_vulkan.vk_overlap) {
        let cap = pf_chunk_cap()
        let nring = 3l + (g_rd.n_layers + cap - 1l) / cap
        g_rd.pf_cmd_ring |> reserve(nring)
        for (_i in range64(nring)) {
            g_rd.pf_cmd_ring |> push(alloc_cmd())
        }
    }
    g_rd.pf_cap_dev = make_call_buf(PF_CAP_WORDS * 4l)
    if (rd_emb_on()) {   //! the window's rows gather off their ids; no window row is the host's, so `xh` binds the one-row command's staging unread
        g_rd.pf_ids = make_call_buf(np * 4l)
        if (g_rd.emb_blk >= 0l) {
            verify(emb_cls_ensure(g_rd.emb_fmt), "vk prefill: the embed-gather class rail must engage")
            g_rd.pf_emb_set = emb_cls_set(g_rd.emb_fmt, arena_planes(g_rd.emb_fmt, g_rd.emb_blk), g_rd.pf_ids, np * 4l, 0u,
                g_rd.tok_xh_host.buf, dim * 4l, g_rd.pf_x, np * dim * 4l, VHZ_X)
        } else {
            verify(ensure_emb_gather_f32_cls(), "vk prefill: the f32 embed-gather class rail must engage")
            g_rd.pf_emb_set = emb_f32_set(g_rd.emb_f32, g_rd.emb_f32_bytes, g_rd.pf_ids, np * 4l, 0u, g_rd.tok_xh_host.buf, dim * 4l,
                g_rd.pf_x, np * dim * 4l, VHZ_X)
        }
    }
    if (g_rd.ple_tbl_blk >= 0l && g_rd.pf_ids != 0ul) {   //! the token table's gather into the side rows (the pre-step's finish reads them under the cos plane's bit)
        let all = g_rd.n_layers * g_rd.ple
        g_rd.pf_ple_tbl_set = emb_cls_set(g_rd.ple_tbl_fmt, arena_planes(g_rd.ple_tbl_fmt, g_rd.ple_tbl_blk), g_rd.pf_ids, np * 4l, 0u,
            g_rd.tok_xh_host.buf, dim * 4l, g_rd.pf_ple, np * all * 4l, VHZ_COS)
    }
    ensure_batch_state()   // hq_dev/hs_dev for the FFN act
    // f16-fed groups (pf_f16_feed) take f16 convert roles; kq consumers the Q8_K quant/act forms
    var any_f16_feed = false
    var anykq = kq_sb(g_rd.cls_fmt_marker)
    var ple_f32 = false   //! a per-layer-embedding kind on its f32 rows: the f32 tile and the f32 act
    for (L in g_rd.layers) {
        any_f16_feed = (any_f16_feed || pf_f16_feed(L.fq) || pf_f16_feed(L.fo) || pf_f16_feed(L.f1) || pf_f16_feed(L.f2)
            || (L.recurrent && (pf_f16_feed(L.fqkv) || pf_f16_feed(L.fz) || pf_f16_feed(L.fout)))
            || (L.ple && (pf_ple_plane_f16(L.bpg, L.fpg) || pf_ple_plane_f16(L.bpp, L.fpp))))
        anykq = (anykq || kq_sb(L.fq) || kq_sb(L.fo) || kq_sb(L.f1) || kq_sb(L.f2) || (L.recurrent && (kq_sb(L.fqkv) || kq_sb(L.fz) || kq_sb(L.fout)))
            || (L.ple && ((L.bpg >= 0l && kq_sb(L.fpg)) || (L.bpp >= 0l && kq_sb(L.fpp)))))
        ple_f32 = ple_f32 || (L.ple && (L.bpg < 0l || L.bpp < 0l))
    }
    if (any_f16_feed) {
        assert(np % 32l == 0l, "dasLLAMA vulkan tier: the s tile's unclamped column needs a 32-row-multiple window")   // the resident planes carry no read slack
    g_rd.pf_xf = make_device_buf(np * dim * 2l)
        g_rd.pf_af = make_device_buf(np * qd * 2l)
        g_rd.pf_hf = make_device_buf(np * hid * 2l)
    }
    if (any_f16_feed) {
        var skb = 262_144l * int64(g_gpu.sm_count)
        let fsk = cm2_splitk_env()
        if (fsk > 1l) {
            to_log(LOG_INFO, "dasLLAMA vulkan tier: DASLLAMA_CM2_SPLITK={fsk} forces the k chunks of every coopmat GEMM the split admits (the cm2 tiles and the KHR arm alike)\n")
            skb = max(skb, np * wide * fsk * 4l)
        }
        skb = min(skb, vk_max_storage_range())   //! the split planes bind as one SSBO range: capped at the device range here, where the size is negotiated
        if (skb > 0l) {
            g_rd.pf_splitk = make_device_buf(skb)
            g_rd.pf_splitk_bytes = skb
        }
    }
    let nsets = int(g_rd.n_layers * PF_ROLES + 2l + (g_rd.head_norm_base > 0l ? PF_ROLES : 0l))   // + prologue, final quantize of the last row, the NextN head's block
    g_rd.pf_sets |> resize(nsets)
    g_rd.pf_arq_sets |> resize(g_rd.n_layers * 2l)
    g_rd.pf_meta |> resize(nsets)
    for (mi in range(nsets)) {
        g_rd.pf_meta[mi] = make_host_buf(BATCH_META_BYTES, true)
    }
    let mirbytes = rd_mir_bytes()
    let normbytes = g_rd.norms_bytes
    let kv16 = rd_kv16()
    let kvq8 = rd_kvq8()
    let kvtq4 = rd_kvtq4()
    let kvq = rd_kv_block_codec()
    let pf_reads_f16 = rd_pf_reads_f16()
    let gated_stamp = rd_attn_gated()
    // the fa pick is per layer: a model's attention classes may differ in head size, and a class with no fa stamp takes the chunked pair
    let fa_dev = cm2_fa_on() && vk_fa_on() && pf_reads_f16
    let fakhr_dev = !fa_dev && khr_kq_tile_on() && vk_fa_on() && pf_reads_f16
    var fa_hs : table<int64>   //! the head sizes the fa tiles serve on this model
    var fakhr_hs : table<int64>   //! ... the KHR flash tile
    var chunked_hs : table<int64>   //! ... and the ones the chunked pair serves
    var any_cap = false   //! a flash layer under the attention softcap: the h256 cap stamps engage
    var any_khr_cap = false
    let sinks = g_rd.has_sinks   //! the sink stamps take every flash layer of a sink model (its heads are 64 wide, the stamps' one size)
    g_rd.pf_facm2 = false
    g_rd.pf_fakhr = false
    var chunked_why : table<string; int64>   //! why each layer took the chunked pair, by count - the load-time lines below
    for (L in g_rd.layers) {
        continue if (L.recurrent)
        L.pf_fa = fa_dev && fa_cm2_serves(L.hs, L.attn_cap, gated_stamp)
        L.pf_fakhr = fakhr_dev && fa_khr_serves(L.hs, L.attn_cap, gated_stamp)
        any_cap = any_cap || (L.pf_fa && L.attn_cap != 0.0)
        any_khr_cap = any_khr_cap || (L.pf_fakhr && L.attn_cap != 0.0)
        g_rd.pf_facm2 = g_rd.pf_facm2 || L.pf_fa
        g_rd.pf_fakhr = g_rd.pf_fakhr || L.pf_fakhr
        if (L.pf_fa) {
            fa_hs |> insert(L.hs)
        } elif (L.pf_fakhr) {
            fakhr_hs |> insert(L.hs)
        } else {
            chunked_hs |> insert(L.hs)
            let why = pf_chunked_why(fa_dev || fakhr_dev, L.hs, L.attn_cap, gated_stamp)
            chunked_why[why] = (chunked_why?[why] ?? 0l) + 1l
        }
    }
    let fa128s = pf_fa128s() && key_exists(chunked_hs, 128l)
    var wide_hs = false   //! a head past 256 takes the wide chunked kernel
    var narrow_hs = false
    for (hsv in keys(chunked_hs)) {
        if (hsv > 256l) {
            wide_hs = true
        } elif (!(fa128s && hsv == 128l)) {
            narrow_hs = true
        }
    }
    verify(ensure_cls_dn_rq() && ensure_cls_ar() && ensure_cls_ar_rq_b()
        && rope_kv_b_stamp_ensure(kv16)
        && (!fa128s || da_attn_b_stamp_ensure(true, false, pf_reads_f16))
        && (!narrow_hs || da_attn_b_stamp_ensure(false, false, pf_reads_f16))
        && (!wide_hs || da_attn_b_stamp_ensure(false, true, pf_reads_f16))
        && (!kvq8 || (ensure_kv_q8_store_b_cls() && ensure_kv_q8_shadow_cls()))
        && (!kvtq4 || (ensure_kv_tq4_store_b_cls() && ensure_kv_tq4_shadow_cls() && ensure_kv_tq4_unrot_cls()))
        && (!key_exists(fa_hs, 64l) || fa_stamp_pair_ensure(false, 64l, false, false, sinks, any_f16_feed))
        && (!key_exists(fa_hs, 96l) || fa_stamp_pair_ensure(false, 96l, false, false, false, any_f16_feed))
        && (!key_exists(fa_hs, 128l) || fa_stamp_pair_ensure(false, 128l, false, false, false, any_f16_feed))
        && (!key_exists(fa_hs, 256l) || fa_stamp_pair_ensure(false, 256l, gated_stamp, false, false, any_f16_feed))
        && (!any_cap || fa_stamp_pair_ensure(false, 256l, false, true, false, any_f16_feed))
        && (!key_exists(fa_hs, 512l) || fa_stamp_pair_ensure(false, 512l, false, false, false, any_f16_feed))
        && (!key_exists(fakhr_hs, 64l) || fa_stamp_pair_ensure(true, 64l, false, false, sinks, any_f16_feed))
        && (!key_exists(fakhr_hs, 128l) || fa_stamp_pair_ensure(true, 128l, false, false, false, any_f16_feed))
        && (!key_exists(fakhr_hs, 256l) || fa_stamp_pair_ensure(true, 256l, gated_stamp, false, false, any_f16_feed))
        && (!any_khr_cap || fa_stamp_pair_ensure(true, 256l, false, true, false, any_f16_feed))
        && (!key_exists(fakhr_hs, 512l) || fa_stamp_pair_ensure(true, 512l, false, false, false, any_f16_feed))
        && (!sinks || long_length(chunked_hs) == 0l)   //! the sink rides the flash tiles alone: a sink model whose layer fell to the chunked pair declined at the sink upload
        && ensure_q8_actrq_cls() && (!g_rd.qk_norm || ensure_qk_rms_cls())
        && (!any_f16_feed || (ensure_tower_clamp_cvt_cls() && ensure_actf16_cls() && ensure_cls_ar_f16_b()))
        && (!anykq || (ensure_cls_q8k_rq() && ensure_q8k_actrq_cls()))
        && (!ple_f32 || (ensure_dn_ba_cls() && ensure_ple_act_f32_cls())),
        "vk prefill: the class rails must engage on a live device")
    if (g_rd.pf_facm2) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: cm2 flash attention armed ({long_length(fa_hs)} head size(s), {kvq ? "the block-codec mirrors' f16 shadow" : "native f16 mirrors"}; {long_length(chunked_hs)} on the chunked pair)\n")
    }
    if (g_rd.pf_fakhr) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: KHR flash attention armed ({long_length(fakhr_hs)} head size(s), {kvq ? "the block-codec mirrors' f16 shadow" : "native f16 mirrors"}; {long_length(chunked_hs)} on the chunked pair)\n")
    }
    for (k, v in keys(chunked_why), values(chunked_why)) {   //! one line a reason, whether or not any layer armed a tile
        to_log(LOG_INFO, "dasLLAMA vulkan tier: {v} attention layer(s) on the chunked pair, off the flash tiles: {k}\n")
    }
    delete chunked_why
    delete fa_hs
    delete fakhr_hs
    delete chunked_hs
    var nkvm = 0l
    for (l in range64(g_rd.n_layers)) {
        if (pf_kv_merged(l, dim, kvd)) {
            nkvm++
        }
    }
    if (nkvm > 0l) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: merged k/v prefill GEMM armed ({nkvm} of {g_rd.n_layers} layers; DASLLAMA_VK_KV_MERGE=0 pins the split pair)\n")
    }
    if (g_rd.moe) {
        pf_moe_setup()
    }
    if (g_rd.hybrid) {
        ensure_dn_state()   // dn_ws_dev (the scan workspace) + the staging buffers the reset/handoff ride
        verify(ensure_dn_conv_cls() && ensure_dn_scan_cls() && (g_rd.dn_zsig ? ensure_dn_scan_p3_sig_cls() : ensure_dn_scan_p3_cls())
            && ensure_dn_tail_cls() && ensure_dn_cap_tail_cls() && (!g_rd.dn_ba_f32 || ensure_dn_ba_cls()),
            "vk prefill: the deltanet class rails must engage on a live device")
        let cd = g_rd.dn_cd
        let di = g_rd.dn_di
        g_rd.pf_dnqkv = make_device_buf(np * cd * 4l)
        g_rd.pf_dnconv = make_device_buf(np * cd * 4l)
        g_rd.pf_dnz = make_device_buf(np * di * 4l)
        g_rd.pf_dnbg = make_device_buf(np * g_rd.dn_nvh * 4l)
        g_rd.pf_dnrq_set = set_rq_cls(fixed_array(g_gpu.dn_ws_dev, g_gpu.hq_dev, g_gpu.hs_dev),
            fixed_array(DN_WS_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES),
            fixed_array(VHZ_DNWS, VHZ_HQ, VHZ_HQ))
        if (any_f16_feed) {
            g_rd.pf_dnof = make_device_buf(np * di * 2l)
            g_rd.pf_dnof_set = set_tower_clamp_cvt_cls(fixed_array(g_gpu.dn_ws_dev, g_rd.pf_dnof),
                fixed_array(DN_WS_BYTES, np * di * 2l), fixed_array(VHZ_DNWS, VHZ_HQ))
        }
    }
    let stagebytes = np * kvd * 4l
    let shbytes = kvq ? rd_shadow_bytes() : 0l
    g_rd.pf_kv_store_sets |> clear()
    g_rd.pf_kv_shadow_sets |> clear()
    for (si in range64(rd_mir_slabs())) {   //! a block codec's window store writes, and its shadow pass reads, the layer's slab: one set a slab
        let (kmir, vmir) = rd_slab_at(si)
        if (kvtq4) {
            let signbytes = KV_TQ4_SIGNS * 4l
            g_rd.pf_kv_store_sets |> push(set_kv_tq4_store_b_cls(fixed_array(g_rd.pf_q, g_rd.pf_kst, g_rd.pf_vst, kmir, vmir, g_rd.tq4_signs_dev),
                fixed_array(np * qrow * 4l, stagebytes, stagebytes, mirbytes, mirbytes, signbytes), fixed_array(VHZ_Q, VHZ_VST, VHZ_VST, VHZ_MIR, VHZ_MIR, 0u)))
            g_rd.pf_kv_shadow_sets |> push(set_kv_tq4_shadow_cls(fixed_array(kmir, vmir, g_rd.ksh_dev, g_rd.vsh_dev),
                fixed_array(mirbytes, mirbytes, shbytes, shbytes), fixed_array(VHZ_MIR, VHZ_MIR, VHZ_SK, VHZ_SK)))
        }
        if (kvq8) {
            g_rd.pf_kv_store_sets |> push(set_kv_q8_store_b_cls(fixed_array(g_rd.pf_kst, g_rd.pf_vst, kmir, vmir),
                fixed_array(stagebytes, stagebytes, mirbytes, mirbytes), fixed_array(VHZ_VST, VHZ_VST, VHZ_MIR, VHZ_MIR)))
            g_rd.pf_kv_shadow_sets |> push(set_kv_q8_shadow_cls(fixed_array(kmir, vmir, g_rd.ksh_dev, g_rd.vsh_dev),
                fixed_array(mirbytes, mirbytes, shbytes, shbytes), fixed_array(VHZ_MIR, VHZ_MIR, VHZ_SK, VHZ_SK)))
        }
    }
    if (kvtq4) {
        let signbytes = KV_TQ4_SIGNS * 4l
        g_rd.pf_unrot_set = set_kv_tq4_unrot_cls(fixed_array(g_rd.pf_attn, g_rd.pf_q, g_rd.tq4_signs_dev),
            fixed_array(np * qd * 4l, np * qrow * 4l, signbytes), fixed_array(VHZ_ATT, VHZ_Q, 0u))
    }
    let readbytes = kvq ? shbytes : mirbytes
    let readbit = kvq ? VHZ_SK : VHZ_MIR
    for (l in range64(g_rd.n_layers)) {
        var L & = unsafe(g_rd.layers[l])
        let b = int(l * PF_ROLES)
        let slab = L.recurrent ? (k = 0ul, v = 0ul) : rd_slab(L)   //! the f16 mirror's rows are the layer's slab; a block codec's shadow is one plane for every layer
        let kread = kvq ? g_rd.ksh_dev : slab.k
        let vread = kvq ? g_rd.vsh_dev : slab.v
        if (L.recurrent) {
            pf_dn_layer_sets(l, dim, g_rd.dn_cd, g_rd.dn_di)
        }
        if (L.moe) {
            pf_moe_layer_sets(l)
        }
        let qkv6 = !L.recurrent && pf_qkv6(l)
        let wo6 = pf_f16_feed(L.fo)
        let fa_lands_f16 = pf_fa_lands_f16(wo6)
        let gu6 = pf_gu6(l)
        let dn6 = pf_dn6(l)
        // 0 quant_xb: pf_xb -> pf_xq/pf_xs (f16-fed: f16 convert -> pf_xf)
        g_rd.pf_sets[b + 0] = pf_feed_set(qkv6, g_rd.pf_xb, VHZ_XB, g_rd.pf_xf, g_rd.pf_xq, g_rd.pf_xs, dim, VHZ_XQ)
        // (GEMM roles 1/2/3/7/10/11/13 build lazily per router variant at record time)
        if (!L.recurrent) {
            // 4 rope+store (class: q, kv, Kmir, Vmir, cos, bias)
            g_rd.pf_sets[b + 4] = pf_rope_kv_set(g_rd.pf_cos, L)
        // 5 attn (class: q, Kmir, Vmir, out) — the cm2 fa tile / h128 coopmat twin when they serve this layer's head size
        let fa128l = fa128s && L.hs == 128l
        if ((L.pf_fa || L.pf_fakhr) && g_rd.has_sinks) {
            let fbufs = fixed_array(g_rd.pf_q, kread, vread, fa_lands_f16 ? g_rd.pf_af : g_rd.pf_attn, g_rd.sinks_dev)
            let fsizes = fixed_array(np * qrow * 4l, readbytes, readbytes, fa_lands_f16 ? np * qd * 2l : np * qd * 4l, g_rd.sinks_bytes)
            let fgbits = fixed_array(VHZ_Q, readbit, readbit, fa_lands_f16 ? VHZ_AQ : VHZ_ATT, 0u)
            g_rd.pf_sets[b + 5] = fa_stamp_sink_set(L.pf_fakhr, fbufs, fsizes, fgbits)
        } elif (L.pf_fa || L.pf_fakhr) {
            let fbufs = fixed_array(g_rd.pf_q, kread, vread, fa_lands_f16 ? g_rd.pf_af : g_rd.pf_attn)
            let fsizes = fixed_array(np * qrow * 4l, readbytes, readbytes, fa_lands_f16 ? np * qd * 2l : np * qd * 4l)
            let fgbits = fixed_array(VHZ_Q, readbit, readbit, fa_lands_f16 ? VHZ_AQ : VHZ_ATT)
            g_rd.pf_sets[b + 5] = fa_stamp_set(L.pf_fakhr, fbufs, fsizes, fgbits)
        } else {
            let abufs5 = fixed_array(g_rd.pf_q, kread, vread, g_rd.pf_attn)
            let asizes5 = fixed_array(np * qrow * 4l, readbytes, readbytes, np * qd * 4l)
            let agbits5 = fixed_array(VHZ_Q, readbit, readbit, VHZ_ATT)
            g_rd.pf_sets[b + 5] = da_attn_b_stamp_set(fa128l, L.hs > 256l, pf_reads_f16, abufs5, asizes5, agbits5)
        }
        // 6 quant_at: pf_attn -> pf_aq/pf_as (f16-fed: f16 convert -> pf_af)
        g_rd.pf_sets[b + 6] = pf_feed_set(wo6, g_rd.pf_attn, VHZ_ATT, g_rd.pf_af, g_rd.pf_aq, g_rd.pf_as, qd, VHZ_AQ)
        }
        // 8 addr_ffn (class: x, add partner, norms, xb out)
        g_rd.pf_sets[b + 8] = set_cls_ar(fixed_array(g_rd.pf_x, g_rd.pf_xb2, g_rd.norms_dev, g_rd.pf_xb),
            fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim * 4l),
            fixed_array(VHZ_X, VHZ_XB2, 0u, VHZ_XB))
        // 9 quant_xb2: pf_xb -> pf_xq (f16-fed: f16 convert -> pf_xf)
        g_rd.pf_sets[b + 9] = pf_feed_set(gu6, g_rd.pf_xb, VHZ_XB, g_rd.pf_xf, g_rd.pf_xq, g_rd.pf_xs, dim, VHZ_XQ)
        // 12 actrq (class: gate, up, quants out, scales out; f16-fed: actf16 -> pf_hf)
        g_rd.pf_sets[b + 12] = (dn6
            ? set_actf16_cls(fixed_array(g_rd.pf_gate, g_rd.pf_up, g_rd.pf_hf),
                fixed_array(np * hid * 4l, np * hid * 4l, np * hid * 2l),
                fixed_array(VHZ_GATE, VHZ_UP, VHZ_HQ))
            : set_actrq_cls(fixed_array(g_rd.pf_gate, g_rd.pf_up, g_gpu.hq_dev, g_gpu.hs_dev),
                fixed_array(np * hid * 4l, np * hid * 4l, BATCH_HQ_BYTES, BATCH_HS_BYTES),
                fixed_array(VHZ_GATE, VHZ_UP, VHZ_HQ, VHZ_HQ)))
        // 14 addr_next (class): x += ffnout, xb = rms_att[l+1]|final
        g_rd.pf_sets[b + 14] = set_cls_ar(fixed_array(g_rd.pf_x, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xb),
            fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim * 4l),
            fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB))
        g_rd.pf_arq_sets[int(l * 2l)] = (gu6
            ? set_cls_ar_f16_b(fixed_array(g_rd.pf_x, g_rd.pf_xb2, g_rd.norms_dev, g_rd.pf_xf),
                fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim * 2l),
                fixed_array(VHZ_X, VHZ_XB2, 0u, VHZ_XQ))
            : set_cls_ar_rq_b(
                fixed_array(g_rd.pf_x, g_rd.pf_xb2, g_rd.norms_dev, g_rd.pf_xq, g_rd.pf_xs),
                fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim, np * (dim / 32l) * 4l),
                fixed_array(VHZ_X, VHZ_XB2, 0u, VHZ_XQ, VHZ_XQ)))
        let next_qkv6 = l + 1l < g_rd.n_layers && pf_qkv6(l + 1l)
        g_rd.pf_arq_sets[int(l * 2l + 1l)] = (next_qkv6
            ? set_cls_ar_f16_b(fixed_array(g_rd.pf_x, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xf),
                fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim * 2l),
                fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XQ))
            : set_cls_ar_rq_b(
                fixed_array(g_rd.pf_x, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xq, g_rd.pf_xs),
                fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim, np * (dim / 32l) * 4l),
                fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XQ, VHZ_XQ)))
        // 15 qk rmsnorm (qk_norm models; class: q, k rows, norm rows)
        if (g_rd.qk_norm && !L.recurrent) {
            g_rd.pf_sets[b + 15] = pf_qk_rms_set()
        }
        // 16 ple_x: the stream rows as they are -> pf_xf (the f16 feed) or pf_xq/pf_xs; 18 ple_act: gelu(gate rows) x the side rows -> hq/hs (17 / 19 are GEMM roles: sets on the encode)
        if (L.ple) {
            let ple = g_rd.ple
            let side_bytes = np * g_rd.n_layers * ple * 4l
            if (L.bpg < 0l && !pf_ple_plane_f16(L.bpg, L.fpg)) {   //! 17: the f32 gate rows over the stream rows (`pf_ple_f32_args`)
                g_rd.pf_sets[b + 17] = set_dn_ba_cls(fixed_array(g_rd.ple_gf32_dev, g_rd.pf_x, g_rd.pf_gate),
                    fixed_array(g_rd.ple_gf32_bytes, np * dim * 4l, np * ple * 4l), fixed_array(0u, VHZ_X, VHZ_GATE))
            } else {
                g_rd.pf_sets[b + 16] = pf_feed_set(pf_ple_plane_f16(L.bpg, L.fpg), g_rd.pf_x, VHZ_X, g_rd.pf_xf, g_rd.pf_xq, g_rd.pf_xs, dim, VHZ_XQ)
            }
            if (L.bpp < 0l && !pf_ple_plane_f16(L.bpp, L.fpp)) {   //! 18: the act's f32 rows into the up plane; 19: the f32 projection over them
                g_rd.pf_sets[b + 18] = set_ple_act_f32_cls(fixed_array(g_rd.pf_gate, g_rd.pf_ple, g_rd.pf_up),
                    fixed_array(np * ple * 4l, side_bytes, np * ple * 4l), fixed_array(VHZ_GATE, VHZ_COS, VHZ_UP))
                g_rd.pf_sets[b + 19] = set_dn_ba_cls(fixed_array(g_rd.ple_pf32_dev, g_rd.pf_up, g_rd.pf_ffnout),
                    fixed_array(g_rd.ple_pf32_bytes, np * ple * 4l, np * dim * 4l), fixed_array(0u, VHZ_UP, VHZ_FFO))
            } elif (pf_ple_plane_f16(L.bpp, L.fpp)) {   //! 18: the act as f16 rows into the hidden plane, the projection's f16 feed
                g_rd.pf_sets[b + 18] = set_actf16_cls(fixed_array(g_rd.pf_gate, g_rd.pf_ple, g_rd.pf_hf),
                    fixed_array(np * ple * 4l, side_bytes, np * hid * 2l), fixed_array(VHZ_GATE, VHZ_COS, VHZ_HQ))
            } else {
                g_rd.pf_sets[b + 18] = set_actrq_cls(fixed_array(g_rd.pf_gate, g_rd.pf_ple, g_gpu.hq_dev, g_gpu.hs_dev),
                    fixed_array(np * ple * 4l, side_bytes, BATCH_HQ_BYTES, BATCH_HS_BYTES),
                    fixed_array(VHZ_GATE, VHZ_COS, VHZ_HQ, VHZ_HQ))
            }
        }
    }
    let bp = pf_prologue_block()
    // prologue: xb = rms_att[0](x), add off (the aa slot binds x itself — add_on 0 never reads it)
    g_rd.pf_sets[bp + 0] = set_cls_ar(fixed_array(g_rd.pf_x, g_rd.pf_x, g_rd.norms_dev, g_rd.pf_xb),
        fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim * 4l),
        fixed_array(VHZ_X, VHZ_X, 0u, VHZ_XB))
    // final quantize of the LAST row (into the decode driver's xq_dev/xs_dev) for the cls GEMV
    g_rd.pf_sets[bp + 1] = set_rq_cls(fixed_array(g_rd.pf_xb, g_rd.xq_dev, g_rd.xs_dev),
        fixed_array(np * dim * 4l, dim, (dim / 32l) * 4l),
        fixed_array(VHZ_XB, VHZ_XQ, VHZ_XQ))
    g_rd.pf_ready = true
}

def private pf_prologue_block : int => int(g_rd.n_layers * PF_ROLES)

//! past the prologue set and the last row's final quantize
def private pf_head_block : int => pf_prologue_block() + 2

def private pf_head_qkv6 : bool => qkv6_of(g_rd.head)

[cold_path]
def private pf_head_decline(why : string) : bool {
    g_rd.pf_head_declined = true
    to_log(LOG_WARNING, "dasLLAMA vulkan resident: the NextN head's prompt warm has no form on this device - {why}; the head's prompt rows stay as the host holds them\n")
    return false
}

//! the chain's head planes and sets, built once per residency behind the caller's `pf_head_ready` / `pf_head_declined` latches
[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm"), arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs")]
def private pf_head_setup : bool {
    let np = g_rd.pf_np
    let dim = g_rd.dim
    let hb = pf_head_block()
    if (long_length(g_rd.pf_sets) < int64(hb) + PF_ROLES) return pf_head_decline("the head's set block was not sized (the driver prepared without the head)")
    let q6 = pf_head_qkv6()
    let e6 = pf_f16_feed(g_rd.head_eh_fmt)
    if (q6 && g_rd.pf_xf == 0ul) return pf_head_decline("the head's q/k/v planes take the f16 feed and the chain carries no f16 feed plane")
    if (!e6 && kq_sb(g_rd.head_eh_fmt) && !ensure_cls_q8k_rq()) return pf_head_decline("the eh_proj plane's Q8_K feed has no requant class")
    if (!ensure_cat2_cls()) return pf_head_decline("the head rows' cat class did not build")
    let catb = np * 2l * dim * 4l
    if (!vk_range_ok(catb)) return pf_head_decline("the head rows' cat plane ({catb} bytes over the {np}-row window, the widest of the head's planes) passes the device's storage range {vk_max_storage_range()}")
    g_rd.head_pf_wemb = make_device_buf(np * dim * 4l)
    g_rd.head_pf_hsrc = make_device_buf(np * dim * 4l)
    g_rd.head_pf_hcos = make_call_buf(np * g_rd.cos_elems * 4l)
    g_rd.pf_sets[hb + 8] = pf_head_cat_set(1l)
    g_rd.head_pf_s_cat2 = set_cat2_cls(fixed_array(g_rd.head_pf_cat, g_rd.pf_xb2, g_rd.pf_ffnout), fixed_array(catb, np * dim * 4l, np * dim * 4l),
        fixed_array(VHZ_ATT, VHZ_XB2, VHZ_FFO))
    let normbytes = g_rd.norms_bytes
    let xb = np * dim * 4l
    g_rd.pf_sets[hb + 6] = set_cls_ar(fixed_array(g_rd.head_pf_wemb, g_rd.head_pf_wemb, g_rd.norms_dev, g_rd.pf_xb2),
        fixed_array(xb, xb, normbytes, xb), fixed_array(VHZ_UP, VHZ_UP, 0u, VHZ_XB2))
    g_rd.pf_sets[hb + 7] = set_cls_ar(fixed_array(g_rd.head_pf_hsrc, g_rd.head_pf_hsrc, g_rd.norms_dev, g_rd.pf_ffnout),
        fixed_array(xb, xb, normbytes, xb), fixed_array(VHZ_GATE, VHZ_GATE, 0u, VHZ_FFO))
    g_rd.pf_sets[hb + 0] = pf_feed_set(q6, g_rd.pf_xb, VHZ_XB, g_rd.pf_xf, g_rd.pf_xq, g_rd.pf_xs, dim, VHZ_XQ)
    g_rd.pf_sets[hb + 4] = pf_rope_kv_set(g_rd.head_pf_hcos, g_rd.head)
    if (g_rd.qk_norm) {
        g_rd.pf_sets[hb + 15] = pf_qk_rms_set()
    }
    g_rd.pf_head_ready = true
    return true
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-draft-head")]
def private pf_hc_head_setup : bool {
    let np = g_rd.pf_np
    let dim = g_rd.dim
    let hc = g_rd.hc_n
    let hcd = hc * dim
    let hb = pf_head_block()
    let catb = np * hc * 2l * dim * 4l
    if (!vk_range_ok(catb)) return pf_head_decline("the hc head rows' cat plane ({catb} bytes: {hc} streams a row over the {np}-row window) passes the device's storage range {vk_max_storage_range()}")
    g_rd.pf_sets[hb + 8] = pf_head_cat_set(hc)
    g_rd.head_pf_s_cat2_hc = set_cat2_cls(fixed_array(g_rd.head_pf_cat, g_rd.pf_xb2, g_rd.pf_hcxn), fixed_array(catb, np * dim * 4l, np * hcd * 4l),
        fixed_array(VHZ_ATT, VHZ_XB2, VHZ_XB2))
    g_rd.head_pf_hsrc_w = make_device_buf(np * hcd * 4l)
    g_rd.head_pf_wsm = make_device_buf(np * hcd * 4l)
    g_rd.head_pf_s_hnorm = set_hc_norm_cls(fixed_array(g_rd.head_pf_hsrc_w, g_rd.pf_hcxn, g_rd.hcn_dev),
        fixed_array(np * hcd * 4l, np * hcd * 4l, g_rd.hcn_bytes), fixed_array(VHZ_GATE, VHZ_XB2, 0u))
    g_rd.head_pf_s_enorm = set_cls_ar(fixed_array(g_rd.pf_x, g_rd.pf_x, g_rd.norms_dev, g_rd.pf_xb2),
        fixed_array(np * dim * 4l, np * dim * 4l, g_rd.norms_bytes, np * dim * 4l), fixed_array(VHZ_X, VHZ_X, 0u, VHZ_XB2))
    g_rd.pf_hc_head_ready = true
    return true
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-draft-head")]
def private pf_hc_head_warm(raw : VkCommandBuffer; var h : VkHaz; w0, wlen, pos0 : int64) {
    let dim = g_rd.dim
    let np = g_rd.pf_np
    let hc = g_rd.hc_n
    let hcd = hc * dim
    let rowb = dim * 4l
    let wrowb = hcd * 4l
    let prev_row = w0 > 0l ? 1l : 0l
    let nh = wlen - 1l + prev_row
    let src_row0 = uint(1l - prev_row)
    var L & = unsafe(g_rd.head)
    let hb = pf_head_block()
    let hbn = uint(g_rd.head_norm_base)
    if (nh > 0l) {
        vhz_dep(raw, h, VHZ_X, VHZ_GATE, true)
        cmd_copy_range(raw, g_rd.pf_hcres, 0l, g_rd.head_pf_hsrc_w, wrowb, (wlen - 1l) * wrowb)
        var pcn = GkHcNormArgs(dim = uint(dim), hc = uint(hc), woff = uint(g_rd.hc_hnorm_row * hcd), row0 = src_row0, eps = g_rd.eps)
        enc_hc_norm_cls(raw, h, g_rd.head_pf_s_hnorm, pcn, nh * hc)
        var pce = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = hbn + uint(RDEC_HEAD_ENORM * dim), eps = g_rd.eps, ascale = 1.0, row0 = src_row0)
        enc_cls_ar(raw, h, g_rd.head_pf_s_enorm, pce, nh)
        var pcc = GkCat2Args(d = uint(dim), rows = uint(nh * hc), rep = uint(hc), smaj = 1u, np = uint(nh), a0 = src_row0,
            b0 = src_row0 * uint(hc))
        enc_cat2_cls(raw, h, g_rd.head_pf_s_cat2_hc, pcc, (nh * hc * dim + 255l) / 256l)
        pf_head_eh_enc(raw, h, nh, hc, g_rd.head_pf_wsm, np * hcd * 4l)
    }
    //! the next window's prev_row row lands in slot 0 only after the norm above read the previous one there
    vhz_dep(raw, h, VHZ_X | VHZ_GATE, VHZ_GATE, true)
    cmd_copy_range(raw, g_rd.pf_hcres, (wlen - 1l) * wrowb, g_rd.head_pf_hsrc_w, 0l, wrowb)
    return if (nh <= 0l)
    vhz_dep(raw, h, VHZ_X, VHZ_X, true)
    for (c in range64(hc)) {
        cmd_copy_rows(raw, g_rd.head_pf_wsm, c * nh * rowb, rowb, g_rd.pf_hcres, c * rowb, wrowb, rowb, nh)
    }
    pf_hc_mix(raw, h, 2l * g_rd.n_layers, nh, false)
    pf_head_kv_store(raw, h, L, nh, pos0 + w0 - prev_row)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm"), arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs")]
def private pf_head_kv_store(raw : VkCommandBuffer; var h : VkHaz; L : RLayer; nh, pos0 : int64) {
    let dim = g_rd.dim
    let np = g_rd.pf_np
    let hb = pf_head_block()
    let kvd = L.kvd
    let q6 = pf_head_qkv6()
    pf_feed_enc(raw, h, g_rd.pf_sets[hb + 0], q6, kq_sb(L.fq), 0l, nh * dim)
    g_rd.pf_head_rows += nh
    let xf = pf_feed_planes(q6, g_rd.pf_xf, g_rd.pf_xq, g_rd.pf_xs, dim)
    let pk = arena_planes(L.fk, L.bk)
    let tkv = cm2_tiles(kvd, nh)
    pf_gemm_enc(raw, h, hb + 2, L.fk, pk.wq, pk.ws, xf.q, xf.s, g_rd.pf_kv, pk.wqb, pk.wsb, xf.qn, xf.sn, np * 2l * kvd * 4l, VHZ_XQ, VHZ_KVK,
        dim, kvd, L.bk, nh, q6, tkv, 0l, false)
    if (!L.vfk) {
        let pv = arena_planes(L.fv, L.bv)
        pf_gemm_enc(raw, h, hb + 3, L.fv, pv.wq, pv.ws, xf.q, xf.s, g_rd.pf_v, pv.wqb, pv.wsb, xf.qn, xf.sn, np * kvd * 4l, VHZ_XQ, VHZ_VST,
            dim, kvd, L.bv, nh, q6, tkv, 0l, false)
        vhz_dep(raw, h, VHZ_VST, VHZ_KVV, true)
        cmd_copy_range(raw, g_rd.pf_v, 0l, g_rd.pf_kv, nh * kvd * 4l, nh * kvd * 4l)
    }
    let voff = uint(nh * kvd)
    let hs = L.hs
    let qd = int(L.qd)
    let qhs = int(g_rd.q_gated ? 2l * hs : hs)
    if (g_rd.qk_norm) {
        let nkvh = kvd / hs
        let vgroups = L.vnorm && !L.vfk ? nkvh : 0l
        let hsmax = g_rd.head_size   // nolint:LINT021 — the q/k norm rows sit at the widest class's stride; the row offset multiplies run 64-bit before the push-field narrow
        let qkw = rd_norm_final() + dim   // nolint:LINT021 — the head's q/k slot sits at index n_layers; the row offset multiplies run 64-bit before the push-field narrow
        //! the warm projects no q row, so the norm takes the k (and v) groups alone: no q group is dispatched
        var pcn = GkQkRmsArgs(hs = uint(hs), nh = 0u, nkvh = uint(nkvh), qstride = uint(g_rd.q_gated ? 2 * qd : qd), kstride = uint(kvd), kbase = 0u,
            qwoff = uint(qkw + g_rd.n_layers * 2l * hsmax), kwoff = uint(qkw + (g_rd.n_layers * 2l + 1l) * hsmax),
            eps = g_rd.eps, qhs = uint(qhs), vfk = L.vfk ? 1u : 0u, vnorm = L.vnorm ? 1u : 0u, vbase = voff)
        enc_qk_rms_cls(raw, h, g_rd.pf_sets[hb + 15], pcn, nh * (nkvh + vgroups))
    }
    let pairs = qd / 2 + int(kvd / 2l)
    let kvq = rd_kv_block_codec()
    let mirb = uint(rd_mir_base(L))
    var pcr = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(L.rot / 2l),
        neox = g_rd.neox ? 1u : 0u, kpair0 = uint(qd / 2), ppp = uint(pairs),
        layerbase = kvq ? 0u : mirb, npos = uint(nh), pos0 = kvq ? 0u : uint(pos0),
        voff = voff, kstride = uint(kvd), qhs = uint(qhs),
        boff = uint(L.boff), hasb = g_rd.has_bias ? 1u : 0u,
        cstride = uint(g_rd.cos_elems), cbase = uint(L.cbase))
    let ropewg = (nh * int64(pairs) + int64(WG_X) - 1l) / int64(WG_X)
    rope_kv_b_stamp_enc(rd_kv16(), raw, h, g_rd.pf_sets[hb + 4], pcr, ropewg)
    if (kvq) {   // a block codec's store pass; the rope stamp stored the f16 rows itself, and an f16 mirror builds no store set. The warm attends nothing: no q head to rotate under tq4
        rd_enc_kv_store(raw, h, g_rd.pf_kv_store_sets[L.mir_slab], true, kvd, hs, nh, 0l, kvd / hs, mirb, uint(pos0), uint(g_rd.n_heads * int64(qhs)), uint(qhs), L.ring)
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm")]
def private pf_head_warm(raw : VkCommandBuffer; var h : VkHaz; w0, wlen, pos0 : int64) {
    let dim = g_rd.dim
    let np = g_rd.pf_np
    let rowb = dim * 4l
    let carry = w0 > 0l ? 1l : 0l
    let nh = wlen - 1l + carry
    return if (nh <= 0l)
    let src_row0 = uint(1l - carry)   // head row j reads the window's row j + 1 - carry of the shifted planes
    var L & = unsafe(g_rd.head)
    let hb = pf_head_block()
    let hbn = uint(g_rd.head_norm_base)
    vhz_dep(raw, h, VHZ_XB, VHZ_GATE, true)
    cmd_copy_range(raw, g_rd.pf_xb, 0l, g_rd.head_pf_hsrc, rowb, (wlen - 1l) * rowb)
    var pch = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = hbn + uint(RDEC_HEAD_HNORM * dim), eps = g_rd.eps, ascale = 1.0, row0 = src_row0)
    enc_cls_ar(raw, h, g_rd.pf_sets[hb + 7], pch, nh)
    vhz_dep(raw, h, VHZ_XB, VHZ_GATE, true)
    cmd_copy_range(raw, g_rd.pf_xb, (wlen - 1l) * rowb, g_rd.head_pf_hsrc, 0l, rowb)
    var pce = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = hbn + uint(RDEC_HEAD_ENORM * dim), eps = g_rd.eps, ascale = 1.0, row0 = src_row0)
    enc_cls_ar(raw, h, g_rd.pf_sets[hb + 6], pce, nh)
    var pcc = GkCat2Args(d = uint(dim), rows = uint(nh), a0 = src_row0, b0 = src_row0)
    enc_cat2_cls(raw, h, g_rd.head_pf_s_cat2, pcc, (nh * dim + 255l) / 256l)
    pf_head_eh_enc(raw, h, nh, 1l, g_rd.pf_x, np * dim * 4l)
    var pca = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = hbn + uint(RDEC_HEAD_ATT * dim), eps = g_rd.eps, ascale = 1.0)
    enc_cls_ar(raw, h, g_rd.pf_sets[pf_prologue_block()], pca, nh)
    pf_head_kv_store(raw, h, L, nh, pos0 + w0 - carry)
}

def private pf_head_eh_enc(raw : VkCommandBuffer; var h : VkHaz; nh, streams : int64; y : uint64; ybytes : int64) {
    let dim = g_rd.dim
    let hb = pf_head_block()
    let ncat = nh * streams
    let e6 = pf_f16_feed(g_rd.head_eh_fmt)
    pf_feed_enc(raw, h, g_rd.pf_sets[hb + 8], e6, kq_sb(g_rd.head_eh_fmt), 0l, ncat * 2l * dim)
    let pe = arena_planes(g_rd.head_eh_fmt, g_rd.head_eh_blk)
    let cf = pf_feed_planes(e6, g_rd.head_pf_catf, g_rd.head_pf_catq, g_rd.head_pf_cats, streams * 2l * dim)
    pf_gemm_enc(raw, h, hb + 0, g_rd.head_eh_fmt, pe.wq, pe.ws, cf.q, cf.s, y,
        pe.wqb, pe.wsb, cf.qn, cf.sn, ybytes, VHZ_AQ, VHZ_X,
        2l * dim, dim, g_rd.head_eh_blk, ncat, e6, 0l, 0l, false)
}

def private pf_head_cat_set(streams : int64) : VkDescriptorSet {
    let width = streams * 2l * g_rd.dim
    let e6 = pf_f16_feed(g_rd.head_eh_fmt)
    let np = g_rd.pf_np
    g_rd.head_pf_cat = make_device_buf(np * width * 4l)
    if (e6) {
        g_rd.head_pf_catf = make_device_buf(np * width * 2l)
    } else {
        g_rd.head_pf_catq = make_device_buf(np * width)
        g_rd.head_pf_cats = make_device_buf(np * (width / Q8_BLOCK_ELEMS) * 4l)
    }
    return pf_feed_set(e6, g_rd.head_pf_cat, VHZ_ATT, g_rd.head_pf_catf, g_rd.head_pf_catq, g_rd.head_pf_cats, width, VHZ_AQ)
}

def private pf_rope_kv_set(cos : uint64; L : RLayer) : VkDescriptorSet {
    let np = g_rd.pf_np
    let kvd = g_rd.kv_dim
    let qrow = g_rd.q_gated ? 2l * g_rd.qd : g_rd.qd
    let kvq = rd_kv_block_codec()
    let storebytes = kvq ? np * kvd * 4l : rd_mir_bytes()
    let storebit = kvq ? VHZ_VST : VHZ_MIR
    let slab = rd_slab(L)
    let rbufs = fixed_array(g_rd.pf_q, g_rd.pf_kv, kvq ? g_rd.pf_kst : slab.k, kvq ? g_rd.pf_vst : slab.v, cos, g_rd.has_bias ? g_rd.bias_dev : g_rd.norms_dev)
    let rsizes = fixed_array(np * qrow * 4l, np * 2l * kvd * 4l, storebytes, storebytes, np * g_rd.cos_elems * 4l, g_rd.has_bias ? g_rd.bias_bytes : g_rd.norms_bytes)
    let rgbits = fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, storebit, storebit, VHZ_COS, 0u)
    return rope_kv_b_stamp_set(rd_kv16(), rbufs, rsizes, rgbits)
}

var private g_pf_qkn_fuse = true

//! The window's q/k norm, rope and K/V store as the token command's one fused pass (on), or the norm pass and the rope-and-store
//! pass (off: the A/B lever's split arm)
def set_vulkan_prefill_qkn_fuse(on : bool) {
    g_pf_qkn_fuse = on
    let form = on ? "one fused pass" : "the norm pass, then the rope-and-store pass"
    let knob = "set_vulkan_prefill_qkn_fuse({on})"
    to_log(LOG_INFO, "dasLLAMA vulkan tier: {knob} - the window's q/k norm, rope, store: {form}\n")
}

def vulkan_prefill_qkn_fuse : bool => g_pf_qkn_fuse

[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
def private pf_qkn_fused : bool
    => g_pf_qkn_fuse && g_env_vulkan.vk_fuse && g_rd.qk_norm && !rd_kv_block_codec()

//! the fused pass over every window part: a part's rows at their positions in their region's slab, `voff` the v rows' base in the
//! projection plane, the k and v rows `kvrow` apart
[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
def private pf_qkn_rope_enc(raw : VkCommandBuffer; var h : VkHaz; l : int64; voff : uint; kvrow, qrow : int64; qhs : int) {
    var L & = unsafe(g_rd.layers[l])
    if (L.pf_s_qkn_rope == null) {
        L.pf_s_qkn_rope = pf_qkn_rope_set(L)
    }
    let dim = g_rd.dim   // nolint:LINT021 — the norm row offsets multiply 64-bit before the push-field narrow
    let hs = L.hs
    let nkvh = L.kv_shared ? 0l : L.kvd / hs   //! a shared-KV layer's q heads alone: its donor's rows already sit in the mirror
    let hsmax = g_rd.head_size   // nolint:LINT021 — the q/k norm rows sit at the widest class's stride; the row offset multiplies run 64-bit before the push-field narrow
    let nlfin = rd_norm_final()   // nolint:LINT021 — the norm row offsets multiply 64-bit before the push-field narrow
    for (sg in g_pf_wsegs) {
        var pcf = GkQknRopeArgs(qd = uint(L.qd), kvd = uint(L.kvd), hs = uint(hs), half = uint(L.rot / 2l),
            neox = g_rd.neox ? 1u : 0u, nh = uint(g_rd.n_heads), layerbase = uint(pf_seg_mir_base(L, sg)),
            qwoff = uint(nlfin + dim + l * 2l * hsmax), kwoff = uint(nlfin + dim + (l * 2l + 1l) * hsmax),
            eps = g_rd.eps, qstride = uint(qhs), boff = uint(L.boff), hasb = g_rd.has_bias ? 1u : 0u, cbase = uint(L.cbase),
            vfk = L.vfk ? 1u : 0u, vnorm = L.vnorm ? 1u : 0u, rowwg = uint(g_rd.n_heads + nkvh), kvrow = uint(kvrow), qrow = uint(qrow),
            cosrow = uint(g_rd.cos_elems), win = 1u, pos0 = uint(sg.pos0), row0 = uint(sg.row0), kbase = 0u, vbase = voff, ring = uint(L.ring))
        if (rd_kv16()) {
            enc_qkn_rope_f16_cls(raw, h, L.pf_s_qkn_rope, pcf, (g_rd.n_heads + nkvh) * sg.npos)
        } else {
            enc_qkn_rope_cls(raw, h, L.pf_s_qkn_rope, pcf, (g_rd.n_heads + nkvh) * sg.npos)
        }
    }
    pfq_ts(raw)   //! the norm's stamp carries the pass; the rope's lands empty so the profile's roles stay in place
}

//! the fused pass's set over the window's planes: the q rows, the projection rows, the layer's slab, the rope rows, the norm rows,
//! the token block (bound, unread by the window form), the bias rows and the projection rows again as the V source
def private pf_qkn_rope_set(L : RLayer) : VkDescriptorSet {
    let np = g_rd.pf_np
    let kvd = g_rd.kv_dim
    let qrow = g_rd.q_gated ? 2l * g_rd.qd : g_rd.qd
    let slab = rd_slab(L)
    let bias_buf = g_rd.has_bias ? g_rd.bias_dev : g_rd.norms_dev
    let bias_bytes = g_rd.has_bias ? g_rd.bias_bytes : g_rd.norms_bytes
    let bufs = fixed_array(g_rd.pf_q, g_rd.pf_kv, slab.k, slab.v, g_rd.pf_cos, g_rd.norms_dev, g_rd.tok_meta.buf, bias_buf, g_rd.pf_kv)
    let sizes = fixed_array(np * qrow * 4l, np * 2l * kvd * 4l, rd_mir_bytes(), rd_mir_bytes(), np * g_rd.cos_elems * 4l, g_rd.norms_bytes,
        TOK_META_BYTES * g_rd.nb, bias_bytes, np * 2l * kvd * 4l)
    let bits = fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, VHZ_MIR, VHZ_MIR, VHZ_COS, 0u, 0u, 0u, VHZ_KVK | VHZ_KVV)
    return rd_kv16() ? set_qkn_rope_f16_cls(bufs, sizes, bits) : set_qkn_rope_cls(bufs, sizes, bits)
}

def private pf_qk_rms_set : VkDescriptorSet {
    let np = g_rd.pf_np
    let qrow = g_rd.q_gated ? 2l * g_rd.qd : g_rd.qd
    return set_qk_rms_cls(fixed_array(g_rd.pf_q, g_rd.pf_kv, g_rd.norms_dev),
        fixed_array(np * qrow * 4l, np * 2l * g_rd.kv_dim * 4l, g_rd.norms_bytes), fixed_array(VHZ_Q, VHZ_KVK, 0u))
}

//! the ask stands until the next ask
[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm")]   // per prefill call, reached through the tier's registered seat
def vk_rdec_head_warm_ask(on : bool) {
    if (g_rd != null) {
        g_rd.pf_head_ask = on
    }
}

[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm")]   // per prefill call, reached through the tier's registered seat
def vk_rdec_head_warm_rows : int64 => g_rd != null ? g_rd.pf_head_rows : 0l

//! a window segment's mirror base in layer `L`: its region's, past the rows the segments packed before it hold
def private pf_seg_mir_base(L : RLayer; sg : RdecPfSeg) : int64 {
    assert(sg.koff == 0l || L.ring == 0l, "vk prefill: a packed segment on a sliding layer's ring")
    return rd_mir_base(L, sg.region) + sg.koff * rd_kv_row_units(L.kvd)
}

//! The armed driver seats several segments in one region: no sliding layer's ring, no block-codec store
[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-scratch")]
def vk_rdec_segs_packed : bool {
    return false if (g_rd == null || !g_rd.ready || rd_kv_block_codec())
    for (L in g_rd.layers) {
        return false if (L.ring != 0l)
    }
    return true
}

var private g_pf_hidden_ask = false
var private @scratch @exact_size g_pf_hidden : array<float>   //! the last chain's final residual rows, [npos x dim], where it was asked for them

//! Whether the next chains land every row's final residual - the rows an embedding pools - beside the logits; the ask stands until
//! the next ask
[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-hidden-rows")]   // per prefill call, reached through the tier's registered seat
def vk_rdec_hidden_ask(on : bool) {
    g_pf_hidden_ask = on
}

//! `nrows` of the last chain's final residual rows from window row `src_row` into `dst` at element `dst_off`
[hot_path]   // per prefill call, reached through the tier's registered seat
def vk_rdec_hidden_rows(var dst : array<float>; src_row, dst_off, nrows : int64) {
    let dim = g_rd.dim
    copy_floats(g_pf_hidden, src_row * dim, dst, dst_off, nrows * dim)
}

//! Resident-driver prefill: `npos` embedded rows (x_batch, [npos x dim]) + per-position rope rows
//! (cos_batch, [npos x hs]) -> the last row's logits, filling the selected mirror region at positions [pos0, pos0 + npos).
[hot_path]   // the per-window resident prefill — encode + one submit per window
def vk_rdec_prefill(x_batch : array<float>; cos_batch : array<float>; ple_rows : array<float>; ple_raw : bool; npos, pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    rd_chain_settle()
    let noids : array<int64>
    vk_demotion_check("prefill")
    assert(!ple_raw || g_rd.ple_gpu, "vk_rdec_prefill: raw per-layer-embedding rows without the projection on device")
    let nosegs : array<RdecPfSeg>
    pf_run(x_batch, noids, false, 1.0, cos_batch, ple_rows, ple_raw, npos, pos0, logits, experts, nosegs)
}

def ple_proj_shape_ok(dim, all : int64) : bool
    => f16g_shape_ok(dim) || (g_gpu.coopmat_mode == COOPMAT_MM && khr_kq_tile_on() && dim % 16l == 0l && all % 16l == 0l)

//! why the window's projection takes the router GEMV over the f16 tile ("" where the tile serves it)
def ple_proj_gemv_why(dim, all, w16_bytes : int64) : string {
    if (!ple_proj_shape_ok(dim, all)) {
        return "no f16 tile serves it (the cm2 tile wants a row width of {int64(F16G_BK) * F16G_SPLIT}-multiples, the KHR tile 16-multiples; dim {dim}, {all} rows)"
    }
    return vk_range_ok(w16_bytes) ? "" : "its f16 panel ({w16_bytes} bytes) passes the device's storage range {vk_max_storage_range()}"
}

//! the projection rows' GEMV class rail: the f32 rows', or the bf16 rows' word-pair or halfword stamp by the model dim
def private ple_proj_rail_ensure(bf16 : bool) : bool {
    if (!bf16) return ensure_router_gemv_cls()
    return ple_proj_bf16_hw(g_rd.dim) ? ensure_router_gemv_bf16_hw_cls() : ensure_router_gemv_bf16_cls()
}

//! the bf16 projection rows' GEMV set over `bufs`: the word-pair stamp at an even model dim, the halfword stamp at an odd one (every
//! row then starts mid-word)
def private ple_proj_bf16_set(bufs : uint64 const[4]; sizes : int64 const[4]; gbits : uint const[4]) : VkDescriptorSet
    => ple_proj_bf16_hw(g_rd.dim) ? set_router_gemv_bf16_hw_cls(bufs, sizes, gbits) : set_router_gemv_bf16_cls(bufs, sizes, gbits)

//! The pre-step's projection on device off the file's own rows (bf16 halves `w16`, else f32 `w32`) and its norm row: the router GEMV
//! under 1/sqrt(dim) at the token command, the window's f16 tile over a panel it converts each window (else the router GEMV) before
//! layer 0. Refused past `pf_ready`: these sets build with the prefill buffers.
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def vk_rdec_set_ple_proj(w16 : array<uint16>; w32 : array<float>; pnorm : array<float>; all : int64) : bool {
    if (g_gpu != null && g_gpu.dry) {
        return false
    }
    assert(g_rd != null && g_rd.ple > 0l, "vk_rdec_set_ple_proj before set_ple")
    let bf16 = !empty(w16)
    let n = bf16 ? long_length(w16) : long_length(w32)
    if (n != all * g_rd.dim || long_length(pnorm) != g_rd.ple || g_rd.pf_ready
            || !(ple_proj_rail_ensure(bf16) && ensure_ple_finish_cls())) {
        return false
    }
    let wp_bytes = n * (bf16 ? 2l : 4l)
    if (!vk_range_ok(wp_bytes)) {   //! the own rows bind whole in the token command's GEMV and the window's
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the per-layer-embedding projection rows ({wp_bytes} bytes) pass the device's storage range {vk_max_storage_range()}\n")
        return false
    }
    g_rd.ple_wp_bf16 = bf16
    g_rd.ple_wp_bytes = wp_bytes
    g_rd.ple_wp_dev = make_device_buf(g_rd.ple_wp_bytes)
    g_rd.ple_pnorm_dev = make_device_buf(g_rd.ple * 4l)
    unsafe {
        upload_region_at(g_rd.ple_wp_dev, 0l, bf16 ? addr<uint8 const?>(w16[0]) : addr<uint8 const?>(w32[0]), g_rd.ple_wp_bytes)
        upload_region_at(g_rd.ple_pnorm_dev, 0l, addr<uint8 const?>(pnorm[0]), g_rd.ple * 4l)
    }
    let gemv_why = ple_proj_gemv_why(g_rd.dim, all, n * 2l)
    if (gemv_why == "") {   //! the window's f16 panel, rewritten off the own rows every window
        g_rd.ple_w16_bytes = n * 2l
        g_rd.ple_w16_dev = make_device_buf(g_rd.ple_w16_bytes)
    }
    let form = gemv_why == "" ? "the f16 tile ({g_gpu.coopmat_mode == COOPMAT_CM2 ? "cm2" : "KHR"})" : "the router GEMV: {gemv_why}"
    to_log(LOG_INFO, "dasLLAMA vulkan resident: the per-layer-embedding window projection runs on {form}\n")
    // the token command's half: the projected row plane, the row GEMV over the embedded row (no norm: the wn slot binds the norm row
    // unread) and the per-slice finish in place on the gathered row
    let nb = g_rd.nb
    g_rd.tok_plep = make_device_buf(all * 4l * nb)
    let pbufs = fixed_array(g_rd.ple_wp_dev, g_rd.x_dev, g_rd.tok_plep, g_rd.ple_pnorm_dev)
    let psizes = fixed_array(g_rd.ple_wp_bytes, g_rd.dim * 4l * nb, all * 4l * nb, g_rd.ple * 4l)
    let pbits = fixed_array(0u, VHZ_X, VHZ_MRT, 0u)
    g_rd.s_tok_pleproj = bf16 ? ple_proj_bf16_set(pbufs, psizes, pbits) : set_router_gemv_cls(pbufs, psizes, pbits)
    g_rd.s_tok_ple_ar = set_ple_finish_cls(fixed_array(g_rd.tok_plep, g_rd.ple_pnorm_dev, g_rd.ple_dev),
        fixed_array(all * 4l * nb, g_rd.ple * 4l, all * 4l * nb), fixed_array(VHZ_MRT, 0u, VHZ_COS))
    g_rd.ple_gpu = true
    rd_invalidate_token()
    return true
}

[hot_path, arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def vk_rdec_prefill_ids(tokens : array<int64>; emb_scale : float; cos_batch : array<float>; npos, pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    rd_chain_settle()
    verify(g_rd != null && rd_emb_on(), "vk_rdec_prefill_ids without a placed token table")
    let nox : array<float>
    vk_demotion_check("prefill")
    let ple_tbl = pf_ple_tbl_on()
    assert(g_rd.ple == 0l || ple_tbl, "vk_rdec_prefill_ids: a per-layer-embedding model without its token table and projection on device")
    let nosegs : array<RdecPfSeg>
    pf_run(nox, tokens, true, emb_scale, cos_batch, nox, ple_tbl, npos, pos0, logits, experts, nosegs)
}

[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-segments")]
def vk_rdec_prefill_segs(x_batch : array<float>; tokens : array<int64>; use_ids : bool; emb_scale : float; cos_batch : array<float>;
                         segs : array<RdecPfSeg>; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    rd_chain_settle()
    assert(g_rd != null && !empty(segs), "vk_rdec_prefill_segs: no driver, or no segment")
    let kvq = rd_kv_block_codec()
    let ple_tbl = g_rd.ple > 0l && use_ids && pf_ple_tbl_on()   //! every segment's side rows gathered off its ids and finished on the device
    assert((g_rd.ple == 0l || ple_tbl) && !g_rd.hc && !g_rd.experts_host && !kvq,
        "vk_rdec_prefill_segs: the chain holds one session a window on a per-layer-embedding model whose rows the host embedded, a hyper-connection model, routed experts on the host, or a block-codec mirror")
    let nox : array<float>
    vk_demotion_check("prefill")
    var npos = 0l
    for (sg in segs) {
        npos += sg.npos
    }
    pf_run(x_batch, tokens, use_ids, emb_scale, cos_batch, nox, ple_tbl, npos, segs[0].pos0, logits, experts, segs)
}

//! The windows that held several sessions' chunks as segments in this process (a cell's witness)
def vk_rdec_prefill_segment_windows : int64 => g_rd != null ? g_rd.pf_seg_windows : 0l

//! After a resident prefill, hand recurrent layer `l`'s device slot to the session `stp`/`histp` name (valid, dirty, bound)
[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def vk_rdec_prefill_dn_own(l : int64; stp : float?; histp : float?) {
    rd_chain_settle()
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    var L & = unsafe(g_rd.layers[l])
    var st & = unsafe(L.dn[g_rd.region])
    dn_slot_bind(st.own, stp, histp, g_rd.dn_parity[g_rd.region], true)
}

//! False when the gather rail cannot build on this device for `fmt` (the caller keeps the CPU embed); true arms the window's
//! ids gather and the one-row command's - which re-records, its row now gathered off the step's token id
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends"), arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-token-gather")]
def vk_rdec_set_emb(emb_block : int64; fmt : int; escale : float) : bool {
    return rd_gather_seat(g_rd != null, "vk_rdec_set_emb before prepare", fmt) $ {
        g_rd.emb_blk = emb_block
        g_rd.emb_fmt = fmt
        rd_tok_emb_staging(escale)
        g_rd.s_tok_emb = emb_cls_set(fmt, arena_planes(fmt, emb_block), g_rd.tok_ids_host.buf, 4l * g_rd.nb, 0u,
            g_rd.tok_xh_host.buf, g_rd.dim * 4l * g_rd.nb, g_rd.x_dev, g_rd.dim * 4l * g_rd.nb, VHZ_X)
        rd_ple_tok_set()
        rd_invalidate_token()
    }
}

//! the token commands' side-row gather, once the per-layer table, the projection and the steps' ids all ride the device: the
//! table's stamp at the row width layers x ple off each row's token id, a row without one copying the host's staged row
def private rd_ple_tok_set {
    return if (g_rd.ple <= 0l || g_rd.ple_tbl_blk < 0l || !g_rd.ple_gpu || !rd_emb_on())
    let all = g_rd.n_layers * g_rd.ple
    g_rd.s_tok_ple = emb_cls_set(g_rd.ple_tbl_fmt, arena_planes(g_rd.ple_tbl_fmt, g_rd.ple_tbl_blk), g_rd.tok_ids_host.buf, 4l * g_rd.nb, 0u,
        g_rd.ple_host.buf, all * 4l * g_rd.nb, g_rd.ple_dev, all * 4l * g_rd.nb, VHZ_COS)
    g_rd.ple_tok_on = true
}

//! the token commands' gather staging - a row's token id and the host's row a row without one stages, `nb` rows of each - and the scale their rows take
def private rd_tok_emb_staging(escale : float) {
    g_rd.emb_scale = escale
    g_rd.tok_ids_host = make_host_buf(4l * g_rd.nb, true)
    g_rd.tok_xh_host = make_host_buf(g_rd.dim * 4l * g_rd.nb, true)
}

//! An F32/F16/BF16 table's exact rows uploaded whole as the gather's f32 plane, for the window and the one-row command. False past
//! RDEC_EMB_F32_CAP, the device's SSBO range or the weight budget, or when the gather rail cannot build (the caller keeps the CPU embed)
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends"), arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-token-gather")]
def vk_rdec_upload_emb_f32(fblob : array<float>; off, vocab, dim : int64; escale : float) : bool {
    if (g_gpu != null && g_gpu.dry) {
        return false
    }
    assert(g_rd != null, "vk_rdec_upload_emb_f32 before prepare")
    let bytes = vocab * dim * 4l
    if (bytes > RDEC_EMB_F32_CAP || !vk_range_ok(bytes) || g_gpu.resident_bytes + bytes > g_gpu.weight_budget
            || !ensure_emb_gather_f32_cls()) {
        return false
    }
    g_rd.emb_f32 = make_device_buf(bytes)
    g_rd.emb_f32_bytes = bytes
    g_gpu.resident_bytes += bytes
    upload_region_at(g_rd.emb_f32, 0l, unsafe(addr<uint8 const?>(fblob[int(off)])), bytes)
    rd_tok_emb_staging(escale)
    g_rd.s_tok_emb = emb_f32_set(g_rd.emb_f32, bytes, g_rd.tok_ids_host.buf, 4l * g_rd.nb, 0u, g_rd.tok_xh_host.buf, g_rd.dim * 4l * g_rd.nb,
        g_rd.x_dev, g_rd.dim * 4l * g_rd.nb, VHZ_X)
    rd_ple_tok_set()
    rd_invalidate_token()
    return true
}

def private rd_gather_seat(ready : bool; what : string; fmt : int; seat : block) : bool {
    return true if (g_gpu != null && g_gpu.dry)
    if (!ready) {
        panic(what)
    }
    return false if (!emb_cls_ensure(fmt))
    invoke(seat)
    return true
}

//! The per-layer-embedding token table in the arena: the ids prefill gathers the side rows from it (the embed gather's kernel at
//! the row width layers x ple, scaled by sqrt(ple)) where the host gathered and uploaded them a window at a time
def vk_rdec_set_ple_table(blk : int64; fmt : int) : bool {
    return rd_gather_seat(g_rd != null && g_rd.ple > 0l, "vk_rdec_set_ple_table before set_ple", fmt) $ {
        g_rd.ple_tbl_blk = blk
        g_rd.ple_tbl_fmt = fmt
    }
}

//! the ids prefill gathers and finishes the pre-step on device: the table placed and the projection held
def private pf_ple_tbl_on : bool => g_rd.ple_tbl_blk >= 0l && g_rd.ple_gpu

//! once per residency, after `pf_setup`
[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain")]
def private pf_hc_setup {
    assert(g_rd.hc && g_rd.experts_host, "vk prefill: the hyper-connection window before its seats")
    let np = g_rd.pf_np
    let dim = g_rd.dim
    let hc = g_rd.hc_n
    let hcd = hc * dim
    let lr = g_rd.hc_lr
    let ner = g_rd.ner
    verify(ensure_hc_init_cls() && ensure_hc_norm_cls() && ensure_router_gemm_cls() && ensure_cls_dn_rq() && ensure_hc_lo_rq_cls()
        && ensure_hc_mix_cls() && ensure_hc_comb_cls() && (!g_rd.ngram || (ensure_ple_gate_cls() && ensure_ple_conv_cls())),
        "vk prefill: the hyper-connection class rails must engage on a live device")
    g_rd.pf_hcres = make_device_buf(np * hcd * 4l)
    g_rd.pf_hcxn = make_device_buf(np * hcd * 4l)
    g_rd.pf_hcxq = make_device_buf(np * hcd)
    g_rd.pf_hcxs = make_device_buf(np * (hcd / Q8_BLOCK_ELEMS) * 4l)
    g_rd.pf_hclo = make_device_buf(np * lr * 4l)
    g_rd.pf_hcup = make_device_buf(np * hcd * 4l)
    g_rd.pf_hcinj = make_device_buf(np * hc * 4l)
    pf_host_sum_ensure()
    let n_sites = long_length(g_rd.hc_sites)
    g_rd.pf_hc_meta0 = length(g_rd.pf_meta)
    g_rd.pf_meta |> reserve(long_length(g_rd.pf_meta) + n_sites * 2l + 2l)
    for (_i in range64(n_sites * 2l + 2l)) {
        g_rd.pf_meta |> push(make_host_buf(BATCH_META_BYTES, true))
    }
    g_rd.pf_s_hc_init = set_hc_init_cls(fixed_array(g_rd.pf_x, g_rd.pf_hcres), fixed_array(np * dim * 4l, np * hcd * 4l), fixed_array(VHZ_X, VHZ_X))
    g_rd.pf_s_hc_norm = set_hc_norm_cls(fixed_array(g_rd.pf_hcres, g_rd.pf_hcxn, g_rd.hcn_dev),
        fixed_array(np * hcd * 4l, np * hcd * 4l, g_rd.hcn_bytes), fixed_array(VHZ_X, VHZ_XB2, 0u))
    g_rd.pf_s_hc_inj = set_router_gemm_cls(fixed_array(g_rd.hcinjw_dev, g_rd.pf_hcxn, g_rd.pf_hcinj),
        fixed_array(g_rd.hcinjw_bytes, np * hcd * 4l, np * hc * 4l), fixed_array(0u, VHZ_XB2, VHZ_MRT))
    g_rd.pf_s_hc_xnrq = set_rq_cls(fixed_array(g_rd.pf_hcxn, g_rd.pf_hcxq, g_rd.pf_hcxs),
        fixed_array(np * hcd * 4l, np * hcd, np * (hcd / Q8_BLOCK_ELEMS) * 4l), fixed_array(VHZ_XB2, VHZ_XQ, VHZ_XQ))
    g_rd.pf_s_hc_lorq = set_hc_lo_rq_cls(fixed_array(g_rd.pf_hclo, g_gpu.hq_dev, g_gpu.hs_dev),
        fixed_array(np * lr * 4l, BATCH_HQ_BYTES, BATCH_HS_BYTES), fixed_array(VHZ_GATE, VHZ_HQ, VHZ_HQ))
    g_rd.pf_s_hc_mix = set_hc_mix_cls(fixed_array(g_rd.pf_hcxn, g_rd.pf_hcup, g_rd.pf_xb),
        fixed_array(np * hcd * 4l, np * hcd * 4l, np * dim * 4l), fixed_array(VHZ_XB2, VHZ_UP, VHZ_XB))
    g_rd.pf_s_hc_comb_a = set_hc_comb_cls(fixed_array(g_rd.pf_hcres, g_rd.pf_xb2, g_rd.pf_hcinj, g_rd.pf_hacc_dev, g_rd.pf_mlog),
        fixed_array(np * hcd * 4l, np * dim * 4l, np * hc * 4l, np * dim * 4l, np * ner * 4l), fixed_array(VHZ_X, VHZ_XB2, VHZ_MRT, VHZ_MDN, VHZ_MRT))
    g_rd.pf_s_hc_comb_f = set_hc_comb_cls(fixed_array(g_rd.pf_hcres, g_rd.pf_ffnout, g_rd.pf_hcinj, g_rd.pf_hacc_dev, g_rd.pf_mlog),
        fixed_array(np * hcd * 4l, np * dim * 4l, np * hc * 4l, np * dim * 4l, np * ner * 4l), fixed_array(VHZ_X, VHZ_FFO, VHZ_MRT, VHZ_MDN, VHZ_MRT))
    if (g_rd.ngram) {
        let ind = g_rd.ngram_ind
        let hist_bytes = g_rd.regions * g_rd.ngram_nrows * hcd * 4l
        g_rd.pf_hcemb = make_call_buf(np * ind * 4l)
        g_rd.pf_hcembq = make_device_buf(np * ind)
        g_rd.pf_hcembs = make_device_buf(np * (ind / Q8_BLOCK_ELEMS) * 4l)
        g_rd.pf_plekey = make_device_buf(np * hcd * 4l)
        g_rd.pf_pleval = make_device_buf(np * dim * 4l)
        g_rd.pf_plegated = make_device_buf(np * hcd * 4l)
        g_rd.pf_hpanel = make_device_buf(np * hcd * 4l)
        g_rd.pf_s_ple_rq = set_rq_cls(fixed_array(g_rd.pf_hcemb, g_rd.pf_hcembq, g_rd.pf_hcembs),
            fixed_array(np * ind * 4l, np * ind, np * (ind / Q8_BLOCK_ELEMS) * 4l), fixed_array(VHZ_COS, VHZ_XQ, VHZ_XQ))
        g_rd.pf_s_ple_gate = set_ple_gate_cls(
            fixed_array(g_rd.pf_plekey, g_rd.pf_hcres, g_rd.pf_pleval, g_rd.pf_plegated, g_rd.plehist_dev, g_rd.plew_dev, g_rd.tok_meta.buf, g_rd.pf_hpanel),
            fixed_array(np * hcd * 4l, np * hcd * 4l, np * dim * 4l, np * hcd * 4l, hist_bytes, g_rd.plew_bytes, TOK_META_BYTES, np * hcd * 4l),
            fixed_array(VHZ_Q, VHZ_X, VHZ_ATT, VHZ_AQ, VHZ_DNSM, 0u, 0u, VHZ_DNSM))
        g_rd.pf_s_ple_conv = set_ple_conv_cls(
            fixed_array(g_rd.pf_hcres, g_rd.pf_plegated, g_rd.plehist_dev, g_rd.plew_dev, g_rd.tok_meta.buf, g_rd.pf_hpanel),
            fixed_array(np * hcd * 4l, np * hcd * 4l, hist_bytes, g_rd.plew_bytes, TOK_META_BYTES, np * hcd * 4l),
            fixed_array(VHZ_X, VHZ_AQ, VHZ_DNSM, 0u, 0u, VHZ_DNSM))
    }
    var nseg = 1l
    for (L in g_rd.layers) {
        nseg += L.moe ? 1l : 0l
    }
    g_rd.pf_seg |> reserve(nseg)
    for (_i in range64(nseg)) {
        g_rd.pf_seg |> push(alloc_cmd())
    }
    g_rd.pf_hc_ready = true
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain")]
def private pf_hc_mix(raw : VkCommandBuffer; var h : VkHaz; site, wlen : int64; head : bool) {
    let np = g_rd.pf_np
    let dim = g_rd.dim
    let hc = g_rd.hc_n
    let hcd = hc * dim
    let lr = g_rd.hc_lr
    let rows = head ? 1l : wlen
    let S & = unsafe(g_rd.hc_sites[site])
    var pcn = GkHcNormArgs(dim = uint(dim), hc = uint(hc), woff = uint(site * hcd), row0 = uint(head ? wlen - 1l : 0l), eps = g_rd.eps)
    enc_hc_norm_cls(raw, h, g_rd.pf_s_hc_norm, pcn, rows * hc)
    if (!head) {
        router_gemm_enc(raw, h, g_rd.pf_s_hc_inj, rows, hc, hcd, site * hc * hcd)
    }
    rq_enc(raw, h, g_rd.pf_s_hc_xnrq, false, 0l, rows * hcd)
    let q8 = int(KqFmt.q8)
    let pd = arena_planes(q8, S.bdown)
    pf_gemm_enc(raw, h, g_rd.pf_hc_meta0 + int(site * 2l), q8, pd.wq, pd.ws, g_rd.pf_hcxq, g_rd.pf_hcxs, g_rd.pf_hclo,
        pd.wqb, pd.wsb, np * hcd, np * (hcd / Q8_BLOCK_ELEMS) * 4l, np * lr * 4l, VHZ_XQ, VHZ_GATE, hcd, lr, S.bdown, rows, false, 0l, 0l, false)
    var pcl = HcLoArgs(nblk = uint(rows * lr / Q8_BLOCK_ELEMS), inv_hc = 1.0 / float(hc))
    enc_hc_lo_rq_cls(raw, h, g_rd.pf_s_hc_lorq, pcl, rq_wgs(false, rows * lr))
    let pu = arena_planes(q8, S.bup)
    pf_gemm_enc(raw, h, g_rd.pf_hc_meta0 + int(site * 2l + 1l), q8, pu.wq, pu.ws, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.pf_hcup,
        pu.wqb, pu.wsb, BATCH_HQ_BYTES, BATCH_HS_BYTES, np * hcd * 4l, VHZ_HQ, VHZ_UP, lr, hcd, S.bup, rows, false, 0l, 0l, false)
    var pcm = GkHcMixArgs(dim = uint(dim), hc = uint(hc), rows = uint(rows))
    enc_hc_mix_cls(raw, h, g_rd.pf_s_hc_mix, pcm, (rows * dim + 255l) / 256l)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain")]
def private pf_hc_scatter(raw : VkCommandBuffer; var h : VkHaz; L : RLayer; wlen, row0 : int64; ffn : bool) {
    let hcd = g_rd.hc_n * g_rd.dim
    let moe = ffn && L.moe
    let rows = wlen - row0
    var pcc = GkHcCombArgs(dim = uint(g_rd.dim), hc = uint(g_rd.hc_n), moe_on = moe ? 1u : 0u, sh_on = (moe && has_dense_triple(L)) ? 1u : 0u,
        gated = (moe && g_rd.sh_gated) ? 1u : 0u, rows = uint(rows), row0 = uint(row0), lstride = uint(g_rd.ner))
    enc_hc_comb_cls(raw, h, ffn ? g_rd.pf_s_hc_comb_f : g_rd.pf_s_hc_comb_a, pcc, (rows * hcd + 255l) / 256l)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain")]
def private pf_hc_side(raw : VkCommandBuffer; var h : VkHaz; wlen, pos0w : int64) {
    let np = g_rd.pf_np
    let dim = g_rd.dim
    let hc = g_rd.hc_n
    let hcd = hc * dim
    let ind = g_rd.ngram_ind
    let nrows = g_rd.ngram_nrows
    let q8 = int(KqFmt.q8)
    rq_enc(raw, h, g_rd.pf_s_ple_rq, false, 0l, wlen * ind)
    let mk = g_rd.pf_hc_meta0 + int(long_length(g_rd.hc_sites) * 2l)
    let pk = arena_planes(q8, g_rd.ple_bkey)
    pf_gemm_enc(raw, h, mk, q8, pk.wq, pk.ws, g_rd.pf_hcembq, g_rd.pf_hcembs, g_rd.pf_plekey,
        pk.wqb, pk.wsb, np * ind, np * (ind / Q8_BLOCK_ELEMS) * 4l, np * hcd * 4l, VHZ_XQ, VHZ_Q, ind, hcd, g_rd.ple_bkey, wlen, false, 0l, 0l, false)
    let pv = arena_planes(q8, g_rd.ple_bval)
    pf_gemm_enc(raw, h, mk + 1, q8, pv.wq, pv.ws, g_rd.pf_hcembq, g_rd.pf_hcembs, g_rd.pf_pleval,
        pv.wqb, pv.wsb, np * ind, np * (ind / Q8_BLOCK_ELEMS) * 4l, np * dim * 4l, VHZ_XQ, VHZ_ATT, ind, dim, g_rd.ple_bval, wlen, false, 0l, 0l, false)
    var pcg = GkPleGateArgs(dim = uint(dim), hc = uint(hc), nrows = uint(nrows), rows = uint(wlen), pos0 = uint(pos0w), eps = g_rd.eps, inv_sq = 1.0 / sqrt(float(dim)))
    enc_ple_gate_cls(raw, h, g_rd.pf_s_ple_gate, pcg, wlen * hc)
    var pcc = GkPleConvArgs(dim = uint(dim), hc = uint(hc), kern = uint(g_rd.ngram_kern), dil = uint(g_rd.ngram_dil), rows = uint(wlen), pos0 = uint(pos0w))
    enc_ple_conv_cls(raw, h, g_rd.pf_s_ple_conv, pcc, (wlen * hcd + 255l) / 256l)
    let m = min(wlen, nrows)
    vhz_dep(raw, h, VHZ_DNSM, VHZ_DNSM, true)
    if (!empty(g_pf_wcaps) && g_pf_wcaps[0].n > 0l) {
        let wc = g_pf_wcaps[0]
        for (ci in range64(wc.n)) {
            let r = int64(g_pf_cap_words[wc.rec + ci * 2l])
            let dst = int64(g_pf_cap_words[wc.rec + ci * 2l + 1l]) * 4l + g_rd.snap_ring_off
            cap_ring_rows(r, nrows, pos0w) $(rr, slot : int64; from_window : bool) {
                let at = slot * hcd * 4l
                if (from_window) {
                    cmd_copy_range(raw, g_rd.pf_hpanel, rr * hcd * 4l, g_rd.snap_dev, dst + at, hcd * 4l)
                } else {
                    cmd_copy_range(raw, g_rd.plehist_dev, g_rd.region * nrows * hcd * 4l + at, g_rd.snap_dev, dst + at, hcd * 4l)
                }
            }
        }
    }
    for (r in range64(wlen - m, wlen)) {
        let slot = (pos0w + r) % nrows
        cmd_copy_range(raw, g_rd.pf_hpanel, r * hcd * 4l, g_rd.plehist_dev, (g_rd.region * nrows + slot) * hcd * 4l, hcd * 4l)
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain"), arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-draft-head")]
def private pf_run_hc(x_batch : array<float>; cos_batch : array<float>; side : array<float>; npos, pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    assert(g_rd != null && g_rd.ready && g_rd.hc && g_rd.experts_host, "vk prefill: the hyper-connection window before the driver's seats")
    pf_setup()
    if (!g_rd.pf_hc_ready) {
        pf_hc_setup()
    }
    rd_carry_sync()
    g_pf_segs |> clear()
    g_pf_segs |> push(RdecPfSeg(row0 = 0l, npos = npos, pos0 = pos0, region = g_rd.region))
    if (g_rd.hybrid) {
        pf_dn_begin()
    }
    let dim = g_rd.dim
    let hc = g_rd.hc_n
    let hcd = hc * dim
    let ind = g_rd.ngram ? g_rd.ngram_ind : 0l
    assert(!g_rd.ngram || long_length(side) >= npos * ind, "vk prefill: the side rows are short of the prompt")
    let cos_elems = g_rd.cos_elems
    g_rd.pf_head_rows = 0l
    let bp = pf_prologue_block()
    let warm = (g_rd.pf_head_ask && g_rd.head_on && g_rd.carry_live && g_rd.hc_nextn
        && (g_rd.pf_hc_head_ready || (!g_rd.pf_head_declined && pf_head_setup() && pf_hc_head_setup())))
    unsafe {
        var w0 = 0l
        while (w0 < npos) {
            let wlen = pf_window_len(npos, w0)
            let last = w0 + wlen >= npos
            let pos0w = pos0 + w0
            pf_window_segs(w0, wlen)
            pf_window_caps(w0)
            rd_tok_row(pos0w, wlen)
            upload_region_at(g_rd.pf_x, 0l, addr<uint8 const?>(x_batch[int(w0 * dim)]), wlen * dim * 4l)
            upload_region_at(g_rd.pf_cos, 0l, addr<uint8 const?>(cos_batch[int(w0 * cos_elems)]), wlen * cos_elems * 4l)
            if (g_rd.ngram) {
                upload_region_at(g_rd.pf_hcemb, 0l, addr<uint8 const?>(side[int(w0 * ind)]), wlen * ind * 4l)
            }
            if (warm) {
                pf_warm_cos(cos_batch, w0, wlen)
            }
            var seg = 0l
            var raw = g_rd.pf_seg[seg]
            var h = cmd_open(raw, true)
            if (g_rd.hybrid) {
                pf_dn_zero_fresh(raw, h)
            }
            var pcinit = GkHcInitArgs(dim = uint(dim), hc = uint(hc), rows = uint(wlen))
            enc_hc_init_cls(raw, h, g_rd.pf_s_hc_init, pcinit, (wlen * hcd + 255l) / 256l)
            for (l in range64(g_rd.n_layers)) {
                var L & = g_rd.layers[l]
                let b = int(l * PF_ROLES)
                let gu6 = pf_gu6(l)
                let dn6 = pf_dn6(l)
                let dnk = kq_sb(L.f2) && !dn6
                if (g_rd.ngram && l == g_rd.ngram_layer) {
                    pf_hc_side(raw, h, wlen, pos0w)
                }
                pf_hc_mix(raw, h, 2l * l, wlen, false)
                if (L.recurrent) {
                    rd_pf_recurrent(raw, h, l, b, wlen)
                } else {
                    pf_attn_block(raw, h, l, b, wlen, pos0w, false)
                }
                pf_hc_scatter(raw, h, L, wlen, 0l, false)
                pf_hc_mix(raw, h, 2l * l + 1l, wlen, false)
                let ffn_row0 = pf_ffn_slice_row0(l, g_rd.n_layers, wlen, gu6 && !L.moe, dn6)
                if (has_dense_triple(L)) {
                    pf_feed_enc(raw, h, g_rd.pf_sets[b + 9], gu6, kq_sb(L.f1) && !gu6, 0l, wlen * dim)
                    pf_ffn_block(raw, h, l, b, wlen, ffn_row0, gu6, dn6, dnk)
                }
                if (L.moe) {
                    verify(L.experts_host, "vk prefill: the hyper-connection chain serves routed experts on the host alone")
                    router_gemm_enc(raw, h, L.pf_router_set, wlen, g_rd.ner, dim, L.router_slot * g_rd.ner * dim)
                    seg++
                    pf_host_cut(raw, g_rd.pf_seg[seg], h, l, wlen, ffn_row0, experts)
                }
                pf_hc_scatter(raw, h, L, wlen, ffn_row0, true)
            }
            pf_hc_mix(raw, h, g_rd.hc_final_site, wlen, true)
            if (last) {
                rq_enc(raw, h, g_rd.pf_sets[bp + 1], kq_sb(g_rd.cls_fmt_marker), 0l, dim)
                pf_cls_enc(raw, h)
                vhz_dep(raw, h, VHZ_LOG, 0u, true)
                cmd_copy_whole(raw, g_rd.logits_dev, g_rd.pf_logits_host.buf, g_rd.vocab * 4l)
                if (g_rd.carry_live) {
                    vhz_dep(raw, h, VHZ_X, 0u, true)
                    cmd_copy_range(raw, g_rd.pf_hcres, (wlen - 1l) * hcd * 4l, g_rd.hid_host.buf, 0l, hcd * 4l)
                }
            }
            if (warm) {
                pf_hc_head_warm(raw, h, w0, wlen, pos0)
            }
            if (g_rd.hybrid) {
                pf_dn_window_done()
            }
            cmd_close(raw)
            verify(seg + 1l == long_length(g_rd.pf_seg), "vk prefill: the window's segments must be the routed layers plus one")
            submit_wait(raw)
            w0 += wlen
        }
        if (g_rd.hybrid) {
            pf_dn_end()
            pf_caps_done()
        }
        host_buf_read(g_rd.pf_logits_host, addr<void?>(logits[0]), g_rd.vocab * 4l)
    }
}

//! The hyper-connection prefill seam (`RdecPrefillHcFn`): the embedded rows, the rope rows and the n-gram side rows of a prompt
//! through the window chain, `experts` the host's routed sum of a layer's rows between the segments
[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain")]
def vk_rdec_prefill_hc(x_batch : array<float>; cos_batch : array<float>; side : array<float>; npos, pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    pf_run_hc(x_batch, cos_batch, side, npos, pos0, logits, experts)
}

//! why an attention layer takes the chunked pair instead of a flash tile: no tile on the device at all (no cooperative matrix at
//! a width the tiles run at, the tile knob off, or no f16 feed), a softcap off an ungated 256 head, a gated head off 256, or a head size no stamp serves
def private pf_chunked_why(tile_dev : bool; hs : int64; cap : float; gated : bool) : string {
    return "no flash tile on this device" if (!tile_dev)
    return "the softcap off an ungated 256 head" if (cap != 0.0 && (gated || hs != 256l))
    return "a gated head off 256" if (gated && hs != 256l)
    return "head size {hs}"
}

[arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-tq4-basis"), arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-segments")]
def private pf_attn_block(raw : VkCommandBuffer; var h : VkHaz; l : int64; b : int; wlen, pos0w : int64; feed_fused : bool) {
    let dim = g_rd.dim
    let np = g_rd.pf_np
    let nlfin = rd_norm_final()   // nolint:LINT021 — the norm row offsets multiply 64-bit before the push-field narrow
    let hsmax = g_rd.head_size   // nolint:LINT021 — the q/k norm rows sit at the widest class's stride; the row offset multiplies run 64-bit before the push-field narrow
    let cos_elems = g_rd.cos_elems   // nolint:LINT021 — the rope rows' stride narrows into the push after a 64-bit multiply
    // this layer's attention class: gemma-4's sliding and global layers differ in head size, kv width, rope and window
    let hs = g_rd.layers[l].hs
    let qd = g_rd.layers[l].qd
    let kvd = g_rd.layers[l].kvd
    let kv_mul = uint(g_rd.layers[l].kv_mul)
    let window = uint(g_rd.layers[l].window)
    let kvsh = g_rd.layers[l].kv_shared
    let pairs = int(qd / 2l + (kvsh ? 0l : kvd / 2l))
    let fa128 = pf_fa128s() && hs == 128l
    let wide = hs > 256l
    let vfk = g_rd.layers[l].vfk
    let vnorm = g_rd.layers[l].vnorm
    // f16-fed roles (mode 4's cm2 tiles): the f32->f16 convert feeds decode-in-load; the fill's tile rule keys off the same flag
    let fq6 = pf_qkv6(l)
    let fo6 = pf_f16_feed(g_rd.layers[l].fo)
    // kq consumers take the Q8_K quant/act forms — unless the f16 feed serves them (k4 cm2)
    let fqk = kq_sb(g_rd.layers[l].fq) && !fq6
    let fok = kq_sb(g_rd.layers[l].fo) && !fo6
    let kvtq4 = rd_kvtq4()
    let kvq = rd_kv_block_codec()
    if (!feed_fused) {
        pf_feed_enc(raw, h, g_rd.pf_sets[b + 0], fq6, fqk, 0l, wlen * dim)
    }
    pfq_ts(raw)
    let pq = arena_planes(g_rd.layers[l].fq, g_rd.layers[l].bq)
    let pk = kvsh ? pq : arena_planes(g_rd.layers[l].fk, g_rd.layers[l].bk)   //! a shared-KV layer has no k/v planes: the q planes fill the slots, unused
    let pv = (vfk || kvsh) ? pk : arena_planes(g_rd.layers[l].fv, g_rd.layers[l].bv)   //! no v plane: the qk-norm pass writes V off the k rows
    let xf = pf_feed_planes(fq6, g_rd.pf_xf, g_rd.pf_xq, g_rd.pf_xs, dim)
    let kv_merged = pf_kv_merged(l, dim, kvd)
    let qrow = g_rd.q_gated ? 2l * qd : qd
    let qhs = int(g_rd.q_gated ? 2l * hs : hs)
    let tq = cm2_tiles(qrow, wlen)
    let tkv = cm2_tiles(kvd, wlen)
    pf_gemm_enc(raw, h, b + 1, g_rd.layers[l].fq, pq.wq, pq.ws, xf.q, xf.s, g_rd.pf_q,
        pq.wqb, pq.wsb, xf.qn, xf.sn, np * qrow * 4l, VHZ_XQ, VHZ_Q, dim, qrow, g_rd.layers[l].bq, wlen, fq6,
        kv_merged ? cm2_tiles(2l * kvd, wlen) : 2l * tkv)
    if (kvsh) {   //! Q only: the donor layer's K/V rows already sit in the mirror; the k and v stamps land empty so the profile's roles stay in place
        pfq_ts(raw)
        pfq_ts(raw)
    } elif (kv_merged) {
        pf_gemm_enc(raw, h, b + 2, g_rd.layers[l].fk, pk.wq, pk.ws, xf.q, xf.s, g_rd.pf_kv,
            pk.wqb, pk.wsb, xf.qn, xf.sn, np * 2l * kvd * 4l, VHZ_XQ, VHZ_KVK | VHZ_KVV, dim, 2l * kvd, g_rd.layers[l].bk, wlen, fq6, tq)
        pfq_ts(raw)   // the v stamp: the merged GEMM's cost lands on k's delta
    } else {
        pf_gemm_enc(raw, h, b + 2, g_rd.layers[l].fk, pk.wq, pk.ws, xf.q, xf.s, g_rd.pf_kv,
            pk.wqb, pk.wsb, xf.qn, xf.sn, np * 2l * kvd * 4l, VHZ_XQ, VHZ_KVK, dim, kvd, g_rd.layers[l].bk, wlen, fq6, tq + tkv)
        if (!vfk) {
            pf_gemm_enc(raw, h, b + 3, g_rd.layers[l].fv, pv.wq, pv.ws, xf.q, xf.s, g_rd.pf_v,
                pv.wqb, pv.wsb, xf.qn, xf.sn, np * kvd * 4l, VHZ_XQ, VHZ_VST, dim, kvd, g_rd.layers[l].bv, wlen, fq6, tq + tkv)
            vhz_dep(raw, h, VHZ_VST, VHZ_KVV, true)
            cmd_copy_range(raw, g_rd.pf_v, 0l, g_rd.pf_kv, wlen * kvd * 4l, wlen * kvd * 4l)   // v into pf_kv's v half
        } else {
            pfq_ts(raw)   //! a V-from-K layer's v stamp lands empty so the profile's roles stay in place
        }
    }
    let voff = uint(kv_merged ? kvd : wlen * kvd)   // the v rows' base in pf_kv, kstride apart
    if (pf_qkn_fused()) {
        pf_qkn_rope_enc(raw, h, l, voff, kv_merged ? 2l * kvd : kvd, qrow, qhs)
    } else {
        if (g_rd.qk_norm) {
            let nkvh = kvsh ? 0l : kvd / hs   //! no k/v norm groups on a shared-KV layer: nothing of its own to norm or store
            let vgroups = vnorm && !vfk ? nkvh : 0l
            var pcn = GkQkRmsArgs(hs = uint(hs), nh = uint(g_rd.n_heads), nkvh = uint(nkvh),
                qstride = uint(qrow), kstride = uint(kv_merged ? 2l * kvd : kvd), kbase = 0u,
                qwoff = uint(nlfin + dim + l * 2l * hsmax), kwoff = uint(nlfin + dim + (l * 2l + 1l) * hsmax),
                eps = g_rd.eps, qhs = uint(qhs), vfk = vfk ? 1u : 0u, vnorm = vnorm ? 1u : 0u, vbase = voff)
            enc_qk_rms_cls(raw, h, g_rd.pf_sets[b + 15], pcn, wlen * (g_rd.n_heads + nkvh + vgroups))
            pfq_ts(raw)
        }
        assert(!kvq || long_length(g_pf_wsegs) == 1l, "vk prefill: a block codec's window holds one segment")
        for (sg in g_pf_wsegs) {
            let ropewg = (sg.npos * int64(pairs) + int64(WG_X) - 1l) / int64(WG_X)
            var pcr = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(g_rd.layers[l].rot / 2l),
                neox = g_rd.neox ? 1u : 0u, kpair0 = uint(qd / 2l), ppp = uint(pairs),
                layerbase = kvq ? 0u : uint(pf_seg_mir_base(g_rd.layers[l], sg)), npos = uint(sg.npos), pos0 = uint(kvq ? sg.row0 : sg.pos0),
                voff = voff, kstride = uint(kv_merged ? 2l * kvd : kvd), qhs = uint(qhs),
                boff = uint(g_rd.layers[l].boff), hasb = g_rd.has_bias ? 1u : 0u,
                cstride = uint(cos_elems), cbase = uint(g_rd.layers[l].cbase), row0 = uint(sg.row0), ring = kvq ? 0u : uint(g_rd.layers[l].ring))
            rope_kv_b_stamp_enc(rd_kv16(), raw, h, g_rd.pf_sets[b + 4], pcr, ropewg)
        }
    }
    if (kvq) {
        let mirb = uint(rd_mir_base(g_rd.layers[l], g_pf_wsegs[0].region))
        rd_enc_kv_store(raw, h, g_rd.pf_kv_store_sets[g_rd.layers[l].mir_slab], true, kvd, hs, wlen, g_rd.n_heads, kvsh ? 0l : kvd / hs, mirb, uint(pos0w), uint(qrow), uint(qhs),
            g_rd.layers[l].ring)
        let kvlen = pos0w + wlen
        let shadow_pos0 = kv_shadow_pos0(int64(window), pos0w)
        var pch = KvQShadowArgs(kvd = uint(kvd), pos0 = uint(shadow_pos0), rows = uint(kvlen - shadow_pos0), layerbase = mirb, ring = uint(g_rd.layers[l].ring))
        let shwg = (rd_kv_row_units(kvd) * (kvlen - shadow_pos0) + int64(WG_X) - 1l) / int64(WG_X)
        if (kvtq4) {
            enc_kv_tq4_shadow_cls(raw, h, g_rd.pf_kv_shadow_sets[g_rd.layers[l].mir_slab], pch, shwg)
        } else {
            enc_kv_q8_shadow_cls(raw, h, g_rd.pf_kv_shadow_sets[g_rd.layers[l].mir_slab], pch, shwg)
        }
    }
    pfq_ts(raw)
    let fa_lands_f16 = pf_fa_lands_f16(fo6)
    for (sg in g_pf_wsegs) {
        let kv_base = kvq ? 0u : uint(pf_seg_mir_base(g_rd.layers[l], sg))   // the rows the attention reads: the mirror's, or a block codec's shadow of this layer from 0
        let kv_ring = kvq ? 0u : uint(g_rd.layers[l].ring)   //! the shadow holds its rows at their positions
        if (g_rd.layers[l].pf_fa || g_rd.layers[l].pf_fakhr) {
            let khr = g_rd.layers[l].pf_fakhr
            var pcf = FaArgs(rows = uint(sg.npos), w0 = uint(sg.pos0), qd = uint(qd),
                kvd = uint(kvd), kv_mul = kv_mul, kvbase = kv_base,
                scale = g_rd.scale, qstride = uint(qrow), qhs = uint(qhs), window = window, cap = g_rd.layers[l].attn_cap,
                sinkoff = uint(l * g_rd.n_heads), row0 = uint(sg.row0), ring = kv_ring)
            fa_stamp_enc(khr, hs, hs == 256l && rd_attn_gated(), pcf.cap != 0.0, g_rd.has_sinks, fa_lands_f16, raw, h, g_rd.pf_sets[b + 5], pcf,
                fa_stamp_wgs(khr, g_rd.n_heads, sg.npos))
        } else {
            let attnwg = fa128 ? g_rd.n_heads * ((sg.npos + 31l) / 32l) : at_attn_tile_wgs(g_rd.n_heads, sg.npos, wide ? 4l : 8l)
            var pca = DaAttnBArgs(npos = uint(sg.npos), nh = uint(g_rd.n_heads), kvd = uint(kvd),
                hs = uint(hs), kv_mul = kv_mul, kbase = kv_base, vbase = kv_base,
                w0 = uint(sg.pos0), scale = g_rd.scale, qhs = uint(qhs), gated = rd_attn_gated() ? 1u : 0u, window = window,
                cap = g_rd.layers[l].attn_cap, row0 = uint(sg.row0), ring = kv_ring)
            da_attn_b_stamp_enc(fa128, wide, rd_pf_reads_f16(), raw, h, g_rd.pf_sets[b + 5], pca, attnwg)
        }
    }
    if (kvtq4) {
        rd_enc_tq4_unrot(raw, h, g_rd.pf_unrot_set, hs, wlen, uint(qd), uint(qrow), uint(qhs))
    }
    pfq_ts(raw)
    let attn_landed_f16 = fa_lands_f16 && (g_rd.layers[l].pf_fa || g_rd.layers[l].pf_fakhr)
    if (!(fo6 && attn_landed_f16)) {
        pf_feed_enc(raw, h, g_rd.pf_sets[b + 6], fo6, fok, 0l, wlen * qd)
    }
    pfq_ts(raw)
    let po = arena_planes(g_rd.layers[l].fo, g_rd.layers[l].bo)
    let af = pf_feed_planes(fo6, g_rd.pf_af, g_rd.pf_aq, g_rd.pf_as, qd)
    pf_gemm_enc(raw, h, b + 7, g_rd.layers[l].fo, po.wq, po.ws, af.q, af.s, g_rd.pf_xb2,
        po.wqb, po.wsb, af.qn, af.sn, np * dim * 4l, VHZ_AQ, VHZ_XB2, qd, dim, g_rd.layers[l].bo, wlen, fo6)
}

//! a dense FFN, or a MoE layer's shared expert, over the rows from `ffn_row0`
def private pf_ffn_block(raw : VkCommandBuffer; var h : VkHaz; l : int64; b : int; wlen, ffn_row0 : int64; gu6, dn6, dnk : bool) {
    let np = g_rd.pf_np
    let dim = g_rd.dim
    let hid = g_rd.layers[l].hid
    let ffn_rows = wlen - ffn_row0
    let p1 = arena_planes(g_rd.layers[l].f1, g_rd.layers[l].b1)
    let p3 = arena_planes(g_rd.layers[l].f3, g_rd.layers[l].b3)
    let gf = pf_feed_planes(gu6, g_rd.pf_xf, g_rd.pf_xq, g_rd.pf_xs, dim)
    let tgu = cm2_tiles(hid, ffn_rows)
    pf_gemm_enc(raw, h, b + 10, g_rd.layers[l].f1, p1.wq, p1.ws, gf.q, gf.s, g_rd.pf_gate,
        p1.wqb, p1.wsb, gf.qn, gf.sn, np * hid * 4l, VHZ_XQ, VHZ_GATE, dim, hid, g_rd.layers[l].b1, ffn_rows, gu6, tgu, ffn_row0)
    pf_gemm_enc(raw, h, b + 11, g_rd.layers[l].f3, p3.wq, p3.ws, gf.q, gf.s, g_rd.pf_up,
        p3.wqb, p3.wsb, gf.qn, gf.sn, np * hid * 4l, VHZ_XQ, VHZ_UP, dim, hid, g_rd.layers[l].b3, ffn_rows, gu6, tgu, ffn_row0)
    let act = g_rd.layers[l].gelu ? ACT_GELU : ACT_SILU
    if (dn6) {
        var pc12 = ActArgs(nelem = uint(ffn_rows * hid), act = act, nblk = uint(ffn_rows * hid / 32l), elem0 = uint(ffn_row0 * hid))
        enc_actf16_cls(raw, h, g_rd.pf_sets[b + 12], pc12, (ffn_rows * hid / 4l + 255l) / 256l)
    } else {
        var pc12 = ActArgs(nelem = uint(wlen * hid), act = act)
        actrq_enc(raw, h, g_rd.pf_sets[b + 12], dnk, pc12, wlen * hid)
    }
    pfq_ts(raw)
    let p2 = arena_planes(g_rd.layers[l].f2, g_rd.layers[l].b2)
    let hf = pf_feed_planes(dn6, g_rd.pf_hf, g_gpu.hq_dev, g_gpu.hs_dev, hid)
    pf_gemm_enc(raw, h, b + 13, g_rd.layers[l].f2, p2.wq, p2.ws, hf.q, hf.s, g_rd.pf_ffnout,
        p2.wqb, p2.wsb, hf.qn, hf.sn, np * dim * 4l, VHZ_HQ, VHZ_FFO, hid, dim, g_rd.layers[l].b2, ffn_rows, dn6, 0l, ffn_row0)
}

[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain"), arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm"), arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#logits-transfer-queue"), arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain")]
def private pf_run(x_batch : array<float>; ids : array<int64>; use_ids : bool; emb_scale : float;
                   cos_batch : array<float>; ple_rows : array<float>; ple_raw : bool; npos, pos0 : int64; var logits : array<float>;
                   experts : RdecHostExpertsRowsFn; segs : array<RdecPfSeg>) {
    assert(g_rd != null && g_rd.ready, "vk prefill before prepare/set_cls")
    g_pf_segs |> clear()
    if (empty(segs)) {
        g_pf_segs |> push(RdecPfSeg(row0 = 0l, npos = npos, pos0 = pos0, region = g_rd.region))
    } else {
        g_pf_segs |> push_clone_from(segs)
    }
    let nseg = long_length(g_pf_segs)
    let hidden = g_pf_hidden_ask   //! an embedding's call: every row's final residual lands, and no classifier runs
    assert(hidden || (nseg <= g_rd.regions && long_length(logits) >= nseg * g_rd.vocab && (!g_rd.carry_live || nseg <= g_rd.nb)),
        "vk prefill: the segments outnumber the regions or the carry's rows, or the logits are short of a slot a segment")
    let ple_tbl = use_ids && ple_raw   //! the side rows gather on device from the token table
    assert(g_rd.ple == 0l || ple_tbl || long_length(ple_rows) >= npos * g_rd.n_layers * g_rd.ple, "vk prefill: the per-layer-embedding rows are short of the prompt")
    pf_setup()
    rd_carry_sync()
    if (g_rd.hybrid) {
        pf_dn_begin()
    }
    let dim = g_rd.dim
    let nlfin = rd_norm_final()
    let cos_elems = g_rd.cos_elems
    g_rd.pf_head_rows = 0l
    let warm = nseg == 1l && g_rd.pf_head_ask && g_rd.head_on && g_rd.carry_live && (g_rd.pf_head_ready || (!g_rd.pf_head_declined && pf_head_setup()))   //! a segmented window warms no head row
    if (hidden) {
        if (g_rd.pf_hid_host.buf == 0ul) {
            g_rd.pf_hid_host = make_host_buf(g_rd.pf_np * dim * 4l, false, [cached = true])
        }
        g_pf_hidden |> reserve(npos * dim)
        g_pf_hidden |> resize(npos * dim)
    }
    unsafe {
        var w0 = 0l
        var last_wlen = 0l
        while (w0 < npos) {
            let tp0 = ref_time_ticks()
            let wlen = pf_window_len(npos, w0)
            let last = w0 + wlen >= npos
            pf_window_segs(w0, wlen)
            pf_window_caps(w0)
            let nwseg = long_length(g_pf_wsegs)
            if (nwseg > 1l) {
                g_rd.pf_seg_windows++
            }
            if (warm) {
                pf_warm_cos(cos_batch, w0, wlen)
            }
            if (use_ids) {
                g_pf_ids_stage |> resize(wlen)
                for (i in range64(wlen)) {
                    let id = ids[w0 + i]
                    if (id < 0l || id >= g_rd.vocab) {   // fail closed loudly, as the CPU's dequant_q8_row does on the same input
                        panic("vk prefill: token id {id} outside the vocab ({g_rd.vocab}) at row {w0 + i}")
                    }
                    g_pf_ids_stage[int(i)] = uint(id)
                }
                upload_region_at(g_rd.pf_ids, 0l, addr<uint8 const?>(g_pf_ids_stage[0]), wlen * 4l)
            } else {
                upload_region_at(g_rd.pf_x, 0l, addr<uint8 const?>(x_batch[int(w0 * dim)]), wlen * dim * 4l)
            }
            upload_region_at(g_rd.pf_cos, 0l, addr<uint8 const?>(cos_batch[int(w0 * cos_elems)]), wlen * cos_elems * 4l)
            if (g_rd.ple > 0l && !ple_tbl) {   //! this window's side-input rows, position-major [wlen x (layers x ple)]
                let all = g_rd.n_layers * g_rd.ple
                upload_region_at(g_rd.pf_ple, 0l, addr<uint8 const?>(ple_rows[int(w0 * all)]), wlen * all * 4l)
            }
            let bp = pf_prologue_block()
            let t_prep = get_time_usec(tp0)
            // record
            var raw = g_rd.pf_cmd
            var h = cmd_open(raw, true)
            pfq_begin(raw)
            pfq_ts(raw)   // q0: submit start
            if (g_rd.hybrid) {
                pf_dn_zero_fresh(raw, h)
            }
            pfq_ts(raw)   // q1: kept for the role-aggregation stamp shape (params ride push now)
            if (use_ids) {
                // no extra pfq_ts - the gather cost folds into the prologue stamp
                rd_emb_enc(raw, h, g_rd.pf_emb_set, wlen, emb_scale)
                if (ple_tbl) {   //! the side rows off the token table, sqrt(ple)-scaled as the host gather leaves them
                    emb_cls_enc(g_rd.ple_tbl_fmt, raw, h, g_rd.pf_ple_tbl_set, wlen, g_rd.n_layers * g_rd.ple, arena_local_blk(g_rd.ple_tbl_blk), sqrt(float(g_rd.ple)))
                }
            }
            if (ple_raw) {   //! the per-layer-embedding pre-step's finish: the projection of the embedded rows under 1/sqrt(dim), normed per slice and averaged into the gathered rows
                let all = g_rd.n_layers * g_rd.ple
                if (g_rd.ple_w16_dev != 0ul) {   //! the window's f16 panel of the projection, then the rows as f16 under the 1/sqrt(dim)
                    var pcw = gk_f16_rows_args(uint(all * dim))
                    if (g_rd.ple_wp_bf16) {
                        enc_ple_bf16_cvt_cls(raw, h, g_rd.pf_ple_wcvt_set, pcw, (all * dim / 4l + 255l) / 256l)
                    } else {
                        enc_tower_clamp_cvt_cls(raw, h, g_rd.pf_ple_wcvt_set, pcw, (all * dim / 4l + 255l) / 256l)
                    }
                    var pcc = gk_f16_rows_args(uint(wlen * dim), 1.0 / sqrt(float(dim)))
                    enc_tower_clamp_cvt_cls(raw, h, g_rd.pf_ple_cvt_set, pcc, (wlen * dim / 4l + 255l) / 256l)
                    var pcg = F16GemmArgs(n = uint(dim), d = uint(all), rows = uint(wlen), woff = 0u, ybase = 0u, ystride = uint(all), ksplit = 0u)
                    let ple_khr = g_gpu.coopmat_mode != COOPMAT_CM2
                    f16_gemm_enc(ple_khr, raw, h, g_rd.pf_ple_gemm_set, pcg, wlen, all)
                } else {
                    ple_proj_enc(raw, h, g_rd.pf_ple_gemm_set, uint(wlen))
                }
                var pcf = GkPleFinArgs(ple = uint(g_rd.ple), eps = g_rd.eps)
                enc_ple_finish_cls(raw, h, g_rd.pf_ple_ar_set, pcf, wlen * g_rd.n_layers)
            }
            if (warm) {
                vhz_dep(raw, h, VHZ_X, VHZ_UP, true)
                cmd_copy_range(raw, g_rd.pf_x, 0l, g_rd.head_pf_wemb, 0l, wlen * dim * 4l)
            }
            var pc_pro = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = 0u, eps = g_rd.eps, ascale = 1.0)
            enc_cls_ar(raw, h, g_rd.pf_sets[bp + 0], pc_pro, wlen)   // prologue norm
            pfq_ts(raw)
            let overlap = g_env_vulkan.vk_overlap && !vk_prof() && !g_rd.experts_host
            let fuse_arq = g_env_vulkan.vk_fuse
            var nextb = 1l
            var cstep = 1l
            var nchunk = 0
            for (l in range64(g_rd.n_layers)) {
                if (overlap && l == nextb) {
                    cmd_close(raw)
                    submit_nofence(raw)
                    assert(nchunk < length(g_rd.pf_cmd_ring), "vk prefill: overlap ring exhausted")
                    raw = g_rd.pf_cmd_ring[nchunk]
                    nchunk++
                    cmd_open(raw, true)
                    cstep = min(cstep * 2l, pf_chunk_cap())
                    nextb += cstep
                }
                let b = int(l * PF_ROLES)
                let np = g_rd.pf_np
                let gu6 = pf_gu6(l)
                let dn6 = pf_dn6(l)
                let guk = kq_sb(g_rd.layers[l].f1) && !gu6
                let dnk = kq_sb(g_rd.layers[l].f2) && !dn6
                if (g_rd.layers[l].recurrent) {
                    rd_pf_recurrent(raw, h, l, b, wlen)
                } else {
                pf_attn_block(raw, h, l, b, wlen, pos0 + w0, l > 0l && pf_qkv_feed_fused(l))
                }
                let pre_on = g_rd.layers[l].pre_post ? 1u : 0u
                var pc8 = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = uint(rd_norm_off(l, RD_NORM_FFN)), eps = g_rd.eps, ascale = 1.0,
                    pre_on = pre_on, pwoff = uint(rd_norm_off(l, RD_NORM_POST_ATT)),
                    abias_on = g_rd.has_obias ? 1u : 0u, aboff = uint(rd_norm_off(l, RD_NORM_OBIAS)))
                let moe = g_rd.layers[l].moe   //! the routed block reads the normed rows the fused twin never stores
                if (fuse_arq && !guk && !moe) {
                    if (gu6) {
                        enc_cls_ar_f16_b(raw, h, g_rd.pf_arq_sets[int(l * 2l)], pc8, wlen)
                    } else {
                        enc_cls_ar_rq_b(raw, h, g_rd.pf_arq_sets[int(l * 2l)], pc8, wlen)
                    }
                    pfq_ts(raw)
                } else {
                    enc_cls_ar(raw, h, g_rd.pf_sets[b + 8], pc8, wlen)
                    pfq_ts(raw)
                    let feed_read = !moe || has_dense_triple(g_rd.layers[l])   //! the routed block takes the f32 rows; only a dense triple reads the feed
                    if (feed_read) {
                        pf_feed_enc(raw, h, g_rd.pf_sets[b + 9], gu6, guk, 0l, wlen * dim)
                    }
                }
                pfq_ts(raw)
                let ffn_row0 = warm || nwseg > 1l || hidden ? 0l : pf_ffn_slice_row0(l, g_rd.n_layers, wlen, gu6 && !moe, dn6)
                let ffn_rows = wlen - ffn_row0
                if (has_dense_triple(g_rd.layers[l])) {
                    pf_ffn_block(raw, h, l, b, wlen, ffn_row0, gu6, dn6, dnk)
                }
                if (moe && g_rd.layers[l].experts_host) {
                    pf_moe_router(raw, h, l, wlen)
                    if (g_rd.layers[l].moe_g4) {
                        pf_moe_g4_feed(raw, h, l, wlen)
                    }
                    pf_host_cut(raw, raw, h, l, wlen, 0l, experts)
                } elif (moe) {
                    pf_moe_block(raw, h, l, wlen)
                }
                let nxt = l + 1l < g_rd.n_layers ? rd_norm_off(l + 1l, RD_NORM_ATT) : nlfin
                let lple = g_rd.layers[l].ple
                if (lple) {
                    // the FFN residual first (no scale, the normed rows unused); the branch leaves its projection in pf_ffnout for the step below
                    var pc14a = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = uint(nxt), eps = g_rd.eps, ascale = 1.0, row0 = uint(ffn_row0),
                        pre_on = pre_on, pwoff = uint(rd_norm_off(l, RD_NORM_POST_FFN)))
                    enc_cls_ar(raw, h, g_rd.pf_sets[b + 14], pc14a, ffn_rows)
                    pf_ple_block(raw, h, l, b, wlen, np)
                }
                var pc14 = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = uint(nxt), eps = g_rd.eps, ascale = g_rd.layers[l].out_scale, row0 = uint(ffn_row0),
                    pre_on = (pre_on != 0u || lple) ? 1u : 0u, pwoff = uint(rd_norm_off(l, lple ? RD_NORM_PLE_POST : RD_NORM_POST_FFN)))
                let next_qkv6 = l + 1l < g_rd.n_layers && pf_qkv6(l + 1l)
                if (moe) {
                    pf_moe_addr_next(raw, h, l, wlen, nxt)
                } elif (l + 1l < g_rd.n_layers && pf_qkv_feed_fused(l + 1l)) {
                    if (next_qkv6) {
                        enc_cls_ar_f16_b(raw, h, g_rd.pf_arq_sets[int(l * 2l + 1l)], pc14, wlen)
                    } else {
                        enc_cls_ar_rq_b(raw, h, g_rd.pf_arq_sets[int(l * 2l + 1l)], pc14, wlen)
                    }
                } else {
                    enc_cls_ar(raw, h, g_rd.pf_sets[b + 14], pc14, ffn_rows)
                }
                pfq_ts(raw)
            }
            if (hidden) {   //! every row's final residual, before the classifier's requant reads the normed rows
                vhz_dep(raw, h, VHZ_X, 0u, true)
                cmd_copy_range(raw, g_rd.pf_x, 0l, g_rd.pf_hid_host.buf, 0l, wlen * dim * 4l)
            }
            var ends = 0l
            for (ws in g_pf_wsegs) {
                break if (hidden)
                let row = ws.row0 + ws.npos - 1l
                let slot = pf_seg_ending_at(w0, row)
                continue if (slot < 0l)
                ends++
                rq_enc(raw, h, g_rd.pf_sets[bp + 1], kq_sb(g_rd.cls_fmt_marker), row * dim, dim)
                if (ends == 1l) {
                    pfq_ts(raw)   //! the requant's stamp: the first segment's; the rest bill to the classifier's
                }
                pf_cls_enc(raw, h)   // the epilogue bills to the classifier's stamp
                vhz_dep(raw, h, VHZ_LOG, 0u, true)
                cmd_copy_range(raw, g_rd.logits_dev, 0l, g_rd.pf_logits_host.buf, slot * g_rd.vocab * 4l, g_rd.vocab * 4l)
                if (g_rd.carry_live) {   //! a segment's carry lands at its slot's row
                    vhz_dep(raw, h, VHZ_XB, 0u, true)
                    cmd_copy_range(raw, g_rd.pf_xb, row * dim * 4l, g_rd.hid_host.buf, slot * dim * 4l, dim * 4l)
                }
            }
            if (ends > 0l) {
                pfq_ts(raw)
            } elif (hidden) {   //! the requant's and the classifier's stamps land empty, so the profile's roles stay in place
                pfq_ts(raw)
                pfq_ts(raw)
            }
            assert(hidden || !last || ends > 0l, "vk prefill: the last window ends no segment")
            if (warm) {
                pf_head_warm(raw, h, w0, wlen, pos0)
            }
            if (g_rd.hybrid) {
                pf_dn_window_done()
            }
            if (vk_prof()) {
                to_log(LOG_INFO, "vk_rdpf hz: {h.disp} nodes, {h.barriers} barriers\n")   // nolint:PERF026,PERF028 — vk_prof()-gated report leg
                if (g_rd.moe && g_rd.pf_midx != 0ul) {   //! no picks buffer where every routed layer's experts serve on the host: the MoE block never set up
                    vhz_dep(raw, h, VHZ_MRT, 0u, true)
                    cmd_copy_whole(raw, g_rd.pf_midx, g_rd.pf_midx_host.buf, wlen * g_rd.k * 4l)
                }
            }
            pfq_end(raw)
            cmd_close(raw)
            let t_record = get_time_usec(tp0) - t_prep
            submit_wait(raw)
            if (hidden) {
                host_buf_read(g_rd.pf_hid_host, addr<void?>(g_pf_hidden[w0 * dim]), wlen * dim * 4l)
            }
            if (vk_prof()) {
                to_log(LOG_INFO, "vk_rdpf w0 {w0} rows {wlen} prep {t_prep} record {t_record} submit {get_time_usec(tp0) - t_prep - t_record}\n")   // nolint:PERF026,PERF028 — vk_prof()-gated report leg
            }
            w0 += wlen
            last_wlen = wlen
        }
        if (g_rd.hybrid) {
            pf_dn_end()
            pf_caps_done()
        }
        if (!hidden) {
            host_buf_read(g_rd.pf_logits_host, addr<void?>(logits[0]), nseg * g_rd.vocab * 4l)
        }
        // per-role GPU aggregation, LAST window, only when every stamp landed; overlapped levels (q|k|v, gate|up) bill the level on the last member's delta
        if (vk_prof()) {
            pf_prof_report(npos)
            if (g_rd.moe && g_rd.pf_midx != 0ul) {
                pf_moe_bucket_report(last_wlen)
            }
        }
    }
}

//! the profile's bucket report: the last MoE layer's picks of the last window as per-expert row
//! counts - the shape the expert tiles run over - with the 32-row tile count the s column takes
//! and the count a 32 / 64 / 128 column ladder would take
[cold_path]
def private pf_moe_bucket_report(wlen : int64) {
    let nk = wlen * g_rd.k
    var cnt : array<int64>
    cnt |> reserve(g_rd.ne)
    cnt |> resize(g_rd.ne)
    unsafe {
        let picks = reinterpret<uint?>(g_rd.pf_midx_host.mapped)
        for (i in range64(nk)) {
            let e = int64(picks[i])
            if (e < g_rd.ne) {
                cnt[e]++
            }
        }
    }
    var used = 0l
    var largest = 0l
    var b32 = 0l
    var b64 = 0l
    var b128 = 0l
    var b_over = 0l
    var tiles_s = 0l
    var tiles_ladder = 0l
    for (c in cnt) {
        continue if (c == 0l)
        used++
        largest = max(largest, c)
        tiles_s += (c + 31l) / 32l
        if (c <= 32l) {
            b32++
            tiles_ladder++
        } elif (c <= 64l) {
            b64++
            tiles_ladder++
        } else {
            if (c <= 128l) {
                b128++
            } else {
                b_over++
            }
            tiles_ladder += (c + 127l) / 128l
        }
    }
    to_log(LOG_INFO, "vk_rdpf moe buckets (the last MoE layer, {nk} rows over {g_rd.ne} experts): {used} non-empty, the largest {largest}; sizes <=32: {b32} 33-64: {b64} 65-128: {b128} >128: {b_over}; 32-row tiles {tiles_s}, a 32/64/128 ladder {tiles_ladder}\n")
    delete cnt
}

def private add_stamps(var acc : auto; qi : uint; n : int64) {
    for (s in range64(n)) {
        acc[s] += pfq_us(qi + uint(s))
    }
}

def private stamp_sum(v : auto) : double {
    var t = 0.0lf
    for (x in v) {
        t += x
    }
    return t
}

[cold_path]   // DASLLAMA_GPU_PROF=1 report leg
def private pf_prof_report(npos : int64) {
    var want = 5u
    for (L in g_rd.layers) {
        want += uint(pf_roles_per_layer(L))
    }
    if (g_pfq_n != want) {
        to_log(LOG_INFO, "vk_rdpf gpu: roles unaggregated ({g_pfq_n} stamps, the table expects {want})\n")
        return
    }
    var att : double[20]
    var dn : double[18]
    var moe : double[26]
    var dnm : double[26]   //! a recurrent MoE layer: the deltanet head's stamps, then the routed tail's
    var natt = 0l
    var ndn = 0l
    var nmoe = 0l
    var ndnm = 0l
    var nhost = 0l
    var qi = 3u
    for (L in g_rd.layers) {
        let n = pf_roles_per_layer(L)
        if (L.moe && L.experts_host) {   // the router alone of the routed block: its stamps would misalign the device layers' roles
            nhost++
            qi += uint(n)
            continue
        }
        if (L.recurrent && L.moe) {
            add_stamps(dnm, qi, n)
            ndnm++
        } elif (L.recurrent) {
            add_stamps(dn, qi, n)
            ndn++
        } elif (L.moe) {
            add_stamps(moe, qi, n)
            nmoe++
        } else {
            add_stamps(att, qi, n)
            natt++
        }
        qi += uint(n)
    }
    let tatt = stamp_sum(att)
    let tdn = stamp_sum(dn)
    let tmoe = stamp_sum(moe)
    let tdnm = stamp_sum(dnm)
    let tot = pfq_us(2u) + pfq_us(qi) + pfq_us(qi + 1u) + (tatt + tdn + tmoe + tdnm)
    to_log(LOG_INFO, "vk_rdpf gpu ({npos} tokens, last window) prologue {int(pfq_us(2u))} fin_rq {int(pfq_us(qi))} cls {int(pfq_us(qi + 1u))} attention layers {natt}: {int(tatt)} recurrent layers {ndn}: {int(tdn)} MoE layers {nmoe}: {int(tmoe)} recurrent MoE layers {ndnm}: {int(tdnm)} total {int(tot)} us\n")
    var has_shexp = false
    var has_g4 = false
    for (L in g_rd.layers) {
        has_shexp ||= L.moe && has_dense_triple(L)
        has_g4 ||= L.moe_g4
    }
    if (nhost > 0l) {
        to_log(LOG_INFO, "vk_rdpf gpu: {nhost} host layers left out of the role table (the router alone, the experts summed on the host between the submits)\n")
    }
    if (nmoe > 0l) {
        //! an MoE layer's stamps: the attention roles (the qkn one shifts them by one), ar1, rq_f, then the tail
        let q = g_rd.qk_norm ? 1 : 0
        let qkn = q > 0 ? "qkn {int(moe[4])} " : ""
        to_log(LOG_INFO, "vk_rdpf moe rq_x {int(moe[0])} q {int(moe[1])} k {int(moe[2])} v {int(moe[3])} {qkn}rope_kv {int(moe[4 + q])} attn {int(moe[5 + q])} rq_a {int(moe[6 + q])} wo {int(moe[7 + q])} ar1 {int(moe[8 + q])} rq_f {int(moe[9 + q])} {pf_moe_tail_text(moe, 10 + q, has_shexp, has_g4)}\n")
    }
    if (ndnm > 0l) {
        //! a recurrent MoE layer's stamps: the deltanet head's (the f32 beta/alpha arm folds two into one), ar1, rq_f, then the tail
        let b = g_rd.dn_ba_f32 ? 0 : 1
        let ba = g_rd.dn_ba_f32 ? "ba {int(dnm[3])}" : "beta {int(dnm[3])} alpha {int(dnm[4])}"
        to_log(LOG_INFO, "vk_rdpf dnmoe rq_x {int(dnm[0])} qkv {int(dnm[1])} z {int(dnm[2])} {ba} conv {int(dnm[4 + b])} scan {int(dnm[5 + b])} onorm {int(dnm[6 + b])} tail {int(dnm[7 + b])} rq_o {int(dnm[8 + b])} out {int(dnm[9 + b])} ar1 {int(dnm[10 + b])} rq_f {int(dnm[11 + b])} {pf_moe_tail_text(dnm, 12 + b, has_shexp, has_g4)}\n")
    }
    if (natt > 0l) {
        // the qkn dispatch stamps between v and rope_kv, shifting every later role by one; the per-layer-embedding branch's four sit between down and ar2
        let q = g_rd.qk_norm ? 1 : 0
        let qkn = q > 0 ? "qkn {int(att[4])} " : ""
        let ple = g_rd.ple > 0l ? "ple_rq {int(att[14 + q])} ple_gate {int(att[15 + q])} ple_act {int(att[16 + q])} ple_proj {int(att[17 + q])} " : ""
        let p = g_rd.ple > 0l ? 4 : 0
        to_log(LOG_INFO, "vk_rdpf attn rq_x {int(att[0])} q {int(att[1])} k {int(att[2])} v {int(att[3])} {qkn}rope_kv {int(att[4 + q])} attn {int(att[5 + q])} rq_a {int(att[6 + q])} wo {int(att[7 + q])} ar1 {int(att[8 + q])} rq_f {int(att[9 + q])} gate {int(att[10 + q])} up {int(att[11 + q])} act {int(att[12 + q])} down {int(att[13 + q])} {ple}ar2 {int(att[14 + q + p])}\n")
    }
    if (ndn > 0l) {
        let ba_stamp_shift = g_rd.dn_ba_f32 ? 0l : 1l
        let ba = g_rd.dn_ba_f32 ? "ba {int(dn[3])}" : "beta {int(dn[3])} alpha {int(dn[4])}"
        to_log(LOG_INFO, "vk_rdpf dn rq_x {int(dn[0])} qkv {int(dn[1])} z {int(dn[2])} {ba} conv {int(dn[4 + ba_stamp_shift])} scan {int(dn[5 + ba_stamp_shift])} onorm {int(dn[6 + ba_stamp_shift])} tail {int(dn[7 + ba_stamp_shift])} rq_o {int(dn[8 + ba_stamp_shift])} out {int(dn[9 + ba_stamp_shift])} ar1 {int(dn[10 + ba_stamp_shift])} rq_f {int(dn[11 + ba_stamp_shift])} gate {int(dn[12 + ba_stamp_shift])} up {int(dn[13 + ba_stamp_shift])} act {int(dn[14 + ba_stamp_shift])} down {int(dn[15 + ba_stamp_shift])} ar2 {int(dn[16 + ba_stamp_shift])}\n")
    }
}

// The arena FFN chain's scratch + the slab-independent act set. The three GEMV sets build lazily
// per SLAB in vk_arena_ffn (bindings 0/1 are the blk's slab planes; `af_role_set` ensures each role's
// class); each GEMV role carries its OWN regions buffer (the weight base pair).
def private ensure_af_state(f2 : int) {
    if (g_gpu.af_ready) {
        return
    }
    ensure_ffn_state()
    let dkq = !kq_block32(f2)
    verify(dkq ? ensure_q8k_actrq_cls() : ensure_q8_actrq_cls(), "vulkan arena: the act class rail must engage on a live device")
    g_gpu.af_meta_g = make_host_buf(META_BYTES, true)
    g_gpu.af_meta_u = make_host_buf(META_BYTES, true)
    g_gpu.af_meta_d = make_host_buf(META_BYTES, true)
    g_gpu.af_xq = make_host_buf(XQ_BYTES, true)
    g_gpu.af_xs = make_host_buf(XS_BYTES, true)
    g_gpu.af_ya_dev = make_device_buf(Y_BYTES)
    g_gpu.af_yb_dev = make_device_buf(Y_BYTES)
    g_gpu.af_yo_dev = make_device_buf(Y_BYTES)
    g_gpu.af_yo_host = make_host_buf(Y_BYTES, false, [cached = true])
    // the requant form follows the DOWN plane; class bindings: gate@0, up@1, hq@2, hs@3
    let abufs = fixed_array(g_gpu.af_ya_dev, g_gpu.af_yb_dev, g_gpu.hq_dev, g_gpu.hs_dev)
    let asizes = fixed_array(Y_BYTES, Y_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES)
    let agbits = fixed_array(VHG_Y1, VHG_Y2, VHG_HQ, VHG_HQ)
    g_gpu.af_actrq_set = set_actrq_cls(abufs, asizes, agbits)   // one family set serves both forms
    g_gpu.af_ready = true
}

// one af GEMV set per (role cache, blk's slab) — the role fixes meta/activation/output bindings.
// xbit: the activation pair's hazard bits (0 for host-written xq/xs, VHG_HQ for the actrq-fed
// down chain — losing it would let the down GEMV race the act+requant write).
def private af_role_set(var sets : table<int; VkDescriptorSet>; fmt : int; blk : int64;
                        mbuf, xqbuf, xsbuf, ybuf : uint64; xqbytes, xsbytes : int64; xbit, ybit : uint) : VkDescriptorSet {
    return cached(sets, arena_slab_idx(blk)) $ {
        verify(gemv_cls_ensure(fmt), "vulkan arena: the gemv class rail must engage on a live device")
        return gemv_planes_set(fmt, arena_planes(fmt, blk), mbuf, META_BYTES, 0u, xqbuf, xsbuf, xqbytes, xsbytes, xbit, ybuf, Y_BYTES, ybit)
    }
}

//! Arena seam: one dense FFN layer against arena-resident planes — gate GEMV, up GEMV, fused
//! act+requant, down GEMV, ONE submit. `blk1/blk3/blk2` are gate/up/down arena block indices.
//! Gate and up must share a format class (they share the activation image); down is free.
def vk_arena_ffn(var y : array<float>; blk1, blk3, blk2 : int64; n, nfe : int64;
                 f1, f3, f2 : int; xq : array<int8>; xs : array<float>; is_gelu : bool) {
    verify(vk_moe_init())
    verify(kq_block32(f3) == kq_block32(f1), "dasLLAMA vulkan arena: gate/up form mismatch - one activation image")
    assert(nfe * 4l <= Y_BYTES && n * 4l <= Y_BYTES, "dasLLAMA vulkan arena: ffn planes exceed y capacity")
    ensure_af_state(f2)
    let xunit = kq_unit(f1)
    let dkq = !kq_block32(f2)
    var gate_set = af_role_set(g_gpu.af_gate_sets, f1, blk1, g_gpu.af_meta_g.buf,
        g_gpu.af_xq.buf, g_gpu.af_xs.buf, g_gpu.af_ya_dev, XQ_BYTES, XS_BYTES, 0u, VHG_Y1)
    var up_set = af_role_set(g_gpu.af_up_sets, f3, blk3, g_gpu.af_meta_u.buf,
        g_gpu.af_xq.buf, g_gpu.af_xs.buf, g_gpu.af_yb_dev, XQ_BYTES, XS_BYTES, 0u, VHG_Y2)
    var down_set = af_role_set(g_gpu.af_down_sets, f2, blk2, g_gpu.af_meta_d.buf,
        g_gpu.hq_dev, g_gpu.hs_dev, g_gpu.af_yo_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHG_HQ, VHG_YO)
    unsafe {
        // per-role regions: one (weight block, xblock) pair each
        fill_gemv_regions(g_gpu.af_meta_g, blk1)
        fill_gemv_regions(g_gpu.af_meta_u, blk3)
        fill_gemv_regions(g_gpu.af_meta_d, blk2)
        memcpy(g_gpu.af_xq.mapped, addr<void?>(xq[0]), n)
        memcpy(g_gpu.af_xs.mapped, addr<void?>(xs[0]), (n / xunit) * 4l)
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            var pcg = GemvArgs(n = uint(n), d = uint(nfe), nreg = 1u, ybase = 0u)
            gemv_enc(f1, raw, h, gate_set, pcg, nfe)
            gemv_enc(f3, raw, h, up_set, pcg, nfe)
            // the requant form follows the DOWN plane - its GEMV consumes hq/hs
            var pca = ActArgs(nelem = uint(nfe), act = is_gelu ? ACT_GELU : ACT_SILU)
            actrq_enc(raw, h, g_gpu.af_actrq_set, dkq, pca, nfe)
            var pcd = GemvArgs(n = uint(nfe), d = uint(n), nreg = 1u, ybase = 0u)
            gemv_enc(f2, raw, h, down_set, pcd, n)
            vhz_dep(raw, h, VHG_YO, 0u, true)
            cmd_copy_whole(raw, g_gpu.af_yo_dev, g_gpu.af_yo_host.buf, n * 4l)
        }
        memcpy(addr<void?>(y[0]), g_gpu.af_yo_host.mapped, n * 4l)
    }
}

// single-region batch schedule over an arena block: 4-word record at 0, all tiles map to region 0 at
// map_off = 4. Returns the dispatch workgroup count. `cm2l` must match the dispatched class's tile rule
// and `tc` its column where the joint pick chose one (0 = the whole pick's); `nsplit` stacks split-k planes; `row0` (word 1) never pairs with `nsplit > 1`
def private fill_arena_batch_sched(hb : HostBuf; d, blk, npos : int64; fmt : int; cm2l : bool = false; nsplit : int64 = 1l; row0 : int64 = 0l; tc : int64 = 0l) : uint {
    let te = batch_tile_edges(fmt, d, npos, cm2l)
    let wtiles = (d + te.td - 1l) / te.td
    let rtiles = (npos + (tc > 0l ? tc : te.tc) - 1l) / (tc > 0l ? tc : te.tc)
    let total = rtiles * wtiles * nsplit
    assert((4l + total) * 4l <= BATCH_META_BYTES, "dasLLAMA vulkan arena: batch schedule exceeds capacity")
    var cur = 4l
    batch_sched_map(unsafe(reinterpret<uint?>(hb.mapped)), cur, 0l, arena_local_blk(blk), npos, total, row0)
    return uint(total)
}

//! Arena seam: batch GEMM of the [d x n] tensor at block `blk` against `npos` quantized activation
//! rows (xq/xs, [npos x n]) into y[0..npos*d). The prefill twin of vk_arena_gemv (q8 only for now).
def vk_arena_gemm(var y : array<float>; blk : int64; n, d, npos : int64; fmt : int; xq : array<int8>; xs : array<float>) {
    verify(vk_moe_init())
    assert(fmt == int(KqFmt.q8), "dasLLAMA vulkan arena_gemm: q8 only")
    assert(npos * n <= BATCH_XQ_BYTES && npos * d * 4l <= BATCH_Y_BYTES, "arena_gemm: batch exceeds scratch")
    arena_ensure_batch(fmt)
    let v = q8_batch_cls_variant(q8_batch_tile_for(d, npos), q8_batch_aligned(d, npos))
    let skey = arena_slab_idx(blk) * 8 + v
    unsafe {
        var a & = g_gpu.arenas[fmt]
        var bset = cached(a.cls_batch_sets, skey) $ {   // dies with the arena on model drop
            verify(q8_batch_cls_ensure(v), "vulkan arena: the batch class rail must engage on a live device")
            let p = arena_planes(fmt, blk)
            return q8_batch_cls_set(v,
                fixed_array(p.wq, p.ws, a.batch_meta.buf, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, g_gpu.batch_y1_dev),
                fixed_array(p.wqb, p.wsb, BATCH_META_BYTES, BATCH_XQ_BYTES, BATCH_XS_BYTES, BATCH_Y_BYTES),
                fixed_array(0u, 0u, 0u, 0u, 0u, VHG_YO))
        }
        let groups = fill_arena_batch_sched(a.batch_meta, d, blk, npos, fmt)
        upload_region_at(g_gpu.batch_xq_dev, 0l, addr<uint8 const?>(xq[0]), npos * n)
        upload_region_at(g_gpu.batch_xs_dev, 0l, addr<uint8 const?>(xs[0]), npos * (n / 32l) * 4l)
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u)
            q8_batch_cls_enc(v, raw, h, bset, pc, int64(groups))
            vhz_dep(raw, h, VHG_YO, 0u, true)
            cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, npos * d * 4l)
        }
        host_buf_read(g_gpu.batch_y1, addr<void?>(y[0]), npos * d * 4l)
    }
}

//! Arena seam: one GEMV of the [d x n] tensor at block index `blk` into y[0..d). Same kernels as
//! the per-op tier — only the weight base is an arena index, not stack-relative.
def vk_arena_gemv(var y : array<float>; blk : int64; n, d : int64; fmt : int; xq : array<int8>; xs : array<float>) {
    assert(g_gpu != null && key_exists(g_gpu.arenas, fmt), "dasLLAMA vulkan arena: format not reserved")
    var yp = unsafe(addr(y[0]))
    let xqp = unsafe(addr(xq[0]))
    let xsp = unsafe(addr(xs[0]))
    arena_ensure_scratch(fmt)
    let unit = kq_unit(fmt)
    let nb = n / unit
    assert(n <= XQ_BYTES && nb * 4l <= XS_BYTES, "dasLLAMA vulkan arena: activations exceed scratch capacity")
    assert(d * 4l <= Y_BYTES, "dasLLAMA vulkan arena: gemv output exceeds y capacity")
    unsafe {
        var a & = g_gpu.arenas[fmt]
        var s_cls = af_role_set(a.cls_sets, fmt, blk, a.meta.buf, a.xq.buf, a.xs.buf, a.y_dev, XQ_BYTES, XS_BYTES, 0u, VHG_YO)   // the cached set dies with the arena on model drop and rebuilds over the new scratch
        var mp = reinterpret<uint?>(a.meta.mapped)
        mp[0] = uint(arena_local_blk(blk))   // one region: the whole tensor at its slab base
        mp[1] = 0u          // activation block base
        memcpy(a.xq.mapped, reinterpret<void?>(xqp), n)   // one int8 quant per weight
        memcpy(a.xs.mapped, reinterpret<void?>(xsp), nb * 4l)
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u)
            gemv_enc(fmt, raw, h, s_cls, pc, d)
            vhz_dep(raw, h, VHG_YO, 0u, true)
            cmd_copy_whole(raw, a.y_dev, a.y.buf, d * 4l)
        }
        memcpy(reinterpret<void?>(yp), a.y.mapped, d * 4l)
    }
}

// ===== prefill batch dispatch (the FFN's batched-GEMM arm) =====

// the fused act+requant class sets (both forms — the mid-step form follows the down stack)
def private ensure_batch_acts_cls {
    if (g_gpu.batch_acts_cls_ready) {
        return
    }
    verify(ensure_q8_actrq_cls() && ensure_q8k_actrq_cls(), "vk batch: the actrq class rails must engage on a live device")
    let abufs = fixed_array(g_gpu.batch_y1_dev, g_gpu.batch_y2_dev, g_gpu.hq_dev, g_gpu.hs_dev)
    let asizes = fixed_array(BATCH_Y_BYTES, BATCH_Y_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES)
    let agbits = fixed_array(VHB_Y1, VHB_Y2, VHB_HQ, VHB_HQ)
    g_gpu.batch_actrq_set = set_actrq_cls(abufs, asizes, agbits)   // one family set serves both forms
    g_gpu.batch_acts_cls_ready = true
}

def private ensure_comb_state {
    if (g_gpu.comb_ready) {
        return
    }
    g_gpu.comb_stage = make_host_buf(4l * COMB_WI_BYTES, storage = false)   // transfer_src staging: w, inv, then the inverse map ring, then the row-expert ring
    g_gpu.comb_w_dev = make_device_buf(COMB_WI_BYTES)
    g_gpu.comb_inv_dev = make_device_buf(COMB_WI_BYTES)
    g_gpu.comb_bkt_dev = make_device_buf(COMB_WI_BYTES)
    g_gpu.comb_rex_dev = make_device_buf(COMB_WI_BYTES)
    g_gpu.comb_y_dev = make_device_buf(BATCH_Y_BYTES)
    g_gpu.comb_y = make_host_buf(BATCH_Y_BYTES, true, [cached = true])
    // combine (class: accumulator rows, slot map, slot weights, down rows)
    verify(ensure_combine_cls() && ensure_combine_row_cls(), "vk combine: the class rail must engage on a live device")
    g_gpu.comb_set = set_combine_cls(
        fixed_array(g_gpu.comb_y_dev, g_gpu.comb_inv_dev, g_gpu.comb_w_dev, g_gpu.batch_y1_dev),
        fixed_array(BATCH_Y_BYTES, COMB_WI_BYTES, COMB_WI_BYTES, BATCH_Y_BYTES),
        fixed_array(VHB_CY, VHB_COMB, VHB_COMB, VHB_Y1))
    g_gpu.comb_row_set = set_combine_row_cls(
        fixed_array(g_gpu.comb_y_dev, g_gpu.comb_w_dev, g_gpu.batch_y1_dev, g_gpu.comb_bkt_dev),
        fixed_array(BATCH_Y_BYTES, COMB_WI_BYTES, BATCH_Y_BYTES, COMB_WI_BYTES),
        fixed_array(VHB_CY, VHB_COMB, VHB_Y1, VHB_COMB))
    g_gpu.comb_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: device-side moe combine engaged\n")
}

// batch bindings differ from decode's: meta is the stack's BATCH table, activations point at a
// role-fixed image (gate/up: the gathered x scratch; down: the requantized hidden planes), y one
// of the two shared device planes (gate/down write y1_dev, up y2_dev)
def private ensure_batch_set(si : int; ybuf, xqbuf, xsbuf : uint64; xqbytes, xsbytes : int64; xbit, ybit : uint; ybytes : int64 = BATCH_Y_BYTES) {
    if (g_gpu.stacks[si].batch_made && g_gpu.stacks[si].batch_y == ybuf && g_gpu.stacks[si].batch_xq == xqbuf) {
        return
    }
    if (!g_gpu.stacks[si].batch_made) {
        g_gpu.stacks[si].batch_meta = make_host_buf(int64(g_gpu.stacks[si].batch_meta_ring) * BATCH_META_BYTES, storage = false)   //! transfer_src staging, one entry a ring slot
        g_gpu.stacks[si].batch_meta_dev = make_device_buf(BATCH_META_BYTES)
        g_gpu.stacks[si].batch_made = true
    }
    // a plane rebind invalidates every class-variant set built over the old planes
    g_gpu.stacks[si].batch_cls_ready = 0u
    g_gpu.stacks[si].batch_y = ybuf
    g_gpu.stacks[si].batch_xq = xqbuf
    g_gpu.stacks[si].batch_xs = xsbuf
    g_gpu.stacks[si].batch_xq_bytes = xqbytes
    g_gpu.stacks[si].batch_xs_bytes = xsbytes
    g_gpu.stacks[si].batch_y_bytes = ybytes
    g_gpu.stacks[si].batch_xbit = xbit
    g_gpu.stacks[si].batch_ybit = ybit
}

def private ensure_batch_sets3(s1, s3, s2 : int) {
    ensure_batch_sets3_x2(s1, s3, s2, false)
}

//! the triple's batch bindings, the second stack (up / z) over the second activation image where `x2`
def private ensure_batch_sets3_x2(s1, s3, s2 : int; x2 : bool) {
    ensure_batch_set(s1, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
    if (x2) {
        ensure_batch_x2()
        ensure_batch_set(s3, g_gpu.batch_y2_dev, g_gpu.batch_xq2_dev, g_gpu.batch_xs2_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y2)
    } else {
        ensure_batch_set(s3, g_gpu.batch_y2_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y2)
    }
    ensure_batch_set(s2, g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.hs_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HQ, VHB_Y1)
}

def private ffn_stacks(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; f1, f3, f2 : int) : tuple<s1 : int; s3 : int; s2 : int>
    => (s1 = find_stack(unsafe(offs1[0]), f1, "prefill ffn gate"), s3 = find_stack(unsafe(offs3[0]), f3, "prefill ffn up"),
        s2 = find_stack(unsafe(offs2[0]), f2, "prefill ffn down"))

// chunk row cap: the gathered x window, both hidden planes, the requant planes, and the down
// output (which reuses y1) must all fit their buffers. Two activation units: `in` = gate/up
// image, `mid` = intermediate (follows the DOWN stack, may differ). 32 for Q8_0, 256 for Q8_K.
def private ffn_chunk_rows(n, nfe, xunit_in, xunit_mid : int64) : int64 {
    var r = min(BATCH_XQ_BYTES / n, BATCH_XS_BYTES / ((n / xunit_in) * 4l))
    r = min(r, BATCH_Y_BYTES / (max(nfe, n) * 4l))
    r = min(r, min(BATCH_HQ_BYTES / nfe, BATCH_HS_BYTES / ((nfe / xunit_mid) * 4l)))
    // tile-aligned so a partial last tile never reads past a buffer-derived cap (cmi8 loads unstaged)
    return min(r, BATCH_CHUNK_ROWS) / BATCH_TILE * BATCH_TILE
}

// stage the chunk's activation rows [r0, r0+rows) into the staging window (xq at 0, xs at
// BATCH_XQ_BYTES); the staging->device copies ride the HEAD of the chain submit — a separate
// synchronous copy submit here measured a full extra round trip per chunk
def private upload_batch_acts(xqp : int8 const?; xsp : float const?; r0, rows, n, xunit : int64) {
    let xq_bytes = rows * n
    let xs_bytes = rows * (n / xunit) * 4l
    unsafe {
        var sp = reinterpret<uint8?>(g_gpu.staging.mapped)
        par_memcpy(g_gpu.staging.mapped, reinterpret<void?>(xqp + r0 * n), xq_bytes)
        par_memcpy(reinterpret<void?>(sp + BATCH_XQ_BYTES), reinterpret<void?>(xsp + r0 * (n / xunit)), xs_bytes)
    }
}

// the chain-head acts copies the fold above defers to record time
def private cmd_copy_batch_acts(raw : VkCommandBuffer; xq_bytes, xs_bytes : int64) {
    cmd_copy_range(raw, g_gpu.staging.buf, 0l, g_gpu.batch_xq_dev, 0l, xq_bytes)
    cmd_copy_range(raw, g_gpu.staging.buf, BATCH_XQ_BYTES, g_gpu.batch_xs_dev, 0l, xs_bytes)
}

//! the second activation image's device planes and host staging (`moe_gpu_image2`), made at its first use
def private ensure_batch_x2 {
    if (g_gpu.batch_x2_ready) {
        return
    }
    g_gpu.batch_xq2_dev = make_device_buf(BATCH_XQ_BYTES)
    g_gpu.batch_xs2_dev = make_device_buf(BATCH_XS_BYTES)
    g_gpu.batch_x2_stage = make_host_buf(BATCH_XQ_BYTES + BATCH_XS_BYTES, false)
    g_gpu.resident_bytes += BATCH_XQ_BYTES + BATCH_XS_BYTES
    g_gpu.batch_x2_ready = true
}

//! the second image's rows [r0, r0+rows) staged like `upload_batch_acts`'s
def private upload_batch_acts2(xqp : int8 const?; xsp : float const?; r0, rows, n, xunit : int64) {
    unsafe {
        var sp = reinterpret<uint8?>(g_gpu.batch_x2_stage.mapped)
        par_memcpy(g_gpu.batch_x2_stage.mapped, reinterpret<void?>(xqp + r0 * n), rows * n)
        par_memcpy(reinterpret<void?>(sp + BATCH_XQ_BYTES), reinterpret<void?>(xsp + r0 * (n / xunit)), rows * (n / xunit) * 4l)
    }
}

//! the second image's chain-head copies, `xq_bytes` / `xs_bytes` of them; none at 0
def private cmd_copy_batch_acts2(raw : VkCommandBuffer; xq_bytes, xs_bytes : int64) {
    return if (xq_bytes == 0l)
    cmd_copy_range(raw, g_gpu.batch_x2_stage.buf, 0l, g_gpu.batch_xq2_dev, 0l, xq_bytes)
    cmd_copy_range(raw, g_gpu.batch_x2_stage.buf, BATCH_XQ_BYTES, g_gpu.batch_xs2_dev, 0l, xs_bytes)
}

// build the chunk's schedule: regions clipped to the row window [r0, r1) (row0 re-based to the
// window), then the per-workgroup region map. Weight offsets follow the stack's format units.
// Returns the workgroup count; `n` rides the push block (param removal = the deletion sweep's)
var private @scratch g_pf_sched_reg : array<int64>    //! a chunk's schedule: the kept regions' indices, their chunk-local row bases, counts and weight block bases
var private @scratch g_pf_sched_row0 : array<int64>
var private @scratch g_pf_sched_cnt : array<int64>
var private @scratch g_pf_sched_blk : array<int64>

[unused_argument(n), arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def private fill_batch_meta(si : int; offs : int64 const?; nregions : int64; n, d : int64; r0, r1 : int64; cm2 : bool = false) : uint {
    let fmt = g_gpu.stacks[si].fmt
    let unit = kq_unit(fmt)
    sched_clip(offs, nregions, r0, r1, g_pf_sched_reg, g_pf_sched_row0, g_pf_sched_cnt)
    let nrc = long_length(g_pf_sched_cnt)
    var nrows = 0l
    var all128 = true   // every active region's cnt a 128-multiple -> aligned-L eligible
    for (c in g_pf_sched_cnt) {
        nrows += c
        all128 = all128 && c % 128l == 0l
    }
    let te = batch_tile_edges(fmt, d, nrc > 0l ? nrows / nrc : r1 - r0, cm2)
    g_gpu.stacks[si].batch_tile = cm2 ? te.tc : te.tile
    g_gpu.stacks[si].batch_aligned = !cm2 && fmt == int(KqFmt.q8) && te.tile == 128l && d % 128l == 0l && all128
    let wtiles = (d + te.td - 1l) / te.td
    let wg_bound = ((r1 - r0 + te.tc - 1l) / te.tc + nrc) * wtiles
    assert((nrc * 4l + wg_bound) * 4l <= BATCH_META_BYTES, "dasLLAMA vulkan tier: batch schedule exceeds capacity")
    // class layout: 4-word records at 0, the per-wg map at map_off (n/d ride the push block)
    g_gpu.stacks[si].batch_map_off = nrc * 4l
    g_pf_sched_blk |> clear()
    g_pf_sched_blk |> reserve(nrc)
    for (r in g_pf_sched_reg) {
        let rel = unsafe(offs[r * 3l]) - g_gpu.stacks[si].base
        assert(rel % unit == 0l, "dasLLAMA vulkan tier: unaligned batch region weight offset")
        g_pf_sched_blk |> push(rel / unit)
    }
    unsafe {
        var mp = reinterpret<uint?>(reinterpret<uint8?>(g_gpu.stacks[si].batch_meta.mapped) + int64(g_gpu.stacks[si].batch_meta_at) * BATCH_META_BYTES)
        return uint(sched_regions(mp, g_pf_sched_blk, g_pf_sched_row0, g_pf_sched_cnt, te.tc, wtiles))
    }
}

let private BATCH_CLS_CM2S = 7   // batch_cls_sets slots of the cm2 s / m / l tiles
let private BATCH_CLS_CM2M = 8
let private BATCH_CLS_CM2L = 9

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def private stack_batch_enc_cm2(raw : VkCommandBuffer; var h : VkHaz; si : int; nwg : uint; n, d : int64) {
    unsafe {
        var st & = g_gpu.stacks[si]
        let tc = st.batch_tile
        let v = tc == 256l ? BATCH_CLS_CM2L : (tc == 128l ? BATCH_CLS_CM2M : BATCH_CLS_CM2S)
        if ((st.batch_cls_ready & (1u << uint(v))) == 0u) {
            verify(cm2_cls_ensure(st.fmt, tc), "vk batch: the cm2 tile class rail must engage")
            st.batch_cls_sets[v] = cm2_cls_set(st.fmt, tc,
                fixed_array(st.wqbuf, st.wsbuf, st.batch_meta_dev, st.batch_xq, st.batch_y),
                fixed_array(st.wqbytes, ws_range(si), BATCH_META_BYTES, st.batch_xq_bytes, st.batch_y_bytes),
                fixed_array(0u, 0u, VHB_META, st.batch_xbit, st.batch_ybit))
            st.batch_cls_ready |= 1u << uint(v)
        }
        var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(st.batch_map_off))
        cm2_cls_enc(st.fmt, tc, raw, h, st.batch_cls_sets[v], pc, int64(nwg))
    }
}

// one stack batch tile on the class rail: the class picked by the stack's format (q8 rides the
// tile router; kq formats are fixed), a per-(stack, variant) cached set over the planes
// ensure_batch_set recorded, BatchArgs push. n/d must MATCH the fill's values.
def private stack_batch_enc(raw : VkCommandBuffer; var h : VkHaz; si : int; nwg : uint; n, d : int64) {
    var st & = unsafe(g_gpu.stacks[si])
    let fmt = st.fmt
    // Q4_0 rides the f16 coopmat arm too (no s8-native for nibbles — mode 2 falls back to sdot4)
    let q40cm = fmt == int(KqFmt.q40) && g_gpu.coopmat_mode == COOPMAT_F16
    var v = 6   // the stack's fixed class (kq); 5 = the q40 f16 MMA twin
    if (fmt == int(KqFmt.q8)) {
        v = q8_batch_cls_variant(st.batch_tile, st.batch_aligned)
    } elif (q40cm) {
        v = 5
    }
    if ((st.batch_cls_ready & (1u << uint(v))) == 0u) {
        let bufs = fixed_array(st.wqbuf, st.wsbuf, st.batch_meta_dev, st.batch_xq, st.batch_xs, st.batch_y)
        let sizes = fixed_array(st.wqbytes, ws_range(si), BATCH_META_BYTES, st.batch_xq_bytes, st.batch_xs_bytes, st.batch_y_bytes)
        let gbits = fixed_array(0u, 0u, VHB_META, st.batch_xbit, st.batch_xbit, st.batch_ybit)
        if (fmt == int(KqFmt.q8)) {
            verify(q8_batch_cls_ensure(v), "vk batch: the q8 batch class rail must engage")
            st.batch_cls_sets[v] = q8_batch_cls_set(v, bufs, sizes, gbits)
        } elif (q40cm || kq_sb(fmt) || blk32_rail(fmt)) {
            verify(kq_batch_cls_ensure(fmt, q40cm), "vk batch: the kq batch class rail must engage")
            st.batch_cls_sets[v] = kq_batch_cls_set_for(fmt, q40cm, bufs, sizes, gbits)
        } else {
            panic("dasLLAMA vulkan tier: no batch class for stack format {fmt}")
        }
        st.batch_cls_ready |= 1u << uint(v)
    }
    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(st.batch_map_off))
    if (fmt == int(KqFmt.q8)) {
        q8_batch_cls_enc(v, raw, h, st.batch_cls_sets[v], pc, int64(nwg))
    } elif (q40cm || kq_sb(fmt) || blk32_rail(fmt)) {
        kq_batch_cls_enc_for(fmt, q40cm, raw, h, st.batch_cls_sets[v], pc, int64(nwg))
    } else {
        panic("dasLLAMA vulkan tier: no batch class for stack format {fmt}")
    }
}

def private cmd_copy_batch_meta(raw : VkCommandBuffer; si : int) {
    cmd_copy_range(raw, g_gpu.stacks[si].batch_meta.buf, int64(g_gpu.stacks[si].batch_meta_at) * BATCH_META_BYTES, g_gpu.stacks[si].batch_meta_dev, 0l, BATCH_META_BYTES)
}

def private batch_acts_head(raw : VkCommandBuffer; var h : VkHaz; axq_bytes, axs_bytes : int64; x2q_bytes : int64 = 0l; x2s_bytes : int64 = 0l) {
    vhz_dep(raw, h, 0u, VHB_ACT, true)
    cmd_copy_batch_acts(raw, axq_bytes, axs_bytes)
    cmd_copy_batch_acts2(raw, x2q_bytes, x2s_bytes)
    vhz_dep(raw, h, 0u, VHB_META, true)
}

def private batch_y1_land(raw : VkCommandBuffer; var h : VkHaz; bytes : int64) {
    vhz_dep(raw, h, VHB_Y1, 0u, true)
    cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, bytes)
}

// one reusable cmd buffer, re-recorded per submit — prefill submits are few and multi-ms, so
// recording cost is noise. comb_npos > 0: down rows stay device-side, accumulating into comb_y_dev;
// only the last chunk appends the k-fold-smaller combined DMA. Hidden planes never leave the device.
def private record_ffn_batch_cmd(s1, s3, s2 : int; nwg_gu, nwg_dn : uint; rows, n, nfe, gout_bytes : int64;
                                 axq_bytes, axs_bytes : int64; is_gelu : bool;
                                 comb_npos : uint = 0u; comb_first : bool = false; comb_last : bool = false;
                                 comb_wi_bytes : int64 = 0l; comb_y_bytes : int64 = 0l;
                                 comb_n : int64 = 0l; comb_k : int64 = 0l; comb_r0 : int64 = 0l; comb_r1 : int64 = 0l;
                                 x2q_bytes : int64 = 0l; x2s_bytes : int64 = 0l) {
    let raw = batch_cmd_raw()
    var h = cmd_open(raw, true)
    batch_acts_head(raw, h, axq_bytes, axs_bytes, x2q_bytes, x2s_bytes)
    cmd_copy_batch_meta(raw, s1)
    cmd_copy_batch_meta(raw, s3)
    cmd_copy_batch_meta(raw, s2)
    if (comb_npos > 0u && comb_first) {
        vhz_dep(raw, h, 0u, VHB_COMB, true)
        cmd_copy_whole(raw, g_gpu.comb_stage.buf, g_gpu.comb_w_dev, comb_wi_bytes)
        cmd_copy_range(raw, g_gpu.comb_stage.buf, COMB_WI_BYTES, g_gpu.comb_inv_dev, 0l, comb_wi_bytes)
    }
    stack_batch_enc(raw, h, s1, nwg_gu, n, nfe)
    stack_batch_enc(raw, h, s3, nwg_gu, n, nfe)
    // the mid-step form follows the DOWN stack — its GEMM is the consumer of the hidden plane
    var pca = ffn_form_act(rows * nfe, nfe, is_gelu)
    ffn_actrq_enc(raw, h, g_gpu.batch_actrq_set, g_gpu.batch_actrq_b_set, kq_sb(g_gpu.stacks[s2].fmt), pca, rows * nfe)
    stack_batch_enc(raw, h, s2, nwg_dn, nfe, n)
    if (comb_npos > 0u) {
        var pcc = GkMoeCombArgs(n = uint(comb_n), k = uint(comb_k), r0 = uint(comb_r0), r1 = uint(comb_r1))
        enc_combine_cls(raw, h, g_gpu.comb_set, pcc, int64(comb_npos))
        if (comb_last) {
            vhz_dep(raw, h, VHB_CY, 0u, true)
            cmd_copy_whole(raw, g_gpu.comb_y_dev, g_gpu.comb_y.buf, comb_y_bytes)
        }
    } else {
        batch_y1_land(raw, h, gout_bytes)
    }
    cmd_close(raw, h, "vk_ffnb")
}

// batch arm: chunked over row windows, down GEMM reuses y1 for output. npos > 0: the combine
// kernel folds each chunk's down rows into comb_y_dev, read back once after the last chunk.
def vk_moe_ffn_batch(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    let (s1, s3, s2) = ffn_stacks(offs1, offs3, offs2, f1, f3, f2)
    let i2 = moe_gpu_image2()
    verify(i2.on || kq_block32(f3) == kq_block32(f1),
        "dasLLAMA vulkan tier: gate/up form mismatch with no second image")
    let xunit_in = kq_unit(f1)
    let xunit_up = kq_unit(f3)
    let xunit_mid = kq_unit(f2)
    ensure_batch_state()
    ensure_batch_acts_cls()
    ensure_batch_sets3_x2(s1, s3, s2, i2.on)
    let comb = npos > 0l
    var comb_k = 0l
    if (comb) {
        ensure_comb_state()
        comb_k = nrows / npos
        assert(comb_k * npos == nrows, "dasLLAMA vulkan tier: combined rows must be npos x k")
        assert(nrows * 4l <= COMB_WI_BYTES && npos * n * 4l <= BATCH_Y_BYTES,
            "dasLLAMA vulkan tier: combined output exceeds capacity")
        unsafe {
            var cp = reinterpret<uint8?>(g_gpu.comb_stage.mapped)
            par_memcpy(g_gpu.comb_stage.mapped, reinterpret<void?>(wp), nrows * 4l)
            par_memcpy(reinterpret<void?>(cp + COMB_WI_BYTES), reinterpret<void?>(invp), nrows * 4l)
        }
    }
    let chunk = ffn_chunk_rows(n, nfe, min(xunit_in, xunit_up), xunit_mid)
    var r0 = 0l
    while (r0 < nrows) {
        let r1 = min(r0 + chunk, nrows)
        let rows = r1 - r0
        let ts0 = ref_time_ticks()
        let nwg1 = fill_batch_meta(s1, offs1, nregions, n, nfe, r0, r1)
        let nwg3 = fill_batch_meta(s3, offs3, nregions, n, nfe, r0, r1)
        assert(nwg1 == nwg3, "dasLLAMA vulkan tier: fused batch pair region mismatch")
        let nwg2 = fill_batch_meta(s2, offs2, nregions, nfe, n, r0, r1)
        let ts1 = ref_time_ticks()
        upload_batch_acts(xqp, xsp, r0, rows, n, xunit_in)
        if (i2.on) {
            upload_batch_acts2(i2.xq, i2.xs, r0, rows, n, xunit_up)
        }
        if (moe_gpu_ffn_form().bias_off >= 0l) {
            ffn_rex_fill(offs1, nregions, r0, r1)
            if (!g_gpu.batch_actrq_b_ready) {
                g_gpu.batch_actrq_b_set = ffn_actrq_b_set(g_gpu.batch_y1_dev, g_gpu.batch_y2_dev, BATCH_Y_BYTES, VHB_Y1, VHB_Y2, VHB_HQ)
                g_gpu.batch_actrq_b_ready = true
            }
        }
        let ts2 = ref_time_ticks()
        record_ffn_batch_cmd(s1, s3, s2, nwg1, nwg2, rows, n, nfe, rows * n * 4l, rows * n, rows * (n / xunit_in) * 4l, is_gelu,
            [comb_npos = comb ? uint(npos) : 0u, comb_first = r0 == 0l, comb_last = r1 == nrows,
             comb_wi_bytes = nrows * 4l, comb_y_bytes = npos * n * 4l,
             comb_n = comb ? n : 0l, comb_k = comb_k, comb_r0 = r0, comb_r1 = r1,
             x2q_bytes = i2.on ? rows * n : 0l, x2s_bytes = i2.on ? rows * (n / xunit_up) * 4l : 0l])
        let ts3 = ref_time_ticks()
        submit_wait(g_gpu.batch_cmd)
        let ts4 = ref_time_ticks()
        if (!comb) {
            unsafe {
                par_memcpy(reinterpret<void?>(goutp + r0 * n), g_gpu.batch_y1.mapped, rows * n * 4l)
            }
        }
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_ffn_batch acts {get_time_usec(ts1) - get_time_usec(ts2)} submit {get_time_usec(ts3) - get_time_usec(ts4)} copy {get_time_usec(ts4)}\n")
        }
        r0 = r1
    }
    if (comb) {
        par_memcpy(unsafe(reinterpret<void?>(goutp)), g_gpu.comb_y.mapped, npos * n * 4l)
    }
}

// one dense chunk: acts + meta staging copies, one tile GEMM over the single whole-plane
// region, y DMA
def private record_dense_cmd(si : int; nwg : uint; n, d, y_bytes : int64; axq_bytes, axs_bytes : int64) {
    record_cmd(batch_cmd_raw(), true) $(raw : VkCommandBuffer; var h : VkHaz) {
        batch_acts_head(raw, h, axq_bytes, axs_bytes)
        cmd_copy_batch_meta(raw, si)
        stack_batch_enc(raw, h, si, nwg, n, d)
        batch_y1_land(raw, h, y_bytes)
    }
}

//! MoeGpuDenseFn: dense batched GEMM — y [nrows x d] = X · Wᵀ over the resident plane
//! ([d x n] at element offset woff), a one-region ride on the batch tile rail. Chunked like
//! the FFN arm; activations arrive pre-quantized in the plane's own form.
def vk_moe_dense(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d, nrows : int64; fmt : int) {
    let si = find_stack(woff, fmt, "dense batch")
    let xunit = kq_unit(fmt)
    ensure_batch_state()
    ensure_batch_set(si, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
    var chunk = min(BATCH_XQ_BYTES / n, BATCH_XS_BYTES / ((n / xunit) * 4l))
    chunk = min(chunk, BATCH_Y_BYTES / (d * 4l))
    chunk = min(chunk, BATCH_CHUNK_ROWS) / BATCH_TILE * BATCH_TILE
    var region = fixed_array(woff, 0l, nrows)
    var r0 = 0l
    while (r0 < nrows) {
        let r1 = min(r0 + chunk, nrows)
        let rows = r1 - r0
        let ts0 = ref_time_ticks()
        let nwg = fill_batch_meta(si, unsafe(addr(region[0])), 1l, n, d, r0, r1)
        let ts1 = ref_time_ticks()
        upload_batch_acts(xqp, xsp, r0, rows, n, xunit)
        let ts2 = ref_time_ticks()
        record_dense_cmd(si, nwg, n, d, rows * d * 4l, rows * n, rows * (n / xunit) * 4l)
        let ts3 = ref_time_ticks()
        submit_wait(g_gpu.batch_cmd)
        let ts4 = ref_time_ticks()
        unsafe {
            par_memcpy(reinterpret<void?>(yp + r0 * d), g_gpu.batch_y1.mapped, rows * d * 4l)
        }
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_dense acts {get_time_usec(ts1) - get_time_usec(ts2)} submit {get_time_usec(ts3) - get_time_usec(ts4)} copy {get_time_usec(ts4)}\n")
        }
        r0 = r1
    }
}

// ===== deltanet dispatch (the recurrent-layer attention chain) =====

// the window chain's class sets over the dn scratch (built here — common cannot require classes)
def private ensure_dn_cls_sets {
    if (g_gpu.dn_cls_ready) {
        return
    }
    verify(ensure_dn_conv_cls() && ensure_dn_scan_cls() && ensure_dn_scan_p3_cls() && ensure_cls_dn_rq() && ensure_cls_q8k_rq(),
        "vk deltanet chain: the class rails must engage on a live device")
    g_gpu.dn_conv_set = set_dn_conv_cls(
        fixed_array(g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.dn_smalls_dev),
        fixed_array(BATCH_Y_BYTES, BATCH_HQ_BYTES, DN_SMALLS_BYTES),
        fixed_array(VHB_Y1, VHB_HQ, VHB_SM))
    g_gpu.dn_scan_set = set_dn_scan_cls(   //! the per-op chain captures nothing: the smalls stand in for the records and the checkpoint rows, unread
        fixed_array(g_gpu.hq_dev, g_gpu.dn_state_dev, g_gpu.dn_smalls_dev, g_gpu.dn_ws_dev, g_gpu.dn_smalls_dev, g_gpu.dn_smalls_dev),
        fixed_array(BATCH_HQ_BYTES, DN_STATE_BYTES, DN_SMALLS_BYTES, DN_WS_BYTES, DN_SMALLS_BYTES, DN_SMALLS_BYTES),
        fixed_array(VHB_HQ, VHB_ST, VHB_SM, VHB_WS, 0u, VHB_SM))
    g_gpu.dn_scan_p3_set = set_dn_scan_p3_cls(
        fixed_array(g_gpu.batch_y2_dev, g_gpu.dn_ws_dev, g_gpu.dn_smalls_dev),
        fixed_array(BATCH_Y_BYTES, DN_WS_BYTES, DN_SMALLS_BYTES),
        fixed_array(VHB_Y2, VHB_WS, VHB_SM))
    g_gpu.dn_rq_set = set_rq_cls(
        fixed_array(g_gpu.dn_ws_dev, g_gpu.hq_dev, g_gpu.hs_dev),
        fixed_array(DN_WS_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES),
        fixed_array(VHB_WS, VHB_HQ, VHB_HQ))
    g_gpu.dn_cls_ready = true
}

// one deltanet window: acts/meta/smalls staging copies, qkv + z tiles, conv, scan, the conv-tail
// extraction (before out-proj overwrites the qkv rows in y1), o requant, out-proj tile, y DMA;
// the state uploads on the first window and reads back on the last
[arch(at="../ARCHITECTURE_GPU_SHARED_KERNELS.md#dn-discrete-kernels")]
def private record_dn_cmd(s_qkv, s_z, s_o : int; nwg_qkv, nwg_z, nwg_out : uint;
                          rows, cd, kd, di, dim, taps, nvh, nkh, ds : int64;
                          axq_bytes, axs_bytes : int64; first, last, o_kq : bool; eps : float; x2q_bytes : int64 = 0l; x2s_bytes : int64 = 0l) {
    let raw = batch_cmd_raw()
    var h = cmd_open(raw, true)
    pfq_begin(raw)
    pfq_ts(raw)   // q0: submit start
    batch_acts_head(raw, h, axq_bytes, axs_bytes, x2q_bytes, x2s_bytes)
    cmd_copy_batch_meta(raw, s_qkv)
    cmd_copy_batch_meta(raw, s_z)
    cmd_copy_batch_meta(raw, s_o)
    // stops SHORT of the hist region — later windows read the device-copied tail rows there
    vhz_dep(raw, h, 0u, VHB_SM, true)
    cmd_copy_range(raw, g_gpu.dn_smalls.buf, 0l, g_gpu.dn_smalls_dev, 0l, DN_SM_HIST * 4l)
    if (first) {
        cmd_copy_range(raw, g_gpu.dn_smalls.buf, DN_SM_HIST * 4l, g_gpu.dn_smalls_dev, DN_SM_HIST * 4l, cd * taps * 4l)
        vhz_dep(raw, h, 0u, VHB_ST, true)
        cmd_copy_whole(raw, g_gpu.dn_state_stage.buf, g_gpu.dn_state_dev, nvh * ds * ds * 4l)
    }
    pfq_ts(raw)   // q1: uploads
    stack_batch_enc(raw, h, s_qkv, nwg_qkv, dim, cd)
    stack_batch_enc(raw, h, s_z, nwg_z, dim, di)
    pfq_ts(raw)   // q2: qkv+z GEMMs
    dn_window_enc(raw, h, g_gpu.dn_conv_set, g_gpu.dn_scan_set, g_gpu.dn_scan_p3_set, rows, cd, kd, nvh, nkh, ds, di, taps + 1l,
        first ? 1u : 2u, uint(DN_SM_HIST), 0u, false, eps)   // q3 conv, q4 the scan, q5 the out-norm
    // conv-tail extraction: last `taps` RAW qkv rows out of y1 before out-proj reuses it
    if (!last) {
        vhz_dep(raw, h, VHB_Y1, VHB_SM, true)
        cmd_copy_range(raw, g_gpu.batch_y1_dev, (rows - taps) * cd * 4l, g_gpu.dn_smalls_dev, DN_SM_HIST * 4l, taps * cd * 4l)
    } else {
        vhz_dep(raw, h, VHB_Y1, 0u, true)
        cmd_copy_range(raw, g_gpu.batch_y1_dev, (rows - taps) * cd * 4l, g_gpu.dn_tail_host.buf, 0l, taps * cd * 4l)
        vhz_dep(raw, h, VHB_ST, 0u, true)
        cmd_copy_whole(raw, g_gpu.dn_state_dev, g_gpu.dn_state_host.buf, nvh * ds * ds * 4l)
    }
    pfq_ts(raw)   // q6: tail copies
    rq_enc(raw, h, g_gpu.dn_rq_set, o_kq, DN_O_OFF, rows * di)
    pfq_ts(raw)   // q7: o requant
    stack_batch_enc(raw, h, s_o, nwg_out, di, dim)
    pfq_ts(raw)   // q8: out GEMM
    batch_y1_land(raw, h, rows * dim * 4l)
    pfq_end(raw)
    cmd_close(raw, h, "vk_dn")
}

//! MoeGpuDnFn: one recurrent layer's whole deltanet attention block as one submit chain per
//! DN_WINDOW rows. yp gets npos x dim rows; tailp gets the last taps qkv rows for the caller's history store.
def vk_moe_dn(var yp : float?; woq, woz, woo : int64; fq, fz, fo : int;
              xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
              convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
              var stp : float?; var tailp : float?; histp : float const?;
              npos, dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    let s_qkv = find_stack(woq, fq, "dn qkv")
    let s_z = find_stack(woz, fz, "dn z-gate")
    let s_o = find_stack(woo, fo, "dn out")
    //! the z plane reads the qkv plane's image unless the call carries its own (`moe_gpu_image2`)
    let i2 = moe_gpu_image2()
    verify(i2.on || kq_sb(fq) == kq_sb(fz), "dasLLAMA vulkan tier: the deltanet qkv and z stacks mix activation forms with no second image")
    let xunit = kq_unit(fq)
    let zunit = kq_unit(fz)
    let taps = dconv - 1l
    if (npos == 1l) {   // the decode step — device-resident state, one pre-recorded submit
        vk_moe_dn_step(yp, s_qkv, s_z, s_o, woq, woz, woo, xqp, xsp, bp, gp,
            convw, wnormp, ap, dtp, stp, histp, dim, cd, kd, di, nvh, nkh, ds, dconv, eps)
        return
    }
    verify(dn_geom_fits(ds, nvh, nkh, kd, di, cd, dconv) && npos > taps && dim % 32l == 0l, "dasLLAMA vulkan tier: deltanet geometry outside chain capacity")
    ensure_batch_state()
    ensure_dn_state()
    ensure_dn_cls_sets()
    ensure_batch_sets3_x2(s_qkv, s_z, s_o, i2.on)
    dn_stage_colds(convw, wnormp, ap, dtp, cd, dconv, nvh, ds, eps)
    unsafe {
        memcpy(reinterpret<void?>(reinterpret<float?>(g_gpu.dn_smalls.mapped) + DN_SM_HIST), reinterpret<void?>(histp), cd * taps * 4l)
        memcpy(g_gpu.dn_state_stage.mapped, reinterpret<void?>(stp), nvh * ds * ds * 4l)
    }
    var qkv_region = fixed_array(woq, 0l, 0l)
    var z_region = fixed_array(woz, 0l, 0l)
    var o_region = fixed_array(woo, 0l, 0l)
    var r0 = 0l
    while (r0 < npos) {
        let rows = min(DN_WINDOW, npos - r0)
        let ts0 = ref_time_ticks()
        qkv_region[2] = rows
        z_region[2] = rows
        o_region[2] = rows
        var nwg_qkv = 0u
        var nwg_z = 0u
        var nwg_out = 0u
        unsafe {
            nwg_qkv = fill_batch_meta(s_qkv, addr(qkv_region[0]), 1l, dim, cd, 0l, rows)
            nwg_z = fill_batch_meta(s_z, addr(z_region[0]), 1l, dim, di, 0l, rows)
            nwg_out = fill_batch_meta(s_o, addr(o_region[0]), 1l, di, dim, 0l, rows)
        }
        upload_batch_acts(xqp, xsp, r0, rows, dim, xunit)
        if (i2.on) {
            upload_batch_acts2(i2.xq, i2.xs, r0, rows, dim, zunit)
        }
        unsafe {
            var sp = reinterpret<float?>(g_gpu.dn_smalls.mapped)
            par_memcpy(reinterpret<void?>(sp + DN_SM_BETA), reinterpret<void?>(bp + r0 * nvh), rows * nvh * 4l)
            par_memcpy(reinterpret<void?>(sp + DN_SM_G), reinterpret<void?>(gp + r0 * nvh), rows * nvh * 4l)
        }
        let ts1 = ref_time_ticks()
        record_dn_cmd(s_qkv, s_z, s_o, nwg_qkv, nwg_z, nwg_out, rows, cd, kd, di, dim, taps, nvh, nkh, ds,
            rows * dim, rows * (dim / xunit) * 4l, r0 == 0l, r0 + rows == npos, kq_sb(fo), eps,
            i2.on ? rows * dim : 0l, i2.on ? rows * (dim / zunit) * 4l : 0l)
        submit_wait(g_gpu.batch_cmd)
        let ts2 = ref_time_ticks()
        unsafe {
            par_memcpy(reinterpret<void?>(yp + r0 * dim), g_gpu.batch_y1.mapped, rows * dim * 4l)
        }
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_dn prep {get_time_usec(ts0) - get_time_usec(ts1)} submit {get_time_usec(ts1) - get_time_usec(ts2)} copy {get_time_usec(ts2)}\n")
            to_log(LOG_INFO, "vk_dn gpu upl {int(pfq_us(1u))} qkv_z {int(pfq_us(2u))} conv {int(pfq_us(3u))} scan_p1 {int(pfq_us(4u))} scan_p2 {int(pfq_us(5u))} tailcp {int(pfq_us(6u))} rq {int(pfq_us(7u))} out {int(pfq_us(8u))}\n")
        }
        r0 += rows
    }
    unsafe {
        par_memcpy(reinterpret<void?>(stp), g_gpu.dn_state_host.mapped, nvh * ds * ds * 4l)
        memcpy(reinterpret<void?>(tailp), g_gpu.dn_tail_host.mapped, taps * cd * 4l)
    }
}

// ===== attention dispatch (the full-attention-layer chain) =====

def private ensure_at_state {
    if (g_gpu.at_ready) {
        return
    }
    ensure_batch_state()
    verify(ensure_at_prep_cls() && ensure_at_attn_cls() && ensure_cls_q8k_rq() && ensure_cls_dn_rq(),
        "vk attention chain: the class rails must engage on a live device")
    g_gpu.at_smalls = make_host_buf(AT_SMALLS_BYTES, false)
    g_gpu.at_smalls_dev = make_device_buf(AT_SMALLS_BYTES)
    g_gpu.at_q_dev = make_device_buf(AT_WINDOW * AT_MAX_QD * 4l)
    g_gpu.at_g_dev = make_device_buf(AT_WINDOW * AT_MAX_QD * 4l)
    g_gpu.at_k_dev = make_device_buf(AT_CTX * AT_MAX_KV * 4l)
    g_gpu.at_v_dev = make_device_buf(AT_CTX * AT_MAX_KV * 4l)
    g_gpu.at_vraw_dev = make_device_buf(AT_WINDOW * AT_MAX_KV * 4l)
    g_gpu.at_kv_host = make_host_buf(2l * AT_WINDOW * AT_MAX_KV * 4l, true, [cached = true])
    g_gpu.at_bias_dev = make_call_buf((AT_MAX_QD + 2l * AT_MAX_KV) * 4l)
    // prep (class: GEMM rows, prepped panel, gate stash, smalls, projection bias) — the k/v forms' gate view never fires
    g_gpu.at_prep_q_set = set_at_prep_cls(
        fixed_array(g_gpu.batch_y1_dev, g_gpu.at_q_dev, g_gpu.at_g_dev, g_gpu.at_smalls_dev, g_gpu.at_bias_dev),
        fixed_array(BATCH_Y_BYTES, AT_WINDOW * AT_MAX_QD * 4l, AT_WINDOW * AT_MAX_QD * 4l, AT_SMALLS_BYTES, (AT_MAX_QD + 2l * AT_MAX_KV) * 4l),
        fixed_array(VHB_Y1, VHB_ATQ, VHB_ATG, VHB_SM, VHB_ATBIAS))
    g_gpu.at_prep_k_set = set_at_prep_cls(
        fixed_array(g_gpu.batch_y2_dev, g_gpu.at_k_dev, g_gpu.at_g_dev, g_gpu.at_smalls_dev, g_gpu.at_bias_dev),
        fixed_array(BATCH_Y_BYTES, AT_CTX * AT_MAX_KV * 4l, AT_WINDOW * AT_MAX_QD * 4l, AT_SMALLS_BYTES, (AT_MAX_QD + 2l * AT_MAX_KV) * 4l),
        fixed_array(VHB_Y2, VHB_ATK, 0u, VHB_SM, VHB_ATBIAS))
    // IN PLACE on at_vraw: src and outp are the same buffer
    g_gpu.at_prep_v_set = set_at_prep_cls(
        fixed_array(g_gpu.at_vraw_dev, g_gpu.at_vraw_dev, g_gpu.at_g_dev, g_gpu.at_smalls_dev, g_gpu.at_bias_dev),
        fixed_array(AT_WINDOW * AT_MAX_KV * 4l, AT_WINDOW * AT_MAX_KV * 4l, AT_WINDOW * AT_MAX_QD * 4l, AT_SMALLS_BYTES, (AT_MAX_QD + 2l * AT_MAX_KV) * 4l),
        fixed_array(VHB_VRAW, VHB_VRAW, VHB_ATG, VHB_SM, VHB_ATBIAS))
    // attn (class: q panel, k panel, v rows, gate/out plane)
    g_gpu.at_attn_set = set_at_attn_cls(
        fixed_array(g_gpu.at_q_dev, g_gpu.at_k_dev, g_gpu.at_v_dev, g_gpu.at_g_dev),
        fixed_array(AT_WINDOW * AT_MAX_QD * 4l, AT_CTX * AT_MAX_KV * 4l, AT_CTX * AT_MAX_KV * 4l, AT_WINDOW * AT_MAX_QD * 4l),
        fixed_array(VHB_ATQ, VHB_ATK, VHB_ATV, VHB_ATG))
    // o requant: ONE family set serves both forms — the encoder picks the kernel per window
    g_gpu.at_rq_set = set_rq_cls(fixed_array(g_gpu.at_g_dev, g_gpu.hq_dev, g_gpu.hs_dev),
        fixed_array(AT_WINDOW * AT_MAX_QD * 4l, BATCH_HQ_BYTES, BATCH_HS_BYTES),
        fixed_array(VHB_ATG, VHB_HQ, VHB_HQ))
    g_gpu.at_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: attention chain engaged\n")
}

//! Does the cm2 flash-attention tile serve this window? A coopmat2-fa device, the fa knob, head 64
//! or 128, and no gate (this chain wires neither the h256 stamps nor their gated epilogue - gated models keep at_attn).
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
def private at_fa_serves(hs : int64; gated : bool) : bool =>
    !gated && cm2_fa_on() && vk_fa_on() && (hs == 64l || hs == 128l)

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
def private ensure_at_fa_state(hs : int64) {
    if (g_gpu.at_fa_ready) {
        return
    }
    verify(ensure_tower_clamp_cvt_cls() && fa_stamp_ensure(false, hs, false, false, false, false),
        "vk attention chain: the fa class rails must engage on a live device")
    let kvbytes = AT_CTX * AT_MAX_KV * 4l
    let kv16bytes = AT_CTX * AT_MAX_KV * 2l
    g_gpu.at_k16_dev = make_device_buf(kv16bytes)
    g_gpu.at_v16_dev = make_device_buf(kv16bytes)
    g_gpu.at_k16cvt_set = set_tower_clamp_cvt_cls(fixed_array(g_gpu.at_k_dev, g_gpu.at_k16_dev),
        fixed_array(kvbytes, kv16bytes), fixed_array(VHB_ATK, VHB_ATK16))
    g_gpu.at_v16cvt_set = set_tower_clamp_cvt_cls(fixed_array(g_gpu.at_v_dev, g_gpu.at_v16_dev),
        fixed_array(kvbytes, kv16bytes), fixed_array(VHB_ATV, VHB_ATV16))
    g_gpu.at_fa_set = fa_stamp_set(false,
        fixed_array(g_gpu.at_q_dev, g_gpu.at_k16_dev, g_gpu.at_v16_dev, g_gpu.at_g_dev),
        fixed_array(AT_WINDOW * AT_MAX_QD * 4l, kv16bytes, kv16bytes, AT_WINDOW * AT_MAX_QD * 4l),
        fixed_array(VHB_ATQ, VHB_ATK16, VHB_ATV16, VHB_ATG))
    g_gpu.at_fa_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: attention chain on the cm2 flash tile (hs {hs})\n")
}

// one attention window: acts/meta/smalls staging copies, q|gate + k + v tiles, the prep pair
// (deinterleave/qk-rms/rope; k lands at absolute positions), v append, the flash attention pass,
// o requant, wo tile, y DMA
[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block")]
def private record_at_cmd(s_q, s_k, s_v, s_o : int; nwg_q, nwg_k, nwg_v, nwg_o : uint;
                          rows, w0, qd, kv_dim, dim, n_heads, hs, half, n_kv, kv_mul : int64;
                          axq_bytes, axs_bytes, sm_bytes : int64; o_kq, gated, qknorm, fa, hasb : bool;
                          eps, scale : float; woq : int64) {
    let raw = batch_cmd_raw()
    var h = cmd_open(raw, true)
    pfq_begin(raw)
    pfq_ts(raw)
    batch_acts_head(raw, h, axq_bytes, axs_bytes)
    cmd_copy_batch_meta(raw, s_q)
    cmd_copy_batch_meta(raw, s_k)
    cmd_copy_batch_meta(raw, s_v)
    cmd_copy_batch_meta(raw, s_o)
    vhz_dep(raw, h, 0u, VHB_SM, true)
    cmd_copy_range(raw, g_gpu.at_smalls.buf, 0l, g_gpu.at_smalls_dev, 0l, sm_bytes)
    pfq_ts(raw)
    stack_batch_enc(raw, h, s_q, nwg_q, dim, gated ? 2l * qd : qd)
    stack_batch_enc(raw, h, s_k, nwg_k, dim, kv_dim)
    stack_batch_enc(raw, h, s_v, nwg_v, dim, kv_dim)
    pfq_ts(raw)
    let pflags = (gated ? AT_FLAG_GATED_Q : 0u) | (qknorm ? AT_FLAG_QK_NORM : 0u)
    let hb = hasb ? 1u : 0u
    var pcq = AtPrepArgs(d = uint(qd), nh = uint(n_heads), hs = uint(hs), half = uint(half),
        flags = pflags, rms_off = uint(AT_SM_RMSQ), cos_off = uint(AT_SM_COS),
        sin_off = uint(AT_SM_COS + rows * half), obase = 0u, is_q_pass = 1u, boff = 0u, hasb = hb, eps = eps)
    enc_at_prep_cls(raw, h, g_gpu.at_prep_q_set, pcq, rows)
    var pck = AtPrepArgs(d = uint(kv_dim), nh = uint(n_kv), hs = uint(hs), half = uint(half),
        flags = pflags, rms_off = uint(AT_SM_RMSK), cos_off = uint(AT_SM_COS),
        sin_off = uint(AT_SM_COS + rows * half), obase = uint(w0), is_q_pass = 0u, boff = uint(qd), hasb = hb, eps = eps)
    enc_at_prep_cls(raw, h, g_gpu.at_prep_k_set, pck, rows)
    if (hasb) {
        var pcv = AtPrepArgs(d = uint(kv_dim), nh = uint(n_kv), hs = uint(hs), half = 0u,
            flags = 0u, rms_off = 0u, cos_off = 0u, sin_off = 0u, obase = 0u, is_q_pass = 0u,
            boff = uint(qd + kv_dim), hasb = 1u, eps = eps)
        enc_at_prep_cls(raw, h, g_gpu.at_prep_v_set, pcv, rows)
    }
    pfq_ts(raw)
    // append raw v at absolute positions
    vhz_dep(raw, h, VHB_VRAW, VHB_ATV, true)
    cmd_copy_range(raw, g_gpu.at_vraw_dev, 0l, g_gpu.at_v_dev, w0 * kv_dim * 4l, rows * kv_dim * 4l)
    if (fa) {
        var pcv = gk_f16_rows_args(uint((w0 + rows) * kv_dim))
        let cvtwgs = ((w0 + rows) * kv_dim / 4l + 255l) / 256l
        enc_tower_clamp_cvt_cls(raw, h, g_gpu.at_k16cvt_set, pcv, cvtwgs)
        enc_tower_clamp_cvt_cls(raw, h, g_gpu.at_v16cvt_set, pcv, cvtwgs)
        if (w0 + rows <= DAT_MIRROR_ROWS) {
            let dkey = dat_key(woq, g_gpu.stacks[s_q].fmt)
            dat_layer_ensure(dkey, kv_dim)
            var DL & = unsafe(g_gpu.dat_layers[dkey])
            vhz_dep(raw, h, VHB_ATK16 | VHB_ATV16, 0u, true)
            cmd_copy_range(raw, g_gpu.at_k16_dev, 0l, DL.k_mir, 0l, (w0 + rows) * kv_dim * 2l)
            cmd_copy_range(raw, g_gpu.at_v16_dev, 0l, DL.v_mir, 0l, (w0 + rows) * kv_dim * 2l)
            DL.cnt = w0 + rows
            DL.filled = true
            g_dat_mirror_fills++
        }
        var pcf = FaArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd), kvd = uint(kv_dim),
            kv_mul = uint(kv_mul), kvbase = 0u, scale = scale)
        fa_stamp_enc(false, hs, false, false, false, false, raw, h, g_gpu.at_fa_set, pcf, fa_stamp_wgs(false, n_heads, rows))
    } else {
        var pca = AtAttnArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd), kvd = uint(kv_dim),
            hs = uint(hs), kv_mul = uint(kv_mul), flags = gated ? AT_ATTN_FLAG_GATED : 0u, dpl = uint(hs / 32l), scale = scale)
        enc_at_attn_cls(raw, h, g_gpu.at_attn_set, pca, at_attn_tile_wgs(n_heads, rows))
    }
    pfq_ts(raw)
    rq_enc(raw, h, g_gpu.at_rq_set, o_kq, 0l, rows * qd)
    pfq_ts(raw)
    stack_batch_enc(raw, h, s_o, nwg_o, qd, dim)
    pfq_ts(raw)
    batch_y1_land(raw, h, rows * dim * 4l)
    //! the K/V host DMAs stay at the END of the chain (ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain: a driver drops the in-cmd barrier)
    vhz_dep(raw, h, VHB_ATK, 0u, true)
    cmd_copy_range(raw, g_gpu.at_k_dev, w0 * kv_dim * 4l, g_gpu.at_kv_host.buf, 0l, rows * kv_dim * 4l)
    vhz_dep(raw, h, VHB_VRAW, 0u, true)
    cmd_copy_range(raw, g_gpu.at_vraw_dev, 0l, g_gpu.at_kv_host.buf, AT_WINDOW * AT_MAX_KV * 4l, rows * kv_dim * 4l)
    pfq_ts(raw)
    pfq_end(raw)
    cmd_close(raw, h, "vk_attn")
}

//! MoeGpuAttnFn: one full-attention layer's whole softmax attention block as one submit chain per
//! AT_WINDOW rows. yp gets npos x dim rows; kp/vp the roped-k / raw-v rows for the caller's cache store.
//! K/V persist device-side at absolute positions, so later windows attend the whole batch.
def vk_moe_attn(var yp : float?; var kp : float?; var vp : float?;
                woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                xqp : int8 const?; xsp : float const?;
                rmsqp : float const?; rmskp : float const?;
                bqp : float const?; bkp : float const?; bvp : float const?;
                cosp : float const?; sinp : float const?;
                npos, dim, qd, kv_dim, hs, rot, n_heads : int64;
                scale : float; eps : float; gated : bool; qknorm : bool) {
    let s_q = find_stack(woq, fq, "prefill attn q")
    let s_k = find_stack(wok, fk, "prefill attn k")
    let s_v = find_stack(wov, fv, "prefill attn v")
    let s_o = find_stack(woo, fo, "prefill attn o")
    // q/k/v share one activation image, so their format class must agree (the FFN triple rule);
    // the wo stack is free — the requant form follows it
    let kq = fq != 0
    assert((fk != 0) == kq && (fv != 0) == kq,
        "dasLLAMA vulkan tier: q/k/v stack format class mismatch in one attention chain")
    let half = rot / 2l
    let n_kv = kv_dim / hs
    let kv_mul = n_kv > 0l ? n_heads / n_kv : 0l
    assert(npos <= AT_CTX && qd <= AT_MAX_QD && kv_dim <= AT_MAX_KV
           && (hs == 128l || hs == 256l) && half >= 1l && 2l * half <= hs
           && n_heads * hs == qd && n_kv * hs == kv_dim && n_heads == n_kv * kv_mul
           && n_heads <= 64l && dim % 32l == 0l && kv_dim % 32l == 0l
           && qd % (fo != 0 ? 256l : 32l) == 0l
           && (gated ? 2l : 1l) * qd * AT_WINDOW * 4l <= BATCH_Y_BYTES
           && qd * AT_WINDOW <= BATCH_HQ_BYTES && npos * dim * 4l <= BATCH_Y_BYTES,
        "dasLLAMA vulkan tier: attention geometry outside chain capacity")
    let xunit = kq ? 256l : 32l
    ensure_batch_state()
    ensure_at_state()
    let fa = at_fa_serves(hs, gated)
    if (fa) {
        ensure_at_fa_state(hs)
    }
    ensure_batch_set(s_q, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
    ensure_batch_set(s_k, g_gpu.batch_y2_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y2)
    ensure_batch_set(s_v, g_gpu.at_vraw_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES,
        VHB_ACT, VHB_VRAW, [ybytes = AT_WINDOW * AT_MAX_KV * 4l])
    ensure_batch_set(s_o, g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.hs_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HQ, VHB_Y1)
    unsafe {
        var sp = reinterpret<float?>(g_gpu.at_smalls.mapped)
        sp[0] = eps
        sp[1] = scale
        if (qknorm) {
            memcpy(reinterpret<void?>(sp + AT_SM_RMSQ), reinterpret<void?>(rmsqp), hs * 4l)
            memcpy(reinterpret<void?>(sp + AT_SM_RMSK), reinterpret<void?>(rmskp), hs * 4l)
        }
    }
    let hasb = bqp != null
    if (hasb) {
        unsafe {
            upload_region_at(g_gpu.at_bias_dev, 0l, reinterpret<uint8 const?>(bqp), qd * 4l)
            upload_region_at(g_gpu.at_bias_dev, qd * 4l, reinterpret<uint8 const?>(bkp), kv_dim * 4l)
            upload_region_at(g_gpu.at_bias_dev, (qd + kv_dim) * 4l, reinterpret<uint8 const?>(bvp), kv_dim * 4l)
        }
    }
    var q_region = fixed_array(woq, 0l, 0l)
    var k_region = fixed_array(wok, 0l, 0l)
    var v_region = fixed_array(wov, 0l, 0l)
    var o_region = fixed_array(woo, 0l, 0l)
    var r0 = 0l
    while (r0 < npos) {
        let rows = min(AT_WINDOW, npos - r0)
        let ts0 = ref_time_ticks()
        q_region[1] = r0
        q_region[2] = rows
        k_region[1] = r0
        k_region[2] = rows
        v_region[1] = r0
        v_region[2] = rows
        o_region[1] = r0
        o_region[2] = rows
        var nwg_q = 0u
        var nwg_k = 0u
        var nwg_v = 0u
        var nwg_o = 0u
        unsafe {
            nwg_q = fill_batch_meta(s_q, addr(q_region[0]), 1l, dim, gated ? 2l * qd : qd, r0, r0 + rows)
            nwg_k = fill_batch_meta(s_k, addr(k_region[0]), 1l, dim, kv_dim, r0, r0 + rows)
            nwg_v = fill_batch_meta(s_v, addr(v_region[0]), 1l, dim, kv_dim, r0, r0 + rows)
            nwg_o = fill_batch_meta(s_o, addr(o_region[0]), 1l, qd, dim, r0, r0 + rows)
        }
        upload_batch_acts(xqp, xsp, r0, rows, dim, xunit)
        unsafe {
            var sp = reinterpret<float?>(g_gpu.at_smalls.mapped)
            par_memcpy(reinterpret<void?>(sp + AT_SM_COS), reinterpret<void?>(cosp + r0 * half), rows * half * 4l)
            par_memcpy(reinterpret<void?>(sp + AT_SM_COS + rows * half), reinterpret<void?>(sinp + r0 * half), rows * half * 4l)
        }
        let ts1 = ref_time_ticks()
        record_at_cmd(s_q, s_k, s_v, s_o, nwg_q, nwg_k, nwg_v, nwg_o,
            rows, r0, qd, kv_dim, dim, n_heads, hs, half, n_kv, kv_mul,
            rows * dim, rows * (dim / xunit) * 4l, (AT_SM_COS + 2l * rows * half) * 4l,
            fo != 0, gated, qknorm, fa, hasb, eps, scale, woq)
        submit_wait(g_gpu.batch_cmd)
        let ts2 = ref_time_ticks()
        unsafe {
            par_memcpy(reinterpret<void?>(yp + r0 * dim), g_gpu.batch_y1.mapped, rows * dim * 4l)
            var kvp8 = reinterpret<uint8?>(g_gpu.at_kv_host.mapped)
            par_memcpy(reinterpret<void?>(kp + r0 * kv_dim), g_gpu.at_kv_host.mapped, rows * kv_dim * 4l)
            par_memcpy(reinterpret<void?>(vp + r0 * kv_dim), reinterpret<void?>(kvp8 + AT_WINDOW * AT_MAX_KV * 4l), rows * kv_dim * 4l)
        }
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_attn prep {get_time_usec(ts0) - get_time_usec(ts1)} submit {get_time_usec(ts1) - get_time_usec(ts2)} copy {get_time_usec(ts2)}\n")
            if (g_pfq_n == 8u) {
                to_log(LOG_INFO, "vk_attn gpu copies {int(pfq_us(1u))} qkv {int(pfq_us(2u))} prep {int(pfq_us(3u))} attn {int(pfq_us(4u))} rq {int(pfq_us(5u))} wo {int(pfq_us(6u))} dma {int(pfq_us(7u))}\n")
            }
        }
        r0 += rows
    }
}

// ===== streamed-prefill dispatch (pinned-mirror layers) =====

def private ensure_stream_slots {
    if (g_gpu.stream_ready) {
        return
    }
    ensure_batch_state()
    g_gpu.stream_cmd = alloc_cmd()
    let nslots = stream_slot_count()
    g_gpu.stream_slots |> reserve(nslots)
    let cap = vk_stream_slot_caps()
    for (_slot in range(nslots)) {
        let s1 = make_stack_shell(-1l, 0l, 0, cap.gu_wq, cap.gu_ws, [xfer_shared = true, per_call = true])
        let s3 = make_stack_shell(-1l, 0l, 0, cap.gu_wq, cap.gu_ws, [xfer_shared = true, per_call = true])
        let s2 = make_stack_shell(-1l, 0l, 0, cap.dn_wq, cap.dn_ws, [xfer_shared = true, per_call = true])
        g_gpu.stacks[s1].batch_meta_ring = XF_RING   //! the per-expert chain fills XF_RING metas ahead of their copies
        g_gpu.stacks[s3].batch_meta_ring = XF_RING
        g_gpu.stacks[s2].batch_meta_ring = XF_RING
        g_gpu.stream_slots |> push(StreamSlot(group = -1, s1 = s1, s3 = s3, s2 = s2))
    }
    g_gpu.stream_ready = true
    g_gpu.land_val |> clear()
    g_gpu.land_val |> resize(max(moe_gpu_stream_ring_experts(), 1l))
    g_gpu.land_at = 0l
    let slot_bytes = 2l * (cap.gu_wq + cap.gu_ws) + cap.dn_wq + cap.dn_ws
    // same read-time carve carved_budget() subtracts — overcommit silently hits the WDDM paging cliff
    let need = moe_gpu_stream_need()
    let reserve = need > 0l ? int64(nslots) * need : STREAM_RESERVE
    assert(int64(nslots) * slot_bytes <= reserve, "dasLLAMA vulkan tier: stream slots exceed the carved stream reserve")
    assert(XF_AHEAD * XF_BUNDLE <= MOE_GPU_LAND_EXPERTS,
        "dasLLAMA vulkan tier: the ring holds fewer experts than the chain keeps in flight - a position would be reused under a command still reading it")
    let form = moe_gpu_stream_ring() ? "a ring of {moe_gpu_stream_ring_experts()} experts" : "a whole layer"
    to_log(LOG_INFO, "dasLLAMA vulkan tier: streamed prefill engaged ({length(g_stream_stacks) / 3} layers, slot {slot_bytes} bytes x {length(g_gpu.stream_slots)}, {form}); host-visible buffers {g_gpu.host_bytes >> 20l} MB, das heap {int64(heap_bytes_allocated()) >> 20l} MB\n")
}

//! a stream slot's plane capacities (`stream_slot_caps`), each plane bound as one storage range the device admits
def private vk_stream_slot_caps : tuple<gu_wq : int64; gu_ws : int64; dn_wq : int64; dn_ws : int64> {
    let cap = stream_slot_caps()
    for (b in [cap.gu_wq, cap.gu_ws, cap.dn_wq, cap.dn_ws]) {
        if (!vk_range_ok(b)) {
            panic("dasLLAMA vulkan tier: the ring slot's plane of {b} bytes ({moe_gpu_stream_ring_experts()} experts) is past the device's storage range {vk_max_storage_range()} - DASLLAMA_GPU_PF_STREAM=0 serves the prompt off the host sums")
        }
    }
    return cap
}

// re-base one slot pseudo-stack onto a stream stack (descriptors stay — the slot buffers are
// the same; only the meta-resolution identity and the kernel choice follow the layer)
def private rebase_pseudo(si : int; ss : StreamStack const) {
    if (g_gpu.stacks[si].fmt != ss.fmt) {
        g_gpu.stacks[si].batch_cls_ready = 0u   //! the cached class sets are the old format's: the next encode rebuilds them for this one
    }
    g_gpu.stacks[si].base = ss.base
    g_gpu.stacks[si].elems = ss.elems
    g_gpu.stacks[si].fmt = ss.fmt
}


//! a stream copy's recording head: every prior transfer-queue copy retired, the shared cmd reset
//! (shared cmds are safe: a wait always intervenes between recordings)
def private stream_copy_begin : VkCommandBuffer {
    let xfer = g_gpu.xfer_fam >= 0
    if (xfer && g_gpu.xfer_value > 0ul) {
        xfer_wait(g_gpu.xfer_value, false)
    }
    var raw = xfer ? g_gpu.xfer_cmd : g_gpu.stream_cmd
    cmd_open(raw, true)
    return raw
}

def private copy_stream_group(si, g : int; sync : bool) {
    let ring = moe_gpu_stream_ring()
    assert(!ring, "dasLLAMA vulkan tier: the streamed slot is the chain's ring of experts - a whole layer's copy has no room in it")
    let slot = g_gpu.stream_slots[si]
    var raw = stream_copy_begin()
    for (k in range(3)) {
        let sk = g * 3 + k
        let psi = stream_slot_stack(slot, k)
        if (g_stream_stacks[sk].slots < g_stream_stacks[sk].nexp) {
            panic("dasLLAMA vulkan tier: the whole-layer stream arm copies every expert of streamed stack {sk}, and the expert cache holds {g_stream_stacks[sk].slots} of {g_stream_stacks[sk].nexp} - a partial cache serves the split chain alone (DASLLAMA_GPU_MOE_SPLIT=1)")
        }
        cmd_copy_whole(raw, g_stream_stacks[sk].hwq.handle, g_gpu.stacks[psi].wqbuf, g_stream_stacks[sk].wqbytes)
        if (g_stream_stacks[sk].wsbytes > 0l) {
            cmd_copy_whole(raw, g_stream_stacks[sk].hws.handle, g_gpu.stacks[psi].wsbuf, g_stream_stacks[sk].wsbytes)
        }
    }
    stream_copy_end(si, raw, sync)
    g_gpu.stream_slots[si].group = g
}

//! a stream copy's submit: the transfer queue signals the timeline (the slot's consumer attaches the
//! GPU-side wait), single-queue mode keeps the old unfenced rail; `sync` blocks the host either way
def private stream_copy_end(si : int; var raw : VkCommandBuffer; sync : bool) {
    let xfer = g_gpu.xfer_fam >= 0
    cmd_close(raw)
    if (xfer) {
        let sig = xfer_submit_after(raw, XFER_NO_WAIT)
        if (sync) {
            xfer_wait(sig, false)
        }
        // the consumer still attaches the GPU-side wait — cross-queue visibility by the book
        g_gpu.stream_slots[si].wait_value = sig
    } elif (sync) {
        submit_wait(raw)
    } else {
        submit_nofence(raw)
    }
}

//! bind group g's slot for a batch dispatch: prefetch miss cold-copies the slot, the pseudo stacks
//! rebase onto it, and the slot's transfer-queue fill hands off — the next compute submit waits the timeline
def private stream_group_bind(g : int; nrows : int64; f3, f2 : int) {
    stream_group_check(g, nrows, f3, f2)
    var si = -1
    for (i in range(length(g_gpu.stream_slots))) {
        if (g_gpu.stream_slots[i].group == g) {
            si = i
        }
    }
    if (si < 0) {
        si = g % length(g_gpu.stream_slots)
        copy_stream_group(si, g, true)
    }
    stream_slot_handoff(si, g)
}

def private stream_group_check(g : int; nrows : int64; f3, f2 : int) {
    let one_row_meet = gpu_dec_meet()
    assert(nrows >= BATCH_TILE || one_row_meet, "dasLLAMA vulkan tier: streamed layers serve prefill batches only")
    assert(g_stream_stacks[g * 3 + 1].fmt == f3 && g_stream_stacks[g * 3 + 2].fmt == f2,
        "dasLLAMA vulkan tier: streamed group format mismatch")
    ensure_stream_slots()
}

def private stream_slot_handoff(si, g : int) {
    rebase_pseudo(g_gpu.stream_slots[si].s1, g_stream_stacks[g * 3])
    rebase_pseudo(g_gpu.stream_slots[si].s3, g_stream_stacks[g * 3 + 1])
    rebase_pseudo(g_gpu.stream_slots[si].s2, g_stream_stacks[g * 3 + 2])
    if (g_gpu.stream_slots[si].wait_value > 0ul) {
        g_gpu.pend_wait = g_gpu.stream_slots[si].wait_value
        g_gpu.stream_slots[si].wait_value = 0ul
    }
}

def private stream_slot_stack(slot : StreamSlot; k : int) : int => k == 0 ? slot.s1 : (k == 1 ? slot.s3 : slot.s2)

var private g_split_announced = false

//! prefetch of the next group runs descending while prefill runs ascending
def private stream_group_prefetch(g : int) {
    let nslots = stream_slot_count()
    if (nslots > 1 && g > 0 && g_gpu.stream_slots[(g - 1) % nslots].group != g - 1) {
        copy_stream_group((g - 1) % nslots, g - 1, false)
    }
}

def vk_moe_ffn_streamed(g : int; var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    stream_group_bind(g, nrows, f3, f2)
    vk_moe_ffn_batch(goutp, offs1, offs3, offs2, nregions, xqp, xsp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    stream_group_prefetch(g)
}

def vk_moe_ffn_streamed_xf(g : int; var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    let t0 = ref_time_ticks()
    let nslots = length(g_gpu.stream_slots)   //! none before the first bind makes them
    let cold = nslots == 0 || g_gpu.stream_slots[g % nslots].group != g
    stream_group_bind(g, nrows, f3, f2)
    let t1 = ref_time_ticks()
    vk_moe_ffn_batch_xf(goutp, offs1, offs3, offs2, nregions, xp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    let t2 = ref_time_ticks()
    stream_group_prefetch(g)
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_stream g{g} {cold ? "cold" : "warm"} bind {get_time_usec(t0) - get_time_usec(t1)} chain {get_time_usec(t1) - get_time_usec(t2)} prefetch {get_time_usec(t2)} us, {nregions} regions {nrows} rows\n")   // nolint:PERF026,PERF028,LINT019 — vk_prof()-gated report leg
    }
}

// ===== the cm2 expert chain: the batch arm over the f16 feed =====

//! MoeGpuFfnXfOkFn: does the cm2 chain serve a layer whose triple is (f1, f3, f2) at this window?
//! Mode 4 on a coopmat2 device, every stack a format the f16 feed admits, and the window
//! inside the f32 x plane's cap. The combined (npos > 0) form only.
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice"), arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
def vk_moe_ffn_xf_ok(npos, n : int64; f1, f3, f2 : int) : bool {
    if (g_gpu == null || !g_gpu.has_coopmat2) {   //! the dry walk answers as the live one: the mint's plan is the load's
        return false
    }
    return pf_f16_feed(f1) && pf_f16_feed(f3) && pf_f16_feed(f2) && npos * n * 4l <= BATCH_Y_BYTES
}

def private ensure_xf_state {
    if (g_gpu.xf_ready) {
        return
    }
    ensure_batch_state()
    ensure_comb_state()
    g_gpu.batch_xf_dev = make_device_buf(BATCH_Y_BYTES)
    g_gpu.resident_bytes += BATCH_Y_BYTES
    verify(ensure_moe_gather_f16_cls() && ensure_moe_gather_f16_row_cls() && ensure_actf16_cls(), "vk batch: the cm2 feed class rails must engage on a live device")
    g_gpu.batch_gather_set = set_moe_gather_f16_cls(
        fixed_array(g_gpu.batch_xf_dev, g_gpu.comb_inv_dev, g_gpu.batch_xq_dev),
        fixed_array(BATCH_Y_BYTES, COMB_WI_BYTES, BATCH_XQ_BYTES),
        fixed_array(VHB_XF, VHB_COMB, VHB_ACT))
    g_gpu.batch_gather_row_set = set_moe_gather_f16_row_cls(
        fixed_array(g_gpu.batch_xf_dev, g_gpu.comb_bkt_dev, g_gpu.batch_xq_dev),
        fixed_array(BATCH_Y_BYTES, COMB_WI_BYTES, BATCH_XQ_BYTES),
        fixed_array(VHB_XF, VHB_COMB, VHB_ACT))
    g_gpu.batch_actf16_set = set_actf16_cls(
        fixed_array(g_gpu.batch_y1_dev, g_gpu.batch_y2_dev, g_gpu.hq_dev),
        fixed_array(BATCH_Y_BYTES, BATCH_Y_BYTES, BATCH_HQ_BYTES),
        fixed_array(VHB_Y1, VHB_Y2, VHB_HQ))
    g_gpu.xf_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: cm2 expert chain engaged\n")
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
def private record_ffn_cm2_cmd(s1, s3, s2 : int; nwg_gu, nwg_dn : uint; rows, n, nfe, nk, npos : int64;
                               r0, r1 : int64; is_gelu : bool) {
    let raw = batch_cmd_raw()
    var h = cmd_open(raw, true)
    if (r0 == 0l) {
        vhz_dep(raw, h, 0u, VHB_XF, true)
        cmd_copy_whole(raw, g_gpu.staging.buf, g_gpu.batch_xf_dev, npos * n * 4l)
        vhz_dep(raw, h, 0u, VHB_COMB, true)
        cmd_copy_whole(raw, g_gpu.comb_stage.buf, g_gpu.comb_w_dev, nk * 4l)
        cmd_copy_range(raw, g_gpu.comb_stage.buf, COMB_WI_BYTES, g_gpu.comb_inv_dev, 0l, nk * 4l)
    }
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, s1)
    cmd_copy_batch_meta(raw, s3)
    cmd_copy_batch_meta(raw, s2)
    let k = uint(nk / npos)
    var pcg = GatherArgs(n = uint(n), k = k, r0 = uint(r0), r1 = uint(r1))
    enc_moe_gather_f16_cls(raw, h, g_gpu.batch_gather_set, pcg, nk)
    stack_batch_enc_cm2(raw, h, s1, nwg_gu, n, nfe)
    stack_batch_enc_cm2(raw, h, s3, nwg_gu, n, nfe)
    xf_act_enc(raw, h, rows, nfe, is_gelu, false)
    stack_batch_enc_cm2(raw, h, s2, nwg_dn, nfe, n)
    var pcc = GkMoeCombArgs(n = uint(n), k = k, r0 = uint(r0), r1 = uint(r1))
    enc_combine_cls(raw, h, g_gpu.comb_set, pcc, npos)
    if (r1 == nk) {
        vhz_dep(raw, h, VHB_CY, 0u, true)
        cmd_copy_whole(raw, g_gpu.comb_y_dev, g_gpu.comb_y.buf, npos * n * 4l)
    }
    cmd_close(raw, h, "vk_ffnc")
}

//! the chain's stacks, sets and host staging (x rows, the combine's w/inv planes) - shared by the
//! synchronous chunk loop and the async single-chunk begin
def private xf_prepare(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; xp : float const?; wp : float const?; invp : uint const?; n, nrows, npos : int64; f1, f3, f2 : int) : tuple<s1 : int; s3 : int; s2 : int> {
    let (s1, s3, s2) = ffn_stacks(offs1, offs3, offs2, f1, f3, f2)
    assert(npos > 0l && nrows % npos == 0l, "dasLLAMA vulkan tier: the cm2 expert chain serves the combined form only")
    assert(nrows * 4l <= COMB_WI_BYTES && npos * n * 4l <= BATCH_Y_BYTES,
        "dasLLAMA vulkan tier: combined output exceeds capacity")
    ensure_xf_state()
    ensure_batch_sets3(s1, s3, s2)
    unsafe {
        par_memcpy(g_gpu.staging.mapped, reinterpret<void?>(xp), npos * n * 4l)
        var cp = reinterpret<uint8?>(g_gpu.comb_stage.mapped)
        par_memcpy(g_gpu.comb_stage.mapped, reinterpret<void?>(wp), nrows * 4l)
        par_memcpy(reinterpret<void?>(cp + COMB_WI_BYTES), reinterpret<void?>(invp), nrows * 4l)
    }
    return (s1 = s1, s3 = s3, s2 = s2)
}

//! one chunk's metas + record; returns after the record so the caller picks wait or no-wait
def private xf_record_chunk(s1, s3, s2 : int; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; n, nfe, nrows, npos, r0, r1 : int64; is_gelu : bool) {
    let nwg1 = fill_batch_meta(s1, offs1, nregions, n, nfe, r0, r1, true)
    let nwg3 = fill_batch_meta(s3, offs3, nregions, n, nfe, r0, r1, true)
    assert(nwg1 == nwg3, "dasLLAMA vulkan tier: fused batch pair region mismatch")
    let nwg2 = fill_batch_meta(s2, offs2, nregions, nfe, n, r0, r1, true)
    if (moe_gpu_ffn_form().bias_off >= 0l) {
        ffn_rex_fill(offs1, nregions, r0, r1)
    }
    record_ffn_cm2_cmd(s1, s3, s2, nwg1, nwg2, r1 - r0, n, nfe, nrows, npos, r0, r1, is_gelu)
}

//! the chain's bias planes: the resident driver's under its form, the per-op upload's otherwise
def private xf_bias_planes : tuple<gb : uint64; ub : uint64; bytes : int64> =>
    moe_gpu_ffn_form().bias_resident ? (gb = g_rd.moe_gb_dev, ub = g_rd.moe_ub_dev, bytes = g_rd.moe_gub_bytes) : (gb = g_gpu.ffn_gb_dev, ub = g_gpu.ffn_ub_dev, bytes = g_gpu.ffn_bias_bytes)

//! the biased act's two sets over the chain's planes, (re)made when the bias planes change: the per-expert form reads the row-expert
//! ring copied into `comb_rex_dev`, the window form the row-expert words `ffn_rex_fill` writes
def private xf_actf16_b_sets {
    let bp = xf_bias_planes()
    return if (bp.gb != 0ul && bp.gb == g_gpu.batch_actf16_b_gb)
    verify(bp.gb != 0ul && ensure_actf16_b_cls(), "vk batch: the routed form asks a biased act, and no bias planes were uploaded")
    if (g_gpu.ffn_rex.buf == 0ul) {
        g_gpu.ffn_rex = make_host_buf(FFN_REX_ROWS * 4l, true)
    }
    g_gpu.batch_actf16_b_set = set_actf16_b_cls(
        fixed_array(g_gpu.batch_y1_dev, g_gpu.batch_y2_dev, g_gpu.hq_dev, bp.gb, bp.ub, g_gpu.comb_rex_dev),
        fixed_array(BATCH_Y_BYTES, BATCH_Y_BYTES, BATCH_HQ_BYTES, bp.bytes, bp.bytes, COMB_WI_BYTES),
        fixed_array(VHB_Y1, VHB_Y2, VHB_HQ, 0u, 0u, VHB_COMB))
    g_gpu.batch_actf16_bw_set = set_actf16_b_cls(
        fixed_array(g_gpu.batch_y1_dev, g_gpu.batch_y2_dev, g_gpu.hq_dev, bp.gb, bp.ub, g_gpu.ffn_rex.buf),
        fixed_array(BATCH_Y_BYTES, BATCH_Y_BYTES, BATCH_HQ_BYTES, bp.bytes, bp.bytes, FFN_REX_ROWS * 4l),
        fixed_array(VHB_Y1, VHB_Y2, VHB_HQ, 0u, 0u, 0u))
    g_gpu.batch_actf16_b_gb = bp.gb
}

//! the chain's act over `rows` chunk rows into the f16 hidden plane: under the routed form's bias the biased twin over the
//! row-expert plane the form names (`per_expert`: the chain's ring copy, else the window form's words), plain otherwise
def private xf_act_enc(raw : VkCommandBuffer; var h : VkHaz; rows, nfe : int64; is_gelu : bool; per_expert : bool) {
    var pca = ffn_form_act(rows * nfe, nfe, is_gelu)
    pca.nblk = uint(rows * nfe / KQ_B32_ELEMS)
    let wgs = (rows * nfe / 4l + 255l) / 256l
    if (pca.bias_on == 0u) {
        enc_actf16_cls(raw, h, g_gpu.batch_actf16_set, pca, wgs)
        return
    }
    xf_actf16_b_sets()
    enc_actf16_b_cls(raw, h, per_expert ? g_gpu.batch_actf16_b_set : g_gpu.batch_actf16_bw_set, pca, wgs)
    g_xf_biased_acts++
}

var private g_xf_biased_acts = 0l   //! the chain's acts that took the routed form's bias rows (a cell's witness)

def vk_xf_biased_acts : int64 => g_xf_biased_acts

//! MoeGpuFfnXfFn: the cm2 route of the batch arm. `xp` = the window's f32 rows (npos x n, one
//! per position); `invp`/`wp` = the combine's slot map and weights over nrows = npos x k bucket
//! rows. Chunked over bucket-row windows like the quant arm; combined rows read back once at the end.
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
def vk_moe_ffn_batch_xf(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    let st = xf_prepare(offs1, offs3, offs2, xp, wp, invp, n, nrows, npos, f1, f3, f2)
    let chunk = xf_chunk_rows(n, nfe)
    var r0 = 0l
    while (r0 < nrows) {
        let r1 = min(r0 + chunk, nrows)
        let ts0 = ref_time_ticks()
        xf_record_chunk(st.s1, st.s3, st.s2, offs1, offs3, offs2, nregions, n, nfe, nrows, npos, r0, r1, is_gelu)
        let ts1 = ref_time_ticks()
        submit_wait(g_gpu.batch_cmd)
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_ffn_cm2 prep {get_time_usec(ts0) - get_time_usec(ts1)} submit {get_time_usec(ts1)}\n")
        }
        r0 = r1
    }
    par_memcpy(unsafe(reinterpret<void?>(goutp)), g_gpu.comb_y.mapped, npos * n * 4l)
}

var private g_xf_pending = false   // ONE async cm2 chain in flight (its combined rows wait in comb_y)

//! the model drop's twin of the FFN-in-flight assert: an async cm2 chain must have been joined
def vk_xf_drop_check {
    assert(!g_xf_pending, "dasLLAMA vulkan tier: model drop with an async cm2 chain in flight")
    g_xf_pending = false
}
var private g_xf_pend_bytes = 0l

//! MoeGpuFfnXfRowsFn: the async arm serves one chunk - this many bucket rows at most.
def vk_moe_ffn_xf_async_rows(n, nfe : int64) : int64 => xf_chunk_rows(n, nfe)

//! MoeGpuFfnXfBeginFn: the cm2 chain over the given regions, submitted and left in flight - the
//! streamed-layer split runs its CPU regions meanwhile; vk_moe_ffn_batch_xf_join collects.
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
def vk_moe_ffn_batch_xf_begin(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    assert(!g_xf_pending, "dasLLAMA vulkan tier: async cm2 chain begin while one is already in flight")
    let cap = xf_chunk_rows(n, nfe)
    assert(nrows <= cap, "dasLLAMA vulkan tier: the async cm2 chain serves one chunk")
    let st = xf_prepare(offs1, offs3, offs2, xp, wp, invp, n, nrows, npos, f1, f3, f2)
    xf_record_chunk(st.s1, st.s3, st.s2, offs1, offs3, offs2, nregions, n, nfe, nrows, npos, 0l, nrows, is_gelu)
    submit_nowait(g_gpu.batch_cmd)
    g_xf_pending = true
    g_xf_pend_bytes = npos * n * 4l
}

//! MoeGpuFfnXfJoinFn: wait the pending chain (its fence, or the per-expert chain's last timeline value) and copy its combined rows out.
def vk_moe_ffn_batch_xf_join(var goutp : float?) {
    assert(g_xf_pending, "dasLLAMA vulkan tier: async cm2 chain join with nothing in flight")
    if (g_xf_per_expert) {
        if (g_xf_empty) {   //! the window pushed no expert: its share of the rows is zero
            for (i in range64(g_xf_pend_bytes / 4l)) {
                unsafe(goutp[i]) = 0.0
            }
            g_xf_pending = false
            g_xf_per_expert = false
            return
        }
        cmp_wait(g_gpu.xf_last)
        g_xf_per_expert = false
    } else {
        wait_fence()
    }
    par_memcpy(unsafe(reinterpret<void?>(goutp)), g_gpu.comb_y.mapped, g_xf_pend_bytes)
    g_xf_pending = false
}

var private g_xf_per_expert = false   //! the chain in flight is the per-expert form: the join waits its timeline value, not the fence
var private g_xf_empty = false        //! ... and it pushed no expert

//! the per-expert chain's rings, made once a model: XF_RING compute and transfer commands with their last signalled values
[cold_path]
def private ensure_xf_rings {
    return if (g_gpu.xf_rings_ready)
    g_gpu.xf_cmds |> reserve(XF_RING)
    g_gpu.xf_vals |> reserve(XF_RING)
    g_gpu.xf_xcmds |> reserve(XF_RING)
    g_gpu.xf_xvals |> reserve(XF_RING)
    for (_i in range(XF_RING)) {
        g_gpu.xf_cmds |> push(alloc_cmd())
        g_gpu.xf_vals |> push(0ul)
        if (g_gpu.xfer_fam >= 0) {
            g_gpu.xf_xcmds |> push(alloc_xfer_cmd())
        }
        g_gpu.xf_xvals |> push(0ul)
    }
    g_gpu.xf_rings_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: per-expert streamed chain engaged (ring {XF_RING}, {g_gpu.xfer_fam >= 0 ? "transfer queue" : "one queue"})\n")
}

//! expert `e` of stream group `g`'s three planes, the cache slot's bytes onto slot `dslot` of the destination stacks `dst`
//! (plane order gate, up, down); an expert the cache does not hold is a panic naming `who` - the window's fetch missed it
def private xf_expert_copy(raw : VkCommandBuffer; g : int; e : int64; dst : int[3]; dslot : int64; who : string) {
    for (k in range(3)) {
        let sk = g * 3 + k
        let src = stream_slot(sk, e)
        if (!src.present) {
            panic("dasLLAMA vulkan tier: {who} claimed expert {e} of streamed stack {sk}, which the expert cache does not hold - the window's fetch missed it")
        }
        let qlen = g_stream_stacks[sk].qlen
        let slen = g_stream_stacks[sk].slen
        cmd_copy_range(raw, src.qbuf, src.qoff, g_gpu.stacks[dst[k]].wqbuf, dslot * qlen, qlen)
        if (slen > 0l) {
            cmd_copy_range(raw, src.sbuf, src.soff, g_gpu.stacks[dst[k]].wsbuf, dslot * slen, slen)
        }
    }
}

//! the stream slot's three stacks in plane order
def private stream_slot_stacks(slot : StreamSlot) : int[3] => fixed_array(slot.s1, slot.s3, slot.s2)

//! one bundle's chain over its `rows` chunk rows, grid entries at [r0, r1) of the grid-list ring; `first` lands the window and zeroes the accumulator
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
def private record_xf_expert_cmd(raw : VkCommandBuffer; s1, s3, s2 : int; nwg_gu, nwg_dn : uint;
                                 rows, n, nfe, nk, npos, r0, r1, nreg : int64; is_gelu, first : bool; copies : block<(raw : VkCommandBuffer) : void>) {
    var h = cmd_open(raw, true)
    invoke(copies, raw)
    cmd_full_barrier(raw)
    if (first) {
        vhz_dep(raw, h, 0u, VHB_XF, true)
        cmd_copy_whole(raw, g_gpu.staging.buf, g_gpu.batch_xf_dev, npos * n * 4l)
        vhz_dep(raw, h, 0u, VHB_COMB | VHB_CY, true)
        cmd_copy_whole(raw, g_gpu.comb_stage.buf, g_gpu.comb_w_dev, nk * 4l)
        cmd_fill(raw, g_gpu.comb_y_dev, 0l, npos * n * 4l, 0u)
    }
    vhz_dep(raw, h, 0u, VHB_COMB, true)
    cmd_copy_range(raw, g_gpu.comb_stage.buf, 2l * COMB_WI_BYTES + r0 * 4l, g_gpu.comb_bkt_dev, r0 * 4l, rows * 4l)
    if (moe_gpu_ffn_form().bias_off >= 0l) {   // the bundle's rows' experts, chunk-local: the full barrier above retired the ring's earlier readers
        cmd_copy_range(raw, g_gpu.comb_stage.buf, 3l * COMB_WI_BYTES + r0 * 4l, g_gpu.comb_rex_dev, 0l, rows * 4l)
    }
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, s1)
    cmd_copy_batch_meta(raw, s3)
    cmd_copy_batch_meta(raw, s2)
    let k = uint(nk / npos)
    var pcg = GatherArgs(n = uint(n), k = k, r0 = uint(r0), r1 = uint(r1))
    enc_moe_gather_f16_row_cls(raw, h, g_gpu.batch_gather_row_set, pcg, rows)
    stack_batch_enc_cm2(raw, h, s1, nwg_gu, n, nfe)
    stack_batch_enc_cm2(raw, h, s3, nwg_gu, n, nfe)
    xf_act_enc(raw, h, rows, nfe, is_gelu, true)
    stack_batch_enc_cm2(raw, h, s2, nwg_dn, nfe, n)
    // the combine a region at a time: a bundle's experts share positions, and the by-row add takes no two rows of one dispatch on one position
    for (i in range64(nreg)) {
        let rb = uint(g_xf_boffs1[i * 3l + 1l])
        let cnt = g_xf_boffs1[i * 3l + 2l]
        var pcc = GkMoeCombArgs(n = uint(n), k = k, r0 = uint(r0) + rb, r1 = uint(r0) + rb + uint(cnt), goff = rb)
        enc_combine_row_cls(raw, h, g_gpu.comb_row_set, pcc, cnt)
    }
    cmd_close(raw, h, "vk_ffnx")
}

//! the chain's close: the accumulator copied out behind every expert's command (the full barrier orders them)
def private record_xf_close_cmd(raw : VkCommandBuffer; n, npos : int64) {
    var h = cmd_open(raw, true)
    cmd_full_barrier(raw)
    vhz_dep(raw, h, VHB_CY, 0u, true)
    cmd_copy_whole(raw, g_gpu.comb_y_dev, g_gpu.comb_y.buf, npos * n * 4l)
    cmd_close(raw, h, "vk_ffnx_out")
}

//! an opened window of the expert chain (`XfMeetHost`): its slot, the pseudo-stacks, and whether the copies ride the transfer queue
struct private XfMeet : XfMeetHost {
    slot : StreamSlot
    s1 : int
    s3 : int
    s2 : int
    p1 : int          //! the layer's pool stacks, the first `npool` regions' (the pool window)
    p3 : int
    p2 : int
    xfer : bool
}

var private g_xfm : XfMeet
var private @scratch g_xf_bkt : array<uint>     //! the opened window's bucket row -> grid entry (the slot map's inverse)
var private @scratch g_xf_boffs1 : array<int64> //! a bundle's region triples at chunk-local row bases
var private @scratch g_xf_boffs3 : array<int64>
var private @scratch g_xf_boffs2 : array<int64>

//! the pool window: the chain's first `g_xf_pool_n` regions on a layer's pool stacks, `copies[r]` the expert region `r`'s slot takes off group `g_xf_pool_g` (-1 a hit); null = the stream form
var g_xf_pool_copies : int const?
var g_xf_pool_n = 0l
var g_xf_pool_g = -1

def xf_pool_on : bool => g_xf_pool_copies != null

//! MoeGpuXfPoolWindowFn: the next window's first `npool` regions on layer `l`'s pool, copies sourced off the group holding `woff1`; its compute waits the layer's last DMA fill
def vk_xf_pool_window(copies : int const?; npool, l, woff1 : int64; fmt1 : int) {
    g_xf_pool_copies = copies
    g_xf_pool_n = copies == null ? 0l : npool
    g_xf_pool_g = copies == null ? -1 : stream_group_of(woff1, fmt1)
    if (copies != null && g_xf_pool_g < 0) {
        panic("dasLLAMA vulkan tier: the pool window names a layer at {woff1} (fmt {fmt1}) with no streamed stack to copy from")
    }
    if (copies != null && g_rd != null && l < long_length(g_rd.hot_layers)) {
        g_gpu.pend_wait = max(g_gpu.pend_wait, g_rd.hot_layers[l].dma_value)
    }
}

//! region `r`'s expert `e` of stream group `g` copied into the pool slot the region's gate offset names (the three planes
//! sit at one slot index, a plane at its stack's stride)
def private xf_expert_copy_pool(raw : VkCommandBuffer; g : int; r, e : int64) {
    let m & = unsafe(g_xfm)
    let slot = (unsafe(m.offs1[r * 3l]) - g_gpu.stacks[m.p1].base) / m.ege
    assert(g_gpu.stacks[m.p1].wqbytes / (g_gpu.stacks[m.p1].elems / m.ege) == g_stream_stacks[g * 3].qlen, "dasLLAMA vulkan tier: a pool slot and a streamed expert differ in bytes")
    xf_expert_copy(raw, g, e, fixed_array(m.p1, m.p3, m.p2), slot, "the pool window")
}

var private @scratch g_xf_pos : array<int64>   //! the ring form: the opened window's region -> its ring position (a stream region's, once its bundle is pushed)

//! the ring form: regions [r_lo, r_hi) past the pool prefix take the next ring positions; returns the compute value the
//! bundle's copies wait - the last reader of any position they land in
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#stream-slot-ring")]
def private xf_assign_land(r_lo, r_hi : int64) : uint64 {
    let m & = unsafe(g_xfm)
    var wait = XFER_NO_WAIT
    for (r in range64(max(r_lo, m.npool), r_hi)) {
        let pos = g_gpu.land_at % moe_gpu_stream_ring_experts()
        g_gpu.land_at++
        g_xf_pos[r] = pos
        wait = max(wait, g_gpu.land_val[pos])
    }
    return wait
}

//! the ring form: the bundle's positions read by the chain that signals `val`
def private xf_land_done(r_lo, r_hi : int64; val : uint64) {
    let m & = unsafe(g_xfm)
    for (r in range64(max(r_lo, m.npool), r_hi)) {
        g_gpu.land_val[g_xf_pos[r]] = val
    }
}

//! a bundle region's copy: into its pool slot for a pool region (none for a hit), onto the stream slot otherwise - at the
//! region's ring position under the ring form, at the expert's own index in a whole-layer slot
def private xf_push_copy(raw : VkCommandBuffer; r, base : int64) {
    let m & = unsafe(g_xfm)
    if (r < m.npool) {
        let e = int64(unsafe(g_xf_pool_copies[r]))
        if (e >= 0l) {
            xf_expert_copy_pool(raw, m.g, r, e)
        }
        return
    }
    let e = (unsafe(m.offs1[r * 3l]) - base) / m.ege   //! the expert off the live offset table: the meet rotates the regions after the window opened
    xf_expert_copy(raw, m.g, e, stream_slot_stacks(m.slot), moe_gpu_stream_ring() ? g_xf_pos[r] : e, "the expert chain")
}

//! MoeGpuFfnXfOpenFn's streamed form: group `g`'s window prepared - the slot rebased, the x rows, the weights and the map staged; nothing submitted until a push
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
def vk_moe_ffn_xf_open_experts(g : int; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    assert(!g_xf_pending && !g_xfm.on, "dasLLAMA vulkan tier: async cm2 chain open while one is already in flight")
    assert(nregions > 0l, "dasLLAMA vulkan tier: the per-expert chain opens over no region")
    if (!g_split_announced) {
        g_split_announced = true
        to_log(LOG_INFO, "dasLLAMA vulkan tier: streamed prefill split engaged (DASLLAMA_GPU_MOE_SPLIT=0 streams whole layers)\n")
    }
    stream_group_check(g, nrows, f3, f2)
    if (!g_gpu.xf_rings_ready) {
        ensure_xf_rings()
    }
    g_xfm.t0 = ref_time_ticks()
    let si = g % length(g_gpu.stream_slots)
    let slot = g_gpu.stream_slots[si]
    let npool = xf_pool_on() ? min(g_xf_pool_n, nregions) : 0l
    let ring_named = !moe_gpu_stream_ring() || xf_pool_on()
    assert(ring_named, "dasLLAMA vulkan tier: the ring form opens a window with no copy list - every stream region names its expert through it")
    g_xf_pos |> grow_resize(nregions)
    if (npool < nregions) {   //! a window over the pool alone binds the pool's stacks through its offsets; the stream slot stays as it is
        rebase_pseudo(slot.s1, g_stream_stacks[g * 3])
        rebase_pseudo(slot.s3, g_stream_stacks[g * 3 + 1])
        rebase_pseudo(slot.s2, g_stream_stacks[g * 3 + 2])
        g_gpu.stream_slots[si].group = -1   //! a partial fill: a later window never mistakes it for the whole group
    }
    var st = xf_prepare(offs1, offs3, offs2, xp, wp, invp, n, nrows, npos, f1, f3, f2)   //! the first region's stacks: the pool's under a pool prefix
    let pst = st
    if (npool > 0l && npool < nregions) {
        st = (s1 = slot.s1, s3 = slot.s3, s2 = slot.s2)
        ensure_batch_sets3(st.s1, st.s3, st.s2)
    }
    g_xf_bkt |> grow_resize(nrows)
    for (gi in range64(nrows)) {
        g_xf_bkt[int64(unsafe(invp[gi]))] = uint(gi)
    }
    let ege = nfe * n
    var exp_bytes = 0l
    for (k in range(3)) {
        let ss & = unsafe(g_stream_stacks[g * 3 + k])
        exp_bytes += (ss.wqbytes + ss.wsbytes) / (ss.elems / ege)
    }
    let cap = xf_chunk_rows(n, nfe)
    assert(int64(XF_RING) * cap * 4l <= COMB_WI_BYTES, "dasLLAMA vulkan tier: the grid-list ring passes its plane")
    g_xfm = XfMeet(on = true, g = g, slot = slot, s1 = st.s1, s3 = st.s3, s2 = st.s2, p1 = pst.s1, p3 = pst.s3, p2 = pst.s2, npool = npool, xfer = g_gpu.xfer_fam >= 0,
        offs1 = offs1, offs3 = offs3, offs2 = offs2, nregions = nregions, ege = ege, cap = cap,
        n = n, nfe = nfe, nk = nrows, npos = npos, is_gelu = is_gelu, pushed = 0l, done = 0l, experts = 0l, t0 = g_xfm.t0, exp_bytes = exp_bytes)
}

//! the bundle's three region lists at chunk-local row bases (region after region), and its chunk rows' grid entries into
//! ring entry `j` of the grid-list staging
def private xf_bundle_lists(r_lo, r_hi : int64; j : int) : int64 {
    let m & = unsafe(g_xfm)
    let nb = r_hi - r_lo
    g_xf_boffs1 |> grow_resize(nb * 3l)
    g_xf_boffs3 |> grow_resize(nb * 3l)
    g_xf_boffs2 |> grow_resize(nb * 3l)
    var rows = 0l
    let ring = moe_gpu_stream_ring()
    let base = g_stream_stacks[m.g * 3].base
    unsafe {
        var gl = reinterpret<uint?>(reinterpret<uint8?>(g_gpu.comb_stage.mapped) + 2l * COMB_WI_BYTES + int64(j) * m.cap * 4l)
        var rx = reinterpret<uint?>(reinterpret<uint8?>(g_gpu.comb_stage.mapped) + 3l * COMB_WI_BYTES + int64(j) * m.cap * 4l)
        for (i in range64(nb)) {
            let r = r_lo + i
            let b0 = m.offs1[r * 3l + 1l]
            let cnt = m.offs1[r * 3l + 2l]
            let ex = uint(xf_region_expert(m, r, base))
            if (ring && r >= m.npool) {
                g_xf_boffs1[i * 3l] = g_stream_stacks[m.g * 3].base + g_xf_pos[r] * m.ege
                g_xf_boffs3[i * 3l] = g_stream_stacks[m.g * 3 + 1].base + g_xf_pos[r] * m.ege
                g_xf_boffs2[i * 3l] = g_stream_stacks[m.g * 3 + 2].base + g_xf_pos[r] * m.ege
            } else {
                g_xf_boffs1[i * 3l] = m.offs1[r * 3l]
                g_xf_boffs3[i * 3l] = m.offs3[r * 3l]
                g_xf_boffs2[i * 3l] = m.offs2[r * 3l]
            }
            g_xf_boffs1[i * 3l + 1l] = rows
            g_xf_boffs3[i * 3l + 1l] = rows
            g_xf_boffs2[i * 3l + 1l] = rows
            g_xf_boffs1[i * 3l + 2l] = cnt
            g_xf_boffs3[i * 3l + 2l] = cnt
            g_xf_boffs2[i * 3l + 2l] = cnt
            for (b in range64(cnt)) {
                gl[rows + b] = g_xf_bkt[b0 + b]
                rx[rows + b] = ex
            }
            rows += cnt
        }
    }
    return rows
}

//! MoeGpuFfnXfPushFn's streamed form: regions [r_lo, r_hi) as one bundle on ring entry `pushed % XF_RING`; without `wait` an entry still in flight answers false and nothing is queued
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
def vk_moe_ffn_xf_push_experts(r_lo, r_hi : int64; wait : bool) : bool {
    assert(g_xfm.on, "dasLLAMA vulkan tier: expert chain push with no window open")
    var m & = unsafe(g_xfm)
    let j = int(m.pushed % int64(XF_RING))
    if (!wait) {
        // at most XF_AHEAD bundles in flight, so the host keeps claiming from its own end: the ring's depth is for re-recording, not for claiming
        while (m.done < m.pushed && cmp_reached(g_gpu.xf_vals[int(m.done % int64(XF_RING))])) {
            m.done++
        }
        return false if (m.pushed - m.done >= XF_AHEAD || (g_gpu.xf_vals[j] > 0ul && !cmp_reached(g_gpu.xf_vals[j]))
            || (m.xfer && g_gpu.xf_xvals[j] > 0ul && !xfer_reached(g_gpu.xf_xvals[j])))
    }
    let bundle_rows = xf_bundle_rows(m, r_lo, r_hi)
    assert(bundle_rows >= 0l, "dasLLAMA vulkan tier: the expert chain's bundle passes the chunk cap or XF_BUNDLE")
    let base = g_stream_stacks[m.g * 3].base
    let ring = moe_gpu_stream_ring()
    let land_wait = ring ? xf_assign_land(r_lo, r_hi) : XFER_NO_WAIT
    var sig = 0ul
    if (m.xfer) {
        if (g_gpu.xf_xvals[j] > 0ul) {
            xfer_wait(g_gpu.xf_xvals[j], false)   //! this entry's earlier copy retired: the command is free to re-record
        }
        var xraw = g_gpu.xf_xcmds[j]
        cmd_open(xraw, true)
        for (r in range64(r_lo, r_hi)) {
            xf_push_copy(xraw, r, base)
        }
        cmd_close(xraw)
        sig = xfer_submit_after(xraw, land_wait)
        g_gpu.xf_xvals[j] = sig
    }
    if (g_gpu.xf_vals[j] > 0ul) {
        cmp_wait(g_gpu.xf_vals[j])   //! this entry's earlier chain retired: its command, its meta entry and its grid list are free
    }
    let rows = xf_bundle_lists(r_lo, r_hi, j)
    let pool = r_lo < m.npool
    let s1 = pool ? m.p1 : m.s1
    let s3 = pool ? m.p3 : m.s3
    let s2 = pool ? m.p2 : m.s2
    g_gpu.stacks[s1].batch_meta_at = j
    g_gpu.stacks[s3].batch_meta_at = j
    g_gpu.stacks[s2].batch_meta_at = j
    let nb = r_hi - r_lo
    unsafe {
        let nwg1 = fill_batch_meta(s1, addr<int64 const?>(g_xf_boffs1[0]), nb, m.n, m.nfe, 0l, rows, true)
        let nwg3 = fill_batch_meta(s3, addr<int64 const?>(g_xf_boffs3[0]), nb, m.n, m.nfe, 0l, rows, true)
        assert(nwg1 == nwg3, "dasLLAMA vulkan tier: fused batch pair region mismatch")
        let nwg2 = fill_batch_meta(s2, addr<int64 const?>(g_xf_boffs2[0]), nb, m.nfe, m.n, 0l, rows, true)
        var raw = g_gpu.xf_cmds[j]
        let r0 = int64(j) * m.cap
        record_xf_expert_cmd(raw, s1, s3, s2, nwg1, nwg2, rows, m.n, m.nfe, m.nk, m.npos, r0, r0 + rows, nb, m.is_gelu, m.pushed == 0l) $(craw : VkCommandBuffer) {
            if (!m.xfer) {
                for (r in range64(r_lo, r_hi)) {
                    xf_push_copy(craw, r, base)
                }
            }
        }
        g_gpu.pend_wait = max(g_gpu.pend_wait, sig)   //! the chain waits its own copy: the values ascend, so the max is this bundle's
        g_gpu.xf_vals[j] = submit_signal(raw)
        if (ring) {
            xf_land_done(r_lo, r_hi, g_gpu.xf_vals[j])
        }
    }
    m.pushed++
    m.experts += nb
    return true
}

//! MoeGpuFfnXfCloseFn's streamed form: the accumulator's copy-out queued behind the pushed chains; a window that pushed nothing leaves the join to answer zero rows
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
def vk_moe_ffn_xf_close_experts {
    assert(g_xfm.on, "dasLLAMA vulkan tier: per-expert chain close with no window open")
    var m & = unsafe(g_xfm)
    if (m.pushed > 0l) {
        let j = int(m.pushed % int64(XF_RING))
        if (g_gpu.xf_vals[j] > 0ul) {
            cmp_wait(g_gpu.xf_vals[j])
        }
        var raw = g_gpu.xf_cmds[j]
        record_xf_close_cmd(raw, m.n, m.npos)
        g_gpu.xf_vals[j] = submit_signal(raw)
        g_gpu.xf_last = g_gpu.xf_vals[j]
    } else {
        g_gpu.xf_last = 0ul
    }
    g_gpu.stacks[m.s1].batch_meta_at = 0
    g_gpu.stacks[m.s3].batch_meta_at = 0
    g_gpu.stacks[m.s2].batch_meta_at = 0
    g_gpu.stacks[m.p1].batch_meta_at = 0
    g_gpu.stacks[m.p3].batch_meta_at = 0
    g_gpu.stacks[m.p2].batch_meta_at = 0
    g_xf_pending = true
    g_xf_per_expert = true
    g_xf_empty = m.pushed == 0l
    g_xf_pend_bytes = m.npos * m.n * 4l
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_xf_experts g{m.g} {m.experts} of {m.nregions} experts in {m.pushed} bundles, {m.nk} rows, {m.exp_bytes * m.experts / 1000000l} MB queued in {get_time_usec(m.t0)} us\n")   // nolint:PERF026,PERF028,LINT019 — vk_prof()-gated report leg
    }
    m.on = false
}

//! MoeGpuFfnXfBundleFn's streamed form: the end of the bundle starting at region `r_lo` of the opened window
def vk_moe_ffn_xf_bundle_end(r_lo, r_max : int64) : int64 => xf_bundle_end(g_xfm, r_lo, r_max)

//! MoeGpuFfnXfBeginFn's streamed form: the whole head at once - open, every bundle pushed in turn, close; the join collects
def vk_moe_ffn_xf_begin_experts(g : int; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    vk_moe_ffn_xf_open_experts(g, offs1, offs3, offs2, nregions, xp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    var r = 0l
    while (r < nregions) {
        let r_hi = vk_moe_ffn_xf_bundle_end(r, nregions)
        verify(vk_moe_ffn_xf_push_experts(r, r_hi, true), "vk expert chain: a waited push always queues")
        r = r_hi
    }
    vk_moe_ffn_xf_close_experts()
}
