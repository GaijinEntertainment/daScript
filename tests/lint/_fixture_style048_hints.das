options gen2

// STYLE048 fixture: every function is one decision the rule makes;
// test_style048_hints.das pins the exact finding each one reports,
// or that it reports none.

require daslib/ast
require daslib/match

enum Mode {
    idle
    run
    stop
}

enum Wide {
    a
    b
    c
    d
    e
    f
    g
}

variant Shape {
    circle : float
    square : float
}

def enum_chain(m : Mode) : int {
    var r = 0
    if (m == Mode.idle) {
        r = 1
    } elif (m == Mode.run) {
        r = 2
    } elif (m == Mode.stop) {
        r = 3
    }
    return r
}

def two_subjects(m : Mode; n : int) : int {
    var r = 0
    if (m == Mode.idle && n >= 0 && n < 10) {
        r = 1
    } elif (m == Mode.run || m == Mode.stop && n == 5) {
        r = 2
    } elif (n == 100) {
        r = 3
    } else {
        r = 4
    }
    return r
}

def variant_chain(s : Shape) : float {
    var r = 0.0
    if (s is circle) {
        r = s as circle
    } elif (s is square) {
        r = s as square
    }
    return r
}

def nested_if(a, b : int) : int {
    var r = 0
    if (a == 1) {
        if (b == 2) {
            r = 10
        } elif (b == 3) {
            r = 11
        }
    } elif (a == 2) {
        r = 12
    }
    return r
}

def nested_exhaustive_inner(a : int; b : bool) : int {
    var r = 0
    if (a == 1) {
        if (b) {
            r = 10
        } elif (!b) {
            r = 11
        }
    } elif (a == 2) {
        r = 12
    }
    return r
}

def dead_arm(m : Mode) : int {
    var r = 0
    if (m == Mode.idle || m == Mode.run) {
        r = 1
    } elif (m == Mode.run) {
        r = 2
    } elif (m == Mode.stop) {
        r = 3
    }
    return r
}

def dead_else(m : Mode) : int {
    var r = 0
    if (m == Mode.idle) {
        r = 1
    } elif (m == Mode.run) {
        r = 2
    } elif (m == Mode.stop) {
        r = 3
    } else {
        r = 4
    }
    return r
}

def same_bodies(k : int; op : string) : int {
    var r = 0
    if (k == 0 && op == "==") {
        r = 1
    } elif (k == 0 && op == "<=") {
        r = 1
    } elif (k == 1 && op == "<") {
        r = 2
    } elif (k == 1 && op == "==") {
        r = 3
    }
    return r
}

def merge_blocked(k : int; op : string) : int {
    var r = 0
    if (k == 0 && op == "x") {
        r = 1
    } elif (op == "a") {
        r = 2
    } elif (k == 1) {
        r = 1
    }
    return r
}

def factored(k : int; op : string) : int {
    var r = 0
    if ((k == 0 && op == "a") || (k == 0 && op == "b")) {
        r = 1
    } elif (k == 1 && op == "a") {
        r = 2
    } elif (k == 2 && op == "c") {
        r = 3
    }
    return r
}

def ranges(n : int) : string {
    var r = ""
    if (n >= 0 && n < 10) {
        r = "digit"
    } elif (n >= 10 && n <= 99) {
        r = "two"
    } elif (n == 100) {
        r = "hundred"
    }
    return r
}

def hex_bound(n : int) : int {
    var r = 0
    if (n >= 0 && n <= 0xFF) {
        r = 1
    } elif (n >= 0x100 && n < 0x200) {
        r = 2
    } elif (n == 0x200) {
        r = 3
    }
    return r
}

def exclusive_low(n : uint) : int {
    var r = 0
    if (n > 9u && n < 20u) {
        r = 1
    } elif (n >= 20u && n <= 29u) {
        r = 2
    } elif (n == 30u) {
        r = 3
    }
    return r
}

def strings(verb : string) : int {
    var r = 0
    if (verb == "GET") {
        r = 1
    } elif (verb == "PUT") {
        r = 2
    } elif (verb == "DELETE") {
        r = 3
    }
    return r
}

