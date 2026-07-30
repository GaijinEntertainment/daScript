options gen2
options _comment_hygiene = true

module dasllama_vulkan_decode shared public
options _cyclomatic_complexity = 0   // whole-token record/setup walks — split queued for the dedup phase
options _function_length = 0   // same

require dasllama/dasllama_vulkan_common   // g_gpu/g_rd state, plumbing, hazard rail, fill metas
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
def vk_rdec_prepare(n_layers, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64;
                    neox : bool; eps, scale : float; qk_norm : bool) : bool {
    if (!vk_moe_init()) {
        return false
    }
    // the shaders index the KV mirror with 32-bit uint meta offsets and the buffer must fit a 2GiB
    // shard — a huge seq_cap would overflow both. The budget gate should decline first; guard anyway.
    if (n_layers * seq_cap * kv_dim > 2_000_000_000l) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: KV mirror ({n_layers} x {seq_cap} x {kv_dim}) exceeds 32-bit index / 2GiB shard\n")
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
    g_rd = new RDec(ready = false, n_layers = n_layers, dim = dim, qd = qd, kv_dim = kv_dim,
                 head_size = head_size, n_heads = n_heads, kv_mul = n_heads / (kv_dim / head_size),
                 hidden = hidden, vocab = vocab, seq_cap = seq_cap, neox = neox, eps = eps, scale = scale,
                 qk_norm = qk_norm)
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
    r.k_mirror = make_device_buf(n_layers * seq_cap * kv_dim * 4l)
    r.v_mirror = make_device_buf(n_layers * seq_cap * kv_dim * 4l)
    // norms layout: [rms_att[l], rms_ffn[l]] x L + rms_final (dim each), then under qk_norm the
    // per-head rows [rms_q[l], rms_k[l]] x L (head_size each) appended past the fixed layout,
    // then the eps/ascale pair the fused ar+requant reads (its binding 4 is the scale output)
    r.ar_scal_off = n_layers * 2l * dim + dim + (qk_norm ? n_layers * 2l * head_size : 0l)
    r.norms_bytes = (r.ar_scal_off + 2l) * 4l
    r.norms_dev = make_device_buf(r.norms_bytes)
    // cos layout: this token's cos[half]+sin[half], then under qk_norm the rms_q/rms_k rows and
    // eps — the fused qk-norm+rope kernel's binding 3 is the V mirror, so they cannot ride norms
    r.qkn_row_off = head_size
    r.qkn_eps_off = head_size + (qk_norm ? n_layers * 2l * head_size : 0l)
    r.cos_elems = r.qkn_eps_off + (qk_norm ? 1l : 0l)
    r.cos_dev = make_device_buf(r.cos_elems * 4l)
    r.scal_dev = make_device_buf(64l)
    r.ar_scal_dev = make_device_buf(64l)
    r.logits_dev = make_device_buf(vocab * 4l)
    r.dummy = make_device_buf(256l)
    r.x_host = make_host_buf(dim * 4l, false, [cached = true])
    r.cos_host = make_host_buf(head_size * 4l, false, [cached = true])
    r.ffn_meta_h = make_host_buf(FFN_META_BYTES, false)
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
    // the fused-rail constants: eps/ascale past the norm rows, and (under qk_norm) a copy of the
    // per-head rms rows + eps in the cos buffer, where the fused qk-norm+rope kernel can reach them
    var pair : float[2]
    pair[0] = g_rd.eps
    pair[1] = 1.0   // ascale — the residual add is unscaled in the decode driver
    upload_region_at(g_rd.norms_dev, g_rd.ar_scal_off * 4l, unsafe(addr<uint8 const?>(pair[0])), 8l)
    if (g_rd.qk_norm) {
        let rows = g_rd.n_layers * 2l * g_rd.head_size
        let src = g_rd.n_layers * 2l * g_rd.dim + g_rd.dim   // rms_q[0] within `norms`
        assert(long_length(norms) >= src + rows, "vk_rdec_upload_norms: qk_norm rows missing")
        upload_region_at(g_rd.cos_dev, g_rd.qkn_row_off * 4l, unsafe(addr<uint8 const?>(norms[src])), rows * 4l)
        upload_region_at(g_rd.cos_dev, g_rd.qkn_eps_off * 4l, unsafe(addr<uint8 const?>(pair[0])), 4l)
    }
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
        let pq = arena_planes(fq)
        let pk = arena_planes(fk)
        let pv = arena_planes(fv)
        let po = arena_planes(fo)
        let p1 = arena_planes(f1)
        let p3 = arena_planes(f3)
        let p2 = arena_planes(f2)
        // metas
        L.m_quant_xb = rd_meta(); L.m_q = rd_meta(); L.m_k = rd_meta(); L.m_v = rd_meta()
        L.m_rope = rd_meta(); L.m_attn = rd_meta(); L.m_quant_at = rd_meta(); L.m_wo = rd_meta()
        L.m_addr_ffn = rd_meta(); L.m_quant_xb2 = rd_meta(); L.m_gate = rd_meta(); L.m_up = rd_meta()
        L.m_actrq = rd_meta(); L.m_down = rd_meta(); L.m_addr_next = rd_meta()
        // quantize xb -> xq/xs (dn_requant: in vk_upf@0, meta@2, xq@3, xs@4)
        L.s_quant_xb = vk_set6(g_rd.xb_dev, g_rd.dummy, L.m_quant_xb.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.dummy,
            dim * 4l, 256l, META_BYTES, dim, (dim / 32l) * 4l, 256l,
            fixed_array(VHZ_XB, 0u, 0u, VHZ_XQ, VHZ_XQ, 0u))
        // q/k/v GEMV: arena[f] planes @0/1, meta@2, xq@3, xs@4, out@5
        L.s_q = vk_set6(pq.wq, pq.ws, L.m_q.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.q_dev,
            pq.wqb, pq.wsb, META_BYTES, dim, (dim / 32l) * 4l, qd * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_Q))
        L.s_k = vk_set6(pk.wq, pk.ws, L.m_k.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev,
            pk.wqb, pk.wsb, META_BYTES, dim, (dim / 32l) * 4l, (qd + 2l * kvd) * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVK))
        L.s_v = vk_set6(pv.wq, pv.ws, L.m_v.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev,
            pv.wqb, pv.wsb, META_BYTES, dim, (dim / 32l) * 4l, (qd + 2l * kvd) * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVV))
        // same-format k/v share arena planes — one two-region dispatch fills both kv halves
        L.kv_merged = fk == fv
        if (L.kv_merged) {
            L.s_kv = vk_set6(pk.wq, pk.ws, L.m_k.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev,
                pk.wqb, pk.wsb, META_BYTES, dim, (dim / 32l) * 4l, (qd + 2l * kvd) * 4l,
                fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVK | VHZ_KVV))
        }
        // qk rmsnorm (qk_norm models): q@0 (q_dev), meta@2, norm rows@3, eps@4, k@5 (kv_dev, k at qd)
        let mirbytes = g_rd.n_layers * g_rd.seq_cap * kvd * 4l
        if (g_rd.qk_norm) {
            L.m_qkn = rd_meta()
            L.s_qkn = vk_set6(g_rd.q_dev, g_rd.dummy, L.m_qkn.buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.kv_dev,
                qd * 4l, 256l, META_BYTES, g_rd.norms_bytes, 64l, (qd + 2l * kvd) * 4l,
                fixed_array(VHZ_Q, 0u, 0u, 0u, 0u, VHZ_KVK))
            // the fused twin: rope's own binding plan, with the rms rows riding the cos buffer
            L.m_qkr = rd_meta()
            L.s_qkn_rope = vk_set6(g_rd.kv_dev, g_rd.k_mirror, L.m_qkr.buf, g_rd.v_mirror, g_rd.cos_dev, g_rd.q_dev,
                (qd + 2l * kvd) * 4l, mirbytes, META_BYTES, mirbytes, g_rd.cos_elems * 4l, qd * 4l,
                fixed_array(VHZ_KVK | VHZ_KVV, VHZ_MIR, 0u, VHZ_MIR, VHZ_COS, VHZ_Q))
        }
        // rope+store: kv@0, Kmirror@1, meta@2, Vmirror@3, cossin@4, q@5
        L.s_rope = vk_set6(g_rd.kv_dev, g_rd.k_mirror, L.m_rope.buf, g_rd.v_mirror, g_rd.cos_dev, g_rd.q_dev,
            (qd + 2l * kvd) * 4l, mirbytes, META_BYTES, mirbytes, g_rd.head_size * 4l, qd * 4l,
            fixed_array(VHZ_KVK | VHZ_KVV, VHZ_MIR, 0u, VHZ_MIR, VHZ_COS, VHZ_Q))
        // attn: q@0, Kmirror@1, meta@2, Vmirror@3, scale@4, out@5
        L.s_attn = vk_set6(g_rd.q_dev, g_rd.k_mirror, L.m_attn.buf, g_rd.v_mirror, g_rd.scal_dev, g_rd.attn_dev,
            qd * 4l, mirbytes, META_BYTES, mirbytes, 64l, qd * 4l,
            fixed_array(VHZ_Q, VHZ_MIR, 0u, VHZ_MIR, 0u, VHZ_ATT))
        // quantize attn -> aq/as
        L.s_quant_at = vk_set6(g_rd.attn_dev, g_rd.dummy, L.m_quant_at.buf, g_rd.aq_dev, g_rd.as_dev, g_rd.dummy,
            qd * 4l, 256l, META_BYTES, qd, (qd / 32l) * 4l, 256l,
            fixed_array(VHZ_ATT, 0u, 0u, VHZ_AQ, VHZ_AQ, 0u))
        // wo GEMV -> xb2
        L.s_wo = vk_set6(po.wq, po.ws, L.m_wo.buf, g_rd.aq_dev, g_rd.as_dev, g_rd.xb2_dev,
            po.wqb, po.wsb, META_BYTES, qd, (qd / 32l) * 4l, dim * 4l,
            fixed_array(0u, 0u, 0u, VHZ_AQ, VHZ_AQ, VHZ_XB2))
        // addrms: x@0 += xb2@1, meta@2, norms@3, scalars@4, xb@5
        L.s_addr_ffn = vk_set6(g_rd.x_dev, g_rd.xb2_dev, L.m_addr_ffn.buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.xb_dev,
            dim * 4l, dim * 4l, META_BYTES, g_rd.norms_bytes, 64l, dim * 4l,
            fixed_array(VHZ_X, VHZ_XB2, 0u, 0u, 0u, VHZ_XB))
        // fused twin: the normed row quantizes in place of the xb store (xs@4, xq@5)
        L.s_addr_ffn_rq = vk_set6(g_rd.x_dev, g_rd.xb2_dev, L.m_addr_ffn.buf, g_rd.norms_dev, g_rd.xs_dev, g_rd.xq_dev,
            dim * 4l, dim * 4l, META_BYTES, g_rd.norms_bytes, (dim / 32l) * 4l, dim,
            fixed_array(VHZ_X, VHZ_XB2, 0u, 0u, VHZ_XQ, VHZ_XQ))
        // quantize xb (post-ffn-norm) -> xq/xs
        L.s_quant_xb2 = vk_set6(g_rd.xb_dev, g_rd.dummy, L.m_quant_xb2.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.dummy,
            dim * 4l, 256l, META_BYTES, dim, (dim / 32l) * 4l, 256l,
            fixed_array(VHZ_XB, 0u, 0u, VHZ_XQ, VHZ_XQ, 0u))
        // gate/up GEMV
        L.s_gate = vk_set6(p1.wq, p1.ws, L.m_gate.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.gate_dev,
            p1.wqb, p1.wsb, META_BYTES, dim, (dim / 32l) * 4l, hid * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_GATE))
        L.s_up = vk_set6(p3.wq, p3.ws, L.m_up.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.up_dev,
            p3.wqb, p3.wsb, META_BYTES, dim, (dim / 32l) * 4l, hid * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_UP))
        // act+requant: up@0, filler@1, ffn_meta@2, hq@3, hs@4, gate@5
        L.s_actrq = vk_set6(g_rd.up_dev, g_rd.dummy, g_gpu.ffn_meta_dev, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.gate_dev,
            hid * 4l, 256l, FFN_META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, hid * 4l,
            fixed_array(VHZ_UP, 0u, VHZ_META, VHZ_HQ, VHZ_HQ, VHZ_GATE))
        // down GEMV -> ffnout
        L.s_down = vk_set6(p2.wq, p2.ws, L.m_down.buf, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.ffnout_dev,
            p2.wqb, p2.wsb, META_BYTES, hid, (hid / 32l) * 4l, dim * 4l,
            fixed_array(0u, 0u, 0u, VHZ_HQ, VHZ_HQ, VHZ_FFO))
        // addrms next: x@0 += ffnout@1, norms@3, xb@5
        L.s_addr_next = vk_set6(g_rd.x_dev, g_rd.ffnout_dev, L.m_addr_next.buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.xb_dev,
            dim * 4l, dim * 4l, META_BYTES, g_rd.norms_bytes, 64l, dim * 4l,
            fixed_array(VHZ_X, VHZ_FFO, 0u, 0u, 0u, VHZ_XB))
        // fused twin — on the last layer this also serves the final norm, so the split rail's
        // separate quant_final dispatch has no fused counterpart
        L.s_addr_next_rq = vk_set6(g_rd.x_dev, g_rd.ffnout_dev, L.m_addr_next.buf, g_rd.norms_dev, g_rd.xs_dev, g_rd.xq_dev,
            dim * 4l, dim * 4l, META_BYTES, g_rd.norms_bytes, (dim / 32l) * 4l, dim,
            fixed_array(VHZ_X, VHZ_FFO, 0u, 0u, VHZ_XQ, VHZ_XQ))
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

        g_rd.m_prologue = rd_meta()
        g_rd.m_quant_final = rd_meta()
        g_rd.cls_meta_dev = make_device_buf(CLS_META_BYTES)
        // prologue: xb = rms_att[0](x), add off
        g_rd.s_prologue = vk_set6(g_rd.x_dev, g_rd.dummy, g_rd.m_prologue.buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.xb_dev,
            dim * 4l, 256l, META_BYTES, g_rd.norms_bytes, 64l, dim * 4l,
            fixed_array(VHZ_X, 0u, 0u, 0u, 0u, VHZ_XB))
        // fused twin — subsumes layer 0's quant_xb
        g_rd.s_prologue_rq = vk_set6(g_rd.x_dev, g_rd.dummy, g_rd.m_prologue.buf, g_rd.norms_dev, g_rd.xs_dev, g_rd.xq_dev,
            dim * 4l, 256l, META_BYTES, g_rd.norms_bytes, (dim / 32l) * 4l, dim,
            fixed_array(VHZ_X, 0u, 0u, 0u, VHZ_XQ, VHZ_XQ))
        // quantize final xb
        g_rd.s_quant_final = vk_set6(g_rd.xb_dev, g_rd.dummy, g_rd.m_quant_final.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.dummy,
            dim * 4l, 256l, META_BYTES, dim, (dim / 32l) * 4l, 256l,
            fixed_array(VHZ_XB, 0u, 0u, VHZ_XQ, VHZ_XQ, 0u))
        // cls GEMV: arena[fmt]@0/1, device meta@2, xq@3, xs@4, logits@5
        let pc = arena_planes(cls_fmt)
        g_rd.cls_set = vk_set6(pc.wq, pc.ws, g_rd.cls_meta_dev, g_rd.xq_dev, g_rd.xs_dev, g_rd.logits_dev,
            pc.wqb, pc.wsb, CLS_META_BYTES, dim, (dim / 32l) * 4l, vocab * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_LOG))
        // cls meta is constant (block base, n=dim, d=vocab) — device-filled once
        var mp = reinterpret<uint?>(g_gpu.staging.mapped)
        mp[0] = uint(dim)
        mp[1] = uint(vocab)
        mp[2] = 1u
        mp[3] = 0u
        mp[4] = uint(cls_block)
        mp[5] = 0u
        staged_upload(g_rd.cls_meta_dev, 0l, CLS_META_BYTES)
        g_rd.cls_fmt_marker = cls_fmt
        g_rd.tok_recorded = false   // the recorded chain references the replaced cls/prologue sets
        g_rd.ready = true
    }
}

