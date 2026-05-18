// Bake-off: byte_swap variants at widths used by the sort family.
// Candidate that wins across {4..256} replaces das::byte_swap in das_qsort_r.h.
//
// Build:  cmake -S examples/sort -B build/example_sort_bench -DCMAKE_BUILD_TYPE=Release
//         cmake --build build/example_sort_bench -j
// Run:    ./build/example_sort_bench/example_byte_swap_bench

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <vector>

// ===== Candidate implementations =====

// chunked-256 — current production impl in src/builtin/das_qsort_r.h.
static inline void bs_chunked256(void *pa, void *pb, size_t width)
{
    unsigned char tmp[256];
    unsigned char *a = (unsigned char *)pa;
    unsigned char *b = (unsigned char *)pb;
    while (width) {
        size_t chunk = sizeof(tmp) < width ? sizeof(tmp) : width;
        memcpy(tmp, a, chunk);
        memcpy(a, b, chunk);
        memcpy(b, tmp, chunk);
        a += chunk; b += chunk; width -= chunk;
    }
}

// chunked-64 — same algorithm, smaller stack buffer. Control to isolate
// whether the 256B stack write is the cost driver.
static inline void bs_chunked64(void *pa, void *pb, size_t width)
{
    unsigned char tmp[64];
    unsigned char *a = (unsigned char *)pa;
    unsigned char *b = (unsigned char *)pb;
    while (width) {
        size_t chunk = sizeof(tmp) < width ? sizeof(tmp) : width;
        memcpy(tmp, a, chunk);
        memcpy(a, b, chunk);
        memcpy(b, tmp, chunk);
        a += chunk; b += chunk; width -= chunk;
    }
}

// words64 — Linux-kernel-style word swap, no buffer. Caller must ensure
// width is a multiple of 8 and pointers are 8-aligned.
static inline void bs_words64(void *pa, void *pb, size_t width)
{
    unsigned char *a = (unsigned char *)pa;
    unsigned char *b = (unsigned char *)pb;
    do {
        width -= 8;
        uint64_t t;
        memcpy(&t, a + width, 8);
        memcpy(a + width, b + width, 8);
        memcpy(b + width, &t, 8);
    } while (width);
}

// sized — switch on width, sized memcpy that the compiler lowers
// to a single SIMD load/store pair. Falls back to chunked-256 for unknown widths.
static inline void bs_sized(void *pa, void *pb, size_t width)
{
    unsigned char *a = (unsigned char *)pa;
    unsigned char *b = (unsigned char *)pb;
    switch (width) {
        case 4:  { uint32_t t; memcpy(&t, a, 4); memcpy(a, b, 4); memcpy(b, &t, 4); return; }
        case 8:  { uint64_t t; memcpy(&t, a, 8); memcpy(a, b, 8); memcpy(b, &t, 8); return; }
        case 16: { unsigned char t[16]; memcpy(t, a, 16); memcpy(a, b, 16); memcpy(b, t, 16); return; }
        case 32: { unsigned char t[32]; memcpy(t, a, 32); memcpy(a, b, 32); memcpy(b, t, 32); return; }
        case 64: { unsigned char t[64]; memcpy(t, a, 64); memcpy(a, b, 64); memcpy(b, t, 64); return; }
    }
    bs_chunked256(pa, pb, width);
}

// hybrid — best-of: sized inline for w ∈ {4,8,16,32,64}, word-swap loop for
// aligned-multiple-of-8, chunked-256 as ultimate fallback.
static inline void bs_hybrid(void *pa, void *pb, size_t width)
{
    unsigned char *a = (unsigned char *)pa;
    unsigned char *b = (unsigned char *)pb;
    switch (width) {
        case 4:  { uint32_t t; memcpy(&t, a, 4); memcpy(a, b, 4); memcpy(b, &t, 4); return; }
        case 8:  { uint64_t t; memcpy(&t, a, 8); memcpy(a, b, 8); memcpy(b, &t, 8); return; }
        case 16: { unsigned char t[16]; memcpy(t, a, 16); memcpy(a, b, 16); memcpy(b, t, 16); return; }
        case 32: { unsigned char t[32]; memcpy(t, a, 32); memcpy(a, b, 32); memcpy(b, t, 32); return; }
        case 64: { unsigned char t[64]; memcpy(t, a, 64); memcpy(a, b, 64); memcpy(b, t, 64); return; }
    }
    if ((((uintptr_t)a | (uintptr_t)b | width) & 7u) == 0) {
        bs_words64(pa, pb, width);
        return;
    }
    bs_chunked256(pa, pb, width);
}

