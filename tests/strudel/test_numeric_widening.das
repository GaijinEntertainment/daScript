options gen2
options indenting = 4

// Compile-coverage for the int|float|double numeric widening (Phase A/B).
// The POINT of this file is that it COMPILES: every widened public modifier is
// exercised with int, float, AND double literals (one shallow chain per modifier
// — kept shallow because deep fmap chains overflow the query stack). If any
// variant regresses to a single concrete numeric type, this file fails to build
// and CI catches it. Runtime behaviour is covered by the other strudel tests.

require dastest/testing_boost
require strudel/strudel
require math

// hap count over one cycle
def private hc(p : Pattern) : int {
    let h <- p(TimeSpan(start = 0.0lf, stop = 1.0lf))
    return length(h)
}

[test]
def test_effect_setters_accept_int_float_double(t : T?) {
    // each modifier called with int, then float, then double — non-structural,
    // so the single held event survives the depth-3 chain.
    t |> equal(hc(atom("sawtooth") |> gain(1) |> gain(0.5) |> gain(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> velocity(1) |> velocity(0.5) |> velocity(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> pan(0) |> pan(0.5) |> pan(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> speed(2) |> speed(1.5) |> speed(1.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> note(60) |> note(60.0) |> note(60.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> freq(440) |> freq(440.0) |> freq(440.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> room(1) |> room(0.5) |> room(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> roomsize(2) |> roomsize(2.0) |> roomsize(2.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> delay(1) |> delay(0.5) |> delay(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> delaytime(1) |> delaytime(0.25) |> delaytime(0.25lf)), 1)
    t |> equal(hc(atom("sawtooth") |> delayfeedback(1) |> delayfeedback(0.5) |> delayfeedback(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> fm(2) |> fm(2.0) |> fm(2.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> fmh(1) |> fmh(1.5) |> fmh(1.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> lpf(1000) |> lpf(1000.0) |> lpf(1000.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> hpf(200) |> hpf(200.0) |> hpf(200.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> lpq(5) |> lpq(5.0) |> lpq(5.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> hpq(5) |> hpq(5.0) |> hpq(5.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> attack(0) |> attack(0.01) |> attack(0.01lf)), 1)
    t |> equal(hc(atom("sawtooth") |> decay(0) |> decay(0.1) |> decay(0.1lf)), 1)
    t |> equal(hc(atom("sawtooth") |> sustain(1) |> sustain(0.5) |> sustain(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> release(1) |> release(0.5) |> release(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> chorus(1) |> chorus(0.5) |> chorus(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> crush(4) |> crush(4.0) |> crush(4.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> shape(0) |> shape(0.5) |> shape(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> djf(0) |> djf(0.5) |> djf(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> bpf(800) |> bpf(800.0) |> bpf(800.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> bpq(5) |> bpq(5.0) |> bpq(5.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> phaser(1) |> phaser(2.0) |> phaser(2.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> tremolo(2) |> tremolo(2.0) |> tremolo(2.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> compressor(-20) |> compressor(-20.0) |> compressor(-20.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> begin(0) |> begin(0.25) |> begin(0.25lf)), 1)
    t |> equal(hc(atom("sawtooth") |> transpose(7) |> transpose(7.0) |> transpose(7.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> add(0) |> add(2.0) |> add(2.0lf)), 1)
}

[test]
def test_newly_public_set_x_scalars(t : T?) {
    // the 9 set_X scalars exposed in Phase B accept int / float / double
    t |> equal(hc(atom("sawtooth") |> set_speed(2) |> set_speed(1.5) |> set_speed(1.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> set_velocity(1) |> set_velocity(0.5) |> set_velocity(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> set_chorus(1) |> set_chorus(0.5) |> set_chorus(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> set_crush(4) |> set_crush(4.0) |> set_crush(4.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> set_shape(0) |> set_shape(0.5) |> set_shape(0.5lf)), 1)
    t |> equal(hc(atom("sawtooth") |> set_phaser(1) |> set_phaser(2.0) |> set_phaser(2.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> set_tremolo(2) |> set_tremolo(2.0) |> set_tremolo(2.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> set_compressor(-20) |> set_compressor(-20.0) |> set_compressor(-20.0lf)), 1)
    t |> equal(hc(atom("sawtooth") |> set_bpf(800) |> set_bpf(800.0) |> set_bpf(800.0lf)), 1)
    // field actually written
    let sp <- atom("sawtooth") |> set_crush(4)
    let h <- sp(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(h[0].value.crush - 4.0) < 0.001)
}

[test]
def test_time_funcs_accept_int_float_double(t : T?) {
    t |> equal(hc(s("bd sd") |> fast(2)), 4)
    t |> equal(hc(s("bd sd") |> fast(2.0)), 4)
    t |> equal(hc(s("bd sd") |> fast(2.0lf)), 4)
    let _a <- s("bd sd") |> slow(2) |> hurry(2)
    let _b <- s("bd") |> linger(1) |> linger(0.5) |> linger(0.5lf)
    t |> equal(hc(s("bd") |> compress(0, 1)), 1)
    t |> equal(hc(s("bd") |> compress(0.25, 0.75)), 1)
    t |> equal(hc(s("bd") |> compress(0.25lf, 0.75lf)), 1)
}

[test]
def test_pattern_overload_still_resolves(t : T?) {
    // widening must NOT shadow the Pattern (mod_pat) overload
    var p <- s("bd sd")
    p <- speed(p, s("1 2"))
    p <- gain(p, s("0.5 1.0"))
    p <- fast(p, s("1 2"))
    t |> success(hc(p) > 0)
}
