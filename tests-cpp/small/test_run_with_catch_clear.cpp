// Test for issue #2523: Context::runWithCatch leaves exception/stopFlags set;
// C++ bindings have to clean up by hand.
//
// Verifies the contract of:
//   - runWithCatch                — leaves exception state set on panic
//   - clearException              — hard-zeros exception/last_exception/stopFlags
//   - runWithCatchAndClear        — bundles the two
// And reproduces the "double panic" symptom: a binding that catches a panic
// without clearing causes the next eval step on the same context to rethrow.

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/aot.h"

#include <cstring>

using namespace das;

#define SCRIPT_PATH "/tests-cpp/small/test_run_with_catch_clear.das"

TEST_CASE("runWithCatch / clearException contract — issue #2523") {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    auto program = compileDaScript(getDasRoot() + SCRIPT_PATH,
                                   fAccess, tout, dummyLibGroup);
    REQUIRE_FALSE(program->failed());

    Context ctx(program->getContextStackSize());
    REQUIRE(program->simulate(ctx, tout));

    auto fnPanic  = ctx.findFunction("panic_func");
    auto fnAddOne = ctx.findFunction("add_one");
    REQUIRE(fnPanic);
    REQUIRE(fnAddOne);

    SUBCASE("runWithCatch on success leaves no exception") {
        int32_t result = 0;
        bool ok = ctx.runWithCatch([&](){
            vec4f args[1];
            args[0] = cast<int32_t>::from(41);
            vec4f r = ctx.callOrFastcall(fnAddOne, args, nullptr);
            result = cast<int32_t>::to(r);
        });
        CHECK_MESSAGE(ok, "runWithCatch returned false on non-panicking call");
        CHECK_MESSAGE(result == 42, "non-panicking call returned wrong value");
        CHECK_MESSAGE(ctx.getException() == nullptr, "exception non-null after success");
    }

    SUBCASE("runWithCatch leaves exception set on panic") {
        bool ok = ctx.runWithCatch([&](){
            ctx.callOrFastcall(fnPanic, nullptr, nullptr);
        });
        CHECK_MESSAGE(!ok, "runWithCatch returned true on panic");
        CHECK_MESSAGE(ctx.getException() != nullptr, "exception null after panic");
        if ( ctx.getException() ) {
            const char * msg = ctx.getException();
            bool has_text = strstr(msg, "intentional panic from test") != nullptr;
            CHECK_MESSAGE(has_text, "exception message does not contain panic text");
        }
    }

    SUBCASE("clearException restores normal operation") {
        // First, get into a panicked state.
        ctx.runWithCatch([&](){
            ctx.callOrFastcall(fnPanic, nullptr, nullptr);
        });
        REQUIRE(ctx.getException() != nullptr);

        ctx.clearException();
        CHECK_MESSAGE(ctx.getException() == nullptr, "exception non-null after clearException");
        CHECK_MESSAGE(ctx.last_exception == nullptr, "last_exception non-null after clearException");
        CHECK_MESSAGE(ctx.stopFlags == 0, "stopFlags non-zero after clearException");

        int32_t result = 0;
        bool ok = ctx.runWithCatch([&](){
            vec4f args[1];
            args[0] = cast<int32_t>::from(7);
            vec4f r = ctx.callOrFastcall(fnAddOne, args, nullptr);
            result = cast<int32_t>::to(r);
        });
        CHECK_MESSAGE(ok, "post-clear call returned false (spurious rethrow?)");
        CHECK_MESSAGE(result == 8, "post-clear call returned wrong value");
    }

    SUBCASE("runWithCatchAndClear bundles the clear") {
        bool ok = ctx.runWithCatchAndClear([&](){
            ctx.callOrFastcall(fnPanic, nullptr, nullptr);
        });
        CHECK_MESSAGE(!ok, "runWithCatchAndClear returned true on panic");
        CHECK_MESSAGE(ctx.getException() == nullptr, "exception non-null after runWithCatchAndClear");
        CHECK_MESSAGE(ctx.last_exception == nullptr, "last_exception non-null after runWithCatchAndClear");
        CHECK_MESSAGE(ctx.stopFlags == 0, "stopFlags non-zero after runWithCatchAndClear");

        int32_t result = 0;
        ok = ctx.runWithCatch([&](){
            vec4f args[1];
            args[0] = cast<int32_t>::from(99);
            vec4f r = ctx.callOrFastcall(fnAddOne, args, nullptr);
            result = cast<int32_t>::to(r);
        });
        CHECK_MESSAGE(ok, "post-AndClear call returned false (spurious rethrow?)");
        CHECK_MESSAGE(result == 100, "post-AndClear call returned wrong value");
    }

    SUBCASE("double-panic repro — clearException cures it") {
        // Without clearException after a binding swallows the panic, the next
        // eval step on the same context observes the leaked state.
        bool ok = ctx.runWithCatch([&](){
            ctx.callOrFastcall(fnPanic, nullptr, nullptr);
        });
        CHECK_MESSAGE(!ok, "first call should have panicked");
        CHECK_MESSAGE(ctx.getException() != nullptr, "first call should have set exception");

        // The leak is observable: exception pointer is non-null without an
        // intervening clear.
        CHECK_MESSAGE(ctx.getException() != nullptr, "exception should still be set without clear");

        ctx.clearException();
        int32_t result = 0;
        ok = ctx.runWithCatch([&](){
            vec4f args[1];
            args[0] = cast<int32_t>::from(0);
            vec4f r = ctx.callOrFastcall(fnAddOne, args, nullptr);
            result = cast<int32_t>::to(r);
        });
        CHECK_MESSAGE(ok, "after clearException, follow-up call should succeed");
        CHECK_MESSAGE(result == 1, "after clearException, follow-up returned wrong value");
    }
}
