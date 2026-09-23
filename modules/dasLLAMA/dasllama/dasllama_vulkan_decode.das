options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_decode shared public
options _cyclomatic_complexity = 0   // whole-token record/setup walks — split queued for the dedup phase
options _function_length = 0   // same

require dasllama/dasllama_lint public
require dasllama/dasllama_vulkan_common   // g_gpu/g_rd state, plumbing, hazard rail, fill metas
require dasllama/dasllama_vulkan_classes   // the class-kernel rail (generated set_/enc_ + gemv_cls_*)
require dasllama/dasllama_kqformat   // kq_sb — the Q8_K-form activation pick per consumer plane
require dasllama/dasllama_common
require dasllama/dasllama_par   // maybe_parallel_for: the N-row logits land a row a lane
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require math
require daslib/strings_boost

// The Vulkan DECODE driver (dasllama_metal_decode's twin): the resident whole-stack token step
// (prepare/set_layer/set_cls + the once-per-epoch recorded command + vk_rdec_token), the KV
// mirror sync/read seams, the deltanet decode step, the FFN GEMV / qkv-group / classifier
// decode arms, and the heat-pinned expert cache.

let RDEC_MIR_SLACK_ROWS = 16l   //! the KV rows a flash fragment may read past a window's end
let RDEC_MIR_SLACK = RDEC_MIR_SLACK_ROWS * AR_MAX_DIM   //! slack elements past each KV mirror's last plane: one fragment at the widest KV row the prepare admits
let DA_NSPLIT_MAX = 16l   //! the decode attention's key splits a head, at most (the partials plane sizes to it)
let RD_NB_MAX = 8l   //! the N-row token command's rows, at most
let RD_PICK_CHUNKS = int64(CLS_ARGMAX_CHUNKS)   //! the pick's first pass: a workgroup a (row, chunk)
let NROW_BX_ATTN = 1        //! `DASLLAMA_VK_NROW_BISECT`, the N-row command's probe bits: no attention
let NROW_BX_NCOL1 = 2       //! the one-column GEMV leaves over row 0
let NROW_BX_ROPE = 4        //! no rope (and no q/k norm)
let NROW_BX_ACTRQ = 8       //! no act + requant
var g_rd_nrow_bisect = 0   //! a probe's bisect of the N-row command, the `NROW_BX_*` bits - the logits are garbage under anyder any

//! the token command's key splits a head: enough (head, split) workgroups to cover the card twice over, at most
//! DA_NSPLIT_MAX; one where the card's SM count is unknown or the heads alone cover it
def da_nsplit(n_heads, sm_count : int64) : int64 {
    if (sm_count <= 0l || n_heads <= 0l || n_heads >= 2l * sm_count) return 1l
    return min((2l * sm_count + n_heads - 1l) / n_heads, DA_NSPLIT_MAX)
}

//! Resident-driver prepare: allocate all per-token + session buffers for a model's geometry. One
//! resident model at a time (the arbiter guarantees it). Call before set_layer / token.
//! `kv16` picks the mirror codec (f16 default upstream) — sessions of the same codec byte-copy.
def vk_rdec_prepare(n_layers, n_attn, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64;
                    neox : bool; eps, scale : float; qk_norm : bool; kv16 : bool; q_gated : bool; rot : int64; dn : RdecDnGeom;
                    mir_elems, cos_elems, regions : int64) : bool {
    if (!vk_moe_init()) {
        return false
    }
    let esz = kv16 ? 2l : 4l
    // every region and its slack bind in one range: past maxStorageBufferRange, or the shaders' uint word index, every mirror read is undefined
    let mir_bound = (mir_elems + RDEC_MIR_SLACK) * regions
    if (mir_bound * esz > vk_max_storage_range() || mir_bound > 4_294_967_295l) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: KV mirror ({n_attn} layers x {seq_cap} positions, {mir_elems} elements a side plus the {RDEC_MIR_SLACK} slack) exceeds maxStorageBufferRange {vk_max_storage_range()} / the 32-bit word index\n")
        return false
    }
    // the residual-stream kernels stage a whole row in ar_row, and the per-head ones a whole head
    // in their row slabs — over either slab they would write past the workgroup array with no diagnostic;
    // the mirror slack covers a flash fragment at a KV row up to AR_MAX_DIM wide
    if (dim > AR_MAX_DIM || head_size > 512l || kv_dim > AR_MAX_DIM) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: dim {dim} or kv_dim {kv_dim} > {AR_MAX_DIM}, or head_size {head_size} > 512, exceeds the workgroup row slabs and the mirror slack\n")
        return false
    }
    let hybrid = dn.cd > 0l
    if (hybrid && !(dn.ds <= DN_MAX_DS && dn.ds % 32l == 0l && (dn.ds & (dn.ds - 1l)) == 0l && dn.nvh <= DN_MAX_HEADS && dn.cd <= DN_MAX_CD && 2l * dn.kd <= DN_MAX_QK
            && dn.dconv <= DN_MAX_DCONV && dn.di == dn.nvh * dn.ds && dn.kd == dn.nkh * dn.ds
            && dn.cd == 2l * dn.kd + dn.di && dn.di % 32l == 0l && dn.di <= DND_OQ_BYTES)) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: deltanet geometry (cd {dn.cd}, kd {dn.kd}, di {dn.di}, heads {dn.nvh}/{dn.nkh}, state {dn.ds}, conv {dn.dconv}) is outside the step kernel's capacity\n")
        return false
    }
    if (g_gpu.dry) {
        return true   // guards above ARE the plan decision; everything below is device state
    }
    if (hybrid && (dn.nvh * dn.ds * dn.ds * 4l * regions > vk_max_storage_range()
            || (DN_SM_HIST + regions * 2l * dn.cd * (dn.dconv - 1l) + DN_SM_SLACK) * 4l > vk_max_storage_range())) {   //! a layer's state slots and its ring pairs each bind as one range
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the deltanet state slots ({regions} regions x {dn.nvh} heads x {dn.ds}^2) pass the device's storage range {vk_max_storage_range()}\n")
        return false
    }
    if (hybrid && !g_gpu.dn_shared_ok) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the device's shared-memory cap declined the deltanet chain - resident driver declines, the per-op rails serve\n")
        return false
    }
    if (hybrid && !(ensure_dn_step_cls() && ensure_router_gemv_f16_cls())) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the deltanet step class rail declined on this device - resident driver declines, the per-op rails serve\n")
        return false
    }
    // all-or-nothing: pre-flight every prefill-attention class pf_setup may pick for this head size
    // (the fa tile with its cap stamp at 256, the h128 twin, the chunked pair - a capped or gated
    // layer falls through the ladder) so a declined kernel declines residency PRE-upload
    let fa_hs_ok = head_size == 64l || head_size == 128l || head_size == 256l || head_size == 512l
    let will_facm2 = cm2_fa_on() && vk_fa_on() && kv16 && fa_hs_ok
    let will_fakhr = !will_facm2 && khr_kq_tile_on() && vk_fa_on() && kv16 && fa_khr_serves(head_size, 0.0, q_gated)
    var fa_ok = true
    if (will_facm2 || will_fakhr) {
        let gated256 = q_gated && head_size == 256l   // the gated stamp exists at 256 on the cm2 arm alone
        let stamp_ok = fa_stamp_ensure(will_fakhr, head_size, gated256, false, false, false)
        fa_ok = stamp_ok && (head_size != 256l || gated256 || fa_stamp_ensure(will_fakhr, 256l, false, true, false, false))   // an ungated 256 pre-flights its softcap twin
    }
    let chunked_ok = (g_gpu.has_coopmat && head_size == 128l && vk_fa_on()
        ? (kv16 ? ensure_da_attn_b_h128_f16_cls() : ensure_da_attn_b_h128_cls())
        : (kv16 ? ensure_da_attn_b_f16_cls() : ensure_da_attn_b_cls()))
    if (!(fa_ok && chunked_ok)) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the prefill-attention class rail declined on this device - resident driver declines, the per-op rails serve\n")
        return false
    }
    g_rd = new RDec(ready = false, n_layers = n_layers, dim = dim, qd = qd, kv_dim = kv_dim,
                 head_size = head_size, n_heads = n_heads, kv_mul = n_heads / (kv_dim / head_size),
                 hidden = hidden, vocab = vocab, seq_cap = seq_cap, neox = neox, eps = eps, scale = scale,
                 qk_norm = qk_norm, kv16 = kv16, kv_esz = esz)
    var r & = unsafe(*g_rd)
    r.q_gated = q_gated
    r.rot = rot
    rdq_reset()
    g_rdq_idle_us = 0.0lf   //! the idle ledger is this file's: a new model starts it at zero with the averages
    g_rdq_prev_last = 0ul
    r.nb = min(regions, RD_NB_MAX)   //! the per-row planes hold a row per region the N-row command can step at once
    let nb = r.nb
    let wide = max(max(qd, hidden), dim)
    r.x_dev = make_device_buf(dim * 4l * nb)
    r.xb_dev = make_device_buf(dim * 4l * nb)
    r.xq_dev = make_device_buf(wide * nb)              // int8 quants, 1 byte/elem
    r.xs_dev = make_device_buf((wide / 32l) * 4l * nb)
    r.kv_dev = make_device_buf((qd + 2l * kv_dim) * 4l * nb)
    r.q_dev = q_gated ? make_device_buf(2l * qd * 4l * nb) : r.kv_dev   //! an ungated q row is the k/v row's front (its first qd elements): one q+k+v GEMV dispatch lands all three
    r.attn_dev = make_device_buf(qd * 4l * nb)
    r.attn_nsplit = da_nsplit(da_attn_row_wgs(n_heads, n_heads / (kv_dim / head_size), 1l), int64(g_gpu.sm_count))
    r.attn_cnt_off = n_heads * max(r.attn_nsplit, 1l) * (head_size + 2l) * nb   //! the pieces: no dispatch splits past the one-row form's count
    r.attn_part_dev = make_device_buf((r.attn_cnt_off + n_heads * nb) * 4l)   //! a counter a (row, kv head, slab) group: a head a group at most, whatever a layer's GQA group
    upload_zeros(r.attn_part_dev, r.attn_cnt_off * 4l, n_heads * nb * 4l)   //! the arrival counters start at rest; the last piece of each group rearms its own
    r.ar_cnt_dev = make_device_buf(16l)
    upload_zeros(r.ar_cnt_dev, 0l, 16l)   //! the counter starts at zero; every epilogue dispatch leaves it there
    r.aq_dev = make_device_buf(qd * nb)
    r.as_dev = make_device_buf((qd / 32l) * 4l * nb)
    r.gate_dev = make_device_buf(hidden * 4l * nb)
    r.up_dev = make_device_buf(hidden * 4l * nb)
    r.ffnout_dev = make_device_buf(dim * 4l * nb)
    r.xb2_dev = make_device_buf(dim * 4l * nb)
    r.n_attn = n_attn
    r.mir_elems = mir_elems + RDEC_MIR_SLACK
    r.regions = regions
    // the two largest allocations of the load: a heap the last model fragmented can refuse them though the plan fit - decline, never die mid-upload
    r.k_mirror = try_device_buf(rd_mir_bytes())
    r.v_mirror = try_device_buf(rd_mir_bytes())
    if (r.k_mirror == 0ul || r.v_mirror == 0ul) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the KV mirror ({(2l * rd_mir_bytes()) >> 20l} MB for {regions} regions of ctx {seq_cap}) found no room on the device - resident driver declines, the per-op rails serve\n")
        return false
    }
    {   // the mirrors start at zero, never at garbage: a flash fragment straddling a window's end reads the unwritten rows past it - inside a plane and in the slack - and zero times a masked probability stays zero where a garbage NaN would not
        var rawz = alloc_transient_cmd()
        let beginz = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(rawz, beginz), null)
        cmd_fill_zero(rawz, r.k_mirror, 0l, rd_mir_bytes())
        cmd_fill_zero(rawz, r.v_mirror, 0l, rd_mir_bytes())
        vk_check(vkEndCommandBuffer(rawz), null)
        submit_wait(rawz)
        free_transient_cmd(rawz)
    }
    r.hybrid = hybrid
    if (hybrid) {
        r.dn_cd = dn.cd; r.dn_kd = dn.kd; r.dn_di = dn.di; r.dn_nvh = dn.nvh; r.dn_nkh = dn.nkh; r.dn_ds = dn.ds; r.dn_dconv = dn.dconv
        r.dn_ba_f32 = dn.ba_f32
        r.dn_proj_dev = make_device_buf((dn.cd + dn.di) * 4l * nb)
        r.dn_oq_dev = make_device_buf(DND_OQ_BYTES * nb)
        r.dn_os_dev = make_device_buf(DND_OS_BYTES * nb)
        r.dn_of_dev = make_device_buf(DND_OQ_BYTES * 4l * nb)
        r.dn_parity |> resize(regions)
        ensure_dnd_state()   // the staging buffers the state uploads and flushes ride
    }
    // norms layout: [rms_att[l], rms_ffn[l]] x L + rms_final (dim each), then under qk_norm the
    // per-head rows [rms_q[l], rms_k[l]] x L (head_size each) appended past the fixed layout,
    // then the eps/ascale pair the fused ar+requant reads (its binding 4 is the scale output)
    r.ar_scal_off = n_layers * RD_NORM_ROWS * dim + dim + (qk_norm ? n_layers * 2l * head_size : 0l)
    r.norms_bytes = (r.ar_scal_off + 2l) * 4l
    r.norms_dev = make_device_buf(r.norms_bytes)
    // cos layout: this token's cos[half]+sin[half] per rope class, one class's row after the other
    // (the qkn class twin reads its norm rows from the norms buffer directly, so nothing else rides the cos plane)
    r.cos_elems = cos_elems
    r.cos_dev = make_device_buf(r.cos_elems * 4l * nb)   //! a row's rope row each, the N-row command's rows behind the first
    r.log_xfer = g_gpu.xfer_fam >= 0
    r.logits_dev = make_device_buf(vocab * 4l * nb, r.log_xfer)
    r.dummy = make_device_buf(256l)
    r.x_host = make_host_buf(dim * 4l * nb, false, [cached = true])
    r.cos_host = make_host_buf(r.cos_elems * 4l * nb, false, [cached = true])
    verify(int64(typeinfo sizeof(type<TokMeta>)) == TOK_META_BYTES, "vk decode: the TokMeta row and TOK_META_BYTES disagree")
    r.tok_meta = make_host_buf(TOK_META_BYTES * nb, true)   // the one-row command's row is row 0
    r.logits_host = make_host_buf(vocab * 4l * nb, false, [cached = true, xfer_shared = r.log_xfer])
    r.pick_dev = make_device_buf(4l * nb, r.log_xfer)
    r.pick_part_max_dev = make_device_buf(4l * nb * RD_PICK_CHUNKS)
    r.pick_part_id_dev = make_device_buf(4l * nb * RD_PICK_CHUNKS)
    r.pick_host = make_host_buf(4l * nb, false, [cached = true, xfer_shared = r.log_xfer])
    r.xlog_recorded |> resize(nb + 1l)
    r.xlog_pick_recorded |> resize(nb + 1l)
    r.xlog_cmd |> reserve(nb + 1l)
    r.xlog_pick_cmd |> reserve(nb + 1l)
    for (_i in range64(nb + 1l)) {
        r.xlog_cmd |> push(r.log_xfer ? alloc_xfer_cmd() : VkCommandBuffer())
        r.xlog_pick_cmd |> push(r.log_xfer ? alloc_xfer_cmd() : VkCommandBuffer())
    }
    r.unsplit_on = r.attn_nsplit > 1l   //! the twins' availability is the device's, decided where their buffers are allocated: never a record's side effect
    r.wide_on = r.attn_nsplit > RD_SPLIT_PIECES
    r.tok_n_recorded |> resize(nb + 1l)
    r.cmd_n |> reserve(nb + 1l)
    r.cmd_n_unsplit |> reserve(nb + 1l)
    for (_i in range64(nb + 1l)) {
        r.cmd_n |> push(alloc_cmd())
        r.cmd_n_unsplit |> push(r.unsplit_on ? alloc_cmd() : VkCommandBuffer())
    }
    r.kv_readback = make_host_buf(2l * kv_dim * 4l * KV_READBACK_ROWS, false, [cached = true])
    ensure_ffn_state()   // hq_dev/hs_dev for the FFN act+requant
    r.tok_recorded |> resize(regions)
    r.cmd |> reserve(regions)
    r.cmd_unsplit |> resize(regions)
    r.tok_wide_recorded |> resize(regions)
    r.cmd_wide |> resize(regions)
    for (ri in range64(regions)) {
        r.cmd |> push(alloc_cmd())
        if (r.unsplit_on) {
            r.cmd_unsplit[ri] = alloc_cmd()
        }
        if (r.wide_on) {
            r.cmd_wide[ri] = alloc_cmd()
        }
    }
    return true
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]
def vk_rdec_select_region(region : int64) {
    assert(g_rd != null && region >= 0l && region < g_rd.regions, "vk_rdec_select_region: no such mirror region")
    g_rd.region = region
}

//! Upload the norm-weight rows once: [rms_att[0], rms_ffn[0], ..., rms_att[L-1], rms_ffn[L-1],
//! rms_final], dim floats each, exactly the layout the layer w_offs index. Under qk_norm the
//! per-head [rms_q[l], rms_k[l]] x L rows (head_size floats each) follow.
def vk_rdec_upload_norms(norms : array<float>; obias : bool) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null, "vk_rdec_upload_norms before prepare")
    g_rd.has_obias = obias
    upload_region_at(g_rd.norms_dev, 0l, unsafe(addr<uint8 const?>(norms[0])), long_length(norms) * 4l)
    // the fused-rail constants: eps/ascale past the norm rows (qk_norm rows serve in place — the
    // qkn class twin binds the norms buffer directly)
    var pair : float[2]
    pair[0] = g_rd.eps
    pair[1] = 1.0   // ascale — the residual add is unscaled in the decode driver
    upload_region_at(g_rd.norms_dev, g_rd.ar_scal_off * 4l, unsafe(addr<uint8 const?>(pair[0])), 8l)
    // norms are the last upload of the resident arm sequence — report the accumulated cost
    vk_upload_report(key_exists(g_gpu.dev_mapped, g_rd.norms_dev))
}

def private rd_ffn_sets(var L : RLayer; b1, b3, b2 : int64; f1, f3, f2 : int) {
    let dim = g_rd.dim
    let hid = L.hid
    let nb = g_rd.nb
    let p1 = arena_planes(f1, b1)
    let p3 = arena_planes(f3, b3)
    let p2 = arena_planes(f2, b2)
    L.b1 = b1; L.b3 = b3; L.b2 = b2
    L.f1 = f1; L.f3 = f3; L.f2 = f2
    L.m_gate = rd_meta(); L.m_up = rd_meta(); L.m_down = rd_meta()
    // addrms (class): x += xb2, norms, xb out — and the fused twin quantizing in place of xb
    L.s_addr_ffn = set_cls_ar(fixed_array(g_rd.x_dev, g_rd.xb2_dev, g_rd.norms_dev, g_rd.xb_dev),
        fixed_array(dim * 4l * nb, dim * 4l * nb, g_rd.norms_bytes, dim * 4l * nb),
        fixed_array(VHZ_X, VHZ_XB2, 0u, VHZ_XB))
    rd_unmake_sets()
    // quantize xb (post-ffn-norm) -> xq/xs
    L.s_quant_xb2 = rd_quant_xb_set()
    // gate/up GEMV
    L.s_gate = gemv_cls_set(f1, fixed_array(p1.wq, p1.ws, L.m_gate.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.gate_dev),
        fixed_array(p1.wqb, p1.wsb, META_BYTES, dim * nb, (dim / 32l) * 4l * nb, hid * 4l * nb),
        fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_GATE))
    L.s_up = gemv_cls_set(f3, fixed_array(p3.wq, p3.ws, L.m_up.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.up_dev),
        fixed_array(p3.wqb, p3.wsb, META_BYTES, dim * nb, (dim / 32l) * 4l * nb, hid * 4l * nb),
        fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_UP))
    // act+requant (class): gate@0, up@1, hq@2, hs@3
    L.s_actrq = set_actrq_cls(fixed_array(g_rd.gate_dev, g_rd.up_dev, g_gpu.hq_dev, g_gpu.hs_dev),
        fixed_array(hid * 4l * nb, hid * 4l * nb, BATCH_HQ_BYTES, BATCH_HS_BYTES),
        fixed_array(VHZ_GATE, VHZ_UP, VHZ_HQ, VHZ_HQ))
    //! the fused form: gate and up q8 planes over the Q8_0 x, the hidden row's Q8_0 quants out (DASLLAMA_VK_FUSE=0 keeps the three dispatches)
    L.gu_on = f1 == int(KqFmt.q8) && f3 == int(KqFmt.q8) && !kq_sb(f2) && vk_fuse_on() && (vk_fuse_bisect_bits() & FUSE_BX_GU) == 0l
    if (L.gu_on) {
        verify(ensure_q8_gemv_gu_cls(), "vk decode: the fused gate-up class rail must engage on a live device")
        L.s_gu = set_q8_gemv_gu_cls(fixed_array(p1.wq, p1.ws, p3.wq, p3.ws, g_rd.xq_dev, g_rd.xs_dev, g_gpu.hq_dev, g_gpu.hs_dev),
            fixed_array(p1.wqb, p1.wsb, p3.wqb, p3.wsb, dim, (dim / 32l) * 4l, BATCH_HQ_BYTES, BATCH_HS_BYTES),
            fixed_array(0u, 0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_HQ, VHZ_HQ))
    }
    // down GEMV -> ffnout
    L.s_down = gemv_cls_set(f2, fixed_array(p2.wq, p2.ws, L.m_down.buf, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.ffnout_dev),
        fixed_array(p2.wqb, p2.wsb, META_BYTES, hid * nb, (hid / 32l) * 4l * nb, dim * 4l * nb),
        fixed_array(0u, 0u, 0u, VHZ_HQ, VHZ_HQ, VHZ_FFO))
    // addrms next: x += ffnout, xb = rms_att[l+1]|final — the fused twin also serves the last layer's final norm
    L.s_addr_next = set_cls_ar(fixed_array(g_rd.x_dev, g_rd.ffnout_dev, g_rd.norms_dev, g_rd.xb_dev),
        fixed_array(dim * 4l * nb, dim * 4l * nb, g_rd.norms_bytes, dim * 4l * nb),
        fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB))
}

def private rd_quant_xb_set() : VkDescriptorSet {
    let dim = g_rd.dim
    let nb = g_rd.nb
    return set_rq_cls(fixed_array(g_rd.xb_dev, g_rd.xq_dev, g_rd.xs_dev),
        fixed_array(dim * 4l * nb, dim * nb, (dim / 32l) * 4l * nb),
        fixed_array(VHZ_XB, VHZ_XQ, VHZ_XQ))
}

def private rd_layer_slot(l : int64) {
    while (long_length(g_rd.layers) <= l) {
        g_rd.layers |> emplace(RLayer())
    }
}

//! a layer's attention geometry, from the resident plan's per-layer record
def private rd_layer_geom(var L : RLayer; g : RdecLayerGeom) {
    L.hs = g.hs; L.qd = g.qd; L.kvd = g.kvd; L.hid = g.hid; L.rot = g.rot
    L.kv_mul = g.kvd > 0l ? g_rd.n_heads / (g.kvd / g.hs) : 1l
    L.pre_post = g.pre_post
    L.gelu = g.gelu
    L.vfk = g.vfk
    L.vnorm = g.vnorm
    L.out_scale = g.out_scale
    L.kv_shared = g.kv_shared
    L.attn_cap = g.attn_cap
    L.moe_g4 = g.moe_g4
    L.window = g.window
    L.mir_base = g.mir_base
    L.cbase = g.cbase
    L.boff = g.boff
}

//! Register a dense layer's arena block indices + formats and build its descriptor sets. Called
//! once per layer after prepare. `mir_idx` is the layer's K/V mirror slot (attention layers only).
def vk_rdec_set_layer(l, mir_idx : int64; bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int; g : RdecLayerGeom) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null, "vk_rdec_set_layer before prepare")
    rd_layer_slot(l)
    var L & = unsafe(g_rd.layers[l])
    rd_layer_geom(L, g)
    rd_attn_sets(L, mir_idx, bq, bk, bv, bo, fq, fk, fv, fo, f1, f3, f2)
    rd_ffn_sets(L, b1, b3, b2, f1, f3, f2)
    L.made = true
    rd_invalidate_token()   //! the recorded chain references the replaced sets
}

//! the attention half of a layer's registration: the quad's blocks, formats and sets (f1 / f3 / f2
//! name the layer's FFN feed forms for the class-rail check); the layer's geometry is set already
def private rd_attn_sets(var L : RLayer; mir_idx : int64; bq, bk, bv, bo : int64; fq, fk, fv, fo, f1, f3, f2 : int) {
    unsafe {
        L.recurrent = false
        L.mir_idx = mir_idx
        L.bq = bq; L.bk = bk; L.bv = bv; L.bo = bo
        L.fq = fq; L.fk = fk; L.fv = fv; L.fo = fo
        let dim = g_rd.dim
        let qd = L.qd
        let kvd = L.kvd
        let nb = g_rd.nb
        let pq = arena_planes(fq, bq)
        let pk = L.kv_shared ? pq : arena_planes(fk, bk)   //! a shared-KV layer has no k/v planes: the q planes fill the slots, the k/v sets stay unused
        let pv = (L.vfk || L.kv_shared) ? pk : arena_planes(fv, bv)   //! no v plane: the k planes fill the slot, the v set stays unused
        let po = arena_planes(fo, bo)
        let anykq = kq_sb(fq) || kq_sb(fo) || kq_sb(f1) || kq_sb(f2)   //! kq consumers take the Q8_K quant/act forms
        let kv16 = g_rd.kv16
        verify(ensure_cls_dn_rq() && ensure_cls_ar() && ensure_cls_ar_rq() && ensure_q8_actrq_cls() && ensure_q8_gemv_gu_cls()
            && (kv16 ? ensure_rope_kv_f16_cls() : ensure_rope_kv_cls())
            && (kv16 ? ensure_da_attn_f16_cls() : ensure_da_attn_cls())
            && (g_rd.has_sinks || !da_slab_is_g2(L.kv_mul) || (kv16 ? ensure_da_attn_f16_g2_cls() : ensure_da_attn_g2_cls()))
            && (!g_rd.qk_norm || (ensure_qk_rms_cls()
                && (kv16 ? ensure_qkn_rope_f16_cls() : ensure_qkn_rope_cls())))
            && (!anykq || (ensure_cls_q8k_rq() && ensure_q8k_actrq_cls()))
            && gemv_cls_ensure(fq) && (L.kv_shared || (gemv_cls_ensure(fk) && (L.vfk || gemv_cls_ensure(fv)))) && gemv_cls_ensure(fo)
            && gemv_cls_ensure(f1) && gemv_cls_ensure(f3) && gemv_cls_ensure(f2),
            "vk decode: the class rails must engage on a live device")
        L.m_q = rd_meta(); L.m_k = rd_meta(); L.m_v = rd_meta(); L.m_wo = rd_meta()
        L.s_quant_xb = rd_quant_xb_set()
        let qbytes = (g_rd.q_gated ? 2l * qd : qd) * 4l
        let qplane = g_rd.q_gated ? qbytes * nb : (qd + 2l * kvd) * 4l * nb   //! the q rows' plane: an ungated row's q sits in its projection row, a row every `qd + 2 kvd`
        // q/k/v class GEMV: planes @0/1, regions@2, xq@3, xs@4, out@5
        L.s_q = gemv_cls_set(fq, fixed_array(pq.wq, pq.ws, L.m_q.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.q_dev),
            fixed_array(pq.wqb, pq.wsb, META_BYTES, dim * nb, (dim / 32l) * 4l * nb, qplane),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_Q))
        if (!L.kv_shared) {
            L.s_k = gemv_cls_set(fk, fixed_array(pk.wq, pk.ws, L.m_k.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev),
                fixed_array(pk.wqb, pk.wsb, META_BYTES, dim * nb, (dim / 32l) * 4l * nb, (qd + 2l * kvd) * 4l * nb),
                fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVK))
        }
        if (!L.vfk && !L.kv_shared) {
            L.s_v = gemv_cls_set(fv, fixed_array(pv.wq, pv.ws, L.m_v.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev),
                fixed_array(pv.wqb, pv.wsb, META_BYTES, dim * nb, (dim / 32l) * 4l * nb, (qd + 2l * kvd) * 4l * nb),
                fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVV))
        }
        L.kv_merged = !L.vfk && !L.kv_shared && fk == fv && arena_slab_idx(bk) == arena_slab_idx(bv)   //! same-format k/v in one slab: one two-region dispatch fills both halves
        if (L.kv_merged) {
            L.s_kv = gemv_cls_set(fk, fixed_array(pk.wq, pk.ws, L.m_k.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev),
                fixed_array(pk.wqb, pk.wsb, META_BYTES, dim * nb, (dim / 32l) * 4l * nb, (qd + 2l * kvd) * 4l * nb),
                fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVK | VHZ_KVV))
        }
        L.qkv_merged = (vk_fuse_on() && !g_rd.q_gated && !L.kv_shared && fq == fk && (L.vfk || fq == fv) && qd % kvd == 0l
            && arena_slab_idx(bq) == arena_slab_idx(bk) && (L.vfk || arena_slab_idx(bq) == arena_slab_idx(bv)))   //! q, k (and v) of one format in one slab: one dispatch of kvd-row regions fills the whole row
        if (L.qkv_merged) {
            L.s_qkv = gemv_cls_set(fq, fixed_array(pq.wq, pq.ws, L.m_q.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev),
                fixed_array(pq.wqb, pq.wsb, META_BYTES, dim * nb, (dim / 32l) * 4l * nb, (qd + 2l * kvd) * 4l * nb),
                fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_Q | VHZ_KVK | VHZ_KVV))
        }
        let mirbytes = rd_mir_bytes()
        let cosbytes = g_rd.cos_elems * 4l
        let bias_bind = rd_bias_bind()
        if (g_rd.qk_norm) {
            // per-head q/k rmsnorm (class): q rows, k rows (kv_dev — kbase=qd rides the push), norms
            L.s_qkn = set_qk_rms_cls(fixed_array(g_rd.q_dev, g_rd.kv_dev, g_rd.norms_dev),
                fixed_array(qplane, (qd + 2l * kvd) * 4l * nb, g_rd.norms_bytes),
                fixed_array(VHZ_Q, VHZ_KVK, 0u))
            // the fused twin (class): norm rows bind the norms buffer, mirror row rides TokMeta, the bias rows their buffer
            let qrbufs = fixed_array(g_rd.q_dev, g_rd.kv_dev, g_rd.k_mirror, g_rd.v_mirror, g_rd.cos_dev, g_rd.norms_dev, g_rd.tok_meta.buf, bias_bind.buf)
            let qrsizes = fixed_array(qplane, (qd + 2l * kvd) * 4l * nb, mirbytes, mirbytes, cosbytes * nb, g_rd.norms_bytes, TOK_META_BYTES * nb, bias_bind.bytes)
            let qrbits = fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, VHZ_MIR, VHZ_MIR, VHZ_COS, 0u, 0u, 0u)
            L.s_qkn_rope = (kv16 ? set_qkn_rope_f16_cls(qrbufs, qrsizes, qrbits)
                : set_qkn_rope_cls(qrbufs, qrsizes, qrbits))
        }
        // rope+store (class): per-token mirror row rides TokMeta, the bias rows their buffer
        let rbufs = fixed_array(g_rd.q_dev, g_rd.kv_dev, g_rd.k_mirror, g_rd.v_mirror, g_rd.cos_dev, g_rd.tok_meta.buf, bias_bind.buf)
        let rsizes = fixed_array(qplane, (qd + 2l * kvd) * 4l * nb, mirbytes, mirbytes, cosbytes * nb, TOK_META_BYTES * nb, bias_bind.bytes)
        let rbits = fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, VHZ_MIR, VHZ_MIR, VHZ_COS, 0u, 0u)
        L.s_rope = kv16 ? set_rope_kv_f16_cls(rbufs, rsizes, rbits) : set_rope_kv_cls(rbufs, rsizes, rbits)
        // attn (class): per-token cnt rides TokMeta, the scale rides the push
        let partbytes = (g_rd.attn_cnt_off + g_rd.n_heads * nb) * 4l
        //! the partials ride the attention-out bit (written and read within the pass); the folded requant lands in the wo feed's aq/as
        if (g_rd.has_sinks) {   //! the sink twins bind the sink plane past the pass's eight
            let abufs = fixed_array(g_rd.q_dev, g_rd.k_mirror, g_rd.v_mirror, g_rd.attn_dev, g_rd.tok_meta.buf, g_rd.attn_part_dev, g_rd.aq_dev, g_rd.as_dev, g_rd.sinks_dev)
            let asizes = fixed_array(qplane, mirbytes, mirbytes, qd * 4l * nb, TOK_META_BYTES * nb, partbytes, qd * nb, (qd / 32l) * 4l * nb, g_rd.sinks_bytes)
            let abits = fixed_array(VHZ_Q, VHZ_MIR, VHZ_MIR, VHZ_ATT, 0u, VHZ_ATT, VHZ_AQ, VHZ_AQ, 0u)
            L.s_attn = kv16 ? set_da_attn_f16_sink_cls(abufs, asizes, abits) : set_da_attn_sink_cls(abufs, asizes, abits)
        } else {
            let abufs = fixed_array(g_rd.q_dev, g_rd.k_mirror, g_rd.v_mirror, g_rd.attn_dev, g_rd.tok_meta.buf, g_rd.attn_part_dev, g_rd.aq_dev, g_rd.as_dev)
            let asizes = fixed_array(qplane, mirbytes, mirbytes, qd * 4l * nb, TOK_META_BYTES * nb, partbytes, qd * nb, (qd / 32l) * 4l * nb)
            let abits = fixed_array(VHZ_Q, VHZ_MIR, VHZ_MIR, VHZ_ATT, 0u, VHZ_ATT, VHZ_AQ, VHZ_AQ)
            L.s_attn = kv16 ? set_da_attn_f16_cls(abufs, asizes, abits) : set_da_attn_cls(abufs, asizes, abits)
        }
        // quantize attn -> aq/as
        L.s_quant_at = set_rq_cls(fixed_array(g_rd.attn_dev, g_rd.aq_dev, g_rd.as_dev),
            fixed_array(qd * 4l * nb, qd * nb, (qd / 32l) * 4l * nb),
            fixed_array(VHZ_ATT, VHZ_AQ, VHZ_AQ))
        // wo GEMV -> xb2
        L.s_wo = gemv_cls_set(fo, fixed_array(po.wq, po.ws, L.m_wo.buf, g_rd.aq_dev, g_rd.as_dev, g_rd.xb2_dev),
            fixed_array(po.wqb, po.wsb, META_BYTES, qd * nb, (qd / 32l) * 4l * nb, dim * 4l * nb),
            fixed_array(0u, 0u, 0u, VHZ_AQ, VHZ_AQ, VHZ_XB2))
    }
}

