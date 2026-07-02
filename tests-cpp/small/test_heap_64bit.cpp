// Phase 1 (daslang 64-bit-arrays project) — verifies the heap/MemoryModel
// layer's uint32 → uint64 widening:
//
//   - `das_context_allocate_i64` / `_reallocate_i64` / `_free_i64` — new
//     entries that take uint64_t byte counts.
//   - `das_context_allocate` / `_reallocate` / `_free` — legacy uint32_t
//     entries still work after widening (they zero-extend into the same
//     internal allocator).
//   - Realistic >4GB allocation is gated on `DASLANG_HUGE_HEAP_TESTS=1`
//     (default off — most CI runners can't spare the RAM).

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/daScriptC.h"
#include "daScript/misc/memory_model.h"      // LinearChunkAllocator
#include "daScript/simulate/aot_builtin.h"   // heap_bytes_allocated

#include <cstdlib>
#include <cstring>

using namespace das;

namespace {

struct InlineCtx {
    das_context      * ctx          = nullptr;
    das_program      * program      = nullptr;
    das_file_access  * file_access  = nullptr;
    das_module_group * module_group = nullptr;
    das_text_writer  * tout         = nullptr;
};

InlineCtx compile_inline(const char * src) {
    InlineCtx ic;
    ic.tout         = das_text_make_printer();
    ic.module_group = das_modulegroup_make();
    ic.file_access  = das_fileaccess_make_default();
    das_fileaccess_introduce_file(ic.file_access, "main.das", src, 1);
    ic.program = das_program_compile((char*)"main.das", ic.file_access, ic.tout, ic.module_group);
    if ( das_program_err_count(ic.program) ) return ic;
    ic.ctx = das_context_make(das_program_context_stack_size(ic.program));
    if ( !das_program_simulate(ic.program, ic.ctx, ic.tout) ) {
        das_context_release(ic.ctx);
        ic.ctx = nullptr;
    }
    return ic;
}

void cleanup_inline_ctx(InlineCtx & ic) {
    if ( ic.ctx )          das_context_release(ic.ctx);
    if ( ic.program )      das_program_release(ic.program);
    if ( ic.file_access )  das_fileaccess_release(ic.file_access);
    if ( ic.module_group ) das_modulegroup_release(ic.module_group);
    if ( ic.tout )         das_text_release(ic.tout);
    ic = InlineCtx{};
}

} // anonymous namespace

TEST_CASE("context heap _i64 API: allocate / reallocate / free round-trip") {
    static const char * SRC = "options gen2\n[export] def main {}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    SUBCASE("allocate_i64 returns non-null and is freeable") {
        void * p = das_context_allocate_i64(ic.ctx, 64);
        REQUIRE(p != nullptr);
        memset(p, 0xAB, 64);
        das_context_free_i64(ic.ctx, p, 64);
    }

    SUBCASE("allocate_i64(0) returns null and free_i64(null) is a no-op") {
        CHECK(das_context_allocate_i64(ic.ctx, 0) == nullptr);
        das_context_free_i64(ic.ctx, nullptr, 0);
    }

    SUBCASE("reallocate_i64 preserves prefix bytes") {
        void * p = das_context_allocate_i64(ic.ctx, 16);
        REQUIRE(p != nullptr);
        for ( int i=0; i<16; i++ ) ((unsigned char*)p)[i] = (unsigned char)i;
        void * q = das_context_reallocate_i64(ic.ctx, p, 16, 64);
        REQUIRE(q != nullptr);
        for ( int i=0; i<16; i++ ) {
            CHECK(((unsigned char*)q)[i] == (unsigned char)i);
        }
        das_context_free_i64(ic.ctx, q, 64);
    }

    SUBCASE("reallocate_i64 to zero frees the block and returns NULL") {
        void * p = das_context_allocate_i64(ic.ctx, 32);
        REQUIRE(p != nullptr);
        void * q = das_context_reallocate_i64(ic.ctx, p, 32, 0);
        CHECK(q == nullptr);
        CHECK(das_context_reallocate_i64(ic.ctx, nullptr, 0, 0) == nullptr);
    }

    cleanup_inline_ctx(ic);
}

