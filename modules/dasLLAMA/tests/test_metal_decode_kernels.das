options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options indenting = 4
options _function_length = 0        // per-kernel gates: fixture + builder dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_kv_codec  // quantize/cvt q8kv + tq4kv rows, fwht rows, tq4_fill_signs — the codec fixtures + their dequant truth
require ?das_metal dasllama/dasllama_metal_kernels  // the decode kernel set: the sq_attn family, the fused rope+KV-store family (Apple builds)
require ?das_metal dasllama/dasllama_metal_common   // the pipeline globals the generated builders read
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // the MetalCell record, check_buf, the device shell
require _attn_oracle
require _compares   // Bar, Shape: the cells' bars
require _kernel_oracles   // attn_q_val / attn_q_row: the query rows; rope_tab_ref: the engine's table rope by its pairing
require math


// The single-query attention kernels vs a direct CPU oracle on synthetic data — the model-less
// unit gate for the shared sq_attn stages (score fill, window mask, both softmax halves, V
// accumulate) and for every call-site fold that surrounds them: attention sinks, the sliding
// window, the split-K partial/combine pair. Seconds per cell, so a stage bug surfaces here
// instead of ten minutes into a model parity arm.
//
// Coverage boundary: this cell exercises the f16 codec, which is the one the shared f16/f32
// generic instantiates twice. The q8_0 and tq4 fill/accumulate stages have their own loads and
// are gated token-for-token by the decode parity arms (arm7-q8kv / arm7b-tq4kv).

let HS = 64                             // head_size == threads per group; epl = HS/32 = 2
let N_HEADS = 4
let KV_MUL = 2                          // 2 kv heads
let KV_DIM = (N_HEADS / KV_MUL) * HS
let QD = N_HEADS * HS
let NBLK = KV_DIM / 32                  // q8/tq4 blocks per KV row
let CHUNK = 64                          // the split-K chunk height the part kernels hardcode

let private BAR_ATTN = Bar(rel = 2e-3, poison_scaled = true)   //! reduction-order noise against the f16-exact fixture and the dequant truth; the poisoned last element moves 0.5 of its magnitude floored at 1

// K/V hold f16-exact small values so the oracle sees the same numbers the kernel loads
def private kv_at(j, d : int) : float => float((j * 7 + d * 3) % 9 - 4) * 0.25

//! every head's attention row at the test shape, K and V both the one slab the gates bind twice; `window` > 0 starts the keys at cnt - window
def private f16_oracle_all(cnt : int; scale, softcap : float; hass : bool; sink : float; window : int) : array<float> {
    let wstart = window > 0 && cnt > window ? cnt - window : 0
    var inscope kv : array<float>
    kv |> resize(cnt * KV_DIM)
    for (j in range(cnt)) {
        for (d in range(KV_DIM)) {
            kv[j * KV_DIM + d] = kv_at(j, d)
        }
    }
    var inscope q <- attn_q_row(QD)
    var want : array<float>
    want |> resize(QD)
    for (h in range(N_HEADS)) {
        attn_row_oracle(q, h * HS, kv, 0, kv, 0, KV_DIM, h, HS, KV_MUL, cnt, scale, want, h * HS, wstart, softcap, hass, sink)
    }
    return <- want
}

//! the gated arm's expectation: `want` under the gates, which must move most of it past the bar
def private gate_want(t : T?; tag : string; var want : array<float>; gate : array<float>) {
    var inscope ungated := want
    gate_rows(want, gate)
    t |> success(check_count(want, ungated, BAR_ATTN) * 2 > length(want), "decode {tag}: the gate moves most of the row past the bar (the gated compare has teeth)")
}

// the single-threadgroup form: one dispatch, the whole context in one pass
def private sq_single_gate(t : T?; var dev, queue; tag : string; cnt : int; softcap : float; hass : bool; gated : bool = false) {
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_attn16_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, "{tag} sq_attn16", pso, err))
        let scale = 1.0 / sqrt(float(HS))
        let sink_v = 0.375
        var inscope kc <- kv_f16(cnt)
        var inscope gate <- attn_gate_row(QD)
        let bk = mc |> in_plain(kc)
        let bv = mc |> in_plain(kc)            // V reads the same slab — the oracle mirrors that
        let bq = gated ? (mc |> in_(q_gate_plane(attn_q_row(QD), gate, HS))) : (mc |> in_(attn_q_row(QD)))
        let by = mc |> out_words(QD, -1000.0)
        let bsink = mc |> in_(plane_of(N_HEADS, sink_v))
        var ka = SqAttnArgs(cnt = uint(cnt), kstride = uint(KV_DIM), nheads = uint(N_HEADS),
            kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale, softcap = softcap,
            hass = hass ? 1u : 0u, qhs = gated ? uint(2 * HS) : 0u, gated = gated ? 1u : 0u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bk], 0ul, mc.buf[bv], 0ul, mc.buf[bq], mc.buf[by], ka, mc.buf[bsink], int64(N_HEADS), int64(HS))
            }
        }
        return if (!ran)
        var inscope want <- f16_oracle_all(cnt, scale, softcap, hass, sink_v, 0)
        if (gated) {
            gate_want(t, tag, want, gate)
        }
        check_buf(t, "decode {tag}", mc.buf[by], want, BAR_ATTN, Shape(row_width = HS))
    }
}

// the split-K pair: one threadgroup per (head, chunk) writes an unnormalized partial, the comb
// kernel rescales across chunks. `window` > 0 also exercises the chunk mask + the chlo skip.
def private sq_part_gate(t : T?; var dev, queue; tag : string; cnt, window : int; gated : bool = false) {
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sp = stamp_of(@@enc_attnp16_c)
        let sc = stamp_of(@@enc_attncomb_c)
        var pso_p = mc |> keep_stamp(sp, err)
        let part_ok = pso_ok(t, "{tag} part16", pso_p, err)
        var pso_c = mc |> keep_stamp(sc, err)
        return if (!pso_ok(t, "{tag} comb", pso_c, err) || !part_ok)
        let scale = 1.0 / sqrt(float(HS))
        let nchunks = (cnt + CHUNK - 1) / CHUNK
        let wstart = window > 0 && cnt > window ? cnt - window : 0
        let chlo = wstart / CHUNK             // the driver's first live chunk
        var inscope kc <- kv_f16(cnt)
        var inscope gate <- attn_gate_row(QD)
        let bk = mc |> in_plain(kc)
        let bv = mc |> in_plain(kc)
        let bq = gated ? (mc |> in_(q_gate_plane(attn_q_row(QD), gate, HS))) : (mc |> in_(attn_q_row(QD)))
        let bpart = mc |> out_words(N_HEADS * nchunks * (HS + 2), -1000.0)
        let by = mc |> out_words(QD, -1000.0)
        let bsink = mc |> in_(plane_of(N_HEADS, 0.0))
        var kap = SqAttnPartArgs(cnt = uint(cnt), kstride = uint(KV_DIM), nheads = uint(N_HEADS),
            kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale, nchunks = uint(nchunks),
            softcap = 0.0, window = uint(window), qhs = gated ? uint(2 * HS) : 0u, gated = gated ? 1u : 0u)
        var kac = SqAttnCombArgs(nch = uint(nchunks), nheads = uint(N_HEADS), hs = uint(HS),
            chlo = uint(chlo), hass = 0u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(sp, pso_p) {
                invoke(sp.enc, enc, mc.buf[bk], 0ul, mc.buf[bv], 0ul, mc.buf[bq], mc.buf[bpart], kap, int64(N_HEADS * nchunks), int64(HS))
            }
            stamp_run(sc, pso_c) {
                invoke(sc.enc, enc, mc.buf[bpart], mc.buf[by], kac, mc.buf[bsink], int64(N_HEADS), int64(HS))
            }
        }
        return if (!ran)
        var inscope want <- f16_oracle_all(cnt, scale, 0.0, false, 0.0, window)
        if (gated) {
            gate_want(t, tag, want, gate)
        }
        check_buf(t, "decode {tag}", mc.buf[by], want, BAR_ATTN, Shape(row_width = HS))
    }
}

[test(description = "The Metal f16 single-query attention kernels, single-threadgroup and split-K with combine, match a CPU attention oracle within 2e-3 under plain, softcap, sink, sliding-window and gated-q variants")]
def test_metal_sq_attn_kernels(t : T?) {
    t |> run("single-query attention kernels == CPU attention oracle (sinks, softcap, split-K, window)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_gate(t) $(var dev : MetalDevice?; var queue : MetalCommandQueue?) {
                sq_single_gate(t, dev, queue, "plain", 40, 0.0, false)
                sq_single_gate(t, dev, queue, "softcap", 40, 30.0, false)
                sq_single_gate(t, dev, queue, "sink", 40, 0.0, true)
                sq_part_gate(t, dev, queue, "splitk", 100, 0)
                sq_part_gate(t, dev, queue, "window", 100, 48)
                sq_single_gate(t, dev, queue, "gated sink", 40, 0.0, true, true)
                sq_part_gate(t, dev, queue, "gated window", 100, 48, true)
            }
        } else {
            t |> skip("das_metal is not built on this platform; attention kernel checks skipped")
        }
    }
}