// Record the whole-token cmd ONCE per set_layer/set_cls epoch: constant metas + scalars fill here;
// per-token state is only x/cos (in-cmd copies) and the rope/attn position words (host-visible)
def private rd_record_token {
    let dim = g_rd.dim
    let qd = g_rd.qd
    let kvd = g_rd.kv_dim
    let hid = g_rd.hidden
    let nlfin = g_rd.n_layers * 2l * dim   // rms_final offset in the norms buffer (dim units later)
    unsafe {
        // constant scalars — attn ([0]=scale) and add_rms ([0]=eps, [1]=ascale)
        var sp = reinterpret<float?>(g_gpu.staging.mapped)
        sp[0] = g_rd.scale
        staged_upload(g_rd.scal_dev, 0l, 64l)
        sp[0] = g_rd.eps
        sp[1] = 1.0
        staged_upload(g_rd.ar_scal_dev, 0l, 64l)
        // decode-owned actrq meta — the in-cmd copy re-establishes ffn_meta_dev every submit (batch rails share it)
        var fp = reinterpret<uint?>(g_rd.ffn_meta_h.mapped)
        fp[0] = uint(hid)
        fp[1] = 0u
        fp[2] = uint(hid / 32l)
        for (i in range(5)) {
            fp[3 + i] = 0u
        }
        fill_addrms_meta(g_rd.m_prologue, dim, 0l, false)   // rms_att[0] at offset 0
        for (l in range64(g_rd.n_layers)) {
            var L & = g_rd.layers[l]
            fill_quant_meta(L.m_quant_xb, dim / 32l)
            fill_gemv_meta(L.m_q, dim, qd, 0l, L.bq)
            if (L.kv_merged) {
                fill_gemv_meta2(L.m_k, dim, kvd, qd, L.bk, L.bv)  // k, v regions land at qd, qd+kvd
            } else {
                fill_gemv_meta(L.m_k, dim, kvd, qd, L.bk)         // k at ybase qd in kv_dev
                fill_gemv_meta(L.m_v, dim, kvd, qd + kvd, L.bv)   // v at ybase qd+kvd
            }
            if (g_rd.qk_norm) {
                let hs = g_rd.head_size
                var mn = reinterpret<uint?>(L.m_qkn.mapped)
                mn[0] = uint(hs); mn[1] = uint(g_rd.n_heads); mn[2] = uint(kvd / hs)
                mn[3] = uint(qd); mn[4] = 0u; mn[5] = uint(qd)   // one row: k lives at qd in kv_dev
                mn[6] = uint(nlfin + dim + l * 2l * hs)          // rms_q[l] past the fixed norms layout
                mn[7] = uint(nlfin + dim + (l * 2l + 1l) * hs)   // rms_k[l]
                fill_qkn_rope_meta(L.m_qkr, l)
            }
            var mr = reinterpret<uint?>(L.m_rope.mapped)
            mr[0] = uint(qd); mr[1] = uint(kvd); mr[2] = uint(g_rd.head_size); mr[3] = uint(g_rd.head_size / 2l)
            mr[4] = g_rd.neox ? 1u : 0u; mr[5] = uint(qd / 2l); mr[6] = uint(qd / 2l + kvd / 2l)
            var ma = reinterpret<uint?>(L.m_attn.mapped)
            ma[2] = uint(kvd); ma[3] = uint(g_rd.head_size); ma[4] = uint(g_rd.kv_mul)
            ma[5] = uint(l * g_rd.seq_cap * kvd); ma[6] = uint(l * g_rd.seq_cap * kvd)
            fill_quant_meta(L.m_quant_at, qd / 32l)
            fill_gemv_meta(L.m_wo, qd, dim, 0l, L.bo)
            fill_addrms_meta(L.m_addr_ffn, dim, (l * 2l + 1l) * dim, true)   // rms_ffn[l]
            fill_quant_meta(L.m_quant_xb2, dim / 32l)
            fill_gemv_meta(L.m_gate, dim, hid, 0l, L.b1)
            fill_gemv_meta(L.m_up, dim, hid, 0l, L.b3)
            fill_gemv_meta(L.m_down, hid, dim, 0l, L.b2)
            let nxt = l + 1l < g_rd.n_layers ? (l + 1l) * 2l * dim : nlfin   // rms_att[l+1] or final
            fill_addrms_meta(L.m_addr_next, dim, nxt, true)
        }
        fill_quant_meta(g_rd.m_quant_final, dim / 32l)
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
        vhz_dep(raw, h, 0u, VHZ_META, true)
        cmd_copy_whole(raw, g_rd.ffn_meta_h.buf, g_gpu.ffn_meta_dev, FFN_META_BYTES)
        // fused rail: a layer's activation quant rides the PREVIOUS addr_next, and qkn rides rope
        let fuse = vk_fuse_on()
        let fuse_qkr = fuse && g_rd.qk_norm
        if (fuse) {
            rd_dispatch(raw, g_gpu.ar_rq_pipeline, g_rd.s_prologue_rq, 1u, h)   // rms_att[0](x) -> xq/xs
        } else {
            rd_dispatch(raw, g_gpu.ar_pipeline, g_rd.s_prologue, 1u, h)   // xb = rms_att[0](x)
        }
        for (l in range64(g_rd.n_layers)) {
            var L & = g_rd.layers[l]
            if (!fuse) {
                rd_dispatch(raw, g_gpu.dn_rq_pipeline, L.s_quant_xb, uint((dim / 4l + 255l) / 256l), h)
            }
            rd_dispatch_gemv(raw, L.fq, L.s_q, rd_gemv_wg(qd), h)
            if (L.kv_merged) {
                rd_dispatch_gemv(raw, L.fk, L.s_kv, rd_gemv_wg(2l * kvd), h)
            } else {
                rd_dispatch_gemv(raw, L.fk, L.s_k, rd_gemv_wg(kvd), h)
                rd_dispatch_gemv(raw, L.fv, L.s_v, rd_gemv_wg(kvd), h)
            }
            if (fuse_qkr) {
                rd_dispatch(raw, g_gpu.qkn_rope_pipeline, L.s_qkn_rope, uint(g_rd.n_heads + kvd / g_rd.head_size), h)
            } else {
                if (g_rd.qk_norm) {
                    rd_dispatch(raw, g_gpu.qkn_pipeline, L.s_qkn, uint(g_rd.n_heads + kvd / g_rd.head_size), h)
                }
                rd_dispatch(raw, g_gpu.rks_pipeline, L.s_rope, uint((qd / 2l + kvd / 2l + int64(WG_X) - 1l) / int64(WG_X)), h)
            }
            rd_dispatch(raw, g_gpu.da_pipeline, L.s_attn, uint(g_rd.n_heads), h)
            rd_dispatch(raw, g_gpu.dn_rq_pipeline, L.s_quant_at, uint((qd / 4l + 255l) / 256l), h)
            rd_dispatch_gemv(raw, L.fo, L.s_wo, rd_gemv_wg(dim), h)
            if (fuse) {
                rd_dispatch(raw, g_gpu.ar_rq_pipeline, L.s_addr_ffn_rq, 1u, h)
            } else {
                rd_dispatch(raw, g_gpu.ar_pipeline, L.s_addr_ffn, 1u, h)
                rd_dispatch(raw, g_gpu.dn_rq_pipeline, L.s_quant_xb2, uint((dim / 4l + 255l) / 256l), h)
            }
            rd_dispatch_gemv(raw, L.f1, L.s_gate, rd_gemv_wg(hid), h)
            rd_dispatch_gemv(raw, L.f3, L.s_up, rd_gemv_wg(hid), h)
            rd_dispatch(raw, g_gpu.actrq_pipeline, L.s_actrq, uint((hid / 4l + 255l) / 256l), h)
            rd_dispatch_gemv(raw, L.f2, L.s_down, rd_gemv_wg(dim), h)
            if (fuse) {
                rd_dispatch(raw, g_gpu.ar_rq_pipeline, L.s_addr_next_rq, 1u, h)
            } else {
                rd_dispatch(raw, g_gpu.ar_pipeline, L.s_addr_next, 1u, h)
            }
        }
        if (!fuse) {
            rd_dispatch(raw, g_gpu.dn_rq_pipeline, g_rd.s_quant_final, uint((dim / 4l + 255l) / 256l), h)
        }
        rd_dispatch_gemv(raw, g_rd.cls_fmt_marker, g_rd.cls_set, rd_gemv_wg(g_rd.vocab), h)
        vhz_dep(raw, h, VHZ_LOG, 0u, true)   // compute -> transfer for the logits DMA
        cmd_copy_whole(raw, g_rd.logits_dev, g_rd.logits_host.buf, g_rd.vocab * 4l)
        pfq_end(raw)
        vk_check(vkEndCommandBuffer(raw), null)
    }
    g_rdq_n = g_pfq_n   // decode's stamp count (prefill re-arms the shared pool per window)
    g_rd.tok_recorded = true
}

