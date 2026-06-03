// Stable-sort algorithm bake-off — three byte-pointer candidates vs std::stable_sort.
//
// Candidates (all match the das_*_r surface: void* base, size_t nel, size_t width,
// inline Compare "less"; equal elements must keep input order):
//   #1 das_stable_merge_r   — bottom-up merge, ping-pong buffer, insertion base runs (≤32)
//   #3 das_stable_natural_r — natural-run detection (asc / strict-desc→reverse) + minrun
//                             extend + bottom-up pairwise merge. Exploits existing order.
//   #4 das_stable_index_r   — decorate with [0..N) indices, sort them with the (already
//                             tuned, unstable) das_qsort_r tiebroken by index → stable, then
//                             apply the gather permutation in place via cycles. Scratch is
//                             N*4 bytes, not N*width — meant to win on WIDE elements.
//
// Baseline: std::stable_sort (typed, on SPayload<W> with operator<). Built in both the
// libc++ and libstdc++ binaries (see CMakeLists) so we see vs both stdlibs.
//
// Stability is VERIFIED, not assumed: every element carries its original index; the
// comparator compares only the key; after sort we check sorted-by-key AND idx ascending
// within each equal-key group. An unstable result aborts the run loudly.
//
// Pure C++ — no daslang runtime. Just includes das_qsort_r.h (for byte_swap / sized_memcpy
// / das_qsort_r). Build/run: see examples/sort/CMakeLists.txt.

#include "daScript/simulate/das_qsort_r.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <vector>

using namespace das;

