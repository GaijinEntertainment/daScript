options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options _function_length = 0        // per-kernel gates: hand-bound buffers/uniforms + dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes
options _dasllama_internal = true

require dastest/testing_boost public
require daslib/math_bits            // float_bits_to_uint / uint_bits_to_float — the bf16 halfword pun
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
// [m x ndim]. A tensor twin skips only when the CAPABILITY probe says the toolchain has no
// Metal-4 tensor support (metal4_tensor_available); with the toolchain proven, a PSO that
// fails to compile is a RED, and the simdgroup bases always assert. Off-Apple (or with no
// Metal device) every check feints.

def private kq_mulmm_gate(t : T?; dev, queue; fmt : int; tensor : bool; m, kdim, ndim : int; halfx : bool = false) {
    let tag = "kq_mulmm_k{fmt}{tensor ? "t" : ""}{halfx ? " halfx" : ""} m={m} k={kdim} n={ndim}"
    var err : string
    let src = (tensor
        ? (halfx
            ? (fmt == 4 ? MetalKqMulMmK4TH_metal_kq_mulmm_t_msl : (fmt == 5 ? MetalKqMulMmK5TH_metal_kq_mulmm_t_msl : MetalKqMulMmK6TH_metal_kq_mulmm_k6_t_msl))
            : (fmt == 4 ? MetalKqMulMmK4T_metal_kq_mulmm_t_msl : (fmt == 5 ? MetalKqMulMmK5T_metal_kq_mulmm_t_msl : MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl)))
        : (fmt == 4 ? MetalKqMulMmK4_metal_kq_mulmm_msl : (fmt == 5 ? MetalKqMulMmK5_metal_kq_mulmm_msl : MetalKqMulMmK6_metal_kq_mulmm_msl)))
    let entry = (tensor
        ? (halfx
            ? (fmt == 4 ? MetalKqMulMmK4TH_metal_kq_mulmm_t_msl_entry : (fmt == 5 ? MetalKqMulMmK5TH_metal_kq_mulmm_t_msl_entry : MetalKqMulMmK6TH_metal_kq_mulmm_k6_t_msl_entry))
            : (fmt == 4 ? MetalKqMulMmK4T_metal_kq_mulmm_t_msl_entry : (fmt == 5 ? MetalKqMulMmK5T_metal_kq_mulmm_t_msl_entry : MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl_entry)))
        : (fmt == 4 ? MetalKqMulMmK4_metal_kq_mulmm_msl_entry : (fmt == 5 ? MetalKqMulMmK5_metal_kq_mulmm_msl_entry : MetalKqMulMmK6_metal_kq_mulmm_msl_entry)))
    let fm = (tensor
        ? (halfx
            ? (fmt == 4 ? MetalKqMulMmK4TH_metal_kq_mulmm_t_msl_fastmath : (fmt == 5 ? MetalKqMulMmK5TH_metal_kq_mulmm_t_msl_fastmath : MetalKqMulMmK6TH_metal_kq_mulmm_k6_t_msl_fastmath))
            : (fmt == 4 ? MetalKqMulMmK4T_metal_kq_mulmm_t_msl_fastmath : (fmt == 5 ? MetalKqMulMmK5T_metal_kq_mulmm_t_msl_fastmath : MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl_fastmath)))
        : (fmt == 4 ? MetalKqMulMmK4_metal_kq_mulmm_msl_fastmath : (fmt == 5 ? MetalKqMulMmK5_metal_kq_mulmm_msl_fastmath : MetalKqMulMmK6_metal_kq_mulmm_msl_fastmath)))
    let tgm = (tensor
        ? (halfx
            ? (fmt == 4 ? MetalKqMulMmK4TH_metal_kq_mulmm_t_msl_tgmem : (fmt == 5 ? MetalKqMulMmK5TH_metal_kq_mulmm_t_msl_tgmem : MetalKqMulMmK6TH_metal_kq_mulmm_k6_t_msl_tgmem))
            : (fmt == 4 ? MetalKqMulMmK4T_metal_kq_mulmm_t_msl_tgmem : (fmt == 5 ? MetalKqMulMmK5T_metal_kq_mulmm_t_msl_tgmem : MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl_tgmem)))
        : (fmt == 4 ? MetalKqMulMmK4_metal_kq_mulmm_msl_tgmem : (fmt == 5 ? MetalKqMulMmK5_metal_kq_mulmm_msl_tgmem : MetalKqMulMmK6_metal_kq_mulmm_msl_tgmem)))
    var pso = pipeline_from_source(dev, src, entry, fm, err)
    if (tensor && pso == null && !metal4_tensor_available(dev)) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor toolchain on this box - arm skipped\n")
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
        // halfx: round-trip through f16 so the oracle dot below stays the arm's exact reference
        xf[i] = halfx ? float(float16(0.07 * float(i % 39) - 1.3)) : 0.07 * float(i % 39) - 1.3
    }
    var xh : array<float16>
    if (halfx) {
        xh |> resize(m * kdim)
        for (i in range(m * kdim)) {
            xh[i] = float16(xf[i])
        }
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
    var bx = halfx ? buf_upload(dev, xh) : buf_upload(dev, xf)
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
    delete xh
    delete want
    delete env
    delete wrow
}

// The tensor arms' skip gate: a CAPABILITY probe, not the artifact under test (an artifact
// gate skips forever green when its producer regresses). Probed once, OR of two independent
// tensor sources — a single regressed source still reds its own arm while the other proves
// the toolchain; only both regressing together would misread as no-support.
var private g_m4_probed = false
var private g_m4_ok = false

def private metal4_tensor_available(dev) : bool {
    if (!g_m4_probed) {
        g_m4_probed = true
        var err : string
        var p1 = pipeline_from_source(dev, MetalQ8MulMmT_metal_q8_mulmm_t_msl, MetalQ8MulMmT_metal_q8_mulmm_t_msl_entry, MetalQ8MulMmT_metal_q8_mulmm_t_msl_fastmath, err)
        var p2 = pipeline_from_source(dev, MetalBf16MulMmT_metal_bf16_mulmm_t_msl, MetalBf16MulMmT_metal_bf16_mulmm_t_msl_entry, MetalBf16MulMmT_metal_bf16_mulmm_t_msl_fastmath, err)
        g_m4_ok = p1 != null || p2 != null
        metal_release(p1)
        metal_release(p2)
    }
    return g_m4_ok
}

