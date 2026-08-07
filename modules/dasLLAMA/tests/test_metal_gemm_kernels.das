options gen2
options _function_length = 0        // per-kernel gates: hand-bound buffers/uniforms + dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes

require dastest/testing_boost public
require daslib/math_bits            // float_bits_to_uint / uint_bits_to_float — the bf16 halfword pun
require llvm/daslib/f16_cvt         // f32_to_f16 — packing the q8 blob's scale halfwords
require ?das_metal dasllama/dasllama_metal_kernels  // the kq mul_mm trio (Apple builds)
require ?das_metal dasllama/dasllama_metal_prefill  // the mul_mm twins (bf16 + Metal-4 tensor forms)
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // shared buf helpers, kq plane builders + eyeball-dump compares
require math

// The production GEMM corpus vs CPU references on synthetic planes: the prefill K-quant
// mul_mm trio (K4/K5/K6) and every Metal-4 tensor twin (kq T trio, Q8MulMmT, Bf16MulMm/T),
// plus the batched-decode q8 family — MetalQ8GemmB[T] (M-pad-32, y at token-row stride ys),
// the split-K pair MetalQ8GemmBSk[T] + MetalSkReduce run as the production two-dispatch
// chain, and the 64-wide N tile MetalQ8Gemm64B[T]. K-quant truth dequants rows through the
// shipped dequant_k*_plane_superblock and dots in double under the envelope bar (float GPU
// accumulation reassociates); bf16 truth dequants the halfword pun exactly; the q8 fixtures
// use pow2 scales + small-int values so every staged f16 value is exact. Prefill shapes ride
// the mul_mm grid contract: m % 32, ndim % 64, kdim % 256 (kq) or % 32, y token-major
// [m x ndim]. A tensor twin whose PSO does not compile on this box/toolchain logs a visible
// skip (no Metal-4 tensor support — the M4 box runs those arms); the simdgroup bases must
// always compile. Off-Apple (or with no Metal device) every check feints.

def private kq_mulmm_gate(t : T?; dev, queue; fmt : int; tensor : bool; m, kdim, ndim : int) {
    let tag = "kq_mulmm_k{fmt}{tensor ? "t" : ""} m={m} k={kdim} n={ndim}"
    var err : string
    let src = (tensor
        ? (fmt == 4 ? metal_kq_mulmm_k4_t_msl : (fmt == 5 ? metal_kq_mulmm_k5_t_msl : metal_kq_mulmm_k6_t_msl))
        : (fmt == 4 ? metal_kq_mulmm_k4_msl : (fmt == 5 ? metal_kq_mulmm_k5_msl : metal_kq_mulmm_k6_msl)))
    let entry = (tensor
        ? (fmt == 4 ? metal_kq_mulmm_k4_t_msl_entry : (fmt == 5 ? metal_kq_mulmm_k5_t_msl_entry : metal_kq_mulmm_k6_t_msl_entry))
        : (fmt == 4 ? metal_kq_mulmm_k4_msl_entry : (fmt == 5 ? metal_kq_mulmm_k5_msl_entry : metal_kq_mulmm_k6_msl_entry)))
    let fm = (tensor
        ? (fmt == 4 ? metal_kq_mulmm_k4_t_msl_fastmath : (fmt == 5 ? metal_kq_mulmm_k5_t_msl_fastmath : metal_kq_mulmm_k6_t_msl_fastmath))
        : (fmt == 4 ? metal_kq_mulmm_k4_msl_fastmath : (fmt == 5 ? metal_kq_mulmm_k5_msl_fastmath : metal_kq_mulmm_k6_msl_fastmath)))
    let tgm = (tensor
        ? (fmt == 4 ? metal_kq_mulmm_k4_t_msl_tgmem : (fmt == 5 ? metal_kq_mulmm_k5_t_msl_tgmem : metal_kq_mulmm_k6_t_msl_tgmem))
        : (fmt == 4 ? metal_kq_mulmm_k4_msl_tgmem : (fmt == 5 ? metal_kq_mulmm_k5_msl_tgmem : metal_kq_mulmm_k6_msl_tgmem)))
    var pso = pipeline_from_source(dev, src, entry, fm, err)
    if (tensor && pso == null) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor support on this box — arm skipped\n")
        return
    }
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nb = kdim / 256
    let nsb = ndim * nb
    var kq : array<uint8>
    var ks : array<uint8>
    kq_fill_planes(fmt, nsb, kq, ks)
    var xf : array<float>
    xf |> resize(m * kdim)
    for (i in range(m * kdim)) {
        xf[i] = 0.07 * float(i % 39) - 1.3
    }
    var want : array<float>
    var env : array<float>
    var wrow : array<float>
    want |> resize(m * ndim)
    env |> resize(m * ndim)
    wrow |> resize(kdim)
    for (j in range(ndim)) {
        kq_row_ref(fmt, j * nb, nb, nsb, kq, ks, wrow)
        for (r in range(m)) {
            var acc = 0.0lf
            var ev = 0.0lf
            for (c in range(kdim)) {
                let p = double(wrow[c]) * double(xf[r * kdim + c])
                acc += p
                ev += abs(p)
            }
            want[r * ndim + j] = float(acc)
            env[r * ndim + j] = float(ev)
        }
    }
    var bks = buf_upload(dev, ks)
    var bkq = buf_upload(dev, kq)
    var bx = buf_upload(dev, xf)
    var by = buf_fill(dev, m * ndim, -1000.0)
    var bk = buf_u32(dev, uint(kdim))
    var bn = buf_u32(dev, uint(ndim))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, tgm, 0)
        metal_set_buffer(enc, bks, fmt == 6 ? uint64(nsb * 16) : 0ul, 0)   // k6: the f16 d tail
        metal_set_buffer(enc, bks, 0ul, 1)
        metal_set_buffer(enc, bkq, 0ul, 2)
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_buffer(enc, bk, 0ul, 5)
        metal_set_buffer(enc, bn, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-4, tag), 0)
    }
    metal_release(bks)
    metal_release(bkq)
    metal_release(bx)
    metal_release(by)
    metal_release(bk)
    metal_release(bn)
    metal_release(pso)
    delete kq
    delete ks
    delete xf
    delete want
    delete env
    delete wrow
}

