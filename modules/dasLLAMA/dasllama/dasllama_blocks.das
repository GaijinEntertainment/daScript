options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_blocks shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common
require dasllama/dasllama_moe   // moe_select_core/moe_ffn_core/moe_shexp_gate — the router the MoE blocks drive
require dasllama/dasllama_attn_prefill   // prefill_attention — the head-threaded prefill the std/dense blocks call
require dasllama/dasllama_math
require dasllama/dasllama_gpu_math
require dasllama/dasllama_par
require dasllama/dasllama_convert
require dasllama/dasllama_rope     // mrope_span_positions — the image-span position map
require dasllama/dasllama_kv_codec   // fwht_signs_row/fwht_unsign_row — the Hadamard rotation in gated attention
require dasllama/dasllama_env       // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require dasllama/dasllama_math_gen
require dasllama/dasllama_gemm_schema
require math

// The std/dense/MoE transformer block kernels — attention and FFN, decode and prefill — plus the
// two default block sets arch files bind. forward() never names these: it dispatches through the
// ArchBlocks function pointers, which is why the whole family lifts out of the hub without a hook.

// ===== std/dense block kernels (decode, single-token) =====
// The default llama-family blocks; a new arch registers a different block without touching forward().

// Flash-decode slicing (fused attention chain): a context deeper than FLASH_DECODE_SLICE splits
// each head's KV pass into ceil(cnt/SLICE_LEN) slices with a per-head combine stage. Slice count
// derives from CONTEXT ONLY (deterministic); ctx <= 256 stays bit-identical, deeper is tolerance-equal.
let FLASH_DECODE_SLICE = 256l       // single-pass threshold: cnt <= this never slices
let FLASH_DECODE_SLICE_LEN = 32l    // slice granularity once slicing engages
let FLASH_DECODE_MAX_SLICES = 32l   // 4 chunks/lane at 16L: Qwen2.5-1.5B d4k attn wall 393->333us/layer vs cap 16

// attn-norm -> QKV(+bias) -> RoPE -> KV-store -> GQA attention -> out-proj(+post-norm) -> residual.
// Can layer l's attention run as the fused chain? Std shape only: Q8 weights, group3 layout, table
// rope, team mode + a backend row core. pre_post_norm defers the residual add to a serial tail.
def private fused_attn_ok(t : Model; l : int64) : bool {
    let c = t.config
    let fq = fmt_at(t.wq_fmt, l)
    let fk = fmt_at(t.wk_fmt, l)
    let fv = fmt_at(t.wv_fmt, l)
    let fo = fmt_at(t.wo_fmt, l)
    let any_kq = fq != KqFmt.q8 || fk != KqFmt.q8 || fv != KqFmt.q8 || fo != KqFmt.q8
    let head_size = layer_head_size(c, l)
    let qd = layer_qd(c, l)
    // head_size % 32: the wo-input requant needs whole 32-quant groups per head (stories15M's 48 falls back)
    return (g_fused_decode && t.quant == QuantMode.q8 && g_use_rope_table
        && t.wv_offs[l] >= 0l && head_size % 32l == 0l
        // partial rotary (glm4moe): fall back to attention_std_decode, which has the rot-aware leaves
        && (c.rope_dim == 0l || c.rope_dim == head_size)
        && kq_servable(t, fq) && kq_servable(t, fk) && kq_servable(t, fv) && kq_servable(t, fo)
        // kq v2 regrain: Q8_K quantizes whole 256-superblocks, so qd and the head grouping must divide out
        && (!any_kq || c.dim % 256l == 0l)
        && (fo == KqFmt.q8 || (qd % 256l == 0l && (head_size >= 256l ? head_size % 256l == 0l : 256l % head_size == 0l)))
        && get_jobque_team_mode() && kernel_backend_has_rows()
        && (!t.wscale_f16 || kernel_backend_has_rows16())
        && lanes_for_work(c.dim * (qd + 2l * layer_kv_dim(c, l)), g_gemv_lane_cap) > 1)
}

// Fused attention block: ONE team rendezvous runs [qkv rows] -> [head-slot norm/rope/kv-store] ->
// [attn heads + wo-input requant] -> [wo rows + residual add] as four stages. Bit-identical to
// attention_std_decode at ctx <= FLASH_DECODE_SLICE; deeper contexts are tolerance-equal, not bit-equal.
def private attention_chain_decode(t : Model; var s : Session; l : int64; pos : int64) { // nolint:STYLE037,STYLE038 — the fused attention chain is one register-coupled sequence; splitting it spills the live state it exists to keep
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let is_sliding = layer_is_sliding(c, l)
    let kstart = (is_sliding && pos + 1l > c.sliding_window) ? (pos + 1l - c.sliding_window) : 0l
    let cnt = pos + 1l - kstart
    let nrun = kv_runs(s, kstart, pos + 1l, s.kv_runs)   // physical runs of this layer's window (flat: 1)
    let prow = kv_phys_row(s, pos)                       // where THIS position's row lives
    // per-weight formats (kq layers reach here only off a repacked load — fused_attn_ok)
    let fq = fmt_at(t.wq_fmt, l)
    let fk = fmt_at(t.wk_fmt, l)
    let fv = fmt_at(t.wv_fmt, l)
    let fo = fmt_at(t.wo_fmt, l)
    let any_kq = fq != KqFmt.q8 || fk != KqFmt.q8 || fv != KqFmt.q8 || fo != KqFmt.q8
    // attention rmsnorm + input quantize: one image per consumed form — q8 reads Q8_0, kq reads Q8_K (kq v2)
    let ts_norm0 = prof_ticks()
    rms_at(s.xb, t, t.rms_att_off + l * dim, s.x, dim)
    if (fq == KqFmt.q8 || fk == KqFmt.q8 || fv == KqFmt.q8) {
        quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
    }
    if (fq != KqFmt.q8 || fk != KqFmt.q8 || fv != KqFmt.q8) {
        quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
    }
    prof_add("norm", ts_norm0)
    let ts_chain = prof_ticks()
    trace_tag(TRACE_TAG_ATTN_CHAIN)
    // stage geometry (same grain math as the per-op dispatches; the C++ side clamps chunks <= range)
    let nslots = int(n_heads + 2l * n_kv_heads)
    let ngq = int((qd + 2l * kv_dim) / 32l)   // stage-0 domain: 32-row groups (head_size % 32 == 0 by the fuse gate)
    let n0 = chain_stage_chunks(ngq, int(qd + 2l * kv_dim), dim * (qd + 2l * kv_dim))
    let n1 = get_dispatch_lanes()
    let ngrp = int(dim / 32l)
    let n2 = chain_stage_chunks(ngrp, int(dim), qd * dim)
    // attn-stage unit: a kq wo requants the output per HEAD GROUP spanning whole 256-superblocks
    let hpu = (fo != KqFmt.q8 && head_size < 256l) ? 256l / head_size : 1l
    let nhu = int(n_heads / hpu)
    // flash-decode slicing engages once cnt crosses FLASH_DECODE_SLICE (ctx-derived, deterministic); nsl == 1 keeps the exact single-pass body
    let nsl = cnt <= FLASH_DECODE_SLICE ? 1 : int(min(FLASH_DECODE_MAX_SLICES, (cnt + FLASH_DECODE_SLICE_LEN - 1l) / FLASH_DECODE_SLICE_LEN))
    let slice_len = (cnt + int64(nsl) - 1l) / int64(nsl)
    if (nsl > 1) {
        let need = int(n_heads) * nsl
        if (length(s.fl_m) < need) {
            s.fl_m |> resize(need)
            s.fl_l |> resize(need)
        }
        let needo = int(n_heads * int64(nsl) * head_size)
        if (length(s.fl_o) < needo) {
            s.fl_o |> resize(needo)
        }
        s.chain_stages |> resize(5)
        s.chain_stages[0] = int3(0, ngq, n0)
        s.chain_stages[1] = int3(0, nslots, nslots)
        s.chain_stages[2] = int3(0, nsl, nsl)
        s.chain_stages[3] = int3(0, nhu, nhu)
        s.chain_stages[4] = int3(0, ngrp, n2)
        count_parallel_dispatch(min(n0, ngq) + nslots + nsl + nhu + min(n2, ngrp))
    } else {
        s.chain_stages |> resize(4)
        s.chain_stages[0] = int3(0, ngq, n0)
        s.chain_stages[1] = int3(0, nslots, nslots)
        s.chain_stages[2] = int3(0, nhu, n1)
        s.chain_stages[3] = int3(0, ngrp, n2)
        count_parallel_dispatch(min(n0, ngq) + nslots + min(n1, nhu) + min(n2, ngrp))
    }
    let mmrows = mm_q8q8_rows_fn()
    let neox = c.rope_neox
    let eps = c.norm_eps
    let seq_len = c.seq_len
    let attn_softcap = c.attn_logit_softcap
    let kdt = s.kv_dtype_k
    let vdt = s.kv_dtype_v
    // Gemma2/3 post-attention norm is a whole-vector reduction, so the residual add lands serially after
    let post_norm = c.pre_post_norm
    unsafe {
        // activations / scratch — raw pointers only (the stage lambda runs in fork contexts)
        var qp = addr(s.q[0])
        var kp = addr(s.k[0])
        var vp = addr(s.v[0])
        var xbp = addr(s.xb[0])
        var xb2p = addr(s.xb2[0])
        var xp = addr(s.x[0])
        var attp = addr(s.att[0])
        var xqp = addr(s.xq[0])
        var xsp = addr(s.xs[0])
        // block-codec K: each stage-0 q slot fills its quantized-query plane after rope; tq4 rotates first
        let kq8 = kdt == KVDtype.q8_0 || kdt == KVDtype.tq4
        let ktq4 = kdt == KVDtype.tq4
        let vtq4 = vdt == KVDtype.tq4
        let sgp = (ktq4 || vtq4) ? addr < float const? >(s.kv_signs[0]) : null
        var attqp = kq8 ? addr(s.attq[0]) : null
        var attsp = kq8 ? addr(s.atts[0]) : null
        var kcl = kv_layer_kbase(s, l, c.seq_len)  // layer-base byte pointers (stores + the _d read dispatch)
        var vcl = kv_layer_vbase(s, l, c.seq_len)
        let runsp = addr < KVRun const? >(s.kv_runs[0])  // this layer's physical runs (fork-safe hoist)
        // weight region bases (element offsets into the Q8 blob); all-kq layers skip q8 bases (may be EMPTY)
        let f16w = t.wscale_f16
        let mmrows16 = mm_q8q8_rows16_fn()
        let has_q8w = fq == KqFmt.q8 || fk == KqFmt.q8 || fv == KqFmt.q8 || fo == KqFmt.q8
        let wqb = has_q8w ? addr < int8 const? >(t.qblob[0]) : null
        var wsq : float const? = null
        var wsk : float const? = null
        var wsv : float const? = null
        var wso : float const? = null
        var wsq16 : uint16 const? = null
        var wsk16 : uint16 const? = null
        var wsv16 : uint16 const? = null
        var wso16 : uint16 const? = null
        if (f16w && has_q8w) {
            let wsb16 = addr < uint16 const? >(t.qscales16[0])
            wsq16 = wsb16 + t.wq_offs[l] / 32l
            wsk16 = wsb16 + t.wk_offs[l] / 32l
            wsv16 = wsb16 + t.wv_offs[l] / 32l
            wso16 = wsb16 + t.wo_offs[l] / 32l
        } elif (has_q8w) {
            let wsb = addr < float const? >(t.qscales[0])
            wsq = wsb + t.wq_offs[l] / 32l
            wsk = wsb + t.wk_offs[l] / 32l
            wsv = wsb + t.wv_offs[l] / 32l
            wso = wsb + t.wo_offs[l] / 32l
        }
        let wpq = wqb + t.wq_offs[l]
        let wpk = wqb + t.wk_offs[l]
        let wpv = wqb + t.wv_offs[l]
        let wpo = wqb + t.wo_offs[l]
        // kq-tagged weights: plane bases + stamped rows core per format + the Q8_K activation image (kq v2)
        var kxqp : int8? = any_kq ? addr(s.kxq[0]) : null
        var kxsp : float? = any_kq ? addr(s.kxs[0]) : null
        var kxbsp : int? = any_kq ? addr(s.kxbs[0]) : null
        let kqpq = kq_plane_q(t, fq, t.wq_offs[l])
        let kspq = kq_plane_s(t, fq, t.wq_offs[l])
        let rowsq = kq_rows_for(fq)
        let kqpk = kq_plane_q(t, fk, t.wk_offs[l])
        let kspk = kq_plane_s(t, fk, t.wk_offs[l])
        let rowsk = kq_rows_for(fk)
        let kqpv = kq_plane_q(t, fv, t.wv_offs[l])
        let kspv = kq_plane_s(t, fv, t.wv_offs[l])
        let rowsv = kq_rows_for(fv)
        let kqpo = kq_plane_q(t, fo, t.wo_offs[l])
        let kspo = kq_plane_s(t, fo, t.wo_offs[l])
        let rowso = kq_rows_for(fo)
        let scq = site_scale(t, fq, t.wq_offs[l])
        let sck = site_scale(t, fk, t.wk_offs[l])
        let scv = site_scale(t, fv, t.wv_offs[l])
        let sco = site_scale(t, fo, t.wo_offs[l])
        // this position's rope table row (dual-rope arches: the sliding class's row)
        let swa_row = is_sliding && has_dual_rope(c)
        let crow = swa_row ? addr < float const? >(s.rope_cos_swa[0]) : addr < float const? >(s.rope_cos[0])
        let srow = swa_row ? addr < float const? >(s.rope_sin_swa[0]) : addr < float const? >(s.rope_sin[0])
        // optional per-arch epilogues (null = off): qkv bias, per-head q/k norm, weightless v norm, sinks, wo bias
        let bqp = c.attn_qkv_bias ? addr < float const? >(t.bq[0]) : null
        let bkp = c.attn_qkv_bias ? addr < float const? >(t.bk[0]) : null
        let bvp = c.attn_qkv_bias ? addr < float const? >(t.bv[0]) : null
        let rqp = c.qk_norm ? addr < float const? >(t.fblob[t.rms_q_offs[l]]) : null
        let rkp = c.qk_norm ? addr < float const? >(t.fblob[t.rms_k_offs[l]]) : null
        let rvp = c.v_norm ? addr < float const? >(t.fblob[t.rms_ones_off]) : null
        let sinksp = c.attn_sinks ? addr < float const? >(t.fblob[t.sinks_off + l * n_heads]) : null
        let bop = c.attn_out_bias ? addr < float const? >(t.bo[0]) : null
        let bqoff = l * qd
        let bkvoff = l * kv_dim
        let booff = l * dim
        // flash-decode partial planes (deep ctx only; the arrays are empty when nsl == 1)
        var flmp = nsl > 1 ? addr(s.fl_m[0]) : null
        var fllp = nsl > 1 ? addr(s.fl_l[0]) : null
        var flop = nsl > 1 ? addr(s.fl_o[0]) : null
        team_parallel_stages(s.chain_stages) <| @(stage : int; jb : int; je : int) {   // nolint:PERF026 — the lambda's capture frame is heap per call: the dispatch API's cost, not this kernel's
            if (stage == 0) {
                // qkv rows in 32-row groups (row-local); a group never straddles a q/k/v region boundary
                unsafe {
                    var r = int64(jb) * 32l
                    let rend = int64(je) * 32l
                    while (r < rend) {
                        if (r < qd) {
                            let r1 = min(rend, qd)
                            if (fq != KqFmt.q8) {
                                invoke(rowsq, qp, kqpq, kspq, kxqp, kxsp, kxbsp, dim, r, r1)
                                if (scq != 1.0) {
                                    scale_inplace(qp + r, scq, r1 - r)
                                }
                            } elif (f16w) {
                                invoke(mmrows16, qp, wpq, wsq16, xqp, xsp, dim, r, r1)
                            } else {
                                invoke(mmrows, qp, wpq, wsq, xqp, xsp, dim, r, r1)
                            }
                            if (bqp != null) {
                                add_inplace(qp + r, bqp + bqoff + r, r1 - r)
                            }
                            r = r1
                        } elif (r < qd + kv_dim) {
                            let r0 = r - qd
                            let r1 = min(rend, qd + kv_dim) - qd
                            if (fk != KqFmt.q8) {
                                invoke(rowsk, kp, kqpk, kspk, kxqp, kxsp, kxbsp, dim, r0, r1)
                                if (sck != 1.0) {
                                    scale_inplace(kp + r0, sck, r1 - r0)
                                }
                            } elif (f16w) {
                                invoke(mmrows16, kp, wpk, wsk16, xqp, xsp, dim, r0, r1)
                            } else {
                                invoke(mmrows, kp, wpk, wsk, xqp, xsp, dim, r0, r1)
                            }
                            if (bkp != null) {
                                add_inplace(kp + r0, bkp + bkvoff + r0, r1 - r0)
                            }
                            r = r1 + qd
                        } else {
                            let r0 = r - qd - kv_dim
                            let r1 = min(rend, qd + 2l * kv_dim) - qd - kv_dim
                            if (fv != KqFmt.q8) {
                                invoke(rowsv, vp, kqpv, kspv, kxqp, kxsp, kxbsp, dim, r0, r1)
                                if (scv != 1.0) {
                                    scale_inplace(vp + r0, scv, r1 - r0)
                                }
                            } elif (f16w) {
                                invoke(mmrows16, vp, wpv, wsv16, xqp, xsp, dim, r0, r1)
                            } else {
                                invoke(mmrows, vp, wpv, wsv, xqp, xsp, dim, r0, r1)
                            }
                            if (bvp != null) {
                                add_inplace(vp + r0, bvp + bkvoff + r0, r1 - r0)
                            }
                            r = r1 + qd + kv_dim
                        }
                    }
                }
            } elif (stage == 1) {
                // head-local epilogue by head slot: norm -> rope -> cache store -> roped-query quantize
                unsafe {
                    for (slot in range(jb, je)) {
                        let sl = int64(slot)
                        if (sl < n_heads) {
                            let r0 = sl * head_size
                            if (rqp != null) {
                                rmsnorm(qp + r0, qp + r0, rqp, head_size, eps)
                            }
                            if (neox) {
                                rope_scaled_neox_tab(qp + r0, head_size, head_size, crow, srow)
                            } else {
                                rope_scaled_tab(qp + r0, head_size, head_size, crow, srow)
                            }
                            if (ktq4) {   // tq4: rotate the roped query head before the plane fill
                                fwht_signs_row(qp + r0, sgp, head_size)
                            }
                            if (kq8) {   // quantize this head's roped query for the int8 score dot
                                quantize_q8_0_into_ptr(qp + r0, head_size, attqp, attsp, r0, r0 / 32l)
                            }
                        } elif (sl < n_heads + n_kv_heads) {
                            let r0 = (sl - n_heads) * head_size
                            if (rkp != null) {
                                rmsnorm(kp + r0, kp + r0, rkp, head_size, eps)
                            }
                            if (neox) {
                                rope_scaled_neox_tab(kp + r0, head_size, head_size, crow, srow)
                            } else {
                                rope_scaled_tab(kp + r0, head_size, head_size, crow, srow)
                            }
                            if (ktq4) {   // tq4: rotate the roped key head before the codec store
                                fwht_signs_row(kp + r0, sgp, head_size)
                            }
                            kv_store_row(kcl, kdt, kv_dim, prow, r0, head_size, kp + r0)
                        } else {
                            let r0 = (sl - n_heads - n_kv_heads) * head_size
                            if (rvp != null) {
                                rmsnorm(vp + r0, vp + r0, rvp, head_size, eps)
                            }
                            if (vtq4) {   // tq4: rotate the value head before the codec store
                                fwht_signs_row(vp + r0, sgp, head_size)
                            }
                            kv_store_row(vcl, vdt, kv_dim, prow, r0, head_size, vp + r0)
                        }
                    }
                }
            } elif (stage == 2 && nsl == 1) {
                // multi-head attention + requant of the wo input; overwrites input quants (stage 0's only reader is done)
                unsafe {
                    for (u in range(jb, je)) {
                        for (g in range64(hpu)) {
                            let hh = int64(u) * hpu + g
                            let qoff = hh * head_size
                            let kvhoff = (hh / kv_mul) * head_size  // GQA: which kv head this query head reads
                            attn_head_decode_d(attp + hh * seq_len, xbp, qp, attqp, attsp, kcl, vcl, kdt, vdt, runsp,
                                nrun, qoff, kvhoff, kstart, cnt, kv_dim, head_size, hh, scale, attn_softcap, sinksp)
                            if (vtq4) {   // tq4 V: this head's xb slice is rotated-basis — un-rotate before requant
                                fwht_unsign_row(xbp + qoff, sgp, head_size)
                            }
                        }
                        let u0 = int64(u) * hpu * head_size
                        if (fo != KqFmt.q8) {   // kq wo: Q8_K over the unit's whole-superblock span
                            quantize_q8_k_into_ptr(xbp + u0, hpu * head_size, kxqp, kxsp, kxbsp, u0, u0 / 256l, u0 / 16l)
                        } else {
                            quantize_q8_0_into_ptr(xbp + u0, hpu * head_size, xqp, xsp, u0, u0 / 32l)
                        }
                    }
                }
            } elif (stage == 2) {
                // flash-decode partials: unit = one slice over EVERY kv head - each K/V row streams once, whole
                unsafe {
                    for (u in range(jb, je)) {
                        let sl = int64(u)
                        let j0 = sl * slice_len
                        let j1 = min(j0 + slice_len, cnt)
                        for (kvh in range64(n_kv_heads)) {
                        let kvhoff = kvh * head_size
                        let qh0 = kvh * kv_mul
                        // K pass: each K row read once, dotted against every group q-head
                        kv_slice_scores_d(attp, qp, attqp, attsp, kcl, kdt, runsp, nrun, kstart, j0, j1, kv_dim, kvhoff, head_size,
                            qh0, kv_mul, seq_len, scale)
                        for (g in range64(kv_mul)) {
                            let hh = qh0 + g
                            var arow = attp + hh * seq_len
                            if (attn_softcap > 0.0) {
                                softcap4(arow + j0, j1 - j0, attn_softcap)
                            }
                            // hand-vectorized max reduction — order-independent, bit-identical to the scalar scan
                            let m = hmax(unsafe(arow + j0), j1 - j0)
                            // exp4 fast path, clamped to -80 like the prefill flash fold (underflow -> ~0)
                            var lsum = 0.0
                            {
                                let m4 = float4(m, m, m, m)
                                var sum4 = float4(0.0)
                                var j = j0
                                while (j + 4l <= j1) {
                                    let e = exp4(max(float4(arow[j], arow[j + 1l], arow[j + 2l], arow[j + 3l]) - m4, EXP4_LO))
                                    arow[j] = e.x
                                    arow[j + 1l] = e.y
                                    arow[j + 2l] = e.z
                                    arow[j + 3l] = e.w
                                    sum4 += e
                                    j += 4l
                                }
                                lsum = sum4.x + sum4.y + sum4.z + sum4.w
                                while (j < j1) {
                                    let e = exp(max(arow[j] - m, -80.0))
                                    arow[j] = e
                                    lsum += e
                                    j++
                                }
                            }
                            var op = flop + (hh * int64(nsl) + sl) * head_size
                            for (i in range64(head_size)) {
                                op[i] = 0.0
                            }
                            flmp[hh * int64(nsl) + sl] = m
                            fllp[hh * int64(nsl) + sl] = lsum
                        }
                        // V pass: each V row read once, weighted into every group head's partial
                        kv_slice_av_d(flop, attp, vcl, vdt, runsp, nrun, kstart, j0, j1, kv_dim, kvhoff, head_size,
                            qh0, kv_mul, seq_len, int64(nsl), sl)
                        }
                    }
                }
            } elif (stage == 3 && nsl > 1) {
                // per-head combine (flash rescale; sink joins max + denominator) + wo-input requant per unit span
                unsafe {
                    for (u in range(jb, je)) {
                        for (g in range64(hpu)) {
                            let hh = int64(u) * hpu + g
                            let qoff = hh * head_size
                            let fbase = hh * int64(nsl)
                            var mg = hmax(unsafe(flmp + fbase), int64(nsl))
                            if (sinksp != null) {
                                mg = max(mg, sinksp[hh])
                            }
                            for (i in range64(head_size)) {
                                xbp[qoff + i] = 0.0
                            }
                            var lg = 0.0
                            {
                                // exp4 over the slice scale factors (nsl <= 16), flash-fold clamp convention
                                let mg4 = float4(mg, mg, mg, mg)
                                var sl = 0l
                                while (sl + 4l <= int64(nsl)) {
                                    let f = exp4(max(float4(flmp[fbase + sl], flmp[fbase + sl + 1l], flmp[fbase + sl + 2l], flmp[fbase + sl + 3l]) - mg4, EXP4_LO))
                                    lg += (fllp[fbase + sl] * f.x + fllp[fbase + sl + 1l] * f.y
                                        + fllp[fbase + sl + 2l] * f.z + fllp[fbase + sl + 3l] * f.w)
                                    axpy(xbp + qoff, flop + (fbase + sl) * head_size, f.x, head_size)
                                    axpy(xbp + qoff, flop + (fbase + sl + 1l) * head_size, f.y, head_size)
                                    axpy(xbp + qoff, flop + (fbase + sl + 2l) * head_size, f.z, head_size)
                                    axpy(xbp + qoff, flop + (fbase + sl + 3l) * head_size, f.w, head_size)
                                    sl += 4l
                                }
                                while (sl < int64(nsl)) {
                                    let f = exp(max(flmp[fbase + sl] - mg, -80.0))
                                    lg += fllp[fbase + sl] * f
                                    axpy(xbp + qoff, flop + (fbase + sl) * head_size, f, head_size)
                                    sl++
                                }
                            }
                            if (sinksp != null) {
                                lg += exp(max(sinksp[hh] - mg, -80.0))
                            }
                            let inv = 1.0 / lg
                            for (i in range64(head_size)) {
                                xbp[qoff + i] *= inv
                            }
                            if (vtq4) {   // tq4 V: combined partials are rotated-basis — un-rotate before requant
                                fwht_unsign_row(xbp + qoff, sgp, head_size)
                            }
                        }
                        let u0 = int64(u) * hpu * head_size
                        if (fo != KqFmt.q8) {   // kq wo: Q8_K over the unit's whole-superblock span
                            quantize_q8_k_into_ptr(xbp + u0, hpu * head_size, kxqp, kxsp, kxbsp, u0, u0 / 256l, u0 / 16l)
                        } else {
                            quantize_q8_0_into_ptr(xbp + u0, hpu * head_size, xqp, xsp, u0, u0 / 32l)
                        }
                    }
                }
            } else {
                // output projection rows + optional bias + residual add, all chunk-local (post-norm arches defer the add)
                unsafe {
                    let r0 = int64(jb) * 32l
                    let r1 = int64(je) * 32l
                    if (fo != KqFmt.q8) {
                        invoke(rowso, xb2p, kqpo, kspo, kxqp, kxsp, kxbsp, qd, r0, r1)
                        if (sco != 1.0) {
                            scale_inplace(xb2p + r0, sco, r1 - r0)
                        }
                    } elif (f16w) {
                        invoke(mmrows16, xb2p, wpo, wso16, xqp, xsp, qd, r0, r1)
                    } else {
                        invoke(mmrows, xb2p, wpo, wso, xqp, xsp, qd, r0, r1)
                    }
                    if (bop != null) {
                        add_inplace(xb2p + r0, bop + booff + r0, r1 - r0)
                    }
                    if (!post_norm) {
                        add_inplace(xp + r0, xb2p + r0, r1 - r0)
                    }
                }
            }
        }
        if (nsl > 1 && g_flash_decode_check) {
            // test-only witness: sliced partials + combine must equal an exact single-pass recompute (fp32 tolerance)
            flash_decode_check_d(qp, attqp, attsp, kcl, vcl, kdt, vdt, xbp, runsp,
                nrun, n_heads, kv_mul, head_size, kv_dim, kstart, cnt, l, int64(nsl), scale, attn_softcap, sinksp,
                vtq4 ? sgp : null)
            g_flash_decode_checked++
        }
    }
    prof_add("attn_chain", ts_chain)
    if (post_norm) {  // Gemma2/3: post-attention RMSNorm before the residual add — same serial
        // tail as the un-fused path (dim-sized, microseconds), bit-identical order
        let ts_resid = prof_ticks()
        rms_at(s.xb2, t, t.rms_post_att_off + l * dim, s.xb2, dim)
        add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb2[0])), dim)
        prof_add("act_resid", ts_resid)
    }
}

