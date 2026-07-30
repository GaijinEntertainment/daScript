options gen2
options _comment_hygiene = true

module dasllama_vulkan_prefill shared public
options _cyclomatic_complexity = 0   // batched record/setup walks — split queued for the dedup phase
options _function_length = 0   // same

require dasllama/dasllama_vulkan_common   // g_gpu/g_rd state, plumbing, hazard rail, fill metas
require dasllama/dasllama_vulkan_decode   // vk_moe_dn_step — the dn chain's npos==1 arm routes through the decode step
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_math
require dasllama/dasllama_par
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

let private PF_ROLES = 16l   // quant_xb q k v rope attn quant_at wo addr_ffn quant_xb2 gate up actrq down addr_next qkn

def private pf_bind(idx : int; b0, b1, b2, b3, b4, b5 : uint64; s0, s1, s2, s3, s4, s5 : int64; gbits : uint[6]) {
    g_rd.pf_sets[idx] = vk_set6(b0, b1, b2, b3, b4, b5, s0, s1, s2, s3, s4, s5, gbits)
}

def private pf_setup {
    if (g_rd.pf_ready) {
        return
    }
    let np = PF_WINDOW
    g_rd.pf_np = np
    let dim = g_rd.dim
    let qd = g_rd.qd
    let kvd = g_rd.kv_dim
    let hid = g_rd.hidden
    let wide = max(max(dim, qd), hid)
    g_rd.pf_x = make_device_buf(np * dim * 4l)
    g_rd.pf_xb = make_device_buf(np * dim * 4l)
    g_rd.pf_xq = make_device_buf(np * wide)
    g_rd.pf_xs = make_device_buf(np * (wide / 32l) * 4l)
    g_rd.pf_q = make_device_buf(np * qd * 4l)
    g_rd.pf_kv = make_device_buf(np * 2l * kvd * 4l)
    g_rd.pf_v = make_device_buf(np * kvd * 4l)   // v GEMM output (copied into pf_kv's v half; the batch GEMM can't offset its output independently of its input row)
    g_rd.pf_attn = make_device_buf(np * qd * 4l)
    g_rd.pf_aq = make_device_buf(np * qd)
    g_rd.pf_as = make_device_buf(np * (qd / 32l) * 4l)
    g_rd.pf_gate = make_device_buf(np * hid * 4l)
    g_rd.pf_up = make_device_buf(np * hid * 4l)
    g_rd.pf_ffnout = make_device_buf(np * dim * 4l)
    g_rd.pf_xb2 = make_device_buf(np * dim * 4l)
    g_rd.pf_cos = make_device_buf(np * g_rd.head_size * 4l)
    g_rd.pf_logits_host = make_host_buf(g_rd.vocab * 4l, false, [cached = true])
    g_rd.pf_cmd = alloc_cmd()
    ensure_batch_state()   // hq_dev/hs_dev for the FFN act
    // fmt-6 (cm2) groups feed the twin f16 rows instead of q8 images
    var any6 = false
    for (L in g_rd.layers) {
        any6 = any6 || L.fq == 6 || L.fo == 6 || L.f1 == 6 || L.f2 == 6
    }
    if (any6) {
        g_rd.pf_xf = make_device_buf(np * dim * 2l)
        g_rd.pf_af = make_device_buf(np * qd * 2l)
        g_rd.pf_hf = make_device_buf(np * hid * 2l)
    }
    let nsets = int(g_rd.n_layers * PF_ROLES + 2l)   // + prologue, final quantize of the last row
    g_rd.pf_sets |> resize(nsets)
    g_rd.pf_meta |> resize(nsets)
    for (mi in range(nsets)) {
        g_rd.pf_meta[mi] = make_host_buf(BATCH_META_BYTES, true)
    }
    let mirbytes = g_rd.n_layers * g_rd.seq_cap * kvd * 4l
    let normbytes = g_rd.norms_bytes
    for (l in range64(g_rd.n_layers)) {
        var L & = unsafe(g_rd.layers[l])
        let b = int(l * PF_ROLES)
        let pq = arena_planes(L.fq); let pk = arena_planes(L.fk); let pv = arena_planes(L.fv)
        let po = arena_planes(L.fo); let p1 = arena_planes(L.f1); let p3 = arena_planes(L.f3); let p2 = arena_planes(L.f2)
        // fmt-6 groups run on the f16 twin plane at binding 3 (idle binding-4 bits are lens-ignored)
        let qkv6 = L.fq == 6
        let xqb = qkv6 ? g_rd.pf_xf : g_rd.pf_xq
        let xsb = qkv6 ? g_rd.dummy : g_rd.pf_xs
        let xqn = qkv6 ? np * dim * 2l : np * dim
        let xsn = qkv6 ? 256l : np * (dim / 32l) * 4l
        let wo6 = L.fo == 6
        let aqb = wo6 ? g_rd.pf_af : g_rd.pf_aq
        let asb = wo6 ? g_rd.dummy : g_rd.pf_as
        let aqn = wo6 ? np * qd * 2l : np * qd
        let asn = wo6 ? 256l : np * (qd / 32l) * 4l
        let gu6 = L.f1 == 6
        let gqb = gu6 ? g_rd.pf_xf : g_rd.pf_xq
        let gsb = gu6 ? g_rd.dummy : g_rd.pf_xs
        let gqn = gu6 ? np * dim * 2l : np * dim
        let gsn = gu6 ? 256l : np * (dim / 32l) * 4l
        let dn6 = L.f2 == 6
        let hqb = dn6 ? g_rd.pf_hf : g_gpu.hq_dev
        let hsb = dn6 ? g_rd.dummy : g_gpu.hs_dev
        let hqn = dn6 ? np * hid * 2l : np * hid
        let hsn = dn6 ? 256l : np * (hid / 32l) * 4l
        // 0 quant_xb: pf_xb -> pf_xq/pf_xs (fmt 6: f16 convert -> pf_xf)
        pf_bind(b + 0, g_rd.pf_xb, g_rd.dummy, g_rd.pf_meta[b + 0].buf, xqb, xsb, g_rd.dummy,
            np * dim * 4l, 256l, BATCH_META_BYTES, xqn, xsn, 256l,
            fixed_array(VHZ_XB, 0u, 0u, VHZ_XQ, VHZ_XQ, 0u))
        // 1/2/3 q/k/v batch GEMM: arena planes, pf_xq/pf_xs act, -> pf_q / pf_kv(k@0) / pf_kv(v@np*kvd)
        pf_bind(b + 1, pq.wq, pq.ws, g_rd.pf_meta[b + 1].buf, xqb, xsb, g_rd.pf_q,
            pq.wqb, pq.wsb, BATCH_META_BYTES, xqn, xsn, np * qd * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_Q))
        pf_bind(b + 2, pk.wq, pk.ws, g_rd.pf_meta[b + 2].buf, xqb, xsb, g_rd.pf_kv,
            pk.wqb, pk.wsb, BATCH_META_BYTES, xqn, xsn, np * 2l * kvd * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVK))
        pf_bind(b + 3, pv.wq, pv.ws, g_rd.pf_meta[b + 3].buf, xqb, xsb, g_rd.pf_v,
            pv.wqb, pv.wsb, BATCH_META_BYTES, xqn, xsn, np * kvd * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_VST))
        // 4 rope+store: q@0 in place, Kmir@1, Vmir@3, cos@4, kv@5
        pf_bind(b + 4, g_rd.pf_q, g_rd.k_mirror, g_rd.pf_meta[b + 4].buf, g_rd.v_mirror, g_rd.pf_cos, g_rd.pf_kv,
            np * qd * 4l, mirbytes, BATCH_META_BYTES, mirbytes, np * g_rd.head_size * 4l, np * 2l * kvd * 4l,
            fixed_array(VHZ_Q, VHZ_MIR, 0u, VHZ_MIR, VHZ_COS, VHZ_KVK | VHZ_KVV))
        // 5 attn: q@0, Kmir@1, Vmir@3, scale@4, out@5
        pf_bind(b + 5, g_rd.pf_q, g_rd.k_mirror, g_rd.pf_meta[b + 5].buf, g_rd.v_mirror, g_rd.scal_dev, g_rd.pf_attn,
            np * qd * 4l, mirbytes, BATCH_META_BYTES, mirbytes, 64l, np * qd * 4l,
            fixed_array(VHZ_Q, VHZ_MIR, 0u, VHZ_MIR, 0u, VHZ_ATT))
        // 6 quant_at: pf_attn -> pf_aq/pf_as (fmt 6: f16 convert -> pf_af)
        pf_bind(b + 6, g_rd.pf_attn, g_rd.dummy, g_rd.pf_meta[b + 6].buf, aqb, asb, g_rd.dummy,
            np * qd * 4l, 256l, BATCH_META_BYTES, aqn, asn, 256l,
            fixed_array(VHZ_ATT, 0u, 0u, VHZ_AQ, VHZ_AQ, 0u))
        // 7 wo GEMM -> pf_xb2
        pf_bind(b + 7, po.wq, po.ws, g_rd.pf_meta[b + 7].buf, aqb, asb, g_rd.pf_xb2,
            po.wqb, po.wsb, BATCH_META_BYTES, aqn, asn, np * dim * 4l,
            fixed_array(0u, 0u, 0u, VHZ_AQ, VHZ_AQ, VHZ_XB2))
        // 8 addr_ffn: x += xb2, xb = rms_ffn
        pf_bind(b + 8, g_rd.pf_x, g_rd.pf_xb2, g_rd.pf_meta[b + 8].buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.pf_xb,
            np * dim * 4l, np * dim * 4l, BATCH_META_BYTES, normbytes, 64l, np * dim * 4l,
            fixed_array(VHZ_X, VHZ_XB2, 0u, 0u, 0u, VHZ_XB))
        // 9 quant_xb2: pf_xb -> pf_xq (fmt 6: f16 convert -> pf_xf)
        pf_bind(b + 9, g_rd.pf_xb, g_rd.dummy, g_rd.pf_meta[b + 9].buf, gqb, gsb, g_rd.dummy,
            np * dim * 4l, 256l, BATCH_META_BYTES, gqn, gsn, 256l,
            fixed_array(VHZ_XB, 0u, 0u, VHZ_XQ, VHZ_XQ, 0u))
        // 10/11 gate/up GEMM
        pf_bind(b + 10, p1.wq, p1.ws, g_rd.pf_meta[b + 10].buf, gqb, gsb, g_rd.pf_gate,
            p1.wqb, p1.wsb, BATCH_META_BYTES, gqn, gsn, np * hid * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_GATE))
        pf_bind(b + 11, p3.wq, p3.ws, g_rd.pf_meta[b + 11].buf, gqb, gsb, g_rd.pf_up,
            p3.wqb, p3.wsb, BATCH_META_BYTES, gqn, gsn, np * hid * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_UP))
        // 12 actrq: up@0, ffn_meta@2, hq@3, hs@4, gate@5 (fmt 6: actf16 -> pf_hf@3)
        pf_bind(b + 12, g_rd.pf_up, g_rd.dummy, g_gpu.ffn_meta_dev, hqb, hsb, g_rd.pf_gate,
            np * hid * 4l, 256l, FFN_META_BYTES, dn6 ? hqn : BATCH_HQ_BYTES, dn6 ? hsn : BATCH_HS_BYTES, np * hid * 4l,
            fixed_array(VHZ_UP, 0u, VHZ_META, VHZ_HQ, VHZ_HQ, VHZ_GATE))
        // 13 down GEMM: hq/hs act -> pf_ffnout
        pf_bind(b + 13, p2.wq, p2.ws, g_rd.pf_meta[b + 13].buf, hqb, hsb, g_rd.pf_ffnout,
            p2.wqb, p2.wsb, BATCH_META_BYTES, hqn, hsn, np * dim * 4l,
            fixed_array(0u, 0u, 0u, VHZ_HQ, VHZ_HQ, VHZ_FFO))
        // 14 addr_next: x += ffnout, xb = rms_att[l+1]|final
        pf_bind(b + 14, g_rd.pf_x, g_rd.pf_ffnout, g_rd.pf_meta[b + 14].buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.pf_xb,
            np * dim * 4l, np * dim * 4l, BATCH_META_BYTES, normbytes, 64l, np * dim * 4l,
            fixed_array(VHZ_X, VHZ_FFO, 0u, 0u, 0u, VHZ_XB))
        // 15 qk rmsnorm (qk_norm models): q@0 (pf_q), norm rows@3, eps@4, k@5 (pf_kv, k at 0)
        if (g_rd.qk_norm) {
            pf_bind(b + 15, g_rd.pf_q, g_rd.dummy, g_rd.pf_meta[b + 15].buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.pf_kv,
                np * qd * 4l, 256l, BATCH_META_BYTES, normbytes, 64l, np * 2l * kvd * 4l,
                fixed_array(VHZ_Q, 0u, 0u, 0u, 0u, VHZ_KVK))
        }
    }
    let bp = int(g_rd.n_layers * PF_ROLES)
    // prologue: xb = rms_att[0](x), add off
    pf_bind(bp + 0, g_rd.pf_x, g_rd.dummy, g_rd.pf_meta[bp + 0].buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.pf_xb,
        np * dim * 4l, 256l, BATCH_META_BYTES, normbytes, 64l, np * dim * 4l,
        fixed_array(VHZ_X, 0u, 0u, 0u, 0u, VHZ_XB))
    // final quantize of the LAST row (into the decode driver's xq_dev/xs_dev) for the cls GEMV
    pf_bind(bp + 1, g_rd.pf_xb, g_rd.dummy, g_rd.pf_meta[bp + 1].buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.dummy,
        np * dim * 4l, 256l, BATCH_META_BYTES, dim, (dim / 32l) * 4l, 256l,
        fixed_array(VHZ_XB, 0u, 0u, VHZ_XQ, VHZ_XQ, 0u))
    g_rd.pf_ready = true
}

