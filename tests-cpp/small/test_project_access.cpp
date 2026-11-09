#include <doctest/doctest.h>
#include "daScript/daScript.h"
using namespace das;

FileAccessPtr get_file_access ( char * pak );

TEST_CASE("a project resolver reads its own line info after get_file_access returns") {
    const string root = getDasRoot() + "/tests-cpp/small/";
    string pak = root + "test_project_access.das_project";
    auto access = get_file_access(pak.data());
    REQUIRE(access);
    TextWriter tout;
    ModuleGroup group;
    auto program = compileDaScript(root + "test_project_access_main.das", access, tout, group);
    REQUIRE(program);
    INFO(tout.str());
    REQUIRE_FALSE(program->failed());
    Context ctx(program->getContextStackSize());
    REQUIRE(program->simulate(ctx, tout));
    auto fn = ctx.findFunction("main");
    REQUIRE(fn);
    auto result = ctx.evalWithCatch(fn, nullptr);
    CHECK(ctx.getException() == nullptr);
    CHECK(cast<int32_t>::to(result) == 42);
}

TEST_CASE("the module resolver keeps the project's compile-time FileAccess alive") {
    const string root = getDasRoot() + "/tests-cpp/small/";
    auto compileAccess = make_smart<FsFileAccess>();
    TextWriter tout;
    ModuleGroup group;
    auto project = compileDaScript(root + "test_project_access.das_project", compileAccess, tout, group);
    REQUIRE(project);
    INFO(tout.str());
    REQUIRE_FALSE(project->failed());
    REQUIRE(project->access == compileAccess);
    const unsigned before = compileAccess->use_count();
    FileAccessPtr resolver = make_smart<FsFileAccess>(root + "test_project_access.das_project", project);
    CHECK(compileAccess->use_count() == before + 1);
    resolver.reset();
    CHECK(compileAccess->use_count() == before);
}
