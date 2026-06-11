// Stage 1b-ii of the tFixedArray rework (FIXED_ARRAY_REWORK.md): both grammars and the
// mangled-name parser now BUILD tFixedArray chains. The parse-shape suites stop the
// pipeline right after the parse leg — a deliberate trailing parse error, since infer
// can't digest FA chains until 1d — and assert the exact chains the grammar actions
// built (splice order, qualifier hoist, gen1 push-at-end quirk, alias-on-element).
// The mangled suite proves the new case '[' parse round-trips the 1a emit.
#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"
using namespace das;

namespace {

// parse-only: the trailing `)` is a parse error in both grammars, so compileDaScript
// returns right after the parse leg with the already-reduced module contents intact
ProgramPtr parseLiteral ( const char * src, const char * name ) {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    fAccess->setFileInfo(name, make_unique<TextFileInfo>(src, uint32_t(strlen(src)), /*own*/false));
    return compileDaScript(name, fAccess, tout, dummyLibGroup);
}

TypeDecl * fieldType ( const ProgramPtr & p, const char * structName, const char * fieldName ) {
    auto st = p->thisModule->findStructure(structName);
    REQUIRE(st != nullptr);
    auto fd = st->findField(fieldName);
    REQUIRE(fd != nullptr);
    REQUIRE(fd->type != nullptr);
    return fd->type;
}

// assert an FA chain of the given dims (outermost first) over the given element baseType
TypeDecl * checkChain ( TypeDecl * t, std::initializer_list<int32_t> dims, Type elemBt ) {
    for ( auto d : dims ) {
        REQUIRE(t != nullptr);
        REQUIRE(t->baseType == Type::tFixedArray);
        CHECK_EQ(t->fixedDim, d);
        t = t->firstType;
    }
    REQUIRE(t != nullptr);
    CHECK_EQ(t->baseType, elemBt);
    CHECK(t->baseType != Type::tFixedArray);
    return t;    // the element, for further checks
}

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

TypeDeclPtr reparse ( const string & mangled ) {
    ModuleLibrary lib;
    MangledNameParser parser;
    const char * ch = mangled.c_str();
    auto t = parser.parseTypeFromMangledName(ch, lib, nullptr);
    CHECK_EQ(*ch, 0);    // consumed the whole name
    return t;
}

}

TEST_CASE("gen2 grammar builds tFixedArray chains") {
    static const char src[] =
        "options gen2\n"
        "struct Foo {\n"
        "    f1 : int[3]\n"
        "    f2 : int[3][4]\n"
        "    f3 : int const [4]\n"
        "    f4 : int[4] const\n"
        "    f5 : int[]\n"
        "    f6 : int[SIZE]\n"
        "    f7 : I3[4]\n"
        "    f8 : int[3] const [4]\n"
        "    f9 : int[3][]\n"
        "}\n"
        "struct ZEnd {\n"
        "    z : int\n"
        "}\n"
        ")\n";
    auto p = parseLiteral(src, "fixed_array_parse_gen2.das");
    REQUIRE(p);
    REQUIRE(p->failed());    // the trailing `)` — proves infer never ran
    SUBCASE("single dim - const-int shortcut folds, expr retained") {
        auto t = fieldType(p,"Foo","f1");
        checkChain(t,{3},Type::tInt);
        REQUIRE(t->fixedDimExpr != nullptr);
        CHECK(t->fixedDimExpr->rtti_isConstant());
    }
    SUBCASE("multi dim - text order is outermost first") {
        checkChain(fieldType(p,"Foo","f2"),{3,4},Type::tInt);
    }
    SUBCASE("const hoists to the chain head, element stays bare") {
        auto h3 = fieldType(p,"Foo","f3");
        auto e3 = checkChain(h3,{4},Type::tInt);
        CHECK(bool(h3->constant));    // bool() — doctest binds operands by reference, bit-fields can't
        CHECK_FALSE(bool(e3->constant));
        auto h4 = fieldType(p,"Foo","f4");
        CHECK(bool(h4->constant));
        CHECK(h3->isSameType(*h4, RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes));
    }
    SUBCASE("[] is the dimAuto sentinel, no dim expr") {
        auto t = fieldType(p,"Foo","f5");
        checkChain(t,{TypeDecl::dimAuto},Type::tInt);
        CHECK(t->fixedDimExpr == nullptr);
    }
    SUBCASE("non-const dim expr is the dimConst sentinel, expr retained") {
        auto t = fieldType(p,"Foo","f6");
        checkChain(t,{TypeDecl::dimConst},Type::tInt);
        CHECK(t->fixedDimExpr != nullptr);
    }
    SUBCASE("alias never hoists - it is the unresolved element name") {
        auto t = fieldType(p,"Foo","f7");
        auto elem = checkChain(t,{4},Type::alias);
        CHECK(t->alias.empty());
        CHECK_EQ(elem->alias, "I3");
    }
    SUBCASE("late dim splices in FRONT (gen1 quirk preserved): int[3] const [4] is [4][3]") {
        auto t = fieldType(p,"Foo","f8");
        checkChain(t,{4,3},Type::tInt);
        CHECK(bool(t->constant));                 // hoisted from the inner chain head
        CHECK_FALSE(bool(t->firstType->constant));
    }
    SUBCASE("mixed dim_list [3][] keeps text order (gen2-only - error in gen1)") {
        checkChain(fieldType(p,"Foo","f9"),{3,TypeDecl::dimAuto},Type::tInt);
    }
}

