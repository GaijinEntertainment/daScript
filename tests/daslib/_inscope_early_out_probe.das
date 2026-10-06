options gen2

def fill(var junk : int[64]) {
    for (i in range(64)) {
        junk[i] = 0x7fffffff
    }
}

def continue_first(n : int) : int {
    var total = 0
    for (i in range(n)) {
        var junk : int[64]
        fill(junk)
        if (i % 2 == 0) {
            continue
        }
        var inscope cells <- [i, i]
        total += length(cells)
    }
    return total
}

def break_first(n : int) : int {
    var total = 0
    for (i in range(n)) {
        var junk : int[64]
        fill(junk)
        if (i == 3) {
            break
        }
        var inscope cells <- [i]
        total += length(cells)
    }
    return total
}

[export]
def main {
    print("{continue_first(6)} {break_first(10)}\n")
}
