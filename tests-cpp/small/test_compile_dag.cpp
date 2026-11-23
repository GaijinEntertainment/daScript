#include <doctest/doctest.h>
#include "daScript/daScript.h"

using namespace das;

TEST_CASE("compile DAG deadlock prevention: a macro module reaching a nesting builtin through a helper is nesting") {
    auto fAccess = make_smart<FsFileAccess>();
    TextWriter logs;
    ModuleGroup libGroup;
    auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_compile_dag.das", fAccess, logs, libGroup);
    REQUIRE_MESSAGE(!program->failed(), logs.str());
    auto nesting = libGroup.findModule("test_compile_dag_nesting");
    auto leaf = libGroup.findModule("test_compile_dag_leaf");
    REQUIRE(nesting);
    REQUIRE(leaf);
    REQUIRE(nesting->macroContext);
    REQUIRE(leaf->macroContext);
    CHECK(nesting->macroNestsCompile);
    CHECK_FALSE(leaf->macroNestsCompile);
}
