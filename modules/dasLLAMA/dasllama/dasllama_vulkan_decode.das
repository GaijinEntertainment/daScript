options gen2
options _comment_hygiene = true

module dasllama_vulkan_decode shared public
options _cyclomatic_complexity = 0   // whole-token record/setup walks — split queued for the dedup phase
options _function_length = 0   // same

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
def vk_rdec_prepare(n_layers, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64;
                    neox : bool; eps, scale : float; qk_norm : bool; kv16 : bool) : bool {
    if (!vk_moe_init()) {
        return false
    }
    let esz = kv16 ? 2l : 4l
    // each mirror side binds as ONE SSBO range: past maxStorageBufferRange (or the shaders' uint
    // word index) every read is undefined. The ctx negotiation caps first; guard direct callers.
    if (n_layers * seq_cap * kv_dim * esz > vk_max_storage_range()
            || n_layers * seq_cap * kv_dim > 4_294_967_295l) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: KV mirror ({n_layers} x {seq_cap} x {kv_dim}) exceeds maxStorageBufferRange {vk_max_storage_range()} / the 32-bit word index\n")
        return false
    }
    // the residual-stream kernels stage a whole row in ar_row, and the per-head ones a whole head
    // in qr_row/da_q — over either slab they would write past the workgroup array with no diagnostic
    if (dim > AR_MAX_DIM || head_size > 256l) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: dim {dim} > {AR_MAX_DIM} or head_size {head_size} > 256 exceeds the workgroup row slabs\n")
        return false
    }
    if (g_gpu.dry) {
        return true   // guards above ARE the plan decision; everything below is device state
    }
    // The driver is all-or-nothing across prefill AND decode: when the device workgroup cap
    // declines prefill's batched attention (the largest footprint; ensure_ declines by name via
    // vkd_wg_fits), residency declines WITH it, pre-upload. Dry-plan gap: followup_vulkan.
    if (!(g_gpu.has_coopmat && head_size == 128l && vk_fa_on()
            ? (kv16 ? ensure_da_attn_b_h128_f16_cls() : ensure_da_attn_b_h128_cls())
            : (kv16 ? ensure_da_attn_b_f16_cls() : ensure_da_attn_b_cls()))) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: the batched-attention class rail declined on this device — resident driver declines, the per-op rails serve\n")
        return false
    }
    g_rd = new RDec(ready = false, n_layers = n_layers, dim = dim, qd = qd, kv_dim = kv_dim,
                 head_size = head_size, n_heads = n_heads, kv_mul = n_heads / (kv_dim / head_size),
                 hidden = hidden, vocab = vocab, seq_cap = seq_cap, neox = neox, eps = eps, scale = scale,
                 qk_norm = qk_norm, kv16 = kv16, kv_esz = esz)
    var r & = unsafe(*g_rd)
    let wide = max(max(qd, hidden), dim)
    r.x_dev = make_device_buf(dim * 4l)
    r.xb_dev = make_device_buf(dim * 4l)
    r.xq_dev = make_device_buf(wide)              // int8 quants, 1 byte/elem
    r.xs_dev = make_device_buf((wide / 32l) * 4l)
    r.q_dev = make_device_buf(qd * 4l)
    r.kv_dev = make_device_buf((qd + 2l * kv_dim) * 4l)
    r.attn_dev = make_device_buf(qd * 4l)
    r.aq_dev = make_device_buf(qd)
    r.as_dev = make_device_buf((qd / 32l) * 4l)
    r.gate_dev = make_device_buf(hidden * 4l)
    r.up_dev = make_device_buf(hidden * 4l)
    r.ffnout_dev = make_device_buf(dim * 4l)
    r.xb2_dev = make_device_buf(dim * 4l)
    r.k_mirror = make_device_buf(n_layers * seq_cap * kv_dim * esz)
    r.v_mirror = make_device_buf(n_layers * seq_cap * kv_dim * esz)
    // norms layout: [rms_att[l], rms_ffn[l]] x L + rms_final (dim each), then under qk_norm the
    // per-head rows [rms_q[l], rms_k[l]] x L (head_size each) appended past the fixed layout,
    // then the eps/ascale pair the fused ar+requant reads (its binding 4 is the scale output)
    r.ar_scal_off = n_layers * 2l * dim + dim + (qk_norm ? n_layers * 2l * head_size : 0l)
    r.norms_bytes = (r.ar_scal_off + 2l) * 4l
    r.norms_dev = make_device_buf(r.norms_bytes)
    // cos layout: this token's cos[half]+sin[half] (the qkn class twin reads its norm rows from
    // the norms buffer directly, so nothing else rides the cos plane)
    r.cos_elems = head_size
    r.cos_dev = make_device_buf(r.cos_elems * 4l)
    r.logits_dev = make_device_buf(vocab * 4l)
    r.dummy = make_device_buf(256l)
    r.x_host = make_host_buf(dim * 4l, false, [cached = true])
    r.cos_host = make_host_buf(head_size * 4l, false, [cached = true])
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

