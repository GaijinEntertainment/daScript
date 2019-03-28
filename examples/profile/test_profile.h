#pragma once

#include "daScript/daScript.h"

struct Object {
    das::float3   pos;  
    das::float3   vel;  
};

typedef std::vector<Object> ObjectArray;

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

void initEsComponents();
void verifyEsComponents();
void testEsUpdate(char * pass, das::Context * ctx);
uint32_t queryEs(const das::Block & block, das::Context * context);


