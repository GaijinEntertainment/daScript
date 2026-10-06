options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options _function_length = 0        // per-kernel gates: fixture + builder dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_codec  // quantize/cvt q8kv + tq4kv rows — the codec fixtures + their dequant truth
require ?das_metal dasllama/dasllama_metal_kernels  // the sq_attn family (Apple builds)
require ?das_metal dasllama/dasllama_metal_common   // the pipeline globals the generated builders read
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // the MetalCell record, check_buf, the device shell
require _attn_oracle
require _compares   // Bar: the cells' bar
require _kernel_oracles   // attn_q_val / attn_q_row: the query rows the decode cells read too
require math

// The single-query attention codec twins (f32 / q8_0 / tq4, single-tg + split-K + comb) vs a
// direct CPU attention oracle. The KV fixtures quantize synthetic f32 rows through the SHIPPED
// codec stores (quantize_q8kv_row / quantize_tq4kv_row) and the oracle attends over the
// DEQUANTIZED values (cvt_*_to_f32) — exactly the numbers the kernel's stream dequant loads,
// so only reduction-order noise separates the sides (the shared 2e-3 bar). Covers per twin:
// sinks, softcap, the sliding window + chlo chunk skip. test_metal_decode_kernels owns the f16
// twins these mirror. Without das_metal in the build, or with no Metal device, every check skips.

let HS = 64                             // head_size == threads per group
let N_HEADS = 4
let KV_MUL = 2                          // 2 kv heads
let KV_DIM = (N_HEADS / KV_MUL) * HS
let QD = N_HEADS * HS
let NBLK = KV_DIM / 32                  // q8/tq4 blocks per KV row
let CHUNK = 64                          // the split-K chunk height the part kernels hardcode

let private BAR_ATTN = Bar(rel = 2e-3, poison_scaled = true)   //! reduction-order noise against the dequant truth; the poisoned last element moves 0.5 of its magnitude floored at 1

// synthetic pre-quant K/V rows
def private kv_val(j, d : int) : float => float((j * 7 + d * 3) % 9 - 4) * 0.25 + 0.03 * float((j + d) % 5)

//! every head's attention row into want[obase + h * HS]: kvbase/qbase place the row inside batch-shared planes; window > 0 starts the keys at cnt - window
def private oracle_all(kdeq, vdeq : array<float>; kvbase : int; q : array<float>; qbase, cnt : int;
                       scale, softcap : float; hass : bool; sink : float; window : int; var want : array<float>; obase : int) {
    let wstart = window > 0 && cnt > window ? cnt - window : 0
    for (h in range(N_HEADS)) {
        attn_row_oracle(q, qbase + h * HS, kdeq, kvbase, vdeq, kvbase, KV_DIM, h, HS, KV_MUL, cnt, scale, want, obase + h * HS,
            wstart, softcap, hass, sink)
    }
}

// Build one codec's K or V fixture: the pre-quant floats, the quantized slab bytes (empty for
// f32), and the dequantized truth the oracle attends over.
def private codec_rows(codec : string; seed, cnt : int; var slab : array<uint8>; var deq : array<float>) {
    var raw : array<float>
    raw |> resize(cnt * KV_DIM)
    for (j in range(cnt)) {
        for (d in range(KV_DIM)) {
            raw[j * KV_DIM + d] = kv_val(j * 3 + seed, d)
        }
    }
    if (codec == "f32") {
        deq <- raw   // nolint:PERF030 — every call site passes a fresh empty local
        return
    }
    let rowb = codec == "q8" ? NBLK * 34 : NBLK * 18
    slab |> resize(cnt * rowb)
    deq |> resize(cnt * KV_DIM)
    unsafe {
        for (j in range(cnt)) {
            if (codec == "q8") {
                quantize_q8kv_row(addr(slab[j * rowb]), addr<float const?>(raw[j * KV_DIM]), int64(KV_DIM))
                cvt_q8kv_to_f32(addr(deq[j * KV_DIM]), addr<uint8 const?>(slab[j * rowb]), int64(KV_DIM))
            } else {
                quantize_tq4kv_row(addr(slab[j * rowb]), addr<float const?>(raw[j * KV_DIM]), int64(KV_DIM))
                cvt_tq4kv_to_f32(addr(deq[j * KV_DIM]), addr<uint8 const?>(slab[j * rowb]), int64(KV_DIM))
            }
        }
    }
    delete raw
}

