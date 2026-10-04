//! The suites' shared compares (`_compares.das`) against hand-built rows: a NaN or a length difference is never inside a bar, each `check` bar kind passes its rows and reds one past it, and the poisoned-expectation control reds a bar too loose to see it. No skips.
options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require _compares
require daslib/math_bits
require daslib/rtti   // LineInfo: the probe handle's log hook
require math

let INF = uint_bits_to_float(0x7f800000u)

[test]
def test_maxdiff_sees_nan(t : T?) {
    t |> run("a NaN on either side is the largest difference, wherever it sits") @(t : T?) {
        let ones <- [1.0, 1.0, 1.0, 1.0]
        for (at in range(4)) {
            var row <- [1.0, 1.0, 1.0, 1.0]
            row[at] = NAN_SENTINEL
            t |> equal(logits_maxdiff(row, ones), INF, "a NaN in got at {at}")
            t |> equal(logits_maxdiff(ones, row), INF, "a NaN in want at {at}")
            t |> equal(maxdiff_at(row, ones), at, "maxdiff_at lands on the NaN at {at}")
            t |> equal(logits_maxabs(row), INF, "a NaN scales no relative bar, at {at}")
        }
        t |> equal(logits_maxdiff(ones, ones), 0.0, "control: equal rows read 0")
        t |> equal(logits_maxdiff([1.0, 3.5, 1.0, 1.0], ones), 2.5, "control: a finite difference reads itself")
        t |> equal(maxdiff_at([1.0, 1.0, 0.0, 1.0], ones), 2, "control: the largest finite difference's index")
        t |> equal(logits_maxabs([1.0, -4.0, 2.0]), 4.0, "control: the largest magnitude")
    }
    t |> run("rows of two lengths are not equal") @(t : T?) {
        t |> equal(logits_maxdiff([1.0, 1.0], [1.0, 1.0, 1.0]), INF, "a truncated row is the largest difference")
        t |> equal(logits_maxdiff([1.0, 1.0, 1.0], [1.0, 1.0]), INF, "an over-long row too")
    }
}

[test]
def test_mismatch_counts(t : T?) {
    t |> run("the exact count takes a length difference as mismatches") @(t : T?) {
        t |> equal(mismatch_exact([1, 2, 3], [1, 2, 3]), 0, "control: equal rows")
        t |> equal(mismatch_exact([1, 9, 3], [1, 2, 3]), 1, "control: one element off")
        t |> equal(mismatch_exact([1, 2], [1, 2, 3]), 1, "a truncated output is one element short")
        t |> equal(mismatch_exact([1, 9, 3, 4, 5], [1, 2, 3]), 3, "one off and two past the end")
        t |> equal(mismatch_exact([7, 1, 9, 3], [1, 2, 3], 1, 0, 3), 1, "a span: got from 1 against want from 0, one off")
        t |> equal(mismatch_exact([7, 1, 2, 3], [1, 2, 3], 1, 0, 3), 0, "control: the same span equal")
        t |> equal(first_mismatch([7, 1, 2, 9], [0, 1, 2, 3], 1, 1, 3), 2, "the first span element off sits at its index in the span")
        t |> equal(first_mismatch([7, 1, 2, 3], [0, 1, 2, 3], 1, 1, 3), -1, "control: an equal span reads -1")
    }
    t |> run("the relative count takes a NaN as off") @(t : T?) {
        let want <- [1.0, 2.0, 3.0]
        let got <- [1.0, NAN_SENTINEL, 3.0]
        t |> equal(mismatch_rel("nan", got, want, APPROX_REL), 1, "mismatch_rel counts the NaN")
        t |> equal(mismatch_rel("nan", got, want, 0.0, 1.0e-3), 1, "mismatch_rel at an absolute floor counts the NaN")
        t |> equal(mismatch_rel("equal", want, want, 0.0, 1.0e-3), 0, "control: equal rows")
        t |> equal(mismatch_rel("miss", [1.0, 2.5, 3.0], want, 0.0, 1.0e-3), 1, "control: a finite miss")
    }
}

