options gen2
options bound_check_elision
require dastest/testing_boost public

// guard forms exercised via the CFG range analysis (index is a uint parameter, so it is not
// constant-folded and the branch guard is the only thing proving it in range).
def guard_early_exit(var a : array<int>; x : uint) {
    if (x >= uint(length(a))) {
        return
    }
    a[x] = int(x) * 10
}

def guard_then(var a : array<int>; x : uint) {
    if (x < uint(length(a))) {
        a[x] = int(x) * 10
    }
}

// when bound_check_elision is on, the optimizer marks provably-in-range
// array/vector accesses as noBoundCheck. behavior must stay identical.

[test]
def test_bound_check_elision(t : T?) {
    t |> run("constant index into fixed array") @(t : T?) {
        var a : int[4]
        for (i in range(4)) {
            a[i] = i * i
        }
        t |> equal(a[0], 0)
        t |> equal(a[2], 4)
        t |> equal(a[3], 9)
    }
    t |> run("loop induction over fixed array") @(t : T?) {
        var a : int[8]
        for (i in range(8)) {
            a[i] = i
        }
        var s = 0
        for (i in range(0, 8)) {
            s += a[i]
        }
        t |> equal(s, 28)
    }
    t |> run("constant index into vector") @(t : T?) {
        let v = float4(1.0, 2.0, 3.0, 4.0)
        t |> equal(v[0], 1.0)
        t |> equal(v[3], 4.0)
    }
    t |> run("partial range still correct") @(t : T?) {
        var a : int[8]
        for (i in range(2, 6)) {
            a[i] = i
        }
        t |> equal(a[1], 0)
        t |> equal(a[5], 5)
        t |> equal(a[6], 0)
    }
    t |> run("dynamic array over its own length") @(t : T?) {
        var x : array<int>
        x |> resize(5)
        for (i in range(x |> length)) {
            x[i] = i * 2
        }
        var s = 0
        for (i in range(0, length(x))) {
            s += x[i]
        }
        t |> equal(x[4], 8)
        t |> equal(s, 0 + 2 + 4 + 6 + 8)
    }
    t |> run("shrinking loop still correct") @(t : T?) {
        // erase inside the loop must NOT be miscompiled to unchecked access
        var x : array<int>
        x |> resize(6)
        for (i in range(x |> length)) {
            if (i < length(x)) {
                x[i] = i
            }
        }
        t |> equal(x[0], 0)
    }
    t |> run("guard: early-exit dominates the access") @(t : T?) {
        var a : array<int>
        a |> resize(4)
        guard_early_exit(a, 2u)
        t |> equal(a[2], 20)
        guard_early_exit(a, 100u)   // out of range: guarded return, no write, no crash
        t |> equal(a[3], 0)
    }
    t |> run("guard: then-branch dominates the access") @(t : T?) {
        var a : array<int>
        a |> resize(4)
        guard_then(a, 3u)
        t |> equal(a[3], 30)
        guard_then(a, 100u)
        t |> equal(a[0], 0)
    }
    t |> run("aliasing: mutating another array stays correct") @(t : T?) {
        // b and a may alias; erasing b must not enable unchecked a[i]
        var a : array<int>
        var b : array<int>
        a |> resize(5)
        b |> resize(5)
        for (i in range(a |> length)) {
            if (length(b) > 0) {
                b |> erase(0)
            }
            if (i < length(a)) {
                a[i] = i
            }
        }
        t |> equal(a[0], 0)
    }
}
