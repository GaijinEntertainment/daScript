// Structural Type::tFixedArray machinery (FIXED_ARRAY_REWORK.md): text
// (describe/mangled/cpp), size family, identity, lifecycle, hashes, and
// classification. Originally a parity suite against equivalent dim-vector nodes;
// those fields are gone, so the expected values are pinned as literals.
#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"
using namespace das;

namespace {

TypeDecl * makeFA ( int32_t d, TypeDecl * elem ) {
    auto fa = new TypeDecl(Type::tFixedArray);
    fa->fixedDim = d;
    fa->firstType = elem;
    return fa;
}

TypeDecl * makeFAChain ( Type bt, std::initializer_list<int32_t> dims ) {
    auto t = new TypeDecl(bt);
    TypeDecl * result = t;
    for ( auto it = rbegin(dims); it != rend(dims); ++it ) {
        result = makeFA(*it, result);
    }
    return result;
}

}

TEST_CASE("tFixedArray text") {
    gc_guard guard;
    SUBCASE("describe") {
        CHECK_EQ(makeFAChain(Type::tInt,{4})->describe(), "int[4]");
        CHECK_EQ(makeFAChain(Type::tFloat,{3,4,4})->describe(), "float[3][4][4]");
        CHECK_EQ(makeFAChain(Type::tInt,{TypeDecl::dimAuto})->describe(), "int[]");
        auto faConst = makeFAChain(Type::tInt,{4});
        faConst->constant = true;
        CHECK_EQ(faConst->describe(), "int const[4]");
    }
    SUBCASE("mangled name") {
        CHECK_EQ(makeFAChain(Type::tInt,{4})->getMangledName(), "[4]i");
        CHECK_EQ(makeFAChain(Type::tFloat,{3,4,4})->getMangledName(), "[3][4][4]f");
        auto faQ = makeFAChain(Type::tInt,{4});
        faQ->constant = true;
        faQ->ref = true;
        CHECK_EQ(faQ->getMangledName(), "C&[4]i");
    }
    SUBCASE("mid-chain alias mangles per level (settled: natural recursion)") {
        auto m4 = makeFAChain(Type::tFloat,{4,4});
        m4->alias = "M4";
        auto m4x3 = makeFA(3, m4);
        CHECK_EQ(m4x3->getMangledName(), "[3][4]Y<M4>[4]f");
    }
    SUBCASE("describeCppType") {
        CHECK_EQ(describeCppType(makeFAChain(Type::tInt,{3,4})), "TDim<TDim<int32_t,4>,3>");
    }
}

TEST_CASE("tFixedArray size family") {
    gc_guard guard;
    auto fa = makeFAChain(Type::tFloat,{3,4,4});
    CHECK_EQ(fa->getSizeOf64(), uint64_t(192));
    CHECK_EQ(fa->getCountOf64(), uint64_t(48));
    CHECK_EQ(fa->getStride64(), uint64_t(64));
    CHECK_EQ(fa->getBaseSizeOf64(), uint64_t(4));
    CHECK_EQ(fa->getAlignOf(), 4);
    SUBCASE("float3[4] stays 48 bytes (memory matters)") {
        auto fa3 = makeFAChain(Type::tFloat3,{4});
        CHECK_EQ(fa3->getSizeOf64(), uint64_t(48));
        CHECK_EQ(fa3->getAlignOf(), 4);
    }
    SUBCASE("unresolved dims fail sizeof") {
        bool faFailed = false;
        makeFAChain(Type::tInt,{TypeDecl::dimAuto})->getSizeOf64(faFailed);
        CHECK(faFailed);
    }
}

TEST_CASE("tFixedArray identity") {
    gc_guard guard;
    auto i4a = makeFAChain(Type::tInt,{4});
    auto i4b = makeFAChain(Type::tInt,{4});
    auto i3  = makeFAChain(Type::tInt,{3});
    auto f44a = makeFAChain(Type::tFloat,{4,4});
    auto f44b = makeFAChain(Type::tFloat,{4,4});
    auto f34  = makeFAChain(Type::tFloat,{3,4});
    auto justInt = new TypeDecl(Type::tInt);
    auto arrInt = new TypeDecl(Type::tArray);
    arrInt->firstType = new TypeDecl(Type::tInt);
    auto same = [](TypeDecl * a, TypeDecl * b) {
        return a->isSameType(*b, RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes);
    };
    CHECK(same(i4a,i4b));
    CHECK(same(f44a,f44b));
    CHECK_FALSE(same(i4a,i3));
    CHECK_FALSE(same(f44a,f34));
    CHECK_FALSE(same(i4a,justInt));
    CHECK_FALSE(same(i4a,arrInt));
    CHECK(i4a->isSameExactType(*i4b));
    CHECK_FALSE(i4a->isSameExactType(*i3));
    SUBCASE("outer const participates per constMatters") {
        auto cFA = makeFAChain(Type::tInt,{4});
        cFA->constant = true;
        CHECK_FALSE(same(cFA,i4a));
        CHECK(cFA->isSameType(*i4a, RefMatters::no, ConstMatters::no, TemporaryMatters::no));
    }
}