// ===== the MoE form: the router plane, the routed block of the token command =====

let private MOE_MAX_EXPERTS = 256l   //! the select kernels' one-workgroup reach: one thread per expert

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def vk_rdec_prepare_moe(ne, k, nfe, nsh, n_moe : int64; sh_gated, norm_topk, is_gelu, act_oai : bool; wscale : float; router : array<float>; dscale : array<float>;
                        rbias : array<float>; gbias : array<float>; ubias : array<float>; dbias : array<float>) : bool {
    let ner = ne + (nsh > 0l && sh_gated ? 1l : 0l)
    let reach = ne <= MOE_MAX_EXPERTS && k <= MAX_ROUTED_SLOTS && nfe % 32l == 0l
    if (g_gpu != null && g_gpu.dry) {
        return reach   //! the reach IS the plan decision; everything below is device state
    }
    assert(g_rd != null, "vk_rdec_prepare_moe before prepare")
    let dim = g_rd.dim
    if (!reach || k * max(nfe, dim) * 4l > Y_BYTES || dim % 64l != 0l) {   //! the router tile steps K by 64
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the MoE geometry ({ne} experts, {k} routed, expert width {nfe}, dim {dim}) is outside the routing kernels' reach\n")
        return false
    }
    if (n_moe * ner * dim * 4l > vk_max_storage_range()) {   //! the router plane binds as one range
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the router plane ({n_moe} layers x {ner} rows x {dim}) passes the device's storage range {vk_max_storage_range()}\n")
        return false
    }
    var r & = unsafe(*g_rd)
    r.moe = true
    r.ne = ne
    r.k = k
    r.nfe = nfe
    r.nsh = nsh
    r.n_moe = n_moe
    r.ner = ner
    r.sh_gated = sh_gated
    r.norm_topk = norm_topk
    r.is_gelu = is_gelu
    r.act_oai = act_oai
    r.wscale = wscale
    r.router_bias = !empty(rbias)
    r.exps_bias = !empty(gbias)
    if (!(ensure_router_gemv_cls() && ensure_topk_cls() && ensure_cls_ar_comb()
            && ensure_q8_actrq_cls() && ensure_q8k_actrq_cls() && (!r.exps_bias || ensure_q8_actrq_b_cls()))) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the routing class rails declined on this device\n")
        return false
    }
    r.moe_rb_dev = r.dummy
    r.moe_rb_bytes = 256l
    if (r.router_bias) {
        assert(long_length(rbias) == n_moe * ne, "vk_rdec_prepare_moe: the router-bias plane is not [n_moe x ne]")
        r.moe_rb_bytes = long_length(rbias) * 4l
        r.moe_rb_dev = make_device_buf(r.moe_rb_bytes)
        upload_region_at(r.moe_rb_dev, 0l, unsafe(addr<uint8 const?>(rbias[0])), r.moe_rb_bytes)
    }
    r.moe_gb_dev = r.dummy
    r.moe_ub_dev = r.dummy
    r.moe_gub_bytes = 256l
    if (!r.exps_bias) {   //! the combine reads the down-bias row unconditionally at row base 0: a biasless model binds a ZERO ROW of the model dim, never the one-float dummy
        r.moe_db_bytes = dim * 4l
        r.moe_db_dev = make_device_buf(r.moe_db_bytes)
        var zrow : array<float>
        zrow |> reserve(dim)
        zrow |> resize(dim)
        upload_region_at(r.moe_db_dev, 0l, unsafe(addr<uint8 const?>(zrow[0])), r.moe_db_bytes)
        delete zrow
    } else {
        assert(long_length(gbias) == n_moe * ne * nfe && long_length(ubias) == n_moe * ne * nfe && long_length(dbias) == n_moe * ne * dim,
            "vk_rdec_prepare_moe: the expert-bias planes are not [n_moe x ne x nfe] / [n_moe x ne x dim]")
        if (max(long_length(gbias), long_length(dbias)) * 4l > vk_max_storage_range()) {   //! each bias plane binds as one range
            to_log(LOG_WARNING, "dasLLAMA vulkan resident: an expert-bias plane ({n_moe} layers x {ne} experts x {max(nfe, dim)}) passes the device's storage range {vk_max_storage_range()}\n")
            return false
        }
        r.moe_gub_bytes = long_length(gbias) * 4l
        r.moe_db_bytes = long_length(dbias) * 4l
        r.moe_gb_dev = make_device_buf(r.moe_gub_bytes)
        r.moe_ub_dev = make_device_buf(r.moe_gub_bytes)
        r.moe_db_dev = make_device_buf(r.moe_db_bytes)
        upload_region_at(r.moe_gb_dev, 0l, unsafe(addr<uint8 const?>(gbias[0])), r.moe_gub_bytes)
        upload_region_at(r.moe_ub_dev, 0l, unsafe(addr<uint8 const?>(ubias[0])), r.moe_gub_bytes)
        upload_region_at(r.moe_db_dev, 0l, unsafe(addr<uint8 const?>(dbias[0])), r.moe_db_bytes)
    }
    r.moe_eidx_dev = make_device_buf(MAX_ROUTED_SLOTS * 4l)
    r.router_bytes = long_length(router) * 4l
    assert(r.router_bytes == n_moe * r.ner * dim * 4l, "vk_rdec_prepare_moe: the router plane is not [n_moe x ner x dim]")
    r.router_dev = make_device_buf(r.router_bytes)
    upload_region_at(r.router_dev, 0l, unsafe(addr<uint8 const?>(router[0])), r.router_bytes)
    if (f16g_shape_ok(dim)) {   //! the f16 mirror serves the cm2 route's window GEMM alone
        var half : array<float16>
        half |> reserve(long_length(router))
        half |> resize(long_length(router))
        for (v, hv in router, half) {
            hv = float16(v)
        }
        r.router16_bytes = long_length(half) * 2l
        r.router16_dev = make_device_buf(r.router16_bytes)
        upload_region_at(r.router16_dev, 0l, unsafe(addr<uint8 const?>(half[0])), r.router16_bytes)
        delete half
    }
    if (empty(dscale)) {
        r.moe_ds_dev = r.dummy
        r.moe_ds_bytes = 256l
    } else {
        assert(long_length(dscale) == n_moe * ne, "vk_rdec_prepare_moe: the down-scale plane is not [n_moe x ne]")
        r.moe_ds_bytes = long_length(dscale) * 4l
        r.moe_ds_dev = make_device_buf(r.moe_ds_bytes)
        upload_region_at(r.moe_ds_dev, 0l, unsafe(addr<uint8 const?>(dscale[0])), r.moe_ds_bytes)
    }
    r.moe_ident_dev = make_device_buf(MAX_ROUTED_SLOTS * 4l)
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.staging.mapped)
        for (i in range64(MAX_ROUTED_SLOTS)) {
            mp[i] = uint(i)
        }
        staged_upload(r.moe_ident_dev, 0l, MAX_ROUTED_SLOTS * 4l)
    }
    let nb = r.nb
    r.moe_dlog_dev = make_device_buf(r.ner * 4l * nb)
    r.moe_dw_dev = make_device_buf(MAX_ROUTED_SLOTS * 4l)
    r.moe_meta1_dev = make_device_buf(META_BYTES)
    r.moe_meta3_dev = make_device_buf(META_BYTES)
    r.moe_meta2_dev = make_device_buf(META_BYTES)
    r.moe_xq_dev = make_device_buf(dim * nb)
    r.moe_xs_dev = make_device_buf((dim / 32l) * 4l * nb)
    r.egate_dev = make_device_buf(k * nfe * 4l * nb)
    r.eup_dev = make_device_buf(k * nfe * 4l * nb)
    r.edown_dev = make_device_buf(k * dim * 4l * nb)
    return true
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def private rd_moe_sets(var L : RLayer) {
    let dim = g_rd.dim
    let nb = g_rd.nb
    let nfe = g_rd.nfe
    let k = g_rd.k
    let pe1 = arena_planes(L.fe1, L.be1)
    let pe3 = arena_planes(L.fe3, L.be3)
    let pe2 = arena_planes(L.fe2, L.be2)
    verify(gemv_cls_ensure(L.fe1) && gemv_cls_ensure(L.fe3) && gemv_cls_ensure(L.fe2)
        && (kq_sb(L.fe1) ? ensure_cls_q8k_rq() : ensure_cls_dn_rq())
        && (kq_sb(L.fe2) ? ensure_q8k_actrq_cls() : ensure_q8_actrq_cls()),
        "vk decode: the expert class rails must engage on a live device")
    L.xe_own = has_dense_triple(L) && (kq_sb(L.fe1) != kq_sb(L.f1) || L.moe_g4)   //! the gemma-4 routed feed is its own norm of x
    let xqb = L.xe_own ? g_rd.moe_xq_dev : g_rd.xq_dev
    let xsb = L.xe_own ? g_rd.moe_xs_dev : g_rd.xs_dev
    let xq_bytes = dim * nb
    let xs_bytes = (dim / 32l) * 4l * nb
    let rows_bytes = dim * 4l * nb
    let egu_bytes = nb * k * nfe * 4l
    let edn_bytes = nb * k * dim * 4l
    let slots_bytes = MAX_ROUTED_SLOTS * 4l
    if (L.xe_own) {
        L.s_quant_xe = set_rq_cls(fixed_array(g_rd.xb_dev, g_rd.moe_xq_dev, g_rd.moe_xs_dev),
            fixed_array(rows_bytes, xq_bytes, xs_bytes), fixed_array(VHZ_XB, VHZ_XQ, VHZ_XQ))
    }
    L.pre2_fused = L.moe_g4 && L.xe_own && vk_fuse_on()
    if (L.pre2_fused) {
        L.pre2_stamp = rq_stamp(L.fe1, false)
        verify(ensure_ar_rq_stamp(L.pre2_stamp), "vk decode: the routed feed's fused norm+requant must engage on a live device")
        L.s_pre2_rq = set_ar_rq_stamp(L.pre2_stamp, g_rd.x_dev, VHZ_X, g_rd.moe_xq_dev, g_rd.moe_xs_dev)   //! add off: the aa slot binds x itself
    }
    L.rtn_fused = L.moe_g4 && vk_fuse_on()
    L.s_router = set_router_gemv_cls(fixed_array(g_rd.router_dev, L.rtn_fused ? g_rd.x_dev : g_rd.xb_dev, g_rd.moe_dlog_dev, g_rd.norms_dev),
        fixed_array(g_rd.router_bytes, rows_bytes, g_rd.ner * 4l * nb, g_rd.norms_bytes), fixed_array(0u, L.rtn_fused ? VHZ_X : VHZ_XB, VHZ_MRT, 0u))
    L.s_topk = set_topk_cls(fixed_array(g_rd.moe_dlog_dev, g_rd.moe_meta1_dev, g_rd.moe_meta3_dev, g_rd.moe_meta2_dev, g_rd.moe_dw_dev, g_rd.moe_ds_dev, g_rd.moe_rb_dev, g_rd.moe_eidx_dev),
        fixed_array(g_rd.ner * 4l * nb, META_BYTES, META_BYTES, META_BYTES, slots_bytes, g_rd.moe_ds_bytes, g_rd.moe_rb_bytes, slots_bytes),
        fixed_array(VHZ_MRT, VHZ_MRT, VHZ_MRT, VHZ_MRT, VHZ_MRT, 0u, 0u, VHZ_MRT))
    L.s_egate = gemv_cls_set(L.fe1, fixed_array(pe1.wq, pe1.ws, g_rd.moe_meta1_dev, xqb, xsb, g_rd.egate_dev),
        fixed_array(pe1.wqb, pe1.wsb, META_BYTES, xq_bytes, xs_bytes, egu_bytes),
        fixed_array(0u, 0u, VHZ_MRT, VHZ_XQ, VHZ_XQ, VHZ_GATE))
    L.s_eup = gemv_cls_set(L.fe3, fixed_array(pe3.wq, pe3.ws, g_rd.moe_meta3_dev, xqb, xsb, g_rd.eup_dev),
        fixed_array(pe3.wqb, pe3.wsb, META_BYTES, xq_bytes, xs_bytes, egu_bytes),
        fixed_array(0u, 0u, VHZ_MRT, VHZ_XQ, VHZ_XQ, VHZ_UP))
    if (g_rd.exps_bias) {   //! the biased act: the picks' gate and up bias rows join before the activation
        L.s_eact = set_q8_actrq_b_cls(fixed_array(g_rd.egate_dev, g_rd.eup_dev, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.moe_gb_dev, g_rd.moe_ub_dev, g_rd.moe_eidx_dev),
            fixed_array(egu_bytes, egu_bytes, BATCH_HQ_BYTES, BATCH_HS_BYTES, g_rd.moe_gub_bytes, g_rd.moe_gub_bytes, slots_bytes),
            fixed_array(VHZ_GATE, VHZ_UP, VHZ_HQ, VHZ_HQ, 0u, 0u, VHZ_MRT))
    } else {
        L.s_eact = set_actrq_cls(fixed_array(g_rd.egate_dev, g_rd.eup_dev, g_gpu.hq_dev, g_gpu.hs_dev),
            fixed_array(egu_bytes, egu_bytes, BATCH_HQ_BYTES, BATCH_HS_BYTES),
            fixed_array(VHZ_GATE, VHZ_UP, VHZ_HQ, VHZ_HQ))
    }
    //! one slab: the gate and up stacks share the quant buffer the fused dispatch binds once
    L.egu = (L.fe1 == int(KqFmt.k4) && L.fe3 == int(KqFmt.k4) && !kq_sb(L.fe2) && nfe % 32l == 0l && pe1.wq == pe3.wq
        && !g_rd.exps_bias && !g_rd.act_oai && vk_fuse_on() && ensure_kq_gemv_k4_gu_cls())   //! the k4 twin carries no bias rows and no clamped act: a biased or gpt-oss-activated k4 stack takes the three dispatches
    if (L.egu) {
        L.s_egu = set_kq_gemv_k4_gu_cls(fixed_array(pe1.wq, pe1.ws, g_rd.moe_meta1_dev, xqb, xsb, g_rd.egate_dev, kq_grid_dev(), g_rd.moe_meta3_dev, g_gpu.hq_dev, g_gpu.hs_dev),
            fixed_array(pe1.wqb, pe1.wsb, META_BYTES, dim, (dim / 32l) * 4l, k * nfe * 4l, KQ_GRID_BYTES, META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES),
            fixed_array(0u, 0u, VHZ_MRT, VHZ_XQ, VHZ_XQ, VHZ_GATE, 0u, VHZ_MRT, VHZ_HQ, VHZ_HQ))
    }
    //! the mx4 twin (gpt-oss): the gate and up stacks in one slab, the biased act and the Q8_0 requant in the same dispatch
    L.egu_mx4 = (L.fe1 == int(KqFmt.mx4) && L.fe3 == int(KqFmt.mx4) && !kq_sb(L.fe2) && nfe % 32l == 0l && pe1.wq == pe3.wq
        && vk_fuse_on() && ensure_mx4_gemv_gu_cls())
    if (L.egu_mx4) {
        L.s_egu = set_mx4_gemv_gu_cls(fixed_array(pe1.wq, pe1.ws, g_rd.moe_meta1_dev, xqb, xsb, g_rd.egate_dev, g_rd.moe_meta3_dev, g_rd.moe_gb_dev, g_rd.moe_ub_dev, g_rd.moe_eidx_dev, g_gpu.hq_dev, g_gpu.hs_dev),
            fixed_array(pe1.wqb, pe1.wsb, META_BYTES, dim, (dim / 32l) * 4l, k * nfe * 4l, META_BYTES, g_rd.moe_gub_bytes, g_rd.moe_gub_bytes, MAX_ROUTED_SLOTS * 4l, BATCH_HQ_BYTES, BATCH_HS_BYTES),
            fixed_array(0u, 0u, VHZ_MRT, VHZ_XQ, VHZ_XQ, VHZ_GATE, VHZ_MRT, 0u, 0u, VHZ_MRT, VHZ_HQ, VHZ_HQ))
    }
    L.s_edown = gemv_cls_set(L.fe2, fixed_array(pe2.wq, pe2.ws, g_rd.moe_meta2_dev, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.edown_dev),
        fixed_array(pe2.wqb, pe2.wsb, META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, edn_bytes),
        fixed_array(0u, 0u, VHZ_MRT, VHZ_HQ, VHZ_HQ, VHZ_MDN))
    L.edown_sum = L.moe_g4 && L.fe2 == int(KqFmt.q51) && vk_fuse_on() && ensure_q51_gemv_sum_cls()
    if (L.edown_sum) {
        L.s_edown_sum = set_q51_gemv_sum_cls(fixed_array(pe2.wq, pe2.ws, g_rd.moe_meta2_dev, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.edown_dev, g_rd.moe_dw_dev),
            fixed_array(pe2.wqb, pe2.wsb, META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, k * dim * 4l, slots_bytes),
            fixed_array(0u, 0u, VHZ_MRT, VHZ_HQ, VHZ_HQ, VHZ_MDN, VHZ_MRT))
    }
    if (L.moe_g4) {
        verify(ensure_cls_ar_comb_g4(), "vk decode: the gemma-4 combine-folded residual class rail must engage on a live device")
        L.s_addr_next = set_cls_ar_comb_g4(fixed_array(g_rd.x_dev, g_rd.ffnout_dev, g_rd.norms_dev, g_rd.xb_dev,
                g_rd.moe_ident_dev, g_rd.moe_dw_dev, g_rd.edown_dev),
            fixed_array(rows_bytes, rows_bytes, g_rd.norms_bytes, rows_bytes, slots_bytes, slots_bytes, edn_bytes),
            fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB, 0u, VHZ_MRT, VHZ_MDN))
        return
    }
    verify(ensure_cls_ar_comb(), "vk decode: the combine-folded residual class rail must engage on a live device")
    L.s_addr_next = set_cls_ar_comb(fixed_array(g_rd.x_dev, g_rd.ffnout_dev, g_rd.norms_dev, g_rd.xb_dev,
            g_rd.moe_ident_dev, g_rd.moe_dw_dev, g_rd.edown_dev, g_rd.moe_dlog_dev, g_rd.moe_db_dev, g_rd.moe_eidx_dev),
        fixed_array(rows_bytes, rows_bytes, g_rd.norms_bytes, rows_bytes, slots_bytes, slots_bytes, edn_bytes, g_rd.ner * 4l * nb, g_rd.moe_db_bytes, slots_bytes),
        fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB, 0u, VHZ_MRT, VHZ_MDN, VHZ_MRT, 0u, VHZ_MRT))
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def private rd_ensure_comb_sets {
    if (g_rd.comb_sets_made) return
    let nl = g_rd.n_layers
    let dim = g_rd.dim
    let nb = g_rd.nb
    let rows_bytes = dim * 4l * nb
    let slots_bytes = MAX_ROUTED_SLOTS * 4l
    let edn_bytes = nb * g_rd.k * dim * 4l
    for (l in range64(nl)) {
        var L & = unsafe(g_rd.layers[l])
        continue if (!L.moe)
        let fn = rd_next_feed(l)
        L.comb_rq = vk_fuse_on() && fn.fmt == int(KqFmt.q8) && !fn.storex && (L.moe_g4 ? ensure_cls_ar_comb_g4_rq() : ensure_cls_ar_comb_rq())
        continue if (!L.comb_rq)
        if (L.moe_g4) {
            L.s_comb_rq = set_cls_ar_comb_g4_rq(fixed_array(g_rd.x_dev, g_rd.ffnout_dev, g_rd.norms_dev, g_rd.xq_dev, g_rd.moe_ident_dev, g_rd.moe_dw_dev, g_rd.edown_dev, g_rd.xs_dev),
                fixed_array(rows_bytes, rows_bytes, g_rd.norms_bytes, dim * nb, slots_bytes, slots_bytes, edn_bytes, (dim / 32l) * 4l * nb),
                fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XQ, 0u, VHZ_MRT, VHZ_MDN, VHZ_XQ))
        } else {
            L.s_comb_rq = set_cls_ar_comb_rq(fixed_array(g_rd.x_dev, g_rd.ffnout_dev, g_rd.norms_dev, g_rd.xq_dev,
                    g_rd.moe_ident_dev, g_rd.moe_dw_dev, g_rd.edown_dev, g_rd.moe_dlog_dev, g_rd.moe_db_dev, g_rd.moe_eidx_dev, g_rd.xs_dev),
                fixed_array(rows_bytes, rows_bytes, g_rd.norms_bytes, dim * nb, slots_bytes, slots_bytes, edn_bytes, g_rd.ner * 4l * nb, g_rd.moe_db_bytes, slots_bytes, (dim / 32l) * 4l * nb),
                fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XQ, 0u, VHZ_MRT, VHZ_MDN, VHZ_MRT, 0u, VHZ_MRT, VHZ_XQ))
        }
    }
    g_rd.comb_sets_made = true
}

//! a layer without a dense triple (an MoE layer with no shared expert): the tail sets the token
//! command encodes around the routed block - the FFN norm, its requant and the residual step
def private rd_tail_sets_bare(var L : RLayer; f1, f3, f2 : int) {
    let dim = g_rd.dim
    let nb = g_rd.nb
    L.b1 = -1l; L.b3 = -1l; L.b2 = -1l
    L.f1 = f1; L.f3 = f3; L.f2 = f2
    L.s_addr_ffn = set_cls_ar(fixed_array(g_rd.x_dev, g_rd.xb2_dev, g_rd.norms_dev, g_rd.xb_dev),
        fixed_array(dim * 4l * nb, dim * 4l * nb, g_rd.norms_bytes, dim * 4l * nb),
        fixed_array(VHZ_X, VHZ_XB2, 0u, VHZ_XB))
    L.s_quant_xb2 = rd_quant_xb_set()
    L.s_addr_next = set_cls_ar(fixed_array(g_rd.x_dev, g_rd.ffnout_dev, g_rd.norms_dev, g_rd.xb_dev),
        fixed_array(dim * 4l * nb, dim * 4l * nb, g_rd.norms_bytes, dim * 4l * nb),
        fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB))
    rd_unmake_sets()
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def vk_rdec_set_moe_experts(l, router_slot, be1, be3, be2 : int64; fe1, fe3, fe2 : int) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null && g_rd.moe, "vk_rdec_set_moe_experts before prepare_moe")
    var L & = unsafe(g_rd.layers[l])
    assert(L.made, "vk_rdec_set_moe_experts before the layer's own registration")
    L.moe = true
    L.router_slot = router_slot
    L.be1 = be1; L.be3 = be3; L.be2 = be2
    L.fe1 = fe1; L.fe3 = fe3; L.fe2 = fe2
    rd_moe_sets(L)
    rd_unmake_sets()
    rd_invalidate_token()
}

//! the dense triple is the shared expert's; b1 < 0 = the layer has none.
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def vk_rdec_set_moe_layer(l, mir_idx, router_slot, bq, bk, bv, bo, b1, b3, b2, be1, be3, be2 : int64; fq, fk, fv, fo, f1, f3, f2, fe1, fe3, fe2 : int; g : RdecLayerGeom) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null && g_rd.moe, "vk_rdec_set_moe_layer before prepare_moe")
    rd_layer_slot(l)
    var L & = unsafe(g_rd.layers[l])
    rd_layer_geom(L, g)
    rd_attn_sets(L, mir_idx, bq, bk, bv, bo, fq, fk, fv, fo, f1, f3, f2)
    if (b1 >= 0l) {
        rd_ffn_sets(L, b1, b3, b2, f1, f3, f2)
    } else {
        rd_tail_sets_bare(L, f1, f3, f2)
    }
    L.made = true
    vk_rdec_set_moe_experts(l, router_slot, be1, be3, be2, fe1, fe3, fe2)
}

//! one recurrent layer's constants (conv taps, out-norm weights, a, dt) into its smalls
def private rdn_upload_constants(L : RLayer; convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?) {
    let cd = g_rd.dn_cd
    let ds = g_rd.dn_ds
    let nvh = g_rd.dn_nvh
    unsafe {
        var sp = reinterpret<float?>(g_gpu.dn_smalls.mapped)
        sp[0] = g_rd.eps
        sp[1] = 1.0 / sqrt(float(ds))
        memcpy(reinterpret<void?>(sp + DN_SM_TAPS), reinterpret<void?>(convw), cd * g_rd.dn_dconv * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_WNORM), reinterpret<void?>(wnormp), ds * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_A), reinterpret<void?>(ap), nvh * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_DT), reinterpret<void?>(dtp), nvh * 4l)
    }
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    cmd_copy_range(raw, g_gpu.dn_smalls.buf, 0l, L.dn[0].smalls_dev, 0l, DN_SM_HIST * 4l)
    vk_check(vkEndCommandBuffer(raw), null)
    submit_wait(g_gpu.batch_cmd)
}

//! the session's state and history into the slot, the history into the region's current parity image
def private rdn_state_upload(var st : DnStep; stp : float const?; histp : float const?; cur_par : int) {
    unsafe {
        memcpy(g_gpu.dn_state_stage.mapped, reinterpret<void?>(stp), st.slot_state_bytes)
        memcpy(reinterpret<void?>(reinterpret<float?>(g_gpu.dn_smalls.mapped) + DN_SM_HIST), reinterpret<void?>(histp), st.ring_image_bytes)
    }
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    cmd_copy_range(raw, g_gpu.dn_state_stage.buf, 0l, st.state_dev, st.state_byte_off, st.slot_state_bytes)
    cmd_copy_range(raw, g_gpu.dn_smalls.buf, DN_SM_HIST * 4l, st.smalls_dev,
        st.hist_off * 4l + int64(cur_par) * st.ring_image_bytes, st.ring_image_bytes)
    vk_check(vkEndCommandBuffer(raw), null)
    submit_wait(g_gpu.batch_cmd)
    st.parity = cur_par
    st.owner_state_addr = intptr(stp)
    st.owner_hist_addr = intptr(histp)
    st.valid = true
    st.dirty = false
}

//! Register a RECURRENT (deltanet) layer: the qkv / z / out q8 arena planes, the beta/alpha rows (q8
//! planes `bbeta`/`balpha`, or the f32 rows at `ba_off` in the `vk_rdec_upload_dn_ba` buffer), the FFN
//! triple, and the layer's device state + smalls; the session's state arrives through `vk_rdec_dn_own`.
[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def vk_rdec_set_dn_layer(l, bqkv, bz, bout, bbeta, balpha, b1, b3, b2, ba_off : int64; fqkv, fz, fout, f1, f3, f2 : int;
                         convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null && g_rd.hybrid, "vk_rdec_set_dn_layer before a hybrid prepare")
    rd_layer_slot(l)
    var L & = unsafe(g_rd.layers[l])
    L.recurrent = true
    L.mir_idx = -1l
    L.hid = g_rd.hidden   //! a recurrent layer's dense triple takes the model's width; its attention fields stay unused
    L.out_scale = 1.0     //! a PLE layer's residual step is unscaled: its row is the gate's feed
    L.bqkv = bqkv; L.bz = bz; L.bout = bout; L.bbeta = bbeta; L.balpha = balpha
    L.fqkv = fqkv; L.fz = fz; L.fout = fout
    L.ba_off = ba_off
    let dim = g_rd.dim
    let nb = g_rd.nb
    let cd = g_rd.dn_cd
    let di = g_rd.dn_di
    let nvh = g_rd.dn_nvh
    let ds = g_rd.dn_ds
    let anykq = kq_sb(f1) || kq_sb(f2)
    verify(ensure_cls_dn_rq() && ensure_cls_ar() && ensure_cls_ar_rq() && ensure_q8_actrq_cls()
        && (!anykq || (ensure_cls_q8k_rq() && ensure_q8k_actrq_cls()))
        && (!kq_sb(fout) || ensure_cls_q8k_rq())
        && gemv_cls_ensure(fqkv) && gemv_cls_ensure(fz) && gemv_cls_ensure(fout)
        && (g_rd.dn_ba_f32 || gemv_cls_ensure(int(KqFmt.q8)))
        && gemv_cls_ensure(f1) && gemv_cls_ensure(f3) && gemv_cls_ensure(f2),
        "vk decode: the class rails must engage on a live device")
    L.m_dnqkv = rd_meta(); L.m_dnz = rd_meta(); L.m_dnout = rd_meta()
    L.s_quant_xb = rd_quant_xb_set()
    let nslot = g_rd.regions
    let state_bytes = nvh * ds * ds * 4l
    let ring_elems = cd * (g_rd.dn_dconv - 1l)
    let smalls_bytes = (DN_SM_HIST + nslot * 2l * ring_elems + DN_SM_SLACK) * 4l
    let state_dev = make_device_buf(state_bytes * nslot)
    let smalls_dev = make_device_buf(smalls_bytes)
    L.dn |> resize(nslot)
    for (s in range64(nslot)) {
        L.dn[s] = DnStep(state_dev = state_dev, smalls_dev = smalls_dev, slot_state_bytes = state_bytes, ring_image_bytes = ring_elems * 4l,
                         smalls_bytes = smalls_bytes, state_byte_off = s * state_bytes, hist_off = DN_SM_HIST + s * 2l * ring_elems,
                         parity = 0, valid = false, dirty = false, owner_state_addr = 0ul, owner_hist_addr = 0ul)
    }
    rdn_upload_constants(L, convw, wnormp, ap, dtp)
    let pqkv = arena_planes(fqkv, bqkv)
    let pz = arena_planes(fz, bz)
    let pout = arena_planes(fout, bout)
    // qkv + z GEMVs into one projection row: planes @0/1, regions@2, xq@3, xs@4, out@5
    L.s_dnqkv = gemv_cls_set(fqkv, fixed_array(pqkv.wq, pqkv.ws, L.m_dnqkv.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.dn_proj_dev),
        fixed_array(pqkv.wqb, pqkv.wsb, META_BYTES, dim * nb, (dim / 32l) * 4l * nb, (cd + di) * 4l * nb),
        fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_DNP))
    L.s_dnz = gemv_cls_set(fz, fixed_array(pz.wq, pz.ws, L.m_dnz.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.dn_proj_dev),
        fixed_array(pz.wqb, pz.wsb, META_BYTES, dim * nb, (dim / 32l) * 4l * nb, (cd + di) * 4l * nb),
        fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_DNZ))   //! the z half of the row under its own class: the qkv and z GEMVs co-run, the step waits on both
    if (g_rd.dn_ba_f32) {
        L.s_dnba = set_router_gemv_f16_cls(fixed_array(g_rd.dn_ba_dev, g_rd.xb_dev, smalls_dev, g_rd.norms_dev),
            fixed_array(g_rd.dn_ba_bytes, dim * 4l * nb, smalls_bytes, g_rd.norms_bytes),
            fixed_array(0u, VHZ_XB, VHZ_DNSM, 0u))
    } else {
        L.m_dnbeta = rd_meta(); L.m_dnalpha = rd_meta()
        let pb = arena_planes(int(KqFmt.q8), bbeta)
        let pa = arena_planes(int(KqFmt.q8), balpha)
        L.s_dnbeta = gemv_cls_set(int(KqFmt.q8), fixed_array(pb.wq, pb.ws, L.m_dnbeta.buf, g_rd.xq_dev, g_rd.xs_dev, smalls_dev),
            fixed_array(pb.wqb, pb.wsb, META_BYTES, dim * nb, (dim / 32l) * 4l * nb, smalls_bytes),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_DNSM))
        L.s_dnalpha = gemv_cls_set(int(KqFmt.q8), fixed_array(pa.wq, pa.ws, L.m_dnalpha.buf, g_rd.xq_dev, g_rd.xs_dev, smalls_dev),
            fixed_array(pa.wqb, pa.wsb, META_BYTES, dim * nb, (dim / 32l) * 4l * nb, smalls_bytes),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_DNSM))
    }
    // the fused step: proj @0, state @1 (in place), smalls @2 (rw), o scales @3, o quants @4, tok @5, o f32 @6
    L.s_dnstep = set_dn_step_cls(
        fixed_array(g_rd.dn_proj_dev, state_dev, smalls_dev, g_rd.dn_os_dev, g_rd.dn_oq_dev, g_rd.tok_meta.buf, g_rd.dn_of_dev),
        fixed_array((cd + di) * 4l * nb, state_bytes * nslot, smalls_bytes, DND_OS_BYTES * nb, DND_OQ_BYTES * nb, TOK_META_BYTES * nb, DND_OQ_BYTES * 4l * nb),
        fixed_array(VHZ_DNP | VHZ_DNZ, VHZ_DNST, VHZ_DNSM, VHZ_DNO, VHZ_DNO, 0u, VHZ_DNO))
    if (kq_sb(fout)) {
        L.s_quant_dno = set_rq_cls(fixed_array(g_rd.dn_of_dev, g_rd.dn_oq_dev, g_rd.dn_os_dev),
            fixed_array(DND_OQ_BYTES * 4l * nb, DND_OQ_BYTES * nb, DND_OS_BYTES * nb),
            fixed_array(VHZ_DNO, VHZ_DNO, VHZ_DNO))
    }
    L.s_dnout = gemv_cls_set(fout, fixed_array(pout.wq, pout.ws, L.m_dnout.buf, g_rd.dn_oq_dev, g_rd.dn_os_dev, g_rd.xb2_dev),
        fixed_array(pout.wqb, pout.wsb, META_BYTES, di * nb, (di / 32l) * 4l * nb, dim * 4l * nb),
        fixed_array(0u, 0u, 0u, VHZ_DNO, VHZ_DNO, VHZ_XB2))
    if (b1 >= 0l) {
        rd_ffn_sets(L, b1, b3, b2, f1, f3, f2)
    } else {
        rd_tail_sets_bare(L, f1, f3, f2)   //! a recurrent MoE layer without a shared expert
    }
    L.made = true
    rd_invalidate_token()
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-attn-planes")]
def vk_rdec_upload_bias(rows : array<float>) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null, "vk_rdec_upload_bias before prepare")
    g_rd.bias_bytes = long_length(rows) * 4l
    g_rd.bias_dev = make_device_buf(g_rd.bias_bytes)
    upload_region_at(g_rd.bias_dev, 0l, unsafe(addr<uint8 const?>(rows[0])), g_rd.bias_bytes)
    g_rd.has_bias = true
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-attn-planes")]
def vk_rdec_upload_sinks(rows : array<float>) : bool {
    if (g_gpu != null && g_gpu.dry) {
        return true
    }
    assert(g_rd != null, "vk_rdec_upload_sinks before prepare")
    if (!(g_rd.kv16 && vk_fa_on() && (cm2_fa_on() || khr_kq_tile_on()) && g_rd.head_size == 64l)) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: attention sinks ride the flash tiles at head size 64 - no cm2 or KHR flash arm on the f16 mirrors here, the resident driver declines\n")
        return false
    }
    let khr = !cm2_fa_on()
    let stamps_ok = (fa_stamp_pair_ensure(khr, 64l, false, false, true, true)
        && (g_rd.kv16 ? ensure_da_attn_f16_sink_cls() : ensure_da_attn_sink_cls()))
    if (!stamps_ok) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: an attention-sink kernel class declined on this device, the resident driver declines\n")
        return false
    }
    g_rd.sinks_bytes = long_length(rows) * 4l
    g_rd.sinks_dev = make_device_buf(g_rd.sinks_bytes)
    upload_region_at(g_rd.sinks_dev, 0l, unsafe(addr<uint8 const?>(rows[0])), g_rd.sinks_bytes)
    g_rd.has_sinks = true
    return true
}

