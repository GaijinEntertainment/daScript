require dastest/testing_boost

require llvm/llvm_jit
require jit

require daslib/fuzzer
require daslib/faker
require daslib/constant_expression

[jit]
def shl_jit (a,b)
    return a << b

def test_shl( t:T?; a:auto(TTA); b:auto(TTB) )
    t |> success ( is_jit_function(@@<(a:TTA;b:TTB):void> shl_jit) )
    t |> strictEqual ( a << b, shl_jit(a,b), "a << b" )

[jit]
def shr_jit (a,b)
    return a >> b

def test_shr( t:T?; a:auto(TTA); b:auto(TTB) )
    t |> success ( is_jit_function(@@<(a:TTA;b:TTB):void> shr_jit) )
    t |> strictEqual ( a >> b, shr_jit(a,b), "a >> b" )

[jit]
def and_jit (a,b)
    return a & b

def test_and( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> and_jit) )
    t |> strictEqual ( a & b, and_jit(a,b), "a & b" )

[jit]
def or_jit (a,b)
    return a | b

def test_or( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> or_jit) )
    t |> strictEqual ( a | b, or_jit(a,b), "a | b" )

[jit]
def xor_jit (a,b)
    return a ^ b

def test_xor( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> xor_jit) )
    t |> strictEqual ( a ^ b, xor_jit(a,b), "a ^ b" )

[jit]
def and_equ_jit (a,b)
    var c = a
    c &= b
    return c

def test_and_equ( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> and_equ_jit) )
    t |> strictEqual ( a & b, and_equ_jit(a,b), "a &= b" )

[jit]
def or_equ_jit (a,b)
    var c = a
    c |= b
    return c

def test_or_equ( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> or_equ_jit) )
    t |> strictEqual ( a | b, or_equ_jit(a,b), "a |= b" )

[jit]
def xor_equ_jit (a,b)
    var c = a
    c ^= b
    return c

def test_xor_equ( t:T?; a,b:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT;b:TT):void> xor_equ_jit) )
    t |> strictEqual ( a ^ b, xor_equ_jit(a,b), "a ^= b" )

[jit]
def shl_equ_jit (a,b)
    var c = a
    c <<= b
    return c

def test_shl_equ( t:T?; a:auto(TTA); b:auto(TTB) )
    t |> success ( is_jit_function(@@<(a:TTA;b:TTB):void> shl_equ_jit) )
    t |> strictEqual ( a << b, shl_equ_jit(a,b), "a <<= b" )

[jit]
def shr_equ_jit (a,b)
    var c = a
    c >>= b
    return c

def test_shr_equ( t:T?; a:auto(TTA); b:auto(TTB) )
    t |> success ( is_jit_function(@@<(a:TTA;b:TTB):void> shr_equ_jit) )
    t |> strictEqual ( a >> b, shr_equ_jit(a,b), "a >>= b" )

[jit]
def rol_jit (a,b)
    return a <<< b

def test_rol( t:T?; a:auto(TTA); b:auto(TTB) )
    t |> success ( is_jit_function(@@<(a:TTA;b:TTB):void> rol_jit) )
    t |> strictEqual ( a <<< b, rol_jit(a,b), "a <<< b" )

[jit]
def ror_jit (a,b)
    return a >>> b

def test_ror( t:T?; a:auto(TTA); b:auto(TTB) )
    t |> success ( is_jit_function(@@<(a:TTA;b:TTB):void> ror_jit) )
    t |> strictEqual ( a >>> b, ror_jit(a,b), "a >>> b" )

[jit]
def rol_equ_jit (a,b)
    var c = a
    c <<<= b
    return c

def test_rol_equ( t:T?; a:auto(TTA); b:auto(TTB) )
    t |> success ( is_jit_function(@@<(a:TTA;b:TTB):void> rol_equ_jit) )
    t |> strictEqual ( a <<< b, rol_equ_jit(a,b), "a <<<= b" )

[test]
def test_bit_arithmetics_op2 ( t:T? )
    t |> run("& | ^") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            fuzz_int_vector_op2(t, fake, "test_and")
            fuzz_int_vector_op2(t, fake, "test_or")
            fuzz_int_vector_op2(t, fake, "test_xor")
            fuzz_shift_op2(t, fake, "test_shl")
            fuzz_shift_op2(t, fake, "test_shr")
            fuzz_rotate_op2(t, fake, "test_rol")
            fuzz_rotate_op2(t, fake, "test_ror")

    t |> run("&=") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            fuzz_int_vector_op2(t, fake, "test_and_equ")
            fuzz_int_vector_op2(t, fake, "test_or_equ")
            fuzz_int_vector_op2(t, fake, "test_xor_equ")
            fuzz_shift_op2(t, fake, "test_shl_equ")
            fuzz_shift_op2(t, fake, "test_shr_equ")
            fuzz_rotate_op2(t, fake, "test_rol_equ")

[jit]
def neg_jit (a)
    return ~a

def test_neg( t:T?; a:auto(TT) )
    t |> success ( is_jit_function(@@<(a:TT):void> neg_jit) )
    t |> equal ( ~a, neg_jit(a), "~a" )

[constant_expression(funcname)]
def fuzz_int_uint ( t:T?; var fake:Faker; funcname:string )
    invoke ( funcname, t, fake |> random_int )
    invoke ( funcname, t, fake |> random_uint )

[test]
def test_bit_arithmetics_op1 ( t:T? )
    t |> run("~") <| @ ( t : T? )
        var fake <- Faker()
        fuzz <|
            fuzz_int_uint(t, fake, "test_neg")
