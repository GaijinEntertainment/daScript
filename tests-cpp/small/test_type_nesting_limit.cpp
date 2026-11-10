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

ProgramPtr compileFieldType ( const string & fieldType ) {
    const string src = "options gen2\nstruct Deep {\n    x : " + fieldType + "\n}\n";
    TextWriter tout;
    ModuleGroup libGroup;
    auto fa = make_smart<FsFileAccess>();
    fa->setFileInfo("type_nesting_limit.das", make_unique<TextFileInfo>(src.c_str(), uint32_t(src.size()), false));
    return compileDaScript("type_nesting_limit.das", fa, tout, libGroup);
}

bool reportsTooDeep ( const ProgramPtr & program ) {
    for ( auto & err : program->errors ) {
        if ( err.cerr == CompilationError::exceeds_type_nesting ) return true;
    }
    return false;
}

}

TEST_CASE("a type nested to the limit compiles, one level past it is a parse error") {
    SUBCASE("pointer levels") {
        CHECK_FALSE(compileFieldType("int" + repeated("?", 256))->failed());
        CHECK(reportsTooDeep(compileFieldType("int" + repeated("?", 257))));
    }
    SUBCASE("double pointer tokens") {
        CHECK_FALSE(compileFieldType("int" + repeated("??", 128))->failed());
        CHECK(reportsTooDeep(compileFieldType("int" + repeated("??", 129))));
    }
    SUBCASE("angle bracket levels") {
        CHECK_FALSE(compileFieldType(repeated("array<", 256) + "int" + repeated(">", 256))->failed());
        CHECK(reportsTooDeep(compileFieldType(repeated("array<", 257) + "int" + repeated(">", 257))));
        CHECK(reportsTooDeep(compileFieldType(repeated("function<():", 257) + "int" + repeated(">", 257))));
    }
    SUBCASE("pointer levels count the nesting under them") {
        CHECK_FALSE(compileFieldType("tuple<int;int" + repeated("?", 200) + ">")->failed());
        CHECK(reportsTooDeep(compileFieldType("tuple<int;tuple<int;int" + repeated("?", 200) + ">" + repeated("?", 200) + ">")));
    }
}

TEST_CASE("a pathologically deep type is one error, not a stack overflow") {
    auto program = compileFieldType("int" + repeated("?", 400000));
    CHECK(reportsTooDeep(program));
    CHECK(program->errors.size() == 1);
}
