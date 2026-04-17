// Tutorial DASTRUDEL-02: Mini-Notation Fundamentals
//
// This tutorial covers:
//   - Sequences: "bd sd ~ cp" — events spaced evenly across one cycle
//   - The rest token: ~ (silence)
//   - Subdivisions: [a b] — squeeze a sub-sequence into one parent step
//   - Repeats: x*N — play x N times within its step
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_02_mini_notation_fundamentals.das

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
// Section 1 — A sequence with rests
// ──────────────────────────────────────────────────────────────────────────
//
// "bd sd ~ cp" is the canonical four-step pattern: kick, snare, REST, clap.
// Tokens are split on whitespace. ~ is the rest (silence) token. Each step
// occupies an equal slice of the cycle (1/4 here).

def example_sequence_with_rest() {
    print("=== Section 1: \"bd sd ~ cp\" — kick, snare, rest, clap ===\n")
    let pat <- s("bd sd ~ cp")
    play(pat, 4.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Subdivisions with [ ]
// ──────────────────────────────────────────────────────────────────────────
//
// Square brackets group tokens into a sub-sequence that fits inside ONE step
// of the parent. "bd [sd sd] ~ cp" plays kick, then two snares packed into
// the second slot, then rest, then clap. Brackets nest arbitrarily.

def example_subdivision() {
    print("\n=== Section 2: \"bd [sd sd] ~ cp\" — two snares in one step ===\n")
    let pat <- s("bd [sd sd] ~ cp")
    play(pat, 4.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Repeats with *N
// ──────────────────────────────────────────────────────────────────────────
//
// The `*N` postfix is shorthand for repeating a token N times within its
// step. "bd hh*4 sd hh" plays a kick, four hi-hats packed into one step,
// snare, then one hi-hat — equivalent to "bd [hh hh hh hh] sd hh".

def example_repeat() {
    print("\n=== Section 3: \"bd hh*4 sd hh\" — four hats in one step ===\n")
    let pat <- s("bd hh*4 sd hh")
    play(pat, 4.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Combining the basics
// ──────────────────────────────────────────────────────────────────────────
//
// All three primitives nest and combine freely. Here a kick, a snare with
// two ghosted snares packed in, a rest, and four hats — a busy backbeat.

def example_combined() {
    print("\n=== Section 4: \"bd [sd sd*2] ~ hh*4\" ===\n")
    let pat <- s("bd [sd sd*2] ~ hh*4")
    play(pat, 4.0)
}

[export]
def main() {
    print("DASTRUDEL-02: Mini-Notation Fundamentals\n\n")
    example_sequence_with_rest()
    example_subdivision()
    example_repeat()
    example_combined()
    print("\nDone!\n")
}
