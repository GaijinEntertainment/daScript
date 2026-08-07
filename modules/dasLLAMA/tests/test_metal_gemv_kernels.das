options gen2
options _function_length = 0        // per-kernel gates: hand-bound buffers/uniforms + dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes

require dastest/testing_boost public
require ?das_metal dasllama/dasllama_metal_kernels  // the kq GEMV / KqMv / MoeGemv kernel set (Apple builds)
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // shared buf helpers, kq plane builders + eyeball-dump compares
require math

// The K-quant decode GEMV corpus (single-stream KqGemv K4/K5/K5C/K6, the small-batch KqMv
// B2/B4/B8 twins, the expert-indexed MoeGemv kq trio) vs a CPU reference on synthetic planes:
// random bit patterns are valid quant data, scales come from finite f16 values, and the truth
// side dequants each row through the SHIPPED dequant_k*_plane_superblock (the same math the
// loaders and repack verifiers trust) and dots in double. Float GPU accumulation reassociates,
// so the compare bar rides each output's DOT ENVELOPE (sum |w·x| — the test_metal_gemm
// precedent): |gpu-cpu| <= max(2e-4 * env, 1e-3). y buffers are sentinel-filled and compared
// WHOLE, so a write outside the guarded rows/columns is a red, not silence. Off-Apple (or with
// no Metal device) every check feints cleanly.

// ===== single-stream kq GEMV (MetalKqGemvK4 / K5 / K5C / K6) =====

def private kq_gemv_gate(t : T?; dev, queue; vform : string; n, d : int) {
    let fmt = vform == "k4" ? 4 : (vform == "k6" ? 6 : 5)
    let tag = "kq_gemv_{vform} n={n} d={d}"
    var err : string
    let src = vform == "k4" ? metal_kq_gemv_k4_msl : (vform == "k5" ? metal_kq_gemv_k5_msl : (vform == "k5c" ? metal_kq_gemv_k5c_msl : metal_kq_gemv_k6_msl))
    let entry = vform == "k4" ? metal_kq_gemv_k4_msl_entry : (vform == "k5" ? metal_kq_gemv_k5_msl_entry : (vform == "k5c" ? metal_kq_gemv_k5c_msl_entry : metal_kq_gemv_k6_msl_entry))
    let fm = vform == "k4" ? metal_kq_gemv_k4_msl_fastmath : (vform == "k5" ? metal_kq_gemv_k5_msl_fastmath : (vform == "k5c" ? metal_kq_gemv_k5c_msl_fastmath : metal_kq_gemv_k6_msl_fastmath))
    var pso = pipeline_from_source(dev, src, entry, fm, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nb = n / 256
    let nsb = d * nb
    var kq : array<uint8>
    var ks : array<uint8>
    kq_fill_planes(fmt, nsb, kq, ks)
    var xv : array<float>
    xv |> resize(n)
    for (i in range(n)) {
        xv[i] = 0.11 * float(i % 41) - 2.2
    }
    var want : array<float>
    var env : array<float>
    var wrow : array<float>
    want |> resize(d)
    env |> resize(d)
    wrow |> resize(n)
    for (r in range(d)) {
        kq_row_ref(fmt, r * nb, nb, nsb, kq, ks, wrow)
        var acc = 0.0lf
        var ev = 0.0lf
        for (c in range(n)) {
            let p = double(wrow[c]) * double(xv[c])
            acc += p
            ev += abs(p)
        }
        want[r] = float(acc)
        env[r] = float(ev)
    }
    var bks = buf_upload(dev, ks)
    var bkq = buf_upload(dev, kq)
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, d, -1000.0)
    var bn = buf_u32(dev, uint(n))
    var bd = buf_u32(dev, uint(d))
    let groups = vform == "k5c" ? (d + 1) / 2 : (d + 3) / 4
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bks, fmt == 6 ? uint64(nsb * 16) : 0ul, 0)   // k6: the f16 d tail
        metal_set_buffer(enc, bks, 0ul, 1)
        metal_set_buffer(enc, bkq, 0ul, 2)
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_buffer(enc, bn, 0ul, 5)
        metal_set_buffer(enc, bd, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(uint(groups), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-4, tag), 0)
    }
    metal_release(bks)
    metal_release(bkq)
    metal_release(bx)
    metal_release(by)
    metal_release(bn)
    metal_release(bd)
    metal_release(pso)
    delete kq
    delete ks
    delete xv
    delete want
    delete env
    delete wrow
}

