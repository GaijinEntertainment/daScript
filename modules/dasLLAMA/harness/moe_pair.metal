// MOE PAIR RACE kernels (moe_pair_race.das): the routed experts' eight-row pair - gate + up + act, down + shared down +
// combine - on the iq4xs split planes. `gu_base` / `dn_base` are the engine's MetalMoeGemvGuIq4xs / MetalMoeDown2Iq4xs as
// generated; the other entries are the raced forms, every one bound the same way.
#include <metal_stdlib>
using namespace metal;

struct MoeGemvArgs {
    uint ndim;
    uint ddim;
    uint eplane;
    uint xs;
    uint ss;
    uint bxs;
    uint bys;
    uint hasb;
    uint nsh;
    uint gstr;
    uint goff;
    uint yadd;
};

#ifndef DN_K
#define DN_K 8
#endif

static inline float iq4_lut(uint q) {
    uint w = ((q < 8u) ? ((q < 4u) ? 3215825025u : 4142587343u) : ((q < 12u) ? 639175937u : 1901675829u));
    return float(((int(((w >> ((q & 3u) * 8u)) & 255u)) ^ 128) - 128));
}

static inline float silu_mul(float v, float u) {
    return (v / (exp((-v)) + 1.0f)) * u;
}

static inline float sig_gate(float x, float g) {
    return (x / (exp((-g)) + 1.0f));
}

#ifdef DIAG_NOSUM   // diagnostic: no cross-lane reduce (wrong values, the reduce's cost out)
#define simd_sum(v) (v)
#endif
#ifdef DIAG_NOLUT   // diagnostic: the nibbles as floats, no codebook read (wrong values, the decode's cost out)
#define LUT4LO(q) float4(float((q) & 15u), float(((q) >> 8u) & 15u), float(((q) >> 16u) & 15u), float(((q) >> 24u) & 15u))
#define LUT4HI(q) float4(float(((q) >> 4u) & 15u), float(((q) >> 12u) & 15u), float(((q) >> 20u) & 15u), float((q) >> 28u))
#else
#define LUT4LO(q) float4(lut[(q) & 15u], lut[((q) >> 8u) & 15u], lut[((q) >> 16u) & 15u], lut[((q) >> 24u) & 15u])
#define LUT4HI(q) float4(lut[((q) >> 4u) & 15u], lut[((q) >> 12u) & 15u], lut[((q) >> 20u) & 15u], lut[(q) >> 28u])
#endif

// the engine's row pair of one plane: sixteen lanes a superblock, two superblocks apart
static inline void plane_dot(device const half* kdh, device const uchar* kscb, device const uint* kqu, device const float4* x,
                             threadgroup float* lut, uint eb, uint xo4, uint first_row, uint nb, uint lane, thread float& p0, thread float& p1) {
    uint ix = lane / 16u;
    uint it = lane % 16u;
    uint bu = it / 2u;
    uint il = it % 2u;
    float sumf[2] = {};
    uint ib = ix;
    while (ib < nb) {
        uint yb4 = xo4 + ib * 64u + bu * 8u + il * 2u;
        float4 y0 = x[yb4];
        float4 y1 = x[yb4 + 1u];
        float4 y2 = x[yb4 + 4u];
        float4 y3 = x[yb4 + 5u];
        #pragma clang loop unroll(full)
        for (int r = 0; r < 2; ++r) {
            uint blk = eb + (first_row + uint(r)) * nb + ib;
            uint qb = blk * 32u + bu * 4u + il * 2u;
            uint q0 = kqu[qb];
            uint q1 = kqu[qb + 1u];
            float4 acc = y0 * LUT4LO(q0);
            acc += y2 * LUT4HI(q0);
            acc += y1 * LUT4LO(q1);
            acc += y3 * LUT4HI(q1);
            uint si = blk * 16u + bu;
            float sc = float((int(uint(kscb[si])) ^ 128) - 128);
            sumf[r] += ((float(kdh[blk]) * sc) * (((acc.x + acc.y) + acc.z) + acc.w));
        }
        ib += 2u;
    }
    p0 = sumf[0];
    p1 = sumf[1];
}