//! Register a dense layer's arena block indices + formats and build its descriptor sets. Called
//! once per layer after prepare. `w_ffn_off`/`w_next_off` are the rms_ffn[l] and rms_att[l+1]
//! (or rms_final for the last layer) offsets in the norms buffer (dim-element units).
def vk_rdec_set_layer(l : int64; bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null, "vk_rdec_set_layer before prepare")
    while (long_length(g_rd.layers) <= l) {
        g_rd.layers |> emplace(RLayer())
    }
    unsafe {
        var L & = g_rd.layers[l]
        L.bq = bq; L.bk = bk; L.bv = bv; L.bo = bo; L.b1 = b1; L.b3 = b3; L.b2 = b2
        L.fq = fq; L.fk = fk; L.fv = fv; L.fo = fo; L.f1 = f1; L.f3 = f3; L.f2 = f2
        let dim = g_rd.dim
        let qd = g_rd.qd
        let kvd = g_rd.kv_dim
        let hid = g_rd.hidden
        let pq = arena_planes(fq, bq)
        let pk = arena_planes(fk, bk)
        let pv = arena_planes(fv, bv)
        let po = arena_planes(fo, bo)
        let p1 = arena_planes(f1, b1)
        let p3 = arena_planes(f3, b3)
        let p2 = arena_planes(f2, b2)
        // every role rides the class rail: params in push, per-token words in the shared TokMeta;
        // kq consumers take the Q8_K quant/act forms (per-256 scales)
        let anykq = kq_sb(fq) || kq_sb(fo) || kq_sb(f1) || kq_sb(f2)
        let kv16 = g_rd.kv16
        verify(ensure_cls_dn_rq() && ensure_cls_ar() && ensure_cls_ar_rq() && ensure_q8_actrq_cls()
            && (kv16 ? ensure_rope_kv_f16_cls() : ensure_rope_kv_cls())
            && (kv16 ? ensure_da_attn_f16_cls() : ensure_da_attn_cls())
            && (!g_rd.qk_norm || (ensure_qk_rms_cls()
                && (kv16 ? ensure_qkn_rope_f16_cls() : ensure_qkn_rope_cls())))
            && (!anykq || (ensure_cls_q8k_rq() && ensure_q8k_actrq_cls()))
            && gemv_cls_ensure(fq) && gemv_cls_ensure(fk) && gemv_cls_ensure(fv) && gemv_cls_ensure(fo)
            && gemv_cls_ensure(f1) && gemv_cls_ensure(f3) && gemv_cls_ensure(f2),
            "vk decode: the class rails must engage on a live device")
        L.m_q = rd_meta(); L.m_k = rd_meta(); L.m_v = rd_meta(); L.m_wo = rd_meta()
        L.m_gate = rd_meta(); L.m_up = rd_meta(); L.m_down = rd_meta()
        // quantize xb -> xq/xs
        L.s_quant_xb = set_rq_cls(fixed_array(g_rd.xb_dev, g_rd.xq_dev, g_rd.xs_dev),
            fixed_array(dim * 4l, dim, (dim / 32l) * 4l),
            fixed_array(VHZ_XB, VHZ_XQ, VHZ_XQ))
        // q/k/v class GEMV: planes @0/1, regions@2, xq@3, xs@4, out@5
        L.s_q = gemv_cls_set(fq, fixed_array(pq.wq, pq.ws, L.m_q.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.q_dev),
            fixed_array(pq.wqb, pq.wsb, META_BYTES, dim, (dim / 32l) * 4l, qd * 4l),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_Q))
        L.s_k = gemv_cls_set(fk, fixed_array(pk.wq, pk.ws, L.m_k.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev),
            fixed_array(pk.wqb, pk.wsb, META_BYTES, dim, (dim / 32l) * 4l, (qd + 2l * kvd) * 4l),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVK))
        L.s_v = gemv_cls_set(fv, fixed_array(pv.wq, pv.ws, L.m_v.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev),
            fixed_array(pv.wqb, pv.wsb, META_BYTES, dim, (dim / 32l) * 4l, (qd + 2l * kvd) * 4l),
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVV))
        // same-format k/v in the SAME slab share arena planes — one two-region dispatch fills both
        // kv halves (a slab boundary between them falls back to the split sets)
        L.kv_merged = fk == fv && arena_slab_idx(bk) == arena_slab_idx(bv)
        if (L.kv_merged) {
            L.s_kv = gemv_cls_set(fk, fixed_array(pk.wq, pk.ws, L.m_k.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev),
                fixed_array(pk.wqb, pk.wsb, META_BYTES, dim, (dim / 32l) * 4l, (qd + 2l * kvd) * 4l),
                fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVK | VHZ_KVV))
        }
        let mirbytes = g_rd.n_layers * g_rd.seq_cap * kvd * g_rd.kv_esz
        if (g_rd.qk_norm) {
            // per-head q/k rmsnorm (class): q rows, k rows (kv_dev — kbase=qd rides the push), norms
            L.s_qkn = set_qk_rms_cls(fixed_array(g_rd.q_dev, g_rd.kv_dev, g_rd.norms_dev),
                fixed_array(qd * 4l, (qd + 2l * kvd) * 4l, g_rd.norms_bytes),
                fixed_array(VHZ_Q, VHZ_KVK, 0u))
            // the fused twin (class): norm rows bind the norms buffer, mirror row rides TokMeta
            let qrbufs = fixed_array(g_rd.q_dev, g_rd.kv_dev, g_rd.k_mirror, g_rd.v_mirror, g_rd.cos_dev, g_rd.norms_dev, g_rd.tok_meta.buf)
            let qrsizes = fixed_array(qd * 4l, (qd + 2l * kvd) * 4l, mirbytes, mirbytes, g_rd.head_size * 4l, g_rd.norms_bytes, 16l)
            let qrbits = fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, VHZ_MIR, VHZ_MIR, VHZ_COS, 0u, 0u)
            L.s_qkn_rope = (kv16 ? set_qkn_rope_f16_cls(qrbufs, qrsizes, qrbits)
                : set_qkn_rope_cls(qrbufs, qrsizes, qrbits))
        }
        // rope+store (class): per-token mirror row rides TokMeta
        let rbufs = fixed_array(g_rd.q_dev, g_rd.kv_dev, g_rd.k_mirror, g_rd.v_mirror, g_rd.cos_dev, g_rd.tok_meta.buf)
        let rsizes = fixed_array(qd * 4l, (qd + 2l * kvd) * 4l, mirbytes, mirbytes, g_rd.head_size * 4l, 16l)
        let rbits = fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, VHZ_MIR, VHZ_MIR, VHZ_COS, 0u)
        L.s_rope = kv16 ? set_rope_kv_f16_cls(rbufs, rsizes, rbits) : set_rope_kv_cls(rbufs, rsizes, rbits)
        // attn (class): per-token cnt rides TokMeta, the scale rides the push
        let abufs = fixed_array(g_rd.q_dev, g_rd.k_mirror, g_rd.v_mirror, g_rd.attn_dev, g_rd.tok_meta.buf)
        let asizes = fixed_array(qd * 4l, mirbytes, mirbytes, qd * 4l, 16l)
        let abits = fixed_array(VHZ_Q, VHZ_MIR, VHZ_MIR, VHZ_ATT, 0u)
        L.s_attn = kv16 ? set_da_attn_f16_cls(abufs, asizes, abits) : set_da_attn_cls(abufs, asizes, abits)
        // quantize attn -> aq/as
        L.s_quant_at = set_rq_cls(fixed_array(g_rd.attn_dev, g_rd.aq_dev, g_rd.as_dev),
            fixed_array(qd * 4l, qd, (qd / 32l) * 4l),
            fixed_array(VHZ_ATT, VHZ_AQ, VHZ_AQ))
        // wo GEMV -> xb2
        L.s_wo = gemv_cls_set(fo, fixed_array(po.wq, po.ws, L.m_wo.buf, g_rd.aq_dev, g_rd.as_dev, g_rd.xb2_dev),
            fixed_array(po.wqb, po.wsb, META_BYTES, qd, (qd / 32l) * 4l, dim * 4l),
            fixed_array(0u, 0u, 0u, VHZ_AQ, VHZ_AQ, VHZ_XB2))
        // addrms (class): x += xb2, norms, xb out — and the fused twin quantizing in place of xb
        L.s_addr_ffn = set_cls_ar(fixed_array(g_rd.x_dev, g_rd.xb2_dev, g_rd.norms_dev, g_rd.xb_dev),
            fixed_array(dim * 4l, dim * 4l, g_rd.norms_bytes, dim * 4l),
            fixed_array(VHZ_X, VHZ_XB2, 0u, VHZ_XB))
        L.s_addr_ffn_rq = set_cls_ar_rq(fixed_array(g_rd.x_dev, g_rd.xb2_dev, g_rd.norms_dev, g_rd.xq_dev, g_rd.xs_dev),
            fixed_array(dim * 4l, dim * 4l, g_rd.norms_bytes, dim, (dim / 32l) * 4l),
            fixed_array(VHZ_X, VHZ_XB2, 0u, VHZ_XQ, VHZ_XQ))
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
        // addrms next: x += ffnout, xb = rms_att[l+1]|final — the fused twin also serves the
        // final norm on the last layer, so the split rail's quant_final has no fused counterpart
        L.s_addr_next = set_cls_ar(fixed_array(g_rd.x_dev, g_rd.ffnout_dev, g_rd.norms_dev, g_rd.xb_dev),
            fixed_array(dim * 4l, dim * 4l, g_rd.norms_bytes, dim * 4l),
            fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB))
        L.s_addr_next_rq = set_cls_ar_rq(fixed_array(g_rd.x_dev, g_rd.ffnout_dev, g_rd.norms_dev, g_rd.xq_dev, g_rd.xs_dev),
            fixed_array(dim * 4l, dim * 4l, g_rd.norms_bytes, dim, (dim / 32l) * 4l),
            fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XQ, VHZ_XQ))
        L.made = true
    }
    g_rd.tok_recorded = false   // the recorded chain references the replaced sets
}