// ===== the small-batch KqMv twins (B2/B4/B8 x K4/K5/K6): y[col * ys + row] =====

def private kq_mvb_gate(t : T?; dev, queue; fmt, bwidth, n, d, nr, ys : int) {
    let tag = "kq_mvb{bwidth}_k{fmt} n={n} d={d} nr={nr} ys={ys}"
    var err : string
    let src = (bwidth == 2 ? (fmt == 4 ? metal_kq_mvb2_k4_msl : (fmt == 5 ? metal_kq_mvb2_k5_msl : metal_kq_mvb2_k6_msl))
        : (bwidth == 4 ? (fmt == 4 ? metal_kq_mvb4_k4_msl : (fmt == 5 ? metal_kq_mvb4_k5_msl : metal_kq_mvb4_k6_msl))
            : (fmt == 4 ? metal_kq_mvb8_k4_msl : (fmt == 5 ? metal_kq_mvb8_k5_msl : metal_kq_mvb8_k6_msl))))
    let entry = (bwidth == 2 ? (fmt == 4 ? metal_kq_mvb2_k4_msl_entry : (fmt == 5 ? metal_kq_mvb2_k5_msl_entry : metal_kq_mvb2_k6_msl_entry))
        : (bwidth == 4 ? (fmt == 4 ? metal_kq_mvb4_k4_msl_entry : (fmt == 5 ? metal_kq_mvb4_k5_msl_entry : metal_kq_mvb4_k6_msl_entry))
            : (fmt == 4 ? metal_kq_mvb8_k4_msl_entry : (fmt == 5 ? metal_kq_mvb8_k5_msl_entry : metal_kq_mvb8_k6_msl_entry))))
    let fm = (bwidth == 2 ? (fmt == 4 ? metal_kq_mvb2_k4_msl_fastmath : (fmt == 5 ? metal_kq_mvb2_k5_msl_fastmath : metal_kq_mvb2_k6_msl_fastmath))
        : (bwidth == 4 ? (fmt == 4 ? metal_kq_mvb4_k4_msl_fastmath : (fmt == 5 ? metal_kq_mvb4_k5_msl_fastmath : metal_kq_mvb4_k6_msl_fastmath))
            : (fmt == 4 ? metal_kq_mvb8_k4_msl_fastmath : (fmt == 5 ? metal_kq_mvb8_k5_msl_fastmath : metal_kq_mvb8_k6_msl_fastmath))))
    var pso = pipeline_from_source(dev, src, entry, fm, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nb = n / 256
    let nsb = d * nb
    var kq : array<uint8>
    var ks : array<uint8>
    kq_fill_planes(fmt, nsb, kq, ks)
    // x is padded to the kernel's column-group width (the driver's mp guarantee); pad rows zero
    let gcols = bwidth == 4 ? (nr + 3) / 4 : 1
    let xpad = bwidth == 2 ? 2 : (bwidth == 4 ? gcols * 4 : 8)
    var xv : array<float>
    xv |> resize(xpad * n)
    for (i in range(xpad * n)) {
        xv[i] = i < nr * n ? 0.09 * float(i % 43) - 1.8 : 0.0
    }
    var want : array<float>
    var env : array<float>
    var wrow : array<float>
    want |> resize(nr * ys)
    env |> resize(nr * ys)
    wrow |> resize(n)
    for (i in range(nr * ys)) {
        want[i] = -1000.0
        env[i] = 0.0
    }
    for (r in range(d)) {
        kq_row_ref(fmt, r * nb, nb, nsb, kq, ks, wrow)
        for (col in range(nr)) {
            var acc = 0.0lf
            var ev = 0.0lf
            for (c in range(n)) {
                let p = double(wrow[c]) * double(xv[col * n + c])
                acc += p
                ev += abs(p)
            }
            want[col * ys + r] = float(acc)
            env[col * ys + r] = float(ev)
        }
    }
    var bks = buf_upload(dev, ks)
    var bkq = buf_upload(dev, kq)
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, nr * ys, -1000.0)
    var ka = KqMvArgs(ndim = uint(n), ddim = uint(d), ys = uint(ys), nr = uint(nr))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        if (bwidth == 8) {
            metal_set_threadgroup_memory_length(enc,
                fmt == 4 ? metal_kq_mvb8_k4_msl_tgmem : (fmt == 5 ? metal_kq_mvb8_k5_msl_tgmem : metal_kq_mvb8_k6_msl_tgmem), 0)
        }
        metal_set_buffer(enc, bks, fmt == 6 ? uint64(nsb * 16) : 0ul, 0)   // k6: the f16 d tail
        metal_set_buffer(enc, bks, 0ul, 1)
        metal_set_buffer(enc, bkq, 0ul, 2)
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        metal_dispatch_threadgroups(enc, uint3(uint((d + 7) / 8), uint(gcols), 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-4, tag), 0)
    }
    metal_release(bks)
    metal_release(bkq)
    metal_release(bx)
    metal_release(by)
    metal_release(pso)
    delete kq
    delete ks
    delete xv
    delete want
    delete env
    delete wrow
}