def private kv_f16(cnt : int) : array<float16> {
    var kv : array<float16>
    kv |> resize(cnt * KV_DIM)
    for (j in range(cnt)) {
        for (d in range(KV_DIM)) {
            kv[j * KV_DIM + d] = float16(kv_at(j, d))
        }
    }
    return <- kv
}


// The single-query attention codec twins (f32 / q8_0 / tq4, single-tg + split-K + comb) vs a
// direct CPU attention oracle. The KV fixtures quantize synthetic f32 rows through the SHIPPED
// codec stores (quantize_q8kv_row / quantize_tq4kv_row) and the oracle attends over the
// DEQUANTIZED values (cvt_*_to_f32) — exactly the numbers the kernel's stream dequant loads,
// so only reduction-order noise separates the sides (the shared 2e-3 bar). Covers per twin:
// sinks, softcap, the sliding window + chlo chunk skip. The f16 single-query cell above owns the f16
// twins these mirror. Without das_metal in the build, or with no Metal device, every check skips.

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
def private sq_codec_single_gate(t : T?; var dev, queue; codec : string; cnt : int; softcap : float; hass : bool; gated : bool = false) {
    let tag = "sq_attn_{codec} cnt={cnt} softcap={softcap} sink={hass} gated={gated}"
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
        let bq = gated ? (mc |> in_(gated_batch(t, tag, q, 1, QD, QD, HS, QD, want))) : (mc |> in_(q))
        let by = mc |> out_words(QD, -1000.0)
        let bsink = mc |> in_(plane_of(N_HEADS, sink_v))
        var ka = SqAttnArgs(cnt = uint(cnt), kstride = uint(codec == "f32" ? KV_DIM : NBLK),
            nheads = uint(N_HEADS), kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale,
            softcap = softcap, hass = hass ? 1u : 0u, qhs = gated ? uint(2 * HS) : 0u, gated = gated ? 1u : 0u)
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
def private sq_codec_part_gate(t : T?; var dev, queue; codec : string; cnt, window : int; gated : bool = false) {
    let tag = "sq_attn_part_{codec} cnt={cnt} window={window} gated={gated}"
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
        let bq = gated ? (mc |> in_(gated_batch(t, tag, q, 1, QD, QD, HS, QD, want))) : (mc |> in_(q))
        let bpart = mc |> out_words(N_HEADS * nchunks * (HS + 2), -1000.0)
        let by = mc |> out_words(QD, -1000.0)
        let bsink = mc |> in_(plane_of(N_HEADS, 0.0))
        var kap = SqAttnPartArgs(cnt = uint(cnt), kstride = uint(codec == "f32" ? KV_DIM : NBLK),
            nheads = uint(N_HEADS), kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale,
            nchunks = uint(nchunks), softcap = 0.0, window = uint(window), qhs = gated ? uint(2 * HS) : 0u, gated = gated ? 1u : 0u)
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

//! the gated arm over `nrows` query rows `qstride` apart: their `qd` q elements as a [q | gate] plane, rows 2 qd apart, and
//! the oracle rows (`wstride` apart) under the same gates, the gate asserted to move them past the bar
def private gated_batch(t : T?; tag : string; q : array<float>; nrows, qstride, qd, hs, wstride : int; var want : array<float>) : array<float> {
    var inscope qs <- [for (k in range(nrows * qd)); q[(k / qd) * qstride + k % qd]]
    var inscope gate <- attn_gate_row(nrows * qd)
    var inscope gw <- gated_rows([for (k in range(nrows * qd)); want[(k / qd) * wstride + k % qd]], gate)
    var inscope ungated := want
    for (k in range(nrows * qd)) {
        want[(k / qd) * wstride + k % qd] = gw[k]
    }
    t |> success(check_count(want, ungated, BAR_ATTN) * 2 > nrows * qd, "{tag}: the gate moves most of the rows past the bar (the gated compare has teeth)")
    return <- q_gate_plane(qs, gate, hs)
}

def private sq_b_gate(t : T?; var dev, queue; codec : string; cnts : array<int>; softcap : float; hass : bool; gated : bool = false) {
    let tag = "sq_attn_b_{codec} B={length(cnts)} softcap={softcap} sink={hass} gated={gated}"
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
        let bq = gated ? (mc |> in_(gated_batch(t, tag, fx.q, nb2, qstride, QD, HS, QD, want))) : (mc |> in_(fx.q))
        let by = mc |> out_words(nb2 * QD, -1000.0)
        let brt = mc |> in_plain(fx.rt)
        let bsink = mc |> in_(plane_of(N_HEADS, sink_v))
        var ka = SqAttnBArgs(lay = uint(B_LAY), kstride = uint(quant ? NBLK : KV_DIM), nheads = uint(N_HEADS),
            kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale, qstride = uint(gated ? 2 * QD : qstride),
            softcap = softcap, hass = hass ? 1u : 0u, qhs = gated ? uint(2 * HS) : 0u, gated = gated ? 1u : 0u)
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

def private sq_partb_gate(t : T?; var dev, queue; codec : string; cnts : array<int>; window : int; hass : bool; gated : bool = false) {
    let tag = "sq_attn_part_b_{codec} B={length(cnts)} window={window} sink={hass} gated={gated}"
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
        let bq = gated ? (mc |> in_(gated_batch(t, tag, fx.q, nb2, qstride, QD, HS, QD, want))) : (mc |> in_(fx.q))
        let bpart = mc |> out_words(nb2 * N_HEADS * nchmax * (HS + 2), -1000.0)
        let by = mc |> out_words(nb2 * QD, -1000.0)
        let brt = mc |> in_plain(fx.rt)
        let bsink = mc |> in_(plane_of(N_HEADS, sink_v))
        var kap = SqAttnPartBArgs(lay = uint(B_LAY), kstride = uint(quant ? NBLK : KV_DIM), nheads = uint(N_HEADS),
            kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale, qstride = uint(gated ? 2 * QD : qstride),
            nchmax = uint(nchmax), softcap = 0.0, window = uint(window), qhs = gated ? uint(2 * HS) : 0u, gated = gated ? 1u : 0u)
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
//! `xtab` runs the combine's table stamp (`enc_attncombd_xt_c`) beside it: its rows the plain combine's bit for bit, the rows8
//! x table those rows' halves at their k-slots, the sums within 1e-5, the rows read in their own order the control.
def private sq_d_gate(t : T?; var dev, queue; codec : string; cnts : array<int>; nsgs : int; hs : int = D_HS; lead : int = 0;
                      window : int = 0; sinks : bool = false; softcap : float = 0.0; gated : bool = false; xtab : bool = false) {
    let halves = hs > D_HS ? hs / D_HS : 1
    let wide = halves > 1
    let rowx = wide ? halves : (D_HS % hs == 0 ? 1 : 3)   //! fixture rows read as ONE kernel row: a wide head's halves, three for a head of 96 (four K/V heads of 96)
    let kvdim = rowx * KV_DIM
    let kvmul = wide ? 1 : D_KVMUL
    let nheads = wide ? 1 : (kvdim / hs) * kvmul   //! a narrower head keeps the slab's bytes: more heads over the same rows
    let qd_row = max(QD, nheads * hs)
    let tag = "sq_attn_d_{codec} B={length(cnts)} nsgs={nsgs} hs={hs} lead={lead} window={window} sinks={sinks} cap={softcap} gated={gated}"
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
        let qd = nheads * hs
        let bq = gated ? (mc |> in_(gated_batch(t, tag, fx.q, nb2, qstride, qd, hs, qd_row, want))) : (mc |> in_(fx.q))
        let bpart = mc |> out_words(nb2 * nheads * halves * nsgs * (hs / halves + 4), -1000.0)
        let by = mc |> out_words(nb2 * qd_row, -1000.0)
        var inscope routes <- [for (_ in range(lead)); uint4(fx.rt[0].x, fx.rt[0].y, fx.rt[0].z, 1u)]
        routes |> push_from([for (row in fx.rt); uint4(row.x, row.y, row.z, row.w / uint(rowx))])
        let brt = mc |> in_plain(routes)
        let rtoff = uint64(lead * 16)
        var kad = SqAttnDArgs(lay = uint(B_LAY), kstride = uint(quant ? NBLK * rowx : kvdim), nheads = uint(nheads),
            kv_mul = uint(kvmul), hs = uint(hs), scale = scale, qstride = uint(gated ? 2 * qd : qstride), nsgs = uint(nsgs), window = uint(window),
            softcap = softcap, qhs = gated ? uint(2 * hs) : 0u, gated = gated ? 1u : 0u)
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
        return if (!xtab)
        let width = nheads * hs
        t |> equal(width, qd_row, "{tag}: the table cell's rows are the combine's width")
        let sx = stamp_of(@@enc_attncombd_xt_c)
        var pso_x = mc |> keep_stamp(sx, err)
        return if (!pso_ok(t, "{tag} comb xt", pso_x, err))
        let sent = -1000.0
        let by2 = mc |> out_words(nb2 * qd_row, sent)
        let bxt = mc |> out_words(8 * width, float16(sent))
        let bxs = mc |> out_words(width / 32 * 8, sent)
        let ran2 = mc |> cell_record(t, "{tag} xt") $(enc : MetalComputeEncoder?) {
            stamp_run(sx, pso_x) {
                invoke(sx.enc, enc, mc.buf[bpart], mc.buf[by2], mc.buf[brt], rtoff, kac, mc.buf[bxt], mc.buf[bxs], mc.buf[bsink], int64(nheads * halves), int64(nb2))
            }
        }
        return if (!ran2)
        var inscope y1 <- buf_download(mc.buf[by], nb2 * qd_row, type<float>)
        check_buf_exact(t, "{tag} xt rows", mc.buf[by2], y1)
        var inscope h <- [for (v in y1); float16(v)]
        var inscope tab <- rows8_table_of(h, nb2, width, float(float16(sent)), sent)
        check_buf_h(t, "{tag} xt table", mc.buf[bxt], tab.xt, BAR_EXACT, Shape(row_width = width))
        check_buf(t, "{tag} xt sums", mc.buf[bxs], tab.xs, Bar(rel = 1e-5), Shape(row_width = 8))
        var inscope plain <- [for (k in range(8 * width)); float(k < nb2 * width ? h[k] : float16(sent))]
        var inscope got <- buf_download(mc.buf[bxt], 8 * width, type<float16>)
        var inscope gotf <- [for (v in got); float(v)]
        check_exceeds(t, "{tag}: the table against the rows' own order (the k-slot order's control)", gotf, plain, BAR_EXACT)
    }
}

[test(description = "The Metal single-query attention kernels over f16, f32, q8 and tq4 KV - single, split-K, batched and fused D forms, heads 64 to 512, windows, sinks, softcap, gated q - match CPU attention over the dequant truth within 2e-3")]
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
                sq_d_gate(t, dev, queue, "f16", [21, 64, 200], 3, xtab = true)            //! the combine's table stamp, three rows of eight
                sq_d_gate(t, dev, queue, "f32", [40, 130], 5, hs = 64, xtab = true)
                sq_d_gate(t, dev, queue, "f16", [21, 64, 200], 3, window = 40, sinks = true, softcap = D_CAP, xtab = true)
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
                //! a gated model's q (qwen35): the head read at its [q | gate] stride, the out under the gate's sigmoid, on every form
                sq_codec_single_gate(t, dev, queue, "f32", 40, 0.0, false, true)
                sq_codec_single_gate(t, dev, queue, "q8", 40, 0.0, true, true)
                sq_codec_part_gate(t, dev, queue, "f32", 100, 48, true)
                sq_codec_part_gate(t, dev, queue, "q8", 200, 80, true)
                sq_b_gate(t, dev, queue, "f16", [21, 64, 127], 0.0, false, true)
                sq_b_gate(t, dev, queue, "q8", [30, 90], 0.0, true, true)
                sq_partb_gate(t, dev, queue, "f16", [30, 100, 200], 80, false, true)
                sq_partb_gate(t, dev, queue, "q8", [30, 100, 200], 0, true, true)
                sq_d_gate(t, dev, queue, "f16", [21, 64, 200], 3, gated = true)
                sq_d_gate(t, dev, queue, "q8", [21, 64, 200], 3, lead = 2, gated = true)
                sq_d_gate(t, dev, queue, "f16", [42, 128, 200], 3, hs = 256, gated = true)
                sq_d_gate(t, dev, queue, "q8", [42, 128, 200], 2, hs = 256, window = 30, sinks = true, gated = true)
                sq_d_gate(t, dev, queue, "f32", [160, 132], 2, hs = 512, gated = true)
            }
        } else {
            t |> skip("das_metal is not built on this platform; attention codec kernel checks skipped")
        }
    }
}


