// Tutorial DASTRUDEL-07: Per-Voice FX & Transforming Combinators
//
// This tutorial covers combinators that take a *transform function* and
// apply it to a copy of the pattern, producing a richer texture:
//   - jux(pat, fn)       : original goes left, fn(pat) goes right (stereo widening)
//   - off(pat, t, fn)    : delayed copy of fn(pat), offset by t cycles (canon)
//   - chunk(pat, n, fn)  : apply fn to every n-th chunk in turn
//
// It also introduces daslang's *per-voice FX* (phaser, tremolo, compressor,
// shape). Like strudel.cc, daslang applies these per event: each playing voice
// carries its own FX chain, run BEFORE the voices are mixed into the orbit, so
// two simultaneous notes can carry different settings without bleeding into each
// other. (Only room/delay/chorus are shared per-orbit send buses — see tut 08.)
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_07_per_voice_fx.das

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
// Section 1 — jux: stereo splitting via a transform
// ──────────────────────────────────────────────────────────────────────────
//
// jux duplicates the pattern, sends the original to the left channel and
// the transformed copy to the right. Common transforms: rev (reverse) and
// transpose (shift pitch). The lambda has type @(Pattern) => Pattern.

def example_jux_rev() {
    print("\n=== Section 1a: jux + rev (left = forward, right = reversed) ===\n")
    let pat <- jux(note("c4 e4 g4 c5", "sine") |> sustain(0.4), @(p) => rev(p))
    play(pat, 0.5lf)
}

