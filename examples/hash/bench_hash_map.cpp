// Performance comparison: std::unordered_* vs das::daslang_hash_* vs
// absl::flat_hash_* on the four key shapes daslang's AST/runtime tables
// actually use (const char* / std::string / uint64_t hashes / void* allocs).
//
// Three workloads per (shape, key, size):
//   insert  — build a fresh table of N keys from empty (no reserve)
//   churn   — same as insert but interleaved with p=0.1 random erase
//   find    — pre-build then time 10*N find calls (all hits)
//
// Build:  cmake -S examples/hash -B build/example_hash_bench -DCMAKE_BUILD_TYPE=Release
//         cmake --build build/example_hash_bench -j
// Run:    ./build/example_hash_bench/example_hash_bench
//
// Hashing-fairness note: for K=const char* all three impls are forced
// to a content-hashing CStrHash so the per-op work is identical; only
// table mechanics (probing / chaining / SwissTable groups) differ.
// For K=std::string / uint64_t / void* each impl uses ITS own blessed
// hash — that's a "full stack vs full stack" comparison.

#include <daScript/das_config.h>
#include <unordered_map>
#include <unordered_set>

#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <string>
#include <utility>
#include <vector>

// das_config.h declares but doesn't define das::das_throw; provide one
// so daslang_hash_map::at()'s missing-key path resolves at link time.
namespace das { void das_throw(const char * msg); }
void das::das_throw(const char * msg) {
    std::fprintf(stderr, "%s\n", msg);
    std::abort();
}