var private @scratch g_dat_cossin : array<float>

var private g_dat_hyd_k : array<float>   // a non-f16/f32 KV session's rows decoded for the hydrate
var private g_dat_hyd_v : array<float>

// any codec but f16/f32: the host decodes the rows [0, pos) to f32 (kv_load_row), the block converts
[cold_path]   // a remedy leg, not the served token
def private attn_dec_hydrate_codec(var s : Session; l, pos, woq : int64; fq : int; kvd, head_size, seq_len : int64) {
    g_dat_hyd_k |> resize(pos * kvd)
    g_dat_hyd_v |> resize(pos * kvd)
    unsafe {
        let kb = kv_layer_kbase(s, l, seq_len)
        let vb = kv_layer_vbase(s, l, seq_len)
        for (r in range64(pos)) {
            kv_load_row(kb, s.kv_dtype_k, kvd, r, 0l, kvd, addr(g_dat_hyd_k[r * kvd]))
            kv_load_row(vb, s.kv_dtype_v, kvd, r, 0l, kvd, addr(g_dat_hyd_v[r * kvd]))
        }
        tq4_unrotate_from_store(s, g_dat_hyd_k, g_dat_hyd_v, pos, kvd, head_size)
        matmul_moe_gpu_attn_dec_hydrate(woq, fq, addr<uint8 const?>(g_dat_hyd_k[0]), addr<uint8 const?>(g_dat_hyd_v[0]), pos, kvd, false)
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block")]
def private attn_dec_shape_ok(t : Model; s : Session; l : int64) : bool {
    let c = t.config
    if (!moe_gpu_attn_on_gpu(l) || t.kv_src[l] != l || s.kv_pool != null || c.attn_out_bias
            || c.v_norm || c.attn_sinks || c.attn_logit_softcap != 0.0 || c.q_gated || layer_is_sliding(c, l)
            || (c.rope_dim > 0l && c.rope_dim != layer_head_size(c, l))
            || c.dim > 8192l || layer_head_size(c, l) > 256l   // the chain's add-rms stages a row of at most AR_MAX_DIM (8192), the attention a head of at most 256
            || layer_kv_dim(c, l) != layer_kv_dim(c, 0l) || layer_head_size(c, l) != layer_head_size(c, 0l)) {   // the block holds ONE geometry
        return false
    }
    let fq = fmt_at(t.wq_fmt, l)
    return kq_sb(fq) == kq_sb(fmt_at(t.wk_fmt, l)) && kq_sb(fq) == kq_sb(fmt_at(t.wv_fmt, l))
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block")]
def private attn_dec_gpu_next_serves(t : Model; s : Session; l, pos : int64) : bool {
    if (l >= t.config.n_layers || !attn_dec_shape_ok(t, s, l)) {
        return false
    }
    return moe_gpu_attn_dec_ok(t.wq_offs[l], int(fmt_at(t.wq_fmt, l)), pos, s.dat_gen) == DAT_SERVE
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block")]
def private attn_dec_gpu_serves(t : Model; var s : Session; l, pos : int64) : bool {
    let c = t.config
    if (!attn_dec_shape_ok(t, s, l)) {
        return false
    }
    let woq = t.wq_offs[l]
    let fq = int(fmt_at(t.wq_fmt, l))
    var verdict = moe_gpu_attn_dec_ok(woq, fq, pos, s.dat_gen)
    if (verdict == DAT_STALE_GEN) {   // once per token: the claim makes every later layer current
        s.dat_gen = moe_gpu_attn_dec_claim(true)
        verdict = moe_gpu_attn_dec_ok(woq, fq, pos, s.dat_gen)
    }
    if (verdict == DAT_HYDRATE) {
        let kv_is_f16 = s.kv_dtype_k == KVDtype.f16 && s.kv_dtype_v == KVDtype.f16
        let kv_is_f32 = s.kv_dtype_k == KVDtype.f32 && s.kv_dtype_v == KVDtype.f32
        let kvd = layer_kv_dim(c, l)
        if (pos == 0l || kv_is_f16 || kv_is_f32) {
            matmul_moe_gpu_attn_dec_hydrate(woq, fq, kv_layer_kbase(s, l, c.seq_len), kv_layer_vbase(s, l, c.seq_len), pos, kvd, kv_is_f16)
        } else {
            attn_dec_hydrate_codec(s, l, pos, woq, fq, kvd, layer_head_size(c, l), c.seq_len)
        }
        verdict = moe_gpu_attn_dec_ok(woq, fq, pos, s.dat_gen)
    }
    return verdict == DAT_SERVE
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block")]
def private attention_gpu_decode(t : Model; var s : Session; l : int64; pos : int64) {
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let qd = n_heads * head_size
    let kv_mul = n_heads / layer_n_kv_heads(c, l)
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let fq = fmt_at(t.wq_fmt, l)
    let ts_norm0 = prof_ticks()
    let q8 = fq == KqFmt.q8
    if (!moe_gpu_ffn_tail_pending()) {
        rms_at(s.xb, t, t.rms_att_off + l * dim, s.x, dim)
        if (q8) {
            quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
        } else {
            quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
        }
    }
    prof_add("norm", ts_norm0)
    g_dat_cossin |> resize(head_size)
    build_rope_row_packed(g_dat_cossin, 0l, pos + s.rope_pos_delta, head_size, c.rope_freq_base, c.rope_freq_scale, c.rope_mscale, t.rope_freqs)
    let ts_gpu = prof_ticks()
    unsafe {
        let rq = c.qk_norm ? addr<float const?>(t.fblob[t.rms_q_offs[l]]) : null
        let rk = c.qk_norm ? addr<float const?>(t.fblob[t.rms_k_offs[l]]) : null
        let bq = c.attn_qkv_bias ? addr<float const?>(t.bq[l * qd]) : null
        let bk = c.attn_qkv_bias ? addr<float const?>(t.bk[l * kv_dim]) : null
        let bv = c.attn_qkv_bias ? addr<float const?>(t.bv[l * kv_dim]) : null
        if (q8) {
            matmul_moe_gpu_attn_dec(s.xb2, s.k, s.v, t.wq_offs[l], t.wk_offs[l], t.wv_offs[l], t.wo_offs[l],
                int(fq), int(fmt_at(t.wk_fmt, l)), int(fmt_at(t.wv_fmt, l)), int(fmt_at(t.wo_fmt, l)),
                s.xq, s.xs, g_dat_cossin, rq, rk, bq, bk, bv, pos, dim, qd, kv_dim, head_size, n_heads, kv_mul, scale, c.norm_eps, c.rope_neox, c.qk_norm)
        } else {
            matmul_moe_gpu_attn_dec(s.xb2, s.k, s.v, t.wq_offs[l], t.wk_offs[l], t.wv_offs[l], t.wo_offs[l],
                int(fq), int(fmt_at(t.wk_fmt, l)), int(fmt_at(t.wv_fmt, l)), int(fmt_at(t.wo_fmt, l)),
                s.kxq, s.kxs, g_dat_cossin, rq, rk, bq, bk, bv, pos, dim, qd, kv_dim, head_size, n_heads, kv_mul, scale, c.norm_eps, c.rope_neox, c.qk_norm)
        }
    }
    prof_add("attn_gpu", ts_gpu)
    attn_dec_store_kv(t, s, l, pos)
    let ts_resid0 = prof_ticks()
    if (c.pre_post_norm) {
        rms_at(s.xb2, t, t.rms_post_att_off + l * dim, s.xb2, dim)
    }
    add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb2[0])), dim)
    prof_add("act_resid", ts_resid0)
    let tail_shape = (g_env_gpu.gpu_dec_tail && moe_gpu_ffn_tail_ok() && c.n_ff_shexp == 0l && !c.pre_post_norm && !c.layer_out_scale
        && !t.experts_mx4 && c.ffn_act != FfnAct.swiglu_oai && t.quant == QuantMode.q8 && c.n_expert_used <= 64l && !c.hyper_conn)
    s.tail_next1 = 0l
    if (tail_shape && !c.moe_exps_bias && l >= c.n_layer_dense_lead && moe_gpu_layer_on_gpu(l) && attn_dec_gpu_next_serves(t, s, l + 1l, pos)) {
        s.tail_next1 = l + 1l
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block")]
def private attn_dec_store_kv(t : Model; var s : Session; l, pos : int64) {
    let c = t.config
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let ts_kv = prof_ticks()
    if (c.attn_qkv_bias) {   // the block returns the projection rows before the bias; the cache row carries it, as on the CPU path
        add_bias(s.k, 0l, t.bk, l * kv_dim, kv_dim)
        add_bias(s.v, 0l, t.bv, l * kv_dim, kv_dim)
    }
    if (c.qk_norm) {
        rms_batch(s.k, s.k, t, t.rms_k_offs[l], head_size, layer_n_kv_heads(c, l))
    }
    unsafe {
        let crow = addr<float const?>(g_dat_cossin[0])
        let srow = addr<float const?>(g_dat_cossin[head_size / 2l])
        if (c.rope_neox) {
            rope_scaled_neox_tab(addr(s.k[0]), head_size, kv_dim, crow, srow)
        } else {
            rope_scaled_tab(addr(s.k[0]), head_size, kv_dim, crow, srow)
        }
    }
    tq4_rotate_for_store(s, s.k, s.v, 1l, kv_dim, head_size)
    var kcl = kv_layer_kbase(s, l, c.seq_len)
    var vcl = kv_layer_vbase(s, l, c.seq_len)
    unsafe {
        kv_store_row(kcl, s.kv_dtype_k, kv_dim, kv_phys_row(s, pos), 0l, kv_dim, addr<float const?>(s.k[0]))
        kv_store_row(vcl, s.kv_dtype_v, kv_dim, kv_phys_row(s, pos), 0l, kv_dim, addr<float const?>(s.v[0]))
    }
    prof_add("kv_store", ts_kv)
}

// ===== the whole-token decode override: the resident suffix as one device chain =====

var private g_span_args : MoeSpanDec   // reused per token: the per-layer arrays keep their capacity
var private g_span_k : array<float>
var private g_span_v : array<float>
var private g_span_tokens = 0l   //! tokens the span served - the tests' engage witness (read on the main context only)
def public span_tokens() : int64 => g_span_tokens

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]
def private span_first_layer(t : Model; var s : Session; pos : int64) : int64 {
    let c = t.config
    var l0 = c.n_layers
    while (l0 > 0l) {
        let l = l0 - 1l
        if (l < c.n_layer_dense_lead || !moe_gpu_layer_on_gpu(l) || kq_sb(fmt_at(t.we1_fmt, l)) != kq_sb(fmt_at(t.we3_fmt, l))
                || (c.n_ff_shexp > 0l && (!moe_gpu_shexp_on_gpu(l) || shexp_gpu_mixed_forms(l))) || !attn_dec_gpu_serves(t, s, l, pos)) {
            break
        }
        l0 = l
    }
    return l0
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]
def private span_model_ok(t : Model) : bool {
    let c = t.config
    if (!g_env_gpu.gpu_dec_span || !moe_gpu_span_ok() || t.quant != QuantMode.q8 || c.n_expert == 0l
            || c.n_expert_used > 64l || c.n_expert > 256l || c.moe_gate != MoeGate.softmax || c.moe_router_bias || c.moe_exp_probs
            || c.n_layers * c.n_expert * c.dim * 4l > moe_gpu_binding_cap(1l) || !t.blocks.std_chain
            || (c.n_ff_shexp > 0l && c.moe_dense_shexp) || c.pre_post_norm || c.layer_out_scale || c.moe_exps_bias || t.experts_mx4) {
        return false
    }
    return c.ffn_act != FfnAct.swiglu_oai && !has_ple(c) && c.n_layer_nextn == 0l
}

[cold_path]   // once per span shape: the arrays keep their capacity
def private span_fill_layers(t : Model; var a : MoeSpanDec; l0, nl : int64) {
    let c = t.config
    let dim = c.dim
    a.woq |> resize(nl)
    a.wok |> resize(nl)
    a.wov |> resize(nl)
    a.woo |> resize(nl)
    a.fq |> resize(nl)
    a.fk |> resize(nl)
    a.fv |> resize(nl)
    a.fo |> resize(nl)
    a.we1 |> resize(nl)
    a.we3 |> resize(nl)
    a.we2 |> resize(nl)
    a.f1 |> resize(nl)
    a.f3 |> resize(nl)
    a.f2 |> resize(nl)
    a.wsh1 |> reserve(nl)
    a.wsh1 |> resize(nl)
    a.wsh3 |> reserve(nl)
    a.wsh3 |> resize(nl)
    a.wsh2 |> reserve(nl)
    a.wsh2 |> resize(nl)
    a.fsh1 |> resize(nl)
    a.fsh3 |> resize(nl)
    a.fsh2 |> resize(nl)
    a.rmsq |> resize(nl)
    a.rmsk |> resize(nl)
    a.bq |> reserve(nl)
    a.bq |> resize(nl)
    a.bk |> reserve(nl)
    a.bk |> resize(nl)
    a.bv |> reserve(nl)
    a.bv |> resize(nl)
    unsafe {
        for (li in range64(nl)) {
            let l = l0 + li
            a.woq[li] = t.wq_offs[l]
            a.wok[li] = t.wk_offs[l]
            a.wov[li] = t.wv_offs[l]
            a.woo[li] = t.wo_offs[l]
            a.fq[li] = int(fmt_at(t.wq_fmt, l))
            a.fk[li] = int(fmt_at(t.wk_fmt, l))
            a.fv[li] = int(fmt_at(t.wv_fmt, l))
            a.fo[li] = int(fmt_at(t.wo_fmt, l))
            a.we1[li] = t.we1_offs[l]
            a.we3[li] = t.we3_offs[l]
            a.we2[li] = t.we2_offs[l]
            a.f1[li] = int(fmt_at(t.we1_fmt, l))
            a.f3[li] = int(fmt_at(t.we3_fmt, l))
            a.f2[li] = int(fmt_at(t.we2_fmt, l))
            let sp = moe_gpu_shexp_planes(l)   //! the planes the tier holds for the layer's shared expert (none where it has none)
            a.wsh1[li] = c.n_ff_shexp > 0l ? sp.o1 : -1l
            a.wsh3[li] = c.n_ff_shexp > 0l ? sp.o3 : -1l
            a.wsh2[li] = c.n_ff_shexp > 0l ? sp.o2 : -1l
            a.fsh1[li] = sp.f1
            a.fsh3[li] = sp.f3
            a.fsh2[li] = sp.f2
            a.rmsq[li] = c.qk_norm ? addr<float const?>(t.fblob[t.rms_q_offs[l]]) : null
            a.rmsk[li] = c.qk_norm ? addr<float const?>(t.fblob[t.rms_k_offs[l]]) : null
            a.bq[li] = c.attn_qkv_bias ? addr<float const?>(t.bq[l * a.qd]) : null
            a.bk[li] = c.attn_qkv_bias ? addr<float const?>(t.bk[l * a.kvd]) : null
            a.bv[li] = c.attn_qkv_bias ? addr<float const?>(t.bv[l * a.kvd]) : null
        }
        a.router = addr<float const?>(t.fblob[t.router_off + l0 * c.n_expert * dim])
        a.shgate = c.n_ff_shexp > 0l && c.moe_shexp_gated ? addr<float const?>(t.fblob[t.shexp_gate_off + l0 * dim]) : null
        a.att_norm = addr<float const?>(t.fblob[t.rms_att_off + l0 * dim])
        a.ffn_norm = addr<float const?>(t.fblob[t.rms_ffn_off + l0 * dim])
        a.blob = addr<float const?>(t.fblob[0])
    }
    g_span_k |> resize(nl * layer_kv_dim(c, l0))
    g_span_v |> resize(nl * layer_kv_dim(c, l0))
}

//! the cached span arguments still describe (t, l0, nl): the same carrier (the router and norm
//! rows are raw pointers into it), the same suffix length and the same first attention offset
def public span_args_current(a : MoeSpanDec; t : Model; l0, nl : int64) : bool {
    if (nl <= 0l || long_length(a.woq) != nl || a.woq[0] != t.wq_offs[l0]) {
        return false
    }
    var blob : float const?
    if (!empty(t.fblob)) {
        blob = unsafe(addr<float const?>(t.fblob[0]))
    }
    return a.blob == blob
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]
def private span_run(t : Model; var s : Session; pos, l0 : int64) {
    let c = t.config
    let dim = c.dim
    let nl = c.n_layers - l0
    let hs = layer_head_size(c, l0)
    let kvd = layer_kv_dim(c, l0)
    var a & = g_span_args
    a.pos = pos
    a.dim = dim
    a.qd = c.n_heads * hs
    a.kvd = kvd
    a.hs = hs
    a.n_heads = c.n_heads
    a.kv_mul = c.n_heads / layer_n_kv_heads(c, l0)
    a.ne = c.n_expert
    a.k = c.n_expert_used
    a.nfe = c.n_ff_exp
    a.scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(hs))
    a.eps = c.norm_eps
    a.wscale = c.expert_weights_scale
    a.neox = c.rope_neox
    a.qk_norm = c.qk_norm
    a.norm_topk = c.norm_topk_prob
    a.is_gelu = c.ffn_act == FfnAct.gelu
    a.nsh = c.n_ff_shexp
    a.sh_gated = c.moe_shexp_gated
    if (!span_args_current(a, t, l0, nl)) {
        span_fill_layers(t, a, l0, nl)
    }
    let ts_norm = prof_ticks()
    rms_at(s.xb, t, t.rms_att_off + l0 * dim, s.x, dim)
    unsafe {
        if (fmt_at(t.wq_fmt, l0) == KqFmt.q8) {
            quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
            a.xq = addr<int8 const?>(s.xq[0])
            a.xs = addr<float const?>(s.xs[0])
        } else {
            quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
            a.xq = addr<int8 const?>(s.kxq[0])
            a.xs = addr<float const?>(s.kxs[0])
        }
    }
    prof_add("norm", ts_norm)
    g_dat_cossin |> resize(hs)
    build_rope_row_packed(g_dat_cossin, 0l, pos + s.rope_pos_delta, hs, c.rope_freq_base, c.rope_freq_scale, c.rope_mscale, t.rope_freqs)
    unsafe {
        a.cossin = addr<float const?>(g_dat_cossin[0])
        a.x = addr(s.x[0])
        a.kout = addr(g_span_k[0])
        a.vout = addr(g_span_v[0])
    }
    let ts_span = prof_ticks()
    matmul_moe_gpu_span_dec(a)
    prof_add("span_gpu", ts_span)
    for (li in range64(nl)) {
        copy_floats(g_span_k, li * kvd, s.k, 0l, kvd)
        copy_floats(g_span_v, li * kvd, s.v, 0l, kvd)
        attn_dec_store_kv(t, s, l0 + li, pos)
    }
}

[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]   // per token
def private moe_span_decode(t : Model; var s : Session; _token, pos : int64) : bool {
    let c = t.config
    if (!span_model_ok(t)) {
        return false
    }
    let l0 = span_first_layer(t, s, pos)
    if (c.n_layers - l0 < 2l || (l0 > 0l && moe_gpu_layer_on_gpu(l0 - 1l))) {
        return false
    }
    for (l in range64(l0)) {
        let ts_blk_attn = prof_ticks()
        t.blocks.attn_decode(t, s, l, pos)
        prof_add("blk_attn", ts_blk_attn)
        let ts_blk_ffn = prof_ticks()
        t.blocks.ffn_decode(t, s, l)
        prof_add("blk_ffn", ts_blk_ffn)
    }
    span_run(t, s, pos, l0)
    g_span_tokens++
    return true
}

//! registered at init, dormant until the MoE placement selects it by name; dastest runs no
//! [init], so a test registers through this entry and then selects - never calls the override
[init]
def dasllama_blocks_span_register() {
    register_decode_override("vulkan_moe_span", @@moe_span_decode)
}

def attention_std_decode(t : Model; var s : Session; l : int64; pos : int64) { // nolint:STYLE037,STYLE038 — dispatches every attention flavour (fused/flash/blocked/classic, GPU or CPU) from one place; the ladder IS the contract
    if (attn_dec_gpu_serves(t, s, l, pos)) {
        attention_gpu_decode(t, s, l, pos)
        return
    }
    if (fused_attn_ok(t, l)) {
        attention_chain_decode(t, s, l, pos)
        return
    }
    trace_tag(TRACE_TAG_ATTN_STD)
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size       // query / attention-output width (== dim except Gemma2/gemma4)
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    var kcl = kv_layer_kbase(s, l, c.seq_len)   // layer-base byte pointers (aliases the source layer when shared)
    var vcl = kv_layer_vbase(s, l, c.seq_len)
    // sliding-window vs global attention per the arch's layer pattern; no-op for short contexts (pos < window)
    let is_sliding = layer_is_sliding(c, l)
    // gemma-4 E-series shared-KV layer: no attn_k/attn_v of its own — project Q only, attend against the SOURCE layer's cache
    let kv_shared = t.kv_src[l] != l
    let kstart = (is_sliding && pos + 1l > c.sliding_window) ? (pos + 1l - c.sliding_window) : 0l
    let cnt = pos + 1l - kstart
    // attention rmsnorm
    let ts_norm0 = prof_ticks()
    rms_at(s.xb, t, t.rms_att_off + l * dim, s.x, dim)
    prof_add("norm", ts_norm0)
    // q, k, v projections (independent — share s.xb; fused into one parallel dispatch when Q8)
    let ts_qkv = prof_ticks()
    if (kv_shared) {
        mm_f(s.q, t, fmt_at(t.wq_fmt, l), t.wq_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, qd)   // Q only
    } else {
        mm_qkv(s, t, l)
    }
    if (c.attn_qkv_bias) {  // Qwen2: + bias before RoPE
        add_bias(s.q, 0l, t.bq, l * qd, qd)
        add_bias(s.k, 0l, t.bk, l * kv_dim, kv_dim)
        add_bias(s.v, 0l, t.bv, l * kv_dim, kv_dim)
    }
    prof_add("mm_qkv", ts_qkv)
    if (c.qk_norm) {  // Qwen3/gemma4: per-head RMSNorm on q/k before RoPE — q/k are n_heads/n_kv_heads rows of head_size
        let ts_qknorm = prof_ticks()
        rms_batch(s.q, s.q, t, t.rms_q_offs[l], head_size, n_heads)
        if (!kv_shared) {
            rms_batch(s.k, s.k, t, t.rms_k_offs[l], head_size, n_kv_heads)
        }
        prof_add("norm", ts_qknorm)
    }
    // gemma4: weightless per-head RMSNorm on V; V-from-K layers already normed V inside mm_qkv
    if (c.v_norm && t.wv_offs[l] >= 0l) {
        let ts_vnorm = prof_ticks()
        rms_batch(s.v, s.v, t, t.rms_ones_off, head_size, n_kv_heads)
        prof_add("norm", ts_vnorm)
    }
    // table path: leaves called DIRECTLY, same as rope_batch_tab, to dodge pass-through-const-write-elision
    let ts_rope = prof_ticks()
    // partial rotary (glm4moe: 64 of 128) — table rows span rot/2 pairs; loader guarantees NEOX
    let rot = c.rope_dim > 0l ? c.rope_dim : head_size
    if (g_use_rope_table) {
        let swa_row = is_sliding && has_dual_rope(c)
        unsafe {
            let crow = swa_row ? addr<float const?>(s.rope_cos_swa[0]) : addr<float const?>(s.rope_cos[0])
            let srow = swa_row ? addr<float const?>(s.rope_sin_swa[0]) : addr<float const?>(s.rope_sin[0])
            if (rot != head_size) {
                rope_scaled_neox_tab_part(addr(s.q[0]), head_size, rot, qd, crow, srow)
                if (!kv_shared) { rope_scaled_neox_tab_part(addr(s.k[0]), head_size, rot, kv_dim, crow, srow) }
            } elif (c.rope_neox) {
                rope_scaled_neox_tab(addr(s.q[0]), head_size, qd, crow, srow)
                if (!kv_shared) { rope_scaled_neox_tab(addr(s.k[0]), head_size, kv_dim, crow, srow) }
            } else {
                rope_scaled_tab(addr(s.q[0]), head_size, qd, crow, srow)
                if (!kv_shared) { rope_scaled_tab(addr(s.k[0]), head_size, kv_dim, crow, srow) }
            }
        }
    } else {
        let theta = is_sliding ? c.rope_freq_base_swa : c.rope_freq_base
        let fscale = is_sliding ? c.rope_freq_scale_swa : c.rope_freq_scale
        let use_ff = !(c.rope_ff_global_only && is_sliding)
        if (rot != head_size) {
            unsafe {
                let has_ff = use_ff && !empty(t.rope_freqs)   // the YaRN divisors over the rotated span, as the table path folds them
                let ffp = has_ff ? addr < float const? >(t.rope_freqs[0]) : null
                rope_scaled_neox_part(addr(s.q[0]), pos, head_size, rot, qd, theta, fscale, has_ff, ffp, c.rope_mscale)
                if (!kv_shared) {
                    rope_scaled_neox_part(addr(s.k[0]), pos, head_size, rot, kv_dim, theta, fscale, has_ff, ffp, c.rope_mscale)
                }
            }
        } else {
            rope_apply(s.q, pos, head_size, qd, theta, fscale, t.rope_freqs, c.rope_neox, c.rope_mscale, use_ff)
            if (!kv_shared) {
                rope_apply(s.k, pos, head_size, kv_dim, theta, fscale, t.rope_freqs, c.rope_neox, c.rope_mscale, use_ff)
            }
        }
    }
    prof_add("rope", ts_rope)
    let ktq4 = s.kv_dtype_k == KVDtype.tq4
    let vtq4 = s.kv_dtype_v == KVDtype.tq4
    if (ktq4 || vtq4) {
        // tq4 basis change: q/k/v rotate per head, attention runs in that basis, xb un-rotates after (⟨Hq,Hk⟩ = ⟨q,k⟩)
        let ts_rot = prof_ticks()
        unsafe {
            let sgp = addr < float const? >(s.kv_signs[0])
            if (ktq4) {
                var qrp = addr(s.q[0])
                for (hh in range64(qd / head_size)) {
                    fwht_signs_row(qrp + hh * head_size, sgp, head_size)
                }
                if (!kv_shared) {
                    var krp = addr(s.k[0])
                    for (hh in range64(kv_dim / head_size)) {
                        fwht_signs_row(krp + hh * head_size, sgp, head_size)
                    }
                }
            }
            if (vtq4 && !kv_shared) {
                var vrp = addr(s.v[0])
                for (hh in range64(kv_dim / head_size)) {
                    fwht_signs_row(vrp + hh * head_size, sgp, head_size)
                }
            }
        }
        prof_add("tq4_rot", ts_rot)
    }
    if (s.kv_dtype_k == KVDtype.q8_0 || ktq4) {
        // quantize the roped query ONCE for the int8x int8/int4 score dot — O(qd), dwarfed by the reads it feeds
        quantize_q8_0_into(s.q, qd, s.attq, s.atts, 0l, 0l)
    }
    // store k, v for this position (shared-KV layers: nothing to store — source layer already cached it)
    if (!kv_shared) {
        let ts_kv = prof_ticks()
        unsafe {
            kv_store_row(kcl, s.kv_dtype_k, kv_dim, kv_phys_row(s, pos), 0l, kv_dim,
                addr < float const? >(s.k[0]))
            kv_store_row(vcl, s.kv_dtype_v, kv_dim, kv_phys_row(s, pos), 0l, kv_dim,
                addr < float const? >(s.v[0]))
        }
        prof_add("kv_store", ts_kv)
    }
    // attention threaded over heads: disjoint att rows per head, result-identical to the inline loop
    let ts_attn = prof_ticks()
    let seq_len = c.seq_len
    let attn_softcap = c.attn_logit_softcap
    // work-proportional lanes; tiny-cnt early-decode attention (~2K MACs at cnt=1) inlines instead
    let attn_lanes = min(int(n_heads), lanes_for_work(2l * cnt * head_size * n_heads, g_gemv_lane_cap))
    let sinksp = (c.attn_sinks
        ? unsafe(addr < float const? >(t.fblob[t.sinks_off + l * n_heads]))
        : null)
    let kdt = s.kv_dtype_k
    let vdt = s.kv_dtype_v
    let nrun = kv_runs(s, kstart, pos + 1l, s.kv_runs)   // physical runs of this layer's window (flat: 1)
    unsafe {
        let kplanes = kdt == KVDtype.q8_0 || kdt == KVDtype.tq4
        let qp = addr < float const? >(s.q[0])
        let qqp = kplanes ? addr < int8 const? >(s.attq[0]) : null
        let qsp = kplanes ? addr < float const? >(s.atts[0]) : null
        let runsp = addr < KVRun const? >(s.kv_runs[0])
        var xbp = addr(s.xb[0])
        var attp = addr(s.att[0])
        maybe_parallel_for(0, int(n_heads), attn_lanes) $(rb, re) {
            unsafe {
                for (h in range(rb, re)) {
                    let hh = int64(h)
                    let qoff = hh * head_size
                    let kvhoff = (hh / kv_mul) * head_size  // GQA: which kv head this query head reads
                    attn_head_decode_d(attp + hh * seq_len, xbp, qp, qqp, qsp, kcl, vcl, kdt, vdt, runsp,
                        nrun, qoff, kvhoff, kstart, cnt, kv_dim, head_size, hh, scale, attn_softcap, sinksp)
                }
            }
        }
    }
    prof_add("attn", ts_attn)
    if (vtq4) {   // tq4 V: xb accumulated in the rotated basis — un-rotate each head's slice
        unsafe {
            let sgp = addr < float const? >(s.kv_signs[0])
            var xrp = addr(s.xb[0])
            for (hh in range64(n_heads)) {
                fwht_unsign_row(xrp + hh * head_size, sgp, head_size)
            }
        }
    }
    // output projection (qd -> dim), optional bias + post-attention norm, residual
    let ts_wo = prof_ticks()
    mm_f(s.xb2, t, fmt_at(t.wo_fmt, l), t.wo_offs[l], s.xb, s.xq, s.xs, s.xbs, qd, dim)
    if (c.attn_out_bias) {  // gpt-oss: + bias on the projected attention output
        add_bias(s.xb2, 0l, t.bo, l * dim, dim)
    }
    prof_add("mm_wo", ts_wo)
    let ts_resid0 = prof_ticks()
    if (c.pre_post_norm) {  // Gemma2: post-attention RMSNorm before the residual add
        rms_at(s.xb2, t, t.rms_post_att_off + l * dim, s.xb2, dim)
    }
    add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb2[0])), dim)
    prof_add("act_resid", ts_resid0)
}