//! Resident-driver prefill: `npos` embedded rows (x_batch, [npos x dim]) + per-position rope rows
//! (cos_batch, [npos x hs]) -> the last row's logits, filling the KV mirror at positions [0, npos).
def vk_rdec_prefill(x_batch : array<float>; cos_batch : array<float>; npos : int64; var logits : array<float>) {
    assert(g_rd != null && g_rd.ready, "vk_rdec_prefill before prepare/set_cls")
    pf_setup()
    let dim = g_rd.dim
    let qd = g_rd.qd
    let kvd = g_rd.kv_dim
    let hid = g_rd.hidden
    let nlfin = g_rd.n_layers * 2l * dim
    let pairs = qd / 2l + kvd / 2l
    unsafe {
        var sp = reinterpret<float?>(g_gpu.staging.mapped)
        sp[0] = g_rd.scale
        staged_upload(g_rd.scal_dev, 0l, 64l)
        sp[0] = g_rd.eps
        sp[1] = 1.0
        staged_upload(g_rd.ar_scal_dev, 0l, 64l)
        // prompts over the PF_WINDOW activation buffers run as sequential windows: each window's
        // rope/attention address the mirror at absolute positions, so window w attends everything
        // [0, w0 + row] the earlier windows stored. Only the last window runs fin_rq + cls.
        var w0 = 0l
        while (w0 < npos) {
            let tp0 = ref_time_ticks()
            let wlen = min(g_rd.pf_np, npos - w0)
            let last = w0 + wlen >= npos
            upload_region_at(g_rd.pf_x, 0l, addr<uint8 const?>(x_batch[int(w0 * dim)]), wlen * dim * 4l)
            upload_region_at(g_rd.pf_cos, 0l, addr<uint8 const?>(cos_batch[int(w0 * g_rd.head_size)]), wlen * g_rd.head_size * 4l)
            fill_ffn_meta(wlen * hid, wlen * hid / 32l, false)   // act(rq|f16) over the window's rows
            // fill metas
            let mA = @(idx : int; n, d, blk : int64; fmt : int) {
                fill_arena_batch_meta(g_rd.pf_meta[idx], n, d, blk, wlen, fmt)
            }
            let mQ = @(idx : int; blocks : int64) { fill_quant_meta(g_rd.pf_meta[idx], blocks) }
            let mC = @(idx : int; elems : int64) { fill_cvt_meta(g_rd.pf_meta[idx], elems) }
            let mR = @(idx : int; woff : int64; add : bool) { fill_addrms_meta(g_rd.pf_meta[idx], dim, woff, add) }
            for (l in range64(g_rd.n_layers)) {
                var L & = g_rd.layers[l]
                let b = int(l * PF_ROLES)
                if (L.fq == 6) {
                    invoke(mC, b + 0, wlen * dim)
                } else {
                    invoke(mQ, b + 0, wlen * dim / 32l)
                }
                invoke(mA, b + 1, dim, qd, L.bq, L.fq)
                invoke(mA, b + 2, dim, kvd, L.bk, L.fk)
                invoke(mA, b + 3, dim, kvd, L.bv, L.fv)
                if (g_rd.qk_norm) {
                    let hs = g_rd.head_size
                    var mn = reinterpret<uint?>(g_rd.pf_meta[b + 15].mapped)
                    mn[0] = uint(hs); mn[1] = uint(g_rd.n_heads); mn[2] = uint(kvd / hs)
                    mn[3] = uint(qd); mn[4] = uint(kvd); mn[5] = 0u   // k rows at 0 in pf_kv, kvd stride
                    mn[6] = uint(nlfin + dim + l * 2l * hs)
                    mn[7] = uint(nlfin + dim + (l * 2l + 1l) * hs)
                }
                var mr = reinterpret<uint?>(g_rd.pf_meta[b + 4].mapped)
                mr[0] = uint(qd); mr[1] = uint(kvd); mr[2] = uint(g_rd.head_size); mr[3] = uint(g_rd.head_size / 2l)
                mr[4] = g_rd.neox ? 1u : 0u; mr[5] = uint(qd / 2l); mr[6] = uint(pairs)
                mr[7] = uint(l * g_rd.seq_cap * kvd); mr[9] = uint(wlen); mr[10] = uint(w0); mr[11] = uint(wlen * kvd)
                var ma = reinterpret<uint?>(g_rd.pf_meta[b + 5].mapped)
                ma[0] = uint(wlen); ma[1] = uint(g_rd.n_heads); ma[2] = uint(kvd); ma[3] = uint(g_rd.head_size); ma[4] = uint(g_rd.kv_mul)
                ma[5] = uint(l * g_rd.seq_cap * kvd); ma[6] = uint(l * g_rd.seq_cap * kvd); ma[7] = uint(w0)
                if (L.fo == 6) {
                    invoke(mC, b + 6, wlen * qd)
                } else {
                    invoke(mQ, b + 6, wlen * qd / 32l)
                }
                invoke(mA, b + 7, qd, dim, L.bo, L.fo)
                invoke(mR, b + 8, (l * 2l + 1l) * dim, true)
                if (L.f1 == 6) {
                    invoke(mC, b + 9, wlen * dim)
                } else {
                    invoke(mQ, b + 9, wlen * dim / 32l)
                }
                invoke(mA, b + 10, dim, hid, L.b1, L.f1)
                invoke(mA, b + 11, dim, hid, L.b3, L.f3)
                invoke(mA, b + 13, hid, dim, L.b2, L.f2)
                let nxt = l + 1l < g_rd.n_layers ? (l + 1l) * 2l * dim : nlfin
                invoke(mR, b + 14, nxt, true)
            }
            let bp = int(g_rd.n_layers * PF_ROLES)
            invoke(mR, bp + 0, 0l, false)   // prologue norm
            if (last) {
                // final quantize reads the LAST row of pf_xb
                var mf = reinterpret<uint?>(g_rd.pf_meta[bp + 1].mapped)
                mf[10] = uint((wlen - 1l) * dim); mf[18] = uint(dim / 32l)
            }
            let t_prep = get_time_usec(tp0)
            // record
            var raw = g_rd.pf_cmd
            let rf : VkCommandBufferResetFlags
            vk_check(vkResetCommandBuffer(raw, rf), null)
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            pfq_begin(raw)
            pfq_ts(raw)   // q0: submit start
            var h : VkHaz
            vhz_dep(raw, h, 0u, VHZ_META, true)
            cmd_copy_whole(raw, g_gpu.ffn_meta.buf, g_gpu.ffn_meta_dev, FFN_META_BYTES)
            pfq_ts(raw)   // q1: meta copy
            let ropewg = uint((wlen * pairs + int64(WG_X) - 1l) / int64(WG_X))
            let fa128 = g_gpu.has_coopmat && g_rd.head_size == 128l && vk_fa_on()
            let attnwg = uint(fa128 ? g_rd.n_heads * ((wlen + 31l) / 32l) : g_rd.n_heads * ((wlen + 7l) / 8l))
            rd_dispatch(raw, g_gpu.ar_pipeline_b, g_rd.pf_sets[bp + 0], uint(wlen), h)   // prologue norm
            for (l in range64(g_rd.n_layers)) {
                let b = int(l * PF_ROLES)
                // fmt-6 (cm2) roles: f32->f16 convert feeds the decode-in-load twin; the fill's fmt
                // must match the dispatched pipe's tile rule, so both key off the same group flag
                let fq6 = g_rd.layers[l].fq == 6
                let fo6 = g_rd.layers[l].fo == 6
                let gu6 = g_rd.layers[l].f1 == 6
                let dn6 = g_rd.layers[l].f2 == 6
                rd_dispatch(raw, weak_copy(fq6 ? g_gpu.f16cvt_pipeline : g_gpu.dn_rq_pipeline), g_rd.pf_sets[b + 0], uint((wlen * dim / 4l + 255l) / 256l), h)
                rd_dispatch(raw, pf_gemm_pipe(fq6, qd, wlen), g_rd.pf_sets[b + 1], fill_arena_batch_meta(g_rd.pf_meta[b + 1], dim, qd, g_rd.layers[l].bq, wlen, fq6 ? 6 : 0), h)
                rd_dispatch(raw, pf_gemm_pipe(fq6, kvd, wlen), g_rd.pf_sets[b + 2], fill_arena_batch_meta(g_rd.pf_meta[b + 2], dim, kvd, g_rd.layers[l].bk, wlen, fq6 ? 6 : 0), h)
                rd_dispatch(raw, pf_gemm_pipe(fq6, kvd, wlen), g_rd.pf_sets[b + 3], fill_arena_batch_meta(g_rd.pf_meta[b + 3], dim, kvd, g_rd.layers[l].bv, wlen, fq6 ? 6 : 0), h)
                vhz_dep(raw, h, VHZ_VST, VHZ_KVV, true)
                cmd_copy_range(raw, g_rd.pf_v, 0l, g_rd.pf_kv, wlen * kvd * 4l, wlen * kvd * 4l)   // v into pf_kv's v half
                if (g_rd.qk_norm) {
                    rd_dispatch(raw, g_gpu.qkn_pipeline, g_rd.pf_sets[b + 15],
                        uint(wlen * (g_rd.n_heads + kvd / g_rd.head_size)), h)
                }
                rd_dispatch(raw, g_gpu.rks_b_pipeline, g_rd.pf_sets[b + 4], ropewg, h)
                rd_dispatch(raw, weak_copy(fa128 ? g_gpu.da_b128_pipeline : g_gpu.da_b_pipeline), g_rd.pf_sets[b + 5], attnwg, h)
                rd_dispatch(raw, weak_copy(fo6 ? g_gpu.f16cvt_pipeline : g_gpu.dn_rq_pipeline), g_rd.pf_sets[b + 6], uint((wlen * qd / 4l + 255l) / 256l), h)
                rd_dispatch(raw, pf_gemm_pipe(fo6, dim, wlen), g_rd.pf_sets[b + 7], fill_arena_batch_meta(g_rd.pf_meta[b + 7], qd, dim, g_rd.layers[l].bo, wlen, fo6 ? 6 : 0), h)
                rd_dispatch(raw, g_gpu.ar_pipeline_b, g_rd.pf_sets[b + 8], uint(wlen), h)
                rd_dispatch(raw, weak_copy(gu6 ? g_gpu.f16cvt_pipeline : g_gpu.dn_rq_pipeline), g_rd.pf_sets[b + 9], uint((wlen * dim / 4l + 255l) / 256l), h)
                rd_dispatch(raw, pf_gemm_pipe(gu6, hid, wlen), g_rd.pf_sets[b + 10], fill_arena_batch_meta(g_rd.pf_meta[b + 10], dim, hid, g_rd.layers[l].b1, wlen, gu6 ? 6 : 0), h)
                rd_dispatch(raw, pf_gemm_pipe(gu6, hid, wlen), g_rd.pf_sets[b + 11], fill_arena_batch_meta(g_rd.pf_meta[b + 11], dim, hid, g_rd.layers[l].b3, wlen, gu6 ? 6 : 0), h)
                rd_dispatch(raw, weak_copy(dn6 ? g_gpu.actf16_pipeline : g_gpu.actrq_pipeline), g_rd.pf_sets[b + 12], uint((wlen * hid / 4l + 255l) / 256l), h)
                rd_dispatch(raw, pf_gemm_pipe(dn6, dim, wlen), g_rd.pf_sets[b + 13], fill_arena_batch_meta(g_rd.pf_meta[b + 13], hid, dim, g_rd.layers[l].b2, wlen, dn6 ? 6 : 0), h)
                rd_dispatch(raw, g_gpu.ar_pipeline_b, g_rd.pf_sets[b + 14], uint(wlen), h)
            }
            if (last) {
                rd_dispatch(raw, g_gpu.dn_rq_pipeline, g_rd.pf_sets[bp + 1], uint((dim / 4l + 255l) / 256l), h)
                rd_dispatch_gemv(raw, g_rd.cls_fmt_marker, g_rd.cls_set, rd_gemv_wg(g_rd.vocab), h)
                vhz_dep(raw, h, VHZ_LOG, 0u, true)
                cmd_copy_whole(raw, g_rd.logits_dev, g_rd.pf_logits_host.buf, g_rd.vocab * 4l)
            }
            if (vk_prof()) {
                to_log(LOG_INFO, "vk_rdpf hz: {h.disp} nodes, {h.barriers} barriers\n")
            }
            pfq_end(raw)
            vk_check(vkEndCommandBuffer(raw), null)
            let t_record = get_time_usec(tp0) - t_prep
            submit_wait(raw)
            if (vk_prof()) {
                to_log(LOG_INFO, "vk_rdpf w0 {w0} rows {wlen} prep {t_prep} record {t_record} submit {get_time_usec(tp0) - t_prep - t_record}\n")
            }
            w0 += wlen
        }
        memcpy(addr<void?>(logits[0]), g_rd.pf_logits_host.mapped, g_rd.vocab * 4l)
        // per-role GPU aggregation (q0 start, q1 meta, q2 prologue, rpl/layer, final rq, cls), only when
        // every stamp landed (deep graphs overflow PFQ_CAP). Multi-window prompts: LAST window only.
        // Overlapped levels (q|k|v, gate|up) put the whole level's cost on the LAST member's delta.
        let rpl = g_rd.qk_norm ? 16l : 15l
        if (vk_prof() && g_pfq_n == 5u + uint(g_rd.n_layers * rpl)) {
            var role : double[16]
            for (l in range64(g_rd.n_layers)) {
                for (s in range(int(rpl))) {
                    role[s] += pfq_us(uint(3l + l * rpl) + uint(s))
                }
            }
            let nq = 3u + uint(g_rd.n_layers * rpl)
            var tot = 0.0lf
            for (s in range(int(rpl))) {
                tot += role[s]
            }
            tot += pfq_us(2u) + pfq_us(nq) + pfq_us(nq + 1u)
            // the qkn dispatch stamps between v and rope_kv, shifting every later role by one
            if (g_rd.qk_norm) {
                to_log(LOG_INFO, "vk_rdpf gpu prologue {int(pfq_us(2u))} rq_x {int(role[0])} q {int(role[1])} k {int(role[2])} v {int(role[3])} qkn {int(role[4])} rope_kv {int(role[5])} attn {int(role[6])} rq_a {int(role[7])} wo {int(role[8])} ar1 {int(role[9])} rq_f {int(role[10])} gate {int(role[11])} up {int(role[12])} act {int(role[13])} down {int(role[14])} ar2 {int(role[15])} fin_rq {int(pfq_us(nq))} cls {int(pfq_us(nq + 1u))} total {int(tot)}\n")
            } else {
                to_log(LOG_INFO, "vk_rdpf gpu prologue {int(pfq_us(2u))} rq_x {int(role[0])} q {int(role[1])} k {int(role[2])} v {int(role[3])} rope_kv {int(role[4])} attn {int(role[5])} rq_a {int(role[6])} wo {int(role[7])} ar1 {int(role[8])} rq_f {int(role[9])} gate {int(role[10])} up {int(role[11])} act {int(role[12])} down {int(role[13])} ar2 {int(role[14])} fin_rq {int(pfq_us(nq))} cls {int(pfq_us(nq + 1u))} total {int(tot)}\n")
            }
        }
    }
}

