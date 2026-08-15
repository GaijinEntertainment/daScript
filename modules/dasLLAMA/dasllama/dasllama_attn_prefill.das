options gen2
options _comment_hygiene = true

module dasllama_attn_prefill shared public

require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_par
require dasllama/dasllama_convert   // quantize_q8_0_into — the prefill activation quant
require math

// Prefill attention, threaded over heads. prefill_attention is the only entry point; the three
// head kernels below it (classic / blocked / flash) and their KV-codec dispatch ladders are its
// implementation. Nothing in the hub names any of them -- the block kernels call in from outside.

// g_attn_prefill_mode: classic/blocked are bit-identical (same score row + FMA order); flash tiles
// with online softmax and is NOT bit-exact (online reorder).
def private attn_head_flash(qp : float const?; kcp, vcp : auto const?; var xbp : float?; // nolint:STYLE037,STYLE038 — online-softmax tiling: the KV-format arms and the running max/sum state are one register-coupled loop nest
                            var qpk, kpk, vpk, kqp, vkqp, tmpp : float?; runs : KVRun const?;
                            nrun, npos, start_pos, uniform_end, head_size, qd, kdim, khoff, vdim, vhoff, qoff : int64;
                            scale : float; is_sliding : bool; sliding_window : int64; attn_softcap : float;
                            has_sink : bool; sink : float) {
    let QT = ATTN_FLASH_QT
    let KV = ATTN_FLASH_KV
    unsafe {
        var p0 = 0l
        while (p0 < npos) {
            let tile_rows = min(QT, npos - p0)
            let ap0 = start_pos + p0
            let apL = ap0 + tile_rows - 1l                 // max query position in this tile
            // uniform_end > 0: the non-causal image span — every query attends [the sliding-window floor, uniform_end)
            let kend = uniform_end > 0l ? uniform_end : apL + 1l
            let kstart_min = (is_sliding && ap0 + 1l > sliding_window) ? (ap0 + 1l - sliding_window) : 0l
            // pack the Q tile contiguous [tile_rows × head_size] (q rows are qd-strided in q_b)
            for (qt in range64(tile_rows)) {
                let qsrc = (p0 + qt) * qd + qoff
                for (d in range64(head_size)) {
                    qpk[qt * head_size + d] = qp[qsrc + d]
                }
            }
            var mrun : float[64]   // must equal ATTN_FLASH_QT (fixed-array dim needs an int32 literal)
            var srun : float[64]
            for (qt in range64(tile_rows)) {
                // the sink (gpt-oss) is one extra virtual score with no V contribution — seeding the
                // online softmax with max = sink, sum = e^(sink-sink) = 1 is exactly that column;
                // later tiles rescale it along with everything else
                mrun[int(qt)] = has_sink ? sink : -1.0e30
                srun[int(qt)] = has_sink ? 1.0 : 0.0
                for (d in range64(head_size)) {
                    vkqp[qt * head_size + d] = 0.0
                }
            }
            var kt0 = kstart_min
            while (kt0 < kend) {
                let kvalid = min(KV, kend - kt0)
                // pack Kᵀ tile [head_size × KV] and V tile [KV × head_size] by physical run: each
                // cached row dequants ONCE via kv_row_to_f32 (the dequant seam — the GEMMs read f32
                // scratch), V straight into its tile row, K through tmpp then transpose-scattered
                for (r in range64(nrun)) {
                    let rj0 = max(runs[r].j0, kt0)
                    let rj1 = min(runs[r].j1, kt0 + kvalid)
                    let rowbase = runs[r].row0 - runs[r].j0
                    for (aj in range64(rj0, rj1)) {
                        let kt = aj - kt0
                        kv_row_to_f32(vpk + kt * head_size, vcp, (rowbase + aj) * vdim + vhoff, head_size)
                        kv_row_to_f32(tmpp, kcp, (rowbase + aj) * kdim + khoff, head_size)
                        for (d in range64(head_size)) {
                            kpk[d * KV + kt] = tmpp[d]
                        }
                    }
                }
                for (kt in range64(kvalid, KV)) {   // zero-pad the tail columns
                    for (d in range64(head_size)) {
                        kpk[d * KV + kt] = 0.0
                        vpk[kt * head_size + d] = 0.0
                    }
                }
                // scores: kq[tile_rows × KV] = Q · Kᵀ  (gemm_f32 accumulates, so zero first)
                for (i in range64(tile_rows * KV)) {
                    kqp[i] = 0.0
                }
                gemm_f32(kqp, qpk, kpk, tile_rows, head_size, KV)
                // ggml-style decomposition (ops.cpp:8771): scale — then soft-cap — the whole score tile
                // in one vectorized sweep each, replacing the scalar per-element multiply in the mask branch.
                scale_inplace(kqp, scale, tile_rows * KV)
                if (attn_softcap > 0.0) {   // Gemma2: soft-cap scores before softmax
                    softcap4(kqp, tile_rows * KV, attn_softcap)
                }
                // online-softmax fold per query row. The causal + sliding mask is a CONTIGUOUS column
                // span [lo, hi] (cached keys kstart_p..ap), so masking is two boundary fills (no
                // per-element branch) and the row max is hand-vectorized (hmax).
                for (qt in range64(tile_rows)) {
                    let ap = ap0 + qt
                    let kstart_p = (is_sliding && ap + 1l > sliding_window) ? (ap + 1l - sliding_window) : 0l
                    let row = qt * KV
                    let lo = max(0l, kstart_p - kt0)              // first valid tile-local column
                    // causal: last valid column is the query's own position; non-causal span: the whole tile
                    let hi = uniform_end > 0l ? kvalid - 1l : min(kvalid - 1l, ap - kt0)
                    if (hi < lo) {                                // no valid keys here → contributes nothing
                        for (kt in range64(KV)) { kqp[row + kt] = 0.0 }
                        continue
                    }
                    for (kt in range64(0l, lo))      { kqp[row + kt] = -1.0e30 }   // below the sliding window
                    for (kt in range64(hi + 1l, KV)) { kqp[row + kt] = -1.0e30 }   // above causal / zero-padded
                    let tmax = hmax(kqp + row, KV)
                    let mold = mrun[int(qt)]
                    let mnew = max(mold, tmax)
                    if (mnew > mold) {                 // new running max → rescale this row's accumulator
                        let ms = exp(mold - mnew)
                        scale_inplace(vkqp + qt * head_size, ms, head_size)
                        srun[int(qt)] *= ms
                    }
                    mrun[int(qt)] = mnew
                    // kq row becomes the (un-normalized) prob row — vectorized exp4 over KV (mult of 4).
                    // Masked entries (-1e30) clamp to -80 so exp4's fast path stays in range and yields ~0
                    // (1.8e-35, negligible vs the real probs); flash is already non-bit-exact (online reorder).
                    let mnew4 = float4(mnew, mnew, mnew, mnew)
                    let floor4 = float4(-80.0, -80.0, -80.0, -80.0)
                    var sum4 = float4(0.0, 0.0, 0.0, 0.0)
                    var prow = reinterpret<float4?>(kqp + row)
                    for (kc in range64(KV / 4l)) {
                        let pe4 = exp4(max(prow[kc] - mnew4, floor4))
                        prow[kc] = pe4
                        sum4 += pe4
                    }
                    srun[int(qt)] += sum4.x + sum4.y + sum4.z + sum4.w
                }
                // accumulate output: vkq[tile_rows × head_size] += P[tile_rows × KV] · V[KV × head_size]
                gemm_f32(vkqp, kqp, vpk, tile_rows, KV, head_size)
                kt0 += KV
            }
            // normalize by the running sum and write the qd-strided output slice
            for (qt in range64(tile_rows)) {
                let inv = srun[int(qt)] > 0.0 ? 1.0 / srun[int(qt)] : 0.0
                let dst = (p0 + qt) * qd + qoff
                for (d in range64(head_size)) {
                    xbp[dst + d] = vkqp[qt * head_size + d] * inv
                }
            }
            p0 += QT
        }
    }
}

