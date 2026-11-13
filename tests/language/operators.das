options gen2
options no_unused_function_arguments = false
require dastest/testing_boost public
require _operators_derived

struct Foo {
    a : int
}

struct Bar {
    a : float
}

def operator as Bar(a : Foo) => Bar(a = float(a.a))

def operator ?as Bar(a : Foo?) : Bar? {
    if (a == null) {
        return null
    }
    return new Bar(a = float(a.a))
}

def operator is Bar(anything) => false

def operator is Bar(_a : Bar) => true

struct Goo {
    a : string
}

def operator [] (t : Goo; cmp : string) => "{t . . a}-{cmp}"

def operator ?[] (t : Goo; cmp : string) : string const?# {
    if (cmp == t . . a) {
        unsafe {
            return addr<string const?#>(t . . a)
        }
    } else {
        unsafe {
            return reinterpret<string const?#>(null)
        }
    }
}

def operator .(t : Goo; name : string) => Goo(a = "{t . . a}.{name}")

def operator ?.(t : Goo; name : string) : string const?# {
    if (name == "yes") {
        unsafe {
            return addr<string const?#>(t . . a)
        }
    } else {
        unsafe {
            return reinterpret<string const?#>(null)
        }
    }
}

def operator . length(t : Goo) => length(t . . a)

def operator ?. length(t : Goo) => length(t . . a) + 1

[test(description = "user as, ?as and is operators convert and test a struct, and user [], ?[], . and ?. operators with named property getters answer on a struct and on a class property inherited across required modules")]
def test_operators(t : T?) {
    t |> run("as, ?as, is") @(t : T?) {
        var f = Foo(a = 13)
        let b = f as Bar
        t |> equal(13., b.a)
        t |> success(b is Bar)
        t |> success(!(f is Bar))
        var pf = unsafe(addr(f))
        let pb = pf ?as Bar ?? Bar(a = 0.0)
        t |> equal(13., pb.a)
        var nf : Foo?
        let nb = nf ?as Bar ?? Bar(a = 1.0)
        t |> equal(1., nb.a)
    }
    t |> run("[], ?[], ., ?.") @(t : T?) {
        var T = Goo(a = "hello")
        let z = T["blah"]
        let w = T?["blah"] ?? "not_blah"
        let W = T?["hello"] ?? "this???"
        let EZ = T.GooBar
        let HARD_YES = T?.yes ?? "how??"
        let HARD_NO = T?.no ?? "hard no"
        t |> equal("hello-blah", z)
        t |> equal("not_blah", w)
        t |> equal("hello", W)
        t |> equal("hello.GooBar", EZ . . a)
        t |> equal("hello", HARD_YES)
        t |> equal("hard no", HARD_NO)
        t |> equal(5, T.length)
        t |> equal(6, T?.length)

        var bar : BarOp? <- new BarOp(a = 42)
        t |> equal(42, bar.Prop)
    }
}

struct BitArray {
    bits : uint64
    def operator [] (index : int) : bool {
        assert(index >= 0 && index < 64)
        let mask = (1ul << uint64(index))
        return (bits & mask) != 0ul
    }
    def operator [] =(index : int, value : bool) {
        assert(index >= 0 && index < 64)
        let mask = (1ul << uint64(index))
        if (value) {
            bits = bits | mask
        } else {
            bits = bits & ~mask
        }
    }
    def static clear(var self : BitArray) {
        self.bits = 0ul
    }
}

[test(description = "a struct's [] and [] = method operators read and write single bits of a 64-bit word, and ||=, &&= and ^^= on an element go through them")]
def test_index_operators(t : T?) {
    t |> run("[], []=") @(t : T?) {
        var a : BitArray
        t |> equal(a.bits, 0ul)
        a[1] = true
        t |> equal(a.bits, 2ul)
        t |> equal(a[1], true)
        a.clear()
        // all bits
        for (bit in 0..64) {
            a[bit] = true
        }
        for (bit in 0..64) {
            t |> equal(a[bit], true)
        }
        t |> equal(a.bits, 0xfffffffffffffffful)
        for (bit in 0..64) {
            a[bit] = false
        }
        for (bit in 0..64) {
            t |> equal(a[bit], false)
        }
        t |> equal(a.bits, 0ul)
        a.clear()
        // checkerboard
        for (bit in 0..32) {
            a[bit * 2] = true
        }
        for (bit in 0..64) {
            t |> equal(a[bit], bit % 2 == 0)
        }
        a.clear()
        for (bit in 0..32) {
            a[bit * 2 + 1] = true
        }
        for (bit in 0..64) {
            t |> equal(a[bit], bit % 2 == 1)
        }
        a.clear()
    }
    t |> run("compound operators") @(t : T?) {
        var a : BitArray
        a.clear()
        a[0] ||= true
        t |> equal(a[0], true)
        a[0] &&= false
        t |> equal(a[0], false)
        a[0] ^^= true
        t |> equal(a[0], true)
        a[0] ^^= true
        t |> equal(a[0], false)
    }
}