// the Metal-4 tensor twin of the (already direct-covered) MetalQ8MulMm — same bindings; the
// q8u form dequant-stages W into @workgroup, so the gate binds its tgmem like production
def private q8_mulmm_t_gate(t : T?; dev, queue; m, kdim, ndim : int; halfx : bool = false; mt : int = 32) {
    let hx = halfx ? " halfx" : ""
    let tag = "q8_mulmm_t{hx} mt{mt} m={m} k={kdim} n={ndim}"
    var err : string
    var pso = (mt == 128
        ? (halfx
            ? pipeline_from_source(dev, MetalQ8MulMmTH128_metal_q8_mulmm_t_msl, MetalQ8MulMmTH128_metal_q8_mulmm_t_msl_entry, MetalQ8MulMmTH128_metal_q8_mulmm_t_msl_fastmath, err)
            : pipeline_from_source(dev, MetalQ8MulMmT128_metal_q8_mulmm_t_msl, MetalQ8MulMmT128_metal_q8_mulmm_t_msl_entry, MetalQ8MulMmT128_metal_q8_mulmm_t_msl_fastmath, err))
        : (halfx
            ? pipeline_from_source(dev, MetalQ8MulMmTH_metal_q8_mulmm_t_msl, MetalQ8MulMmTH_metal_q8_mulmm_t_msl_entry, MetalQ8MulMmTH_metal_q8_mulmm_t_msl_fastmath, err)
            : pipeline_from_source(dev, MetalQ8MulMmT_metal_q8_mulmm_t_msl, MetalQ8MulMmT_metal_q8_mulmm_t_msl_entry, MetalQ8MulMmT_metal_q8_mulmm_t_msl_fastmath, err)))
    if (pso == null && !metal4_tensor_available(dev)) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor toolchain on this box - arm skipped\n")
        return
    }
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nb = kdim / 32
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(ndim, nb, blob, wdeq)
    var xf : array<float>
    q8_x_fill(m * kdim, xf)
    var xh : array<float16>
    if (halfx) {
        xh |> resize(m * kdim)
        for (i in range(m * kdim)) {
            xh[i] = float16(xf[i])   // the fixture is f16-exact, so the oracle dot is unchanged
        }
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
    var bw = buf_upload(dev, blob)
    var bx = halfx ? buf_upload(dev, xh) : buf_upload(dev, xf)
    var by = buf_fill(dev, m * ndim, -1000.0)
    var bk = buf_u32(dev, uint(kdim))
    var bn = buf_u32(dev, uint(ndim))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, mt == 128
            ? (halfx ? MetalQ8MulMmTH128_metal_q8_mulmm_t_msl_tgmem : MetalQ8MulMmT128_metal_q8_mulmm_t_msl_tgmem)
            : (halfx ? MetalQ8MulMmTH_metal_q8_mulmm_t_msl_tgmem : MetalQ8MulMmT_metal_q8_mulmm_t_msl_tgmem), 0)
        metal_set_buffer(enc, bw, 0ul, 0)
        metal_set_buffer(enc, bw, 0ul, 1)
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_buffer(enc, bk, 0ul, 4)
        metal_set_buffer(enc, bn, 0ul, 5)
        metal_dispatch_threadgroups(enc, uint3(uint(m / mt), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
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

// the dev-W pair: the q8 blob dequantized to a device f16 panel (one thread per 34B block),
// then the all-device half x half mul_mm reading it - the production >= 256-row prefill
// form. Both dispatches chain in ONE encoder (hazard tracking orders the panel write/read).
def private q8_devw_gate(t : T?; dev, queue; m, kdim, ndim : int) {
    let tag = "q8_devw m={m} k={kdim} n={ndim}"
    var err : string
    var dq_pso = pipeline_from_source(dev, metal_dequant_q8h_msl, metal_dequant_q8h_msl_entry, metal_dequant_q8h_msl_fastmath, err)
    var mm_pso = pipeline_from_source(dev, metal_half_mulmm_th_msl, metal_half_mulmm_th_msl_entry, metal_half_mulmm_th_msl_fastmath, err)
    if (mm_pso == null && !metal4_tensor_available(dev)) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor toolchain on this box - arm skipped\n")
        return
    }
    t |> success(dq_pso != null && mm_pso != null, "{tag}: pipeline: {err}")
    return if (dq_pso == null || mm_pso == null)
    let nb = kdim / 32
    let nblk = ndim * nb
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(ndim, nb, blob, wdeq)
    var xf : array<float>
    q8_x_fill(m * kdim, xf)
    var xh : array<float16>
    xh |> resize(m * kdim)
    for (i in range(m * kdim)) {
        xh[i] = float16(xf[i])   // the fixture is f16-exact, so the oracle dot is unchanged
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
    var bw = buf_upload(dev, blob)
    var bx = buf_upload(dev, xh)
    var bwh = buf_fill(dev, ndim * kdim, float16(-100.0))   // poison fill: an unwritten panel block reds the compare
    var by = buf_fill(dev, m * ndim, -1000.0)
    var bk = buf_u32(dev, uint(kdim))
    var bn = buf_u32(dev, uint(ndim))
    var dka = CvtArgs(total = uint(nblk))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, dq_pso)
        metal_set_buffer(enc, bw, 0ul, 0)
        metal_set_buffer(enc, bw, 0ul, 1)
        metal_set_buffer(enc, bwh, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(dka)), uint64(typeinfo sizeof(dka)), 3)
        metal_dispatch_threadgroups(enc, uint3(uint((nblk + 255) / 256), 1u, 1u), uint3(256u, 1u, 1u))
        metal_set_pipeline(enc, mm_pso)
        metal_set_buffer(enc, bwh, 0ul, 0)
        metal_set_buffer(enc, bx, 0ul, 1)
        metal_set_buffer(enc, by, 0ul, 2)
        metal_set_buffer(enc, bk, 0ul, 3)
        metal_set_buffer(enc, bn, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-3, tag), 0)
    }
    // the N-TILED chain (the production over-knee form): two half-N tiles, each dequant at
    // the blob column offset + GEMM into y's column slice - same offset math as pf_devw_panel
    if (ndim % 128 == 0) {
        let td = ndim / 2
        let nbr = kdim / 32
        var by_t = buf_fill(dev, m * ndim, -500.0)
        var bwh_t = buf_fill(dev, td * kdim, float16(-100.0))
        var bwh_t2 = buf_fill(dev, td * kdim, float16(-100.0))
        var tka = CvtArgs(total = uint(td * nbr))
        let rant = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
            for (ti in range(2)) {
                let tbase = ti * td
                var bwh_ti = ti == 0 ? bwh_t : bwh_t2
                metal_set_pipeline(enc, dq_pso)
                metal_set_buffer(enc, bw, uint64(tbase * nbr * 34), 0)
                metal_set_buffer(enc, bw, uint64(tbase * nbr * 34), 1)
                metal_set_buffer(enc, bwh_ti, 0ul, 2)
                metal_set_bytes(enc, unsafe(addr<void?>(tka)), uint64(typeinfo sizeof(tka)), 3)
                metal_dispatch_threadgroups(enc, uint3(uint((td * nbr + 255) / 256), 1u, 1u), uint3(256u, 1u, 1u))
                metal_set_pipeline(enc, mm_pso)
                metal_set_buffer(enc, bwh_ti, 0ul, 0)
                metal_set_buffer(enc, bx, 0ul, 1)
                metal_set_buffer(enc, by_t, uint64(tbase * 4), 2)
                metal_set_buffer(enc, bk, 0ul, 3)
                metal_set_buffer(enc, bn, 0ul, 4)
                metal_dispatch_threadgroups(enc, uint3(uint(m / 32), uint(td / 64), 1u), uint3(128u, 1u, 1u))
            }
        }
        t |> success(rant, "{tag}: tiled encode: {err}")
        if (rant) {
            t |> equal(buf_mismatch_env(by_t, want, env, 2e-3, "{tag} tiled"), 0)
        }
        metal_release(by_t)
        metal_release(bwh_t)
        metal_release(bwh_t2)
    }
    // negative control: the GEMM alone over a poison-only panel (dequant skipped) must MISS
    // the oracle - proves the chained compare above actually rode the dequant output
    var bwh_p = buf_fill(dev, ndim * kdim, float16(-100.0))
    var by_p = buf_fill(dev, m * ndim, -1000.0)
    let ranp = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, mm_pso)
        metal_set_buffer(enc, bwh_p, 0ul, 0)
        metal_set_buffer(enc, bx, 0ul, 1)
        metal_set_buffer(enc, by_p, 0ul, 2)
        metal_set_buffer(enc, bk, 0ul, 3)
        metal_set_buffer(enc, bn, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ranp, "{tag}: control encode: {err}")
    if (ranp) {
        // quiet count (no eyeball dump - this red is the expectation, not a failure)
        var badp = 0
        unsafe {
            let p = reinterpret<float const?>(metal_buffer_contents(by_p))
            for (k in range(m * ndim)) {
                badp += abs(p[k] - want[k]) <= max(2e-3 * env[k], 1e-3) ? 0 : 1
            }
        }
        t |> success(badp > 0, "{tag}: poison-panel control reds the bar ({badp} over)")
    }
    metal_release(bwh_p)
    metal_release(by_p)
    metal_release(bw)
    metal_release(bx)
    metal_release(bwh)
    metal_release(by)
    metal_release(bk)
    metal_release(bn)
    metal_release(dq_pso)
    metal_release(mm_pso)
    delete blob
    delete xf
    delete xh
    delete want
    delete env
    delete wdeq
}