// One head of BLOCKED prefill attention (see prefill_attention) — codec-generic + run-walking.
// attp_h = this head's ATTN_BLOCK_Q score rows (maxctx-strided). Scores go through kv_score
// (q8_0 K: the same dot decode runs, keeping prefill == decode bit-exact under the block codec).
def private attn_head_blocked(var attp_h : float?; var xbp : float?; qp : float const?; // nolint:STYLE037,STYLE038 — two K/V passes register-coupled through the shared score plane; the causal/span bounds must stay with both
                              qq : int8 const ?; qsp : float const?;
                              kcp, vcp : auto const?; runs : KVRun const?;
                              nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh : int64;
                              scale : float; is_sliding : bool; sliding_window : int64;
                              attn_softcap : float; sinksp : float const?) {
    unsafe {
        var p0 = 0l
        while (p0 < npos) {
            let bq = min(ATTN_BLOCK_Q, npos - p0)
            let ap_first = start_pos + p0
            let ap_last = ap_first + bq - 1l
            // uniform_end > 0: the non-causal image span — every query attends [the sliding-window floor, uniform_end)
            let bend = uniform_end > 0l ? uniform_end : ap_last + 1l
            let kstart_block = (is_sliding && ap_first + 1l > sliding_window) ? (ap_first + 1l - sliding_window) : 0l
            // PASS 1: scores — each K[kk] loaded once, reused across the block's queries
            for (r in range64(nrun)) {
                let rj0 = max(runs[r].j0, kstart_block)
                let rj1 = min(runs[r].j1, bend)
                let rowbase = runs[r].row0 - runs[r].j0
                for (kk in range64(rj0, rj1)) {
                    let koff = (rowbase + kk) * kdim + khoff
                    let bi_lo = uniform_end > 0l ? 0l : max(0l, kk - ap_first)   // causal: keep queries with ap >= kk
                    var bi_hi = bq
                    if (is_sliding) {                            // window: keep queries with kstart_p <= kk
                        bi_hi = min(bq, kk + sliding_window - ap_first)
                    }
                    for (bi in range64(bi_lo, bi_hi)) {
                        let p = p0 + bi
                        let ap = start_pos + p
                        let kstart_p = (is_sliding && ap + 1l > sliding_window) ? (ap + 1l - sliding_window) : 0l
                        let qbase = p * qd + qoff
                        attp_h[bi * maxctx + (kk - kstart_p)] = kv_score(qp, qq, qsp, kcp, koff, qbase, head_size) * scale
                    }
                }
            }
            // softcap + softmax per query, then zero its output slice
            for (bi in range64(bq)) {
                let p = p0 + bi
                let ap = start_pos + p
                let kstart_p = (is_sliding && ap + 1l > sliding_window) ? (ap + 1l - sliding_window) : 0l
                let cnt = (uniform_end > 0l ? uniform_end : ap + 1l) - kstart_p
                let rbase = bi * maxctx
                if (attn_softcap > 0.0) {  // Gemma2: soft-cap scores before softmax
                    softcap4(attp_h + rbase, cnt, attn_softcap)
                }
                if (sinksp != null) {  // gpt-oss: sink joins the denominator
                    softmax_sink(attp_h + rbase, cnt, sinksp[hh])
                } else {
                    softmax(attp_h + rbase, cnt)
                }
                let qbase = p * qd + qoff
                for (i in range64(head_size)) {
                    xbp[qbase + i] = 0.0
                }
            }
            // PASS 2: output += Σ att[j]·V[j] — each V[kk] loaded once, reused across the block
            for (r in range64(nrun)) {
                let rj0 = max(runs[r].j0, kstart_block)
                let rj1 = min(runs[r].j1, bend)
                let rowbase = runs[r].row0 - runs[r].j0
                for (kk in range64(rj0, rj1)) {
                    let voff = (rowbase + kk) * vdim + vhoff
                    let bi_lo = uniform_end > 0l ? 0l : max(0l, kk - ap_first)
                    var bi_hi = bq
                    if (is_sliding) {
                        bi_hi = min(bq, kk + sliding_window - ap_first)
                    }
                    for (bi in range64(bi_lo, bi_hi)) {
                        let p = p0 + bi
                        let ap = start_pos + p
                        let kstart_p = (is_sliding && ap + 1l > sliding_window) ? (ap + 1l - sliding_window) : 0l
                        let qbase = p * qd + qoff
                        kv_axpy(xbp + qbase, vcp + voff, attp_h[bi * maxctx + (kk - kstart_p)], head_size)
                    }
                }
            }
            p0 += ATTN_BLOCK_Q
        }
    }
}

