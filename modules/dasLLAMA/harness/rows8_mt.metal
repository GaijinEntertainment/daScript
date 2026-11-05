#include <metal_stdlib>
#include <metal_tensor>
#include <MetalPerformancePrimitives/MetalPerformancePrimitives.h>
using namespace metal;
using namespace mpp::tensor_ops;

// rows8 matrix-unit Q4_K GEMM prototype: Y^T[n][t] = W[n][:] . X^T[:][t] on Metal 4 matmul2d, 16 weight rows x 8 token
// rows x 32 k (one Q4_K sub-block) a product. The left operand is a cooperative tensor filled from registers with the raw
// uint8 codes (exact integers into the matrix unit), the right operand the x table as a device half tensor, and the
// sub-block's scale and min apply once in an fp32 epilogue: y += dsc dot - dmn sum_x.
//
// This box's cooperative layouts (rows8_tensor_probe, index order (k, row)):
//   left, 16 elements a lane: rows m0, m0 + 8 with m0 = ((lane >> 1) & 3) + 4 ((lane >> 4) & 1), k-slots 4 kq + i and
//   16 + 4 kq + i (i < 4) with kq = (lane & 1) + 2 ((lane >> 3) & 1); element order: 4 k of row m0, the same 4 of m0 + 8,
//   then the 16 + 4 kq quad of each row.
//   destination, 8 elements a lane: tokens 4 kq + i of row m0, then of row m0 + 8; lanes with bit 3 set hold the pad
//   columns 8..15.
// The x table carries logical k-slot 4 kq + i at physical byte 8 kq + i and 16 + 4 kq + i at 8 kq + 4 + i of the
// sub-block, so a lane's 8 codes of a row are one 8-byte load.

kernel void k4_rows8_mt(device const uint4* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],   // NOLINT
                        device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                        device const float* xs [[buffer(7)]],
                        uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    const uint nsb = K / 256u;
#if defined(KS) || defined(SG1)
    const uint n0 = tgid.x * 16u;
#else
    const uint n0 = tgid.x * 64u + sgid * 16u;
#endif
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const bool pad = ((lane >> 3) & 1u) != 0u;
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
#ifdef KS
    const uint per = (nsb + KS - 1u) / KS;
    const uint sbeg = min(sgid * per, nsb);
    const uint send = min(sbeg + per, nsb);
#else
    const uint per = (nsb + NSK - 1u) / NSK;
    const uint sbeg = min(tgid.z * per, nsb);
    const uint send = min(sbeg + per, nsb);
#endif
    constexpr auto desc = matmul2d_descriptor(16, 8, 32, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<uint8_t, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 8));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    const uint tq = pad ? 0u : 4u * kq;   // the lane's token quad (a pad lane reads token 0's sums and stores nothing)
    for (uint sb = sbeg; sb < send; ++sb) {
        const uint blka = ra * nsb + sb;
        const uint blkb = rb * nsb + sb;
        const uint4 sva = sc[blka];
        const uint4 svb = sc[blkb];
        const half2 dda = as_type<half2>(sva.x);
        const half2 ddb = as_type<half2>(svb.x);
        const float da = float(dda.x), dmina = float(dda.y), db = float(ddb.x), dminb = float(ddb.y);
        const uint scla = sva.y & 0x3F3F3F3Fu, scha = (sva.w & 0x0F0F0F0Fu) | ((sva.y >> 2) & 0x30303030u);
        const uint mnla = sva.z & 0x3F3F3F3Fu, mnha = ((sva.w >> 4) & 0x0F0F0F0Fu) | ((sva.z >> 2) & 0x30303030u);
        const uint sclb = svb.y & 0x3F3F3F3Fu, schb = (svb.w & 0x0F0F0F0Fu) | ((svb.y >> 2) & 0x30303030u);
        const uint mnlb = svb.z & 0x3F3F3F3Fu, mnhb = ((svb.w >> 4) & 0x0F0F0F0Fu) | ((svb.z >> 2) & 0x30303030u);
        const device uint* qa = qs + blka * 32u + 2u * kq;
        const device uint* qb = qs + blkb * 32u + 2u * kq;
#ifdef PF
        uint2 wa_n = *(const device uint2*)(qa);
        uint2 wb_n = *(const device uint2*)(qb);
#endif
        for (uint j = 0; j < 4; ++j) {
#ifdef OPONLY
            const uint2 wa = uint2(lane * 0x01010101u + j, lane ^ 0x5A5A5A5Au);
            const uint2 wb = uint2(lane * 0x10101010u + j, lane ^ 0xA5A5A5A5u);
#elif defined(PF)
            const uint2 wa = wa_n;
            const uint2 wb = wb_n;
            if (j < 3u) {
                wa_n = *(const device uint2*)(qa + 8u * (j + 1u));
                wb_n = *(const device uint2*)(qb + 8u * (j + 1u));
            }
#else
            const uint2 wa = *(const device uint2*)(qa + 8u * j);
            const uint2 wb = *(const device uint2*)(qb + 8u * j);
#endif
            for (uint hi = 0; hi < 2; ++hi) {
                const uint s = 2u * j + hi;
                const uint ua0 = (wa.x >> (4u * hi)) & 0x0F0F0F0Fu;
                const uint ua1 = (wa.y >> (4u * hi)) & 0x0F0F0F0Fu;
                const uint ub0 = (wb.x >> (4u * hi)) & 0x0F0F0F0Fu;
                const uint ub1 = (wb.y >> (4u * hi)) & 0x0F0F0F0Fu;
                for (uint i = 0; i < 4; ++i) {
                    Lw[i] = uchar(ua0 >> (8u * i));
                    Lw[4u + i] = uchar(ub0 >> (8u * i));
                    Lw[8u + i] = uchar(ua1 >> (8u * i));
                    Lw[12u + i] = uchar(ub1 >> (8u * i));
                }
                auto Rx = tX.template slice<32, 8>(int((sb * 8u + s) * 32u), 0);
                auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
#ifdef LOADONLY
                for (uint i = 0; i < 8; ++i) {
                    d[i] = float(Lw[i]) + float(Lw[8u + i]);
                }
#else
                op.run(Lw, Rx, d);
#endif
                const uint sh = 8u * (s & 3u);
                const float dsca = da * float(((s < 4u ? scla : scha) >> sh) & 0xFFu);
                const float dmna = dmina * float(((s < 4u ? mnla : mnha) >> sh) & 0xFFu);
                const float dscb = db * float(((s < 4u ? sclb : schb) >> sh) & 0xFFu);
                const float dmnb = dminb * float(((s < 4u ? mnlb : mnhb) >> sh) & 0xFFu);
                const float4 sum = *(const device float4*)(xs + (sb * 8u + s) * 8u + tq);
                for (uint i = 0; i < 4; ++i) {
                    acc[i] = fma(dsca, d[i], fma(-dmna, sum[i], acc[i]));
                    acc[4u + i] = fma(dscb, d[4u + i], fma(-dmnb, sum[i], acc[4u + i]));
                }
            }
        }
    }
