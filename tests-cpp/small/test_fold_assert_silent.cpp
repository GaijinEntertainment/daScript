#include <doctest/doctest.h>
#include "daScript/daScript.h"

using namespace das;

namespace {

    struct AssertLogCapture {
        string seen;
        AssertLogCapture() {
            onLogCppDebugAgent("fold_assert_probe", [this](Context *, const LineInfo *, int, const char * text) {
                seen += text;
                return true;
            });
        }
        ~AssertLogCapture() { uninstallCppDebugAgent("fold_assert_probe"); }
    };
}

TEST_CASE("an assert the constant folder trips reports nothing, the same assert at run time reports") {
    TextPrinter tout;
    ModuleGroup libGroup;
    auto fAccess = make_smart<FsFileAccess>();
    AssertLogCapture capture;
    auto program = compileDaScript(getDasRoot() + "/tests-cpp/small/test_fold_assert_silent.das", fAccess, tout, libGroup);
    REQUIRE_FALSE(program->failed());
    CHECK_EQ(capture.seen.find("assert failed"), string::npos);
    Context ctx(program->getContextStackSize());
    REQUIRE(program->simulate(ctx, tout));
    auto fn = ctx.findFunction("unused_asserted_call");
    REQUIRE(fn != nullptr);
    auto res = ctx.evalWithCatch(fn, nullptr);
    REQUIRE(ctx.getException() == nullptr);
    CHECK(cast<bool>::to(res));
    CHECK_NE(capture.seen.find("assert failed, not positive"), string::npos);
}