//! hasb 0 leaves the binding unread - the norms buffer only fills the slot
def private rd_bias_bind() : tuple<buf : uint64; bytes : int64> {
    return g_rd.has_bias ? (buf = g_rd.bias_dev, bytes = g_rd.bias_bytes) : (buf = g_rd.norms_dev, bytes = g_rd.norms_bytes)
}

def private rd_bias_off(L : RLayer) : uint => uint(L.boff)

//! The f32 arm's beta/alpha rows for every recurrent layer, in layer order: per layer `[beta rows
//! (nvh x dim) ; alpha rows (nvh x dim)]`; a layer's base is its `ba_off` at set_dn_layer. The
//! device keeps them as f16 - half the bytes the prefill tile GEMM and the decode GEMV read per row.
def vk_rdec_upload_dn_ba(rows : array<float>) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null && g_rd.hybrid, "vk_rdec_upload_dn_ba before a hybrid prepare")
    var half : array<float16>
    half |> reserve(long_length(rows))
    half |> resize(long_length(rows))
    for (r, hv in rows, half) {
        hv = float16(r)
    }
    g_rd.dn_ba_bytes = long_length(half) * 2l
    g_rd.dn_ba_dev = make_device_buf(g_rd.dn_ba_bytes)
    upload_region_at(g_rd.dn_ba_dev, 0l, unsafe(addr<uint8 const?>(half[0])), g_rd.dn_ba_bytes)
    delete half
}

def private rd_dn_advance_parity(slot : int64) {
    g_rd.dn_parity[slot] = 1 - g_rd.dn_parity[slot]
    for (L in g_rd.layers) {
        if (L.recurrent) {
            L.dn[slot].parity = g_rd.dn_parity[slot]
            L.dn[slot].dirty = true
        }
    }
}

//! Bind a recurrent layer's device slot (the selected region's) to the calling session's state (`stp`) and conv history (`histp`) before a token
[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#dn-step-owner"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def vk_rdec_dn_own(l : int64; stp : float?; histp : float?) {
    var L & = unsafe(g_rd.layers[l])
    let slot = g_rd.region
    var st & = unsafe(L.dn[slot])
    let caller = intptr(stp)
    if (st.valid && st.owner_state_addr == caller) {
        return
    }
    if (st.valid && st.dirty) {
        dnd_flush_down(st)
    }
    rdn_state_upload(st, stp, histp, g_rd.dn_parity[slot])
}

//! The per-layer-embedding seats: the side-input row buffers at the model's ple width (after prepare, before the layers)
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def vk_rdec_set_ple(ple : int64) : bool {
    if (g_gpu != null && g_gpu.dry) {
        return true
    }
    assert(g_rd != null, "vk_rdec_set_ple before prepare")
    if (ple <= 0l || ple % 32l != 0l) {
        return false   //! the gate GEMV's output and the act's requant take 32-blocks
    }
    g_rd.ple = ple
    let row_bytes = g_rd.n_layers * ple * 4l * g_rd.nb
    g_rd.ple_host = make_host_buf(row_bytes, false, [cached = true])
    g_rd.ple_dev = make_device_buf(row_bytes)
    g_rd.pleg_dev = make_device_buf(ple * 4l * g_rd.nb)
    rd_unmake_sets()
    rd_invalidate_token()
    return true
}

//! One layer's per-layer-embedding branch: the gate [dim x ple] and proj [ple x dim] planes and the sets of its four steps
def vk_rdec_set_ple_layer(l, bgate, bproj : int64; fmt : int) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null && g_rd.ple > 0l, "vk_rdec_set_ple_layer before set_ple")
    let nb = g_rd.nb
    rd_layer_slot(l)
    let dim = g_rd.dim
    let ple = g_rd.ple
    var L & = unsafe(g_rd.layers[l])
    L.ple = true
    L.bpg = bgate; L.bpp = bproj; L.fpg = fmt
    let pg = arena_planes(fmt, bgate)
    let pp = arena_planes(fmt, bproj)
    verify(gemv_cls_ensure(fmt) && ensure_q8_actrq_cls() && ensure_cls_dn_rq(), "vk decode: the per-layer-embedding rails must engage on a live device")
    L.m_pg = rd_meta(); L.m_pp = rd_meta()
    let all = g_rd.n_layers * ple
    // the residual row as it is -> xq/xs (the gate reads the unnormed stream)
    L.s_ple_x = set_rq_cls(fixed_array(g_rd.x_dev, g_rd.xq_dev, g_rd.xs_dev),
        fixed_array(dim * 4l * nb, dim * nb, (dim / 32l) * 4l * nb),
        fixed_array(VHZ_X, VHZ_XQ, VHZ_XQ))
    // gate GEMV [dim -> ple] into pleg (the FFN gate plane's hazard bit, free by then)
    L.s_ple_gate = gemv_cls_set(fmt, fixed_array(pg.wq, pg.ws, L.m_pg.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.pleg_dev),
        fixed_array(pg.wqb, pg.wsb, META_BYTES, dim * nb, (dim / 32l) * 4l * nb, ple * 4l * nb),
        fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_GATE))
    // gelu(gate) x this layer's side-input row -> hq/hs (the row's layer base rides the push; the side rows ride the cos plane's bit)
    L.s_ple_act = set_actrq_cls(fixed_array(g_rd.pleg_dev, g_rd.ple_dev, g_gpu.hq_dev, g_gpu.hs_dev),
        fixed_array(ple * 4l * nb, all * 4l * nb, BATCH_HQ_BYTES, BATCH_HS_BYTES),
        fixed_array(VHZ_GATE, VHZ_COS, VHZ_HQ, VHZ_HQ))
    // proj GEMV [ple -> dim] into ffnout (the residual step's add partner slot)
    L.s_ple_proj = gemv_cls_set(fmt, fixed_array(pp.wq, pp.ws, L.m_pp.buf, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.ffnout_dev),
        fixed_array(pp.wqb, pp.wsb, META_BYTES, ple * nb, (ple / 32l) * 4l * nb, dim * 4l * nb),
        fixed_array(0u, 0u, 0u, VHZ_HQ, VHZ_HQ, VHZ_FFO))
    L.ple_fused = vk_fuse_on() && fmt == int(KqFmt.q8) && ple <= 256l && ple % 32l == 0l && ensure_cls_ar_rq() && ensure_q8_gemv_pleact_cls()
    if (L.ple_fused) {
        L.s_ple_ar_rq = set_ar_rq_stamp(RqStamp.rq, g_rd.ffnout_dev, VHZ_FFO, g_rd.xq_dev, g_rd.xs_dev)
        L.s_ple_pa = set_q8_gemv_pleact_cls(fixed_array(pp.wq, pp.ws, g_rd.pleg_dev, g_rd.ple_dev, g_rd.ffnout_dev),
            fixed_array(pp.wqb, pp.wsb, ple * 4l * nb, all * 4l * nb, dim * 4l * nb),
            fixed_array(0u, 0u, VHZ_GATE, VHZ_COS, VHZ_FFO))
    }
    rd_unmake_sets()
    rd_invalidate_token()
}

//! Register the classifier plane (arena block + format) and build the prologue / final-norm / cls
//! sets, and the logits epilogue where the model has one (the final softcap `cap`, the suppressed
//! ids `supp`). Call after all layers are set.
def vk_rdec_set_cls(cls_block : int64; cls_fmt : int; cap : float; supp : array<int64>) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null, "vk_rdec_set_cls before prepare")
    let dim = g_rd.dim
    let vocab = g_rd.vocab
    let nb = g_rd.nb
    g_rd.cls_cap = cap
    g_rd.nsupp = long_length(supp)
    unsafe {
        verify(ensure_cls_ar() && ensure_cls_ar_rq() && ensure_cls_dn_rq() && gemv_cls_ensure(cls_fmt)
            && (!kq_sb(cls_fmt) || ensure_cls_q8k_rq())
            && (!rd_cls_epi_on() || ensure_cls_epi()) && ensure_cls_argmax_part() && ensure_cls_argmax_fin(),
            "vk decode: the class rails must engage on a live device")
        let part_bytes = 4l * nb * RD_PICK_CHUNKS
        g_rd.s_pick_part = set_cls_argmax_part(fixed_array(g_rd.logits_dev, g_rd.pick_part_max_dev, g_rd.pick_part_id_dev), fixed_array(vocab * 4l * nb, part_bytes, part_bytes), fixed_array(VHZ_LOG, VHZ_LOG, VHZ_LOG))
        g_rd.s_pick_fin = set_cls_argmax_fin(fixed_array(g_rd.pick_part_max_dev, g_rd.pick_part_id_dev, g_rd.pick_dev), fixed_array(part_bytes, part_bytes, 4l * nb), fixed_array(VHZ_LOG, VHZ_LOG, VHZ_LOG))
        if (rd_cls_epi_on()) {
            var supp_bytes = 256l
            if (g_rd.nsupp > 0l) {
                supp_bytes = g_rd.nsupp * 4l
                g_rd.supp_dev = make_device_buf(supp_bytes)
                var sp = reinterpret<uint?>(g_gpu.staging.mapped)
                for (i in range64(g_rd.nsupp)) {
                    sp[i] = uint(supp[i])
                }
                staged_upload(g_rd.supp_dev, 0l, supp_bytes)
            }
            g_rd.cls_epi_set = set_cls_epi(fixed_array(g_rd.logits_dev, g_rd.nsupp > 0l ? g_rd.supp_dev : g_rd.dummy),
                fixed_array(vocab * 4l * nb, supp_bytes), fixed_array(VHZ_LOG, 0u))
        }
        // prologue (class): xb = rms_att[0](x), add off — the aa slot binds x itself (add_on=0 never reads it)
        g_rd.s_prologue = set_cls_ar(fixed_array(g_rd.x_dev, g_rd.x_dev, g_rd.norms_dev, g_rd.xb_dev),
            fixed_array(dim * 4l * nb, dim * 4l * nb, g_rd.norms_bytes, dim * 4l * nb),
            fixed_array(VHZ_X, VHZ_X, 0u, VHZ_XB))
        rd_unmake_sets()
        // quantize final xb
        g_rd.s_quant_final = rd_quant_xb_set()
        // cls GEMV (class): a device one-pair regions buffer replaces the device-filled meta
        // (device for the same reason — every vocab-row subgroup reads it)
        g_rd.cls_regions_dev = make_device_buf(CLS_META_BYTES)
        var mp = reinterpret<uint?>(g_gpu.staging.mapped)
        mp[0] = uint(arena_local_blk(cls_block))
        mp[1] = 0u
        staged_upload(g_rd.cls_regions_dev, 0l, CLS_META_BYTES)
        let pc = arena_planes(cls_fmt, cls_block)
        g_rd.cls_set = gemv_cls_set(cls_fmt, fixed_array(pc.wq, pc.ws, g_rd.cls_regions_dev, g_rd.xq_dev, g_rd.xs_dev, g_rd.logits_dev),
            fixed_array(pc.wqb, pc.wsb, CLS_META_BYTES, dim * nb, (dim / 32l) * 4l * nb, vocab * 4l * nb),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_LOG))
        g_rd.cls_fmt_marker = cls_fmt
        rd_invalidate_token()   // the recorded chain references the replaced cls/prologue sets
        g_rd.ready = true
    }
}

//! the classifier epilogue has work: a final softcap or suppressed ids
def rd_cls_epi_on : bool => g_rd.cls_cap > 0.0 || g_rd.nsupp > 0l

//! the epilogue over the `nrows` logits rows the classifier GEMV just wrote (the prefill's tail encodes one row the same)
def rd_enc_cls_epi(raw : VkCommandBuffer; var h : VkHaz; nrows : int64 = 1l) {
    var pce = ClsEpiArgs(vocab = uint(g_rd.vocab), cap = g_rd.cls_cap, nsupp = uint(g_rd.nsupp), rows = uint(nrows))
    enc_cls_epi(raw, h, g_rd.cls_epi_set, pce, (g_rd.vocab * nrows + 255l) / 256l)
}

// Record the whole-token cmd ONCE per set_layer/set_cls epoch: constant metas + scalars fill here;
// per-token state is only x/cos (in-cmd copies) and the rope/attn position words (host-visible)
[cold_path]   // once per epoch (set replacement re-records); the rerecord A/B leg is opt-in
def private rd_record_token {
    rd_fill_gemv_regions_all()
    rd_record_unsplit_twin()
    rd_encode_token(g_rd.cmd[g_rd.region], 1l, rd_split_pieces())
    g_rdq_n = g_pfq_n   // decode's stamp count (prefill re-arms the shared pool per window)
    g_rd.tok_recorded[g_rd.region] = true
    g_rd.tok_wide_recorded[g_rd.region] = false
}

//! the split form's pieces on this device: the ladder's count, never past the partials plane's (`attn_nsplit`)
def private rd_split_pieces : int64 => min(RD_SPLIT_PIECES, g_rd.attn_nsplit)
def private rd_wide_pieces : int64 => min(RD_SPLIT_WIDE_PIECES, g_rd.attn_nsplit)

