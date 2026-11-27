options gen2
options auto_inline_functions = false

def long_rows(var src : array<array<int>>) : array<array<int>> {
    var rows : array<array<int>>
    for (x in src) {
        if (length(x) > 1) {
            rows |> push_clone(x)
        }
    }
    return <- rows
}

def doubled_values(src : array<int>) : array<int> {
    var doubled : array<int>
    for (x in src) {
        if (x > 1) {
            doubled |> push_clone(x * 2)
        }
    }
    return <- doubled
}

def push(var rows : array<auto(T)>; var row : T&; moving : bool) {
    rows |> emplace(row)
}

def moved_rows(var src : array<array<int>>) : array<array<int>> {
    var moved : array<array<int>>
    for (x in src) {
        moved |> push(x, true)
    }
    return <- moved
}
