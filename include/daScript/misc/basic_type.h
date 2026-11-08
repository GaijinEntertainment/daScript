#pragma once

#include <cstdint>
#include <limits.h> // ULONG_MAX
#include <functional>
#include <type_traits>

#include "daScript/misc/platform.h"
#include "daScript/misc/vectypes.h"
#include "daScript/misc/rangetype.h"

namespace das {

    enum Type : int32_t {
        none,
        autoinfer,
        alias,
        option,
        typeDecl,
        typeMacro,
        fakeContext,
        fakeLineInfo,
        anyArgument,
        tVoid,
        tBool,
        tInt8,
        tUInt8,
        tInt16,
        tUInt16,
        tInt64,
        tUInt64,
        tInt,
        tInt2,
        tInt3,
        tInt4,
        tUInt,
        tUInt2,
        tUInt3,
        tUInt4,
        tFloat,
        tFloat2,
        tFloat3,
        tFloat4,
        tDouble,
        tRange,
        tURange,
        tRange64,
        tURange64,
        tString,
        tStructure,
        tHandle,
        tEnumeration,
        tEnumeration8,
        tEnumeration16,
        tEnumeration64,
        tBitfield,
        tBitfield8,
        tBitfield16,
        tBitfield64,
        tPointer,
        tFunction,
        tLambda,
        tIterator,
        tArray,
        tTable,
        tBlock,
        tTuple,
        tVariant,
        tFixedArray,    // AST-only (FIXED_ARRAY_REWORK.md): structural fixed-array TypeDecl node.
                        //  Runtime TypeInfo never carries it — fixed arrays stay flattened to dim[]
                        //  at the single AST->TypeInfo conversion point (ast_debug_info_helper.cpp).
        tDistinct,      // AST-only: nominal `distinct Foo = int` type. ABI-identical to firstType
                        //  (the underlying workhorse type); annotation points at DistinctTypeAnnotation.
                        //  Erased to the underlying type at the same AST->TypeInfo conversion point.
        // ===== 16/8-bit type lattice — appended (NEVER insert above: das_base_type in
        // daScriptC.h pins the numeric values of everything before this point). Vectors are
        // byte-packed in the low bytes of the vec4f slot; scalars ride widened in lane 0.
        tFloat16,       // IEEE-754 binary16 scalar (das name "float16"; "half" is a builtin alias)
        tHalf2,         // 16-bit fp vectors: {2,3,4,8} lanes, 8-lane form fills the 128-bit slot
        tHalf3,
        tHalf4,
        tHalf8,
        tShort2,        // int16 vectors (storage + convert only, no closed arithmetic)
        tShort3,
        tShort4,
        tShort8,
        tUShort2,       // uint16 vectors
        tUShort3,
        tUShort4,
        tUShort8,
        tByte2,         // int8 vectors — byte = SIGNED int8 (house naming: bare=signed)
        tByte3,
        tByte4,
        tByte8,
        tByte16,
        tUByte2,        // uint8 vectors
        tUByte3,
        tUByte4,
        tUByte8,
        tUByte16
        // (reserved next: bfloat16 family — bfloat, bfloat2/3/4/8 — decided OUT for now)
    };

    template <typename TT>
    struct EnumStubAny {
        typedef TT BaseType;
        TT value;
    };

    typedef EnumStubAny<int32_t> EnumStub;
    typedef EnumStubAny<int8_t>   EnumStub8;
    typedef EnumStubAny<int16_t>  EnumStub16;
    typedef EnumStubAny<int64_t>  EnumStub64;
    // unsigned-underlying variants — used by the typer to dispatch `int(uint8Enum)` / `uint(uint8Enum)`
    // (and 16/64-bit) so the byte zero-extends instead of sign-extending via int8_t/int16_t/int64_t.
    typedef EnumStubAny<uint8_t>  EnumStub8u;
    typedef EnumStubAny<uint16_t> EnumStub16u;
    typedef EnumStubAny<uint64_t> EnumStub64u;

    template <typename ST>
    struct BitfieldAny {
        ST value;
        __forceinline BitfieldAny() {}
        // exactly ST converts implicitly (bitfield math decays to ST through operator ST&,
        // and every cast/call boundary must convert back); every other type converts only
        // through an explicit cast, so implicit narrowing - int included - dies at the call site
        template <typename QQ, typename std::enable_if<std::is_same<QQ,ST>::value,int>::type = 0>
        __forceinline BitfieldAny(QQ v) : value(v) {}
        template <typename QQ, typename std::enable_if<!std::is_same<QQ,ST>::value,int>::type = 0>
        __forceinline explicit BitfieldAny(QQ v) : value(ST(v)) {}
        __forceinline operator ST &() { return value; }
        __forceinline operator float() const { return float(value); }
        __forceinline operator double() const { return double(value); }
        __forceinline operator int8_t() const { return int8_t(value); }
        __forceinline operator uint8_t() const { return uint8_t(value); }
        __forceinline operator uint32_t() const { return uint32_t(value); }
        __forceinline operator int32_t() const { return int32_t(value); }
        __forceinline operator int16_t() const { return int16_t(value); }
        __forceinline operator uint16_t() const { return uint16_t(value); }
        __forceinline operator int64_t() const { return int64_t(value); }
        __forceinline operator uint64_t() const { return uint64_t(value); }
        __forceinline bool operator==(const BitfieldAny<ST> &f) const { return value == f.value; }
        __forceinline bool operator!=(const BitfieldAny<ST> &f) const { return value != f.value; }
        __forceinline bool operator==(ST f) const { return value == f; }
        __forceinline bool operator!=(ST f) const { return value != f; }
    };

