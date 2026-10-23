options gen2


require daslib/export_c

struct Pair {
    lo : int
    hi : int
}

var g_seen = 0

[export_c]
def widen(p : Pair) : Pair {
    g_seen++
    return Pair(lo = p.lo - 1, hi = p.hi + 1)
}

[export_c(name = "tally")]
def count_seen() : int => g_seen

[export_c]
def mapped(n : int) : int {
    let fx <- @@(x : int) => x * 3
    return invoke(fx, n)
}

[export_c]
def count_keys(n : int) : int {
    var tab : table<int; int>
    for (i in range(n)) {
        tab[i % 7] += i
    }
    return length(tab)
}

def helper_public(x : int) : int => x + 1
