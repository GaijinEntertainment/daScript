options gen2
options indenting = 4


require daslib/export_c

struct Vec2 {
    x : float
    y : float
}

[export_c]
def dot(a, b : Vec2) : float {
    return a.x * b.x + a.y * b.y
}

[export_c(name = "scaled")]
def scale(v : Vec2; k : float) : Vec2 {
    return Vec2(x = v.x * k, y = v.y * k)
}

def length_squared(v : Vec2) : float {
    return dot(v, v)
}
