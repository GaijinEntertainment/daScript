options gen2
options persistent_heap

require _common public
require daslib/sort_boost

// partial_sort(arr, N) — sort just the first N elements; remainder unspecified.
// Compares against full sort over the same array. As N → length the two
// converge; as N → 1 partial_sort wins by an arbitrarily large margin.

def run_partial_sort_int(b : B?; n : int; k : int) {
    b |> run("partial_sort_int_N{k}/{n}", n) {
        var arr <- make_int_array(n)
        sort_boost::partial_sort(arr, k)
        if (arr[0] > arr[1]) {
            b->failNow()
        }
    }
}

def run_full_sort_then_take(b : B?; n : int; k : int) {
    b |> run("full_sort_take_N{k}/{n}", n) {
        var arr <- make_int_array(n)
        sort(arr)
        if (arr[k - 1] < arr[0]) {
            b->failNow()
        }
    }
}

def run_partial_sort_struct(b : B?; n : int; k : int) {
    b |> run("partial_sort_struct_N{k}/{n}", n) {
        var arr <- make_struct_array(n)
        sort_boost::partial_sort(arr, k) $(x, y : KeyedItem) : bool {
            return x.key < y.key
        }
        if (arr[0].key > arr[1].key) {
            b->failNow()
        }
    }
}

// Same M=100K source array, sweep N.

[benchmark]
def partial_sort_int_n1(b : B?) {
    run_partial_sort_int(b, 100000, 1)
}

[benchmark]
def partial_sort_int_n10(b : B?) {
    run_partial_sort_int(b, 100000, 10)
}

[benchmark]
def partial_sort_int_n100(b : B?) {
    run_partial_sort_int(b, 100000, 100)
}

[benchmark]
def partial_sort_int_n1000(b : B?) {
    run_partial_sort_int(b, 100000, 1000)
}

[benchmark]
def full_sort_take_n100(b : B?) {
    run_full_sort_then_take(b, 100000, 100)
}

[benchmark]
def partial_sort_struct_n100(b : B?) {
    run_partial_sort_struct(b, 100000, 100)
}
