#include <doctest/doctest.h>

#include "daScript/daScript.h"

using namespace das;

namespace {

string repeated ( const string & piece, int count ) {
    string out;
    out.reserve(piece.size() * size_t(count));
    for ( int i = 0; i != count; ++i ) out += piece;
    return out;
}

struct Outcome {
    bool    failed = false;
    int     tooDeepErrors = 0;
};

Outcome compileSource ( const string & src ) {
    TextWriter tout;
    ModuleGroup libGroup;
    auto fa = make_smart<FsFileAccess>();
    fa->setFileInfo("type_nesting_limit.das", make_unique<TextFileInfo>(src.c_str(), uint32_t(src.size()), false));
    auto program = compileDaScript("type_nesting_limit.das", fa, tout, libGroup);
    Outcome outcome;
    outcome.failed = program->failed();
    for ( auto & err : program->errors ) {
        if ( err.cerr == CompilationError::exceeds_type_nesting ) outcome.tooDeepErrors ++;
    }
    return outcome;
}

Outcome compileFieldType ( const string & fieldType ) {
    return compileSource("options gen2\nstruct Deep {\n    x : " + fieldType + "\n}\n");
}

Outcome compileFieldTypeGen1 ( const string & fieldType ) {
    return compileSource("options gen2 = false\nstruct Deep\n    x : " + fieldType + "\n");
}

bool reportsTooDeep ( const Outcome & outcome ) {
    return outcome.tooDeepErrors != 0;
}

const int limit = 64;

}

TEST_CASE("a type nested to the limit compiles, one level past it is a parse error") {
    SUBCASE("pointer levels") {
        CHECK_FALSE(compileFieldType("int" + repeated("?", limit)).failed);
        CHECK(reportsTooDeep(compileFieldType("int" + repeated("?", limit + 1))));
    }
    SUBCASE("double pointer tokens") {
        CHECK_FALSE(compileFieldType("int" + repeated("??", limit / 2)).failed);
        CHECK(reportsTooDeep(compileFieldType("int" + repeated("??", limit / 2 + 1))));
        CHECK_FALSE(compileFieldType("int" + repeated("?", limit - 2) + "??").failed);
        CHECK(reportsTooDeep(compileFieldType("int" + repeated("?", limit - 1) + "??")));
    }
    SUBCASE("angle bracket levels") {
        CHECK_FALSE(compileFieldType(repeated("array<", limit) + "int" + repeated(">", limit)).failed);
        CHECK(reportsTooDeep(compileFieldType(repeated("array<", limit + 1) + "int" + repeated(">", limit + 1))));
    }
    SUBCASE("pointer levels count the nesting under them") {
        const int overHalf = limit / 2 + 1;
        const string pointers = repeated("?", overHalf);
        const string inner = "tuple<int;int" + pointers + ">";
        CHECK_FALSE(compileFieldType(inner).failed);
        CHECK(reportsTooDeep(compileFieldType("tuple<int;" + inner + pointers + ">")));
        CHECK(reportsTooDeep(compileFieldType("table<int;int" + pointers + ">" + pointers)));
    }
}

TEST_CASE("every nesting type constructor stops at the limit") {
    const char * openers[] = {
        "array<", "iterator<", "smart_ptr<", "table<int;", "tuple<int;", "variant<a:int;b:",
        "function<():", "lambda<():", "block<():"
    };
    for ( auto opener : openers ) {
        CAPTURE(opener);
        CHECK(reportsTooDeep(compileFieldType(repeated(opener, limit + 1) + "int" + repeated(">", limit + 1))));
    }
}

TEST_CASE("the gen1 grammar stops at the same nesting limit") {
    CHECK_FALSE(compileFieldTypeGen1("int" + repeated("?", limit)).failed);
    CHECK(reportsTooDeep(compileFieldTypeGen1("int" + repeated("?", limit + 1))));
    CHECK(reportsTooDeep(compileFieldTypeGen1("int" + repeated("??", limit / 2 + 1))));
    CHECK(reportsTooDeep(compileFieldTypeGen1("int" + repeated("?", limit - 1) + "??")));
    CHECK(reportsTooDeep(compileFieldTypeGen1(repeated("array<", limit + 1) + "int" + repeated(">", limit + 1))));
}

TEST_CASE("the parse ends at the first type nested too deeply") {
    const string deep = "int" + repeated("?", limit + 1);
    auto outcome = compileSource("options gen2\nstruct Deep {\n    x : " + deep + "\n    y : " + deep + "\n}\n");
    CHECK(outcome.tooDeepErrors == 1);
}

TEST_CASE("a pathologically deep type is one error, not a stack overflow") {
    auto outcome = compileFieldType("int" + repeated("?", 400000));
    CHECK(outcome.tooDeepErrors == 1);
}