// ===== raw (original) operator family — `!` before an access/test
// operator bypasses the overload =====

def operator [] (_a : array<float>; _i : int) : float => -999.0

def operator ?[] (_a : array<float>; _i : int) : float? => null

def operator [] =(var _a : array<float>; _i : int; _v : float) {
    pass    // a lying setter: swallows the write
}

struct Qoo {
    v : int
}

def operator ??(_p : Qoo? const; _d : Qoo const) : Qoo => Qoo(v = -777)

variant IntFloat {
    i : int
    f : float
}

def operator is i(_v : IntFloat) => false

def operator as i(_v : IntFloat) => -888

def operator ?as i(_v : IntFloat) : int? => null

def operator is int(_g : Goo) => true

[test(description = "a ! prefix on ., ?., [], ?[], ??, is, as and ?as bypasses the user operator overload and reaches the builtin field, element, coalescing or variant check")]
def test_raw_operators(t : T?) {
    t |> run("!. and !?. reach the real field") @(t : T?) {
        var g = Goo(a = "hello")
        t |> equal("hello", g!.a)                    // g.GooBar promotes, g!.a does not
        t |> equal("hello.GooBar", g.GooBar!.a)
        t |> equal(g . . a, g!.a)                    // legacy spelling stays equivalent
        var pg = unsafe(addr(g))
        t |> equal("hello", pg!?.a ?? "dflt")
        var ng : Goo?
        t |> equal("dflt", ng!?.a ?? "dflt")
    }
    t |> run("![ and !?[ reach the real element") @(t : T?) {
        let xs <- [1.0, 2.0, 3.0]
        t |> equal(-999.0, xs[1])                    // the overload shadows builtin indexing
        t |> equal(2.0, xs![1])
        t |> equal(-1.0, xs?[1] ?? -1.0)             // the ?[] overload yields null
        t |> equal(2.0, xs!?[1] ?? -1.0)
        t |> equal(-1.0, xs!?[9] ?? -1.0)            // raw safe-index still bounds-checks
    }
    t |> run("![ bypasses the write path too") @(t : T?) {
        var ws <- [1.0, 2.0, 3.0]
        ws[1] = 55.0                                 // the lying []= swallows the write
        t |> equal(2.0, ws![1])
        ws![1] = 55.0                                // raw write reaches the real element
        t |> equal(55.0, ws![1])
        ws![1] += 1.0                                // nolint:PERF013 - raw compound write is the tested construct
        t |> equal(56.0, ws![1])
    }
    t |> run("!?? reaches builtin pointer coalescing") @(t : T?) {
        var q = Qoo(v = 13)
        var pq = unsafe(addr(q))
        t |> equal(-777, (pq ?? Qoo(v = 1)).v)       // the ?? overload wins
        t |> equal(13, (pq !?? Qoo(v = 1)).v)
        var nq : Qoo?
        t |> equal(1, (nq !?? Qoo(v = 1)).v)
    }
    t |> run("!is, !as, !?as reach the real variant") @(t : T?) {
        let vif = IntFloat(i = 42)
        t |> success(!(vif is i))                    // the lying `is` operator wins
        t |> success(vif !is i)
        t |> equal(-888, vif as i)                   // the lying `as` operator wins
        t |> equal(42, vif !as i)
        t |> equal(-1, vif ?as i ?? -1)              // the lying `?as` operator wins
        t |> equal(42, unsafe(vif !?as i) ?? -1)
    }
    t |> run("!is type form reaches the static check") @(t : T?) {
        let g = Goo(a = "x")
        t |> success(g is int)                       // the lying `is int` operator wins
        t |> success(!(g !is int))
    }
}

struct ArraySlots {
    slots : array<array<int>>
    def operator [] <-(index : int, var value : array<int>) {
        slots[index] <- value
    }
}

struct NamedRows {
    rows : table<string; array<int>>
}

def operator [] <-(var n : NamedRows; key : string; var value : array<int>) {
    n.rows[key] <- value
}

struct RefGrid {
    cells : array<array<int>>
}

def operator [] (var g : RefGrid; index : int) : array<int>& {
    unsafe {
        return g.cells[index]
    }
}

