options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options _function_length = 0        // per-kernel gates: hand-bound buffers/uniforms + dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes

require dastest/testing_boost public
require dasllama/dasllama_kv_codec  // quantize/cvt q8kv + tq4kv rows — the codec fixtures + their dequant truth
require ?das_metal dasllama/dasllama_metal_kernels  // the sq_attn family (Apple builds)
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // shared buf helpers + eyeball-dump compares
require math

// The single-query attention codec twins (f32 / q8_0 / tq4, single-tg + split-K + comb) vs a
// direct CPU attention oracle. The KV fixtures quantize synthetic f32 rows through the SHIPPED
// codec stores (quantize_q8kv_row / quantize_tq4kv_row) and the oracle attends over the
// DEQUANTIZED values (cvt_*_to_f32) — exactly the numbers the kernel's stream dequant loads,
// so only reduction-order noise separates the sides (the shared 2e-3 bar). Covers per twin:
// sinks, softcap, the sliding window + chlo chunk skip. test_metal_decode_kernels owns the f16
// twins these mirror. Off-Apple (or with no Metal device) every check feints cleanly.

let HS = 64                             // head_size == threads per group
let N_HEADS = 4
let KV_MUL = 2                          // 2 kv heads
let KV_DIM = (N_HEADS / KV_MUL) * HS
let QD = N_HEADS * HS
let NBLK = KV_DIM / 32                  // q8/tq4 blocks per KV row
let CHUNK = 64                          // the split-K chunk height the part kernels hardcode

// synthetic pre-quant K/V and q rows
def private kv_val(j, d : int) : float => float((j * 7 + d * 3) % 9 - 4) * 0.25 + 0.03 * float((j + d) % 5)

def private q_val(i : int) : float => float((i * 5) % 11 - 5) * 0.125

// causal single-query attention over dequantized K/V planes for one head; kvbase/qbase place
// the row inside batch-shared planes; window > 0 masks rows below cnt - window; sink > -1e30
// folds an attention sink into max and denominator
def private attn_oracle(kdeq, vdeq : array<float>; kvbase : int; q : array<float>; qbase, h, cnt : int;
                        scale, softcap, sink : float; window : int) : array<float> {
    let khoff = (h / KV_MUL) * HS
    let wstart = window > 0 && cnt > window ? cnt - window : 0
    var sc : array<float>
    sc |> resize(cnt)
    for (j in range(cnt)) {
        if (j < wstart) {
            sc[j] = -3.0e38
            continue
        }
        var s = 0.0
        for (d in range(HS)) {
            s += q[qbase + h * HS + d] * kdeq[kvbase + j * KV_DIM + khoff + d]
        }
        let raw = s * scale
        sc[j] = softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(raw * 2.0 / softcap))) : raw
    }
    var mm = -3.0e38
    for (j in range(cnt)) {
        mm = max(mm, sc[j])
    }
    mm = max(mm, sink)
    var tot = 0.0
    for (j in range(cnt)) {
        sc[j] = exp(sc[j] - mm)
        tot += sc[j]
    }
    if (sink > -1.0e30) {
        tot += exp(max(sink - mm, -80.0))
    }
    var out : array<float>
    out |> resize(HS)
    for (d in range(HS)) {
        var acc = 0.0
        for (j in range(cnt)) {
            acc += sc[j] * vdeq[kvbase + j * KV_DIM + khoff + d]
        }
        out[d] = acc / tot
    }
    delete sc
    return <- out
}