// the kq dev-W chains: the format's superblock dequant into a device f16 panel, then the
// SAME all-device GEMM as q8's - per-format truth via kq_row_ref, plus the shared
// poison-panel negative control (dequant skipped must miss the oracle)
def private kq_devw_gate(t : T?; dev, queue; fmt : int; m, kdim, ndim : int) {
    let tag = "kq_devw_k{fmt} m={m} k={kdim} n={ndim}"
    var err : string
    let dsrc = fmt == 4 ? MetalDequantK4H_metal_dequant_k45_msl : (fmt == 5 ? MetalDequantK5H_metal_dequant_k45_msl : metal_dequant_k6h_msl)
    let dentry = fmt == 4 ? MetalDequantK4H_metal_dequant_k45_msl_entry : (fmt == 5 ? MetalDequantK5H_metal_dequant_k45_msl_entry : metal_dequant_k6h_msl_entry)
    let dfm = fmt == 4 ? MetalDequantK4H_metal_dequant_k45_msl_fastmath : (fmt == 5 ? MetalDequantK5H_metal_dequant_k45_msl_fastmath : metal_dequant_k6h_msl_fastmath)
    var dq_pso = pipeline_from_source(dev, dsrc, dentry, dfm, err)
    var mm_pso = pipeline_from_source(dev, metal_half_mulmm_th_msl, metal_half_mulmm_th_msl_entry, metal_half_mulmm_th_msl_fastmath, err)
    if (mm_pso == null && !metal4_tensor_available(dev)) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor toolchain on this box - arm skipped\n")
        return
    }
    t |> success(dq_pso != null && mm_pso != null, "{tag}: pipeline: {err}")
    return if (dq_pso == null || mm_pso == null)
    let nb = kdim / 256
    let nsb = ndim * nb
    let nblk = ndim * (kdim / 32)
    var kq : array<uint8>
    var ks : array<uint8>
    kq_fill_planes(fmt, nsb, kq, ks)
    var xf : array<float>
    xf |> resize(m * kdim)
    for (i in range(m * kdim)) {
        xf[i] = float(float16(0.07 * float(i % 39) - 1.3))   // f16 round-trip: the oracle dot stays exact
    }
    var xh : array<float16>
    xh |> resize(m * kdim)
    for (i in range(m * kdim)) {
        xh[i] = float16(xf[i])
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
    var bx = buf_upload(dev, xh)
    var bwh = buf_fill(dev, ndim * kdim, float16(-100.0))   // poison fill: an unwritten panel block reds the compare
    var by = buf_fill(dev, m * ndim, -1000.0)
    var bk = buf_u32(dev, uint(kdim))
    var bn = buf_u32(dev, uint(ndim))
    var dka = CvtArgs(total = uint(nblk))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, dq_pso)
        metal_set_buffer(enc, bks, fmt == 6 ? uint64(nsb * 16) : 0ul, 0)   // k6: the f16 d tail
        metal_set_buffer(enc, bks, 0ul, 1)
        metal_set_buffer(enc, bkq, 0ul, 2)
        metal_set_buffer(enc, bwh, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr<void?>(dka)), uint64(typeinfo sizeof(dka)), 4)
        metal_dispatch_threadgroups(enc, uint3(uint((nblk + 255) / 256), 1u, 1u), uint3(256u, 1u, 1u))
        metal_set_pipeline(enc, mm_pso)
        metal_set_buffer(enc, bwh, 0ul, 0)
        metal_set_buffer(enc, bx, 0ul, 1)
        metal_set_buffer(enc, by, 0ul, 2)
        metal_set_buffer(enc, bk, 0ul, 3)
        metal_set_buffer(enc, bn, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, 2e-4, tag), 0)
    }
    // the N-TILED chain: per-format tile plane offsets (16B scales, 128/160/192B quant
    // superblocks, the k6 2B d tail) - same math as pf_devw_panel_kq
    if (ndim % 128 == 0) {
        let td = ndim / 2
        let qstride = fmt == 4 ? 128 : (fmt == 5 ? 160 : 192)
        var by_t = buf_fill(dev, m * ndim, -500.0)
        var bwh_t = buf_fill(dev, td * kdim, float16(-100.0))
        var bwh_t2 = buf_fill(dev, td * kdim, float16(-100.0))
        var tka = CvtArgs(total = uint(td * (kdim / 32)))
        let rant = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
            for (ti in range(2)) {
                let tbase = ti * td
                var bwh_ti = ti == 0 ? bwh_t : bwh_t2
                let so = uint64(tbase * nb * 16)
                metal_set_pipeline(enc, dq_pso)
                metal_set_buffer(enc, bks, fmt == 6 ? uint64(nsb * 16 + tbase * nb * 2) : so, 0)
                metal_set_buffer(enc, bks, so, 1)
                metal_set_buffer(enc, bkq, uint64(tbase * nb * qstride), 2)
                metal_set_buffer(enc, bwh_ti, 0ul, 3)
                metal_set_bytes(enc, unsafe(addr<void?>(tka)), uint64(typeinfo sizeof(tka)), 4)
                metal_dispatch_threadgroups(enc, uint3(uint((td * (kdim / 32) + 255) / 256), 1u, 1u), uint3(256u, 1u, 1u))
                metal_set_pipeline(enc, mm_pso)
                metal_set_buffer(enc, bwh_ti, 0ul, 0)
                metal_set_buffer(enc, bx, 0ul, 1)
                metal_set_buffer(enc, by_t, uint64(tbase * 4), 2)
                metal_set_buffer(enc, bk, 0ul, 3)
                metal_set_buffer(enc, bn, 0ul, 4)
                metal_dispatch_threadgroups(enc, uint3(uint(m / 32), uint(td / 64), 1u), uint3(128u, 1u, 1u))
            }
        }
        t |> success(rant, "{tag}: tiled encode: {err}")
        if (rant) {
            t |> equal(buf_mismatch_env(by_t, want, env, 2e-4, "{tag} tiled"), 0)
        }
        metal_release(by_t)
        metal_release(bwh_t)
        metal_release(bwh_t2)
    }
    // negative control: the GEMM alone over a poison-only panel must MISS the oracle
    var bwh_p = buf_fill(dev, ndim * kdim, float16(-100.0))
    var by_p = buf_fill(dev, m * ndim, -1000.0)
    let ranp = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, mm_pso)
        metal_set_buffer(enc, bwh_p, 0ul, 0)
        metal_set_buffer(enc, bx, 0ul, 1)
        metal_set_buffer(enc, by_p, 0ul, 2)
        metal_set_buffer(enc, bk, 0ul, 3)
        metal_set_buffer(enc, bn, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ranp, "{tag}: control encode: {err}")
    if (ranp) {
        var badp = 0
        unsafe {
            let p = reinterpret<float const?>(metal_buffer_contents(by_p))
            for (k in range(m * ndim)) {
                badp += abs(p[k] - want[k]) <= max(2e-4 * env[k], 1e-3) ? 0 : 1
            }
        }
        t |> success(badp > 0, "{tag}: poison-panel control reds the bar ({badp} over)")
    }
    metal_release(bwh_p)
    metal_release(by_p)
    metal_release(bks)
    metal_release(bkq)
    metal_release(bx)
    metal_release(bwh)
    metal_release(by)
    metal_release(bk)
    metal_release(bn)
    metal_release(dq_pso)
    metal_release(mm_pso)
    delete kq
    delete ks
    delete xf
    delete xh
    delete want
    delete env
    delete wrow
}

