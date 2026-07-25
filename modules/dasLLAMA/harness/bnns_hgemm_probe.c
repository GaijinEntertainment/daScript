// BNNS f16/bf16 matmul feasibility probe for the dasLLAMA BNNS-f16 arm.
// Answers, in order: (1) which dtype combos BNNSFilterCreateLayerBroadcastMatMul accepts and
// computes correctly as DYNAMIC two-input filters (b_is_weights=1 creates fine but
// BNNSFilterApplyTwoInput returns -1 — weights-marked filters are a dead end here, and dynamic
// suits the driver anyway: no capture, no pointer-lifetime coupling); (2) accumulator width of
// the f16 paths (adversarial 1024+0.25 vector); (3) filter create/destroy cost at the E4B proj
// shape (create-per-call feasibility); (4) GFLOPS vs M per working combo, n_threads 1 vs
// default, against the known cblas_sgemm f32 ceilings; (5) [strips] pthread strip-dispatch
// (n_threads=1 per strip, strided o_desc) — thread-safety, placement, aggregate scaling.
//
// M1 Max verdicts (2026-07-24, E4B proj shape 10752x2560): f16xf16->f32 rel_err 1.5e-6 with a
// PROVEN f32 accumulator (1663.75 exact), 2316 GFLOPS internal-MT / 1194 single-thread / ~12ms
// best strips (6-16 strips, == internal-MT ceiling; f32 sgemm ref 2155 = 13.1ms, single 881);
// create+destroy 0.2us -> create-per-call; f32xbf16->f32 works (2096 internal-MT) but its
// internal widen does NOT strip-scale (best 18.5ms @ strips=2, worse beyond) -> under the
// jobque-strips contention discipline the shipped arm is f16xf16 (weights bf16->f16 once,
// acts f32->f16 per call), bf16-direct is a documented dead lane; f16xbf16 mixed CREATE fails.
//
// build: cc -O3 -std=c11 -Wno-deprecated-declarations modules/dasLLAMA/harness/bnns_hgemm_probe.c \
//        -framework Accelerate -o /tmp/bnns_probe
// run:   /tmp/bnns_probe [N K [strips]]   (default 10752 2560 — the E4B per_layer_model_proj shape)

#define ACCELERATE_NEW_LAPACK
#include <Accelerate/Accelerate.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef _Float16 f16_t;

static double now_s(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + 1e-9 * (double)ts.tv_nsec;
}

static void fill(float * p, size_t n, unsigned seed) {
    unsigned s = seed * 2654435761u + 12345u;
    for (size_t i = 0; i < n; i++) {
        s = s * 1664525u + 1013904223u;
        p[i] = (float)((s >> 8) & 0xffff) / 65536.0f - 0.5f;
    }
}

// round-to-nearest-even f32 -> bf16
static uint16_t f32_to_bf16(float f) {
    uint32_t b; memcpy(&b, &f, 4);
    b += 0x7fffu + ((b >> 16) & 1u);
    return (uint16_t)(b >> 16);
}
static float bf16_to_f32(uint16_t h) {
    uint32_t b = (uint32_t)h << 16;
    float f; memcpy(&f, &b, 4);
    return f;
}

enum dt { DT_F32, DT_F16, DT_BF16 };
static const char * dt_name[] = { "f32", "f16", "bf16" };
static BNNSDataType dt_bnns(enum dt t) {
    switch (t) {
        case DT_F16:  return BNNSDataTypeFloat16;
        case DT_BF16: return BNNSDataTypeBFloat16;
        default:      return BNNSDataTypeFloat32;
    }
}
static size_t dt_size(enum dt t) { return t == DT_F32 ? 4 : 2; }