def private oracle_all(kdeq, vdeq : array<float>; kvbase : int; q : array<float>; qbase, cnt : int;
                       scale, softcap, sink : float; window : int) : array<float> {
    var want : array<float>
    want |> resize(QD)
    for (h in range(N_HEADS)) {
        var col <- attn_oracle(kdeq, vdeq, kvbase, q, qbase, h, cnt, scale, softcap, sink, window)
        for (d in range(HS)) {
            want[h * HS + d] = col[d]
        }
        delete col
    }
    return <- want
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

def private q_rows : array<float> {
    var q : array<float>
    q |> resize(QD)
    for (i in range(QD)) {
        q[i] = q_val(i)
    }
    return <- q
}

// the single-threadgroup codec twins: one dispatch, the whole context in one pass
def private sq_codec_single_gate(t : T?; dev, queue; codec : string; cnt : int; softcap : float; hass : bool) {
    let tag = "sq_attn_{codec} cnt={cnt} softcap={softcap} sink={hass}"
    var err : string
    let src = codec == "f32" ? MetalSqAttnF32_metal_sq_attn_msl : (codec == "q8" ? MetalSqAttnQ8_metal_sq_attn_msl : MetalSqAttnTq4_metal_sq_attn_msl)
    let entry = codec == "f32" ? MetalSqAttnF32_metal_sq_attn_msl_entry : (codec == "q8" ? MetalSqAttnQ8_metal_sq_attn_msl_entry : MetalSqAttnTq4_metal_sq_attn_msl_entry)
    let fm = codec == "f32" ? MetalSqAttnF32_metal_sq_attn_msl_fastmath : (codec == "q8" ? MetalSqAttnQ8_metal_sq_attn_msl_fastmath : MetalSqAttnTq4_metal_sq_attn_msl_fastmath)
    let tgm = codec == "f32" ? MetalSqAttnF32_metal_sq_attn_msl_tgmem : (codec == "q8" ? MetalSqAttnQ8_metal_sq_attn_msl_tgmem : MetalSqAttnTq4_metal_sq_attn_msl_tgmem)
    var pso = pipeline_from_source(dev, src, entry, fm, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var kslab : array<uint8>
    var vslab : array<uint8>
    var kdeq : array<float>
    var vdeq : array<float>
    codec_rows(codec, 1, cnt, kslab, kdeq)
    codec_rows(codec, 2, cnt, vslab, vdeq)
    var q <- q_rows()
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
    var want <- oracle_all(kdeq, vdeq, 0, q, 0, cnt, scale, softcap, hass ? sink_v : -3.0e38, 0)
    var bk = codec == "f32" ? buf_upload(dev, kdeq) : buf_upload(dev, kslab)
    var bv = codec == "f32" ? buf_upload(dev, vdeq) : buf_upload(dev, vslab)
    var bq = buf_upload(dev, q)
    var by = buf_fill(dev, QD, -1000.0)
    var bsink = buf_fill(dev, N_HEADS, sink_v)
    var ka = SqAttnArgs(cnt = uint(cnt), kstride = uint(codec == "f32" ? KV_DIM : NBLK),
        nheads = uint(N_HEADS), kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale,
        softcap = softcap, hass = hass ? 1u : 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, tgm, 0)
        if (codec == "f32") {
            metal_set_buffer(enc, bk, 0ul, 0)
            metal_set_buffer(enc, bv, 0ul, 1)
            metal_set_buffer(enc, bq, 0ul, 2)
            metal_set_buffer(enc, by, 0ul, 3)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
        } else {
            metal_set_buffer(enc, bk, 0ul, 0)
            metal_set_buffer(enc, bk, 0ul, 1)
            metal_set_buffer(enc, bv, 0ul, 2)
            metal_set_buffer(enc, bv, 0ul, 3)
            metal_set_buffer(enc, bq, 0ul, 4)
            metal_set_buffer(enc, by, 0ul, 5)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 6)
        }
        metal_set_buffer(enc, bsink, 0ul, 20)
        metal_dispatch_threadgroups(enc, uint3(uint(N_HEADS), 1u, 1u), uint3(uint(HS), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_bars(by, want, 2e-3, 0.0, tag), 0)
    }
    metal_release(bk)
    metal_release(bv)
    metal_release(bq)
    metal_release(by)
    metal_release(bsink)
    metal_release(pso)
    delete kslab
    delete vslab
    delete kdeq
    delete vdeq
    delete q
    delete want
}

// the split-K pair: codec part kernel writes unnormalized chunk partials, the dtype-agnostic
// comb rescales across chunks; window > 0 also exercises the chunk mask + the chlo skip
def private sq_codec_part_gate(t : T?; dev, queue; codec : string; cnt, window : int) {
    let tag = "sq_attn_part_{codec} cnt={cnt} window={window}"
    var err : string
    let src = codec == "f32" ? MetalSqAttnPartF32_metal_sq_attn_part_msl : (codec == "q8" ? MetalSqAttnPartQ8_metal_sq_attn_part_msl : MetalSqAttnPartTq4_metal_sq_attn_part_msl)
    let entry = codec == "f32" ? MetalSqAttnPartF32_metal_sq_attn_part_msl_entry : (codec == "q8" ? MetalSqAttnPartQ8_metal_sq_attn_part_msl_entry : MetalSqAttnPartTq4_metal_sq_attn_part_msl_entry)
    let fm = codec == "f32" ? MetalSqAttnPartF32_metal_sq_attn_part_msl_fastmath : (codec == "q8" ? MetalSqAttnPartQ8_metal_sq_attn_part_msl_fastmath : MetalSqAttnPartTq4_metal_sq_attn_part_msl_fastmath)
    let tgm = codec == "f32" ? MetalSqAttnPartF32_metal_sq_attn_part_msl_tgmem : (codec == "q8" ? MetalSqAttnPartQ8_metal_sq_attn_part_msl_tgmem : MetalSqAttnPartTq4_metal_sq_attn_part_msl_tgmem)
    var pso_p = pipeline_from_source(dev, src, entry, fm, err)
    t |> success(pso_p != null, "{tag}: part pipeline: {err}")
    var pso_c = pipeline_from_source(dev, MetalSqAttnComb_metal_sq_attn_comb_msl, MetalSqAttnComb_metal_sq_attn_comb_msl_entry, MetalSqAttnComb_metal_sq_attn_comb_msl_fastmath, err)
    t |> success(pso_c != null, "{tag}: comb pipeline: {err}")
    if (pso_p == null || pso_c == null) {
        metal_release(pso_p)
        metal_release(pso_c)
        return
    }
    var kslab : array<uint8>
    var vslab : array<uint8>
    var kdeq : array<float>
    var vdeq : array<float>
    codec_rows(codec, 1, cnt, kslab, kdeq)
    codec_rows(codec, 2, cnt, vslab, vdeq)
    var q <- q_rows()
    let scale = 1.0 / sqrt(float(HS))
    let nchunks = (cnt + CHUNK - 1) / CHUNK
    let wstart = window > 0 && cnt > window ? cnt - window : 0
    let chlo = wstart / CHUNK
    var want <- oracle_all(kdeq, vdeq, 0, q, 0, cnt, scale, 0.0, -3.0e38, window)
    var bk = codec == "f32" ? buf_upload(dev, kdeq) : buf_upload(dev, kslab)
    var bv = codec == "f32" ? buf_upload(dev, vdeq) : buf_upload(dev, vslab)
    var bq = buf_upload(dev, q)
    var bpart = buf_fill(dev, N_HEADS * nchunks * (HS + 2), -1000.0)
    var by = buf_fill(dev, QD, -1000.0)
    var bsink = buf_fill(dev, N_HEADS, 0.0)
    var kap = SqAttnPartArgs(cnt = uint(cnt), kstride = uint(codec == "f32" ? KV_DIM : NBLK),
        nheads = uint(N_HEADS), kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale,
        nchunks = uint(nchunks), softcap = 0.0, window = uint(window))
    var kac = SqAttnCombArgs(nch = uint(nchunks), nheads = uint(N_HEADS), hs = uint(HS),
        chlo = uint(chlo), hass = 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_p)
        metal_set_threadgroup_memory_length(enc, tgm, 0)
        if (codec == "f32") {
            metal_set_buffer(enc, bk, 0ul, 0)
            metal_set_buffer(enc, bv, 0ul, 1)
            metal_set_buffer(enc, bq, 0ul, 2)
            metal_set_buffer(enc, bpart, 0ul, 3)
            metal_set_bytes(enc, unsafe(addr<void?>(kap)), uint64(typeinfo sizeof(kap)), 4)
        } else {
            metal_set_buffer(enc, bk, 0ul, 0)
            metal_set_buffer(enc, bk, 0ul, 1)
            metal_set_buffer(enc, bv, 0ul, 2)
            metal_set_buffer(enc, bv, 0ul, 3)
            metal_set_buffer(enc, bq, 0ul, 4)
            metal_set_buffer(enc, bpart, 0ul, 5)
            metal_set_bytes(enc, unsafe(addr<void?>(kap)), uint64(typeinfo sizeof(kap)), 6)
        }
        metal_dispatch_threadgroups(enc, uint3(uint(N_HEADS * nchunks), 1u, 1u), uint3(uint(HS), 1u, 1u))
        metal_set_pipeline(enc, pso_c)
        metal_set_buffer(enc, bpart, 0ul, 0)
        metal_set_buffer(enc, by, 0ul, 1)
        metal_set_bytes(enc, unsafe(addr<void?>(kac)), uint64(typeinfo sizeof(kac)), 2)
        metal_set_buffer(enc, bsink, 0ul, 20)
        metal_dispatch_threadgroups(enc, uint3(uint(N_HEADS), 1u, 1u), uint3(uint(HS), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_bars(by, want, 2e-3, 0.0, tag), 0)
    }
    metal_release(bk)
    metal_release(bv)
    metal_release(bq)
    metal_release(bpart)
    metal_release(by)
    metal_release(bsink)
    metal_release(pso_p)
    metal_release(pso_c)
    delete kslab
    delete vslab
    delete kdeq
    delete vdeq
    delete q
    delete want
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
        fx.q[i] = q_val(i % (QD + 13))
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

def private batch_want(fx : BatchAttnFix; cnts : array<int>; qstride : int; scale, softcap, sink : float; window : int) : array<float> {
    let nb2 = length(cnts)
    var want : array<float>
    want |> resize(nb2 * QD)
    for (r in range(nb2)) {
        var row <- oracle_all(fx.kdeq, fx.vdeq, fx.roff[r], fx.q, r * qstride, cnts[r], scale, softcap, sink, window)
        for (i in range(QD)) {
            want[r * QD + i] = row[i]
        }
        delete row
    }
    return <- want
}

def private sq_b_gate(t : T?; dev, queue; codec : string; cnts : array<int>; softcap : float; hass : bool) {
    let tag = "sq_attn_b_{codec} B={length(cnts)} softcap={softcap} sink={hass}"
    var err : string
    let src = codec == "f16" ? MetalSqAttnBF16_metal_sq_attn_b_msl : (codec == "f32" ? MetalSqAttnBF32_metal_sq_attn_b_msl : (codec == "q8" ? MetalSqAttnBQ8_metal_sq_attn_b_msl : MetalSqAttnBTq4_metal_sq_attn_b_msl))
    let entry = codec == "f16" ? MetalSqAttnBF16_metal_sq_attn_b_msl_entry : (codec == "f32" ? MetalSqAttnBF32_metal_sq_attn_b_msl_entry : (codec == "q8" ? MetalSqAttnBQ8_metal_sq_attn_b_msl_entry : MetalSqAttnBTq4_metal_sq_attn_b_msl_entry))
    let fm = codec == "f16" ? MetalSqAttnBF16_metal_sq_attn_b_msl_fastmath : (codec == "f32" ? MetalSqAttnBF32_metal_sq_attn_b_msl_fastmath : (codec == "q8" ? MetalSqAttnBQ8_metal_sq_attn_b_msl_fastmath : MetalSqAttnBTq4_metal_sq_attn_b_msl_fastmath))
    let tgm = codec == "f16" ? MetalSqAttnBF16_metal_sq_attn_b_msl_tgmem : (codec == "f32" ? MetalSqAttnBF32_metal_sq_attn_b_msl_tgmem : (codec == "q8" ? MetalSqAttnBQ8_metal_sq_attn_b_msl_tgmem : MetalSqAttnBTq4_metal_sq_attn_b_msl_tgmem))
    var pso = pipeline_from_source(dev, src, entry, fm, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nb2 = length(cnts)
    let qstride = QD + 37
    let quant = codec == "q8" || codec == "tq4"
    var fx : BatchAttnFix
    build_batch_fix(codec, cnts, qstride, fx)
    if (softcap > 0.0) {
        // same x300 amplitude boost as the single gate — softcap under the base fixture is identity
        for (qv in fx.q) {
            qv *= 300.0
        }
    }
    let sink_v = 0.375
    let scale = 1.0 / sqrt(float(HS))
    var want <- batch_want(fx, cnts, qstride, scale, softcap, hass ? sink_v : -3.0e38, 0)
    var bk = quant ? buf_upload(dev, fx.kslab) : (codec == "f16" ? buf_upload(dev, fx.kh) : buf_upload(dev, fx.kf))
    var bv = quant ? buf_upload(dev, fx.vslab) : (codec == "f16" ? buf_upload(dev, fx.vh) : buf_upload(dev, fx.vf))
    var bq = buf_upload(dev, fx.q)
    var by = buf_fill(dev, nb2 * QD, -1000.0)
    var brt = buf_upload(dev, fx.rt)
    var bsink = buf_fill(dev, N_HEADS, sink_v)
    var ka = SqAttnBArgs(lay = uint(B_LAY), kstride = uint(quant ? NBLK : KV_DIM), nheads = uint(N_HEADS),
        kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale, qstride = uint(qstride),
        softcap = softcap, hass = hass ? 1u : 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, tgm, 0)
        if (quant) {
            metal_set_buffer(enc, bk, 0ul, 0)
            metal_set_buffer(enc, bk, 0ul, 1)
            metal_set_buffer(enc, bv, 0ul, 2)
            metal_set_buffer(enc, bv, 0ul, 3)
            metal_set_buffer(enc, bq, 0ul, 4)
            metal_set_buffer(enc, by, 0ul, 5)
            metal_set_buffer(enc, brt, 0ul, 6)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 7)
        } else {
            metal_set_buffer(enc, bk, 0ul, 0)
            metal_set_buffer(enc, bv, 0ul, 1)
            metal_set_buffer(enc, bq, 0ul, 2)
            metal_set_buffer(enc, by, 0ul, 3)
            metal_set_buffer(enc, brt, 0ul, 4)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        }
        metal_set_buffer(enc, bsink, 0ul, 20)
        metal_dispatch_threadgroups(enc, uint3(uint(N_HEADS * nb2), 1u, 1u), uint3(uint(HS), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_bars(by, want, 2e-3, 0.0, tag), 0)
    }
    metal_release(bk)
    metal_release(bv)
    metal_release(bq)
    metal_release(by)
    metal_release(brt)
    metal_release(bsink)
    metal_release(pso)
    delete fx
    delete want
}

def private sq_partb_gate(t : T?; dev, queue; codec : string; cnts : array<int>; window : int; hass : bool) {
    let tag = "sq_attn_part_b_{codec} B={length(cnts)} window={window} sink={hass}"
    var err : string
    let src = codec == "f16" ? MetalSqAttnPartBF16_metal_sq_attn_part_b_msl : (codec == "f32" ? MetalSqAttnPartBF32_metal_sq_attn_part_b_msl : (codec == "q8" ? MetalSqAttnPartBQ8_metal_sq_attn_part_b_msl : MetalSqAttnPartBTq4_metal_sq_attn_part_b_msl))
    let entry = codec == "f16" ? MetalSqAttnPartBF16_metal_sq_attn_part_b_msl_entry : (codec == "f32" ? MetalSqAttnPartBF32_metal_sq_attn_part_b_msl_entry : (codec == "q8" ? MetalSqAttnPartBQ8_metal_sq_attn_part_b_msl_entry : MetalSqAttnPartBTq4_metal_sq_attn_part_b_msl_entry))
    let fm = codec == "f16" ? MetalSqAttnPartBF16_metal_sq_attn_part_b_msl_fastmath : (codec == "f32" ? MetalSqAttnPartBF32_metal_sq_attn_part_b_msl_fastmath : (codec == "q8" ? MetalSqAttnPartBQ8_metal_sq_attn_part_b_msl_fastmath : MetalSqAttnPartBTq4_metal_sq_attn_part_b_msl_fastmath))
    let tgm = codec == "f16" ? MetalSqAttnPartBF16_metal_sq_attn_part_b_msl_tgmem : (codec == "f32" ? MetalSqAttnPartBF32_metal_sq_attn_part_b_msl_tgmem : (codec == "q8" ? MetalSqAttnPartBQ8_metal_sq_attn_part_b_msl_tgmem : MetalSqAttnPartBTq4_metal_sq_attn_part_b_msl_tgmem))
    var pso_p = pipeline_from_source(dev, src, entry, fm, err)
    t |> success(pso_p != null, "{tag}: part pipeline: {err}")
    var pso_c = pipeline_from_source(dev, MetalSqAttnCombB_metal_sq_attn_comb_msl, MetalSqAttnCombB_metal_sq_attn_comb_msl_entry, MetalSqAttnCombB_metal_sq_attn_comb_msl_fastmath, err)
    t |> success(pso_c != null, "{tag}: comb pipeline: {err}")
    if (pso_p == null || pso_c == null) {
        metal_release(pso_p)
        metal_release(pso_c)
        return
    }
    let nb2 = length(cnts)
    let qstride = QD + 37
    let quant = codec == "q8" || codec == "tq4"
    var fx : BatchAttnFix
    build_batch_fix(codec, cnts, qstride, fx)
    var cmax = 0
    for (c in cnts) {
        cmax = max(cmax, c)
    }
    let nchmax = (cmax + CHUNK - 1) / CHUNK
    let sink_v = 0.375
    let scale = 1.0 / sqrt(float(HS))
    var want <- batch_want(fx, cnts, qstride, scale, 0.0, hass ? sink_v : -3.0e38, window)
    var bk = quant ? buf_upload(dev, fx.kslab) : (codec == "f16" ? buf_upload(dev, fx.kh) : buf_upload(dev, fx.kf))
    var bv = quant ? buf_upload(dev, fx.vslab) : (codec == "f16" ? buf_upload(dev, fx.vh) : buf_upload(dev, fx.vf))
    var bq = buf_upload(dev, fx.q)
    var bpart = buf_fill(dev, nb2 * N_HEADS * nchmax * (HS + 2), -1000.0)
    var by = buf_fill(dev, nb2 * QD, -1000.0)
    var brt = buf_upload(dev, fx.rt)
    var bsink = buf_fill(dev, N_HEADS, sink_v)
    var kap = SqAttnPartBArgs(lay = uint(B_LAY), kstride = uint(quant ? NBLK : KV_DIM), nheads = uint(N_HEADS),
        kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale, qstride = uint(qstride),
        nchmax = uint(nchmax), softcap = 0.0, window = uint(window))
    var kac = SqAttnCombArgs(nch = uint(nchmax), nheads = uint(N_HEADS), hs = uint(HS),
        window = uint(window), hass = hass ? 1u : 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_p)
        metal_set_threadgroup_memory_length(enc, tgm, 0)
        if (quant) {
            metal_set_buffer(enc, bk, 0ul, 0)
            metal_set_buffer(enc, bk, 0ul, 1)
            metal_set_buffer(enc, bv, 0ul, 2)
            metal_set_buffer(enc, bv, 0ul, 3)
            metal_set_buffer(enc, bq, 0ul, 4)
            metal_set_buffer(enc, bpart, 0ul, 5)
            metal_set_buffer(enc, brt, 0ul, 6)
            metal_set_bytes(enc, unsafe(addr<void?>(kap)), uint64(typeinfo sizeof(kap)), 7)
        } else {
            metal_set_buffer(enc, bk, 0ul, 0)
            metal_set_buffer(enc, bv, 0ul, 1)
            metal_set_buffer(enc, bq, 0ul, 2)
            metal_set_buffer(enc, bpart, 0ul, 3)
            metal_set_buffer(enc, brt, 0ul, 4)
            metal_set_bytes(enc, unsafe(addr<void?>(kap)), uint64(typeinfo sizeof(kap)), 5)
        }
        metal_dispatch_threadgroups(enc, uint3(uint(N_HEADS * nchmax * nb2), 1u, 1u), uint3(uint(HS), 1u, 1u))
        metal_set_pipeline(enc, pso_c)
        metal_set_buffer(enc, bpart, 0ul, 0)
        metal_set_buffer(enc, by, 0ul, 1)
        metal_set_bytes(enc, unsafe(addr<void?>(kac)), uint64(typeinfo sizeof(kac)), 2)
        metal_set_buffer(enc, brt, 0ul, 3)
        metal_set_buffer(enc, bsink, 0ul, 20)
        metal_dispatch_threadgroups(enc, uint3(uint(N_HEADS * nb2), 1u, 1u), uint3(uint(HS), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_bars(by, want, 2e-3, 0.0, tag), 0)
    }
    metal_release(bk)
    metal_release(bv)
    metal_release(bq)
    metal_release(bpart)
    metal_release(by)
    metal_release(brt)
    metal_release(bsink)
    metal_release(pso_p)
    metal_release(pso_c)
    delete fx
    delete want
}

// ===== the fused single-pass "D" family (hs == 128): part + parallel cross-slice comb =====
// D reuses build_batch_fix: nheads=2, kv_mul=2, hs=128 gives the SAME kv_dim/qd as the hs=64
// shape, so only the oracle's head geometry differs. D is plain causal (no window/softcap/sink).

let private D_HS = 128
let private D_NHEADS = 2
let private D_KVMUL = 2

def private attn_oracle_d(kdeq, vdeq : array<float>; kvbase : int; q : array<float>; qbase, h, cnt : int; scale : float) : array<float> {
    let khoff = (h / D_KVMUL) * D_HS
    var sc : array<float>
    sc |> resize(cnt)
    var mm = -3.0e38
    for (j in range(cnt)) {
        var s = 0.0
        for (d in range(D_HS)) {
            s += q[qbase + h * D_HS + d] * kdeq[kvbase + j * KV_DIM + khoff + d]
        }
        sc[j] = s * scale
        mm = max(mm, sc[j])
    }
    var tot = 0.0
    for (j in range(cnt)) {
        sc[j] = exp(sc[j] - mm)
        tot += sc[j]
    }
    var out : array<float>
    out |> resize(D_HS)
    for (d in range(D_HS)) {
        var acc = 0.0
        for (j in range(cnt)) {
            acc += sc[j] * vdeq[kvbase + j * KV_DIM + khoff + d]
        }
        out[d] = acc / tot
    }
    delete sc
    return <- out
}

def private sq_d_gate(t : T?; dev, queue; codec : string; cnts : array<int>; nsgs : int) {
    let tag = "sq_attn_d_{codec} B={length(cnts)} nsgs={nsgs}"
    var err : string
    let src = codec == "f16" ? MetalSqAttnDF16_metal_sq_attn_d_msl : (codec == "f32" ? MetalSqAttnDF32_metal_sq_attn_d_msl : (codec == "q8" ? MetalSqAttnDQ8_metal_sq_attn_d_msl : MetalSqAttnDTq4_metal_sq_attn_d_msl))
    let entry = codec == "f16" ? MetalSqAttnDF16_metal_sq_attn_d_msl_entry : (codec == "f32" ? MetalSqAttnDF32_metal_sq_attn_d_msl_entry : (codec == "q8" ? MetalSqAttnDQ8_metal_sq_attn_d_msl_entry : MetalSqAttnDTq4_metal_sq_attn_d_msl_entry))
    let fm = codec == "f16" ? MetalSqAttnDF16_metal_sq_attn_d_msl_fastmath : (codec == "f32" ? MetalSqAttnDF32_metal_sq_attn_d_msl_fastmath : (codec == "q8" ? MetalSqAttnDQ8_metal_sq_attn_d_msl_fastmath : MetalSqAttnDTq4_metal_sq_attn_d_msl_fastmath))
    let tgm = codec == "f16" ? MetalSqAttnDF16_metal_sq_attn_d_msl_tgmem : (codec == "f32" ? MetalSqAttnDF32_metal_sq_attn_d_msl_tgmem : (codec == "q8" ? MetalSqAttnDQ8_metal_sq_attn_d_msl_tgmem : MetalSqAttnDTq4_metal_sq_attn_d_msl_tgmem))
    var pso_d = pipeline_from_source(dev, src, entry, fm, err)
    t |> success(pso_d != null, "{tag}: part pipeline: {err}")
    var pso_c = pipeline_from_source(dev, metal_sq_attn_comb_d_msl, metal_sq_attn_comb_d_msl_entry, metal_sq_attn_comb_d_msl_fastmath, err)
    t |> success(pso_c != null, "{tag}: comb pipeline: {err}")
    if (pso_d == null || pso_c == null) {
        metal_release(pso_d)
        metal_release(pso_c)
        return
    }
    let nb2 = length(cnts)
    let qstride = QD + 36                   // q reads through a float4 view — keep the stride % 4
    let quant = codec == "q8" || codec == "tq4"
    var fx : BatchAttnFix
    build_batch_fix(codec, cnts, qstride, fx)
    let scale = 1.0 / sqrt(float(D_HS))
    var want : array<float>
    want |> resize(nb2 * QD)
    for (r in range(nb2)) {
        for (h in range(D_NHEADS)) {
            var col <- attn_oracle_d(fx.kdeq, fx.vdeq, fx.roff[r], fx.q, r * qstride, h, cnts[r], scale)
            for (d in range(D_HS)) {
                want[r * QD + h * D_HS + d] = col[d]
            }
            delete col
        }
    }
    var bk = quant ? buf_upload(dev, fx.kslab) : (codec == "f16" ? buf_upload(dev, fx.kh) : buf_upload(dev, fx.kf))
    var bv = quant ? buf_upload(dev, fx.vslab) : (codec == "f16" ? buf_upload(dev, fx.vh) : buf_upload(dev, fx.vf))
    var bq = buf_upload(dev, fx.q)
    var bpart = buf_fill(dev, nb2 * D_NHEADS * nsgs * (D_HS + 4), -1000.0)
    var by = buf_fill(dev, nb2 * QD, -1000.0)
    var brt = buf_upload(dev, fx.rt)
    var kad = SqAttnDArgs(lay = uint(B_LAY), kstride = uint(quant ? NBLK : KV_DIM), nheads = uint(D_NHEADS),
        kv_mul = uint(D_KVMUL), hs = uint(D_HS), scale = scale, qstride = uint(qstride), nsgs = uint(nsgs))
    var kac = SqAttnCombDArgs(nchmax = uint(nsgs), nheads = uint(D_NHEADS), hs = uint(D_HS))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_d)
        metal_set_threadgroup_memory_length(enc, tgm, 0)
        if (quant) {
            metal_set_buffer(enc, bk, 0ul, 0)
            metal_set_buffer(enc, bk, 0ul, 1)
            metal_set_buffer(enc, bv, 0ul, 2)
            metal_set_buffer(enc, bv, 0ul, 3)
            metal_set_buffer(enc, bq, 0ul, 4)
            metal_set_buffer(enc, bpart, 0ul, 5)
            metal_set_buffer(enc, brt, 0ul, 6)
            metal_set_bytes(enc, unsafe(addr<void?>(kad)), uint64(typeinfo sizeof(kad)), 7)
        } else {
            metal_set_buffer(enc, bk, 0ul, 0)
            metal_set_buffer(enc, bv, 0ul, 1)
            metal_set_buffer(enc, bq, 0ul, 2)
            metal_set_buffer(enc, bpart, 0ul, 3)
            metal_set_buffer(enc, brt, 0ul, 4)
            metal_set_bytes(enc, unsafe(addr<void?>(kad)), uint64(typeinfo sizeof(kad)), 5)
        }
        metal_dispatch_threadgroups(enc, uint3(uint(D_NHEADS), uint((nsgs + 3) / 4), uint(nb2)), uint3(128u, 1u, 1u))
        metal_set_pipeline(enc, pso_c)
        metal_set_threadgroup_memory_length(enc, metal_sq_attn_comb_d_msl_tgmem, 0)
        metal_set_buffer(enc, bpart, 0ul, 0)
        metal_set_buffer(enc, by, 0ul, 1)
        metal_set_buffer(enc, brt, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(kac)), uint64(typeinfo sizeof(kac)), 3)
        metal_dispatch_threadgroups(enc, uint3(uint(D_NHEADS), uint(nb2), 1u), uint3(uint(D_HS), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_bars(by, want, 2e-3, 0.0, tag), 0)
    }
    metal_release(bk)
    metal_release(bv)
    metal_release(bq)
    metal_release(bpart)
    metal_release(by)
    metal_release(brt)
    metal_release(pso_d)
    metal_release(pso_c)
    delete fx
    delete want
}

[test]
def test_metal_attn_codec_kernels(t : T?) {
    t |> run("sq_attn codec twins (f32/q8/tq4, single + split-K) == CPU attention over the dequant truth") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU kernel checks skipped\n")
                    return
                }
                var queue = metal_new_command_queue(dev)
                sq_codec_single_gate(t, dev, queue, "f32", 40, 0.0, false)
                sq_codec_single_gate(t, dev, queue, "f32", 40, 0.0, true)
                sq_codec_single_gate(t, dev, queue, "q8", 40, 0.0, false)
                sq_codec_single_gate(t, dev, queue, "q8", 40, 30.0, false)
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
                sq_b_gate(t, dev, queue, "q8", [30, 90], 30.0, false)
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
                sq_d_gate(t, dev, queue, "q8", [21, 64, 200], 3)
                sq_d_gate(t, dev, queue, "tq4", [30, 100], 2)
                metal_release(queue)
            }
            t |> equal(metal_log_leaks_if_any(), 0l)
        } else {
            feint("das_metal is not built on this platform; attention codec kernel checks skipped\n")
        }
    }
}
