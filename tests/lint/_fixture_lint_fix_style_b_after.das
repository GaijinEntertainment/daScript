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
    if (x > 10 || x < 0) return -1
    if (x == 5 || (x == 6 ? true : false)) return 0
    if (x == 7) return 1    // a comment keeps the guards apart
    if (x == 8) return 1
    return x
}

def is_small(x : int) : bool {
    return x < 3
}

def is_big(x : int; y : int) : bool {
    return !(x > 3 || y > 3)
}

def odd_one(x : int) : bool {
    if (x == 1) return true
    // the comment keeps both returns
    return false
}

def compares(a : bool; b : bool; s : string) : bool {
    let one = a
    let two = !a
    let three = !(a || b)
    let four = (a && b)
    let five = !(length(s) > 0)
    return one && two && three && four && five
}

def limits(x : int; y : float; lo : int) : float {
    let a = clamp(x, 0, 10)
    let b = clamp(x, 0, 10)
    let c = clamp(x, 0, 10)
    let d = clamp(y, 0.0, 1.0)
    let e = max(min(y, 1.0), 0.0)
    let f = min(max(x, lo), 10)
    let g = min(max(x, 10), -5)
    return float(a + b + c + f + g) + d + e
}

def from_json(v : JsonValue?) : int {
    let a = v ?? 13
    let b = (v ?? -1) + 1
    let c = v?["k"] ?? (a * 2)
    let d = from_JV(v, type<int>)
    return a + b + c + d
}

def bits(var f : Flags; var h : Holder) : bool {
    f.alpha = true
    f.beta = false
    h.flags.beta = true
    let set = f.alpha
    let clear = !h.flags.beta
    return set && clear
}

def rtti(e : Expression?) : bool {
    return string(e.__rtti) == "ExprVar"
}
