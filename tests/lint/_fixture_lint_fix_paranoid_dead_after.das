//fmt:ignore-file
options gen2
options auto_inline_functions = false

var calls = 0

def bump() : bool {
    calls++
    return calls > 1
}

def unreachable(x : int) : int {
    if (x > 0) {
        return 1
        calls += 2 // keep this note
    }
    return 0
    calls++; calls++
}

def ternaries(c : bool; x : int) : int {
    let a = x + 1
    let b = bump() ? x : x
    return a + b
}

def branches(c : bool) : int {
    var r = 0
    r += 1
    r *= 2
    {
        let t = r
        r = t + 1
    }
    if (bump()) {
        r = 3
    } else {
        r = 3
    }
    if (c) {
        r = 4
    } else {
        r = 4 // same
    }
    return r
}