#define GU_ARGS device const half* kdh [[buffer(0)]], device const uchar* kscb [[buffer(1)]], device const uint* kqu [[buffer(2)]], \
    device const float4* x [[buffer(3)]], device float* y [[buffer(4)]], constant MoeGemvArgs& ka [[buffer(5)]], \
    device const uint* sel [[buffer(7)]], device const half* kdh1 [[buffer(8)]], device const uchar* kscb1 [[buffer(9)]], \
    device const uint* kqu1 [[buffer(10)]], device const float4* xsh [[buffer(11)]], device const float* lg [[buffer(12)]], \
    device const float* selw [[buffer(13)]], \
    uint3 lid3 [[thread_position_in_threadgroup]], uint sgid [[simdgroup_index_in_threadgroup]], \
    uint lane [[thread_index_in_simdgroup]], uint3 tg [[threadgroup_position_in_grid]], uint3 ntg [[threadgroups_per_grid]]

kernel void gu_base(GU_ARGS) {
    threadgroup float lut[64];
    uint slot = tg.y;
    uint st = tg.z;
    uint eb = sel[st * ka.ss + slot] * ka.eplane;
    uint xo4 = (st * ka.bxs + slot * ka.xs) >> 2u;
    uint yrow0 = st * ka.bys + slot * ka.ddim;
    lut[lid3.x] = iq4_lut(lid3.x & 15u);
    threadgroup_barrier(mem_flags::mem_threadgroup);
    uint nb = ka.ndim / 256u;
    uint first_row = (tg.x * 2u + sgid) * 2u;
    float pg0, pg1, pu0, pu1;
    plane_dot(kdh, kscb, kqu, x, lut, eb, xo4, first_row, nb, lane, pg0, pg1);
    plane_dot(kdh1, kscb1, kqu1, x, lut, eb, xo4, first_row, nb, lane, pu0, pu1);
    float g0 = simd_sum(pg0);
    float g1 = simd_sum(pg1);
    float u0 = simd_sum(pu0);
    float u1 = simd_sum(pu1);
    if (lane == 0u && first_row < ka.ddim) {
        y[yrow0 + first_row] = silu_mul(g0, u0);
    }
    if (lane == 0u && first_row + 1u < ka.ddim) {
        y[yrow0 + first_row + 1u] = silu_mul(g1, u1);
    }
}

// The streams that picked this (row, slot)'s expert, as a 64-bit (stream, slot) mask the simdgroup's ballot builds (lane l
// tests picks l and l + 32 of the rows' first eight streams). The first stream that picked the expert serves every one.
static inline ulong picks_of(device const uint* sel, uint ss, uint k, uint nst, uint e, uint lane) {
    uint s0 = lane / k;
    uint j0 = lane % k;
    uint s1 = (lane + 32u) / k;
    uint j1 = (lane + 32u) % k;
    bool h0 = s0 < nst && sel[s0 * ss + j0] == e;
    bool h1 = s1 < nst && sel[s1 * ss + j1] == e;
    ulong lo = static_cast<ulong>(simd_ballot(h0)) & 0xFFFFFFFFul;
    ulong hi = static_cast<ulong>(simd_ballot(h1)) & 0xFFFFFFFFul;
    return lo | (hi << 32u);
}

