options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/match
require daslib/ast_boost
require daslib/defer
require daslib/ast

enum Color {
    black
    red
    green
    blue
}

variant IF {
    i : int
    f : float
}

struct Foo {
    a : int
    v : IF
}

struct Bar {
    a : int
    b : float
}

struct AB {
    a, b : int
}

[sideeffects]
def enum_match(color : Color) {
    return match (color) {
        Color.black => 0
        Color.red => 1
        _ => -1
    }
}

[sideeffects]
def enum_static_match(color, blah) {
    return static_match (color) {
        Color.black => 0
        match_expr(blah) => 1
        _ => -1
    }
}

[sideeffects]
def static_match_by_type(what) {
    return static_match (what) {
        match_type(type<int>, expr) => expr
        _ => -1
    }
}

[sideeffects]
def struct_match(f : Foo) {
    match (f) {
        Foo(a = 13) => $ {
            return 0
        }
        Foo(a = anyA) => $ {
            return anyA
        }
    }
}

[sideeffects]
def struct_match(b : Bar) {
    return match (b) {
        Bar(a = 13) => 0
        Bar(a = anyA, b = 13.0) => anyA
        _ => -1
    }
}

[sideeffects]
def guards_match(ab : AB) {
    match (ab) {
        AB(a = a, b = b) && (b > a) => $ {
            return "{b} > {a}"
        }
        AB(a = a, b = b) => $ {
            return "{b} <= {a}"
        }
    }
}

[sideeffects]
def variant_as_match(v : IF) {
    return match (v) {
        i as i => "int"
        f as f => "float"
    }
}

[sideeffects]
def nested_struct_match(v : Foo) {
    return match (v) {
        Foo(a = 1, v = i as i) => 1
        Foo(v = f as f) => 2
        _ => 0
    }
}

[sideeffects]
def variant_match(v : IF) {
    return match (v) {
        IF(i = i) => 1
        IF(f = f) => 2
    }
}

[sideeffects]
def static_array_match(A : int[3]) {
    return match (A) {
        fixed_array(a, b, c) && (a + b + c) == 6 => 1  // total of 3 elements, sum is 6
        fixed_array<int>(0, ...) => 0  // starts with 0
        fixed_array<int>(..., 13) => 2  // ends with 13
        fixed_array<int>(12, ..., 12) => 3  // starts and ends with 12
        _ => -1
    }
}

[sideeffects]
def dynamic_array_match(A : array<int>) {
    return match (A) {
        [ a, b, c] && (a + b + c) == 6 => 1  // total of 3 elements, sum is 6
        array<int>(0, 0, 0, ...) => 0  // first 3 are 0
        array<int>(..., 1, 2) => 2  // ends with 1,2
        array<int>(0, 1, ..., 2, 3) => 3  // starts with 0,1, ends with 2,3
        _ => -1
    }
}


[sideeffects]
def ascending_array_match(A : int[3]) {
    return match (A) {
        fixed_array<int>(x, match_expr(x + 1), match_expr(x + 2)) => true
        _ => false
    }
}

[sideeffects]
def tuple_match(A : tuple<int; float; string>) {
    return match (A) {
        (1, _, "3") => 1
        (13, ...) => 2  // starts with 13
        (..., "13") => 3  // ends with "13"
        (2, ..., "2") => 4  // starts with 2, ends with "2"
        _ => 0
    }
}

[sideeffects]
def or_match(i : int) {
    return match (i) {
        1 | 2 => true
        _ => false
    }
}

[sideeffects]
def or_match_v(B : Bar) {
    return match (B) {
        Bar(a = 1, b = b) | Bar(a = 2, b = b) => b
        _ => 0.0
    }
}

[sideeffects]
def or_match_prior_capture(ab : AB) {
    return match (ab) {
        AB(a = a, b = 1 | 2) => a
        _ => -1
    }
}

[sideeffects]
def or_capture_both_branches(ab : AB) {
    return match (ab) {
        AB(a = a, b = (b && (b > 5)) | (b && (b < 5))) => a * 100 + b
        _ => -1
    }
}

[sideeffects]
def struct_ptr_match(f : Foo?) {
    return match (f) {
        null => -1
        Foo(a = 13) => 0
        Foo(a = anyA) => anyA
    }
}

[sideeffects]
def match_handle_ptr(T : TypeDeclPtr) {
    return match (T) {
        null => "null"
        TypeDecl(baseType = Type.tPointer) => "pointer"
        _ => "anything"
    }
}

[sideeffects]
def match_ast_expr_ptr(E : ExpressionPtr) {
    return match (E) {
        null => "null"
        ExprConstInt(value = v) => "int {v}"
        _ => "anything"
    }
}

[sideeffects]
def match_ast_op2_shape(E : ExpressionPtr) {
    return match (E) {
        ExprOp2(op = "+", left = ExprOp2(op = "*", left = a, right = b),
                right = c) => "mad {a.__rtti} {b.__rtti} {c.__rtti}"
        ExprOp2(op = "+" | "-", left = l) => "addsub {l.__rtti}"
        ExprSwizzle(mask = "xy", value = v) => "swizzle-xy {v.__rtti}"
        _ => "other"
    }
}