#ifdef KS
    threadgroup float red[(KS - 1) * 8 * 32];
    if (sgid != 0u) {
        for (uint i = 0; i < 8; ++i) {
            red[((sgid - 1u) * 8u + i) * 32u + lane] = acc[i];
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if (sgid != 0u) {
        return;
    }
    for (uint g = 1; g < KS; ++g) {
        for (uint i = 0; i < 8; ++i) {
            acc[i] += red[((g - 1u) * 8u + i) * 32u + lane];
        }
    }
#endif
    if (pad) {
        return;
    }
    device float* yp = y + tgid.z * 8u * N;
    for (uint i = 0; i < 4; ++i) {
        yp[(tq + i) * N + ra] = acc[i];
        yp[(tq + i) * N + rb] = acc[4u + i];
    }
}

// q8_0 twin: 34-byte blocks (f16 d, 32 int8) a 32-element group, row-major by output row. The codes enter as uint8
// q + 128 (one xor), the epilogue y += d (dot - 128 sum_x). The lane's 8 codes of a row sit at byte 2 + 8 kq of the block.
kernel void q8_rows8_mt(device const uchar* blob [[buffer(0)]], device const half* T [[buffer(2)]],
                        device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                        device const float* xs [[buffer(7)]],
                        uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    const uint nkb = K / 32u;
#ifdef KS
    const uint n0 = tgid.x * 16u;
    const uint per = (nkb + KS - 1u) / KS;
    const uint kbeg = min(sgid * per, nkb);
    const uint kend = min(kbeg + per, nkb);
#else
#ifdef SG1
    const uint n0 = tgid.x * 16u;
#else
    const uint n0 = tgid.x * 64u + sgid * 16u;
#endif
    const uint per = (nkb + NSK - 1u) / NSK;
    const uint kbeg = min(tgid.z * per, nkb);
    const uint kend = min(kbeg + per, nkb);
#endif
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const bool pad = ((lane >> 3) & 1u) != 0u;
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
    constexpr auto desc = matmul2d_descriptor(16, 8, 32, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<uint8_t, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 8));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    const uint tq = pad ? 0u : 4u * kq;
    const device uchar* pa = blob + (ra * nkb) * 34u + 2u + 8u * kq;
    const device uchar* pb = blob + (rb * nkb) * 34u + 2u + 8u * kq;
    for (uint kb = kbeg; kb < kend; ++kb) {
        const device uchar* ba = pa + kb * 34u;
        const device uchar* bb = pb + kb * 34u;
#ifdef OPONLY
        // d shares its block's cache lines with the codes, so the op-only arm synthesizes it too
        const float da = float(kb & 7u) * 0.125f;
        const float db = float(lane & 7u) * 0.125f;
#else
        const float da = float(*(const device half*)(blob + (ra * nkb + kb) * 34u));
        const float db = float(*(const device half*)(blob + (rb * nkb + kb) * 34u));
#endif
#ifdef OPONLY
        const uchar4 a0 = as_type<uchar4>(lane * 0x01010101u + kb), a1 = as_type<uchar4>(lane ^ 0x5A5A5A5Au);
        const uchar4 b0 = as_type<uchar4>(lane * 0x10101010u + kb), b1 = as_type<uchar4>(lane ^ 0xA5A5A5A5u);
#else
        const packed_uchar4 a0 = *(const device packed_uchar4*)(ba);
        const packed_uchar4 a1 = *(const device packed_uchar4*)(ba + 4u);
        const packed_uchar4 b0 = *(const device packed_uchar4*)(bb);
        const packed_uchar4 b1 = *(const device packed_uchar4*)(bb + 4u);
#endif
#ifdef LOADSONLY
        acc[0] += da * float(as_type<uint>(uchar4(a0)) ^ as_type<uint>(uchar4(a1)));
        acc[4] += db * float(as_type<uint>(uchar4(b0)) ^ as_type<uint>(uchar4(b1)));
        continue;
#endif
        for (uint i = 0; i < 4; ++i) {
            Lw[i] = a0[i] ^ 0x80u;
            Lw[4u + i] = b0[i] ^ 0x80u;
            Lw[8u + i] = a1[i] ^ 0x80u;
            Lw[12u + i] = b1[i] ^ 0x80u;
        }
        auto Rx = tX.template slice<32, 8>(int(kb * 32u), 0);
        auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
#ifdef LOADONLY
        for (uint i = 0; i < 8; ++i) {
            d[i] = float(Lw[i]) + float(Lw[8u + i]);
        }
#else
        op.run(Lw, Rx, d);
#endif
        const float4 sum = *(const device float4*)(xs + kb * 8u + tq);
        for (uint i = 0; i < 4; ++i) {
            acc[i] = fma(da, d[i] - 128.0f * sum[i], acc[i]);
            acc[4u + i] = fma(db, d[4u + i] - 128.0f * sum[i], acc[4u + i]);
        }
    }
#ifdef KS
    threadgroup float red[(KS - 1) * 8 * 32];
    if (sgid != 0u) {
        for (uint i = 0; i < 8; ++i) {
            red[((sgid - 1u) * 8u + i) * 32u + lane] = acc[i];
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if (sgid != 0u) {
        return;
    }
    for (uint g = 1; g < KS; ++g) {
        for (uint i = 0; i < 8; ++i) {
            acc[i] += red[((g - 1u) * 8u + i) * 32u + lane];
        }
    }
#endif
    if (pad) {
        return;
    }
    device float* yp = y + tgid.z * 8u * N;
    for (uint i = 0; i < 4; ++i) {
        yp[(tq + i) * N + ra] = acc[i];
        yp[(tq + i) * N + rb] = acc[4u + i];
    }
}

// K4 on 64-deep products: the left operand is HALF, element 1024 + q sc (q the nibble, sc the sub-block's 6-bit scale: the
// product is at most 945, so 1024 + q sc is exact in half and its bits are 0x6400 + q sc), one product spans a superblock's
// two sub-blocks j, and the epilogue applies the superblock's d once: y += d (dot - 1024 (sum_0 + sum_1)) - dmin (mn_0 sum_0
// + mn_1 sum_1). Layout (probe, K = 64): 32 elements a lane, quads at k 4 kq, 16 + 4 kq, 32 + 4 kq, 48 + 4 kq, each for
// rows m0 then m0 + 8; the low nibbles are the group's first sub-block (k < 32), the high ones the second.
kernel void k4_rows8_mt64(device const uint4* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                          device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                          device const float* xs [[buffer(7)]],
                          uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    const uint nsb = K / 256u;
#ifdef SG1
    const uint n0 = tgid.x * 16u;
#else
    const uint n0 = tgid.x * 64u + sgid * 16u;
#endif
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const bool pad = ((lane >> 3) & 1u) != 0u;
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
    const uint per = (nsb + NSK - 1u) / NSK;
    const uint sbeg = min(tgid.z * per, nsb);
    const uint send = min(sbeg + per, nsb);
    constexpr auto desc = matmul2d_descriptor(16, 8, 64, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<half, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 8));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    const uint tq = pad ? 0u : 4u * kq;
    for (uint sb = sbeg; sb < send; ++sb) {
        const uint blka = ra * nsb + sb;
        const uint blkb = rb * nsb + sb;
        const uint4 sva = sc[blka];
        const uint4 svb = sc[blkb];
        const half2 dda = as_type<half2>(sva.x);
        const half2 ddb = as_type<half2>(svb.x);
        const float da = float(dda.x), dmina = float(dda.y), db = float(ddb.x), dminb = float(ddb.y);
        const uint scla = sva.y & 0x3F3F3F3Fu, scha = (sva.w & 0x0F0F0F0Fu) | ((sva.y >> 2) & 0x30303030u);
        const uint mnla = sva.z & 0x3F3F3F3Fu, mnha = ((sva.w >> 4) & 0x0F0F0F0Fu) | ((sva.z >> 2) & 0x30303030u);
        const uint sclb = svb.y & 0x3F3F3F3Fu, schb = (svb.w & 0x0F0F0F0Fu) | ((svb.y >> 2) & 0x30303030u);
        const uint mnlb = svb.z & 0x3F3F3F3Fu, mnhb = ((svb.w >> 4) & 0x0F0F0F0Fu) | ((svb.z >> 2) & 0x30303030u);
        const device uint* qa = qs + blka * 32u + 2u * kq;
        const device uint* qb = qs + blkb * 32u + 2u * kq;
#ifdef PF
        uint2 wa_n = *(const device uint2*)(qa);
        uint2 wb_n = *(const device uint2*)(qb);
#endif
        _Pragma("clang loop unroll(full)")
        for (uint j = 0; j < 4; ++j) {
#if defined(OPONLY)
            const uint2 wa = uint2(lane * 0x01010101u + j, lane ^ 0x5A5A5A5Au);
            const uint2 wb = uint2(lane * 0x10101010u + j, lane ^ 0xA5A5A5A5u);
#elif defined(PF)
            const uint2 wa = wa_n;
            const uint2 wb = wb_n;
            const uint jn = j + 1u;
            const device uint* qan = jn < 4u ? qa + 8u * jn : qa + 32u * nsb;   // the next group: this superblock's next run, or the next superblock's first
            const device uint* qbn = jn < 4u ? qb + 8u * jn : qb + 32u * nsb;
            if (jn < 4u || sb + 1u < send) {
                wa_n = *(const device uint2*)(qan);
                wb_n = *(const device uint2*)(qbn);
            }
#else
            const uint2 wa = *(const device uint2*)(qa + 8u * j);
            const uint2 wb = *(const device uint2*)(qb + 8u * j);
#endif
            const uint s0 = 2u * j;
            const uint sh0 = 8u * (s0 & 3u), sh1 = 8u * ((s0 + 1u) & 3u);
            const uint sca0 = ((s0 < 4u ? scla : scha) >> sh0) & 0xFFu, sca1 = ((s0 + 1u < 4u ? scla : scha) >> sh1) & 0xFFu;
            const uint scb0 = ((s0 < 4u ? sclb : schb) >> sh0) & 0xFFu, scb1 = ((s0 + 1u < 4u ? sclb : schb) >> sh1) & 0xFFu;
            const float mna0 = float(((s0 < 4u ? mnla : mnha) >> sh0) & 0xFFu), mna1 = float(((s0 + 1u < 4u ? mnla : mnha) >> sh1) & 0xFFu);
            const float mnb0 = float(((s0 < 4u ? mnlb : mnhb) >> sh0) & 0xFFu), mnb1 = float(((s0 + 1u < 4u ? mnlb : mnhb) >> sh1) & 0xFFu);
#ifdef LOADSONLY
            // the loads alone: the quant and scale words fold into the accumulator, nothing is filled or multiplied
            acc[j] += float(wa.x ^ wa.y ^ sva.x ^ sva.w);
            acc[4u + j] += float(wb.x ^ wb.y ^ svb.x ^ svb.w);
            continue;
#endif
            // quad q of the group: word (q & 1) of the lane's 8 bytes, nibble (q >> 1): q = 0 -> k 4 kq (lo of word 0), 1 -> 16 + 4 kq (lo of word 1), 2 -> 32 + 4 kq (hi of word 0), 3 -> 48 + 4 kq (hi of word 1)
            _Pragma("clang loop unroll(full)")
            for (uint q = 0; q < 4; ++q) {
                const uint wordA = (q & 1u) ? wa.y : wa.x;
                const uint wordB = (q & 1u) ? wb.y : wb.x;
                const uint nib = 4u * (q >> 1);
                const uint sA = (q >> 1) ? sca1 : sca0;
                const uint sB = (q >> 1) ? scb1 : scb0;
#ifdef SUBN
                const half2 ha0 = as_type<half2>(((wordA >> nib) & 0x000F000Fu) * sA);          // q sc as the subnormal half q sc 2^-24, exact
                const half2 ha1 = as_type<half2>(((wordA >> (nib + 8u)) & 0x000F000Fu) * sA);
                const half2 hb0 = as_type<half2>(((wordB >> nib) & 0x000F000Fu) * sB);
                const half2 hb1 = as_type<half2>(((wordB >> (nib + 8u)) & 0x000F000Fu) * sB);
#else
                const half2 ha0 = as_type<half2>((((wordA >> nib) & 0x000F000Fu) * sA) | 0x64006400u);          // codes 0, 2
                const half2 ha1 = as_type<half2>((((wordA >> (nib + 8u)) & 0x000F000Fu) * sA) | 0x64006400u);   // codes 1, 3
                const half2 hb0 = as_type<half2>((((wordB >> nib) & 0x000F000Fu) * sB) | 0x64006400u);
                const half2 hb1 = as_type<half2>((((wordB >> (nib + 8u)) & 0x000F000Fu) * sB) | 0x64006400u);
#endif
                Lw[q * 8u + 0u] = ha0.x;
                Lw[q * 8u + 1u] = ha1.x;
                Lw[q * 8u + 2u] = ha0.y;
                Lw[q * 8u + 3u] = ha1.y;
                Lw[q * 8u + 4u] = hb0.x;
                Lw[q * 8u + 5u] = hb1.x;
                Lw[q * 8u + 6u] = hb0.y;
                Lw[q * 8u + 7u] = hb1.y;
            }
            auto Rx = tX.template slice<64, 8>(int((sb * 4u + j) * 64u), 0);
            auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
#ifdef LOADONLY
            for (uint i = 0; i < 8; ++i) {
                d[i] = float(Lw[i]) + float(Lw[8u + i]) + float(Lw[16u + i]) + float(Lw[24u + i]);
            }
#else
            op.run(Lw, Rx, d);
#endif
            const float4 sum0 = *(const device float4*)(xs + (sb * 8u + s0) * 8u + tq);
            const float4 sum1 = *(const device float4*)(xs + (sb * 8u + s0 + 1u) * 8u + tq);
#ifdef SUBN
            for (uint i = 0; i < 4; ++i) {
                acc[i] = fma(da * 16777216.0f, d[i], fma(-dmina, fma(mna0, sum0[i], mna1 * sum1[i]), acc[i]));
                acc[4u + i] = fma(db * 16777216.0f, d[4u + i], fma(-dminb, fma(mnb0, sum0[i], mnb1 * sum1[i]), acc[4u + i]));
            }
#else
            for (uint i = 0; i < 4; ++i) {
                const float sum01 = sum0[i] + sum1[i];
                acc[i] = fma(da, d[i] - 1024.0f * sum01, fma(-dmina, fma(mna0, sum0[i], mna1 * sum1[i]), acc[i]));
                acc[4u + i] = fma(db, d[4u + i] - 1024.0f * sum01, fma(-dminb, fma(mnb0, sum0[i], mnb1 * sum1[i]), acc[4u + i]));
            }
#endif
        }
    }
    if (pad) {
        return;
    }
    device float* yp = y + tgid.z * 8u * N;
    for (uint i = 0; i < 4; ++i) {
        yp[(tq + i) * N + ra] = acc[i];
        yp[(tq + i) * N + rb] = acc[4u + i];
    }
}

// The 32-deep twins below share k4_rows8_mt's layout: the probe gives the same lane map for uint8, int8 and half lefts at
// K = 32 (and K = 64 extends it by the next 32-group), so the x table's k-slot order and the lane's one 8-byte load of a
// row's two sub-blocks carry over unchanged.
inline void rows8_store(device float* y, uint z, uint N, uint tq, uint ra, uint rb, thread const float* acc) {
    device float* yp = y + z * 8u * N;
    for (uint i = 0; i < 4; ++i) {
        yp[(tq + i) * N + ra] = acc[i];
        yp[(tq + i) * N + rb] = acc[4u + i];
    }
}

// K4 on 32-deep products, one sub-block each, so sc and mn are uniform within a product. U8: the left is the raw code
// 0..15 as uint8 and y += d sc dot - dmin mn sum_x. Otherwise the left is half 1024 + q (bits 0x6400 | q) and y += d sc
// (dot - 1024 sum_x) - dmin mn sum_x; SUBN makes it the subnormal half q 2^-24 and scales the dot by 2^24.
#ifdef U8
#define K4_LT uint8_t
#else
#define K4_LT half
#endif
kernel void k4_rows8_32(device const uint4* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                        device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                        device const float* xs [[buffer(7)]],
                        uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    const uint nsb = K / 256u;
    const uint n0 = tgid.x * 64u + sgid * 16u;
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const bool pad = ((lane >> 3) & 1u) != 0u;
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
    const uint per = (nsb + NSK - 1u) / NSK;
    const uint sbeg = min(tgid.z * per, nsb);
    const uint send = min(sbeg + per, nsb);
    constexpr auto desc = matmul2d_descriptor(16, 8, 32, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<K4_LT, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 8));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    const uint tq = pad ? 0u : 4u * kq;
#if defined(U8)
    const float dscale = 1.0f;
#elif defined(SUBN)
    const float dscale = 16777216.0f;
#else
    const float dscale = 1.0f;
    const float bias = 1024.0f;
#endif
    for (uint sb = sbeg; sb < send; ++sb) {
        const uint blka = ra * nsb + sb;
        const uint blkb = rb * nsb + sb;
        const uint4 sva = sc[blka];
        const uint4 svb = sc[blkb];
        const half2 dda = as_type<half2>(sva.x);
        const half2 ddb = as_type<half2>(svb.x);
        const float da = float(dda.x) * dscale, dmina = float(dda.y), db = float(ddb.x) * dscale, dminb = float(ddb.y);
        const uint2 scma[2] = {uint2(sva.y & 0x3F3F3F3Fu, sva.z & 0x3F3F3F3Fu),
                               uint2((sva.w & 0x0F0F0F0Fu) | ((sva.y >> 2) & 0x30303030u), ((sva.w >> 4) & 0x0F0F0F0Fu) | ((sva.z >> 2) & 0x30303030u))};
        const uint2 scmb[2] = {uint2(svb.y & 0x3F3F3F3Fu, svb.z & 0x3F3F3F3Fu),
                               uint2((svb.w & 0x0F0F0F0Fu) | ((svb.y >> 2) & 0x30303030u), ((svb.w >> 4) & 0x0F0F0F0Fu) | ((svb.z >> 2) & 0x30303030u))};
        const device uint2* qa = (const device uint2*)(qs + blka * 32u + 2u * kq);
        const device uint2* qb = (const device uint2*)(qs + blkb * 32u + 2u * kq);
        _Pragma("clang loop unroll(full)")
        for (uint j = 0; j < 4; ++j) {
            const uint2 wa = qa[4u * j];
            const uint2 wb = qb[4u * j];
            _Pragma("clang loop unroll(full)")
            for (uint hi = 0; hi < 2; ++hi) {
                const uint s = 2u * j + hi;
                const uint nib = 4u * hi;
#ifdef U8
                const uchar4 a0 = as_type<uchar4>((wa.x >> nib) & 0x0F0F0F0Fu);
                const uchar4 a1 = as_type<uchar4>((wa.y >> nib) & 0x0F0F0F0Fu);
                const uchar4 b0 = as_type<uchar4>((wb.x >> nib) & 0x0F0F0F0Fu);
                const uchar4 b1 = as_type<uchar4>((wb.y >> nib) & 0x0F0F0F0Fu);
                for (uint i = 0; i < 4; ++i) {
                    Lw[i] = a0[i];
                    Lw[4u + i] = b0[i];
                    Lw[8u + i] = a1[i];
                    Lw[12u + i] = b1[i];
                }
#else
#ifdef SUBN
                const uint hb = 0u;
#else
                const uint hb = 0x64006400u;
#endif
                // word w's codes 0, 2 sit in its 0x000F000F lanes, codes 1, 3 one byte up
                const uint w4[4] = {wa.x, wb.x, wa.y, wb.y};   // element quads: row a lo, row b lo, row a hi, row b hi
                _Pragma("clang loop unroll(full)")
                for (uint g = 0; g < 4; ++g) {
                    const half2 h02 = as_type<half2>(((w4[g] >> nib) & 0x000F000Fu) | hb);
                    const half2 h13 = as_type<half2>(((w4[g] >> (nib + 8u)) & 0x000F000Fu) | hb);
                    Lw[4u * g + 0u] = h02.x;
                    Lw[4u * g + 1u] = h13.x;
                    Lw[4u * g + 2u] = h02.y;
                    Lw[4u * g + 3u] = h13.y;
                }
#endif
                auto Rx = tX.template slice<32, 8>(int((sb * 8u + s) * 32u), 0);
                auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
                op.run(Lw, Rx, d);
                const uint sh = 8u * (s & 3u);
                const uint2 pa = scma[s >> 2], pb = scmb[s >> 2];
                const float dsca = da * float((pa.x >> sh) & 0xFFu), dmna = dmina * float((pa.y >> sh) & 0xFFu);
                const float dscb = db * float((pb.x >> sh) & 0xFFu), dmnb = dminb * float((pb.y >> sh) & 0xFFu);
                const float4 sum = *(const device float4*)(xs + (sb * 8u + s) * 8u + tq);
                for (uint i = 0; i < 4; ++i) {
#if defined(U8) || defined(SUBN)
                    acc[i] = fma(dsca, d[i], fma(-dmna, sum[i], acc[i]));
                    acc[4u + i] = fma(dscb, d[4u + i], fma(-dmnb, sum[i], acc[4u + i]));
#else
                    acc[i] = fma(dsca, d[i] - bias * sum[i], fma(-dmna, sum[i], acc[i]));
                    acc[4u + i] = fma(dscb, d[4u + i] - bias * sum[i], fma(-dmnb, sum[i], acc[4u + i]));
#endif
                }
            }
        }
    }
    if (!pad) {
        rows8_store(y, tgid.z, N, tq, ra, rb, acc);
    }
}

