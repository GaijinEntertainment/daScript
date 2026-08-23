#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/simulate/interop.h"
#include "daScript/simulate/simulate_nodes.h"

using namespace das;

namespace {

int32_t  t_int    ( int32_t a )  { return a + 1; }
uint32_t t_uint   ( uint32_t a ) { return a + 2u; }
int64_t  t_int64  ( int64_t a )  { return a + 3; }
uint64_t t_uint64 ( uint64_t a ) { return a + 4ul; }
float    t_float  ( float a )    { return a + 0.5f; }
bool     t_bool   ( int32_t a )  { return a != 0; }
char *   t_ptr    ( int32_t a )  { return (char *) (uintptr_t) (0x1000 + a); }
int      g_void_hits = 0;
void     t_void   ( int32_t a )  { g_void_hits += a; }
float3   t_float3 ( float a )    { return float3(a, a + 1.0f, a + 2.0f); }
enum class E64 : uint64_t { A = 41 };
E64      t_enum64 ( int32_t a )  { return (E64)(uint64_t)(a); }

LineInfo g_at;

template <typename NodeT, typename... CtorArgs>
NodeT * make_call_node ( SimNode ** args, int nArgs, CtorArgs&&... ctorArgs ) {
    auto node = new NodeT(g_at, std::forward<CtorArgs>(ctorArgs)...);
    node->arguments = args;
    node->nArguments = nArgs;
    return node;
}

template <typename FN>
bool throws_wrong_slot ( Context & ctx, const FN & call ) {
    bool ok = ctx.runWithCatchAndClear([&]() { call(); });
    return !ok;
}

} // namespace

TEST_CASE("interop slot matrix: member-flavor extern nodes") {
    Context ctx(4 * 1024 * 1024);
    SimNode * argi[1] = { new SimNode_ConstValue(g_at, cast<int32_t>::from(41)) };
    SimNode * argf[1] = { new SimNode_ConstValue(g_at, cast<float>::from(1.5f)) };
    SimNode * argi64[1] = { new SimNode_ConstValue(g_at, cast<int64_t>::from(int64_t(39))) };

    SUBCASE("matching slots return the callee's value") {
        auto ni = make_call_node<SimNode_ExtFuncCall<decltype(&t_int)>>(argi, 1, "t_int", &t_int);
        CHECK(ni->evalInt(ctx) == 42);
        auto nf = make_call_node<SimNode_ExtFuncCall<decltype(&t_float)>>(argf, 1, "t_float", &t_float);
        CHECK(nf->evalFloat(ctx) == 2.0f);
        auto np = make_call_node<SimNode_ExtFuncCall<decltype(&t_ptr)>>(argi, 1, "t_ptr", &t_ptr);
        CHECK(np->evalPtr(ctx) == (char *)(uintptr_t)0x1029);
        auto nb = make_call_node<SimNode_ExtFuncCall<decltype(&t_bool)>>(argi, 1, "t_bool", &t_bool);
        CHECK(nb->evalBool(ctx) == true);
    }

    SUBCASE("eval() boxes every carrier") {
        auto ni = make_call_node<SimNode_ExtFuncCall<decltype(&t_int)>>(argi, 1, "t_int", &t_int);
        CHECK(cast<int32_t>::to(ni->eval(ctx)) == 42);
        auto nv = make_call_node<SimNode_ExtFuncCall<decltype(&t_float3)>>(argf, 1, "t_float3", &t_float3);
        float3 v = cast<float3>::to(nv->eval(ctx));
        CHECK(v.x == 1.5f);
        CHECK(v.z == 3.5f);
        g_void_hits = 0;
        auto nz = make_call_node<SimNode_ExtFuncCall<decltype(&t_void)>>(argi, 1, "t_void", &t_void);
        nz->eval(ctx);
        CHECK(g_void_hits == 41);
    }

    SUBCASE("signedness pairs forward, width mismatches throw") {
        auto nu = make_call_node<SimNode_ExtFuncCall<decltype(&t_uint)>>(argi, 1, "t_uint", &t_uint);
        CHECK(nu->evalInt(ctx) == 43);                 // uint -> Int (SimNode_At index shape)
        auto ni = make_call_node<SimNode_ExtFuncCall<decltype(&t_int)>>(argi, 1, "t_int", &t_int);
        CHECK(ni->evalUInt(ctx) == 42u);               // int -> UInt (enum-backed reads)
        auto n64 = make_call_node<SimNode_ExtFuncCall<decltype(&t_int64)>>(argi64, 1, "t_int64", &t_int64);
        CHECK(n64->evalUInt64(ctx) == 42ul);           // int64 -> UInt64
        auto nu64 = make_call_node<SimNode_ExtFuncCall<decltype(&t_uint64)>>(argi64, 1, "t_uint64", &t_uint64);
        CHECK(nu64->evalInt64(ctx) == 43);             // uint64 -> Int64
        CHECK(throws_wrong_slot(ctx, [&]{ ni->evalFloat(ctx); }));
        CHECK(throws_wrong_slot(ctx, [&]{ ni->evalInt64(ctx); }));
        CHECK(throws_wrong_slot(ctx, [&]{ nu->evalBool(ctx); }));
        CHECK(throws_wrong_slot(ctx, [&]{ n64->evalInt(ctx); }));
    }

    SUBCASE("boxed and void carriers refuse every typed slot without calling the callee") {
        auto nv = make_call_node<SimNode_ExtFuncCall<decltype(&t_float3)>>(argf, 1, "t_float3", &t_float3);
        CHECK(throws_wrong_slot(ctx, [&]{ nv->evalFloat(ctx); }));
        CHECK(throws_wrong_slot(ctx, [&]{ nv->evalInt(ctx); }));
        CHECK(throws_wrong_slot(ctx, [&]{ nv->evalPtr(ctx); }));
        g_void_hits = 0;
        auto nz = make_call_node<SimNode_ExtFuncCall<decltype(&t_void)>>(argi, 1, "t_void", &t_void);
        CHECK(throws_wrong_slot(ctx, [&]{ nz->evalInt(ctx); }));
        CHECK(g_void_hits == 0);
    }

    SUBCASE("8-byte enums carry in the int64 slot") {
        auto ne = make_call_node<SimNode_ExtFuncCall<decltype(&t_enum64)>>(argi, 1, "t_enum64", &t_enum64);
        CHECK(ne->evalInt64(ctx) == 41);
        CHECK(throws_wrong_slot(ctx, [&]{ ne->evalFloat(ctx); }));
    }

    SUBCASE("NTTP flavor matches the member flavor") {
        auto ni = make_call_node<SimNode_ExtFuncCallInline<decltype(&t_int), &t_int>>(argi, 1, "t_int");
        CHECK(ni->evalInt(ctx) == 42);
        CHECK(ni->evalUInt(ctx) == 42u);
        CHECK(cast<int32_t>::to(ni->eval(ctx)) == 42);
        CHECK(throws_wrong_slot(ctx, [&]{ ni->evalFloat(ctx); }));
    }
}
