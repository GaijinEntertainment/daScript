#include <doctest/doctest.h>

#include "daScript/daScript.h"

using namespace das;

namespace {

static ProgramPtr compileLiteral(const char * src, uint32_t len) {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    const string name = "eof_no_newline_test.das";
    fAccess->setFileInfo(name, make_unique<TextFileInfo>(src, len, /*own*/false));
    return compileDaScript(name, fAccess, tout, dummyLibGroup);
}

}  // namespace

TEST_CASE("gen2: top-level statement at EOF without trailing newline") {
    SUBCASE("options X at EOF, no trailing newline") {
        static const char src[] = "options gen2\noptions no_aot";
        auto p = compileLiteral(src, uint32_t(sizeof(src) - 1));
        REQUIRE(p);
        CHECK_FALSE(p->failed());
    }
    SUBCASE("require X at EOF, no trailing newline") {
        static const char src[] = "options gen2\nrequire daslib/strings_boost";
        auto p = compileLiteral(src, uint32_t(sizeof(src) - 1));
        REQUIRE(p);
        CHECK_FALSE(p->failed());
    }
    SUBCASE("control: trailing newline still parses (no double-emit regression)") {
        static const char src[] = "options gen2\noptions no_aot\n";
        auto p = compileLiteral(src, uint32_t(sizeof(src) - 1));
        REQUIRE(p);
        CHECK_FALSE(p->failed());
    }
}