//! the ladder's pieces for an attended span: one under RD_UNSPLIT_POS, the split form's to RD_WIDE_POS, the wide form's past it
def private rd_pieces_for_span(span : int64) : int64 {
    if (span < RD_UNSPLIT_POS) return 1l
    return span < RD_WIDE_POS ? RD_SPLIT_PIECES : RD_SPLIT_WIDE_PIECES
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-decode-attn-split")]
def private rd_layer_pieces(L : RLayer; nsplit : int64) : int64 {
    let span = L.window == 0l ? nsplit : min(nsplit, rd_pieces_for_span(L.window))
    let wave = max(1l, int64(g_gpu.sm_count) / (da_attn_row_wgs(g_rd.n_heads, L.kv_mul, 1l) * g_rd.nb))
    return min(span, wave)
}

//! the attention form a one-row step at `pos` takes
def private rd_form_at(pos : int64) : RdForm {
    if (g_rd.unsplit_on && pos < RD_UNSPLIT_POS) return RdForm.unsplit
    return (g_rd.wide_on && pos >= RD_WIDE_POS) ? RdForm.wide : RdForm.split
}

//! the wide twin: the same chain at RD_SPLIT_WIDE_PIECES into `cmd_wide`, its stamp names kept apart (sec.2.2al)
[cold_path]
def private rd_record_wide_twin {
    var split_names <- g_rdq_stamp_names
    let split_n = g_rdq_n
    let split_overflow = g_rdq_overflow
    rd_encode_token(g_rd.cmd_wide[g_rd.region], 1l, rd_wide_pieces())
    delete g_rdq_names_wide
    g_rdq_names_wide <- g_rdq_stamp_names
    g_rdq_n_wide = g_pfq_n
    g_rdq_overflow_wide = g_rdq_overflow
    delete g_rdq_stamp_names
    g_rdq_stamp_names <- split_names
    g_rdq_n = split_n
    g_rdq_overflow = split_overflow
    g_rd.tok_wide_recorded[g_rd.region] = true
}

//! every GEMV's (rel_wblock, xblock) region table, filled for the planes each layer holds
def private rd_fill_gemv_regions_all {
    for (l in range64(g_rd.n_layers)) {
        var L & = unsafe(g_rd.layers[l])
        if (L.recurrent) {
            fill_gemv_regions(L.m_dnqkv, L.bqkv)
            fill_gemv_regions(L.m_dnz, L.bz)
            fill_gemv_regions(L.m_dnout, L.bout)
            if (!g_rd.dn_ba_f32) {
                fill_gemv_regions(L.m_dnbeta, L.bbeta)
                fill_gemv_regions(L.m_dnalpha, L.balpha)
            }
        } elif (L.qkv_merged) {
            fill_gemv_regions_qkv(L.m_q, L.bq, L.bk, L.bv, L.qd / L.kvd, L.kvd * (g_rd.dim / fmt_unit(L.fq)), !L.vfk)
            fill_gemv_regions(L.m_wo, L.bo)
        } else {
            fill_gemv_regions(L.m_q, L.bq)
            if (L.kv_shared) {   //! no k/v planes of its own
            } elif (L.kv_merged) {
                fill_gemv_regions2(L.m_k, L.bk, L.bv)   // k, v regions land at qd, qd+kvd (push ybase)
            } else {
                fill_gemv_regions(L.m_k, L.bk)
                if (!L.vfk) {
                    fill_gemv_regions(L.m_v, L.bv)
                }
            }
            fill_gemv_regions(L.m_wo, L.bo)
        }
        if (has_dense_triple(L)) {
            fill_gemv_regions(L.m_gate, L.b1)
            fill_gemv_regions(L.m_up, L.b3)
            fill_gemv_regions(L.m_down, L.b2)
        }
        if (L.ple) {
            fill_gemv_regions(L.m_pg, L.bpg)
            fill_gemv_regions(L.m_pp, L.bpp)
        }
    }
}

//! the unsplit twin on a split device: the same chain at one split into `cmd_unsplit`, its stamp names and overflow flag kept apart
[cold_path]   // beside rd_record_token, and the rerecord probe's per-token leg
def private rd_record_unsplit_twin {
    if (!g_rd.unsplit_on) return
    rd_encode_token(g_rd.cmd_unsplit[g_rd.region], 1l, 1l)
    delete g_rdq_names_unsplit
    g_rdq_names_unsplit <- g_rdq_stamp_names
    g_rdq_n_unsplit = g_pfq_n
    g_rdq_overflow_unsplit = g_rdq_overflow
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private rq_stamp(kfmt : int; storex : bool) : RqStamp {
    if (kq_sb(kfmt)) {
        return storex ? RqStamp.rqkx : RqStamp.rqk
    }
    return storex ? RqStamp.rqx : RqStamp.rq
}

//! a fused site's set: x, its add partner (the FFN site's xb2, the next site's ffnout), the norms, the quant feed and - on a row-storing stamp - xb;
//! the rows stamp binds the same five over the N-row command's planes
def private set_ar_rq_stamp(stamp : RqStamp; add : uint64; aclass : uint; xq, xs : uint64) : VkDescriptorSet {
    let dim = g_rd.dim
    let nb = g_rd.norms_bytes
    if (stamp == RqStamp.rq) {
        return set_cls_ar_rq(fixed_array(g_rd.x_dev, add, g_rd.norms_dev, xq, xs),
            fixed_array(dim * 4l, dim * 4l, nb, dim, (dim / 32l) * 4l), fixed_array(VHZ_X, aclass, 0u, VHZ_XQ, VHZ_XQ))
    } elif (stamp == RqStamp.rq_b) {
        let rows = g_rd.nb
        return set_cls_ar_rq_b(fixed_array(g_rd.x_dev, add, g_rd.norms_dev, xq, xs),
            fixed_array(dim * 4l * rows, dim * 4l * rows, nb, dim * rows, (dim / 32l) * 4l * rows), fixed_array(VHZ_X, aclass, 0u, VHZ_XQ, VHZ_XQ))
    } elif (stamp == RqStamp.rqx) {
        return set_cls_ar_rqx(fixed_array(g_rd.x_dev, add, g_rd.norms_dev, xq, xs, g_rd.xb_dev),
            fixed_array(dim * 4l, dim * 4l, nb, dim, (dim / 32l) * 4l, dim * 4l), fixed_array(VHZ_X, aclass, 0u, VHZ_XQ, VHZ_XQ, VHZ_XB))
    } elif (stamp == RqStamp.rqk) {
        return set_cls_ar_rqk(fixed_array(g_rd.x_dev, add, g_rd.norms_dev, xq, xs),
            fixed_array(dim * 4l, dim * 4l, nb, dim, (dim / 32l) * 4l), fixed_array(VHZ_X, aclass, 0u, VHZ_XQ, VHZ_XQ))
    }
    return set_cls_ar_rqkx(fixed_array(g_rd.x_dev, add, g_rd.norms_dev, xq, xs, g_rd.xb_dev),
        fixed_array(dim * 4l, dim * 4l, nb, dim, (dim / 32l) * 4l, dim * 4l), fixed_array(VHZ_X, aclass, 0u, VHZ_XQ, VHZ_XQ, VHZ_XB))
}

def private ensure_ar_rq_stamp(stamp : RqStamp) : bool {
    if (stamp == RqStamp.rq) return ensure_cls_ar_rq()
    if (stamp == RqStamp.rq_b) return ensure_cls_ar_rq_b()
    if (stamp == RqStamp.rqx) return ensure_cls_ar_rqx()
    if (stamp == RqStamp.rqk) return ensure_cls_ar_rqk()
    return ensure_cls_ar_rqkx()
}

//! the stamp's dispatch: one workgroup on the one-row stamps, a workgroup a row on the rows stamp
def private enc_ar_rq_stamp(stamp : RqStamp; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : ArArgs; rows : int64 = 1l) {
    if (stamp == RqStamp.rq) {
        enc_cls_ar_rq(raw, h, s, pc, 1l)
    } elif (stamp == RqStamp.rq_b) {
        enc_cls_ar_rq_b(raw, h, s, pc, rows)
    } elif (stamp == RqStamp.rqx) {
        enc_cls_ar_rqx(raw, h, s, pc, 1l)
    } elif (stamp == RqStamp.rqk) {
        enc_cls_ar_rqk(raw, h, s, pc, 1l)
    } else {
        enc_cls_ar_rqkx(raw, h, s, pc, 1l)
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private rd_head_feed(L : RLayer) : tuple<fmt : int; storex : bool> => (fmt = L.recurrent ? L.fqkv : L.fq, storex = L.recurrent)

//! the feed of the head after layer `l`: the next layer's, or the classifier's
def private rd_next_feed(l : int64) : tuple<fmt : int; storex : bool>
    => l + 1l < g_rd.n_layers ? rd_head_feed(g_rd.layers[l + 1l]) : (fmt = g_rd.cls_fmt_marker, storex = false)

//! the rows form's fused add+rms+requant stamp writes Q8_0 blocks and no float row: it serves a head whose feed is that and nothing more
def private rd_rows_fuse(feed : tuple<fmt : int; storex : bool>) : bool => !kq_sb(feed.fmt) && !feed.storex

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private rd_ensure_fused_sets {
    if (g_rd.fused_sets_made) {
        return
    }
    let nl = g_rd.n_layers
    var used : bool[4]
    unsafe {
        let f0 = rd_head_feed(g_rd.layers[0])
        g_rd.rq_pro_stamp = rq_stamp(f0.fmt, f0.storex)
        used[int(g_rd.rq_pro_stamp)] = true
        for (l in range64(nl)) {
            var L & = g_rd.layers[l]
            L.rq_ffn_stamp = rq_stamp(L.f1, L.moe)   //! a bare MoE layer's f1 is the experts' feed format; a MoE site stores the row its router reads
            used[int(L.rq_ffn_stamp)] = true
            continue if (L.moe)   //! a MoE layer's next site is the combine, which carries its own norm and store - its quantizing form is `rd_ensure_comb_sets`'s
            let fn = rd_next_feed(l)
            L.rq_next_stamp = rq_stamp(fn.fmt, fn.storex)
            used[int(L.rq_next_stamp)] = true
        }
    }
    verify((!used[int(RqStamp.rq)] || ensure_cls_ar_rq()) && (!used[int(RqStamp.rqx)] || ensure_cls_ar_rqx())
        && (!used[int(RqStamp.rqk)] || ensure_cls_ar_rqk()) && (!used[int(RqStamp.rqkx)] || ensure_cls_ar_rqkx()),
        "vk decode: the fused add+rms+requant rails must engage on a live device")
    let dim = g_rd.dim
    g_rd.s_prologue_rq = set_ar_rq_stamp(g_rd.rq_pro_stamp, g_rd.x_dev, VHZ_X, g_rd.xq_dev, g_rd.xs_dev)   // add off: the aa slot binds x itself
    for (l in range64(nl)) {
        var L & = unsafe(g_rd.layers[l])
        L.s_addr_ffn_rq = set_ar_rq_stamp(L.rq_ffn_stamp, g_rd.xb2_dev, VHZ_XB2, g_rd.xq_dev, g_rd.xs_dev)
        L.wo_ar = !L.recurrent && L.fo == int(KqFmt.q8) && L.rq_ffn_stamp == RqStamp.rq && (vk_fuse_bisect_bits() & (FUSE_BX_WO_AR | FUSE_BX_POST_ATT_RQ)) == 0l && ensure_q8_gemv_ar_cls()
        if (L.wo_ar) {
            let po = arena_planes(L.fo, L.bo)
            L.s_wo_ar = set_q8_gemv_ar_cls(fixed_array(po.wq, po.ws, g_rd.aq_dev, g_rd.as_dev, g_rd.xb2_dev, g_rd.x_dev, g_rd.norms_dev, g_rd.xq_dev, g_rd.xs_dev, g_rd.ar_cnt_dev),
                fixed_array(po.wqb, po.wsb, L.qd, (L.qd / 32l) * 4l, dim * 4l, dim * 4l, g_rd.norms_bytes, dim, (dim / 32l) * 4l, 16l),
                fixed_array(0u, 0u, VHZ_AQ, VHZ_AQ, VHZ_XB2, VHZ_X, 0u, VHZ_XQ, VHZ_XQ, VHZ_XB2))
        }
        if (!L.moe) {
            L.s_addr_next_rq = set_ar_rq_stamp(L.rq_next_stamp, g_rd.ffnout_dev, VHZ_FFO, g_rd.xq_dev, g_rd.xs_dev)
            //! a PLE layer joins whatever its next stamp is: its step quantizes the row as it is, for the gate's feed
            L.down_ar = has_dense_triple(L) && L.f2 == int(KqFmt.q8) && (L.ple || L.rq_next_stamp == RqStamp.rq) && (vk_fuse_bisect_bits() & (FUSE_BX_DOWN_AR | FUSE_BX_POST_FFN_RQ)) == 0l && ensure_q8_gemv_ar_cls()
            if (L.down_ar) {
                let p2 = arena_planes(L.f2, L.b2)
                L.s_down_ar = set_q8_gemv_ar_cls(fixed_array(p2.wq, p2.ws, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.ffnout_dev, g_rd.x_dev, g_rd.norms_dev, g_rd.xq_dev, g_rd.xs_dev, g_rd.ar_cnt_dev),
                    fixed_array(p2.wqb, p2.wsb, BATCH_HQ_BYTES, BATCH_HS_BYTES, dim * 4l, dim * 4l, g_rd.norms_bytes, dim, (dim / 32l) * 4l, 16l),
                    fixed_array(0u, 0u, VHZ_HQ, VHZ_HQ, VHZ_FFO, VHZ_X, 0u, VHZ_XQ, VHZ_XQ, VHZ_FFO))
            }
        }
    }
    rd_ensure_comb_sets()
    g_rd.fused_sets_made = true
    rd_announce_fuse_bisect()
}

//! the fused sites' announce: every site fused, or the bits DASLLAMA_VK_FUSE_BISECT keeps split, each named as taken or inert here
def private rd_announce_fuse_bisect {
    let bx = vk_fuse_bisect_bits()
    if (bx == 0l) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: fused add+rms+requant on every decode site (DASLLAMA_VK_FUSE=0 pins the split dispatches, DASLLAMA_VK_FUSE_BISECT one form at a time)\n")
        return
    }
    var wo_elig = false
    var down_elig = false
    var gu_elig = false
    for (L in g_rd.layers) {
        wo_elig = wo_elig || (!L.recurrent && L.fo == int(KqFmt.q8) && L.rq_ffn_stamp == RqStamp.rq)
        down_elig = down_elig || (has_dense_triple(L) && L.f2 == int(KqFmt.q8) && (L.ple || L.rq_next_stamp == RqStamp.rq))
        gu_elig = gu_elig || (L.f1 == int(KqFmt.q8) && L.f3 == int(KqFmt.q8) && !kq_sb(L.f2))
    }
    let names <- [
        (bit = FUSE_BX_WO_AR, name = "the wo epilogue", live = wo_elig),
        (bit = FUSE_BX_DOWN_AR, name = "the down epilogue", live = down_elig),
        (bit = FUSE_BX_GU, name = "the fused gate-up", live = gu_elig),
        (bit = FUSE_BX_PROLOGUE_RQ, name = "the prologue requant stamp", live = true),
        (bit = FUSE_BX_POST_ATT_RQ, name = "the post-attention requant stamp", live = true),
        (bit = FUSE_BX_POST_FFN_RQ, name = "the post-FFN requant stamp", live = true),
        (bit = FUSE_BX_QKN_ROPE, name = "the fused qk-norm+rope", live = g_rd.qk_norm && !g_rd.q_gated)]
    var taken : array<string>
    var inert : array<string>
    for (e in names) {
        continue if ((bx & e.bit) == 0l)
        if (e.live) {
            taken |> push("{e.name} ({e.bit})")
        } else {
            inert |> push("{e.name} ({e.bit})")
        }
    }
    let inert_part = empty(inert) ? "" : "; inert on this model: {join(inert, ", ")}"
    to_log(LOG_INFO, "dasLLAMA vulkan tier: DASLLAMA_VK_FUSE_BISECT={bx} keeps on the split dispatches: {join(taken, ", ")}{inert_part}\n")
    if (!empty(inert)) {
        to_log(LOG_WARNING, "dasLLAMA vulkan tier: DASLLAMA_VK_FUSE_BISECT names a form this model never fuses - that arm's A/B is a tie by construction\n")
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-decode-attn-split")]
def private rd_attn_rqk(L : RLayer) : uint {
    if (!kq_sb(L.fo)) return DA_RQ_Q8
    return L.hs % 256l == 0l ? DA_RQ_Q8K : DA_RQ_NONE
}

def private rd_enc_da_attn(raw : VkCommandBuffer; var h : VkHaz; var L : RLayer; var pca : DaAttnArgs; awgs : int64) {
    if (g_rd.has_sinks) {
        if (g_rd.kv16) {
            enc_da_attn_f16_sink_cls(raw, h, L.s_attn, pca, awgs)
        } else {
            enc_da_attn_sink_cls(raw, h, L.s_attn, pca, awgs)
        }
    } elif (da_slab_is_g2(L.kv_mul)) {
        if (g_rd.kv16) {
            enc_da_attn_f16_g2_cls(raw, h, L.s_attn, pca, awgs)
        } else {
            enc_da_attn_g2_cls(raw, h, L.s_attn, pca, awgs)
        }
    } elif (g_rd.kv16) {
        enc_da_attn_f16_cls(raw, h, L.s_attn, pca, awgs)
    } else {
        enc_da_attn_cls(raw, h, L.s_attn, pca, awgs)
    }
}

//! the attention head over `nrows` rows: the projections as GEMVs into the rows of the projection plane (the N-column leaves past
//! one row), the rope and store at each row's own position and region, the attention over each row's own history; `ncols` is the
//! GEMV leaves' column count and `nrow_bx` the N-row bisect bits (both the one-row reading at one row)
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def private rd_encode_attn_head(raw : VkCommandBuffer; var h : VkHaz; var L : RLayer; l : int64; fuse_qkr : bool; nlfin, nsplit, nrows : int64; ncols : uint; nrow_bx : int) {
    let dim = g_rd.dim
    let qd = L.qd
    let kvd = L.kvd
    let one = nrows == 1l
    let nr = uint(nrows)
    let qrows = g_rd.q_gated ? 2l * qd : qd
    let qstride = uint(g_rd.q_gated ? 2l * L.hs : L.hs)
    let kvrow = uint(qd + 2l * kvd)   //! a row's stride in the projection plane, where the ungated q row sits in front of its k and v
    let q_plane_row = uint(g_rd.q_gated ? 2l * qd : qd + 2l * kvd)   //! a gated q's [q | gate] heads fill a plane of their own; an ungated q rides the projection row
    if (L.qkv_merged) {   //! q in kvd-row pieces, then k, then v: one dispatch writes the whole projection row
        let nreg = qd / kvd + (L.vfk ? 1l : 2l)
        var pcqkv = GemvArgs(n = uint(dim), d = uint(kvd), nreg = uint(nreg), ybase = 0u, ncols = ncols, ystride = kvrow)
        gemv_enc(L.fq, raw, h, L.s_qkv, pcqkv, nreg * kvd)
        rd_ts(raw, "a:qkv")
    } else {
        var pcq = GemvArgs(n = uint(dim), d = uint(qrows), nreg = 1u, ybase = 0u, ncols = ncols, ystride = q_plane_row)
        gemv_enc(L.fq, raw, h, L.s_q, pcq, qrows)
        rd_ts(raw, "a:q")
    }
    if (L.kv_shared || L.qkv_merged) {   //! a shared-KV layer projects Q only: the donor layer's rows already sit in the mirror
    } elif (L.kv_merged) {
        var pckv = GemvArgs(n = uint(dim), d = uint(kvd), nreg = 2u, ybase = uint(qd), ncols = ncols, ystride = kvrow)
        gemv_enc(L.fk, raw, h, L.s_kv, pckv, 2l * kvd)
        rd_ts(raw, "a:kv")
    } else {
        var pck = GemvArgs(n = uint(dim), d = uint(kvd), nreg = 1u, ybase = uint(qd), ncols = ncols, ystride = kvrow)
        gemv_enc(L.fk, raw, h, L.s_k, pck, kvd)
        rd_ts(raw, "a:kv")
        if (!L.vfk) {   //! a V-from-K layer has no v plane: the rope store takes V off the k row
            var pcv = GemvArgs(n = uint(dim), d = uint(kvd), nreg = 1u, ybase = uint(qd + kvd), ncols = ncols, ystride = kvrow)
            gemv_enc(L.fv, raw, h, L.s_v, pcv, kvd)
            rd_ts(raw, "a:kv")   //! the split v stamp folds into the kv slot
        }
    }
    if ((nrow_bx & (NROW_BX_ATTN | NROW_BX_ROPE)) == (NROW_BX_ATTN | NROW_BX_ROPE)) {
        var pcw0 = GemvArgs(n = uint(qd), d = uint(dim), nreg = 1u, ybase = 0u, ncols = ncols, ystride = uint(dim))
        gemv_enc(L.fo, raw, h, L.s_wo, pcw0, dim)
        rd_ts(raw, "a:wo")
        return
    }
    let hs = L.hs
    let hsmax = g_rd.head_size   // nolint:LINT021 — the q/k norm rows sit at the widest class's stride; the row offset multiplies run 64-bit before the push-field narrow
    let mirbase = uint(one ? rd_mir_base(L) : L.mir_base)   //! the one-row command's region rides its push; an N-row step's rows carry theirs in TokMeta
    let nkh = L.kv_shared ? 0l : kvd / hs   //! the k-head groups also store the mirror row: none on a shared-KV layer
    if (fuse_qkr && (nrow_bx & NROW_BX_ROPE) == 0) {
        var pcf = QknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(L.rot / 2l),
            neox = g_rd.neox ? 1u : 0u, nh = uint(g_rd.n_heads), layerbase = mirbase,
            qwoff = uint(nlfin + dim + l * 2l * hsmax), kwoff = uint(nlfin + dim + (l * 2l + 1l) * hsmax),
            eps = g_rd.eps, qstride = qstride, boff = rd_bias_off(L), hasb = g_rd.has_bias ? 1u : 0u, cbase = uint(L.cbase),
            vfk = L.vfk ? 1u : 0u, vnorm = L.vnorm ? 1u : 0u,
            rowwg = uint(g_rd.n_heads + nkh), kvrow = kvrow, qrow = q_plane_row, cosrow = uint(g_rd.cos_elems))
        if (g_rd.kv16) {
            enc_qkn_rope_f16_cls(raw, h, L.s_qkn_rope, pcf, (g_rd.n_heads + nkh) * nrows)
        } else {
            enc_qkn_rope_cls(raw, h, L.s_qkn_rope, pcf, (g_rd.n_heads + nkh) * nrows)
        }
        rd_ts(raw, "a:qknrope")
    } elif ((nrow_bx & NROW_BX_ROPE) == 0) {
        if (g_rd.qk_norm) {
            var pcn = QkRmsArgs(hs = uint(hs), nh = uint(g_rd.n_heads), nkvh = uint(nkh),
                qstride = kvrow, kstride = kvrow, kbase = uint(qd),
                qwoff = uint(nlfin + dim + l * 2l * hsmax), kwoff = uint(nlfin + dim + (l * 2l + 1l) * hsmax),
                eps = g_rd.eps, qhs = uint(hs))
            enc_qk_rms_cls(raw, h, L.s_qkn, pcn, (g_rd.n_heads + nkh) * nrows)
            rd_ts(raw, "a:qkn")
        }
        let npairs = qd / 2l + (L.kv_shared ? 0l : kvd / 2l)   //! the k pairs also store the mirror row: none on a shared-KV layer
        var pcr = RopeKvArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(hs / 2l),
            neox = g_rd.neox ? 1u : 0u, kpair0 = uint(qd / 2l), npairs = uint(npairs),
            layerbase = mirbase, boff = rd_bias_off(L), hasb = g_rd.has_bias ? 1u : 0u, cbase = uint(L.cbase),
            rows = nr, kvrow = kvrow, qrow = q_plane_row, cosrow = uint(g_rd.cos_elems))
        let rwgs = (npairs * nrows + int64(WG_X) - 1l) / int64(WG_X)
        if (g_rd.kv16) {
            enc_rope_kv_f16_cls(raw, h, L.s_rope, pcr, rwgs)
        } else {
            enc_rope_kv_cls(raw, h, L.s_rope, pcr, rwgs)
        }
        rd_ts(raw, "a:rope")
    }
    let rqk = rd_attn_rqk(L)
    let rowwgs = da_attn_row_wgs(g_rd.n_heads, L.kv_mul, nsplit)   //! a row's workgroups: the kernel's row index and the grid share it
    var pca = DaAttnArgs(kvd = uint(kvd), hs = uint(hs), kv_mul = uint(L.kv_mul),
        kbase = mirbase, vbase = mirbase, scale = g_rd.scale, qstride = qstride, gated = g_rd.q_gated ? 1u : 0u,
        window = uint(L.window), cap = L.attn_cap, nsplit = uint(nsplit), rqk = rqk, sinkoff = uint(l * g_rd.n_heads),
        rowwg = uint(rowwgs), qrow = q_plane_row, cntoff = uint(g_rd.attn_cnt_off))
    if ((nrow_bx & NROW_BX_ATTN) == 0) {
        rd_enc_da_attn(raw, h, L, pca, rowwgs * nrows)
    }
    rd_ts(raw, "a:attn")
    if (rqk == DA_RQ_NONE) {   //! a head the fold cannot block (a K-quant wo plane under a head off 256): the separate requant
        rq_enc(raw, h, L.s_quant_at, L.fo, qd * nrows)
        rd_ts(raw, "a:rq_a")
    }
    if (one && L.wo_ar) {
        var pcwa = GemvArArgs(n = uint(qd), d = uint(dim), wblk = uint(arena_local_blk(L.bo)), xblk = 0u, add_on = 1u,
            woff = uint(rd_norm_off(l, RD_NORM_FFN)), pwoff = uint(rd_norm_off(l, RD_NORM_POST_ATT)), pre_on = L.pre_post ? 1u : 0u,
            no_norm = 0u, eps = g_rd.eps, ascale = 1.0, abias_on = g_rd.has_obias ? 1u : 0u, aboff = uint(rd_norm_off(l, RD_NORM_OBIAS)))
        enc_q8_gemv_ar_cls(raw, h, L.s_wo_ar, pcwa, rd_ar_wgs(dim))
        rd_ts(raw, "a:wo_ar")
        return
    }
    var pcw = GemvArgs(n = uint(qd), d = uint(dim), nreg = 1u, ybase = 0u, ncols = ncols, ystride = uint(dim))
    gemv_enc(L.fo, raw, h, L.s_wo, pcw, dim)
    rd_ts(raw, "a:wo")
}

//! the epilogue GEMV's workgroups: eight rows a 256-thread workgroup (four times the family's 64-thread rows)
def private rd_ar_wgs(rows : int64) : int64 => (rows + 4l * g_gpu.rows_per_wg - 1l) / (4l * g_gpu.rows_per_wg)

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command"), arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def private rd_encode_dn_head(raw : VkCommandBuffer; var h : VkHaz; var L : RLayer; nrows : int64; ncols : uint) {
    let dim = g_rd.dim
    let cd = g_rd.dn_cd
    let di = g_rd.dn_di
    let nvh = g_rd.dn_nvh
    let nr = uint(nrows)
    var pcqkv = GemvArgs(n = uint(dim), d = uint(cd), nreg = 1u, ybase = 0u, ncols = ncols, ystride = uint(cd + di))
    gemv_enc(L.fqkv, raw, h, L.s_dnqkv, pcqkv, cd)
    rd_ts(raw, "d:qkv")
    var pcz = GemvArgs(n = uint(dim), d = uint(di), nreg = 1u, ybase = uint(cd), ncols = ncols, ystride = uint(cd + di))
    gemv_enc(L.fz, raw, h, L.s_dnz, pcz, di)
    rd_ts(raw, "d:z")
    var bstride = uint(nvh)
    if (g_rd.dn_ba_f32) {
        bstride = uint(2l * nvh)
        var pcb = RouterArgs(ne = uint(2l * nvh), dim = uint(dim), woff = uint(L.ba_off), obase = uint(DN_SM_BETA),
            ne_split = uint(nvh), obase_split = uint(DN_SM_G), ncols = nr)
        enc_router_gemv_f16_cls(raw, h, L.s_dnba, pcb, 2l * nvh)
    } else {
        if (kq_sb(L.fqkv)) {   // the row above was requantized Q8_K for the kq GEMVs; the q8 beta/alpha GEMVs read it as Q8_0 (no stamp: the requant bills to the ba role)
            rq_enc(raw, h, L.s_quant_xb, int(KqFmt.q8), dim * nrows)
        }
        var pcb = GemvArgs(n = uint(dim), d = uint(nvh), nreg = 1u, ybase = uint(DN_SM_BETA), ncols = ncols, ystride = uint(nvh))
        gemv_enc(int(KqFmt.q8), raw, h, L.s_dnbeta, pcb, nvh)
        var pcg = GemvArgs(n = uint(dim), d = uint(nvh), nreg = 1u, ybase = uint(DN_SM_G), ncols = ncols, ystride = uint(nvh))
        gemv_enc(int(KqFmt.q8), raw, h, L.s_dnalpha, pcg, nvh)
    }
    rd_ts(raw, "d:ba")
    var pcd = DnStepArgs(cd = uint(cd), kd = uint(g_rd.dn_kd), nkh = uint(g_rd.dn_nkh), ds = uint(g_rd.dn_ds),
        dconv = uint(g_rd.dn_dconv), beta_off = uint(DN_SM_BETA), g_off = uint(DN_SM_G),
        taps_off = uint(DN_SM_TAPS), wnorm_off = uint(DN_SM_WNORM), a_off = uint(DN_SM_A),
        dt_off = uint(DN_SM_DT), hist_off = uint(DN_SM_HIST),
        eps = g_rd.eps, qscale = 1.0 / sqrt(float(g_rd.dn_ds)), out_f32 = kq_sb(L.fout) ? 1u : 0u, beta_row_stride = bstride)
    enc_dn_step_cls(raw, h, L.s_dnstep, pcd, nvh * nrows)
    rd_ts(raw, "d:step")
    if (kq_sb(L.fout)) {   // no stamp here - the requant bills to the out role
        rq_enc(raw, h, L.s_quant_dno, L.fout, di * nrows)
    }
    var pco = GemvArgs(n = uint(di), d = uint(dim), nreg = 1u, ybase = 0u, ncols = ncols, ystride = uint(dim))
    gemv_enc(L.fout, raw, h, L.s_dnout, pco, dim)
    rd_ts(raw, "d:out")
}

//! the token command's fuse gates; a gated, partial-rope or v-normed model rides the fused qk-norm+rope pair whatever the fuse lever says - the split pair lacks those arms
def private rd_fuse_gates : tuple<fuse : bool; fuse_qkr : bool> {
    let fuse = vk_fuse_on()
    var need = false
    for (L in g_rd.layers) {
        need = need || (!L.recurrent && (L.rot != L.hs || L.vfk || L.vnorm))
    }
    return (fuse = fuse, fuse_qkr = g_rd.qk_norm && ((fuse && (vk_fuse_bisect_bits() & FUSE_BX_QKN_ROPE) == 0l) || g_rd.q_gated || need))
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private rd_ts(raw : VkCommandBuffer; name : string) {
    let n0 = g_pfq_n
    pfq_ts(raw)
    if (g_pfq_n != n0) {
        g_rdq_stamp_names |> push(name)
    } elif (g_pfq_on) {
        g_rdq_overflow = true
    }
}

//! a layer's three profile tables by stamp-name prefix: its head's, its dense triple's (a MoE layer's is its shared expert) and its tail's
[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private rd_head_pfx(L : RLayer) : string => L.recurrent ? "d:" : "a:"

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private rd_ffn_pfx(L : RLayer) : string => L.moe ? "m:sh_" : rd_head_pfx(L)

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private rd_tail_pfx(L : RLayer) : string => L.moe ? "m:" : rd_head_pfx(L)

def private rd_encode_ffn(raw : VkCommandBuffer; var h : VkHaz; var L : RLayer; l, nxt, nrows : int64; ncols : uint; nrow_bx : int) {
    let dim = g_rd.dim
    let hid = L.hid
    let pfx = rd_ffn_pfx(L)
    let act = L.gelu ? ACT_GELU : ACT_SILU
    let one = nrows == 1l
    let nr = uint(nrows)
    if (L.gu_on && (nrow_bx & (NROW_BX_NCOL1 | NROW_BX_ACTRQ)) == 0) {
        if (one) {
            var pcgu = GuArgs(n = uint(dim), d = uint(hid), gblk = uint(arena_local_blk(L.b1)), ublk = uint(arena_local_blk(L.b3)), act = act)
            enc_q8_gemv_gu_cls(raw, h, L.s_gu, pcgu, hid / 32l)
        } else {
            var pcgu = GuNArgs(n = uint(dim), d = uint(hid), gblk = uint(arena_local_blk(L.b1)), ublk = uint(arena_local_blk(L.b3)), act = act, ncols = nr)
            q8_gemv_gu_n_enc(raw, h, L.s_gu_n, pcgu, hid / 32l)
        }
        rd_ts(raw, L.moe ? "m:sh_gu" : "{pfx}gu")
    } else {
        var pcg = GemvArgs(n = uint(dim), d = uint(hid), nreg = 1u, ybase = 0u, ncols = ncols, ystride = uint(hid))
        gemv_enc(L.f1, raw, h, L.s_gate, pcg, hid)
        rd_ts(raw, "{pfx}gate")
        gemv_enc(L.f3, raw, h, L.s_up, pcg, hid)
        rd_ts(raw, "{pfx}up")
        if ((nrow_bx & NROW_BX_ACTRQ) == 0) {
            if (kq_sb(L.f2)) {
                var pc_act_k = ActArgs(nelem = uint(hid) * nr, act = act, nblk = uint(hid / 256l) * nr)
                enc_q8k_actrq_cls(raw, h, L.s_actrq, pc_act_k, (hid * nrows / 8l + 255l) / 256l)
            } else {
                var pc_act = ActArgs(nelem = uint(hid) * nr, act = act, nblk = uint(hid / 32l) * nr)
                enc_q8_actrq_cls(raw, h, L.s_actrq, pc_act, (hid * nrows / 4l + 255l) / 256l)
            }
            rd_ts(raw, L.moe ? "m:sh_act" : "{pfx}actrq")
        }
    }
    if (one && L.down_ar) {   //! the post-FFN residual step rides the GEMV's last workgroup (a PLE layer's quantizes the row as it is, unscaled: the gate's feed)
        var pcda = GemvArArgs(n = uint(hid), d = uint(dim), wblk = uint(arena_local_blk(L.b2)), xblk = 0u, add_on = 1u,
            woff = uint(nxt), pwoff = uint(rd_norm_off(l, RD_NORM_POST_FFN)), pre_on = L.pre_post ? 1u : 0u,
            no_norm = L.ple ? 1u : 0u, eps = g_rd.eps, ascale = L.ple ? 1.0 : L.out_scale)
        enc_q8_gemv_ar_cls(raw, h, L.s_down_ar, pcda, rd_ar_wgs(dim))
        rd_ts(raw, "{pfx}down_ar")
        return
    }
    var pcd = GemvArgs(n = uint(hid), d = uint(dim), nreg = 1u, ybase = 0u, ncols = ncols, ystride = uint(dim))
    gemv_enc(L.f2, raw, h, L.s_down, pcd, dim)
    rd_ts(raw, "{pfx}down")
}

//! The per-layer-embedding branch after the FFN residual, over `nrows` rows: gelu(gate . x) x side[l] -> proj -> ffnout (the next-norm step adds it under the ple post norm)
def private rd_encode_ple(raw : VkCommandBuffer; var h : VkHaz; var L : RLayer; l, nrows : int64; ncols : uint) {
    let dim = g_rd.dim
    let ple = g_rd.ple
    let nr = uint(nrows)
    if (!L.ple_fused) {   //! unfused: the gate's feed still wants its own Q8_0 quants of the rows
        var pcx = RqArgs(inbase = 0u, nblk = uint(dim / 32l) * nr)
        enc_cls_dn_rq(raw, h, L.s_ple_x, pcx, (dim * nrows / 4l + 255l) / 256l)
        rd_ts(raw, "p:rq_x")
    }
    var pcg = GemvArgs(n = uint(dim), d = uint(ple), nreg = 1u, ybase = 0u, ncols = ncols, ystride = uint(ple))
    gemv_enc(L.fpg, raw, h, L.s_ple_gate, pcg, ple)
    rd_ts(raw, "p:gate")
    if (L.ple_fused) {
        var pcf = PleActArgs(n = uint(ple), d = uint(dim), wblk = uint(arena_local_blk(L.bpp)), ubase = uint(l * ple), act = ACT_GELU,
            ncols = nr, sstride = uint(g_rd.n_layers * ple))
        enc_q8_gemv_pleact_cls(raw, h, L.s_ple_pa, pcf, rd_ar_wgs(dim))
        rd_ts(raw, "p:actproj")
        return
    }
    var pca = ActArgs(nelem = uint(ple) * nr, act = ACT_GELU, nblk = uint(ple / 32l) * nr,
        ubase = uint(l * ple), ustride = uint(g_rd.n_layers * ple), ulen = uint(ple))
    enc_q8_actrq_cls(raw, h, L.s_ple_act, pca, (ple * nrows / 4l + 255l) / 256l)
    rd_ts(raw, "p:act")
    var pcp = GemvArgs(n = uint(ple), d = uint(dim), nreg = 1u, ybase = 0u, ncols = ncols, ystride = uint(dim))
    gemv_enc(L.fpg, raw, h, L.s_ple_proj, pcp, dim)
    rd_ts(raw, "p:proj")
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def private rd_encode_moe(raw : VkCommandBuffer; var h : VkHaz; var L : RLayer; l, nrows : int64) {
    let dim = g_rd.dim
    let nfe = g_rd.nfe
    let k = g_rd.k
    let one = nrows == 1l
    let nr = uint(nrows)
    if (L.moe_g4) {   //! the routed feed: x under the routed branch's pre norm into xb (the dense triple's requant took the FFN-normed row already)
        var pcn = ArArgs(dim = uint(dim), add_on = 0u, woff = uint(rd_norm_off(l, RD_NORM_PRE_FFN2)), eps = g_rd.eps, ascale = 1.0)
        if (one && L.pre2_fused) {
            enc_ar_rq_stamp(L.pre2_stamp, raw, h, L.s_pre2_rq, pcn)
            rd_ts(raw, "m:pre2_rq")
        } else {
            enc_cls_ar(raw, h, L.s_addr_ffn, pcn, nrows)
            rd_ts(raw, "m:pre2")
        }
    }
    if (L.xe_own && !(one && L.pre2_fused)) {
        rq_enc(raw, h, L.s_quant_xe, L.fe1, dim * nrows)
        rd_ts(raw, "m:rq_e")
    }
    if (L.moe_g4 && !L.rtn_fused) {   //! the router's input: a weightless rms of x under the router scale over sqrt(dim), into xb
        var pcn = ArArgs(dim = uint(dim), add_on = 0u, woff = uint(rd_norm_off(l, RD_NORM_ROUTER)), eps = g_rd.eps, ascale = 1.0)
        enc_cls_ar(raw, h, L.s_addr_ffn, pcn, nrows)
        rd_ts(raw, "m:rtn")
    }
    var pcr = RouterArgs(ne = uint(g_rd.ner), dim = uint(dim), woff = uint(L.router_slot * g_rd.ner * dim), obase = 0u,
        norm_on = L.rtn_fused ? 1u : 0u, nwoff = uint(rd_norm_off(l, RD_NORM_ROUTER)), eps = g_rd.eps, ncols = nr)
    enc_router_gemv_cls(raw, h, L.s_router, pcr, g_rd.ner)
    rd_ts(raw, "m:router")
    var pct = TopkArgs(ne = uint(g_rd.ne), k = uint(k), norm = g_rd.norm_topk ? 1u : 0u, wscale = g_rd.wscale,
        dsoff = L.moe_g4 ? uint(L.router_slot * g_rd.ne) : NO_DSCALE,
        rbias_on = g_rd.router_bias ? 1u : 0u, rboff = uint(L.router_slot * g_rd.ne),
        base1 = uint(arena_local_blk(L.be1)), stride1 = uint(nfe * dim / fmt_unit(L.fe1)),
        base3 = uint(arena_local_blk(L.be3)), stride3 = uint(nfe * dim / fmt_unit(L.fe3)),
        base2 = uint(arena_local_blk(L.be2)), stride2 = uint(nfe * dim / fmt_unit(L.fe2)),
        xnb2 = uint(nfe / fmt_unit(L.fe2)), lstride = uint(g_rd.ner), xnb1 = uint(dim / fmt_unit(L.fe1)))
    if (one) {
        enc_topk_cls(raw, h, L.s_topk, pct, 1l)
    } else {
        enc_topk_n_cls(raw, h, L.s_topk_n, pct, nrows)
    }
    rd_ts(raw, "m:topk")
    var pcg = GemvArgs(n = uint(dim), d = uint(nfe), nreg = uint(k) * nr, ybase = 0u)
    if (one && L.egu) {
        pcg.lanes_per_row = gemv_lanes_per_row(L.fe1, dim)
        pcg.act = g_rd.is_gelu ? ACT_GELU : ACT_SILU
        enc_kq_gemv_k4_gu_cls(raw, h, L.s_egu, pcg, k * nfe / 32l)
        rd_ts(raw, "m:e_gu")
    } elif (one && L.egu_mx4) {
        pcg.act = act_code(g_rd.act_oai, g_rd.is_gelu)
        pcg.bias_on = g_rd.exps_bias ? 1u : 0u
        pcg.gboff = uint(L.router_slot * g_rd.ne * nfe)
        pcg.uboff = uint(L.router_slot * g_rd.ne * nfe)
        enc_mx4_gemv_gu_cls(raw, h, L.s_egu, pcg, k * nfe / 32l)
        rd_ts(raw, "m:e_gu")
    } else {
        gemv_enc(L.fe1, raw, h, L.s_egate, pcg, k * nfe * nrows)
        rd_ts(raw, "m:e_gate")
        gemv_enc(L.fe3, raw, h, L.s_eup, pcg, k * nfe * nrows)
        rd_ts(raw, "m:e_up")
        let dkq = kq_sb(L.fe2)
        let slot_elems = k * nfe * nrows
        var pca = ActArgs(nelem = uint(slot_elems), act = act_code(g_rd.act_oai, g_rd.is_gelu), nblk = uint(slot_elems / (dkq ? 256l : 32l)),
            bias_on = g_rd.exps_bias ? 1u : 0u, gboff = uint(L.router_slot * g_rd.ne * nfe), uboff = uint(L.router_slot * g_rd.ne * nfe), brow = uint(nfe))
        let awgs = (slot_elems / (dkq ? 8l : 4l) + 255l) / 256l
        if (g_rd.exps_bias) {
            verify(!dkq, "vk decode: biased expert stacks feed a Q8_K down plane - the biased act has no Q8_K twin")
            enc_q8_actrq_b_cls(raw, h, L.s_eact, pca, awgs)
        } elif (dkq) {
            enc_q8k_actrq_cls(raw, h, L.s_eact, pca, awgs)
        } else {
            enc_q8_actrq_cls(raw, h, L.s_eact, pca, awgs)
        }
        rd_ts(raw, "m:e_act")
    }
    var pcd = GemvArgs(n = uint(nfe), d = uint(dim), nreg = uint(k) * nr, ybase = 0u)
    if (one && L.edown_sum) {
        enc_q51_gemv_sum_cls(raw, h, L.s_edown_sum, pcd, gemv_wgs(dim, 0u))
        rd_ts(raw, "m:e_down_sum")
    } else {
        gemv_enc(L.fe2, raw, h, L.s_edown, pcd, k * dim * nrows)
        rd_ts(raw, "m:e_down")
    }
}

//! The residual step's push block over a MoE layer's combine: the shared row at its gate when the layer has
//! one, the k routed rows through the slot map, then the norm at `woff`.
def rd_ar_comb_args(L : RLayer; l, woff : int64) : ArArgs {
    if (L.moe_g4) {   //! both branches normed, their sum normed, the step scaled: the gemma-4 form's rows
        return ArArgs(dim = uint(g_rd.dim), add_on = 1u, woff = uint(woff), eps = g_rd.eps, ascale = L.out_scale,
            slots = uint(g_rd.k), pwoff = uint(rd_norm_off(l, RD_NORM_POST_FFN1)),
            routed_post_off = uint(rd_norm_off(l, RD_NORM_POST_FFN2)), sum_post_off = uint(rd_norm_off(l, RD_NORM_POST_FFN)))
    }
    return ArArgs(dim = uint(g_rd.dim), add_on = has_dense_triple(L) ? 1u : 0u, woff = uint(woff), eps = g_rd.eps, ascale = 1.0,
        slots = uint(g_rd.k), lstride = uint(g_rd.ner), gated = g_rd.sh_gated ? 1u : 0u,
        dbias_on = g_rd.exps_bias ? 1u : 0u, dboff = uint(L.router_slot * g_rd.ne * g_rd.dim))
}

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]   // recording leg (also the rerecord A/B probe) — not the steady-state token path
def private rd_encode_token(raw : VkCommandBuffer; nrows, nsplit : int64) {
    let dim = g_rd.dim
    let nlfin = rd_norm_final()   // rms_final offset in the norms buffer
    let one = nrows == 1l
    let nr = uint(nrows)
    let nrow_bx = one ? 0 : g_rd_nrow_bisect   //! the bisect knob reaches the N-row command alone
    let ncols = (nrow_bx & NROW_BX_NCOL1) != 0 ? 1u : nr
    unsafe {
        let rf : VkCommandBufferResetFlags
        vk_check(vkResetCommandBuffer(raw, rf), null)
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        pfq_begin(raw)
        g_rdq_stamp_names |> clear()
        g_rdq_overflow = false
        rd_ts(raw, "")   // t0 anchor — the recorded-once cmd re-stamps every token submit
        var h : VkHaz
        vhz_dep(raw, h, 0u, VHZ_X, true)
        cmd_copy_whole(raw, g_rd.x_host.buf, g_rd.x_dev, dim * 4l * nrows)
        vhz_dep(raw, h, 0u, VHZ_COS, true)
        cmd_copy_whole(raw, g_rd.cos_host.buf, g_rd.cos_dev, g_rd.cos_elems * 4l * nrows)
        if (g_rd.ple > 0l) {   //! the side-input rows ride the cos plane's hazard bit: written here, read by the act step
            cmd_copy_whole(raw, g_rd.ple_host.buf, g_rd.ple_dev, g_rd.n_layers * g_rd.ple * 4l * nrows)
        }
        rd_ts(raw, "p:upload")
        if (g_rd.ple > 0l && g_rd.ple_gpu) {   //! the pre-step's finish: the projection of the embedded rows, normed a slice at a time and averaged into the gathered rows
            let all = g_rd.n_layers * g_rd.ple
            var pcr = RouterArgs(ne = uint(all), dim = uint(dim), woff = 0u, obase = 0u, norm_on = 0u, nwoff = 0u, eps = g_rd.eps, ncols = nr, ne_split = 0u, obase_split = 0u)
            enc_router_gemv_f16_cls(raw, h, g_rd.s_tok_pleproj, pcr, all)
            rd_ts(raw, "t:pleproj")
            var pca = ArArgs(dim = uint(g_rd.ple), add_on = 1u, woff = 0u, eps = g_rd.eps, ascale = 1.0 / sqrt(2.0), pre_on = 1u, pwoff = 0u)
            enc_cls_ar(raw, h, g_rd.s_tok_ple_ar, pca, g_rd.n_layers * nrows)
            rd_ts(raw, "t:plefin")
        }
        let (fuse, fuse_qkr) = rd_fuse_gates()
        if (one && fuse) {
            rd_ensure_fused_sets()
        }
        var pc_pro = ArArgs(dim = uint(dim), add_on = 0u, woff = 0u, eps = g_rd.eps, ascale = 1.0)
        //! the rows form fuses a site wherever its feed is Q8_0 (the `rq_b` stamp, a row a workgroup); the one-row form fuses under the lever and its bisect
        let bx = vk_fuse_bisect_bits()
        let f0 = rd_head_feed(g_rd.layers[0])
        let pro_fused = one ? fuse && (bx & FUSE_BX_PROLOGUE_RQ) == 0l : rd_rows_fuse(f0)
        let att_fused = one && fuse && (bx & FUSE_BX_POST_ATT_RQ) == 0l
        let next_fused = one && fuse && (bx & FUSE_BX_POST_FFN_RQ) == 0l
        if (pro_fused) {   // rms_att[0](x) -> xq/xs (and xb where layer 0's head reads it)
            if (one) {
                enc_ar_rq_stamp(g_rd.rq_pro_stamp, raw, h, g_rd.s_prologue_rq, pc_pro)
            } else {
                enc_ar_rq_stamp(RqStamp.rq_b, raw, h, g_rd.s_prologue_rq_b, pc_pro, nrows)
            }
        } else {
            enc_cls_ar(raw, h, g_rd.s_prologue, pc_pro, nrows)   // xb = rms_att[0](x)
        }
        rd_ts(raw, "p:prologue")
        //! the head's feed is quantized already: the fused prologue, a fused next site, or a MoE layer's quantizing combine (which leaves no float row to requant)
        var xq_ready = pro_fused
        for (l in range64(g_rd.n_layers)) {
            var L & = g_rd.layers[l]
            let hpfx = rd_head_pfx(L)
            let tpfx = rd_tail_pfx(L)
            if (!xq_ready) {
                rq_enc(raw, h, L.s_quant_xb, L.recurrent ? L.fqkv : L.fq, dim * nrows)
                rd_ts(raw, "{hpfx}rq_x")
            }
            if (L.recurrent) {
                rd_encode_dn_head(raw, h, L, nrows, ncols)
            } else {
                rd_encode_attn_head(raw, h, L, l, fuse_qkr, nlfin, rd_layer_pieces(L, nsplit), nrows, ncols, nrow_bx)
            }
            var pc8 = ArArgs(dim = uint(dim), add_on = 1u, woff = uint(rd_norm_off(l, RD_NORM_FFN)), eps = g_rd.eps, ascale = 1.0,
                pre_on = L.pre_post ? 1u : 0u, pwoff = uint(rd_norm_off(l, RD_NORM_POST_ATT)),
                abias_on = g_rd.has_obias ? 1u : 0u, aboff = uint(rd_norm_off(l, RD_NORM_OBIAS)))
            let ffn_fused = one ? att_fused : (!kq_sb(L.f1) && !L.moe)   //! the rows form: the shared expert's feed, or on a bare routed block the experts' own (their format is the layer's f1)
            if (ffn_fused) {
                if (!(one && L.wo_ar)) {   //! else the wo GEMV's epilogue took the step
                    if (one) {
                        enc_ar_rq_stamp(L.rq_ffn_stamp, raw, h, L.s_addr_ffn_rq, pc8)
                    } else {
                        enc_ar_rq_stamp(RqStamp.rq_b, raw, h, L.s_addr_ffn_rq_b, pc8, nrows)
                    }
                    rd_ts(raw, "{tpfx}arrq_f")
                }
            } else {
                enc_cls_ar(raw, h, L.s_addr_ffn, pc8, nrows)
                rd_ts(raw, "{tpfx}ar1")
                rq_enc(raw, h, L.s_quant_xb2, L.f1, dim * nrows)
                rd_ts(raw, "{tpfx}rq_f")
            }
            let nxt = l + 1l < g_rd.n_layers ? rd_norm_off(l + 1l, RD_NORM_ATT) : nlfin   //! rms_att[l+1] | final
            let fn = rd_next_feed(l)
            if (has_dense_triple(L)) {
                rd_encode_ffn(raw, h, L, l, nxt, nrows, ncols, nrow_bx)
            }
            if (L.moe) {
                rd_encode_moe(raw, h, L, l, nrows)
            }
            if (L.ple) {
                // the FFN residual first (no scale, the normed output unused); the branch leaves its projection in ffnout for the step below
                var pc14a = ArArgs(dim = uint(dim), add_on = 1u, woff = uint(nxt), eps = g_rd.eps, ascale = 1.0,
                    pre_on = L.pre_post ? 1u : 0u, pwoff = uint(rd_norm_off(l, RD_NORM_POST_FFN)))
                if (one && L.down_ar) {   //! the down GEMV's epilogue took the step
                } elif (L.ple_fused) {   //! the step quantizes the rows it writes, as they are: the gate's feed
                    pc14a.no_norm = 1u
                    if (one) {
                        enc_ar_rq_stamp(RqStamp.rq, raw, h, L.s_ple_ar_rq, pc14a)
                    } else {
                        enc_ar_rq_stamp(RqStamp.rq_b, raw, h, L.s_ple_ar_rq_n, pc14a, nrows)
                    }
                    rd_ts(raw, "{tpfx}ar2a_rq")
                } else {
                    enc_cls_ar(raw, h, L.s_addr_next, pc14a, nrows)
                    rd_ts(raw, "{tpfx}ar2a")
                }
                rd_encode_ple(raw, h, L, l, nrows, ncols)
            }
            var pc14 = ArArgs(dim = uint(dim), add_on = 1u, woff = uint(nxt), eps = g_rd.eps, ascale = L.out_scale,
                pre_on = (L.pre_post || L.ple) ? 1u : 0u, pwoff = uint(rd_norm_off(l, L.ple ? RD_NORM_PLE_POST : RD_NORM_POST_FFN)))
            if (L.moe) {
                var pc14c = rd_ar_comb_args(L, l, nxt)
                pc14c.presummed = (one && L.edown_sum) ? 1u : 0u   //! the one-row command's summing down GEMV folded the k rows; the rows and the window chain write them apart
                if (L.comb_rq && L.moe_g4) {   //! the next head's Q8_0 feed quantized here
                    enc_cls_ar_comb_g4_rq(raw, h, L.s_comb_rq, pc14c, nrows)
                } elif (L.comb_rq) {
                    enc_cls_ar_comb_rq(raw, h, L.s_comb_rq, pc14c, nrows)
                } elif (L.moe_g4) {
                    enc_cls_ar_comb_g4(raw, h, L.s_addr_next, pc14c, nrows)
                } else {
                    enc_cls_ar_comb(raw, h, L.s_addr_next, pc14c, nrows)
                }
                rd_ts(raw, "m:ar2")
                xq_ready = L.comb_rq
                continue
            }
            let nxt_fused = one ? next_fused : rd_rows_fuse(fn)
            if (nxt_fused) {
                if (!(one && L.down_ar) || L.ple) {   //! a PLE layer's down epilogue took the FFN step; this is the branch's
                    if (one) {
                        enc_ar_rq_stamp(L.rq_next_stamp, raw, h, L.s_addr_next_rq, pc14)
                    } else {
                        enc_ar_rq_stamp(RqStamp.rq_b, raw, h, L.s_addr_next_rq_b, pc14, nrows)
                    }
                    rd_ts(raw, "{tpfx}arrq_n")
                }
            } else {
                enc_cls_ar(raw, h, L.s_addr_next, pc14, nrows)
                rd_ts(raw, "{tpfx}ar2")
            }
            xq_ready = nxt_fused
        }
        if (!xq_ready) {   //! a split last site stored the normed rows: quantized here for the classifier
            rq_enc(raw, h, g_rd.s_quant_final, g_rd.cls_fmt_marker, dim * nrows)
            rd_ts(raw, "t:fin_rq")
        }
        var pcc = GemvArgs(n = uint(dim), d = uint(g_rd.vocab), nreg = 1u, ybase = 0u, ncols = ncols, ystride = uint(g_rd.vocab))
        gemv_enc(g_rd.cls_fmt_marker, raw, h, g_rd.cls_set, pcc, g_rd.vocab)
        rd_ts(raw, "t:cls")
        if (rd_cls_epi_on()) {
            rd_enc_cls_epi(raw, h, nrows)
            rd_ts(raw, "t:epi")
        }
        var pcp = ArgmaxArgs(vocab = uint(g_rd.vocab))   //! the passes run every step; the ask decides only what lands (`rd_submit_land`)
        enc_cls_argmax_part(raw, h, g_rd.s_pick_part, pcp, nrows * RD_PICK_CHUNKS)
        enc_cls_argmax_fin(raw, h, g_rd.s_pick_fin, pcp, nrows)
        rd_ts(raw, "t:pick")
        if (!g_rd.log_xfer) {   //! the transfer queue carries the logits down (`rd_xlog`); without one the command copies them itself
            vhz_dep(raw, h, VHZ_LOG, 0u, true)   // compute -> transfer for the logits DMA
            cmd_copy_whole(raw, g_rd.logits_dev, g_rd.logits_host.buf, g_rd.vocab * 4l * nrows)
            cmd_copy_whole(raw, g_rd.pick_dev, g_rd.pick_host.buf, 4l * nrows)
            rd_ts(raw, "t:dma")   //! the logits' trip to host memory, billed apart: on a box whose host-visible memory is slow it is the step's largest interval
        }
        pfq_end(raw)
        vk_check(vkEndCommandBuffer(raw), null)
    }
}

def private rdq_group_line(pfx : string; n : double) : string {
    return build_string() $(w) {
        for (nm in g_rdq_print_order) {
            if (nm |> starts_with(pfx)) {
                w |> write("{slice(nm, length(pfx))} {int((g_rdq_us_by_name?[nm] ?? 0.0lf) / n)} ")
            }
        }
    }
}

var private g_rdq_prev_last = 0ul   //! the last stamp of the command sampled before, on the device clock
var private g_rdq_idle_us = 0.0lf   //! the device's idle between one sampled command's last stamp and the next one's first, summed

[cold_path]   // DASLLAMA_GPU_PROF reporting leg
def private rdq_sample(names : array<string>; n : uint; overflow : bool) {
    let want = length(names)
    unsafe {   //! the idle between commands: what the host spent between them, plus whatever the device holds outside the stamps
        var tp = reinterpret<uint64?>(g_pfq_buf.mapped)
        if (g_rdq_prev_last != 0ul && g_rdq_count > 0l) {
            g_rdq_idle_us += double(tp[0u] - g_rdq_prev_last) * double(g_pfq_period) / 1000.0lf
        }
        g_rdq_prev_last = tp[n - 1u]
    }
    if (overflow || n != uint(want)) {
        // a bare stamp or a full query pool: the whole command's GPU span reads off the first and last stamps
        unsafe {
            var tp = reinterpret<uint64?>(g_pfq_buf.mapped)
            g_rdq_total += double(tp[n - 1u] - tp[0u]) * double(g_pfq_period) / 1000.0lf
        }
        if (g_rdq_count == 0l) {   // the first token: the eight longest stamp gaps, by stamp index, name the dispatches worth a look
            var top <- [for (i in range(1, int(n))); (us = pfq_us(uint(i)), at = uint(i))]
            top |> sort() $(a, b) => a.us > b.us
            let line = build_string() $(w) {
                for (k in range(min(8, length(top)))) {
                    w |> write("#{top[k].at} {int(top[k].us)}us ")
                }
            }
            to_log(LOG_INFO, "vk_rdec first token: {n} stamps, longest gaps {line}\n")
            delete top
        }
        g_rdq_count++
        if (g_rdq_count % 32l == 0l) {
            let why = overflow ? "the query pool filled at PFQ_CAP" : "{n} stamps, the recorder named {want}"
            to_log(LOG_INFO, "vk_rdec gpu avg/token over {g_rdq_count}: total {int(g_rdq_total / double(g_rdq_count))} us (roles unaggregated: {why})\n")
        }
        return
    }
    for (i in range(1, want)) {   // stamp 0 is the anchor
        let nm = names[i]
        if (!key_exists(g_rdq_us_by_name, nm)) {
            g_rdq_print_order |> push(nm)
            g_rdq_us_by_name[nm] = 0.0lf
        }
        g_rdq_us_by_name[nm] += pfq_us(uint(i))
    }
    unsafe {
        var tp = reinterpret<uint64?>(g_pfq_buf.mapped)
        g_rdq_total += double(tp[n - 1u] - tp[0u]) * double(g_pfq_period) / 1000.0lf
    }
    g_rdq_count++
    if (g_rdq_count % 32l == 0l) {
        let cnt = double(g_rdq_count)
        var ndn = 0l
        var nmoe = 0l
        for (L in g_rd.layers) {
            ndn += L.recurrent ? 1l : 0l
            nmoe += L.moe ? 1l : 0l
        }
        let natt = long_length(g_rd.layers) - ndn
        to_log(LOG_INFO, "vk_rdec gpu avg/token over {g_rdq_count}: {rdq_group_line("p:", cnt)}{rdq_group_line("a:", cnt)}{rdq_group_line("t:", cnt)}total {int(g_rdq_total / cnt)} (attention layers {natt}, recurrent {ndn}, MoE {nmoe}); idle between commands {int(g_rdq_idle_us / cnt)}\n")
        if (nmoe > 0l) {
            to_log(LOG_INFO, "vk_rdec moe avg/token (the MoE layers' tails, summed over the layers): {rdq_group_line("m:", cnt)}\n")
        }
        if (ndn > 0l) {
            to_log(LOG_INFO, "vk_rdec dn avg/token: {rdq_group_line("d:", cnt)}\n")
        }
    }
}

let private RD_PAGING_BYTES_PER_US = 40_000l   // 40 GB/s, the PCIe class

//! The resident driver's device weight bytes (rdec_note_weight_bytes's seat; the plan hands them over after prepare)
def vk_rdec_note_weight_bytes(bytes : int64) {
    if (g_rd != null) {
        g_rd.weight_bytes = bytes
    }
}

//! whether the step at `served` tokens so far reads its logits row for the guard: the first four tokens and every 256th
def private rd_guard_step(served : int64) : bool => served < 4l || ((served + 1l) & 255l) == 0l

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]
def private rd_logits_guard(logits : float const?; n : int64) {
    for (i in range64(n)) {
        let v = unsafe(logits[i])
        return if (v != 0.0 && v > -1.0e29)   //! a suppressed id's pin is not a live logit
    }
    panic("dasLLAMA vulkan tier: the device returned an all-zero logits row - the card is over-committed (planes demoted past what the driver can page back) or lost; close other GPU applications, lower DASLLAMA_GPU_VRAM_MB, or restart")
}

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]   // once per armed driver
def private rd_warmup_check(wall_us : int64) {
    vk_demotion_check("decode warm-up")
    if (g_rd.weight_bytes > 0l && wall_us > g_rd.weight_bytes / RD_PAGING_BYTES_PER_US) {
        to_log(LOG_WARNING, "dasLLAMA vulkan tier: a decode token ran under {RD_PAGING_BYTES_PER_US / 1_000l} GB/s over {g_rd.weight_bytes / 1_000_000l} MB of resident weights - the paging class; close other GPU applications or lower DASLLAMA_GPU_VRAM_MB\n")
    }
}

//! the logits' trip to host memory on the transfer queue: the copy command for `nrows` rows (the rows' picks behind them), or
//! under `picks_only` the picks alone, each recorded on first use
def private rd_xlog(nrows : int64; picks_only : bool) : VkCommandBuffer {
    var raw = picks_only ? g_rd.xlog_pick_cmd[nrows] : g_rd.xlog_cmd[nrows]
    let recorded = picks_only ? g_rd.xlog_pick_recorded[nrows] : g_rd.xlog_recorded[nrows]
    if (!recorded) {
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        if (!picks_only) {
            cmd_copy_whole(raw, g_rd.logits_dev, g_rd.logits_host.buf, g_rd.vocab * 4l * nrows)
        }
        cmd_copy_whole(raw, g_rd.pick_dev, g_rd.pick_host.buf, 4l * nrows)
        vk_check(vkEndCommandBuffer(raw), null)
        if (picks_only) {
            g_rd.xlog_pick_recorded[nrows] = true
        } else {
            g_rd.xlog_recorded[nrows] = true
        }
    }
    return raw
}

var private g_rd_xlog_wait = 0ul   //! the transfer timeline value the step's landing (logits or picks) completes at (0 = the command carried its own copy, the fence is the wait)

[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#logits-transfer-queue")]
def private rd_submit_land(var cmd : VkCommandBuffer; nrows : int64; picks_only : bool) {
    if (g_rd.log_xfer) {
        let cv = submit_signal(cmd)
        g_rd_xlog_wait = xfer_submit_after(rd_xlog(nrows, picks_only), cv)
    } else {
        submit_nowait(cmd)
        g_rd_xlog_wait = 0ul
    }
}

//! ... and its wait: the step's landing is in host memory when it returns, and the next submit starts a new one
def private rd_wait_land {
    if (g_rd_xlog_wait != 0ul) {
        xfer_wait(g_rd_xlog_wait, true)
        g_rd_xlog_wait = 0ul
    } else {
        wait_fence()
    }
}

//! Resident-driver token: the whole stack on device from the embedded residual `x` and this position's rope row -> logits, or
//! under `pick_only` the row's argmax id alone (`logits` unwritten; a guard step lands the logits too, so the over-commit check
//! keeps its cadence). `cnt` = cached positions incl. this one; K/V for `pos` store into the mirror. Recorded once.
[hot_path]   // the per-token resident submit — no goo between the memcpys and the fence
def vk_rdec_token(x : array<float>; cossin : array<float>; ple_row : array<float>; pos, cnt : int64; pick_only : bool; var logits : array<float>) : int64 {
    assert(g_rd != null && g_rd.ready, "vk_rdec_token before prepare/set_cls")
    if (!g_rd.tok_recorded[g_rd.region]) {
        rd_record_token()
    } elif (vk_rdec_rerecord_on()) {   // probe arm: pay the full re-encode every token, the split form and its twins
        rd_record_unsplit_twin()
        rd_encode_token(g_rd.cmd[g_rd.region], 1l, rd_split_pieces())
        g_rd.tok_wide_recorded[g_rd.region] = false
    }
    let form = rd_form_at(pos)
    if (form == RdForm.wide && !g_rd.tok_wide_recorded[g_rd.region]) {
        rd_record_wide_twin()
    }
    let ts0 = ref_time_ticks()
    let prof = vk_prof()
    let unsplit = form == RdForm.unsplit
    var cmd = g_rd.cmd[g_rd.region]
    if (unsplit) {
        cmd = g_rd.cmd_unsplit[g_rd.region]
    } elif (form == RdForm.wide) {
        cmd = g_rd.cmd_wide[g_rd.region]
    }
    if (unsplit) {
        g_rd.unsplit_served++
    }
    var picked = -1l
    let guard = rd_guard_step(g_rd.tokens_served)
    let picks_only = pick_only && !guard
    unsafe {
        memcpy(g_rd.x_host.mapped, addr<void?>(x[0]), g_rd.dim * 4l)
        memcpy(g_rd.cos_host.mapped, addr<void?>(cossin[0]), g_rd.cos_elems * 4l)
        if (g_rd.ple > 0l) {
            assert(long_length(ple_row) >= g_rd.n_layers * g_rd.ple, "vk_rdec_token: the per-layer-embedding row is short")
            memcpy(g_rd.ple_host.mapped, addr<void?>(ple_row[0]), g_rd.n_layers * g_rd.ple * 4l)
        }
        var tm = reinterpret<TokMeta?>(g_rd.tok_meta.mapped)
        tm.pos = uint(pos)   // every carrier composes its mirror row as layerbase + pos * kvd
        tm.cnt = uint(cnt)
        tm.parity = g_rd.hybrid ? uint(g_rd.dn_parity[g_rd.region]) : 0u
        tm.mirbase = 0u   //! the one-row command's region rides its push; an N-row step leaves its row 0 entry here
        tm.dnslot = uint(g_rd.region)
        if (prof) {   //! the profiled form splits the wall into its pieces; the served form is one call
            let t1 = ref_time_ticks()
            rd_submit_land(cmd, 1l, picks_only)
            let t2 = ref_time_ticks()
            rd_wait_land()
            let t3 = ref_time_ticks()
            picked = rd_land_one(pick_only, logits)
            let t4 = ref_time_ticks()
            g_rdq_host_us[0] += double(get_time_usec(ts0)) - double(get_time_usec(t1))
            g_rdq_host_us[1] += double(get_time_usec(t1)) - double(get_time_usec(t2))
            g_rdq_host_us[2] += double(get_time_usec(t2)) - double(get_time_usec(t3))
            g_rdq_host_us[3] += double(get_time_usec(t3)) - double(get_time_usec(t4))
        } else {
            rd_submit_land(cmd, 1l, picks_only)
            rd_wait_land()
            picked = rd_land_one(pick_only, logits)
        }
    }
    g_rd.tokens_served++
    if (guard) {
        rd_logits_guard(unsafe(reinterpret<float const?>(g_rd.logits_host.mapped)), g_rd.vocab)
    }
    if (g_rd.tokens_served == 8l) {
        rd_warmup_check(int64(get_time_usec(ts0)))
    }
    if (prof) {
        g_rdq_wall_us += double(get_time_usec(ts0))
        if (g_rdq_count % 32l == 31l) {
            let n = double(g_rdq_count + 1l)
            to_log(LOG_INFO, "vk_rdec host wall/token over {g_rdq_count + 1l}: {int(g_rdq_wall_us / n)} us (memcpys {int(g_rdq_host_us[0] / n)} + submit {int(g_rdq_host_us[1] / n)} + fence {int(g_rdq_host_us[2] / n)} + logits copy {int(g_rdq_host_us[3] / n)})\n")   // nolint:PERF026,PERF028 — vk_prof()-gated report leg
        }
    }
    if (g_rd.hybrid) {
        rd_dn_advance_parity(g_rd.region)
    }
    if (prof) {   // the ledger reads the submitted form's stamps: a run crossing a form edge, or arriving from the N-row command, restarts its averages
        if (g_rdq_count > 0l && (form != g_rdq_attn_form || g_rdq_form_n)) {
            rdq_form_switch(form, pos)
            g_rdq_idle_us = 0.0lf
        }
        g_rdq_form_n = false
        g_rdq_attn_form = form
        if (form == RdForm.unsplit && g_rdq_n_unsplit > 2u) {
            rdq_sample(g_rdq_names_unsplit, g_rdq_n_unsplit, g_rdq_overflow_unsplit)
        } elif (form == RdForm.wide && g_rdq_n_wide > 2u) {
            rdq_sample(g_rdq_names_wide, g_rdq_n_wide, g_rdq_overflow_wide)
        } elif (form == RdForm.split && g_rdq_n > 2u) {
            rdq_sample(g_rdq_stamp_names, g_rdq_n, g_rdq_overflow)
        }
    }
    return picked
}

// ===== the N-row token command: a batched decode step's rows through one weight pass =====

//! the N-row command answers 0 rows for `why`, said once per armed model: the batched step runs a row at a time from here on
//! the routed planes hold `MAX_ROUTED_SLOTS` slots a step: every row's k picks must fit
def rd_routed_slots_fit(nb, k : int64) : bool => nb * k <= MAX_ROUTED_SLOTS

def private rd_n_decline(why : string) : int64 {
    if (!g_rd.n_decline_said) {
        g_rd.n_decline_said = true
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the N-row token command has no form for {why} - a batched step runs a row at a time\n")
    }
    return 0l
}

//! the rows the N-row command steps at once on the armed model; 0 where a layer or the tail has no N-row form
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def vk_rdec_token_n_rows : int64 {
    if (g_rd == null || !g_rd.ready) return 0l
    if (!gemv_cls_has_n(g_rd.cls_fmt_marker)) return rd_n_decline("the classifier's format has no N-column GEMV leaf")
    for (L in g_rd.layers) {
        if (L.recurrent) {
            if (!(gemv_cls_has_n(L.fqkv) && gemv_cls_has_n(L.fz) && gemv_cls_has_n(L.fout))) return rd_n_decline("a recurrent layer's weight format with no N-column GEMV leaf")
            if (has_dense_triple(L) && !(gemv_cls_has_n(L.f1) && gemv_cls_has_n(L.f3) && gemv_cls_has_n(L.f2))) return rd_n_decline("a recurrent layer's FFN weight format with no N-column GEMV leaf")
            continue
        }
        if (L.moe && !rd_routed_slots_fit(g_rd.nb, g_rd.k)) return rd_n_decline("more routed slots than the block's slot planes hold")
        if (L.moe && L.ple) return rd_n_decline("a per-layer-embedding layer with a routed block")
        if (!(gemv_cls_has_n(L.fq) && (L.kv_shared || (gemv_cls_has_n(L.fk) && (L.vfk || gemv_cls_has_n(L.fv)))) && gemv_cls_has_n(L.fo))
                || (has_dense_triple(L) && !(gemv_cls_has_n(L.f1) && gemv_cls_has_n(L.f3) && gemv_cls_has_n(L.f2)))
                || (L.ple && !gemv_cls_has_n(L.fpg))) return rd_n_decline("a weight format with no N-column GEMV leaf")
    }
    if (g_rd.n_rail == NRail.unasked) {   //! the command's kernel classes build once, before any step commits to it
        g_rd.n_rail = rd_ensure_n_rail() ? NRail.built : NRail.declined
    }
    return g_rd.n_rail == NRail.built ? g_rd.nb : 0l
}

//! the N-column leaves of every plane the command dispatches and its fused Q8_0 site, built on this device; false logs the decline once
[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def private rd_ensure_n_rail : bool {
    let nb = g_rd.nb
    var ok = gemv_cls_ensure_n(g_rd.cls_fmt_marker, nb) && ensure_cls_ar_rq_b()
    for (L in g_rd.layers) {
        break if (!ok)
        if (dasllama_noisy()) {
            to_log(LOG_INFO, "vk_rdec n rail: layer recurrent {L.recurrent} moe {L.moe} ple {L.ple} dense {has_dense_triple(L)} fq {L.fq} fk {L.fk} fv {L.fv} fo {L.fo} fqkv {L.fqkv} fz {L.fz} fout {L.fout} f1 {L.f1} f3 {L.f3} f2 {L.f2} fpg {L.fpg}\n")
        }
        let head_ok = (L.recurrent   //! its own statement: a `?:` binds looser than the `&&` chain below, which would then be the else arm alone
                ? (gemv_cls_ensure_n(L.fqkv, nb) && gemv_cls_ensure_n(L.fz, nb) && gemv_cls_ensure_n(L.fout, nb) && (g_rd.dn_ba_f32 || gemv_cls_ensure_n(int(KqFmt.q8), nb)))
                : (gemv_cls_ensure_n(L.fq, nb) && (L.kv_shared || (gemv_cls_ensure_n(L.fk, nb) && (L.vfk || gemv_cls_ensure_n(L.fv, nb)))) && gemv_cls_ensure_n(L.fo, nb)))
        ok = (head_ok
            && (!has_dense_triple(L) || (gemv_cls_ensure_n(L.f1, nb) && gemv_cls_ensure_n(L.f3, nb) && gemv_cls_ensure_n(L.f2, nb)))
            && (!L.gu_on || q8_gemv_gu_n_ensure(nb))   //! the fused gate-up's N form declines the rail whole, as the GEMV leaves do
            && (!L.moe || ensure_topk_n_cls())
            && (!L.ple || gemv_cls_ensure_n(L.fpg, nb)))
    }
    if (!ok) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: a class of the N-row command declined on this device (an N-column GEMV leaf, the fused add+rms+requant rows stamp, the fused gate-up N form, the per-row top-k, or the per-layer-embedding gate's N leaf) - the batched step runs a row at a time\n")
    }
    return ok
}

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def private rd_ensure_n_sets {
    if (g_rd.n_sets_made) return
    let dim = g_rd.dim
    let nb = g_rd.nb
    if (rd_rows_fuse(rd_head_feed(g_rd.layers[0]))) {
        g_rd.s_prologue_rq_b = set_ar_rq_stamp(RqStamp.rq_b, g_rd.x_dev, VHZ_X, g_rd.xq_dev, g_rd.xs_dev)
    }
    for (l in range64(g_rd.n_layers)) {
        var L & = unsafe(g_rd.layers[l])
        if (!kq_sb(L.f1)) {
            L.s_addr_ffn_rq_b = set_ar_rq_stamp(RqStamp.rq_b, g_rd.xb2_dev, VHZ_XB2, g_rd.xq_dev, g_rd.xs_dev)
        }
        if (rd_rows_fuse(rd_next_feed(l))) {
            L.s_addr_next_rq_b = set_ar_rq_stamp(RqStamp.rq_b, g_rd.ffnout_dev, VHZ_FFO, g_rd.xq_dev, g_rd.xs_dev)
        }
        if (L.gu_on) {   // the rail ensured the class before any row count recorded
            let p1 = arena_planes(L.f1, L.b1)
            let p3 = arena_planes(L.f3, L.b3)
            L.s_gu_n = q8_gemv_gu_n_set(fixed_array(p1.wq, p1.ws, p3.wq, p3.ws, g_rd.xq_dev, g_rd.xs_dev, g_gpu.hq_dev, g_gpu.hs_dev),
                fixed_array(p1.wqb, p1.wsb, p3.wqb, p3.wsb, dim * nb, (dim / 32l) * 4l * nb, BATCH_HQ_BYTES, BATCH_HS_BYTES),
                fixed_array(0u, 0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_HQ, VHZ_HQ))
        }
        if (L.moe) {
            let slots_bytes = MAX_ROUTED_SLOTS * 4l
            L.s_topk_n = set_topk_n_cls(fixed_array(g_rd.moe_dlog_dev, g_rd.moe_meta1_dev, g_rd.moe_meta3_dev, g_rd.moe_meta2_dev, g_rd.moe_dw_dev, g_rd.moe_ds_dev, g_rd.moe_rb_dev, g_rd.moe_eidx_dev),
                fixed_array(g_rd.ner * 4l * nb, META_BYTES, META_BYTES, META_BYTES, slots_bytes, g_rd.moe_ds_bytes, g_rd.moe_rb_bytes, slots_bytes),
                fixed_array(VHZ_MRT, VHZ_MRT, VHZ_MRT, VHZ_MRT, VHZ_MRT, 0u, 0u, VHZ_MRT))
        }
        if (L.ple && L.ple_fused) {
            L.s_ple_ar_rq_n = set_ar_rq_stamp(RqStamp.rq_b, g_rd.ffnout_dev, VHZ_FFO, g_rd.xq_dev, g_rd.xs_dev)
        }
    }
    rd_ensure_comb_sets()
    g_rd.n_sets_made = true
}

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def private rd_record_n_form(raw : VkCommandBuffer; nrows, nsplit : int64; form : RdForm) {
    var one_names <- g_rdq_stamp_names
    let one_overflow = g_rdq_overflow
    rd_encode_token(raw, nrows, nsplit)
    if (form == RdForm.split) {
        delete g_rdq_names_n
        g_rdq_names_n <- g_rdq_stamp_names
        g_rdq_n_n = g_pfq_n
        g_rdq_overflow_n = g_rdq_overflow
    } else {
        delete g_rdq_names_n_unsplit
        g_rdq_names_n_unsplit <- g_rdq_stamp_names
        g_rdq_n_n_unsplit = g_pfq_n
        g_rdq_overflow_n_unsplit = g_rdq_overflow
    }
    delete g_rdq_stamp_names
    g_rdq_stamp_names <- one_names
    g_rdq_overflow = one_overflow
}


//! the landed pick of row `i` as a token id: the passes leave `0xFFFFFFFF` on a row with no element (never a served row)
def private rd_landed_pick(i : int64) : int64 {
    var pk = unsafe(reinterpret<uint?>(g_rd.pick_host.mapped))
    let pick = int64(unsafe(pk[i]))
    if (pick >= g_rd.vocab) {
        panic("dasLLAMA vulkan tier: the device landed no pick for row {i} ({pick} of a {g_rd.vocab} vocab) - engine bug")
    }
    return pick
}

//! the one-row landing: the pick under `pick_only`, else the logits row into `logits` (-1)
def private rd_land_one(pick_only : bool; var logits : array<float>) : int64 {
    if (pick_only) {
        return rd_landed_pick(0l)
    }
    unsafe {
        memcpy(addr<void?>(logits[0]), g_rd.logits_host.mapped, g_rd.vocab * 4l)
    }
    return -1l
}

//! the N-row landing: each row's logits at its pointer, or - where the pointer is null - its pick into `picks` (-1 on a landed row)
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def private rd_land_rows_n(lrows : array<float?>; var picks : array<int64>; nrows : int64) {
    let row_bytes = g_rd.vocab * 4l
    var src = unsafe(reinterpret<uint8?>(g_rd.logits_host.mapped))
    var rowptrs = unsafe(addr<float ??>(lrows[0]))   // the row pointers as a raw pointer: the lanes' lambda captures no array
    for (i in range64(nrows)) {
        picks[i] = lrows[i] == null ? rd_landed_pick(i) : -1l
    }
    let jobs = min(int(nrows), get_dispatch_lanes())
    maybe_parallel_for(0, int(nrows), jobs) $(rb, re) {
        unsafe {
            for (i in range(rb, re)) {
                continue if (rowptrs[i] == null)
                memcpy(reinterpret<void?>(rowptrs[i]), reinterpret<void?>(src + int64(i) * row_bytes), row_bytes)
            }
        }
    }
}

//! the N-row submit: `nrows` rows through one command, each row's logits landed at its own pointer, or - where the
//! pointer is null - the row's argmax id alone into `picks` (`RdecTokenNFn`)
[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def vk_rdec_token_n(x : array<float>; cossin : array<float>; ple : array<float>; ple_raw : bool; poss : array<int64>; cnts : array<int64>; regs : array<int64>; nrows : int64; lrows : array<float?>; var picks : array<int64>) {
    assert(g_rd != null && g_rd.ready, "vk_rdec_token_n before prepare/set_cls")
    verify(long_length(picks) >= nrows, "vk_rdec_token_n: fewer pick slots than rows")
    verify(nrows >= 2l && nrows <= g_rd.nb, "vk_rdec_token_n: the row count is outside the per-row planes (a one-row step is the one-row command's: its region rides the push, the N-row rows' ride TokMeta)")
    verify(long_length(lrows) >= nrows, "vk_rdec_token_n: fewer logits rows than rows")
    verify(g_rd.ple <= 0l || long_length(ple) >= g_rd.n_layers * g_rd.ple * nrows, "vk_rdec_token_n: the per-layer-embedding rows are short")
    var all_pick = true   //! every row wants its argmax alone: the logits never leave the device
    for (i in range64(nrows)) {
        all_pick = all_pick && lrows[i] == null
    }
    assert(g_rd.ple <= 0l || ple_raw == g_rd.ple_gpu, "vk_rdec_token_n: raw per-layer-embedding rows without the projection on device, or finished rows with it")
    let nsplit = rd_split_pieces()
    if (!g_rd.tok_n_recorded[nrows]) {
        rd_fill_gemv_regions_all()
        rd_ensure_n_sets()
        rd_record_n_form(g_rd.cmd_n[nrows], nrows, nsplit, RdForm.split)
        if (g_rd.unsplit_on) {
            rd_record_n_form(g_rd.cmd_n_unsplit[nrows], nrows, 1l, RdForm.unsplit)
        }
        g_rd.tok_n_recorded[nrows] = true
    }
    var maxpos = 0l
    for (i in range64(nrows)) {
        maxpos = max(maxpos, poss[i])
    }
    //! the rows' form is the batch's furthest row's: the unsplit twin while every row sits under RD_UNSPLIT_POS, the split
    //! form past it at every span (the ruler reads the wide form slower at four rows, sec.2.2al)
    let unsplit = g_rd.unsplit_on && maxpos < RD_UNSPLIT_POS
    let form = unsplit ? RdForm.unsplit : RdForm.split
    let prof = vk_prof()
    let ts0 = ref_time_ticks()
    unsafe {
        memcpy(g_rd.x_host.mapped, addr<void?>(x[0]), g_rd.dim * 4l * nrows)
        memcpy(g_rd.cos_host.mapped, addr<void?>(cossin[0]), g_rd.cos_elems * 4l * nrows)
        if (g_rd.ple > 0l) {
            memcpy(g_rd.ple_host.mapped, addr<void?>(ple[0]), g_rd.n_layers * g_rd.ple * 4l * nrows)
        }
        var tm = reinterpret<TokMeta?>(g_rd.tok_meta.mapped)
        for (i in range64(nrows)) {
            tm[i].pos = uint(poss[i])
            tm[i].cnt = uint(cnts[i])
            tm[i].parity = g_rd.hybrid ? uint(g_rd.dn_parity[regs[i]]) : 0u
            tm[i].mirbase = uint(regs[i] * g_rd.mir_elems)
            tm[i].dnslot = uint(regs[i])
        }
        var cmd = unsplit ? g_rd.cmd_n_unsplit[nrows] : g_rd.cmd_n[nrows]
        if (prof) {   // the profiled form splits the wall into its pieces, as the one-row submit does
            let t1 = ref_time_ticks()
            rd_submit_land(cmd, nrows, all_pick)
            let t2 = ref_time_ticks()
            rd_wait_land()
            let t3 = ref_time_ticks()
            rd_land_rows_n(lrows, picks, nrows)
            let t4 = ref_time_ticks()
            g_rdq_host_us[0] += double(get_time_usec(ts0)) - double(get_time_usec(t1))
            g_rdq_host_us[1] += double(get_time_usec(t1)) - double(get_time_usec(t2))
            g_rdq_host_us[2] += double(get_time_usec(t2)) - double(get_time_usec(t3))
            g_rdq_host_us[3] += double(get_time_usec(t3)) - double(get_time_usec(t4))
        } else {
            rd_submit_land(cmd, nrows, all_pick)
            rd_wait_land()
            rd_land_rows_n(lrows, picks, nrows)
        }
    }
    g_rd.tokens_served += nrows
    if (unsplit) {
        g_rd.unsplit_served += nrows
    }
    if (g_rd.hybrid) {
        for (i in range64(nrows)) {
            rd_dn_advance_parity(regs[i])
        }
    }
    if (prof) {   // the ledger reads the N-row command's stamps apart from the one-row command's: a switch between them restarts the averages
        g_rdq_wall_us += double(get_time_usec(ts0))
        if (g_rdq_count % 32l == 31l) {
            let n = double(g_rdq_count + 1l)
            to_log(LOG_INFO, "vk_rdec host wall/step over {g_rdq_count + 1l} of {nrows} rows: {int(g_rdq_wall_us / n)} us (memcpys {int(g_rdq_host_us[0] / n)} + submit {int(g_rdq_host_us[1] / n)} + fence {int(g_rdq_host_us[2] / n)} + logits copy {int(g_rdq_host_us[3] / n)})\n")   // nolint:PERF026,PERF028 — vk_prof()-gated report leg
        }
        if (g_rdq_count > 0l && (!g_rdq_form_n || form != g_rdq_attn_form)) {
            rdq_form_switch(form, maxpos)
            g_rdq_idle_us = 0.0lf
        }
        g_rdq_form_n = true
        g_rdq_attn_form = form
        if (unsplit && g_rdq_n_n_unsplit > 2u) {
            rdq_sample(g_rdq_names_n_unsplit, g_rdq_n_n_unsplit, g_rdq_overflow_n_unsplit)
        } elif (!unsplit && g_rdq_n_n > 2u) {
            rdq_sample(g_rdq_names_n, g_rdq_n_n, g_rdq_overflow_n)
        }
    }
}

//! Batch-decode support: upload one layer's KV history ([npos x kvd] codec rows) into the shared
//! mirror at layer `l`, positions [0, npos). The session codec matches the mirror codec (the arm
//! gate), so the rows byte-copy.
[hot_path]   // per layer per batch-decode row
def vk_rdec_sync_kv(l : int64; kp : uint8 const?; vp : uint8 const?; npos : int64) {
    assert(g_rd != null, "vk_rdec_sync_kv before prepare")
    let L & = unsafe(g_rd.layers[l])
    if (L.mir_idx < 0l) {
        return
    }
    let rowb = L.kvd * g_rd.kv_esz
    let base = rd_mir_base(L) * g_rd.kv_esz
    upload_region_at(g_rd.k_mirror, base, kp, npos * rowb)
    upload_region_at(g_rd.v_mirror, base, vp, npos * rowb)
}

//! Bulk hydration: read layer `l`'s mirror rows [0, npos) into the caller's cache (kp/vp,
//! [npos x kvd] codec rows each), one submit per KV_READBACK_ROWS rows. The host-KV-authoritative
//! rail — makes continuations, CPU fallback, and slot switches valid after a device-resident prefill.
[hot_path]   // the hydration rail — per layer, KV_READBACK_ROWS rows per submit
def vk_rdec_read_kv_bulk(l : int64; var kp : uint8?; var vp : uint8?; npos : int64) {
    assert(g_rd != null, "vk_rdec_read_kv_bulk before prepare")
    let L & = unsafe(g_rd.layers[l])
    if (L.mir_idx < 0l) {
        return
    }
    verify(vk_moe_init())
    let rowb = L.kvd * g_rd.kv_esz
    var p0 = 0l
    var raw = alloc_transient_cmd()   // re-recorded per chunk (the pool allows reset), freed below
    while (p0 < npos) {
        let rows = min(KV_READBACK_ROWS, npos - p0)
        let bytes = rows * rowb
        let off = rd_mir_base(L) * g_rd.kv_esz + p0 * rowb
        unsafe {
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            cmd_copy_range(raw, g_rd.k_mirror, off, g_rd.kv_readback.buf, 0l, bytes)
            cmd_copy_range(raw, g_rd.v_mirror, off, g_rd.kv_readback.buf, bytes, bytes)
            vk_check(vkEndCommandBuffer(raw), null)
            submit_wait(raw)
            memcpy(reinterpret<void?>(kp + p0 * rowb), g_rd.kv_readback.mapped, bytes)
            memcpy(reinterpret<void?>(vp + p0 * rowb), reinterpret<uint8?>(g_rd.kv_readback.mapped) + bytes, bytes)
        }
        p0 += rows
    }
    free_transient_cmd(raw)
}

//! Batch-decode writeback: read layer `l`'s new K/V row at `pos` from the mirror back into the
//! caller's cache row (kp/vp, one codec row each). Keeps the CPU cache current after a GPU decode step.
[hot_path]   // per layer per batch-decode row
def vk_rdec_read_kv(l, pos : int64; var kp : uint8?; var vp : uint8?) {
    assert(g_rd != null, "vk_rdec_read_kv before prepare")
    let L & = unsafe(g_rd.layers[l])
    if (L.mir_idx < 0l) {
        return
    }
    let rowb = L.kvd * g_rd.kv_esz
    let off = rd_mir_base(L) * g_rd.kv_esz + pos * rowb
    verify(vk_moe_init())
    unsafe {
        var raw = alloc_transient_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        cmd_copy_range(raw, g_rd.k_mirror, off, g_rd.kv_readback.buf, 0l, rowb)
        cmd_copy_range(raw, g_rd.v_mirror, off, g_rd.kv_readback.buf, rowb, rowb)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(reinterpret<void?>(kp), g_rd.kv_readback.mapped, rowb)
        memcpy(reinterpret<void?>(vp), reinterpret<uint8?>(g_rd.kv_readback.mapped) + rowb, rowb)
        free_transient_cmd(raw)
    }
}

// ===== deltanet decode step (npos == 1 through the same MoeGpuDnFn) =====
// One pre-recorded submit per layer per token: qkv+z GEMVs into a SHARED y buffer -> the fused
// step kernel -> out GEMV -> y DMA. State/ring stay device-resident; flush/invalidate = the seams.

[cold_path]
def private ensure_dnd_state {
    if (g_gpu.dnd_ready) {
        return
    }
    ensure_dn_state()   // pulls in ffn/batch state: batch_cmd + the dn staging bufs
    g_gpu.dnd_bg = make_host_buf(DND_BG_BYTES, false)
    g_gpu.dnd_oq_dev = make_device_buf(DND_OQ_BYTES)
    g_gpu.dnd_os_dev = make_device_buf(DND_OS_BYTES)
    g_gpu.dnd_of_dev = make_device_buf(DND_OQ_BYTES * 4l)
    g_gpu.dnd_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: deltanet decode step engaged\n")
}

// create one layer's step chain: per-layer state/smalls buffers, the three sets, the once-filled
// GEMV metas (dn stacks never serve FFN GEMVs, so their host metas are ours to pin), and the
// warm cmd — per token only acts/β/γ refresh and the cmd resubmits as-is.
[cold_path]
def private dnd_step_make(s_qkv, s_z, s_o : int; woq, woz, woo : int64;
                          dim, cd, kd, di, nvh, nkh, ds, taps : int64; eps : float) {
    ensure_gemv_scratch(s_qkv)
    ensure_gemv_scratch(s_z)
    ensure_gemv_scratch(s_o)
    var qkv_region = fixed_array(woq, 0l, 1l)
    var z_region = fixed_array(woz, 0l, 1l)
    var o_region = fixed_array(woo, 0l, 1l)
    unsafe {   // regions are ours to pin: dn stacks never serve FFN GEMVs
        fill_stack_sched_rows(s_qkv, addr(qkv_region[0]), 1l, 1l, 0l)
        fill_stack_sched_rows(s_z, addr(z_region[0]), 1l, 1l, 0l)
        fill_stack_sched_rows(s_o, addr(o_region[0]), 1l, 1l, 0l)
    }
    var st = DnStep(state_dev = make_device_buf(nvh * ds * ds * 4l),
                    smalls_dev = make_device_buf(DND_SMALLS_BYTES),
                    cmd = alloc_cmd(), s_qkv = s_qkv, s_z = s_z, s_o = s_o,
                    slot_state_bytes = nvh * ds * ds * 4l, ring_image_bytes = cd * taps * 4l, smalls_bytes = DND_SMALLS_BYTES, state_byte_off = 0l,
                    parity = 0, tok <- make_host_buf(TOK_META_BYTES, true), valid = false, dirty = false,
                    owner_state_addr = 0ul, owner_hist_addr = 0ul)
    let fqkv = g_gpu.stacks[s_qkv].fmt
    let fz = g_gpu.stacks[s_z].fmt
    let fo = g_gpu.stacks[s_o].fmt
    verify(gemv_cls_ensure(fz) && gemv_cls_ensure(fo) && ensure_dn_step_cls(),
        "vk decode: the gemv class rail must engage on a live device")
    // z GEMV rides the qkv stack's SHARED acts image and y buffer (y base cd in the push) — the fused kernel sees qkv rows AND the z row on one binding
    st.z_set = gemv_cls_set(fz, fixed_array(g_gpu.stacks[s_z].wqbuf, g_gpu.stacks[s_z].wsbuf, g_gpu.stacks[s_z].meta.buf,
            g_gpu.stacks[s_qkv].xq.buf, g_gpu.stacks[s_qkv].xs.buf, g_gpu.stacks[s_qkv].y_dev),
        fixed_array(g_gpu.stacks[s_z].wqbytes, ws_range(s_z), META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES),
        fixed_array(0u, 0u, 0u, 0u, 0u, VHG_Y2))
    // fused step (class): proj = the shared y (qkv rows + z at cd); state advances in place — residency IS that binding; parity rides TokMeta
    st.fused_set = set_dn_step_cls(
        fixed_array(g_gpu.stacks[s_qkv].y_dev, st.state_dev, st.smalls_dev, g_gpu.dnd_os_dev, g_gpu.dnd_oq_dev, st.tok.buf, g_gpu.dnd_of_dev),
        fixed_array(Y_BYTES, st.slot_state_bytes, DND_SMALLS_BYTES, DND_OS_BYTES, DND_OQ_BYTES, TOK_META_BYTES, DND_OQ_BYTES * 4l),
        fixed_array(VHG_Y1 | VHG_Y2, VHG_ST, VHG_SM, VHG_WS, VHG_WS, 0u, VHG_WS))
    // out GEMV: out planes@0/1, its once-filled regions@2, the o image@3/4, its y_dev@5
    st.out_set = gemv_cls_set(fo, fixed_array(g_gpu.stacks[s_o].wqbuf, g_gpu.stacks[s_o].wsbuf, g_gpu.stacks[s_o].meta.buf,
            g_gpu.dnd_oq_dev, g_gpu.dnd_os_dev, g_gpu.stacks[s_o].y_dev),
        fixed_array(g_gpu.stacks[s_o].wqbytes, ws_range(s_o), META_BYTES, DND_OQ_BYTES, DND_OS_BYTES, Y_BYTES),
        fixed_array(0u, 0u, 0u, VHG_WS, VHG_WS, VHG_YO))
    // the qkv stack's class set plays Y1 in this chain (role-fluid sets re-declare per record)
    var qset = stack_gemv_cls_set(s_qkv)
    hz_set_bits(qset, 0u, 0u, 0u, 0u, 0u, VHG_Y1)
    let raw = st.cmd
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    vhz_dep(raw, h, 0u, VHG_SM, true)   // one declaration for the smalls copy group
    cmd_copy_range(raw, g_gpu.dnd_bg.buf, 0l, st.smalls_dev, DN_SM_BETA * 4l, nvh * 4l)
    cmd_copy_range(raw, g_gpu.dnd_bg.buf, nvh * 4l, st.smalls_dev, DN_SM_G * 4l, nvh * 4l)
    var pcqkv = GemvArgs(n = uint(dim), d = uint(cd), nreg = 1u, ybase = 0u)
    gemv_enc(fqkv, raw, h, qset, pcqkv, cd)
    var pcz = GemvArgs(n = uint(dim), d = uint(di), nreg = 1u, ybase = uint(cd))
    gemv_enc(fz, raw, h, st.z_set, pcz, di)
    var pcd = DnStepArgs(cd = uint(cd), kd = uint(kd), nkh = uint(nkh), ds = uint(ds),
        dconv = uint(taps + 1l), beta_off = uint(DN_SM_BETA), g_off = uint(DN_SM_G),
        taps_off = uint(DN_SM_TAPS), wnorm_off = uint(DN_SM_WNORM), a_off = uint(DN_SM_A),
        dt_off = uint(DN_SM_DT), hist_off = uint(DN_SM_HIST),
        eps = eps, qscale = 1.0 / sqrt(float(ds)))
    enc_dn_step_cls(raw, h, st.fused_set, pcd, nvh)
    var pco = GemvArgs(n = uint(di), d = uint(dim), nreg = 1u, ybase = 0u)
    gemv_enc(fo, raw, h, st.out_set, pco, dim)
    vhz_dep(raw, h, VHG_YO, 0u, true)
    cmd_copy_whole(raw, g_gpu.stacks[s_o].y_dev, g_gpu.stacks[s_o].y.buf, dim * 4l)
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_dnd hz: {h.disp} nodes, {h.barriers} barriers\n")
    }
    vk_check(vkEndCommandBuffer(raw), null)
    g_gpu.dnd_steps |> insert(woq, st)
}

// cold upload: the engine's CPU state is authoritative — stage state + the whole smalls image
// (colds + ring; the β/γ region carries garbage until the first step overwrites it) and copy up
def private dnd_cold_upload(st : DnStep; stp : float const?; histp : float const?;
                            convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                            cd, taps, dconv, nvh, ds : int64; eps : float) {
    unsafe {
        var sp = reinterpret<float?>(g_gpu.dn_smalls.mapped)
        sp[0] = eps
        sp[1] = 1.0 / sqrt(float(ds))
        memcpy(reinterpret<void?>(sp + DN_SM_TAPS), reinterpret<void?>(convw), cd * dconv * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_WNORM), reinterpret<void?>(wnormp), ds * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_A), reinterpret<void?>(ap), nvh * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_DT), reinterpret<void?>(dtp), nvh * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_HIST), reinterpret<void?>(histp), cd * taps * 4l)
        memcpy(g_gpu.dn_state_stage.mapped, reinterpret<void?>(stp), st.slot_state_bytes)
    }
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    cmd_copy_whole(raw, g_gpu.dn_smalls.buf, st.smalls_dev, DN_SMALLS_BYTES)
    cmd_copy_whole(raw, g_gpu.dn_state_stage.buf, st.state_dev, st.slot_state_bytes)
    vk_check(vkEndCommandBuffer(raw), null)
    submit_wait(g_gpu.batch_cmd)
}