// the native-bf16 A-stage pair: W is bf16 halfwords (top 16 bits of f32), row-major [ndim x kdim]
// The dense f32-weight tower stamp. exact=true builds pow2/small-int fixtures every staged
// value survives f32->f16 exactly (the compare then measures tiling/indexing alone, 2e-3);
// exact=false lets the f16 stage round on BOTH operands (double-RNE model ~9.8e-4*env, bar
// 4e-3). m < mp exercises the pad rows: X pad rows are zero, so their outputs pin at 0.
def private f32_mulmm_gate(t : T?; dev, queue; exact : bool; m, mp, kdim, ndim : int) {
    let tag = "f32_mulmm{exact ? " exact" : ""} m={m}/{mp} k={kdim} n={ndim}"
    var err : string
    var pso = pipeline_from_source(dev, metal_f32_mulmm_msl, metal_f32_mulmm_msl_entry, metal_f32_mulmm_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var w : array<float>
    w |> reserve(ndim * kdim)   // the gemma4uv shapes pass the unreserved-resize guard
    w |> resize(ndim * kdim)
    var xf : array<float>
    xf |> reserve(mp * kdim)
    xf |> resize(mp * kdim)
    if (exact) {
        let lut = fixed_array(0.25, 0.5, 1.0, 2.0, -0.25, -0.5, -1.0, -2.0)
        for (i in range(ndim * kdim)) {
            w[i] = lut[(i * 5 + 1) % 8] * float((i * 3) % 5)
        }
        for (i in range(m * kdim)) {
            xf[i] = lut[(i * 7 + 3) % 8]
        }
    } else {
        for (i in range(ndim * kdim)) {
            w[i] = 0.13 * float((i * 3) % 13) - 0.8
        }
        for (i in range(m * kdim)) {
            xf[i] = 0.11 * float(i % 37) - 1.9
        }
    }
    var want : array<float>
    var env : array<float>
    want |> reserve(mp * ndim)
    want |> resize(mp * ndim)
    env |> reserve(mp * ndim)
    env |> resize(mp * ndim)
    for (j in range(ndim)) {
        for (r in range(m)) {
            var acc = 0.0lf
            var ev = 0.0lf
            for (c in range(kdim)) {
                let p = double(w[j * kdim + c]) * double(xf[r * kdim + c])
                acc += p
                ev += abs(p)
            }
            want[r * ndim + j] = float(acc)
            env[r * ndim + j] = float(ev)
        }
    }
    var bw = buf_upload(dev, w)
    var bx = buf_upload(dev, xf)
    var by = buf_fill(dev, mp * ndim, -1000.0)
    var bk = buf_u32(dev, uint(kdim))
    var bn = buf_u32(dev, uint(ndim))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_f32_mulmm_msl_tgmem, 0)
        metal_set_buffer(enc, bw, 0ul, 0)
        metal_set_buffer(enc, bx, 0ul, 1)
        metal_set_buffer(enc, by, 0ul, 2)
        metal_set_buffer(enc, bk, 0ul, 3)
        metal_set_buffer(enc, bn, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint(mp / 32), uint(ndim / 64), 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(by, want, env, exact ? 2e-3 : 4e-3, tag), 0)
    }
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(bk)
    metal_release(bn)
    metal_release(pso)
    delete w
    delete xf
    delete want
    delete env
}

