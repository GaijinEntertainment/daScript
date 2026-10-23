options gen2

require dastest/testing_boost public

struct ThreeInts {
    a : int
    b : int
    c : int
}

// typeinfo leaves this dimension unresolved in the first pass
let UNRESOLVED_DIM = typeinfo sizeof(type<ThreeInts>) / 4

struct WithArr {
    a : array<int>
}

variant HeapVariant {
    p : int
    w : WithArr
}

tuple HeapTuple {
    n : int
    a : array<int>
}

[test]
def test_clone_variant_between_inferred_locals(t : T?) {
    var src = HeapVariant(p = 1)
    var dst = HeapVariant(p = 2)
    dst := src
    t |> success(dst is p)
    t |> equal(dst as p, 1)
}

[test]
def test_clone_variant_deep_copies_heap_alternative(t : T?) {
    var src = HeapVariant(w = WithArr(a <- [1, 2, 3]))
    var dst = HeapVariant(p = 2)
    dst := src
    (src as w).a[0] = 100
    t |> success(dst is w)
    t |> equal(length((dst as w).a), 3)
    t |> equal((dst as w).a[0], 1)
}

[test]
def test_clone_variant_from_inferred_let(t : T?) {
    let src = HeapVariant(w = WithArr(a <- [4, 5]))
    var dst = HeapVariant(p = 2)
    dst := src
    t |> success(dst is w)
    t |> equal((dst as w).a[1], 5)
}

[test]
def test_clone_tuple_between_inferred_locals(t : T?) {
    var src = HeapTuple(n = 1, a <- [1, 2])
    var dst = HeapTuple(n = 2)
    dst := src
    src.a[0] = 100
    t |> equal(dst.n, 1)
    t |> equal(length(dst.a), 2)
    t |> equal(dst.a[0], 1)
}

def lambda_over_unresolved_dim(a : int[UNRESOLVED_DIM]) : int {
    let fn <- @(x : int[UNRESOLVED_DIM]) : int {
        return x[1] + length(x)
    }
    return invoke(fn, a)
}

[test]
def test_lambda_with_unresolved_dim_argument(t : T?) {
    let x = fixed_array(10, 20, 30)
    t |> equal(lambda_over_unresolved_dim(x), 23)
}