[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#dn-step-owner")]
def vk_moe_dn_step(var yp : float?; s_qkv, s_z, s_o : int; woq, woz, woo : int64;
                   xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
                   convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                   stp : float const?; histp : float const?;
                   dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    let taps = dconv - 1l
    assert(ds <= DN_MAX_DS && ds % 32l == 0l && (ds & (ds - 1l)) == 0l && nvh <= DN_MAX_HEADS && cd <= DN_MAX_CD
           && 2l * kd <= DN_MAX_QK && dconv <= DN_MAX_DCONV && di == nvh * ds && kd == nkh * ds && cd == 2l * kd + di
           && dim % 32l == 0l && di % 32l == 0l && di <= DND_OQ_BYTES,
        "dasLLAMA vulkan tier: deltanet geometry outside step capacity")
    ensure_dnd_state()
    if (!key_exists(g_gpu.dnd_steps, woq)) {
        dnd_step_make(s_qkv, s_z, s_o, woq, woz, woo, dim, cd, kd, di, nvh, nkh, ds, taps, eps)
    }
    var st = unsafe(g_gpu.dnd_steps?[woq])   // the make above made the key: a read, never an insert
    let caller_state_addr = intptr(stp)
    if (st.valid && st.owner_state_addr != caller_state_addr) {
        if (st.dirty) {
            dnd_flush_down(*st)
        }
        st.valid = false
        st.dirty = false
    }
    if (!st.valid) {
        dnd_cold_upload(*st, stp, histp, convw, wnormp, ap, dtp, cd, taps, dconv, nvh, ds, eps)
        st.parity = 0
        st.owner_state_addr = caller_state_addr
        st.owner_hist_addr = intptr(histp)
    }
    let ts0 = ref_time_ticks()
    fill_stack_acts(s_qkv, xqp, xsp, dim)   // the z GEMV set shares this image
    unsafe {
        var bg = reinterpret<float?>(g_gpu.dnd_bg.mapped)
        memcpy(reinterpret<void?>(bg), reinterpret<void?>(bp), nvh * 4l)
        memcpy(reinterpret<void?>(bg + nvh), reinterpret<void?>(gp), nvh * 4l)
        var tm = reinterpret<TokMeta?>(st.tok.mapped)
        tm.parity = uint(st.parity)
        tm.mirbase = 0u
        tm.dnslot = 0u
    }
    let ts1 = ref_time_ticks()
    submit_wait(st.cmd)
    let ts2 = ref_time_ticks()
    unsafe {
        memcpy(reinterpret<void?>(yp), g_gpu.stacks[s_o].y.mapped, dim * 4l)
    }
    st.parity = 1 - st.parity
    st.valid = true
    st.dirty = true
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_dnd prep {get_time_usec(ts0) - get_time_usec(ts1)} submit {get_time_usec(ts1) - get_time_usec(ts2)} copy {get_time_usec(ts2)}\n")   // nolint:PERF026,PERF028 — vk_prof()-gated report leg
    }
}

