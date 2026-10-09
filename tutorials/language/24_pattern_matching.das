// Tutorial 24: Pattern Matching
//
// This tutorial covers:
//   - match macro: `pattern => body` arms
//   - Matching integers, enums, structs
//   - Variable binding: a bare name in a pattern binds
//   - Wildcards (_) and guards (&&)
//   - OR patterns (|)
//   - Tuple and array matching
//   - Variant matching
//   - Null/pointer matching
//   - match_expr for computed patterns
//   - Bool matching
//   - Matching several values at once: match (x, y)
//   - Both patterns (&): bind the whole value and destructure it
//   - Range patterns: (lo..hi), half-open
//   - match as a value: let s = match (x) { ... }
//   - static_match (generic-safe)
//
// Run: daslang.exe tutorials/language/24_pattern_matching.das

options gen2

require daslib/match

// === Enums and structs for examples ===

enum Color {
    red
    green
    blue
    black
}

struct Point {
    x : int
    y : int
}

struct Shape {
    kind : string
    size : int
}

variant Value {
    i : int
    f : float
    s : string
}

// === Basic integer matching ===
// Each arm is `pattern => body`. A body of several statements is a `$ { }` block;
// `return` inside it returns from the enclosing function.

def describe_number(n : int) : string {
    match (n) {
        0 => $ { return "zero" }
        1 => $ { return "one" }
        2 => $ { return "two" }
        _ => $ {                // wildcard — matches anything
            return "other"
        }
    }
    return "unreachable"
}

// === match as a value ===
// In value position each arm body is an expression, and the arms must cover
// every value - here the `_` arm - so every path produces a value. A match over
// an enum, bool or variant must name every value or end with `_`, and an arm an
// earlier one already covers is a compile error.

def color_code(c : Color) : int {
    return match (c) {
        Color.red => 1          // gen2 dot syntax for enum values
        Color.green => 2
        Color.blue => 3
        _ => 0
    }
}

// === Struct matching + variable binding ===
// A bare name in a pattern binds the matched value; it never compares
// against an existing variable (use match_expr(var) for that).

def describe_point(p : Point) : string {
    return match (p) {
        Point(x = 0, y = 0) => "origin"             // exact values
        Point(x = 0, y = y) => "on y-axis at {y}"   // bind y, x must be 0
        Point(x = x, y = 0) => "on x-axis at {x}"   // bind x, y must be 0
        Point(x = x, y = y) => "at ({x}, {y})"      // bind both
    }
}

// === Guards ===

def classify_shape(s : Shape) : string {
    return match (s) {
        Shape(kind = "circle", size = sz) && sz > 100 => "large circle ({sz})"
        Shape(kind = "circle", size = sz) => "small circle ({sz})"
        Shape(kind = k, size = sz) => "{k} of size {sz}"
    }
}

// === OR patterns ===

def is_primary(c : Color) : bool {
    return match (c) {
        Color.red | Color.green | Color.blue => true
        _ => false
    }
}

// === Tuple matching ===

def classify_pair(t : tuple<int; string>) : string {
    return match (t) {
        (0, "zero") => "zero-zero"          // exact match
        (1, _) => "starts with one"         // _ matches any one element
        (n, "hello") => "hello #{n}"        // bind first, match second
        _ => "other"
    }
}

// === Matching several values ===
// match (a, b) takes one tuple-shaped pattern per arm, one element per value.

def quadrant(x, y : int) : string {
    return match (x, y) {
        (0, 0) => "origin"
        (0, _) | (_, 0) => "on an axis"
        (a, b) && a > 0 && b > 0 => "first"
        (a, _) && a < 0 => "left half"
        _ => "fourth"
    }
}

// === Both patterns with & ===
// A bare name on one side of & binds the whole value; the other side destructures it.

def describe_on_axis(p : Point) : string {
    return match (p) {
        whole & Point(x = 0) => "on the y-axis: ({whole.x}, {whole.y})"
        _ => "off the y-axis"
    }
}

// === Range patterns ===
// (lo..hi) matches lo <= n && n < hi - half-open, like a for range.

def size_class(n : int) : string {
    return match (n) {
        (0..10) => "digit"
        (10..100) | (1000..2000) => "some"
        _ => "other"
    }
}

// === Variant matching ===
// A variant pattern is spelled like the variant constructor.

def describe_value(v : Value) : string {
    return match (v) {
        Value(i = i) => "integer: {i}"
        Value(f = f) => "float: {f}"
        Value(s = s) => "string: {s}"
    }
}

// === Null / pointer matching ===

