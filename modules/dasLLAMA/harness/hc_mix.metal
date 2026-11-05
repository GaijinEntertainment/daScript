// HC MIX RACE kernels (hc_mix_race.das): the hyper-connection mixer site over eight wide rows. `hcn_base` / `sglo_base` are the
// engine's one-row pair (MetalQ8GemvHcN, MetalQ8GemvSgLo) as generated; the `*_base` entries after them are today's eight-row
// chain (metal_hc_norm, metal_moe_router, MetalRows8Prep, MetalRows8Q8, MetalHcLo, metal_hc_mix) as generated; the rest are the
// raced eight-row forms.
#include <metal_stdlib>
using namespace metal;
#include <MetalPerformancePrimitives/MetalPerformancePrimitives.h>
using namespace mpp::tensor_ops;

struct Q8HcArgs {
    uint dim;
    uint hc;
    float eps;
};

struct GkHcNormArgs {
    uint dim;
    uint hc;
    uint woff;
    uint row0;
    float eps;
};

struct GkRouterArgs {
    uint ne;
    uint dim;
    uint woff;
    uint obase;
    uint norm_on;
    uint nwoff;
    float eps;
    uint ncols;
    uint ne_split;
    uint obase_split;
    uint col0;
    uint xs;
    uint hasb;
    uint boff;
    float yscale;
};

struct GkHcMixArgs {
    uint dim;
    uint hc;
    uint rows;
};

// the raced forms' shape: the stream width, the stream count, the low rank, the norm's epsilon
struct HcMix8Args {
    uint dim;
    uint hc;
    uint lr;
    float eps;
};

static inline float dot4_seq(float4 a, float4 b) {
    return fma(a.w, b.w, fma(a.z, b.z, fma(a.y, b.y, (a.x * b.x))));
}
static inline float q8_oct_dot(device const char* qv, uint qb, float4 x0, float4 x1) {
    return ((((((((float(qv[qb]) * x0.x) + (float(qv[(qb + 1u)]) * x0.y)) + (float(qv[(qb + 2u)]) * x0.z)) + (float(qv[(qb + 3u)]) * x0.w)) + (float(qv[(qb + 4u)]) * x1.x)) + (float(qv[(qb + 5u)]) * x1.y)) + (float(qv[(qb + 6u)]) * x1.z)) + (float(qv[(qb + 7u)]) * x1.w));
}
static inline float q8_quad_dot(device const char* qv, uint qb, float4 x0) {
    return ((((float(qv[qb]) * x0.x) + (float(qv[(qb + 1u)]) * x0.y)) + (float(qv[(qb + 2u)]) * x0.z)) + (float(qv[(qb + 3u)]) * x0.w));
}
static inline float rms_inv(float ms, bool rsq) {
    return (rsq ? rsqrt(ms) : (1.0f / sqrt(ms)));
}
static inline float sigmoid_f32(float v) {
    return (1.0f / (exp((-v)) + 1.0f));
}
static inline float silu_f32(float v) {
    return (v / (exp((-v)) + 1.0f));
}
static inline uint rows8_kq(uint lane) {
    return ((lane & 1u) + (((lane >> 3u) & 1u) * 2u));
}
static inline uint rows8_m0(uint lane) {
    return (((lane >> 1u) & 3u) + (((lane >> 4u) & 1u) * 4u));
}
static inline bool rows8_pad(uint lane) {
    return (((lane >> 3u) & 1u) != 0u);
}
static inline uint2 rows8_q8_run(device const uchar* wqb, uint base, uint blk, uint kq) {
    uint b0 = (((base + (blk * 34u)) + 2u) + (kq * 8u));
    uint lo = (((uint(wqb[b0]) | (uint(wqb[(b0 + 1u)]) << 8u)) | (uint(wqb[(b0 + 2u)]) << 16u)) | (uint(wqb[(b0 + 3u)]) << 24u));
    uint hi = (((uint(wqb[(b0 + 4u)]) | (uint(wqb[(b0 + 5u)]) << 8u)) | (uint(wqb[(b0 + 6u)]) << 16u)) | (uint(wqb[(b0 + 7u)]) << 24u));
    return uint2(lo, hi);
}