// the single-threadgroup codec twins: one dispatch, the whole context in one pass
def private sq_codec_single_gate(t : T?; var dev, queue; codec : string; cnt : int; softcap : float; hass : bool) {
    let tag = "sq_attn_{codec} cnt={cnt} softcap={softcap} sink={hass}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let quant = codec != "f32"
        let sp = stamp_of(@@enc_attn32_c)
        let sq = codec == "q8" ? stamp_of(@@enc_attn_q8_c) : stamp_of(@@enc_attn_tq4_c)
        var pso = quant ? (mc |> keep_stamp(sq, err)) : (mc |> keep_stamp(sp, err))
        return if (!pso_ok(t, tag, pso, err))
        var inscope kslab, vslab : array<uint8>
        var inscope kdeq, vdeq : array<float>
        codec_rows(codec, 1, cnt, kslab, kdeq)
        codec_rows(codec, 2, cnt, vslab, vdeq)
        var inscope q <- attn_q_row(QD)
        if (softcap > 0.0) {
            // softcap must be OBSERVABLE: at the base fixture's |raw| <= 0.07 the tanh is identity
            // to 1e-8 and the arm passes with the branch deleted. x300 puts |raw| ~ 21, moving the
            // compared output by 1.4e-2 (measured) against the shared 2e-3 bar.
            for (qv in q) {
                qv *= 300.0
            }
        }
        let sink_v = 0.375
        let scale = 1.0 / sqrt(float(HS))
        var inscope want : array<float>
        want |> resize(QD)
        oracle_all(kdeq, vdeq, 0, q, 0, cnt, scale, softcap, hass, sink_v, 0, want, 0)
        let bk = codec == "f32" ? (mc |> in_(kdeq)) : (mc |> in_plain(kslab))
        let bv = codec == "f32" ? (mc |> in_(vdeq)) : (mc |> in_plain(vslab))
        let bq = mc |> in_(q)
        let by = mc |> out_words(QD, -1000.0)
        let bsink = mc |> in_(plane_of(N_HEADS, sink_v))
        var ka = SqAttnArgs(cnt = uint(cnt), kstride = uint(codec == "f32" ? KV_DIM : NBLK),
            nheads = uint(N_HEADS), kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale,
            softcap = softcap, hass = hass ? 1u : 0u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            let (k, v, qp, y, s) = (mc.buf[bk], mc.buf[bv], mc.buf[bq], mc.buf[by], mc.buf[bsink])
            if (quant) {
                stamp_run(sq, pso) {
                    invoke(sq.enc, enc, k, 0ul, k, v, 0ul, v, qp, y, ka, s, int64(N_HEADS), int64(HS))
                }
            } else {
                stamp_run(sp, pso) {
                    invoke(sp.enc, enc, k, 0ul, v, 0ul, qp, y, ka, s, int64(N_HEADS), int64(HS))
                }
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_ATTN, Shape(row_width = HS))
    }
}

// the split-K pair: codec part kernel writes unnormalized chunk partials, the dtype-agnostic
// comb rescales across chunks; window > 0 also exercises the chunk mask + the chlo skip
def private sq_codec_part_gate(t : T?; var dev, queue; codec : string; cnt, window : int) {
    let tag = "sq_attn_part_{codec} cnt={cnt} window={window}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let quant = codec != "f32"
        let sp = stamp_of(@@enc_attnp32_c)
        let sq = codec == "q8" ? stamp_of(@@enc_attnp_q8_c) : stamp_of(@@enc_attnp_tq4_c)
        let sc = stamp_of(@@enc_attncomb_c)
        var pso_p = quant ? (mc |> keep_stamp(sq, err)) : (mc |> keep_stamp(sp, err))
        let part_ok = pso_ok(t, "{tag} part", pso_p, err)
        var pso_c = mc |> keep_stamp(sc, err)
        return if (!pso_ok(t, "{tag} comb", pso_c, err) || !part_ok)
        var inscope kslab, vslab : array<uint8>
        var inscope kdeq, vdeq : array<float>
        codec_rows(codec, 1, cnt, kslab, kdeq)
        codec_rows(codec, 2, cnt, vslab, vdeq)
        var inscope q <- attn_q_row(QD)
        let scale = 1.0 / sqrt(float(HS))
        let nchunks = (cnt + CHUNK - 1) / CHUNK
        let wstart = window > 0 && cnt > window ? cnt - window : 0
        let chlo = wstart / CHUNK
        var inscope want : array<float>
        want |> resize(QD)
        oracle_all(kdeq, vdeq, 0, q, 0, cnt, scale, 0.0, false, 0.0, window, want, 0)
        let bk = codec == "f32" ? (mc |> in_(kdeq)) : (mc |> in_plain(kslab))
        let bv = codec == "f32" ? (mc |> in_(vdeq)) : (mc |> in_plain(vslab))
        let bq = mc |> in_(q)
        let bpart = mc |> out_words(N_HEADS * nchunks * (HS + 2), -1000.0)
        let by = mc |> out_words(QD, -1000.0)
        let bsink = mc |> in_(plane_of(N_HEADS, 0.0))
        var kap = SqAttnPartArgs(cnt = uint(cnt), kstride = uint(codec == "f32" ? KV_DIM : NBLK),
            nheads = uint(N_HEADS), kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale,
            nchunks = uint(nchunks), softcap = 0.0, window = uint(window))
        var kac = SqAttnCombArgs(nch = uint(nchunks), nheads = uint(N_HEADS), hs = uint(HS),
            chlo = uint(chlo), hass = 0u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            let (k, v, qp, p) = (mc.buf[bk], mc.buf[bv], mc.buf[bq], mc.buf[bpart])
            let ntg = int64(N_HEADS * nchunks)
            if (quant) {
                stamp_run(sq, pso_p) {
                    invoke(sq.enc, enc, k, 0ul, k, v, 0ul, v, qp, p, kap, ntg, int64(HS))
                }
            } else {
                stamp_run(sp, pso_p) {
                    invoke(sp.enc, enc, k, 0ul, v, 0ul, qp, p, kap, ntg, int64(HS))
                }
            }
            stamp_run(sc, pso_c) {
                invoke(sc.enc, enc, p, mc.buf[by], kac, mc.buf[bsink], int64(N_HEADS), int64(HS))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_ATTN, Shape(row_width = HS))
    }
}

