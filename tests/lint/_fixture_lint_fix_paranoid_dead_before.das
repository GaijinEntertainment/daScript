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
        calls++
        calls += 2 // keep this note
    }
    return 0
    calls++; calls++
}

def ternaries(c : bool; x : int) : int {
    let a = c ? x + 1 : x + 1
    let b = bump() ? x : x
    return a + b
}

def branches(c : bool) : int {
    var r = 0
    if (c) {
        r += 1
        r *= 2
    } else {
        r += 1
        r *= 2
    }
    if (r > 0) {
        let t = r
        r = t + 1
    } else {
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