def private rdq_sample {
    // stamp count follows each layer's own kv_merged (K-quant ggufs mix formats per layer)
    let fuse = vk_fuse_on()
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
        var names : array<string>
        if (fuse) {
            names <- ["q", "kv", fuse_qkr ? "qknrope" : "rope", "attn", "rq_a", "wo", "arrq_f", "gate", "up", "actrq", "down", "arrq_n"]
        } elif (g_rd.qk_norm) {
            names <- ["rq_x", "q", "kv", "qkn", "rope", "attn", "rq_a", "wo", "ar1", "rq_f", "gate", "up", "actrq", "down", "ar2"]
        } else {
            names <- ["rq_x", "q", "kv", "rope", "attn", "rq_a", "wo", "ar1", "rq_f", "gate", "up", "actrq", "down", "ar2"]
        }
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
    }
    let kvd = g_rd.kv_dim
    unsafe {
        memcpy(g_rd.x_host.mapped, addr<void?>(x[0]), g_rd.dim * 4l)
        memcpy(g_rd.cos_host.mapped, addr<void?>(cossin[0]), g_rd.head_size * 4l)
        for (l in range64(g_rd.n_layers)) {
            var L & = g_rd.layers[l]
            let row = uint(l * g_rd.seq_cap * kvd + pos * kvd)
            var mr = reinterpret<uint?>(L.m_rope.mapped)
            mr[7] = row
            mr[8] = row
            if (g_rd.qk_norm) {   // the fused twin keeps the mirror-row words at the same indices
                var mq = reinterpret<uint?>(L.m_qkr.mapped)
                mq[7] = row
                mq[8] = row
            }
            var ma = reinterpret<uint?>(L.m_attn.mapped)
            ma[0] = uint(cnt)
        }
        submit_wait(g_rd.cmd)
        memcpy(addr<void?>(logits[0]), g_rd.logits_host.mapped, g_rd.vocab * 4l)
    }
    if (vk_prof() && g_rdq_n > 2u) {
        rdq_sample()
    }
}