kernel void hcn_base(device const half* wsh [[buffer(0)]],
        device const char* wqb [[buffer(1)]],
        device const float4* res [[buffer(2)]],
        device float* y [[buffer(3)]],
        constant uint& ndim [[buffer(4)]],
        constant uint& ddim [[buffer(5)]],
        device const float4* wn [[buffer(6)]],
        device float4* xn [[buffer(7)]],
        device const float4* wi [[buffer(8)]],
        device float* inj [[buffer(9)]],
        constant Q8HcArgs& ka [[buffer(10)]],
        uint3 gl_LocalInvocationID [[thread_position_in_threadgroup]],
        uint gl_SubgroupID [[simdgroup_index_in_threadgroup]],
        uint gl_SubgroupInvocationID [[thread_index_in_simdgroup]],
        uint3 gl_WorkGroupID [[threadgroup_position_in_grid]],
        threadgroup char * shmem [[threadgroup(0)]]) {
    threadgroup float * stash = (threadgroup float *)(shmem + 768);
    uint sg = gl_SubgroupID;
    uint lane = gl_SubgroupInvocationID;
    uint tid = gl_LocalInvocationID.x;
    uint nb = (ndim / 32u);
    uint nbs = (ka.dim / 32u);
    uint first_row = (gl_WorkGroupID.x * 2u);
    uint ix = (lane / 4u);
    uint il = (lane % 4u);
    bool injrows = (first_row >= ddim);
    uint base_row = (injrows ? (first_row - ddim) : first_row);
    float sumf[8] = {};
    float sq[4] = {};
    uint ib = ((sg * 8u) + ix);
    while ((ib < nb)) {
        uint xi = ((ib * 8u) + (il * 2u));
        uint c = (ib / nbs);
        float4 r0 = res[xi];
        float4 r1 = res[(xi + 1u)];
        sq[c] += (((((r0.x * r0.x) + (r0.y * r0.y)) + (r0.z * r0.z)) + (r0.w * r0.w)) + ((((r1.x * r1.x) + (r1.y * r1.y)) + (r1.z * r1.z)) + (r1.w * r1.w)));
        float4 x0 = (res[xi] * wn[xi]);
        float4 x1 = (res[(xi + 1u)] * wn[(xi + 1u)]);
        #pragma clang loop unroll(full)
        for (int r = 0; r < 2; ++r) {
            if (injrows) {
                uint wb = (((base_row + uint(r)) * (ndim / 4u)) + xi);
                sumf[((uint(r) * 4u) + c)] += (dot4_seq(wi[wb], x0) + dot4_seq(wi[(wb + 1u)], x1));
            } else {
                uint bi = (((base_row + uint(r)) * nb) + ib);
                float d = float(wsh[(bi * 17u)]);
                uint qb = (((bi * 34u) + 2u) + (il * 8u));
                sumf[((uint(r) * 4u) + c)] += (q8_oct_dot(wqb, qb, x0, x1) * d);
            }
        }
        ib += 32u;
    }
    #pragma clang loop unroll(full)
    for (int k = 0; k < 8; ++k) {
        float v = simd_sum(sumf[k]);
        if ((lane == 0u)) {
            stash[((uint(k) * 4u) + sg)] = v;
        }
    }
    #pragma clang loop unroll(full)
    for (int k = 0; k < 4; ++k) {
        float v = simd_sum(sq[k]);
        if ((lane == 0u)) {
            stash[(((uint(k) * 4u) + 32u) + sg)] = v;
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    float inv[4] = {};
    #pragma clang loop unroll(full)
    for (int k = 0; k < 4; ++k) {
        uint ks = (uint(k) + 8u);
        float ssq = ((stash[(ks * 4u)] + stash[((ks * 4u) + 1u)]) + (stash[((ks * 4u) + 2u)] + stash[((ks * 4u) + 3u)]));
        inv[k] = rms_inv(((ssq / float(ka.dim)) + ka.eps), true);
    }
    if ((tid == 0u)) {
        #pragma clang loop unroll(full)
        for (int r = 0; r < 2; ++r) {
            float tot = 0.0f;
            #pragma clang loop unroll(full)
            for (int k = 0; k < 4; ++k) {
                uint kr = ((uint(r) * 4u) + uint(k));
                float pk = ((stash[(kr * 4u)] + stash[((kr * 4u) + 1u)]) + (stash[((kr * 4u) + 2u)] + stash[((kr * 4u) + 3u)]));
                tot += (pk * inv[k]);
            }
            if (injrows) {
                if (((base_row + uint(r)) < ka.hc)) {
                    inj[(base_row + uint(r))] = tot;
                }
            } else if (((first_row + uint(r)) < ddim)) {
                y[(first_row + uint(r))] = tot;
            }
        }
    }
    uint ntg = (ddim / 2u);
    uint n4 = (ndim / 4u);
    uint slice = (((n4 + ntg) - 1u) / ntg);
    if ((gl_WorkGroupID.x < ntg)) {
        uint end = min(((gl_WorkGroupID.x + 1u) * slice), n4);
        uint e = ((gl_WorkGroupID.x * slice) + tid);
        while ((e < end)) {
            xn[e] = ((res[e] * inv[(e / (ka.dim / 4u))]) * wn[e]);
            e += 128u;
        }
    }
}

kernel void sglo_base(device half* wsh [[buffer(0)]],
        device char* wqb [[buffer(1)]],
        device const float4* x [[buffer(2)]],
        device const float* xn [[buffer(3)]],
        constant uint& ndim [[buffer(4)]],
        constant uint& ddim [[buffer(5)]],
        device float* y [[buffer(6)]],
        constant uint& hc [[buffer(7)]],
        uint gl_SubgroupID [[simdgroup_index_in_threadgroup]],
        uint gl_SubgroupInvocationID [[thread_index_in_simdgroup]],
        uint gl_SubgroupSize [[threads_per_simdgroup]],
        uint3 gl_WorkGroupID [[threadgroup_position_in_grid]],
        threadgroup char * shmem [[threadgroup(0)]]) {
    threadgroup float * stash = (threadgroup float *)(shmem);
    uint g0 = ndim;
    device const float4* x0_s2 = (x + gl_SubgroupInvocationID);
    uint row = (((((gl_SubgroupID / (8u / hc)) * (ddim / hc)) + (gl_WorkGroupID.x * (8u / hc))) + (gl_SubgroupID % (8u / hc))));
    if ((row >= (ddim))) {
        return;
    }
    uint lane = gl_SubgroupInvocationID;
    uint nb = (g0 / 32u);
    uint nb4 = (g0 / 4u);
    uint wrow = (row * nb);
    device half * sp = wsh + (((wrow + (lane / 8u)) * 17u));
    device char * qp = wqb + (((((wrow + (lane / 8u)) * 34u) + 2u) + ((lane % 8u) * 4u)));
    float acc = 0.0f;
    float4 lw[4] = {};
    uint k = lane;
    while (((k + 96u) < nb4)) {
        #pragma clang loop unroll(full)
        for (int c = 0; c < 4; ++c) {
            float d = float(sp[(uint(c) * 68u)]);
            uint qb = (uint(c) * 136u);
            lw[c] = (float4(float(qp[qb]), float(qp[(qb + 1u)]), float(qp[(qb + 2u)]), float(qp[(qb + 3u)])) * d);
        }
        #pragma clang loop unroll(full)
        for (int c = 0; c < 4; ++c) {
            float4 xr = x0_s2[(uint(c) * 32u)];
            float4 xv = (float4(silu_f32((xr.x / float(hc))), silu_f32((xr.y / float(hc))), silu_f32((xr.z / float(hc))), silu_f32((xr.w / float(hc)))));
            acc += ((((lw[c].x * xv.x) + (lw[c].y * xv.y)) + (lw[c].z * xv.z)) + (lw[c].w * xv.w));
        }
        sp = (sp + (272));
        qp = (qp + (544));
        x0_s2 = (x0_s2 + (128));
        k += 128u;
    }
    while ((k < nb4)) {
        uint bi = (wrow + (k / 8u));
        float d = float(wsh[(bi * 17u)]);
        uint qb = (((bi * 34u) + 2u) + ((k % 8u) * 4u));
        float4 xr = (x[k]);
        float4 xv = (float4(silu_f32((xr.x / float(hc))), silu_f32((xr.y / float(hc))), silu_f32((xr.z / float(hc))), silu_f32((xr.w / float(hc)))));
        acc += (q8_quad_dot(wqb, qb, xv) * d);
        k += gl_SubgroupSize;
    }
    acc = simd_sum(acc);
    if ((lane == 0u)) {
        stash[gl_SubgroupID] = acc;
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    uint ipt = (8u / hc);
    if (((gl_SubgroupID == 0u) && (lane < ipt))) {
        uint i = ((gl_WorkGroupID.x * ipt) + lane);
        uint dim = (ddim / hc);
        float s = 0.0f;
        uint c = 0u;
        while ((c < hc)) {
            s += (xn[((c * dim) + i)] * sigmoid_f32(stash[((c * ipt) + lane)]));
            ++c;
        }
        y[i] = (s * (1.0f / float(hc)));
    }
}

kernel void hcnorm_base(device const float* res [[buffer(0)]],
        device float* xn [[buffer(1)]],
        device const float* wn [[buffer(2)]],
        constant GkHcNormArgs& pa [[buffer(3)]],
        uint3 gl_LocalInvocationID [[thread_position_in_threadgroup]],
        uint gl_NumSubgroups [[simdgroups_per_threadgroup]],
        uint gl_SubgroupID [[simdgroup_index_in_threadgroup]],
        uint gl_SubgroupInvocationID [[thread_index_in_simdgroup]],
        uint3 gl_WorkGroupID [[threadgroup_position_in_grid]],
        threadgroup char * shmem [[threadgroup(0)]]) {
    threadgroup float * part = (threadgroup float *)(shmem);
    uint g = gl_WorkGroupID.x;
    uint src = (((pa.row0 * pa.hc) * pa.dim) + (g * pa.dim));
    uint base = (g * pa.dim);
    uint wb = (pa.woff + ((g % max(1u, pa.hc)) * pa.dim));
    uint tid = gl_LocalInvocationID.x;
    float ss = 0.0f;
    uint i = tid;
    while ((i < pa.dim)) {
        float v = res[(src + i)];
        ss += (v * v);
        i += 1024u;
    }
    float tot = 0.0f;
    float ss_s3 = simd_sum(ss);
    if ((gl_SubgroupInvocationID == 0u)) {
        part[gl_SubgroupID] = ss_s3;
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    uint sgi = 0u;
    while ((sgi < gl_NumSubgroups)) {
        tot += part[sgi];
        ++sgi;
    }
    float inv = rms_inv(((tot / float(pa.dim)) + pa.eps), true);
    i = tid;
    while ((i < pa.dim)) {
        xn[(base + i)] = ((res[(src + i)] * inv) * wn[(wb + i)]);
        i += 1024u;
    }
}

kernel void router_base(device const float4* w [[buffer(0)]],
        device const float4* x [[buffer(1)]],
        device float* logits [[buffer(2)]],
        constant GkRouterArgs& pa [[buffer(5)]],
        uint3 gl_LocalInvocationID [[thread_position_in_threadgroup]],
        uint gl_NumSubgroups [[simdgroups_per_threadgroup]],
        uint gl_SubgroupID [[simdgroup_index_in_threadgroup]],
        uint gl_SubgroupInvocationID [[thread_index_in_simdgroup]],
        uint3 gl_WorkGroupID [[threadgroup_position_in_grid]],
        threadgroup char * shmem [[threadgroup(0)]]) {
    threadgroup float * part = (threadgroup float *)(shmem);
    uint e = gl_WorkGroupID.x;
    uint lane = gl_LocalInvocationID.x;
    uint cb = gl_WorkGroupID.y;
    uint xs = ((pa.xs == 0u) ? pa.dim : pa.xs);
    uint xc0 = ((pa.col0 + cb) * xs);
    uint nw = (pa.dim / 4u);
    uint wb = ((pa.woff + (e * pa.dim)) / 4u);
    float acc = 0.0f;
    uint i = lane;
    while ((i < nw)) {
        acc += dot4_seq(w[(wb + i)], x[((xc0 / 4u) + i)]);
        i += 256u;
    }
    float v = simd_sum(acc);
    if ((gl_SubgroupInvocationID == 0u)) {
        part[gl_SubgroupID * 8u] = v;
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if ((lane == 0u)) {
        float a = 0.0f;
        uint sgi = 0u;
        while ((sgi < gl_NumSubgroups)) {
            a += part[sgi * 8u];
            ++sgi;
        }
        logits[((pa.obase + ((pa.col0 + cb) * pa.ne)) + e)] = a;
    }
}

kernel void prep_base(device const float* xf [[buffer(0)]],
        device half* xt [[buffer(1)]],
        device float* xs [[buffer(2)]],
        constant uint& kdim [[buffer(3)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]]) {
    uint gid = gl_GlobalInvocationID.x;
    uint nb = (kdim / 32u);
    if ((gid < (nb * 8u))) {
        uint blk = (gid >> 3u);
        uint t = (gid & 7u);
        uint row = ((t * kdim) + (blk * 32u));
        float sum = 0.0f;
        #pragma clang loop unroll(full)
        for (int slot = 0; slot < 32; ++slot) {
            uint kq = (uint((slot & 15)) >> 2u);
            uint i = uint((slot & 3));
            uint phys = ((slot < 16) ? ((kq * 8u) + i) : (((kq * 8u) + 4u) + i));
            half v = half(xf[(row + phys)]);
            xt[(row + uint(slot))] = v;
            sum += float(v);
        }
        xs[((blk * 8u) + t)] = sum;
    }
}

kernel void r8q8_base(device const half* wsh [[buffer(0)]],
        device const uchar* wqb [[buffer(1)]],
        device const half* xt [[buffer(2)]],
        device float* yk [[buffer(3)]],
        device const float4* xs4 [[buffer(4)]],
        constant uint& kdim [[buffer(5)]],
        constant uint& ndim [[buffer(6)]],
        uint gl_SubgroupID [[simdgroup_index_in_threadgroup]],
        uint gl_SubgroupInvocationID [[thread_index_in_simdgroup]],
        uint3 gl_WorkGroupID [[threadgroup_position_in_grid]],
        threadgroup char * shmem [[threadgroup(0)]]) {
    threadgroup float * red = (threadgroup float *)(shmem);
    uint nb = (kdim / 32u);
    uint ks = gl_SubgroupID;
    uint lane = gl_SubgroupInvocationID;
    uint m0 = rows8_m0(lane);
    uint kq = rows8_kq(lane);
    bool pad = rows8_pad(lane);
    uint tile = gl_WorkGroupID.x;
    uint ra = ((tile * 16u) + m0);
    uint xstride = (kdim);
    uint rb = (ra + 8u);
    constexpr auto __tgd_acc = matmul2d_descriptor(16, 8, 32, false, true, true, matmul2d_descriptor::mode::multiply);
    matmul2d<__tgd_acc, execution_simdgroup> __tgop_acc;
    auto __tgl_acc = __tgop_acc.template get_left_input_cooperative_tensor<half, half, float>();
    auto __tgx_acc = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half *)((xt + (0))), dextents<int32_t, 2>(int(xstride), 8));
    auto __tgs_acc = __tgx_acc.template slice<32, 8>(0, 0);
    auto __tgf_acc = __tgop_acc.template get_destination_cooperative_tensor<decltype(__tgl_acc), decltype(__tgs_acc), float>();
    float y8[8] = {};
    uint tq = (pad ? 0u : (kq * 4u));
    uint b = ks;
    while ((b < nb)) {
        uint ba = ((ra * nb) + b);
        uint bb = ((rb * nb) + b);
        float2 fa = (float2(float(wsh[(ba * 17u)]), 0.0f));
        float2 fb = (float2(float(wsh[(bb * 17u)]), 0.0f));
        uint2 ra2 = (rows8_q8_run(wqb, 0u, ba, kq) ^ uint2(2155905152u, 2155905152u));
        uint2 rb2 = (rows8_q8_run(wqb, 0u, bb, kq) ^ uint2(2155905152u, 2155905152u));
        uint4 qa = uint4(ra2.x, ra2.y, 0u, 0u);
        uint4 qb = uint4(rb2.x, rb2.y, 0u, 0u);
        #pragma clang loop unroll(full)
        for (int h = 0; h < 2; ++h) {
            uint sh = (uint(h) * 16u);
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qa.x >> sh) & 255u)) | (((qa.x >> sh) & 65280u) << 8u))));
                __tgl_acc[(uint(h) * 2u)] = __h2.x;
                __tgl_acc[((uint(h) * 2u) + 1u)] = __h2.y;
            }
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qb.x >> sh) & 255u)) | (((qb.x >> sh) & 65280u) << 8u))));
                __tgl_acc[((uint(h) * 2u) + 4u)] = __h2.x;
                __tgl_acc[(((uint(h) * 2u) + 4u) + 1u)] = __h2.y;
            }
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qa.y >> sh) & 255u)) | (((qa.y >> sh) & 65280u) << 8u))));
                __tgl_acc[((uint(h) * 2u) + 8u)] = __h2.x;
                __tgl_acc[(((uint(h) * 2u) + 8u) + 1u)] = __h2.y;
            }
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qb.y >> sh) & 255u)) | (((qb.y >> sh) & 65280u) << 8u))));
                __tgl_acc[((uint(h) * 2u) + 12u)] = __h2.x;
                __tgl_acc[(((uint(h) * 2u) + 12u) + 1u)] = __h2.y;
            }
        }
        {
            auto __B = __tgx_acc.template slice<32, 8>(int(b * 32u), 0);
            __tgop_acc.run(__tgl_acc, __B, __tgf_acc);
        }
        float4 s0v = xs4[((b * 2u) + (tq >> 2u))];
        #pragma clang loop unroll(full)
        for (int i = 0; i < 4; ++i) {
            y8[i] = fma(fa.x, (__tgf_acc[uint(i)] - (s0v[i] * 1152.0f)), y8[i]);
            y8[(i + 4)] = fma(fb.x, (__tgf_acc[(uint(i) + 4u)] - (s0v[i] * 1152.0f)), y8[(i + 4)]);
        }
        b += 8u;
    }
    #pragma clang loop unroll(full)
    for (int i = 0; i < 8; ++i) {
        red[(((ks * 256u) + (uint(i) * 32u)) + lane)] = y8[i];
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if ((!pad)) {
        threadgroup float * pr = red + (((ks * 32u) + lane));
        float s = pr[0];
        #pragma clang loop unroll(full)
        for (int k = 0; k < 7; ++k) {
            s += pr[((k * 256) + 256)];
        }
        uint t = (tq + (ks & 3u));
        uint row = ((ks < 4u) ? ra : rb);
        yk[((t * ndim) + row)] = s;
    }
}

