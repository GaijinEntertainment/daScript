//fmt:ignore-file
options gen2
options auto_inline_functions = false

require daslib/ast
require daslib/json_boost
require math

bitfield Flags {
    alpha
    beta
}

struct Holder {
    flags : Flags
}

def guards(x : int) : int {
    if (x > 10) return -1
    if (x < 0) {
        return -1
    }
    if (x == 5) return 0
    if (x == 6 ? true : false) return 0
    if (x == 7) return 1    // a comment keeps the guards apart
    if (x == 8) return 1
    return x
}

def is_small(x : int) : bool {
    if (x < 3) {
        return true
    } else {
        return false
    }
}

def is_big(x : int; y : int) : bool {
    if (x > 3 || y > 3) return false
    return true
}

def odd_one(x : int) : bool {
    if (x == 1) return true
    // the comment keeps both returns
    return false
}

def compares(a : bool; b : bool; s : string) : bool {
    let one = a == true
    let two = a != true
    let three = false == (a || b)
    let four = (a && b) == true
    let five = length(s) > 0 == false
    return one && two && three && four && five
}

def limits(x : int; y : float; lo : int) : float {
    let a = min(max(x, 0), 10)
    let b = max(min(x, 10), 0)
    let c = min(10, max(0, x))
    let d = min(max(y, 0.0), 1.0)
    let e = max(min(y, 1.0), 0.0)
    let f = min(max(x, lo), 10)
    let g = min(max(x, 10), -5)
    return float(a + b + c + f + g) + d + e
}

def from_json(v : JsonValue?) : int {
    let a = from_JV(v, type<int>, 13)
    let b = from_JV(v, type<int>, -1) + 1
    let c = from_JV(v?["k"], type<int>, a * 2)
    let d = from_JV(v, type<int>)
    return a + b + c + d
}

def bits(var f : Flags; var h : Holder) : bool {
    f |= Flags.alpha
    f &= ~Flags.beta
    h.flags |= Flags.beta
    let set = uint(f & Flags.alpha) != 0u
    let clear = int(h.flags & Flags.beta) == 0
    return set && clear
}

def rtti(e : Expression?) : bool {
    return string(e.__rtti) == "ExprVar"
}