// ===== the batched twins: row-table arenas, ragged depths, one dispatch =====

// Per-row fixture into shared K/V arenas: quantized (or f16/f32) rows at the table's bases,
// dequant truth flattened at deq row offsets (roff[r] element base).
struct private BatchAttnFix {
    kslab : array<uint8>        // q8/tq4 K arena bytes (empty otherwise)
    vslab : array<uint8>
    kh : array<float16>         // f16 arenas (empty otherwise)
    vh : array<float16>
    kf : array<float>           // f32 arenas (empty otherwise)
    vf : array<float>
    kdeq : array<float>         // [sum cnt x KV_DIM] dequant truth
    vdeq : array<float>
    roff : array<int>           // per-row element base into kdeq/vdeq
    rt : array<uint4>
    q : array<float>            // [B x qstride] fused rows (q region at h*HS)
}

let private B_LAY = 1
let private B_LAYSTRIDE = 4000      // element arenas (f16/f32)
let private B_LAYSTRIDE_B = 2048    // byte arenas (q8/tq4)

def private build_batch_fix(codec : string; cnts : array<int>; qstride : int; var fx : BatchAttnFix) {
    let nb2 = length(cnts)
    let rowb = codec == "q8" ? NBLK * 34 : NBLK * 18
    // row spacing must clear the deepest slab: cnt <= 200 rows x KV_DIM elems (or rowb bytes)
    let easz = 90000
    let basz = 92160
    if (codec == "f16") {
        fx.kh |> resize(easz)
        fx.vh |> resize(easz)
    } elif (codec == "f32") {
        fx.kf |> resize(easz)
        fx.vf |> resize(easz)
    } else {
        fx.kslab |> resize(basz)
        fx.vslab |> resize(basz)
    }
    fx.rt |> resize(nb2)
    fx.roff |> resize(nb2)
    var total = 0
    for (r in range(nb2)) {
        fx.roff[r] = total * KV_DIM
        total += cnts[r]
    }
    fx.kdeq |> resize(total * KV_DIM)
    fx.vdeq |> resize(total * KV_DIM)
    fx.q |> resize(nb2 * qstride)
    for (i in range(nb2 * qstride)) {
        fx.q[i] = attn_q_val(i % (QD + 13))
    }
    var raw : array<float>
    raw |> resize(KV_DIM)
    unsafe {
        for (r in range(nb2)) {
            let ebase = 500 + r * 27000
            let bbase = 512 + r * 30000
            fx.rt[r] = uint4(uint(codec == "q8" || codec == "tq4" ? bbase : ebase),
                uint((codec == "q8" || codec == "tq4" ? bbase : ebase) + (codec == "q8" || codec == "tq4" ? 0 : 300)),
                uint(codec == "q8" || codec == "tq4" ? B_LAYSTRIDE_B : B_LAYSTRIDE), uint(cnts[r]))
            let kb0 = int(fx.rt[r].x) + B_LAY * int(fx.rt[r].z)
            let vb0 = int(fx.rt[r].y) + B_LAY * int(fx.rt[r].z)
            for (j in range(cnts[r])) {
                for (side in range(2)) {
                    for (d in range(KV_DIM)) {
                        raw[d] = kv_val(j * 3 + r * 7 + side * 11, d)
                    }
                    let dq = fx.roff[r] + j * KV_DIM
                    if (codec == "f16") {
                        for (d in range(KV_DIM)) {
                            let hv = float16(raw[d])
                            if (side == 0) {
                                fx.kh[kb0 + j * KV_DIM + d] = hv
                                fx.kdeq[dq + d] = float(hv)
                            } else {
                                fx.vh[vb0 + j * KV_DIM + d] = hv
                                fx.vdeq[dq + d] = float(hv)
                            }
                        }
                    } elif (codec == "f32") {
                        for (d in range(KV_DIM)) {
                            if (side == 0) {
                                fx.kf[kb0 + j * KV_DIM + d] = raw[d]
                                fx.kdeq[dq + d] = raw[d]
                            } else {
                                fx.vf[vb0 + j * KV_DIM + d] = raw[d]
                                fx.vdeq[dq + d] = raw[d]
                            }
                        }
                    } else {
                        let sb = (side == 0 ? kb0 : vb0) + j * rowb
                        if (side == 0) {
                            if (codec == "q8") {
                                quantize_q8kv_row(addr(fx.kslab[sb]), addr<float const?>(raw[0]), int64(KV_DIM))
                                cvt_q8kv_to_f32(addr(fx.kdeq[dq]), addr<uint8 const?>(fx.kslab[sb]), int64(KV_DIM))
                            } else {
                                quantize_tq4kv_row(addr(fx.kslab[sb]), addr<float const?>(raw[0]), int64(KV_DIM))
                                cvt_tq4kv_to_f32(addr(fx.kdeq[dq]), addr<uint8 const?>(fx.kslab[sb]), int64(KV_DIM))
                            }
                        } else {
                            if (codec == "q8") {
                                quantize_q8kv_row(addr(fx.vslab[sb]), addr<float const?>(raw[0]), int64(KV_DIM))
                                cvt_q8kv_to_f32(addr(fx.vdeq[dq]), addr<uint8 const?>(fx.vslab[sb]), int64(KV_DIM))
                            } else {
                                quantize_tq4kv_row(addr(fx.vslab[sb]), addr<float const?>(raw[0]), int64(KV_DIM))
                                cvt_tq4kv_to_f32(addr(fx.vdeq[dq]), addr<uint8 const?>(fx.vslab[sb]), int64(KV_DIM))
                            }
                        }
                    }
                }
            }
        }
    }
    delete raw
}

