#include <doctest/doctest.h>
#include "daScript/daScript.h"

using namespace das;

TEST_CASE("compile DAG: four threads compile the modules one thread compiles") {
    auto compileWith = []( int32_t threads ) {
        auto fAccess = make_smart<FsFileAccess>();
        TextWriter logs;
        ModuleGroup libGroup;
        CodeOfPolicies policies;
        policies.compile_threads = threads;
        auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_compile_dag.das", fAccess, logs, libGroup, policies);
        REQUIRE_MESSAGE(!program->failed(), logs.str());
        vector<string> names;
        for ( auto mod : libGroup.getModules() ) names.push_back(mod->name + (mod->macroNestsCompile ? " nesting" : ""));
        sort(names.begin(), names.end());
        return names;
    };
    auto parallel = compileWith(4);
    CHECK(find(parallel.begin(), parallel.end(), "test_compile_dag_nesting nesting") != parallel.end());
    CHECK(find(parallel.begin(), parallel.end(), "test_compile_dag_leaf") != parallel.end());
    CHECK_EQ(parallel, compileWith(1));
}

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