// The fused rope + KV-row-store family (all four mirror codecs, flat + head-cooperative +
// batched) vs the shipped CPU seam on synthetic data: the CPU side is the SAME rope tables
// (rope_scaled_*_tab) followed by the SAME codec stores (quantize_q8kv_row / fwht_signs_row +
// quantize_tq4kv_row) the CPU fallback runs — so the quantized mirrors compare BYTE-EXACT (the
// codec contract: CPU fallback steps read what the GPU stored). f32 rope outputs compare within
// the shared fp bar; f16 mirrors within the f16-rounding envelope. The batched arms compare the
// WHOLE mirror arena against a sentinel-filled expectation, so a stray write outside the row
// table's target row is a red, not silence. Without das_metal in the build, or with no Metal
// device, every check skips.

let private BAR_FP = Bar(rel = 1e-4, poison_scaled = true)   //! the f32 rope outputs; the poisoned last element moves 0.5 of its magnitude floored at 1
let private BAR_F16 = Bar(rel = 1e-3, abs_floor = 1e-3, poison_scaled = true)   //! the f16 mirrors: the store rounds at 2^-11 relative


def private rpst_flat_gate(t : T?; var dev, queue; f16dst : bool; hs, nheads, kv_mul, rot : int; neox, hasbias : bool) {
    let tag = "rpst{f16dst ? 16 : 32} hs={hs} nh={nheads} kvm={kv_mul} rot={rot} neox={neox} bias={hasbias}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = f16dst ? stamp_of(@@enc_rpst16_c) : stamp_of(@@enc_rpst32_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nkvh = nheads / kv_mul
        let qd = nheads * hs
        let kvd = nkvh * hs
        let half = hs / 2
        let halfr = rot / 2
        let npairs_q = qd / 2
        let npairs = (qd + kvd) / 2
        var inscope qk, vsrc, bias, tcos, tsin : array<float>
        qk |> resize(qd + kvd)
        vsrc |> resize(kvd)
        bias |> resize(qd + 2 * kvd)
        tcos |> resize(half)
        tsin |> resize(half)
        for (k in range(qd + kvd)) {
            qk[k] = 0.11 * float(k % 41) - 2.2
        }
        for (k in range(kvd)) {
            vsrc[k] = 0.07 * float(k % 29) - 1.1
        }
        for (k in range(qd + 2 * kvd)) {
            bias[k] = hasbias ? 0.03 * float(k % 17) - 0.25 : 0.0
        }
        for (j in range(half)) {
            tcos[j] = j < halfr ? cos(0.013 * float((j * 7) % 91)) : 1.0
            tsin[j] = j < halfr ? sin(0.013 * float((j * 7) % 91)) : 0.0
        }
        // CPU: bias -> the shipped rope tables over the q and k regions; v = raw v + its bias
        var inscope qk_exp := qk
        var inscope v_exp := vsrc
        if (hasbias) {
            for (k in range(qd + kvd)) {
                qk_exp[k] += bias[k]
            }
            for (k in range(kvd)) {
                v_exp[k] += bias[qd + kvd + k]
            }
        }
        rope_tab_ref(qk_exp, 0, hs, rot, qd, neox, tcos, tsin)
        rope_tab_ref(qk_exp, qd, hs, rot, kvd, neox, tcos, tsin)
        // the kernel ropes q IN PLACE and k INTO THE MIRROR — the k third of qk must stay raw
        var inscope want_qk := qk
        for (k in range(qd)) {
            want_qk[k] = qk_exp[k]
        }
        var inscope want_k : array<float>
        want_k |> resize(kvd)
        for (k in range(kvd)) {
            want_k[k] = qk_exp[qd + k]
        }
        let bqk = mc |> io_(qk)
        let bvs = mc |> in_(vsrc)
        let bkd = f16dst ? (mc |> out_words(kvd, float16(-1000.0))) : (mc |> out_words(kvd, -1000.0))
        let bvd = f16dst ? (mc |> out_words(kvd, float16(-1000.0))) : (mc |> out_words(kvd, -1000.0))
        let btc = mc |> in_(tcos)
        let bts = mc |> in_(tsin)
        let bbias = mc |> in_(bias)
        var ka = gk_rope_kv_args(int64(qd), int64(kvd), int64(hs), int64(npairs_q), int64(npairs), neox, hasbias, int64(rot))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                //! the shared template's binds: q, the k view of the same buffer, the v rows, the mirror K and V rows, the bias, the cos and sin rows
                invoke(st.enc, enc, mc.buf[bqk], mc.buf[bqk], mc.buf[bvs], 0ul, mc.buf[bkd], 0ul, mc.buf[bvd], 0ul, mc.buf[bbias], mc.buf[btc], 0ul, mc.buf[bts], ka, int64(npairs))
            }
        }
        return if (!ran)
        let heads = Shape(row_width = hs)
        t |> success(abs(want_qk[3] - qk[3]) > 1e-3, "{tag}: the roped q row differs from its input at a rotated index (the in-place compare has teeth)")
        check_buf(t, "{tag} q", mc.buf[bqk], want_qk, BAR_FP, heads)
        if (f16dst) {
            check_buf_h(t, "{tag} k", mc.buf[bkd], want_k, BAR_F16, heads)
            check_buf_h(t, "{tag} v", mc.buf[bvd], v_exp, BAR_F16, heads)
        } else {
            check_buf(t, "{tag} k", mc.buf[bkd], want_k, BAR_FP, heads)
            check_buf(t, "{tag} v", mc.buf[bvd], v_exp, BAR_FP, heads)
        }
    }
}

let private QKN_VPLAIN = 0   //! the V row: its projection
let private QKN_VNORM = 1    //! ... scaled by its own rms
let private QKN_VFK = 2      //! ... the raw k row under the k norm's rms