//! the asserts `blk` makes on a probe handle that fail, counted instead of failing the cell
def private probe_fails(blk : block<(p : T?) : void>) : int {
    var fails = 0
    var p = new T("probe", ref_time_ticks(), false)
    unsafe {
        p.onFail <- @ capture(& fails)(now : bool) {
            fails++
        }
    }
    p.onLog <- @(msg : string; at : LineInfo) {}
    invoke(blk, p)
    unsafe {
        delete p
    }
    return fails
}

//! `check` of `got` against `want` at `bar` on a probe: the count it returns and the asserts that failed
def private probed_check(got, want : array<float>; bar : Bar) : int2 {
    var off = 0
    let fails = probe_fails() $(p) {
        off = check(p, "probe", got, want, bar, FLAT_SHAPE)
    }
    return int2(off, fails)
}

def private probed_check(got : array<float>; want : array<auto(WT)>; bar : Bar; env : array<auto(ET)>) : int2 {
    var off = 0
    let fails = probe_fails() $(p) {
        off = check(p, "probe", got, want, bar, Shape(row_width = 2), env)
    }
    return int2(off, fails)
}

[test]
def test_check_bars(t : T?) {
    t |> run("rel_of_larger: bar_ok's bar, the default kind") @(t : T?) {
        let want <- four_row()
        t |> equal(probed_check([1.0, -10.01, 2.0, 4.0], want, BAR_APPROX), int2(0, 0), "control: inside 2e-3 of the larger magnitude")
        t |> equal(probed_check([1.0, -10.1, 2.0, 4.0], want, BAR_APPROX), int2(1, 1), "one element past the bar: counted, the cell red")
        t |> equal(probed_check([1.2, -10.0, 2.0, 4.0], want, Bar(abs_floor = 0.25)), int2(0, 0), "control: inside the absolute floor")
        t |> equal(probed_check(want, want, Bar(rel = 1.0)), int2(0, 1), "a bar the poison lands inside reds the control")
    }
    t |> run("exact: bit-equal, the flag and the kind alike") @(t : T?) {
        let want <- four_row()
        t |> equal(probed_check(want, want, BAR_EXACT), int2(0, 0), "control: equal rows")
        t |> equal(probed_check([1.0, -10.0, 2.0, 4.000001], want, BAR_EXACT), int2(1, 1), "one ulp-scale difference")
        t |> equal(probed_check([1.0, -10.0, 2.5, 4.0], want, Bar(kind = BarKind.exact)), int2(1, 1), "the kind spelling")
        t |> equal(probed_check(want, want, Bar(exact = true, poison = 0.0)), int2(0, 1), "a zero poison is not seen: the control reds")
    }
    t |> run("rel_of_max: rel times the expectation's largest magnitude, floored") @(t : T?) {
        let want <- four_row()
        let bar = Bar(kind = BarKind.rel_of_max, rel = 0.01, poison = 1.0, poison_of_max = true)
        t |> equal(probed_check([1.09, -10.0, 2.0, 3.91], want, bar), int2(0, 0), "control: 0.09 inside 1% of 10")
        t |> equal(probed_check([1.11, -10.0, 2.0, 4.0], want, bar), int2(1, 1), "0.11 outside it")
        t |> equal(probed_check([1.11, -10.0, 2.0, 4.0], want, Bar(kind = BarKind.rel_of_max, rel = 0.01, abs_floor = 0.2)), int2(0, 0), "the floor holds it")
        t |> equal(probed_check(want, want, Bar(kind = BarKind.rel_of_max, rel = 0.5, poison = 1.0)), int2(0, 1), "an unscaled poison inside a wide bar reds the control")
    }
    t |> run("poison_of_max: the poison scaled by the expectation's largest magnitude") @(t : T?) {
        let want <- four_row()
        t |> equal(probed_check(want, want, Bar(kind = BarKind.rel_of_max, rel = 0.05, poison = 0.1, poison_of_max = true)), int2(0, 0),
            "0.1 times 10 lands past a bar of 0.5")
        t |> equal(probed_check(want, want, Bar(kind = BarKind.rel_of_max, rel = 0.05, poison = 0.1)), int2(0, 1),
            "control: the unscaled 0.1 sits inside it")
    }
    t |> run("rel_of_max: a NaN in the expectation scales no bar") @(t : T?) {
        t |> equal(check_count([1.05, 10.0, 0.0], [1.0, 10.0, NAN_SENTINEL], Bar(kind = BarKind.rel_of_max, rel = 0.01)), 1,
            "a trailing NaN: only its own element off, the rest held at 1% of 10")
        t |> equal(check_count([0.0, 1.05, 10.0], [NAN_SENTINEL, 1.0, 10.0], Bar(kind = BarKind.rel_of_max, rel = 0.01)), 1,
            "a leading NaN: the same")
        t |> equal(check_count([1.2, 10.0, 0.0], [1.0, 10.0, NAN_SENTINEL], Bar(kind = BarKind.rel_of_max, rel = 0.01)), 2,
            "control: 0.2 past 1% of 10 is still off beside the NaN")
    }
    t |> run("envelope and per_element: each element's own bar, the expectation float or double") @(t : T?) {
        let want <- four_row()
        let env <- [1.0, 100.0, 1.0, 1.0]
        let envb = Bar(kind = BarKind.envelope, rel = 0.01, abs_floor = 1.0e-3)
        t |> equal(probed_check([1.0, -10.5, 2.0, 4.0], want, envb, env), int2(0, 0), "control: 0.5 inside 1% of an envelope of 100")
        t |> equal(probed_check([1.5, -10.0, 2.0, 4.0], want, envb, env), int2(1, 1), "0.5 outside 1% of an envelope of 1")
        let wantd <- [1.0lf, -10.0lf, 2.0lf, 4.0lf]
        t |> equal(probed_check([1.0, -10.5, 2.0, 4.0], wantd, envb, env), int2(0, 0), "a double expectation: control")
        t |> equal(probed_check([1.5, -10.0, 2.0, 4.0], wantd, envb, env), int2(1, 1), "a double expectation: off")
        let bars <- [0.1lf, 0.1lf, 0.1lf, 0.1lf]
        let pe = Bar(kind = BarKind.per_element)
        t |> equal(probed_check([1.05, -10.0, 2.0, 4.0], want, pe, bars), int2(0, 0), "control: inside its own bar")
        t |> equal(probed_check([1.0, -10.0, 2.2, 4.0], want, pe, bars), int2(1, 1), "outside its own bar")
        t |> equal(probed_check([1.0, -10.0, 2.05, 4.0], want, pe, [0.1, 0.1]), int2(1, 1), "an element past the bars' end holds at 0")
    }
    t |> run("poison_at: the control at a chosen element") @(t : T?) {
        let want <- four_row()
        t |> equal(probed_check(want, want, Bar(rel = 0.1, poison_at = 1)), int2(0, 1), "0.5 on the -10 sits inside 10% of the larger magnitude")
        t |> equal(probed_check(want, want, Bar(rel = 0.1, poison_at = 0)), int2(0, 0), "control: 0.5 on the 1 does not")
        t |> equal(probed_check(want, want, Bar(rel = 0.1, poison_at = 9)), int2(0, 1), "an element past the end cannot be poisoned: red")
        t |> equal(probed_check(want, want, Bar(rel = 0.1, poison = 0.1, poison_at = 1)), int2(0, 1), "a fixed 0.1 on the -10 sits inside")
        t |> equal(probed_check(want, want, Bar(rel = 0.1, poison = 0.1, poison_abs = 5.0, poison_at = 1)), int2(0, 0), "poison_abs: 0.1 + 5 on the -10 lands past the bar")
        t |> equal(probed_check(want, want, Bar(rel = 0.1, poison = 0.1, poison_abs = 0.1, poison_at = 1)), int2(0, 1), "control: 0.1 + 0.1 on the -10 still sits inside")
        t |> equal(probed_check(want, want, Bar(rel = 0.1, poison = 0.1, poison_rel = 0.5, poison_at = 1)), int2(0, 0), "poison_rel: at least half the element's magnitude lands outside")
    }
}

