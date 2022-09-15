require dastest/testing_boost public

require llvm/llvm_jit
require daslib/fuzzer
require daslib/faker

require daslib/constant_expression

[jit_llvm]
def test_min2 ( a, b : int )
    return a < b ? a : b

[jit_llvm]
def test_min3 ( a, b, c : int )
    if a < b
        return a < c ? a : c
    else
        return b < c ? b : c

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
    t |> strictEqual ( a + b, add_jit(a,b) )

def test_sub( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> sub_jit) )
    t |> strictEqual ( a - b, sub_jit(a,b) )

def test_mul( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> mul_jit) )
    t |> strictEqual ( a * b, mul_jit(a,b) )

def test_div( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> div_jit) )
    if b != TT()
        t |> strictEqual ( a / b, div_jit(a,b) )

def test_mod( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> mod_jit) )
    if b != TT()
        t |> strictEqual ( a % b, mod_jit(a,b) )

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
        fuzz_numeric(t, fake, "test_add")
        fuzz_numeric(t, fake, "test_sub")
        fuzz_numeric(t, fake, "test_mul")
        fuzz_numeric(t, fake, "test_div")
        fuzz_numeric(t, fake, "test_mod")

options log