//! Register the classifier plane (arena block + format) and build the prologue / final-norm / cls
//! sets. Call after all layers are set.
def vk_rdec_set_cls(cls_block : int64; cls_fmt : int) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    assert(g_rd != null, "vk_rdec_set_cls before prepare")
    let dim = g_rd.dim
    let vocab = g_rd.vocab
    unsafe {
        verify(ensure_cls_ar() && ensure_cls_ar_rq() && ensure_cls_dn_rq() && gemv_cls_ensure(cls_fmt)
            && (!kq_sb(cls_fmt) || ensure_cls_q8k_rq()),
            "vk decode: the class rails must engage on a live device")
        // prologue (class): xb = rms_att[0](x), add off — the aa slot binds x itself (add_on=0 never reads it)
        g_rd.s_prologue = set_cls_ar(fixed_array(g_rd.x_dev, g_rd.x_dev, g_rd.norms_dev, g_rd.xb_dev),
            fixed_array(dim * 4l, dim * 4l, g_rd.norms_bytes, dim * 4l),
            fixed_array(VHZ_X, VHZ_X, 0u, VHZ_XB))
        // fused twin — subsumes layer 0's quant_xb
        g_rd.s_prologue_rq = set_cls_ar_rq(fixed_array(g_rd.x_dev, g_rd.x_dev, g_rd.norms_dev, g_rd.xq_dev, g_rd.xs_dev),
            fixed_array(dim * 4l, dim * 4l, g_rd.norms_bytes, dim, (dim / 32l) * 4l),
            fixed_array(VHZ_X, VHZ_X, 0u, VHZ_XQ, VHZ_XQ))
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

// Record the whole-token cmd ONCE per set_layer/set_cls epoch: constant metas + scalars fill here;
// per-token state is only x/cos (in-cmd copies) and the rope/attn position words (host-visible)
def private rd_record_token {
    for (l in range64(g_rd.n_layers)) {
        var L & = unsafe(g_rd.layers[l])
        fill_gemv_regions(L.m_q, L.bq)
        if (L.kv_merged) {
            fill_gemv_regions2(L.m_k, L.bk, L.bv)   // k, v regions land at qd, qd+kvd (push ybase)
        } else {
            fill_gemv_regions(L.m_k, L.bk)
            fill_gemv_regions(L.m_v, L.bv)
        }
        fill_gemv_regions(L.m_wo, L.bo)
        fill_gemv_regions(L.m_gate, L.b1)
        fill_gemv_regions(L.m_up, L.b3)
        fill_gemv_regions(L.m_down, L.b2)
    }
    rd_encode_token()
}

// The fused ar+rq twin emits Q8_0 only — true when every x-quant consumer (each layer's qkv and
// gate/up groups, plus the classifier) reads a Q8_0-form image, i.e. the fuse may serve them all.
def private rd_x_quants_b32 : bool {
    for (L in g_rd.layers) {
        if (kq_sb(L.fq) || kq_sb(L.f1)) {
            return false
        }
    }
    return !kq_sb(g_rd.cls_fmt_marker)
}

// The encode half alone: reset + re-encode the token cmd over the already-filled metas/sets.
// The --rerecord-ab probe calls this per token to price the class twins' push-borne pos/cnt.
def private rd_encode_token {
    let dim = g_rd.dim
    let qd = g_rd.qd
    let kvd = g_rd.kv_dim
    let hid = g_rd.hidden
    let nlfin = g_rd.n_layers * 2l * dim   // rms_final offset in the norms buffer (dim units later)
    unsafe {
        var raw = g_rd.cmd
        let rf : VkCommandBufferResetFlags
        vk_check(vkResetCommandBuffer(raw, rf), null)
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        pfq_begin(raw)
        pfq_ts(raw)   // t0 anchor — the recorded-once cmd re-stamps every token submit
        var h : VkHaz
        vhz_dep(raw, h, 0u, VHZ_X, true)
        cmd_copy_whole(raw, g_rd.x_host.buf, g_rd.x_dev, dim * 4l)
        vhz_dep(raw, h, 0u, VHZ_COS, true)
        cmd_copy_whole(raw, g_rd.cos_host.buf, g_rd.cos_dev, g_rd.head_size * 4l)
        // fused rail (Q8_0-emitting — a kq x-quant consumer forces the split rail via the gate)
        let fuse = vk_fuse_on() && rd_x_quants_b32()
        let fuse_qkr = fuse && g_rd.qk_norm
        let rpw = g_gpu.rows_per_wg
        var pc_rq_dim = RqArgs(inbase = 0u, nblk = uint(dim / 32l))
        var pc_rq_qd = RqArgs(inbase = 0u, nblk = uint(qd / 32l))
        var pc_rq_dim_k = RqArgs(inbase = 0u, nblk = uint(dim / 256l))
        var pc_rq_qd_k = RqArgs(inbase = 0u, nblk = uint(qd / 256l))
        var pc_act = ActArgs(nelem = uint(hid), gelu = 0u, nblk = uint(hid / 32l))
        var pc_act_k = ActArgs(nelem = uint(hid), gelu = 0u, nblk = uint(hid / 256l))
        let rqwg_dim = (dim / 4l + 255l) / 256l
        let rqwg_qd = (qd / 4l + 255l) / 256l
        let rqwg_dim_k = (dim / 8l + 255l) / 256l
        let rqwg_qd_k = (qd / 8l + 255l) / 256l
        var pc_pro = ArArgs(dim = uint(dim), add_on = 0u, woff = 0u, eps = g_rd.eps, ascale = 1.0)
        if (fuse) {
            enc_cls_ar_rq(raw, h, g_rd.s_prologue_rq, pc_pro, 1l)   // rms_att[0](x) -> xq/xs
        } else {
            enc_cls_ar(raw, h, g_rd.s_prologue, pc_pro, 1l)   // xb = rms_att[0](x)
        }
        pfq_ts(raw)
        for (l in range64(g_rd.n_layers)) {
            var L & = g_rd.layers[l]
            if (!fuse) {
                if (kq_sb(L.fq)) {
                    enc_cls_q8k_rq(raw, h, L.s_quant_xb, pc_rq_dim_k, rqwg_dim_k)
                } else {
                    enc_cls_dn_rq(raw, h, L.s_quant_xb, pc_rq_dim, rqwg_dim)
                }
                pfq_ts(raw)
            }
            var pcq = GemvArgs(n = uint(dim), d = uint(qd), nreg = 1u, ybase = 0u)
            gemv_cls_enc(L.fq, raw, h, L.s_q, pcq, (qd + rpw - 1l) / rpw)
            pfq_ts(raw)
            if (L.kv_merged) {
                var pckv = GemvArgs(n = uint(dim), d = uint(kvd), nreg = 2u, ybase = uint(qd))
                gemv_cls_enc(L.fk, raw, h, L.s_kv, pckv, (2l * kvd + rpw - 1l) / rpw)
                pfq_ts(raw)
            } else {
                var pck = GemvArgs(n = uint(dim), d = uint(kvd), nreg = 1u, ybase = uint(qd))
                gemv_cls_enc(L.fk, raw, h, L.s_k, pck, (kvd + rpw - 1l) / rpw)
                pfq_ts(raw)
                var pcv = GemvArgs(n = uint(dim), d = uint(kvd), nreg = 1u, ybase = uint(qd + kvd))
                gemv_cls_enc(L.fv, raw, h, L.s_v, pcv, (kvd + rpw - 1l) / rpw)
                pfq_ts(raw)
            }
            let hs = g_rd.head_size
            let mirbase = uint(l * g_rd.seq_cap * kvd)
            if (fuse_qkr) {
                var pcf = QknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(hs / 2l),
                    neox = g_rd.neox ? 1u : 0u, nh = uint(g_rd.n_heads), layerbase = mirbase,
                    qwoff = uint(nlfin + dim + l * 2l * hs), kwoff = uint(nlfin + dim + (l * 2l + 1l) * hs),
                    eps = g_rd.eps)
                if (g_rd.kv16) {
                    enc_qkn_rope_f16_cls(raw, h, L.s_qkn_rope, pcf, g_rd.n_heads + kvd / hs)
                } else {
                    enc_qkn_rope_cls(raw, h, L.s_qkn_rope, pcf, g_rd.n_heads + kvd / hs)
                }
                pfq_ts(raw)
            } else {
                if (g_rd.qk_norm) {
                    var pcn = QkRmsArgs(hs = uint(hs), nh = uint(g_rd.n_heads), nkvh = uint(kvd / hs),
                        qstride = uint(qd), kstride = uint(kvd), kbase = uint(qd),
                        qwoff = uint(nlfin + dim + l * 2l * hs), kwoff = uint(nlfin + dim + (l * 2l + 1l) * hs),
                        eps = g_rd.eps)
                    enc_qk_rms_cls(raw, h, L.s_qkn, pcn, g_rd.n_heads + kvd / hs)
                    pfq_ts(raw)
                }
                var pcr = RopeKvArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(hs / 2l),
                    neox = g_rd.neox ? 1u : 0u, kpair0 = uint(qd / 2l), npairs = uint(qd / 2l + kvd / 2l),
                    layerbase = mirbase)
                if (g_rd.kv16) {
                    enc_rope_kv_f16_cls(raw, h, L.s_rope, pcr, (qd / 2l + kvd / 2l + int64(WG_X) - 1l) / int64(WG_X))
                } else {
                    enc_rope_kv_cls(raw, h, L.s_rope, pcr, (qd / 2l + kvd / 2l + int64(WG_X) - 1l) / int64(WG_X))
                }
                pfq_ts(raw)
            }
            var pca = DaAttnArgs(kvd = uint(kvd), hs = uint(hs), kv_mul = uint(g_rd.kv_mul),
                kbase = mirbase, vbase = mirbase, scale = g_rd.scale)
            if (g_rd.kv16) {
                enc_da_attn_f16_cls(raw, h, L.s_attn, pca, g_rd.n_heads)
            } else {
                enc_da_attn_cls(raw, h, L.s_attn, pca, g_rd.n_heads)
            }
            pfq_ts(raw)
            if (kq_sb(L.fo)) {
                enc_cls_q8k_rq(raw, h, L.s_quant_at, pc_rq_qd_k, rqwg_qd_k)
            } else {
                enc_cls_dn_rq(raw, h, L.s_quant_at, pc_rq_qd, rqwg_qd)
            }
            pfq_ts(raw)
            var pcw = GemvArgs(n = uint(qd), d = uint(dim), nreg = 1u, ybase = 0u)
            gemv_cls_enc(L.fo, raw, h, L.s_wo, pcw, (dim + rpw - 1l) / rpw)
            pfq_ts(raw)
            var pc8 = ArArgs(dim = uint(dim), add_on = 1u, woff = uint((l * 2l + 1l) * dim), eps = g_rd.eps, ascale = 1.0)
            if (fuse) {
                enc_cls_ar_rq(raw, h, L.s_addr_ffn_rq, pc8, 1l)
                pfq_ts(raw)
            } else {
                enc_cls_ar(raw, h, L.s_addr_ffn, pc8, 1l)
                pfq_ts(raw)
                if (kq_sb(L.f1)) {
                    enc_cls_q8k_rq(raw, h, L.s_quant_xb2, pc_rq_dim_k, rqwg_dim_k)
                } else {
                    enc_cls_dn_rq(raw, h, L.s_quant_xb2, pc_rq_dim, rqwg_dim)
                }
                pfq_ts(raw)
            }
            var pcg = GemvArgs(n = uint(dim), d = uint(hid), nreg = 1u, ybase = 0u)
            gemv_cls_enc(L.f1, raw, h, L.s_gate, pcg, (hid + rpw - 1l) / rpw)
            pfq_ts(raw)
            gemv_cls_enc(L.f3, raw, h, L.s_up, pcg, (hid + rpw - 1l) / rpw)
            pfq_ts(raw)
            if (kq_sb(L.f2)) {
                enc_q8k_actrq_cls(raw, h, L.s_actrq, pc_act_k, (hid / 8l + 255l) / 256l)
            } else {
                enc_q8_actrq_cls(raw, h, L.s_actrq, pc_act, (hid / 4l + 255l) / 256l)
            }
            pfq_ts(raw)
            var pcd = GemvArgs(n = uint(hid), d = uint(dim), nreg = 1u, ybase = 0u)
            gemv_cls_enc(L.f2, raw, h, L.s_down, pcd, (dim + rpw - 1l) / rpw)
            pfq_ts(raw)
            let nxt = l + 1l < g_rd.n_layers ? (l + 1l) * 2l * dim : nlfin   // nolint:LINT021 — rms_att[l+1]|final, composed from int64 layout offsets
            var pc14 = ArArgs(dim = uint(dim), add_on = 1u, woff = uint(nxt), eps = g_rd.eps, ascale = 1.0)
            if (fuse) {
                enc_cls_ar_rq(raw, h, L.s_addr_next_rq, pc14, 1l)
            } else {
                enc_cls_ar(raw, h, L.s_addr_next, pc14, 1l)
            }
            pfq_ts(raw)
        }
        if (!fuse) {
            if (kq_sb(g_rd.cls_fmt_marker)) {
                enc_cls_q8k_rq(raw, h, g_rd.s_quant_final, pc_rq_dim_k, rqwg_dim_k)
            } else {
                enc_cls_dn_rq(raw, h, g_rd.s_quant_final, pc_rq_dim, rqwg_dim)
            }
            pfq_ts(raw)
        }
        var pcc = GemvArgs(n = uint(dim), d = uint(g_rd.vocab), nreg = 1u, ybase = 0u)
        gemv_cls_enc(g_rd.cls_fmt_marker, raw, h, g_rd.cls_set, pcc, (g_rd.vocab + rpw - 1l) / rpw)
        pfq_ts(raw)
        vhz_dep(raw, h, VHZ_LOG, 0u, true)   // compute -> transfer for the logits DMA
        cmd_copy_whole(raw, g_rd.logits_dev, g_rd.logits_host.buf, g_rd.vocab * 4l)
        pfq_end(raw)
        vk_check(vkEndCommandBuffer(raw), null)
    }
    g_rdq_n = g_pfq_n   // decode's stamp count (prefill re-arms the shared pool per window)
    g_rd.tok_recorded = true
}

