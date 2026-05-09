// Test for the C API additions in daScriptC.h:
//   - das_context_allocate / _reallocate / _free
//   - das_array_init / _init_borrowed / _reserve / _resize / _clear / _at /
//                      _lock / _unlock
//   - das_table_init / _reserve / _clear / _find / _insert / _erase /
//                      _lock / _unlock
//   - das_argument_array / _table and das_result_array / _table
//
// Plus a runtime canary on the das_array / das_table layout — duplicates the
// compile-time static_asserts in daScriptC.cpp with a readable failure message.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/daScriptC.h"
#include "daScript/misc/arraytype.h"

#include <cstddef>
#include <cstring>

using namespace das;

namespace {

// Compile + simulate an inline daslang script via the C API. Returns a context
// (caller-owned) plus the program / file_access / module_group it depends on.
// Caller releases all four via cleanup_inline_ctx().
struct InlineCtx {
    das_context     * ctx          = nullptr;
    das_program     * program      = nullptr;
    das_file_access * file_access  = nullptr;
    das_module_group * module_group = nullptr;
    das_text_writer * tout         = nullptr;
};

InlineCtx compile_inline(const char * src) {
    InlineCtx ic;
    ic.tout         = das_text_make_printer();
    ic.module_group = das_modulegroup_make();
    ic.file_access  = das_fileaccess_make_default();
    das_fileaccess_introduce_file(ic.file_access, "main.das", src, 1);
    ic.program = das_program_compile((char*)"main.das", ic.file_access, ic.tout, ic.module_group);
    if ( das_program_err_count(ic.program) ) return ic; // ctx left null
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

TEST_CASE("das_array / das_table layout matches Array / Table at runtime") {
    // Belt-and-braces with the static_asserts in daScriptC.cpp: surface a clear
    // diagnostic if the C struct ever drifts from the C++ runtime.
    CHECK(sizeof(das_array) == sizeof(Array));
    CHECK(offsetof(das_array, data)     == offsetof(Array, data));
    CHECK(offsetof(das_array, size)     == offsetof(Array, size));
    CHECK(offsetof(das_array, capacity) == offsetof(Array, capacity));
    CHECK(offsetof(das_array, flags)    == offsetof(Array, flags));

    CHECK(sizeof(das_table) == sizeof(Table));
    CHECK(offsetof(das_table, data)       == offsetof(Table, data));
    CHECK(offsetof(das_table, size)       == offsetof(Table, size));
    CHECK(offsetof(das_table, capacity)   == offsetof(Table, capacity));
    CHECK(offsetof(das_table, flags)      == offsetof(Table, flags));
    CHECK(offsetof(das_table, keys)       == offsetof(Table, keys));
    CHECK(offsetof(das_table, hashes)     == offsetof(Table, hashes));
    CHECK(offsetof(das_table, tombstones) == offsetof(Table, tombstones));
}

TEST_CASE("context heap API: allocate / reallocate / free round-trip") {
    static const char * SRC = "options gen2\n[export] def main {}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    SUBCASE("allocate returns non-null and is freeable") {
        void * p = das_context_allocate(ic.ctx, 64);
        REQUIRE(p != nullptr);
        memset(p, 0xAB, 64);
        das_context_free(ic.ctx, p, 64);
    }

    SUBCASE("allocate(0) returns null and free(null) is a no-op") {
        CHECK(das_context_allocate(ic.ctx, 0) == nullptr);
        das_context_free(ic.ctx, nullptr, 0); // must not crash
    }

    SUBCASE("reallocate preserves prefix bytes") {
        void * p = das_context_allocate(ic.ctx, 16);
        REQUIRE(p != nullptr);
        for ( int i=0; i<16; i++ ) ((unsigned char*)p)[i] = (unsigned char)i;
        void * q = das_context_reallocate(ic.ctx, p, 16, 64);
        REQUIRE(q != nullptr);
        for ( int i=0; i<16; i++ ) {
            CHECK(((unsigned char*)q)[i] == (unsigned char)i);
        }
        das_context_free(ic.ctx, q, 64);
    }

    SUBCASE("reallocate to zero frees the block and returns NULL") {
        // The runtime allocator's reallocate can't reach new_size==0 safely;
        // the C wrapper handles it as free(ptr, old_size) + return NULL.
        void * p = das_context_allocate(ic.ctx, 32);
        REQUIRE(p != nullptr);
        void * q = das_context_reallocate(ic.ctx, p, 32, 0);
        CHECK(q == nullptr);
        // reallocate(NULL, 0, 0) is also a no-op.
        CHECK(das_context_reallocate(ic.ctx, nullptr, 0, 0) == nullptr);
    }

    cleanup_inline_ctx(ic);
}

TEST_CASE("das_array borrowed: daslang reads C-owned data") {
    static const char * SRC =
        "options gen2\n"
        "[export] def sum_array(arr : array<int>#) : int {\n"
        "    var t = 0\n"
        "    for (v in arr) { t += v; }\n"
        "    return t\n"
        "}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    int data[5] = { 10, 20, 30, 40, 50 };
    das_array borrowed;
    das_array_init_borrowed(&borrowed, data, 5, 5);
    // Borrowed-mode invariant: data points at our buffer; lock=1; shared=true.
    CHECK(borrowed.data == (char*)data);
    CHECK(borrowed.size == 5);
    CHECK(borrowed.capacity == 5);
    CHECK(borrowed.lock == 1);
    CHECK((borrowed.flags & 1u) == 1u); // shared bit set

    // Defensive: das_array_lock / _unlock are runtime no-ops on shared=true,
    // so a misuse-by-unlock cannot drop the lock and turn the borrow into an
    // owned-data path.
    das_array_lock(ic.ctx, &borrowed);
    CHECK(borrowed.lock == 1);
    das_array_unlock(ic.ctx, &borrowed);
    CHECK(borrowed.lock == 1);

    das_function * fn = das_context_find_function(ic.ctx, "sum_array");
    REQUIRE(fn != nullptr);
    vec4f args[1] = { das_result_array(&borrowed) };
    vec4f r = das_context_eval_with_catch(ic.ctx, fn, args);
    CHECK(das_context_get_exception(ic.ctx) == nullptr);
    CHECK(das_argument_int(r) == 150);
    // No mutation, no copy: the original buffer is unchanged.
    CHECK(data[0] == 10);
    CHECK(borrowed.data == (char*)data);

    cleanup_inline_ctx(ic);
}

TEST_CASE("das_array context-owned: daslang grows; C reads via das_array_at") {
    static const char * SRC =
        "options gen2\n"
        "[export] def fill_squares(var arr : array<int>; n : int) {\n"
        "    arr |> resize(n)\n"
        "    for (i in range(n)) { arr[i] = i * i; }\n"
        "}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    das_array owned;
    das_array_init(&owned);
    CHECK(owned.data == nullptr);
    CHECK(owned.size == 0);

    das_function * fn = das_context_find_function(ic.ctx, "fill_squares");
    REQUIRE(fn != nullptr);
    vec4f args[2] = {
        das_result_array(&owned),
        das_result_int(6),
    };
    das_context_eval_with_catch(ic.ctx, fn, args);
    CHECK(das_context_get_exception(ic.ctx) == nullptr);
    CHECK(owned.size == 6);
    CHECK(owned.capacity >= 6);
    REQUIRE(owned.data != nullptr);
    for ( uint32_t i = 0; i < owned.size; i++ ) {
        int v = *(int*)das_array_at(&owned, i, sizeof(int));
        CHECK(v == int(i*i));
    }

    das_array_clear(ic.ctx, &owned, sizeof(int));
    CHECK(owned.data == nullptr);
    CHECK(owned.size == 0);
    CHECK(owned.capacity == 0);

    cleanup_inline_ctx(ic);
}

TEST_CASE("das_array C-side resize / lock / unlock without daslang") {
    static const char * SRC = "options gen2\n[export] def main {}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    das_array a;
    das_array_init(&a);
    das_array_resize(ic.ctx, &a, 10, sizeof(int), /*zero=*/1);
    REQUIRE(a.data != nullptr);
    CHECK(a.size == 10);
    for ( uint32_t i = 0; i < 10; i++ ) {
        CHECK(*(int*)das_array_at(&a, i, sizeof(int)) == 0); // zeroed
        *(int*)das_array_at(&a, i, sizeof(int)) = int(i) * 7;
    }
    das_array_lock(ic.ctx, &a);
    CHECK(a.lock == 1);
    das_array_lock(ic.ctx, &a);
    CHECK(a.lock == 2);
    das_array_unlock(ic.ctx, &a);
    das_array_unlock(ic.ctx, &a);
    CHECK(a.lock == 0);
    das_array_clear(ic.ctx, &a, sizeof(int));

    cleanup_inline_ctx(ic);
}

TEST_CASE("das_table fill from daslang; C reads via das_table_find") {
    static const char * SRC =
        "options gen2\n"
        "[export] def count_values(var counts : table<int; int>; data : array<int>#) {\n"
        "    for (v in data) {\n"
        "        if (key_exists(counts, v)) { counts[v] = counts[v] + 1; }\n"
        "        else { counts[v] = 1; }\n"
        "    }\n"
        "}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    int input[10] = { 7, 3, 7, 1, 3, 7, 1, 1, 1, 3 };
    das_array data_view;
    das_array_init_borrowed(&data_view, input, 10, 10);

    das_table counts;
    das_table_init(&counts);
    das_function * fn = das_context_find_function(ic.ctx, "count_values");
    REQUIRE(fn != nullptr);
    vec4f args[2] = {
        das_result_table(&counts),
        das_result_array(&data_view),
    };
    das_context_eval_with_catch(ic.ctx, fn, args);
    CHECK(das_context_get_exception(ic.ctx) == nullptr);
    CHECK(counts.size == 3);

    int k1 = 1, k3 = 3, k7 = 7, k99 = 99;
    int * p1 = (int*)das_table_find(ic.ctx, &counts, DAS_TYPE_INT, &k1, sizeof(int));
    int * p3 = (int*)das_table_find(ic.ctx, &counts, DAS_TYPE_INT, &k3, sizeof(int));
    int * p7 = (int*)das_table_find(ic.ctx, &counts, DAS_TYPE_INT, &k7, sizeof(int));
    int * p99 = (int*)das_table_find(ic.ctx, &counts, DAS_TYPE_INT, &k99, sizeof(int));
    REQUIRE(p1); CHECK(*p1 == 4);
    REQUIRE(p3); CHECK(*p3 == 3);
    REQUIRE(p7); CHECK(*p7 == 3);
    CHECK(p99 == nullptr);

    das_table_clear(ic.ctx, &counts, DAS_TYPE_INT, sizeof(int));
    CHECK(counts.data == nullptr);
    CHECK(counts.size == 0);

    cleanup_inline_ctx(ic);
}

TEST_CASE("das_table insert / find / erase round-trip from C only") {
    static const char * SRC = "options gen2\n[export] def main {}\n";
    InlineCtx ic = compile_inline(SRC);
    REQUIRE(ic.ctx != nullptr);

    SUBCASE("int keys") {
        das_table t;
        das_table_init(&t);
        for ( int i = 0; i < 32; i++ ) {
            int * pv = (int*)das_table_insert(ic.ctx, &t, DAS_TYPE_INT, &i, sizeof(int));
            REQUIRE(pv);
            *pv = i * 10;
        }
        CHECK(t.size == 32);
        for ( int i = 0; i < 32; i++ ) {
            int * pv = (int*)das_table_find(ic.ctx, &t, DAS_TYPE_INT, &i, sizeof(int));
            REQUIRE(pv);
            CHECK(*pv == i * 10);
        }
        int missing = 999;
        CHECK(das_table_find(ic.ctx, &t, DAS_TYPE_INT, &missing, sizeof(int)) == nullptr);
        int kerase = 5;
        CHECK(das_table_erase(ic.ctx, &t, DAS_TYPE_INT, &kerase, sizeof(int)) == 1);
        CHECK(das_table_erase(ic.ctx, &t, DAS_TYPE_INT, &kerase, sizeof(int)) == 0);
        CHECK(das_table_find(ic.ctx, &t, DAS_TYPE_INT, &kerase, sizeof(int)) == nullptr);
        das_table_clear(ic.ctx, &t, DAS_TYPE_INT, sizeof(int));
    }

    SUBCASE("int64 keys") {
        das_table t;
        das_table_init(&t);
        long long k = 0x123456789abcdefLL;
        int v_in = 42;
        int * pv = (int*)das_table_insert(ic.ctx, &t, DAS_TYPE_INT64, &k, sizeof(int));
        REQUIRE(pv);
        *pv = v_in;
        int * found = (int*)das_table_find(ic.ctx, &t, DAS_TYPE_INT64, &k, sizeof(int));
        REQUIRE(found);
        CHECK(*found == 42);
        das_table_clear(ic.ctx, &t, DAS_TYPE_INT64, sizeof(int));
    }

    SUBCASE("uint64 keys") {
        das_table t;
        das_table_init(&t);
        unsigned long long k = 0xdeadbeefcafebabeULL;
        int * pv = (int*)das_table_insert(ic.ctx, &t, DAS_TYPE_UINT64, &k, sizeof(int));
        REQUIRE(pv);
        *pv = 7;
        int * found = (int*)das_table_find(ic.ctx, &t, DAS_TYPE_UINT64, &k, sizeof(int));
        REQUIRE(found);
        CHECK(*found == 7);
        das_table_clear(ic.ctx, &t, DAS_TYPE_UINT64, sizeof(int));
    }

    SUBCASE("enumeration / bitfield keys hit the extended dispatch") {
        // Both reuse the underlying integer hash but go through the
        // tEnumeration / tBitfield case arms — proves the dispatch now
        // mirrors table_reserve_impl, not just the scalar subset.
        das_table t;
        das_table_init(&t);
        int32_t e_key = 7;       // tEnumeration is stored as int32
        uint32_t b_key = 0xCAFEu; // tBitfield is stored as uint32

        int * pe = (int*)das_table_insert(ic.ctx, &t, DAS_TYPE_ENUMERATION, &e_key, sizeof(int));
        REQUIRE(pe);
        *pe = 100;
        int * found_e = (int*)das_table_find(ic.ctx, &t, DAS_TYPE_ENUMERATION, &e_key, sizeof(int));
        REQUIRE(found_e);
        CHECK(*found_e == 100);
        das_table_clear(ic.ctx, &t, DAS_TYPE_ENUMERATION, sizeof(int));

        das_table_init(&t);
        int * pb = (int*)das_table_insert(ic.ctx, &t, DAS_TYPE_BITFIELD, &b_key, sizeof(int));
        REQUIRE(pb);
        *pb = 200;
        int * found_b = (int*)das_table_find(ic.ctx, &t, DAS_TYPE_BITFIELD, &b_key, sizeof(int));
        REQUIRE(found_b);
        CHECK(*found_b == 200);
        das_table_clear(ic.ctx, &t, DAS_TYPE_BITFIELD, sizeof(int));
    }

    cleanup_inline_ctx(ic);
}