// the dedup form: a threadgroup per (row block, slot, stream) as the base, but only the expert's first picking stream works,
// over every stream that picked it - the plane words and the codebook decode once, each stream's x against them
#ifndef DD_ST
#define DD_ST 8
#endif
static inline void plane_dot_dd(device const half* kdh, device const uchar* kscb, device const uint* kqu, device const float4* x,
                                threadgroup float* lut, uint eb, uint bxs4, uint smask, uint first_row, uint nb, uint lane,
                                thread float (&p)[DD_ST][2]) {
    uint ix = lane / 16u;
    uint it = lane % 16u;
    uint bu = it / 2u;
    uint il = it % 2u;
    #pragma clang loop unroll(full)
    for (int s = 0; s < DD_ST; ++s) {
        p[s][0] = 0.0f;
        p[s][1] = 0.0f;
    }
    uint ib = ix;
    while (ib < nb) {
        float4 w[2][4];
        float dsc[2];
        #pragma clang loop unroll(full)
        for (int r = 0; r < 2; ++r) {
            uint blk = eb + (first_row + uint(r)) * nb + ib;
            uint qb = blk * 32u + bu * 4u + il * 2u;
            uint q0 = kqu[qb];
            uint q1 = kqu[qb + 1u];
            w[r][0] = LUT4LO(q0);
            w[r][1] = LUT4HI(q0);
            w[r][2] = LUT4LO(q1);
            w[r][3] = LUT4HI(q1);
            uint si = blk * 16u + bu;
            float sc = float((int(uint(kscb[si])) ^ 128) - 128);
            dsc[r] = float(kdh[blk]) * sc;
        }
        uint xb = ib * 64u + bu * 8u + il * 2u;
        #pragma clang loop unroll(full)
        for (int s = 0; s < DD_ST; ++s) {
            if ((smask >> uint(s)) & 1u) {
                uint yb4 = uint(s) * bxs4 + xb;
                float4 y0 = x[yb4];
                float4 y1 = x[yb4 + 1u];
                float4 y2 = x[yb4 + 4u];
                float4 y3 = x[yb4 + 5u];
                #pragma clang loop unroll(full)
                for (int r = 0; r < 2; ++r) {
                    float4 acc = y0 * w[r][0];
                    acc += y2 * w[r][1];
                    acc += y1 * w[r][2];
                    acc += y3 * w[r][3];
                    p[s][r] += (dsc[r] * (((acc.x + acc.y) + acc.z) + acc.w));
                }
            }
        }
        ib += 2u;
    }
}

kernel void gu_dd(GU_ARGS) {
    threadgroup float lut[64];
    uint slot = tg.y;
    uint st = tg.z;
    uint k = ka.ss / 2u;
    uint e = sel[st * ka.ss + slot];
    ulong pk = picks_of(sel, ka.ss, k, ntg.z, e, lane);
    ulong kmask = (1ul << ulong(k)) - 1ul;
    if ((pk & ((1ul << ulong(st * k)) - 1ul)) != 0ul) {
        return;   // an earlier stream picked it and serves this one
    }
    uint smask = 0u;
    uint slots[DD_ST];
    #pragma clang loop unroll(full)
    for (int s = 0; s < DD_ST; ++s) {
        ulong b = (pk >> ulong(uint(s) * k)) & kmask;
        smask |= b != 0ul ? (1u << uint(s)) : 0u;
        slots[s] = b != 0ul ? ctz(uint(b)) : 0u;
    }
    lut[lid3.x] = iq4_lut(lid3.x & 15u);
    threadgroup_barrier(mem_flags::mem_threadgroup);
    uint eb = e * ka.eplane;
    uint nb = ka.ndim / 256u;
    uint first_row = (tg.x * 2u + sgid) * 2u;
    float pg[DD_ST][2];
    float pu[DD_ST][2];
    plane_dot_dd(kdh, kscb, kqu, x, lut, eb, ka.bxs >> 2u, smask, first_row, nb, lane, pg);
    plane_dot_dd(kdh1, kscb1, kqu1, x, lut, eb, ka.bxs >> 2u, smask, first_row, nb, lane, pu);
    #pragma clang loop unroll(full)
    for (int s = 0; s < DD_ST; ++s) {
        if ((smask >> uint(s)) & 1u) {
            float g0 = simd_sum(pg[s][0]);
            float g1 = simd_sum(pg[s][1]);
            float u0 = simd_sum(pu[s][0]);
            float u1 = simd_sum(pu[s][1]);
            uint yrow0 = uint(s) * ka.bys + slots[s] * ka.ddim;
            if (lane == 0u && first_row < ka.ddim) {
                y[yrow0 + first_row] = silu_mul(g0, u0);
            }
            if (lane == 0u && first_row + 1u < ka.ddim) {
                y[yrow0 + first_row + 1u] = silu_mul(g1, u1);
            }
        }
    }
}

