#include "daScript/misc/platform.h"

#if DAS_TRACK_ALLOC

#include <atomic>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <mutex>
#include <new>
#include <chrono>

#if defined(_MSC_VER)
    #include <windows.h>
    #include <dbghelp.h>
    #pragma comment(lib, "dbghelp.lib")
#elif defined(__linux__) || defined(__APPLE__)
    #include <execinfo.h>
    #include <cxxabi.h>
    #include <dlfcn.h>
#endif

// init_seg(lib) puts this TU's file-scope dynamic initializers in the "library"
// group, which runs before any init_seg(user) — i.e., before all user-level C++
// static constructors. Our registration struct (below) calls std::atexit() from
// its constructor, so its handler lands at the bottom of the atexit LIFO stack.
// Subsequent user static destructors register via __cxa_atexit and stack on top.
// Result: our dump fires AFTER every user-level static destructor has run, so
// process-lifetime statics (Meyers singletons, static vectors, etc.) no longer
// appear as leaks.
#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable: 4073)  // initializers put in library initialization area
    #pragma init_seg(lib)
    #pragma warning(pop)
#endif

namespace das {

static constexpr int kFrames = 16;

struct AllocInfo {
    size_t size;
    int    frameCount;
    void * frames[kFrames];
};

struct Entry {
    void *      key;        // nullptr = empty, kTombstone = deleted
    AllocInfo   info;
};

static void * const kTombstone = reinterpret_cast<void*>(uintptr_t(-1));

// Hand-rolled open-addressing hash map keyed by void*. Uses std::malloc/std::free
// directly so its own storage never re-enters our hooks.
struct LeakMap {
    Entry *  entries   = nullptr;
    size_t   capacity  = 0;
    size_t   live      = 0;     // real entries
    size_t   filled    = 0;     // real + tombstones (for rehash threshold)

    void init() {
        capacity = 4096;
        entries = static_cast<Entry*>(std::calloc(capacity, sizeof(Entry)));
    }

    static size_t mix(void *p) {
        uintptr_t x = reinterpret_cast<uintptr_t>(p);
        x ^= x >> 33;
        x *= 0xff51afd7ed558ccdULL;
        x ^= x >> 33;
        x *= 0xc4ceb9fe1a85ec53ULL;
        x ^= x >> 33;
        return size_t(x);
    }

    size_t find(void *key) const {
        size_t mask = capacity - 1;
        size_t i = mix(key) & mask;
        size_t first_tomb = SIZE_MAX;
        while (true) {
            void *k = entries[i].key;
            if (k == nullptr) return (first_tomb != SIZE_MAX) ? first_tomb : i;
            if (k == key)     return i;
            if (k == kTombstone && first_tomb == SIZE_MAX) first_tomb = i;
            i = (i + 1) & mask;
        }
    }

    void rehash(size_t new_cap) {
        Entry *old_entries = entries;
        size_t old_cap = capacity;
        entries = static_cast<Entry*>(std::calloc(new_cap, sizeof(Entry)));
        capacity = new_cap;
        size_t mask = new_cap - 1;
        for (size_t j = 0; j < old_cap; ++j) {
            void *k = old_entries[j].key;
            if (k == nullptr || k == kTombstone) continue;
            size_t i = mix(k) & mask;
            while (entries[i].key != nullptr) i = (i + 1) & mask;
            entries[i] = old_entries[j];
        }
        std::free(old_entries);
        filled = live;
    }

    void insert(void *key, const AllocInfo &info) {
        if (!entries) init();
        if ((filled + 1) * 4 >= capacity * 3) rehash(capacity * 2);
        size_t i = find(key);
        void *k = entries[i].key;
        entries[i].key  = key;
        entries[i].info = info;
        if (k == nullptr) { ++filled; ++live; }
        else if (k == kTombstone) { ++live; }
        // else: replacement of existing key, counts unchanged
    }