def describe_point_ptr(p : Point?) : string {
    return match (p) {
        null => "null"                          // null check
        Point(x = 0, y = 0) => "origin"         // auto-dereferences
        Point(x = x, y = y) => "({x}, {y})"
    }
}


// === static_match — safe in generics ===
// Arms with type mismatches are silently dropped,
// so this function works regardless of argument type.

def static_type_name(what) : string {
    return static_match (what) {
        match_type(type<int>, v) => "int({v})"
        match_type(type<float>, v) => "float({v})"
        match_type(type<string>, v) => "string({v})"
        _ => "unknown"
    }
}

// === Array matching ===
// Static arrays match element-by-element.
// Dynamic arrays match head elements; tail is ignored.

def classify_static_arr(a : int[3]) : string {
    return match (a) {
        fixed_array<int>(0, 0, 0) => "zeros"
        fixed_array<int>(1, b, c) => "1,{b},{c}"    // first is 1, bind rest
        _ => "other"
    }
}

def classify_dynamic_arr(a : array<int>) : string {
    return match (a) {
        array<int>(0, 0, ...) => "starts with 0,0"
        array<int>(x, y, ...) => "starts with {x},{y}"  // bind first two (any length)
        _ => "other"
    }
}

// === match_expr — computed patterns ===
// match_expr(expression) evaluates at runtime instead of matching literally.
// Useful when the expected value depends on captured variables.

def is_consecutive_triple(t : tuple<int; int; int>) : bool {
    return match (t) {
        (a, match_expr(a + 1), match_expr(a + 2)) => true
        _ => false
    }
}

// === Bool matching ===

def describe_bool(b : bool) : string {
    return match (b) {
        true => "yes"
        _ => "no"
    }
}

// === Main ===

[export]
def main {   // nolint:STYLE038 - tutorial walkthrough, the sequence IS the lesson

    // Integer matching
    print("integer matching:\n")
    print("  0 -> {describe_number(0)}\n")
    print("  1 -> {describe_number(1)}\n")
    print("  42 -> {describe_number(42)}\n")

    // Enum matching
    print("enum matching:\n")
    print("  red -> {color_code(Color.red)}\n")
    print("  green -> {color_code(Color.green)}\n")
    print("  black -> {color_code(Color.black)}\n")

    // Struct matching
    print("struct matching:\n")
    print("  (0,0) -> {describe_point(Point(x=0, y=0))}\n")
    print("  (0,5) -> {describe_point(Point(x=0, y=5))}\n")
    print("  (3,0) -> {describe_point(Point(x=3, y=0))}\n")
    print("  (2,7) -> {describe_point(Point(x=2, y=7))}\n")

    // Guards
    print("guards:\n")
    let s1 = classify_shape(Shape(kind = "circle", size = 200))
    print("  circle 200 -> {s1}\n")
    let s2 = classify_shape(Shape(kind = "circle", size = 50))
    print("  circle 50 -> {s2}\n")
    let s3 = classify_shape(Shape(kind = "square", size = 30))
    print("  square 30 -> {s3}\n")

    // OR patterns
    print("OR patterns:\n")
    print("  red primary? {is_primary(Color.red)}\n")
    print("  black primary? {is_primary(Color.black)}\n")

    // Tuple matching
    print("tuple matching:\n")
    var t0 = (0, "zero")
    print("  (0,zero) -> {classify_pair(t0)}\n")
    t0 = (1, "xyz")
    print("  (1,xyz) -> {classify_pair(t0)}\n")
    t0 = (5, "hello")
    print("  (5,hello) -> {classify_pair(t0)}\n")
    t0 = (9, "bye")
    print("  (9,bye) -> {classify_pair(t0)}\n")

    // Several values
    print("several values:\n")
    print("  (0,0) -> {quadrant(0, 0)}\n")
    print("  (0,4) -> {quadrant(0, 4)}\n")
    print("  (2,3) -> {quadrant(2, 3)}\n")
    print("  (-1,3) -> {quadrant(-1, 3)}\n")
    print("  (2,-3) -> {quadrant(2, -3)}\n")

    // Both patterns
    print("both patterns:\n")
    print("  (0,4) -> {describe_on_axis(Point(x=0, y=4))}\n")
    print("  (1,4) -> {describe_on_axis(Point(x=1, y=4))}\n")

    // Range patterns
    print("range patterns:\n")
    print("  9 -> {size_class(9)}\n")
    print("  10 -> {size_class(10)}\n")
    print("  1500 -> {size_class(1500)}\n")
    print("  500 -> {size_class(500)}\n")

    // Variant matching
    print("variant matching:\n")
    var v : Value = Value(i = 42)
    print("  int -> {describe_value(v)}\n")
    v = Value(f = 3.14)
    print("  float -> {describe_value(v)}\n")

    // Null matching
    print("null matching:\n")
    var p = new Point(x = 10, y = 20)
    print("  point -> {describe_point_ptr(p)}\n")
    var np : Point? = null
    print("  null -> {describe_point_ptr(np)}\n")
    unsafe { delete p; }


    // static_match
    print("static_match:\n")
    print("  {static_type_name(42)}\n")
    print("  {static_type_name(3.14)}\n")
    print("  {static_type_name("hi")}\n")

    // Array matching
    print("array matching:\n")
    var sa = fixed_array<int>(0, 0, 0)
    print("  [0,0,0] -> {classify_static_arr(sa)}\n")
    sa = fixed_array<int>(1, 5, 9)
    print("  [1,5,9] -> {classify_static_arr(sa)}\n")
    sa = fixed_array<int>(7, 7, 7)
    print("  [7,7,7] -> {classify_static_arr(sa)}\n")
    let da <- [0, 0, 3, 4]
    print("  [0,0,3,4] -> {classify_dynamic_arr(da)}\n")
    let da2 <- [5, 6]
    print("  [5,6] -> {classify_dynamic_arr(da2)}\n")

    // match_expr
    print("match_expr:\n")
    print("  (1,2,3) consecutive? {is_consecutive_triple((1, 2, 3))}\n")
    print("  (5,6,7) consecutive? {is_consecutive_triple((5, 6, 7))}\n")
    print("  (1,3,5) consecutive? {is_consecutive_triple((1, 3, 5))}\n")

    // Bool matching
    print("bool matching:\n")
    print("  true -> {describe_bool(true)}\n")
    print("  false -> {describe_bool(false)}\n")

    // === Summary ===
    // - require daslib/match
    // - match (expr) { pattern => $ { statements } }
    // - let v = match (expr) { pattern => expression ... _ => expression }
    // - match (a, b) { (pa, pb) => ... } — several values at once
    // - a bare name binds the matched value; _ matches anything
    // - Guards: pattern && condition => ...
    // - OR: pattern1 | pattern2 => ...
    // - Both: whole & Type(field = v) => ... binds the whole value too
    // - Ranges: (0..10) => ... is 0 to 9
    // - Structs: Type(field = value_or_binding) => ...
    // - Enums: Enum.value => ...
    // - Variants: Variant(alternative = binding) => ...
    // - Tuples: (val, _, x) => ...
    // - Arrays: fixed_array<int>(1, b, c) => ...
    // - match_expr: (a, match_expr(a + 1)) — computed patterns
    // - Bool: true / false
    // - Null: null => ... for pointer types
    // - static_match — silently drops type-mismatched arms (generics)

    print("done\n")
}

