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
                metal_release(queue)
            }
            t |> equal(metal_log_leaks_if_any(), 0l)
        } else {
            feint("das_metal is not built on this platform; GEMM kernel checks skipped\n")
        }
    }
}
