require dastest/testing_boost

require jit

require daslib/fuzzer
require daslib/faker
require daslib/math
require daslib/constant_expression

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
        fuzz_float_double_or_float_vec_op1(t,fake,"test_sqrt")

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
        fuzz_float_or_float_vec_op1(t,fake,"test_rcp")

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
        fuzz_float_double_or_float_vec_op1(t,fake,"test_exp")

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
    t |> equal ( clz(a), jit_clz(a), "clz(a)" )

[test]
def test_clz ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_all_unsigned_ints_op1(t,fake,"test_clz")

[jit]
def jit_min(a,b)
    return min(a,b)

def test_min( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a,b:TT):void> jit_min) )
    t |> strictEqual ( min(a,b), jit_min(a,b), "min(a,b)" )

[test]
def test_min ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_and_vector_op2(t,fake,"test_min")

[jit]
def jit_max(a,b)
    return max(a,b)

def test_max( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a,b:TT):void> jit_max) )
    t |> strictEqual ( max(a,b), jit_max(a,b), "max(a,b)" )

[test]
def test_max ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_and_vector_op2(t,fake,"test_max")

[jit]
def jit_abs(a)
    return abs(a)

def test_abs( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):TT> jit_abs) )
    t |> strictEqual ( abs(a), jit_abs(a), "abs(a)" )

[test]
def test_abs ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_and_vector_signed_op1(t,fake,"test_abs")

[jit]
def jit_saturate(a)
    return saturate(a)

def test_saturate( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):TT> jit_saturate) )
    t |> strictEqual ( saturate(a), jit_saturate(a), "saturate(a)" )

[test]
def test_saturate ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_float_or_float_vec_op1(t,fake,"test_saturate")

/*
[jit]
def jit_mad(a,b,c)
    return mad(a,b,c)

def test_mad( t:T?; a:auto(TT); b:auto(QQ); c:TT )
    t |> success ( is_jit_function(@@<(a:TT;b:QQ;c:TT):TT> jit_mad) )
    t |> strictEqual ( mad(a,b,c), jit_mad(a,b,c), "mad({a},{b},{c})" )

[test]
def test_mad ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_numeric_op3(t,fake,"test_mad")
    fuzz <|
        fuzz_vec_op3(t,fake,"test_mad")
    fuzz <|
        fuzz_vec_mad_op3(t,fake,"test_mad")

[jit]
def jit_lerp(a,b,c)
    return lerp(a,b,c)

def test_lerp( t:T?; a:auto(TT); b:auto(QQ); c:TT )
    t |> success ( is_jit_function(@@<(a:TT;b:QQ;c:TT):TT> jit_mad) )
    t |> strictEqual ( lerp(a,b,c), jit_lerp(a,b,c), "lerp({a},{b},{c})" )

[test]
def test_lerp ( t:T? )
    var fake <- Faker()
    fuzz <|
        fuzz_float_double_or_float_vec_op3(t,fake,"test_lerp")
*/