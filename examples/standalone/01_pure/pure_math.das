options gen2
options stack = 4096

// Tier A: pure POD compute. No das heap is ever touched here - no arrays, no
// tables, no strings, no closures - so the generated context needs only the
// stack and the constant data the standalone emitter puts in globals.
//
// `options stack = 4096` is honored exactly: the standalone context reserves
// this many bytes plus the headroom the global initializers need.

struct Vec3 {
    x : float
    y : float
    z : float
}

enum Axis {
    x
    y
    z
}

let TAPS = fixed_array<float>(0.25, 0.5, 0.25)

[export]
def dot(a, b : Vec3) : float {
    return a.x * b.x + a.y * b.y + a.z * b.z
}

[export]
def scale(v : Vec3; k : float) : Vec3 {
    return Vec3(x = v.x * k, y = v.y * k, z = v.z * k)
}

[export]
def component(v : Vec3; axis : Axis) : float {
    if (axis == Axis.x) {
        return v.x
    } elif (axis == Axis.y) {
        return v.y
    }
    return v.z
}

// A dim (fixed-size array) lives in globals or on the stack, never on the heap.
[export]
def weighted_sum(v : Vec3) : float {
    let parts = fixed_array<float>(v.x, v.y, v.z)
    var total = 0.0
    for (p, w in parts, TAPS) {
        total += p * w
    }
    return total
}

[export]
def collatz_steps(n : int) : int {
    var steps = 0
    var value = n
    while (value > 1) {
        value = value % 2 == 0 ? value / 2 : value * 3 + 1
        steps++
    }
    return steps
}
