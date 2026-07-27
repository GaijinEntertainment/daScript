options gen2
options _comment_hygiene = true

module dasllama_math_vulkan shared public

require dasllama/dasllama_math
require dasllama/dasllama_gemm_schema  // kq_qsb/kq_ssb + the q8/kq plane consts — the stride source
require dasllama/dasllama_par
require dasllama/dasllama_vulkan_lens
require vulkan
require vulkan/vulkan_boost
require spirv/spirv_shader
require spirv/spirv_builtins public
require math
require strings
require daslib/strings_boost
require daslib/fio

// dasLLAMA Vulkan GPU tier — per-layer MoE expert-stack offload. The loader gathers offloaded
// layers' PREPARED planes into row-major device planes — q8 stacks as int8 quant words + f16
// scales, native K-quant stacks (k4/k5/k6/q40, fmt-tagged) as superblock quant payloads + decoded
// 20B scale rows — and hands them to vk_moe_upload_stack (moe_gpu_upload_resident — every load
// rail, gguf and mapped image alike); the MoE dispatches then route those layers' whole expert
// FFN chain (gate+up GEMMs -> act -> requant -> down) through vk_moe_ffn — decode row counts on
// the GEMV kernels, prefill batches on the per-format tiled GEMMs (q8 and native kq
// alike). Activations arrive in the layer's own form: Q8_0 per-32 scales for q8
// stacks, Q8_K per-256 for kq. Needs 8/16-bit SSBO storage + compute subgroups +
// shaderIntegerDotProduct (probed; any absent = tier off, CPU fallback).
//
// STREAMED layers (DASLLAMA_GPU_MOE_STREAM=N, the N layers below the resident set): the loader
// keeps their device-layout planes in pinned host mirrors; per prefill each layer's stacks DMA
// into one of two device slots whose pseudo-stacks re-base onto the layer, so the batch rail
// runs verbatim. After a layer's fenced chain, the NEXT layer's copy submits UNFENCED and rides
// the GPU-idle window while the CPU runs attention — the following chain's transfer->compute
// barrier orders it (in-order queue). Decode of streamed layers stays on the CPU planes.
//
// Submission model (v2): every stack carries its own descriptor set + host meta/xq/xs/y buffers,
// so descriptors are written ONCE at upload and command buffers are PRE-RECORDED (per stack, plus
// one per gate+up pair — the fused form the engine dispatches). Per token a dispatch is: refresh
// the mapped meta/activation contents, resubmit the recorded buffer, wait on the ONE reusable
// fence, memcpy y back. No per-dispatch allocation, recording, descriptor writes, or queue drain.
// Enabled by DASLLAMA_GPU_MOE_LAYERS=<K> (expert stacks of the LAST K layers).
//
// The registered "vulkan" KernelBackend below stays a named-only stub (priority -1, GEMV slots
// panic) — the MoE tier hooks are the real integration surface.

// ===== the kernel =====
// One SUBGROUP per output row of one region. Weights arrive as TWO row-major planes — packed
// int8 quant words + a float16 scale plane (16-bit storage) — NOT 34-byte blocks: an
// interleaved block layout byte-loads at stride 34 across the warp and measured ~21GB/s on the
// 3060 Ti (~1.2ms per dispatch — the real cost behind stage-1a's "1.35ms/dispatch"). Here 8
// lanes cooperate per 32-weight block (lane = one 4-byte word), so every warp load is 128
// CONSECUTIVE bytes — one transaction. The block's integer dot stays exact — sdot4 (dp4a) per
// word, then a 3-step shuffleXor fold across the 8 lanes; lane 0 applies the f16*f32 scales.
// Params ride in vk_meta (NOT push constants — the generated pusher drops small trailing
// scalars, dasVulkan ROADMAP): [0]=n [1]=d [2]=nregions [3]=y base row (deltanet step chains
// route two GEMVs into one shared y buffer; every other fill zeroes it), then
// (rel_wblock, xblock) uint pairs per region — 32-weight BLOCK offsets (weight blocks relative
// to the resident stack, activation blocks into vk_xq/vk_xs).

var @ssbo @binding = 0 vk_wq : array<uint>
var @ssbo @binding = 0 vk_wq4 : array<uint4>   // vec4 view of the same weight words (16B loads)
var @ssbo @binding = 1 vk_ws : array<float16>
var @ssbo @binding = 2 vk_meta : array<uint>
var @ssbo @binding = 3 vk_xq : array<uint>
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

[compute_shader(local_size_x=256, name="q8_moe_batch_spv")]
def q8_moe_batch {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let rid = vk_meta[vk_meta[3u] + gl_WorkGroupID.x]
    let rb = 4u + rid * 4u
    let wblk0 = vk_meta[rb]
    let row0 = vk_meta[rb + 1u]
    let cnt = vk_meta[rb + 2u]
    let tix = gl_WorkGroupID.x - vk_meta[rb + 3u]
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
    let rid = vk_meta[vk_meta[3u] + gl_WorkGroupID.x]
    let rb = 4u + rid * 4u
    let wblk0 = vk_meta[rb]
    let row0 = vk_meta[rb + 1u]
    let cnt = vk_meta[rb + 2u]
    let tix = gl_WorkGroupID.x - vk_meta[rb + 3u]
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
    let rid = vk_meta[vk_meta[3u] + gl_WorkGroupID.x]
    let rb = 4u + rid * 4u
    let wblk0 = vk_meta[rb]
    let row0 = vk_meta[rb + 1u]
    let cnt = vk_meta[rb + 2u]
    let tix = gl_WorkGroupID.x - vk_meta[rb + 3u]
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
    let rid = vk_meta[vk_meta[3u] + gl_WorkGroupID.x]
    let rb = 4u + rid * 4u
    let wblk0 = vk_meta[rb]
    let row0 = vk_meta[rb + 1u]
    let cnt = vk_meta[rb + 2u]
    let tix = gl_WorkGroupID.x - vk_meta[rb + 3u]
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
    let rid = vk_meta[vk_meta[3u] + gl_WorkGroupID.x]
    let rb = 4u + rid * 4u
    let wblk0 = vk_meta[rb]
    let row0 = vk_meta[rb + 1u]
    let tix = gl_WorkGroupID.x - vk_meta[rb + 3u]
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
    let rid = vk_meta[vk_meta[3u] + gl_WorkGroupID.x]
    let rb = 4u + rid * 4u
    let wblk0 = vk_meta[rb]
    let row0 = vk_meta[rb + 1u]
    let cnt = vk_meta[rb + 2u]
    let tix = gl_WorkGroupID.x - vk_meta[rb + 3u]
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
    let rid = vk_meta[vk_meta[3u] + gl_WorkGroupID.x]
    let rb = 4u + rid * 4u
    let wsb0 = vk_meta[rb]
    let row0 = vk_meta[rb + 1u]
    let cnt = vk_meta[rb + 2u]
    let tix = gl_WorkGroupID.x - vk_meta[rb + 3u]
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
    let rid = vk_meta[vk_meta[3u] + gl_WorkGroupID.x]
    let rb = 4u + rid * 4u
    let wblk0 = vk_meta[rb]           // region base in 32-weight blocks (= 34B array elements)
    let row0 = vk_meta[rb + 1u]       // window-relative activation row base
    let cnt = vk_meta[rb + 2u]
    let tix = gl_WorkGroupID.x - vk_meta[rb + 3u]
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
        var m = max(max(abs(v0), abs(v1)), max(abs(v2), abs(v3)))
        m = max(m, subgroupShuffleXor(m, 1u))
        m = max(m, subgroupShuffleXor(m, 2u))
        m = max(m, subgroupShuffleXor(m, 4u))
        let d = m / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        if (lane % 8u == 0u) {
            vk_xs[b] = d
        }
        let q0 = int(round(v0 * id))
        let q1 = int(round(v1 * id))
        let q2 = int(round(v2 * id))
        let q3 = int(round(v3 * id))
        let w = ((uint(q0) & 0xFFu) | ((uint(q1) & 0xFFu) << 8u)
            | ((uint(q2) & 0xFFu) << 16u) | ((uint(q3) & 0xFFu) << 24u))
        vk_xq[b * 8u + lane % 8u] = w
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
        m = max(m, subgroupShuffleXor(m, 1u))
        m = max(m, subgroupShuffleXor(m, 2u))
        m = max(m, subgroupShuffleXor(m, 4u))
        m = max(m, subgroupShuffleXor(m, 8u))
        m = max(m, subgroupShuffleXor(m, 16u))
        let d = m / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        if (lane == 0u) {
            vk_xs[b] = d
        }
        for [unroll] (r in range(8)) {
            let q = int(round(vals[r] * id))
            let b8 = uint(q) & 0xFFu
            let p = b8 | (subgroupShuffleXor(b8, 1u) << 8u)
            let w = p | (subgroupShuffleXor(p, 2u) << 16u)
            if (lane % 4u == 0u) {
                vk_xq[b * 64u + uint(r) * 8u + lane / 4u] = w
            }
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
        let v0 = vk_upf[eb]
        let v1 = vk_upf[eb + 1u]
        let v2 = vk_upf[eb + 2u]
        let v3 = vk_upf[eb + 3u]
        var m = max(max(abs(v0), abs(v1)), max(abs(v2), abs(v3)))
        m = max(m, subgroupShuffleXor(m, 1u))
        m = max(m, subgroupShuffleXor(m, 2u))
        m = max(m, subgroupShuffleXor(m, 4u))
        let d = m / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        if (lane % 8u == 0u) {
            vk_xs[b] = d
        }
        let q0 = int(round(v0 * id))
        let q1 = int(round(v1 * id))
        let q2 = int(round(v2 * id))
        let q3 = int(round(v3 * id))
        let w = ((uint(q0) & 0xFFu) | ((uint(q1) & 0xFFu) << 8u)
            | ((uint(q2) & 0xFFu) << 16u) | ((uint(q3) & 0xFFu) << 24u))
        vk_xq[b * 8u + lane % 8u] = w
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
    var i = tid
    while (i < 8u * hs) {
        let qr = q0 + i / hs
        fa_q[i] = qr < rows ? vk_upf[qr * qd + h * hs + i % hs] : 0.0
        i += 256u
    }
    if (tid < 8u) {
        fa_m[tid] = -3.0e38
        fa_l[tid] = 0.0
    }
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
    var k0 = 0u
    while (k0 < kend) {
        i = tid
        while (i < 32u * hs) {
            let kr = k0 + i / hs
            fa_kv[(i / hs) * 257u + i % hs] = kr < kend ? vk_wsf[kr * kvd + kvh * hs + i % hs] : 0.0
            i += 256u
        }
        barrier()
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
        fa_p[r * 32u + c] = (gk <= gq && q0 + r < rows) ? s * scale : -3.0e38
        barrier()
        if (tid < 8u) {   // per-row online-softmax update: max, rescale, exponentiate in place
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

[compute_shader(local_size_x=256, name="ar_add_rms_spv")]
def ar_add_rms {
    let dim = vk_meta[0u]
    let add_on = vk_meta[1u]
    let woff = vk_meta[2u]   // norm-weight row base (resident driver shares one norms buffer)
    let eps = vk_xs[0u]
    let ascale = vk_xs[1u]
    let tid = gl_LocalInvocationID.x
    var ss = 0.0
    var k = tid
    while (k < dim) {
        let vv = add_on != 0u ? (vk_upf[k] + vk_wsf[k]) * ascale : vk_upf[k]
        vk_upf[k] = vv
        ar_row[k] = vv
        ss += vv * vv
        k += 256u
    }
    ss = subgroupAdd(ss)
    if (gl_SubgroupInvocationID == 0u) {
        ar_part[gl_SubgroupID] = ss
    }
    barrier()
    if (tid == 0u) {
        var tot = 0.0
        var sgi = 0u
        while (sgi < gl_NumSubgroups) {
            tot += ar_part[sgi]
            sgi++
        }
        ar_inv[0u] = 1.0 / sqrt(tot / float(dim) + eps)   // the CPU rmsnorm's own form, not rsqrt
    }
    barrier()
    let inv = ar_inv[0u]
    k = tid
    while (k < dim) {
        vk_y[k] = vk_xqf[woff + k] * (ar_row[k] * inv)
        k += 256u
    }
}

// batched add+rmsnorm: one workgroup per row (@workgroup arrays are per-wg, rows independent).
// Same math as ar_add_rms; row = gl_WorkGroupID.x offsets x/a/y by row*dim. Norm weight is shared.
[compute_shader(local_size_x=256, name="ar_add_rms_b_spv")]
def ar_add_rms_b {
    let dim = vk_meta[0u]
    let add_on = vk_meta[1u]
    let woff = vk_meta[2u]
    let eps = vk_xs[0u]
    let ascale = vk_xs[1u]
    let row = gl_WorkGroupID.x
    let base = row * dim
    let tid = gl_LocalInvocationID.x
    var ss = 0.0
    var k = tid
    while (k < dim) {
        let vv = add_on != 0u ? (vk_upf[base + k] + vk_wsf[base + k]) * ascale : vk_upf[base + k]
        vk_upf[base + k] = vv
        ar_row[k] = vv
        ss += vv * vv
        k += 256u
    }
    ss = subgroupAdd(ss)
    if (gl_SubgroupInvocationID == 0u) {
        ar_part[gl_SubgroupID] = ss
    }
    barrier()
    if (tid == 0u) {
        var tot = 0.0
        var sgi = 0u
        while (sgi < gl_NumSubgroups) {
            tot += ar_part[sgi]
            sgi++
        }
        ar_inv[0u] = 1.0 / sqrt(tot / float(dim) + eps)
    }
    barrier()
    let inv = ar_inv[0u]
    k = tid
    while (k < dim) {
        vk_y[base + k] = vk_xqf[woff + k] * (ar_row[k] * inv)
        k += 256u
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
    ss = subgroupAdd(ss)
    if (gl_SubgroupInvocationID == 0u) {
        ar_part[gl_SubgroupID] = ss
    }
    barrier()
    if (tid == 0u) {
        var tot = 0.0
        var sgi = 0u
        while (sgi < gl_NumSubgroups) {
            tot += ar_part[sgi]
            sgi++
        }
        ar_inv[0u] = 1.0 / sqrt(tot / float(hs) + vk_xs[0u])
    }
    barrier()
    if (tid < hs) {
        let inv = ar_inv[0u]
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
    var i = tid
    while (i < 8u * hs) {
        let qr = q0 + i / hs
        fa_q[i] = qr < npos ? vk_upf[qr * qd + h * hs + i % hs] : 0.0
        i += 256u
    }
    if (tid < 8u) {
        fa_m[tid] = -3.0e38
        fa_l[tid] = 0.0
    }
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
    var k0 = 0u
    while (k0 < kend) {
        i = tid
        while (i < 32u * hs) {
            let kr = k0 + i / hs
            fa_kv[(i / hs) * 257u + i % hs] = kr < kend ? vk_wsf[kbase + kr * kvd + kvh * hs + i % hs] : 0.0
            i += 256u
        }
        barrier()
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
        fa_p[r * 32u + c] = (gk <= gq && q0 + r < npos) ? s * scale : -3.0e38
        barrier()
        if (tid < 8u) {   // per-row online-softmax update: max, rescale, exponentiate in place
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
        m = max(m, subgroupShuffleXor(m, 1u))
        m = max(m, subgroupShuffleXor(m, 2u))
        m = max(m, subgroupShuffleXor(m, 4u))
        m = max(m, subgroupShuffleXor(m, 8u))
        m = max(m, subgroupShuffleXor(m, 16u))
        let d = m / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        if (lane == 0u) {
            vk_xs[b] = d
        }
        for [unroll] (r in range(8)) {
            let q = int(round(vals[r] * id))
            let b8 = uint(q) & 0xFFu
            let p = b8 | (subgroupShuffleXor(b8, 1u) << 8u)
            let w = p | (subgroupShuffleXor(p, 2u) << 16u)
            if (lane % 4u == 0u) {
                vk_xq[b * 64u + uint(r) * 8u + lane / 4u] = w
            }
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
        let j = lane % 4u
        let brel = lane / 4u
        let bstride = gl_SubgroupSize / 4u
        let nb = nsb * 8u
        var acc = 0.0
        var b0 = 0u
        while (b0 < nb) {
            let bb = b0 + brel
            if (bb < nb) {
                let sb = bb / 8u
                let blk = bb % 8u
                let wsb = wsb0 + sb
                let w = vk_wq[wsb * 32u + blk * 4u + j]
                let xw = (xsb0 + sb) * 64u + blk * 8u + j
                let xlo = vk_xq[xw]
                let xhi = vk_xq[xw + 4u]
                let idot = sdot4(w & 0x0F0F0F0F, xlo) + sdot4((w >> 4u) & 0x0F0F0F0F, xhi)
                let bs = sdot4(xlo, 0x01010101) + sdot4(xhi, 0x01010101)
                let dm = unpackHalf2x16(vk_wsu[wsb * 5u])
                let sc = int((vk_wsu[wsb * 5u + 1u + blk / 4u] >> ((blk % 4u) * 8u)) & 0xFF)
                let mn = int((vk_wsu[wsb * 5u + 3u + blk / 4u] >> ((blk % 4u) * 8u)) & 0xFF)
                acc += vk_xs[xsb0 + sb] * (dm.x * float(sc * idot) - dm.y * float(mn * bs))
            }
            b0 += bstride
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
        let j = lane % 4u
        let brel = lane / 4u
        let bstride = gl_SubgroupSize / 4u
        let nb = nsb * 8u
        var acc = 0.0
        var b0 = 0u
        while (b0 < nb) {
            let bb = b0 + brel
            if (bb < nb) {
                let sb = bb / 8u
                let blk = bb % 8u
                let wsb = wsb0 + sb
                let w = vk_wq[wsb * 32u + blk * 4u + j]
                let xw = (xsb0 + sb) * 64u + blk * 8u + j
                let xlo = vk_xq[xw]
                let xhi = vk_xq[xw + 4u]
                let idot = sdot4(w & 0x0F0F0F0F, xlo) + sdot4((w >> 4u) & 0x0F0F0F0F, xhi)
                let bs = sdot4(xlo, 0x01010101) + sdot4(xhi, 0x01010101)
                let dp = unpackHalf2x16(vk_wsu[wsb * 5u + blk / 2u])   // 8 f16 d packed in words 0..3
                let dv = blk % 2u == 0u ? dp.x : dp.y
                acc += vk_xs[xsb0 + sb] * dv * float(idot - 8 * bs)
            }
            b0 += bstride
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
        let j = lane % 4u
        let brel = lane / 4u
        let bstride = gl_SubgroupSize / 4u
        let nb = nsb * 8u
        var acc = 0.0
        var b0 = 0u
        while (b0 < nb) {
            let bb = b0 + brel
            if (bb < nb) {
                let sb = bb / 8u
                let blk = bb % 8u
                let wsb = wsb0 + sb
                let w = vk_wq[wsb * 40u + blk * 4u + j]
                let hb = (vk_wq[wsb * 40u + 32u + blk] >> (j * 8u)) & 0xFF
                let xw = (xsb0 + sb) * 64u + blk * 8u + j
                let xlo = vk_xq[xw]
                let xhi = vk_xq[xw + 4u]
                let idot = (sdot4((w & 0x0F0F0F0F) | k5_dep(hb & 0xF), xlo)
                    + sdot4(((w >> 4u) & 0x0F0F0F0F) | k5_dep(hb >> 4u), xhi))
                let bs = sdot4(xlo, 0x01010101) + sdot4(xhi, 0x01010101)
                let dm = unpackHalf2x16(vk_wsu[wsb * 5u])
                let sc = int((vk_wsu[wsb * 5u + 1u + blk / 4u] >> ((blk % 4u) * 8u)) & 0xFF)
                let mn = int((vk_wsu[wsb * 5u + 3u + blk / 4u] >> ((blk % 4u) * 8u)) & 0xFF)
                acc += vk_xs[xsb0 + sb] * (dm.x * float(sc * idot) - dm.y * float(mn * bs))
            }
            b0 += bstride
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
        let j = lane % 4u
        let brel = lane / 4u
        let bstride = gl_SubgroupSize / 4u
        let nb = nsb * 8u
        var acc = 0.0
        var b0 = 0u
        while (b0 < nb) {
            let bb = b0 + brel
            if (bb < nb) {
                let sb = bb / 8u
                let blk = bb % 8u
                let wsb = wsb0 + sb
                let w = vk_wq[wsb * 48u + blk * 4u + j]
                let qhw = wsb * 48u + 32u + (blk / 4u) * 8u + j
                let shift = (blk % 4u) * 2u
                let lo6 = (w & 0x0F0F0F0F) | (((vk_wq[qhw] >> shift) & 0x03030303) << 4u)
                let hi6 = ((w >> 4u) & 0x0F0F0F0F) | (((vk_wq[qhw + 4u] >> shift) & 0x03030303) << 4u)
                let xw = (xsb0 + sb) * 64u + blk * 8u + j
                let xlo = vk_xq[xw]
                let xhi = vk_xq[xw + 4u]
                let ilo = sdot4(lo6, xlo)
                let ihi = sdot4(hi6, xhi)
                let blo = sdot4(xlo, 0x01010101)
                let bhi = sdot4(xhi, 0x01010101)
                let s4 = int4(unpack8(int(vk_wsu[wsb * 5u + blk / 2u])))   // 4 signed sub-scales
                let slo = (blk % 2u) == 0u ? s4.x : s4.z
                let shi = (blk % 2u) == 0u ? s4.y : s4.w
                let dd = unpackHalf2x16(vk_wsu[wsb * 5u + 4u]).x
                acc += vk_xs[xsb0 + sb] * dd * float(slo * (ilo - 32 * blo) + shi * (ihi - 32 * bhi))
            }
            b0 += bstride
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

// Q4_K tile: per block acc += xs * (d*sc*idot - dmin*mn*bsum)
[compute_shader(local_size_x=256, name="kq_moe_batch_k4_spv")]
def kq_moe_batch_k4 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let rid = vk_meta[vk_meta[3u] + gl_WorkGroupID.x]
    let rb = 4u + rid * 4u
    let wsb0 = vk_meta[rb]
    let row0 = vk_meta[rb + 1u]
    let cnt = vk_meta[rb + 2u]
    let tix = gl_WorkGroupID.x - vk_meta[rb + 3u]
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
        if (tid < 32u) {
            var xs = 0.0
            if (xt * 32u + tid < cnt) {
                xs = vk_xs[(row0 + xt * 32u + tid) * nsb + s]
            }
            bt_xs[tid] = xs
        }
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
        barrier()
        let xscl = bt_xs[grp]
        for (bb in range(8)) {
            let bu = uint(bb)
            var i0 = 0
            var i1 = 0
            var i2 = 0
            var i3 = 0
            var bs = 0
            for (kk in range(2)) {
                let k = bu * 2u + uint(kk)
                let xw = bt_xw4[grp * 17u + k]
                let wv0 = bt_ww4[word * 17u + k]
                let wv1 = bt_ww4[(word + 8u) * 17u + k]
                let wv2 = bt_ww4[(word + 16u) * 17u + k]
                let wv3 = bt_ww4[(word + 24u) * 17u + k]
                bs += sdot4(xw.x, 0x01010101) + sdot4(xw.y, 0x01010101) + sdot4(xw.z, 0x01010101) + sdot4(xw.w, 0x01010101)
                i0 += sdot4(wv0.x, xw.x) + sdot4(wv0.y, xw.y) + sdot4(wv0.z, xw.z) + sdot4(wv0.w, xw.w)
                i1 += sdot4(wv1.x, xw.x) + sdot4(wv1.y, xw.y) + sdot4(wv1.z, xw.z) + sdot4(wv1.w, xw.w)
                i2 += sdot4(wv2.x, xw.x) + sdot4(wv2.y, xw.y) + sdot4(wv2.z, xw.z) + sdot4(wv2.w, xw.w)
                i3 += sdot4(wv3.x, xw.x) + sdot4(wv3.y, xw.y) + sdot4(wv3.z, xw.z) + sdot4(wv3.w, xw.w)
            }
            f0 += xscl * (bt_ws[bu * 32u + word] * float(i0) - bt_wsb[bu * 32u + word] * float(bs))
            f1 += xscl * (bt_ws[bu * 32u + word + 8u] * float(i1) - bt_wsb[bu * 32u + word + 8u] * float(bs))
            f2 += xscl * (bt_ws[bu * 32u + word + 16u] * float(i2) - bt_wsb[bu * 32u + word + 16u] * float(bs))
            f3 += xscl * (bt_ws[bu * 32u + word + 24u] * float(i3) - bt_wsb[bu * 32u + word + 24u] * float(bs))
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

// Q4_0 tile: the k4 staging with one scale plane — per block acc += xs * d * (idot - 8*bsum)
[compute_shader(local_size_x=256, name="kq_moe_batch_q40_spv")]
def kq_moe_batch_q40 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let rid = vk_meta[vk_meta[3u] + gl_WorkGroupID.x]
    let rb = 4u + rid * 4u
    let wsb0 = vk_meta[rb]
    let row0 = vk_meta[rb + 1u]
    let cnt = vk_meta[rb + 2u]
    let tix = gl_WorkGroupID.x - vk_meta[rb + 3u]
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
        if (tid < 32u) {
            var xs = 0.0
            if (xt * 32u + tid < cnt) {
                xs = vk_xs[(row0 + xt * 32u + tid) * nsb + s]
            }
            bt_xs[tid] = xs
        }
        var sa = 0.0
        if (wt * 32u + slane < d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let dp = unpackHalf2x16(vk_wsu[wsb * 5u + sblk / 2u])
            sa = sblk % 2u == 0u ? dp.x : dp.y
        }
        bt_ws[sblk * 32u + slane] = sa
        barrier()
        let xscl = bt_xs[grp]
        for (bb in range(8)) {
            let bu = uint(bb)
            var i0 = 0
            var i1 = 0
            var i2 = 0
            var i3 = 0
            var bs = 0
            for (kk in range(2)) {
                let k = bu * 2u + uint(kk)
                let xw = bt_xw4[grp * 17u + k]
                let wv0 = bt_ww4[word * 17u + k]
                let wv1 = bt_ww4[(word + 8u) * 17u + k]
                let wv2 = bt_ww4[(word + 16u) * 17u + k]
                let wv3 = bt_ww4[(word + 24u) * 17u + k]
                bs += sdot4(xw.x, 0x01010101) + sdot4(xw.y, 0x01010101) + sdot4(xw.z, 0x01010101) + sdot4(xw.w, 0x01010101)
                i0 += sdot4(wv0.x, xw.x) + sdot4(wv0.y, xw.y) + sdot4(wv0.z, xw.z) + sdot4(wv0.w, xw.w)
                i1 += sdot4(wv1.x, xw.x) + sdot4(wv1.y, xw.y) + sdot4(wv1.z, xw.z) + sdot4(wv1.w, xw.w)
                i2 += sdot4(wv2.x, xw.x) + sdot4(wv2.y, xw.y) + sdot4(wv2.z, xw.z) + sdot4(wv2.w, xw.w)
                i3 += sdot4(wv3.x, xw.x) + sdot4(wv3.y, xw.y) + sdot4(wv3.z, xw.z) + sdot4(wv3.w, xw.w)
            }
            let off = 8 * bs
            f0 += xscl * bt_ws[bu * 32u + word] * float(i0 - off)
            f1 += xscl * bt_ws[bu * 32u + word + 8u] * float(i1 - off)
            f2 += xscl * bt_ws[bu * 32u + word + 16u] * float(i2 - off)
            f3 += xscl * bt_ws[bu * 32u + word + 24u] * float(i3 - off)
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

// Q5_K tile: the k4 compose with the 5th bit OR'd in from the qh region
[compute_shader(local_size_x=256, name="kq_moe_batch_k5_spv")]
def kq_moe_batch_k5 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let rid = vk_meta[vk_meta[3u] + gl_WorkGroupID.x]
    let rb = 4u + rid * 4u
    let wsb0 = vk_meta[rb]
    let row0 = vk_meta[rb + 1u]
    let cnt = vk_meta[rb + 2u]
    let tix = gl_WorkGroupID.x - vk_meta[rb + 3u]
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
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let hh = k % 2u
            var xv = uint4(0u)
            if (xrow < cnt) {
                let xb4 = ((row0 + xrow) * nsb + s) * 64u + bu * 8u + hh * 4u
                xv = uint4(vk_xq[xb4], vk_xq[xb4 + 1u], vk_xq[xb4 + 2u], vk_xq[xb4 + 3u])
            }
            bt_xw4[grp * 17u + k] = xv
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
        if (tid < 32u) {
            var xs = 0.0
            if (xt * 32u + tid < cnt) {
                xs = vk_xs[(row0 + xt * 32u + tid) * nsb + s]
            }
            bt_xs[tid] = xs
        }
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
        barrier()
        let xscl = bt_xs[grp]
        for (bb in range(8)) {
            let bu = uint(bb)
            var i0 = 0
            var i1 = 0
            var i2 = 0
            var i3 = 0
            var bs = 0
            for (kk in range(2)) {
                let k = bu * 2u + uint(kk)
                let xw = bt_xw4[grp * 17u + k]
                let wv0 = bt_ww4[word * 17u + k]
                let wv1 = bt_ww4[(word + 8u) * 17u + k]
                let wv2 = bt_ww4[(word + 16u) * 17u + k]
                let wv3 = bt_ww4[(word + 24u) * 17u + k]
                bs += sdot4(xw.x, 0x01010101) + sdot4(xw.y, 0x01010101) + sdot4(xw.z, 0x01010101) + sdot4(xw.w, 0x01010101)
                i0 += sdot4(wv0.x, xw.x) + sdot4(wv0.y, xw.y) + sdot4(wv0.z, xw.z) + sdot4(wv0.w, xw.w)
                i1 += sdot4(wv1.x, xw.x) + sdot4(wv1.y, xw.y) + sdot4(wv1.z, xw.z) + sdot4(wv1.w, xw.w)
                i2 += sdot4(wv2.x, xw.x) + sdot4(wv2.y, xw.y) + sdot4(wv2.z, xw.z) + sdot4(wv2.w, xw.w)
                i3 += sdot4(wv3.x, xw.x) + sdot4(wv3.y, xw.y) + sdot4(wv3.z, xw.z) + sdot4(wv3.w, xw.w)
            }
            f0 += xscl * (bt_ws[bu * 32u + word] * float(i0) - bt_wsb[bu * 32u + word] * float(bs))
            f1 += xscl * (bt_ws[bu * 32u + word + 8u] * float(i1) - bt_wsb[bu * 32u + word + 8u] * float(bs))
            f2 += xscl * (bt_ws[bu * 32u + word + 16u] * float(i2) - bt_wsb[bu * 32u + word + 16u] * float(bs))
            f3 += xscl * (bt_ws[bu * 32u + word + 24u] * float(i3) - bt_wsb[bu * 32u + word + 24u] * float(bs))
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

// Q6_K tile: composed 6-bit quants in [0, 63], the -32 folded per 16-half as slo/shi vs the
// half's block sum — the lo/hi word split (j 0..3 / 4..7) IS the sub-scale split
[compute_shader(local_size_x=256, name="kq_moe_batch_k6_spv")]
def kq_moe_batch_k6 {
    let n = vk_meta[0u]
    let d = vk_meta[1u]
    let rid = vk_meta[vk_meta[3u] + gl_WorkGroupID.x]
    let rb = 4u + rid * 4u
    let wsb0 = vk_meta[rb]
    let row0 = vk_meta[rb + 1u]
    let cnt = vk_meta[rb + 2u]
    let tix = gl_WorkGroupID.x - vk_meta[rb + 3u]
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
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let hh = k % 2u
            var xv = uint4(0u)
            if (xrow < cnt) {
                let xb4 = ((row0 + xrow) * nsb + s) * 64u + bu * 8u + hh * 4u
                xv = uint4(vk_xq[xb4], vk_xq[xb4 + 1u], vk_xq[xb4 + 2u], vk_xq[xb4 + 3u])
            }
            bt_xw4[grp * 17u + k] = xv
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
        if (tid < 32u) {
            var xs = 0.0
            if (xt * 32u + tid < cnt) {
                xs = vk_xs[(row0 + xt * 32u + tid) * nsb + s]
            }
            bt_xs[tid] = xs
        }
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

// ===== device state (process-lifetime; never finalized by design) =====

let private WEIGHT_CAP = 4_200_000_000l    // default ceiling — NV/WDDM heapBudget IGNORES other
                                           // processes (reported 7608 of 8192MB under a 1.7GB desktop);
                                           // over-commit = silent SysMem demotion, act role 35x slower
let private VRAM_RESERVE = 805_306_368l    // headroom below the queried heap budget when it clamps
                                           // the cap down: the tier's own scratch (~210MB batch
                                           // planes) + eviction slack
let private STAGING_BYTES = 268_435_456l   // one upload chunk (largest single memcpy)
let private XQ_BYTES = 1_048_576l          // per-stack activation quants capacity
let private XS_BYTES = 262_144l            // ... their scales
let private Y_BYTES = 1_048_576l           // per-stack output capacity
let private META_BYTES = 4_096l            // params + region table
let private MAX_STACKS = 384               // descriptor pool capacity. Dense/shexp/qkv rails add
                                           // ~250 small stacks on top of experts+cls; starving a
                                           // LATER walk section here costs far more than the pool
                                           // (a 224 cap silently cut 6 DN layers = -14% tg)
let private WG_X = 64u                     // kernel workgroup width (spec-const axis 0 can retune it)
let private BATCH_XQ_BYTES = 33_554_432l   // device-local gathered-activation scratch (chunk cap)
let private BATCH_XS_BYTES = 8_388_608l    // ... its scale plane
let private BATCH_Y_BYTES = 67_108_864l    // one output plane (two exist: gate/single + up)
let private BATCH_META_BYTES = 262_144l    // per-stack batch params + regions + workgroup map
let private BATCH_TILE = 32l               // kernel tile edge (rows and cols)

// the Q8_0 batch kernel's tile edge — 128 for the mul_mm L-tile kernel (modes 3/4), 32 otherwise.
// Meta builders MUST use the same edge the bound kernel derives (wtiles = ceil(d / edge)).
def private q8_batch_tile : int64 => g_gpu.coopmat_mode >= 3 ? 128l : BATCH_TILE

let private MM_SMALL_CNT = 192l   // measured crossover (qwen3-4B pp: sdot4 +42% at 128, L +2.5% at 192)

// mode-3 small-batch tier: 32 = sdot4 (default — beats BOTH coopmat tiles below the crossover:
// pp64 887 sdot4 / 677 M / 426 L), 64 = the coopmat M tile (the experiment arm), 128 = always-L
var private g_mm_small = -1l

def private mm_small_tile : int64 {
    if (g_mm_small < 0l) {
        let e = get_env_variable("DASLLAMA_MM_SMALL")
        g_mm_small = e == "64" ? 64l : (e == "128" ? 128l : BATCH_TILE)   // else fails closed to sdot4
    }
    return g_mm_small
}

// per-dispatch tile router (the lcpp l/m ladder, ours resolved by measurement): in mm mode a
// small-cnt or small-d dispatch drops to the small tier — the L tile starves the grid and pads
// its B panel there. Pure in (d, cnt, mode), so pipe pick and meta fill can never disagree.
def private q8_batch_tile_for(d, cnt : int64) : int64 {
    if (g_gpu.coopmat_mode < 3) {
        return q8_batch_tile()
    }
    return d <= 64l || cnt < MM_SMALL_CNT ? mm_small_tile() : 128l
}

// aligned-L eligibility: every tile interior (d and cnt both 128-multiples) — the guard-free
// pipeline. Pure in (d, cnt, mode) like the tile router, so pipe pick and meta fill agree.
def private q8_batch_aligned(d, cnt : int64) : bool
    => d % 128l == 0l && cnt % 128l == 0l && q8_batch_tile_for(d, cnt) == 128l

def private q8_batch_pipe_for(d, cnt : int64) : Pipeline
    => q8_batch_pipe(q8_batch_tile_for(d, cnt), q8_batch_aligned(d, cnt))

// resident prefill GEMM pipe: a fmt-6 role rides the cm2 twin, everything else the q8 tile router
def private pf_gemm_pipe(fmt6 : bool; d, cnt : int64) : Pipeline {
    if (fmt6) {
        return weak_copy(g_gpu.batch_cm2)
    }
    return q8_batch_pipe_for(d, cnt)
}
let private BATCH_CHUNK_ROWS = 8_192l      // dispatch row-window cap (bounds y and the wg map)
let private BATCH_HQ_BYTES = 33_554_432l   // device requantized-hidden quants (the down GEMM's activations)
let private BATCH_HS_BYTES = 8_388_608l    // ... its scale plane
let private FFN_META_BYTES = 64l           // act/requant params (element count, gelu flag, block count)
                                           // + the combine's [n, k, r0, r1] at words 4..7
let private COMB_WI_BYTES = 2_097_152l     // combine w / inv plane cap (npos*k grid entries x 4B)
let private CLS_META_BYTES = 64l           // classifier GEMV params (one region, filled once)
var private g_coopmat_mode_force = -1

//! Test hook: pin the GEMM mode (0 sdot4 / 1 f16 / 2 int8 / 3 mm) ahead of the mm default and
//! DASLLAMA_COOPMAT — must run before the tier's lazy device init. The CPU-reference tier tests
//! pin 0 so their oracles stay reference-kernel-deterministic under any router default.
def vk_force_coopmat_mode(m : int) {
    g_coopmat_mode_force = m
}

let private STREAM_RESERVE = 1_100_000_000l  // LEGACY fallback stream-slot carve — used only when
                                             // the loader never reported the model's exact need
                                             // (moe_gpu_stream_need; big-expert geometries like
                                             // GLM Air exceed this constant by design)

// deltanet chain capacities (npos chunks over DN_WINDOW windows, state carried device-side).
// Maxima match qwen35 (ds 128, 32 v-heads, conv dim 8192, dconv 4) — vk_moe_dn asserts them
// and the engine arm fail-closes on bigger models.
let private DN_CS = 64l
let private DN_MAX_DS = 128l
let private DN_MAX_HEADS = 32l
let private DN_MAX_CD = 8_192l
let private DN_MAX_DCONV = 4l
let private DN_WINDOW = 1_024l
let private DN_NCHUNKS = DN_WINDOW / DN_CS
// phase-1 slab per (head x chunk): 7 CS x ds panels + 2 CS x CS tiles + 4 CS smalls; phase-2
// per-head area: state working copy + och rows. ~155MB total — the price of the 2-phase scan
let private DN_SLAB = 7l * DN_CS * DN_MAX_DS + 2l * DN_CS * DN_CS + 4l * DN_CS
let private DN_P2_OFF = DN_MAX_HEADS * DN_NCHUNKS * DN_SLAB
let private DN_P2_SLAB = DN_MAX_DS * DN_MAX_DS + DN_CS * DN_MAX_DS
let private DN_O_OFF = DN_P2_OFF + DN_MAX_HEADS * DN_P2_SLAB
let private DN_WS_BYTES = (DN_O_OFF + DN_WINDOW * DN_MAX_HEADS * DN_MAX_DS) * 4l
let private DN_STATE_BYTES = DN_MAX_HEADS * DN_MAX_DS * DN_MAX_DS * 4l
let private DN_META_BYTES = 128l
// smalls plane float offsets, ending in the conv history region — the per-window smalls
// re-stage stops SHORT of it so it never clobbers the device-side prev-window tail
let private DN_SM_BETA = 16l
let private DN_SM_G = DN_SM_BETA + DN_WINDOW * DN_MAX_HEADS
let private DN_SM_TAPS = DN_SM_G + DN_WINDOW * DN_MAX_HEADS
let private DN_SM_WNORM = DN_SM_TAPS + DN_MAX_CD * DN_MAX_DCONV
let private DN_SM_A = DN_SM_WNORM + DN_MAX_DS
let private DN_SM_DT = DN_SM_A + DN_MAX_HEADS
let private DN_SM_HIST = DN_SM_DT + DN_MAX_HEADS
let private DN_SMALLS_BYTES = (DN_SM_HIST + DN_MAX_CD * (DN_MAX_DCONV - 1l) + 64l) * 4l
let private DN_TAIL_BYTES = DN_MAX_CD * (DN_MAX_DCONV - 1l) * 4l

// decode-step scratch: the raw β/γ staging (+ the ring parity word) and the o-row Q8_0 image
// the out GEMV reads. Step smalls carry a SECOND conv-ring image after the shared layout — the
// fused kernel reads one parity image and writes the other (readers span workgroups).
let private DND_BG_BYTES = (2l * DN_MAX_HEADS + 4l) * 4l
let private DND_OQ_BYTES = DN_MAX_HEADS * DN_MAX_DS
let private DND_OS_BYTES = (DN_MAX_HEADS * DN_MAX_DS / 32l) * 4l
let private DND_SMALLS_BYTES = DN_SMALLS_BYTES + DN_MAX_CD * (DN_MAX_DCONV - 1l) * 4l

// heat-cache pool stacks live in a SYNTHETIC element-offset space far above any real plane
// offset, so find_stack / fill_stack_meta_rows / the whole FFN GEMV rail run verbatim on them
let private HEAT_BASE = 1l << 42l
let private HEAT_STRIDE = 1l << 32l          // per pool-stack offset room (>> nslots * ege)
let private HEAT_UPLOAD_CAP = 2              // steady-state expert uploads per advise call
let private HEAT_UPLOAD_CAP_FILL = 8         // while the pool is underfull (warmup fills fast)

// attention chain capacities: roped K / raw V persist device-side at ABSOLUTE batch positions
// across windows, so later windows attend the whole batch — the engine arm gates on start_pos == 0.
// Maxima cover the qwen3 family; vk_moe_attn asserts them (fail-closed on bigger geometry).
let private AT_WINDOW = 512l
let private AT_CTX = 4_096l
let private AT_MAX_QD = 8_192l
let private AT_MAX_KV = 1_024l
let private AT_MAX_HS = 256l
let private AT_META_BYTES = 128l
let private AR_MAX_DIM = 4_096l   // the add+rms row cap — == the ar_row workgroup slab
// smalls plane float offsets: [eps, scale] consts, the per-head qk-rms weight rows, then the
// window's cos/sin rope rows packed contiguously (sin offset = AT_SM_COS + rows*half, meta [13])
let private AT_SM_RMSQ = 16l
let private AT_SM_RMSK = AT_SM_RMSQ + AT_MAX_HS
let private AT_SM_COS = AT_SM_RMSK + AT_MAX_HS
let private AT_SMALLS_BYTES = (AT_SM_COS + 2l * AT_WINDOW * (AT_MAX_HS / 2l) + 64l) * 4l

struct private HostBuf {
    buf : uint64       // VkBuffer
    mem : uint64       // VkDeviceMemory
    mapped : void?     // persistently mapped (host-coherent)
    bytes : int64
}

struct private VkStack {
    base : int64             // first weight-element offset covered (into the q8 plane)
    elems : int64            // element count (offs resolve against [base, base+elems))
    fmt : int                // int(KqFmt): 0 = q8, 1/2/3 = k4/k5/k6 — picks kernels + meta units
    wqbytes : int64          // quant plane bytes (q8: elems; kq: elems/256 * {128,160,192})
    wqbuf : uint64           // device-local row-major quants (bound as array<uint>)
    wsbuf : uint64           // device-local scale plane (q8: f16 halfwords; kq: 20B decoded rows)
    wsbytes : int64
    // decode/GEMV dispatch state (lazy — first GEMV or cls use creates it; dense-GEMM stacks
    // never do, saving ~2.3MB host + 1MB device per stack across the ~130 dense planes)
    raw_set : VkDescriptorSet   // this stack's 6 bindings, written on first GEMV use
    meta : HostBuf
    xq : HostBuf
    xs : HostBuf
    y : HostBuf              // host-cached readback staging (the same-submit DMA copy lands here)
    y_dev : uint64           // device-local y the kernel writes — see batch_y1_dev on why
    gemv_made : bool
    batch_meta : HostBuf     // prefill batch meta STAGING (lazy — first batch dispatch creates it)
    batch_meta_dev : uint64  // device copy the kernel reads — a host-visible meta costs ~26us of
                             // serialized uncached PCIe reads PER WORKGROUP (measured, 12K wgs/dispatch)
    batch_set : VkDescriptorSet   // batch bindings: stack planes + act-image scratch + a y plane
    batch_y : uint64         // which shared y plane batch_set binds (role-fixed per stack)
    batch_xq : uint64        // which activation-quant plane it binds (gate/up: gathered x; down: hq)
    batch_tile : int64       // the tile edge the last fill_batch_meta routed — the bind must match
    batch_aligned : bool     // ... and whether that fill qualified for the guard-free aligned-L pipe
    batch_made : bool
    // classifier dispatch state (lazy — set up on the first cls GEMV against this stack)
    cls_set : VkDescriptorSet
    cls_meta_dev : uint64    // device meta — vocab-row dispatches run ~150K subgroups, each
                             // reading params; host-visible meta would be uncached PCIe reads
    cls_cmd : VkCommandBuffer
    cls_made : bool
}

// one pinned-host-mirror weight stack of a STREAMED layer (device-layout planes kept in
// host-visible memory, DMA'd into a slot per prefill). Stacks arrive gate/up/down per layer
// (loader walk order, descending layers) — group g = stacks [3g, 3g+3), runtime-next = g-1.
struct private StreamStack {
    base : int64
    elems : int64
    fmt : int
    wqbytes : int64
    wsbytes : int64
    hwq : HostBuf            // pinned mirror, quant plane (transfer_src)
    hws : HostBuf            // ... scale plane
}

// one streaming slot: 3 pseudo-stacks (entries in g_gpu.stacks whose plane buffers are the
// slot's device scratch, re-based per layer so find_stack + the whole batch rail work verbatim)
struct private StreamSlot {
    group : int              // stream group currently resident in the slot (-1 = empty)
    s1 : int                 // pseudo-stack indices: gate / up / down
    s3 : int
    s2 : int
    wait_value : uint64      // timeline value of the slot's in-flight transfer-queue fill (0 = none)
}

// pre-recorded decode-form FFN chain per (gate, up, down) stack triple: GEMV pair, fused
// act+requant, down GEMV, y DMA — refreshed per token via the stacks' host-visible metas,
// resubmitted as-is
struct private FfnCmd {
    cmd : VkCommandBuffer
    actrq_set : VkDescriptorSet   // gate y_dev + up y_dev (vk_upf) -> hq/hs device planes
    down_set : VkDescriptorSet    // down stack planes + hq/hs as activations + its y_dev
    nrows : int64                 // row count cmd was recorded with
}

// one recurrent layer's pre-recorded deltanet decode-step chain + its device-resident state.
// valid: the device state/ring/colds match the engine's layer state (cold-upload when not);
// dirty: the device advanced past the CPU copy — the flush hook syncs before any CPU read.
struct private DnStep {
    state_dev : uint64       // nvh·ds·ds f32 — the fused step advances it in place (its binding 1)
    smalls_dev : uint64      // per-layer smalls: colds + the two parity ring images + the β/γ region
    z_set : VkDescriptorSet  // z GEMV rerouted into the shared y buffer (meta y base = cd)
    fused_set : VkDescriptorSet
    out_set : VkDescriptorSet
    cmd : VkCommandBuffer    // the warm step chain, recorded once — no per-token re-record
    s_qkv : int
    s_z : int
    s_o : int
    state_bytes : int64
    ring_bytes : int64
    parity : int             // ring image holding current history (the kernel writes the other)
    valid : bool
    dirty : bool
}

struct private QkvCmd {
    cmd : VkCommandBuffer   // the warm q/k/v group, recorded once (planes fixed per layer)
    k_set : VkDescriptorSet // k/v read the Q stack's shared acts image and write its shared y
    v_set : VkDescriptorSet
    sq : int
    ybytes : int64
}

struct private HeatPool {
    s1 : int = -1                 // pool stacks in the synthetic offset space (-1 = unservable layer)
    s3 : int = -1
    s2 : int = -1
    src1 : int = -1               // stream-mirror stacks (the upload source planes)
    src3 : int = -1
    src2 : int = -1
    ege : int64                   // n*nfe elements per expert per plane
    nslots : int
    slot_expert : array<int64>    // slot -> pinned expert (-1 free)
    emap : table<int64; int>      // expert -> slot
}

struct private GpuState {
    instance : Instance
    device : Device
    phys : VkPhysicalDevice
    fam : uint
    queue : VkQueue
    pool : CommandPool
    set_layout : DescriptorSetLayout
    desc_pool : DescriptorPool
    pipe_layout : PipelineLayout
    pipeline : Pipeline
    batch_pipeline : Pipeline   // prefill batched-GEMM kernel (same set/pipe layout)
    batch_coopmat_f16 : Pipeline  // prefill Q8_0 GEMM on tensor cores, f16-dequant (DASLLAMA_COOPMAT=f16)
    batch_coopmat_i8 : Pipeline   // prefill Q8_0 GEMM on tensor cores, native s8 (DASLLAMA_COOPMAT=int8)
    batch_coopmat_q40 : Pipeline  // prefill Q4_0 GEMM on tensor cores, f16-dequant (COOPMAT=f16, fmt 4)
    batch_coopmat_mm : Pipeline   // prefill Q8_0 GEMM, the mul_mm 128x128 L-tile (DASLLAMA_COOPMAT=mm)
    batch_coopmat_mm_m : Pipeline // ... its 64x64 M-tile twin, routed in for small-cnt dispatches
    batch_coopmat_mm_a : Pipeline // ... the aligned-L twin (no staging guards/edge path — 2.2x the L rate)
    batch_cm2 : Pipeline          // the cm2 decode-in-load prefill twin (mode 4, fmt-6 q8n stacks)
    gemv_q8n : Pipeline           // the q8n native-block decode GEMV (mode 4's fmt-6 groupn twin)
    xf16_pipeline : Pipeline      // cm2 chain: Q8_0 activation image -> f16 plane
    actf16_pipeline : Pipeline    // cm2 chain: act(g)*u -> f16 hidden plane (the actrq stand-in)
    f16cvt_pipeline : Pipeline    // arena cm2 chain: f32 rows -> f16 plane (the quant-role stand-in)
    actrq_pipeline : Pipeline   // fused act(g)*u + q8_0 requant of the hidden rows
    gemv_k4 : Pipeline          // native K-quant GEMVs (per-format decode kernels)
    gemv_k5 : Pipeline
    gemv_k6 : Pipeline
    gemv_q40 : Pipeline
    actrq_k_pipeline : Pipeline // fused act + Q8_K requant twin (per-256 scale) for the kq chains
    batch_k4 : Pipeline         // native K-quant prefill tiles (per-format batch kernels)
    batch_k5 : Pipeline
    batch_k6 : Pipeline
    batch_q40 : Pipeline
    comb_pipeline : Pipeline    // routed weighted-sum combine (device-side reduce, prefill batches)
    dn_conv_pipeline : Pipeline // deltanet conv + SiLU + q/k L2-norm (one workgroup per position)
    dnd_fused_pipeline : Pipeline // deltanet decode step, whole chain fused (one workgroup per v-head)
    dn_scan_p1_pipeline : Pipeline // deltanet scan phase 1: chunk-local pieces (wg per head x chunk)
    dn_scan_p2_pipeline : Pipeline // deltanet scan phase 2: sequential state chain (wg per v-head)
    dn_rq_pipeline : Pipeline   // plain Q8_0 requant of the scan's o rows (out-proj image)
    at_prep_q_pipeline : Pipeline   // attention q(|gate) deinterleave + qk-rms + partial rope
    at_prep_k_pipeline : Pipeline   // attention k rms + rope, written at absolute batch positions
    at_attn_pipeline : Pipeline     // flash-style causal attention (GQA, online softmax, out-gate)
    q8k_rq_pipeline : Pipeline      // plain Q8_K requant (the kq wo GEMM's activation image)
    ar_pipeline : Pipeline          // resident driver: fused residual-add + RMSNorm (meta[1]=0 = norm only)
    rks_pipeline : Pipeline         // resident driver: fused rope + KV-row store (f32 mirror)
    da_pipeline : Pipeline          // resident driver: decode attention over the f32 KV mirror
    ar_pipeline_b : Pipeline        // prefill: batched add+rmsnorm
    rks_b_pipeline : Pipeline       // prefill: batched rope + KV-store
    da_b_pipeline : Pipeline        // prefill: batched causal attention (generic hs)
    da_b128_pipeline : Pipeline     // prefill: the hs=128 coopmat flash-attention twin (coopmat devices)
    qkn_pipeline : Pipeline         // resident driver: per-head q/k rmsnorm (qk_norm models), decode + prefill
    staging : HostBuf
    fence : VkFence          // the one reusable submit fence (reset after every wait)
    rows_per_wg : int64      // subgroups per workgroup = output rows per workgroup
    has_coopmat : bool       // device supports VK_KHR_cooperative_matrix (the f16/int8 tensor tiles)
    has_coopmat2 : bool      // device supports NV_cooperative_matrix2 (tensor-addressed wg tiles + decode fns)
    coopmat_mode : int       // Q8_0 prefill GEMM: 0 = sdot4, 1 = f16 coopmat, 2 = int8 coopmat, 3 = mul_mm L-tile (default), 4 = cm2 decode-in-load
    weight_budget : int64    // resident-weight cap — queried heap budget minus reserve, or VRAM_BUDGET
    @do_not_delete stacks : array<VkStack>
    // the resident driver's weight arenas, one per format present (see the arena section). Separate
    // from `stacks` on purpose: the two tiers never co-arm on one model, and the arena has no
    // per-plane descriptor set to cap.
    arenas : table<int; ArenaFmt>
    // residual-stream state (lazy — the add+rms kernel's buffers and its one descriptor set)
    ar_x_dev : uint64
    ar_a_dev : uint64
    ar_w_dev : uint64
    ar_y_dev : uint64
    ar_meta : HostBuf
    ar_scal : HostBuf
    ar_host : HostBuf
    ar_set : VkDescriptorSet
    ar_ready : bool
    // arena FFN chain state (gate/up/act+requant/down as one submit against arena-resident planes)
    af_meta_g : HostBuf
    af_meta_u : HostBuf
    af_meta_d : HostBuf
    af_xq : HostBuf
    af_xs : HostBuf
    af_ya_dev : uint64        // gate output plane
    af_yb_dev : uint64        // up output plane
    af_yo_dev : uint64        // down output
    af_yo_host : HostBuf
    af_gate_set : VkDescriptorSet
    af_up_set : VkDescriptorSet
    af_actrq_set : VkDescriptorSet
    af_down_set : VkDescriptorSet
    af_ready : bool
    // rope+KV-store state: qkv in, roped q out, K/V planes = one contiguous device KV scratch
    rks_qkv_dev : uint64
    rks_q_dev : uint64
    rks_scal : HostBuf
    rks_meta : HostBuf
    rks_k_dev : uint64        // f32 K mirror (rows x kv_dim, absolute positions)
    rks_v_dev : uint64        // f32 V mirror
    rks_q_host : HostBuf
    rks_kv_host : HostBuf      // K then V readback (test/parity only; the driver keeps them device-side)
    rks_set : VkDescriptorSet
    rks_ready : bool
    // decode-attention state: q in, out; reads the rks K/V mirror planes
    da_q_dev : uint64
    da_scal : HostBuf
    da_meta : HostBuf
    da_out_dev : uint64
    da_out_host : HostBuf
    da_set : VkDescriptorSet
    da_ready : bool
    ffn_cmds : table<int64; FfnCmd>   // (gate, up, down stack idx triple) -> decode FFN chain
    resident_bytes : int64
    // heat-pinned expert cache (decode split-FFN) + the one-in-flight async FFN discipline
    heat_pools : table<int64; HeatPool>   // layer -> slot pool
    ffn_pend_s2 : int = -1        // down stack of the pending async FFN (-1 = none in flight)
    ffn_pend_bytes : int64
    qkv_cmds : table<int64; QkvCmd>   // q plane offset -> the layer's decode q/k/v group
    dev_mem : table<uint64; uint64>   // device buffer -> its VkDeviceMemory (rollback destroy needs it)
    dev_mapped : table<uint64; uint64>   // ReBAR: device buffer -> persistent CPU mapping (uploads write direct)
    rebar_type : int = -1         // DL|HV|HC memory type on the full-VRAM heap (-1 = staging path only)
    has_memprio : bool            // device enabled VK_EXT_memory_priority — allocations chain priority 1.0
    host_mem : table<uint64; uint64>  // host-visible buffer -> memory: every make_host_buf result except
                                      // the process-lifetime pair (staging, profiler readback) — the
                                      // whole-model drop sweeps this
    model_cmds : array<VkCommandBuffer>   // every alloc_cmd result — freed together at model drop
    // FFN mid-chain state (lazy — first FFN dispatch creates it)
    hq_dev : uint64          // device requantized-hidden quants (the down GEMM's activation image)
    hs_dev : uint64          // ... its scales
    ffn_meta : HostBuf       // act/requant params staging
    ffn_meta_dev : uint64
    ffn_ready : bool
    // prefill batch state (lazy — first batch dispatch creates it; decode-only runs skip it)
    batch_xq_dev : uint64    // device-local gathered-activation quants (chunk window)
    batch_xs_dev : uint64    // ... their scales
    // y is written DEVICE-LOCAL and DMA-copied out in the same submit: shader scatter-stores
    // straight into host-visible memory measured ~100MB/s (~350ms per pp512 down dispatch);
    // the copy engine moves the same bytes in ~2ms
    batch_y1_dev : uint64    // gate / single output plane (kernel writes; also the down output)
    batch_y2_dev : uint64    // up output plane (the fused pair's second write target)
    batch_y1 : HostBuf       // host-cached readback staging for batch_y1_dev
    batch_y2 : HostBuf       // ... and batch_y2_dev (idle since the FFN fold — kept for debug taps)
    batch_actrq_set : VkDescriptorSet   // batch twin of FfnCmd's fused act+requant set (y1/y2 planes)
    batch_cmd : VkCommandBuffer   // one reusable batch cmd buffer, re-recorded per submit
    batch_ready : bool
    // cm2 batch state (lazy — first q8n batch dispatch creates it; mode 4 only)
    batch_xf_dev : uint64    // f16 gathered-activation plane (the gate/up twins' B side)
    batch_hf_dev : uint64    // f16 hidden plane (the down twin's B side)
    batch_xf_set : VkDescriptorSet   // the Q8_0 -> f16 dequant dispatch
    batch_hf_set : VkDescriptorSet   // the act -> f16 dispatch
    cm2_batch_ready : bool
    // combined-output state (lazy — first combined prefill dispatch creates it)
    comb_stage : HostBuf     // staging: routing w plane + grid->bucket map (device copies at first-chunk head)
    comb_w_dev : uint64
    comb_inv_dev : uint64
    comb_y_dev : uint64      // accumulated npos x n output (the combine kernel's cross-chunk accumulator)
    comb_y : HostBuf         // host-cached readback staging
    comb_set : VkDescriptorSet
    comb_ready : bool
    // streamed-prefill state (lazy — first streamed dispatch creates the slots)
    @do_not_delete stream_stacks : array<StreamStack>
    stream_slots : array<StreamSlot>
    stream_cmd : VkCommandBuffer  // reusable plane-copy cmd (safe: a fenced chain wait always
                                  // intervenes between two recordings — see copy_stream_group)
    stream_max_gu_wq : int64      // per-plane capacity maxima across streamed stacks
    stream_max_gu_ws : int64
    stream_max_dn_wq : int64
    stream_max_dn_ws : int64
    stream_ready : bool
    stream_reserve : int64        // bytes carved out of weight_budget for the 2 stream slots
    // dedicated transfer family (phase 5): mirror->slot copies overlap compute, handed off via a
    // timeline semaphore the next compute submit waits on. -1 = single-queue mode (the old rail).
    xfer_fam : int = -1
    xfer_queue : VkQueue
    xfer_pool : CommandPool
    xfer_cmd : VkCommandBuffer
    xfer_sem : VkSemaphore
    xfer_value : uint64           // last signaled timeline value
    pend_wait : uint64            // value the NEXT compute submit must wait for (0 = none)
    // deltanet chain state (lazy — first DN dispatch creates it)
    dn_meta : HostBuf             // dn kernel params staging
    dn_meta_dev : uint64
    dn_smalls : HostBuf           // beta/g/taps/wnorm/a/dt/hist staging
    dn_smalls_dev : uint64
    dn_state_stage : HostBuf      // f32 state upload staging (chain start)
    dn_state_host : HostBuf       // host-cached state readback (chain end)
    dn_tail_host : HostBuf        // host-cached conv-tail readback (the caller's history store)
    dn_state_dev : uint64
    dn_ws_dev : uint64            // per-head scan slabs + the gated o rows
    dn_conv_set : VkDescriptorSet
    dn_scan_set : VkDescriptorSet
    dn_rq_set : VkDescriptorSet
    dn_ready : bool
    // deltanet decode-step state (lazy — first npos == 1 DN dispatch creates it)
    dnd_meta : HostBuf            // step meta staging (npos = 1 geometry, filled once)
    dnd_bg : HostBuf              // per-token raw β/γ + ring-parity staging (copied into the layer smalls)
    dnd_oq_dev : uint64           // the o row's Q8_0 image (fused-step out, the out GEMV's activations)
    dnd_os_dev : uint64
    dnd_steps : table<int64; DnStep>   // qkv plane offset -> the layer's step chain
    dnd_ready : bool
    // attention chain state (lazy — first attention dispatch creates it)
    at_meta : HostBuf             // attention kernel params staging
    at_meta_dev : uint64
    at_smalls : HostBuf           // eps/scale/rms rows + the window's cos/sin rope rows
    at_smalls_dev : uint64
    at_q_dev : uint64             // the window's normed+roped q rows
    at_g_dev : uint64             // gate rows in, attention o rows out (same element, same thread)
    at_k_dev : uint64             // roped k rows at ABSOLUTE batch positions (windows accumulate)
    at_v_dev : uint64             // raw v rows at absolute positions
    at_vraw_dev : uint64          // the v GEMM's window-relative output (device-copied into at_v_dev)
    at_kv_host : HostBuf          // host-cached k/v readback (k at 0, v at AT_WINDOW*AT_MAX_KV floats)
    at_prep_q_set : VkDescriptorSet
    at_prep_k_set : VkDescriptorSet
    at_attn_set : VkDescriptorSet
    at_rq_set : VkDescriptorSet
    at_ready : bool
}

var private g_gpu : GpuState?
var private g_init_failed = false
var private g_vk_prof = false      // DASLLAMA_GPU_PROF=1: per-piece batch-chain timing to log
var private g_vk_prof_probed = false
var private g_vk_dispatches = 0ul
var private g_vk_submits = 0ul
var private g_vk_upload_us = 0l
var private g_vk_upload_bytes = 0l

//! Lifetime vkCmdDispatch calls the tier RECORDS (not executions — a re-submitted pre-recorded
//! cmd does not advance it). Pair with vk_submit_count for per-step encode-cost deltas.
def vk_dispatch_count : uint64 => g_vk_dispatches

//! Lifetime queue submissions (real cmds + staging round-trips).
def vk_submit_count : uint64 => g_vk_submits

def private vk_prof : bool {
    if (!g_vk_prof_probed) {
        g_vk_prof_probed = true
        g_vk_prof = get_env_variable("DASLLAMA_GPU_PROF") == "1"
    }
    return g_vk_prof
}

def private dev_raw : VkDevice {
    return boost_value_to_vk(g_gpu.device)
}

// host-visible memory comes in two flavors on discrete cards: write-combined (fast CPU writes,
// ~100x SLOWER CPU reads) and host-cached. Buffers the CPU reads back (the y planes) MUST be
// cached — a 33MB memcpy out of a WC mapping measured ~400ms/layer at pp512
def private find_host_mem(type_bits : uint; cached : bool) : uint {
    if (cached) {
        var mp : VkPhysicalDeviceMemoryProperties
        vkGetPhysicalDeviceMemoryProperties(g_gpu.phys, mp)
        for (i in range(int(mp.memoryTypeCount))) {
            let fl = mp.memoryTypes[i].propertyFlags
            if ((type_bits & (1u << uint(i))) != 0u && fl.host_visible && fl.host_coherent && fl.host_cached) {
                return uint(i)
            }
        }
    }
    var want : VkMemoryPropertyFlags
    want.host_visible = true
    want.host_coherent = true
    return find_memory_type(g_gpu.phys, type_bits, want)
}

def private make_host_buf(bytes : int64; storage : bool; cached : bool = false; xfer_shared : bool = false) : HostBuf {
    var bci : BufferCreateInfo
    bci.size = uint64(bytes)
    bci.usage.storage_buffer = storage
    bci.usage.transfer_src = !storage
    bci.usage.transfer_dst = true   // readback staging targets (y DMA copies land here)
    if (xfer_shared && g_gpu.xfer_fam >= 0) {
        // stream mirrors: transfer-queue fills + compute-queue heat slices — CONCURRENT dodges ownership transfers
        bci.sharingMode = VkSharingMode.CONCURRENT
        bci.pQueueFamilyIndices <- [g_gpu.fam, uint(g_gpu.xfer_fam)]
    }
    var b <- create_buffer(g_gpu.device, bci)   // deliberately never finalized (process-lifetime)
    let req = get_buffer_memory_requirements(g_gpu.device, b)
    let mai = MemoryAllocateInfo(allocationSize = req.size,
                                 memoryTypeIndex = find_host_mem(req.memoryTypeBits, cached))
    var m <- allocate_memory(g_gpu.device, mai)
    bind_buffer_memory(g_gpu.device, b, m, 0ul)
    var mapped : void? = null
    let mf : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(dev_raw(), boost_value_to_vk(m), 0ul, uint64(bytes), mf, addr(mapped)), null)
    }
    g_gpu.host_mem |> insert(b._vk, m._vk)
    return HostBuf(buf = b._vk, mem = m._vk, mapped = mapped, bytes = bytes)
}

def private make_device_buf(bytes : int64; xfer_shared : bool = false) : uint64 {
    var bci : BufferCreateInfo
    bci.size = uint64(bytes)
    bci.usage.storage_buffer = true
    bci.usage.transfer_dst = true
    bci.usage.transfer_src = true   // y planes DMA back out (device -> host staging)
    if (xfer_shared && g_gpu.xfer_fam >= 0) {
        // stream slots: transfer-queue fills + compute-queue reads — CONCURRENT dodges ownership transfers
        bci.sharingMode = VkSharingMode.CONCURRENT
        bci.pQueueFamilyIndices <- [g_gpu.fam, uint(g_gpu.xfer_fam)]
    }
    var b <- create_buffer(g_gpu.device, bci)
    let req = get_buffer_memory_requirements(g_gpu.device, b)
    // priority 1.0 (default 0.5): under pressure the driver demotes lower-priority (desktop) memory first
    var prio = VkMemoryPriorityAllocateInfoEXT()
    prio.priority = 1.0
    if (g_gpu.rebar_type >= 0 && (req.memoryTypeBits & (1u << uint(g_gpu.rebar_type))) != 0u) {
        // ReBAR attempt — raw alloc so a decline falls back to plain device-local below
        var rmai = VkMemoryAllocateInfo()
        rmai.allocationSize = req.size
        rmai.memoryTypeIndex = uint(g_gpu.rebar_type)
        if (g_gpu.has_memprio) {
            unsafe {
                rmai.pNext = addr<void?>(prio)
            }
        }
        var rmem : VkDeviceMemory
        if (vkAllocateMemory(dev_raw(), rmai, null, rmem) == VkResult.SUCCESS) {
            vk_check(vkBindBufferMemory(dev_raw(), unsafe(reinterpret<VkBuffer>(b._vk)), rmem, 0ul), null)
            var mapped : void? = null
            let mf : VkMemoryMapFlags
            unsafe {
                vk_check(vkMapMemory(dev_raw(), rmem, 0ul, req.size, mf, addr(mapped)), null)
            }
            g_gpu.dev_mem |> insert(b._vk, unsafe(reinterpret<uint64>(rmem)))
            g_gpu.dev_mapped |> insert(b._vk, unsafe(reinterpret<uint64>(mapped)))
            return b._vk
        }
    }
    var want : VkMemoryPropertyFlags
    want.device_local = true
    var mai = MemoryAllocateInfo(allocationSize = req.size,
                                 memoryTypeIndex = find_memory_type(g_gpu.phys, req.memoryTypeBits, want))
    if (g_gpu.has_memprio) {
        unsafe {
            mai.next = addr<void?>(prio)
        }
    }
    var m <- allocate_memory(g_gpu.device, mai)
    bind_buffer_memory(g_gpu.device, b, m, 0ul)
    g_gpu.dev_mem |> insert(b._vk, m._vk)
    return b._vk
}

def private destroy_device_buf(h : uint64) {
    unsafe {
        vkDestroyBuffer(dev_raw(), reinterpret<VkBuffer>(h), null)
        if (key_exists(g_gpu.dev_mem, h)) {
            vkFreeMemory(dev_raw(), reinterpret<VkDeviceMemory>(g_gpu.dev_mem[h]), null)   // implicitly unmaps
            g_gpu.dev_mem |> erase(h)
        }
    }
    g_gpu.dev_mapped |> erase(h)
}

def private destroy_host_buf(hb : HostBuf) {   // vkFreeMemory implicitly unmaps
    unsafe {
        vkDestroyBuffer(dev_raw(), reinterpret<VkBuffer>(hb.buf), null)
        vkFreeMemory(dev_raw(), reinterpret<VkDeviceMemory>(hb.mem), null)
    }
    g_gpu.host_mem |> erase(hb.buf)
}

def private nonowning_buf(h : uint64) : Buffer {
    var b : Buffer
    b._vk = h
    return b
}

def private vk_moe_init : bool {
    if (g_init_failed) {   // checked FIRST: a partially-built g_gpu must never report ready
        return false
    }
    if (g_gpu != null) {
        return true
    }
    g_init_failed = true   // cleared only on full success
    if (volkInitialize() != 0) {
        to_log(LOG_ERROR, "dasLLAMA vulkan tier: no Vulkan loader\n")
        return false
    }
    g_gpu = new GpuState()
    g_gpu.instance <- create_instance("dasLLAMA vulkan tier", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(g_gpu.instance))
    g_gpu.phys = select_physical_device(g_gpu.instance)
    if (!storage_8_16_supported(g_gpu.phys) || !subgroup_compute_ops_supported(g_gpu.phys)
            || !integer_dot_product_supported(g_gpu.phys)) {
        to_log(LOG_ERROR, "dasLLAMA vulkan tier: device lacks 8/16-bit SSBO storage, compute subgroup ops, or shaderIntegerDotProduct — tier disabled\n")
        return false
    }
    g_gpu.fam = select_graphics_queue_family(g_gpu.phys)
    // coopmat requested whenever supported — the fa twin needs it even in sdot4 GEMM mode
    g_gpu.has_coopmat = cooperative_matrix_supported(g_gpu.phys)
    g_gpu.has_coopmat2 = g_gpu.has_coopmat && cooperative_matrix2_supported(g_gpu.phys)
    // fastest MEASURED default (mm; cm2 opt-in until it beats mm); DASLLAMA_COOPMAT overrides
    g_gpu.coopmat_mode = g_gpu.has_coopmat ? 3 : 0
    if (g_gpu.has_coopmat) {
        let mode = get_env_variable("DASLLAMA_COOPMAT")
        if (mode == "sdot4") {
            g_gpu.coopmat_mode = 0
        } elif (mode == "f16") {
            g_gpu.coopmat_mode = 1
        } elif (mode == "int8") {
            g_gpu.coopmat_mode = 2
        } elif (mode == "mm") {
            g_gpu.coopmat_mode = 3
        } elif (mode == "cm2" && g_gpu.has_coopmat2) {
            g_gpu.coopmat_mode = 4
        }
    }
    if (g_coopmat_mode_force >= 0) {
        g_gpu.coopmat_mode = g_gpu.has_coopmat ? g_coopmat_mode_force : 0
    }
    if (g_gpu.coopmat_mode == 4 && !g_gpu.has_coopmat2) {
        g_gpu.coopmat_mode = 3   // forced/requested cm2 without the extension: mm
    }
    // dedicated transfer family: streamed copies overlap compute (DASLLAMA_VK_XFERQ=0 = bisect hatch)
    var xfam = -1
    if (g_gpu.has_coopmat && get_env_variable("DASLLAMA_VK_XFERQ") != "0"
            && timeline_semaphore_supported(g_gpu.phys)) {
        xfam = select_transfer_queue_family(g_gpu.phys)
    }
    if (g_gpu.has_coopmat2 && g_gpu.coopmat_mode == 4) {
        // #77's creator: coopmat2 + BDA + vulkanMemoryModel + the query-then-enable extras
        g_gpu.device <- create_device_storage_8_16_int_dot_coopmat2(g_gpu.phys, g_gpu.fam, xfam)
        to_log(LOG_INFO, "dasLLAMA vulkan tier: Q8_0 prefill on NV_coopmat2 tensor tiles (decode-in-load)\n")
    } elif (g_gpu.has_coopmat) {
        g_gpu.device <- create_device_storage_8_16_int_dot_coopmat(g_gpu.phys, g_gpu.fam, xfam)
        if (g_gpu.coopmat_mode != 0) {
            let cm_name = g_gpu.coopmat_mode == 1 ? "f16" : (g_gpu.coopmat_mode == 2 ? "int8" : "mul_mm L-tile")
            to_log(LOG_INFO, "dasLLAMA vulkan tier: Q8_0 prefill on {cm_name} cooperative-matrix tiles\n")
        }
    } else {
        g_gpu.device <- create_device_storage_8_16_int_dot(g_gpu.phys, g_gpu.fam)
    }
    volkLoadDevice(dev_raw())
    g_gpu.queue = get_device_queue(g_gpu.device, g_gpu.fam, 0u)
    if (xfam >= 0) {
        g_gpu.xfer_fam = xfam
        g_gpu.xfer_queue = get_device_queue(g_gpu.device, uint(xfam), 0u)
        var xpci : CommandPoolCreateInfo
        xpci.queueFamilyIndex = uint(xfam)
        xpci.flags.reset_command_buffer = true
        g_gpu.xfer_pool <- create_command_pool(g_gpu.device, xpci)
        var xai = VkCommandBufferAllocateInfo()
        xai.commandPool = boost_value_to_vk(g_gpu.xfer_pool)
        xai.level = VkCommandBufferLevel.PRIMARY
        xai.commandBufferCount = 1u
        var tci = VkSemaphoreTypeCreateInfo()
        tci.semaphoreType = VkSemaphoreType.TIMELINE
        tci.initialValue = 0ul
        var sci = VkSemaphoreCreateInfo()
        unsafe {
            vk_check(vkAllocateCommandBuffers(dev_raw(), xai, addr(g_gpu.xfer_cmd)), null)
            sci.pNext = addr<void?>(tci)
            vk_check(vkCreateSemaphore(dev_raw(), sci, null, g_gpu.xfer_sem), null)
        }
        to_log(LOG_INFO, "dasLLAMA vulkan tier: transfer queue armed (family {xfam})\n")
    }
    // WDDM residency shield: allocations chain priority 1.0 (DASLLAMA_VK_MEMPRIO=0 = bisect hatch)
    g_gpu.has_memprio = (g_gpu.has_coopmat && memory_priority_supported(g_gpu.phys)
        && get_env_variable("DASLLAMA_VK_MEMPRIO") != "0")
    if (g_gpu.has_memprio) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: memory-priority residency shield armed\n")
    }
    let sg = subgroup_properties(g_gpu.phys).subgroupSize
    if (sg < 32u) {
        // the gid-mapped quant kernels reduce with xor masks up to 16 (q8k amax spans 32 lanes)
        to_log(LOG_ERROR, "dasLLAMA vulkan tier: subgroupSize {sg} < 32 — tier disabled\n")
        return false
    }
    g_gpu.rows_per_wg = int64((WG_X + sg - 1u) / sg)   // = gl_NumSubgroups for our 1D workgroup
    // env wins outright (=0 pins WEIGHT_CAP — the misreporting-driver hatch); else live budget - reserve
    g_gpu.weight_budget = WEIGHT_CAP
    let vram_mb = gpu_want_vram_mb()
    let vram_override = vram_mb > 0l || has_env_variable("DASLLAMA_GPU_VRAM_MB")
    if (vram_mb > 0l) {
        g_gpu.weight_budget = vram_mb * 1_000_000l
    }
    if (device_extension_available(g_gpu.phys, "VK_EXT_memory_budget")) {
        var mb = VkPhysicalDeviceMemoryBudgetPropertiesEXT()   // sType pre-filled by the ctor
        var mp2 = VkPhysicalDeviceMemoryProperties2()
        unsafe {
            mp2.pNext = addr<void?>(mb)
        }
        vkGetPhysicalDeviceMemoryProperties2(g_gpu.phys, mp2)
        var best = -1
        for (i in range(int(mp2.memoryProperties.memoryHeapCount))) {
            if (mp2.memoryProperties.memoryHeaps[i].flags.device_local
                    && (best < 0 || mb.heapBudget[i] > mb.heapBudget[best])) {
                best = i
            }
        }
        if (best >= 0 && mb.heapBudget[best] > 0ul) {
            let safe_cap = max(int64(mb.heapBudget[best]) - VRAM_RESERVE, 0l)
            // the honest cliff leaves 2GB of the raw heap for the desktop (6000MB healthy, 6803 demoted)
            let cliff = min(safe_cap, int64(mp2.memoryProperties.memoryHeaps[best].size) - 2_147_483_648l)
            if (!vram_override) {
                g_gpu.weight_budget = min(safe_cap, g_gpu.weight_budget)
            } elif (g_gpu.weight_budget > cliff) {
                to_log(LOG_WARNING, "dasLLAMA vulkan tier: DASLLAMA_GPU_VRAM_MB {g_gpu.weight_budget / 1000000l} MB is above the demotion cliff — WDDM will silently demote buffers to SysMem (act-role collapse); lower it to <= {cliff / 1000000l} MB\n")
            }
            to_log(LOG_INFO, "dasLLAMA vulkan tier: device heap budget {int64(mb.heapBudget[best]) / 1000000l} MB (used {int64(mb.heapUsage[best]) / 1000000l}), weight cap {g_gpu.weight_budget / 1000000l} MB\n")
        }
    }
    // stream-slot carve: exact loader-reported need, else the legacy constant (direct-upload paths)
    if (gpu_want_moe_stream() != 0l) {
        let need = moe_gpu_stream_need()
        if (need > 0l) {
            g_gpu.stream_reserve = 2l * need
        } elif (gpu_want_moe_stream() > 0l) {
            g_gpu.stream_reserve = STREAM_RESERVE
        }
        g_gpu.weight_budget = max(g_gpu.weight_budget - g_gpu.stream_reserve, 0l)
    }
    // ReBAR: DL|HV|HC on the full-VRAM heap (the >1GB check excludes the 256MiB legacy window)
    if (get_env_variable("DASLLAMA_VK_REBAR") != "0") {
        var rmp : VkPhysicalDeviceMemoryProperties
        vkGetPhysicalDeviceMemoryProperties(g_gpu.phys, rmp)
        for (i in range(int(rmp.memoryTypeCount))) {
            let fl = rmp.memoryTypes[i].propertyFlags
            if (fl.device_local && fl.host_visible && fl.host_coherent
                    && int64(rmp.memoryHeaps[int(rmp.memoryTypes[i].heapIndex)].size) > 1_073_741_824l) {
                g_gpu.rebar_type = i
                break
            }
        }
        if (g_gpu.rebar_type >= 0) {
            to_log(LOG_INFO, "dasLLAMA vulkan tier: ReBAR active — uploads write direct to VRAM (DASLLAMA_VK_REBAR=0 restores staging)\n")
        }
    }
    var pci : CommandPoolCreateInfo
    pci.queueFamilyIndex = g_gpu.fam
    pci.flags.reset_command_buffer = true   // per-stack buffers re-record if a shape ever changes
    g_gpu.pool <- create_command_pool(g_gpu.device, pci)

    // one layout, MAX_STACKS sets: each resident stack gets its own set at upload time
    var dslci : DescriptorSetLayoutCreateInfo
    dslci.pBindings |> reserve(6)
    for (bind in range(6)) {
        var b0 = DescriptorSetLayoutBinding(binding = uint(bind),
                                            descriptorType = VkDescriptorType.STORAGE_BUFFER,
                                            descriptorCount = 1u)
        b0.stageFlags.compute = true
        dslci.pBindings |> emplace(b0)
    }
    g_gpu.set_layout <- create_descriptor_set_layout(g_gpu.device, dslci)
    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = uint(5 * MAX_STACKS)   // decode + batch set per stack, 3 per FFN triple, 3 per DN step layer, batch act/rq
    var psize = DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_BUFFER, descriptorCount = uint(6 * 5 * MAX_STACKS))
    dpci.pPoolSizes |> push(psize)
    g_gpu.desc_pool <- create_descriptor_pool(g_gpu.device, dpci)

    var plci : PipelineLayoutCreateInfo
    plci.pSetLayouts <- [weak_copy(g_gpu.set_layout)]
    g_gpu.pipe_layout <- create_pipeline_layout(g_gpu.device, plci)
    g_pipe_access |> clear()
    g_gpu.pipeline <- make_kernel_pipe("q8_moe_groupn", q8_moe_groupn_spv)
    g_gpu.batch_pipeline <- make_kernel_pipe("q8_moe_batch", q8_moe_batch_spv)   // same 6-binding layout
    if (g_gpu.coopmat_mode == 1) {   // f16 tensor-core prefill GEMMs (coopmat device only)
        g_gpu.batch_coopmat_f16 <- make_kernel_pipe("q8_moe_batch_cmf16", q8_moe_batch_cmf16_spv)
        g_gpu.batch_coopmat_q40 <- make_kernel_pipe("kq_moe_batch_q40_cmf16", kq_moe_batch_q40_cmf16_spv)
    } elif (g_gpu.coopmat_mode == 2) {   // native-s8 tensor-core prefill GEMM
        g_gpu.batch_coopmat_i8 <- make_kernel_pipe("q8_moe_batch_cmi8", q8_moe_batch_cmi8_spv)
    } elif (g_gpu.coopmat_mode >= 3) {   // the mul_mm L/M-tile prefill GEMMs (routed per dispatch;
        // mode 4 keeps the whole mm ladder for fmt-0 stacks and adds the cm2 rails for fmt-6)
        g_gpu.batch_coopmat_mm <- make_kernel_pipe("q8_moe_batch_mm", q8_moe_batch_mm_spv)
        g_gpu.batch_coopmat_mm_m <- make_kernel_pipe("q8_moe_batch_mm_m", q8_moe_batch_mm_m_spv)
        g_gpu.batch_coopmat_mm_a <- make_kernel_pipe("q8_moe_batch_mm_a", q8_moe_batch_mm_a_spv)
        if (g_gpu.coopmat_mode == 4) {
            g_gpu.batch_cm2 <- make_kernel_pipe("q8n_moe_batch_cm2", q8n_moe_batch_cm2_spv)
            g_gpu.gemv_q8n <- make_kernel_pipe("q8n_moe_groupn", q8n_moe_groupn_spv)
            g_gpu.xf16_pipeline <- make_kernel_pipe("q8_moe_xf16", q8_moe_xf16_spv)
            g_gpu.actf16_pipeline <- make_kernel_pipe("q8_moe_actf16", q8_moe_actf16_spv)
            g_gpu.f16cvt_pipeline <- make_kernel_pipe("rd_f16cvt", rd_f16cvt_spv)
        }
    }
    g_gpu.actrq_pipeline <- make_kernel_pipe("q8_moe_actrq", q8_moe_actrq_spv)
    g_gpu.gemv_k4 <- make_kernel_pipe("kq_moe_gemv_k4", kq_moe_gemv_k4_spv)
    g_gpu.gemv_k5 <- make_kernel_pipe("kq_moe_gemv_k5", kq_moe_gemv_k5_spv)
    g_gpu.gemv_k6 <- make_kernel_pipe("kq_moe_gemv_k6", kq_moe_gemv_k6_spv)
    g_gpu.gemv_q40 <- make_kernel_pipe("kq_moe_gemv_q40", kq_moe_gemv_q40_spv)
    g_gpu.actrq_k_pipeline <- make_kernel_pipe("q8k_moe_actrq", q8k_moe_actrq_spv)
    g_gpu.batch_k4 <- make_kernel_pipe("kq_moe_batch_k4", kq_moe_batch_k4_spv)
    g_gpu.batch_k5 <- make_kernel_pipe("kq_moe_batch_k5", kq_moe_batch_k5_spv)
    g_gpu.batch_k6 <- make_kernel_pipe("kq_moe_batch_k6", kq_moe_batch_k6_spv)
    g_gpu.batch_q40 <- make_kernel_pipe("kq_moe_batch_q40", kq_moe_batch_q40_spv)
    g_gpu.comb_pipeline <- make_kernel_pipe("moe_combine", moe_combine_spv)
    g_gpu.dn_conv_pipeline <- make_kernel_pipe("dn_conv", dn_conv_spv)
    g_gpu.dnd_fused_pipeline <- make_kernel_pipe("dn_step_fused", dn_step_fused_spv)
    g_gpu.dn_scan_p1_pipeline <- make_kernel_pipe("dn_scan_p1", dn_scan_p1_spv)
    g_gpu.dn_scan_p2_pipeline <- make_kernel_pipe("dn_scan_p2", dn_scan_p2_spv)
    g_gpu.dn_rq_pipeline <- make_kernel_pipe("dn_requant", dn_requant_spv)
    g_gpu.at_prep_q_pipeline <- make_kernel_pipe("at_prep_q", at_prep_q_spv)
    g_gpu.at_prep_k_pipeline <- make_kernel_pipe("at_prep_k", at_prep_k_spv)
    g_gpu.at_attn_pipeline <- make_kernel_pipe("at_attn", at_attn_spv)
    g_gpu.q8k_rq_pipeline <- make_kernel_pipe("q8k_requant", q8k_requant_spv)
    g_gpu.ar_pipeline <- make_kernel_pipe("ar_add_rms", ar_add_rms_spv)
    g_gpu.rks_pipeline <- make_kernel_pipe("rope_kv_store", rope_kv_store_spv)
    g_gpu.da_pipeline <- make_kernel_pipe("da_attn", da_attn_spv)
    g_gpu.ar_pipeline_b <- make_kernel_pipe("ar_add_rms_b", ar_add_rms_b_spv)
    g_gpu.rks_b_pipeline <- make_kernel_pipe("rope_kv_store_b", rope_kv_store_b_spv)
    g_gpu.da_b_pipeline <- make_kernel_pipe("da_attn_b", da_attn_b_spv)
    if (g_gpu.has_coopmat) {
        g_gpu.da_b128_pipeline <- make_kernel_pipe("da_attn_b_h128", da_attn_b_h128_spv)
    }
    g_gpu.qkn_pipeline <- make_kernel_pipe("qk_rms", qk_rms_spv)

    var fence <- create_fence(g_gpu.device, FenceCreateInfo())   // never finalized (process-lifetime)
    g_gpu.fence = boost_value_to_vk(fence)
    g_gpu.staging = make_host_buf(STAGING_BYTES, false)
    g_gpu.host_mem |> erase(g_gpu.staging.buf)   // process-lifetime — exempt from the model-drop sweep
    g_init_failed = false
    to_log(LOG_INFO, "dasLLAMA vulkan tier: device ready (subgroup {int(sg)}, {g_gpu.rows_per_wg} rows/wg)\n")
    if (dasllama_noisy()) {
        // storage-range is the limit that actually binds the arena; allocs/descriptors do not
        var lp : VkPhysicalDeviceProperties
        vkGetPhysicalDeviceProperties(g_gpu.phys, lp)
        to_log(LOG_INFO, "dasLLAMA vulkan tier: limits allocs {lp.limits.maxMemoryAllocationCount} storage-range {lp.limits.maxStorageBufferRange} desc-storage {lp.limits.maxDescriptorSetStorageBuffers}\n")
    }
    return true
}

// ===== upload (loader hook — moe_gpu_upload_resident gathers, we just stage) =====

def private staged_upload(dst : uint64; dst_off : int64; bytes : int64) {
    var regions : array<BufferCopy>
    let rc = BufferCopy(srcOffset = 0ul, dstOffset = uint64(dst_off), size = uint64(bytes))
    regions |> push(rc)
    g_vk_submits++
    run_cmd_sync(g_gpu.device, g_gpu.pool, g_gpu.queue) $(cmd) {
        cmd_copy_buffer(cmd, nonowning_buf(g_gpu.staging.buf), nonowning_buf(dst), regions)
    }
}

// Stage `total` bytes into `dst` starting at `dst_off`. The arena packs many tensors into one
// buffer, so it needs the offset; the per-op tier's own planes start at 0. ReBAR buffers skip
// staging outright — the CPU writes the mapped VRAM; visibility comes from the next queue submit.
def private upload_region_at(dst : uint64; dst_off : int64; src : uint8 const?; total : int64) {
    let t0 = ref_time_ticks()
    if (key_exists(g_gpu.dev_mapped, dst)) {
        unsafe {
            par_memcpy(reinterpret<void?>(g_gpu.dev_mapped[dst] + uint64(dst_off)), reinterpret<void? -const>(src), total)
        }
    } else {
        var off = 0l
        while (off < total) {
            let chunk = min(total - off, STAGING_BYTES)
            unsafe {
                par_memcpy(g_gpu.staging.mapped, reinterpret<void? -const>(src + off), chunk)
            }
            staged_upload(dst, dst_off + off, chunk)
            off += chunk
        }
    }
    g_vk_upload_us += int64(get_time_usec(t0))
    g_vk_upload_bytes += total
}

def private upload_region(dst : uint64; src : uint8 const?; total : int64) {
    upload_region_at(dst, 0l, src, total)
}

def private alloc_desc_set : VkDescriptorSet {
    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(g_gpu.desc_pool)
    dsai.descriptorSetCount = 1u
    var raw_set_layout = boost_value_to_vk(g_gpu.set_layout)
    var raw : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_set_layout)
        vk_check(vkAllocateDescriptorSets(dev_raw(), dsai, addr(raw)), null)
    }
    return raw
}

def private write_buf_desc(var writes : array<WriteDescriptorSet>; set_ : VkDescriptorSet; binding : uint; buf : uint64; bytes : int64) {
    var w = WriteDescriptorSet(dstSet = vk_value_to_boost(set_), dstBinding = binding,
                               descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    var binfo = DescriptorBufferInfo(buffer = nonowning_buf(buf), range_ = uint64(bytes))
    w.pBufferInfo |> push(binfo)
    writes |> emplace(w)
}

// weight plane byte sizes for one [rows x n] stack in the tier's device layout
def private stack_plane_bytes(n, rows : int64; fmt : int) : tuple<wq : int64; ws : int64> {
    let nb = arena_blocks_for(n, rows, fmt)   // strides live in arena_block_bytes — one source
    let bb = arena_block_bytes(fmt)
    return (wq = nb * bb.wq, ws = nb * bb.ws)
}

// build a VkStack shell (weight plane buffers only — the GEMV/cls scratch is lazy, see
// ensure_gemv_scratch). Resident stacks pass their exact plane sizes; streaming pseudo-stacks
// pass slot capacities and a base of -1 (re-based per layer).
def private make_stack_shell(base, elems : int64; fmt : int; wqcap, wscap : int64; xfer_shared : bool = false) : int {
    // q8n has no scale plane (wscap 0) — floor the buffer so its descriptor slot stays valid
    var st = VkStack(base = base, elems = elems, fmt = fmt, wqbytes = wqcap,
                     wqbuf = make_device_buf(wqcap, xfer_shared), wsbuf = make_device_buf(max(wscap, 64l), xfer_shared), wsbytes = wscap)
    g_gpu.stacks |> push(st)
    return length(g_gpu.stacks) - 1
}

// the descriptor range for a stack's ws binding — the 64B floor of the shell above
def private ws_range(si : int) : int64 => max(g_gpu.stacks[si].wsbytes, 64l)

// decode/GEMV scratch for one stack (per-stack host bufs + the written 6-binding set). Lazy:
// only stacks that actually serve GEMV or cls dispatches pay for it — dense-GEMM planes never do.
def private ensure_gemv_scratch(si : int) {
    if (g_gpu.stacks[si].gemv_made) {
        return
    }
    g_gpu.stacks[si].raw_set = alloc_desc_set()
    g_gpu.stacks[si].meta = make_host_buf(META_BYTES, true)
    g_gpu.stacks[si].xq = make_host_buf(XQ_BYTES, true)
    g_gpu.stacks[si].xs = make_host_buf(XS_BYTES, true)
    g_gpu.stacks[si].y = make_host_buf(Y_BYTES, false, [cached = true])
    g_gpu.stacks[si].y_dev = make_device_buf(Y_BYTES)
    var writes : array<WriteDescriptorSet>
    write_buf_desc(writes, g_gpu.stacks[si].raw_set, 0u, g_gpu.stacks[si].wqbuf, g_gpu.stacks[si].wqbytes)
    write_buf_desc(writes, g_gpu.stacks[si].raw_set, 1u, g_gpu.stacks[si].wsbuf, ws_range(si))
    write_buf_desc(writes, g_gpu.stacks[si].raw_set, 2u, g_gpu.stacks[si].meta.buf, META_BYTES)
    write_buf_desc(writes, g_gpu.stacks[si].raw_set, 3u, g_gpu.stacks[si].xq.buf, XQ_BYTES)
    write_buf_desc(writes, g_gpu.stacks[si].raw_set, 4u, g_gpu.stacks[si].xs.buf, XS_BYTES)
    write_buf_desc(writes, g_gpu.stacks[si].raw_set, 5u, g_gpu.stacks[si].y_dev, Y_BYTES)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    // default y role; role-fluid chains (gate/up pair, qkv group, dn step) re-declare before recording
    hz_set_bits(g_gpu.stacks[si].raw_set, 0u, 0u, 0u, 0u, 0u, VHG_Y1)
    g_gpu.stacks[si].gemv_made = true
}

// stream arm: keep the gathered device-layout planes in a pinned host mirror (host-visible
// memory the copy engine DMA-streams into a slot per prefill). No VRAM spent here.
def private vk_moe_stream_stack(wq : uint8 const?; ws : uint8 const?; woff : int64; n, rows : int64; fmt : int) : bool {
    let pb = stack_plane_bytes(n, rows, fmt)
    var ss = StreamStack(base = woff, elems = rows * n, fmt = fmt, wqbytes = pb.wq, wsbytes = pb.ws,
                         hwq = make_host_buf(pb.wq, false, [xfer_shared = true]),
                         hws = make_host_buf(max(pb.ws, 64l), false, [xfer_shared = true]))
    unsafe {
        par_memcpy(ss.hwq.mapped, reinterpret<void? -const>(wq), pb.wq)
        if (pb.ws > 0l) {   // q8n mirrors carry no scale plane
            par_memcpy(ss.hws.mapped, reinterpret<void? -const>(ws), pb.ws)
        }
    }
    if (length(g_gpu.stream_stacks) % 3 == 2) {   // walk order gate/up/down: this one is a down
        g_gpu.stream_max_dn_wq = max(g_gpu.stream_max_dn_wq, pb.wq)
        g_gpu.stream_max_dn_ws = max(g_gpu.stream_max_dn_ws, pb.ws)
    } else {
        g_gpu.stream_max_gu_wq = max(g_gpu.stream_max_gu_wq, pb.wq)
        g_gpu.stream_max_gu_ws = max(g_gpu.stream_max_gu_ws, pb.ws)
    }
    g_gpu.stream_stacks |> emplace(ss)
    return true
}

//! MoeGpuUploadFn: upload one weight stack ([rows x n] at element offset woff) from the loader's
//! gathered row-major planes. stream = pinned host mirror for per-prefill DMA instead of VRAM.
def vk_moe_upload_stack(wq : uint8 const?; ws : uint8 const?; woff : int64; n : int64; rows : int64; fmt : int; stream : bool) : bool {
    if (!vk_moe_init()) {
        return false
    }
    if (stream) {
        return vk_moe_stream_stack(wq, ws, woff, n, rows, fmt)
    }
    let pb = stack_plane_bytes(n, rows, fmt)
    let wqbytes = pb.wq
    let wsbytes = pb.ws
    if (g_gpu.resident_bytes + wqbytes + wsbytes > g_gpu.weight_budget) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: VRAM budget reached at {g_gpu.resident_bytes} bytes\n")
        return false
    }
    if (length(g_gpu.stacks) >= MAX_STACKS) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: descriptor capacity reached at {length(g_gpu.stacks)} stacks\n")
        return false
    }
    let si = make_stack_shell(woff, rows * n, fmt, wqbytes, wsbytes)
    upload_region(g_gpu.stacks[si].wqbuf, wq, wqbytes)
    upload_region(g_gpu.stacks[si].wsbuf, ws, wsbytes)
    g_gpu.resident_bytes += wqbytes + wsbytes
    return true
}

//! MoeGpuDropFn: roll back the last n uploads (a partial layer triple the budget cut mid-way).
//! Only freshly-uploaded stacks with no dispatch state yet may be dropped.
def vk_moe_drop_stacks(n : int; stream : bool) {
    if (stream) {
        for (_i in range(n)) {
            let k = length(g_gpu.stream_stacks) - 1
            destroy_host_buf(g_gpu.stream_stacks[k].hwq)
            destroy_host_buf(g_gpu.stream_stacks[k].hws)
            g_gpu.stream_stacks |> resize(k)
        }
        return
    }
    for (_i in range(n)) {
        let k = length(g_gpu.stacks) - 1
        assert(!g_gpu.stacks[k].gemv_made && !g_gpu.stacks[k].batch_made && !g_gpu.stacks[k].cls_made,
            "dasLLAMA vulkan tier: dropping a stack with live dispatch state")
        g_gpu.resident_bytes -= g_gpu.stacks[k].wqbytes + g_gpu.stacks[k].wsbytes
        destroy_device_buf(g_gpu.stacks[k].wqbuf)
        destroy_device_buf(g_gpu.stacks[k].wsbuf)
        g_gpu.stacks |> resize(k)
    }
}

//! MoeGpuDropModelFn: destroy every model-owned device object — stacks, arenas, the resident
//! driver, all lazy dispatch state, their descriptor sets and command buffers. Device-lifetime
//! state (instance/device/pipelines/staging/fence/profiler) survives and rebuilds lazily.
def vk_drop_model_state {
    if (g_gpu == null) {
        return
    }
    vk_check(vkQueueWaitIdle(g_gpu.queue), null)
    if (g_gpu.xfer_fam >= 0 && g_gpu.xfer_value > 0ul) {
        xfer_host_wait(g_gpu.xfer_value)   // in-flight transfer-queue copies retire before the sweeps
    }
    g_gpu.pend_wait = 0ul
    assert(g_gpu.ffn_pend_s2 == -1, "dasLLAMA vulkan tier: model drop with an async FFN in flight")
    // the resident driver's das-side shell first (its VK objects die in the sweeps below)
    if (g_rd != null) {
        unsafe {
            delete g_rd
        }
        g_rd = null
    }
    // every alloc_cmd result is model-owned — one bulk free
    if (!empty(g_gpu.model_cmds)) {
        unsafe {
            vkFreeCommandBuffers(dev_raw(), boost_value_to_vk(g_gpu.pool), uint(length(g_gpu.model_cmds)), addr(g_gpu.model_cmds[0]))
        }
        g_gpu.model_cmds |> clear()
    }
    // every descriptor set came from the one pool and every one is model-owned
    let rdpf : VkDescriptorPoolResetFlags
    vk_check(vkResetDescriptorPool(dev_raw(), boost_value_to_vk(g_gpu.desc_pool), rdpf), null)
    g_set_bits |> clear()   // pool reset recycles set handles — stale region bits must not survive
    // device buffers: dev_mem tracks every make_device_buf result — all model-owned
    for (buf, mem in keys(g_gpu.dev_mem), values(g_gpu.dev_mem)) {
        unsafe {
            vkDestroyBuffer(dev_raw(), reinterpret<VkBuffer>(buf), null)
            vkFreeMemory(dev_raw(), reinterpret<VkDeviceMemory>(mem), null)
        }
    }
    g_gpu.dev_mem |> clear()
    g_gpu.dev_mapped |> clear()
    g_vk_upload_us = 0l   // per-model upload diagnostics — a reloaded model reports its own numbers
    g_vk_upload_bytes = 0l
    g_rdq_n = 0u          // decode-role profiler likewise — averages must not span models
    g_rdq_count = 0l
    g_rdq_total = 0.0lf
    g_rdq_pro = 0.0lf
    g_rdq_tail = 0.0lf
    for (i in range(16)) {
        g_rdq_role[i] = 0.0lf
    }
    // host-visible buffers: host_mem tracks every make_host_buf result minus the exempt pair
    for (buf, mem in keys(g_gpu.host_mem), values(g_gpu.host_mem)) {
        unsafe {
            vkDestroyBuffer(dev_raw(), reinterpret<VkBuffer>(buf), null)
            vkFreeMemory(dev_raw(), reinterpret<VkDeviceMemory>(mem), null)
        }
    }
    g_gpu.host_mem |> clear()
    // das-side shells + the lazy-family latches (the ensure_* paths rebuild on next use).
    // clear(), not delete, where values carry VK handles — nothing das-heap-owned lives inside
    // those structs, and delete would try to finalize the handle fields
    g_gpu.stacks |> clear()
    delete g_gpu.stream_stacks
    delete g_gpu.stream_slots
    g_gpu.arenas |> clear()
    g_gpu.ffn_cmds |> clear()
    g_gpu.qkv_cmds |> clear()
    g_gpu.dnd_steps |> clear()
    delete g_gpu.heat_pools
    g_gpu.stream_max_gu_wq = 0l
    g_gpu.stream_max_gu_ws = 0l
    g_gpu.stream_max_dn_wq = 0l
    g_gpu.stream_max_dn_ws = 0l
    g_gpu.ar_ready = false
    g_gpu.af_ready = false
    g_gpu.rks_ready = false
    g_gpu.da_ready = false
    g_gpu.ffn_ready = false
    g_gpu.batch_ready = false
    g_gpu.comb_ready = false
    g_gpu.stream_ready = false
    g_gpu.dn_ready = false
    g_gpu.dnd_ready = false
    g_gpu.at_ready = false
    g_gpu.ffn_pend_s2 = -1
    g_gpu.ffn_pend_bytes = 0l
    g_gpu.resident_bytes = 0l
}

// ===== dispatch plumbing =====

// stack resolution is by (format, offset) — weight element offsets live in PER-FORMAT plane
// spaces (q8 qblob vs the k4/k5/k6 planes), so a bare offset is ambiguous across formats (a k4
// gate and a k5 down can share the numeric range); within one format's space ranges are disjoint
def private find_stack_idx(w0 : int64; fmt : int) : int {
    for (i in range(length(g_gpu.stacks))) {
        if (g_gpu.stacks[i].fmt == fmt && w0 >= g_gpu.stacks[i].base && w0 < g_gpu.stacks[i].base + g_gpu.stacks[i].elems) {
            return i
        }
    }
    return -1
}

def private find_stack(w0 : int64; fmt : int) : int {
    let i = find_stack_idx(w0, fmt)
    if (i < 0) {
        panic("dasLLAMA vulkan tier: dispatch against a non-resident weight region (fmt {fmt} element offset {w0})")
    }
    return i
}

// ===== the resident driver's weight arena =====
// One buffer pair per FORMAT (kernels index both planes off ONE base, so a shared cursor only stays
// aligned at a constant stride); tensors by block index. Sub-allocation, not a hardware-limit dodge.

struct private ArenaFmt {
    fmt : int                // int(KqFmt): 0 = q8, 1/2/3 = k4/k5/k6, 4 = q40
    wqbuf : uint64           // device-local quants for every tensor of this format
    wsbuf : uint64           // ... and their scale rows, at the SAME block index
    wqbytes : int64
    wsbytes : int64
    blocks : int64           // bump cursor: 32-weight blocks (q8) or 256-weight superblocks (kq)
    cap_blocks : int64
    // dispatch scratch — ONE set per format: bindings 0/1 stay put, only the meta changes per call
    raw_set : VkDescriptorSet
    meta : HostBuf
    xq : HostBuf
    xs : HostBuf
    y : HostBuf              // host-cached readback (the same-submit DMA lands here)
    y_dev : uint64
    ready : bool
    // batch-GEMM scratch (prefill): shares g_gpu.batch_xq_dev/xs_dev/y1_dev; one set + meta per fmt
    batch_set : VkDescriptorSet
    batch_meta : HostBuf
    batch_ready : bool
}

// per-block plane strides — the block is 32 weights for q8 and a 256-weight superblock for kq.
// Mirrors stack_plane_bytes: it computes the same totals as count * these strides.
// the tier's stack fmt space is int(KqFmt) — bridge to the gemm_schema kq id space (4/5/6/40)
def private vk_kq_schema_id(fmt : int) : int {
    if (fmt == 1) return 4
    if (fmt == 2) return 5
    if (fmt == 3) return 6
    if (fmt == 4) return 40
    panic("vk_kq_schema_id: not a kq stack fmt")
    return 0
}

// fmt space here = int(KqFmt): 0 q8, 6 q8n (both 32-elem blocks), 1-4 the kq lattice + q40
def private vk_fmt_b32(fmt : int) : bool => fmt == 0 || fmt == 6

// meta/activation block unit of a stack format — q8 AND q8n count 32-weight blocks, the kq
// lattice counts 256-weight superblocks
def private fmt_unit(fmt : int) : int64 => vk_fmt_b32(fmt) ? 32l : 256l

def private arena_block_bytes(fmt : int) : tuple<wq : int64; ws : int64> {
    if (fmt == 0) {
        return (wq = Q8_QPB, ws = Q8_SPB)     // 32 int8 quants + one f16 scale
    }
    if (fmt == 6) {
        return (wq = Q8N_BPB, ws = 0l)        // q8n: one interleaved 34B plane, scale inside
    }
    // decoded scale row: [f16 d][f16 dmin][8 sc][8 mn]
    return (wq = kq_qsb(vk_kq_schema_id(fmt)), ws = KQ_DEV_SSB)
}

def private arena_blocks_for(n, rows : int64; fmt : int) : int64 {
    assert(vk_fmt_b32(fmt) || n % KQ_SUPERBLOCK_ELEMS == 0l, "dasLLAMA vulkan arena: kq row length must be a superblock multiple")
    assert(!vk_fmt_b32(fmt) || n % Q8_BLOCK_ELEMS == 0l, "dasLLAMA vulkan arena: q8 row length must be a 32-block multiple")
    return rows * (n / (vk_fmt_b32(fmt) ? Q8_BLOCK_ELEMS : KQ_SUPERBLOCK_ELEMS))
}

// Reserve one format's arena up front. Sizing is a decision, not a walk: the resident driver knows
// every tensor's shape from Model metadata before a byte moves, so it either fits or declines.
def private arena_reserve(fmt : int; cap_blocks : int64) : bool {
    if (key_exists(g_gpu.arenas, fmt)) {
        // model reload: reset the bump cursor so it overwrites; an oversized reload then trips arena_place's -1 (decline to CPU) rather than overflowing
        unsafe {
            g_gpu.arenas[fmt].blocks = 0l
        }
        return true
    }
    let bb = arena_block_bytes(fmt)
    let qbytes = cap_blocks * bb.wq
    // q8n has no scale plane — floor the ws buffer so its descriptor slots stay valid (stack-shell rule)
    let sbytes = max(cap_blocks * bb.ws, 64l)
    // maxStorageBufferRange is the real ceiling (uint32 indexing reaches 16GiB); NV/AMD report 4GiB
    var lp : VkPhysicalDeviceProperties
    vkGetPhysicalDeviceProperties(g_gpu.phys, lp)
    let shard_cap = int64(lp.limits.maxStorageBufferRange)
    if (qbytes > shard_cap || sbytes > shard_cap) {
        to_log(LOG_WARNING, "dasLLAMA vulkan arena: fmt {fmt} needs {qbytes} quant + {sbytes} scale bytes — over the device's {shard_cap}B storage-range cap\n")
        return false
    }
    var a = ArenaFmt(fmt = fmt, cap_blocks = cap_blocks, wqbytes = qbytes, wsbytes = sbytes,
                     wqbuf = make_device_buf(qbytes), wsbuf = make_device_buf(sbytes))
    g_gpu.arenas |> insert(fmt, a)
    return true
}

//! Place one [rows x n] tensor in its format's arena and upload it. Returns the BLOCK INDEX the
//! meta needs (mp[4 + row*2] = block + row * blocks_per_row), or -1 if the arena is exhausted.
def private arena_place(wq : uint8 const?; ws : uint8 const?; n, rows : int64; fmt : int) : int64 {
    if (!key_exists(g_gpu.arenas, fmt)) {
        return -1l
    }
    let need = arena_blocks_for(n, rows, fmt)
    unsafe {
        var a & = g_gpu.arenas[fmt]
        if (a.blocks + need > a.cap_blocks) {
            return -1l
        }
        let blk = a.blocks
        a.blocks += need
        let bb = arena_block_bytes(fmt)
        upload_region_at(a.wqbuf, blk * bb.wq, wq, need * bb.wq)
        if (bb.ws > 0l) {
            upload_region_at(a.wsbuf, blk * bb.ws, ws, need * bb.ws)
        }
        return blk
    }
}

//! Arena seam: reserve one format's arena for `cap_blocks` 32-weight blocks (q8) or 256-weight
//! superblocks (kq). false = over the shard cap or no device — the caller turns that into a decline.
def vk_arena_reserve(fmt : int; cap_blocks : int64) : bool {
    if (!vk_moe_init()) {
        return false
    }
    return arena_reserve(fmt, cap_blocks)
}

//! Arena seam: place + upload one [rows x n] tensor from the loader's gathered device-layout
//! planes. Returns its block index (what the meta's weight base wants), or -1 if exhausted.
def vk_arena_place(wq : array<uint8>; ws : array<uint8>; n : int64; rows : int64; fmt : int) : int64 {
    // q8n (fmt 6) carries its scale inside the interleaved plane — an EMPTY ws is its contract
    if (!vk_moe_init() || empty(wq) || (empty(ws) && fmt != 6)) {
        return -1l
    }
    return arena_place(unsafe(addr(wq[0])), empty(ws) ? null : unsafe(addr(ws[0])), n, rows, fmt)
}

// Bindings 0/1 are the arena planes; 2-5 are per-dispatch scratch, written exactly as
// ensure_gemv_scratch writes them for a per-op stack — so an arena-resident tensor runs the SAME
// kernels with the same meta convention. Lazy: a format only pays when it actually dispatches.
def private arena_ensure_scratch(fmt : int) {
    unsafe {
        var a & = g_gpu.arenas[fmt]
        if (a.ready) {
            return
        }
        a.meta = make_host_buf(META_BYTES, true)
        a.xq = make_host_buf(XQ_BYTES, true)
        a.xs = make_host_buf(XS_BYTES, true)
        a.y = make_host_buf(Y_BYTES, false, [cached = true])
        a.y_dev = make_device_buf(Y_BYTES)
        a.raw_set = alloc_desc_set()
        var writes : array<WriteDescriptorSet>
        write_buf_desc(writes, a.raw_set, 0u, a.wqbuf, a.wqbytes)
        write_buf_desc(writes, a.raw_set, 1u, a.wsbuf, a.wsbytes)
        write_buf_desc(writes, a.raw_set, 2u, a.meta.buf, META_BYTES)
        write_buf_desc(writes, a.raw_set, 3u, a.xq.buf, XQ_BYTES)
        write_buf_desc(writes, a.raw_set, 4u, a.xs.buf, XS_BYTES)
        write_buf_desc(writes, a.raw_set, 5u, a.y_dev, Y_BYTES)
        let no_copies : array<CopyDescriptorSet>
        update_descriptor_sets(g_gpu.device, writes, no_copies)
        hz_set_bits(a.raw_set, 0u, 0u, 0u, 0u, 0u, VHG_YO)
        a.ready = true
    }
}

// residual-stream scratch for the add+rms kernel (lazy — only a resident-driver run creates it)
def private ensure_ar_state {
    if (g_gpu.ar_ready) {
        return
    }
    let bytes = AR_MAX_DIM * 4l
    g_gpu.ar_x_dev = make_device_buf(bytes)
    g_gpu.ar_a_dev = make_device_buf(bytes)
    g_gpu.ar_w_dev = make_device_buf(bytes)
    g_gpu.ar_y_dev = make_device_buf(bytes)
    g_gpu.ar_meta = make_host_buf(META_BYTES, true)
    g_gpu.ar_scal = make_host_buf(XS_BYTES, true)
    g_gpu.ar_host = make_host_buf(bytes, false, [cached = true])   // x and y readback
    g_gpu.ar_set = alloc_desc_set()
    var writes : array<WriteDescriptorSet>
    write_buf_desc(writes, g_gpu.ar_set, 0u, g_gpu.ar_x_dev, bytes)
    write_buf_desc(writes, g_gpu.ar_set, 1u, g_gpu.ar_a_dev, bytes)
    write_buf_desc(writes, g_gpu.ar_set, 2u, g_gpu.ar_meta.buf, META_BYTES)
    write_buf_desc(writes, g_gpu.ar_set, 3u, g_gpu.ar_w_dev, bytes)
    write_buf_desc(writes, g_gpu.ar_set, 4u, g_gpu.ar_scal.buf, XS_BYTES)
    write_buf_desc(writes, g_gpu.ar_set, 5u, g_gpu.ar_y_dev, bytes)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    hz_set_bits(g_gpu.ar_set, VHG_X, 0u, 0u, 0u, 0u, VHG_YO)   // a/w/meta/scal fill pre-cmd
    g_gpu.ar_ready = true
}

//! Resident-driver seam: x = (x + a) * ascale then y = rms(x) * w, in ONE dispatch. `add` false
//! runs the plain norm (the stack prologue). x is updated in place — it IS the residual stream.
def vk_add_rms(var x : array<float>; a : array<float>; w : array<float>; var y : array<float>;
               dim : int64; eps, ascale : float; add : bool) {
    verify(vk_moe_init())   // assert() refuses side-effecting expressions; the probe must run
    assert(dim <= AR_MAX_DIM, "dasLLAMA vulkan: add_rms dim exceeds AR_MAX_DIM")
    ensure_ar_state()
    let bytes = dim * 4l
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.ar_meta.mapped)
        mp[0] = uint(dim)
        mp[1] = add ? 1u : 0u
        mp[2] = 0u   // standalone seam: norm weight starts at 0
        var sp = reinterpret<float?>(g_gpu.ar_scal.mapped)
        sp[0] = eps
        sp[1] = ascale
        upload_region_at(g_gpu.ar_x_dev, 0l, addr<uint8 const?>(x[0]), bytes)
        upload_region_at(g_gpu.ar_w_dev, 0l, addr<uint8 const?>(w[0]), bytes)
        if (add) {
            upload_region_at(g_gpu.ar_a_dev, 0l, addr<uint8 const?>(a[0]), bytes)
        }
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        hz_dispatch(raw, h, weak_copy(g_gpu.ar_pipeline), g_gpu.ar_set, 1u)   // one workgroup owns the whole row (it reduces)
        vhz_dep(raw, h, VHG_YO, 0u, true)   // x reads back post-fence in the second cmd
        cmd_copy_whole(raw, g_gpu.ar_y_dev, g_gpu.ar_host.buf, bytes)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(y[0]), g_gpu.ar_host.mapped, int(bytes))
        // x is the residual stream: read the updated row back too
        var raw2 = alloc_cmd()
        vk_check(vkBeginCommandBuffer(raw2, begin), null)
        cmd_copy_whole(raw2, g_gpu.ar_x_dev, g_gpu.ar_host.buf, bytes)
        vk_check(vkEndCommandBuffer(raw2), null)
        submit_wait(raw2)
        memcpy(addr<void?>(x[0]), g_gpu.ar_host.mapped, int(bytes))
    }
}

// The arena FFN chain's four sets. Bindings mirror ffn_cmd_for exactly — only bindings 0/1 change,
// from a stack's own plane pair to its format's arena. Each role needs its OWN set because the
// weight base lives at a fixed meta offset, so it cannot vary within one set.
def private ensure_af_state(f1, f3, f2 : int) {
    if (g_gpu.af_ready) {
        return
    }
    ensure_ffn_state()
    g_gpu.af_meta_g = make_host_buf(META_BYTES, true)
    g_gpu.af_meta_u = make_host_buf(META_BYTES, true)
    g_gpu.af_meta_d = make_host_buf(META_BYTES, true)
    g_gpu.af_xq = make_host_buf(XQ_BYTES, true)
    g_gpu.af_xs = make_host_buf(XS_BYTES, true)
    g_gpu.af_ya_dev = make_device_buf(Y_BYTES)
    g_gpu.af_yb_dev = make_device_buf(Y_BYTES)
    g_gpu.af_yo_dev = make_device_buf(Y_BYTES)
    g_gpu.af_yo_host = make_host_buf(Y_BYTES, false, [cached = true])
    g_gpu.af_gate_set = alloc_desc_set()
    g_gpu.af_up_set = alloc_desc_set()
    g_gpu.af_actrq_set = alloc_desc_set()
    g_gpu.af_down_set = alloc_desc_set()
    var writes : array<WriteDescriptorSet>
    unsafe {
        var a1 & = g_gpu.arenas[f1]
        var a3 & = g_gpu.arenas[f3]
        var a2 & = g_gpu.arenas[f2]
        write_buf_desc(writes, g_gpu.af_gate_set, 0u, a1.wqbuf, a1.wqbytes)
        write_buf_desc(writes, g_gpu.af_gate_set, 1u, a1.wsbuf, a1.wsbytes)
        write_buf_desc(writes, g_gpu.af_up_set, 0u, a3.wqbuf, a3.wqbytes)
        write_buf_desc(writes, g_gpu.af_up_set, 1u, a3.wsbuf, a3.wsbytes)
        write_buf_desc(writes, g_gpu.af_down_set, 0u, a2.wqbuf, a2.wqbytes)
        write_buf_desc(writes, g_gpu.af_down_set, 1u, a2.wsbuf, a2.wsbytes)
        // the act+requant set has no weights: up@0, gate@5, hq/hs out (binding 1 is filler)
        write_buf_desc(writes, g_gpu.af_actrq_set, 0u, g_gpu.af_yb_dev, Y_BYTES)
        write_buf_desc(writes, g_gpu.af_actrq_set, 1u, a1.wsbuf, a1.wsbytes)
        write_buf_desc(writes, g_gpu.af_actrq_set, 5u, g_gpu.af_ya_dev, Y_BYTES)
    }
    write_buf_desc(writes, g_gpu.af_gate_set, 2u, g_gpu.af_meta_g.buf, META_BYTES)
    write_buf_desc(writes, g_gpu.af_gate_set, 3u, g_gpu.af_xq.buf, XQ_BYTES)
    write_buf_desc(writes, g_gpu.af_gate_set, 4u, g_gpu.af_xs.buf, XS_BYTES)
    write_buf_desc(writes, g_gpu.af_gate_set, 5u, g_gpu.af_ya_dev, Y_BYTES)
    write_buf_desc(writes, g_gpu.af_up_set, 2u, g_gpu.af_meta_u.buf, META_BYTES)
    write_buf_desc(writes, g_gpu.af_up_set, 3u, g_gpu.af_xq.buf, XQ_BYTES)
    write_buf_desc(writes, g_gpu.af_up_set, 4u, g_gpu.af_xs.buf, XS_BYTES)
    write_buf_desc(writes, g_gpu.af_up_set, 5u, g_gpu.af_yb_dev, Y_BYTES)
    write_buf_desc(writes, g_gpu.af_actrq_set, 2u, g_gpu.ffn_meta_dev, FFN_META_BYTES)
    write_buf_desc(writes, g_gpu.af_actrq_set, 3u, g_gpu.hq_dev, BATCH_HQ_BYTES)
    write_buf_desc(writes, g_gpu.af_actrq_set, 4u, g_gpu.hs_dev, BATCH_HS_BYTES)
    write_buf_desc(writes, g_gpu.af_down_set, 2u, g_gpu.af_meta_d.buf, META_BYTES)
    write_buf_desc(writes, g_gpu.af_down_set, 3u, g_gpu.hq_dev, BATCH_HQ_BYTES)
    write_buf_desc(writes, g_gpu.af_down_set, 4u, g_gpu.hs_dev, BATCH_HS_BYTES)
    write_buf_desc(writes, g_gpu.af_down_set, 5u, g_gpu.af_yo_dev, Y_BYTES)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    hz_set_bits(g_gpu.af_gate_set, 0u, 0u, 0u, 0u, 0u, VHG_Y1)
    hz_set_bits(g_gpu.af_up_set, 0u, 0u, 0u, 0u, 0u, VHG_Y2)
    hz_set_bits(g_gpu.af_actrq_set, VHG_Y2, 0u, VHG_META, VHG_HQ, VHG_HQ, VHG_Y1)
    hz_set_bits(g_gpu.af_down_set, 0u, 0u, 0u, VHG_HQ, VHG_HQ, VHG_YO)
    g_gpu.af_ready = true
}

def private fill_af_meta(hb : HostBuf; n, d, blk : int64) {
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = 1u          // one region — a dense plane, not a routed expert set
        mp[3] = 0u
        mp[4] = uint(blk)   // the arena block index
        mp[5] = 0u
    }
}

//! True when a Vulkan device is up (probes lazily). Non-panicking — the resident-driver seams that
//! `verify(vk_moe_init())` are fine on this hardware, but a test wants to feint on a headless box.
def vk_device_ready() : bool => vk_moe_init()

let private RKS_MAX_QD = 8_192l
let private RKS_MAX_KVD = 1_024l
let private RKS_MAX_CTX = 4_096l   // K/V mirror rows in the standalone-kernel scratch
let private KV_READBACK_ROWS = 256l   // bulk mirror->host hydration chunk (rows per submit)

def private ensure_rks_state {
    if (g_gpu.rks_ready) {
        return
    }
    g_gpu.rks_qkv_dev = make_device_buf((RKS_MAX_QD + 2l * RKS_MAX_KVD) * 4l)
    g_gpu.rks_q_dev = make_device_buf(RKS_MAX_QD * 4l)
    g_gpu.rks_k_dev = make_device_buf(RKS_MAX_CTX * RKS_MAX_KVD * 4l)
    g_gpu.rks_v_dev = make_device_buf(RKS_MAX_CTX * RKS_MAX_KVD * 4l)
    g_gpu.rks_scal = make_host_buf(XS_BYTES, true)
    g_gpu.rks_meta = make_host_buf(META_BYTES, true)
    g_gpu.rks_q_host = make_host_buf(RKS_MAX_QD * 4l, false, [cached = true])
    g_gpu.rks_kv_host = make_host_buf(2l * RKS_MAX_KVD * 4l, false, [cached = true])
    g_gpu.rks_set = alloc_desc_set()
    var writes : array<WriteDescriptorSet>
    write_buf_desc(writes, g_gpu.rks_set, 0u, g_gpu.rks_qkv_dev, (RKS_MAX_QD + 2l * RKS_MAX_KVD) * 4l)
    write_buf_desc(writes, g_gpu.rks_set, 1u, g_gpu.rks_k_dev, RKS_MAX_CTX * RKS_MAX_KVD * 4l)
    write_buf_desc(writes, g_gpu.rks_set, 2u, g_gpu.rks_meta.buf, META_BYTES)
    write_buf_desc(writes, g_gpu.rks_set, 3u, g_gpu.rks_v_dev, RKS_MAX_CTX * RKS_MAX_KVD * 4l)
    write_buf_desc(writes, g_gpu.rks_set, 4u, g_gpu.rks_scal.buf, XS_BYTES)
    write_buf_desc(writes, g_gpu.rks_set, 5u, g_gpu.rks_q_dev, RKS_MAX_QD * 4l)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    hz_set_bits(g_gpu.rks_set, 0u, VHG_Y2, 0u, VHG_Y3, 0u, VHG_Y1)   // K mirror / V mirror / roped q
    g_gpu.rks_ready = true
}


def private ensure_da_state {
    if (g_gpu.da_ready) {
        return
    }
    ensure_rks_state()   // shares the K/V mirror planes
    g_gpu.da_q_dev = make_device_buf(RKS_MAX_QD * 4l)
    g_gpu.da_out_dev = make_device_buf(RKS_MAX_QD * 4l)
    g_gpu.da_scal = make_host_buf(XS_BYTES, true)
    g_gpu.da_meta = make_host_buf(META_BYTES, true)
    g_gpu.da_out_host = make_host_buf(RKS_MAX_QD * 4l, false, [cached = true])
    g_gpu.da_set = alloc_desc_set()
    var writes : array<WriteDescriptorSet>
    write_buf_desc(writes, g_gpu.da_set, 0u, g_gpu.da_q_dev, RKS_MAX_QD * 4l)
    write_buf_desc(writes, g_gpu.da_set, 1u, g_gpu.rks_k_dev, RKS_MAX_CTX * RKS_MAX_KVD * 4l)
    write_buf_desc(writes, g_gpu.da_set, 2u, g_gpu.da_meta.buf, META_BYTES)
    write_buf_desc(writes, g_gpu.da_set, 3u, g_gpu.rks_v_dev, RKS_MAX_CTX * RKS_MAX_KVD * 4l)
    write_buf_desc(writes, g_gpu.da_set, 4u, g_gpu.da_scal.buf, XS_BYTES)
    write_buf_desc(writes, g_gpu.da_set, 5u, g_gpu.da_out_dev, RKS_MAX_QD * 4l)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    hz_set_bits(g_gpu.da_set, 0u, 0u, 0u, 0u, 0u, VHG_YO)   // q/mirrors fill pre-cmd (fence-ordered)
    g_gpu.da_ready = true
}

//! Resident-driver seam: decode attention of the roped q row against the `cnt` cached positions in
//! the device K/V mirror (the same planes vk_rope_kv_store wrote), GQA per kv_mul, into out[0..qd).
def vk_decode_attn(var q : array<float>; var out : array<float>; cnt, qd, kv_dim, head_size, kv_mul : int64; scale : float) {
    verify(vk_moe_init())
    assert(head_size <= 256l && qd <= RKS_MAX_QD, "dasLLAMA vulkan: decode-attn geometry over scratch")
    ensure_da_state()
    let n_heads = qd / head_size
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.da_meta.mapped)
        mp[0] = uint(cnt)
        mp[1] = uint(qd)
        mp[2] = uint(kv_dim)
        mp[3] = uint(head_size)
        mp[4] = uint(kv_mul)
        mp[5] = 0u   // standalone seam: mirror base 0
        mp[6] = 0u
        var sp = reinterpret<float?>(g_gpu.da_scal.mapped)
        sp[0] = scale
        upload_region_at(g_gpu.da_q_dev, 0l, addr<uint8 const?>(q[0]), qd * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        hz_dispatch(raw, h, weak_copy(g_gpu.da_pipeline), g_gpu.da_set, uint(n_heads))
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.da_out_dev, g_gpu.da_out_host.buf, qd * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(out[0]), g_gpu.da_out_host.mapped, int(qd * 4l))
    }
}

//! Resident-driver seam (standalone-kernel form): rope the q row in place, rope the k row into the
//! device K mirror at row `pos`, copy v raw into the V mirror at `pos`; then read q + that row's
//! K/V back for parity. `neox` picks llama's adjacent-pair vs qwen/gemma's half-offset rope.
def vk_rope_kv_store(var qkv : array<float>; var qout : array<float>; var krow : array<float>; var vrow : array<float>;
                     cossin : array<float>; qd, kv_dim, head_size, pos : int64; neox : bool) {
    verify(vk_moe_init())
    let half = head_size / 2l
    assert(qd <= RKS_MAX_QD && kv_dim <= RKS_MAX_KVD && pos < RKS_MAX_CTX, "dasLLAMA vulkan: rope_kv geometry over scratch")
    ensure_rks_state()
    let kpair0 = qd / 2l
    let npairs = kpair0 + kv_dim / 2l
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.rks_meta.mapped)
        mp[0] = uint(qd)
        mp[1] = uint(kv_dim)
        mp[2] = uint(head_size)
        mp[3] = uint(half)
        mp[4] = neox ? 1u : 0u
        mp[5] = uint(kpair0)
        mp[6] = uint(npairs)
        mp[7] = uint(pos * kv_dim)   // K mirror row offset (f32 elements)
        mp[8] = uint(pos * kv_dim)   // V mirror row offset
        memcpy(g_gpu.rks_scal.mapped, addr<void? -const>(cossin[0]), int(head_size * 4l))   // cos[half] + sin[half]
        upload_region_at(g_gpu.rks_qkv_dev, 0l, addr<uint8 const?>(qkv[0]), (qd + 2l * kv_dim) * 4l)
        // q enters the kernel via vk_y (binding 5) = rks_q_dev — seed it with the raw q rows
        upload_region_at(g_gpu.rks_q_dev, 0l, addr<uint8 const?>(qkv[0]), qd * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        hz_dispatch(raw, h, weak_copy(g_gpu.rks_pipeline), g_gpu.rks_set, uint((npairs + int64(WG_X) - 1l) / int64(WG_X)))
        vhz_dep(raw, h, VHG_Y1 | VHG_Y2 | VHG_Y3, 0u, true)   // one declaration for the q/k/v readback group
        cmd_copy_whole(raw, g_gpu.rks_q_dev, g_gpu.rks_q_host.buf, qd * 4l)
        cmd_copy_range(raw, g_gpu.rks_k_dev, pos * kv_dim * 4l, g_gpu.rks_kv_host.buf, 0l, kv_dim * 4l)
        cmd_copy_range(raw, g_gpu.rks_v_dev, pos * kv_dim * 4l, g_gpu.rks_kv_host.buf, kv_dim * 4l, kv_dim * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(qout[0]), g_gpu.rks_q_host.mapped, int(qd * 4l))
        memcpy(addr<void?>(krow[0]), g_gpu.rks_kv_host.mapped, int(kv_dim * 4l))
        memcpy(addr<void?>(vrow[0]), reinterpret<float?>(g_gpu.rks_kv_host.mapped) + kv_dim, int(kv_dim * 4l))
    }
}

// ===== the resident whole-stack decode driver =====
// One cb/token: residual + KV mirror stay device-side, host only uploads x + rope row, reads logits.
// v1 re-records per token (still 1 submit). Activations quantized on-device before each GEMV.

struct private RLayer {
    // arena block indices + formats for this layer's 7 planes
    bq, bk, bv, bo, b1, b3, b2 : int64
    fq, fk, fv, fo, f1, f3, f2 : int
    // the layer's descriptor sets (built once, referenced every token)
    s_quant_xb : VkDescriptorSet    // dn_requant of the attn-norm activation
    s_q, s_k, s_v : VkDescriptorSet  // q/k/v GEMV
    s_kv : VkDescriptorSet          // merged k+v two-region GEMV (fk == fv: same arena planes)
    kv_merged : bool
    s_qkn : VkDescriptorSet         // per-head q/k rmsnorm (qk_norm models only)
    s_rope : VkDescriptorSet
    s_attn : VkDescriptorSet
    s_quant_at : VkDescriptorSet
    s_wo : VkDescriptorSet
    s_addr_ffn : VkDescriptorSet
    s_quant_xb2 : VkDescriptorSet
    s_gate, s_up : VkDescriptorSet
    s_actrq : VkDescriptorSet
    s_down : VkDescriptorSet
    s_addr_next : VkDescriptorSet
    // per-dispatch metas (host-visible; contents rewritten per token)
    m_quant_xb, m_q, m_k, m_v, m_qkn, m_rope, m_attn, m_quant_at, m_wo, m_addr_ffn : HostBuf
    m_quant_xb2, m_gate, m_up, m_actrq, m_down, m_addr_next : HostBuf
    next_norm_off : int64   // rms_ffn is w_off for addr_ffn; addr_next uses rms_att[l+1] or final
    made : bool
}

struct private RDec {
    ready : bool
    n_layers, dim, qd, kv_dim, head_size, n_heads, kv_mul, hidden, vocab, seq_cap : int64
    neox : bool
    qk_norm : bool                // per-head q/k rmsnorm role armed (norms buffer carries the rows)
    norms_bytes : int64           // full norms buffer size (grows past the fixed layout under qk_norm)
    eps, scale : float
    // per-token device buffers
    x_dev, xb_dev : uint64        // residual, normed activation
    xq_dev, xs_dev : uint64       // quantized activation (shared across the layer's GEMVs)
    q_dev, kv_dev : uint64        // roped q; the k/v projection buffer rope reads (k@qd, v@qd+kvd)
    attn_dev : uint64             // attention output
    aq_dev, as_dev : uint64       // quantized attn (wo input)
    gate_dev, up_dev : uint64
    ffnout_dev : uint64
    xb2_dev : uint64              // wo output
    k_mirror, v_mirror : uint64   // [n_layers x seq_cap x kv_dim] f32
    norms_dev : uint64            // rms_att[l]/rms_ffn[l] interleaved + final, uploaded once
    cos_dev : uint64              // this token's rope row (cos[half] + sin[half])
    scal_dev : uint64             // da_attn scalars: [0] = attn scale
    ar_scal_dev : uint64          // add_rms scalars: [0] = eps, [1] = ascale (1.0 for the residual add)
    logits_dev : uint64
    x_host, logits_host : HostBuf
    kv_readback : HostBuf         // per-layer K/V row -> CPU cache writeback
    cls_meta_dev : uint64
    cls_set : VkDescriptorSet
    s_prologue : VkDescriptorSet  // xb = rms_att[0](x)
    m_prologue : HostBuf
    s_quant_final : VkDescriptorSet
    m_quant_final : HostBuf
    dummy : uint64                // filler for unused bindings
    cls_fmt_marker : int
    layers : array<RLayer>
    cmd : VkCommandBuffer
    tok_recorded : bool           // cmd holds the whole-token chain, re-submittable as-is
    cos_host : HostBuf            // this token's rope row, copied to cos_dev in-cmd
    ffn_meta_h : HostBuf          // decode-owned actrq meta source (g_gpu.ffn_meta is shared with the batch rails)
    // prefill batch buffers (lazy — sized MAX_NPOS x their width) + per-layer prefill sets
    pf_ready : bool
    pf_np : int64                 // MAX_NPOS the buffers were sized for
    pf_x, pf_xb, pf_xq, pf_xs : uint64
    pf_q, pf_kv, pf_v, pf_attn, pf_aq, pf_as : uint64
    pf_gate, pf_up, pf_ffnout, pf_xb2 : uint64
    pf_cos : uint64               // [MAX_NPOS x hs] rope rows
    pf_xf, pf_af, pf_hf : uint64  // cm2 (fmt-6) f16 twin feeds: xb rows, attn rows, hidden rows
    pf_logits_host : HostBuf
    pf_meta : array<HostBuf>      // one host meta per (layer, role) + prologue/final/cls
    pf_sets : array<VkDescriptorSet>
    pf_cmd : VkCommandBuffer
}

let private PF_WINDOW = 512l   // prefill activation-buffer rows; longer prompts run as sequential windows

var private g_rd : RDec?

def private rd_set6(b0, b1, b2, b3, b4, b5 : uint64; s0, s1, s2, s3, s4, s5 : int64; gbits : uint[6]) : VkDescriptorSet {
    var set = alloc_desc_set()
    var writes : array<WriteDescriptorSet>
    write_buf_desc(writes, set, 0u, b0, s0)
    write_buf_desc(writes, set, 1u, b1, s1)
    write_buf_desc(writes, set, 2u, b2, s2)
    write_buf_desc(writes, set, 3u, b3, s3)
    write_buf_desc(writes, set, 4u, b4, s4)
    write_buf_desc(writes, set, 5u, b5, s5)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    hz_set_bits(set, gbits)
    return set
}

// a small host-visible meta buffer (uint words), contents filled per token during recording
def private rd_meta() : HostBuf => make_host_buf(META_BYTES, true)

// one arena's plane handles + sizes in ONE table lookup (das rejects two lookups in one expr)
def private arena_planes(fmt : int) : tuple<wq : uint64; ws : uint64; wqb : int64; wsb : int64> {
    unsafe {
        var a & = g_gpu.arenas[fmt]
        return (wq = a.wqbuf, ws = a.wsbuf, wqb = a.wqbytes, wsb = a.wsbytes)
    }
}

//! Resident-driver prepare: allocate all per-token + session buffers for a model's geometry. One
//! resident model at a time (the arbiter guarantees it). Call before set_layer / token.
def vk_rdec_prepare(n_layers, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64;
                    neox : bool; eps, scale : float; qk_norm : bool) : bool {
    if (!vk_moe_init()) {
        return false
    }
    // the shaders index the KV mirror with 32-bit uint meta offsets and the buffer must fit a 2GiB
    // shard — a huge seq_cap would overflow both. The budget gate should decline first; guard anyway.
    if (n_layers * seq_cap * kv_dim > 2_000_000_000l) {
        to_log(LOG_WARNING, "dasLLAMA vulkan resident: KV mirror ({n_layers} x {seq_cap} x {kv_dim}) exceeds 32-bit index / 2GiB shard\n")
        return false
    }
    g_rd = new RDec(ready = false, n_layers = n_layers, dim = dim, qd = qd, kv_dim = kv_dim,
                 head_size = head_size, n_heads = n_heads, kv_mul = n_heads / (kv_dim / head_size),
                 hidden = hidden, vocab = vocab, seq_cap = seq_cap, neox = neox, eps = eps, scale = scale,
                 qk_norm = qk_norm)
    var r & = unsafe(*g_rd)
    let wide = max(max(qd, hidden), dim)
    r.x_dev = make_device_buf(dim * 4l)
    r.xb_dev = make_device_buf(dim * 4l)
    r.xq_dev = make_device_buf(wide)              // int8 quants, 1 byte/elem
    r.xs_dev = make_device_buf((wide / 32l) * 4l)
    r.q_dev = make_device_buf(qd * 4l)
    r.kv_dev = make_device_buf((qd + 2l * kv_dim) * 4l)
    r.attn_dev = make_device_buf(qd * 4l)
    r.aq_dev = make_device_buf(qd)
    r.as_dev = make_device_buf((qd / 32l) * 4l)
    r.gate_dev = make_device_buf(hidden * 4l)
    r.up_dev = make_device_buf(hidden * 4l)
    r.ffnout_dev = make_device_buf(dim * 4l)
    r.xb2_dev = make_device_buf(dim * 4l)
    r.k_mirror = make_device_buf(n_layers * seq_cap * kv_dim * 4l)
    r.v_mirror = make_device_buf(n_layers * seq_cap * kv_dim * 4l)
    // norms layout: [rms_att[l], rms_ffn[l]] x L + rms_final (dim each), then under qk_norm the
    // per-head rows [rms_q[l], rms_k[l]] x L (head_size each) appended past the fixed layout
    r.norms_bytes = (n_layers * 2l * dim + dim + (qk_norm ? n_layers * 2l * head_size : 0l)) * 4l
    r.norms_dev = make_device_buf(r.norms_bytes)
    r.cos_dev = make_device_buf(head_size * 4l)
    r.scal_dev = make_device_buf(64l)
    r.ar_scal_dev = make_device_buf(64l)
    r.logits_dev = make_device_buf(vocab * 4l)
    r.dummy = make_device_buf(256l)
    r.x_host = make_host_buf(dim * 4l, false, [cached = true])
    r.cos_host = make_host_buf(head_size * 4l, false, [cached = true])
    r.ffn_meta_h = make_host_buf(FFN_META_BYTES, false)
    r.logits_host = make_host_buf(vocab * 4l, false, [cached = true])
    r.kv_readback = make_host_buf(2l * kv_dim * 4l * KV_READBACK_ROWS, false, [cached = true])
    ensure_ffn_state()   // hq_dev/hs_dev for the FFN act+requant
    r.cmd = alloc_cmd()
    return true
}

//! Upload the norm-weight rows once: [rms_att[0], rms_ffn[0], ..., rms_att[L-1], rms_ffn[L-1],
//! rms_final], dim floats each, exactly the layout the layer w_offs index. Under qk_norm the
//! per-head [rms_q[l], rms_k[l]] x L rows (head_size floats each) follow.
def vk_rdec_upload_norms(norms : array<float>) {
    assert(g_rd != null, "vk_rdec_upload_norms before prepare")
    upload_region_at(g_rd.norms_dev, 0l, unsafe(addr<uint8 const?>(norms[0])), int64(length(norms)) * 4l)
    // norms are the last upload of the resident arm sequence — report the accumulated cost
    if (g_vk_upload_us > 0l) {
        let gbps = double(g_vk_upload_bytes) / (double(g_vk_upload_us) * 1000.0lf)
        to_log(LOG_INFO, "dasLLAMA vulkan tier: uploads {g_vk_upload_bytes / 1000000l} MB in {g_vk_upload_us / 1000l} ms ({gbps} GB/s, {key_exists(g_gpu.dev_mapped, g_rd.norms_dev) ? "direct" : "staged"})\n")
    }
}

//! Register a dense layer's arena block indices + formats and build its descriptor sets. Called
//! once per layer after prepare. `w_ffn_off`/`w_next_off` are the rms_ffn[l] and rms_att[l+1]
//! (or rms_final for the last layer) offsets in the norms buffer (dim-element units).
def vk_rdec_set_layer(l : int64; bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int) {
    assert(g_rd != null, "vk_rdec_set_layer before prepare")
    while (int64(length(g_rd.layers)) <= l) {
        g_rd.layers |> emplace(RLayer())
    }
    unsafe {
        var L & = g_rd.layers[l]
        L.bq = bq; L.bk = bk; L.bv = bv; L.bo = bo; L.b1 = b1; L.b3 = b3; L.b2 = b2
        L.fq = fq; L.fk = fk; L.fv = fv; L.fo = fo; L.f1 = f1; L.f3 = f3; L.f2 = f2
        let dim = g_rd.dim
        let qd = g_rd.qd
        let kvd = g_rd.kv_dim
        let hid = g_rd.hidden
        let pq = arena_planes(fq)
        let pk = arena_planes(fk)
        let pv = arena_planes(fv)
        let po = arena_planes(fo)
        let p1 = arena_planes(f1)
        let p3 = arena_planes(f3)
        let p2 = arena_planes(f2)
        // metas
        L.m_quant_xb = rd_meta(); L.m_q = rd_meta(); L.m_k = rd_meta(); L.m_v = rd_meta()
        L.m_rope = rd_meta(); L.m_attn = rd_meta(); L.m_quant_at = rd_meta(); L.m_wo = rd_meta()
        L.m_addr_ffn = rd_meta(); L.m_quant_xb2 = rd_meta(); L.m_gate = rd_meta(); L.m_up = rd_meta()
        L.m_actrq = rd_meta(); L.m_down = rd_meta(); L.m_addr_next = rd_meta()
        // quantize xb -> xq/xs (dn_requant: in vk_upf@0, meta@2, xq@3, xs@4)
        L.s_quant_xb = rd_set6(g_rd.xb_dev, g_rd.dummy, L.m_quant_xb.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.dummy,
            dim * 4l, 256l, META_BYTES, dim, (dim / 32l) * 4l, 256l,
            fixed_array(VHZ_XB, 0u, 0u, VHZ_XQ, VHZ_XQ, 0u))
        // q/k/v GEMV: arena[f] planes @0/1, meta@2, xq@3, xs@4, out@5
        L.s_q = rd_set6(pq.wq, pq.ws, L.m_q.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.q_dev,
            pq.wqb, pq.wsb, META_BYTES, dim, (dim / 32l) * 4l, qd * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_Q))
        L.s_k = rd_set6(pk.wq, pk.ws, L.m_k.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev,
            pk.wqb, pk.wsb, META_BYTES, dim, (dim / 32l) * 4l, (qd + 2l * kvd) * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVK))
        L.s_v = rd_set6(pv.wq, pv.ws, L.m_v.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev,
            pv.wqb, pv.wsb, META_BYTES, dim, (dim / 32l) * 4l, (qd + 2l * kvd) * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVV))
        // same-format k/v share arena planes — one two-region dispatch fills both kv halves
        L.kv_merged = fk == fv
        if (L.kv_merged) {
            L.s_kv = rd_set6(pk.wq, pk.ws, L.m_k.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.kv_dev,
                pk.wqb, pk.wsb, META_BYTES, dim, (dim / 32l) * 4l, (qd + 2l * kvd) * 4l,
                fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVK | VHZ_KVV))
        }
        // qk rmsnorm (qk_norm models): q@0 (q_dev), meta@2, norm rows@3, eps@4, k@5 (kv_dev, k at qd)
        if (g_rd.qk_norm) {
            L.m_qkn = rd_meta()
            L.s_qkn = rd_set6(g_rd.q_dev, g_rd.dummy, L.m_qkn.buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.kv_dev,
                qd * 4l, 256l, META_BYTES, g_rd.norms_bytes, 64l, (qd + 2l * kvd) * 4l,
                fixed_array(VHZ_Q, 0u, 0u, 0u, 0u, VHZ_KVK))
        }
        // rope+store: kv@0, Kmirror@1, meta@2, Vmirror@3, cossin@4, q@5
        let mirbytes = g_rd.n_layers * g_rd.seq_cap * kvd * 4l
        L.s_rope = rd_set6(g_rd.kv_dev, g_rd.k_mirror, L.m_rope.buf, g_rd.v_mirror, g_rd.cos_dev, g_rd.q_dev,
            (qd + 2l * kvd) * 4l, mirbytes, META_BYTES, mirbytes, g_rd.head_size * 4l, qd * 4l,
            fixed_array(VHZ_KVK | VHZ_KVV, VHZ_MIR, 0u, VHZ_MIR, VHZ_COS, VHZ_Q))
        // attn: q@0, Kmirror@1, meta@2, Vmirror@3, scale@4, out@5
        L.s_attn = rd_set6(g_rd.q_dev, g_rd.k_mirror, L.m_attn.buf, g_rd.v_mirror, g_rd.scal_dev, g_rd.attn_dev,
            qd * 4l, mirbytes, META_BYTES, mirbytes, 64l, qd * 4l,
            fixed_array(VHZ_Q, VHZ_MIR, 0u, VHZ_MIR, 0u, VHZ_ATT))
        // quantize attn -> aq/as
        L.s_quant_at = rd_set6(g_rd.attn_dev, g_rd.dummy, L.m_quant_at.buf, g_rd.aq_dev, g_rd.as_dev, g_rd.dummy,
            qd * 4l, 256l, META_BYTES, qd, (qd / 32l) * 4l, 256l,
            fixed_array(VHZ_ATT, 0u, 0u, VHZ_AQ, VHZ_AQ, 0u))
        // wo GEMV -> xb2
        L.s_wo = rd_set6(po.wq, po.ws, L.m_wo.buf, g_rd.aq_dev, g_rd.as_dev, g_rd.xb2_dev,
            po.wqb, po.wsb, META_BYTES, qd, (qd / 32l) * 4l, dim * 4l,
            fixed_array(0u, 0u, 0u, VHZ_AQ, VHZ_AQ, VHZ_XB2))
        // addrms: x@0 += xb2@1, meta@2, norms@3, scalars@4, xb@5
        L.s_addr_ffn = rd_set6(g_rd.x_dev, g_rd.xb2_dev, L.m_addr_ffn.buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.xb_dev,
            dim * 4l, dim * 4l, META_BYTES, g_rd.norms_bytes, 64l, dim * 4l,
            fixed_array(VHZ_X, VHZ_XB2, 0u, 0u, 0u, VHZ_XB))
        // quantize xb (post-ffn-norm) -> xq/xs
        L.s_quant_xb2 = rd_set6(g_rd.xb_dev, g_rd.dummy, L.m_quant_xb2.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.dummy,
            dim * 4l, 256l, META_BYTES, dim, (dim / 32l) * 4l, 256l,
            fixed_array(VHZ_XB, 0u, 0u, VHZ_XQ, VHZ_XQ, 0u))
        // gate/up GEMV
        L.s_gate = rd_set6(p1.wq, p1.ws, L.m_gate.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.gate_dev,
            p1.wqb, p1.wsb, META_BYTES, dim, (dim / 32l) * 4l, hid * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_GATE))
        L.s_up = rd_set6(p3.wq, p3.ws, L.m_up.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.up_dev,
            p3.wqb, p3.wsb, META_BYTES, dim, (dim / 32l) * 4l, hid * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_UP))
        // act+requant: up@0, filler@1, ffn_meta@2, hq@3, hs@4, gate@5
        L.s_actrq = rd_set6(g_rd.up_dev, g_rd.dummy, g_gpu.ffn_meta_dev, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.gate_dev,
            hid * 4l, 256l, FFN_META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, hid * 4l,
            fixed_array(VHZ_UP, 0u, VHZ_META, VHZ_HQ, VHZ_HQ, VHZ_GATE))
        // down GEMV -> ffnout
        L.s_down = rd_set6(p2.wq, p2.ws, L.m_down.buf, g_gpu.hq_dev, g_gpu.hs_dev, g_rd.ffnout_dev,
            p2.wqb, p2.wsb, META_BYTES, hid, (hid / 32l) * 4l, dim * 4l,
            fixed_array(0u, 0u, 0u, VHZ_HQ, VHZ_HQ, VHZ_FFO))
        // addrms next: x@0 += ffnout@1, norms@3, xb@5
        L.s_addr_next = rd_set6(g_rd.x_dev, g_rd.ffnout_dev, L.m_addr_next.buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.xb_dev,
            dim * 4l, dim * 4l, META_BYTES, g_rd.norms_bytes, 64l, dim * 4l,
            fixed_array(VHZ_X, VHZ_FFO, 0u, 0u, 0u, VHZ_XB))
        L.made = true
    }
    g_rd.tok_recorded = false   // the recorded chain references the replaced sets
}

//! Register the classifier plane (arena block + format) and build the prologue / final-norm / cls
//! sets. Call after all layers are set.
def vk_rdec_set_cls(cls_block : int64; cls_fmt : int) {
    assert(g_rd != null, "vk_rdec_set_cls before prepare")
    let dim = g_rd.dim
    let vocab = g_rd.vocab
    unsafe {

        g_rd.m_prologue = rd_meta()
        g_rd.m_quant_final = rd_meta()
        g_rd.cls_meta_dev = make_device_buf(CLS_META_BYTES)
        // prologue: xb = rms_att[0](x), add off
        g_rd.s_prologue = rd_set6(g_rd.x_dev, g_rd.dummy, g_rd.m_prologue.buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.xb_dev,
            dim * 4l, 256l, META_BYTES, g_rd.norms_bytes, 64l, dim * 4l,
            fixed_array(VHZ_X, 0u, 0u, 0u, 0u, VHZ_XB))
        // quantize final xb
        g_rd.s_quant_final = rd_set6(g_rd.xb_dev, g_rd.dummy, g_rd.m_quant_final.buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.dummy,
            dim * 4l, 256l, META_BYTES, dim, (dim / 32l) * 4l, 256l,
            fixed_array(VHZ_XB, 0u, 0u, VHZ_XQ, VHZ_XQ, 0u))
        // cls GEMV: arena[fmt]@0/1, device meta@2, xq@3, xs@4, logits@5
        let pc = arena_planes(cls_fmt)
        g_rd.cls_set = rd_set6(pc.wq, pc.ws, g_rd.cls_meta_dev, g_rd.xq_dev, g_rd.xs_dev, g_rd.logits_dev,
            pc.wqb, pc.wsb, CLS_META_BYTES, dim, (dim / 32l) * 4l, vocab * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_LOG))
        // cls meta is constant (block base, n=dim, d=vocab) — device-filled once
        var mp = reinterpret<uint?>(g_gpu.staging.mapped)
        mp[0] = uint(dim)
        mp[1] = uint(vocab)
        mp[2] = 1u
        mp[3] = 0u
        mp[4] = uint(cls_block)
        mp[5] = 0u
        staged_upload(g_rd.cls_meta_dev, 0l, CLS_META_BYTES)
        g_rd.cls_fmt_marker = cls_fmt
        g_rd.tok_recorded = false   // the recorded chain references the replaced cls/prologue sets
        g_rd.ready = true
    }
}

// gemv meta: [0]=n [1]=d [2]=nregions=1 [3]=ybase(row) [4]=weight block [5]=xblock base
def private fill_gemv_meta(hb : HostBuf; n, d, ybase, blk : int64) {
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = 1u
        mp[3] = uint(ybase)
        mp[4] = uint(blk)
        mp[5] = 0u
    }
}

// two-region form: regions land contiguously in y at ybase, ybase+d (groupn's r*d+row rule)
def private fill_gemv_meta2(hb : HostBuf; n, d, ybase, blk_a, blk_b : int64) {
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = 2u
        mp[3] = uint(ybase)
        mp[4] = uint(blk_a)
        mp[5] = 0u
        mp[6] = uint(blk_b)
        mp[7] = 0u
    }
}

// dn_requant meta: [10]=input base (0) [18]=block count
def private fill_quant_meta(hb : HostBuf; nblocks : int64) {
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[10] = 0u
        mp[18] = uint(nblocks)
    }
}

// the f32->f16 convert's meta (arena cm2 chain): word 0 = element count
def private fill_cvt_meta(hb : HostBuf; elems : int64) {
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(elems)
    }
}

def private fill_addrms_meta(hb : HostBuf; dim, woff : int64; add : bool) {
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(dim)
        mp[1] = add ? 1u : 0u
        mp[2] = uint(woff)
    }
}

def private rd_gemv_wg(rows : int64) : uint => uint((rows + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg)

// ===== GPU per-dispatch profiler (DASLLAMA_GPU_PROF=1) =====
// BOTTOM_OF_PIPE stamps; results ride a same-submit copy, no extra fence. Overlapped hazard
// levels put the whole level's cost on the LAST member's delta (earlier members read near-zero)
let private PFQ_CAP = 2048u
var private g_pfq_pool : QueryPool
var private g_pfq_buf : HostBuf
var private g_pfq_ready = false
var private g_pfq_on = false
var private g_pfq_n = 0u
var private g_pfq_period = 0.0

def private ensure_pfq {
    if (g_pfq_ready) {
        return
    }
    var lp : VkPhysicalDeviceProperties
    vkGetPhysicalDeviceProperties(g_gpu.phys, lp)
    g_pfq_period = lp.limits.timestampPeriod
    g_pfq_pool <- create_query_pool(g_gpu.device, QueryPoolCreateInfo(queryType = VkQueryType.TIMESTAMP, queryCount = PFQ_CAP))
    g_pfq_buf = make_host_buf(int64(PFQ_CAP) * 8l, true, [cached = true])
    g_gpu.host_mem |> erase(g_pfq_buf.buf)   // process-lifetime — exempt from the model-drop sweep
    g_pfq_ready = true
}

def private pfq_begin(raw : VkCommandBuffer) {
    if (!vk_prof()) {
        return
    }
    ensure_pfq()
    g_pfq_on = true
    g_pfq_n = 0u
    cmd_reset_query_pool(vk_value_to_boost(raw), g_pfq_pool, 0u, PFQ_CAP)
}

def private pfq_ts(raw : VkCommandBuffer) {
    if (g_pfq_on && g_pfq_n < PFQ_CAP) {
        var stage : VkPipelineStageFlags
        stage.bottom_of_pipe = true
        cmd_write_timestamp(vk_value_to_boost(raw), stage, g_pfq_pool, g_pfq_n)
        g_pfq_n++
    }
}

def private pfq_end(raw : VkCommandBuffer) {
    if (g_pfq_on) {
        var qrf : VkQueryResultFlags
        qrf._64 = true
        qrf.wait = true
        cmd_copy_query_pool_results(vk_value_to_boost(raw), g_pfq_pool, 0u, g_pfq_n,
            nonowning_buf(g_pfq_buf.buf), 0ul, 8ul, qrf)
        g_pfq_on = false
    }
}

// query i's delta to its predecessor, usec
def private pfq_us(i : uint) : double {
    unsafe {
        var tp = reinterpret<uint64?>(g_pfq_buf.mapped)
        return double(tp[i] - tp[i - 1u]) * double(g_pfq_period) / 1000.0lf
    }
}

// ===== the hazard-mask rail (the dasLLAMA-metal bar_dep port: declared masks, derived barriers) =====
// Nodes declare role-bit (region, not VkBuffer) r/w masks; barrier only on RAW/WAW/WAR vs pending, then reset.
// Barrier dst is always full-stage (the wholesale reset relies on it); src narrows to the pending stages.

let private VHZ_X = 0x1u          // residual x
let private VHZ_XB = 0x2u         // normed panel xb
let private VHZ_XQ = 0x4u         // quantized activations xq+xs
let private VHZ_Q = 0x8u          // q rows
let private VHZ_KVK = 0x10u       // kv panel, k half
let private VHZ_KVV = 0x20u       // kv panel, v half
let private VHZ_VST = 0x40u       // prefill v-GEMM staging (pf_v)
let private VHZ_MIR = 0x80u       // k/v mirrors
let private VHZ_ATT = 0x100u      // attention out
let private VHZ_AQ = 0x200u       // quantized attention out aq+as
let private VHZ_XB2 = 0x400u      // wo out
let private VHZ_GATE = 0x800u     // ffn gate plane
let private VHZ_UP = 0x1000u      // ffn up plane
let private VHZ_HQ = 0x2000u      // requantized hidden hq+hs
let private VHZ_FFO = 0x4000u     // ffn down out
let private VHZ_META = 0x8000u    // ffn_meta_dev
let private VHZ_LOG = 0x10000u    // logits
let private VHZ_COS = 0x20000u    // rope cos/sin rows

// batch/hybrid-tier region bits — a separate namespace over the same rail (each recorder's VkHaz
// is private to its cmd buffer, so the two families never meet in one pending set)
let private VHB_ACT = 0x1u        // batch_xq/batch_xs activation staging
let private VHB_META = 0x2u       // stack batch metas + the chain's params (ffn/dn/at meta)
let private VHB_Y1 = 0x4u         // batch_y1 (gate / qkv / q|gate / down / wo out)
let private VHB_Y2 = 0x8u         // batch_y2 (up / z / k out)
let private VHB_HQ = 0x10u        // hq+hs requant image
let private VHB_COMB = 0x20u      // combine weight/bucket planes
let private VHB_CY = 0x40u        // combine accumulator
let private VHB_SM = 0x80u        // smalls image (dn/at)
let private VHB_ST = 0x100u       // dn recurrent state
let private VHB_WS = 0x200u       // dn slabs/o scratch
let private VHB_ATQ = 0x400u      // at deinterleaved q rows
let private VHB_ATG = 0x800u      // at gate/o rows
let private VHB_ATK = 0x1000u     // at roped-k panel
let private VHB_ATV = 0x2000u     // at v panel (absolute positions)
let private VHB_VRAW = 0x4000u    // at raw-v window

// serial-seam region bits (decode GEMV preps, arena + standalone seams) — third namespace, same rail
let private VHG_META = 0x1u       // ffn/dn params staged to device
let private VHG_SM = 0x2u         // dn smalls image (β/γ + parity ring)
let private VHG_Y1 = 0x4u         // gate / qkv / q out
let private VHG_Y2 = 0x8u         // up / z / k out
let private VHG_Y3 = 0x10u        // v out (qkv group, rope v mirror)
let private VHG_HQ = 0x20u        // hq+hs requant image
let private VHG_ST = 0x40u        // dn recurrent state (in place)
let private VHG_WS = 0x80u        // dn o scratch (oq+os)
let private VHG_X = 0x100u        // add_rms residual x (in place)
let private VHG_YO = 0x200u       // the host-bound result plane (the DMA source)

struct private VkHaz {
    w : uint            // bits with a pending (unbarriered) write
    r : uint            // bits with a pending read
    pend_compute : bool // stages present in the pending set — the next barrier's src side
    pend_transfer : bool
    barriers : int
    disp : int
}

// ===== the derived-dispatch registry (the vulkan lens) =====
// Pipelines register lens-derived binding access masks at creation, descriptor sets declare
// per-binding region bits at build — hz_dispatch composes the two into vhz_dep masks.

var private g_kernel_access : table<string; uint2>   // kernel name -> (read-bindings, write-bindings)
var private g_pipe_access : table<uint64; uint2>     // VkPipeline -> (read-bindings, write-bindings)
var private g_set_bits : table<uint64; SetBits>      // VkDescriptorSet -> per-binding region-bit masks

struct private SetBits {
    b : uint[6]
}

def private ensure_kernel_access {
    if (!empty(g_kernel_access)) {
        return
    }
    for (ent in split(vk_kernel_access_spec(), ";")) {
        if (ent == "") {
            continue
        }
        let nv <- split(ent, "=")
        let rw <- split(nv[1], ",")
        g_kernel_access |> insert(nv[0], uint2(uint(to_int(rw[0])), uint(to_int(rw[1]))))
    }
}

//! Debug: log the lens-derived per-kernel binding access masks (bindings 0-5, hex bit masks).
def vk_dump_kernel_access {
    ensure_kernel_access()
    for (k, v in keys(g_kernel_access), values(g_kernel_access)) {
        to_log(LOG_INFO, "vk_access {k} r={v.x} w={v.y}\n")
    }
}

// declare the region bits sitting at each of a set's 6 bindings (0 = untracked); a set must be
// declared before hz_dispatch can serve it — the strict rail panics on an undeclared set
def private hz_set_bits(set_ : VkDescriptorSet; b0, b1, b2, b3, b4, b5 : uint) {
    var sb : SetBits
    sb.b[0] = b0
    sb.b[1] = b1
    sb.b[2] = b2
    sb.b[3] = b3
    sb.b[4] = b4
    sb.b[5] = b5
    g_set_bits[intptr(set_)] = sb
}

def private hz_set_bits(set_ : VkDescriptorSet; gbits : uint[6]) {
    var sb : SetBits
    for (i in range(6)) {
        sb.b[i] = gbits[i]
    }
    g_set_bits[intptr(set_)] = sb
}

def private hz_pipe_register(pipe : Pipeline; kernel : string) {
    ensure_kernel_access()
    if (!key_exists(g_kernel_access, kernel)) {
        panic("dasLLAMA vulkan lens: no derived access for kernel {kernel}")
    }
    g_pipe_access[intptr(boost_value_to_vk(pipe))] = g_kernel_access[kernel]
}

// create a kernel's pipeline and register its lens-derived access masks (the shader-module local
// is deliberately never finalized — same lifetime the hand-written creation block had)
def private make_kernel_pipe(kernel : string; var spv : array<uint>) : Pipeline {
    var shader <- create_shader_module(g_gpu.device, spv)
    var pipe <- create_compute_pipeline(g_gpu.device, g_gpu.pipe_layout, shader)
    hz_pipe_register(pipe, kernel)
    return <- pipe
}

// derive this dispatch's hazard masks: the set's region bits distributed over the kernel's
// read/write bindings. Undeclared pipeline or set = a programming error, not a fallback.
def private hz_masks(pipe : Pipeline; var set_ : VkDescriptorSet) : tuple<r : uint; w : uint> {
    let pk = intptr(boost_value_to_vk(pipe))
    if (!key_exists(g_pipe_access, pk)) {
        panic("dasLLAMA vulkan lens: dispatch through an unregistered pipeline")
    }
    let sk = intptr(set_)
    if (!key_exists(g_set_bits, sk)) {
        panic("dasLLAMA vulkan lens: dispatch against a set with no declared region bits")
    }
    let acc = g_pipe_access[pk]
    let sb = g_set_bits[sk]
    var r = 0u
    var w = 0u
    for (i in range(6)) {
        let bit = 1u << uint(i)
        if ((acc.x & bit) != 0u) {
            r |= sb.b[i]
        }
        if ((acc.y & bit) != 0u) {
            w |= sb.b[i]
        }
    }
    return (r, w)
}

// the derived-mask dispatch node: declare via the composed masks, bind, dispatch
def private hz_dispatch(raw : VkCommandBuffer; var h : VkHaz; pipe : Pipeline; var set_ : VkDescriptorSet; groups : uint) {
    let m = hz_masks(pipe, set_)
    vhz_dep(raw, h, m.r, m.w)
    cmd_bind_pipeline(vk_value_to_boost(raw), pipe, VkPipelineBindPoint.COMPUTE)
    cmd_bind_dispatch(raw, set_, groups)
}

var private g_vhz_paranoid = -1   // DASLLAMA_VK_HAZARD_PARANOID: barrier before every node
var private g_vhz_trace = -1      // DASLLAMA_VK_HAZARD_TRACE: log every declared node (schedule oracle)
var private g_mm_m_logged = false   // one-time mul_mm M-tile engagement log

def private vhz_paranoid : bool {
    if (g_vhz_paranoid < 0) {
        g_vhz_paranoid = get_env_variable("DASLLAMA_VK_HAZARD_PARANOID") == "1" ? 1 : 0
    }
    return g_vhz_paranoid != 0
}

def private vhz_trace : bool {
    if (g_vhz_trace < 0) {
        g_vhz_trace = get_env_variable("DASLLAMA_VK_HAZARD_TRACE") == "1" ? 1 : 0
    }
    return g_vhz_trace != 0
}

var private g_vk_fa = -1   // DASLLAMA_VK_FA=0 pins the generic da_attn_b (flash-attn bisect hatch)

def private vk_fa_on : bool {
    if (g_vk_fa < 0) {
        g_vk_fa = get_env_variable("DASLLAMA_VK_FA") == "0" ? 0 : 1
    }
    return g_vk_fa != 0
}

// the per-node gate: barrier iff this node's declared masks conflict with the pending set
def private vhz_dep(raw : VkCommandBuffer; var h : VkHaz; rmask, wmask : uint; transfer : bool = false) {
    h.disp++
    if (vhz_trace()) {
        to_log(LOG_INFO, "vhz {h.disp} r={rmask} w={wmask} t={transfer ? 1 : 0}\n")
    }
    var bar = (rmask & h.w) != 0u || (wmask & h.w) != 0u || (wmask & h.r) != 0u
    if (vhz_paranoid() && (h.w != 0u || h.r != 0u)) {
        bar = true
    }
    if (bar) {
        var mb : MemoryBarrier
        mb.srcAccessMask.shader_write = h.pend_compute
        mb.srcAccessMask.transfer_write = h.pend_transfer
        mb.dstAccessMask.shader_read = true
        mb.dstAccessMask.shader_write = true
        mb.dstAccessMask.transfer_read = true
        mb.dstAccessMask.transfer_write = true
        var mbs : array<MemoryBarrier>
        mbs |> emplace(mb)
        var sstage : VkPipelineStageFlags
        sstage.compute_shader = h.pend_compute
        sstage.transfer = h.pend_transfer
        var dstage : VkPipelineStageFlags
        dstage.compute_shader = true
        dstage.transfer = true
        let dflags : VkDependencyFlags
        let no_bufb : array<BufferMemoryBarrier>
        let no_imgb : array<ImageMemoryBarrier>
        cmd_pipeline_barrier(vk_value_to_boost(raw), sstage, dstage, dflags, mbs, no_bufb, no_imgb)
        h.w = 0u
        h.r = 0u
        h.pend_compute = false
        h.pend_transfer = false
        h.barriers++
    }
    h.r |= rmask
    h.w |= wmask
    if (transfer) {
        h.pend_transfer = true
    } else {
        h.pend_compute = true
    }
}

def private rd_dispatch(raw : VkCommandBuffer; pipe : Pipeline; var set : VkDescriptorSet; groups : uint; var h : VkHaz) {
    hz_dispatch(raw, h, pipe, set, groups)
    pfq_ts(raw)
}

// GEMV dispatch — the per-format kernel selected off the plane format
def private rd_dispatch_gemv(raw : VkCommandBuffer; fmt : int; var set : VkDescriptorSet; groups : uint; var h : VkHaz) {
    rd_dispatch(raw, gemv_pipe(fmt), set, groups, h)
}

// Record the whole-token cmd ONCE per set_layer/set_cls epoch: constant metas + scalars fill here;
// per-token state is only x/cos (in-cmd copies) and the rope/attn position words (host-visible)
def private rd_record_token {
    let dim = g_rd.dim
    let qd = g_rd.qd
    let kvd = g_rd.kv_dim
    let hid = g_rd.hidden
    let nlfin = g_rd.n_layers * 2l * dim   // rms_final offset in the norms buffer (dim units later)
    unsafe {
        // constant scalars — attn ([0]=scale) and add_rms ([0]=eps, [1]=ascale)
        var sp = reinterpret<float?>(g_gpu.staging.mapped)
        sp[0] = g_rd.scale
        staged_upload(g_rd.scal_dev, 0l, 64l)
        sp[0] = g_rd.eps
        sp[1] = 1.0
        staged_upload(g_rd.ar_scal_dev, 0l, 64l)
        // decode-owned actrq meta — the in-cmd copy re-establishes ffn_meta_dev every submit (batch rails share it)
        var fp = reinterpret<uint?>(g_rd.ffn_meta_h.mapped)
        fp[0] = uint(hid)
        fp[1] = 0u
        fp[2] = uint(hid / 32l)
        for (i in range(5)) {
            fp[3 + i] = 0u
        }
        fill_addrms_meta(g_rd.m_prologue, dim, 0l, false)   // rms_att[0] at offset 0
        for (l in range64(g_rd.n_layers)) {
            var L & = g_rd.layers[l]
            fill_quant_meta(L.m_quant_xb, dim / 32l)
            fill_gemv_meta(L.m_q, dim, qd, 0l, L.bq)
            if (L.kv_merged) {
                fill_gemv_meta2(L.m_k, dim, kvd, qd, L.bk, L.bv)  // k, v regions land at qd, qd+kvd
            } else {
                fill_gemv_meta(L.m_k, dim, kvd, qd, L.bk)         // k at ybase qd in kv_dev
                fill_gemv_meta(L.m_v, dim, kvd, qd + kvd, L.bv)   // v at ybase qd+kvd
            }
            if (g_rd.qk_norm) {
                let hs = g_rd.head_size
                var mn = reinterpret<uint?>(L.m_qkn.mapped)
                mn[0] = uint(hs); mn[1] = uint(g_rd.n_heads); mn[2] = uint(kvd / hs)
                mn[3] = uint(qd); mn[4] = 0u; mn[5] = uint(qd)   // one row: k lives at qd in kv_dev
                mn[6] = uint(nlfin + dim + l * 2l * hs)          // rms_q[l] past the fixed norms layout
                mn[7] = uint(nlfin + dim + (l * 2l + 1l) * hs)   // rms_k[l]
            }
            var mr = reinterpret<uint?>(L.m_rope.mapped)
            mr[0] = uint(qd); mr[1] = uint(kvd); mr[2] = uint(g_rd.head_size); mr[3] = uint(g_rd.head_size / 2l)
            mr[4] = g_rd.neox ? 1u : 0u; mr[5] = uint(qd / 2l); mr[6] = uint(qd / 2l + kvd / 2l)
            var ma = reinterpret<uint?>(L.m_attn.mapped)
            ma[2] = uint(kvd); ma[3] = uint(g_rd.head_size); ma[4] = uint(g_rd.kv_mul)
            ma[5] = uint(l * g_rd.seq_cap * kvd); ma[6] = uint(l * g_rd.seq_cap * kvd)
            fill_quant_meta(L.m_quant_at, qd / 32l)
            fill_gemv_meta(L.m_wo, qd, dim, 0l, L.bo)
            fill_addrms_meta(L.m_addr_ffn, dim, (l * 2l + 1l) * dim, true)   // rms_ffn[l]
            fill_quant_meta(L.m_quant_xb2, dim / 32l)
            fill_gemv_meta(L.m_gate, dim, hid, 0l, L.b1)
            fill_gemv_meta(L.m_up, dim, hid, 0l, L.b3)
            fill_gemv_meta(L.m_down, hid, dim, 0l, L.b2)
            let nxt = l + 1l < g_rd.n_layers ? (l + 1l) * 2l * dim : nlfin   // rms_att[l+1] or final
            fill_addrms_meta(L.m_addr_next, dim, nxt, true)
        }
        fill_quant_meta(g_rd.m_quant_final, dim / 32l)
        var raw = g_rd.cmd
        let rf : VkCommandBufferResetFlags
        vk_check(vkResetCommandBuffer(raw, rf), null)
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        pfq_begin(raw)
        pfq_ts(raw)   // t0 anchor — the recorded-once cmd re-stamps every token submit
        var h : VkHaz
        vhz_dep(raw, h, 0u, VHZ_X, true)
        cmd_copy_whole(raw, g_rd.x_host.buf, g_rd.x_dev, dim * 4l)
        vhz_dep(raw, h, 0u, VHZ_COS, true)
        cmd_copy_whole(raw, g_rd.cos_host.buf, g_rd.cos_dev, g_rd.head_size * 4l)
        vhz_dep(raw, h, 0u, VHZ_META, true)
        cmd_copy_whole(raw, g_rd.ffn_meta_h.buf, g_gpu.ffn_meta_dev, FFN_META_BYTES)
        rd_dispatch(raw, g_gpu.ar_pipeline, g_rd.s_prologue, 1u, h)   // xb = rms_att[0](x)
        for (l in range64(g_rd.n_layers)) {
            var L & = g_rd.layers[l]
            rd_dispatch(raw, g_gpu.dn_rq_pipeline, L.s_quant_xb, uint((dim / 4l + 255l) / 256l), h)
            rd_dispatch_gemv(raw, L.fq, L.s_q, rd_gemv_wg(qd), h)
            if (L.kv_merged) {
                rd_dispatch_gemv(raw, L.fk, L.s_kv, rd_gemv_wg(2l * kvd), h)
            } else {
                rd_dispatch_gemv(raw, L.fk, L.s_k, rd_gemv_wg(kvd), h)
                rd_dispatch_gemv(raw, L.fv, L.s_v, rd_gemv_wg(kvd), h)
            }
            if (g_rd.qk_norm) {
                rd_dispatch(raw, g_gpu.qkn_pipeline, L.s_qkn, uint(g_rd.n_heads + kvd / g_rd.head_size), h)
            }
            rd_dispatch(raw, g_gpu.rks_pipeline, L.s_rope, uint((qd / 2l + kvd / 2l + int64(WG_X) - 1l) / int64(WG_X)), h)
            rd_dispatch(raw, g_gpu.da_pipeline, L.s_attn, uint(g_rd.n_heads), h)
            rd_dispatch(raw, g_gpu.dn_rq_pipeline, L.s_quant_at, uint((qd / 4l + 255l) / 256l), h)
            rd_dispatch_gemv(raw, L.fo, L.s_wo, rd_gemv_wg(dim), h)
            rd_dispatch(raw, g_gpu.ar_pipeline, L.s_addr_ffn, 1u, h)
            rd_dispatch(raw, g_gpu.dn_rq_pipeline, L.s_quant_xb2, uint((dim / 4l + 255l) / 256l), h)
            rd_dispatch_gemv(raw, L.f1, L.s_gate, rd_gemv_wg(hid), h)
            rd_dispatch_gemv(raw, L.f3, L.s_up, rd_gemv_wg(hid), h)
            rd_dispatch(raw, g_gpu.actrq_pipeline, L.s_actrq, uint((hid / 4l + 255l) / 256l), h)
            rd_dispatch_gemv(raw, L.f2, L.s_down, rd_gemv_wg(dim), h)
            rd_dispatch(raw, g_gpu.ar_pipeline, L.s_addr_next, 1u, h)
        }
        rd_dispatch(raw, g_gpu.dn_rq_pipeline, g_rd.s_quant_final, uint((dim / 4l + 255l) / 256l), h)
        rd_dispatch_gemv(raw, g_rd.cls_fmt_marker, g_rd.cls_set, rd_gemv_wg(g_rd.vocab), h)
        vhz_dep(raw, h, VHZ_LOG, 0u, true)   // compute -> transfer for the logits DMA
        cmd_copy_whole(raw, g_rd.logits_dev, g_rd.logits_host.buf, g_rd.vocab * 4l)
        pfq_end(raw)
        vk_check(vkEndCommandBuffer(raw), null)
    }
    g_rdq_n = g_pfq_n   // decode's stamp count (prefill re-arms the shared pool per window)
    g_rd.tok_recorded = true
}

// decode per-role accumulation across tokens (stamps overwrite each submit); printed every 32
var private g_rdq_n = 0u
var private g_rdq_count = 0l
var private g_rdq_total = 0.0lf
var private g_rdq_pro = 0.0lf
var private g_rdq_tail = 0.0lf
var private g_rdq_role : double[16]

def private rdq_sample {
    // stamp count follows each layer's own kv_merged (K-quant ggufs mix formats per layer)
    var want = 4   // anchor + prologue + fin_rq + cls
    let base_rpl = g_rd.qk_norm ? 16 : 15
    for (L in g_rd.layers) {
        want += base_rpl - (L.kv_merged ? 1 : 0)
    }
    if (g_rdq_n != uint(want)) {
        return   // deep graphs overflow PFQ_CAP — no aggregation
    }
    g_rdq_pro += pfq_us(1u)
    // k+v aggregate into ONE "kv" slot (merged = 1 stamp, split = 2) — uniform incl. mixed models
    var qi = 2u
    for (L in g_rd.layers) {
        g_rdq_role[0] += pfq_us(qi)
        g_rdq_role[1] += pfq_us(qi + 1u)
        g_rdq_role[2] += pfq_us(qi + 2u)
        qi += 3u
        if (!L.kv_merged) {
            g_rdq_role[2] += pfq_us(qi)
            qi++
        }
        var s = 3
        var rest = base_rpl - 4   // qkn? rope attn rq_a wo ar1 rq_f gate up actrq down ar2
        while (rest > 0) {
            g_rdq_role[s] += pfq_us(qi)
            s++
            qi++
            rest--
        }
    }
    g_rdq_tail += pfq_us(g_rdq_n - 2u) + pfq_us(g_rdq_n - 1u)
    unsafe {
        var tp = reinterpret<uint64?>(g_pfq_buf.mapped)
        g_rdq_total += double(tp[g_rdq_n - 1u] - tp[0u]) * double(g_pfq_period) / 1000.0lf
    }
    g_rdq_count++
    if (g_rdq_count % 32l == 0l) {
        let n = double(g_rdq_count)
        var names : array<string>
        if (g_rd.qk_norm) {
            names <- ["rq_x", "q", "kv", "qkn", "rope", "attn", "rq_a", "wo", "ar1", "rq_f", "gate", "up", "actrq", "down", "ar2"]
        } else {
            names <- ["rq_x", "q", "kv", "rope", "attn", "rq_a", "wo", "ar1", "rq_f", "gate", "up", "actrq", "down", "ar2"]
        }
        let rs = build_string() $(w) {
            for (s in range(length(names))) {
                w |> write("{names[s]} {int(g_rdq_role[s] / n)} ")
            }
        }
        delete names
        to_log(LOG_INFO, "vk_rdec gpu avg/token over {g_rdq_count}: prologue {int(g_rdq_pro / n)} {rs}tail {int(g_rdq_tail / n)} total {int(g_rdq_total / n)}\n")
    }
}

//! Resident-driver token: run the whole stack on device from the embedded residual `x` and this
//! position's rope row (cos[half]+sin[half]) -> logits. `cnt` = cached positions incl. this one; K/V
//! for `pos` store into the mirror. Recorded once — a step is two memcpys + position words + one submit.
def vk_rdec_token(x : array<float>; cossin : array<float>; pos, cnt : int64; var logits : array<float>) {
    assert(g_rd != null && g_rd.ready, "vk_rdec_token before prepare/set_cls")
    if (!g_rd.tok_recorded) {
        rd_record_token()
    }
    let kvd = g_rd.kv_dim
    unsafe {
        memcpy(g_rd.x_host.mapped, addr<void? -const>(x[0]), int(g_rd.dim * 4l))
        memcpy(g_rd.cos_host.mapped, addr<void? -const>(cossin[0]), int(g_rd.head_size * 4l))
        for (l in range64(g_rd.n_layers)) {
            var L & = g_rd.layers[l]
            var mr = reinterpret<uint?>(L.m_rope.mapped)
            mr[7] = uint(l * g_rd.seq_cap * kvd + pos * kvd)
            mr[8] = uint(l * g_rd.seq_cap * kvd + pos * kvd)
            var ma = reinterpret<uint?>(L.m_attn.mapped)
            ma[0] = uint(cnt)
        }
        submit_wait(g_rd.cmd)
        memcpy(addr<void?>(logits[0]), g_rd.logits_host.mapped, int(g_rd.vocab * 4l))
    }
    if (vk_prof() && g_rdq_n > 2u) {
        rdq_sample()
    }
}

// ===== the resident prefill driver (batched, npos rows, one submit) =====
// Batch-kernel twin of vk_rdec_token; only the LAST row's logits are produced (reuses the decode
// tail). PF_ROLES prefill sets/layer + the buffers are built once; metas fill per call (npos varies).

let private PF_ROLES = 16l   // quant_xb q k v rope attn quant_at wo addr_ffn quant_xb2 gate up actrq down addr_next qkn

def private pf_bind(idx : int; b0, b1, b2, b3, b4, b5 : uint64; s0, s1, s2, s3, s4, s5 : int64; gbits : uint[6]) {
    g_rd.pf_sets[idx] = rd_set6(b0, b1, b2, b3, b4, b5, s0, s1, s2, s3, s4, s5, gbits)
}

def private pf_setup {
    if (g_rd.pf_ready) {
        return
    }
    let np = PF_WINDOW
    g_rd.pf_np = np
    let dim = g_rd.dim
    let qd = g_rd.qd
    let kvd = g_rd.kv_dim
    let hid = g_rd.hidden
    let wide = max(max(dim, qd), hid)
    g_rd.pf_x = make_device_buf(np * dim * 4l)
    g_rd.pf_xb = make_device_buf(np * dim * 4l)
    g_rd.pf_xq = make_device_buf(np * wide)
    g_rd.pf_xs = make_device_buf(np * (wide / 32l) * 4l)
    g_rd.pf_q = make_device_buf(np * qd * 4l)
    g_rd.pf_kv = make_device_buf(np * 2l * kvd * 4l)
    g_rd.pf_v = make_device_buf(np * kvd * 4l)   // v GEMM output (copied into pf_kv's v half; the batch GEMM can't offset its output independently of its input row)
    g_rd.pf_attn = make_device_buf(np * qd * 4l)
    g_rd.pf_aq = make_device_buf(np * qd)
    g_rd.pf_as = make_device_buf(np * (qd / 32l) * 4l)
    g_rd.pf_gate = make_device_buf(np * hid * 4l)
    g_rd.pf_up = make_device_buf(np * hid * 4l)
    g_rd.pf_ffnout = make_device_buf(np * dim * 4l)
    g_rd.pf_xb2 = make_device_buf(np * dim * 4l)
    g_rd.pf_cos = make_device_buf(np * g_rd.head_size * 4l)
    g_rd.pf_logits_host = make_host_buf(g_rd.vocab * 4l, false, [cached = true])
    g_rd.pf_cmd = alloc_cmd()
    ensure_batch_state()   // hq_dev/hs_dev for the FFN act
    // fmt-6 (cm2) groups feed the twin f16 rows instead of q8 images
    var any6 = false
    for (L in g_rd.layers) {
        any6 = any6 || L.fq == 6 || L.fo == 6 || L.f1 == 6 || L.f2 == 6
    }
    if (any6) {
        g_rd.pf_xf = make_device_buf(np * dim * 2l)
        g_rd.pf_af = make_device_buf(np * qd * 2l)
        g_rd.pf_hf = make_device_buf(np * hid * 2l)
    }
    let nsets = int(g_rd.n_layers * PF_ROLES + 2l)   // + prologue, final quantize of the last row
    g_rd.pf_sets |> resize(nsets)
    g_rd.pf_meta |> resize(nsets)
    for (mi in range(nsets)) {
        g_rd.pf_meta[mi] = make_host_buf(BATCH_META_BYTES, true)
    }
    let mirbytes = g_rd.n_layers * g_rd.seq_cap * kvd * 4l
    let normbytes = g_rd.norms_bytes
    for (l in range64(g_rd.n_layers)) {
        var L & = unsafe(g_rd.layers[l])
        let b = int(l * PF_ROLES)
        let pq = arena_planes(L.fq); let pk = arena_planes(L.fk); let pv = arena_planes(L.fv)
        let po = arena_planes(L.fo); let p1 = arena_planes(L.f1); let p3 = arena_planes(L.f3); let p2 = arena_planes(L.f2)
        // fmt-6 groups run on the f16 twin plane at binding 3 (idle binding-4 bits are lens-ignored)
        let qkv6 = L.fq == 6
        let xqb = qkv6 ? g_rd.pf_xf : g_rd.pf_xq
        let xsb = qkv6 ? g_rd.dummy : g_rd.pf_xs
        let xqn = qkv6 ? np * dim * 2l : np * dim
        let xsn = qkv6 ? 256l : np * (dim / 32l) * 4l
        let wo6 = L.fo == 6
        let aqb = wo6 ? g_rd.pf_af : g_rd.pf_aq
        let asb = wo6 ? g_rd.dummy : g_rd.pf_as
        let aqn = wo6 ? np * qd * 2l : np * qd
        let asn = wo6 ? 256l : np * (qd / 32l) * 4l
        let gu6 = L.f1 == 6
        let gqb = gu6 ? g_rd.pf_xf : g_rd.pf_xq
        let gsb = gu6 ? g_rd.dummy : g_rd.pf_xs
        let gqn = gu6 ? np * dim * 2l : np * dim
        let gsn = gu6 ? 256l : np * (dim / 32l) * 4l
        let dn6 = L.f2 == 6
        let hqb = dn6 ? g_rd.pf_hf : g_gpu.hq_dev
        let hsb = dn6 ? g_rd.dummy : g_gpu.hs_dev
        let hqn = dn6 ? np * hid * 2l : np * hid
        let hsn = dn6 ? 256l : np * (hid / 32l) * 4l
        // 0 quant_xb: pf_xb -> pf_xq/pf_xs (fmt 6: f16 convert -> pf_xf)
        pf_bind(b + 0, g_rd.pf_xb, g_rd.dummy, g_rd.pf_meta[b + 0].buf, xqb, xsb, g_rd.dummy,
            np * dim * 4l, 256l, BATCH_META_BYTES, xqn, xsn, 256l,
            fixed_array(VHZ_XB, 0u, 0u, VHZ_XQ, VHZ_XQ, 0u))
        // 1/2/3 q/k/v batch GEMM: arena planes, pf_xq/pf_xs act, -> pf_q / pf_kv(k@0) / pf_kv(v@np*kvd)
        pf_bind(b + 1, pq.wq, pq.ws, g_rd.pf_meta[b + 1].buf, xqb, xsb, g_rd.pf_q,
            pq.wqb, pq.wsb, BATCH_META_BYTES, xqn, xsn, np * qd * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_Q))
        pf_bind(b + 2, pk.wq, pk.ws, g_rd.pf_meta[b + 2].buf, xqb, xsb, g_rd.pf_kv,
            pk.wqb, pk.wsb, BATCH_META_BYTES, xqn, xsn, np * 2l * kvd * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_KVK))
        pf_bind(b + 3, pv.wq, pv.ws, g_rd.pf_meta[b + 3].buf, xqb, xsb, g_rd.pf_v,
            pv.wqb, pv.wsb, BATCH_META_BYTES, xqn, xsn, np * kvd * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_VST))
        // 4 rope+store: q@0 in place, Kmir@1, Vmir@3, cos@4, kv@5
        pf_bind(b + 4, g_rd.pf_q, g_rd.k_mirror, g_rd.pf_meta[b + 4].buf, g_rd.v_mirror, g_rd.pf_cos, g_rd.pf_kv,
            np * qd * 4l, mirbytes, BATCH_META_BYTES, mirbytes, np * g_rd.head_size * 4l, np * 2l * kvd * 4l,
            fixed_array(VHZ_Q, VHZ_MIR, 0u, VHZ_MIR, VHZ_COS, VHZ_KVK | VHZ_KVV))
        // 5 attn: q@0, Kmir@1, Vmir@3, scale@4, out@5
        pf_bind(b + 5, g_rd.pf_q, g_rd.k_mirror, g_rd.pf_meta[b + 5].buf, g_rd.v_mirror, g_rd.scal_dev, g_rd.pf_attn,
            np * qd * 4l, mirbytes, BATCH_META_BYTES, mirbytes, 64l, np * qd * 4l,
            fixed_array(VHZ_Q, VHZ_MIR, 0u, VHZ_MIR, 0u, VHZ_ATT))
        // 6 quant_at: pf_attn -> pf_aq/pf_as (fmt 6: f16 convert -> pf_af)
        pf_bind(b + 6, g_rd.pf_attn, g_rd.dummy, g_rd.pf_meta[b + 6].buf, aqb, asb, g_rd.dummy,
            np * qd * 4l, 256l, BATCH_META_BYTES, aqn, asn, 256l,
            fixed_array(VHZ_ATT, 0u, 0u, VHZ_AQ, VHZ_AQ, 0u))
        // 7 wo GEMM -> pf_xb2
        pf_bind(b + 7, po.wq, po.ws, g_rd.pf_meta[b + 7].buf, aqb, asb, g_rd.pf_xb2,
            po.wqb, po.wsb, BATCH_META_BYTES, aqn, asn, np * dim * 4l,
            fixed_array(0u, 0u, 0u, VHZ_AQ, VHZ_AQ, VHZ_XB2))
        // 8 addr_ffn: x += xb2, xb = rms_ffn
        pf_bind(b + 8, g_rd.pf_x, g_rd.pf_xb2, g_rd.pf_meta[b + 8].buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.pf_xb,
            np * dim * 4l, np * dim * 4l, BATCH_META_BYTES, normbytes, 64l, np * dim * 4l,
            fixed_array(VHZ_X, VHZ_XB2, 0u, 0u, 0u, VHZ_XB))
        // 9 quant_xb2: pf_xb -> pf_xq (fmt 6: f16 convert -> pf_xf)
        pf_bind(b + 9, g_rd.pf_xb, g_rd.dummy, g_rd.pf_meta[b + 9].buf, gqb, gsb, g_rd.dummy,
            np * dim * 4l, 256l, BATCH_META_BYTES, gqn, gsn, 256l,
            fixed_array(VHZ_XB, 0u, 0u, VHZ_XQ, VHZ_XQ, 0u))
        // 10/11 gate/up GEMM
        pf_bind(b + 10, p1.wq, p1.ws, g_rd.pf_meta[b + 10].buf, gqb, gsb, g_rd.pf_gate,
            p1.wqb, p1.wsb, BATCH_META_BYTES, gqn, gsn, np * hid * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_GATE))
        pf_bind(b + 11, p3.wq, p3.ws, g_rd.pf_meta[b + 11].buf, gqb, gsb, g_rd.pf_up,
            p3.wqb, p3.wsb, BATCH_META_BYTES, gqn, gsn, np * hid * 4l,
            fixed_array(0u, 0u, 0u, VHZ_XQ, VHZ_XQ, VHZ_UP))
        // 12 actrq: up@0, ffn_meta@2, hq@3, hs@4, gate@5 (fmt 6: actf16 -> pf_hf@3)
        pf_bind(b + 12, g_rd.pf_up, g_rd.dummy, g_gpu.ffn_meta_dev, hqb, hsb, g_rd.pf_gate,
            np * hid * 4l, 256l, FFN_META_BYTES, dn6 ? hqn : BATCH_HQ_BYTES, dn6 ? hsn : BATCH_HS_BYTES, np * hid * 4l,
            fixed_array(VHZ_UP, 0u, VHZ_META, VHZ_HQ, VHZ_HQ, VHZ_GATE))
        // 13 down GEMM: hq/hs act -> pf_ffnout
        pf_bind(b + 13, p2.wq, p2.ws, g_rd.pf_meta[b + 13].buf, hqb, hsb, g_rd.pf_ffnout,
            p2.wqb, p2.wsb, BATCH_META_BYTES, hqn, hsn, np * dim * 4l,
            fixed_array(0u, 0u, 0u, VHZ_HQ, VHZ_HQ, VHZ_FFO))
        // 14 addr_next: x += ffnout, xb = rms_att[l+1]|final
        pf_bind(b + 14, g_rd.pf_x, g_rd.pf_ffnout, g_rd.pf_meta[b + 14].buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.pf_xb,
            np * dim * 4l, np * dim * 4l, BATCH_META_BYTES, normbytes, 64l, np * dim * 4l,
            fixed_array(VHZ_X, VHZ_FFO, 0u, 0u, 0u, VHZ_XB))
        // 15 qk rmsnorm (qk_norm models): q@0 (pf_q), norm rows@3, eps@4, k@5 (pf_kv, k at 0)
        if (g_rd.qk_norm) {
            pf_bind(b + 15, g_rd.pf_q, g_rd.dummy, g_rd.pf_meta[b + 15].buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.pf_kv,
                np * qd * 4l, 256l, BATCH_META_BYTES, normbytes, 64l, np * 2l * kvd * 4l,
                fixed_array(VHZ_Q, 0u, 0u, 0u, 0u, VHZ_KVK))
        }
    }
    let bp = int(g_rd.n_layers * PF_ROLES)
    // prologue: xb = rms_att[0](x), add off
    pf_bind(bp + 0, g_rd.pf_x, g_rd.dummy, g_rd.pf_meta[bp + 0].buf, g_rd.norms_dev, g_rd.ar_scal_dev, g_rd.pf_xb,
        np * dim * 4l, 256l, BATCH_META_BYTES, normbytes, 64l, np * dim * 4l,
        fixed_array(VHZ_X, 0u, 0u, 0u, 0u, VHZ_XB))
    // final quantize of the LAST row (into the decode driver's xq_dev/xs_dev) for the cls GEMV
    pf_bind(bp + 1, g_rd.pf_xb, g_rd.dummy, g_rd.pf_meta[bp + 1].buf, g_rd.xq_dev, g_rd.xs_dev, g_rd.dummy,
        np * dim * 4l, 256l, BATCH_META_BYTES, dim, (dim / 32l) * 4l, 256l,
        fixed_array(VHZ_XB, 0u, 0u, VHZ_XQ, VHZ_XQ, 0u))
    g_rd.pf_ready = true
}

//! Batch-decode support: upload one layer's f32 KV history ([npos x kvd] each) into the shared
//! mirror at layer `l`, positions [0, npos). Resyncs a session's cache before its decode step.
def vk_rdec_sync_kv(l : int64; kp : float const?; vp : float const?; npos : int64) {
    assert(g_rd != null, "vk_rdec_sync_kv before prepare")
    let kvd = g_rd.kv_dim
    let base = (l * g_rd.seq_cap) * kvd * 4l
    unsafe {
        upload_region_at(g_rd.k_mirror, base, reinterpret<uint8 const?>(kp), npos * kvd * 4l)
        upload_region_at(g_rd.v_mirror, base, reinterpret<uint8 const?>(vp), npos * kvd * 4l)
    }
}

//! Bulk hydration: read layer `l`'s mirror rows [0, npos) into the caller's f32 cache (kp/vp,
//! [npos x kvd] each), one submit per KV_READBACK_ROWS rows. The host-KV-authoritative rail —
//! makes continuations, CPU fallback, and slot switches valid after a device-resident prefill.
def vk_rdec_read_kv_bulk(l : int64; kp : float?; vp : float?; npos : int64) {
    assert(g_rd != null, "vk_rdec_read_kv_bulk before prepare")
    verify(vk_moe_init())
    let kvd = g_rd.kv_dim
    var p0 = 0l
    var raw = alloc_transient_cmd()   // re-recorded per chunk (the pool allows reset), freed below
    while (p0 < npos) {
        let rows = min(KV_READBACK_ROWS, npos - p0)
        let bytes = rows * kvd * 4l
        let off = ((l * g_rd.seq_cap) + p0) * kvd * 4l
        unsafe {
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            cmd_copy_range(raw, g_rd.k_mirror, off, g_rd.kv_readback.buf, 0l, bytes)
            cmd_copy_range(raw, g_rd.v_mirror, off, g_rd.kv_readback.buf, bytes, bytes)
            vk_check(vkEndCommandBuffer(raw), null)
            submit_wait(raw)
            memcpy(reinterpret<void?>(kp + p0 * kvd), g_rd.kv_readback.mapped, int(bytes))
            memcpy(reinterpret<void?>(vp + p0 * kvd), reinterpret<uint8?>(g_rd.kv_readback.mapped) + bytes, int(bytes))
        }
        p0 += rows
    }
    free_transient_cmd(raw)
}

//! Batch-decode writeback: read layer `l`'s new K/V row at `pos` from the mirror back into the
//! caller's f32 cache row (kp/vp, kvd each). Keeps the CPU cache current after a GPU decode step.
def vk_rdec_read_kv(l, pos : int64; kp : float?; vp : float?) {
    assert(g_rd != null, "vk_rdec_read_kv before prepare")
    let kvd = g_rd.kv_dim
    let off = ((l * g_rd.seq_cap) + pos) * kvd * 4l
    verify(vk_moe_init())
    unsafe {
        var raw = alloc_transient_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        cmd_copy_range(raw, g_rd.k_mirror, off, g_rd.kv_readback.buf, 0l, kvd * 4l)
        cmd_copy_range(raw, g_rd.v_mirror, off, g_rd.kv_readback.buf, kvd * 4l, kvd * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(reinterpret<void?>(kp), g_rd.kv_readback.mapped, int(kvd * 4l))
        memcpy(reinterpret<void?>(vp), reinterpret<float?>(g_rd.kv_readback.mapped) + kvd, int(kvd * 4l))
        free_transient_cmd(raw)
    }
}

//! Resident-driver prefill: `npos` embedded rows (x_batch, [npos x dim]) + per-position rope rows
//! (cos_batch, [npos x hs]) -> the last row's logits, filling the KV mirror at positions [0, npos).
def vk_rdec_prefill(x_batch : array<float>; cos_batch : array<float>; npos : int64; var logits : array<float>) {
    assert(g_rd != null && g_rd.ready, "vk_rdec_prefill before prepare/set_cls")
    pf_setup()
    let dim = g_rd.dim
    let qd = g_rd.qd
    let kvd = g_rd.kv_dim
    let hid = g_rd.hidden
    let nlfin = g_rd.n_layers * 2l * dim
    let pairs = qd / 2l + kvd / 2l
    unsafe {
        var sp = reinterpret<float?>(g_gpu.staging.mapped)
        sp[0] = g_rd.scale
        staged_upload(g_rd.scal_dev, 0l, 64l)
        sp[0] = g_rd.eps
        sp[1] = 1.0
        staged_upload(g_rd.ar_scal_dev, 0l, 64l)
        // prompts over the PF_WINDOW activation buffers run as sequential windows: each window's
        // rope/attention address the mirror at absolute positions, so window w attends everything
        // [0, w0 + row] the earlier windows stored. Only the last window runs fin_rq + cls.
        var w0 = 0l
        while (w0 < npos) {
            let tp0 = ref_time_ticks()
            let wlen = min(g_rd.pf_np, npos - w0)
            let last = w0 + wlen >= npos
            upload_region_at(g_rd.pf_x, 0l, addr<uint8 const?>(x_batch[int(w0 * dim)]), wlen * dim * 4l)
            upload_region_at(g_rd.pf_cos, 0l, addr<uint8 const?>(cos_batch[int(w0 * g_rd.head_size)]), wlen * g_rd.head_size * 4l)
            fill_ffn_meta(wlen * hid, wlen * hid / 32l, false)   // act(rq|f16) over the window's rows
            // fill metas
            let mA = @(idx : int; n, d, blk : int64; fmt : int) {
                fill_arena_batch_meta(g_rd.pf_meta[idx], n, d, blk, wlen, fmt)
            }
            let mQ = @(idx : int; blocks : int64) { fill_quant_meta(g_rd.pf_meta[idx], blocks) }
            let mC = @(idx : int; elems : int64) { fill_cvt_meta(g_rd.pf_meta[idx], elems) }
            let mR = @(idx : int; woff : int64; add : bool) { fill_addrms_meta(g_rd.pf_meta[idx], dim, woff, add) }
            for (l in range64(g_rd.n_layers)) {
                var L & = g_rd.layers[l]
                let b = int(l * PF_ROLES)
                if (L.fq == 6) {
                    invoke(mC, b + 0, wlen * dim)
                } else {
                    invoke(mQ, b + 0, wlen * dim / 32l)
                }
                invoke(mA, b + 1, dim, qd, L.bq, L.fq)
                invoke(mA, b + 2, dim, kvd, L.bk, L.fk)
                invoke(mA, b + 3, dim, kvd, L.bv, L.fv)
                if (g_rd.qk_norm) {
                    let hs = g_rd.head_size
                    var mn = reinterpret<uint?>(g_rd.pf_meta[b + 15].mapped)
                    mn[0] = uint(hs); mn[1] = uint(g_rd.n_heads); mn[2] = uint(kvd / hs)
                    mn[3] = uint(qd); mn[4] = uint(kvd); mn[5] = 0u   // k rows at 0 in pf_kv, kvd stride
                    mn[6] = uint(nlfin + dim + l * 2l * hs)
                    mn[7] = uint(nlfin + dim + (l * 2l + 1l) * hs)
                }
                var mr = reinterpret<uint?>(g_rd.pf_meta[b + 4].mapped)
                mr[0] = uint(qd); mr[1] = uint(kvd); mr[2] = uint(g_rd.head_size); mr[3] = uint(g_rd.head_size / 2l)
                mr[4] = g_rd.neox ? 1u : 0u; mr[5] = uint(qd / 2l); mr[6] = uint(pairs)
                mr[7] = uint(l * g_rd.seq_cap * kvd); mr[9] = uint(wlen); mr[10] = uint(w0); mr[11] = uint(wlen * kvd)
                var ma = reinterpret<uint?>(g_rd.pf_meta[b + 5].mapped)
                ma[0] = uint(wlen); ma[1] = uint(g_rd.n_heads); ma[2] = uint(kvd); ma[3] = uint(g_rd.head_size); ma[4] = uint(g_rd.kv_mul)
                ma[5] = uint(l * g_rd.seq_cap * kvd); ma[6] = uint(l * g_rd.seq_cap * kvd); ma[7] = uint(w0)
                if (L.fo == 6) {
                    invoke(mC, b + 6, wlen * qd)
                } else {
                    invoke(mQ, b + 6, wlen * qd / 32l)
                }
                invoke(mA, b + 7, qd, dim, L.bo, L.fo)
                invoke(mR, b + 8, (l * 2l + 1l) * dim, true)
                if (L.f1 == 6) {
                    invoke(mC, b + 9, wlen * dim)
                } else {
                    invoke(mQ, b + 9, wlen * dim / 32l)
                }
                invoke(mA, b + 10, dim, hid, L.b1, L.f1)
                invoke(mA, b + 11, dim, hid, L.b3, L.f3)
                invoke(mA, b + 13, hid, dim, L.b2, L.f2)
                let nxt = l + 1l < g_rd.n_layers ? (l + 1l) * 2l * dim : nlfin
                invoke(mR, b + 14, nxt, true)
            }
            let bp = int(g_rd.n_layers * PF_ROLES)
            invoke(mR, bp + 0, 0l, false)   // prologue norm
            if (last) {
                // final quantize reads the LAST row of pf_xb
                var mf = reinterpret<uint?>(g_rd.pf_meta[bp + 1].mapped)
                mf[10] = uint((wlen - 1l) * dim); mf[18] = uint(dim / 32l)
            }
            let t_prep = get_time_usec(tp0)
            // record
            var raw = g_rd.pf_cmd
            let rf : VkCommandBufferResetFlags
            vk_check(vkResetCommandBuffer(raw, rf), null)
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            pfq_begin(raw)
            pfq_ts(raw)   // q0: submit start
            var h : VkHaz
            vhz_dep(raw, h, 0u, VHZ_META, true)
            cmd_copy_whole(raw, g_gpu.ffn_meta.buf, g_gpu.ffn_meta_dev, FFN_META_BYTES)
            pfq_ts(raw)   // q1: meta copy
            let ropewg = uint((wlen * pairs + int64(WG_X) - 1l) / int64(WG_X))
            let fa128 = g_gpu.has_coopmat && g_rd.head_size == 128l && vk_fa_on()
            let attnwg = uint(fa128 ? g_rd.n_heads * ((wlen + 31l) / 32l) : g_rd.n_heads * ((wlen + 7l) / 8l))
            rd_dispatch(raw, g_gpu.ar_pipeline_b, g_rd.pf_sets[bp + 0], uint(wlen), h)   // prologue norm
            for (l in range64(g_rd.n_layers)) {
                let b = int(l * PF_ROLES)
                // fmt-6 (cm2) roles: f32->f16 convert feeds the decode-in-load twin; the fill's fmt
                // must match the dispatched pipe's tile rule, so both key off the same group flag
                let fq6 = g_rd.layers[l].fq == 6
                let fo6 = g_rd.layers[l].fo == 6
                let gu6 = g_rd.layers[l].f1 == 6
                let dn6 = g_rd.layers[l].f2 == 6
                rd_dispatch(raw, weak_copy(fq6 ? g_gpu.f16cvt_pipeline : g_gpu.dn_rq_pipeline), g_rd.pf_sets[b + 0], uint((wlen * dim / 4l + 255l) / 256l), h)
                rd_dispatch(raw, pf_gemm_pipe(fq6, qd, wlen), g_rd.pf_sets[b + 1], fill_arena_batch_meta(g_rd.pf_meta[b + 1], dim, qd, g_rd.layers[l].bq, wlen, fq6 ? 6 : 0), h)
                rd_dispatch(raw, pf_gemm_pipe(fq6, kvd, wlen), g_rd.pf_sets[b + 2], fill_arena_batch_meta(g_rd.pf_meta[b + 2], dim, kvd, g_rd.layers[l].bk, wlen, fq6 ? 6 : 0), h)
                rd_dispatch(raw, pf_gemm_pipe(fq6, kvd, wlen), g_rd.pf_sets[b + 3], fill_arena_batch_meta(g_rd.pf_meta[b + 3], dim, kvd, g_rd.layers[l].bv, wlen, fq6 ? 6 : 0), h)
                vhz_dep(raw, h, VHZ_VST, VHZ_KVV, true)
                cmd_copy_range(raw, g_rd.pf_v, 0l, g_rd.pf_kv, wlen * kvd * 4l, wlen * kvd * 4l)   // v into pf_kv's v half
                if (g_rd.qk_norm) {
                    rd_dispatch(raw, g_gpu.qkn_pipeline, g_rd.pf_sets[b + 15],
                        uint(wlen * (g_rd.n_heads + kvd / g_rd.head_size)), h)
                }
                rd_dispatch(raw, g_gpu.rks_b_pipeline, g_rd.pf_sets[b + 4], ropewg, h)
                rd_dispatch(raw, weak_copy(fa128 ? g_gpu.da_b128_pipeline : g_gpu.da_b_pipeline), g_rd.pf_sets[b + 5], attnwg, h)
                rd_dispatch(raw, weak_copy(fo6 ? g_gpu.f16cvt_pipeline : g_gpu.dn_rq_pipeline), g_rd.pf_sets[b + 6], uint((wlen * qd / 4l + 255l) / 256l), h)
                rd_dispatch(raw, pf_gemm_pipe(fo6, dim, wlen), g_rd.pf_sets[b + 7], fill_arena_batch_meta(g_rd.pf_meta[b + 7], qd, dim, g_rd.layers[l].bo, wlen, fo6 ? 6 : 0), h)
                rd_dispatch(raw, g_gpu.ar_pipeline_b, g_rd.pf_sets[b + 8], uint(wlen), h)
                rd_dispatch(raw, weak_copy(gu6 ? g_gpu.f16cvt_pipeline : g_gpu.dn_rq_pipeline), g_rd.pf_sets[b + 9], uint((wlen * dim / 4l + 255l) / 256l), h)
                rd_dispatch(raw, pf_gemm_pipe(gu6, hid, wlen), g_rd.pf_sets[b + 10], fill_arena_batch_meta(g_rd.pf_meta[b + 10], dim, hid, g_rd.layers[l].b1, wlen, gu6 ? 6 : 0), h)
                rd_dispatch(raw, pf_gemm_pipe(gu6, hid, wlen), g_rd.pf_sets[b + 11], fill_arena_batch_meta(g_rd.pf_meta[b + 11], dim, hid, g_rd.layers[l].b3, wlen, gu6 ? 6 : 0), h)
                rd_dispatch(raw, weak_copy(dn6 ? g_gpu.actf16_pipeline : g_gpu.actrq_pipeline), g_rd.pf_sets[b + 12], uint((wlen * hid / 4l + 255l) / 256l), h)
                rd_dispatch(raw, pf_gemm_pipe(dn6, dim, wlen), g_rd.pf_sets[b + 13], fill_arena_batch_meta(g_rd.pf_meta[b + 13], hid, dim, g_rd.layers[l].b2, wlen, dn6 ? 6 : 0), h)
                rd_dispatch(raw, g_gpu.ar_pipeline_b, g_rd.pf_sets[b + 14], uint(wlen), h)
            }
            if (last) {
                rd_dispatch(raw, g_gpu.dn_rq_pipeline, g_rd.pf_sets[bp + 1], uint((dim / 4l + 255l) / 256l), h)
                rd_dispatch_gemv(raw, g_rd.cls_fmt_marker, g_rd.cls_set, rd_gemv_wg(g_rd.vocab), h)
                vhz_dep(raw, h, VHZ_LOG, 0u, true)
                cmd_copy_whole(raw, g_rd.logits_dev, g_rd.pf_logits_host.buf, g_rd.vocab * 4l)
            }
            if (vk_prof()) {
                to_log(LOG_INFO, "vk_rdpf hz: {h.disp} nodes, {h.barriers} barriers\n")
            }
            pfq_end(raw)
            vk_check(vkEndCommandBuffer(raw), null)
            let t_record = get_time_usec(tp0) - t_prep
            submit_wait(raw)
            if (vk_prof()) {
                to_log(LOG_INFO, "vk_rdpf w0 {w0} rows {wlen} prep {t_prep} record {t_record} submit {get_time_usec(tp0) - t_prep - t_record}\n")
            }
            w0 += wlen
        }
        memcpy(addr<void?>(logits[0]), g_rd.pf_logits_host.mapped, int(g_rd.vocab * 4l))
        // per-role GPU aggregation (q0 start, q1 meta, q2 prologue, rpl/layer, final rq, cls), only when
        // every stamp landed (deep graphs overflow PFQ_CAP). Multi-window prompts: LAST window only.
        // Overlapped levels (q|k|v, gate|up) put the whole level's cost on the LAST member's delta.
        let rpl = g_rd.qk_norm ? 16l : 15l
        if (vk_prof() && g_pfq_n == 5u + uint(g_rd.n_layers * rpl)) {
            var role : double[16]
            for (l in range64(g_rd.n_layers)) {
                for (s in range(int(rpl))) {
                    role[s] += pfq_us(uint(3l + l * rpl) + uint(s))
                }
            }
            let nq = 3u + uint(g_rd.n_layers * rpl)
            var tot = 0.0lf
            for (s in range(int(rpl))) {
                tot += role[s]
            }
            tot += pfq_us(2u) + pfq_us(nq) + pfq_us(nq + 1u)
            // the qkn dispatch stamps between v and rope_kv, shifting every later role by one
            if (g_rd.qk_norm) {
                to_log(LOG_INFO, "vk_rdpf gpu prologue {int(pfq_us(2u))} rq_x {int(role[0])} q {int(role[1])} k {int(role[2])} v {int(role[3])} qkn {int(role[4])} rope_kv {int(role[5])} attn {int(role[6])} rq_a {int(role[7])} wo {int(role[8])} ar1 {int(role[9])} rq_f {int(role[10])} gate {int(role[11])} up {int(role[12])} act {int(role[13])} down {int(role[14])} ar2 {int(role[15])} fin_rq {int(pfq_us(nq))} cls {int(pfq_us(nq + 1u))} total {int(tot)}\n")
            } else {
                to_log(LOG_INFO, "vk_rdpf gpu prologue {int(pfq_us(2u))} rq_x {int(role[0])} q {int(role[1])} k {int(role[2])} v {int(role[3])} rope_kv {int(role[4])} attn {int(role[5])} rq_a {int(role[6])} wo {int(role[7])} ar1 {int(role[8])} rq_f {int(role[9])} gate {int(role[10])} up {int(role[11])} act {int(role[12])} down {int(role[13])} ar2 {int(role[14])} fin_rq {int(pfq_us(nq))} cls {int(pfq_us(nq + 1u))} total {int(tot)}\n")
            }
        }
    }
}

//! Arena seam: one dense FFN layer against arena-resident planes — gate GEMV, up GEMV, fused
//! act+requant, down GEMV, ONE submit. `blk1/blk3/blk2` are gate/up/down arena block indices.
//! Gate and up must share a format class (they share the activation image); down is free.
def vk_arena_ffn(var y : array<float>; blk1, blk3, blk2 : int64; n, nfe : int64;
                 f1, f3, f2 : int; xq : array<int8>; xs : array<float>; is_gelu : bool) {
    verify(vk_moe_init())
    assert(vk_fmt_b32(f3) == vk_fmt_b32(f1), "dasLLAMA vulkan arena: gate/up form mismatch — one activation image")
    assert(nfe * 4l <= Y_BYTES && n * 4l <= Y_BYTES, "dasLLAMA vulkan arena: ffn planes exceed y capacity")
    ensure_af_state(f1, f3, f2)
    let xunit = fmt_unit(f1)
    let dkq = !vk_fmt_b32(f2)
    fill_af_meta(g_gpu.af_meta_g, n, nfe, blk1)
    fill_af_meta(g_gpu.af_meta_u, n, nfe, blk3)
    fill_af_meta(g_gpu.af_meta_d, nfe, n, blk2)
    fill_ffn_meta(nfe, nfe / (dkq ? 256l : 32l), is_gelu)
    unsafe {
        memcpy(g_gpu.af_xq.mapped, addr<void? -const>(xq[0]), int(n))
        memcpy(g_gpu.af_xs.mapped, addr<void? -const>(xs[0]), int((n / xunit) * 4l))
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        vhz_dep(raw, h, 0u, VHG_META, true)
        cmd_copy_whole(raw, g_gpu.ffn_meta.buf, g_gpu.ffn_meta_dev, FFN_META_BYTES)
        let gwg = uint((nfe + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg)
        hz_dispatch(raw, h, gemv_pipe(f1), g_gpu.af_gate_set, gwg)
        hz_dispatch(raw, h, gemv_pipe(f3), g_gpu.af_up_set, gwg)
        // the requant form follows the DOWN plane — its GEMV consumes hq/hs
        hz_dispatch(raw, h, weak_copy(dkq ? g_gpu.actrq_k_pipeline : g_gpu.actrq_pipeline),
            g_gpu.af_actrq_set, uint((nfe / (dkq ? 8l : 4l) + 255l) / 256l))
        hz_dispatch(raw, h, gemv_pipe(f2), g_gpu.af_down_set, uint((n + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.af_yo_dev, g_gpu.af_yo_host.buf, n * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(y[0]), g_gpu.af_yo_host.mapped, int(n * 4l))
    }
}

// single-region batch-GEMM meta over an arena block: all tiles map to region 0. Returns the
// dispatch workgroup count. Mirrors fill_batch_meta for one region [0, npos).
def private fill_arena_batch_meta(hb : HostBuf; n, d, blk, npos : int64; fmt : int) : uint {
    let tile = fmt == 0 ? q8_batch_tile_for(d, npos) : BATCH_TILE
    // the cm2 twin tiles 64 weight-rows x 128 tokens per workgroup (fill_batch_meta's rule)
    let tile_d = fmt == 6 ? 64l : tile
    let tile_c = fmt == 6 ? 128l : tile
    let wtiles = (d + tile_d - 1l) / tile_d
    let rtiles = (npos + tile_c - 1l) / tile_c
    let total = rtiles * wtiles
    assert((8l + total) * 4l <= BATCH_META_BYTES, "dasLLAMA vulkan arena: batch meta exceeds capacity")
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = 1u          // one region
        mp[3] = 8u          // map_off = 4 + nrc*4
        mp[4] = uint(blk)   // arena block base (already in fmt units)
        mp[5] = 0u          // rb0 - r0
        mp[6] = uint(npos)  // cnt
        mp[7] = 0u          // wg base
        for (g in range64(total)) {
            mp[8l + g] = 0u
        }
    }
    return uint(total)
}

def private arena_ensure_batch(fmt : int) {
    unsafe {
        var a & = g_gpu.arenas[fmt]
        if (a.batch_ready) {
            return
        }
        ensure_batch_state()
        a.batch_meta = make_host_buf(BATCH_META_BYTES, true)
        a.batch_set = alloc_desc_set()
        var writes : array<WriteDescriptorSet>
        write_buf_desc(writes, a.batch_set, 0u, a.wqbuf, a.wqbytes)
        write_buf_desc(writes, a.batch_set, 1u, a.wsbuf, a.wsbytes)
        write_buf_desc(writes, a.batch_set, 2u, a.batch_meta.buf, BATCH_META_BYTES)
        write_buf_desc(writes, a.batch_set, 3u, g_gpu.batch_xq_dev, BATCH_XQ_BYTES)
        write_buf_desc(writes, a.batch_set, 4u, g_gpu.batch_xs_dev, BATCH_XS_BYTES)
        write_buf_desc(writes, a.batch_set, 5u, g_gpu.batch_y1_dev, BATCH_Y_BYTES)
        let no_copies : array<CopyDescriptorSet>
        update_descriptor_sets(g_gpu.device, writes, no_copies)
        hz_set_bits(a.batch_set, 0u, 0u, 0u, 0u, 0u, VHG_YO)   // meta/acts fill pre-cmd
        a.batch_ready = true
    }
}

//! Arena seam: batch GEMM of the [d x n] tensor at block `blk` against `npos` quantized activation
//! rows (xq/xs, [npos x n]) into y[0..npos*d). The prefill twin of vk_arena_gemv (q8 only for now).
def vk_arena_gemm(var y : array<float>; blk : int64; n, d, npos : int64; fmt : int; xq : array<int8>; xs : array<float>) {
    verify(vk_moe_init())
    assert(fmt == 0, "dasLLAMA vulkan arena_gemm: q8 only")
    assert(npos * n <= BATCH_XQ_BYTES && npos * d * 4l <= BATCH_Y_BYTES, "arena_gemm: batch exceeds scratch")
    arena_ensure_batch(fmt)
    let groups = fill_arena_batch_meta(g_gpu.arenas[fmt].batch_meta, n, d, blk, npos, fmt)
    unsafe {
        upload_region_at(g_gpu.batch_xq_dev, 0l, addr<uint8 const?>(xq[0]), npos * n)
        upload_region_at(g_gpu.batch_xs_dev, 0l, addr<uint8 const?>(xs[0]), npos * (n / 32l) * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        hz_dispatch(raw, h, q8_batch_pipe_for(d, npos), g_gpu.arenas[fmt].batch_set, groups)
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, npos * d * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(y[0]), g_gpu.batch_y1.mapped, int(npos * d * 4l))
    }
}

//! Arena seam: one GEMV of the [d x n] tensor at block index `blk` into y[0..d). Same kernels and
//! meta layout as the per-op tier — only the weight base is an arena index, not stack-relative.
def vk_arena_gemv(var y : array<float>; blk : int64; n, d : int64; fmt : int; xq : array<int8>; xs : array<float>) {
    assert(g_gpu != null && key_exists(g_gpu.arenas, fmt), "dasLLAMA vulkan arena: format not reserved")
    var yp = unsafe(addr(y[0]))
    let xqp = unsafe(addr(xq[0]))
    let xsp = unsafe(addr(xs[0]))
    arena_ensure_scratch(fmt)
    let unit = fmt_unit(fmt)   // q8 AND q8n take 32-block Q8_0 activations
    let nb = n / unit
    assert(n <= XQ_BYTES && nb * 4l <= XS_BYTES, "dasLLAMA vulkan arena: activations exceed scratch capacity")
    assert(d * 4l <= Y_BYTES, "dasLLAMA vulkan arena: gemv output exceeds y capacity")
    unsafe {
        var a & = g_gpu.arenas[fmt]
        var mp = reinterpret<uint?>(a.meta.mapped)
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = 1u          // one region: the whole tensor
        mp[3] = 0u          // y base row
        mp[4] = uint(blk)   // THE arena base — a stack-relative offset in the per-op tier
        mp[5] = 0u          // activation block base
        memcpy(a.xq.mapped, reinterpret<void? -const>(xqp), int(n))   // one int8 quant per weight
        memcpy(a.xs.mapped, reinterpret<void? -const>(xsp), int(nb * 4l))
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        hz_dispatch(raw, h, gemv_pipe(fmt), a.raw_set, uint((d + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, a.y_dev, a.y.buf, d * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(reinterpret<void?>(yp), a.y.mapped, int(d * 4l))
    }
}

// One-shot command buffers (the KV readback rails) are freed by their caller — registering them
// in model_cmds would grow it per hydration until the next model drop.
def private alloc_transient_cmd : VkCommandBuffer {
    var ai = VkCommandBufferAllocateInfo()
    ai.commandPool = boost_value_to_vk(g_gpu.pool)
    ai.level = VkCommandBufferLevel.PRIMARY
    ai.commandBufferCount = 1u
    var raw : VkCommandBuffer
    unsafe {
        vk_check(vkAllocateCommandBuffers(dev_raw(), ai, addr(raw)), null)
    }
    return raw
}

def private free_transient_cmd(var raw : VkCommandBuffer) {
    unsafe {
        vkFreeCommandBuffers(dev_raw(), boost_value_to_vk(g_gpu.pool), 1u, addr(raw))
    }
}

def private alloc_cmd : VkCommandBuffer {
    var raw = alloc_transient_cmd()
    g_gpu.model_cmds |> push(raw)   // all persistent cmds are model-owned; freed by the model drop
    return raw
}

def private cmd_bind_dispatch(raw : VkCommandBuffer; var set_ : VkDescriptorSet; groups : uint) {
    unsafe {
        vkCmdBindDescriptorSets(raw, VkPipelineBindPoint.COMPUTE, boost_value_to_vk(g_gpu.pipe_layout), 0u, 1u, addr(set_), 0u, null)
    }
    g_vk_dispatches++
    vkCmdDispatch(raw, groups, 1u, 1u)
}

// the GEMV pipeline for a stack's format (0 = the q8 kernel, 1/2/3/4 = kq_moe_gemv_k4/5/6/q40)
def private gemv_pipe(fmt : int) : Pipeline {
    if (fmt == 0) {
        return weak_copy(g_gpu.pipeline)
    } elif (fmt == 1) {
        return weak_copy(g_gpu.gemv_k4)
    } elif (fmt == 2) {
        return weak_copy(g_gpu.gemv_k5)
    } elif (fmt == 3) {
        return weak_copy(g_gpu.gemv_k6)
    } elif (fmt == 4) {
        return weak_copy(g_gpu.gemv_q40)
    } elif (fmt == 6) {
        return weak_copy(g_gpu.gemv_q8n)
    }
    panic("dasLLAMA vulkan tier: no GEMV kernel for stack format {fmt}")
    return weak_copy(g_gpu.pipeline)
}

// The Q8_0 prefill batch pipeline, switched to the tensor-core coopmat variant when DASLLAMA_COOPMAT
// selected one (weak alias — g_gpu owns). `tile` = the edge the caller's meta used; picks L vs M in mm mode.
def private q8_batch_pipe(tile : int64; aligned : bool = false) : Pipeline {
    if (g_gpu.coopmat_mode == 1) {
        return weak_copy(g_gpu.batch_coopmat_f16)
    } elif (g_gpu.coopmat_mode == 2) {
        return weak_copy(g_gpu.batch_coopmat_i8)
    } elif (g_gpu.coopmat_mode >= 3) {
        if (tile == BATCH_TILE) {   // the small tier IS the sdot4 kernel — measured, not a fallback
            return weak_copy(g_gpu.batch_pipeline)
        }
        if (tile == 64l) {
            if (!g_mm_m_logged) {
                g_mm_m_logged = true
                to_log(LOG_INFO, "dasLLAMA vulkan tier: mul_mm M-tile engaged\n")
            }
            return weak_copy(g_gpu.batch_coopmat_mm_m)
        }
        return weak_copy(aligned ? g_gpu.batch_coopmat_mm_a : g_gpu.batch_coopmat_mm)
    }
    return weak_copy(g_gpu.batch_pipeline)
}

def private batch_pipe(fmt : int; tile : int64; aligned : bool = false) : Pipeline {
    if (fmt == 0) {
        return q8_batch_pipe(tile, aligned)
    } elif (fmt == 1) {
        return weak_copy(g_gpu.batch_k4)
    } elif (fmt == 2) {
        return weak_copy(g_gpu.batch_k5)
    } elif (fmt == 3) {
        return weak_copy(g_gpu.batch_k6)
    } elif (fmt == 4) {
        // Q4_0 rides the f16 coopmat arm too (no s8-native for nibbles — mode 2 falls back to sdot4)
        return weak_copy(g_gpu.coopmat_mode == 1 ? g_gpu.batch_coopmat_q40 : g_gpu.batch_q40)
    } elif (fmt == 6) {
        return weak_copy(g_gpu.batch_cm2)
    }
    panic("dasLLAMA vulkan tier: no batch tile for stack format {fmt}")
    return weak_copy(g_gpu.batch_pipeline)
}

def private submit_nowait(var raw : VkCommandBuffer) {
    g_vk_submits++
    var submit = VkSubmitInfo()
    submit.commandBufferCount = 1u
    let fence = g_gpu.fence
    // a pending transfer-queue handoff attaches here — later in-order submits inherit the ordering
    var tss = VkTimelineSemaphoreSubmitInfo()
    var wval = g_gpu.pend_wait
    var sem = g_gpu.xfer_sem
    var wstage : VkPipelineStageFlags
    wstage.transfer = true
    wstage.compute_shader = true
    unsafe {
        submit.pCommandBuffers = addr(raw)
        if (wval > 0ul) {
            tss.waitSemaphoreValueCount = 1u
            tss.pWaitSemaphoreValues = addr(wval)
            submit.pNext = addr<void?>(tss)
            submit.waitSemaphoreCount = 1u
            submit.pWaitSemaphores = addr(sem)
            submit.pWaitDstStageMask = addr<uint?>(wstage)
            g_gpu.pend_wait = 0ul
        }
        vk_check(vkQueueSubmit(g_gpu.queue, 1u, addr(submit), fence), null)
    }
}

def private wait_fence {
    var fence = g_gpu.fence
    unsafe {
        // spin, never block: vkWaitForFences pays 1-2ms OS wake-up latency per submit (measured)
        while (vkGetFenceStatus(dev_raw(), fence) != VkResult.SUCCESS) {}
        vk_check(vkResetFences(dev_raw(), 1u, addr(fence)), null)
    }
}

def private submit_wait(var raw : VkCommandBuffer) {
    submit_nowait(raw)
    wait_fence()
}

// refresh a stack's mapped GEMV meta, one entry per ROW (cnt > 1 regions expand so output lands
// row-major): [n, d, nrows, ybase, (rel_wblock, xblock)*], block units per format (32 q8 / 256 kq).
def private fill_stack_meta_rows(si : int; offs : int64 const?; nregions : int64; n, d, nrows, xnb : int64; ybase : int64 = 0l) {
    assert((4l + nrows * 2l) * 4l <= META_BYTES, "dasLLAMA vulkan tier: FFN meta exceeds capacity")
    let unit = fmt_unit(g_gpu.stacks[si].fmt)
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.stacks[si].meta.mapped)
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = uint(nrows)
        mp[3] = uint(ybase)
        for (r in range64(nregions)) {
            let rel = offs[r * 3l] - g_gpu.stacks[si].base
            let row0 = offs[r * 3l + 1l]
            let cnt = offs[r * 3l + 2l]
            assert(rel % unit == 0l, "dasLLAMA vulkan tier: unaligned region offset")
            for (i in range64(cnt)) {
                mp[4l + (row0 + i) * 2l] = uint(rel / unit)
                mp[5l + (row0 + i) * 2l] = uint((row0 + i) * xnb)
            }
        }
    }
}

def private fill_stack_acts(si : int; xqp : int8 const?; xsp : float const?; xq_bytes : int64) {
    assert(xq_bytes <= XQ_BYTES, "dasLLAMA vulkan tier: dispatch exceeds scratch capacity")
    let unit = fmt_unit(g_gpu.stacks[si].fmt)
    unsafe {
        memcpy(g_gpu.stacks[si].xq.mapped, reinterpret<void? -const>(xqp), int(xq_bytes))
        memcpy(g_gpu.stacks[si].xs.mapped, reinterpret<void? -const>(xsp), int((xq_bytes / unit) * 4l))
    }
}

// ===== the FFN mid-chain state (shared by the decode-GEMV and batch arms) =====

def private ensure_ffn_state {
    if (g_gpu.ffn_ready) {
        return
    }
    g_gpu.hq_dev = make_device_buf(BATCH_HQ_BYTES)
    g_gpu.hs_dev = make_device_buf(BATCH_HS_BYTES)
    g_gpu.ffn_meta = make_host_buf(FFN_META_BYTES, false)   // transfer_src staging
    g_gpu.ffn_meta_dev = make_device_buf(FFN_META_BYTES)
    g_gpu.ffn_ready = true
}

def private fill_ffn_meta(elems, nblocks : int64; is_gelu : bool; comb_n : int64 = 0l; comb_k : int64 = 0l; comb_r0 : int64 = 0l; comb_r1 : int64 = 0l; xf_words : int64 = 0l) {
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.ffn_meta.mapped)
        mp[0] = uint(elems)
        mp[1] = is_gelu ? 1u : 0u
        mp[2] = uint(nblocks)
        mp[3] = uint(xf_words)   // cm2 chain: the dequant kernel's word count (q8 image / 4)
        mp[4] = uint(comb_n)
        mp[5] = uint(comb_k)
        mp[6] = uint(comb_r0)
        mp[7] = uint(comb_r1)
    }
}

// ===== prefill batch dispatch (the FFN's batched-GEMM arm) =====

def private ensure_batch_state {
    if (g_gpu.batch_ready) {
        return
    }
    ensure_ffn_state()
    g_gpu.batch_xq_dev = make_device_buf(BATCH_XQ_BYTES)
    g_gpu.batch_xs_dev = make_device_buf(BATCH_XS_BYTES)
    g_gpu.batch_y1_dev = make_device_buf(BATCH_Y_BYTES)
    g_gpu.batch_y2_dev = make_device_buf(BATCH_Y_BYTES)
    g_gpu.batch_y1 = make_host_buf(BATCH_Y_BYTES, true, [cached = true])
    g_gpu.batch_y2 = make_host_buf(BATCH_Y_BYTES, true, [cached = true])
    g_gpu.batch_cmd = alloc_cmd()
    g_gpu.batch_actrq_set = alloc_desc_set()
    var writes : array<WriteDescriptorSet>
    // fused act+requant bindings: up@0, params@2, hq/hs@3/4, gate@5; binding 1 is unused filler
    write_buf_desc(writes, g_gpu.batch_actrq_set, 0u, g_gpu.batch_y2_dev, BATCH_Y_BYTES)
    write_buf_desc(writes, g_gpu.batch_actrq_set, 1u, g_gpu.batch_xs_dev, BATCH_XS_BYTES)
    write_buf_desc(writes, g_gpu.batch_actrq_set, 2u, g_gpu.ffn_meta_dev, FFN_META_BYTES)
    write_buf_desc(writes, g_gpu.batch_actrq_set, 3u, g_gpu.hq_dev, BATCH_HQ_BYTES)
    write_buf_desc(writes, g_gpu.batch_actrq_set, 4u, g_gpu.hs_dev, BATCH_HS_BYTES)
    write_buf_desc(writes, g_gpu.batch_actrq_set, 5u, g_gpu.batch_y1_dev, BATCH_Y_BYTES)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    hz_set_bits(g_gpu.batch_actrq_set, VHB_Y2, 0u, VHB_META, VHB_HQ, VHB_HQ, VHB_Y1)
    g_gpu.resident_bytes += BATCH_XQ_BYTES + BATCH_XS_BYTES
    g_gpu.batch_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: prefill batch tier engaged\n")
}

// hazard bits for the cm2 chain's two f16 planes
let private VHB_XF = 0x8000u      // f16 gathered-activation plane (dequant out, gate/up twin B)
let private VHB_HF = 0x10000u     // f16 hidden plane (actf16 out, down twin B)

// cm2 batch state: the two f16 planes (2 bytes per element — 2x the q8 image caps bound the
// same chunk rows) and the dequant/act convert sets. Mode 4 + fmt-6 stacks only.
def private ensure_cm2_batch {
    if (g_gpu.cm2_batch_ready) {
        return
    }
    ensure_batch_state()
    g_gpu.batch_xf_dev = make_device_buf(2l * BATCH_XQ_BYTES)
    g_gpu.batch_hf_dev = make_device_buf(2l * BATCH_HQ_BYTES)
    g_gpu.batch_xf_set = alloc_desc_set()
    g_gpu.batch_hf_set = alloc_desc_set()
    var writes : array<WriteDescriptorSet>
    // dequant bindings: f16 out@0, params@2, xq/xs@3/4; 1/5 unused filler
    write_buf_desc(writes, g_gpu.batch_xf_set, 0u, g_gpu.batch_xf_dev, 2l * BATCH_XQ_BYTES)
    write_buf_desc(writes, g_gpu.batch_xf_set, 1u, g_gpu.batch_xs_dev, BATCH_XS_BYTES)
    write_buf_desc(writes, g_gpu.batch_xf_set, 2u, g_gpu.ffn_meta_dev, FFN_META_BYTES)
    write_buf_desc(writes, g_gpu.batch_xf_set, 3u, g_gpu.batch_xq_dev, BATCH_XQ_BYTES)
    write_buf_desc(writes, g_gpu.batch_xf_set, 4u, g_gpu.batch_xs_dev, BATCH_XS_BYTES)
    write_buf_desc(writes, g_gpu.batch_xf_set, 5u, g_gpu.batch_y1_dev, BATCH_Y_BYTES)
    // act-f16 bindings mirror actrq: up@0, params@2, f16 hidden out@3, gate@5; 1/4 unused filler
    write_buf_desc(writes, g_gpu.batch_hf_set, 0u, g_gpu.batch_y2_dev, BATCH_Y_BYTES)
    write_buf_desc(writes, g_gpu.batch_hf_set, 1u, g_gpu.batch_xs_dev, BATCH_XS_BYTES)
    write_buf_desc(writes, g_gpu.batch_hf_set, 2u, g_gpu.ffn_meta_dev, FFN_META_BYTES)
    write_buf_desc(writes, g_gpu.batch_hf_set, 3u, g_gpu.batch_hf_dev, 2l * BATCH_HQ_BYTES)
    write_buf_desc(writes, g_gpu.batch_hf_set, 4u, g_gpu.hs_dev, BATCH_HS_BYTES)
    write_buf_desc(writes, g_gpu.batch_hf_set, 5u, g_gpu.batch_y1_dev, BATCH_Y_BYTES)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    hz_set_bits(g_gpu.batch_xf_set, VHB_XF, 0u, VHB_META, VHB_ACT, VHB_ACT, 0u)
    hz_set_bits(g_gpu.batch_hf_set, VHB_Y2, 0u, VHB_META, VHB_HF, 0u, VHB_Y1)
    g_gpu.resident_bytes += 2l * BATCH_XQ_BYTES + 2l * BATCH_HQ_BYTES
    g_gpu.cm2_batch_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: cm2 prefill chain engaged (decode-in-load, f16 activations)\n")
}

def private ensure_comb_state {
    if (g_gpu.comb_ready) {
        return
    }
    g_gpu.comb_stage = make_host_buf(2l * COMB_WI_BYTES, false)   // transfer_src staging: w then inv
    g_gpu.comb_w_dev = make_device_buf(COMB_WI_BYTES)
    g_gpu.comb_inv_dev = make_device_buf(COMB_WI_BYTES)
    g_gpu.comb_y_dev = make_device_buf(BATCH_Y_BYTES)
    g_gpu.comb_y = make_host_buf(BATCH_Y_BYTES, true, [cached = true])
    g_gpu.comb_set = alloc_desc_set()
    var writes : array<WriteDescriptorSet>
    // combine bindings: output@0, params@2, bucket map@3, weights@4, down rows@5; binding 1 unused
    write_buf_desc(writes, g_gpu.comb_set, 0u, g_gpu.comb_y_dev, BATCH_Y_BYTES)
    write_buf_desc(writes, g_gpu.comb_set, 1u, g_gpu.comb_w_dev, COMB_WI_BYTES)
    write_buf_desc(writes, g_gpu.comb_set, 2u, g_gpu.ffn_meta_dev, FFN_META_BYTES)
    write_buf_desc(writes, g_gpu.comb_set, 3u, g_gpu.comb_inv_dev, COMB_WI_BYTES)
    write_buf_desc(writes, g_gpu.comb_set, 4u, g_gpu.comb_w_dev, COMB_WI_BYTES)
    write_buf_desc(writes, g_gpu.comb_set, 5u, g_gpu.batch_y1_dev, BATCH_Y_BYTES)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    hz_set_bits(g_gpu.comb_set, VHB_CY, VHB_COMB, VHB_META, VHB_COMB, VHB_COMB, VHB_Y1)
    g_gpu.comb_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: device-side moe combine engaged\n")
}

// batch bindings differ from decode's: meta is the stack's BATCH table, activations point at a
// role-fixed image (gate/up: the gathered x scratch; down: the requantized hidden planes), y one
// of the two shared device planes (gate/down write y1_dev, up y2_dev)
def private ensure_batch_set(si : int; ybuf, xqbuf, xsbuf : uint64; xqbytes, xsbytes : int64; xbit, ybit : uint; ybytes : int64 = BATCH_Y_BYTES) {
    if (g_gpu.stacks[si].batch_made && g_gpu.stacks[si].batch_y == ybuf && g_gpu.stacks[si].batch_xq == xqbuf) {
        return
    }
    if (!g_gpu.stacks[si].batch_made) {
        g_gpu.stacks[si].batch_meta = make_host_buf(BATCH_META_BYTES, false)   // transfer_src staging
        g_gpu.stacks[si].batch_meta_dev = make_device_buf(BATCH_META_BYTES)
        g_gpu.stacks[si].batch_set = alloc_desc_set()
        g_gpu.stacks[si].batch_made = true
    }
    var writes : array<WriteDescriptorSet>
    write_buf_desc(writes, g_gpu.stacks[si].batch_set, 0u, g_gpu.stacks[si].wqbuf, g_gpu.stacks[si].wqbytes)
    write_buf_desc(writes, g_gpu.stacks[si].batch_set, 1u, g_gpu.stacks[si].wsbuf, ws_range(si))
    write_buf_desc(writes, g_gpu.stacks[si].batch_set, 2u, g_gpu.stacks[si].batch_meta_dev, BATCH_META_BYTES)
    write_buf_desc(writes, g_gpu.stacks[si].batch_set, 3u, xqbuf, xqbytes)
    write_buf_desc(writes, g_gpu.stacks[si].batch_set, 4u, xsbuf, xsbytes)
    write_buf_desc(writes, g_gpu.stacks[si].batch_set, 5u, ybuf, ybytes)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    hz_set_bits(g_gpu.stacks[si].batch_set, 0u, 0u, VHB_META, xbit, xbit, ybit)
    g_gpu.stacks[si].batch_y = ybuf
    g_gpu.stacks[si].batch_xq = xqbuf
}

// chunk row cap: the gathered x window, both hidden planes, the requant planes, and the down
// output (which reuses y1) must all fit their buffers. Two activation units: `in` = gate/up
// image, `mid` = intermediate (follows the DOWN stack, may differ). 32 for Q8_0, 256 for Q8_K.
def private ffn_chunk_rows(n, nfe, xunit_in, xunit_mid : int64) : int64 {
    var r = min(BATCH_XQ_BYTES / n, BATCH_XS_BYTES / ((n / xunit_in) * 4l))
    r = min(r, BATCH_Y_BYTES / (max(nfe, n) * 4l))
    r = min(r, min(BATCH_HQ_BYTES / nfe, BATCH_HS_BYTES / ((nfe / xunit_mid) * 4l)))
    // tile-aligned so a partial last tile never reads past a buffer-derived cap (cmi8 loads unstaged)
    return min(r, BATCH_CHUNK_ROWS) / BATCH_TILE * BATCH_TILE
}

// parallel host memcpy — the batch chain moves 8-34MB per hop and a single lane runs ~14GB/s
// while the box's DRAM feeds 3-4x that; jobque lanes are idle while the GPU owns the layer
let private PAR_COPY_CHUNK = 2_097_152l

def private par_memcpy(dst : void?; src : void?; bytes : int64) {
    if (bytes < PAR_COPY_CHUNK * 2l || !is_job_que_available()) {
        unsafe {
            memcpy(dst, src, int(bytes))
        }
        return
    }
    let nch = int((bytes + PAR_COPY_CHUNK - 1l) / PAR_COPY_CHUNK)
    var d8 = unsafe(reinterpret<uint8?>(dst))
    var s8 = unsafe(reinterpret<uint8?>(src))
    maybe_parallel_for(0, nch, nch) $(cb, ce) {
        unsafe {
            for (c in range(cb, ce)) {
                let off = int64(c) * PAR_COPY_CHUNK
                memcpy(reinterpret<void?>(d8 + off), reinterpret<void?>(s8 + off), int(min(PAR_COPY_CHUNK, bytes - off)))
            }
        }
    }
}

// stage the chunk's activation rows [r0, r0+rows) into the staging window (xq at 0, xs at
// BATCH_XQ_BYTES); the staging->device copies ride the HEAD of the chain submit — a separate
// synchronous copy submit here measured a full extra round trip per chunk
def private upload_batch_acts(xqp : int8 const?; xsp : float const?; r0, rows, n, xunit : int64) {
    let xq_bytes = rows * n
    let xs_bytes = rows * (n / xunit) * 4l
    unsafe {
        var sp = reinterpret<uint8?>(g_gpu.staging.mapped)
        par_memcpy(g_gpu.staging.mapped, reinterpret<void? -const>(xqp + r0 * n), xq_bytes)
        par_memcpy(reinterpret<void?>(sp + BATCH_XQ_BYTES), reinterpret<void? -const>(xsp + r0 * (n / xunit)), xs_bytes)
    }
}

// the chain-head acts copies the fold above defers to record time
def private cmd_copy_batch_acts(raw : VkCommandBuffer; xq_bytes, xs_bytes : int64) {
    var qregions : array<BufferCopy>
    qregions |> push(BufferCopy(srcOffset = 0ul, dstOffset = 0ul, size = uint64(xq_bytes)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(g_gpu.staging.buf), nonowning_buf(g_gpu.batch_xq_dev), qregions)
    var sregions : array<BufferCopy>
    sregions |> push(BufferCopy(srcOffset = uint64(BATCH_XQ_BYTES), dstOffset = 0ul, size = uint64(xs_bytes)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(g_gpu.staging.buf), nonowning_buf(g_gpu.batch_xs_dev), sregions)
}

// build the chunk's meta table: regions clipped to the row window [r0, r1) (row0 re-based to the
// window), then the per-workgroup region map. Weight offsets follow the stack's format units —
// 32-weight blocks for q8, superblocks for kq. Returns the workgroup count.
def private fill_batch_meta(si : int; offs : int64 const?; nregions : int64; n, d : int64; r0, r1 : int64) : uint {
    let fmt = g_gpu.stacks[si].fmt
    let unit = fmt_unit(fmt)
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.stacks[si].batch_meta.mapped)
        var nrc = 0l
        var nrows = 0l
        var all128 = true   // every active region's cnt a 128-multiple -> aligned-L eligible
        for (r in range64(nregions)) {
            let b0 = max(offs[r * 3l + 1l], r0)
            let b1 = min(offs[r * 3l + 1l] + offs[r * 3l + 2l], r1)
            if (b1 > b0) {
                nrc++
                nrows += b1 - b0
                all128 = all128 && (b1 - b0) % 128l == 0l
            }
        }
        // route the tile on the AVERAGE rows per active region; the cm2 twin tiles 64w x 128t
        let tile = fmt == 0 ? q8_batch_tile_for(d, nrc > 0l ? nrows / nrc : r1 - r0) : BATCH_TILE
        let tile_d = fmt == 6 ? 64l : tile
        let tile_c = fmt == 6 ? 128l : tile
        g_gpu.stacks[si].batch_tile = tile
        g_gpu.stacks[si].batch_aligned = fmt == 0 && tile == 128l && d % 128l == 0l && all128
        let wtiles = (d + tile_d - 1l) / tile_d
        let wg_bound = ((r1 - r0 + tile_c - 1l) / tile_c + nrc) * wtiles
        assert((4l + nrc * 4l + wg_bound) * 4l <= BATCH_META_BYTES, "dasLLAMA vulkan tier: batch meta exceeds capacity")
        let map_off = 4l + nrc * 4l
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = uint(nrc)
        mp[3] = uint(map_off)
        var ri = 0l
        var wg = 0l
        for (r in range64(nregions)) {
            let rb0 = max(offs[r * 3l + 1l], r0)
            let rb1 = min(offs[r * 3l + 1l] + offs[r * 3l + 2l], r1)
            if (rb1 <= rb0) {
                continue
            }
            let rel = offs[r * 3l] - g_gpu.stacks[si].base
            assert(rel % unit == 0l, "dasLLAMA vulkan tier: unaligned batch region weight offset")
            let mb = 4l + ri * 4l
            mp[mb] = uint(rel / unit)
            mp[mb + 1l] = uint(rb0 - r0)
            mp[mb + 2l] = uint(rb1 - rb0)
            mp[mb + 3l] = uint(wg)
            let rwg = ((rb1 - rb0 + tile_c - 1l) / tile_c) * wtiles
            for (g in range64(rwg)) {
                mp[map_off + wg + g] = uint(ri)
            }
            wg += rwg
            ri++
        }
        return uint(wg)
    }
}

def private cmd_copy_batch_meta(raw : VkCommandBuffer; si : int) {
    var regions : array<BufferCopy>
    regions |> push(BufferCopy(srcOffset = 0ul, dstOffset = 0ul, size = uint64(BATCH_META_BYTES)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(g_gpu.stacks[si].batch_meta.buf),
                    nonowning_buf(g_gpu.stacks[si].batch_meta_dev), regions)
}

def private cmd_copy_whole(raw : VkCommandBuffer; src, dst : uint64; bytes : int64) {
    var regions : array<BufferCopy>
    regions |> push(BufferCopy(srcOffset = 0ul, dstOffset = 0ul, size = uint64(bytes)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(src), nonowning_buf(dst), regions)
}

def private cmd_copy_at(raw : VkCommandBuffer; src : uint64; src_off : int64; dst : uint64; bytes : int64) {
    var regions : array<BufferCopy>
    regions |> push(BufferCopy(srcOffset = uint64(src_off), dstOffset = 0ul, size = uint64(bytes)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(src), nonowning_buf(dst), regions)
}

// one reusable cmd buffer, re-recorded per submit — prefill submits are few and multi-ms, so
// recording cost is noise. comb_npos > 0: down rows stay device-side, accumulating into comb_y_dev;
// only the last chunk appends the k-fold-smaller combined DMA. Hidden planes never leave the device.
def private record_ffn_batch_cmd(s1, s3, s2 : int; nwg_gu, nwg_dn : uint; rows, nfe, gout_bytes : int64; axq_bytes, axs_bytes : int64;
                                 comb_npos : uint = 0u; comb_first : bool = false; comb_last : bool = false;
                                 comb_wi_bytes : int64 = 0l; comb_y_bytes : int64 = 0l) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    // one declaration per same-region copy GROUP — per-copy would fake WAW on the shared bit
    vhz_dep(raw, h, 0u, VHB_ACT, true)
    cmd_copy_batch_acts(raw, axq_bytes, axs_bytes)
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, s1)
    cmd_copy_batch_meta(raw, s3)
    cmd_copy_batch_meta(raw, s2)
    cmd_copy_whole(raw, g_gpu.ffn_meta.buf, g_gpu.ffn_meta_dev, FFN_META_BYTES)
    if (comb_npos > 0u && comb_first) {
        vhz_dep(raw, h, 0u, VHB_COMB, true)
        cmd_copy_whole(raw, g_gpu.comb_stage.buf, g_gpu.comb_w_dev, comb_wi_bytes)
        cmd_copy_at(raw, g_gpu.comb_stage.buf, COMB_WI_BYTES, g_gpu.comb_inv_dev, comb_wi_bytes)
    }
    if (g_gpu.stacks[s1].fmt == 6) {   // the cm2 chain: dequant feed, twin GEMMs, f16 mid-step
        hz_dispatch(raw, h, weak_copy(g_gpu.xf16_pipeline), g_gpu.batch_xf_set,
            uint((axq_bytes / 4l + 255l) / 256l))
    }
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s1].fmt, g_gpu.stacks[s1].batch_tile, g_gpu.stacks[s1].batch_aligned),
        g_gpu.stacks[s1].batch_set, nwg_gu)
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s3].fmt, g_gpu.stacks[s3].batch_tile, g_gpu.stacks[s3].batch_aligned),
        g_gpu.stacks[s3].batch_set, nwg_gu)
    // the mid-step form follows the DOWN stack — its GEMM is the consumer of the hidden plane
    if (g_gpu.stacks[s2].fmt == 6) {
        hz_dispatch(raw, h, weak_copy(g_gpu.actf16_pipeline), g_gpu.batch_hf_set,
            uint((rows * nfe / 4l + 255l) / 256l))
    } else {
        let dkq = g_gpu.stacks[s2].fmt != 0
        hz_dispatch(raw, h, weak_copy(dkq ? g_gpu.actrq_k_pipeline : g_gpu.actrq_pipeline),
            g_gpu.batch_actrq_set, uint((rows * nfe / (dkq ? 8l : 4l) + 255l) / 256l))
    }
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s2].fmt, g_gpu.stacks[s2].batch_tile, g_gpu.stacks[s2].batch_aligned),
        g_gpu.stacks[s2].batch_set, nwg_dn)
    if (comb_npos > 0u) {
        hz_dispatch(raw, h, weak_copy(g_gpu.comb_pipeline), g_gpu.comb_set, comb_npos)
        if (comb_last) {
            vhz_dep(raw, h, VHB_CY, 0u, true)
            cmd_copy_whole(raw, g_gpu.comb_y_dev, g_gpu.comb_y.buf, comb_y_bytes)
        }
    } else {
        vhz_dep(raw, h, VHB_Y1, 0u, true)
        cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, gout_bytes)
    }
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_ffnb hz: {h.disp} nodes, {h.barriers} barriers\n")
    }
    vk_check(vkEndCommandBuffer(raw), null)
}

// batch arm: chunked over row windows, down GEMM reuses y1 for output. npos > 0: the combine
// kernel folds each chunk's down rows into comb_y_dev, read back once after the last chunk.
def private vk_moe_ffn_batch(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    let s1 = find_stack(unsafe(offs1[0]), f1)
    let s3 = find_stack(unsafe(offs3[0]), f3)
    let s2 = find_stack(unsafe(offs2[0]), f2)
    // gate/up share one activation image (forms must agree); down's intermediate is its own unit
    let kq_in = !vk_fmt_b32(f1)
    assert(!vk_fmt_b32(f3) == kq_in,
        "dasLLAMA vulkan tier: gate/up form mismatch — they share one activation image")
    let xunit_in = fmt_unit(f1)
    let xunit_mid = fmt_unit(f2)
    ensure_batch_state()
    if (f1 == 6 || f2 == 6) {
        ensure_cm2_batch()   // any twin in the chain needs the f16 planes + convert sets
    }
    // a q8n gate/up pair loads the dequantized f16 plane as B; a q8n down loads the f16 hidden
    if (f1 == 6) {
        ensure_batch_set(s1, g_gpu.batch_y1_dev, g_gpu.batch_xf_dev, g_gpu.batch_xs_dev, 2l * BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_XF, VHB_Y1)
        ensure_batch_set(s3, g_gpu.batch_y2_dev, g_gpu.batch_xf_dev, g_gpu.batch_xs_dev, 2l * BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_XF, VHB_Y2)
    } else {
        ensure_batch_set(s1, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
        ensure_batch_set(s3, g_gpu.batch_y2_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y2)
    }
    if (f2 == 6) {
        ensure_batch_set(s2, g_gpu.batch_y1_dev, g_gpu.batch_hf_dev, g_gpu.hs_dev, 2l * BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HF, VHB_Y1)
    } else {
        ensure_batch_set(s2, g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.hs_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HQ, VHB_Y1)
    }
    let comb = npos > 0l
    var comb_k = 0l
    if (comb) {
        ensure_comb_state()
        comb_k = nrows / npos
        assert(comb_k * npos == nrows, "dasLLAMA vulkan tier: combined rows must be npos x k")
        assert(nrows * 4l <= COMB_WI_BYTES && npos * n * 4l <= BATCH_Y_BYTES,
            "dasLLAMA vulkan tier: combined output exceeds capacity")
        unsafe {
            var cp = reinterpret<uint8?>(g_gpu.comb_stage.mapped)
            par_memcpy(g_gpu.comb_stage.mapped, reinterpret<void? -const>(wp), nrows * 4l)
            par_memcpy(reinterpret<void?>(cp + COMB_WI_BYTES), reinterpret<void? -const>(invp), nrows * 4l)
        }
    }
    let chunk = ffn_chunk_rows(n, nfe, xunit_in, xunit_mid)
    var r0 = 0l
    while (r0 < nrows) {
        let r1 = min(r0 + chunk, nrows)
        let rows = r1 - r0
        let ts0 = ref_time_ticks()
        let nwg1 = fill_batch_meta(s1, offs1, nregions, n, nfe, r0, r1)
        let nwg3 = fill_batch_meta(s3, offs3, nregions, n, nfe, r0, r1)
        assert(nwg1 == nwg3, "dasLLAMA vulkan tier: fused batch pair region mismatch")
        let nwg2 = fill_batch_meta(s2, offs2, nregions, nfe, n, r0, r1)
        let ts1 = ref_time_ticks()
        upload_batch_acts(xqp, xsp, r0, rows, n, xunit_in)
        let ts2 = ref_time_ticks()
        fill_ffn_meta(rows * nfe, rows * nfe / xunit_mid, is_gelu, comb ? n : 0l, comb_k, r0, r1,
            f1 == 6 ? rows * n / 4l : 0l)
        record_ffn_batch_cmd(s1, s3, s2, nwg1, nwg2, rows, nfe, rows * n * 4l, rows * n, rows * (n / xunit_in) * 4l,
            [comb_npos = comb ? uint(npos) : 0u, comb_first = r0 == 0l, comb_last = r1 == nrows,
             comb_wi_bytes = nrows * 4l, comb_y_bytes = npos * n * 4l])
        let ts3 = ref_time_ticks()
        submit_wait(g_gpu.batch_cmd)
        let ts4 = ref_time_ticks()
        if (!comb) {
            unsafe {
                par_memcpy(reinterpret<void?>(goutp + r0 * n), g_gpu.batch_y1.mapped, rows * n * 4l)
            }
        }
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_ffn_batch acts {get_time_usec(ts1) - get_time_usec(ts2)} submit {get_time_usec(ts3) - get_time_usec(ts4)} copy {get_time_usec(ts4)}\n")
        }
        r0 = r1
    }
    if (comb) {
        unsafe {
            par_memcpy(reinterpret<void?>(goutp), g_gpu.comb_y.mapped, npos * n * 4l)
        }
    }
}

// one dense chunk: acts + meta staging copies, one tile GEMM over the single whole-plane
// region, y DMA
def private record_dense_cmd(si : int; nwg : uint; y_bytes : int64; axq_bytes, axs_bytes : int64) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    vhz_dep(raw, h, 0u, VHB_ACT, true)
    cmd_copy_batch_acts(raw, axq_bytes, axs_bytes)
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, si)
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[si].fmt, g_gpu.stacks[si].batch_tile, g_gpu.stacks[si].batch_aligned),
        g_gpu.stacks[si].batch_set, nwg)
    vhz_dep(raw, h, VHB_Y1, 0u, true)
    cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, y_bytes)
    vk_check(vkEndCommandBuffer(raw), null)
}

//! MoeGpuDenseFn: dense batched GEMM — y [nrows x d] = X · Wᵀ over the resident plane
//! ([d x n] at element offset woff), a one-region ride on the batch tile rail. Chunked like
//! the FFN arm; activations arrive pre-quantized in the plane's own form.
def vk_moe_dense(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d, nrows : int64; fmt : int) {
    let si = find_stack(woff, fmt)
    let xunit = fmt_unit(fmt)
    ensure_batch_state()
    ensure_batch_set(si, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
    var chunk = min(BATCH_XQ_BYTES / n, BATCH_XS_BYTES / ((n / xunit) * 4l))
    chunk = min(chunk, BATCH_Y_BYTES / (d * 4l))
    chunk = min(chunk, BATCH_CHUNK_ROWS) / BATCH_TILE * BATCH_TILE
    var region = fixed_array(woff, 0l, nrows)
    var r0 = 0l
    while (r0 < nrows) {
        let r1 = min(r0 + chunk, nrows)
        let rows = r1 - r0
        let ts0 = ref_time_ticks()
        var nwg = 0u
        unsafe {
            nwg = fill_batch_meta(si, addr(region[0]), 1l, n, d, r0, r1)
        }
        let ts1 = ref_time_ticks()
        upload_batch_acts(xqp, xsp, r0, rows, n, xunit)
        let ts2 = ref_time_ticks()
        record_dense_cmd(si, nwg, rows * d * 4l, rows * n, rows * (n / xunit) * 4l)
        let ts3 = ref_time_ticks()
        submit_wait(g_gpu.batch_cmd)
        let ts4 = ref_time_ticks()
        unsafe {
            par_memcpy(reinterpret<void?>(yp + r0 * d), g_gpu.batch_y1.mapped, rows * d * 4l)
        }
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_dense acts {get_time_usec(ts1) - get_time_usec(ts2)} submit {get_time_usec(ts3) - get_time_usec(ts4)} copy {get_time_usec(ts4)}\n")
        }
        r0 = r1
    }
}

// ===== deltanet dispatch (the recurrent-layer attention chain) =====

def private cmd_copy_range(raw : VkCommandBuffer; src : uint64; src_off : int64; dst : uint64; dst_off : int64; bytes : int64) {
    var regions : array<BufferCopy>
    regions |> push(BufferCopy(srcOffset = uint64(src_off), dstOffset = uint64(dst_off), size = uint64(bytes)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(src), nonowning_buf(dst), regions)
}

def private ensure_dn_state {
    if (g_gpu.dn_ready) {
        return
    }
    ensure_batch_state()
    g_gpu.dn_meta = make_host_buf(DN_META_BYTES, false)
    g_gpu.dn_meta_dev = make_device_buf(DN_META_BYTES)
    g_gpu.dn_smalls = make_host_buf(DN_SMALLS_BYTES, false)
    g_gpu.dn_smalls_dev = make_device_buf(DN_SMALLS_BYTES)
    g_gpu.dn_state_stage = make_host_buf(DN_STATE_BYTES, false)
    g_gpu.dn_state_host = make_host_buf(DN_STATE_BYTES, true, [cached = true])
    g_gpu.dn_tail_host = make_host_buf(DN_TAIL_BYTES, true, [cached = true])
    g_gpu.dn_state_dev = make_device_buf(DN_STATE_BYTES)
    g_gpu.dn_ws_dev = make_device_buf(DN_WS_BYTES)
    g_gpu.dn_conv_set = alloc_desc_set()
    g_gpu.dn_scan_set = alloc_desc_set()
    g_gpu.dn_rq_set = alloc_desc_set()
    var writes : array<WriteDescriptorSet>
    // conv bindings: qkv@5, conv output@0 (vk_upf view of hq_dev), smalls@4, params@2; 1/3 fillers
    write_buf_desc(writes, g_gpu.dn_conv_set, 0u, g_gpu.hq_dev, BATCH_HQ_BYTES)
    write_buf_desc(writes, g_gpu.dn_conv_set, 1u, g_gpu.dn_meta_dev, DN_META_BYTES)
    write_buf_desc(writes, g_gpu.dn_conv_set, 2u, g_gpu.dn_meta_dev, DN_META_BYTES)
    write_buf_desc(writes, g_gpu.dn_conv_set, 3u, g_gpu.dn_meta_dev, DN_META_BYTES)
    write_buf_desc(writes, g_gpu.dn_conv_set, 4u, g_gpu.dn_smalls_dev, DN_SMALLS_BYTES)
    write_buf_desc(writes, g_gpu.dn_conv_set, 5u, g_gpu.batch_y1_dev, BATCH_Y_BYTES)
    // scan bindings: conv rows@0, state@1, slabs/o@3, smalls@4, z rows@5, params@2
    write_buf_desc(writes, g_gpu.dn_scan_set, 0u, g_gpu.hq_dev, BATCH_HQ_BYTES)
    write_buf_desc(writes, g_gpu.dn_scan_set, 1u, g_gpu.dn_state_dev, DN_STATE_BYTES)
    write_buf_desc(writes, g_gpu.dn_scan_set, 2u, g_gpu.dn_meta_dev, DN_META_BYTES)
    write_buf_desc(writes, g_gpu.dn_scan_set, 3u, g_gpu.dn_ws_dev, DN_WS_BYTES)
    write_buf_desc(writes, g_gpu.dn_scan_set, 4u, g_gpu.dn_smalls_dev, DN_SMALLS_BYTES)
    write_buf_desc(writes, g_gpu.dn_scan_set, 5u, g_gpu.batch_y2_dev, BATCH_Y_BYTES)
    // requant bindings: o rows@0, quant words@3, scales@4 (out-proj activation image), params@2
    write_buf_desc(writes, g_gpu.dn_rq_set, 0u, g_gpu.dn_ws_dev, DN_WS_BYTES)
    write_buf_desc(writes, g_gpu.dn_rq_set, 1u, g_gpu.dn_meta_dev, DN_META_BYTES)
    write_buf_desc(writes, g_gpu.dn_rq_set, 2u, g_gpu.dn_meta_dev, DN_META_BYTES)
    write_buf_desc(writes, g_gpu.dn_rq_set, 3u, g_gpu.hq_dev, BATCH_HQ_BYTES)
    write_buf_desc(writes, g_gpu.dn_rq_set, 4u, g_gpu.hs_dev, BATCH_HS_BYTES)
    write_buf_desc(writes, g_gpu.dn_rq_set, 5u, g_gpu.dn_meta_dev, DN_META_BYTES)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    hz_set_bits(g_gpu.dn_conv_set, VHB_HQ, 0u, VHB_META, 0u, VHB_SM, VHB_Y1)   // 1/3 = untouched fillers
    hz_set_bits(g_gpu.dn_scan_set, VHB_HQ, VHB_ST, VHB_META, VHB_WS, VHB_SM, VHB_Y2)
    hz_set_bits(g_gpu.dn_rq_set, VHB_WS, 0u, VHB_META, VHB_HQ, VHB_HQ, 0u)     // 1/5 = untouched fillers
    g_gpu.dn_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: deltanet chain engaged\n")
}

def private fill_dn_meta(npos_w, cd, kd, di, nvh, nkh, ds, dconv : int64; hist_mode : int; nblocks : int64) {
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.dn_meta.mapped)
        mp[0] = uint(npos_w)
        mp[1] = uint(cd)
        mp[2] = uint(kd)
        mp[3] = uint(di)
        mp[4] = uint(nvh)
        mp[5] = uint(nkh)
        mp[6] = uint(ds)
        mp[7] = uint(dconv)
        mp[8] = uint(hist_mode)
        mp[9] = uint(DN_SLAB)
        mp[10] = uint(DN_O_OFF)
        mp[11] = uint(DN_SM_BETA)
        mp[12] = uint(DN_SM_G)
        mp[13] = uint(DN_SM_TAPS)
        mp[14] = uint(DN_SM_WNORM)
        mp[15] = uint(DN_SM_A)
        mp[16] = uint(DN_SM_DT)
        mp[17] = uint(DN_SM_HIST)
        mp[18] = uint(nblocks)
        mp[19] = uint(DN_P2_OFF)
    }
}

// one deltanet window: acts/meta/smalls staging copies, qkv + z tiles, conv, scan, the conv-tail
// extraction (before out-proj overwrites the qkv rows in y1), o requant, out-proj tile, y DMA;
// the state uploads on the first window and reads back on the last
def private record_dn_cmd(s_qkv, s_z, s_o : int; nwg_qkv, nwg_z, nwg_out : uint; rows, cd, di, dim, taps, nvh, ds : int64;
                          axq_bytes, axs_bytes : int64; first, last : bool) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    pfq_begin(raw)
    pfq_ts(raw)   // q0: submit start
    vhz_dep(raw, h, 0u, VHB_ACT, true)
    cmd_copy_batch_acts(raw, axq_bytes, axs_bytes)
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, s_qkv)
    cmd_copy_batch_meta(raw, s_z)
    cmd_copy_batch_meta(raw, s_o)
    cmd_copy_whole(raw, g_gpu.dn_meta.buf, g_gpu.dn_meta_dev, DN_META_BYTES)
    // stops SHORT of the hist region — later windows read the device-copied tail rows there
    vhz_dep(raw, h, 0u, VHB_SM, true)
    cmd_copy_range(raw, g_gpu.dn_smalls.buf, 0l, g_gpu.dn_smalls_dev, 0l, DN_SM_HIST * 4l)
    if (first) {
        cmd_copy_range(raw, g_gpu.dn_smalls.buf, DN_SM_HIST * 4l, g_gpu.dn_smalls_dev, DN_SM_HIST * 4l, cd * taps * 4l)
        vhz_dep(raw, h, 0u, VHB_ST, true)
        cmd_copy_whole(raw, g_gpu.dn_state_stage.buf, g_gpu.dn_state_dev, nvh * ds * ds * 4l)
    }
    pfq_ts(raw)   // q1: uploads
    hz_dispatch(raw, h, batch_pipe(0, g_gpu.stacks[s_qkv].batch_tile, g_gpu.stacks[s_qkv].batch_aligned),
        g_gpu.stacks[s_qkv].batch_set, nwg_qkv)
    hz_dispatch(raw, h, batch_pipe(0, g_gpu.stacks[s_z].batch_tile, g_gpu.stacks[s_z].batch_aligned),
        g_gpu.stacks[s_z].batch_set, nwg_z)
    pfq_ts(raw)   // q2: qkv+z GEMMs
    hz_dispatch(raw, h, weak_copy(g_gpu.dn_conv_pipeline), g_gpu.dn_conv_set, uint(rows))
    pfq_ts(raw)   // q3: conv
    hz_dispatch(raw, h, weak_copy(g_gpu.dn_scan_p1_pipeline), g_gpu.dn_scan_set, uint(nvh * ((rows + DN_CS - 1l) / DN_CS)))
    pfq_ts(raw)   // q4: scan phase 1
    hz_dispatch(raw, h, weak_copy(g_gpu.dn_scan_p2_pipeline), g_gpu.dn_scan_set, uint(nvh))
    pfq_ts(raw)   // q5: scan phase 2
    // conv-tail extraction: last `taps` RAW qkv rows out of y1 before out-proj reuses it
    if (!last) {
        vhz_dep(raw, h, VHB_Y1, VHB_SM, true)
        cmd_copy_range(raw, g_gpu.batch_y1_dev, (rows - taps) * cd * 4l, g_gpu.dn_smalls_dev, DN_SM_HIST * 4l, taps * cd * 4l)
    } else {
        vhz_dep(raw, h, VHB_Y1, 0u, true)
        cmd_copy_range(raw, g_gpu.batch_y1_dev, (rows - taps) * cd * 4l, g_gpu.dn_tail_host.buf, 0l, taps * cd * 4l)
        vhz_dep(raw, h, VHB_ST, 0u, true)
        cmd_copy_whole(raw, g_gpu.dn_state_dev, g_gpu.dn_state_host.buf, nvh * ds * ds * 4l)
    }
    pfq_ts(raw)   // q6: tail copies
    hz_dispatch(raw, h, weak_copy(g_gpu.dn_rq_pipeline), g_gpu.dn_rq_set, uint((rows * di / 4l + 255l) / 256l))
    pfq_ts(raw)   // q7: o requant
    hz_dispatch(raw, h, batch_pipe(0, g_gpu.stacks[s_o].batch_tile, g_gpu.stacks[s_o].batch_aligned),
        g_gpu.stacks[s_o].batch_set, nwg_out)
    pfq_ts(raw)   // q8: out GEMM
    vhz_dep(raw, h, VHB_Y1, 0u, true)
    cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, rows * dim * 4l)
    pfq_end(raw)
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_dn hz: {h.disp} nodes, {h.barriers} barriers\n")
    }
    vk_check(vkEndCommandBuffer(raw), null)
}

//! MoeGpuDnFn: one recurrent layer's whole deltanet attention block as one submit chain per
//! DN_WINDOW rows. yp gets npos x dim rows; tailp gets the last taps qkv rows for the caller's history store.
def vk_moe_dn(var yp : float?; woq, woz, woo : int64;
              xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
              convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
              var stp : float?; var tailp : float?; histp : float const?;
              npos, dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    let s_qkv = find_stack(woq, 0)
    let s_z = find_stack(woz, 0)
    let s_o = find_stack(woo, 0)
    let taps = dconv - 1l
    if (npos == 1l) {   // the decode step — device-resident state, one pre-recorded submit
        vk_moe_dn_step(yp, s_qkv, s_z, s_o, woq, woz, woo, xqp, xsp, bp, gp,
            convw, wnormp, ap, dtp, stp, histp, dim, cd, kd, di, nvh, nkh, ds, dconv, eps)
        return
    }
    assert(ds <= DN_MAX_DS && ds % 16l == 0l && nvh <= DN_MAX_HEADS && cd <= DN_MAX_CD
           && dconv <= DN_MAX_DCONV && di == nvh * ds && kd == nkh * ds && cd == 2l * kd + di
           && npos > taps && dim % 32l == 0l && di % 32l == 0l,
        "dasLLAMA vulkan tier: deltanet geometry outside chain capacity")
    ensure_batch_state()
    ensure_dn_state()
    ensure_batch_set(s_qkv, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
    ensure_batch_set(s_z, g_gpu.batch_y2_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y2)
    ensure_batch_set(s_o, g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.hs_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HQ, VHB_Y1)
    unsafe {
        var sp = reinterpret<float?>(g_gpu.dn_smalls.mapped)
        sp[0] = eps
        sp[1] = 1.0 / sqrt(float(ds))
        memcpy(reinterpret<void?>(sp + DN_SM_TAPS), reinterpret<void? -const>(convw), int(cd * dconv * 4l))
        memcpy(reinterpret<void?>(sp + DN_SM_WNORM), reinterpret<void? -const>(wnormp), int(ds * 4l))
        memcpy(reinterpret<void?>(sp + DN_SM_A), reinterpret<void? -const>(ap), int(nvh * 4l))
        memcpy(reinterpret<void?>(sp + DN_SM_DT), reinterpret<void? -const>(dtp), int(nvh * 4l))
        memcpy(reinterpret<void?>(sp + DN_SM_HIST), reinterpret<void? -const>(histp), int(cd * taps * 4l))
        memcpy(g_gpu.dn_state_stage.mapped, reinterpret<void? -const>(stp), int(nvh * ds * ds * 4l))
    }
    var qkv_region = fixed_array(woq, 0l, 0l)
    var z_region = fixed_array(woz, 0l, 0l)
    var o_region = fixed_array(woo, 0l, 0l)
    var r0 = 0l
    while (r0 < npos) {
        let rows = min(DN_WINDOW, npos - r0)
        let ts0 = ref_time_ticks()
        qkv_region[2] = rows
        z_region[2] = rows
        o_region[2] = rows
        var nwg_qkv = 0u
        var nwg_z = 0u
        var nwg_out = 0u
        unsafe {
            nwg_qkv = fill_batch_meta(s_qkv, addr(qkv_region[0]), 1l, dim, cd, 0l, rows)
            nwg_z = fill_batch_meta(s_z, addr(z_region[0]), 1l, dim, di, 0l, rows)
            nwg_out = fill_batch_meta(s_o, addr(o_region[0]), 1l, di, dim, 0l, rows)
        }
        upload_batch_acts(xqp, xsp, r0, rows, dim, 32l)
        unsafe {
            var sp = reinterpret<float?>(g_gpu.dn_smalls.mapped)
            par_memcpy(reinterpret<void?>(sp + DN_SM_BETA), reinterpret<void? -const>(bp + r0 * nvh), rows * nvh * 4l)
            par_memcpy(reinterpret<void?>(sp + DN_SM_G), reinterpret<void? -const>(gp + r0 * nvh), rows * nvh * 4l)
        }
        fill_dn_meta(rows, cd, kd, di, nvh, nkh, ds, dconv, r0 == 0l ? 1 : 2, rows * di / 32l)
        let ts1 = ref_time_ticks()
        record_dn_cmd(s_qkv, s_z, s_o, nwg_qkv, nwg_z, nwg_out, rows, cd, di, dim, taps, nvh, ds,
            rows * dim, rows * (dim / 32l) * 4l, r0 == 0l, r0 + rows == npos)
        submit_wait(g_gpu.batch_cmd)
        let ts2 = ref_time_ticks()
        unsafe {
            par_memcpy(reinterpret<void?>(yp + r0 * dim), g_gpu.batch_y1.mapped, rows * dim * 4l)
        }
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_dn prep {get_time_usec(ts0) - get_time_usec(ts1)} submit {get_time_usec(ts1) - get_time_usec(ts2)} copy {get_time_usec(ts2)}\n")
            to_log(LOG_INFO, "vk_dn gpu upl {int(pfq_us(1u))} qkv_z {int(pfq_us(2u))} conv {int(pfq_us(3u))} scan_p1 {int(pfq_us(4u))} scan_p2 {int(pfq_us(5u))} tailcp {int(pfq_us(6u))} rq {int(pfq_us(7u))} out {int(pfq_us(8u))}\n")
        }
        r0 += rows
    }
    unsafe {
        par_memcpy(reinterpret<void?>(stp), g_gpu.dn_state_host.mapped, nvh * ds * ds * 4l)
        memcpy(reinterpret<void?>(tailp), g_gpu.dn_tail_host.mapped, int(taps * cd * 4l))
    }
}

// ===== deltanet decode step (npos == 1 through the same MoeGpuDnFn) =====
// One pre-recorded submit per layer per token: qkv+z GEMVs into a SHARED y buffer -> the fused
// step kernel -> out GEMV -> y DMA. State/ring stay device-resident; flush/invalidate = the seams.

def private ensure_dnd_state {
    if (g_gpu.dnd_ready) {
        return
    }
    ensure_dn_state()   // pulls in ffn/batch state: batch_cmd + the dn staging bufs
    g_gpu.dnd_meta = make_host_buf(DN_META_BYTES, false)
    g_gpu.dnd_bg = make_host_buf(DND_BG_BYTES, false)
    g_gpu.dnd_oq_dev = make_device_buf(DND_OQ_BYTES)
    g_gpu.dnd_os_dev = make_device_buf(DND_OS_BYTES)
    g_gpu.dnd_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: deltanet decode step engaged\n")
}

def private fill_dnd_meta(cd, kd, di, nvh, nkh, ds, dconv : int64) {
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.dnd_meta.mapped)
        mp[0] = 1u
        mp[1] = uint(cd)
        mp[2] = uint(kd)
        mp[3] = uint(di)
        mp[4] = uint(nvh)
        mp[5] = uint(nkh)
        mp[6] = uint(ds)
        mp[7] = uint(dconv)
        mp[8] = 0u
        mp[9] = 0u
        mp[10] = 0u
        mp[11] = uint(DN_SM_BETA)
        mp[12] = uint(DN_SM_G)
        mp[13] = uint(DN_SM_TAPS)
        mp[14] = uint(DN_SM_WNORM)
        mp[15] = uint(DN_SM_A)
        mp[16] = uint(DN_SM_DT)
        mp[17] = uint(DN_SM_HIST)
        mp[18] = 0u
        mp[19] = 0u
    }
}

// create one layer's step chain: per-layer state/smalls buffers, the three sets, the once-filled
// GEMV metas (dn stacks never serve FFN GEMVs, so their host metas are ours to pin), and the
// warm cmd — per token only acts/β/γ refresh and the cmd resubmits as-is.
def private dnd_step_make(s_qkv, s_z, s_o : int; woq, woz, woo : int64;
                          dim, cd, di, nvh, ds, taps : int64) {
    ensure_gemv_scratch(s_qkv)
    ensure_gemv_scratch(s_z)
    ensure_gemv_scratch(s_o)
    var qkv_region = fixed_array(woq, 0l, 1l)
    var z_region = fixed_array(woz, 0l, 1l)
    var o_region = fixed_array(woo, 0l, 1l)
    unsafe {
        fill_stack_meta_rows(s_qkv, addr(qkv_region[0]), 1l, dim, cd, 1l, 0l)
        fill_stack_meta_rows(s_z, addr(z_region[0]), 1l, dim, di, 1l, 0l, cd)
        fill_stack_meta_rows(s_o, addr(o_region[0]), 1l, di, dim, 1l, 0l)
    }
    var st = DnStep(state_dev = make_device_buf(nvh * ds * ds * 4l),
                    smalls_dev = make_device_buf(DND_SMALLS_BYTES),
                    z_set = alloc_desc_set(), fused_set = alloc_desc_set(), out_set = alloc_desc_set(),
                    cmd = alloc_cmd(), s_qkv = s_qkv, s_z = s_z, s_o = s_o,
                    state_bytes = nvh * ds * ds * 4l, ring_bytes = cd * taps * 4l,
                    parity = 0, valid = false, dirty = false)
    var writes : array<WriteDescriptorSet>
    // z GEMV: z planes/meta (y base cd), the qkv stack's SHARED acts image and y buffer — the fused kernel sees qkv rows AND the z row on one binding
    write_buf_desc(writes, st.z_set, 0u, g_gpu.stacks[s_z].wqbuf, g_gpu.stacks[s_z].wqbytes)
    write_buf_desc(writes, st.z_set, 1u, g_gpu.stacks[s_z].wsbuf, g_gpu.stacks[s_z].wsbytes)
    write_buf_desc(writes, st.z_set, 2u, g_gpu.stacks[s_z].meta.buf, META_BYTES)
    write_buf_desc(writes, st.z_set, 3u, g_gpu.stacks[s_qkv].xq.buf, XQ_BYTES)
    write_buf_desc(writes, st.z_set, 4u, g_gpu.stacks[s_qkv].xs.buf, XS_BYTES)
    write_buf_desc(writes, st.z_set, 5u, g_gpu.stacks[s_qkv].y_dev, Y_BYTES)
    // fused step: o scales@0, THIS in-place state@1 (residency IS this binding), o quants@3, parity-ring smalls@4, the shared y@5
    write_buf_desc(writes, st.fused_set, 0u, g_gpu.dnd_os_dev, DND_OS_BYTES)
    write_buf_desc(writes, st.fused_set, 1u, st.state_dev, st.state_bytes)
    write_buf_desc(writes, st.fused_set, 2u, g_gpu.dn_meta_dev, DN_META_BYTES)
    write_buf_desc(writes, st.fused_set, 3u, g_gpu.dnd_oq_dev, DND_OQ_BYTES)
    write_buf_desc(writes, st.fused_set, 4u, st.smalls_dev, DND_SMALLS_BYTES)
    write_buf_desc(writes, st.fused_set, 5u, g_gpu.stacks[s_qkv].y_dev, Y_BYTES)
    // out GEMV: out planes@0/1, the stack's once-filled host meta@2, the o image@3/4, its y_dev@5
    write_buf_desc(writes, st.out_set, 0u, g_gpu.stacks[s_o].wqbuf, g_gpu.stacks[s_o].wqbytes)
    write_buf_desc(writes, st.out_set, 1u, g_gpu.stacks[s_o].wsbuf, g_gpu.stacks[s_o].wsbytes)
    write_buf_desc(writes, st.out_set, 2u, g_gpu.stacks[s_o].meta.buf, META_BYTES)
    write_buf_desc(writes, st.out_set, 3u, g_gpu.dnd_oq_dev, DND_OQ_BYTES)
    write_buf_desc(writes, st.out_set, 4u, g_gpu.dnd_os_dev, DND_OS_BYTES)
    write_buf_desc(writes, st.out_set, 5u, g_gpu.stacks[s_o].y_dev, Y_BYTES)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    // the qkv stack's raw_set plays Y1 in this chain (role-fluid sets re-declare per record)
    hz_set_bits(g_gpu.stacks[s_qkv].raw_set, 0u, 0u, 0u, 0u, 0u, VHG_Y1)
    hz_set_bits(st.z_set, 0u, 0u, 0u, 0u, 0u, VHG_Y2)
    hz_set_bits(st.fused_set, VHG_WS, VHG_ST, VHG_META, VHG_WS, VHG_SM, VHG_Y1 | VHG_Y2)
    hz_set_bits(st.out_set, 0u, 0u, 0u, VHG_WS, VHG_WS, VHG_YO)
    let raw = st.cmd
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    vhz_dep(raw, h, 0u, VHG_SM, true)   // one declaration for the smalls copy group
    cmd_copy_range(raw, g_gpu.dnd_bg.buf, 0l, st.smalls_dev, DN_SM_BETA * 4l, nvh * 4l)
    cmd_copy_range(raw, g_gpu.dnd_bg.buf, nvh * 4l, st.smalls_dev, DN_SM_G * 4l, nvh * 4l)
    cmd_copy_range(raw, g_gpu.dnd_bg.buf, 2l * nvh * 4l, st.smalls_dev, 8l, 4l)   // the ring parity word
    vhz_dep(raw, h, 0u, VHG_META, true)
    cmd_copy_whole(raw, g_gpu.dnd_meta.buf, g_gpu.dn_meta_dev, DN_META_BYTES)
    hz_dispatch(raw, h, gemv_pipe(0), g_gpu.stacks[s_qkv].raw_set, uint((cd + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
    hz_dispatch(raw, h, gemv_pipe(0), st.z_set, uint((di + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
    hz_dispatch(raw, h, weak_copy(g_gpu.dnd_fused_pipeline), st.fused_set, uint(nvh))
    hz_dispatch(raw, h, gemv_pipe(0), st.out_set, uint((dim + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
    vhz_dep(raw, h, VHG_YO, 0u, true)
    cmd_copy_whole(raw, g_gpu.stacks[s_o].y_dev, g_gpu.stacks[s_o].y.buf, dim * 4l)
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_dnd hz: {h.disp} nodes, {h.barriers} barriers\n")
    }
    vk_check(vkEndCommandBuffer(raw), null)
    g_gpu.dnd_steps |> insert(woq, st)
}

// cold upload: the engine's CPU state is authoritative — stage state + the whole smalls image
// (colds + ring; the β/γ region carries garbage until the first step overwrites it) and copy up
def private dnd_cold_upload(st : DnStep; stp : float const?; histp : float const?;
                            convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                            cd, taps, dconv, nvh, ds : int64; eps : float) {
    unsafe {
        var sp = reinterpret<float?>(g_gpu.dn_smalls.mapped)
        sp[0] = eps
        sp[1] = 1.0 / sqrt(float(ds))
        memcpy(reinterpret<void?>(sp + DN_SM_TAPS), reinterpret<void? -const>(convw), int(cd * dconv * 4l))
        memcpy(reinterpret<void?>(sp + DN_SM_WNORM), reinterpret<void? -const>(wnormp), int(ds * 4l))
        memcpy(reinterpret<void?>(sp + DN_SM_A), reinterpret<void? -const>(ap), int(nvh * 4l))
        memcpy(reinterpret<void?>(sp + DN_SM_DT), reinterpret<void? -const>(dtp), int(nvh * 4l))
        memcpy(reinterpret<void?>(sp + DN_SM_HIST), reinterpret<void? -const>(histp), int(cd * taps * 4l))
        memcpy(g_gpu.dn_state_stage.mapped, reinterpret<void? -const>(stp), int(st.state_bytes))
    }
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    cmd_copy_whole(raw, g_gpu.dn_smalls.buf, st.smalls_dev, DN_SMALLS_BYTES)
    cmd_copy_whole(raw, g_gpu.dn_state_stage.buf, st.state_dev, st.state_bytes)
    vk_check(vkEndCommandBuffer(raw), null)
    submit_wait(g_gpu.batch_cmd)
}

def private vk_moe_dn_step(var yp : float?; s_qkv, s_z, s_o : int; woq, woz, woo : int64;
                           xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
                           convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                           stp : float const?; histp : float const?;
                           dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    let taps = dconv - 1l
    assert(ds <= DN_MAX_DS && ds % 32l == 0l && nvh <= DN_MAX_HEADS && cd <= DN_MAX_CD
           && dconv <= DN_MAX_DCONV && di == nvh * ds && kd == nkh * ds && cd == 2l * kd + di
           && dim % 32l == 0l && di % 32l == 0l && di <= DND_OQ_BYTES,
        "dasLLAMA vulkan tier: deltanet geometry outside step capacity")
    ensure_dnd_state()
    if (!key_exists(g_gpu.dnd_steps, woq)) {
        fill_dnd_meta(cd, kd, di, nvh, nkh, ds, dconv)
        dnd_step_make(s_qkv, s_z, s_o, woq, woz, woo, dim, cd, di, nvh, ds, taps)
    }
    var st = g_gpu.dnd_steps[woq]
    if (!st.valid) {
        dnd_cold_upload(st, stp, histp, convw, wnormp, ap, dtp, cd, taps, dconv, nvh, ds, eps)
        st.parity = 0   // the cold image is ring image 0
    }
    let ts0 = ref_time_ticks()
    fill_stack_acts(s_qkv, xqp, xsp, dim)   // the z GEMV set shares this image
    unsafe {
        var bg = reinterpret<float?>(g_gpu.dnd_bg.mapped)
        memcpy(reinterpret<void?>(bg), reinterpret<void? -const>(bp), int(nvh * 4l))
        memcpy(reinterpret<void?>(bg + nvh), reinterpret<void? -const>(gp), int(nvh * 4l))
        bg[2l * nvh] = float(st.parity)
    }
    let ts1 = ref_time_ticks()
    submit_wait(st.cmd)
    let ts2 = ref_time_ticks()
    unsafe {
        memcpy(reinterpret<void?>(yp), g_gpu.stacks[s_o].y.mapped, int(dim * 4l))
    }
    st.parity = 1 - st.parity   // the step wrote the other image; it now holds current history
    st.valid = true
    st.dirty = true
    g_gpu.dnd_steps[woq] = st
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_dnd prep {get_time_usec(ts0) - get_time_usec(ts1)} submit {get_time_usec(ts1) - get_time_usec(ts2)} copy {get_time_usec(ts2)}\n")
    }
}

// MoeGpuDnFlushFn: sync one layer's device-resident state/ring down into the engine's buffers.
// Any flush retires the device copy (the CPU is about to read AND may then advance on its own) —
// the next GPU step cold-uploads from the engine's then-current state.
def vk_dn_step_flush(l : int64; var stp : float?; var histp : float?) {
    if (g_gpu == null || !g_gpu.dnd_ready) {
        return
    }
    let woq = moe_gpu_dn_layer_key(l)
    if (woq < 0l || !key_exists(g_gpu.dnd_steps, woq)) {
        return
    }
    var st = g_gpu.dnd_steps[woq]
    if (st.dirty) {
        let raw = g_gpu.batch_cmd
        let rf : VkCommandBufferResetFlags
        vk_check(vkResetCommandBuffer(raw, rf), null)
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        cmd_copy_whole(raw, st.state_dev, g_gpu.dn_state_host.buf, st.state_bytes)
        cmd_copy_range(raw, st.smalls_dev, DN_SM_HIST * 4l + int64(st.parity) * st.ring_bytes,
            g_gpu.dn_tail_host.buf, 0l, st.ring_bytes)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(g_gpu.batch_cmd)
        unsafe {
            memcpy(reinterpret<void?>(stp), g_gpu.dn_state_host.mapped, int(st.state_bytes))
            memcpy(reinterpret<void?>(histp), g_gpu.dn_tail_host.mapped, int(st.ring_bytes))
        }
    }
    st.dirty = false
    st.valid = false
    g_gpu.dnd_steps[woq] = st
}

// MoeGpuDnInvalidateFn: the engine rebuilt/zeroed its CPU state — every device copy is stale
def vk_dn_step_invalidate {
    if (g_gpu == null) {
        return
    }
    for (st in values(g_gpu.dnd_steps)) {
        st.valid = false
        st.dirty = false
    }
}

// ===== attention dispatch (the full-attention-layer chain) =====

def private ensure_at_state {
    if (g_gpu.at_ready) {
        return
    }
    ensure_batch_state()
    g_gpu.at_meta = make_host_buf(AT_META_BYTES, false)
    g_gpu.at_meta_dev = make_device_buf(AT_META_BYTES)
    g_gpu.at_smalls = make_host_buf(AT_SMALLS_BYTES, false)
    g_gpu.at_smalls_dev = make_device_buf(AT_SMALLS_BYTES)
    g_gpu.at_q_dev = make_device_buf(AT_WINDOW * AT_MAX_QD * 4l)
    g_gpu.at_g_dev = make_device_buf(AT_WINDOW * AT_MAX_QD * 4l)
    g_gpu.at_k_dev = make_device_buf(AT_CTX * AT_MAX_KV * 4l)
    g_gpu.at_v_dev = make_device_buf(AT_CTX * AT_MAX_KV * 4l)
    g_gpu.at_vraw_dev = make_device_buf(AT_WINDOW * AT_MAX_KV * 4l)
    g_gpu.at_kv_host = make_host_buf(2l * AT_WINDOW * AT_MAX_KV * 4l, true, [cached = true])
    g_gpu.at_prep_q_set = alloc_desc_set()
    g_gpu.at_prep_k_set = alloc_desc_set()
    g_gpu.at_attn_set = alloc_desc_set()
    g_gpu.at_rq_set = alloc_desc_set()
    var writes : array<WriteDescriptorSet>
    // prep-q bindings: q|gate GEMM rows@5, q out@0, gate out@3, smalls@4, params@2; 1 filler
    write_buf_desc(writes, g_gpu.at_prep_q_set, 0u, g_gpu.at_q_dev, AT_WINDOW * AT_MAX_QD * 4l)
    write_buf_desc(writes, g_gpu.at_prep_q_set, 1u, g_gpu.at_meta_dev, AT_META_BYTES)
    write_buf_desc(writes, g_gpu.at_prep_q_set, 2u, g_gpu.at_meta_dev, AT_META_BYTES)
    write_buf_desc(writes, g_gpu.at_prep_q_set, 3u, g_gpu.at_g_dev, AT_WINDOW * AT_MAX_QD * 4l)
    write_buf_desc(writes, g_gpu.at_prep_q_set, 4u, g_gpu.at_smalls_dev, AT_SMALLS_BYTES)
    write_buf_desc(writes, g_gpu.at_prep_q_set, 5u, g_gpu.batch_y1_dev, BATCH_Y_BYTES)
    // prep-k: roped k out@0 lands at ABSOLUTE positions; the gate view@3 never fires for a k row
    write_buf_desc(writes, g_gpu.at_prep_k_set, 0u, g_gpu.at_k_dev, AT_CTX * AT_MAX_KV * 4l)
    write_buf_desc(writes, g_gpu.at_prep_k_set, 1u, g_gpu.at_meta_dev, AT_META_BYTES)
    write_buf_desc(writes, g_gpu.at_prep_k_set, 2u, g_gpu.at_meta_dev, AT_META_BYTES)
    write_buf_desc(writes, g_gpu.at_prep_k_set, 3u, g_gpu.at_meta_dev, AT_META_BYTES)
    write_buf_desc(writes, g_gpu.at_prep_k_set, 4u, g_gpu.at_smalls_dev, AT_SMALLS_BYTES)
    write_buf_desc(writes, g_gpu.at_prep_k_set, 5u, g_gpu.batch_y2_dev, BATCH_Y_BYTES)
    // attn bindings: q@0, k@1, gate in/o out@3, smalls@4, v@5, params@2
    write_buf_desc(writes, g_gpu.at_attn_set, 0u, g_gpu.at_q_dev, AT_WINDOW * AT_MAX_QD * 4l)
    write_buf_desc(writes, g_gpu.at_attn_set, 1u, g_gpu.at_k_dev, AT_CTX * AT_MAX_KV * 4l)
    write_buf_desc(writes, g_gpu.at_attn_set, 2u, g_gpu.at_meta_dev, AT_META_BYTES)
    write_buf_desc(writes, g_gpu.at_attn_set, 3u, g_gpu.at_g_dev, AT_WINDOW * AT_MAX_QD * 4l)
    write_buf_desc(writes, g_gpu.at_attn_set, 4u, g_gpu.at_smalls_dev, AT_SMALLS_BYTES)
    write_buf_desc(writes, g_gpu.at_attn_set, 5u, g_gpu.at_v_dev, AT_CTX * AT_MAX_KV * 4l)
    // requant bindings: o rows@0, quant words@3, scales@4 (wo GEMM activation image), params@2
    write_buf_desc(writes, g_gpu.at_rq_set, 0u, g_gpu.at_g_dev, AT_WINDOW * AT_MAX_QD * 4l)
    write_buf_desc(writes, g_gpu.at_rq_set, 1u, g_gpu.at_meta_dev, AT_META_BYTES)
    write_buf_desc(writes, g_gpu.at_rq_set, 2u, g_gpu.at_meta_dev, AT_META_BYTES)
    write_buf_desc(writes, g_gpu.at_rq_set, 3u, g_gpu.hq_dev, BATCH_HQ_BYTES)
    write_buf_desc(writes, g_gpu.at_rq_set, 4u, g_gpu.hs_dev, BATCH_HS_BYTES)
    write_buf_desc(writes, g_gpu.at_rq_set, 5u, g_gpu.at_meta_dev, AT_META_BYTES)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    hz_set_bits(g_gpu.at_prep_q_set, VHB_ATQ, 0u, VHB_META, VHB_ATG, VHB_SM, VHB_Y1)
    hz_set_bits(g_gpu.at_prep_k_set, VHB_ATK, 0u, VHB_META, 0u, VHB_SM, VHB_Y2)   // 3 = the never-fired gate view
    hz_set_bits(g_gpu.at_attn_set, VHB_ATQ, VHB_ATK, VHB_META, VHB_ATG, VHB_SM, VHB_ATV)
    hz_set_bits(g_gpu.at_rq_set, VHB_ATG, 0u, VHB_META, VHB_HQ, VHB_HQ, 0u)
    g_gpu.at_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: attention chain engaged\n")
}

def private fill_at_meta(rows, w0, qd, kv_dim, hs, half, n_heads, n_kv_heads, kv_mul, nblocks : int64; gated, qknorm : bool) {
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.at_meta.mapped)
        mp[0] = uint(rows)
        mp[1] = uint(w0)
        mp[2] = uint(qd)
        mp[3] = uint(kv_dim)
        mp[4] = uint(hs)
        mp[5] = uint(half)
        mp[6] = uint(n_heads)
        mp[7] = uint(n_kv_heads)
        mp[8] = uint(kv_mul)
        mp[9] = uint((gated ? 1 : 0) | (qknorm ? 2 : 0))
        mp[10] = 0u
        mp[11] = uint(hs / 32l)
        mp[12] = uint(w0 + rows)
        mp[13] = uint(AT_SM_COS + rows * half)
        mp[14] = uint(AT_SM_RMSQ)
        mp[15] = uint(AT_SM_RMSK)
        mp[16] = uint(AT_SM_COS)
        mp[17] = 0u
        mp[18] = uint(nblocks)
    }
}

// one attention window: acts/meta/smalls staging copies, q|gate + k + v tiles, the prep pair
// (deinterleave/qk-rms/rope; k lands at absolute positions), v append + k/v host DMA in the
// transfer window, the flash attention pass, o requant, wo tile, y DMA
def private record_at_cmd(s_q, s_k, s_v, s_o : int; nwg_q, nwg_k, nwg_v, nwg_o : uint;
                          rows, w0, qd, kv_dim, dim, n_heads : int64;
                          axq_bytes, axs_bytes, sm_bytes : int64; o_kq : bool) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    vhz_dep(raw, h, 0u, VHB_ACT, true)
    cmd_copy_batch_acts(raw, axq_bytes, axs_bytes)
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, s_q)
    cmd_copy_batch_meta(raw, s_k)
    cmd_copy_batch_meta(raw, s_v)
    cmd_copy_batch_meta(raw, s_o)
    cmd_copy_whole(raw, g_gpu.at_meta.buf, g_gpu.at_meta_dev, AT_META_BYTES)
    vhz_dep(raw, h, 0u, VHB_SM, true)
    cmd_copy_range(raw, g_gpu.at_smalls.buf, 0l, g_gpu.at_smalls_dev, 0l, sm_bytes)
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s_q].fmt, g_gpu.stacks[s_q].batch_tile, g_gpu.stacks[s_q].batch_aligned),
        g_gpu.stacks[s_q].batch_set, nwg_q)
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s_k].fmt, g_gpu.stacks[s_k].batch_tile, g_gpu.stacks[s_k].batch_aligned),
        g_gpu.stacks[s_k].batch_set, nwg_k)
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s_v].fmt, g_gpu.stacks[s_v].batch_tile, g_gpu.stacks[s_v].batch_aligned),
        g_gpu.stacks[s_v].batch_set, nwg_v)
    hz_dispatch(raw, h, weak_copy(g_gpu.at_prep_q_pipeline), g_gpu.at_prep_q_set, uint(rows))
    hz_dispatch(raw, h, weak_copy(g_gpu.at_prep_k_pipeline), g_gpu.at_prep_k_set, uint(rows))
    // transfer window: append raw v at absolute positions, DMA roped k/raw v to host
    vhz_dep(raw, h, VHB_VRAW, VHB_ATV, true)
    cmd_copy_range(raw, g_gpu.at_vraw_dev, 0l, g_gpu.at_v_dev, w0 * kv_dim * 4l, rows * kv_dim * 4l)
    vhz_dep(raw, h, VHB_ATK, 0u, true)
    cmd_copy_range(raw, g_gpu.at_k_dev, w0 * kv_dim * 4l, g_gpu.at_kv_host.buf, 0l, rows * kv_dim * 4l)
    vhz_dep(raw, h, VHB_VRAW, 0u, true)
    cmd_copy_range(raw, g_gpu.at_vraw_dev, 0l, g_gpu.at_kv_host.buf, AT_WINDOW * AT_MAX_KV * 4l, rows * kv_dim * 4l)
    hz_dispatch(raw, h, weak_copy(g_gpu.at_attn_pipeline), g_gpu.at_attn_set, uint(n_heads * ((rows + 7l) / 8l)))
    hz_dispatch(raw, h, weak_copy(o_kq ? g_gpu.q8k_rq_pipeline : g_gpu.dn_rq_pipeline),
        g_gpu.at_rq_set, uint((rows * qd / (o_kq ? 8l : 4l) + 255l) / 256l))
    hz_dispatch(raw, h, batch_pipe(g_gpu.stacks[s_o].fmt, g_gpu.stacks[s_o].batch_tile, g_gpu.stacks[s_o].batch_aligned),
        g_gpu.stacks[s_o].batch_set, nwg_o)
    vhz_dep(raw, h, VHB_Y1, 0u, true)
    cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, rows * dim * 4l)
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_attn hz: {h.disp} nodes, {h.barriers} barriers\n")
    }
    vk_check(vkEndCommandBuffer(raw), null)
}

//! MoeGpuAttnFn: one full-attention layer's whole softmax attention block as one submit chain per
//! AT_WINDOW rows. yp gets npos x dim rows; kp/vp the roped-k / raw-v rows for the caller's cache store.
//! K/V persist device-side at absolute positions, so later windows attend the whole batch.
def vk_moe_attn(var yp : float?; var kp : float?; var vp : float?;
                woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                xqp : int8 const?; xsp : float const?;
                rmsqp : float const?; rmskp : float const?;
                cosp : float const?; sinp : float const?;
                npos, dim, qd, kv_dim, hs, rot, n_heads, kv_mul : int64;
                scale : float; eps : float; gated : bool; qknorm : bool) {
    let s_q = find_stack(woq, fq)
    let s_k = find_stack(wok, fk)
    let s_v = find_stack(wov, fv)
    let s_o = find_stack(woo, fo)
    // q/k/v share one activation image, so their format class must agree (the FFN triple rule);
    // the wo stack is free — the requant form follows it
    let kq = fq != 0
    assert((fk != 0) == kq && (fv != 0) == kq,
        "dasLLAMA vulkan tier: q/k/v stack format class mismatch in one attention chain")
    let half = rot / 2l
    let n_kv = kv_dim / hs
    assert(npos <= AT_CTX && qd <= AT_MAX_QD && kv_dim <= AT_MAX_KV
           && (hs == 128l || hs == 256l) && half >= 1l && 2l * half <= hs
           && n_heads * hs == qd && n_kv * hs == kv_dim && n_heads == n_kv * kv_mul
           && n_heads <= 64l && dim % 32l == 0l && kv_dim % 32l == 0l
           && qd % (fo != 0 ? 256l : 32l) == 0l
           && (gated ? 2l : 1l) * qd * AT_WINDOW * 4l <= BATCH_Y_BYTES
           && qd * AT_WINDOW <= BATCH_HQ_BYTES && npos * dim * 4l <= BATCH_Y_BYTES,
        "dasLLAMA vulkan tier: attention geometry outside chain capacity")
    let xunit = kq ? 256l : 32l
    ensure_batch_state()
    ensure_at_state()
    ensure_batch_set(s_q, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
    ensure_batch_set(s_k, g_gpu.batch_y2_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y2)
    ensure_batch_set(s_v, g_gpu.at_vraw_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES,
        VHB_ACT, VHB_VRAW, [ybytes = AT_WINDOW * AT_MAX_KV * 4l])
    ensure_batch_set(s_o, g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.hs_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HQ, VHB_Y1)
    unsafe {
        var sp = reinterpret<float?>(g_gpu.at_smalls.mapped)
        sp[0] = eps
        sp[1] = scale
        if (qknorm) {
            memcpy(reinterpret<void?>(sp + AT_SM_RMSQ), reinterpret<void? -const>(rmsqp), int(hs * 4l))
            memcpy(reinterpret<void?>(sp + AT_SM_RMSK), reinterpret<void? -const>(rmskp), int(hs * 4l))
        }
    }
    var q_region = fixed_array(woq, 0l, 0l)
    var k_region = fixed_array(wok, 0l, 0l)
    var v_region = fixed_array(wov, 0l, 0l)
    var o_region = fixed_array(woo, 0l, 0l)
    var r0 = 0l
    while (r0 < npos) {
        let rows = min(AT_WINDOW, npos - r0)
        let ts0 = ref_time_ticks()
        q_region[1] = r0
        q_region[2] = rows
        k_region[1] = r0
        k_region[2] = rows
        v_region[1] = r0
        v_region[2] = rows
        o_region[1] = r0
        o_region[2] = rows
        var nwg_q = 0u
        var nwg_k = 0u
        var nwg_v = 0u
        var nwg_o = 0u
        unsafe {
            nwg_q = fill_batch_meta(s_q, addr(q_region[0]), 1l, dim, gated ? 2l * qd : qd, r0, r0 + rows)
            nwg_k = fill_batch_meta(s_k, addr(k_region[0]), 1l, dim, kv_dim, r0, r0 + rows)
            nwg_v = fill_batch_meta(s_v, addr(v_region[0]), 1l, dim, kv_dim, r0, r0 + rows)
            nwg_o = fill_batch_meta(s_o, addr(o_region[0]), 1l, qd, dim, r0, r0 + rows)
        }
        upload_batch_acts(xqp, xsp, r0, rows, dim, xunit)
        unsafe {
            var sp = reinterpret<float?>(g_gpu.at_smalls.mapped)
            par_memcpy(reinterpret<void?>(sp + AT_SM_COS), reinterpret<void? -const>(cosp + r0 * half), rows * half * 4l)
            par_memcpy(reinterpret<void?>(sp + AT_SM_COS + rows * half), reinterpret<void? -const>(sinp + r0 * half), rows * half * 4l)
        }
        fill_at_meta(rows, r0, qd, kv_dim, hs, half, n_heads, n_kv, kv_mul,
            rows * qd / (fo != 0 ? 256l : 32l), gated, qknorm)
        let ts1 = ref_time_ticks()
        record_at_cmd(s_q, s_k, s_v, s_o, nwg_q, nwg_k, nwg_v, nwg_o,
            rows, r0, qd, kv_dim, dim, n_heads,
            rows * dim, rows * (dim / xunit) * 4l, (AT_SM_COS + 2l * rows * half) * 4l, fo != 0)
        submit_wait(g_gpu.batch_cmd)
        let ts2 = ref_time_ticks()
        unsafe {
            par_memcpy(reinterpret<void?>(yp + r0 * dim), g_gpu.batch_y1.mapped, rows * dim * 4l)
            var kvp8 = reinterpret<uint8?>(g_gpu.at_kv_host.mapped)
            par_memcpy(reinterpret<void?>(kp + r0 * kv_dim), g_gpu.at_kv_host.mapped, rows * kv_dim * 4l)
            par_memcpy(reinterpret<void?>(vp + r0 * kv_dim), reinterpret<void?>(kvp8 + AT_WINDOW * AT_MAX_KV * 4l), rows * kv_dim * 4l)
        }
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_attn prep {get_time_usec(ts0) - get_time_usec(ts1)} submit {get_time_usec(ts1) - get_time_usec(ts2)} copy {get_time_usec(ts2)}\n")
        }
        r0 += rows
    }
}

// ===== decode dispatch (the FFN's GEMV arm) =====

// pre-record (or refresh on a row-count change) the triple's chain: gate+up GEMV pair, fused
// act+requant, down GEMV, y DMA. Per token only the stacks' host-visible metas, the activation
// image, and the ffn params staging change — the recorded buffer resubmits as-is.
def private ffn_cmd_for(s1, s3, s2 : int; nrows, n, nfe : int64) : VkCommandBuffer {
    let key = (int64(s1) * int64(MAX_STACKS) + int64(s3)) * int64(MAX_STACKS) + int64(s2)
    if (!key_exists(g_gpu.ffn_cmds, key)) {
        var fc = FfnCmd(cmd = alloc_cmd(), actrq_set = alloc_desc_set(),
                        down_set = alloc_desc_set(), nrows = 0l)
        var writes : array<WriteDescriptorSet>
        // fused act+requant bindings: up@0, params@2, hq/hs@3/4, gate@5; binding 1 is unused filler
        write_buf_desc(writes, fc.actrq_set, 0u, g_gpu.stacks[s3].y_dev, Y_BYTES)
        write_buf_desc(writes, fc.actrq_set, 1u, g_gpu.stacks[s1].wsbuf, ws_range(s1))
        write_buf_desc(writes, fc.actrq_set, 2u, g_gpu.ffn_meta_dev, FFN_META_BYTES)
        write_buf_desc(writes, fc.actrq_set, 3u, g_gpu.hq_dev, BATCH_HQ_BYTES)
        write_buf_desc(writes, fc.actrq_set, 4u, g_gpu.hs_dev, BATCH_HS_BYTES)
        write_buf_desc(writes, fc.actrq_set, 5u, g_gpu.stacks[s1].y_dev, Y_BYTES)
        // down: its own planes + host-visible meta, hq/hs as the activation image, its y_dev
        write_buf_desc(writes, fc.down_set, 0u, g_gpu.stacks[s2].wqbuf, g_gpu.stacks[s2].wqbytes)
        write_buf_desc(writes, fc.down_set, 1u, g_gpu.stacks[s2].wsbuf, ws_range(s2))
        write_buf_desc(writes, fc.down_set, 2u, g_gpu.stacks[s2].meta.buf, META_BYTES)
        write_buf_desc(writes, fc.down_set, 3u, g_gpu.hq_dev, BATCH_HQ_BYTES)
        write_buf_desc(writes, fc.down_set, 4u, g_gpu.hs_dev, BATCH_HS_BYTES)
        write_buf_desc(writes, fc.down_set, 5u, g_gpu.stacks[s2].y_dev, Y_BYTES)
        let no_copies : array<CopyDescriptorSet>
        update_descriptor_sets(g_gpu.device, writes, no_copies)
        g_gpu.ffn_cmds |> insert(key, fc)
    }
    var fc = g_gpu.ffn_cmds[key]
    if (fc.nrows != nrows) {
        if (fc.nrows != 0l) {
            let rf : VkCommandBufferResetFlags
            vk_check(vkResetCommandBuffer(fc.cmd, rf), null)
        }
        let raw = fc.cmd
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        // gate/up roles on the two stacks' raw_sets, this chain's view (re-declared per record)
        hz_set_bits(g_gpu.stacks[s1].raw_set, 0u, 0u, 0u, 0u, 0u, VHG_Y1)
        hz_set_bits(g_gpu.stacks[s3].raw_set, 0u, 0u, 0u, 0u, 0u, VHG_Y2)
        hz_set_bits(fc.actrq_set, VHG_Y2, 0u, VHG_META, VHG_HQ, VHG_HQ, VHG_Y1)
        hz_set_bits(fc.down_set, 0u, 0u, 0u, VHG_HQ, VHG_HQ, VHG_YO)
        var h : VkHaz
        vhz_dep(raw, h, 0u, VHG_META, true)
        cmd_copy_whole(raw, g_gpu.ffn_meta.buf, g_gpu.ffn_meta_dev, FFN_META_BYTES)
        let ggu = uint((nrows * nfe + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg)
        hz_dispatch(raw, h, gemv_pipe(g_gpu.stacks[s1].fmt), g_gpu.stacks[s1].raw_set, ggu)
        hz_dispatch(raw, h, gemv_pipe(g_gpu.stacks[s3].fmt), g_gpu.stacks[s3].raw_set, ggu)
        // the requant form follows the DOWN stack (q8n counts as q8 — decode acts stay Q8_0)
        let dkq = !vk_fmt_b32(g_gpu.stacks[s2].fmt)
        hz_dispatch(raw, h, weak_copy(dkq ? g_gpu.actrq_k_pipeline : g_gpu.actrq_pipeline),
            fc.actrq_set, uint((nrows * nfe / (dkq ? 8l : 4l) + 255l) / 256l))
        hz_dispatch(raw, h, gemv_pipe(g_gpu.stacks[s2].fmt), fc.down_set, uint((nrows * n + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.stacks[s2].y_dev, g_gpu.stacks[s2].y.buf, nrows * n * 4l)
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_ffng hz: {h.disp} nodes, {h.barriers} barriers\n")
        }
        vk_check(vkEndCommandBuffer(raw), null)
        g_gpu.ffn_cmds[key] = FfnCmd(cmd = fc.cmd, actrq_set = fc.actrq_set,
            down_set = fc.down_set, nrows = nrows)
    }
    return fc.cmd
}

// fill metas/acts and fetch the recorded chain — shared by the sync GEMV form and the async
// begin/join pair. Returns the down stack index alongside the cmd (join reads its y staging).
def private ffn_gemv_prep(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n, nfe, nrows : int64; f1, f3, f2 : int; is_gelu : bool) : tuple<cmd : VkCommandBuffer; s2 : int> {
    ensure_ffn_state()   // fill_ffn_meta below writes its staging: a shexp-only run dispatches FFN first
    let s1 = find_stack(unsafe(offs1[0]), f1)
    let s3 = find_stack(unsafe(offs3[0]), f3)
    let s2 = find_stack(unsafe(offs2[0]), f2)
    assert(nrows * max(nfe, n) * 4l <= Y_BYTES, "dasLLAMA vulkan tier: FFN dispatch exceeds output capacity")
    // gate/up share one activation image (same xqp below) so forms must agree; down's is its own
    let kq_in = !vk_fmt_b32(f1)
    assert(!vk_fmt_b32(f3) == kq_in,
        "dasLLAMA vulkan tier: gate/up form mismatch — they share one activation image")
    let xunit_in = fmt_unit(f1)
    let xunit_mid = fmt_unit(f2)
    ensure_gemv_scratch(s1)
    ensure_gemv_scratch(s3)
    ensure_gemv_scratch(s2)
    fill_stack_meta_rows(s1, offs1, nregions, n, nfe, nrows, n / xunit_in)
    fill_stack_meta_rows(s3, offs3, nregions, n, nfe, nrows, n / xunit_in)
    fill_stack_meta_rows(s2, offs2, nregions, nfe, n, nrows, nfe / xunit_mid)
    fill_stack_acts(s1, xqp, xsp, nrows * n)
    fill_stack_acts(s3, xqp, xsp, nrows * n)
    fill_ffn_meta(nrows * nfe, nrows * nfe / xunit_mid, is_gelu)
    return (cmd = ffn_cmd_for(s1, s3, s2, nrows, n, nfe), s2 = s2)
}

def private vk_moe_ffn_gemv(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n, nfe, nrows : int64; f1, f3, f2 : int; is_gelu : bool) {
    // decode-form only — vk_moe_ffn routes the combined (npos > 0) contract to the batch arm instead
    var pc = ffn_gemv_prep(offs1, offs3, offs2, nregions, xqp, xsp, n, nfe, nrows, f1, f3, f2, is_gelu)
    submit_wait(pc.cmd)
    unsafe {
        memcpy(reinterpret<void?>(goutp), g_gpu.stacks[pc.s2].y.mapped, int(nrows * n * 4l))
    }
}

//! MoeGpuFfnBeginFn: submit the decode FFN chain and return — the CPU runs the miss experts
//! (and anything else) while the GPU crunches; vk_moe_ffn_join collects. ONE in flight.
def vk_moe_ffn_begin(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; n, nfe, nrows : int64; f1, f3, f2 : int; is_gelu : bool) {
    assert(g_gpu.ffn_pend_s2 < 0, "dasLLAMA vulkan tier: async FFN begin while one is already in flight")
    var pc = ffn_gemv_prep(offs1, offs3, offs2, nregions, xqp, xsp, n, nfe, nrows, f1, f3, f2, is_gelu)
    submit_nowait(pc.cmd)
    g_gpu.ffn_pend_s2 = pc.s2
    g_gpu.ffn_pend_bytes = nrows * n * 4l
}

//! MoeGpuFfnJoinFn: spin the pending async FFN's fence and copy its output rows back.
def vk_moe_ffn_join(var goutp : float?) {
    assert(g_gpu.ffn_pend_s2 >= 0, "dasLLAMA vulkan tier: async FFN join with nothing in flight")
    wait_fence()
    unsafe {
        memcpy(reinterpret<void?>(goutp), g_gpu.stacks[g_gpu.ffn_pend_s2].y.mapped, int(g_gpu.ffn_pend_bytes))
    }
    g_gpu.ffn_pend_s2 = -1
}

// ===== decode q/k/v group (one submit per attention layer) =====
// Shared activation image + shared y (per-plane bases via the GEMV meta) ⇒ ONE submit, not three
// (~74us each would eat the win). Serial-chain work: pays on GPU read speed, nothing hides here.

def private qkv_cmd_make(sq, sk, sv : int; woq, wok, wov : int64; n, dq, dk, dv : int64) {
    ensure_gemv_scratch(sq)
    ensure_gemv_scratch(sk)
    ensure_gemv_scratch(sv)
    var qreg = fixed_array(woq, 0l, 1l)
    var kreg = fixed_array(wok, 0l, 1l)
    var vreg = fixed_array(wov, 0l, 1l)
    unsafe {   // metas are ours to pin: these stacks serve no other decode GEMV
        fill_stack_meta_rows(sq, addr(qreg[0]), 1l, n, dq, 1l, 0l)
        fill_stack_meta_rows(sk, addr(kreg[0]), 1l, n, dk, 1l, 0l, dq)
        fill_stack_meta_rows(sv, addr(vreg[0]), 1l, n, dv, 1l, 0l, dq + dk)
    }
    var qc = QkvCmd(cmd = alloc_cmd(), k_set = alloc_desc_set(), v_set = alloc_desc_set(),
                    sq = sq, ybytes = (dq + dk + dv) * 4l)
    assert(qc.ybytes <= Y_BYTES, "dasLLAMA vulkan tier: qkv group exceeds output capacity")
    var writes : array<WriteDescriptorSet>
    write_buf_desc(writes, qc.k_set, 0u, g_gpu.stacks[sk].wqbuf, g_gpu.stacks[sk].wqbytes)
    write_buf_desc(writes, qc.k_set, 1u, g_gpu.stacks[sk].wsbuf, g_gpu.stacks[sk].wsbytes)
    write_buf_desc(writes, qc.k_set, 2u, g_gpu.stacks[sk].meta.buf, META_BYTES)
    write_buf_desc(writes, qc.k_set, 3u, g_gpu.stacks[sq].xq.buf, XQ_BYTES)
    write_buf_desc(writes, qc.k_set, 4u, g_gpu.stacks[sq].xs.buf, XS_BYTES)
    write_buf_desc(writes, qc.k_set, 5u, g_gpu.stacks[sq].y_dev, Y_BYTES)
    write_buf_desc(writes, qc.v_set, 0u, g_gpu.stacks[sv].wqbuf, g_gpu.stacks[sv].wqbytes)
    write_buf_desc(writes, qc.v_set, 1u, g_gpu.stacks[sv].wsbuf, g_gpu.stacks[sv].wsbytes)
    write_buf_desc(writes, qc.v_set, 2u, g_gpu.stacks[sv].meta.buf, META_BYTES)
    write_buf_desc(writes, qc.v_set, 3u, g_gpu.stacks[sq].xq.buf, XQ_BYTES)
    write_buf_desc(writes, qc.v_set, 4u, g_gpu.stacks[sq].xs.buf, XS_BYTES)
    write_buf_desc(writes, qc.v_set, 5u, g_gpu.stacks[sq].y_dev, Y_BYTES)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    let raw = qc.cmd
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    let fq = g_gpu.stacks[sq].fmt
    let fk = g_gpu.stacks[sk].fmt
    let fv = g_gpu.stacks[sv].fmt
    // three disjoint row levels on the shared y — the q stack's raw_set plays Y1 in this chain
    hz_set_bits(g_gpu.stacks[sq].raw_set, 0u, 0u, 0u, 0u, 0u, VHG_Y1)
    hz_set_bits(qc.k_set, 0u, 0u, 0u, 0u, 0u, VHG_Y2)
    hz_set_bits(qc.v_set, 0u, 0u, 0u, 0u, 0u, VHG_Y3)
    var h : VkHaz
    hz_dispatch(raw, h, gemv_pipe(fq), g_gpu.stacks[sq].raw_set, uint((dq + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
    hz_dispatch(raw, h, gemv_pipe(fk), qc.k_set, uint((dk + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
    hz_dispatch(raw, h, gemv_pipe(fv), qc.v_set, uint((dv + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
    vhz_dep(raw, h, VHG_Y1 | VHG_Y2 | VHG_Y3, 0u, true)
    cmd_copy_whole(raw, g_gpu.stacks[sq].y_dev, g_gpu.stacks[sq].y.buf, qc.ybytes)
    vk_check(vkEndCommandBuffer(raw), null)
    g_gpu.qkv_cmds |> insert(woq, qc)
}

//! MoeGpuQkvFn: one attention layer's q/k/v decode projections as ONE submit. The three planes
//! must share a quant CLASS (one activation image); the caller's gate enforces it.
def vk_moe_qkv(var yq : float?; var yk : float?; var yv : float?; woq, wok, wov : int64;
               fq, fk, fv : int; xqp : int8 const?; xsp : float const?; n, dq, dk, dv : int64) {
    let sq = find_stack(woq, fq)
    let sk = find_stack(wok, fk)
    let sv = find_stack(wov, fv)
    if (!key_exists(g_gpu.qkv_cmds, woq)) {
        qkv_cmd_make(sq, sk, sv, woq, wok, wov, n, dq, dk, dv)
    }
    var qc = g_gpu.qkv_cmds[woq]
    fill_stack_acts(sq, xqp, xsp, n)
    submit_wait(qc.cmd)
    unsafe {
        let yp = reinterpret<float?>(g_gpu.stacks[sq].y.mapped)
        memcpy(reinterpret<void?>(yq), reinterpret<void?>(yp), int(dq * 4l))
        memcpy(reinterpret<void?>(yk), reinterpret<void?>(yp + dq), int(dk * 4l))
        memcpy(reinterpret<void?>(yv), reinterpret<void?>(yp + dq + dk), int(dv * 4l))
    }
}

// ===== heat-pinned expert cache (decode split-FFN) =====
// Per-layer slot pools live in the SYNTHETIC offset space, so the whole decode FFN GEMV rail
// (find_stack, metas, FfnCmd) serves hit sets verbatim; uploads DMA-slice the pinned stream mirrors — no CPU gather on the decode path.

def private heat_stream_src(base : int64; fmt : int) : int {
    for (i in range(length(g_gpu.stream_stacks))) {
        if (g_gpu.stream_stacks[i].base == base && g_gpu.stream_stacks[i].fmt == fmt) {
            return i
        }
    }
    return -1
}

def private heat_pool_make(l : int64; b1, b3, b2 : int64; f1, f3, f2 : int; n, nfe : int64) {
    ensure_batch_state()   // advise records its slice copies into batch_cmd
    var pool = HeatPool(ege = n * nfe)
    let src1 = heat_stream_src(b1, f1)
    let src3 = heat_stream_src(b3, f3)
    let src2 = heat_stream_src(b2, f2)
    let want = gpu_want_heat()
    // gate/up share one act image; down is free — the mid-step form follows IT (the mixed-triple rail)
    if (src1 < 0 || src3 < 0 || src2 < 0 || vk_fmt_b32(f3) != vk_fmt_b32(f1) || want <= 0l) {
        let why = (want <= 0l ? "want 0"
            : (src1 < 0 || src3 < 0 || src2 < 0 ? "no mirror src {src1}/{src3}/{src2} for b {b1}/{b3}/{b2} fmt {f1}/{f3}/{f2}"
            : "gate/up form mismatch fmt {f1}/{f3}"))
        to_log(LOG_INFO, "dasLLAMA vulkan tier: heat cache skips layer {l} ({why})\n")
        g_gpu.heat_pools[l] <- pool
        return
    }
    let pb1 = stack_plane_bytes(n, nfe, f1)
    let pb3 = stack_plane_bytes(n, nfe, f3)
    let pb2 = stack_plane_bytes(nfe, n, f2)
    let per_slot = pb1.wq + pb1.ws + pb3.wq + pb3.ws + pb2.wq + pb2.ws
    let c = min(want, (g_gpu.weight_budget - g_gpu.resident_bytes) / per_slot)
    if (c <= 0l) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: heat cache skips layer {l} (VRAM budget)\n")
        g_gpu.heat_pools[l] <- pool
        return
    }
    let base = HEAT_BASE + l * 3l * HEAT_STRIDE
    pool.s1 = make_stack_shell(base, c * pool.ege, f1, c * pb1.wq, c * pb1.ws)
    pool.s3 = make_stack_shell(base + HEAT_STRIDE, c * pool.ege, f3, c * pb3.wq, c * pb3.ws)
    pool.s2 = make_stack_shell(base + 2l * HEAT_STRIDE, c * pool.ege, f2, c * pb2.wq, c * pb2.ws)
    ensure_gemv_scratch(pool.s1)
    ensure_gemv_scratch(pool.s3)
    ensure_gemv_scratch(pool.s2)
    pool.src1 = src1
    pool.src3 = src3
    pool.src2 = src2
    pool.nslots = int(c)
    pool.slot_expert |> resize(int(c))
    for (i in range(int(c))) {
        pool.slot_expert[i] = -1l
    }
    g_gpu.resident_bytes += c * per_slot
    to_log(LOG_INFO, "dasLLAMA vulkan tier: heat cache layer {l}: {c} slots ({c * per_slot} bytes)\n")
    g_gpu.heat_pools[l] <- pool
}

// one plane slice, mirror -> pool slot (quants + scales; per-row bytes are uniform by layout)
def private heat_record_slice(raw : VkCommandBuffer; src_si, dst_si : int; e, slot, rowlen, ege : int64) {
    let rpe = ege / rowlen
    let rows_src = g_gpu.stream_stacks[src_si].elems / rowlen
    let rbq = g_gpu.stream_stacks[src_si].wqbytes / rows_src
    let rbs = g_gpu.stream_stacks[src_si].wsbytes / rows_src
    cmd_copy_range(raw, g_gpu.stream_stacks[src_si].hwq.buf, e * rpe * rbq,
        g_gpu.stacks[dst_si].wqbuf, slot * rpe * rbq, rpe * rbq)
    if (rbs > 0l) {   // q8n mirrors carry no scale plane
        cmd_copy_range(raw, g_gpu.stream_stacks[src_si].hws.buf, e * rpe * rbs,
            g_gpu.stacks[dst_si].wsbuf, slot * rpe * rbs, rpe * rbs)
    }
}

//! MoeGpuHeatAdviseFn: reconcile layer l's pool toward the desired hot set (ids heat-descending).
//! Pool creation is lazy (budget-clamped C); uploads are capped per call so a re-pin spreads
//! across advise ticks instead of stalling one token.
def vk_moe_heat_advise(l : int64; b1, b3, b2 : int64; f1, f3, f2 : int; n, nfe : int64; ids : int64 const?; nids : int64) {
    if (g_gpu == null) {
        return
    }
    if (!key_exists(g_gpu.heat_pools, l)) {
        heat_pool_make(l, b1, b3, b2, f1, f3, f2, n, nfe)
    }
    var pool & = unsafe(g_gpu.heat_pools[l])
    if (pool.nslots <= 0) {
        return
    }
    let nwant = min(nids, int64(pool.nslots))
    var keep : table<int64>
    for (i in range64(nwant)) {
        keep |> insert(unsafe(ids[i]))
    }
    var pinned = 0
    for (s in range(pool.nslots)) {
        let e = pool.slot_expert[s]
        if (e >= 0l && !key_exists(keep, e)) {
            pool.emap |> erase(e)
            pool.slot_expert[s] = -1l
        } elif (e >= 0l) {
            pinned++
        }
    }
    delete keep
    let cap = pinned * 2 < pool.nslots ? HEAT_UPLOAD_CAP_FILL : HEAT_UPLOAD_CAP
    var up = 0
    var free_s = 0
    var raw = g_gpu.batch_cmd
    for (i in range64(nwant)) {
        break if (up >= cap)
        let e = unsafe(ids[i])
        continue if (key_exists(pool.emap, e))
        while (free_s < pool.nslots && pool.slot_expert[free_s] >= 0l) {
            free_s++
        }
        break if (free_s >= pool.nslots)
        if (up == 0) {
            let rf : VkCommandBufferResetFlags
            vk_check(vkResetCommandBuffer(raw, rf), null)
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
        }
        heat_record_slice(raw, pool.src1, pool.s1, e, int64(free_s), n, pool.ege)
        heat_record_slice(raw, pool.src3, pool.s3, e, int64(free_s), n, pool.ege)
        heat_record_slice(raw, pool.src2, pool.s2, e, int64(free_s), nfe, pool.ege)
        pool.emap |> insert(e, free_s)
        pool.slot_expert[free_s] = e
        up++
    }
    if (up > 0) {
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
    }
}

//! MoeGpuHeatQueryFn: partition the routed top-k against layer l's pinned slots — hit flags per
//! original position, compacted synthetic offset triples for the hits.
def vk_moe_heat_query(l : int64; idx : int64 const?; k : int64; var hit : int8?; var o1 : int64?; var o3 : int64?; var o2 : int64?) : int64 {
    if (g_gpu == null || !key_exists(g_gpu.heat_pools, l)) {
        return 0l
    }
    let pool & = unsafe(g_gpu.heat_pools[l])
    if (pool.nslots <= 0) {
        return 0l
    }
    var nh = 0l
    unsafe {
        for (j in range64(k)) {
            let sl = pool.emap?[idx[j]] ?? -1
            hit[j] = int8(sl >= 0 ? 1 : 0)
            if (sl >= 0) {
                let so = int64(sl) * pool.ege
                o1[nh] = g_gpu.stacks[pool.s1].base + so
                o3[nh] = g_gpu.stacks[pool.s3].base + so
                o2[nh] = g_gpu.stacks[pool.s2].base + so
                nh++
            }
        }
    }
    return nh
}

// ===== streamed-prefill dispatch (pinned-mirror layers) =====

def private ensure_stream_slots {
    if (g_gpu.stream_ready) {
        return
    }
    ensure_batch_state()
    g_gpu.stream_cmd = alloc_cmd()
    g_gpu.stream_slots |> reserve(2)
    for (_slot in range(2)) {
        let s1 = make_stack_shell(-1l, 0l, 0, g_gpu.stream_max_gu_wq, g_gpu.stream_max_gu_ws, [xfer_shared = true])
        let s3 = make_stack_shell(-1l, 0l, 0, g_gpu.stream_max_gu_wq, g_gpu.stream_max_gu_ws, [xfer_shared = true])
        let s2 = make_stack_shell(-1l, 0l, 0, g_gpu.stream_max_dn_wq, g_gpu.stream_max_dn_ws, [xfer_shared = true])
        g_gpu.stream_slots |> push(StreamSlot(group = -1, s1 = s1, s3 = s3, s2 = s2))
    }
    g_gpu.stream_ready = true
    let slot_bytes = 2l * (g_gpu.stream_max_gu_wq + g_gpu.stream_max_gu_ws) + g_gpu.stream_max_dn_wq + g_gpu.stream_max_dn_ws
    // reserve was carved from the weight budget at init — overcommit here silently hits the WDDM paging cliff
    let reserve = g_gpu.stream_reserve > 0l ? g_gpu.stream_reserve : STREAM_RESERVE
    assert(2l * slot_bytes <= reserve, "dasLLAMA vulkan tier: stream slots exceed the carved stream reserve")
    to_log(LOG_INFO, "dasLLAMA vulkan tier: streamed prefill engaged ({length(g_gpu.stream_stacks) / 3} layers, slot {slot_bytes} bytes x 2)\n")
}

// re-base one slot pseudo-stack onto a stream stack (descriptors stay — the slot buffers are
// the same; only the meta-resolution identity and the kernel choice follow the layer)
def private rebase_pseudo(si : int; ss : StreamStack const) {
    g_gpu.stacks[si].base = ss.base
    g_gpu.stacks[si].elems = ss.elems
    g_gpu.stacks[si].fmt = ss.fmt
}

// host-wait the transfer timeline up to `v` — the backstop before re-recording the shared xfer
// cmd, and the sync (cold-miss) completion wait
def private xfer_host_wait(v : uint64) {
    var wi = VkSemaphoreWaitInfo()
    var sem = g_gpu.xfer_sem
    var val = v
    unsafe {
        wi.semaphoreCount = 1u
        wi.pSemaphores = addr(sem)
        wi.pValues = addr(val)
        vk_check(vkWaitSemaphores(dev_raw(), wi, 0xFFFFFFFFFFFFFFFFul), null)
    }
}

// Transfer family armed: copies ride the dedicated queue signaling the timeline (true overlap;
// the slot's consumer attaches the GPU-side wait — see vk_moe_ffn_streamed). Single-queue mode
// keeps the old unfenced rail. Shared cmds are safe: a wait always intervenes between recordings.
def private copy_stream_group(si, g : int; sync : bool) {
    let slot = g_gpu.stream_slots[si]
    let xfer = g_gpu.xfer_fam >= 0
    if (xfer && g_gpu.xfer_value > 0ul) {
        xfer_host_wait(g_gpu.xfer_value)   // every prior transfer-queue copy retired
    }
    var raw = xfer ? g_gpu.xfer_cmd : g_gpu.stream_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    for (k in range(3)) {
        let sk = g * 3 + k
        let psi = k == 0 ? slot.s1 : (k == 1 ? slot.s3 : slot.s2)
        cmd_copy_whole(raw, g_gpu.stream_stacks[sk].hwq.buf, g_gpu.stacks[psi].wqbuf, g_gpu.stream_stacks[sk].wqbytes)
        if (g_gpu.stream_stacks[sk].wsbytes > 0l) {   // q8n mirrors carry no scale plane
            cmd_copy_whole(raw, g_gpu.stream_stacks[sk].hws.buf, g_gpu.stacks[psi].wsbuf, g_gpu.stream_stacks[sk].wsbytes)
        }
    }
    vk_check(vkEndCommandBuffer(raw), null)
    if (xfer) {
        g_gpu.xfer_value++
        var sig = g_gpu.xfer_value
        var tss = VkTimelineSemaphoreSubmitInfo()
        var sem = g_gpu.xfer_sem
        var submit = VkSubmitInfo()
        submit.commandBufferCount = 1u
        submit.signalSemaphoreCount = 1u
        let nofence : VkFence
        unsafe {
            tss.signalSemaphoreValueCount = 1u
            tss.pSignalSemaphoreValues = addr(sig)
            submit.pNext = addr<void?>(tss)
            submit.pCommandBuffers = addr(raw)
            submit.pSignalSemaphores = addr(sem)
            vk_check(vkQueueSubmit(g_gpu.xfer_queue, 1u, addr(submit), nofence), null)
        }
        if (sync) {
            xfer_host_wait(g_gpu.xfer_value)
        }
        // the consumer still attaches the GPU-side wait — cross-queue visibility by the book
        g_gpu.stream_slots[si].wait_value = g_gpu.xfer_value
    } elif (sync) {
        submit_wait(raw)
    } else {
        var submit = VkSubmitInfo()
        submit.commandBufferCount = 1u
        let nofence : VkFence
        unsafe {
            submit.pCommandBuffers = addr(raw)
            vk_check(vkQueueSubmit(g_gpu.queue, 1u, addr(submit), nofence), null)
        }
    }
    g_gpu.stream_slots[si].group = g
}

// prefetch miss cold-copies the slot; prefetch of the next group runs descending while prefill runs ascending
def private vk_moe_ffn_streamed(g : int; var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    assert(nrows >= BATCH_TILE, "dasLLAMA vulkan tier: streamed layers serve prefill batches only")
    assert(g_gpu.stream_stacks[g * 3 + 1].fmt == f3 && g_gpu.stream_stacks[g * 3 + 2].fmt == f2,
        "dasLLAMA vulkan tier: streamed group format mismatch")
    ensure_stream_slots()
    var si = -1
    for (i in range(2)) {
        if (g_gpu.stream_slots[i].group == g) {
            si = i
        }
    }
    if (si < 0) {
        si = g % 2
        copy_stream_group(si, g, true)
    }
    rebase_pseudo(g_gpu.stream_slots[si].s1, g_gpu.stream_stacks[g * 3])
    rebase_pseudo(g_gpu.stream_slots[si].s3, g_gpu.stream_stacks[g * 3 + 1])
    rebase_pseudo(g_gpu.stream_slots[si].s2, g_gpu.stream_stacks[g * 3 + 2])
    // the slot's transfer-queue fill hands off here: the next compute submit waits the timeline
    if (g_gpu.stream_slots[si].wait_value > 0ul) {
        g_gpu.pend_wait = g_gpu.stream_slots[si].wait_value
        g_gpu.stream_slots[si].wait_value = 0ul
    }
    vk_moe_ffn_batch(goutp, offs1, offs3, offs2, nregions, xqp, xsp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    if (g > 0 && g_gpu.stream_slots[(g - 1) % 2].group != g - 1) {
        copy_stream_group((g - 1) % 2, g - 1, false)
    }
}

//! MoeGpuFfnFn: routed experts' whole FFN chain (gate+up GEMMs, activation, requant, down GEMM)
//! as device-side submit chains over resident stacks — hidden activations, and (npos > 0) the
//! combined output, never cross PCIe. Streamed layers route through the slot rail; else panics.
def vk_moe_ffn(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    ensure_ffn_state()
    if (find_stack_idx(unsafe(offs1[0]), f1) < 0) {
        let w0 = unsafe(offs1[0])
        var g = -1
        for (i in range(length(g_gpu.stream_stacks) / 3)) {
            if (g_gpu.stream_stacks[i * 3].fmt == f1 && w0 >= g_gpu.stream_stacks[i * 3].base
                    && w0 < g_gpu.stream_stacks[i * 3].base + g_gpu.stream_stacks[i * 3].elems) {
                g = i
            }
        }
        if (g < 0) {
            panic("dasLLAMA vulkan tier: dispatch against a region that is neither resident nor streamed (fmt {f1} element offset {w0})")
        }
        vk_moe_ffn_streamed(g, goutp, offs1, offs3, offs2, nregions, xqp, xsp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
        return
    }
    if (nrows < BATCH_TILE) {
        assert(npos == 0l, "dasLLAMA vulkan tier: combined output needs the batch arm")
        vk_moe_ffn_gemv(goutp, offs1, offs3, offs2, nregions, xqp, xsp, n, nfe, nrows, f1, f3, f2, is_gelu)
    } else {
        vk_moe_ffn_batch(goutp, offs1, offs3, offs2, nregions, xqp, xsp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    }
}

// ===== classifier dispatch (the head GEMV) =====

// One-time dispatch state for a resident classifier plane: the meta never changes (ONE region
// covering the whole plane, activations at image row 0), so it uploads device-local once and
// the command buffer records once — per token only the activation refresh and the submit remain.
def private ensure_cls(si : int; woff, n, d : int64) {
    if (g_gpu.stacks[si].cls_made) {
        return
    }
    ensure_gemv_scratch(si)
    let unit = g_gpu.stacks[si].fmt != 0 ? 256l : 32l
    let rel = woff - g_gpu.stacks[si].base
    assert(rel % unit == 0l, "dasLLAMA vulkan tier: unaligned classifier plane offset")
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.staging.mapped)
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = 1u
        mp[3] = 0u
        mp[4] = uint(rel / unit)
        mp[5] = 0u
    }
    g_gpu.stacks[si].cls_meta_dev = make_device_buf(CLS_META_BYTES)
    staged_upload(g_gpu.stacks[si].cls_meta_dev, 0l, CLS_META_BYTES)
    g_gpu.stacks[si].cls_set = alloc_desc_set()
    var writes : array<WriteDescriptorSet>
    write_buf_desc(writes, g_gpu.stacks[si].cls_set, 0u, g_gpu.stacks[si].wqbuf, g_gpu.stacks[si].wqbytes)
    write_buf_desc(writes, g_gpu.stacks[si].cls_set, 1u, g_gpu.stacks[si].wsbuf, g_gpu.stacks[si].wsbytes)
    write_buf_desc(writes, g_gpu.stacks[si].cls_set, 2u, g_gpu.stacks[si].cls_meta_dev, CLS_META_BYTES)
    write_buf_desc(writes, g_gpu.stacks[si].cls_set, 3u, g_gpu.stacks[si].xq.buf, XQ_BYTES)
    write_buf_desc(writes, g_gpu.stacks[si].cls_set, 4u, g_gpu.stacks[si].xs.buf, XS_BYTES)
    write_buf_desc(writes, g_gpu.stacks[si].cls_set, 5u, g_gpu.stacks[si].y_dev, Y_BYTES)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    var raw = alloc_cmd()
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    hz_set_bits(g_gpu.stacks[si].cls_set, 0u, 0u, 0u, 0u, 0u, VHG_YO)
    var h : VkHaz
    hz_dispatch(raw, h, gemv_pipe(g_gpu.stacks[si].fmt), g_gpu.stacks[si].cls_set,
        uint((d + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg))
    vhz_dep(raw, h, VHG_YO, 0u, true)
    cmd_copy_whole(raw, g_gpu.stacks[si].y_dev, g_gpu.stacks[si].y.buf, d * 4l)
    vk_check(vkEndCommandBuffer(raw), null)
    g_gpu.stacks[si].cls_cmd = raw
    g_gpu.stacks[si].cls_made = true
}

//! MoeGpuClsFn: the classifier GEMV — y[d] = W · x over the resident plane at woff, activation
//! in the plane's own quant form (Q8_0 per-32 / Q8_K per-256), result DMA'd to the host-cached
//! staging in the same submit.
def vk_moe_cls(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d : int64; fmt : int) {
    let si = find_stack(woff, fmt)
    assert(d * 4l <= Y_BYTES, "dasLLAMA vulkan tier: classifier vocab exceeds output capacity")
    ensure_cls(si, woff, n, d)
    fill_stack_acts(si, xqp, xsp, n)
    submit_wait(g_gpu.stacks[si].cls_cmd)
    unsafe {
        memcpy(reinterpret<void?>(yp), g_gpu.stacks[si].y.mapped, int(d * 4l))
    }
}

// ===== registration =====

var private g_vk_probed = false
var private g_vk_available = false
var private g_vk_why = ""   // why the device probe said no — discarded before, which made it silent
var private g_vk_arm_said = -1   // last logged arm outcome; -1 = nothing said yet

def private vulkan_backend_available : bool {
    if (g_vk_probed) {
        return g_vk_available
    }
    g_vk_probed = true
    // raw calls, not boost creators — those panic on failure instead of reporting absence
    if (volkInitialize() != 0) {
        g_vk_why = "no vulkan loader (volkInitialize failed)"
        return false
    }
    let ci = VkInstanceCreateInfo()
    var inst : VkInstance
    let cr = vkCreateInstance(ci, null, inst)
    if (cr != VkResult.SUCCESS) {
        g_vk_why = "vkCreateInstance refused ({cr})"
        return false
    }
    volkLoadInstance(inst)
    var count = 0u
    vkEnumeratePhysicalDevices(inst, count, null)
    vkDestroyInstance(inst, null)
    g_vk_available = count > 0u
    g_vk_why = g_vk_available ? "" : "vulkan loader present but 0 physical devices"
    return g_vk_available
}

[unused_argument(yp, wp, sp, xqp, xsp, n, d)]
def private vulkan_no_gemv(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    panic("dasLLAMA vulkan backend has no GEMV core — the MoE tier (DASLLAMA_GPU_MOE_LAYERS) is its only dispatch")
}

[unused_argument(yp, wp, sp, xqp, xsp, n, d, ntok)]
def private vulkan_no_batch(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    panic("dasLLAMA vulkan backend has no batch core — the MoE tier (DASLLAMA_GPU_MOE_LAYERS) is its only dispatch")
}

[unused_argument(y0p, y1p, y2p, wp, sp, woff0, woff1, woff2, xqp, xsp, n, d0, d1, d2)]
def private vulkan_no_group3(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?; woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    panic("dasLLAMA vulkan backend has no group3 core — the MoE tier (DASLLAMA_GPU_MOE_LAYERS) is its only dispatch")
}

[unused_argument(wp, sp, n, d)]
def private vulkan_repack_noop(var wp : int8?; var sp : float?; n, d : int64) {}

def private vk_resident_bytes : int64 => g_gpu != null ? g_gpu.resident_bytes : 0l

// The resident driver sizes a whole model against these BEFORE uploading anything. Probing the
// device here (rather than reporting 0 until something else arms it) is what lets the gate answer
// "would this fit?" on a cold tier.
def private vk_weight_budget : int64 => vk_moe_init() ? g_gpu.weight_budget : 0l

def private vk_plane_bytes(n, rows : int64; fmt : int) : int64 {
    let pb = stack_plane_bytes(n, rows, fmt)
    return pb.wq + pb.ws
}

// the loader's upload router asks this before gathering q8 expert stacks: mode 4 (cm2) wants
// them as fmt-6 q8n blocks (decode-in-load). Probes the device — the answer must precede the
// first gather.
def private vk_expert_q8n : bool => vk_moe_init() && g_gpu.coopmat_mode == 4

def private vk_device_name : string {
    if (g_gpu == null) {
        return ""
    }
    var props : VkPhysicalDeviceProperties
    vkGetPhysicalDeviceProperties(g_gpu.phys, props)
    unsafe {
        return "{reinterpret<string>(addr(props.deviceName[0]))}"
    }
}

// vulkan_moe_gpu_arm is idempotent and re-runs on every re-arm, so log only on OUTCOME CHANGE.
// An unconditional line here floods the log (14k lines in one GLM run) and gets tuned out.
def private arm_say(outcome : int; msg : string) {
    if (!dasllama_noisy() || g_vk_arm_said == outcome) {
        return
    }
    g_vk_arm_said = outcome
    to_log(LOG_INFO, "dasLLAMA noisy: {msg}\n")
}

//! Arm the vulkan MoE tier from the effective GPU want (env-else-programmatic). Registered as
//! the ``moe_gpu_tier_arm`` probe; ``[init]`` also runs it directly for the env-only path.
//! Idempotent — re-arming after ``set_gpu_tier_want`` just refreshes the layer count.
def vulkan_moe_gpu_arm : bool {
    let layers = gpu_want_moe_layers()
    // counts test == 0, NOT <= 0: -1 is the auto sentinel and must read as a REQUEST, or an
    // auto-sized-layers-only want would silently fail to arm
    let nothing_wanted = (layers == 0l && gpu_want_moe_stream() == 0l && !gpu_want_dn() && !gpu_want_attn() && !gpu_want_dnd()
        && gpu_want_heat() <= 0l && !gpu_want_shexp() && !gpu_want_qkv() && !gpu_want_cls())
    if (nothing_wanted) {   // checked first — the device probe is not free
        arm_say(0, "GPU tier NOT armed — no rail requested (set DASLLAMA_GPU=1 or call set_gpu_tier_want)")
        return false
    }
    if (!vulkan_backend_available()) {
        arm_say(2, "GPU tier NOT armed — {g_vk_why}")
        return false
    }
    set_moe_gpu_layer_count(layers)
    if (!moe_gpu_tier_installed()) {
        install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
    }
    arm_say(1, "GPU tier armed — vulkan, moe layers {layers}{layers < 0l ? " (auto)" : ""}, stream {gpu_want_moe_stream()}")
    return true
}

// the vulkan flavor's config identity. ENV-OR-AUTO knobs only — resolving them (vk_moe_init)
// would carve the stream reserve BEFORE the loader reports the model's slot need (518ae164a);
// "auto" resolves deterministically per box and the image is local, so the identity is stable.
def private vk_bake_tag : string {
    if (!vulkan_moe_gpu_arm()) {
        return ""
    }
    let cm = get_env_variable("DASLLAMA_COOPMAT")
    let vb = get_env_variable("DASLLAMA_GPU_VRAM_MB")
    return ("m{cm == "" ? "auto" : cm} b{vb == "" ? "auto" : vb}"
        + " l{gpu_want_moe_layers()} s{gpu_want_moe_stream()}"
        + " r{gpu_want_dn() ? 1 : 0}{gpu_want_attn() ? 1 : 0}{gpu_want_dnd() ? 1 : 0}{gpu_want_shexp() ? 1 : 0}{gpu_want_qkv() ? 1 : 0}{gpu_want_cls() ? 1 : 0}")
}

[init]
def dasllama_math_vulkan_register() {
    register_kernel_backend(KernelBackend(name = "vulkan", mm = @@vulkan_no_gemv, batch = @@vulkan_no_batch,
        group3 = @@vulkan_no_group3, repack = @@vulkan_repack_noop,
        needs_repack = false, priority = -1, available = @@vulkan_backend_available))
    set_moe_gpu_arm("vulkan", @@vulkan_moe_gpu_arm)
    set_moe_gpu_bake_tag(@@vk_bake_tag)
    set_moe_gpu_vram_report(@@vk_resident_bytes)
    set_moe_gpu_device_report(@@vk_device_name)
    set_moe_gpu_budget_hooks(@@vk_weight_budget, @@vk_plane_bytes)
    set_moe_gpu_expert_q8n_probe(@@vk_expert_q8n)
    install_moe_gpu_resident(@@vk_arena_reserve, @@vk_arena_place, @@vk_rdec_prepare,
        @@vk_rdec_upload_norms, @@vk_rdec_set_layer, @@vk_rdec_set_cls, @@vk_rdec_token, @@vk_rdec_prefill,
        @@vk_rdec_sync_kv, @@vk_rdec_read_kv, @@vk_rdec_read_kv_bulk)
    set_moe_gpu_dn_state_hooks(@@vk_dn_step_flush, @@vk_dn_step_invalidate)
    set_moe_gpu_heat_hooks(@@vk_moe_heat_query, @@vk_moe_heat_advise, @@vk_moe_ffn_begin, @@vk_moe_ffn_join)
    set_moe_gpu_qkv_hook(@@vk_moe_qkv)
    set_moe_gpu_drop_model_hook(@@vk_drop_model_state)
    vulkan_moe_gpu_arm()
}