TEST_CASE("legacy uint32_t C-API still works after heap widening") {
    // After Phase 1 widens the internal allocator to uint64_t, the legacy
    // uint32_t entry points must still work — they zero-extend their size
    // argument and forward to the same allocator. This guards against an
    // accidental ABI break for embedders that still link against the old API.
    static const char * SRC = "options gen2\n[export] def main {}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    void * p = das_context_allocate(ic.ctx, 256);
    REQUIRE(p != nullptr);
    memset(p, 0x5A, 256);
    void * q = das_context_reallocate(ic.ctx, p, 256, 1024);
    REQUIRE(q != nullptr);
    for ( int i=0; i<256; i++ ) CHECK(((unsigned char*)q)[i] == 0x5A);
    das_context_free(ic.ctx, q, 1024);

    cleanup_inline_ctx(ic);
}

TEST_CASE("alignMask uint32 truncation guard: 4 GB allocation reports correct bytesAllocated (gated)") {
    // Repro for the alignMask uint32_t truncation in MemoryModel::allocate
    // (`size = (size + alignMask) & ~alignMask` — `~alignMask` is uint32 and
    // zero-extends to 0x00000000FFFFFFF0 when ANDed with uint64 size). Sizes
    // ≥ 4 GB lose their high 32 bits, the function takes the shoe path with
    // size=0, computes `si = (0>>4) - 1 = 0xFFFFFFFF`, and dereferences
    // `chunks[0xFFFFFFFF]` — a wild-address read that crashes the process.
    //
    // On master with the bug: das_context_allocate_i64(ctx, 4GB) crashes
    // inside MemoryModel::allocate via shoe.chunks OOB.
    // After widening alignMask to uint64_t: the AND no longer truncates,
    // the allocation lands in the bigStuff path, and bytesAllocated() grows
    // by ≥ 4 GB. The CHECK below catches a regression where the mask flips
    // back to uint32 (bytesAllocated grows by ~16 instead of ≥ 4 GB).
    if constexpr ( sizeof(void*) < 8 ) {
        WARN("DASLANG_HUGE_HEAP_TESTS: 32-bit build, skipping");
        return;
    }
    const char * env = getenv("DASLANG_HUGE_HEAP_TESTS");
    if ( !env || env[0] != '1' ) {
        WARN("DASLANG_HUGE_HEAP_TESTS=1 not set, skipping 4 GB alignMask probe");
        return;
    }

    // persistent_heap routes through PersistentHeapAllocator (MemoryModel/bigStuff) —
    // the path this alignMask probe targets. (The default LinearHeapAllocator serves
    // >4GB too since HeapChunk went 64-bit; that path has its own test below.)
    static const char * SRC =
        "options gen2\n"
        "options persistent_heap = true\n"
        "[export] def main {}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    const uint64_t HUGE_BYTES = uint64_t(4) * 1024 * 1024 * 1024;  // exactly 4 GB
    const uint64_t before = heap_bytes_allocated(reinterpret_cast<Context*>(ic.ctx));
    void * p = das_context_allocate_i64(ic.ctx, HUGE_BYTES);
    REQUIRE(p != nullptr);
    const uint64_t after = heap_bytes_allocated(reinterpret_cast<Context*>(ic.ctx));
    CHECK(after - before >= HUGE_BYTES);
    das_context_free_i64(ic.ctx, p, HUGE_BYTES);

    cleanup_inline_ctx(ic);
}

TEST_CASE("uint64 size accepts values larger than UINT32_MAX (gated)") {
    // Only runs when DASLANG_HUGE_HEAP_TESTS=1 — a 5GB allocation isn't free
    // even on big runners. Compile-time disabled on 32-bit builds where
    // size_t can't hold the value.
    if constexpr ( sizeof(void*) < 8 ) {
        WARN("DASLANG_HUGE_HEAP_TESTS: 32-bit build, skipping");
        return;
    }
    const char * env = getenv("DASLANG_HUGE_HEAP_TESTS");
    if ( !env || env[0] != '1' ) {
        WARN("DASLANG_HUGE_HEAP_TESTS=1 not set, skipping 5GB alloc");
        return;
    }

    // persistent_heap: exercises the PersistentHeapAllocator / MemoryModel::bigStuff
    // path specifically (the default linear heap's >4GB path is tested below).
    static const char * SRC =
        "options gen2\n"
        "options persistent_heap = true\n"
        "[export] def main {}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    // 5 GB — exceeds the old uint32_t cap (~4GB). Should succeed on a
    // RAM-rich runner; that's the whole point of the widening.
    const uint64_t HUGE_BYTES = uint64_t(5) * 1024 * 1024 * 1024;
    void * p = das_context_allocate_i64(ic.ctx, HUGE_BYTES);
    REQUIRE(p != nullptr);
    // Touch the first and last byte so a silent truncation to <4GB shows up
    // as a segfault rather than a passing test.
    ((unsigned char*)p)[0] = 0xC0;
    ((unsigned char*)p)[HUGE_BYTES - 1] = 0xDE;
    CHECK(((unsigned char*)p)[0] == 0xC0);
    CHECK(((unsigned char*)p)[HUGE_BYTES - 1] == 0xDE);
    das_context_free_i64(ic.ctx, p, HUGE_BYTES);

    cleanup_inline_ctx(ic);
}

