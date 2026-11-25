options gen2

//! a program for uncategorized_no_aot.das: one site of each kind
//! log_inscope_pod reports
var g = 1

def total(xs : array<int>) : int {
    var s = 0
    for (x in xs) {
        s += x
    }
    return s
}

def grow(var xs : array<int>) : int {
    xs |> push(1)
    return length(xs)
}

def pair_from(v : int) : array<int> => <- [v, v + 1]

def total_of_all(rows : array<array<int>>) : int {
    var s = 0
    for (row in rows) {
        s += total(row)
    }
    return s
}

def beside_unsafe() : int {
    let p = unsafe(addr(g))
    var kept : array<int>
    kept |> push(g)
    var moved : array<int>
    moved <- kept
    return *p + length(moved)
}

def returns_local() : array<int> {
    var made : array<int>
    made |> push(g)
    return <- made
}

def holds_lambdas() : int {
    var steps : array<lambda<() : int>>
    return length(steps)
}

def yields_literal() : iterator<int> {
    return <- generator<int>() <| $() {   // nolint:STYLE002 piped block only
        for (x in [g, 2]) {
            yield x
        }
        yield total([g, 4])
        return false
    }
}

[export]
def main() : int {
    var s = 0
    for (x in [g, 2, 3]) {
        s += x
    }
    for (x in each([g, 2])) {
        s += x
    }
    for (x in yields_literal()) {
        s += x
    }
    s += total_of_all([pair_from(g)])
    s += length(returns_local())
    return s + total([g, 2]) + grow([g, 2]) + beside_unsafe() + holds_lambdas()
}
