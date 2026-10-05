options gen2

expect 31209:4

require daslib/style_lint


def bad_arrays() : int {
    var kq : array<uint8>
    var ks : array<uint8>
    return length(kq) + length(ks)
}

def bad_run_of_three() : int {
    var x, y : int
    var z : int
    return x + y + z
}

def bad_inscope() : int {
    var inscope seen : table<string>
    var inscope dupes : table<string>
    return length(seen) + length(dupes)
}

def bad_lets() : int {
    let a : int
    let b : int
    return a + b
}


def good_initialized() : int {
    var inscope ids <- [1, 2]
    var inscope ends <- [3]
    return length(ids) + length(ends)
}

def good_commented() : int {
    var rows : int  // one row per entry
    var cols : int  // one column per field
    return rows + cols
}

def good_annotated() : int {
    var @exact_size first : array<int>
    var @exact_size second : array<int>
    return length(first) + length(second)
}

def good_different_types() : float {
    var i : int
    var f : float
    return float(i) + f
}

def good_different_heads() : int {
    var inscope a : array<int>
    var b : array<int>
    return length(a) + length(b)
}

def good_separated() : int {
    var rows : array<int>

    var cols : array<int>
    return length(rows) + length(cols)
}

def good_generic(a : auto(TT)) : int {
    var x : TT
    var y : int
    return int(x) + y + int(a)
}

[export]
def main {
    print("{good_generic(1)}\n")
}