    typedef BitfieldAny<uint8_t> Bitfield8;
    typedef BitfieldAny<uint16_t> Bitfield16;
    typedef BitfieldAny<uint32_t> Bitfield;
    typedef BitfieldAny<uint64_t> Bitfield64;

    __forceinline bool operator==(uint8_t f, const Bitfield8 &t) { return t.value == f; }
    __forceinline bool operator!=(uint8_t f, const Bitfield8 &t) { return t.value != f; }
    __forceinline bool operator==(uint16_t f, const Bitfield16 &t) { return t.value == f; }
    __forceinline bool operator!=(uint16_t f, const Bitfield16 &t) { return t.value != f; }
    __forceinline bool operator==(uint32_t f, const Bitfield &t) { return t.value == f; }
    __forceinline bool operator!=(uint32_t f, const Bitfield &t) { return t.value != f; }
    __forceinline bool operator==(uint64_t f, const Bitfield64 &t) { return t.value == f; }
    __forceinline bool operator!=(uint64_t f, const Bitfield64 &t) { return t.value != f; }

    template <typename TT> struct ToBasicType {
        enum { type = Type::none };
        static_assert( int(type)!=int(Type::none),"This type is not supported or not bound. For the bound type missing or not included are "
            "MAKE_TYPE_FACTORY or MAKE_EXTERNAL_TYPE_FACTORY as well as addAnnotation in the appropriate module.");
    };

