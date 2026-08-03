options gen2
options _comment_hygiene = true

module dasllama_vulkan_kernels shared public
options _function_length = 0   // GPU kernel bodies are the domain shape
options _cyclomatic_complexity = 0   // same

require dasllama/dasllama_vulkan_lens
require spirv/spirv_shader
require spirv/spirv_builtins
require math

// The Vulkan kernel set — every [compute_shader] with its @ssbo/@workgroup globals, and nothing
// else: no device state, no vulkan API, no engine types. The derived-access spec is built HERE
// (the lens call-macro scans the calling module); dasllama_vulkan_common consumes it.

// ===== the kernel =====
// One SUBGROUP per output row of one region. Weights arrive as TWO row-major planes — packed
// int8 quant words + a float16 scale plane (16-bit storage) — NOT 34-byte blocks: an
// interleaved block layout byte-loads at stride 34 across the warp and measured ~21GB/s on the
// 3060 Ti (~1.2ms per dispatch — the real cost behind stage-1a's "1.35ms/dispatch"). Here 8
// lanes cooperate per 32-weight block (lane = one 4-byte word), so every warp load is 128
// CONSECUTIVE bytes — one transaction. The block's integer dot stays exact — sdot4 (dp4a) per
// word, then a 3-step shuffleXor fold across the 8 lanes; lane 0 applies the f16*f32 scales.
// Params ride in vk_meta. (Historically "push constants drop small trailing scalars" — RESOLVED
// upstream: dasVulkan stages pushes through the std140 walk, gate tests/integration/
// test_push_std140.das. The class-kernel arc moves params to push constants; see
// plans/vulkan_class_kernels.md.) Words: [0]=n [1]=d [2]=nregions [3]=y base row (deltanet step chains
// route two GEMVs into one shared y buffer; every other fill zeroes it), then
// (rel_wblock, xblock) uint pairs per region — 32-weight BLOCK offsets (weight blocks relative
// to the resident stack, activation blocks into vk_xq/vk_xs).

var @ssbo @binding = 0 vk_wq : array<uint>
var @ssbo @binding = 0 vk_wq4 : array<uint4>   // vec4 view of the same weight words (16B loads)
var @ssbo @binding = 1 vk_ws : array<float16>
var @ssbo @binding = 2 vk_meta : array<uint>
var @ssbo @binding = 3 vk_xq : array<uint>
var @ssbo @binding = 3 vk_xq4 : array<uint4>   // vec4 view of the activation words (16B loads)
var @ssbo @binding = 4 vk_xs : array<float>
var @ssbo @binding = 5 vk_y : array<float>

[compute_shader(local_size_x=64, local_size_x_id=0, name="q8_moe_groupn_spv")]
def q8_moe_groupn {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let nreg = vk_meta[2u]
    let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
    if (rg < nreg * d) {
        let r = rg / d
        let row = rg % d
        let nb = n / 32u
        let wb0 = vk_meta[4u + r * 2u] + row * nb
        let xb0 = vk_meta[5u + r * 2u]
        let lane = gl_SubgroupInvocationID
        // uint4 loads, 2 lanes per block (a block is 2 vec4s): subgroupSize/2 blocks per pass,
        // 512B/warp in flight, one shuffle per pass (probe: +4-12% DRAM vs scalar)
        let bpp = gl_SubgroupSize / 2u
        var acc = 0.0
        var bi0 = 0u
        while (bi0 < nb) {
            let b = bi0 + lane / 2u
            if (b < nb) {
                let wv = vk_wq4[(wb0 + bi0) * 2u + lane]
                let xbase = (xb0 + bi0) * 8u + lane * 4u
                var idot = sdot4(wv.x, vk_xq[xbase])
                idot += sdot4(wv.y, vk_xq[xbase + 1u])
                idot += sdot4(wv.z, vk_xq[xbase + 2u])
                idot += sdot4(wv.w, vk_xq[xbase + 3u])
                idot += subgroupShuffleXor(idot, 1u)
                if (lane % 2u == 0u) {
                    acc += float(idot) * float(vk_ws[wb0 + b]) * vk_xs[xb0 + b]
                }
            }
            bi0 += bpp
        }
        let total = subgroupAdd(acc)
        if (subgroupElect()) {
            vk_y[vk_meta[3u] + rg] = total
        }
    }
}

// ===== the prefill kernel =====
// 32x32 tiles through shared so weight rows stream once per 32 rows — a GEMV-style loop would
// re-stream the whole expert stack per row (~6GB/dispatch at pp512). K advances 8 blocks per barrier pair, cutting barrier count 8x.

var @workgroup bt_xw4 : uint4[544]   // x tile: 32 rows x 16 uint4 chunks (8 blocks deep), stride 17
                                     // in v4 units — vectorized so the hot loop issues 1 shared load
                                     // per 4 sdot4 (scalar tiles were LSU-throughput-bound at 1:1)
var @workgroup bt_ww4 : uint4[544]   // w tile: 32 cols x 16 chunks, stride 17. A thread's 4 output
                                     // cols are word+{0,8,16,24} (NOT word*4+c: with 16B elements a
                                     // 4-col-contiguous warp lands 8 lanes on 2 banks; stride-8 cols
                                     // put the 8 lanes on 8 distinct banks every access phase)
var @workgroup bt_xs : float[256]    // x scales per (block, row); kq kernels use [row] only (per-256)
var @workgroup bt_ws : float[256]    // w scales per (block, col)

// this workgroup's region record from the batch meta's per-wg region map (word [3] points at it;
// records are 4 words): x = weight base (per-format units), y = window-relative activation row
// base, z = row count, w = the wg's tile index rebased to the region
def private vk_region_rec : uint4 {
    let rid = vk_meta[vk_meta[3u] + gl_WorkGroupID.x]
    let rb = 4u + rid * 4u
    return uint4(vk_meta[rb], vk_meta[rb + 1u], vk_meta[rb + 2u], gl_WorkGroupID.x - vk_meta[rb + 3u])
}

[compute_shader(local_size_x=256, name="q8_moe_batch_spv")]
def q8_moe_batch {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let reg = vk_region_rec()
    let wblk0 = reg.x
    let row0 = reg.y
    let cnt = reg.z
    let tix = reg.w
    let wtiles = (d + 31u) / 32u
    let xt = tix / wtiles
    let wt = tix % wtiles
    let nbb = n / 32u
    let nsteps = (nbb + 7u) / 8u     // q8 K need not be superblock-aligned (nfe 1408 = 44 blocks)
    let tid = gl_LocalInvocationID.x
    let word = tid % 8u
    let grp = tid / 8u               // 0..31: the x row AND w col this thread stages
    let xrow = xt * 32u + grp
    let wcol = wt * 32u + grp
    let slane = tid % 32u            // scale staging: this thread covers (block sblk, lane slane)
    let sblk = tid / 32u
    var f0 = 0.0
    var f1 = 0.0
    var f2 = 0.0
    var f3 = 0.0
    var s = 0u
    while (s < nsteps) {
        let b0 = s * 8u
        for (ki in range(2)) {       // this thread stages v4 chunks word and word+8 of its row/col
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let h = (k % 2u) * 4u
            var xv = uint4(0u)
            if (xrow < cnt && b0 + bu < nbb) {
                let xb4 = ((row0 + xrow) * nbb + b0 + bu) * 8u + h
                xv = uint4(vk_xq[xb4], vk_xq[xb4 + 1u], vk_xq[xb4 + 2u], vk_xq[xb4 + 3u])
            }
            bt_xw4[grp * 17u + k] = xv
            var wv = uint4(0u)
            if (wcol < d && b0 + bu < nbb) {
                let wb4 = (wblk0 + wcol * nbb + b0 + bu) * 8u + h
                wv = uint4(vk_wq[wb4], vk_wq[wb4 + 1u], vk_wq[wb4 + 2u], vk_wq[wb4 + 3u])
            }
            bt_ww4[grp * 17u + k] = wv
        }
        var xs = 0.0
        if (xt * 32u + slane < cnt && b0 + sblk < nbb) {
            xs = vk_xs[(row0 + xt * 32u + slane) * nbb + b0 + sblk]
        }
        bt_xs[sblk * 32u + slane] = xs
        var ws = 0.0
        if (wt * 32u + slane < d && b0 + sblk < nbb) {
            ws = float(vk_ws[wblk0 + (wt * 32u + slane) * nbb + b0 + sblk])
        }
        bt_ws[sblk * 32u + slane] = ws
        barrier()
        for (bb in range(8)) {       // zero-filled tail blocks contribute 0 (ws = xs = 0)
            let bu = uint(bb)
            var i0 = 0
            var i1 = 0
            var i2 = 0
            var i3 = 0
            for (kk in range(2)) {
                let k = bu * 2u + uint(kk)
                let xw = bt_xw4[grp * 17u + k]
                let wv0 = bt_ww4[word * 17u + k]
                let wv1 = bt_ww4[(word + 8u) * 17u + k]
                let wv2 = bt_ww4[(word + 16u) * 17u + k]
                let wv3 = bt_ww4[(word + 24u) * 17u + k]
                i0 += sdot4(wv0.x, xw.x) + sdot4(wv0.y, xw.y) + sdot4(wv0.z, xw.z) + sdot4(wv0.w, xw.w)
                i1 += sdot4(wv1.x, xw.x) + sdot4(wv1.y, xw.y) + sdot4(wv1.z, xw.z) + sdot4(wv1.w, xw.w)
                i2 += sdot4(wv2.x, xw.x) + sdot4(wv2.y, xw.y) + sdot4(wv2.z, xw.z) + sdot4(wv2.w, xw.w)
                i3 += sdot4(wv3.x, xw.x) + sdot4(wv3.y, xw.y) + sdot4(wv3.z, xw.z) + sdot4(wv3.w, xw.w)
            }
            let xscl = bt_xs[bu * 32u + grp]
            f0 += float(i0) * bt_ws[bu * 32u + word] * xscl
            f1 += float(i1) * bt_ws[bu * 32u + word + 8u] * xscl
            f2 += float(i2) * bt_ws[bu * 32u + word + 16u] * xscl
            f3 += float(i3) * bt_ws[bu * 32u + word + 24u] * xscl
        }
        barrier()
        s++
    }
    if (xrow < cnt) {
        let yb = (row0 + xrow) * d + wt * 32u + word
        if (wt * 32u + word < d) {
            vk_y[yb] = f0
        }
        if (wt * 32u + word + 8u < d) {
            vk_y[yb + 8u] = f1
        }
        if (wt * 32u + word + 16u < d) {
            vk_y[yb + 16u] = f2
        }
        if (wt * 32u + word + 24u < d) {
            vk_y[yb + 24u] = f3
        }
    }
}

// ===== the prefill kernel — cooperative-matrix variants (DASLLAMA_COOPMAT=f16|int8) =====
// Same MoE routing + 32x32 tile + Q8_0 layout as q8_moe_batch, but the inner GEMM rides the tensor
// cores: 128 threads / 4 subgroups, one 16x16 subtile each; edge tiles zero-fill + bounds-check.

var @workgroup cm_as : float16[8192]   // 32 local rows x 256 k, row-major stride 256 (f16 dequant A)
var @workgroup cm_bs : float16[8192]   // 256 k x 32 cols, row-major stride 32 (f16 dequant B)
var @workgroup cm_out : float[1024]    // 4 subgroups x 16x16 f32 scratch (bounds-checked write-out)

[compute_shader(local_size_x=128, name="q8_moe_batch_cmf16_spv")]
def q8_moe_batch_cmf16 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let reg = vk_region_rec()
    let wblk0 = reg.x
    let row0 = reg.y
    let cnt = reg.z
    let tix = reg.w
    let wtiles = (d + 31u) / 32u
    let xt = tix / wtiles
    let wt = tix % wtiles
    let nbb = n / 32u
    let nsteps = (nbb + 7u) / 8u
    let tid = gl_LocalInvocationID.x
    let sg = gl_SubgroupID          // 0..3 (subgroupSize 32)
    let smi = sg / 2u               // which 16-row half of the tile
    let sni = sg % 2u               // which 16-col half of the tile
    var acc : coopmatAcc_f32_16x16
    var s = 0u
    while (s < nsteps) {
        let b0 = s * 8u
        for (i in range(16)) {       // stage A: 32x64 = 2048 uints, 16/thread; OOR -> 0
            let u = tid * 16u + uint(i)
            let row = u / 64u          // 0..31 local token row
            let chunk = u % 64u
            let blk = chunk / 8u
            let w = chunk % 8u
            let xrow = xt * 32u + row
            let gblk = b0 + blk
            var packed = 0u
            var xs = 0.0
            if (xrow < cnt && gblk < nbb) {
                packed = vk_xq[((row0 + xrow) * nbb + gblk) * 8u + w]
                xs = vk_xs[(row0 + xrow) * nbb + gblk]
            }
            let qv = int4(unpack8(int(packed)))
            let o = row * 256u + blk * 32u + w * 4u
            cm_as[o] = float16(float(qv.x) * xs)
            cm_as[o + 1u] = float16(float(qv.y) * xs)
            cm_as[o + 2u] = float16(float(qv.z) * xs)
            cm_as[o + 3u] = float16(float(qv.w) * xs)
        }
        for (i in range(16)) {       // stage B: 32x64 = 2048 uints, 16/thread; OOR -> 0
            let u = tid * 16u + uint(i)
            let col = u / 64u          // 0..31 output col
            let chunk = u % 64u
            let blk = chunk / 8u
            let w = chunk % 8u
            let wcol = wt * 32u + col
            let gblk = b0 + blk
            var packed = 0u
            var ws = 0.0
            if (wcol < d && gblk < nbb) {
                packed = vk_wq[(wblk0 + wcol * nbb + gblk) * 8u + w]
                ws = float(vk_ws[wblk0 + wcol * nbb + gblk])
            }
            let qv = int4(unpack8(int(packed)))
            let k = blk * 32u + w * 4u
            cm_bs[k * 32u + col] = float16(float(qv.x) * ws)
            cm_bs[(k + 1u) * 32u + col] = float16(float(qv.y) * ws)
            cm_bs[(k + 2u) * 32u + col] = float16(float(qv.z) * ws)
            cm_bs[(k + 3u) * 32u + col] = float16(float(qv.w) * ws)
        }
        barrier()
        for (bb in range(8)) {       // 8 blocks x 2 K-subtiles = 16 MMAs into one acc
            for (ks in range(2)) {
                var a : coopmatA_f16_16x16
                var b : coopmatB_f16_16x16
                coopmatLoad(a, cm_as, int(smi * 16u * 256u + uint(bb) * 32u + uint(ks) * 16u), 256, 0)
                coopmatLoad(b, cm_bs, int((uint(bb) * 32u + uint(ks) * 16u) * 32u + sni * 16u), 32, 0)
                acc = coopmatMulAdd(a, b, acc)
            }
        }
        barrier()
        s++
    }
    coopmatStore(acc, cm_out, int(sg * 256u), 16, 0)   // scratch, then bounds-checked write-out
    barrier()
    let lane = gl_SubgroupInvocationID
    for (e in range(8)) {
        let idx = lane * 8u + uint(e)
        let m = idx / 16u
        let nn = idx % 16u
        let orow = xt * 32u + smi * 16u + m
        let ocol = wt * 32u + sni * 16u + nn
        if (orow < cnt && ocol < d) {
            vk_y[(row0 + orow) * d + ocol] = cm_out[sg * 256u + idx]
        }
    }
}

// int8 path: the tensor cores eat s8 quants directly — K-tile 32 = one Q8_0 block, one MMA per block,
// then the s32 tile spills to shared for the SIMT xs[m]*ws[n] scale (the per-32 scale can't ride the
// opaque tile). s8 loads come straight from global (int8 views); s8-unit offsets need uint idx (~4GB).

var @ssbo @binding = 0 vk_wq8 : array<int8>   // int8 view of the vk_wq bytes (same VkBuffer)
var @ssbo @binding = 3 vk_xq8 : array<int8>   // int8 view of the vk_xq bytes

var @workgroup cmi_s32 : int[1024]     // 4 subgroups x 16x16 s32 spill
var @workgroup cmi_acc : float[1024]   // 4 subgroups x 16x16 f32 accumulator

[compute_shader(local_size_x=128, name="q8_moe_batch_cmi8_spv")]
def q8_moe_batch_cmi8 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let reg = vk_region_rec()
    let wblk0 = reg.x
    let row0 = reg.y
    let cnt = reg.z
    let tix = reg.w
    let wtiles = (d + 31u) / 32u
    let xt = tix / wtiles
    let wt = tix % wtiles
    let nbb = n / 32u
    let sg = gl_SubgroupID
    let lane = gl_SubgroupInvocationID
    let smi = sg / 2u
    let sni = sg % 2u
    let arow = xt * 32u + smi * 16u        // this subgroup's local token-row base
    let bcol = wt * 32u + sni * 16u        // this subgroup's output-col base
    for (e in range(8)) {
        cmi_acc[sg * 256u + lane * 8u + uint(e)] = 0.0
    }
    var blk = 0u
    while (blk < nbb) {
        var a : coopmatA_s8_16x32
        var b : coopmatB_s8_32x16
        coopmatLoad(a, vk_xq8, ((row0 + arow) * nbb + blk) * 32u, int(n), 0)      // A 16 rows, rowmajor
        coopmatLoad(b, vk_wq8, (wblk0 + bcol * nbb + blk) * 32u, int(n), 1)       // B 16 cols, colmajor
        var s32 : coopmatAcc_s32_16x16
        s32 = coopmatMulAdd(a, b, s32)   // nolint:STYLE011 — MulAdd seeds from its own zero-init tile
        coopmatStore(s32, cmi_s32, int(sg * 256u), 16, 0)
        barrier()
        for (e in range(8)) {              // SIMT: acc += s32 * xs[m] * ws[n]
            let idx = lane * 8u + uint(e)
            let m = idx / 16u
            let nn = idx % 16u
            let xs = vk_xs[(row0 + arow + m) * nbb + blk]
            let ws = float(vk_ws[wblk0 + (bcol + nn) * nbb + blk])
            // i*ws*xs, matching q8_moe_batch's multiply association — bit-parity with the sdot4 kernel
            cmi_acc[sg * 256u + idx] += float(cmi_s32[sg * 256u + idx]) * ws * xs
        }
        barrier()
        blk++
    }
    for (e in range(8)) {                  // bounds-checked write-out (edge tiles)
        let idx = lane * 8u + uint(e)
        let m = idx / 16u
        let nn = idx % 16u
        let orow = arow + m
        let ocol = bcol + nn
        if (orow < cnt && ocol < d) {
            vk_y[(row0 + orow) * d + ocol] = cmi_acc[sg * 256u + idx]
        }
    }
}

// The mul_mm L-tile variant (DASLLAMA_COOPMAT=mm): llama.cpp's aligned-L geometry — BM=BN=128
// BK=32, 4 warps 2x2 with 16 f16acc fragments each — 108-129% of lcpp's own mul_mm in isolation
// (modules/dasVulkan/_coopmat_mulmm_port.das). Edge tiles bounce stores via cml_o; subgroupSize 32.

let private CML_SST = 20u   // shared row stride in uints (16 f16-pair data + 4 pad — bank spread)

var @workgroup cml_a : uint[2560]     // A tile: 128 weight rows x one block (32 K) as f16 pairs
var @workgroup cml_b : uint[2560]     // B tile: 128 token rows x one block
var @workgroup cml_o : float16[1024]  // 4 warps x 16x16 f16-acc store bounce (edge tiles only) —
                                      // 2KB keeps total shared at 22528B = 2 wg/SM (f32 tipped to 1)

