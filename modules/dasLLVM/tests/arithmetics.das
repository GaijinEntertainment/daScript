require dastest/testing_boost public

require llvm/llvm_jit
require daslib/fuzzer
require daslib/faker

require daslib/constant_expression

/*
[jit_llvm]
def test_min2 ( a, b : int )
    return a < b ? a : b

[jit_llvm]
def test_min3 ( a, b, c : int )
    if a < b
        return a < c ? a : c
    else
        return b < c ? b : c
*/

[jit_llvm]
def add_jit (a,b)
    return a + b

[jit_llvm]
def sub_jit (a,b)
    return a - b

[jit_llvm]
def mul_jit (a,b)
    return a * b

[jit_llvm]
def div_jit (a,b)
    return a / b

[jit_llvm]
def mod_jit (a,b)
    return a % b

def test_add( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> add_jit) )
    t |> strictEqual ( a + b, add_jit(a,b), "a + b" )

def test_sub( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> sub_jit) )
    t |> strictEqual ( a - b, sub_jit(a,b), "{a} - {b}" )

def test_mul( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> mul_jit) )
    t |> strictEqual ( a * b, mul_jit(a,b), "{a} * {b}" )

def test_div( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> div_jit) )
    if b != TT()
        t |> strictEqual ( a / b, div_jit(a,b), "{a} / {b}" )

def test_mod( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> mod_jit) )
    if b != TT()
        t |> strictEqual ( a % b, mod_jit(a,b), "{a} % {b}" )

[constant_expression(funcname)]
def fuzz_numeric_and_vector ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint, fake |> random_uint )
    invoke ( funcname, t, fake |> random_float, fake |> random_float )
    invoke ( funcname, t, fake |> random_double, fake |> random_double )
    invoke ( funcname, t, fake |> random_int2,  fake |> random_int2 )
    invoke ( funcname, t, fake |> random_int3,  fake |> random_int3 )
    invoke ( funcname, t, fake |> random_int4,  fake |> random_int4 )
    invoke ( funcname, t, fake |> random_uint2,  fake |> random_uint2 )
    invoke ( funcname, t, fake |> random_uint3,  fake |> random_uint3 )
    invoke ( funcname, t, fake |> random_uint4,  fake |> random_uint4 )
    invoke ( funcname, t, fake |> random_float2, fake |> random_float2 )
    invoke ( funcname, t, fake |> random_float3, fake |> random_float3 )
    invoke ( funcname, t, fake |> random_float4, fake |> random_float4 )

[constant_expression(funcname)]
def fuzz_numeric ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint, fake |> random_uint )
    invoke ( funcname, t, fake |> random_float, fake |> random_float )
    invoke ( funcname, t, fake |> random_double, fake |> random_double )

[test]
def test_arithmetics ( t:T? )
    t |> run("+, -, *, /, %") <| @ ( t : T? )
        var fake <- Faker()
        fuzz_numeric_and_vector(t, fake, "test_add")
        fuzz_numeric_and_vector(t, fake, "test_sub")
        fuzz_numeric_and_vector(t, fake, "test_mul")
        fuzz_numeric_and_vector(t, fake, "test_div")
        fuzz_numeric(t, fake, "test_mod")



