options gen2
options persistent_heap

require _common public
require daslib/sort_boost

// Heap-op overhead micro-benchmark. Two scenarios:
//   1. Build-then-extract (heap-sort shape): make_heap + pop_heap loop
//   2. Bounded-N streaming insert (top_n_by shape): push_heap + pop_heap loop
// Sanity check on the per-op cost of the new primitives.

def run_build_then_extract(b : B?; n : int) {
    b |> run("build_extract/{n}", n) {
        var arr <- make_int_array(n)
        sort_boost::make_heap(arr)
        while (!empty(arr)) {
            sort_boost::pop_heap(arr)
            arr |> resize(length(arr) - 1)
        }
        if (!empty(arr)) {
            b->failNow()
        }
    }
}

def run_bounded_topn(b : B?; n : int; cap : int) {
    b |> run("bounded_topn_cap{cap}/{n}", n) {
        var src <- make_int_array(n)
        var buf : array<int>
        buf |> reserve(cap)
        for (x in src) {
            if (length(buf) < cap) {
                buf |> push(x)
                sort_boost::push_heap(buf)
            } elif (x < buf[0]) {
                sort_boost::pop_heap(buf)
                buf[length(buf) - 1] = x
                sort_boost::push_heap(buf)
            }
        }
        if (length(buf) != cap) {
            b->failNow()
        }
    }
}

[benchmark]
def heap_build_extract_10k(b : B?) {
    run_build_then_extract(b, 10000)
}

[benchmark]
def heap_bounded_topn_cap100(b : B?) {
    run_bounded_topn(b, 100000, 100)
}

[benchmark]
def heap_bounded_topn_cap1000(b : B?) {
    run_bounded_topn(b, 100000, 1000)
}
