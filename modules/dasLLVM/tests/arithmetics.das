require dastest/testing_boost

require llvm/llvm_jit
require daslib/fuzzer
require daslib/faker

require daslib/constant_expression

[jit]
def add_jit (a,b)
    return a + b

[jit]
def sub_jit (a,b)
    return a - b

[jit]
def mul_jit (a,b)
    return a * b

[jit]
def div_jit (a,b)
    return a / b

[jit]
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

def test_div_vec( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> div_jit) )
    t |> strictEqual ( a / b, div_jit(a,b), "{a} / {b}" )

def test_mod( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> mod_jit) )
    if b != TT()
        t |> strictEqual ( a % b, mod_jit(a,b), "{a} % {b}" )

[jit]
def add_equ_jit (a,b)
    var c = a
    c += b
    return c

[jit]
def sub_equ_jit (a,b)
    var c = a
    c -= b
    return c

[jit]
def mul_equ_jit (a,b)
    var c = a
    c *= b
    return c

[jit]
def div_equ_jit (a,b)
    var c = a
    c /= b
    return c

[jit]
def mod_equ_jit (a,b)
    var c = a
    c %= b
    return c

def test_add_equ( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> add_equ_jit) )
    t |> strictEqual ( a + b, add_equ_jit(a,b), "a + b" )

def test_sub_equ( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> sub_equ_jit) )
    t |> strictEqual ( a - b, sub_equ_jit(a,b), "{a} - {b}" )

def test_mul_equ( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> mul_equ_jit) )
    t |> strictEqual ( a * b, mul_equ_jit(a,b), "{a} * {b}" )

def test_div_equ( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> div_equ_jit) )
    if b != TT()
        t |> strictEqual ( a / b, div_equ_jit(a,b), "{a} / {b}" )

def test_div_equ_vec( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> div_equ_jit) )
    t |> strictEqual ( a / b, div_equ_jit(a,b), "{a} / {b}" )

def test_mod_equ( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> mod_equ_jit) )
    if b != TT()
        t |> strictEqual ( a % b, mod_equ_jit(a,b), "{a} % {b}" )

[constant_expression(funcname)]
def fuzz_numeric_and_vector_op2 ( t:T?; var fake:Faker; funcname:string )
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
def fuzz_numeric_op2 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint, fake |> random_uint )
    invoke ( funcname, t, fake |> random_float, fake |> random_float )
    invoke ( funcname, t, fake |> random_double, fake |> random_double )

[constant_expression(funcname)]
def fuzz_numeric_vec_scal_op2 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int2,  fake |> random_int )
    invoke ( funcname, t, fake |> random_int3,  fake |> random_int )
    invoke ( funcname, t, fake |> random_int4,  fake |> random_int )
    invoke ( funcname, t, fake |> random_uint2,  fake |> random_uint )
    invoke ( funcname, t, fake |> random_uint3,  fake |> random_uint )
    invoke ( funcname, t, fake |> random_uint4,  fake |> random_uint )
    invoke ( funcname, t, fake |> random_float2, fake |> random_float )
    invoke ( funcname, t, fake |> random_float3, fake |> random_float )
    invoke ( funcname, t, fake |> random_float4, fake |> random_float )

[constant_expression(funcname)]
def fuzz_numeric_scal_vec_op2 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int2 )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int3 )
    invoke ( funcname, t, fake |> random_int,  fake |> random_int4 )
    invoke ( funcname, t, fake |> random_uint,  fake |> random_uint2 )
    invoke ( funcname, t, fake |> random_uint,  fake |> random_uint3 )
    invoke ( funcname, t, fake |> random_uint,  fake |> random_uint4 )
    invoke ( funcname, t, fake |> random_float, fake |> random_float2 )
    invoke ( funcname, t, fake |> random_float, fake |> random_float3 )
    invoke ( funcname, t, fake |> random_float, fake |> random_float4 )