def attention_gpu_prefill(t : Model; var s : Session; l : int64; npos : int64; gated : bool) {
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let qd = n_heads * head_size
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let rot = c.rope_dim > 0l ? c.rope_dim : head_size
    let fq = fmt_at(t.wq_fmt, l)
    let kq_cls = fq != KqFmt.q8
    let ts_norm0 = prof_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_att_off + l * dim, dim, npos)
    if (kq_cls) {   // the chain's activation image follows the q/k/v stack class
        s.kxqb |> resize(npos * dim)
        s.kxsb |> resize(npos * dim / 256l)
        s.kxbsb |> resize(npos * dim / 16l)
        requant_rows_q8k_bs(s.xb_b, dim, npos, s.kxqb, s.kxsb, s.kxbsb)
    } else {
        requant_rows_q8(s.xb_b, dim, npos, s.xqb, s.xsb)
    }
    if (!g_use_rope_table) {   // the GPU rope consumes table rows — build them (bit-identical values)
        build_rope_table(s.rope_cos, s.rope_sin, t, c.rope_freq_base, c.rope_freq_scale, 0l, npos, rot, true)
    }
    prof_add("at_norm", ts_norm0)
    let ts_gpu = prof_ticks()
    unsafe {
        let rq = c.qk_norm ? addr<float const?>(t.fblob[t.rms_q_offs[l]]) : null
        let rk = c.qk_norm ? addr<float const?>(t.fblob[t.rms_k_offs[l]]) : null
        let bq = c.attn_qkv_bias ? addr<float const?>(t.bq[l * qd]) : null
        let bk = c.attn_qkv_bias ? addr<float const?>(t.bk[l * kv_dim]) : null
        let bv = c.attn_qkv_bias ? addr<float const?>(t.bv[l * kv_dim]) : null
        if (kq_cls) {
            matmul_moe_gpu_attn(s.xb2_b, s.k_b, s.v_b,
                t.wq_offs[l], t.wk_offs[l], t.wv_offs[l], t.wo_offs[l],
                int(fq), int(fmt_at(t.wk_fmt, l)), int(fmt_at(t.wv_fmt, l)), int(fmt_at(t.wo_fmt, l)),
                s.kxqb, s.kxsb, rq, rk, bq, bk, bv,
                addr<float const?>(s.rope_cos[0]), addr<float const?>(s.rope_sin[0]),
                npos, dim, qd, kv_dim, head_size, rot, n_heads,
                scale, c.norm_eps, gated, c.qk_norm)
        } else {
            matmul_moe_gpu_attn(s.xb2_b, s.k_b, s.v_b,
                t.wq_offs[l], t.wk_offs[l], t.wv_offs[l], t.wo_offs[l],
                int(fq), int(fmt_at(t.wk_fmt, l)), int(fmt_at(t.wv_fmt, l)), int(fmt_at(t.wo_fmt, l)),
                s.xqb, s.xsb, rq, rk, bq, bk, bv,
                addr<float const?>(s.rope_cos[0]), addr<float const?>(s.rope_sin[0]),
                npos, dim, qd, kv_dim, head_size, rot, n_heads,
                scale, c.norm_eps, gated, c.qk_norm)
        }
    }
    prof_add("at_gpu", ts_gpu)
    // the cache store runs the normal CPU codec off the DMA'd k/v rows
    let ts_kv = prof_ticks()
    tq4_rotate_for_store(s, s.k_b, s.v_b, npos, kv_dim, head_size)
    var kcl = kv_layer_kbase(s, l, c.seq_len)
    var vcl = kv_layer_vbase(s, l, c.seq_len)
    let store_nrun = kv_runs(s, 0l, npos, s.kv_runs)
    kv_store_batch(kcl, vcl, s.kv_dtype_k, s.kv_dtype_v, s.k_b, s.v_b,
        unsafe(addr < KVRun const? >(s.kv_runs[0])), store_nrun, 0l, npos, kv_dim)
    prof_add("kv_store", ts_kv)
    let ts_resid = prof_ticks()
    add_batch(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
    prof_add("at_resid", ts_resid)
}

// npos >= 32 (a batch worth dispatching), start of sequence only (the chain reads the batch's own
// K/V device-side). A non-causal image span always
// declines: the GPU chains encode the causal mask — the guard survives lifting start_pos == 0.
def attn_gpu_prefill_ready(s : Session; l : int64; npos : int64; start_pos : int64) : bool {
    return (npos >= 32l && npos <= 4096l && start_pos == 0l && s.attn_uniform_end == 0l
        && moe_gpu_attn_on_gpu(l))
}

// attn-norm → batched QKV(+bias) → RoPE → KV-store → prefill attention → out-proj(+post-norm) → residual.
[no_alloc, no_env, no_io]
def attention_std_prefill(t : Model; var s : Session; l : int64; npos : int64; start_pos : int64) {
    if (attn_gpu_prefill_ready(s, l, npos, start_pos)) {
        attention_gpu_prefill(t, s, l, npos, t.config.q_gated)
        return
    }
    let c = t.config
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    var kcl = kv_layer_kbase(s, l, c.seq_len)   // layer-base byte pointers (aliases the source layer when shared)
    var vcl = kv_layer_vbase(s, l, c.seq_len)
    let is_sliding = layer_is_sliding(c, l)
    let kv_shared = t.kv_src[l] != l
    attn_qkv_rope_prefix(t, s, l, npos, start_pos)
    let ktq4 = s.kv_dtype_k == KVDtype.tq4
    let vtq4 = s.kv_dtype_v == KVDtype.tq4
    if (ktq4 || vtq4) {
        // tq4 basis change at the RoPE seam (see attention_std_decode); xb_b un-rotates below
        let ts_rot = prof_ticks()
        tq4_rotate_qkv(s.q_b, s.k_b, s.v_b, npos, qd, kv_dim, head_size, unsafe(addr < float const? >(s.kv_signs[0])), ktq4, vtq4, kv_shared)
        prof_add("tq4_rot", ts_rot)
    }
    // separate pass (provably identical to interleaving); shared-KV layers store nothing — already cached
    if (!kv_shared) {
        let ts_kv = prof_ticks()
        let store_nrun = kv_runs(s, start_pos, start_pos + npos, s.kv_runs)   // where the new rows land
        kv_store_batch(kcl, vcl, s.kv_dtype_k, s.kv_dtype_v, s.k_b, s.v_b,
            unsafe(addr < KVRun const? >(s.kv_runs[0])), store_nrun, start_pos, npos, kv_dim)
        prof_add("kv_store", ts_kv)
    }
    // causal attention into xb_b; output for position p lands at xb_b[p*qd..], feeding wo below
    let ts_attn = prof_ticks()
    let sinksp = (c.attn_sinks
        ? unsafe(addr < float const? >(t.fblob[t.sinks_off + l * n_heads]))
        : null)
    prefill_attention(s, kcl, vcl, npos, start_pos, head_size, qd, kv_dim, n_heads, kv_mul, scale,
                      is_sliding, c.sliding_window, c.attn_logit_softcap, sinksp)
    prof_add("attn", ts_attn)
    if (vtq4) {   // tq4 V: xb_b rows accumulated in the rotated basis — un-rotate per (pos, head)
        tq4_unrotate_batch(s.xb_b, npos, qd, head_size, unsafe(addr < float const? >(s.kv_signs[0])))
    }
    attn_out_suffix(t, s, l, npos)
}

// Batched causal conv over the whole prefill window + per-row epilogue (SiLU, q/k L2-norm, q
// pre-scale): row p depends only on input rows p-taps..p, so rows parallelize freely. Weights
// transpose tap-major once per layer for vector fmas instead of a per-channel scalar tap walk.
def ffn_dense_prefill(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let dim = c.dim
    let hidden = layer_hidden(t, l)   // gemma-4 E2B varies FFN width per layer
    // ffn rmsnorm, per position
    let ts_norm1 = prof_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_ffn_off + l * dim, dim, npos)
    prof_add("norm", ts_norm1)
    // gated FFN (batched): hb = act(W1·xb) * (W3·xb) — act/mul elementwise, so batch-flat
    let ts_ffn = prof_ticks()
    mm_b_f(s, s.hb_b, t, fmt_at(t.w1_fmt, l), t.w1_offs[l], s.xb_b, dim, hidden, npos)
    mm_b_f(s, s.hb2_b, t, fmt_at(t.w3_fmt, l), t.w3_offs[l], s.xb_b, dim, hidden, npos)
    prof_add("mm_ffn", ts_ffn)
    let ts_act = prof_ticks()
    gate_batch(s.hb_b, s.hb2_b, c.ffn_act, hidden, npos)   // fused act*gate (exp4 unless act_vec off)
    prof_add("act", ts_act)
    // down projection (batched) + optional post-FFN norm + residual
    let ts_ffn2 = prof_ticks()
    mm_b_f(s, s.xb_b, t, fmt_at(t.w2_fmt, l), t.w2_offs[l], s.hb_b, hidden, dim, npos)
    prof_add("mm_ffn_down", ts_ffn2)   // own bucket: down is long-K + often a different format (Q4_K_M keeps it q6_K)
    let ts_resid1 = prof_ticks()
    if (c.pre_post_norm) {  // Gemma2: post-FFN RMSNorm per position before residual
        rms_batch(s.xb_b, s.xb_b, t, t.rms_post_ffn_off + l * dim, dim, npos)
    }
    add_batch(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb_b[0])), npos * dim)
    // E-series applies out_scale AFTER its PLE block (in ffn_gemma4_prefill) — skip it here when PLE is on
    if (c.layer_out_scale && !has_ple(c)) {  // gemma4: scale the whole layer output (after the residual)
        let os = t.fblob[t.out_scale_off + l]
        scale_inplace(unsafe(addr(s.x_b[0])), os, npos * dim)
    }
    prof_add("act_resid", ts_resid1)
}

// Raw byte move as int64 lanes — the Q8 gather's copy. Integer loads/stores are bit-exact by
// construction with no FP semantics (a float-lane copy of quant bytes could in principle quiet
// sNaN bit patterns on some targets). n8 = byte count / 8.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def private copy_int64s(var d : int64?; s : int64 const?; n8 : int64) {
    unsafe {
        for (j in range64(n8)) {
            d[j] = s[j]
        }
    }
}

// Gather one expert bucket's Q8-quantized activation rows (quants + scales) out of the whole-batch
// image: gather row r comes from position moe_bkt[b0+r]/k. The int8 quant rows move as int64 lanes
// (dim % 32 == 0 so dim % 8 == 0); the scales are genuine floats and ride the tuned copy_floats.
def moe_gather_rows(var s : Session; b0, cnt, dim, k : int64; with_bs : bool = false; kform : bool = false) {
    let nb = kform ? dim / 256l : dim / 32l   // kq v2: Q8_K rows carry per-256 scales
    let d8 = dim / 8l
    unsafe {
        var xqp : int64 const?
        var xsp : float const?
        if (kform) {
            xqp = addr < int64 const? >(s.kxqb[0])
            xsp = addr < float const? >(s.kxsb[0])
        } else {
            xqp = addr < int64 const? >(s.xqb[0])
            xsp = addr < float const? >(s.xsb[0])
        }
        var gqp = addr < int64? >(s.moe_gxq[0])
        var gsp = addr(s.moe_gxs[0])
        for (r in range64(cnt)) {
            let p = s.moe_bkt[b0 + r] / k
            copy_int64s(gqp + r * d8, xqp + p * d8, d8)
            copy_floats(gsp + r * nb, xsp + p * nb, nb)
        }
        if (with_bs) {   // kq-expert layers: the per-16 sum rows ride along (dim/16 ints = dim/32 int64s)
            let nb8 = dim / 32l
            var bp : int64 const?
            if (kform) {
                bp = addr < int64 const? >(s.kxbsb[0])
            } else {
                bp = addr < int64 const? >(s.xbsb[0])
            }
            var gbp = addr < int64? >(s.moe_gxbs[0])
            for (r in range64(cnt)) {
                let p = s.moe_bkt[b0 + r] / k
                copy_int64s(gbp + r * nb8, bp + p * nb8, nb8)
            }
        }
    }
}

// Expand one native-MXFP4 expert region [d x n] into EXACT Q8 blocks: q[j] = doubled-e2m1 LUT
// value, scale = e8m0_half(e) — no requantization error. Interleaved-mr backends write the
// repacked form directly (byte-for-byte map); non-interleaved backends expand row-major then repack.
def private expand_mx4_region_q8(t : Model; woff, n, d : int64; @scratch var wq : array<int8>; @scratch var ws : array<float>) { // nolint:STYLE038 — one mx4 region expansion: bounds, unpack, requant; the phases share the scratch buffer
    let total = n * d
    wq |> resize(total)
    ws |> resize(total / 32l)
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    let nblk = total / 32l
    unsafe {
        let lutp = addr(lut[0])   // frame outlives the join, so workers read a live pointer
        let np = addr(t.mxq[woff / 2l])
        let ep = addr(t.mxs[woff / 32l])
        var qp = addr(wq[0])
        var sp = addr(ws[0])
        // scale bytes: 1:1 positional map in both the row-major and the interleaved layout
        maybe_parallel_for(total >= g_requant_par_threshold, 0, int(nblk), get_dispatch_lanes()) $(sb, se) {
            unsafe {
                for (bi in range(sb, se)) {
                    sp[bi] = e8m0_half(uint(ep[bi]))
                }
            }
        }
        if (g_active_mx4_expand_interleaved) {
            // interleaved: mr-row block-groups first, then row-major tail; bias128 stamps need a BIASED LUT copy
            let mr = active_q8_layout_mr()
            let wbias = int(active_q8_wbias())
            let kgv = active_q8_kgroup()
            var blut : int8[16]
            for (i in range(16)) {
                blut[i] = int8(int(lut[i]) + wbias)
            }
            let nbyt = 16l * mr    // nibble bytes per block-group
            let qbyt = 32l * mr    // q8 bytes per block-group
            let ngrp = (d / mr) * (n / 32l)
            let blutp = addr(blut[0])   // frame outlives the join, same as lutp
            if (kgv == 8l) {
                maybe_parallel_for(total >= g_requant_par_threshold, 0, int(ngrp), get_dispatch_lanes()) $(gb, ge) {
                    unsafe {
                        for (bg0 in range(gb, ge)) {
                            let bg = int64(bg0)
                            let nb = bg * nbyt
                            let qb = bg * qbyt
                            for (off in range64(nbyt)) {
                                let j = off / (4l * mr)
                                let rem = off % (4l * mr)
                                let q8o = (j / 2l) * 8l * mr + (rem / 4l) * 8l + (j % 2l) * 4l + rem % 4l
                                let b = int(np[nb + off])
                                qp[qb + q8o] = blutp[b & 15]
                                qp[qb + nbyt + q8o] = blutp[b >> 4]
                            }
                        }
                    }
                }
            } else {
                maybe_parallel_for(total >= g_requant_par_threshold, 0, int(ngrp), get_dispatch_lanes()) $(gb, ge) {
                    unsafe {
                        for (bg0 in range(gb, ge)) {
                            let bg = int64(bg0)
                            let nb = bg * nbyt
                            let qb = bg * qbyt
                            for (off in range64(nbyt)) {
                                let b = int(np[nb + off])
                                qp[qb + off] = blutp[b & 15]
                                qp[qb + nbyt + off] = blutp[b >> 4]
                            }
                        }
                    }
                }
            }
            for (bi in range64(ngrp * mr, nblk)) {   // row tail (d % mr): row-major from here on
                let nb16 = bi * 16l
                let qb = bi * 32l
                for (j in range64(16l)) {
                    let b = int(np[nb16 + j])
                    qp[qb + j] = lutp[b & 15]
                    qp[qb + j + 16l] = lutp[b >> 4]
                }
            }
        } else {
            maybe_parallel_for(total >= g_requant_par_threshold, 0, int(nblk), get_dispatch_lanes()) $(bb, be) {
                unsafe {
                    for (bi0 in range(bb, be)) {
                        let bi = int64(bi0)
                        let nb16 = bi * 16l
                        let qb = bi * 32l
                        for (j in range64(16l)) {
                            let b = int(np[nb16 + j])
                            qp[qb + j] = lutp[b & 15]
                            qp[qb + j + 16l] = lutp[b >> 4]
                        }
                    }
                }
            }
        }
    }
    if (g_active_needs_repack && !g_active_mx4_expand_interleaved) {
        // row-major-expanded scratch on a repack backend: interleave through the backend's own Q8 repack
        repack_q8q8_weight(wq, ws, 0l, n, d)
    }
}

// MoE FFN prefill, grouped: route every position, bucket by expert, run ONE batched GEMM chain per
// touched expert instead of npos*k single-token matmuls. Bit-exact vs the per-position reference
// path (native-MXFP4 experts are the one exception — differ by kernel summation order only).

var private g_gpu_combine = true
var private g_gpu_combine_probed = false

[cold_path]   // one-time env resolution; the accessor is a global test after the first call
def private load_gpu_combine() {
    g_gpu_combine_probed = true
    g_gpu_combine = g_env_gpu.gpu_combine
}

def gpu_combine_on : bool {
    if (!g_gpu_combine_probed) {
        load_gpu_combine()
    }
    return g_gpu_combine
}

let private SPLIT_BUS_BPS = 13.5e9
let private SPLIT_GPU_ROW_S = 1.2e-6
let private SPLIT_CPU_ROW_S = 10.0e-6

//! bytes per weight of a repacked plane: the superblock quant payload + its scale row, off the
//! stride table; q8 is 32 quants + an f16 scale per block
def kq_bytes_per_weight(f : KqFmt) : float => kq_sb(f) ? float(kq_qsb(f) + kq_ssb(f)) / float(kq_elems(f)) : 34.0 / 32.0

//! Runtime routing gate over the device-side routed combine (overrides the
//! ``DASLLAMA_GPU_COMBINE`` env probe) — the in-process A/B lever: both arms share one
//! process, one residency, one paging weather, so the measured delta is the combine alone.
def set_gpu_combine_route(on : bool) {
    g_gpu_combine_probed = true
    g_gpu_combine = on
}

var private g_gpu_prefill_route = true

//! Runtime routing gate over the GPU MoE PREFILL arm (`fused_gpu`) in both grouped prefills —
//! the generic one and gemma-4's twin. Routing-only: uploads and residency stay put, so an
//! in-process A/B isolates device-side prefill against the same resident set (decode is untouched).
def set_moe_gpu_prefill_route(on : bool) {
    g_gpu_prefill_route = on
}

def gpu_prefill_route_on() : bool => g_gpu_prefill_route

var private g_shexp_gpu_layers = 0l   //! layers whose shared expert the device chain served in a prefill - the arm's engage witness (read on the main context only)
def public shexp_gpu_prefill_layers() : int64 => g_shexp_gpu_layers

//! why this prefill's shared expert stays on the CPU rail: the model's shape, the tier's marks, or the call's size and route
def private shexp_gpu_decline_reason(c : Config; l, npos : int64) : string {
    if (c.moe_dense_shexp) return "the model's shared expert is a dense FFN, not the device chain's call shape"
    if (!moe_gpu_shexp_on_gpu(l)) return "the shared expert's planes are not resident on the tier"
    if (npos < 32l) return "a prefill under 32 positions"
    return "the prefill route lever is off"
}

//! the (position, slot) pairs bucketed by expert - a CSR built in place: moe_cnt[e] ends as expert e's run END, moe_bkt the pairs in expert order
def private moe_prefill_bucket(var s : Session; nk, ne : int64) {
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
}