// The per-stamp role names, in the order rdq_sample aggregates them (see the kv/rq_x notes below).
def private rdq_role_names(fuse, fuse_qkr, qk_norm : bool) : array<string> {
    return <- ["q", "kv", fuse_qkr ? "qknrope" : "rope", "attn", "rq_a", "wo", "arrq_f", "gate", "up", "actrq", "down", "arrq_n"] if (fuse)
    return <- ["rq_x", "q", "kv", "qkn", "rope", "attn", "rq_a", "wo", "ar1", "rq_f", "gate", "up", "actrq", "down", "ar2"] if (qk_norm)
    return <- ["rq_x", "q", "kv", "rope", "attn", "rq_a", "wo", "ar1", "rq_f", "gate", "up", "actrq", "down", "ar2"]
}

def private rdq_sample {
    // the fuse gate must MATCH rd_encode_token's or the aggregation walks the wrong shape
    let fuse = vk_fuse_on() && rd_x_quants_b32()
    let fuse_qkr = fuse && g_rd.qk_norm
    var want = fuse ? 3 : 4   // anchor + prologue + [fin_rq — split rail only] + cls
    // fused is 13 either way: merging qkn into rope exactly offsets keeping a separate rope
    let base_rpl = fuse ? 13 : (g_rd.qk_norm ? 16 : 15)
    for (L in g_rd.layers) {
        want += base_rpl - (L.kv_merged ? 1 : 0)
    }
    if (g_rdq_n != uint(want)) {
        return   // deep graphs overflow PFQ_CAP — no aggregation
    }
    g_rdq_pro += pfq_us(1u)
    // k+v aggregate into ONE "kv" slot (split = 2 stamps); fused drops rq_x, so the kv slot shifts down
    let nroles = base_rpl - 1
    let kv_at = fuse ? 1 : 2
    var qi = 2u
    for (L in g_rd.layers) {
        var s = 0
        while (s < nroles) {
            g_rdq_role[s] += pfq_us(qi)
            qi++
            if (s == kv_at && !L.kv_merged) {
                g_rdq_role[s] += pfq_us(qi)   // the split v stamp folds into the kv slot
                qi++
            }
            s++
        }
    }
    // split rail: fin_rq + cls; fused: cls alone (the last addr_next already did the final norm)
    g_rdq_tail += (fuse ? 0.0lf : pfq_us(g_rdq_n - 2u)) + pfq_us(g_rdq_n - 1u)
    unsafe {
        var tp = reinterpret<uint64?>(g_pfq_buf.mapped)
        g_rdq_total += double(tp[g_rdq_n - 1u] - tp[0u]) * double(g_pfq_period) / 1000.0lf
    }
    g_rdq_count++
    if (g_rdq_count % 32l == 0l) {
        let n = double(g_rdq_count)
        var names <- rdq_role_names(fuse, fuse_qkr, g_rd.qk_norm)
        let rs = build_string() $(w) {
            for (s in range(length(names))) {
                w |> write("{names[s]} {int(g_rdq_role[s] / n)} ")
            }
        }
        delete names
        to_log(LOG_INFO, "vk_rdec gpu avg/token over {g_rdq_count}: prologue {int(g_rdq_pro / n)} {rs}tail {int(g_rdq_tail / n)} total {int(g_rdq_total / n)}\n")
    }
}