// ===== the expert-indexed MoeGemv kq trio (MetalMoeGemvK4 / K5 / K6) =====

def private moe_kq_gemv_gate(t : T?; dev, queue; fmt, n, d, net, k, nst : int; perslot : bool) {
    let tag = "moe_gemv_k{fmt} n={n} d={d} net={net} k={k} nst={nst} perslot={perslot}"
    var err : string
    let src = fmt == 4 ? metal_moe_gemv_k4_msl : (fmt == 5 ? metal_moe_gemv_k5_msl : metal_moe_gemv_k6_msl)
    let entry = fmt == 4 ? metal_moe_gemv_k4_msl_entry : (fmt == 5 ? metal_moe_gemv_k5_msl_entry : metal_moe_gemv_k6_msl_entry)
    let fm = fmt == 4 ? metal_moe_gemv_k4_msl_fastmath : (fmt == 5 ? metal_moe_gemv_k5_msl_fastmath : metal_moe_gemv_k6_msl_fastmath)
    var pso = pipeline_from_source(dev, src, entry, fm, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nb = n / 256
    let nsb = net * d * nb              // the whole expert stack's superblocks
    var kq : array<uint8>
    var ks : array<uint8>
    kq_fill_planes(fmt, nsb, kq, ks)
    // selection rows: [k idx u32][k w f32] per stream — only the idx section is read here
    var sel : array<uint>
    sel |> resize(nst * 2 * k)
    for (s in range(nst)) {
        for (j in range(k)) {
            sel[s * 2 * k + j] = uint((s + j * 3 + net - 1) % net)
        }
    }
    let xrows = perslot ? nst * k : nst
    var xv : array<float>
    xv |> resize(xrows * n)
    for (i in range(xrows * n)) {
        xv[i] = 0.07 * float(i % 37) - 1.2
    }
    var want : array<float>
    var env : array<float>
    var wrow : array<float>
    want |> resize(nst * k * d)
    env |> resize(nst * k * d)
    wrow |> resize(n)
    for (s in range(nst)) {
        for (j in range(k)) {
            let e = int(sel[s * 2 * k + j])
            let x0 = (perslot ? s * k + j : s) * n
            for (r in range(d)) {
                kq_row_ref(fmt, (e * d + r) * nb, nb, nsb, kq, ks, wrow)
                var acc = 0.0lf
                var ev = 0.0lf
                for (c in range(n)) {
                    let p = double(wrow[c]) * double(xv[x0 + c])
                    acc += p
                    ev += abs(p)
                }
                want[(s * k + j) * d + r] = float(acc)
                env[(s * k + j) * d + r] = float(ev)
            }
        }
    }
    var bks = buf_upload(dev, ks)
    var bkq = buf_upload(dev, kq)
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, nst * k * d, -1000.0)
    var bsel = buf_upload(dev, sel)
    var ka = MoeGemvArgs(ndim = uint(n), ddim = uint(d), eplane = uint(d * nb),
        xs = perslot ? uint(n) : 0u, ss = uint(2 * k),
        bxs = uint((perslot ? k : 1) * n), bys = uint(k * d), hasb = 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bks, fmt == 6 ? uint64(nsb * 16) : 0ul, 0)   // k6: the f16 d tail
        metal_set_buffer(enc, bks, 0ul, 1)
        metal_set_buffer(enc, bkq, 0ul, 2)
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        metal_set_buffer(enc, bsel, 0ul, 7)
        metal_dispatch_threadgroups(enc, uint3(uint((d + 3) / 4), uint(k), uint(nst)), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-4, tag), 0)
    }
    metal_release(bks)
    metal_release(bkq)
    metal_release(bx)
    metal_release(by)
    metal_release(bsel)
    metal_release(pso)
    delete kq
    delete ks
    delete sel
    delete xv
    delete want
    delete env
    delete wrow
}

