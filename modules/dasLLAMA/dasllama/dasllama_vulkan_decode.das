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

//! Resident-driver prepare: allocate all per-token + session buffers for a model's geometry. One
//! resident model at a time (the arbiter guarantees it). Call before set_layer / token.
//! `kv16` picks the mirror codec (f16 default upstream) — sessions of the same codec byte-copy.
def vk_rdec_prepare(n_layers, n_attn, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64;
                    neox : bool; eps, scale : float; qk_norm : bool; kv16 : bool; q_gated : bool; rot : int64; dn : RdecDnGeom;
                    mir_elems, cos_elems : int64) : bool {
    if (!vk_moe_init()) {
        return false
    }
    let esz = kv16 ? 2l : 4l
    // each mirror side binds as ONE SSBO range: past maxStorageBufferRange (or the shaders' uint
    // word index) every read is undefined. The ctx negotiation caps first; guard direct callers.
    if (mir_elems * esz > vk_max_storage_range() || mir_elems > 4_294_967_295l) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: KV mirror ({n_attn} layers x {seq_cap} positions, {mir_elems} elements a side) exceeds maxStorageBufferRange {vk_max_storage_range()} / the 32-bit word index\n")
        return false
    }
    // the residual-stream kernels stage a whole row in ar_row, and the per-head ones a whole head
    // in their row slabs — over either slab they would write past the workgroup array with no diagnostic
    if (dim > AR_MAX_DIM || head_size > 512l) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: dim {dim} > {AR_MAX_DIM} or head_size {head_size} > 512 exceeds the workgroup row slabs\n")
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
    if (hybrid && !g_gpu.dn_shared_ok) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the device's shared-memory cap declined the deltanet chain - resident driver declines, the per-op rails serve\n")
        return false
    }
    if (hybrid && !(ensure_dn_step_cls() && ensure_router_gemv_f16_cls())) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the deltanet step class rail declined on this device - resident driver declines, the per-op rails serve\n")
        return false
    }
    // all-or-nothing: pre-flight the SAME prefill-attention pick pf_setup will make (fa tile,
    // h128 twin, or the chunked pair) so a declined kernel declines residency PRE-upload
    let will_facm2 = (g_gpu.has_coopmat2_fa && vk_fa_on() && kv16
        && (head_size == 64l || head_size == 128l || head_size == 256l || head_size == 512l))
    if (!(will_facm2
            ? (head_size == 64l ? ensure_fa_cm2_h64_cls() : (head_size == 128l ? ensure_fa_cm2_h128_cls()
                : (head_size == 512l ? ensure_fa_cm2_h512_cls() : (q_gated ? ensure_fa_cm2_h256_g_cls() : ensure_fa_cm2_h256_cls()))))
            : (g_gpu.has_coopmat && head_size == 128l && vk_fa_on()
                ? (kv16 ? ensure_da_attn_b_h128_f16_cls() : ensure_da_attn_b_h128_cls())
                : (kv16 ? ensure_da_attn_b_f16_cls() : ensure_da_attn_b_cls())))) {
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
    let wide = max(max(qd, hidden), dim)
    r.x_dev = make_device_buf(dim * 4l)
    r.xb_dev = make_device_buf(dim * 4l)
    r.xq_dev = make_device_buf(wide)              // int8 quants, 1 byte/elem
    r.xs_dev = make_device_buf((wide / 32l) * 4l)
    r.q_dev = make_device_buf((q_gated ? 2l * qd : qd) * 4l)
    r.kv_dev = make_device_buf((qd + 2l * kv_dim) * 4l)
    r.attn_dev = make_device_buf(qd * 4l)
    r.attn_nsplit = da_nsplit(n_heads, int64(g_gpu.sm_count))
    r.attn_part_dev = make_device_buf(n_heads * DA_NSPLIT_MAX * (head_size + 2l) * 4l)
    r.aq_dev = make_device_buf(qd)
    r.as_dev = make_device_buf((qd / 32l) * 4l)
    r.gate_dev = make_device_buf(hidden * 4l)
    r.up_dev = make_device_buf(hidden * 4l)
    r.ffnout_dev = make_device_buf(dim * 4l)
    r.xb2_dev = make_device_buf(dim * 4l)
    r.n_attn = n_attn
    r.mir_elems = mir_elems
    // the two largest allocations of the load: a heap the last model fragmented can refuse them though the plan fit - decline, never die mid-upload
    r.k_mirror = try_device_buf(mir_elems * esz)
    r.v_mirror = try_device_buf(mir_elems * esz)
    if (r.k_mirror == 0ul || r.v_mirror == 0ul) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the KV mirror ({(2l * mir_elems * esz) >> 20l} MB for ctx {seq_cap}) found no room on the device - resident driver declines, the per-op rails serve\n")
        return false
    }
    r.hybrid = hybrid
    if (hybrid) {
        r.dn_cd = dn.cd; r.dn_kd = dn.kd; r.dn_di = dn.di; r.dn_nvh = dn.nvh; r.dn_nkh = dn.nkh; r.dn_ds = dn.ds; r.dn_dconv = dn.dconv
        r.dn_ba_f32 = dn.ba_f32
        r.dn_proj_dev = make_device_buf((dn.cd + dn.di) * 4l)
        r.dn_oq_dev = make_device_buf(DND_OQ_BYTES)
        r.dn_os_dev = make_device_buf(DND_OS_BYTES)
        r.dn_of_dev = make_device_buf(DND_OQ_BYTES * 4l)
        r.dn_parity = 0
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
    r.cos_dev = make_device_buf(r.cos_elems * 4l)
    r.logits_dev = make_device_buf(vocab * 4l)
    r.dummy = make_device_buf(256l)
    r.x_host = make_host_buf(dim * 4l, false, [cached = true])
    r.cos_host = make_host_buf(r.cos_elems * 4l, false, [cached = true])
    r.tok_meta = make_host_buf(16l, true)   // the shared TokMeta block (pos/cnt)
    r.logits_host = make_host_buf(vocab * 4l, false, [cached = true])
    r.kv_readback = make_host_buf(2l * kv_dim * 4l * KV_READBACK_ROWS, false, [cached = true])
    ensure_ffn_state()   // hq_dev/hs_dev for the FFN act+requant
    r.cmd = alloc_cmd()
    return true
}

//! Upload the norm-weight rows once: [rms_att[0], rms_ffn[0], ..., rms_att[L-1], rms_ffn[L-1],
//! rms_final], dim floats each, exactly the layout the layer w_offs index. Under qk_norm the
//! per-head [rms_q[l], rms_k[l]] x L rows (head_size floats each) follow.
def vk_rdec_upload_norms(norms : array<float>) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null, "vk_rdec_upload_norms before prepare")
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
    let p1 = arena_planes(f1, b1)
    let p3 = arena_planes(f3, b3)
    let p2 = arena_planes(f2, b2)
    L.b1 = b1; L.b3 = b3; L.b2 = b2
    L.f1 = f1; L.f3 = f3; L.f2 = f2
    L.m_gate = rd_meta(); L.m_up = rd_meta(); L.m_down = rd_meta()
    unsafe {
        // addrms (class): x += xb2, norms, xb out — and the fused twin quantizing in place of xb
        L.s_addr_ffn = set_cls_ar(fixed_array(g_rd.x_dev, g_rd.xb2_dev, g_rd.norms_dev, g_rd.xb_dev),
            fixed_array(dim * 4l, dim * 4l, g_rd.norms_bytes, dim * 4l),
            fixed_array(VHZ_X, VHZ_XB2, 0u, VHZ_XB))
        g_rd.fused_sets_made = false
        // quantize xb (post-ffn-norm) -> xq/xs
        L.s_quant_xb2 = set_rq_cls(fixed_array(g_rd.xb_dev, g_rd.xq_dev, g_rd.xs_dev),
            fixed_array(dim * 4l, dim, (dim / 32l) * 4l),
            fixed_array(VHZ_XB, VHZ_XQ, VHZ_XQ))
        // gate/up GEMV
        L.s_gate = gemv_cls_set(f1, fixed_array(p1.wq, p1.ws, L.m_gate.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.gate_dev),
            fixed_array(p1.wqb, p1.wsb, META_BYTES, dim, (dim / 32l) * 4l, hid * 4l),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_GATE))
        L.s_up = gemv_cls_set(f3, fixed_array(p3.wq, p3.ws, L.m_up.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.up_dev),
            fixed_array(p3.wqb, p3.wsb, META_BYTES, dim, (dim / 32l) * 4l, hid * 4l),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_UP))
        // act+requant (class): gate@0, up@1, hq@2, hs@3
        L.s_actrq = set_actrq_cls(fixed_array(g_rd.gate_dev, g_rd.up_dev, g_gpu.hq_dev, g_gpu.hs_dev),
            fixed_array(hid * 4l, hid * 4l, BATCH_HQ_BYTES, BATCH_HS_BYTES),
            fixed_array(VHZ_GATE, VHZ_UP, VHZ_HQ, VHZ_HQ))
        // down GEMV -> ffnout
        L.s_down = gemv_cls_set(f2, fixed_array(p2.wq, p2.ws, L.m_down.buf, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.ffnout_dev),
            fixed_array(p2.wqb, p2.wsb, META_BYTES, hid, (hid / 32l) * 4l, dim * 4l),
            fixed_array(0u, 0u, 0u, VHZ_HQ, VHZ_HQ, VHZ_FFO))
        // addrms next: x += ffnout, xb = rms_att[l+1]|final — the fused twin also serves the last layer's final norm
        L.s_addr_next = set_cls_ar(fixed_array(g_rd.x_dev, g_rd.ffnout_dev, g_rd.norms_dev, g_rd.xb_dev),
            fixed_array(dim * 4l, dim * 4l, g_rd.norms_bytes, dim * 4l),
            fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB))
    }
}

