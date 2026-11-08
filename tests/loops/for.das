options gen2

require dastest/testing_boost public

// Per-iteration finally on for-over-dynamic-array loops.

[test(description = "a for loop over a dynamic array runs its finally block per iteration through plain, break, continue, return, nested, inscope and empty cases")]
def test_for_array(t : T?) {
    t |> run("plain") @(t : T?) {
        var arr <- [10, 20, 30]
        var count = 0
        for (_x in arr) {
        } finally {
            count ++
        }
        t |> equal(count, 3)
    }
    t |> run("break") @(t : T?) {
        var arr <- [1, 2, 3, 4, 5]
        var count = 0
        for (x in arr) {
            if (x == 3) {
                break
            }
        } finally {
            count ++
        }
        // iterations for 1,2,3 each fire finally (break runs after x==3's finally)
        t |> equal(count, 3)
    }
    t |> run("continue") @(t : T?) {
        var arr <- [1, 2, 3, 4]
        var count = 0
        var hits = 0
        for (x in arr) {
            if (x % 2 == 0) {
                continue
            }
            hits ++
        } finally {
            count ++
        }
        t |> equal(count, 4)
        t |> equal(hits, 2)
    }
    t |> run("return") @(t : T?) {
        // return captures count=1 (after iter x=10's finally); iter x=20's finally runs after return expr is evaluated
        t |> equal(for_array_return(), 1)
    }
    t |> run("nested") @(t : T?) {
        var a <- [1, 2, 3]
        var b <- [10, 20]
        var outer = 0
        var inner = 0
        for (_y in a) {
            for (_x in b) {
            } finally {
                inner ++
            }
        } finally {
            outer ++
        }
        t |> equal(outer, 3)
        t |> equal(inner, 6)
    }
    t |> run("inscope") @(t : T?) {
        var src <- [1, 2, 3]
        for (i in src) {
            var inscope tmp : array<int>
            tmp |> push(i); tmp |> push(i * 2)
            t |> equal(tmp[0], i)
            t |> equal(tmp[1], i * 2)
        }
    }
    t |> run("empty") @(t : T?) {
        var arr : array<int>
        var count = 0
        for (_x in arr) {
        } finally {
            count ++
        }
        // empty array: body never enters, finally never runs
        t |> equal(count, 0)
    }
}

def for_array_return() : int {
    var arr <- [10, 20, 30]
    var count = 0
    for (x in arr) {
        if (x == 20) {
            return count
        }
    } finally {
        count ++
    }
    return -1
}

// Empty-body for loops. Codegen must not mis-select the fused single-statement
// for-node (a dead single statement is 1 AST node but 0 SimNodes), and an empty
// body over a SIDE-EFFECTING source must still evaluate the source — the empty-loop
// elision is gated on side-effect-free sources only.

var g_src_calls = 0

def counting_source : array<int> {
    g_src_calls++
    return [1, 2, 3]
}

[test(description = "a for loop with an empty body still iterates its source and still evaluates a side-effecting source, while a pure empty loop is a no-op")]
def test_for_empty_body(t : T?) {
    t |> run("empty body over array still iterates the source") @(t : T?) {
        var iters = 0
        for (_x in [1, 2, 3]) {
        } finally {
            iters++
        }
        t |> equal(iters, 3)
    }
    t |> run("empty body over range still iterates the source") @(t : T?) {
        var iters = 0
        for (_i in range(5)) {
        } finally {
            iters++
        }
        t |> equal(iters, 5)
    }
    t |> run("empty body over fixed array still iterates the source") @(t : T?) {
        let fa = fixed_array(7, 8, 9)
        var iters = 0
        for (_x in fa) {
        } finally {
            iters++
        }
        t |> equal(iters, 3)
    }
    t |> run("pure empty loop is a no-op") @(t : T?) {
        for (_x in [1, 2, 3]) {
        }
        t |> success(true)
    }
    t |> run("side-effecting source is NOT elided") @(t : T?) {
        g_src_calls = 0
        for (_x in counting_source()) {
        }
        t |> equal(g_src_calls, 1)
    }
}

// Per-iteration finally on for-over-fixed-array (dim) loops.

