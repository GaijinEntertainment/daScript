#include <doctest/doctest.h>

#include "daScript/daScript.h"

using namespace das;

// Optional require: `require ?guard target` loads *target* only when module *guard* is registered.
// When *guard* is absent the require is skipped with no error (even if *target* does not exist);
// when *guard* is present the require behaves exactly like a normal require, so a missing *target*
// still surfaces as a prerequisite error (no masking).

namespace {

static ProgramPtr compileLiteral(const char * src) {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    const string name = "optional_require_test.das";
    fAccess->setFileInfo(name, make_unique<TextFileInfo>(src, uint32_t(strlen(src)), /*own*/false));
    return compileDaScript(name, fAccess, tout, dummyLibGroup);
}

}  // namespace

TEST_CASE("optional require: ?guard target") {
    SUBCASE("guard absent -> require skipped, no error even when target does not exist") {
        auto p = compileLiteral("options gen2\nrequire ?no_such_guard_xyz no_such_target_xyz\n");
        REQUIRE(p);
        CHECK_FALSE(p->failed());
    }
    SUBCASE("guard present + missing target -> real prerequisite error (no masking)") {
        auto p = compileLiteral("options gen2\nrequire ?strings no_such_target_xyz\n");
        REQUIRE(p);
        CHECK(p->failed());
    }
    SUBCASE("control: plain (non-optional) require of a missing target still errors") {
        auto p = compileLiteral("options gen2\nrequire no_such_target_xyz\n");
        REQUIRE(p);
        CHECK(p->failed());
    }
    SUBCASE("guard present + valid target -> loads normally") {
        auto p = compileLiteral("options gen2\nrequire ?strings strings\n");
        REQUIRE(p);
        CHECK_FALSE(p->failed());
    }
}
