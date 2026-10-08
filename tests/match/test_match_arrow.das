options gen2
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/match

struct Point {
    x : int
    y : int
}

variant IntOrStr {
    i : int
    s : string
}

enum Dir {
    north
    east
    south
    west
}

[sideeffects]
def pair_kind(x, y : int) {
    return match (x, y) {
        (0, 0) => "origin"
        (0, _) | (_, 0) => "axis"
        (a, b) && a == b => "diagonal {a}"
        (a, b) => "{a}:{b}"
    }
}

[test]
def test_several_values(t : T?) {
    t |> equal(pair_kind(0, 0), "origin")
    t |> equal(pair_kind(0, 5), "axis")
    t |> equal(pair_kind(5, 0), "axis")
    t |> equal(pair_kind(3, 3), "diagonal 3")
    t |> equal(pair_kind(3, 4), "3:4")
}

[sideeffects]
def mixed_kinds(p : Point; d : Dir; v : IntOrStr) {
    return match (p, d, v) {
        (Point(x = 0, y = y), Dir.north | Dir.south, IntOrStr(i = n)) => "vertical {y} {n}"
        (Point(x = x, y = 0), Dir.east | Dir.west, IntOrStr(s = s)) => "horizontal {x} {s}"
        _ => "other"
    }
}

[test]
def test_several_values_of_different_types(t : T?) {
    t |> equal(mixed_kinds(Point(x = 0, y = 7), Dir.south, IntOrStr(i = 3)), "vertical 7 3")
    t |> equal(mixed_kinds(Point(x = 2, y = 0), Dir.west, IntOrStr(s = "w")), "horizontal 2 w")
    t |> equal(mixed_kinds(Point(x = 2, y = 0), Dir.north, IntOrStr(s = "w")), "other")
}

[sideeffects]
def int_alternation(n : int) {
    return match (n) {
        1 | 2 => "one or two"
        3 => "three"
        _ => "other"
    }
}

[test]
def test_int_alternation_is_not_folded(t : T?) {
    t |> equal(int_alternation(1), "one or two")
    t |> equal(int_alternation(2), "one or two")
    t |> equal(int_alternation(3), "three")
}

[sideeffects]
def guard_keeps_bitwise_or(n : int) {
    return match (n) {
        a && (a | 1) == 3 => "low bits {a}"
        _ => "other"
    }
}

[test]
def test_guard_keeps_bitwise_or(t : T?) {
    t |> equal(guard_keeps_bitwise_or(2), "low bits 2")
    t |> equal(guard_keeps_bitwise_or(3), "low bits 3")
    t |> equal(guard_keeps_bitwise_or(4), "other")
}

[sideeffects]
def bare_name_binds(x : int) {
    let y = 100
    return match (x) {
        y && y > 0 => "bound {y}"
        _ => "outer {y}"
    }
}

[test]
def test_bare_name_binds_instead_of_comparing(t : T?) {
    t |> equal(bare_name_binds(5), "bound 5")
    t |> equal(bare_name_binds(-5), "outer 100")
}

[sideeffects]
def pinned(x, limit : int) {
    return match (x) {
        match_expr(limit) => "at limit"
        _ => "not at limit"
    }
}

[test]
def test_match_expr_compares_a_variable(t : T?) {
    t |> equal(pinned(7, 7), "at limit")
    t |> equal(pinned(6, 7), "not at limit")
}

[sideeffects]
def bool_pair(a, b : bool) {
    return match (a, b) {
        (true, true) => "both"
        (true, false) | (false, true) => "one"
        _ => "none"
    }
}

[test]
def test_bool_patterns(t : T?) {
    t |> equal(bool_pair(true, true), "both")
    t |> equal(bool_pair(false, true), "one")
    t |> equal(bool_pair(true, false), "one")
    t |> equal(bool_pair(false, false), "none")
}

[sideeffects]
def value_in_let(n : int) {
    let s = match (n) {
        0 => "zero"
        k && k < 0 => "negative {-k}"
        k => "positive {k}"
    }
    return s
}

[test]
def test_value_in_let(t : T?) {
    t |> equal(value_in_let(0), "zero")
    t |> equal(value_in_let(-4), "negative 4")
    t |> equal(value_in_let(9), "positive 9")
}

[sideeffects]
def value_as_argument(n : int) {
    return "<" + match (n % 3) {
        0 => "fizz"
        _ => "{n}"
    } + ">"
}

[test]
def test_value_as_argument(t : T?) {
    t |> equal(value_as_argument(3), "<fizz>")
    t |> equal(value_as_argument(4), "<4>")
}

[sideeffects]
def value_inside_statement_arm(n : int) {
    var res = ""
    match (n) {
        0 => $ {
            res = "zero"
        }
        _ => $ {
            res = match (n % 2) {
                0 => "even"
                _ => "odd"
            }
        }
    }
    return res
}

[test]
def test_value_inside_statement_arm(t : T?) {
    t |> equal(value_inside_statement_arm(0), "zero")
    t |> equal(value_inside_statement_arm(3), "odd")
    t |> equal(value_inside_statement_arm(8), "even")
}

[sideeffects]
def assignment_bodies(values : array<int>) {
    var pos = 0
    var neg = 0
    for (v in values) {
        match (v) {
            n && n > 0 => $ { pos += n }
            n && n < 0 => $ { neg -= n }
            _ => $ {}
        }
    }
    return (pos, neg)
}

[test]
def test_assignment_bodies(t : T?) {
    let sums = assignment_bodies([1, -2, 0, 5, -7])
    t |> equal(sums._0, 6)
    t |> equal(sums._1, 9)
}

[sideeffects]
def generic_kind(what) {
    return static_match (what) {
        match_type(type<int>, v) => "int {v}"
        match_type(type<string>, v) => "string {v}"
        _ => "other"
    }
}

[test]
def test_static_match_arrow_arms(t : T?) {
    t |> equal(generic_kind(5), "int 5")
    t |> equal(generic_kind("x"), "string x")
    t |> equal(generic_kind(1.5), "other")
}

class Shape {
    def abstract area : int
}

class Square : Shape {
    side : int
    def override area : int => side * side
}

class Rect : Shape {
    w, h : int
    def override area : int => w * h
}

def make_shape(n : int) : Shape? {
    return match (n) {
        0 => new Square(side = 3)
        _ => new Rect(w = 2, h = n)
    }
}

[test]
def test_value_takes_declared_result_type(t : T?) {
    var sq = make_shape(0)
    var re = make_shape(5)
    t |> equal(sq.area(), 9)
    t |> equal(re.area(), 10)
    var s : Shape? = match (1) {
        1 => new Rect(w = 1, h = 7)
        _ => new Square(side = 1)
    }
    t |> equal(s.area(), 7)
    unsafe {
        delete sq
        delete re
        delete s
    }
}
