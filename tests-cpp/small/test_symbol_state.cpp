#include <doctest/doctest.h>
#include "daScript/daScript.h"

using namespace das;

#define PROGRAM_A_PATH "/tests-cpp/small/test_symbol_state_a.das"
#define PROGRAM_B_PATH "/tests-cpp/small/test_symbol_state_b.das"

// src/ast/ARCHITECTURE.md sec.4
TEST_CASE("symbol use and context slots are per program, not per shared function") {
    TextPrinter tout;
    auto fAccess = make_smart<FsFileAccess>();
    ModuleGroup groupA;
    auto a = compileDaScript(getDasRoot() + PROGRAM_A_PATH, fAccess, tout, groupA);
    REQUIRE(a);
    REQUIRE_FALSE(a->failed());
    auto shared = Module::require("test_symbol_state_shared");
    REQUIRE(shared);
    auto usedByA = shared->findUniqueFunction("used_by_a");
    auto usedByB = shared->findUniqueFunction("used_by_b");
    REQUIRE(usedByA);
    REQUIRE(usedByB);
    CHECK(a->isUsed(usedByA));
    CHECK_FALSE(a->isUsed(usedByB));
    CHECK_GE(a->indexOf(usedByA), 0);
    CHECK_EQ(a->indexOf(usedByB), -2);

    ModuleGroup groupB;
    auto b = compileDaScript(getDasRoot() + PROGRAM_B_PATH, fAccess, tout, groupB);
    REQUIRE(b);
    REQUIRE_FALSE(b->failed());
    CHECK(b->isUsed(usedByB));
    CHECK_FALSE(b->isUsed(usedByA));
    CHECK_GE(b->indexOf(usedByB), 0);
    CHECK_EQ(b->indexOf(usedByA), -2);

    CHECK(a->isUsed(usedByA));
    CHECK_FALSE(a->isUsed(usedByB));
    CHECK_GE(a->indexOf(usedByA), 0);
    CHECK_EQ(a->indexOf(usedByB), -2);
}
