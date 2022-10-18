require dastest/testing_boost

require llvm/llvm_jit
require daslib/fuzzer
require daslib/faker
require daslib/math
require daslib/constant_expression

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
        fuzz_numeric_op1(t,fake,"test_float")

[jit]
def jit_float4 ( x )
    return float4(x)

def test_float4( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_float4) )
    t |> strictEqual ( float4(a), jit_float4(a), "float4(a)" )

[test]
def test_intrin_float4 ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_op1(t,fake,"test_float4")

[jit]
def jit_sqrt ( x )
    return sqrt(x)

def test_sqrt( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_sqrt) )
    t |> strictEqual ( sqrt(a), jit_sqrt(a), "sqrt(a)" )

[test]
def test_intrin_sqrt ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_float_double_or_float_vec(t,fake,"test_sqrt")

[sideeffects,jit]
def jit_rcp ( x )
    return rcp(x)

def test_rcp( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_rcp) )
    jit_rcp(a)
    // note: we can't really compare, because precision is different
    //t |> equal ( rcp(a), jit_rcp(a), "rcp(a)" )

[test]
def test_intrin_rcp ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_float_or_float_vec(t,fake,"test_rcp")

[sideeffects,jit]
def jit_exp ( x )
    return exp(x)

def test_exp( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_exp) )
    jit_exp(a)
    // note: we can't really compare, because precision is different
    //t |> equal ( exp(a), jit_exp(a), "exp(a)" )

[test]
def test_intrin_exp ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_float_double_or_float_vec(t,fake,"test_exp")

[jit]
def jit_popcount(x)
    return popcnt(x)

def test_popcount( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_popcount) )
    t |> equal ( popcnt(a), jit_popcount(a), "popcnt(a)" )

[test]
def test_popcount ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_all_unsigned_ints_op1(t,fake,"test_popcount")

[jit]
def jit_ctz(x)
    return ctz(x)

def test_ctz( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_ctz) )
    t |> equal ( ctz(a), jit_ctz(a), "ctz(a)" )

[test]
def test_ctz ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_all_unsigned_ints_op1(t,fake,"test_ctz")

[jit]
def jit_clz(x)
    return clz(x)

def test_clz( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> jit_clz) )
    t |> equal ( clz(a), jit_clz(a), "clz({a})" )

[test]
def test_clz ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_all_unsigned_ints_op1(t,fake,"test_clz")