// ===== Harness =====

namespace {

// Correctness check: swap two distinct buffers, check that contents exchanged.
template <typename Fn>
void verify(const char * name, Fn fn, size_t width) {
    std::vector<unsigned char> a(width), b(width), a0(width), b0(width);
    std::mt19937 rng(0xBADBEEFu);
    for (size_t i = 0; i < width; i++) { a[i] = a0[i] = (unsigned char)(rng() & 0xFF); b[i] = b0[i] = (unsigned char)(rng() & 0xFF); }
    fn(a.data(), b.data(), width);
    if (memcmp(a.data(), b0.data(), width) != 0 || memcmp(b.data(), a0.data(), width) != 0) {
        std::fprintf(stderr, "FAIL: '%s' produced wrong swap at width=%zu\n", name, width);
        std::abort();
    }
}

template <typename Fn>
double time_op(Fn fn, size_t width, size_t pairs, int iters)
{
    // Big arena of `pairs` × 2 elements; swap each pair in turn. Iterate
    // many times to amortize timing overhead.
    std::vector<unsigned char> data(pairs * 2 * width);
    std::mt19937 rng(0xC0FFEE);
    for (size_t i = 0; i < data.size(); i++) data[i] = (unsigned char)(rng() & 0xFF);
    using clk = std::chrono::steady_clock;
    auto t0 = clk::now();
    for (int it = 0; it < iters; it++) {
        for (size_t p = 0; p < pairs; p++) {
            fn(data.data() + (2*p) * width, data.data() + (2*p + 1) * width, width);
        }
    }
    auto t1 = clk::now();
    // Anti-DCE: force a volatile observation of post-loop data so the optimizer
    // can't prove the swap loop is dead. (The prior `data.back() == 0xDEAD`
    // guard was always false — `data.back()` is unsigned char (0–255), can
    // never equal 0xDEAD — so the optimizer could fold and eliminate the loop.)
    volatile unsigned char observed = data.back();
    (void)observed;
    double ns = double(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
    return ns / double(iters) / double(pairs);
}

struct Row { const char * name; double ns; };

void run_width(size_t width)
{
    const size_t pairs = 1024;   // L1-friendly working set: pairs * 2 * width
    const int iters = 5000;

    verify("chunked256", bs_chunked256, width);
    verify("chunked64",  bs_chunked64,  width);
    if (width >= 8 && (width % 8) == 0) verify("words64", bs_words64, width);
    verify("sized",      bs_sized,      width);
    verify("hybrid",     bs_hybrid,     width);

    double r_chunked256 = time_op(bs_chunked256, width, pairs, iters);
    double r_chunked64  = time_op(bs_chunked64,  width, pairs, iters);
    double r_words64    = (width >= 8 && (width % 8) == 0) ? time_op(bs_words64, width, pairs, iters) : -1.0;
    double r_sized      = time_op(bs_sized,      width, pairs, iters);
    double r_hybrid     = time_op(bs_hybrid,     width, pairs, iters);

    std::printf("| %4zu | %10.2f | %10.2f | ", width, r_chunked256, r_chunked64);
    if (r_words64 < 0.0) std::printf("        n/a | ");
    else                 std::printf("%10.2f | ", r_words64);
    std::printf("%10.2f | %10.2f |\n", r_sized, r_hybrid);
}

} // anonymous

int main()
{
    std::printf("# byte_swap candidate bake-off (ns per swap, lower=better)\n\n");
    std::printf("Working set: 1024 pairs × 2 × W bytes. 5000 iterations. Per-pair time printed.\n\n");
    std::printf("| W    | chunked256 |  chunked64 |    words64 |      sized |     hybrid |\n");
    std::printf("|------|-----------:|-----------:|-----------:|-----------:|-----------:|\n");
    for (size_t w : { (size_t)4, (size_t)8, (size_t)16, (size_t)32, (size_t)64, (size_t)128, (size_t)256, (size_t)512 }) {
        run_width(w);
    }
    return 0;
}
