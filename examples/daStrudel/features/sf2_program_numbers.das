options gen2
options indenting = 4
options persistent_heap

// Using numeric GM program IDs instead of instrument names.
// Program numbers: 0=acoustic_grand_piano, 73=flute, 48=string_ensemble.
// Hear: piano chords, flute melody, and string pad — same as sf2_instruments but with numbers.
// SF2 playback is a daslang-only extension.

require feature_common

[export]
def main() {
    let pat <- stack([
        // String ensemble pad — program 48
        note("<[a2,c3,e3] [f2,a2,c3] [c2,e2,g2] [g2,b2,d3]>") |> sf2(48) |> gain(0.3),
        // Flute melody — program 73
        note("<[a4 c5 e5] [f4 a4 c5] [c4 e4 g4] [g4 b4 d5]>") |> sf2(73) |> gain(0.5),
        // Piano chords — program 0
        note("<[a3,c4,e4] [f3,a3,c4] [c3,e3,g3] [g3,b3,d4]>") |> sf2(0) |> gain(0.4)
    ])
    play_feature_cps(pat, 0.5lf) {
        strudel_load_sf2("{get_das_root()}/examples/daStrudel/midi_sf2_demo/FluidR3_GM.sf2")
    }
}