[test(description = "the []<- operator in free and method form, and <- through a [] that returns a reference, move the array into the slot and empty the source")]
def test_index_move_operator(t : T?) {
    t |> run("[]<- free form") @(t : T?) {
        var n : NamedRows
        var v <- [1, 2, 3]
        n["a"] <- v
        t |> equal(0, length(v))
        t |> equal(3, length(n.rows["a"]))
    }
    t |> run("[]<- method form") @(t : T?) {
        var s : ArraySlots
        s.slots |> resize(2)
        var v <- [4, 5]
        s[1] <- v
        t |> equal(0, length(v))
        t |> equal(2, length(s.slots[1]))
    }
    t |> run("move through operator [] ref") @(t : T?) {
        var g : RefGrid
        g.cells |> resize(1)
        var v <- [1, 2]
        g[0] <- v
        t |> equal(0, length(v))
        t |> equal(2, length(g.cells[0]))
    }
}

struct iVec2 {
    x, y : int
}

def iVec2(X, Y : int) => iVec2(x = X, y = Y)

def operator -(a : iVec2) : iVec2 => iVec2(x = -a.x, y = -a.y)

def operator +(a, b : iVec2) : iVec2 => iVec2(x = a.x + b.x, y = a.y + b.y)

def operator ==(a, b : iVec2) : bool => (a.x == b.x) && (a.y == b.y)

[test(description = "user unary minus, binary plus and == operators on a struct compose in one expression")]
def test_operator_overload(t : T?) {
    t |> run("negate operator") @(t : T?) {
        let a = iVec2(4, 5)
        t |> success(-a == iVec2(-4, -5))
    }
    t |> run("add operator") @(t : T?) {
        let a = iVec2(4, 5)
        let b = iVec2(6, 7)
        t |> success(a + b == iVec2(10, 12))
    }
}

struct CastFoo {
    x, y : int
}

struct CastFoo3D : CastFoo {
    z : int
}

struct CastFooVision {
    f3d : CastFoo3D
    color : float4
}

def setFooGeneric(var foo; x, y) {
    foo.x = x
    foo.y = y
}

def setFoo(var foo : CastFoo; x, y : int) {
    foo.x = x
    foo.y = y
}

def setFooPtr(var foo : CastFoo?; x, y : int) {
    foo.x = x
    foo.y = y
}

[test(description = "cast to the base of a derived struct value or pointer lets base-typed code write the derived object's base fields, and reinterpret of a struct reads its bytes as another layout")]
def test_cast(t : T?) {
    t |> run("cast value struct") @(t : T?) {
        var f : CastFoo
        var f3d : CastFoo3D
        t |> equal(f.x, 0)
        t |> equal(f.y, 0)
        t |> equal(f3d.x, 0)
        t |> equal(f3d.y, 0)
        setFooGeneric(f, 1, 2)
        setFooGeneric(f3d, 1, 2)
        t |> equal(f.x, 1)
        t |> equal(f.y, 2)
        t |> equal(f3d.x, 1)
        t |> equal(f3d.y, 2)
        setFoo(f, 3, 4)
        setFoo(cast<CastFoo>(f3d), 3, 4)
        t |> equal(f.x, 3)
        t |> equal(f.y, 4)
        t |> equal(f3d.x, 3)
        t |> equal(f3d.y, 4)
        (cast<CastFoo>(f3d)).y = 5
        t |> equal(f3d.y, 5)
        var fv : CastFooVision
        (cast<CastFoo>(fv.f3d)).y = 13
        t |> equal((cast<CastFoo>(fv.f3d)).y, 13)
    }
    t |> run("cast pointer struct") @(t : T?) {
        var f = new CastFoo
        var f3d = new CastFoo3D
        t |> equal(f.x, 0)
        t |> equal(f.y, 0)
        t |> equal(f3d.x, 0)
        t |> equal(f3d.y, 0)
        setFooGeneric(f, 1, 2)
        setFooGeneric(f3d, 1, 2)
        t |> equal(f.x, 1)
        t |> equal(f.y, 2)
        t |> equal(f3d.x, 1)
        t |> equal(f3d.y, 2)
        setFooPtr(f, 3, 4)
        setFooPtr(cast<CastFoo?>(f3d), 3, 4)
        t |> equal(f.x, 3)
        t |> equal(f.y, 4)
        t |> equal(f3d.x, 3)
        t |> equal(f3d.y, 4)
        (cast<CastFoo?>(f3d)).y = 5
        t |> equal(f3d.y, 5)
        var fv = new CastFooVision
        (cast<CastFoo>(fv.f3d)).y = 13
        t |> equal((cast<CastFoo>(fv.f3d)).y, 13)
        unsafe {
            delete f
            delete f3d
            delete fv
        }
    }
    t |> run("reinterpret struct") @(t : T?) {
        let f2i = Int2Float(iv = 0x3f400000, fv = 1.0)
        let i2f = unsafe(reinterpret<Float2Int>(f2i))
        t |> equal(i2f.fv, 0.75)
        t |> equal(i2f.iv, 0x3f800000)
    }
}