[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#dn-step-owner"), unused_argument(histp)]
def vk_dn_step_flush(l : int64; var stp : float?; var histp : float?) {
    if (g_gpu == null || !g_gpu.dnd_ready) {
        return
    }
    if (g_rd != null && g_rd.hybrid && l < long_length(g_rd.layers) && g_rd.layers[l].recurrent) {
        var L & = unsafe(g_rd.layers[l])
        for (st in L.dn) {
            if (st.valid && st.owner_state_addr == intptr(stp)) {
                dnd_send_home(st)
            }
        }
        return
    }
    let woq = moe_gpu_dn_layer_key(l)
    if (woq < 0l || !key_exists(g_gpu.dnd_steps, woq)) {
        return
    }
    var st = unsafe(g_gpu.dnd_steps?[woq])   // the key_exists above made sure of the key: a read, never an insert
    let caller_state_addr = intptr(stp)
    if (st.owner_state_addr != caller_state_addr) {
        return
    }
    dnd_send_home(*st)
}

//! the owner's state goes back to the host where the device holds the newer copy, and the slot is free
def private dnd_send_home(var st : DnStep) {
    if (st.dirty) {
        dnd_flush_down(st)
    }
    dnd_drop_slot(st)
}

def private dnd_drop_slot(var st : DnStep) {
    st.valid = false
    st.dirty = false
    st.owner_state_addr = 0ul
    st.owner_hist_addr = 0ul
}

def private dnd_flush_down(st : DnStep) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    cmd_copy_range(raw, st.state_dev, st.state_byte_off, g_gpu.dn_state_host.buf, 0l, st.slot_state_bytes)
    cmd_copy_range(raw, st.smalls_dev, st.hist_off * 4l + int64(st.parity) * st.ring_image_bytes,
        g_gpu.dn_tail_host.buf, 0l, st.ring_image_bytes)
    vk_check(vkEndCommandBuffer(raw), null)
    submit_wait(g_gpu.batch_cmd)
    unsafe {
        memcpy(reinterpret<void?>(st.owner_state_addr), g_gpu.dn_state_host.mapped, st.slot_state_bytes)
        memcpy(reinterpret<void?>(st.owner_hist_addr), g_gpu.dn_tail_host.mapped, st.ring_image_bytes)
    }
}

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#dn-step-owner")]   // once per prompt, before its first window records
def vk_rdec_dn_flush_owners {
    for (L in g_rd.layers) {
        continue if (!L.recurrent || !L.dn[g_rd.region].valid)
        dnd_send_home(L.dn[g_rd.region])
    }
}

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#dn-step-owner")]   // a model drop, never the served token
def vk_dn_step_invalidate {
    if (g_gpu == null) {
        return
    }
    for (st in values(g_gpu.dnd_steps)) {
        dnd_drop_slot(st)
    }
    if (g_rd != null && g_rd.hybrid) {
        for (L in g_rd.layers) {
            continue if (!L.recurrent)
            for (st in L.dn) {
                dnd_drop_slot(st)
            }
        }
    }
}

[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#dn-step-owner")]   // per session reset, rollback and delete
def vk_dn_step_release(state_lo : uint64; state_hi : uint64) {
    if (g_gpu == null) {
        return
    }
    for (st in values(g_gpu.dnd_steps)) {
        if (st.owner_state_addr >= state_lo && st.owner_state_addr < state_hi) {
            dnd_drop_slot(st)
        }
    }
    if (g_rd != null && g_rd.hybrid) {
        for (L in g_rd.layers) {
            continue if (!L.recurrent)
            for (st in L.dn) {
                if (st.owner_state_addr >= state_lo && st.owner_state_addr < state_hi) {
                    dnd_drop_slot(st)
                }
            }
        }
    }
}

// ===== decode dispatch (the FFN's GEMV arm) =====

// lazy per-stack class-GEMV set (dies with the stack); role bits re-declare per chain via hz_set_bits
def private stack_gemv_cls_set(si : int) : VkDescriptorSet {
    var st & = unsafe(g_gpu.stacks[si])
    if (!st.gemv_cls_made) {
        verify(gemv_cls_ensure(st.fmt), "vk decode: the gemv class rail must engage on a live device")
        st.gemv_cls_set = gemv_cls_set(st.fmt,
            fixed_array(st.wqbuf, st.wsbuf, st.meta.buf, st.xq.buf, st.xs.buf, st.y_dev),
            fixed_array(st.wqbytes, ws_range(si), META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES),
            fixed_array(0u, 0u, 0u, 0u, 0u, 0u))
        st.gemv_cls_made = true
    }
    return st.gemv_cls_set
}

// pre-record (or refresh on a row-count / activation-form change) the triple's chain: gate+up
// GEMV pair, fused act+requant, down GEMV, y DMA. Per token only the stacks' host-visible
// regions and the activation image change — the recorded buffer resubmits as-is.
[cold_path]   // creates the triple's sets once
def private ffn_cmd_key(s1, s3, s2 : int) : int64 {
    let key = (int64(s1) * int64(MAX_STACKS) + int64(s3)) * int64(MAX_STACKS) + int64(s2)
    if (!key_exists(g_gpu.ffn_cmds, key)) {
        var fc = FfnCmd(cmd = alloc_cmd(), nrows = 0l)
        let down_is_kq = !kq_block32(g_gpu.stacks[s2].fmt)
        verify((down_is_kq ? ensure_q8k_actrq_cls() : ensure_q8_actrq_cls()) && gemv_cls_ensure(g_gpu.stacks[s2].fmt),
            "vk decode: the class rails must engage on a live device")
        // fused act+requant (class): gate@0, up@1, hq@2, hs@3 — one family set serves both forms
        fc.actrq_set = set_actrq_cls(
            fixed_array(g_gpu.stacks[s1].y_dev, g_gpu.stacks[s3].y_dev, g_gpu.hq_dev, g_gpu.hs_dev),
            fixed_array(Y_BYTES, Y_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES),
            fixed_array(VHG_Y1, VHG_Y2, VHG_HQ, VHG_HQ))
        // down: its own planes + regions, hq/hs as the activation image, its y_dev
        fc.down_set = gemv_cls_set(g_gpu.stacks[s2].fmt,
            fixed_array(g_gpu.stacks[s2].wqbuf, g_gpu.stacks[s2].wsbuf, g_gpu.stacks[s2].meta.buf, g_gpu.hq_dev, g_gpu.hs_dev, g_gpu.stacks[s2].y_dev),
            fixed_array(g_gpu.stacks[s2].wqbytes, ws_range(s2), META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, Y_BYTES),
            fixed_array(0u, 0u, 0u, VHG_HQ, VHG_HQ, VHG_YO))
        g_gpu.ffn_cmds |> insert(key, fc)
    }
    return key
}

//! Records the decode FFN chain into an OPEN command buffer: gate/up GEMVs, the fused
//! act+requant, the down GEMV (its rows land in the down stack's y_dev under VHG_YO).
def private ffn_chain_enc(raw : VkCommandBuffer; var h : VkHaz; var fc : FfnCmd; s1, s3, s2 : int; nrows, n, nfe : int64; is_gelu : bool) {
    var gset = stack_gemv_cls_set(s1)
    var uset = stack_gemv_cls_set(s3)
    hz_set_bits(gset, 0u, 0u, 0u, 0u, 0u, VHG_Y1)
    hz_set_bits(uset, 0u, 0u, 0u, 0u, 0u, VHG_Y2)
    var pcg = GemvArgs(n = uint(n), d = uint(nfe), nreg = uint(nrows), ybase = 0u)
    gemv_enc(g_gpu.stacks[s1].fmt, raw, h, gset, pcg, nrows * nfe)
    gemv_enc(g_gpu.stacks[s3].fmt, raw, h, uset, pcg, nrows * nfe)
    let down_is_kq = !kq_block32(g_gpu.stacks[s2].fmt)
    var pca = ActArgs(nelem = uint(nrows * nfe), act = is_gelu ? ACT_GELU : ACT_SILU,
        nblk = uint(nrows * nfe / (down_is_kq ? 256l : 32l)))
    let awgs = (nrows * nfe / (down_is_kq ? 8l : 4l) + 255l) / 256l
    if (down_is_kq) {
        enc_q8k_actrq_cls(raw, h, fc.actrq_set, pca, awgs)
    } else {
        enc_q8_actrq_cls(raw, h, fc.actrq_set, pca, awgs)
    }
    var pcd = GemvArgs(n = uint(nfe), d = uint(n), nreg = uint(nrows), ybase = 0u)
    gemv_enc(g_gpu.stacks[s2].fmt, raw, h, fc.down_set, pcd, nrows * n)
}

def private ffn_cmd_for(s1, s3, s2 : int; nrows, n, nfe : int64; is_gelu : bool) : VkCommandBuffer {
    let key = ffn_cmd_key(s1, s3, s2)
    var fc = g_gpu.ffn_cmds[key]
    if (fc.nrows != nrows || fc.gelu != is_gelu) {
        if (fc.nrows != 0l) {
            let rf : VkCommandBufferResetFlags
            vk_check(vkResetCommandBuffer(fc.cmd, rf), null)
        }
        let raw = fc.cmd
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        ffn_chain_enc(raw, h, fc, s1, s3, s2, nrows, n, nfe, is_gelu)
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.stacks[s2].y_dev, g_gpu.stacks[s2].y.buf, nrows * n * 4l)
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_ffng hz: {h.disp} nodes, {h.barriers} barriers\n")
        }
        vk_check(vkEndCommandBuffer(raw), null)
        g_gpu.ffn_cmds[key] = FfnCmd(cmd = fc.cmd, actrq_set = fc.actrq_set,
            down_set = fc.down_set, nrows = nrows, gelu = is_gelu)
    }
    return fc.cmd
}

def private ffn_fill(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n, nfe, nrows : int64; f1, f3, f2 : int) : tuple<s1 : int; s3 : int; s2 : int> {
    ensure_ffn_state()   // hq/hs planes for the act+requant: a shexp-only run dispatches FFN first
    let s1 = find_stack(unsafe(offs1[0]), f1, "decode ffn gate")
    let s3 = find_stack(unsafe(offs3[0]), f3, "decode ffn up")
    let s2 = find_stack(unsafe(offs2[0]), f2, "decode ffn down")
    assert(nrows * max(nfe, n) * 4l <= Y_BYTES, "dasLLAMA vulkan tier: FFN dispatch exceeds output capacity")
    // gate/up share one activation image (same xqp below) so forms must agree; down's is its own
    let kq_in = !kq_block32(f1)
    verify(!kq_block32(f3) == kq_in,
        "dasLLAMA vulkan tier: gate/up form mismatch - they share one activation image")
    let xunit_in = fmt_unit(f1)
    ensure_gemv_scratch(s1)
    ensure_gemv_scratch(s3)
    ensure_gemv_scratch(s2)
    fill_stack_sched_rows(s1, offs1, nregions, nrows, n / xunit_in)
    fill_stack_sched_rows(s3, offs3, nregions, nrows, n / xunit_in)
    fill_stack_sched_rows(s2, offs2, nregions, nrows, nfe / fmt_unit(f2))
    fill_stack_acts(s1, xqp, xsp, nrows * n)
    fill_stack_acts(s3, xqp, xsp, nrows * n)
    return (s1 = s1, s3 = s3, s2 = s2)
}

//! Fills metas/acts and returns the recorded chain with the down stack index - the async join
//! reads that stack's y staging.
def private ffn_gemv_prep(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n, nfe, nrows : int64; f1, f3, f2 : int; is_gelu : bool) : tuple<cmd : VkCommandBuffer; s2 : int> {
    let st = ffn_fill(offs1, offs3, offs2, nregions, xqp, xsp, n, nfe, nrows, f1, f3, f2)
    return (cmd = ffn_cmd_for(st.s1, st.s3, st.s2, nrows, n, nfe, is_gelu), s2 = st.s2)
}