def private four_row() : array<float> => [1.0, -10.0, 2.0, 4.0]

let WIDE_BAR = Bar(abs_floor = 1.0e30, poison = 1.0e31)   //! a bar every finite difference sits inside, the poison past it

[test]
def test_check_nan_and_length(t : T?) {
    t |> run("a NaN on either side is off at any bar") @(t : T?) {
        let want <- [1.0, 2.0, 3.0]
        let wide = WIDE_BAR
        t |> equal(probed_check([1.0, NAN_SENTINEL, 3.0], want, wide), int2(1, 1), "a NaN in the kernel's output")
        t |> equal(probed_check(want, [1.0, NAN_SENTINEL, 3.0], wide), int2(1, 1), "a NaN in the reference")
        t |> equal(probed_check([1.0, NAN_SENTINEL, 3.0], want, BAR_EXACT), int2(1, 1), "on the exact bar")
        t |> equal(probed_check(want, want, wide), int2(0, 0), "control: equal rows at the same bar")
    }
    t |> run("a length difference is off") @(t : T?) {
        let want <- [1.0, 2.0, 3.0]
        t |> equal(probed_check([1.0, 2.0], want, BAR_APPROX), int2(1, 1), "a truncated output")
        t |> equal(probed_check([1.0, 2.0, 3.0, 4.0, 5.0], want, BAR_EXACT), int2(2, 1), "an over-long one")
        let none : array<float>
        t |> equal(probed_check(none, none, BAR_EXACT), int2(0, 1), "an empty compare has nothing to poison: red")
    }
    t |> run("check_count counts what check reds, with no assert") @(t : T?) {
        let want <- [1.0, 2.0, 3.0]
        t |> equal(check_count(want, want, BAR_APPROX), 0, "control: equal rows count 0")
        t |> equal(check_count([1.0, 2.5, 3.0], want, BAR_APPROX), 1, "a finite miss counts")
        t |> equal(check_count([1.0, NAN_SENTINEL, 3.0], want, WIDE_BAR), 1, "a NaN in the kernel's output counts at any bar")
        t |> equal(check_count(want, [1.0, NAN_SENTINEL, 3.0], WIDE_BAR), 1, "a NaN in the reference counts")
        t |> equal(check_count([1.0, 2.0], want, BAR_EXACT), 1, "a truncated output counts its missing element")
        t |> equal(check_count([1.0, 2.0, 3.0, 4.0, 5.0], want, BAR_APPROX), 2, "an over-long one counts both extras")
        let fails = probe_fails() $(p) {
            check_count([1.0, 9.0, 3.0], want, BAR_EXACT)
        }
        t |> equal(fails, 0, "a miss asserts nothing")
    }
    t |> run("check_rows is check with its rows and heads") @(t : T?) {
        let fails = probe_fails() $(p) {
            check_rows(p, "rows", [1.0, 2.0, 3.5, 4.0], [1.0, 2.0, 3.0, 4.0], BAR_APPROX, 2, 1)
        }
        t |> equal(fails, 1, "one element off")
        t |> equal(element_at(5, Shape(row_width = 4, head_width = 2)), "row 1 head 0 col 1", "the element's row, head and column")
        t |> equal(element_at(5, FLAT_SHAPE), "element 5", "a flat shape names the index")
    }
}