[sideeffects]
def match_op_string(o : das_string) {
    return match (o) {
        "+" => "plus"
        "-" => "minus"
        _ => "other"
    }
}

struct Cmd {
    rtti = "Cmd"
}

[match_as_is]
struct CmdMove : Cmd {
    override rtti = "CmdMove"
    x : float
    y : float
}

def operator is CmdMove(cmd : Cmd) => cmd.rtti == "CmdMove"

def operator is CmdMove(anything) => false

def operator as CmdMove(cmd : Cmd ==const) : CmdMove const& {
    assert(cmd.rtti == "CmdMove")
    unsafe {
        return reinterpret<CmdMove const&>(cmd)
    }
}

def operator as CmdMove(var cmd : Cmd ==const) : CmdMove& {
    assert(cmd.rtti == "CmdMove")
    unsafe {
        return reinterpret<CmdMove&>(cmd)
    }
}

def operator as CmdMove(anything) {
    panic("Cannot cast to CmdMove")
    return CmdMove()
}

[sideeffects]
def matching_as_and_is(cmd : Cmd) {
    return match (cmd) {
        CmdMove(x = x, y = y) => x + y
        _ => 0.
    }
}

[sideeffects]
def matching_type_as_is(cmd : Cmd) {
    return match (cmd) {
        match_type(type<CmdMove>, cmdm) => cmdm.x + cmdm.y
        _ => 0.
    }
}

[match_copy, safe_when_uninitialized]
struct CmdLocate : Cmd {
    override rtti = "CmdLocate"
    x : float
    y : float
    z : float
}

def match_copy(var cmdm : CmdLocate; cmd : Cmd) {
    if (cmd.rtti != "CmdLocate") {
        return false
    }
    cmdm = unsafe(reinterpret<CmdLocate const&>(cmd))
    return true
}

[sideeffects]
def matching_copy(cmd : Cmd) {
    return match (cmd) {
        CmdLocate(x = x, y = y, z = z) => x + y + z
        _ => 0.
    }
}

[sideeffects]
def matching_type_copy(cmd : Cmd) {
    return match (cmd) {
        match_type(type<CmdLocate>, cmdl) => cmdl.x + cmdl.y + cmdl.z
        _ => 0.
    }
}