//! DASLLAMA_GPU_PROF=1 decode ledger: per-token sums over each per-layer submit (prep + submit + wait).
var private g_dec_prof_tokens = 0l
var private g_dec_prof_ffn_us = 0l
var private g_dec_prof_ffn_n = 0l
var private g_dec_prof_attn_us = 0l
var private g_dec_prof_attn_n = 0l
var private g_dec_prof_attn_rec_us = 0l
var private g_dec_prof_attn_wait_us = 0l
var private g_dec_prof_first_woq = -1l

def vk_moe_ffn_gemv(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n, nfe, nrows : int64; f1, f3, f2 : int; is_gelu : bool) {
    // decode-form only — vk_moe_ffn routes the combined (npos > 0) contract to the batch arm instead
    let ts0 = ref_time_ticks()
    vk_moe_ffn_begin(offs1, offs3, offs2, nregions, xqp, xsp, n, nfe, nrows, f1, f3, f2, is_gelu)
    vk_moe_ffn_join(goutp)
    if (vk_prof()) {
        g_dec_prof_ffn_us += int64(get_time_usec(ts0))
        g_dec_prof_ffn_n++
    }
}

//! MoeGpuFfnBeginFn: submit the decode FFN chain and return — the CPU runs the miss experts
//! (and anything else) while the GPU crunches; vk_moe_ffn_join collects. ONE in flight.
def vk_moe_ffn_begin(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n, nfe, nrows : int64; f1, f3, f2 : int; is_gelu : bool) {
    assert(g_gpu.ffn_pend_s2 < 0, "dasLLAMA vulkan tier: async FFN begin while one is already in flight")
    var pc = ffn_gemv_prep(offs1, offs3, offs2, nregions, xqp, xsp, n, nfe, nrows, f1, f3, f2, is_gelu)
    submit_nowait(pc.cmd)
    g_gpu.ffn_pend_s2 = pc.s2
    g_gpu.ffn_pend_bytes = nrows * n * 4l
}

//! MoeGpuFfnJoinFn: spin the pending async FFN's fence and copy its output rows back.
def vk_moe_ffn_join(var goutp : float?) {
    assert(g_gpu.ffn_pend_s2 >= 0, "dasLLAMA vulkan tier: async FFN join with nothing in flight")
    wait_fence()
    unsafe {
        memcpy(reinterpret<void?>(goutp), g_gpu.stacks[g_gpu.ffn_pend_s2].y.mapped, g_gpu.ffn_pend_bytes)
    }
    g_gpu.ffn_pend_s2 = -1
}

// ===== the decode FFN tail: FFN(l) + combine + add-rms + requant into layer l+1's attention acts =====

[cold_path]   // lazy init, once per model
def private ensure_tail_state(n : int64) {
    if (g_gpu.tail_ready) {
        return
    }
    verify(ensure_dec_combine_cls() && ensure_cls_ar() && ensure_cls_q8k_rq() && ensure_cls_dn_rq(),
        "vk decode ffn tail: the class rails must engage on a live device")
    g_gpu.tail_xin = make_host_buf(n * 4l, true)
    g_gpu.tail_w = make_host_buf(MAX_ROUTED_SLOTS * 4l, true)
    g_gpu.tail_xres_dev = make_device_buf(n * 4l)
    g_gpu.tail_xb_dev = make_device_buf(n * 4l)
    g_gpu.tail_xres = make_host_buf(n * 4l, false, [cached = true])
    g_gpu.tail_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: decode ffn tail engaged (DASLLAMA_GPU_DEC_TAIL=0 collects every FFN on the host)\n")
}

let private MAX_ROUTED_SLOTS = 64l   // routed slots the tail's and the span's weight staging hold (n_expert_used)

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block")]   // recording leg: once per layer pair
def private tail_cmd_for(s1, s3, s2, sq : int; woq_next : int64; fq_next : int; anp : float const?; n, nfe, nrows : int64; is_gelu : bool; eps : float) : VkCommandBuffer {
    if (!key_exists(g_gpu.tail_cmds, woq_next)) {
        let key = ffn_cmd_key(s1, s3, s2)
        var fc = g_gpu.ffn_cmds[key]
        var tc = TailCmd(cmd = alloc_cmd(), nrows = nrows)
        tc.an_dev = make_device_buf(n * 4l)
        upload_region_at(tc.an_dev, 0l, unsafe(reinterpret<uint8 const?>(anp)), n * 4l)
        tc.s_comb = set_dec_combine_cls(
            fixed_array(g_gpu.tail_xin.buf, g_gpu.tail_w.buf, g_gpu.stacks[s2].y_dev, g_gpu.tail_xres_dev),
            fixed_array(n * 4l, MAX_ROUTED_SLOTS * 4l, Y_BYTES, n * 4l),
            fixed_array(0u, 0u, VHG_YO, VHG_XRES))
        tc.s_ar = set_cls_ar(fixed_array(g_gpu.tail_xres_dev, g_gpu.tail_xres_dev, tc.an_dev, g_gpu.tail_xb_dev),
            fixed_array(n * 4l, n * 4l, n * 4l, n * 4l), fixed_array(VHG_XRES, VHG_XRES, 0u, VHG_XB))
        tc.s_rq = set_rq_cls(fixed_array(g_gpu.tail_xb_dev, g_gpu.stacks[sq].xq.buf, g_gpu.stacks[sq].xs.buf),
            fixed_array(n * 4l, XQ_BYTES, XS_BYTES), fixed_array(VHG_XB, VHG_ACTS, VHG_ACTS))
        let raw = tc.cmd
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        ffn_chain_enc(raw, h, fc, s1, s3, s2, nrows, n, nfe, is_gelu)
        var pcc = DecCombineArgs(n = uint(n), k = uint(nrows))
        enc_dec_combine_cls(raw, h, tc.s_comb, pcc, (n + 255l) / 256l)
        var pca = ArArgs(dim = uint(n), add_on = 0u, woff = 0u, eps = eps, ascale = 1.0)
        enc_cls_ar(raw, h, tc.s_ar, pca, 1l)
        rq_enc(raw, h, tc.s_rq, fq_next, n)
        vhz_dep(raw, h, VHG_XRES | VHG_ACTS, 0u, true)
        cmd_copy_whole(raw, g_gpu.tail_xres_dev, g_gpu.tail_xres.buf, n * 4l)
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_ffn_tail hz: {h.disp} nodes, {h.barriers} barriers\n")
        }
        vk_check(vkEndCommandBuffer(raw), null)
        g_gpu.tail_cmds |> insert(woq_next, tc)
    }
    var tc = g_gpu.tail_cmds[woq_next]
    assert(tc.nrows == nrows, "dasLLAMA vulkan tier: decode ffn tail recorded with another slot count")
    return tc.cmd
}

//! MoeGpuFfnTailFn: the resident FFN chain plus its tail - combine + residual, the next layer's
//! attention rms + requant into that layer's acts image - submitted WITHOUT a fence; the next
//! attention submit's fence covers both and its collect lands the residual row in xoutp.
[hot_path]   // per token
def vk_moe_ffn_tail(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n, nfe, nrows : int64; f1, f3, f2 : int; is_gelu : bool; wp : float const?; xp : float const?; var xoutp : float?; woq_next : int64; fq_next : int; anp : float const?; eps : float) {
    assert(g_gpu.tail_pending_key < 0l, "dasLLAMA vulkan tier: decode ffn tail while one is pending")
    assert(nrows <= MAX_ROUTED_SLOTS, "dasLLAMA vulkan tier: decode ffn tail slot count exceeds its weight staging")
    let ts0 = ref_time_ticks()
    let st = ffn_fill(offs1, offs3, offs2, nregions, xqp, xsp, n, nfe, nrows, f1, f3, f2)
    ensure_tail_state(n)
    let sq = find_stack(woq_next, fq_next, "decode ffn tail q")
    ensure_gemv_scratch(sq)
    var cmd = tail_cmd_for(st.s1, st.s3, st.s2, sq, woq_next, fq_next, anp, n, nfe, nrows, is_gelu, eps)
    unsafe {
        memcpy(g_gpu.tail_xin.mapped, reinterpret<void?>(xp), n * 4l)
        memcpy(g_gpu.tail_w.mapped, reinterpret<void?>(wp), nrows * 4l)
    }
    submit_nofence(cmd)
    g_gpu.tail_pending_key = dat_key(woq_next, fq_next)
    g_gpu.tail_xout = xoutp
    if (vk_prof()) {
        g_dec_prof_ffn_us += int64(get_time_usec(ts0))
        g_dec_prof_ffn_n++
    }
}

//! MoeGpuFfnTailPendingFn: a tail is in flight, uncollected
def vk_moe_ffn_tail_pending : bool => g_gpu.tail_pending_key >= 0l

// ===== the whole-token decode span: the resident layers as ONE recorded chain, ONE submit =====


def private span_layer_sets(a : MoeSpanDec; li : int64) : SpanLayer {
    let nl = long_length(a.woq)
    let dim = a.dim
    let nb = dim * 4l
    var SL = SpanLayer(woq = a.woq[li], sq = find_stack(a.woq[li], a.fq[li], "span q"),
        s1 = find_stack(a.we1[li], a.f1[li], "span gate"), s3 = find_stack(a.we3[li], a.f3[li], "span up"),
        s2 = find_stack(a.we2[li], a.f2[li], "span down"))
    ensure_gemv_scratch(SL.sq)
    ensure_gemv_scratch(SL.s1)
    ensure_gemv_scratch(SL.s3)
    ensure_gemv_scratch(SL.s2)
    SL.s_ar_ffn = set_cls_ar(fixed_array(g_gpu.span_x_dev, g_gpu.dat_xb2_dev, g_gpu.span_norms_dev, g_gpu.span_xb_dev),
        fixed_array(nb, nb, nl * 2l * nb, nb), fixed_array(VHS_X, VHZ_XB2, 0u, VHS_XB))
    SL.s_rq_g1 = set_rq_cls(fixed_array(g_gpu.span_xb_dev, g_gpu.stacks[SL.s1].xq.buf, g_gpu.stacks[SL.s1].xs.buf),
        fixed_array(nb, XQ_BYTES, XS_BYTES), fixed_array(VHS_XB, VHS_GACTS, VHS_GACTS))
    SL.s_rq_g3 = set_rq_cls(fixed_array(g_gpu.span_xb_dev, g_gpu.stacks[SL.s3].xq.buf, g_gpu.stacks[SL.s3].xs.buf),
        fixed_array(nb, XQ_BYTES, XS_BYTES), fixed_array(VHS_XB, VHS_GACTS, VHS_GACTS))
    SL.s_router = set_router_gemv_cls(fixed_array(g_gpu.span_router_dev, g_gpu.span_xb_dev, g_gpu.span_logits_dev, g_gpu.span_xb_dev),
        fixed_array(nl * a.ne * nb, nb, MOE_MAX_EXPERTS * 4l, nb), fixed_array(0u, VHS_XB, VHS_LOG, 0u))   //! the norms slot: the span's router reads the normed row, never the norm
    SL.s_topk = set_topk_cls(   //! the span folds no down scale: the logits row fills the ds binding, never read at NO_DSCALE
        fixed_array(g_gpu.span_logits_dev, g_gpu.stacks[SL.s1].meta.buf, g_gpu.stacks[SL.s3].meta.buf, g_gpu.stacks[SL.s2].meta.buf, g_gpu.span_w_dev, g_gpu.span_logits_dev, g_gpu.span_logits_dev, g_gpu.span_eidx_dev),
        fixed_array(MOE_MAX_EXPERTS * 4l, META_BYTES, META_BYTES, META_BYTES, MAX_ROUTED_SLOTS * 4l, MOE_MAX_EXPERTS * 4l, MOE_MAX_EXPERTS * 4l, MAX_ROUTED_SLOTS * 4l),
        fixed_array(VHS_LOG, VHS_META, VHS_META, VHS_META, VHS_META, VHS_LOG, VHS_LOG, VHS_META))
    SL.s_comb = set_dec_combine_cls(fixed_array(g_gpu.span_x_dev, g_gpu.span_w_dev, g_gpu.stacks[SL.s2].y_dev, g_gpu.span_x_dev),
        fixed_array(nb, MAX_ROUTED_SLOTS * 4l, Y_BYTES, nb), fixed_array(VHS_X, VHS_META, VHG_YO, VHS_X))
    if (a.nsh > 0l) {
        let q8 = int(KqFmt.q8)
        SL.sh1 = find_stack(a.wsh1[li], q8, "span shared gate")
        SL.sh3 = find_stack(a.wsh3[li], q8, "span shared up")
        SL.sh2 = find_stack(a.wsh2[li], q8, "span shared down")
        ensure_gemv_scratch(SL.sh1)
        ensure_gemv_scratch(SL.sh3)
        ensure_gemv_scratch(SL.sh2)
        var reg1 = fixed_array(a.wsh1[li], 0l, 1l)
        var reg3 = fixed_array(a.wsh3[li], 0l, 1l)
        var reg2 = fixed_array(a.wsh2[li], 0l, 1l)
        unsafe {
            fill_stack_sched_rows(SL.sh1, addr(reg1[0]), 1l, 1l, dim / 32l)
            fill_stack_sched_rows(SL.sh3, addr(reg3[0]), 1l, 1l, dim / 32l)
            fill_stack_sched_rows(SL.sh2, addr(reg2[0]), 1l, 1l, a.nsh / 32l)
        }
        SL.s_rq_sh1 = set_rq_cls(fixed_array(g_gpu.span_xb_dev, g_gpu.stacks[SL.sh1].xq.buf, g_gpu.stacks[SL.sh1].xs.buf),
            fixed_array(nb, XQ_BYTES, XS_BYTES), fixed_array(VHS_XB, VHS_GACTS, VHS_GACTS))
        SL.s_rq_sh3 = set_rq_cls(fixed_array(g_gpu.span_xb_dev, g_gpu.stacks[SL.sh3].xq.buf, g_gpu.stacks[SL.sh3].xs.buf),
            fixed_array(nb, XQ_BYTES, XS_BYTES), fixed_array(VHS_XB, VHS_GACTS, VHS_GACTS))
        if (a.sh_gated) {
            SL.s_shgate = set_router_gemv_cls(fixed_array(g_gpu.span_shgate_dev, g_gpu.span_xb_dev, g_gpu.span_logits_dev, g_gpu.span_xb_dev),
                fixed_array(nl * nb, nb, SPAN_LOGITS_BYTES, nb), fixed_array(0u, VHS_XB, VHS_LOG, 0u))
        }
        SL.s_comb_sh = set_dec_combine_sh_cls(
            fixed_array(g_gpu.span_x_dev, g_gpu.span_w_dev, g_gpu.stacks[SL.s2].y_dev, g_gpu.stacks[SL.sh2].y_dev, g_gpu.span_logits_dev, g_gpu.span_x_dev),
            fixed_array(nb, MAX_ROUTED_SLOTS * 4l, Y_BYTES, Y_BYTES, SPAN_LOGITS_BYTES, nb),
            fixed_array(VHS_X, VHS_META, VHG_YO, VHG_YO, VHS_LOG, VHS_X))
    }
    if (li + 1l < nl) {
        let sqn = find_stack(a.woq[li + 1l], a.fq[li + 1l], "span next q")
        ensure_gemv_scratch(sqn)
        SL.s_ar_att = set_cls_ar(fixed_array(g_gpu.span_x_dev, g_gpu.span_x_dev, g_gpu.span_norms_dev, g_gpu.span_xb_dev),
            fixed_array(nb, nb, nl * 2l * nb, nb), fixed_array(VHS_X, VHS_X, 0u, VHS_XB))
        SL.s_rq_att = set_rq_cls(fixed_array(g_gpu.span_xb_dev, g_gpu.stacks[sqn].xq.buf, g_gpu.stacks[sqn].xs.buf),
            fixed_array(nb, XQ_BYTES, XS_BYTES), fixed_array(VHS_XB, VHS_ACTS, VHS_ACTS))
    }
    return SL
}

let private SPAN_LOGITS_BYTES = (MOE_MAX_EXPERTS + 1l) * 4l   //! the router logits, then the shared expert's gate logit

def private rq_enc(raw : VkCommandBuffer; var h : VkHaz; var rq_set : VkDescriptorSet; fmt : int; dim : int64) {
    if (kq_sb(fmt)) {
        var pcr = RqArgs(inbase = 0u, nblk = uint(dim / 256l))
        enc_cls_q8k_rq(raw, h, rq_set, pcr, (dim / 8l + 255l) / 256l)
    } else {
        var pcr = RqArgs(inbase = 0u, nblk = uint(dim / 32l))
        enc_cls_dn_rq(raw, h, rq_set, pcr, (dim / 4l + 255l) / 256l)
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]
def private span_meta_base(woff : int64; si : int; fmt : int) : uint {
    let unit = fmt_unit(fmt)
    let rel = woff - g_gpu.stacks[si].base
    assert(rel % unit == 0l, "dasLLAMA vulkan tier: unaligned expert plane offset")
    return uint(rel / unit)
}

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]   // recording leg: once per span
def private span_record(a : MoeSpanDec) {
    let nl = long_length(a.woq)
    let dim = a.dim
    let qd = a.qd
    let kvd = a.kvd
    g_gpu.span_cmd = alloc_cmd()
    var raw = g_gpu.span_cmd
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    vhz_dep(raw, h, 0u, VHS_X, true)
    cmd_copy_whole(raw, g_gpu.span_xin.buf, g_gpu.span_x_dev, dim * 4l)
    for (li in range64(nl)) {
        var L & = unsafe(g_gpu.dat_layers[dat_key(a.woq[li], a.fq[li])])
        var SL = g_gpu.span_layers[li]
        at_chain_enc(raw, h, L, a.fq[li], a.fk[li], a.fv[li], a.fo[li], dim, qd, kvd, a.hs, a.n_heads, a.kv_mul, a.scale, a.eps, a.neox, a.qk_norm)
        vhz_dep(raw, h, VHZ_XB2 | VHZ_KVK | VHZ_KVV, 0u, true)
        cmd_copy_range(raw, g_gpu.dat_kv_dev, qd * 4l, g_gpu.span_out.buf, li * 2l * kvd * 4l, 2l * kvd * 4l)
        var pcar = ArArgs(dim = uint(dim), add_on = 1u, woff = uint((li * 2l + 1l) * dim), eps = a.eps, ascale = 1.0)
        enc_cls_ar(raw, h, SL.s_ar_ffn, pcar, 1l)
        rq_enc(raw, h, SL.s_rq_g1, a.f1[li], dim)
        rq_enc(raw, h, SL.s_rq_g3, a.f3[li], dim)
        vhz_dep(raw, h, VHS_GACTS, 0u)
        var pcro = RouterArgs(ne = uint(a.ne), dim = uint(dim), woff = uint(li * a.ne * dim), obase = 0u)
        enc_router_gemv_cls(raw, h, SL.s_router, pcro, a.ne)
        var pct = TopkArgs(ne = uint(a.ne), k = uint(a.k), norm = a.norm_topk ? 1u : 0u, wscale = a.wscale, dsoff = NO_DSCALE,
            base1 = span_meta_base(a.we1[li], SL.s1, a.f1[li]), stride1 = uint(a.nfe * dim / fmt_unit(a.f1[li])),
            base3 = span_meta_base(a.we3[li], SL.s3, a.f3[li]), stride3 = uint(a.nfe * dim / fmt_unit(a.f3[li])),
            base2 = span_meta_base(a.we2[li], SL.s2, a.f2[li]), stride2 = uint(a.nfe * dim / fmt_unit(a.f2[li])),
            xnb2 = uint(a.nfe / fmt_unit(a.f2[li])))
        enc_topk_cls(raw, h, SL.s_topk, pct, 1l)
        vhz_dep(raw, h, VHS_META, 0u)
        var fc = g_gpu.ffn_cmds[ffn_cmd_key(SL.s1, SL.s3, SL.s2)]
        ffn_chain_enc(raw, h, fc, SL.s1, SL.s3, SL.s2, a.k, dim, a.nfe, a.is_gelu)
        if (a.nsh > 0l) {
            rq_enc(raw, h, SL.s_rq_sh1, int(KqFmt.q8), dim)
            rq_enc(raw, h, SL.s_rq_sh3, int(KqFmt.q8), dim)
            vhz_dep(raw, h, VHS_GACTS, 0u)
            if (a.sh_gated) {
                var pcsg = RouterArgs(ne = 1u, dim = uint(dim), woff = uint(li * dim), obase = uint(MOE_MAX_EXPERTS))
                enc_router_gemv_cls(raw, h, SL.s_shgate, pcsg, 1l)
            }
            var fsh = g_gpu.ffn_cmds[ffn_cmd_key(SL.sh1, SL.sh3, SL.sh2)]
            ffn_chain_enc(raw, h, fsh, SL.sh1, SL.sh3, SL.sh2, 1l, dim, a.nsh, a.is_gelu)
            var pcs = DecCombineShArgs(n = uint(dim), k = uint(a.k), gidx = uint(MOE_MAX_EXPERTS), gated = a.sh_gated ? 1u : 0u)
            enc_dec_combine_sh_cls(raw, h, SL.s_comb_sh, pcs, (dim + 255l) / 256l)
        } else {
            var pcc = DecCombineArgs(n = uint(dim), k = uint(a.k))
            enc_dec_combine_cls(raw, h, SL.s_comb, pcc, (dim + 255l) / 256l)
        }
        if (li + 1l < nl) {
            var pca2 = ArArgs(dim = uint(dim), add_on = 0u, woff = uint((li + 1l) * 2l * dim), eps = a.eps, ascale = 1.0)
            enc_cls_ar(raw, h, SL.s_ar_att, pca2, 1l)
            rq_enc(raw, h, SL.s_rq_att, a.fq[li + 1l], dim)
            vhz_dep(raw, h, VHS_ACTS, 0u)
        }
    }
    vhz_dep(raw, h, VHS_X, 0u, true)
    cmd_copy_range(raw, g_gpu.span_x_dev, 0l, g_gpu.span_out.buf, nl * 2l * kvd * 4l, dim * 4l)
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_span hz: {h.disp} nodes, {h.barriers} barriers over {nl} layers\n")
    }
    vk_check(vkEndCommandBuffer(raw), null)
}

//! Forget the recorded span so the next token records anew: the span records once per model and
//! asserts on a changed suffix, and a second model in one process - the tests' shape - has no
//! model drop between the two. The old record's objects stay model-owned until the drop.
def vk_span_reset {
    g_gpu.span_layers |> clear()
    g_gpu.span_key = -1l
    g_gpu.span_nl = 0l
    g_gpu.span_form = 0l
}

//! the routing shape a span record depends on, as one word: experts, slots, the shared expert's width and its gate
def private span_form_word(a : MoeSpanDec) : int64 => ((a.ne * 1024l + a.k) * 65536l + a.nsh) * 2l + (a.sh_gated ? 1l : 0l)

[cold_path]   // lazy init, once per span
def private ensure_span_state(a : MoeSpanDec) {
    let nl = long_length(a.woq)
    if (g_gpu.span_key >= 0l) {
        assert(g_gpu.span_key == a.woq[0] && g_gpu.span_nl == nl && g_gpu.span_form == span_form_word(a),
            "dasLLAMA vulkan tier: the decode span changed under a live model")
        return
    }
    assert(a.ne <= MOE_MAX_EXPERTS && a.k <= MAX_ROUTED_SLOTS, "dasLLAMA vulkan tier: decode span routing exceeds the top-k kernel's reach")
    verify(ensure_router_gemv_cls() && ensure_topk_cls() && ensure_dec_combine_cls() && ensure_cls_ar()
            && ensure_cls_q8k_rq() && ensure_cls_dn_rq() && (a.nsh == 0l || ensure_dec_combine_sh_cls()),
        "vk decode span: the class rails must engage on a live device")
    let dim = a.dim
    g_gpu.span_x_dev = make_device_buf(dim * 4l)
    g_gpu.span_xb_dev = make_device_buf(dim * 4l)
    g_gpu.span_w_dev = make_device_buf(MAX_ROUTED_SLOTS * 4l)
    g_gpu.span_eidx_dev = make_device_buf(MAX_ROUTED_SLOTS * 4l)
    g_gpu.span_logits_dev = make_device_buf(SPAN_LOGITS_BYTES)
    g_gpu.span_router_dev = make_device_buf(nl * a.ne * dim * 4l)
    g_gpu.span_norms_dev = make_device_buf(nl * 2l * dim * 4l)
    if (a.nsh > 0l && a.sh_gated) {
        g_gpu.span_shgate_dev = make_device_buf(nl * dim * 4l)
        upload_region_at(g_gpu.span_shgate_dev, 0l, unsafe(reinterpret<uint8 const?>(a.shgate)), nl * dim * 4l)
    }
    unsafe {
        upload_region_at(g_gpu.span_router_dev, 0l, reinterpret<uint8 const?>(a.router), nl * a.ne * dim * 4l)
        for (li in range64(nl)) {
            upload_region_at(g_gpu.span_norms_dev, li * 2l * dim * 4l, reinterpret<uint8 const?>(a.att_norm + li * dim), dim * 4l)
            upload_region_at(g_gpu.span_norms_dev, (li * 2l + 1l) * dim * 4l, reinterpret<uint8 const?>(a.ffn_norm + li * dim), dim * 4l)
        }
    }
    g_gpu.span_xin = make_host_buf(dim * 4l, false)
    g_gpu.span_out = make_host_buf(nl * 2l * a.kvd * 4l + dim * 4l, false, [cached = true])
    g_gpu.span_layers |> clear()
    g_gpu.span_layers |> reserve(nl)
    for (li in range64(nl)) {
        g_gpu.span_layers |> push(span_layer_sets(a, li))
    }
    span_record(a)
    g_gpu.span_key = a.woq[0]
    g_gpu.span_nl = nl
    g_gpu.span_form = span_form_word(a)
    let sh_note = a.nsh > 0l ? " with the shared expert" : ""
    to_log(LOG_INFO, "dasLLAMA vulkan tier: decode span engaged ({nl} resident layers{sh_note} as one submit per token; DASLLAMA_GPU_DEC_SPAN=0 runs the per-layer submits)\n")
}

//! MoeGpuSpanDecFn: one token through the resident layers - one submit; the host feeds the
//! first layer's attention row and takes back x and every layer's raw k/v rows.
[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]   // per token
def vk_moe_span_dec(var a : MoeSpanDec) {
    let ts0 = ref_time_ticks()
    let nl = long_length(a.woq)
    ensure_dat_state(a.dim, a.qd, a.kvd, a.hs)
    ensure_ffn_state()
    for (li in range64(nl)) {
        dat_layer_ensure(dat_key(a.woq[li], a.fq[li]), a.kvd)
        dat_layer_sets(a.woq[li], a.wok[li], a.wov[li], a.woo[li], a.fq[li], a.fk[li], a.fv[li], a.fo[li], a.rmsq[li], a.rmsk[li],
            a.bq[li], a.bk[li], a.bv[li], a.qk_norm)
    }
    ensure_span_state(a)
    assert(g_gpu.tail_pending_key < 0l, "dasLLAMA vulkan tier: decode span with a decode ffn tail pending")
    fill_stack_acts(g_gpu.span_layers[0].sq, a.xq, a.xs, a.dim)
    unsafe {
        memcpy(g_gpu.span_xin.mapped, reinterpret<void?>(a.x), a.dim * 4l)
        memcpy(g_gpu.dat_cos.mapped, reinterpret<void?>(a.cossin), a.hs * 4l)
        var tm = reinterpret<TokMeta?>(g_gpu.dat_tok.mapped)
        tm.pos = uint(a.pos)
        tm.cnt = uint(a.pos + 1l)
        tm.mirbase = 0u   //! the block's mirror rides the push: the attention adds the row's base to it
    }
    let ts1 = ref_time_ticks()
    submit_wait(g_gpu.span_cmd)
    unsafe {
        var op = reinterpret<uint8?>(g_gpu.span_out.mapped)
        let kvb = a.kvd * 4l
        for (li in range64(nl)) {
            memcpy(reinterpret<void?>(a.kout + li * a.kvd), reinterpret<void?>(op + li * 2l * kvb), kvb)
            memcpy(reinterpret<void?>(a.vout + li * a.kvd), reinterpret<void?>(op + li * 2l * kvb + kvb), kvb)
        }
        memcpy(reinterpret<void?>(a.x), reinterpret<void?>(op + nl * 2l * kvb), a.dim * 4l)
    }
    for (li in range64(nl)) {
        var L = unsafe(g_gpu.dat_layers?[dat_key(a.woq[li], a.fq[li])])
        L.cnt = a.pos + 1l
    }
    if (vk_prof()) {
        g_dec_prof_attn_rec_us += int64(get_time_usec(ts0)) - int64(get_time_usec(ts1))
        g_dec_prof_attn_wait_us += int64(get_time_usec(ts1))
        g_dec_prof_attn_us += int64(get_time_usec(ts0))
        g_dec_prof_attn_n++
        if (g_dec_prof_first_woq < 0l) {
            g_dec_prof_first_woq = a.woq[0]
        }
    }
}

// ===== decode q/k/v group (one submit per attention layer) =====
// Shared activation image + shared y (per-plane bases via the GEMV meta) ⇒ ONE submit, not three
// (~74us each would eat the win). Serial-chain work: pays on GPU read speed, nothing hides here.

def private qkv_cmd_make(sq, sk, sv : int; woq, wok, wov : int64; n, dq, dk, dv : int64) {
    ensure_gemv_scratch(sq)
    ensure_gemv_scratch(sk)
    ensure_gemv_scratch(sv)
    var qreg = fixed_array(woq, 0l, 1l)
    var kreg = fixed_array(wok, 0l, 1l)
    var vreg = fixed_array(wov, 0l, 1l)
    unsafe {   // regions are ours to pin: these stacks serve no other decode GEMV
        fill_stack_sched_rows(sq, addr(qreg[0]), 1l, 1l, 0l)
        fill_stack_sched_rows(sk, addr(kreg[0]), 1l, 1l, 0l)
        fill_stack_sched_rows(sv, addr(vreg[0]), 1l, 1l, 0l)
    }
    var qc = QkvCmd(cmd = alloc_cmd(), sq = sq, ybytes = (dq + dk + dv) * 4l)
    assert(qc.ybytes <= Y_BYTES, "dasLLAMA vulkan tier: qkv group exceeds output capacity")
    let fk = g_gpu.stacks[sk].fmt
    let fv = g_gpu.stacks[sv].fmt
    verify(gemv_cls_ensure(fk) && gemv_cls_ensure(fv),
        "vk decode: the gemv class rail must engage on a live device")
    // three disjoint row levels on the shared y (Y1/Y2/Y3): k/v sets ride the q stack's acts image
    qc.k_set = gemv_cls_set(fk, fixed_array(g_gpu.stacks[sk].wqbuf, g_gpu.stacks[sk].wsbuf, g_gpu.stacks[sk].meta.buf,
            g_gpu.stacks[sq].xq.buf, g_gpu.stacks[sq].xs.buf, g_gpu.stacks[sq].y_dev),
        fixed_array(g_gpu.stacks[sk].wqbytes, ws_range(sk), META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES),
        fixed_array(0u, 0u, 0u, 0u, 0u, VHG_Y2))
    qc.v_set = gemv_cls_set(fv, fixed_array(g_gpu.stacks[sv].wqbuf, g_gpu.stacks[sv].wsbuf, g_gpu.stacks[sv].meta.buf,
            g_gpu.stacks[sq].xq.buf, g_gpu.stacks[sq].xs.buf, g_gpu.stacks[sq].y_dev),
        fixed_array(g_gpu.stacks[sv].wqbytes, ws_range(sv), META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES),
        fixed_array(0u, 0u, 0u, 0u, 0u, VHG_Y3))
    let raw = qc.cmd
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    let fq = g_gpu.stacks[sq].fmt
    // the q stack's class set plays Y1 in this chain (role-fluid, re-declared per record)
    var qset = stack_gemv_cls_set(sq)
    hz_set_bits(qset, 0u, 0u, 0u, 0u, 0u, VHG_Y1)
    var h : VkHaz
    var pcq = GemvArgs(n = uint(n), d = uint(dq), nreg = 1u, ybase = 0u)
    gemv_enc(fq, raw, h, qset, pcq, dq)
    var pck = GemvArgs(n = uint(n), d = uint(dk), nreg = 1u, ybase = uint(dq))
    gemv_enc(fk, raw, h, qc.k_set, pck, dk)
    var pcv = GemvArgs(n = uint(n), d = uint(dv), nreg = 1u, ybase = uint(dq + dk))
    gemv_enc(fv, raw, h, qc.v_set, pcv, dv)
    vhz_dep(raw, h, VHG_Y1 | VHG_Y2 | VHG_Y3, 0u, true)
    cmd_copy_whole(raw, g_gpu.stacks[sq].y_dev, g_gpu.stacks[sq].y.buf, qc.ybytes)
    vk_check(vkEndCommandBuffer(raw), null)
    g_gpu.qkv_cmds |> insert(woq, qc)
}

