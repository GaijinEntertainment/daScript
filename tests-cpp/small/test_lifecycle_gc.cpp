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

TEST_CASE("collection boundary does not repeatedly walk retained empty capacity") {
    Fixture f;
    REQUIRE(f.load("/tests-cpp/small/test_lifecycle_gc.das"));
    REQUIRE(f.frames(200));
    REQUIRE(f.ctx->collectHeapIfMostlyFree());
    for (int i = 0; i != 20; ++i) {
        CHECK_FALSE(f.ctx->collectHeapIfMostlyFree());
    }
}

TEST_CASE("allocation pressure remains a safety net after a collection") {
    Fixture f;
    REQUIRE(f.load("/tests-cpp/small/test_lifecycle_gc.das"));
    f.ctx->gcHeapBudget = 256 * 1024;
    f.ctx->gcStringBudget = 256 * 1024;
    f.ctx->collectHeap(nullptr, true, false);
    const auto count = f.ctx->gcCollections;
    CHECK_FALSE(f.ctx->collectHeapIfMostlyFree());
    REQUIRE(f.frames(8));
    CHECK(f.ctx->collectHeapIfMostlyFree());
    CHECK(f.ctx->gcCollections == count + 1);
    CHECK(f.ctx->gcHeapReclaimed >= 8 * 16384 * sizeof(float));
    CHECK(f.ctx->gcTotalUsec >= f.ctx->gcLastUsec);
    CHECK(f.ctx->gcPeakUsec >= f.ctx->gcLastUsec);
    CHECK_FALSE(f.ctx->collectHeapIfMostlyFree());
    auto * p = f.ctx->heap->impl_allocate(1024);
    f.ctx->heap->impl_free(p, 1024);
    CHECK_FALSE(f.ctx->collectHeapIfMostlyFree());
}

TEST_CASE("shrinking reallocations do not wrap cumulative allocated bytes") {
    PersistentHeapAllocator heap;
    auto * p = heap.impl_allocate(1024);
    p = heap.impl_reallocate(p, 1024, 512);
    REQUIRE(p);
    CHECK(heap.getTotalBytesAllocated() == 1024);
    CHECK(heap.getTotalBytesDeleted() == 512);
    CHECK(heap.getTotalReallocations() == 1);
    heap.impl_free(p, 512);
    CHECK(heap.getTotalFrees() == 1);
    CHECK(heap.getTotalBytesDeleted() == 1024);
}

TEST_CASE("null reallocation is counted as an allocation") {
    PersistentHeapAllocator heap;
    auto * p = heap.impl_reallocate(nullptr, 0, 128);
    REQUIRE(p);
    CHECK(heap.getTotalAllocations() == 1);
    CHECK(heap.getTotalReallocations() == 0);
    heap.impl_free(p, 128);
}

TEST_CASE("string pressure independently triggers the shared collection boundary") {
    Fixture f;
    REQUIRE(f.load("/tests-cpp/small/test_lifecycle_gc.das"));
    f.ctx->collectHeap(nullptr, true, false);
    f.ctx->gcStringBudget = 1024;
    const string text(4096, 'x');
    REQUIRE(f.ctx->allocateString(text, nullptr) != nullptr);
    CHECK(f.ctx->collectHeapIfMostlyFree());
    CHECK(f.ctx->gcStringReclaimed >= 4096);
    CHECK_FALSE(f.ctx->collectHeapIfMostlyFree());
}

TEST_CASE("heap-only collection preserves pending string pressure") {
    Fixture f;
    REQUIRE(f.load("/tests-cpp/small/test_lifecycle_gc.das"));
    f.ctx->collectHeap(nullptr, true, false);
    f.ctx->gcStringBudget = 1024;
    const string text(4096, 'x');
    REQUIRE(f.ctx->allocateString(text, nullptr) != nullptr);
    f.ctx->collectHeap(nullptr, false, false);
    CHECK(f.ctx->collectHeapIfMostlyFree());
    CHECK(f.ctx->gcStringReclaimed >= 4096);
    CHECK_FALSE(f.ctx->collectHeapIfMostlyFree());
}

TEST_CASE("retained diagnostic strings do not repeatedly trigger collection") {
    Fixture f;
    REQUIRE(f.load("/tests-cpp/small/test_lifecycle_gc.das"));
    f.ctx->collectHeap(nullptr, true, false);
    f.ctx->gcStringBudget = 1024;
    f.ctx->stringHeap->setTrackAllocations(true);
    const string text(4096, 'x');
    REQUIRE(f.ctx->allocateString(text, nullptr) != nullptr);
    for (int i = 0; i != 3; ++i) CHECK_FALSE(f.ctx->collectHeapIfMostlyFree());
}