[test(description = "Every match form (enum, struct, variant, array, tuple, or, pointer, AST and is/as patterns) selects the right arm")]
def test_all_match(t : T?) { // nolint:STYLE038 - flat list of independent match arms
    t |> run("enum match") @@(t : T?) {
        t |> equal(0, enum_match(Color.black))
        t |> equal(1, enum_match(Color.red))
        t |> equal(-1, enum_match(Color.green))
        t |> equal(-1, enum_match(Color.blue))
    }
    t |> run("enum static match") @@(t : T?) {
        t |> equal(0, enum_static_match(Color.black, null))
        t |> equal(1, enum_static_match(1234, 1234))
        t |> equal(-1, enum_static_match(5678, 1234))
        t |> equal(-1, enum_static_match("a", 1234))
    }
    t |> run("enum static match by type") @@(t : T?) {
        t |> equal(13, static_match_by_type(13))
        t |> equal(-1, static_match_by_type(13.))
    }
    t |> run("struct match") @@(t : T?) {
        t |> equal(0, struct_match(Foo(a = 13)))
        t |> equal(23, struct_match(Foo(a = 23)))
        t |> equal(0, struct_match(Bar(a = 13)))
        t |> equal(23, struct_match(Bar(a = 23, b = 13.0)))
        t |> equal(-1, struct_match(Bar(a = 23, b = 14.0)))
    }
    t |> run("guards match") @@(t : T?) {
        t |> equal("2 > 1", guards_match(AB(a = 1, b = 2)))
        t |> equal("1 <= 2", guards_match(AB(a = 2, b = 1)))
    }
    t |> run("variant match") @@(t : T?) {
        t |> equal(1, variant_match(IF(i = 1)))
        t |> equal(2, variant_match(IF(f = 1.0)))
    }
    t |> run("variant as match") @@(t : T?) {
        t |> equal("int", variant_as_match(IF(i = 1)))
        t |> equal("float", variant_as_match(IF(f = 1.0)))
    }
    t |> run("nested struct + variant as match") @@(t : T?) {
        t |> equal(1, nested_struct_match(Foo(a = 1, v = IF(i = 1))))
        t |> equal(2, nested_struct_match(Foo(v = IF(f = 1.))))
        t |> equal(0, nested_struct_match(Foo(a = 13, v = IF(i = 1))))
    }
    t |> run("static array match") @@(t : T?) {
        t |> equal(1, static_array_match(fixed_array<int>(1, 2, 3)))
        t |> equal(0, static_array_match(fixed_array<int>(0, 1, 2)))   // strts with 0
        t |> equal(0, static_array_match(fixed_array<int>(0, 0, 0)))   // strts with 0
        t |> equal(-1, static_array_match(fixed_array<int>(3, 3, 3)))
        t |> equal(2, static_array_match(fixed_array<int>(3, 3, 13))) // ends with 12
        t |> equal(3, static_array_match(fixed_array<int>(12, 3, 12))) // starts and ends with 12
    }
    t |> run("dynamic array match") @@(t : T?) {
        t |> equal(1, dynamic_array_match(array<int>(1, 2, 3)))          // summ is 6, 3 total
        t |> equal(0, dynamic_array_match(array<int>(0, 0, 0)))          // first 3 are 0
        t |> equal(0, dynamic_array_match(array<int>(0, 0, 0, 1, 2, 3))) // first 3 are 0
        t |> equal(-1, dynamic_array_match(array<int>(3, 3, 3)))
        t |> equal(-1, dynamic_array_match(array<int>(1, 2, 3, 4)))
        t |> equal(-1, dynamic_array_match(array<int>(2, 2)))
        t |> equal(2, dynamic_array_match(array<int>(1, 2)))     // ends with 1,2
        t |> equal(2, dynamic_array_match(array<int>(2, 1, 2)))  // ends with 1,2
        t |> equal(3, dynamic_array_match(array<int>(0, 1, 2, 3))) // starts with 0,1, ends with 2,3
        // starts with 0,1, ends with 2,3
        t |> equal(3, dynamic_array_match(array<int>(0, 1, 12, 11, 10, 2, 3)))
    }
    t |> run("match_expr") @@(t : T?) {
        t |> equal(true, ascending_array_match(fixed_array<int>(1, 2, 3)))
        t |> equal(true, ascending_array_match(fixed_array<int>(0, 1, 2)))
        t |> equal(false, ascending_array_match(fixed_array<int>(1, 1, 2)))
    }
    t |> run("match tuple") @@(t : T?) {
        t |> equal(1, tuple_match((1, 2.0, "3")))
        t |> equal(0, tuple_match((1, 2.0, "4")))
        t |> equal(2, tuple_match((13, 2.0, "4")))
        t |> equal(3, tuple_match((11, 2.0, "13")))
        t |> equal(4, tuple_match((2, 2.0, "2")))
    }
    t |> run("or match") @@(t : T?) {
        t |> equal(true, or_match(1))
        t |> equal(true, or_match(2))
        t |> equal(false, or_match(3))
    }
    t |> run("or match with variable") @@(t : T?) {
        t |> equal(1., or_match_v(Bar(a = 1, b = 1.)))
        t |> equal(1., or_match_v(Bar(a = 2, b = 1.)))
        t |> equal(0., or_match_v(Bar(a = 3, b = 1.)))
    }
    t |> run("or match keeps captures made before it") @@(t : T?) {
        t |> equal(7, or_match_prior_capture(AB(a = 7, b = 2)))
        t |> equal(-1, or_match_prior_capture(AB(a = 7, b = 3)))
        t |> equal(304, or_capture_both_branches(AB(a = 3, b = 4)))
        t |> equal(309, or_capture_both_branches(AB(a = 3, b = 9)))
        t |> equal(-1, or_capture_both_branches(AB(a = 3, b = 5)))
    }
    t |> run("struct ptr match") @@(t : T?) {
        t |> equal(-1, struct_ptr_match(null))
        t |> equal(0, struct_ptr_match(new Foo(a = 13)))
        t |> equal(23, struct_ptr_match(new Foo(a = 23)))
    }
    t |> run("match handle ptr") @@(t : T?) {
        ast_gc_guard() {
            t |> equal("null", match_handle_ptr(default<TypeDeclPtr>))
            var tp = new TypeDecl(baseType = Type.tPointer)
            t |> equal("pointer", match_handle_ptr(tp))
            var tnp = new TypeDecl(baseType = Type.tInt)
            t |> equal("anything", match_handle_ptr(tnp))
        }
    }
    t |> run("match ast expression ptr") @@(t : T?) {
        ast_gc_guard() {
            t |> equal("null", match_ast_expr_ptr(default<ExpressionPtr>))
            var ep <- new ExprConstInt(value = 13)
            t |> equal("int 13", match_ast_expr_ptr(ep))
            var fp <- new ExprConstFloat(value = 13.0)
            t |> equal("anything", match_ast_expr_ptr(fp))
        }
    }
    t |> run("match ast op2 shapes") @@(t : T?) {
        ast_gc_guard() {
            var a <- new ExprConstInt(value = 1)
            var b <- new ExprConstInt(value = 2)
            var mul <- new ExprOp2(op := "*", left = a, right = b)
            var c <- new ExprConstInt(value = 3)
            var mad <- new ExprOp2(op := "+", left = mul, right = c)
            t |> equal("mad ExprConstInt ExprConstInt ExprConstInt", match_ast_op2_shape(mad))
            var x <- new ExprConstInt(value = 4)
            var y <- new ExprConstInt(value = 5)
            var sub <- new ExprOp2(op := "-", left = x, right = y)
            t |> equal("addsub ExprConstInt", match_ast_op2_shape(sub))
            var dx <- new ExprConstInt(value = 6)
            var dy <- new ExprConstInt(value = 7)
            var dv <- new ExprOp2(op := "/", left = dx, right = dy)
            t |> equal("other", match_ast_op2_shape(dv))
            var sv <- new ExprVar(name := "v")
            var sw <- new ExprSwizzle(value = sv, mask := "xy")
            t |> equal("swizzle-xy ExprVar", match_ast_op2_shape(sw))
            var sv2 <- new ExprVar(name := "w")
            var sw2 <- new ExprSwizzle(value = sv2, mask := "zw")
            t |> equal("other", match_ast_op2_shape(sw2))
            // null guard: node arms must fail cleanly on null without an `if
            // (null)` arm
            t |> equal("other", match_ast_op2_shape(default<ExpressionPtr>))
        }
    }
    t |> run("match das_string scrutinee against string literals") @@(t : T?) {
        ast_gc_guard() {
            var l <- new ExprConstInt(value = 1)
            var r <- new ExprConstInt(value = 2)
            var plus <- new ExprOp2(op := "+", left = l, right = r)
            t |> equal("plus", match_op_string(plus.op))
            var l2 <- new ExprConstInt(value = 3)
            var r2 <- new ExprConstInt(value = 4)
            var dv <- new ExprOp2(op := "/", left = l2, right = r2)
            t |> equal("other", match_op_string(dv.op))
        }
    }
    t |> run("match as is") @@(t : T?) {
        t |> equal(0., matching_as_and_is(Cmd()))
        t |> equal(3., matching_as_and_is(CmdMove(x = 1., y = 2.)))
    }
    t |> run("match type is") @@(t : T?) {
        t |> equal(0., matching_type_as_is(Cmd()))
        t |> equal(3., matching_type_as_is(CmdMove(x = 1., y = 2.)))
    }
    t |> run("match copy") @@(t : T?) {
        t |> equal(0., matching_copy(Cmd()))
        t |> equal(6., matching_copy(CmdLocate(x = 1., y = 2., z = 3.)))
    }
    t |> run("match type copy") @@(t : T?) {
        t |> equal(0., matching_type_copy(Cmd()))
        t |> equal(6., matching_type_copy(CmdLocate(x = 1., y = 2., z = 3.)))
    }
}