def private batch_want(fx : BatchAttnFix; cnts : array<int>; qstride : int; scale, softcap : float; hass : bool; sink : float; window : int) : array<float> {
    let nb2 = length(cnts)
    var want : array<float>
    want |> resize(nb2 * QD)
    for (r in range(nb2)) {
        oracle_all(fx.kdeq, fx.vdeq, fx.roff[r], fx.q, r * qstride, cnts[r], scale, softcap, hass, sink, window, want, r * QD)
    }
    return <- want
}

//! the fixture's K and V arenas as the cell's two read planes: the codec's slab bytes, halves or floats
def private kv_planes(var mc; codec : string; fx : BatchAttnFix) : int2 {
    if (codec == "q8" || codec == "tq4") {
        return int2(mc |> in_plain(fx.kslab), mc |> in_plain(fx.vslab))
    } elif (codec == "f16") {
        return int2(mc |> in_plain(fx.kh), mc |> in_plain(fx.vh))
    }
    return int2(mc |> in_(fx.kf), mc |> in_(fx.vf))
}

def private sq_b_gate(t : T?; var dev, queue; codec : string; cnts : array<int>; softcap : float; hass : bool) {
    let tag = "sq_attn_b_{codec} B={length(cnts)} softcap={softcap} sink={hass}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let quant = codec == "q8" || codec == "tq4"
        let sp = codec == "f16" ? stamp_of(@@enc_attnb16_c) : stamp_of(@@enc_attnb32_c)
        let sq = codec == "q8" ? stamp_of(@@enc_attnb_q8_c) : stamp_of(@@enc_attnb_tq4_c)
        var pso = quant ? (mc |> keep_stamp(sq, err)) : (mc |> keep_stamp(sp, err))
        return if (!pso_ok(t, tag, pso, err))
        let nb2 = length(cnts)
        let qstride = QD + 37
        var inscope fx : BatchAttnFix
        build_batch_fix(codec, cnts, qstride, fx)
        if (softcap > 0.0) {
            // same x300 amplitude boost as the single gate — softcap under the base fixture is identity
            for (qv in fx.q) {
                qv *= 300.0
            }
        }
        let sink_v = 0.375
        let scale = 1.0 / sqrt(float(HS))
        var inscope want <- batch_want(fx, cnts, qstride, scale, softcap, hass, sink_v, 0)
        let kv = mc |> kv_planes(codec, fx)
        let bq = mc |> in_(fx.q)
        let by = mc |> out_words(nb2 * QD, -1000.0)
        let brt = mc |> in_plain(fx.rt)
        let bsink = mc |> in_(plane_of(N_HEADS, sink_v))
        var ka = SqAttnBArgs(lay = uint(B_LAY), kstride = uint(quant ? NBLK : KV_DIM), nheads = uint(N_HEADS),
            kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale, qstride = uint(qstride),
            softcap = softcap, hass = hass ? 1u : 0u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            let (k, v, q, y, rt, s) = (mc.buf[kv.x], mc.buf[kv.y], mc.buf[bq], mc.buf[by], mc.buf[brt], mc.buf[bsink])
            let wgs = int64(N_HEADS * nb2)
            if (quant) {
                stamp_run(sq, pso) {
                    invoke(sq.enc, enc, k, k, v, v, q, y, rt, 0ul, ka, s, wgs, int64(HS))
                }
            } else {
                stamp_run(sp, pso) {
                    invoke(sp.enc, enc, k, v, q, y, rt, 0ul, ka, s, wgs, int64(HS))
                }
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_ATTN, Shape(row_width = QD, head_width = HS))
    }
}

