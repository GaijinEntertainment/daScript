// kernel_flash_attn_ext monomorphized by hand from the reference engine's Metal flash
// attention (the lab-arm discipline): DK = DV = 72, Q = 8 queries/tg, C = 64 keys/iter,
// NSG = 4 simdgroups, all-float device types, no mask / sinks / bias / softcap - the
// bidirectional vision-tower shape. Q/K/V read the towers' COMPACT [pos x heads*72] layout
// (row stride args.qd / args.kv_dim); dst writes the same compact layout.
// K rows walk unguarded in C-blocks, so npos must be % 64 and the V buffer carries
// >= 64*ns20 floats of tail slack (dead PV tiles read past the last row by design).
#include <metal_stdlib>
using namespace metal;

struct Args {
    uint  qd;
    uint  kv_dim;
    uint  head_size;
    uint  kv_mul;
    uint  npos;
    uint  np32;
    float scale;
    uint  qoff;
    uint  qrows;
    uint  window;
    float softcap;
    uint  hass;
    uint  uend;
    uint  ulo;
};

#define DK   72
#define DV   72
#define DK4  18
#define DK8  9
#define DV4  18
#define PV   128
#define PV4  32
#define PV8  16
#define QQ   8
#define CC   64
#define NSG  4
#define NW   32
#define NQ   (QQ/NSG)
#define SH   (2*CC)

