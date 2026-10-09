options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/match
require daslib/ast_boost
require daslib/defer

// ===========================
// Edge case types
// ===========================

variant IntOrStr {
    i : int
    s : string
}

struct Nested {
    inner : Point
    tag : string
}

struct Point {
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
        Nested(inner = Point(x = 0, y = 0), tag = "origin") => "origin labeled"
        Nested(inner = Point(x = x, y = y)) => "({x},{y})"
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
def match_expr_captured(p : Point) {
    return match (p) {
        Point(x = x, y = match_expr(x)) => "diagonal"  // y == x
        Point(x = x, y = match_expr(x * 2)) => "double"  // y == x*2
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
def struct_all_wildcard(p : Point) {
    match (p) {
        Point(x = 0, y = 0) => $ {
            return "origin"
        }
        Point() => $ {
            return "any point"
        }
    }
}

// ===========================
// Struct match: field ordering shouldn't matter
// ===========================

[sideeffects]
def struct_field_order(p : Point) {
    return match (p) {
        Point(y = 0, x = x) => x
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
        Outer(mid = Nested(inner = Point(x = 0, y = 0), tag = "root"), id = 1) => "root-1"
        Outer(mid = Nested(inner = Point(x = x))) => "x={x}"
    }
}

// ===========================
// match_expr in struct: captured var used in expression
// (tests the apply_template fix for root ExprVar replacement)
// ===========================

[sideeffects]
def match_expr_struct_capture(p : Point) {
    return match (p) {
        Point(x = x, y = match_expr(x)) => "diagonal x={x}"
        Point(x = x, y = match_expr(x * 2)) => "double x={x}"
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
def null_ptr_match(p : Point?) {
    return match (p) {
        null => "null"
        Point(x = 0, y = 0) => "origin"
        Point(x = x) => "x={x}"
    }
}

// ===========================
// Guard on nested struct pattern
// ===========================

[sideeffects]
def guard_nested(n : Nested) {
    return match (n) {
        Nested(inner = Point(x = x, y = y)) && x == y => "diagonal {x}"
        Nested(inner = Point(x = x)) && x > 10 => "big x={x}"
        _ => "other"
    }
}

// ===========================
// OR pattern with variant as-syntax
// ===========================

// OR pattern on variant — no capture, just type check
// NOTE: match_or requires matching access expressions for captured vars,
// so using wildcards instead of captures when patterns access different fields
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

enum Color {
    Red
    Green
    Blue
}

[sideeffects]
def enum_match(c : Color) {
    return match (c) {
        Color.Red => "red"
        Color.Blue => "blue"
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

[test]
def test_match_edge_cases(t : T?) { // nolint:STYLE038 - flat list of independent match arms

    t |> run("nested struct match") @(t : T?) {
        t |> equal("origin labeled", nested_match(Nested(inner = Point(x = 0, y = 0), tag = "origin")))
        t |> equal("(3,4)", nested_match(Nested(inner = Point(x = 3, y = 4), tag = "test")))
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
        t |> equal("diagonal", match_expr_captured(Point(x = 5, y = 5)))
        t |> equal("double", match_expr_captured(Point(x = 3, y = 6)))
        t |> equal("other", match_expr_captured(Point(x = 1, y = 7)))
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
        t |> equal("origin", struct_all_wildcard(Point(x = 0, y = 0)))
        t |> equal("any point", struct_all_wildcard(Point(x = 1, y = 2)))
    }

    t |> run("struct field order") @(t : T?) {
        t |> equal(5, struct_field_order(Point(x = 5, y = 0)))
        t |> equal(-1, struct_field_order(Point(x = 5, y = 1)))
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
        t |> equal("x int 10", nested_variant_in_struct(HasVariant(name = "x", value = IntOrStr(i = 10))))
        t |> equal("string world", nested_variant_in_struct(HasVariant(name = "y", value = IntOrStr(s = "world"))))
        t |> equal("other", nested_variant_in_struct(HasVariant(name = "z", value = IntOrStr(i = 1))))
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
        t |> equal("root-1", deep_nested_match(Outer(mid = Nested(inner = Point(x = 0, y = 0), tag = "root"), id = 1)))
        t |> equal("x=7", deep_nested_match(Outer(mid = Nested(inner = Point(x = 7, y = 3), tag = "foo"), id = 2)))
    }

    t |> run("match_expr struct capture") @(t : T?) {
        t |> equal("diagonal x=5", match_expr_struct_capture(Point(x = 5, y = 5)))
        t |> equal("double x=3", match_expr_struct_capture(Point(x = 3, y = 6)))
        t |> equal("other", match_expr_struct_capture(Point(x = 1, y = 7)))
    }

    t |> run("match_expr tuple capture") @(t : T?) {
        t |> equal("equal=5", match_expr_tuple_capture((5, 5)))
        t |> equal("different", match_expr_tuple_capture((1, 2)))
    }

    t |> run("null pointer match") @(t : T?) {
        t |> equal("null", null_ptr_match(null))
        unsafe {
            var inscope p = new Point(x = 0, y = 0)
            t |> equal("origin", null_ptr_match(p))
            var inscope q = new Point(x = 7, y = 3)
            t |> equal("x=7", null_ptr_match(q))
        }
    }

    t |> run("guard on nested struct") @(t : T?) {
        t |> equal("diagonal 3", guard_nested(Nested(inner = Point(x = 3, y = 3), tag = "")))
        t |> equal("big x=20", guard_nested(Nested(inner = Point(x = 20, y = 1), tag = "")))
        t |> equal("other", guard_nested(Nested(inner = Point(x = 5, y = 6), tag = "")))
    }

    t |> run("OR variant match") @(t : T?) {
        t |> equal("a or c", or_variant_match(ABC(a = 1)))
        t |> equal("a or c", or_variant_match(ABC(c = "hi")))
        t |> equal("b", or_variant_match(ABC(b = 1.0)))
    }

    t |> run("enum match") @(t : T?) {
        t |> equal("red", enum_match(Color.Red))
        t |> equal("blue", enum_match(Color.Blue))
        t |> equal("other", enum_match(Color.Green))
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
