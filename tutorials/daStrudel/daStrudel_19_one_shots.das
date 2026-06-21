// Tutorial DASTRUDEL-19: One-shots — fire-and-forget stings on host events
//
// A strudel pattern is normally an endless loop. daStrudel adds the missing
// piece for game / tool audio: a pattern that plays ONCE and then "stops and
// dies".
//
//   • STOP  — `once(pat)` / `playFor(pat, n)` gate a pattern to its first cycle
//             (or first n cycles) and emit silence forever after. Pure Pattern
//             combinators — they compose with everything else.
//   • DIE   — `strudel_one_shot(pat, gain, quant, len)` fires `pat` as a
//             self-retiring track: it plays, goes silent, and the track removes
//             itself from the mix once every voice has finished ringing (the
//             release tail is never clipped). Returns the track index.
//
// `quant_cycles` snaps the start to the global beat grid, so a sting fired at an
// arbitrary instant still lands in time. `strudel_active_tracks()` tells you how
// many tracks are live — handy for "is the sting still ringing?".
//
// Because each one-shot is an independent track built fresh per call, the same
// mechanism scales to parametric SFX: vary the pattern per event and fire a
// different sting each time — no baked / shipped audio assets.
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_19_one_shots.das

options gen2
options persistent_heap
options indenting = 4

require strudel/strudel public
require strudel/strudel_player public
require audio/audio_boost
require daslib/fio
require math

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — once() / playFor(): the "stop" half (pure pattern, no audio)
// ──────────────────────────────────────────────────────────────────────────
//
// Querying the gated pattern directly shows the onsets vanish after the window —
// no audio device needed. `once(pat)` keeps only cycle 0; `playFor(pat, n)`
// keeps the first n cycles.

def example_gate() {
    print("\n=== Section 1: once() / playFor() gate a pattern (the STOP half) ===\n")
    let phrase <- once(note("c5 e5 g5 c6", "triangle"))
    var c0 <- phrase(TimeSpan(start = 0.0lf, stop = 1.0lf))
    var c1 <- phrase(TimeSpan(start = 1.0lf, stop = 2.0lf))
    print("once(): cycle 0 = {length(c0)} onsets (plays), cycle 1 = {length(c1)} onsets (silent forever)\n")
    delete c0
    delete c1

    let two <- playFor(note("c5", "triangle"), 2)
    var a <- two(TimeSpan(start = 0.0lf, stop = 1.0lf))
    var b <- two(TimeSpan(start = 1.0lf, stop = 2.0lf))
    var c <- two(TimeSpan(start = 2.0lf, stop = 3.0lf))
    print("playFor(.,2): cycles 0,1 = {length(a)},{length(b)} onsets; cycle 2 = {length(c)} (stopped)\n")
    delete a
    delete b
    delete c
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — strudel_one_shot(): the "die" half (fire + auto-retire)
// ──────────────────────────────────────────────────────────────────────────
//
// Fire a sting from the host loop. The track is live while it sounds, then
// auto-removes once its voices have finished ringing — strudel_active_tracks()
// drops back on its own. We never call strudel_remove_track.

def example_fire() {
    print("\n=== Section 2: strudel_one_shot fires a self-retiring sting (the DIE half) ===\n")
    with_audio_system() {
        strudel_create_channel()
        strudel_set_cps(1.0lf)
        let sting <- (note("c5 e5 g5 c6", "triangle")
            |> gain(0.8) |> room(0.5) |> orbit(2) |> attack(0.005) |> release(0.4))
        var fired = false
        let t0 = ref_time_ticks()
        while (double(get_time_usec(t0)) / 1000000.0lf < 4.0lf) {
            let elapsed = double(get_time_usec(t0)) / 1000000.0lf
            if (!fired && elapsed > 0.5lf) {
                strudel_one_shot(sting, 1.0, 0.0, 0.5)   // gain 1.0, immediate, half a cycle long
                fired = true
                print("fired — active tracks now {strudel_active_tracks()}\n")
            }
            strudel_tick()
            sleep(5u)
        }
        print("rang out — active tracks now {strudel_active_tracks()} (the track retired itself)\n")
        strudel_shutdown()
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Quantize to the beat
// ──────────────────────────────────────────────────────────────────────────
//
// `quant_cycles > 0` snaps the start to the global beat grid: 0.25 = the next
// quarter-cycle beat. The sting waits a few milliseconds so it lands musically
// in time instead of on the exact frame of the event — "more professional".

def example_quantized() {
    print("\n=== Section 3: quantize the start to the next beat ===\n")
    with_audio_system() {
        strudel_create_channel()
        strudel_set_cps(1.0lf)
        let sting <- (note("e5 g5 c6", "triangle")
            |> gain(0.8) |> room(0.4) |> orbit(2) |> attack(0.005) |> release(0.3))
        var fired = false
        let t0 = ref_time_ticks()
        while (double(get_time_usec(t0)) / 1000000.0lf < 3.0lf) {
            let elapsed = double(get_time_usec(t0)) / 1000000.0lf
            if (!fired && elapsed > 0.3lf) {
                strudel_one_shot(sting, 1.0, 0.25, 0.5)   // snap to the next 1/4-cycle beat
                fired = true
            }
            strudel_tick()
            sleep(5u)
        }
        strudel_shutdown()
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Parametric SFX: a different sting per event
// ──────────────────────────────────────────────────────────────────────────
//
// Each one-shot is a fresh, independent track, so the pattern can vary per
// event. Here a sequence of "events" of rising intensity fires a higher sting
// each time — vary pitch by impact, instrument by surface, etc. No assets.

def example_parametric() {
    print("\n=== Section 4: parametric one-shots — a new sting per event ===\n")
    with_audio_system() {
        strudel_create_channel()
        strudel_set_cps(1.0lf)
        let phrases <- ["c4 e4 g4", "e4 g4 c5", "g4 c5 e5", "c5 e5 g5"]
        var nextEvent = 0
        let t0 = ref_time_ticks()
        while (double(get_time_usec(t0)) / 1000000.0lf < 6.0lf) {
            let elapsed = double(get_time_usec(t0)) / 1000000.0lf
            if (nextEvent < length(phrases) && elapsed > double(nextEvent) * 1.2lf + 0.3lf) {
                let sting <- (note(phrases[nextEvent], "triangle")
                    |> gain(0.7) |> room(0.4) |> orbit(2) |> attack(0.005) |> release(0.3))
                strudel_one_shot(sting, 1.0, 0.0, 0.5)
                print("event {nextEvent} fired ({phrases[nextEvent]})\n")
                nextEvent ++
            }
            strudel_tick()
            sleep(5u)
        }
        strudel_shutdown()
    }
}

[export]
def main() {
    print("DASTRUDEL-19: One-shots — fire-and-forget stings on host events\n")
    example_gate()
    example_fire()
    example_quantized()
    example_parametric()
    print("\nDone.\n")
}
