// Stage 1c of the tFixedArray rework (FIXED_ARRAY_REWORK.md): typeFactory / interop
// produce tFixedArray chains instead of dim vectors, and makeTypeInfo flattens FA chains
// into the (forever-flattened) runtime TypeInfo. Includes the settled multi-dim fix:
// C int[3][4] maps to FA(3, FA(4, int)) — the old dim-vector push produced the
// inner-first order, a latent stride bug.
#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/ast/ast.h"
using namespace das;

namespace {

TypeDecl * makeOldDim ( Type bt, std::initializer_list<int32_t> dims ) {
    auto t = new TypeDecl(bt);
    for ( auto d : dims ) t->dim.push_back(d);
    return t;
}

}

TEST_CASE("typeFactory produces tFixedArray chains") {
    gc_guard guard;
    ModuleLibrary lib;
    SUBCASE("native C array - isNativeDim on the FA node, non-ref") {
        auto t = typeFactory<int[4]>::make(lib);
        REQUIRE(t->baseType == Type::tFixedArray);
        CHECK_EQ(t->fixedDim, 4);
        CHECK(t->isNativeDim);
        CHECK_FALSE(t->ref);
        REQUIRE(t->firstType != nullptr);
        CHECK_EQ(t->firstType->baseType, Type::tInt);
        CHECK_EQ(t->describe(), "int[4]");
    }
    SUBCASE("multi-dim C array maps outermost-first (the settled fix)") {
        auto t = typeFactory<int[3][4]>::make(lib);
        REQUIRE(t->baseType == Type::tFixedArray);
        CHECK_EQ(t->fixedDim, 3);
        REQUIRE(t->firstType != nullptr);
        REQUIRE(t->firstType->baseType == Type::tFixedArray);
        CHECK_EQ(t->firstType->fixedDim, 4);
        CHECK_EQ(t->firstType->firstType->baseType, Type::tInt);
        CHECK_EQ(t->describe(), "int[3][4]");
        CHECK_EQ(t->getSizeOf64(), uint64_t(48));
        CHECK_EQ(t->getStride64(), uint64_t(16));   // row stride of int[4] — was 12 pre-fix
    }
    SUBCASE("TDim<> - das-side fixed array, no isNativeDim") {
        auto t = typeFactory<TDim<int32_t,4>>::make(lib);
        REQUIRE(t->baseType == Type::tFixedArray);
        CHECK_EQ(t->fixedDim, 4);
        CHECK_FALSE(t->isNativeDim);
        CHECK_EQ(t->firstType->baseType, Type::tInt);
        auto nested = typeFactory<TDim<TDim<int32_t,4>,3>>::make(lib);
        CHECK_EQ(nested->describe(), "int[3][4]");
    }
    SUBCASE("const element hoists to the FA head (canonical form)") {
        auto elem = new TypeDecl(Type::tInt);
        elem->constant = true;
        auto t = makeFixedArrayTypeDecl(4, elem);
        CHECK(t->constant);
        CHECK_FALSE(t->firstType->constant);
    }
    SUBCASE("makeArgumentType composes through the FA-aware classifiers") {
        auto t = makeArgumentType<int[4]>(lib);
        REQUIRE(t->baseType == Type::tFixedArray);
        CHECK_FALSE(t->ref);          // isRefType() branch clears ref, skips constant
        CHECK_FALSE(t->constant);
    }
}

TEST_CASE("makeTypeInfo flattens tFixedArray chains byte-equal to dim-vector input") {
    gc_guard guard;
    ModuleLibrary lib;
    SUBCASE("flattened TypeInfo parity") {
        DebugInfoHelper faHelp, oldHelp;
        auto fa = typeFactory<int[3][4]>::make(lib);
        auto faInfo = faHelp.makeTypeInfo(nullptr, fa);
        auto oldInfo = oldHelp.makeTypeInfo(nullptr, makeOldDim(Type::tInt,{3,4}));
        CHECK_EQ(faInfo->type, Type::tInt);
        REQUIRE_EQ(faInfo->dimSize, oldInfo->dimSize);
        REQUIRE_EQ(faInfo->dimSize, 2u);
        CHECK_EQ(faInfo->dim[0], 3u);
        CHECK_EQ(faInfo->dim[1], 4u);
        CHECK_EQ(faInfo->size, oldInfo->size);
        CHECK_EQ(faInfo->flags, oldInfo->flags);
        CHECK_EQ(faInfo->hash, oldInfo->hash);
    }
    SUBCASE("head qualifiers ride into the flattened flags") {
        DebugInfoHelper faHelp, oldHelp;
        auto fa = makeFixedArrayTypeDecl(4, new TypeDecl(Type::tInt));
        fa->constant = true;
        auto faInfo = faHelp.makeTypeInfo(nullptr, fa);
        auto old = makeOldDim(Type::tInt,{4});
        old->constant = true;
        auto oldInfo = oldHelp.makeTypeInfo(nullptr, old);
        CHECK((faInfo->flags & TypeInfo::flag_isConst) != 0u);
        CHECK_EQ(faInfo->flags, oldInfo->flags);
        CHECK_EQ(faInfo->hash, oldInfo->hash);
    }
    SUBCASE("cache key is shared - chain and flattened mangled text are identical") {
        DebugInfoHelper help;
        auto a = help.makeTypeInfo(nullptr, typeFactory<int[4]>::make(lib));
        auto b = help.makeTypeInfo(nullptr, makeOldDim(Type::tInt,{4}));
        CHECK_EQ(a, b);   // same helper, same mangled key, same TypeInfo node
    }
}
