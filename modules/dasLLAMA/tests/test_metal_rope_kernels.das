options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options _function_length = 0        // per-kernel gates: fixture + builder dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_codec  // quantize_q8kv_row / quantize_tq4kv_row / fwht rows / tq4_fill_signs
require ?das_metal dasllama/dasllama_metal_kernels  // the fused rope+KV-store family (Apple builds)
require ?das_metal dasllama/dasllama_metal_common   // the pipeline globals the generated builders read
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // the MetalCell record, check_buf, the device shell
require _compares   // Bar, Shape: the cells' bars
require _kernel_oracles   // rope_tab_ref: the engine's table rope by its pairing
require math

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

// ===== the flat thread-per-pair f16/f32 mirrors (MetalRopeStoreF16 / MetalRopeStoreF32) =====

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
        var ka = RopeStoreArgs(qd = uint(qd), head_size = uint(hs), npairs_q = uint(npairs_q),
            npairs = uint(npairs), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u, rot = uint(rot))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bqk], mc.buf[bvs], 0ul, mc.buf[bkd], 0ul, mc.buf[bvd], 0ul, mc.buf[btc], 0ul, mc.buf[bts], mc.buf[bbias], ka, int64(npairs))
            }
        }
        return if (!ran)
        let heads = Shape(row_width = hs)
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

// ===== the head-cooperative form (MetalRopeStoreHF16): bias -> per-head RMSNorm -> rope -> f16 =====

def private rpst_h16_gate(t : T?; var dev, queue; hs, nheads, kv_mul, rot : int; neox, hasbias, hasnorm : bool) {
    let tag = "rpst_h16 hs={hs} nh={nheads} kvm={kv_mul} rot={rot} neox={neox} bias={hasbias} norm={hasnorm}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_rpst_h16_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nkvh = nheads / kv_mul
        let qd = nheads * hs
        let kvd = nkvh * hs
        let halfr = rot / 2
        let eps = 1.0e-5
        var inscope qk, vsrc, bias, w, tcos, tsin : array<float>
        qk |> resize(qd + kvd)
        vsrc |> resize(kvd)
        bias |> resize(qd + 2 * kvd)
        w |> resize(2 * hs)
        tcos |> resize(halfr)
        tsin |> resize(halfr)
        for (k in range(qd + kvd)) {
            qk[k] = 0.13 * float(k % 37) - 1.9
        }
        for (k in range(kvd)) {
            vsrc[k] = 0.09 * float(k % 31) - 1.3
        }
        for (k in range(qd + 2 * kvd)) {
            bias[k] = hasbias ? 0.02 * float(k % 19) - 0.2 : 0.0
        }
        for (k in range(2 * hs)) {
            w[k] = 0.6 + 0.04 * float(k % 11)
        }
        for (j in range(halfr)) {
            tcos[j] = cos(0.017 * float((j * 5) % 83))
            tsin[j] = sin(0.017 * float((j * 5) % 83))
        }
        // CPU per head: bias -> (norm) -> the shipped rope table on the post-norm row -> store
        var inscope qk_exp := qk
        var inscope v_exp := vsrc
        for (tg in range(nheads + nkvh)) {
            let isK = tg >= nheads
            let hh = isK ? tg - nheads : tg
            let base = isK ? qd + hh * hs : tg * hs
            for (e in range(hs)) {
                qk_exp[base + e] += bias[base + e]
            }
            if (hasnorm) {
                var ss = 0.0
                for (e in range(hs)) {
                    ss += qk_exp[base + e] * qk_exp[base + e]
                }
                let inv = 1.0 / sqrt(ss / float(hs) + eps)
                for (e in range(hs)) {
                    qk_exp[base + e] = w[(isK ? hs : 0) + e] * (qk_exp[base + e] * inv)
                }
            }
            rope_tab_ref(qk_exp, base, hs, rot, hs, neox, tcos, tsin)
        }
        for (k in range(kvd)) {
            v_exp[k] += bias[qd + kvd + k]
        }
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
        let bkd = mc |> out_words(kvd, float16(-1000.0))
        let bvd = mc |> out_words(kvd, float16(-1000.0))
        let btc = mc |> in_(tcos)
        let bts = mc |> in_(tsin)
        let bw = mc |> in_(w)
        let bbias = mc |> in_(bias)
        var ka = RopeStoreHArgs(qd = uint(qd), head_size = uint(hs), nheads = uint(nheads),
            kv_mul = uint(kv_mul), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u,
            has_norm = hasnorm ? 1u : 0u, eps = eps, rot = uint(rot))
        let ntg = nheads + 2 * nkvh
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bqk], mc.buf[bvs], 0ul, mc.buf[bkd], 0ul, mc.buf[bvd], 0ul, mc.buf[btc], mc.buf[bts], mc.buf[bw], mc.buf[bbias], ka, int64(ntg), int64(hs))
            }
        }
        return if (!ran)
        let heads = Shape(row_width = hs)
        check_buf(t, "{tag} q", mc.buf[bqk], want_qk, BAR_FP, heads)
        check_buf_h(t, "{tag} k", mc.buf[bkd], want_k, BAR_F16, heads)
        check_buf_h(t, "{tag} v", mc.buf[bvd], v_exp, BAR_F16, heads)
    }
}

