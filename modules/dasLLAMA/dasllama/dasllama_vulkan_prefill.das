options gen2
options _comment_hygiene = true

module dasllama_vulkan_prefill shared public
options _cyclomatic_complexity = 0   // batched record/setup walks — split queued for the dedup phase
options _function_length = 0   // same

require dasllama/dasllama_vulkan_common   // g_gpu/g_rd state, plumbing, hazard rail, fill metas
require dasllama/dasllama_vulkan_classes   // the class-kernel rail (generated set_/enc_ + gemv_cls_*)
require dasllama/dasllama_vulkan_decode   // vk_moe_dn_step — the dn chain's npos==1 arm routes through the decode step
require dasllama/dasllama_kqformat   // KqFmt — the arena fmt slots are its ordinals
require dasllama/dasllama_gpu_tier   // moe_gpu_stream_need — the stream-slot carve's source of truth
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_math
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

// mode 4 feeds the dense q8 prefill GEMMs f16 rows through the cm2 tiles (the decode GEMV
// keeps its q8 chain — the format pick is decoupled)
def private pf_f16_feed(f : int) : bool => f == int(KqFmt.q8) && g_gpu.coopmat_mode == 4

// one prefill GEMM role on the class rail: schedule fill + a per-(role, variant) cached set +
// BatchArgs push. fmt picks the class: fmt-0 in mode 4 = the cm2 l/m tiles, kq = the kq tile
// family, else the q8 router (whose variant moves with wlen — the last window is shorter).
def private pf_gemm_enc(raw : VkCommandBuffer; var h : VkHaz; idx : int; fmt : int;
                        wq, ws, ab, sb, yb : uint64; wqn, wsn, an, sn, yn : int64;
                        abit, ybit : uint; n, d, blk, wlen : int64) {
    let cm2l = fmt == int(KqFmt.q8) && g_gpu.coopmat_mode == 4
    // cm2 tile + split picks (pure in d/wlen/n/sm_count) — computed once, fed to fill AND enc
    var nsplit = 1l
    var ksplit = 0l
    var cm2_tc = 0l
    if (cm2l) {
        cm2_tc = batch_tile_edges(fmt, d, wlen, true).tc
        let sp = cm2_split_k(d, wlen, n, cm2_tc)
        nsplit = sp.nsplit
        ksplit = sp.ksplit
    }
    let groups = int64(fill_arena_batch_sched(g_rd.pf_meta[idx], d, blk, wlen, fmt, cm2l, nsplit))
    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u, ksplit = nsplit > 1l ? uint(ksplit) : 0u)
    if (cm2l) {
        // native fmt-0 decode-in-load; a split pick reroutes the GEMM into the scratch planes
        let split = nsplit > 1l
        if (split) {
            assert(nsplit * wlen * d * 4l <= g_rd.pf_splitk_bytes, "vk prefill: split-k planes exceed the scratch bound")
        }
        let yob = split ? g_rd.pf_splitk : yb
        let yon = split ? g_rd.pf_splitk_bytes : yn
        let yog = split ? VHG_SK : ybit
        let ml = cm2_tc == 256l
        let key = idx * 16 + (ml ? (split ? 9 : 8) : (split ? 11 : 10))
        if (!key_exists(g_rd.pf_cls_gemm, key)) {
            if (ml) {
                verify(ensure_q8_batch_cm2l_cls(), "vk prefill: the cm2 l-tile class rail must engage")
                g_rd.pf_cls_gemm[key] = set_q8_batch_cm2l_cls(
                    fixed_array(wq, ws, g_rd.pf_meta[idx].buf, ab, yob),
                    fixed_array(wqn, wsn, BATCH_META_BYTES, an, yon),
                    fixed_array(0u, 0u, 0u, abit, yog))
            } else {
                verify(ensure_q8_batch_cm2m_cls(), "vk prefill: the cm2 m-tile class rail must engage")
                g_rd.pf_cls_gemm[key] = set_q8_batch_cm2m_cls(
                    fixed_array(wq, ws, g_rd.pf_meta[idx].buf, ab, yob),
                    fixed_array(wqn, wsn, BATCH_META_BYTES, an, yon),
                    fixed_array(0u, 0u, 0u, abit, yog))
            }
        }
        if (ml) {
            enc_q8_batch_cm2l_cls(raw, h, g_rd.pf_cls_gemm[key], pc, groups)
        } else {
            enc_q8_batch_cm2m_cls(raw, h, g_rd.pf_cls_gemm[key], pc, groups)
        }
        if (split) {
            let rkey = idx * 16 + 12
            if (!key_exists(g_rd.pf_cls_gemm, rkey)) {
                verify(ensure_splitk_reduce_cls(), "vk prefill: the split-k reduce class rail must engage")
                g_rd.pf_cls_gemm[rkey] = set_splitk_reduce_cls(
                    fixed_array(g_rd.pf_splitk, yb),
                    fixed_array(g_rd.pf_splitk_bytes, yn),
                    fixed_array(VHG_SK, ybit))
            }
            var pcr = SkRedArgs(nelem = uint(wlen * d), k = uint(nsplit))
            enc_splitk_reduce_cls(raw, h, g_rd.pf_cls_gemm[rkey], pcr, (wlen * d + 1_023l) / 1_024l)
        }
    } elif (kq_sb(fmt)) {
        // the role's format is static, so the slot never collides with the q8 router's variants
        let q40cm = fmt == int(KqFmt.q40) && g_gpu.coopmat_mode == 1
        let key = idx * 16 + 6
        if (!key_exists(g_rd.pf_cls_gemm, key)) {
            verify(kq_batch_cls_ensure(fmt, q40cm), "vk prefill: the kq batch class rail must engage")
            g_rd.pf_cls_gemm[key] = kq_batch_cls_set_for(fmt, q40cm,
                fixed_array(wq, ws, g_rd.pf_meta[idx].buf, ab, sb, yb),
                fixed_array(wqn, wsn, BATCH_META_BYTES, an, sn, yn),
                fixed_array(0u, 0u, 0u, abit, abit, ybit))
        }
        kq_batch_cls_enc_for(fmt, q40cm, raw, h, g_rd.pf_cls_gemm[key], pc, groups)
    } else {
        let v = q8_batch_cls_variant(q8_batch_tile_for(d, wlen), q8_batch_aligned(d, wlen))
        let key = idx * 16 + v
        if (!key_exists(g_rd.pf_cls_gemm, key)) {
            verify(q8_batch_cls_ensure(v), "vk prefill: the q8 batch class rail must engage")
            g_rd.pf_cls_gemm[key] = q8_batch_cls_set(v,
                fixed_array(wq, ws, g_rd.pf_meta[idx].buf, ab, sb, yb),
                fixed_array(wqn, wsn, BATCH_META_BYTES, an, sn, yn),
                fixed_array(0u, 0u, 0u, abit, abit, ybit))
        }
        q8_batch_cls_enc(v, raw, h, g_rd.pf_cls_gemm[key], pc, groups)
    }
    pfq_ts(raw)
}

