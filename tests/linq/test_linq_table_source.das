options gen2

require dastest/testing_boost public
require daslib/linq_boost
require strings

// Table source (each_kv / keys / values) through _fold — fused TableAdapter lanes must agree with
// hand loops over the same table. Slot order is unspecified but stable per table instance, so
// order-sensitive expectations compare against a keys()/values() walk of the same table.

struct Pt {
    x : int
    y : int
}

def make_int_table(n : int) : table<int; int> {
    var t <- { for (i in range(n)); i => i * 10 }
    return <- t
}

def make_pt_table : table<string; Pt> {
    var t : table<string; Pt>
    t |> insert("a", Pt(x = 1, y = 10))
    t |> insert("b", Pt(x = 2, y = 20))
    t |> insert("c", Pt(x = 3, y = 30))
    t |> insert("d", Pt(x = 4, y = 40))
    return <- t
}

[test]
def test_table_fold_count_shortcuts(t : T?) {
    t |> run("bare count is length") @(t : T?) {
        var tab <- make_int_table(10)
        t |> equal(_fold(each_kv(tab).count()), 10)
        t |> equal(_fold(keys(tab).count()), 10)
        t |> equal(_fold(values(tab).count()), 10)
        t |> equal(_fold(each_kv(tab).long_count()), 10l)
        delete tab
    }
    t |> run("count with predicate walks") @(t : T?) {
        var tab <- make_int_table(10)
        t |> equal(_fold(each_kv(tab)._where(_.key % 2 == 0).count()), 5)
        t |> equal(_fold(each_kv(tab)._where(_.value > 50).count()), 4)
        delete tab
    }
    t |> run("empty table") @(t : T?) {
        let e : table<int; int>
        t |> equal(_fold(each_kv(e).count()), 0)
        t |> equal(_fold(each_kv(e).any()), false)
        t |> equal(_fold(each_kv(e)._where(_.key > 0).count()), 0)
    }
}

[test]
def test_table_fold_accumulators(t : T?) {
    t |> run("sum/min/max over values and keys") @(t : T?) {
        var tab <- make_int_table(10)
        t |> equal(_fold(values(tab).sum()), 450)
        t |> equal(_fold(keys(tab).sum()), 45)
        t |> equal(_fold(each_kv(tab)._select(_.value).sum()), 450)
        t |> equal(_fold(each_kv(tab)._select(_.key).min()), 0)
        t |> equal(_fold(each_kv(tab)._select(_.value).max()), 90)
        // body touches both sides — zipped walk
        t |> equal(_fold(each_kv(tab)._select(_.key + _.value).sum()), 495)
        delete tab
    }
    t |> run("early exit: any/all/contains") @(t : T?) {
        var tab <- make_int_table(10)
        t |> equal(_fold(each_kv(tab).any()), true)
        t |> equal(_fold(each_kv(tab)._where(_.value > 80).any()), true)
        t |> equal(_fold(each_kv(tab)._where(_.value > 90).any()), false)
        t |> equal(_fold(each_kv(tab)._select(_.key)._all(_ >= 0)), true)
        t |> equal(_fold(values(tab).contains(40)), true)
        t |> equal(_fold(values(tab).contains(41)), false)
        delete tab
    }
}

[test]
def test_table_fold_to_array_agreement(t : T?) {
    t |> run("kv where+select agrees with hand loop, in slot order") @(t : T?) {
        var tab <- make_pt_table()
        var expected : array<int>
        for (_k, v in keys(tab), values(tab)) {
            if (v.x > 1) {
                expected |> push(v.y)
            }
        }
        var got <- _fold(each_kv(tab)._where(_.value.x > 1)._select(_.value.y).to_array())
        t |> equal(length(got), length(expected))
        for (i in range(length(expected))) {
            t |> equal(got[i], expected[i])
        }
        delete got
        delete expected
        delete tab
    }
    t |> run("keys to_array in slot order") @(t : T?) {
        var tab <- make_pt_table()
        var expected : array<string>
        for (k in keys(tab)) {
            expected |> push(k)
        }
        // bare `<src>.to_array()` is not a recognized chain (any source) — keep a where on it
        var got <- _fold(keys(tab)._where(_ != "zzz").to_array())
        t |> equal(length(got), length(expected))
        for (i in range(length(expected))) {
            t |> equal(got[i], expected[i])
        }
        delete got
        delete expected
        delete tab
    }
    t |> run("whole-kv escape: identity to_array") @(t : T?) {
        var tab <- make_pt_table()
        var got <- _fold(each_kv(tab)._where(_.value.x >= 3).to_array())
        t |> equal(length(got), 2)
        for (kv in got) {
            let expectedPt = tab?[kv.key] ?? Pt()
            t |> equal(expectedPt.x, kv.value.x)
            t |> equal(expectedPt.y, kv.value.y)
        }
        delete got
        delete tab
    }
}

