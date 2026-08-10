options gen2
options _comment_hygiene = true

module dasllama_ple shared public

require dasllama/dasllama_common
require dasllama/dasllama_moe   // moe_select/moe_experts_apply — gemma4's router and expert dispatch
require dasllama/dasllama_blocks   // the gemma4 MoE prefill shares the dense/grouped block helpers
require dasllama/dasllama_math
require dasllama/dasllama_par
require dasllama/dasllama_convert
require math

// gemma-4 E-series per-layer embeddings (PLE) and the gemma4 MoE FFN. PLE is a per-(token, layer)
// side input mixed into each layer's output, so its pre-step runs inside the shared forward
// sequence — common reaches it through the hooks registered below, which is what lets this whole
// subsystem live outside the hub (the engine is required back, so the umbrella breaks the cycle).

// ===== gemma-4 E-series per-layer embeddings (PLE) =====
// A per-(token, layer) side input mixed into each layer's output (mirrors llama.cpp gemma4's
// build_inp_per_layer + per_layer_embd). ple_inp is POSITION-major: (p, l, i) at p*(layers*ple)+l*ple+i.

// Pre-step finish (shared by decode/prefill): given the gathered+sqrt(ple)-scaled token rows already
// in s.ple_inp[0..npos*all), add the projected embedding branch. proj = rmsnorm((1/√dim)·(model_proj·
// embd)); ple_inp = (gathered + proj)/√2. embd_scaled is the sqrt(dim)-scaled residual (s.x / s.x_b).
def private ple_pre_finish(t : Model; var s : Session; embd_scaled : array<float>; npos : int64) {
    trace_tag(TRACE_TAG_PLE)   // decode: runs between cls and layer 0, would inherit the stale cls tag
    let c = t.config
    let ple = c.n_embd_per_layer
    let all = c.n_layers * ple
    scale_inplace(unsafe(addr(s.ple_inp[0])), sqrt(float(ple)), npos * all)
    // model_proj · embedding, then × 1/√dim — native-BF16 rows when kept (exact widen), else fp32 expand
    if (t.ple_model_bf16) {
        matmul_bf16_batch(s.ple_proj, t.bf16blob, t.ple_model_off, embd_scaled, c.dim, all, npos)
    } else {
        mm_fblob_b(s.ple_proj, t, t.ple_model_off, embd_scaled, c.dim, all, npos)
    }
    scale_inplace(unsafe(addr(s.ple_proj[0])), 1.0 / sqrt(float(c.dim)), npos * all)
    // RMSNorm each (position, layer) ple-slice with the single proj-norm vector
    rms_batch(s.ple_proj, s.ple_proj, t, t.ple_pnorm_off, ple, npos * c.n_layers)
    add_inplace(unsafe(addr(s.ple_inp[0])), unsafe(addr(s.ple_proj[0])), npos * all)
    scale_inplace(unsafe(addr(s.ple_inp[0])), 1.0 / sqrt(2.0), npos * all)
}

// One PLE token row (all = layers*ple elements, superblock-aligned by the detect gate) off the
// gather's storage: native disk-order K-quant planes (the table is excluded from the grp repack)
// or the q8 linear region on fallback loads.
def private ple_gather_row(t : Model; row : int64; var dst : array<float>; doff : int64) {
    let all = t.config.n_layers * t.config.n_embd_per_layer
    let eloff = t.ple_emb_off + row * all
    if (t.ple_emb_fmt == KqFmt.q8) {
        dequant_q8_row(t, eloff, all, unsafe(addr(dst[doff])))
        return
    }
    for (blk in range64(all / 256l)) {
        let sb = (eloff + blk * 256l) / 256l
        if (t.ple_emb_fmt == KqFmt.k4) {
            dequant_k4_plane_superblock(t.k4q, sb * K4_QSB, t.k4s, sb * K4_SSB, dst, doff + blk * 256l)
        } elif (t.ple_emb_fmt == KqFmt.k5) {
            dequant_k5_plane_superblock(t.k5q, sb * K5_QSB, t.k5s, sb * K5_SSB, dst, doff + blk * 256l)
        } elif (t.ple_emb_fmt == KqFmt.k6) {
            dequant_k6_plane_superblock(t.k6q, sb * K6_QSB, t.k6s, sb * K6_SSB, dst, doff + blk * 256l)
        } elif (t.ple_emb_fmt == KqFmt.q40) {
            dequant_q40_plane_superblock(t.q40q, sb * Q40_QSB, t.q40s, sb * Q40_SSB, dst, doff + blk * 256l)
        } else {
            panic("ple_gather_row: '{t.ple_emb_fmt}' has no kq plane pair")
        }
    }
}

