// Find x10 only — compares daslang_hash_{map,set} vs daslang_insert_only_hash_{map,set}.
//
// Both use the same hash, same load factor, same layout (parallel hashes/keys/values
// vectors). The insert-only variant drops erase + tombstone tracking. Since no erases
// happen in the bench, the tables end up structurally identical — this measures the
// inherent cost of carrying the erase machinery (one extra branch in reserve_slot, one
// extra `<= HASH_KILLED` in iterator skip; find_index is identical between the two).
// Likely outcome: small delta, dominated by noise. The bench answers "by how much?".
//
// Build:  cmake --build build/example_hash_bench --config Release --target example_hash_insert_only_bench
// Run:    build/example_hash_bench/Release/example_hash_insert_only_bench.exe

#include <daScript/das_config.h>

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <string>
#include <unordered_set>
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

struct CStrEq {
    bool operator () (const char * a, const char * b) const noexcept { return std::strcmp(a, b) == 0; }
};
struct CStrHash {
    size_t operator () (const char * s) const noexcept {
        uint64_t h = 14695981039346656037ull;
        for (; *s; ++s) { h ^= uint8_t(*s); h *= 1099511628211ull; }
        return size_t(h);
    }
};

// Map traits — pick (Regular, InsertOnly) per key type
template <typename K>
struct MapTraits {
    using Reg = das::daslang_hash_map<K, void*>;
    using IO  = das::daslang_insert_only_hash_map<K, void*>;
};
template <>
struct MapTraits<const char*> {
    using Reg = das::daslang_hash_map<const char*, void*, CStrHash, CStrEq>;
    using IO  = das::daslang_insert_only_hash_map<const char*, void*, CStrHash, CStrEq>;
};
template <typename K>
struct SetTraits {
    using Reg = das::daslang_hash_set<K>;
    using IO  = das::daslang_insert_only_hash_set<K>;
};
template <>
struct SetTraits<const char*> {
    using Reg = das::daslang_hash_set<const char*, CStrHash, CStrEq>;
    using IO  = das::daslang_insert_only_hash_set<const char*, CStrHash, CStrEq>;
};

// ===== Corpora (same generator as the other benches) =====
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

