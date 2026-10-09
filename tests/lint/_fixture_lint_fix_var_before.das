//fmt:ignore-file
options gen2
options auto_inline_functions = false

struct Box {
    v : int
}

def read_box(var b : Box) : int {
    return b.v
}

def read_two(a : int; var b : Box) : int {
    return a + b.v
}

def read_ptr(p : int?) : int {
    var q = p
    return q == null ? 0 : *q
}

def locals() : int {
    var x = 13
    var y, z : int = 1
    return x + y + z
}