//! the CPU form of one head: `x` plus its bias, its inverse rms
def private qkn_head_ref(src, bias : array<float>; base, bbase, hs : int; hasbias : bool; eps : float) : tuple<x : array<float>; inv : float> {
    var x : array<float>
    x |> resize(hs)
    var ss = 0.0
    for (e in range(hs)) {
        x[e] = src[base + e] + (hasbias ? bias[bbase + e] : 0.0)
        ss += x[e] * x[e]
    }
    return (x <- x, inv = 1.0 / sqrt(ss / float(hs) + eps))
}

//! the rows form's dispatch over the cell's planes `b` (q rows, K arena, V arena, cos, weights, sin, bias, route table)
def private qkn_record_rows(t : T?; var mc : MetalCell; tag : string; st : auto(ST); b : int[8]; var ka : GkQknRopeArgs; nwg, nb : int64) : bool {
    var err : string
    var pso = mc |> keep_stamp(st, err)
    return false if (!pso_ok(t, tag, pso, err))
    return mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
        stamp_run(st, pso) {
            invoke(st.enc, enc, mc.buf[b[0]], mc.buf[b[0]], mc.buf[b[1]], 0ul, mc.buf[b[2]], 0ul, mc.buf[b[3]], 0ul, mc.buf[b[4]], mc.buf[b[5]], 0ul,
                mc.buf[b[6]], mc.buf[b[7]], 0ul, mc.buf[b[0]], ka, nwg, nb, int64(ka.hs / 2u))
        }
    }
}

//! ... and the one-row form's, the K and V rows bound `koffb` / `voffb` bytes into their arenas
def private qkn_record_one(t : T?; var mc : MetalCell; tag : string; st : auto(ST); b : int[8]; var ka : GkQknRopeArgs; nwg : int64; koffb, voffb : uint64) : bool {
    var err : string
    var pso = mc |> keep_stamp(st, err)
    return false if (!pso_ok(t, tag, pso, err))
    return mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
        stamp_run(st, pso) {
            invoke(st.enc, enc, mc.buf[b[0]], mc.buf[b[0]], mc.buf[b[1]], koffb, mc.buf[b[2]], voffb, mc.buf[b[3]], 0ul, mc.buf[b[4]], mc.buf[b[5]], 0ul,
                mc.buf[b[6]], mc.buf[b[0]], ka, nwg, int64(ka.hs / 2u))
        }
    }
}

def private qkn_gate(t : T?; var dev, queue; f16dst, rows : bool; hs, nheads, kv_mul, rot, nb : int; neox, hasbias : bool; vmode : int) {
    let tag = "qkn{rows ? "_b" : ""}{f16dst ? 16 : 32} hs={hs} nh={nheads} kvm={kv_mul} rot={rot} nb={nb} neox={neox} bias={hasbias} v={vmode}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        let nkvh = nheads / kv_mul
        let qd = nheads * hs
        let kvd = nkvh * hs
        let halfr = rot / 2
        let qkvd = qd + 2 * kvd
        let eps = 1.0e-6
        let lay = 1
        let laystride = 9000
        let k_one_row_off = 96    //! the one-row form's K and V rows bound this many elements into the arenas
        let v_one_row_off = 400
        let asz = rows ? 36000 : 400 + kvd
        var inscope qk, bias, w, tcos, tsin : array<float>
        var inscope rt : array<uint4>
        qk |> resize(nb * qkvd)
        bias |> resize(qkvd)
        w |> resize(2 * hs)
        tcos |> resize(nb * halfr)
        tsin |> resize(nb * halfr)
        rt |> resize(nb)
        for (k in range(nb * qkvd)) {
            qk[k] = 0.13 * float(k % 37) - 1.9
        }
        for (k in range(qkvd)) {
            bias[k] = hasbias ? 0.02 * float(k % 19) - 0.2 : 0.0
        }
        for (k in range(2 * hs)) {
            w[k] = 0.6 + 0.04 * float(k % 11)
        }
        for (k in range(nb * halfr)) {
            tcos[k] = cos(0.017 * float((k * 5) % 83))
            tsin[k] = sin(0.017 * float((k * 5) % 83))
        }
        for (r in range(nb)) {
            rt[r] = uint4(uint(r * 2500), uint(1000 + r * 2500), uint(laystride), uint(r + 2))
        }
        var inscope want_qk := qk
        var inscope want_ka, want_va : array<float>
        want_ka |> resize(asz)
        want_va |> resize(asz)
        for (k in range(asz)) {
            want_ka[k] = -1000.0
            want_va[k] = -1000.0
        }
        for (r in range(nb)) {
            let qb = r * qkvd
            let kdst = rows ? int(rt[r].x) + lay * laystride + (int(rt[r].w) - 1) * kvd : k_one_row_off
            let vdst = rows ? int(rt[r].y) + lay * laystride + (int(rt[r].w) - 1) * kvd : v_one_row_off
            for (h in range(nheads)) {
                let hq = qkn_head_ref(qk, bias, qb + h * hs, h * hs, hs, hasbias, eps)
                for (e in range(hs)) {
                    want_qk[qb + h * hs + e] = w[e] * (hq.x[e] * hq.inv)
                }
            }
            rope_tab_ref(want_qk, qb, hs, rot, qd, neox, tcos, tsin, r * halfr)
            var inscope kr : array<float>
            kr |> resize(kvd)
            for (h in range(nkvh)) {
                let hk = qkn_head_ref(qk, bias, qb + qd + h * hs, qd + h * hs, hs, hasbias, eps)
                let hv = qkn_head_ref(qk, bias, qb + qd + kvd + h * hs, qd + kvd + h * hs, hs, hasbias, eps)
                for (e in range(hs)) {
                    kr[h * hs + e] = w[hs + e] * (hk.x[e] * hk.inv)
                    want_va[vdst + h * hs + e] = (vmode == QKN_VFK ? hk.x[e] * hk.inv
                        : (vmode == QKN_VNORM ? hv.x[e] * hv.inv : hv.x[e]))
                }
            }
            rope_tab_ref(kr, 0, hs, rot, kvd, neox, tcos, tsin, r * halfr)
            for (k in range(kvd)) {
                want_ka[kdst + k] = kr[k]
            }
        }
        let bqk = mc |> io_(qk)
        let bka = f16dst ? (mc |> out_words(asz, float16(-1000.0))) : (mc |> out_words(asz, -1000.0))
        let bva = f16dst ? (mc |> out_words(asz, float16(-1000.0))) : (mc |> out_words(asz, -1000.0))
        let btc = mc |> in_(tcos)
        let bts = mc |> in_(tsin)
        let bw = mc |> in_(w)
        let brt = mc |> in_plain(rt)
        let bbias = mc |> in_(bias)
        let esz = f16dst ? 2ul : 4ul
        var ka = GkQknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(halfr), neox = neox ? 1u : 0u, nh = uint(nheads),
            layerbase = uint(lay), kwoff = uint(hs), eps = eps, hasb = hasbias ? 1u : 0u, vfk = vmode == QKN_VFK ? 1u : 0u,
            vnorm = vmode == QKN_VNORM ? 1u : 0u, kvrow = uint(qkvd), qrow = uint(qkvd), cosrow = uint(halfr))
        let nwg = int64(nheads + (rows ? 2 : 1) * nkvh)   //! the rows stamps give the V rows workgroups of their own
        let b = fixed_array(bqk, bka, bva, btc, bw, bts, bbias, brt)
        var ran = false
        if (rows) {
            ran = (f16dst ? qkn_record_rows(t, mc, tag, stamp_of(@@enc_qkn_rope_b16), b, ka, nwg, int64(nb))
                : qkn_record_rows(t, mc, tag, stamp_of(@@enc_qkn_rope_b32), b, ka, nwg, int64(nb)))
        } else {
            ran = (f16dst ? qkn_record_one(t, mc, tag, stamp_of(@@enc_qkn_rope16), b, ka, nwg, uint64(k_one_row_off) * esz, uint64(v_one_row_off) * esz)
                : qkn_record_one(t, mc, tag, stamp_of(@@enc_qkn_rope32), b, ka, nwg, uint64(k_one_row_off) * esz, uint64(v_one_row_off) * esz))
        }
        return if (!ran)
        t |> success(moved_off(want_qk, qk, 3, BAR_FP), "{tag}: the normed q row differs from its input past the bar (the in-place compare has teeth)")
        check_buf(t, "{tag} q", mc.buf[bqk], want_qk, BAR_FP, Shape(row_width = hs))
        if (f16dst) {
            check_buf_h(t, "{tag} karena", mc.buf[bka], want_ka, BAR_F16, FLAT_SHAPE)
            check_buf_h(t, "{tag} varena", mc.buf[bva], want_va, BAR_F16, FLAT_SHAPE)
        } else {
            check_buf(t, "{tag} karena", mc.buf[bka], want_ka, BAR_FP, FLAT_SHAPE)
            check_buf(t, "{tag} varena", mc.buf[bva], want_va, BAR_FP, FLAT_SHAPE)
        }
    }
}