// ===== the fused QKV rope-store pair (MetalQ8GemvQkvRsF16/F32) =====

// One dispatch = q8 GEMV over all three QKV segments + rope + segment stores: q rows to qo
// (f32), k rows roped into the mirror (f16 clamped / f32), v rows straight. Segments bind ONE
// blob at per-segment byte offsets — the production @off scheme. The exact-arithmetic fixture
// makes qo (and the f32 twin's k/v) BIT-exact; the f16 twin's k/v differ only by store rounding.
def private qkv_rs_gate(t : T?; dev, queue; f16 : bool; neox, has_bias : bool; ndim, qd, kvd, hs : int) {
    let tag = "qkv_rs{f16 ? 16 : 32}{neox ? " neox" : ""}{has_bias ? " bias" : ""} n={ndim} qd={qd} kvd={kvd}"
    var err : string
    var pso = (f16
        ? pipeline_from_source(dev, metal_q8_gemv_qkv_rs16_msl, metal_q8_gemv_qkv_rs16_msl_entry, metal_q8_gemv_qkv_rs16_msl_fastmath, err)
        : pipeline_from_source(dev, metal_q8_gemv_qkv_rs32_msl, metal_q8_gemv_qkv_rs32_msl_entry, metal_q8_gemv_qkv_rs32_msl_fastmath, err))
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let ddim = qd + 2 * kvd
    let half = hs / 2
    let nb = ndim / 32
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(ddim, nb, blob, wdeq)   // one stacked blob: q rows, then k, then v (segment offsets)
    var xf : array<float>
    q8_x_fill(ndim, xf)
    var tcos : array<float>
    var tsin : array<float>
    tcos |> resize(half)
    tsin |> resize(half)
    for (j in range(half)) {
        tcos[j] = float((j * 3) % 5 - 2) * 0.5
        tsin[j] = float((j * 7 + 1) % 5 - 2) * 0.25
    }
    var bias : array<float>
    bias |> resize(ddim)
    for (i in range(ddim)) {
        bias[i] = float((i * 13) % 9 - 4) * 0.25
    }
    var qwant : array<float>
    var kwant : array<float>
    var vwant : array<float>
    qwant |> resize(qd)
    kwant |> resize(kvd)
    vwant |> resize(kvd)
    for (pr in range(ddim / 2)) {
        var row = pr * 2
        var row2 = row + 1
        if (neox && row < qd + kvd) {
            let lp = row < qd ? pr : pr - qd / 2
            let seg = row < qd ? 0 : qd
            row = seg + (lp / half) * hs + lp % half
            row2 = row + half
        }
        var a0 = 0.0lf
        var a1 = 0.0lf
        for (c in range(ndim)) {
            a0 += double(wdeq[row * ndim + c]) * double(xf[c])
            a1 += double(wdeq[row2 * ndim + c]) * double(xf[c])
        }
        if (has_bias) {
            a0 += double(bias[row])
            a1 += double(bias[row2])
        }
        if (row < qd) {
            let j = neox ? row % hs : (row % hs) / 2
            qwant[row] = float(a0 * double(tcos[j]) - a1 * double(tsin[j]))
            qwant[row2] = float(a0 * double(tsin[j]) + a1 * double(tcos[j]))
        } elif (row < qd + kvd) {
            let ik = row - qd
            let j = neox ? ik % hs : (ik % hs) / 2
            kwant[ik] = float(a0 * double(tcos[j]) - a1 * double(tsin[j]))
            kwant[row2 - qd] = float(a0 * double(tsin[j]) + a1 * double(tcos[j]))
        } else {
            let iv = row - qd - kvd
            vwant[iv] = float(a0)
            vwant[iv + 1] = float(a1)
        }
    }
    var ka = Q8QkvRsArgs(ndim = uint(ndim), ddim = uint(ddim), qd = uint(qd), kvd = uint(kvd),
        hs = uint(hs), neox = neox ? 1u : 0u, has_bias = has_bias ? 1u : 0u)
    let koffb = uint64(qd * nb * 34)         // k segment's byte offset into the stacked blob
    let voffb = uint64((qd + kvd) * nb * 34)
    var bw = buf_upload(dev, blob)
    var bx = buf_upload(dev, xf)
    var bqo = buf_fill(dev, qd, -1000.0)
    var bkd = f16 ? buf_fill(dev, kvd, float16(-1000.0)) : buf_fill(dev, kvd, -1000.0)
    var bvd = f16 ? buf_fill(dev, kvd, float16(-1000.0)) : buf_fill(dev, kvd, -1000.0)
    var bcos = buf_upload(dev, tcos)
    var bsin = buf_upload(dev, tsin)
    var bbias = buf_upload(dev, bias)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)      // wq byte view
        metal_set_buffer(enc, bw, 0ul, 1)      // wq half-scale view
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, bqo, 0ul, 3)
        metal_set_buffer(enc, bkd, 0ul, 4)
        metal_set_buffer(enc, bvd, 0ul, 5)
        metal_set_buffer(enc, bcos, 0ul, 6)
        metal_set_buffer(enc, bsin, 0ul, 7)
        metal_set_buffer(enc, bbias, 0ul, 8)
        metal_set_buffer(enc, bw, koffb, 9)    // wk views at the k segment's offset
        metal_set_buffer(enc, bw, koffb, 10)
        metal_set_buffer(enc, bw, voffb, 11)   // wv views
        metal_set_buffer(enc, bw, voffb, 12)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 13)
        metal_dispatch_threadgroups(enc, uint3(uint((ddim / 2 + 3) / 4), 1u, 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bqo, qwant, "{tag} q"), 0)
        if (f16) {
            t |> equal(buf_mismatch_f16(bkd, kwant, 1e-3, 0.0, "{tag} k"), 0)
            t |> equal(buf_mismatch_f16(bvd, vwant, 1e-3, 0.0, "{tag} v"), 0)
        } else {
            t |> equal(buf_mismatch_exact(bkd, kwant, "{tag} k"), 0)
            t |> equal(buf_mismatch_exact(bvd, vwant, "{tag} v"), 0)
        }
    }
    metal_release(bw)
    metal_release(bx)
    metal_release(bqo)
    metal_release(bkd)
    metal_release(bvd)
    metal_release(bcos)
    metal_release(bsin)
    metal_release(bbias)
    metal_release(pso)
    delete blob
    delete wdeq
    delete xf
    delete tcos
    delete tsin
    delete bias
    delete qwant
    delete kwant
    delete vwant
}