// the hc variant: K/V PER-HEAD CONTIGUOUS [heads x np x 72] (ld = 72, the reference's own
// tensor layout) - isolates the strided-layout cache tax the h variant pays
kernel void lab_lcpp_flash72hc(
        device const float * q    [[buffer(0)]],
        device const half  * k    [[buffer(1)]],
        device const half  * v    [[buffer(2)]],
        device       float * dst  [[buffer(3)]],
        constant Args      & args [[buffer(4)]],
        threadgroup  float * shm  [[threadgroup(0)]],
        uint3  tgpig [[threadgroup_position_in_grid]],
        ushort tiisg [[thread_index_in_simdgroup]],
        ushort sgitg [[simdgroup_index_in_threadgroup]]) {
    const int iq2 = tgpig.y;
    const int iq1 = tgpig.x * QQ;
    const int NS10 = DK;
    const int NS20 = DV;

    threadgroup half  * sq  = (threadgroup half *) shm;
    threadgroup half4 * sq4 = (threadgroup half4 *) shm;
    threadgroup float  * so  = shm + (QQ*DK)/2;
    threadgroup float4 * so4 = (threadgroup float4 *)(shm + (QQ*DK)/2);
    threadgroup float  * ss  = shm + (QQ*DK)/2 + QQ*PV;
    threadgroup float2 * ss2 = (threadgroup float2 *)(shm + (QQ*DK)/2 + QQ*PV);

    for (short jj = 0; jj < NQ; ++jj) {
        const short j = jj*NSG + sgitg;
        device const float4 * q4 = (device const float4 *)(q + (iq1 + j)*args.qd + iq2*DK);
        for (short i = tiisg; i < DK4; i += NW) {
            sq4[j*DK4 + i] = (iq1 + j < (int) args.npos) ? half4(q4[i]) : half4(0.0h);
        }
        for (short i = tiisg; i < DV4; i += NW) {
            so4[j*PV4 + i] = float4(0.0f);
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);

    float S[NQ] = { 0.0f, 0.0f };
    float M[NQ] = { -FLT_MAX/2, -FLT_MAX/2 };

    for (int ic = 0; ic < (int) args.npos; ic += CC) {
        {
            device const half * pk = k + iq2*(int) args.np32*DK + ic*NS10 + sgitg*(8*NS10);
            threadgroup const half * pq = sq;
            threadgroup float * ps = ss + sgitg*8;
            for (short cc = 0; cc < 2; ++cc) {
                simdgroup_float8x8 mqk = make_filled_simdgroup_matrix<float, 8>(0.0f);
                for (short i = 0; i < DK8; ++i) {
                    simdgroup_half8x8 mk;
                    simdgroup_half8x8 mq;
                    simdgroup_barrier(mem_flags::mem_none);
                    simdgroup_load(mk, pk + 8*i, NS10, 0, true);
                    simdgroup_load(mq, pq + 8*i, DK);
                    simdgroup_barrier(mem_flags::mem_none);
                    simdgroup_multiply_accumulate(mqk, mq, mk, mqk);
                }
                simdgroup_store(mqk, ps, SH, 0, false);
                pk += 8*(NSG*NS10);
                ps += 8*NSG;
            }
        }
        threadgroup_barrier(mem_flags::mem_threadgroup);

        for (short jj = 0; jj < NQ; ++jj) {
            const short j = jj*NSG + sgitg;
            const float m = M[jj];
            float2 s2 = ss2[j*(SH/2) + tiisg]*args.scale;
            M[jj] = simd_max(max(M[jj], max(s2[0], s2[1])));
            const float  ms  = exp(m  - M[jj]);
            const float2 vs2 = exp(s2 - M[jj]);
            S[jj] = S[jj]*ms + simd_sum(vs2[0] + vs2[1]);
            ss2[j*(SH/2) + tiisg] = vs2;
            for (short i = tiisg; i < DV4; i += NW) {
                so4[j*PV4 + i] *= ms;
            }
        }
        threadgroup_barrier(mem_flags::mem_threadgroup);

        {
            simdgroup_float8x8 lo[4];
            {
                threadgroup float * sot = so + 8*sgitg;
                for (short ii = 0; ii < 4; ++ii) {
                    simdgroup_load(lo[ii], sot, PV, 0, false);
                    sot += 8*NSG;
                }
            }
            {
                device const half * pv = v + iq2*(int) args.np32*DV + ic*NS20 + 8*sgitg;
                for (short cc = 0; cc < 4; ++cc) {
                    simdgroup_float8x8 vs[2];
                    simdgroup_load(vs[0], ss + 16*cc + 0, SH, 0, false);
                    simdgroup_load(vs[1], ss + 16*cc + 8, SH, 0, false);
                    for (short ii = 0; ii < 2; ++ii) {
                        simdgroup_half8x8 mv[4];
                        simdgroup_load(mv[0], pv + 0*NSG + 16*ii*NSG + 0*8*NS20, NS20, 0, false);
                        simdgroup_load(mv[1], pv + 8*NSG + 16*ii*NSG + 0*8*NS20, NS20, 0, false);
                        simdgroup_load(mv[2], pv + 0*NSG + 16*ii*NSG + 1*8*NS20, NS20, 0, false);
                        simdgroup_load(mv[3], pv + 8*NSG + 16*ii*NSG + 1*8*NS20, NS20, 0, false);
                        simdgroup_multiply_accumulate(lo[2*ii + 0], vs[0], mv[0], lo[2*ii + 0]);
                        simdgroup_multiply_accumulate(lo[2*ii + 1], vs[0], mv[1], lo[2*ii + 1]);
                        simdgroup_multiply_accumulate(lo[2*ii + 0], vs[1], mv[2], lo[2*ii + 0]);
                        simdgroup_multiply_accumulate(lo[2*ii + 1], vs[1], mv[3], lo[2*ii + 1]);
                    }
                    pv += 2*8*NS20;
                }
            }
            {
                threadgroup float * sot = so + 8*sgitg;
                for (short ii = 0; ii < 4; ++ii) {
                    simdgroup_store(lo[ii], sot, PV, 0, false);
                    sot += 8*NSG;
                }
            }
        }
        threadgroup_barrier(mem_flags::mem_threadgroup);
    }

    for (short jj = 0; jj < NQ; ++jj) {
        const short j = jj*NSG + sgitg;
        if (iq1 + j >= (int) args.npos) {
            break;
        }
        device float4 * dst4 = (device float4 *)(dst + (iq1 + j)*args.qd + iq2*DV);
        const float scale = S[jj] == 0.0f ? 0.0f : 1.0f/S[jj];
        for (short i = tiisg; i < DV4; i += NW) {
            dst4[i] = so4[j*PV4 + i]*scale;
        }
    }
}

// the reference engine's own data path (FA_TYPES): Q staged f32 -> half, K/V device HALF,
// half8x8 operands with float accumulation - the f16 twin of lab_lcpp_flash72 below
kernel void lab_lcpp_flash72h(
        device const float * q    [[buffer(0)]],
        device const half  * k    [[buffer(1)]],
        device const half  * v    [[buffer(2)]],
        device       float * dst  [[buffer(3)]],
        constant Args      & args [[buffer(4)]],
        threadgroup  float * shm  [[threadgroup(0)]],
        uint3  tgpig [[threadgroup_position_in_grid]],
        ushort tiisg [[thread_index_in_simdgroup]],
        ushort sgitg [[simdgroup_index_in_threadgroup]]) {
    const int iq2 = tgpig.y;
    const int iq1 = tgpig.x * QQ;
    const int NS10 = args.kv_dim;
    const int NS20 = args.kv_dim;

    threadgroup half  * sq  = (threadgroup half *) shm;
    threadgroup half4 * sq4 = (threadgroup half4 *) shm;
    threadgroup float  * so  = shm + (QQ*DK)/2;
    threadgroup float4 * so4 = (threadgroup float4 *)(shm + (QQ*DK)/2);
    threadgroup float  * ss  = shm + (QQ*DK)/2 + QQ*PV;
    threadgroup float2 * ss2 = (threadgroup float2 *)(shm + (QQ*DK)/2 + QQ*PV);

    for (short jj = 0; jj < NQ; ++jj) {
        const short j = jj*NSG + sgitg;
        device const float4 * q4 = (device const float4 *)(q + (iq1 + j)*args.qd + iq2*DK);
        for (short i = tiisg; i < DK4; i += NW) {
            sq4[j*DK4 + i] = (iq1 + j < (int) args.npos) ? half4(q4[i]) : half4(0.0h);
        }
        for (short i = tiisg; i < DV4; i += NW) {
            so4[j*PV4 + i] = float4(0.0f);
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);

    float S[NQ] = { 0.0f, 0.0f };
    float M[NQ] = { -FLT_MAX/2, -FLT_MAX/2 };

    for (int ic = 0; ic < (int) args.npos; ic += CC) {
        {
            device const half * pk = k + ic*NS10 + iq2*DK + sgitg*(8*NS10);
            threadgroup const half * pq = sq;
            threadgroup float * ps = ss + sgitg*8;
            for (short cc = 0; cc < 2; ++cc) {
                simdgroup_float8x8 mqk = make_filled_simdgroup_matrix<float, 8>(0.0f);
                for (short i = 0; i < DK8; ++i) {
                    simdgroup_half8x8 mk;
                    simdgroup_half8x8 mq;
                    simdgroup_barrier(mem_flags::mem_none);
                    simdgroup_load(mk, pk + 8*i, NS10, 0, true);
                    simdgroup_load(mq, pq + 8*i, DK);
                    simdgroup_barrier(mem_flags::mem_none);
                    simdgroup_multiply_accumulate(mqk, mq, mk, mqk);
                }
                simdgroup_store(mqk, ps, SH, 0, false);
                pk += 8*(NSG*NS10);
                ps += 8*NSG;
            }
        }
        threadgroup_barrier(mem_flags::mem_threadgroup);

        for (short jj = 0; jj < NQ; ++jj) {
            const short j = jj*NSG + sgitg;
            const float m = M[jj];
            float2 s2 = ss2[j*(SH/2) + tiisg]*args.scale;
            M[jj] = simd_max(max(M[jj], max(s2[0], s2[1])));
            const float  ms  = exp(m  - M[jj]);
            const float2 vs2 = exp(s2 - M[jj]);
            S[jj] = S[jj]*ms + simd_sum(vs2[0] + vs2[1]);
            ss2[j*(SH/2) + tiisg] = vs2;
            for (short i = tiisg; i < DV4; i += NW) {
                so4[j*PV4 + i] *= ms;
            }
        }
        threadgroup_barrier(mem_flags::mem_threadgroup);

        {
            simdgroup_float8x8 lo[4];
            {
                threadgroup float * sot = so + 8*sgitg;
                for (short ii = 0; ii < 4; ++ii) {
                    simdgroup_load(lo[ii], sot, PV, 0, false);
                    sot += 8*NSG;
                }
            }
            {
                device const half * pv = v + ic*NS20 + iq2*DV + 8*sgitg;
                for (short cc = 0; cc < 4; ++cc) {
                    simdgroup_float8x8 vs[2];
                    simdgroup_load(vs[0], ss + 16*cc + 0, SH, 0, false);
                    simdgroup_load(vs[1], ss + 16*cc + 8, SH, 0, false);
                    for (short ii = 0; ii < 2; ++ii) {
                        simdgroup_half8x8 mv[4];
                        simdgroup_load(mv[0], pv + 0*NSG + 16*ii*NSG + 0*8*NS20, NS20, 0, false);
                        simdgroup_load(mv[1], pv + 8*NSG + 16*ii*NSG + 0*8*NS20, NS20, 0, false);
                        simdgroup_load(mv[2], pv + 0*NSG + 16*ii*NSG + 1*8*NS20, NS20, 0, false);
                        simdgroup_load(mv[3], pv + 8*NSG + 16*ii*NSG + 1*8*NS20, NS20, 0, false);
                        simdgroup_multiply_accumulate(lo[2*ii + 0], vs[0], mv[0], lo[2*ii + 0]);
                        simdgroup_multiply_accumulate(lo[2*ii + 1], vs[0], mv[1], lo[2*ii + 1]);
                        simdgroup_multiply_accumulate(lo[2*ii + 0], vs[1], mv[2], lo[2*ii + 0]);
                        simdgroup_multiply_accumulate(lo[2*ii + 1], vs[1], mv[3], lo[2*ii + 1]);
                    }
                    pv += 2*8*NS20;
                }
            }
            {
                threadgroup float * sot = so + 8*sgitg;
                for (short ii = 0; ii < 4; ++ii) {
                    simdgroup_store(lo[ii], sot, PV, 0, false);
                    sot += 8*NSG;
                }
            }
        }
        threadgroup_barrier(mem_flags::mem_threadgroup);
    }

    for (short jj = 0; jj < NQ; ++jj) {
        const short j = jj*NSG + sgitg;
        if (iq1 + j >= (int) args.npos) {
            break;
        }
        device float4 * dst4 = (device float4 *)(dst + (iq1 + j)*args.qd + iq2*DV);
        const float scale = S[jj] == 0.0f ? 0.0f : 1.0f/S[jj];
        for (short i = tiisg; i < DV4; i += NW) {
            dst4[i] = so4[j*PV4 + i]*scale;
        }
    }
}

kernel void lab_lcpp_flash72(
        device const float * q    [[buffer(0)]],
        device const float * k    [[buffer(1)]],
        device const float * v    [[buffer(2)]],
        device       float * dst  [[buffer(3)]],
        constant Args      & args [[buffer(4)]],
        threadgroup  float * shm  [[threadgroup(0)]],
        uint3  tgpig [[threadgroup_position_in_grid]],
        ushort tiisg [[thread_index_in_simdgroup]],
        ushort sgitg [[simdgroup_index_in_threadgroup]]) {
    const int iq2 = tgpig.y;        // head
    const int iq1 = tgpig.x * QQ;   // first query row of this tg
    const int NS10 = args.kv_dim;   // K row stride (floats)
    const int NS20 = args.kv_dim;   // V row stride (floats)

    threadgroup float  * sq  = shm;
    threadgroup float4 * sq4 = (threadgroup float4 *) shm;
    threadgroup float  * so  = shm + QQ*DK;
    threadgroup float4 * so4 = (threadgroup float4 *)(shm + QQ*DK);
    threadgroup float  * ss  = shm + QQ*DK + QQ*PV;
    threadgroup float2 * ss2 = (threadgroup float2 *)(shm + QQ*DK + QQ*PV);

    // stage Q, zero O
    for (short jj = 0; jj < NQ; ++jj) {
        const short j = jj*NSG + sgitg;
        device const float4 * q4 = (device const float4 *)(q + (iq1 + j)*args.qd + iq2*DK);
        for (short i = tiisg; i < DK4; i += NW) {
            sq4[j*DK4 + i] = (iq1 + j < (int) args.npos) ? q4[i] : float4(0.0f);
        }
        for (short i = tiisg; i < DV4; i += NW) {
            so4[j*PV4 + i] = float4(0.0f);
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);

    float S[NQ] = { 0.0f, 0.0f };
    float M[NQ] = { -FLT_MAX/2, -FLT_MAX/2 };

    for (int ic = 0; ic < (int) args.npos; ic += CC) {
        // Q*K^T: each simdgroup computes (C/8)/NSG = 2 8-column score tiles, K straight
        // from device memory via transposed simdgroup loads
        {
            device const float * pk = k + ic*NS10 + iq2*DK + sgitg*(8*NS10);
            threadgroup const float * pq = sq;
            threadgroup float * ps = ss + sgitg*8;
            for (short cc = 0; cc < 2; ++cc) {
                simdgroup_float8x8 mqk = make_filled_simdgroup_matrix<float, 8>(0.0f);
                // DK % 16 != 0: the single-tile walk, 9 8-wide chunks
                for (short i = 0; i < DK8; ++i) {
                    simdgroup_float8x8 mk;
                    simdgroup_float8x8 mq;
                    simdgroup_barrier(mem_flags::mem_none);
                    simdgroup_load(mk, pk + 8*i, NS10, 0, true);
                    simdgroup_load(mq, pq + 8*i, DK);
                    simdgroup_barrier(mem_flags::mem_none);
                    simdgroup_multiply_accumulate(mqk, mq, mk, mqk);
                }
                simdgroup_store(mqk, ps, SH, 0, false);
                pk += 8*(NSG*NS10);
                ps += 8*NSG;
            }
        }
        threadgroup_barrier(mem_flags::mem_threadgroup);

        // online softmax over this C block (no mask): each simdgroup owns NQ query rows
        for (short jj = 0; jj < NQ; ++jj) {
            const short j = jj*NSG + sgitg;
            const float m = M[jj];
            float2 s2 = ss2[j*(SH/2) + tiisg]*args.scale;
            M[jj] = simd_max(max(M[jj], max(s2[0], s2[1])));
            const float  ms  = exp(m  - M[jj]);
            const float2 vs2 = exp(s2 - M[jj]);
            S[jj] = S[jj]*ms + simd_sum(vs2[0] + vs2[1]);
            ss2[j*(SH/2) + tiisg] = vs2;
            for (short i = tiisg; i < DV4; i += NW) {
                so4[j*PV4 + i] *= ms;
            }
        }
        threadgroup_barrier(mem_flags::mem_threadgroup);

        // O += P*V: PV8/NSG = 4 column tiles per simdgroup, V straight from device memory
        {
            simdgroup_float8x8 lo[4];
            {
                threadgroup float * sot = so + 8*sgitg;
                for (short ii = 0; ii < 4; ++ii) {
                    simdgroup_load(lo[ii], sot, PV, 0, false);
                    sot += 8*NSG;
                }
            }
            {
                device const float * pv = v + ic*NS20 + iq2*DV + 8*sgitg;
                for (short cc = 0; cc < 4; ++cc) {
                    simdgroup_float8x8 vs[2];
                    simdgroup_load(vs[0], ss + 16*cc + 0, SH, 0, false);
                    simdgroup_load(vs[1], ss + 16*cc + 8, SH, 0, false);
                    for (short ii = 0; ii < 2; ++ii) {
                        simdgroup_float8x8 mv[4];
                        simdgroup_load(mv[0], pv + 0*NSG + 16*ii*NSG + 0*8*NS20, NS20, 0, false);
                        simdgroup_load(mv[1], pv + 8*NSG + 16*ii*NSG + 0*8*NS20, NS20, 0, false);
                        simdgroup_load(mv[2], pv + 0*NSG + 16*ii*NSG + 1*8*NS20, NS20, 0, false);
                        simdgroup_load(mv[3], pv + 8*NSG + 16*ii*NSG + 1*8*NS20, NS20, 0, false);
                        simdgroup_multiply_accumulate(lo[2*ii + 0], vs[0], mv[0], lo[2*ii + 0]);
                        simdgroup_multiply_accumulate(lo[2*ii + 1], vs[0], mv[1], lo[2*ii + 1]);
                        simdgroup_multiply_accumulate(lo[2*ii + 0], vs[1], mv[2], lo[2*ii + 0]);
                        simdgroup_multiply_accumulate(lo[2*ii + 1], vs[1], mv[3], lo[2*ii + 1]);
                    }
                    pv += 2*8*NS20;
                }
            }
            {
                threadgroup float * sot = so + 8*sgitg;
                for (short ii = 0; ii < 4; ++ii) {
                    simdgroup_store(lo[ii], sot, PV, 0, false);
                    sot += 8*NSG;
                }
            }
        }
        threadgroup_barrier(mem_flags::mem_threadgroup);
    }

    // normalize and store the compact [pos x heads*DV] rows
    for (short jj = 0; jj < NQ; ++jj) {
        const short j = jj*NSG + sgitg;
        if (iq1 + j >= (int) args.npos) {
            break;
        }
        device float4 * dst4 = (device float4 *)(dst + (iq1 + j)*args.qd + iq2*DV);
        const float scale = S[jj] == 0.0f ? 0.0f : 1.0f/S[jj];
        for (short i = tiisg; i < DV4; i += NW) {
            dst4[i] = so4[j*PV4 + i]*scale;
        }
    }
}
