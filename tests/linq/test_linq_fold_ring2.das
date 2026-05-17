options gen2
require daslib/linq
require dastest/testing_boost public

require daslib/linq_boost

// Phase 2B Ring 2 — early-exit lane (first / first_or_default / any / all / contains).
// Lives in its own file so the iterator-type generic instances `linq.all` / `linq.contains`
// don't collide with the ones registered by `test_any_all_contains` in test_linq_fold.das.
// `first` on empty source panics (matches linq.das line 2383); not tested directly here
// since dastest lacks a verify_panic helper. Non-empty cases cover the main paths.

[test]
def test_first_early_exit(t : T?) {
    t |> run("first: singleton") @(t : T?) {
        let arr <- [42]
        let f = _fold(each(arr).first())
        t |> equal(42, f)
    }
    t |> run("first: many returns first") @(t : T?) {
        let arr <- [7, 8, 9]
        let f = _fold(each(arr).first())
        t |> equal(7, f)
        let f_old = _old_fold(each(arr).first())
        t |> equal(f_old, f)
    }
    t |> run("first: where matches returns first match") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let f = _fold(each(arr)._where(_ > 2).first())
        t |> equal(3, f)
    }
    t |> run("first: select returns projected first") @(t : T?) {
        let arr <- [1, 2, 3]
        let f = _fold(each(arr)._select(_ * 10).first())
        t |> equal(10, f)
    }
}

[test]
def test_first_or_default_early_exit(t : T?) {
    t |> run("first_or_default: empty returns default") @(t : T?) {
        let arr : array<int>
        let f = _fold(each(arr).first_or_default(99))
        t |> equal(99, f)
    }
    t |> run("first_or_default: non-empty returns first") @(t : T?) {
        let arr <- [7, 8, 9]
        let f = _fold(each(arr).first_or_default(99))
        t |> equal(7, f)
        let f_old = _old_fold(each(arr).first_or_default(99))
        t |> equal(f_old, f)
    }
    t |> run("first_or_default: where matches returns first match") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let f = _fold(each(arr)._where(_ > 3).first_or_default(0))
        t |> equal(4, f)
    }
    t |> run("first_or_default: where no match returns default") @(t : T?) {
        let arr <- [1, 2, 3]
        let f = _fold(each(arr)._where(_ > 100).first_or_default(-1))
        t |> equal(-1, f)
    }
}

[test]
def test_any_early_exit(t : T?) {
    t |> run("any: empty → false") @(t : T?) {
        let arr : array<int>
        let r = _fold(each(arr).any())
        t |> equal(false, r)
    }
    t |> run("any: non-empty (no pred) → true (shortcut path)") @(t : T?) {
        let arr <- [1]
        let r = _fold(each(arr).any())
        t |> equal(true, r)
    }
    t |> run("any: with where matching → true") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr)._where(_ > 3).any())
        t |> equal(true, r)
    }
    t |> run("any: with where not matching → false") @(t : T?) {
        let arr <- [1, 2, 3]
        let r = _fold(each(arr)._where(_ > 100).any())
        t |> equal(false, r)
    }
    t |> run("any: with pred matching → true") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr)._any(_ > 3))
        t |> equal(true, r)
    }
}

[test]
def test_all_early_exit(t : T?) {
    t |> run("all: empty → true (vacuous)") @(t : T?) {
        let arr : array<int>
        let r = _fold(each(arr)._all(_ > 0))
        t |> equal(true, r)
    }
    t |> run("all: all match → true") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr)._all(_ > 0))
        t |> equal(true, r)
    }
    t |> run("all: one fails → false") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr)._all(_ > 2))
        t |> equal(false, r)
    }
    t |> run("all: with upstream where") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        // Only elements > 2 pass: 3, 4, 5. All of those > 0 → true.
        let r = _fold(each(arr)._where(_ > 2)._all(_ > 0))
        t |> equal(true, r)
    }
}

[test]
def test_contains_early_exit(t : T?) {
    t |> run("contains: empty → false") @(t : T?) {
        let arr : array<int>
        let r = _fold(each(arr).contains(3))
        t |> equal(false, r)
    }
    t |> run("contains: found → true") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr).contains(3))
        t |> equal(true, r)
    }
    t |> run("contains: not found → false") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr).contains(99))
        t |> equal(false, r)
    }
    t |> run("contains: select then compare against projected") @(t : T?) {
        let arr <- [1, 2, 3]
        // Projected values: [10, 20, 30]. contains(20) → true.
        let r = _fold(each(arr)._select(_ * 10).contains(20))
        t |> equal(true, r)
    }
}