    void erase(void *key) {
        if (!entries || !live) return;
        size_t i = find(key);
        if (entries[i].key == key) {
            entries[i].key = kTombstone;
            --live;
        }
    }
};

// Leaked Meyers singleton: the map is never destroyed, so track_free after static
// destructors is safe.
static LeakMap & getMap() {
    alignas(LeakMap) static unsigned char storage[sizeof(LeakMap)];
    static LeakMap *m = ::new (storage) LeakMap();
    return *m;
}

static std::mutex & getMutex() {
    alignas(std::mutex) static unsigned char storage[sizeof(std::mutex)];
    static std::mutex *m = ::new (storage) std::mutex();
    return *m;
}

static std::atomic<bool>     g_armed{false};
static std::atomic<uint64_t> g_orphan_free{0}; // free() of ptr not in map — cross-module guard
static thread_local bool     tl_inside = false;

struct ReentryGuard {
    bool prev;
    ReentryGuard() : prev(tl_inside) { tl_inside = true; }
    ~ReentryGuard() { tl_inside = prev; }
};

#if defined(_MSC_VER) && defined(_M_X64)
// Implemented in alloc_tracker_fast_stack.cpp — cached .pdata + lightweight
// unwinder + landmark tail-memoize. Drop-in replacement for CaptureStackBackTrace.
// skipFrames drops the N deepest frames; default 2 hides das_fast_stack_capture
// itself and the tracker hook that called it.
unsigned das_fast_stack_capture(void **stack, unsigned maxFrames, int skipFrames = 2) noexcept;
#endif

static int capture_stack(void **frames, int max_frames) noexcept {
#if defined(_MSC_VER) && defined(_M_X64)
    // skipFrames=2: drop das_fast_stack_capture + track_alloc_hook so the
    // visible top frame is the caller of the tracker (operator new wrapper,
    // das_aligned_alloc16, etc.).
    return (int)das_fast_stack_capture(frames, (unsigned)max_frames, 2);
#elif defined(_MSC_VER)
    // FramesToSkip=1: drop the noinline track_alloc_hook itself.
    return (int)CaptureStackBackTrace(1, (DWORD)max_frames, frames, nullptr);
#elif defined(__linux__) || defined(__APPLE__)
    // Drop backtrace()'s own frame and track_alloc_hook.
    void * raw[64];
    int n = max_frames + 2 > 64 ? 64 : max_frames + 2;
    int got = backtrace(raw, n);
    int skip = got > 2 ? 2 : got;
    int out = got - skip;
    if (out > max_frames) out = max_frames;
    for (int i = 0; i < out; ++i) frames[i] = raw[skip + i];
    return out;
#else
    (void)frames; (void)max_frames;
    return 0;
#endif
}

// noinline so skipFrames=2 in capture_stack (via das_fast_stack_capture) reliably
// drops this frame regardless of /Ob level.
#if defined(_MSC_VER)
__declspec(noinline)
#else
__attribute__((noinline))
#endif
void track_alloc_hook(void *p, size_t sz) noexcept {
    if (!g_armed.load(std::memory_order_relaxed)) return;
    if (!p || tl_inside) return;
    ReentryGuard g;
    AllocInfo info;
    info.size = sz;
    info.frameCount = capture_stack(info.frames, kFrames);
    std::lock_guard<std::mutex> lock(getMutex());
    getMap().insert(p, info);
}

void track_free_hook(void *p) noexcept {
    if (!g_armed.load(std::memory_order_relaxed)) return;
    if (!p || tl_inside) return;
    ReentryGuard g;
    std::lock_guard<std::mutex> lock(getMutex());
    LeakMap &m = getMap();
    if (m.entries && m.live) {
        size_t i = m.find(p);
        if (m.entries[i].key == p) {
            m.entries[i].key = kTombstone;
            --m.live;
            return;
        }
    }
    // Pointer not in our map. Either allocated pre-arm, or freed by a module that
    // doesn't share our override (cross-DLL). Counted for diagnostics.
    g_orphan_free.fetch_add(1, std::memory_order_relaxed);
}

void arm_alloc_tracking() noexcept {
    g_armed.store(true, std::memory_order_release);
}

// ------------------------- Symbolization -------------------------

#if defined(_MSC_VER)
static bool g_symInitialized = false;

static void init_symbols() {
    if (g_symInitialized) return;
    // FAIL_CRITICAL_ERRORS + NO_PROMPTS: never pop message boxes from DBGHELP.
    // OMAP_FIND_NEAREST: handle BBT-optimized binaries correctly.
    // fInvadeProcess=TRUE enumerates all loaded modules at init; combined with
    // DEFERRED_LOADS, PDBs are mapped on first SymFromAddr for each module.
    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS |
                  SYMOPT_OMAP_FIND_NEAREST | SYMOPT_FAIL_CRITICAL_ERRORS |
                  SYMOPT_NO_PROMPTS | SYMOPT_INCLUDE_32BIT_MODULES);
    if (SymInitialize(GetCurrentProcess(), nullptr, TRUE)) {
        g_symInitialized = true;
        // Refresh catches any modules loaded after init (late-bound DLLs).
        SymRefreshModuleList(GetCurrentProcess());
    }
}