// ===========================
// Edge case types
// ===========================

variant IntOrStr {
    i : int
    s : string
}

struct Nested {
    inner : EdgePoint
    tag : string
}

struct EdgePoint {
    x : int
    y : int
}

struct Empty {}

variant ABC {
    a : int
    b : float
    c : string
}

// ===========================
// Nested struct matching
// ===========================

[sideeffects]
def nested_match(n : Nested) {
    return match (n) {
        Nested(inner = EdgePoint(x = 0, y = 0), tag = "origin") => "origin labeled"
        Nested(inner = EdgePoint(x = x, y = y)) => "({x},{y})"
    }
}

// ===========================
// Match on empty struct (no fields)
// ===========================

[sideeffects, unused_argument(e)]
def empty_struct_match(e : Empty) {
    return match (e) {
        Empty() => "empty"
    }
}

// ===========================
// Match string values
// ===========================

[sideeffects]
def string_match(s : string) {
    return match (s) {
        "hello" => 1
        "world" => 2
        _ => 0
    }
}

// ===========================
// Boolean match — tests that if(true)/if(false) inside match block
// are treated as pattern match arms, not constant-folded.
// ===========================

[sideeffects]
def bool_match(b : bool) {
    return match (b) {
        true => "yes"
        false => "no"
    }
}

// ===========================
// Multiple variant alternatives (3-way)
// ===========================

[sideeffects]
def three_variant_match(v : ABC) {
    return match (v) {
        a as a => "a={a}"
        b as b => "b={b}"
        c as c => "c={c}"
    }
}

// ===========================
// Variant match with guard on captured value
// ===========================

[sideeffects]
def variant_guard_match(v : IntOrStr) {
    return match (v) {
        i as i && i > 0 => "positive int"
        i as i => "non-positive int"
        s as s => "string: {s}"
    }
}

// ===========================
// Tuple with ... (spread)
// ===========================

[sideeffects]
def tuple4_match(t : tuple<int; int; int; int>) {
    return match (t) {
        (1, ...) => "starts with 1"
        (..., 9) => "ends with 9"
        (0, ..., 0) => "bookends 0"
        _ => "other"
    }
}

// ===========================
// match_expr with captured variable
// ===========================

[sideeffects]
def match_expr_captured(p : EdgePoint) {
    return match (p) {
        EdgePoint(x = x, y = match_expr(x)) => "diagonal"  // y == x
        EdgePoint(x = x, y = match_expr(x * 2)) => "double"  // y == x*2
        _ => "other"
    }
}

// ===========================
// Dynamic array with spread and literal values
// ===========================

[sideeffects]
def dynamic_array_tail_literal(A : array<int>) {
    return match (A) {
        array<int>(..., 42) => "ends with 42"
        array<int>(1, ...) => "starts with 1"
        _ => "other"
    }
}

