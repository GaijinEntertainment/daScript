options gen2

require dastest/testing_boost
require daslib/fuzzer
require daslib/faker
require daslib/constant_expression

def add_jit(a, b) => a + b

def sub_jit(a, b) => a - b

def mul_jit(a, b) => a * b

def div_jit(a, b) => a / b

def mod_jit(a, b) => a % b

def test_add(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > add_jit))
    t |> strictEqual(a + b, add_jit(a, b), "a + b")
}

def test_sub(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > sub_jit))
    t |> strictEqual(a - b, sub_jit(a, b), "a - b")
}

def test_mul(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > mul_jit))
    t |> strictEqual(a * b, mul_jit(a, b), "a * b")
}

def test_div(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > div_jit))
    if (b != TT()) {
        t |> strictEqual(a / b, div_jit(a, b), "a / b")
    }
}


def test_div_vec(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > div_jit))
    t |> strictEqual(a / b, div_jit(a, b), "a / b")
}

def test_mod(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > mod_jit))
    if (b != TT()) {
        t |> strictEqual(a % b, mod_jit(a, b), "a % b")
    }
}

def add_equ_jit(a, b) {
    var c = a
    c += b
    return c
}

def sub_equ_jit(a, b) {
    var c = a
    c -= b
    return c
}

def mul_equ_jit(a, b) {
    var c = a
    c *= b
    return c
}

def div_equ_jit(a, b) {
    var c = a
    c /= b
    return c
}

def mod_equ_jit(a, b) {
    var c = a
    c %= b
    return c
}

def test_add_equ(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > add_equ_jit))
    t |> strictEqual(a + b, add_equ_jit(a, b), "a + b")
}

def test_sub_equ(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > sub_equ_jit))
    t |> strictEqual(a - b, sub_equ_jit(a, b), "a - b")
}

def test_mul_equ(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > mul_equ_jit))
    t |> strictEqual(a * b, mul_equ_jit(a, b), "a * b")
}

def test_div_equ(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > div_equ_jit))
    if (b != TT()) {
        t |> strictEqual(a / b, div_equ_jit(a, b), "a / b")
    }
}

def test_div_equ_vec(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > div_equ_jit))
    t |> strictEqual(a / b, div_equ_jit(a, b), "a / b")
}

def test_mod_equ(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > mod_equ_jit))
    if (b != TT()) {
        t |> strictEqual(a % b, mod_equ_jit(a, b), "a % b")
    }
}

[test(description = "JIT-compiled binary + - * / % and their compound-assignment forms return the interpreter's results on fuzzed operands")]
def test_arithmetics_op2(t : T?) {
    t |> run("+, -, *, /, %") @(t : T?) {
        var fake <- Faker()
        fuzz() {
            fuzz_numeric_and_vector_op2(t, fake, "test_add")
            fuzz_numeric_and_vector_op2(t, fake, "test_sub")
            fuzz_numeric_and_vector_op2(t, fake, "test_mul")
            fuzz_numeric_and_vector_op2(t, fake, "test_div")
            fuzz_numeric_op2(t, fake, "test_mod")
        }
    }

    t |> run("+=, -=, *=, /=, %=") @(t : T?) {
        var fake <- Faker()
        fuzz() {
            fuzz_numeric_and_vector_op2(t, fake, "test_add_equ")
            fuzz_numeric_and_vector_op2(t, fake, "test_sub_equ")
            fuzz_numeric_and_vector_op2(t, fake, "test_mul_equ")
            fuzz_numeric_and_vector_op2(t, fake, "test_div_equ")
            fuzz_numeric_op2(t, fake, "test_mod_equ")
        }
    }
}