//! Resident-driver token: run the whole stack on device from the embedded residual `x` and this
//! position's rope row (cos[half]+sin[half]) -> logits. `cnt` = cached positions incl. this one; K/V
//! for `pos` store into the mirror. Recorded once — a step is two memcpys + position words + one submit.
def vk_rdec_token(x : array<float>; cossin : array<float>; pos, cnt : int64; var logits : array<float>) {
    assert(g_rd != null && g_rd.ready, "vk_rdec_token before prepare/set_cls")
    if (!g_rd.tok_recorded) {
        rd_record_token()
    } elif (vk_rdec_rerecord_on()) {
        rd_encode_token()   // probe arm: pay the full re-encode every token
    }
    unsafe {
        memcpy(g_rd.x_host.mapped, addr<void?>(x[0]), g_rd.dim * 4l)
        memcpy(g_rd.cos_host.mapped, addr<void?>(cossin[0]), g_rd.head_size * 4l)
        var tm = reinterpret<TokMeta?>(g_rd.tok_meta.mapped)
        tm.pos = uint(pos)   // every carrier composes its mirror row as layerbase + pos * kvd
        tm.cnt = uint(cnt)
        submit_wait(g_rd.cmd)
        memcpy(addr<void?>(logits[0]), g_rd.logits_host.mapped, g_rd.vocab * 4l)
    }
    if (vk_prof() && g_rdq_n > 2u) {
        rdq_sample()
    }
}

