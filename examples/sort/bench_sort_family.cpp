// Performance comparison: std::* vs the winning das_*_r implementations
// in src/builtin/das_qsort_r.h. After Phase 0.1 algorithm bake-off (replaced
// smoothsort with pdq, Lomuto introselect with Hoare introselect, swap-based
// heap with hole-sliding, and nth-then-sort partial_sort with heap-of-N).
//
// Pure C++ — no daslang runtime. Just include the header.
// Build:  cmake -S examples/sort -B build/example_sort_bench -DCMAKE_BUILD_TYPE=Release
//         cmake --build build/example_sort_bench -j
// Run:    ./build/example_sort_bench/example_sort_bench

#include "daScript/simulate/das_qsort_r.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <vector>

using namespace das;

namespace {

template <int W> struct Payload {
    int32_t key;
    char pad[W - sizeof(int32_t)];
    bool operator<(const Payload & o) const { return key < o.key; }
};
using P32 = Payload<32>;
using P128 = Payload<128>;

template <typename T> T make_value(int32_t k) { T v{}; v.key = k; return v; }
template <> int32_t make_value<int32_t>(int32_t k) { return k; }
template <> int64_t make_value<int64_t>(int32_t k) { return int64_t(k); }

template <typename T> int32_t key_of(const T & v) { return v.key; }
template <> int32_t key_of(const int32_t & v) { return v; }
template <> int32_t key_of(const int64_t & v) { return int32_t(v); }

template <typename T> std::vector<T> make_input(size_t n, uint32_t seed) {
    std::mt19937 rng(seed);
    std::vector<T> a; a.reserve(n);
    for (size_t i = 0; i < n; i++) a.push_back(make_value<T>(int32_t(rng() & 0xFFFFFF)));
    return a;
}

template <typename T> auto das_cmp() {
    return [](const void * a, const void * b) {
        return key_of(*static_cast<const T *>(a)) < key_of(*static_cast<const T *>(b));
    };
}

// C-qsort-style comparator (returns int): used to bench the libc qsort entry
// point. Captureless so it converts to a function pointer.
template <typename T>
static int c_qsort_cmp(const void * a, const void * b)
{
    int32_t ka = key_of(*static_cast<const T *>(a));
    int32_t kb = key_of(*static_cast<const T *>(b));
    return (ka > kb) - (ka < kb);
}

// Typed bool comparator — peer to std::less<T>{} for das_sort<T> and std::sort.
template <typename T>
static bool typed_less(const T & a, const T & b) { return key_of(a) < key_of(b); }

// Verification predicates — every operation's output is checked once before
// timing; bench aborts loudly on incorrect output.

template <typename T> bool is_sorted_range(const std::vector<T> & a) {
    for (size_t i = 1; i < a.size(); i++) if (key_of(a[i]) < key_of(a[i-1])) return false;
    return true;
}

template <typename T> bool is_partial_sorted(const std::vector<T> & a, size_t n) {
    if (n > a.size()) n = a.size();
    for (size_t i = 1; i < n; i++) if (key_of(a[i]) < key_of(a[i-1])) return false;
    if (n == 0 || n == a.size()) return true;
    int32_t kth = key_of(a[n-1]);
    for (size_t i = n; i < a.size(); i++) if (key_of(a[i]) < kth) return false;
    return true;
}

template <typename T> bool is_nth_correct(const std::vector<T> & a, size_t k) {
    if (k >= a.size()) return true;
    int32_t kth = key_of(a[k]);
    for (size_t i = 0; i < k; i++) if (key_of(a[i]) > kth) return false;
    for (size_t i = k+1; i < a.size(); i++) if (key_of(a[i]) < kth) return false;
    return true;
}

template <typename T> bool is_max_heap(const std::vector<T> & a) {
    size_t n = a.size();
    for (size_t i = 0; i < n; i++) {
        size_t l = 2*i + 1, r = 2*i + 2;
        if (l < n && key_of(a[i]) < key_of(a[l])) return false;
        if (r < n && key_of(a[i]) < key_of(a[r])) return false;
    }
    return true;
}

template <typename T, typename Op>
double time_op(const std::vector<T> & seed_data, int iters, Op op) {
    using clk = std::chrono::steady_clock;
    auto t0 = clk::now();
    for (int i = 0; i < iters; i++) {
        std::vector<T> a = seed_data;
        op(a);
    }
    auto t1 = clk::now();
    double ns = double(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
    return ns / double(iters);
}

template <typename T, typename Op, typename Verify>
double bench_one(const char * name, const std::vector<T> & seed_data, int iters, Op op, Verify verify) {
    std::vector<T> check = seed_data;
    op(check);
    if (!verify(check)) {
        std::fprintf(stderr, "FAIL: '%s' produced wrong output (N=%zu, sizeof=%zu)\n",
                     name, seed_data.size(), sizeof(T));
        std::abort();
    }
    return time_op(seed_data, iters, op);
}

int pick_iters(size_t n) {
    if (n <= 1000)    return 10000;
    if (n <= 10000)   return 1000;
    return 200;  // n <= 100000 — higher iter count for stable ratios
}

template <typename T>
void run_for_type(const char * type_name) {
    const size_t sizes[] = { 1000, 10000, 100000 };
    const size_t topn = 10;
    std::printf("\n## %s (sizeof=%zu)\n\n", type_name, sizeof(T));
    std::printf("| N      | op                  |   std (ns) |   das (ns) | ratio das/std |\n");
    std::printf("|--------|---------------------|-----------:|-----------:|--------------:|\n");
    for (size_t n : sizes) {
        auto data = make_input<T>(n, 0xC0FFEEu);
        int iters = pick_iters(n);

        double s_sort = bench_one("std::sort", data, iters,
            [](std::vector<T> & a) { std::sort(a.begin(), a.end()); },
            is_sorted_range<T>);
        double d_sort = bench_one("das_qsort_r", data, iters,
            [](std::vector<T> & a) { das_qsort_r(a.data(), a.size(), sizeof(T), das_cmp<T>()); },
            is_sorted_range<T>);

        auto verify_partial = [](const std::vector<T> & a) { return is_partial_sorted(a, topn); };
        double s_partial = bench_one("std::partial_sort", data, iters,
            [](std::vector<T> & a) { std::partial_sort(a.begin(), a.begin() + topn, a.end()); },
            verify_partial);
        double d_partial = bench_one("das_partial_sort_r", data, iters,
            [](std::vector<T> & a) { das_partial_sort_r(a.data(), a.size(), topn, sizeof(T), das_cmp<T>()); },
            verify_partial);

        size_t k = n / 2;
        auto verify_nth = [k](const std::vector<T> & a) { return is_nth_correct(a, k); };
        double s_nth = bench_one("std::nth_element", data, iters,
            [k](std::vector<T> & a) { std::nth_element(a.begin(), a.begin() + k, a.end()); },
            verify_nth);
        double d_nth = bench_one("das_nth_element_r", data, iters,
            [k](std::vector<T> & a) { das_nth_element_r(a.data(), a.size(), k, sizeof(T), das_cmp<T>()); },
            verify_nth);

        double s_heap = bench_one("std::make_heap", data, iters,
            [](std::vector<T> & a) { std::make_heap(a.begin(), a.end()); },
            is_max_heap<T>);
        double d_heap = bench_one("das_make_heap_r", data, iters,
            [](std::vector<T> & a) { das_make_heap_r(a.data(), a.size(), sizeof(T), das_cmp<T>()); },
            is_max_heap<T>);

        double s_hsort = bench_one("std heap_sort", data, iters,
            [](std::vector<T> & a) {
                std::make_heap(a.begin(), a.end());
                for (auto it = a.end(); it != a.begin(); --it) std::pop_heap(a.begin(), it);
            },
            is_sorted_range<T>);
        double d_hsort = bench_one("das heap_sort", data, iters,
            [](std::vector<T> & a) {
                das_make_heap_r(a.data(), a.size(), sizeof(T), das_cmp<T>());
                for (size_t len = a.size(); len > 0; --len) {
                    das_pop_heap_r(a.data(), len, sizeof(T), das_cmp<T>());
                }
            },
            is_sorted_range<T>);

        auto row = [&](const char * op, double s, double d) {
            std::printf("| %-6zu | %-19s | %10.0f | %10.0f | %13.2f |\n", n, op, s, d, d / s);
        };
        row("sort",         s_sort,    d_sort);
        row("partial_sort",  s_partial, d_partial);
        row("nth_element",   s_nth,     d_nth);
        row("make_heap",     s_heap,    d_heap);
        row("heap_sort",     s_hsort,   d_hsort);
    }
}

// Sort-only deep dive: four entry points on the same input.
//   std::sort       — typed iterator, typed cmp, fully inlined (apples-to-apples ceiling)
//   C qsort         — byte-pointer + runtime width + int-cmp via function pointer (libc baseline)
//   das_qsort_r     — byte-pointer + runtime width + bool-cmp (our daslang binding-path impl)
//   das_sort<T>     — typed iterator + typed cmp (our typed pdqsort-lite, apples-to-apples vs std::sort)
//
// The four-way table isolates two cost dimensions:
//   - typed vs byte-pointer  (das_sort<T> vs das_qsort_r — same algorithm, different access)
//   - our algorithm vs libstdc++'s (das_sort<T> vs std::sort — same access, different algorithm)
template <typename T>
void run_sort_deep_dive(const char * type_name)
{
    const size_t sizes[] = { 1000, 10000, 100000 };
    std::printf("\n## sort deep-dive — %s (sizeof=%zu)\n\n", type_name, sizeof(T));
    std::printf("Columns: std::sort=stdlib, C=libc qsort, dr=das_qsort_r (hybrid byte), db=das_qsort_block_r\n");
    std::printf("(pure block byte), dt=das_sort<T> (hybrid typed), dtb=das_sort_block<T> (pure block typed)\n\n");
    std::printf("| N      |  std::sort |   C qsort  |    dr_byte |  db_byte_b |   dt_typed | dtb_typed_b| C/std  | dr/std | db/std | dt/std | dtb/std|\n");
    std::printf("|--------|-----------:|-----------:|-----------:|-----------:|-----------:|-----------:|-------:|-------:|-------:|-------:|-------:|\n");
    for (size_t n : sizes) {
        auto data = make_input<T>(n, 0xC0FFEEu);
        int iters = pick_iters(n);

        double s_sort = bench_one("std::sort", data, iters,
            [](std::vector<T> & a) { std::sort(a.begin(), a.end()); },
            is_sorted_range<T>);
        double c_sort = bench_one("C qsort", data, iters,
            [](std::vector<T> & a) { std::qsort(a.data(), a.size(), sizeof(T), c_qsort_cmp<T>); },
            is_sorted_range<T>);
        double dr_sort = bench_one("das_qsort_r", data, iters,
            [](std::vector<T> & a) { das_qsort_r(a.data(), a.size(), sizeof(T), das_cmp<T>()); },
            is_sorted_range<T>);
        double db_sort = bench_one("das_qsort_block_r", data, iters,
            [](std::vector<T> & a) { das_qsort_block_r(a.data(), a.size(), sizeof(T), das_cmp<T>()); },
            is_sorted_range<T>);
        double dt_sort = bench_one("das_sort<T>", data, iters,
            [](std::vector<T> & a) { das_sort(a.data(), a.data() + a.size(), typed_less<T>); },
            is_sorted_range<T>);
        double dtb_sort = bench_one("das_sort_block<T>", data, iters,
            [](std::vector<T> & a) { das_sort_block(a.data(), a.data() + a.size(), typed_less<T>); },
            is_sorted_range<T>);

        std::printf("| %-6zu | %10.0f | %10.0f | %10.0f | %10.0f | %10.0f | %10.0f | %6.2f | %6.2f | %6.2f | %6.2f | %6.2f |\n",
            n, s_sort, c_sort, dr_sort, db_sort, dt_sort, dtb_sort,
            c_sort / s_sort, dr_sort / s_sort, db_sort / s_sort, dt_sort / s_sort, dtb_sort / s_sort);
    }
}

} // anonymous

int main() {
    std::printf("# das_qsort_r.h vs std::* — final perf comparison (Phase 0.1)\n\n");
#if defined(_LIBCPP_VERSION)
    std::printf("C++ stdlib: **libc++ %d** (pdqsort + block-quicksort partition)\n", _LIBCPP_VERSION);
#elif defined(__GLIBCXX__)
    std::printf("C++ stdlib: **libstdc++ %d** (Musser introsort)\n", __GLIBCXX__);
#else
    std::printf("C++ stdlib: unknown\n");
#endif
    std::printf("Compiler: ");
#if defined(__clang__)
    std::printf("clang %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(__GNUC__)
    std::printf("gcc %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#else
    std::printf("unknown\n");
#endif
    std::printf("\nEach implementation's output is verified for correctness before timing —\n");
    std::printf("the harness aborts loudly on a bad result.\n");
    std::printf("partial_sort uses top-N=10; nth_element uses k=N/2; heap_sort builds then drains via pop_heap.\n");
    run_for_type<int32_t>("int32_t");
    run_for_type<int64_t>("int64_t");
    run_for_type<P32>("Payload<32>");
    run_for_type<P128>("Payload<128>");

    std::printf("\n# Sort deep-dive — four entry points on the same input\n");
    std::printf("\nIsolates byte-pointer-vs-typed cost and our-algorithm-vs-libstdc++'s.\n");
    run_sort_deep_dive<int32_t>("int32_t");
    run_sort_deep_dive<int64_t>("int64_t");
    run_sort_deep_dive<P32>("Payload<32>");
    run_sort_deep_dive<P128>("Payload<128>");
    return 0;
}