// q8_0 on 32-deep products with the lane's 8 codes of a row as one 2-byte-aligned 8-byte load (the block's codes start at
// byte 2 of 34). Q8_LT selects the left: uint8 q + 128 (y += d (dot - 128 sum_x)), int8 q or half q (y += d dot).
#ifndef Q8_LT
#define Q8_LT uint8_t
#endif
kernel void q8_rows8_32(device const uchar* blob [[buffer(0)]], device const half* T [[buffer(2)]],
                        device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                        device const float* xs [[buffer(7)]],
                        uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    const uint nkb = K / 32u;
    const uint n0 = tgid.x * 64u + sgid * 16u;
    const uint per = (nkb + NSK - 1u) / NSK;
    const uint kbeg = min(tgid.z * per, nkb);
    const uint kend = min(kbeg + per, nkb);
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const bool pad = ((lane >> 3) & 1u) != 0u;
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
    constexpr auto desc = matmul2d_descriptor(16, 8, 32, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<Q8_LT, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 8));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    const uint tq = pad ? 0u : 4u * kq;
    const device ushort* pa = (const device ushort*)(blob + (ra * nkb) * 34u);
    const device ushort* pb = (const device ushort*)(blob + (rb * nkb) * 34u);
    for (uint kb = kbeg; kb < kend; ++kb) {
        const device ushort* ba = pa + kb * 17u;
        const device ushort* bb = pb + kb * 17u;
        const float da = float(as_type<half>(ba[0]));
        const float db = float(as_type<half>(bb[0]));
        const uint2 wa = as_type<uint2>(ushort4(*(const device packed_ushort4*)(ba + 1u + 4u * kq)));
        const uint2 wb = as_type<uint2>(ushort4(*(const device packed_ushort4*)(bb + 1u + 4u * kq)));
#if defined(Q8_HALF)
        const half4 ha0 = half4(as_type<char4>(wa.x)), ha1 = half4(as_type<char4>(wa.y));
        const half4 hb0 = half4(as_type<char4>(wb.x)), hb1 = half4(as_type<char4>(wb.y));
#elif defined(Q8_I8)
        const char4 ha0 = as_type<char4>(wa.x), ha1 = as_type<char4>(wa.y), hb0 = as_type<char4>(wb.x), hb1 = as_type<char4>(wb.y);
#else
        const uchar4 ha0 = as_type<uchar4>(wa.x ^ 0x80808080u), ha1 = as_type<uchar4>(wa.y ^ 0x80808080u);
        const uchar4 hb0 = as_type<uchar4>(wb.x ^ 0x80808080u), hb1 = as_type<uchar4>(wb.y ^ 0x80808080u);
#endif
        for (uint i = 0; i < 4; ++i) {
            Lw[i] = ha0[i];
            Lw[4u + i] = hb0[i];
            Lw[8u + i] = ha1[i];
            Lw[12u + i] = hb1[i];
        }
        auto Rx = tX.template slice<32, 8>(int(kb * 32u), 0);
        auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
        op.run(Lw, Rx, d);
#if defined(Q8_HALF) || defined(Q8_I8)
        for (uint i = 0; i < 4; ++i) {
            acc[i] = fma(da, d[i], acc[i]);
            acc[4u + i] = fma(db, d[4u + i], acc[4u + i]);
        }
#else
        const float4 sum = *(const device float4*)(xs + kb * 8u + tq);
        for (uint i = 0; i < 4; ++i) {
            acc[i] = fma(da, d[i] - 128.0f * sum[i], acc[i]);
            acc[4u + i] = fma(db, d[4u + i] - 128.0f * sum[i], acc[4u + i]);
        }
#endif
    }
    if (!pad) {
        rows8_store(y, tgid.z, N, tq, ra, rb, acc);
    }
}

// Block-diagonal 64-deep products: 16 rows x 16 columns x 64 k with a uint8 left, the right the block-diagonal x table
// Tb [16][K] (columns 0..7 the tokens' x on each 64-group's first 32 k and zero on the rest, columns 8..15 the reverse),
// so one run yields both 32-blocks' dots separately: a lane without bit 3 holds the first block's for tokens 4 kq + i,
// a lane with it the second's for tokens 4 (lane & 1) + i (probe, 16 x 16 x 64: the same lane map as 16 x 8, the former
// pad columns now real). Each lane scales its own block; the pair folds by a shuffle across bit 3 at the end.
kernel void k4_rows8_bd(device const uint4* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                        device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                        device const float* xs [[buffer(7)]],
                        uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    const uint nsb = K / 256u;
    const uint n0 = tgid.x * 64u + sgid * 16u;
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const uint hi = (lane >> 3) & 1u;
    const uint tq = 4u * (lane & 1u);
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
    const uint per = (nsb + NSK - 1u) / NSK;
    const uint sbeg = min(tgid.z * per, nsb);
    const uint send = min(sbeg + per, nsb);
    constexpr auto desc = matmul2d_descriptor(16, 16, 64, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<uint8_t, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 16));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    for (uint sb = sbeg; sb < send; ++sb) {
        const uint blka = ra * nsb + sb;
        const uint blkb = rb * nsb + sb;
        const uint4 sva = sc[blka];
        const uint4 svb = sc[blkb];
        const half2 dda = as_type<half2>(sva.x);
        const half2 ddb = as_type<half2>(svb.x);
        const float da = float(dda.x), dmina = float(dda.y), db = float(ddb.x), dminb = float(ddb.y);
        const uint2 scma[2] = {uint2(sva.y & 0x3F3F3F3Fu, sva.z & 0x3F3F3F3Fu),
                               uint2((sva.w & 0x0F0F0F0Fu) | ((sva.y >> 2) & 0x30303030u), ((sva.w >> 4) & 0x0F0F0F0Fu) | ((sva.z >> 2) & 0x30303030u))};
        const uint2 scmb[2] = {uint2(svb.y & 0x3F3F3F3Fu, svb.z & 0x3F3F3F3Fu),
                               uint2((svb.w & 0x0F0F0F0Fu) | ((svb.y >> 2) & 0x30303030u), ((svb.w >> 4) & 0x0F0F0F0Fu) | ((svb.z >> 2) & 0x30303030u))};
        const device uint2* qa = (const device uint2*)(qs + blka * 32u + 2u * kq);
        const device uint2* qb = (const device uint2*)(qs + blkb * 32u + 2u * kq);
        _Pragma("clang loop unroll(full)")
        for (uint j = 0; j < 4; ++j) {
            const uint2 wa = qa[4u * j];
            const uint2 wb = qb[4u * j];
            // quad q: k 4 kq (lo of word x), 16 + 4 kq (lo of y), 32 + 4 kq (hi of x), 48 + 4 kq (hi of y); rows a then b
            _Pragma("clang loop unroll(full)")
            for (uint q = 0; q < 4; ++q) {
                const uint nib = 4u * (q >> 1);
                const uchar4 ca = as_type<uchar4>((((q & 1u) ? wa.y : wa.x) >> nib) & 0x0F0F0F0Fu);
                const uchar4 cb = as_type<uchar4>((((q & 1u) ? wb.y : wb.x) >> nib) & 0x0F0F0F0Fu);
                for (uint i = 0; i < 4; ++i) {
                    Lw[8u * q + i] = ca[i];
                    Lw[8u * q + 4u + i] = cb[i];
                }
            }
            auto Rx = tX.template slice<64, 16>(int((sb * 4u + j) * 64u), 0);
            auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
            op.run(Lw, Rx, d);
            const uint s = 2u * j + hi;
            const uint sh = 8u * (s & 3u);
            const uint2 pa = scma[s >> 2], pb = scmb[s >> 2];
            const float dsca = da * float((pa.x >> sh) & 0xFFu), dmna = dmina * float((pa.y >> sh) & 0xFFu);
            const float dscb = db * float((pb.x >> sh) & 0xFFu), dmnb = dminb * float((pb.y >> sh) & 0xFFu);
            const float4 sum = *(const device float4*)(xs + (sb * 8u + s) * 8u + tq);
            for (uint i = 0; i < 4; ++i) {
                acc[i] = fma(dsca, d[i], fma(-dmna, sum[i], acc[i]));
                acc[4u + i] = fma(dscb, d[4u + i], fma(-dmnb, sum[i], acc[4u + i]));
            }
        }
    }
    for (uint i = 0; i < 8; ++i) {
        acc[i] += simd_shuffle_xor(acc[i], 8u);
    }
    if (hi == 0u) {
        rows8_store(y, tgid.z, N, tq, ra, rb, acc);
    }
}

// q8_0 twin of k4_rows8_bd: one run covers blocks kb, kb + 1 (a way's range is rounded to block pairs), left q + 128
// with block kb's codes at k < 32, each lane folding its own block's y += d (dot - 128 sum_x).
kernel void q8_rows8_bd(device const uchar* blob [[buffer(0)]], device const half* T [[buffer(2)]],
                        device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                        device const float* xs [[buffer(7)]],
                        uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    const uint nkb = K / 32u;
    const uint n0 = tgid.x * 64u + sgid * 16u;
    const uint per = 2u * ((nkb / 2u + NSK - 1u) / NSK);
    const uint kbeg = min(tgid.z * per, nkb);
    const uint kend = min(kbeg + per, nkb);
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const uint hi = (lane >> 3) & 1u;
    const uint tq = 4u * (lane & 1u);
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
    constexpr auto desc = matmul2d_descriptor(16, 16, 64, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<uint8_t, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 16));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    const device ushort* pa = (const device ushort*)(blob + (ra * nkb) * 34u);
    const device ushort* pb = (const device ushort*)(blob + (rb * nkb) * 34u);
    for (uint kb = kbeg; kb < kend; kb += 2u) {
        _Pragma("clang loop unroll(full)")
        for (uint h = 0; h < 2; ++h) {
            const device ushort* ba = pa + (kb + h) * 17u;
            const device ushort* bb = pb + (kb + h) * 17u;
            const uint2 wa = as_type<uint2>(ushort4(*(const device packed_ushort4*)(ba + 1u + 4u * kq))) ^ 0x80808080u;
            const uint2 wb = as_type<uint2>(ushort4(*(const device packed_ushort4*)(bb + 1u + 4u * kq))) ^ 0x80808080u;
            const uchar4 a0 = as_type<uchar4>(wa.x), a1 = as_type<uchar4>(wa.y), b0 = as_type<uchar4>(wb.x), b1 = as_type<uchar4>(wb.y);
            for (uint i = 0; i < 4; ++i) {
                Lw[16u * h + i] = a0[i];
                Lw[16u * h + 4u + i] = b0[i];
                Lw[16u * h + 8u + i] = a1[i];
                Lw[16u * h + 12u + i] = b1[i];
            }
        }
        auto Rx = tX.template slice<64, 16>(int(kb * 32u), 0);
        auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
        op.run(Lw, Rx, d);
        const uint kh = kb + hi;
        const float da = float(as_type<half>(pa[kh * 17u]));
        const float db = float(as_type<half>(pb[kh * 17u]));
        const float4 sum = *(const device float4*)(xs + kh * 8u + tq);
        for (uint i = 0; i < 4; ++i) {
            acc[i] = fma(da, d[i] - 128.0f * sum[i], acc[i]);
            acc[4u + i] = fma(db, d[4u + i] - 128.0f * sum[i], acc[4u + i]);
        }
    }
    for (uint i = 0; i < 8; ++i) {
        acc[i] += simd_shuffle_xor(acc[i], 8u);
    }
    if (hi == 0u) {
        rows8_store(y, tgid.z, N, tq, ra, rb, acc);
    }
}

