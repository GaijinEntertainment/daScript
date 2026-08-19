options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options _function_length = 0        // per-kernel gates: hand-bound buffers/uniforms + dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_math      // rope_scaled_tab / rope_scaled_neox_tab(_part) — the CPU rope truth
require dasllama/dasllama_kv_codec  // quantize_q8kv_row / quantize_tq4kv_row / fwht rows / tq4_fill_signs
require ?das_metal dasllama/dasllama_metal_kernels  // the fused rope+KV-store family (Apple builds)
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // shared buf helpers + eyeball-dump compares
require math

// The fused rope + KV-row-store family (all four mirror codecs, flat + head-cooperative +
// batched) vs the shipped CPU seam on synthetic data: the CPU side is the SAME rope tables
// (rope_scaled_*_tab) followed by the SAME codec stores (quantize_q8kv_row / fwht_signs_row +
// quantize_tq4kv_row) the CPU fallback runs — so the quantized mirrors compare BYTE-EXACT (the
// codec contract: CPU fallback steps read what the GPU stored). f32 rope outputs compare within
// the shared fp bar; f16 mirrors within the f16-rounding envelope. The batched arms compare the
// WHOLE mirror arena against a sentinel-filled expectation, so a stray write outside the row
// table's target row is a red, not silence. Off-Apple (or with no Metal device) every check
// feints cleanly.

// ===== the flat thread-per-pair f16/f32 mirrors (MetalRopeStoreF16 / MetalRopeStoreF32) =====