// one 34B-block q8 blob (nrows x nkb blocks) + its exact dequant: pow2 scales and small-int
// quants keep every staged f16/f32 value exact, leaving only accumulation order to the envelope
def private q8_blob_fill(nrows, nkb : int; var blob : array<uint8>; var wdeq : array<float>) {
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    blob |> resize(nrows * nkb * 34)
    wdeq |> resize(nrows * nkb * 32)
    unsafe {
        for (b in range(nrows * nkb)) {
            let d = lut[(b * 7 + 1) % 4]
            let dbits = f32_to_f16(d)
            blob[b * 34] = uint8(dbits & 0xFFu)
            blob[b * 34 + 1] = uint8(dbits >> 8u)
            for (c in range(32)) {
                let q = (b * 5 + c * 11) % 16 - 8
                blob[b * 34 + 2 + c] = uint8(q & 0xFF)
                wdeq[b * 32 + c] = float(q) * d
            }
        }
    }
}

// exact-under-f16-staging X rows: small ints scaled by pow2 (the q8 gates' shared shape)
def private q8_x_fill(nn : int; var xf : array<float>) {
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    xf |> resize(nn)
    for (i in range(nn)) {
        xf[i] = float((i * 5 + i / 17) % 16 - 8) * lut[(i / 3) % 4]
    }
}

// the Metal-4 tensor twin of the (already direct-covered) MetalQ8MulMm — same bindings, no tgmem
def private q8_mulmm_t_gate(t : T?; dev, queue; m, kdim, ndim : int) {
    let tag = "q8_mulmm_t m={m} k={kdim} n={ndim}"
    var err : string
    var pso = pipeline_from_source(dev, metal_q8_mulmm_t_msl, metal_q8_mulmm_t_msl_entry, metal_q8_mulmm_t_msl_fastmath, err)
    if (pso == null) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor support on this box — arm skipped\n")
        return
    }
    let nb = kdim / 32
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(ndim, nb, blob, wdeq)
    var xf : array<float>
    q8_x_fill(m * kdim, xf)
    var want : array<float>
    var env : array<float>
    want |> resize(m * ndim)
    env |> resize(m * ndim)
    for (j in range(ndim)) {
        for (r in range(m)) {
            var acc = 0.0lf
            var ev = 0.0lf
            for (c in range(kdim)) {
                let p = double(wdeq[j * kdim + c]) * double(xf[r * kdim + c])
                acc += p
                ev += abs(p)
            }
            want[r * ndim + j] = float(acc)
            env[r * ndim + j] = float(ev)
        }
    }
    var bw = buf_upload(dev, blob)
    var bx = buf_upload(dev, xf)
    var by = buf_fill(dev, m * ndim, -1000.0)
    var bk = buf_u32(dev, uint(kdim))
    var bn = buf_u32(dev, uint(ndim))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)
        metal_set_buffer(enc, bw, 0ul, 1)
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_buffer(enc, bk, 0ul, 4)
        metal_set_buffer(enc, bn, 0ul, 5)
        metal_dispatch_threadgroups(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-3, tag), 0)
    }
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(bk)
    metal_release(bn)
    metal_release(pso)
    delete blob
    delete xf
    delete want
    delete env
    delete wdeq
}

// the native-bf16 A-stage pair: W is bf16 halfwords (top 16 bits of f32), row-major [ndim x kdim]
def private bf16_mulmm_gate(t : T?; dev, queue; tensor : bool; m, kdim, ndim : int) {
    let tag = "bf16_mulmm{tensor ? "_t" : ""} m={m} k={kdim} n={ndim}"
    var err : string
    var pso = (tensor
        ? pipeline_from_source(dev, metal_bf16_mulmm_t_msl, metal_bf16_mulmm_t_msl_entry, metal_bf16_mulmm_t_msl_fastmath, err)
        : pipeline_from_source(dev, metal_bf16_mulmm_msl, metal_bf16_mulmm_msl_entry, metal_bf16_mulmm_msl_fastmath, err))
    if (pso == null) {
        // the base needs bfloat MSL support, the twin Metal-4 tensor — either absence is a box fact
        to_log(LOG_INFO, "{tag}: pipeline unavailable on this box — arm skipped\n")
        return
    }
    var wb : array<uint16>
    wb |> resize(ndim * kdim)
    var wdeq : array<float>
    wdeq |> resize(ndim * kdim)
    for (i in range(ndim * kdim)) {
        wb[i] = uint16(float_bits_to_uint(0.125 * float((i * 3) % 13 - 6)) >> 16u)
        wdeq[i] = uint_bits_to_float(uint(wb[i]) << 16u)
    }
    var xf : array<float>
    xf |> resize(m * kdim)
    for (i in range(m * kdim)) {
        xf[i] = 0.11 * float(i % 37) - 1.9
    }
    var want : array<float>
    var env : array<float>
    want |> resize(m * ndim)
    env |> resize(m * ndim)
    for (j in range(ndim)) {
        for (r in range(m)) {
            var acc = 0.0lf
            var ev = 0.0lf
            for (c in range(kdim)) {
                let p = double(wdeq[j * kdim + c]) * double(xf[r * kdim + c])
                acc += p
                ev += abs(p)
            }
            want[r * ndim + j] = float(acc)
            env[r * ndim + j] = float(ev)
        }
    }
    var bw = buf_upload(dev, wb)
    var bx = buf_upload(dev, xf)
    var by = buf_fill(dev, m * ndim, -1000.0)
    var bk = buf_u32(dev, uint(kdim))
    var bn = buf_u32(dev, uint(ndim))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        if (!tensor) {
            metal_set_threadgroup_memory_length(enc, metal_bf16_mulmm_msl_tgmem, 0)
        }
        metal_set_buffer(enc, bw, 0ul, 0)
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_buffer(enc, bk, 0ul, 4)
        metal_set_buffer(enc, bn, 0ul, 5)
        metal_dispatch_threadgroups(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-3, tag), 0)
    }
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(bk)
    metal_release(bn)
    metal_release(pso)
    delete wb
    delete wdeq
    delete xf
    delete want
    delete env
}

