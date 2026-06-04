// Tutorial DASTRUDEL-08: Effects & Filters
//
// daslang's strudel splits effects into two groups (shared with tutorial 07):
//
//   Per-orbit bus FX  — room/roomsize, delay/delaytime/delayfeedback, chorus
//                       one shared instance per orbit; voices send wet/dry
//   Per-voice FX      — gain, pan, speed, lpf, hpf, bpf, phaser, tremolo,
//                       compressor, shape, crush, coarse, djf, fm, vowel
//                       baked into each individual voice
//
// Use orbit(N) to route a pattern to a specific bus so each layer can carry
// its own reverb size and delay time. Use the per-voice setters to colour
// individual notes. (Every setter below also has a pattern-valued overload —
// feed it a signal to make the parameter move; see tutorial 09.)
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_08_effects_filters.das

options gen2
options persistent_heap
options indenting = 4

require strudel/strudel public
require strudel/strudel_player public
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
// Section 1 — lpf and hpf (per-voice filters)
// ──────────────────────────────────────────────────────────────────────────
//
// lpf(freq) cuts harmonics above `freq` Hz; hpf(freq) cuts below. Compare a
// muffled sawtooth (lpf 200) against a thin one (hpf 2000).

def example_lpf_hpf() {
    print("\n=== Section 1: lpf vs hpf on a sawtooth ===\n")
    let pat_lpf <- atom("sawtooth") |> note(48.0) |> sustain(1.0) |> lpf(200.0)
    play(pat_lpf, 0.5lf, 3.0lf)

    let pat_hpf <- atom("sawtooth") |> note(48.0) |> sustain(1.0) |> hpf(2000.0)
    play(pat_hpf, 0.5lf, 3.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — room (reverb, per-orbit bus)
// ──────────────────────────────────────────────────────────────────────────
//
// room(amount) sets the wet send to the orbit's reverb bus; roomsize(N)
// controls the room dimensions. Both live on the bus, so all voices on the
// same orbit share one reverb instance.

def example_room() {
    print("\n=== Section 2: dry vs wet reverb ===\n")
    let pat <- note("c4 ~ c4 ~", "sine") |> attack(0.001) |> decay(0.15) |> sustain(0.0) |> release(0.01) |> room(0.8) |> roomsize(2.0)
    play(pat, 0.5lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — delay (per-orbit bus)
// ──────────────────────────────────────────────────────────────────────────
//
// delay(amount), delaytime(seconds), delayfeedback(0..1) drive the orbit's
// delay line. delaytime defaults to a tempo-aware 3/16 cycle (see tutorial
// 10 for the resolver), so plain `delay(0.5)` already locks to the cps.

def example_delay() {
    print("\n=== Section 3: delay echo on a pluck ===\n")
    let pat <- note("c4 ~ e4 ~ g4 ~ c5 ~", "sine") |> attack(0.001) |> decay(0.15) |> sustain(0.0) |> release(0.01) |> delay(0.5) |> delaytime(0.5) |> delayfeedback(0.4)
    play(pat, 0.5lf, 6.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — phaser (per-voice)
// ──────────────────────────────────────────────────────────────────────────
//
// phaser(rateHz) sweeps a notch through the spectrum at the given rate.
// Because it is per-voice, every note carries its own phase — two stacked
// voices stay independent.

def example_phaser() {
    print("\n=== Section 4: per-voice phaser on a sustained sawtooth ===\n")
    let pat <- run(8) |> scale("D:pentatonic") |> sound("sawtooth") |> sustain(1.0) |> release(0.5) |> phaser(2.0)
    play(pat, 0.5lf, 6.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 5 — per-orbit FX busses: two layers, two reverbs
// ──────────────────────────────────────────────────────────────────────────
//
// Different orbits have independent FX instances. Here, orbit 0 gets a
// tight small room while orbit 1 sits inside a big cathedral.

def example_orbit_busses() {
    print("\n=== Section 5: two orbits, two reverb sizes ===\n")
    let pat <- stack([
        note("c4 ~ e4 ~", "sine") |> attack(0.001) |> decay(0.15) |> sustain(0.0) |> release(0.01) |> room(0.8) |> roomsize(0.5) |> orbit(0),
        note("c3", "triangle") |> attack(0.5) |> decay(0.2) |> sustain(0.6) |> release(0.5) |> room(0.8) |> roomsize(8.0) |> orbit(1)
    ])
    play(pat, 0.5lf, 6.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 6 — crush & shape (per-voice waveshaping)
// ──────────────────────────────────────────────────────────────────────────
//
// crush(bits) reduces bit depth for a lo-fi grit; coarse(n) drops the sample
// rate by a factor of n (decimation). shape(amount) is a waveshaper that adds
// harmonic distortion. All per-voice.

def example_crush_shape() {
    print("\n=== Section 6: crush + coarse, then shape (waveshaper) ===\n")
    let pat_crush <- note("c3 e3 g3 c4", "sawtooth") |> sustain(0.8) |> crush(4.0) |> coarse(4)
    play(pat_crush, 0.8lf, 4.0lf)

    let pat_shape <- note("c3 e3 g3 c4", "sine") |> sustain(0.8) |> shape(0.6)
    play(pat_shape, 0.8lf, 4.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 7 — tremolo & compressor (per-voice amplitude FX)
// ──────────────────────────────────────────────────────────────────────────
//
// tremolo(rateHz) modulates amplitude for a pulsing effect (tremolodepth sets
// how deep); compressor(thresholdDb) tames dynamics so quiet and loud notes
// sit closer together.

def example_tremolo_compressor() {
    print("\n=== Section 7: tremolo, then compressor ===\n")
    let pat_trem <- note("d3 d3 d#3 d3", "supersaw") |> sustain(1.0) |> release(0.5) |> tremolo(8.0) |> tremolodepth(0.8)
    play(pat_trem, 0.5lf, 4.0lf)

    let pat_comp <- s("bd sd hh cp") |> compressor(-20.0)
    play(pat_comp, 0.5lf, 4.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 8 — bpf & djf (more per-voice filters)
// ──────────────────────────────────────────────────────────────────────────
//
// bpf(freq) is a band-pass filter (bpq sets resonance/Q); djf(x) is a single
// DJ-style filter knob — below 0.5 it acts as a low-pass, above 0.5 a
// high-pass, so one parameter sweeps from muffled to thin.

def example_bpf_djf() {
    print("\n=== Section 8: band-pass, then DJ filter ===\n")
    let pat_bpf <- note("c3 e3 g3 c4", "sawtooth") |> sustain(0.8) |> bpf(800.0) |> bpq(6.0)
    play(pat_bpf, 0.8lf, 4.0lf)

    let pat_djf <- note("c3 e3 g3 c4", "sawtooth") |> sustain(0.8) |> djf(0.8)
    play(pat_djf, 0.8lf, 4.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 9 — chorus (per-orbit bus)
// ──────────────────────────────────────────────────────────────────────────
//
// chorus(amount) thickens a sound by mixing in slightly detuned copies. Like
// room and delay it lives on the orbit bus, so all voices on the same orbit
// share one chorus instance.

def example_chorus() {
    print("\n=== Section 9: chorus on a chord progression ===\n")
    let pat <- note("<[c3,e3,g3] [f3,a3,c4] [g3,b3,d4] [a3,c4,e4]>", "sawtooth") |> chorus(0.5) |> attack(0.05) |> decay(0.1) |> sustain(0.6) |> release(0.3) |> gain(0.4)
    play(pat, 0.5lf, 8.0lf)
}

[export]
def main() {
    print("DASTRUDEL-08: Effects & Filters\n")
    example_lpf_hpf()
    example_room()
    example_delay()
    example_phaser()
    example_orbit_busses()
    example_crush_shape()
    example_tremolo_compressor()
    example_bpf_djf()
    example_chorus()
    print("\nDone.\n")
}