struct Int2Float {
    iv : uint
    fv : float
}

struct Float2Int {
    fv : float
    iv : uint
}

[test(description = "arithmetic after a parenthesized reinterpret applies to the cast pointer at the target element stride, not to the operand")]
def test_cast_binds_tight(t : T?) {
    var buf : array<uint8>
    buf |> resize(4)
    unsafe {
        var p = reinterpret<uint8?>(addr(buf[0])) + 2
        *p = uint8(0xAB)
        var praw = reinterpret<void?>(addr(buf[0]))     // nolint:STYLE034 - the spelling under test
        var q = reinterpret<uint8?>(praw) + 3
        *q = uint8(0xCD)
    }
    t |> equal(uint(buf[0]), 0x0u)
    t |> equal(uint(buf[2]), 0xABu)
    t |> equal(uint(buf[3]), 0xCDu)
}

[test(description = "a postfix index after a parenthesized reinterpret applies to the cast result, not to the operand")]
def test_cast_postfix_applies_outside(t : T?) {
    var v = float3(1., 2., 3.)
    unsafe {
        let py = reinterpret<float?>(addr(v))[1]   // nolint:STYLE034 - the spelling under test
        t |> equal(py, 2.0)
    }
}

[sideeffects, no_jit]
def div_by_zero(num : auto(numT)) : numT {
    var ex = false
    var res = num
    try {
        res = num / numT(0)
    } recover {
        ex = true
    }
    assert(ex)
    return ex ? num : res
}

[sideeffects, no_jit]
def mod_by_zero(num : auto(numT)) : numT {
    var ex = false
    var res = num
    try {
        res = num % numT(0)
    } recover {
        ex = true
    }
    assert(ex)
    return ex ? num : res
}

[test(description = "integer division and modulo by zero raise a recoverable panic for int, uint, int64 and uint64")]
def test_div_by_zero(t : T?) {
    t |> run("int div and mod") @(t : T?) {
        t |> equal(div_by_zero(1), 1)
        t |> equal(mod_by_zero(1), 1)
    }
    t |> run("uint div and mod") @(t : T?) {
        t |> equal(div_by_zero(1u), 1u)
        t |> equal(mod_by_zero(1u), 1u)
    }
    t |> run("int64 div and mod") @(t : T?) {
        t |> equal(div_by_zero(1l), 1l)
        t |> equal(mod_by_zero(1l), 1l)
    }
    t |> run("uint64 div and mod") @(t : T?) {
        t |> equal(div_by_zero(1ul), 1ul)
        t |> equal(mod_by_zero(1ul), 1ul)
    }
}

var _0 = 0
var _1 = 1
var _2 = 2
var _4 = 4
var _8 = 8
var _30 = 30
var _31 = 31
var _32 = 32
var _33 = 33
var _36 = 36

def test_shl_scalar(t : T?) {
    t |> equal(1 << _1, 2)
    t |> equal(1 << _31, INT_MIN)
    t |> equal(0xFFFFFFFF << uint(_0), 0xFFFFFFFF)
    t |> equal(1 << _32, 1)
    t |> equal(1u << uint(-_1), 0x80000000)
    let x = -8
    t |> equal(x >> _1, -_4)
    t |> equal(x >> _31, -_1)
    t |> equal(x >> _32, -_8)
    t |> equal(x >> -_1, -_1)
    let y = 0x80000000
    t |> equal(y >> uint(_1), 0x40000000)
    t |> equal(y >> uint(_31), 0x1)
    t |> equal(y >> uint(_32), 0x80000000)
}

def test_rotational_shifts(t : T?) {
    t |> equal(0x12345678 <<< uint(_4), 0x23456781)
    t |> equal(0x12345678 <<< uint(_36), 0x23456781)
    t |> equal(0x12345678 <<< uint(_0), 0x12345678)
    t |> equal(0x12345678 >>> uint(_0), 0x12345678)
    var x1 = 0x12345678
    x1 >>>= uint(_0)
    t |> equal(x1, 0x12345678)
    x1 <<<= uint(_0)
    t |> equal(x1, 0x12345678)
    x1 >>>= uint(_4)
    t |> equal(x1, 0x81234567)
    var x2 = 0x12345678
    x2 <<<= uint(_36)
    t |> equal(x2, 0x23456781)
    var x3 = 0x12345678
    x3 <<<= uint(-_4)
    t |> equal(x3, 0x81234567)
}