// The M-pad-32 batched-decode GEMM (MetalQ8GemmB) and its Metal-4 tensor twin: X [mp32 x kdim]
// f32 token rows, dual-view q8 blob W, Y stored at TOKEN-row stride ys — the fused-QKV segment
// stride at QKV sites, so ys > ndim must leave the gap columns untouched (the sentinel
// whole-buffer compare asserts exactly that).
def private q8_gemm_b_gate(t : T?; dev, queue; tensor : bool; mp32, kdim, ndim, ys : int) {
    let tag = "q8_gemm_b{tensor ? "t" : ""} m={mp32} k={kdim} n={ndim} ys={ys}"
    var err : string
    var pso = (tensor
        ? pipeline_from_source(dev, metal_q8_gemm_b_t_msl, metal_q8_gemm_b_t_msl_entry, metal_q8_gemm_b_t_msl_fastmath, err)
        : pipeline_from_source(dev, metal_q8_gemm_b_msl, metal_q8_gemm_b_msl_entry, metal_q8_gemm_b_msl_fastmath, err))
    if (tensor && pso == null) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor support on this box — arm skipped\n")
        return
    }
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(ndim, kdim / 32, blob, wdeq)
    var xf : array<float>
    q8_x_fill(mp32 * kdim, xf)
    var want : array<float>
    var env : array<float>
    want |> resize(mp32 * ys)
    env |> resize(mp32 * ys)
    for (i in range(mp32 * ys)) {
        want[i] = -1000.0
        env[i] = 0.0
    }
    for (r in range(mp32)) {
        for (j in range(ndim)) {
            var acc = 0.0lf
            var ev = 0.0lf
            for (c in range(kdim)) {
                let p = double(wdeq[j * kdim + c]) * double(xf[r * kdim + c])
                acc += p
                ev += abs(p)
            }
            want[r * ys + j] = float(acc)
            env[r * ys + j] = float(ev)
        }
    }
    var ka = gemm_ka(int64(kdim), int64(ndim), int64(ys))
    var bw = buf_upload(dev, blob)
    var bx = buf_upload(dev, xf)
    var by = buf_fill(dev, mp32 * ys, -1000.0)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        if (!tensor) {
            metal_set_threadgroup_memory_length(enc, metal_q8_gemm_b_msl_tgmem, 0)
        }
        metal_set_buffer(enc, bw, 0ul, 0)   // byte view
        metal_set_buffer(enc, bw, 0ul, 2)   // half-scale view of the same blob
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        metal_dispatch_threadgroups(enc, uint3(uint((mp32 / 32) * (ndim / 32)), 1u, 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-3, tag), 0)
    }
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(pso)
    delete blob
    delete wdeq
    delete xf
    delete want
    delete env
}

// The split-K pair (MetalQ8GemmBSk[T] + MetalSkReduce) run as the production two-dispatch chain
// in one serial encoder: grid.y is the k-slice axis, each slice's partial-C plane is checked
// against a per-slice CPU dot, then the reduce's summed y against the full dot. mp == 32 is the
// family's policy gate.
def private q8_gemm_sk_gate(t : T?; dev, queue; tensor : bool; kdim, ndim, ksplit : int) {
    let tag = "q8_gemm_sk{tensor ? "t" : ""} k={kdim} n={ndim} sk={ksplit}"
    var err : string
    var pso = (tensor
        ? pipeline_from_source(dev, metal_q8_gemm_b_sk_t_msl, metal_q8_gemm_b_sk_t_msl_entry, metal_q8_gemm_b_sk_t_msl_fastmath, err)
        : pipeline_from_source(dev, metal_q8_gemm_b_sk_msl, metal_q8_gemm_b_sk_msl_entry, metal_q8_gemm_b_sk_msl_fastmath, err))
    if (tensor && pso == null) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor support on this box — arm skipped\n")
        return
    }
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var psoR = pipeline_from_source(dev, metal_sk_reduce_msl, metal_sk_reduce_msl_entry, metal_sk_reduce_msl_fastmath, err)
    t |> success(psoR != null, "{tag}: reduce pipeline: {err}")
    if (psoR == null) {
        metal_release(pso)
        return
    }
    let mp = 32
    let kslice = kdim / ksplit
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(ndim, kdim / 32, blob, wdeq)
    var xf : array<float>
    q8_x_fill(mp * kdim, xf)
    var pwant : array<float>
    var penv : array<float>
    var ywant : array<float>
    var yenv : array<float>
    pwant |> resize(ksplit * mp * ndim)
    penv |> resize(ksplit * mp * ndim)
    ywant |> resize(mp * ndim)
    yenv |> resize(mp * ndim)
    for (r in range(mp)) {
        for (j in range(ndim)) {
            var yacc = 0.0lf
            var yev = 0.0lf
            for (sl in range(ksplit)) {
                var acc = 0.0lf
                var ev = 0.0lf
                for (c in range(sl * kslice, (sl + 1) * kslice)) {
                    let p = double(wdeq[j * kdim + c]) * double(xf[r * kdim + c])
                    acc += p
                    ev += abs(p)
                }
                pwant[sl * mp * ndim + r * ndim + j] = float(acc)
                penv[sl * mp * ndim + r * ndim + j] = float(ev)
                yacc += acc
                yev += ev
            }
            ywant[r * ndim + j] = float(yacc)
            yenv[r * ndim + j] = float(yev)
        }
    }
    var ka = gemm_sk_ka(int64(kdim), int64(ndim), int64(ksplit))
    var kaR = SkReduceArgs(total = uint(mp * ndim), ksplit = uint(ksplit))
    var bw = buf_upload(dev, blob)
    var bx = buf_upload(dev, xf)
    var bpart = buf_fill(dev, ksplit * mp * ndim, -1000.0)
    var by = buf_fill(dev, mp * ndim, -1000.0)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        if (!tensor) {
            metal_set_threadgroup_memory_length(enc, metal_q8_gemm_b_sk_msl_tgmem, 0)
        }
        metal_set_buffer(enc, bw, 0ul, 0)   // byte view
        metal_set_buffer(enc, bw, 0ul, 2)   // half-scale view of the same blob
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, bpart, 0ul, 4)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        metal_dispatch_threadgroups(enc, uint3(uint(ndim / 32), uint(ksplit), 1u), uint3(128u, 1u, 1u))
        metal_set_pipeline(enc, psoR)
        metal_set_buffer(enc, bpart, 0ul, 0)
        metal_set_buffer(enc, by, 0ul, 1)
        metal_set_bytes(enc, unsafe(addr<void?>(kaR)), uint64(typeinfo sizeof(kaR)), 2)
        metal_dispatch_threadgroups(enc, uint3(uint((mp * ndim + 127) / 128), 1u, 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(bpart, pwant, penv, 2e-3, "{tag} part"), 0)
        t |> equal(buf_mismatch_env(by, ywant, yenv, 2e-3, "{tag} y"), 0)
    }
    metal_release(bw)
    metal_release(bx)
    metal_release(bpart)
    metal_release(by)
    metal_release(pso)
    metal_release(psoR)
    delete blob
    delete wdeq
    delete xf
    delete pwant
    delete penv
    delete ywant
    delete yenv
}