[test]
def test_table_fold_order_distinct_take(t : T?) {
    t |> run("order_by key descending") @(t : T?) {
        var tab <- make_int_table(10)
        var got <- _fold(each_kv(tab)._select(_.key).order_by_descending(@(k : int) => k).to_array())
        t |> equal(length(got), 10)
        for (i in range(10)) {
            t |> equal(got[i], 9 - i)
        }
        delete got
        delete tab
    }
    t |> run("redundant distinct over keys/kv is dropped but correct") @(t : T?) {
        var tab <- make_int_table(10)
        t |> equal(_fold(each_kv(tab).distinct().count()), 10)
        t |> equal(_fold(each_kv(tab)._where(_.key > 4).distinct().count()), 5)
        t |> equal(_fold(keys(tab).distinct().count()), 10)
        delete tab
    }
    t |> run("values distinct stays real") @(t : T?) {
        var dup : table<int; int>
        dup |> insert(1, 7)
        dup |> insert(2, 7)
        dup |> insert(3, 8)
        t |> equal(_fold(values(dup).distinct().count()), 2)
        t |> equal(_fold(each_kv(dup)._select(_.value).distinct().count()), 2)
        delete dup
    }
    t |> run("take/skip ride the walk") @(t : T?) {
        var tab <- make_int_table(10)
        t |> equal(_fold(each_kv(tab)._select(_.value).take(3).count()), 3)
        t |> equal(_fold(each_kv(tab)._select(_.key).skip(4).count()), 6)
        var firstTwo <- _fold(keys(tab).take(2).to_array())
        var expected : array<int>
        for (k in keys(tab)) {
            if (length(expected) < 2) {
                expected |> push(k)
            }
        }
        t |> equal(length(firstTwo), 2)
        t |> equal(firstTwo[0], expected[0])
        t |> equal(firstTwo[1], expected[1])
        delete firstTwo
        delete expected
        delete tab
    }
    t |> run("first / first_or_default") @(t : T?) {
        var tab <- make_int_table(10)
        t |> equal(_fold(each_kv(tab)._where(_.key == 7)._select(_.value).first()), 70)
        t |> equal(_fold(each_kv(tab)._where(_.key == 99)._select(_.value).first_or_default(-1)), -1)
        delete tab
    }
}

[test]
def test_table_fold_iterator_result(t : T?) {
    t |> run("chain consumed as iterator") @(t : T?) {
        var tab <- make_int_table(6)
        var s = 0
        for (v in _fold(each_kv(tab)._where(_.key % 2 == 0)._select(_.value))) {
            s += v
        }
        t |> equal(s, 0 + 20 + 40)
        delete tab
    }
}

[test]
def test_table_fold_set_form(t : T?) {
    t |> run("keys over table<K> set") @(t : T?) {
        var s : table<string>
        s |> insert("alpha")
        s |> insert("beta")
        s |> insert("gamma")
        t |> equal(_fold(keys(s).count()), 3)
        t |> equal(_fold(keys(s)._where(_ |> length() > 4).count()), 2)
        delete s
    }
}

def private bump_key(var c : int&) : int {
    c++
    return 2
}

// Point-lookup folds: `where(kv.key == X)` + terminator → O(1) key probe (key_exists / `tab?[X]`).
// Probes must agree with the scan on hit AND miss; non-probe shapes must keep riding the scan.

