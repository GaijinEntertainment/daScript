options gen2
options bound_check_elision
require dastest/testing_boost public

[export] def elided_array_sum : int {
    var a : array<int>
    a |> resize(16)
    for (i in range(length(a))) {
        a[i] = i * 3
    }
    var s = 0
    for (i in range(length(a))) {   // nolint:PERF018
        s += a[i]
    }
    return s
}

[export] def elided_fixed_sum : int {
    var a : int[8]
    for (i in range(8)) {
        a[i] = i + 1
    }
    var s = 0
    for (i in urange(8u)) {
        s += a[i]
    }
    return s
}

[export] def elided_vector_sum : float {
    var v = float4(1.0, 2.0, 3.0, 4.0)
    for (i in range(4)) {
        v[i] *= 2.0
    }
    let w = int3(5, 6, 7)
    var s = 0.0
    for (i in range(4)) {
        s += v[i]
    }
    for (i in range(3)) {
        s += float(w[i])
    }
    return s
}

def array_at(a : array<int>; i : int) : int => a[i]

def fixed_at(a : int[4]; i : int) : int => a[i]

def vector_at(v : float4; i : int) : float => v[i]

def panics(blk : block<() : void>) : bool {
    var caught = false
    try {
        invoke(blk)
    } recover {
        caught = true
    }
    return caught
}

[test(description = "an index proven in range reads and writes the right elements")]
def test_elided_index_results(t : T?) {
    t |> equal(elided_array_sum(), 360)
    t |> equal(elided_fixed_sum(), 36)
    t |> equal(elided_vector_sum(), 38.0)
}

[test(description = "an array index not proven in range still panics out of range")]
def test_unproven_array_index_checked(t : T?) {
    var a : array<int>
    a |> resize(4)
    t |> equal(array_at(a, 3), 0)
    let caught1 = panics() {
        let _r = array_at(a, 4)
    }
    t |> success(caught1)
    let caught2 = panics() {
        let _r = array_at(a, -1)
    }
    t |> success(caught2)
}

[test(description = "a fixed array or vector index not proven in range still panics")]
def test_unproven_dim_and_vector_index_checked(t : T?) {
    let f : int[4]
    let v = float4(1.0)
    let caught3 = panics() {
        let _r = fixed_at(f, 4)
    }
    t |> success(caught3)
    let caught4 = panics() {
        let _r = vector_at(v, 4)
    }
    t |> success(caught4)
}

[export] def elided_small_int_sum : int {
    var bytes : array<uint8>
    bytes |> resize(16)
    for (i in range(length(bytes))) {
        bytes[i] = uint8(i * 3)
    }
    var chars : int8[8]
    for (i in range(8)) {
        chars[i] = int8(i - 4)
    }
    var s = 0
    for (i in range(length(bytes))) {   // nolint:PERF018
        s += int(bytes[i])
    }
    for (i in urange(8u)) {
        s += int(chars[i])
    }
    return s
}

[test(description = "a small int element read through an index proven in range has the right value and sign")]
def test_elided_small_int_reads(t : T?) {
    t |> equal(elided_small_int_sum(), 356)
}
