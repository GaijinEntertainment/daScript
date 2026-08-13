#include <doctest/doctest.h>

#include "daScript/daScript.h"

using namespace das;

// The unreserved-resize guard through the host-embedding path: CodeOfPolicies::
// max_unreserved_size flows Program::simulate -> Context, a bare growing resize past it
// throws, and an exact reserve first passes at any size. The das-side twin
// (tests/language/max_unreserved_size.das) covers the `options` spelling and the
// runtime get/set calls; this covers the policy one.

namespace {

const char * SRC =
    "options gen2\n"
    "[export] def bare_resize {\n"
    "    var a : array<uint8>\n"
    "    a |> resize(65537)\n"
    "}\n"
    "[export] def reserved_resize {\n"
    "    var a : array<uint8>\n"
    "    a |> reserve(200000)\n"
    "    a |> resize(200000)\n"
    "}\n";

} // namespace

TEST_CASE("max_unreserved_size flows from CodeOfPolicies to the context") {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    const string name = "max_unreserved_size_test.das";
    fAccess->setFileInfo(name, make_unique<TextFileInfo>(SRC, uint32_t(strlen(SRC)), /*own*/false));
    CodeOfPolicies policies;
    policies.max_unreserved_size = 65536;
    auto program = compileDaScript(name, fAccess, tout, dummyLibGroup, policies);
    REQUIRE(program);
    REQUIRE_FALSE(program->failed());
    Context ctx(program->getContextStackSize());
    REQUIRE(program->simulate(ctx, tout));
    CHECK_EQ(ctx.maxUnreservedSize, uint64_t(65536));

    SUBCASE("bare growing resize past the policy throws") {
        auto fn = ctx.findFunction("bare_resize");
        REQUIRE(fn);
        ctx.evalWithCatch(fn, nullptr);
        CHECK(ctx.getException() != nullptr);
    }
    SUBCASE("exact reserve first passes at any size") {
        auto fn = ctx.findFunction("reserved_resize");
        REQUIRE(fn);
        ctx.evalWithCatch(fn, nullptr);
        CHECK(ctx.getException() == nullptr);
    }
}
