// Tutorial DASTRUDEL-06: Stacking & Combining
//
// This tutorial covers structural combinators that build a single Pattern
// out of several smaller ones:
//   - stack(...)       : play patterns simultaneously (vertical layering)
//   - cat(...)         : play patterns one cycle after another (horizontal sequencing)
//   - layer(...)       : transform the SAME pattern several ways and stack the results
//   - superimpose(fn)  : overlay a transformed copy of a pattern on top of itself
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_06_stacking_combining.das

options gen2
options indenting = 4

require strudel/strudel public
require strudel/strudel_player public
require audio/audio_boost
require daslib/fio

// ──────────────────────────────────────────────────────────────────────────
// Minimal playback harness — same as examples/daStrudel/features/feature_common.das
// (inlined here so the tutorial stands on its own).
// ──────────────────────────────────────────────────────────────────────────

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
// Section 1 — stack: play multiple patterns at the same time
// ──────────────────────────────────────────────────────────────────────────
//
// stack accepts an array of patterns. Each one runs in parallel; their
// events are merged into the same cycle. Use stack to combine independent
// voices (bass + lead + drums).

def example_stack() {
    print("\n=== Section 1: stack ===\n")
    let pat <- stack([
        note("c3 e3 g3 c4", "sine") |> sustain(0.5),
        note("c5 ~ ~ g4 ~ ~ e4 ~", "triangle") |> sustain(0.3) |> gain(0.4)
    ])
    play(pat, 0.5lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — cat: sequence patterns across cycles
// ──────────────────────────────────────────────────────────────────────────
//
// With N patterns, pattern i plays during cycle (i mod N), each stretched
// to fill exactly one cycle. Use cat to build longer arrangements out of
// short building blocks.

def example_cat() {
    print("\n=== Section 2: cat ===\n")
    let pat <- cat([
        note("c4 e4 g4 c5", "sine") |> sustain(0.4),
        note("c5 g4 e4 c4", "sine") |> sustain(0.4),
        note("c4 e4 g4 b4", "sine") |> sustain(0.4)
    ])
    play(pat, 0.5lf, 6.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — layer: stack transforms of one base pattern
// ──────────────────────────────────────────────────────────────────────────
//
// layer takes an array of already-built patterns AND a base pattern, and
// stacks them all. The idiom is to derive each layer from the same base
// (root + transposed copies) to build chords from a melody.

def example_layer_chord() {
    print("\n=== Section 3: layer (build a triad from a melody) ===\n")
    let pat <- layer([
        note("c4 e4 g4 c5", "sine") |> sustain(0.4) |> transpose(4.0),  // major third on top
        note("c4 e4 g4 c5", "sine") |> sustain(0.4) |> transpose(7.0)   // perfect fifth on top
    ], note("c4 e4 g4 c5", "sine") |> sustain(0.4))                      // root
    play(pat, 0.5lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — superimpose: overlay a transformed copy of a pattern
// ──────────────────────────────────────────────────────────────────────────
//
// superimpose(pat, fn) is equivalent to stack([pat, fn(pat)]) but reads
// more naturally in a pipe. Here we add a faster octave-up echo on top.

def example_superimpose() {
    print("\n=== Section 4: superimpose ===\n")
    let pat <- superimpose(note("c4 e4 g4 c5", "sine") |> sustain(0.4),
                           @(p) => gain(fast(transpose(p, 12.0), 2.0lf), 0.4))
    play(pat, 0.5lf)
}

[export]
def main() {
    print("DASTRUDEL-06: Stacking & Combining\n")
    example_stack()
    example_cat()
    example_layer_chord()
    example_superimpose()
    print("\nDone.\n")
}
