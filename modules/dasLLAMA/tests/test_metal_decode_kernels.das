options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options indenting = 4
options _dasllama_internal = true

require dastest/testing_boost public
require ?das_metal dasllama/dasllama_metal_kernels  // the decode kernel set (Apple builds)
require ?das_metal dasllama/dasllama_metal_common   // the pipeline globals the generated builders read
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // the MetalCell record, check_buf, the device shell
require _attn_oracle
require _compares   // Bar: the cells' bar
require _kernel_oracles   // attn_q_row: the query row the codec cells read too
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

let private BAR_ATTN = Bar(rel = 2e-3, poison_scaled = true)   //! reduction-order noise against the f16-exact fixture; the poisoned last element moves 0.5 of its magnitude floored at 1

// K/V hold f16-exact small values so the oracle sees the same numbers the kernel loads
def private kv_at(j, d : int) : float => float((j * 7 + d * 3) % 9 - 4) * 0.25

//! every head's attention row at the test shape, K and V both the one slab the gates bind twice; `window` > 0 starts the keys at cnt - window
def private oracle_all(cnt : int; scale, softcap : float; hass : bool; sink : float; window : int) : array<float> {
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

// the single-threadgroup form: one dispatch, the whole context in one pass
def private sq_single_gate(t : T?; var dev, queue; tag : string; cnt : int; softcap : float; hass : bool) {
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_attn16_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, "{tag} sq_attn16", pso, err))
        let scale = 1.0 / sqrt(float(HS))
        let sink_v = 0.375
        var inscope kc <- kv_f16(cnt)
        let bk = mc |> in_plain(kc)
        let bv = mc |> in_plain(kc)            // V reads the same slab — the oracle mirrors that
        let bq = mc |> in_(attn_q_row(QD))
        let by = mc |> out_words(QD, -1000.0)
        let bsink = mc |> in_(plane_of(N_HEADS, sink_v))
        var ka = SqAttnArgs(cnt = uint(cnt), kstride = uint(KV_DIM), nheads = uint(N_HEADS),
            kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale, softcap = softcap,
            hass = hass ? 1u : 0u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bk], 0ul, mc.buf[bv], 0ul, mc.buf[bq], mc.buf[by], ka, mc.buf[bsink], int64(N_HEADS), int64(HS))
            }
        }
        return if (!ran)
        var inscope want <- oracle_all(cnt, scale, softcap, hass, sink_v, 0)
        check_buf(t, "decode {tag}", mc.buf[by], want, BAR_ATTN, Shape(row_width = HS))
    }
}

// the split-K pair: one threadgroup per (head, chunk) writes an unnormalized partial, the comb
// kernel rescales across chunks. `window` > 0 also exercises the chunk mask + the chlo skip.
def private sq_part_gate(t : T?; var dev, queue; tag : string; cnt, window : int) {
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
        let bk = mc |> in_plain(kc)
        let bv = mc |> in_plain(kc)
        let bq = mc |> in_(attn_q_row(QD))
        let bpart = mc |> out_words(N_HEADS * nchunks * (HS + 2), -1000.0)
        let by = mc |> out_words(QD, -1000.0)
        let bsink = mc |> in_(plane_of(N_HEADS, 0.0))
        var kap = SqAttnPartArgs(cnt = uint(cnt), kstride = uint(KV_DIM), nheads = uint(N_HEADS),
            kv_mul = uint(KV_MUL), hs = uint(HS), scale = scale, nchunks = uint(nchunks),
            softcap = 0.0, window = uint(window))
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
        var inscope want <- oracle_all(cnt, scale, 0.0, false, 0.0, window)
        check_buf(t, "decode {tag}", mc.buf[by], want, BAR_ATTN, Shape(row_width = HS))
    }
}

[test]
def test_metal_sq_attn_kernels(t : T?) {
    t |> run("single-query attention kernels == CPU attention oracle (sinks, softcap, split-K, window)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_gate(t) $(var dev : MetalDevice?; var queue : MetalCommandQueue?) {
                sq_single_gate(t, dev, queue, "plain", 40, 0.0, false)
                sq_single_gate(t, dev, queue, "softcap", 40, 30.0, false)
                sq_single_gate(t, dev, queue, "sink", 40, 0.0, true)
                sq_part_gate(t, dev, queue, "splitk", 100, 0)
                sq_part_gate(t, dev, queue, "window", 100, 48)
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
