//fmt:ignore-file
options gen2
options auto_inline_functions = false

def private take(blk : block<(a : int; b : int) : int>) : int {
    return invoke(blk, 1, 2)
}

def private unused_arg(a : int; _b : int) : int {
    return a
}

def public_unused_arg(a : int; b : int) : int {
    return a
}

def unused_locals() : int {
    let _x = 13
    var total = 0
    for (_i in range(3)) {
        total += 2
    }
    return total
}

def renamed() : int {
    var y = 1
    y += 2
    for (k in range(3)) {
        y += k
    }
    return y + take($(p, b) => p + b + y)
}

def block_arg() : int {
    return take($(p, _q) => p)
}

def shadowed(y : int) : int {
    var _y = y
    _y += 1
    return _y
}

def in_comment() : int {
    var _z = 1
    _z += 1 // _z grows
    return _z
}

struct Pt {
    v : int
}

def field_named(p : Pt) : int {
    var _v = p.v
    _v += 1
    return _v
}

def prefixed_taken() : int {
    var _t = 1
    _t += 1
    {
        let t = 2
    }
    return _t
}

def in_string() : string {
    var _w = 1
    _w += 1
    return "{_w} _w"
}
