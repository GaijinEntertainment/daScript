// Tutorial DASTRUDEL-09: ADSR & Envelope Shaping
//
// An ADSR envelope shapes a note's amplitude over time:
//   Attack  — time from key-down to peak
//   Decay   — time to drop from peak to the sustain level
//   Sustain — held level (0..1) while the note is on
//   Release — time to fade to silence after key-up
//
// daslang's strudel uses TEMPO-AWARE defaults: each ADSR field defaults to
// a -1 sentinel meaning "unset". A resolver applies sensible defaults at
// playback time:
//   * nothing set            → held tone (sustain = 1, tiny edges)
//   * decay set, no sustain  → percussion (sustain = 0)
//   * sustain set explicitly → always wins
// This lets `note(...).s("sine")` ring out for the full note, instead of
// decaying to silence in 50ms like older defaults.
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_09_adsr_envelopes.das

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
// Section 1 — Default envelope: held tone
// ──────────────────────────────────────────────────────────────────────────
//
// With no ADSR setters at all, the resolver picks a held-tone envelope:
// quick attack, full sustain, short release. The note rings for its full
// scheduled duration.

def example_default_held() {
    print("\n=== Section 1: default envelope (held tone) ===\n")
    let pat <- note("c4 e4 g4 c5", "sine")
    play(pat, 0.5lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Plucky percussion: decay only
// ──────────────────────────────────────────────────────────────────────────
//
// Set decay alone and the resolver assumes a percussive envelope: sustain
// drops to 0, so each note "plucks" then decays away. This is the idiom
// for short, rhythmic synth blips.

def example_pluck() {
    print("\n=== Section 2: pluck — decay only, sustain auto-zero ===\n")
    let pat <- note("c4 e4 g4 c5", "sine") |> decay(0.15)
    play(pat, 0.5lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Explicit ADSR: pad with slow attack and release
// ──────────────────────────────────────────────────────────────────────────
//
// Setting all four fields gives full manual control. A pad-style envelope
// uses a long attack so the note swells in, plus a long release so it
// fades out gradually after key-up.

def example_pad() {
    print("\n=== Section 3: pad — explicit slow attack and release ===\n")
    let pat <- note("c3", "triangle") |> attack(0.5) |> decay(0.2) |> sustain(0.6) |> release(0.5)
    play(pat, 0.25lf, 6.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Tempo-aware delay defaults
// ──────────────────────────────────────────────────────────────────────────
//
// The same "unset sentinel + resolver" idea applies to delaytime. Plain
// delay(x) without delaytime(...) derives delaytime from delaysync (cycles)
// and the current cps. The 3/16 cycle default gives a dotted-eighth feel
// that automatically tracks tempo changes.
//
// Below: same pattern at two different cps values — the echo timing
// follows the tempo.

def example_tempo_delay() {
    print("\n=== Section 4a: tempo-aware delay at cps = 0.5 ===\n")
    let pat_slow <- note("c4 ~ e4 ~ g4 ~", "sine") |> decay(0.15) |> delay(0.6)
    play(pat_slow, 0.5lf, 4.0lf)

    print("\n=== Section 4b: same pattern at cps = 1.0 — echoes track tempo ===\n")
    let pat_fast <- note("c4 ~ e4 ~ g4 ~", "sine") |> decay(0.15) |> delay(0.6)
    play(pat_fast, 1.0lf, 4.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 5 — Velocity via gain on top of ADSR
// ──────────────────────────────────────────────────────────────────────────
//
// gain multiplies the envelope output. Use it for a velocity curve over a
// pattern: alternating loud and soft hits while the envelope shape stays
// constant.

def example_velocity_curve() {
    print("\n=== Section 5: velocity (gain) on top of a fixed envelope ===\n")
    let pat <- stack([
        note("c4", "sine") |> decay(0.15) |> gain(0.9),
        note("e4", "sine") |> decay(0.15) |> gain(0.4),
        note("g4", "sine") |> decay(0.15) |> gain(0.7),
        note("c5", "sine") |> decay(0.15) |> gain(0.3)
    ])
    play(pat, 0.5lf)
}

[export]
def main() {
    print("DASTRUDEL-09: ADSR & Envelope Shaping\n")
    example_default_held()
    example_pluck()
    example_pad()
    example_tempo_delay()
    example_velocity_curve()
    print("\nDone.\n")
}
