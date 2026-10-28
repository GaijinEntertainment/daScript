//fmt:ignore-file
options gen2
options auto_inline_functions = false
options _unused_private = true

def private sneaky(q : int?; v : int) {
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

//! never called
def private dead_helper(x : int) : int {
    return x + 1
}

def private dead_short(x : int) : int => x * 2

// a section note
def private dead_after_comment(x : int) : int {
    return x
}

struct private DeadStruct {
    a : int
    b : float
}

enum private DeadEnum {
    one
    two
}

def live(var q : int?) : int {
    var x = 1 // nolint:LINT007
    x += 2 // nolint:LINT007,LINT008 - note
    x += 3 // nolint: LINT007, LINT006
    // nolint:LINT007
    let unused = 1 // nolint:LINT002,LINT007 - the reason stays with a live code
    sneaky(q, x)
    sneaky_pair(q, q, x)
    return x
}