namespace {

constexpr uint32_t SEED = 0xC0FFEEu;
constexpr size_t   SIZES[] = { 1000, 10000, 100000 };
constexpr size_t   MAX_N   = 100000;
constexpr int      FIND_MULT = 10;

// DCE sink — every workload xors its result into here so the optimizer
// can't elide the work.
volatile size_t g_sink = 0;

// ===== Probe instrumentation =====
//
// We wrap each impl's KeyEqual in CountingEq, which bumps two counters
// for every call. To measure "collision factor" portably across three
// very different impls (chained / linear / SwissTable), we count the
// number of full key-equality calls per find — that's the real work
// the table has to do AFTER its pre-screening (hash chain walk for std,
// hash match for das, SIMD H2 match for absl). Per-find counters are
// reset by start_find()/end_find() around each m.find() call site.
size_t g_eq_calls       = 0;
size_t g_eq_max_per_find = 0;
size_t g_eq_cur         = 0;

template <typename BaseEq>
struct CountingEq {
    BaseEq base{};
    template <typename A, typename B>
    bool operator () (const A & a, const B & b) const noexcept {
        ++g_eq_calls;
        ++g_eq_cur;
        return base(a, b);
    }
};

inline void start_find () noexcept { g_eq_cur = 0; }
inline void end_find   () noexcept { if (g_eq_cur > g_eq_max_per_find) g_eq_max_per_find = g_eq_cur; }

int pick_iters_build (size_t n) {
    if (n <= 1000)  return 1000;
    if (n <= 10000) return 100;
    return 10;                            // n <= 100000
}
int pick_iters_find (size_t n) {
    if (n <= 1000)  return 500;
    if (n <= 10000) return 50;
    return 5;                             // n <= 100000
}

// ===== Hashing-fair adapters for const char* =====
//
// FNV-64 byte hash — matches daslang_hash<const char*> exactly.
struct CStrHash {
    size_t operator () (const char * s) const noexcept {
        uint64_t h = 14695981039346656037ull;
        for (; *s; ++s) { h ^= uint8_t(*s); h *= 1099511628211ull; }
        return size_t(h);
    }
};
struct CStrEq {
    bool operator () (const char * a, const char * b) const noexcept {
        return std::strcmp(a, b) == 0;
    }
};

// ===== Corpora =====
struct StringCorpus {
    std::vector<std::string> strs;     // owns bytes
    std::vector<const char*> cstrs;    // .c_str() of each
    std::vector<uint64_t>    hashes;   // FNV-64 of each
};
StringCorpus make_string_corpus (size_t n, uint32_t seed) {
    static const char alpha[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
    constexpr int ALPHA_N = sizeof(alpha) - 1;
    std::mt19937 rng(seed);
    std::normal_distribution<float> len_dist(10.0f, 4.0f);
    StringCorpus c;
    c.strs.reserve(n); c.cstrs.reserve(n); c.hashes.reserve(n);
    CStrHash hasher;
    // Use a set to ensure uniqueness; retry on collision so we get exactly N.
    std::unordered_set<std::string> seen;
    seen.reserve(n);
    while (c.strs.size() < n) {
        int len = int(len_dist(rng));
        if (len < 4) len = 4;
        if (len > 20) len = 20;
        std::string s;
        s.resize(size_t(len));
        for (int i = 0; i < len; ++i) s[i] = alpha[rng() % ALPHA_N];
        if (seen.insert(s).second) c.strs.push_back(std::move(s));
    }
    // c_str() pointers must be captured AFTER all push_backs since
    // strs may reallocate. Reserve above guarantees no reallocation past
    // this point, but capture in a separate pass to keep the contract clear.
    for (const auto & s : c.strs) {
        c.cstrs.push_back(s.c_str());
        c.hashes.push_back(uint64_t(hasher(s.c_str())));
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

// Query vectors (10*N samples from keys, with replacement). All hits.
template <typename K>
std::vector<K> make_find_queries (const std::vector<K> & keys, uint32_t seed) {
    std::mt19937 rng(seed);
    std::vector<K> q; q.reserve(keys.size() * size_t(FIND_MULT));
    std::uniform_int_distribution<size_t> pick(0, keys.size() - 1);
    for (size_t i = 0; i < keys.size() * size_t(FIND_MULT); ++i) {
        q.push_back(keys[pick(rng)]);
    }
    return q;
}

// ===== Per-key container traits =====
template <typename K>
struct MapTypes {
    using StdM  = std::unordered_map<K, void*>;
    using DasM  = das::daslang_hash_map<K, void*>;
    using AbslM = absl::flat_hash_map<K, void*>;
};
template <>
struct MapTypes<const char*> {
    using StdM  = std::unordered_map<const char*, void*, CStrHash, CStrEq>;
    using DasM  = das::daslang_hash_map<const char*, void*, CStrHash, CStrEq>;
    using AbslM = absl::flat_hash_map<const char*, void*, CStrHash, CStrEq>;
};
template <typename K>
struct SetTypes {
    using StdS  = std::unordered_set<K>;
    using DasS  = das::daslang_hash_set<K>;
    using AbslS = absl::flat_hash_set<K>;
};
template <>
struct SetTypes<const char*> {
    using StdS  = std::unordered_set<const char*, CStrHash, CStrEq>;
    using DasS  = das::daslang_hash_set<const char*, CStrHash, CStrEq>;
    using AbslS = absl::flat_hash_set<const char*, CStrHash, CStrEq>;
};

// ===== Counting variants — used ONLY for probe-stat measurement, not perf =====
// Plug CountingEq<base> into the KeyEqual slot of each impl. Hash function and
// other template params stay identical to the perf-path types above so the
// internal layout is the same — only the eq call is instrumented.
template <typename K>
struct CountingMapTypes {
    using StdM  = std::unordered_map<K, void*, std::hash<K>,         CountingEq<std::equal_to<K>>>;
    using DasM  = das::daslang_hash_map<K, void*, das::daslang_hash<K>, CountingEq<std::equal_to<K>>>;
    using AbslM = absl::flat_hash_map<K, void*, absl::Hash<K>,       CountingEq<std::equal_to<K>>>;
};
template <>
struct CountingMapTypes<const char*> {
    using StdM  = std::unordered_map<const char*, void*, CStrHash, CountingEq<CStrEq>>;
    using DasM  = das::daslang_hash_map<const char*, void*, CStrHash, CountingEq<CStrEq>>;
    using AbslM = absl::flat_hash_map<const char*, void*, CStrHash, CountingEq<CStrEq>>;
};

template <typename K>
struct CountingSetTypes {
    using StdS  = std::unordered_set<K, std::hash<K>,         CountingEq<std::equal_to<K>>>;
    using DasS  = das::daslang_hash_set<K, das::daslang_hash<K>, CountingEq<std::equal_to<K>>>;
    using AbslS = absl::flat_hash_set<K, absl::Hash<K>,       CountingEq<std::equal_to<K>>>;
};
template <>
struct CountingSetTypes<const char*> {
    using StdS  = std::unordered_set<const char*, CStrHash, CountingEq<CStrEq>>;
    using DasS  = das::daslang_hash_set<const char*, CStrHash, CountingEq<CStrEq>>;
    using AbslS = absl::flat_hash_set<const char*, CStrHash, CountingEq<CStrEq>>;
};

// ===== Workloads — map =====
template <typename Map>
double bench_insert_map (const std::vector<typename Map::key_type> & keys, int iters,
                         const char * label) {
    // Verify once.
    {
        Map check;
        for (const auto & k : keys) check.emplace(k, (void*)nullptr);
        if (check.size() != keys.size()) {
            std::fprintf(stderr, "FAIL [%s insert map]: size %zu != expected %zu\n",
                         label, check.size(), keys.size());
            std::abort();
        }
    }
    using clk = std::chrono::steady_clock;
    auto t0 = clk::now();
    size_t check_sum = 0;
    for (int i = 0; i < iters; ++i) {
        Map m;
        for (const auto & k : keys) m.emplace(k, (void*)nullptr);
        check_sum ^= m.size();
    }
    auto t1 = clk::now();
    g_sink ^= check_sum;
    return double(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()) / iters;
}

template <typename Map>
double bench_churn_map (const std::vector<typename Map::key_type> & keys, int iters,
                        const char * label) {
    using K = typename Map::key_type;
    // Verify once: count erases via same RNG path; compare against m.size().
    {
        std::mt19937 rng(SEED);
        std::uniform_real_distribution<float> p01(0.0f, 1.0f);
        Map check;
        std::vector<K> live; live.reserve(keys.size());
        for (const auto & k : keys) {
            check.emplace(k, (void*)nullptr);
            live.push_back(k);
            if (p01(rng) < 0.1f && !live.empty()) {
                size_t victim = std::uniform_int_distribution<size_t>(0, live.size() - 1)(rng);
                check.erase(live[victim]);
                live[victim] = live.back();
                live.pop_back();
            }
        }
        if (check.size() != live.size()) {
            std::fprintf(stderr, "FAIL [%s churn map]: size %zu != live %zu\n",
                         label, check.size(), live.size());
            std::abort();
        }
    }
    using clk = std::chrono::steady_clock;
    auto t0 = clk::now();
    size_t check_sum = 0;
    for (int i = 0; i < iters; ++i) {
        std::mt19937 rng(SEED + uint32_t(i));
        std::uniform_real_distribution<float> p01(0.0f, 1.0f);
        Map m;
        std::vector<K> live; live.reserve(keys.size());
        for (const auto & k : keys) {
            m.emplace(k, (void*)nullptr);
            live.push_back(k);
            if (p01(rng) < 0.1f && !live.empty()) {
                size_t victim = std::uniform_int_distribution<size_t>(0, live.size() - 1)(rng);
                m.erase(live[victim]);
                live[victim] = live.back();
                live.pop_back();
            }
        }
        check_sum ^= m.size();
    }
    auto t1 = clk::now();
    g_sink ^= check_sum;
    return double(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()) / iters;
}

template <typename Map>
double bench_find_map (const std::vector<typename Map::key_type> & keys,
                       const std::vector<typename Map::key_type> & queries,
                       int iters, const char * label) {
    Map m;
    for (const auto & k : keys) m.emplace(k, (void*)nullptr);
    if (m.size() != keys.size()) {
        std::fprintf(stderr, "FAIL [%s find pre-build]: size %zu != expected %zu\n",
                     label, m.size(), keys.size());
        std::abort();
    }
    // Verify once: every query should be a hit.
    {
        size_t hits = 0;
        for (const auto & q : queries) hits += (m.find(q) != m.end()) ? 1 : 0;
        if (hits != queries.size()) {
            std::fprintf(stderr, "FAIL [%s find verify]: hits %zu != queries %zu\n",
                         label, hits, queries.size());
            std::abort();
        }
    }
    using clk = std::chrono::steady_clock;
    auto t0 = clk::now();
    size_t hits = 0;
    for (int i = 0; i < iters; ++i) {
        for (const auto & q : queries) hits += (m.find(q) != m.end()) ? 1 : 0;
    }
    auto t1 = clk::now();
    g_sink ^= hits;
    return double(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()) / iters;
}

// ===== Workloads — set =====
template <typename Set>
double bench_insert_set (const std::vector<typename Set::key_type> & keys, int iters,
                         const char * label) {
    {
        Set check;
        for (const auto & k : keys) check.insert(k);
        if (check.size() != keys.size()) {
            std::fprintf(stderr, "FAIL [%s insert set]: size %zu != expected %zu\n",
                         label, check.size(), keys.size());
            std::abort();
        }
    }
    using clk = std::chrono::steady_clock;
    auto t0 = clk::now();
    size_t check_sum = 0;
    for (int i = 0; i < iters; ++i) {
        Set s;
        for (const auto & k : keys) s.insert(k);
        check_sum ^= s.size();
    }
    auto t1 = clk::now();
    g_sink ^= check_sum;
    return double(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()) / iters;
}

template <typename Set>
double bench_churn_set (const std::vector<typename Set::key_type> & keys, int iters,
                        const char * label) {
    using K = typename Set::key_type;
    {
        std::mt19937 rng(SEED);
        std::uniform_real_distribution<float> p01(0.0f, 1.0f);
        Set check;
        std::vector<K> live; live.reserve(keys.size());
        for (const auto & k : keys) {
            check.insert(k);
            live.push_back(k);
            if (p01(rng) < 0.1f && !live.empty()) {
                size_t victim = std::uniform_int_distribution<size_t>(0, live.size() - 1)(rng);
                check.erase(live[victim]);
                live[victim] = live.back();
                live.pop_back();
            }
        }
        if (check.size() != live.size()) {
            std::fprintf(stderr, "FAIL [%s churn set]: size %zu != live %zu\n",
                         label, check.size(), live.size());
            std::abort();
        }
    }
    using clk = std::chrono::steady_clock;
    auto t0 = clk::now();
    size_t check_sum = 0;
    for (int i = 0; i < iters; ++i) {
        std::mt19937 rng(SEED + uint32_t(i));
        std::uniform_real_distribution<float> p01(0.0f, 1.0f);
        Set s;
        std::vector<K> live; live.reserve(keys.size());
        for (const auto & k : keys) {
            s.insert(k);
            live.push_back(k);
            if (p01(rng) < 0.1f && !live.empty()) {
                size_t victim = std::uniform_int_distribution<size_t>(0, live.size() - 1)(rng);
                s.erase(live[victim]);
                live[victim] = live.back();
                live.pop_back();
            }
        }
        check_sum ^= s.size();
    }
    auto t1 = clk::now();
    g_sink ^= check_sum;
    return double(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()) / iters;
}

template <typename Set>
double bench_find_set (const std::vector<typename Set::key_type> & keys,
                       const std::vector<typename Set::key_type> & queries,
                       int iters, const char * label) {
    Set s;
    for (const auto & k : keys) s.insert(k);
    if (s.size() != keys.size()) {
        std::fprintf(stderr, "FAIL [%s find set pre-build]: size %zu != expected %zu\n",
                     label, s.size(), keys.size());
        std::abort();
    }
    {
        size_t hits = 0;
        for (const auto & q : queries) hits += (s.find(q) != s.end()) ? 1 : 0;
        if (hits != queries.size()) {
            std::fprintf(stderr, "FAIL [%s find set verify]: hits %zu != queries %zu\n",
                         label, hits, queries.size());
            std::abort();
        }
    }
    using clk = std::chrono::steady_clock;
    auto t0 = clk::now();
    size_t hits = 0;
    for (int i = 0; i < iters; ++i) {
        for (const auto & q : queries) hits += (s.find(q) != s.end()) ? 1 : 0;
    }
    auto t1 = clk::now();
    g_sink ^= hits;
    return double(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()) / iters;
}

// ===== Probe measurement (avg key-equality calls per find) =====
struct ProbeStats {
    double avg_per_find;   // total eq calls / queries.size()
    size_t max_per_find;   // worst-case probe in this run
    double load_factor;    // size / bucket_count
};

template <typename Map>
ProbeStats measure_probe_map (const std::vector<typename Map::key_type> & keys,
                              const std::vector<typename Map::key_type> & queries) {
    Map m;
    for (const auto & k : keys) m.emplace(k, (void*)nullptr);
    g_eq_calls = 0; g_eq_max_per_find = 0; g_eq_cur = 0;
    size_t hits = 0;
    for (const auto & q : queries) {
        start_find();
        hits += (m.find(q) != m.end()) ? 1 : 0;
        end_find();
    }
    g_sink ^= hits;
    return { double(g_eq_calls) / double(queries.size()),
             g_eq_max_per_find,
             m.bucket_count() ? double(m.size()) / double(m.bucket_count()) : 0.0 };
}

template <typename Set>
ProbeStats measure_probe_set (const std::vector<typename Set::key_type> & keys,
                              const std::vector<typename Set::key_type> & queries) {
    Set s;
    for (const auto & k : keys) s.insert(k);
    g_eq_calls = 0; g_eq_max_per_find = 0; g_eq_cur = 0;
    size_t hits = 0;
    for (const auto & q : queries) {
        start_find();
        hits += (s.find(q) != s.end()) ? 1 : 0;
        end_find();
    }
    g_sink ^= hits;
    return { double(g_eq_calls) / double(queries.size()),
             g_eq_max_per_find,
             s.bucket_count() ? double(s.size()) / double(s.bucket_count()) : 0.0 };
}

// ===== Table emit =====
void print_table_header (const char * title) {
    std::printf("\n## %s\n\n", title);
    std::printf("| N      | op           |    std (ns) |    das (ns) |   absl (ns) |  das/std |  absl/std |  das/absl |\n");
    std::printf("|--------|--------------|------------:|------------:|------------:|---------:|----------:|----------:|\n");
}
void print_row (size_t n, const char * op, double s, double d, double a) {
    std::printf("| %-6zu | %-12s | %11.0f | %11.0f | %11.0f | %8.2f | %9.2f | %9.2f |\n",
                n, op, s, d, a, d / s, a / s, d / a);
}
void print_probe_header (const char * title) {
    std::printf("\n### %s — probe stats (find on built table, all hits)\n\n", title);
    std::printf("| N      |  std cmp/find | std max | std lf | das cmp/find | das max | das lf | absl cmp/find | absl max | absl lf |\n");
    std::printf("|--------|--------------:|--------:|-------:|-------------:|--------:|-------:|--------------:|---------:|--------:|\n");
}
void print_probe_row (size_t n, ProbeStats s, ProbeStats d, ProbeStats a) {
    std::printf("| %-6zu | %13.3f | %7zu | %6.3f | %12.3f | %7zu | %6.3f | %13.3f | %8zu | %7.3f |\n",
                n,
                s.avg_per_find, s.max_per_find, s.load_factor,
                d.avg_per_find, d.max_per_find, d.load_factor,
                a.avg_per_find, a.max_per_find, a.load_factor);
}

// ===== Per-(shape × key) runners =====
template <typename K>
void run_map_table (const char * title, const std::vector<K> & full_keys) {
    using T  = MapTypes<K>;
    using CT = CountingMapTypes<K>;
    print_table_header(title);
    std::vector<std::tuple<size_t, ProbeStats, ProbeStats, ProbeStats>> probes;
    for (size_t n : SIZES) {
        if (n > full_keys.size()) continue;
        std::vector<K> keys(full_keys.begin(), full_keys.begin() + n);
        const int it_b = pick_iters_build(n);
        const int it_f = pick_iters_find(n);
        auto queries = make_find_queries(keys, SEED + uint32_t(n));

        double s_ins = bench_insert_map<typename T::StdM> (keys, it_b, "std");
        double d_ins = bench_insert_map<typename T::DasM> (keys, it_b, "das");
        double a_ins = bench_insert_map<typename T::AbslM>(keys, it_b, "absl");
        print_row(n, "insert", s_ins, d_ins, a_ins);

        double s_chu = bench_churn_map<typename T::StdM> (keys, it_b, "std");
        double d_chu = bench_churn_map<typename T::DasM> (keys, it_b, "das");
        double a_chu = bench_churn_map<typename T::AbslM>(keys, it_b, "absl");
        print_row(n, "churn", s_chu, d_chu, a_chu);

        double s_fnd = bench_find_map<typename T::StdM> (keys, queries, it_f, "std");
        double d_fnd = bench_find_map<typename T::DasM> (keys, queries, it_f, "das");
        double a_fnd = bench_find_map<typename T::AbslM>(keys, queries, it_f, "absl");
        print_row(n, "find (10xN)", s_fnd, d_fnd, a_fnd);

        // Probe stats via instrumented eq predicates (counting variants).
        auto sp = measure_probe_map<typename CT::StdM> (keys, queries);
        auto dp = measure_probe_map<typename CT::DasM> (keys, queries);
        auto ap = measure_probe_map<typename CT::AbslM>(keys, queries);
        probes.emplace_back(n, sp, dp, ap);

        std::fflush(stdout);
    }
    print_probe_header(title);
    for (auto & p : probes) {
        print_probe_row(std::get<0>(p), std::get<1>(p), std::get<2>(p), std::get<3>(p));
    }
    std::fflush(stdout);
}

template <typename K>
void run_set_table (const char * title, const std::vector<K> & full_keys) {
    using T  = SetTypes<K>;
    using CT = CountingSetTypes<K>;
    print_table_header(title);
    std::vector<std::tuple<size_t, ProbeStats, ProbeStats, ProbeStats>> probes;
    for (size_t n : SIZES) {
        if (n > full_keys.size()) continue;
        std::vector<K> keys(full_keys.begin(), full_keys.begin() + n);
        const int it_b = pick_iters_build(n);
        const int it_f = pick_iters_find(n);
        auto queries = make_find_queries(keys, SEED + uint32_t(n));

        double s_ins = bench_insert_set<typename T::StdS> (keys, it_b, "std");
        double d_ins = bench_insert_set<typename T::DasS> (keys, it_b, "das");
        double a_ins = bench_insert_set<typename T::AbslS>(keys, it_b, "absl");
        print_row(n, "insert", s_ins, d_ins, a_ins);

        double s_chu = bench_churn_set<typename T::StdS> (keys, it_b, "std");
        double d_chu = bench_churn_set<typename T::DasS> (keys, it_b, "das");
        double a_chu = bench_churn_set<typename T::AbslS>(keys, it_b, "absl");
        print_row(n, "churn", s_chu, d_chu, a_chu);

        double s_fnd = bench_find_set<typename T::StdS> (keys, queries, it_f, "std");
        double d_fnd = bench_find_set<typename T::DasS> (keys, queries, it_f, "das");
        double a_fnd = bench_find_set<typename T::AbslS>(keys, queries, it_f, "absl");
        print_row(n, "find (10xN)", s_fnd, d_fnd, a_fnd);

        auto sp = measure_probe_set<typename CT::StdS> (keys, queries);
        auto dp = measure_probe_set<typename CT::DasS> (keys, queries);
        auto ap = measure_probe_set<typename CT::AbslS>(keys, queries);
        probes.emplace_back(n, sp, dp, ap);

        std::fflush(stdout);
    }
    print_probe_header(title);
    for (auto & p : probes) {
        print_probe_row(std::get<0>(p), std::get<1>(p), std::get<2>(p), std::get<3>(p));
    }
    std::fflush(stdout);
}

} // anonymous

int main () {
    std::printf("# Hash table bench: std::unordered_* vs das::daslang_hash_* vs absl::flat_hash_*\n\n");
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
    std::printf("\nWorkloads — per row, all three impls run the SAME workload on the SAME N keys.\n");
    std::printf("Times are nanoseconds per iteration (whole-phase wall time for one full pass).\n");
    std::printf("Ratios: das/std, absl/std, das/absl (lower = faster).\n");
    std::printf("\nNote on K=const char*: all three impls forced to a content-hashing CStrHash\n");
    std::printf("so per-op work is identical; only table mechanics differ.\n");
    std::printf("\nProbe-stats tables (one per shape x key) report avg key-equality calls per find,\n");
    std::printf("worst-case (max), and post-build load factor. ~1.0 cmp/find = clean lookups;\n");
    std::printf("higher means the impl waded through hash collisions / chain entries before the hit.\n");

    auto str_corpus   = make_string_corpus(MAX_N, SEED);
    auto ptr_pool_32  = make_ptr_pool(MAX_N, 32);
    auto ptr_pool_128 = make_ptr_pool(MAX_N, 128);

    run_map_table<const char*>("Map<const char*, void*>",  str_corpus.cstrs);
    run_map_table<std::string>("Map<std::string, void*>",  str_corpus.strs);
    run_map_table<uint64_t>   ("Map<uint64_t, void*>",     str_corpus.hashes);
    run_map_table<void*>      ("Map<void* (32B alloc), void*>",  ptr_pool_32.ptrs);
    run_map_table<void*>      ("Map<void* (128B alloc), void*>", ptr_pool_128.ptrs);

    run_set_table<const char*>("Set<const char*>",         str_corpus.cstrs);
    run_set_table<std::string>("Set<std::string>",         str_corpus.strs);
    run_set_table<uint64_t>   ("Set<uint64_t>",            str_corpus.hashes);
    run_set_table<void*>      ("Set<void* (32B alloc)>",   ptr_pool_32.ptrs);
    run_set_table<void*>      ("Set<void* (128B alloc)>",  ptr_pool_128.ptrs);

    // Force a use of g_sink so the optimizer keeps everything.
    std::printf("\n(checksum: 0x%zx)\n", size_t(g_sink));
    return 0;
}
