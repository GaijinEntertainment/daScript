options gen2
options persistent_heap

require _common public
require daslib/sort_boost

// nth_element(arr, N) — place the kth element at position k without sorting
// either side. Compared against full sort + index. nth_element scales like
// O(M); full sort like O(M log M). For median (k = M/2) nth_element should
// dominate at any M.

def run_nth_element(b : B?; n : int; k : int) {
    b |> run("nth_element_k{k}/{n}", n) {
        var arr <- make_int_array(n)
        sort_boost::nth_element(arr, k)
        if (arr[k] < 0) {
            b->failNow()
        }
    }
}

def run_full_sort_index(b : B?; n : int; k : int) {
    b |> run("full_sort_index_k{k}/{n}", n) {
        var arr <- make_int_array(n)
        sort(arr)
        if (arr[k] < 0) {
            b->failNow()
        }
    }
}

[benchmark]
def nth_element_head_k10(b : B?) {
    run_nth_element(b, 100000, 10)
}

[benchmark]
def nth_element_median(b : B?) {
    run_nth_element(b, 100000, 50000)
}

[benchmark]
def nth_element_tail_k99990(b : B?) {
    run_nth_element(b, 100000, 99990)
}

[benchmark]
def full_sort_index_median(b : B?) {
    run_full_sort_index(b, 100000, 50000)
}