//! Arena seam: one dense FFN layer against arena-resident planes — gate GEMV, up GEMV, fused
//! act+requant, down GEMV, ONE submit. `blk1/blk3/blk2` are gate/up/down arena block indices.
//! Gate and up must share a format class (they share the activation image); down is free.
def vk_arena_ffn(var y : array<float>; blk1, blk3, blk2 : int64; n, nfe : int64;
                 f1, f3, f2 : int; xq : array<int8>; xs : array<float>; is_gelu : bool) {
    verify(vk_moe_init())
    assert(vk_fmt_b32(f3) == vk_fmt_b32(f1), "dasLLAMA vulkan arena: gate/up form mismatch — one activation image")
    assert(nfe * 4l <= Y_BYTES && n * 4l <= Y_BYTES, "dasLLAMA vulkan arena: ffn planes exceed y capacity")
    ensure_af_state(f1, f3, f2)
    let xunit = fmt_unit(f1)
    let dkq = !vk_fmt_b32(f2)
    fill_af_meta(g_gpu.af_meta_g, n, nfe, blk1)
    fill_af_meta(g_gpu.af_meta_u, n, nfe, blk3)
    fill_af_meta(g_gpu.af_meta_d, nfe, n, blk2)
    fill_ffn_meta(nfe, nfe / (dkq ? 256l : 32l), is_gelu)
    unsafe {
        memcpy(g_gpu.af_xq.mapped, addr<void?>(xq[0]), n)
        memcpy(g_gpu.af_xs.mapped, addr<void?>(xs[0]), (n / xunit) * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        vhz_dep(raw, h, 0u, VHG_META, true)
        cmd_copy_whole(raw, g_gpu.ffn_meta.buf, g_gpu.ffn_meta_dev, FFN_META_BYTES)
        let gwg = uint((nfe + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg)
        hz_dispatch(raw, h, gemv_pipe(f1), g_gpu.af_gate_set, gwg)
        hz_dispatch(raw, h, gemv_pipe(f3), g_gpu.af_up_set, gwg)
        // the requant form follows the DOWN plane — its GEMV consumes hq/hs
        hz_dispatch(raw, h, weak_copy(dkq ? g_gpu.actrq_k_pipeline : g_gpu.actrq_pipeline),
            g_gpu.af_actrq_set, uint((nfe / (dkq ? 8l : 4l) + 255l) / 256l))
        hz_dispatch(raw, h, gemv_pipe(f2), g_gpu.af_down_set, uint((n + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.af_yo_dev, g_gpu.af_yo_host.buf, n * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(y[0]), g_gpu.af_yo_host.mapped, n * 4l)
    }
}

// single-region batch-GEMM meta over an arena block: all tiles map to region 0. Returns the
// dispatch workgroup count. Mirrors fill_batch_meta for one region [0, npos).
def private fill_arena_batch_meta(hb : HostBuf; n, d, blk, npos : int64; fmt : int) : uint {
    let tile = fmt == 0 ? q8_batch_tile_for(d, npos) : BATCH_TILE
    // the cm2 twin tiles 64 weight-rows x 128 tokens per workgroup (fill_batch_meta's rule)
    let tile_d = fmt == 6 ? 64l : tile
    let tile_c = fmt == 6 ? 128l : tile
    let wtiles = (d + tile_d - 1l) / tile_d
    let rtiles = (npos + tile_c - 1l) / tile_c
    let total = rtiles * wtiles
    assert((8l + total) * 4l <= BATCH_META_BYTES, "dasLLAMA vulkan arena: batch meta exceeds capacity")
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = 1u          // one region
        mp[3] = 8u          // map_off = 4 + nrc*4
        mp[4] = uint(blk)   // arena block base (already in fmt units)
        mp[5] = 0u          // rb0 - r0
        mp[6] = uint(npos)  // cnt
        mp[7] = 0u          // wg base
        for (g in range64(total)) {
            mp[8l + g] = 0u
        }
    }
    return uint(total)
}

def private arena_ensure_batch(fmt : int) {
    unsafe {
        var a & = g_gpu.arenas[fmt]
        if (a.batch_ready) {
            return
        }
        ensure_batch_state()
        a.batch_meta = make_host_buf(BATCH_META_BYTES, true)
        a.batch_set = vk_set6(a.wqbuf, a.wsbuf, a.batch_meta.buf, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, g_gpu.batch_y1_dev,
            a.wqbytes, a.wsbytes, BATCH_META_BYTES, BATCH_XQ_BYTES, BATCH_XS_BYTES, BATCH_Y_BYTES,
            fixed_array(0u, 0u, 0u, 0u, 0u, VHG_YO))   // meta/acts fill pre-cmd
        a.batch_ready = true
    }
}

//! Arena seam: batch GEMM of the [d x n] tensor at block `blk` against `npos` quantized activation
//! rows (xq/xs, [npos x n]) into y[0..npos*d). The prefill twin of vk_arena_gemv (q8 only for now).
def vk_arena_gemm(var y : array<float>; blk : int64; n, d, npos : int64; fmt : int; xq : array<int8>; xs : array<float>) {
    verify(vk_moe_init())
    assert(fmt == 0, "dasLLAMA vulkan arena_gemm: q8 only")
    assert(npos * n <= BATCH_XQ_BYTES && npos * d * 4l <= BATCH_Y_BYTES, "arena_gemm: batch exceeds scratch")
    arena_ensure_batch(fmt)
    let groups = fill_arena_batch_meta(g_gpu.arenas[fmt].batch_meta, n, d, blk, npos, fmt)
    unsafe {
        upload_region_at(g_gpu.batch_xq_dev, 0l, addr<uint8 const?>(xq[0]), npos * n)
        upload_region_at(g_gpu.batch_xs_dev, 0l, addr<uint8 const?>(xs[0]), npos * (n / 32l) * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        hz_dispatch(raw, h, q8_batch_pipe_for(d, npos), g_gpu.arenas[fmt].batch_set, groups)
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, npos * d * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(y[0]), g_gpu.batch_y1.mapped, npos * d * 4l)
    }
}

//! Arena seam: one GEMV of the [d x n] tensor at block index `blk` into y[0..d). Same kernels and
//! meta layout as the per-op tier — only the weight base is an arena index, not stack-relative.
def vk_arena_gemv(var y : array<float>; blk : int64; n, d : int64; fmt : int; xq : array<int8>; xs : array<float>) {
    assert(g_gpu != null && key_exists(g_gpu.arenas, fmt), "dasLLAMA vulkan arena: format not reserved")
    var yp = unsafe(addr(y[0]))
    let xqp = unsafe(addr(xq[0]))
    let xsp = unsafe(addr(xs[0]))
    arena_ensure_scratch(fmt)
    let unit = fmt_unit(fmt)   // q8 AND q8n take 32-block Q8_0 activations
    let nb = n / unit
    assert(n <= XQ_BYTES && nb * 4l <= XS_BYTES, "dasLLAMA vulkan arena: activations exceed scratch capacity")
    assert(d * 4l <= Y_BYTES, "dasLLAMA vulkan arena: gemv output exceeds y capacity")
    unsafe {
        var a & = g_gpu.arenas[fmt]
        var mp = reinterpret<uint?>(a.meta.mapped)
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = 1u          // one region: the whole tensor
        mp[3] = 0u          // y base row
        mp[4] = uint(blk)   // THE arena base — a stack-relative offset in the per-op tier
        mp[5] = 0u          // activation block base
        memcpy(a.xq.mapped, reinterpret<void?>(xqp), n)   // one int8 quant per weight
        memcpy(a.xs.mapped, reinterpret<void?>(xsp), nb * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        hz_dispatch(raw, h, gemv_pipe(fmt), a.raw_set, uint((d + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, a.y_dev, a.y.buf, d * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(reinterpret<void?>(yp), a.y.mapped, d * 4l)
    }
}

// ===== prefill batch dispatch (the FFN's batched-GEMM arm) =====


// hazard bits for the cm2 chain's two f16 planes
let private VHB_XF = 0x8000u      // f16 gathered-activation plane (dequant out, gate/up twin B)
let private VHB_HF = 0x10000u     // f16 hidden plane (actf16 out, down twin B)

// cm2 batch state: the two f16 planes (2 bytes per element — 2x the q8 image caps bound the
// same chunk rows) and the dequant/act convert sets. Mode 4 + fmt-6 stacks only.
def private ensure_cm2_batch {
    if (g_gpu.cm2_batch_ready) {
        return
    }
    ensure_batch_state()
    g_gpu.batch_xf_dev = make_device_buf(2l * BATCH_XQ_BYTES)
    g_gpu.batch_hf_dev = make_device_buf(2l * BATCH_HQ_BYTES)
    // dequant bindings: f16 out@0, params@2, xq/xs@3/4; 1/5 unused filler
    g_gpu.batch_xf_set = vk_set6(g_gpu.batch_xf_dev, g_gpu.batch_xs_dev, g_gpu.ffn_meta_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, g_gpu.batch_y1_dev,
        2l * BATCH_XQ_BYTES, BATCH_XS_BYTES, FFN_META_BYTES, BATCH_XQ_BYTES, BATCH_XS_BYTES, BATCH_Y_BYTES,
        fixed_array(VHB_XF, 0u, VHB_META, VHB_ACT, VHB_ACT, 0u))
    // act-f16 bindings mirror actrq: up@0, params@2, f16 hidden out@3, gate@5; 1/4 unused filler
    g_gpu.batch_hf_set = vk_set6(g_gpu.batch_y2_dev, g_gpu.batch_xs_dev, g_gpu.ffn_meta_dev, g_gpu.batch_hf_dev, g_gpu.hs_dev, g_gpu.batch_y1_dev,
        BATCH_Y_BYTES, BATCH_XS_BYTES, FFN_META_BYTES, 2l * BATCH_HQ_BYTES, BATCH_HS_BYTES, BATCH_Y_BYTES,
        fixed_array(VHB_Y2, 0u, VHB_META, VHB_HF, 0u, VHB_Y1))
    g_gpu.resident_bytes += 2l * BATCH_XQ_BYTES + 2l * BATCH_HQ_BYTES
    g_gpu.cm2_batch_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: cm2 prefill chain engaged (decode-in-load, f16 activations)\n")
}

def private ensure_comb_state {
    if (g_gpu.comb_ready) {
        return
    }
    g_gpu.comb_stage = make_host_buf(2l * COMB_WI_BYTES, false)   // transfer_src staging: w then inv
    g_gpu.comb_w_dev = make_device_buf(COMB_WI_BYTES)
    g_gpu.comb_inv_dev = make_device_buf(COMB_WI_BYTES)
    g_gpu.comb_y_dev = make_device_buf(BATCH_Y_BYTES)
    g_gpu.comb_y = make_host_buf(BATCH_Y_BYTES, true, [cached = true])
    // combine bindings: output@0, params@2, bucket map@3, weights@4, down rows@5; binding 1 unused
    g_gpu.comb_set = vk_set6(g_gpu.comb_y_dev, g_gpu.comb_w_dev, g_gpu.ffn_meta_dev, g_gpu.comb_inv_dev, g_gpu.comb_w_dev, g_gpu.batch_y1_dev,
        BATCH_Y_BYTES, COMB_WI_BYTES, FFN_META_BYTES, COMB_WI_BYTES, COMB_WI_BYTES, BATCH_Y_BYTES,
        fixed_array(VHB_CY, VHB_COMB, VHB_META, VHB_COMB, VHB_COMB, VHB_Y1))
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
        g_gpu.stacks[si].batch_meta = make_host_buf(BATCH_META_BYTES, false)   // transfer_src staging
        g_gpu.stacks[si].batch_meta_dev = make_device_buf(BATCH_META_BYTES)
        g_gpu.stacks[si].batch_set = alloc_desc_set()
        g_gpu.stacks[si].batch_made = true
    }
    vk_write6(g_gpu.stacks[si].batch_set,
        g_gpu.stacks[si].wqbuf, g_gpu.stacks[si].wsbuf, g_gpu.stacks[si].batch_meta_dev, xqbuf, xsbuf, ybuf,
        g_gpu.stacks[si].wqbytes, ws_range(si), BATCH_META_BYTES, xqbytes, xsbytes, ybytes,
        fixed_array(0u, 0u, VHB_META, xbit, xbit, ybit))
    g_gpu.stacks[si].batch_y = ybuf
    g_gpu.stacks[si].batch_xq = xqbuf
}

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
    var qregions : array<BufferCopy>
    qregions |> push(BufferCopy(srcOffset = 0ul, dstOffset = 0ul, size = uint64(xq_bytes)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(g_gpu.staging.buf), nonowning_buf(g_gpu.batch_xq_dev), qregions)
    var sregions : array<BufferCopy>
    sregions |> push(BufferCopy(srcOffset = uint64(BATCH_XQ_BYTES), dstOffset = 0ul, size = uint64(xs_bytes)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(g_gpu.staging.buf), nonowning_buf(g_gpu.batch_xs_dev), sregions)
}

// build the chunk's meta table: regions clipped to the row window [r0, r1) (row0 re-based to the
// window), then the per-workgroup region map. Weight offsets follow the stack's format units —
// 32-weight blocks for q8, superblocks for kq. Returns the workgroup count.
def private fill_batch_meta(si : int; offs : int64 const?; nregions : int64; n, d : int64; r0, r1 : int64) : uint {
    let fmt = g_gpu.stacks[si].fmt
    let unit = fmt_unit(fmt)
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.stacks[si].batch_meta.mapped)
        var nrc = 0l
        var nrows = 0l
        var all128 = true   // every active region's cnt a 128-multiple -> aligned-L eligible
        for (r in range64(nregions)) {
            let b0 = max(offs[r * 3l + 1l], r0)
            let b1 = min(offs[r * 3l + 1l] + offs[r * 3l + 2l], r1)
            if (b1 > b0) {
                nrc++
                nrows += b1 - b0
                all128 = all128 && (b1 - b0) % 128l == 0l
            }
        }
        // route the tile on the AVERAGE rows per active region; the cm2 twin tiles 64w x 128t
        let tile = fmt == 0 ? q8_batch_tile_for(d, nrc > 0l ? nrows / nrc : r1 - r0) : BATCH_TILE
        let tile_d = fmt == 6 ? 64l : tile
        let tile_c = fmt == 6 ? 128l : tile
        g_gpu.stacks[si].batch_tile = tile
        g_gpu.stacks[si].batch_aligned = fmt == 0 && tile == 128l && d % 128l == 0l && all128
        let wtiles = (d + tile_d - 1l) / tile_d
        let wg_bound = ((r1 - r0 + tile_c - 1l) / tile_c + nrc) * wtiles
        assert((4l + nrc * 4l + wg_bound) * 4l <= BATCH_META_BYTES, "dasLLAMA vulkan tier: batch meta exceeds capacity")
        let map_off = 4l + nrc * 4l
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = uint(nrc)
        mp[3] = uint(map_off)
        var ri = 0l
        var wg = 0l
        for (r in range64(nregions)) {
            let rb0 = max(offs[r * 3l + 1l], r0)
            let rb1 = min(offs[r * 3l + 1l] + offs[r * 3l + 2l], r1)
            if (rb1 <= rb0) {
                continue
            }
            let rel = offs[r * 3l] - g_gpu.stacks[si].base
            assert(rel % unit == 0l, "dasLLAMA vulkan tier: unaligned batch region weight offset")
            let mb = 4l + ri * 4l
            mp[mb] = uint(rel / unit)
            mp[mb + 1l] = uint(rb0 - r0)
            mp[mb + 2l] = uint(rb1 - rb0)
            mp[mb + 3l] = uint(wg)
            let rwg = ((rb1 - rb0 + tile_c - 1l) / tile_c) * wtiles
            for (g in range64(rwg)) {
                mp[map_off + wg + g] = uint(ri)
            }
            wg += rwg
            ri++
        }
        return uint(wg)
    }
}

def private cmd_copy_batch_meta(raw : VkCommandBuffer; si : int) {
    var regions : array<BufferCopy>
    regions |> push(BufferCopy(srcOffset = 0ul, dstOffset = 0ul, size = uint64(BATCH_META_BYTES)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(g_gpu.stacks[si].batch_meta.buf),
                    nonowning_buf(g_gpu.stacks[si].batch_meta_dev), regions)
}

def private cmd_copy_at(raw : VkCommandBuffer; src : uint64; src_off : int64; dst : uint64; bytes : int64) {
    var regions : array<BufferCopy>
    regions |> push(BufferCopy(srcOffset = uint64(src_off), dstOffset = 0ul, size = uint64(bytes)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(src), nonowning_buf(dst), regions)
}

// one reusable cmd buffer, re-recorded per submit — prefill submits are few and multi-ms, so
// recording cost is noise. comb_npos > 0: down rows stay device-side, accumulating into comb_y_dev;
// only the last chunk appends the k-fold-smaller combined DMA. Hidden planes never leave the device.
def private record_ffn_batch_cmd(s1, s3, s2 : int; nwg_gu, nwg_dn : uint; rows, nfe, gout_bytes : int64; axq_bytes, axs_bytes : int64;
                                 comb_npos : uint = 0u; comb_first : bool = false; comb_last : bool = false;
                                 comb_wi_bytes : int64 = 0l; comb_y_bytes : int64 = 0l) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    // one declaration per same-region copy GROUP — per-copy would fake WAW on the shared bit
    vhz_dep(raw, h, 0u, VHB_ACT, true)
    cmd_copy_batch_acts(raw, axq_bytes, axs_bytes)
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, s1)
    cmd_copy_batch_meta(raw, s3)
    cmd_copy_batch_meta(raw, s2)
    cmd_copy_whole(raw, g_gpu.ffn_meta.buf, g_gpu.ffn_meta_dev, FFN_META_BYTES)
    if (comb_npos > 0u && comb_first) {
        vhz_dep(raw, h, 0u, VHB_COMB, true)
        cmd_copy_whole(raw, g_gpu.comb_stage.buf, g_gpu.comb_w_dev, comb_wi_bytes)
        cmd_copy_at(raw, g_gpu.comb_stage.buf, COMB_WI_BYTES, g_gpu.comb_inv_dev, comb_wi_bytes)
    }
    if (g_gpu.stacks[s1].fmt == 6) {   // the cm2 chain: dequant feed, twin GEMMs, f16 mid-step
        hz_dispatch(raw, h, weak_copy(g_gpu.xf16_pipeline), g_gpu.batch_xf_set,
            uint((axq_bytes / 4l + 255l) / 256l))
    }
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s1].fmt, g_gpu.stacks[s1].batch_tile, g_gpu.stacks[s1].batch_aligned),
        g_gpu.stacks[s1].batch_set, nwg_gu)
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s3].fmt, g_gpu.stacks[s3].batch_tile, g_gpu.stacks[s3].batch_aligned),
        g_gpu.stacks[s3].batch_set, nwg_gu)
    // the mid-step form follows the DOWN stack — its GEMM is the consumer of the hidden plane
    if (g_gpu.stacks[s2].fmt == 6) {
        hz_dispatch(raw, h, weak_copy(g_gpu.actf16_pipeline), g_gpu.batch_hf_set,
            uint((rows * nfe / 4l + 255l) / 256l))
    } else {
        let dkq = g_gpu.stacks[s2].fmt != 0
        hz_dispatch(raw, h, weak_copy(dkq ? g_gpu.actrq_k_pipeline : g_gpu.actrq_pipeline),
            g_gpu.batch_actrq_set, uint((rows * nfe / (dkq ? 8l : 4l) + 255l) / 256l))
    }
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s2].fmt, g_gpu.stacks[s2].batch_tile, g_gpu.stacks[s2].batch_aligned),
        g_gpu.stacks[s2].batch_set, nwg_dn)
    if (comb_npos > 0u) {
        hz_dispatch(raw, h, weak_copy(g_gpu.comb_pipeline), g_gpu.comb_set, comb_npos)
        if (comb_last) {
            vhz_dep(raw, h, VHB_CY, 0u, true)
            cmd_copy_whole(raw, g_gpu.comb_y_dev, g_gpu.comb_y.buf, comb_y_bytes)
        }
    } else {
        vhz_dep(raw, h, VHB_Y1, 0u, true)
        cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, gout_bytes)
    }
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_ffnb hz: {h.disp} nodes, {h.barriers} barriers\n")
    }
    vk_check(vkEndCommandBuffer(raw), null)
}