// ===========================
// OR pattern with int values
// ===========================

[sideeffects]
def or_pattern_simple(i : int) {
    return match (i) {
        1 | 2 | 3 => "low"
        10 | 20 => "round"
        _ => "other"
    }
}

// ===========================
// Struct match with all fields wildcarded (no field constraints)
// ===========================

[sideeffects]
def struct_all_wildcard(p : EdgePoint) {
    match (p) {
        EdgePoint(x = 0, y = 0) => $ {
            return "origin"
        }
        EdgePoint() => $ {
            return "any point"
        }
    }
}

// ===========================
// Struct match: field ordering shouldn't matter
// ===========================

[sideeffects]
def struct_field_order(p : EdgePoint) {
    return match (p) {
        EdgePoint(y = 0, x = x) => x
        _ => -1
    }
}

// ===========================
// Variant match using Variant(field = ...) syntax
// ===========================

[sideeffects]
def variant_field_syntax(v : IntOrStr) {
    return match (v) {
        IntOrStr(i = 42) => "the answer"
        IntOrStr(i = i) => "int {i}"
        IntOrStr(s = s) => "str {s}"
    }
}

// ===========================
// Static array match with exact values
// ===========================

[sideeffects]
def static_array_exact(A : int[3]) {
    return match (A) {
        fixed_array<int>(1, 2, 3) => "123"
        fixed_array<int>(0, ...) => "starts 0"
        _ => "other"
    }
}

// ===========================
// Nested variant inside struct
// ===========================

struct HasVariant {
    name : string
    value : IntOrStr
}

[sideeffects]
def nested_variant_in_struct(h : HasVariant) {
    return match (h) {
        HasVariant(name = "x", value = i as i) => "x int {i}"
        HasVariant(value = s as s) => "string {s}"
        _ => "other"
    }
}

// ===========================
// static_match — fallthrough on type mismatch
// ===========================

[sideeffects]
def static_match_int_or_float(what) {
    return static_match (what) {
        match_type(type<int>, v) => "int:{v}"
        match_type(type<float>, v) => "float:{v}"
        _ => "other"
    }
}

// ===========================
// match on tuple with capture
// ===========================

[sideeffects]
def tuple_capture(t : tuple<int; string>) {
    match (t) {
        (n, "hello") => $ {
            return n * 10
        }
        (n, _) => $ {
            return n
        }
    }
}

// ===========================
// deep nesting: struct -> struct -> value
// ===========================

struct Outer {
    mid : Nested
    id : int
}

[sideeffects]
def deep_nested_match(o : Outer) {
    return match (o) {
        Outer(mid = Nested(inner = EdgePoint(x = 0, y = 0), tag = "root"), id = 1) => "root-1"
        Outer(mid = Nested(inner = EdgePoint(x = x))) => "x={x}"
    }
}

// ===========================
// match_expr in struct: captured var used in expression
// (tests the apply_template fix for root ExprVar replacement)
// ===========================

[sideeffects]
def match_expr_struct_capture(p : EdgePoint) {
    return match (p) {
        EdgePoint(x = x, y = match_expr(x)) => "diagonal x={x}"
        EdgePoint(x = x, y = match_expr(x * 2)) => "double x={x}"
        _ => "other"
    }
}

// ===========================
// match_expr in tuple: captured var in expression
// ===========================

[sideeffects]
def match_expr_tuple_capture(t : tuple<int; int>) {
    return match (t) {
        (a, match_expr(a)) => "equal={a}"
        _ => "different"
    }
}

// ===========================
// Null pointer matching
// ===========================

[sideeffects]
def null_ptr_match(p : EdgePoint?) {
    return match (p) {
        null => "null"
        EdgePoint(x = 0, y = 0) => "origin"
        EdgePoint(x = x) => "x={x}"
    }
}

// ===========================
// Guard on nested struct pattern
// ===========================

[sideeffects]
def guard_nested(n : Nested) {
    return match (n) {
        Nested(inner = EdgePoint(x = x, y = y)) && x == y => "diagonal {x}"
        Nested(inner = EdgePoint(x = x)) && x > 10 => "big x={x}"
        _ => "other"
    }
}

// ===========================
// OR pattern with variant as-syntax
// ===========================

// OR pattern on variant — no capture, just type check
// NOTE: match_or requires matching access expressions for captured
// vars, so using wildcards instead of captures when patterns access
// different fields
[sideeffects]
def or_variant_match(v : ABC) {
    return match (v) {
        (_ as a) | (_ as c) => "a or c"
        _ => "b"
    }
}

// ===========================
// enum match via match macro
// ===========================

enum EdgeColor {
    Red
    Green
    Blue
}

[sideeffects]
def edge_enum_match(c : EdgeColor) {
    return match (c) {
        EdgeColor.Red => "red"
        EdgeColor.Blue => "blue"
        _ => "other"
    }
}

// ===========================
// int literal match via match macro
// ===========================