def private sq_partb_gate(t : T?; var dev, queue; codec : string; cnts : array<int>; window : int; hass : bool) {
    let tag = "sq_attn_part_b_{codec} B={length(cnts)} window={window} sink={hass}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let quant = codec == "q8" || codec == "tq4"
        let sp = codec == "f16" ? stamp_of(@@enc_attnpb16_c) : stamp_of(@@enc_attnpb32_c)
        let sq = codec == "q8" ? stamp_of(@@enc_attnpb_q8_c) : stamp_of(@@enc_attnpb_tq4_c)
        let sc = stamp_of(@@enc_attn_comb_b)
        var pso_p = quant ? (mc |> keep_stamp(sq, err)) : (mc |> keep_stamp(sp, err))
        let part_ok = pso_ok(t, "{tag} part", pso_p, err)
        var pso_c = mc |> keep_stamp(sc, err)
        return if (!pso_ok(t, "{tag} comb", pso_c, err) || !part_ok)
        let nb2 = length(cnts)
        let qstride = QD + 37
        var inscope fx : BatchAttnFix
        build_batch_fix(codec, cnts, qstride, fx)
        var cmax = 0
        for (c in cnts) {
            cmax = max(cmax, c)
        }
        let nchmax = (cmax + CHUNK - 1) / CHUNK
        let sink_v = 0.375
        let scale = 1.0 / sqrt(float(HS))
        var inscope want <- batch_want(fx, cnts, qstride, scale, 0.0, hass, sink_v, window)
        let kv = mc |> kv_planes(codec, fx)
        let bq = mc |> in_(fx.q)
        let bpart = mc |> out_words(nb2 * N_HEADS * nchmax * (HS + 2), -1000.0)
        let by = mc |> out_words(nb2 * QD, -1000.0)
        let brt = mc |> in_plain(fx.rt)
        let bsink = mc |> in_(plane_of(N_HEADS, sink_v))
        var kap = SqAttnPartBArgs(lay = uint(B_LAY), kstride = uint(quant ? NBLK : KV_DIM), nheads = uint(N_HEADS),
            kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale, qstride = uint(qstride),
            nchmax = uint(nchmax), softcap = 0.0, window = uint(window))
        var kac = SqAttnCombArgs(nch = uint(nchmax), nheads = uint(N_HEADS), hs = uint(HS),
            window = uint(window), hass = hass ? 1u : 0u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            let (k, v, q, p, rt) = (mc.buf[kv.x], mc.buf[kv.y], mc.buf[bq], mc.buf[bpart], mc.buf[brt])
            let wgs = int64(N_HEADS * nchmax * nb2)
            if (quant) {
                stamp_run(sq, pso_p) {
                    invoke(sq.enc, enc, k, k, v, v, q, p, rt, 0ul, kap, wgs, int64(HS))
                }
            } else {
                stamp_run(sp, pso_p) {
                    invoke(sp.enc, enc, k, v, q, p, rt, 0ul, kap, wgs, int64(HS))
                }
            }
            stamp_run(sc, pso_c) {
                invoke(sc.enc, enc, p, mc.buf[by], kac, rt, 0ul, mc.buf[bsink], int64(N_HEADS * nb2), int64(HS))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_ATTN, Shape(row_width = QD, head_width = HS))
    }
}

// ===== the fused single-pass "D" family (hs == 128): part + parallel cross-slice comb =====
// D reuses build_batch_fix: nheads=2, kv_mul=2, hs=128 gives the SAME kv_dim/qd as the hs=64
// shape, so only the oracle's head geometry differs. D is plain causal (no window/softcap/sink).

let private D_HS = 128
let private D_NHEADS = 2
let private D_KVMUL = 2
let private D_CAP = 0.05   //! a logit cap inside the fixture's score range: an uncapped kernel lands outside the bar

