#pragma once

#include "daScript/simulate/simulate.h"

struct Object {
    das::float3   pos;  
    das::float3   vel;  
};

typedef das::vector<Object> ObjectArray;

int AddOne(int a);

int testDict(das::Array & arr);
float testExpLoop(int count);
int testFibR(int n);
int testFibI(int n);
void testNBodiesInit();
void testNBodies(int32_t N);
void testParticles(int count);
void testParticlesI(int count);
int testPrimes(int n);
void testTryCatch(das::Context * context);

void updateObject(Object & obj);
void updateTest(ObjectArray & objects);
void update10000(ObjectArray & objects, das::Context * context);
void update10000ks(ObjectArray & objects, das::Context * context);

// ES

struct EsAttribute {
    EsAttribute() = default;
    EsAttribute ( const das::string & n, uint32_t sz, bool rf, vec4f d )
    : def(d), name(n), size(sz), ref(rf) {}
    vec4f       def = v_zero();
    das::string      name;
    uint32_t    size = 0;
    bool        ref;
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
void verifyEsComponents();
void testEsUpdate(char * pass, das::Context * ctx);
uint32_t queryEs(const das::Block & block, das::Context * context);


