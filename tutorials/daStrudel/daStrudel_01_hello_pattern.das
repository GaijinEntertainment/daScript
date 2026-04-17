// Tutorial DASTRUDEL-01: Hello Pattern
//
// This tutorial covers:
//   - Your first Pattern: s("bd"), note("c4"), and pure()
//   - The Pattern type — a pure function from time span to events (Haps)
//   - Querying a Pattern by hand to see what a Hap looks like
//   - Playing a Pattern through the audio system
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_01_hello_pattern.das

options gen2
options indenting = 4
options persistent_heap

require strudel/strudel public
require strudel/strudel_player
require audio/audio_boost
require daslib/fio

// Tiny harness — feed a pattern to the main-thread player for `seconds` seconds.
// The full version (with WAV rendering, memory tracking, etc.) lives in
// examples/daStrudel/features/feature_common.das.
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
// Section 1 — What is a Pattern? Querying one by hand
// ──────────────────────────────────────────────────────────────────────────
//
// A Pattern is a pure function from a TimeSpan (a query window) to an
// array of Haps (events that fall within that window). No side effects,
// no audio — just data. Time is measured in *cycles*, not seconds.
//
// pure(Event) lifts a single Event into a Pattern that emits it once
// per cycle.

def example_query_by_hand() {
    print("=== Section 1: Query a Pattern by hand ===\n")
    let pat <- pure(Event(s = "bd"))
    var haps <- invoke(pat, TimeSpan(start = 0.0lf, stop = 1.0lf))
    print("  Querying [0, 1) returned {length(haps)} hap(s):\n")
    for (h in haps) {
        print("    whole=[{h.whole.start}, {h.whole.stop})  s='{h.value.s}'\n")
    }
    delete haps
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — s("bd"): one drum hit per cycle
// ──────────────────────────────────────────────────────────────────────────
//
// s() parses mini-notation and builds a Pattern of sound events. Even the
// simplest input — a single sound name — is valid mini-notation: it produces
// one hap per cycle, just like pure() did, but typed by the parser.

def example_s_one_sound() {
    print("\n=== Section 2: s(\"bd\") — one bass-drum hit per cycle ===\n")
    let pat <- s("bd")
    play(pat, 4.0)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — note("c4"): one synth note per cycle
// ──────────────────────────────────────────────────────────────────────────
//
// note() is the synth counterpart of s(). It parses note names ("c4",
// "eb3", "g#5") into MIDI numbers. The second arg picks an oscillator;
// "sine" is the default.

def example_note_one_note() {
    print("\n=== Section 3: note(\"c4\") — one sine note per cycle ===\n")
    let pat <- note("c4", "sine") |> sustain(0.5)
    play(pat, 4.0)
}

[export]
def main() {
    print("DASTRUDEL-01: Hello Pattern\n\n")
    example_query_by_hand()
    example_s_one_sound()
    example_note_one_note()
    print("\nDone!\n")
}
