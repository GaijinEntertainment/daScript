// PR-H (daslang 64-bit-arrays project) — C-API completeness for arrays / tables.
//
// Phase 1 (#2746) widened das_array.size / .capacity and das_table.size /
// .capacity to uint64_t. The C entry points (das_array_resize / _reserve /
// _at / _init_borrowed, das_table_reserve) still took uint32_t, so embedders
// who needed >4G-element collections had to drop down to the raw runtime
// (array_reserve / table_reserve_impl) which take uint64_t.
//
// This PR adds _i64 siblings that thread uint64_t through the C boundary
// without narrowing. The legacy uint32_t entries remain for callers that
// don't need wide counts.
//
// Tests:
//   - Round-trip on small values — proves the new entries actually call the
//     same runtime helpers and round-trip via the legacy _at / find readers.
//   - >UINT32_MAX gated probes (DASLANG_HUGE_HEAP_TESTS=1, 64-bit only) — the
//     surface that would have silently truncated on the uint32 entries.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/daScriptC.h"
#include "daScript/misc/arraytype.h"

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

bool huge_heap_enabled() {
    if constexpr ( sizeof(void*) < 8 ) return false;
    const char * env = getenv("DASLANG_HUGE_HEAP_TESTS");
    return env && env[0] == '1';
}

} // anonymous namespace

// ---------------------------------------------------------------------------
// das_array _i64 API
// ---------------------------------------------------------------------------

TEST_CASE("das_array _i64 API: resize / reserve / at round-trip on small sizes") {
    static const char * SRC = "options gen2\n[export] def main {}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    das_array a;
    das_array_init(&a);

    SUBCASE("resize_i64 + at_i64 round-trip") {
        das_array_resize_i64(ic.ctx, &a, uint64_t(12), sizeof(int), /*zero=*/1);
        REQUIRE(a.data != nullptr);
        CHECK(a.size == 12u);
        for ( uint64_t i = 0; i < 12; i++ ) {
            *(int*)das_array_at_i64(&a, i, sizeof(int)) = int(i) * 13;
        }
        for ( uint64_t i = 0; i < 12; i++ ) {
            CHECK(*(int*)das_array_at_i64(&a, i, sizeof(int)) == int(i) * 13);
        }
        // Legacy at() still reads the same slots (cross-check).
        for ( uint32_t i = 0; i < 12; i++ ) {
            CHECK(*(int*)das_array_at(&a, i, sizeof(int)) == int(i) * 13);
        }
    }

    SUBCASE("reserve_i64 grows capacity without changing size") {
        das_array_reserve_i64(ic.ctx, &a, uint64_t(64), sizeof(int));
        CHECK(a.size == 0u);
        CHECK(a.capacity >= 64u);
    }

    das_array_clear(ic.ctx, &a, sizeof(int));
    cleanup_inline_ctx(ic);
}

