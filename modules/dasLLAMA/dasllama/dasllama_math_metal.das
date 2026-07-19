options gen2
options _comment_hygiene = true

module dasllama_math_metal shared public

require dasllama/dasllama_math
require metal/msl_shader
require metal/das_metal_boost

// The Metal GPU prefill-GEMM backend (Phase-5 dasMetal offload experiment): a BATCH-ONLY donor —
// decode GEMV is bandwidth-bound on the same unified memory and stays CPU. Registered at [init]
// with priority -1 so it never wins auto/load-select; it activates only through
// select_batch_backend("metal") (env rail: DASLLAMA_PIN_BATCH_BACKEND). The donor rules of
// apply_batch_override make the full recipe:
//   DASLLAMA_PIN_BACKEND=arm64-sdot   (row-major weights — a repack backend's layout is unreadable here)
//   DASLLAMA_WSCALE_F16=0             (keep the f32 scale plane — the override swaps only the f32 batch slot)
//   DASLLAMA_PIN_BATCH_BACKEND=metal
//
// Two kernels, both 128 threads (4 simdgroups) per threadgroup, picked per shape by the driver
// (bench_metal_gemm_kernels.das is the lab that tuned them): a 32x32-C-block kernel for small
// grids and a 64x64 one (halves both device re-read directions, 16 macs per 8 tile loads) that
// wins ~1.6x on every big shape but starves below ~128 threadgroups. Per 32-k step each kernel
// dequantizes Q8 activation/weight blocks into f16 threadgroup tiles with RUN-STAGING (one
// thread = one 16-element contiguous run = one scale load + four byte4 device loads — the
// staging knockout showed per-element scale reloads were 55% of kernel time), barriers, and
// runs 8x8 simdgroup tiles via the MIXED mac (f32 accumulators, f16 operands). Dequant-to-half
// carries ggml's tolerance envelope: int8 quants are exact in half, the folded scale product
// rounds — pinned runs compare against CPU within relative tolerance, not bit-exact.
//
// The host driver is self-contained: weight regions upload lazily ON FIRST USE, keyed by the
// region's base address (weights are stable owned arrays after load; the GPU copy doubles their
// unified-memory footprint — an experiment cost, recorded in the masterplan). Activations and
// outputs ride pooled shared-memory buffers, padded to 32-row multiples (the kernel has no edge
// masking — pad activations are zeroed, pad output rows are never read back).

class MetalQ8Gemm {
    @ssbo @binding = 0 wq : array<byte4>    // W [d x n] row-major Q8 quants (byte4 view — 4-wide loads)
    @ssbo @binding = 1 ws : array<float>    // W scales [d x n/32]
    @ssbo @binding = 2 xq : array<byte4>    // X [M' x n] Q8 quants (M' = 64-padded ntok)
    @ssbo @binding = 3 xs : array<float>    // X scales [M' x n/32]
    @ssbo @binding = 4 y : array<float>     // Y [M' x d]
    @uniform @binding = 5 kdim : uint       // n (K)
    @uniform @binding = 6 ndim : uint       // d (N)
    @workgroup ta : float16[1024]           // A block: 32 tokens x 32 k, dequantized
    @workgroup tb : float16[1024]           // B block: 32 k x 32 outputs, dequantized + transposed

    [metal_kernel(name="metal_q8_gemm_msl")]
    def metal_q8_gemm {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let qm = (sg / 2u) * 16u            // this simdgroup's 16x16 quadrant of the C block
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let run = lid % 64u                 // 64 A runs on threads 0-63, 64 B runs on 64-127
        let rr = run / 2u                   // row within the 32-row tile
        let c0 = (run % 2u) * 16u           // k offset within the 32-k block
        var kb = 0u
        while (kb * 32u < kdim) {
            let k40 = (kb * 32u + c0) / 4u
            if (lid < 64u) {
                let s = xs[(mBase + rr) * (kdim / 32u) + kb]
                let src = (mBase + rr) * kdim4 + k40
                let dst = rr * 32u + c0
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(xq[src + i4])) * s
                    let o = dst + i4 * 4u
                    ta[o] = float16(v.x)
                    ta[o + 1u] = float16(v.y)
                    ta[o + 2u] = float16(v.z)
                    ta[o + 3u] = float16(v.w)
                    i4++
                }
            } else {
                let s = ws[(nBase + rr) * (kdim / 32u) + kb]
                let src = (nBase + rr) * kdim4 + k40
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(wq[src + i4])) * s
                    let o = c0 + i4 * 4u
                    tb[o * 32u + rr] = float16(v.x)
                    tb[(o + 1u) * 32u + rr] = float16(v.y)
                    tb[(o + 2u) * 32u + rr] = float16(v.z)
                    tb[(o + 3u) * 32u + rr] = float16(v.w)
                    i4++
                }
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 32u + qn, 32u)
                simdgroup_load(b1, tb, k8 * 32u + (qn + 8u), 32u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
    }
}

