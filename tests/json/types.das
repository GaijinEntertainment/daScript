require dastest/testing_boost public
require daslib/json_boost

def test_equ ( t:T?; x:auto(TT) )
    var jv = JV(x)
    // to_log(LOG_INFO, "{x}:{typeinfo(typename x)} serialized as {write_json(jv)}\n")
    t |> equal(x, from_JV(jv, decltype_noref(x)))
    unsafe
        delete jv

[test]
def basic_types ( t: T? )
    test_equ(t, 1)
    test_equ(t, 2u)
    test_equ(t, int8(1))
    test_equ(t, uint8(2))
    test_equ(t, int16(1))
    test_equ(t, uint16(2))
    test_equ(t, 12345678901234567890l)
    test_equ(t, 0x1234567812345678ul)
    test_equ(t, 1.)
    test_equ(t, 2.lf)
    test_equ(t, bitfield(13))

[test]
def vector_types ( t: T? )
    test_equ(t, float2(1,2))
    test_equ(t, float3(1,2,3))
    test_equ(t, float4(1,2,3,4))
    test_equ(t, int2(1,2))
    test_equ(t, int3(1,2,3))
    test_equ(t, int4(1,2,3,4))
    test_equ(t, uint2(1,2))
    test_equ(t, uint3(1,2,3))
    test_equ(t, uint4(1,2,3,4))

enum FooBar
    foo
    bar

[test]
def enum_types ( t: T? )
    test_equ(t, FooBar foo)
    test_equ(t, FooBar bar)

struct SFoo
    a : int
    b : float

def operator == ( a,b: SFoo )
    return a.a==b.a && a.b==b.b

struct NFoo
    c : double
    s : SFoo

def operator == ( a,b: NFoo )
    return a.c==b.c && a.s==b.s

[test]
def struct_types ( t: T? )
    test_equ(t, [[SFoo a=1,b=2.]])
    test_equ(t, [[NFoo c=1.lf,s=[[SFoo a=1,b=2.]]]])

def operator == ( a,b:int[4] )
    for A,B in a,b
        if A!=B
            return false
    return true

def operator == ( a,b:SFoo[2] )
    for A,B in a,b
        if !(A==B)
            return false
    return true

[test]
def dim_types ( t: T? )
    test_equ(t, [[int 1;2;3;4]])
    test_equ(t, [[SFoo a=1,b=2.;a=2,b=3.]])

def operator == ( a,b:array<int> )
    for A,B in a,b
        if A!=B
            return false
    return true

def operator == ( a,b:array<SFoo> )
    for A,B in a,b
        if !(A==B)
            return false
    return true

[test]
def array_types ( t: T? )
    test_equ(t, [{int 1;2;3;4}])
    test_equ(t, [{SFoo a=1,b=2.;a=2,b=3.}])

typedef TFoo = tuple<int; float; string>

def operator == ( a,b: TFoo )
    return a._0==b._0 && a._1==b._1 && a._2==b._2

[test]
def tuple_types ( t: T? )
    test_equ(t, [[auto 1,2.0,"3"]])