// Decode pre-step: gather the single token's per-layer embedding row, then finish.
def private ple_pre_decode(t : Model; var s : Session; token : int64) {
    ple_gather_row(t, token, s.ple_inp, 0l)
    ple_pre_finish(t, s, s.x, 1l)
}

//! Prefill pre-step: gather every position's per-layer embedding row, then finish (batched).
//! Public as the late-decline fallback for a driver that claimed the pre-step via the gate.
def private ple_pre_prefill_impl(t : Model; var s : Session; tokens : array<int64>; npos : int64) {
    let all = t.config.n_layers * t.config.n_embd_per_layer
    for (p in range64(npos)) {
        ple_gather_row(t, tokens[p], s.ple_inp, p * all)
    }
    ple_pre_finish(t, s, s.x_b, npos)
}

// Prefill-from-embeddings (multimodal) pre-step: no token ids, so the gather uses the padding token
// (id 0) broadcast across positions — matching llama.cpp gemma4's build_inp_per_layer media branch.
def private ple_pre_prefill_pad_impl(t : Model; var s : Session; npos : int64) {
    let all = t.config.n_layers * t.config.n_embd_per_layer
    for (p in range64(npos)) {
        ple_gather_row(t, 0l, s.ple_inp, p * all)
    }
    ple_pre_finish(t, s, s.x_b, npos)
}

// Per-layer block (decode): cur += post_norm(proj · (gelu(inp_gate·cur) ⊙ ple_inp[l])). In place on s.x.
def private ple_block_decode(t : Model; var s : Session; l : int64) {
    trace_tag(TRACE_TAG_PLE)
    let c = t.config
    let dim = c.dim
    let ple = c.n_embd_per_layer
    mm(s.ple_gate, t, t.ple_gate_off + l * dim * ple, s.x, s.xq, s.xs, dim, ple)   // dim -> ple
    gelu(s.ple_gate, ple)
    mul_inplace(unsafe(addr(s.ple_gate[0])), unsafe(addr(s.ple_inp[l * ple])), ple)  // ⊙ this layer's side input
    mm(s.xb, t, t.ple_proj_off + l * ple * dim, s.ple_gate, s.xq, s.xs, ple, dim)   // ple -> dim
    rms_at(s.xb, t, t.ple_post_off + l * dim, s.xb, dim)
    add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb[0])), dim)   // residual (pe_in + projected)
}

// Per-layer block (prefill, batched over npos). ple_inp is position-major so the layer-l side vector
// of position p sits at p*(layers*ple) + l*ple.
def private ple_block_prefill(t : Model; var s : Session; l : int64; npos : int64) {
    // a broken gate promise would read an empty/stale ple_inp here — fail loud, never mismeasure
    if (s.ple_gpu_pending) panic("ple_block_prefill: GPU pre-step promised but the CPU body engaged")
    let c = t.config
    let dim = c.dim
    let ple = c.n_embd_per_layer
    let all = c.n_layers * ple
    mm_b(s.ple_gate, t, t.ple_gate_off + l * dim * ple, s.x_b, s.xqb, s.xsb, dim, ple, npos)
    gelu(s.ple_gate, npos * ple)
    for (p in range64(npos)) {
        mul_inplace(unsafe(addr(s.ple_gate[p * ple])), unsafe(addr(s.ple_inp[p * all + l * ple])), ple)
    }
    mm_b(s.xb_b, t, t.ple_proj_off + l * ple * dim, s.ple_gate, s.xqb, s.xsb, ple, dim, npos)
    rms_batch(s.xb_b, s.xb_b, t, t.ple_post_off + l * dim, dim, npos)
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb_b[0])), npos * dim)
}

