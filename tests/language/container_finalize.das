options gen2
options persistent_heap   // heap_bytes_allocated only tracks individual frees on the persistent heap
require dastest/testing_boost public

// Two independent signals per operation: the finalizer ran (counted), and the bytes came
// back (heap delta). The carve-out cases assert the opposite of both.

var g_finalized = 0

struct Probe {
    xs : array<int>
}

def finalize(var p : Probe) {
    g_finalized++
    delete p.xs
}

struct Holder {
    v : int
}

def private make_probes(n : int) : array<Probe> {
    var arr : array<Probe>
    arr |> resize(n)
    for (i in range(n)) {
        arr[i].xs |> reserve(1024)
        arr[i].xs |> push(i * 10)
    }
    return <- arr
}

[test]
def test_erase_finalizes(t : T?) {
    t |> run("erase finalizes the removed element") @(t : T?) {
        var inscope arr <- make_probes(3)
        g_finalized = 0
        let before = heap_bytes_allocated()
        arr |> erase(0)
        let after = heap_bytes_allocated()
        t |> equal(g_finalized, 1)
        t |> success(after < before, "the removed element's buffer came back")
        t |> equal(length(arr), 2)
        t |> equal(arr[0].xs[0], 10)
        t |> equal(arr[1].xs[0], 20)
    }
    t |> run("erase_range finalizes each removed element") @(t : T?) {
        var inscope arr <- make_probes(4)
        g_finalized = 0
        let before = heap_bytes_allocated()
        arr |> erase(1, 2)
        let after = heap_bytes_allocated()
        t |> equal(g_finalized, 2)
        t |> success(after < before, "both removed buffers came back")
        t |> equal(length(arr), 2)
        t |> equal(arr[1].xs[0], 30)
    }
    t |> run("clear finalizes every element") @(t : T?) {
        var inscope arr <- make_probes(2)
        g_finalized = 0
        let before = heap_bytes_allocated()
        arr |> clear()
        let after = heap_bytes_allocated()
        t |> equal(g_finalized, 2)
        t |> success(after < before, "every element's buffer came back")
        t |> equal(length(arr), 0)
    }
    t |> run("shrinking resize finalizes the dropped tail") @(t : T?) {
        var inscope arr <- make_probes(3)
        g_finalized = 0
        let before = heap_bytes_allocated()
        arr |> resize(1)
        let after = heap_bytes_allocated()
        t |> equal(g_finalized, 2)
        t |> success(after < before, "the dropped tail's buffers came back")
        t |> equal(arr[0].xs[0], 0)
    }
    t |> run("pop finalizes the popped element") @(t : T?) {
        var inscope arr <- make_probes(2)
        g_finalized = 0
        let before = heap_bytes_allocated()
        arr |> pop()
        let after = heap_bytes_allocated()
        t |> equal(g_finalized, 1)
        t |> success(after < before, "the popped element's buffer came back")
    }
    t |> run("plain nested containers release too, with no user finalizer") @(t : T?) {
        var inscope arr : array<array<int>>
        arr |> resize(2)
        arr[0] |> reserve(1024)
        arr[0] |> push(1)
        arr[1] |> reserve(1024)
        arr[1] |> push(2)
        let before = heap_bytes_allocated()
        arr |> clear()
        let after = heap_bytes_allocated()
        t |> success(after < before, "clear released the inner buffers")
    }
}

[test]
def test_pointer_carveout(t : T?) {
    t |> run("borrowed array pointers survive erase and clear") @(t : T?) {
        var p1 = new Holder(v = 41)
        var p2 = new Holder(v = 42)
        var arr : array<Holder?>
        arr |> push(p1)
        arr |> push(p2)
        arr |> erase(0)
        t |> equal(p1.v, 41)
        arr |> clear()
        t |> equal(p2.v, 42)
        // still ours to free - a double-free here would panic if erase/clear owned them
        unsafe {
            delete p1
        }
        unsafe {
            delete p2
        }
        unsafe {
            delete arr    // already cleared - buffer only, the borrowed-pointer idiom
        }
    }
    t |> run("borrowed struct-pointer elements skip the finalizer") @(t : T?) {
        var q = new Probe()
        q.xs |> push(9)
        var arr : array<Probe?>
        arr |> push(q)
        g_finalized = 0
        arr |> erase(0)
        arr |> push(q)
        arr |> clear()
        t |> equal(g_finalized, 0)
        t |> equal(q.xs[0], 9)
        unsafe {
            delete q      // runs the finalizer once, on our terms
        }
        t |> equal(g_finalized, 1)
        unsafe {
            delete arr
        }
    }
    t |> run("borrowed table value pointers survive erase and clear") @(t : T?) {
        var q1 = new Holder(v = 51)
        var q2 = new Holder(v = 52)
        var tab : table<string; Holder?>
        tab["a"] = q1
        tab["b"] = q2
        tab |> erase("a")
        t |> equal(q1.v, 51)
        tab |> clear()
        t |> equal(q2.v, 52)
        unsafe {
            delete q1
        }
        unsafe {
            delete q2
        }
        unsafe {
            delete tab    // already cleared - buffer only
        }
    }
}

[test]
def test_composite_carveout(t : T?) {
    t |> run("pointer-carrying composites skip the finalizer") @(t : T?) {
        // the linq-planner shape: tuple-of-pointers elements alias live data
        var w1 = new Holder(v = 61)
        var w2 = new Holder(v = 62)
        var arr : array<tuple<Holder?; Holder?>>
        arr |> push((w1, w2))
        arr |> erase(0)
        t |> equal(w1.v, 61)
        t |> equal(w2.v, 62)
        unsafe {
            delete w1
        }
        unsafe {
            delete w2
        }
        unsafe {
            delete arr
        }
    }
}

[test]
def test_table_erase_finalizes(t : T?) {
    t |> run("table erase finalizes the dropped value") @(t : T?) {
        var inscope tab : table<string; Probe>
        tab["a"].xs |> reserve(1024)
        tab["a"].xs |> push(1)
        tab["b"].xs |> push(2)
        g_finalized = 0
        let before = heap_bytes_allocated()
        tab |> erase("a")
        let after = heap_bytes_allocated()
        t |> equal(g_finalized, 1)
        t |> success(after < before, "the dropped value's buffer came back")
        t |> equal(tab["b"].xs[0], 2)
    }
    t |> run("erase of a missing key finalizes nothing") @(t : T?) {
        var inscope tab : table<string; Probe>
        tab["a"].xs |> push(1)
        g_finalized = 0
        let erased = tab |> erase("zzz")
        t |> success(!erased, "missing key reports false")
        t |> equal(g_finalized, 0)
    }
    t |> run("table clear finalizes every value") @(t : T?) {
        var inscope tab : table<string; Probe>
        tab["a"].xs |> reserve(1024)
        tab["a"].xs |> push(1)
        tab["b"].xs |> reserve(1024)
        tab["b"].xs |> push(2)
        g_finalized = 0
        let before = heap_bytes_allocated()
        tab |> clear()
        let after = heap_bytes_allocated()
        t |> equal(g_finalized, 2)
        t |> success(after < before, "every value's buffer came back")
        t |> equal(length(tab), 0)
    }
}