def private rd_quant_xb_set() : VkDescriptorSet {
    let dim = g_rd.dim
    return set_rq_cls(fixed_array(g_rd.xb_dev, g_rd.xq_dev, g_rd.xs_dev),
        fixed_array(dim * 4l, dim, (dim / 32l) * 4l),
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
    unsafe {
        var L & = g_rd.layers[l]
        rd_layer_geom(L, g)
        rd_attn_sets(L, mir_idx, bq, bk, bv, bo, fq, fk, fv, fo, f1, f3, f2)
        rd_ffn_sets(L, b1, b3, b2, f1, f3, f2)
        L.made = true
    }
    g_rd.tok_recorded = false   //! the recorded chain references the replaced sets
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
        let pq = arena_planes(fq, bq)
        let pk = L.kv_shared ? pq : arena_planes(fk, bk)   //! a shared-KV layer has no k/v planes: the q planes fill the slots, the k/v sets stay unused
        let pv = (L.vfk || L.kv_shared) ? pk : arena_planes(fv, bv)   //! no v plane: the k planes fill the slot, the v set stays unused
        let po = arena_planes(fo, bo)
        let anykq = kq_sb(fq) || kq_sb(fo) || kq_sb(f1) || kq_sb(f2)   //! kq consumers take the Q8_K quant/act forms
        let kv16 = g_rd.kv16
        verify(ensure_cls_dn_rq() && ensure_cls_ar() && ensure_cls_ar_rq() && ensure_q8_actrq_cls()
            && (kv16 ? ensure_rope_kv_f16_cls() : ensure_rope_kv_cls())
            && (kv16 ? ensure_da_attn_f16_cls() : ensure_da_attn_cls())
            && (!g_rd.qk_norm || (ensure_qk_rms_cls()
                && (kv16 ? ensure_qkn_rope_f16_cls() : ensure_qkn_rope_cls())))
            && (!anykq || (ensure_cls_q8k_rq() && ensure_q8k_actrq_cls()))
            && gemv_cls_ensure(fq) && (L.kv_shared || (gemv_cls_ensure(fk) && (L.vfk || gemv_cls_ensure(fv)))) && gemv_cls_ensure(fo)
            && gemv_cls_ensure(f1) && gemv_cls_ensure(f3) && gemv_cls_ensure(f2),
            "vk decode: the class rails must engage on a live device")
        L.m_q = rd_meta(); L.m_k = rd_meta(); L.m_v = rd_meta(); L.m_wo = rd_meta()
        L.s_quant_xb = rd_quant_xb_set()
        let qbytes = (g_rd.q_gated ? 2l * qd : qd) * 4l
        // q/k/v class GEMV: planes @0/1, regions@2, xq@3, xs@4, out@5
        L.s_q = gemv_cls_set(fq, fixed_array(pq.wq, pq.ws, L.m_q.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.q_dev),
            fixed_array(pq.wqb, pq.wsb, META_BYTES, dim, (dim / 32l) * 4l, qbytes),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_Q))
        if (!L.kv_shared) {
            L.s_k = gemv_cls_set(fk, fixed_array(pk.wq, pk.ws, L.m_k.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev),
                fixed_array(pk.wqb, pk.wsb, META_BYTES, dim, (dim / 32l) * 4l, (qd + 2l * kvd) * 4l),
                fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVK))
        }
        if (!L.vfk && !L.kv_shared) {
            L.s_v = gemv_cls_set(fv, fixed_array(pv.wq, pv.ws, L.m_v.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev),
                fixed_array(pv.wqb, pv.wsb, META_BYTES, dim, (dim / 32l) * 4l, (qd + 2l * kvd) * 4l),
                fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVV))
        }
        L.kv_merged = !L.vfk && !L.kv_shared && fk == fv && arena_slab_idx(bk) == arena_slab_idx(bv)   //! same-format k/v in one slab: one two-region dispatch fills both halves
        if (L.kv_merged) {
            L.s_kv = gemv_cls_set(fk, fixed_array(pk.wq, pk.ws, L.m_k.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev),
                fixed_array(pk.wqb, pk.wsb, META_BYTES, dim, (dim / 32l) * 4l, (qd + 2l * kvd) * 4l),
                fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVK | VHZ_KVV))
        }
        let mirbytes = g_rd.mir_elems * g_rd.kv_esz
        let cosbytes = g_rd.cos_elems * 4l
        let bias_bind = rd_bias_bind()
        if (g_rd.qk_norm) {
            // per-head q/k rmsnorm (class): q rows, k rows (kv_dev — kbase=qd rides the push), norms
            L.s_qkn = set_qk_rms_cls(fixed_array(g_rd.q_dev, g_rd.kv_dev, g_rd.norms_dev),
                fixed_array(qd * 4l, (qd + 2l * kvd) * 4l, g_rd.norms_bytes),
                fixed_array(VHZ_Q, VHZ_KVK, 0u))
            // the fused twin (class): norm rows bind the norms buffer, mirror row rides TokMeta, the bias rows their buffer
            let qrbufs = fixed_array(g_rd.q_dev, g_rd.kv_dev, g_rd.k_mirror, g_rd.v_mirror, g_rd.cos_dev, g_rd.norms_dev, g_rd.tok_meta.buf, bias_bind.buf)
            let qrsizes = fixed_array(qbytes, (qd + 2l * kvd) * 4l, mirbytes, mirbytes, cosbytes, g_rd.norms_bytes, 16l, bias_bind.bytes)
            let qrbits = fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, VHZ_MIR, VHZ_MIR, VHZ_COS, 0u, 0u, 0u)
            L.s_qkn_rope = (kv16 ? set_qkn_rope_f16_cls(qrbufs, qrsizes, qrbits)
                : set_qkn_rope_cls(qrbufs, qrsizes, qrbits))
        }
        // rope+store (class): per-token mirror row rides TokMeta, the bias rows their buffer
        let rbufs = fixed_array(g_rd.q_dev, g_rd.kv_dev, g_rd.k_mirror, g_rd.v_mirror, g_rd.cos_dev, g_rd.tok_meta.buf, bias_bind.buf)
        let rsizes = fixed_array(qd * 4l, (qd + 2l * kvd) * 4l, mirbytes, mirbytes, cosbytes, 16l, bias_bind.bytes)
        let rbits = fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, VHZ_MIR, VHZ_MIR, VHZ_COS, 0u, 0u)
        L.s_rope = kv16 ? set_rope_kv_f16_cls(rbufs, rsizes, rbits) : set_rope_kv_cls(rbufs, rsizes, rbits)
        // attn (class): per-token cnt rides TokMeta, the scale rides the push
        let partbytes = g_rd.n_heads * DA_NSPLIT_MAX * (g_rd.head_size + 2l) * 4l
        let abufs = fixed_array(g_rd.q_dev, g_rd.k_mirror, g_rd.v_mirror, g_rd.attn_dev, g_rd.tok_meta.buf, g_rd.attn_part_dev)
        let asizes = fixed_array(qbytes, mirbytes, mirbytes, qd * 4l, 16l, partbytes)
        let abits = fixed_array(VHZ_Q, VHZ_MIR, VHZ_MIR, VHZ_ATT, 0u, VHZ_ATT)   //! the partials ride the attention-out bit: written by the pass, read by the combine that writes the row
        L.s_attn = kv16 ? set_da_attn_f16_cls(abufs, asizes, abits) : set_da_attn_cls(abufs, asizes, abits)
        if (g_rd.attn_nsplit > 1l) {
            verify(ensure_da_attn_comb_cls(), "vk decode: the split attention's combine class rail must engage on a live device")
            L.s_attn_comb = set_da_attn_comb_cls(fixed_array(g_rd.q_dev, g_rd.attn_part_dev, g_rd.attn_dev),
                fixed_array(qbytes, partbytes, qd * 4l), fixed_array(VHZ_Q, VHZ_ATT, VHZ_ATT))
        }
        // quantize attn -> aq/as
        L.s_quant_at = set_rq_cls(fixed_array(g_rd.attn_dev, g_rd.aq_dev, g_rd.as_dev),
            fixed_array(qd * 4l, qd, (qd / 32l) * 4l),
            fixed_array(VHZ_ATT, VHZ_AQ, VHZ_AQ))
        // wo GEMV -> xb2
        L.s_wo = gemv_cls_set(fo, fixed_array(po.wq, po.ws, L.m_wo.buf, g_rd.aq_dev, g_rd.as_dev, g_rd.xb2_dev),
            fixed_array(po.wqb, po.wsb, META_BYTES, qd, (qd / 32l) * 4l, dim * 4l),
            fixed_array(0u, 0u, 0u, VHZ_AQ, VHZ_AQ, VHZ_XB2))
    }
}

// ===== the MoE form: the router plane, the routed block of the token command =====

let private MOE_MAX_EXPERTS = 256l   //! the select kernels' one-workgroup reach: one thread per expert

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def vk_rdec_prepare_moe(ne, k, nfe, nsh, n_moe : int64; sh_gated, norm_topk, is_gelu : bool; wscale : float; router : array<float>; dscale : array<float>) : bool {
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
    r.wscale = wscale
    if (!(ensure_router_gemv_cls() && ensure_topk_cls() && ensure_cls_ar_comb()
            && ensure_q8_actrq_cls() && ensure_q8k_actrq_cls())) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the routing class rails declined on this device\n")
        return false
    }
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
    r.moe_dlog_dev = make_device_buf(r.ner * 4l)
    r.moe_dw_dev = make_device_buf(MAX_ROUTED_SLOTS * 4l)
    r.moe_meta1_dev = make_device_buf(META_BYTES)
    r.moe_meta3_dev = make_device_buf(META_BYTES)
    r.moe_meta2_dev = make_device_buf(META_BYTES)
    r.moe_xq_dev = make_device_buf(dim)
    r.moe_xs_dev = make_device_buf((dim / 32l) * 4l)
    r.egate_dev = make_device_buf(k * nfe * 4l)
    r.eup_dev = make_device_buf(k * nfe * 4l)
    r.edown_dev = make_device_buf(k * dim * 4l)
    return true
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def private rd_moe_sets(var L : RLayer) {
    let dim = g_rd.dim
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
    if (L.xe_own) {
        L.s_quant_xe = set_rq_cls(fixed_array(g_rd.xb_dev, g_rd.moe_xq_dev, g_rd.moe_xs_dev),
            fixed_array(dim * 4l, dim, (dim / 32l) * 4l), fixed_array(VHZ_XB, VHZ_XQ, VHZ_XQ))
    }
    L.s_router = set_router_gemv_cls(fixed_array(g_rd.router_dev, g_rd.xb_dev, g_rd.moe_dlog_dev),
        fixed_array(g_rd.router_bytes, dim * 4l, g_rd.ner * 4l), fixed_array(0u, VHZ_XB, VHZ_MRT))
    L.s_topk = set_topk_cls(fixed_array(g_rd.moe_dlog_dev, g_rd.moe_meta1_dev, g_rd.moe_meta3_dev, g_rd.moe_meta2_dev, g_rd.moe_dw_dev, g_rd.moe_ds_dev),
        fixed_array(g_rd.ner * 4l, META_BYTES, META_BYTES, META_BYTES, MAX_ROUTED_SLOTS * 4l, g_rd.moe_ds_bytes),
        fixed_array(VHZ_MRT, VHZ_MRT, VHZ_MRT, VHZ_MRT, VHZ_MRT, 0u))
    L.s_egate = gemv_cls_set(L.fe1, fixed_array(pe1.wq, pe1.ws, g_rd.moe_meta1_dev, xqb, xsb, g_rd.egate_dev),
        fixed_array(pe1.wqb, pe1.wsb, META_BYTES, dim, (dim / 32l) * 4l, k * nfe * 4l),
        fixed_array(0u, 0u, VHZ_MRT, VHZ_XQ, VHZ_XQ, VHZ_GATE))
    L.s_eup = gemv_cls_set(L.fe3, fixed_array(pe3.wq, pe3.ws, g_rd.moe_meta3_dev, xqb, xsb, g_rd.eup_dev),
        fixed_array(pe3.wqb, pe3.wsb, META_BYTES, dim, (dim / 32l) * 4l, k * nfe * 4l),
        fixed_array(0u, 0u, VHZ_MRT, VHZ_XQ, VHZ_XQ, VHZ_UP))
    L.s_eact = set_actrq_cls(fixed_array(g_rd.egate_dev, g_rd.eup_dev, g_gpu.hq_dev, g_gpu.hs_dev),
        fixed_array(k * nfe * 4l, k * nfe * 4l, BATCH_HQ_BYTES, BATCH_HS_BYTES),
        fixed_array(VHZ_GATE, VHZ_UP, VHZ_HQ, VHZ_HQ))
    L.s_edown = gemv_cls_set(L.fe2, fixed_array(pe2.wq, pe2.ws, g_rd.moe_meta2_dev, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.edown_dev),
        fixed_array(pe2.wqb, pe2.wsb, META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, k * dim * 4l),
        fixed_array(0u, 0u, VHZ_MRT, VHZ_HQ, VHZ_HQ, VHZ_MDN))
    if (L.moe_g4) {
        verify(ensure_cls_ar_comb_g4(), "vk decode: the gemma-4 combine-folded residual class rail must engage on a live device")
        L.s_addr_next = set_cls_ar_comb_g4(fixed_array(g_rd.x_dev, g_rd.ffnout_dev, g_rd.norms_dev, g_rd.xb_dev,
                g_rd.moe_ident_dev, g_rd.moe_dw_dev, g_rd.edown_dev),
            fixed_array(dim * 4l, dim * 4l, g_rd.norms_bytes, dim * 4l, MAX_ROUTED_SLOTS * 4l, MAX_ROUTED_SLOTS * 4l, k * dim * 4l),
            fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB, 0u, VHZ_MRT, VHZ_MDN))
        return
    }
    verify(ensure_cls_ar_comb(), "vk decode: the combine-folded residual class rail must engage on a live device")
    L.s_addr_next = set_cls_ar_comb(fixed_array(g_rd.x_dev, g_rd.ffnout_dev, g_rd.norms_dev, g_rd.xb_dev,
            g_rd.moe_ident_dev, g_rd.moe_dw_dev, g_rd.edown_dev, g_rd.moe_dlog_dev),
        fixed_array(dim * 4l, dim * 4l, g_rd.norms_bytes, dim * 4l, MAX_ROUTED_SLOTS * 4l, MAX_ROUTED_SLOTS * 4l, k * dim * 4l, g_rd.ner * 4l),
        fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB, 0u, VHZ_MRT, VHZ_MDN, VHZ_MRT))
}