// The 64-wide N tile (MetalQ8Gemm64B[T], the classifier site): same M-pad-32 contract but
// ndim % 64 and y at DENSE stride ka.ndim — ys rides along as 0 exactly as the production
// classifier call does, proving the kernel never reads it.
def private q8_gemm64_b_gate(t : T?; dev, queue; tensor : bool; mp32, kdim, ndim : int) {
    let tag = "q8_gemm64_b{tensor ? "t" : ""} m={mp32} k={kdim} n={ndim}"
    var err : string
    var pso = (tensor
        ? pipeline_from_source(dev, metal_q8_gemm64_b_t_msl, metal_q8_gemm64_b_t_msl_entry, metal_q8_gemm64_b_t_msl_fastmath, err)
        : pipeline_from_source(dev, metal_q8_gemm64_b_msl, metal_q8_gemm64_b_msl_entry, metal_q8_gemm64_b_msl_fastmath, err))
    if (tensor && pso == null) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor support on this box — arm skipped\n")
        return
    }
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(ndim, kdim / 32, blob, wdeq)
    var xf : array<float>
    q8_x_fill(mp32 * kdim, xf)
    var want : array<float>
    var env : array<float>
    want |> resize(mp32 * ndim)
    env |> resize(mp32 * ndim)
    for (r in range(mp32)) {
        for (j in range(ndim)) {
            var acc = 0.0lf
            var ev = 0.0lf
            for (c in range(kdim)) {
                let p = double(wdeq[j * kdim + c]) * double(xf[r * kdim + c])
                acc += p
                ev += abs(p)
            }
            want[r * ndim + j] = float(acc)
            env[r * ndim + j] = float(ev)
        }
    }
    var ka = gemm_ka(int64(kdim), int64(ndim), 0l)
    var bw = buf_upload(dev, blob)
    var bx = buf_upload(dev, xf)
    var by = buf_fill(dev, mp32 * ndim, -1000.0)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        if (!tensor) {
            metal_set_threadgroup_memory_length(enc, metal_q8_gemm64_b_msl_tgmem, 0)
        }
        metal_set_buffer(enc, bw, 0ul, 0)   // byte view
        metal_set_buffer(enc, bw, 0ul, 2)   // half-scale view of the same blob
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        metal_dispatch_threadgroups(enc, uint3(uint((mp32 / 32) * (ndim / 64)), 1u, 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-3, tag), 0)
    }
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(pso)
    delete blob
    delete wdeq
    delete xf
    delete want
    delete env
}

// ===== the MoE gathered mul_mm family (Wave C) =====

// Padded CSR route fixture: expert e's rows start at a 32-aligned base (the route prefix
// contract). gather=1 rows read X through bkt (flat slot / nk); padded tail rows clamp to the
// LAST live entry exactly as the kernels do, so srcrow carries the already-clamped answer.
struct private MoeFix {
    ne : int
    pad : int           // total padded bucket rows
    maxtiles : int      // grid.x — experts with fewer padded tiles exit in-kernel
    cnt : array<uint>
    basep : array<uint>
    bkt : array<uint>
    srcrow : array<int> // per padded bucket row: the X row its result reads
}

def private moe_fix(counts : array<int>; gather : bool; nk, nx : int) : MoeFix {
    var fx : MoeFix
    fx.ne = length(counts)
    fx.cnt |> reserve(fx.ne)
    fx.basep |> reserve(fx.ne)
    var pad = 0
    var mt = 1
    for (c in counts) {
        fx.cnt |> push(uint(c))
        fx.basep |> push(uint(pad))
        let rows = (c + 31) / 32 * 32
        mt = max(mt, rows / 32)
        pad += rows
    }
    fx.pad = pad
    fx.maxtiles = mt
    fx.srcrow |> resize(pad)
    if (gather) {
        fx.bkt |> resize(pad)
        for (e in range(fx.ne)) {
            let rb = int(fx.basep[e])
            let rows = (counts[e] + 31) / 32 * 32
            for (r in range(rows)) {
                if (r < counts[e]) {
                    fx.bkt[rb + r] = uint((r * 7 + e * 5 + 3) % (nx * nk))
                }
                fx.srcrow[rb + r] = int(fx.bkt[rb + min(r, counts[e] - 1)]) / nk
            }
        }
    } else {
        for (r in range(pad)) {
            fx.srcrow[r] = r
        }
    }
    return <- fx
}

// shared truth walk: want/env over the whole padded bucket panel from a flat [nrows_w x kdim]
// dequant plane (row of expert e, col j at wdeq[(e*ndim + j)*kdim]), plus an optional bias row
def private moe_mm_truth(fx : MoeFix; wdeq, xf, wb : array<float>; kdim, ndim : int; hasb : bool;
                         var want, env : array<float>) {
    want |> resize(fx.pad * ndim)
    env |> resize(fx.pad * ndim)
    for (e in range(fx.ne)) {
        let rows = (int(fx.cnt[e]) + 31) / 32 * 32
        for (r in range(rows)) {
            let row = int(fx.basep[e]) + r
            let src = fx.srcrow[row]
            for (j in range(ndim)) {
                var acc = 0.0lf
                var ev = 0.0lf
                for (c in range(kdim)) {
                    let p = double(wdeq[(e * ndim + j) * kdim + c]) * double(xf[src * kdim + c])
                    acc += p
                    ev += abs(p)
                }
                if (hasb) {
                    acc += double(wb[e * ndim + j])
                    ev += abs(double(wb[e * ndim + j]))
                }
                want[row * ndim + j] = float(acc)
                env[row * ndim + j] = float(ev)
            }
        }
    }
}

