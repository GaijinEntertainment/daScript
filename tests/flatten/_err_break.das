options gen2
options indenting = 4

require daslib/flatten

// break / continue inside a flattened loop need per-iteration / loop-scoped masks
// (deferred to a later cut) — rejected loudly for now.
[flatten]
def breaky(x : int) : int {
    var s = 0
    for (i in range(4)) {
        if (i == 2) {
            break
        }
        s = s + x
    }
    return s
}

[export]
def main {
    print("{breaky(3)}\n")
}