namespace {

// ============================================================================
// Element type — key (sort key) + idx (original position, for stability check).
// pad makes sizeof == W. W >= 16 so pad is never zero-length.
// ============================================================================

template <int W> struct SPayload {
    int32_t key;
    int32_t idx;
    char    pad[W - 8];
    bool operator<(const SPayload & o) const { return key < o.key; }   // key-only → equal keys stay equal
};

template <typename T> int32_t key_of(const T & v) { return v.key; }
template <typename T> int32_t idx_of(const T & v) { return v.idx; }

// Byte-pointer comparator over the candidates: key is the first 4 bytes of every SPayload<W>.
inline bool key_less(const void * a, const void * b) {
    return *static_cast<const int32_t *>(a) < *static_cast<const int32_t *>(b);
}

// Stand-in for the daslang interp regime, where `cmp` is a block-eval (key() twice + less),
// not an inlined int compare. ~24 dummy flops makes the comparator the bottleneck — this is
// the axis that punishes #4 (which does ~2× comparator calls for its stability tiebreak).
inline bool key_less_expensive(const void * a, const void * b) {
    int32_t ka = *static_cast<const int32_t *>(a);
    int32_t kb = *static_cast<const int32_t *>(b);
    volatile uint32_t sink = 0u;
    for (int t = 0; t < 24; t++) sink += (uint32_t(ka) ^ uint32_t(kb + t)) * 2654435761u;
    (void)sink;
    return ka < kb;
}
template <typename T> struct ExpensiveLess {
    bool operator()(const T & x, const T & y) const { return key_less_expensive(&x, &y); }
};

// Comparator-call counter — explains the adaptive wins (a fast result with FEWER comparator
// calls is doing genuinely less work, not skipping it; correctness is proven separately).
static uint64_t g_cmp = 0;
inline bool key_less_counted(const void * a, const void * b) {
    g_cmp++;
    return *static_cast<const int32_t *>(a) < *static_cast<const int32_t *>(b);
}
template <typename T> struct CountingLess {
    bool operator()(const T & x, const T & y) const { g_cmp++; return x.key < y.key; }
};
// Typed comparator for the typed header path das_stable_sort<T>.
template <typename T> struct TypedLess {
    bool operator()(const T & x, const T & y) const { return x.key < y.key; }
};

// ============================================================================
// Shared building blocks
// ============================================================================

// Stable insertion sort over the element range [lo, hi). Only shifts elements that are
// strictly greater than the held value, so equal elements never cross — stable.
template <typename Compare>
static void insertion_run(unsigned char * a, size_t lo, size_t hi, size_t w, Compare cmp, unsigned char * tmp) {
    for (size_t i = lo + 1; i < hi; i++) {
        if (!cmp(a + i*w, a + (i-1)*w)) continue;         // a[i] >= a[i-1] already
        sized_memcpy(tmp, a + i*w, w);
        size_t j = i;
        do {
            sized_memcpy(a + j*w, a + (j-1)*w, w);
            j--;
        } while (j > lo && cmp(tmp, a + (j-1)*w));
        sized_memcpy(a + j*w, tmp, w);
    }
}

// Merge sorted runs [l,m) and [m,r) from src into dst. Stable: takes the left element on a
// tie (right < left is false).
template <typename Compare>
static void merge_runs(const unsigned char * src, unsigned char * dst,
                       size_t l, size_t m, size_t r, size_t w, Compare cmp) {
    size_t i = l, j = m, k = l;
    while (i < m && j < r) {
        if (cmp(src + j*w, src + i*w)) { sized_memcpy(dst + (k++)*w, src + j*w, w); j++; }
        else                          { sized_memcpy(dst + (k++)*w, src + i*w, w); i++; }
    }
    if (i < m) { memcpy(dst + k*w, src + i*w, (m - i)*w); k += (m - i); }
    if (j < r) { memcpy(dst + k*w, src + j*w, (r - j)*w); }
}

// ============================================================================
// #1 — bottom-up merge, ping-pong buffer, insertion base runs
// ============================================================================

template <typename Compare>
inline void das_stable_merge_r(void * base, size_t nel, size_t width, Compare cmp) {
    if (nel <= 1) return;
    unsigned char * a = static_cast<unsigned char *>(base);
    const size_t RUN = 32;
    std::vector<unsigned char> buf(nel * width);
    std::vector<unsigned char> tmp(width);

    for (size_t lo = 0; lo < nel; lo += RUN)
        insertion_run(a, lo, std::min(lo + RUN, nel), width, cmp, tmp.data());

    unsigned char * src = a;
    unsigned char * dst = buf.data();
    for (size_t runLen = RUN; runLen < nel; runLen *= 2) {
        for (size_t lo = 0; lo < nel; lo += 2*runLen) {
            size_t m = std::min(lo + runLen, nel);
            size_t r = std::min(lo + 2*runLen, nel);
            if (m >= r) memcpy(dst + lo*width, src + lo*width, (r - lo)*width);  // lone tail run
            else        merge_runs(src, dst, lo, m, r, width, cmp);
        }
        std::swap(src, dst);
    }
    if (src != a) memcpy(a, src, nel * width);
}

// #3 — natural-run merge is now the SHIPPED algorithm: das_stable_sort_r (byte) +
// das_stable_sort<T> (typed), both in das_qsort_r.h. The bench validates those directly
// (columns "#3 byte" / "#3 typed").

// ============================================================================
// #4 — index decoration: sort indices with das_qsort_r (tiebroken → stable),
//      then apply the gather permutation in place via cycles.
// ============================================================================

template <typename Compare>
inline void das_stable_index_r(void * base, size_t nel, size_t width, Compare cmp) {
    if (nel <= 1) return;
    unsigned char * a = static_cast<unsigned char *>(base);
    std::vector<uint32_t> perm(nel);
    for (uint32_t k = 0; k < uint32_t(nel); k++) perm[k] = k;

    das_qsort_r(perm.data(), nel, sizeof(uint32_t), [&](const void * pa, const void * pb) {
        uint32_t ia = *static_cast<const uint32_t *>(pa);
        uint32_t ib = *static_cast<const uint32_t *>(pb);
        const unsigned char * ea = a + size_t(ia) * width;
        const unsigned char * eb = a + size_t(ib) * width;
        if (cmp(ea, eb)) return true;
        if (cmp(eb, ea)) return false;
        return ia < ib;                                          // unique tiebreak → stable order
    });

    // Apply gather permutation final[k] = orig[perm[k]] in place. Each cycle reads each
    // source exactly once before it is overwritten; orig[k] is parked in tmp.
    std::vector<unsigned char> tmp(width);
    const uint32_t DONE = 0xFFFFFFFFu;
    for (uint32_t k = 0; k < uint32_t(nel); k++) {
        if (perm[k] == DONE || perm[k] == k) { perm[k] = DONE; continue; }
        sized_memcpy(tmp.data(), a + size_t(k)*width, width);    // park orig[k]
        uint32_t cur = k;
        for (;;) {
            uint32_t src = perm[cur];
            if (src == k) { sized_memcpy(a + size_t(cur)*width, tmp.data(), width); perm[cur] = DONE; break; }
            sized_memcpy(a + size_t(cur)*width, a + size_t(src)*width, width);
            perm[cur] = DONE;
            cur = src;
        }
    }
}

// ============================================================================
// Verification — the gold standard. Checks ALL of:
//   * permutation: output idx values are exactly {0..n-1} (no drops, no duplicates)
//   * element integrity: each output element's key matches the input element at that idx
//     (catches field corruption / key-idx mismatch)
//   * sorted by key
//   * stable: idx ascending within each equal-key group
// A "fast because it skipped work" bug fails the permutation/integrity checks.
// ============================================================================

template <typename T>
bool verify_full(const std::vector<T> & out, const std::vector<T> & in) {
    size_t n = out.size();
    if (in.size() != n) return false;
    std::vector<char> seen(n, 0);
    for (size_t i = 0; i < n; i++) {
        int32_t ix = idx_of(out[i]);
        if (ix < 0 || size_t(ix) >= n || seen[ix]) return false;                          // permutation
        seen[ix] = 1;
        if (key_of(out[i]) != key_of(in[size_t(ix)])) return false;                        // element integrity
        if (i && key_of(out[i]) < key_of(out[i-1])) return false;                          // sorted
        if (i && key_of(out[i]) == key_of(out[i-1]) && idx_of(out[i]) < idx_of(out[i-1])) return false; // stable
    }
    return true;
}

// ============================================================================
// Input patterns. idx is always the original position (0..n-1).
// keymod controls collision density (small → many equal-key groups).
// ============================================================================

enum class Pattern { Random, Dups, Sorted, Partial, Reverse, AllEqual };

const char * pattern_name(Pattern p) {
    switch (p) {
        case Pattern::Random:   return "random";
        case Pattern::Dups:     return "dups(k%256)";
        case Pattern::Sorted:   return "sorted";
        case Pattern::Partial:  return "partial95";
        case Pattern::Reverse:  return "reverse";
        case Pattern::AllEqual: return "all-equal";
    }
    return "?";
}

template <typename T>
std::vector<T> make_pattern(Pattern p, size_t n, uint32_t seed) {
    std::mt19937 rng(seed);
    std::vector<T> v(n);
    for (size_t i = 0; i < n; i++) v[i].idx = int32_t(i);
    switch (p) {
        case Pattern::Random:
            for (size_t i = 0; i < n; i++) v[i].key = int32_t(rng() & 0xFFFFFF);
            break;
        case Pattern::Dups:
            for (size_t i = 0; i < n; i++) v[i].key = int32_t(rng() % 256);
            break;
        case Pattern::Sorted:
            for (size_t i = 0; i < n; i++) v[i].key = int32_t(i);
            break;
        case Pattern::Partial:                                   // sorted, then 5% random swaps
            for (size_t i = 0; i < n; i++) v[i].key = int32_t(i);
            for (size_t s = 0; n > 0 && s < n/20; s++) std::swap(v[rng()%n].key, v[rng()%n].key);
            break;
        case Pattern::Reverse:
            for (size_t i = 0; i < n; i++) v[i].key = int32_t(n - i);
            break;
        case Pattern::AllEqual:                                  // every key 42 → output must be input order
            for (size_t i = 0; i < n; i++) v[i].key = 42;
            break;
    }
    return v;
}

// ============================================================================
// Timing harness (mirrors bench_sort_family.cpp)
// ============================================================================

template <typename T, typename Op>
double time_op(const std::vector<T> & seed, int iters, Op op) {
    using clk = std::chrono::steady_clock;
    auto t0 = clk::now();
    for (int i = 0; i < iters; i++) { std::vector<T> a = seed; op(a); }
    auto t1 = clk::now();
    return double(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()) / double(iters);
}

template <typename T, typename Op>
double bench_one(const char * name, Pattern pat, const std::vector<T> & seed, int iters, Op op) {
    std::vector<T> check = seed;
    op(check);
    if (!verify_full(check, seed)) {
        std::fprintf(stderr, "FAIL: '%s' wrong output (pattern=%s, N=%zu, sizeof=%zu)\n",
                     name, pattern_name(pat), seed.size(), sizeof(T));
        std::abort();
    }
    return time_op(seed, iters, op);
}

// ============================================================================
// Edge-case correctness sweep — runs BEFORE any timing. Hammers every candidate
// over tiny/boundary sizes (around the RUN/MINRUN=32 cutoff), every pattern, and
// several seeds, asserting verify_full each time. A bug that "wins" by skipping
// work dies here, loudly, before we ever read a ratio.
// ============================================================================

template <typename T>
void correctness_sweep(const char * type_name) {
    const size_t  ns[]   = { 0, 1, 2, 3, 5, 15, 16, 17, 31, 32, 33, 63, 64, 65, 100, 1000, 4096 };
    const Pattern pats[] = { Pattern::Random, Pattern::Dups, Pattern::Sorted,
                             Pattern::Partial, Pattern::Reverse, Pattern::AllEqual };
    int cases = 0;
    for (uint32_t seed = 1; seed <= 5; seed++) {
        for (size_t n : ns) {
            for (Pattern p : pats) {
                auto in = make_pattern<T>(p, n, seed * 7919u);
                // Run each candidate on its own fresh copy, verify against the pristine input.
                auto fail = [&](const char * nm) {
                    std::fprintf(stderr, "CORRECTNESS FAIL: %s on %s n=%zu pattern=%s seed=%u\n",
                                 nm, type_name, n, pattern_name(p), seed);
                    std::abort();
                };
                auto run = [&](const char * nm, auto fn) {
                    auto out = in;
                    fn(out.data(), out.size(), sizeof(T), key_less);
                    if (!verify_full(out, in)) fail(nm);
                    cases++;
                };
                // Shipped functions (das_qsort_r.h): byte + typed.
                run("#3 byte",  [](void * b, size_t e, size_t w, bool(*c)(const void*, const void*)) { das_stable_sort_r(b, e, w, c); });
                { auto out = in; das_stable_sort(out.data(), out.data() + out.size(), TypedLess<T>()); if (!verify_full(out, in)) fail("#3 typed"); cases++; }
                // Reference candidates (bake-off record): #1 dropped, #4 deferred.
                run("#1 merge", [](void * b, size_t e, size_t w, bool(*c)(const void*, const void*)) { das_stable_merge_r(b, e, w, c); });
                run("#4 index", [](void * b, size_t e, size_t w, bool(*c)(const void*, const void*)) { das_stable_index_r(b, e, w, c); });
            }
        }
    }
    std::printf("correctness: %-14s %d cases OK (sorted + stable + permutation + element-integrity)\n", type_name, cases);
}

int pick_iters(size_t n) {
    if (n <= 1000)  return 5000;
    if (n <= 10000) return 500;
    return 100;
}

template <typename T>
void run_for_type(const char * type_name) {
    const size_t sizes[]      = { 1000, 10000, 100000 };
    const Pattern patterns[]  = { Pattern::Random, Pattern::Dups, Pattern::Sorted, Pattern::Partial, Pattern::Reverse };
    std::printf("\n## %s (sizeof=%zu)\n\n", type_name, sizeof(T));
    std::printf("| N      | pattern    | std::stable | #3 byte   | #3 typed  | #1 merge  | #4 index  | b/std | t/std | i/std |\n");
    std::printf("|--------|------------|------------:|----------:|----------:|----------:|----------:|------:|------:|------:|\n");
    for (size_t n : sizes) {
        int iters = pick_iters(n);
        for (Pattern pat : patterns) {
            auto data = make_pattern<T>(pat, n, 0xC0FFEEu);

            double s_std = bench_one("std::stable_sort", pat, data, iters,
                [](std::vector<T> & a) { std::stable_sort(a.begin(), a.end()); });
            double d_b = bench_one("#3 byte", pat, data, iters,       // shipped: das_stable_sort_r
                [](std::vector<T> & a) { das_stable_sort_r(a.data(), a.size(), sizeof(T), key_less); });
            double d_t = bench_one("#3 typed", pat, data, iters,      // shipped: das_stable_sort<T> (AOT path)
                [](std::vector<T> & a) { das_stable_sort(a.data(), a.data() + a.size(), TypedLess<T>()); });
            double d_m = bench_one("#1 merge", pat, data, iters,      // reference (dropped)
                [](std::vector<T> & a) { das_stable_merge_r(a.data(), a.size(), sizeof(T), key_less); });
            double d_i = bench_one("#4 index", pat, data, iters,      // reference (deferred)
                [](std::vector<T> & a) { das_stable_index_r(a.data(), a.size(), sizeof(T), key_less); });

            std::printf("| %-6zu | %-10s | %11.0f | %9.0f | %9.0f | %9.0f | %9.0f | %5.2f | %5.2f | %5.2f |\n",
                n, pattern_name(pat), s_std, d_b, d_t, d_m, d_i, d_b/s_std, d_t/s_std, d_i/s_std);
        }
    }
}

// Expensive-comparator pass (one width) — simulates the daslang interp regime where the
// comparator dominates. Shows whether #4's 2× comparator calls flip the movement-bound win.
template <typename T>
void run_expensive_cmp(const char * type_name) {
    const size_t sizes[]     = { 10000, 100000 };
    const Pattern patterns[] = { Pattern::Random, Pattern::Dups, Pattern::Sorted, Pattern::Partial, Pattern::Reverse };
    std::printf("\n## EXPENSIVE comparator — %s (sizeof=%zu) — daslang-interp proxy (byte path)\n\n", type_name, sizeof(T));
    std::printf("| N      | pattern    | std::stable | #3 byte   | #1 merge  | #4 index  | b/std | m/std | i/std |\n");
    std::printf("|--------|------------|------------:|----------:|----------:|----------:|------:|------:|------:|\n");
    for (size_t n : sizes) {
        int iters = pick_iters(n) / 4 + 1;          // expensive cmp → fewer iters
        for (Pattern pat : patterns) {
            auto data = make_pattern<T>(pat, n, 0xC0FFEEu);
            double s_std = bench_one("std::stable_sort", pat, data, iters,
                [](std::vector<T> & a) { std::stable_sort(a.begin(), a.end(), ExpensiveLess<T>()); });
            double d_b = bench_one("#3 byte", pat, data, iters,
                [](std::vector<T> & a) { das_stable_sort_r(a.data(), a.size(), sizeof(T), key_less_expensive); });
            double d_m = bench_one("#1 merge", pat, data, iters,
                [](std::vector<T> & a) { das_stable_merge_r(a.data(), a.size(), sizeof(T), key_less_expensive); });
            double d_i = bench_one("#4 index", pat, data, iters,
                [](std::vector<T> & a) { das_stable_index_r(a.data(), a.size(), sizeof(T), key_less_expensive); });
            std::printf("| %-6zu | %-10s | %11.0f | %9.0f | %9.0f | %9.0f | %5.2f | %5.2f | %5.2f |\n",
                n, pattern_name(pat), s_std, d_b, d_m, d_i, d_b/s_std, d_m/s_std, d_i/s_std);
        }
    }
}

template <typename T>
void run_cmp_counts(const char * type_name) {
    const size_t n = 100000;
    const Pattern pats[] = { Pattern::Random, Pattern::Sorted, Pattern::Reverse, Pattern::Dups, Pattern::AllEqual };
    std::printf("\n## comparator-call counts — %s, N=%zu (why the adaptive cells are real)\n\n", type_name, n);
    std::printf("| pattern    | std::stable | #3 byte   | #3 typed  | #1 merge  | #4 index  |\n");
    std::printf("|------------|------------:|----------:|----------:|----------:|----------:|\n");
    for (Pattern p : pats) {
        auto in = make_pattern<T>(p, n, 0xC0FFEEu);
        auto count = [&](auto fn) -> uint64_t { auto a = in; g_cmp = 0; fn(a); return g_cmp; };
        uint64_t cs = count([](std::vector<T> & a) { std::stable_sort(a.begin(), a.end(), CountingLess<T>()); });
        uint64_t cb = count([](std::vector<T> & a) { das_stable_sort_r(a.data(), a.size(), sizeof(T), key_less_counted); });
        uint64_t ct = count([](std::vector<T> & a) { das_stable_sort(a.data(), a.data() + a.size(), CountingLess<T>()); });
        uint64_t cm = count([](std::vector<T> & a) { das_stable_merge_r(a.data(), a.size(), sizeof(T), key_less_counted); });
        uint64_t ci = count([](std::vector<T> & a) { das_stable_index_r(a.data(), a.size(), sizeof(T), key_less_counted); });
        std::printf("| %-10s | %11llu | %9llu | %9llu | %9llu | %9llu |\n", pattern_name(p),
                    (unsigned long long)cs, (unsigned long long)cb, (unsigned long long)ct,
                    (unsigned long long)cm, (unsigned long long)ci);
    }
}

} // anonymous

