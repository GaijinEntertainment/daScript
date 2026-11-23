#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include <atomic>
#include <thread>
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

TEST_CASE("a locked FileAccess loads no new file, whatever subclass serves the files") {
    struct CountingAccess : FileAccess {
        int32_t loads = 0;
        virtual FileInfo * getNewFileInfo ( const string & fileName ) override {
            loads ++;
            return setFileInfo(fileName, make_unique<TextFileInfo>("", 0, false));
        }
    };
    auto access = make_smart<CountingAccess>();
    access->lock();
    CHECK(access->getFileInfo("late.das") == nullptr);
    CHECK(access->loads == 0);
    access->unlock();
    CHECK(access->getFileInfo("late.das") != nullptr);
    CHECK(access->loads == 1);
}

TEST_CASE("a project resolver answers threads that ask it at once") {
    const string root = getDasRoot() + "/tests-cpp/small/";
    string pak = root + "test_project_access.das_project";
    auto access = get_file_access(pak.data());
    REQUIRE(access);
    std::atomic<int32_t> wrong{0};
    vector<daScriptEnvironment> envs(8, *daScriptEnvironment::getBound());
    for ( auto & env : envs ) env.g_Program = nullptr;
    vector<std::thread> threads;
    for ( int32_t t = 0; t != 8; ++t ) {
        threads.emplace_back([&, t]() {
            daScriptEnvironment::setBound(&envs[t]);
            for ( int32_t i = 0; i != 500; ++i ) {
                auto info = access->getModuleInfo("probe", root + "test_project_access_main.das");
                if ( info.moduleName != "probe" ) wrong ++;
            }
            daScriptEnvironment::setBound(nullptr);
        });
    }
    for ( auto & th : threads ) th.join();
    CHECK(wrong == 0);
}
