#include <doctest/doctest.h>

#include "daScript/daScript.h"

using namespace das;

namespace {
    struct Fixture {
        TextPrinter tout;
        ModuleGroup libGroup;
        ProgramPtr program;
        unique_ptr<Context> ctx;
        SimFunction * updateFn = nullptr;

        bool load ( const char * rel ) {
            auto fAccess = make_smart<FsFileAccess>();
            program = compileDaScript(getDasRoot() + rel, fAccess, tout, libGroup);
            if ( !program || program->failed() ) return false;
            ctx = make_unique<Context>(program->getContextStackSize());
            if ( !program->simulate(*ctx, tout) ) return false;
            if ( auto initFn = ctx->findFunction("init") ) {
                ctx->runWithCatchAndClear([&](){ ctx->callOrFastcall(initFn, nullptr, nullptr); });
            }
            updateFn = ctx->findFunction("update");
            return updateFn != nullptr;
        }

        bool frames ( int n ) {
            return ctx->runWithCatchAndClear([&](){
                for ( int i = 0; i != n; ++i ) ctx->callOrFastcall(updateFn, nullptr, nullptr);
            });
        }
    };
}

TEST_CASE("collectHeapIfMostlyFree reclaims a gc context's per-frame garbage") {
    Fixture f;
    REQUIRE(f.load("/tests-cpp/small/test_lifecycle_gc.das"));
    REQUIRE(f.ctx->persistent);
    REQUIRE(f.ctx->gcEnabled);

    REQUIRE(f.frames(200));
    uint64_t grown = f.ctx->heap->bytesAllocated();
    CHECK_MESSAGE(grown > uint64_t(200) * 16384 * sizeof(float) / 2,
        "200 frames of a 64 KB junk array should have grown the heap past 6 MB, got ", grown);

    CHECK(f.ctx->collectHeapIfMostlyFree());
    uint64_t after = f.ctx->heap->bytesAllocated();
    CHECK_MESSAGE(after * 4 < grown,
        "a collection should have reclaimed the junk: before ", grown, " after ", after);

    REQUIRE(f.frames(200));
    CHECK(f.ctx->collectHeapIfMostlyFree());
    CHECK_MESSAGE(f.ctx->heap->bytesAllocated() * 4 < grown,
        "the boundary must hold across repeated passes, not once");
}

TEST_CASE("collectHeapIfMostlyFree is a no-op on a context that did not opt in") {
    Fixture f;
    REQUIRE(f.load("/tests-cpp/small/test_lifecycle_gc_nogc.das"));
    REQUIRE_FALSE(f.ctx->gcEnabled);

    REQUIRE(f.frames(50));
    bool ranClean = f.ctx->runWithCatchAndClear([&](){
        CHECK_FALSE(f.ctx->collectHeapIfMostlyFree());
    });
    CHECK_MESSAGE(ranClean, "a non-gc context must decline quietly, never throw");
}
