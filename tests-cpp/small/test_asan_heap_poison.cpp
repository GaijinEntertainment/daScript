#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/misc/das_asan.h"
#include "daScript/simulate/aot_builtin.h"

#include <cstring>

using namespace das;

#if DAS_ASAN

static bool poisoned ( const void * p ) {
    return __asan_address_is_poisoned(p) != 0;
}

TEST_CASE("asan: the persistent heap hands every allocation to asan, which bounds it") {
    MemoryModel mm;
    CHECK(mm.maxShoeAllocation == 0);
    char * p = mm.allocate(20);
    REQUIRE(p != nullptr);
    CHECK_FALSE(poisoned(p));
    CHECK_FALSE(poisoned(p + 31));
    CHECK(poisoned(p + 32));
    CHECK(mm.bytesAllocated() == 32);
    mm.free(p, 20);
    CHECK(poisoned(p));
    CHECK(mm.bytesAllocated() == 0);
}

TEST_CASE("asan: an object the sweep finds dead is freed through asan") {
    MemoryModel mm;
    char * live = mm.allocate(16);
    char * dead = mm.allocate(16);
    REQUIRE(live != nullptr);
    REQUIRE(dead != nullptr);
    auto it = mm.bigStuff.find(live);
    REQUIRE(it != mm.bigStuff.end());
    it->second |= DAS_PAGE_GC_MASK;
    mm.sweep();
    CHECK_FALSE(poisoned(live));
    CHECK(poisoned(dead));
    CHECK(mm.bytesAllocated() == 16);
    CHECK_FALSE(mm.isAllocatedPtr(dead, 16));
    mm.free(live, 16);
}

TEST_CASE("asan: reallocate carries the old contents into a block asan bounds") {
    MemoryModel mm;
    char * p = mm.allocate(20);
    REQUIRE(p != nullptr);
    memset(p, 7, 20);
    char * q = mm.reallocate(p, 20, 600);
    REQUIRE(q != nullptr);
    CHECK(q[19] == 7);
    CHECK_FALSE(poisoned(q + 607));
    CHECK(poisoned(q + 608));
    mm.free(q, 600);
}

TEST_CASE("asan: the linear heap poisons its chunk, and a gap follows every allocation") {
    LinearHeapAllocator heap;
    char * a = heap.impl_allocate(20);
    char * b = heap.impl_allocate(20);
    REQUIRE(a != nullptr);
    REQUIRE(b != nullptr);
    CHECK_FALSE(poisoned(a + 19));
    CHECK(poisoned(a + 20));
    CHECK(b >= a + 32 + DAS_ASAN_REDZONE);
    CHECK(poisoned(b + 20));
    heap.reset();
    CHECK(poisoned(a));
}

TEST_CASE("asan: the linear object heap poisons a free in the middle of its chunk") {
    LinearHeapAllocator heap;
    char * a = heap.impl_allocate(20);
    char * b = heap.impl_allocate(20);
    REQUIRE(a != nullptr);
    REQUIRE(b != nullptr);
    heap.impl_free(a, 20);
    CHECK(poisoned(a));
    CHECK_FALSE(poisoned(b));
}

TEST_CASE("asan: the linear string heap packs strings with no gap") {
    LinearStringAllocator strings;
    char * a = strings.impl_allocate(4);
    char * b = strings.impl_allocate(4);
    REQUIRE(a != nullptr);
    REQUIRE(b != nullptr);
    memcpy(a, "abc", 4);
    memcpy(b, "def", 4);
    CHECK(b == a + 4);
    CHECK_FALSE(poisoned(a + 3));
    int count = 0;
    strings.forEachString([&](const char *) { count ++; });
    CHECK(count == 2);
}

TEST_CASE("asan: an array's elements past its size are poisoned up to its capacity") {
    Context ctx(16*1024, true);
    CodeOfPolicies policies;
    policies.persistent_heap = true;
    ctx.setup(0, 0, policies, AnnotationArgumentList());
    Array arr;
    memset(&arr, 0, sizeof(Array));
    array_resize(ctx, arr, 3, sizeof(int32_t), true, nullptr);
    REQUIRE(arr.data != nullptr);
    REQUIRE(arr.capacity > 3);
    auto at = [&]( uint64_t i ) { return arr.data + i*sizeof(int32_t); };
    CHECK_FALSE(poisoned(at(2)));
    CHECK(poisoned(at(3)));
    CHECK(poisoned(at(arr.capacity - 1)));
    builtin_array_push_back(arr, sizeof(int32_t), &ctx, nullptr);
    CHECK_FALSE(poisoned(at(3)));
    CHECK(poisoned(at(4)));
    array_resize(ctx, arr, 1, sizeof(int32_t), false, nullptr);
    CHECK(poisoned(at(1)));
    array_reserve(ctx, arr, 1000, sizeof(int32_t), nullptr);
    CHECK_FALSE(poisoned(at(0)));
    CHECK(poisoned(at(1)));
    CHECK(poisoned(at(999)));
    ctx.free(arr.data, arr.capacity*sizeof(int32_t), nullptr);
}

TEST_CASE("asan: the context stack poisons everything below its live frames") {
    StackAllocator stack(4096);
    char * EP = nullptr;
    char * SP = nullptr;
    CHECK(poisoned(stack.bottom()));
    CHECK(poisoned(stack.top() - 1));
    REQUIRE(stack.push(64, EP, SP));
    char * frame = stack.ap();
    CHECK_FALSE(poisoned(frame));
    CHECK_FALSE(poisoned(frame + 63));
    CHECK(poisoned(frame - 1));
    char * EP2 = nullptr;
    char * SP2 = nullptr;
    REQUIRE(stack.push(32, EP2, SP2));
    char * inner = stack.ap();
    CHECK_FALSE(poisoned(inner));
    stack.pop(EP2, SP2);
    CHECK(poisoned(inner));
    CHECK_FALSE(poisoned(frame));
    stack.pop(EP, SP);
    CHECK(poisoned(frame));
}

TEST_CASE("asan: a borrowed array view is never annotated") {
    int32_t storage[8] = {};
    Array arr;
    memset(&arr, 0, sizeof(Array));
    array_mark_locked(arr, storage, 2, 8);
    DAS_ASAN_ANNOTATE_ARRAY(arr, sizeof(int32_t), 8, 2);
    CHECK_FALSE(poisoned(&storage[5]));
}

#else

TEST_CASE("asan: without the sanitizer the persistent heap keeps its size classes") {
    MemoryModel mm;
    CHECK(DAS_ASAN_REDZONE == 0);
    CHECK(mm.maxShoeAllocation == DAS_MAX_SHOE_ALLOCATION);
}

#endif
