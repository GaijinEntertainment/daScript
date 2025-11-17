#pragma once

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/aot.h"

struct DAS_MOD_API Object {
    das::float3   pos;
    das::float3   vel;

    __forceinline bool operator == (const Object & that) const {
        return pos == that.pos && vel == that.vel;
    }
    __forceinline bool operator != (const Object & that) const {
        return pos != that.pos || vel != that.vel;
    }

};

typedef das::vector<Object> ObjectArray;

namespace das {
    template <>
    struct typeName<ObjectArray> {
        constexpr static const char * name() {
            return "ObjectArray";
        }
    };
}

namespace das {
    template<>
    struct SimPolicy<Object>
    {
        static __forceinline const Object &to ( vec4f a ) { return *(const Object *)v_extract_ptr(v_cast_vec4i(a)); }
        static __forceinline bool Equ     ( vec4f a, vec4f b, Context &, LineInfo * ) { return to(a) == to(b); }
        static __forceinline bool NotEqu  ( vec4f a, vec4f b, Context &, LineInfo * ) { return to(a) != to(b); }
    };

    __forceinline bool objEqu ( const Object & a, const Object & b ) { return a == b; }
    __forceinline bool objNeq ( const Object & a, const Object & b ) { return a != b; }
}

DAS_MOD_API int AddOne(int a);

DAS_MOD_API int testDict(das::Array & arr);
DAS_MOD_API float testExpLoop(int count);
DAS_MOD_API int testFibR(int n);
DAS_MOD_API int testFibI(int n);
DAS_MOD_API void testNBodiesInit();
DAS_MOD_API void testNBodies(int32_t N);
DAS_MOD_API void testNBodiesS(int32_t N);
DAS_MOD_API void testParticles(int count);
DAS_MOD_API void testParticlesI(int count);
DAS_MOD_API int testPrimes(int n);
DAS_MOD_API void testTryCatch(das::Context * context, das::LineInfoArg * at);
DAS_MOD_API int testTree();
DAS_MOD_API uint32_t testMaxFrom1s(uint32_t x);
DAS_MOD_API void testTableSort ( das::TArray<int32_t> & tab );

DAS_MOD_API void testManagedInt(const das::TBlock<void, const das::vector<int32_t>> & blk, das::Context * context, das::LineInfoArg * at);

DAS_MOD_API void updateObject(Object & obj);
DAS_MOD_API void updateTest(ObjectArray & objects);
DAS_MOD_API void update10000(ObjectArray & objects, das::Context * context, das::LineInfoArg * at);
DAS_MOD_API void update10000ks(ObjectArray & objects, das::Context * context, das::LineInfoArg * at);

// ES

struct DAS_MOD_API EsAttribute {
    EsAttribute() = default;
    EsAttribute ( const das::string & n, uint32_t sz, bool rf, vec4f d, const char * ds = "" )
        : def(d), name(n), size(sz), ref(rf), def_s(ds) {}
    ~EsAttribute() { if(def_s) free((void *)def_s); }
    vec4f       def = v_zero();
    das::string name;
    uint32_t    size = 0;
    bool        ref;
    const char *def_s = nullptr;
};

struct EsAttributeTable {
    das::vector<EsAttribute> attributes;
};

struct EsPassAttributeTable  : EsAttributeTable {
    das::string pass;
    uint64_t    mangledNameHash = 0;
    int32_t     functionIndex = 0;
};

struct EsComponent {
    das::string      name;
    uint32_t    size = 0;
    uint32_t    stride = 0;
    bool        boxed = false;

    EsComponent() = default;
    EsComponent(const das::string & n, size_t sz, size_t st, bool bx) :
    name(n), size(uint32_t(sz)), stride(uint32_t(st)), boxed(bx) {}
};

constexpr int g_total = 100000;
inline DAS_THREAD_LOCAL(das::vector<das::float3>)    g_pos;
inline DAS_THREAD_LOCAL(das::vector<das::float3>)    g_vel;
inline DAS_THREAD_LOCAL(das::vector<das::float3 *>)  g_velBoxed;
inline DAS_THREAD_LOCAL(das::vector<EsComponent>)    g_components;

DAS_MOD_API void initEsComponents();
DAS_MOD_API void initEsComponentsTable ();
DAS_MOD_API void releaseEsComponents();
DAS_MOD_API void verifyEsComponents(das::Context * ctx, das::LineInfoArg * at);
DAS_MOD_API void testEsUpdate(char * pass, das::Context * ctx, das::LineInfoArg * at);
DAS_MOD_API uint32_t queryEs(const das::Block & block, das::Context * context, das::LineInfoArg * at);

DAS_MOD_API int testQueens();
DAS_MOD_API double testSnorm();
DAS_MOD_API int testMandelbrot();
DAS_MOD_API float test_f2i ( const das::TArray<char *> & nums, int TOTAL_NUMBERS, int TOTAL_TIMES );
DAS_MOD_API int32_t test_f2s ( const das::TArray<float> & nums, int TOTAL_NUMBERS, int TOTAL_TIMES );