    template<> struct ToBasicType<Bitfield>     { enum { type = Type::tBitfield }; };
    template<> struct ToBasicType<Bitfield8>    { enum { type = Type::tBitfield8 }; };
    template<> struct ToBasicType<Bitfield16>   { enum { type = Type::tBitfield16 }; };
    template<> struct ToBasicType<Bitfield64>   { enum { type = Type::tBitfield64 }; };
    template<> struct ToBasicType<EnumStub>     { enum { type = Type::tEnumeration }; };
    template<> struct ToBasicType<EnumStub8>    { enum { type = Type::tEnumeration8 }; };
    template<> struct ToBasicType<EnumStub16>   { enum { type = Type::tEnumeration16 }; };
    template<> struct ToBasicType<EnumStub64>   { enum { type = Type::tEnumeration64 }; };
    template<> struct ToBasicType<EnumStub8u>   { enum { type = Type::tEnumeration8 }; };
    template<> struct ToBasicType<EnumStub16u>  { enum { type = Type::tEnumeration16 }; };
    template<> struct ToBasicType<EnumStub64u>  { enum { type = Type::tEnumeration64 }; };
    template<> struct ToBasicType<void *>       { enum { type = Type::tPointer }; };
    template<> struct ToBasicType<char *>       { enum { type = Type::tString }; };
    template<> struct ToBasicType<bool>         { enum { type = Type::tBool }; };
    template<> struct ToBasicType<int64_t>      { enum { type = Type::tInt64 }; };
    template<> struct ToBasicType<uint64_t>     { enum { type = Type::tUInt64 }; };
    template<> struct ToBasicType<int8_t>       { enum { type = Type::tInt8 }; };
    template<> struct ToBasicType<uint8_t>      { enum { type = Type::tUInt8 }; };
    template<> struct ToBasicType<int16_t>      { enum { type = Type::tInt16 }; };
    template<> struct ToBasicType<uint16_t>     { enum { type = Type::tUInt16 }; };
    template<> struct ToBasicType<int32_t>      { enum { type = Type::tInt }; };
    template<> struct ToBasicType<uint32_t>     { enum { type = Type::tUInt }; };
    template<> struct ToBasicType<float>        { enum { type = Type::tFloat }; };
    template<> struct ToBasicType<void>         { enum { type = Type::tVoid }; };
    template<> struct ToBasicType<char>         { enum { type = Type::tInt8 }; };
#if defined(_WIN32)
    // Broadened from _MSC_VER to cover mingw (clang-mingw64 + gcc-mingw) too —
    // Windows LLP64 makes `long`/`unsigned long` 32-bit regardless of compiler.
    // Without this, dasClangBind fails to build on mingw because libclang's
    // CXUnsavedFile::Length (unsigned long) hits the primary template's
    // static_assert.
    template<> struct ToBasicType<long>             { enum { type = Type::tInt }; };
    template<> struct ToBasicType<unsigned long>    { enum { type = Type::tUInt }; };
    template<> struct ToBasicType<long double>      { enum { type = Type::tDouble }; };
    template<> struct ToBasicType<wchar_t>          { enum { type = Type::tUInt16 }; };
#endif
#if defined(__APPLE__)
    // note - under MSVC size_t is unsigned __int64 (or 32) accordingly
    template<> struct ToBasicType<size_t>           { enum { type = sizeof(size_t)==8 ? Type::tUInt64 : Type::tUInt }; };
    template<> struct ToBasicType<long>             { enum { type = Type::tInt }; };
    template<> struct ToBasicType<long double>      { enum { type = Type::tDouble }; };
    template<> struct ToBasicType<wchar_t>          { enum { type = Type::tUInt16 }; };
#endif
#if !defined(_MSC_VER) && !defined(__APPLE__) && !defined(_EMSCRIPTEN_VER) && defined(ULONG_MAX) && ULONG_MAX == UINT64_MAX
    template<> struct ToBasicType<long long int>      { enum { type = Type::tInt64 }; };
    template<> struct ToBasicType<unsigned long long int>     { enum { type = Type::tUInt64 }; };
#endif
#ifdef _EMSCRIPTEN_VER
    template<> struct ToBasicType<unsigned long>    { enum { type = Type::tUInt }; };
#endif
    template<> struct ToBasicType<float2>       { enum { type = Type::tFloat2 }; };
    template<> struct ToBasicType<float3>       { enum { type = Type::tFloat3 }; };
    template<> struct ToBasicType<float4>       { enum { type = Type::tFloat4 }; };
    template<> struct ToBasicType<int2>         { enum { type = Type::tInt2 }; };
    template<> struct ToBasicType<int3>         { enum { type = Type::tInt3 }; };
    template<> struct ToBasicType<int4>         { enum { type = Type::tInt4 }; };
    template<> struct ToBasicType<uint2>        { enum { type = Type::tUInt2 }; };
    template<> struct ToBasicType<uint3>        { enum { type = Type::tUInt3 }; };
    template<> struct ToBasicType<uint4>        { enum { type = Type::tUInt4 }; };
    template<> struct ToBasicType<double>       { enum { type = Type::tDouble }; };
    template<> struct ToBasicType<float16_t>    { enum { type = Type::tFloat16 }; };
    template<> struct ToBasicType<half2>        { enum { type = Type::tHalf2 }; };
    template<> struct ToBasicType<half3>        { enum { type = Type::tHalf3 }; };
    template<> struct ToBasicType<half4>        { enum { type = Type::tHalf4 }; };
    template<> struct ToBasicType<half8>        { enum { type = Type::tHalf8 }; };
    template<> struct ToBasicType<short2>       { enum { type = Type::tShort2 }; };
    template<> struct ToBasicType<short3>       { enum { type = Type::tShort3 }; };
    template<> struct ToBasicType<short4>       { enum { type = Type::tShort4 }; };
    template<> struct ToBasicType<short8>       { enum { type = Type::tShort8 }; };
    template<> struct ToBasicType<ushort2>      { enum { type = Type::tUShort2 }; };
    template<> struct ToBasicType<ushort3>      { enum { type = Type::tUShort3 }; };
    template<> struct ToBasicType<ushort4>      { enum { type = Type::tUShort4 }; };
    template<> struct ToBasicType<ushort8>      { enum { type = Type::tUShort8 }; };
    template<> struct ToBasicType<byte2>        { enum { type = Type::tByte2 }; };
    template<> struct ToBasicType<byte3>        { enum { type = Type::tByte3 }; };
    template<> struct ToBasicType<byte4>        { enum { type = Type::tByte4 }; };
    template<> struct ToBasicType<byte8>        { enum { type = Type::tByte8 }; };
    template<> struct ToBasicType<byte16>       { enum { type = Type::tByte16 }; };
    template<> struct ToBasicType<ubyte2>       { enum { type = Type::tUByte2 }; };
    template<> struct ToBasicType<ubyte3>       { enum { type = Type::tUByte3 }; };
    template<> struct ToBasicType<ubyte4>       { enum { type = Type::tUByte4 }; };
    template<> struct ToBasicType<ubyte8>       { enum { type = Type::tUByte8 }; };
    template<> struct ToBasicType<ubyte16>      { enum { type = Type::tUByte16 }; };
    template<> struct ToBasicType<range>        { enum { type = Type::tRange }; };
    template<> struct ToBasicType<urange>       { enum { type = Type::tURange }; };
    template<> struct ToBasicType<range64>      { enum { type = Type::tRange64 }; };
    template<> struct ToBasicType<urange64>     { enum { type = Type::tURange64 }; };
    template<> struct ToBasicType<vec4f>        { enum { type = Type::anyArgument }; };
}

namespace std {
    template <>
    struct hash<das::Bitfield> {
        size_t operator()(das::Bitfield b) const {
            return hash<uint32_t>()(b.value);
        }
    };
}
