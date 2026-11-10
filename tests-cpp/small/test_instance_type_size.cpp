#include <doctest/doctest.h>

#include "daScript/daScript.h"

using namespace das;

namespace {

struct Outcome {
    int             instanceTypeErrors = 0;
    int             otherErrors = 0;
    vector<string>  compilingLines;
    vector<string>  instancedLines;
};

void collectLocationLines ( const string & extra, Outcome & outcome ) {
    size_t at = 0;
    while ( at < extra.size() ) {
        auto eol = extra.find('\n', at);
        if ( eol == string::npos ) eol = extra.size();
        auto line = extra.substr(at, eol - at);
        if ( line.rfind("while compiling ", 0) == 0 ) outcome.compilingLines.push_back(line);
        if ( line.rfind("instanced from ", 0) == 0 ) outcome.instancedLines.push_back(line);
        at = eol + 1;
    }
}

Outcome compileLiteral ( const string & src ) {
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
        collectLocationLines(err.extra, outcome);
    }
    return outcome;
}

bool abbreviated ( const string & line ) {
    return line.find("<...>") != string::npos;
}

}

TEST_CASE("an oversized generic instance is the only error of its compile") {
    auto outcome = compileLiteral(
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
    auto outcome = compileLiteral(
        "options gen2\n"
        "def grow(a; n : int) : int {\n"
        "    return n == 0 ? 0 : grow($(x : typedecl(a)) { return x; }, n - 1)\n"
        "}\n"
        "[export]\n"
        "def main {\n"
        "    print(\"{grow(1, 3)}\\n\")\n"
        "}\n");
    CHECK(outcome.instanceTypeErrors == 1);
    REQUIRE(outcome.compilingLines.size() == 1);
    CHECK(abbreviated(outcome.compilingLines[0]));
    REQUIRE(outcome.instancedLines.size() > 2);
    CHECK(abbreviated(outcome.instancedLines[0]));
    CHECK_FALSE(abbreviated(outcome.instancedLines.back()));
    for ( auto & line : outcome.compilingLines ) CHECK(line.size() < 4096);
    for ( auto & line : outcome.instancedLines ) CHECK(line.size() < 4096);
}

TEST_CASE("an inference error prints the small types of the functions it names in full") {
    auto outcome = compileLiteral(
        "options gen2\n"
        "def pick(a; b) : int {\n"
        "    return a + no_such_name\n"
        "}\n"
        "[export]\n"
        "def main {\n"
        "    print(\"{pick(1, 2.0)}\\n\")\n"
        "}\n");
    CHECK(outcome.instanceTypeErrors == 0);
    REQUIRE(outcome.compilingLines.size() >= 1);
    CHECK(outcome.compilingLines[0].find("(a: int const explicit; b: float const explicit): int const") != string::npos);
    REQUIRE(outcome.instancedLines.size() >= 1);
    CHECK(outcome.instancedLines[0].find("instanced from main: void at ") == 0);
}