def private rpst_flat_gate(t : T?; dev, queue; f16dst : bool; hs, nheads, kv_mul, rot : int; neox, hasbias : bool) {
    let tag = "rpst{f16dst ? 16 : 32} hs={hs} nh={nheads} kvm={kv_mul} rot={rot} neox={neox} bias={hasbias}"
    var err : string
    var pso = (f16dst
        ? pipeline_from_source(dev, MetalRopeStoreF16_metal_rope_store_msl, MetalRopeStoreF16_metal_rope_store_msl_entry, MetalRopeStoreF16_metal_rope_store_msl_fastmath, err)
        : pipeline_from_source(dev, MetalRopeStoreF32_metal_rope_store_msl, MetalRopeStoreF32_metal_rope_store_msl_entry, MetalRopeStoreF32_metal_rope_store_msl_fastmath, err))
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nkvh = nheads / kv_mul
    let qd = nheads * hs
    let kvd = nkvh * hs
    let half = hs / 2
    let halfr = rot / 2
    let npairs_q = qd / 2
    let npairs = (qd + kvd) / 2
    var qk : array<float>
    var vsrc : array<float>
    var bias : array<float>
    var tcos : array<float>
    var tsin : array<float>
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
    var qk_exp := qk
    var v_exp := vsrc
    if (hasbias) {
        for (k in range(qd + kvd)) {
            qk_exp[k] += bias[k]
        }
        for (k in range(kvd)) {
            v_exp[k] += bias[qd + kvd + k]
        }
    }
    unsafe {
        if (!neox) {
            rope_scaled_tab(addr(qk_exp[0]), int64(hs), int64(qd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
            rope_scaled_tab(addr(qk_exp[qd]), int64(hs), int64(kvd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
        } elif (rot == hs) {
            rope_scaled_neox_tab(addr(qk_exp[0]), int64(hs), int64(qd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
            rope_scaled_neox_tab(addr(qk_exp[qd]), int64(hs), int64(kvd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
        } else {
            rope_scaled_neox_tab_part(addr(qk_exp[0]), int64(hs), int64(rot), int64(qd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
            rope_scaled_neox_tab_part(addr(qk_exp[qd]), int64(hs), int64(rot), int64(kvd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
        }
    }
    // the kernel ropes q IN PLACE and k INTO THE MIRROR — the k third of qk must stay raw
    var want_qk := qk
    for (k in range(qd)) {
        want_qk[k] = qk_exp[k]
    }
    var want_k : array<float>
    want_k |> resize(kvd)
    for (k in range(kvd)) {
        want_k[k] = qk_exp[qd + k]
    }
    var bqk = buf_upload(dev, qk)
    var bvs = buf_upload(dev, vsrc)
    var bkd = f16dst ? buf_fill(dev, kvd, float16(-1000.0)) : buf_fill(dev, kvd, -1000.0)
    var bvd = f16dst ? buf_fill(dev, kvd, float16(-1000.0)) : buf_fill(dev, kvd, -1000.0)
    var btc = buf_upload(dev, tcos)
    var bts = buf_upload(dev, tsin)
    var bbias = buf_upload(dev, bias)
    var ka = RopeStoreArgs(qd = uint(qd), head_size = uint(hs), npairs_q = uint(npairs_q),
        npairs = uint(npairs), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u, rot = uint(rot))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bqk, 0ul, 0)
        metal_set_buffer(enc, bvs, 0ul, 1)
        metal_set_buffer(enc, bkd, 0ul, 2)
        metal_set_buffer(enc, bvd, 0ul, 3)
        metal_set_buffer(enc, btc, 0ul, 4)
        metal_set_buffer(enc, bts, 0ul, 5)
        metal_set_buffer(enc, bbias, 0ul, 6)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 7)
        metal_dispatch_threadgroups(enc, uint3(uint((npairs + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bqk, want_qk, "{tag} q"), 0)
        if (f16dst) {
            t |> equal(buf_mismatch_f16(bkd, want_k, 1e-3, 1e-3, "{tag} k"), 0)
            t |> equal(buf_mismatch_f16(bvd, v_exp, 1e-3, 1e-3, "{tag} v"), 0)
        } else {
            t |> equal(buf_mismatch_approx(bkd, want_k, "{tag} k"), 0)
            t |> equal(buf_mismatch_approx(bvd, v_exp, "{tag} v"), 0)
        }
    }
    metal_release(bqk)
    metal_release(bvs)
    metal_release(bkd)
    metal_release(bvd)
    metal_release(btc)
    metal_release(bts)
    metal_release(bbias)
    metal_release(pso)
    delete qk
    delete vsrc
    delete bias
    delete tcos
    delete tsin
    delete qk_exp
    delete v_exp
    delete want_qk
    delete want_k
}

// ===== the head-cooperative form (MetalRopeStoreHF16): bias -> per-head RMSNorm -> rope -> f16 =====

def private rpst_h16_gate(t : T?; dev, queue; hs, nheads, kv_mul, rot : int; neox, hasbias, hasnorm : bool) {
    let tag = "rpst_h16 hs={hs} nh={nheads} kvm={kv_mul} rot={rot} neox={neox} bias={hasbias} norm={hasnorm}"
    var err : string
    var pso = pipeline_from_source(dev, metal_rope_store_h16_msl, metal_rope_store_h16_msl_entry, metal_rope_store_h16_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nkvh = nheads / kv_mul
    let qd = nheads * hs
    let kvd = nkvh * hs
    let halfr = rot / 2
    let eps = 1.0e-5
    var qk : array<float>
    var vsrc : array<float>
    var bias : array<float>
    var w : array<float>
    var tcos : array<float>
    var tsin : array<float>
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
    var qk_exp := qk
    var v_exp := vsrc
    unsafe {
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
            if (neox && rot == hs) {
                rope_scaled_neox_tab(addr(qk_exp[base]), int64(hs), int64(hs), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
            } elif (neox) {
                rope_scaled_neox_tab_part(addr(qk_exp[base]), int64(hs), int64(rot), int64(hs), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
            } else {
                rope_scaled_tab(addr(qk_exp[base]), int64(hs), int64(hs), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
            }
        }
        for (k in range(kvd)) {
            v_exp[k] += bias[qd + kvd + k]
        }
    }
    var want_qk := qk
    for (k in range(qd)) {
        want_qk[k] = qk_exp[k]
    }
    var want_k : array<float>
    want_k |> resize(kvd)
    for (k in range(kvd)) {
        want_k[k] = qk_exp[qd + k]
    }
    var bqk = buf_upload(dev, qk)
    var bvs = buf_upload(dev, vsrc)
    var bkd = buf_fill(dev, kvd, float16(-1000.0))
    var bvd = buf_fill(dev, kvd, float16(-1000.0))
    var btc = buf_upload(dev, tcos)
    var bts = buf_upload(dev, tsin)
    var bw = buf_upload(dev, w)
    var bbias = buf_upload(dev, bias)
    var ka = RopeStoreHArgs(qd = uint(qd), head_size = uint(hs), nheads = uint(nheads),
        kv_mul = uint(kv_mul), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u,
        has_norm = hasnorm ? 1u : 0u, eps = eps, rot = uint(rot))
    let ntg = nheads + 2 * nkvh
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_rope_store_h16_msl_tgmem, 0)
        metal_set_buffer(enc, bqk, 0ul, 0)
        metal_set_buffer(enc, bvs, 0ul, 1)
        metal_set_buffer(enc, bkd, 0ul, 2)
        metal_set_buffer(enc, bvd, 0ul, 3)
        metal_set_buffer(enc, btc, 0ul, 4)
        metal_set_buffer(enc, bts, 0ul, 5)
        metal_set_buffer(enc, bw, 0ul, 6)
        metal_set_buffer(enc, bbias, 0ul, 7)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 8)
        metal_dispatch_threadgroups(enc, uint3(uint(ntg), 1u, 1u), uint3(uint(hs), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bqk, want_qk, "{tag} q"), 0)
        t |> equal(buf_mismatch_f16(bkd, want_k, 1e-3, 1e-3, "{tag} k"), 0)
        t |> equal(buf_mismatch_f16(bvd, v_exp, 1e-3, 1e-3, "{tag} v"), 0)
    }
    metal_release(bqk)
    metal_release(bvs)
    metal_release(bkd)
    metal_release(bvd)
    metal_release(btc)
    metal_release(bts)
    metal_release(bw)
    metal_release(bbias)
    metal_release(pso)
    delete qk
    delete vsrc
    delete bias
    delete w
    delete tcos
    delete tsin
    delete qk_exp
    delete v_exp
    delete want_qk
    delete want_k
}

// ===== the q8_0 mirror (MetalRopeStoreQ8): rope -> quantize_q8kv_row, byte-exact =====

def private rpst_q8_gate(t : T?; dev, queue; hs, nheads, kv_mul : int; neox, hasbias : bool) {
    let tag = "rpst_q8 hs={hs} nh={nheads} kvm={kv_mul} neox={neox} bias={hasbias}"
    var err : string
    var pso = pipeline_from_source(dev, metal_rope_store_q8_msl, metal_rope_store_q8_msl_entry, metal_rope_store_q8_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nkvh = nheads / kv_mul
    let qd = nheads * hs
    let kvd = nkvh * hs
    let half = hs / 2
    let nblk = kvd / 32
    let npairs_q = qd / 2
    let nthreads = npairs_q + 2 * nblk
    var qk : array<float>
    var vsrc : array<float>
    var bias : array<float>
    var tcos : array<float>
    var tsin : array<float>
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
    var qk_exp := qk
    var v_exp := vsrc
    if (hasbias) {
        for (k in range(qd + kvd)) {
            qk_exp[k] += bias[k]
        }
        for (k in range(kvd)) {
            v_exp[k] += bias[qd + kvd + k]
        }
    }
    var kwant : array<uint8>
    var vwant : array<uint8>
    kwant |> resize(nblk * 34)
    vwant |> resize(nblk * 34)
    unsafe {
        if (neox) {
            rope_scaled_neox_tab(addr(qk_exp[0]), int64(hs), int64(qd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
            rope_scaled_neox_tab(addr(qk_exp[qd]), int64(hs), int64(kvd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
        } else {
            rope_scaled_tab(addr(qk_exp[0]), int64(hs), int64(qd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
            rope_scaled_tab(addr(qk_exp[qd]), int64(hs), int64(kvd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
        }
        quantize_q8kv_row(addr(kwant[0]), addr<float const?>(qk_exp[qd]), int64(kvd))
        quantize_q8kv_row(addr(vwant[0]), addr<float const?>(v_exp[0]), int64(kvd))
    }
    var want_qk := qk
    for (k in range(qd)) {
        want_qk[k] = qk_exp[k]
    }
    var bqk = buf_upload(dev, qk)
    var bvs = buf_upload(dev, vsrc)
    var bkrow = buf_fill(dev, nblk * 34, uint8(0xA5))
    var bvrow = buf_fill(dev, nblk * 34, uint8(0xA5))
    var btc = buf_upload(dev, tcos)
    var bts = buf_upload(dev, tsin)
    var bbias = buf_upload(dev, bias)
    var ka = RopeStoreQ8Args(qd = uint(qd), head_size = uint(hs), npairs_q = uint(npairs_q),
        nblk = uint(nblk), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bqk, 0ul, 0)
        metal_set_buffer(enc, bvs, 0ul, 1)
        metal_set_buffer(enc, bkrow, 0ul, 2)
        metal_set_buffer(enc, bkrow, 0ul, 3)
        metal_set_buffer(enc, bvrow, 0ul, 4)
        metal_set_buffer(enc, bvrow, 0ul, 5)
        metal_set_buffer(enc, btc, 0ul, 6)
        metal_set_buffer(enc, bts, 0ul, 7)
        metal_set_buffer(enc, bbias, 0ul, 8)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 9)
        metal_dispatch_threadgroups(enc, uint3(uint((nthreads + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bqk, want_qk, "{tag} q"), 0)
        t |> equal(buf_mismatch_exact(bkrow, kwant, "{tag} kbytes"), 0)
        t |> equal(buf_mismatch_exact(bvrow, vwant, "{tag} vbytes"), 0)
    }
    metal_release(bqk)
    metal_release(bvs)
    metal_release(bkrow)
    metal_release(bvrow)
    metal_release(btc)
    metal_release(bts)
    metal_release(bbias)
    metal_release(pso)
    delete qk
    delete vsrc
    delete bias
    delete tcos
    delete tsin
    delete qk_exp
    delete v_exp
    delete kwant
    delete vwant
    delete want_qk
}

// ===== the tq4 mirror (MetalRopeStoreTq4): rope -> fwht_signs_row -> quantize_tq4kv_row =====

def private rpst_tq4_gate(t : T?; dev, queue; hs, nheads, kv_mul : int; neox, hasbias : bool) {
    let tag = "rpst_tq4 hs={hs} nh={nheads} kvm={kv_mul} neox={neox} bias={hasbias}"
    var err : string
    var pso = pipeline_from_source(dev, metal_rope_store_tq4_msl, metal_rope_store_tq4_msl_entry, metal_rope_store_tq4_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nkvh = nheads / kv_mul
    let qd = nheads * hs
    let kvd = nkvh * hs
    let half = hs / 2
    let rowb = (kvd / 32) * 18
    var qk : array<float>
    var vsrc : array<float>
    var bias : array<float>
    var tcos : array<float>
    var tsin : array<float>
    var signs : array<float>
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
    var qk_exp := qk
    var v_exp := vsrc
    if (hasbias) {
        for (k in range(qd + kvd)) {
            qk_exp[k] += bias[k]
        }
        for (k in range(kvd)) {
            v_exp[k] += bias[qd + kvd + k]
        }
    }
    var kwant : array<uint8>
    var vwant : array<uint8>
    kwant |> resize(rowb)
    vwant |> resize(rowb)
    unsafe {
        if (neox) {
            rope_scaled_neox_tab(addr(qk_exp[0]), int64(hs), int64(qd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
            rope_scaled_neox_tab(addr(qk_exp[qd]), int64(hs), int64(kvd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
        } else {
            rope_scaled_tab(addr(qk_exp[0]), int64(hs), int64(qd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
            rope_scaled_tab(addr(qk_exp[qd]), int64(hs), int64(kvd), addr<float const?>(tcos[0]), addr<float const?>(tsin[0]))
        }
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
    var want_qk := qk
    for (k in range(qd)) {
        want_qk[k] = qk_exp[k]
    }
    var bqk = buf_upload(dev, qk)
    var bvs = buf_upload(dev, vsrc)
    var bkrow = buf_fill(dev, rowb, uint8(0xA5))
    var bvrow = buf_fill(dev, rowb, uint8(0xA5))
    var btc = buf_upload(dev, tcos)
    var bts = buf_upload(dev, tsin)
    var bsg = buf_upload(dev, signs)
    var bbias = buf_upload(dev, bias)
    var ka = RopeStoreTq4Args(qd = uint(qd), head_size = uint(hs), nheads = uint(nheads),
        kv_mul = uint(kv_mul), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u)
    let ntg = nheads + 2 * nkvh
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_rope_store_tq4_msl_tgmem, 0)
        metal_set_buffer(enc, bqk, 0ul, 0)
        metal_set_buffer(enc, bvs, 0ul, 1)
        metal_set_buffer(enc, bkrow, 0ul, 2)
        metal_set_buffer(enc, bkrow, 0ul, 3)
        metal_set_buffer(enc, bvrow, 0ul, 4)
        metal_set_buffer(enc, bvrow, 0ul, 5)
        metal_set_buffer(enc, btc, 0ul, 6)
        metal_set_buffer(enc, bts, 0ul, 7)
        metal_set_buffer(enc, bsg, 0ul, 8)
        metal_set_buffer(enc, bbias, 0ul, 9)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 10)
        metal_dispatch_threadgroups(enc, uint3(uint(ntg), 1u, 1u), uint3(uint(hs), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bqk, want_qk, "{tag} q"), 0)
        t |> equal(buf_mismatch_exact(bkrow, kwant, "{tag} kbytes"), 0)
        t |> equal(buf_mismatch_exact(bvrow, vwant, "{tag} vbytes"), 0)
    }
    metal_release(bqk)
    metal_release(bvs)
    metal_release(bkrow)
    metal_release(bvrow)
    metal_release(btc)
    metal_release(bts)
    metal_release(bsg)
    metal_release(bbias)
    metal_release(pso)
    delete qk
    delete vsrc
    delete bias
    delete tcos
    delete tsin
    delete signs
    delete qk_exp
    delete v_exp
    delete kwant
    delete vwant
    delete want_qk
}

// ===== the batched f16/f32 mirrors (MetalRopeStoreBF16 / BF32): row table + whole-arena compare =====

def private rpst_b_gate(t : T?; dev, queue; f16dst : bool; hs, nheads, kv_mul, rot, nb : int; neox, hasbias : bool) {
    let tag = "rpst_b{f16dst ? 16 : 32} hs={hs} nh={nheads} kvm={kv_mul} rot={rot} nb={nb} neox={neox} bias={hasbias}"
    var err : string
    var pso = (f16dst
        ? pipeline_from_source(dev, MetalRopeStoreBF16_metal_rope_store_b_msl, MetalRopeStoreBF16_metal_rope_store_b_msl_entry, MetalRopeStoreBF16_metal_rope_store_b_msl_fastmath, err)
        : pipeline_from_source(dev, MetalRopeStoreBF32_metal_rope_store_b_msl, MetalRopeStoreBF32_metal_rope_store_b_msl_entry, MetalRopeStoreBF32_metal_rope_store_b_msl_fastmath, err))
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
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
    var qk : array<float>
    var bias : array<float>
    var tcos : array<float>
    var tsin : array<float>
    var rt : array<uint4>
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
    var want_qk := qk
    var want_ka : array<float>
    var want_va : array<float>
    want_ka |> resize(asz)
    want_va |> resize(asz)
    for (k in range(asz)) {
        want_ka[k] = -1000.0
        want_va[k] = -1000.0
    }
    unsafe {
        for (r in range(nb)) {
            let qb = r * qstride
            if (hasbias) {
                for (k in range(qd + kvd)) {
                    want_qk[qb + k] += bias[k]
                }
            }
            if (neox && rot == hs) {
                rope_scaled_neox_tab(addr(want_qk[qb]), int64(hs), int64(qd), addr<float const?>(tcos[r * halfr]), addr<float const?>(tsin[r * halfr]))
                rope_scaled_neox_tab(addr(want_qk[qb + qd]), int64(hs), int64(kvd), addr<float const?>(tcos[r * halfr]), addr<float const?>(tsin[r * halfr]))
            } elif (neox) {
                rope_scaled_neox_tab_part(addr(want_qk[qb]), int64(hs), int64(rot), int64(qd), addr<float const?>(tcos[r * halfr]), addr<float const?>(tsin[r * halfr]))
                rope_scaled_neox_tab_part(addr(want_qk[qb + qd]), int64(hs), int64(rot), int64(kvd), addr<float const?>(tcos[r * halfr]), addr<float const?>(tsin[r * halfr]))
            } else {
                rope_scaled_tab(addr(want_qk[qb]), int64(hs), int64(qd), addr<float const?>(tcos[r * halfr]), addr<float const?>(tsin[r * halfr]))
                rope_scaled_tab(addr(want_qk[qb + qd]), int64(hs), int64(kvd), addr<float const?>(tcos[r * halfr]), addr<float const?>(tsin[r * halfr]))
            }
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
    }
    var bqk = buf_upload(dev, qk)
    var bka = f16dst ? buf_fill(dev, asz, float16(-1000.0)) : buf_fill(dev, asz, -1000.0)
    var bva = f16dst ? buf_fill(dev, asz, float16(-1000.0)) : buf_fill(dev, asz, -1000.0)
    var btc = buf_upload(dev, tcos)
    var bts = buf_upload(dev, tsin)
    var brt = buf_upload(dev, rt)
    var bbias = buf_upload(dev, bias)
    var ka = RopeStoreBArgs(lay = uint(lay), kv_dim = uint(kvd), qd = uint(qd), head_size = uint(hs),
        npairs_q = uint(npairs_q), npairs = uint(npairs), qstride = uint(qstride),
        neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u, rot = uint(rot))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bqk, 0ul, 0)
        metal_set_buffer(enc, bka, 0ul, 1)
        metal_set_buffer(enc, bva, 0ul, 2)
        metal_set_buffer(enc, btc, 0ul, 3)
        metal_set_buffer(enc, bts, 0ul, 4)
        metal_set_buffer(enc, brt, 0ul, 5)
        metal_set_buffer(enc, bbias, 0ul, 6)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 7)
        metal_dispatch_threadgroups(enc, uint3(uint((npairs + 63) / 64), uint(nb), 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bqk, want_qk, "{tag} q"), 0)
        if (f16dst) {
            t |> equal(buf_mismatch_f16(bka, want_ka, 1e-3, 1e-3, "{tag} karena"), 0)
            t |> equal(buf_mismatch_f16(bva, want_va, 1e-3, 1e-3, "{tag} varena"), 0)
        } else {
            t |> equal(buf_mismatch_approx(bka, want_ka, "{tag} karena"), 0)
            t |> equal(buf_mismatch_approx(bva, want_va, "{tag} varena"), 0)
        }
    }
    metal_release(bqk)
    metal_release(bka)
    metal_release(bva)
    metal_release(btc)
    metal_release(bts)
    metal_release(brt)
    metal_release(bbias)
    metal_release(pso)
    delete qk
    delete bias
    delete tcos
    delete tsin
    delete rt
    delete want_qk
    delete want_ka
    delete want_va
}

// ===== the batched q8_0 mirror (MetalRopeStoreBQ8): byte row table, byte-exact arena =====

def private rpst_bq8_gate(t : T?; dev, queue; hs, nheads, kv_mul, nb : int; neox, hasbias : bool) {
    let tag = "rpst_b_q8 hs={hs} nh={nheads} kvm={kv_mul} nb={nb} neox={neox} bias={hasbias}"
    var err : string
    var pso = pipeline_from_source(dev, metal_rope_store_b_q8_msl, metal_rope_store_b_q8_msl_entry, metal_rope_store_b_q8_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nkvh = nheads / kv_mul
    let qd = nheads * hs
    let kvd = nkvh * hs
    let half = hs / 2
    let nblk = kvd / 32
    let rowb = nblk * 34
    let npairs_q = qd / 2
    let nthreads = npairs_q + 2 * nblk
    let qstride = qd + 2 * kvd
    let lay = 1
    let laystride = 8192
    let asz = 28672
    var qk : array<float>
    var bias : array<float>
    var tcos : array<float>
    var tsin : array<float>
    var rt : array<uint4>
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
    var want_qk := qk
    var want_a : array<uint8>
    want_a |> resize(asz)
    for (k in range(asz)) {
        want_a[k] = uint8(0xA5)
    }
    unsafe {
        for (r in range(nb)) {
            let qb = r * qstride
            var krow : array<float>
            var vrow : array<float>
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
            if (neox) {
                rope_scaled_neox_tab(addr(want_qk[qb]), int64(hs), int64(qd), addr<float const?>(tcos[r * half]), addr<float const?>(tsin[r * half]))
                rope_scaled_neox_tab(addr(krow[0]), int64(hs), int64(kvd), addr<float const?>(tcos[r * half]), addr<float const?>(tsin[r * half]))
            } else {
                rope_scaled_tab(addr(want_qk[qb]), int64(hs), int64(qd), addr<float const?>(tcos[r * half]), addr<float const?>(tsin[r * half]))
                rope_scaled_tab(addr(krow[0]), int64(hs), int64(kvd), addr<float const?>(tcos[r * half]), addr<float const?>(tsin[r * half]))
            }
            let kb0b = int(rt[r].x) + lay * laystride + (int(rt[r].w) - 1) * rowb
            let vb0b = int(rt[r].y) + lay * laystride + (int(rt[r].w) - 1) * rowb
            quantize_q8kv_row(addr(want_a[kb0b]), addr<float const?>(krow[0]), int64(kvd))
            quantize_q8kv_row(addr(want_a[vb0b]), addr<float const?>(vrow[0]), int64(kvd))
            delete krow
            delete vrow
        }
    }
    var bqk = buf_upload(dev, qk)
    var barena = buf_fill(dev, asz, uint8(0xA5))
    var btc = buf_upload(dev, tcos)
    var bts = buf_upload(dev, tsin)
    var brt = buf_upload(dev, rt)
    var bbias = buf_upload(dev, bias)
    var ka = RopeStoreBQ8Args(lay = uint(lay), nblk = uint(nblk), qd = uint(qd), head_size = uint(hs),
        npairs_q = uint(npairs_q), qstride = uint(qstride), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bqk, 0ul, 0)
        metal_set_buffer(enc, barena, 0ul, 1)
        metal_set_buffer(enc, barena, 0ul, 2)
        metal_set_buffer(enc, barena, 0ul, 3)
        metal_set_buffer(enc, barena, 0ul, 4)
        metal_set_buffer(enc, btc, 0ul, 5)
        metal_set_buffer(enc, bts, 0ul, 6)
        metal_set_buffer(enc, brt, 0ul, 7)
        metal_set_buffer(enc, bbias, 0ul, 8)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 9)
        metal_dispatch_threadgroups(enc, uint3(uint((nthreads + 63) / 64), uint(nb), 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bqk, want_qk, "{tag} q"), 0)
        t |> equal(buf_mismatch_exact(barena, want_a, "{tag} arena"), 0)
    }
    metal_release(bqk)
    metal_release(barena)
    metal_release(btc)
    metal_release(bts)
    metal_release(brt)
    metal_release(bbias)
    metal_release(pso)
    delete qk
    delete bias
    delete tcos
    delete tsin
    delete rt
    delete want_qk
    delete want_a
}

// ===== the batched tq4 mirror (MetalRopeStoreBTq4): head-cooperative + byte row table =====

def private rpst_btq4_gate(t : T?; dev, queue; hs, nheads, kv_mul, nb : int; neox, hasbias : bool) {
    let tag = "rpst_b_tq4 hs={hs} nh={nheads} kvm={kv_mul} nb={nb} neox={neox} bias={hasbias}"
    var err : string
    var pso = pipeline_from_source(dev, metal_rope_store_b_tq4_msl, metal_rope_store_b_tq4_msl_entry, metal_rope_store_b_tq4_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nkvh = nheads / kv_mul
    let qd = nheads * hs
    let kvd = nkvh * hs
    let half = hs / 2
    let rowb = (kvd / 32) * 18
    let qstride = qd + 2 * kvd
    let lay = 1
    let laystride = 8192
    let asz = 28672
    var qk : array<float>
    var bias : array<float>
    var tcos : array<float>
    var tsin : array<float>
    var signs : array<float>
    var rt : array<uint4>
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
    var want_qk := qk
    var want_a : array<uint8>
    want_a |> resize(asz)
    for (k in range(asz)) {
        want_a[k] = uint8(0xA5)
    }
    unsafe {
        for (r in range(nb)) {
            let qb = r * qstride
            var krow : array<float>
            var vrow : array<float>
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
            if (neox) {
                rope_scaled_neox_tab(addr(want_qk[qb]), int64(hs), int64(qd), addr<float const?>(tcos[r * half]), addr<float const?>(tsin[r * half]))
                rope_scaled_neox_tab(addr(krow[0]), int64(hs), int64(kvd), addr<float const?>(tcos[r * half]), addr<float const?>(tsin[r * half]))
            } else {
                rope_scaled_tab(addr(want_qk[qb]), int64(hs), int64(qd), addr<float const?>(tcos[r * half]), addr<float const?>(tsin[r * half]))
                rope_scaled_tab(addr(krow[0]), int64(hs), int64(kvd), addr<float const?>(tcos[r * half]), addr<float const?>(tsin[r * half]))
            }
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
            delete krow
            delete vrow
        }
    }
    var bqk = buf_upload(dev, qk)
    var barena = buf_fill(dev, asz, uint8(0xA5))
    var btc = buf_upload(dev, tcos)
    var bts = buf_upload(dev, tsin)
    var bsg = buf_upload(dev, signs)
    var brt = buf_upload(dev, rt)
    var bbias = buf_upload(dev, bias)
    var ka = RopeStoreBTq4Args(lay = uint(lay), qd = uint(qd), head_size = uint(hs), nheads = uint(nheads),
        kv_mul = uint(kv_mul), qstride = uint(qstride), neox = neox ? 1u : 0u, has_bias = hasbias ? 1u : 0u)
    let ntg = nheads + 2 * nkvh
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_rope_store_b_tq4_msl_tgmem, 0)
        metal_set_buffer(enc, bqk, 0ul, 0)
        metal_set_buffer(enc, barena, 0ul, 1)
        metal_set_buffer(enc, barena, 0ul, 2)
        metal_set_buffer(enc, barena, 0ul, 3)
        metal_set_buffer(enc, barena, 0ul, 4)
        metal_set_buffer(enc, btc, 0ul, 5)
        metal_set_buffer(enc, bts, 0ul, 6)
        metal_set_buffer(enc, bsg, 0ul, 7)
        metal_set_buffer(enc, brt, 0ul, 8)
        metal_set_buffer(enc, bbias, 0ul, 9)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 10)
        metal_dispatch_threadgroups(enc, uint3(uint(ntg), uint(nb), 1u), uint3(uint(hs), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bqk, want_qk, "{tag} q"), 0)
        t |> equal(buf_mismatch_exact(barena, want_a, "{tag} arena"), 0)
    }
    metal_release(bqk)
    metal_release(barena)
    metal_release(btc)
    metal_release(bts)
    metal_release(bsg)
    metal_release(brt)
    metal_release(bbias)
    metal_release(pso)
    delete qk
    delete bias
    delete tcos
    delete tsin
    delete signs
    delete rt
    delete want_qk
    delete want_a
}

// ===== the tq4 un-rotate (MetalFwhtUnsign) vs fwht_unsign_row =====

def private unsign_gate(t : T?; dev, queue; hs, nheads, nrows, pad : int) {
    let tag = "fwht_unsign hs={hs} nh={nheads} rows={nrows} pad={pad}"
    var err : string
    var pso = pipeline_from_source(dev, metal_fwht_unsign_msl, metal_fwht_unsign_msl_entry, metal_fwht_unsign_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let qstride = nheads * hs + pad
    var yo : array<float>
    var signs : array<float>
    yo |> resize(nrows * qstride)
    tq4_fill_signs(signs, int64(hs))
    for (k in range(nrows * qstride)) {
        yo[k] = 0.23 * float(k % 67) - 3.5
    }
    var want := yo
    unsafe {
        for (r in range(nrows)) {
            for (h in range(nheads)) {
                fwht_unsign_row(addr(want[r * qstride + h * hs]), addr<float const?>(signs[0]), int64(hs))
            }
        }
    }
    var byo = buf_upload(dev, yo)
    var bsg = buf_upload(dev, signs)
    var bhs = buf_u32(dev, uint(hs))
    var bqs = buf_u32(dev, uint(qstride))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_fwht_unsign_msl_tgmem, 0)
        metal_set_buffer(enc, byo, 0ul, 0)
        metal_set_buffer(enc, bsg, 0ul, 1)
        metal_set_buffer(enc, bhs, 0ul, 2)
        metal_set_buffer(enc, bqs, 0ul, 3)
        metal_dispatch_threadgroups(enc, uint3(uint(nheads), uint(nrows), 1u), uint3(uint(hs), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(byo, want, tag), 0)
    }
    metal_release(byo)
    metal_release(bsg)
    metal_release(bhs)
    metal_release(bqs)
    metal_release(pso)
    delete yo
    delete signs
    delete want
}

[test]
def test_metal_rope_store_kernels(t : T?) {
    t |> run("fused rope + KV-store kernels == the CPU rope/codec seam (all four mirrors, flat/head/batched)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU kernel checks skipped\n")
                    return
                }
                var queue = metal_new_command_queue(dev)
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
                // batched twins: row tables + whole-arena sentinel compares
                rpst_b_gate(t, dev, queue, true, 64, 4, 2, 64, 3, true, true)
                rpst_b_gate(t, dev, queue, true, 64, 4, 2, 64, 2, false, false)
                rpst_b_gate(t, dev, queue, false, 64, 4, 2, 32, 2, true, false)
                rpst_bq8_gate(t, dev, queue, 64, 4, 2, 3, true, false)
                rpst_bq8_gate(t, dev, queue, 64, 4, 2, 2, false, true)
                rpst_btq4_gate(t, dev, queue, 64, 4, 2, 2, true, true)
                rpst_btq4_gate(t, dev, queue, 64, 4, 2, 2, false, false)
                // the tq4 un-rotate partner
                unsign_gate(t, dev, queue, 64, 4, 3, 32)
                unsign_gate(t, dev, queue, 128, 2, 1, 0)
                metal_release(queue)
            }
            t |> equal(metal_log_leaks_if_any(), 0l)
        } else {
            feint("das_metal is not built on this platform; rope-store kernel checks skipped\n")
        }
    }
}
