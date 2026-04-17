// Tutorial DASTRUDEL-03: Mini-Notation Advanced
//
// This tutorial covers:
//   - Angle brackets <a b c> — pick one element per cycle (alternation)
//   - Elongation @N — make an element occupy N units of its parent
//   - Degrade ? — randomly drop events with 50% probability
//   - Replicate !N — repeat an element N times in its parent sequence
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_03_mini_notation_advanced.das

options gen2
options indenting = 4
options persistent_heap

require strudel/strudel public
require strudel/strudel_player
require audio/audio_boost
require daslib/fio

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
// Section 1 — Alternation with < >
// ──────────────────────────────────────────────────────────────────────────
//
// Angle brackets pick ONE element per cycle, advancing on each cycle.
// "<bd sd cp>" plays bd on cycle 0, sd on cycle 1, cp on cycle 2, then
// loops. Inside a sequence, the angle group itself takes one step.

def example_alternation() {
    print("=== Section 1: \"<bd sd cp>\" — one per cycle ===\n")
    let pat <- s("<bd sd cp>")
    play(pat, 6.0)
}

def example_alternation_in_sequence() {
    print("\n=== Section 1b: \"bd <sd cp> hh hh\" — alternation inside a sequence ===\n")
    // The <sd cp> slot alternates each cycle: snare on cycle 0, clap on cycle 1.
    let pat <- s("bd <sd cp> hh hh")
    play(pat, 4.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Elongation with @N
// ──────────────────────────────────────────────────────────────────────────
//
// Postfix @N makes an element occupy N units of the parent sequence
// (default 1). "bd@3 sd" gives the kick three quarters of the cycle and
// the snare the last quarter — without elongation each would get half.

def example_elongation() {
    print("\n=== Section 2: \"bd@3 sd\" — kick takes 3/4 of the cycle ===\n")
    let pat <- s("bd@3 sd") |> sustain(0.5)
    play(pat, 4.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Degrade with ?
// ──────────────────────────────────────────────────────────────────────────
//
// Postfix ? randomly drops events at the marked element with 50% probability.
// "bd hh? sd hh?" plays the snare and kick reliably; the hi-hats appear
// roughly half the time, randomly per cycle. Great for adding human feel.

def example_degrade() {
    print("\n=== Section 3: \"bd hh? sd hh?\" — humanised hi-hats ===\n")
    let pat <- s("bd hh? sd hh?")
    play(pat, 6.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Replicate with !N
// ──────────────────────────────────────────────────────────────────────────
//
// Postfix !N is like *N but repeats the element across N parent slots
// instead of squeezing them into one. "bd!3 sd" expands to "bd bd bd sd"
// — four equal slots, three kicks then a snare.

def example_replicate() {
    print("\n=== Section 4: \"bd!3 sd\" — three kicks then a snare ===\n")
    let pat <- s("bd!3 sd")
    play(pat, 4.0)
}

[export]
def main() {
    print("DASTRUDEL-03: Mini-Notation Advanced\n\n")
    example_alternation()
    example_alternation_in_sequence()
    example_elongation()
    example_degrade()
    example_replicate()
    print("\nDone!\n")
}
