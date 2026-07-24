// Accelerate (AMX/SME) GEMM/GEMV ceiling curves for the "accelerate" batch-donor backend.
// Measures cblas_sgemm GFLOPS vs token count M at a weight shape (N out-rows, K), matching the
// exact call pattern ggml-blas.cpp uses (row-major, C[MxN] = Y[MxK] * X[NxK]^T), plus a
// cblas_sgemv leg for the decode-lane datapoint. Re-run verbatim on any new box (M4/M5).
//
// build: cc -O3 -std=c11 -DACCELERATE_NEW_LAPACK modules/dasLLAMA/harness/accel_sgemm_bench.c \
//        -framework Accelerate -o /tmp/accel_sgemm_bench
// run:   /tmp/accel_sgemm_bench N K [strips]     (strips: split N into that many sgemm calls —
//                                                 the strip-vs-full-plane scratch question)
//        M sweep is built in: 1 2 4 8 16 32 64 128 256 512.
// env:   VECLIB_MAXIMUM_THREADS caps Accelerate's internal threading (report echoes it).
// NOTE: timing runs only inside a granted quiet window (Parsec off) — see measurement protocol.

#include <Accelerate/Accelerate.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

// one timed pass: strips sgemm calls covering all N rows (strips==1 -> the ggml-blas call shape)
static void gemm_pass(int M, int N, int K, int strips, const float * y, const float * x, float * c) {
    for (int s = 0; s < strips; s++) {
        const int n0 = (int)((long long)N * s / strips);
        const int n1 = (int)((long long)N * (s + 1) / strips);
        if (n1 <= n0) continue;
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
                    M, n1 - n0, K,
                    1.0f, y, K,
                          x + (size_t)n0 * K, K,
                    0.0f, c + n0, N);
    }
}

int main(int argc, char ** argv) {
    const int N = argc > 1 ? atoi(argv[1]) : 4096;
    const int K = argc > 2 ? atoi(argv[2]) : 4096;
    const int strips = argc > 3 ? atoi(argv[3]) : 1;
    if (N <= 0 || K <= 0 || strips <= 0) {
        fprintf(stderr, "usage: %s N K [strips]\n", argv[0]);
        return 1;
    }
    const char * vt = getenv("VECLIB_MAXIMUM_THREADS");
    printf("# accel_sgemm_bench N=%d K=%d strips=%d VECLIB_MAXIMUM_THREADS=%s\n",
           N, K, strips, vt ? vt : "(unset)");

    static const int MS[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512 };
    const int NM = (int)(sizeof(MS) / sizeof(MS[0]));
    const int MMAX = MS[NM - 1];

    float * x = malloc((size_t)N * K * sizeof(float));      // weights, dequant stand-in
    float * y = malloc((size_t)MMAX * K * sizeof(float));   // activations
    float * c = malloc((size_t)MMAX * N * sizeof(float));
    if (!x || !y || !c) { fprintf(stderr, "alloc failed\n"); return 1; }
    fill(x, (size_t)N * K, 1);
    fill(y, (size_t)MMAX * K, 2);

    printf("# %6s %12s %10s %10s %8s\n", "M", "GFLOPS", "ms/pass", "reps", "GMAC/s");
    for (int mi = 0; mi < NM; mi++) {
        const int M = MS[mi];
        const double flops = 2.0 * M * (double)N * K;
        gemm_pass(M, N, K, strips, y, x, c);                // warmup / first-touch
        // adaptive rep count for >=100ms of timed work
        int reps = 1;
        for (;;) {
            const double t0 = now_s();
            for (int r = 0; r < reps; r++) gemm_pass(M, N, K, strips, y, x, c);
            const double dt = now_s() - t0;
            if (dt >= 0.1 || reps >= 1 << 20) {
                printf("  %6d %12.1f %10.3f %10d %8.1f\n",
                       M, flops * reps / dt / 1e9, dt / reps * 1e3, reps, flops * reps / dt / 2e9);
                break;
            }
            reps = dt > 0.001 ? (int)(reps * (0.12 / dt) + 1) : reps * 8;
        }
    }

    // decode datapoint: sgemv (x is row-major NxK, y one token) vs sgemm at M=1 above
    {
        const double flops = 2.0 * (double)N * K;
        cblas_sgemv(CblasRowMajor, CblasNoTrans, N, K, 1.0f, x, K, y, 1, 0.0f, c, 1);
        int reps = 1;
        for (;;) {
            const double t0 = now_s();
            for (int r = 0; r < reps; r++)
                cblas_sgemv(CblasRowMajor, CblasNoTrans, N, K, 1.0f, x, K, y, 1, 0.0f, c, 1);
            const double dt = now_s() - t0;
            if (dt >= 0.1 || reps >= 1 << 20) {
                printf("# sgemv        %12.1f GFLOPS %10.3f ms/pass  (f32 weight-stream %.1f GB/s)\n",
                       flops * reps / dt / 1e9, dt / reps * 1e3,
                       (double)N * K * sizeof(float) * reps / dt / 1e9);
                break;
            }
            reps = dt > 0.001 ? (int)(reps * (0.12 / dt) + 1) : reps * 8;
        }
    }

    free(x); free(y); free(c);
    return 0;
}