[cold_path]   // once per residency — buffers, sets, fa arming
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
    // f16-fed groups (pf_f16_feed) take f16 convert roles; kq consumers the Q8_K quant/act forms
    var any6 = false
    var anykq = kq_sb(g_rd.cls_fmt_marker)
    for (L in g_rd.layers) {
        any6 = any6 || pf_f16_feed(L.fq) || pf_f16_feed(L.fo) || pf_f16_feed(L.f1) || pf_f16_feed(L.f2)
        anykq = anykq || kq_sb(L.fq) || kq_sb(L.fo) || kq_sb(L.f1) || kq_sb(L.f2)
    }
    if (any6) {
        g_rd.pf_xf = make_device_buf(np * dim * 2l)
        g_rd.pf_af = make_device_buf(np * qd * 2l)
        g_rd.pf_hf = make_device_buf(np * hid * 2l)
    }
    // cm2 split-k scratch: heuristic bound 128*256*2*SMs floats (tiles * nsplit <= 2 * cores)
    if (any6 && g_gpu.coopmat_mode == 4) {
        var skb = 262_144l * int64(g_gpu.sm_count)
        let fsk = cm2_splitk_env()
        if (fsk > 1l) {
            skb = max(skb, np * wide * fsk * 4l)
        }
        if (skb > 0l) {
            g_rd.pf_splitk = make_device_buf(skb)
            g_rd.pf_splitk_bytes = skb
        }
    }
    let nsets = int(g_rd.n_layers * PF_ROLES + 2l)   // + prologue, final quantize of the last row
    g_rd.pf_sets |> resize(nsets)
    g_rd.pf_meta |> resize(nsets)
    for (mi in range(nsets)) {
        g_rd.pf_meta[mi] = make_host_buf(BATCH_META_BYTES, true)
    }
    let mirbytes = g_rd.n_layers * g_rd.seq_cap * kvd * g_rd.kv_esz
    let normbytes = g_rd.norms_bytes
    let kv16 = g_rd.kv16
    // the cm2 fa tiles read f16 K/V, so f16 mirrors bind directly; f32 keeps the chunked/h128 paths
    g_rd.pf_facm2 = (g_gpu.has_coopmat2_fa && vk_fa_on() && kv16
        && (g_rd.head_size == 64l || g_rd.head_size == 128l))
    let fa128s = !g_rd.pf_facm2 && g_gpu.has_coopmat && g_rd.head_size == 128l && vk_fa_on()
    verify(ensure_cls_dn_rq() && ensure_cls_ar()
        && (kv16 ? ensure_rope_kv_b_f16_cls() : ensure_rope_kv_b_cls())
        && (fa128s ? (kv16 ? ensure_da_attn_b_h128_f16_cls() : ensure_da_attn_b_h128_cls())
            : (kv16 ? ensure_da_attn_b_f16_cls() : ensure_da_attn_b_cls()))
        && (!g_rd.pf_facm2
            || (g_rd.head_size == 64l ? ensure_fa_cm2_h64_cls() : ensure_fa_cm2_h128_cls()))
        && ensure_q8_actrq_cls() && (!g_rd.qk_norm || ensure_qk_rms_cls())
        && (!any6 || (ensure_f16cvt_cls() && ensure_actf16_cls()))
        && (!anykq || (ensure_cls_q8k_rq() && ensure_q8k_actrq_cls())),
        "vk prefill: the class rails must engage on a live device")
    if (g_rd.pf_facm2) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: cm2 flash attention armed (hs {g_rd.head_size}, native f16 mirrors)\n")
    }
    for (l in range64(g_rd.n_layers)) {
        var L & = unsafe(g_rd.layers[l])
        let b = int(l * PF_ROLES)
        let qkv6 = pf_f16_feed(L.fq)
        let wo6 = pf_f16_feed(L.fo)
        let gu6 = pf_f16_feed(L.f1)
        let dn6 = pf_f16_feed(L.f2)
        // 0 quant_xb: pf_xb -> pf_xq/pf_xs (f16-fed: f16 convert -> pf_xf)
        g_rd.pf_sets[b + 0] = (qkv6
            ? set_f16cvt_cls(fixed_array(g_rd.pf_xb, g_rd.pf_xf),
                fixed_array(np * dim * 4l, np * dim * 2l), fixed_array(VHZ_XB, VHZ_XQ))
            : set_rq_cls(fixed_array(g_rd.pf_xb, g_rd.pf_xq, g_rd.pf_xs),
                fixed_array(np * dim * 4l, np * dim, np * (dim / 32l) * 4l),
                fixed_array(VHZ_XB, VHZ_XQ, VHZ_XQ)))
        // (GEMM roles 1/2/3/7/10/11/13 build lazily per router variant at record time)
        {
            // 4 rope+store (class: q, kv, Kmir, Vmir, cos)
            let rbufs = fixed_array(g_rd.pf_q, g_rd.pf_kv, g_rd.k_mirror, g_rd.v_mirror, g_rd.pf_cos)
            let rsizes = fixed_array(np * qd * 4l, np * 2l * kvd * 4l, mirbytes, mirbytes, np * g_rd.head_size * 4l)
            let rgbits = fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, VHZ_MIR, VHZ_MIR, VHZ_COS)
            g_rd.pf_sets[b + 4] = (kv16 ? set_rope_kv_b_f16_cls(rbufs, rsizes, rgbits)
                : set_rope_kv_b_cls(rbufs, rsizes, rgbits))
        }
        // 5 attn (class: q, Kmir, Vmir, out) — the cm2 fa tile / h128 coopmat twin when they serve
        if (g_rd.pf_facm2) {
            g_rd.pf_sets[b + 5] = set_fa_cm2_cls(
                fixed_array(g_rd.pf_q, g_rd.k_mirror, g_rd.v_mirror, g_rd.pf_attn),
                fixed_array(np * qd * 4l, mirbytes, mirbytes, np * qd * 4l),
                fixed_array(VHZ_Q, VHZ_MIR, VHZ_MIR, VHZ_ATT))
        } else {
            let abufs5 = fixed_array(g_rd.pf_q, g_rd.k_mirror, g_rd.v_mirror, g_rd.pf_attn)
            let asizes5 = fixed_array(np * qd * 4l, mirbytes, mirbytes, np * qd * 4l)
            let agbits5 = fixed_array(VHZ_Q, VHZ_MIR, VHZ_MIR, VHZ_ATT)
            g_rd.pf_sets[b + 5] = (fa128s
                ? (kv16 ? set_da_attn_b_h128_f16_cls(abufs5, asizes5, agbits5) : set_da_attn_b_h128_cls(abufs5, asizes5, agbits5))
                : (kv16 ? set_da_attn_b_f16_cls(abufs5, asizes5, agbits5) : set_da_attn_b_cls(abufs5, asizes5, agbits5)))
        }
        // 6 quant_at: pf_attn -> pf_aq/pf_as (f16-fed: f16 convert -> pf_af)
        g_rd.pf_sets[b + 6] = (wo6
            ? set_f16cvt_cls(fixed_array(g_rd.pf_attn, g_rd.pf_af),
                fixed_array(np * qd * 4l, np * qd * 2l), fixed_array(VHZ_ATT, VHZ_AQ))
            : set_rq_cls(fixed_array(g_rd.pf_attn, g_rd.pf_aq, g_rd.pf_as),
                fixed_array(np * qd * 4l, np * qd, np * (qd / 32l) * 4l),
                fixed_array(VHZ_ATT, VHZ_AQ, VHZ_AQ)))
        // 8 addr_ffn (class: x, add partner, norms, xb out)
        g_rd.pf_sets[b + 8] = set_cls_ar(fixed_array(g_rd.pf_x, g_rd.pf_xb2, g_rd.norms_dev, g_rd.pf_xb),
            fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim * 4l),
            fixed_array(VHZ_X, VHZ_XB2, 0u, VHZ_XB))
        // 9 quant_xb2: pf_xb -> pf_xq (f16-fed: f16 convert -> pf_xf)
        g_rd.pf_sets[b + 9] = (gu6
            ? set_f16cvt_cls(fixed_array(g_rd.pf_xb, g_rd.pf_xf),
                fixed_array(np * dim * 4l, np * dim * 2l), fixed_array(VHZ_XB, VHZ_XQ))
            : set_rq_cls(fixed_array(g_rd.pf_xb, g_rd.pf_xq, g_rd.pf_xs),
                fixed_array(np * dim * 4l, np * dim, np * (dim / 32l) * 4l),
                fixed_array(VHZ_XB, VHZ_XQ, VHZ_XQ)))
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
        // 15 qk rmsnorm (qk_norm models; class: q, k rows, norm rows)
        if (g_rd.qk_norm) {
            g_rd.pf_sets[b + 15] = set_qk_rms_cls(fixed_array(g_rd.pf_q, g_rd.pf_kv, g_rd.norms_dev),
                fixed_array(np * qd * 4l, np * 2l * kvd * 4l, normbytes),
                fixed_array(VHZ_Q, VHZ_KVK, 0u))
        }
    }
    let bp = int(g_rd.n_layers * PF_ROLES)
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