[test(description = "JIT-compiled vector-by-scalar * / and *= /= return the interpreter's results on fuzzed operands")]
def test_arithmetics_vec_and_scalar_op2(t : T?) {
    if (HOST_FAST_MATH) {
        t |> skip("host runtime is built -ffast-math: vector division is a reciprocal multiply, not bit-equal to the scalar reference")
        return
    }
    t |> run("* / vector vs salar") @(t : T?) {
        var fake <- Faker()
        fuzz() {
            fuzz_numeric_vec_scal_op2(t, fake, "test_mul")
            fuzz_numeric_vec_scal_op2(t, fake, "test_div_vec")
            fuzz_numeric_scal_vec_op2(t, fake, "test_mul")
            fuzz_numeric_scal_vec_op2(t, fake, "test_div_vec")
        }
    }

    t |> run("*= /= vector vs scalar") @(t : T?) {
        var fake <- Faker()
        fuzz() {
            fuzz_numeric_vec_scal_op2(t, fake, "test_mul_equ")
            fuzz_numeric_vec_scal_op2(t, fake, "test_div_equ_vec")
        }
    }
}

def post_inc_jit(var a) => a++

def post_dec_jit(var a) => a--

def pre_inc_jit(var a) {
    ++a
    return a
}

def pre_dec_jit(var a) {
    --a
    return a
}

def test_post_inc(t : T?; a : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT) : void > post_inc_jit))
    if (a != a) {
        return
    }
    t |> strictEqual(a, post_inc_jit(a), "return a ++")
}

def test_post_dec(t : T?; a : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT) : void > post_dec_jit))
    if (a != a) {
        return
    }
    t |> strictEqual(a, post_dec_jit(a), "return a --")
}

def test_pre_inc(t : T?; a : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT) : void > pre_inc_jit))
    if (a != a) {
        return
    }
    t |> strictEqual(a + TT(1), pre_inc_jit(a), "return ++ a")  // the cast is redundant for TT=int, needed for other TT
}

def test_pre_dec(t : T?; a : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT) : void > pre_dec_jit))
    if (a != a) {
        return
    }
    t |> strictEqual(a - TT(1), pre_dec_jit(a), "return -- a")  // the cast is redundant for TT=int, needed for other TT
}

def neg_jit(a) => -a

def test_neg(t : T?; a : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT) : void > neg_jit))
    if (a != a) {
        return
    }
    t |> strictEqual(-a, neg_jit(a), "-a {a}")
}

[test(description = "JIT-compiled unary minus and the pre/post increment and decrement forms return the interpreter's results")]
def test_arithmetics_op1(t : T?) {
    t |> run("-") @(t : T?) {
        var fake <- Faker()
        fuzz() {
            fuzz_numeric_and_vector_op1(t, fake, "test_neg")
        }
    }
    t |> run("++, --, +++, ---") @(t : T?) {
        var fake <- Faker()
        fuzz() {
            fuzz_numeric_op1(t, fake, "test_post_inc")
            fuzz_numeric_op1(t, fake, "test_post_dec")
            fuzz_numeric_op1(t, fake, "test_pre_inc")
            fuzz_numeric_op1(t, fake, "test_pre_dec")
        }
    }
}

def shl_jit(a, b) => a << b

def test_shl(t : T?; a : auto(TTA); b : auto(TTB)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TTA; b : TTB) : void > shl_jit))
    t |> strictEqual(a << b, shl_jit(a, b), "a << b")
}

def shr_jit(a, b) => a >> b

def test_shr(t : T?; a : auto(TTA); b : auto(TTB)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TTA; b : TTB) : void > shr_jit))
    t |> strictEqual(a >> b, shr_jit(a, b), "a >> b")
}

def and_jit(a, b) => a & b

def test_and(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > and_jit))
    t |> strictEqual(a & b, and_jit(a, b), "a & b")
}

def or_jit(a, b) => a | b

def test_or(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > or_jit))
    t |> strictEqual(a | b, or_jit(a, b), "a | b")
}

def xor_jit(a, b) => a ^ b

def test_xor(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > xor_jit))
    t |> strictEqual(a ^ b, xor_jit(a, b), "a ^ b")
}

def and_equ_jit(a, b) {
    var c = a
    c &= b
    return c
}

def test_and_equ(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > and_equ_jit))
    t |> strictEqual(a & b, and_equ_jit(a, b), "a &= b")
}