[test]
def test_check_exact_typed(t : T?) {
    t |> run("bytes and words: the count, the length rule and the poison") @(t : T?) {
        let a <- [int8(1), int8(-2), int8(3)]
        var fails = probe_fails() $(p) {
            t |> equal(check_exact(p, "i8", a, a), 0, "control: equal bytes")
        }
        t |> equal(fails, 0, "control: nothing reds")
        fails = probe_fails() $(p) {
            t |> equal(check_exact(p, "i8", [int8(1), int8(9), int8(3)], a), 1, "one byte off")
        }
        t |> equal(fails, 1, "one byte off reds")
        fails = probe_fails() $(p) {
            t |> equal(check_exact(p, "u16", [uint16(0x3F80), uint16(0x4000)], [uint16(0x3F80)]), 1, "an over-long row")
        }
        t |> equal(fails, 1, "a length difference reds")
        let none : array<uint>
        fails = probe_fails() $(p) {
            check_exact(p, "empty", none, none)
        }
        t |> equal(fails, 1, "an empty compare has nothing to poison: red")
    }
    t |> run("the poisoned expectation one past an element whose ulp exceeds one is not seen: red") @(t : T?) {
        var fails = probe_fails() $(p) {
            t |> equal(check_exact(p, "big", [1.0e8], [1.0e8]), 0, "the rows are equal")
        }
        t |> equal(fails, 1, "1e8 + 1 rounds to 1e8: the control cannot see its poison and reds")
        fails = probe_fails() $(p) {
            check_exact(p, "small", [1.0], [1.0])
        }
        t |> equal(fails, 0, "control: 1 + 1 is seen")
    }
}