// k4_rows8_mt64 software-pipelined across superblocks: a superblock's weight words (both rows' scale uint4 and the lane's
// four 8-byte quant runs a row) load DEPTH - 1 superblocks before their fill, so the loads overlap the matrix work of the
// superblocks ahead of them. The loads past the way's range clamp to its last superblock. Same math as k4_rows8_mt64.
#ifndef DEPTH
#define DEPTH 2
#endif
struct K64Words {
    uint4 sa, sb;
    uint2 qa[4], qb[4];
};
inline K64Words k64_words(const device uint4* sc, const device uint* qs, uint blka, uint blkb, uint kq) {
    K64Words w;
    w.sa = sc[blka];
    w.sb = sc[blkb];
    const device uint2* pa = (const device uint2*)(qs + blka * 32u + 2u * kq);
    const device uint2* pb = (const device uint2*)(qs + blkb * 32u + 2u * kq);
    for (uint j = 0; j < 4; ++j) {
        w.qa[j] = pa[4u * j];
        w.qb[j] = pb[4u * j];
    }
    return w;
}
kernel void k4_rows8_mt64p(device const uint4* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                           device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                           device const float* xs [[buffer(7)]],
                           uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    const uint nsb = K / 256u;
    const uint n0 = tgid.x * 64u + sgid * 16u;
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const bool pad = ((lane >> 3) & 1u) != 0u;
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
    const uint per = (nsb + NSK - 1u) / NSK;
    const uint sbeg = min(tgid.z * per, nsb);
    const uint send = min(sbeg + per, nsb);
    constexpr auto desc = matmul2d_descriptor(16, 8, 64, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<half, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 8));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    const uint tq = pad ? 0u : 4u * kq;
    if (sbeg < send) {
        const uint last = send - 1u;
        K64Words ahead[DEPTH - 1];
        _Pragma("clang loop unroll(full)")
        for (uint p = 0; p < DEPTH - 1; ++p) {
            const uint s = min(sbeg + p, last);
            ahead[p] = k64_words(sc, qs, ra * nsb + s, rb * nsb + s, kq);
        }
        for (uint sb = sbeg; sb < send; ++sb) {
            const K64Words cw = ahead[0];
            _Pragma("clang loop unroll(full)")
            for (uint p = 0; p + 1u < DEPTH - 1; ++p) {
                ahead[p] = ahead[p + 1u];
            }
            const uint sn = min(sb + (DEPTH - 1u), last);
            ahead[DEPTH - 2] = k64_words(sc, qs, ra * nsb + sn, rb * nsb + sn, kq);
            const half2 dda = as_type<half2>(cw.sa.x);
            const half2 ddb = as_type<half2>(cw.sb.x);
            const float da = float(dda.x), dmina = float(dda.y), db = float(ddb.x), dminb = float(ddb.y);
            const uint scla = cw.sa.y & 0x3F3F3F3Fu, scha = (cw.sa.w & 0x0F0F0F0Fu) | ((cw.sa.y >> 2) & 0x30303030u);
            const uint mnla = cw.sa.z & 0x3F3F3F3Fu, mnha = ((cw.sa.w >> 4) & 0x0F0F0F0Fu) | ((cw.sa.z >> 2) & 0x30303030u);
            const uint sclb = cw.sb.y & 0x3F3F3F3Fu, schb = (cw.sb.w & 0x0F0F0F0Fu) | ((cw.sb.y >> 2) & 0x30303030u);
            const uint mnlb = cw.sb.z & 0x3F3F3F3Fu, mnhb = ((cw.sb.w >> 4) & 0x0F0F0F0Fu) | ((cw.sb.z >> 2) & 0x30303030u);
            _Pragma("clang loop unroll(full)")
            for (uint j = 0; j < 4; ++j) {
                const uint2 wa = cw.qa[j];
                const uint2 wb = cw.qb[j];
                const uint s0 = 2u * j;
                const uint sh0 = 8u * (s0 & 3u), sh1 = 8u * ((s0 + 1u) & 3u);
                const uint sca0 = ((s0 < 4u ? scla : scha) >> sh0) & 0xFFu, sca1 = ((s0 + 1u < 4u ? scla : scha) >> sh1) & 0xFFu;
                const uint scb0 = ((s0 < 4u ? sclb : schb) >> sh0) & 0xFFu, scb1 = ((s0 + 1u < 4u ? sclb : schb) >> sh1) & 0xFFu;
                const float mna0 = float(((s0 < 4u ? mnla : mnha) >> sh0) & 0xFFu), mna1 = float(((s0 + 1u < 4u ? mnla : mnha) >> sh1) & 0xFFu);
                const float mnb0 = float(((s0 < 4u ? mnlb : mnhb) >> sh0) & 0xFFu), mnb1 = float(((s0 + 1u < 4u ? mnlb : mnhb) >> sh1) & 0xFFu);
                _Pragma("clang loop unroll(full)")
                for (uint q = 0; q < 4; ++q) {
                    const uint wordA = (q & 1u) ? wa.y : wa.x;
                    const uint wordB = (q & 1u) ? wb.y : wb.x;
                    const uint nib = 4u * (q >> 1);
                    const uint sA = (q >> 1) ? sca1 : sca0;
                    const uint sB = (q >> 1) ? scb1 : scb0;
                    const half2 ha0 = as_type<half2>((((wordA >> nib) & 0x000F000Fu) * sA) | 0x64006400u);
                    const half2 ha1 = as_type<half2>((((wordA >> (nib + 8u)) & 0x000F000Fu) * sA) | 0x64006400u);
                    const half2 hb0 = as_type<half2>((((wordB >> nib) & 0x000F000Fu) * sB) | 0x64006400u);
                    const half2 hb1 = as_type<half2>((((wordB >> (nib + 8u)) & 0x000F000Fu) * sB) | 0x64006400u);
                    Lw[q * 8u + 0u] = ha0.x;
                    Lw[q * 8u + 1u] = ha1.x;
                    Lw[q * 8u + 2u] = ha0.y;
                    Lw[q * 8u + 3u] = ha1.y;
                    Lw[q * 8u + 4u] = hb0.x;
                    Lw[q * 8u + 5u] = hb1.x;
                    Lw[q * 8u + 6u] = hb0.y;
                    Lw[q * 8u + 7u] = hb1.y;
                }
                auto Rx = tX.template slice<64, 8>(int((sb * 4u + j) * 64u), 0);
                auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
                op.run(Lw, Rx, d);
                const float4 sum0 = *(const device float4*)(xs + (sb * 8u + s0) * 8u + tq);
                const float4 sum1 = *(const device float4*)(xs + (sb * 8u + s0 + 1u) * 8u + tq);
                for (uint i = 0; i < 4; ++i) {
                    const float sum01 = sum0[i] + sum1[i];
                    acc[i] = fma(da, d[i] - 1024.0f * sum01, fma(-dmina, fma(mna0, sum0[i], mna1 * sum1[i]), acc[i]));
                    acc[4u + i] = fma(db, d[4u + i] - 1024.0f * sum01, fma(-dminb, fma(mnb0, sum0[i], mnb1 * sum1[i]), acc[4u + i]));
                }
            }
        }
    }
    if (!pad) {
        rows8_store(y, tgid.z, N, tq, ra, rb, acc);
    }
}

// K4, generic product shape: MM weight rows (16 or 32) x 8 tokens x KK (64, 128 or 256) a product, the half left operand
// holding 1024 + q sc. Layouts (probe): a lane's rows are m0 + 16 rr + 8 r (rr < MM / 16, r < 2); its k-slots 32 c + 16 i +
// 4 kq + e (c < KK / 32, i < 2, e < 4); left element index (((c (MM / 16) + rr) 2 + i) 2 + r) 4 + e; destination element
// (rr 2 + r) 4 + t for token 4 kq + t.
#ifndef MM
#define MM 16
#endif
#ifndef KK
#define KK 64
#endif
kernel void k4_rows8_mtg(device const uint4* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                         device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                         device const float* xs [[buffer(7)]],
                         uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    constexpr uint NR = MM / 8;        // rows a lane
    constexpr uint NJ = KK / 64;       // 64-groups a product
    const uint nsb = K / 256u;
    const uint n0 = tgid.x * (4u * MM) + sgid * MM;
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const bool pad = ((lane >> 3) & 1u) != 0u;
    const uint per = (nsb + NSK - 1u) / NSK;
    const uint sbeg = min(tgid.z * per, nsb);
    const uint send = min(sbeg + per, nsb);
    constexpr auto desc = matmul2d_descriptor(MM, 8, KK, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<half, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 8));
    float acc[NR * 4];
    for (uint i = 0; i < NR * 4; ++i) {
        acc[i] = 0.0f;
    }
    const uint tq = pad ? 0u : 4u * kq;
    for (uint sb = sbeg; sb < send; ++sb) {
        float dr[NR], dminr[NR];
        uint scl[NR], sch[NR], mnl[NR], mnh[NR];
        const device uint* qr[NR];
        for (uint rr = 0; rr < NR; ++rr) {
            const uint row = n0 + m0 + 8u * rr;    // rr = 2 rrh + r: rows m0, m0 + 8, m0 + 16, m0 + 24 in layout order
            const uint blk = row * nsb + sb;
            const uint4 sv = sc[blk];
            const half2 dd = as_type<half2>(sv.x);
            dr[rr] = float(dd.x);
            dminr[rr] = float(dd.y);
            scl[rr] = sv.y & 0x3F3F3F3Fu;
            sch[rr] = (sv.w & 0x0F0F0F0Fu) | ((sv.y >> 2) & 0x30303030u);
            mnl[rr] = sv.z & 0x3F3F3F3Fu;
            mnh[rr] = ((sv.w >> 4) & 0x0F0F0F0Fu) | ((sv.z >> 2) & 0x30303030u);
            qr[rr] = qs + blk * 32u + 2u * kq;
        }
        for (uint jp = 0; jp < 4u / NJ; ++jp) {   // products a superblock
            float sumw[4];                        // the tokens' 1024 sum_x over the product's sub-blocks
            float mnsum[NR][4];                   // per row: sum over the product's sub-blocks of mn_s sum_x,s
            for (uint t = 0; t < 4; ++t) {
                sumw[t] = 0.0f;
                for (uint rr = 0; rr < NR; ++rr) {
                    mnsum[rr][t] = 0.0f;
                }
            }
            for (uint jj = 0; jj < NJ; ++jj) {
                const uint j = jp * NJ + jj;
                const uint s0 = 2u * j;
                const uint sh0 = 8u * (s0 & 3u), sh1 = 8u * ((s0 + 1u) & 3u);
                const float4 sum0 = *(const device float4*)(xs + (sb * 8u + s0) * 8u + tq);
                const float4 sum1 = *(const device float4*)(xs + (sb * 8u + s0 + 1u) * 8u + tq);
                for (uint t = 0; t < 4; ++t) {
                    sumw[t] += sum0[t] + sum1[t];
                }
                for (uint rr = 0; rr < NR; ++rr) {
                    const uint2 w = *(const device uint2*)(qr[rr] + 8u * j);
                    const uint sca0 = ((s0 < 4u ? scl[rr] : sch[rr]) >> sh0) & 0xFFu;
                    const uint sca1 = ((s0 + 1u < 4u ? scl[rr] : sch[rr]) >> sh1) & 0xFFu;
                    const float mn0 = float(((s0 < 4u ? mnl[rr] : mnh[rr]) >> sh0) & 0xFFu);
                    const float mn1 = float(((s0 + 1u < 4u ? mnl[rr] : mnh[rr]) >> sh1) & 0xFFu);
                    for (uint t = 0; t < 4; ++t) {
                        mnsum[rr][t] = fma(mn0, sum0[t], fma(mn1, sum1[t], mnsum[rr][t]));
                    }
                    const uint rrh = rr >> 1, r = rr & 1u;
                    for (uint q = 0; q < 4; ++q) {   // quad q: c = 2 jj + (q >> 1), i = q & 1
                        const uint word = (q & 1u) ? w.y : w.x;
                        const uint nib = 4u * (q >> 1);
                        const uint sA = (q >> 1) ? sca1 : sca0;
                        const half2 h0 = as_type<half2>((((word >> nib) & 0x000F000Fu) * sA) | 0x64006400u);
                        const half2 h1 = as_type<half2>((((word >> (nib + 8u)) & 0x000F000Fu) * sA) | 0x64006400u);
                        const uint c = 2u * jj + (q >> 1);
                        const uint i = q & 1u;
                        const uint base = (((c * (MM / 16u) + rrh) * 2u + i) * 2u + r) * 4u;
                        Lw[base + 0u] = h0.x;
                        Lw[base + 1u] = h1.x;
                        Lw[base + 2u] = h0.y;
                        Lw[base + 3u] = h1.y;
                    }
                }
            }
            auto Rx = tX.template slice<KK, 8>(int(sb * 256u + jp * KK), 0);
            auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
            op.run(Lw, Rx, d);
            for (uint rr = 0; rr < NR; ++rr) {
                const uint rrh = rr >> 1, r = rr & 1u;
                for (uint t = 0; t < 4; ++t) {
                    const uint di = (rrh * 2u + r) * 4u + t;
                    acc[rr * 4u + t] = fma(dr[rr], d[di] - 1024.0f * sumw[t], fma(-dminr[rr], mnsum[rr][t], acc[rr * 4u + t]));
                }
            }
        }
    }
    if (pad) {
        return;
    }
    device float* yp = y + tgid.z * 8u * N;
    for (uint rr = 0; rr < NR; ++rr) {
        const uint row = n0 + m0 + 8u * rr;
        for (uint t = 0; t < 4; ++t) {
            yp[(tq + t) * N + row] = acc[rr * 4u + t];
        }
    }
}