TEST_CASE("LinearChunkAllocator accepts a >4GB initial size") {
    // No allocation happens until the first allocate(), so this probe is cheap
    // enough to run ungated everywhere. Before HeapChunk went 64-bit,
    // setInitialSize rejected anything over UINT32_MAX with a DAS_VERIFYF.
    if constexpr ( sizeof(void*) < 8 ) {
        WARN("32-bit build, skipping >4GB initial-size probe");
        return;
    }
    LinearChunkAllocator alloc;
    const uint64_t HUGE_BYTES = uint64_t(6) * 1024 * 1024 * 1024;
    alloc.setInitialSize(HUGE_BYTES);
    CHECK_EQ(alloc.initialSize, HUGE_BYTES);
}

TEST_CASE("LinearChunkAllocator serves a >4GB single chunk (gated)") {
    // The wave-6 lift: HeapChunk size/offset are uint64, so one linear chunk can
    // exceed 4GB. Only the first and last bytes are touched — lazy commit keeps
    // resident memory tiny — but the address-space/commit charge is still ~4GB,
    // hence the gate.
    if constexpr ( sizeof(void*) < 8 ) {
        WARN("DASLANG_HUGE_HEAP_TESTS: 32-bit build, skipping");
        return;
    }
    const char * env = getenv("DASLANG_HUGE_HEAP_TESTS");
    if ( !env || env[0] != '1' ) {
        WARN("DASLANG_HUGE_HEAP_TESTS=1 not set, skipping >4GB linear-chunk test");
        return;
    }

    LinearChunkAllocator alloc;
    const uint64_t HUGE_BYTES = (uint64_t(1) << 32) + 64;   // 4GB + 64
    char * p = alloc.allocate(HUGE_BYTES);
    REQUIRE(p != nullptr);
    p[0] = char(0xC0);
    p[HUGE_BYTES - 1] = char(0xDE);
    CHECK_EQ(p[0], char(0xC0));
    CHECK_EQ(p[HUGE_BYTES - 1], char(0xDE));
    CHECK(alloc.isOwnPtr(p));
    CHECK(alloc.isOwnPtr(p + HUGE_BYTES - 1));
    CHECK_GE(alloc.bytesAllocated(), HUGE_BYTES);
    // Tail free rolls the linear offset back — the whole chunk reads as empty again.
    alloc.free(p, HUGE_BYTES);
    CHECK_EQ(alloc.bytesAllocated(), uint64_t(0));
    alloc.reset();
}

TEST_CASE("default (linear) context heap serves >4GB allocations (gated)") {
    // The das-visible half of the lift: no `options persistent_heap` — the default
    // LinearHeapAllocator path itself serves >4GB (this is what forced persistent_heap
    // onto every dasLLAMA model-loading test before wave 6).
    if constexpr ( sizeof(void*) < 8 ) {
        WARN("DASLANG_HUGE_HEAP_TESTS: 32-bit build, skipping");
        return;
    }
    const char * env = getenv("DASLANG_HUGE_HEAP_TESTS");
    if ( !env || env[0] != '1' ) {
        WARN("DASLANG_HUGE_HEAP_TESTS=1 not set, skipping >4GB linear-heap test");
        return;
    }

    static const char * SRC = "options gen2\n[export] def main {}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    const uint64_t HUGE_BYTES = uint64_t(5) * 1024 * 1024 * 1024;
    const uint64_t before = heap_bytes_allocated(reinterpret_cast<Context*>(ic.ctx));
    void * p = das_context_allocate_i64(ic.ctx, HUGE_BYTES);
    REQUIRE(p != nullptr);
    ((unsigned char*)p)[0] = 0xC0;
    ((unsigned char*)p)[HUGE_BYTES - 1] = 0xDE;
    CHECK(((unsigned char*)p)[0] == 0xC0);
    CHECK(((unsigned char*)p)[HUGE_BYTES - 1] == 0xDE);
    const uint64_t after = heap_bytes_allocated(reinterpret_cast<Context*>(ic.ctx));
    CHECK(after - before >= HUGE_BYTES);
    das_context_free_i64(ic.ctx, p, HUGE_BYTES);

    cleanup_inline_ctx(ic);
}
