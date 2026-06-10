// Stage 1a of the tFixedArray rework (FIXED_ARRAY_REWORK.md): the structural
// Type::tFixedArray machinery is additive and nothing in the language produces FA nodes
// yet, so this suite hand-builds them and proves the new arms against equivalent
// old-style dim-vector nodes — text (describe/mangled/cpp), size family, identity,
// lifecycle, hashes, and classification parity.
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

TypeDecl * makeOldDim ( Type bt, std::initializer_list<int32_t> dims ) {
    auto t = new TypeDecl(bt);
    for ( auto d : dims ) t->dim.push_back(d);
    return t;
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

TEST_CASE("tFixedArray text matches dim-vector nodes") {
    gc_guard guard;
    SUBCASE("describe") {
        CHECK_EQ(makeFAChain(Type::tInt,{4})->describe(),
                 makeOldDim(Type::tInt,{4})->describe());
        CHECK_EQ(makeFAChain(Type::tFloat,{3,4,4})->describe(),
                 makeOldDim(Type::tFloat,{3,4,4})->describe());
        CHECK_EQ(makeFAChain(Type::tFloat,{3,4,4})->describe(), "float[3][4][4]");
        auto faAuto = makeFAChain(Type::tInt,{TypeDecl::dimAuto});
        auto oldAuto = makeOldDim(Type::tInt,{TypeDecl::dimAuto});
        CHECK_EQ(faAuto->describe(), oldAuto->describe());
        CHECK_EQ(faAuto->describe(), "int[]");
        auto faConst = makeFAChain(Type::tInt,{4});
        faConst->constant = true;
        auto oldConst = makeOldDim(Type::tInt,{4});
        oldConst->constant = true;
        CHECK_EQ(faConst->describe(), oldConst->describe());
        CHECK_EQ(faConst->describe(), "int const[4]");
    }
    SUBCASE("mangled name") {
        CHECK_EQ(makeFAChain(Type::tInt,{4})->getMangledName(),
                 makeOldDim(Type::tInt,{4})->getMangledName());
        CHECK_EQ(makeFAChain(Type::tInt,{4})->getMangledName(), "[4]i");
        CHECK_EQ(makeFAChain(Type::tFloat,{3,4,4})->getMangledName(),
                 makeOldDim(Type::tFloat,{3,4,4})->getMangledName());
        CHECK_EQ(makeFAChain(Type::tFloat,{3,4,4})->getMangledName(), "[3][4][4]f");
        auto faQ = makeFAChain(Type::tInt,{4});
        faQ->constant = true;
        faQ->ref = true;
        auto oldQ = makeOldDim(Type::tInt,{4});
        oldQ->constant = true;
        oldQ->ref = true;
        CHECK_EQ(faQ->getMangledName(), oldQ->getMangledName());
        CHECK_EQ(faQ->getMangledName(), "C&[4]i");
        CHECK_EQ(makeFAChain(Type::tInt,{4})->getMangledNameHash(),
                 makeOldDim(Type::tInt,{4})->getMangledNameHash());
    }
    SUBCASE("mid-chain alias mangles per level (settled: natural recursion)") {
        auto m4 = makeFAChain(Type::tFloat,{4,4});
        m4->alias = "M4";
        auto m4x3 = makeFA(3, m4);
        CHECK_EQ(m4x3->getMangledName(), "[3][4]Y<M4>[4]f");
    }
    SUBCASE("describeCppType") {
        CHECK_EQ(describeCppType(makeFAChain(Type::tInt,{3,4})),
                 describeCppType(makeOldDim(Type::tInt,{3,4})));
        CHECK_EQ(describeCppType(makeFAChain(Type::tInt,{3,4})), "TDim<TDim<int32_t,4>,3>");
    }
}

TEST_CASE("tFixedArray size family matches dim-vector nodes") {
    gc_guard guard;
    auto fa = makeFAChain(Type::tFloat,{3,4,4});
    auto old = makeOldDim(Type::tFloat,{3,4,4});
    CHECK_EQ(fa->getSizeOf64(), old->getSizeOf64());
    CHECK_EQ(fa->getSizeOf64(), uint64_t(192));
    CHECK_EQ(fa->getCountOf64(), old->getCountOf64());
    CHECK_EQ(fa->getCountOf64(), uint64_t(48));
    CHECK_EQ(fa->getStride64(), old->getStride64());
    CHECK_EQ(fa->getStride64(), uint64_t(64));
    CHECK_EQ(fa->getBaseSizeOf64(), old->getBaseSizeOf64());
    CHECK_EQ(fa->getBaseSizeOf64(), uint64_t(4));
    CHECK_EQ(fa->getAlignOf(), old->getAlignOf());
    SUBCASE("float3[4] stays 48 bytes (memory matters)") {
        auto fa3 = makeFAChain(Type::tFloat3,{4});
        auto old3 = makeOldDim(Type::tFloat3,{4});
        CHECK_EQ(fa3->getSizeOf64(), old3->getSizeOf64());
        CHECK_EQ(fa3->getSizeOf64(), uint64_t(48));
        CHECK_EQ(fa3->getAlignOf(), old3->getAlignOf());
    }
    SUBCASE("unresolved dims fail sizeof") {
        bool faFailed = false, oldFailed = false;
        makeFAChain(Type::tInt,{TypeDecl::dimAuto})->getSizeOf64(faFailed);
        makeOldDim(Type::tInt,{TypeDecl::dimAuto})->getSizeOf64(oldFailed);
        CHECK(faFailed);
        CHECK_EQ(faFailed, oldFailed);
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
    auto oldI4 = makeOldDim(Type::tInt,{4});
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
    // the two representations never meet within one build; structural inequality is by design
    CHECK_FALSE(same(i4a,oldI4));
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

TEST_CASE("tFixedArray classification parity with dim-vector nodes") {
    gc_guard guard;
    auto fa = makeFAChain(Type::tInt,{4});
    auto old = makeOldDim(Type::tInt,{4});
    CHECK(fa->isArray());
    CHECK_EQ(fa->isArray(), old->isArray());
    CHECK(fa->isFixedArray());
    CHECK_EQ(fa->isRefType(), old->isRefType());
    CHECK_EQ(fa->canCopy(), old->canCopy());
    CHECK_EQ(fa->canMove(), old->canMove());
    CHECK_EQ(fa->canClone(), old->canClone());
    CHECK_EQ(fa->isPod(), old->isPod());
    CHECK_EQ(fa->isRawPod(), old->isRawPod());
    CHECK_EQ(fa->isNoHeapType(), old->isNoHeapType());
    CHECK_EQ(fa->isLocal(), old->isLocal());
    CHECK_EQ(fa->isShareable(), old->isShareable());
    CHECK_EQ(fa->isWorkhorseType(), old->isWorkhorseType());
    CHECK_EQ(fa->isFoldable(), old->isFoldable());
    CHECK_EQ(fa->canInitWithZero(), old->canInitWithZero());
    CHECK_EQ(fa->isPolicyType(), old->isPolicyType());
    CHECK_EQ(fa->isVecPolicyType(), old->isVecPolicyType());
    CHECK_EQ(fa->unsafeInit(), old->unsafeInit());
    CHECK_EQ(fa->needInScope(), old->needInScope());
    CHECK_EQ(fa->gcFlags(), old->gcFlags());
    CHECK_EQ(fa->isAuto(), old->isAuto());
    CHECK_EQ(fa->isAutoOrAlias(), old->isAutoOrAlias());
    CHECK_EQ(fa->isAutoArrayResolved(), old->isAutoArrayResolved());
    CHECK_EQ(fa->isExprType(), old->isExprType());
    SUBCASE("string element carries string gc flags through the FA level") {
        auto faS = makeFAChain(Type::tString,{4});
        auto oldS = makeOldDim(Type::tString,{4});
        CHECK_EQ(faS->gcFlags(), oldS->gcFlags());
        CHECK_EQ(faS->hasStringData(), oldS->hasStringData());
        CHECK(faS->hasStringData());
    }
    SUBCASE("auto dims register as auto") {
        auto faA = makeFAChain(Type::tInt,{TypeDecl::dimAuto});
        auto oldA = makeOldDim(Type::tInt,{TypeDecl::dimAuto});
        CHECK_EQ(faA->isAuto(), oldA->isAuto());
        CHECK(faA->isAuto());
        CHECK_EQ(faA->isAutoArrayResolved(), oldA->isAutoArrayResolved());
        CHECK_FALSE(faA->isAutoArrayResolved());
        auto faElemAuto = makeFA(4, new TypeDecl(Type::autoinfer));
        CHECK(faElemAuto->isAuto());
    }
    SUBCASE("dimConst registers as expression type") {
        auto faC = makeFAChain(Type::tInt,{TypeDecl::dimConst});
        auto oldC = makeOldDim(Type::tInt,{TypeDecl::dimConst});
        CHECK_EQ(faC->isExprType(), oldC->isExprType());
        CHECK(faC->isExprType());
        CHECK_EQ(faC->isAliasOrExpr(), oldC->isAliasOrExpr());
    }
}