//! Batch-decode support: upload one layer's f32 KV history ([npos x kvd] each) into the shared
//! mirror at layer `l`, positions [0, npos). Resyncs a session's cache before its decode step.
def vk_rdec_sync_kv(l : int64; kp : float const?; vp : float const?; npos : int64) {
    assert(g_rd != null, "vk_rdec_sync_kv before prepare")
    let kvd = g_rd.kv_dim
    let base = (l * g_rd.seq_cap) * kvd * 4l
    unsafe {
        upload_region_at(g_rd.k_mirror, base, reinterpret<uint8 const?>(kp), npos * kvd * 4l)
        upload_region_at(g_rd.v_mirror, base, reinterpret<uint8 const?>(vp), npos * kvd * 4l)
    }
}

//! Bulk hydration: read layer `l`'s mirror rows [0, npos) into the caller's f32 cache (kp/vp,
//! [npos x kvd] each), one submit per KV_READBACK_ROWS rows. The host-KV-authoritative rail —
//! makes continuations, CPU fallback, and slot switches valid after a device-resident prefill.
def vk_rdec_read_kv_bulk(l : int64; kp : float?; vp : float?; npos : int64) {
    assert(g_rd != null, "vk_rdec_read_kv_bulk before prepare")
    verify(vk_moe_init())
    let kvd = g_rd.kv_dim
    var p0 = 0l
    var raw = alloc_transient_cmd()   // re-recorded per chunk (the pool allows reset), freed below
    while (p0 < npos) {
        let rows = min(KV_READBACK_ROWS, npos - p0)
        let bytes = rows * kvd * 4l
        let off = ((l * g_rd.seq_cap) + p0) * kvd * 4l
        unsafe {
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            cmd_copy_range(raw, g_rd.k_mirror, off, g_rd.kv_readback.buf, 0l, bytes)
            cmd_copy_range(raw, g_rd.v_mirror, off, g_rd.kv_readback.buf, bytes, bytes)
            vk_check(vkEndCommandBuffer(raw), null)
            submit_wait(raw)
            memcpy(reinterpret<void?>(kp + p0 * kvd), g_rd.kv_readback.mapped, bytes)
            memcpy(reinterpret<void?>(vp + p0 * kvd), reinterpret<uint8?>(g_rd.kv_readback.mapped) + bytes, bytes)
        }
        p0 += rows
    }
    free_transient_cmd(raw)
}