// R rows of one plane (or of NP planes sharing x) a simdgroup over the engine's lane map: the x float4s loaded once an
// iteration for every row and plane, each row's arithmetic the base's
template <int R, int NP>
static inline void rows_dot(device const half* const* kdh, device const uchar* const* kscb, device const uint* const* kqu, device const float4* x,
                            threadgroup float* lut, uint eb, uint xo4, uint first_row, uint nb, uint lane, thread float (&p)[NP][R]) {
    uint ix = lane / 16u;
    uint it = lane % 16u;
    uint bu = it / 2u;
    uint il = it % 2u;
    #pragma clang loop unroll(full)
    for (int u = 0; u < NP; ++u) {
        #pragma clang loop unroll(full)
        for (int r = 0; r < R; ++r) {
            p[u][r] = 0.0f;
        }
    }
    uint ib = ix;
    while (ib < nb) {
        uint yb4 = xo4 + ib * 64u + bu * 8u + il * 2u;
        float4 y0 = x[yb4];
        float4 y1 = x[yb4 + 1u];
        float4 y2 = x[yb4 + 4u];
        float4 y3 = x[yb4 + 5u];
        #pragma clang loop unroll(full)
        for (int u = 0; u < NP; ++u) {
            #pragma clang loop unroll(full)
            for (int r = 0; r < R; ++r) {
                uint blk = eb + (first_row + uint(r)) * nb + ib;
                uint qb = blk * 32u + bu * 4u + il * 2u;
                uint q0 = kqu[u][qb];
                uint q1 = kqu[u][qb + 1u];
                float4 acc = y0 * LUT4LO(q0);
                acc += y2 * LUT4HI(q0);
                acc += y1 * LUT4LO(q1);
                acc += y3 * LUT4HI(q1);
                uint si = blk * 16u + bu;
                float sc = float((int(uint(kscb[u][si])) ^ 128) - 128);
                p[u][r] += ((float(kdh[u][blk]) * sc) * (((acc.x + acc.y) + acc.z) + acc.w));
            }
        }
        ib += 2u;
    }
}

#ifndef GR
#define GR 2
#endif
// gate and up in one loop (x once for both planes), GR rows a simdgroup, two simdgroups a threadgroup
kernel void gu_rows(GU_ARGS) {
    threadgroup float lut[64];
#ifdef FLAT   // a flat grid, the (stream, slot) pairs fastest: every pair of one row block runs side by side
    uint k = ka.ss / 2u;
    uint nrb = (ka.ddim + 2u * uint(GR) - 1u) / (2u * uint(GR));
    uint nst = ntg.x / (k * nrb);
    uint st = tg.x % nst;
    uint slot = (tg.x / nst) % k;
    uint xb = tg.x / (nst * k);
#else
    uint slot = tg.y;
    uint st = tg.z;
    uint xb = tg.x;
#endif
    uint eb = sel[st * ka.ss + slot] * ka.eplane;
    uint xo4 = (st * ka.bxs + slot * ka.xs) >> 2u;
    uint yrow0 = st * ka.bys + slot * ka.ddim;
    lut[lid3.x] = iq4_lut(lid3.x & 15u);
    threadgroup_barrier(mem_flags::mem_threadgroup);
    uint nb = ka.ndim / 256u;
    uint first_row = (xb * 2u + sgid) * uint(GR);
    device const half* dh[2] = {kdh, kdh1};
    device const uchar* sb[2] = {kscb, kscb1};
    device const uint* qu[2] = {kqu, kqu1};
    float p[2][GR];
    rows_dot<GR, 2>(dh, sb, qu, x, lut, eb, xo4, first_row, nb, lane, p);
    #pragma clang loop unroll(full)
    for (int r = 0; r < GR; ++r) {
        float g = simd_sum(p[0][r]);
        float u = simd_sum(p[1][r]);
        if (lane == 0u && first_row + uint(r) < ka.ddim) {
            y[yrow0 + first_row + uint(r)] = silu_mul(g, u);
        }
    }
}