kernel void hclo_base(device float* g [[buffer(0)]],
        constant uint& total [[buffer(1)]],
        constant uint& hc [[buffer(2)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]]) {
    uint gid = gl_GlobalInvocationID.x;
    if ((gid < total)) {
        float vv = g[gid];
        g[gid] = (silu_f32((vv / float(hc))));
    }
}

kernel void hcmix_base(device const float* xn [[buffer(0)]],
        device const float* gate [[buffer(1)]],
        device float* xb [[buffer(2)]],
        constant GkHcMixArgs& pa [[buffer(3)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]]) {
    uint e = gl_GlobalInvocationID.x;
    if ((e < (pa.rows * pa.dim))) {
        uint r = (e / pa.dim);
        uint i = (e - (r * pa.dim));
        uint rb = ((r * pa.hc) * pa.dim);
        float acc = 0.0f;
        uint c = 0u;
        while ((c < pa.hc)) {
            uint x = ((rb + (c * pa.dim)) + i);
            acc += (xn[x] * sigmoid_f32(gate[x]));
            ++c;
        }
        xb[e] = (acc * (1.0f / float(pa.hc)));
    }
}

// the reduce-scatter of N partials over the lanes' 32 (N a power of two <= 32): lane L ends holding the lanes' sum of value
// L % N - the lanes past N fold first, then each halving trades half its values with the lane `o` across
template <int N>
static inline float rscat(thread float* v, uint lane) {
    #pragma clang loop unroll(full)
    for (int o = 16; o >= N; o >>= 1) {
        #pragma clang loop unroll(full)
        for (int i = 0; i < N; ++i) {
            v[i] += simd_shuffle_xor(v[i], ushort(o));
        }
    }
    #pragma clang loop unroll(full)
    for (int o = N / 2; o >= 1; o >>= 1) {
        bool hi = (lane & uint(o)) != 0u;
        #pragma clang loop unroll(full)
        for (int i = 0; i < o; ++i) {
            float send = hi ? v[i] : v[i + o];
            float keep = hi ? v[i + o] : v[i];
            v[i] = keep + simd_shuffle_xor(send, ushort(o));
        }
    }
    return v[0];
}