// ===== the fused W1|W3 + activation family =====

def private silu_gelu_ref(g, u : double; gelu : bool) : float {
    if (!gelu) {
        return float((g / (1.0lf + exp(-g))) * u)
    }
    let ga = 0.7978845608028654lf * (g + 0.044715lf * g * g * g)
    return float(0.5lf * g * (2.0lf - 2.0lf / (1.0lf + exp(2.0lf * ga))) * u)
}

// MetalQ8GemvW13Sw: one simdgroup dots gate row (w1) AND up row (w3), stores act(g)·u.
def private w13sw_gate(t : T?; dev, queue; gelu : bool; ndim, hidden : int) {
    let tag = "w13sw{gelu ? " gelu" : ""} n={ndim} h={hidden}"
    var err : string
    var pso = pipeline_from_source(dev, metal_q8_gemv_w13sw16_msl, metal_q8_gemv_w13sw16_msl_entry, metal_q8_gemv_w13sw16_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nb = ndim / 32
    var blob1 : array<uint8>
    var wdeq1 : array<float>
    var blob3 : array<uint8>
    var wdeq3 : array<float>
    q8_blob_fill(2 * hidden, nb, blob1, wdeq1)   // one generator, two slices: w1 rows then w3 rows
    blob3 |> resize(hidden * nb * 34)
    wdeq3 |> resize(hidden * ndim)
    for (i in range(hidden * nb * 34)) {
        blob3[i] = blob1[hidden * nb * 34 + i]
    }
    for (i in range(hidden * ndim)) {
        wdeq3[i] = wdeq1[hidden * ndim + i]
    }
    var xf : array<float>
    q8_x_fill(ndim, xf)
    var want : array<float>
    want |> resize(hidden)
    for (r in range(hidden)) {
        var g = 0.0lf
        var u = 0.0lf
        for (c in range(ndim)) {
            g += double(wdeq1[r * ndim + c]) * double(xf[c])
            u += double(wdeq3[r * ndim + c]) * double(xf[c])
        }
        want[r] = silu_gelu_ref(g, u, gelu)
    }
    var bw1 = buf_upload(dev, blob1)
    var bw3 = buf_upload(dev, blob3)
    var bx = buf_upload(dev, xf)
    var bh = buf_fill(dev, hidden, -1000.0)
    var bn = buf_u32(dev, uint(ndim))
    var bhid = buf_u32(dev, uint(hidden))
    var bact = buf_u32(dev, gelu ? 1u : 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw1, 0ul, 0)
        metal_set_buffer(enc, bw1, 0ul, 1)
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, bh, 0ul, 3)
        metal_set_buffer(enc, bn, 0ul, 4)
        metal_set_buffer(enc, bhid, 0ul, 5)
        metal_set_buffer(enc, bw3, 0ul, 6)
        metal_set_buffer(enc, bw3, 0ul, 7)
        metal_set_buffer(enc, bact, 0ul, 8)
        metal_dispatch_threadgroups(enc, uint3(uint((hidden + 3) / 4), 1u, 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bh, want, tag), 0)
    }
    metal_release(bw1)
    metal_release(bw3)
    metal_release(bx)
    metal_release(bh)
    metal_release(bn)
    metal_release(bhid)
    metal_release(bact)
    metal_release(pso)
    delete blob1
    delete wdeq1
    delete blob3
    delete wdeq3
    delete xf
    delete want
}