// K4 on 128-deep products, one a superblock half-pair: product jp covers the superblock's quant bytes 64 jp .. 64 jp + 63
// (sub-blocks 4 jp .. 4 jp + 3), and lane kq's 32 k-slots of a row are ONE 16-byte load at byte 64 jp + 16 kq. That run
// sits in 32-byte run 2 jp + (kq >> 1) at position 16 (kq & 1), so its low nibbles are sub-block s0 = 4 jp + 2 (kq >> 1)
// and its high ones s0 + 1: a lane folds only two scales into its half 1024 + q sc. Left element (((c 2 + i) 2 + r) 4 + e)
// (probe, 16 x 8 x 128, half and uint8 alike: k-slot 32 c + 16 i + 4 kq + e, row m0 + 8 r) takes nibble c >> 1 of byte e
// of word 2 (c & 1) + i of the run, so the x table (the harness's d128 table) carries at product slot 32 c + 16 i + 4 kq + e
// the x of sub-block s0 + (c >> 1), position 16 (kq & 1) + 4 (2 (c & 1) + i) + e. Epilogue once a product:
// y += d (dot - 1024 (sum_0 + .. + sum_3)) - dmin (mn_0 sum_0 + .. + mn_3 sum_3) over its four 32-blocks.
kernel void k4_rows8_d128(device const uint4* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                          device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                          device const float* xs [[buffer(7)]],
                          uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    const uint nsb = K / 256u;
    const uint n0 = tgid.x * 64u + sgid * 16u;
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const bool pad = ((lane >> 3) & 1u) != 0u;
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
    const uint per = (nsb + NSK - 1u) / NSK;
    const uint sbeg = min(tgid.z * per, nsb);
    const uint send = min(sbeg + per, nsb);
    constexpr auto desc = matmul2d_descriptor(16, 8, 128, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<half, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 8));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    const uint tq = pad ? 0u : 4u * kq;
    const uint shl = 16u * (kq >> 1);   // sub-block s0's byte within its four-scale word
    for (uint sb = sbeg; sb < send; ++sb) {
        const uint blka = ra * nsb + sb;
        const uint blkb = rb * nsb + sb;
        const uint4 sva = sc[blka];
        const uint4 svb = sc[blkb];
        const half2 dda = as_type<half2>(sva.x);
        const half2 ddb = as_type<half2>(svb.x);
        const float da = float(dda.x), dmina = float(dda.y), db = float(ddb.x), dminb = float(ddb.y);
        const uint scw[2][2] = {{sva.y & 0x3F3F3F3Fu, (sva.w & 0x0F0F0F0Fu) | ((sva.y >> 2) & 0x30303030u)},
                                {svb.y & 0x3F3F3F3Fu, (svb.w & 0x0F0F0F0Fu) | ((svb.y >> 2) & 0x30303030u)}};
        const uint mnw[2][2] = {{sva.z & 0x3F3F3F3Fu, ((sva.w >> 4) & 0x0F0F0F0Fu) | ((sva.z >> 2) & 0x30303030u)},
                                {svb.z & 0x3F3F3F3Fu, ((svb.w >> 4) & 0x0F0F0F0Fu) | ((svb.z >> 2) & 0x30303030u)}};
        const device uint4* qa = (const device uint4*)(qs + blka * 32u) + kq;
        const device uint4* qb = (const device uint4*)(qs + blkb * 32u) + kq;
        _Pragma("clang loop unroll(full)")
        for (uint jp = 0; jp < 2; ++jp) {
#ifdef OPONLY
            const uint4 wa = uint4(lane * 0x01010101u + jp, lane ^ 0x5A5A5A5Au, lane * 0x03030303u, lane ^ 0x3C3C3C3Cu);
            const uint4 wb = uint4(lane * 0x10101010u + jp, lane ^ 0xA5A5A5A5u, lane * 0x05050505u, lane ^ 0xC3C3C3C3u);
#else
            const uint4 wa = qa[4u * jp];
            const uint4 wb = qb[4u * jp];
#endif
#ifdef LOADSONLY
            acc[jp] += float(wa.x ^ wa.y ^ wa.z ^ wa.w ^ sva.x ^ sva.w);
            acc[4u + jp] += float(wb.x ^ wb.y ^ wb.z ^ wb.w ^ svb.x ^ svb.w);
            continue;
#endif
            const uint4 w4[2] = {wa, wb};
            _Pragma("clang loop unroll(full)")
            for (uint r = 0; r < 2; ++r) {
                const uint s_lo = (scw[r][jp] >> shl) & 0xFFu;
                const uint s_hi = (scw[r][jp] >> (shl + 8u)) & 0xFFu;
                _Pragma("clang loop unroll(full)")
                for (uint c = 0; c < 4; ++c) {
                    const uint nib = 4u * (c >> 1);
                    const uint s = (c >> 1) ? s_hi : s_lo;
                    _Pragma("clang loop unroll(full)")
                    for (uint i = 0; i < 2; ++i) {
                        const uint word = w4[r][2u * (c & 1u) + i];
                        const half2 h02 = as_type<half2>((((word >> nib) & 0x000F000Fu) * s) | 0x64006400u);
                        const half2 h13 = as_type<half2>((((word >> (nib + 8u)) & 0x000F000Fu) * s) | 0x64006400u);
                        const uint base = ((c * 2u + i) * 2u + r) * 4u;
                        Lw[base + 0u] = h02.x;
                        Lw[base + 1u] = h13.x;
                        Lw[base + 2u] = h02.y;
                        Lw[base + 3u] = h13.y;
                    }
                }
            }
            auto Rx = tX.template slice<128, 8>(int((sb * 2u + jp) * 128u), 0);
            auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
#ifdef LOADONLY
            for (uint i = 0; i < 8; ++i) {
                d[i] = float(Lw[i]) + float(Lw[16u + i]) + float(Lw[32u + i]) + float(Lw[48u + i]);
            }
#else
            op.run(Lw, Rx, d);
#endif
            const device float4* xq = (const device float4*)(xs + (sb * 8u + 4u * jp) * 8u + tq);
            const float4 sum0 = xq[0], sum1 = xq[2], sum2 = xq[4], sum3 = xq[6];
            const uint mwa = mnw[0][jp], mwb = mnw[1][jp];
            const float4 mna = float4(float(mwa & 0xFFu), float((mwa >> 8) & 0xFFu), float((mwa >> 16) & 0xFFu), float(mwa >> 24));
            const float4 mnb = float4(float(mwb & 0xFFu), float((mwb >> 8) & 0xFFu), float((mwb >> 16) & 0xFFu), float(mwb >> 24));
            for (uint i = 0; i < 4; ++i) {
                const float4 sv = float4(sum0[i], sum1[i], sum2[i], sum3[i]);
                const float s4 = sv.x + sv.y + sv.z + sv.w;
                acc[i] = fma(da, d[i] - 1024.0f * s4, fma(-dmina, dot(mna, sv), acc[i]));
                acc[4u + i] = fma(db, d[4u + i] - 1024.0f * s4, fma(-dminb, dot(mnb, sv), acc[4u + i]));
            }
        }
    }
    if (!pad) {
        rows8_store(y, tgid.z, N, tq, ra, rb, acc);
    }
}

// Q6_K on 16-deep products, one an int8 scale block, exact: the left is the 6-bit code q - 32 as int8 (K6_U8: the raw code
// 0..63 as uint8, the 32 folded through the harness's per-16 token sums), epilogue y += d sc dot (K6_U8: d sc (dot - 32
// sum16)). Planes: the engine's split form, a 192-byte quant block (ql[128], qh[64], disk order) and the scale plane's
// 16-byte int8 strips followed by the f16 d tail. Layout (probe, 16 x 8 x 16, int8 / uint8 / half alike): lane kq holds
// k-slots 4 kq + e of rows m0, m0 + 8, so the x table is x in natural order: a scale block is 16 consecutive elements
// 128 h + 32 g + 16 l16 + p of the superblock, and lane kq's four codes of block (h, g, l16) are ql bytes 64 h + 32 (g & 1)
// + 16 l16 + 4 kq (nibble g >> 1) and qh bytes 32 h + 16 l16 + 4 kq (bits 2 g): three 4-byte loads a row serve the four
// blocks g of one (h, l16), twelve a row a superblock, every byte used once.
#ifdef K6_U8
#define K6_LT uint8_t
#else
#define K6_LT int8_t
#endif
kernel void k6_rows8_d16(device const uint4* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                         device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                         device const float* xs [[buffer(7)]],
                         uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    const uint nsb = K / 256u;
    const uint n0 = tgid.x * 64u + sgid * 16u;
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const bool pad = ((lane >> 3) & 1u) != 0u;
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
    const uint per = (nsb + NSK - 1u) / NSK;
    const uint sbeg = min(tgid.z * per, nsb);
    const uint send = min(sbeg + per, nsb);
    const device half* dt = (const device half*)(sc + N * nsb);
    constexpr auto desc = matmul2d_descriptor(16, 8, 16, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<K6_LT, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 8));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    const uint tq = pad ? 0u : 4u * kq;
    for (uint sb = sbeg; sb < send; ++sb) {
        const uint blka = ra * nsb + sb;
        const uint blkb = rb * nsb + sb;
#ifdef OPONLY
        const uint4 sva = uint4(0x02030405u ^ sb, 0x01020304u, 0x7F010203u, 0x05060708u ^ lane);
        const uint4 svb = uint4(0x03040506u, 0x02030405u ^ sb, 0x01020304u, 0x06070809u ^ lane);
        const float da = float(sb & 7u) * 0.125f, db = float(lane & 7u) * 0.125f;
#else
        const uint4 sva = sc[blka];
        const uint4 svb = sc[blkb];
        const float da = float(dt[blka]), db = float(dt[blkb]);
#endif
        const device uint* qa = qs + blka * 48u + kq;
        const device uint* qb = qs + blkb * 48u + kq;
        _Pragma("clang loop unroll(full)")
        for (uint h = 0; h < 2; ++h) {
            _Pragma("clang loop unroll(full)")
            for (uint l16 = 0; l16 < 2; ++l16) {
                const uint o = 16u * h + 4u * l16;
#ifdef OPONLY
                const uint la0 = lane * 0x01010101u + o, la1 = lane ^ 0x5A5A5A5Au, lah = lane * 0x03030303u ^ sb;
                const uint lb0 = lane * 0x10101010u + o, lb1 = lane ^ 0xA5A5A5A5u, lbh = lane * 0x05050505u ^ sb;
#else
                const uint la0 = qa[o], la1 = qa[o + 8u], lah = qa[32u + 8u * h + 4u * l16];
                const uint lb0 = qb[o], lb1 = qb[o + 8u], lbh = qb[32u + 8u * h + 4u * l16];
#endif
#ifdef LOADSONLY
                acc[2u * h + l16] += float(la0 ^ la1 ^ lah ^ sva.x ^ sva.w) * da;
                acc[4u + 2u * h + l16] += float(lb0 ^ lb1 ^ lbh ^ svb.x ^ svb.w) * db;
                continue;
#endif
                _Pragma("clang loop unroll(full)")
                for (uint g = 0; g < 4; ++g) {
                    const uint nib = 4u * (g >> 1);
                    const uint ca = ((((g & 1u) ? la1 : la0) >> nib) & 0x0F0F0F0Fu) | (((lah >> (2u * g)) & 0x03030303u) << 4);
                    const uint cb = ((((g & 1u) ? lb1 : lb0) >> nib) & 0x0F0F0F0Fu) | (((lbh >> (2u * g)) & 0x03030303u) << 4);
#ifdef K6_U8
                    const uchar4 va = as_type<uchar4>(ca), vb = as_type<uchar4>(cb);
#else
                    const char4 va = as_type<char4>(((ca | 0x80808080u) - 0x20202020u) ^ 0x80808080u);   // per byte q - 32, no borrow across bytes
                    const char4 vb = as_type<char4>(((cb | 0x80808080u) - 0x20202020u) ^ 0x80808080u);
#endif
                    for (uint e = 0; e < 4; ++e) {
                        Lw[e] = va[e];
                        Lw[4u + e] = vb[e];
                    }
                    const uint blk16 = 8u * h + 2u * g + l16;   // the scale block, and the superblock's 16-group index
                    auto Rx = tX.template slice<16, 8>(int(sb * 256u + 16u * blk16), 0);
                    auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
                    op.run(Lw, Rx, d);
                    const float fa = da * float(as_type<char4>(sva[blk16 >> 2])[blk16 & 3u]);
                    const float fb = db * float(as_type<char4>(svb[blk16 >> 2])[blk16 & 3u]);
#ifdef K6_U8
                    const float4 s16 = *(const device float4*)(xs + (sb * 16u + blk16) * 8u + tq);
                    for (uint i = 0; i < 4; ++i) {
                        acc[i] = fma(fa, d[i] - 32.0f * s16[i], acc[i]);
                        acc[4u + i] = fma(fb, d[4u + i] - 32.0f * s16[i], acc[4u + i]);
                    }
#else
                    for (uint i = 0; i < 4; ++i) {
                        acc[i] = fma(fa, d[i], acc[i]);
                        acc[4u + i] = fma(fb, d[4u + i], acc[4u + i]);
                    }
#endif
                }
            }
        }
    }
    if (!pad) {
        rows8_store(y, tgid.z, N, tq, ra, rb, acc);
    }
}

