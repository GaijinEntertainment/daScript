// Tutorial DASTRUDEL-05: Euclidean Rhythms
//
// This tutorial covers:
//   - euclid(pat, k, n): play pat at k evenly-spaced onsets across n steps
//   - The Bjorklund algorithm and why it sounds "musical"
//   - Polyrhythm: stacking euclid patterns with different (k, n) ratios
//   - Why this codebase uses euclid(pat, k, n) instead of bd(k,n) mini-notation
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_05_euclidean_rhythms.das

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
// Section 1 — bjorklund(k, n): the underlying rhythm
// ──────────────────────────────────────────────────────────────────────────
//
// bjorklund(3, 8) returns [x . . x . . x .] — three onsets distributed as
// evenly as possible across eight steps. This is the algorithm Toussaint
// showed maps onto traditional rhythms from many cultures (the Cuban
// tresillo (3,8), the Cumbia (3,4), the Bossa-Nova (3,8) rotated, etc.).

def example_bjorklund_print() {
    print("=== Section 1: bjorklund(3, 8) — print the raw pattern ===\n")
    let r <- bjorklund(3, 8)
    print("  ")
    for (b in r) {
        let glyph = b ? "x" : "."
        print("{glyph} ")
    }
    print("\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — euclid(pat, k, n): kick on the Euclidean grid
// ──────────────────────────────────────────────────────────────────────────
//
// euclid(pat, k, n) speeds the pattern up by n, then keeps only the haps
// that fall on the k Bjorklund onsets. The classic (3, 8) "tresillo" kick
// pattern is one line of code.

def example_tresillo() {
    print("\n=== Section 2: euclid(bd, 3, 8) — Cuban tresillo ===\n")
    let pat <- atom("bd") |> euclid(3, 8)
    play(pat, 6.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Other classic Euclidean rhythms
// ──────────────────────────────────────────────────────────────────────────
//
// (5, 8)  — Cumbia / West-African bell
// (5, 16) — Bossa-Nova clave (with rotation)
// (7, 8)  — dense, almost-straight rhythm
// (3, 4)  — three-against-four feel

def example_clave_family() {
    print("\n=== Section 3: euclid(5, 8) — Cumbia bell pattern ===\n")
    let pat <- atom("bd") |> euclid(5, 8)
    play(pat, 6.0)

    print("\n=== Section 3b: euclid(7, 8) — dense, near-straight ===\n")
    let pat2 <- atom("hh") |> euclid(7, 8)
    play(pat2, 4.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Polyrhythm: stack patterns with different (k, n) ratios
// ──────────────────────────────────────────────────────────────────────────
//
// stack() layers patterns simultaneously. Stack two Euclidean patterns
// whose n values share no common factor and you get a true polyrhythm:
// here a 3-against-8 kick under a 5-against-8 hat — the cycle aligns
// every cycle (both are length 8) but the onsets weave.

def example_polyrhythm() {
    print("\n=== Section 4: euclid(3,8) kick + euclid(5,8) hi-hat ===\n")
    let pat <- stack([
        atom("bd") |> euclid(3, 8),
        atom("hh") |> euclid(5, 8)
    ])
    play(pat, 6.0)
}

// ──────────────────────────────────────────────────────────────────────────
// A note on syntax
// ──────────────────────────────────────────────────────────────────────────
//
// In the original Tidal/Strudel mini-notation, "bd(3,8)" is the inline form
// for euclid. THIS daStrudel build does not parse parentheses inside
// mini-notation — you call euclid(pat, k, n) as a normal function. The
// fluent pipe style stays compact:
//   atom("bd") |> euclid(3, 8)
//   s("bd sd") |> euclid(5, 8)
//
// Rotation (Tidal's `bd(3,8,1)`, `euclidRot`) is not yet exposed as a
// standalone function in this build. Achieve it for now by reordering
// elements inside the pattern fed to euclid, or with rev/slow chains.

[export]
def main() {
    print("DASTRUDEL-05: Euclidean Rhythms\n\n")
    example_bjorklund_print()
    example_tresillo()
    example_clave_family()
    example_polyrhythm()
    print("\nDone!\n")
}
