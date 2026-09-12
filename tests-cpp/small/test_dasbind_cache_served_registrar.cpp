// A dasbind registrar the module cache serves in a process that already compiled it
// (a live reload after the cache warmed) still retargets a fresh dependent's calls to its
// proxies. The registrar's [extern] apply ran in the first compile only; the served copy is
// a new Function object, and a transformCall keyed on the object alone left the calls on
// the stubs - a runtime "extern has no body" in the dependent, and a cache record carrying
// the unrewritten calls to every later process.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/ast/ast_serializer.h"

#include <cstdio>
#include <cstring>

using namespace das;

// the subject lives behind DAS_BIND_EXTERNAL (das_config.h), and the fixture names its libc
// through the platform-qualified library arguments parseExternArgs reads on MSVC, Apple and
// Linux alone; elsewhere (a MinGW Windows build) [extern] has no library to bind
#if DAS_BIND_EXTERNAL && (defined(_MSC_VER) || defined(__APPLE__) || defined(__linux__))

namespace {

struct ProbeRun {
    bool        compiled = false;
    bool        simulated = false;
    bool        called = false;
    int32_t     result = 0;
    string      exception;
    uint64_t    served = 0;
    bool        wrote = false;
};

ProbeRun run_probe ( const string & script, const string & cachePath ) {
    ProbeRun run;
    TextPrinter tout;
    ModuleFileCache cache;              // outlives the program: its FileInfos back the AST's LineInfos
    ModuleGroup libGroup;
    auto fAccess = make_smart<FsFileAccess>();
    cache.install(cachePath, cachePath, false);
    // the live host's policy: no promotion of shared modules, so the second compile walks
    // the same module list the record stream carries instead of skipping the promoted ones
    CodeOfPolicies policies;
    policies.ignore_shared_modules = true;
    auto program = compileDaScript(script, fAccess, tout, libGroup, policies);
    run.compiled = !program->failed();
    if ( run.compiled ) {
        Context ctx(program->getContextStackSize());
        run.simulated = program->simulate(ctx, tout);
        auto res = cache.finish();
        run.served = res.served;
        run.wrote = res.wrote;
        if ( run.simulated ) {
            auto fn = ctx.findFunction("probe");
            if ( fn ) {
                run.called = ctx.runWithCatch([&]() {
                    run.result = cast<int32_t>::to(ctx.eval(fn, nullptr));
                });
                if ( !run.called && ctx.getException() ) run.exception = ctx.getException();
            }
        }
    } else {
        cache.finish();
    }
    return run;
}

}

TEST_CASE("dasbind: a registrar served from the module cache still retargets a fresh dependent") {
    const string root = getDasRoot() + "/tests-cpp/small/";
    const string cachePath = getDasRoot() + "/.jitted_scripts/tests_cpp_dasbind_cache.dascache";   // gitignored, outside the pruned default directory
    remove(cachePath.c_str());

    // cold: the registrar compiles from source, its apply registers the proxies, the cache is written
    auto cold = run_probe(root + "test_dasbind_cache_drv_a.das", cachePath);
    REQUIRE_MESSAGE(cold.compiled, "cold compile failed");
    REQUIRE_MESSAGE(cold.simulated, "cold simulate failed");
    CHECK_MESSAGE(cold.called, "cold call threw: " << cold.exception);
    CHECK_EQ(cold.result, 7);
    REQUIRE_MESSAGE(cold.wrote, "the cold compile wrote no cache");

    // warm, in the same process: a different driver reparses, the registrar is served - its
    // builtin hash matches the process that populated it, so no drift resume reparses it
    auto warm = run_probe(root + "test_dasbind_cache_drv_b.das", cachePath);
    REQUIRE_MESSAGE(warm.compiled, "warm compile failed");
    REQUIRE_MESSAGE(warm.simulated, "warm simulate failed");
    CHECK_MESSAGE(warm.served >= 1, "the warm compile served nothing from the cache");
    CHECK_MESSAGE(warm.called, "warm call threw: " << warm.exception);
    CHECK_EQ(warm.result, 109);

    // a third compile reads the proxy the served registrar left in the dasbind module after
    // its own program died: the proxy's types must be the module's, not that program's
    auto again = run_probe(root + "test_dasbind_cache_drv_a.das", cachePath);
    remove(cachePath.c_str());
    REQUIRE_MESSAGE(again.compiled, "third compile failed");
    REQUIRE_MESSAGE(again.simulated, "third simulate failed");
    CHECK_MESSAGE(again.called, "third call threw: " << again.exception);
    CHECK_EQ(again.result, 7);
}

#endif
