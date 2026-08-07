options gen2
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

// causal single-query attention over dequantized K/V planes for one head; window > 0 masks
// rows below cnt - window; sink > -1e30 folds an attention sink into max and denominator
def private attn_oracle(kdeq, vdeq : array<float>; h, cnt : int; scale, softcap, sink : float; window : int) : array<float> {
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
            s += q_val(h * HS + d) * kdeq[j * KV_DIM + khoff + d]
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
            acc += sc[j] * vdeq[j * KV_DIM + khoff + d]
        }
        out[d] = acc / tot
    }
    delete sc
    return <- out
}

def private oracle_all(kdeq, vdeq : array<float>; cnt : int; scale, softcap, sink : float; window : int) : array<float> {
    var want : array<float>
    want |> resize(QD)
    for (h in range(N_HEADS)) {
        var col <- attn_oracle(kdeq, vdeq, h, cnt, scale, softcap, sink, window)
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
    let src = codec == "f32" ? metal_sq_attn32_msl : (codec == "q8" ? metal_sq_attn_q8_msl : metal_sq_attn_tq4_msl)
    let entry = codec == "f32" ? metal_sq_attn32_msl_entry : (codec == "q8" ? metal_sq_attn_q8_msl_entry : metal_sq_attn_tq4_msl_entry)
    let fm = codec == "f32" ? metal_sq_attn32_msl_fastmath : (codec == "q8" ? metal_sq_attn_q8_msl_fastmath : metal_sq_attn_tq4_msl_fastmath)
    let tgm = codec == "f32" ? metal_sq_attn32_msl_tgmem : (codec == "q8" ? metal_sq_attn_q8_msl_tgmem : metal_sq_attn_tq4_msl_tgmem)
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
    let sink_v = 0.375
    let scale = 1.0 / sqrt(float(HS))
    var want <- oracle_all(kdeq, vdeq, cnt, scale, softcap, hass ? sink_v : -3.0e38, 0)
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
    let src = codec == "f32" ? metal_sq_attn_part32_msl : (codec == "q8" ? metal_sq_attn_part_q8_msl : metal_sq_attn_part_tq4_msl)
    let entry = codec == "f32" ? metal_sq_attn_part32_msl_entry : (codec == "q8" ? metal_sq_attn_part_q8_msl_entry : metal_sq_attn_part_tq4_msl_entry)
    let fm = codec == "f32" ? metal_sq_attn_part32_msl_fastmath : (codec == "q8" ? metal_sq_attn_part_q8_msl_fastmath : metal_sq_attn_part_tq4_msl_fastmath)
    let tgm = codec == "f32" ? metal_sq_attn_part32_msl_tgmem : (codec == "q8" ? metal_sq_attn_part_q8_msl_tgmem : metal_sq_attn_part_tq4_msl_tgmem)
    var pso_p = pipeline_from_source(dev, src, entry, fm, err)
    t |> success(pso_p != null, "{tag}: part pipeline: {err}")
    var pso_c = pipeline_from_source(dev, metal_sq_attn_comb_msl, metal_sq_attn_comb_msl_entry, metal_sq_attn_comb_msl_fastmath, err)
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
    var want <- oracle_all(kdeq, vdeq, cnt, scale, 0.0, -3.0e38, window)
    var bk = codec == "f32" ? buf_upload(dev, kdeq) : buf_upload(dev, kslab)
    var bv = codec == "f32" ? buf_upload(dev, vdeq) : buf_upload(dev, vslab)
    var bq = buf_upload(dev, q)
    var bpart = buf_fill(dev, N_HEADS * nchunks * (HS + 2), -1000.0)
    var by = buf_fill(dev, QD, -1000.0)
    var bsink = buf_fill(dev, N_HEADS, 0.0)
    var kap = SqAttnPartArgs(cnt = uint(cnt), kstride = uint(codec == "f32" ? KV_DIM : NBLK),
        nheads = uint(N_HEADS), kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale,
        nchunks = uint(nchunks), softcap = 0.0, window = uint(window))
    var kac = SqAttnCombArgs(nchunks = uint(nchunks), nheads = uint(N_HEADS), hs = uint(HS),
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
                metal_release(queue)
            }
            t |> equal(metal_log_leaks_if_any(), 0l)
        } else {
            feint("das_metal is not built on this platform; attention codec kernel checks skipped\n")
        }
    }
}
