// 64-bit size accessors for the AST value->literal conversion path.
// Array::size / Table::size are uint64_t. The 32-bit accessors (any_array_size /
// any_table_size) panic past INT_MAX instead of silently truncating; the long_
// counterparts (any_array_long_size / any_table_long_size) carry the full count.
//
// We can exercise the >INT_MAX path without a multi-GB allocation: the accessors
// only read the `size` field, so a fabricated Array/Table with `size` set (and a
// null `data`) is enough.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_builtin_ast.h"

#include <cstring>

using namespace das;

#define SCRIPT_PATH "/tests-cpp/small/test_any_size_64bit.das"

TEST_CASE("any_array/table size — 64-bit accessors + overflow panic") {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    auto program = compileDaScript(getDasRoot() + SCRIPT_PATH,
                                   fAccess, tout, dummyLibGroup);
    REQUIRE_FALSE(program->failed());

    Context ctx(program->getContextStackSize());
    REQUIRE(program->simulate(ctx, tout));

    LineInfoArg at;
    const uint64_t big = uint64_t(INT32_MAX) + 100u; // > INT_MAX; as int32_t it wraps negative (INT32_MIN + 99), not a valid count

    SUBCASE("array: under INT_MAX returns exact size; long matches") {
        Array arr; memset(&arr, 0, sizeof(arr));
        arr.size = 5;
        int32_t v = -1;
        bool ok = ctx.runWithCatchAndClear([&](){ v = any_array_size(&arr, &ctx, &at); });
        CHECK_MESSAGE(ok, "small array size must not panic");
        CHECK(v == 5);
        CHECK(any_array_long_size(&arr) == 5);
    }

    SUBCASE("array: past INT_MAX panics; long carries the full count") {
        Array arr; memset(&arr, 0, sizeof(arr));
        arr.size = big;
        // the bug the 32-bit accessor used to hide: long_ returns the full value
        CHECK(any_array_long_size(&arr) == int64_t(big));
        // 32-bit accessor panics instead of returning a wrapped-negative count
        bool ok = ctx.runWithCatchAndClear([&](){ (void) any_array_size(&arr, &ctx, &at); });
        CHECK_FALSE_MESSAGE(ok, "oversized array size must panic");
        CHECK(ctx.getException() == nullptr); // AndClear scrubbed it
    }

    SUBCASE("table: under INT_MAX returns exact size; long matches") {
        Table tab; memset(&tab, 0, sizeof(tab));
        tab.size = 3;
        int32_t v = -1;
        bool ok = ctx.runWithCatchAndClear([&](){ v = any_table_size(&tab, &ctx, &at); });
        CHECK_MESSAGE(ok, "small table size must not panic");
        CHECK(v == 3);
        CHECK(any_table_long_size(&tab) == 3);
    }

    SUBCASE("table: past INT_MAX panics; long carries the full count") {
        Table tab; memset(&tab, 0, sizeof(tab));
        tab.size = big;
        CHECK(any_table_long_size(&tab) == int64_t(big));
        bool ok = ctx.runWithCatchAndClear([&](){ (void) any_table_size(&tab, &ctx, &at); });
        CHECK_FALSE_MESSAGE(ok, "oversized table size must panic");
    }
}