// disp larger than this means SymFromAddr fell back to a distant symbol;
// suppress the name and show module+offset instead. 64KB is generous for a
// single C++ function; typical codegen functions are under 4KB.
static constexpr DWORD64 kMaxTrustedSymbolOffset = 0x10000;

static void print_module_fallback(FILE *out, HANDLE proc, void *addr, DWORD64 addr64) {
    IMAGEHLP_MODULE64 modInfo;
    memset(&modInfo, 0, sizeof(modInfo));
    modInfo.SizeOfStruct = sizeof(modInfo);
    if (SymGetModuleInfo64(proc, addr64, &modInfo)) {
        DWORD64 moduleOffset = addr64 - modInfo.BaseOfImage;
        fprintf(out, "    %p  %s+0x%llx\n",
                addr, modInfo.ModuleName, (unsigned long long)moduleOffset);
    } else {
        fprintf(out, "    %p  ?\n", addr);
    }
}

static void print_frame(FILE *out, void *addr) {
    HANDLE proc = GetCurrentProcess();
    DWORD64 addr64 = (DWORD64)addr;

    char symbuf[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char)];
    SYMBOL_INFO *sym = (SYMBOL_INFO*)symbuf;
    sym->SizeOfStruct = sizeof(SYMBOL_INFO);
    sym->MaxNameLen   = MAX_SYM_NAME;

    DWORD64 disp = 0;
    bool haveSym = SymFromAddr(proc, addr64, &disp, sym) != FALSE;
    if (!haveSym || disp >= kMaxTrustedSymbolOffset) {
        // SymFromAddr failed, or matched a symbol so far away it's almost
        // certainly a misattribution. Show module+offset instead — more
        // useful than `transform_syntax+0x747d`.
        print_module_fallback(out, proc, addr, addr64);
        return;
    }

    const char *name = sym->Name;
    IMAGEHLP_LINE64 line;
    memset(&line, 0, sizeof(line));
    line.SizeOfStruct = sizeof(line);
    DWORD lineDisp = 0;
    if (SymGetLineFromAddr64(proc, addr64, &lineDisp, &line)) {
        fprintf(out, "    %p  %s+0x%llx   %s:%lu\n",
                addr, name, (unsigned long long)disp, line.FileName, (unsigned long)line.LineNumber);
    } else {
        fprintf(out, "    %p  %s+0x%llx\n",
                addr, name, (unsigned long long)disp);
    }
}
#else
static void init_symbols() {}

static void print_frame(FILE *out, void *addr) {
#if defined(__linux__) || defined(__APPLE__)
    Dl_info info;
    if (dladdr(addr, &info) && info.dli_sname) {
        int status = 0;
        char *demangled = abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &status);
        const char *name = (status == 0 && demangled) ? demangled : info.dli_sname;
        uintptr_t offset = (uintptr_t)addr - (uintptr_t)info.dli_saddr;
        fprintf(out, "    %p  %s+0x%lx   (%s)\n",
                addr, name, (unsigned long)offset, info.dli_fname ? info.dli_fname : "?");
        std::free(demangled);
    } else {
        fprintf(out, "    %p  ?\n", addr);
    }
#else
    fprintf(out, "    %p\n", addr);
#endif
}
#endif

// ------------------------- Dump -------------------------

struct Group {
    uint64_t  hash;
    size_t    count;
    size_t    totalBytes;
    size_t    minSize;
    size_t    maxSize;
    void *    samplePtr;
    int       frameCount;
    void *    frames[kFrames];
};

static uint64_t hash_frames(void * const *frames, int n) {
    uint64_t h = 0xcbf29ce484222325ULL;
    for (int i = 0; i < n; ++i) {
        h ^= (uint64_t)(uintptr_t)frames[i];
        h *= 0x100000001b3ULL;
    }
    return h;
}