def private bf16_mulmm_gate(t : T?; dev, queue; tensor : bool; m, kdim, ndim : int; halfx : bool = false) {
    let tag = "bf16_mulmm{tensor ? "_t" : ""}{halfx ? " halfx" : ""} m={m} k={kdim} n={ndim}"
    var err : string
    var pso = (tensor
        ? (halfx
            ? pipeline_from_source(dev, MetalBf16MulMmTH_metal_bf16_mulmm_t_msl, MetalBf16MulMmTH_metal_bf16_mulmm_t_msl_entry, MetalBf16MulMmTH_metal_bf16_mulmm_t_msl_fastmath, err)
            : pipeline_from_source(dev, MetalBf16MulMmT_metal_bf16_mulmm_t_msl, MetalBf16MulMmT_metal_bf16_mulmm_t_msl_entry, MetalBf16MulMmT_metal_bf16_mulmm_t_msl_fastmath, err))
        : pipeline_from_source(dev, metal_bf16_mulmm_msl, metal_bf16_mulmm_msl_entry, metal_bf16_mulmm_msl_fastmath, err))
    if (tensor && pso == null && !metal4_tensor_available(dev)) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor toolchain on this box - arm skipped\n")
        return
    }
    // the simdgroup BASE asserts — production compiles it unconditionally, and this arm is its
    // only coverage; a broken bf16 A-stage must red here, not skip
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
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
        // halfx: round-trip through f16 so the oracle dot below stays the arm's exact reference
        xf[i] = halfx ? float(float16(0.11 * float(i % 37) - 1.9)) : 0.11 * float(i % 37) - 1.9
    }
    var xh : array<float16>
    if (halfx) {
        xh |> resize(m * kdim)
        for (i in range(m * kdim)) {
            xh[i] = float16(xf[i])
        }
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
    var bx = halfx ? buf_upload(dev, xh) : buf_upload(dev, xf)
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
    delete xh
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
        ? pipeline_from_source(dev, MetalQ8GemmBT_metal_q8_gemm_b_t_msl, MetalQ8GemmBT_metal_q8_gemm_b_t_msl_entry, MetalQ8GemmBT_metal_q8_gemm_b_t_msl_fastmath, err)
        : pipeline_from_source(dev, MetalQ8GemmB_metal_q8_gemm_b_msl, MetalQ8GemmB_metal_q8_gemm_b_msl_entry, MetalQ8GemmB_metal_q8_gemm_b_msl_fastmath, err))
    if (tensor && pso == null && !metal4_tensor_available(dev)) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor toolchain on this box - arm skipped\n")
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
        metal_set_threadgroup_memory_length(enc, tensor ? MetalQ8GemmBT_metal_q8_gemm_b_t_msl_tgmem : MetalQ8GemmB_metal_q8_gemm_b_msl_tgmem, 0)
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
        ? pipeline_from_source(dev, MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl, MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl_entry, MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl_fastmath, err)
        : pipeline_from_source(dev, MetalQ8GemmBSk_metal_q8_gemm_b_msl, MetalQ8GemmBSk_metal_q8_gemm_b_msl_entry, MetalQ8GemmBSk_metal_q8_gemm_b_msl_fastmath, err))
    if (tensor && pso == null && !metal4_tensor_available(dev)) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor toolchain on this box - arm skipped\n")
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
        metal_set_threadgroup_memory_length(enc, tensor ? MetalQ8GemmBSkT_metal_q8_gemm_b_t_msl_tgmem : MetalQ8GemmBSk_metal_q8_gemm_b_msl_tgmem, 0)
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
        ? pipeline_from_source(dev, MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl, MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl_entry, MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl_fastmath, err)
        : pipeline_from_source(dev, metal_q8_gemm64_b_msl, metal_q8_gemm64_b_msl_entry, metal_q8_gemm64_b_msl_fastmath, err))
    if (tensor && pso == null && !metal4_tensor_available(dev)) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor toolchain on this box - arm skipped\n")
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
        metal_set_threadgroup_memory_length(enc, tensor ? MetalQ8Gemm64BT_metal_q8_gemm_b_t_msl_tgmem : metal_q8_gemm64_b_msl_tgmem, 0)
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
def private moe_mulmm_q8_gate(t : T?; dev, queue; tensor : bool; kdim, ndim : int; counts : array<int>; gather : bool; halfx : bool = false) {
    let tag = "moe_mulmm_q8{tensor ? "t" : ""}{halfx ? " halfx" : ""}{gather ? " gather" : ""} k={kdim} n={ndim}"
    var err : string
    var pso = (tensor
        ? (halfx
            ? pipeline_from_source(dev, MetalMoeMulMmQ8TH_metal_moe_mulmm_q8_t_msl, MetalMoeMulMmQ8TH_metal_moe_mulmm_q8_t_msl_entry, MetalMoeMulMmQ8TH_metal_moe_mulmm_q8_t_msl_fastmath, err)
            : pipeline_from_source(dev, MetalMoeMulMmQ8T_metal_moe_mulmm_q8_t_msl, MetalMoeMulMmQ8T_metal_moe_mulmm_q8_t_msl_entry, MetalMoeMulMmQ8T_metal_moe_mulmm_q8_t_msl_fastmath, err))
        : pipeline_from_source(dev, metal_moe_mulmm_q8_msl, metal_moe_mulmm_q8_msl_entry, metal_moe_mulmm_q8_msl_fastmath, err))
    if (tensor && pso == null && !metal4_tensor_available(dev)) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor toolchain on this box - arm skipped\n")
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
    var xh : array<float16>
    if (halfx) {
        // round-trip through f16 so moe_mm_truth stays the arm's exact reference
        xh |> resize(length(xf))
        for (xv, hv in xf, xh) {
            hv = float16(xv)
            xv = float(hv)
        }
    }
    var wb : array<float>
    var want : array<float>
    var env : array<float>
    moe_mm_truth(fx, wdeq, xf, wb, kdim, ndim, false, want, env)
    var ka = MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim), eplane = uint(ndim * nkb),
        nk = uint(nk), gather = gather ? 1u : 0u)
    var bw = buf_upload(dev, blob)
    var bx = halfx ? buf_upload(dev, xh) : buf_upload(dev, xf)
    var by = buf_fill(dev, fx.pad * ndim, -1000.0)
    var bcnt = buf_upload(dev, fx.cnt)
    var bbase = buf_upload(dev, fx.basep)
    var bbkt = gather ? buf_upload(dev, fx.bkt) : buf_u32(dev, 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)   // half-scale view
        metal_set_buffer(enc, bw, 0ul, 1)   // byte view of the same blob
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        if (tensor) {   // the tensor twin keeps its compact layout (contiguous-only, no bkt)
            metal_set_threadgroup_memory_length(enc, halfx ? MetalMoeMulMmQ8TH_metal_moe_mulmm_q8_t_msl_tgmem : MetalMoeMulMmQ8T_metal_moe_mulmm_q8_t_msl_tgmem, 0)
            metal_set_buffer(enc, bx, 0ul, 2)
            metal_set_buffer(enc, by, 0ul, 3)
            metal_set_buffer(enc, bcnt, 0ul, 6)
            metal_set_buffer(enc, bbase, 0ul, 7)
        } else {        // the joined family rider binds at the MetalMoeMulMmBase numbers
            metal_set_threadgroup_memory_length(enc, metal_moe_mulmm_q8_msl_tgmem, 0)
            metal_set_buffer(enc, bx, 0ul, 3)
            metal_set_buffer(enc, by, 0ul, 4)
            metal_set_buffer(enc, bcnt, 0ul, 7)
            metal_set_buffer(enc, bbase, 0ul, 8)
            metal_set_buffer(enc, bbkt, 0ul, 9)
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
    delete xh
    delete wb
    delete want
    delete env
}