// One head of CLASSIC prefill attention (see prefill_attention) — codec-generic + run-walking.
// attp_h = this head's private score row (maxctx wide, window-relative); each query's window
// [kstart, ap] intersects the layer's physical runs. Scores through kv_score (see attn_head_blocked).
def private attn_head_classic(var attp_h : float?; var xbp : float?; qp : float const?;
                              qq : int8 const?; qsp : float const?;
                              kcp, vcp : auto const?; runs : KVRun const?;
                              nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh : int64;
                              scale : float; is_sliding : bool; sliding_window : int64;
                              attn_softcap : float; sinksp : float const?) {
    unsafe {
        for (p in range64(npos)) {
            let ap = start_pos + p
            // uniform_end > 0: the non-causal image span — every query attends [the sliding-window floor, uniform_end)
            let aend = uniform_end > 0l ? uniform_end : ap + 1l
            let kstart = (is_sliding && ap + 1l > sliding_window) ? (ap + 1l - sliding_window) : 0l
            let cnt = aend - kstart
            let qbase = p * qd + qoff           // q row == output row (both qd-strided)
            for (r in range64(nrun)) {
                let rj0 = max(runs[r].j0, kstart)
                let rj1 = min(runs[r].j1, aend)
                let rowbase = runs[r].row0 - runs[r].j0
                for (aj in range64(rj0, rj1)) {
                    attp_h[aj - kstart] = kv_score(qp, qq, qsp, kcp, (rowbase + aj) * kdim + khoff, qbase, head_size) * scale
                }
            }
            if (attn_softcap > 0.0) {  // Gemma2: soft-cap scores before softmax
                softcap4(attp_h, cnt, attn_softcap)
            }
            if (sinksp != null) {  // gpt-oss: sink joins the denominator
                softmax_sink(attp_h, cnt, sinksp[hh])
            } else {
                softmax(attp_h, cnt)
            }
            for (i in range64(head_size)) {
                xbp[qbase + i] = 0.0
            }
            for (r in range64(nrun)) {   // output += Σ att[j]·V[j]
                let rj0 = max(runs[r].j0, kstart)
                let rj1 = min(runs[r].j1, aend)
                let rowbase = runs[r].row0 - runs[r].j0
                for (aj in range64(rj0, rj1)) {
                    kv_axpy(xbp + qbase, vcp + (rowbase + aj) * vdim + vhoff, attp_h[aj - kstart], head_size)
                }
            }
        }
    }
}