[sideeffects]
def int_literal_match(n : int) {
    return match (n) {
        0 => "zero"
        1 => "one"
        42 => "answer"
        _ => "other"
    }
}

// ===========================
// float literal match
// ===========================

[sideeffects]
def float_match(f : float) {
    return match (f) {
        0.0 => "zero"
        1.0 => "one"
        _ => "other"
    }
}

// ===========================
// TESTS
// ===========================

[test(description = "Edge-case match shapes (nested, empty, spread, tail literal, or, guards) select the right arm")]
def test_match_edge_cases(t : T?) { // nolint:STYLE038 - flat list of independent match arms

    t |> run("nested struct match") @(t : T?) {
        t |> equal("origin labeled",
                   nested_match(Nested(inner = EdgePoint(x = 0, y = 0), tag = "origin")))
        t |> equal("(3,4)", nested_match(Nested(inner = EdgePoint(x = 3, y = 4), tag = "test")))
    }

    t |> run("empty struct match") @(t : T?) {
        t |> equal("empty", empty_struct_match(Empty()))
    }

    t |> run("string match") @(t : T?) {
        t |> equal(1, string_match("hello"))
        t |> equal(2, string_match("world"))
        t |> equal(0, string_match("xyz"))
        t |> equal(0, string_match(""))
    }

    t |> run("bool match") @(t : T?) {
        t |> equal("yes", bool_match(true))
        t |> equal("no", bool_match(false))
    }

    t |> run("three-way variant match") @(t : T?) {
        t |> equal("a=42", three_variant_match(ABC(a = 42)))
        t |> equal("b=3.14", three_variant_match(ABC(b = 3.14)))
        t |> equal("c=hi", three_variant_match(ABC(c = "hi")))
    }

    t |> run("variant match with guard") @(t : T?) {
        t |> equal("positive int", variant_guard_match(IntOrStr(i = 5)))
        t |> equal("non-positive int", variant_guard_match(IntOrStr(i = -3)))
        t |> equal("non-positive int", variant_guard_match(IntOrStr(i = 0)))
        t |> equal("string: hello", variant_guard_match(IntOrStr(s = "hello")))
    }

    t |> run("tuple with spread") @(t : T?) {
        t |> equal("starts with 1", tuple4_match((1, 2, 3, 4)))
        t |> equal("ends with 9", tuple4_match((2, 3, 4, 9)))
        t |> equal("bookends 0", tuple4_match((0, 3, 4, 0)))
        t |> equal("other", tuple4_match((2, 3, 4, 5)))
    }

    t |> run("match_expr with captured variable") @(t : T?) {
        t |> equal("diagonal", match_expr_captured(EdgePoint(x = 5, y = 5)))
        t |> equal("double", match_expr_captured(EdgePoint(x = 3, y = 6)))
        t |> equal("other", match_expr_captured(EdgePoint(x = 1, y = 7)))
    }

    t |> run("dynamic array tail literal") @(t : T?) {
        t |> equal("ends with 42", dynamic_array_tail_literal(array<int>(1, 2, 42)))
        t |> equal("starts with 1", dynamic_array_tail_literal(array<int>(1, 2, 3)))
        t |> equal("other", dynamic_array_tail_literal(array<int>(2, 3, 4)))
        t |> equal("other", dynamic_array_tail_literal(array<int>()))
    }

    t |> run("OR pattern simple") @(t : T?) {
        t |> equal("low", or_pattern_simple(1))
        t |> equal("low", or_pattern_simple(2))
        t |> equal("low", or_pattern_simple(3))
        t |> equal("round", or_pattern_simple(10))
        t |> equal("round", or_pattern_simple(20))
        t |> equal("other", or_pattern_simple(5))
    }

    t |> run("struct all wildcard") @(t : T?) {
        t |> equal("origin", struct_all_wildcard(EdgePoint(x = 0, y = 0)))
        t |> equal("any point", struct_all_wildcard(EdgePoint(x = 1, y = 2)))
    }

    t |> run("struct field order") @(t : T?) {
        t |> equal(5, struct_field_order(EdgePoint(x = 5, y = 0)))
        t |> equal(-1, struct_field_order(EdgePoint(x = 5, y = 1)))
    }

    t |> run("variant field syntax") @(t : T?) {
        t |> equal("the answer", variant_field_syntax(IntOrStr(i = 42)))
        t |> equal("int 7", variant_field_syntax(IntOrStr(i = 7)))
        t |> equal("str hello", variant_field_syntax(IntOrStr(s = "hello")))
    }

    t |> run("static array exact") @(t : T?) {
        t |> equal("123", static_array_exact(fixed_array<int>(1, 2, 3)))
        t |> equal("starts 0", static_array_exact(fixed_array<int>(0, 5, 5)))
        t |> equal("other", static_array_exact(fixed_array<int>(5, 5, 5)))
    }

    t |> run("nested variant in struct") @(t : T?) {
        t |> equal("x int 10",
                   nested_variant_in_struct(HasVariant(name = "x", value = IntOrStr(i = 10))))
        t |> equal("string world",
                   nested_variant_in_struct(HasVariant(name = "y", value = IntOrStr(s = "world"))))
        t |> equal("other",
                   nested_variant_in_struct(HasVariant(name = "z", value = IntOrStr(i = 1))))
    }

    t |> run("static_match int or float") @(t : T?) {
        t |> equal("int:42", static_match_int_or_float(42))
        t |> equal("float:3.14", static_match_int_or_float(3.14))
        t |> equal("other", static_match_int_or_float("hello"))
    }

    t |> run("tuple capture") @(t : T?) {
        t |> equal(50, tuple_capture((5, "hello")))
        t |> equal(3, tuple_capture((3, "bye")))
    }

    t |> run("deep nested match") @(t : T?) {
        t |> equal("root-1",
                   deep_nested_match(
                                     Outer(
                                           mid = Nested(inner = EdgePoint(x = 0, y = 0),
                                                        tag = "root"), id = 1)))
        t |> equal("x=7",
                   deep_nested_match(
                                     Outer(
                                           mid = Nested(inner = EdgePoint(x = 7, y = 3),
                                                        tag = "foo"), id = 2)))
    }

    t |> run("match_expr struct capture") @(t : T?) {
        t |> equal("diagonal x=5", match_expr_struct_capture(EdgePoint(x = 5, y = 5)))
        t |> equal("double x=3", match_expr_struct_capture(EdgePoint(x = 3, y = 6)))
        t |> equal("other", match_expr_struct_capture(EdgePoint(x = 1, y = 7)))
    }

    t |> run("match_expr tuple capture") @(t : T?) {
        t |> equal("equal=5", match_expr_tuple_capture((5, 5)))
        t |> equal("different", match_expr_tuple_capture((1, 2)))
    }

    t |> run("null pointer match") @(t : T?) {
        t |> equal("null", null_ptr_match(null))
        unsafe {
            var inscope p = new EdgePoint(x = 0, y = 0)
            t |> equal("origin", null_ptr_match(p))
            var inscope q = new EdgePoint(x = 7, y = 3)
            t |> equal("x=7", null_ptr_match(q))
        }
    }

    t |> run("guard on nested struct") @(t : T?) {
        t |> equal("diagonal 3", guard_nested(Nested(inner = EdgePoint(x = 3, y = 3), tag = "")))
        t |> equal("big x=20", guard_nested(Nested(inner = EdgePoint(x = 20, y = 1), tag = "")))
        t |> equal("other", guard_nested(Nested(inner = EdgePoint(x = 5, y = 6), tag = "")))
    }

    t |> run("OR variant match") @(t : T?) {
        t |> equal("a or c", or_variant_match(ABC(a = 1)))
        t |> equal("a or c", or_variant_match(ABC(c = "hi")))
        t |> equal("b", or_variant_match(ABC(b = 1.0)))
    }

    t |> run("enum match") @(t : T?) {
        t |> equal("red", edge_enum_match(EdgeColor.Red))
        t |> equal("blue", edge_enum_match(EdgeColor.Blue))
        t |> equal("other", edge_enum_match(EdgeColor.Green))
    }

    t |> run("int literal match") @(t : T?) {
        t |> equal("zero", int_literal_match(0))
        t |> equal("one", int_literal_match(1))
        t |> equal("answer", int_literal_match(42))
        t |> equal("other", int_literal_match(99))
    }

    t |> run("float match") @(t : T?) {
        t |> equal("zero", float_match(0.0))
        t |> equal("one", float_match(1.0))
        t |> equal("other", float_match(3.14))
    }
}

