options gen2
options _enable_default_off_rules = true

expect 31209:8

require daslib/style_lint
require daslib/match

enum Mode {
    idle
    run
    stop
}

variant Shape {
    circle : float
    square : float
}

def bad_enum_chain(m : Mode) : int {
    var r = 0
    if (m == Mode.idle) {                               // STYLE048
        r = 1
    } elif (m == Mode.run) {
        r = 2
    } elif (m == Mode.stop) {
        r = 3
    }
    return r
}

def bad_two_subjects(m : Mode; n : int) : int {
    var r = 0
    if (m == Mode.idle && n >= 0 && n < 10) {           // STYLE048
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

def bad_variant(s : Shape) : float {
    var r = 0.0
    if (s is circle) {                                  // STYLE048
        r = s as circle
    } elif (s is square) {
        r = s as square
    }
    return r
}

def bad_nested(a, b : int) : int {
    var r = 0
    if (a == 1) {                                       // STYLE048
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

def bad_dead_arm(m : Mode) : int {
    var r = 0
    if (m == Mode.idle || m == Mode.run) {
        r = 1
    } elif (m == Mode.run) {                            // STYLE048: can never run
        r = 2
    } elif (m == Mode.stop) {
        r = 3
    }
    return r
}

def bad_dead_else(m : Mode) : int {
    var r = 0
    if (m == Mode.idle) {                               // STYLE048: the else never runs
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

def bad_same_bodies(k : int; op : string) : int {
    var r = 0
    if (k == 0 && op == "==") {                         // STYLE048: arms with one body merge
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

def bad_ranges(n : int) : string {
    var r = ""
    if (n >= 0 && n < 10) {                             // STYLE048: ranges and constants
        r = "digit"
    } elif (n >= 10 && n <= 99) {
        r = "two"
    } elif (n == 100) {
        r = "hundred"
    }
    return r
}

def good_match(m : Mode) : int {
    var r = 0
    match (m) {
        Mode.idle => $ { r = 1 }
        Mode.run => $ { r = 2 }
        Mode.stop => $ { r = 3 }
    }
    return r
}

def good_two_arms(x : int) : int {
    var r = 0
    if (x == 1) {
        r = 1
    } elif (x == 2) {
        r = 2
    }
    return r
}

def good_guards(x : int; f, g : bool) : int {
    var r = 0
    if (x == 1 && f) {
        r = 1
    } elif (g) {
        r = 2
    } elif (f || g) {
        r = 3
    }
    return r
}

def good_float(x : float) : int {
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

def good_nested_with_prefix(a, b : int) : int {
    var r = 0
    if (a == 1) {
        r = 5
        if (b == 2) {
            r = 10
        }
    } elif (a == 2) {
        r = 12
    }
    return r
}