// The one-row references: one token's GEMV (x = the natural table's token 0), two rows a simdgroup, four simdgroups a
// threadgroup, simd_sum reduction, y[row] written. Their figure is the byte floor a shape's eight-row kernels are read
// against where no engine figure exists.
// K4: lane (ix = lane >> 3, iq, ir) takes superblocks ix, ix + 4, ..; in each, bytes 8 ir .. 8 ir + 7 of 32-byte runs iq
// and iq + 2 (sub-blocks 2 iq, 2 iq + 1, 2 iq + 4, 2 iq + 5).
kernel void k4_gemv1(device const uint4* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                     device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]],
                     uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    const uint nsb = K / 256u;
    const uint r0 = (tgid.x * 4u + sgid) * 2u;
    const uint ix = lane >> 3, iq = (lane >> 2) & 1u, ir = lane & 3u;
    float s[2] = {0.0f, 0.0f};
    for (uint sb = ix; sb < nsb; sb += 4u) {
        float4 xv[4][2];
        float sumx[4];
        _Pragma("clang loop unroll(full)")
        for (uint u = 0; u < 4; ++u) {
            const uint sub = 2u * iq + (u & 1u) + 4u * (u >> 1);
            const device half4* xp = (const device half4*)(T + sb * 256u + 32u * sub + 8u * ir);
            xv[u][0] = float4(xp[0]);
            xv[u][1] = float4(xp[1]);
            sumx[u] = dot(xv[u][0] + xv[u][1], float4(1.0f));
        }
        _Pragma("clang loop unroll(full)")
        for (uint rr = 0; rr < 2; ++rr) {
            const uint blk = (r0 + rr) * nsb + sb;
            const uint4 sv = sc[blk];
            const half2 dd = as_type<half2>(sv.x);
            const uint scw[2] = {sv.y & 0x3F3F3F3Fu, (sv.w & 0x0F0F0F0Fu) | ((sv.y >> 2) & 0x30303030u)};
            const uint mnw[2] = {sv.z & 0x3F3F3F3Fu, ((sv.w >> 4) & 0x0F0F0F0Fu) | ((sv.z >> 2) & 0x30303030u)};
            const uint2 q[2] = {*(const device uint2*)(qs + blk * 32u + 8u * iq + 2u * ir), *(const device uint2*)(qs + blk * 32u + 8u * (iq + 2u) + 2u * ir)};
            float acc = 0.0f;
            _Pragma("clang loop unroll(full)")
            for (uint u = 0; u < 4; ++u) {
                const uint sub = 2u * iq + (u & 1u) + 4u * (u >> 1);
                const uint2 w = q[u >> 1];
                const uint nib = 4u * (u & 1u);
                const uint lo = (w.x >> nib) & 0x0F0F0F0Fu, hi = (w.y >> nib) & 0x0F0F0F0Fu;
                const float dq = dot(float4(as_type<uchar4>(lo)), xv[u][0]) + dot(float4(as_type<uchar4>(hi)), xv[u][1]);
                const float scs = float((scw[sub >> 2] >> (8u * (sub & 3u))) & 0xFFu);
                const float mns = float((mnw[sub >> 2] >> (8u * (sub & 3u))) & 0xFFu);
                acc = fma(float(dd.x) * scs, dq, fma(-float(dd.y) * mns, sumx[u], acc));
            }
            s[rr] += acc;
        }
    }
    for (uint rr = 0; rr < 2; ++rr) {
        const float t = simd_sum(s[rr]);
        if (lane == 0u) {
            y[r0 + rr] = t;
        }
    }
}

// K6: lane (ix = lane >> 4, ip, il) takes superblocks ix, ix + 2, ..; in each, ql bytes 64 ip + 4 il and 64 ip + 32 + 4 il,
// qh bytes 32 ip + 4 il: elements 128 ip + 32 g + 4 il + e, scales 8 ip + 2 g + (il >> 2).
kernel void k6_gemv1(device const uint4* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                     device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]],
                     uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    const uint nsb = K / 256u;
    const uint r0 = (tgid.x * 4u + sgid) * 2u;
    const uint ix = lane >> 4, ip = (lane >> 3) & 1u, il = lane & 7u;
    const device half* dt = (const device half*)(sc + N * nsb);
    const device char* scb = (const device char*)sc;
    float s[2] = {0.0f, 0.0f};
    for (uint sb = ix; sb < nsb; sb += 2u) {
        float4 xv[4];
        float sumx[4];
        _Pragma("clang loop unroll(full)")
        for (uint g = 0; g < 4; ++g) {
            xv[g] = float4(*(const device half4*)(T + sb * 256u + 128u * ip + 32u * g + 4u * il));
            sumx[g] = xv[g].x + xv[g].y + xv[g].z + xv[g].w;
        }
        _Pragma("clang loop unroll(full)")
        for (uint rr = 0; rr < 2; ++rr) {
            const uint blk = (r0 + rr) * nsb + sb;
            const device uint* q = qs + blk * 48u;
            const uint l0 = q[16u * ip + il], l1 = q[16u * ip + 8u + il], hb = q[32u + 8u * ip + il];
            const device char* scp = scb + blk * 16u + 8u * ip + (il >> 2);
            float acc = 0.0f;
            _Pragma("clang loop unroll(full)")
            for (uint g = 0; g < 4; ++g) {
                const uint c = ((((g & 1u) ? l1 : l0) >> (4u * (g >> 1))) & 0x0F0F0F0Fu) | (((hb >> (2u * g)) & 0x03030303u) << 4);
                acc = fma(float(scp[2u * g]), dot(float4(as_type<uchar4>(c)), xv[g]) - 32.0f * sumx[g], acc);
            }
            s[rr] = fma(float(dt[blk]), acc, s[rr]);
        }
    }
    for (uint rr = 0; rr < 2; ++rr) {
        const float t = simd_sum(s[rr]);
        if (lane == 0u) {
            y[r0 + rr] = t;
        }
    }
}

// The affine g64 members (mlx4b / mlx8b: codes verbatim, element k of a row at bits 4k of the code words (4-bit) or byte
// k (8-bit); the scale plane one 4-byte (scale, bias) pair a 64-group, BF16 halves) on 16 x 8 x 64 products, one a group,
// the pair in the epilogue: y += s dot + b sum_x64, sum_x64 the two 32-block sums of the half-rounded x the product read.
// Layout (probe, 16 x 8 x 64, half / int8 / uint8 alike): left element (((c 2 + i) 2 + r) 4 + e) is k-slot 32 c + 16 i +
// 4 kq + e of row m0 + 8 r. Under the k4 kernels' x table (slot 16 i + 4 kq + e of 32-block c at physical 8 kq + 4 i + e)
// lane kq's codes of a row are 32-block c's elements 8 kq .. 8 kq + 7: word kq of the block (4-bit, nibbles 4 i + e) or
// its 8-byte run kq (8-bit, word i, byte e) - two loads a row a group. CONTIG instead reads a table whose 64-group slot
// 32 c + 16 i + 4 kq + e sits at physical 16 kq + 8 c + 4 i + e, so the lane's codes are the group's 8-byte run kq (4-bit)
// or 16-byte run kq (8-bit), one load. LT_U8: the codes as uint8; LT_I8: int8 (8-bit as q - 128, its 128 folded into the
// bias); else half, the exact code from 1024 + q less 1024.
#if defined(LT_U8)
#define MLX_LT uint8_t
#elif defined(LT_I8)
#define MLX_LT int8_t
#else
#define MLX_LT half
#endif

inline float2 mlx_pair(uint p) {
#ifdef MLX_F16
    const half2 h = as_type<half2>(p);
    return float2(h);
#else
    return float2(as_type<float>(p << 16), as_type<float>(p & 0xFFFF0000u));
#endif
}

// four 4-bit codes (nibbles 0..3 of v's low 16 bits) as the bytes of one word
inline uint mlx_nib4(uint v) {
    const uint x = (v & 0xFFu) | ((v & 0xFF00u) << 8);
    return (x & 0x000F000Fu) | (((x >> 4) & 0x000F000Fu) << 8);
}

template <bool A8, typename TL>
inline void mlx_fill_quad(thread TL& Lw, uint base, uint w) {
    // w: four codes ee = 0..3 as bytes (8-bit) or as nibbles 0..3 of its low 16 bits (4-bit)
#if defined(LT_U8) || defined(LT_I8)
    uint b = A8 ? w : mlx_nib4(w);
#if defined(LT_I8)
    if (A8) {
        b ^= 0x80808080u;
    }
    const char4 v = as_type<char4>(b);
#else
    const uchar4 v = as_type<uchar4>(b);
#endif
    Lw[base + 0u] = v.x;
    Lw[base + 1u] = v.y;
    Lw[base + 2u] = v.z;
    Lw[base + 3u] = v.w;
#elif defined(LT_HCVT)
    const uint b = A8 ? w : mlx_nib4(w);
    Lw[base + 0u] = half(b & 0xFFu);
    Lw[base + 1u] = half((b >> 8) & 0xFFu);
    Lw[base + 2u] = half((b >> 16) & 0xFFu);
    Lw[base + 3u] = half(b >> 24);
#else
    uint p02, p13;
    if (A8) {
        p02 = w & 0x00FF00FFu;
        p13 = (w >> 8) & 0x00FF00FFu;
    } else {
        p02 = (w & 0xFu) | ((w & 0xF00u) << 8);
        p13 = ((w >> 4) & 0xFu) | ((w & 0xF000u) << 4);
    }
    const half2 h02 = as_type<half2>(p02 | 0x64006400u) - half2(1024.0h);
    const half2 h13 = as_type<half2>(p13 | 0x64006400u) - half2(1024.0h);
    Lw[base + 0u] = h02.x;
    Lw[base + 1u] = h13.x;
    Lw[base + 2u] = h02.y;
    Lw[base + 3u] = h13.y;
#endif
}

template <bool A8>
inline void mlx_rows8(device const uint* sc, device const uint* qs, device const half* T, device float* y, uint K, uint N, uint NSK,
                      device const float* xs, uint3 tgid, uint sgid, uint lane) {
    const uint ng = K / 64u;
    const uint n0 = tgid.x * 64u + sgid * 16u;
    if (n0 >= N) {
        return;
    }
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const bool pad = ((lane >> 3) & 1u) != 0u;
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
    const uint per = (ng + NSK - 1u) / NSK;
    const uint gbeg = min(tgid.z * per, ng);
    const uint gend = min(gbeg + per, ng);
    constexpr auto desc = matmul2d_descriptor(16, 8, 64, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<MLX_LT, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 8));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    const uint tq = pad ? 0u : 4u * kq;
    const uint gw = A8 ? 16u : 8u;   // code words a row a group
    for (uint g = gbeg; g < gend; ++g) {
        const uint ga = ra * ng + g;
        const uint gb = rb * ng + g;
        const float2 pa = mlx_pair(sc[ga]);
        const float2 pb = mlx_pair(sc[gb]);
        uint wa[4], wb[4];   // the lane's code words of a row: quad (c, i) at 2 c + i (8-bit) or c (4-bit)
#ifdef CONTIG
        if (A8) {
            const uint4 ua = *(const device uint4*)(qs + ga * gw + 4u * kq);
            const uint4 ub = *(const device uint4*)(qs + gb * gw + 4u * kq);
            wa[0] = ua.x; wa[1] = ua.y; wa[2] = ua.z; wa[3] = ua.w;
            wb[0] = ub.x; wb[1] = ub.y; wb[2] = ub.z; wb[3] = ub.w;
        } else {
            const uint2 ua = *(const device uint2*)(qs + ga * gw + 2u * kq);
            const uint2 ub = *(const device uint2*)(qs + gb * gw + 2u * kq);
            wa[0] = ua.x; wa[1] = ua.y;
            wb[0] = ub.x; wb[1] = ub.y;
        }
#else
        if (A8) {
            const uint2 a0 = *(const device uint2*)(qs + ga * gw + 2u * kq);
            const uint2 a1 = *(const device uint2*)(qs + ga * gw + 8u + 2u * kq);
            const uint2 b0 = *(const device uint2*)(qs + gb * gw + 2u * kq);
            const uint2 b1 = *(const device uint2*)(qs + gb * gw + 8u + 2u * kq);
            wa[0] = a0.x; wa[1] = a0.y; wa[2] = a1.x; wa[3] = a1.y;
            wb[0] = b0.x; wb[1] = b0.y; wb[2] = b1.x; wb[3] = b1.y;
        } else {
            wa[0] = qs[ga * gw + kq];
            wa[1] = qs[ga * gw + 4u + kq];
            wb[0] = qs[gb * gw + kq];
            wb[1] = qs[gb * gw + 4u + kq];
        }
#endif
        _Pragma("clang loop unroll(full)")
        for (uint q = 0; q < 4; ++q) {   // quad q = 2 c + i
            const uint c = q >> 1, i = q & 1u;
            const uint xa = A8 ? wa[q] : (wa[c] >> (16u * i));
            const uint xb = A8 ? wb[q] : (wb[c] >> (16u * i));
            mlx_fill_quad<A8>(Lw, q * 8u, xa);
            mlx_fill_quad<A8>(Lw, q * 8u + 4u, xb);
        }
        auto Rx = tX.template slice<64, 8>(int(g * 64u), 0);
        auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
        op.run(Lw, Rx, d);
        const float4 s0 = *(const device float4*)(xs + (2u * g) * 8u + tq);
        const float4 s1 = *(const device float4*)(xs + (2u * g + 1u) * 8u + tq);
        float ba = pa.y, bb = pb.y;
#ifdef LT_I8
        if (A8) {
            ba = fma(128.0f, pa.x, ba);
            bb = fma(128.0f, pb.x, bb);
        }
#endif
        for (uint t = 0; t < 4; ++t) {
            const float sx = s0[t] + s1[t];
            acc[t] = fma(pa.x, d[t], fma(ba, sx, acc[t]));
            acc[4u + t] = fma(pb.x, d[4u + t], fma(bb, sx, acc[4u + t]));
        }
    }
    if (!pad) {
        rows8_store(y, tgid.z, N, tq, ra, rb, acc);
    }
}

