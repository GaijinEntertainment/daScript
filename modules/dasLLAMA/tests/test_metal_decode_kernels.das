options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options indenting = 4

require dastest/testing_boost public
require ?das_metal dasllama/dasllama_metal_kernels  // the decode kernel set (Apple builds)
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require math

// The single-query attention kernels vs a direct CPU oracle on synthetic data — the model-less
// unit gate for the shared sq_attn stages (score fill, window mask, both softmax halves, V
// accumulate) and for every call-site fold that surrounds them: attention sinks, the sliding
// window, the split-K partial/combine pair. Seconds per cell, so a stage bug surfaces here
// instead of ten minutes into a model parity arm.
//
// Coverage boundary: this file exercises the f16 codec, which is the one the shared f16/f32
// generic instantiates twice. The q8_0 and tq4 fill/accumulate stages have their own loads and
// are gated token-for-token by the decode parity arms (arm7-q8kv / arm7b-tq4kv).

let HS = 64                             // head_size == threads per group; epl = HS/32 = 2
let N_HEADS = 4
let KV_MUL = 2                          // 2 kv heads
let KV_DIM = (N_HEADS / KV_MUL) * HS
let QD = N_HEADS * HS
let CHUNK = 64                          // the split-K chunk height the part kernels hardcode

def private approx(x, y : float) : bool {
    return abs(x - y) <= 2e-3 * max(1.0, max(abs(x), abs(y)))
}

// K/V hold f16-exact small values so the oracle sees the same numbers the kernel loads
def private kv_at(j, d : int) : float => float((j * 7 + d * 3) % 9 - 4) * 0.25

def private q_at(i : int) : float => float((i * 5) % 11 - 5) * 0.125

// causal single-query attention for one (head, context) at the test shape. `window` > 0 masks
// rows below cnt - window; `sink` > -1e30 folds an attention sink into max and denominator.
def private attn_oracle(h, cnt : int; scale, softcap, sink : float; window : int) : array<float> {
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
            s += q_at(h * HS + d) * kv_at(j, khoff + d)
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
            acc += sc[j] * kv_at(j, khoff + d)
        }
        out[d] = acc / tot
    }
    return <- out
}

def private oracle_all(cnt : int; scale, softcap, sink : float; window : int) : array<float> {
    var want : array<float>
    want |> resize(QD)
    for (h in range(N_HEADS)) {
        var col <- attn_oracle(h, cnt, scale, softcap, sink, window)
        for (d in range(HS)) {
            want[h * HS + d] = col[d]
        }
        delete col
    }
    return <- want
}