def bool_and_enum(on : bool; m : Mode) : int {
    var r = 0
    if (on && m == Mode.idle) {
        r = 1
    } elif (!on && m == Mode.idle) {
        r = 2
    } elif (on && m == Mode.run) {
        r = 3
    } elif (!on && m == Mode.run) {
        r = 4
    }
    return r
}

def unmatched_named(m : Mode; n : int) : int {
    var r = 0
    if (m == Mode.idle && n == 0) {
        r = 1
    } elif (m == Mode.idle && n == 1) {
        r = 2
    } elif (m == Mode.run) {
        r = 3
    }
    return r
}

def unmatched_one(w : Wide) : int {
    var r = 0
    if (w == Wide.a || w == Wide.b) {
        r = 1
    } elif (w == Wide.c || w == Wide.d) {
        r = 2
    } elif (w == Wide.e || w == Wide.f) {
        r = 3
    }
    return r
}

def unmatched_many(w : Wide) : int {
    var r = 0
    if (w == Wide.a) {
        r = 1
    } elif (w == Wide.b) {
        r = 2
    } elif (w == Wide.c) {
        r = 3
    }
    return r
}

def subject_once(a, b : int) : int {
    var r = 0
    if (a == 1 && b == 2) {
        r = 1
    } elif (a == 2) {
        r = 2
    } elif (a == 3) {
        r = 3
    }
    return r
}

def big_unsigned(u : uint64) : int {
    var r = 0
    if (u == 0xFFFFFFFFFFFFFFFFul) {
        r = 1
    } elif (u == 1ul) {
        r = 2
    } elif (u == 2ul) {
        r = 3
    } elif (u == 3ul) {
        r = 4
    }
    return r
}

def quiet_two_arms(x : int) : int {
    var r = 0
    if (x == 1) {
        r = 1
    } elif (x == 2) {
        r = 2
    }
    return r
}

def quiet_guards(x : int; f, g : bool) : int {
    var r = 0
    if (x == 1 && f) {
        r = 1
    } elif (x == 2 && g) {
        r = 2
    } elif (x == 3) {
        r = 3
    }
    return r
}

def quiet_float(x : float) : int {
    var r = 0
    if (x == 1.0) {
        r = 1
    } elif (x == 2.0) {
        r = 2
    } elif (x == 3.0) {
        r = 3
    }
    return r
}

def quiet_too_many_alternatives(a, b, c, d : int) : int {
    var r = 0
    if ((a == 1 || a == 2) && (b == 1 || b == 2) && (c == 1 || c == 2) && (d == 1 || d == 2)) {
        r = 1
    } elif (a == 3) {
        r = 2
    } elif (a == 4) {
        r = 3
    }
    return r
}

def quiet_too_many_subjects(a, b, c, d, e : int) : int {
    var r = 0
    if (a == 1 && b == 1 && c == 1 && d == 1 && e == 1) {
        r = 1
    } elif (a == 2 && b == 2 && c == 2 && d == 2 && e == 2) {
        r = 2
    } elif (a == 3 && b == 3 && c == 3 && d == 3 && e == 3) {
        r = 3
    }
    return r
}

def quiet_dead_alternative(x : int) : int {
    var r = 0
    if (x == 1) {
        r = 1
    } elif (x == 1 || x == 2) {
        r = 2
    } elif (x == 3) {
        r = 3
    }
    return r
}

def quiet_guarded_inner(a, b : int) : int {
    var r = 0
    if (a == 1) {
        if (b == 2 && b > a) {
            r = 10
        } else {
            r = 11
        }
    } elif (a == 2) {
        r = 12
    }
    return r
}

def quiet_rtti(e : Expression?) : int {
    var r = 0
    if (e.__rtti == "ExprVar") {
        r = 1
    } elif (e.__rtti == "ExprCall") {
        r = 2
    } elif (e.__rtti == "ExprField") {
        r = 3
    }
    return r
}

def quiet_already_match(m : Mode) : int {
    var r = 0
    match (m) {
        Mode.idle => $ {
            r = 1
        }
        Mode.run => $ {
            r = 2
        }
        Mode.stop => $ {
            r = 3
        }
    }
    return r
}