[test(description = "a for loop over a fixed array runs its finally block per iteration through plain, break, continue, return and inscope cases")]
def test_for_fixed_array(t : T?) {
    t |> run("plain") @(t : T?) {
        var arr : int[5]
        arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50
        var count = 0
        for (_x in arr) {
        } finally {
            count ++
        }
        t |> equal(count, 5)
    }
    t |> run("break") @(t : T?) {
        var arr : int[5]
        arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5
        var count = 0
        for (x in arr) {
            if (x == 3) {
                break
            }
        } finally {
            count ++
        }
        t |> equal(count, 3)
    }
    t |> run("continue") @(t : T?) {
        var arr : int[4]
        arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4
        var count = 0
        var hits = 0
        for (x in arr) {
            if (x % 2 == 0) {
                continue
            }
            hits ++
        } finally {
            count ++
        }
        t |> equal(count, 4)
        t |> equal(hits, 2)
    }
    t |> run("return") @(t : T?) {
        t |> equal(for_fixed_return(), 1)
    }
    t |> run("inscope") @(t : T?) {
        var src : int[3]
        src[0] = 7; src[1] = 8; src[2] = 9
        for (i in src) {
            var inscope tmp : array<int>
            tmp |> push(i)
            t |> equal(tmp[0], i)
        }
    }
}

def for_fixed_return() : int {
    var arr : int[3]
    arr[0] = 10; arr[1] = 20; arr[2] = 30
    var count = 0
    for (x in arr) {
        if (x == 20) {
            return count
        }
    } finally {
        count ++
    }
    return -1
}

// Per-iteration finally on for-over-iterator loops (SimNode_ForWithIterator path).

def make_iter(n : int) : iterator<int> {
    return <- generator<int> {
        for (i in range(n)) {
            yield i
        }
        return false
    }
}

[test(description = "a for loop over an iterator runs its finally block per iteration through plain, break, continue, return, inscope and empty cases")]
def test_for_iterator(t : T?) {
    t |> run("plain") @(t : T?) {
        var count = 0
        for (_x in make_iter(5)) {
        } finally {
            count ++
        }
        t |> equal(count, 5)
    }
    t |> run("break") @(t : T?) {
        var count = 0
        for (x in make_iter(10)) {
            if (x == 4) {
                break
            }
        } finally {
            count ++
        }
        // iterations for x=0..4 each fire finally (break runs after x==4's finally)
        t |> equal(count, 5)
    }
    t |> run("continue") @(t : T?) {
        var count = 0
        var hits = 0
        for (x in make_iter(6)) {
            if (x % 2 == 0) {
                continue
            }
            hits ++
        } finally {
            count ++
        }
        t |> equal(count, 6)
        t |> equal(hits, 3)
    }
    t |> run("return") @(t : T?) {
        t |> equal(for_iterator_return(), 2)
    }
    t |> run("inscope") @(t : T?) {
        for (i in make_iter(4)) {
            var inscope tmp : array<int>
            tmp |> push(i); tmp |> push(i * 10)
            t |> equal(tmp[0], i)
            t |> equal(tmp[1], i * 10)
        }
    }
    t |> run("empty") @(t : T?) {
        var count = 0
        for (_x in make_iter(0)) {
        } finally {
            count ++
        }
        t |> equal(count, 0)
    }
}

def for_iterator_return() : int {
    var count = 0
    for (x in make_iter(5)) {
        if (x == 2) {
            return count
        }
    } finally {
        count ++
    }
    return -1
}

// A for-loop whose body simulates to exactly ONE SimNode plus a label (a goto target).
// The fused single-statement (`loop1`) for-node reads list[0] directly and cannot service
// jumpToLabel, so node selection must follow the simulated body count AND require no labels:
// a label makes total==1 but must still take the general labeled node, or the in-loop goto
// leaks out (returns garbage) instead of jumping back to the label.

def loop_back_to_label : int {
    var counter = 0
    var guard = 0
    for (_i in range(2)) {
        label 0:
        if (guard < 3) {
            guard ++
            counter += 10
            goto 0
        }
    }
    return counter
}

[test(description = "a goto to a label inside a single-statement for body jumps back within the loop instead of leaving it")]
def test_for_label_body(t : T?) {
    t |> run("in-loop goto to a label in a single-statement body jumps back, not out") @(t : T?) {
        t |> equal(loop_back_to_label(), 30)
    }
}