// batch arm: chunked over row windows, down GEMM reuses y1 for output. npos > 0: the combine
// kernel folds each chunk's down rows into comb_y_dev, read back once after the last chunk.
def vk_moe_ffn_batch(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    let s1 = find_stack(unsafe(offs1[0]), f1)
    let s3 = find_stack(unsafe(offs3[0]), f3)
    let s2 = find_stack(unsafe(offs2[0]), f2)
    // gate/up share one activation image (forms must agree); down's intermediate is its own unit
    let kq_in = !vk_fmt_b32(f1)
    assert(!vk_fmt_b32(f3) == kq_in,
        "dasLLAMA vulkan tier: gate/up form mismatch — they share one activation image")
    let xunit_in = fmt_unit(f1)
    let xunit_mid = fmt_unit(f2)
    ensure_batch_state()
    if (f1 == 6 || f2 == 6) {
        ensure_cm2_batch()   // any twin in the chain needs the f16 planes + convert sets
    }
    // a q8n gate/up pair loads the dequantized f16 plane as B; a q8n down loads the f16 hidden
    if (f1 == 6) {
        ensure_batch_set(s1, g_gpu.batch_y1_dev, g_gpu.batch_xf_dev, g_gpu.batch_xs_dev, 2l * BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_XF, VHB_Y1)
        ensure_batch_set(s3, g_gpu.batch_y2_dev, g_gpu.batch_xf_dev, g_gpu.batch_xs_dev, 2l * BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_XF, VHB_Y2)
    } else {
        ensure_batch_set(s1, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
        ensure_batch_set(s3, g_gpu.batch_y2_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y2)
    }
    if (f2 == 6) {
        ensure_batch_set(s2, g_gpu.batch_y1_dev, g_gpu.batch_hf_dev, g_gpu.hs_dev, 2l * BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HF, VHB_Y1)
    } else {
        ensure_batch_set(s2, g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.hs_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HQ, VHB_Y1)
    }
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
    let chunk = ffn_chunk_rows(n, nfe, xunit_in, xunit_mid)
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
        let ts2 = ref_time_ticks()
        fill_ffn_meta(rows * nfe, rows * nfe / xunit_mid, is_gelu, comb ? n : 0l, comb_k, r0, r1,
            f1 == 6 ? rows * n / 4l : 0l)
        record_ffn_batch_cmd(s1, s3, s2, nwg1, nwg2, rows, nfe, rows * n * 4l, rows * n, rows * (n / xunit_in) * 4l,
            [comb_npos = comb ? uint(npos) : 0u, comb_first = r0 == 0l, comb_last = r1 == nrows,
             comb_wi_bytes = nrows * 4l, comb_y_bytes = npos * n * 4l])
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
        unsafe {
            par_memcpy(reinterpret<void?>(goutp), g_gpu.comb_y.mapped, npos * n * 4l)
        }
    }
}

