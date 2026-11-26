options gen2
options persistent_heap

require dastest/testing_boost
require daslib/linq

let N = 10000
let OUTER = 10
let TOTAL = N * OUTER

struct Holder {
    data : int?
}

var g_data : int?

def sum_through_argument(p : int?) {
    var sum = 0
    for (_j in range(OUTER)) {
        for (i in range(N)) {
            sum += unsafe(p[i])
        }
    }
    return sum
}

def make_ints() => <- [for (i in range(N)); i & 1023]

[benchmark]
def ptr_at_int(b : B?) {
    var arr <- make_ints()
    let expected = sum(arr) * OUTER
    var p = unsafe(addr(arr[0]))
    b |> run("local_ptr_local_idx/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += unsafe(p[i])
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("local_ptr_expr_idx/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += unsafe(p[N - 1 - i])
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("local_ptr_uint_idx/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in urange(uint(N))) {
                sum += unsafe(p[i])
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("local_ptr_const_idx/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (_i in range(N)) {
                sum += unsafe(p[1])
            }
        }
        b |> equal(sum, TOTAL)
    }
    b |> run("argument_ptr_local_idx/{TOTAL}", TOTAL) {
        b |> equal(sum_through_argument(p), expected)
    }
    g_data = p
    b |> run("global_ptr_local_idx/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += unsafe(g_data[i])
            }
        }
        b |> equal(sum, expected)
    }
    g_data = null
    var holder = new Holder(data = p)
    b |> run("field_ptr_local_idx/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += unsafe(holder.data[i])
            }
        }
        b |> equal(sum, expected)
    }
    unsafe {
        delete holder
    }
}

[benchmark]
def ptr_at_uint8(b : B?) {
    var bytes <- [for (i in range(N)); uint8(i & 255)]
    var expected = 0
    for (v in bytes) {
        expected += int(v)
    }
    expected *= OUTER
    let p = unsafe(addr(bytes[0]))
    b |> run("local_ptr_local_idx/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += int(unsafe(p[i]))
            }
        }
        b |> equal(sum, expected)
    }
}

[benchmark]
def ptr_at_float3(b : B?) {
    var points <- [for (i in range(N)); float3(float(i & 7), 1.0, 2.0)]
    let p = unsafe(addr(points[0]))
    b |> run("local_ptr_local_idx/{TOTAL}", TOTAL) {
        var sum = float3(0.0)
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += unsafe(p[i])
            }
        }
        b |> equal(sum.y, float(TOTAL))
    }
}