[compute_shader(local_size_x=128, name="q8_moe_batch_mm_spv")]
def q8_moe_batch_mm {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let reg = vk_region_rec()
    let wblk0 = reg.x
    let row0 = reg.y
    let cnt = reg.z
    let tix = reg.w
    let wtiles = (d + 127u) / 128u
    let xt = tix / wtiles             // token (B) tile
    let wt = tix % wtiles             // weight (A) tile
    let gm0 = wt * 128u
    let gn0 = xt * 128u
    let nbb = n / 32u
    let tid = gl_LocalInvocationID.x
    let warp_r = gl_SubgroupID % 2u   // which 64-row half of the A tile
    let warp_c = gl_SubgroupID / 2u   // which 64-col half of the B tile
    let lw = tid % 8u                 // which packed word (4 qs) of the staged row's block
    let lr0 = tid / 8u                // staging row base 0..15
    var acc00 : coopmatAcc_f16_16x16
    var acc01 : coopmatAcc_f16_16x16
    var acc02 : coopmatAcc_f16_16x16
    var acc03 : coopmatAcc_f16_16x16
    var acc10 : coopmatAcc_f16_16x16
    var acc11 : coopmatAcc_f16_16x16
    var acc12 : coopmatAcc_f16_16x16
    var acc13 : coopmatAcc_f16_16x16
    var acc20 : coopmatAcc_f16_16x16
    var acc21 : coopmatAcc_f16_16x16
    var acc22 : coopmatAcc_f16_16x16
    var acc23 : coopmatAcc_f16_16x16
    var acc30 : coopmatAcc_f16_16x16
    var acc31 : coopmatAcc_f16_16x16
    var acc32 : coopmatAcc_f16_16x16
    var acc33 : coopmatAcc_f16_16x16
    var kb = 0u
    while (kb < nbb) {
        for [unroll] (p in range(8)) {   // stage A: 128 weight rows x 1 block; OOR rows -> 0
            let row = lr0 + uint(p) * 16u
            let wrow = gm0 + row
            var v = float4(0.0)
            if (wrow < d) {
                let ib = wblk0 + wrow * nbb + kb
                v = float4(int4(unpack8(int(vk_wq[ib * 8u + lw])))) * float(vk_ws[ib])
            }
            let o = row * CML_SST + lw * 2u
            cml_a[o] = packHalf2x16(v.xy)
            cml_a[o + 1u] = packHalf2x16(v.zw)
        }
        for [unroll] (p in range(8)) {   // stage B: 128 token rows x 1 block; OOR rows -> 0
            let row = lr0 + uint(p) * 16u
            var v = float4(0.0)
            if (gn0 + row < cnt) {
                let ib = (row0 + gn0 + row) * nbb + kb
                v = float4(int4(unpack8(int(vk_xq[ib * 8u + lw])))) * vk_xs[ib]
            }
            let o = row * CML_SST + lw * 2u
            cml_b[o] = packHalf2x16(v.xy)
            cml_b[o + 1u] = packHalf2x16(v.zw)
        }
        barrier()
        for [unroll] (ks in range(2)) {   // 2 K-subtiles x (4 A rows x 4 B cols), their MMA order
            let k8 = uint(ks) * 8u
            var ca : coopmatA_f16_16x16
            var cb : coopmatB_f16_16x16
            coopmatLoad(ca, cml_a, int((warp_r * 64u) * CML_SST + k8), int(CML_SST), 0)
            coopmatLoad(cb, cml_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
            acc00 = coopmatMulAdd(ca, cb, acc00)
            coopmatLoad(cb, cml_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
            acc01 = coopmatMulAdd(ca, cb, acc01)
            coopmatLoad(cb, cml_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
            acc02 = coopmatMulAdd(ca, cb, acc02)
            coopmatLoad(cb, cml_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
            acc03 = coopmatMulAdd(ca, cb, acc03)
            coopmatLoad(ca, cml_a, int((warp_r * 64u + 16u) * CML_SST + k8), int(CML_SST), 0)
            coopmatLoad(cb, cml_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
            acc10 = coopmatMulAdd(ca, cb, acc10)
            coopmatLoad(cb, cml_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
            acc11 = coopmatMulAdd(ca, cb, acc11)
            coopmatLoad(cb, cml_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
            acc12 = coopmatMulAdd(ca, cb, acc12)
            coopmatLoad(cb, cml_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
            acc13 = coopmatMulAdd(ca, cb, acc13)
            coopmatLoad(ca, cml_a, int((warp_r * 64u + 32u) * CML_SST + k8), int(CML_SST), 0)
            coopmatLoad(cb, cml_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
            acc20 = coopmatMulAdd(ca, cb, acc20)
            coopmatLoad(cb, cml_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
            acc21 = coopmatMulAdd(ca, cb, acc21)
            coopmatLoad(cb, cml_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
            acc22 = coopmatMulAdd(ca, cb, acc22)
            coopmatLoad(cb, cml_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
            acc23 = coopmatMulAdd(ca, cb, acc23)
            coopmatLoad(ca, cml_a, int((warp_r * 64u + 48u) * CML_SST + k8), int(CML_SST), 0)
            coopmatLoad(cb, cml_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
            acc30 = coopmatMulAdd(ca, cb, acc30)
            coopmatLoad(cb, cml_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
            acc31 = coopmatMulAdd(ca, cb, acc31)
            coopmatLoad(cb, cml_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
            acc32 = coopmatMulAdd(ca, cb, acc32)
            coopmatLoad(cb, cml_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
            acc33 = coopmatMulAdd(ca, cb, acc33)
        }
        barrier()
        kb++
    }
    // stores: y[(row0 + token) * d + wrow] — fragment weight-dim contiguous = column-major, stride d
    let dr = gm0 + warp_r * 64u
    let dc = gn0 + warp_c * 64u
    if (gm0 + 128u <= d && gn0 + 128u <= cnt) {
        var accw : coopmatAcc_f32_16x16
        let yb = (row0 + dc) * d + dr
        coopmatConvert(accw, acc00)
        coopmatStore(accw, vk_y, int(yb), int(d), 1)
        coopmatConvert(accw, acc01)
        coopmatStore(accw, vk_y, int(yb + 16u * d), int(d), 1)
        coopmatConvert(accw, acc02)
        coopmatStore(accw, vk_y, int(yb + 32u * d), int(d), 1)
        coopmatConvert(accw, acc03)
        coopmatStore(accw, vk_y, int(yb + 48u * d), int(d), 1)
        coopmatConvert(accw, acc10)
        coopmatStore(accw, vk_y, int(yb + 16u), int(d), 1)
        coopmatConvert(accw, acc11)
        coopmatStore(accw, vk_y, int(yb + 16u * d + 16u), int(d), 1)
        coopmatConvert(accw, acc12)
        coopmatStore(accw, vk_y, int(yb + 32u * d + 16u), int(d), 1)
        coopmatConvert(accw, acc13)
        coopmatStore(accw, vk_y, int(yb + 48u * d + 16u), int(d), 1)
        coopmatConvert(accw, acc20)
        coopmatStore(accw, vk_y, int(yb + 32u), int(d), 1)
        coopmatConvert(accw, acc21)
        coopmatStore(accw, vk_y, int(yb + 16u * d + 32u), int(d), 1)
        coopmatConvert(accw, acc22)
        coopmatStore(accw, vk_y, int(yb + 32u * d + 32u), int(d), 1)
        coopmatConvert(accw, acc23)
        coopmatStore(accw, vk_y, int(yb + 48u * d + 32u), int(d), 1)
        coopmatConvert(accw, acc30)
        coopmatStore(accw, vk_y, int(yb + 48u), int(d), 1)
        coopmatConvert(accw, acc31)
        coopmatStore(accw, vk_y, int(yb + 16u * d + 48u), int(d), 1)
        coopmatConvert(accw, acc32)
        coopmatStore(accw, vk_y, int(yb + 32u * d + 48u), int(d), 1)
        coopmatConvert(accw, acc33)
        coopmatStore(accw, vk_y, int(yb + 48u * d + 48u), int(d), 1)
    } else {
        // edge tile: every fragment bounces its f16 acc through this warp's cml_o slab (rowmajor,
        // row = weight dim), then 32 lanes widen + write 8 elements each under bounds checks.
        // Value-identical to the direct arm (coopmatConvert only widens). Uniform across the wg.
        let lane = gl_SubgroupInvocationID
        let ob = gl_SubgroupID * 256u
        for (fi in range(16)) {
            let fr = uint(fi) / 4u       // fragment row: weight offset fr*16
            let fc = uint(fi) % 4u       // fragment col: token offset fc*16
            if (fi == 0) {
                coopmatStore(acc00, cml_o, int(ob), 16, 0)
            } elif (fi == 1) {
                coopmatStore(acc01, cml_o, int(ob), 16, 0)
            } elif (fi == 2) {
                coopmatStore(acc02, cml_o, int(ob), 16, 0)
            } elif (fi == 3) {
                coopmatStore(acc03, cml_o, int(ob), 16, 0)
            } elif (fi == 4) {
                coopmatStore(acc10, cml_o, int(ob), 16, 0)
            } elif (fi == 5) {
                coopmatStore(acc11, cml_o, int(ob), 16, 0)
            } elif (fi == 6) {
                coopmatStore(acc12, cml_o, int(ob), 16, 0)
            } elif (fi == 7) {
                coopmatStore(acc13, cml_o, int(ob), 16, 0)
            } elif (fi == 8) {
                coopmatStore(acc20, cml_o, int(ob), 16, 0)
            } elif (fi == 9) {
                coopmatStore(acc21, cml_o, int(ob), 16, 0)
            } elif (fi == 10) {
                coopmatStore(acc22, cml_o, int(ob), 16, 0)
            } elif (fi == 11) {
                coopmatStore(acc23, cml_o, int(ob), 16, 0)
            } elif (fi == 12) {
                coopmatStore(acc30, cml_o, int(ob), 16, 0)
            } elif (fi == 13) {
                coopmatStore(acc31, cml_o, int(ob), 16, 0)
            } elif (fi == 14) {
                coopmatStore(acc32, cml_o, int(ob), 16, 0)
            } else {
                coopmatStore(acc33, cml_o, int(ob), 16, 0)
            }
            barrier()
            for (e in range(8)) {
                let idx = lane * 8u + uint(e)
                let wr = dr + fr * 16u + idx / 16u
                let tc = dc + fc * 16u + idx % 16u
                if (wr < d && tc < cnt) {
                    vk_y[(row0 + tc) * d + wr] = float(cml_o[ob + idx])
                }
            }
            barrier()
        }
    }
}

// The aligned L twin: staging guards + edge branch removed — the guards serialize the unrolled
// staging loads (14.5 -> 31.2 TF on the gate shape), and even a never-taken guarded path costs
// ~20%, so like lcpp's _aligned pipelines this is its own pipeline (d + every cnt 128-multiples).

var @workgroup cma_a : uint[2560]     // A tile: 128 weight rows x one block (32 K) as f16 pairs
var @workgroup cma_b : uint[2560]     // B tile: 128 token rows x one block

[compute_shader(local_size_x=128, name="q8_moe_batch_mm_a_spv")]
def q8_moe_batch_mm_a {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let reg = vk_region_rec()
    let wblk0 = reg.x
    let row0 = reg.y
    let tix = reg.w
    let wtiles = (d + 127u) / 128u
    let xt = tix / wtiles             // token (B) tile
    let wt = tix % wtiles             // weight (A) tile
    let gm0 = wt * 128u
    let gn0 = xt * 128u
    let nbb = n / 32u
    let tid = gl_LocalInvocationID.x
    let warp_r = gl_SubgroupID % 2u   // which 64-row half of the A tile
    let warp_c = gl_SubgroupID / 2u   // which 64-col half of the B tile
    let lw = tid % 8u                 // which packed word (4 qs) of the staged row's block
    let lr0 = tid / 8u                // staging row base 0..15
    var acc00 : coopmatAcc_f16_16x16
    var acc01 : coopmatAcc_f16_16x16
    var acc02 : coopmatAcc_f16_16x16
    var acc03 : coopmatAcc_f16_16x16
    var acc10 : coopmatAcc_f16_16x16
    var acc11 : coopmatAcc_f16_16x16
    var acc12 : coopmatAcc_f16_16x16
    var acc13 : coopmatAcc_f16_16x16
    var acc20 : coopmatAcc_f16_16x16
    var acc21 : coopmatAcc_f16_16x16
    var acc22 : coopmatAcc_f16_16x16
    var acc23 : coopmatAcc_f16_16x16
    var acc30 : coopmatAcc_f16_16x16
    var acc31 : coopmatAcc_f16_16x16
    var acc32 : coopmatAcc_f16_16x16
    var acc33 : coopmatAcc_f16_16x16
    var kb = 0u
    while (kb < nbb) {
        for [unroll] (p in range(8)) {   // stage A: 128 weight rows x 1 block, unguarded
            let row = lr0 + uint(p) * 16u
            let ib = wblk0 + (gm0 + row) * nbb + kb
            let v = float4(int4(unpack8(int(vk_wq[ib * 8u + lw])))) * float(vk_ws[ib])
            let o = row * CML_SST + lw * 2u
            cma_a[o] = packHalf2x16(v.xy)
            cma_a[o + 1u] = packHalf2x16(v.zw)
        }
        for [unroll] (p in range(8)) {   // stage B: 128 token rows x 1 block, unguarded
            let row = lr0 + uint(p) * 16u
            let ib = (row0 + gn0 + row) * nbb + kb
            let v = float4(int4(unpack8(int(vk_xq[ib * 8u + lw])))) * vk_xs[ib]
            let o = row * CML_SST + lw * 2u
            cma_b[o] = packHalf2x16(v.xy)
            cma_b[o + 1u] = packHalf2x16(v.zw)
        }
        barrier()
        for [unroll] (ks in range(2)) {   // 2 K-subtiles x (4 A rows x 4 B cols), their MMA order
            let k8 = uint(ks) * 8u
            var ca : coopmatA_f16_16x16
            var cb : coopmatB_f16_16x16
            coopmatLoad(ca, cma_a, int((warp_r * 64u) * CML_SST + k8), int(CML_SST), 0)
            coopmatLoad(cb, cma_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
            acc00 = coopmatMulAdd(ca, cb, acc00)
            coopmatLoad(cb, cma_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
            acc01 = coopmatMulAdd(ca, cb, acc01)
            coopmatLoad(cb, cma_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
            acc02 = coopmatMulAdd(ca, cb, acc02)
            coopmatLoad(cb, cma_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
            acc03 = coopmatMulAdd(ca, cb, acc03)
            coopmatLoad(ca, cma_a, int((warp_r * 64u + 16u) * CML_SST + k8), int(CML_SST), 0)
            coopmatLoad(cb, cma_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
            acc10 = coopmatMulAdd(ca, cb, acc10)
            coopmatLoad(cb, cma_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
            acc11 = coopmatMulAdd(ca, cb, acc11)
            coopmatLoad(cb, cma_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
            acc12 = coopmatMulAdd(ca, cb, acc12)
            coopmatLoad(cb, cma_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
            acc13 = coopmatMulAdd(ca, cb, acc13)
            coopmatLoad(ca, cma_a, int((warp_r * 64u + 32u) * CML_SST + k8), int(CML_SST), 0)
            coopmatLoad(cb, cma_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
            acc20 = coopmatMulAdd(ca, cb, acc20)
            coopmatLoad(cb, cma_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
            acc21 = coopmatMulAdd(ca, cb, acc21)
            coopmatLoad(cb, cma_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
            acc22 = coopmatMulAdd(ca, cb, acc22)
            coopmatLoad(cb, cma_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
            acc23 = coopmatMulAdd(ca, cb, acc23)
            coopmatLoad(ca, cma_a, int((warp_r * 64u + 48u) * CML_SST + k8), int(CML_SST), 0)
            coopmatLoad(cb, cma_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
            acc30 = coopmatMulAdd(ca, cb, acc30)
            coopmatLoad(cb, cma_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
            acc31 = coopmatMulAdd(ca, cb, acc31)
            coopmatLoad(cb, cma_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
            acc32 = coopmatMulAdd(ca, cb, acc32)
            coopmatLoad(cb, cma_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
            acc33 = coopmatMulAdd(ca, cb, acc33)
        }
        barrier()
        kb++
    }
    // stores: y[(row0 + token) * d + wrow] — fragment weight-dim contiguous = column-major, stride d
    let dr = gm0 + warp_r * 64u
    let dc = gn0 + warp_c * 64u
    var accw : coopmatAcc_f32_16x16
    let yb = (row0 + dc) * d + dr
    coopmatConvert(accw, acc00)
    coopmatStore(accw, vk_y, int(yb), int(d), 1)
    coopmatConvert(accw, acc01)
    coopmatStore(accw, vk_y, int(yb + 16u * d), int(d), 1)
    coopmatConvert(accw, acc02)
    coopmatStore(accw, vk_y, int(yb + 32u * d), int(d), 1)
    coopmatConvert(accw, acc03)
    coopmatStore(accw, vk_y, int(yb + 48u * d), int(d), 1)
    coopmatConvert(accw, acc10)
    coopmatStore(accw, vk_y, int(yb + 16u), int(d), 1)
    coopmatConvert(accw, acc11)
    coopmatStore(accw, vk_y, int(yb + 16u * d + 16u), int(d), 1)
    coopmatConvert(accw, acc12)
    coopmatStore(accw, vk_y, int(yb + 32u * d + 16u), int(d), 1)
    coopmatConvert(accw, acc13)
    coopmatStore(accw, vk_y, int(yb + 48u * d + 16u), int(d), 1)
    coopmatConvert(accw, acc20)
    coopmatStore(accw, vk_y, int(yb + 32u), int(d), 1)
    coopmatConvert(accw, acc21)
    coopmatStore(accw, vk_y, int(yb + 16u * d + 32u), int(d), 1)
    coopmatConvert(accw, acc22)
    coopmatStore(accw, vk_y, int(yb + 32u * d + 32u), int(d), 1)
    coopmatConvert(accw, acc23)
    coopmatStore(accw, vk_y, int(yb + 48u * d + 32u), int(d), 1)
    coopmatConvert(accw, acc30)
    coopmatStore(accw, vk_y, int(yb + 48u), int(d), 1)
    coopmatConvert(accw, acc31)
    coopmatStore(accw, vk_y, int(yb + 16u * d + 48u), int(d), 1)
    coopmatConvert(accw, acc32)
    coopmatStore(accw, vk_y, int(yb + 32u * d + 48u), int(d), 1)
    coopmatConvert(accw, acc33)
    coopmatStore(accw, vk_y, int(yb + 48u * d + 48u), int(d), 1)
}

// The mul_mm M-tile twin (64x64, lcpp's cm1 m-warptile): the L scheme at half edge — 4 warps 2x2
// over the tile, 2x2 fragments each, 12.3KB shared. Routed in by q8_batch_tile_for when cnt or d
// <= 64: the L tile pads every such dispatch to 128 B-columns (MoE regions run ~32 rows).

var @workgroup cmm_a : uint[1280]     // A tile: 64 weight rows x one block (32 K) as f16 pairs
var @workgroup cmm_b : uint[1280]     // B tile: 64 token rows x one block

[compute_shader(local_size_x=128, name="q8_moe_batch_mm_m_spv")]
def q8_moe_batch_mm_m {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let reg = vk_region_rec()
    let wblk0 = reg.x
    let row0 = reg.y
    let cnt = reg.z
    let tix = reg.w
    let wtiles = (d + 63u) / 64u
    let xt = tix / wtiles             // token (B) tile
    let wt = tix % wtiles             // weight (A) tile
    let gm0 = wt * 64u
    let gn0 = xt * 64u
    let nbb = n / 32u
    let tid = gl_LocalInvocationID.x
    let warp_r = gl_SubgroupID % 2u   // which 32-row half of the A tile
    let warp_c = gl_SubgroupID / 2u   // which 32-col half of the B tile
    let lw = tid % 8u                 // which packed word (4 qs) of the staged row's block
    let lr0 = tid / 8u                // staging row base 0..15
    var acc00 : coopmatAcc_f16_16x16
    var acc01 : coopmatAcc_f16_16x16
    var acc10 : coopmatAcc_f16_16x16
    var acc11 : coopmatAcc_f16_16x16
    var kb = 0u
    while (kb < nbb) {
        for [unroll] (p in range(4)) {   // stage A: 64 weight rows x 1 block; OOR rows -> 0
            let row = lr0 + uint(p) * 16u
            let wrow = gm0 + row
            var v = float4(0.0)
            if (wrow < d) {
                let ib = wblk0 + wrow * nbb + kb
                v = float4(int4(unpack8(int(vk_wq[ib * 8u + lw])))) * float(vk_ws[ib])
            }
            let o = row * CML_SST + lw * 2u
            cmm_a[o] = packHalf2x16(v.xy)
            cmm_a[o + 1u] = packHalf2x16(v.zw)
        }
        for [unroll] (p in range(4)) {   // stage B: 64 token rows x 1 block; OOR rows -> 0
            let row = lr0 + uint(p) * 16u
            var v = float4(0.0)
            if (gn0 + row < cnt) {
                let ib = (row0 + gn0 + row) * nbb + kb
                v = float4(int4(unpack8(int(vk_xq[ib * 8u + lw])))) * vk_xs[ib]
            }
            let o = row * CML_SST + lw * 2u
            cmm_b[o] = packHalf2x16(v.xy)
            cmm_b[o + 1u] = packHalf2x16(v.zw)
        }
        barrier()
        for [unroll] (ks in range(2)) {   // 2 K-subtiles x (2 A rows x 2 B cols), the L MMA order
            let k8 = uint(ks) * 8u
            var ca : coopmatA_f16_16x16
            var cb : coopmatB_f16_16x16
            coopmatLoad(ca, cmm_a, int((warp_r * 32u) * CML_SST + k8), int(CML_SST), 0)
            coopmatLoad(cb, cmm_b, int((warp_c * 32u) * CML_SST + k8), int(CML_SST), 1)
            acc00 = coopmatMulAdd(ca, cb, acc00)
            coopmatLoad(cb, cmm_b, int((warp_c * 32u + 16u) * CML_SST + k8), int(CML_SST), 1)
            acc01 = coopmatMulAdd(ca, cb, acc01)
            coopmatLoad(ca, cmm_a, int((warp_r * 32u + 16u) * CML_SST + k8), int(CML_SST), 0)
            coopmatLoad(cb, cmm_b, int((warp_c * 32u) * CML_SST + k8), int(CML_SST), 1)
            acc10 = coopmatMulAdd(ca, cb, acc10)
            coopmatLoad(cb, cmm_b, int((warp_c * 32u + 16u) * CML_SST + k8), int(CML_SST), 1)
            acc11 = coopmatMulAdd(ca, cb, acc11)
        }
        barrier()
        kb++
    }
    // stores: y[(row0 + token) * d + wrow] — same column-major layout as the L tile
    let dr = gm0 + warp_r * 32u
    let dc = gn0 + warp_c * 32u
    if (gm0 + 64u <= d && gn0 + 64u <= cnt) {
        var accw : coopmatAcc_f32_16x16
        let yb = (row0 + dc) * d + dr
        coopmatConvert(accw, acc00)
        coopmatStore(accw, vk_y, int(yb), int(d), 1)
        coopmatConvert(accw, acc01)
        coopmatStore(accw, vk_y, int(yb + 16u * d), int(d), 1)
        coopmatConvert(accw, acc10)
        coopmatStore(accw, vk_y, int(yb + 16u), int(d), 1)
        coopmatConvert(accw, acc11)
        coopmatStore(accw, vk_y, int(yb + 16u * d + 16u), int(d), 1)
    } else {
        // edge tile: bounce each f16 acc through this warp's cml_o slab, bounds-checked write-out
        let lane = gl_SubgroupInvocationID
        let ob = gl_SubgroupID * 256u
        for (fi in range(4)) {
            let fr = uint(fi) / 2u       // fragment row: weight offset fr*16
            let fc = uint(fi) % 2u       // fragment col: token offset fc*16
            if (fi == 0) {
                coopmatStore(acc00, cml_o, int(ob), 16, 0)
            } elif (fi == 1) {
                coopmatStore(acc01, cml_o, int(ob), 16, 0)
            } elif (fi == 2) {
                coopmatStore(acc10, cml_o, int(ob), 16, 0)
            } else {
                coopmatStore(acc11, cml_o, int(ob), 16, 0)
            }
            barrier()
            for (e in range(8)) {
                let idx = lane * 8u + uint(e)
                let wr = dr + fr * 16u + idx / 16u
                let tc = dc + fc * 16u + idx % 16u
                if (wr < d && tc < cnt) {
                    vk_y[(row0 + tc) * d + wr] = float(cml_o[ob + idx])
                }
            }
            barrier()
        }
    }
}

// Q4_0 coopmat f16 variant (fmt=4 stacks): kq_moe_batch_q40 math baked into f16 dequant — w =
// (nib-8)*d_b, a = aq*xs (per-256 Q8_K) — then the same 4-subgroup MMA as q8_moe_batch_cmf16.
// No int8-native arm: nibbles would need dequant-to-s8 shared staging (byte stores, NV-slow).

[compute_shader(local_size_x=128, name="kq_moe_batch_q40_cmf16_spv")]
def kq_moe_batch_q40_cmf16 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let reg = vk_region_rec()
    let wsb0 = reg.x
    let row0 = reg.y
    let cnt = reg.z
    let tix = reg.w
    let wtiles = (d + 31u) / 32u
    let xt = tix / wtiles
    let wt = tix % wtiles
    let nsb = n / 256u
    let tid = gl_LocalInvocationID.x
    let sg = gl_SubgroupID
    let smi = sg / 2u
    let sni = sg % 2u
    var acc : coopmatAcc_f32_16x16
    var s = 0u
    while (s < nsb) {
        // stage A: 32 rows x 64 uints (one superblock of Q8_K acts), 16/thread — one thread stays in
        // one row (16 divides 64), so xs (per-256) loads ONCE per thread
        let arow0 = tid * 16u / 64u            // this thread's row
        let ac0 = tid * 16u % 64u              // first chunk within the row
        let axrow = xt * 32u + arow0
        var xs = 0.0
        if (axrow < cnt) {
            xs = vk_xs[(row0 + axrow) * nsb + s]
        }
        for (i in range(16)) {
            let c = ac0 + uint(i)
            let bu = c / 8u
            let ww = c % 8u
            var packed = 0u
            if (axrow < cnt) {
                packed = vk_xq[((row0 + axrow) * nsb + s) * 64u + c]
            }
            let qv = int4(unpack8(int(packed)))
            let o = arow0 * 256u + bu * 32u + ww * 4u
            cm_as[o] = float16(float(qv.x) * xs)
            cm_as[o + 1u] = float16(float(qv.y) * xs)
            cm_as[o + 2u] = float16(float(qv.z) * xs)
            cm_as[o + 3u] = float16(float(qv.w) * xs)
        }
        // stage B: 32 cols x 32 uints (one superblock of nibble-packed weights), 8/thread; each uint
        // yields 4 lo-nibble weights (k = bu*32 + w*4 + j) and 4 hi (k + 16), dequant (nib-8)*d_b
        for (i in range(8)) {
            let u2 = tid * 8u + uint(i)
            let col = u2 / 32u
            let c = u2 % 32u
            let bu = c / 4u
            let w = c % 4u
            let wcol = wt * 32u + col
            var r = 0u
            var db = 0.0
            if (wcol < d) {
                let wsb = wsb0 + wcol * nsb + s
                r = vk_wq[wsb * 32u + c]
                let dp = unpackHalf2x16(vk_wsu[wsb * 5u + bu / 2u])
                db = bu % 2u == 0u ? dp.x : dp.y
            }
            let k = bu * 32u + w * 4u
            for (j in range(4)) {
                let lo = int((r >> (uint(j) * 8u)) & 0x0Fu)
                let hi = int((r >> (uint(j) * 8u + 4u)) & 0x0Fu)
                cm_bs[(k + uint(j)) * 32u + col] = float16(float(lo - 8) * db)
                cm_bs[(k + 16u + uint(j)) * 32u + col] = float16(float(hi - 8) * db)
            }
        }
        barrier()
        for (bb in range(8)) {
            for (ks in range(2)) {
                var a : coopmatA_f16_16x16
                var b : coopmatB_f16_16x16
                coopmatLoad(a, cm_as, int(smi * 16u * 256u + uint(bb) * 32u + uint(ks) * 16u), 256, 0)
                coopmatLoad(b, cm_bs, int((uint(bb) * 32u + uint(ks) * 16u) * 32u + sni * 16u), 32, 0)
                acc = coopmatMulAdd(a, b, acc)
            }
        }
        barrier()
        s++
    }
    coopmatStore(acc, cm_out, int(sg * 256u), 16, 0)
    barrier()
    let lane = gl_SubgroupInvocationID
    for (e in range(8)) {
        let idx = lane * 8u + uint(e)
        let m = idx / 16u
        let nn = idx % 16u
        let orow = xt * 32u + smi * 16u + m
        let ocol = wt * 32u + sni * 16u + nn
        if (orow < cnt && ocol < d) {
            vk_y[(row0 + orow) * d + ocol] = cm_out[sg * 256u + idx]
        }
    }
}

// ===== the cm2 (NV_cooperative_matrix2) prefill twin =====
// Mode 4, fmt-6 q8n stacks: 34B blocks decoded IN-LOAD, wg-scope tensor tiles, f16 activations.
// P5a winner geometry (n2x256); layout dims bound every access — edge tiles clamp, no guard code.

struct VkBlockQ8N {
    d : float16
    qs : uint16[16]
}

var @ssbo @binding = 0 vk_wqn : array<VkBlockQ8N>
var @ssbo @binding = 0 vk_wq16 : array<uint16>    // u16 view of the same blocks (17/blk — the GEMV path)
var @ssbo @binding = 3 vk_xf16 : array<float16>   // f16 activation plane (the twin's B side)
var @ssbo @binding = 0 vk_of16 : array<float16>   // f16 output view (the dequant convert kernel)

[spirv_decode, unused_argument(bc)]
def decode_q8n(blk : VkBlockQ8N; bc, cib : uint2) : float16 {
    let s = uint(blk.qs[int((cib.y & 30u) >> 1u)])
    let sh = 24 - int(cib.y & 1u) * 8
    let q = (int(s) << sh) >> 24
    return blk.d * float16(float(q))
}

[compute_shader(local_size_x=256, name="q8n_moe_batch_cm2_spv")]
def q8n_moe_batch_cm2 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let reg = vk_region_rec()
    let wblk0 = reg.x
    let row0 = reg.y
    let cnt = reg.z
    let tix = reg.w
    let ttiles = (cnt + 127u) / 128u
    let xt = tix % ttiles             // token 128-tile (fastest)
    let wt = tix / ttiles             // weight 64-tile
    // A: the region's [d x n] q8n plane — block addressing, decode-in-load
    var tla : tensorLayout2DPad
    tensorLayoutCreate(tla)
    tensorLayoutSetBlockSize(tla, 1u, 32u)
    tensorLayoutSetDimension(tla, d, n)
    tensorLayoutSetStride(tla, n / 32u, 1u)
    // B: the f16 activation rows, transpose-viewed; the dimension cuts at the region's end
    var tlb : tensorLayout2DPad
    tensorLayoutCreate(tlb)
    tensorLayoutSetDimension(tlb, row0 + cnt, n)
    tensorLayoutSetStride(tlb, n, 1u)
    // out: token-major y through the transpose view. MUST be the clamp-Constant layout type:
    // only clamped layouts DISCARD out-of-bounds stores — an Undefined-clamp store from a
    // partial token tile stomps the neighboring region's rows (lcpp's store type is clamp 1 too)
    var tlo : tensorLayout2DPad
    tensorLayoutCreate(tlo)
    tensorLayoutSetDimension(tlo, row0 + cnt, d)
    tensorLayoutSetStride(tlo, d, 1u)
    var tv : tensorView2Dt
    tensorViewCreate(tv)
    var a : coopmatWgA_f16_64x16
    var b0 : coopmatWgB_f16_16x64
    var b1 : coopmatWgB_f16_16x64
    var acc0 : coopmatWgAcc_f16_64x64
    var acc1 : coopmatWgAcc_f16_64x64
    let t0 = row0 + xt * 128u
    var k = 0u
    while (k < n) {
        coopmatLoadTensorDecode(a, vk_wqn, wblk0, tla, wt * 64u, 64u, k, 16u, @@decode_q8n)
        coopmatLoadTensor(b0, vk_xf16, 0u, tlb, t0, 64u, k, 16u, tv)
        coopmatLoadTensor(b1, vk_xf16, 0u, tlb, t0 + 64u, 64u, k, 16u, tv)
        acc0 = coopmatMulAdd(a, b0, acc0)
        acc1 = coopmatMulAdd(a, b1, acc1)
        k += 16u
    }
    coopmatClamp(acc0, -65504.0, 65504.0)
    coopmatClamp(acc1, -65504.0, 65504.0)
    var accw0 : coopmatWgAcc_f32_64x64
    coopmatConvert(accw0, acc0)
    coopmatStoreTensor(accw0, vk_y, 0u, tlo, t0, 64u, wt * 64u, 64u, tv)
    var accw1 : coopmatWgAcc_f32_64x64
    coopmatConvert(accw1, acc1)
    coopmatStoreTensor(accw1, vk_y, 0u, tlo, t0 + 64u, 64u, wt * 64u, 64u, tv)
}

// the q8n decode GEMV: q8_moe_groupn's region skeleton on the 34B blocks (P4's L2 arm — 17 u16
// per block, u16-pair loads combined to words, 8 lanes per block, d broadcast-read from u16[0]).
// Activations stay Q8_0 — decode never leaves the int-dot path, only the weight addressing moves.
[compute_shader(local_size_x=64, local_size_x_id=0, name="q8n_moe_groupn_spv")]
def q8n_moe_groupn {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let nreg = vk_meta[2u]
    let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
    if (rg < nreg * d) {
        let r = rg / d
        let row = rg % d
        let nb = n / 32u
        let wb0 = vk_meta[4u + r * 2u] + row * nb
        let xb0 = vk_meta[5u + r * 2u]
        let lane = gl_SubgroupInvocationID
        let my_word = lane % 8u
        let my_boff = lane / 8u
        let bpp = gl_SubgroupSize / 8u
        var acc = 0.0
        var bi0 = 0u
        while (bi0 < nb) {
            let b = bi0 + my_boff
            var idot = 0
            var dbits = 0u
            if (b < nb) {
                let base = (wb0 + b) * 17u
                let lo = uint(vk_wq16[base + 1u + my_word * 2u])
                let hi = uint(vk_wq16[base + 2u + my_word * 2u])
                idot = sdot4(lo | (hi << 16u), vk_xq[(xb0 + b) * 8u + my_word])
                dbits = uint(vk_wq16[base])
            }
            idot += subgroupShuffleXor(idot, 1u)
            idot += subgroupShuffleXor(idot, 2u)
            idot += subgroupShuffleXor(idot, 4u)
            if (my_word == 0u && b < nb) {
                acc += float(idot) * unpackHalf2x16(dbits).x * vk_xs[xb0 + b]
            }
            bi0 += bpp
        }
        let total = subgroupAdd(acc)
        if (subgroupElect()) {
            vk_y[vk_meta[3u] + rg] = total
        }
    }
}

// the cm2 chain's activation feed: dequantize the uploaded Q8_0 image to the f16 plane the twin
// loads as B. One uint word (4 quants) per thread; ffn-meta word 3 carries the word count.
[compute_shader(local_size_x=256, name="q8_moe_xf16_spv")]
def q8_moe_xf16 {
    let w = gl_GlobalInvocationID.x
    if (w < vk_meta[3u]) {
        let v = int(vk_xq[w])
        let s = vk_xs[w / 8u]
        vk_of16[w * 4u] = float16(float((v << 24) >> 24) * s)
        vk_of16[w * 4u + 1u] = float16(float((v << 16) >> 24) * s)
        vk_of16[w * 4u + 2u] = float16(float((v << 8) >> 24) * s)
        vk_of16[w * 4u + 3u] = float16(float(v >> 24) * s)
    }
}

// ===== the FFN mid-chain kernels =====
// ONE fused act+requant step between gate+up and down GEMMs — hidden activations never cross PCIe.
// Binding 0 doubles as a float view here (vk_upf) vs uint weights in the GEMMs — SPIR-V only emits referenced globals.

var @ssbo @binding = 0 vk_upf : array<float>

// g = act(g) * u — the SwiGLU/GeGLU elementwise core (gelu taken as uint: the emitter's
// scalar params are numeric)
def private act_mul(v, u : float; gelu : uint) : float => (
    gelu != 0u ? (0.5 * v * (1.0 + tanh(0.7978845608028654 * (v + 0.044715 * v * v * v)))) * u
        : (v / (1.0 + exp(-v))) * u)

// 8-lane Q8_0 block quantize: xor-1/2/4 amax fold over the block's 8 co-active lanes (must be
// 8-aligned), d to vk_xs[b], returns this lane's packed word — the caller stores it to its plane
def private q8_blk_pack(b, lane8 : uint; v0, v1, v2, v3 : float) : uint {
    var m = max(max(abs(v0), abs(v1)), max(abs(v2), abs(v3)))
    m = max(m, subgroupShuffleXor(m, 1u))
    m = max(m, subgroupShuffleXor(m, 2u))
    m = max(m, subgroupShuffleXor(m, 4u))
    let d = m / 127.0
    let id = d != 0.0 ? 1.0 / d : 0.0
    if (lane8 == 0u) {
        vk_xs[b] = d
    }
    let q0 = int(round(v0 * id))
    let q1 = int(round(v1 * id))
    let q2 = int(round(v2 * id))
    let q3 = int(round(v3 * id))
    return ((uint(q0) & 0xFFu) | ((uint(q1) & 0xFFu) << 8u)
        | ((uint(q2) & 0xFFu) << 16u) | ((uint(q3) & 0xFFu) << 24u))
}

// whole-subgroup Q8_K amax: 5-shuffle fold, d to vk_xs[b], returns id (the quantize multiplier)
def private q8k_blk_scale(b, lane : uint; m0 : float) : float {
    var m = m0
    m = max(m, subgroupShuffleXor(m, 1u))
    m = max(m, subgroupShuffleXor(m, 2u))
    m = max(m, subgroupShuffleXor(m, 4u))
    m = max(m, subgroupShuffleXor(m, 8u))
    m = max(m, subgroupShuffleXor(m, 16u))
    let d = m / 127.0
    if (lane == 0u) {
        vk_xs[b] = d
    }
    return d != 0.0 ? 1.0 / d : 0.0
}

// butterfly byte pack of one quantized round: lanes 4j..4j+3 fold into the superblock's word j
def private q8k_pack_word(b, r, lane : uint; v, id : float) {
    let q = int(round(v * id))
    let b8 = uint(q) & 0xFFu
    let p = b8 | (subgroupShuffleXor(b8, 1u) << 8u)
    let w = p | (subgroupShuffleXor(p, 2u) << 16u)
    if (lane % 4u == 0u) {
        vk_xq[b * 64u + r * 8u + lane / 4u] = w
    }
}

// fused act + q8_0 requant of the hidden rows (gate floats in vk_y, up in vk_upf -> vk_xq
// words + vk_xs per-32 scales). Subgroup per 4 blocks, word per lane: coalesced loads,
// single-pass act, 8-lane shuffle amax (xor 1/2/4); bit-identical to the thread-per-block form.
[compute_shader(local_size_x=256, name="q8_moe_actrq_spv")]
def q8_moe_actrq {
    let gid = gl_GlobalInvocationID.x
    let sg = gid / 32u
    let lane = gid % 32u
    let b = sg * 4u + lane / 8u
    if (b < vk_meta[2u]) {
        let gelu = vk_meta[1u]
        let eb = sg * 128u + lane * 4u
        let v0 = act_mul(vk_y[eb], vk_upf[eb], gelu)
        let v1 = act_mul(vk_y[eb + 1u], vk_upf[eb + 1u], gelu)
        let v2 = act_mul(vk_y[eb + 2u], vk_upf[eb + 2u], gelu)
        let v3 = act_mul(vk_y[eb + 3u], vk_upf[eb + 3u], gelu)
        vk_xq[b * 8u + lane % 8u] = q8_blk_pack(b, lane % 8u, v0, v1, v2, v3)
    }
}

// the Q8_K twin for the K-quant chains: one SUBGROUP per 256-weight superblock, per-256 scale.
// 8 coalesced 128B rounds per lane, whole-subgroup 5-shuffle amax, butterfly byte pack (the old
// thread-per-superblock form strode 1KB per lane = ~21x slower). Bit-identical to the old form.
[compute_shader(local_size_x=256, name="q8k_moe_actrq_spv")]
def q8k_moe_actrq {
    let gid = gl_GlobalInvocationID.x
    let b = gid / 32u
    let lane = gid % 32u
    if (b < vk_meta[2u]) {
        let gelu = vk_meta[1u]
        let base = b * 256u
        var vals : float[8]
        var m = 0.0
        for [unroll] (r in range(8)) {
            let e = base + uint(r) * 32u + lane
            let v = act_mul(vk_y[e], vk_upf[e], gelu)
            vals[r] = v
            m = max(m, abs(v))
        }
        let id = q8k_blk_scale(b, lane, m)
        for [unroll] (r in range(8)) {
            q8k_pack_word(b, uint(r), lane, vals[r], id)
        }
    }
}

// the cm2 chain's mid-step: act(gate) * up straight to the f16 hidden plane the down twin loads
// as B — no quantization on this route (same bindings as actrq: up@0, params@2, out@3, gate@5).
[compute_shader(local_size_x=256, name="q8_moe_actf16_spv")]
def q8_moe_actf16 {
    let e = gl_GlobalInvocationID.x * 4u
    if (e < vk_meta[0u]) {
        let gelu = vk_meta[1u]
        vk_xf16[e] = float16(act_mul(vk_y[e], vk_upf[e], gelu))
        vk_xf16[e + 1u] = float16(act_mul(vk_y[e + 1u], vk_upf[e + 1u], gelu))
        vk_xf16[e + 2u] = float16(act_mul(vk_y[e + 2u], vk_upf[e + 2u], gelu))
        vk_xf16[e + 3u] = float16(act_mul(vk_y[e + 3u], vk_upf[e + 3u], gelu))
    }
}

// the arena cm2 chain's activation feed: straight f32 -> f16 rows for the twin's B side — the
// resident prefill's quant-role stand-in (no quantize on the fmt-6 route). Meta word 0 carries
// the element count (always a 32-multiple, so the 4-wide guard is exact).
[compute_shader(local_size_x=256, name="rd_f16cvt_spv")]
def rd_f16cvt {
    let e = gl_GlobalInvocationID.x * 4u
    if (e < vk_meta[0u]) {
        vk_xf16[e] = float16(vk_upf[e])
        vk_xf16[e + 1u] = float16(vk_upf[e + 1u])
        vk_xf16[e + 2u] = float16(vk_upf[e + 2u])
        vk_xf16[e + 3u] = float16(vk_upf[e + 3u])
    }
}

// ===== the combine kernel =====
// One workgroup per output POSITION, summing its k slots' down-GEMM rows ascending j (matches
// the decode accumulation order); r0 == 0 starts the sum, chunk-spanning positions accumulate across dispatches.
[compute_shader(local_size_x=256, name="moe_combine_spv")]
def moe_combine {
    let p = gl_WorkGroupID.x
    let n = vk_meta[4u]
    let k = vk_meta[5u]
    let r0 = vk_meta[6u]
    let r1 = vk_meta[7u]
    var i = gl_LocalInvocationID.x
    while (i < n) {
        var acc = r0 == 0u ? 0.0 : vk_upf[p * n + i]
        var j = 0u
        while (j < k) {
            let jb = vk_xq[p * k + j]
            if (jb >= r0 && jb < r1) {
                acc += vk_xs[p * k + j] * vk_y[(jb - r0) * n + i]
            }
            j++
        }
        vk_upf[p * n + i] = acc
        i += 256u
    }
}

// ===== the deltanet kernels =====
// dn_conv (depthwise conv + SiLU + qk L2-norm) then the two-phase chunked delta-rule scan (dn_scan_p1
// parallel over head x chunk, dn_scan_p2 sequential per head). Drift-class numerics vs the CPU path.

var @ssbo @binding = 1 vk_wsf : array<float>   // float view: the persistent per-head f32 state
var @ssbo @binding = 3 vk_xqf : array<float>   // float view: the DN_WS panel/o scratch
var @ssbo @binding = 5 vk_yq : array<uint>     // uint view of the y binding: the fused ar+requant's quant words

// conv output rows (f32, the scan's input) ride binding 0's float view (vk_upf)

// conv shared: the whole position row staged in shared (silu'd), plus its q/k block scales —
// every handoff stays in shared, each global output element has exactly one writer
var @workgroup cv_row : float[8192]
var @workgroup cv_scale : float[64]

[compute_shader(local_size_x=256, name="dn_conv_spv")]
def dn_conv {
    let p = gl_WorkGroupID.x
    let cd = vk_meta[1u]
    let kd = vk_meta[2u]
    let dconv = vk_meta[7u]
    let taps = dconv - 1u
    let hist_mode = vk_meta[8u]
    let taps_off = vk_meta[13u]
    let hist_off = vk_meta[17u]
    let ds = vk_meta[6u]
    let eps = vk_xs[0u]
    let qscale = vk_xs[1u]
    let tid = gl_LocalInvocationID.x
    // depthwise conv + SiLU into shared, strided channels; taps in the raw fblob layout (ch x dconv)
    var ch = tid
    while (ch < cd) {
        var acc = 0.0
        var k = 0u
        while (k < dconv) {
            let sp = int(p + k) - int(taps)
            var xv = 0.0
            if (sp >= 0) {
                xv = vk_y[uint(sp) * cd + ch]
            } elif (hist_mode == 1u) {
                xv = vk_xs[hist_off + ch * taps + p + k]
            } else {
                xv = vk_xs[hist_off + (p + k) * cd + ch]
            }
            acc += xv * vk_xs[taps_off + ch * dconv + k]
            k++
        }
        cv_row[ch] = acc / (1.0 + exp(-acc))
        ch += 256u
    }
    barrier()
    // per-head L2 norm scale over the q and k regions (2*kd channels in ds-blocks), q pre-scale
    // folded in — one thread per block, f32 square-sum (CPU accumulates double: drift-class)
    let nblk = (2u * kd) / ds
    if (tid < nblk) {
        var sum = 0.0
        var j = 0u
        while (j < ds) {
            let v = cv_row[tid * ds + j]
            sum += v * v
            j++
        }
        cv_scale[tid] = (1.0 / max(sqrt(sum), eps)) * (tid < kd / ds ? qscale : 1.0)
    }
    barrier()
    ch = tid
    while (ch < cd) {
        let s = ch < 2u * kd ? cv_scale[ch / ds] : 1.0
        vk_upf[p * cd + ch] = cv_row[ch] * s
        ch += 256u
    }
}

// the decode-step FUSED chain (one workgroup per v-head): conv preamble -> chunk-length-1 delta
// rule IN PLACE on the persistent state -> gated out-norm -> the head's Q8_0 requant. The conv
// ring is parity-double-buffered: q/k channels have nvh/nkh reader heads, so readers stay on one image while owner heads write the other.

var @workgroup dsf_q : float[128]
var @workgroup dsf_k : float[128]
var @workgroup dsf_v : float[128]
var @workgroup dsf_vn : float[128]
var @workgroup dsf_o : float[128]
var @workgroup dsf_s : float[8]

[compute_shader(local_size_x=256, name="dn_step_fused_spv")]
def dn_step_fused {
    let h = gl_WorkGroupID.x
    let cd = vk_meta[1u]
    let kd = vk_meta[2u]
    let nkh = vk_meta[5u]
    let ds = vk_meta[6u]
    let dconv = vk_meta[7u]
    let taps = dconv - 1u
    let beta_off = vk_meta[11u]
    let g_off = vk_meta[12u]
    let taps_off = vk_meta[13u]
    let wnorm_off = vk_meta[14u]
    let a_off = vk_meta[15u]
    let dt_off = vk_meta[16u]
    let hist_off = vk_meta[17u]
    let eps = vk_xs[0u]
    let qscale = vk_xs[1u]
    let par = uint(int(vk_xs[2u]))
    let rrb = hist_off + par * cd * taps
    let wrb = hist_off + (1u - par) * cd * taps
    let tid = gl_LocalInvocationID.x
    let kh = h % nkh
    var e = tid
    while (e < 3u * ds) {
        let sl = e / ds
        let j = e % ds
        let ch = (sl == 0u ? kh * ds : (sl == 1u ? kd + kh * ds : 2u * kd + h * ds)) + j
        let fresh = vk_y[ch]
        var acc = fresh * vk_xs[taps_off + ch * dconv + taps]
        var k = 0u
        while (k < taps) {
            acc += vk_xs[rrb + ch * taps + k] * vk_xs[taps_off + ch * dconv + k]
            k++
        }
        if (sl == 2u || h < nkh) {   // this head owns the channel's ring advance
            k = 0u
            while (k + 1u < taps) {
                vk_xs[wrb + ch * taps + k] = vk_xs[rrb + ch * taps + k + 1u]
                k++
            }
            vk_xs[wrb + ch * taps + taps - 1u] = fresh
        }
        let sv = acc / (1.0 + exp(-acc))
        if (sl == 0u) {
            dsf_q[j] = sv
        } elif (sl == 1u) {
            dsf_k[j] = sv
        } else {
            dsf_v[j] = sv
        }
        e += 256u
    }
    barrier()
    // q/k L2 scales + the per-token beta/g transforms — tiny serial lanes (ds is 128-class)
    if (tid < 2u) {
        var sum = 0.0
        var j = 0u
        while (j < ds) {
            let v = tid == 0u ? dsf_q[j] : dsf_k[j]
            sum += v * v
            j++
        }
        dsf_s[tid] = (1.0 / max(sqrt(sum), eps)) * (tid == 0u ? qscale : 1.0)
    } elif (tid == 2u) {
        let graw = vk_xs[g_off + h] + vk_xs[dt_off + h]
        dsf_s[2u] = exp(vk_xs[a_off + h] * (graw > 20.0 ? graw : log(1.0 + exp(graw))))
        dsf_s[3u] = 1.0 / (1.0 + exp(-vk_xs[beta_off + h]))
    }
    barrier()
    e = tid
    while (e < ds) {
        dsf_q[e] = dsf_q[e] * dsf_s[0u]
        dsf_k[e] = dsf_k[e] * dsf_s[1u]
        e += 256u
    }
    barrier()
    if (tid == 0u) {
        var qk = 0.0
        var j = 0u
        while (j < ds) {
            qk += dsf_q[j] * dsf_k[j]
            j++
        }
        dsf_s[4u] = qk
    }
    barrier()
    // the delta rule at chunk length 1, state in place: vnew = b (v - e^g (k.S));
    // o = e^g (q.S) + (q.k) vnew; S = e^g S + kT (x) vnew
    let sbase = h * ds * ds
    let eg = dsf_s[2u]
    let bt = dsf_s[3u]
    if (tid < ds) {
        var ks = 0.0
        var qs = 0.0
        var i = 0u
        while (i < ds) {
            let sv = vk_wsf[sbase + i * ds + tid]
            ks += dsf_k[i] * sv
            qs += dsf_q[i] * sv
            i++
        }
        let vn = bt * (dsf_v[tid] - eg * ks)
        dsf_vn[tid] = vn
        dsf_o[tid] = eg * qs + dsf_s[4u] * vn
    }
    barrier()
    e = tid
    while (e < ds * ds) {
        vk_wsf[sbase + e] = vk_wsf[sbase + e] * eg + dsf_k[e / ds] * dsf_vn[e % ds]
        e += 256u
    }
    if (tid == 0u) {
        var sum = 0.0
        var j = 0u
        while (j < ds) {
            sum += dsf_o[j] * dsf_o[j]
            j++
        }
        dsf_s[5u] = 1.0 / sqrt(sum / float(ds) + eps)
    }
    barrier()
    e = tid
    while (e < ds) {
        let zv = vk_y[cd + h * ds + e]
        let xn = dsf_o[e] * dsf_s[5u] * vk_xs[wnorm_off + e]
        dsf_o[e] = xn * (zv / (1.0 + exp(-zv)))
        e += 256u
    }
    barrier()
    // the head's Q8_0 blocks (ds % 32 asserted host-side): scales to binding 0, quants to 3
    if (tid < ds / 32u) {
        let gb = (h * ds) / 32u + tid
        let jb = tid * 32u
        var amax = 0.0
        for (i in range(32)) {
            amax = max(amax, abs(dsf_o[jb + uint(i)]))
        }
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        vk_upf[gb] = d
        for (wi in range(8)) {
            let wj = jb + uint(wi) * 4u
            var word = 0u
            for (j in range(4)) {
                let q = int(round(dsf_o[wj + uint(j)] * id))
                word = word | ((uint(q) & 0xFF) << (uint(j) * 8u))
            }
            vk_xq[gb * 8u + uint(wi)] = word
        }
    }
}

// scan shared: the 64x64 unit-lower solve tile + the GEMM staging tiles (16-deep K)
var @workgroup st_tm : float[4096]
var @workgroup st_ta : float[2048]
var @workgroup st_tb : float[2048]

// per-invocation C accumulator (Private storage): max outputs/thread = ds*ds / 256 = 64
var @private_global dg_acc : float[64]

// C[M x N] (+)= A[M x K] * B[K x N] over DN_WS floats; C must not alias A or B. Operand visibility is
// the CALLER's job (a buffer sync before the call); the trailing sync makes C available to the next
// phase. C rides per-thread registers across the K stages (same add order as through-memory — bit-identical).
def private dn_gemm(cofs, aofs, bofs : uint; mm, kk, nn : uint; lda, ldb, ldc : uint; accum : uint) {
    let tid = gl_LocalInvocationID.x
    var o = tid
    var oi = 0u
    while (o < mm * nn) {
        dg_acc[oi] = accum == 0u ? 0.0 : vk_xqf[cofs + (o / nn) * ldc + o % nn]
        oi++
        o += 256u
    }
    var kb = 0u
    while (kb < kk) {
        let kt = min(kk - kb, 16u)
        var i = tid
        while (i < mm * kt) {
            st_ta[(i / kt) * 16u + i % kt] = vk_xqf[aofs + (i / kt) * lda + kb + i % kt]
            i += 256u
        }
        i = tid
        while (i < kt * nn) {
            st_tb[(i / nn) * 128u + i % nn] = vk_xqf[bofs + (kb + i / nn) * ldb + i % nn]
            i += 256u
        }
        barrier()
        o = tid
        oi = 0u
        while (o < mm * nn) {
            let r = o / nn
            let c = o % nn
            var acc = dg_acc[oi]
            var k2 = 0u
            while (k2 < kt) {
                acc += st_ta[r * 16u + k2] * st_tb[k2 * 128u + c]
                k2++
            }
            dg_acc[oi] = acc
            oi++
            o += 256u
        }
        barrier()
        kb += 16u
    }
    o = tid
    oi = 0u
    while (o < mm * nn) {
        vk_xqf[cofs + (o / nn) * ldc + o % nn] = dg_acc[oi]
        oi++
        o += 256u
    }
    memoryBarrierBuffer()
    barrier()
}

// The scan splits over the matrix-linear state recurrence S' = e^glast S + kgT (vnloc + w S): phase 1
// computes every state-independent piece per (head x chunk) at nvh*nchunks workgroups; phase 2 walks
// chunks sequentially with only the 4 state-dependent GEMMs + out-norm (one workgroup per v-head).
[compute_shader(local_size_x=256, name="dn_scan_p1_spv")]
def dn_scan_p1 {
    let npos = vk_meta[0u]
    let cd = vk_meta[1u]
    let kd = vk_meta[2u]
    let nvh = vk_meta[4u]
    let nkh = vk_meta[5u]
    let ds = vk_meta[6u]
    let beta_off = vk_meta[11u]
    let g_off = vk_meta[12u]
    let a_off = vk_meta[15u]
    let dt_off = vk_meta[16u]
    let tid = gl_LocalInvocationID.x
    let CS = 64u
    let nch = (npos + CS - 1u) / CS
    let wid = gl_WorkGroupID.x
    let h = wid / nch
    let c0 = (wid % nch) * CS
    let cl = min(CS, npos - c0)
    // slab regions (floats, per head x chunk)
    let slab = vk_meta[9u] * wid
    let qc = slab
    let kcT = slab + CS * ds
    let kb = slab + 2u * CS * ds
    let vb = slab + 3u * CS * ds
    let w = slab + 4u * CS * ds
    let vnew = slab + 5u * CS * ds
    let kgT = slab + 6u * CS * ds
    let tmg = slab + 7u * CS * ds
    let kq = tmg + CS * CS
    let gcum = kq + CS * CS
    let egc = gcum + CS
    let kgc = gcum + 2u * CS
    let bvec = gcum + 3u * CS
    let kh = h % nkh
    let qoff = kh * ds
    let koff = kd + kh * ds
    let voff = 2u * kd + h * ds
    let aa = vk_xs[a_off + h]
    let dtb = vk_xs[dt_off + h]
    // per-token transforms: beta = sigmoid(raw), g-cumsum of a*softplus(raw + dt) —
    // sequential scan, one thread (the other lanes wait at the barrier)
    if (tid == 0u) {
        var gacc = 0.0
        var t = 0u
        while (t < cl) {
            let graw = vk_xs[g_off + (c0 + t) * nvh + h] + dtb
            gacc += aa * (graw > 20.0 ? graw : log(1.0 + exp(graw)))
            vk_xqf[gcum + t] = gacc
            vk_xqf[egc + t] = exp(gacc)
            vk_xqf[bvec + t] = 1.0 / (1.0 + exp(-vk_xs[beta_off + (c0 + t) * nvh + h]))
            t++
        }
    }
    memoryBarrierBuffer()
    barrier()
    // panels off the conv rows: qc/kb/vb row-major cl x ds, kcT transposed ds x CS
    var e = tid
    while (e < cl * ds) {
        let i = e / ds
        let j = e % ds
        let crow = (c0 + i) * cd
        let kv = vk_upf[crow + koff + j]
        let bv = vk_xqf[bvec + i]
        vk_xqf[qc + i * ds + j] = vk_upf[crow + qoff + j]
        vk_xqf[kcT + j * CS + i] = kv
        vk_xqf[kb + i * ds + j] = kv * bv
        vk_xqf[vb + i * ds + j] = vk_upf[crow + voff + j] * bv
        e += 256u
    }
    memoryBarrierBuffer()
    barrier()
    // A = (k_beta k^T) decay-scaled strictly lower (negated); KQ keeps its diagonal
    dn_gemm(tmg, kb, kcT, cl, ds, cl, ds, CS, CS, 0u)
    dn_gemm(kq, qc, kcT, cl, ds, cl, ds, CS, CS, 0u)
    e = tid
    while (e < cl * cl) {
        let i = e / cl
        let j = e % cl
        if (j < i) {
            let d = exp(vk_xqf[gcum + i] - vk_xqf[gcum + j])
            vk_xqf[tmg + i * CS + j] = -vk_xqf[tmg + i * CS + j] * d
            vk_xqf[kq + i * CS + j] = vk_xqf[kq + i * CS + j] * d
        } elif (j == i) {
            vk_xqf[tmg + i * CS + j] = 1.0
        } else {
            vk_xqf[tmg + i * CS + j] = 0.0
            vk_xqf[kq + i * CS + j] = 0.0
        }
        e += 256u
    }
    memoryBarrierBuffer()
    barrier()
    // unit-lower solve in shared: T[i][j] = A'[i][j] + sum_{tt in (j,i)} A'[i][tt] T[tt][j],
    // rows finalized top-down (phase 1 computes from originals + final rows, phase 2 writes)
    e = tid
    while (e < cl * cl) {
        st_tm[(e / cl) * CS + e % cl] = vk_xqf[tmg + (e / cl) * CS + e % cl]
        e += 256u
    }
    barrier()
    var i2 = 1u
    while (i2 < cl) {
        var fin = 0.0
        if (tid < i2) {
            fin = st_tm[i2 * CS + tid]
            var t2 = tid + 1u
            while (t2 < i2) {
                fin += st_tm[i2 * CS + t2] * st_tm[t2 * CS + tid]
                t2++
            }
        }
        barrier()
        if (tid < i2) {
            st_tm[i2 * CS + tid] = fin
        }
        barrier()
        i2++
    }
    e = tid
    while (e < cl * cl) {
        vk_xqf[tmg + (e / cl) * CS + e % cl] = st_tm[(e / cl) * CS + e % cl]
        e += 256u
    }
    // d = T v_beta - (T (k_beta e^gcum)) S — kb scaled in place, minus folded into the scale;
    // phase 2 adds the w S term, so vnew here holds only the local half (vnloc)
    e = tid
    while (e < cl * ds) {
        vk_xqf[kb + e] = vk_xqf[kb + e] * -vk_xqf[egc + e / ds]
        e += 256u
    }
    memoryBarrierBuffer()
    barrier()
    dn_gemm(w, tmg, kb, cl, cl, ds, CS, ds, ds, 0u)
    dn_gemm(vnew, tmg, vb, cl, cl, ds, CS, ds, ds, 0u)
    // qc scaled in place to (q e^gcum) — phase 2's och GEMM reads it directly
    e = tid
    while (e < cl * ds) {
        vk_xqf[qc + e] = vk_xqf[qc + e] * vk_xqf[egc + e / ds]
        e += 256u
    }
    // transition piece: kgT = (k e^{glast - gcum})^T, phase 2's state-advance A operand
    let glast = vk_xqf[gcum + cl - 1u]
    if (tid < cl) {
        vk_xqf[kgc + tid] = exp(glast - vk_xqf[gcum + tid])
    }
    memoryBarrierBuffer()
    barrier()
    e = tid
    while (e < ds * cl) {
        let j = e / cl
        let i = e % cl
        vk_xqf[kgT + j * CS + i] = vk_xqf[kcT + j * CS + i] * vk_xqf[kgc + i]
        e += 256u
    }
}

[compute_shader(local_size_x=256, name="dn_scan_p2_spv")]
def dn_scan_p2 {
    let h = gl_WorkGroupID.x
    let npos = vk_meta[0u]
    let di = vk_meta[3u]
    let ds = vk_meta[6u]
    let o_off = vk_meta[10u]
    let wnorm_off = vk_meta[14u]
    let eps = vk_xs[0u]
    let tid = gl_LocalInvocationID.x
    let CS = 64u
    let nch = (npos + CS - 1u) / CS
    let st = vk_meta[19u] + h * (ds * ds + CS * ds)
    let och = st + ds * ds
    // the persistent state works in its slab copy (one GEMM surface for everything)
    var i0 = tid
    while (i0 < ds * ds) {
        vk_xqf[st + i0] = vk_wsf[h * ds * ds + i0]
        i0 += 256u
    }
    memoryBarrierBuffer()
    barrier()
    var c = 0u
    while (c < nch) {
        let c0 = c * CS
        let cl = min(CS, npos - c0)
        let slab = vk_meta[9u] * (h * nch + c)
        let qc = slab
        let w = slab + 4u * CS * ds
        let vnew = slab + 5u * CS * ds
        let kgT = slab + 6u * CS * ds
        let kq = slab + 7u * CS * ds + CS * CS
        let gcum = slab + 7u * CS * ds + 2u * CS * CS
        // d = vnloc + w S (in place over phase 1's local half)
        dn_gemm(vnew, w, st, cl, ds, ds, ds, ds, ds, 1u)
        // o = (q e^gcum) S + KQ d
        dn_gemm(och, qc, st, cl, ds, ds, ds, ds, ds, 0u)
        dn_gemm(och, kq, vnew, cl, cl, ds, CS, ds, ds, 1u)
        // gated out-norm straight into the o rows: rms(och) * wnorm, silu(z)-gated — one thread
        // per row (the row loop is tiny; heads supply the parallelism)
        if (tid < cl) {
            let ob = och + tid * ds
            var sum = 0.0
            var j3 = 0u
            while (j3 < ds) {
                let v = vk_xqf[ob + j3]
                sum += v * v
                j3++
            }
            let scale = 1.0 / sqrt(sum / float(ds) + eps)
            let orow = o_off + (c0 + tid) * di + h * ds
            let zrow = (c0 + tid) * di + h * ds
            j3 = 0u
            while (j3 < ds) {
                let zv = vk_y[zrow + j3]
                let xn = vk_xqf[ob + j3] * scale * vk_xs[wnorm_off + j3]
                vk_xqf[orow + j3] = xn * (zv / (1.0 + exp(-zv)))
                j3++
            }
        }
        barrier()
        // state advance, once per chunk: S = e^glast S + kgT d
        let egl = exp(vk_xqf[gcum + cl - 1u])
        var e = tid
        while (e < ds * ds) {
            vk_xqf[st + e] = vk_xqf[st + e] * egl
            e += 256u
        }
        memoryBarrierBuffer()
        barrier()
        dn_gemm(st, kgT, vnew, ds, cl, ds, CS, ds, ds, 1u)
        c++
    }
    // the advanced state back to its persistent plane
    i0 = tid
    while (i0 < ds * ds) {
        vk_wsf[h * ds * ds + i0] = vk_xqf[st + i0]
        i0 += 256u
    }
}

// plain Q8_0 requant of the o rows (no act) — the out-proj GEMM's activation image; input
// floats at the DN_WS o region (binding 0 float view). Subgroup per 4 blocks, word per lane
// (same coalesced shape as q8_moe_actrq); bit-identical to the thread-per-block form.
[compute_shader(local_size_x=256, name="dn_requant_spv")]
def dn_requant {
    let gid = gl_GlobalInvocationID.x
    let sg = gid / 32u
    let lane = gid % 32u
    let b = sg * 4u + lane / 8u
    if (b < vk_meta[18u]) {
        let eb = vk_meta[10u] + sg * 128u + lane * 4u
        vk_xq[b * 8u + lane % 8u] = q8_blk_pack(b, lane % 8u,
            vk_upf[eb], vk_upf[eb + 1u], vk_upf[eb + 2u], vk_upf[eb + 3u])
    }
}

// ===== the attention kernels =====
// at_prep_q/at_prep_k (deinterleave + qk-RMSNorm + partial rope) then at_attn (flash-style causal
// attention, online softmax). f32 per-tile accumulation is drift-class vs CPU — parity bar is token-identical output, not bit-identical.

var @workgroup at_row : float[8192]   // one position's projection row (qd <= 8192)
var @workgroup at_hs : float[64]      // per-head inverse rms (n_heads <= 64)

// deinterleave (gated q), per-head RMSNorm, partial NEOX rope — the shared core of at_prep_q/
// at_prep_k. Head square-sums run f32 by one thread per head (the CPU rmsnorm sums f32 too).
def private at_prep_core(d, nh, rms_off, obase, qsrc : uint) {
    let p = gl_WorkGroupID.x
    let hs = vk_meta[4u]
    let half = vk_meta[5u]
    let flags = vk_meta[9u]
    let gated = qsrc != 0u ? (flags & 1u) : 0u
    let qknorm = flags & 2u
    let cos_off = vk_meta[16u]
    let sin_off = vk_meta[13u]
    let eps = vk_xs[0u]
    let tid = gl_LocalInvocationID.x
    var i = tid
    while (i < d) {
        let h = i / hs
        let j = i % hs
        if (gated != 0u) {
            at_row[i] = vk_y[p * 2u * d + h * 2u * hs + j]
            vk_xqf[p * d + i] = vk_y[p * 2u * d + h * 2u * hs + hs + j]
        } else {
            at_row[i] = vk_y[p * d + i]
        }
        i += 256u
    }
    barrier()
    if (qknorm != 0u && tid < nh) {
        var ss = 0.0
        var j = 0u
        while (j < hs) {
            let v = at_row[tid * hs + j]
            ss += v * v
            j++
        }
        at_hs[tid] = 1.0 / sqrt(ss / float(hs) + eps)
    }
    barrier()
    i = tid
    while (i < d) {
        let h = i / hs
        let j = i % hs
        var v = at_row[i]
        if (qknorm != 0u) {
            v = vk_xs[rms_off + j] * (v * at_hs[h])
        }
        var outv = v
        if (j < half) {
            var v1 = at_row[h * hs + j + half]
            if (qknorm != 0u) {
                v1 = vk_xs[rms_off + j + half] * (v1 * at_hs[h])
            }
            outv = v * vk_xs[cos_off + p * half + j] - v1 * vk_xs[sin_off + p * half + j]
        } elif (j < 2u * half) {
            let jj = j - half
            var v0 = at_row[h * hs + jj]
            if (qknorm != 0u) {
                v0 = vk_xs[rms_off + jj] * (v0 * at_hs[h])
            }
            outv = v0 * vk_xs[sin_off + p * half + jj] + v * vk_xs[cos_off + p * half + jj]
        }
        vk_upf[(obase + p) * d + i] = outv
        i += 256u
    }
}

[compute_shader(local_size_x=256, name="at_prep_q_spv")]
def at_prep_q {
    at_prep_core(vk_meta[2u], vk_meta[6u], vk_meta[14u], 0u, 1u)
}

[compute_shader(local_size_x=256, name="at_prep_k_spv")]
def at_prep_k {
    at_prep_core(vk_meta[3u], vk_meta[7u], vk_meta[15u], vk_meta[1u], 0u)
}

// attention shared: the tile's 8 q rows, one 32-row K/V stage (stride 257 — a lane's strided
// column reads land 32 lanes on 32 distinct banks), the 8x32 score/probability tile, and the
// per-row online-softmax state
var @workgroup fa_q : float[2048]
var @workgroup fa_kv : float[8224]
var @workgroup fa_p : float[256]
var @workgroup fa_m : float[8]
var @workgroup fa_l : float[8]
var @workgroup fa_a : float[8]

// ===== the flash Q-tile core (at_attn and da_attn_b share it; the h128 coopmat twin does not) =====

// stage the 8-query tile and reset the online-softmax state
def private fa_load_q(q0, nrows, qd, h, hs : uint) {
    let tid = gl_LocalInvocationID.x
    var i = tid
    while (i < 8u * hs) {
        let qr = q0 + i / hs
        fa_q[i] = qr < nrows ? vk_upf[qr * qd + h * hs + i % hs] : 0.0
        i += 256u
    }
    if (tid < 8u) {
        fa_m[tid] = -3.0e38
        fa_l[tid] = 0.0
    }
}

// stage one 32-position K tile from vk_wsf at kbase (the V reuse of this staging loads per-kernel —
// the two kernels' V planes live on different bindings)
def private fa_load_k(k0, kend, kbase, kvd, kvh, hs : uint) {
    var i = gl_LocalInvocationID.x
    while (i < 32u * hs) {
        let kr = k0 + i / hs
        fa_kv[(i / hs) * 257u + i % hs] = kr < kend ? vk_wsf[kbase + kr * kvd + kvh * hs + i % hs] : 0.0
        i += 256u
    }
}

// tile QK^T: lane (r, c) scores query r against position k0+c, causal-masked into fa_p
def private fa_qk_score(r, c, hs, k0, gq, q0, nrows : uint; scale : float) {
    var s = 0.0
    var j = 0u
    while (j < hs) {   // 4-wide manual unroll (hs % 32 == 0)
        s += fa_q[r * hs + j] * fa_kv[c * 257u + j]
        s += fa_q[r * hs + j + 1u] * fa_kv[c * 257u + j + 1u]
        s += fa_q[r * hs + j + 2u] * fa_kv[c * 257u + j + 2u]
        s += fa_q[r * hs + j + 3u] * fa_kv[c * 257u + j + 3u]
        j += 4u
    }
    let gk = k0 + c
    fa_p[r * 32u + c] = (gk <= gq && q0 + r < nrows) ? s * scale : -3.0e38
}

// per-row online-softmax update: max, rescale, exponentiate in place (thread tid owns row tid)
def private fa_softmax_update {
    let tid = gl_LocalInvocationID.x
    if (tid < 8u) {
        var mt = fa_m[tid]
        var j2 = 0u
        while (j2 < 32u) {
            mt = max(mt, fa_p[tid * 32u + j2])
            j2++
        }
        let alpha = exp(fa_m[tid] - mt)
        var lsum = 0.0
        j2 = 0u
        while (j2 < 32u) {
            let e = exp(fa_p[tid * 32u + j2] - mt)
            fa_p[tid * 32u + j2] = e
            lsum += e
            j2++
        }
        fa_l[tid] = fa_l[tid] * alpha + lsum
        fa_m[tid] = mt
        fa_a[tid] = alpha
    }
}

// one workgroup per (head, 8-query tile); lane c owns output dims c + 32*jj (strided — keeps
// the AV shared reads conflict-free). The V tile REUSES the K tile's staging in shared.
[compute_shader(local_size_x=256, name="at_attn_spv")]
def at_attn {
    let rows = vk_meta[0u]
    let w0 = vk_meta[1u]
    let qd = vk_meta[2u]
    let kvd = vk_meta[3u]
    let hs = vk_meta[4u]
    let kv_mul = vk_meta[8u]
    let flags = vk_meta[9u]
    let dpl = vk_meta[11u]
    let scale = vk_xs[1u]
    let qtiles = (rows + 7u) / 8u
    let h = gl_WorkGroupID.x / qtiles
    let q0 = (gl_WorkGroupID.x % qtiles) * 8u
    let tid = gl_LocalInvocationID.x
    let r = tid / 32u
    let c = tid % 32u
    let kvh = h / kv_mul
    fa_load_q(q0, rows, qd, h, hs)
    var o0 = 0.0
    var o1 = 0.0
    var o2 = 0.0
    var o3 = 0.0
    var o4 = 0.0
    var o5 = 0.0
    var o6 = 0.0
    var o7 = 0.0
    barrier()
    let gq = w0 + q0 + r
    let kend = w0 + min(q0 + 8u, rows)   // causal bound (exclusive) of the tile's last valid row
    var i = tid
    var k0 = 0u
    while (k0 < kend) {
        fa_load_k(k0, kend, 0u, kvd, kvh, hs)
        barrier()
        fa_qk_score(r, c, hs, k0, gq, q0, rows, scale)
        barrier()
        fa_softmax_update()
        barrier()
        let av = fa_a[r]
        o0 *= av
        o1 *= av
        o2 *= av
        o3 *= av
        if (dpl > 4u) {
            o4 *= av
            o5 *= av
            o6 *= av
            o7 *= av
        }
        i = tid
        while (i < 32u * hs) {   // V tile reuses the K staging (all K reads completed above)
            let kr = k0 + i / hs
            fa_kv[(i / hs) * 257u + i % hs] = kr < kend ? vk_y[kr * kvd + kvh * hs + i % hs] : 0.0
            i += 256u
        }
        barrier()
        var c2 = 0u
        while (c2 < 32u) {
            let pw = fa_p[r * 32u + c2]
            let vb = c2 * 257u + c
            o0 += pw * fa_kv[vb]
            o1 += pw * fa_kv[vb + 32u]
            o2 += pw * fa_kv[vb + 64u]
            o3 += pw * fa_kv[vb + 96u]
            if (dpl > 4u) {
                o4 += pw * fa_kv[vb + 128u]
                o5 += pw * fa_kv[vb + 160u]
                o6 += pw * fa_kv[vb + 192u]
                o7 += pw * fa_kv[vb + 224u]
            }
            c2++
        }
        barrier()
        k0 += 32u
    }
    if (q0 + r < rows) {
        let inv = 1.0 / fa_l[r]
        let ob = (q0 + r) * qd + h * hs + c
        let gated = flags & 1u
        var jj = 0u
        while (jj < dpl) {
            var v = (jj == 0u ? o0 : (jj == 1u ? o1 : (jj == 2u ? o2 : (jj == 3u ? o3
                : (jj == 4u ? o4 : (jj == 5u ? o5 : (jj == 6u ? o6 : o7))))))) * inv
            if (gated != 0u) {
                let g = vk_xqf[ob + jj * 32u]
                v = v / (1.0 + exp(-g))
            }
            vk_xqf[ob + jj * 32u] = v
            jj++
        }
    }
}

// ===== the resident driver's residual-stream kernels =====
// x = (x + a) * ascale then y = rms(x) * w in ONE dispatch — collapses the layer boundary. meta[1]
// = 0 skips the add (the prologue's plain norm). Bindings 0 x | 1 a | 2 meta | 3 w | 5 y.

var @workgroup ar_row : float[4096]   // AR_MAX_DIM — staged so the reread after the reduce needs
                                      // no device-memory barrier (barrier() is a workgroup fence)
var @workgroup ar_part : float[64]    // one partial per subgroup — 64 covers subgroupSize >= 4 (256/4), the Vulkan floor
var @workgroup ar_inv : float[1]

// workgroup-wide rms inverse from per-thread sum-of-squares partials, via ar_part/ar_inv.
// Op order is contractual — the fused twins (ar_add_rms_rq, qkn_rope_kv) promise BIT-IDENTICAL.
def private wg_rms_inv(ss0 : float; n : uint; eps : float) : float {
    let ss = subgroupAdd(ss0)
    if (gl_SubgroupInvocationID == 0u) {
        ar_part[gl_SubgroupID] = ss
    }
    barrier()
    if (gl_LocalInvocationID.x == 0u) {
        var tot = 0.0
        var sgi = 0u
        while (sgi < gl_NumSubgroups) {
            tot += ar_part[sgi]
            sgi++
        }
        ar_inv[0u] = 1.0 / sqrt(tot / float(n) + eps)   // the CPU rmsnorm's own form, not rsqrt
    }
    barrier()
    return ar_inv[0u]
}

// the residual add + ar_row stash + sum-of-squares pass over one row at `base`
def private ar_accum_row(base, dim, add_on : uint; ascale : float) : float {
    var ss = 0.0
    var k = gl_LocalInvocationID.x
    while (k < dim) {
        let vv = add_on != 0u ? (vk_upf[base + k] + vk_wsf[base + k]) * ascale : vk_upf[base + k]
        vk_upf[base + k] = vv
        ar_row[k] = vv
        ss += vv * vv
        k += 256u
    }
    return ss
}

// add+rmsnorm, one workgroup per row (@workgroup arrays are per-wg, rows independent); row =
// gl_WorkGroupID.x offsets x/a/y by row*dim. The decode seams dispatch it with ONE workgroup.
[compute_shader(local_size_x=256, name="ar_add_rms_b_spv")]
def ar_add_rms_b {
    let dim = vk_meta[0u]
    let woff = vk_meta[2u]   // norm-weight row base (resident driver shares one norms buffer)
    let base = gl_WorkGroupID.x * dim
    let ss = ar_accum_row(base, dim, vk_meta[1u], vk_xs[1u])
    let inv = wg_rms_inv(ss, dim, vk_xs[0u])
    var k = gl_LocalInvocationID.x
    while (k < dim) {
        vk_y[base + k] = vk_xqf[woff + k] * (ar_row[k] * inv)
        k += 256u
    }
}

// ar_add_rms + dn_requant in ONE dispatch: the normed row quantizes out of ar_row, so xb never
// reaches memory. Verbatim reduce/amax/rounding => BIT-IDENTICAL to the split pair. Bindings 0 x |
// 1 a | 2 meta | 3 w+eps/ascale | 4 xs | 5 xq. meta 0=dim 1=add_on 2=woff 3=eps off (4 is xs now).
[compute_shader(local_size_x=256, name="ar_add_rms_rq_spv")]
def ar_add_rms_rq {
    let dim = vk_meta[0u]
    let add_on = vk_meta[1u]
    let woff = vk_meta[2u]
    let soff = vk_meta[3u]
    let tid = gl_LocalInvocationID.x
    let ss = ar_accum_row(0u, dim, add_on, vk_xqf[soff + 1u])
    let inv = wg_rms_inv(ss, dim, vk_xqf[soff])
    // quantize the normed row instead of storing it: 8 lanes per 32-block, 32 blocks per pass. The
    // 8 lanes of a block are consecutive AND 8-aligned, so the xor-1/2/4 fold only ever exchanges
    // with co-active lanes even when other groups in the subgroup have left the loop.
    let nblk = dim / 32u
    let lane8 = tid % 8u
    var b = tid / 8u
    while (b < nblk) {
        let base = b * 32u + lane8 * 4u
        let v0 = vk_xqf[woff + base] * (ar_row[base] * inv)
        let v1 = vk_xqf[woff + base + 1u] * (ar_row[base + 1u] * inv)
        let v2 = vk_xqf[woff + base + 2u] * (ar_row[base + 2u] * inv)
        let v3 = vk_xqf[woff + base + 3u] * (ar_row[base + 3u] * inv)
        vk_yq[b * 8u + lane8] = q8_blk_pack(b, lane8, v0, v1, v2, v3)
        b += 32u
    }
}

// per-head RMSNorm on q/k between the projections and rope (qk_norm models), one wg per head-row,
// in place: q rows in vk_upf, k rows in vk_y; weight rows in vk_xqf; eps = vk_xs[0]. meta 0=hs
// 1=nh 2=nkvh 3=qstride 4=kstride 5=kbase 6=qwoff 7=kwoff; dispatch rows*(nh+nkvh) wgs.
[compute_shader(local_size_x=256, name="qk_rms_spv")]
def qk_rms {
    let hs = vk_meta[0u]
    let nh = vk_meta[1u]
    let per = nh + vk_meta[2u]
    let wid = gl_WorkGroupID.x
    let r = wid / per
    let hh = wid % per
    let is_q = hh < nh
    let base = is_q ? r * vk_meta[3u] + hh * hs : vk_meta[5u] + r * vk_meta[4u] + (hh - nh) * hs
    let wbase = is_q ? vk_meta[6u] : vk_meta[7u]
    let tid = gl_LocalInvocationID.x
    var ss = 0.0
    if (tid < hs) {
        let v = is_q ? vk_upf[base + tid] : vk_y[base + tid]
        ss = v * v
    }
    let inv = wg_rms_inv(ss, hs, vk_xs[0u])
    if (tid < hs) {
        let w = vk_xqf[wbase + tid]
        if (is_q) {
            vk_upf[base + tid] = w * (vk_upf[base + tid] * inv)
        } else {
            vk_y[base + tid] = w * (vk_y[base + tid] * inv)
        }
    }
}

// fused rope + KV-row store (decode, one token), f32 mirror. One thread per rotation pair; each
// owns 2 distinct K + 2 V slots (no packing, no hazard). q roped in vk_y, k roped into vk_wsf, v raw
// into vk_xqf at absolute pos. meta 0=qd 1=kvd 2=hs 3=half 4=neox 5=kpair0 6=npairs 7=krow_f 8=vrow_f.
[compute_shader(local_size_x=256, name="rope_kv_store_spv")]
def rope_kv_store {
    let gid = gl_GlobalInvocationID.x
    let npairs = vk_meta[6u]
    if (gid < npairs) {
        let qd = vk_meta[0u]
        let hs = vk_meta[2u]
        let half = vk_meta[3u]
        let neox = vk_meta[4u]
        let kpair0 = vk_meta[5u]
        let is_k = gid >= kpair0
        let lp = is_k ? gid - kpair0 : gid           // local pair index within q or k
        let h = lp / half
        let j = lp % half
        let fcr = vk_xs[j]
        let fci = vk_xs[half + j]
        // NORM (llama): adjacent pair (2j, 2j+1). NEOX (qwen/gemma): (j, j+half).
        let e0 = neox != 0u ? j : j * 2u
        let e1 = neox != 0u ? j + half : j * 2u + 1u
        if (is_k) {
            let kb = qd + h * hs
            let a0 = vk_upf[kb + e0]
            let a1 = vk_upf[kb + e1]
            vk_wsf[vk_meta[7u] + h * hs + e0] = a0 * fcr - a1 * fci
            vk_wsf[vk_meta[7u] + h * hs + e1] = a0 * fci + a1 * fcr
            // v is not roped — copy the same two elements raw into the V plane
            let vb = qd + vk_meta[1u] + h * hs
            vk_xqf[vk_meta[8u] + h * hs + e0] = vk_upf[vb + e0]
            vk_xqf[vk_meta[8u] + h * hs + e1] = vk_upf[vb + e1]
        } else {
            let qb = h * hs
            let a0 = vk_y[qb + e0]
            let a1 = vk_y[qb + e1]
            vk_y[qb + e0] = a0 * fcr - a1 * fci
            vk_y[qb + e1] = a0 * fci + a1 * fcr
        }
    }
}

// qk_rms + rope_kv_store in ONE dispatch (qk_norm only), one wg per head-row = qk_rms's own shape;
// verbatim reduce + rotation => BIT-IDENTICAL. rms rows + eps ride the cos buffer (binding 3 is the
// V mirror). 0 kv|1 Kmir|2 meta|3 Vmir|4 cos+rms+eps|5 q. meta = rope's, 5=nh 9=qw 10=kw 11=eps.
var @workgroup qr_row : float[256]   // this head's q or k row (hs <= 256, same cap as da_q)

[compute_shader(local_size_x=256, name="qkn_rope_kv_spv")]
def qkn_rope_kv {
    let qd = vk_meta[0u]
    let hs = vk_meta[2u]
    let half = vk_meta[3u]
    let neox = vk_meta[4u]
    let nh = vk_meta[5u]
    let wid = gl_WorkGroupID.x
    let is_k = wid >= nh
    let h = is_k ? wid - nh : wid
    let tid = gl_LocalInvocationID.x
    // q rows live in vk_y, k rows at qd in vk_upf (the kv projection buffer)
    let sbase = is_k ? qd + h * hs : h * hs
    var v = 0.0
    if (tid < hs) {
        v = is_k ? vk_upf[sbase + tid] : vk_y[sbase + tid]
    }
    let ss = tid < hs ? v * v : 0.0
    let inv = wg_rms_inv(ss, hs, vk_xs[vk_meta[11u]])
    if (tid < hs) {
        qr_row[tid] = vk_xs[(is_k ? vk_meta[10u] : vk_meta[9u]) + tid] * (v * inv)
    }
    barrier()
    // rope this head's pairs off the normed row. NORM (llama): adjacent pair (2j, 2j+1).
    // NEOX (qwen/gemma): (j, j+half). Either way the pairs partition [0, hs).
    if (tid < half) {
        let fcr = vk_xs[tid]
        let fci = vk_xs[half + tid]
        let e0 = neox != 0u ? tid : tid * 2u
        let e1 = neox != 0u ? tid + half : tid * 2u + 1u
        let a0 = qr_row[e0]
        let a1 = qr_row[e1]
        let r0 = a0 * fcr - a1 * fci
        let r1 = a0 * fci + a1 * fcr
        if (is_k) {
            let kb = vk_meta[7u] + h * hs
            vk_wsf[kb + e0] = r0
            vk_wsf[kb + e1] = r1
        } else {
            let qb = h * hs
            vk_y[qb + e0] = r0
            vk_y[qb + e1] = r1
        }
    }
    // v is not roped or normed — the k-head wgs copy their own raw row into the V plane
    if (is_k && tid < hs) {
        vk_xqf[vk_meta[8u] + h * hs + tid] = vk_upf[qd + vk_meta[1u] + h * hs + tid]
    }
}

// decode attention (one query row): one wg/head, GQA, f32 K/V mirror (K vk_wsf, V vk_xqf).
// Online softmax over 256-position chunks (running max/denom, alpha-rescaled V accumulators) —
// shared use is chunk-sized, ANY cnt works. meta 0=cnt 2=kvd 3=hs 4=kv_mul; scale vk_xs[0].
var @workgroup da_q : float[256]      // this head's query row (hs <= 256)
var @workgroup da_sc : float[256]     // chunk scores, then the cross-group V partial reduce
var @workgroup da_part : float[64]    // per-subgroup partials — 64 covers subgroupSize >= 4 (was [8], OOB on subgroupSize < 32, e.g. Intel 16)
var @workgroup da_ml : float[2]       // [0] = running max, [1] = running softmax denom

[compute_shader(local_size_x=256, name="da_attn_spv")]
def da_attn {
    let cnt = vk_meta[0u]
    let kvd = vk_meta[2u]
    let hs = vk_meta[3u]
    let kv_mul = vk_meta[4u]
    let kbase = vk_meta[5u]   // layer base into the shared KV mirror (0 for the standalone seam)
    let vbase = vk_meta[6u]
    let scale = vk_xs[0u]
    let h = gl_WorkGroupID.x
    let kvh = h / kv_mul
    let tid = gl_LocalInvocationID.x
    let npart = 256u / hs     // V-phase partial groups; thread tid = group g, output dim c
    let g = tid / hs
    let c = tid % hs
    var i = tid
    while (i < hs) {
        da_q[i] = vk_upf[h * hs + i]
        i += 256u
    }
    if (tid == 0u) {
        da_ml[1u] = 0.0
    }
    barrier()
    var acc = 0.0             // this thread's V partial (group g's slice of dim c)
    var mrun = -3.0e38        // running max — uniform, tracked in lockstep by every thread
    var c0 = 0u
    while (c0 < cnt) {
        let clen = min(256u, cnt - c0)
        // chunk scores: thread tid owns position c0 + tid
        var s = -3.0e38
        if (tid < clen) {
            var sv = 0.0
            var d = 0u
            while (d < hs) {
                sv += da_q[d] * vk_wsf[kbase + (c0 + tid) * kvd + kvh * hs + d]
                d++
            }
            s = sv * scale
        }
        let tmax = subgroupMax(s)
        if (gl_SubgroupInvocationID == 0u) {
            da_part[gl_SubgroupID] = tmax
        }
        barrier()
        if (tid == 0u) {
            var m = mrun
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                m = max(m, da_part[sgi])
                sgi++
            }
            da_ml[0u] = m
        }
        barrier()
        let mnew = da_ml[0u]
        let alpha = exp(mrun - mnew)
        mrun = mnew
        // exp + chunk sum; denom rescale rides the same alpha
        let e = tid < clen ? exp(s - mnew) : 0.0
        da_sc[tid] = e
        let tsum = subgroupAdd(e)
        if (gl_SubgroupInvocationID == 0u) {
            da_part[gl_SubgroupID] = tsum
        }
        barrier()
        if (tid == 0u) {
            var lsum = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                lsum += da_part[sgi]
                sgi++
            }
            da_ml[1u] = da_ml[1u] * alpha + lsum
        }
        // V accumulate: group g strides the chunk, rescaled by this chunk's alpha
        acc *= alpha
        if (tid < npart * hs) {
            var jj = g
            while (jj < clen) {
                acc += da_sc[jj] * vk_xqf[vbase + (c0 + jj) * kvd + kvh * hs + c]
                jj += npart
            }
        }
        barrier()   // da_sc/da_part settle before the next chunk overwrites them
        c0 += 256u
    }
    // cross-group reduce (da_sc reused as the partial slab: slot tid == g * hs + c)
    da_sc[tid] = acc
    barrier()
    if (tid < hs) {
        let linv = 1.0 / da_ml[1u]
        var tot = 0.0
        var gg = 0u
        while (gg < npart) {
            tot += da_sc[gg * hs + tid]
            gg++
        }
        vk_y[h * hs + tid] = tot * linv
    }
}

// batched causal attention (prefill), flash-style Q tile: one wg per (head, 8-query tile), // nolint:STYLE014
// 32-position shared K/V tiles (V reuses the K staging), per-row online softmax, lane c owns dims
// c + 32*jj. Needs hs % 32 == 0, <= 256 (resident_plan gates); reduce order differs from the row
// kernel so the parity bar is IDS. meta 0=window rows 1=nh 2=kvd 3=hs 4=kv_mul 5=kbase 6=vbase
// 7=w0 (window base — queries attend mirror positions [0, w0+row]); scale vk_xs[0].
[compute_shader(local_size_x=256, name="da_attn_b_spv")]
def da_attn_b {
    let npos = vk_meta[0u]
    let nh = vk_meta[1u]
    let kvd = vk_meta[2u]
    let hs = vk_meta[3u]
    let kv_mul = vk_meta[4u]
    let kbase = vk_meta[5u]
    let vbase = vk_meta[6u]
    let w0 = vk_meta[7u]
    let scale = vk_xs[0u]
    let qd = nh * hs
    let dpl = hs / 32u
    let qtiles = (npos + 7u) / 8u
    let h = gl_WorkGroupID.x / qtiles
    let q0 = (gl_WorkGroupID.x % qtiles) * 8u
    let tid = gl_LocalInvocationID.x
    let r = tid / 32u
    let c = tid % 32u
    let kvh = h / kv_mul
    fa_load_q(q0, npos, qd, h, hs)
    var o0 = 0.0
    var o1 = 0.0
    var o2 = 0.0
    var o3 = 0.0
    var o4 = 0.0
    var o5 = 0.0
    var o6 = 0.0
    var o7 = 0.0
    barrier()
    let gq = w0 + q0 + r
    let kend = w0 + min(q0 + 8u, npos)   // causal bound (exclusive) of the tile's last valid row
    var i = tid
    var k0 = 0u
    while (k0 < kend) {
        fa_load_k(k0, kend, kbase, kvd, kvh, hs)
        barrier()
        fa_qk_score(r, c, hs, k0, gq, q0, npos, scale)
        barrier()
        fa_softmax_update()
        barrier()
        let av = fa_a[r]
        o0 *= av
        if (dpl > 1u) {
            o1 *= av
        }
        if (dpl > 2u) {
            o2 *= av
            o3 *= av
        }
        if (dpl > 4u) {
            o4 *= av
            o5 *= av
            o6 *= av
            o7 *= av
        }
        i = tid
        while (i < 32u * hs) {   // V tile reuses the K staging (all K reads completed above)
            let kr = k0 + i / hs
            fa_kv[(i / hs) * 257u + i % hs] = kr < kend ? vk_xqf[vbase + kr * kvd + kvh * hs + i % hs] : 0.0
            i += 256u
        }
        barrier()
        var c2 = 0u
        while (c2 < 32u) {
            let pw = fa_p[r * 32u + c2]
            let vb = c2 * 257u + c
            o0 += pw * fa_kv[vb]
            if (dpl > 1u) {
                o1 += pw * fa_kv[vb + 32u]
            }
            if (dpl > 2u) {
                o2 += pw * fa_kv[vb + 64u]
                o3 += pw * fa_kv[vb + 96u]
            }
            if (dpl > 4u) {
                o4 += pw * fa_kv[vb + 128u]
                o5 += pw * fa_kv[vb + 160u]
                o6 += pw * fa_kv[vb + 192u]
                o7 += pw * fa_kv[vb + 224u]
            }
            c2++
        }
        barrier()
        k0 += 32u
    }
    if (q0 + r < npos) {
        let inv = 1.0 / fa_l[r]
        let ob = (q0 + r) * qd + h * hs + c
        var jj = 0u
        while (jj < dpl) {
            vk_y[ob + jj * 32u] = (jj == 0u ? o0 : (jj == 1u ? o1 : (jj == 2u ? o2 : (jj == 3u ? o3
                : (jj == 4u ? o4 : (jj == 5u ? o5 : (jj == 6u ? o6 : o7))))))) * inv
            jj++
        }
    }
}

// hs=128 flash-attention twin of da_attn_b (coopmat devices): QK^T on f16 MMAs (hoisted q frags),
// shuffle-reduced online softmax, register-tiled PV over the f16 V tile. Same meta contract;
// parity bar is IDS (f16 staging + different reduce order). Measured 5.0x da_attn_b (3060 Ti).

var @workgroup fb_q : uint[2048]
var @workgroup fb_kv : uint[2048]
var @workgroup fb_p : float[1024]
var @workgroup fb_m : float[32]
var @workgroup fb_l : float[32]
var @workgroup fb_a : float[32]

[compute_shader(local_size_x=128, name="da_attn_b_h128_spv")]
def da_attn_b_h128 {
    let npos = vk_meta[0u]
    let nh = vk_meta[1u]
    let kvd = vk_meta[2u]
    let kv_mul = vk_meta[4u]
    let kbase = vk_meta[5u]
    let vbase = vk_meta[6u]
    let w0 = vk_meta[7u]
    let scale = vk_xs[0u]
    let qd = nh * 128u
    let qtiles = (npos + 31u) / 32u
    let h = gl_WorkGroupID.x / qtiles
    let q0 = (gl_WorkGroupID.x % qtiles) * 32u
    let tid = gl_LocalInvocationID.x
    let warp = tid / 32u
    let kvh = h / kv_mul
    var i = tid
    while (i < 2048u) {   // stage the 32-row q tile once, f32 -> f16 pairs
        let qr = q0 + i / 64u
        let jp = (i % 64u) * 2u
        if (qr < npos) {
            let src = qr * qd + h * 128u + jp
            fb_q[i] = packHalf2x16(float2(vk_upf[src], vk_upf[src + 1u]))
        } else {
            fb_q[i] = 0u
        }
        i += 128u
    }
    if (tid < 32u) {
        fb_m[tid] = -3.0e38
        fb_l[tid] = 0.0
    }
    var o : float[32]   // 2 rows x 16 cols per thread
    for (z in range(32)) {
        o[z] = 0.0
    }
    let orow0 = (tid / 8u) * 2u
    let oe = (tid % 8u) * 16u
    let srow = tid / 4u
    let cb = (tid % 4u) * 8u
    let lane4 = tid % 4u
    barrier()
    // my S block's q fragments — invariant across the whole K loop
    let br = warp / 2u
    let bc = warp % 2u
    var qa0 : coopmatA_f16_16x16
    var qa1 : coopmatA_f16_16x16
    var qa2 : coopmatA_f16_16x16
    var qa3 : coopmatA_f16_16x16
    var qa4 : coopmatA_f16_16x16
    var qa5 : coopmatA_f16_16x16
    var qa6 : coopmatA_f16_16x16
    var qa7 : coopmatA_f16_16x16
    coopmatLoad(qa0, fb_q, int(br * 16u) * 64, 64, 0)
    coopmatLoad(qa1, fb_q, int(br * 16u) * 64 + 8, 64, 0)
    coopmatLoad(qa2, fb_q, int(br * 16u) * 64 + 16, 64, 0)
    coopmatLoad(qa3, fb_q, int(br * 16u) * 64 + 24, 64, 0)
    coopmatLoad(qa4, fb_q, int(br * 16u) * 64 + 32, 64, 0)
    coopmatLoad(qa5, fb_q, int(br * 16u) * 64 + 40, 64, 0)
    coopmatLoad(qa6, fb_q, int(br * 16u) * 64 + 48, 64, 0)
    coopmatLoad(qa7, fb_q, int(br * 16u) * 64 + 56, 64, 0)
    let kend = w0 + min(q0 + 32u, npos)
    var k0 = 0u
    while (k0 < kend) {
        i = tid
        while (i < 2048u) {   // K tile, f16 pairs
            let kr = k0 + i / 64u
            let jp = (i % 64u) * 2u
            if (kr < kend) {
                let src = kbase + kr * kvd + kvh * 128u + jp
                fb_kv[i] = packHalf2x16(float2(vk_wsf[src], vk_wsf[src + 1u]))
            } else {
                fb_kv[i] = 0u
            }
            i += 128u
        }
        barrier()
        var acc : coopmatAcc_f32_16x16
        var kb : coopmatB_f16_16x16
        coopmatLoad(kb, fb_kv, int(bc * 16u) * 64, 64, 1)
        acc = coopmatMulAdd(qa0, kb, acc)
        coopmatLoad(kb, fb_kv, int(bc * 16u) * 64 + 8, 64, 1)
        acc = coopmatMulAdd(qa1, kb, acc)
        coopmatLoad(kb, fb_kv, int(bc * 16u) * 64 + 16, 64, 1)
        acc = coopmatMulAdd(qa2, kb, acc)
        coopmatLoad(kb, fb_kv, int(bc * 16u) * 64 + 24, 64, 1)
        acc = coopmatMulAdd(qa3, kb, acc)
        coopmatLoad(kb, fb_kv, int(bc * 16u) * 64 + 32, 64, 1)
        acc = coopmatMulAdd(qa4, kb, acc)
        coopmatLoad(kb, fb_kv, int(bc * 16u) * 64 + 40, 64, 1)
        acc = coopmatMulAdd(qa5, kb, acc)
        coopmatLoad(kb, fb_kv, int(bc * 16u) * 64 + 48, 64, 1)
        acc = coopmatMulAdd(qa6, kb, acc)
        coopmatLoad(kb, fb_kv, int(bc * 16u) * 64 + 56, 64, 1)
        acc = coopmatMulAdd(qa7, kb, acc)
        coopmatStore(acc, fb_p, int(br * 16u) * 32 + int(bc * 16u), 32, 0)
        barrier()
        // online softmax: 4 threads/row (8 cols each), shuffle-reduced within the aligned quad
        let gq = w0 + q0 + srow
        let m_old = fb_m[srow]
        var mloc = -3.0e38
        for [unroll] (j in range(8)) {
            let idx = srow * 32u + cb + uint(j)
            let gk = k0 + cb + uint(j)
            let s = (gk <= gq && q0 + srow < npos && gk < kend) ? fb_p[idx] * scale : -3.0e38
            fb_p[idx] = s
            mloc = max(mloc, s)
        }
        mloc = max(mloc, subgroupShuffleXor(mloc, 1u))
        mloc = max(mloc, subgroupShuffleXor(mloc, 2u))
        let mt = max(m_old, mloc)
        var lloc = 0.0
        for [unroll] (j in range(8)) {
            let idx = srow * 32u + cb + uint(j)
            let e = exp(fb_p[idx] - mt)
            fb_p[idx] = e
            lloc += e
        }
        lloc += subgroupShuffleXor(lloc, 1u)
        lloc += subgroupShuffleXor(lloc, 2u)
        if (lane4 == 0u) {
            let alpha = exp(m_old - mt)
            fb_l[srow] = fb_l[srow] * alpha + lloc
            fb_m[srow] = mt
            fb_a[srow] = alpha
        }
        barrier()
        i = tid
        while (i < 2048u) {   // V tile reuses the K staging
            let kr = k0 + i / 64u
            let jp = (i % 64u) * 2u
            if (kr < kend) {
                let src = vbase + kr * kvd + kvh * 128u + jp
                fb_kv[i] = packHalf2x16(float2(vk_xqf[src], vk_xqf[src + 1u]))
            } else {
                fb_kv[i] = 0u
            }
            i += 128u
        }
        barrier()
        let av0 = fb_a[orow0]
        let av1 = fb_a[orow0 + 1u]
        for (z in range(16)) {
            o[z] *= av0
            o[16 + z] *= av1
        }
        var c2 = 0u
        while (c2 < 32u) {
            let pw0 = fb_p[orow0 * 32u + c2]
            let pw1 = fb_p[(orow0 + 1u) * 32u + c2]
            let vb = c2 * 64u + oe / 2u
            for [unroll] (zp in range(8)) {
                let pair = unpackHalf2x16(fb_kv[vb + uint(zp)])
                o[zp * 2] += pw0 * pair.x
                o[zp * 2 + 1] += pw0 * pair.y
                o[16 + zp * 2] += pw1 * pair.x
                o[16 + zp * 2 + 1] += pw1 * pair.y
            }
            c2++
        }
        barrier()
        k0 += 32u
    }
    if (q0 + orow0 < npos) {
        let inv0 = 1.0 / fb_l[orow0]
        let ob0 = (q0 + orow0) * qd + h * 128u + oe
        for (z in range(16)) {
            vk_y[ob0 + uint(z)] = o[z] * inv0
        }
    }
    if (q0 + orow0 + 1u < npos) {
        let inv1 = 1.0 / fb_l[orow0 + 1u]
        let ob1 = (q0 + orow0 + 1u) * qd + h * 128u + oe
        for (z in range(16)) {
            vk_y[ob1 + uint(z)] = o[16 + z] * inv1
        }
    }
}

// batched rope + KV-store (prefill, npos positions). q roped in place (vk_upf); k/v ride vk_y (k at
// 0, v at voff); roped k -> vk_wsf, raw v -> vk_xqf at absolute pos. cos/sin per pos in vk_xs.
// meta 0=qd 1=kvd 2=hs 3=half 4=neox 5=kpair0 6=pairs/pos 7=layerbase 9=npos 10=pos0 11=voff
[compute_shader(local_size_x=256, name="rope_kv_store_b_spv")]
def rope_kv_store_b {
    let gid = gl_GlobalInvocationID.x
    let ppp = vk_meta[6u]
    let npos = vk_meta[9u]
    if (gid < npos * ppp) {
        let qd = vk_meta[0u]
        let kvd = vk_meta[1u]
        let hs = vk_meta[2u]
        let half = vk_meta[3u]
        let neox = vk_meta[4u]
        let kpair0 = vk_meta[5u]
        let p = gid / ppp
        let lp = gid % ppp
        let is_k = lp >= kpair0
        let pr = is_k ? lp - kpair0 : lp
        let h = pr / half
        let j = pr % half
        let cb = p * hs           // this position's cos/sin row
        let fcr = vk_xs[cb + j]
        let fci = vk_xs[cb + half + j]
        let e0 = neox != 0u ? j : j * 2u
        let e1 = neox != 0u ? j + half : j * 2u + 1u
        let apos = vk_meta[10u] + p
        if (is_k) {
            let kb = p * kvd + h * hs
            let a0 = vk_y[kb + e0]
            let a1 = vk_y[kb + e1]
            let mo = vk_meta[7u] + apos * kvd + h * hs
            vk_wsf[mo + e0] = a0 * fcr - a1 * fci
            vk_wsf[mo + e1] = a0 * fci + a1 * fcr
            let vb = vk_meta[11u] + p * kvd + h * hs
            vk_xqf[mo + e0] = vk_y[vb + e0]
            vk_xqf[mo + e1] = vk_y[vb + e1]
        } else {
            let qb = p * qd + h * hs
            let a0 = vk_upf[qb + e0]
            let a1 = vk_upf[qb + e1]
            vk_upf[qb + e0] = a0 * fcr - a1 * fci
            vk_upf[qb + e1] = a0 * fci + a1 * fcr
        }
    }
}

// plain Q8_K requant (no act) — the kq wo GEMM's activation image; input floats at vk_upf base
// vk_meta[10], one SUBGROUP per 256-weight superblock ([18] = superblock count): 8 coalesced
// rounds/lane, 5-shuffle amax, butterfly pack — mirrors q8k_moe_actrq minus the act; bit-identical.
[compute_shader(local_size_x=256, name="q8k_requant_spv")]
def q8k_requant {
    let gid = gl_GlobalInvocationID.x
    let b = gid / 32u
    let lane = gid % 32u
    if (b < vk_meta[18u]) {
        let base = vk_meta[10u] + b * 256u
        var vals : float[8]
        var m = 0.0
        for [unroll] (r in range(8)) {
            let v = vk_upf[base + uint(r) * 32u + lane]
            vals[r] = v
            m = max(m, abs(v))
        }
        let id = q8k_blk_scale(b, lane, m)
        for [unroll] (r in range(8)) {
            q8k_pack_word(b, uint(r), lane, vals[r], id)
        }
    }
}

// ===== the K-quant decode kernels =====
// 4 lanes cooperate per 32-block — a lane owns one nibble word pairing weights k / k+16, so both
// halves land in the SAME block and warp loads stay consecutive; the dot is exact sdot4 over [0,63] quants.

var @ssbo @binding = 1 vk_wsu : array<uint>

// deposit 4 packed high-bits (one per nibble lane of hb) at bit 4 of each byte lane
def private k5_dep(hb : uint) : uint => (
    ((hb & 1u) << 4u) | ((hb & 2u) << 11u) | ((hb & 4u) << 18u) | ((hb & 8u) << 25u))

// ===== the K-quant decode GEMV family: ONE LANE PER 32-BLOCK, uint4 loads =====
// uint4 index `wsb*stride4 + blk` IS block blk's 4 quant words: 4x bytes in flight/warp (512B vs
// 128B, q8_moe_groupn's lever) + one scale decode/block, not four. DRIFT-CLASS (1 float term/block).

let private KQ_LOW = 0x0F0F0F0Fu   // nibble mask
let private KQ_ONES = 0x01010101u  // sdot4 against this sums 4 signed bytes

// a block's activation sum (the min/offset terms fold against it)
def private kq_blk_sum(xl, xh : uint4) : int {
    let bs = sdot4(xl.x, KQ_ONES) + sdot4(xl.y, KQ_ONES) + sdot4(xl.z, KQ_ONES) + sdot4(xl.w, KQ_ONES)
    return bs + sdot4(xh.x, KQ_ONES) + sdot4(xh.y, KQ_ONES) + sdot4(xh.z, KQ_ONES) + sdot4(xh.w, KQ_ONES)
}

// a 4-bit block's dot: weight word j's LOW nibbles hit x word j, its HIGH nibbles x word j+4
def private kq_blk_dot(wv, xl, xh : uint4) : int {
    var idot = sdot4(wv.x & KQ_LOW, xl.x) + sdot4(wv.y & KQ_LOW, xl.y)
    idot += sdot4(wv.z & KQ_LOW, xl.z) + sdot4(wv.w & KQ_LOW, xl.w)
    idot += sdot4((wv.x >> 4u) & KQ_LOW, xh.x) + sdot4((wv.y >> 4u) & KQ_LOW, xh.y)
    return idot + sdot4((wv.z >> 4u) & KQ_LOW, xh.z) + sdot4((wv.w >> 4u) & KQ_LOW, xh.w)
}

// Q6_K's dot, halves kept apart: the lo/hi nibble halves ARE the block's two 16-weight sub-scale
// groups, so each needs its own dot and activation sum. qh contributes 2 bits per weight.
// Returns (ilo, ihi, blo, bhi) — an int4, since a shader function cannot return a tuple.
def private k6_blk_dot(wv, xl, xh, qhl, qhh : uint4; shift : uint) : int4 {
    let m2 = 0x03030303u
    let l0 = (wv.x & KQ_LOW) | (((qhl.x >> shift) & m2) << 4u)
    let l1 = (wv.y & KQ_LOW) | (((qhl.y >> shift) & m2) << 4u)
    let l2 = (wv.z & KQ_LOW) | (((qhl.z >> shift) & m2) << 4u)
    let l3 = (wv.w & KQ_LOW) | (((qhl.w >> shift) & m2) << 4u)
    let h0 = ((wv.x >> 4u) & KQ_LOW) | (((qhh.x >> shift) & m2) << 4u)
    let h1 = ((wv.y >> 4u) & KQ_LOW) | (((qhh.y >> shift) & m2) << 4u)
    let h2 = ((wv.z >> 4u) & KQ_LOW) | (((qhh.z >> shift) & m2) << 4u)
    let h3 = ((wv.w >> 4u) & KQ_LOW) | (((qhh.w >> shift) & m2) << 4u)
    return int4(sdot4(l0, xl.x) + sdot4(l1, xl.y) + sdot4(l2, xl.z) + sdot4(l3, xl.w),
                sdot4(h0, xh.x) + sdot4(h1, xh.y) + sdot4(h2, xh.z) + sdot4(h3, xh.w),
                sdot4(xl.x, KQ_ONES) + sdot4(xl.y, KQ_ONES) + sdot4(xl.z, KQ_ONES) + sdot4(xl.w, KQ_ONES),
                sdot4(xh.x, KQ_ONES) + sdot4(xh.y, KQ_ONES) + sdot4(xh.z, KQ_ONES) + sdot4(xh.w, KQ_ONES))
}

// the Q4_K/Q5_K per-block scale fold: d*sc*idot - dmin*mn*bsum, int products kept exact
def private kq_gemv_dm(wsb, blk : uint; idot, bs : int) : float {
    let dm = unpackHalf2x16(vk_wsu[wsb * 5u])
    let sc = int((vk_wsu[wsb * 5u + 1u + blk / 4u] >> ((blk % 4u) * 8u)) & 0xFF)
    let mn = int((vk_wsu[wsb * 5u + 3u + blk / 4u] >> ((blk % 4u) * 8u)) & 0xFF)
    return dm.x * float(sc * idot) - dm.y * float(mn * bs)
}

// one Q4_0 f16 d per 32-block, 8 packed as half pairs in the superblock's wsu words 0..3
def private q40_blk_d(wsb, blk : uint) : float {
    let dp = unpackHalf2x16(vk_wsu[wsb * 5u + blk / 2u])
    return blk % 2u == 0u ? dp.x : dp.y
}

// Q5_K's dot: the same fold with the 5th bit deposited from the block's qh word, byte j serving
// weight word j (low nibble -> lo half, high nibble -> hi half)
def private k5_blk_dot(wv, xl, xh : uint4; qh : uint) : int {
    let h0 = qh & 0xFFu
    let h1 = (qh >> 8u) & 0xFFu
    let h2 = (qh >> 16u) & 0xFFu
    let h3 = (qh >> 24u) & 0xFFu
    var idot = sdot4((wv.x & KQ_LOW) | k5_dep(h0 & 0xFu), xl.x)
    idot += sdot4((wv.y & KQ_LOW) | k5_dep(h1 & 0xFu), xl.y)
    idot += sdot4((wv.z & KQ_LOW) | k5_dep(h2 & 0xFu), xl.z)
    idot += sdot4((wv.w & KQ_LOW) | k5_dep(h3 & 0xFu), xl.w)
    idot += sdot4(((wv.x >> 4u) & KQ_LOW) | k5_dep(h0 >> 4u), xh.x)
    idot += sdot4(((wv.y >> 4u) & KQ_LOW) | k5_dep(h1 >> 4u), xh.y)
    idot += sdot4(((wv.z >> 4u) & KQ_LOW) | k5_dep(h2 >> 4u), xh.z)
    return idot + sdot4(((wv.w >> 4u) & KQ_LOW) | k5_dep(h3 >> 4u), xh.w)
}

// Q4_K: w = d*sc*q - dmin*mn*1, q in [0,15] — per 32-block xs*(d*sc*idot - dmin*mn*bsum)
[compute_shader(local_size_x=64, local_size_x_id=0, name="kq_moe_gemv_k4_spv")]
def kq_moe_gemv_k4 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let nreg = vk_meta[2u]
    let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
    if (rg < nreg * d) {
        let r = rg / d
        let row = rg % d
        let nsb = n / 256u
        let wsb0 = vk_meta[4u + r * 2u] + row * nsb
        let xsb0 = vk_meta[5u + r * 2u]
        let lane = gl_SubgroupInvocationID
        let nb = nsb * 8u
        var acc = 0.0
        var b0 = 0u
        while (b0 < nb) {
            let bb = b0 + lane
            if (bb < nb) {
                let sb = bb / 8u
                let blk = bb % 8u
                let wsb = wsb0 + sb
                let xb4 = (xsb0 + sb) * 16u + blk * 2u
                let xl = vk_xq4[xb4]
                let xh = vk_xq4[xb4 + 1u]
                let idot = kq_blk_dot(vk_wq4[wsb * 8u + blk], xl, xh)
                acc += vk_xs[xsb0 + sb] * kq_gemv_dm(wsb, blk, idot, kq_blk_sum(xl, xh))
            }
            b0 += gl_SubgroupSize
        }
        let total = subgroupAdd(acc)
        if (subgroupElect()) {
            vk_y[vk_meta[3u] + rg] = total
        }
    }
}

// Q4_0: w = d*(q - 8), q in [0,15] — the k4 nibble tiling with no min plane, so the -8 folds as an
// integer subtract against the block sums and one f16 d per 32-block scales the whole term.
[compute_shader(local_size_x=64, local_size_x_id=0, name="kq_moe_gemv_q40_spv")]
def kq_moe_gemv_q40 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let nreg = vk_meta[2u]
    let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
    if (rg < nreg * d) {
        let r = rg / d
        let row = rg % d
        let nsb = n / 256u
        let wsb0 = vk_meta[4u + r * 2u] + row * nsb
        let xsb0 = vk_meta[5u + r * 2u]
        let lane = gl_SubgroupInvocationID
        let nb = nsb * 8u
        var acc = 0.0
        var b0 = 0u
        while (b0 < nb) {
            let bb = b0 + lane
            if (bb < nb) {
                let sb = bb / 8u
                let blk = bb % 8u
                let wsb = wsb0 + sb
                let xb4 = (xsb0 + sb) * 16u + blk * 2u
                let xl = vk_xq4[xb4]
                let xh = vk_xq4[xb4 + 1u]
                let idot = kq_blk_dot(vk_wq4[wsb * 8u + blk], xl, xh)
                acc += vk_xs[xsb0 + sb] * q40_blk_d(wsb, blk) * float(idot - 8 * kq_blk_sum(xl, xh))
            }
            b0 += gl_SubgroupSize
        }
        let total = subgroupAdd(acc)
        if (subgroupElect()) {
            vk_y[vk_meta[3u] + rg] = total
        }
    }
}

// Q5_K: the k4 fold with the 5th bit OR'd in from the 32B qh region (quants in [0, 31])
[compute_shader(local_size_x=64, local_size_x_id=0, name="kq_moe_gemv_k5_spv")]
def kq_moe_gemv_k5 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let nreg = vk_meta[2u]
    let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
    if (rg < nreg * d) {
        let r = rg / d
        let row = rg % d
        let nsb = n / 256u
        let wsb0 = vk_meta[4u + r * 2u] + row * nsb
        let xsb0 = vk_meta[5u + r * 2u]
        let lane = gl_SubgroupInvocationID
        let nb = nsb * 8u
        var acc = 0.0
        var b0 = 0u
        while (b0 < nb) {
            let bb = b0 + lane
            if (bb < nb) {
                let sb = bb / 8u
                let blk = bb % 8u
                let wsb = wsb0 + sb
                let xb4 = (xsb0 + sb) * 16u + blk * 2u
                let xl = vk_xq4[xb4]
                let xh = vk_xq4[xb4 + 1u]
                let idot = k5_blk_dot(vk_wq4[wsb * 10u + blk], xl, xh, vk_wq[wsb * 40u + 32u + blk])
                acc += vk_xs[xsb0 + sb] * kq_gemv_dm(wsb, blk, idot, kq_blk_sum(xl, xh))
            }
            b0 += gl_SubgroupSize
        }
        let total = subgroupAdd(acc)
        if (subgroupElect()) {
            vk_y[vk_meta[3u] + rg] = total
        }
    }
}

// Q6_K: q = (ql | qh<<4) - 32 with per-16 SIGNED int8 sub-scales — the -32 folds as o = 32*s
// against the per-16 block sums (the CPU portable-dot formula); lo/hi nibble halves of a block
// are exactly its two 16-weight sub-scale groups
[compute_shader(local_size_x=64, local_size_x_id=0, name="kq_moe_gemv_k6_spv")]
def kq_moe_gemv_k6 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let nreg = vk_meta[2u]
    let rg = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
    if (rg < nreg * d) {
        let r = rg / d
        let row = rg % d
        let nsb = n / 256u
        let wsb0 = vk_meta[4u + r * 2u] + row * nsb
        let xsb0 = vk_meta[5u + r * 2u]
        let lane = gl_SubgroupInvocationID
        let nb = nsb * 8u
        var acc = 0.0
        var b0 = 0u
        while (b0 < nb) {
            let bb = b0 + lane
            if (bb < nb) {
                let sb = bb / 8u
                let blk = bb % 8u
                let wsb = wsb0 + sb
                let qh4 = wsb * 12u + 8u + (blk / 4u) * 2u
                let xb4 = (xsb0 + sb) * 16u + blk * 2u
                let dot = k6_blk_dot(vk_wq4[wsb * 12u + blk], vk_xq4[xb4], vk_xq4[xb4 + 1u],
                    vk_wq4[qh4], vk_wq4[qh4 + 1u], (blk % 4u) * 2u)
                let s4 = int4(unpack8(int(vk_wsu[wsb * 5u + blk / 2u])))   // 4 signed sub-scales
                let slo = (blk % 2u) == 0u ? s4.x : s4.z
                let shi = (blk % 2u) == 0u ? s4.y : s4.w
                let dd = unpackHalf2x16(vk_wsu[wsb * 5u + 4u]).x
                acc += vk_xs[xsb0 + sb] * dd * float(slo * (dot.x - 32 * dot.z) + shi * (dot.y - 32 * dot.w))
            }
            b0 += gl_SubgroupSize
        }
        let total = subgroupAdd(acc)
        if (subgroupElect()) {
            vk_y[vk_meta[3u] + rg] = total
        }
    }
}

// ===== the K-quant batch kernels =====
// Same 32x32 region-list GEMM shape as q8_moe_batch; K-quant min/offset terms fold per 16-weight
// half via inline block sums (cheaper than a staging role on this bandwidth-bound kernel).

var @workgroup bt_wsb : float[256]   // second per-(block, col) scale plane (bt_ws carries the first)

// ===== the kq tile's shared stages =====
// The four kq_moe_batch_* kernels are one 32x32 tile shape; only the w-nibble compose, the scale
// stage, and the per-block FMA (float order contractual per format) differ. Everything else is here.

// x tile stage: this thread's two uint4 chunks of the Q8_K activation rows
def private kq_bt_load_x(row0, xrow, cnt, nsb, s, word, grp : uint) {
    for (ki in range(2)) {
        let k = word + uint(ki) * 8u
        let bu = k / 2u
        let hh = k % 2u          // 0 = lo-nibble words 0..3 of the block, 1 = hi words 4..7
        var xv = uint4(0u)
        if (xrow < cnt) {
            let xb4 = ((row0 + xrow) * nsb + s) * 64u + bu * 8u + hh * 4u
            xv = uint4(vk_xq[xb4], vk_xq[xb4 + 1u], vk_xq[xb4 + 2u], vk_xq[xb4 + 3u])
        }
        bt_xw4[grp * 17u + k] = xv
    }
}

// w tile stage, 4-bit nibble compose (Q4_K's 32B/block plane; Q4_0 shares the layout)
def private kq_bt_load_w4(wsb0, wcol, d, nsb, s, word, grp : uint) {
    for (ki in range(2)) {
        let k = word + uint(ki) * 8u
        let bu = k / 2u
        let hh = k % 2u
        var wv = uint4(0u)
        if (wcol < d) {
            let wsb = wsb0 + wcol * nsb + s
            let rb4 = wsb * 32u + bu * 4u
            let r0 = vk_wq[rb4]
            let r1 = vk_wq[rb4 + 1u]
            let r2 = vk_wq[rb4 + 2u]
            let r3 = vk_wq[rb4 + 3u]
            if (hh == 0u) {
                wv = uint4(r0 & 0x0F0F0F0F, r1 & 0x0F0F0F0F, r2 & 0x0F0F0F0F, r3 & 0x0F0F0F0F)
            } else {
                wv = uint4((r0 >> 4u) & 0x0F0F0F0F, (r1 >> 4u) & 0x0F0F0F0F,
                    (r2 >> 4u) & 0x0F0F0F0F, (r3 >> 4u) & 0x0F0F0F0F)
            }
        }
        bt_ww4[grp * 17u + k] = wv
    }
}

// w tile stage, Q5_K: the k4 compose with the 5th bit OR'd in from the 40B block's qh words
def private kq_bt_load_w5(wsb0, wcol, d, nsb, s, word, grp : uint) {
    for (ki in range(2)) {
        let k = word + uint(ki) * 8u
        let bu = k / 2u
        let hh = k % 2u
        var wv = uint4(0u)
        if (wcol < d) {
            let wsb = wsb0 + wcol * nsb + s
            let rb4 = wsb * 40u + bu * 4u
            let r0 = vk_wq[rb4]
            let r1 = vk_wq[rb4 + 1u]
            let r2 = vk_wq[rb4 + 2u]
            let r3 = vk_wq[rb4 + 3u]
            let hbw = vk_wq[wsb * 40u + 32u + bu]
            let h0 = hbw & 0xFF
            let h1 = (hbw >> 8u) & 0xFF
            let h2 = (hbw >> 16u) & 0xFF
            let h3 = (hbw >> 24u) & 0xFF
            if (hh == 0u) {
                wv = uint4((r0 & 0x0F0F0F0F) | k5_dep(h0 & 0xF), (r1 & 0x0F0F0F0F) | k5_dep(h1 & 0xF),
                    (r2 & 0x0F0F0F0F) | k5_dep(h2 & 0xF), (r3 & 0x0F0F0F0F) | k5_dep(h3 & 0xF))
            } else {
                wv = uint4(((r0 >> 4u) & 0x0F0F0F0F) | k5_dep(h0 >> 4u), ((r1 >> 4u) & 0x0F0F0F0F) | k5_dep(h1 >> 4u),
                    ((r2 >> 4u) & 0x0F0F0F0F) | k5_dep(h2 >> 4u), ((r3 >> 4u) & 0x0F0F0F0F) | k5_dep(h3 >> 4u))
            }
        }
        bt_ww4[grp * 17u + k] = wv
    }
}

// w tile stage, Q6_K: 48B/block, the 2-bit highs deposited per 16-half (the lo/hi word split IS
// the sub-scale split)
def private kq_bt_load_w6(wsb0, wcol, d, nsb, s, word, grp : uint) {
    for (ki in range(2)) {
        let k = word + uint(ki) * 8u
        let bu = k / 2u
        let hh = k % 2u
        var wv = uint4(0u)
        if (wcol < d) {
            let wsb = wsb0 + wcol * nsb + s
            let rb4 = wsb * 48u + bu * 4u
            let r0 = vk_wq[rb4]
            let r1 = vk_wq[rb4 + 1u]
            let r2 = vk_wq[rb4 + 2u]
            let r3 = vk_wq[rb4 + 3u]
            let qb4 = wsb * 48u + 32u + (bu / 4u) * 8u + hh * 4u   // words w and w+4 qh adjoin
            let q0 = vk_wq[qb4]
            let q1 = vk_wq[qb4 + 1u]
            let q2 = vk_wq[qb4 + 2u]
            let q3 = vk_wq[qb4 + 3u]
            let shift = (bu % 4u) * 2u
            if (hh == 0u) {
                wv = uint4((r0 & 0x0F0F0F0F) | (((q0 >> shift) & 0x03030303) << 4u),
                    (r1 & 0x0F0F0F0F) | (((q1 >> shift) & 0x03030303) << 4u),
                    (r2 & 0x0F0F0F0F) | (((q2 >> shift) & 0x03030303) << 4u),
                    (r3 & 0x0F0F0F0F) | (((q3 >> shift) & 0x03030303) << 4u))
            } else {
                wv = uint4(((r0 >> 4u) & 0x0F0F0F0F) | (((q0 >> shift) & 0x03030303) << 4u),
                    ((r1 >> 4u) & 0x0F0F0F0F) | (((q1 >> shift) & 0x03030303) << 4u),
                    ((r2 >> 4u) & 0x0F0F0F0F) | (((q2 >> shift) & 0x03030303) << 4u),
                    ((r3 >> 4u) & 0x0F0F0F0F) | (((q3 >> shift) & 0x03030303) << 4u))
            }
        }
        bt_ww4[grp * 17u + k] = wv
    }
}

// x-scale stage: one Q8_K d per (row, superblock), threads 0..31 cover the 32 tile rows
def private kq_bt_load_xs(row0, xt, cnt, nsb, s, tid : uint) {
    if (tid < 32u) {
        var xs = 0.0
        if (xt * 32u + tid < cnt) {
            xs = vk_xs[(row0 + xt * 32u + tid) * nsb + s]
        }
        bt_xs[tid] = xs
    }
}

// w-scale stage for the d/min pair form (Q4_K/Q5_K share the 5-word wsu layout)
def private kq_bt_load_dm(wsb0, wt, d, nsb, s, slane, sblk : uint) {
    var sa = 0.0
    var sbv = 0.0
    if (wt * 32u + slane < d) {
        let wsb = wsb0 + (wt * 32u + slane) * nsb + s
        let dm = unpackHalf2x16(vk_wsu[wsb * 5u])
        let sc = int((vk_wsu[wsb * 5u + 1u + sblk / 4u] >> ((sblk % 4u) * 8u)) & 0xFF)
        let mn = int((vk_wsu[wsb * 5u + 3u + sblk / 4u] >> ((sblk % 4u) * 8u)) & 0xFF)
        sa = dm.x * float(sc)
        sbv = dm.y * float(mn)
    }
    bt_ws[sblk * 32u + slane] = sa
    bt_wsb[sblk * 32u + slane] = sbv
}

// one 32-weight block's integer tile dots: this thread's 4 output columns + the x block sum
def private kq_bt_dots(bu, grp, word : uint; var i : int4&) : int {
    var bs = 0
    for (kk in range(2)) {
        let k = bu * 2u + uint(kk)
        let xw = bt_xw4[grp * 17u + k]
        let wv0 = bt_ww4[word * 17u + k]
        let wv1 = bt_ww4[(word + 8u) * 17u + k]
        let wv2 = bt_ww4[(word + 16u) * 17u + k]
        let wv3 = bt_ww4[(word + 24u) * 17u + k]
        bs += sdot4(xw.x, 0x01010101) + sdot4(xw.y, 0x01010101) + sdot4(xw.z, 0x01010101) + sdot4(xw.w, 0x01010101)
        i.x += sdot4(wv0.x, xw.x) + sdot4(wv0.y, xw.y) + sdot4(wv0.z, xw.z) + sdot4(wv0.w, xw.w)
        i.y += sdot4(wv1.x, xw.x) + sdot4(wv1.y, xw.y) + sdot4(wv1.z, xw.z) + sdot4(wv1.w, xw.w)
        i.z += sdot4(wv2.x, xw.x) + sdot4(wv2.y, xw.y) + sdot4(wv2.z, xw.z) + sdot4(wv2.w, xw.w)
        i.w += sdot4(wv3.x, xw.x) + sdot4(wv3.y, xw.y) + sdot4(wv3.z, xw.z) + sdot4(wv3.w, xw.w)
    }
    return bs
}

// per-block FMA, d/min form: xscl * (d*sc*idot - dmin*mn*bsum), lane-wise = the scalar quadruple
def private kq_bt_fma_dm(bu, grp, word : uint; xscl : float) : float4 {
    var i = int4(0)
    let bs = kq_bt_dots(bu, grp, word, i)
    let ws = float4(bt_ws[bu * 32u + word], bt_ws[bu * 32u + word + 8u],
        bt_ws[bu * 32u + word + 16u], bt_ws[bu * 32u + word + 24u])
    let wsb = float4(bt_wsb[bu * 32u + word], bt_wsb[bu * 32u + word + 8u],
        bt_wsb[bu * 32u + word + 16u], bt_wsb[bu * 32u + word + 24u])
    return xscl * (ws * float4(i) - wsb * float(bs))
}

// per-block FMA, Q4_0 form: xscl * d * (idot - 8*bsum) — the int offset subtracts before the cast
def private kq_bt_fma_q40(bu, grp, word : uint; xscl : float) : float4 {
    var i = int4(0)
    let bs = kq_bt_dots(bu, grp, word, i)
    let ws = float4(bt_ws[bu * 32u + word], bt_ws[bu * 32u + word + 8u],
        bt_ws[bu * 32u + word + 16u], bt_ws[bu * 32u + word + 24u])
    return xscl * ws * float4(i - int4(8 * bs))
}

// y epilogue: this thread's 4 output columns, each edge-guarded
def private kq_bt_store_y(xrow, cnt, row0, d, wt, word : uint; f : float4) {
    if (xrow < cnt) {
        let yb = (row0 + xrow) * d + wt * 32u + word
        if (wt * 32u + word < d) {
            vk_y[yb] = f.x
        }
        if (wt * 32u + word + 8u < d) {
            vk_y[yb + 8u] = f.y
        }
        if (wt * 32u + word + 16u < d) {
            vk_y[yb + 16u] = f.z
        }
        if (wt * 32u + word + 24u < d) {
            vk_y[yb + 24u] = f.w
        }
    }
}

// Q4_K tile: per block acc += xs * (d*sc*idot - dmin*mn*bsum)
[compute_shader(local_size_x=256, name="kq_moe_batch_k4_spv")]
def kq_moe_batch_k4 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let reg = vk_region_rec()
    let wsb0 = reg.x
    let row0 = reg.y
    let cnt = reg.z
    let tix = reg.w
    let wtiles = (d + 31u) / 32u
    let xt = tix / wtiles
    let wt = tix % wtiles
    let nsb = n / 256u
    let tid = gl_LocalInvocationID.x
    let word = tid % 8u
    let grp = tid / 8u
    let xrow = xt * 32u + grp
    let wcol = wt * 32u + grp
    var f = float4(0.0)
    var s = 0u
    while (s < nsb) {
        kq_bt_load_x(row0, xrow, cnt, nsb, s, word, grp)
        kq_bt_load_w4(wsb0, wcol, d, nsb, s, word, grp)
        kq_bt_load_xs(row0, xt, cnt, nsb, s, tid)
        kq_bt_load_dm(wsb0, wt, d, nsb, s, tid % 32u, tid / 32u)
        barrier()
        let xscl = bt_xs[grp]
        for (bb in range(8)) {
            f += kq_bt_fma_dm(uint(bb), grp, word, xscl)
        }
        barrier()
        s++
    }
    kq_bt_store_y(xrow, cnt, row0, d, wt, word, f)
}

// Q4_0 tile: the k4 staging with one scale plane — per block acc += xs * d * (idot - 8*bsum)
[compute_shader(local_size_x=256, name="kq_moe_batch_q40_spv")]
def kq_moe_batch_q40 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let reg = vk_region_rec()
    let wsb0 = reg.x
    let row0 = reg.y
    let cnt = reg.z
    let tix = reg.w
    let wtiles = (d + 31u) / 32u
    let xt = tix / wtiles
    let wt = tix % wtiles
    let nsb = n / 256u
    let tid = gl_LocalInvocationID.x
    let word = tid % 8u
    let grp = tid / 8u
    let xrow = xt * 32u + grp
    let wcol = wt * 32u + grp
    let slane = tid % 32u
    let sblk = tid / 32u
    var f = float4(0.0)
    var s = 0u
    while (s < nsb) {
        kq_bt_load_x(row0, xrow, cnt, nsb, s, word, grp)
        kq_bt_load_w4(wsb0, wcol, d, nsb, s, word, grp)
        kq_bt_load_xs(row0, xt, cnt, nsb, s, tid)
        // q40's scale stage: one f16 d per (block, col)
        var sa = 0.0
        if (wt * 32u + slane < d) {
            sa = q40_blk_d(wsb0 + (wt * 32u + slane) * nsb + s, sblk)
        }
        bt_ws[sblk * 32u + slane] = sa
        barrier()
        let xscl = bt_xs[grp]
        for (bb in range(8)) {
            f += kq_bt_fma_q40(uint(bb), grp, word, xscl)
        }
        barrier()
        s++
    }
    kq_bt_store_y(xrow, cnt, row0, d, wt, word, f)
}

// Q5_K tile: the k4 compose with the 5th bit OR'd in from the qh region
[compute_shader(local_size_x=256, name="kq_moe_batch_k5_spv")]
def kq_moe_batch_k5 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let reg = vk_region_rec()
    let wsb0 = reg.x
    let row0 = reg.y
    let cnt = reg.z
    let tix = reg.w
    let wtiles = (d + 31u) / 32u
    let xt = tix / wtiles
    let wt = tix % wtiles
    let nsb = n / 256u
    let tid = gl_LocalInvocationID.x
    let word = tid % 8u
    let grp = tid / 8u
    let xrow = xt * 32u + grp
    let wcol = wt * 32u + grp
    var f = float4(0.0)
    var s = 0u
    while (s < nsb) {
        kq_bt_load_x(row0, xrow, cnt, nsb, s, word, grp)
        kq_bt_load_w5(wsb0, wcol, d, nsb, s, word, grp)
        kq_bt_load_xs(row0, xt, cnt, nsb, s, tid)
        kq_bt_load_dm(wsb0, wt, d, nsb, s, tid % 32u, tid / 32u)
        barrier()
        let xscl = bt_xs[grp]
        for (bb in range(8)) {
            f += kq_bt_fma_dm(uint(bb), grp, word, xscl)
        }
        barrier()
        s++
    }
    kq_bt_store_y(xrow, cnt, row0, d, wt, word, f)
}

// Q6_K tile: composed 6-bit quants in [0, 63], the -32 folded per 16-half as slo/shi vs the
// half's block sum — the lo/hi word split (j 0..3 / 4..7) IS the sub-scale split
[compute_shader(local_size_x=256, name="kq_moe_batch_k6_spv")]
def kq_moe_batch_k6 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let reg = vk_region_rec()
    let wsb0 = reg.x
    let row0 = reg.y
    let cnt = reg.z
    let tix = reg.w
    let wtiles = (d + 31u) / 32u
    let xt = tix / wtiles
    let wt = tix % wtiles
    let nsb = n / 256u
    let tid = gl_LocalInvocationID.x
    let word = tid % 8u
    let grp = tid / 8u
    let xrow = xt * 32u + grp
    let wcol = wt * 32u + grp
    let slane = tid % 32u
    let sblk = tid / 32u
    var f0 = 0.0
    var f1 = 0.0
    var f2 = 0.0
    var f3 = 0.0
    var s = 0u
    while (s < nsb) {
        kq_bt_load_x(row0, xrow, cnt, nsb, s, word, grp)
        kq_bt_load_w6(wsb0, wcol, d, nsb, s, word, grp)
        kq_bt_load_xs(row0, xt, cnt, nsb, s, tid)
        // k6's scale stage: signed i8 sub-scales per 16-half against one block d
        var sa = 0.0
        var sbv = 0.0
        if (wt * 32u + slane < d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let s4 = int4(unpack8(int(vk_wsu[wsb * 5u + sblk / 2u])))
            let slo = (sblk % 2u) == 0u ? s4.x : s4.z
            let shi = (sblk % 2u) == 0u ? s4.y : s4.w
            let dd = unpackHalf2x16(vk_wsu[wsb * 5u + 4u]).x
            sa = dd * float(slo)
            sbv = dd * float(shi)
        }
        bt_ws[sblk * 32u + slane] = sa
        bt_wsb[sblk * 32u + slane] = sbv
        barrier()
        let xscl = bt_xs[grp]
        for (bb in range(8)) {
            let bu = uint(bb)
            let kL = bu * 2u
            let kH = bu * 2u + 1u
            let xwL = bt_xw4[grp * 17u + kL]
            let xwH = bt_xw4[grp * 17u + kH]
            let blo = sdot4(xwL.x, 0x01010101) + sdot4(xwL.y, 0x01010101) + sdot4(xwL.z, 0x01010101) + sdot4(xwL.w, 0x01010101)
            let bhi = sdot4(xwH.x, 0x01010101) + sdot4(xwH.y, 0x01010101) + sdot4(xwH.z, 0x01010101) + sdot4(xwH.w, 0x01010101)
            let wL0 = bt_ww4[word * 17u + kL]
            let wL1 = bt_ww4[(word + 8u) * 17u + kL]
            let wL2 = bt_ww4[(word + 16u) * 17u + kL]
            let wL3 = bt_ww4[(word + 24u) * 17u + kL]
            let wH0 = bt_ww4[word * 17u + kH]
            let wH1 = bt_ww4[(word + 8u) * 17u + kH]
            let wH2 = bt_ww4[(word + 16u) * 17u + kH]
            let wH3 = bt_ww4[(word + 24u) * 17u + kH]
            let l0 = sdot4(wL0.x, xwL.x) + sdot4(wL0.y, xwL.y) + sdot4(wL0.z, xwL.z) + sdot4(wL0.w, xwL.w)
            let l1 = sdot4(wL1.x, xwL.x) + sdot4(wL1.y, xwL.y) + sdot4(wL1.z, xwL.z) + sdot4(wL1.w, xwL.w)
            let l2 = sdot4(wL2.x, xwL.x) + sdot4(wL2.y, xwL.y) + sdot4(wL2.z, xwL.z) + sdot4(wL2.w, xwL.w)
            let l3 = sdot4(wL3.x, xwL.x) + sdot4(wL3.y, xwL.y) + sdot4(wL3.z, xwL.z) + sdot4(wL3.w, xwL.w)
            let h0 = sdot4(wH0.x, xwH.x) + sdot4(wH0.y, xwH.y) + sdot4(wH0.z, xwH.z) + sdot4(wH0.w, xwH.w)
            let h1 = sdot4(wH1.x, xwH.x) + sdot4(wH1.y, xwH.y) + sdot4(wH1.z, xwH.z) + sdot4(wH1.w, xwH.w)
            let h2 = sdot4(wH2.x, xwH.x) + sdot4(wH2.y, xwH.y) + sdot4(wH2.z, xwH.z) + sdot4(wH2.w, xwH.w)
            let h3 = sdot4(wH3.x, xwH.x) + sdot4(wH3.y, xwH.y) + sdot4(wH3.z, xwH.z) + sdot4(wH3.w, xwH.w)
            f0 += xscl * (bt_ws[bu * 32u + word] * float(l0 - 32 * blo) + bt_wsb[bu * 32u + word] * float(h0 - 32 * bhi))
            f1 += xscl * (bt_ws[bu * 32u + word + 8u] * float(l1 - 32 * blo) + bt_wsb[bu * 32u + word + 8u] * float(h1 - 32 * bhi))
            f2 += xscl * (bt_ws[bu * 32u + word + 16u] * float(l2 - 32 * blo) + bt_wsb[bu * 32u + word + 16u] * float(h2 - 32 * bhi))
            f3 += xscl * (bt_ws[bu * 32u + word + 24u] * float(l3 - 32 * blo) + bt_wsb[bu * 32u + word + 24u] * float(h3 - 32 * bhi))
        }
        barrier()
        s++
    }
    kq_bt_store_y(xrow, cnt, row0, d, wt, word, float4(f0, f1, f2, f3))
}

// vk_kernel_access_spec is a [call_macro] over the CALLING module's globals + kernels — it must
// expand in this module, next to what it scans; the hazard rail reads it through this function
def vk_kernels_access_spec : string => vk_kernel_access_spec()