// one dense chunk: acts + meta staging copies, one tile GEMM over the single whole-plane
// region, y DMA
def private record_dense_cmd(si : int; nwg : uint; y_bytes : int64; axq_bytes, axs_bytes : int64) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    vhz_dep(raw, h, 0u, VHB_ACT, true)
    cmd_copy_batch_acts(raw, axq_bytes, axs_bytes)
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, si)
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[si].fmt, g_gpu.stacks[si].batch_tile, g_gpu.stacks[si].batch_aligned),
        g_gpu.stacks[si].batch_set, nwg)
    vhz_dep(raw, h, VHB_Y1, 0u, true)
    cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, y_bytes)
    vk_check(vkEndCommandBuffer(raw), null)
}

//! MoeGpuDenseFn: dense batched GEMM — y [nrows x d] = X · Wᵀ over the resident plane
//! ([d x n] at element offset woff), a one-region ride on the batch tile rail. Chunked like
//! the FFN arm; activations arrive pre-quantized in the plane's own form.
def vk_moe_dense(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d, nrows : int64; fmt : int) {
    let si = find_stack(woff, fmt)
    let xunit = fmt_unit(fmt)
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
        var nwg = 0u
        unsafe {
            nwg = fill_batch_meta(si, addr(region[0]), 1l, n, d, r0, r1)
        }
        let ts1 = ref_time_ticks()
        upload_batch_acts(xqp, xsp, r0, rows, n, xunit)
        let ts2 = ref_time_ticks()
        record_dense_cmd(si, nwg, rows * d * 4l, rows * n, rows * (n / xunit) * 4l)
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



def private fill_dn_meta(npos_w, cd, kd, di, nvh, nkh, ds, dconv : int64; hist_mode : int; nblocks : int64) {
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.dn_meta.mapped)
        mp[0] = uint(npos_w)
        mp[1] = uint(cd)
        mp[2] = uint(kd)
        mp[3] = uint(di)
        mp[4] = uint(nvh)
        mp[5] = uint(nkh)
        mp[6] = uint(ds)
        mp[7] = uint(dconv)
        mp[8] = uint(hist_mode)
        mp[9] = uint(DN_SLAB)
        mp[10] = uint(DN_O_OFF)
        mp[11] = uint(DN_SM_BETA)
        mp[12] = uint(DN_SM_G)
        mp[13] = uint(DN_SM_TAPS)
        mp[14] = uint(DN_SM_WNORM)
        mp[15] = uint(DN_SM_A)
        mp[16] = uint(DN_SM_DT)
        mp[17] = uint(DN_SM_HIST)
        mp[18] = uint(nblocks)
        mp[19] = uint(DN_P2_OFF)
    }
}