[test]
def test_compare_helpers(t : T?) {
    t |> run("planes, copies and spans") @(t : T?) {
        t |> equal(plane_of(3, 2.5), [2.5, 2.5, 2.5], "a plane of the value")
        t |> success(is_nan(nan_plane(2)[1]), "the NaN plane")
        t |> equal(poison_plane(1)[0], POISON, "the poison plane")
        t |> equal(poisoned_copy([1.0, 2.0], 1, 0.5), [1.0, 2.5], "a float moved at its element")
        t |> equal(poisoned_copy([1, 2, 3], 0, 4), [5, 2, 3], "an int moved")
        t |> equal(copy_span([int8(1), int8(2), int8(3), int8(4)], 1, 2), [int8(2), int8(3)], "a byte span")
        t |> equal(copy_span([1.0, 2.0, 3.0], 1l, 2l), [2.0, 3.0], "a float span at 64-bit bounds")
    }
    t |> run("argmax and the taint seam") @(t : T?) {
        t |> equal(argmax_of([1.0, 7.0, 3.0, 7.0], 4l), 1l, "the first largest")
        t |> equal(argmax_of([1.0, 7.0, 3.0, 9.0], 3l), 1l, "within the first n")
        t |> equal(opaque(2.5), 2.5, "a float through the seam")
        t |> equal(opaque(0x1234u), 0x1234u, "a word through the seam")
        if (jit_enabled()) {
            t |> success(jit_only(t), "the JIT passes the JIT-only gate")
        }
    }
}

[test]
def test_sweep_tally(t : T?) {
    t |> run("a length sweep tallies lengths, misses, poison legs and inert elements") @(t : T?) {
        var r : Sweep
        sweep_check(r, 2, 0.0, [1.0, 2.0], [1.0, 2.0])
        sweep_check(r, 3, 0.1, [1.0, 2.05, 3.0], [1.0, 2.0, 3.0], 0.5)
        t |> equal(r.cases, 2, "two lengths")
        t |> equal(r.elems, 5, "five elements")
        t |> equal(r.bad, 0, "control: none off")
        t |> equal(r.worst_n, 3, "the largest difference's length")
        t |> equal(r.worst_at, 1, "and its element")
        var fails = probe_fails() $(p) {
            report_sweep(p, "clean", r, 2)
        }
        t |> equal(fails, 0, "control: a clean sweep reports green")
        sweep_check(r, 2, 0.1, [NAN_SENTINEL, 2.0], [1.0, 2.0], 0.5)
        sweep_check(r, 2, 0.0, [1.0], [1.0, 2.0])
        t |> equal(r.bad, 2, "a NaN and a missing element are off")
        sweep_check(r, 1, 1.0, [1.0], [1.0], 0.5)
        t |> equal(r.poison_inside, 1, "a poison inside a loose bar is counted")
        sweep_inert(r, [1.0, 5.0], [1.0, 2.0])
        t |> equal(r.inert, 1, "one element left where it was")
        fails = probe_fails() $(p) {
            report_sweep(p, "dirty", r, 2, true)
        }
        t |> equal(fails, 4, "the length count, the misses, the poison leg and the inert element each red")
    }
    t |> run("a sweep with no length run, or no element compared, reds under the default count") @(t : T?) {
        let none : Sweep
        var fails = probe_fails() $(p) {
            report_sweep(p, "none", none)
        }
        t |> equal(fails, 1, "no length ran")
        var empty : Sweep
        let no_elems : array<float>
        sweep_check(empty, 0, 0.0, no_elems, no_elems)
        fails = probe_fails() $(p) {
            report_sweep(p, "empty", empty)
        }
        t |> equal(fails, 1, "a length ran with no element")
        var one : Sweep
        sweep_check(one, 1, 0.0, [1.0], [1.0])
        fails = probe_fails() $(p) {
            report_sweep(p, "one", one)
        }
        t |> equal(fails, 0, "control: one length, one element")
    }
}