// Per-iteration finally on for-over-multi-source loops: `for (a, b in src1, src2)`.

[test(description = "a for loop over several sources runs its finally block per iteration through plain, break, continue, return, inscope, empty and uneven-length cases")]
def test_for_multi_source(t : T?) {
    t |> run("plain") @(t : T?) {
        var count = 0
        var sum = 0
        for (a, b in range(4), range(10, 14)) {
            sum += a + b
        } finally {
            count ++
        }
        t |> equal(count, 4)
        t |> equal(sum, (0 + 1 + 2 + 3) + (10 + 11 + 12 + 13))
    }
    t |> run("break") @(t : T?) {
        var count = 0
        for (a, _b in range(10), range(100, 110)) {
            if (a == 3) {
                break
            }
        } finally {
            count ++
        }
        // iterations for a=0..3 each fire finally (break runs after a==3's finally)
        t |> equal(count, 4)
    }
    t |> run("continue") @(t : T?) {
        var count = 0
        var hits = 0
        for (a, _b in range(4), range(10, 14)) {
            if (a % 2 == 0) {
                continue
            }
            hits ++
        } finally {
            count ++
        }
        t |> equal(count, 4)
        t |> equal(hits, 2)
    }
    t |> run("return") @(t : T?) {
        t |> equal(for_multi_return(), 2)
    }
    t |> run("inscope") @(t : T?) {
        for (a, b in range(3), range(100, 103)) {
            var inscope tmp : array<int>
            tmp |> push(a); tmp |> push(b)
            t |> equal(tmp[0], a)
            t |> equal(tmp[1], b)
        }
    }
    t |> run("empty") @(t : T?) {
        var count = 0
        // shortest source is empty -> loop body never enters
        for (_a, _b in range(0), range(10, 15)) {
        } finally {
            count ++
        }
        t |> equal(count, 0)
    }
    t |> run("uneven sources") @(t : T?) {
        var count = 0
        // shortest source (length 3) determines iteration count
        for (_a, _b in range(10), range(100, 103)) {
        } finally {
            count ++
        }
        t |> equal(count, 3)
    }
}

def for_multi_return() : int {
    var count = 0
    for (a, _b in range(10), range(100, 110)) {
        if (a == 2) {
            return count
        }
    } finally {
        count ++
    }
    return -1
}

// Per-iteration finally on for-over-range loops.

[test(description = "a for loop over a range runs its finally block per iteration through plain, break, continue, return, nested, inscope and empty cases")]
def test_for_range(t : T?) {
    t |> run("plain") @(t : T?) {
        var count = 0
        for (_x in range(10)) {
        } finally {
            count ++
        }
        t |> equal(count, 10)
    }
    t |> run("break") @(t : T?) {
        var count = 0
        for (x in range(10)) {
            if (x == 5) {
                break
            }
        } finally {
            count ++
        }
        // iterations 0..5 each fire finally (break after x==5's finally)
        t |> equal(count, 6)
    }
    t |> run("continue") @(t : T?) {
        var count = 0
        var hits = 0
        for (x in range(10)) {
            if (x % 2 == 0) {
                continue
            }
            hits ++
        } finally {
            count ++
        }
        t |> equal(count, 10)
        t |> equal(hits, 5)
    }
    t |> run("return") @(t : T?) {
        t |> equal(for_range_return(), 3)
    }
    t |> run("nested") @(t : T?) {
        var outer = 0
        var inner = 0
        for (_y in range(4)) {
            for (_x in range(3)) {
            } finally {
                inner ++
            }
        } finally {
            outer ++
        }
        t |> equal(outer, 4)
        t |> equal(inner, 12)
    }
    t |> run("inscope") @(t : T?) {
        for (i in range(5)) {
            var inscope arr : array<int>
            arr |> push(i)
            t |> equal(arr[0], i)
        }
    }
    t |> run("empty") @(t : T?) {
        var count = 0
        for (_x in range(0)) {
        } finally {
            count ++
        }
        // empty iterator: body never enters, finally never runs
        t |> equal(count, 0)
    }
}

def for_range_return() : int {
    var count = 0
    for (x in range(10)) {
        if (x == 3) {
            return count
        }
    } finally {
        count ++
    }
    return -1
}