// one deltanet window: acts/meta/smalls staging copies, qkv + z tiles, conv, scan, the conv-tail
// extraction (before out-proj overwrites the qkv rows in y1), o requant, out-proj tile, y DMA;
// the state uploads on the first window and reads back on the last
def private record_dn_cmd(s_qkv, s_z, s_o : int; nwg_qkv, nwg_z, nwg_out : uint; rows, cd, di, dim, taps, nvh, ds : int64;
                          axq_bytes, axs_bytes : int64; first, last : bool) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    pfq_begin(raw)
    pfq_ts(raw)   // q0: submit start
    vhz_dep(raw, h, 0u, VHB_ACT, true)
    cmd_copy_batch_acts(raw, axq_bytes, axs_bytes)
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, s_qkv)
    cmd_copy_batch_meta(raw, s_z)
    cmd_copy_batch_meta(raw, s_o)
    cmd_copy_whole(raw, g_gpu.dn_meta.buf, g_gpu.dn_meta_dev, DN_META_BYTES)
    // stops SHORT of the hist region — later windows read the device-copied tail rows there
    vhz_dep(raw, h, 0u, VHB_SM, true)
    cmd_copy_range(raw, g_gpu.dn_smalls.buf, 0l, g_gpu.dn_smalls_dev, 0l, DN_SM_HIST * 4l)
    if (first) {
        cmd_copy_range(raw, g_gpu.dn_smalls.buf, DN_SM_HIST * 4l, g_gpu.dn_smalls_dev, DN_SM_HIST * 4l, cd * taps * 4l)
        vhz_dep(raw, h, 0u, VHB_ST, true)
        cmd_copy_whole(raw, g_gpu.dn_state_stage.buf, g_gpu.dn_state_dev, nvh * ds * ds * 4l)
    }
    pfq_ts(raw)   // q1: uploads
    hz_dispatch(raw, h, batch_pipe(0, g_gpu.stacks[s_qkv].batch_tile, g_gpu.stacks[s_qkv].batch_aligned),
        g_gpu.stacks[s_qkv].batch_set, nwg_qkv)
    hz_dispatch(raw, h, batch_pipe(0, g_gpu.stacks[s_z].batch_tile, g_gpu.stacks[s_z].batch_aligned),
        g_gpu.stacks[s_z].batch_set, nwg_z)
    pfq_ts(raw)   // q2: qkv+z GEMMs
    hz_dispatch(raw, h, weak_copy(g_gpu.dn_conv_pipeline), g_gpu.dn_conv_set, uint(rows))
    pfq_ts(raw)   // q3: conv
    hz_dispatch(raw, h, weak_copy(g_gpu.dn_scan_p1_pipeline), g_gpu.dn_scan_set, uint(nvh * ((rows + DN_CS - 1l) / DN_CS)))
    pfq_ts(raw)   // q4: scan phase 1
    hz_dispatch(raw, h, weak_copy(g_gpu.dn_scan_p2_pipeline), g_gpu.dn_scan_set, uint(nvh))
    pfq_ts(raw)   // q5: scan phase 2
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
    hz_dispatch(raw, h, weak_copy(g_gpu.dn_rq_pipeline), g_gpu.dn_rq_set, uint((rows * di / 4l + 255l) / 256l))
    pfq_ts(raw)   // q7: o requant
    hz_dispatch(raw, h, batch_pipe(0, g_gpu.stacks[s_o].batch_tile, g_gpu.stacks[s_o].batch_aligned),
        g_gpu.stacks[s_o].batch_set, nwg_out)
    pfq_ts(raw)   // q8: out GEMM
    vhz_dep(raw, h, VHB_Y1, 0u, true)
    cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, rows * dim * 4l)
    pfq_end(raw)
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_dn hz: {h.disp} nodes, {h.barriers} barriers\n")
    }
    vk_check(vkEndCommandBuffer(raw), null)
}