// the reduce-scatter of 8 partials over an 8-lane group: lane L ends holding the group's sum of value L % 8
static inline float rscat8g(thread float* v, uint lane) {
    #pragma clang loop unroll(full)
    for (int o = 4; o >= 1; o >>= 1) {
        bool hi = (lane & uint(o)) != 0u;
        #pragma clang loop unroll(full)
        for (int i = 0; i < o; ++i) {
            float send = hi ? v[i] : v[i + o];
            float keep = hi ? v[i + o] : v[i];
            v[i] = keep + simd_shuffle_xor(send, ushort(o));
        }
    }
    return v[0];
}

#ifndef RS
#define RS 4
#endif
#ifndef NSG
#define NSG 4
#endif

// the eight-row down split by stream: threadgroup (tile, c) dots the tile's RS x NSG q8 rows (or, past the last tile, the hc f32
// inject rows) against stream c of the eight gamma-scaled wide rows; a simdgroup owns RS rows over the stream's whole K, a lane
// an 8-quant chunk of every eighth block; tile 0's simdgroup 0 also sums each row's stream squares. The partials land
// unnormed - the stream's inverse norm scales them where they are summed
kernel void hcn8(device const half* wsh [[buffer(0)]],
        device const char* wqb [[buffer(1)]],
        device const float4* res [[buffer(2)]],
        device float* part [[buffer(3)]],
        device const float4* wn [[buffer(4)]],
        device const float4* wi [[buffer(5)]],
        device float* ipart [[buffer(6)]],
        device float* ssq [[buffer(7)]],
        constant HcMix8Args& ka [[buffer(8)]],
        uint sg [[simdgroup_index_in_threadgroup]],
        uint lane [[thread_index_in_simdgroup]],
        uint3 tgp [[threadgroup_position_in_grid]]) {
    uint c = tgp.y;
    uint tile = tgp.x;
    uint ntile = ka.lr / uint(RS * NSG);
    bool injt = tile >= ntile;
    uint row0 = injt ? sg * uint(RS) : (tile * uint(NSG) + sg) * uint(RS);
    if (injt && row0 >= ka.hc) {
        return;
    }
    uint nbs = ka.dim / 32u;
    uint nbk = nbs * ka.hc;
    uint rs4 = ka.dim * ka.hc / 4u;
    uint ix = lane / 4u;
    uint il = lane % 4u;
    bool dosq = tile == 0u && sg == 0u;
    float acc[RS * 8] = {};
    float sq[8] = {};
    for (uint ib = ix; ib < nbs; ib += 8u) {
        uint kb = c * nbs + ib;
        uint xi = kb * 8u + il * 2u;
        float4 g0 = wn[xi];
        float4 g1 = wn[xi + 1u];
        #pragma clang loop unroll(full)
        for (int t = 0; t < 8; ++t) {
            float4 r0 = res[uint(t) * rs4 + xi];
            float4 r1 = res[uint(t) * rs4 + xi + 1u];
            if (dosq) {
                sq[t] += (((((r0.x * r0.x) + (r0.y * r0.y)) + (r0.z * r0.z)) + (r0.w * r0.w)) + ((((r1.x * r1.x) + (r1.y * r1.y)) + (r1.z * r1.z)) + (r1.w * r1.w)));
            }
            float4 x0 = r0 * g0;
            float4 x1 = r1 * g1;
            #pragma clang loop unroll(full)
            for (int r = 0; r < RS; ++r) {
                if (injt) {
                    uint wb = (row0 + uint(r)) * rs4 + xi;
                    acc[r * 8 + t] += dot4_seq(wi[wb], x0) + dot4_seq(wi[wb + 1u], x1);
                } else {
                    uint bi = (row0 + uint(r)) * nbk + kb;
                    acc[r * 8 + t] += q8_oct_dot(wqb, bi * 34u + 2u + il * 8u, x0, x1) * float(wsh[bi * 17u]);
                }
            }
        }
    }
    float v = rscat<RS * 8>(acc, lane);
    if (lane < uint(RS * 8)) {
        uint r = lane / 8u;
        uint t = lane % 8u;
        if (injt) {
            if (row0 + r < ka.hc) {
                ipart[(c * 8u + t) * ka.hc + row0 + r] = v;
            }
        } else {
            part[(c * 8u + t) * ka.lr + row0 + r] = v;
        }
    }
    if (dosq) {
        float s = rscat<8>(sq, lane);
        if (lane < 8u) {
            ssq[c * 8u + lane] = s;
        }
    }
}

#ifndef COLS
#define COLS 1
#endif
#define LRMAX 512