// convert f32 source into dtype buffer; returns rounded-back f32 copy for the reference
static void dt_convert(enum dt t, const float * src, void * dst, float * rounded, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (t == DT_F32) {
            ((float *)dst)[i] = src[i];
            rounded[i] = src[i];
        } else if (t == DT_F16) {
            f16_t h = (f16_t)src[i];
            ((f16_t *)dst)[i] = h;
            rounded[i] = (float)h;
        } else {
            uint16_t h = f32_to_bf16(src[i]);
            ((uint16_t *)dst)[i] = h;
            rounded[i] = bf16_to_f32(h);
        }
    }
}

static void desc_matrix(BNNSNDArrayDescriptor * d, enum dt t, size_t cols, size_t rows) {
    memset(d, 0, sizeof(*d));
    d->layout = BNNSDataLayoutRowMajorMatrix;
    d->size[0] = cols;     // size[0] = columns (innermost)
    d->size[1] = rows;
    d->data_type = dt_bnns(t);
}

// dynamic two-input filter: C[MxN] = A[MxK] * B[NxK]^T, all pointers at apply time.
// o_ldc > N writes a strided output (row stride o_ldc values) — the strip-dispatch shape.
static BNNSFilter make_mm(enum dt ta, enum dt tb, enum dt to, int M, int N, int K,
                          size_t n_threads, int o_ldc) {
    BNNSLayerParametersBroadcastMatMul p;
    memset(&p, 0, sizeof(p));
    p.alpha = 1.0f;
    p.beta = 0.0f;
    p.transB = true;
    desc_matrix(&p.iA_desc, ta, (size_t)K, (size_t)M);
    desc_matrix(&p.iB_desc, tb, (size_t)K, (size_t)N);
    desc_matrix(&p.o_desc, to, (size_t)N, (size_t)M);
    if (o_ldc > 0) {
        p.o_desc.stride[0] = 1;
        p.o_desc.stride[1] = (size_t)o_ldc;
    }
    BNNSFilterParameters fp;
    memset(&fp, 0, sizeof(fp));
    fp.n_threads = n_threads;
    return BNNSFilterCreateLayerBroadcastMatMul(&p, &fp);
}

static double ref_and_err(const float * ra, const float * rb, const void * c, enum dt to,
                          int M, int N, int K) {
    double max_err = 0.0, max_ref = 0.0;
    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            double acc = 0.0;
            for (int k = 0; k < K; k++) acc += (double)ra[(size_t)m * K + k] * rb[(size_t)n * K + k];
            double got = to == DT_F32 ? (double)((const float *)c)[(size_t)m * N + n]
                       : to == DT_F16 ? (double)((const f16_t *)c)[(size_t)m * N + n]
                                      : (double)bf16_to_f32(((const uint16_t *)c)[(size_t)m * N + n]);
            double e = fabs(got - acc);
            if (e > max_err) max_err = e;
            if (fabs(acc) > max_ref) max_ref = fabs(acc);
        }
    }
    return max_err / (max_ref > 0 ? max_ref : 1.0);
}

// one strip of the pthread strip-dispatch test: create+apply+destroy on rows [n0,n1) of W,
// writing the strided output slice c[.., n0:n1] in place (o_ldc = N) — the das driver call shape
struct strip_arg { int n0, n1, M, N, K; const void * pa; const uint8_t * pb; size_t brow;
                   enum dt ta, tb; float * c; int rc; };
static void * strip_go(void * argp) {
    struct strip_arg * a = argp;
    BNNSFilter f = make_mm(a->ta, a->tb, DT_F32, a->M, a->n1 - a->n0, a->K, 1, a->N);
    if (!f) { a->rc = -1; return NULL; }
    a->rc = BNNSFilterApplyTwoInput(f, a->pa, a->pb + (size_t)a->n0 * a->brow, a->c + a->n0);
    BNNSFilterDestroy(f);
    return NULL;
}

