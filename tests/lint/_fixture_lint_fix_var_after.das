//fmt:ignore-file
options gen2
options auto_inline_functions = false

struct Box {
    v : int
}

def read_box(b : Box) : int {
    return b.v
}

def read_two(a : int; b : Box) : int {
    return a + b.v
}

def read_ptr(p : int?) : int {
    let q = p
    return q == null ? 0 : *q
}

def locals() : int {
    let x = 13
    let y, z : int = 1
    return x + y + z
}