// ===== Map find bench =====
template <typename Map>
double bench_find_map (const std::vector<typename Map::key_type> & keys,
                       const std::vector<typename Map::key_type> & queries,
                       int iters, const char * label) {
    Map m;
    for (const auto & k : keys) m.emplace(k, (void*)nullptr);
    if (m.size() != keys.size()) {
        std::fprintf(stderr, "FAIL [%s map pre-build]: size %zu != %zu\n", label, m.size(), keys.size());
        std::abort();
    }
    {
        size_t hits = 0;
        for (const auto & q : queries) hits += (m.find(q) != m.end()) ? 1 : 0;
        if (hits != queries.size()) {
            std::fprintf(stderr, "FAIL [%s map verify]: hits %zu != %zu\n", label, hits, queries.size());
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

template <typename Set>
double bench_find_set (const std::vector<typename Set::key_type> & keys,
                       const std::vector<typename Set::key_type> & queries,
                       int iters, const char * label) {
    Set s;
    for (const auto & k : keys) s.insert(k);
    if (s.size() != keys.size()) {
        std::fprintf(stderr, "FAIL [%s set pre-build]: size %zu != %zu\n", label, s.size(), keys.size());
        std::abort();
    }
    {
        size_t hits = 0;
        for (const auto & q : queries) hits += (s.find(q) != s.end()) ? 1 : 0;
        if (hits != queries.size()) {
            std::fprintf(stderr, "FAIL [%s set verify]: hits %zu != %zu\n", label, hits, queries.size());
            std::abort();
        }
    }
    using clk = std::chrono::steady_clock;
    auto t0 = clk::now();
    size_t hits = 0;
    for (int i = 0; i < iters; ++i)
        for (const auto & q : queries) hits += (s.find(q) != s.end()) ? 1 : 0;
    auto t1 = clk::now();
    g_sink ^= hits;
    return double(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()) / iters;
}

template <typename K>
void run_map_rows (const char * title, const std::vector<K> & full_keys) {
    using T = MapTraits<K>;
    std::printf("\n## %s\n\n", title);
    std::printf("| N      |   regular (ns) | insert-only (ns) |  IO/reg |\n");
    std::printf("|--------|---------------:|-----------------:|--------:|\n");
    for (size_t n : SIZES) {
        if (n > full_keys.size()) continue;
        std::vector<K> keys(full_keys.begin(), full_keys.begin() + n);
        const int it = pick_iters_find(n);
        auto queries = make_find_queries(keys, SEED + uint32_t(n));
        double reg = bench_find_map<typename T::Reg>(keys, queries, it, "regular");
        double io  = bench_find_map<typename T::IO> (keys, queries, it, "insert-only");
        std::printf("| %-6zu | %14.0f | %16.0f | %7.3f |\n", n, reg, io, io / reg);
    }
    std::fflush(stdout);
}

template <typename K>
void run_set_rows (const char * title, const std::vector<K> & full_keys) {
    using T = SetTraits<K>;
    std::printf("\n## %s\n\n", title);
    std::printf("| N      |   regular (ns) | insert-only (ns) |  IO/reg |\n");
    std::printf("|--------|---------------:|-----------------:|--------:|\n");
    for (size_t n : SIZES) {
        if (n > full_keys.size()) continue;
        std::vector<K> keys(full_keys.begin(), full_keys.begin() + n);
        const int it = pick_iters_find(n);
        auto queries = make_find_queries(keys, SEED + uint32_t(n));
        double reg = bench_find_set<typename T::Reg>(keys, queries, it, "regular");
        double io  = bench_find_set<typename T::IO> (keys, queries, it, "insert-only");
        std::printf("| %-6zu | %14.0f | %16.0f | %7.3f |\n", n, reg, io, io / reg);
    }
    std::fflush(stdout);
}

} // anonymous

int main () {
    std::printf("# daslang_hash_* vs daslang_insert_only_hash_* — find x10\n\n");
    std::printf("Stdlib: ");
#if defined(_MSC_VER)
    std::printf("MSVC stdlib (_MSC_VER=%d)\n", _MSC_VER);
#elif defined(__GLIBCXX__)
    std::printf("libstdc++ %d\n", __GLIBCXX__);
#elif defined(_LIBCPP_VERSION)
    std::printf("libc++ %d\n", _LIBCPP_VERSION);
#else
    std::printf("unknown\n");
#endif
    std::printf("Compiler: ");
#if defined(_MSC_VER)
    std::printf("MSVC %d\n", _MSC_VER);
#elif defined(__clang__)
    std::printf("clang %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(__GNUC__)
    std::printf("gcc %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#else
    std::printf("unknown\n");
#endif
    std::printf("\nNo erases happen in this bench, so both variants build structurally identical tables.\n");
    std::printf("Find_index code is identical between regular and insert-only — the only differences\n");
    std::printf("are iterator skip cost (irrelevant for find) and reserve_slot branch count (irrelevant\n");
    std::printf("for find after the table is built). IO/reg ratio at ~1.0 = no meaningful difference.\n");

    auto str_corpus   = make_string_corpus(MAX_N, SEED);
    auto ptr_pool_32  = make_ptr_pool(MAX_N, 32);
    auto ptr_pool_128 = make_ptr_pool(MAX_N, 128);

    run_map_rows<const char*>("Map<const char*, void*>",        str_corpus.cstrs);
    run_map_rows<std::string>("Map<std::string, void*>",        str_corpus.strs);
    run_map_rows<uint64_t>   ("Map<uint64_t, void*>",           str_corpus.hashes);
    run_map_rows<void*>      ("Map<void* (32B alloc), void*>",  ptr_pool_32.ptrs);
    run_map_rows<void*>      ("Map<void* (128B alloc), void*>", ptr_pool_128.ptrs);

    run_set_rows<const char*>("Set<const char*>",        str_corpus.cstrs);
    run_set_rows<std::string>("Set<std::string>",        str_corpus.strs);
    run_set_rows<uint64_t>   ("Set<uint64_t>",           str_corpus.hashes);
    run_set_rows<void*>      ("Set<void* (32B alloc)>",  ptr_pool_32.ptrs);
    run_set_rows<void*>      ("Set<void* (128B alloc)>", ptr_pool_128.ptrs);

    std::printf("\n(checksum: 0x%zx)\n", size_t(g_sink));
    return 0;
}