//! a gated model's [q | gate] rows: each row's q heads off `src` (rows `sstride` apart) under `attn_gate_row`'s gates
def private gated_q_rows(src : array<float>; nrows, sstride, nheads, hs : int) : array<float> {
    let qd = nheads * hs
    var inscope q <- [for (k in range(nrows * qd)); src[(k / qd) * sstride + k % qd]]
    return <- q_gate_plane(q, attn_gate_row(nrows * qd), hs)
}


[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#metal-rope-store-rot")]
def private rpst_q8_gate(t : T?; var dev, queue; hs, nheads, kv_mul : int; neox, hasbias : bool; rot : int = 0; gated : bool = false) {
    let tag = "rpst_q8 hs={hs} nh={nheads} kvm={kv_mul} neox={neox} bias={hasbias} rot={rot} gated={gated}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_rpst_q8_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nkvh = nheads / kv_mul
        let qd = nheads * hs
        let kvd = nkvh * hs
        let half = rot > 0 ? rot / 2 : hs / 2   // the rope table row: rot/2 wide on a partial carrier
        let nblk = kvd / 32
        let npairs_q = qd / 2
        let nthreads = int(q8_store_threads(int64(qd), int64(kvd)))   // the dispatcher's own count: a thread a q pair, then a lane an element from the next simdgroup boundary
        var inscope qk, vsrc, bias, tcos, tsin : array<float>
        qk |> resize(qd + kvd)
        vsrc |> resize(kvd)
        bias |> resize(qd + 2 * kvd)
        tcos |> resize(half)
        tsin |> resize(half)
        for (k in range(qd + kvd)) {
            qk[k] = 0.17 * float(k % 43) - 2.7
        }
        for (k in range(kvd)) {
            vsrc[k] = 0.11 * float(k % 27) - 1.5
        }
        for (k in range(qd + 2 * kvd)) {
            bias[k] = hasbias ? 0.03 * float(k % 13) - 0.15 : 0.0
        }
        for (j in range(half)) {
            tcos[j] = cos(0.019 * float((j * 3) % 79))
            tsin[j] = sin(0.019 * float((j * 3) % 79))
        }
        var inscope qk_exp := qk
        var inscope v_exp := vsrc
        if (hasbias) {
            for (k in range(qd + kvd)) {
                qk_exp[k] += bias[k]
            }
            for (k in range(kvd)) {
                v_exp[k] += bias[qd + kvd + k]
            }
        }
        var inscope kwant, vwant : array<uint8>
        kwant |> resize(nblk * 34)
        vwant |> resize(nblk * 34)
        rope_tab_ref(qk_exp, 0, hs, rot, qd, neox, tcos, tsin)
        rope_tab_ref(qk_exp, qd, hs, rot, kvd, neox, tcos, tsin)
        unsafe {
            quantize_q8kv_row(addr(kwant[0]), addr<float const?>(qk_exp[qd]), int64(kvd))
            quantize_q8kv_row(addr(vwant[0]), addr<float const?>(v_exp[0]), int64(kvd))
        }
        var inscope want_qk := qk
        for (k in range(qd)) {
            want_qk[k] = qk_exp[k]
        }
        let bqk = mc |> io_(qk)
        let bvs = mc |> in_(vsrc)
        let bkrow = mc |> out_words(nblk * 34, uint8(0xA5))
        let bvrow = mc |> out_words(nblk * 34, uint8(0xA5))
        let btc = mc |> in_(tcos)
        let bts = mc |> in_(tsin)
        let bbias = mc |> in_(bias)
        var ka = RopeStoreQ8Args(qd = uint(qd), head_size = uint(hs), npairs_q = uint(npairs_q),
            nblk = uint(nblk), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u, rot = uint(rot), qhs = gated ? uint(2 * hs) : 0u)
        //! the gated arm: the q heads ride their [q | gate] rows 2 hs apart, the gates past them untouched, the projection row's q span unread
        var inscope qg <- gated_q_rows(qk, 1, 0, nheads, hs)
        var inscope qg_want <- gated_q_rows(want_qk, 1, 0, nheads, hs)
        let bqg = mc |> io_(qg)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bqk], mc.buf[bvs], 0ul, mc.buf[bkrow], 0ul, mc.buf[bkrow], mc.buf[bvrow], 0ul, mc.buf[bvrow], mc.buf[btc], mc.buf[bts], mc.buf[bbias], ka,
                    gated ? mc.buf[bqg] : mc.buf[bqk], int64(nthreads))
            }
        }
        return if (!ran)
        if (gated) {
            check_buf(t, "{tag} q in the gated rows", mc.buf[bqg], qg_want, BAR_FP, Shape(row_width = 2 * hs))
            check_buf(t, "{tag} projection row untouched", mc.buf[bqk], qk, BAR_EXACT, Shape(row_width = hs))
        } else {
            check_buf(t, "{tag} q", mc.buf[bqk], want_qk, BAR_FP, Shape(row_width = hs))
        }
        check_buf_exact(t, "{tag} kbytes", mc.buf[bkrow], kwant)
        check_buf_exact(t, "{tag} vbytes", mc.buf[bvrow], vwant)
    }
}


def private kvrows_q8_gate(t : T?; var dev, queue; rows, kvd : int) {
    let tag = "kvrows_q8 rows={rows} kvd={kvd}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_kv_rows_q8)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nblk = kvd / 32
        var inscope ks, vs : array<float>
        ks |> resize(rows * kvd)
        vs |> resize(rows * kvd)
        for (k in range(rows * kvd)) {
            ks[k] = 0.17 * float(k % 43) - 2.7
            vs[k] = 0.11 * float(k % 27) - 1.5
        }
        var inscope kwant, vwant : array<uint8>
        kwant |> resize(rows * nblk * 34)
        vwant |> resize(rows * nblk * 34)
        unsafe {   // the rows are contiguous: one call quantizes every block
            quantize_q8kv_row(addr(kwant[0]), addr<float const?>(ks[0]), int64(rows * kvd))
            quantize_q8kv_row(addr(vwant[0]), addr<float const?>(vs[0]), int64(rows * kvd))
        }
        let bks = mc |> in_(ks)
        let bvs = mc |> in_(vs)
        let bkrow = mc |> out_words(rows * nblk * 34, uint8(0xA5))
        let bvrow = mc |> out_words(rows * nblk * 34, uint8(0xA5))
        var ka = KvRowsArgs(rows = uint(rows), nblk = uint(nblk), hs = 64u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bks], 0ul, mc.buf[bvs], 0ul, mc.buf[bkrow], 0ul, mc.buf[bkrow], mc.buf[bvrow], 0ul, mc.buf[bvrow], ka, 2l * int64(rows) * int64(kvd))   // the window's own count: a lane an element
            }
        }
        return if (!ran)
        check_buf_exact(t, "{tag} kbytes", mc.buf[bkrow], kwant)
        check_buf_exact(t, "{tag} vbytes", mc.buf[bvrow], vwant)
    }
}


def private rpst_tq4_gate(t : T?; var dev, queue; hs, nheads, kv_mul : int; neox, hasbias : bool; rot : int = 0; gated : bool = false) {
    let tag = "rpst_tq4 hs={hs} nh={nheads} kvm={kv_mul} neox={neox} bias={hasbias} rot={rot} gated={gated}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_rpst_tq4_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nkvh = nheads / kv_mul
        let qd = nheads * hs
        let kvd = nkvh * hs
        let half = rot > 0 ? rot / 2 : hs / 2   // the rope table row: rot/2 wide on a partial carrier
        let rowb = (kvd / 32) * 18
        var inscope qk, vsrc, bias, tcos, tsin, signs : array<float>
        qk |> resize(qd + kvd)
        vsrc |> resize(kvd)
        bias |> resize(qd + 2 * kvd)
        tcos |> resize(half)
        tsin |> resize(half)
        tq4_fill_signs(signs, int64(hs))
        for (k in range(qd + kvd)) {
            qk[k] = 0.15 * float(k % 47) - 2.9
        }
        for (k in range(kvd)) {
            vsrc[k] = 0.13 * float(k % 33) - 1.7
        }
        for (k in range(qd + 2 * kvd)) {
            bias[k] = hasbias ? 0.02 * float(k % 23) - 0.2 : 0.0
        }
        for (j in range(half)) {
            tcos[j] = cos(0.021 * float((j * 11) % 73))
            tsin[j] = sin(0.021 * float((j * 11) % 73))
        }
        var inscope qk_exp := qk
        var inscope v_exp := vsrc
        if (hasbias) {
            for (k in range(qd + kvd)) {
                qk_exp[k] += bias[k]
            }
            for (k in range(kvd)) {
                v_exp[k] += bias[qd + kvd + k]
            }
        }
        var inscope kwant, vwant : array<uint8>
        kwant |> resize(rowb)
        vwant |> resize(rowb)
        rope_tab_ref(qk_exp, 0, hs, rot, qd, neox, tcos, tsin)
        rope_tab_ref(qk_exp, qd, hs, rot, kvd, neox, tcos, tsin)
        unsafe {
            for (h in range(nheads)) {
                fwht_signs_row(addr(qk_exp[h * hs]), addr<float const?>(signs[0]), int64(hs))
            }
            for (h in range(nkvh)) {
                fwht_signs_row(addr(qk_exp[qd + h * hs]), addr<float const?>(signs[0]), int64(hs))
                fwht_signs_row(addr(v_exp[h * hs]), addr<float const?>(signs[0]), int64(hs))
            }
            quantize_tq4kv_row(addr(kwant[0]), addr<float const?>(qk_exp[qd]), int64(kvd))
            quantize_tq4kv_row(addr(vwant[0]), addr<float const?>(v_exp[0]), int64(kvd))
        }
        var inscope want_qk := qk
        for (k in range(qd)) {
            want_qk[k] = qk_exp[k]
        }
        let bqk = mc |> io_(qk)
        let bvs = mc |> in_(vsrc)
        let bkrow = mc |> out_words(rowb, uint8(0xA5))
        let bvrow = mc |> out_words(rowb, uint8(0xA5))
        let btc = mc |> in_(tcos)
        let bts = mc |> in_(tsin)
        let bsg = mc |> in_(signs)
        let bbias = mc |> in_(bias)
        var ka = RopeStoreTq4Args(qd = uint(qd), head_size = uint(hs), nheads = uint(nheads),
            kv_mul = uint(kv_mul), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u, rot = uint(rot), qhs = gated ? uint(2 * hs) : 0u)
        var inscope qg <- gated_q_rows(qk, 1, 0, nheads, hs)
        var inscope qg_want <- gated_q_rows(want_qk, 1, 0, nheads, hs)
        let bqg = mc |> io_(qg)
        let ntg = nheads + 2 * nkvh
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bqk], mc.buf[bvs], 0ul, mc.buf[bkrow], 0ul, mc.buf[bkrow], mc.buf[bvrow], 0ul, mc.buf[bvrow], mc.buf[btc], mc.buf[bts], mc.buf[bsg], mc.buf[bbias], ka,
                    gated ? mc.buf[bqg] : mc.buf[bqk], int64(ntg), int64(hs))
            }
        }
        return if (!ran)
        if (gated) {
            check_buf(t, "{tag} q in the gated rows", mc.buf[bqg], qg_want, BAR_FP, Shape(row_width = 2 * hs))
            check_buf(t, "{tag} projection row untouched", mc.buf[bqk], qk, BAR_EXACT, Shape(row_width = hs))
        } else {
            check_buf(t, "{tag} q", mc.buf[bqk], want_qk, BAR_FP, Shape(row_width = hs))
        }
        check_buf_exact(t, "{tag} kbytes", mc.buf[bkrow], kwant)
        check_buf_exact(t, "{tag} vbytes", mc.buf[bvrow], vwant)
    }
}