//! a layer without a dense triple (an MoE layer with no shared expert): the tail sets the token
//! command encodes around the routed block - the FFN norm, its requant and the residual step
def private rd_tail_sets_bare(var L : RLayer; f1, f3, f2 : int) {
    let dim = g_rd.dim
    L.b1 = -1l; L.b3 = -1l; L.b2 = -1l
    L.f1 = f1; L.f3 = f3; L.f2 = f2
    L.s_addr_ffn = set_cls_ar(fixed_array(g_rd.x_dev, g_rd.xb2_dev, g_rd.norms_dev, g_rd.xb_dev),
        fixed_array(dim * 4l, dim * 4l, g_rd.norms_bytes, dim * 4l),
        fixed_array(VHZ_X, VHZ_XB2, 0u, VHZ_XB))
    L.s_quant_xb2 = set_rq_cls(fixed_array(g_rd.xb_dev, g_rd.xq_dev, g_rd.xs_dev),
        fixed_array(dim * 4l, dim, (dim / 32l) * 4l),
        fixed_array(VHZ_XB, VHZ_XQ, VHZ_XQ))
    L.s_addr_next = set_cls_ar(fixed_array(g_rd.x_dev, g_rd.ffnout_dev, g_rd.norms_dev, g_rd.xb_dev),
        fixed_array(dim * 4l, dim * 4l, g_rd.norms_bytes, dim * 4l),
        fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB))
    g_rd.fused_sets_made = false
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def vk_rdec_set_moe_experts(l, router_slot, be1, be3, be2 : int64; fe1, fe3, fe2 : int) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null && g_rd.moe, "vk_rdec_set_moe_experts before prepare_moe")
    unsafe {
        var L & = g_rd.layers[l]
        assert(L.made, "vk_rdec_set_moe_experts before the layer's own registration")
        L.moe = true
        L.router_slot = router_slot
        L.be1 = be1; L.be3 = be3; L.be2 = be2
        L.fe1 = fe1; L.fe3 = fe3; L.fe2 = fe2
        rd_moe_sets(L)
    }
    g_rd.fused_sets_made = false
    g_rd.tok_recorded = false
}

//! the dense triple is the shared expert's; b1 < 0 = the layer has none.
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def vk_rdec_set_moe_layer(l, mir_idx, router_slot, bq, bk, bv, bo, b1, b3, b2, be1, be3, be2 : int64; fq, fk, fv, fo, f1, f3, f2, fe1, fe3, fe2 : int; g : RdecLayerGeom) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null && g_rd.moe, "vk_rdec_set_moe_layer before prepare_moe")
    rd_layer_slot(l)
    unsafe {
        var L & = g_rd.layers[l]
        rd_layer_geom(L, g)
        rd_attn_sets(L, mir_idx, bq, bk, bv, bo, fq, fk, fv, fo, f1, f3, f2)
        if (b1 >= 0l) {
            rd_ffn_sets(L, b1, b3, b2, f1, f3, f2)
        } else {
            rd_tail_sets_bare(L, f1, f3, f2)
        }
        L.made = true
    }
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
    cmd_copy_range(raw, g_gpu.dn_smalls.buf, 0l, L.dn.smalls_dev, 0l, DN_SM_HIST * 4l)
    vk_check(vkEndCommandBuffer(raw), null)
    submit_wait(g_gpu.batch_cmd)
}

def private rdn_state_upload(var st : DnStep; stp : float const?; histp : float const?) {
    unsafe {
        memcpy(g_gpu.dn_state_stage.mapped, reinterpret<void?>(stp), st.state_bytes)
        memcpy(reinterpret<void?>(reinterpret<float?>(g_gpu.dn_smalls.mapped) + DN_SM_HIST), reinterpret<void?>(histp), st.ring_bytes)
    }
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    cmd_copy_whole(raw, g_gpu.dn_state_stage.buf, st.state_dev, st.state_bytes)
    cmd_copy_range(raw, g_gpu.dn_smalls.buf, DN_SM_HIST * 4l, st.smalls_dev,
        DN_SM_HIST * 4l + int64(g_rd.dn_parity) * st.ring_bytes, st.ring_bytes)
    vk_check(vkEndCommandBuffer(raw), null)
    submit_wait(g_gpu.batch_cmd)
    st.parity = g_rd.dn_parity
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
    unsafe {
        var L & = g_rd.layers[l]
        L.recurrent = true
        L.mir_idx = -1l
        L.hid = g_rd.hidden   //! a recurrent layer's dense triple takes the model's width; its attention fields stay unused
        L.out_scale = 1.0     //! ... and its residual step is unscaled
        L.bqkv = bqkv; L.bz = bz; L.bout = bout; L.bbeta = bbeta; L.balpha = balpha
        L.fqkv = fqkv; L.fz = fz; L.fout = fout
        L.ba_off = ba_off
        let dim = g_rd.dim
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
        L.dn = DnStep(state_dev = make_device_buf(nvh * ds * ds * 4l), smalls_dev = make_device_buf(DND_SMALLS_BYTES),
                      state_bytes = nvh * ds * ds * 4l, ring_bytes = cd * (g_rd.dn_dconv - 1l) * 4l,
                      parity = 0, valid = false, dirty = false, owner_state_addr = 0ul, owner_hist_addr = 0ul)
        rdn_upload_constants(L, convw, wnormp, ap, dtp)
        let pqkv = arena_planes(fqkv, bqkv)
        let pz = arena_planes(fz, bz)
        let pout = arena_planes(fout, bout)
        // qkv + z GEMVs into one projection row: planes @0/1, regions@2, xq@3, xs@4, out@5
        L.s_dnqkv = gemv_cls_set(fqkv, fixed_array(pqkv.wq, pqkv.ws, L.m_dnqkv.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.dn_proj_dev),
            fixed_array(pqkv.wqb, pqkv.wsb, META_BYTES, dim, (dim / 32l) * 4l, (cd + di) * 4l),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_DNP))
        L.s_dnz = gemv_cls_set(fz, fixed_array(pz.wq, pz.ws, L.m_dnz.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.dn_proj_dev),
            fixed_array(pz.wqb, pz.wsb, META_BYTES, dim, (dim / 32l) * 4l, (cd + di) * 4l),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_DNZ))   //! the z half of the row under its own class: the qkv and z GEMVs co-run, the step waits on both
        if (g_rd.dn_ba_f32) {
            L.s_dnba = set_router_gemv_f16_cls(fixed_array(g_rd.dn_ba_dev, g_rd.xb_dev, L.dn.smalls_dev),
                fixed_array(g_rd.dn_ba_bytes, dim * 4l, DND_SMALLS_BYTES),
                fixed_array(0u, VHZ_XB, VHZ_DNSM))
        } else {
            L.m_dnbeta = rd_meta(); L.m_dnalpha = rd_meta()
            let pb = arena_planes(int(KqFmt.q8), bbeta)
            let pa = arena_planes(int(KqFmt.q8), balpha)
            L.s_dnbeta = gemv_cls_set(int(KqFmt.q8), fixed_array(pb.wq, pb.ws, L.m_dnbeta.buf, g_rd.xq_dev, g_rd.xs_dev, L.dn.smalls_dev),
                fixed_array(pb.wqb, pb.wsb, META_BYTES, dim, (dim / 32l) * 4l, DND_SMALLS_BYTES),
                fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_DNSM))
            L.s_dnalpha = gemv_cls_set(int(KqFmt.q8), fixed_array(pa.wq, pa.ws, L.m_dnalpha.buf, g_rd.xq_dev, g_rd.xs_dev, L.dn.smalls_dev),
                fixed_array(pa.wqb, pa.wsb, META_BYTES, dim, (dim / 32l) * 4l, DND_SMALLS_BYTES),
                fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_DNSM))
        }
        // the fused step: proj @0, state @1 (in place), smalls @2 (rw), o scales @3, o quants @4, tok @5, o f32 @6
        L.s_dnstep = set_dn_step_cls(
            fixed_array(g_rd.dn_proj_dev, L.dn.state_dev, L.dn.smalls_dev, g_rd.dn_os_dev, g_rd.dn_oq_dev, g_rd.tok_meta.buf, g_rd.dn_of_dev),
            fixed_array((cd + di) * 4l, L.dn.state_bytes, DND_SMALLS_BYTES, DND_OS_BYTES, DND_OQ_BYTES, 16l, DND_OQ_BYTES * 4l),
            fixed_array(VHZ_DNP | VHZ_DNZ, VHZ_DNST, VHZ_DNSM, VHZ_DNO, VHZ_DNO, 0u, VHZ_DNO))
        if (kq_sb(fout)) {
            L.s_quant_dno = set_rq_cls(fixed_array(g_rd.dn_of_dev, g_rd.dn_oq_dev, g_rd.dn_os_dev),
                fixed_array(DND_OQ_BYTES * 4l, DND_OQ_BYTES, DND_OS_BYTES),
                fixed_array(VHZ_DNO, VHZ_DNO, VHZ_DNO))
        }
        L.s_dnout = gemv_cls_set(fout, fixed_array(pout.wq, pout.ws, L.m_dnout.buf, g_rd.dn_oq_dev, g_rd.dn_os_dev, g_rd.xb2_dev),
            fixed_array(pout.wqb, pout.wsb, META_BYTES, di, (di / 32l) * 4l, dim * 4l),
            fixed_array(0u, 0u, 0u, VHZ_DNO, VHZ_DNO, VHZ_XB2))
        if (b1 >= 0l) {
            rd_ffn_sets(L, b1, b3, b2, f1, f3, f2)
        } else {
            rd_tail_sets_bare(L, f1, f3, f2)   //! a recurrent MoE layer without a shared expert
        }
        L.made = true
    }
    g_rd.tok_recorded = false
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
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