// the single-threadgroup form: one dispatch, the whole context in one pass
def private sq_single_gate(t : T?; dev, queue; tag : string; cnt : int; softcap : float; hass : bool) {
    var err : string
    var pso = pipeline_from_source(dev, MetalSqAttnF16_metal_sq_attn_msl, MetalSqAttnF16_metal_sq_attn_msl_entry, MetalSqAttnF16_metal_sq_attn_msl_fastmath, err)
    t |> success(pso != null, "{tag}: sq_attn16 pipeline: {err}")
    return if (pso == null)
    let scale = 1.0 / sqrt(float(HS))
    let sink_v = 0.375
    let kc <- kv_f16(cnt)
    let q <- q_f32()
    var bk = buf_upload(dev, kc)
    var bv = buf_upload(dev, kc)            // V reads the same slab — the oracle mirrors that
    var bq = buf_upload(dev, q)
    var by = buf_fill(dev, QD, -1000.0)
    var bsink = buf_fill(dev, N_HEADS, sink_v)
    var ka = SqAttnArgs(cnt = uint(cnt), kstride = uint(KV_DIM), nheads = uint(N_HEADS),
        kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale, softcap = softcap,
        hass = hass ? 1u : 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, MetalSqAttnF16_metal_sq_attn_msl_tgmem, 0)
        metal_set_buffer(enc, bk, 0ul, 0)
        metal_set_buffer(enc, bv, 0ul, 1)
        metal_set_buffer(enc, bq, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
        metal_set_buffer(enc, bsink, 0ul, 20)
        metal_dispatch_threadgroups(enc, uint3(uint(N_HEADS), 1u, 1u), uint3(uint(HS), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        var want <- oracle_all(cnt, scale, softcap, hass ? sink_v : -3.0e38, 0)
        t |> equal(buf_mismatch_approx(by, want), 0)
        delete want
    }
    release_all([bk, bv, bq, by, bsink])
    metal_release(pso)
}

// the split-K pair: one threadgroup per (head, chunk) writes an unnormalized partial, the comb
// kernel rescales across chunks. `window` > 0 also exercises the chunk mask + the chlo skip.
def private sq_part_gate(t : T?; dev, queue; tag : string; cnt, window : int) {
    var err : string
    var pso_p = pipeline_from_source(dev, metal_sq_attn_part16_msl, metal_sq_attn_part16_msl_entry, metal_sq_attn_part16_msl_fastmath, err)
    t |> success(pso_p != null, "{tag}: part16 pipeline: {err}")
    var pso_c = pipeline_from_source(dev, metal_sq_attn_comb_msl, metal_sq_attn_comb_msl_entry, metal_sq_attn_comb_msl_fastmath, err)
    t |> success(pso_c != null, "{tag}: comb pipeline: {err}")
    if (pso_p == null || pso_c == null) {
        metal_release(pso_p)
        metal_release(pso_c)
        return
    }
    let scale = 1.0 / sqrt(float(HS))
    let nchunks = (cnt + CHUNK - 1) / CHUNK
    let wstart = window > 0 && cnt > window ? cnt - window : 0
    let chlo = wstart / CHUNK             // the driver's first live chunk
    let kc <- kv_f16(cnt)
    let q <- q_f32()
    var bk = buf_upload(dev, kc)
    var bv = buf_upload(dev, kc)
    var bq = buf_upload(dev, q)
    var bpart = buf_fill(dev, N_HEADS * nchunks * (HS + 2), -1000.0)
    var by = buf_fill(dev, QD, -1000.0)
    var bsink = buf_fill(dev, N_HEADS, 0.0)
    var kap = SqAttnPartArgs(cnt = uint(cnt), kstride = uint(KV_DIM), nheads = uint(N_HEADS),
        kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale, nchunks = uint(nchunks),
        softcap = 0.0, window = uint(window))
    var kac = SqAttnCombArgs(nchunks = uint(nchunks), nheads = uint(N_HEADS), hs = uint(HS),
        chlo = uint(chlo), hass = 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_p)
        metal_set_threadgroup_memory_length(enc, metal_sq_attn_part16_msl_tgmem, 0)
        metal_set_buffer(enc, bk, 0ul, 0)
        metal_set_buffer(enc, bv, 0ul, 1)
        metal_set_buffer(enc, bq, 0ul, 2)
        metal_set_buffer(enc, bpart, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr<void?>(kap)), uint64(typeinfo sizeof(kap)), 4)
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
        var want <- oracle_all(cnt, scale, 0.0, -3.0e38, window)
        t |> equal(buf_mismatch_approx(by, want), 0)
        delete want
    }
    release_all([bk, bv, bq, bpart, by, bsink])
    metal_release(pso_p)
    metal_release(pso_c)
}

[test]
def test_metal_sq_attn_kernels(t : T?) {
    t |> run("single-query attention kernels == CPU attention oracle (sinks, softcap, split-K, window)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU kernel checks skipped\n")
                    return
                }
                var queue = metal_new_command_queue(dev)
                sq_single_gate(t, dev, queue, "plain", 40, 0.0, false)
                sq_single_gate(t, dev, queue, "softcap", 40, 30.0, false)
                sq_single_gate(t, dev, queue, "sink", 40, 0.0, true)
                sq_part_gate(t, dev, queue, "splitk", 100, 0)
                sq_part_gate(t, dev, queue, "window", 100, 48)
                metal_release(queue)
            }
            assert_no_metal_leaks()
        } else {
            feint("das_metal is not built on this platform; attention kernel checks skipped\n")
        }
    }
}

// ===== synthetic inputs + buffer plumbing =====

def private release_all(handles : array<auto(TT)>) {
    for (h in handles) {
        metal_release(h)
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

def private q_f32 : array<float> {
    var q : array<float>
    q |> resize(QD)
    for (i in range(QD)) {
        q[i] = q_at(i)
    }
    return <- q
}

def private buf_upload(dev; arr : array<auto(TT)>) : auto {
    var buf = metal_new_buffer(dev, uint64(long_length(arr)) * uint64(typeinfo sizeof(arr[0])))
    var p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(length(arr))) {
        unsafe(p[k]) = arr[k]
    }
    return buf
}

def private buf_fill(dev; nn : int; value : auto(TT)) : auto {
    var buf = metal_new_buffer(dev, uint64(nn) * uint64(typeinfo sizeof(value)))
    var p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(nn)) {
        unsafe(p[k]) = value
    }
    return buf
}

def private buf_mismatch_approx(buf; want : array<float>) : int {
    let p = unsafe(reinterpret<float const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        if (!approx(unsafe(p[k]), want[k])) {
            bad++
        }
    }
    return bad
}