// gemma4 MoE router input: weightless RMSNorm(attn_out) × (1/√dim) × ffn_gate_inp_s, into s.xb2.
// Distinct from the plain-matmul routers: the RMS is over attn_out (not the shared-expert pre-norm),
// weightless, then scaled by 1/√n_embd and a learned per-channel scale before the ffn_gate_inp matmul.
def private gemma4_router_tmp(t : Model; var s : Session; l : int64) {
    let dim = t.config.dim
    // sum of squares in DOUBLE (matches ggml_rms_norm): float drift here flips the discrete top-8 expert pick
    var ss = 0.0lf
    for (i in range64(dim)) {
        ss += double(s.moe_ao[i] * s.moe_ao[i])
    }
    let scale = 1.0 / sqrt(float(ss / double(dim)) + t.config.norm_eps)
    let inv_d = 1.0 / sqrt(float(dim))
    unsafe {
        let scp = addr < float const? >(t.fblob[t.moe_router_scale_off + l * dim])
        var op = addr(s.xb2[0])
        let ip = addr < float const? >(s.moe_ao[0])
        let si = scale * inv_d
        for (i in range64(dim)) {
            op[i] = ip[i] * si * scp[i]
        }
    }
}

// gemma4 MoE layer (26B-A4B): two parallel branches of the residual stream in s.moe_ao, SUMMED.
// routed = post_ffw_norm_2(Σ_j w_j · GeGLU_e_j(pre_ffw_norm_2(attn_out))); dense = a full dense
// shared expert, always on. Leaves post_ffw_norm(routed + dense) in s.moe_acc.
def private gemma4_moe_ffn(t : Model; var s : Session; l : int64) {
    let c = t.config
    let dim = c.dim
    let ne = c.n_expert
    let k = c.n_expert_used
    // routed branch — input, custom router, select, per-expert down scale, experts, post-norm
    let ts_norm0 = prof_ticks()
    rms_at(s.xb, t, t.rms_pre_ffn2_off + l * dim, s.moe_ao, dim)
    prof_add("norm", ts_norm0)
    trace_tag(TRACE_TAG_ROUTE)   // decode router GEMV — without this it rides the stale attn tag in traces
    let ts_route = prof_ticks()
    gemma4_router_tmp(t, s, l)
    mm_fblob(s.moe_logits, t, t.router_off + l * ne * dim, s.xb2, dim, ne)
    moe_select(c, unsafe(addr(s.moe_logits[0])), s.moe_idx, s.moe_w, 0l)
    for (j in range64(k)) {   // ggml scales each expert's down output by ffn_down_exps_s — fold it into w_j
        s.moe_w[j] *= t.fblob[t.moe_down_scale_off + l * ne + s.moe_idx[j]]
    }
    prof_add("moe_route", ts_route)
    moe_experts_apply(t, s, l)
    let ts_norm1 = prof_ticks()
    rms_at(s.moe_acc, t, t.rms_post_ffn2_off + l * dim, s.moe_acc, dim)
    prof_add("norm", ts_norm1)
    // dense parallel shared expert (its own pre/post norms), into s.xb
    let ts_sh = prof_ticks()
    trace_tag(TRACE_TAG_SHEXP)
    rms_at(s.xb, t, t.rms_ffn_off + l * dim, s.moe_ao, dim)
    dense_ffn_inplace(t, s, l)
    rms_at(s.xb, t, t.rms_post_ffn1_off + l * dim, s.xb, dim)
    prof_add("mm_shexp", ts_sh)
    // combine then the shared post_ffw_norm on the sum
    let ts_comb = prof_ticks()
    add_inplace(unsafe(addr(s.moe_acc[0])), unsafe(addr(s.xb[0])), dim)
    prof_add("act_resid", ts_comb)
    let ts_norm2 = prof_ticks()
    rms_at(s.moe_acc, t, t.rms_post_ffn_off + l * dim, s.moe_acc, dim)
    prof_add("norm", ts_norm2)
}