def or_equ_jit(a, b) {
    var c = a
    c |= b
    return c
}

def test_or_equ(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > or_equ_jit))
    t |> strictEqual(a | b, or_equ_jit(a, b), "a |= b")
}

def xor_equ_jit(a, b) {
    var c = a
    c ^= b
    return c
}

def test_xor_equ(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > xor_equ_jit))
    t |> strictEqual(a ^ b, xor_equ_jit(a, b), "a ^= b")
}

def shl_equ_jit(a, b) {
    var c = a
    c <<= b
    return c
}

def test_shl_equ(t : T?; a : auto(TTA); b : auto(TTB)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TTA; b : TTB) : void > shl_equ_jit))
    t |> strictEqual(a << b, shl_equ_jit(a, b), "a <<= b")
}

def shr_equ_jit(a, b) {
    var c = a
    c >>= b
    return c
}

def test_shr_equ(t : T?; a : auto(TTA); b : auto(TTB)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TTA; b : TTB) : void > shr_equ_jit))
    t |> strictEqual(a >> b, shr_equ_jit(a, b), "a >>= b")
}

def rol_jit(a, b) => a <<< b

def test_rol(t : T?; a : auto(TTA); b : auto(TTB)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TTA; b : TTB) : void > rol_jit))
    t |> strictEqual(a <<< b, rol_jit(a, b), "a <<< b")
}

def ror_jit(a, b) => a >>> b

def test_ror(t : T?; a : auto(TTA); b : auto(TTB)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TTA; b : TTB) : void > ror_jit))
    t |> strictEqual(a >>> b, ror_jit(a, b), "a >>> b")
}

def rol_equ_jit(a, b) {
    var c = a
    c <<<= b
    return c
}

def test_rol_equ(t : T?; a : auto(TTA); b : auto(TTB)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TTA; b : TTB) : void > rol_equ_jit))
    t |> strictEqual(a <<< b, rol_equ_jit(a, b), "a <<<= b")
}

[test(description = "JIT-compiled binary bitwise operators and their compound-assignment forms return the interpreter's results on fuzzed operands")]
def test_bit_arithmetics_op2(t : T?) {
    t |> run("& | ^") @(t : T?) {
        var fake <- Faker()
        fuzz() {
            fuzz_int_vector_op2(t, fake, "test_and")
            fuzz_int_vector_op2(t, fake, "test_or")
            fuzz_int_vector_op2(t, fake, "test_xor")
            fuzz_shift_op2(t, fake, "test_shl")
            fuzz_shift_op2(t, fake, "test_shr")
            fuzz_rotate_op2(t, fake, "test_rol")
            fuzz_rotate_op2(t, fake, "test_ror")
        }
    }

    t |> run("&=") @(t : T?) {
        var fake <- Faker()
        fuzz() {
            fuzz_int_vector_op2(t, fake, "test_and_equ")
            fuzz_int_vector_op2(t, fake, "test_or_equ")
            fuzz_int_vector_op2(t, fake, "test_xor_equ")
            fuzz_shift_op2(t, fake, "test_shl_equ")
            fuzz_shift_op2(t, fake, "test_shr_equ")
            fuzz_rotate_op2(t, fake, "test_rol_equ")
        }
    }
}

def bit_neg_jit(a) => ~a

def test_bit_neg(t : T?; a : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT) : void > bit_neg_jit))
    t |> equal(~a, bit_neg_jit(a), "~a")
}

[constant_expression(funcname)]
def fuzz_int_uint(t : T?; var fake : Faker; funcname : string) {
    invoke(funcname, t, fake |> random_int)
    invoke(funcname, t, fake |> random_uint)
}

[test(description = "JIT-compiled bitwise complement ~ returns the interpreter's result on fuzzed operands")]
def test_bit_arithmetics_op1(t : T?) {
    t |> run("~") @(t : T?) {
        var fake <- Faker()
        fuzz() {
            fuzz_int_uint(t, fake, "test_bit_neg")
        }
    }
}

def jit_not(a : bool) => !a