// The gathered K-quant mul_mm trio (MetalMoeMulMmK4/K5/K6): per-expert superblock planes off
// the shared kq fixture, truth through the shipped dequant_k*_plane_superblock. Binds at the
// MetalMoeMulMmBase family numbers (0..2 weight views, x 3, y 4, cnt/base/bkt 7..9); k6's
// binding 0 is the f16 d-tail view of the scale plane.
def private moe_mulmm_kq_gate(t : T?; dev, queue; fmt : int; kdim, ndim : int; counts : array<int>; gather : bool) {
    let tag = "moe_mulmm_k{fmt}{gather ? " gather" : ""} k={kdim} n={ndim}"
    var err : string
    let src = fmt == 4 ? metal_moe_mulmm_k4_msl : (fmt == 5 ? metal_moe_mulmm_k5_msl : metal_moe_mulmm_k6_msl)
    let entry = fmt == 4 ? metal_moe_mulmm_k4_msl_entry : (fmt == 5 ? metal_moe_mulmm_k5_msl_entry : metal_moe_mulmm_k6_msl_entry)
    let fm = fmt == 4 ? metal_moe_mulmm_k4_msl_fastmath : (fmt == 5 ? metal_moe_mulmm_k5_msl_fastmath : metal_moe_mulmm_k6_msl_fastmath)
    let tgm = fmt == 4 ? metal_moe_mulmm_k4_msl_tgmem : (fmt == 5 ? metal_moe_mulmm_k5_msl_tgmem : metal_moe_mulmm_k6_msl_tgmem)
    var pso = pipeline_from_source(dev, src, entry, fm, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let ne = length(counts)
    let nb = kdim / 256
    let eplane = ndim * nb
    let nsb = ne * eplane
    var kq : array<uint8>
    var ks : array<uint8>
    kq_fill_planes(fmt, nsb, kq, ks)
    let nk = 2
    let nx = 24
    var fx <- moe_fix(counts, gather, nk, nx)
    var xf : array<float>
    xf |> resize((gather ? nx : fx.pad) * kdim)
    for (i in range(length(xf))) {
        xf[i] = float((i * 5) % 9 - 4)
    }
    var wdeq : array<float>
    wdeq |> resize(ne * ndim * kdim)
    var wrow : array<float>
    wrow |> resize(kdim)
    for (e in range(ne)) {
        for (j in range(ndim)) {
            kq_row_ref(fmt, e * eplane + j * nb, nb, nsb, kq, ks, wrow)
            for (c in range(kdim)) {
                wdeq[(e * ndim + j) * kdim + c] = wrow[c]
            }
        }
    }
    var wb : array<float>
    var want : array<float>
    var env : array<float>
    moe_mm_truth(fx, wdeq, xf, wb, kdim, ndim, false, want, env)
    var ka = MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim), eplane = uint(eplane),
        nk = uint(nk), gather = gather ? 1u : 0u)
    var bks = buf_upload(dev, ks)
    var bkq = buf_upload(dev, kq)
    var bx = buf_upload(dev, xf)
    var by = buf_fill(dev, fx.pad * ndim, -1000.0)
    var bcnt = buf_upload(dev, fx.cnt)
    var bbase = buf_upload(dev, fx.basep)
    var bbkt = gather ? buf_upload(dev, fx.bkt) : buf_u32(dev, 0u)   // gather=0: unread 4B stand-in
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, tgm, 0)
        metal_set_buffer(enc, bks, fmt == 6 ? uint64(nsb * 16) : 0ul, 0)   // k6: the f16 d tail
        metal_set_buffer(enc, bks, 0ul, 1)
        metal_set_buffer(enc, bkq, 0ul, 2)
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        metal_set_buffer(enc, bcnt, 0ul, 7)
        metal_set_buffer(enc, bbase, 0ul, 8)
        metal_set_buffer(enc, bbkt, 0ul, 9)
        metal_dispatch_threadgroups(enc, uint3(uint(fx.maxtiles), uint(ndim / 64), uint(ne)), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-4, tag), 0)
    }
    metal_release(bks)
    metal_release(bkq)
    metal_release(bx)
    metal_release(by)
    metal_release(bcnt)
    metal_release(bbase)
    metal_release(bbkt)
    metal_release(pso)
    delete kq
    delete ks
    delete fx
    delete xf
    delete wdeq
    delete wrow
    delete wb
    delete want
    delete env
}