// output:
// integer matching:
//   0 -> zero
//   1 -> one
//   42 -> other
// enum matching:
//   red -> 1
//   green -> 2
//   black -> 0
// struct matching:
//   (0,0) -> origin
//   (0,5) -> on y-axis at 5
//   (3,0) -> on x-axis at 3
//   (2,7) -> at (2, 7)
// guards:
//   circle 200 -> large circle (200)
//   circle 50 -> small circle (50)
//   square 30 -> square of size 30
// OR patterns:
//   red primary? true
//   black primary? false
// tuple matching:
//   (0,zero) -> zero-zero
//   (1,xyz) -> starts with one
//   (5,hello) -> hello #5
//   (9,bye) -> other
// several values:
//   (0,0) -> origin
//   (0,4) -> on an axis
//   (2,3) -> first
//   (-1,3) -> left half
//   (2,-3) -> fourth
// both patterns:
//   (0,4) -> on the y-axis: (0, 4)
//   (1,4) -> off the y-axis
// range patterns:
//   9 -> digit
//   10 -> some
//   1500 -> some
//   500 -> other
// variant matching:
//   int -> integer: 42
//   float -> float: 3.14
// null matching:
//   point -> (10, 20)
//   null -> null
// static_match:
//   int(42)
//   float(3.14)
//   string(hi)
// array matching:
//   [0,0,0] -> zeros
//   [1,5,9] -> 1,5,9
//   [7,7,7] -> other
//   [0,0,3,4] -> starts with 0,0
//   [5,6] -> starts with 5,6
// match_expr:
//   (1,2,3) consecutive? true
//   (5,6,7) consecutive? true
//   (1,3,5) consecutive? false
// bool matching:
//   true -> yes
//   false -> no
// done