//! `lead` route rows sit ahead of the cell's own and both kernels bind the table past them (the verify's layer offset):
//! each is the first stream's slab at ONE key, so a kernel reading the table from its start lands outside the bar
//! A head of 256 (the WIDE stamps, float codecs) reads the same slab as ONE head over rows twice as wide: each stream holds
//! `cnts / 2` keys of 256, and the combine takes the partials as two heads of 128. A head of 512 reads it as rows four times
//! as wide - `cnts / 4` keys, four heads of 128 to the combine - over a query row as wide as the head.
//! `window` keys of sliding span (0 = the whole context) and, under `sinks`, a sink logit a head that differs head to head.
//! `softcap` > 0 caps every logit; the ragged counts leave lanes past a slice's rows, which must still weigh nothing under it.
def private sq_d_gate(t : T?; var dev, queue; codec : string; cnts : array<int>; nsgs : int; hs : int = D_HS; lead : int = 0;
                      window : int = 0; sinks : bool = false; softcap : float = 0.0) {
    let halves = hs > D_HS ? hs / D_HS : 1
    let wide = halves > 1
    let rowx = wide ? halves : (D_HS % hs == 0 ? 1 : 3)   //! fixture rows read as ONE kernel row: a wide head's halves, three for a head of 96 (four K/V heads of 96)
    let kvdim = rowx * KV_DIM
    let kvmul = wide ? 1 : D_KVMUL
    let nheads = wide ? 1 : (kvdim / hs) * kvmul   //! a narrower head keeps the slab's bytes: more heads over the same rows
    let qd_row = max(QD, nheads * hs)
    let tag = "sq_attn_d_{codec} B={length(cnts)} nsgs={nsgs} hs={hs} lead={lead} window={window} sinks={sinks} cap={softcap}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let quant = codec == "q8" || codec == "tq4"
        let sp = (halves == 4 ? (codec == "f16" ? stamp_of(@@enc_attndx16_c) : stamp_of(@@enc_attndx32_c))
            : wide ? (codec == "f16" ? stamp_of(@@enc_attndw16_c) : stamp_of(@@enc_attndw32_c))
            : (codec == "f16" ? stamp_of(@@enc_attnd16_c) : stamp_of(@@enc_attnd32_c)))
        let sq = (halves == 4 ? (codec == "q8" ? stamp_of(@@enc_attndx_q8_c) : stamp_of(@@enc_attndx_tq4_c))
            : wide ? (codec == "q8" ? stamp_of(@@enc_attndw_q8_c) : stamp_of(@@enc_attndw_tq4_c))
            : (codec == "q8" ? stamp_of(@@enc_attnd_q8_c) : stamp_of(@@enc_attnd_tq4_c)))
        let sc = stamp_of(@@enc_attncombd_c)
        var pso_d = quant ? (mc |> keep_stamp(sq, err)) : (mc |> keep_stamp(sp, err))
        let part_ok = pso_ok(t, "{tag} part", pso_d, err)
        var pso_c = mc |> keep_stamp(sc, err)
        return if (!pso_ok(t, "{tag} comb", pso_c, err) || !part_ok)
        let nb2 = length(cnts)
        let qstride = qd_row + 36               // q reads through a float4 view — keep the stride % 4
        var inscope fx : BatchAttnFix
        build_batch_fix(codec, cnts, qstride, fx)
        let scale = 1.0 / sqrt(float(hs))
        var inscope want : array<float>
        want |> resize(nb2 * qd_row)
        var inscope snk <- [for (h in range(nheads)); 0.375 + 0.25 * float(h)]
        for (r in range(nb2)) {
            let keys = cnts[r] / rowx
            let kstart = window > 0 && keys > window ? keys - window : 0
            for (h in range(nheads)) {
                attn_row_oracle(fx.q, r * qstride + h * hs, fx.kdeq, fx.roff[r], fx.vdeq, fx.roff[r], kvdim, h, hs, kvmul, keys, scale,
                    want, r * qd_row + h * hs, kstart, softcap, sinks, snk[h])
            }
        }
        let bsink = mc |> in_(snk)
        let kv = mc |> kv_planes(codec, fx)
        let bq = mc |> in_(fx.q)
        let bpart = mc |> out_words(nb2 * nheads * halves * nsgs * (hs / halves + 4), -1000.0)
        let by = mc |> out_words(nb2 * qd_row, -1000.0)
        var inscope routes <- [for (_ in range(lead)); uint4(fx.rt[0].x, fx.rt[0].y, fx.rt[0].z, 1u)]
        routes |> push_from([for (row in fx.rt); uint4(row.x, row.y, row.z, row.w / uint(rowx))])
        let brt = mc |> in_plain(routes)
        let rtoff = uint64(lead * 16)
        var kad = SqAttnDArgs(lay = uint(B_LAY), kstride = uint(quant ? NBLK * rowx : kvdim), nheads = uint(nheads),
            kv_mul = uint(kvmul), hs = uint(hs), scale = scale, qstride = uint(qstride), nsgs = uint(nsgs), window = uint(window),
            softcap = softcap)
        var kac = SqAttnCombDArgs(nchmax = uint(nsgs), nheads = uint(nheads * halves), hs = uint(hs / halves),
            hass = sinks ? 1u : 0u, hdiv = uint(halves))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            let (k, v, q, p, rt) = (mc.buf[kv.x], mc.buf[kv.y], mc.buf[bq], mc.buf[bpart], mc.buf[brt])
            let (nh, ns, nr) = (int64(nheads), int64(nsgs), int64(nb2))
            if (quant) {
                stamp_run(sq, pso_d) {
                    invoke(sq.enc, enc, k, k, v, v, q, p, rt, rtoff, kad, nh, ns, nr)
                }
            } else {
                stamp_run(sp, pso_d) {
                    invoke(sp.enc, enc, k, v, q, p, rt, rtoff, kad, nh, ns, nr)
                }
            }
            stamp_run(sc, pso_c) {
                invoke(sc.enc, enc, p, mc.buf[by], rt, rtoff, kac, mc.buf[bsink], nh * int64(halves), nr)
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_ATTN, Shape(row_width = qd_row, head_width = hs))
    }
}