// The gathered q8 mul_mm (MetalMoeMulMmQ8) and its tensor twin — the twin serves CONTIGUOUS
// (gather=0, down-site) shapes only and never binds bkt.
def private moe_mulmm_q8_gate(t : T?; dev, queue; tensor : bool; kdim, ndim : int; counts : array<int>; gather : bool) {
    let tag = "moe_mulmm_q8{tensor ? "t" : ""}{gather ? " gather" : ""} k={kdim} n={ndim}"
    var err : string
    var pso = (tensor
        ? pipeline_from_source(dev, metal_moe_mulmm_q8_t_msl, metal_moe_mulmm_q8_t_msl_entry, metal_moe_mulmm_q8_t_msl_fastmath, err)
        : pipeline_from_source(dev, metal_moe_mulmm_q8_msl, metal_moe_mulmm_q8_msl_entry, metal_moe_mulmm_q8_msl_fastmath, err))
    if (tensor && pso == null) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor support on this box — arm skipped\n")
        return
    }
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let ne = length(counts)
    let nkb = kdim / 32
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(ne * ndim, nkb, blob, wdeq)
    let nk = 2
    let nx = 24
    var fx <- moe_fix(counts, gather, nk, nx)
    var xf : array<float>
    q8_x_fill((gather ? nx : fx.pad) * kdim, xf)
    var wb : array<float>
    var want : array<float>
    var env : array<float>
    moe_mm_truth(fx, wdeq, xf, wb, kdim, ndim, false, want, env)
    var ka = MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim), eplane = uint(ndim * nkb),
        nk = uint(nk), gather = gather ? 1u : 0u)
    var bw = buf_upload(dev, blob)
    var bx = buf_upload(dev, xf)
    var by = buf_fill(dev, fx.pad * ndim, -1000.0)
    var bcnt = buf_upload(dev, fx.cnt)
    var bbase = buf_upload(dev, fx.basep)
    var bbkt = gather ? buf_upload(dev, fx.bkt) : buf_u32(dev, 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        if (!tensor) {
            metal_set_threadgroup_memory_length(enc, metal_moe_mulmm_q8_msl_tgmem, 0)
        }
        metal_set_buffer(enc, bw, 0ul, 0)   // half-scale view
        metal_set_buffer(enc, bw, 0ul, 1)   // byte view of the same blob
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        metal_set_buffer(enc, bcnt, 0ul, 6)
        metal_set_buffer(enc, bbase, 0ul, 7)
        if (!tensor) {
            metal_set_buffer(enc, bbkt, 0ul, 8)
        }
        metal_dispatch_threadgroups(enc, uint3(uint(fx.maxtiles), uint(ndim / 64), uint(ne)), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-3, tag), 0)
    }
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(bcnt)
    metal_release(bbase)
    metal_release(bbkt)
    metal_release(pso)
    delete blob
    delete wdeq
    delete fx
    delete xf
    delete wb
    delete want
    delete env
}

// The gathered MXFP4 mul_mm (MetalMoeMulMmMx4) and its tensor twin: nibble plane + e8m0 scale
// plane + host-built vtab, per-expert bias folded when hasb (accumulator seed in the base,
// rank-1 chunk in the twin). Truth replicates the kernels' exact e8m0 decode.
def private moe_mulmm_mx4_gate(t : T?; dev, queue; tensor : bool; kdim, ndim : int; counts : array<int>; gather, hasb : bool) {
    let tag = "moe_mulmm_mx4{tensor ? "t" : ""}{gather ? " gather" : ""}{hasb ? " bias" : ""} k={kdim} n={ndim}"
    var err : string
    var pso = (tensor
        ? pipeline_from_source(dev, metal_moe_mulmm_mx4_t_msl, metal_moe_mulmm_mx4_t_msl_entry, metal_moe_mulmm_mx4_t_msl_fastmath, err)
        : pipeline_from_source(dev, metal_moe_mulmm_mx4_msl, metal_moe_mulmm_mx4_msl_entry, metal_moe_mulmm_mx4_msl_fastmath, err))
    if (tensor && pso == null) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor support on this box — arm skipped\n")
        return
    }
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let ne = length(counts)
    let nkb = kdim / 32
    let tblk = ne * ndim * nkb
    var vtab : array<float>
    vtab |> resize(16)
    for (i in range(8)) {
        let v = float(fixed_array(0, 1, 2, 3, 4, 6, 8, 12)[i])
        vtab[i] = v
        vtab[i + 8] = -v
    }
    var nplane : array<uint>
    nplane |> resize(tblk * 4)
    for (i in range(tblk * 4)) {
        nplane[i] = uint(i * 3 + 1) * 2654435761u
    }
    var eplane8 : array<uint8>
    eplane8 |> resize(tblk)
    for (i in range(tblk)) {
        eplane8[i] = uint8(124 + (i * 3 + 1) % 8)   // pow2 scales 2^-4..2^3 — f16-exact products
    }
    var wdeq : array<float>
    wdeq |> resize(ne * ndim * kdim)
    for (blk in range(tblk)) {
        let sc = uint(eplane8[blk])
        let d = uint_bits_to_float(sc < 2u ? (0x00200000u << sc) : ((sc - 1u) << 23u))
        for (k in range(32)) {
            let byi = k % 16
            let nby = (nplane[blk * 4 + byi / 4] >> uint((byi % 4) * 8)) & 255u
            wdeq[blk * 32 + k] = vtab[int(k < 16 ? (nby & 15u) : (nby >> 4u))] * d
        }
    }
    var wb : array<float>
    wb |> resize(ne * ndim)
    for (i in range(ne * ndim)) {
        wb[i] = float((i * 11) % 9 - 4) * 0.25
    }
    let nk = 2
    let nx = 24
    var fx <- moe_fix(counts, gather, nk, nx)
    var xf : array<float>
    q8_x_fill((gather ? nx : fx.pad) * kdim, xf)
    var want : array<float>
    var env : array<float>
    moe_mm_truth(fx, wdeq, xf, wb, kdim, ndim, hasb, want, env)
    var ka = MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim), eplane = uint(ndim * nkb),
        nk = uint(nk), gather = gather ? 1u : 0u, hasb = hasb ? 1u : 0u)
    var bnq = buf_upload(dev, nplane)
    var bee = buf_upload(dev, eplane8)
    var bvt = buf_upload(dev, vtab)
    var bwb = buf_upload(dev, wb)
    var bx = buf_upload(dev, xf)
    var by = buf_fill(dev, fx.pad * ndim, -1000.0)
    var bcnt = buf_upload(dev, fx.cnt)
    var bbase = buf_upload(dev, fx.basep)
    var bbkt = gather ? buf_upload(dev, fx.bkt) : buf_u32(dev, 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, tensor ? metal_moe_mulmm_mx4_t_msl_tgmem : metal_moe_mulmm_mx4_msl_tgmem, 0)
        metal_set_buffer(enc, bnq, 0ul, 0)
        metal_set_buffer(enc, bee, 0ul, 1)
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        metal_set_buffer(enc, bcnt, 0ul, 6)
        metal_set_buffer(enc, bbase, 0ul, 7)
        if (!tensor) {
            metal_set_buffer(enc, bbkt, 0ul, 8)
        }
        metal_set_buffer(enc, bwb, 0ul, 12)
        metal_set_buffer(enc, bvt, 0ul, 13)
        metal_dispatch_threadgroups(enc, uint3(uint(fx.maxtiles), uint(ndim / 64), uint(ne)), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-3, tag), 0)
    }
    metal_release(bnq)
    metal_release(bee)
    metal_release(bvt)
    metal_release(bwb)
    metal_release(bx)
    metal_release(by)
    metal_release(bcnt)
    metal_release(bbase)
    metal_release(bbkt)
    metal_release(pso)
    delete vtab
    delete nplane
    delete eplane8
    delete wdeq
    delete wb
    delete fx
    delete xf
    delete want
    delete env
}

