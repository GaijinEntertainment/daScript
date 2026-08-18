options gen2
options _comment_hygiene = true

module dasllama_blocks shared public

require dasllama/dasllama_common
require dasllama/dasllama_moe   // moe_select_core/moe_ffn_core/moe_shexp_gate — the router the MoE blocks drive
require dasllama/dasllama_attn_prefill   // prefill_attention — the head-threaded prefill the std/dense blocks call
require dasllama/dasllama_math
require dasllama/dasllama_par
require dasllama/dasllama_convert
require dasllama/dasllama_kv_codec   // fwht_signs_row/fwht_unsign_row — the Hadamard rotation in gated attention
require dasllama/dasllama_env       // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
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
        s.chain_stages[2] = int3(0, int(n_kv_heads) * nsl, int(n_kv_heads) * nsl)
        s.chain_stages[3] = int3(0, nhu, nhu)
        s.chain_stages[4] = int3(0, ngrp, n2)
        count_parallel_dispatch(min(n0, ngq) + nslots + int(n_kv_heads) * nsl + nhu + min(n2, ngrp))
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
        // the lambda's capture frame is heap per call — the dispatch API's cost, not this kernel's
        team_parallel_stages(s.chain_stages) <| @(stage : int; jb : int; je : int) {
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
                // flash-decode partials: unit = (KV-head, slice) — K/V rows stream ONCE, serving all kv_mul q-heads
                unsafe {
                    for (u in range(jb, je)) {
                        let kvh = int64(u / nsl)
                        let sl = int64(u % nsl)
                        let kvhoff = kvh * head_size
                        let qh0 = kvh * kv_mul
                        let j0 = sl * slice_len
                        let j1 = min(j0 + slice_len, cnt)
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

def attention_std_decode(t : Model; var s : Session; l : int64; pos : int64) { // nolint:STYLE037,STYLE038 — dispatches every attention flavour (fused/flash/blocked/classic, GPU or CPU) from one place; the ladder IS the contract
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
                rope_scaled_neox_part(addr(s.q[0]), pos, head_size, rot, qd, theta, fscale, false, null, c.rope_mscale)
                if (!kv_shared) {
                    rope_scaled_neox_part(addr(s.k[0]), pos, head_size, rot, kv_dim, theta, fscale, false, null, c.rope_mscale)
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
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let kv_mul = n_heads / n_kv_heads
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
        if (kq_cls) {
            matmul_moe_gpu_attn(s.xb2_b, s.k_b, s.v_b,
                t.wq_offs[l], t.wk_offs[l], t.wv_offs[l], t.wo_offs[l],
                int(fq), int(fmt_at(t.wk_fmt, l)), int(fmt_at(t.wv_fmt, l)), int(fmt_at(t.wo_fmt, l)),
                s.kxqb, s.kxsb,
                c.qk_norm ? addr<float const?>(t.fblob[t.rms_q_offs[l]]) : null,
                c.qk_norm ? addr<float const?>(t.fblob[t.rms_k_offs[l]]) : null,
                addr<float const?>(s.rope_cos[0]), addr<float const?>(s.rope_sin[0]),
                npos, dim, qd, kv_dim, head_size, rot, n_heads, kv_mul,
                scale, c.norm_eps, gated, c.qk_norm)
        } else {
            matmul_moe_gpu_attn(s.xb2_b, s.k_b, s.v_b,
                t.wq_offs[l], t.wk_offs[l], t.wv_offs[l], t.wo_offs[l],
                int(fq), int(fmt_at(t.wk_fmt, l)), int(fmt_at(t.wv_fmt, l)), int(fmt_at(t.wo_fmt, l)),
                s.xqb, s.xsb,
                c.qk_norm ? addr<float const?>(t.fblob[t.rms_q_offs[l]]) : null,
                c.qk_norm ? addr<float const?>(t.fblob[t.rms_k_offs[l]]) : null,
                addr<float const?>(s.rope_cos[0]), addr<float const?>(s.rope_sin[0]),
                npos, dim, qd, kv_dim, head_size, rot, n_heads, kv_mul,
                scale, c.norm_eps, gated, c.qk_norm)
        }
    }
    prof_add("at_gpu", ts_gpu)
    // the cache store runs the normal CPU codec off the DMA'd k/v rows
    let ts_kv = prof_ticks()
    var kcl = kv_layer_kbase(s, l, c.seq_len)
    var vcl = kv_layer_vbase(s, l, c.seq_len)
    let store_nrun = kv_runs(s, 0l, npos, s.kv_runs)
    unsafe {
        kv_store_batch(kcl, vcl, s.kv_dtype_k, s.kv_dtype_v, s.k_b, s.v_b,
            addr < KVRun const? >(s.kv_runs[0]), store_nrun, 0l, npos, kv_dim)
    }
    prof_add("kv_store", ts_kv)
    let ts_resid = prof_ticks()
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
    prof_add("at_resid", ts_resid)
}

// npos >= 32 (a batch worth dispatching), start of sequence only (the chain reads the batch's own
// K/V device-side), tq4 excluded (basis change is CPU-side). A non-causal image span always
// declines: the GPU chains encode the causal mask — the guard survives lifting start_pos == 0.
def attn_gpu_prefill_ready(s : Session; l : int64; npos : int64; start_pos : int64) : bool {
    return (npos >= 32l && npos <= 4096l && start_pos == 0l && s.attn_uniform_end == 0l
        && moe_gpu_attn_on_gpu(l)
        && s.kv_dtype_k != KVDtype.tq4 && s.kv_dtype_v != KVDtype.tq4)
}

// attn-norm → batched QKV(+bias) → RoPE → KV-store → prefill attention → out-proj(+post-norm) → residual.
[no_alloc, no_env, no_io]
def attention_std_prefill(t : Model; var s : Session; l : int64; npos : int64; start_pos : int64) {
    if (attn_gpu_prefill_ready(s, l, npos, start_pos)) {
        attention_gpu_prefill(t, s, l, npos, false)   // buckets at_norm / at_gpu / kv_store / at_resid
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
        unsafe {
            let sgp = addr < float const? >(s.kv_signs[0])
            if (ktq4) {
                tq4_rotate_batch(s.q_b, npos, qd, head_size, sgp)
                if (!kv_shared) {
                    tq4_rotate_batch(s.k_b, npos, kv_dim, head_size, sgp)
                }
            }
            if (vtq4 && !kv_shared) {
                tq4_rotate_batch(s.v_b, npos, kv_dim, head_size, sgp)
            }
        }
        prof_add("tq4_rot", ts_rot)
    }
    // separate pass (provably identical to interleaving); shared-KV layers store nothing — already cached
    if (!kv_shared) {
        let ts_kv = prof_ticks()
        let store_nrun = kv_runs(s, start_pos, start_pos + npos, s.kv_runs)   // where the new rows land
        unsafe {
            kv_store_batch(kcl, vcl, s.kv_dtype_k, s.kv_dtype_v, s.k_b, s.v_b,
                addr < KVRun const? >(s.kv_runs[0]), store_nrun, start_pos, npos, kv_dim)
        }
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
        unsafe {
            tq4_unrotate_batch(s.xb_b, npos, qd, head_size, addr < float const? >(s.kv_signs[0]))
        }
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
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb_b[0])), npos * dim)
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

def private ffn_moe_prefill_grouped(t : Model; var s : Session; l : int64; npos : int64) { // nolint:STYLE037,STYLE038 — the grouped MoE prefill is one bucketed pipeline over experts; each stage feeds the next through shared scratch
    let c = t.config
    let dim = c.dim
    let nb = dim / 32l
    let ne = c.n_expert
    let k = c.n_expert_used
    let nfe = c.n_ff_exp
    let nsh = c.n_ff_shexp
    let nfx = max(nfe, nsh)
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
    let fused_gpu = (!c.moe_exps_bias && !t.experts_mx4 && c.ffn_act != FfnAct.swiglu_oai
        && (moe_gpu_layer_on_gpu(l) || moe_gpu_layer_streamed(l)) && g_moe_fused_expert_gemms && npos >= 32l && gpu_prefill_route_on())
    // device-side combine rides fused_gpu; DASLLAMA_GPU_COMBINE=0 falls back to park + CPU reduce
    let gpu_comb = fused_gpu && gpu_combine_on()
    let fused = (t.experts_mx4 && g_active_mx4_native_batch && g_moe_fused_expert_gemms) || fused_kq || fused_gpu
    let grows = fused ? nk : npos
    // a long media prefill (omni ASR, ~2.6k rows x k experts) sizes these into the tens of MB (sizing mirrors ple.das)
    s.moe_rlog |> zeroed_resize(npos * ne)
    s.moe_idx_b |> zeroed_resize(nk)
    s.moe_w_b |> zeroed_resize(nk)
    s.moe_cnt |> zeroed_resize(ne)
    s.moe_bkt |> zeroed_resize(nk)
    s.moe_gxq |> zeroed_resize(grows * dim)
    s.moe_gxs |> zeroed_resize(grows * nb)
    s.moe_ghb |> zeroed_resize(grows * nfx)
    s.moe_ghb2 |> zeroed_resize(grows * nfx)
    s.moe_hq |> zeroed_resize(grows * nfx)
    s.moe_hs |> zeroed_resize(grows * nfx / 32l)
    s.moe_gout |> zeroed_resize(grows * dim)
    s.moe_eout |> zeroed_resize(nk * dim)
    if (nsh > 0l && c.moe_shexp_gated) {
        s.moe_gl_b |> zeroed_resize(npos)
    }
    if (kq_layer) {
        s.moe_gxbs |> zeroed_resize(grows * dim / 16l)
        s.moe_hbs |> zeroed_resize(grows * nfx / 16l)
    }

    // 1. route every position: one batched router GEMM, then the shared select per row; shexp gate dots ride along
    trace_tag(TRACE_TAG_ROUTE)
    let ts_route = prof_ticks()
    mm_fblob_b(s.moe_rlog, t, t.router_off + l * ne * dim, s.xb_b, dim, ne, npos)
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
                    moe_select_core(gate, ne, k, norm_topk, wscale, lgp, idxp, wp, p * k, ebp)
                    if (glp != null) {
                        glp[p] = 1.0 / (1.0 + exp(-dot(gvp, xbp + p * dim, dim)))
                    }
                }
            }
        }
    }
    // bucket (position, slot) pairs by expert — CSR in-place cursor: moe_cnt[e] ends as e's run END
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

    // 2. quantize every normed activation row ONCE; kq v2 gate/up gathers off Q8_K, q8 stacks off Q8_0
    let moe_kform = kq_layer && moe_pair_kform(f1, f3)
    if (moe_kform) {
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

    // 3F. fused: gate/up/down each run as ONE region-list dispatch (3 fork/joins/layer, not 3/expert)
    if (fused) {
        trace_tag(TRACE_TAG_MOE)
        let ts_gather = prof_ticks()
        // the GPU tile computes activation block sums in-kernel — skip the per-16 sum gather
        moe_gather_rows(s, 0l, nk, dim, k, moe_kform && !fused_gpu, moe_kform)
        prof_add("moe_gather", ts_gather)
        // region triples (woff, row0, cnt) per touched expert; the three planes share row0/cnt
        s.moe_offs1 |> clear(); s.moe_offs1 |> reserve(ne * 3l)
        s.moe_offs3 |> clear(); s.moe_offs3 |> reserve(ne * 3l)
        s.moe_offs2 |> clear(); s.moe_offs2 |> reserve(ne * 3l)
        if (c.moe_exps_bias) {
            s.moe_boffs1 |> clear(); s.moe_boffs1 |> reserve(ne)
            s.moe_boffs3 |> clear(); s.moe_boffs3 |> reserve(ne)
            s.moe_boffs2 |> clear(); s.moe_boffs2 |> reserve(ne)
        }
        // LPT order (biggest buckets first) front-loads expensive spans; expert-order left 32% of GEMM windows idle
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
            let eg = l * ne + e
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
            if (c.moe_exps_bias) {
                s.moe_boffs1 |> push(t.web1_off + eg * nfe)
                s.moe_boffs3 |> push(t.web3_off + eg * nfe)
                s.moe_boffs2 |> push(t.web2_off + eg * dim)
            }
            nreg++
        }
        let ts_g1 = prof_ticks()
        if (gpu_comb) {   // GPU tier: gate+up/act/requant/down AND the routed weighted-sum combine
            // gout comes back as npos combined rows; the 33MB-class bucket-row readback + park scatter disappear
            s.moe_inv |> resize(nk)
            for (j in range64(nk)) {
                s.moe_inv[s.moe_bkt[j]] = uint(j)
            }
            matmul_moe_gpu_ffn_combined(s.moe_eout, s.moe_offs1, s.moe_offs3, s.moe_offs2, nreg,
                s.moe_gxq, s.moe_gxs, s.moe_w_b, s.moe_inv, dim, nfe, nk, npos, int(f1), int(f3), int(f2), c.ffn_act == FfnAct.gelu)
            prof_add("moe_gpu", ts_g1)
        } elif (fused_gpu) {   // combine off: per-row gout + the park/CPU-reduce path below
            matmul_moe_gpu_ffn(s.moe_gout, s.moe_offs1, s.moe_offs3, s.moe_offs2, nreg,
                s.moe_gxq, s.moe_gxs, dim, nfe, nk, int(f1), int(f3), int(f2), c.ffn_act == FfnAct.gelu)
            prof_add("moe_gpu", ts_g1)
        } else {
            if (fused_kq) {   // native-plane: one batched region-list GEMM per projection (never biased)
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
            } elif (c.moe_exps_bias) {   // gpt-oss: gate/up biases fold into the stores, before the act
                matmul_mx4q8_batch_groupn(s.moe_ghb, t.mxq, t.mxs, s.moe_offs1, nreg, s.moe_gxq, s.moe_gxs, t.fblob, s.moe_boffs1, dim, nfe)
                matmul_mx4q8_batch_groupn(s.moe_ghb2, t.mxq, t.mxs, s.moe_offs3, nreg, s.moe_gxq, s.moe_gxs, t.fblob, s.moe_boffs3, dim, nfe)
            } else {
                matmul_mx4q8_batch_groupn(s.moe_ghb, t.mxq, t.mxs, s.moe_offs1, nreg, s.moe_gxq, s.moe_gxs, dim, nfe)
                matmul_mx4q8_batch_groupn(s.moe_ghb2, t.mxq, t.mxs, s.moe_offs3, nreg, s.moe_gxq, s.moe_gxs, dim, nfe)
            }
            prof_add("mm_gemm", ts_g1)
            let ts_act = prof_ticks()
            gate_batch(s.moe_ghb, s.moe_ghb2, c.ffn_act, nfe, nk)
            prof_add("act", ts_act)
            if (fused_kq && kq_sb(f2)) {   // kq v2: sb down stacks read Q8_K-form rows (+ per-16 sums); q51/q8 the Q8_0 form
                requant_rows_q8k_bs(s.moe_ghb, nfe, nk, s.moe_hq, s.moe_hs, s.moe_hbs)
            } else {
                requant_rows_q8(s.moe_ghb, nfe, nk, s.moe_hq, s.moe_hs)
            }
            let ts_g3 = prof_ticks()
            if (fused_kq && kq_sb(f2)) {
                mm_b_kq_groupn(s.moe_gout, t, f2, s.moe_offs2, nreg, s.moe_hq, s.moe_hs, s.moe_hbs, nfe, dim)
            } elif (fused_kq) {
                mm_b_q51_groupn(s.moe_gout, t, s.moe_offs2, nreg, s.moe_hq, s.moe_hs, nfe, dim)
            } elif (c.moe_exps_bias) {   // ... and the down biases, inside the routed weight (ggml_add_id order)
                matmul_mx4q8_batch_groupn(s.moe_gout, t.mxq, t.mxs, s.moe_offs2, nreg, s.moe_hq, s.moe_hs, t.fblob, s.moe_boffs2, nfe, dim)
            } else {
                matmul_mx4q8_batch_groupn(s.moe_gout, t.mxq, t.mxs, s.moe_offs2, nreg, s.moe_hq, s.moe_hs, nfe, dim)
            }
            prof_add("mm_gemm", ts_g3)
        }
        // threaded bucket scatter: serial was an ~8ms ALL-LANE hole here (~67MB traffic at pp512/30B)
        if (!gpu_comb) {
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
    }

    // 3. one batched GEMM chain per touched expert; raw down rows park for the ordered reduce (skipped when fused ran)
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
        let eg = l * ne + e
        let base = e * dim * nfe
        if (kq_layer) {   // native kq batch tiles off the repacked planes, per-kind fmt (down may differ)
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
        } elif (t.experts_mx4 && g_active_mx4_native_batch) {
            // native mx4 batch GEMMs straight off the nibble/E8M0 planes — no expansion
            mm_b_mx4_pre(s.moe_ghb, t, t.we1_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
            mm_b_mx4_pre(s.moe_ghb2, t, t.we3_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
        } elif (t.experts_mx4) {   // exact Q8 expansion of this expert, then the proven batch GEMMs
            let ts_ex = prof_ticks()
            expand_mx4_region_q8(t, t.we1_offs[l] + base, dim, nfe, s.mx4_wq, s.mx4_ws)
            prof_add("moe_expand", ts_ex)
            let ts_g1 = prof_ticks()
            matmul_q8q8_batch(s.moe_ghb, s.mx4_wq, s.mx4_ws, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
            prof_add("mm_gemm", ts_g1)
            let ts_ex2 = prof_ticks()
            expand_mx4_region_q8(t, t.we3_offs[l] + base, dim, nfe, s.mx4_wq, s.mx4_ws)
            prof_add("moe_expand", ts_ex2)
            let ts_g2 = prof_ticks()
            matmul_q8q8_batch(s.moe_ghb2, s.mx4_wq, s.mx4_ws, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
            prof_add("mm_gemm", ts_g2)
        } else {
            mm_b_q8_pre(s.moe_ghb, t, t.we1_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
            mm_b_q8_pre(s.moe_ghb2, t, t.we3_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
        }
        if (c.moe_exps_bias) {  // gpt-oss: this expert's gate/up biases before the activation
            for (r in range64(cnt)) {
                add_bias(s.moe_ghb, r * nfe, t.fblob, t.web1_off + eg * nfe, nfe)
                add_bias(s.moe_ghb2, r * nfe, t.fblob, t.web3_off + eg * nfe, nfe)
            }
        }
        let ts_act = prof_ticks()
        gate_batch(s.moe_ghb, s.moe_ghb2, c.ffn_act, nfe, cnt)
        prof_add("act", ts_act)
        if (kq_sb(f2)) {   // kq v2: Q8_K-form rows + the per-16 sums the fold consumes; q51/q8 the Q8_0 form
            requant_rows_q8k_bs(s.moe_ghb, nfe, cnt, s.moe_hq, s.moe_hs, s.moe_hbs)
        } else {
            requant_rows_q8(s.moe_ghb, nfe, cnt, s.moe_hq, s.moe_hs)
        }
        if (kq_sb(f2)) {
            mm_b_kq_pre(s.moe_gout, t, f2, t.we2_offs[l] + base, s.moe_hq, s.moe_hs, s.moe_hbs, nfe, dim, cnt)
        } elif (f2 == KqFmt.q51) {
            mm_b_q51_pre(s.moe_gout, t, t.we2_offs[l] + base, s.moe_hq, s.moe_hs, nfe, dim, cnt)
        } elif (t.experts_mx4 && g_active_mx4_native_batch) {
            mm_b_mx4_pre(s.moe_gout, t, t.we2_offs[l] + base, s.moe_hq, s.moe_hs, nfe, dim, cnt)
        } elif (t.experts_mx4) {
            let ts_ex3 = prof_ticks()
            expand_mx4_region_q8(t, t.we2_offs[l] + base, nfe, dim, s.mx4_wq, s.mx4_ws)
            prof_add("moe_expand", ts_ex3)
            let ts_g3 = prof_ticks()
            matmul_q8q8_batch(s.moe_gout, s.mx4_wq, s.mx4_ws, s.moe_hq, s.moe_hs, nfe, dim, cnt)
            prof_add("mm_gemm", ts_g3)
        } else {
            mm_b_q8_pre(s.moe_gout, t, t.we2_offs[l] + base, s.moe_hq, s.moe_hs, nfe, dim, cnt)
        }
        if (c.moe_exps_bias) {  // ... and its down bias, inside the routed weight (ggml_add_id order)
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

    // 4. shared expert for ALL positions — a plain dense batched FFN off the whole-batch image
    if (nsh > 0l) {
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
        let wp = addr<float const?>(s.moe_w_b[0])
        let sp = nsh > 0l ? addr<float const?>(s.moe_gout[0]) : null
        let gp = nsh > 0l && c.moe_shexp_gated ? addr<float const?>(s.moe_gl_b[0]) : null
        maybe_parallel_for(nk * dim >= g_act_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    var arow = ap + pp * dim
                    if (gpu_comb) {
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
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
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
    rms_batch(s.xb_b, s.x_b, t, t.rms_ffn_off + l * dim, dim, npos)
    prof_add("norm", ts_norm)
    // native-MXFP4 grouped path pays a per-touched-expert Q8 expansion (~120MB/expert); under ~8 rows/expert the per-position mx4 GEMVs are cheaper
    let mx4_grouped_pays = !t.experts_mx4 || npos * c.n_expert_used >= 8l * c.n_expert
    // kq-expert layers ride grouped buckets via the kq batch tile; portable/no-kq backends take the per-position path
    let kq_grouped_ok = !moe_layer_sb_kq(t, l) || (t.kq_repacked && kernel_backend_has_kq_batch())
    if (t.quant == QuantMode.q8 && g_moe_grouped_prefill && mx4_grouped_pays && kq_grouped_ok) {
        ffn_moe_prefill_grouped(t, s, l, npos)
    } else {
        // per-position reference path — moe_ffn_core carries its own section buckets
        for (p in range64(npos)) {
            copy_floats(s.xb_b, p * dim, s.xb, 0l, dim)
            moe_ffn_core(t, s, l)
            if (c.pre_post_norm) {
                rms_at(s.moe_acc, t, t.rms_post_ffn_off + l * dim, s.moe_acc, dim)
            }
            add_inplace(unsafe(addr(s.x_b[p * dim])), unsafe(addr(s.moe_acc[0])), dim)   // residual add into this position's x_b row
        }
    }
    if (c.layer_out_scale) {
        let os = t.fblob[t.out_scale_off + l]
        scale_inplace(unsafe(addr(s.x_b[0])), os, npos * dim)
    }
}

def forward_prefill_embd(t : Model; var s : Session; embd : array<float>; npos, start_pos : int64;
                         non_causal : bool = false; src_off : int64 = 0l; skip_logits : bool = false) {
    if (npos <= 0l) {
        return
    }
    if (t.quant == QuantMode.q4_0) {
        panic("forward_prefill_embd: q4_0 mode has no batched prefill path (use q8 or fp32)")
    }
    if (non_causal && t.config.recr_mask != 0ul) {
        panic("forward_prefill_embd: a non-causal span cannot apply to recurrent layers (recr_mask != 0) - the recurrence has no uniform-bound form")
    }
    if (long_length(embd) < (src_off + npos) * t.config.dim) {
        panic("forward_prefill_embd: embd has {long_length(embd)} floats - rows [{src_off},{src_off + npos}) of dim={t.config.dim} need {(src_off + npos) * t.config.dim}")
    }
    kv_ensure(s, start_pos + npos)   // paged sessions: grow the block table BEFORE any cache-pointer hoist
    forward_prefill_alloc(t, s, npos, start_pos)
    let dim = t.config.dim
    let ts_embed = prof_ticks()
    let base = src_off * dim
    for (i in range64(npos * dim)) {
        s.x_b[i] = embd[base + i]
    }
    prof_add("embed", ts_embed)
    if (has_ple(t.config)) {  // gemma-4 E-series: PLE side input (padding-token gather for the media path)
        let ts_ple = prof_ticks()
        ple_pre_prefill_pad(t, s, npos)
        prof_add("ple", ts_ple)
    }
    // the non-causal image span (mtmd_decode_use_non_causal, gemma vision): every query of THIS
    // call attends [the sliding-window floor, start_pos + npos) — set for exactly this body
    s.attn_uniform_end = non_causal ? start_pos + npos : 0l
    forward_prefill_body(t, s, npos, start_pos, skip_logits)
    s.attn_uniform_end = 0l
}

//! Fill ``dst[at*dim ..]`` with the embedding rows of ``tokens`` (embed_scale applied) — the
//! text-span half of a multimodal prefill buffer.
def embed_text_rows(t : Model; tokens : array<int64>; var dst : array<float>; at : int64) {
    let dim = t.config.dim
    let n = long_length(tokens)
    for (p in range64(n)) {
        embed_row(t, tokens[p], unsafe(addr(dst[(at + p) * dim])))
    }
    if (t.config.embed_scale != 1.0) {
        scale_inplace(unsafe(addr(dst[at * dim])), t.config.embed_scale, n * dim)
    }
}

//! ``eval_``'s embedding-input twin: prefill ``npos`` embedding rows at the session cursor and
//! advance it. Leaves ``s.logits`` = the last position's logits. ``non_causal`` marks the rows
//! as a non-causal image span (gemma vision) — see forward_prefill_embd.
def eval_embd_(t : Model; var s : Session; embd : array<float>; npos : int64; non_causal : bool = false) {
    if (npos <= 0l) {
        return
    }
    forward_prefill_embd(t, s, embd, npos, s.n_past, non_causal)
    s.n_past += npos
}

//! `eval_embd_` for a prompt carrying ONE non-causal image span: rows [span_lo, span_hi) prefill
//! with the span flag, the text around them causally. Three evals, because the flag is per-call —
//! the KV cache is identical either way, so only the masks differ (`vision_plan.md` §pipeline).
def eval_embd_span_(t : Model; var s : Session; embd : array<float>; npos, span_lo, span_hi : int64) {
    if (npos <= 0l) {
        return
    }
    if (span_lo < 0l || span_hi > npos || span_hi <= span_lo) {
        panic("eval_embd_span_: span [{span_lo},{span_hi}) outside [0,{npos})")
    }
    // slices read embd in place via src_off; only the last non-empty slice produces logits
    eval_embd_slice_(t, s, embd, 0l, span_lo, false, false)
    eval_embd_slice_(t, s, embd, span_lo, span_hi, true, span_hi >= npos)
    eval_embd_slice_(t, s, embd, span_hi, npos, false, true)
}

def private eval_embd_slice_(t : Model; var s : Session; embd : array<float>; lo, hi : int64;
                             non_causal, last : bool) {
    let n = hi - lo
    if (n <= 0l) {
        return
    }
    forward_prefill_embd(t, s, embd, n, s.n_past, non_causal, src_off = lo, skip_logits = !last)
    s.n_past += n
}

//! Mean-pooled, L2-normalized sentence embedding for `tokens` into `out`: last-layer hidden state
//! averaged over positions, unit-normalized. Runs one forward over `s` from position 0 — pass a
//! FRESH session (no KV history assumed; cache is left filled).
def embed_forward(t : Model; var s : Session; tokens : array<int64>; npos : int64; var out : array<float>) {
    let c = t.config
    let dim = c.dim
    out |> resize(dim)
    for (i in range64(dim)) {
        out[i] = 0.0
    }
    if (npos <= 0l) {
        return
    }
    if (t.quant != QuantMode.q4_0 && npos > 1l) {
        // batched prefill fills s.x_b (npos × dim residual) and leaves it intact; the body norms only
        // the last position, so re-apply the final RMSNorm to EVERY position and accumulate.
        forward_prefill(t, s, tokens, npos, 0l)
        for (p in range64(npos)) {
            rms_at(s.xb, t, t.rms_final_off, s.x_b, p * dim, dim)
            add_inplace(unsafe(addr(out[0])), unsafe(addr(s.xb[0])), dim)
        }
    } else {
        // q4_0 (no batched prefill) or a single token: per-token forward leaves s.xb = post-final-norm hidden.
        for (p in range64(npos)) {
            forward(t, s, tokens[p], p)
            add_inplace(unsafe(addr(out[0])), unsafe(addr(s.xb[0])), dim)
        }
    }
    s.n_past = npos   // KV now holds npos positions (start_pos was 0) — keep the session cursor consistent
    // mean over positions, then L2-normalize
    let inv = 1.0 / float(npos)
    var ss = 0.0
    for (i in range64(dim)) {
        out[i] *= inv
        ss += out[i] * out[i]
    }
    let rn = ss > 0.0 ? 1.0 / sqrt(ss) : 0.0
    for (i in range64(dim)) {
        out[i] *= rn
    }
}

//! Mean-pooled, L2-normalized sentence embedding of `text` (`dim` floats): tokenize, run one forward,
//! average the decoder's last-layer hidden state over positions, unit-normalize. Decoder-as-embedder
//! yields RAG-grade vectors, not a substitute for a dedicated embedding model.
def embed_(m : Model; text : string) : array<float> {
    var s <- create_session_(m)
    let tokens = encode_(m, text)
    var out : array<float>
    embed_forward(m, s, tokens, long_length(tokens), out)
    delete s
    return <- out
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
    rms_at(s.xb, t, t.rms_ffn_off + l * dim, s.x, dim)
    prof_add("norm", ts_norm)
    moe_ffn_core(t, s, l)
    let ts_resid = prof_ticks()
    if (c.pre_post_norm) {
        rms_at(s.moe_acc, t, t.rms_post_ffn_off + l * dim, s.moe_acc, dim)
    }
    if (c.layer_out_scale) {
        add_scale_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.moe_acc[0])), t.fblob[t.out_scale_off + l], dim)
    } else {
        add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.moe_acc[0])), dim)
    }
    prof_add("act_resid", ts_resid)
}
// ===== dense FFN block kernels (decode) =====
def dense_ffn_inplace(t : Model; var s : Session; l : int64) {
    let c = t.config
    let dim = c.dim
    let hidden = layer_hidden(t, l)   // gemma-4 E2B varies FFN width per layer
    let ts_ffn = prof_ticks()
    let f1 = fmt_at(t.w1_fmt, l)
    let f2 = fmt_at(t.w2_fmt, l)
    let f3 = fmt_at(t.w3_fmt, l)
    if (f1 != KqFmt.q8 || f2 != KqFmt.q8 || f3 != KqFmt.q8) {
        // K-quant FFN (formats mix, e.g. Q4_K_M keeps ffn_down at Q6_K): one bs-quantize serves every format
        if (f1 == KqFmt.q8 || f3 == KqFmt.q8) {
            quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
        }
        if (f1 != KqFmt.q8 || f3 != KqFmt.q8) {   // kq v2: every kq format reads the Q8_K form
            quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
        }
        mm_pre_f(s.hb, t, f1, t.w1_offs[l], s.xq, s.xs, s.kxq, s.kxs, s.kxbs, dim, hidden)
        mm_pre_f(s.hb2, t, f3, t.w3_offs[l], s.xq, s.xs, s.kxq, s.kxs, s.kxbs, dim, hidden)
        prof_add("mm_ffn", ts_ffn)
        let ts_act = prof_ticks()
        gate_batch(s.hb, s.hb2, c.ffn_act, hidden, 1l)
        prof_add("act", ts_act)
        let ts_down = prof_ticks()
        if (f2 == KqFmt.q8) {
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
    if (fused_ffn_ok(t, l)) {
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
        team_parallel_stages(s.chain_stages) <| @(stage : int; jb : int; je : int) {   // nolint:PERF026 — dispatch-API lambda capture, see attention_chain_decode
            unsafe {
                let grain = stage == 0 ? grain0 : 32l
                let r0 = int64(jb) * grain
                let r1 = int64(je) * grain
                if (stage == 0) {
                    // paired gate/up rows, then act + requant on exactly these rows
                    if (f1 != KqFmt.q8) {
                        invoke(rows1, gup, kqp1, ksp1, kxqp, kxsp, kxbsp, dim, r0, r1)
                    } elif (f16w) {
                        invoke(mmrows16, gup, wp1, ws116, xqp, xsp, dim, r0, r1)
                    } else {
                        invoke(mmrows, gup, wp1, ws1, xqp, xsp, dim, r0, r1)
                    }
                    if (f3 != KqFmt.q8) {
                        invoke(rows3, gup + hidden, kqp3, ksp3, kxqp, kxsp, kxbsp, dim, r0, r1)
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
    return ArchBlocks(attn_decode = @@attention_std_decode, ffn_decode = @@ffn_dense_decode,
                      attn_prefill = @@attention_std_prefill, ffn_prefill = @@ffn_dense_prefill)
}

//! The MoE block set: standard attention + the routed-experts FFN (decode and naive-per-token
//! prefill). Arches whose configure sets `moe` register these instead of std_blocks.
def moe_blocks() : ArchBlocks {
    return ArchBlocks(attn_decode = @@attention_std_decode, ffn_decode = @@ffn_moe_decode,
                      attn_prefill = @@attention_std_prefill, ffn_prefill = @@ffn_moe_prefill,
                      ffn_is_dense = false)
}
