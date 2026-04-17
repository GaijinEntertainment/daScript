// Tutorial DASTRUDEL-11: Synthesis
//
// This tutorial covers:
//   - Oscillator sources via s("sine"|"sawtooth"|"square"|"triangle"|"supersaw")
//   - Supersaw (seven detuned saws) for thick pad sounds
//   - Noise sources ("white", "pink", "brown")
//   - FM synthesis with fm() and fmh() (modulation index and harmonicity)
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_11_synthesis.das

options gen2
options indenting = 4
options persistent_heap

require strudel/strudel public
require strudel/strudel_player
require audio/audio_boost
require daslib/fio

// Tiny harness — feed a pattern to the main-thread player for `seconds` seconds.
def play(var pat : Pattern; seconds : float = 4.0; cps : double = 0.5lf) {
    with_audio_system() {
        strudel_create_channel()
        strudel_set_cps(cps)
        strudel_add_track(pat)
        let t0 = ref_time_ticks()
        while (float(get_time_usec(t0)) / 1000000.0 < seconds) {
            strudel_tick()
            sleep(5u)
        }
        strudel_shutdown()
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Basic oscillators
// ──────────────────────────────────────────────────────────────────────────
//
// s("name") picks the oscillator. Seven are built in:
//   - sine:      pure tone, no harmonics
//   - triangle:  soft flute-like, odd harmonics, gentle roll-off
//   - square:    hollow woody, odd harmonics only
//   - sawtooth:  bright and buzzy, all harmonics
//   - supersaw:  five detuned sawtooths stacked, fat and wide
//   - pink:      pink noise (-3 dB/octave)
//   - white:     white noise (flat spectrum)

def example_oscillators() {
    print("=== Section 1: Oscillators (sine, triangle, square, sawtooth) ===\n")
    // Alternate one oscillator per cycle with <> — angle brackets step per cycle.
    let pat <- s("<sine triangle square sawtooth>") |> note(48.0) |> attack(0.02) |> decay(0.1) |> sustain(0.7) |> release(0.3) |> lpf(2000.0)
    play(pat, 8.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Supersaw (thick pad sound)
// ──────────────────────────────────────────────────────────────────────────
//
// The supersaw voice is seven detuned sawtooth oscillators stacked — the
// classic "trance" pad. It is much wider and fatter than a single saw. A
// low-pass filter tames the top end for a warm tone.

def example_supersaw() {
    print("\n=== Section 2: Supersaw chord ===\n")
    // C minor triad voiced across two octaves for extra depth.
    let pat <- note("c3,eb3,g3,c4", "supersaw") |> attack(0.1) |> decay(0.1) |> sustain(0.8) |> release(0.6) |> lpf(3500.0)
    play(pat, 4.0, 0.25lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Noise sources
// ──────────────────────────────────────────────────────────────────────────
//
// "white" and "pink" are pitch-less noise generators. Pink noise has more
// energy in the low end — perceptually "warmer" than white. Filter them
// to sculpt specific colours (ocean, wind, static, hi-hat).

def example_noise() {
    print("\n=== Section 3: Pink noise filtered into a pad ===\n")
    let pat <- atom("pink") |> sustain(1.0) |> lpf(400.0) |> gain(0.4)
    play(pat, 4.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — FM synthesis (fm = index, fmh = harmonicity)
// ──────────────────────────────────────────────────────────────────────────
//
// FM modulates the sine carrier's frequency with a second sine. Two knobs:
//   - fm(index):       modulation depth — higher = more harmonics
//   - fmh(harmonicity): ratio of modulator to carrier frequency
// Integer fmh values (1, 2, 3...) are "musical"; non-integer values give
// inharmonic, bell- or clang-like timbres.

def example_fm_bells() {
    print("\n=== Section 4a: FM bells (fm=1, fmh=7) ===\n")
    let pat <- note("c5", "sine") |> fm(1.0) |> fmh(7.0) |> decay(0.3) |> release(0.8)
    play(pat, 4.0)
}

def example_fm_gentle() {
    print("\n=== Section 4b: Gentle FM (fm=2, fmh=2) ===\n")
    let pat <- note("<c3 e3 g3 c4>", "sine") |> fm(2.0) |> fmh(2.0) |> attack(0.02) |> decay(0.2) |> sustain(0.6) |> release(0.3)
    play(pat, 4.0)
}

[export]
def main() {
    print("DASTRUDEL-11: Synthesis\n\n")
    example_oscillators()
    example_supersaw()
    example_noise()
    example_fm_bells()
    example_fm_gentle()
    print("\nDone!\n")
}
