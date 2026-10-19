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
    var z = 1
    z += 1 // _z grows
    return z
}

struct Pt {
    v : int
}

def field_named(p : Pt) : int {
    var v = p.v
    v += 1
    return v
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
    var w = 1
    w += 1
    return "{w} _w"
}

class Counter {
    hits : int
    def bump() : int {
        var _hits = 1
        _hits += hits
        return _hits
    }
}
