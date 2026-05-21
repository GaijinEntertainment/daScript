// PR-I (daslang 64-bit-arrays project) — int32_t -> uint32_t cast misuse in
// aot.h indexing paths. Two bug classes:
//
//   (1) Corruption — TArray<TT>::operator()(int32_t, Context*) and
//       safe_index(int32_t): for arr.size > UINT32_MAX, a negative index
//       wraps via uint32_t(-2) = 4294967294, which is < size, so the bounds
//       check passes silently. The access then uses the SIGNED index
//       (data[-2]) — out-of-bounds read/write that the runtime believes is
//       valid. Real precondition: >4 GB-element array (persistent_heap) hit
//       from AOT-emitted code with an int-typed (not int64) index expression.
//
//   (2) Diagnostic — all the int32 overloads (das_default_vector_index,
//       das_ati/das_atci, das_vec_index, TDim, das_index<Matrix>, TArray
//       itself when size < UINT32_MAX): same uint32_t-cast pattern, but
//       SIZE_POLICY caps at UINT32_MAX so the wrap-pass-bounds-check window
//       doesn't exist. The bug surfaces as a misleading error message
//       ("4294967294 of 10") instead of the actual signed index ("-2 of 10")
//       — confusing every developer who passes a negative index.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/aot.h"

#include <cstring>
#include <vector>

using namespace das;

TEST_CASE("aot.h int32 indexing — negative-index corruption + message clarity") {

    SUBCASE("TArray::safe_index(-2) on size>UINT32_MAX returns nullptr (corruption)") {
        // Fake TArray<uint8_t> with size beyond UINT32_MAX. Real (small) backing
        // buffer placed mid-stride so a wrapped index doesn't underflow the page.
        TArray<uint8_t> arr;
        uint8_t fake_buf[32] = {};
        arr.data = (char*)(fake_buf + 16);
        arr.size = uint64_t(5'000'000'000ULL);
        arr.capacity = arr.size;

        uint8_t * p_mut = TArray<uint8_t>::safe_index(&arr, int32_t(-2), nullptr);
        // Master: uint32_t(-2) = 4294967294 < 5e9 -> bounds check passes ->
        //         returns data+(-2) = fake_buf+14 (non-null, bug proven).
        // Fix:    negative detected before unsigned cast -> nullptr.
        CHECK(p_mut == nullptr);

        const TArray<uint8_t> & carr = arr;
        const uint8_t * p_const = TArray<uint8_t>::safe_index(&carr, int32_t(-2), nullptr);
        CHECK(p_const == nullptr);
    }

    SUBCASE("TArray::operator()(-2) error message shows '-2' (diagnostic)") {
        Context ctx(1024);
        TArray<int32_t> arr;
        int32_t buf[10] = {};
        arr.data = (char*)buf;
        arr.size = 10;
        arr.capacity = 10;

        bool ok = ctx.runWithCatch([&](){
            (void)arr(int32_t(-2), &ctx);
        });
        CHECK_FALSE(ok);
        REQUIRE(ctx.getException() != nullptr);
        const char * msg = ctx.getException();
        // Master: "array index out of range, 4294967294 of 10"
        // Fix:    "array index out of range, -2 of 10"
        CHECK_MESSAGE(strstr(msg, "-2") != nullptr,
                      "expected '-2' in error, got: ", msg);
        CHECK_MESSAGE(strstr(msg, "4294967294") == nullptr,
                      "error must NOT show the uint32 wrap value, got: ", msg);
        ctx.clearException();
    }

    SUBCASE("das_default_vector_index::at(-2) error message shows '-2' (diagnostic)") {
        Context ctx(1024);
        std::vector<int32_t> vec(10, 0);

        bool ok = ctx.runWithCatch([&](){
            (void)das_default_vector_index<std::vector<int32_t>, int32_t>::at(vec, int32_t(-2), &ctx);
        });
        CHECK_FALSE(ok);
        REQUIRE(ctx.getException() != nullptr);
        const char * msg = ctx.getException();
        CHECK_MESSAGE(strstr(msg, "-2") != nullptr,
                      "expected '-2' in error, got: ", msg);
        CHECK_MESSAGE(strstr(msg, "4294967294") == nullptr,
                      "error must NOT show wrap value, got: ", msg);
        ctx.clearException();
    }

    SUBCASE("TDim::operator()(-2) error message shows '-2' (diagnostic)") {
        Context ctx(1024);
        TDim<int32_t, 5> dim;
        for ( int i=0; i<5; i++ ) dim[i] = i;

        bool ok = ctx.runWithCatch([&](){
            (void)dim(int32_t(-2), &ctx);
        });
        CHECK_FALSE(ok);
        REQUIRE(ctx.getException() != nullptr);
        const char * msg = ctx.getException();
        CHECK_MESSAGE(strstr(msg, "-2") != nullptr,
                      "expected '-2' in error, got: ", msg);
        CHECK_MESSAGE(strstr(msg, "4294967294") == nullptr,
                      "error must NOT show wrap value, got: ", msg);
        ctx.clearException();
    }
}