def private rpst_b_gate(t : T?; var dev, queue; f16dst : bool; hs, nheads, kv_mul, rot, nb : int; neox, hasbias : bool) {
    let tag = "rpst_b{f16dst ? 16 : 32} hs={hs} nh={nheads} kvm={kv_mul} rot={rot} nb={nb} neox={neox} bias={hasbias}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = f16dst ? stamp_of(@@enc_rpstb16_c) : stamp_of(@@enc_rpstb32_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nkvh = nheads / kv_mul
        let qd = nheads * hs
        let kvd = nkvh * hs
        let halfr = rot / 2
        let npairs_q = qd / 2
        let npairs = (qd + kvd) / 2
        let qstride = qd + 2 * kvd
        let lay = 1
        let laystride = 9000
        let asz = 36000
        var inscope qk, bias, tcos, tsin : array<float>
        var inscope rt : array<uint4>
        qk |> resize(nb * qstride)
        bias |> resize(qd + 2 * kvd)
        tcos |> resize(nb * halfr)
        tsin |> resize(nb * halfr)
        rt |> resize(nb)
        for (k in range(nb * qstride)) {
            qk[k] = 0.11 * float(k % 53) - 2.4
        }
        for (k in range(qd + 2 * kvd)) {
            bias[k] = hasbias ? 0.03 * float(k % 17) - 0.25 : 0.0
        }
        for (k in range(nb * halfr)) {
            tcos[k] = cos(0.011 * float((k * 7) % 97))
            tsin[k] = sin(0.011 * float((k * 7) % 97))
        }
        for (r in range(nb)) {
            rt[r] = uint4(uint(r * 2500), uint(1000 + r * 2500), uint(laystride), uint(r + 2))
        }
        // CPU: per row, bias -> rope q in the fused row, rope k -> the k arena, v third + bias -> the
        // v arena. Whole arenas compare against sentinel-filled expectations — stray writes are reds.
        var inscope want_qk := qk
        var inscope want_ka, want_va : array<float>
        want_ka |> resize(asz)
        want_va |> resize(asz)
        for (k in range(asz)) {
            want_ka[k] = -1000.0
            want_va[k] = -1000.0
        }
        for (r in range(nb)) {
            let qb = r * qstride
            if (hasbias) {
                for (k in range(qd + kvd)) {
                    want_qk[qb + k] += bias[k]
                }
            }
            rope_tab_ref(want_qk, qb, hs, rot, qd, neox, tcos, tsin, r * halfr)
            rope_tab_ref(want_qk, qb + qd, hs, rot, kvd, neox, tcos, tsin, r * halfr)
            let kdst = int(rt[r].x) + lay * laystride + (int(rt[r].w) - 1) * kvd
            let vdst = int(rt[r].y) + lay * laystride + (int(rt[r].w) - 1) * kvd
            for (k in range(kvd)) {
                want_ka[kdst + k] = want_qk[qb + qd + k]
                want_va[vdst + k] = qk[qb + qd + kvd + k] + (hasbias ? bias[qd + kvd + k] : 0.0)
            }
            // the kernel leaves the fused row's k and v thirds raw — undo the k-third rope/bias
            for (k in range(qd, qd + kvd)) {
                want_qk[qb + k] = qk[qb + k]
            }
        }
        let bqk = mc |> io_(qk)
        let bka = f16dst ? (mc |> out_words(asz, float16(-1000.0))) : (mc |> out_words(asz, -1000.0))
        let bva = f16dst ? (mc |> out_words(asz, float16(-1000.0))) : (mc |> out_words(asz, -1000.0))
        let btc = mc |> in_(tcos)
        let bts = mc |> in_(tsin)
        let brt = mc |> in_plain(rt)
        let bbias = mc |> in_(bias)
        var ka = gk_rope_kv_args(int64(qd), int64(kvd), int64(hs), int64(npairs_q), int64(npairs), neox, hasbias, int64(rot),
            0l, 0l, int64(nb), int64(qstride), int64(qstride), int64(rot / 2), int64(lay))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                //! the shared template's binds: the fused rows as q, k and v views, the mirror K and V arenas, the bias, the cos and sin rows, the row table
                invoke(st.enc, enc, mc.buf[bqk], mc.buf[bqk], mc.buf[bqk], 0ul, mc.buf[bka], 0ul, mc.buf[bva], 0ul, mc.buf[bbias], mc.buf[btc], 0ul, mc.buf[bts], mc.buf[brt], 0ul, ka, int64(npairs), int64(nb))
            }
        }
        return if (!ran)
        t |> success(moved_off(want_qk, qk, 3, BAR_FP), "{tag}: the roped q row differs from its input at a rotated index past the bar (the in-place compare has teeth)")
        check_buf(t, "{tag} q", mc.buf[bqk], want_qk, BAR_FP, Shape(row_width = qstride))
        if (f16dst) {
            check_buf_h(t, "{tag} karena", mc.buf[bka], want_ka, BAR_F16, FLAT_SHAPE)
            check_buf_h(t, "{tag} varena", mc.buf[bva], want_va, BAR_F16, FLAT_SHAPE)
        } else {
            check_buf(t, "{tag} karena", mc.buf[bka], want_ka, BAR_FP, FLAT_SHAPE)
            check_buf(t, "{tag} varena", mc.buf[bva], want_va, BAR_FP, FLAT_SHAPE)
        }
    }
}