// gemma4 FFN decode: dense (12B/31B) or the parallel dense-shared + routed MoE (26B-A4B) by expert count.
[no_alloc, no_env, no_io]
def ffn_gemma4_decode(t : Model; var s : Session; l : int64) {
    let c = t.config
    if (c.n_expert <= 0l) {
        ffn_dense_decode(t, s, l)   // out_scale deferred to after the PLE block when has_ple
        if (has_ple(c)) {  // E-series: per-layer embedding block sits between the FFN residual and out_scale
            let ts_ple = prof_ticks()
            ple_block_decode(t, s, l)
            if (c.layer_out_scale) {
                scale_inplace(unsafe(addr(s.x[0])), t.fblob[t.out_scale_off + l], c.dim)
            }
            prof_add("ple", ts_ple)
        }
        return
    }
    let dim = c.dim
    let ts_copy = prof_ticks()
    copy_floats(s.x, 0l, s.moe_ao, 0l, dim)
    prof_add("act_resid", ts_copy)
    gemma4_moe_ffn(t, s, l)
    let ts_resid = prof_ticks()
    if (c.layer_out_scale) {
        add_scale_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.moe_acc[0])), t.fblob[t.out_scale_off + l], dim)
    } else {
        add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.moe_acc[0])), dim)
    }
    prof_add("act_resid", ts_resid)
}

// gemma4 FFN prefill: dense delegates; MoE takes the grouped expert-bucketed path (Q8, default) or the
// per-position reference loop (fp32 loads and the bit-identical A/B switch).
[no_alloc, no_env, no_io]
def ffn_gemma4_prefill(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    if (c.n_expert <= 0l) {
        ffn_dense_prefill(t, s, l, npos)   // out_scale deferred to after the PLE block when has_ple
        if (has_ple(c)) {  // E-series per-layer embedding block, then out_scale
            let ts_ple = prof_ticks()
            ple_block_prefill(t, s, l, npos)
            if (c.layer_out_scale) {
                scale_inplace(unsafe(addr(s.x_b[0])), t.fblob[t.out_scale_off + l], npos * c.dim)
            }
            prof_add("ple", ts_ple)
        }
        return
    }
    let dim = c.dim
    // kq-expert layers ride grouped buckets via the kq batch tile; portable/no-kq backends take the per-position path
    let kq_grouped_ok = !moe_layer_sb_kq(t, l) || (t.kq_repacked && kernel_backend_has_kq_batch())
    if (t.quant == QuantMode.q8 && g_moe_grouped_prefill && kq_grouped_ok) {
        gemma4_moe_prefill_grouped(t, s, l, npos)
    } else {
        for (p in range64(npos)) {
            copy_floats(s.x_b, p * dim, s.moe_ao, 0l, dim)
            gemma4_moe_ffn(t, s, l)
            add_inplace(unsafe(addr(s.x_b[p * dim])), unsafe(addr(s.moe_acc[0])), dim)
        }
    }
    if (c.layer_out_scale) {
        scale_inplace(unsafe(addr(s.x_b[0])), t.fblob[t.out_scale_off + l], npos * dim)
    }
}

// gemma4 MoE router input, batched: twin of gemma4_router_tmp over npos positions. Sum-of-squares
// accumulates in DOUBLE to match ggml_float — top-8 select is near-tie-brittle, float SoS would flip
// experts. Bit-identical per row to the decode single-token path.
def private gemma4_router_batch(t : Model; var rin : array<float>; src : array<float>; l, dim, npos : int64) {
    let eps = t.config.norm_eps
    let inv_d = 1.0 / sqrt(float(dim))
    unsafe {
        var op = addr(rin[0])
        let ip = addr < float const? >(src[0])
        let scp = addr < float const? >(t.fblob[t.moe_router_scale_off + l * dim])
        maybe_parallel_for(dim * npos >= g_norm_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let off = int64(p) * dim
                    var ss = 0.0lf
                    for (i in range64(dim)) {
                        let v = ip[off + i]
                        ss += double(v * v)
                    }
                    let scale = 1.0 / sqrt(float(ss / double(dim)) + eps)
                    for (i in range64(dim)) {
                        op[off + i] = ip[off + i] * scale * inv_d * scp[i]
                    }
                }
            }
        }
    }
}