// MetalGemvW13SwB2/B4: the fixed-B batch twins (silu only; binding 1 is SKIPPED by the class —
// scale views sit at 2/9). ndim = 320 walks one full 256-elem X panel plus the strided tail.
def private w13sw_b_gate(t : T?; dev, queue; bwidth, ndim, hidden : int) {
    let tag = "w13sw_b{bwidth} n={ndim} h={hidden}"
    var err : string
    var pso = (bwidth == 2
        ? pipeline_from_source(dev, metal_gemv_w13sw_b2_msl, metal_gemv_w13sw_b2_msl_entry, metal_gemv_w13sw_b2_msl_fastmath, err)
        : pipeline_from_source(dev, metal_gemv_w13sw_b4_msl, metal_gemv_w13sw_b4_msl_entry, metal_gemv_w13sw_b4_msl_fastmath, err))
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nb = ndim / 32
    var blob1 : array<uint8>
    var wdeq1 : array<float>
    var blob3 : array<uint8>
    var wdeq3 : array<float>
    q8_blob_fill(2 * hidden, nb, blob1, wdeq1)
    blob3 |> resize(hidden * nb * 34)
    wdeq3 |> resize(hidden * ndim)
    for (i in range(hidden * nb * 34)) {
        blob3[i] = blob1[hidden * nb * 34 + i]
    }
    for (i in range(hidden * ndim)) {
        wdeq3[i] = wdeq1[hidden * ndim + i]
    }
    var xf : array<float>
    q8_x_fill(bwidth * ndim, xf)
    var want : array<float>
    want |> resize(bwidth * hidden)
    for (b in range(bwidth)) {
        for (r in range(hidden)) {
            var g = 0.0lf
            var u = 0.0lf
            for (c in range(ndim)) {
                g += double(wdeq1[r * ndim + c]) * double(xf[b * ndim + c])
                u += double(wdeq3[r * ndim + c]) * double(xf[b * ndim + c])
            }
            want[b * hidden + r] = silu_gelu_ref(g, u, false)
        }
    }
    var bw1 = buf_upload(dev, blob1)
    var bw3 = buf_upload(dev, blob3)
    var bx = buf_upload(dev, xf)
    var by = buf_fill(dev, bwidth * hidden, -1000.0)
    var bn = buf_u32(dev, uint(ndim))
    var bhid = buf_u32(dev, uint(hidden))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, bwidth == 2 ? metal_gemv_w13sw_b2_msl_tgmem : metal_gemv_w13sw_b4_msl_tgmem, 0)
        metal_set_buffer(enc, bw1, 0ul, 0)
        metal_set_buffer(enc, bw1, 0ul, 2)
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_buffer(enc, bn, 0ul, 5)
        metal_set_buffer(enc, bhid, 0ul, 6)
        metal_set_buffer(enc, bw3, 0ul, 8)
        metal_set_buffer(enc, bw3, 0ul, 9)
        metal_dispatch_threadgroups(enc, uint3(uint((hidden + 3) / 4), 1u, 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(by, want, tag), 0)
    }
    metal_release(bw1)
    metal_release(bw3)
    metal_release(bx)
    metal_release(by)
    metal_release(bn)
    metal_release(bhid)
    metal_release(pso)
    delete blob1
    delete wdeq1
    delete blob3
    delete wdeq3
    delete xf
    delete want
}