//! Bind a recurrent layer's device slot to the calling session's state (`stp`) and conv history (`histp`) before a token
[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#dn-step-owner"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def vk_rdec_dn_own(l : int64; stp : float?; histp : float?) {
    var L & = unsafe(g_rd.layers[l])
    let caller = intptr(stp)
    if (L.dn.valid && L.dn.owner_state_addr == caller) {
        return
    }
    if (L.dn.valid && L.dn.dirty) {
        dnd_flush_down(L.dn)
    }
    rdn_state_upload(L.dn, stp, histp)
}

//! The per-layer-embedding seats: the side-input row buffers at the model's ple width (after prepare, before the layers)
def vk_rdec_set_ple(ple : int64) : bool {
    if (g_gpu != null && g_gpu.dry) {
        return true
    }
    assert(g_rd != null, "vk_rdec_set_ple before prepare")
    if (ple <= 0l || ple % 32l != 0l) {
        return false   //! the gate GEMV's output and the act's requant take 32-blocks
    }
    g_rd.ple = ple
    let row_bytes = g_rd.n_layers * ple * 4l
    g_rd.ple_host = make_host_buf(row_bytes, false, [cached = true])
    g_rd.ple_dev = make_device_buf(row_bytes)
    g_rd.pleg_dev = make_device_buf(ple * 4l)
    g_rd.tok_recorded = false
    return true
}

//! One layer's per-layer-embedding branch: the gate [dim x ple] and proj [ple x dim] planes and the sets of its four steps
def vk_rdec_set_ple_layer(l, bgate, bproj : int64; fmt : int) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null && g_rd.ple > 0l, "vk_rdec_set_ple_layer before set_ple")
    rd_layer_slot(l)
    let dim = g_rd.dim
    let ple = g_rd.ple
    unsafe {
        var L & = g_rd.layers[l]
        L.ple = true
        L.bpg = bgate; L.bpp = bproj; L.fpg = fmt
        let pg = arena_planes(fmt, bgate)
        let pp = arena_planes(fmt, bproj)
        verify(gemv_cls_ensure(fmt) && ensure_q8_actrq_cls() && ensure_cls_dn_rq(), "vk decode: the per-layer-embedding rails must engage on a live device")
        L.m_pg = rd_meta(); L.m_pp = rd_meta()
        // the residual row as it is -> xq/xs (the gate reads the unnormed stream)
        L.s_ple_x = set_rq_cls(fixed_array(g_rd.x_dev, g_rd.xq_dev, g_rd.xs_dev),
            fixed_array(dim * 4l, dim, (dim / 32l) * 4l),
            fixed_array(VHZ_X, VHZ_XQ, VHZ_XQ))
        // gate GEMV [dim -> ple] into pleg (the FFN gate plane's hazard bit, free by then)
        L.s_ple_gate = gemv_cls_set(fmt, fixed_array(pg.wq, pg.ws, L.m_pg.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.pleg_dev),
            fixed_array(pg.wqb, pg.wsb, META_BYTES, dim, (dim / 32l) * 4l, ple * 4l),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_GATE))
        // gelu(gate) x this layer's side-input row -> hq/hs (the row's layer base rides the push; the side rows ride the cos plane's bit)
        L.s_ple_act = set_actrq_cls(fixed_array(g_rd.pleg_dev, g_rd.ple_dev, g_gpu.hq_dev, g_gpu.hs_dev),
            fixed_array(ple * 4l, g_rd.n_layers * ple * 4l, BATCH_HQ_BYTES, BATCH_HS_BYTES),
            fixed_array(VHZ_GATE, VHZ_COS, VHZ_HQ, VHZ_HQ))
        // proj GEMV [ple -> dim] into ffnout (the residual step's add partner slot)
        L.s_ple_proj = gemv_cls_set(fmt, fixed_array(pp.wq, pp.ws, L.m_pp.buf, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.ffnout_dev),
            fixed_array(pp.wqb, pp.wsb, META_BYTES, ple, (ple / 32l) * 4l, dim * 4l),
            fixed_array(0u, 0u, 0u, VHZ_HQ, VHZ_HQ, VHZ_FFO))
    }
    g_rd.tok_recorded = false
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
    g_rd.cls_cap = cap
    g_rd.nsupp = long_length(supp)
    unsafe {
        verify(ensure_cls_ar() && ensure_cls_ar_rq() && ensure_cls_dn_rq() && gemv_cls_ensure(cls_fmt)
            && (!kq_sb(cls_fmt) || ensure_cls_q8k_rq())
            && (!rd_cls_epi_on() || ensure_cls_epi()),
            "vk decode: the class rails must engage on a live device")
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
                fixed_array(vocab * 4l, supp_bytes), fixed_array(VHZ_LOG, 0u))
        }
        // prologue (class): xb = rms_att[0](x), add off — the aa slot binds x itself (add_on=0 never reads it)
        g_rd.s_prologue = set_cls_ar(fixed_array(g_rd.x_dev, g_rd.x_dev, g_rd.norms_dev, g_rd.xb_dev),
            fixed_array(dim * 4l, dim * 4l, g_rd.norms_bytes, dim * 4l),
            fixed_array(VHZ_X, VHZ_X, 0u, VHZ_XB))
        g_rd.fused_sets_made = false
        // quantize final xb
        g_rd.s_quant_final = set_rq_cls(fixed_array(g_rd.xb_dev, g_rd.xq_dev, g_rd.xs_dev),
            fixed_array(dim * 4l, dim, (dim / 32l) * 4l),
            fixed_array(VHZ_XB, VHZ_XQ, VHZ_XQ))
        // cls GEMV (class): a device one-pair regions buffer replaces the device-filled meta
        // (device for the same reason — every vocab-row subgroup reads it)
        g_rd.cls_regions_dev = make_device_buf(CLS_META_BYTES)
        var mp = reinterpret<uint?>(g_gpu.staging.mapped)
        mp[0] = uint(arena_local_blk(cls_block))
        mp[1] = 0u
        staged_upload(g_rd.cls_regions_dev, 0l, CLS_META_BYTES)
        let pc = arena_planes(cls_fmt, cls_block)
        g_rd.cls_set = gemv_cls_set(cls_fmt, fixed_array(pc.wq, pc.ws, g_rd.cls_regions_dev, g_rd.xq_dev, g_rd.xs_dev, g_rd.logits_dev),
            fixed_array(pc.wqb, pc.wsb, CLS_META_BYTES, dim, (dim / 32l) * 4l, vocab * 4l),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_LOG))
        g_rd.cls_fmt_marker = cls_fmt
        g_rd.tok_recorded = false   // the recorded chain references the replaced cls/prologue sets
        g_rd.ready = true
    }
}

//! the classifier epilogue has work: a final softcap or suppressed ids
def rd_cls_epi_on : bool => g_rd.cls_cap > 0.0 || g_rd.nsupp > 0l

//! the epilogue over the logits row the classifier GEMV just wrote (the prefill's tail encodes the same)
def rd_enc_cls_epi(raw : VkCommandBuffer; var h : VkHaz) {
    var pce = ClsEpiArgs(vocab = uint(g_rd.vocab), cap = g_rd.cls_cap, nsupp = uint(g_rd.nsupp))
    enc_cls_epi(raw, h, g_rd.cls_epi_set, pce, (g_rd.vocab + 255l) / 256l)
}