def private rpst_bq8_gate(t : T?; var dev, queue; hs, nheads, kv_mul, nb : int; neox, hasbias : bool; rot : int = 0; gated : bool = false) {
    let tag = "rpst_b_q8 hs={hs} nh={nheads} kvm={kv_mul} nb={nb} neox={neox} bias={hasbias} rot={rot} gated={gated}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_rpstb_q8_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nkvh = nheads / kv_mul
        let qd = nheads * hs
        let kvd = nkvh * hs
        let half = rot > 0 ? rot / 2 : hs / 2   // the rope table row: rot/2 wide on a partial carrier
        let nblk = kvd / 32
        let rowb = nblk * 34
        let npairs_q = qd / 2
        let nthreads = int(q8_store_threads(int64(qd), int64(kvd)))   // the dispatcher's own count: a thread a q pair, then a lane an element from the next simdgroup boundary
        let qstride = qd + 2 * kvd
        let lay = 1
        let laystride = 8192
        let asz = 28672
        var inscope qk, bias, tcos, tsin : array<float>
        var inscope rt : array<uint4>
        qk |> resize(nb * qstride)
        bias |> resize(qd + 2 * kvd)
        tcos |> resize(nb * half)
        tsin |> resize(nb * half)
        rt |> resize(nb + 1)
        for (k in range(nb * qstride)) {
            qk[k] = 0.19 * float(k % 59) - 3.1
        }
        for (k in range(qd + 2 * kvd)) {
            bias[k] = hasbias ? 0.02 * float(k % 21) - 0.18 : 0.0
        }
        for (k in range(nb * half)) {
            tcos[k] = cos(0.009 * float((k * 13) % 89))
            tsin[k] = sin(0.009 * float((k * 13) % 89))
        }
        rt[0] = uint4(0u, 0u, 0u, 1u)   //! a foreign lead row: the table binds one row past it, and a kernel reading from its start lands every row a slot off and one at arena byte 0
        for (r in range(nb)) {
            rt[r + 1] = uint4(uint(r * 1024), uint(16384 + r * 1024), uint(laystride), uint(r + 1))
        }
        var inscope want_qk := qk
        var inscope want_a : array<uint8>
        want_a |> resize(asz)
        for (k in range(asz)) {
            want_a[k] = uint8(0xA5)
        }
        unsafe {
            for (r in range(nb)) {
                let qb = r * qstride
                var inscope krow, vrow : array<float>
                krow |> resize(kvd)
                vrow |> resize(kvd)
                for (k in range(kvd)) {
                    krow[k] = qk[qb + qd + k] + (hasbias ? bias[qd + k] : 0.0)
                    vrow[k] = qk[qb + qd + kvd + k] + (hasbias ? bias[qd + kvd + k] : 0.0)
                }
                if (hasbias) {
                    for (k in range(qd)) {
                        want_qk[qb + k] += bias[k]
                    }
                }
                rope_tab_ref(want_qk, qb, hs, rot, qd, neox, tcos, tsin, r * half)
                rope_tab_ref(krow, 0, hs, rot, kvd, neox, tcos, tsin, r * half)
                let kb0b = int(rt[r + 1].x) + lay * laystride + (int(rt[r + 1].w) - 1) * rowb
                let vb0b = int(rt[r + 1].y) + lay * laystride + (int(rt[r + 1].w) - 1) * rowb
                quantize_q8kv_row(addr(want_a[kb0b]), addr<float const?>(krow[0]), int64(kvd))
                quantize_q8kv_row(addr(want_a[vb0b]), addr<float const?>(vrow[0]), int64(kvd))
            }
        }
        let bqk = mc |> io_(qk)
        let barena = mc |> out_words(asz, uint8(0xA5))
        let btc = mc |> in_(tcos)
        let bts = mc |> in_(tsin)
        let brt = mc |> in_plain(rt)
        let bbias = mc |> in_(bias)
        var ka = RopeStoreBQ8Args(lay = uint(lay), nblk = uint(nblk), qd = uint(qd), head_size = uint(hs),
            npairs_q = uint(npairs_q), qstride = uint(qstride), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u, rot = uint(rot),
            qhs = gated ? uint(2 * hs) : 0u, qrow = gated ? uint(2 * qd) : 0u)
        var inscope qg <- gated_q_rows(qk, nb, qstride, nheads, hs)
        var inscope qg_want <- gated_q_rows(want_qk, nb, qstride, nheads, hs)
        let bqg = mc |> io_(qg)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bqk], mc.buf[barena], mc.buf[barena], mc.buf[barena], mc.buf[barena], mc.buf[btc], mc.buf[bts], mc.buf[brt], 16ul, mc.buf[bbias], ka,
                    gated ? mc.buf[bqg] : mc.buf[bqk], int64(nthreads), int64(nb))
            }
        }
        return if (!ran)
        if (gated) {
            check_buf(t, "{tag} q in the gated rows", mc.buf[bqg], qg_want, BAR_FP, Shape(row_width = 2 * hs))
            check_buf(t, "{tag} projection rows untouched", mc.buf[bqk], qk, BAR_EXACT, Shape(row_width = qstride))
        } else {
            check_buf(t, "{tag} q", mc.buf[bqk], want_qk, BAR_FP, Shape(row_width = qstride))
        }
        check_buf_exact(t, "{tag} arena", mc.buf[barena], want_a)
    }
}


def private rpst_btq4_gate(t : T?; var dev, queue; hs, nheads, kv_mul, nb : int; neox, hasbias : bool; rot : int = 0; gated : bool = false) {
    let tag = "rpst_b_tq4 hs={hs} nh={nheads} kvm={kv_mul} nb={nb} neox={neox} bias={hasbias} rot={rot} gated={gated}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_rpstb_tq4_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nkvh = nheads / kv_mul
        let qd = nheads * hs
        let kvd = nkvh * hs
        let half = rot > 0 ? rot / 2 : hs / 2   // the rope table row: rot/2 wide on a partial carrier
        let rowb = (kvd / 32) * 18
        let qstride = qd + 2 * kvd
        let lay = 1
        let laystride = 8192
        let asz = 28672
        var inscope qk, bias, tcos, tsin, signs : array<float>
        var inscope rt : array<uint4>
        qk |> resize(nb * qstride)
        bias |> resize(qd + 2 * kvd)
        tcos |> resize(nb * half)
        tsin |> resize(nb * half)
        tq4_fill_signs(signs, int64(hs))
        rt |> resize(nb + 1)
        for (k in range(nb * qstride)) {
            qk[k] = 0.21 * float(k % 61) - 3.3
        }
        for (k in range(qd + 2 * kvd)) {
            bias[k] = hasbias ? 0.02 * float(k % 25) - 0.22 : 0.0
        }
        for (k in range(nb * half)) {
            tcos[k] = cos(0.007 * float((k * 17) % 101))
            tsin[k] = sin(0.007 * float((k * 17) % 101))
        }
        rt[0] = uint4(0u, 0u, 0u, 1u)   //! a foreign lead row: the table binds one row past it, and a kernel reading from its start lands every row a slot off and one at arena byte 0
        for (r in range(nb)) {
            rt[r + 1] = uint4(uint(r * 1024), uint(16384 + r * 1024), uint(laystride), uint(r + 1))
        }
        var inscope want_qk := qk
        var inscope want_a : array<uint8>
        want_a |> resize(asz)
        for (k in range(asz)) {
            want_a[k] = uint8(0xA5)
        }
        unsafe {
            for (r in range(nb)) {
                let qb = r * qstride
                var inscope krow, vrow : array<float>
                krow |> resize(kvd)
                vrow |> resize(kvd)
                for (k in range(kvd)) {
                    krow[k] = qk[qb + qd + k] + (hasbias ? bias[qd + k] : 0.0)
                    vrow[k] = qk[qb + qd + kvd + k] + (hasbias ? bias[qd + kvd + k] : 0.0)
                }
                if (hasbias) {
                    for (k in range(qd)) {
                        want_qk[qb + k] += bias[k]
                    }
                }
                rope_tab_ref(want_qk, qb, hs, rot, qd, neox, tcos, tsin, r * half)
                rope_tab_ref(krow, 0, hs, rot, kvd, neox, tcos, tsin, r * half)
                for (h in range(nheads)) {
                    fwht_signs_row(addr(want_qk[qb + h * hs]), addr<float const?>(signs[0]), int64(hs))
                }
                for (h in range(nkvh)) {
                    fwht_signs_row(addr(krow[h * hs]), addr<float const?>(signs[0]), int64(hs))
                    fwht_signs_row(addr(vrow[h * hs]), addr<float const?>(signs[0]), int64(hs))
                }
                let kb0b = int(rt[r + 1].x) + lay * laystride + (int(rt[r + 1].w) - 1) * rowb
                let vb0b = int(rt[r + 1].y) + lay * laystride + (int(rt[r + 1].w) - 1) * rowb
                quantize_tq4kv_row(addr(want_a[kb0b]), addr<float const?>(krow[0]), int64(kvd))
                quantize_tq4kv_row(addr(want_a[vb0b]), addr<float const?>(vrow[0]), int64(kvd))
            }
        }
        let bqk = mc |> io_(qk)
        let barena = mc |> out_words(asz, uint8(0xA5))
        let btc = mc |> in_(tcos)
        let bts = mc |> in_(tsin)
        let bsg = mc |> in_(signs)
        let brt = mc |> in_plain(rt)
        let bbias = mc |> in_(bias)
        var ka = RopeStoreBTq4Args(lay = uint(lay), qd = uint(qd), head_size = uint(hs), nheads = uint(nheads),
            kv_mul = uint(kv_mul), qstride = uint(qstride), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u, rot = uint(rot),
            qhs = gated ? uint(2 * hs) : 0u, qrow = gated ? uint(2 * qd) : 0u)
        var inscope qg <- gated_q_rows(qk, nb, qstride, nheads, hs)
        var inscope qg_want <- gated_q_rows(want_qk, nb, qstride, nheads, hs)
        let bqg = mc |> io_(qg)
        let ntg = nheads + 2 * nkvh
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bqk], mc.buf[barena], mc.buf[barena], mc.buf[barena], mc.buf[barena], mc.buf[btc], mc.buf[bts], mc.buf[bsg], mc.buf[brt], 16ul, mc.buf[bbias], ka,
                    gated ? mc.buf[bqg] : mc.buf[bqk], int64(ntg), int64(nb), int64(hs))
            }
        }
        return if (!ran)
        if (gated) {
            check_buf(t, "{tag} q in the gated rows", mc.buf[bqg], qg_want, BAR_FP, Shape(row_width = 2 * hs))
            check_buf(t, "{tag} projection rows untouched", mc.buf[bqk], qk, BAR_EXACT, Shape(row_width = qstride))
        } else {
            check_buf(t, "{tag} q", mc.buf[bqk], want_qk, BAR_FP, Shape(row_width = qstride))
        }
        check_buf_exact(t, "{tag} arena", mc.buf[barena], want_a)
    }
}