// The gathered MXFP4 mul_mm (MetalMoeMulMmMx4) and its tensor twin: nibble plane + e8m0 scale
// plane + host-built vtab, per-expert bias folded when hasb (accumulator seed in the base,
// rank-1 chunk in the twin). Truth replicates the kernels' exact e8m0 decode.
def private moe_mulmm_mx4_gate(t : T?; dev, queue; tensor : bool; kdim, ndim : int; counts : array<int>; gather, hasb : bool; halfx : bool = false) {
    let tag = "moe_mulmm_mx4{tensor ? "t" : ""}{halfx ? " halfx" : ""}{gather ? " gather" : ""}{hasb ? " bias" : ""} k={kdim} n={ndim}"
    var err : string
    var pso = (tensor
        ? (halfx
            ? pipeline_from_source(dev, MetalMoeMulMmMx4TH_metal_moe_mulmm_mx4_t_msl, MetalMoeMulMmMx4TH_metal_moe_mulmm_mx4_t_msl_entry, MetalMoeMulMmMx4TH_metal_moe_mulmm_mx4_t_msl_fastmath, err)
            : pipeline_from_source(dev, MetalMoeMulMmMx4T_metal_moe_mulmm_mx4_t_msl, MetalMoeMulMmMx4T_metal_moe_mulmm_mx4_t_msl_entry, MetalMoeMulMmMx4T_metal_moe_mulmm_mx4_t_msl_fastmath, err))
        : pipeline_from_source(dev, metal_moe_mulmm_mx4_msl, metal_moe_mulmm_mx4_msl_entry, metal_moe_mulmm_mx4_msl_fastmath, err))
    if (tensor && pso == null && !metal4_tensor_available(dev)) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor toolchain on this box - arm skipped\n")
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
    var xh : array<float16>
    if (halfx) {
        // round-trip through f16 so moe_mm_truth stays the arm's exact reference
        xh |> resize(length(xf))
        for (xv, hv in xf, xh) {
            hv = float16(xv)
            xv = float(hv)
        }
    }
    var want : array<float>
    var env : array<float>
    moe_mm_truth(fx, wdeq, xf, wb, kdim, ndim, hasb, want, env)
    var ka = MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim), eplane = uint(ndim * nkb),
        nk = uint(nk), gather = gather ? 1u : 0u, hasb = hasb ? 1u : 0u)
    var bnq = buf_upload(dev, nplane)
    var bee = buf_upload(dev, eplane8)
    var bvt = buf_upload(dev, vtab)
    var bwb = buf_upload(dev, wb)
    var bx = halfx ? buf_upload(dev, xh) : buf_upload(dev, xf)
    var by = buf_fill(dev, fx.pad * ndim, -1000.0)
    var bcnt = buf_upload(dev, fx.cnt)
    var bbase = buf_upload(dev, fx.basep)
    var bbkt = gather ? buf_upload(dev, fx.bkt) : buf_u32(dev, 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, tensor ? (halfx ? MetalMoeMulMmMx4TH_metal_moe_mulmm_mx4_t_msl_tgmem : MetalMoeMulMmMx4T_metal_moe_mulmm_mx4_t_msl_tgmem) : metal_moe_mulmm_mx4_msl_tgmem, 0)
        metal_set_buffer(enc, bnq, 0ul, 0)
        metal_set_buffer(enc, bee, 0ul, 1)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        if (tensor) {   // the tensor twin keeps its compact layout (contiguous-only, no bkt)
            metal_set_buffer(enc, bx, 0ul, 2)
            metal_set_buffer(enc, by, 0ul, 3)
            metal_set_buffer(enc, bcnt, 0ul, 6)
            metal_set_buffer(enc, bbase, 0ul, 7)
        } else {        // the joined family rider binds at the MetalMoeMulMmBase numbers
            metal_set_buffer(enc, bx, 0ul, 3)
            metal_set_buffer(enc, by, 0ul, 4)
            metal_set_buffer(enc, bcnt, 0ul, 7)
            metal_set_buffer(enc, bbase, 0ul, 8)
            metal_set_buffer(enc, bbkt, 0ul, 9)
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
    delete xh
    delete want
    delete env
}