// ===== the fixed-B batched q8 GEMV twins =====

// MetalGemvB2/B4 (x-staged panel form): dense [B x ndim] X, y at ROW stride ys — the exact
// fixture makes every output BIT-exact, and ys > ddim gap columns stay sentinel.
def private gemv_b_gate(t : T?; dev, queue; bwidth, ndim, ddim, ys : int) {
    let tag = "gemv_b{bwidth} n={ndim} d={ddim} ys={ys}"
    var err : string
    var pso = (bwidth == 2
        ? pipeline_from_source(dev, metal_gemv_b2_msl, metal_gemv_b2_msl_entry, metal_gemv_b2_msl_fastmath, err)
        : pipeline_from_source(dev, metal_gemv_b4_msl, metal_gemv_b4_msl_entry, metal_gemv_b4_msl_fastmath, err))
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(ddim, ndim / 32, blob, wdeq)
    var xf : array<float>
    q8_x_fill(bwidth * ndim, xf)
    var want : array<float>
    want |> resize(bwidth * ys)
    for (i in range(bwidth * ys)) {
        want[i] = -1000.0
    }
    for (b in range(bwidth)) {
        for (r in range(ddim)) {
            var acc = 0.0lf
            for (c in range(ndim)) {
                acc += double(wdeq[r * ndim + c]) * double(xf[b * ndim + c])
            }
            want[b * ys + r] = float(acc)
        }
    }
    var bw = buf_upload(dev, blob)
    var bx = buf_upload(dev, xf)
    var by = buf_fill(dev, bwidth * ys, -1000.0)
    var bn = buf_u32(dev, uint(ndim))
    var bd = buf_u32(dev, uint(ddim))
    var bys = buf_u32(dev, uint(ys))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, bwidth == 2 ? metal_gemv_b2_msl_tgmem : metal_gemv_b4_msl_tgmem, 0)
        metal_set_buffer(enc, bw, 0ul, 0)   // byte view
        metal_set_buffer(enc, bw, 0ul, 2)   // half-scale view
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_buffer(enc, bn, 0ul, 5)
        metal_set_buffer(enc, bd, 0ul, 6)
        metal_set_buffer(enc, bys, 0ul, 7)
        metal_dispatch_threadgroups(enc, uint3(uint((ddim + 3) / 4), 1u, 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(by, want, tag), 0)
    }
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(bn)
    metal_release(bd)
    metal_release(bys)
    metal_release(pso)
    delete blob
    delete wdeq
    delete xf
    delete want
}

// MetalQ8MvB2/B4 (streamed skinny form, 64-thread tg, n % 256): X rows at float4 stride xs4,
// b4 stores gate on nrows (the b<nrows pad columns compute and discard, x reads clamp to the
// last live row). b2 always writes both columns — B=1 rides an unread y row by design.
def private q8_mv_b_gate(t : T?; dev, queue; bwidth, ndim, ddim, nrows, xs4, ys : int) {
    let tag = "q8_mv_b{bwidth} n={ndim} d={ddim} nr={nrows} xs4={xs4} ys={ys}"
    var err : string
    var pso = (bwidth == 2
        ? pipeline_from_source(dev, metal_q8_mv_b2_msl, metal_q8_mv_b2_msl_entry, metal_q8_mv_b2_msl_fastmath, err)
        : pipeline_from_source(dev, metal_q8_mv_b4_msl, metal_q8_mv_b4_msl_entry, metal_q8_mv_b4_msl_fastmath, err))
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(ddim, ndim / 32, blob, wdeq)
    var xf : array<float>
    q8_x_fill(nrows * xs4 * 4, xf)
    let nwr = bwidth == 2 ? 2 : nrows   // b2 stores both columns unconditionally
    var want : array<float>
    want |> resize(nwr * ys)
    for (i in range(nwr * ys)) {
        want[i] = -1000.0
    }
    for (b in range(nwr)) {
        let src = min(b, nrows - 1)     // the b4 pad-column x clamp
        for (r in range(ddim)) {
            var acc = 0.0lf
            for (c in range(ndim)) {
                acc += double(wdeq[r * ndim + c]) * double(xf[src * xs4 * 4 + c])
            }
            want[b * ys + r] = float(acc)
        }
    }
    var ka = Q8MvArgs(ndim = uint(ndim), ddim = uint(ddim), nrows = uint(nrows),
        xs4 = uint(xs4), ys = uint(ys))
    var bw = buf_upload(dev, blob)
    var bx = buf_upload(dev, xf)
    var by = buf_fill(dev, nwr * ys, -1000.0)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)   // half-scale view
        metal_set_buffer(enc, bw, 0ul, 1)   // byte view
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
        metal_dispatch_threadgroups(enc, uint3(uint(bwidth == 2 ? (ddim + 3) / 4 : (ddim + 7) / 8), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(by, want, tag), 0)
    }
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(pso)
    delete blob
    delete wdeq
    delete xf
    delete want
}

