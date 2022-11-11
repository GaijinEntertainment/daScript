require dastest/testing_boost

require llvm/llvm_jit
require daslib/fuzzer
require daslib/faker
require daslib/math
require daslib/constant_expression

////////////////////////////
// floating point type ctors

// float

[jit]
def jit_float ( x )
    return float(x)

def test_float( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_float) )
    t |> strictEqual ( float(a), jit_float(a), "float(a)" )

[test]
def test_intrin_float ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_and_storage_op1(t,fake,"test_float")

// double

[jit]
def jit_double ( x )
    return double(x)

def test_double( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_double) )
    t |> strictEqual ( double(a), jit_double(a), "double(a)" )

[test]
def test_intrin_double ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_and_storage_op1(t,fake,"test_double")

/////////////////////
// integer type ctors

// int

[jit]
def jit_int ( x )
    return int(x)

def test_int( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_int) )
    t |> strictEqual ( int(a), jit_int(a), "int(a)" )

[test]
def test_intrin_int ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_and_storage_op1(t,fake,"test_int")

// uint

[jit]
def jit_uint ( x )
    return uint(x)

def test_uint( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_uint) )
    t |> strictEqual ( uint(a), jit_uint(a), "uint(a)" )

[test]
def test_intrin_uint ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_and_storage_op1(t,fake,"test_uint")

// int8

[jit]
def jit_int8 ( x )
    return int8(x)

def test_int8( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_int8) )
    t |> strictEqual ( int8(a), jit_int8(a), "int8(a)" )

[test]
def test_intrin_int8 ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_and_storage_op1(t,fake,"test_int8")

// uint8

[jit]
def jit_uint8 ( x )
    return uint8(x)

def test_uint8( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_uint8) )
    t |> strictEqual ( uint8(a), jit_uint8(a), "uint8(a)" )

[test]
def test_intrin_uint8 ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_and_storage_op1(t,fake,"test_uint8")

// int16

[jit]
def jit_int16 ( x )
    return int16(x)

def test_int16( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_int16) )
    t |> strictEqual ( int16(a), jit_int16(a), "int16(a)" )

[test]
def test_intrin_int16 ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_and_storage_op1(t,fake,"test_int16")

// uint16

[jit]
def jit_uint16 ( x )
    return uint16(x)

def test_uint16( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_uint16) )
    t |> strictEqual ( uint16(a), jit_uint16(a), "uint16(a)" )

[test]
def test_intrin_uint16 ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_and_storage_op1(t,fake,"test_uint16")

// int64

[jit]
def jit_int64 ( x )
    return int64(x)

def test_int64( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_int64) )
    t |> strictEqual ( int64(a), jit_int64(a), "int64(a)" )

[test]
def test_intrin_int64 ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_and_storage_op1(t,fake,"test_int64")

// uint64

[jit]
def jit_uint64 ( x )
    return uint64(x)

def test_uint64( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_uint64) )
    t |> strictEqual ( uint64(a), jit_uint64(a), "uint64(a)" )

[test]
def test_intrin_uint64 ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_and_storage_op1(t,fake,"test_uint64")

///////////////
// vector ctors

// float4

[jit]
def jit_float4 ( x )
    return float4(x)

def test_float4( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_float4) )
    t |> strictEqual ( float4(a), jit_float4(a), "float4(a)" )

[jit,sideeffects]
def jit_float4_0
    return float4()

[jit]
def jit_float4_2(a,b)
    return float4(a,b)

[jit]
def jit_float4_3(a,b,c)
    return float4(a,b,c)

[jit]
def jit_float4_4 ( x,y,z,w )
    return float4(x,y,z,w)

def test_float4_4( t:T?; x,y,z,w:auto(TT) )
    t |> success ( is_jit_function(@@<(x,y,z,w:TT):void> jit_float4_4) )
    t |> strictEqual ( float4(x,y,z,w), jit_float4_4(x,y,z,w), "float4(x,y,z,w)" )