//! Resident-driver prefill: `npos` embedded rows (x_batch, [npos x dim]) + per-position rope rows
//! (cos_batch, [npos x hs]) -> the last row's logits, filling the KV mirror at positions [0, npos).
[hot_path]   // the per-window resident prefill — encode + one submit per window
def vk_rdec_prefill(x_batch : array<float>; cos_batch : array<float>; npos : int64; var logits : array<float>) {
    assert(g_rd != null && g_rd.ready, "vk_rdec_prefill before prepare/set_cls")
    pf_setup()
    let dim = g_rd.dim
    let qd = g_rd.qd
    let kvd = g_rd.kv_dim
    let hid = g_rd.hidden
    let nlfin = g_rd.n_layers * 2l * dim
    let pairs = int(qd / 2l + kvd / 2l)
    unsafe {
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
            let bp = int(g_rd.n_layers * PF_ROLES)
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
            pfq_ts(raw)   // q1: kept for the role-aggregation stamp shape (params ride push now)
            let ropewg = (wlen * int64(pairs) + int64(WG_X) - 1l) / int64(WG_X)
            let fa128 = !g_rd.pf_facm2 && g_gpu.has_coopmat && g_rd.head_size == 128l && vk_fa_on()
            let attnwg = fa128 ? g_rd.n_heads * ((wlen + 31l) / 32l) : g_rd.n_heads * ((wlen + 7l) / 8l)
            let hs = g_rd.head_size
            var pc_pro = ArArgs(dim = uint(dim), add_on = 0u, woff = 0u, eps = g_rd.eps, ascale = 1.0)
            enc_cls_ar(raw, h, g_rd.pf_sets[bp + 0], pc_pro, wlen)   // prologue norm
            pfq_ts(raw)
            for (l in range64(g_rd.n_layers)) {
                let b = int(l * PF_ROLES)
                // f16-fed roles (fmt-6, and fmt-0 under mode 4's l-tile): f32->f16 convert feeds
                // the decode-in-load GEMM; the fill's tile rule keys off the same group flag
                let fq6 = pf_f16_feed(g_rd.layers[l].fq)
                let fo6 = pf_f16_feed(g_rd.layers[l].fo)
                let gu6 = pf_f16_feed(g_rd.layers[l].f1)
                let dn6 = pf_f16_feed(g_rd.layers[l].f2)
                // kq consumers take the Q8_K quant/act forms (the arm declines mixed groups)
                let fqk = kq_sb(g_rd.layers[l].fq)
                let fok = kq_sb(g_rd.layers[l].fo)
                let guk = kq_sb(g_rd.layers[l].f1)
                let dnk = kq_sb(g_rd.layers[l].f2)
                let mirb = l * g_rd.seq_cap * kvd   // nolint:LINT021 — the mirror offset multiply must run 64-bit before the push-field narrow
                if (fq6) {
                    var pc0 = ActArgs(nelem = uint(wlen * dim), gelu = 0u, nblk = 0u)
                    enc_f16cvt_cls(raw, h, g_rd.pf_sets[b + 0], pc0, (wlen * dim / 4l + 255l) / 256l)
                } elif (fqk) {
                    var pc0 = RqArgs(inbase = 0u, nblk = uint(wlen * dim / 256l))
                    enc_cls_q8k_rq(raw, h, g_rd.pf_sets[b + 0], pc0, (wlen * dim / 8l + 255l) / 256l)
                } else {
                    var pc0 = RqArgs(inbase = 0u, nblk = uint(wlen * dim / 32l))
                    enc_cls_dn_rq(raw, h, g_rd.pf_sets[b + 0], pc0, (wlen * dim / 4l + 255l) / 256l)
                }
                pfq_ts(raw)
                let pq = arena_planes(g_rd.layers[l].fq, g_rd.layers[l].bq)
                let pk = arena_planes(g_rd.layers[l].fk, g_rd.layers[l].bk)
                let pv = arena_planes(g_rd.layers[l].fv, g_rd.layers[l].bv)
                let np = g_rd.pf_np
                let xqb = fq6 ? g_rd.pf_xf : g_rd.pf_xq
                let xsb = fq6 ? g_rd.dummy : g_rd.pf_xs
                let xqn = fq6 ? np * dim * 2l : np * dim
                let xsn = fq6 ? 256l : np * (dim / 32l) * 4l
                pf_gemm_enc(raw, h, b + 1, g_rd.layers[l].fq, pq.wq, pq.ws, xqb, xsb, g_rd.pf_q,
                    pq.wqb, pq.wsb, xqn, xsn, np * qd * 4l, VHZ_XQ, VHZ_Q, dim, qd, g_rd.layers[l].bq, wlen)
                pf_gemm_enc(raw, h, b + 2, g_rd.layers[l].fk, pk.wq, pk.ws, xqb, xsb, g_rd.pf_kv,
                    pk.wqb, pk.wsb, xqn, xsn, np * 2l * kvd * 4l, VHZ_XQ, VHZ_KVK, dim, kvd, g_rd.layers[l].bk, wlen)
                pf_gemm_enc(raw, h, b + 3, g_rd.layers[l].fv, pv.wq, pv.ws, xqb, xsb, g_rd.pf_v,
                    pv.wqb, pv.wsb, xqn, xsn, np * kvd * 4l, VHZ_XQ, VHZ_VST, dim, kvd, g_rd.layers[l].bv, wlen)
                vhz_dep(raw, h, VHZ_VST, VHZ_KVV, true)
                cmd_copy_range(raw, g_rd.pf_v, 0l, g_rd.pf_kv, wlen * kvd * 4l, wlen * kvd * 4l)   // v into pf_kv's v half
                if (g_rd.qk_norm) {
                    var pcn = QkRmsArgs(hs = uint(hs), nh = uint(g_rd.n_heads), nkvh = uint(kvd / hs),
                        qstride = uint(qd), kstride = uint(kvd), kbase = 0u,
                        qwoff = uint(nlfin + dim + l * 2l * hs), kwoff = uint(nlfin + dim + (l * 2l + 1l) * hs),
                        eps = g_rd.eps)
                    enc_qk_rms_cls(raw, h, g_rd.pf_sets[b + 15], pcn, wlen * (g_rd.n_heads + kvd / hs))
                    pfq_ts(raw)
                }
                var pcr = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(hs / 2l),
                    neox = g_rd.neox ? 1u : 0u, kpair0 = uint(qd / 2l), ppp = uint(pairs),
                    layerbase = uint(mirb), npos = uint(wlen), pos0 = uint(w0), voff = uint(wlen * kvd))
                if (g_rd.kv16) {
                    enc_rope_kv_b_f16_cls(raw, h, g_rd.pf_sets[b + 4], pcr, ropewg)
                } else {
                    enc_rope_kv_b_cls(raw, h, g_rd.pf_sets[b + 4], pcr, ropewg)
                }
                pfq_ts(raw)
                if (g_rd.pf_facm2) {
                    // the cm2 fa tile reads the f16 mirrors directly
                    var pcf = FaCm2Args(rows = uint(wlen), w0 = uint(w0), qd = uint(qd),
                        kvd = uint(kvd), kv_mul = uint(g_rd.kv_mul), kvbase = uint(mirb),
                        scale = g_rd.scale)
                    let fawgs = g_rd.n_heads * ((wlen + 63l) / 64l)
                    if (hs == 64l) {
                        enc_fa_cm2_h64_cls(raw, h, g_rd.pf_sets[b + 5], pcf, fawgs)
                    } else {
                        enc_fa_cm2_h128_cls(raw, h, g_rd.pf_sets[b + 5], pcf, fawgs)
                    }
                } else {
                    var pca = DaAttnBArgs(npos = uint(wlen), nh = uint(g_rd.n_heads), kvd = uint(kvd),
                        hs = uint(hs), kv_mul = uint(g_rd.kv_mul), kbase = uint(mirb), vbase = uint(mirb),
                        w0 = uint(w0), scale = g_rd.scale)
                    if (fa128) {
                        if (g_rd.kv16) {
                            enc_da_attn_b_h128_f16_cls(raw, h, g_rd.pf_sets[b + 5], pca, attnwg)
                        } else {
                            enc_da_attn_b_h128_cls(raw, h, g_rd.pf_sets[b + 5], pca, attnwg)
                        }
                    } else {
                        if (g_rd.kv16) {
                            enc_da_attn_b_f16_cls(raw, h, g_rd.pf_sets[b + 5], pca, attnwg)
                        } else {
                            enc_da_attn_b_cls(raw, h, g_rd.pf_sets[b + 5], pca, attnwg)
                        }
                    }
                }
                pfq_ts(raw)
                if (fo6) {
                    var pc6 = ActArgs(nelem = uint(wlen * qd), gelu = 0u, nblk = 0u)
                    enc_f16cvt_cls(raw, h, g_rd.pf_sets[b + 6], pc6, (wlen * qd / 4l + 255l) / 256l)
                } elif (fok) {
                    var pc6 = RqArgs(inbase = 0u, nblk = uint(wlen * qd / 256l))
                    enc_cls_q8k_rq(raw, h, g_rd.pf_sets[b + 6], pc6, (wlen * qd / 8l + 255l) / 256l)
                } else {
                    var pc6 = RqArgs(inbase = 0u, nblk = uint(wlen * qd / 32l))
                    enc_cls_dn_rq(raw, h, g_rd.pf_sets[b + 6], pc6, (wlen * qd / 4l + 255l) / 256l)
                }
                pfq_ts(raw)
                let po = arena_planes(g_rd.layers[l].fo, g_rd.layers[l].bo)
                let aqb = fo6 ? g_rd.pf_af : g_rd.pf_aq
                let asb = fo6 ? g_rd.dummy : g_rd.pf_as
                let aqn = fo6 ? np * qd * 2l : np * qd
                let asn = fo6 ? 256l : np * (qd / 32l) * 4l
                pf_gemm_enc(raw, h, b + 7, g_rd.layers[l].fo, po.wq, po.ws, aqb, asb, g_rd.pf_xb2,
                    po.wqb, po.wsb, aqn, asn, np * dim * 4l, VHZ_AQ, VHZ_XB2, qd, dim, g_rd.layers[l].bo, wlen)
                var pc8 = ArArgs(dim = uint(dim), add_on = 1u, woff = uint((l * 2l + 1l) * dim), eps = g_rd.eps, ascale = 1.0)
                enc_cls_ar(raw, h, g_rd.pf_sets[b + 8], pc8, wlen)
                pfq_ts(raw)
                if (gu6) {
                    var pc9 = ActArgs(nelem = uint(wlen * dim), gelu = 0u, nblk = 0u)
                    enc_f16cvt_cls(raw, h, g_rd.pf_sets[b + 9], pc9, (wlen * dim / 4l + 255l) / 256l)
                } elif (guk) {
                    var pc9 = RqArgs(inbase = 0u, nblk = uint(wlen * dim / 256l))
                    enc_cls_q8k_rq(raw, h, g_rd.pf_sets[b + 9], pc9, (wlen * dim / 8l + 255l) / 256l)
                } else {
                    var pc9 = RqArgs(inbase = 0u, nblk = uint(wlen * dim / 32l))
                    enc_cls_dn_rq(raw, h, g_rd.pf_sets[b + 9], pc9, (wlen * dim / 4l + 255l) / 256l)
                }
                pfq_ts(raw)
                let p1 = arena_planes(g_rd.layers[l].f1, g_rd.layers[l].b1)
                let p3 = arena_planes(g_rd.layers[l].f3, g_rd.layers[l].b3)
                let gqb = gu6 ? g_rd.pf_xf : g_rd.pf_xq
                let gsb = gu6 ? g_rd.dummy : g_rd.pf_xs
                let gqn = gu6 ? np * dim * 2l : np * dim
                let gsn = gu6 ? 256l : np * (dim / 32l) * 4l
                pf_gemm_enc(raw, h, b + 10, g_rd.layers[l].f1, p1.wq, p1.ws, gqb, gsb, g_rd.pf_gate,
                    p1.wqb, p1.wsb, gqn, gsn, np * hid * 4l, VHZ_XQ, VHZ_GATE, dim, hid, g_rd.layers[l].b1, wlen)
                pf_gemm_enc(raw, h, b + 11, g_rd.layers[l].f3, p3.wq, p3.ws, gqb, gsb, g_rd.pf_up,
                    p3.wqb, p3.wsb, gqn, gsn, np * hid * 4l, VHZ_XQ, VHZ_UP, dim, hid, g_rd.layers[l].b3, wlen)
                if (dn6) {
                    var pc12 = ActArgs(nelem = uint(wlen * hid), gelu = 0u, nblk = uint(wlen * hid / 32l))
                    enc_actf16_cls(raw, h, g_rd.pf_sets[b + 12], pc12, (wlen * hid / 4l + 255l) / 256l)
                } elif (dnk) {
                    var pc12 = ActArgs(nelem = uint(wlen * hid), gelu = 0u, nblk = uint(wlen * hid / 256l))
                    enc_q8k_actrq_cls(raw, h, g_rd.pf_sets[b + 12], pc12, (wlen * hid / 8l + 255l) / 256l)
                } else {
                    var pc12 = ActArgs(nelem = uint(wlen * hid), gelu = 0u, nblk = uint(wlen * hid / 32l))
                    enc_q8_actrq_cls(raw, h, g_rd.pf_sets[b + 12], pc12, (wlen * hid / 4l + 255l) / 256l)
                }
                pfq_ts(raw)
                let p2 = arena_planes(g_rd.layers[l].f2, g_rd.layers[l].b2)
                let hqb = dn6 ? g_rd.pf_hf : g_gpu.hq_dev
                let hsb = dn6 ? g_rd.dummy : g_gpu.hs_dev
                let hqn = dn6 ? np * hid * 2l : np * hid
                let hsn = dn6 ? 256l : np * (hid / 32l) * 4l
                pf_gemm_enc(raw, h, b + 13, g_rd.layers[l].f2, p2.wq, p2.ws, hqb, hsb, g_rd.pf_ffnout,
                    p2.wqb, p2.wsb, hqn, hsn, np * dim * 4l, VHZ_HQ, VHZ_FFO, hid, dim, g_rd.layers[l].b2, wlen)
                let nxt = l + 1l < g_rd.n_layers ? (l + 1l) * 2l * dim : nlfin   // nolint:LINT021 — composed from int64 layout offsets
                var pc14 = ArArgs(dim = uint(dim), add_on = 1u, woff = uint(nxt), eps = g_rd.eps, ascale = 1.0)
                enc_cls_ar(raw, h, g_rd.pf_sets[b + 14], pc14, wlen)
                pfq_ts(raw)
            }
            if (last) {
                if (kq_sb(g_rd.cls_fmt_marker)) {
                    var pcf = RqArgs(inbase = uint((wlen - 1l) * dim), nblk = uint(dim / 256l))
                    enc_cls_q8k_rq(raw, h, g_rd.pf_sets[bp + 1], pcf, (dim / 8l + 255l) / 256l)
                } else {
                    var pcf = RqArgs(inbase = uint((wlen - 1l) * dim), nblk = uint(dim / 32l))
                    enc_cls_dn_rq(raw, h, g_rd.pf_sets[bp + 1], pcf, (dim / 4l + 255l) / 256l)
                }
                pfq_ts(raw)
                var pcc = GemvArgs(n = uint(dim), d = uint(g_rd.vocab), nreg = 1u, ybase = 0u)
                gemv_cls_enc(g_rd.cls_fmt_marker, raw, h, g_rd.cls_set, pcc,
                    (g_rd.vocab + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg)
                pfq_ts(raw)
                vhz_dep(raw, h, VHZ_LOG, 0u, true)
                cmd_copy_whole(raw, g_rd.logits_dev, g_rd.pf_logits_host.buf, g_rd.vocab * 4l)
            }
            if (vk_prof()) {
                to_log(LOG_INFO, "vk_rdpf hz: {h.disp} nodes, {h.barriers} barriers\n")   // nolint:PERF026,PERF028 — vk_prof()-gated report leg
            }
            pfq_end(raw)
            vk_check(vkEndCommandBuffer(raw), null)
            let t_record = get_time_usec(tp0) - t_prep
            submit_wait(raw)
            if (vk_prof()) {
                to_log(LOG_INFO, "vk_rdpf w0 {w0} rows {wlen} prep {t_prep} record {t_record} submit {get_time_usec(tp0) - t_prep - t_record}\n")   // nolint:PERF026,PERF028 — vk_prof()-gated report leg
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
                for (s in range64(rpl)) {
                    role[s] += pfq_us(uint(3l + l * rpl) + uint(s))
                }
            }
            let nq = 3u + uint(g_rd.n_layers * rpl)
            var tot = 0.0lf
            for (s in range64(rpl)) {
                tot += role[s]
            }
            tot += pfq_us(2u) + pfq_us(nq) + pfq_us(nq + 1u)
            // the qkn dispatch stamps between v and rope_kv, shifting every later role by one
            if (g_rd.qk_norm) {
                to_log(LOG_INFO, "vk_rdpf gpu prologue {int(pfq_us(2u))} rq_x {int(role[0])} q {int(role[1])} k {int(role[2])} v {int(role[3])} qkn {int(role[4])} rope_kv {int(role[5])} attn {int(role[6])} rq_a {int(role[7])} wo {int(role[8])} ar1 {int(role[9])} rq_f {int(role[10])} gate {int(role[11])} up {int(role[12])} act {int(role[13])} down {int(role[14])} ar2 {int(role[15])} fin_rq {int(pfq_us(nq))} cls {int(pfq_us(nq + 1u))} total {int(tot)}\n")   // nolint:PERF026,PERF028 — vk_prof()-gated report leg
            } else {
                to_log(LOG_INFO, "vk_rdpf gpu prologue {int(pfq_us(2u))} rq_x {int(role[0])} q {int(role[1])} k {int(role[2])} v {int(role[3])} rope_kv {int(role[4])} attn {int(role[5])} rq_a {int(role[6])} wo {int(role[7])} ar1 {int(role[8])} rq_f {int(role[9])} gate {int(role[10])} up {int(role[11])} act {int(role[12])} down {int(role[13])} ar2 {int(role[14])} fin_rq {int(pfq_us(nq))} cls {int(pfq_us(nq + 1u))} total {int(tot)}\n")   // nolint:PERF026,PERF028 — vk_prof()-gated report leg
            }
        }
    }
}

