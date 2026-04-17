// Tutorial DASTRUDEL-13: SF2 SoundFont Playback
//
// This tutorial covers:
//   - Loading a General MIDI SoundFont with strudel_load_sf2
//   - Selecting instruments by GM program name: sf2("piano") / sf2("flute")
//   - Selecting by MIDI program number: sf2(0) / sf2(40)
//   - The GM drum map on bank 128: sf2(0) |> sf2_bank(128)
//   - Per-note expression and velocity: sf2_expression / velocity
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_13_sf2_soundfont.das
//
// Requires: a General MIDI .sf2 file (e.g. FluidR3_GM.sf2) under
// examples/daStrudel/midi_sf2_demo/ — see that folder's README.md for
// download links. If the SoundFont is missing, SF2 events are silent.

options gen2
options indenting = 4
options persistent_heap

require strudel/strudel public
require strudel/strudel_player
require audio/audio_boost
require daslib/fio

def find_sf2() : string {
    let dir = "{get_das_root()}/examples/daStrudel/midi_sf2_demo"
    var st : FStat
    if (stat("{dir}/FluidR3_GM.sf2", st)) {
        return "{dir}/FluidR3_GM.sf2"
    }
    if (stat("{dir}/GeneralUser GS v1.471.sf2", st)) {
        return "{dir}/GeneralUser GS v1.471.sf2"
    }
    if (stat("{dir}/Musyng Kite.sf2", st)) {
        return "{dir}/Musyng Kite.sf2"
    }
    return ""
}

def play(var pat : Pattern; seconds : float = 6.0; cps : double = 0.5lf; sf2_path : string = "") {
    with_audio_system() {
        strudel_create_channel()
        strudel_set_cps(cps)
        if (!empty(sf2_path)) {
            strudel_load_sf2(sf2_path)
        }
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
// Section 1 — Instruments by name
// ──────────────────────────────────────────────────────────────────────────
//
// sf2("piano") looks up the GM preset whose name matches. Common names:
// "piano", "flute", "string_ensemble", "acoustic_bass", "electric_guitar".
// The built-in name table maps to standard GM program numbers.

def example_instruments(sf2_path : string) {
    print("=== Section 1: Piano scale ===\n")
    let pat <- note("c3 d3 e3 f3 g3 a3 b3 c4") |> sf2("piano") |> gain(0.6)
    play(pat, 8.0, 0.5lf, sf2_path)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Instruments by program number
// ──────────────────────────────────────────────────────────────────────────
//
// sf2(program) takes a GM program number (0..127). Reference:
//   0   = acoustic grand piano
//   24  = nylon guitar
//   40  = violin
//   56  = trumpet
//   73  = flute
//   81  = lead square
// Useful when you want a specific patch or the name lookup doesn't match.

def example_program_numbers(sf2_path : string) {
    print("\n=== Section 2: Program numbers (piano=0, flute=73) ===\n")
    // Stack a piano melody and a flute countermelody.
    let pat <- stack([
        note("c4 e4 g4 c5") |> sf2(0) |> gain(0.5),
        note("g4 a4 e4 g4") |> sf2(73) |> gain(0.5)
    ])
    play(pat, 8.0, 0.5lf, sf2_path)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — GM drums (bank 128)
// ──────────────────────────────────────────────────────────────────────────
//
// The GM percussion map lives on bank 128 — every MIDI note number is a
// different drum sound. Key mappings (octave 2):
//   c2  = kick (36)          fs2 = closed hi-hat (42)
//   d2  = snare (38)         as2 = open hi-hat (46)
//   eb2 = hand clap (39)     cs2 = side stick (37)
// Use sf2(0) |> sf2_bank(128) to select the drum kit.

def example_drums(sf2_path : string) {
    print("\n=== Section 3: GM drum kit on bank 128 ===\n")
    let pat <- note("c2 ~ d2 ~, [fs2 fs2] [fs2 fs2] [fs2 fs2] [fs2 fs2]") |> sf2(0) |> sf2_bank(128) |> gain(0.7)
    play(pat, 8.0, 0.5lf, sf2_path)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Expression vs velocity
// ──────────────────────────────────────────────────────────────────────────
//
// Two ways to shape per-note dynamics on SF2 patches:
//   - velocity(v):       note attack intensity (0..1) — changes timbre
//   - sf2_expression(e): continuous volume (0..1) — changes level only
// Gain is the post-mix volume. Velocity + expression together drive
// realistic phrasing on instruments that respond to both.

def example_expression(sf2_path : string) {
    print("\n=== Section 4: String pad with expression layer ===\n")
    let pat <- stack([
        // Soft strings — low expression
        note("<[c3,e3,g3] [f3,a3,c4]>") |> sf2("string_ensemble") |> sf2_expression(0.3) |> gain(0.5),
        // Full strings — high expression
        note("<[g3,b3,d4] [a3,c4,e4]>") |> sf2("string_ensemble") |> sf2_expression(1.0) |> gain(0.5)
    ])
    play(pat, 8.0, 0.5lf, sf2_path)
}

[export]
def main() {
    print("DASTRUDEL-13: SF2 SoundFont\n\n")
    let sf2_path = find_sf2()
    if (empty(sf2_path)) {
        print("No SF2 found — place a GM SoundFont under examples/daStrudel/midi_sf2_demo/\n")
        print("(SF2 events will be silent; tutorial will still run)\n\n")
    } else {
        print("Using SoundFont: {sf2_path}\n\n")
    }
    example_instruments(sf2_path)
    example_program_numbers(sf2_path)
    example_drums(sf2_path)
    example_expression(sf2_path)
    print("\nDone!\n")
}
