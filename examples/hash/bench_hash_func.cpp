// Find x10 only — isolates "hash function" from "table mechanics" by running
// every cell on the 2x2 cross of:
//   tables: das::daslang_hash_map, absl::flat_hash_map
//   hashes: das::daslang_hash<K>,  absl::Hash<K>
//
// Read columns down for hash effect on same table; read rows across for
// table effect with same hash.
//
// Build:  cmake --build build/example_hash_bench --config Release --target example_hash_func_bench
// Run:    build/example_hash_bench/Release/example_hash_func_bench.exe
//
// Standalone — does not need example_hash_bench to be built.

#include <daScript/das_config.h>
#include <absl/container/flat_hash_map.h>
#include <absl/hash/hash.h>
#include <absl/strings/string_view.h>

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <string>
#include <unordered_set>     // corpus uniqueness only
#include <utility>
#include <vector>

namespace das { void das_throw(const char * msg); }
void das::das_throw(const char * msg) {
    std::fprintf(stderr, "%s\n", msg); std::abort();
}

namespace {

constexpr uint32_t SEED      = 0xC0FFEEu;
constexpr size_t   SIZES[]   = { 1000, 10000, 100000 };
constexpr size_t   MAX_N     = 100000;
constexpr int      FIND_MULT = 10;

volatile size_t g_sink = 0;

int pick_iters_find (size_t n) {
    if (n <= 1000)  return 500;
    if (n <= 10000) return 50;
    return 5;
}

// ===== const char* helpers =====
// For absl on const char*, wrap absl::Hash<string_view> to hash CONTENT
// (absl::Hash<const char*> would hash the pointer — useless here). Equality
// likewise needs strcmp not pointer compare.
struct CStrEq {
    bool operator () (const char * a, const char * b) const noexcept { return std::strcmp(a, b) == 0; }
};
struct AbslCStrContentHash {
    size_t operator () (const char * s) const noexcept {
        return absl::Hash<absl::string_view>{}(absl::string_view(s));
    }
};

// ===== Hash traits per K =====
template <typename K>
struct HashTraits {
    using DasH  = das::daslang_hash<K>;
    using AbslH = absl::Hash<K>;
    using Eq    = std::equal_to<K>;
};
template <>
struct HashTraits<const char*> {
    using DasH  = das::daslang_hash<const char*>;     // FNV-64 on bytes
    using AbslH = AbslCStrContentHash;                // absl Hash on bytes
    using Eq    = CStrEq;
};

// ===== Corpora (same identifier-like generator as the main bench) =====
struct StringCorpus {
    std::vector<std::string> strs;
    std::vector<const char*> cstrs;
    std::vector<uint64_t>    hashes;
};
StringCorpus make_string_corpus (size_t n, uint32_t seed) {
    static const char alpha[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
    constexpr int ALPHA_N = sizeof(alpha) - 1;
    std::mt19937 rng(seed);
    std::normal_distribution<float> len_dist(10.0f, 4.0f);
    StringCorpus c;
    c.strs.reserve(n); c.cstrs.reserve(n); c.hashes.reserve(n);
    std::unordered_set<std::string> seen; seen.reserve(n);
    while (c.strs.size() < n) {
        int len = int(len_dist(rng));
        if (len < 4)  len = 4;
        if (len > 20) len = 20;
        std::string s; s.resize(size_t(len));
        for (int i = 0; i < len; ++i) s[i] = alpha[rng() % ALPHA_N];
        if (seen.insert(s).second) c.strs.push_back(std::move(s));
    }
    das::daslang_hash<das::string> h{};
    for (const auto & s : c.strs) {
        c.cstrs.push_back(s.c_str());
        c.hashes.push_back(uint64_t(h(s)));
    }
    return c;
}

struct PtrPool {
    std::vector<void*> ptrs;
    size_t block_size = 0;
    PtrPool () = default;
    ~PtrPool () { for (auto p : ptrs) std::free(p); }
    PtrPool (const PtrPool &) = delete;
    PtrPool & operator = (const PtrPool &) = delete;
    PtrPool (PtrPool && o) noexcept : ptrs(std::move(o.ptrs)), block_size(o.block_size) {}
};
PtrPool make_ptr_pool (size_t n, size_t block_size) {
    PtrPool p; p.block_size = block_size; p.ptrs.reserve(n);
    for (size_t i = 0; i < n; ++i) p.ptrs.push_back(std::malloc(block_size));
    return p;
}

template <typename K>
std::vector<K> make_find_queries (const std::vector<K> & keys, uint32_t seed) {
    std::mt19937 rng(seed);
    std::vector<K> q; q.reserve(keys.size() * size_t(FIND_MULT));
    std::uniform_int_distribution<size_t> pick(0, keys.size() - 1);
    for (size_t i = 0; i < keys.size() * size_t(FIND_MULT); ++i) q.push_back(keys[pick(rng)]);
    return q;
}

// ===== Generic find bench =====
template <template <class,class,class,class> class TableT, typename K, typename Hash, typename Eq>
double bench_find (const std::vector<K> & keys, const std::vector<K> & queries, int iters,
                   const char * label) {
    TableT<K, void*, Hash, Eq> m;
    for (const auto & k : keys) m.emplace(k, (void*)nullptr);
    if (m.size() != keys.size()) {
        std::fprintf(stderr, "FAIL [%s pre-build]: size %zu != %zu\n", label, m.size(), keys.size());
        std::abort();
    }
    {
        size_t hits = 0;
        for (const auto & q : queries) hits += (m.find(q) != m.end()) ? 1 : 0;
        if (hits != queries.size()) {
            std::fprintf(stderr, "FAIL [%s verify]: hits %zu != %zu\n", label, hits, queries.size());
            std::abort();
        }
    }
    using clk = std::chrono::steady_clock;
    auto t0 = clk::now();
    size_t hits = 0;
    for (int i = 0; i < iters; ++i)
        for (const auto & q : queries) hits += (m.find(q) != m.end()) ? 1 : 0;
    auto t1 = clk::now();
    g_sink ^= hits;
    return double(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()) / iters;
}

template <typename K>
void run_table (const char * title, const std::vector<K> & full_keys) {
    using H = HashTraits<K>;
    std::printf("\n## %s\n\n", title);
    std::printf("| N      | hash    |   das table (ns) |  absl table (ns) | das/absl |\n");
    std::printf("|--------|---------|-----------------:|-----------------:|---------:|\n");
    for (size_t n : SIZES) {
        if (n > full_keys.size()) continue;
        std::vector<K> keys(full_keys.begin(), full_keys.begin() + n);
        const int it = pick_iters_find(n);
        auto queries = make_find_queries(keys, SEED + uint32_t(n));

        double d_dh = bench_find<das::daslang_hash_map, K, typename H::DasH,  typename H::Eq>(keys, queries, it, "das/das");
        double a_dh = bench_find<absl::flat_hash_map,   K, typename H::DasH,  typename H::Eq>(keys, queries, it, "absl/das");
        double d_ah = bench_find<das::daslang_hash_map, K, typename H::AbslH, typename H::Eq>(keys, queries, it, "das/absl");
        double a_ah = bench_find<absl::flat_hash_map,   K, typename H::AbslH, typename H::Eq>(keys, queries, it, "absl/absl");

        std::printf("| %-6zu | daslang | %16.0f | %16.0f | %8.2f |\n", n, d_dh, a_dh, d_dh / a_dh);
        std::printf("| %-6zu | absl    | %16.0f | %16.0f | %8.2f |\n", n, d_ah, a_ah, d_ah / a_ah);
    }
    std::fflush(stdout);
}

} // anonymous

int main () {
    std::printf("# Hash function vs table mechanics — find x10 isolation\n\n");
    std::printf("C++ stdlib: ");
#if defined(_LIBCPP_VERSION)
    std::printf("libc++ %d\n", _LIBCPP_VERSION);
#elif defined(__GLIBCXX__)
    std::printf("libstdc++ %d\n", __GLIBCXX__);
#elif defined(_MSC_VER)
    std::printf("MSVC stdlib (_MSC_VER=%d)\n", _MSC_VER);
#else
    std::printf("unknown\n");
#endif
    std::printf("Compiler: ");
#if defined(__clang__)
    std::printf("clang %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(__GNUC__)
    std::printf("gcc %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    std::printf("MSVC %d\n", _MSC_VER);
#else
    std::printf("unknown\n");
#endif
    std::printf("\nEach cell: pre-build table with N keys, then time 10*N find calls (all hits).\n");
    std::printf("Per (key, N): two rows (daslang hash vs absl hash) x two cols (das table vs absl table).\n");
    std::printf("\n  vertical comparison (within table column): HASH effect for that table.\n");
    std::printf("  horizontal comparison (within hash row):    TABLE effect for that hash.\n");
    std::printf("\nFor K=const char* both hashes hash CONTENT (absl wrapped to use string_view) so it's apples-to-apples.\n");

    auto str_corpus   = make_string_corpus(MAX_N, SEED);
    auto ptr_pool_32  = make_ptr_pool(MAX_N, 32);
    auto ptr_pool_128 = make_ptr_pool(MAX_N, 128);

    run_table<const char*>("Map<const char*, void*>",        str_corpus.cstrs);
    run_table<std::string>("Map<std::string, void*>",        str_corpus.strs);
    run_table<uint64_t>   ("Map<uint64_t, void*>",           str_corpus.hashes);
    run_table<void*>      ("Map<void* (32B alloc), void*>",  ptr_pool_32.ptrs);
    run_table<void*>      ("Map<void* (128B alloc), void*>", ptr_pool_128.ptrs);

    std::printf("\n(checksum: 0x%zx)\n", size_t(g_sink));
    return 0;
}
