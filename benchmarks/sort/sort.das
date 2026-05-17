options gen2
options persistent_heap

require _common public

// Baseline: plain ``sort(arr, cmp)`` over int / user-struct arrays. Boris
// flagged that sort itself had no dedicated benchmark; this fills the gap and
// gives a reference point for the partial_sort / nth_element / heap_ops files.

def run_sort_int(b : B?; n : int) {
    b |> run("sort_int/{n}", n) {
        var arr <- make_int_array(n)
        sort(arr)
        if (arr[0] > arr[1]) {
            b->failNow()
        }
    }
}

def run_sort_struct_by_key(b : B?; n : int) {
    b |> run("sort_struct_by_key/{n}", n) {
        var arr <- make_struct_array(n)
        sort(arr) $(x, y : KeyedItem) : bool {
            return x.key < y.key
        }
        if (arr[0].key > arr[1].key) {
            b->failNow()
        }
    }
}

[benchmark]
def sort_int_10k(b : B?) {
    run_sort_int(b, 10000)
}

[benchmark]
def sort_int_100k(b : B?) {
    run_sort_int(b, 100000)
}

[benchmark]
def sort_struct_by_key_100k(b : B?) {
    run_sort_struct_by_key(b, 100000)
}
