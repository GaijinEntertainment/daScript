options gen2

require daslib/export_c


enum Kind : int16 {
    one = 1
    two
}

struct Inner {
    a : int8
    b : int64
}

struct Outer {
    i : Inner
    p : Inner?
    taps : float[3]
    k : Kind
}

[export_c]
def outer_a(o : Outer) : int {
    return int(o.i.a)
}