// ===== the q8_0 mirror (MetalRopeStoreQ8): rope -> quantize_q8kv_row, byte-exact =====

[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#metal-rope-store-rot")]
def private rpst_q8_gate(t : T?; var dev, queue; hs, nheads, kv_mul : int; neox, hasbias : bool; rot : int = 0) {
    let tag = "rpst_q8 hs={hs} nh={nheads} kvm={kv_mul} neox={neox} bias={hasbias} rot={rot}"
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
        let nthreads = npairs_q + 2 * nblk
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
            nblk = uint(nblk), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u, rot = uint(rot))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bqk], mc.buf[bvs], 0ul, mc.buf[bkrow], 0ul, mc.buf[bkrow], mc.buf[bvrow], 0ul, mc.buf[bvrow], mc.buf[btc], mc.buf[bts], mc.buf[bbias], ka, int64(nthreads))
            }
        }
        return if (!ran)
        check_buf(t, "{tag} q", mc.buf[bqk], want_qk, BAR_FP, Shape(row_width = hs))
        check_buf_exact(t, "{tag} kbytes", mc.buf[bkrow], kwant)
        check_buf_exact(t, "{tag} vbytes", mc.buf[bvrow], vwant)
    }
}

// ===== the tq4 mirror (MetalRopeStoreTq4): rope -> fwht_signs_row -> quantize_tq4kv_row =====

def private rpst_tq4_gate(t : T?; var dev, queue; hs, nheads, kv_mul : int; neox, hasbias : bool; rot : int = 0) {
    let tag = "rpst_tq4 hs={hs} nh={nheads} kvm={kv_mul} neox={neox} bias={hasbias} rot={rot}"
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
            kv_mul = uint(kv_mul), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u, rot = uint(rot))
        let ntg = nheads + 2 * nkvh
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bqk], mc.buf[bvs], 0ul, mc.buf[bkrow], 0ul, mc.buf[bkrow], mc.buf[bvrow], 0ul, mc.buf[bvrow], mc.buf[btc], mc.buf[bts], mc.buf[bsg], mc.buf[bbias], ka, int64(ntg), int64(hs))
            }
        }
        return if (!ran)
        check_buf(t, "{tag} q", mc.buf[bqk], want_qk, BAR_FP, Shape(row_width = hs))
        check_buf_exact(t, "{tag} kbytes", mc.buf[bkrow], kwant)
        check_buf_exact(t, "{tag} vbytes", mc.buf[bvrow], vwant)
    }
}

// ===== the batched f16/f32 mirrors (MetalRopeStoreBF16 / BF32): row table + whole-arena compare =====

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
        var ka = RopeStoreBArgs(lay = uint(lay), kv_dim = uint(kvd), qd = uint(qd), head_size = uint(hs),
            npairs_q = uint(npairs_q), npairs = uint(npairs), qstride = uint(qstride),
            neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u, rot = uint(rot))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bqk], mc.buf[bka], mc.buf[bva], mc.buf[btc], mc.buf[bts], mc.buf[brt], 0ul, mc.buf[bbias], ka, int64(npairs), int64(nb))
            }
        }
        return if (!ran)
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

// ===== the batched q8_0 mirror (MetalRopeStoreBQ8): byte row table, byte-exact arena =====