TEST_CASE("tFixedArray lifecycle and hashes") {
    gc_guard guard;
    SUBCASE("copy constructor deep-copies the new fields") {
        auto src = makeFA(TypeDecl::dimConst, new TypeDecl(Type::tInt));
        src->fixedDimExpr = new ExprConstInt(7);
        auto copy = new TypeDecl(*src);
        CHECK_EQ(copy->fixedDim, TypeDecl::dimConst);
        REQUIRE(copy->fixedDimExpr != nullptr);
        CHECK(copy->fixedDimExpr != src->fixedDimExpr);
        REQUIRE(copy->firstType != nullptr);
        CHECK_EQ(copy->firstType->baseType, Type::tInt);
    }
    SUBCASE("static clone deep-copies the new fields") {
        auto src = makeFA(TypeDecl::dimConst, new TypeDecl(Type::tInt));
        src->fixedDimExpr = new ExprConstInt(7);
        TypeDeclPtr dest = nullptr;
        TypeDecl::clone(dest, src);
        REQUIRE(dest != nullptr);
        CHECK_EQ(dest->fixedDim, TypeDecl::dimConst);
        REQUIRE(dest->fixedDimExpr != nullptr);
        CHECK(dest->fixedDimExpr != src->fixedDimExpr);
    }
    SUBCASE("hashes discriminate size, rank, and element") {
        auto i4 = makeFAChain(Type::tInt,{4});
        auto i3 = makeFAChain(Type::tInt,{3});
        auto f4 = makeFAChain(Type::tFloat,{4});
        auto i44 = makeFAChain(Type::tInt,{4,4});
        auto justInt = new TypeDecl(Type::tInt);
        CHECK_NE(i4->getSemanticHash(), i3->getSemanticHash());
        CHECK_NE(i4->getSemanticHash(), f4->getSemanticHash());
        CHECK_NE(i4->getSemanticHash(), i44->getSemanticHash());
        CHECK_NE(i4->getSemanticHash(), justInt->getSemanticHash());
        auto lookup = [](TypeDecl * t) { uint64_t h = 0; t->getLookupHash(h); return h; };
        CHECK_NE(lookup(i4), lookup(i3));
        CHECK_NE(lookup(i4), lookup(i44));
        CHECK_NE(lookup(i4), lookup(justInt));
    }
}

TEST_CASE("tFixedArray classification") {
    gc_guard guard;
    auto fa = makeFAChain(Type::tInt,{4});
    CHECK(fa->isArray());
    CHECK(fa->isFixedArray());
    CHECK(fa->isRefType());
    CHECK(fa->canCopy());
    CHECK(fa->canMove());
    CHECK(fa->canClone());
    CHECK(fa->isPod());
    CHECK(fa->isRawPod());
    CHECK(fa->isNoHeapType());
    CHECK(fa->isLocal());
    CHECK(fa->isShareable());
    CHECK_FALSE(fa->isWorkhorseType());
    CHECK_FALSE(fa->isFoldable());
    CHECK_FALSE(fa->canInitWithZero());
    CHECK(fa->isPolicyType());
    CHECK_FALSE(fa->isVecPolicyType());
    CHECK_FALSE(fa->unsafeInit());
    CHECK_FALSE(fa->needInScope());
    CHECK_EQ(fa->gcFlags(), 0u);
    CHECK_FALSE(fa->isAuto());
    CHECK_FALSE(fa->isAutoOrAlias());
    CHECK(fa->isAutoArrayResolved());
    CHECK_FALSE(fa->isExprType());
    SUBCASE("string element carries string gc flags through the FA level") {
        auto faS = makeFAChain(Type::tString,{4});
        CHECK((faS->gcFlags() & TypeDecl::gcFlag_stringHeap) != 0u);
        CHECK(faS->hasStringData());
    }
    SUBCASE("auto dims register as auto") {
        auto faA = makeFAChain(Type::tInt,{TypeDecl::dimAuto});
        CHECK(faA->isAuto());
        CHECK_FALSE(faA->isAutoArrayResolved());
        auto faElemAuto = makeFA(4, new TypeDecl(Type::autoinfer));
        CHECK(faElemAuto->isAuto());
    }
    SUBCASE("dimConst registers as expression type") {
        auto faC = makeFAChain(Type::tInt,{TypeDecl::dimConst});
        CHECK(faC->isExprType());
        CHECK(faC->isAliasOrExpr());
    }
}