struct SitesPoint {
    x : int
    y : int
}

def private point_for(x : int) : SitesPoint? {
    var log = ""
    match (x) {
        0 => $ { log = "zero" }
        _ => $ { log = "other" }
    }
    var p : SitesPoint? = match (x) {
        0 => null
        _ => new SitesPoint(x = x, y = length(log))
    }
    return match (x) {
        0 => p
        n => match (n) {
            1 => new SitesPoint(x = 1, y = 1)
            _ => p
        }
    }
}

[test(description = "Each match of a function keeps its own site, so typed lets, nested matches and statement matches do not mix")]
def test_every_match_of_a_function_keeps_its_place(t : T?) {
    t |> run("a typed let and a typed return type their null arms") @(t : T?) {
        t |> success(point_for(0) == null)
    }
    t |> run("a match nested in an arm keeps its own place") @(t : T?) {
        t |> equal(point_for(1).y, 1)
    }
    t |> run("the statement match ran before the value matches") @(t : T?) {
        let p = point_for(7)
        t |> success(p != null)
        t |> equal(p.x, 7)
        t |> equal(p.y, 5)
    }
}

[sideeffects]
def op2_left_name(E : ExpressionPtr) : string {
    return match (E) {
        ExprOp2(op = "+" | "-") & o2 =>
            o2.left is ExprVar ? "{o2.op} {(o2.left as ExprVar).name}" : "{o2.op}"
        _ => "other"
    }
}

