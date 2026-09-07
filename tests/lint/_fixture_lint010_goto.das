options gen2

// LINT010 fixture: every store below a goto or a label is read at the jump target, so none is
// dead; the last function has no jumps and its first store is the one genuine finding.

def forward_goto(n : int) : int {
    var res = 1
    if (n > 0) {
        goto label 1
    }
    res = 2
    label 1:
    return res
}

def computed_goto(n : int) : int {
    var res = -1
    goto n
    label 0:
    res = 10
    goto label 2
    label 1:
    res = 11
    goto label 2
    label 2:
    return res
}

def store_before_backward_label() : int {
    var acc = 0
    var i = 0
    label 0:
    acc += i
    i++
    if (i < 3) {
        goto label 0
    }
    return acc
}

def genuine_dead_store(x : int) : int {
    var t = 0
    t = x * 2
    return t
}