#ifndef DR
#define DR 2
#endif
// the down at DR rows a simdgroup: the slots in order, each slot's x loaded once for the DR rows
kernel void dn_rows(GU_ARGS) {
    threadgroup float lut[64];
    lut[lid3.x] = iq4_lut(lid3.x & 15u);
    threadgroup_barrier(mem_flags::mem_threadgroup);
#ifdef FLAT   // a flat grid, the streams fastest: every stream of one row block runs side by side
    uint nrb = (ka.ddim + 2u * uint(DR) - 1u) / (2u * uint(DR));
    uint nst = ntg.x / nrb;
    uint st = tg.x % nst;
    uint xb = tg.x / nst;
#else
    uint st = tg.y;
    uint xb = tg.x;
#endif
    uint nb = ka.ndim / 256u;
    uint first_row = (xb * 2u + sgid) * uint(DR);
    uint k = ka.ss / 2u;
    float a[DR];
    #pragma clang loop unroll(full)
    for (int r = 0; r < DR; ++r) {
        a[r] = 0.0f;
    }
    device const half* dh[1] = {kdh};
    device const uchar* sb[1] = {kscb};
    device const uint* qu[1] = {kqu};
    uint slot = 0u;
    while (slot < k) {
        uint eb = sel[st * ka.ss + slot] * ka.eplane;
        uint xo4 = (st * ka.bxs + slot * ka.xs) >> 2u;
        float p[1][DR];
        rows_dot<DR, 1>(dh, sb, qu, x, lut, eb, xo4, first_row, nb, lane, p);
        float w = selw[st * ka.ss + k + slot];
        #pragma clang loop unroll(full)
        for (int r = 0; r < DR; ++r) {
            a[r] += w * simd_sum(p[0][r]);
        }
        ++slot;
    }
    device const half* dh1[1] = {kdh1};
    device const uchar* sb1[1] = {kscb1};
    device const uint* qu1[1] = {kqu1};
    float q[1][DR];
    rows_dot<DR, 1>(dh1, sb1, qu1, xsh, lut, 0u, (st * ka.nsh) >> 2u, first_row, ka.nsh / 256u, lane, q);
    float gl = lg[st * ka.gstr + ka.goff];
    uint yrow0 = st * ka.bys;
    #pragma clang loop unroll(full)
    for (int r = 0; r < DR; ++r) {
        float v = a[r] + sig_gate(simd_sum(q[0][r]), gl);
        uint yi = yrow0 + first_row + uint(r);
        if (lane == 0u && first_row + uint(r) < ka.ddim) {
            y[yi] = ((ka.yadd != 0u) ? y[yi] : 0.0f) + v;
        }
    }
}