[test]
def test_metal_attn_codec_kernels(t : T?) {
    t |> run("sq_attn codec twins (f32/q8/tq4, single + split-K) == CPU attention over the dequant truth") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_gate(t) $(var dev : MetalDevice?; var queue : MetalCommandQueue?) {
                sq_codec_single_gate(t, dev, queue, "f32", 40, 0.0, false)
                sq_codec_single_gate(t, dev, queue, "f32", 40, 0.0, true)
                sq_codec_single_gate(t, dev, queue, "q8", 40, 0.0, false)
                sq_codec_single_gate(t, dev, queue, "q8", 40, D_CAP, false)
                sq_codec_single_gate(t, dev, queue, "tq4", 40, 0.0, false)
                sq_codec_single_gate(t, dev, queue, "tq4", 40, 0.0, true)
                sq_codec_part_gate(t, dev, queue, "f32", 100, 0)
                sq_codec_part_gate(t, dev, queue, "f32", 100, 48)
                sq_codec_part_gate(t, dev, queue, "q8", 100, 0)
                sq_codec_part_gate(t, dev, queue, "q8", 200, 80)
                sq_codec_part_gate(t, dev, queue, "tq4", 100, 0)
                sq_codec_part_gate(t, dev, queue, "tq4", 100, 48)
                // batched twins: ragged depths through the row table, one dispatch
                sq_b_gate(t, dev, queue, "f16", [21, 64, 127], 0.0, false)
                sq_b_gate(t, dev, queue, "f16", [30, 90], 0.0, true)
                sq_b_gate(t, dev, queue, "f32", [21, 64, 127], 0.0, false)
                sq_b_gate(t, dev, queue, "q8", [21, 64, 127], 0.0, false)
                sq_b_gate(t, dev, queue, "q8", [30, 90], D_CAP, false)
                sq_b_gate(t, dev, queue, "tq4", [21, 64, 127], 0.0, false)
                // batched split-K + the batched comb (per-row chunk counts, window, sinks)
                sq_partb_gate(t, dev, queue, "f16", [30, 100, 200], 0, false)
                sq_partb_gate(t, dev, queue, "f16", [30, 100, 200], 80, false)
                sq_partb_gate(t, dev, queue, "f32", [30, 100, 200], 0, true)
                sq_partb_gate(t, dev, queue, "q8", [30, 100, 200], 0, false)
                sq_partb_gate(t, dev, queue, "q8", [64, 200], 80, false)
                sq_partb_gate(t, dev, queue, "tq4", [30, 100, 200], 0, false)
                sq_partb_gate(t, dev, queue, "tq4", [30, 100, 200], 80, true)
                // the fused single-pass D family (hs = 128): slice-group counts off the 4-sg grid
                sq_d_gate(t, dev, queue, "f16", [21, 64, 200], 3)
                sq_d_gate(t, dev, queue, "f32", [40, 130], 5)
                sq_d_gate(t, dev, queue, "f16", [21, 64, 200], 3, hs = 64)
                sq_d_gate(t, dev, queue, "f32", [40, 130], 5, hs = 64)
                sq_d_gate(t, dev, queue, "f16", [21, 64, 200], 3, hs = 64, lead = 2)
                sq_d_gate(t, dev, queue, "f16", [42, 128, 200], 3, hs = 256)   //! the wide stamps: 21, 64 and 100 keys of a 256 head, the last past the slice groups
                sq_d_gate(t, dev, queue, "f32", [80, 132], 5, hs = 256)
                sq_d_gate(t, dev, queue, "f16", [42, 128, 200], 2, hs = 256, lead = 2)
                sq_d_gate(t, dev, queue, "q8", [21, 64, 200], 3, lead = 3)
                sq_d_gate(t, dev, queue, "f16", [21, 64, 200], 3, hs = 64, window = 40, sinks = true)
                sq_d_gate(t, dev, queue, "f16", [21, 64, 200], 5, window = 100)
                sq_d_gate(t, dev, queue, "f32", [40, 130], 5, hs = 64, sinks = true)
                sq_d_gate(t, dev, queue, "f16", [42, 128, 200], 3, hs = 256, window = 30, sinks = true)
                sq_d_gate(t, dev, queue, "q8", [21, 64, 200], 3, window = 40, sinks = true)
                sq_d_gate(t, dev, queue, "tq4", [30, 100], 2, lead = 1, window = 300)
                sq_d_gate(t, dev, queue, "f16", [84, 128, 200], 3, hs = 512)   //! the head-of-512 stamps: 21, 32 and 50 keys, the last past the slice groups
                sq_d_gate(t, dev, queue, "f32", [160, 132], 2, hs = 512)
                sq_d_gate(t, dev, queue, "f16", [84, 128, 200], 2, hs = 512, lead = 2, window = 30, sinks = true)
                sq_d_gate(t, dev, queue, "f16", [84, 128, 200], 3, hs = 512, softcap = D_CAP)
                sq_d_gate(t, dev, queue, "f16", [21, 64, 200], 3, softcap = D_CAP)
                sq_d_gate(t, dev, queue, "f16", [21, 64, 200], 3, hs = 64, window = 40, sinks = true, softcap = D_CAP)
                sq_d_gate(t, dev, queue, "f32", [40, 130], 5, softcap = D_CAP)
                sq_d_gate(t, dev, queue, "f16", [42, 128, 200], 3, hs = 256, softcap = D_CAP)
                sq_d_gate(t, dev, queue, "q8", [21, 64, 200], 3, softcap = D_CAP)
                sq_d_gate(t, dev, queue, "tq4", [30, 100], 2, softcap = D_CAP)
                sq_d_gate(t, dev, queue, "q8", [21, 64, 200], 3)
                sq_d_gate(t, dev, queue, "tq4", [30, 100], 2)
                sq_d_gate(t, dev, queue, "q8", [21, 64, 200], 3, hs = 64)
                sq_d_gate(t, dev, queue, "tq4", [30, 100], 2, hs = 64, window = 40, sinks = true)
                sq_d_gate(t, dev, queue, "q8", [42, 128, 200], 3, hs = 256)
                sq_d_gate(t, dev, queue, "q8", [42, 128, 200], 2, hs = 256, lead = 2, window = 30, sinks = true, softcap = D_CAP)
                sq_d_gate(t, dev, queue, "tq4", [60, 200], 2, hs = 256)
                sq_d_gate(t, dev, queue, "q8", [84, 128, 200], 3, hs = 512)
                sq_d_gate(t, dev, queue, "tq4", [120, 200], 2, hs = 512, lead = 1, window = 30, sinks = true)
                sq_d_gate(t, dev, queue, "f32", [80, 132], 3, hs = 256, window = 30, sinks = true, softcap = D_CAP)    // the wide and quant wide stamps under a window and the cap
                sq_d_gate(t, dev, queue, "f32", [160, 132], 2, hs = 512, window = 20, softcap = D_CAP)
                sq_d_gate(t, dev, queue, "tq4", [60, 200], 2, hs = 256, window = 30, sinks = true, softcap = D_CAP)
                sq_d_gate(t, dev, queue, "q8", [84, 128, 200], 2, hs = 512, lead = 1, window = 20, softcap = D_CAP)
                sq_d_gate(t, dev, queue, "tq4", [120, 200], 2, hs = 512, softcap = D_CAP)
                sq_d_gate(t, dev, queue, "f16", [21, 63, 198], 3, hs = 96)   //! a head of 96 (phi-3): 24 lanes own a quad, three 32-element blocks a head on the block codecs
                sq_d_gate(t, dev, queue, "f32", [60, 198], 3, hs = 96, lead = 1, window = 20, sinks = true, softcap = D_CAP)
                sq_d_gate(t, dev, queue, "q8", [21, 63, 198], 3, hs = 96)
                sq_d_gate(t, dev, queue, "tq4", [60, 198], 2, hs = 96, window = 30, sinks = true)
            }
        } else {
            t |> skip("das_metal is not built on this platform; attention codec kernel checks skipped")
        }
    }
}
