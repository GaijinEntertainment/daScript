// isFullySealed must test secondType where it checks secondType — the original
// code re-tested firstType (copy-paste, flagged by PVS V522), so a sealed key
// type masked an unsealed value type (e.g. table<int; auto>).
#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/ast/ast.h"
using namespace das;

TEST_CASE("isFullySealed checks secondType independently of firstType") {
    gc_guard guard;
    SUBCASE("sealed key, unsealed value") {
        auto t = new TypeDecl(Type::tTable, cppBindingLineInfo());
        t->firstType = new TypeDecl(Type::tInt, cppBindingLineInfo());
        t->secondType = new TypeDecl(Type::autoinfer, cppBindingLineInfo());
        CHECK_FALSE(t->isFullySealed());
    }
    SUBCASE("unsealed key, sealed value") {
        auto t = new TypeDecl(Type::tTable, cppBindingLineInfo());
        t->firstType = new TypeDecl(Type::autoinfer, cppBindingLineInfo());
        t->secondType = new TypeDecl(Type::tFloat, cppBindingLineInfo());
        CHECK_FALSE(t->isFullySealed());
    }
    SUBCASE("both sealed") {
        auto t = new TypeDecl(Type::tTable, cppBindingLineInfo());
        t->firstType = new TypeDecl(Type::tInt, cppBindingLineInfo());
        t->secondType = new TypeDecl(Type::tFloat, cppBindingLineInfo());
        CHECK(t->isFullySealed());
    }
}