[test]
def test_intrin_float4 ( t:T? )
    t |> run("float4()") <| @ ( t : T? )
        t |> strictEqual ( float4(), jit_float4_0(), "float4()" )
    t |> run("float4(x)") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            test_float4( t, fake |> random_int )
            test_float4( t, fake |> random_uint )
            test_float4( t, fake |> random_float )
            test_float4( t, fake |> random_double )
            test_float4( t, fake |> random_float4 )
            test_float4( t, fake |> random_int4 )
            test_float4( t, fake |> random_uint4 )
    t |> run("float4(x,y)") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            let f1 = fake |> random_float
            let f2 = fake |> random_float3
            t |> strictEqual ( float4(f1,f2), jit_float4_2(f1,f2), "float4(x,y) // 1,3" )
        fuzz <|
            let f1 = fake |> random_float2
            let f2 = fake |> random_float2
            t |> strictEqual ( float4(f1,f2), jit_float4_2(f1,f2), "float4(x,y) // 2,2" )
        fuzz <|
            let f1 = fake |> random_float3
            let f2 = fake |> random_float
            t |> strictEqual ( float4(f1,f2), jit_float4_2(f1,f2), "float4(x,y) // 3,1" )
    t |> run("float4(x,y,z)") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            let f1 = fake |> random_float
            let f2 = fake |> random_float
            let f3 = fake |> random_float2
            t |> strictEqual ( float4(f1,f2,f3), jit_float4_3(f1,f2,f3), "float4(x,y) // 1,1,2" )
        fuzz <|
            let f1 = fake |> random_float
            let f2 = fake |> random_float2
            let f3 = fake |> random_float
            t |> strictEqual ( float4(f1,f2,f3), jit_float4_3(f1,f2,f3), "float4(x,y,z) // 1,2,1" )
    t |> run("float4(x,y,z,w)") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            fuzz_numeric_op4(t,fake,"test_float4_4")

// float3

[jit]
def jit_float3 ( x )
    return float3(x)

def test_float3( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_float3) )
    t |> strictEqual ( float3(a), jit_float3(a), "float3(a)" )

[jit,sideeffects]
def jit_float3_0
    return float3()

[jit]
def jit_float3_2(a,b)
    return float3(a,b)

[jit]
def jit_float3_3(a,b,c)
    return float3(a,b,c)

def test_float3_3( t:T?; x,y,z:auto(TT) )
    t |> success ( is_jit_function(@@<(x,y,z:TT):void> jit_float3_3) )
    t |> strictEqual ( float3(x,y,z), jit_float3_3(x,y,z), "float3(x,y,z)" )

[test]
def test_intrin_float3 ( t:T? )
    t |> run("float3()") <| @ ( t : T? )
        t |> strictEqual ( float3(), jit_float3_0(), "float3()" )
    t |> run("float3(x)") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            test_float3( t, fake |> random_int )
            test_float3( t, fake |> random_uint )
            test_float3( t, fake |> random_float )
            test_float3( t, fake |> random_double )
            test_float3( t, fake |> random_float3 )
            test_float3( t, fake |> random_int3 )
            test_float3( t, fake |> random_uint3 )
    t |> run("float3(x,y)") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            let f1 = fake |> random_float
            let f2 = fake |> random_float2
            t |> strictEqual ( float3(f1,f2), jit_float3_2(f1,f2), "float3(x,y) // 1,2" )
        fuzz <|
            let f1 = fake |> random_float2
            let f2 = fake |> random_float
            t |> strictEqual ( float3(f1,f2), jit_float3_2(f1,f2), "float3(x,y) // 2,1" )
    t |> run("float3(x,y,z)") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            fuzz_numeric_op3(t,fake,"test_float3_3")

// float2

[jit]
def jit_float2 ( x )
    return float2(x)

def test_float2( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_float2) )
    t |> strictEqual ( float2(a), jit_float2(a), "float2(a)" )

[jit,sideeffects]
def jit_float2_0
    return float2()

[jit]
def jit_float2_2(a,b)
    return float2(a,b)

def test_float2_2( t:T?; x,y:auto(TT) )
    t |> success ( is_jit_function(@@<(x,y:TT):void> jit_float2_2) )
    t |> strictEqual ( float2(x,y), jit_float2_2(x,y), "float2(x,y)" )

[test]
def test_intrin_float2 ( t:T? )
    t |> run("float2()") <| @ ( t : T? )
        t |> strictEqual ( float2(), jit_float2_0(), "float2()" )
    t |> run("float2(x)") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            test_float2( t, fake |> random_int )
            test_float2( t, fake |> random_uint )
            test_float2( t, fake |> random_float )
            test_float2( t, fake |> random_double )
            test_float2( t, fake |> random_float2 )
            test_float2( t, fake |> random_int2 )
            test_float2( t, fake |> random_uint2 )
    t |> run("float2(x,y,z)") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            fuzz_numeric_op2(t,fake,"test_float2_2")