def example_jux_transpose() {
    print("\n=== Section 1b: jux + transpose(7) (left = root, right = fifth above) ===\n")
    let pat <- jux(note("c4 e4 g4 c5", "sine") |> sustain(0.4),
                   @(p) => transpose(p, 7.0))
    play(pat, 0.5lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — off: canon, delayed transformed copy
// ──────────────────────────────────────────────────────────────────────────
//
// off(pat, time, fn) plays the original AND a copy of fn(pat) delayed by
// `time` cycles. With time = 0.25 and a transpose-up-an-octave transform,
// you get a classic canon at the octave.

def example_off_canon() {
    print("\n=== Section 2: off — canon at the octave, 1/4 cycle later ===\n")
    let pat <- off(note("c4 e4 g4 b4", "sine") |> sustain(0.4), 0.25lf,
                   @(p) => transpose(p, 12.0))
    play(pat, 0.5lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — chunk: apply a transform to one n-th of the pattern at a time
// ──────────────────────────────────────────────────────────────────────────
//
// chunk(pat, n, fn) splits each cycle into n equal slices and applies fn
// to slice 0 on cycle 0, slice 1 on cycle 1, etc. Below, slices take turns
// playing twice as fast.

def example_chunk_fast() {
    print("\n=== Section 3: chunk — each quarter in turn plays twice as fast ===\n")
    let pat <- chunk(note("c4 e4 g4 c5", "sine") |> sustain(0.4), 4,
                     @(p) => fast(p, 2.0lf))
    play(pat, 0.5lf, 6.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — per-voice FX: phaser
// ──────────────────────────────────────────────────────────────────────────
//
// Per-voice FX — phaser, tremolo, compressor, shape — are baked into each
// playing voice (as in strudel.cc). Because the FX live with the voice, two
// simultaneous notes in a single stack can carry DIFFERENT phaser rates without
// leaking into each other. (room/delay/chorus differ — those are per-orbit
// send buses; see tutorial 08.)
//
// The full split (shared with tutorial 08):
//   Per-voice  (independent per note): gain, pan, speed, lpf, hpf, bpf,
//              phaser, tremolo, compressor, shape, crush, coarse, djf, fm, vowel
//   Per-orbit  (one shared bus instance per orbit): room/roomsize,
//              delay/delaytime/delayfeedback, chorus

def example_per_voice_phaser() {
    print("\n=== Section 4: per-voice phaser at two different rates ===\n")
    let pat <- stack([
        note("c4", "sawtooth") |> sustain(1.0) |> phaser(0.5) |> gain(0.4),  // slow sweep
        note("g4", "sawtooth") |> sustain(1.0) |> phaser(2.0) |> gain(0.4)   // fast sweep
    ])
    play(pat, 0.5lf, 6.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 5 — probabilistic combinators: sometimes, degradeBy
// ──────────────────────────────────────────────────────────────────────────
//
// sometimes(pat, fn) applies fn to a random ~50% of events; degradeBy(prob)
// randomly DROPS events with the given probability. Both are how you turn a
// rigid loop into something that breathes — humanised fills and thinning.

def example_sometimes() {
    print("\n=== Section 5a: sometimes — half the hits play twice as fast ===\n")
    let pat <- sometimes(s("bd sd hh cp"), @(x) => fast(x, 2.0lf))
    play(pat, 0.5lf, 6.0lf)
}

def example_degrade() {
    print("\n=== Section 5b: degradeBy(0.4) — randomly drop 40% of a hi-hat run ===\n")
    let pat <- s("hh*8") |> degradeBy(0.4)
    play(pat, 0.5lf, 6.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 6 — cycle-conditional: every, when_cycle
// ──────────────────────────────────────────────────────────────────────────
//
// every(n, pat_on, pat_off) plays pat_on on cycles 0, n, 2n, ... and pat_off
// otherwise — note daslang's every takes two PATTERNS, not a transform.
// when_cycle(pat, cond, fn) applies fn only on cycles where cond(cycle) is true.

def example_every() {
    print("\n=== Section 6a: every 4th cycle, the line reverses ===\n")
    // every() moves both patterns into its closure, so build the line twice
    // (an independent copy for each branch) rather than aliasing one.
    let pat <- every(4, note("c4 e4 g4 c5", "sine") |> sustain(0.4) |> rev(),
                        note("c4 e4 g4 c5", "sine") |> sustain(0.4))
    play(pat, 0.5lf, 8.0lf)
}

def example_when_cycle() {
    print("\n=== Section 6b: when_cycle — speed up on even cycles only ===\n")
    let pat <- when_cycle(note("c4 e4 g4 c5", "sine") |> sustain(0.4),
                          @(c) => c % 2 == 0, @(x) => fast(x, 2.0lf))
    play(pat, 0.5lf, 8.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 7 — reordering & gating: shuffle, scramble, mask
// ──────────────────────────────────────────────────────────────────────────
//
// shuffle(n) cuts the cycle into n slices and plays them in a shuffled order
// (a permutation — every slice once); scramble(n) picks n slices at random
// (repeats allowed). mask(pat, bool_pat) gates events through a 1/~ pattern.

def example_shuffle_scramble() {
    print("\n=== Section 7a: shuffle vs scramble of an 8-note scale ===\n")
    let shuf <- note("c4 d4 e4 f4 g4 a4 b4 c5", "sine") |> sustain(0.4) |> shuffle(8)
    play(shuf, 0.5lf, 4.0lf)
    let scrm <- note("c4 d4 e4 f4 g4 a4 b4 c5", "sine") |> sustain(0.4) |> scramble(8)
    play(scrm, 0.5lf, 4.0lf)
}

def example_mask() {
    print("\n=== Section 7b: mask — gate a melody through \"1 ~ 1 ~\" ===\n")
    let pat <- note("c4 d4 e4 f4", "sine") |> sustain(0.4) |> mask(s("1 ~ 1 ~"))
    play(pat, 0.5lf)
}

[export]
def main() {
    print("DASTRUDEL-07: Per-Voice FX & Combinators\n")
    example_jux_rev()
    example_jux_transpose()
    example_off_canon()
    example_chunk_fast()
    example_per_voice_phaser()
    example_sometimes()
    example_degrade()
    example_every()
    example_when_cycle()
    example_shuffle_scramble()
    example_mask()
    print("\nDone.\n")
}