kernel void mlx4_rows8(device const uint* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                       device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                       device const float* xs [[buffer(7)]],
                       uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    mlx_rows8<false>(sc, qs, T, y, K, N, NSK, xs, tgid, sgid, lane);
}

kernel void mlx8_rows8(device const uint* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                       device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                       device const float* xs [[buffer(7)]],
                       uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    mlx_rows8<true>(sc, qs, T, y, K, N, NSK, xs, tgid, sgid, lane);
}

// The affine one-row reference, the engine's wide form: a lane takes a whole 32-block (one 16-byte load on 4-bit, two on
// 8-bit), its codes masked in place against x divided by each code's power of two, the pair folded once a block; two rows a
// simdgroup sharing the lane's x.
template <bool A8>
inline void mlx_gemv1(device const uint* sc, device const uint* qs, device const half* T, device float* y, uint K, uint3 tgid, uint sgid, uint lane) {
    const uint nb = K / 32u;
    const uint r0 = (tgid.x * 4u + sgid) * 2u;
    const uint bw = A8 ? 8u : 4u;
    const device ushort* sp = (const device ushort*)sc;
    float s[2] = {0.0f, 0.0f};
    for (uint ib = lane; ib < nb; ib += 32u) {
        float4 xv[8];
        float xsum = 0.0f;
        const device half4* xp = (const device half4*)(T + ib * 32u);
        _Pragma("clang loop unroll(full)")
        for (uint j = 0; j < 8; ++j) {
            const float4 x4 = float4(xp[j]);
            xsum += (x4.x + x4.y) + (x4.z + x4.w);
            if (A8) {
                xv[j] = x4 * float4(1.0f, 0.00390625f, 1.52587890625e-5f, 5.9604644775390625e-8f);
            } else {
                xv[j] = x4 * ((j & 1u) ? 1.52587890625e-5f : 1.0f) * float4(1.0f, 0.0625f, 0.00390625f, 0.000244140625f);
            }
        }
        _Pragma("clang loop unroll(full)")
        for (uint rr = 0; rr < 2; ++rr) {
            const uint blk = (r0 + rr) * nb + ib;
            const float2 p = float2(as_type<float>(uint(sp[blk & ~1u]) << 16), as_type<float>(uint(sp[blk | 1u]) << 16));
            float qd = 0.0f;
            if (A8) {
                const uint4 u0 = *(const device uint4*)(qs + blk * bw);
                const uint4 u1 = *(const device uint4*)(qs + blk * bw + 4u);
                const uint u[8] = {u0.x, u0.y, u0.z, u0.w, u1.x, u1.y, u1.z, u1.w};
                _Pragma("clang loop unroll(full)")
                for (uint j = 0; j < 8; ++j) {
                    qd += dot(float4(float(u[j] & 0xFFu), float(u[j] & 0xFF00u), float(u[j] & 0xFF0000u), float(u[j] & 0xFF000000u)), xv[j]);
                }
            } else {
                const uint4 u0 = *(const device uint4*)(qs + blk * bw);
                const uint u[4] = {u0.x, u0.y, u0.z, u0.w};
                _Pragma("clang loop unroll(full)")
                for (uint j = 0; j < 8; ++j) {
                    const uint w = u[j >> 1];
                    const uint sh = (j & 1u) * 16u;
                    qd += dot(float4(float(w & (15u << sh)), float(w & (240u << sh)), float(w & (3840u << sh)), float(w & (61440u << sh))), xv[j]);
                }
            }
            s[rr] = fma(p.x, qd, fma(p.y, xsum, s[rr]));
        }
    }
    for (uint rr = 0; rr < 2; ++rr) {
        const float t = simd_sum(s[rr]);
        if (lane == 0u) {
            y[r0 + rr] = t;
        }
    }
}

kernel void mlx4_gemv1(device const uint* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                       device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]],
                       uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    mlx_gemv1<false>(sc, qs, T, y, K, tgid, sgid, lane);
}

kernel void mlx8_gemv1(device const uint* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                       device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]],
                       uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    mlx_gemv1<true>(sc, qs, T, y, K, tgid, sgid, lane);
}

// The split-K reduce the shipped form dispatches after a split site: y = the sum of NSK planes, one float4 a thread.
kernel void rows8_reduce(device const float4* part [[buffer(0)]], device float4* y [[buffer(1)]], constant uint& total [[buffer(2)]],
                         constant uint& NSK [[buffer(3)]], uint gid [[thread_position_in_grid]]) {
    if (gid >= total) {
        return;
    }
    float4 a = part[gid];
    for (uint s = 1u; s < NSK; ++s) {
        a += part[s * total + gid];
    }
    y[gid] = a;
}

// The in-threadgroup K split: a threadgroup owns TG_RS row tiles of 16 and TG_KS K slices of each, simdgroup sgid the tile
// sgid % TG_RS and the slice sgid / TG_RS; the grid's z adds NSK outer ways (planes past one). TG_IL interleaves the slices
// group by group (slice s takes groups s, s + S, ..) instead of contiguous runs. The slices' partial y meets in threadgroup
// memory, [slice][tile][element][lane], and simdgroup ks sums elements ks, ks + TG_KS, .. in slice order.
#ifndef TG_RS
#define TG_RS 1
#endif
#ifndef TG_KS
#define TG_KS 8
#endif

inline void tg_reduce_store(threadgroup float* red, thread const float* acc, device float* y, uint z, uint N, uint tq, uint ra, uint rb,
                            uint rs, uint ks, uint lane, bool live) {
    for (uint i = 0; i < 8u; ++i) {
        red[((ks * TG_RS + rs) * 8u + i) * 32u + lane] = acc[i];
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if (!live) {
        return;
    }
    device float* yp = y + z * 8u * N;
    for (uint i = ks; i < 8u; i += TG_KS) {
        float s = red[(rs * 8u + i) * 32u + lane];
        for (uint k = 1u; k < TG_KS; ++k) {
            s += red[((k * TG_RS + rs) * 8u + i) * 32u + lane];
        }
        yp[(tq + (i & 3u)) * N + (i < 4u ? ra : rb)] = s;
    }
}

// the slice's groups of ng: contiguous [beg, end) step 1, or interleaved from the slice step S
struct TgRange {
    uint beg, end, step;
};

inline TgRange tg_range(uint ng, uint z, uint ks, uint NSK, bool live) {
    const uint slices = NSK * TG_KS;
    const uint slice = z * TG_KS + ks;
    TgRange r;
#ifdef TG_IL
    r.beg = slice;
    r.end = live ? ng : 0u;
    r.step = slices;
#else
    const uint per = (ng + slices - 1u) / slices;
    r.beg = min(slice * per, ng);
    r.end = live ? min(r.beg + per, ng) : r.beg;
    r.step = 1u;
#endif
    return r;
}

template <bool A8>
inline void mlx_rows8_tg(device const uint* sc, device const uint* qs, device const half* T, device float* y, uint K, uint N, uint NSK,
                         device const float* xs, uint3 tgid, uint sgid, uint lane, threadgroup float* red) {
    const uint ng = K / 64u;
    const uint rs = sgid % TG_RS, ks = sgid / TG_RS;
    const uint n0 = tgid.x * (16u * TG_RS) + rs * 16u;
    const bool rows_ok = n0 < N;
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const bool pad = ((lane >> 3) & 1u) != 0u;
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
    const TgRange gr = tg_range(ng, tgid.z, ks, NSK, rows_ok);
    constexpr auto desc = matmul2d_descriptor(16, 8, 64, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<half, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 8));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    const uint tq = pad ? 0u : 4u * kq;
    const uint gw = A8 ? 16u : 8u;
    for (uint g = gr.beg; g < gr.end; g += gr.step) {
        const uint ga = ra * ng + g;
        const uint gb = rb * ng + g;
        const float2 pa = mlx_pair(sc[ga]);
        const float2 pb = mlx_pair(sc[gb]);
        uint wa[4], wb[4];
        if (A8) {
            const uint2 a0 = *(const device uint2*)(qs + ga * gw + 2u * kq);
            const uint2 a1 = *(const device uint2*)(qs + ga * gw + 8u + 2u * kq);
            const uint2 b0 = *(const device uint2*)(qs + gb * gw + 2u * kq);
            const uint2 b1 = *(const device uint2*)(qs + gb * gw + 8u + 2u * kq);
            wa[0] = a0.x; wa[1] = a0.y; wa[2] = a1.x; wa[3] = a1.y;
            wb[0] = b0.x; wb[1] = b0.y; wb[2] = b1.x; wb[3] = b1.y;
        } else {
            wa[0] = qs[ga * gw + kq];
            wa[1] = qs[ga * gw + 4u + kq];
            wb[0] = qs[gb * gw + kq];
            wb[1] = qs[gb * gw + 4u + kq];
        }
        _Pragma("clang loop unroll(full)")
        for (uint q = 0; q < 4; ++q) {
            const uint c = q >> 1, i = q & 1u;
            const uint xa = A8 ? wa[q] : (wa[c] >> (16u * i));
            const uint xb = A8 ? wb[q] : (wb[c] >> (16u * i));
            mlx_fill_quad<A8>(Lw, q * 8u, xa);
            mlx_fill_quad<A8>(Lw, q * 8u + 4u, xb);
        }
        auto Rx = tX.template slice<64, 8>(int(g * 64u), 0);
        auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
        op.run(Lw, Rx, d);
        const float4 s0 = *(const device float4*)(xs + (2u * g) * 8u + tq);
        const float4 s1 = *(const device float4*)(xs + (2u * g + 1u) * 8u + tq);
        for (uint t = 0; t < 4; ++t) {
            const float sx = s0[t] + s1[t];
            acc[t] = fma(pa.x, d[t], fma(pa.y, sx, acc[t]));
            acc[4u + t] = fma(pb.x, d[4u + t], fma(pb.y, sx, acc[4u + t]));
        }
    }
    tg_reduce_store(red, acc, y, tgid.z, N, tq, ra, rb, rs, ks, lane, rows_ok && !pad);
}

kernel void mlx4_rows8_tg(device const uint* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                          device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                          device const float* xs [[buffer(7)]],
                          uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    threadgroup float red[TG_KS * TG_RS * 256];
    mlx_rows8_tg<false>(sc, qs, T, y, K, N, NSK, xs, tgid, sgid, lane, red);
}

kernel void mlx8_rows8_tg(device const uint* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                          device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                          device const float* xs [[buffer(7)]],
                          uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    threadgroup float red[TG_KS * TG_RS * 256];
    mlx_rows8_tg<true>(sc, qs, T, y, K, N, NSK, xs, tgid, sgid, lane, red);
}

