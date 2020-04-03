#pragma once

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/aot.h"

struct Object {
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
typedef das::vector<int32_t> ManagedIntArray;

namespace das {
    template<>
    struct SimPolicy<Object>
    {
        static __forceinline const Object &to ( vec4f a ) { return *(const Object *)v_extract_ptr(v_cast_vec4i(a)); }
        static __forceinline bool Equ     ( vec4f a, vec4f b, Context & ) { return to(a) == to(b); }
        static __forceinline bool NotEqu  ( vec4f a, vec4f b, Context & ) { return to(a) != to(b); }
    };

    template <>
    struct das_index<ObjectArray> : das_default_vector_index<ObjectArray, ObjectArray::value_type> {};

    template <>
    struct das_index<ManagedIntArray> : das_default_vector_index<ManagedIntArray, ManagedIntArray::value_type> {};
}

int AddOne(int a);

int testDict(das::Array & arr);
float testExpLoop(int count);
int testFibR(int n);
int testFibI(int n);
void testNBodiesInit();
void testNBodies(int32_t N);
void testNBodiesS(int32_t N);
void testParticles(int count);
void testParticlesI(int count);
int testPrimes(int n);
void testTryCatch(das::Context * context);
int testTree();

void testManagedInt(const das::TBlock<void, const ManagedIntArray> & blk, das::Context * context);

void updateObject(Object & obj);
void updateTest(ObjectArray & objects);
void update10000(ObjectArray & objects, das::Context * context);
void update10000ks(ObjectArray & objects, das::Context * context);

// ES

struct EsAttribute {
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
    das::string  pass;
    int32_t functionIndex;
};

struct EsComponent {
    das::string      name;
    void *      data = nullptr;
    uint32_t    size = 0;
    uint32_t    stride = 0;
    bool        boxed = false;

    EsComponent() = default;
    EsComponent(const das::string & n, void * d, size_t sz, size_t st, bool bx) :
    name(n), data(d), size(uint32_t(sz)), stride(uint32_t(st)), boxed(bx) {}
};

constexpr int g_total = 100000;
extern das::vector<das::float3>   g_pos;
extern das::vector<das::float3>   g_vel;
extern das::vector<das::float3 *> g_velBoxed;
extern das::vector<EsComponent> g_components;

void initEsComponents();
void verifyEsComponents(das::Context * ctx);
void testEsUpdate(char * pass, das::Context * ctx);
uint32_t queryEs(const das::Block & block, das::Context * context);