def test_vector_shifts(t : T?) {
    let vi2 : int2 = int2(-8, 16)
    t |> equal(vi2 << _2, int2(-32, 64))
    t |> equal(vi2 >> -_30, int2(-2, 4))
    let vi3 : int3 = int3(-1l, 0x7FFFFFFFl, -0x80000000l)
    t |> equal(vi3 >> -_31, int3(-1, 0x3FFFFFFF, -1073741824))
    let vu4 : uint4 = uint4(0x80000000, 0xC0000000, 0xE0000000, 0xF0000000)
    t |> equal(vu4 >> _2, uint4(0x20000000, 0x30000000, 0x38000000, 0x3c000000))
    let vu2 : uint2 = uint2(0xFFFFFFFF, 0x12345678)
    t |> equal(vu2 << _0, uint2(0xFFFFFFFF, 0x12345678))
    t |> equal(vu2 >> _33, uint2(0x7FFFFFFF, 0x91a2b3c))
}

class ShiftA {
    x : int
}

def operator <<(a : ShiftA; y : int) => new ShiftA(x = a.x << y)
def operator >>(a : ShiftA; y : int) => new ShiftA(x = a.x >> y)
def operator <<=(var a : ShiftA; y : int) { a.x <<= y; }
def operator >>=(var a : ShiftA; y : int) { a.x >>= y; }
def operator <<(a : ShiftA; b : ShiftA) => new ShiftA(x = a.x << b.x)
def operator >>(a : ShiftA; b : ShiftA) => new ShiftA(x = a.x >> b.x)
def operator <<=(var a : ShiftA; b : ShiftA) { a.x <<= b.x; }
def operator >>=(var a : ShiftA; b : ShiftA) { a.x >>= b.x; }

def test_all_A_shifts(t : T?) {
    let a1 = new ShiftA(x = 8)
    let neg_a = new ShiftA(x = -8)
    t |> equal((*a1 << _1).x, 16)
    t |> equal((*a1 >> _1).x, 4)
    t |> equal((*neg_a >> _1).x, -4)
    t |> equal((*a1 << _32).x, 8)
    t |> equal((*a1 >> _33).x, 4)
    var a2 = new ShiftA(x = 8)
    *a2 <<= _1
    t |> equal(a2.x, 16)
    var a3 = new ShiftA(x = 16)
    *a3 >>= _2
    t |> equal(a3.x, 4)
    var a4 = new ShiftA(x = 1)
    *a4 <<= _31
    t |> equal(a4.x, INT_MIN)
    *a4 <<= _1
    t |> equal(a4.x, 0)
    let a5 = new ShiftA(x = 8)
    let shift_a = new ShiftA(x = 2)
    let zero_a = new ShiftA(x = 0)
    t |> equal((*a5 << *shift_a).x, 32)
    t |> equal((*a5 >> *shift_a).x, 2)
    t |> equal((*a5 << *zero_a).x, 8)
    t |> equal((*neg_a >> *shift_a).x, -2)
    var a6 = new ShiftA(x = 16)
    let shift_a2 = new ShiftA(x = 3)
    *a6 <<= *shift_a2
    t |> equal(a6.x, 128)
    *a6 >>= *shift_a2
    t |> equal(a6.x, 16)
}

[test(description = "scalar shifts take the count modulo the bit width, rotations and their compound forms rotate both ways, vector shifts apply per component, and user shift operators on a class accept int and class operands")]
def test_shifts(t : T?) {
    t |> run("scalar shift left/right") @(t : T?) { test_shl_scalar(t); }
    t |> run("rotational shifts") @(t : T?) { test_rotational_shifts(t); }
    t |> run("vector shifts") @(t : T?) { test_vector_shifts(t); }
    t |> run("custom operator shifts") @(t : T?) { test_all_A_shifts(t); }
}

def sab_panic_time(msg : string) {
    panic(msg)
    return false
}

[test(description = "||= and &&= short-circuit like || and && and never evaluate the right side once the result is decided")]
def test_setand_and_setor_bool(t : T?) {
    t |> run("short-circuit ||= and &&=") @(t : T?) {
        var libertad = true
        libertad = libertad || sab_panic_time("do not panic")
        libertad ||= sab_panic_time("now panic")
        libertad = false
        libertad = libertad && sab_panic_time("do not panic")
        libertad &&= sab_panic_time("now panic")
        t |> success(true)
    }
}

