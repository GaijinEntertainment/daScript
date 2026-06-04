// Tutorial DASTRUDEL-11: Scales & Music Theory
//
// Scales let you write melodies in terms of degrees (0, 1, 2, ...) instead
// of absolute pitches. Combined with a scale name like "C4:major" or
// "D:dorian", the same degree pattern produces wildly different moods.
//
// Topics:
//   - n("0 2 4") |> scale("C4:major")        — degrees against a scale
//   - scale_pattern("0 1 2", "C4:minor")     — shorthand: numeric pattern + scale + sound
//   - modes: dorian, mixolydian, lydian, phrygian, locrian
//   - transposition by switching the scale root or by transpose(semitones)
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_11_scales_music_theory.das

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
// Section 1 — Major vs minor: same degrees, different mood
// ──────────────────────────────────────────────────────────────────────────
//
// scale_pattern(notation, scale_def, sound) is the shorthand for
//   n(notation) |> scale(scale_def) |> sound(sound)
// Run the same degree sequence "0 2 4 6" against C major and C minor and
// hear how the third and sixth shift the mood.

def example_major_vs_minor() {
    print("\n=== Section 1: major vs minor on the same degrees ===\n")
    let pat <- stack([
        scale_pattern("0 2 4 6", "C4:major") |> decay(0.2) |> gain(0.5),
        scale_pattern("0 2 4 6", "C4:minor", "sawtooth") |> sustain(0.3) |> release(0.2) |> gain(0.3) |> lpf(2000.0)
    ])
    play(pat, 0.5lf, 4.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Pentatonic: only "safe" notes
// ──────────────────────────────────────────────────────────────────────────
//
// "C4:major:pentatonic" maps degrees 0..4 onto C, D, E, G, A — a five-note
// scale with no semitone clashes, so any degree pattern always sounds
// consonant. Useful when you want guaranteed-pretty arpeggios.

def example_pentatonic() {
    print("\n=== Section 2: C major pentatonic — degrees 0..4 ===\n")
    let pat <- scale_pattern("0 1 2 3 4", "C4:major:pentatonic") |> decay(0.1) |> release(0.2)
    play(pat, 0.5lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Modes: same root, different intervals
// ──────────────────────────────────────────────────────────────────────────
//
// Modes are seven-note scales that share the white-keys interval pattern
// but start on a different degree. dorian (minor with raised 6th) sounds
// jazzy/folksy; mixolydian (major with lowered 7th) sounds bluesy.

def example_modes() {
    print("\n=== Section 3a: D dorian — minor with a bright 6th ===\n")
    let dorian_pat <- scale_pattern("0 1 2 3 4 5 6 7", "D4:dorian") |> decay(0.15) |> release(0.2)
    play(dorian_pat, 0.5lf, 5.0lf)

    print("\n=== Section 3b: G mixolydian — major with a flat 7th ===\n")
    let mixo_pat <- scale_pattern("0 1 2 3 4 5 6 7", "G4:mixolydian") |> decay(0.15) |> release(0.2)
    play(mixo_pat, 0.5lf, 5.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Transposition
// ──────────────────────────────────────────────────────────────────────────
//
// Two ways to transpose:
//   1. Change the scale root: "C4:major" → "E4:major" — the degrees stay
//      the same but the actual pitches shift.
//   2. transpose(semitones) — applied after pitch resolution, shifts every
//      note by the given number of semitones.

def example_transpose() {
    print("\n=== Section 4: melody played in C, then transposed up a fifth ===\n")
    let pat <- stack([
        scale_pattern("0 2 4 2", "C4:major") |> decay(0.15) |> gain(0.5),
        scale_pattern("0 2 4 2", "C4:major") |> transpose(7.0) |> decay(0.15) |> gain(0.5)
    ])
    play(pat, 0.5lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 5 — Two octaves, same pentatonic
// ──────────────────────────────────────────────────────────────────────────
//
// You can stack two scale_pattern voices at different octaves to build a
// bass + melody pair from one scale. Switch the root (C4 vs C5) to move
// between octaves while keeping the degree pattern.

def example_two_octaves() {
    print("\n=== Section 5: bass + lead in C major pentatonic, two octaves ===\n")
    let pat <- stack([
        scale_pattern("0 2 4 3", "C4:major:pentatonic", "sawtooth") |> sustain(0.3) |> release(0.1) |> lpf(500.0) |> gain(0.4),
        scale_pattern("4 2 3 0", "C5:major:pentatonic") |> decay(0.05) |> release(0.2) |> delay(0.3) |> delayfeedback(0.3) |> gain(0.5)
    ])
    play(pat, 0.5lf, 5.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 6 — degree_to_note: the primitive under scale()
// ──────────────────────────────────────────────────────────────────────────
//
// scale() is built on degree_to_note(degree, root_midi, intervals), which
// converts one scale degree to a MIDI note. Degree 0 = root, degree 7 wraps
// up an octave, negative degrees wrap backwards. get_scale_intervals_by_name
// returns the semitone table for a named scale, so you can resolve degrees
// by hand and feed the MIDI numbers straight into note().

def example_degree_to_note() {
    print("\n=== Section 6: degree_to_note maps degrees to MIDI notes ===\n")
    let root = 60   // C4
    let intervals <- get_scale_intervals_by_name("major")
    // Resolve a 0..7 run by hand and print the MIDI numbers.
    for (deg in range(0, 8)) {
        let midi = degree_to_note(deg, root, intervals)
        print("  degree {deg} -> MIDI {int(midi)}\n")
    }
    // Play degrees 0,2,4,7 resolved through the same primitive.
    let n0 = degree_to_note(0, root, intervals)
    let n2 = degree_to_note(2, root, intervals)
    let n4 = degree_to_note(4, root, intervals)
    let n7 = degree_to_note(7, root, intervals)
    let pat <- s("sine sine sine sine") |> note(note("{int(n0)} {int(n2)} {int(n4)} {int(n7)}")) |> decay(0.15) |> release(0.2)
    play(pat, 0.5lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 7 — add: shift notes by semitones
// ──────────────────────────────────────────────────────────────────────────
//
// add(pat, n) adds n semitones to every event's note — the chromatic-shift
// alias of transpose. run(8) produces a 0..7 ramp across the cycle; add(48)
// lifts it into the audible C3..G3 range as a rising chromatic line.

def example_add() {
    print("\n=== Section 7: add() shifts a degree ramp into the audible range ===\n")
    let pat <- run(8) |> add(48.0) |> sound("sine") |> sustain(0.3)
    play(pat, 0.5lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 8 — freq: pitch directly in Hz
// ──────────────────────────────────────────────────────────────────────────
//
// note() derives the playback frequency from a MIDI number via note_to_freq.
// freq() bypasses that path entirely and sets an absolute frequency in Hz, so
// you can use tuning systems or raw frequency sweeps that have no MIDI name.
// Here a sawtooth steps through 220 / 277 / 330 / 440 Hz (an A-major-ish chord).

def example_freq() {
    print("\n=== Section 8: freq() sets pitch directly in Hz ===\n")
    let pat <- (
        s("sawtooth*4")
        |> freq(note("220 277 330 440"))
        |> lpf(2500.0) |> release(0.2) |> gain(0.5)
    )
    play(pat, 0.5lf)
}

[export]
def main() {
    print("DASTRUDEL-11: Scales & Music Theory\n")
    example_major_vs_minor()
    example_pentatonic()
    example_modes()
    example_transpose()
    example_two_octaves()
    example_degree_to_note()
    example_add()
    example_freq()
    print("\nDone.\n")
}
