options gen2
options indenting = 4

// Phase C/D features: freq() control, euclidRot, chop, slice, and the
// bd(3,8) Euclidean mini-notation form.

require dastest/testing_boost
require strudel/strudel
require strudel/strudel_time
require math

def private at(d : double) : TimeSpan {
    return TimeSpan(start = 0.0lf, stop = d)
}

def private hc(p : Pattern) : int {
    let h <- p(TimeSpan(start = 0.0lf, stop = 1.0lf))
    return length(h)
}

[test]
def test_freq_sets_field(t : T?) {
    let p <- atom("sawtooth") |> freq(440.0)
    let h <- p(at(1.0lf))
    t |> equal(length(h), 1)
    t |> success(abs(h[0].value.freq - 440.0) < 0.001)
    // default is 0 (derive from note)
    let p2 <- atom("sawtooth")
    let h2 <- p2(at(1.0lf))
    t |> success(abs(h2[0].value.freq) < 0.001)
}

[test]
def test_freq_pattern_overload(t : T?) {
    // pattern-valued freq still resolves and samples
    let p <- atom("sawtooth") |> freq(note("220 440"))
    let h <- p(at(1.0lf))
    t |> success(!empty(h))
}

[test]
def test_note_to_freq_exists(t : T?) {
    // feedback claimed this was missing — it is not
    t |> success(abs(note_to_freq(69.0) - 440.0) < 0.5)
}

[test]
def test_euclidRot(t : T?) {
    // euclid(3,8) onsets: steps 0,3,6 -> 0, 0.375, 0.75
    let e0 <- atom("bd") |> euclid(3, 8)
    let h0 <- e0(at(1.0lf))
    t |> equal(length(h0), 3)
    t |> success(abs(h0[0].whole.start - 0.0lf) < 0.001lf)
    // euclidRot(3,8,1): rotate left by 1 -> steps 2,5,7 -> 0.25, 0.625, 0.875
    let e1 <- atom("bd") |> euclidRot(3, 8, 1)
    let h1 <- e1(at(1.0lf))
    t |> equal(length(h1), 3)
    t |> success(abs(h1[0].whole.start - 0.25lf) < 0.001lf)
    t |> success(abs(h1[1].whole.start - 0.625lf) < 0.001lf)
    t |> success(abs(h1[2].whole.start - 0.875lf) < 0.001lf)
}

[test]
def test_chop(t : T?) {
    // chop(4): one held sample -> 4 back-to-back slices in [0,1)
    let p <- atom("bev") |> chop(4)
    let h <- p(at(1.0lf))
    t |> equal(length(h), 4)
    t |> success(abs(h[0].value.begin - 0.0) < 0.001)
    t |> success(abs(h[0].value.end_pos - 0.25) < 0.001)
    t |> success(abs(h[3].value.begin - 0.75) < 0.001)
    t |> success(abs(h[3].value.end_pos - 1.0) < 0.001)
    // whole subdivided
    t |> success(abs(h[0].whole.start - 0.0lf) < 0.001lf)
    t |> success(abs(h[0].whole.stop - 0.25lf) < 0.001lf)
}

[test]
def test_slice(t : T?) {
    // slice(4, "0 2 1 3"): structure from idx pattern; begin/end from index
    let p <- s("bev") |> slice(4, note("0 2 1 3"))
    let h <- p(at(1.0lf))
    t |> equal(length(h), 4)
    // slot 0 plays slice 0
    t |> success(abs(h[0].value.begin - 0.0) < 0.001)
    // slot 1 plays slice 2 -> begin 0.5
    t |> success(abs(h[1].value.begin - 0.5) < 0.001)
    // slot 2 plays slice 1 -> begin 0.25
    t |> success(abs(h[2].value.begin - 0.25) < 0.001)
    // sound carried from pat
    t |> equal(h[0].value.s, "bev")
    // out-of-range indices wrap modulo n: "4 5" with n=4 -> slices 0, 1
    let pw <- s("bev") |> slice(4, note("4 5"))
    let hw <- pw(at(1.0lf))
    t |> equal(length(hw), 2)
    t |> success(abs(hw[0].value.begin - 0.0) < 0.001)
    t |> success(abs(hw[1].value.begin - 0.25) < 0.001)
}

[test]
def test_granulation_guards(t : T?) {
    // live-coding safety: n <= 0 must not crash — returns the pattern unchanged
    t |> equal(hc(atom("bev") |> chop(0)), 1)
    t |> equal(hc(atom("bev") |> chop(-2)), 1)
    t |> equal(hc(atom("bev") |> striate(0)), 1)
    t |> equal(hc(s("bev") |> slice(0, note("0 1"))), 1)
}

[test]
def test_mini_euclid_paren(t : T?) {
    // "bd(3,8)" -> euclid(3,8)
    let p <- s("bd(3,8)")
    let h <- p(at(1.0lf))
    t |> equal(length(h), 3)
    t |> success(abs(h[0].whole.start - 0.0lf) < 0.001lf)
    t |> success(abs(h[1].whole.start - 0.375lf) < 0.001lf)
    t |> success(abs(h[2].whole.start - 0.75lf) < 0.001lf)
    // "bd(3,8,2)" -> euclidRot(3,8,2): rotate left by 2 -> steps 1,4,6 -> 0.125, 0.5, 0.75
    let pr <- s("bd(3,8,2)")
    let hr <- pr(at(1.0lf))
    t |> equal(length(hr), 3)
    t |> success(abs(hr[0].whole.start - 0.125lf) < 0.001lf)
    t |> success(abs(hr[1].whole.start - 0.5lf) < 0.001lf)
    t |> success(abs(hr[2].whole.start - 0.75lf) < 0.001lf)
    // composes in a sequence with another atom
    let p2 <- s("bd(3,8) sd")
    let h2 <- p2(at(1.0lf))
    t |> equal(length(h2), 4)
}