int main() {
    std::printf("# stable-sort bake-off — das candidates vs std::stable_sort\n\n");
#if defined(_LIBCPP_VERSION)
    std::printf("C++ stdlib: **libc++ %d**\n", _LIBCPP_VERSION);
#elif defined(__GLIBCXX__)
    std::printf("C++ stdlib: **libstdc++ %d**\n", __GLIBCXX__);
#else
    std::printf("C++ stdlib: unknown\n");
#endif
    std::printf("\nColumns: ns/op (lower better); ratios are candidate/std::stable_sort (<1 = we win).\n");
    std::printf("Stability verified per run (idx-within-equal-key check). Timing includes the per-iter vector copy + each algo's own scratch alloc, same as std.\n\n");

    std::printf("### Correctness sweep (runs before timing)\n\n```\n");
    correctness_sweep<SPayload<16>>("SPayload<16>");
    correctness_sweep<SPayload<32>>("SPayload<32>");
    correctness_sweep<SPayload<128>>("SPayload<128>");
    std::printf("```\n");

    run_for_type<SPayload<16>>("SPayload<16>");
    run_for_type<SPayload<32>>("SPayload<32>");
    run_for_type<SPayload<128>>("SPayload<128>");
    run_expensive_cmp<SPayload<32>>("SPayload<32>");
    run_cmp_counts<SPayload<32>>("SPayload<32>");
    return 0;
}
