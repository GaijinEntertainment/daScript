options gen2
options persistent_heap

require dastest/testing_boost

let N = 10000
let OUTER = 10
let TOTAL = N * OUTER

def fill_through_argument(var p : int?) {
    for (_j in range(OUTER)) {
        for (i in range(N)) {
            unsafe(p[i]) = i + 1
        }
    }
}

[benchmark]
def ptr_set_int(b : B?) {
    var arr : array<int>
    arr |> resize(N)
    var p = unsafe(addr(arr[0]))
    b |> run("local_ptr_local_idx/{TOTAL}", TOTAL) {
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                unsafe(p[i]) = i + 1
            }
        }
        b |> equal(arr[N - 1], N)
    }
    b |> run("local_ptr_expr_idx/{TOTAL}", TOTAL) {
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                unsafe(p[N - 1 - i]) = i + 2
            }
        }
        b |> equal(arr[0], N + 1)
    }
    b |> run("argument_ptr_local_idx/{TOTAL}", TOTAL) {
        fill_through_argument(p)
        b |> equal(arr[N - 1], N)
    }
    b |> run("deref_local_ptr/{TOTAL}", TOTAL) {
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                *p = i + 3
            }
        }
        b |> equal(arr[0], N + 2)
    }
}

[benchmark]
def ptr_set_uint8(b : B?) {
    var bytes : array<uint8>
    bytes |> resize(N)
    var p = unsafe(addr(bytes[0]))
    b |> run("local_ptr_local_idx_narrowed/{TOTAL}", TOTAL) {
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                unsafe(p[i]) = uint8(i + 1)
            }
        }
        b |> equal(int(bytes[254]), 255)
    }
    b |> run("deref_walking_ptr_narrowed/{TOTAL}", TOTAL) {
        for (_j in range(OUTER)) {
            var dest_addr : uint64 = unsafe(reinterpret<uint64>(p))
            for (i in range(N)) {
                *unsafe(reinterpret<uint8?>(dest_addr)) = uint8(i + 2)
                dest_addr++
            }
        }
        b |> equal(int(bytes[253]), 255)
    }
}