// the eight rows' inverse norms off the streams' squares, then the low-rank rows as the inverse-scaled stream partials' sum
// through silu(lo / hc) into `xs`; threadgroup 0 lands the scatter logits the same way
static inline void lo_stage(threadgroup float* xs, threadgroup float* invs, device const float* part, device const float* ssq,
        device const float* ipart, device float* inj, constant HcMix8Args& ka, uint tid, uint ntid, bool first) {
    if (tid < 8u * ka.hc) {
        uint t = tid / ka.hc;
        uint c = tid % ka.hc;
        invs[tid] = rms_inv(ssq[c * 8u + t] / float(ka.dim) + ka.eps, true);
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    for (uint e = tid; e < 8u * ka.lr; e += ntid) {
        uint t = e / ka.lr;
        uint k = e % ka.lr;
        float lo = 0.0f;
        for (uint c = 0u; c < ka.hc; ++c) {
            lo += part[(c * 8u + t) * ka.lr + k] * invs[t * ka.hc + c];
        }
        xs[e] = silu_f32(lo / float(ka.hc));
    }
    if (first && tid < 8u * ka.hc) {
        uint t = tid / ka.hc;
        uint j = tid % ka.hc;
        float s = 0.0f;
        for (uint c = 0u; c < ka.hc; ++c) {
            s += ipart[(c * 8u + t) * ka.hc + j] * invs[t * ka.hc + c];
        }
        inj[t * ka.hc + j] = s;
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
}

// the up of column i over the eight rows: lane group c (8 lanes) walks stream c's row i, a lane every eighth quad, the eight
// rows' x from `xs`; the group's reduce-scatter leaves lane j row j's gate, and the gated mean over the groups lands y[j][i]
static inline void up_col(device const half* wsh, device const char* wqb, threadgroup const float4* xs4, threadgroup const float* invs,
        device const float* res, device const float* wn, device float* y, constant HcMix8Args& ka, uint i, uint lane) {
    uint grp = lane / 8u;
    uint lj = lane % 8u;
    uint row = grp * ka.dim + i;
    uint nbr = ka.lr / 32u;
    uint nq = ka.lr / 4u;
    float acc[8] = {};
    for (uint q = lj; q < nq; q += 8u) {
        uint bi = row * nbr + q / 8u;
        float d = float(wsh[bi * 17u]);
        uint qb = bi * 34u + 2u + (q % 8u) * 4u;
        #pragma clang loop unroll(full)
        for (int t = 0; t < 8; ++t) {
            acc[t] += q8_quad_dot(wqb, qb, xs4[uint(t) * nq + q]) * d;
        }
    }
    float gate = rscat8g(acc, lane);
    uint hcd = ka.dim * ka.hc;
    float xnv = (res[lj * hcd + grp * ka.dim + i] * invs[lj * ka.hc + grp]) * wn[grp * ka.dim + i];
    float v = xnv * sigmoid_f32(gate);
    float v0 = simd_shuffle(v, ushort(lj));
    float v1 = simd_shuffle(v, ushort(8u + lj));
    float v2 = simd_shuffle(v, ushort(16u + lj));
    float v3 = simd_shuffle(v, ushort(24u + lj));
    if (grp == 0u) {
        y[lj * ka.dim + i] = ((((0.0f + v0) + v1) + v2) + v3) * (1.0f / float(ka.hc));
    }
}

// the eight-row up with the partials' finish in its prologue: every threadgroup sums the stream partials into the low-rank
// rows itself; a simdgroup takes COLS columns, eight simdgroups a threadgroup (hc = 4: a lane group a stream)
kernel void sglo8(device const half* wsh [[buffer(0)]],
        device const char* wqb [[buffer(1)]],
        device const float* part [[buffer(2)]],
        device const float* ssq [[buffer(3)]],
        device const float* ipart [[buffer(4)]],
        device const float* res [[buffer(5)]],
        device const float* wn [[buffer(6)]],
        device float* y [[buffer(7)]],
        device float* inj [[buffer(8)]],
        constant HcMix8Args& ka [[buffer(9)]],
        uint tid [[thread_index_in_threadgroup]],
        uint sg [[simdgroup_index_in_threadgroup]],
        uint lane [[thread_index_in_simdgroup]],
        uint3 tgp [[threadgroup_position_in_grid]]) {
    threadgroup float xs[8 * LRMAX];
    threadgroup float invs[64];
    lo_stage(xs, invs, part, ssq, ipart, inj, ka, tid, 256u, tgp.x == 0u);
    #pragma clang loop unroll(full)
    for (int col = 0; col < COLS; ++col) {
        uint i = (tgp.x * uint(COLS) + uint(col)) * 8u + sg;
        up_col(wsh, wqb, (threadgroup const float4*)xs, invs, res, wn, y, ka, i, lane);
    }
}

// the partials' finish as its own dispatch: the low-rank rows through silu(lo / hc) into `lox`, the inverse norms into
// `invx`, the scatter logits
kernel void lofin8(device const float* part [[buffer(2)]],
        device const float* ssq [[buffer(3)]],
        device const float* ipart [[buffer(4)]],
        device float* inj [[buffer(8)]],
        constant HcMix8Args& ka [[buffer(9)]],
        device float* lox [[buffer(10)]],
        device float* invx [[buffer(11)]],
        uint tid [[thread_index_in_threadgroup]]) {
    threadgroup float xs[8 * LRMAX];
    threadgroup float invs[64];
    lo_stage(xs, invs, part, ssq, ipart, inj, ka, tid, 1024u, true);
    for (uint e = tid; e < 8u * ka.lr; e += 1024u) {
        lox[e] = xs[e];
    }
    if (tid < 8u * ka.hc) {
        invx[tid] = invs[tid];
    }
}

// the eight-row up over the finished rows: `lox` staged into threadgroup memory, then the columns as `sglo8`'s
kernel void sglo8f(device const half* wsh [[buffer(0)]],
        device const char* wqb [[buffer(1)]],
        device const float* res [[buffer(5)]],
        device const float* wn [[buffer(6)]],
        device float* y [[buffer(7)]],
        constant HcMix8Args& ka [[buffer(9)]],
        device const float* lox [[buffer(10)]],
        device const float* invx [[buffer(11)]],
        uint tid [[thread_index_in_threadgroup]],
        uint sg [[simdgroup_index_in_threadgroup]],
        uint lane [[thread_index_in_simdgroup]],
        uint3 tgp [[threadgroup_position_in_grid]]) {
    threadgroup float xs[8 * LRMAX];
    threadgroup float invs[64];
    for (uint e = tid; e < 8u * ka.lr; e += 256u) {
        xs[e] = lox[e];
    }
    if (tid < 8u * ka.hc) {
        invs[tid] = invx[tid];
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    #pragma clang loop unroll(full)
    for (int col = 0; col < COLS; ++col) {
        uint i = (tgp.x * uint(COLS) + uint(col)) * 8u + sg;
        up_col(wsh, wqb, (threadgroup const float4*)xs, invs, res, wn, y, ka, i, lane);
    }
}

// the eight rows' prep: threadgroup (c, t) stores stream c of row t gamma-scaled as the rows8 x table (halves in the k-slot
// order, the table's row stride the wide row) with the 32-blocks' sums, the stream's squares, and the inject rows' stream
// partials over the f32 gamma-scaled stream; a simdgroup a block, a lane a k-slot
kernel void hcprep8(device const float* res [[buffer(0)]],
        device const float* wn [[buffer(1)]],
        device const float* wi [[buffer(2)]],
        device half* xt [[buffer(3)]],
        device float* xs [[buffer(4)]],
        device float* ssq [[buffer(5)]],
        device float* ipart [[buffer(6)]],
        constant HcMix8Args& ka [[buffer(7)]],
        uint tid [[thread_index_in_threadgroup]],
        uint sg [[simdgroup_index_in_threadgroup]],
        uint lane [[thread_index_in_simdgroup]],
        uint nsg [[simdgroups_per_threadgroup]],
        uint3 tgp [[threadgroup_position_in_grid]]) {
    threadgroup float red[32 * 5];
    uint c = tgp.x;
    uint t = tgp.y;
    uint hcd = ka.dim * ka.hc;
    uint nbs = ka.dim / 32u;
    uint kq = (lane & 15u) >> 2u;
    uint ie = lane & 3u;
    uint phys = lane < 16u ? kq * 8u + ie : kq * 8u + 4u + ie;
    float sq = 0.0f;
    float ip[4] = {};
    for (uint ib = sg; ib < nbs; ib += nsg) {
        uint k = c * ka.dim + ib * 32u + phys;
        float r = res[t * hcd + k];
        float xg = r * wn[k];
        sq += r * r;
        half h = half(xg);
        xt[t * hcd + c * ka.dim + ib * 32u + lane] = h;
        float bs = simd_sum(float(h));
        if (lane == 0u) {
            xs[(c * nbs + ib) * 8u + t] = bs;
        }
        #pragma clang loop unroll(full)
        for (int j = 0; j < 4; ++j) {
            ip[j] += wi[uint(j) * hcd + k] * xg;
        }
    }
    sq = simd_sum(sq);
    #pragma clang loop unroll(full)
    for (int j = 0; j < 4; ++j) {
        ip[j] = simd_sum(ip[j]);
    }
    if (lane == 0u) {
        red[sg] = sq;
        #pragma clang loop unroll(full)
        for (int j = 0; j < 4; ++j) {
            red[32u * uint(j + 1) + sg] = ip[j];
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if (tid < 5u) {
        float s = 0.0f;
        for (uint k = 0u; k < nsg; ++k) {
            s += red[tid * 32u + k];
        }
        if (tid == 0u) {
            ssq[c * 8u + t] = s;
        } else {
            ipart[(c * 8u + t) * ka.hc + tid - 1u] = s;
        }
    }
}

// the prep with the norm first: threadgroup (c, t), 1024 threads, sums stream c of row t's squares in `metal_hc_norm`'s order,
// so the normed stream is its bits, stores the normed stream as the x table in `prep_base`'s order and sums, its inverse norm,
// and the inject rows' stream partials over the f32 normed stream
kernel void hcprep8n(device const float* res [[buffer(0)]],
        device const float* wn [[buffer(1)]],
        device const float* wi [[buffer(2)]],
        device half* xt [[buffer(3)]],
        device float* xs [[buffer(4)]],
        device float* invx [[buffer(5)]],
        device float* ipart [[buffer(6)]],
        constant HcMix8Args& ka [[buffer(7)]],
        uint tid [[thread_index_in_threadgroup]],
        uint sg [[simdgroup_index_in_threadgroup]],
        uint lane [[thread_index_in_simdgroup]],
        uint nsg [[simdgroups_per_threadgroup]],
        uint3 tgp [[threadgroup_position_in_grid]]) {
    threadgroup float part[32];
    threadgroup float red[4 * 32];
    uint c = tgp.x;
    uint t = tgp.y;
    uint hcd = ka.dim * ka.hc;
    uint src = t * hcd + c * ka.dim;
    float ss = 0.0f;
    for (uint i = tid; i < ka.dim; i += 1024u) {
        float v = res[src + i];
        ss += v * v;
    }
    float s3 = simd_sum(ss);
    if (lane == 0u) {
        part[sg] = s3;
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    float tot = 0.0f;
    for (uint k = 0u; k < nsg; ++k) {
        tot += part[k];
    }
    float inv = rms_inv(tot / float(ka.dim) + ka.eps, true);
    if (tid == 0u) {
        invx[t * ka.hc + c] = inv;
    }
    uint nbs = ka.dim / 32u;
    if (tid < nbs) {
        uint row = t * hcd + c * ka.dim + tid * 32u;
        float sum = 0.0f;
        #pragma clang loop unroll(full)
        for (int slot = 0; slot < 32; ++slot) {
            uint kq = (uint((slot & 15)) >> 2u);
            uint i = uint((slot & 3));
            uint phys = ((slot < 16) ? ((kq * 8u) + i) : (((kq * 8u) + 4u) + i));
            uint k = c * ka.dim + tid * 32u + phys;
            half v = half((res[t * hcd + k] * inv) * wn[k]);
            xt[row + uint(slot)] = v;
            sum += float(v);
        }
        xs[(c * nbs + tid) * 8u + t] = sum;
    }
    float ip[4] = {};
    for (uint i = tid; i < ka.dim; i += 1024u) {
        uint k = c * ka.dim + i;
        float xv = (res[t * hcd + k] * inv) * wn[k];
        #pragma clang loop unroll(full)
        for (int j = 0; j < 4; ++j) {
            ip[j] += wi[uint(j) * hcd + k] * xv;
        }
    }
    #pragma clang loop unroll(full)
    for (int j = 0; j < 4; ++j) {
        float v = simd_sum(ip[j]);
        if (lane == 0u) {
            red[uint(j) * 32u + sg] = v;
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if (tid < 4u) {
        float s = 0.0f;
        for (uint k = 0u; k < nsg; ++k) {
            s += red[tid * 32u + k];
        }
        ipart[(c * 8u + t) * ka.hc + tid] = s;
    }
}

// the finish over normed partials: the low-rank rows the partials' sum through silu(lo / hc) as the up's x table, the logits
kernel void lofin8n(device const float* part [[buffer(2)]],
        device const float* ipart [[buffer(4)]],
        device float* inj [[buffer(8)]],
        constant HcMix8Args& ka [[buffer(9)]],
        device half* xt2 [[buffer(10)]],
        device float* xs2 [[buffer(11)]],
        uint tid [[thread_index_in_threadgroup]],
        uint sg [[simdgroup_index_in_threadgroup]],
        uint lane [[thread_index_in_simdgroup]],
        uint3 tgp [[threadgroup_position_in_grid]]) {
    if (tgp.x == 0u && tid < 8u * ka.hc) {
        uint t = tid / ka.hc;
        uint j = tid % ka.hc;
        float s = 0.0f;
        for (uint c = 0u; c < ka.hc; ++c) {
            s += ipart[(c * 8u + t) * ka.hc + j];
        }
        inj[t * ka.hc + j] = s;
    }
    uint blk = tgp.x;
    uint t = sg;
    uint kq = (lane & 15u) >> 2u;
    uint ie = lane & 3u;
    uint k = blk * 32u + (lane < 16u ? kq * 8u + ie : kq * 8u + 4u + ie);
    float lo = 0.0f;
    for (uint c = 0u; c < ka.hc; ++c) {
        lo += part[(c * 8u + t) * ka.lr + k];
    }
    half v = half(silu_f32(lo / float(ka.hc)));
    xt2[t * ka.lr + blk * 32u + lane] = v;
    float bs = simd_sum(float(v));
    if (lane == 0u) {
        xs2[blk * 8u + t] = bs;
    }
}

// the rows8 q8 product over one stream's K: threadgroup (tile, c) the plain stamp's body over stream c's blocks, its rows'
// unnormed partials at [c][t][row]
kernel void r8q8s(device const half* wsh [[buffer(0)]],
        device const uchar* wqb [[buffer(1)]],
        device const half* xt [[buffer(2)]],
        device float* yk [[buffer(3)]],
        device const float4* xs4 [[buffer(4)]],
        constant uint& kdim [[buffer(5)]],
        constant uint& ndim [[buffer(6)]],
        constant uint& kseg [[buffer(7)]],
        uint gl_SubgroupID [[simdgroup_index_in_threadgroup]],
        uint gl_SubgroupInvocationID [[thread_index_in_simdgroup]],
        uint3 gl_WorkGroupID [[threadgroup_position_in_grid]],
        threadgroup char * shmem [[threadgroup(0)]]) {
    threadgroup float * red = (threadgroup float *)(shmem);
    uint nb = (kdim / 32u);
    uint ks = gl_SubgroupID;
    uint lane = gl_SubgroupInvocationID;
    uint m0 = rows8_m0(lane);
    uint kq = rows8_kq(lane);
    bool pad = rows8_pad(lane);
    uint tile = gl_WorkGroupID.x;
    uint seg = gl_WorkGroupID.y;
    uint ra = ((tile * 16u) + m0);
    uint rb = (ra + 8u);
    constexpr auto __tgd_acc = matmul2d_descriptor(16, 8, 32, false, true, true, matmul2d_descriptor::mode::multiply);
    matmul2d<__tgd_acc, execution_simdgroup> __tgop_acc;
    auto __tgl_acc = __tgop_acc.template get_left_input_cooperative_tensor<half, half, float>();
    auto __tgx_acc = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half *)((xt + (0))), dextents<int32_t, 2>(int(kdim), 8));
    auto __tgs_acc = __tgx_acc.template slice<32, 8>(0, 0);
    auto __tgf_acc = __tgop_acc.template get_destination_cooperative_tensor<decltype(__tgl_acc), decltype(__tgs_acc), float>();
    float y8[8] = {};
    uint tq = (pad ? 0u : (kq * 4u));
    uint bend = (seg + 1u) * kseg;
    uint b = seg * kseg + ks;
    while ((b < bend)) {
        uint ba = ((ra * nb) + b);
        uint bb = ((rb * nb) + b);
        float fa = float(wsh[(ba * 17u)]);
        float fb = float(wsh[(bb * 17u)]);
        uint2 qa = (rows8_q8_run(wqb, 0u, ba, kq) ^ uint2(2155905152u, 2155905152u));
        uint2 qb = (rows8_q8_run(wqb, 0u, bb, kq) ^ uint2(2155905152u, 2155905152u));
        #pragma clang loop unroll(full)
        for (int h = 0; h < 2; ++h) {
            uint sh = (uint(h) * 16u);
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qa.x >> sh) & 255u)) | (((qa.x >> sh) & 65280u) << 8u))));
                __tgl_acc[(uint(h) * 2u)] = __h2.x;
                __tgl_acc[((uint(h) * 2u) + 1u)] = __h2.y;
            }
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qb.x >> sh) & 255u)) | (((qb.x >> sh) & 65280u) << 8u))));
                __tgl_acc[((uint(h) * 2u) + 4u)] = __h2.x;
                __tgl_acc[(((uint(h) * 2u) + 4u) + 1u)] = __h2.y;
            }
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qa.y >> sh) & 255u)) | (((qa.y >> sh) & 65280u) << 8u))));
                __tgl_acc[((uint(h) * 2u) + 8u)] = __h2.x;
                __tgl_acc[(((uint(h) * 2u) + 8u) + 1u)] = __h2.y;
            }
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qb.y >> sh) & 255u)) | (((qb.y >> sh) & 65280u) << 8u))));
                __tgl_acc[((uint(h) * 2u) + 12u)] = __h2.x;
                __tgl_acc[(((uint(h) * 2u) + 12u) + 1u)] = __h2.y;
            }
        }
        {
            auto __B = __tgx_acc.template slice<32, 8>(int(b * 32u), 0);
            __tgop_acc.run(__tgl_acc, __B, __tgf_acc);
        }
        float4 s0v = xs4[((b * 2u) + (tq >> 2u))];
        #pragma clang loop unroll(full)
        for (int i = 0; i < 4; ++i) {
            y8[i] = fma(fa, (__tgf_acc[uint(i)] - (s0v[i] * 1152.0f)), y8[i]);
            y8[(i + 4)] = fma(fb, (__tgf_acc[(uint(i) + 4u)] - (s0v[i] * 1152.0f)), y8[(i + 4)]);
        }
        b += 8u;
    }
    #pragma clang loop unroll(full)
    for (int i = 0; i < 8; ++i) {
        red[(((ks * 256u) + (uint(i) * 32u)) + lane)] = y8[i];
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if ((!pad)) {
        threadgroup float * pr = red + (((ks * 32u) + lane));
        float s = pr[0];
        #pragma clang loop unroll(full)
        for (int k = 0; k < 7; ++k) {
            s += pr[((k * 256) + 256)];
        }
        uint t = (tq + (ks & 3u));
        uint row = ((ks < 4u) ? ra : rb);
        yk[(((seg * 8u + t) * ndim) + row)] = s;
    }
}

