options gen2
options solid_context
options no_aot

//! a child for the solid block test: a function that only reads the
//! block, so its base is an argument, indexing the first global

def promoted_squares() : int[64] {
    var t : int[64]
    for (i in range(64)) {
        t[i] = i * i
    }
    return t
}

let shared private promoted_tab = promoted_squares()

[hint(noinline)]
def promoted_sum(n : int) : int {
    var s = 0
    for (i in range(n)) {
        s += promoted_tab[i & 63]
    }
    return s
}

[export]
def main {
    print("SOLID_PROMOTED {promoted_sum(1000)}\n")
}