[test]
def test_table_point_lookup(t : T?) {
    t |> run("kv any/count/first probes, hit and miss") @(t : T?) {
        var tab <- make_int_table(10)
        let k = 7
        t |> equal(_fold(each_kv(tab)._where(_.key == k).any()), true)
        t |> equal(_fold(each_kv(tab)._where(_.key == 99).any()), false)
        t |> equal(_fold(each_kv(tab)._any(_.key == k)), true)
        t |> equal(_fold(each_kv(tab)._where(_.key == k).count()), 1)
        t |> equal(_fold(each_kv(tab)._where(_.key == 99).count()), 0)
        t |> equal(_fold(each_kv(tab)._count(_.key == 3)), 1)
        let f = _fold(each_kv(tab)._where(k == _.key).first())   // flipped operand side
        t |> equal(f.key, 7)
        t |> equal(f.value, 70)
        let m = _fold(each_kv(tab)._where(_.key == 99).first_or_default(default<tuple<key : int; value : int>>))
        t |> equal(m.key, 0)
        delete tab
    }
    t |> run("probe + trailing select projects the probed element") @(t : T?) {
        var tab <- make_int_table(10)
        t |> equal(_fold(each_kv(tab)._where(_.key == 4)._select(_.value).first()), 40)
        t |> equal(_fold(each_kv(tab)._where(_.key == 99)._select(_.value).first_or_default(-1)), -1)
        t |> equal(_fold(each_kv(tab)._where(_.key == 4)._select("{_.key}:{_.value}").first()), "4:40")
        delete tab
    }
    t |> run("keys lane probes + set form contains") @(t : T?) {
        var tab <- make_int_table(10)
        t |> equal(_fold(keys(tab)._where(_ == 5).any()), true)
        t |> equal(_fold(keys(tab)._where(_ == 5).first()), 5)
        t |> equal(_fold(keys(tab)._where(_ == 5)._select(_ * 100).first()), 500)
        t |> equal(_fold(keys(tab).contains(5)), true)
        t |> equal(_fold(keys(tab).contains(55)), false)
        delete tab
        var s : table<string> <- { "x", "y" }
        t |> equal(_fold(keys(s).contains("y")), true)
        t |> equal(_fold(keys(s).contains("z")), false)
        delete s
    }
    t |> run("first probe panics on a missing key, like the scan") @(t : T?) {
        var tab <- make_int_table(4)
        var panicked = false
        try {
            let _r = _fold(each_kv(tab)._where(_.key == 99).first())
        } recover {
            panicked = true
        }
        t |> equal(panicked, true)
        delete tab
    }
    t |> run("non-probe shapes stay scans and stay correct") @(t : T?) {
        var tab <- make_int_table(10)
        t |> equal(_fold(each_kv(tab)._where(_.key != 5).count()), 9)                      // wrong operator
        t |> equal(_fold(each_kv(tab)._where(_.key == _.value / 10).count()), 10)          // X references the binder
        t |> equal(_fold(each_kv(tab)._where(_.key == 5)._where(_.value > 0).any()), true) // collapses to a compound && predicate
        delete tab
    }
    t |> run("impure X stays a scan — per-element evaluation preserved") @(t : T?) {
        var tab <- make_int_table(4)
        var evals = 0
        t |> equal(_fold(each_kv(tab)._where(_.key == bump_key(evals)).count()), 1)
        t |> equal(evals, 4, "side-effectful X must evaluate per element, not once")
        delete tab
    }
}

// Tier-2 over the raw each_kv iterator (no _fold) — the [unsafe_outside_of_for] contract requires the
// explicit unsafe(...) wrap at a bare chain head; fused chains rewrite the head before inference.

[test]
def test_each_kv_tier2(t : T?) {
    t |> run("where/select/to_array over each_kv") @(t : T?) {
        var tab : table<string; int>
        tab |> insert("a", 1)
        tab |> insert("b", 2)
        tab |> insert("c", 3)
        var vals <- unsafe(each_kv(tab)) |> _where(_.value > 1) |> _select(_.value) |> to_array()
        vals |> sort()  // slot order is unspecified
        t |> equal(length(vals), 2)
        t |> equal(vals[0], 2)
        t |> equal(vals[1], 3)
        delete vals
    }
    t |> run("keys participate in the chain") @(t : T?) {
        var tab : table<int; int>
        for (i in range(6)) {
            tab |> insert(i, i * i)
        }
        let n = unsafe(each_kv(tab)) |> _where(_.key % 2 == 0) |> count()
        t |> equal(n, 3)
    }
}