TEST_CASE("gen1 grammar builds tFixedArray chains") {
    static const char src[] =
        "options gen2 = false\n"
        "struct Foo\n"
        "    f1 : int[3]\n"
        "    f2 : int[3][4]\n"
        "    f3 : int[][]\n"
        "    f4 : int[]\n"
        "    f5 : int const [3]\n"
        "    f6 : int[3] const [4]\n"
        "    f7 : int[][3]\n"
        "struct ZEnd\n"
        "    z : int\n"
        ")\n";
    auto p = parseLiteral(src, "fixed_array_parse_gen1.das");
    REQUIRE(p);
    REQUIRE(p->failed());
    SUBCASE("single and multi dim") {
        checkChain(fieldType(p,"Foo","f1"),{3},Type::tInt);
        checkChain(fieldType(p,"Foo","f2"),{3,4},Type::tInt);
    }
    SUBCASE("push-at-end quirk: the second [] of int[][] goes INNERMOST") {
        // (`int[3][]` is a syntax error in gen1 - dim_list shift preference - so the
        // is-already-FA branch of appendAutoDim is only reachable via repeated [])
        checkChain(fieldType(p,"Foo","f3"),{TypeDecl::dimAuto,TypeDecl::dimAuto},Type::tInt);
    }
    SUBCASE("bare [] wraps a plain type") {
        checkChain(fieldType(p,"Foo","f4"),{TypeDecl::dimAuto},Type::tInt);
    }
    SUBCASE("int[][3] splices the late dim in FRONT of the auto dim") {
        checkChain(fieldType(p,"Foo","f7"),{3,TypeDecl::dimAuto},Type::tInt);
    }
    SUBCASE("const hoists to the chain head") {
        auto t = fieldType(p,"Foo","f5");
        auto e = checkChain(t,{3},Type::tInt);
        CHECK(bool(t->constant));
        CHECK_FALSE(bool(e->constant));
    }
    SUBCASE("late dim splices in FRONT: int[3] const [4] is [4][3], const on head") {
        auto t = fieldType(p,"Foo","f6");
        checkChain(t,{4,3},Type::tInt);
        CHECK(bool(t->constant));
        CHECK_FALSE(bool(t->firstType->constant));
    }
}