// The arena FFN chain's scratch + the slab-independent act set. The three GEMV sets build lazily
// per SLAB in vk_arena_ffn (bindings 0/1 are the blk's slab planes); each GEMV role carries its
// OWN regions buffer (the weight base pair).
def private ensure_af_state(f1, f3, f2 : int) {
    if (g_gpu.af_ready) {
        return
    }
    ensure_ffn_state()
    let dkq = !vk_fmt_b32(f2)
    verify(gemv_cls_ensure(f1) && gemv_cls_ensure(f3) && gemv_cls_ensure(f2)
        && (dkq ? ensure_q8k_actrq_cls() : ensure_q8_actrq_cls()),
        "vulkan arena: the class rails must engage on a live device")
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
    let sl = arena_slab_idx(blk)
    if (!key_exists(sets, sl)) {
        let p = arena_planes(fmt, blk)
        sets[sl] = gemv_cls_set(fmt,
            fixed_array(p.wq, p.ws, mbuf, xqbuf, xsbuf, ybuf),
            fixed_array(p.wqb, p.wsb, META_BYTES, xqbytes, xsbytes, Y_BYTES),
            fixed_array(0u, 0u, 0u, xbit, xbit, ybit))
    }
    return sets[sl]
}

//! Arena seam: one dense FFN layer against arena-resident planes — gate GEMV, up GEMV, fused
//! act+requant, down GEMV, ONE submit. `blk1/blk3/blk2` are gate/up/down arena block indices.
//! Gate and up must share a format class (they share the activation image); down is free.
def vk_arena_ffn(var y : array<float>; blk1, blk3, blk2 : int64; n, nfe : int64;
                 f1, f3, f2 : int; xq : array<int8>; xs : array<float>; is_gelu : bool) {
    verify(vk_moe_init())
    assert(vk_fmt_b32(f3) == vk_fmt_b32(f1), "dasLLAMA vulkan arena: gate/up form mismatch - one activation image")
    assert(nfe * 4l <= Y_BYTES && n * 4l <= Y_BYTES, "dasLLAMA vulkan arena: ffn planes exceed y capacity")
    ensure_af_state(f1, f3, f2)
    let xunit = fmt_unit(f1)
    let dkq = !vk_fmt_b32(f2)
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
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        let gwg = (nfe + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg
        var pcg = GemvArgs(n = uint(n), d = uint(nfe), nreg = 1u, ybase = 0u)
        gemv_cls_enc(f1, raw, h, gate_set, pcg, gwg)
        gemv_cls_enc(f3, raw, h, up_set, pcg, gwg)
        // the requant form follows the DOWN plane — its GEMV consumes hq/hs
        var pca = ActArgs(nelem = uint(nfe), gelu = is_gelu ? 1u : 0u, nblk = uint(nfe / (dkq ? 256l : 32l)))
        let awgs = (nfe / (dkq ? 8l : 4l) + 255l) / 256l
        if (dkq) {
            enc_q8k_actrq_cls(raw, h, g_gpu.af_actrq_set, pca, awgs)
        } else {
            enc_q8_actrq_cls(raw, h, g_gpu.af_actrq_set, pca, awgs)
        }
        var pcd = GemvArgs(n = uint(nfe), d = uint(n), nreg = 1u, ybase = 0u)
        gemv_cls_enc(f2, raw, h, down_set, pcd, (n + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg)
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.af_yo_dev, g_gpu.af_yo_host.buf, n * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(y[0]), g_gpu.af_yo_host.mapped, n * 4l)
    }
}