static void format_with_commas(char *buf, size_t buflen, uint64_t v) {
    char tmp[32];
    int n = snprintf(tmp, sizeof(tmp), "%llu", (unsigned long long)v);
    // insert commas
    int out = 0;
    for (int i = 0; i < n && out < (int)buflen - 1; ++i) {
        int remaining = n - i;
        if (i > 0 && (remaining % 3) == 0) {
            if (out < (int)buflen - 1) buf[out++] = ',';
        }
        buf[out++] = tmp[i];
    }
    buf[out] = 0;
}

size_t dump_alloc_leaks(FILE *out) {
    // Disarm tracking during dump so fprintf/Sym*/etc don't churn the map.
    bool was_armed = g_armed.exchange(false);
    (void)was_armed;
    tl_inside = true;

    auto t0 = std::chrono::steady_clock::now();

    // Snapshot groups under lock so other threads (if any) don't race us.
    // Use std::malloc for snapshot storage — no tracker re-entry.
    Group *groups = nullptr;
    size_t groupCount = 0;
    size_t groupCap   = 0;
    size_t totalLeaks = 0;
    size_t totalBytes = 0;
    size_t largestSingle = 0;
    bool   groupsExhausted = false;

    {
        std::lock_guard<std::mutex> lock(getMutex());
        LeakMap &m = getMap();
        for (size_t i = 0; i < m.capacity; ++i) {
            void *k = m.entries[i].key;
            if (k == nullptr || k == kTombstone) continue;
            const AllocInfo &info = m.entries[i].info;
            ++totalLeaks;
            totalBytes += info.size;
            if (info.size > largestSingle) largestSingle = info.size;

            if (groupsExhausted) continue; // keep counting totals, drop per-site detail

            uint64_t h = hash_frames(info.frames, info.frameCount);

            // linear scan for existing group (small N typical; replace w/ hash later if needed)
            Group *g = nullptr;
            for (size_t j = 0; j < groupCount; ++j) {
                if (groups[j].hash == h) { g = &groups[j]; break; }
            }
            if (!g) {
                if (groupCount == groupCap) {
                    size_t newCap = groupCap ? groupCap * 2 : 64;
                    Group *resized = (Group*)std::realloc(groups, newCap * sizeof(Group));
                    if (!resized) {
                        // OOM during shutdown report: keep the buffer we already have,
                        // tally totals on the remaining entries but stop adding sites.
                        groupsExhausted = true;
                        continue;
                    }
                    groups   = resized;
                    groupCap = newCap;
                }
                g = &groups[groupCount++];
                g->hash       = h;
                g->count      = 0;
                g->totalBytes = 0;
                g->minSize    = SIZE_MAX;
                g->maxSize    = 0;
                g->samplePtr  = k;
                g->frameCount = info.frameCount;
                memcpy(g->frames, info.frames, info.frameCount * sizeof(void*));
            }
            g->count      += 1;
            g->totalBytes += info.size;
            if (info.size < g->minSize) g->minSize = info.size;
            if (info.size > g->maxSize) g->maxSize = info.size;
        }
    }

    // Sort groups: by totalBytes desc, then count desc, then hash for stability.
    // Simple insertion sort — groupCount is typically small.
    for (size_t i = 1; i < groupCount; ++i) {
        Group key = groups[i];
        size_t j = i;
        while (j > 0) {
            const Group &a = groups[j - 1];
            bool before = (a.totalBytes < key.totalBytes) ||
                          (a.totalBytes == key.totalBytes && a.count < key.count) ||
                          (a.totalBytes == key.totalBytes && a.count == key.count && a.hash < key.hash);
            if (!before) break;
            groups[j] = groups[j - 1];
            --j;
        }
        groups[j] = key;
    }

    // No leaks → silent. Skip the entire report (header, body, footer).
    if (totalLeaks == 0) {
        std::free(groups);
        tl_inside = false;
        return 0;
    }

    // Header.
    char b1[32], b2[32], b3[32];
    format_with_commas(b1, sizeof(b1), totalLeaks);
    format_with_commas(b2, sizeof(b2), totalBytes);
    format_with_commas(b3, sizeof(b3), largestSingle);

    fprintf(out, "\n=== daslang C++ heap leak report ===\n");
    fprintf(out, "Total live allocations: %s\n", b1);
    fprintf(out, "Total bytes:            %s\n", b2);
    fprintf(out, "Distinct leak sites:    %zu\n", groupCount);
    fprintf(out, "Largest single alloc:   %s bytes\n", b3);
    uint64_t orphan = g_orphan_free.load(std::memory_order_relaxed);
    if (orphan) {
        fprintf(out, "Orphan frees (pre-arm or cross-module): %llu\n",
                (unsigned long long)orphan);
    }
    if (groupsExhausted) {
        fprintf(out, "WARNING: realloc() failed during grouping; per-site detail truncated.\n"
                     "         Totals above are accurate; only the leak-site breakdown is partial.\n");
    }

    if (groupCount > 0) {
        init_symbols();
        for (size_t i = 0; i < groupCount; ++i) {
            const Group &g = groups[i];
            uint64_t mean = g.count ? g.totalBytes / g.count : 0;
            char bc[32], bt[32], bmn[32], bmx[32], bavg[32];
            format_with_commas(bc,  sizeof(bc),  g.count);
            format_with_commas(bt,  sizeof(bt),  g.totalBytes);
            format_with_commas(bmn, sizeof(bmn), g.minSize);
            format_with_commas(bmx, sizeof(bmx), g.maxSize);
            format_with_commas(bavg,sizeof(bavg),mean);
            fprintf(out, "\n--- Leak site #%zu: %s allocs, %s bytes (min %s, max %s, mean %s) ---\n",
                    i + 1, bc, bt, bmn, bmx, bavg);
            fprintf(out, "  sample ptr: %p\n", g.samplePtr);
            for (int j = 0; j < g.frameCount; ++j) {
                print_frame(out, g.frames[j]);
            }
        }
    }

    auto t1 = std::chrono::steady_clock::now();
    double secs = std::chrono::duration<double>(t1 - t0).count();
    fprintf(out, "\n=== End report (%s leaks, %zu sites, symbolized in %.2fs) ===\n",
            b1, groupCount, secs);
    fflush(out);

    std::free(groups);
    tl_inside = false;
    // intentionally leave disarmed after dump
    return totalLeaks;
}