[test]
def test_arithmetics_op2 ( t:T? )
    t |> run("+, -, *, /, %") <| @ ( t : T? )
        var fake <- Faker()
        fuzz_numeric_and_vector_op2(t, fake, "test_add")
        fuzz_numeric_and_vector_op2(t, fake, "test_sub")
        fuzz_numeric_and_vector_op2(t, fake, "test_mul")
        fuzz_numeric_and_vector_op2(t, fake, "test_div")
        fuzz_numeric_op2(t, fake, "test_mod")

    t |> run("+=, -=, *=, /=, %=") <| @ ( t : T? )
        var fake <- Faker()
        fuzz_numeric_and_vector_op2(t, fake, "test_add_equ")
        fuzz_numeric_and_vector_op2(t, fake, "test_sub_equ")
        fuzz_numeric_and_vector_op2(t, fake, "test_mul_equ")
        fuzz_numeric_and_vector_op2(t, fake, "test_div_equ")
        fuzz_numeric_op2(t, fake, "test_mod_equ")

[constant_expression(funcname)]
def fuzz_numeric_and_vector_op1 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int )
    invoke ( funcname, t, fake |> random_uint )
    invoke ( funcname, t, fake |> random_float )
    invoke ( funcname, t, fake |> random_double )
    invoke ( funcname, t, fake |> random_int2 )
    invoke ( funcname, t, fake |> random_int3 )
    invoke ( funcname, t, fake |> random_int4 )
    invoke ( funcname, t, fake |> random_uint2 )
    invoke ( funcname, t, fake |> random_uint3 )
    invoke ( funcname, t, fake |> random_uint4 )
    invoke ( funcname, t, fake |> random_float2 )
    invoke ( funcname, t, fake |> random_float3 )
    invoke ( funcname, t, fake |> random_float4 )

[constant_expression(funcname)]
def fuzz_numeric_op1 ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int )
    invoke ( funcname, t, fake |> random_uint )
    invoke ( funcname, t, fake |> random_float )
    invoke ( funcname, t, fake |> random_double )

[test]
def test_arithmetics_vec_and_scalar_op2 ( t:T? )
    t |> run("* / vector vs salar") <| @ ( t : T? )
        var fake <- Faker()
        fuzz_numeric_vec_scal_op2(t, fake, "test_mul")
        fuzz_numeric_vec_scal_op2(t, fake, "test_div_vec")
        fuzz_numeric_scal_vec_op2(t, fake, "test_mul")
        fuzz_numeric_scal_vec_op2(t, fake, "test_div_vec")

    t |> run("*= /= vector vs scalar") <| @ ( t : T? )
        var fake <- Faker()
        fuzz_numeric_vec_scal_op2(t, fake, "test_mul_equ")
        fuzz_numeric_vec_scal_op2(t, fake, "test_div_equ_vec")

[jit]
def post_inc_jit (var a)
    return a++

[jit]
def post_dec_jit (var a)
    return a--

[jit]
def pre_inc_jit (var a)
    ++a
    return a

[jit]
def pre_dec_jit (var a)
    --a
    return a

def test_post_inc( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> post_inc_jit) )
    t |> strictEqual ( a, post_inc_jit(a), "return {a} ++" )

def test_post_dec( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> post_dec_jit) )
    t |> strictEqual ( a, post_dec_jit(a), "return {a} --" )

def test_pre_inc( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> pre_inc_jit) )
    t |> strictEqual ( a+TT(1), pre_inc_jit(a), "return ++ {a}" )

def test_pre_dec( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> pre_dec_jit) )
    t |> strictEqual ( a-TT(1), pre_dec_jit(a), "return -- {a}" )

[test]
def test_arithmetics_op1 ( t:T? )
    t |> run("++, --, +++, ---") <| @ ( t : T? )
        var fake <- Faker()
        fuzz_numeric_op1(t, fake, "test_post_inc")
        fuzz_numeric_op1(t, fake, "test_post_dec")
        fuzz_numeric_op1(t, fake, "test_pre_inc")
        fuzz_numeric_op1(t, fake, "test_pre_dec")
