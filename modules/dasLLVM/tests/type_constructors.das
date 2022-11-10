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

[test]
def test_intrin_float4 ( t:T? )
    t |> run("float4()") <| @ ( t : T? )
        t |> strictEqual ( float4(), jit_float4_0(), "float4()" )
    t |> run("float4(x)") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            fuzz_numeric_op1(t,fake,"test_float4")
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
    t |> run("float4(x,y)") <| @ ( t : T? )
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




