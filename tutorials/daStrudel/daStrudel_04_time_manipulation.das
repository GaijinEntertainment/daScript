// Tutorial DASTRUDEL-04: Time Manipulation
//
// This tutorial covers:
//   - fast(N): squeeze N copies of the pattern into one cycle
//   - slow(N): stretch one copy of the pattern over N cycles
//   - rev: reverse event positions within each cycle
//   - hurry(N): like fast(N), but also pitch-shifts samples up
//   - ply / iter / iterBack / palindrome: per-event and per-cycle restructuring
//   - linger / compress: loop a fraction / squeeze into a window
//   - striate / chop: granular sample slicing
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_04_time_manipulation.das

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
// Section 1 — fast(N): denser, faster
// ──────────────────────────────────────────────────────────────────────────
//
// fast(2) repeats the pattern twice per cycle. Internally it scales the
// query span by N before passing it to the inner pattern, then divides
// every returned hap's timing back by N.

def example_fast() {
    print("=== Section 1: fast(2) — pattern plays twice per cycle ===\n")
    let pat <- s("c4 e4 g4 c5") |> fast(2.0lf)
    play(pat, 4.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — slow(N): sparser, slower
// ──────────────────────────────────────────────────────────────────────────
//
// slow(N) is exactly fast(1/N). One full pass of the pattern now takes N
// cycles. Useful for stretching melodies or making slow LFO-style patterns.

def example_slow() {
    print("\n=== Section 2: slow(2) — pattern takes 2 cycles to complete ===\n")
    let pat <- s("bd sd hh cp") |> slow(2.0lf)
    play(pat, 6.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — rev: reverse the cycle
// ──────────────────────────────────────────────────────────────────────────
//
// rev mirrors the query span within each cycle, queries the inner pattern
// at the mirrored position, then mirrors the results back. The result:
// "c4 e4 g4 c5" plays backward as "c5 g4 e4 c4". Per-cycle reversal — not
// a global reverse. Combine with jux for instant stereo widening (see
// tutorial 07: combinators).

def example_rev() {
    print("\n=== Section 3: rev — \"c4 e4 g4 c5\" played backward ===\n")
    let pat <- note("c4 e4 g4 c5", "sine") |> sustain(0.4) |> rev
    play(pat, 4.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — hurry(N): fast plus pitch shift
// ──────────────────────────────────────────────────────────────────────────
//
// hurry(N) speeds time up by N (like fast) AND multiplies the per-event
// playback `speed` by N. For sample-based sounds this lifts pitch by
// log2(N) octaves. Hear: hurry(2) doubles tempo and pitches an octave up.

def example_hurry() {
    print("\n=== Section 4: hurry(2) — twice as fast and an octave higher ===\n")
    let pat <- s("c3 e3 g3 c4") |> hurry(2.0lf)
    play(pat, 4.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 5 — The time algebra: composing transforms
// ──────────────────────────────────────────────────────────────────────────
//
// All four operate on the SAME thing — the query timeline. They compose
// freely: rev(slow(2, p)) reverses a slowed pattern; fast(2, rev(p)) plays
// the reversed pattern twice per cycle. Order matters, but the result is
// always another Pattern.

def example_composition() {
    print("\n=== Section 5: rev |> slow(2) — reversed and stretched ===\n")
    let pat <- note("c4 e4 g4 b4", "sine") |> sustain(0.4) |> rev |> slow(2.0lf)
    play(pat, 6.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 6 — ply(N): repeat each event in place
// ──────────────────────────────────────────────────────────────────────────
//
// ply(N) plays each event N times back-to-back inside its own slot. Unlike
// fast (which repeats the WHOLE pattern), ply subdivides each note locally —
// a stutter effect. "c4 e4 g4 c5" |> ply(3) gives nine notes, three per slot.

def example_ply() {
    print("\n=== Section 6: ply(3) — each note stuttered three times ===\n")
    let pat <- note("c4 e4 g4 c5", "sine") |> sustain(0.3) |> ply(3)
    play(pat, 4.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 7 — iter / iterBack / palindrome: per-cycle rotation
// ──────────────────────────────────────────────────────────────────────────
//
// iter(N) rotates the pattern start by 1/N each cycle, walking through every
// rotation over N cycles. iterBack(N) rotates the other direction. palindrome
// alternates the pattern forward then reversed on successive cycles.

def example_iter() {
    print("\n=== Section 7: iter(4) — phrase rotates start point each cycle ===\n")
    let pat <- note("c4 e4 g4 a4", "sine") |> iter(4)
    play(pat, 8.0)
}

def example_iter_back() {
    print("\n=== Section 7b: iterBack(4) — rotation in reverse direction ===\n")
    let pat <- note("c4 e4 g4 a4", "sine") |> iterBack(4)
    play(pat, 8.0)
}

def example_palindrome() {
    print("\n=== Section 7c: palindrome — forward then reversed each cycle ===\n")
    let pat <- note("c4 e4 g4 c5", "sine") |> palindrome
    play(pat, 8.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 8 — linger / compress: looping and windowing
// ──────────────────────────────────────────────────────────────────────────
//
// linger(t) loops the first fraction t of the cycle, filling the rest by
// repeating it. compress(b,e) squeezes the whole pattern into the [b,e)
// window of the cycle, leaving silence before b and after e.

def example_linger() {
    print("\n=== Section 8: linger(0.5) — loop the first half of the phrase ===\n")
    let pat <- note("c4 e4 g4 c5 a4 g4 e4 c4", "sine") |> linger(0.5lf)
    play(pat, 4.0)
}

def example_compress() {
    print("\n=== Section 8b: compress(0.25, 0.75) — play only the middle half ===\n")
    let pat <- note("c4 e4 g4 c5", "sine") |> sustain(0.3) |> compress(0.25lf, 0.75lf)
    play(pat, 4.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 9 — striate / chop: granular sample slicing
// ──────────────────────────────────────────────────────────────────────────
//
// striate(N) cuts a sample into N pieces and spreads those pieces across the
// whole cycle. chop(N) also cuts into N pieces but keeps the slices inside
// each event's own slot. Both need a sample-ish source — here the synth "bd".

def example_striate() {
    print("\n=== Section 9: striate(4) — sample sliced across the cycle ===\n")
    let pat <- s("bd sd hh cp") |> striate(4) |> gain(0.4)
    play(pat, 4.0)
}

def example_chop() {
    print("\n=== Section 9b: chop(4) — each event cut into 4 in-place slices ===\n")
    let pat <- s("bd") |> chop(4) |> gain(0.6)
    play(pat, 4.0)
}

[export]
def main() {
    print("DASTRUDEL-04: Time Manipulation\n\n")
    example_fast()
    example_slow()
    example_rev()
    example_hurry()
    example_composition()
    example_ply()
    example_iter()
    example_iter_back()
    example_palindrome()
    example_linger()
    example_compress()
    example_striate()
    example_chop()
    print("\nDone!\n")
}
