options gen2

// Fixture for test_c_header_emit: one function per shape the C boundary has to spell.

enum Axis8 : int8 {
    lo = -3
    hi = 7
}

struct Vec3 {
    x : float
    y : float
    z : float
}

struct Node {
    next : Node?
    v : int
}

[export_c]
def dot(a, b : Vec3) : float {
    return a.x * b.x + a.y * b.y + a.z * b.z
}

[export_c(name = "scale_by")]
def scale(v : Vec3; k : float) : Vec3 {
    return Vec3(x = v.x * k, y = v.y * k, z = v.z * k)
}

[export_c]
def bump(var v : Vec3) {
    v.x += 1.0
}

[export_c]
def stretch(v : float3; k : float) : float3 {
    return v * k
}

[export_c]
def spread(r : range) : range {
    return r
}

[export_c]
def greet(who : string) : string {
    return "hi {who}"
}

[export_c]
def flip(b : bool) : bool {
    return !b
}

[export_c]
def widen(n : int64) : uint64 {
    return uint64(n)
}

[export_c]
def raw(p : void?) : Vec3 const? {
    return unsafe(reinterpret<Vec3 const?>(p))
}

[export_c]
def pick(v : Vec3; ax : Axis8) : float {
    return ax == Axis8.lo ? v.x : v.z
}

[export_c]
def first(d : int[4]) : int {
    return d[0]
}

[export_c]
def depth(n : Node?) : int {
    return n != null ? n.v : 0
}

[export_c]
def nothing() {}