// ------------------------- Atexit registration -------------------------
//
// With #pragma init_seg(lib) at the top of this TU, the constructor of
// g_register_leak_dump_atexit runs before any init_seg(user) static ctor.
// std::atexit() pushes onto a LIFO stack shared with __cxa_atexit (which
// C++ static destructors use). Because we register first, our handler sits
// at the bottom of the stack and fires LAST — after every user-level
// static destructor. This means static-lifetime allocations (Meyers
// singletons, static vectors, etc.) are freed before we report, and only
// genuine leaks remain in the dump.
//
// Idempotence guard: protects against the explicit dump_alloc_leaks(stderr)
// call from main.cpp or a double-registered atexit.

static std::atomic<bool> g_dumped{false};

// Defined in src/ast/ast_module.cpp. Incremented in Module::Initialize, decremented
// in Module::Shutdown. If nonzero at exit time, the process bypassed cleanup
// (e.g., dastest exits via fio::exit -> C exit() without calling Module::Shutdown),
// so live allocations are not real leaks — they're memory the cleanup never got
// a chance to free. Suppress the dump in that case to avoid noise.
extern std::atomic<int> g_envTotal;

static void dump_alloc_leaks_atexit() {
    if (g_dumped.exchange(true, std::memory_order_acq_rel)) return;
    int pending = g_envTotal.load(std::memory_order_relaxed);
    if (pending > 0) {
        fprintf(stderr,
            "\n=== daslang C++ heap leak report SKIPPED ===\n"
            "Module::Shutdown() was not called (%d environment(s) still active).\n"
            "Likely cause: the process exited via exit()/abort() bypassing cleanup.\n"
            "Live allocations are not reported because they may still be owned.\n",
            pending);
        fflush(stderr);
        return;
    }
    dump_alloc_leaks(stderr);
}

struct RegisterLeakDumpAtExit {
    RegisterLeakDumpAtExit() noexcept { std::atexit(&dump_alloc_leaks_atexit); }
};
static RegisterLeakDumpAtExit g_register_leak_dump_atexit;

} // namespace das

#endif // DAS_TRACK_ALLOC
