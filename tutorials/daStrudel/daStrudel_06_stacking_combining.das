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
options persistent_heap
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

// ──────────────────────────────────────────────────────────────────────────
// Section 5 — fastcat: squeeze all patterns into ONE cycle
// ──────────────────────────────────────────────────────────────────────────
//
// cat gives each pattern a whole cycle in turn; fastcat squeezes ALL of
// them into a single cycle, side by side. With N patterns each fills 1/N
// of every cycle. It is exactly fast(cat(pats), N).

def example_fastcat() {
    print("\n=== Section 5: fastcat ===\n")
    let pat <- fastcat([
        s("bd"),
        s("hh"),
        s("sd"),
        s("hh")
    ])
    play(pat, 0.5lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 6 — randcat / chooseCycles: pick one pattern per cycle
// ──────────────────────────────────────────────────────────────────────────
//
// randcat picks one of the patterns at random each cycle (deterministic —
// the same cycle index always picks the same pattern). chooseCycles is an
// alias for the same per-cycle behaviour; both are aliases of choose in this
// build. Use them to vary a phrase from cycle to cycle without writing out
// every variation by hand.

def example_randcat() {
    print("\n=== Section 6: randcat / chooseCycles ===\n")
    let pat <- randcat([
        note("c4 e4 g4 c5", "sine") |> sustain(0.4),
        note("c4 g4 e4 c4", "sine") |> sustain(0.4),
        note("c4 d4 e4 f4", "sine") |> sustain(0.4)
    ])
    play(pat, 0.5lf, 6.0lf)

    // chooseCycles is the same per-cycle pick under a different name.
    let pat2 <- chooseCycles([
        s("bd hh sd hh"),
        s("bd bd sd hh")
    ])
    play(pat2, 0.5lf, 4.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 7 — choose: pick one pattern per cycle (randcat's base)
// ──────────────────────────────────────────────────────────────────────────
//
// In this build choose performs the same discrete, per-cycle random pick
// that randcat and chooseCycles do — randcat / chooseCycles are aliases of
// choose. (Some pattern libraries make choose a continuous per-event signal;
// daStrudel keeps it per-cycle so note patterns do not glitch.)

def example_choose() {
    print("\n=== Section 7: choose ===\n")
    let pat <- choose([
        note("c4 e4 g4", "sine") |> sustain(0.4),
        note("d4 f4 a4", "sine") |> sustain(0.4),
        note("e4 g4 b4", "sine") |> sustain(0.4)
    ])
    play(pat, 0.5lf, 6.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 8 — wchoose: weighted per-cycle pick
// ──────────────────────────────────────────────────────────────────────────
//
// wchoose takes TWO arrays — the patterns, then a parallel array of float
// weights. A pattern's chance of being chosen each cycle is its weight over
// the sum of all weights. Here bd is three times as likely as cp or sd.

def example_wchoose() {
    print("\n=== Section 8: wchoose ===\n")
    let pat <- wchoose([
        s("bd*4"),
        s("cp*4"),
        s("sd*4")
    ], [3.0, 1.0, 1.0])
    play(pat, 0.5lf, 6.0lf)
}

[export]
def main() {
    print("DASTRUDEL-06: Stacking & Combining\n")
    example_stack()
    example_cat()
    example_layer_chord()
    example_superimpose()
    example_fastcat()
    example_randcat()
    example_choose()
    example_wchoose()
    print("\nDone.\n")
}