// ===== the prefill attention GEMM pair + Metal-4 tensor twins =====

// MetalAttnQKMm[T]: raw scores att[h x qrows x np32] = scale * q · k over surviving 32x64
// blocks — blocks fully above the causal diagonal (or fully outside the sliding window) exit
// without writing, which the sentinel whole-slab compare asserts. QK arms keep npos == np32:
// the tensor twin stages K pad rows RAW (no zero guard — the base has one), so pads must not
// exist for the pair to agree.
def private attn_qk_mm_gate(t : T?; dev, queue; tensor : bool; heads, hs, kv_mul, mp, np32, qoff, window : int; uend : int = 0) {
    let tag = "attn_qk_mm{tensor ? "t" : ""} h={heads} hs={hs} m={mp} n={np32} qoff={qoff} w={window} u={uend}"
    var err : string
    var pso = (tensor
        ? pipeline_from_source(dev, metal_attn_qk_mm_t_msl, metal_attn_qk_mm_t_msl_entry, metal_attn_qk_mm_t_msl_fastmath, err)
        : pipeline_from_source(dev, metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath, err))
    if (tensor && pso == null && !metal4_tensor_available(dev)) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor toolchain on this box - arm skipped\n")
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
                // fused span: tail rows above uend stay causal, so the skip floor is
                // max(uend, the block's causal reach) — mirrors the kernel's gate
                let skip = ((uend > 0 ? nBase >= max(uend, qoff + mBase + 32) : nBase > qoff + mBase + 31)
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
        window = uint(window), uend = uint(uend))
    var bq = buf_upload(dev, qv)
    var bk = buf_upload(dev, kv)
    var batt = buf_fill(dev, heads * mp * np32, float16(-1000.0))   // f16 slab; the sentinel is f16-exact
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
        t |> equal(buf_mismatch_env_h(batt, want, env, 2e-3, tag), 0)
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

// MetalAttnAVMm[T]: xb[m x qd] = P · V where P = pf_p_weight(raw f16 score, rowstat) applied
// at staging — the gate feeds RAW scores + CPU-minted stats and oracles the same transform.
// The tensor twin zero-guards V rows >= npos — the npos < np32 arm fills the pad rows with
// garbage to prove the guard is live (their P weights are zero past each row's live length).
def private attn_av_mm_gate(t : T?; dev, queue; tensor : bool; heads, hs, kv_mul, mp, np32, npos, qoff : int; uend : int = 0; padfree : bool = false) {
    let tag = "attn_av_mm{tensor ? "t" : ""}{padfree ? " padfree" : ""} h={heads} hs={hs} m={mp} n={np32} npos={npos} u={uend}"
    var err : string
    var pso = (tensor
        ? (padfree
            ? pipeline_from_source(dev, MetalAttnAVMmTP_metal_attn_av_mm_t_msl, MetalAttnAVMmTP_metal_attn_av_mm_t_msl_entry, MetalAttnAVMmTP_metal_attn_av_mm_t_msl_fastmath, err)
            : pipeline_from_source(dev, MetalAttnAVMmT_metal_attn_av_mm_t_msl, MetalAttnAVMmT_metal_attn_av_mm_t_msl_entry, MetalAttnAVMmT_metal_attn_av_mm_t_msl_fastmath, err))
        : (padfree
            ? pipeline_from_source(dev, MetalAttnAVMmP_metal_attn_av_mm_msl, MetalAttnAVMmP_metal_attn_av_mm_msl_entry, MetalAttnAVMmP_metal_attn_av_mm_msl_fastmath, err)
            : pipeline_from_source(dev, MetalAttnAVMm_metal_attn_av_mm_msl, MetalAttnAVMm_metal_attn_av_mm_msl_entry, MetalAttnAVMm_metal_attn_av_mm_msl_fastmath, err)))
    if (tensor && pso == null && !metal4_tensor_available(dev)) {
        to_log(LOG_INFO, "{tag}: no Metal-4 tensor toolchain on this box - arm skipped\n")
        return
    }
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let qd = heads * hs
    let kvd = (heads / kv_mul) * hs
    var attf : array<float16>
    attf |> resize(heads * mp * np32)
    for (i in range(heads * mp * np32)) {
        attf[i] = float16(float((i * 3) % 13 - 6) * 0.25)   // raw scores, every slot populated
    }
    var stats : array<float>
    stats |> resize(heads * mp * 2)
    for (h in range(heads)) {
        for (m in range(mp)) {
            let qpos = qoff + m
            let cnt = uend > 0 && qpos < uend ? uend : qpos + 1
            var rm = -1.0e30
            for (pos in range(cnt)) {
                rm = max(rm, float(attf[(h * mp + m) * np32 + pos]))
            }
            var sum = 0.0
            for (pos in range(cnt)) {
                sum += exp(float(attf[(h * mp + m) * np32 + pos]) - rm)
            }
            stats[(h * mp + m) * 2] = rm
            stats[(h * mp + m) * 2 + 1] = 1.0 / sum
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
            let qpos = qoff + m
            for (col in range(hs)) {
                var acc = 0.0lf
                var ev = 0.0lf
                for (pos in range(npos)) {
                    let pw = pf_p_weight(float(attf[(h * mp + m) * np32 + pos]), uint(pos), uint(qpos),
                        0u, 0.0, uint(uend), 0u, stats[(h * mp + m) * 2], stats[(h * mp + m) * 2 + 1])
                    let p = double(pw) * double(vv[pos * kvd + kvhoff + col])
                    acc += p
                    ev += abs(p)
                }
                want[m * qd + h * hs + col] = float(acc)
                env[m * qd + h * hs + col] = float(ev)
            }
        }
    }
    var ka = AttnArgs(qd = uint(qd), kv_dim = uint(kvd), head_size = uint(hs), kv_mul = uint(kv_mul),
        npos = uint(npos), np32 = uint(np32), scale = 1.0, qoff = uint(qoff), qrows = uint(mp),
        uend = uint(uend))
    var batt = buf_upload(dev, attf)
    var bstat = buf_upload(dev, stats)
    var bv = buf_upload(dev, vv)
    var bxb = buf_fill(dev, mp * qd, -1000.0)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, tensor
            ? (padfree ? MetalAttnAVMmTP_metal_attn_av_mm_t_msl_tgmem : MetalAttnAVMmT_metal_attn_av_mm_t_msl_tgmem)
            : (padfree ? MetalAttnAVMmP_metal_attn_av_mm_msl_tgmem : MetalAttnAVMm_metal_attn_av_mm_msl_tgmem), 0)
        metal_set_buffer(enc, batt, 0ul, 0)
        metal_set_buffer(enc, bv, 0ul, 1)
        metal_set_buffer(enc, bxb, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
        metal_set_buffer(enc, bstat, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint(mp / 32), uint(hs / 64), uint(heads)), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_env(bxb, want, env, 2e-3, tag), 0)
    }
    metal_release(batt)
    metal_release(bstat)
    metal_release(bv)
    metal_release(bxb)
    metal_release(pso)
    delete attf
    delete stats
    delete vv
    delete want
    delete env
}