//! MoeGpuQkvFn: one attention layer's q/k/v decode projections as ONE submit. The three planes
//! must share a quant CLASS (one activation image); the caller's gate enforces it.
def vk_moe_qkv(var yq : float?; var yk : float?; var yv : float?; woq, wok, wov : int64;
               fq, fk, fv : int; xqp : int8 const?; xsp : float const?; n, dq, dk, dv : int64) {
    let sq = find_stack(woq, fq, "decode qkv q")
    let sk = find_stack(wok, fk, "decode qkv k")
    let sv = find_stack(wov, fv, "decode qkv v")
    if (!key_exists(g_gpu.qkv_cmds, woq)) {
        qkv_cmd_make(sq, sk, sv, woq, wok, wov, n, dq, dk, dv)
    }
    var qc = g_gpu.qkv_cmds[woq]
    fill_stack_acts(sq, xqp, xsp, n)
    submit_wait(qc.cmd)
    unsafe {
        let yp = reinterpret<float?>(g_gpu.stacks[sq].y.mapped)
        memcpy(reinterpret<void?>(yq), reinterpret<void?>(yp), dq * 4l)
        memcpy(reinterpret<void?>(yk), reinterpret<void?>(yp + dq), dk * 4l)
        memcpy(reinterpret<void?>(yv), reinterpret<void?>(yp + dq + dk), dv * 4l)
    }
}

// ===== heat-pinned expert cache (decode split-FFN) =====
// Per-layer slot pools live in the SYNTHETIC offset space, so the whole decode FFN GEMV rail
// (find_stack, metas, FfnCmd) serves hit sets verbatim; uploads DMA-slice the pinned stream mirrors — no CPU gather on the decode path.

def private heat_stream_src(base : int64; fmt : int) : int {
    for (i in range(length(g_gpu.stream_stacks))) {
        if (g_gpu.stream_stacks[i].base == base && g_gpu.stream_stacks[i].fmt == fmt) {
            return i
        }
    }
    return -1
}

def private heat_pool_make(l : int64; b1, b3, b2 : int64; f1, f3, f2 : int; n, nfe : int64) {
    ensure_batch_state()   // advise records its slice copies into batch_cmd
    var pool = HeatPool(ege = n * nfe)
    let src1 = heat_stream_src(b1, f1)
    let src3 = heat_stream_src(b3, f3)
    let src2 = heat_stream_src(b2, f2)
    let want = gpu_want_heat()
    // gate/up share one act image; down is free — the mid-step form follows IT (the mixed-triple rail)
    if (src1 < 0 || src3 < 0 || src2 < 0 || kq_block32(f3) != kq_block32(f1) || want <= 0l
            || !arena_rows_ok(n, f1) || !arena_rows_ok(n, f3) || !arena_rows_ok(nfe, f2)) {
        let why = (want <= 0l ? "want 0"
            : (src1 < 0 || src3 < 0 || src2 < 0 ? "no mirror src {src1}/{src3}/{src2} for b {b1}/{b3}/{b2} fmt {f1}/{f3}/{f2}"
            : (kq_block32(f3) != kq_block32(f1) ? "gate/up form mismatch fmt {f1}/{f3}"
            : "row length not a block multiple ({n}/{nfe})")))
        to_log(LOG_INFO, "dasLLAMA vulkan tier: heat cache skips layer {l} ({why})\n")
        g_gpu.heat_pools[l] <- pool
        return
    }
    let pb1 = stack_plane_bytes(n, nfe, f1)
    let pb3 = stack_plane_bytes(n, nfe, f3)
    let pb2 = stack_plane_bytes(nfe, n, f2)
    let per_slot = pb1.wq + pb1.ws + pb3.wq + pb3.ws + pb2.wq + pb2.ws
    let c = min(want, (carved_budget() - g_gpu.resident_bytes) / per_slot)
    if (c <= 0l) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: heat cache skips layer {l} (VRAM budget)\n")
        g_gpu.heat_pools[l] <- pool
        return
    }
    let base = HEAT_BASE + l * 3l * HEAT_STRIDE
    pool.s1 = make_stack_shell(base, c * pool.ege, f1, c * pb1.wq, c * pb1.ws)
    pool.s3 = make_stack_shell(base + HEAT_STRIDE, c * pool.ege, f3, c * pb3.wq, c * pb3.ws)
    pool.s2 = make_stack_shell(base + 2l * HEAT_STRIDE, c * pool.ege, f2, c * pb2.wq, c * pb2.ws)
    ensure_gemv_scratch(pool.s1)
    ensure_gemv_scratch(pool.s3)
    ensure_gemv_scratch(pool.s2)
    pool.src1 = src1
    pool.src3 = src3
    pool.src2 = src2
    pool.nslots = int(c)
    pool.slot_expert |> resize(c)
    for (i in range64(c)) {
        pool.slot_expert[i] = -1l
    }
    g_gpu.resident_bytes += c * per_slot
    to_log(LOG_INFO, "dasLLAMA vulkan tier: heat cache layer {l}: {c} slots ({c * per_slot} bytes)\n")
    g_gpu.heat_pools[l] <- pool
}

// one plane slice, mirror -> pool slot (quants + scales; per-row bytes are uniform by layout)
def private heat_record_slice(raw : VkCommandBuffer; src_si, dst_si : int; e, slot, rowlen, ege : int64) {
    let rpe = ege / rowlen
    let rows_src = g_gpu.stream_stacks[src_si].elems / rowlen
    let rbq = g_gpu.stream_stacks[src_si].wqbytes / rows_src
    let rbs = g_gpu.stream_stacks[src_si].wsbytes / rows_src
    cmd_copy_range(raw, g_gpu.stream_stacks[src_si].hwq.buf, e * rpe * rbq,
        g_gpu.stacks[dst_si].wqbuf, slot * rpe * rbq, rpe * rbq)
    if (rbs > 0l) {
        cmd_copy_range(raw, g_gpu.stream_stacks[src_si].hws.buf, e * rpe * rbs,
            g_gpu.stacks[dst_si].wsbuf, slot * rpe * rbs, rpe * rbs)
    }
}

//! MoeGpuHeatAdviseFn: reconcile layer l's pool toward the desired hot set (ids heat-descending).
//! Pool creation is lazy (budget-clamped C); uploads are capped per call so a re-pin spreads
//! across advise ticks instead of stalling one token.
def vk_moe_heat_advise(l : int64; b1, b3, b2 : int64; f1, f3, f2 : int; n, nfe : int64; ids : int64 const?; nids : int64) {
    if (g_gpu == null) {
        return
    }
    if (!key_exists(g_gpu.heat_pools, l)) {
        heat_pool_make(l, b1, b3, b2, f1, f3, f2, n, nfe)
    }
    var pool & = unsafe(g_gpu.heat_pools[l])
    if (pool.nslots <= 0) {
        return
    }
    let nwant = min(nids, int64(pool.nslots))
    var keep : table<int64>
    for (i in range64(nwant)) {
        keep |> insert(unsafe(ids[i]))
    }
    var pinned = 0
    for (s in range(pool.nslots)) {
        let e = pool.slot_expert[s]
        if (e >= 0l && !key_exists(keep, e)) {
            pool.emap |> erase(e)
            pool.slot_expert[s] = -1l
        } elif (e >= 0l) {
            pinned++
        }
    }
    delete keep
    let cap = pinned * 2 < pool.nslots ? HEAT_UPLOAD_CAP_FILL : HEAT_UPLOAD_CAP
    var up = 0
    var free_s = 0
    var raw = g_gpu.batch_cmd
    for (i in range64(nwant)) {
        break if (up >= cap)
        let e = unsafe(ids[i])
        continue if (key_exists(pool.emap, e))
        while (free_s < pool.nslots && pool.slot_expert[free_s] >= 0l) {
            free_s++
        }
        break if (free_s >= pool.nslots)
        if (up == 0) {
            let rf : VkCommandBufferResetFlags
            vk_check(vkResetCommandBuffer(raw, rf), null)
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
        }
        heat_record_slice(raw, pool.src1, pool.s1, e, int64(free_s), n, pool.ege)
        heat_record_slice(raw, pool.src3, pool.s3, e, int64(free_s), n, pool.ege)
        heat_record_slice(raw, pool.src2, pool.s2, e, int64(free_s), nfe, pool.ege)
        pool.emap |> insert(e, free_s)
        pool.slot_expert[free_s] = e
        up++
    }
    if (up > 0) {
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
    }
}

//! MoeGpuHeatQueryFn: partition the routed top-k against layer l's pinned slots — hit flags per
//! original position, compacted synthetic offset triples for the hits.
def vk_moe_heat_query(l : int64; idx : int64 const?; k : int64; var hit : int8?; var o1 : int64?; var o3 : int64?; var o2 : int64?) : int64 {
    if (g_gpu == null || !key_exists(g_gpu.heat_pools, l)) {
        return 0l
    }
    let pool & = unsafe(g_gpu.heat_pools[l])
    if (pool.nslots <= 0) {
        return 0l
    }
    var nh = 0l
    unsafe {
        for (j in range64(k)) {
            let sl = pool.emap?[idx[j]] ?? -1
            hit[j] = int8(sl >= 0 ? 1 : 0)
            if (sl >= 0) {
                let so = int64(sl) * pool.ege
                o1[nh] = g_gpu.stacks[pool.s1].base + so
                o3[nh] = g_gpu.stacks[pool.s3].base + so
                o2[nh] = g_gpu.stacks[pool.s2].base + so
                nh++
            }
        }
    }
    return nh
}

// ===== classifier dispatch (the head GEMV) =====

// One-time dispatch state for a resident classifier plane: the regions pair never changes (ONE
// region covering the whole plane, activations at image row 0), so it uploads device-local once
// and the command buffer records once — per token only the activation refresh and the submit remain.
def private ensure_cls(si : int; woff, n, d : int64) {
    if (g_gpu.stacks[si].cls_made) {
        return
    }
    ensure_gemv_scratch(si)
    let fmt = g_gpu.stacks[si].fmt
    verify(gemv_cls_ensure(fmt), "vk decode: the gemv class rail must engage on a live device")
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.staging.mapped)
        mp[0] = span_meta_base(woff, si, fmt)
        mp[1] = 0u
    }
    g_gpu.stacks[si].cls_regions_dev = make_device_buf(CLS_META_BYTES)
    staged_upload(g_gpu.stacks[si].cls_regions_dev, 0l, CLS_META_BYTES)
    g_gpu.stacks[si].cls_set = gemv_cls_set(fmt,
        fixed_array(g_gpu.stacks[si].wqbuf, g_gpu.stacks[si].wsbuf, g_gpu.stacks[si].cls_regions_dev,
            g_gpu.stacks[si].xq.buf, g_gpu.stacks[si].xs.buf, g_gpu.stacks[si].y_dev),
        fixed_array(g_gpu.stacks[si].wqbytes, ws_range(si), CLS_META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES),
        fixed_array(0u, 0u, 0u, 0u, 0u, VHG_YO))
    var raw = alloc_cmd()
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u)
    gemv_enc(fmt, raw, h, g_gpu.stacks[si].cls_set, pc, d)
    vhz_dep(raw, h, VHG_YO, 0u, true)
    cmd_copy_whole(raw, g_gpu.stacks[si].y_dev, g_gpu.stacks[si].y.buf, d * 4l)
    vk_check(vkEndCommandBuffer(raw), null)
    g_gpu.stacks[si].cls_cmd = raw
    g_gpu.stacks[si].cls_made = true
}

//! MoeGpuClsFn: the classifier GEMV — y[d] = W · x over the resident plane at woff, activation
//! in the plane's own quant form (Q8_0 per-32 / Q8_K per-256), result DMA'd to the host-cached
//! staging in the same submit.
def vk_moe_cls(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d : int64; fmt : int) {
    let si = find_stack(woff, fmt, "classifier")
    assert(d * 4l <= Y_BYTES, "dasLLAMA vulkan tier: classifier vocab exceeds output capacity")
    ensure_cls(si, woff, n, d)
    fill_stack_acts(si, xqp, xsp, n)
    submit_wait(g_gpu.stacks[si].cls_cmd)
    unsafe {
        memcpy(reinterpret<void?>(yp), g_gpu.stacks[si].y.mapped, d * 4l)
    }
}

// ===== the decode attention block (the per-op tier): one submit per layer =====

var private g_dat_gen = 0l

[cold_path]   // lazy init, once per model
def private ensure_dat_state(dim, qd, kvd, hs : int64) {
    if (g_gpu.dat_ready) {
        assert(dim == g_gpu.dat_dim && qd == g_gpu.dat_qd && kvd == g_gpu.dat_kvd && hs == g_gpu.dat_hs,
            "dasLLAMA vulkan tier: decode attention block geometry changed under a live model")
        return
    }
    verify(ensure_qkn_rope_f16_cls() && ensure_rope_kv_f16_cls() && ensure_da_attn_f16_cls()
            && ensure_cls_q8k_rq() && ensure_cls_dn_rq(),
        "vk decode attention: the class rails must engage on a live device")
    g_gpu.dat_dim = dim
    g_gpu.dat_qd = qd
    g_gpu.dat_kvd = kvd
    g_gpu.dat_hs = hs
    g_gpu.dat_q_dev = make_device_buf(qd * 4l)
    g_gpu.dat_kv_dev = make_device_buf((qd + 2l * kvd) * 4l)
    g_gpu.dat_attn_dev = make_device_buf(qd * 4l)
    g_gpu.dat_aq_dev = make_device_buf(qd)
    g_gpu.dat_as_dev = make_device_buf((qd / 32l) * 4l)
    g_gpu.dat_xb2_dev = make_device_buf(dim * 4l)
    g_gpu.dat_cos = make_host_buf(hs * 4l, true)   // the kernels read the host-visible row directly
    g_gpu.dat_tok = make_host_buf(TOK_META_BYTES, true)
    g_gpu.dat_out = make_host_buf(dim * 4l + 2l * kvd * 4l, false, [cached = true])   // wo row | raw k | raw v
    g_gpu.dat_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: decode attention block engaged (mirror cap {DAT_MIRROR_ROWS} rows)\n")
}

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block")]   // once per layer: the chain's sets
def private dat_layer_sets(woq, wok, wov, woo : int64; fq, fk, fv, fo : int; rmsqp : float const?; rmskp : float const?;
                           bqp : float const?; bkp : float const?; bvp : float const?; qk_norm : bool) {
    var L & = unsafe(g_gpu.dat_layers[dat_key(woq, fq)])
    if (L.sets_made) {
        return
    }
    let sq = find_stack(woq, fq, "decode attn q")
    let sk = find_stack(wok, fk, "decode attn k")
    let sv = find_stack(wov, fv, "decode attn v")
    let so = find_stack(woo, fo, "decode attn o")
    ensure_gemv_scratch(sq)
    ensure_gemv_scratch(sk)
    ensure_gemv_scratch(sv)
    ensure_gemv_scratch(so)
    var qreg = fixed_array(woq, 0l, 1l)
    var kreg = fixed_array(wok, 0l, 1l)
    var vreg = fixed_array(wov, 0l, 1l)
    var oreg = fixed_array(woo, 0l, 1l)
    unsafe {   // regions are ours to pin: these stacks serve no other decode GEMV
        fill_stack_sched_rows(sq, addr(qreg[0]), 1l, 1l, 0l)
        fill_stack_sched_rows(sk, addr(kreg[0]), 1l, 1l, 0l)
        fill_stack_sched_rows(sv, addr(vreg[0]), 1l, 1l, 0l)
        fill_stack_sched_rows(so, addr(oreg[0]), 1l, 1l, 0l)
    }
    verify(gemv_cls_ensure(fq) && gemv_cls_ensure(fk) && gemv_cls_ensure(fv) && gemv_cls_ensure(fo),
        "vk decode attention: the gemv class rail must engage on a live device")
    let qd = g_gpu.dat_qd
    let kvd = g_gpu.dat_kvd
    let dim = g_gpu.dat_dim
    let hs = g_gpu.dat_hs
    let mirbytes = DAT_MIRROR_ROWS * kvd * 2l
    L.s_q = gemv_cls_set(fq, fixed_array(g_gpu.stacks[sq].wqbuf, g_gpu.stacks[sq].wsbuf, g_gpu.stacks[sq].meta.buf,
            g_gpu.stacks[sq].xq.buf, g_gpu.stacks[sq].xs.buf, g_gpu.dat_q_dev),
        fixed_array(g_gpu.stacks[sq].wqbytes, ws_range(sq), META_BYTES, XQ_BYTES, XS_BYTES, qd * 4l),
        fixed_array(0u, 0u, 0u, 0u, 0u, VHZ_Q))
    L.s_k = gemv_cls_set(fk, fixed_array(g_gpu.stacks[sk].wqbuf, g_gpu.stacks[sk].wsbuf, g_gpu.stacks[sk].meta.buf,
            g_gpu.stacks[sq].xq.buf, g_gpu.stacks[sq].xs.buf, g_gpu.dat_kv_dev),
        fixed_array(g_gpu.stacks[sk].wqbytes, ws_range(sk), META_BYTES, XQ_BYTES, XS_BYTES, (qd + 2l * kvd) * 4l),
        fixed_array(0u, 0u, 0u, 0u, 0u, VHZ_KVK))
    L.s_v = gemv_cls_set(fv, fixed_array(g_gpu.stacks[sv].wqbuf, g_gpu.stacks[sv].wsbuf, g_gpu.stacks[sv].meta.buf,
            g_gpu.stacks[sq].xq.buf, g_gpu.stacks[sq].xs.buf, g_gpu.dat_kv_dev),
        fixed_array(g_gpu.stacks[sv].wqbytes, ws_range(sv), META_BYTES, XQ_BYTES, XS_BYTES, (qd + 2l * kvd) * 4l),
        fixed_array(0u, 0u, 0u, 0u, 0u, VHZ_KVV))
    L.has_bias = bqp != null
    if (L.has_bias) {
        L.bias = make_device_buf((qd + 2l * kvd) * 4l)
        unsafe {
            upload_region_at(L.bias, 0l, reinterpret<uint8 const?>(bqp), qd * 4l)
            upload_region_at(L.bias, qd * 4l, reinterpret<uint8 const?>(bkp), kvd * 4l)
            upload_region_at(L.bias, (qd + kvd) * 4l, reinterpret<uint8 const?>(bvp), kvd * 4l)
        }
    }
    // hasb 0 leaves this binding unread - the cos buffer only fills the slot
    let bbuf = L.has_bias ? L.bias : g_gpu.dat_cos.buf
    let bbytes = L.has_bias ? (qd + 2l * kvd) * 4l : hs * 4l
    if (qk_norm) {
        L.norms = make_device_buf(2l * hs * 4l)
        unsafe {
            upload_region_at(L.norms, 0l, reinterpret<uint8 const?>(rmsqp), hs * 4l)
            upload_region_at(L.norms, hs * 4l, reinterpret<uint8 const?>(rmskp), hs * 4l)
        }
        L.s_qkr = set_qkn_rope_f16_cls(
            fixed_array(g_gpu.dat_q_dev, g_gpu.dat_kv_dev, L.k_mir, L.v_mir, g_gpu.dat_cos.buf, L.norms, g_gpu.dat_tok.buf, bbuf),
            fixed_array(qd * 4l, (qd + 2l * kvd) * 4l, mirbytes, mirbytes, hs * 4l, 2l * hs * 4l, TOK_META_BYTES, bbytes),
            fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, VHZ_MIR, VHZ_MIR, 0u, 0u, 0u, 0u))
    } else {
        L.s_qkr = set_rope_kv_f16_cls(
            fixed_array(g_gpu.dat_q_dev, g_gpu.dat_kv_dev, L.k_mir, L.v_mir, g_gpu.dat_cos.buf, g_gpu.dat_tok.buf, bbuf),
            fixed_array(qd * 4l, (qd + 2l * kvd) * 4l, mirbytes, mirbytes, hs * 4l, TOK_META_BYTES, bbytes),
            fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, VHZ_MIR, VHZ_MIR, 0u, 0u, 0u))
    }
    L.s_attn = set_da_attn_f16_cls(   //! the unsplit, unfolded pass: the out rows fill the partials and quant bindings, never read or written
        fixed_array(g_gpu.dat_q_dev, L.k_mir, L.v_mir, g_gpu.dat_attn_dev, g_gpu.dat_tok.buf, g_gpu.dat_attn_dev, g_gpu.dat_attn_dev, g_gpu.dat_attn_dev),
        fixed_array(qd * 4l, mirbytes, mirbytes, qd * 4l, TOK_META_BYTES, qd * 4l, qd * 4l, qd * 4l),
        fixed_array(VHZ_Q, VHZ_MIR, VHZ_MIR, VHZ_ATT, 0u, VHZ_ATT, VHZ_ATT, VHZ_ATT))
    L.s_rq = set_rq_cls(fixed_array(g_gpu.dat_attn_dev, g_gpu.dat_aq_dev, g_gpu.dat_as_dev),
        fixed_array(qd * 4l, qd, (qd / 32l) * 4l),
        fixed_array(VHZ_ATT, VHZ_AQ, VHZ_AQ))
    L.s_wo = gemv_cls_set(fo, fixed_array(g_gpu.stacks[so].wqbuf, g_gpu.stacks[so].wsbuf, g_gpu.stacks[so].meta.buf,
            g_gpu.dat_aq_dev, g_gpu.dat_as_dev, g_gpu.dat_xb2_dev),
        fixed_array(g_gpu.stacks[so].wqbytes, ws_range(so), META_BYTES, qd, (qd / 32l) * 4l, dim * 4l),
        fixed_array(0u, 0u, 0u, VHZ_AQ, VHZ_AQ, VHZ_XB2))
    L.fq = fq
    L.fk = fk
    L.fv = fv
    L.fo = fo
    L.sets_made = true
}

//! prefill window fills of a decode mirror since the last mint - record_at_cmd's fill increments it,
//! the claim consumes it
var g_dat_mirror_fills = 0l

//! the model drop's decode-side reset: a dropped model's fills must not mint the next model's generation
def vk_decode_drop_state {
    g_dat_mirror_fills = 0l
}

//! MoeGpuAttnDecClaimFn: mint the mirrors' generation - after a GPU prefill filled them (fresh =
//! false), or for a session decoding from position 0 (fresh = true: every layer starts empty).
def vk_moe_attn_dec_claim(fresh : bool) : int64 {
    if (!fresh && g_dat_mirror_fills == 0l) {   // nothing filled since the last mint: no mint, 0 = the caller keeps its own generation
        return 0l
    }
    if (g_gpu != null) {
        for (L in values(g_gpu.dat_layers)) {   // a mirror no window filled holds another session's rows
            if (fresh || !L.filled) {
                L.cnt = 0l
            }
            L.filled = false
        }
    }
    g_dat_mirror_fills = 0l
    g_dat_gen++
    return g_dat_gen
}

//! MoeGpuAttnDecOkFn: serve when the layer's mirror holds exactly [0, pos) of the session's
//! generation; else name what the engine must do first (claim fresh / hydrate the layer) or never.
def vk_moe_attn_dec_ok(woq : int64; fq : int; pos, gen : int64) : int {
    if (g_gpu == null || g_gpu.dry || pos >= DAT_MIRROR_ROWS) {
        return DAT_NEVER
    }
    if (gen == 0l || gen != g_dat_gen) {
        return DAT_STALE_GEN
    }
    let key = dat_key(woq, fq)
    if (!key_exists(g_gpu.dat_layers, key) || unsafe(g_gpu.dat_layers[key]).cnt != pos) {
        return DAT_HYDRATE
    }
    return DAT_SERVE
}

var private @scratch g_dat_h16 : array<float16>

//! MoeGpuAttnDecHydrateFn: the layer's mirror takes the host cache's rows [0, npos) (K roped, V
//! raw - the host cache's own forms; f16 rows copy through, f32 rows convert), then holds exactly
//! those positions. npos = 0 leaves it empty.
[cold_path]   // a remedy leg (a rewind, a CPU-prefilled prefix) - never the served token
def vk_moe_attn_dec_hydrate(woq : int64; fq : int; kp : uint8 const?; vp : uint8 const?; npos, kvd : int64; f16 : bool) {
    let key = dat_key(woq, fq)
    dat_layer_ensure(key, kvd)
    var L = unsafe(g_gpu.dat_layers?[key])
    if (npos > 0l && f16) {
        upload_region_at(L.k_mir, 0l, kp, npos * kvd * 2l)
        upload_region_at(L.v_mir, 0l, vp, npos * kvd * 2l)
    } elif (npos > 0l) {
        g_dat_h16 |> resize(npos * kvd)
        unsafe {
            let kf = reinterpret<float const?>(kp)
            let vf = reinterpret<float const?>(vp)
            for (i in range64(npos * kvd)) {
                g_dat_h16[i] = float16(clamp(kf[i], -65504.0, 65504.0))
            }
            upload_region_at(L.k_mir, 0l, addr<uint8 const?>(g_dat_h16[0]), npos * kvd * 2l)
            for (i in range64(npos * kvd)) {
                g_dat_h16[i] = float16(clamp(vf[i], -65504.0, 65504.0))
            }
            upload_region_at(L.v_mir, 0l, addr<uint8 const?>(g_dat_h16[0]), npos * kvd * 2l)
        }
    }
    L.cnt = npos
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block")]
def private at_chain_enc(raw : VkCommandBuffer; var h : VkHaz; var L : DatLayer; fq, fk, fv, fo : int; dim, qd, kvd, hs, n_heads, kv_mul : int64; scale, eps : float; neox, qk_norm : bool) {
    var pcq = GemvArgs(n = uint(dim), d = uint(qd), nreg = 1u, ybase = 0u)
    gemv_enc(fq, raw, h, L.s_q, pcq, qd)
    var pck = GemvArgs(n = uint(dim), d = uint(kvd), nreg = 1u, ybase = uint(qd))
    gemv_enc(fk, raw, h, L.s_k, pck, kvd)
    var pcv = GemvArgs(n = uint(dim), d = uint(kvd), nreg = 1u, ybase = uint(qd + kvd))
    gemv_enc(fv, raw, h, L.s_v, pcv, kvd)
    let hb = L.has_bias ? 1u : 0u   // the layer's own row is bound, so boff stays 0
    if (qk_norm) {
        var pcf = QknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(hs / 2l),
            neox = neox ? 1u : 0u, nh = uint(n_heads), layerbase = 0u, qwoff = 0u, kwoff = uint(hs), eps = eps, boff = 0u, hasb = hb)
        enc_qkn_rope_f16_cls(raw, h, L.s_qkr, pcf, n_heads + kvd / hs)
    } else {
        var pcr = RopeKvArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(hs / 2l),
            neox = neox ? 1u : 0u, kpair0 = uint(qd / 2l), npairs = uint(qd / 2l + kvd / 2l), layerbase = 0u, boff = 0u, hasb = hb)
        enc_rope_kv_f16_cls(raw, h, L.s_qkr, pcr, (qd / 2l + kvd / 2l + int64(WG_X) - 1l) / int64(WG_X))
    }
    var pca = DaAttnArgs(kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u, scale = scale, rqk = DA_RQ_NONE)   //! unsplit: no counters
    enc_da_attn_f16_cls(raw, h, L.s_attn, pca, da_attn_row_wgs(n_heads, kv_mul, 1l))
    rq_enc(raw, h, L.s_rq, fo, qd)
    var pcw = GemvArgs(n = uint(qd), d = uint(dim), nreg = 1u, ybase = 0u)
    gemv_enc(fo, raw, h, L.s_wo, pcw, dim)
}

//! MoeGpuAttnDecFn: one layer's decode attention on the device. The chain
//! records ONCE per layer (pos rides TokMeta); per token only the acts, the rope row and the
//! submit move. k and v come back raw - the host derives its cache row.
[cold_path]   // recording leg (also the rerecord A/B probe) - not the steady-state token path
def private at_record_layer(var L : DatLayer; fq, fk, fv, fo : int; dim, qd, kvd, hs, n_heads, kv_mul : int64; scale, eps : float; neox, qk_norm : bool) {
if (!L.recorded || vk_rdec_rerecord_on()) {
    if (!L.recorded) {
        L.cmd = alloc_cmd()
    } else {
        let rf : VkCommandBufferResetFlags
        vk_check(vkResetCommandBuffer(L.cmd, rf), null)
    }
    var raw = L.cmd
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    at_chain_enc(raw, h, L, fq, fk, fv, fo, dim, qd, kvd, hs, n_heads, kv_mul, scale, eps, neox, qk_norm)
    vhz_dep(raw, h, VHZ_XB2 | VHZ_KVK | VHZ_KVV, 0u, true)
    cmd_copy_whole(raw, g_gpu.dat_xb2_dev, g_gpu.dat_out.buf, dim * 4l)
    cmd_copy_range(raw, g_gpu.dat_kv_dev, qd * 4l, g_gpu.dat_out.buf, dim * 4l, 2l * kvd * 4l)
    vk_check(vkEndCommandBuffer(raw), null)
    L.recorded = true
}
}

[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block")]   // per token
def vk_moe_attn_dec(var xb2p : float?; var kp : float?; var vp : float?; woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                    xqp : int8 const?; xsp : float const?; cosp : float const?; rmsqp : float const?; rmskp : float const?;
                    bqp : float const?; bkp : float const?; bvp : float const?;
                    pos, dim, qd, kvd, hs, n_heads, kv_mul : int64; scale, eps : float; neox, qk_norm : bool) {
    let ts0 = ref_time_ticks()
    ensure_dat_state(dim, qd, kvd, hs)
    let key = dat_key(woq, fq)
    dat_layer_ensure(key, kvd)
    dat_layer_sets(woq, wok, wov, woo, fq, fk, fv, fo, rmsqp, rmskp, bqp, bkp, bvp, qk_norm)
    var L = unsafe(g_gpu.dat_layers?[key])   // the ensure above made the key: a read, never an insert
    let sq = find_stack(woq, fq, "decode attn q")
    at_record_layer(*L, fq, fk, fv, fo, dim, qd, kvd, hs, n_heads, kv_mul, scale, eps, neox, qk_norm)
    let has_pending_tail = g_gpu.tail_pending_key >= 0l
    if (has_pending_tail) {
        assert(g_gpu.tail_pending_key == key, "dasLLAMA vulkan tier: the pending decode ffn tail targets another layer")
    } else {
        fill_stack_acts(sq, xqp, xsp, dim)
    }
    unsafe {
        memcpy(g_gpu.dat_cos.mapped, reinterpret<void?>(cosp), hs * 4l)
        var tm = reinterpret<TokMeta?>(g_gpu.dat_tok.mapped)
        tm.pos = uint(pos)
        tm.cnt = uint(pos + 1l)
        tm.mirbase = 0u
    }
    let ts1 = ref_time_ticks()
    submit_wait(L.cmd)
    if (vk_prof()) {
        g_dec_prof_attn_rec_us += int64(get_time_usec(ts0)) - int64(get_time_usec(ts1))
        g_dec_prof_attn_wait_us += int64(get_time_usec(ts1))
    }
    unsafe {
        var op = reinterpret<uint8?>(g_gpu.dat_out.mapped)
        memcpy(reinterpret<void?>(xb2p), reinterpret<void?>(op), dim * 4l)
        memcpy(reinterpret<void?>(kp), reinterpret<void?>(op + dim * 4l), kvd * 4l)
        memcpy(reinterpret<void?>(vp), reinterpret<void?>(op + dim * 4l + kvd * 4l), kvd * 4l)
    }
    L.cnt = pos + 1l
    if (has_pending_tail) {
        unsafe {
            memcpy(reinterpret<void?>(g_gpu.tail_xout), g_gpu.tail_xres.mapped, dim * 4l)
        }
        g_gpu.tail_pending_key = -1l
        g_gpu.tail_xout = null
    }
    if (vk_prof()) {
        g_dec_prof_attn_us += int64(get_time_usec(ts0))
        g_dec_prof_attn_n++
        if (g_dec_prof_first_woq < 0l) {
            g_dec_prof_first_woq = woq
        }
        if (woq == g_dec_prof_first_woq) {
            g_dec_prof_tokens++
            if (g_dec_prof_tokens % 32l == 0l) {
                to_log(LOG_INFO, "vk_dec prof over {g_dec_prof_tokens} tokens: attn block {g_dec_prof_attn_us / g_dec_prof_tokens} us/token ({g_dec_prof_attn_n / g_dec_prof_tokens} submits, {g_dec_prof_attn_us / max(g_dec_prof_attn_n, 1l)} us each = record {g_dec_prof_attn_rec_us / max(g_dec_prof_attn_n, 1l)} + submit/wait {g_dec_prof_attn_wait_us / max(g_dec_prof_attn_n, 1l)}), ffn chain {g_dec_prof_ffn_us / g_dec_prof_tokens} us/token ({g_dec_prof_ffn_n / g_dec_prof_tokens} submits, {g_dec_prof_ffn_us / max(g_dec_prof_ffn_n, 1l)} us each)\n")   // nolint:PERF026,PERF028 — vk_prof()-gated report leg
            }
        }
    }
}