int main(int argc, char ** argv) {
    const int N = argc > 1 ? atoi(argv[1]) : 10752;
    const int K = argc > 2 ? atoi(argv[2]) : 2560;
    printf("# bnns_hgemm_probe N=%d K=%d (dynamic two-input filters)\n", N, K);

    struct { enum dt a, b, o; const char * note; } combos[] = {
        { DT_F32, DT_F32, DT_F32,  "sanity" },
        { DT_F16, DT_F16, DT_F32,  "the arm" },
        { DT_F16, DT_F16, DT_F16,  "f16 out" },
        { DT_F32, DT_F16, DT_F32,  "mixed: f32 acts, no x conversion" },
        { DT_F32, DT_BF16, DT_F32, "direct bf16 weights, f32 acts" },
        { DT_F16, DT_BF16, DT_F32, "direct bf16 weights, f16 acts" },
        { DT_BF16, DT_BF16, DT_F32, "bf16 both" },
    };
    const int NC = (int)(sizeof(combos) / sizeof(combos[0]));
    bool ok[16] = { false };

    // ---------- (1) dtype combo matrix, small shape ----------
    {
        const int cM = 8, cN = 64, cK = K;
        float * fa = malloc((size_t)cM * cK * 4), * fb = malloc((size_t)cN * cK * 4);
        float * ra = malloc((size_t)cM * cK * 4), * rb = malloc((size_t)cN * cK * 4);
        void * da = malloc((size_t)cM * cK * 4), * db = malloc((size_t)cN * cK * 4);
        void * dc = malloc((size_t)cM * cN * 4);
        fill(fa, (size_t)cM * cK, 1);
        fill(fb, (size_t)cN * cK, 2);
        printf("## combo matrix (M=%d N=%d K=%d, rel err vs f64 ref on rounded inputs)\n", cM, cN, cK);
        for (int i = 0; i < NC; i++) {
            dt_convert(combos[i].a, fa, da, ra, (size_t)cM * cK);
            dt_convert(combos[i].b, fb, db, rb, (size_t)cN * cK);
            memset(dc, 0, (size_t)cM * cN * 4);
            BNNSFilter f = make_mm(combos[i].a, combos[i].b, combos[i].o, cM, cN, cK, 1, 0);
            if (!f) {
                printf("  %4s x %4s -> %4s : CREATE FAILED   (%s)\n",
                       dt_name[combos[i].a], dt_name[combos[i].b], dt_name[combos[i].o], combos[i].note);
                continue;
            }
            int rc = BNNSFilterApplyTwoInput(f, da, db, dc);
            double err = rc == 0 ? ref_and_err(ra, rb, dc, combos[i].o, cM, cN, cK) : -1.0;
            ok[i] = rc == 0 && err >= 0 && err < 0.05;
            printf("  %4s x %4s -> %4s : rc=%d rel_err=%.3g   (%s)\n",
                   dt_name[combos[i].a], dt_name[combos[i].b], dt_name[combos[i].o], rc, err, combos[i].note);
            BNNSFilterDestroy(f);
        }

        // ---------- (2) accumulator width: 1024 + 0.25 * (K-1) ----------
        // f16 ulp at 1024 is 1.0 -> a strict-f16 accumulator drops every +0.25 (RNE), lands ~1024;
        // an f32 accumulator lands exactly 1024 + 0.25*(K-1).
        for (int oi = 0; oi < 2; oi++) {
            enum dt to = oi == 0 ? DT_F32 : DT_F16;
            f16_t * ha = (f16_t *)da, * hb = (f16_t *)db;
            for (int k = 0; k < cK; k++) { ha[k] = (f16_t)(k == 0 ? 1024.0f : 0.25f); hb[k] = (f16_t)1.0f; }
            BNNSFilter f = make_mm(DT_F16, DT_F16, to, 1, 1, cK, 1, 0);
            if (!f) { printf("## accum test (o=%s): CREATE FAILED\n", dt_name[to]); continue; }
            memset(dc, 0, 4);
            int rc = BNNSFilterApplyTwoInput(f, da, db, dc);
            double got = to == DT_F32 ? (double)((float *)dc)[0] : (double)((f16_t *)dc)[0];
            printf("## accum test (o=%s): rc=%d got=%.2f  (f32-accum => %.2f, f16-accum => ~1024)\n",
                   dt_name[to], rc, got, 1024.0 + 0.25 * (cK - 1));
            BNNSFilterDestroy(f);
        }
        free(fa); free(fb); free(ra); free(rb); free(da); free(db); free(dc);
    }

    // ---------- (3)+(4) proj-shape perf ----------
    static const int MS[] = { 32, 64, 128, 256, 512 };
    const int NM = (int)(sizeof(MS) / sizeof(MS[0]));
    const int MMAX = MS[NM - 1];
    float * fw = malloc((size_t)N * K * 4);
    float * fx = malloc((size_t)MMAX * K * 4);
    fill(fw, (size_t)N * K, 3);
    fill(fx, (size_t)MMAX * K, 4);
    f16_t * hw = malloc((size_t)N * K * 2);
    f16_t * hx = malloc((size_t)MMAX * K * 2);
    uint16_t * bw = malloc((size_t)N * K * 2);
    for (size_t i = 0; i < (size_t)N * K; i++) { hw[i] = (f16_t)fw[i]; bw[i] = f32_to_bf16(fw[i]); }
    for (size_t i = 0; i < (size_t)MMAX * K; i++) hx[i] = (f16_t)fx[i];
    float * c = malloc((size_t)MMAX * N * 4);

    {   // filter create cost at proj shape — the create-per-call question
        double t0 = now_s();
        const int reps = 100;
        for (int r = 0; r < reps; r++) {
            BNNSFilter f = make_mm(DT_F16, DT_F16, DT_F32, 512, N, K, 1, 0);
            if (!f) { printf("## create-cost: CREATE FAILED\n"); break; }
            BNNSFilterDestroy(f);
        }
        printf("## filter create+destroy (M=512 dynamic): %.1f us/call\n", (now_s() - t0) / reps * 1e6);
    }

    for (int i = 0; i < NC; i++) {
        if (!ok[i] || (combos[i].a == DT_F32 && combos[i].b == DT_F32)) continue;
        const void * pa = combos[i].a == DT_F32 ? (const void *)fx
                        : combos[i].a == DT_F16 ? (const void *)hx : NULL;
        const void * pb = combos[i].b == DT_F16 ? (const void *)hw
                        : combos[i].b == DT_BF16 ? (const void *)bw : (const void *)fw;
        if (!pa) continue;  // no bf16 activation buffer staged — skip bf16-A perf
        for (int ti = 0; ti < 2; ti++) {
            const size_t nthr = ti == 0 ? 1 : 0;
            printf("## %s x %s -> %s, n_threads=%zu\n",
                   dt_name[combos[i].a], dt_name[combos[i].b], dt_name[combos[i].o], nthr);
            printf("# %6s %12s %10s %8s\n", "M", "GFLOPS", "ms/pass", "reps");
            for (int mi = 0; mi < NM; mi++) {
                const int M = MS[mi];
                BNNSFilter f = make_mm(combos[i].a, combos[i].b, combos[i].o, M, N, K, nthr, 0);
                if (!f) { printf("  %6d CREATE FAILED\n", M); continue; }
                const double flops = 2.0 * M * (double)N * K;
                if (BNNSFilterApplyTwoInput(f, pa, pb, c) != 0) {   // warmup + rc guard
                    printf("  %6d APPLY FAILED\n", M);
                    BNNSFilterDestroy(f);
                    continue;
                }
                int reps = 1;
                for (;;) {
                    const double t0 = now_s();
                    for (int r = 0; r < reps; r++) BNNSFilterApplyTwoInput(f, pa, pb, c);
                    const double dt = now_s() - t0;
                    if (dt >= 0.1 || reps >= 1 << 20) {
                        printf("  %6d %12.1f %10.3f %8d\n", M, flops * reps / dt / 1e9, dt / reps * 1e3, reps);
                        break;
                    }
                    reps = dt > 0.001 ? (int)(reps * (0.12 / dt) + 1) : reps * 8;
                }
                BNNSFilterDestroy(f);
            }
        }
    }

    // strips mode: 8 pthreads, each create+apply+destroy on its own row slice of W with
    // n_threads=1 — the exact shape of the das strip-dispatch. Checks thread-safety and
    // whether the aggregate reaches the internal-MT ceiling.
    if (argc > 3 && atoi(argv[3]) > 1) {
        const int strips = atoi(argv[3]) > 64 ? 64 : atoi(argv[3]);
        const int M = 512;
        for (int ci = 0; ci < 2; ci++) {
            enum dt ta = ci == 0 ? DT_F16 : DT_F32;
            enum dt tb = ci == 0 ? DT_F16 : DT_BF16;
            const void * pa = ci == 0 ? (const void *)hx : (const void *)fx;
            const uint8_t * pb = ci == 0 ? (const uint8_t *)hw : (const uint8_t *)bw;
            const double flops = 2.0 * M * (double)N * K;
            double best = 1e30;
            int bad = 0;
            for (int rep = 0; rep < 8 && !bad; rep++) {
                pthread_t th[64];
                struct strip_arg sa[64];
                const double t0 = now_s();
                for (int s = 0; s < strips; s++) {
                    sa[s] = (struct strip_arg){ (int)((long long)N * s / strips),
                                                (int)((long long)N * (s + 1) / strips),
                                                M, N, K, pa, pb, dt_size(tb) * (size_t)K,
                                                ta, tb, c, 0 };
                    pthread_create(&th[s], NULL, strip_go, &sa[s]);
                }
                for (int s = 0; s < strips; s++) { pthread_join(th[s], NULL); bad |= sa[s].rc; }
                const double dt = now_s() - t0;
                if (dt < best) best = dt;
            }
            if (bad) { printf("## strips %s x %s: APPLY FAILED\n", dt_name[ta], dt_name[tb]); continue; }
            // strided-write placement check: full-plane apply (internal MT) vs strips result
            float * cref = malloc((size_t)M * N * 4);
            BNNSFilter f = make_mm(ta, tb, DT_F32, M, N, K, 0, 0);
            int rc = f ? BNNSFilterApplyTwoInput(f, pa, pb, cref) : -1;
            if (f) BNNSFilterDestroy(f);
            double mism = -1.0;
            if (rc == 0) {
                mism = 0.0;
                for (size_t i = 0; i < (size_t)M * N; i += 97) {
                    double e = fabs((double)c[i] - (double)cref[i]);
                    double r = fabs((double)cref[i]) + 1e-6;
                    if (e / r > mism) mism = e / r;
                }
            }
            free(cref);
            printf("## strips=%d %s x %s -> f32, M=%d, n_threads=1 each: %.1f GFLOPS best (%.3f ms), vs-full rel mismatch %.3g\n",
                   strips, dt_name[ta], dt_name[tb], M, flops / best / 1e9, best * 1e3, mism);
        }
    }

    // f32 sgemm reference point at M=512 (same box, same buffers) for the ratio line
    {
        const int M = 512;
        const double flops = 2.0 * M * (double)N * K;
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, M, N, K, 1.0f, fx, K, fw, K, 0.0f, c, N);
        int reps = 1;
        for (;;) {
            const double t0 = now_s();
            for (int r = 0; r < reps; r++)
                cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, M, N, K, 1.0f, fx, K, fw, K, 0.0f, c, N);
            const double dt = now_s() - t0;
            if (dt >= 0.1 || reps >= 1 << 20) {
                printf("## cblas_sgemm f32 reference M=512 (this process): %.1f GFLOPS\n", flops * reps / dt / 1e9);
                break;
            }
            reps = dt > 0.001 ? (int)(reps * (0.12 / dt) + 1) : reps * 8;
        }
    }

    free(fw); free(fx); free(hw); free(hx); free(bw); free(c);
    return 0;
}