TEST_CASE("das_array_init_borrowed_i64 stores 64-bit count/capacity verbatim") {
    static const char * SRC = "options gen2\n[export] def main {}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    // Small backing buffer is fine -- we're checking the metadata fields
    // get the wide values without truncation. (No daslang code reads beyond
    // index 0, so no out-of-bounds access happens.)
    char fake_buf[16] = {};
    das_array view;
    const uint64_t WIDE_COUNT    = uint64_t(5'000'000'000ULL);
    const uint64_t WIDE_CAPACITY = uint64_t(5'000'000'000ULL);
    das_array_init_borrowed_i64(&view, fake_buf, WIDE_COUNT, WIDE_CAPACITY);
    CHECK(view.size     == WIDE_COUNT);
    CHECK(view.capacity == WIDE_CAPACITY);
    CHECK(view.data     == fake_buf);
    // 'shared' flag matches the legacy entry's behavior.
    CHECK((view.flags & 1u) == 1u);

    cleanup_inline_ctx(ic);
}

TEST_CASE("das_array _i64 API: resize past UINT32_MAX (gated)") {
    // The legacy uint32_t entries would truncate this to 705032704; the _i64
    // entries thread the full uint64_t through to array_resize / array_reserve.
    if ( !huge_heap_enabled() ) {
        WARN("DASLANG_HUGE_HEAP_TESTS=1 not set (or 32-bit build); skipping >UINT32_MAX C-API probe");
        return;
    }

    // PersistentHeapAllocator path -- the default LinearHeapAllocator is
    // uint32-bounded by design.
    static const char * SRC =
        "options gen2\n"
        "options persistent_heap = true\n"
        "[export] def main {}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    das_array a;
    das_array_init(&a);

    // 5 GiB uint8 elements -- comfortably past UINT32_MAX (~4.29 GiB), which
    // is what proves the _i64 entries don't truncate via uint32 narrowing.
    // Stride 1 keeps the byte total equal to the element count.
    const uint64_t HUGE_N = uint64_t(5) * 1024 * 1024 * 1024;  // 5 GiB > UINT32_MAX
    static_assert(uint64_t(5) * 1024 * 1024 * 1024 > uint64_t(UINT32_MAX),
                  "HUGE_N must exceed UINT32_MAX to exercise the _i64 path");
    das_array_resize_i64(ic.ctx, &a, HUGE_N, /*stride=*/1, /*zero=*/0);
    REQUIRE(a.data != nullptr);
    REQUIRE(das_context_get_exception(ic.ctx) == nullptr);
    CHECK(a.size == HUGE_N);
    CHECK(a.capacity >= HUGE_N);

    // Write + read sentinels at indices > UINT32_MAX. Pre-fix the uint32_t entry
    // would have truncated these indices into the low ~4 GiB region and
    // returned a wrong pointer; the _i64 entry must thread the full uint64_t.
    const uint64_t IDX_A = uint64_t(UINT32_MAX) + 17;        // ~4.29 G + 17
    const uint64_t IDX_B = HUGE_N - 2;                       // ~5 G - 2
    const uint64_t IDX_C = HUGE_N - 1;                       // ~5 G - 1
    *(uint8_t*)das_array_at_i64(&a, IDX_A, sizeof(uint8_t)) = 0xAA;
    *(uint8_t*)das_array_at_i64(&a, IDX_B, sizeof(uint8_t)) = 0xBB;
    *(uint8_t*)das_array_at_i64(&a, IDX_C, sizeof(uint8_t)) = 0xCC;
    CHECK(*(uint8_t*)das_array_at_i64(&a, IDX_A, sizeof(uint8_t)) == 0xAA);
    CHECK(*(uint8_t*)das_array_at_i64(&a, IDX_B, sizeof(uint8_t)) == 0xBB);
    CHECK(*(uint8_t*)das_array_at_i64(&a, IDX_C, sizeof(uint8_t)) == 0xCC);

    das_array_clear(ic.ctx, &a, sizeof(uint8_t));
    cleanup_inline_ctx(ic);
}

// ---------------------------------------------------------------------------
// das_table _i64 API
// ---------------------------------------------------------------------------

TEST_CASE("das_table_reserve_i64: round-trip on small capacity") {
    static const char * SRC = "options gen2\n[export] def main {}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    das_table t;
    das_table_init(&t);

    das_table_reserve_i64(ic.ctx, &t, DAS_TYPE_INT, uint64_t(64), /*value_size=*/sizeof(int));
    CHECK(t.size == 0u);
    CHECK(t.capacity >= 64u);

    // Insert via the legacy entry (no _i64 needed -- find/insert/erase use
    // the table's internal hash table, not a wide capacity arg).
    int key = 7;
    int * slot = (int*)das_table_insert(ic.ctx, &t, DAS_TYPE_INT, &key, sizeof(int));
    REQUIRE(slot != nullptr);
    *slot = 42;
    int * found = (int*)das_table_find(ic.ctx, &t, DAS_TYPE_INT, &key, sizeof(int));
    REQUIRE(found != nullptr);
    CHECK(*found == 42);

    das_table_clear(ic.ctx, &t, DAS_TYPE_INT, sizeof(int));
    cleanup_inline_ctx(ic);
}