[test]
def test_metal_gemm_kernels(t : T?) {
    t |> run("prefill GEMM corpus (kq mul_mm trio + bf16 + Metal-4 tensor twins) == CPU references") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU kernel checks skipped\n")
                    return
                }
                var queue = metal_new_command_queue(dev)
                // the simdgroup kq trio + the tensor twins (twins feint without Metal-4)
                for (fmt in [4, 5, 6]) {
                    kq_mulmm_gate(t, dev, queue, fmt, false, 64, 512, 64)
                    kq_mulmm_gate(t, dev, queue, fmt, true, 64, 512, 64)
                    kq_mulmm_gate(t, dev, queue, fmt, true, 64, 512, 64, halfx = true)
                }
                kq_mulmm_gate(t, dev, queue, 4, false, 32, 256, 128)   // second shape: 2 col tiles
                q8_mulmm_t_gate(t, dev, queue, 64, 256, 64)
                q8_mulmm_t_gate(t, dev, queue, 64, 256, 64, halfx = true)
                q8_mulmm_t_gate(t, dev, queue, 128, 256, 64, mt = 128)   // tall M-tile stamps
                q8_mulmm_t_gate(t, dev, queue, 256, 256, 64, halfx = true, mt = 128)
                q8_devw_gate(t, dev, queue, 64, 256, 64)
                q8_devw_gate(t, dev, queue, 32, 288, 128)   // non-256-multiple block count arms the dequant tail guard
                for (fmt in [4, 5, 6]) {
                    kq_devw_gate(t, dev, queue, fmt, 64, 512, 64)
                    kq_devw_gate(t, dev, queue, fmt, 32, 512, 128)   // ndim%128: the tiled leg runs
                }
                bf16_mulmm_gate(t, dev, queue, false, 64, 256, 64)
                bf16_mulmm_gate(t, dev, queue, true, 64, 256, 64)
                bf16_mulmm_gate(t, dev, queue, true, 64, 256, 64, halfx = true)

                // ===== the dense f32 tower stamp =====
                f32_mulmm_gate(t, dev, queue, true, 32, 32, 384, 384)      // whisper-tiny qkv/o, exact stage
                f32_mulmm_gate(t, dev, queue, false, 32, 32, 384, 1536)    // tiny fc1, rounded stage
                f32_mulmm_gate(t, dev, queue, false, 32, 32, 1536, 384)    // tiny fc2
                f32_mulmm_gate(t, dev, queue, false, 64, 64, 896, 896)     // qwen3a qkv/o
                f32_mulmm_gate(t, dev, queue, true, 1504, 1504, 1280, 1280)  // whisper-large T=1500 tile count
                f32_mulmm_gate(t, dev, queue, false, 64, 64, 6912, 3840)   // gemma4uv patch GEMM - longest dot
                f32_mulmm_gate(t, dev, queue, false, 100, 128, 3840, 3840) // gemma4uv proj + M-pad rows pinned 0
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
                moe_mulmm_q8_gate(t, dev, queue, true, 128, 64, [8, 33], false, halfx = true)
                moe_mulmm_mx4_gate(t, dev, queue, false, 96, 64, [32, 40, 8], true, true)
                moe_mulmm_mx4_gate(t, dev, queue, false, 128, 64, [8, 33], false, false)
                moe_mulmm_mx4_gate(t, dev, queue, true, 128, 64, [8, 33], false, true)
                moe_mulmm_mx4_gate(t, dev, queue, true, 128, 64, [8, 33], false, false)
                moe_mulmm_mx4_gate(t, dev, queue, true, 128, 64, [8, 33], false, true, halfx = true)
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
                // the PADFREE (clean) stamps: npos == np32 divisible, no pad row in any chunk
                attn_av_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 128, 128, 64, padfree = true)
                attn_av_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 128, 64, padfree = true)
                attn_av_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 64, 64, 0, padfree = true)   // fresh square (the span-fused head shape)
                attn_av_mm_gate(t, dev, queue, false, 4, 64, 2, 32, 96, 96, 64, uend = 96, padfree = true)   // span rows under uend
                attn_av_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 128, 0, uend = 128, padfree = true)
                // the uniform-span (uend) arms: every query row attends [0, uend) — base AND
                // tensor twins (the _t twins' only span coverage; they feint without Metal-4)
                attn_qk_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 128, 0, 0, uend = 128)
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 0, 0, uend = 128)
                attn_qk_mm_gate(t, dev, queue, false, 4, 64, 2, 32, 256, 224, 128, uend = 192)   // span past the window floor
                attn_qk_mm_gate(t, dev, queue, true, 4, 64, 2, 32, 256, 224, 128, uend = 192)
                attn_av_mm_gate(t, dev, queue, false, 4, 64, 2, 64, 128, 96, 0, uend = 96)
                attn_av_mm_gate(t, dev, queue, true, 4, 64, 2, 64, 128, 96, 0, uend = 96)
                metal_release(queue)
            }
            t |> equal(metal_log_leaks_if_any(), 0l)
        } else {
            feint("das_metal is not built on this platform; GEMM kernel checks skipped\n")
        }
    }
}