// ===== the prefill attention GEMM pair + Metal-4 tensor twins =====

// MetalAttnQKMm[T]: raw scores att[h x qrows x np32] = scale * q · k over surviving 32x64
// blocks — blocks fully above the causal diagonal (or fully outside the sliding window) exit
// without writing, which the sentinel whole-slab compare asserts. QK arms keep npos == np32:
// the tensor twin stages K pad rows RAW (no zero guard — the base has one), so pads must not
// exist for the pair to agree.
def private attn_qk_mm_gate(t : T?; dev, queue; tensor : bool; heads, hs, kv_mul, mp, np32, qoff, window : int) {
    let tag = "attn_qk_mm{tensor ? "t" : ""} h={heads} hs={hs} m={mp} n={np32} qoff={qoff} w={window}"
    var err : string
    var pso = (tensor
        ? pipeline_from_source(dev, metal_attn_qk_mm_t_msl, metal_attn_qk_mm_t_msl_entry, metal_attn_qk_mm_t_msl_fastmath, err)
        : pipeline_from_source(dev, metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath, err))
    if (tensor && pso == null) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor support on this box — arm skipped\n")
        return
    }
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let qd = heads * hs
    let kvd = (heads / kv_mul) * hs
    let scale = 0.125
    var qv : array<float>
    q8_x_fill(mp * qd, qv)
    var kv : array<float>
    kv |> resize(np32 * kvd)
    for (i in range(np32 * kvd)) {
        kv[i] = float((i * 7 + 3) % 15 - 7) * 0.5
    }
    var want : array<float>
    var env : array<float>
    want |> resize(heads * mp * np32)
    env |> resize(heads * mp * np32)
    for (i in range(heads * mp * np32)) {
        want[i] = -1000.0
        env[i] = 0.0
    }
    for (h in range(heads)) {
        let kvhoff = (h / kv_mul) * hs
        for (m in range(mp)) {
            let mBase = m / 32 * 32
            for (nb in range(np32 / 64)) {
                let nBase = nb * 64
                let skip = (nBase > qoff + mBase + 31
                    || (window > 0 && qoff + mBase + 1 > window && nBase + 64 <= qoff + mBase + 1 - window))
                continue if (skip)
                for (pos in range(nBase, nBase + 64)) {
                    var acc = 0.0lf
                    var ev = 0.0lf
                    for (c in range(hs)) {
                        let p = double(qv[m * qd + h * hs + c]) * double(scale) * double(kv[pos * kvd + kvhoff + c])
                        acc += p
                        ev += abs(p)
                    }
                    want[(h * mp + m) * np32 + pos] = float(acc)
                    env[(h * mp + m) * np32 + pos] = float(ev)
                }
            }
        }
    }
    var ka = AttnArgs(qd = uint(qd), kv_dim = uint(kvd), head_size = uint(hs), kv_mul = uint(kv_mul),
        npos = uint(np32), np32 = uint(np32), scale = scale, qoff = uint(qoff), qrows = uint(mp),
        window = uint(window))
    var bq = buf_upload(dev, qv)
    var bk = buf_upload(dev, kv)
    var batt = buf_fill(dev, heads * mp * np32, -1000.0)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, tensor ? metal_attn_qk_mm_t_msl_tgmem : metal_attn_qk_mm_msl_tgmem, 0)
        metal_set_buffer(enc, bq, 0ul, 0)
        metal_set_buffer(enc, bk, 0ul, 1)
        metal_set_buffer(enc, batt, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
        metal_dispatch_threadgroups(enc, uint3(uint(mp / 32), uint(np32 / 64), uint(heads)), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(batt, want, env, 2e-3, tag), 0)
    }
    metal_release(bq)
    metal_release(bk)
    metal_release(batt)
    metal_release(pso)
    delete qv
    delete kv
    delete want
    delete env
}

// MetalAttnAVMm[T]: xb[m x qd] = P · V with the normalized-P slab causally zero past
// qoff+m and past npos. The tensor twin zero-guards V rows >= npos — the npos < np32 arm
// fills the pad rows with garbage to prove the guard is live.
def private attn_av_mm_gate(t : T?; dev, queue; tensor : bool; heads, hs, kv_mul, mp, np32, npos, qoff : int) {
    let tag = "attn_av_mm{tensor ? "t" : ""} h={heads} hs={hs} m={mp} n={np32} npos={npos}"
    var err : string
    var pso = (tensor
        ? pipeline_from_source(dev, metal_attn_av_mm_t_msl, metal_attn_av_mm_t_msl_entry, metal_attn_av_mm_t_msl_fastmath, err)
        : pipeline_from_source(dev, metal_attn_av_mm_msl, metal_attn_av_mm_msl_entry, metal_attn_av_mm_msl_fastmath, err))
    if (tensor && pso == null) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor support on this box — arm skipped\n")
        return
    }
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let qd = heads * hs
    let kvd = (heads / kv_mul) * hs
    var attf : array<float>
    attf |> resize(heads * mp * np32)
    for (h in range(heads)) {
        for (m in range(mp)) {
            for (pos in range(np32)) {
                let live = pos <= qoff + m && pos < npos
                attf[(h * mp + m) * np32 + pos] = live ? float((h * 31 + m * 7 + pos * 3) % 13 - 6) * 0.25 : 0.0
            }
        }
    }
    var vv : array<float>
    vv |> resize(np32 * kvd)
    for (i in range(np32 * kvd)) {
        vv[i] = i / kvd < npos ? float((i * 7 + 3) % 15 - 7) * 0.5 : 999.0   // garbage pads: the guard's proof
    }
    var want : array<float>
    var env : array<float>
    want |> resize(mp * qd)
    env |> resize(mp * qd)
    for (h in range(heads)) {
        let kvhoff = (h / kv_mul) * hs
        for (m in range(mp)) {
            for (col in range(hs)) {
                var acc = 0.0lf
                var ev = 0.0lf
                for (pos in range(npos)) {
                    let p = double(attf[(h * mp + m) * np32 + pos]) * double(vv[pos * kvd + kvhoff + col])
                    acc += p
                    ev += abs(p)
                }
                want[m * qd + h * hs + col] = float(acc)
                env[m * qd + h * hs + col] = float(ev)
            }
        }
    }
    var ka = AttnArgs(qd = uint(qd), kv_dim = uint(kvd), head_size = uint(hs), kv_mul = uint(kv_mul),
        npos = uint(npos), np32 = uint(np32), scale = 1.0, qoff = uint(qoff), qrows = uint(mp))
    var batt = buf_upload(dev, attf)
    var bv = buf_upload(dev, vv)
    var bxb = buf_fill(dev, mp * qd, -1000.0)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, tensor ? metal_attn_av_mm_t_msl_tgmem : metal_attn_av_mm_msl_tgmem, 0)
        metal_set_buffer(enc, batt, 0ul, 0)
        metal_set_buffer(enc, bv, 0ul, 1)
        metal_set_buffer(enc, bxb, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
        metal_dispatch_threadgroups(enc, uint3(uint(mp / 32), uint(hs / 64), uint(heads)), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(bxb, want, env, 2e-3, tag), 0)
    }
    metal_release(batt)
    metal_release(bv)
    metal_release(bxb)
    metal_release(pso)
    delete attf
    delete vv
    delete want
    delete env
}