// the down with the slots split over the threadgroup's simdgroups: simdgroup s dots slot s's row block (KR rows), the last
// simdgroup the shared expert's too; the combine in slot order off threadgroup memory - the base's sums and order
#ifndef KR
#define KR 2
#endif
kernel void dn_ks(GU_ARGS) {
    threadgroup float lut[64];
    threadgroup float part[DN_K + 1][KR];
    if (lid3.x < 64u) {
        lut[lid3.x] = iq4_lut(lid3.x & 15u);
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    uint st = tg.y;
    uint nb = ka.ndim / 256u;
    uint first_row = tg.x * uint(KR);
    uint k = ka.ss / 2u;
    uint slot = sgid;
    device const half* dh[1] = {kdh};
    device const uchar* sb[1] = {kscb};
    device const uint* qu[1] = {kqu};
    {
        uint eb = sel[st * ka.ss + slot] * ka.eplane;
        uint xo4 = (st * ka.bxs + slot * ka.xs) >> 2u;
        float p[1][KR];
        rows_dot<KR, 1>(dh, sb, qu, x, lut, eb, xo4, first_row, nb, lane, p);
        #pragma clang loop unroll(full)
        for (int r = 0; r < KR; ++r) {
            float s = simd_sum(p[0][r]);
            if (lane == 0u) {
                part[slot][r] = s;
            }
        }
    }
    if (sgid == uint(DN_K) - 1u) {
        device const half* dh1[1] = {kdh1};
        device const uchar* sb1[1] = {kscb1};
        device const uint* qu1[1] = {kqu1};
        float q[1][KR];
        rows_dot<KR, 1>(dh1, sb1, qu1, xsh, lut, 0u, (st * ka.nsh) >> 2u, first_row, ka.nsh / 256u, lane, q);
        #pragma clang loop unroll(full)
        for (int r = 0; r < KR; ++r) {
            float s = simd_sum(q[0][r]);
            if (lane == 0u) {
                part[DN_K][r] = s;
            }
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if (lid3.x < uint(KR) && first_row + lid3.x < ka.ddim) {
        uint r = lid3.x;
        float a = 0.0f;
        for (uint s = 0u; s < k; ++s) {
            a += selw[st * ka.ss + k + s] * part[s][r];
        }
        float gl = lg[st * ka.gstr + ka.goff];
        a += sig_gate(part[DN_K][r], gl);
        uint yi = st * ka.bys + first_row + r;
        y[yi] = ((ka.yadd != 0u) ? y[yi] : 0.0f) + a;
    }
}

// the down deduplicated over the streams: a threadgroup a row block (XR rows) of every stream; the 64 (stream, slot) picks and
// the shared expert (its eight streams the followers of one entry) as leaders, each distinct expert's rows decoded once a
// simdgroup, every picking pair's x dotted against them into the partial sums; the combine per stream in slot order then the
// shared row under its gate - the base's per-pair arithmetic and order
#ifndef XR
#define XR 4
#endif
#define XSG 8
kernel void dn_dd(GU_ARGS) {
    threadgroup float lut[64];
    threadgroup float part[72][XR];
    threadgroup uint picks[64];
    threadgroup uint lead[72];
    threadgroup ulong fol[72];
    threadgroup uint nlead;
    uint t = lid3.x;
    uint k = ka.ss / 2u;
    uint nst = ka.hasb;   // the harness's stream count rides the unread bias flag
    uint np = nst * k;
    if (t < 64u) {
        lut[t] = iq4_lut(t & 15u);
        picks[t] = t < np ? sel[(t / k) * ka.ss + t % k] : 0xFFFFFFFFu;
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if (sgid == 0u) {   // the leaders in pick order, each with its followers' mask, the shared expert last
        uint base = 0u;
        for (uint h = 0u; h < 2u; ++h) {
            uint i = lane + 32u * h;
            uint e = picks[i];
            bool first = i < np;
            ulong m = 0ul;
            for (uint j = 0u; j < np; ++j) {
                bool same = picks[j] == e;
                first = first && !(same && j < i);
                m |= same && j >= i ? (1ul << ulong(j)) : 0ul;
            }
            uint pre = simd_prefix_exclusive_sum(first ? 1u : 0u);
            if (first) {
                lead[base + pre] = i;
                fol[base + pre] = m;
            }
            base += simd_sum(first ? 1u : 0u);
        }
        if (lane == 0u) {
            lead[base] = 64u;
            fol[base] = (1ul << ulong(nst)) - 1ul;
            nlead = base + 1u;
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    uint first_row = tg.x * uint(XR);
    uint ix = lane / 16u;
    uint it = lane % 16u;
    uint bu = it / 2u;
    uint il = it % 2u;
    uint nl = nlead;
    for (uint li = sgid; li < nl; li += uint(XSG)) {
        uint i = lead[li];
        ulong m = fol[li];
        bool sh = i == 64u;
        uint nb = sh ? ka.nsh / 256u : ka.ndim / 256u;
        device const half* dh = sh ? kdh1 : kdh;
        device const uchar* sb = sh ? kscb1 : kscb;
        device const uint* qu = sh ? kqu1 : kqu;
        device const float4* xv = sh ? xsh : x;
        uint eb = sh ? 0u : picks[i] * ka.eplane;
        float4 w[XR][4];
        float dsc[XR];
        uint ib = ix;   // K 512: one superblock a lane half, the base's single iteration
        #pragma clang loop unroll(full)
        for (int r = 0; r < XR; ++r) {
            uint blk = eb + (first_row + uint(r)) * nb + ib;
            uint qb = blk * 32u + bu * 4u + il * 2u;
            uint q0 = qu[qb];
            uint q1 = qu[qb + 1u];
            w[r][0] = LUT4LO(q0);
            w[r][1] = LUT4HI(q0);
            w[r][2] = LUT4LO(q1);
            w[r][3] = LUT4HI(q1);
            uint si = blk * 16u + bu;
            float sc = float((int(uint(sb[si])) ^ 128) - 128);
            dsc[r] = float(dh[blk]) * sc;
        }
        while (m != 0ul) {
            uint j = ctz(m);
            m &= m - 1ul;
            uint xo4 = sh ? (j * ka.nsh) >> 2u : ((j / k) * ka.bxs + (j % k) * ka.xs) >> 2u;
            uint yb4 = xo4 + ib * 64u + bu * 8u + il * 2u;
            float4 y0 = xv[yb4];
            float4 y1 = xv[yb4 + 1u];
            float4 y2 = xv[yb4 + 4u];
            float4 y3 = xv[yb4 + 5u];
            #pragma clang loop unroll(full)
            for (int r = 0; r < XR; ++r) {
                float4 acc = y0 * w[r][0];
                acc += y2 * w[r][1];
                acc += y1 * w[r][2];
                acc += y3 * w[r][3];
                float s = simd_sum(0.0f + (dsc[r] * (((acc.x + acc.y) + acc.z) + acc.w)));
                if (lane == 0u) {
                    part[sh ? 64u + j : j][r] = s;
                }
            }
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if (t < nst * uint(XR)) {
        uint st = t / uint(XR);
        uint r = t % uint(XR);
        if (first_row + r < ka.ddim) {
            float a = 0.0f;
            for (uint s = 0u; s < k; ++s) {
                a += selw[st * ka.ss + k + s] * part[st * k + s][r];
            }
            a += sig_gate(part[64u + st][r], lg[st * ka.gstr + ka.goff]);
            uint yi = st * ka.bys + first_row + r;
            y[yi] = ((ka.yadd != 0u) ? y[yi] : 0.0f) + a;
        }
    }
}

// a read-bandwidth reference: every thread sums uint4s strided over the buffer bound at 2
kernel void bw_read(device const uint4* kqu4 [[buffer(2)]], device float* y [[buffer(4)]], constant uint& n4 [[buffer(6)]],
                    uint gid [[thread_position_in_grid]], uint nthr [[threads_per_grid]]) {
    uint4 s = 0u;
    for (uint i = gid; i < n4; i += nthr) {
        s ^= kqu4[i];
    }
    if ((s.x ^ s.y ^ s.z ^ s.w) == 0x12345678u) {
        y[gid] = 1.0f;
    }
}

kernel void dn_base(GU_ARGS) {
    threadgroup float lut[64];
    lut[lid3.x] = iq4_lut(lid3.x & 15u);
    threadgroup_barrier(mem_flags::mem_threadgroup);
    uint st = tg.y;
    uint nb = ka.ndim / 256u;
    uint first_row = (tg.x * 2u + sgid) * 2u;
    uint k = ka.ss / 2u;
    float a0 = 0.0f;
    float a1 = 0.0f;
    uint slot = 0u;
    while (slot < k) {
        uint eb = sel[st * ka.ss + slot] * ka.eplane;
        uint xo4 = (st * ka.bxs + slot * ka.xs) >> 2u;
        float p0, p1;
        plane_dot(kdh, kscb, kqu, x, lut, eb, xo4, first_row, nb, lane, p0, p1);
        float s0 = simd_sum(p0);
        float s1 = simd_sum(p1);
        float w = selw[st * ka.ss + k + slot];
        a0 += w * s0;
        a1 += w * s1;
        ++slot;
    }
    float q0, q1;
    plane_dot(kdh1, kscb1, kqu1, xsh, lut, 0u, (st * ka.nsh) >> 2u, first_row, ka.nsh / 256u, lane, q0, q1);
    float gl = lg[st * ka.gstr + ka.goff];
    a0 += sig_gate(simd_sum(q0), gl);
    a1 += sig_gate(simd_sum(q1), gl);
    uint yrow0 = st * ka.bys;
    if (lane == 0u && first_row < ka.ddim) {
        uint yi = yrow0 + first_row;
        y[yi] = ((ka.yadd != 0u) ? y[yi] : 0.0f) + a0;
    }
    if (lane == 0u && first_row + 1u < ka.ddim) {
        uint yi1 = yrow0 + first_row + 1u;
        y[yi1] = ((ka.yadd != 0u) ? y[yi1] : 0.0f) + a1;
    }
}

// the down with its slot loop at the compile-time slot count, unrolled, so the slots' plane and x loads are all in flight
#ifndef DN_K
#define DN_K 8
#endif
kernel void dn_unroll(GU_ARGS) {
    threadgroup float lut[64];
    lut[lid3.x] = iq4_lut(lid3.x & 15u);
    threadgroup_barrier(mem_flags::mem_threadgroup);
    uint st = tg.y;
    uint nb = ka.ndim / 256u;
    uint first_row = (tg.x * 2u + sgid) * 2u;
    uint k = ka.ss / 2u;
    float s0[DN_K];
    float s1[DN_K];
    #pragma clang loop unroll(full)
    for (int slot = 0; slot < DN_K; ++slot) {
        uint eb = sel[st * ka.ss + uint(slot)] * ka.eplane;
        uint xo4 = (st * ka.bxs + uint(slot) * ka.xs) >> 2u;
        float p0, p1;
        plane_dot(kdh, kscb, kqu, x, lut, eb, xo4, first_row, nb, lane, p0, p1);
        s0[slot] = simd_sum(p0);
        s1[slot] = simd_sum(p1);
    }
    float q0, q1;
    plane_dot(kdh1, kscb1, kqu1, xsh, lut, 0u, (st * ka.nsh) >> 2u, first_row, ka.nsh / 256u, lane, q0, q1);
    float a0 = 0.0f;
    float a1 = 0.0f;
    #pragma clang loop unroll(full)
    for (int slot = 0; slot < DN_K; ++slot) {
        float w = selw[st * ka.ss + k + uint(slot)];
        a0 += w * s0[slot];
        a1 += w * s1[slot];
    }
    float gl = lg[st * ka.gstr + ka.goff];
    a0 += sig_gate(simd_sum(q0), gl);
    a1 += sig_gate(simd_sum(q1), gl);
    uint yrow0 = st * ka.bys;
    if (lane == 0u && first_row < ka.ddim) {
        uint yi = yrow0 + first_row;
        y[yi] = ((ka.yadd != 0u) ? y[yi] : 0.0f) + a0;
    }
    if (lane == 0u && first_row + 1u < ka.ddim) {
        uint yi1 = yrow0 + first_row + 1u;
        y[yi1] = ((ka.yadd != 0u) ? y[yi1] : 0.0f) + a1;
    }
}