// Record the whole-token cmd ONCE per set_layer/set_cls epoch: constant metas + scalars fill here;
// per-token state is only x/cos (in-cmd copies) and the rope/attn position words (host-visible)
[cold_path]   // once per epoch (set replacement re-records); the rerecord A/B leg is opt-in
def private rd_record_token {
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
    rd_encode_token()
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private rq_stamp(kfmt : int; storex : bool) : RqStamp {
    if (kq_sb(kfmt)) {
        return storex ? RqStamp.rqkx : RqStamp.rqk
    }
    return storex ? RqStamp.rqx : RqStamp.rq
}

//! a fused site's set: x, its add partner (the FFN site's xb2, the next site's ffnout), the norms, the quant feed and - on a row-storing stamp - xb
def private set_ar_rq_stamp(stamp : RqStamp; add : uint64; aclass : uint; xq, xs : uint64) : VkDescriptorSet {
    let dim = g_rd.dim
    let nb = g_rd.norms_bytes
    if (stamp == RqStamp.rq) {
        return set_cls_ar_rq(fixed_array(g_rd.x_dev, add, g_rd.norms_dev, xq, xs),
            fixed_array(dim * 4l, dim * 4l, nb, dim, (dim / 32l) * 4l), fixed_array(VHZ_X, aclass, 0u, VHZ_XQ, VHZ_XQ))
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

def private enc_ar_rq_stamp(stamp : RqStamp; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : ArArgs) {
    if (stamp == RqStamp.rq) {
        enc_cls_ar_rq(raw, h, s, pc, 1l)
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
            if (L.moe) {   //! a MoE layer's next site is the combine, which carries its own norm and store
                continue
            }
            if (l + 1l < nl) {
                let fn = rd_head_feed(g_rd.layers[l + 1l])
                L.rq_next_stamp = rq_stamp(fn.fmt, fn.storex)
            } else {
                L.rq_next_stamp = rq_stamp(g_rd.cls_fmt_marker, false)
            }
            used[int(L.rq_next_stamp)] = true
        }
    }
    verify((!used[int(RqStamp.rq)] || ensure_cls_ar_rq()) && (!used[int(RqStamp.rqx)] || ensure_cls_ar_rqx())
        && (!used[int(RqStamp.rqk)] || ensure_cls_ar_rqk()) && (!used[int(RqStamp.rqkx)] || ensure_cls_ar_rqkx()),
        "vk decode: the fused add+rms+requant rails must engage on a live device")
    unsafe {
        g_rd.s_prologue_rq = set_ar_rq_stamp(g_rd.rq_pro_stamp, g_rd.x_dev, VHZ_X, g_rd.xq_dev, g_rd.xs_dev)   // add off: the aa slot binds x itself
        for (l in range64(nl)) {
            var L & = g_rd.layers[l]
            L.s_addr_ffn_rq = set_ar_rq_stamp(L.rq_ffn_stamp, g_rd.xb2_dev, VHZ_XB2, g_rd.xq_dev, g_rd.xs_dev)
            if (!L.moe) {
                L.s_addr_next_rq = set_ar_rq_stamp(L.rq_next_stamp, g_rd.ffnout_dev, VHZ_FFO, g_rd.xq_dev, g_rd.xs_dev)
            }
        }
    }
    g_rd.fused_sets_made = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: fused add+rms+requant on every decode site (DASLLAMA_VK_FUSE=0 pins the split dispatches)\n")
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private rd_encode_attn_head(raw : VkCommandBuffer; var h : VkHaz; var L : RLayer; l : int64; fuse_qkr : bool; nlfin : int64) {
    let dim = g_rd.dim
    let qd = L.qd
    let kvd = L.kvd
    let qrows = g_rd.q_gated ? 2l * qd : qd
    let qstride = uint(g_rd.q_gated ? 2l * L.hs : L.hs)
    var pc_rq_qd = RqArgs(inbase = 0u, nblk = uint(qd / 32l))
    var pc_rq_qd_k = RqArgs(inbase = 0u, nblk = uint(qd / 256l))
    let rqwg_qd = (qd / 4l + 255l) / 256l
    let rqwg_qd_k = (qd / 8l + 255l) / 256l
    var pcq = GemvArgs(n = uint(dim), d = uint(qrows), nreg = 1u, ybase = 0u)
    gemv_enc(L.fq, raw, h, L.s_q, pcq, qrows)
    rd_ts(raw, "a:q")
    if (L.kv_shared) {   //! a shared-KV layer projects Q only: the donor layer's rows already sit in the mirror
    } elif (L.kv_merged) {
        var pckv = GemvArgs(n = uint(dim), d = uint(kvd), nreg = 2u, ybase = uint(qd))
        gemv_enc(L.fk, raw, h, L.s_kv, pckv, 2l * kvd)
        rd_ts(raw, "a:kv")
    } else {
        var pck = GemvArgs(n = uint(dim), d = uint(kvd), nreg = 1u, ybase = uint(qd))
        gemv_enc(L.fk, raw, h, L.s_k, pck, kvd)
        rd_ts(raw, "a:kv")
        if (!L.vfk) {   //! a V-from-K layer has no v plane: the rope store takes V off the k row
            var pcv = GemvArgs(n = uint(dim), d = uint(kvd), nreg = 1u, ybase = uint(qd + kvd))
            gemv_enc(L.fv, raw, h, L.s_v, pcv, kvd)
            rd_ts(raw, "a:kv")   //! the split v stamp folds into the kv slot
        }
    }
    let hs = L.hs
    let hsmax = g_rd.head_size   // nolint:LINT021 — the q/k norm rows sit at the widest class's stride; the row offset multiplies run 64-bit before the push-field narrow
    let mirbase = uint(L.mir_base)
    if (fuse_qkr) {
        var pcf = QknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(L.rot / 2l),
            neox = g_rd.neox ? 1u : 0u, nh = uint(g_rd.n_heads), layerbase = mirbase,
            qwoff = uint(nlfin + dim + l * 2l * hsmax), kwoff = uint(nlfin + dim + (l * 2l + 1l) * hsmax),
            eps = g_rd.eps, qstride = qstride, boff = rd_bias_off(L), hasb = g_rd.has_bias ? 1u : 0u, cbase = uint(L.cbase),
            vfk = L.vfk ? 1u : 0u, vnorm = L.vnorm ? 1u : 0u)
        let nkh = L.kv_shared ? 0l : kvd / hs   //! the k-head groups also store the mirror row: none on a shared-KV layer
        if (g_rd.kv16) {
            enc_qkn_rope_f16_cls(raw, h, L.s_qkn_rope, pcf, g_rd.n_heads + nkh)
        } else {
            enc_qkn_rope_cls(raw, h, L.s_qkn_rope, pcf, g_rd.n_heads + nkh)
        }
        rd_ts(raw, "a:qknrope")
    } else {
        let nkh = L.kv_shared ? 0l : kvd / hs
        if (g_rd.qk_norm) {
            var pcn = QkRmsArgs(hs = uint(hs), nh = uint(g_rd.n_heads), nkvh = uint(nkh),
                qstride = uint(qd), kstride = uint(kvd), kbase = uint(qd),
                qwoff = uint(nlfin + dim + l * 2l * hsmax), kwoff = uint(nlfin + dim + (l * 2l + 1l) * hsmax),
                eps = g_rd.eps, qhs = uint(hs))
            enc_qk_rms_cls(raw, h, L.s_qkn, pcn, g_rd.n_heads + nkh)
            rd_ts(raw, "a:qkn")
        }
        let npairs = qd / 2l + (L.kv_shared ? 0l : kvd / 2l)   //! the k pairs also store the mirror row: none on a shared-KV layer
        var pcr = RopeKvArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(hs / 2l),
            neox = g_rd.neox ? 1u : 0u, kpair0 = uint(qd / 2l), npairs = uint(npairs),
            layerbase = mirbase, boff = rd_bias_off(L), hasb = g_rd.has_bias ? 1u : 0u, cbase = uint(L.cbase))
        if (g_rd.kv16) {
            enc_rope_kv_f16_cls(raw, h, L.s_rope, pcr, (npairs + int64(WG_X) - 1l) / int64(WG_X))
        } else {
            enc_rope_kv_cls(raw, h, L.s_rope, pcr, (npairs + int64(WG_X) - 1l) / int64(WG_X))
        }
        rd_ts(raw, "a:rope")
    }
    let nsplit = g_rd.attn_nsplit
    var pca = DaAttnArgs(kvd = uint(kvd), hs = uint(hs), kv_mul = uint(L.kv_mul),
        kbase = mirbase, vbase = mirbase, scale = g_rd.scale, qstride = qstride, gated = g_rd.q_gated ? 1u : 0u,
        window = uint(L.window), cap = L.attn_cap, nsplit = uint(nsplit))
    if (g_rd.kv16) {
        enc_da_attn_f16_cls(raw, h, L.s_attn, pca, g_rd.n_heads * nsplit)
    } else {
        enc_da_attn_cls(raw, h, L.s_attn, pca, g_rd.n_heads * nsplit)
    }
    rd_ts(raw, "a:attn")
    if (nsplit > 1l) {
        enc_da_attn_comb_cls(raw, h, L.s_attn_comb, pca, g_rd.n_heads)
        rd_ts(raw, "a:attc")
    }
    if (kq_sb(L.fo)) {
        enc_cls_q8k_rq(raw, h, L.s_quant_at, pc_rq_qd_k, rqwg_qd_k)
    } else {
        enc_cls_dn_rq(raw, h, L.s_quant_at, pc_rq_qd, rqwg_qd)
    }
    rd_ts(raw, "a:rq_a")
    var pcw = GemvArgs(n = uint(qd), d = uint(dim), nreg = 1u, ybase = 0u)
    gemv_enc(L.fo, raw, h, L.s_wo, pcw, dim)
    rd_ts(raw, "a:wo")
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private rd_encode_dn_head(raw : VkCommandBuffer; var h : VkHaz; var L : RLayer) {
    let dim = g_rd.dim
    let cd = g_rd.dn_cd
    let di = g_rd.dn_di
    let nvh = g_rd.dn_nvh
    var pcqkv = GemvArgs(n = uint(dim), d = uint(cd), nreg = 1u, ybase = 0u)
    gemv_enc(L.fqkv, raw, h, L.s_dnqkv, pcqkv, cd)
    rd_ts(raw, "d:qkv")
    var pcz = GemvArgs(n = uint(dim), d = uint(di), nreg = 1u, ybase = uint(cd))
    gemv_enc(L.fz, raw, h, L.s_dnz, pcz, di)
    rd_ts(raw, "d:z")
    if (g_rd.dn_ba_f32) {
        var pcb = RouterArgs(ne = uint(2l * nvh), dim = uint(dim), woff = uint(L.ba_off), obase = uint(DN_SM_BETA),
            ne_split = uint(nvh), obase_split = uint(DN_SM_G))
        enc_router_gemv_f16_cls(raw, h, L.s_dnba, pcb, 2l * nvh)
    } else {
        if (kq_sb(L.fqkv)) {   // the row above was requantized Q8_K for the kq GEMVs; the q8 beta/alpha GEMVs read it as Q8_0 (no stamp: the requant bills to the ba role)
            var pcr = RqArgs(inbase = 0u, nblk = uint(dim / 32l))
            enc_cls_dn_rq(raw, h, L.s_quant_xb, pcr, (dim / 4l + 255l) / 256l)
        }
        var pcb = GemvArgs(n = uint(dim), d = uint(nvh), nreg = 1u, ybase = uint(DN_SM_BETA))
        gemv_enc(int(KqFmt.q8), raw, h, L.s_dnbeta, pcb, nvh)
        var pcg = GemvArgs(n = uint(dim), d = uint(nvh), nreg = 1u, ybase = uint(DN_SM_G))
        gemv_enc(int(KqFmt.q8), raw, h, L.s_dnalpha, pcg, nvh)
    }
    rd_ts(raw, "d:ba")
    var pcd = DnStepArgs(cd = uint(cd), kd = uint(g_rd.dn_kd), nkh = uint(g_rd.dn_nkh), ds = uint(g_rd.dn_ds),
        dconv = uint(g_rd.dn_dconv), beta_off = uint(DN_SM_BETA), g_off = uint(DN_SM_G),
        taps_off = uint(DN_SM_TAPS), wnorm_off = uint(DN_SM_WNORM), a_off = uint(DN_SM_A),
        dt_off = uint(DN_SM_DT), hist_off = uint(DN_SM_HIST),
        eps = g_rd.eps, qscale = 1.0 / sqrt(float(g_rd.dn_ds)), out_f32 = kq_sb(L.fout) ? 1u : 0u)
    enc_dn_step_cls(raw, h, L.s_dnstep, pcd, nvh)
    rd_ts(raw, "d:step")
    if (kq_sb(L.fout)) {   // no stamp here - the requant bills to the out role
        var pcr = RqArgs(inbase = 0u, nblk = uint(di / 256l))
        enc_cls_q8k_rq(raw, h, L.s_quant_dno, pcr, (di / 8l + 255l) / 256l)
    }
    var pco = GemvArgs(n = uint(di), d = uint(dim), nreg = 1u, ybase = 0u)
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
    return (fuse = fuse, fuse_qkr = g_rd.qk_norm && (fuse || g_rd.q_gated || need))
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

def private rd_encode_ffn(raw : VkCommandBuffer; var h : VkHaz; var L : RLayer) {
    let dim = g_rd.dim
    let hid = L.hid
    let pfx = rd_ffn_pfx(L)
    let gelu = L.gelu ? 1u : 0u
    var pc_act = ActArgs(nelem = uint(hid), gelu = gelu, nblk = uint(hid / 32l))
    var pc_act_k = ActArgs(nelem = uint(hid), gelu = gelu, nblk = uint(hid / 256l))
    var pcg = GemvArgs(n = uint(dim), d = uint(hid), nreg = 1u, ybase = 0u)
    gemv_enc(L.f1, raw, h, L.s_gate, pcg, hid)
    rd_ts(raw, "{pfx}gate")
    gemv_enc(L.f3, raw, h, L.s_up, pcg, hid)
    rd_ts(raw, "{pfx}up")
    if (kq_sb(L.f2)) {
        enc_q8k_actrq_cls(raw, h, L.s_actrq, pc_act_k, (hid / 8l + 255l) / 256l)
    } else {
        enc_q8_actrq_cls(raw, h, L.s_actrq, pc_act, (hid / 4l + 255l) / 256l)
    }
    rd_ts(raw, L.moe ? "m:sh_act" : "{pfx}actrq")
    var pcd = GemvArgs(n = uint(hid), d = uint(dim), nreg = 1u, ybase = 0u)
    gemv_enc(L.f2, raw, h, L.s_down, pcd, dim)
    rd_ts(raw, "{pfx}down")
}

//! The per-layer-embedding branch after the FFN residual: gelu(gate . x) x side[l] -> proj -> ffnout (the next-norm step adds it under the ple post norm)
def private rd_encode_ple(raw : VkCommandBuffer; var h : VkHaz; var L : RLayer; l : int64) {
    let dim = g_rd.dim
    let ple = g_rd.ple
    var pcx = RqArgs(inbase = 0u, nblk = uint(dim / 32l))
    enc_cls_dn_rq(raw, h, L.s_ple_x, pcx, (dim / 4l + 255l) / 256l)
    rd_ts(raw, "p:rq_x")
    var pcg = GemvArgs(n = uint(dim), d = uint(ple), nreg = 1u, ybase = 0u)
    gemv_enc(L.fpg, raw, h, L.s_ple_gate, pcg, ple)
    rd_ts(raw, "p:gate")
    var pca = ActArgs(nelem = uint(ple), gelu = 1u, nblk = uint(ple / 32l), ubase = uint(l * ple))
    enc_q8_actrq_cls(raw, h, L.s_ple_act, pca, (ple / 4l + 255l) / 256l)
    rd_ts(raw, "p:act")
    var pcp = GemvArgs(n = uint(ple), d = uint(dim), nreg = 1u, ybase = 0u)
    gemv_enc(L.fpg, raw, h, L.s_ple_proj, pcp, dim)
    rd_ts(raw, "p:proj")
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def private rd_encode_moe(raw : VkCommandBuffer; var h : VkHaz; var L : RLayer; l : int64;
                          var pc_rq_dim : RqArgs; var pc_rq_dim_k : RqArgs; rqwg_dim, rqwg_dim_k : int64) {
    let dim = g_rd.dim
    let nfe = g_rd.nfe
    let k = g_rd.k
    if (L.moe_g4) {   //! the routed feed: x under the routed branch's pre norm into xb (the dense triple's requant took the FFN-normed row already)
        var pcn = ArArgs(dim = uint(dim), add_on = 0u, woff = uint(rd_norm_off(l, RD_NORM_PRE_FFN2)), eps = g_rd.eps, ascale = 1.0)
        enc_cls_ar(raw, h, L.s_addr_ffn, pcn, 1l)
        rd_ts(raw, "m:pre2")
    }
    if (L.xe_own) {
        if (kq_sb(L.fe1)) {
            enc_cls_q8k_rq(raw, h, L.s_quant_xe, pc_rq_dim_k, rqwg_dim_k)
        } else {
            enc_cls_dn_rq(raw, h, L.s_quant_xe, pc_rq_dim, rqwg_dim)
        }
        rd_ts(raw, "m:rq_e")
    }
    if (L.moe_g4) {   //! the router's input: a weightless rms of x under the router scale over sqrt(dim), into xb
        var pcn = ArArgs(dim = uint(dim), add_on = 0u, woff = uint(rd_norm_off(l, RD_NORM_ROUTER)), eps = g_rd.eps, ascale = 1.0)
        enc_cls_ar(raw, h, L.s_addr_ffn, pcn, 1l)
        rd_ts(raw, "m:rtn")
    }
    var pcr = RouterArgs(ne = uint(g_rd.ner), dim = uint(dim), woff = uint(L.router_slot * g_rd.ner * dim), obase = 0u)
    enc_router_gemv_cls(raw, h, L.s_router, pcr, g_rd.ner)
    rd_ts(raw, "m:router")
    var pct = TopkArgs(ne = uint(g_rd.ne), k = uint(k), norm = g_rd.norm_topk ? 1u : 0u, wscale = g_rd.wscale,
        dsoff = L.moe_g4 ? uint(L.router_slot * g_rd.ne) : NO_DSCALE,
        base1 = uint(arena_local_blk(L.be1)), stride1 = uint(nfe * dim / fmt_unit(L.fe1)),
        base3 = uint(arena_local_blk(L.be3)), stride3 = uint(nfe * dim / fmt_unit(L.fe3)),
        base2 = uint(arena_local_blk(L.be2)), stride2 = uint(nfe * dim / fmt_unit(L.fe2)),
        xnb2 = uint(nfe / fmt_unit(L.fe2)))
    enc_topk_cls(raw, h, L.s_topk, pct, 1l)
    rd_ts(raw, "m:topk")
    var pcg = GemvArgs(n = uint(dim), d = uint(nfe), nreg = uint(k), ybase = 0u)
    gemv_enc(L.fe1, raw, h, L.s_egate, pcg, k * nfe)
    rd_ts(raw, "m:e_gate")
    gemv_enc(L.fe3, raw, h, L.s_eup, pcg, k * nfe)
    rd_ts(raw, "m:e_up")
    let dkq = kq_sb(L.fe2)
    var pca = ActArgs(nelem = uint(k * nfe), gelu = g_rd.is_gelu ? 1u : 0u, nblk = uint(k * nfe / (dkq ? 256l : 32l)))
    let awgs = (k * nfe / (dkq ? 8l : 4l) + 255l) / 256l
    if (dkq) {
        enc_q8k_actrq_cls(raw, h, L.s_eact, pca, awgs)
    } else {
        enc_q8_actrq_cls(raw, h, L.s_eact, pca, awgs)
    }
    rd_ts(raw, "m:e_act")
    var pcd = GemvArgs(n = uint(nfe), d = uint(dim), nreg = uint(k), ybase = 0u)
    gemv_enc(L.fe2, raw, h, L.s_edown, pcd, k * dim)
    rd_ts(raw, "m:e_down")
}

//! The residual step's push block over a MoE layer's combine: the shared row at its gate when the layer has
//! one, the k routed rows through the slot map, then the norm at `woff`.
def rd_ar_comb_args(L : RLayer; l, woff : int64) : ArArgs {
    if (L.moe_g4) {   //! both branches normed, their sum normed, the step scaled: the gemma-4 form's rows
        return ArArgs(dim = uint(g_rd.dim), add_on = 1u, woff = uint(woff), eps = g_rd.eps, ascale = L.out_scale,
            slots = uint(g_rd.k), pwoff = uint(rd_norm_off(l, RD_NORM_POST_FFN1)),
            p2off = uint(rd_norm_off(l, RD_NORM_POST_FFN2)), p3off = uint(rd_norm_off(l, RD_NORM_POST_FFN)))
    }
    return ArArgs(dim = uint(g_rd.dim), add_on = has_dense_triple(L) ? 1u : 0u, woff = uint(woff), eps = g_rd.eps, ascale = 1.0,
        slots = uint(g_rd.k), lstride = uint(g_rd.ner), gated = g_rd.sh_gated ? 1u : 0u)
}

// The encode half alone: reset + re-encode the token cmd over the already-filled metas/sets.
// The --rerecord-ab probe calls this per token to price the class twins' push-borne pos/cnt.
[cold_path]   // recording leg (also the rerecord A/B probe) — not the steady-state token path
def private rd_encode_token {
    let dim = g_rd.dim
    let nlfin = rd_norm_final()   // rms_final offset in the norms buffer
    unsafe {
        var raw = g_rd.cmd
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
        cmd_copy_whole(raw, g_rd.x_host.buf, g_rd.x_dev, dim * 4l)
        vhz_dep(raw, h, 0u, VHZ_COS, true)
        cmd_copy_whole(raw, g_rd.cos_host.buf, g_rd.cos_dev, g_rd.cos_elems * 4l)
        if (g_rd.ple > 0l) {   //! the side-input row rides the cos plane's hazard bit: written here, read by the act step
            cmd_copy_whole(raw, g_rd.ple_host.buf, g_rd.ple_dev, g_rd.n_layers * g_rd.ple * 4l)
        }
        let (fuse, fuse_qkr) = rd_fuse_gates()
        if (fuse) {
            rd_ensure_fused_sets()
        }
        var pc_rq_dim = RqArgs(inbase = 0u, nblk = uint(dim / 32l))
        var pc_rq_dim_k = RqArgs(inbase = 0u, nblk = uint(dim / 256l))
        let rqwg_dim = (dim / 4l + 255l) / 256l
        let rqwg_dim_k = (dim / 8l + 255l) / 256l
        var pc_pro = ArArgs(dim = uint(dim), add_on = 0u, woff = 0u, eps = g_rd.eps, ascale = 1.0)
        if (fuse) {
            enc_ar_rq_stamp(g_rd.rq_pro_stamp, raw, h, g_rd.s_prologue_rq, pc_pro)   // rms_att[0](x) -> xq/xs (and xb where layer 0's head reads it)
        } else {
            enc_cls_ar(raw, h, g_rd.s_prologue, pc_pro, 1l)   // xb = rms_att[0](x)
        }
        rd_ts(raw, "p:prologue")
        for (l in range64(g_rd.n_layers)) {
            var L & = g_rd.layers[l]
            let hpfx = rd_head_pfx(L)
            let tpfx = rd_tail_pfx(L)
            let after_moe_combine = l > 0l && g_rd.layers[l - 1l].moe
            if (!fuse || after_moe_combine) {
                if (kq_sb(L.recurrent ? L.fqkv : L.fq)) {
                    enc_cls_q8k_rq(raw, h, L.s_quant_xb, pc_rq_dim_k, rqwg_dim_k)
                } else {
                    enc_cls_dn_rq(raw, h, L.s_quant_xb, pc_rq_dim, rqwg_dim)
                }
                rd_ts(raw, "{hpfx}rq_x")
            }
            if (L.recurrent) {
                rd_encode_dn_head(raw, h, L)
            } else {
                rd_encode_attn_head(raw, h, L, l, fuse_qkr, nlfin)
            }
            var pc8 = ArArgs(dim = uint(dim), add_on = 1u, woff = uint(rd_norm_off(l, RD_NORM_FFN)), eps = g_rd.eps, ascale = 1.0,
                pre_on = L.pre_post ? 1u : 0u, pwoff = uint(rd_norm_off(l, RD_NORM_POST_ATT)))
            if (fuse) {
                enc_ar_rq_stamp(L.rq_ffn_stamp, raw, h, L.s_addr_ffn_rq, pc8)
                rd_ts(raw, "{tpfx}arrq_f")
            } else {
                enc_cls_ar(raw, h, L.s_addr_ffn, pc8, 1l)
                rd_ts(raw, "{tpfx}ar1")
                if (kq_sb(L.f1)) {
                    enc_cls_q8k_rq(raw, h, L.s_quant_xb2, pc_rq_dim_k, rqwg_dim_k)
                } else {
                    enc_cls_dn_rq(raw, h, L.s_quant_xb2, pc_rq_dim, rqwg_dim)
                }
                rd_ts(raw, "{tpfx}rq_f")
            }
            if (has_dense_triple(L)) {
                rd_encode_ffn(raw, h, L)
            }
            if (L.moe) {
                rd_encode_moe(raw, h, L, l, pc_rq_dim, pc_rq_dim_k, rqwg_dim, rqwg_dim_k)
            }
            let nxt = l + 1l < g_rd.n_layers ? rd_norm_off(l + 1l, RD_NORM_ATT) : nlfin   //! rms_att[l+1] | final
            if (L.ple) {
                // the FFN residual first (no scale, the normed output unused); the branch leaves its projection in ffnout for the step below
                var pc14a = ArArgs(dim = uint(dim), add_on = 1u, woff = uint(nxt), eps = g_rd.eps, ascale = 1.0,
                    pre_on = L.pre_post ? 1u : 0u, pwoff = uint(rd_norm_off(l, RD_NORM_POST_FFN)))
                enc_cls_ar(raw, h, L.s_addr_next, pc14a, 1l)
                rd_ts(raw, "{tpfx}ar2a")
                rd_encode_ple(raw, h, L, l)
            }
            var pc14 = ArArgs(dim = uint(dim), add_on = 1u, woff = uint(nxt), eps = g_rd.eps, ascale = L.out_scale,
                pre_on = (L.pre_post || L.ple) ? 1u : 0u, pwoff = uint(rd_norm_off(l, L.ple ? RD_NORM_PLE_POST : RD_NORM_POST_FFN)))
            if (L.moe) {
                var pc14c = rd_ar_comb_args(L, l, nxt)
                if (L.moe_g4) {
                    enc_cls_ar_comb_g4(raw, h, L.s_addr_next, pc14c, 1l)
                } else {
                    enc_cls_ar_comb(raw, h, L.s_addr_next, pc14c, 1l)
                }
                rd_ts(raw, "m:ar2")
            } elif (fuse) {
                enc_ar_rq_stamp(L.rq_next_stamp, raw, h, L.s_addr_next_rq, pc14)
                rd_ts(raw, "{tpfx}arrq_n")
            } else {
                enc_cls_ar(raw, h, L.s_addr_next, pc14, 1l)
                rd_ts(raw, "{tpfx}ar2")
            }
        }
        if (!fuse || g_rd.layers[g_rd.n_layers - 1l].moe) {   //! the fused next site did the final norm and its requant
            if (kq_sb(g_rd.cls_fmt_marker)) {
                enc_cls_q8k_rq(raw, h, g_rd.s_quant_final, pc_rq_dim_k, rqwg_dim_k)
            } else {
                enc_cls_dn_rq(raw, h, g_rd.s_quant_final, pc_rq_dim, rqwg_dim)
            }
            rd_ts(raw, "t:fin_rq")
        }
        var pcc = GemvArgs(n = uint(dim), d = uint(g_rd.vocab), nreg = 1u, ybase = 0u)
        gemv_enc(g_rd.cls_fmt_marker, raw, h, g_rd.cls_set, pcc, g_rd.vocab)
        rd_ts(raw, "t:cls")
        if (rd_cls_epi_on()) {
            rd_enc_cls_epi(raw, h)
            rd_ts(raw, "t:epi")
        }
        vhz_dep(raw, h, VHZ_LOG, 0u, true)   // compute -> transfer for the logits DMA
        cmd_copy_whole(raw, g_rd.logits_dev, g_rd.logits_host.buf, g_rd.vocab * 4l)
        pfq_end(raw)
        vk_check(vkEndCommandBuffer(raw), null)
    }
    g_rdq_n = g_pfq_n   // decode's stamp count (prefill re-arms the shared pool per window)
    g_rd.tok_recorded = true
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

[cold_path]   // DASLLAMA_GPU_PROF reporting leg
def private rdq_sample {
    let want = length(g_rdq_stamp_names)
    if (g_rdq_overflow || g_rdq_n != uint(want)) {
        // a bare stamp or a full query pool: the whole command's GPU span reads off the first and last stamps
        unsafe {
            var tp = reinterpret<uint64?>(g_pfq_buf.mapped)
            g_rdq_total += double(tp[g_rdq_n - 1u] - tp[0u]) * double(g_pfq_period) / 1000.0lf
        }
        if (g_rdq_count == 0l) {   // the first token: the eight longest stamp gaps, by stamp index, name the dispatches worth a look
            var top <- [for (i in range(1, int(g_rdq_n))); (us = pfq_us(uint(i)), at = uint(i))]
            top |> sort() $(a, b) => a.us > b.us
            let line = build_string() $(w) {
                for (k in range(min(8, length(top)))) {
                    w |> write("#{top[k].at} {int(top[k].us)}us ")
                }
            }
            to_log(LOG_INFO, "vk_rdec first token: {g_rdq_n} stamps, longest gaps {line}\n")
            delete top
        }
        g_rdq_count++
        if (g_rdq_count % 32l == 0l) {
            let why = g_rdq_overflow ? "the query pool filled at PFQ_CAP" : "{g_rdq_n} stamps, the recorder named {want}"
            to_log(LOG_INFO, "vk_rdec gpu avg/token over {g_rdq_count}: total {int(g_rdq_total / double(g_rdq_count))} us (roles unaggregated: {why})\n")
        }
        return
    }
    for (i in range(1, want)) {   // stamp 0 is the anchor
        let nm = g_rdq_stamp_names[i]
        if (!key_exists(g_rdq_us_by_name, nm)) {
            g_rdq_print_order |> push(nm)
            g_rdq_us_by_name[nm] = 0.0lf
        }
        g_rdq_us_by_name[nm] += pfq_us(uint(i))
    }
    unsafe {
        var tp = reinterpret<uint64?>(g_pfq_buf.mapped)
        g_rdq_total += double(tp[g_rdq_n - 1u] - tp[0u]) * double(g_pfq_period) / 1000.0lf
    }
    g_rdq_count++
    if (g_rdq_count % 32l == 0l) {
        let n = double(g_rdq_count)
        var ndn = 0l
        var nmoe = 0l
        for (L in g_rd.layers) {
            ndn += L.recurrent ? 1l : 0l
            nmoe += L.moe ? 1l : 0l
        }
        let natt = long_length(g_rd.layers) - ndn
        to_log(LOG_INFO, "vk_rdec gpu avg/token over {g_rdq_count}: {rdq_group_line("p:", n)}{rdq_group_line("a:", n)}{rdq_group_line("t:", n)}total {int(g_rdq_total / n)} (attention layers {natt}, recurrent {ndn}, MoE {nmoe})\n")
        if (nmoe > 0l) {
            to_log(LOG_INFO, "vk_rdec moe avg/token (the MoE layers' tails, summed over the layers): {rdq_group_line("m:", n)}\n")
        }
        if (ndn > 0l) {
            to_log(LOG_INFO, "vk_rdec dn avg/token: {rdq_group_line("d:", n)}\n")
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

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]   // the first four tokens and every 256th
def private rd_logits_guard(logits : array<float>) {
    for (v in logits) {
        return if (v != 0.0)
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

//! Resident-driver token: run the whole stack on device from the embedded residual `x` and this
//! position's rope row (cos[half]+sin[half]) -> logits. `cnt` = cached positions incl. this one; K/V
//! for `pos` store into the mirror. Recorded once — a step is two memcpys + position words + one submit.
[hot_path]   // the per-token resident submit — no goo between the memcpys and the fence
def vk_rdec_token(x : array<float>; cossin : array<float>; ple_row : array<float>; pos, cnt : int64; var logits : array<float>) {
    assert(g_rd != null && g_rd.ready, "vk_rdec_token before prepare/set_cls")
    if (!g_rd.tok_recorded) {
        rd_record_token()
    } elif (vk_rdec_rerecord_on()) {
        rd_encode_token()   // probe arm: pay the full re-encode every token
    }
    let ts0 = ref_time_ticks()
    let prof = vk_prof()
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
        tm.parity = uint(g_rd.dn_parity)
        if (prof) {   //! the profiled form splits the wall into its pieces; the served form is one call
            let t1 = ref_time_ticks()
            submit_nowait(g_rd.cmd)
            let t2 = ref_time_ticks()
            wait_fence()
            let t3 = ref_time_ticks()
            memcpy(addr<void?>(logits[0]), g_rd.logits_host.mapped, g_rd.vocab * 4l)
            let t4 = ref_time_ticks()
            g_rdq_host_us[0] += double(get_time_usec(ts0)) - double(get_time_usec(t1))
            g_rdq_host_us[1] += double(get_time_usec(t1)) - double(get_time_usec(t2))
            g_rdq_host_us[2] += double(get_time_usec(t2)) - double(get_time_usec(t3))
            g_rdq_host_us[3] += double(get_time_usec(t3)) - double(get_time_usec(t4))
        } else {
            submit_wait(g_rd.cmd)
            memcpy(addr<void?>(logits[0]), g_rd.logits_host.mapped, g_rd.vocab * 4l)
        }
    }
    g_rd.tokens_served++
    if (g_rd.tokens_served <= 4l || (g_rd.tokens_served & 255l) == 0l) {
        rd_logits_guard(logits)
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
        // the steps wrote the other ring image: it is the current history now, and every slot is dirty
        g_rd.dn_parity = 1 - g_rd.dn_parity
        for (L in g_rd.layers) {
            if (L.recurrent) {
                L.dn.parity = g_rd.dn_parity
                L.dn.dirty = true
            }
        }
    }
    if (vk_prof() && g_rdq_n > 2u) {
        rdq_sample()
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
    let base = L.mir_base * g_rd.kv_esz
    unsafe {
        upload_region_at(g_rd.k_mirror, base, kp, npos * rowb)
        upload_region_at(g_rd.v_mirror, base, vp, npos * rowb)
    }
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
        let off = L.mir_base * g_rd.kv_esz + p0 * rowb
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
    let off = L.mir_base * g_rd.kv_esz + pos * rowb
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
                    state_bytes = nvh * ds * ds * 4l, ring_bytes = cd * taps * 4l,
                    parity = 0, tok <- make_host_buf(16l, true), valid = false, dirty = false,
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
        fixed_array(Y_BYTES, st.state_bytes, DND_SMALLS_BYTES, DND_OS_BYTES, DND_OQ_BYTES, 16l, DND_OQ_BYTES * 4l),
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
        memcpy(g_gpu.dn_state_stage.mapped, reinterpret<void?>(stp), st.state_bytes)
    }
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    cmd_copy_whole(raw, g_gpu.dn_smalls.buf, st.smalls_dev, DN_SMALLS_BYTES)
    cmd_copy_whole(raw, g_gpu.dn_state_stage.buf, st.state_dev, st.state_bytes)
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
    var st : DnStep?
    unsafe {
        st = g_gpu.dnd_steps?[woq]   // the make above made the key: a read, never an insert
    }
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
        if (L.dn.valid && L.dn.owner_state_addr == intptr(stp)) {
            if (L.dn.dirty) {
                dnd_flush_down(L.dn)
            }
            dnd_drop_slot(L.dn)
        }
        return
    }
    let woq = moe_gpu_dn_layer_key(l)
    if (woq < 0l || !key_exists(g_gpu.dnd_steps, woq)) {
        return
    }
    var st : DnStep?
    unsafe {
        st = g_gpu.dnd_steps?[woq]   // the key_exists above made sure of the key: a read, never an insert
    }
    let caller_state_addr = intptr(stp)
    if (st.owner_state_addr != caller_state_addr) {
        return
    }
    if (st.dirty) {
        dnd_flush_down(*st)
    }
    dnd_drop_slot(*st)
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
    cmd_copy_whole(raw, st.state_dev, g_gpu.dn_state_host.buf, st.state_bytes)
    cmd_copy_range(raw, st.smalls_dev, DN_SM_HIST * 4l + int64(st.parity) * st.ring_bytes,
        g_gpu.dn_tail_host.buf, 0l, st.ring_bytes)
    vk_check(vkEndCommandBuffer(raw), null)
    submit_wait(g_gpu.batch_cmd)
    unsafe {
        memcpy(reinterpret<void?>(st.owner_state_addr), g_gpu.dn_state_host.mapped, st.state_bytes)
        memcpy(reinterpret<void?>(st.owner_hist_addr), g_gpu.dn_tail_host.mapped, st.ring_bytes)
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
            if (L.recurrent) {
                dnd_drop_slot(L.dn)
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
            if (L.recurrent && L.dn.owner_state_addr >= state_lo && L.dn.owner_state_addr < state_hi) {
                dnd_drop_slot(L.dn)
            }
        }
    }
}

// ===== decode dispatch (the FFN's GEMV arm) =====

// lazy per-stack class-GEMV set (dies with the stack); role bits re-declare per chain via hz_set_bits
def private stack_gemv_cls_set(si : int) : VkDescriptorSet {
    unsafe {
        var st & = g_gpu.stacks[si]
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
}

// pre-record (or refresh on a row-count / activation-form change) the triple's chain: gate+up
// GEMV pair, fused act+requant, down GEMV, y DMA. Per token only the stacks' host-visible
// regions and the activation image change — the recorded buffer resubmits as-is.
[cold_path]   // creates the triple's sets once
def private ffn_cmd_key(s1, s3, s2 : int) : int64 {
    let key = (int64(s1) * int64(MAX_STACKS) + int64(s3)) * int64(MAX_STACKS) + int64(s2)
    if (!key_exists(g_gpu.ffn_cmds, key)) {
        var fc = FfnCmd(cmd = alloc_cmd(), nrows = 0l)
        let down_is_kq = !vk_fmt_b32(g_gpu.stacks[s2].fmt)
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
    let down_is_kq = !vk_fmt_b32(g_gpu.stacks[s2].fmt)
    var pca = ActArgs(nelem = uint(nrows * nfe), gelu = is_gelu ? 1u : 0u,
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
    let kq_in = !vk_fmt_b32(f1)
    assert(!vk_fmt_b32(f3) == kq_in,
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
    var pc = ffn_gemv_prep(offs1, offs3, offs2, nregions, xqp, xsp, n, nfe, nrows, f1, f3, f2, is_gelu)
    submit_wait(pc.cmd)
    unsafe {
        memcpy(reinterpret<void?>(goutp), g_gpu.stacks[pc.s2].y.mapped, nrows * n * 4l)
    }
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
    assert(g_gpu.tail_pending_woq < 0l, "dasLLAMA vulkan tier: decode ffn tail while one is pending")
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
    g_gpu.tail_pending_woq = woq_next
    g_gpu.tail_xout = xoutp
    if (vk_prof()) {
        g_dec_prof_ffn_us += int64(get_time_usec(ts0))
        g_dec_prof_ffn_n++
    }
}

//! MoeGpuFfnTailPendingFn: a tail is in flight, uncollected
def vk_moe_ffn_tail_pending : bool => g_gpu.tail_pending_woq >= 0l

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
    SL.s_router = set_router_gemv_cls(fixed_array(g_gpu.span_router_dev, g_gpu.span_xb_dev, g_gpu.span_logits_dev),
        fixed_array(nl * a.ne * nb, nb, MOE_MAX_EXPERTS * 4l), fixed_array(0u, VHS_XB, VHS_LOG))
    SL.s_topk = set_topk_cls(   //! the span folds no down scale: the logits row fills the ds binding, never read at NO_DSCALE
        fixed_array(g_gpu.span_logits_dev, g_gpu.stacks[SL.s1].meta.buf, g_gpu.stacks[SL.s3].meta.buf, g_gpu.stacks[SL.s2].meta.buf, g_gpu.span_w_dev, g_gpu.span_logits_dev),
        fixed_array(MOE_MAX_EXPERTS * 4l, META_BYTES, META_BYTES, META_BYTES, MAX_ROUTED_SLOTS * 4l, MOE_MAX_EXPERTS * 4l),
        fixed_array(VHS_LOG, VHS_META, VHS_META, VHS_META, VHS_META, VHS_LOG))
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
            SL.s_shgate = set_router_gemv_cls(fixed_array(g_gpu.span_shgate_dev, g_gpu.span_xb_dev, g_gpu.span_logits_dev),
                fixed_array(nl * nb, nb, SPAN_LOGITS_BYTES), fixed_array(0u, VHS_XB, VHS_LOG))
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
        var L & = unsafe(g_gpu.dat_layers[a.woq[li]])
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
    g_gpu.span_logits_dev = make_device_buf(SPAN_LOGITS_BYTES)
    g_gpu.span_router_dev = make_device_buf(nl * a.ne * dim * 4l)
    g_gpu.span_norms_dev = make_device_buf(nl * 2l * dim * 4l)
    if (a.nsh > 0l && a.sh_gated) {
        g_gpu.span_shgate_dev = make_device_buf(nl * dim * 4l)
        unsafe {
            upload_region_at(g_gpu.span_shgate_dev, 0l, reinterpret<uint8 const?>(a.shgate), nl * dim * 4l)
        }
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
        dat_layer_ensure(a.woq[li], a.kvd)
        dat_layer_sets(a.woq[li], a.wok[li], a.wov[li], a.woo[li], a.fq[li], a.fk[li], a.fv[li], a.fo[li], a.rmsq[li], a.rmsk[li],
            a.bq[li], a.bk[li], a.bv[li], a.qk_norm)
    }
    ensure_span_state(a)
    assert(g_gpu.tail_pending_woq < 0l, "dasLLAMA vulkan tier: decode span with a decode ffn tail pending")
    fill_stack_acts(g_gpu.span_layers[0].sq, a.xq, a.xs, a.dim)
    unsafe {
        memcpy(g_gpu.span_xin.mapped, reinterpret<void?>(a.x), a.dim * 4l)
        memcpy(g_gpu.dat_cos.mapped, reinterpret<void?>(a.cossin), a.hs * 4l)
        var tm = reinterpret<TokMeta?>(g_gpu.dat_tok.mapped)
        tm.pos = uint(a.pos)
        tm.cnt = uint(a.pos + 1l)
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
        var L : DatLayer?
        unsafe {
            L = g_gpu.dat_layers?[a.woq[li]]
        }
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
    if (src1 < 0 || src3 < 0 || src2 < 0 || vk_fmt_b32(f3) != vk_fmt_b32(f1) || want <= 0l
            || !arena_rows_ok(n, f1) || !arena_rows_ok(n, f3) || !arena_rows_ok(nfe, f2)) {
        let why = (want <= 0l ? "want 0"
            : (src1 < 0 || src3 < 0 || src2 < 0 ? "no mirror src {src1}/{src3}/{src2} for b {b1}/{b3}/{b2} fmt {f1}/{f3}/{f2}"
            : (vk_fmt_b32(f3) != vk_fmt_b32(f1) ? "gate/up form mismatch fmt {f1}/{f3}"
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
    let unit = fmt_unit(g_gpu.stacks[si].fmt)
    let rel = woff - g_gpu.stacks[si].base
    assert(rel % unit == 0l, "dasLLAMA vulkan tier: unaligned classifier plane offset")
    let fmt = g_gpu.stacks[si].fmt
    verify(gemv_cls_ensure(fmt), "vk decode: the gemv class rail must engage on a live device")
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.staging.mapped)
        mp[0] = uint(rel / unit)
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
    g_gpu.dat_tok = make_host_buf(16l, true)
    g_gpu.dat_out = make_host_buf(dim * 4l + 2l * kvd * 4l, false, [cached = true])   // wo row | raw k | raw v
    g_gpu.dat_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: decode attention block engaged (mirror cap {DAT_MIRROR_ROWS} rows)\n")
}

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block")]   // once per layer: the chain's sets
def private dat_layer_sets(woq, wok, wov, woo : int64; fq, fk, fv, fo : int; rmsqp : float const?; rmskp : float const?;
                           bqp : float const?; bkp : float const?; bvp : float const?; qk_norm : bool) {
    var L & = unsafe(g_gpu.dat_layers[woq])
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
            fixed_array(qd * 4l, (qd + 2l * kvd) * 4l, mirbytes, mirbytes, hs * 4l, 2l * hs * 4l, 16l, bbytes),
            fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, VHZ_MIR, VHZ_MIR, 0u, 0u, 0u, 0u))
    } else {
        L.s_qkr = set_rope_kv_f16_cls(
            fixed_array(g_gpu.dat_q_dev, g_gpu.dat_kv_dev, L.k_mir, L.v_mir, g_gpu.dat_cos.buf, g_gpu.dat_tok.buf, bbuf),
            fixed_array(qd * 4l, (qd + 2l * kvd) * 4l, mirbytes, mirbytes, hs * 4l, 16l, bbytes),
            fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, VHZ_MIR, VHZ_MIR, 0u, 0u, 0u))
    }
    L.s_attn = set_da_attn_f16_cls(   //! the unsplit pass: the out rows fill the partials binding, never read
        fixed_array(g_gpu.dat_q_dev, L.k_mir, L.v_mir, g_gpu.dat_attn_dev, g_gpu.dat_tok.buf, g_gpu.dat_attn_dev),
        fixed_array(qd * 4l, mirbytes, mirbytes, qd * 4l, 16l, qd * 4l),
        fixed_array(VHZ_Q, VHZ_MIR, VHZ_MIR, VHZ_ATT, 0u, VHZ_ATT))
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
def vk_moe_attn_dec_ok(woq, pos, gen : int64) : int {
    if (g_gpu == null || g_gpu.dry || pos >= DAT_MIRROR_ROWS) {
        return DAT_NEVER
    }
    if (gen == 0l || gen != g_dat_gen) {
        return DAT_STALE_GEN
    }
    if (!key_exists(g_gpu.dat_layers, woq) || unsafe(g_gpu.dat_layers[woq]).cnt != pos) {
        return DAT_HYDRATE
    }
    return DAT_SERVE
}

var private @scratch g_dat_h16 : array<float16>

//! MoeGpuAttnDecHydrateFn: the layer's mirror takes the host cache's rows [0, npos) (K roped, V
//! raw - the host cache's own forms; f16 rows copy through, f32 rows convert), then holds exactly
//! those positions. npos = 0 leaves it empty.
[cold_path]   // a remedy leg (a rewind, a CPU-prefilled prefix) - never the served token
def vk_moe_attn_dec_hydrate(woq : int64; kp : uint8 const?; vp : uint8 const?; npos, kvd : int64; f16 : bool) {
    dat_layer_ensure(woq, kvd)
    var L : DatLayer?
    unsafe {
        L = g_gpu.dat_layers?[woq]
    }
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
    var pca = DaAttnArgs(kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u, scale = scale)
    enc_da_attn_f16_cls(raw, h, L.s_attn, pca, n_heads)
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
    dat_layer_ensure(woq, kvd)
    dat_layer_sets(woq, wok, wov, woo, fq, fk, fv, fo, rmsqp, rmskp, bqp, bkp, bvp, qk_norm)
    var L : DatLayer?
    unsafe {
        L = g_gpu.dat_layers?[woq]   // the ensure above made the key: a read, never an insert
    }
    let sq = find_stack(woq, fq, "decode attn q")
    at_record_layer(*L, fq, fk, fv, fo, dim, qd, kvd, hs, n_heads, kv_mul, scale, eps, neox, qk_norm)
    let has_pending_tail = g_gpu.tail_pending_woq >= 0l
    if (has_pending_tail) {
        assert(g_gpu.tail_pending_woq == woq, "dasLLAMA vulkan tier: the pending decode ffn tail targets another layer")
    } else {
        fill_stack_acts(sq, xqp, xsp, dim)
    }
    unsafe {
        memcpy(g_gpu.dat_cos.mapped, reinterpret<void?>(cosp), hs * 4l)
        var tm = reinterpret<TokMeta?>(g_gpu.dat_tok.mapped)
        tm.pos = uint(pos)
        tm.cnt = uint(pos + 1l)
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
        g_gpu.tail_pending_woq = -1l
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