// single-region batch schedule over an arena block: 4-word record at 0, all tiles map to region
// 0 at map_off = 4. Returns the dispatch workgroup count. `cm2l` must match the dispatched
// class's tile rule (batch_tile_edges); `nsplit` stacks split-k planes onto the same map.
def private fill_arena_batch_sched(hb : HostBuf; d, blk, npos : int64; fmt : int; cm2l : bool = false; nsplit : int64 = 1l) : uint {
    let te = batch_tile_edges(fmt, d, npos, cm2l)
    let wtiles = (d + te.td - 1l) / te.td
    let rtiles = (npos + te.tc - 1l) / te.tc
    let total = rtiles * wtiles * nsplit
    assert((4l + total) * 4l <= BATCH_META_BYTES, "dasLLAMA vulkan arena: batch schedule exceeds capacity")
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(arena_local_blk(blk))   // slab-local block base (already in fmt units)
        mp[1] = 0u          // rb0 - r0
        mp[2] = uint(npos)  // cnt
        mp[3] = 0u          // wg base
        for (g in range64(total)) {
            mp[4l + g] = 0u
        }
    }
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
        if (!key_exists(a.cls_batch_sets, skey)) {   // dies with the arena on model drop
            verify(q8_batch_cls_ensure(v), "vulkan arena: the batch class rail must engage on a live device")
            let p = arena_planes(fmt, blk)
            a.cls_batch_sets[skey] = q8_batch_cls_set(v,
                fixed_array(p.wq, p.ws, a.batch_meta.buf, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, g_gpu.batch_y1_dev),
                fixed_array(p.wqb, p.wsb, BATCH_META_BYTES, BATCH_XQ_BYTES, BATCH_XS_BYTES, BATCH_Y_BYTES),
                fixed_array(0u, 0u, 0u, 0u, 0u, VHG_YO))
        }
        let groups = fill_arena_batch_sched(a.batch_meta, d, blk, npos, fmt)
        upload_region_at(g_gpu.batch_xq_dev, 0l, addr<uint8 const?>(xq[0]), npos * n)
        upload_region_at(g_gpu.batch_xs_dev, 0l, addr<uint8 const?>(xs[0]), npos * (n / 32l) * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u)
        q8_batch_cls_enc(v, raw, h, a.cls_batch_sets[skey], pc, int64(groups))
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, npos * d * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(y[0]), g_gpu.batch_y1.mapped, npos * d * 4l)
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
    let unit = fmt_unit(fmt)   // q8 takes 32-block Q8_0 activations
    let nb = n / unit
    assert(n <= XQ_BYTES && nb * 4l <= XS_BYTES, "dasLLAMA vulkan arena: activations exceed scratch capacity")
    assert(d * 4l <= Y_BYTES, "dasLLAMA vulkan arena: gemv output exceeds y capacity")
    let sl = arena_slab_idx(blk)
    unsafe {
        var a & = g_gpu.arenas[fmt]
        if (!key_exists(a.cls_sets, sl)) {   // dies with the arena on model drop — rebuilt over the new scratch
            verify(gemv_cls_ensure(fmt), "vulkan arena: the gemv class rail must engage on a live device")
            let p = arena_planes(fmt, blk)
            a.cls_sets[sl] = gemv_cls_set(fmt,
                fixed_array(p.wq, p.ws, a.meta.buf, a.xq.buf, a.xs.buf, a.y_dev),
                fixed_array(p.wqb, p.wsb, META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES),
                fixed_array(0u, 0u, 0u, 0u, 0u, VHG_YO))
        }
        var mp = reinterpret<uint?>(a.meta.mapped)
        mp[0] = uint(arena_local_blk(blk))   // one region: the whole tensor at its slab base
        mp[1] = 0u          // activation block base
        memcpy(a.xq.mapped, reinterpret<void?>(xqp), n)   // one int8 quant per weight
        memcpy(a.xs.mapped, reinterpret<void?>(xsp), nb * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u)
        gemv_cls_enc(fmt, raw, h, a.cls_sets[sl], pc, (d + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg)
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, a.y_dev, a.y.buf, d * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
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
    g_gpu.comb_stage = make_host_buf(2l * COMB_WI_BYTES, false)   // transfer_src staging: w then inv
    g_gpu.comb_w_dev = make_device_buf(COMB_WI_BYTES)
    g_gpu.comb_inv_dev = make_device_buf(COMB_WI_BYTES)
    g_gpu.comb_y_dev = make_device_buf(BATCH_Y_BYTES)
    g_gpu.comb_y = make_host_buf(BATCH_Y_BYTES, true, [cached = true])
    // combine (class: accumulator rows, slot map, slot weights, down rows)
    verify(ensure_combine_cls(), "vk combine: the class rail must engage on a live device")
    g_gpu.comb_set = set_combine_cls(
        fixed_array(g_gpu.comb_y_dev, g_gpu.comb_inv_dev, g_gpu.comb_w_dev, g_gpu.batch_y1_dev),
        fixed_array(BATCH_Y_BYTES, COMB_WI_BYTES, COMB_WI_BYTES, BATCH_Y_BYTES),
        fixed_array(VHB_CY, VHB_COMB, VHB_COMB, VHB_Y1))
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

// build the chunk's schedule: regions clipped to the row window [r0, r1) (row0 re-based to the
// window), then the per-workgroup region map. Weight offsets follow the stack's format units.
// Returns the workgroup count; `n` rides the push block (param removal = the deletion sweep's)
[unused_argument(n)]
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
        // route the tile on the AVERAGE rows per active region
        let te = batch_tile_edges(fmt, d, nrc > 0l ? nrows / nrc : r1 - r0)
        g_gpu.stacks[si].batch_tile = te.tile
        g_gpu.stacks[si].batch_aligned = fmt == int(KqFmt.q8) && te.tile == 128l && d % 128l == 0l && all128
        let wtiles = (d + te.td - 1l) / te.td
        let wg_bound = ((r1 - r0 + te.tc - 1l) / te.tc + nrc) * wtiles
        assert((nrc * 4l + wg_bound) * 4l <= BATCH_META_BYTES, "dasLLAMA vulkan tier: batch schedule exceeds capacity")
        // class layout: 4-word records at 0, the per-wg map at map_off (n/d ride the push block)
        let map_off = nrc * 4l
        g_gpu.stacks[si].batch_map_off = map_off
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
            let mb = ri * 4l
            mp[mb] = uint(rel / unit)
            mp[mb + 1l] = uint(rb0 - r0)
            mp[mb + 2l] = uint(rb1 - rb0)
            mp[mb + 3l] = uint(wg)
            let rwg = ((rb1 - rb0 + te.tc - 1l) / te.tc) * wtiles
            for (g in range64(rwg)) {
                mp[map_off + wg + g] = uint(ri)
            }
            wg += rwg
            ri++
        }
        return uint(wg)
    }
}