//! MoeGpuDnFn: one recurrent layer's whole deltanet attention block as one submit chain per
//! DN_WINDOW rows. yp gets npos x dim rows; tailp gets the last taps qkv rows for the caller's history store.
def vk_moe_dn(var yp : float?; woq, woz, woo : int64;
              xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
              convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
              var stp : float?; var tailp : float?; histp : float const?;
              npos, dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    let s_qkv = find_stack(woq, 0)
    let s_z = find_stack(woz, 0)
    let s_o = find_stack(woo, 0)
    let taps = dconv - 1l
    if (npos == 1l) {   // the decode step — device-resident state, one pre-recorded submit
        vk_moe_dn_step(yp, s_qkv, s_z, s_o, woq, woz, woo, xqp, xsp, bp, gp,
            convw, wnormp, ap, dtp, stp, histp, dim, cd, kd, di, nvh, nkh, ds, dconv, eps)
        return
    }
    assert(ds <= DN_MAX_DS && ds % 16l == 0l && nvh <= DN_MAX_HEADS && cd <= DN_MAX_CD
           && dconv <= DN_MAX_DCONV && di == nvh * ds && kd == nkh * ds && cd == 2l * kd + di
           && npos > taps && dim % 32l == 0l && di % 32l == 0l,
        "dasLLAMA vulkan tier: deltanet geometry outside chain capacity")
    ensure_batch_state()
    ensure_dn_state()
    ensure_batch_set(s_qkv, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
    ensure_batch_set(s_z, g_gpu.batch_y2_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y2)
    ensure_batch_set(s_o, g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.hs_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HQ, VHB_Y1)
    unsafe {
        var sp = reinterpret<float?>(g_gpu.dn_smalls.mapped)
        sp[0] = eps
        sp[1] = 1.0 / sqrt(float(ds))
        memcpy(reinterpret<void?>(sp + DN_SM_TAPS), reinterpret<void?>(convw), cd * dconv * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_WNORM), reinterpret<void?>(wnormp), ds * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_A), reinterpret<void?>(ap), nvh * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_DT), reinterpret<void?>(dtp), nvh * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_HIST), reinterpret<void?>(histp), cd * taps * 4l)
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
        upload_batch_acts(xqp, xsp, r0, rows, dim, 32l)
        unsafe {
            var sp = reinterpret<float?>(g_gpu.dn_smalls.mapped)
            par_memcpy(reinterpret<void?>(sp + DN_SM_BETA), reinterpret<void?>(bp + r0 * nvh), rows * nvh * 4l)
            par_memcpy(reinterpret<void?>(sp + DN_SM_G), reinterpret<void?>(gp + r0 * nvh), rows * nvh * 4l)
        }
        fill_dn_meta(rows, cd, kd, di, nvh, nkh, ds, dconv, r0 == 0l ? 1 : 2, rows * di / 32l)
        let ts1 = ref_time_ticks()
        record_dn_cmd(s_qkv, s_z, s_o, nwg_qkv, nwg_z, nwg_out, rows, cd, di, dim, taps, nvh, ds,
            rows * dim, rows * (dim / 32l) * 4l, r0 == 0l, r0 + rows == npos)
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
    g_gpu.at_meta = make_host_buf(AT_META_BYTES, false)
    g_gpu.at_meta_dev = make_device_buf(AT_META_BYTES)
    g_gpu.at_smalls = make_host_buf(AT_SMALLS_BYTES, false)
    g_gpu.at_smalls_dev = make_device_buf(AT_SMALLS_BYTES)
    g_gpu.at_q_dev = make_device_buf(AT_WINDOW * AT_MAX_QD * 4l)
    g_gpu.at_g_dev = make_device_buf(AT_WINDOW * AT_MAX_QD * 4l)
    g_gpu.at_k_dev = make_device_buf(AT_CTX * AT_MAX_KV * 4l)
    g_gpu.at_v_dev = make_device_buf(AT_CTX * AT_MAX_KV * 4l)
    g_gpu.at_vraw_dev = make_device_buf(AT_WINDOW * AT_MAX_KV * 4l)
    g_gpu.at_kv_host = make_host_buf(2l * AT_WINDOW * AT_MAX_KV * 4l, true, [cached = true])
    // prep-q bindings: q|gate GEMM rows@5, q out@0, gate out@3, smalls@4, params@2; 1 filler
    g_gpu.at_prep_q_set = vk_set6(g_gpu.at_q_dev, g_gpu.at_meta_dev, g_gpu.at_meta_dev, g_gpu.at_g_dev, g_gpu.at_smalls_dev, g_gpu.batch_y1_dev,
        AT_WINDOW * AT_MAX_QD * 4l, AT_META_BYTES, AT_META_BYTES, AT_WINDOW * AT_MAX_QD * 4l, AT_SMALLS_BYTES, BATCH_Y_BYTES,
        fixed_array(VHB_ATQ, 0u, VHB_META, VHB_ATG, VHB_SM, VHB_Y1))
    // prep-k: roped k out@0 lands at ABSOLUTE positions; the gate view@3 never fires for a k row
    g_gpu.at_prep_k_set = vk_set6(g_gpu.at_k_dev, g_gpu.at_meta_dev, g_gpu.at_meta_dev, g_gpu.at_meta_dev, g_gpu.at_smalls_dev, g_gpu.batch_y2_dev,
        AT_CTX * AT_MAX_KV * 4l, AT_META_BYTES, AT_META_BYTES, AT_META_BYTES, AT_SMALLS_BYTES, BATCH_Y_BYTES,
        fixed_array(VHB_ATK, 0u, VHB_META, 0u, VHB_SM, VHB_Y2))   // 3 = the never-fired gate view
    // attn bindings: q@0, k@1, gate in/o out@3, smalls@4, v@5, params@2
    g_gpu.at_attn_set = vk_set6(g_gpu.at_q_dev, g_gpu.at_k_dev, g_gpu.at_meta_dev, g_gpu.at_g_dev, g_gpu.at_smalls_dev, g_gpu.at_v_dev,
        AT_WINDOW * AT_MAX_QD * 4l, AT_CTX * AT_MAX_KV * 4l, AT_META_BYTES, AT_WINDOW * AT_MAX_QD * 4l, AT_SMALLS_BYTES, AT_CTX * AT_MAX_KV * 4l,
        fixed_array(VHB_ATQ, VHB_ATK, VHB_META, VHB_ATG, VHB_SM, VHB_ATV))
    // requant bindings: o rows@0, quant words@3, scales@4 (wo GEMM activation image), params@2
    g_gpu.at_rq_set = vk_set6(g_gpu.at_g_dev, g_gpu.at_meta_dev, g_gpu.at_meta_dev, g_gpu.hq_dev, g_gpu.hs_dev, g_gpu.at_meta_dev,
        AT_WINDOW * AT_MAX_QD * 4l, AT_META_BYTES, AT_META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, AT_META_BYTES,
        fixed_array(VHB_ATG, 0u, VHB_META, VHB_HQ, VHB_HQ, 0u))
    g_gpu.at_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: attention chain engaged\n")
}

def private fill_at_meta(rows, w0, qd, kv_dim, hs, half, n_heads, n_kv_heads, kv_mul, nblocks : int64; gated, qknorm : bool) {
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.at_meta.mapped)
        mp[0] = uint(rows)
        mp[1] = uint(w0)
        mp[2] = uint(qd)
        mp[3] = uint(kv_dim)
        mp[4] = uint(hs)
        mp[5] = uint(half)
        mp[6] = uint(n_heads)
        mp[7] = uint(n_kv_heads)
        mp[8] = uint(kv_mul)
        mp[9] = uint((gated ? 1 : 0) | (qknorm ? 2 : 0))
        mp[10] = 0u
        mp[11] = uint(hs / 32l)
        mp[12] = uint(w0 + rows)
        mp[13] = uint(AT_SM_COS + rows * half)
        mp[14] = uint(AT_SM_RMSQ)
        mp[15] = uint(AT_SM_RMSK)
        mp[16] = uint(AT_SM_COS)
        mp[17] = 0u
        mp[18] = uint(nblocks)
    }
}

// one attention window: acts/meta/smalls staging copies, q|gate + k + v tiles, the prep pair
// (deinterleave/qk-rms/rope; k lands at absolute positions), v append + k/v host DMA in the
// transfer window, the flash attention pass, o requant, wo tile, y DMA
def private record_at_cmd(s_q, s_k, s_v, s_o : int; nwg_q, nwg_k, nwg_v, nwg_o : uint;
                          rows, w0, qd, kv_dim, dim, n_heads : int64;
                          axq_bytes, axs_bytes, sm_bytes : int64; o_kq : bool) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    vhz_dep(raw, h, 0u, VHB_ACT, true)
    cmd_copy_batch_acts(raw, axq_bytes, axs_bytes)
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, s_q)
    cmd_copy_batch_meta(raw, s_k)
    cmd_copy_batch_meta(raw, s_v)
    cmd_copy_batch_meta(raw, s_o)
    cmd_copy_whole(raw, g_gpu.at_meta.buf, g_gpu.at_meta_dev, AT_META_BYTES)
    vhz_dep(raw, h, 0u, VHB_SM, true)
    cmd_copy_range(raw, g_gpu.at_smalls.buf, 0l, g_gpu.at_smalls_dev, 0l, sm_bytes)
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s_q].fmt, g_gpu.stacks[s_q].batch_tile, g_gpu.stacks[s_q].batch_aligned),
        g_gpu.stacks[s_q].batch_set, nwg_q)
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s_k].fmt, g_gpu.stacks[s_k].batch_tile, g_gpu.stacks[s_k].batch_aligned),
        g_gpu.stacks[s_k].batch_set, nwg_k)
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s_v].fmt, g_gpu.stacks[s_v].batch_tile, g_gpu.stacks[s_v].batch_aligned),
        g_gpu.stacks[s_v].batch_set, nwg_v)
    hz_dispatch(raw, h, weak_copy(g_gpu.at_prep_q_pipeline), g_gpu.at_prep_q_set, uint(rows))
    hz_dispatch(raw, h, weak_copy(g_gpu.at_prep_k_pipeline), g_gpu.at_prep_k_set, uint(rows))
    // transfer window: append raw v at absolute positions, DMA roped k/raw v to host
    vhz_dep(raw, h, VHB_VRAW, VHB_ATV, true)
    cmd_copy_range(raw, g_gpu.at_vraw_dev, 0l, g_gpu.at_v_dev, w0 * kv_dim * 4l, rows * kv_dim * 4l)
    vhz_dep(raw, h, VHB_ATK, 0u, true)
    cmd_copy_range(raw, g_gpu.at_k_dev, w0 * kv_dim * 4l, g_gpu.at_kv_host.buf, 0l, rows * kv_dim * 4l)
    vhz_dep(raw, h, VHB_VRAW, 0u, true)
    cmd_copy_range(raw, g_gpu.at_vraw_dev, 0l, g_gpu.at_kv_host.buf, AT_WINDOW * AT_MAX_KV * 4l, rows * kv_dim * 4l)
    hz_dispatch(raw, h, weak_copy(g_gpu.at_attn_pipeline), g_gpu.at_attn_set, uint(n_heads * ((rows + 7l) / 8l)))
    hz_dispatch(raw, h, weak_copy(o_kq ? g_gpu.q8k_rq_pipeline : g_gpu.dn_rq_pipeline),
        g_gpu.at_rq_set, uint((rows * qd / (o_kq ? 8l : 4l) + 255l) / 256l))
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s_o].fmt, g_gpu.stacks[s_o].batch_tile, g_gpu.stacks[s_o].batch_aligned),
        g_gpu.stacks[s_o].batch_set, nwg_o)
    vhz_dep(raw, h, VHB_Y1, 0u, true)
    cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, rows * dim * 4l)
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_attn hz: {h.disp} nodes, {h.barriers} barriers\n")
    }
    vk_check(vkEndCommandBuffer(raw), null)
}

