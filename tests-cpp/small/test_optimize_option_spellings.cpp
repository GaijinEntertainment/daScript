// Program::getOptimize - every options spelling the compiler accepts for "no optimizer" turns it off.
#include <doctest/doctest.h>

#include "daScript/daScript.h"

using namespace das;

namespace {

static ProgramPtr compileLiteral ( const char * src ) {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    const string name = "optimize_option_test.das";
    fAccess->setFileInfo(name, make_unique<TextFileInfo>(src, uint32_t(strlen(src)), /*own*/false));
    return compileDaScript(name, fAccess, tout, dummyLibGroup);
}

static bool optimizeOf ( const char * optionsLine ) {
    string src = string("options gen2\n") + optionsLine + "\n[export]\ndef main {\n    print(\"x\\n\")\n}\n";
    auto prog = compileLiteral(src.c_str());
    REQUIRE(prog);
    REQUIRE_FALSE(prog->failed());
    return prog->getOptimize();
}

}

TEST_CASE("every accepted options spelling turns the optimizer off") {
    CHECK(optimizeOf(""));
    CHECK_FALSE(optimizeOf("options optimize = false"));
    CHECK_FALSE(optimizeOf("options no_optimization"));
    // the CodeOfPolicies field's own name carries the /*option*/ marker, so the parser accepts it;
    // getOptimize must read it, or the option is accepted and ignored
    CHECK_FALSE(optimizeOf("options no_optimizations"));
    CHECK(optimizeOf("options no_optimizations = false"));
}

TEST_CASE("an unknown options name is a compile error") {
    auto prog = compileLiteral("options gen2\noptions no_such_option\n[export]\ndef main {\n    print(\"x\\n\")\n}\n");
    REQUIRE(prog);
    CHECK(prog->failed());
}