// gemma4 MoE prefill, grouped: batched twin of gemma4_moe_ffn (26B-A4B). Routes, buckets by expert,
// runs one Q8 batch GEMM chain per touched expert, then the dense shared expert over all positions.
// Bit-identical per row to the naive per-position path — g_moe_grouped_prefill is a pure perf lever.
def private gemma4_moe_prefill_grouped(t : Model; var s : Session; l : int64; npos : int64) { // nolint:STYLE037,STYLE038 — the grouped gemma4 MoE prefill is one bucketed pipeline; its stages share scratch and must stay together
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    let nb = dim / 32l
    let ne = c.n_expert
    let k = c.n_expert_used
    let nfe = c.n_ff_exp
    let nk = npos * k
    // per-layer expert stack formats (kq layers: the gate routed us here only with the batch tile up)
    let kq_layer = moe_layer_kq(t, l)
    let f1 = fmt_at(t.we1_fmt, l)
    let f2 = fmt_at(t.we2_fmt, l)
    let f3 = fmt_at(t.we3_fmt, l)
    // GPU tier: offloaded layers run the routed chain device-side; the fused arm gathers ALL nk bucket rows at once (grows)
    let fused_gpu = (!c.moe_exps_bias && !t.experts_mx4 && c.ffn_act != FfnAct.swiglu_oai
        && (moe_gpu_layer_on_gpu(l) || moe_gpu_layer_streamed(l)) && g_moe_fused_expert_gemms && npos >= 32l && gpu_prefill_route_on())
    let gpu_comb = fused_gpu && gpu_combine_on()
    // fused CPU: one region-list GEMM per projection; a q8 stack keeps the per-expert chain (mirror of blocks.das fused_kq)
    let sb_fusable = t.kq_repacked && kernel_backend_has_kq_batch_groupn()
    let fused_kq = (kq_layer && g_moe_fused_expert_gemms
        && (kq_sb(f1) ? sb_fusable : f1 == KqFmt.q51)
        && (kq_sb(f2) ? sb_fusable : f2 == KqFmt.q51)
        && (kq_sb(f3) ? sb_fusable : f3 == KqFmt.q51))
    let fused = fused_gpu || fused_kq
    let grows = fused ? nk : npos
    s.moe_rlog |> resize(npos * ne)
    s.moe_idx_b |> resize(nk)
    s.moe_w_b |> resize(nk)
    s.moe_cnt |> resize(ne)
    s.moe_bkt |> resize(nk)
    s.moe_gxq |> resize(grows * dim)
    s.moe_gxs |> resize(grows * nb)
    s.moe_ghb |> resize(grows * nfe)
    s.moe_ghb2 |> resize(grows * nfe)
    s.moe_hq |> resize(grows * nfe)
    s.moe_hs |> resize(grows * nfe / 32l)
    s.moe_gout |> resize(grows * dim)
    s.moe_eout |> resize(nk * dim)
    if (kq_layer) {
        s.moe_gxbs |> resize(grows * dim / 16l)
        s.moe_hbs |> resize(grows * nfe / 16l)
    }

    // 1. route every position: double-RMS router input, one batched GEMM, per-row select + down-scale fold
    trace_tag(TRACE_TAG_ROUTE)
    let ts_route = prof_ticks()
    gemma4_router_batch(t, s.xb2_b, s.x_b, l, dim, npos)
    mm_fblob_b(s.moe_rlog, t, t.router_off + l * ne * dim, s.xb2_b, dim, ne, npos)
    for (p in range64(npos)) {
        moe_select(c, unsafe(addr(s.moe_rlog[p * ne])), s.moe_idx_b, s.moe_w_b, p * k)
        for (j in range64(k)) {
            s.moe_w_b[p * k + j] *= t.fblob[t.moe_down_scale_off + l * ne + s.moe_idx_b[p * k + j]]
        }
    }
    // bucket (position, slot) pairs by expert — CSR in-place cursor trick (moe_cnt[e] ends as its run END)
    for (e in range64(ne)) {
        s.moe_cnt[e] = 0l
    }
    for (r in range64(nk)) {
        s.moe_cnt[s.moe_idx_b[r]]++
    }
    var run = 0l
    for (e in range64(ne)) {
        let cn = s.moe_cnt[e]
        s.moe_cnt[e] = run
        run += cn
    }
    for (r in range64(nk)) {
        let e = s.moe_idx_b[r]
        s.moe_bkt[s.moe_cnt[e]] = r
        s.moe_cnt[e]++
    }
    prof_add("moe_route", ts_route)

    // 2. expert input = pre_ffw_norm_2(residual) per position, quantized ONCE for every expert GEMM
    rms_batch(s.xb_b, s.x_b, t, t.rms_pre_ffn2_off + l * dim, dim, npos)
    // kq v2: kq gate/up stacks gather off a Q8_K-form image; q8 stacks off the Q8_0 image
    let moe_kform = kq_layer && moe_pair_kform(f1, f3)
    if (moe_kform) {
        s.kxqb |> resize(npos * dim)
        s.kxsb |> resize(npos * dim / 256l)
        s.kxbsb |> resize(npos * dim / 16l)
        requant_rows_q8k_bs(s.xb_b, dim, npos, s.kxqb, s.kxsb, s.kxbsb)
    } else {
        requant_rows_q8(s.xb_b, dim, npos, s.xqb, s.xsb)
    }

    // 3F. fused: gate/up/down each run as ONE region-list dispatch (3 fork/joins/layer, not 3/expert)
    if (fused) {
        trace_tag(TRACE_TAG_MOE)
        let ts_gather = prof_ticks()
        // the GPU tile computes activation block sums in-kernel — skip the per-16 sum gather there
        moe_gather_rows(s, 0l, nk, dim, k, moe_kform && !fused_gpu, moe_kform)
        prof_add("moe_gather", ts_gather)
        // region triples (woff, row0, cnt) per touched expert; the three planes share row0/cnt
        s.moe_offs1 |> clear(); s.moe_offs1 |> reserve(ne * 3l)
        s.moe_offs3 |> clear(); s.moe_offs3 |> reserve(ne * 3l)
        s.moe_offs2 |> clear(); s.moe_offs2 |> reserve(ne * 3l)
        // LPT order (biggest buckets first) front-loads expensive spans
        s.moe_ord |> clear()
        s.moe_ord |> reserve(ne)   // capacity persists across layers — no per-layer alloc
        for (e in range64(ne)) {
            let b0 = e == 0l ? 0l : s.moe_cnt[e - 1l]
            let cnt = s.moe_cnt[e] - b0
            if (cnt > 0l) {
                s.moe_ord |> push(int2(int(e), int(cnt)))
            }
        }
        lpt_order(s.moe_ord)
        var nreg = 0l
        for (oe in s.moe_ord) {
            let e = int64(oe.x)
            let cnt = int64(oe.y)
            let b0 = e == 0l ? 0l : s.moe_cnt[e - 1l]
            let base = e * dim * nfe
            s.moe_offs1 |> push(t.we1_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
            s.moe_offs1 |> push(b0)
            s.moe_offs1 |> push(cnt)
            s.moe_offs3 |> push(t.we3_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
            s.moe_offs3 |> push(b0)
            s.moe_offs3 |> push(cnt)
            s.moe_offs2 |> push(t.we2_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
            s.moe_offs2 |> push(b0)
            s.moe_offs2 |> push(cnt)
            nreg++
        }
        let ts_g1 = prof_ticks()
        if (gpu_comb) {   // gout comes back as npos combined rows — the park + CPU reduce disappear
            s.moe_inv |> resize(nk)
            for (j in range64(nk)) {
                s.moe_inv[s.moe_bkt[j]] = uint(j)
            }
            matmul_moe_gpu_ffn_combined(s.moe_eout, s.moe_offs1, s.moe_offs3, s.moe_offs2, nreg,
                s.moe_gxq, s.moe_gxs, s.moe_w_b, s.moe_inv, dim, nfe, nk, npos, int(f1), int(f3), int(f2), c.ffn_act == FfnAct.gelu)
            prof_add("moe_gpu", ts_g1)
        } elif (fused_gpu) {
            matmul_moe_gpu_ffn(s.moe_gout, s.moe_offs1, s.moe_offs3, s.moe_offs2, nreg,
                s.moe_gxq, s.moe_gxs, dim, nfe, nk, int(f1), int(f3), int(f2), c.ffn_act == FfnAct.gelu)
            prof_add("moe_gpu", ts_g1)
        } else {   // fused_kq: native-plane region-list chain, one batched GEMM per projection
            if (kq_sb(f1)) {
                mm_b_kq_groupn(s.moe_ghb, t, f1, s.moe_offs1, nreg, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe)
            } else {
                mm_b_q51_groupn(s.moe_ghb, t, s.moe_offs1, nreg, s.moe_gxq, s.moe_gxs, dim, nfe)
            }
            if (kq_sb(f3)) {
                mm_b_kq_groupn(s.moe_ghb2, t, f3, s.moe_offs3, nreg, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe)
            } else {
                mm_b_q51_groupn(s.moe_ghb2, t, s.moe_offs3, nreg, s.moe_gxq, s.moe_gxs, dim, nfe)
            }
            prof_add("mm_gemm", ts_g1)
            let ts_act = prof_ticks()
            gate_batch(s.moe_ghb, s.moe_ghb2, c.ffn_act, nfe, nk)
            prof_add("act", ts_act)
            // kq v2: sb down stacks read Q8_K-form rows (+ per-16 sums); q51 the Q8_0 form
            if (kq_sb(f2)) {
                requant_rows_q8k_bs(s.moe_ghb, nfe, nk, s.moe_hq, s.moe_hs, s.moe_hbs)
            } else {
                requant_rows_q8(s.moe_ghb, nfe, nk, s.moe_hq, s.moe_hs)
            }
            let ts_g3 = prof_ticks()
            if (kq_sb(f2)) {
                mm_b_kq_groupn(s.moe_gout, t, f2, s.moe_offs2, nreg, s.moe_hq, s.moe_hs, s.moe_hbs, nfe, dim)
            } else {
                mm_b_q51_groupn(s.moe_gout, t, s.moe_offs2, nreg, s.moe_hq, s.moe_hs, nfe, dim)
            }
            prof_add("mm_moe_dn", ts_g3)   // down separately from gate/up — the q51-vs-kq lane split is the diagnostic
        }
        if (!gpu_comb) {   // combine off: park the raw down rows in bucket order for the reduce
            trace_tag(TRACE_TAG_PARK)
            let ts_park = prof_ticks()
            unsafe {
                let gp = addr<float const?>(s.moe_gout[0])
                var ep = addr(s.moe_eout[0])
                let bp = addr<int64 const?>(s.moe_bkt[0])
                maybe_parallel_for(nk * dim >= g_act_par_threshold, 0, int(nk), get_dispatch_lanes()) $(rb, re) {
                    unsafe {
                        for (j in range(rb, re)) {
                            let jj = int64(j)
                            copy_floats(ep + bp[jj] * dim, gp + jj * dim, dim)
                        }
                    }
                }
            }
            prof_add("moe_park", ts_park)
        }
    }

    // 3. one batched GEMM chain per touched expert, parking raw down rows (skipped when 3F ran)
    if (!fused) {
        trace_tag(TRACE_TAG_MOE)
    }
    for (e in range64(fused ? 0l : ne)) {
        let b0 = e == 0l ? 0l : s.moe_cnt[e - 1l]
        let cnt = s.moe_cnt[e] - b0
        if (cnt == 0l) {
            continue
        }
        let ts_gather = prof_ticks()
        moe_gather_rows(s, b0, cnt, dim, k, moe_kform, moe_kform)
        prof_add("moe_gather", ts_gather)
        let base = e * dim * nfe
        if (kq_sb(f1)) {
            mm_b_kq_pre(s.moe_ghb, t, f1, t.we1_offs[l] + base, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe, cnt)
        } elif (f1 == KqFmt.q51) {
            mm_b_q51_pre(s.moe_ghb, t, t.we1_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
        } else {
            mm_b_q8_pre(s.moe_ghb, t, t.we1_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
        }
        if (kq_sb(f3)) {
            mm_b_kq_pre(s.moe_ghb2, t, f3, t.we3_offs[l] + base, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe, cnt)
        } elif (f3 == KqFmt.q51) {
            mm_b_q51_pre(s.moe_ghb2, t, t.we3_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
        } else {
            mm_b_q8_pre(s.moe_ghb2, t, t.we3_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
        }
        let ts_act = prof_ticks()
        gate_batch(s.moe_ghb, s.moe_ghb2, c.ffn_act, nfe, cnt)
        prof_add("act", ts_act)
        if (kq_sb(f2)) {
            // kq v2: Q8_K-form rows + the per-16 sums the fold consumes
            requant_rows_q8k_bs(s.moe_ghb, nfe, cnt, s.moe_hq, s.moe_hs, s.moe_hbs)
            mm_b_kq_pre(s.moe_gout, t, f2, t.we2_offs[l] + base, s.moe_hq, s.moe_hs, s.moe_hbs, nfe, dim, cnt)
        } elif (f2 == KqFmt.q51) {
            requant_rows_q8(s.moe_ghb, nfe, cnt, s.moe_hq, s.moe_hs)
            mm_b_q51_pre(s.moe_gout, t, t.we2_offs[l] + base, s.moe_hq, s.moe_hs, nfe, dim, cnt)
        } else {
            requant_rows_q8(s.moe_ghb, nfe, cnt, s.moe_hq, s.moe_hs)
            mm_b_q8_pre(s.moe_gout, t, t.we2_offs[l] + base, s.moe_hq, s.moe_hs, nfe, dim, cnt)
        }
        let ts_park = prof_ticks()
        for (r in range64(cnt)) {
            copy_floats(s.moe_gout, r * dim, s.moe_eout, s.moe_bkt[b0 + r] * dim, dim)
        }
        prof_add("moe_park", ts_park)
    }

    // 4. ordered reduce into xb2_b: k weighted routed outputs in slot order, matching decode's accumulation order
    let ts_red = prof_ticks()
    unsafe {
        var ap = addr(s.xb2_b[0])
        let ep = addr < float const? >(s.moe_eout[0])
        let wp = addr < float const? >(s.moe_w_b[0])
        maybe_parallel_for(nk * dim >= g_act_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    var arow = ap + pp * dim
                    if (gpu_comb) {   // the device already reduced: eout's head IS the combined row
                        let cr = ep + pp * dim
                        for (i in range64(dim)) {
                            arow[i] = cr[i]
                        }
                    } else {
                        for (i in range64(dim)) {
                            arow[i] = 0.0
                        }
                        for (j in range64(k)) {
                            let w = wp[pp * k + j]
                            let er = ep + (pp * k + j) * dim
                            for (i in range64(dim)) {
                                arow[i] += w * er[i]
                            }
                        }
                    }
                }
            }
        }
    }
    prof_add("moe_reduce", ts_red)
    rms_batch(s.xb2_b, s.xb2_b, t, t.rms_post_ffn2_off + l * dim, dim, npos)

    // 5. dense parallel shared expert over ALL positions: rms_ffn -> GeGLU -> post_ffw_norm_1 into gout
    let ts_sh = prof_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_ffn_off + l * dim, dim, npos)
    mm_b_f(s, s.hb_b, t, fmt_at(t.w1_fmt, l), t.w1_offs[l], s.xb_b, dim, hidden, npos)
    mm_b_f(s, s.hb2_b, t, fmt_at(t.w3_fmt, l), t.w3_offs[l], s.xb_b, dim, hidden, npos)
    gate_batch(s.hb_b, s.hb2_b, c.ffn_act, hidden, npos)
    mm_b_f(s, s.moe_gout, t, fmt_at(t.w2_fmt, l), t.w2_offs[l], s.hb_b, hidden, dim, npos)
    rms_batch(s.moe_gout, s.moe_gout, t, t.rms_post_ffn1_off + l * dim, dim, npos)
    prof_add("mm_shexp", ts_sh)

    // 6. combine (routed + dense), shared post_ffw_norm, residual add; layer-output scale is the caller's job
    add_inplace(unsafe(addr(s.xb2_b[0])), unsafe(addr(s.moe_gout[0])), npos * dim)
    rms_batch(s.xb2_b, s.xb2_b, t, t.rms_post_ffn_off + l * dim, dim, npos)
    let ts_resid = prof_ticks()
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
    prof_add("act_resid", ts_resid)
}
[init]
def private ple_hooks_register() {
    register_ple_pre_decode(@@ple_pre_decode)
    register_ple_pre_prefill(@@ple_pre_prefill_impl)
    register_ple_pre_prefill_pad(@@ple_pre_prefill_pad_impl)
}