// one stack batch tile on the class rail: the class picked by the stack's format (q8 rides the
// tile router; kq formats are fixed), a per-(stack, variant) cached set over the planes
// ensure_batch_set recorded, BatchArgs push. n/d must MATCH the fill's values.
def private stack_batch_enc(raw : VkCommandBuffer; var h : VkHaz; si : int; nwg : uint; n, d : int64) {
    unsafe {
        var st & = g_gpu.stacks[si]
        let fmt = st.fmt
        // Q4_0 rides the f16 coopmat arm too (no s8-native for nibbles — mode 2 falls back to sdot4)
        let q40cm = fmt == int(KqFmt.q40) && g_gpu.coopmat_mode == 1
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
            } elif (q40cm || kq_sb(fmt)) {
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
        } elif (q40cm || kq_sb(fmt)) {
            kq_batch_cls_enc_for(fmt, q40cm, raw, h, st.batch_cls_sets[v], pc, int64(nwg))
        } else {
            panic("dasLLAMA vulkan tier: no batch class for stack format {fmt}")
        }
    }
}

def private cmd_copy_batch_meta(raw : VkCommandBuffer; si : int) {
    var regions : array<BufferCopy>
    regions |> push(BufferCopy(srcOffset = 0ul, dstOffset = 0ul, size = uint64(BATCH_META_BYTES)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(g_gpu.stacks[si].batch_meta.buf),
                    nonowning_buf(g_gpu.stacks[si].batch_meta_dev), regions)
}

// one reusable cmd buffer, re-recorded per submit — prefill submits are few and multi-ms, so
// recording cost is noise. comb_npos > 0: down rows stay device-side, accumulating into comb_y_dev;
// only the last chunk appends the k-fold-smaller combined DMA. Hidden planes never leave the device.
def private record_ffn_batch_cmd(s1, s3, s2 : int; nwg_gu, nwg_dn : uint; rows, n, nfe, gout_bytes : int64;
                                 axq_bytes, axs_bytes, mid_nblk : int64; is_gelu : bool;
                                 comb_npos : uint = 0u; comb_first : bool = false; comb_last : bool = false;
                                 comb_wi_bytes : int64 = 0l; comb_y_bytes : int64 = 0l;
                                 comb_n : int64 = 0l; comb_k : int64 = 0l; comb_r0 : int64 = 0l; comb_r1 : int64 = 0l) {
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
    if (comb_npos > 0u && comb_first) {
        vhz_dep(raw, h, 0u, VHB_COMB, true)
        cmd_copy_whole(raw, g_gpu.comb_stage.buf, g_gpu.comb_w_dev, comb_wi_bytes)
        cmd_copy_range(raw, g_gpu.comb_stage.buf, COMB_WI_BYTES, g_gpu.comb_inv_dev, 0l, comb_wi_bytes)
    }
    stack_batch_enc(raw, h, s1, nwg_gu, n, nfe)
    stack_batch_enc(raw, h, s3, nwg_gu, n, nfe)
    // the mid-step form follows the DOWN stack — its GEMM is the consumer of the hidden plane
    var pca = ActArgs(nelem = uint(rows * nfe), gelu = is_gelu ? 1u : 0u, nblk = uint(mid_nblk))
    let dkq = g_gpu.stacks[s2].fmt != 0
    let awgs = (rows * nfe / (dkq ? 8l : 4l) + 255l) / 256l
    if (dkq) {
        enc_q8k_actrq_cls(raw, h, g_gpu.batch_actrq_set, pca, awgs)
    } else {
        enc_q8_actrq_cls(raw, h, g_gpu.batch_actrq_set, pca, awgs)
    }
    stack_batch_enc(raw, h, s2, nwg_dn, nfe, n)
    if (comb_npos > 0u) {
        var pcc = CombineArgs(n = uint(comb_n), k = uint(comb_k), r0 = uint(comb_r0), r1 = uint(comb_r1))
        enc_combine_cls(raw, h, g_gpu.comb_set, pcc, int64(comb_npos))
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
    let s1 = find_stack(unsafe(offs1[0]), f1, "prefill ffn gate")
    let s3 = find_stack(unsafe(offs3[0]), f3, "prefill ffn up")
    let s2 = find_stack(unsafe(offs2[0]), f2, "prefill ffn down")
    // gate/up share one activation image (forms must agree); down's intermediate is its own unit
    let kq_in = !vk_fmt_b32(f1)
    assert(!vk_fmt_b32(f3) == kq_in,
        "dasLLAMA vulkan tier: gate/up form mismatch - they share one activation image")
    let xunit_in = fmt_unit(f1)
    let xunit_mid = fmt_unit(f2)
    ensure_batch_state()
    ensure_batch_acts_cls()
    ensure_batch_set(s1, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
    ensure_batch_set(s3, g_gpu.batch_y2_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y2)
    ensure_batch_set(s2, g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.hs_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HQ, VHB_Y1)
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
        record_ffn_batch_cmd(s1, s3, s2, nwg1, nwg2, rows, n, nfe, rows * n * 4l, rows * n, rows * (n / xunit_in) * 4l,
            rows * nfe / xunit_mid, is_gelu,
            [comb_npos = comb ? uint(npos) : 0u, comb_first = r0 == 0l, comb_last = r1 == nrows,
             comb_wi_bytes = nrows * 4l, comb_y_bytes = npos * n * 4l,
             comb_n = comb ? n : 0l, comb_k = comb_k, comb_r0 = r0, comb_r1 = r1])
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
def private record_dense_cmd(si : int; nwg : uint; n, d, y_bytes : int64; axq_bytes, axs_bytes : int64) {
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
    stack_batch_enc(raw, h, si, nwg, n, d)
    vhz_dep(raw, h, VHB_Y1, 0u, true)
    cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, y_bytes)
    vk_check(vkEndCommandBuffer(raw), null)
}

//! MoeGpuDenseFn: dense batched GEMM — y [nrows x d] = X · Wᵀ over the resident plane
//! ([d x n] at element offset woff), a one-region ride on the batch tile rail. Chunked like
//! the FFN arm; activations arrive pre-quantized in the plane's own form.
def vk_moe_dense(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d, nrows : int64; fmt : int) {
    let si = find_stack(woff, fmt, "dense batch")
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
    verify(ensure_dn_conv_cls() && ensure_dn_scan_p1_cls() && ensure_dn_scan_p2_cls() && ensure_cls_dn_rq(),
        "vk deltanet chain: the class rails must engage on a live device")
    g_gpu.dn_conv_set = set_dn_conv_cls(
        fixed_array(g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.dn_smalls_dev),
        fixed_array(BATCH_Y_BYTES, BATCH_HQ_BYTES, DN_SMALLS_BYTES),
        fixed_array(VHB_Y1, VHB_HQ, VHB_SM))
    g_gpu.dn_scan_set = set_dn_scan_p1_cls(
        fixed_array(g_gpu.hq_dev, g_gpu.dn_smalls_dev, g_gpu.dn_ws_dev),
        fixed_array(BATCH_HQ_BYTES, DN_SMALLS_BYTES, DN_WS_BYTES),
        fixed_array(VHB_HQ, VHB_SM, VHB_WS))
    g_gpu.dn_scan_p2_set = set_dn_scan_p2_cls(
        fixed_array(g_gpu.batch_y2_dev, g_gpu.dn_state_dev, g_gpu.dn_ws_dev, g_gpu.dn_smalls_dev),
        fixed_array(BATCH_Y_BYTES, DN_STATE_BYTES, DN_WS_BYTES, DN_SMALLS_BYTES),
        fixed_array(VHB_Y2, VHB_ST, VHB_WS, VHB_SM))
    g_gpu.dn_rq_set = set_rq_cls(
        fixed_array(g_gpu.dn_ws_dev, g_gpu.hq_dev, g_gpu.hs_dev),
        fixed_array(DN_WS_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES),
        fixed_array(VHB_WS, VHB_HQ, VHB_HQ))
    g_gpu.dn_cls_ready = true
}

// one deltanet window: acts/meta/smalls staging copies, qkv + z tiles, conv, scan, the conv-tail
// extraction (before out-proj overwrites the qkv rows in y1), o requant, out-proj tile, y DMA;
// the state uploads on the first window and reads back on the last
def private record_dn_cmd(s_qkv, s_z, s_o : int; nwg_qkv, nwg_z, nwg_out : uint;
                          rows, cd, kd, di, dim, taps, nvh, nkh, ds : int64;
                          axq_bytes, axs_bytes : int64; first, last : bool; eps : float) {
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
    var pcc = DnConvArgs(cd = uint(cd), kd = uint(kd), ds = uint(ds), dconv = uint(taps + 1l),
        hist_mode = first ? 1u : 2u, taps_off = uint(DN_SM_TAPS), hist_off = uint(DN_SM_HIST),
        eps = eps, qscale = 1.0 / sqrt(float(ds)))
    enc_dn_conv_cls(raw, h, g_gpu.dn_conv_set, pcc, rows)
    pfq_ts(raw)   // q3: conv
    var pc1 = DnScanP1Args(npos = uint(rows), cd = uint(cd), kd = uint(kd), nvh = uint(nvh),
        nkh = uint(nkh), ds = uint(ds), beta_off = uint(DN_SM_BETA), g_off = uint(DN_SM_G),
        a_off = uint(DN_SM_A), dt_off = uint(DN_SM_DT), slab_stride = uint(DN_SLAB))
    enc_dn_scan_p1_cls(raw, h, g_gpu.dn_scan_set, pc1, nvh * ((rows + DN_CS - 1l) / DN_CS))
    pfq_ts(raw)   // q4: scan phase 1
    var pc2 = DnScanP2Args(npos = uint(rows), di = uint(di), ds = uint(ds), o_off = uint(DN_O_OFF),
        wnorm_off = uint(DN_SM_WNORM), slab_stride = uint(DN_SLAB), st_off = uint(DN_P2_OFF), eps = eps)
    enc_dn_scan_p2_cls(raw, h, g_gpu.dn_scan_p2_set, pc2, nvh)
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
    var pcr = RqArgs(inbase = uint(DN_O_OFF), nblk = uint(rows * di / 32l))
    enc_cls_dn_rq(raw, h, g_gpu.dn_rq_set, pcr, (rows * di / 4l + 255l) / 256l)
    pfq_ts(raw)   // q7: o requant
    stack_batch_enc(raw, h, s_o, nwg_out, di, dim)
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
    let s_qkv = find_stack(woq, 0, "dn qkv")
    let s_z = find_stack(woz, 0, "dn z-gate")
    let s_o = find_stack(woo, 0, "dn out")
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
    ensure_dn_cls_sets()
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
        let ts1 = ref_time_ticks()
        record_dn_cmd(s_qkv, s_z, s_o, nwg_qkv, nwg_z, nwg_out, rows, cd, kd, di, dim, taps, nvh, nkh, ds,
            rows * dim, rows * (dim / 32l) * 4l, r0 == 0l, r0 + rows == npos, eps)
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
    // prep (class: GEMM rows, prepped panel, gate stash, smalls) — the k form's gate view never fires
    g_gpu.at_prep_q_set = set_at_prep_cls(
        fixed_array(g_gpu.batch_y1_dev, g_gpu.at_q_dev, g_gpu.at_g_dev, g_gpu.at_smalls_dev),
        fixed_array(BATCH_Y_BYTES, AT_WINDOW * AT_MAX_QD * 4l, AT_WINDOW * AT_MAX_QD * 4l, AT_SMALLS_BYTES),
        fixed_array(VHB_Y1, VHB_ATQ, VHB_ATG, VHB_SM))
    g_gpu.at_prep_k_set = set_at_prep_cls(
        fixed_array(g_gpu.batch_y2_dev, g_gpu.at_k_dev, g_gpu.at_g_dev, g_gpu.at_smalls_dev),
        fixed_array(BATCH_Y_BYTES, AT_CTX * AT_MAX_KV * 4l, AT_WINDOW * AT_MAX_QD * 4l, AT_SMALLS_BYTES),
        fixed_array(VHB_Y2, VHB_ATK, 0u, VHB_SM))
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

// one attention window: acts/meta/smalls staging copies, q|gate + k + v tiles, the prep pair
// (deinterleave/qk-rms/rope; k lands at absolute positions), v append + k/v host DMA in the
// transfer window, the flash attention pass, o requant, wo tile, y DMA
def private record_at_cmd(s_q, s_k, s_v, s_o : int; nwg_q, nwg_k, nwg_v, nwg_o : uint;
                          rows, w0, qd, kv_dim, dim, n_heads, hs, half, n_kv, kv_mul, o_nblk : int64;
                          axq_bytes, axs_bytes, sm_bytes : int64; o_kq, gated, qknorm : bool;
                          eps, scale : float) {
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
    vhz_dep(raw, h, 0u, VHB_SM, true)
    cmd_copy_range(raw, g_gpu.at_smalls.buf, 0l, g_gpu.at_smalls_dev, 0l, sm_bytes)
    stack_batch_enc(raw, h, s_q, nwg_q, dim, gated ? 2l * qd : qd)
    stack_batch_enc(raw, h, s_k, nwg_k, dim, kv_dim)
    stack_batch_enc(raw, h, s_v, nwg_v, dim, kv_dim)
    let pflags = uint((gated ? 1 : 0) | (qknorm ? 2 : 0))
    var pcq = AtPrepArgs(d = uint(qd), nh = uint(n_heads), hs = uint(hs), half = uint(half),
        flags = pflags, rms_off = uint(AT_SM_RMSQ), cos_off = uint(AT_SM_COS),
        sin_off = uint(AT_SM_COS + rows * half), obase = 0u, qsrc = 1u, eps = eps)
    enc_at_prep_cls(raw, h, g_gpu.at_prep_q_set, pcq, rows)
    var pck = AtPrepArgs(d = uint(kv_dim), nh = uint(n_kv), hs = uint(hs), half = uint(half),
        flags = pflags, rms_off = uint(AT_SM_RMSK), cos_off = uint(AT_SM_COS),
        sin_off = uint(AT_SM_COS + rows * half), obase = uint(w0), qsrc = 0u, eps = eps)
    enc_at_prep_cls(raw, h, g_gpu.at_prep_k_set, pck, rows)
    // transfer window: append raw v at absolute positions, DMA roped k/raw v to host
    vhz_dep(raw, h, VHB_VRAW, VHB_ATV, true)
    cmd_copy_range(raw, g_gpu.at_vraw_dev, 0l, g_gpu.at_v_dev, w0 * kv_dim * 4l, rows * kv_dim * 4l)
    vhz_dep(raw, h, VHB_ATK, 0u, true)
    cmd_copy_range(raw, g_gpu.at_k_dev, w0 * kv_dim * 4l, g_gpu.at_kv_host.buf, 0l, rows * kv_dim * 4l)
    vhz_dep(raw, h, VHB_VRAW, 0u, true)
    cmd_copy_range(raw, g_gpu.at_vraw_dev, 0l, g_gpu.at_kv_host.buf, AT_WINDOW * AT_MAX_KV * 4l, rows * kv_dim * 4l)
    var pca = AtAttnArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd), kvd = uint(kv_dim),
        hs = uint(hs), kv_mul = uint(kv_mul), flags = gated ? 1u : 0u, dpl = uint(hs / 32l), scale = scale)
    enc_at_attn_cls(raw, h, g_gpu.at_attn_set, pca, n_heads * ((rows + 7l) / 8l))
    var pcr = RqArgs(inbase = 0u, nblk = uint(o_nblk))
    let rqwgs = (rows * qd / (o_kq ? 8l : 4l) + 255l) / 256l
    if (o_kq) {
        enc_cls_q8k_rq(raw, h, g_gpu.at_rq_set, pcr, rqwgs)
    } else {
        enc_cls_dn_rq(raw, h, g_gpu.at_rq_set, pcr, rqwgs)
    }
    stack_batch_enc(raw, h, s_o, nwg_o, qd, dim)
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
        let ts1 = ref_time_ticks()
        record_at_cmd(s_q, s_k, s_v, s_o, nwg_q, nwg_k, nwg_v, nwg_o,
            rows, r0, qd, kv_dim, dim, n_heads, hs, half, n_kv, kv_mul,
            rows * qd / (fo != 0 ? 256l : 32l),
            rows * dim, rows * (dim / xunit) * 4l, (AT_SM_COS + 2l * rows * half) * 4l,
            fo != 0, gated, qknorm, eps, scale)
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
    // same read-time carve carved_budget() subtracts — overcommit silently hits the WDDM paging cliff
    let need = moe_gpu_stream_need()
    let reserve = need > 0l ? 2l * need : STREAM_RESERVE
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
        if (g_gpu.stream_stacks[sk].wsbytes > 0l) {
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