def safe_addr(var a) {
    unsafe {
        return addr(a)
    }
}

def safe_addr_val(var a : auto&) {
    unsafe {
        return addr(a)
    }
}

[test(description = "?[] with ?? on arrays, tables, fixed arrays and vectors, directly and through pointers, writes the element when it exists and the fallback when the index misses or the pointer is null")]
def test_safe_index(t : T?) {  // nolint:STYLE038 - flat list of independent test arms
    t |> run("dynamic array safe index") @(t : T?) {
        var ar : array<int>
        var dummy : int
        ar?[0] ?? dummy = 1
        t |> equal(dummy, 1)
        resize(ar, 1)
        ar?[0] ?? dummy = 2
        t |> equal(length(ar), 1)
        t |> equal(ar[0], 2)
        t |> equal(dummy, 1)
    }
    t |> run("array pointer safe index") @(t : T?) {
        var ar : array<int>
        var par = safe_addr(ar)
        var dummy = 0
        par?[0] ?? dummy = 1
        t |> equal(dummy, 1)
        resize(ar, 1)
        par?[0] ?? dummy = 2
        t |> equal(ar[0], 2)
        t |> equal(dummy, 1)
        par = null
        par?[0] ?? dummy = 3
        t |> equal(dummy, 3)
    }
    t |> run("table safe index") @(t : T?) {
        var ta : table<string; int>
        var dummy = 0
        ta?["one"] ?? dummy = 1
        t |> equal(dummy, 1)
        ta |> insert("one", 0)
        ta?["one"] ?? dummy = 2
        t |> equal(ta |> get_value("one"), 2)
        t |> equal(dummy, 1)
    }
    t |> run("table pointer safe index") @(t : T?) {
        var ta : table<string; int>
        var pta = safe_addr(ta)
        var dummy = 0
        pta?["one"] ?? dummy = 1
        t |> equal(dummy, 1)
        ta |> insert("one", 1)
        pta?["one"] ?? dummy = 2
        t |> equal(ta |> get_value("one"), 2)
        t |> equal(dummy, 1)
        pta = null
        pta?["one"] ?? dummy = 3
        t |> equal(dummy, 3)
    }
    t |> run("fixed array safe index") @(t : T?) {
        var di : int[2]
        var dummy = 0
        di?[3] ?? dummy = 1
        t |> equal(dummy, 1)
        di?[0] ?? dummy = 2
        t |> equal(di[0], 2)
        t |> equal(dummy, 1)
    }
    t |> run("fixed array pointer safe index") @(t : T?) {
        var di : int[2]
        var pdi = safe_addr(di)
        var dummy = 0
        pdi?[3] ?? dummy = 1
        t |> equal(dummy, 1)
        di[0] = 0
        pdi?[0] ?? dummy = 2
        t |> equal(di[0], 2)
        t |> equal(dummy, 1)
        pdi = null
        pdi?[0] ?? dummy = 3
        t |> equal(dummy, 3)
    }
    t |> run("vector safe index") @(t : T?) {
        var vec = int3(1, 2, 3)
        var dummy = 0
        vec?[3] ?? dummy = 1
        t |> equal(dummy, 1)
        vec?[0] ?? dummy = 2
        t |> equal(vec[0], 2)
        t |> equal(dummy, 1)
    }
    t |> run("vector pointer safe index") @(t : T?) {
        var vec = int3(1, 2, 3)
        var pvec = safe_addr_val(vec)
        var dummy = 0
        pvec?[3] ?? dummy = 1
        t |> equal(dummy, 1)
        vec[0] = 0
        pvec?[0] ?? dummy = 2
        t |> equal(vec[0], 2)
        t |> equal(dummy, 1)
        pvec = null
        pvec?[0] ?? dummy = 3
        t |> equal(dummy, 3)
    }
}

