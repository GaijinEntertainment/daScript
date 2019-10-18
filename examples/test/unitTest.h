#pragma once

#include "daScript/simulate/simulate.h"

//sample of your-engine-float3-type to be aliased as float3 in daScript.
struct Point3 { float x, y, z; };

template <> struct das::das_alias<Point3> : das::das_alias_vec<Point3,float3> {};

__forceinline Point3 getSamplePoint3() {return Point3{0,1,2};}
__forceinline Point3 doubleSamplePoint3(const Point3 &a) { return Point3{ a.x + a.x, a.y + a.y, a.z + a.z }; }

struct TestObjectFoo {
    int fooData;
    int propAdd13() {
        return fooData + 13;
    }
    __forceinline bool operator == ( const TestObjectFoo & obj ) const {
        return fooData == obj.fooData;
    }
    __forceinline bool operator != ( const TestObjectFoo & obj ) const {
        return fooData != obj.fooData;
    }
};

struct TestObjectBar {
    TestObjectFoo * fooPtr;
    float           barData;
    TestObjectFoo & getFoo() { return *fooPtr; }
    TestObjectFoo * getFooPtr() { return fooPtr; }
};

struct TestObjectNotLocal {
    int fooData;
};

int *getPtr();

void testFields ( das::Context * ctx );
void test_das_string(const das::Block & block, das::Context * context);
vec4f new_and_init ( das::Context & context, das::SimNode_CallBase * call, vec4f * );

struct CppS1 {
    virtual ~CppS1() {}
    // int64_t * a;
    int64_t b;
    int32_t c;
};

struct CppS2 : CppS1 {
    int32_t d;
};

__forceinline int CppS1Size() { return int(sizeof(CppS1)); }
__forceinline int CppS2Size() { return int(sizeof(CppS2)); }
__forceinline int CppS2DOffset() { return int(offsetof(CppS2, d)); }

