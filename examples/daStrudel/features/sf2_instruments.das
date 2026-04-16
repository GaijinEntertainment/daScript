options gen2
options indenting = 4
options persistent_heap

// Three GM instruments layered: string ensemble pad, flute melody, piano chords.
// Hear: lush strings sustain Am-F-C-G, flute arpeggiates over them, piano punctuates chords.
// SF2 is daslang-only — no strudel.cc equivalent.

require feature_common

[export]
def main() {
    let pat <- stack([
        // String ensemble pad — sustained chords, Am-F-C-G
        note_pattern("<[a2,c3,e3] [f2,a2,c3] [c2,e2,g2] [g2,b2,d3]>") |> set_sf2("string_ensemble") |> set_gain(0.3),
        // Flute melody — arpeggiated notes over the chords
        note_pattern("<[a4 c5 e5] [f4 a4 c5] [c4 e4 g4] [g4 b4 d5]>") |> set_sf2("flute") |> set_gain(0.5),
        // Piano chords — block chords for harmonic support
        note_pattern("<[a3,c4,e4] [f3,a3,c4] [c3,e3,g3] [g3,b3,d4]>") |> set_sf2("piano") |> set_gain(0.4)
    ])
    play_feature_cps(pat, 0.5lf) <| $ {
        strudel_load_sf2("{get_das_root()}/examples/daStrudel/midi_sf2_demo/FluidR3_GM.sf2")
    }
}