// k4 on the in-threadgroup split: k4_rows8_mt64's product, the slices over 64-groups (a superblock's four), the superblock's
// scale block read a group
kernel void k4_rows8_tg(device const uint4* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                        device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                        device const float* xs [[buffer(7)]],
                        uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]]) {
    threadgroup float red[TG_KS * TG_RS * 256];
    const uint nsb = K / 256u;
    const uint rs = sgid % TG_RS, ks = sgid / TG_RS;
    const uint n0 = tgid.x * (16u * TG_RS) + rs * 16u;
    const bool rows_ok = n0 < N;
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const bool pad = ((lane >> 3) & 1u) != 0u;
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
    const TgRange gr = tg_range(nsb * 4u, tgid.z, ks, NSK, rows_ok);
    constexpr auto desc = matmul2d_descriptor(16, 8, 64, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<half, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 8));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    const uint tq = pad ? 0u : 4u * kq;
    for (uint g = gr.beg; g < gr.end; g += gr.step) {
        const uint sb = g >> 2, j = g & 3u;
        const uint blka = ra * nsb + sb;
        const uint blkb = rb * nsb + sb;
        const uint4 sva = sc[blka];
        const uint4 svb = sc[blkb];
        const half2 dda = as_type<half2>(sva.x);
        const half2 ddb = as_type<half2>(svb.x);
        const float da = float(dda.x), dmina = float(dda.y), db = float(ddb.x), dminb = float(ddb.y);
        const uint scla = sva.y & 0x3F3F3F3Fu, scha = (sva.w & 0x0F0F0F0Fu) | ((sva.y >> 2) & 0x30303030u);
        const uint mnla = sva.z & 0x3F3F3F3Fu, mnha = ((sva.w >> 4) & 0x0F0F0F0Fu) | ((sva.z >> 2) & 0x30303030u);
        const uint sclb = svb.y & 0x3F3F3F3Fu, schb = (svb.w & 0x0F0F0F0Fu) | ((svb.y >> 2) & 0x30303030u);
        const uint mnlb = svb.z & 0x3F3F3F3Fu, mnhb = ((svb.w >> 4) & 0x0F0F0F0Fu) | ((svb.z >> 2) & 0x30303030u);
        const uint2 wa = *(const device uint2*)(qs + blka * 32u + 2u * kq + 8u * j);
        const uint2 wb = *(const device uint2*)(qs + blkb * 32u + 2u * kq + 8u * j);
        const uint s0 = 2u * j;
        const uint sh0 = 8u * (s0 & 3u), sh1 = 8u * ((s0 + 1u) & 3u);
        const uint sca0 = ((s0 < 4u ? scla : scha) >> sh0) & 0xFFu, sca1 = ((s0 + 1u < 4u ? scla : scha) >> sh1) & 0xFFu;
        const uint scb0 = ((s0 < 4u ? sclb : schb) >> sh0) & 0xFFu, scb1 = ((s0 + 1u < 4u ? sclb : schb) >> sh1) & 0xFFu;
        const float mna0 = float(((s0 < 4u ? mnla : mnha) >> sh0) & 0xFFu), mna1 = float(((s0 + 1u < 4u ? mnla : mnha) >> sh1) & 0xFFu);
        const float mnb0 = float(((s0 < 4u ? mnlb : mnhb) >> sh0) & 0xFFu), mnb1 = float(((s0 + 1u < 4u ? mnlb : mnhb) >> sh1) & 0xFFu);
        _Pragma("clang loop unroll(full)")
        for (uint q = 0; q < 4; ++q) {
            const uint wordA = (q & 1u) ? wa.y : wa.x;
            const uint wordB = (q & 1u) ? wb.y : wb.x;
            const uint nib = 4u * (q >> 1);
            const uint sA = (q >> 1) ? sca1 : sca0;
            const uint sB = (q >> 1) ? scb1 : scb0;
            const half2 ha0 = as_type<half2>((((wordA >> nib) & 0x000F000Fu) * sA) | 0x64006400u);
            const half2 ha1 = as_type<half2>((((wordA >> (nib + 8u)) & 0x000F000Fu) * sA) | 0x64006400u);
            const half2 hb0 = as_type<half2>((((wordB >> nib) & 0x000F000Fu) * sB) | 0x64006400u);
            const half2 hb1 = as_type<half2>((((wordB >> (nib + 8u)) & 0x000F000Fu) * sB) | 0x64006400u);
            Lw[q * 8u + 0u] = ha0.x;
            Lw[q * 8u + 1u] = ha1.x;
            Lw[q * 8u + 2u] = ha0.y;
            Lw[q * 8u + 3u] = ha1.y;
            Lw[q * 8u + 4u] = hb0.x;
            Lw[q * 8u + 5u] = hb1.x;
            Lw[q * 8u + 6u] = hb0.y;
            Lw[q * 8u + 7u] = hb1.y;
        }
        auto Rx = tX.template slice<64, 8>(int(g * 64u), 0);
        auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
        op.run(Lw, Rx, d);
        const float4 sum0 = *(const device float4*)(xs + (sb * 8u + s0) * 8u + tq);
        const float4 sum1 = *(const device float4*)(xs + (sb * 8u + s0 + 1u) * 8u + tq);
        for (uint i = 0; i < 4; ++i) {
            const float sum01 = sum0[i] + sum1[i];
            acc[i] = fma(da, d[i] - 1024.0f * sum01, fma(-dmina, fma(mna0, sum0[i], mna1 * sum1[i]), acc[i]));
            acc[4u + i] = fma(db, d[4u + i] - 1024.0f * sum01, fma(-dminb, fma(mnb0, sum0[i], mnb1 * sum1[i]), acc[4u + i]));
        }
    }
    tg_reduce_store(red, acc, y, tgid.z, N, tq, ra, rb, rs, ks, lane, rows_ok && !pad);
}

// IQ4_XS on the in-threadgroup split: the engine's split planes (128 quant bytes a superblock; the scale plane's 16-byte
// strips, the eight signed sub-scales ls - 32 in bytes 0..7, then the f16 d tail), one 16 x 8 x 32 product a sub-block, the
// LEFT operand the int8 kvalues, the x table in natural order, the epilogue y += d ls dot. Sub-block js's 16 bytes hold
// element i (low nibble) and i + 16 (high nibble) at byte i, so lane kq's left elements - k 4 kq + e and 16 + 4 kq + e of
// rows m0 and m0 + 8 - are the low and high nibbles of word kq of the sub-block: one load a row a sub-block. IQ_LUT picks
// the codebook lookup: 0 a register byte-select over four packed words, 1 a 16-entry threadgroup table, 2 a 256-entry
// threadgroup table of a byte's two int8 values. IQ_BLK is the slices' unit: 32 (a sub-block, slice s owns sub-block s of
// every superblock) or 256 (a superblock).
#ifndef IQ_LUT
#define IQ_LUT 0
#endif
#ifndef IQ_BLK
#define IQ_BLK 32
#endif
#define IQ4W0 0xBFAD9881u
#define IQ4W1 0xF6EADDCFu
#define IQ4W2 0x26190D01u
#define IQ4W3 0x71594535u

inline uint iq4_kv_byte(uint n) {
    const uint w = n < 8u ? (n < 4u ? IQ4W0 : IQ4W1) : (n < 12u ? IQ4W2 : IQ4W3);
    return (w >> (8u * (n & 3u))) & 0xFFu;
}

// the four int8 kvalues of the nibbles at bit `sh` of w's four bytes, packed a byte each
inline uint iq4_reg4(uint w, uint sh) {
    uint r = 0u;
    _Pragma("clang loop unroll(full)")
    for (uint e = 0; e < 4u; ++e) {
        r |= iq4_kv_byte((w >> (8u * e + sh)) & 15u) << (8u * e);
    }
    return r;
}

template <typename TL>
inline void iq4_fill(thread TL& Lw, uint base, uint w, threadgroup const char* kvt, threadgroup const ushort* kv2) {
#if IQ_LUT == 0
    const char4 lo = as_type<char4>(iq4_reg4(w, 0u));
    const char4 hi = as_type<char4>(iq4_reg4(w, 4u));
    _Pragma("clang loop unroll(full)")
    for (uint e = 0; e < 4u; ++e) {
        Lw[base + e] = lo[e];
        Lw[base + 8u + e] = hi[e];
    }
#elif IQ_LUT == 1
    _Pragma("clang loop unroll(full)")
    for (uint e = 0; e < 4u; ++e) {
        Lw[base + e] = kvt[(w >> (8u * e)) & 15u];
        Lw[base + 8u + e] = kvt[(w >> (8u * e + 4u)) & 15u];
    }
#else
    _Pragma("clang loop unroll(full)")
    for (uint e = 0; e < 4u; ++e) {
        const ushort v = kv2[(w >> (8u * e)) & 0xFFu];
        Lw[base + e] = as_type<char>(uchar(v & 0xFFu));
        Lw[base + 8u + e] = as_type<char>(uchar(v >> 8));
    }
#endif
}

kernel void iq4_rows8_tg(device const uint4* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                         device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]], constant uint& NSK [[buffer(6)]],
                         device const float* xs [[buffer(7)]],
                         uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]],
                         uint tid [[thread_index_in_threadgroup]]) {
    threadgroup float red[TG_KS * TG_RS * 256];
    threadgroup char kvt[16];
    threadgroup ushort kv2[256];
#if IQ_LUT == 1
    if (tid < 16u) {
        kvt[tid] = as_type<char>(uchar(iq4_kv_byte(tid)));
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
#elif IQ_LUT == 2
    for (uint i = tid; i < 256u; i += 32u * TG_RS * TG_KS) {
        kv2[i] = ushort(iq4_kv_byte(i & 15u) | (iq4_kv_byte(i >> 4) << 8));
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
#endif
    const uint nsb = K / 256u;
    const uint rs = sgid % TG_RS, ks = sgid / TG_RS;
    const uint n0 = tgid.x * (16u * TG_RS) + rs * 16u;
    const bool rows_ok = n0 < N;
    const uint m0 = ((lane >> 1) & 3u) + 4u * ((lane >> 4) & 1u);
    const uint kq = (lane & 1u) + 2u * ((lane >> 3) & 1u);
    const bool pad = ((lane >> 3) & 1u) != 0u;
    const uint ra = n0 + m0;
    const uint rb = ra + 8u;
    const TgRange gr = tg_range(nsb * (256u / IQ_BLK), tgid.z, ks, NSK, rows_ok);
    const device half* dt = (const device half*)(sc + N * nsb);
    const device uint* scw = (const device uint*)sc;
    constexpr auto desc = matmul2d_descriptor(16, 8, 32, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<int8_t, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half*)T, dextents<int32_t, 2>(K, 8));
    float acc[8];
    for (uint i = 0; i < 8; ++i) {
        acc[i] = 0.0f;
    }
    const uint tq = pad ? 0u : 4u * kq;
    for (uint g = gr.beg; g < gr.end; g += gr.step) {
        const uint sb = g / (256u / IQ_BLK);
        const uint blka = ra * nsb + sb;
        const uint blkb = rb * nsb + sb;
        const float da = float(dt[blka]), db = float(dt[blkb]);
        _Pragma("clang loop unroll(full)")
        for (uint jj = 0; jj < IQ_BLK / 32u; ++jj) {
            const uint js = IQ_BLK == 32u ? (g & 7u) : jj;
            const float fa = da * float(as_type<char4>(scw[blka * 4u + (js >> 2)])[js & 3u]);
            const float fb = db * float(as_type<char4>(scw[blkb * 4u + (js >> 2)])[js & 3u]);
            const uint wa = qs[blka * 32u + js * 4u + kq];
            const uint wb = qs[blkb * 32u + js * 4u + kq];
            iq4_fill(Lw, 0u, wa, kvt, kv2);
            iq4_fill(Lw, 4u, wb, kvt, kv2);
            auto Rx = tX.template slice<32, 8>(int(sb * 256u + 32u * js), 0);
            auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
            op.run(Lw, Rx, d);
            for (uint i = 0; i < 4; ++i) {
                acc[i] = fma(fa, d[i], acc[i]);
                acc[4u + i] = fma(fb, d[4u + i], acc[4u + i]);
            }
        }
    }
    tg_reduce_store(red, acc, y, tgid.z, N, tq, ra, rb, rs, ks, lane, rows_ok && !pad);
}

// the IQ4_XS one-row reference: lane (ix = lane >> 3, js = lane & 7) takes sub-block js of superblocks ix, ix + 4, ..,
// two rows a simdgroup, the codebook a 16-entry threadgroup float table
kernel void iq4_gemv1(device const uint4* sc [[buffer(0)]], device const uint* qs [[buffer(1)]], device const half* T [[buffer(2)]],
                      device float* y [[buffer(3)]], constant uint& K [[buffer(4)]], constant uint& N [[buffer(5)]],
                      uint3 tgid [[threadgroup_position_in_grid]], uint sgid [[simdgroup_index_in_threadgroup]], uint lane [[thread_index_in_simdgroup]],
                      uint tid [[thread_index_in_threadgroup]]) {
    threadgroup float kvf[16];
    if (tid < 16u) {
        kvf[tid] = float(as_type<char>(uchar(iq4_kv_byte(tid))));
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    const uint nsb = K / 256u;
    const uint r0 = (tgid.x * 4u + sgid) * 2u;
    const uint ix = lane >> 3, js = lane & 7u;
    const device half* dt = (const device half*)(sc + N * nsb);
    const device char* scb = (const device char*)sc;
    float s[2] = {0.0f, 0.0f};
    for (uint sb = ix; sb < nsb; sb += 4u) {
        const device half4* xp = (const device half4*)(T + sb * 256u + 32u * js);
        float4 xv[8];
        _Pragma("clang loop unroll(full)")
        for (uint u = 0; u < 8; ++u) {
            xv[u] = float4(xp[u]);
        }
        _Pragma("clang loop unroll(full)")
        for (uint rr = 0; rr < 2; ++rr) {
            const uint blk = (r0 + rr) * nsb + sb;
            const uint4 w = *(const device uint4*)(qs + blk * 32u + js * 4u);
            float acc = 0.0f;
            _Pragma("clang loop unroll(full)")
            for (uint k = 0; k < 4; ++k) {
                const uint wk = w[k];
                const float4 lo = float4(kvf[wk & 15u], kvf[(wk >> 8) & 15u], kvf[(wk >> 16) & 15u], kvf[(wk >> 24) & 15u]);
                const float4 hi = float4(kvf[(wk >> 4) & 15u], kvf[(wk >> 12) & 15u], kvf[(wk >> 20) & 15u], kvf[(wk >> 28) & 15u]);
                acc += dot(lo, xv[k]) + dot(hi, xv[4u + k]);
            }
            s[rr] = fma(float(dt[blk]) * float(scb[blk * 16u + js]), acc, s[rr]);
        }
    }
    for (uint rr = 0; rr < 2; ++rr) {
        const float t = simd_sum(s[rr]);
        if (lane == 0u) {
            y[r0 + rr] = t;
        }
    }
}