// Prefill codec dispatch (same contract as attn_head_decode_d): one wrapper per prefill worker,
// the 3x3 K×V matrix spelled once. Prefill is compute-bound, so dequant-in-dot is fine — no
// query quantize needed.
def private attn_head_flash_d(qp : float const?; kcl, vcl : uint8 const?; kdt, vdt : KVDtype; var xbp : float?; // nolint:STYLE038 — 16-way K x V codec dispatch ladder; each arm is one typed call, nothing to factor
                              var qpk, kpk, vpk, kqp, vkqp, tmpp : float?; runs : KVRun const?;
                              nrun, npos, start_pos, uniform_end, head_size, qd, kv_dim, kvhoff, qoff : int64;
                              scale : float; is_sliding : bool; sliding_window : int64; attn_softcap : float;
                              has_sink : bool; sink : float) {
    let kdim = kv_side_off(kdt, kv_dim)
    let khoff = kv_side_off(kdt, kvhoff)
    let vdim = kv_side_off(vdt, kv_dim)
    let vhoff = kv_side_off(vdt, kvhoff)
    unsafe {
        if (kdt == KVDtype.q8_0) {
            if (vdt == KVDtype.q8_0) {
                attn_head_flash(qp, kcl, vcl, xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } elif (vdt == KVDtype.tq4) {
                attn_head_flash(qp, kcl, reinterpret<TQ4B const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } elif (vdt == KVDtype.f16) {
                attn_head_flash(qp, kcl, reinterpret<uint16 const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } else {
                attn_head_flash(qp, kcl, reinterpret<float const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            }
        } elif (kdt == KVDtype.tq4) {
            if (vdt == KVDtype.q8_0) {
                attn_head_flash(qp, reinterpret<TQ4B const?>(kcl), vcl, xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } elif (vdt == KVDtype.tq4) {
                attn_head_flash(qp, reinterpret<TQ4B const?>(kcl), reinterpret<TQ4B const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } elif (vdt == KVDtype.f16) {
                attn_head_flash(qp, reinterpret<TQ4B const?>(kcl), reinterpret<uint16 const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } else {
                attn_head_flash(qp, reinterpret<TQ4B const?>(kcl), reinterpret<float const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            }
        } elif (kdt == KVDtype.f16) {
            if (vdt == KVDtype.q8_0) {
                attn_head_flash(qp, reinterpret<uint16 const?>(kcl), vcl, xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } elif (vdt == KVDtype.tq4) {
                attn_head_flash(qp, reinterpret<uint16 const?>(kcl), reinterpret<TQ4B const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } elif (vdt == KVDtype.f16) {
                attn_head_flash(qp, reinterpret<uint16 const?>(kcl), reinterpret<uint16 const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } else {
                attn_head_flash(qp, reinterpret<uint16 const?>(kcl), reinterpret<float const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            }
        } elif (vdt == KVDtype.q8_0) {
            attn_head_flash(qp, reinterpret<float const?>(kcl), vcl, xbp,
                qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
        } elif (vdt == KVDtype.tq4) {
            attn_head_flash(qp, reinterpret<float const?>(kcl), reinterpret<TQ4B const?>(vcl), xbp,
                qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
        } elif (vdt == KVDtype.f16) {
            attn_head_flash(qp, reinterpret<float const?>(kcl), reinterpret<uint16 const?>(vcl), xbp,
                qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
        } else {
            attn_head_flash(qp, reinterpret<float const?>(kcl), reinterpret<float const?>(vcl), xbp,
                qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, uniform_end, head_size, qd,
                kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
        }
    }
}

def private attn_head_blocked_d(var attp_h : float?; var xbp : float?; qp : float const?; // nolint:STYLE038 — 16-way K x V codec dispatch ladder; each arm is one typed call, nothing to factor
                                qq : int8 const ?; qsp : float const?;
                                kcl, vcl : uint8 const?; kdt, vdt : KVDtype; runs : KVRun const?;
                                nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kv_dim, kvhoff, qoff, hh : int64;
                                scale : float; is_sliding : bool; sliding_window : int64;
                                attn_softcap : float; sinksp : float const?) {
    let kdim = kv_side_off(kdt, kv_dim)
    let khoff = kv_side_off(kdt, kvhoff)
    let vdim = kv_side_off(vdt, kv_dim)
    let vhoff = kv_side_off(vdt, kvhoff)
    unsafe {
        if (kdt == KVDtype.q8_0) {
            if (vdt == KVDtype.q8_0) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, kcl, vcl,
                    runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, kcl, reinterpret<TQ4B const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, kcl, reinterpret<uint16 const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } else {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, kcl, reinterpret<float const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            }
        } elif (kdt == KVDtype.tq4) {
            if (vdt == KVDtype.q8_0) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), vcl,
                    runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<TQ4B const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<uint16 const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } else {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<float const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            }
        } elif (kdt == KVDtype.f16) {
            if (vdt == KVDtype.q8_0) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), vcl,
                    runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<TQ4B const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<uint16 const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } else {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<float const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            }
        } elif (vdt == KVDtype.q8_0) {
            attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), vcl,
                runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        } elif (vdt == KVDtype.tq4) {
            attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<TQ4B const?>(vcl),
                runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        } elif (vdt == KVDtype.f16) {
            attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<uint16 const?>(vcl),
                runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        } else {
            attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<float const?>(vcl),
                runs, nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        }
    }
}

def private attn_head_classic_d(var attp_h : float?; var xbp : float?; qp : float const?; // nolint:STYLE038 — 16-way K x V codec dispatch ladder; each arm is one typed call, nothing to factor
                                qq : int8 const ?; qsp : float const?;
                                kcl, vcl : uint8 const?; kdt, vdt : KVDtype; runs : KVRun const?;
                                nrun, npos, start_pos, uniform_end, qd, head_size, kv_dim, kvhoff, qoff, hh : int64;
                                scale : float; is_sliding : bool; sliding_window : int64;
                                attn_softcap : float; sinksp : float const?) {
    let kdim = kv_side_off(kdt, kv_dim)
    let khoff = kv_side_off(kdt, kvhoff)
    let vdim = kv_side_off(vdt, kv_dim)
    let vhoff = kv_side_off(vdt, kvhoff)
    unsafe {
        if (kdt == KVDtype.q8_0) {
            if (vdt == KVDtype.q8_0) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, kcl, vcl,
                    runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, kcl, reinterpret<TQ4B const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, kcl, reinterpret<uint16 const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } else {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, kcl, reinterpret<float const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            }
        } elif (kdt == KVDtype.tq4) {
            if (vdt == KVDtype.q8_0) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), vcl,
                    runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<TQ4B const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<uint16 const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } else {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<float const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            }
        } elif (kdt == KVDtype.f16) {
            if (vdt == KVDtype.q8_0) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), vcl,
                    runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<TQ4B const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<uint16 const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } else {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<float const?>(vcl),
                    runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            }
        } elif (vdt == KVDtype.q8_0) {
            attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), vcl,
                runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        } elif (vdt == KVDtype.tq4) {
            attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<TQ4B const?>(vcl),
                runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        } elif (vdt == KVDtype.f16) {
            attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<uint16 const?>(vcl),
                runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        } else {
            attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<float const?>(vcl),
                runs, nrun, npos, start_pos, uniform_end, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        }
    }
}

def prefill_attention(var s : Session; kcl, vcl : uint8 const?; npos, start_pos, head_size, qd, kv_dim, // nolint:STYLE038 — one threaded pass over heads: mode select, per-head scratch slicing and the parallel dispatch share hoisted geometry
                      n_heads, kv_mul : int64; scale : float;
                      is_sliding : bool; sliding_window : int64; attn_softcap : float;
                      sinksp : float const?) {           // this layer's per-head sink logits; null = no sinks
    let maxctx = start_pos + npos          // per-head att row width (= max cnt over this call)
    // the non-causal image span (0 = causal); the ONE writer (forward_prefill_embd) sets exactly
    // start_pos + npos for exactly one un-chunked call, so any other value is a foreign write —
    // and an in-range smaller one still drives a negative score count under a sliding window
    let uniform_end = s.attn_uniform_end
    if (uniform_end != 0l && uniform_end != maxctx) {
        panic("prefill_attention: attn_uniform_end {uniform_end} is not this call's span end {maxctx}")
    }
    // narrow batches are decode-shaped: under the prefill work threshold they lose every lane
    let attn_par = npos <= ATTN_NARROW_NPOS || npos * maxctx * n_heads >= g_attn_par_threshold
    let kdt = s.kv_dtype_k
    let vdt = s.kv_dtype_v
    // ONE run list over the layer's widest window; workers intersect their sub-windows with each run
    let kstart_min = (is_sliding && start_pos + 1l > sliding_window) ? (start_pos + 1l - sliding_window) : 0l
    let nrun = kv_runs(s, kstart_min, start_pos + npos, s.kv_runs)
    // block-codec K: quantize every query row once so scores run the SAME int8 dot as decode; flash skips this (f32 tiles)
    if ((kdt == KVDtype.q8_0 || kdt == KVDtype.tq4) && g_attn_prefill_mode != AttnPrefillMode.flash) {
        if (long_length(s.attq) < npos * qd) {
            s.attq |> resize(npos * qd)
            s.atts |> resize(npos * qd / 32l)
        }
        quantize_q8_0_into(s.q_b, npos * qd, s.attq, s.atts, 0l, 0l)
    }
    unsafe {
        let qp = addr < float const? >(s.q_b[0])
        let kq8bc = (kdt == KVDtype.q8_0 || kdt == KVDtype.tq4) && g_attn_prefill_mode != AttnPrefillMode.flash
        let qqp = kq8bc ? addr < int8 const? >(s.attq[0]) : null
        let qsp = kq8bc ? addr < float const? >(s.atts[0]) : null
        let runsp = addr < KVRun const? >(s.kv_runs[0])
        var xbp = addr(s.xb_b[0])
        // att_b is sized only for blocked/classic (flash packs into fa_* instead) — null under flash, never dereferenced
        var attp = !empty(s.att_b) ? addr(s.att_b[0]) : null
        if (g_attn_prefill_mode == AttnPrefillMode.flash) {
            // per-head packing scratch bases (sized in forward_prefill for this mode)
            var qpk0 = addr(s.fa_q[0]);  var kpk0 = addr(s.fa_k[0]);  var vpk0 = addr(s.fa_v[0])
            var kqp0 = addr(s.fa_kq[0]); var vkqp0 = addr(s.fa_vkq[0]); var tmp0 = addr(s.fa_tmp[0])
            maybe_parallel_for(attn_par, 0, int(n_heads), get_dispatch_lanes()) $(rb, re) {
                unsafe {
                    for (h in range(rb, re)) {
                        let hh = int64(h)
                        let qoff = hh * head_size
                        let kvhoff = (hh / kv_mul) * head_size
                        attn_head_flash_d(qp, kcl, vcl, kdt, vdt, xbp,
                            qpk0 + hh * ATTN_FLASH_QT * head_size,
                            kpk0 + hh * head_size * ATTN_FLASH_KV,
                            vpk0 + hh * ATTN_FLASH_KV * head_size,
                            kqp0 + hh * ATTN_FLASH_QT * ATTN_FLASH_KV,
                            vkqp0 + hh * ATTN_FLASH_QT * head_size,
                            tmp0 + hh * head_size, runsp,
                            nrun, npos, start_pos, uniform_end, head_size, qd, kv_dim, kvhoff, qoff,
                            scale, is_sliding, sliding_window, attn_softcap,
                            sinksp != null, sinksp != null ? sinksp[hh] : 0.0)
                    }
                }
            }
        } elif (g_attn_prefill_mode == AttnPrefillMode.blocked) {
            maybe_parallel_for(attn_par, 0, int(n_heads), get_dispatch_lanes()) $(rb, re) {
                unsafe {
                    for (h in range(rb, re)) {
                        let hh = int64(h)
                        let qoff = hh * head_size
                        let kvhoff = (hh / kv_mul) * head_size
                        attn_head_blocked_d(attp + hh * ATTN_BLOCK_Q * maxctx, xbp, qp, qqp, qsp, kcl, vcl, kdt, vdt, runsp,
                            nrun, npos, start_pos, uniform_end, maxctx, qd, head_size, kv_dim, kvhoff, qoff, hh,
                            scale, is_sliding, sliding_window, attn_softcap, sinksp)
                    }
                }
            }
        } else {
            maybe_parallel_for(attn_par, 0, int(n_heads), get_dispatch_lanes()) $(rb, re) {
                unsafe {
                    for (h in range(rb, re)) {
                        let hh = int64(h)
                        let qoff = hh * head_size
                        let kvhoff = (hh / kv_mul) * head_size
                        attn_head_classic_d(attp + hh * maxctx, xbp, qp, qqp, qsp, kcl, vcl, kdt, vdt, runsp,
                            nrun, npos, start_pos, uniform_end, qd, head_size, kv_dim, kvhoff, qoff, hh,
                            scale, is_sliding, sliding_window, attn_softcap, sinksp)
                    }
                }
            }
        }
    }
}
