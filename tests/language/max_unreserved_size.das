options gen2
options max_unreserved_size = 65536

require dastest/testing_boost

// The unreserved-resize guard: a resize that has to GROW past max_unreserved_size bytes
// panics; a prior exact reserve means the resize never grows, so it passes at any size.
// The 64 KB option above keeps every case cheap; the shipped default is 64 MB.

[test]
def test_unreserved_resize_panics(t : T?) {
    t |> run("bare growing resize past the threshold panics") @(t : T?) {
        var failed = false
        try {
            var a : array<uint8>
            a |> resize(65537)
        } recover {
            failed = true
        }
        t |> success(failed, "resize over the byte threshold must panic")
    }
    t |> run("resize_no_init is the same growth path") @(t : T?) {
        var failed = false
        try {
            var a : array<uint8>
            a |> resize_no_init(65537)
        } recover {
            failed = true
        }
        t |> success(failed, "resize_no_init over the threshold must panic")
    }
    t |> run("the int64 overload is the same growth path") @(t : T?) {
        var failed = false
        try {
            var a : array<uint8>
            a |> resize(65537l)
        } recover {
            failed = true
        }
        t |> success(failed, "int64 resize over the threshold must panic")
    }
    t |> run("stride counts toward the byte threshold") @(t : T?) {
        var failed = false
        try {
            var a : array<int>
            a |> resize(20000)          // 80000 bytes: the threshold is bytes, not elements
        } recover {
            failed = true
        }
        t |> success(failed, "the guard measures newSize * stride")
    }
}

[test]
def test_reserved_and_small_resizes_pass(t : T?) {
    t |> run("reserve-then-resize passes at any size, capacity exact") @(t : T?) {
        var a : array<uint8>
        a |> reserve(200000)
        a |> resize(200000)
        t |> equal(length(a), 200000)
        t |> equal(capacity(a), 200000)
    }
    t |> run("below the threshold resize passes, capacity pow2") @(t : T?) {
        var a : array<int>
        a |> resize(1000)               // 4000 bytes, well under the 64 KB threshold
        t |> equal(capacity(a), 1024)
    }
    t |> run("ensure_capacity append crosses the threshold without panic") @(t : T?) {
        var a : array<uint8>
        var size = 0
        while (size < 200000) {
            size += 30000
            a |> ensure_capacity(size)
            a |> resize(size)
        }
        t |> equal(length(a), 210000)
        t |> success(capacity(a) >= 210000)
    }
    t |> run("shrink never panics regardless of size") @(t : T?) {
        var a : array<uint8>
        a |> reserve(200000)
        a |> resize(200000)
        a |> resize(100000)             // shrink of an over-threshold array is not a growth
        t |> equal(length(a), 100000)
        a |> pop()
        t |> equal(length(a), 99999)
    }
}

[test]
def test_max_unreserved_size_api(t : T?) {
    t |> run("option, getter, and runtime setter agree") @(t : T?) {
        t |> equal(max_unreserved_size(), 65536ul)
        set_max_unreserved_size(1048576ul)
        t |> equal(max_unreserved_size(), 1048576ul)
        var ok : array<uint8>
        ok |> resize(100000)            // under the raised limit now
        t |> equal(length(ok), 100000)
        set_max_unreserved_size(65536ul)
        var failed = false
        try {
            var a : array<uint8>
            a |> resize(100000)
        } recover {
            failed = true
        }
        t |> success(failed, "lowering the limit back re-arms the guard")
    }
}
