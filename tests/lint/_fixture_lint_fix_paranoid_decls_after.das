//fmt:ignore-file
options gen2
options auto_inline_functions = false
options _unused_private = true

def private sneaky(var q : int?; v : int) {
    unsafe {
        *reinterpret<int?>(q) = v
    }
}

def private sneaky_pair(a, q : int?; v : int) {
    unsafe {
        *reinterpret<int?>(q) = v + *a
    }
}

def public_sneaky(q : int?; v : int) {
    unsafe {
        *reinterpret<int?>(q) = v
    }
}

// a section note
def private dead_after_comment(x : int) : int {
    return x
}

def live(var q : int?) : int {
    var x = 1
    x += 2 // - note
    x += 3
    let unused = 1 // nolint:LINT002
    sneaky(q, x)
    sneaky_pair(q, q, x)
    return x
}
