// Tutorial DASTRUDEL-08: Effects & Filters
//
// daslang's strudel splits effects into two groups:
//
//   Per-orbit bus FX     — room (reverb), delay, chorus
//                          one shared instance per orbit, voices send wet/dry
//   Per-voice FX         — lpf, hpf, phaser, tremolo, compressor, shape, crush
//                          baked into each individual voice
//
// Use orbit(N) to route a pattern to a specific bus so each layer can carry
// its own reverb size and delay time. Use the per-voice setters to colour
// individual notes.
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_08_effects_filters.das

options gen2
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
// 09 for the resolver), so plain `delay(0.5)` already locks to the cps.

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

[export]
def main() {
    print("DASTRUDEL-08: Effects & Filters\n")
    example_lpf_hpf()
    example_room()
    example_delay()
    example_phaser()
    example_orbit_busses()
    print("\nDone.\n")
}
