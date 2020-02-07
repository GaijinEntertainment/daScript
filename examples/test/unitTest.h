#pragma once

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/bind_enum.h"

enum class SomeEnum {
    zero
,   one
,   two
};

DAS_BIND_ENUM_CAST(SomeEnum);

namespace Goo {
    enum class GooEnum {
        regular
    ,   hazardous
    };
}

DAS_BIND_ENUM_CAST(Goo::GooEnum);

enum SomeEnum98 {
    SomeEnum98_zero = 0
,   SomeEnum98_one  = 1
,   SomeEnum98_two  = 2
};

DAS_BIND_ENUM_CAST_98(SomeEnum98);

Goo::GooEnum efn_flip ( Goo::GooEnum goo );
SomeEnum efn_takeOne_giveTwo ( SomeEnum one );
SomeEnum98 efn_takeOne_giveTwo_98 ( SomeEnum98 one );
SomeEnum98_DasProxy efn_takeOne_giveTwo_98_DasProxy ( SomeEnum98_DasProxy two );

//sample of your-engine-float3-type to be aliased as float3 in daScript.
struct Point3 { float x, y, z; };

template <> struct das::das_alias<Point3> : das::das_alias_vec<Point3,float3> {};

struct SomeDummyType {
    int32_t foo;
    float   bar;
};

__forceinline Point3 getSamplePoint3() {return Point3{0,1,2};}
__forceinline Point3 doubleSamplePoint3(const Point3 &a) { return Point3{ a.x + a.x, a.y + a.y, a.z + a.z }; }

struct TestObjectFoo {
    Point3 hit;
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

__forceinline TestObjectFoo makeDummy() { TestObjectFoo x; x.fooData = 1; return x; }
__forceinline int takeDummy ( const TestObjectFoo & x ) { return x.fooData; }

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

uint32_t CheckEid ( char * const name, das::Context * context );
uint32_t CheckEidHint ( char * const name, uint32_t hashHint, das::Context * context );

