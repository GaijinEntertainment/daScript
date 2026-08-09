options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _function_length = 0        // one MSL kernel body per lab twin — barrier/register coupled, no seam
options _cyclomatic_complexity = 0  // the lab runner is a flat shape/arm ladder

require metal/msl_shader
require ?das_metal metal/das_metal_boost
require dasllama/dasllama_metal_kernels   // production k6 stamp + MetalMmTileBase
require ?das_metal ../../tests/_metal_kernel_common.das   // kq plane fills + buf helpers
require math
require daslib/math_bits
require strings

// The dense KV-quant prefill GEMM lab: production k6 (the SIXBIT arm of MetalKqMulMmK45T —
// superblock scalars RELOADED per kb) vs the historical cached twin (sv/dall hoisted across the
// 8 kb of a superblock behind a js==0 guard). The cache cost an occupancy tier (896 vs 1024
// max_threads) and measured 1.7-10% slower per shape — this lab keeps that verdict measurable.
// Same arithmetic in the same order, so the two arms must be BIT-exact.
//
//   bin/daslang -jit modules/dasLLAMA/benchmarks/matmul/bench_metal_kq_mm_lab.das

// The cached twin: production's body with the per-kb scalar reload replaced by the hoisted
// sv/dall + js==0 guard (blk changes every 8th kb) — the pre-merge production form.
class LabKqK6Cached : MetalMmTileBase {
    @ssbo @binding = 0 @role = "weight" kdh : array<float16>
    @ssbo @binding = 1 @role = "weight" ksc : array<uint4>
    @ssbo @binding = 2 @role = "weight" kqu : array<uint>
    @ssbo @binding = 3 @role = "read" xf : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint

    [metal_kernel(name="lab_kq_k6_cached_msl")]
    def lab_kq_k6_cached {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        var mc : simdgroup_float8x8[8]
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        var va : float16[16]
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u
        let bB = (sg / 2u) * 128u
        let nsb = kdim / 256u
        let wrow = nBase + lr0
        var xcur = unsafe(addr(xf[(mBase + lr1) * kdim4 + iy / 4u]))
        var sv : uint4
        var dall = 0.0
        var kb = 0u
        while (kb < nkb) {
            let sb = kb / 8u
            let js = kb % 8u
            let blk = wrow * nsb + sb
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            let half6 = js / 4u
            let gg = js % 4u
            if (js == 0u) {   // superblock scalars change every 8th kb; uniform branch, no divergence
                sv = ksc[blk]
                dall = float(kdh[blk])
            }
            let si = half6 * 8u + gg * 2u + il0
            let scw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
            let s6 = float(((int(scw >> ((si & 3u) * 8u)) & 255) ^ 128) - 128)
            let dsc = dall * s6
            let dmn = dsc * 32.0
            let dsc1 = dsc * 0.00390625
            let dsc2 = dsc * 0.0000152587890625
            let dsc3 = dsc * 0.000000059604644775390625
            let qb = blk * 48u + half6 * 16u + (gg & 1u) * 8u + il0 * 4u
            let hb = blk * 48u + 32u + half6 * 8u + il0 * 4u
            let nsh = (gg / 2u) * 4u
            let hsh = gg * 2u
            for [unroll_full] (k in range(4)) {
                let u = kqu[qb + uint(k)]
                let hu = kqu[hb + uint(k)]
                let qv = ((u >> nsh) & 0x0F0F0F0F) | (((hu >> hsh) & 0x03030303) << 4u)
                va[k * 4] = float16(dsc * float(qv & 255u) - dmn)
                va[k * 4 + 1] = float16(dsc1 * float(qv & 65280u) - dmn)
                va[k * 4 + 2] = float16(dsc2 * float(qv & 16711680u) - dmn)
                va[k * 4 + 3] = float16(dsc3 * float(qv & 4278190080u) - dmn)
            }
            barrier()
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            acc_quad(aB, bB, ma, mb, mc)
            xcur = unsafe(xcur + 8)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

let LAB_M = 512         // prefill token batch (rows of X)
let LAB_ROUNDS = 9      // strict A/B alternation per round, best-of reported
let LAB_MMS = 8         // dispatches per timed command buffer

def private time_pass(queue; pso; bufs; tgm : uint64; m, n, d : int) : double {
    var cb = metal_new_command_buffer(queue)
    var enc = metal_new_compute_encoder(cb)
    metal_set_pipeline(enc, pso)
    metal_set_threadgroup_memory_length(enc, tgm, 0)
    let nsb = d * (n / 256)
    metal_set_buffer(enc, bufs[0], uint64(nsb * 16), 0)   // k6: the f16 d tail rides after the sub-scale blocks
    metal_set_buffer(enc, bufs[0], 0ul, 1)
    metal_set_buffer(enc, bufs[1], 0ul, 2)
    metal_set_buffer(enc, bufs[2], 0ul, 3)
    metal_set_buffer(enc, bufs[3], 0ul, 4)
    metal_set_buffer(enc, bufs[4], 0ul, 5)
    metal_set_buffer(enc, bufs[5], 0ul, 6)
    for (_r in range(LAB_MMS)) {
        metal_dispatch_threadgroups(enc, uint3(uint(m / 32), uint(d / 64), 1u), uint3(128u, 1u, 1u))
    }
    metal_end_encoding(enc)
    metal_commit(cb)
    metal_wait_until_completed(cb)
    let err = metal_command_buffer_error(cb)
    if (!empty(err)) {
        panic("command buffer error: {err}")
    }
    let us = (metal_command_buffer_gpu_end_time(cb) - metal_command_buffer_gpu_start_time(cb)) * 1e6lf
    metal_release(enc)
    metal_release(cb)
    return us / double(LAB_MMS)
}

def private main_apple(dummy) {
    var dev = metal_create_system_default_device()
    if (dev == null) {
        print("skipped: no Metal device\n")
        return
    }
    print("device={metal_device_name(dev)} m={LAB_M} rounds={LAB_ROUNDS} mms/pass={LAB_MMS}\n")
    var queue = metal_new_command_queue(dev)
    var err = ""
    var pso_prod = pipeline_from_source(dev, MetalKqMulMmK6_metal_kq_mulmm_msl, MetalKqMulMmK6_metal_kq_mulmm_msl_entry, MetalKqMulMmK6_metal_kq_mulmm_msl_fastmath, err)
    if (pso_prod == null) {
        panic("prod pso: {err}")
    }
    var pso_cached = pipeline_from_source(dev, lab_kq_k6_cached_msl, lab_kq_k6_cached_msl_entry, lab_kq_k6_cached_msl_fastmath, err)
    if (pso_cached == null) {
        panic("cached pso: {err}")
    }
    print("prod: max_threads={metal_pipeline_max_total_threads(pso_prod)}  cached: max_threads={metal_pipeline_max_total_threads(pso_cached)}\n")
    // llama-3B-class GEMM geometries (n = reduction dim, d = output rows); n % 256, d % 64
    var shapes <- [("kv3b", 3072, 1024), ("q3b", 3072, 3072), ("w13_3b", 3072, 8192), ("w2_3b", 8192, 3072)]
    let m = LAB_M
    for (sh in shapes) {
        let n = sh._1
        let d = sh._2
        let nb = n / 256
        let nsb = d * nb
        var kq : array<uint8>
        var ks : array<uint8>
        kq_fill_planes(6, nsb, kq, ks)
        var xf : array<float>
        xf |> resize(m * n)
        for (i in range(m * n)) {
            xf[i] = 0.07 * float(i % 39) - 1.3
        }
        var bks = buf_upload(dev, ks)
        var bkq = buf_upload(dev, kq)
        var bx = buf_upload(dev, xf)
        var bya = buf_fill(dev, m * d, -1000.0)
        var byb = buf_fill(dev, m * d, -2000.0)
        var bk = buf_u32(dev, uint(n))
        var bn = buf_u32(dev, uint(d))
        var bufs_a <- [bks, bkq, bx, bya, bk, bn]
        var bufs_b <- [bks, bkq, bx, byb, bk, bn]
        // correctness first: the arms differ only in load schedule — outputs must be BIT-exact
        time_pass(queue, pso_prod, bufs_a, MetalKqMulMmK6_metal_kq_mulmm_msl_tgmem, m, n, d)
        time_pass(queue, pso_cached, bufs_b, lab_kq_k6_cached_msl_tgmem, m, n, d)
        var mismatches = 0
        unsafe {
            let pa = reinterpret<uint?>(metal_buffer_contents(bya))
            let pb = reinterpret<uint?>(metal_buffer_contents(byb))
            for (i in range(m * d)) {
                if (pa[i] != pb[i]) {
                    mismatches++
                }
            }
            // poisoned-fill negative control: the kernel must have written every element
            let poison = float_bits_to_uint(-1000.0)
            var untouched = 0
            for (i in range(m * d)) {
                if (pa[i] == poison) {
                    untouched++
                }
            }
            if (untouched > 0) {
                panic("{sh._0}: {untouched} outputs untouched - dispatch geometry wrong")
            }
        }
        if (mismatches > 0) {
            panic("{sh._0}: {mismatches} of {m * d} outputs differ - arms are not the same arithmetic")
        }
        let mac_mm = double(m) * double(n) * double(d)
        var best_a = 1e30lf
        var best_b = 1e30lf
        var rows_a : array<int>
        var rows_b : array<int>
        rows_a |> reserve(LAB_ROUNDS)
        rows_b |> reserve(LAB_ROUNDS)
        for (_round in range(LAB_ROUNDS)) {
            let ua = time_pass(queue, pso_prod, bufs_a, MetalKqMulMmK6_metal_kq_mulmm_msl_tgmem, m, n, d)
            let ub = time_pass(queue, pso_cached, bufs_b, lab_kq_k6_cached_msl_tgmem, m, n, d)
            best_a = min(best_a, ua)
            best_b = min(best_b, ub)
            rows_a |> push(int(ua))
            rows_b |> push(int(ub))
        }
        print("--- {sh._0} ({m}x{n}x{d}, {mac_mm / 1e9lf} GMAC/mm) bit-exact ok\n")
        print("  prod   best {best_a / 1000.0lf} ms/mm  {mac_mm / best_a / 1000.0lf} GMAC/s  us/round:{rows_a}\n")
        print("  cached best {best_b / 1000.0lf} ms/mm  {mac_mm / best_b / 1000.0lf} GMAC/s  us/round:{rows_b}\n")
        delete rows_a
        delete rows_b
        print("  cached vs prod: {(best_b - best_a) * 100.0lf / best_a}%\n")
        metal_release(bks)
        metal_release(bkq)
        metal_release(bx)
        metal_release(bya)
        metal_release(byb)
        metal_release(bk)
        metal_release(bn)
        delete kq
        delete ks
        delete xf
        bufs_a |> clear()   // borrowed pointers — clear before delete (delete would free pointees)
        bufs_b |> clear()
        unsafe {
            delete bufs_a
            delete bufs_b
        }
    }
    metal_release(pso_prod)
    metal_release(pso_cached)
    metal_release(queue)
    metal_release(dev)
    delete shapes
}

[export]
def main {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        main_apple(0)
    } else {
        print("das_metal is not built on this platform\n")
    }
}