//! Batch-decode support: upload one layer's KV history ([npos x kvd] each, the mirror codec's
//! rows) into the shared mirror at layer `l`, positions [0, npos). Resyncs a session's cache
//! before its decode step — the session codec matches the mirror codec (the arm gate), so the
//! rows byte-copy.
def vk_rdec_sync_kv(l : int64; kp : uint8 const?; vp : uint8 const?; npos : int64) {
    assert(g_rd != null, "vk_rdec_sync_kv before prepare")
    let rowb = g_rd.kv_dim * g_rd.kv_esz
    let base = (l * g_rd.seq_cap) * rowb
    unsafe {
        upload_region_at(g_rd.k_mirror, base, kp, npos * rowb)
        upload_region_at(g_rd.v_mirror, base, vp, npos * rowb)
    }
}

//! Bulk hydration: read layer `l`'s mirror rows [0, npos) into the caller's cache (kp/vp,
//! [npos x kvd] codec rows each), one submit per KV_READBACK_ROWS rows. The host-KV-authoritative
//! rail — makes continuations, CPU fallback, and slot switches valid after a device-resident prefill.
def vk_rdec_read_kv_bulk(l : int64; kp : uint8?; vp : uint8?; npos : int64) {
    assert(g_rd != null, "vk_rdec_read_kv_bulk before prepare")
    verify(vk_moe_init())
    let rowb = g_rd.kv_dim * g_rd.kv_esz
    var p0 = 0l
    var raw = alloc_transient_cmd()   // re-recorded per chunk (the pool allows reset), freed below
    while (p0 < npos) {
        let rows = min(KV_READBACK_ROWS, npos - p0)
        let bytes = rows * rowb
        let off = ((l * g_rd.seq_cap) + p0) * rowb
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
def vk_rdec_read_kv(l, pos : int64; kp : uint8?; vp : uint8?) {
    assert(g_rd != null, "vk_rdec_read_kv before prepare")
    let rowb = g_rd.kv_dim * g_rd.kv_esz
    let off = ((l * g_rd.seq_cap) + pos) * rowb
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

def private ensure_dnd_state {
    if (g_gpu.dnd_ready) {
        return
    }
    ensure_dn_state()   // pulls in ffn/batch state: batch_cmd + the dn staging bufs
    g_gpu.dnd_bg = make_host_buf(DND_BG_BYTES, false)
    g_gpu.dnd_oq_dev = make_device_buf(DND_OQ_BYTES)
    g_gpu.dnd_os_dev = make_device_buf(DND_OS_BYTES)
    g_gpu.dnd_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: deltanet decode step engaged\n")
}

// create one layer's step chain: per-layer state/smalls buffers, the three sets, the once-filled
// GEMV metas (dn stacks never serve FFN GEMVs, so their host metas are ours to pin), and the
// warm cmd — per token only acts/β/γ refresh and the cmd resubmits as-is.
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
                    parity = 0, tok <- make_host_buf(16l, true), valid = false, dirty = false)
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
        fixed_array(g_gpu.stacks[s_qkv].y_dev, st.state_dev, st.smalls_dev, g_gpu.dnd_os_dev, g_gpu.dnd_oq_dev, st.tok.buf),
        fixed_array(Y_BYTES, st.state_bytes, DND_SMALLS_BYTES, DND_OS_BYTES, DND_OQ_BYTES, 16l),
        fixed_array(VHG_Y1 | VHG_Y2, VHG_ST, VHG_SM, VHG_WS, VHG_WS, 0u))
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
    let rpw = g_gpu.rows_per_wg
    var pcqkv = GemvArgs(n = uint(dim), d = uint(cd), nreg = 1u, ybase = 0u)
    gemv_cls_enc(fqkv, raw, h, qset, pcqkv, (cd + rpw - 1l) / rpw)
    var pcz = GemvArgs(n = uint(dim), d = uint(di), nreg = 1u, ybase = uint(cd))
    gemv_cls_enc(fz, raw, h, st.z_set, pcz, (di + rpw - 1l) / rpw)
    var pcd = DnStepArgs(cd = uint(cd), kd = uint(kd), nkh = uint(nkh), ds = uint(ds),
        dconv = uint(taps + 1l), beta_off = uint(DN_SM_BETA), g_off = uint(DN_SM_G),
        taps_off = uint(DN_SM_TAPS), wnorm_off = uint(DN_SM_WNORM), a_off = uint(DN_SM_A),
        dt_off = uint(DN_SM_DT), hist_off = uint(DN_SM_HIST),
        eps = eps, qscale = 1.0 / sqrt(float(ds)))
    enc_dn_step_cls(raw, h, st.fused_set, pcd, nvh)
    var pco = GemvArgs(n = uint(di), d = uint(dim), nreg = 1u, ybase = 0u)
    gemv_cls_enc(fo, raw, h, st.out_set, pco, (dim + rpw - 1l) / rpw)
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

def vk_moe_dn_step(var yp : float?; s_qkv, s_z, s_o : int; woq, woz, woo : int64;
                   xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
                   convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                   stp : float const?; histp : float const?;
                   dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    let taps = dconv - 1l
    assert(ds <= DN_MAX_DS && ds % 32l == 0l && nvh <= DN_MAX_HEADS && cd <= DN_MAX_CD
           && dconv <= DN_MAX_DCONV && di == nvh * ds && kd == nkh * ds && cd == 2l * kd + di
           && dim % 32l == 0l && di % 32l == 0l && di <= DND_OQ_BYTES,
        "dasLLAMA vulkan tier: deltanet geometry outside step capacity")
    ensure_dnd_state()
    if (!key_exists(g_gpu.dnd_steps, woq)) {
        dnd_step_make(s_qkv, s_z, s_o, woq, woz, woo, dim, cd, kd, di, nvh, nkh, ds, taps, eps)
    }
    var st = g_gpu.dnd_steps[woq]
    if (!st.valid) {
        dnd_cold_upload(st, stp, histp, convw, wnormp, ap, dtp, cd, taps, dconv, nvh, ds, eps)
        st.parity = 0   // the cold image is ring image 0
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
    st.parity = 1 - st.parity   // the step wrote the other image; it now holds current history
    st.valid = true
    st.dirty = true
    g_gpu.dnd_steps[woq] = st
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_dnd prep {get_time_usec(ts0) - get_time_usec(ts1)} submit {get_time_usec(ts1) - get_time_usec(ts2)} copy {get_time_usec(ts2)}\n")
    }
}

// MoeGpuDnFlushFn: sync one layer's device-resident state/ring down into the engine's buffers.
// Any flush retires the device copy (the CPU is about to read AND may then advance on its own) —
// the next GPU step cold-uploads from the engine's then-current state.
def vk_dn_step_flush(l : int64; var stp : float?; var histp : float?) {
    if (g_gpu == null || !g_gpu.dnd_ready) {
        return
    }
    let woq = moe_gpu_dn_layer_key(l)
    if (woq < 0l || !key_exists(g_gpu.dnd_steps, woq)) {
        return
    }
    var st = g_gpu.dnd_steps[woq]
    if (st.dirty) {
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
            memcpy(reinterpret<void?>(stp), g_gpu.dn_state_host.mapped, st.state_bytes)
            memcpy(reinterpret<void?>(histp), g_gpu.dn_tail_host.mapped, st.ring_bytes)
        }
    }
    st.dirty = false
    st.valid = false
    g_gpu.dnd_steps[woq] = st
}