def private unsign_gate(t : T?; var dev, queue; hs, nheads, nrows, pad : int; gated : bool = false) {
    let tag = "fwht_unsign hs={hs} nh={nheads} rows={nrows} pad={pad} gated={gated}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_unsign_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let qstride = nheads * hs + pad
        var inscope yo, signs : array<float>
        yo |> resize(nrows * qstride)
        tq4_fill_signs(signs, int64(hs))
        for (k in range(nrows * qstride)) {
            yo[k] = 0.23 * float(k % 67) - 3.5
        }
        var inscope want := yo
        unsafe {
            for (r in range(nrows)) {
                for (h in range(nheads)) {
                    fwht_unsign_row(addr(want[r * qstride + h * hs]), addr<float const?>(signs[0]), int64(hs))
                }
            }
        }
        //! the gated arm: each head's output under its gate's sigmoid after the un-rotation, the gates in [q | gate] rows
        let qd = nheads * hs
        var inscope gate <- attn_gate_row(nrows * qd)
        var inscope qg <- q_gate_plane(attn_q_row(nrows * qd), gate, hs)
        var inscope want_g := want
        for (r in range(nrows)) {
            var inscope grow <- gated_rows([for (e in range(qd)); want[r * qstride + e]], [for (e in range(qd)); gate[r * qd + e]])
            for (e in range(qd)) {
                want_g[r * qstride + e] = grow[e]
            }
        }
        let byo = mc |> io_(yo)
        let bsg = mc |> in_(signs)
        let bhs = mc |> in_plain([uint(hs)])
        let bqs = mc |> in_plain([uint(qstride)])
        let bqg = mc |> in_(qg)
        let bgs = mc |> in_plain([gated ? uint(2 * qd) : 0u])   //! a zero stride is the ungated form, the gate rows bound but unread
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[byo], mc.buf[bsg], mc.buf[bhs], mc.buf[bqs], mc.buf[bqg], mc.buf[bgs], int64(nheads), int64(nrows), int64(hs))
            }
        }
        return if (!ran)
        if (gated) {
            t |> success(check_count(want_g, want, BAR_FP) * 2 > length(want), "{tag}: the gate moves most of the un-rotated rows past the bar (the gated compare has teeth)")
            check_buf(t, tag, mc.buf[byo], want_g, BAR_FP, Shape(row_width = qstride))
        } else {
            check_buf(t, tag, mc.buf[byo], want, BAR_FP, Shape(row_width = qstride))
        }
    }
}

[test(description = "The fused Metal rope + KV-store kernels (flat, q/k-norm, batched, partial and gated rope, tq4 un-rotate) match the CPU rope and codec seam: f32 rows within 1e-4 and f16 mirrors within 1e-3 relative, q8 and tq4 bytes exact, inputs untouched")]
def test_metal_rope_store_kernels(t : T?) {
    t |> run("fused rope + KV-store kernels == the CPU rope/codec seam (all four mirrors, flat/head/batched)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_gate(t) $(var dev : MetalDevice?; var queue : MetalCommandQueue?) {
                // flat f16/f32: NORM + NEOX + partial-NEOX (the qwen35 shape), bias on/off
                rpst_flat_gate(t, dev, queue, true, 64, 4, 2, 64, false, true)
                rpst_flat_gate(t, dev, queue, true, 64, 4, 2, 64, true, false)
                rpst_flat_gate(t, dev, queue, true, 64, 4, 2, 32, true, false)
                rpst_flat_gate(t, dev, queue, false, 64, 4, 2, 64, true, true)
                rpst_flat_gate(t, dev, queue, false, 64, 4, 2, 64, false, false)
                // the fused q/k norm form, one row and rows: NORM, NEOX, the partial NEOX of qwen35, the wide heads, the three V forms
                for (f16dst in fixed_array(true, false)) {
                    for (rows in fixed_array(false, true)) {
                        let nb = rows ? 3 : 1
                        qkn_gate(t, dev, queue, f16dst, rows, 64, 4, 2, 64, nb, true, true, QKN_VPLAIN)
                        qkn_gate(t, dev, queue, f16dst, rows, 128, 4, 2, 128, nb, false, false, QKN_VPLAIN)
                        qkn_gate(t, dev, queue, f16dst, rows, 256, 2, 2, 64, nb, true, false, QKN_VPLAIN)
                        qkn_gate(t, dev, queue, f16dst, rows, 256, 2, 1, 256, nb, true, false, QKN_VNORM)
                        qkn_gate(t, dev, queue, f16dst, rows, 512, 2, 2, 512, nb, true, false, QKN_VFK)
                        qkn_gate(t, dev, queue, f16dst, rows, 96, 4, 2, 96, nb, true, true, QKN_VNORM)
                    }
                }
                // quantized mirrors: byte-exact vs quantize_q8kv_row / quantize_tq4kv_row
                rpst_q8_gate(t, dev, queue, 64, 4, 2, true, true)
                rpst_q8_gate(t, dev, queue, 64, 4, 2, false, false)
                //! a q span off the 32-pair lattice (three heads of 96: 144 pairs): the K/V lanes start at the next simdgroup boundary past it
                rpst_q8_gate(t, dev, queue, 96, 3, 3, true, false)
                rpst_bq8_gate(t, dev, queue, 96, 3, 3, 2, false, true)
                // the window rows' store: a lane an element over the rows, the dispatcher's count
                kvrows_q8_gate(t, dev, queue, 3, 128)
                kvrows_q8_gate(t, dev, queue, 5, 96)
                rpst_tq4_gate(t, dev, queue, 64, 4, 2, true, true)
                rpst_tq4_gate(t, dev, queue, 64, 4, 2, false, false)
                // the partial carrier (qwen35: rope 64 of head 256) on both block codecs, the neox flag either way
                rpst_q8_gate(t, dev, queue, 256, 2, 2, true, true, 64)
                rpst_q8_gate(t, dev, queue, 256, 2, 2, false, false, 64)
                rpst_tq4_gate(t, dev, queue, 256, 2, 2, true, false, 64)
                rpst_tq4_gate(t, dev, queue, 256, 2, 2, false, true, 64)
                // the whole head named as the prefix (rot == head_size, what every full-rope layer sends): the same bytes as the default
                rpst_q8_gate(t, dev, queue, 64, 4, 2, true, true, 64)
                rpst_tq4_gate(t, dev, queue, 64, 4, 2, false, false, 64)
                // batched twins: row tables + whole-arena sentinel compares
                rpst_b_gate(t, dev, queue, true, 64, 4, 2, 64, 3, true, true)
                rpst_b_gate(t, dev, queue, true, 64, 4, 2, 64, 2, false, false)
                rpst_b_gate(t, dev, queue, false, 64, 4, 2, 32, 2, true, false)
                rpst_bq8_gate(t, dev, queue, 64, 4, 2, 3, true, false)
                rpst_bq8_gate(t, dev, queue, 64, 4, 2, 2, false, true)
                rpst_btq4_gate(t, dev, queue, 64, 4, 2, 2, true, true)
                rpst_btq4_gate(t, dev, queue, 64, 4, 2, 2, false, false)
                // the partial carrier's batched twins: per-row rope rows rot/2 wide
                rpst_bq8_gate(t, dev, queue, 256, 2, 2, 3, true, true, 64)
                rpst_bq8_gate(t, dev, queue, 256, 2, 2, 2, false, false, 64)
                rpst_btq4_gate(t, dev, queue, 256, 2, 2, 2, true, false, 64)
                rpst_btq4_gate(t, dev, queue, 256, 2, 2, 3, false, true, 64)
                rpst_bq8_gate(t, dev, queue, 64, 4, 2, 3, true, false, 64)
                rpst_btq4_gate(t, dev, queue, 64, 4, 2, 2, true, true, 64)
                // the tq4 un-rotate partner
                unsign_gate(t, dev, queue, 64, 4, 3, 32)
                unsign_gate(t, dev, queue, 128, 2, 1, 0)
                //! a gated model's q rows (qwen35: head 256, rope 64): the [q | gate] heads roped and rotated in their own rows, the gate after the un-rotation
                rpst_q8_gate(t, dev, queue, 256, 2, 2, true, false, 64, true)
                rpst_q8_gate(t, dev, queue, 64, 4, 2, false, true, 0, true)
                rpst_tq4_gate(t, dev, queue, 256, 2, 2, true, false, 64, true)
                rpst_tq4_gate(t, dev, queue, 64, 4, 2, false, true, 0, true)
                rpst_bq8_gate(t, dev, queue, 256, 2, 2, 3, true, false, 64, true)
                rpst_btq4_gate(t, dev, queue, 256, 2, 2, 2, true, false, 64, true)
                unsign_gate(t, dev, queue, 256, 2, 3, 0, true)
                unsign_gate(t, dev, queue, 64, 4, 2, 32, true)
            }
        } else {
            t |> skip("das_metal is not built on this platform; rope-store kernel checks skipped")
        }
    }
}