//! Batch-decode writeback: read layer `l`'s new K/V row at `pos` from the mirror back into the
//! caller's f32 cache row (kp/vp, kvd each). Keeps the CPU cache current after a GPU decode step.
def vk_rdec_read_kv(l, pos : int64; kp : float?; vp : float?) {
    assert(g_rd != null, "vk_rdec_read_kv before prepare")
    let kvd = g_rd.kv_dim
    let off = ((l * g_rd.seq_cap) + pos) * kvd * 4l
    verify(vk_moe_init())
    unsafe {
        var raw = alloc_transient_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        cmd_copy_range(raw, g_rd.k_mirror, off, g_rd.kv_readback.buf, 0l, kvd * 4l)
        cmd_copy_range(raw, g_rd.v_mirror, off, g_rd.kv_readback.buf, kvd * 4l, kvd * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(reinterpret<void?>(kp), g_rd.kv_readback.mapped, kvd * 4l)
        memcpy(reinterpret<void?>(vp), reinterpret<float?>(g_rd.kv_readback.mapped) + kvd, kvd * 4l)
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
    g_gpu.dnd_meta = make_host_buf(DN_META_BYTES, false)
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
                          dim, cd, di, nvh, ds, taps : int64) {
    ensure_gemv_scratch(s_qkv)
    ensure_gemv_scratch(s_z)
    ensure_gemv_scratch(s_o)
    var qkv_region = fixed_array(woq, 0l, 1l)
    var z_region = fixed_array(woz, 0l, 1l)
    var o_region = fixed_array(woo, 0l, 1l)
    unsafe {
        fill_stack_meta_rows(s_qkv, addr(qkv_region[0]), 1l, dim, cd, 1l, 0l)
        fill_stack_meta_rows(s_z, addr(z_region[0]), 1l, dim, di, 1l, 0l, cd)
        fill_stack_meta_rows(s_o, addr(o_region[0]), 1l, di, dim, 1l, 0l)
    }
    var st = DnStep(state_dev = make_device_buf(nvh * ds * ds * 4l),
                    smalls_dev = make_device_buf(DND_SMALLS_BYTES),
                    cmd = alloc_cmd(), s_qkv = s_qkv, s_z = s_z, s_o = s_o,
                    state_bytes = nvh * ds * ds * 4l, ring_bytes = cd * taps * 4l,
                    parity = 0, valid = false, dirty = false)
    // z GEMV: z planes/meta (y base cd), the qkv stack's SHARED acts image and y buffer — the fused kernel sees qkv rows AND the z row on one binding
    st.z_set = vk_set6(g_gpu.stacks[s_z].wqbuf, g_gpu.stacks[s_z].wsbuf, g_gpu.stacks[s_z].meta.buf,
        g_gpu.stacks[s_qkv].xq.buf, g_gpu.stacks[s_qkv].xs.buf, g_gpu.stacks[s_qkv].y_dev,
        g_gpu.stacks[s_z].wqbytes, g_gpu.stacks[s_z].wsbytes, META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES,
        fixed_array(0u, 0u, 0u, 0u, 0u, VHG_Y2))
    // fused step: o scales@0, THIS in-place state@1 (residency IS this binding), o quants@3, parity-ring smalls@4, the shared y@5
    st.fused_set = vk_set6(g_gpu.dnd_os_dev, st.state_dev, g_gpu.dn_meta_dev, g_gpu.dnd_oq_dev, st.smalls_dev, g_gpu.stacks[s_qkv].y_dev,
        DND_OS_BYTES, st.state_bytes, DN_META_BYTES, DND_OQ_BYTES, DND_SMALLS_BYTES, Y_BYTES,
        fixed_array(VHG_WS, VHG_ST, VHG_META, VHG_WS, VHG_SM, VHG_Y1 | VHG_Y2))
    // out GEMV: out planes@0/1, the stack's once-filled host meta@2, the o image@3/4, its y_dev@5
    st.out_set = vk_set6(g_gpu.stacks[s_o].wqbuf, g_gpu.stacks[s_o].wsbuf, g_gpu.stacks[s_o].meta.buf,
        g_gpu.dnd_oq_dev, g_gpu.dnd_os_dev, g_gpu.stacks[s_o].y_dev,
        g_gpu.stacks[s_o].wqbytes, g_gpu.stacks[s_o].wsbytes, META_BYTES, DND_OQ_BYTES, DND_OS_BYTES, Y_BYTES,
        fixed_array(0u, 0u, 0u, VHG_WS, VHG_WS, VHG_YO))
    // the qkv stack's raw_set plays Y1 in this chain (role-fluid sets re-declare per record)
    hz_set_bits(g_gpu.stacks[s_qkv].raw_set, 0u, 0u, 0u, 0u, 0u, VHG_Y1)
    let raw = st.cmd
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    vhz_dep(raw, h, 0u, VHG_SM, true)   // one declaration for the smalls copy group
    cmd_copy_range(raw, g_gpu.dnd_bg.buf, 0l, st.smalls_dev, DN_SM_BETA * 4l, nvh * 4l)
    cmd_copy_range(raw, g_gpu.dnd_bg.buf, nvh * 4l, st.smalls_dev, DN_SM_G * 4l, nvh * 4l)
    cmd_copy_range(raw, g_gpu.dnd_bg.buf, 2l * nvh * 4l, st.smalls_dev, 8l, 4l)   // the ring parity word
    vhz_dep(raw, h, 0u, VHG_META, true)
    cmd_copy_whole(raw, g_gpu.dnd_meta.buf, g_gpu.dn_meta_dev, DN_META_BYTES)
    hz_dispatch(raw, h, gemv_pipe(0), g_gpu.stacks[s_qkv].raw_set, uint((cd + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
    hz_dispatch(raw, h, gemv_pipe(0), st.z_set, uint((di + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
    hz_dispatch(raw, h, weak_copy(g_gpu.dnd_fused_pipeline), st.fused_set, uint(nvh))
    hz_dispatch(raw, h, gemv_pipe(0), st.out_set, uint((dim + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
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
        fill_dn_meta(g_gpu.dnd_meta, 1l, cd, kd, di, nvh, nkh, ds, dconv, 0)
        dnd_step_make(s_qkv, s_z, s_o, woq, woz, woo, dim, cd, di, nvh, ds, taps)
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
        bg[2l * nvh] = float(st.parity)
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

// pre-record (or refresh on a row-count change) the triple's chain: gate+up GEMV pair, fused
// act+requant, down GEMV, y DMA. Per token only the stacks' host-visible metas, the activation
// image, and the ffn params staging change — the recorded buffer resubmits as-is.
def private ffn_cmd_for(s1, s3, s2 : int; nrows, n, nfe : int64) : VkCommandBuffer {
    let key = (int64(s1) * int64(MAX_STACKS) + int64(s3)) * int64(MAX_STACKS) + int64(s2)
    if (!key_exists(g_gpu.ffn_cmds, key)) {
        var fc = FfnCmd(cmd = alloc_cmd(), nrows = 0l)
        // fused act+requant bindings: up@0, params@2, hq/hs@3/4, gate@5; binding 1 is unused filler
        fc.actrq_set = vk_set6(g_gpu.stacks[s3].y_dev, g_gpu.stacks[s1].wsbuf, g_gpu.ffn_meta_dev, g_gpu.hq_dev, g_gpu.hs_dev, g_gpu.stacks[s1].y_dev,
            Y_BYTES, ws_range(s1), FFN_META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, Y_BYTES,
            fixed_array(VHG_Y2, 0u, VHG_META, VHG_HQ, VHG_HQ, VHG_Y1))
        // down: its own planes + host-visible meta, hq/hs as the activation image, its y_dev
        fc.down_set = vk_set6(g_gpu.stacks[s2].wqbuf, g_gpu.stacks[s2].wsbuf, g_gpu.stacks[s2].meta.buf, g_gpu.hq_dev, g_gpu.hs_dev, g_gpu.stacks[s2].y_dev,
            g_gpu.stacks[s2].wqbytes, ws_range(s2), META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, Y_BYTES,
            fixed_array(0u, 0u, 0u, VHG_HQ, VHG_HQ, VHG_YO))
        g_gpu.ffn_cmds |> insert(key, fc)
    }
    var fc = g_gpu.ffn_cmds[key]
    if (fc.nrows != nrows) {
        if (fc.nrows != 0l) {
            let rf : VkCommandBufferResetFlags
            vk_check(vkResetCommandBuffer(fc.cmd, rf), null)
        }
        let raw = fc.cmd
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        // gate/up roles on the two stacks' raw_sets, this chain's view (re-declared per record)
        hz_set_bits(g_gpu.stacks[s1].raw_set, 0u, 0u, 0u, 0u, 0u, VHG_Y1)
        hz_set_bits(g_gpu.stacks[s3].raw_set, 0u, 0u, 0u, 0u, 0u, VHG_Y2)
        var h : VkHaz
        vhz_dep(raw, h, 0u, VHG_META, true)
        cmd_copy_whole(raw, g_gpu.ffn_meta.buf, g_gpu.ffn_meta_dev, FFN_META_BYTES)
        let ggu = uint((nrows * nfe + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg)
        hz_dispatch(raw, h, gemv_pipe(g_gpu.stacks[s1].fmt), g_gpu.stacks[s1].raw_set, ggu)
        hz_dispatch(raw, h, gemv_pipe(g_gpu.stacks[s3].fmt), g_gpu.stacks[s3].raw_set, ggu)
        // the requant form follows the DOWN stack (q8n counts as q8 — decode acts stay Q8_0)
        let dkq = !vk_fmt_b32(g_gpu.stacks[s2].fmt)
        hz_dispatch(raw, h, weak_copy(dkq ? g_gpu.actrq_k_pipeline : g_gpu.actrq_pipeline),
            fc.actrq_set, uint((nrows * nfe / (dkq ? 8l : 4l) + 255l) / 256l))
        hz_dispatch(raw, h, gemv_pipe(g_gpu.stacks[s2].fmt), fc.down_set, uint((nrows * n + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.stacks[s2].y_dev, g_gpu.stacks[s2].y.buf, nrows * n * 4l)
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_ffng hz: {h.disp} nodes, {h.barriers} barriers\n")
        }
        vk_check(vkEndCommandBuffer(raw), null)
        g_gpu.ffn_cmds[key] = FfnCmd(cmd = fc.cmd, actrq_set = fc.actrq_set,
            down_set = fc.down_set, nrows = nrows)
    }
    return fc.cmd
}

// fill metas/acts and fetch the recorded chain — shared by the sync GEMV form and the async
// begin/join pair. Returns the down stack index alongside the cmd (join reads its y staging).
def private ffn_gemv_prep(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n, nfe, nrows : int64; f1, f3, f2 : int; is_gelu : bool) : tuple<cmd : VkCommandBuffer; s2 : int> {
    ensure_ffn_state()   // fill_ffn_meta below writes its staging: a shexp-only run dispatches FFN first
    let s1 = find_stack(unsafe(offs1[0]), f1)
    let s3 = find_stack(unsafe(offs3[0]), f3)
    let s2 = find_stack(unsafe(offs2[0]), f2)
    assert(nrows * max(nfe, n) * 4l <= Y_BYTES, "dasLLAMA vulkan tier: FFN dispatch exceeds output capacity")
    // gate/up share one activation image (same xqp below) so forms must agree; down's is its own
    let kq_in = !vk_fmt_b32(f1)
    assert(!vk_fmt_b32(f3) == kq_in,
        "dasLLAMA vulkan tier: gate/up form mismatch — they share one activation image")
    let xunit_in = fmt_unit(f1)
    let xunit_mid = fmt_unit(f2)
    ensure_gemv_scratch(s1)
    ensure_gemv_scratch(s3)
    ensure_gemv_scratch(s2)
    fill_stack_meta_rows(s1, offs1, nregions, n, nfe, nrows, n / xunit_in)
    fill_stack_meta_rows(s3, offs3, nregions, n, nfe, nrows, n / xunit_in)
    fill_stack_meta_rows(s2, offs2, nregions, nfe, n, nrows, nfe / xunit_mid)
    fill_stack_acts(s1, xqp, xsp, nrows * n)
    fill_stack_acts(s3, xqp, xsp, nrows * n)
    fill_ffn_meta(nrows * nfe, nrows * nfe / xunit_mid, is_gelu)
    return (cmd = ffn_cmd_for(s1, s3, s2, nrows, n, nfe), s2 = s2)
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
    unsafe {   // metas are ours to pin: these stacks serve no other decode GEMV
        fill_stack_meta_rows(sq, addr(qreg[0]), 1l, n, dq, 1l, 0l)
        fill_stack_meta_rows(sk, addr(kreg[0]), 1l, n, dk, 1l, 0l, dq)
        fill_stack_meta_rows(sv, addr(vreg[0]), 1l, n, dv, 1l, 0l, dq + dk)
    }
    var qc = QkvCmd(cmd = alloc_cmd(), sq = sq, ybytes = (dq + dk + dv) * 4l)
    assert(qc.ybytes <= Y_BYTES, "dasLLAMA vulkan tier: qkv group exceeds output capacity")
    // three disjoint row levels on the shared y (Y1/Y2/Y3): k/v sets ride the q stack's acts image
    qc.k_set = vk_set6(g_gpu.stacks[sk].wqbuf, g_gpu.stacks[sk].wsbuf, g_gpu.stacks[sk].meta.buf,
        g_gpu.stacks[sq].xq.buf, g_gpu.stacks[sq].xs.buf, g_gpu.stacks[sq].y_dev,
        g_gpu.stacks[sk].wqbytes, g_gpu.stacks[sk].wsbytes, META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES,
        fixed_array(0u, 0u, 0u, 0u, 0u, VHG_Y2))
    qc.v_set = vk_set6(g_gpu.stacks[sv].wqbuf, g_gpu.stacks[sv].wsbuf, g_gpu.stacks[sv].meta.buf,
        g_gpu.stacks[sq].xq.buf, g_gpu.stacks[sq].xs.buf, g_gpu.stacks[sq].y_dev,
        g_gpu.stacks[sv].wqbytes, g_gpu.stacks[sv].wsbytes, META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES,
        fixed_array(0u, 0u, 0u, 0u, 0u, VHG_Y3))
    let raw = qc.cmd
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    let fq = g_gpu.stacks[sq].fmt
    let fk = g_gpu.stacks[sk].fmt
    let fv = g_gpu.stacks[sv].fmt
    // the q stack's raw_set plays Y1 in this chain (role-fluid, re-declared per record)
    hz_set_bits(g_gpu.stacks[sq].raw_set, 0u, 0u, 0u, 0u, 0u, VHG_Y1)
    var h : VkHaz
    hz_dispatch(raw, h, gemv_pipe(fq), g_gpu.stacks[sq].raw_set, uint((dq + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
    hz_dispatch(raw, h, gemv_pipe(fk), qc.k_set, uint((dk + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
    hz_dispatch(raw, h, gemv_pipe(fv), qc.v_set, uint((dv + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
    vhz_dep(raw, h, VHG_Y1 | VHG_Y2 | VHG_Y3, 0u, true)
    cmd_copy_whole(raw, g_gpu.stacks[sq].y_dev, g_gpu.stacks[sq].y.buf, qc.ybytes)
    vk_check(vkEndCommandBuffer(raw), null)
    g_gpu.qkv_cmds |> insert(woq, qc)
}

//! MoeGpuQkvFn: one attention layer's q/k/v decode projections as ONE submit. The three planes
//! must share a quant CLASS (one activation image); the caller's gate enforces it.
def vk_moe_qkv(var yq : float?; var yk : float?; var yv : float?; woq, wok, wov : int64;
               fq, fk, fv : int; xqp : int8 const?; xsp : float const?; n, dq, dk, dv : int64) {
    let sq = find_stack(woq, fq)
    let sk = find_stack(wok, fk)
    let sv = find_stack(wov, fv)
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
    if (src1 < 0 || src3 < 0 || src2 < 0 || vk_fmt_b32(f3) != vk_fmt_b32(f1) || want <= 0l) {
        let why = (want <= 0l ? "want 0"
            : (src1 < 0 || src3 < 0 || src2 < 0 ? "no mirror src {src1}/{src3}/{src2} for b {b1}/{b3}/{b2} fmt {f1}/{f3}/{f2}"
            : "gate/up form mismatch fmt {f1}/{f3}"))
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
    pool.slot_expert |> resize(int(c))
    for (i in range(int(c))) {
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
    if (rbs > 0l) {   // q8n mirrors carry no scale plane
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

// One-time dispatch state for a resident classifier plane: the meta never changes (ONE region
// covering the whole plane, activations at image row 0), so it uploads device-local once and
// the command buffer records once — per token only the activation refresh and the submit remain.
def private ensure_cls(si : int; woff, n, d : int64) {
    if (g_gpu.stacks[si].cls_made) {
        return
    }
    ensure_gemv_scratch(si)
    // fmt_unit, not `fmt != 0`: a q8n cls plane (fmt 6, retagged on q8n boxes) counts 32-blocks
    let unit = fmt_unit(g_gpu.stacks[si].fmt)
    let rel = woff - g_gpu.stacks[si].base
    assert(rel % unit == 0l, "dasLLAMA vulkan tier: unaligned classifier plane offset")
    fill_gemv_meta(g_gpu.staging, n, d, 0l, rel / unit)
    g_gpu.stacks[si].cls_meta_dev = make_device_buf(CLS_META_BYTES)
    staged_upload(g_gpu.stacks[si].cls_meta_dev, 0l, CLS_META_BYTES)
    g_gpu.stacks[si].cls_set = vk_set6(g_gpu.stacks[si].wqbuf, g_gpu.stacks[si].wsbuf, g_gpu.stacks[si].cls_meta_dev,
        g_gpu.stacks[si].xq.buf, g_gpu.stacks[si].xs.buf, g_gpu.stacks[si].y_dev,
        g_gpu.stacks[si].wqbytes, g_gpu.stacks[si].wsbytes, CLS_META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES,
        fixed_array(0u, 0u, 0u, 0u, 0u, VHG_YO))
    var raw = alloc_cmd()
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    hz_dispatch(raw, h, gemv_pipe(g_gpu.stacks[si].fmt), g_gpu.stacks[si].cls_set,
        uint((d + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
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
    let si = find_stack(woff, fmt)
    assert(d * 4l <= Y_BYTES, "dasLLAMA vulkan tier: classifier vocab exceeds output capacity")
    ensure_cls(si, woff, n, d)
    fill_stack_acts(si, xqp, xsp, n)
    submit_wait(g_gpu.stacks[si].cls_cmd)
    unsafe {
        memcpy(reinterpret<void?>(yp), g_gpu.stacks[si].y.mapped, d * 4l)
    }
}
