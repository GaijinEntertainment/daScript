//! a generic that calls itself with a type built from its own
//! argument type more than once per call never stops instancing;
//! each such chain is one error, whatever carries the type
expect 30516:12

options gen2

def by_tuple(a; n : int) : int {
    return n == 0 ? 0 : by_tuple((a, a), n - 1)
}

def by_variant(a; n : int) : int {
    var v : variant<x : typedecl(a); y : typedecl(a)>
    return n == 0 ? 0 : by_variant(v, n - 1)
}

def by_table(a; n : int) : int {
    var t : table<int; tuple<typedecl(a); typedecl(a)>>
    return n == 0 ? 0 : by_table(t, n - 1)
}

def by_array(a; n : int) : int {
    var t : array<tuple<typedecl(a); typedecl(a)>>
    return n == 0 ? 0 : by_array(t, n - 1)
}

def by_block(a; n : int) : int {
    return n == 0 ? 0 : by_block($(x : typedecl(a)) { return x; }, n - 1)
}

def by_lambda(a; n : int) : int {
    return n == 0 ? 0 : by_lambda(@(x : typedecl(a)) : typedecl(a) { return x; }, n - 1)
}

def by_local_function(a; n : int) : int {
    return n == 0 ? 0 : by_local_function(@@(x : typedecl(a)) : typedecl(a) { return x; }, n - 1)
}

def by_named_type(a : auto(T); n : int) : int {
    var t : tuple<T; T>
    return n == 0 ? 0 : by_named_type(t, n - 1)
}

def pair_of(a : auto(T)) : tuple<T; T> {
    var r : tuple<T; T>
    return r
}

def by_result(a; n : int) : int {
    return n == 0 ? 0 : by_result(pair_of(a), n - 1)
}

def mutual_b(a; n : int) : int {
    return n == 0 ? 0 : mutual_a((a, a), n - 1)
}

def mutual_a(a; n : int) : int {
    return n == 0 ? 0 : mutual_b((a, a), n - 1)
}

def two_wide(a; b; n : int) : int {
    return n == 0 ? 0 : two_wide(b, (a, b), n - 1)
}

def three_wide(a; b; c; n : int) : int {
    return n == 0 ? 0 : three_wide(b, c, (a, b, c), n - 1)
}

[export]
def main {
    print("{by_tuple(1, 3)}\n")
    print("{by_variant(1, 3)}\n")
    print("{by_table(1, 3)}\n")
    print("{by_array(1, 3)}\n")
    print("{by_block(1, 3)}\n")
    print("{by_lambda(1, 3)}\n")
    print("{by_local_function(1, 3)}\n")
    print("{by_named_type(1, 3)}\n")
    print("{by_result(1, 3)}\n")
    print("{mutual_a(1, 3)}\n")
    print("{two_wide(1, 2, 3)}\n")
    print("{three_wide(1, 2, 3, 3)}\n")
}
