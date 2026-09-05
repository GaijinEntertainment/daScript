// KeyHash (runtime_table.h) is the hash AOT's TTable and every table grow use; the interpreter's
// table nodes, the JIT helpers, the JSON scanner and the C API hash a builtin key as itself and a
// handled key as its annotation's workhorse. Each cell pins that KeyHash agrees, per key type - the
// two rails disagreeing is a table that loses its keys at the first grow (ARCHITECTURE.md,
// "Table key hashing").

#include <doctest/doctest.h>

#include "daScript/daScript.h"
#include "daScript/simulate/runtime_table.h"
#include "daScript/simulate/jit_abi.h"

// two handled value types the way a module declares them: a 12-byte vector whose workhorse is
// vec4f (the shape of dasUnitTest's Point3 and dasImgui's ImVec2) and a 4-byte id whose workhorse
// is int32 (dasUnitTest's EntityId)
struct NarrowVec { float x, y, z; };
struct IdHandle { int32_t value; };

namespace das {
    template <> struct WrapType<NarrowVec> { enum { value = true }; typedef vec4f type; typedef vec4f rettype; };
    template <> struct cast<NarrowVec> : cast_fVec<NarrowVec> {};
    template <> struct WrapType<IdHandle> { enum { value = true }; typedef int32_t type; typedef int32_t rettype; };
    template <> struct cast<IdHandle> {
        static __forceinline IdHandle to ( vec4f x ) { IdHandle id; id.value = v_extract_xi(v_cast_vec4i(x)); return id; }
        static __forceinline vec4f from ( IdHandle x ) { return v_cast_vec4f(v_seti_x(x.value)); }
    };
}

using namespace das;

namespace {

template <typename T>
void check_builtin ( Context & ctx, const T & key ) {
    CHECK_EQ(KeyHash<T>()(ctx, key), hash_function(ctx, key));
}

template <typename T>
uint64_t workhorse_hash ( Context & ctx, const T & key ) {
    using workhorse = typename WrapType<T>::type;
    return hash_function(ctx, cast<workhorse>::to(cast<T>::from(key)));
}

}

TEST_CASE("a builtin table key hashes as itself through KeyHash") {
    Context ctx;
    check_builtin<bool>(ctx, true);
    check_builtin<int8_t>(ctx, int8_t(-3));
    check_builtin<uint8_t>(ctx, uint8_t(250));
    check_builtin<int16_t>(ctx, int16_t(-1234));
    check_builtin<uint16_t>(ctx, uint16_t(65000));
    check_builtin<int32_t>(ctx, int32_t(-7));
    check_builtin<uint32_t>(ctx, uint32_t(0xdeadbeefu));
    check_builtin<int64_t>(ctx, int64_t(-1) << 40);
    check_builtin<uint64_t>(ctx, uint64_t(1) << 63);
    check_builtin<float>(ctx, 1.5f);
    check_builtin<double>(ctx, 3.25);
    Bitfield bf; bf.value = 5u;
    check_builtin(ctx, bf);
    Bitfield8 bf8; bf8.value = 5u;
    check_builtin(ctx, bf8);
    Bitfield16 bf16; bf16.value = 5u;
    check_builtin(ctx, bf16);
    Bitfield64 bf64; bf64.value = 5ull;
    check_builtin(ctx, bf64);
    check_builtin(ctx, int2(1, 2));
    check_builtin(ctx, int3(1, 2, 3));
    check_builtin(ctx, int4(1, 2, 3, 4));
    check_builtin(ctx, uint2(1u, 2u));
    check_builtin(ctx, uint3(1u, 2u, 3u));
    check_builtin(ctx, uint4(1u, 2u, 3u, 4u));
    check_builtin(ctx, float2(1.f, 2.f));
    check_builtin(ctx, float3(1.f, 2.f, 3.f));
    check_builtin(ctx, float4(1.f, 2.f, 3.f, 4.f));
    check_builtin(ctx, range(3, 9));
    check_builtin(ctx, urange(3u, 9u));
    range64 r64; r64.from = 3; r64.to = 9;
    check_builtin(ctx, r64);
    urange64 ur64; ur64.from = 3; ur64.to = 9;
    check_builtin(ctx, ur64);
    char * str = (char *) "key";
    check_builtin(ctx, str);
    void * ptr = &ctx;
    check_builtin(ctx, ptr);
}

TEST_CASE("a builtin vector or range key does not take the workhorse detour") {
    // the detour hashes a vec4f's 16 bytes; the node hashes the key's own 8 or 12
    Context ctx;
    CHECK_NE(KeyHash<int2>()(ctx, int2(1, 2)), workhorse_hash(ctx, int2(1, 2)));
    CHECK_NE(KeyHash<float3>()(ctx, float3(1.f, 2.f, 3.f)), workhorse_hash(ctx, float3(1.f, 2.f, 3.f)));
    CHECK_NE(KeyHash<range>()(ctx, range(3, 9)), workhorse_hash(ctx, range(3, 9)));
}

TEST_CASE("a handled table key hashes as its workhorse through KeyHash") {
    Context ctx;
    NarrowVec nv { 1.f, 2.f, 3.f };
    CHECK_EQ(KeyHash<NarrowVec>()(ctx, nv), workhorse_hash(ctx, nv));
    CHECK_NE(KeyHash<NarrowVec>()(ctx, nv), hash_function(ctx, nv));   // not its 12 raw bytes
    IdHandle id { 77 };
    CHECK_EQ(KeyHash<IdHandle>()(ctx, id), hash_function(ctx, int32_t(77)));
}