[test]
def test_metal_gemm_kernels(t : T?) {
    t |> run("prefill GEMM corpus (kq mul_mm trio + bf16 + Metal-4 tensor twins) == CPU references") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU kernel checks skipped\n")
                    return
                }
                var queue = metal_new_command_queue(dev)
                // the simdgroup kq trio + the tensor twins (twins feint without Metal-4)
                for (fmt in [4, 5, 6]) {
                    kq_mulmm_gate(t, dev, queue, fmt, false, 64, 512, 64)
                    kq_mulmm_gate(t, dev, queue, fmt, true, 64, 512, 64)
                }
                kq_mulmm_gate(t, dev, queue, 4, false, 32, 256, 128)   // second shape: 2 col tiles
                q8_mulmm_t_gate(t, dev, queue, 64, 256, 64)
                bf16_mulmm_gate(t, dev, queue, false, 64, 256, 64)
                bf16_mulmm_gate(t, dev, queue, true, 64, 256, 64)
                // the batched-decode q8 GEMM family: dense + fused-QKV ys stride, both twins
                q8_gemm_b_gate(t, dev, queue, false, 64, 128, 64, 64)
                q8_gemm_b_gate(t, dev, queue, false, 64, 128, 64, 128)
                q8_gemm_b_gate(t, dev, queue, true, 64, 128, 64, 64)
                q8_gemm_b_gate(t, dev, queue, true, 64, 128, 64, 128)
                q8_gemm_sk_gate(t, dev, queue, false, 128, 64, 2)
                q8_gemm_sk_gate(t, dev, queue, false, 128, 64, 4)      // kbn == 1 edge
                q8_gemm_sk_gate(t, dev, queue, true, 128, 64, 2)
                q8_gemm64_b_gate(t, dev, queue, false, 64, 128, 128)
                q8_gemm64_b_gate(t, dev, queue, true, 64, 128, 128)
                // the MoE gathered mul_mm family: kq trio + q8/mx4 with their tensor twins
                // (a zero-count expert rides the first arm; twins are contiguous-only by design)
                moe_mulmm_kq_gate(t, dev, queue, 4, 256, 64, [32, 0, 40, 8], true)
                moe_mulmm_kq_gate(t, dev, queue, 5, 256, 64, [32, 40, 8], true)
                moe_mulmm_kq_gate(t, dev, queue, 6, 256, 64, [32, 40, 8], true)
                moe_mulmm_kq_gate(t, dev, queue, 4, 256, 128, [8, 33], false)   // 2 col tiles + contiguous
                moe_mulmm_q8_gate(t, dev, queue, false, 96, 64, [32, 40, 8], true)
                moe_mulmm_q8_gate(t, dev, queue, false, 128, 64, [8, 33], false)
                moe_mulmm_q8_gate(t, dev, queue, true, 128, 64, [8, 33], false)
                moe_mulmm_mx4_gate(t, dev, queue, false, 96, 64, [32, 40, 8], true, true)
                moe_mulmm_mx4_gate(t, dev, queue, false, 128, 64, [8, 33], false, false)
                moe_mulmm_mx4_gate(t, dev, queue, true, 128, 64, [8, 33], false, true)
                moe_mulmm_mx4_gate(t, dev, queue, true, 128, 64, [8, 33], false, false)
                // the prefill attention GEMM pair: causal-skip, window-skip, GQA, 2-slab hs,
                // and the AV pad-guard arm (npos < np32, garbage V pads)
                attn_qk_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 128, 0, 0)
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 0, 0)
                attn_qk_mm_gate(t, dev, queue, false, 4, 64, 2, 32, 256, 224, 128)
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 32, 256, 224, 128)
                attn_qk_mm_gate(t, dev, queue, false, 2, 128, 1, 32, 64, 32, 0)   // two K-slabs
                attn_qk_mm_gate(t, dev, queue, true, 2, 128, 1, 32, 64, 32, 0)
                attn_av_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 128, 128, 64)
                attn_av_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 128, 64)
                attn_av_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 128, 96, 64)
                attn_av_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 96, 64)
                attn_av_mm_gate(t, dev, queue, false, 2, 128, 1, 32, 64, 64, 32)  // vBase=64 tile
                attn_av_mm_gate(t, dev, queue, true, 2, 128, 1, 32, 64, 64, 32)
                metal_release(queue)
            }
            t |> equal(metal_log_leaks_if_any(), 0l)
        } else {
            feint("das_metal is not built on this platform; GEMM kernel checks skipped\n")
        }
    }
}
