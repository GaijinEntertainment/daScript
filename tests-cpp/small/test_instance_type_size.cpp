#include <doctest/doctest.h>

#include "daScript/daScript.h"

using namespace das;

namespace {

struct Outcome {
    int     instanceTypeErrors = 0;
    int     otherErrors = 0;
    size_t  longestExtra = 0;
    bool    hasAbbreviatedType = false;
};

Outcome compileSource ( const string & src ) {
    TextWriter tout;
    ModuleGroup libGroup;
    auto fa = make_smart<FsFileAccess>();
    fa->setFileInfo("instance_type_size.das", make_unique<TextFileInfo>(src.c_str(), uint32_t(src.size()), false));
    auto program = compileDaScript("instance_type_size.das", fa, tout, libGroup);
    Outcome outcome;
    for ( auto & err : program->errors ) {
        if ( err.cerr == CompilationError::exceeds_instance_type ) {
            outcome.instanceTypeErrors ++;
        } else {
            outcome.otherErrors ++;
        }
        outcome.longestExtra = max(outcome.longestExtra, err.extra.size());
        if ( err.extra.find("<...>") != string::npos ) outcome.hasAbbreviatedType = true;
    }
    return outcome;
}

}

TEST_CASE("an oversized generic instance is the only error of its compile") {
    auto outcome = compileSource(
        "options gen2\n"
        "def grow(a) {\n"
        "    return grow((a, a))\n"
        "}\n"
        "[export]\n"
        "def main {\n"
        "    grow(1)\n"
        "}\n");
    CHECK(outcome.instanceTypeErrors == 1);
    CHECK(outcome.otherErrors == 0);
}

TEST_CASE("an inference error abbreviates the oversized types of the functions it names") {
    auto outcome = compileSource(
        "options gen2\n"
        "def grow(a; n : int) : int {\n"
        "    return n == 0 ? 0 : grow($(x : typedecl(a)) { return x; }, n - 1)\n"
        "}\n"
        "[export]\n"
        "def main {\n"
        "    print(\"{grow(1, 3)}\\n\")\n"
        "}\n");
    CHECK(outcome.instanceTypeErrors == 1);
    CHECK(outcome.hasAbbreviatedType);
    CHECK(outcome.longestExtra < 64 * 1024);
}