def jit_xorxor(a, b : bool) => a ^^ b

def jit_set_xorxor(var a : bool; b : bool) {
    a ^^= b
    return a
}

var monad_test = 0

[sideeffects]
def getA_true {
    monad_test |= 1
    return true
}

[sideeffects]
def getA_false => false

[sideeffects]
def getB_true {
    monad_test |= 2
    return true
}

[sideeffects]
def getB_false {
    monad_test |= 4
    return false
}

def jit_andand_true_true => getA_true() && getB_true()

def jit_andand_true_false => getA_true() && getB_false()

def jit_andand_false_true => getA_false() && getB_true()

def jit_andand_false_false => getA_false() && getB_false()

def jit_set_andand_true_true {
    var a = getA_true()
    a &&= getB_true()
    return a
}

def jit_set_andand_true_false {
    var a = getA_true()
    a &&= getB_false()
    return a
}

def jit_set_andand_false_true {
    var a = getA_false()
    a &&= getB_true()
    return a
}

def jit_set_andand_false_false {
    var a = getA_false()
    a &&= getB_false()
    return a
}


def jit_oror_true_true => getA_true() || getB_true()

def jit_oror_true_false => getA_true() || getB_false()

def jit_oror_false_true => getA_false() || getB_true()

def jit_oror_false_false => getA_false() || getB_false()

def jit_set_oror_true_true {
    var a = getA_true()
    a ||= getB_true()
    return a
}

def jit_set_oror_true_false {
    var a = getA_true()
    a ||= getB_false()
    return a
}

def jit_set_oror_false_true {
    var a = getA_false()
    a ||= getB_true()
    return a
}

def jit_set_oror_false_false {
    var a = getA_false()
    a ||= getB_false()
    return a
}

[test(description = "JIT-compiled !, ^^, && and || and their compound-assignment forms return the interpreter's results, including short-circuit evaluation")]
def test_bool_arithmetics(t : T?) {    // nolint:STYLE038 -- flat list of independent JIT arms, one per boolean operator
    t |> run("!") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@jit_not))
        t |> equal(true, jit_not(false))
        t |> equal(false, jit_not(true))
    }
    t |> run("^^ ^^=") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@jit_xorxor))
        t |> equal(false, jit_xorxor(false, false))
        t |> equal(true,  jit_xorxor(true, false))
        t |> equal(true,  jit_xorxor(false, true))
        t |> equal(false, jit_xorxor(true, true))
        t |> success(!jit_enabled() || is_jit_function(@@jit_set_xorxor))
        t |> equal(false, jit_set_xorxor(false, false))
        t |> equal(true,  jit_set_xorxor(true, false))
        t |> equal(true,  jit_set_xorxor(false, true))
        t |> equal(false, jit_set_xorxor(true, true))
    }
    t |> run("&&") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@jit_andand_true_true))
        monad_test = 0
        let tt = jit_andand_true_true()
        t |> success(tt)
        t |> success(monad_test == 3)
        t |> success(!jit_enabled() || is_jit_function(@@jit_andand_true_false))
        monad_test = 0
        let tf = jit_andand_true_false()
        t |> success(!tf)
        t |> success(monad_test == 5)
        t |> success(!jit_enabled() || is_jit_function(@@jit_andand_false_true))
        monad_test = 0
        let ft = jit_andand_false_true()
        t |> success(!ft)
        t |> success(monad_test == 0)
        t |> success(!jit_enabled() || is_jit_function(@@jit_andand_false_false))
        monad_test = 0
        let ff = jit_andand_false_false()
        t |> success(!ff)
        t |> success(monad_test == 0)
    }
    t |> run("&&=") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@jit_set_andand_true_true))
        monad_test = 0
        let tt = jit_set_andand_true_true()
        t |> success(tt)
        t |> success(monad_test == 3)
        t |> success(!jit_enabled() || is_jit_function(@@jit_set_andand_true_false))
        monad_test = 0
        let tf = jit_set_andand_true_false()
        t |> success(!tf)
        t |> success(monad_test == 5)
        t |> success(!jit_enabled() || is_jit_function(@@jit_set_andand_false_true))
        monad_test = 0
        let ft = jit_set_andand_false_true()
        t |> success(!ft)
        t |> success(monad_test == 0)
        t |> success(!jit_enabled() || is_jit_function(@@jit_set_andand_false_false))
        monad_test = 0
        let ff = jit_set_andand_false_false()
        t |> success(!ff)
        t |> success(monad_test == 0)
    }
    t |> run("||") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@jit_oror_true_true))
        monad_test = 0
        let tt = jit_oror_true_true()
        t |> success(tt)
        t |> success(monad_test == 1)
        t |> success(!jit_enabled() || is_jit_function(@@jit_oror_true_false))
        monad_test = 0
        let tf = jit_oror_true_false()
        t |> success(tf)
        t |> success(monad_test == 1)
        t |> success(!jit_enabled() || is_jit_function(@@jit_oror_false_true))
        monad_test = 0
        let ft = jit_oror_false_true()
        t |> success(ft)
        t |> success(monad_test == 2)
        t |> success(!jit_enabled() || is_jit_function(@@jit_oror_false_false))
        monad_test = 0
        let ff = jit_oror_false_false()
        t |> success(!ff)
        t |> success(monad_test == 4)
    }
    t |> run("||=") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@jit_set_oror_true_true))
        monad_test = 0
        let tt = jit_set_oror_true_true()
        t |> success(tt)
        t |> success(monad_test == 1)
        t |> success(!jit_enabled() || is_jit_function(@@jit_set_oror_true_false))
        monad_test = 0
        let tf = jit_set_oror_true_false()
        t |> success(tf)
        t |> success(monad_test == 1)
        t |> success(!jit_enabled() || is_jit_function(@@jit_set_oror_false_true))
        monad_test = 0
        let ft = jit_set_oror_false_true()
        t |> success(ft)
        t |> success(monad_test == 2)
        t |> success(!jit_enabled() || is_jit_function(@@jit_set_oror_false_false))
        monad_test = 0
        let ff = jit_set_oror_false_false()
        t |> success(!ff)
        t |> success(monad_test == 4)
    }
}