[arch(at="../ARCHITECTURE_RUNTIME_JOBQUE.md#moe-region-split")]
def private moe_prefill_regions_lpt(t : Model; var s : Session; l, nk, split_cap : int64; skip_gpu_experts : bool = false) : int64 {
    let c = t.config
    let dim = c.dim
    let ne = c.n_expert
    let nfe = c.n_ff_exp
    let cap_reg = split_cap > 0l ? ne + nk / split_cap + 1l : ne
    s.moe_offs1 |> clear(); s.moe_offs1 |> reserve(cap_reg * 3l)
    s.moe_offs3 |> clear(); s.moe_offs3 |> reserve(cap_reg * 3l)
    s.moe_offs2 |> clear(); s.moe_offs2 |> reserve(cap_reg * 3l)
    if (c.moe_exps_bias) {
        s.moe_boffs1 |> clear(); s.moe_boffs1 |> reserve(cap_reg)
        s.moe_boffs3 |> clear(); s.moe_boffs3 |> reserve(cap_reg)
        s.moe_boffs2 |> clear(); s.moe_boffs2 |> reserve(cap_reg)
    }
    s.moe_ord |> clear()
    s.moe_ord |> reserve(ne)   // capacity persists across layers — no per-layer alloc
    for (e in range64(ne)) {
        let b0 = e == 0l ? 0l : s.moe_cnt[e - 1l]
        let cnt = s.moe_cnt[e] - b0
        if (cnt > 0l && !(skip_gpu_experts && s.moe_exp_gpu[e] != int8(0))) {   // a split's device experts take no host region
            s.moe_ord |> push(int2(int(e), int(cnt)))
        }
    }
    lpt_order(s.moe_ord)
    var nreg = 0l
    for (oe in s.moe_ord) {
        let e = int64(oe.x)
        let cnt = int64(oe.y)
        let b0 = e == 0l ? 0l : s.moe_cnt[e - 1l]
        let eg = l * ne + e
        let base = e * dim * nfe
        var sub0 = 0l
        while (sub0 < cnt) {   //! sub-regions share the expert's weights; the bias lists repeat per sub-region
            let scnt = split_cap > 0l ? min(cnt - sub0, split_cap) : cnt
            s.moe_offs1 |> push(t.we1_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
            s.moe_offs1 |> push(b0 + sub0)
            s.moe_offs1 |> push(scnt)
            s.moe_offs3 |> push(t.we3_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
            s.moe_offs3 |> push(b0 + sub0)
            s.moe_offs3 |> push(scnt)
            s.moe_offs2 |> push(t.we2_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
            s.moe_offs2 |> push(b0 + sub0)
            s.moe_offs2 |> push(scnt)
            if (c.moe_exps_bias) {
                s.moe_boffs1 |> push(t.web1_off + eg * nfe)
                s.moe_boffs3 |> push(t.web3_off + eg * nfe)
                s.moe_boffs2 |> push(t.web2_off + eg * dim)
            }
            nreg++
            sub0 += scnt
        }
    }
    return nreg
}

//! the raw down rows land in s.moe_gout in bucket order; the down biases add before the routed weight scales a row
def private moe_prefill_fused_cpu(t : Model; var s : Session; l, nreg, nk, ts_g1 : int64) {
    let c = t.config
    let dim = c.dim
    let nfe = c.n_ff_exp
    let f1 = fmt_at(t.we1_fmt, l)
    let f2 = fmt_at(t.we2_fmt, l)
    let f3 = fmt_at(t.we3_fmt, l)
    mm_b_f_groupn(s.moe_ghb, t, f1, s.moe_offs1, s.moe_boffs1, c.moe_exps_bias, nreg, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe)
    mm_b_f_groupn(s.moe_ghb2, t, f3, s.moe_offs3, s.moe_boffs3, c.moe_exps_bias, nreg, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe)
    prof_add("mm_gemm", ts_g1)
    let ts_act = prof_ticks()
    gate_batch(s.moe_ghb, s.moe_ghb2, c.ffn_act, nfe, nk)
    prof_add("act", ts_act)
    requant_rows_f(f2, s.moe_ghb, nfe, nk, s.moe_hq, s.moe_hs, s.moe_hbs)
    let ts_g3 = prof_ticks()
    mm_b_f_groupn(s.moe_gout, t, f2, s.moe_offs2, s.moe_boffs2, c.moe_exps_bias, nreg, s.moe_hq, s.moe_hs, s.moe_hbs, nfe, dim)
    prof_add("mm_gemm", ts_g3)
}

def private moe_prefill_park(var s : Session; nk, dim : int64) {
    trace_tag(TRACE_TAG_PARK)
    let ts_park = prof_ticks()
    unsafe {
        let gp = addr < float const? >(s.moe_gout[0])
        var ep = addr(s.moe_eout[0])
        let bp = addr < int64 const? >(s.moe_bkt[0])
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

//! one mx4 region at `off` (`n` in, `d` out) expanded to Q8 into the scratch `wq` / `ws`, then its GEMM over `cnt` Q8_0 rows into `y`
def private moe_expert_mx4_mm(t : Model; var y : array<float>; @scratch var wq : array<int8>; @scratch var ws : array<float>;
                              xq : array<int8>; xs : array<float>; off, n, d, cnt : int64) {
    let ts_ex = prof_ticks()
    expand_mx4_region_q8(t, off, n, d, wq, ws)
    prof_add("moe_expand", ts_ex)
    let ts_g = prof_ticks()
    matmul_q8q8_batch(y, wq, ws, xq, xs, n, d, cnt)
    prof_add("mm_gemm", ts_g)
}

//! one expert's gate and up GEMMs over its `cnt` gathered rows (`moe_gather_rows`'s image) into the hidden panels: the stacks'
//! batch tiles in their own forms, or on an mx4 stack with no native mx4 batch kernel each region expanded to Q8 first
def moe_expert_gate_up(t : Model; var s : Session; f1, f3 : KqFmt; off1, off3, dim, nfe, cnt : int64) {
    if (!t.experts_mx4 || g_active_mx4_native_batch) {
        mm_b_f_pre(s.moe_ghb, t, f1, off1, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe, cnt)
        mm_b_f_pre(s.moe_ghb2, t, f3, off3, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe, cnt)
        return
    }
    moe_expert_mx4_mm(t, s.moe_ghb, s.mx4_wq, s.mx4_ws, s.moe_gxq, s.moe_gxs, off1, dim, nfe, cnt)
    moe_expert_mx4_mm(t, s.moe_ghb2, s.mx4_wq, s.mx4_ws, s.moe_gxq, s.moe_gxs, off3, dim, nfe, cnt)
}

//! ... and its down GEMM over the requantized hidden rows into the parked down rows, the same two forms
def moe_expert_down(t : Model; var s : Session; f2 : KqFmt; off2, dim, nfe, cnt : int64) {
    if (!t.experts_mx4 || g_active_mx4_native_batch) {
        mm_b_f_pre(s.moe_gout, t, f2, off2, s.moe_hq, s.moe_hs, s.moe_hbs, nfe, dim, cnt)
        return
    }
    moe_expert_mx4_mm(t, s.moe_gout, s.mx4_wq, s.mx4_ws, s.moe_hq, s.moe_hs, off2, nfe, dim, cnt)
}

//! one batched GEMM chain per touched expert (the unfused arm); the raw down rows park for the ordered reduce; under `split`
//! the experts marked in s.moe_exp_gpu are the device's and take no chain here
def private moe_prefill_per_expert(t : Model; var s : Session; l : int64; moe_kform : bool; skip_gpu_experts : bool = false) {
    for (e in range64(t.config.n_expert)) {
        let cnt = s.moe_cnt[e] - (e == 0l ? 0l : s.moe_cnt[e - 1l])
        if (cnt == 0l || (skip_gpu_experts && s.moe_exp_gpu[e] != int8(0))) {
            continue
        }
        moe_prefill_expert_chain(t, s, l, e, moe_kform)
    }
}

var private g_rare_arena_pin = -1     //! `set_rare_arena`'s pin: -1 reads the environment
var private g_rare_onepass_pin = -1   //! `set_rare_onepass`'s pin: -1 reads the environment
var private g_rare_batched_pin = -1   //! `set_rare_batched`'s pin: -1 reads the environment

//! the rare rail's arena arm (`DASLLAMA_RARE_ARENA`): the host share reads streamed experts off the tier's pinned device-layout planes through the row-major kernels; the pin is a fixture's in-process twin, -1 lifts it
def rare_arena_on() : bool => g_rare_arena_pin >= 0 ? g_rare_arena_pin == 1 : g_env_engine.rare_arena != 0l

//! whether the arena arm serves model `t`'s host picks and window rows: a pin of 1 or the knob at 2 outright (the formats
//! witness and the row-major kernels' A/B arm), else the arm where the load kept no planar planes - a trimmed lane holds a
//! streamed expert once, in the pinned mirror; where the planar planes are loaded the tuned planar kernels serve
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-arena")]
def rare_arena_serves(t : Model) : bool => g_rare_arena_pin >= 0 ? g_rare_arena_pin == 1 : g_env_engine.rare_arena == 2l || (g_env_engine.rare_arena != 0l && t.planes_trimmed)

//! the one-pass lane (`DASLLAMA_RARE_ONEPASS`): each (expert, row) pair's whole chain on one lane, one fork a batch; off = the three region-list passes
def rare_onepass_on() : bool => g_rare_onepass_pin >= 0 ? g_rare_onepass_pin == 1 : g_env_engine.rare_onepass

//! the batched expert cores on the one-pass lane (`DASLLAMA_RARE_BATCHED`): an expert's rows through the tiles, the plane read once; off = the rows core a row
def rare_batched_on() : bool => g_rare_batched_pin >= 0 ? g_rare_batched_pin == 1 : g_env_engine.rare_batched

def set_rare_onepass(pin : int) {
    g_rare_onepass_pin = pin
}

def rare_onepass_pin : int => g_rare_onepass_pin

def set_rare_batched(pin : int) {
    g_rare_batched_pin = pin
}

def rare_batched_pin : int => g_rare_batched_pin

//! the formats whose device layout a row-major kernel reads (the one-copy rail): every expert format the tier streams
def rm_dev_serves(f : KqFmt) : bool {
    return (f == KqFmt.q8 || f == KqFmt.iq4nl32 || f == KqFmt.q4032 || f == KqFmt.q51 || f == KqFmt.q5032 || f == KqFmt.mx4
        || f == KqFmt.k4 || f == KqFmt.k5 || f == KqFmt.k6 || f == KqFmt.k3 || f == KqFmt.k2 || f == KqFmt.q40
        || f == KqFmt.iq4nl || f == KqFmt.iq4xs || f == KqFmt.iq3s4 || f == KqFmt.nv4
        || f == KqFmt.iq3s || f == KqFmt.iq3xxs || f == KqFmt.iq2s || f == KqFmt.iq2xs || f == KqFmt.iq2xxs)
}

def set_rare_arena(pin : int) {
    g_rare_arena_pin = pin
}

//! the pin as it was written (-1 = none): a fixture puts back what it found
def rare_arena_pin : int => g_rare_arena_pin

//! one plane of the routed triple as the lanes read it: its format, its plane bases, the strides a region's offset resolves
//! through, and the rows core - every pointer taken in the main context
struct private RarePlane {
    f : KqFmt
    sid : int              //! the schema id (a superblock format)
    q : uint8 const?       //! the quant plane (a superblock or per-32 format)
    s : uint8 const?       //! ... and its scale plane
    qsb : int64            //! bytes a block in each
    ssb : int64
    belems : int64         //! elements a block: 256 or 32
    tb : int64             //! the batched core's token block
    panel_bytes : int64    //! the batched core's panel scratch a lane (a superblock format whose tile reads an unpacked panel)
    kqfn : MatmulKqRowsFn  //! the backend's rows core (a superblock format)
    q8q : int8 const?      //! the q8 planes
    q8s : float const?
    q8fn : MatmulQ8Q8RowsFn
    dev_layout : bool      //! the planes are the tier's pinned device-layout stack, read through the format's row-major kernel
    base : int64           //! the stack's first element (the region offsets are relative to it)
    ege : int64            //! an expert's elements, the stack's slots an expert each
    scales : float const?  //! the stack's per-expert scales (`exps_scale_ptr`), null where it carries none
    view : MoeGpuStreamView   //! the cache's layout of the stack, captured in the main context: a lane resolves experts off it alone
}

//! the device-plane pointers of the block at `woff` of a device-served plane: its expert's cache slot off the captured view plus
//! the block's offset inside the expert; an expert the cache does not hold is a panic by name (the window's fetch missed it)
def private rare_dev_ptrs(p : RarePlane; woff : int64) : tuple<q : uint8 const?; s : uint8 const?> {
    let e = (woff - p.base) / p.ege
    unsafe {
        let s = p.view.slot_of[e]
        if (s < 0) {
            panic("dasLLAMA: the rare rail asked for expert {e} of the streamed stack at {p.base} (fmt {int(p.f)}), which the expert cache does not hold")
        }
        let blk = (woff - p.base - e * p.ege) / p.belems
        if (s >= STREAM_RING_BASE) {   //! a ring slot holds the expert's page-widened lane span: its bytes start the lane skew in
            let r = int64(s - STREAM_RING_BASE)
            return (q = p.view.ring_q + r * p.view.ring_qlen + stream_page_span(p.view.qoff, p.view.qlen, e).skew + blk * p.qsb,
                    s = p.view.ring_s + r * p.view.ring_slen + stream_page_span(p.view.soff, p.view.slen, e).skew + blk * p.ssb)
        }
        return (q = p.view.q + int64(s) * p.view.qlen + blk * p.qsb, s = p.view.s + int64(s) * p.view.slen + blk * p.ssb)
    }
}

//! layer `l`'s routed triple in plane order gate, up, down: each plane's format and its stack's offset
def private expert_planes(t : Model; l : int64) : tuple<fmts : KqFmt[3]; offs : int64[3]> =>
    (fmts = fixed_array(expert_fmt(t, t.we1_fmt, l), expert_fmt(t, t.we3_fmt, l), expert_fmt(t, t.we2_fmt, l)), offs = fixed_array(t.we1_offs[l], t.we3_offs[l], t.we2_offs[l]))

//! expert `e`'s three planes of layer `l` in the cache, read off the lane into the ring where absent; false where the layer's stacks are not device-served
def rare_fetch_expert(t : Model; l, e : int64) : bool {
    let ep = expert_planes(t, l)
    for (f, off in ep.fmts, ep.offs) {
        return false if (!moe_gpu_stream_fetch(off, int(f), e))
    }
    return true
}

//! the experts of regions [lo, hi] of s.moe_ord in the cache: every one queued in the list's order, then each waited for
def private rare_fetch_regions(t : Model; s : Session; l, lo, hi : int64) {
    for (r in range64(lo, hi + 1l)) {
        rare_request_expert(t, l, int64(s.moe_ord[r].x))
    }
    for (r in range64(lo, hi + 1l)) {
        rare_fetch_expert(t, l, int64(s.moe_ord[r].x))
    }
}

var @exact_size g_expert_heat : array<int64>   //! the picks a (layer, expert) took since the load - a window's rows, a token's one: the expert cache's profile
var g_expert_heat_layers = 0l      //! the profile's shape
var g_expert_heat_experts = 0l

//! the layers the profile covers: the trunk's and the NextN head's, whose experts route and stream like a trunk layer's
def expert_heat_layer_count(t : Model) : int64 => t.config.n_layers + t.config.n_layer_nextn

//! the profile sized to the model, zero, at the first pick after a load
[cold_path]
def private expert_heat_make(t : Model) {
    g_expert_heat_layers = expert_heat_layer_count(t)
    g_expert_heat_experts = t.config.n_expert
    g_expert_heat |> reserve_resize(g_expert_heat_layers * g_expert_heat_experts)
}

def expert_heat_add(t : Model; l, e, rows : int64) {
    if (empty(g_expert_heat)) {
        expert_heat_make(t)
    }
    g_expert_heat[l * g_expert_heat_experts + e] += rows
}

def private expert_heat_add_regions(t : Model; s : Session; l, lo, hi : int64) {
    for (r in range64(lo, hi + 1l)) {
        expert_heat_add(t, l, int64(s.moe_ord[r].x), int64(s.moe_ord[r].y))
    }
}

//! expert `e`'s three planes of layer `l` queued for the readers where the cache lacks them (the call returns at once)
def private rare_request_expert(t : Model; l, e : int64) {
    let ep = expert_planes(t, l)
    for (f, off in ep.fmts, ep.offs) {
        moe_gpu_stream_request(off, int(f), e)
    }
}

def private rare_expert_ready(t : Model; l, e : int64) : bool {
    let ep = expert_planes(t, l)
    for (f, off in ep.fmts, ep.offs) {
        return false if (!moe_gpu_stream_ready(off, int(f), e))
    }
    return true
}

var private @scratch g_rot_ord : array<int2>
var private @scratch g_rot_offs : array<int64>

//! regions [a, h] of the window's list moved under regions [lo, a): the list and its three offset tables stay one order
def private moe_regions_rotate(var s : Session; lo, a, h : int64) {
    let nb = a - lo
    let n = h - lo + 1l
    ensure_length(g_rot_ord, n)
    for (i in range64(n)) {
        g_rot_ord[i] = s.moe_ord[lo + (i < n - nb ? nb + i : i - (n - nb))]
    }
    for (i in range64(n)) {
        s.moe_ord[lo + i] = g_rot_ord[i]
    }
    moe_offs_rotate(s.moe_offs1, lo, nb, n)
    moe_offs_rotate(s.moe_offs3, lo, nb, n)
    moe_offs_rotate(s.moe_offs2, lo, nb, n)
}

//! one offset table's triples of regions [lo, lo + n) rotated by `nb`: the last n - nb triples first
def private moe_offs_rotate(var offs : array<int64>; lo, nb, n : int64) {
    ensure_length(g_rot_offs, n * 3l)
    for (i in range64(n)) {
        let src = lo + (i < n - nb ? nb + i : i - (n - nb))
        for (j in range64(3l)) {
            g_rot_offs[i * 3l + j] = offs[src * 3l + j]
        }
    }
    for (i in range64(n * 3l)) {
        offs[lo * 3l + i] = g_rot_offs[i]
    }
}

//! every expert of the window's `nreg` regions readable now: the meet will rotate no region under the host's claim
def rare_window_ready(t : Model; s : Session; l, nreg : int64) : bool => rare_ready_prefix(t, s, l, 0l, nreg) == nreg

//! the regions [g, g_hi) cut at the first expert the cache does not hold yet: the device's bundle is the ready prefix
def private rare_ready_prefix(t : Model; s : Session; l, g, g_hi : int64) : int64 {
    for (r in range64(g, g_hi)) {
        return r if (!rare_expert_ready(t, l, int64(s.moe_ord[r].x)))
    }
    return g_hi
}

//! a lane's scratch for the batched rail: an expert's activation rows gathered contiguous, the cores' panel and block sums
struct private RareLane {
    xq : array<int8>
    xs : array<float>
    xbs : array<int>
    panel : array<uint8>
    bsums : array<int>
}

struct private RareLaneP {
    xq : int8?
    xs : float?
    xbs : int?
    panel : uint8?
    bsums : int?
    hist : int64?   //! RARE_HIST_N pairs of (runs, us) by run row count, this lane's (the gpu_prof report)
}

let private RARE_HIST_N = 6l   //! 1, 2, 3, 4-7, 8-15, 16+ rows a run
let private RARE_LANE_STATS = RARE_HIST_N * 2l + 2l   //! a lane's stats: the histogram's (runs, us) pairs, the rows its device-plane cores served, their us

var g_rare_dev_rows = 0l   //! the rows the rare rail read off the device planes, every layer and window (the matrix's witness)

def private rare_hist_bin(ntok : int64) : int64 => ntok <= 3l ? ntok - 1l : (ntok < 8l ? 3l : (ntok < 16l ? 4l : 5l))

var private @scratch g_rare_lanes : array<RareLane>
var private @scratch g_rare_lanep : array<RareLaneP>
var private @scratch g_rare_hist : array<int64>

//! the lanes' scratch sized for `rows` rows of `n` under the batch's planes, its pointers taken here in the main context
def private rare_lanes_size(rows, n, xs_per_row, xbs_per_row : int64; p1, p3, p2 : RarePlane) : RareLaneP? {
    let slots = get_dispatch_slot_bound()
    ensure_length(g_rare_lanes, int64(slots))
    ensure_length(g_rare_lanep, int64(slots))
    ensure_length(g_rare_hist, int64(slots) * RARE_LANE_STATS)
    for (h in g_rare_hist) {
        h = 0l
    }
    let panel = max(max(p1.panel_bytes, p3.panel_bytes), p2.panel_bytes)
    let bsums = max(max(b32_expert_bsum_ints(p1.f, n, rows), b32_expert_bsum_ints(p3.f, n, rows)), b32_expert_bsum_ints(p2.f, n, rows))
    unsafe {
        for (i in range(slots)) {
            var ln & = g_rare_lanes[i]
            ensure_length(ln.xq, rows * n)
            ensure_length(ln.xs, rows * xs_per_row)
            ensure_length(ln.xbs, max(rows * xbs_per_row, 1l))
            ensure_length(ln.panel, max(panel, 1l))
            ensure_length(ln.bsums, max(bsums, 1l))
            g_rare_lanep[i] = RareLaneP(xq = addr(ln.xq[0]), xs = addr(ln.xs[0]), xbs = addr(ln.xbs[0]), panel = addr(ln.panel[0]), bsums = addr(ln.bsums[0]),
                hist = addr(g_rare_hist[int64(i) * RARE_LANE_STATS]))
        }
        return addr(g_rare_lanep[0])
    }
}

//! the region at element offset `woff` of plane `p` as a batched GEMM over `ntok` contiguous activation rows, on the calling
//! lane: the superblock and per-32 batch cores read the plane once a tile; a q8 plane has no lane core and runs its rows
//! core a row
def private rare_batch_core(p : RarePlane; var yp : float?; woff : int64; xq : int8 const?; xs : float const?; xbs : int const?; n, d, ntok : int64; var ln : RareLaneP) {
    unsafe {
        if (p.dev_layout) {
            let t_dev = ref_time_ticks()
            var t = 0l
            let xs_row = n / p.belems   //! a superblock plane's activation carries a scale a superblock, the rest a 32-block; the Q8_K sums sixteen a superblock
            let ptrs = rare_dev_ptrs(p, woff)
            while (t + 4l <= ntok) {
                rm_dev_tile4(p, yp + t * d, ptrs.q, ptrs.s, xq + t * n, xs + t * xs_row, xbs != null ? xbs + t * (n / 16l) : null, n, d)
                t += 4l
            }
            while (t < ntok) {
                rare_rows_core(p, yp + t * d, woff, xq + t * n, xs + t * xs_row, xbs != null ? xbs + t * (n / 16l) : null, n, 0l, d)
                t++
            }
            ln.hist[RARE_HIST_N * 2l + 1l] += int64(get_time_usec(t_dev))
            return
        }
        if (p.f == KqFmt.q8) {
            for (t in range64(ntok)) {
                invoke(p.q8fn, yp + t * d, p.q8q + woff, p.q8s + woff / 32l, xq + t * n, xs + t * (n / 32l), n, 0l, d)
            }
            return
        }
        let blk = woff / p.belems
        let qp = p.q + blk * p.qsb
        let sp = p.s + blk * p.ssb
        if (p.belems == 256l) {
            kq_expert_batch_core(p.sid, yp, qp, sp, xq, xs, xbs, n, d, ntok, p.tb, p.panel_bytes > 0l ? ln.panel : null)
        } else {
            b32_expert_batch_core(p.f, yp, qp, sp, xq, xs, n, d, ntok, p.tb, ln.bsums)
        }
        rare_scale_planar_rows(p, yp, woff, ntok * d)
    }
}

//! the plane record of format `f`'s region space, `n` the plane's row width; `on` false where the format has no rows core
//! (mx4, a q8 plane under f16 scales)
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-arena")]
def private rare_plane(t : Model; f : KqFmt; n, woff : int64) : tuple<on : bool; p : RarePlane> {
    var p = RarePlane(f = f, base = woff, ege = expert_elems(t.config), scales = exps_scale_ptr(t, woff))
    unsafe {
        if (rare_arena_serves(t) && rm_dev_serves(f)) {
            p.view = moe_gpu_stream_view(woff, int(f))
            if (p.view.q != null) {   //! the stack's pinned planes, where it holds every expert resident; an unmapped stack views null and the ring serves it
                p.dev_layout = true
                p.q = p.view.q
                p.s = p.view.s
                let b = dev_block_bytes(f)
                p.qsb = b.qsb
                p.ssb = b.ssb
                p.belems = b.belems
                return (on = true, p = p)
            }
        }
        if (kq_sb(f) && t.kq_repacked) {
            let i = int(f)   //! the plane slot; the rows core is keyed by the schema id
            p.sid = kq_schema_id(f)
            p.q = addr<uint8 const?>(t.kq[i].q[0])
            p.s = addr<uint8 const?>(t.kq[i].s[0])
            p.qsb = kq_qsb(f)
            p.ssb = kq_ssb(f)
            p.belems = 256l
            p.tb = kq_token_block(p.sid, n)
            p.panel_bytes = kq_walk_panel_bytes(p.sid, n, kq_layout_of(p.sid))
            p.kqfn = kq_rows_fn(p.sid)
            return (on = true, p = p)
        }
        if (f == KqFmt.iq4nl32 || f == KqFmt.q51 || f == KqFmt.q4032 || f == KqFmt.q5032) {
            kq_plane_pair(t, f) $(q, sp) {
                p.q = addr<uint8 const?>(q[0])
                p.s = addr<uint8 const?>(sp[0])
            }
            p.qsb = kq_qsb(f)
            p.ssb = kq_ssb(f)
            p.belems = 32l
            p.tb = effective_token_block(get_q8_token_block(), n)
            return (on = true, p = p)
        }
        if (f == KqFmt.q8 && !t.wscale_f16) {
            p.q8q = addr<int8 const?>(t.qblob[0])
            p.q8s = addr<float const?>(t.qscales[0])
            p.q8fn = mm_q8q8_rows_fn()
            return (on = true, p = p)
        }
    }
    return (on = false, p = p)
}

//! four activation rows against every row of a device plane's region at `dq` / `ds` through the format's row-major
//! tile, token t's row at yp + t * d
def private rm_dev_tile4(p : RarePlane; var yp : float?; dq, ds : uint8 const?; xq : int8 const?; xs : float const?; xbs : int const?; n, d : int64) {   // nolint:STYLE037 - kq_fmt_stamp's generated arms, one a format
    let f = p.f
    kq_fmt_stamp(f) $(F) {
        rm_tile4_gen(yp, dq, ds, xq, xs, xbs, n, d, F)
    }
}

//! the region at element offset `woff` of plane `p` dotted with one activation row over rows [rb, re), on the calling lane
def private rare_rows_core(p : RarePlane; var yp : float?; woff : int64; xq : int8 const?; xs : float const?; xbs : int const?; n, rb, re : int64) {   // nolint:STYLE037 - kq_fmt_stamp's generated arms, one a format
    unsafe {
        if (p.dev_layout) {
            let ptrs = rare_dev_ptrs(p, woff)
            let f = p.f
            kq_fmt_stamp(f) $(F) {
                rm_gemv_gen(yp, ptrs.q, ptrs.s, xq, xs, xbs, n, rb, re, F)
            }
            return
        }
        if (p.f == KqFmt.q8) {
            invoke(p.q8fn, yp, p.q8q + woff, p.q8s + woff / 32l, xq, xs, n, rb, re)
            return
        }
        let blk = woff / p.belems
        let qp = p.q + blk * p.qsb
        let sp = p.s + blk * p.ssb
        if (p.belems == 256l) {
            invoke(p.kqfn, yp, qp, sp, xq, xs, xbs, n, rb, re)
        } elif (p.f == KqFmt.iq4nl32) {
            iq4nl32q8_gemv_gen(yp, qp, sp, xq, xs, xbs, n, rb, re)
        } elif (p.f == KqFmt.q51) {
            q51q8_gemv_gen(yp, qp, sp, xq, xs, xbs, n, rb, re)
        } elif (p.f == KqFmt.q4032) {
            q4032q8_gemv_gen(yp, qp, sp, xq, xs, xbs, n, rb, re)
        } else {
            q5032q8_gemv_gen(yp, qp, sp, xq, xs, xbs, n, rb, re)
        }
        rare_scale_planar_rows(p, yp + rb, woff, re - rb)
    }
}

//! the planar read's rows multiplied by expert `woff`'s scale
def private rare_scale_planar_rows(p : RarePlane; var yp : float?; woff, count : int64) {
    return if (p.scales == null)
    scale_inplace(yp, unsafe(p.scales[(woff - p.base) / p.ege]), count)
}

//! whether every plane of layer l's routed triple has a rows core the one-pass rail can call
def private rare_onepass_serves(t : Model; l : int64) : bool {
    let ep = expert_planes(t, l)
    for (f, off in ep.fmts, ep.offs) {
        return false if (!rare_plane(t, f, 256l, off).on)
    }
    return !t.config.moe_exps_bias
}

//! whether layer l's routed triple is the tier's pinned device-layout stack, so a token's host picks read it through the rail
//! (one copy of a streamed layer's experts): every plane device-served, the one-pass rail serving the layer
def private rare_dev_layer(t : Model; l : int64) : bool {
    return false if (!rare_arena_serves(t) || !rare_onepass_serves(t, l))
    let ep = expert_planes(t, l)
    for (f, off in ep.fmts, ep.offs) {
        return false if (!rare_plane(t, f, 256l, off).p.dev_layout)
    }
    return true
}

var g_rare_dec_rows = 0l   //! the (row, expert) pairs a token's or a window's host sum read off the device planes (the decode cells' witness)
var g_rare_slot_copies = 0l   //! the hot pool slots filled off a streamed layer's pinned mirror (the pool's witness that no planar plane was read)

//! a planar read of layer `l`'s experts on a trimmed GPU lane is a panic by name: the lane carries no planar family
[cold_path]
def trim_planar_guard(t : Model; l : int64; what : string) {
    if (t.planes_trimmed) {
        panic("dasLLAMA: planes_trimmed GPU lane - {what} would read routed layer {l}'s experts off the planar planes the lane does not carry (the stream mirror or the rare rail declined this run; the decline line above)")
    }
}

//! the expert plane at `eoff` of the stack at `base` copied in the device layout off the tier's pinned mirror into `wq` / `ws`; false where the stack is not device-served, and the caller gathers off the planar planes
def rare_expert_slot_copy(t : Model; f : KqFmt; n, rows, base, eoff : int64; var wq : array<uint8>; var ws : array<uint8>) : bool {
    return false if (!rare_arena_on() || !rm_dev_serves(f) || !moe_gpu_stream_fetch(base, int(f), (eoff - base) / (t.config.dim * t.config.n_ff_exp)))
    let p = rare_plane(t, f, n, base).p   //! after the fetch: the ring the view carries may be born by it
    return false if (!p.dev_layout)
    let ptrs = rare_dev_ptrs(p, eoff)
    let nblk = n * rows / p.belems
    wq |> grow_resize(nblk * p.qsb)
    ws |> grow_resize(nblk * p.ssb)
    unsafe {
        memcpy(addr<void?>(wq[0]), reinterpret<void const?>(ptrs.q), nblk * p.qsb)
        memcpy(addr<void?>(ws[0]), reinterpret<void const?>(ptrs.s), nblk * p.ssb)
    }
    g_rare_slot_copies++
    return true
}

//! one GEMV pass of the one-row rail: `nreg` regions of plane `p` - region j's weights at `woffs[j]`, its activation row the
//! `xoffs[j]`th of the image (`xq`, `xs` a scale every `xs_per` elements, `xbs` a sum every 16) - each region's `d` rows split
//! into `nch` chunks, one fork over the (region, chunk) pairs, region j's row into `y + j * d`
def private rm_rail_pass(p : RarePlane; var y : float?; woffs, xoffs : int64 const?; nreg : int64; xq : int8 const?; xs : float const?; xbs : int const?; xs_per, n, d : int64) {
    let lanes = get_dispatch_lanes()
    let nch = clamp((2l * int64(lanes) + nreg - 1l) / nreg, 1l, d / 8l)   //! two work items a lane at least, eight rows a chunk at least
    let rows = (d + nch - 1l) / nch
    let rows8 = (rows + 7l) / 8l * 8l
    let nchunks = (d + rows8 - 1l) / rows8
    unsafe {
        maybe_parallel_for_indexed(0, int(nreg * nchunks), lanes) $(_slot, jb, je) {
            unsafe {
                for (w in range(jb, je)) {
                    let j = int64(w) / nchunks
                    let rb = (int64(w) % nchunks) * rows8
                    let re = min(d, rb + rows8)
                    let xo = xoffs[j]
                    rare_rows_core(p, y + j * d, woffs[j], xq + xo, xs + xo / xs_per, xbs != null ? xbs + xo / 16l : null, n, rb, re)
                }
            }
        }
    }
}

//! the first `k` picks of s.moe_idx / s.moe_w summed into s.moe_acc through the rail's row-major kernels; false where a plane of the triple is not device-served, and `moe_routed_sum_k` serves
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-arena")]
def moe_routed_rail_k(t : Model; var s : Session; l, k : int64) : bool {
    return false if (!rare_dev_layer(t, l))
    let c = t.config
    let dim = c.dim
    let nfe = c.n_ff_exp
    let ege = expert_elems(c)
    for (i in range64(dim)) {
        s.moe_acc[i] = 0.0
    }
    return true if (k == 0l)
    let f1 = expert_fmt(t, t.we1_fmt, l)
    let f3 = expert_fmt(t, t.we3_fmt, l)
    let f2 = expert_fmt(t, t.we2_fmt, l)
    moe_quant_feed(t, s, l, moe_layer_kq(t, l))
    for (j in range64(k)) {   //! queued at once and read off the lane before the planes are captured: the ring may be born here
        rare_request_expert(t, l, s.moe_idx[j])
        expert_heat_add(t, l, s.moe_idx[j], 1l)
    }
    for (j in range64(k)) {
        rare_fetch_expert(t, l, s.moe_idx[j])
    }
    let p1 = rare_plane(t, f1, dim, t.we1_offs[l]).p
    let p3 = rare_plane(t, f3, dim, t.we3_offs[l]).p
    let p2 = rare_plane(t, f2, nfe, t.we2_offs[l]).p
    var woffs, xoffs : int64[RDEC_RAIL_PICKS]
    assert(k <= int64(RDEC_RAIL_PICKS), "moe_routed_rail_k: more picks than the rail's region list holds")
    let ts_gu = prof_ticks()
    unsafe {
        for (j in range64(k)) {
            xoffs[j] = 0l   //! every pick reads the one row's image
        }
        let xq8 = addr<int8 const?>(s.moe_xq[0])
        let xs8 = addr<float const?>(s.moe_xs[0])
        let xqk = addr<int8 const?>(s.kxq[0])
        let xsk = addr<float const?>(s.kxs[0])
        let xbk = addr<int const?>(s.kxbs[0])
        for (j in range64(k)) {
            woffs[j] = t.we1_offs[l] + s.moe_idx[j] * ege
        }
        if (kq_sb(f1)) {
            rm_rail_pass(p1, addr(s.moe_gu[0]), addr<int64 const?>(woffs[0]), addr<int64 const?>(xoffs[0]), k, xqk, xsk, xbk, 256l, dim, nfe)
        } else {
            rm_rail_pass(p1, addr(s.moe_gu[0]), addr<int64 const?>(woffs[0]), addr<int64 const?>(xoffs[0]), k, xq8, xs8, null, 32l, dim, nfe)
        }
        for (j in range64(k)) {
            woffs[j] = t.we3_offs[l] + s.moe_idx[j] * ege
        }
        if (kq_sb(f3)) {
            rm_rail_pass(p3, addr(s.moe_gu2[0]), addr<int64 const?>(woffs[0]), addr<int64 const?>(xoffs[0]), k, xqk, xsk, xbk, 256l, dim, nfe)
        } else {
            rm_rail_pass(p3, addr(s.moe_gu2[0]), addr<int64 const?>(woffs[0]), addr<int64 const?>(xoffs[0]), k, xq8, xs8, null, 32l, dim, nfe)
        }
    }
    prof_add("mm_moe", ts_gu)
    let ts_act = prof_ticks()
    gate_batch(s.moe_gu, s.moe_gu2, c.ffn_act, nfe, k)
    prof_add("act", ts_act)
    requant_rows_f(f2, s.moe_gu, nfe, k, s.moe_dq, s.moe_ds, s.moe_dbs)
    let ts_down = prof_ticks()
    unsafe {
        for (j in range64(k)) {
            woffs[j] = t.we2_offs[l] + s.moe_idx[j] * ege
            xoffs[j] = j * nfe   //! each down reads its own requantized row
        }
        let dsb = kq_sb(f2)
        rm_rail_pass(p2, addr(s.moe_dn[0]), addr<int64 const?>(woffs[0]), addr<int64 const?>(xoffs[0]), k, addr<int8 const?>(s.moe_dq[0]),
            addr<float const?>(s.moe_ds[0]), dsb ? addr<int const?>(s.moe_dbs[0]) : null, dsb ? 256l : 32l, nfe, dim)
    }
    prof_add("mm_moe_dn", ts_down)
    moe_gpu_stream_ring_release()
    moe_routed_reduce(s, k, dim)
    moe_trace_row(s, l, k, dim)
    g_rare_dec_rows += k
    return true
}

let private RDEC_RAIL_PICKS = 128   //! the one-row rail's region list: the routed slots a decode step stages (RDEC_ROUTED_SLOTS), a row's picks at most

let private RARE_EXPERTS = 32l   //! the meet's host claim: this many of the rarest experts at a time, or
let private RARE_ROWS = 256l     //! ... this many rows - one batch of the rare rail, the ring checked between claims

var g_moe_meet_gpu_experts = 0l   //! the meet's experts the device took, every layer and window (the probe's and the tests' witness)
var g_moe_meet_cpu_experts = 0l   //! ... and the host
var g_moe_meet_gpu_rows = 0l
var g_moe_meet_cpu_rows = 0l
var g_moe_meet_last_gpu_rows = 0l   //! the last meet's rows a side, for its caller's report line
var g_moe_meet_last_cpu_rows = 0l
var private g_moe_host_first_pin = -1l   //! `set_moe_host_first`'s pin: -1 reads the environment (DASLLAMA_MOE_HOST_FIRST)

//! a fixture's lever over DASLLAMA_MOE_HOST_FIRST: the meet's host share starts with `experts` of the rarest experts whatever the
//! device's pace, so a cell reaches the host rail on a carrier the device would take whole; 0 lifts it, -1 reads the environment
def set_moe_host_first(experts : int64) {
    g_moe_host_first_pin = experts
}

//! the pin as it was written (-1 = none): a fixture puts back what it found
def moe_host_first_pin : int64 => g_moe_host_first_pin

def private moe_meet_host_first() : int64 => g_moe_host_first_pin >= 0l ? g_moe_host_first_pin : g_env_engine.moe_host_first

//! returns the device's expert count, -1 where the knob is off or the chain declined to open; the host experts' rows sit in s.moe_eout under s.moe_w_cpu, and the caller joins s.moe_gpu_out and sums the two
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
def moe_prefill_meet(t : Model; var s : Session; l, nrows, nreg : int64; f1, f3, f2 : int; dev_limit : int64 = -1l) : int64 {
    let c = t.config
    let dim = c.dim
    let nk = nrows * c.n_expert_used
    return -1l if (!gpu_want_moe_split() || !matmul_moe_gpu_ffn_combined_xf_open(s.moe_offs1, s.moe_offs3, s.moe_offs2, nreg, s.xb_b, s.moe_w_b, s.moe_inv,
        dim, c.n_ff_exp, nk, nrows, f1, f3, f2, c.ffn_act == FfnAct.gelu))
    for (r in range64(nreg)) {
        rare_request_expert(t, l, int64(s.moe_ord[r].x))
    }
    expert_heat_add_regions(t, s, l, 0l, nreg - 1l)
    moe_routed_rows_requant(t, s, l, nrows)
    s.moe_exp_gpu |> grow_resize(c.n_expert)
    for (e in range64(c.n_expert)) {
        s.moe_exp_gpu[e] = int8(0)
    }
    var g = 0l
    var h = nreg - 1l
    var gpu_rows = 0l
    var cpu_rows = 0l
    var host_first = min(moe_meet_host_first(), nreg)
    let dl = dev_limit < 0l ? nreg : dev_limit   //! the device claims no region past it: the pool window's host-only tail
    let host_cap = nrows == 1l ? 1l : RARE_EXPERTS   //! one row an expert: the host takes one at a time, so the device keeps claiming between them
    while (true) {
        var a = min(rare_ready_prefix(t, s, l, g, h + 1l), dl)
        while (g < a && host_first <= 0l) {
            let g_hi = matmul_moe_gpu_ffn_combined_xf_bundle(g, a)
            break if (!matmul_moe_gpu_ffn_combined_xf_push(g, g_hi, false))
            for (r in range64(g, g_hi)) {
                s.moe_exp_gpu[int64(s.moe_ord[r].x)] = int8(1)
                gpu_rows += int64(s.moe_ord[r].y)
            }
            g = g_hi
        }
        break if (g > h)
        if (a == g) {
            if (g >= dl) {
                a = h + 1l
            } else {
                rare_fetch_expert(t, l, int64(s.moe_ord[g].x))
                continue
            }
        }
        //! under a device limit the host claims off the list's tail: the pool window's copy and victim lists are frozen by
        //! region, and a claim rotated up from under the limit would move the regions under them
        let top = dl < nreg ? h : a - 1l
        var lo = top
        var rows = int64(s.moe_ord[top].y)
        while (lo > g && top - lo + 1l < host_cap && rows + int64(s.moe_ord[lo - 1l].y) <= RARE_ROWS) {
            lo--
            rows += int64(s.moe_ord[lo].y)
        }
        if (top < h) {
            moe_regions_rotate(s, lo, top + 1l, h)
            lo = h - (top - lo)
        }
        host_first -= h - lo + 1l
        let tb = ref_time_ticks()
        rare_fetch_regions(t, s, l, lo, h)   //! the batch's experts landed before the lanes fork
        moe_prefill_rare_rows(t, s, l, lo, h)
        if (g_env_gpu.gpu_prof) {
            to_log(LOG_INFO, "rdec_rare l{l}: {h - lo + 1l} experts {rows} rows {get_time_usec(tb)} us\n")   // nolint:PERF026,PERF028,LINT019 — gpu_prof-gated report leg
        }
        cpu_rows += rows
        h = lo - 1l
    }
    matmul_moe_gpu_ffn_combined_xf_close()
    s.moe_w_cpu |> grow_resize(nk)
    for (j in range64(nk)) {
        s.moe_w_cpu[j] = s.moe_exp_gpu[s.moe_idx_b[j]] != int8(0) ? 0.0 : s.moe_w_b[j]
    }
    g_moe_meet_gpu_experts += g
    g_moe_meet_cpu_experts += nreg - g
    g_moe_meet_gpu_rows += gpu_rows
    g_moe_meet_cpu_rows += cpu_rows
    g_moe_meet_last_gpu_rows = gpu_rows
    g_moe_meet_last_cpu_rows = cpu_rows
    return g
}

//! regions [lo, hi] of s.moe_ord as (expert, row) pairs, each row's chain parked into s.moe_eout at its slot; the three-pass region-list form serves the formats with no rows core, and the A/B arm
def moe_prefill_rare_rows(t : Model; var s : Session; l, lo, hi : int64) {
    if (rare_onepass_on() && rare_onepass_serves(t, l)) {
        moe_prefill_rare_rows_onepass(t, s, l, lo, hi)
    } else {
        trim_planar_guard(t, l, "the meet's three-pass host share") if (t.planes_trimmed)
        moe_prefill_rare_rows_passes(t, s, l, lo, hi)
    }
}

//! the pairs of regions [lo, hi] of s.moe_ord: each row's expert, its bucket row and its position
def private rare_pairs(s : Session; k, lo, hi : int64; var pe : array<int64>; var pb : array<int64>; var pp : array<int64>) : int64 {
    var np = 0l
    for (r in range64(lo, hi + 1l)) {
        np += int64(s.moe_ord[r].y)
    }
    ensure_length(pe, np)
    ensure_length(pb, np)
    ensure_length(pp, np)
    var j = 0l
    for (r in range64(lo, hi + 1l)) {
        let e = int64(s.moe_ord[r].x)
        let b0 = e == 0l ? 0l : s.moe_cnt[e - 1l]
        for (b in range64(int64(s.moe_ord[r].y))) {
            pe[j] = e
            pb[j] = b0 + b
            pp[j] = s.moe_bkt[b0 + b] / k
            j++
        }
    }
    return np
}

var private @scratch g_rare_pe : array<int64>
var private @scratch g_rare_pb : array<int64>
var private @scratch g_rare_pp : array<int64>

[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
def private moe_prefill_rare_rows_onepass(t : Model; var s : Session; l, lo, hi : int64) {   // nolint:STYLE038 — one lane body: the region lists, the fork, the report
    let c = t.config
    let dim = c.dim
    let nfe = c.n_ff_exp
    let k = c.n_expert_used
    let ege = expert_elems(c)
    let f1 = expert_fmt(t, t.we1_fmt, l)
    let f2 = expert_fmt(t, t.we2_fmt, l)
    let f3 = expert_fmt(t, t.we3_fmt, l)
    let kform = moe_layer_kq(t, l) && moe_pair_kform(f1, f3)
    let dn_sb = kq_sb(f2)
    let np = rare_pairs(s, k, lo, hi, g_rare_pe, g_rare_pb, g_rare_pp)
    return if (np == 0l)
    ensure_length(s.moe_gu, np * nfe)
    ensure_length(s.moe_gu2, np * nfe)
    ensure_length(s.moe_dq, np * nfe)
    ensure_length(s.moe_ds, np * nfe / 32l)
    ensure_length(s.moe_dbs, np * nfe / 16l)
    ensure_length(s.moe_dn, np * dim)
    let ts = prof_ticks()
    let is_gelu = c.ffn_act == FfnAct.gelu
    let is_oai = c.ffn_act == FfnAct.swiglu_oai
    let vec = g_act_vec
    let we1 = t.we1_offs[l]
    let we3 = t.we3_offs[l]
    let we2 = t.we2_offs[l]
    let p1 = rare_plane(t, f1, dim, we1).p
    let p3 = rare_plane(t, f3, dim, we3).p
    let p2 = rare_plane(t, f2, nfe, we2).p
    let xs_stride = kform ? dim / 256l : dim / 32l
    let xbs_stride = kform ? dim / 16l : 0l
    let batched = rare_batched_on()
    var lanes = rare_lanes_size(np, dim, xs_stride, xbs_stride, p1, p3, p2)
    unsafe {
        let xq0 = kform ? addr<int8 const?>(s.kxqb[0]) : addr<int8 const?>(s.xqb[0])
        let xs0 = kform ? addr<float const?>(s.kxsb[0]) : addr<float const?>(s.xsb[0])
        let xbs0 = kform ? addr<int const?>(s.kxbsb[0]) : null
        let hs_stride = dn_sb ? nfe / 256l : nfe / 32l
        let hbs_stride = nfe / 16l
        var gu = addr(s.moe_gu[0])
        var gu2 = addr(s.moe_gu2[0])
        var dq = addr(s.moe_dq[0])
        var ds = addr(s.moe_ds[0])
        var dbs = addr(s.moe_dbs[0])
        var dn = addr(s.moe_dn[0])
        var eout = addr(s.moe_eout[0])
        let pe = addr<int64 const?>(g_rare_pe[0])
        let pb = addr<int64 const?>(g_rare_pb[0])
        let pp = addr<int64 const?>(g_rare_pp[0])
        let bkt = addr<int64 const?>(s.moe_bkt[0])
        maybe_parallel_for_indexed(0, int(np), get_dispatch_lanes()) $(slot, jb, je) {
            unsafe {
                var ln & = lanes[slot]
                var j = int64(jb)
                while (j < int64(je)) {
                    let e = pe[j]
                    var run_end = j + 1l
                    while (run_end < int64(je) && pe[run_end] == e) {
                        run_end++
                    }
                    let ntok = run_end - j
                    let t_run = ref_time_ticks()
                    for (ti in range64(ntok)) {
                        let p = pp[j + ti]
                        memcpy(ln.xq + ti * dim, xq0 + p * dim, dim)
                        memcpy(ln.xs + ti * xs_stride, xs0 + p * xs_stride, xs_stride * 4l)
                        if (xbs0 != null) {
                            memcpy(ln.xbs + ti * xbs_stride, xbs0 + p * xbs_stride, xbs_stride * 4l)
                        }
                    }
                    let lxbs = xbs0 != null ? ln.xbs : null
                    if (batched) {
                        rare_batch_core(p1, gu + j * nfe, we1 + e * ege, ln.xq, ln.xs, lxbs, dim, nfe, ntok, ln)
                        rare_batch_core(p3, gu2 + j * nfe, we3 + e * ege, ln.xq, ln.xs, lxbs, dim, nfe, ntok, ln)
                    } else {
                        for (ti in range64(ntok)) {
                            let xq = ln.xq + ti * dim
                            let xs = ln.xs + ti * xs_stride
                            let xbs = lxbs != null ? lxbs + ti * xbs_stride : null
                            rare_rows_core(p1, gu + (j + ti) * nfe, we1 + e * ege, xq, xs, xbs, dim, 0l, nfe)
                            rare_rows_core(p3, gu2 + (j + ti) * nfe, we3 + e * ege, xq, xs, xbs, dim, 0l, nfe)
                        }
                    }
                    for (ti in range64(ntok)) {
                        var g = gu + (j + ti) * nfe
                        let u = gu2 + (j + ti) * nfe
                        if (vec) {
                            if (is_gelu) {
                                geglu4(g, u, nfe)
                            } elif (is_oai) {
                                swiglu_oai4(g, u, nfe)
                            } else {
                                swiglu4(g, u, nfe)
                            }
                        } elif (is_gelu) {
                            geglu(g, u, nfe)
                        } elif (is_oai) {
                            swiglu_oai(g, u, nfe)
                        } else {
                            swiglu(g, u, nfe)
                        }
                        if (dn_sb) {
                            quantize_q8_k_into_ptr(g, nfe, dq, ds, dbs, (j + ti) * nfe, (j + ti) * hs_stride, (j + ti) * hbs_stride)
                        } else {
                            quantize_q8_0_bs_into_ptr(g, nfe, dq, ds, dbs, (j + ti) * nfe, (j + ti) * hs_stride, (j + ti) * hbs_stride)
                        }
                    }
                    if (batched) {
                        rare_batch_core(p2, dn + j * dim, we2 + e * ege, dq + j * nfe, ds + j * hs_stride, dbs + j * hbs_stride, nfe, dim, ntok, ln)
                    } else {
                        for (ti in range64(ntok)) {
                            rare_rows_core(p2, dn + (j + ti) * dim, we2 + e * ege, dq + (j + ti) * nfe, ds + (j + ti) * hs_stride, dbs + (j + ti) * hbs_stride, nfe, 0l, dim)
                        }
                    }
                    for (ti in range64(ntok)) {
                        copy_floats(eout + bkt[pb[j + ti]] * dim, dn + (j + ti) * dim, dim)
                    }
                    let hb = rare_hist_bin(ntok) * 2l
                    ln.hist[hb]++
                    ln.hist[hb + 1l] += int64(get_time_usec(t_run))
                    if (p1.dev_layout) {
                        ln.hist[RARE_HIST_N * 2l] += ntok
                    }
                    j = run_end
                }
            }
        }
    }
    prof_add("rare_onepass", ts)
    for (i in range64(long_length(g_rare_hist) / RARE_LANE_STATS)) {
        g_rare_dev_rows += g_rare_hist[i * RARE_LANE_STATS + RARE_HIST_N * 2l]
    }
    if (g_env_gpu.gpu_prof) {
        var line = "rare_hist l{l}:"   // nolint:PERF026,PERF028,LINT019 — gpu_prof-gated report line
        for (b in range64(RARE_HIST_N)) {
            var runs = 0l
            var us = 0l
            for (i in range64(long_length(g_rare_hist) / RARE_LANE_STATS)) {
                runs += g_rare_hist[i * RARE_LANE_STATS + b * 2l]
                us += g_rare_hist[i * RARE_LANE_STATS + b * 2l + 1l]
            }
            line += " b{b} {runs} {us}"   // nolint:PERF001,PERF026,PERF028,LINT019 — gpu_prof-gated report line, a dozen bins
        }
        var dev_us = 0l
        for (i in range64(long_length(g_rare_hist) / RARE_LANE_STATS)) {
            dev_us += g_rare_hist[i * RARE_LANE_STATS + RARE_HIST_N * 2l + 1l]
        }
        line += " dev_core_us {dev_us}"   // nolint:PERF026,PERF028,LINT019 — gpu_prof-gated report line
        to_log(LOG_INFO, "{line}\n")   // nolint:PERF026,PERF028,LINT019 — gpu_prof-gated report line
    }
}

//! the three-pass form: one region-list GEMV chain over the pairs - the decode's routed chain's shape, three fork/joins a batch
def private moe_prefill_rare_rows_passes(t : Model; var s : Session; l, lo, hi : int64) {
    let c = t.config
    let dim = c.dim
    let nfe = c.n_ff_exp
    let ne = c.n_expert
    let k = c.n_expert_used
    let ege = expert_elems(c)
    let f1 = fmt_at(t.we1_fmt, l)
    let f2 = fmt_at(t.we2_fmt, l)
    let f3 = fmt_at(t.we3_fmt, l)
    var np = 0l
    for (r in range64(lo, hi + 1l)) {
        np += int64(s.moe_ord[r].y)
    }
    return if (np == 0l)
    //! grow-only: the decode's routed chain shares these at its own k-row size, and a shorter window must not cut them
    ensure_length(s.moe_offs, np * 2l)
    ensure_length(s.moe_boffs, np)
    ensure_length(s.moe_gu, np * nfe)
    ensure_length(s.moe_gu2, np * nfe)
    ensure_length(s.moe_dq, np * nfe)
    ensure_length(s.moe_ds, np * nfe / 32l)
    ensure_length(s.moe_dbs, np * nfe / 16l)
    ensure_length(s.moe_dn, np * dim)
    for (leg in range(3)) {
        var j = 0l
        for (r in range64(lo, hi + 1l)) {
            let e = int64(s.moe_ord[r].x)
            let b0 = e == 0l ? 0l : s.moe_cnt[e - 1l]
            let eg = l * ne + e
            for (b in range64(int64(s.moe_ord[r].y))) {
                let p = s.moe_bkt[b0 + b] / k
                if (leg == 0) {
                    s.moe_offs[j * 2l] = t.we1_offs[l] + e * ege
                    s.moe_offs[j * 2l + 1l] = p * dim
                    s.moe_boffs[j] = t.web1_off + eg * nfe
                } elif (leg == 1) {
                    s.moe_offs[j * 2l] = t.we3_offs[l] + e * ege
                    s.moe_offs[j * 2l + 1l] = p * dim
                    s.moe_boffs[j] = t.web3_off + eg * nfe
                } else {
                    s.moe_offs[j * 2l] = t.we2_offs[l] + e * ege
                    s.moe_offs[j * 2l + 1l] = j * nfe
                    s.moe_boffs[j] = t.web2_off + eg * dim
                }
                j++
            }
        }
        if (leg == 0) {
            let ts_gu = prof_ticks()
            mm_at_f_groupn(s.moe_gu, t, f1, s.moe_offs, s.moe_boffs, c.moe_exps_bias, np, s.xqb, s.xsb, s.kxqb, s.kxsb, s.kxbsb, dim, nfe)
            prof_add("rare_gate", ts_gu)
        } elif (leg == 1) {
            let ts_up = prof_ticks()
            mm_at_f_groupn(s.moe_gu2, t, f3, s.moe_offs, s.moe_boffs, c.moe_exps_bias, np, s.xqb, s.xsb, s.kxqb, s.kxsb, s.kxbsb, dim, nfe)
            prof_add("rare_up", ts_up)
            let ts_act = prof_ticks()
            gate_batch(s.moe_gu, s.moe_gu2, c.ffn_act, nfe, np)
            prof_add("rare_act", ts_act)
            let ts_rq = prof_ticks()
            requant_rows_f(f2, s.moe_gu, nfe, np, s.moe_dq, s.moe_ds, s.moe_dbs)
            prof_add("rare_requant", ts_rq)
        } else {
            let ts_dn = prof_ticks()
            mm_at_f_groupn(s.moe_dn, t, f2, s.moe_offs, s.moe_boffs, c.moe_exps_bias, np, s.moe_dq, s.moe_ds, s.moe_dq, s.moe_ds, s.moe_dbs, nfe, dim)
            prof_add("rare_down", ts_dn)
        }
    }
    let ts_park = prof_ticks()
    var j = 0l
    for (r in range64(lo, hi + 1l)) {
        let e = int64(s.moe_ord[r].x)
        let b0 = e == 0l ? 0l : s.moe_cnt[e - 1l]
        for (b in range64(int64(s.moe_ord[r].y))) {
            copy_floats(s.moe_dn, j * dim, s.moe_eout, s.moe_bkt[b0 + b] * dim, dim)
            j++
        }
    }
    prof_add("rare_park", ts_park)
}

//! one expert's chain over its bucket rows - the gather, gate and up, the act, the requant, down - parked into s.moe_eout at the rows' slots
def moe_prefill_expert_chain(t : Model; var s : Session; l, e : int64; moe_kform : bool) {
    let c = t.config
    let dim = c.dim
    let ne = c.n_expert
    let k = c.n_expert_used
    let nfe = c.n_ff_exp
    let f1 = fmt_at(t.we1_fmt, l)
    let f2 = fmt_at(t.we2_fmt, l)
    let f3 = fmt_at(t.we3_fmt, l)
    let b0 = e == 0l ? 0l : s.moe_cnt[e - 1l]
    let cnt = s.moe_cnt[e] - b0
    {
        let ts_gather = prof_ticks()
        moe_gather_rows(s, b0, cnt, dim, k, moe_kform, moe_kform)
        prof_add("moe_gather", ts_gather)
        let eg = l * ne + e
        let base = e * dim * nfe
        moe_expert_gate_up(t, s, f1, f3, t.we1_offs[l] + base, t.we3_offs[l] + base, dim, nfe, cnt)
        if (c.moe_exps_bias) {  // gpt-oss: this expert's gate/up biases before the activation
            for (r in range64(cnt)) {
                add_bias(s.moe_ghb, r * nfe, t.fblob, t.web1_off + eg * nfe, nfe)
                add_bias(s.moe_ghb2, r * nfe, t.fblob, t.web3_off + eg * nfe, nfe)
            }
        }
        let ts_act = prof_ticks()
        gate_batch(s.moe_ghb, s.moe_ghb2, c.ffn_act, nfe, cnt)
        prof_add("act", ts_act)
        requant_rows_f(f2, s.moe_ghb, nfe, cnt, s.moe_hq, s.moe_hs, s.moe_hbs)
        moe_expert_down(t, s, f2, t.we2_offs[l] + base, dim, nfe, cnt)
        if (c.moe_exps_bias) {  // ... and its down bias, added before the routed weight scales it
            for (r in range64(cnt)) {
                add_bias(s.moe_gout, r * dim, t.fblob, t.web2_off + eg * dim, dim)
            }
        }
        let ts_park = prof_ticks()
        for (r in range64(cnt)) {
            copy_floats(s.moe_gout, r * dim, s.moe_eout, s.moe_bkt[b0 + r] * dim, dim)
        }
        prof_add("moe_park", ts_park)
    }
}

//! slot order, decode's accumulation order; a zero-weight slot (a split's device share) is skipped: its parked row is stale
def private moe_reduce_row(var arow : float?; ep : float const?; wp : float const?; k, dim : int64) {
    unsafe {
        for (i in range64(dim)) {
            arow[i] = 0.0
        }
        for (j in range64(k)) {
            let w = wp[j]
            continue if (w == 0.0)
            let er = ep + j * dim
            for (i in range64(dim)) {
                arow[i] += w * er[i]
            }
        }
    }
}

def private moe_prefill_scratch(t : Model; var s : Session; npos, grows : int64; kq_layer : bool) {
    let c = t.config
    let dim = c.dim
    let nb = dim / 32l
    let ne = c.n_expert
    let nk = npos * c.n_expert_used
    let nfx = max(c.n_ff_exp, c.n_ff_shexp)
    s.moe_rlog |> grow_resize(npos * ne)
    s.moe_idx_b |> grow_resize(nk)
    s.moe_w_b |> grow_resize(nk)
    s.moe_cnt |> grow_resize(ne)
    s.moe_bkt |> grow_resize(nk)
    s.moe_gxq |> grow_resize(grows * dim)
    s.moe_gxs |> grow_resize(grows * nb)
    s.moe_ghb |> grow_resize(grows * nfx)
    s.moe_ghb2 |> grow_resize(grows * nfx)
    s.moe_hq |> grow_resize(grows * nfx)
    s.moe_hs |> grow_resize(grows * nfx / 32l)
    s.moe_gout |> grow_resize(grows * dim)
    s.moe_eout |> grow_resize(nk * dim)
    if (c.n_ff_shexp > 0l && c.moe_shexp_gated) {
        s.moe_gl_b |> grow_resize(npos)
    }
    if (kq_layer) {
        s.moe_gxbs |> grow_resize(grows * dim / 16l)
        s.moe_hbs |> grow_resize(grows * nfx / 16l)
    }
}

//! the router bias adds into s.moe_rlog in place; the shared gate's logit is left to the driver's scatter
def private moe_prefill_select_rows(t : Model; var s : Session; l, npos : int64) {
    let c = t.config
    let ne = c.n_expert
    let k = c.n_expert_used
    let ts_select = prof_ticks()
    unsafe {
        var rlp = addr(s.moe_rlog[0])
        var idxp = addr(s.moe_idx_b[0])
        var wp = addr(s.moe_w_b[0])
        let rbp = c.moe_router_bias ? addr<float const?>(t.fblob[t.router_b_off + l * ne]) : null
        let ebp = c.moe_exp_probs ? addr<float const?>(t.fblob[t.exp_probs_off + l * ne]) : null
        let gate = c.moe_gate
        let norm_topk = c.norm_topk_prob
        let wscale = c.expert_weights_scale
        let tp = moe_pick_tape_rows_begin(npos, k)
        maybe_parallel_for(npos > 1l, 0, int(npos), get_dispatch_lanes()) $(pb, pe) {
            unsafe {
                for (pp in range(pb, pe)) {
                    let p = int64(pp)
                    var lgp = rlp + p * ne
                    if (rbp != null) {
                        for (i in range64(ne)) {
                            lgp[i] += rbp[i]
                        }
                    }
                    if (tp != null) {
                        moe_pick_tape_mask_row(lgp, ne, k, tp + p * k)
                    }
                    moe_select_core(gate, ne, k, norm_topk, wscale, lgp, idxp, wp, p * k, ebp)
                }
            }
        }
    }
    moe_pick_tape_rows_done(s.moe_idx_b, npos, k)
    moe_prefill_bucket(s, npos * k, ne)
    prof_add("moe_route", ts_select)
}

//! The routed experts' sums of `npos` rows alone, on the CPU, for a driver that holds every other plane of the layer: the rows
//! in s.xb_b and the router logits in s.moe_rlog (position-major, `ne` a row) -> s.xb2_b = the k experts' outputs under their
//! weights, a row a position - the grouped path's CPU stages; the shared expert and the residual are the driver's
def moe_routed_sum_rows(t : Model; var s : Session; l, npos : int64) {
    moe_routed_rows_select(t, s, l, npos)
    moe_routed_rows_sum(t, s, l, npos, false)
}

//! the rows' routed chain takes the fused region dispatches, else a chain an expert
def private moe_routed_rows_take_fused_regions(t : Model; l : int64) : bool {
    let f1 = fmt_at(t.we1_fmt, l)
    let f2 = fmt_at(t.we2_fmt, l)
    let f3 = fmt_at(t.we3_fmt, l)
    let sb_fusable = t.kq_repacked && kernel_backend_has_kq_batch_groupn()
    return ((t.experts_mx4 && g_active_mx4_native_batch && g_moe_fused_expert_gemms)
        || (moe_layer_kq(t, l) && g_moe_fused_expert_gemms
            && (kq_sb(f1) ? sb_fusable : f1 == KqFmt.q51)
            && (kq_sb(f2) ? sb_fusable : f2 == KqFmt.q51)
            && (kq_sb(f3) ? sb_fusable : f3 == KqFmt.q51)))
}

//! `moe_routed_sum_rows`' first half: the rows' selects off s.moe_rlog into s.moe_idx_b / s.moe_w_b and the bucket by expert
//! (s.moe_cnt an expert's run end, s.moe_bkt the slot rows in expert order) - what a driver reads to split the picks
def moe_routed_rows_select(t : Model; var s : Session; l, npos : int64) {
    let nk = npos * t.config.n_expert_used
    moe_prefill_scratch(t, s, npos, moe_routed_rows_take_fused_regions(t, l) ? nk : npos, moe_layer_kq(t, l))
    s.xb2_b |> grow_resize(npos * t.config.dim)
    moe_prefill_select_rows(t, s, l, npos)
}

//! ... and its second: the experts' chains and the rows' sums into s.xb2_b. Under `split` the experts marked in
//! s.moe_exp_gpu are a driver's and s.moe_w_cpu carries the weights with their slots at zero: the sums are the rest's alone
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-arena")]
def moe_routed_rows_sum(t : Model; var s : Session; l, npos : int64; skip_gpu_experts : bool) {
    let c = t.config
    let dim = c.dim
    let k = c.n_expert_used
    let nk = npos * k
    let fused = moe_routed_rows_take_fused_regions(t, l)
    let moe_kform = moe_routed_rows_requant(t, s, l, npos)
    if (rare_dev_layer(t, l)) {
        trace_tag(TRACE_TAG_MOE)
        let nreg = moe_prefill_regions_lpt(t, s, l, nk, 0l, skip_gpu_experts)
        if (nreg > 0l) {
            rare_fetch_regions(t, s, l, 0l, nreg - 1l)
            expert_heat_add_regions(t, s, l, 0l, nreg - 1l)
            moe_prefill_rare_rows(t, s, l, 0l, nreg - 1l)
            moe_gpu_stream_ring_release()
            for (oe in s.moe_ord) {
                g_rare_dec_rows += int64(oe.y)
            }
        }
    } elif (fused) {
        trim_planar_guard(t, l, "the window's host sum")
        trace_tag(TRACE_TAG_MOE)
        let ts_gather = prof_ticks()
        moe_gather_rows(s, 0l, nk, dim, k, moe_kform, moe_kform)
        prof_add("moe_gather", ts_gather)
        let nreg = moe_prefill_regions_lpt(t, s, l, nk, 32l, skip_gpu_experts)
        if (nreg > 0l) {
            moe_prefill_fused_cpu(t, s, l, nreg, nk, prof_ticks())
            moe_prefill_park(s, nk, dim)
        }
    } else {
        trim_planar_guard(t, l, "the window's host sum") if (t.planes_trimmed)
        trace_tag(TRACE_TAG_MOE)
        moe_prefill_per_expert(t, s, l, moe_kform, skip_gpu_experts)
    }
    moe_routed_rows_reduce(s, npos, k, dim, skip_gpu_experts)
}

//! the window's routed rows requantized for the experts' GEMMs - Q8_K where the gate/up pair reads it, else Q8_0; answers which (`moe_kform`)
def moe_routed_rows_requant(t : Model; var s : Session; l, npos : int64) : bool {
    let dim = t.config.dim
    let moe_kform = moe_layer_kq(t, l) && moe_pair_kform(fmt_at(t.we1_fmt, l), fmt_at(t.we3_fmt, l))
    if (moe_kform) {
        s.kxqb |> resize(npos * dim)
        s.kxsb |> resize(npos * dim / 256l)
        s.kxbsb |> resize(npos * dim / 16l)
        requant_rows_q8k_bs(s.xb_b, dim, npos, s.kxqb, s.kxsb, s.kxbsb)
    } else {
        requant_rows_q8(s.xb_b, dim, npos, s.xqb, s.xsb)
    }
    return moe_kform
}

//! the rows' weighted sums of their parked slot rows into s.xb2_b, under s.moe_w_cpu with `skip_gpu_experts` (a driver's slots at zero, their stale rows skipped), else under s.moe_w_b
def moe_routed_rows_reduce(var s : Session; npos, k, dim : int64; skip_gpu_experts : bool) {
    let nk = npos * k
    trace_tag(TRACE_TAG_PARK)
    let ts_red = prof_ticks()
    unsafe {
        var ap = addr(s.xb2_b[0])
        let ep = addr<float const?>(s.moe_eout[0])
        let wp = skip_gpu_experts ? addr<float const?>(s.moe_w_cpu[0]) : addr<float const?>(s.moe_w_b[0])
        maybe_parallel_for(nk * dim >= g_act_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    moe_reduce_row(ap + pp * dim, ep + pp * k * dim, wp + pp * k, k, dim)
                }
            }
        }
    }
    prof_add("moe_reduce", ts_red)
}

//! `y` = one shared-expert plane over `npos` rows: the Q8_0-form image (`xq` / `xs`) or the Q8_K-form one (`s.kxqb` / `s.kxsb` /
//! `s.kxbsb`), whichever the plane's format reads
def private shexp_plane_rows(t : Model; var s : Session; var y : array<float>; p : tuple<f : KqFmt; off : int64>; xq : array<int8>; xs : array<float>; n, d, npos : int64) {
    if (fmt_reads_q8k(p.f)) {
        mm_b_f_pre(y, t, p.f, p.off, s.kxqb, s.kxsb, s.kxbsb, n, d, npos)
    } else {
        mm_b_f_pre(y, t, p.f, p.off, xq, xs, s.kxbsb, n, d, npos)
    }
}

//! the Q8_K-form image of `npos` rows of `x` (n wide) into `s.kxqb` / `s.kxsb` / `s.kxbsb`, where one of the planes reads it
def private shexp_q8k_rows(var s : Session; x : array<float>; n, npos : int64; want : bool) {
    if (want) {
        ensure_length(s.kxqb, npos * n)
        ensure_length(s.kxsb, npos * n / 256l)
        ensure_length(s.kxbsb, npos * n / 16l)
        requant_rows_q8k_bs(x, n, npos, s.kxqb, s.kxsb, s.kxbsb)
    }
}

//! layer `l`'s shared expert over every prefill row off its own planes - each in the file's format (`shexp_plane`) - into
//! `s.moe_gout`
[arch(at="../ARCHITECTURE_ENGINE_FORMATS.md#kq-token-table")]
def private shexp_prefill_planes(t : Model; var s : Session; l, npos : int64) {
    let c = t.config
    let dim = c.dim
    let nsh = c.n_ff_shexp
    let p1 = shexp_plane(t, l, 1)
    let p3 = shexp_plane(t, l, 3)
    let p2 = shexp_plane(t, l, 2)
    shexp_q8k_rows(s, s.xb_b, dim, npos, fmt_reads_q8k(p1.f) || fmt_reads_q8k(p3.f))
    shexp_plane_rows(t, s, s.moe_ghb, p1, s.xqb, s.xsb, dim, nsh, npos)
    shexp_plane_rows(t, s, s.moe_ghb2, p3, s.xqb, s.xsb, dim, nsh, npos)
    let ts_act2 = prof_ticks()
    gate_batch(s.moe_ghb, s.moe_ghb2, c.ffn_act, nsh, npos)
    prof_add("act", ts_act2)
    requant_rows_q8(s.moe_ghb, nsh, npos, s.moe_hq, s.moe_hs)
    shexp_q8k_rows(s, s.moe_ghb, nsh, npos, fmt_reads_q8k(p2.f))
    shexp_plane_rows(t, s, s.moe_gout, p2, s.moe_hq, s.moe_hs, nsh, dim, npos)
}

var private @scratch g_up_bq : array<int8>   //! the up stacks' own bucket-row image where a gate and up take two activation forms
var private @scratch g_up_bs : array<float>

//! the bucket rows' image off one form's quantized rows (`unit` elements a scale): bucket row r reads position bkt[r] / k
def private moe_gather_image(bkt : array<int64>; xq : array<int8>; xs : array<float>; nk, dim, k, unit : int64; var gq : array<int8>; var gs : array<float>) {
    let nb = dim / unit
    gq |> grow_resize(nk * dim)
    gs |> grow_resize(nk * nb)
    for (r in range64(nk)) {
        let p = bkt[r] / k
        unsafe {
            memcpy(addr(gq[r * dim]), addr(xq[p * dim]), dim)
            memcpy(addr(gs[r * nb]), addr(xs[p * nb]), nb * 4l)
        }
    }
}

//! the device combine's rows plus each position's routed down biases under its weights: the expert chain's down GEMM adds none
def private moe_prefill_down_bias(t : Model; var s : Session; l, npos : int64) {
    let c = t.config
    let k = c.n_expert_used
    let dim = c.dim
    for (p in range64(npos)) {
        for (j in range64(k)) {
            let e = l * c.n_expert + s.moe_idx_b[p * k + j]
            unsafe {
                axpy(addr(s.moe_eout[p * dim]), addr<float const?>(t.fblob[t.web2_off + e * dim]), s.moe_w_b[p * k + j], dim)
            }
        }
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split"), arch(at="../ARCHITECTURE_RUNTIME_JOBQUE.md#moe-region-split")]
def private ffn_moe_prefill_grouped(t : Model; var s : Session; l : int64; npos : int64) { // nolint:STYLE037,STYLE038 — the grouped MoE prefill is one bucketed pipeline over experts; each stage feeds the next through shared scratch
    let c = t.config
    let dim = c.dim
    let ne = c.n_expert
    let k = c.n_expert_used
    let nfe = c.n_ff_exp
    let nsh = c.n_ff_shexp
    let nk = npos * k
    // fused (native mx4-batch): gather/hidden/output images span every routed row at once, bucket-ordered
    let kq_layer = moe_layer_kq(t, l)
    let f1 = fmt_at(t.we1_fmt, l)
    let f2 = fmt_at(t.we2_fmt, l)
    let f3 = fmt_at(t.we3_fmt, l)
    // fused single-dispatch GEMMs: all stacks native-plane (sb-kq needs repack+tile; q51 always)
    let sb_fusable = t.kq_repacked && kernel_backend_has_kq_batch_groupn()
    let fused_kq = (kq_layer && g_moe_fused_expert_gemms
        && (kq_sb(f1) ? sb_fusable : f1 == KqFmt.q51)
        && (kq_sb(f2) ? sb_fusable : f2 == KqFmt.q51)
        && (kq_sb(f3) ? sb_fusable : f3 == KqFmt.q51))
    // GPU MoE tier: offloaded layers run the whole expert FFN chain device-side; tiny batches stay CPU (GPU submit cost > the GEMM)
    let ef1 = expert_fmt(t, t.we1_fmt, l)
    let ef3 = expert_fmt(t, t.we3_fmt, l)
    let ef2 = expert_fmt(t, t.we2_fmt, l)
    //! gpt-oss's routed form (the clamped swiglu, the per-expert biases) rides the Q8-fed chain with the device combine
    let routed_plain = !c.moe_exps_bias && c.ffn_act != FfnAct.swiglu_oai
    let fused_gpu = ((routed_plain || gpu_combine_on())
        && (moe_gpu_layer_on_gpu(l) || moe_gpu_layer_streamed(l)) && g_moe_fused_expert_gemms && npos >= 32l && gpu_prefill_route_on())
    // device-side combine rides fused_gpu; DASLLAMA_GPU_COMBINE=0 falls back to park + CPU reduce
    let gpu_comb = fused_gpu && gpu_combine_on()
    let gpu_xf = gpu_comb && routed_plain && moe_gpu_ffn_xf_ok(npos, dim, int(ef1), int(ef3), int(ef2))
    var split_gpu_regions = 0l
    var split_meet = false   //! the meet ran this layer: the host's rows in s.moe_eout, the device's in s.moe_gpu_out
    let fused = (t.experts_mx4 && g_active_mx4_native_batch && g_moe_fused_expert_gemms) || fused_kq || fused_gpu
    moe_prefill_scratch(t, s, npos, fused ? nk : npos, kq_layer)

    // 1. route every position: one batched router GEMM, then the shared select per row; shexp gate dots ride along
    trace_tag(TRACE_TAG_ROUTE)
    let ts_route = prof_ticks()
    mm_fblob_rows(s.moe_rlog, t, t.router_off + l * ne * dim, s.xb_b, dim, ne, npos)
    prof_add("moe_router", ts_route)
    let ts_select = prof_ticks()
    // per-row bias+select+gate dot, threaded: serial loop was ~2-3ms every-lane-idle/layer here
    unsafe {
        var rlp = addr(s.moe_rlog[0])
        var idxp = addr(s.moe_idx_b[0])
        var wp = addr(s.moe_w_b[0])
        var glp = nsh > 0l && c.moe_shexp_gated ? addr(s.moe_gl_b[0]) : null
        let xbp = addr<float const?>(s.xb_b[0])
        let rbp = c.moe_router_bias ? addr<float const?>(t.fblob[t.router_b_off + l * ne]) : null
        let ebp = c.moe_exp_probs ? addr<float const?>(t.fblob[t.exp_probs_off + l * ne]) : null
        let gvp = nsh > 0l && c.moe_shexp_gated ? addr<float const?>(t.fblob[t.shexp_gate_off + l * dim]) : null
        let gate = c.moe_gate
        let norm_topk = c.norm_topk_prob
        let wscale = c.expert_weights_scale
        let tp = moe_pick_tape_rows_begin(npos, k)
        maybe_parallel_for(npos > 1l, 0, int(npos), get_dispatch_lanes()) $(pb, pe) {
            unsafe {
                for (pp in range(pb, pe)) {
                    let p = int64(pp)
                    var lgp = rlp + p * ne
                    if (rbp != null) {
                        for (i in range64(ne)) {
                            lgp[i] += rbp[i]
                        }
                    }
                    if (tp != null) {
                        moe_pick_tape_mask_row(lgp, ne, k, tp + p * k)
                    }
                    moe_select_core(gate, ne, k, norm_topk, wscale, lgp, idxp, wp, p * k, ebp)
                    if (glp != null) {
                        glp[p] = sigmoid_f32(dot(gvp, xbp + p * dim, dim))
                    }
                }
            }
        }
    }
    moe_pick_tape_rows_done(s.moe_idx_b, npos, k)
    moe_prefill_bucket(s, nk, ne)
    prof_add("moe_route", ts_select)

    // 2. quantize every normed activation row ONCE; kq v2 gate/up gathers off Q8_K, q8 stacks off Q8_0
    let mixed_gpu = kq_layer && kq_sb(f1) != kq_sb(f3) && fused_gpu   //! the f16-fed chain reads no image; the Q8-fed one two
    let two_forms = mixed_gpu && !gpu_xf   //! a gate and up of two forms ride the device chain alone, each stack fed its own image (`moe_gpu_image2`)
    let moe_kform = mixed_gpu ? kq_sb(f1) : kq_layer && moe_pair_kform(f1, f3)
    if (gpu_xf) {
        if (nsh > 0l) {   // the shared expert's GEMM reads the Q8_0 image whatever the routed feed is
            requant_rows_q8(s.xb_b, dim, npos, s.xqb, s.xsb)
        }
    } elif (two_forms) {
        s.kxqb |> resize(npos * dim)
        s.kxsb |> resize(npos * dim / 256l)
        s.kxbsb |> resize(npos * dim / 16l)
        requant_rows_q8k_bs(s.xb_b, dim, npos, s.kxqb, s.kxsb, s.kxbsb)
        requant_rows_q8(s.xb_b, dim, npos, s.xqb, s.xsb)
    } elif (moe_kform) {
        s.kxqb |> resize(npos * dim)
        s.kxsb |> resize(npos * dim / 256l)
        s.kxbsb |> resize(npos * dim / 16l)
        requant_rows_q8k_bs(s.xb_b, dim, npos, s.kxqb, s.kxsb, s.kxbsb)
        if (nsh > 0l && !c.moe_dense_shexp) {
            // the q8 shexp batch (step 4) reads the Q8_0 image — without this it silently corrupts
            requant_rows_q8(s.xb_b, dim, npos, s.xqb, s.xsb)
        }
    } else {
        requant_rows_q8(s.xb_b, dim, npos, s.xqb, s.xsb)
    }

    // 2b. the shared expert on the device
    let sh_gpu = (nsh > 0l && !c.moe_dense_shexp && moe_gpu_shexp_on_gpu(l) && npos >= 32l && gpu_prefill_route_on())
    if (nsh > 0l && !sh_gpu) {
        moe_gpu_shexp_declined(shexp_gpu_decline_reason(c, l, npos))
    }
    if (sh_gpu) {
        trace_tag(TRACE_TAG_SHEXP)
        let ts_shg = prof_ticks()
        let p = moe_gpu_shexp_planes(l)   //! the planes the tier holds: the file's own, the gate and up on one activation class
        s.moe_shout |> grow_resize(npos * dim)
        s.moe_offs1 |> clear()
        s.moe_offs1 |> push(p.o1)   // nolint:STYLE012 — one region into reused scratch
        s.moe_offs1 |> push(0l)
        s.moe_offs1 |> push(npos)
        s.moe_offs3 |> clear()
        s.moe_offs3 |> push(p.o3)   // nolint:STYLE012 — one region into reused scratch
        s.moe_offs3 |> push(0l)
        s.moe_offs3 |> push(npos)
        s.moe_offs2 |> clear()
        s.moe_offs2 |> push(p.o2)   // nolint:STYLE012 — one region into reused scratch
        s.moe_offs2 |> push(0l)
        s.moe_offs2 |> push(npos)
        if (gpu_combine_on() && moe_gpu_ffn_xf_ok(npos, dim, p.f1, p.f3, p.f2)) {
            s.moe_sh_inv |> resize(npos)
            s.moe_sh_w |> resize(npos)
            for (r in range64(npos)) {
                s.moe_sh_inv[r] = uint(r)
                s.moe_sh_w[r] = 1.0
            }
            matmul_moe_gpu_ffn_combined_xf(s.moe_shout, s.moe_offs1, s.moe_offs3, s.moe_offs2, 1l,
                s.xb_b, s.moe_sh_w, s.moe_sh_inv, dim, nsh, npos, npos, p.f1, p.f3, p.f2, c.ffn_act == FfnAct.gelu)
        } elif (shexp_gpu_mixed_forms(l)) {   //! a gate and up of two activation forms: each GEMM fed its own image
            shexp_gpu_gemms(t, l, npos, s.xb_b, s.moe_shout)
        } elif (kq_sb(p.f1)) {   //! a superblock gate and up read the rows' Q8_K image, made here where the routed feed made none
            if (gpu_xf || !moe_kform) {
                s.kxqb |> resize(npos * dim)
                s.kxsb |> resize(npos * dim / 256l)
                s.kxbsb |> resize(npos * dim / 16l)
                requant_rows_q8k_bs(s.xb_b, dim, npos, s.kxqb, s.kxsb, s.kxbsb)
            }
            matmul_moe_gpu_ffn(s.moe_shout, s.moe_offs1, s.moe_offs3, s.moe_offs2, 1l,
                s.kxqb, s.kxsb, dim, nsh, npos, p.f1, p.f3, p.f2, c.ffn_act == FfnAct.gelu)
        } else {
            matmul_moe_gpu_ffn(s.moe_shout, s.moe_offs1, s.moe_offs3, s.moe_offs2, 1l,
                s.xqb, s.xsb, dim, nsh, npos, p.f1, p.f3, p.f2, c.ffn_act == FfnAct.gelu)
        }
        prof_add("shexp_gpu", ts_shg)
        g_shexp_gpu_layers++
    }

    // 3F. fused: gate/up/down each run as ONE region-list dispatch (3 fork/joins/layer, not 3/expert)
    if (fused) {
        trace_tag(TRACE_TAG_MOE)
        let ts_gather = prof_ticks()
        if (!gpu_xf) {
            moe_gather_rows(s, 0l, nk, dim, k, moe_kform && !fused_gpu, moe_kform)
        }
        if (two_forms) {   //! the up stacks' own image: the bucket rows off the other form's quantized rows
            if (moe_kform) {
                moe_gather_image(s.moe_bkt, s.xqb, s.xsb, nk, dim, k, 32l, g_up_bq, g_up_bs)
            } else {
                moe_gather_image(s.moe_bkt, s.kxqb, s.kxsb, nk, dim, k, 256l, g_up_bq, g_up_bs)
            }
            set_moe_gpu_image2(g_up_bq, g_up_bs)
        }
        prof_add("moe_gather", ts_gather)
        let split_cap = !gpu_comb && !fused_gpu ? 32l : 0l
        let nreg = moe_prefill_regions_lpt(t, s, l, nk, split_cap)
        let split_ok = (gpu_xf && moe_gpu_layer_streamed(l) && gpu_want_moe_split() && sb_fusable
            && moe_gpu_ffn_xf_ok(npos, dim, int(f1), int(f3), int(f2)) && nreg > 0l && int64(s.moe_ord[0].y) <= moe_gpu_ffn_xf_async_rows(dim, nfe))
        if (split_ok && gpu_comb) {
            s.moe_inv |> resize(nk)
            for (j in range64(nk)) {
                s.moe_inv[s.moe_bkt[j]] = uint(j)
            }
            let ts_meet = prof_ticks()
            let g = moe_prefill_meet(t, s, l, npos, nreg, int(f1), int(f3), int(f2))
            if (g >= 0l) {
                split_meet = true
                split_gpu_regions = g
                prof_add("moe_meet", ts_meet)
                let ts_join = prof_ticks()
                s.moe_gpu_out |> grow_resize(npos * dim)
                matmul_moe_gpu_ffn_combined_xf_join(s.moe_gpu_out)
                prof_add("split_wait", ts_join)
            }
        }
        if (split_ok && !split_meet) {
            let bytes_e = float(nfe * dim) * (kq_bytes_per_weight(f1) + kq_bytes_per_weight(f3) + kq_bytes_per_weight(f2))
            var rows_gpu = 0l
            var rows_cpu = nk
            for (oe in s.moe_ord) {
                let cnt = int64(oe.y)
                let t_gpu = float(split_gpu_regions + 1l) * bytes_e / SPLIT_BUS_BPS + float(rows_gpu + cnt) * SPLIT_GPU_ROW_S
                let t_cpu = float(rows_cpu - cnt) * SPLIT_CPU_ROW_S
                if (t_gpu > t_cpu) {
                    break
                }
                split_gpu_regions++
                rows_gpu += cnt
                rows_cpu -= cnt
            }
            if (split_gpu_regions >= nreg) {
                split_gpu_regions = 0l
            }
        }
        let ts_g1 = prof_ticks()
        if (split_meet) {
        } elif (gpu_comb) {   // GPU tier: gate+up/act/requant/down AND the routed weighted-sum combine
            // gout comes back as npos combined rows; the 33MB-class bucket-row readback + park scatter disappear
            s.moe_inv |> resize(nk)
            for (j in range64(nk)) {
                s.moe_inv[s.moe_bkt[j]] = uint(j)
            }
            if (split_gpu_regions > 0l) {
                s.moe_exp_gpu |> resize(ne)
                for (e in range64(ne)) {
                    s.moe_exp_gpu[e] = int8(0)
                }
                for (r in range64(split_gpu_regions)) {
                    s.moe_exp_gpu[s.moe_ord[r].x] = int8(1)
                }
                s.moe_w_gpu |> resize(nk)
                s.moe_w_cpu |> resize(nk)
                for (j in range64(nk)) {
                    let on_gpu = s.moe_exp_gpu[s.moe_idx_b[j]] != int8(0)
                    s.moe_w_gpu[j] = on_gpu ? s.moe_w_b[j] : 0.0
                    s.moe_w_cpu[j] = on_gpu ? 0.0 : s.moe_w_b[j]
                }
                let nreg_c = nreg - split_gpu_regions
                s.moe_offs1_cpu |> resize(nreg_c * 3l)
                s.moe_offs3_cpu |> resize(nreg_c * 3l)
                s.moe_offs2_cpu |> resize(nreg_c * 3l)
                for (i in range64(nreg_c * 3l)) {
                    s.moe_offs1_cpu[i] = s.moe_offs1[split_gpu_regions * 3l + i]
                    s.moe_offs3_cpu[i] = s.moe_offs3[split_gpu_regions * 3l + i]
                    s.moe_offs2_cpu[i] = s.moe_offs2[split_gpu_regions * 3l + i]
                }
                matmul_moe_gpu_ffn_combined_xf_begin(s.moe_offs1, s.moe_offs3, s.moe_offs2, split_gpu_regions,
                    s.xb_b, s.moe_w_gpu, s.moe_inv, dim, nfe, nk, npos, int(f1), int(f3), int(f2), c.ffn_act == FfnAct.gelu)
                prof_add("moe_gpu", ts_g1)
                let ts_c = prof_ticks()
                s.kxqb |> resize(npos * dim)
                s.kxsb |> resize(npos * dim / 256l)
                s.kxbsb |> resize(npos * dim / 16l)
                requant_rows_q8k_bs(s.xb_b, dim, npos, s.kxqb, s.kxsb, s.kxbsb)
                moe_gather_rows(s, 0l, nk, dim, k, true, true)
                mm_b_kq_groupn(s.moe_ghb, t, f1, s.moe_offs1_cpu, nreg_c, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe)
                mm_b_kq_groupn(s.moe_ghb2, t, f3, s.moe_offs3_cpu, nreg_c, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe)
                gate_batch(s.moe_ghb, s.moe_ghb2, c.ffn_act, nfe, nk)
                requant_rows_q8k_bs(s.moe_ghb, nfe, nk, s.moe_hq, s.moe_hs, s.moe_hbs)
                mm_b_kq_groupn(s.moe_gout, t, f2, s.moe_offs2_cpu, nreg_c, s.moe_hq, s.moe_hs, s.moe_hbs, nfe, dim)
                unsafe {
                    let gp = addr<float const?>(s.moe_gout[0])
                    var ep = addr(s.moe_eout[0])
                    for (r in range64(nreg_c)) {
                        let b0 = s.moe_offs1_cpu[r * 3l + 1l]
                        let b1 = b0 + s.moe_offs1_cpu[r * 3l + 2l]
                        for (b in range64(b0, b1)) {
                            copy_floats(ep + s.moe_bkt[b] * dim, gp + b * dim, dim)
                        }
                    }
                }
                prof_add("mm_gemm", ts_c)
                let ts_join = prof_ticks()
                s.moe_gpu_out |> grow_resize(npos * dim)
                matmul_moe_gpu_ffn_combined_xf_join(s.moe_gpu_out)
                prof_add("split_wait", ts_join)   // GPU residue after the CPU tail - 0 = fully hidden
            } elif (gpu_xf) {
                matmul_moe_gpu_ffn_combined_xf(s.moe_eout, s.moe_offs1, s.moe_offs3, s.moe_offs2, nreg,
                    s.xb_b, s.moe_w_b, s.moe_inv, dim, nfe, nk, npos, int(ef1), int(ef3), int(ef2), c.ffn_act == FfnAct.gelu)
                prof_add("moe_gpu", ts_g1)
            } else {
                if (!routed_plain) {
                    set_moe_gpu_ffn_form(moe_routed_form(t, l))
                }
                matmul_moe_gpu_ffn_combined(s.moe_eout, s.moe_offs1, s.moe_offs3, s.moe_offs2, nreg,
                    s.moe_gxq, s.moe_gxs, s.moe_w_b, s.moe_inv, dim, nfe, nk, npos, int(ef1), int(ef3), int(ef2), c.ffn_act == FfnAct.gelu)
                if (!routed_plain) {
                    clear_moe_gpu_ffn_form()
                }
                if (c.moe_exps_bias) {
                    moe_prefill_down_bias(t, s, l, npos)
                }
                prof_add("moe_gpu", ts_g1)
            }
        } elif (fused_gpu) {   // combine off: per-row gout + the park/CPU-reduce path below
            matmul_moe_gpu_ffn(s.moe_gout, s.moe_offs1, s.moe_offs3, s.moe_offs2, nreg,
                s.moe_gxq, s.moe_gxs, dim, nfe, nk, int(ef1), int(ef3), int(ef2), c.ffn_act == FfnAct.gelu)
            prof_add("moe_gpu", ts_g1)
        } else {
            moe_prefill_fused_cpu(t, s, l, nreg, nk, ts_g1)
        }
        if (two_forms) {
            clear_moe_gpu_image2()
        }
        // threaded bucket scatter: serial was an ~8ms ALL-LANE hole here (~67MB traffic at pp512/30B)
        if (!gpu_comb) {
            moe_prefill_park(s, nk, dim)
        }
    }

    // 3. one batched GEMM chain per touched expert; raw down rows park for the ordered reduce (skipped when fused ran)
    if (!fused) {
        trace_tag(TRACE_TAG_MOE)
        moe_prefill_per_expert(t, s, l, moe_kform)
    }

    // 4. shared expert for ALL positions — a plain dense batched FFN off the whole-batch image
    if (nsh > 0l && !sh_gpu && shexp_tagged(t, l)) {
        trace_tag(TRACE_TAG_SHEXP)
        shexp_prefill_planes(t, s, l, npos)
    } elif (nsh > 0l && !sh_gpu) {
        trace_tag(TRACE_TAG_SHEXP)
        let she = dim * nsh
        mm_b_q8_pre(s.moe_ghb, t, t.wsh1_off + l * she, s.xqb, s.xsb, dim, nsh, npos)
        mm_b_q8_pre(s.moe_ghb2, t, t.wsh3_off + l * she, s.xqb, s.xsb, dim, nsh, npos)
        let ts_act2 = prof_ticks()
        gate_batch(s.moe_ghb, s.moe_ghb2, c.ffn_act, nsh, npos)
        prof_add("act", ts_act2)
        requant_rows_q8(s.moe_ghb, nsh, npos, s.moe_hq, s.moe_hs)
        mm_b_q8_pre(s.moe_gout, t, t.wsh2_off + l * she, s.moe_hq, s.moe_hs, nsh, dim, npos)
    }

    // 5. ordered reduce into xb2_b, decode's accumulation order (bit-identical); plain mul+add, not axpy
    trace_tag(TRACE_TAG_PARK)
    let ts_red = prof_ticks()
    unsafe {
        var ap = addr(s.xb2_b[0])
        let ep = addr<float const?>(s.moe_eout[0])
        let split_on = split_gpu_regions > 0l || split_meet
        let wp = split_on ? addr<float const?>(s.moe_w_cpu[0]) : addr<float const?>(s.moe_w_b[0])
        let split_gpu_rows = split_on ? addr<float const?>(s.moe_gpu_out[0]) : null
        let sp = nsh > 0l ? (sh_gpu ? addr<float const?>(s.moe_shout[0]) : addr<float const?>(s.moe_gout[0])) : null
        let gp = nsh > 0l && c.moe_shexp_gated ? addr<float const?>(s.moe_gl_b[0]) : null
        maybe_parallel_for(nk * dim >= g_act_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    var arow = ap + pp * dim
                    if (split_gpu_rows != null) {
                        let cr = split_gpu_rows + pp * dim
                        for (i in range64(dim)) {
                            arow[i] = cr[i]
                        }
                        for (j in range64(k)) {
                            let w = wp[pp * k + j]
                            if (w != 0.0) {
                                let er = ep + (pp * k + j) * dim
                                for (i in range64(dim)) {
                                    arow[i] += w * er[i]
                                }
                            }
                        }
                    } elif (gpu_comb) {
                        let cr = ep + pp * dim
                        for (i in range64(dim)) {
                            arow[i] = cr[i]
                        }
                    } else {
                        moe_reduce_row(arow, ep + pp * k * dim, wp + pp * k, k, dim)
                    }
                    if (sp != null) {
                        let g = gp != null ? gp[pp] : 1.0   // ungated shexp (glm4moe) adds at weight 1
                        let sr = sp + pp * dim
                        for (i in range64(dim)) {
                            arow[i] += g * sr[i]
                        }
                    }
                }
            }
        }
    }
    prof_add("moe_reduce", ts_red)
    // post-norm + residual (the layer-output scale tail is shared with the reference path)
    let ts_resid = prof_ticks()
    if (c.pre_post_norm) {
        rms_batch(s.xb2_b, s.xb2_b, t, t.rms_post_ffn_off + l * dim, dim, npos)
    }
    if (c.hyper_conn) {
        block_resid_b(t, s, s.xb2_b, npos)
    } else {
        add_batch(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
    }
    prof_add("act_resid", ts_resid)
}

// MoE FFN, prefill: norm, then either the grouped expert-bucketed path (Q8, default) or the
// per-position reference path through the decode scratch (fp32 models — q4_0 never prefills — and
// the bit-identical A/B switch).
[no_alloc, no_env, no_io]
def ffn_moe_prefill(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    if (l < c.n_layer_dense_lead) {   // glm4moe: the leading layer(s) are plain dense FFN
        ffn_dense_prefill(t, s, l, npos)
        return
    }
    let dim = c.dim
    let ts_norm = prof_ticks()
    block_in_b(t, s, t.rms_ffn_off + l * dim, npos)
    prof_add("norm", ts_norm)
    // native-MXFP4 grouped path pays a per-touched-expert Q8 expansion (~120MB/expert); under ~8 rows/expert the per-position mx4 GEMVs are cheaper
    let mx4_grouped_pays = !t.experts_mx4 || npos * c.n_expert_used >= 8l * c.n_expert
    // kq-expert layers ride grouped buckets via the kq batch tile; portable/no-kq backends take the per-position path
    let kq_grouped_ok = !moe_layer_sb_kq(t, l) || (t.kq_repacked && kernel_backend_has_kq_batch())
    // a speculative verify's rows are decode-shaped: the bucketed GEMM chain over a handful of rows costs several
    // GEMV rounds of the same experts, so narrow batches take the per-position GEMV route (bit-identical)
    let narrow = npos <= ATTN_NARROW_NPOS
    if (t.quant == QuantMode.q8 && g_moe_grouped_prefill && mx4_grouped_pays && kq_grouped_ok && !narrow) {
        ffn_moe_prefill_grouped(t, s, l, npos)
    } else {
        // per-position reference path — moe_ffn_core carries its own section buckets
        for (p in range64(npos)) {
            copy_floats(s.xb_b, p * dim, s.xb, 0l, dim)
            moe_ffn_core(t, s, l)
            if (c.pre_post_norm) {
                rms_at(s.moe_acc, t, t.rms_post_ffn_off + l * dim, s.moe_acc, dim)
            }
            block_resid_row(t, s, s.moe_acc, p)   // residual add into this position's row
        }
    }
    if (c.layer_out_scale) {
        let os = t.fblob[t.out_scale_off + l]
        scale_inplace(unsafe(addr(s.x_b[0])), os, npos * dim)
    }
}

[arch(at="../ARCHITECTURE_GPU.md#gpu-backends"), arch(at="../ARCHITECTURE_RUNTIME_JOBQUE.md#hybrid-pool-policy")]
def forward_prefill_embd(t : Model; var s : Session; embd : array<float>; npos, start_pos : int64;
                         non_causal : bool = false; src_off : int64 = 0l; skip_logits : bool = false;
                         span_lo : int64 = -1l; span_hi : int64 = -1l; ds_wide : bool = false) {
    if (npos <= 0l) {
        return
    }
    // span_lo/span_hi (chunk-relative): non-causal rows [span_lo, span_hi) inside an otherwise
    // causal call — the fused image turn. non_causal = the whole-call span (span bounds ignored).
    let spanned = non_causal || span_lo >= 0l
    if (spanned && t.config.recr_mask != 0ul) {
        panic("forward_prefill_embd: a non-causal span cannot apply to recurrent layers (recr_mask != 0) - the recurrence has no uniform-bound form")
    }
    if (span_lo >= 0l && (span_hi <= span_lo || span_hi > npos)) {
        panic("forward_prefill_embd: span [{span_lo},{span_hi}) outside [0,{npos})")
    }
    if (npos >= 32l) {
        dispatch_phase_batch()
    }
    // ds_wide (qwen3vl deepstack): every row is (1+n_deepstack)·dim wide — slice 0 is the
    // residual input, the tail slices stash for the per-layer adds (text rows carry zeros)
    let n_ds = ds_wide ? t.config.n_deepstack : 0l
    if (ds_wide && n_ds <= 0l) {
        panic("forward_prefill_embd: ds_wide rows on a model with no deepstack layers")
    }
    let stride = (1l + n_ds) * t.config.dim
    if (long_length(embd) < (src_off + npos) * stride) {
        panic("forward_prefill_embd: embd has {long_length(embd)} floats - rows [{src_off},{src_off + npos}) of width {stride} need {(src_off + npos) * stride}")
    }
    kv_ensure(s, start_pos + npos)   // paged sessions: grow the block table BEFORE any cache-pointer hoist
    forward_prefill_alloc(t, s, npos, start_pos)
    let dim = t.config.dim
    let ts_embed = prof_ticks()
    if (ds_wide) {
        s.wide_src = unsafe(addr < float const? >(embd[0]))
        s.wide_stride = stride
        s.wide_soff = src_off
        s.ds_active = true
    } else {
        let base = src_off * dim
        for (i in range64(npos * dim)) {
            s.x_b[i] = embd[base + i]
        }
    }
    prof_add("embed", ts_embed)
    if (has_ple(t.config)) {  // gemma-4 E-series: PLE side input (padding-token gather for the media path)
        let ts_ple = prof_ticks()
        if (long_length(s.embd_ids) >= src_off + npos) {   // a media body: its text rows take their own tokens' side input
            ple_pre_prefill_gated(t, s, s.embd_ids, src_off, npos, start_pos)
        } else {
            ple_pre_prefill_pad_gated(t, s, npos, start_pos)
        }
        prof_add("ple", ts_ple)
    }
    // the non-causal image span (gemma vision): span queries attend
    // [the sliding-window floor, attn_uniform_end), the rest stay causal — set for exactly this body
    if (non_causal) {
        s.attn_uniform_lo = start_pos
        s.attn_uniform_end = start_pos + npos
    } elif (span_lo >= 0l) {
        s.attn_uniform_lo = start_pos + span_lo
        s.attn_uniform_end = start_pos + span_hi
    } else {
        s.attn_uniform_lo = 0l
        s.attn_uniform_end = 0l
    }
    s.embed_gpu_pending = false   // this entry filled x_b itself - no stash for the body to backfill
    forward_prefill_body(t, s, npos, start_pos, skip_logits)
    s.attn_uniform_lo = 0l
    s.attn_uniform_end = 0l
    s.ds_active = false
    s.wide_src = null   // the borrow dies with the call
}

//! ``embed_text_rows`` at a wider row stride (deepstack quanta): each token's dim-wide
//! embedding lands at ``dst[(at+p)·row_w]``; the row tails stay as-is (zero for text rows).
def embed_text_rows_strided(t : Model; tokens : array<int64>; var dst : array<float>; at, row_w : int64) {
    let dim = t.config.dim
    for (p in range64(long_length(tokens))) {
        unsafe {
            embed_row(t, tokens[p], addr(dst[(at + p) * row_w]))
            if (t.config.embed_scale != 1.0) {
                scale_inplace(addr(dst[(at + p) * row_w]), t.config.embed_scale, dim)
            }
        }
    }
}

//! One prefill body's rows at the decoder's row stride: ``head`` text, ``n_media`` media rows of width ``src_w``,
//! ``tail`` text. A deepstack decoder's stride is (1+n)·dim - a text row and a dim-wide media row keep a zero tail.
//! On a gemma-4 E-series decoder the session takes each row's token id for ``eval_embd_body_``, which spends them.
def media_body_rows_(t : Model; var s : Session; head : array<int64>; rows : array<float>; n_media, src_w : int64;
                     tail : array<int64>; var embd : array<float>) {
    let rw = t.config.dim * (1l + t.config.n_deepstack)
    let n_head = long_length(head)
    let npos = n_head + n_media + long_length(tail)
    embd |> clear()
    embd |> ensure_capacity(npos * rw)
    embd |> resize(npos * rw)
    embed_text_rows_strided(t, head, embd, 0l, rw)
    for (r in range64(n_media)) {
        unsafe {
            memcpy(addr(embd[(n_head + r) * rw]), addr(rows[r * src_w]), src_w * 4l)
        }
    }
    embed_text_rows_strided(t, tail, embd, n_head + n_media, rw)
    s.embd_ids |> clear()
    if (has_ple(t.config)) {
        s.embd_ids |> resize(npos)
        for (p in range64(n_head)) {
            s.embd_ids[p] = head[p]
        }
        for (p in range64(long_length(tail))) {
            s.embd_ids[n_head + n_media + p] = tail[p]
        }
    }
}

//! Prefill ``npos`` rows whose [span_lo, span_hi) are a NON-CAUSAL span (span_hi <= span_lo = every row causal);
//! a non-zero merged ``grid`` ropes the span on the image-grid walk.
def eval_embd_body_(t : Model; var s : Session; embd : array<float>; npos, span_lo, span_hi : int64; grid : int2) {
    if (span_hi > span_lo && grid.x > 0) {
        eval_embd_span_mrope_(t, s, embd, npos, span_lo, span_hi, int64(grid.x), int64(grid.y))
    } elif (span_hi > span_lo) {
        eval_embd_span_(t, s, embd, npos, span_lo, span_hi)
    } else {
        eval_embd_(t, s, embd, npos)
    }
    s.embd_ids |> clear()
}

//! ``eval_``'s embedding-input twin: prefill ``npos`` embedding rows at the session cursor and
//! advance it. Leaves ``s.logits`` = the last position's logits. ``non_causal`` marks the rows
//! as a non-causal image span (gemma vision) — see forward_prefill_embd.
def eval_embd_(t : Model; var s : Session; embd : array<float>; npos : int64; non_causal : bool = false) {
    if (npos <= 0l) {
        return
    }
    kv_capacity_check(t, npos, s.n_past)
    forward_prefill_embd(t, s, embd, npos, s.n_past, non_causal, ds_wide = embd_is_ds_wide(t, embd, npos))
    s.n_past += npos
}

var private g_span_fuse = true

[init]
def private span_fuse_env() {
    g_span_fuse = g_env_vision.span_fuse
    if (!g_span_fuse) {
        to_log(LOG_WARNING, "dasLLAMA: DASLLAMA_SPAN_FUSE=0 - image turns run the three-eval splice\n")
    }
}

//! A/B seat: pin the pre-fuse three-eval splice (head / rows / tail as separate prefill calls) —
//! the parity rail for the fused span eval (DASLLAMA_SPAN_FUSE's setter twin). Default fused.
def public set_span_fuse(on : bool) {
    g_span_fuse = on
}

// An override that cannot serve the per-query span mask registers the split-span capability
// (register_prefill_override_split_span) and keeps the three-eval splice; Metal serves the
// fused mask (AttnArgs.ulo), and the CPU arms mask per query.
def private span_fusable : bool => g_span_fuse && !active_prefill_splits_span()

//! `eval_embd_` for a prompt carrying ONE non-causal image span: rows [span_lo, span_hi) prefill
//! non-causally, the text around them causally. One fused eval (per-query mask) where the backend
//! serves it; the three-eval splice is the fallback (`vision_plan.md` §pipeline).
def eval_embd_span_(t : Model; var s : Session; embd : array<float>; npos, span_lo, span_hi : int64) {
    if (npos <= 0l) {
        return
    }
    if (span_lo < 0l || span_hi > npos || span_hi <= span_lo) {
        panic("eval_embd_span_: span [{span_lo},{span_hi}) outside [0,{npos})")
    }
    kv_capacity_check(t, npos, s.n_past)
    if (span_fusable()) {
        forward_prefill_embd(t, s, embd, npos, s.n_past, span_lo = span_lo, span_hi = span_hi,
                             ds_wide = embd_is_ds_wide(t, embd, npos))
        s.n_past += npos
        return
    }
    // slices read embd in place via src_off; only the last non-empty slice produces logits
    let wide = embd_is_ds_wide(t, embd, npos)
    eval_embd_slice_(t, s, embd, 0l, span_lo, false, false, wide)
    eval_embd_slice_(t, s, embd, span_lo, span_hi, true, span_hi >= npos, wide)
    eval_embd_slice_(t, s, embd, span_hi, npos, false, true, wide)
}

// exact-length wide detection: a deepstack model's image quantum carries (1+n_ds)·dim-wide rows
// (text rows zero-tailed); every other embd buffer is exactly npos·dim
def private embd_is_ds_wide(t : Model; embd : array<float>; npos : int64) : bool {
    return (t.config.n_deepstack > 0l
        && long_length(embd) == npos * (1l + t.config.n_deepstack) * t.config.dim)
}

def private eval_embd_slice_(t : Model; var s : Session; embd : array<float>; lo, hi : int64;
                             non_causal, last : bool; ds_wide : bool = false) {
    let n = hi - lo
    if (n <= 0l) {
        return
    }
    forward_prefill_embd(t, s, embd, n, s.n_past, non_causal, src_off = lo, skip_logits = !last, ds_wide = ds_wide)
    s.n_past += n
}

//! `eval_embd_span_`'s qwen mrope twin: the span rows are an nx×ny image grid roping on the
//! image-grid walk (t = pos0, per-row h/w, position advance max(nx, ny)); surrounding text
//! ropes sequentially at delta'd positions. Same uniform-span mask; only the angles differ.
def eval_embd_span_mrope_(t : Model; var s : Session; embd : array<float>; npos, span_lo, span_hi, nx, ny : int64) {
    if (npos <= 0l) {
        return
    }
    if (span_lo < 0l || span_hi > npos || span_hi <= span_lo) {
        panic("eval_embd_span_mrope_: span [{span_lo},{span_hi}) outside [0,{npos})")
    }
    kv_capacity_check(t, npos, s.n_past)
    let sect = t.config.rope_sections
    if (sect.x + sect.y + sect.z + sect.w == 0) {
        panic("eval_embd_span_mrope_: the model carries no rope.dimension_sections - not an mrope arch")
    }
    let base = s.n_past + s.rope_pos_delta
    let wide = embd_is_ds_wide(t, embd, npos)
    if (span_fusable()) {
        mrope_span_positions(s.mrope_pos, base, npos, span_lo, span_hi, nx, ny)
        forward_prefill_embd(t, s, embd, npos, s.n_past, span_lo = span_lo, span_hi = span_hi, ds_wide = wide)
        s.n_past += npos
        s.mrope_pos |> clear()
    } else {
        var whole : array<int4>
        mrope_span_positions(whole, base, npos, span_lo, span_hi, nx, ny)
        eval_embd_slice_mrope_(t, s, whole, embd, 0l, span_lo, false, false, wide)
        eval_embd_slice_mrope_(t, s, whole, embd, span_lo, span_hi, true, span_hi >= npos, wide)
        eval_embd_slice_mrope_(t, s, whole, embd, span_hi, npos, false, true, wide)
        delete whole
    }
    s.rope_pos_delta += max(nx, ny) - (span_hi - span_lo)
}

// a splice slice carries its own view of the quantum map (positions are quantum-absolute)
def private eval_embd_slice_mrope_(t : Model; var s : Session; whole : array<int4>; embd : array<float>;
                                   lo, hi : int64; non_causal, last : bool; ds_wide : bool = false) {
    let n = hi - lo
    if (n <= 0l) {
        return
    }
    s.mrope_pos |> resize(n)
    for (i in range64(n)) {
        s.mrope_pos[i] = whole[lo + i]
    }
    forward_prefill_embd(t, s, embd, n, s.n_past, non_causal, src_off = lo, skip_logits = !last, ds_wide = ds_wide)
    s.n_past += n
    s.mrope_pos |> clear()
}

//! L2-normalized sentence embedding for `tokens` into `out`: the post-final-norm hidden rows `pooling` picks, averaged, cut to
//! the leading `dims` components where `dims` is 1..dim-1 (the Matryoshka read), unit-normalized once. Runs one forward over
//! `s` from position 0 - pass a FRESH session (no KV history assumed; cache is left filled).
[arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-last-row-tail")]
//! `pooling` picks the rows that make the vector: POOLING_MEAN averages every position's post-final-norm row, POOLING_LAST
//! takes the last position's (the EOS the tokenizer appends - what Qwen3-Embedding is trained to be read at), POOLING_CLS the first's
def embed_forward(t : Model; var s : Session; tokens : array<int64>; npos : int64; var out : array<float>; pooling : int64 = POOLING_MEAN; dims : int64 = 0l) {
    let dim = t.config.dim
    out |> resize(dim)
    for (i in range64(dim)) {
        out[i] = 0.0
    }
    if (npos <= 0l) {
        return
    }
    let first = embed_first_row(pooling, npos)
    let last = embed_end_row(pooling, npos)
    if (t.quant != QuantMode.q4_0) {
        // batched prefill - one row included: a decode override's step lands no hidden row - fills s.x_b (npos × dim residual)
        // and leaves it intact; the body norms only the last position, so re-apply the final RMSNorm to the pooled positions.
        s.keep_hidden = true
        forward_prefill(t, s, tokens, npos, 0l, true)
        embed_pool_rows(t, s, first, last, out)
    } else {
        // q4_0 (no batched prefill): per-token forward leaves s.xb = post-final-norm hidden.
        for (p in range64(npos)) {
            forward(t, s, tokens[p], p)
            if (p >= first && p < last) {
                add_inplace(unsafe(addr(out[0])), unsafe(addr(s.xb[0])), dim)
            }
        }
    }
    s.n_past = npos   // KV now holds npos positions (start_pos was 0) — keep the session cursor consistent
    embed_finish(out, last - first, dims, dim)
}

//! the first row `pooling` reads of an `npos`-row forward
def private embed_first_row(pooling, npos : int64) : int64 => pooling == POOLING_LAST ? npos - 1l : 0l

//! one past the last row `pooling` reads
def private embed_end_row(pooling, npos : int64) : int64 => pooling == POOLING_CLS ? 1l : npos

//! the post-final-norm rows `first` .. `last` of a keep_hidden forward's `s.x_b`, summed into `out`
def private embed_pool_rows(t : Model; var s : Session; first, last : int64; var out : array<float>) {
    let dim = t.config.dim
    for (p in range64(first, last)) {
        rms_at(s.xb, t, t.rms_final_off, s.x_b, p * dim, dim)
        add_inplace(unsafe(addr(out[0])), unsafe(addr(s.xb[0])), dim)
    }
}

//! the pooled rows' mean, the cut to the leading `dims`, then the L2 normalize: cutting before the normalize is the
//! renormalized cut
def private embed_finish(var out : array<float>; rows, dims, dim : int64) {
    let width = dims > 0l && dims < dim ? dims : dim
    out |> resize(width)
    let inv = 1.0 / float(rows)
    var ss = 0.0
    for (i in range64(width)) {
        out[i] *= inv
        ss += out[i] * out[i]
    }
    let rn = ss > 0.0 ? 1.0 / sqrt(ss) : 0.0
    for (i in range64(width)) {
        out[i] *= rn
    }
}

//! The calling context's embed sessions, reused: an embedding is one forward from position 0 that rewrites every row it
//! reads, and building a context-sized session costs more than the forward. Keyed by the model's identity and geometry;
//! a model whose session holds state past its rows (`session_keeps_rows_only` false) is never pooled.
var private g_embed_pool : array<Session>
var private g_embed_key = ""
var private g_embed_pool_builds = 0

var private g_embed_lone_forwards = 0

//! how many times this context built its embed session pool - the test's view of the rebuild on a model change
def embed_pool_builds() : int => g_embed_pool_builds

//! how many batch texts ran `embed_forward` alone, outside the shared window (a one-row text, every q4_0 text) - the test's view
def embed_lone_forwards() : int => g_embed_lone_forwards

//! how many of the pool's sessions are device-home - no host K/V - the test's view of the scratch window's sessions
def embed_pool_device_sessions() : int {
    var n = 0
    for (s in g_embed_pool) {
        n += s.device_kv ? 1 : 0
    }
    return n
}

//! the embed pool's key: the model's planes (`model_planes_key`), its geometry, the codec and the form
def embed_pool_key(m : Model; kvdt : KVDtype; device : bool) : string {
    let c = m.config
    return "{model_planes_key(m)}|{c.dim}|{c.hidden_dim}|{c.n_layers}|{c.n_heads}|{c.n_kv_heads}|{c.seq_len}|{c.vocab_size}|{kvdt}|{device}"
}

//! `n` of the context's pooled sessions for `m` in the K/V codec `kvdt`, each at position 0 - device-home ones (no host K/V,
//! `device`) where the armed driver seats the texts in its scratch region; the pool is rebuilt when the model, the codec, the
//! form or the count changes
def private embed_pool_take(m : Model; n : int64; kvdt : KVDtype; device : bool) {
    let key = embed_pool_key(m, kvdt, device)
    if (key != g_embed_key || long_length(g_embed_pool) < n) {
        delete g_embed_pool
        g_embed_pool |> reserve(n)   // the window's segments point into it: it never grows past this
        for (_ in range64(n)) {
            g_embed_pool |> emplace(device ? create_device_session_(m, kvdt) : create_session_(m, kvdt, kvdt))
        }
        g_embed_key = key
        g_embed_pool_builds++
    }
    for (k in range64(n)) {
        assume s = g_embed_pool[k]
        s.n_past = 0l
        s.keep_hidden = false
        sel_reset(s)
    }
}

//! L2-normalized sentence embedding of `text` (`dim` floats, or the leading `dims`): one forward, the decoder's last-layer
//! hidden rows pooled the way `pooling` asks, cut, unit-normalized; past the context its leading `seq_len` tokens. `kvdt` is the
//! sessions' K/V codec, `device` the armed driver's scratch window (`embed_batch_`'s).
def embed_(m : Model; text : string; pooling : int64 = POOLING_MEAN; dims : int64 = 0l; kvdt : KVDtype = KVDtype.f32; device : bool = false) : array<float> {
    if (device) {
        var inscope one <- embed_batch_(m, [text], pooling, dims, EMBED_WINDOW_ROWS, kvdt, true)
        return <- one[0]
    }
    let tokens = encode_(m, text)
    var npos = long_length(tokens)
    if (npos > m.config.seq_len) {
        to_log(LOG_WARNING, "embed: {npos} tokens past the model context of {m.config.seq_len}; the leading {m.config.seq_len} are embedded\n")
        npos = m.config.seq_len
    }
    var out : array<float>
    if (session_keeps_rows_only(m.config)) {
        embed_pool_take(m, 1l, kvdt, false)
        embed_forward(m, g_embed_pool[0], tokens, npos, out, pooling, dims)
    } else {
        var s <- create_session_(m, kvdt, kvdt)
        embed_forward(m, s, tokens, npos, out, pooling, dims)
        delete s
    }
    return <- out
}

//! the rows one embedding window takes: past it, the batch's next texts start a window of their own
let EMBED_WINDOW_ROWS = 4096l

//! `embed_` over many texts, each its own unit vector, in input order: the texts share one prefill window (a segment and a
//! session a text, every row's hidden state kept) up to `window_rows` rows; a one-token or q4_0 text runs `embed_forward` alone,
//! except under `device`, where every text rides the driver's scratch window on a device-home session. `kvdt` as in `embed_`.
def embed_batch_(m : Model; texts : array<string>; pooling : int64 = POOLING_MEAN; dims : int64 = 0l; window_rows : int64 = EMBED_WINDOW_ROWS;
                 kvdt : KVDtype = KVDtype.f32; device : bool = false) : array<array<float>> {
    let n = long_length(texts)
    var toks : array<array<int64>>
    toks |> reserve(n)
    for (text in texts) {
        var ids <- encode_(m, text)
        if (long_length(ids) > m.config.seq_len) {
            to_log(LOG_WARNING, "embed: {length(ids)} tokens past the model context of {m.config.seq_len}; the leading {m.config.seq_len} are embedded\n")
            ids |> resize(m.config.seq_len)
        }
        toks |> emplace(ids)
    }
    var out : array<array<float>>
    out |> resize(n)
    var i = 0l
    while (i < n) {
        var j = i
        var rows = 0l
        while (j < n && (j == i || rows + long_length(toks[j]) <= window_rows)) {
            rows += long_length(toks[j])
            j++
        }
        embed_window(m, toks, i, j, pooling, dims, kvdt, device, out)
        i = j
    }
    delete toks
    return <- out
}

//! texts `lo` .. `hi` of `toks` as one window into `out`
def private embed_window(m : Model; toks : array<array<int64>>; lo, hi, pooling, dims : int64; kvdt : KVDtype; device : bool;
                         var out : array<array<float>>) {
    let pooled = session_keeps_rows_only(m.config)
    var sess : array<Session>
    if (pooled) {
        embed_pool_take(m, hi - lo, kvdt, device)
    } else {
        sess |> reserve(hi - lo)   // the segments point into it: it never grows after the first emplace
    }
    var segs : array<PrefillSeg>
    for (k in range64(lo, hi)) {
        let np = long_length(toks[k])
        if (!pooled) {
            sess |> emplace(create_session_(m, kvdt, kvdt))
        }
        assume s = pooled ? g_embed_pool[k - lo] : sess[k - lo]
        if (!device && (m.quant == QuantMode.q4_0 || np <= 1l)) {
            g_embed_lone_forwards++
            embed_forward(m, s, toks[k], np, out[k], pooling, dims)
        } else {
            s.keep_hidden = true
            segs |> push(PrefillSeg(s = unsafe(addr(s)), start_pos = 0l, npos = np, toki = k))
        }
    }
    if (long_length(segs) == 1l && !device) {
        assume sg = segs[0]
        embed_forward(m, *sg.s, toks[sg.toki], sg.npos, out[sg.toki], pooling, dims)
    } elif (!empty(segs)) {
        forward_prefill_segments(m, segs, toks, true)
        let dim = m.config.dim
        for (sg in segs) {
            assume v = out[sg.toki]
            v |> reserve(dim)
            v |> resize(dim)
            for (d in range64(dim)) {
                v[d] = 0.0
            }
            let first = embed_first_row(pooling, sg.npos)
            let last = embed_end_row(pooling, sg.npos)
            embed_pool_rows(m, *sg.s, first, last, v)
            sg.s.n_past = sg.npos
            embed_finish(v, last - first, dims, dim)
        }
    }
    segs |> clear()   // the segments borrow sess's sessions: a delete would free them twice
    delete sess
}

// MoE FFN, decode: rmsnorm -> routed top-k + shared expert -> residual. Mirrors ffn_dense_decode's
// optional post-norm / layer-output-scale steps so future MoE arches compose with them.
[no_alloc, no_env, no_io]
def ffn_moe_decode(t : Model; var s : Session; l : int64) {
    let c = t.config
    if (l < c.n_layer_dense_lead) {   // glm4moe: the leading layer(s) are plain dense FFN
        ffn_dense_decode(t, s, l)
        return
    }
    let dim = c.dim
    let ts_norm = prof_ticks()
    block_in(t, s, t.rms_ffn_off + l * dim)
    prof_add("norm", ts_norm)
    moe_ffn_core(t, s, l)
    if (s.tail_taken) {
        s.tail_taken = false
        return
    }
    let ts_resid = prof_ticks()
    if (c.pre_post_norm) {
        rms_at(s.moe_acc, t, t.rms_post_ffn_off + l * dim, s.moe_acc, dim)
    }
    if (c.layer_out_scale) {
        add_scale_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.moe_acc[0])), t.fblob[t.out_scale_off + l], dim)
    } else {
        block_resid(t, s, s.moe_acc)
    }
    prof_add("act_resid", ts_resid)
}
// ===== dense FFN block kernels (decode) =====
//! layer `l`'s dense FFN triple sits on the per-op tier's FFN rail: its decode row runs on the device
[arch(at="../ARCHITECTURE_GPU_VULKAN_PEROP.md#perop-dense-ffn")]
def private dense_ffn_gpu(t : Model; l : int64) : bool {
    return (moe_gpu_tier_installed() && long_length(t.w1_offs) > l
        && moe_gpu_dense_ffn_on_gpu(int(fmt_at(t.w1_fmt, l)), int(fmt_at(t.w3_fmt, l)), int(fmt_at(t.w2_fmt, l)), t.w1_offs[l], t.w3_offs[l], t.w2_offs[l]))
}

def dense_ffn_inplace(t : Model; var s : Session; l : int64) {
    let c = t.config
    let dim = c.dim
    let hidden = layer_hidden(t, l)   // gemma-4 E2B varies FFN width per layer
    let ts_ffn = prof_ticks()
    let f1 = fmt_at(t.w1_fmt, l)
    let f2 = fmt_at(t.w2_fmt, l)
    let f3 = fmt_at(t.w3_fmt, l)
    if (dense_ffn_gpu(t, l)) {
        let gelu = c.ffn_act == FfnAct.gelu
        if (kq_sb(f1) != kq_sb(f3)) {   //! a gate and up of two activation forms: each GEMM over its own image (`x` is read before `y` is written)
            ffn_gpu_gemms(t.w1_offs[l], t.w3_offs[l], t.w2_offs[l], int(f1), int(f3), int(f2), c.ffn_act, dim, hidden, 1l, s.xb, s.xb)
            moe_gpu_dense_ffn_note(1l)
        } elif (fmt_reads_q8k(f1)) {
            quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
            matmul_gpu_dense_ffn(s.xb, t.w1_offs[l], t.w3_offs[l], t.w2_offs[l], s.kxq, s.kxs, dim, hidden, int(f1), int(f3), int(f2), gelu)
        } else {
            quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
            matmul_gpu_dense_ffn(s.xb, t.w1_offs[l], t.w3_offs[l], t.w2_offs[l], s.xq, s.xs, dim, hidden, int(f1), int(f3), int(f2), gelu)
        }
        prof_add("ffn_gpu", ts_ffn)
        return
    }
    if (f1 != KqFmt.q8 || f2 != KqFmt.q8 || f3 != KqFmt.q8) {
        // K-quant FFN (formats mix, e.g. Q4_K_M keeps ffn_down at Q6_K): one bs-quantize serves every format
        if (!fmt_reads_q8k(f1) || !fmt_reads_q8k(f3)) {
            quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
        }
        if (fmt_reads_q8k(f1) || fmt_reads_q8k(f3)) {   // kq v2: every kq format reads the Q8_K form
            quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
        }
        mm_pre_f(s.hb, t, f1, t.w1_offs[l], s.xq, s.xs, s.kxq, s.kxs, s.kxbs, dim, hidden)
        mm_pre_f(s.hb2, t, f3, t.w3_offs[l], s.xq, s.xs, s.kxq, s.kxs, s.kxbs, dim, hidden)
        prof_add("mm_ffn", ts_ffn)
        let ts_act = prof_ticks()
        gate_batch(s.hb, s.hb2, c.ffn_act, hidden, 1l)
        prof_add("act", ts_act)
        let ts_down = prof_ticks()
        if (!fmt_reads_q8k(f2)) {
            quantize_q8_0_into(s.hb, hidden, s.xq, s.xs, 0l, 0l)
        } else {   // kq v2
            quantize_q8_k_into(s.hb, hidden, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
        }
        mm_pre_f(s.xb, t, f2, t.w2_offs[l], s.xq, s.xs, s.kxq, s.kxs, s.kxbs, hidden, dim)
        prof_add("mm_ffn_down", ts_down)
    } elif (t.quant == QuantMode.q8) {
        // gate+up share the activation: ONE quantize + ONE region-list dispatch into ffn_gu's halves
        quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
        s.ffn_offs[0] = t.w1_offs[l]
        s.ffn_offs[1] = 0l
        s.ffn_offs[2] = t.w3_offs[l]
        s.ffn_offs[3] = 0l
        mm_at_q8_groupn(s.ffn_gu, t, s.ffn_offs, 2l, s.xq, s.xs, dim, hidden)
        prof_add("mm_ffn", ts_ffn)
        let ts_act = prof_ticks()
        unsafe {
            gate_batch(addr(s.ffn_gu[0]), addr < float const? >(s.ffn_gu[hidden]), c.ffn_act, hidden, 1l)
        }
        prof_add("act", ts_act)
        // down projection off the gated half
        let ts_down = prof_ticks()
        quantize_q8_0_into(s.ffn_gu, hidden, s.xq, s.xs, 0l, 0l)
        mm_at_q8_pre(s.xb, t, t.w2_offs[l], s.xq, s.xs, hidden, dim)
        prof_add("mm_ffn_down", ts_down)
    } else {
        mm(s.hb, t, t.w1_offs[l], s.xb, s.xq, s.xs, dim, hidden)
        mm(s.hb2, t, t.w3_offs[l], s.xb, s.xq, s.xs, dim, hidden)
        prof_add("mm_ffn", ts_ffn)
        let ts_act = prof_ticks()
        gate_batch(s.hb, s.hb2, c.ffn_act, hidden, 1l)   // fused act*gate; same kernel as prefill (npos=1 → inline)
        prof_add("act", ts_act)
        let ts_ffn2 = prof_ticks()
        mm(s.xb, t, t.w2_offs[l], s.hb, s.xq, s.xs, hidden, dim)
        prof_add("mm_ffn_down", ts_ffn2)
    }
}

[no_alloc, no_env, no_io]
def ffn_dense_decode(t : Model; var s : Session; l : int64) {
    if (fused_ffn_ok(t, l) && !dense_ffn_gpu(t, l)) {
        ffn_chain_decode(t, s, l)
        return
    }
    trace_tag(TRACE_TAG_FFN_STD)
    let c = t.config
    let dim = c.dim
    // ffn rmsnorm
    let ts_norm1 = prof_ticks()
    rms_at(s.xb, t, t.rms_ffn_off + l * dim, s.x, dim)
    prof_add("norm", ts_norm1)
    // gated FFN: hb = act(W1·xb) * (W3·xb) — silu (SwiGLU) or gelu (GeGLU, Gemma2)
    dense_ffn_inplace(t, s, l)
    let ts_resid1 = prof_ticks()
    if (c.pre_post_norm) {  // Gemma2: post-FFN RMSNorm before the residual add
        rms_at(s.xb, t, t.rms_post_ffn_off + l * dim, s.xb, dim)
    }
    // gemma4: E-series applies layer-output scale AFTER its PLE block, so skip it here when PLE is on
    if (c.layer_out_scale && !has_ple(c)) {
        add_scale_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb[0])), t.fblob[t.out_scale_off + l], dim)
    } else {
        add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb[0])), dim)
    }
    prof_add("act_resid", ts_resid1)
}

