options gen2
require dastest/testing_boost public
require daslib/match
require daslib/ast_boost
require daslib/defer

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
        ExprOp2(op = "+", left = ExprOp2(op = "*", left = a, right = b), right = c) => "mad {a.__rtti} {b.__rtti} {c.__rtti}"
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

[test]
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
        t |> equal(3, dynamic_array_match(array<int>(0, 1, 12, 11, 10, 2, 3))) // starts with 0,1, ends with 2,3
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
            // null guard: node arms must fail cleanly on null without an `if (null)` arm
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