// 64x64 C block per threadgroup: halves device re-read vs 32x32, 16 macs per 8 tile loads.
// Requires d % 64 == 0 and a big grid; driver falls back to MetalQ8Gemm otherwise.
class MetalQ8Gemm64 {
    @ssbo @binding = 0 wq : array<byte4>    // W [d x n] row-major Q8 quants (byte4 view)
    @ssbo @binding = 1 ws : array<float>    // W scales [d x n/32]
    @ssbo @binding = 2 xq : array<byte4>    // X [M' x n] Q8 quants (M' = 64-padded ntok)
    @ssbo @binding = 3 xs : array<float>    // X scales [M' x n/32]
    @ssbo @binding = 4 y : array<float>     // Y [M' x d]
    @uniform @binding = 5 kdim : uint       // n (K)
    @uniform @binding = 6 ndim : uint       // d (N)
    @workgroup ta : float16[2048]           // A block: 64 tokens x 32 k, dequantized
    @workgroup tb : float16[2048]           // B block: 32 k x 64 outputs, dequantized + transposed

    [metal_kernel(name="metal_q8_gemm64_msl")]
    def metal_q8_gemm64 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 64u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        let qm = (sg / 2u) * 32u            // this simdgroup's 32x32 quadrant of the C block
        let qn = (sg % 2u) * 32u
        var mc : simdgroup_float8x8[16]     // 4x4 tile quadrant
        let kdim4 = kdim / 4u
        let ra = lid / 2u                   // one A run and one B run per thread
        let c0a = (lid % 2u) * 16u
        var kb = 0u
        while (kb * 32u < kdim) {
            {
                let s = xs[(mBase + ra) * (kdim / 32u) + kb]
                let src = (mBase + ra) * kdim4 + (kb * 32u + c0a) / 4u
                let dst = ra * 32u + c0a
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(xq[src + i4])) * s
                    let o = dst + i4 * 4u
                    ta[o] = float16(v.x)
                    ta[o + 1u] = float16(v.y)
                    ta[o + 2u] = float16(v.z)
                    ta[o + 3u] = float16(v.w)
                    i4++
                }
            }
            {
                let sw = ws[(nBase + ra) * (kdim / 32u) + kb]
                let srcw = (nBase + ra) * kdim4 + (kb * 32u + c0a) / 4u
                var j4 = 0u
                while (j4 < 4u) {
                    let v = float4(int4(wq[srcw + j4])) * sw
                    let o = c0a + j4 * 4u
                    tb[o * 64u + ra] = float16(v.x)
                    tb[(o + 1u) * 64u + ra] = float16(v.y)
                    tb[(o + 2u) * 64u + ra] = float16(v.z)
                    tb[(o + 3u) * 64u + ra] = float16(v.w)
                    j4++
                }
            }
            barrier()
            // math ROLLED over matrix arrays + das-pointer tile bumps (hand-unrolled
            // spelling measured max_threads 704 — an occupancy tier down)
            var ma : simdgroup_half8x8[4]
            var mb : simdgroup_half8x8[4]
            var acur = unsafe(addr(ta[qm * 32u]))
            var bcur = unsafe(addr(tb[qn]))
            for [unroll_full] (_k8 in range(4)) {
                for [unroll_full] (i in range(4)) {
                    simdgroup_load(ma[i], acur, uint(i) * 256u, 32u)
                }
                for [unroll_full] (i in range(4)) {
                    simdgroup_load(mb[i], bcur, uint(i) * 8u, 64u)
                }
                for [unroll_full] (i in range(16)) {
                    simdgroup_multiply_accumulate(mc[i], ma[i / 4], mb[i % 4], mc[i])
                }
                acur = unsafe(acur + 8)
                bcur = unsafe(bcur + 512)
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        for [unroll_full] (i in range(16)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

//! Pick the 64x64 kernel when the shape divides evenly and the grid is big enough to fill the
//! GPU (below ~128 threadgroups the 32x32 kernel's occupancy wins — lab-measured on M1 Max).
//! Shared with the resident-prefill driver so both dispatch paths agree.
def public gemm_use64(mp, d : int64) : bool {
    return (d % 64l) == 0l && (mp / 64l) * (d / 64l) >= 128l
}

// ===== host driver =====

var private g_dev : MetalDevice?
var private g_queue : MetalCommandQueue?
var private g_pso : MetalComputePipeline?
var private g_pso64 : MetalComputePipeline?
var private g_failed = false
var private g_wbufs : table<uint64; MetalBuffer?>   // intptr(region base) -> lazily uploaded weights
var private g_sbufs : table<uint64; MetalBuffer?>   // intptr(region base) -> lazily uploaded scales
var private g_pool : MetalBufferPool
var private g_min_ntok = 32l
var private g_dispatches = 0l
var private g_fallbacks = 0l

//! Batches below this token count delegate to the CPU portable batch kernel (GPU submit
//! overhead outweighs the work). Set 1 to force every pinned batch onto the GPU (tests).
def public set_metal_gemm_min_ntok(v : int64) {
    g_min_ntok = v
}

//! (gpu dispatches, cpu fallbacks) since load — diagnostics / tests.
def public metal_gemm_stats() : tuple<dispatches : int64; fallbacks : int64> {
    return (dispatches = g_dispatches, fallbacks = g_fallbacks)
}

//! Release every GPU object the driver holds (weight caches, pools, pipeline, queue, device) —
//! the tests' leak-gate rail. The next pinned dispatch re-initializes lazily.
def public metal_gemm_shutdown {
    for (buf in values(g_wbufs)) {
        if (buf != null) {
            metal_release(buf)
        }
        buf = null
    }
    unsafe {
        delete g_wbufs
    }
    for (buf in values(g_sbufs)) {
        if (buf != null) {
            metal_release(buf)
        }
        buf = null
    }
    unsafe {
        delete g_sbufs
    }
    pool_drain(g_pool)
    if (g_pso != null) {
        metal_release(g_pso)
        g_pso = null
    }
    if (g_pso64 != null) {
        metal_release(g_pso64)
        g_pso64 = null
    }
    if (g_queue != null) {
        metal_release(g_queue)
        g_queue = null
    }
    if (g_dev != null) {
        metal_release(g_dev)
        g_dev = null
    }
    g_failed = false
}

def private metal_gemm_init : bool {
    if (g_pso != null) {
        return true
    }
    if (g_failed) {
        return false
    }
    g_dev = metal_create_system_default_device()
    if (g_dev == null) {
        to_log(LOG_WARNING, "dasLLAMA metal: no Metal device — batch pin falls back to the portable CPU kernel\n")
        g_failed = true
        return false
    }
    g_queue = metal_new_command_queue(g_dev)
    var err : string
    g_pso = pipeline_from_source(g_dev, metal_q8_gemm_msl, metal_q8_gemm_msl_entry, metal_q8_gemm_msl_fastmath, err)
    if (g_pso != null) {
        g_pso64 = pipeline_from_source(g_dev, metal_q8_gemm64_msl, metal_q8_gemm64_msl_entry, metal_q8_gemm64_msl_fastmath, err)
    }
    if (g_pso == null || g_pso64 == null) {
        to_log(LOG_ERROR, "dasLLAMA metal: kernel compile failed — {err}\n")
        if (g_pso != null) {
            metal_release(g_pso)
            g_pso = null
        }
        metal_release(g_queue)
        g_queue = null
        metal_release(g_dev)
        g_dev = null
        g_failed = true
        return false
    }
    to_log(LOG_INFO, "dasLLAMA metal: GEMM offload live on {metal_device_name(g_dev)}\n")
    return true
}

// lazily upload one stable weight/scale region, keyed by its base address (post-load weight
// arrays never move; the reinterpret is address math only — nothing writes through it)
def private upload_region(var tab : table<uint64; MetalBuffer?>; src : void?; bytes : uint64) : MetalBuffer? {
    let key = intptr(src)
    if (key_exists(tab, key)) {
        return tab[key]
    }
    var buf = metal_new_buffer(g_dev, bytes)
    unsafe {
        memcpy(metal_buffer_contents(buf), src, int(bytes))
    }
    tab[key] = buf
    return buf
}

// one pooled 4-byte uniform buffer holding a uint
def private uniform_u32(v : uint) : MetalBuffer? {
    var buf = pool_acquire(g_pool, g_dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

//! The batch slot (MatmulQ8Q8BatchFn): y[ntok x d] = X[ntok x n] . W[d x n]^T on the GPU.
//! Small/odd shapes delegate to the portable CPU batch kernel; any GPU-side init failure
//! degrades to the same fallback (logged once).
def metal_q8q8_batch(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    if (ntok < g_min_ntok || (n % 32l) != 0l || (d % 32l) != 0l || !metal_gemm_init()) {
        g_fallbacks++
        let cpu = kernel_backend_batch_fn("portable")
        invoke(cpu, yp, wp, sp, xqp, xsp, n, d, ntok)
        return
    }
    let nb = n / 32l
    let mp = ((ntok + 63l) / 64l) * 64l     // kernels have no edge masking — pad M to 64 rows
    var wbuf = upload_region(g_wbufs, unsafe(reinterpret<void?>(wp)), uint64(d * n))
    var sbuf = upload_region(g_sbufs, unsafe(reinterpret<void?>(sp)), uint64(d * nb * 4l))
    var xqbuf = pool_acquire(g_pool, g_dev, uint64(mp * n))
    var xsbuf = pool_acquire(g_pool, g_dev, uint64(mp * nb * 4l))
    var ybuf = pool_acquire(g_pool, g_dev, uint64(mp * d * 4l))
    unsafe {
        var pq = reinterpret<uint8?>(metal_buffer_contents(xqbuf))
        memcpy(reinterpret<void?>(pq), reinterpret<void?>(xqp), int(ntok * n))
        memset8(reinterpret<void?>(pq + ntok * n), uint8(0), int((mp - ntok) * n))
        var ps = reinterpret<uint8?>(metal_buffer_contents(xsbuf))
        memcpy(reinterpret<void?>(ps), reinterpret<void?>(xsp), int(ntok * nb * 4l))
        memset8(reinterpret<void?>(ps + ntok * nb * 4l), uint8(0), int((mp - ntok) * nb * 4l))
    }
    var kdimbuf = uniform_u32(uint(n))
    var ndimbuf = uniform_u32(uint(d))
    var err : string
    let use64 = gemm_use64(mp, d)
    let groups = uint(use64 ? (mp / 64l) * (d / 64l) : (mp / 32l) * (d / 32l))
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, use64 ? g_pso64 : g_pso)
        metal_set_threadgroup_memory_length(enc, use64 ? metal_q8_gemm64_msl_tgmem : metal_q8_gemm_msl_tgmem, 0)
        metal_set_buffer(enc, wbuf, 0ul, 0)
        metal_set_buffer(enc, sbuf, 0ul, 1)
        metal_set_buffer(enc, xqbuf, 0ul, 2)
        metal_set_buffer(enc, xsbuf, 0ul, 3)
        metal_set_buffer(enc, ybuf, 0ul, 4)
        metal_set_buffer(enc, kdimbuf, 0ul, 5)
        metal_set_buffer(enc, ndimbuf, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(groups, 1u, 1u), uint3(128u, 1u, 1u))
    }
    if (ran) {
        unsafe {
            memcpy(reinterpret<void?>(yp), metal_buffer_contents(ybuf), int(ntok * d * 4l))
        }
        g_dispatches++
    } else {
        to_log(LOG_ERROR, "dasLLAMA metal: dispatch failed ({err}) — falling back to the portable CPU kernel\n")
        g_fallbacks++
        let cpu = kernel_backend_batch_fn("portable")
        invoke(cpu, yp, wp, sp, xqp, xsp, n, d, ntok)
    }
    pool_release(g_pool, xqbuf, uint64(mp * n))
    pool_release(g_pool, xsbuf, uint64(mp * nb * 4l))
    pool_release(g_pool, ybuf, uint64(mp * d * 4l))
    pool_release(g_pool, kdimbuf, 4ul)
    pool_release(g_pool, ndimbuf, 4ul)
}

// ===== registration =====

def private metal_backend_available : bool {
    if (g_pso != null) {
        return true
    }
    if (g_failed) {
        return false
    }
    let dev = metal_create_system_default_device()
    if (dev == null) {
        return false
    }
    metal_release(dev)
    return true
}

[unused_argument(yp, wp, sp, xqp, xsp, n, d)]
def private metal_no_gemv(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    panic("dasLLAMA metal backend is batch-only — pin it via DASLLAMA_PIN_BATCH_BACKEND=metal, never as the whole backend")
}

[unused_argument(y0p, y1p, y2p, wp, sp, woff0, woff1, woff2, xqp, xsp, n, d0, d1, d2)]
def private metal_no_group3(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?; woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    panic("dasLLAMA metal backend is batch-only — pin it via DASLLAMA_PIN_BATCH_BACKEND=metal, never as the whole backend")
}

[unused_argument(wp, sp, n, d)]
def private metal_repack_noop(var wp : int8?; var sp : float?; n, d : int64) {}

[init]
def dasllama_math_metal_register() {
    // priority -1: never wins auto- or load-select (even registering first); reachable only by
    // name — select_batch_backend("metal") — with the device probe as the availability witness
    register_kernel_backend(KernelBackend(name = "metal", mm = @@metal_no_gemv, batch = @@metal_q8q8_batch,
        group3 = @@metal_no_group3, repack = @@metal_repack_noop,
        needs_repack = false, priority = -1, available = @@metal_backend_available))
}