TEST_CASE("gen1 {{ }} table literal synthesizes auto[] as a tFixedArray") {
    static const char src[] =
        "options gen2 = false\n"
        "def foo\n"
        "    let t <- {{ \"a\" => 1 }}\n"
        "struct ZEnd\n"
        "    z : int\n"
        ")\n";
    auto p = parseLiteral(src, "fixed_array_parse_gen1_tab.das");
    REQUIRE(p);
    REQUIRE(p->failed());
    Function * foo = nullptr;
    p->thisModule->functions.foreach([&](auto && fn){
        if ( fn->name=="foo" ) foo = fn;
    });
    REQUIRE(foo != nullptr);
    REQUIRE(foo->body != nullptr);
    REQUIRE(foo->body->rtti_isBlock());
    auto blk = static_cast<ExprBlock *>(foo->body);
    REQUIRE(!blk->list.empty());
    REQUIRE(blk->list[0]->rtti_isLet());
    auto let = static_cast<ExprLet *>(blk->list[0]);
    REQUIRE(!let->variables.empty());
    auto init = let->variables[0]->init;
    REQUIRE(init != nullptr);
    REQUIRE(init->rtti_isCallLikeExpr());
    auto call = static_cast<ExprLooksLikeCall *>(init);
    REQUIRE(!call->arguments.empty());
    REQUIRE(call->arguments[0]->rtti_isMakeArray());
    auto mka = static_cast<ExprMakeArray *>(call->arguments[0]);
    REQUIRE(mka->makeType != nullptr);
    checkChain(mka->makeType,{TypeDecl::dimAuto},Type::autoinfer);
}

TEST_CASE("mangled name parse builds tFixedArray and round-trips the emit") {
    gc_guard guard;
    SUBCASE("plain chains round-trip byte-identically") {
        for ( auto txt : { "[4]i", "[3][4]i", "[3][4][4]f" } ) {
            auto t = reparse(txt);
            CHECK_EQ(t->getMangledName(), txt);
        }
        auto t = reparse("[3][4]i");
        checkChain(t,{3,4},Type::tInt);
        CHECK(t->fixedDimExpr == nullptr);    // mangled text carries no exprs
        CHECK(t->isSameType(*makeFAChain(Type::tInt,{3,4}),
            RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes));
    }
    SUBCASE("sentinels round-trip") {
        auto a = reparse("[-1]i");
        checkChain(a,{TypeDecl::dimAuto},Type::tInt);
        CHECK_EQ(a->getMangledName(), "[-1]i");
        auto c = reparse("[-2]i");
        checkChain(c,{TypeDecl::dimConst},Type::tInt);
        CHECK_EQ(c->getMangledName(), "[-2]i");
    }
    SUBCASE("qualifier prefixes land on the FA head") {
        auto t = reparse("C&[4]i");
        checkChain(t,{4},Type::tInt);
        CHECK(bool(t->constant));
        CHECK(bool(t->ref));
        CHECK_EQ(t->getMangledName(), "C&[4]i");
    }
    SUBCASE("Y<> immediately after [d] labels THAT node - the [3][4]Y<M4>[4]f golden") {
        auto t = reparse("[3][4]Y<M4>[4]f");
        checkChain(t,{3,4,4},Type::tFloat);
        CHECK(t->alias.empty());
        CHECK_EQ(t->firstType->alias, "M4");
        CHECK(t->firstType->firstType->alias.empty());
        CHECK_EQ(t->getMangledName(), "[3][4]Y<M4>[4]f");
        // and the asymmetry: [3]Y<I>i re-parses with the label claimed by the FA node
        auto asym = reparse("[3]Y<I>i");
        CHECK_EQ(asym->alias, "I");
        CHECK(asym->firstType->alias.empty());
    }
    SUBCASE("standalone Y<> on a non-FA type is untouched") {
        auto t = reparse("Y<foo>i");
        CHECK_EQ(t->baseType, Type::tInt);
        CHECK_EQ(t->alias, "foo");
    }
    SUBCASE("fullName remove-suffixes round-trip on the FA node") {
        auto built = makeFAChain(Type::tInt,{3});
        built->removeDim = true;
        built->removeConstant = true;
        auto full = built->getMangledName(true);
        CHECK_EQ(full, "[3]-[]-Ci");
        auto t = reparse(full);
        CHECK(bool(t->removeDim));
        CHECK(bool(t->removeConstant));
        CHECK_EQ(t->getMangledName(true), full);
    }
    SUBCASE("FA nested in containers round-trips") {
        auto arr = new TypeDecl(Type::tArray);
        arr->firstType = makeFAChain(Type::tInt,{4});
        auto t = reparse(arr->getMangledName());
        CHECK(t->isSameType(*arr, RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes));
    }
}
