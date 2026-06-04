// Tutorial DASTRUDEL-09: Signals & Modulation
//
// This tutorial covers:
//   - Signals — continuous patterns of numbers: sine, saw, tri, square,
//     perlin, rand, and the discrete run / irand
//   - range(lo, hi) — remap a signal's native 0..1 into any span
//   - Pattern-valued setters — feed a signal into lpf / gain / pan / ...
//     so an effect parameter MOVES over time (the flagship feature)
//   - slow() on a signal — stretch the modulation across several cycles
//
// A signal is just a Pattern like any other — but instead of notes or
// drum hits it carries a number in each Hap. The same combinators
// (slow, fast, range) that shape rhythmic patterns also shape signals.
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_09_signals_modulation.das

options gen2
options persistent_heap
options indenting = 4

require strudel/strudel public
require strudel/strudel_player public
require strudel/strudel_time
require audio/audio_boost
require daslib/fio

def play(var pat : Pattern; cps : double = 0.5lf; seconds : double = 4.0lf) {
    with_audio_system() {
        strudel_create_channel()
        strudel_set_cps(cps)
        strudel_add_track(pat)
        let t0 = ref_time_ticks()
        while (double(get_time_usec(t0)) / 1000000.0lf < seconds) {
            strudel_tick()
            sleep(5u)
        }
        strudel_shutdown()
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — A signal is a pattern of numbers
// ──────────────────────────────────────────────────────────────────────────
//
// Query a signal by hand and you get Haps whose value is a number, not a
// sound. sine() is a *continuous* signal: one query window yields one Hap
// carrying the wave's value at the start of that window. At phase 0 a
// strudel sine sits at its midpoint, 0.5 (it sweeps 0 -> 1 -> 0 per cycle).
// run(n) is a *discrete* signal: n equal steps valued 0, 1, ... n-1.

def example_query_signal() {
    print("=== Section 1: query signals by hand ===\n")
    let sig <- sine()
    var h <- invoke(sig, TimeSpan(start = 0.0lf, stop = 1.0lf))
    print("  sine() over [0,1) -> {length(h)} hap, value = {h[0].value.note}\n")
    delete h

    let rn <- run(4)
    var hr <- invoke(rn, TimeSpan(start = 0.0lf, stop = 1.0lf))
    print("  run(4) over [0,1) -> {length(hr)} haps: ")
    for (x in hr) {
        print("{x.value.note} ")
    }
    print("\n")
    delete hr
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — range: remap a signal into a useful span
// ──────────────────────────────────────────────────────────────────────────
//
// Every continuous signal (sine, cosine, saw, isaw, tri, itri, square,
// perlin, rand) runs 0..1. range(lo, hi) stretches that into the span you
// actually want — a filter cutoff in Hz, a gain in 0..1, a pan position.
// Here run() drives pitch directly: run(8) |> add(48) makes a chromatic
// ramp from MIDI 48 (C3) upward, one step per eighth of the cycle.

def example_run_melody() {
    print("\n=== Section 2: run() as a melodic ramp (C3 chromatic, 8 steps) ===\n")
    let pat <- run(8) |> add(48.0) |> sound("sine") |> sustain(0.3)
    play(pat, 0.5lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — pattern-valued lpf: a moving filter sweep
// ──────────────────────────────────────────────────────────────────────────
//
// This is the headline. Every per-event setter (lpf, gain, pan, speed, ...)
// has an overload that takes a *Pattern* instead of a constant. Feed it a
// signal and the parameter tracks the signal over time. sine() |> range(200,
// 4000) |> slow(4) sweeps the cutoff from 200 Hz to 4 kHz and back over four
// cycles — the classic filter-sweep pad.

def example_lpf_sweep() {
    print("\n=== Section 3: sine -> lpf sweep on a sawtooth ===\n")
    let pat <- (
        atom("sawtooth") |> note(48.0) |> sustain(1.0)
        |> lpf(sine() |> range(200.0, 4000.0) |> slow(4.0lf))
    )
    play(pat, 0.5lf, 8.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — modulating gain and pan
// ──────────────────────────────────────────────────────────────────────────
//
// The same trick works for any setter. A slow sine on gain gives a smooth
// volume swell (tremolo by hand); a slow sine on pan drifts the sound across
// the stereo field. Stack them and one sustained tone breathes and moves.

def example_gain_pan() {
    print("\n=== Section 4: sine -> gain swell + sine -> pan drift ===\n")
    let pat <- (
        atom("sine") |> note(48.0) |> sustain(1.0)
        |> gain(sine() |> slow(2.0lf))
        |> pan(sine() |> slow(4.0lf))
    )
    play(pat, 0.5lf, 8.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 5 — the noisy signals: perlin and rand
// ──────────────────────────────────────────────────────────────────────────
//
// rand() is white-noise-random — a fresh value every query, good for
// scattering pan or velocity. perlin() is smooth/organic random — it wanders
// instead of jumping, ideal for slow, living modulation. Here perlin drives
// gain on a pad for an unpredictable swell, while rand scatters pan per note.

def example_perlin_rand() {
    print("\n=== Section 5: perlin -> gain swell, rand -> pan scatter ===\n")
    let pad <- (
        atom("sawtooth") |> note(48.0) |> sustain(1.0) |> lpf(800.0)
        |> gain(perlin() |> range(0.1, 0.8) |> slow(4.0lf))
    )
    play(pad, 0.5lf, 6.0lf)

    let mel <- note("c4 e4 g4 a4 g4 e4", "sine") |> sustain(0.5) |> pan(rand())
    play(mel, 0.5lf)
}

[export]
def main() {
    print("DASTRUDEL-09: Signals & Modulation\n\n")
    example_query_signal()
    example_run_melody()
    example_lpf_sweep()
    example_gain_pan()
    example_perlin_rand()
    print("\nDone.\n")
}
