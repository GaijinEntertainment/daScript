options gen2

struct P {
    x : int
    q : Q
}

struct Q {
    y : int
}

variant V {
    i : int
    f : float
}

def mark(a : auto; _text : string) : int => 0

def spans(p : P; pp : P?; vv : V; f3 : float3) : int {
    var total = 0
    total += mark(p.x, "p.x")
    total += mark(p.q.y, "p.q.y")
    total += mark(p.q, "p.q")
    total += mark(1 + p.x, "1 + p.x")
    total += mark(pp?.x, "pp?.x")
    total += mark(unsafe(vv as i), "vv as i")
    total += mark(vv is i, "vv is i")
    total += mark(unsafe(vv ?as i), "vv ?as i")
    total += mark(f3.xy, "f3.xy")
    return total
}