// the finish over the stream partials: the inverse norms into `invx`, the low-rank rows through silu(lo / hc) as the up's x
// table and its sums (one thread a (block, row)), the scatter logits
kernel void lofin8t(device const float* part [[buffer(2)]],
        device const float* ssq [[buffer(3)]],
        device const float* ipart [[buffer(4)]],
        device float* inj [[buffer(8)]],
        constant HcMix8Args& ka [[buffer(9)]],
        device half* xt2 [[buffer(10)]],
        device float* xs2 [[buffer(11)]],
        device float* invx [[buffer(12)]],
        uint tid [[thread_index_in_threadgroup]]) {
    threadgroup float invs[64];
    if (tid < 8u * ka.hc) {
        uint t = tid / ka.hc;
        uint c = tid % ka.hc;
        float v = rms_inv(ssq[c * 8u + t] / float(ka.dim) + ka.eps, true);
        invs[tid] = v;
        invx[tid] = v;
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if (tid < 8u * ka.hc) {
        uint t = tid / ka.hc;
        uint j = tid % ka.hc;
        float s = 0.0f;
        for (uint c = 0u; c < ka.hc; ++c) {
            s += ipart[(c * 8u + t) * ka.hc + j] * invs[t * ka.hc + c];
        }
        inj[t * ka.hc + j] = s;
    }
    uint nb = ka.lr / 32u;
    if (tid < nb * 8u) {
        uint blk = tid >> 3u;
        uint t = tid & 7u;
        uint row = t * ka.lr + blk * 32u;
        float sum = 0.0f;
        #pragma clang loop unroll(full)
        for (int slot = 0; slot < 32; ++slot) {
            uint kq = (uint((slot & 15)) >> 2u);
            uint i = uint((slot & 3));
            uint phys = ((slot < 16) ? ((kq * 8u) + i) : (((kq * 8u) + 4u) + i));
            uint k = blk * 32u + phys;
            float lo = 0.0f;
            for (uint c = 0u; c < ka.hc; ++c) {
                lo += part[(c * 8u + t) * ka.lr + k] * invs[t * ka.hc + c];
            }
            half v = half(silu_f32(lo / float(ka.hc)));
            xt2[row + uint(slot)] = v;
            sum += float(v);
        }
        xs2[blk * 8u + t] = sum;
    }
}

// the finish a block a threadgroup: simdgroup t row t's block, lane its k-slot; threadgroup 0 also lands the logits and the
// inverse norms
kernel void lofin8w(device const float* part [[buffer(2)]],
        device const float* ssq [[buffer(3)]],
        device const float* ipart [[buffer(4)]],
        device float* inj [[buffer(8)]],
        constant HcMix8Args& ka [[buffer(9)]],
        device half* xt2 [[buffer(10)]],
        device float* xs2 [[buffer(11)]],
        device float* invx [[buffer(12)]],
        uint tid [[thread_index_in_threadgroup]],
        uint sg [[simdgroup_index_in_threadgroup]],
        uint lane [[thread_index_in_simdgroup]],
        uint3 tgp [[threadgroup_position_in_grid]]) {
    threadgroup float invs[64];
    bool first = tgp.x == 0u;
    if (tid < 8u * ka.hc) {
        uint t = tid / ka.hc;
        uint c = tid % ka.hc;
        float v = rms_inv(ssq[c * 8u + t] / float(ka.dim) + ka.eps, true);
        invs[tid] = v;
        if (first) {
            invx[tid] = v;
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if (first && tid < 8u * ka.hc) {
        uint t = tid / ka.hc;
        uint j = tid % ka.hc;
        float s = 0.0f;
        for (uint c = 0u; c < ka.hc; ++c) {
            s += ipart[(c * 8u + t) * ka.hc + j] * invs[t * ka.hc + c];
        }
        inj[t * ka.hc + j] = s;
    }
    uint blk = tgp.x;
    uint t = sg;
    uint kq = (lane & 15u) >> 2u;
    uint ie = lane & 3u;
    uint k = blk * 32u + (lane < 16u ? kq * 8u + ie : kq * 8u + 4u + ie);
    float lo = 0.0f;
    for (uint c = 0u; c < ka.hc; ++c) {
        lo += part[(c * 8u + t) * ka.lr + k] * invs[t * ka.hc + c];
    }
    half v = half(silu_f32(lo / float(ka.hc)));
    xt2[t * ka.lr + blk * 32u + lane] = v;
    float bs = simd_sum(float(v));
    if (lane == 0u) {
        xs2[blk * 8u + t] = bs;
    }
}

#ifndef KSP
#define KSP 2
#endif

// `r8up_mix` with K split over KSP simdgroups a stream tile: simdgroup c KSP + h the blocks h, h + KSP, ..; the partials meet
// in threadgroup memory in split order
kernel void r8up_mixk(device const half* wsh [[buffer(0)]],
        device const uchar* wqb [[buffer(1)]],
        device const half* xt [[buffer(2)]],
        device float* y [[buffer(3)]],
        device const float4* xs4 [[buffer(4)]],
        device const float* res [[buffer(5)]],
        device const float* wn [[buffer(6)]],
        constant HcMix8Args& ka [[buffer(9)]],
        device const float* invx [[buffer(12)]],
        uint tid [[thread_index_in_threadgroup]],
        uint gl_SubgroupID [[simdgroup_index_in_threadgroup]],
        uint gl_SubgroupInvocationID [[thread_index_in_simdgroup]],
        uint3 gl_WorkGroupID [[threadgroup_position_in_grid]]) {
    threadgroup float gt[KSP * 4 * 16 * 8];
    uint nb = ka.lr / 32u;
    uint c = gl_SubgroupID / uint(KSP);
    uint kh = gl_SubgroupID % uint(KSP);
    uint lane = gl_SubgroupInvocationID;
    uint m0 = rows8_m0(lane);
    uint kq = rows8_kq(lane);
    bool pad = rows8_pad(lane);
    uint col0 = gl_WorkGroupID.x * 16u;
    uint ra = c * ka.dim + col0 + m0;
    uint rb = (ra + 8u);
    constexpr auto __tgd_acc = matmul2d_descriptor(16, 8, 32, false, true, true, matmul2d_descriptor::mode::multiply);
    matmul2d<__tgd_acc, execution_simdgroup> __tgop_acc;
    auto __tgl_acc = __tgop_acc.template get_left_input_cooperative_tensor<half, half, float>();
    auto __tgx_acc = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half *)((xt + (0))), dextents<int32_t, 2>(int(ka.lr), 8));
    auto __tgs_acc = __tgx_acc.template slice<32, 8>(0, 0);
    auto __tgf_acc = __tgop_acc.template get_destination_cooperative_tensor<decltype(__tgl_acc), decltype(__tgs_acc), float>();
    float y8[8] = {};
    uint tq = (pad ? 0u : (kq * 4u));
    for (uint b = kh; b < nb; b += uint(KSP)) {
        uint ba = ((ra * nb) + b);
        uint bb = ((rb * nb) + b);
        float fa = float(wsh[(ba * 17u)]);
        float fb = float(wsh[(bb * 17u)]);
        uint2 qa = (rows8_q8_run(wqb, 0u, ba, kq) ^ uint2(2155905152u, 2155905152u));
        uint2 qb = (rows8_q8_run(wqb, 0u, bb, kq) ^ uint2(2155905152u, 2155905152u));
        #pragma clang loop unroll(full)
        for (int h = 0; h < 2; ++h) {
            uint sh = (uint(h) * 16u);
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qa.x >> sh) & 255u)) | (((qa.x >> sh) & 65280u) << 8u))));
                __tgl_acc[(uint(h) * 2u)] = __h2.x;
                __tgl_acc[((uint(h) * 2u) + 1u)] = __h2.y;
            }
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qb.x >> sh) & 255u)) | (((qb.x >> sh) & 65280u) << 8u))));
                __tgl_acc[((uint(h) * 2u) + 4u)] = __h2.x;
                __tgl_acc[(((uint(h) * 2u) + 4u) + 1u)] = __h2.y;
            }
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qa.y >> sh) & 255u)) | (((qa.y >> sh) & 65280u) << 8u))));
                __tgl_acc[((uint(h) * 2u) + 8u)] = __h2.x;
                __tgl_acc[(((uint(h) * 2u) + 8u) + 1u)] = __h2.y;
            }
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qb.y >> sh) & 255u)) | (((qb.y >> sh) & 65280u) << 8u))));
                __tgl_acc[((uint(h) * 2u) + 12u)] = __h2.x;
                __tgl_acc[(((uint(h) * 2u) + 12u) + 1u)] = __h2.y;
            }
        }
        {
            auto __B = __tgx_acc.template slice<32, 8>(int(b * 32u), 0);
            __tgop_acc.run(__tgl_acc, __B, __tgf_acc);
        }
        float4 s0v = xs4[((b * 2u) + (tq >> 2u))];
        #pragma clang loop unroll(full)
        for (int i = 0; i < 4; ++i) {
            y8[i] = fma(fa, (__tgf_acc[uint(i)] - (s0v[i] * 1152.0f)), y8[i]);
            y8[(i + 4)] = fma(fb, (__tgf_acc[(uint(i) + 4u)] - (s0v[i] * 1152.0f)), y8[(i + 4)]);
        }
    }
    if (!pad) {
        #pragma clang loop unroll(full)
        for (int i = 0; i < 4; ++i) {
            gt[((kh * 4u + c) * 16u + m0) * 8u + tq + uint(i)] = y8[i];
            gt[((kh * 4u + c) * 16u + m0 + 8u) * 8u + tq + uint(i)] = y8[4 + i];
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    if (tid < 128u) {
        uint j = tid / 8u;
        uint t = tid % 8u;
        uint i = col0 + j;
        uint hcd = ka.dim * ka.hc;
        float s = 0.0f;
        for (uint cc = 0u; cc < ka.hc; ++cc) {
            float g = gt[(cc * 16u + j) * 8u + t];
            #pragma clang loop unroll(full)
            for (int h = 1; h < KSP; ++h) {
                g += gt[((uint(h) * 4u + cc) * 16u + j) * 8u + t];
            }
            float xnv = (res[t * hcd + cc * ka.dim + i] * invx[t * ka.hc + cc]) * wn[cc * ka.dim + i];
            s += xnv * sigmoid_f32(g);
        }
        y[t * ka.dim + i] = s * (1.0f / float(ka.hc));
    }
}

// the rows8 q8 up with the mix in its landing: threadgroup j owns columns 16 j .. 16 j + 15, simdgroup c their rows of stream
// c over the whole K; the four tiles meet in threadgroup memory, and a thread a (column, row) lands the gated stream mean
kernel void r8up_mix(device const half* wsh [[buffer(0)]],
        device const uchar* wqb [[buffer(1)]],
        device const half* xt [[buffer(2)]],
        device float* y [[buffer(3)]],
        device const float4* xs4 [[buffer(4)]],
        device const float* res [[buffer(5)]],
        device const float* wn [[buffer(6)]],
        constant HcMix8Args& ka [[buffer(9)]],
        device const float* invx [[buffer(12)]],
        uint tid [[thread_index_in_threadgroup]],
        uint gl_SubgroupID [[simdgroup_index_in_threadgroup]],
        uint gl_SubgroupInvocationID [[thread_index_in_simdgroup]],
        uint3 gl_WorkGroupID [[threadgroup_position_in_grid]]) {
    threadgroup float gt[4 * 16 * 8];
    uint nb = ka.lr / 32u;
    uint c = gl_SubgroupID;
    uint lane = gl_SubgroupInvocationID;
    uint m0 = rows8_m0(lane);
    uint kq = rows8_kq(lane);
    bool pad = rows8_pad(lane);
    uint col0 = gl_WorkGroupID.x * 16u;
    uint ra = c * ka.dim + col0 + m0;
    uint rb = (ra + 8u);
    constexpr auto __tgd_acc = matmul2d_descriptor(16, 8, 32, false, true, true, matmul2d_descriptor::mode::multiply);
    matmul2d<__tgd_acc, execution_simdgroup> __tgop_acc;
    auto __tgl_acc = __tgop_acc.template get_left_input_cooperative_tensor<half, half, float>();
    auto __tgx_acc = tensor<device half, dextents<int32_t, 2>, tensor_inline>((device half *)((xt + (0))), dextents<int32_t, 2>(int(ka.lr), 8));
    auto __tgs_acc = __tgx_acc.template slice<32, 8>(0, 0);
    auto __tgf_acc = __tgop_acc.template get_destination_cooperative_tensor<decltype(__tgl_acc), decltype(__tgs_acc), float>();
    float y8[8] = {};
    uint tq = (pad ? 0u : (kq * 4u));
    for (uint b = 0u; b < nb; ++b) {
        uint ba = ((ra * nb) + b);
        uint bb = ((rb * nb) + b);
        float fa = float(wsh[(ba * 17u)]);
        float fb = float(wsh[(bb * 17u)]);
        uint2 qa = (rows8_q8_run(wqb, 0u, ba, kq) ^ uint2(2155905152u, 2155905152u));
        uint2 qb = (rows8_q8_run(wqb, 0u, bb, kq) ^ uint2(2155905152u, 2155905152u));
        #pragma clang loop unroll(full)
        for (int h = 0; h < 2; ++h) {
            uint sh = (uint(h) * 16u);
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qa.x >> sh) & 255u)) | (((qa.x >> sh) & 65280u) << 8u))));
                __tgl_acc[(uint(h) * 2u)] = __h2.x;
                __tgl_acc[((uint(h) * 2u) + 1u)] = __h2.y;
            }
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qb.x >> sh) & 255u)) | (((qb.x >> sh) & 65280u) << 8u))));
                __tgl_acc[((uint(h) * 2u) + 4u)] = __h2.x;
                __tgl_acc[(((uint(h) * 2u) + 4u) + 1u)] = __h2.y;
            }
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qa.y >> sh) & 255u)) | (((qa.y >> sh) & 65280u) << 8u))));
                __tgl_acc[((uint(h) * 2u) + 8u)] = __h2.x;
                __tgl_acc[(((uint(h) * 2u) + 8u) + 1u)] = __h2.y;
            }
            {
                half2 __h2 = as_type<half2>(uint(((1677747200u | ((qb.y >> sh) & 255u)) | (((qb.y >> sh) & 65280u) << 8u))));
                __tgl_acc[((uint(h) * 2u) + 12u)] = __h2.x;
                __tgl_acc[(((uint(h) * 2u) + 12u) + 1u)] = __h2.y;
            }
        }
        {
            auto __B = __tgx_acc.template slice<32, 8>(int(b * 32u), 0);
            __tgop_acc.run(__tgl_acc, __B, __tgf_acc);
        }
        float4 s0v = xs4[((b * 2u) + (tq >> 2u))];
        #pragma clang loop unroll(full)
        for (int i = 0; i < 4; ++i) {
            y8[i] = fma(fa, (__tgf_acc[uint(i)] - (s0v[i] * 1152.0f)), y8[i]);
            y8[(i + 4)] = fma(fb, (__tgf_acc[(uint(i) + 4u)] - (s0v[i] * 1152.0f)), y8[(i + 4)]);
        }
    }
    if (!pad) {
        #pragma clang loop unroll(full)
        for (int i = 0; i < 4; ++i) {
            gt[(c * 16u + m0) * 8u + tq + uint(i)] = y8[i];
            gt[(c * 16u + m0 + 8u) * 8u + tq + uint(i)] = y8[4 + i];
        }
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
    uint j = tid / 8u;
    uint t = tid % 8u;
    uint i = col0 + j;
    uint hcd = ka.dim * ka.hc;
    float s = 0.0f;
    for (uint cc = 0u; cc < ka.hc; ++cc) {
        float xnv = (res[t * hcd + cc * ka.dim + i] * invx[t * ka.hc + cc]) * wn[cc * ka.dim + i];
        s += xnv * sigmoid_f32(gt[(cc * 16u + j) * 8u + t]);
    }
    y[t * ka.dim + i] = s * (1.0f / float(ka.hc));
}

// the read-bandwidth reference: float4s of a plane summed
kernel void bw_read(device const float4* q [[buffer(2)]],
        device float* y [[buffer(4)]],
        constant uint& n4 [[buffer(6)]],
        uint gid [[thread_position_in_grid]],
        uint nthr [[threads_per_grid]]) {
    float4 a = float4(0.0f);
    for (uint i = gid; i < n4; i += nthr) {
        a += q[i];
    }
    if (a.x == 1234.5f) {
        y[0] = a.y;
    }
}
