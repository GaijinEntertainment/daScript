// kernel_bench — single-thread raw Q8_0 x Q8_0 dot-kernel throughput for llama.cpp/ggml.
// The kernel-to-kernel mirror of dasLLAMA's harness/gemm_1core_probe.das: no graph, no threadpool,
// no repack (x86 has no Q8_0 weight repack) — quantize weights + activations once, then loop
// rows OUTER / tokens INNER (weights streamed once, the ntok x n activation slice stays
// cache-resident — the same traversal as the das row-major batch kernel) calling
// ggml_vec_dot_q8_0_q8_0 directly through the CPU type traits. Best-of-N rounds, GMAC/s.
//
// Lives in dasLLAMA/harness/oracle/; copy to <llama.cpp>/examples/kernel-bench/ and add
// `add_subdirectory(kernel-bench)` to examples/CMakeLists.txt (same wiring as simple-ids).

#include "ggml.h"
#include "ggml-cpu.h"

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <vector>

static void fillf(std::vector<float> & a, int64_t cnt) {
    a.resize((size_t) cnt);
    for (int64_t i = 0; i < cnt; i++) {
        a[(size_t) i] = (float) (i % 13) * 0.1f - 0.6f;
    }
}

static double now_us() {
    using namespace std::chrono;
    return duration<double, std::micro>(steady_clock::now().time_since_epoch()).count();
}

static void bench_shape(const char * name, int64_t n, int64_t d, int64_t ntok, int reps, int rounds) {
    const ggml_type_traits_cpu * tr = ggml_get_type_traits_cpu(GGML_TYPE_Q8_0);
    const size_t row_sz = ggml_row_size(GGML_TYPE_Q8_0, n);

    std::vector<float> wf;
    fillf(wf, n * d);
    std::vector<uint8_t> wq(row_sz * (size_t) d);
    ggml_quantize_chunk(GGML_TYPE_Q8_0, wf.data(), wq.data(), 0, d, n, nullptr);

    std::vector<float> xf;
    fillf(xf, n);
    std::vector<uint8_t> xq(row_sz * (size_t) ntok);
    for (int64_t tk = 0; tk < ntok; tk++) {
        tr->from_float(xf.data(), xq.data() + (size_t) tk * row_sz, n);
    }

    std::vector<float> y((size_t) (ntok * d));
    double best = 1e30;
    for (int r = 0; r < rounds; r++) {
        const double t0 = now_us();
        for (int rep = 0; rep < reps; rep++) {
            for (int64_t row = 0; row < d; row++) {
                const uint8_t * wrow = wq.data() + (size_t) row * row_sz;
                for (int64_t tk = 0; tk < ntok; tk++) {
                    tr->vec_dot((int) n, &y[(size_t) (tk * d + row)], 0, wrow, 0, xq.data() + (size_t) tk * row_sz, 0, 1);
                }
            }
        }
        const double us = now_us() - t0;
        if (us < best) {
            best = us;
        }
    }
    const double gmacs = (double) n * (double) d * (double) ntok * reps / (best * 1e3);
    printf("--- %s (n=%lld d=%lld ntok=%lld, x%d) ---\n", name, (long long) n, (long long) d, (long long) ntok, reps);
    printf("  vec_dot_q8_0_q8_0: %.3f GMAC/s  (%.0f us best-of-%d)\n", gmacs, best, rounds);
}

int main() {
    printf("single-thread raw ggml Q8_0 x Q8_0 vec_dot throughput (no graph, no threadpool), ntok=128\n\n");
    bench_shape("kv  2048x512  ", 2048, 512, 128, 8, 6);
    bench_shape("qo  2048x2048 ", 2048, 2048, 128, 3, 6);
    bench_shape("w13 2048x5632 ", 2048, 5632, 128, 1, 6);
    bench_shape("w2  5632x2048 ", 5632, 2048, 128, 1, 6);
    return 0;
}
