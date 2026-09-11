options gen2
options indenting = 4

require dastest/testing_boost
require strudel/strudel

// The shapes strudel.cc patterns lean on that ported silently wrong or not at all: a mask step
// that spells false, the space form of a scale name, the time shifts, the 25% chance, and the
// pattern-valued add and sub.

def haps_of(var pat : Pattern; start, stop : double) : array<Hap> {
    return <- pat(TimeSpan(start = start, stop = stop))
}

def sounds(var pat : Pattern; start, stop : double) : array<string> {
    var haps <- haps_of(pat, start, stop)
    return <- [for (h in haps); h.value.s]
}

def notes(var pat : Pattern; start, stop : double) : array<float> {
    var haps <- haps_of(pat, start, stop)
    return <- [for (h in haps); h.value.note]
}

//! whether some hap of sound `name` has its whole starting at `start` (a query window also returns partial haps)
def has_start(var haps : array<Hap>; name : string; start : double) : bool {
    for (h in haps) {
        if (h.value.s == name && abs(h.whole.start - start) < 1e-9lf) {
            return true
        }
    }
    return false
}

[test]
def test_mask_false_steps(t : T?) {
    t |> run("a mask step spelled f, false or 0 masks out; t and 1 keep") @(t : T?) {
        t |> equal(sounds(mask(s("a b c d"), s("t f t f")), 0.0lf, 1.0lf), ["a", "c"])
        t |> equal(sounds(mask(s("a b c d"), s("1 0 false true")), 0.0lf, 1.0lf), ["a", "d"])
    }
    t |> run("a cycling mask gates whole cycles: <1 0> keeps cycle 0 and drops cycle 1") @(t : T?) {
        let pat <- mask(s("a b"), s("<1 0>"))
        t |> equal(length(sounds(pat, 0.0lf, 1.0lf)), 2)
        t |> equal(length(sounds(pat, 1.0lf, 2.0lf)), 0)
        t |> equal(length(sounds(pat, 2.0lf, 3.0lf)), 2)
    }
}

[test]
def test_struct_false_steps(t : T?) {
    t |> run("struct_ takes its timing from the truthy steps only") @(t : T?) {
        let haps <- haps_of(struct_(atom("bd"), s("x f x x")), 0.0lf, 1.0lf)
        t |> equal(length(haps), 3)
        t |> equal(haps[0].whole.start, 0.0lf)
        t |> equal(haps[1].whole.start, 0.5lf)
        t |> equal(haps[2].whole.start, 0.75lf)
    }
}

[test]
def test_scale_space_form(t : T?) {
    t |> run("`G1 minor` is the same scale as `G1:minor`, not minor with a C root") @(t : T?) {
        let colon <- notes(n("0 2 4") |> set_scale("G1:minor"), 0.0lf, 1.0lf)
        let space <- notes(n("0 2 4") |> set_scale("G1 minor"), 0.0lf, 1.0lf)
        t |> equal(space, colon)
        t |> equal(colon[0], 31.0)
    }
}

[test]
def test_early_late(t : T?) {
    t |> run("late moves an event's whole later by the offset, early earlier") @(t : T?) {
        var l <- haps_of(late(s("a b"), 0.25), 0.0lf, 1.0lf)
        t |> success(has_start(l, "a", 0.25lf), "late: a starts at 0.25")
        t |> success(has_start(l, "b", 0.75lf), "late: b starts at 0.75")
        var e <- haps_of(early(s("a b"), 0.25), 0.0lf, 1.0lf)
        t |> success(has_start(e, "b", 0.25lf), "early: b starts at 0.25")
        t |> success(has_start(e, "a", 0.75lf), "early: a starts at 0.75")
    }
}

[test]
def test_rarely(t : T?) {
    t |> run("rarely is public and keeps every event") @(t : T?) {
        let pat <- s("a b c d") |> rarely(@(var x : Pattern) => x |> set_note(72.0))
        t |> equal(length(sounds(pat, 0.0lf, 1.0lf)), 4)
    }
}

[test]
def test_add_sub_pattern(t : T?) {
    t |> run("add with a pattern samples the offset per cycle") @(t : T?) {
        let pat <- note("c3 e3") |> add(n("<0 12>"))
        t |> equal(notes(pat, 0.0lf, 1.0lf), [48.0, 52.0])
        t |> equal(notes(pat, 1.0lf, 2.0lf), [60.0, 64.0])
    }
    t |> run("sub with a scalar and with a pattern") @(t : T?) {
        t |> equal(notes(note("c3") |> sub(12), 0.0lf, 1.0lf), [36.0])
        let pat <- note("a4 a3") |> sub(n("<7 12>"))
        t |> equal(notes(pat, 0.0lf, 1.0lf), [62.0, 50.0])
        t |> equal(notes(pat, 1.0lf, 2.0lf), [57.0, 45.0])
    }
}

[test]
def test_note_name_case(t : T?) {
    t |> run("an upper-case note letter names the same note as the lower-case one") @(t : T?) {
        t |> equal(notes(note("C4 Bb3 F#2 Eb5"), 0.0lf, 1.0lf), notes(note("c4 bb3 f#2 eb5"), 0.0lf, 1.0lf))
        t |> equal(notes(note("C4 Bb3"), 0.0lf, 1.0lf), [60.0, 58.0])
        t |> equal(notes(note("<G4 A4>,Bb3"), 1.0lf, 2.0lf), [69.0, 58.0])
    }
}

[test]
def test_negative_numbers(t : T?) {
    t |> run("a negative numeric atom keeps its sign") @(t : T?) {
        t |> equal(notes(n("0 -2 -4.5"), 0.0lf, 1.0lf), [0.0, -2.0, -4.5])
        t |> equal(notes(n("<0 -2>"), 1.0lf, 2.0lf), [-2.0])
        t |> equal(notes(n("0 -2") |> scale("C3:minor"), 0.0lf, 1.0lf), [48.0, 44.0])
    }
    t |> run("a negative euclid rotation turns the other way") @(t : T?) {
        var back <- haps_of(s("x(3,8,-1)"), 0.0lf, 1.0lf)
        let seven <- haps_of(s("x(3,8,7)"), 0.0lf, 1.0lf)
        t |> equal(length(back), 3)
        for (i in range(3)) {
            t |> equal(back[i].whole.start, seven[i].whole.start)
        }
        t |> success(has_start(back, "x", 0.125lf), "x(3,8,-1) starts at 1/8, where x(3,8,1) starts at 1/4")
    }
}

[test]
def test_nested_alternation(t : T?) {
    t |> run("an alternation inside a sequence step advances with the outer cycle") @(t : T?) {
        let pat <- n("<0 1 2> <5 6 7>")
        t |> equal(notes(pat, 0.0lf, 1.0lf), [0.0, 5.0])
        t |> equal(notes(pat, 1.0lf, 2.0lf), [1.0, 6.0])
        t |> equal(notes(pat, 2.0lf, 3.0lf), [2.0, 7.0])
        t |> equal(notes(pat, 3.0lf, 4.0lf), [0.0, 5.0])
    }
    t |> run("an alternation inside an alternation runs at half the outer clock") @(t : T?) {
        let pat <- n("<<0 1> 9>")
        t |> equal(notes(pat, 0.0lf, 1.0lf), [0.0])
        t |> equal(notes(pat, 1.0lf, 2.0lf), [9.0])
        t |> equal(notes(pat, 2.0lf, 3.0lf), [1.0])
        t |> equal(notes(pat, 3.0lf, 4.0lf), [9.0])
    }
}