[test(description = "?[] with a uint index on arrays, fixed arrays and vectors, directly and through pointers, writes in-range elements and the fallback past the end or through a null pointer")]
def test_safe_index_uint(t : T?) {
    t |> run("dynamic array safe index uint") @(t : T?) {
        var ar : array<int>
        var dummy = 0
        ar?[0u] ?? dummy = 1
        t |> equal(dummy, 1)
        resize(ar, 2)
        ar?[0u] ?? dummy = 10
        t |> equal(ar[0], 10)
        ar?[1u] ?? dummy = 20
        t |> equal(ar[1], 20)
        ar?[2u] ?? dummy = 99
        t |> equal(dummy, 99)
    }
    t |> run("fixed array safe index uint") @(t : T?) {
        var di : int[3]
        var dummy = 0
        di?[0u] ?? dummy = 10
        t |> equal(di[0], 10)
        di?[2u] ?? dummy = 30
        t |> equal(di[2], 30)
        di?[3u] ?? dummy = 99
        t |> equal(dummy, 99)
    }
    t |> run("vector safe index uint") @(t : T?) {
        var vec = int3(1, 2, 3)
        var dummy = 0
        vec?[0u] ?? dummy = 10
        t |> equal(vec[0], 10)
        vec?[2u] ?? dummy = 30
        t |> equal(vec[2], 30)
        vec?[3u] ?? dummy = 99
        t |> equal(dummy, 99)
    }
    t |> run("array pointer safe index uint") @(t : T?) {
        var ar : array<int>
        var par = safe_addr(ar)
        var dummy = 0
        par?[0u] ?? dummy = 1
        t |> equal(dummy, 1)
        resize(ar, 1)
        par?[0u] ?? dummy = 2
        t |> equal(ar[0], 2)
        par = null
        par?[0u] ?? dummy = 3
        t |> equal(dummy, 3)
    }
    t |> run("fixed array pointer safe index uint") @(t : T?) {
        var di : int[2]
        var pdi = safe_addr(di)
        var dummy = 0
        pdi?[3u] ?? dummy = 1
        t |> equal(dummy, 1)
        di[0] = 0
        pdi?[0u] ?? dummy = 2
        t |> equal(di[0], 2)
        pdi = null
        pdi?[0u] ?? dummy = 3
        t |> equal(dummy, 3)
    }
    t |> run("vector pointer safe index uint") @(t : T?) {
        var vec = int3(1, 2, 3)
        var pvec = safe_addr_val(vec)
        var dummy = 0
        pvec?[3u] ?? dummy = 1
        t |> equal(dummy, 1)
        vec[0] = 0
        pvec?[0u] ?? dummy = 2
        t |> equal(vec[0], 2)
        pvec = null
        pvec?[0u] ?? dummy = 3
        t |> equal(dummy, 3)
    }
}

[test(description = "?[] reads the in-range elements of arrays, fixed arrays, vectors and tables and yields the ?? fallback for a missing index or key")]
def test_safe_index_read(t : T?) {
    t |> run("array safe index read") @(t : T?) {
        let ar <- [1, 2, 3]
        var val = ar?[1] ?? 0
        t |> equal(val, 2)
        val = ar?[5] ?? 42
        t |> equal(val, 42)
    }
    t |> run("fixed array safe index read") @(t : T?) {
        let di = fixed_array(10, 20, 30)
        var val = di?[1] ?? 0
        t |> equal(val, 20)
        val = di?[5] ?? 42
        t |> equal(val, 42)
    }
    t |> run("vector safe index read") @(t : T?) {
        let vec = int3(10, 20, 30)
        var val = vec?[1] ?? 0
        t |> equal(val, 20)
        val = vec?[3] ?? 42
        t |> equal(val, 42)
    }
    t |> run("table safe index read") @(t : T?) {
        let ta <- {"a" => 1, "b" => 2}
        var val = ta?["a"] ?? 0
        t |> equal(val, 1)
        val = ta?["c"] ?? 42
        t |> equal(val, 42)
    }
}

[never_inline]
def private take_fallback(b : block<(v : float) : float>) : float => invoke(b, 1.0)

[never_inline]
def private coalesce_const(p : float const?) : float => take_fallback() $(v : float) => p ?? v

[test(description = "?? on a const pointer inside a block returns the pointee when non-null and the block argument when null")]
def test_null_coalescing_const_pointer_const_value(t : T?) {
    t |> equal(coalesce_const(null), 1.0)
    let x = 2.0
    t |> equal(coalesce_const(unsafe(addr(x))), 2.0)
}

struct SafeOpFoo {
    a : string
}

def operator [] (tf : SafeOpFoo; cmp : string) : string => "{tf . . a}-{cmp}"

def operator ?[] (tf : SafeOpFoo; cmp : string) : string const?# {
    if (cmp == tf . . a) {
        unsafe {
            return addr<string const?#>(tf . . a)
        }
    } else {
        unsafe {
            return reinterpret<string const?#>(null)
        }
    }
}

def operator .(tf : SafeOpFoo; name : string) : SafeOpFoo => SafeOpFoo(a = "{tf . . a}.{name}")