[test]
def test_metal_kq_gemv_kernels(t : T?) {
    t |> run("kq GEMV / KqMv / MoeGemv-kq kernels == the plane-dequant CPU reference") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU kernel checks skipped\n")
                    return
                }
                var queue = metal_new_command_queue(dev)
                // single-stream: all four variants; d = 11 exercises the row-guard tails
                kq_gemv_gate(t, dev, queue, "k4", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k5", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k5c", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k6", 1280, 11)
                kq_gemv_gate(t, dev, queue, "k4", 512, 30)
                kq_gemv_gate(t, dev, queue, "k6", 512, 30)
                // the small-batch twins: nr = 2 / 3 (b4 col guard) / 6 (b8 pad), ys > d
                for (fmt in [4, 5, 6]) {
                    kq_mvb_gate(t, dev, queue, fmt, 2, 768, 17, 2, 22)
                    kq_mvb_gate(t, dev, queue, fmt, 4, 768, 17, 3, 22)
                    kq_mvb_gate(t, dev, queue, fmt, 8, 768, 17, 6, 22)
                }
                // expert-indexed: shared-x (gate/up) and per-slot (down) forms
                for (fmt in [4, 5, 6]) {
                    moe_kq_gemv_gate(t, dev, queue, fmt, 512, 9, 5, 3, 2, false)
                    moe_kq_gemv_gate(t, dev, queue, fmt, 512, 9, 5, 3, 2, true)
                }
                // the fused QKV rope-store pair: norm/neox x bias x f16/f32 mirror
                qkv_rs_gate(t, dev, queue, true, false, true, 64, 16, 8, 8)
                qkv_rs_gate(t, dev, queue, true, true, false, 64, 16, 8, 8)
                qkv_rs_gate(t, dev, queue, false, true, true, 64, 16, 8, 8)
                qkv_rs_gate(t, dev, queue, false, false, false, 64, 16, 8, 8)
                // the fused W1|W3 + activation family (h=18: the row guard / min-clamp tails)
                w13sw_gate(t, dev, queue, false, 320, 18)
                w13sw_gate(t, dev, queue, true, 64, 8)
                w13sw_b_gate(t, dev, queue, 2, 320, 18)
                w13sw_b_gate(t, dev, queue, 4, 320, 18)
                // the fixed-B q8 GEMV twins: panel+tail ndim, min-clamp d, ys gaps, padded xs4,
                // and the b4 nrows=3 store gate / pad-column clamp
                gemv_b_gate(t, dev, queue, 2, 320, 18, 18)
                gemv_b_gate(t, dev, queue, 2, 256, 8, 24)
                gemv_b_gate(t, dev, queue, 4, 320, 18, 18)
                gemv_b_gate(t, dev, queue, 4, 256, 8, 24)
                q8_mv_b_gate(t, dev, queue, 2, 512, 11, 2, 128, 11)
                q8_mv_b_gate(t, dev, queue, 2, 256, 8, 2, 68, 20)
                q8_mv_b_gate(t, dev, queue, 4, 512, 11, 3, 128, 11)
                q8_mv_b_gate(t, dev, queue, 4, 256, 8, 4, 64, 8)
                metal_release(queue)
            }
            t |> equal(metal_log_leaks_if_any(), 0l)
        } else {
            feint("das_metal is not built on this platform; kq GEMV kernel checks skipped\n")
        }
    }
}