//! MoeGpuAttnFn: one full-attention layer's whole softmax attention block as one submit chain per
//! AT_WINDOW rows. yp gets npos x dim rows; kp/vp the roped-k / raw-v rows for the caller's cache store.
//! K/V persist device-side at absolute positions, so later windows attend the whole batch.
def vk_moe_attn(var yp : float?; var kp : float?; var vp : float?;
                woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                xqp : int8 const?; xsp : float const?;
                rmsqp : float const?; rmskp : float const?;
                cosp : float const?; sinp : float const?;
                npos, dim, qd, kv_dim, hs, rot, n_heads, kv_mul : int64;
                scale : float; eps : float; gated : bool; qknorm : bool) {
    let s_q = find_stack(woq, fq)
    let s_k = find_stack(wok, fk)
    let s_v = find_stack(wov, fv)
    let s_o = find_stack(woo, fo)
    // q/k/v share one activation image, so their format class must agree (the FFN triple rule);
    // the wo stack is free — the requant form follows it
    let kq = fq != 0
    assert((fk != 0) == kq && (fv != 0) == kq,
        "dasLLAMA vulkan tier: q/k/v stack format class mismatch in one attention chain")
    let half = rot / 2l
    let n_kv = kv_dim / hs
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
        fill_at_meta(rows, r0, qd, kv_dim, hs, half, n_heads, n_kv, kv_mul,
            rows * qd / (fo != 0 ? 256l : 32l), gated, qknorm)
        let ts1 = ref_time_ticks()
        record_at_cmd(s_q, s_k, s_v, s_o, nwg_q, nwg_k, nwg_v, nwg_o,
            rows, r0, qd, kv_dim, dim, n_heads,
            rows * dim, rows * (dim / xunit) * 4l, (AT_SM_COS + 2l * rows * half) * 4l, fo != 0)
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
    g_gpu.stream_slots |> reserve(2)
    for (_slot in range(2)) {
        let s1 = make_stack_shell(-1l, 0l, 0, g_gpu.stream_max_gu_wq, g_gpu.stream_max_gu_ws, [xfer_shared = true])
        let s3 = make_stack_shell(-1l, 0l, 0, g_gpu.stream_max_gu_wq, g_gpu.stream_max_gu_ws, [xfer_shared = true])
        let s2 = make_stack_shell(-1l, 0l, 0, g_gpu.stream_max_dn_wq, g_gpu.stream_max_dn_ws, [xfer_shared = true])
        g_gpu.stream_slots |> push(StreamSlot(group = -1, s1 = s1, s3 = s3, s2 = s2))
    }
    g_gpu.stream_ready = true
    let slot_bytes = 2l * (g_gpu.stream_max_gu_wq + g_gpu.stream_max_gu_ws) + g_gpu.stream_max_dn_wq + g_gpu.stream_max_dn_ws
    // reserve was carved from the weight budget at init — overcommit here silently hits the WDDM paging cliff
    let reserve = g_gpu.stream_reserve > 0l ? g_gpu.stream_reserve : STREAM_RESERVE
    assert(2l * slot_bytes <= reserve, "dasLLAMA vulkan tier: stream slots exceed the carved stream reserve")
    to_log(LOG_INFO, "dasLLAMA vulkan tier: streamed prefill engaged ({length(g_gpu.stream_stacks) / 3} layers, slot {slot_bytes} bytes x 2)\n")
}

// re-base one slot pseudo-stack onto a stream stack (descriptors stay — the slot buffers are
// the same; only the meta-resolution identity and the kernel choice follow the layer)
def private rebase_pseudo(si : int; ss : StreamStack const) {
    g_gpu.stacks[si].base = ss.base
    g_gpu.stacks[si].elems = ss.elems
    g_gpu.stacks[si].fmt = ss.fmt
}


// Transfer family armed: copies ride the dedicated queue signaling the timeline (true overlap;
// the slot's consumer attaches the GPU-side wait — see vk_moe_ffn_streamed). Single-queue mode
// keeps the old unfenced rail. Shared cmds are safe: a wait always intervenes between recordings.
def private copy_stream_group(si, g : int; sync : bool) {
    let slot = g_gpu.stream_slots[si]
    let xfer = g_gpu.xfer_fam >= 0
    if (xfer && g_gpu.xfer_value > 0ul) {
        xfer_host_wait(g_gpu.xfer_value)   // every prior transfer-queue copy retired
    }
    var raw = xfer ? g_gpu.xfer_cmd : g_gpu.stream_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    for (k in range(3)) {
        let sk = g * 3 + k
        let psi = k == 0 ? slot.s1 : (k == 1 ? slot.s3 : slot.s2)
        cmd_copy_whole(raw, g_gpu.stream_stacks[sk].hwq.buf, g_gpu.stacks[psi].wqbuf, g_gpu.stream_stacks[sk].wqbytes)
        if (g_gpu.stream_stacks[sk].wsbytes > 0l) {   // q8n mirrors carry no scale plane
            cmd_copy_whole(raw, g_gpu.stream_stacks[sk].hws.buf, g_gpu.stacks[psi].wsbuf, g_gpu.stream_stacks[sk].wsbytes)
        }
    }
    vk_check(vkEndCommandBuffer(raw), null)
    if (xfer) {
        g_gpu.xfer_value++
        var sig = g_gpu.xfer_value
        var tss = VkTimelineSemaphoreSubmitInfo()
        var sem = g_gpu.xfer_sem
        var submit = VkSubmitInfo()
        submit.commandBufferCount = 1u
        submit.signalSemaphoreCount = 1u
        let nofence : VkFence
        unsafe {
            tss.signalSemaphoreValueCount = 1u
            tss.pSignalSemaphoreValues = addr(sig)
            submit.pNext = addr<void?>(tss)
            submit.pCommandBuffers = addr(raw)
            submit.pSignalSemaphores = addr(sem)
            vk_check(vkQueueSubmit(g_gpu.xfer_queue, 1u, addr(submit), nofence), null)
        }
        if (sync) {
            xfer_host_wait(g_gpu.xfer_value)
        }
        // the consumer still attaches the GPU-side wait — cross-queue visibility by the book
        g_gpu.stream_slots[si].wait_value = g_gpu.xfer_value
    } elif (sync) {
        submit_wait(raw)
    } else {
        var submit = VkSubmitInfo()
        submit.commandBufferCount = 1u
        let nofence : VkFence
        unsafe {
            submit.pCommandBuffers = addr(raw)
            vk_check(vkQueueSubmit(g_gpu.queue, 1u, addr(submit), nofence), null)
        }
    }
    g_gpu.stream_slots[si].group = g
}

// prefetch miss cold-copies the slot; prefetch of the next group runs descending while prefill runs ascending
def vk_moe_ffn_streamed(g : int; var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    assert(nrows >= BATCH_TILE, "dasLLAMA vulkan tier: streamed layers serve prefill batches only")
    assert(g_gpu.stream_stacks[g * 3 + 1].fmt == f3 && g_gpu.stream_stacks[g * 3 + 2].fmt == f2,
        "dasLLAMA vulkan tier: streamed group format mismatch")
    ensure_stream_slots()
    var si = -1
    for (i in range(2)) {
        if (g_gpu.stream_slots[i].group == g) {
            si = i
        }
    }
    if (si < 0) {
        si = g % 2
        copy_stream_group(si, g, true)
    }
    rebase_pseudo(g_gpu.stream_slots[si].s1, g_gpu.stream_stacks[g * 3])
    rebase_pseudo(g_gpu.stream_slots[si].s3, g_gpu.stream_stacks[g * 3 + 1])
    rebase_pseudo(g_gpu.stream_slots[si].s2, g_gpu.stream_stacks[g * 3 + 2])
    // the slot's transfer-queue fill hands off here: the next compute submit waits the timeline
    if (g_gpu.stream_slots[si].wait_value > 0ul) {
        g_gpu.pend_wait = g_gpu.stream_slots[si].wait_value
        g_gpu.stream_slots[si].wait_value = 0ul
    }
    vk_moe_ffn_batch(goutp, offs1, offs3, offs2, nregions, xqp, xsp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    if (g > 0 && g_gpu.stream_slots[(g - 1) % 2].group != g - 1) {
        copy_stream_group((g - 1) % 2, g - 1, false)
    }
}