def eq_jit(a, b) => a == b

def test_eq(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > eq_jit))
    t |> equal(a == b, eq_jit(a, b), "a == b")
}

def neq_jit(a, b) => a != b

def test_neq(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > neq_jit))
    t |> equal(a != b, neq_jit(a, b), "a != b")
}

def less_jit(a, b) => a < b

def test_less(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > less_jit))
    t |> equal(a < b, less_jit(a, b), "a < b")
}

def leq_jit(a, b) => a <= b

def test_leq(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > leq_jit))
    t |> equal(a <= b, leq_jit(a, b), "a <= b")
}

def gt_jit(a, b) => a > b

def test_gt(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > gt_jit))
    t |> equal(a > b, gt_jit(a, b), "a > b")
}

def geq_jit(a, b) => a >= b

def test_geq(t : T?; a, b : auto(TT)) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : TT; b : TT) : void > geq_jit))
    t |> equal(a >= b, geq_jit(a, b), "a >= b")
}

[test(description = "JIT-compiled == != >= <= > < return the interpreter's results on fuzzed operands")]
def test_compare_op2(t : T?) {
    t |> run("== != >= <= > <") @(t : T?) {
        var fake <- Faker()
        fuzz() {
            fuzz_eq_neq_op2(t, fake, "test_eq")
            fuzz_eq_neq_op2(t, fake, "test_neq")
            fuzz_compareable_op2(t, fake, "test_less")
            fuzz_compareable_op2(t, fake, "test_gt")
            fuzz_compareable_op2(t, fake, "test_leq")
            fuzz_compareable_op2(t, fake, "test_geq")
        }
    }
}

def div_jit(a : int, b : int) => a / b

def mod_jit(a : int, b : int) => a % b

def div64_jit(a : int64, b : int64) => a / b

def mod64_jit(a : int64, b : int64) => a % b