def private rpst_bq8_gate(t : T?; var dev, queue; hs, nheads, kv_mul, nb : int; neox, hasbias : bool; rot : int = 0) {
    let tag = "rpst_b_q8 hs={hs} nh={nheads} kvm={kv_mul} nb={nb} neox={neox} bias={hasbias} rot={rot}"
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
        let nthreads = npairs_q + 2 * nblk
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
        rt |> resize(nb)
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
        for (r in range(nb)) {
            rt[r] = uint4(uint(r * 1024), uint(16384 + r * 1024), uint(laystride), uint(r + 1))
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
                let kb0b = int(rt[r].x) + lay * laystride + (int(rt[r].w) - 1) * rowb
                let vb0b = int(rt[r].y) + lay * laystride + (int(rt[r].w) - 1) * rowb
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
            npairs_q = uint(npairs_q), qstride = uint(qstride), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u, rot = uint(rot))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bqk], mc.buf[barena], mc.buf[barena], mc.buf[barena], mc.buf[barena], mc.buf[btc], mc.buf[bts], mc.buf[brt], mc.buf[bbias], ka, int64(nthreads), int64(nb))
            }
        }
        return if (!ran)
        check_buf(t, "{tag} q", mc.buf[bqk], want_qk, BAR_FP, Shape(row_width = qstride))
        check_buf_exact(t, "{tag} arena", mc.buf[barena], want_a)
    }
}

// ===== the batched tq4 mirror (MetalRopeStoreBTq4): head-cooperative + byte row table =====

def private rpst_btq4_gate(t : T?; var dev, queue; hs, nheads, kv_mul, nb : int; neox, hasbias : bool; rot : int = 0) {
    let tag = "rpst_b_tq4 hs={hs} nh={nheads} kvm={kv_mul} nb={nb} neox={neox} bias={hasbias} rot={rot}"
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
        rt |> resize(nb)
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
        for (r in range(nb)) {
            rt[r] = uint4(uint(r * 1024), uint(16384 + r * 1024), uint(laystride), uint(r + 1))
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
                let kb0b = int(rt[r].x) + lay * laystride + (int(rt[r].w) - 1) * rowb
                let vb0b = int(rt[r].y) + lay * laystride + (int(rt[r].w) - 1) * rowb
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
            kv_mul = uint(kv_mul), qstride = uint(qstride), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u, rot = uint(rot))
        let ntg = nheads + 2 * nkvh
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bqk], mc.buf[barena], mc.buf[barena], mc.buf[barena], mc.buf[barena], mc.buf[btc], mc.buf[bts], mc.buf[bsg], mc.buf[brt], mc.buf[bbias], ka, int64(ntg), int64(nb), int64(hs))
            }
        }
        return if (!ran)
        check_buf(t, "{tag} q", mc.buf[bqk], want_qk, BAR_FP, Shape(row_width = qstride))
        check_buf_exact(t, "{tag} arena", mc.buf[barena], want_a)
    }
}

// ===== the tq4 un-rotate (MetalFwhtUnsign) vs fwht_unsign_row =====

def private unsign_gate(t : T?; var dev, queue; hs, nheads, nrows, pad : int) {
    let tag = "fwht_unsign hs={hs} nh={nheads} rows={nrows} pad={pad}"
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
        let byo = mc |> io_(yo)
        let bsg = mc |> in_(signs)
        let bhs = mc |> in_plain([uint(hs)])
        let bqs = mc |> in_plain([uint(qstride)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[byo], mc.buf[bsg], mc.buf[bhs], mc.buf[bqs], int64(nheads), int64(nrows), int64(hs))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[byo], want, BAR_FP, Shape(row_width = qstride))
    }
}

[test]
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
                // head-cooperative: the QK-norm fold on and off, plus the gemma4-ish wide head
                rpst_h16_gate(t, dev, queue, 64, 4, 2, 64, true, true, true)
                rpst_h16_gate(t, dev, queue, 64, 4, 2, 64, false, false, false)
                rpst_h16_gate(t, dev, queue, 128, 2, 2, 128, true, false, true)
                // quantized mirrors: byte-exact vs quantize_q8kv_row / quantize_tq4kv_row
                rpst_q8_gate(t, dev, queue, 64, 4, 2, true, true)
                rpst_q8_gate(t, dev, queue, 64, 4, 2, false, false)
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
            }
        } else {
            t |> skip("das_metal is not built on this platform; rope-store kernel checks skipped")
        }
    }
}