// Fused dense FFN block: ONE team rendezvous runs [w1+w3 rows + act + requant] -> [w2 rows +
// residual add] as two stages, gate/up rows PAIRED per chunk (requant lands in xq2/xs2, not
// xq/xs — other stage-0 chunks still read the input quants). Bit-identical to ffn_dense_decode.
[arch(at="../ARCHITECTURE_ENGINE_FORMATS_NV4.md#nv4-site-scale")]
def ffn_chain_decode(t : Model; var s : Session; l : int64) { // nolint:STYLE038 — ONE team rendezvous: the two stages and their paired gate/up chunking share hoisted scratch and must not be split across a barrier
    let c = t.config
    let dim = c.dim
    let hidden = layer_hidden(t, l)   // gemma-4 E2B varies FFN width per layer
    // per-weight formats (kq layers reach here only off a repacked load — fused_ffn_ok)
    let f1 = fmt_at(t.w1_fmt, l)
    let f2 = fmt_at(t.w2_fmt, l)
    let f3 = fmt_at(t.w3_fmt, l)
    let any_kq = f1 != KqFmt.q8 || f2 != KqFmt.q8 || f3 != KqFmt.q8
    let ts_norm1 = prof_ticks()
    rms_at(s.xb, t, t.rms_ffn_off + l * dim, s.x, dim)
    // one input quantize per consumed form: q8 gate/up read Q8_0, kq gate/up read Q8_K (kq v2)
    if (f1 == KqFmt.q8 || f3 == KqFmt.q8) {
        quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
    }
    if (f1 != KqFmt.q8 || f3 != KqFmt.q8) {
        quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
    }
    prof_add("norm", ts_norm1)
    let ts_chain = prof_ticks()
    trace_tag(TRACE_TAG_FFN_CHAIN)
    let mmrows = mm_q8q8_rows_fn()
    let is_gelu = c.ffn_act == FfnAct.gelu
    let is_oai = c.ffn_act == FfnAct.swiglu_oai
    let vec = g_act_vec
    // Gemma2/3 post-FFN norm is a whole-vector reduction, so the chain defers the residual add to a serial tail
    let post_norm = c.pre_post_norm
    let out_scale_here = c.layer_out_scale && !has_ple(c)
    let defer_add = post_norm || out_scale_here
    // stage-0 grain: 32 rows for a q8 down-proj; 256 for a kq one (Q8_K quantizes whole superblocks)
    let grain0 = f2 != KqFmt.q8 ? 256l : 32l
    let ngrp0 = int(hidden / grain0)
    let ngrp1 = int(dim / 32l)
    let n0 = chain_stage_chunks(ngrp0, int(hidden), 2l * dim * hidden)
    let n1 = chain_stage_chunks(ngrp1, int(dim), hidden * dim)
    s.chain_stages |> resize(2)
    s.chain_stages[0] = int3(0, ngrp0, n0)
    s.chain_stages[1] = int3(0, ngrp1, n1)
    count_parallel_dispatch(min(n0, ngrp0) + min(n1, ngrp1))
    unsafe {
        var xp = addr(s.x[0])
        var xbp = addr(s.xb[0])
        var gup = addr(s.ffn_gu[0])
        var xqp = addr(s.xq[0])
        var xsp = addr(s.xs[0])
        var xq2p = addr(s.xq2[0])
        var xs2p = addr(s.xs2[0])
        let f16w = t.wscale_f16
        let mmrows16 = mm_q8q8_rows16_fn()
        // all-kq layers skip the q8 bases — qblob/qscales can be EMPTY on an all-K-quant file
        let has_q8w = f1 == KqFmt.q8 || f2 == KqFmt.q8 || f3 == KqFmt.q8
        let wqb = has_q8w ? addr < int8 const? >(t.qblob[0]) : null
        let w1o = t.w1_offs[l]
        let w3o = t.w3_offs[l]
        let w2o = t.w2_offs[l]
        var ws1 : float const? = null
        var ws3 : float const? = null
        var ws2 : float const? = null
        var ws116 : uint16 const? = null
        var ws316 : uint16 const? = null
        var ws216 : uint16 const? = null
        if (f16w && has_q8w) {
            let wsb16 = addr < uint16 const? >(t.qscales16[0])
            ws116 = wsb16 + w1o / 32l
            ws316 = wsb16 + w3o / 32l
            ws216 = wsb16 + w2o / 32l
        } elif (has_q8w) {
            let wsb = addr < float const? >(t.qscales[0])
            ws1 = wsb + w1o / 32l
            ws3 = wsb + w3o / 32l
            ws2 = wsb + w2o / 32l
        }
        let wp1 = wqb + w1o
        let wp3 = wqb + w3o
        let wp2 = wqb + w2o
        // kq-tagged weights: plane bases + rows cores + the Q8_K activation image (kq v2); q8 rows cores keep Q8_0
        var kxqp : int8? = any_kq ? addr(s.kxq[0]) : null
        var kxsp : float? = any_kq ? addr(s.kxs[0]) : null
        var kxbsp : int? = any_kq ? addr(s.kxbs[0]) : null
        var xbs2w : int? = f2 != KqFmt.q8 ? addr(s.xbs2[0]) : null
        let kqp1 = kq_plane_q(t, f1, w1o)
        let ksp1 = kq_plane_s(t, f1, w1o)
        let rows1 = kq_rows_for(f1)
        let kqp3 = kq_plane_q(t, f3, w3o)
        let ksp3 = kq_plane_s(t, f3, w3o)
        let rows3 = kq_rows_for(f3)
        let kqp2 = kq_plane_q(t, f2, w2o)
        let ksp2 = kq_plane_s(t, f2, w2o)
        let rows2 = kq_rows_for(f2)
        let sc1 = site_scale(t, f1, w1o)
        let sc3 = site_scale(t, f3, w3o)
        let sc2 = site_scale(t, f2, w2o)
        team_parallel_stages(s.chain_stages) <| @(stage : int; jb : int; je : int) {   // nolint:PERF026 — dispatch-API lambda capture, see attention_chain_decode
            unsafe {
                let grain = stage == 0 ? grain0 : 32l
                let r0 = int64(jb) * grain
                let r1 = int64(je) * grain
                if (stage == 0) {
                    // paired gate/up rows, then act + requant on exactly these rows
                    if (f1 != KqFmt.q8) {
                        invoke(rows1, gup, kqp1, ksp1, kxqp, kxsp, kxbsp, dim, r0, r1)
                        if (sc1 != 1.0) {
                            scale_inplace(gup + r0, sc1, r1 - r0)
                        }
                    } elif (f16w) {
                        invoke(mmrows16, gup, wp1, ws116, xqp, xsp, dim, r0, r1)
                    } else {
                        invoke(mmrows, gup, wp1, ws1, xqp, xsp, dim, r0, r1)
                    }
                    if (f3 != KqFmt.q8) {
                        invoke(rows3, gup + hidden, kqp3, ksp3, kxqp, kxsp, kxbsp, dim, r0, r1)
                        if (sc3 != 1.0) {
                            scale_inplace(gup + hidden + r0, sc3, r1 - r0)
                        }
                    } elif (f16w) {
                        invoke(mmrows16, gup + hidden, wp3, ws316, xqp, xsp, dim, r0, r1)
                    } else {
                        invoke(mmrows, gup + hidden, wp3, ws3, xqp, xsp, dim, r0, r1)
                    }
                    if (vec) {
                        if (is_gelu) {
                            geglu4(gup + r0, gup + hidden + r0, r1 - r0)
                        } elif (is_oai) {
                            swiglu_oai4(gup + r0, gup + hidden + r0, r1 - r0)
                        } else {
                            swiglu4(gup + r0, gup + hidden + r0, r1 - r0)
                        }
                    } else {
                        if (is_gelu) {
                            geglu(gup + r0, gup + hidden + r0, r1 - r0)
                        } elif (is_oai) {
                            swiglu_oai(gup + r0, gup + hidden + r0, r1 - r0)
                        } else {
                            swiglu(gup + r0, gup + hidden + r0, r1 - r0)
                        }
                    }
                    if (f2 != KqFmt.q8) {   // kq down-proj: Q8_K requant, whole superblocks (grain 256)
                        quantize_q8_k_into_ptr(gup + r0, r1 - r0, xq2p, xs2p, xbs2w, r0, r0 / 256l, r0 / 16l)
                    } else {
                        quantize_q8_0_into_ptr(gup + r0, r1 - r0, xq2p, xs2p, r0, r0 / 32l)
                    }
                } else {
                    // down-projection rows + residual add (post-norm/layer_out_scale arches defer the add)
                    if (f2 != KqFmt.q8) {
                        invoke(rows2, xbp, kqp2, ksp2, xq2p, xs2p, xbs2w, hidden, r0, r1)
                        if (sc2 != 1.0) {
                            scale_inplace(xbp + r0, sc2, r1 - r0)
                        }
                    } elif (f16w) {
                        invoke(mmrows16, xbp, wp2, ws216, xq2p, xs2p, hidden, r0, r1)
                    } else {
                        invoke(mmrows, xbp, wp2, ws2, xq2p, xs2p, hidden, r0, r1)
                    }
                    if (!defer_add) {
                        add_inplace(xp + r0, xbp + r0, r1 - r0)
                    }
                }
            }
        }
    }
    prof_add("ffn_chain", ts_chain)
    if (defer_add) {  // same serial tail as the un-fused path (dim-sized, microseconds), bit-identical order
        let ts_resid = prof_ticks()
        if (post_norm) {  // Gemma2/3/4: post-FFN RMSNorm before the residual add
            rms_at(s.xb, t, t.rms_post_ffn_off + l * dim, s.xb, dim)
        }
        if (out_scale_here) {  // gemma4 12B/31B: residual + per-layer output scale, one pass
            add_scale_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb[0])), t.fblob[t.out_scale_off + l], dim)
        } else {
            add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb[0])), dim)
        }
        prof_add("act_resid", ts_resid)
    }
}

def std_blocks() : ArchBlocks {
    return ArchBlocks(std_chain = true, attn_decode = @@attention_std_decode, ffn_decode = @@ffn_dense_decode,
                      attn_prefill = @@attention_std_prefill, ffn_prefill = @@ffn_dense_prefill)
}

//! The MoE block set: standard attention + the routed-experts FFN (decode and naive-per-token
//! prefill). Arches whose configure sets `moe` register these instead of std_blocks.
def moe_blocks() : ArchBlocks {
    return ArchBlocks(std_chain = true, attn_decode = @@attention_std_decode, ffn_decode = @@ffn_moe_decode,
                      attn_prefill = @@attention_std_prefill, ffn_prefill = @@ffn_moe_prefill,
                      ffn_is_dense = false)
}