[sideeffects]
def var_name_len(E : ExpressionPtr) : int {
    match (E) {
        ExprVar() & v => $ {
            return length(v.name)
        }
    }
    return -1
}

[sideeffects]
def const_value(E : ExpressionPtr) : int {
    return match (E) {
        k & ExprConstInt() => k.value
        _ => -1
    }
}

[sideeffects]
def class_only(E : ExpressionPtr) : string {
    return match (E) {
        ExprVar() => "var"
        ExprConstInt() => "int"
        _ => "other"
    }
}

[test(description = "The ampersand form binds an AST node at the class the pattern tests")]
def test_match_ast_node_bind(t : T?) {
    t |> run("& binds the node at the class the pattern tests") @@(t : T?) {
        ast_gc_guard() {
            var a <- new ExprVar(name := "a")
            var b <- new ExprVar(name := "b")
            var sum <- new ExprOp2(op := "+", left = a, right = b)
            t |> equal("+ a", op2_left_name(sum))
            var c <- new ExprVar(name := "c")
            t |> equal("other", op2_left_name(c))
        }
    }
    t |> run("& binds the peeled node in a statement arm") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "abc")
            var wrapped <- new ExprRef2Value(subexpr = v)
            t |> equal(3, var_name_len(wrapped))
            var k <- new ExprConstInt(value = 1)
            t |> equal(-1, var_name_len(k))
        }
    }
    t |> run("a name on the left of & binds the node at the class too") @@(t : T?) {
        ast_gc_guard() {
            var k <- new ExprConstInt(value = 42)
            t |> equal(42, const_value(k))
            var v <- new ExprVar(name := "x")
            t |> equal(-1, const_value(v))
        }
    }
    t |> run("a class pattern with no fields tests the class only") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "x")
            var k <- new ExprConstInt(value = 7)
            var f <- new ExprConstFloat(value = 1.0)
            t |> equal("var", class_only(v))
            t |> equal("int", class_only(k))
            t |> equal("other", class_only(f))
            t |> equal("other", class_only(null))
        }
    }
}

// ExprRef2Value transparency: the typer wraps reads of references in
// ExprRef2Value (no surface syntax), so AST class patterns peel it on
// the source side at every nesting level — mirroring qmatch. An
// explicit ExprRef2Value(...) pattern still matches the wrapper
// itself, and $v captures of Expression-typed values bind the peeled
// node.

[sideeffects]
def match_field_of_var(E : ExpressionPtr) : string {
    return match (E) {
        ExprField(name = fname, value = ExprVar(name = vname)) => "{vname}.{fname}"
        _ => "other"
    }
}

[sideeffects]
def match_wrapper_explicitly(E : ExpressionPtr) : string {
    return match (E) {
        ExprRef2Value(subexpr = ExprVar(name = vname)) => "wrapped {vname}"
        ExprVar(name = vname) => "bare {vname}"
        _ => "other"
    }
}

[sideeffects]
def capture_field_value(E : ExpressionPtr) : string {
    return match (E) {
        ExprField(value = base) => "{base.__rtti}"
        _ => "other"
    }
}

[test(description = "A class pattern peels a ref-to-value wrapper, while an explicit wrapper pattern matches the wrapper itself")]
def test_r2v_peel(t : T?) {
    t |> run("top-level wrapper peels for class patterns") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "a")
            var fld <- new ExprField(name := "foo", value = v)
            var wrapped <- new ExprRef2Value(subexpr = fld)
            t |> equal("a.foo", match_field_of_var(wrapped))
        }
    }
    t |> run("nested wrapper peels at the field boundary") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "b")
            var wrappedVar <- new ExprRef2Value(subexpr = v)
            var fld <- new ExprField(name := "bar", value = wrappedVar)
            t |> equal("b.bar", match_field_of_var(fld))
        }
    }
    t |> run("unwrapped source still matches (no-op peel)") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "c")
            var fld <- new ExprField(name := "baz", value = v)
            t |> equal("c.baz", match_field_of_var(fld))
        }
    }
    t |> run("explicit ExprRef2Value pattern matches the wrapper itself") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "d")
            var wrapped <- new ExprRef2Value(subexpr = v)
            t |> equal("wrapped d", match_wrapper_explicitly(wrapped))
            var bare <- new ExprVar(name := "e")
            t |> equal("bare e", match_wrapper_explicitly(bare))
        }
    }
    t |> run("$v capture of an Expression field binds the peeled node") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "f")
            var wrappedVar <- new ExprRef2Value(subexpr = v)
            var fld <- new ExprField(name := "qux", value = wrappedVar)
            t |> equal("ExprVar", capture_field_value(fld))
        }
    }
    t |> run("double wrapper peels fully") @@(t : T?) {
        ast_gc_guard() {
            var v <- new ExprVar(name := "g")
            var w1 <- new ExprRef2Value(subexpr = v)
            var w2 <- new ExprRef2Value(subexpr = w1)
            var fld <- new ExprField(name := "deep", value = w2)
            t |> equal("g.deep", match_field_of_var(fld))
        }
    }
}