def operator ?.(tf : SafeOpFoo; name : string) : string const?# {
    if (name == "yes") {
        unsafe {
            return addr<string const?#>(tf . . a)
        }
    } else {
        unsafe {
            return reinterpret<string const?#>(null)
        }
    }
}

[test(description = "user [], ?[], . and ?. operators on a struct return the computed value or address on a hit and fall back through ?? on a miss")]
def test_safe_operators(t : T?) {
    t |> run("operator [] on struct") @(t : T?) {
        let tf = SafeOpFoo(a = "hello")
        t |> equal(tf["blah"], "hello-blah")
    }
    t |> run("operator ?[] miss and hit") @(t : T?) {
        var tf = SafeOpFoo(a = "hello")
        t |> equal(tf?["blah"] ?? "not_blah", "not_blah")
        t |> equal(tf?["hello"] ?? "this???", "hello")
    }
    t |> run("operator . on struct") @(t : T?) {
        let tf = SafeOpFoo(a = "hello")
        let ez = tf.fooBar
        t |> equal(ez . . a, "hello.fooBar")
    }
    t |> run("operator ?. hit and miss") @(t : T?) {
        var tf = SafeOpFoo(a = "hello")
        t |> equal(tf?.yes ?? "how??", "hello")
        t |> equal(tf?.no ?? "hard no", "hard no")
    }
}

def rp_addX(a, b) => a + b

[test(description = "|> chains calls left to right and <| supplies the last argument of the same call")]
def test_rpipe(t : T?) {
    t |> run("pipe operators") @(t : T?) {
        let v = 12 |> rp_addX(2) |> rp_addX(3)
        t |> equal(v, 17)
        var c = 1 |> rp_addX <| 2
        c ++
        t |> equal(c, 4)
    }
}

struct LateOpCell {
    v : int
}

struct LateOpHolder {
    x : LateOpCell
}

[test(description = "a [], ?? or property read ahead of a user operator whose signature names a later typedef waits for that operator, and does not take the built-in element, pointer or field read")]
def test_operator_ahead_of_unresolved_signature(t : T?) {
    let row : LateOpCell[2]
    let element = row[1]
    t |> equal(element, 42)
    var missing : LateOpCell?
    let fallback : LateOpCell
    let coalesced = missing ?? fallback
    t |> equal(coalesced, 103)
    let holder : LateOpHolder
    let property = holder.x
    t |> equal(property, 101)
}

def operator [] (_a : LateOpRow; _i : int) : int => 42

def operator ??(_a : LateOpCellPtr; _b : LateOpCellValue) : int => 103

def operator . x(_a : LateOpHolderValue) : int => 101

typedef LateOpRow = LateOpCell[2]

typedef LateOpCellPtr = LateOpCell?

typedef LateOpCellValue = LateOpCell

typedef LateOpHolderValue = LateOpHolder

struct LatePairHolder {
    v : int
}

[test(description = "a property read ahead of its named operator, whose signature names a later typedef, waits for it and does not take the by-name operator . of the same struct")]
def test_named_property_ahead_of_by_name_operator(t : T?) {
    let holder : LatePairHolder
    t |> equal(holder.x, 101)
}

def operator . x(_h : LatePairHolderValue) : int => 101

def operator .(_h : LatePairHolder; _name : string) : int => 7

typedef LatePairHolderValue = LatePairHolder

struct LateByNameFoo {
    a : int
}

struct LateByNameBar {
    a : float
}

var g_late_by_name_named = 101

var g_late_by_name_other = 7

[test(description = "is, as and ?. ahead of their named operator, whose signature names a later typedef, wait for it and do not take the by-name operator of the same type")]
def test_named_operator_ahead_of_by_name_operator(t : T?) {
    var foo : LateByNameFoo
    t |> success(foo is LateByNameBar)
    t |> equal(foo as LateByNameBar, 101)
    t |> equal(unsafe(addr(foo))?.x ?? 0, 101)
}

def operator is LateByNameBar(_f : LateByNameFooValue) : bool => true

def operator is(_f : LateByNameFoo; _name : string) : bool => false

def operator as LateByNameBar(_f : LateByNameFooValue) : int => 101

def operator as(_f : LateByNameFoo; _name : string) : int => 7

def operator ?. x(_f : LateByNameFooPtr) : int? => unsafe(addr(g_late_by_name_named))

def operator ?.(_f : LateByNameFoo?; _name : string) : int? => unsafe(addr(g_late_by_name_other))

typedef LateByNameFooValue = LateByNameFoo

typedef LateByNameFooPtr = LateByNameFoo?