def set_div_jit(a : int, b : int) {
    var r = a
    r /= b
    return r
}

def set_mod_jit(a : int, b : int) {
    var r = a
    r %= b
    return r
}

def set_div64_jit(a : int64, b : int64) {
    var r = a
    r /= b
    return r
}

def set_mod64_jit(a : int64, b : int64) {
    var r = a
    r %= b
    return r
}

def expect_div_panic(t : T?; msg : string; blk : block) {
    var failed = false
    try {
        invoke(blk)
    } recover {
        failed = true
    }
    t |> success(failed, msg)
}

[test(description = "INT_MIN % -1 folds to zero and INT_MIN / -1 panics instead of trapping the process, while ordinary operands are unaffected")]
def test_division_overflow(t : T?) {
    t |> run("modulo folds to zero") @@(t : T?) {
        t |> equal(mod_jit(INT_MIN, -1), 0, "INT_MIN % -1")
        t |> equal(mod64_jit(LONG_MIN, -1l), 0l, "LONG_MIN % -1")
        t |> equal(set_mod_jit(INT_MIN, -1), 0, "INT_MIN %= -1")
        t |> equal(set_mod64_jit(LONG_MIN, -1l), 0l, "LONG_MIN %= -1")
    }
    t |> run("division panics") @@(t : T?) {
        expect_div_panic(t, "INT_MIN / -1") $ {
            print("{div_jit(INT_MIN, -1)}")
        }
        expect_div_panic(t, "LONG_MIN / -1") $ {
            print("{div64_jit(LONG_MIN, -1l)}")
        }
        expect_div_panic(t, "INT_MIN /= -1") $ {
            print("{set_div_jit(INT_MIN, -1)}")
        }
        expect_div_panic(t, "LONG_MIN /= -1") $ {
            feint("{set_div64_jit(LONG_MIN, -1l)}")
        }
    }
    t |> run("plain values unaffected") @@(t : T?) {
        t |> equal(div_jit(-14, 7), -2, "-14 / 7")
        t |> equal(mod_jit(-14, 3), -2, "-14 % 3")
        t |> equal(div64_jit(-14l, 7l), -2l, "-14 / 7 int64")
    }
}

def div_checked(a : int, b : int) => a / b

[hint(unsafe_division_check)]
def div_unchecked(a : int, b : int) => a / b

[hint(unsafe_division_check)]
def mod_unchecked(a : int, b : int) => a % b

[hint(unsafe_division_check)]
def mod64_unchecked(a : int64, b : int64) => a % b

[hint(unsafe_division_check)]
def set_div_unchecked(a : int, b : int) {
    var r = a
    r /= b
    return r
}

[hint(unsafe_division_check)]
def set_mod_unchecked(a : int, b : int) {
    var r = a
    r %= b
    return r
}

[hint(unsafe_division_check)]
def count_divisors(n : int) {
    var count = 0
    for (i in range(1, n + 1)) {
        if (n % i == 0) {
            count++
        }
    }
    return count
}

[test(description = "a function hinted unsafe_division_check divides correctly on plain values and leaves un-hinted functions beside it panicking on INT_MIN / -1")]
def test_unsafe_division_check(t : T?) {
    t |> run("plain values under the hint") @@(t : T?) {
        t |> equal(div_unchecked(-14, 7), -2, "-14 / 7")
        t |> equal(mod_unchecked(-14, 3), -2, "-14 % 3")
        t |> equal(mod64_unchecked(-14l, 3l), -2l, "-14 % 3 int64")
        t |> equal(set_div_unchecked(100, 7), 14, "100 /= 7")
        t |> equal(set_mod_unchecked(100, 7), 2, "100 %= 7")
        t |> equal(count_divisors(360), 24, "divisors of 360")
    }
    t |> run("the hint stays with its function") @@(t : T?) {
        var panicked = false
        try {
            feint("{div_checked(INT_MIN, -1)}")
        } recover {
            panicked = true
        }
        t |> success(panicked, "an un-hinted INT_MIN / -1 beside hinted functions still panics")
    }
}