// MoeGpuDnInvalidateFn: the engine rebuilt/zeroed its CPU state — every device copy is stale
def vk_dn_step_invalidate {
    if (g_gpu == null) {
        return
    }
    for (st in values(g_gpu.dnd_steps)) {
        st.valid = false
        st.dirty = false
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
def private ffn_cmd_for(s1, s3, s2 : int; nrows, n, nfe : int64; is_gelu : bool) : VkCommandBuffer {
    let key = (int64(s1) * int64(MAX_STACKS) + int64(s3)) * int64(MAX_STACKS) + int64(s2)
    if (!key_exists(g_gpu.ffn_cmds, key)) {
        var fc = FfnCmd(cmd = alloc_cmd(), nrows = 0l)
        // the requant form follows the DOWN stack
        let dkq = !vk_fmt_b32(g_gpu.stacks[s2].fmt)
        verify((dkq ? ensure_q8k_actrq_cls() : ensure_q8_actrq_cls()) && gemv_cls_ensure(g_gpu.stacks[s2].fmt),
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
    var fc = g_gpu.ffn_cmds[key]
    if (fc.nrows != nrows || fc.gelu != is_gelu) {
        if (fc.nrows != 0l) {
            let rf : VkCommandBufferResetFlags
            vk_check(vkResetCommandBuffer(fc.cmd, rf), null)
        }
        let raw = fc.cmd
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        // gate/up roles on the two stacks' class sets, this chain's view (re-declared per record)
        var gset = stack_gemv_cls_set(s1)
        var uset = stack_gemv_cls_set(s3)
        hz_set_bits(gset, 0u, 0u, 0u, 0u, 0u, VHG_Y1)
        hz_set_bits(uset, 0u, 0u, 0u, 0u, 0u, VHG_Y2)
        var h : VkHaz
        let rpw = g_gpu.rows_per_wg
        let ggu = (nrows * nfe + rpw - 1l) / rpw
        var pcg = GemvArgs(n = uint(n), d = uint(nfe), nreg = uint(nrows), ybase = 0u)
        gemv_cls_enc(g_gpu.stacks[s1].fmt, raw, h, gset, pcg, ggu)
        gemv_cls_enc(g_gpu.stacks[s3].fmt, raw, h, uset, pcg, ggu)
        // the requant form follows the DOWN stack
        let dkq = !vk_fmt_b32(g_gpu.stacks[s2].fmt)
        var pca = ActArgs(nelem = uint(nrows * nfe), gelu = is_gelu ? 1u : 0u,
            nblk = uint(nrows * nfe / (dkq ? 256l : 32l)))
        let awgs = (nrows * nfe / (dkq ? 8l : 4l) + 255l) / 256l
        if (dkq) {
            enc_q8k_actrq_cls(raw, h, fc.actrq_set, pca, awgs)
        } else {
            enc_q8_actrq_cls(raw, h, fc.actrq_set, pca, awgs)
        }
        var pcd = GemvArgs(n = uint(nfe), d = uint(n), nreg = uint(nrows), ybase = 0u)
        gemv_cls_enc(g_gpu.stacks[s2].fmt, raw, h, fc.down_set, pcd, (nrows * n + rpw - 1l) / rpw)
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

// fill metas/acts and fetch the recorded chain — shared by the sync GEMV form and the async
// begin/join pair. Returns the down stack index alongside the cmd (join reads its y staging).
def private ffn_gemv_prep(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n, nfe, nrows : int64; f1, f3, f2 : int; is_gelu : bool) : tuple<cmd : VkCommandBuffer; s2 : int> {
    ensure_ffn_state()   // hq/hs planes for the act+requant: a shexp-only run dispatches FFN first
    let s1 = find_stack(unsafe(offs1[0]), f1, "decode ffn gate")
    let s3 = find_stack(unsafe(offs3[0]), f3, "decode ffn up")
    let s2 = find_stack(unsafe(offs2[0]), f2, "decode ffn down")
    assert(nrows * max(nfe, n) * 4l <= Y_BYTES, "dasLLAMA vulkan tier: FFN dispatch exceeds output capacity")
    // gate/up share one activation image (same xqp below) so forms must agree; down's is its own
    let kq_in = !vk_fmt_b32(f1)
    assert(!vk_fmt_b32(f3) == kq_in,
        "dasLLAMA vulkan tier: gate/up form mismatch — they share one activation image")
    let xunit_in = fmt_unit(f1)
    ensure_gemv_scratch(s1)
    ensure_gemv_scratch(s3)
    ensure_gemv_scratch(s2)
    fill_stack_sched_rows(s1, offs1, nregions, nrows, n / xunit_in)
    fill_stack_sched_rows(s3, offs3, nregions, nrows, n / xunit_in)
    fill_stack_sched_rows(s2, offs2, nregions, nrows, nfe / fmt_unit(f2))
    fill_stack_acts(s1, xqp, xsp, nrows * n)
    fill_stack_acts(s3, xqp, xsp, nrows * n)
    return (cmd = ffn_cmd_for(s1, s3, s2, nrows, n, nfe, is_gelu), s2 = s2)
}

def vk_moe_ffn_gemv(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n, nfe, nrows : int64; f1, f3, f2 : int; is_gelu : bool) {
    // decode-form only — vk_moe_ffn routes the combined (npos > 0) contract to the batch arm instead
    var pc = ffn_gemv_prep(offs1, offs3, offs2, nregions, xqp, xsp, n, nfe, nrows, f1, f3, f2, is_gelu)
    submit_wait(pc.cmd)
    unsafe {
        memcpy(reinterpret<void?>(goutp), g_gpu.stacks[pc.s2].y.mapped, nrows * n * 4l)
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
    let rpw = g_gpu.rows_per_wg
    var pcq = GemvArgs(n = uint(n), d = uint(dq), nreg = 1u, ybase = 0u)
    gemv_cls_enc(fq, raw, h, qset, pcq, (dq + rpw - 1l) / rpw)
    var pck = GemvArgs(n = uint(n), d = uint(dk), nreg = 1u, ybase = uint(dq))
    gemv_cls_enc(fk, raw, h, qc.k_set, pck, (dk + rpw - 1l) / rpw)
    var pcv = GemvArgs(n = uint(n), d = uint(dv), nreg = 1u, ybase = uint(dq + dk))
    gemv_cls_enc(fv, raw, h, qc.v_set, pcv, (dv + rpw - 1l) / rpw)
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
    let rpw = g_gpu.rows_per_wg
    var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u)
    gemv_cls_enc(fmt, raw, h, g_gpu.stacks[si].cls_set, pc, (d + rpw - 1l) / rpw)
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
