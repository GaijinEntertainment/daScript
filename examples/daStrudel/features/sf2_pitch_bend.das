options gen2
options indenting = 4
options persistent_heap

// Guitar notes with pitch bend — bent down, center, and bent up.
// Hear: first note bends below pitch, second is natural, third bends above — like a guitarist bending strings.
// SF2 pitch bend is a daslang-only extension.

require feature_common

[export]
def main() {
    let pat <- stack([
        // Bent down — pitch wheel below center
        note("e3") |> sf2("guitar") |> sf2_pitch_bend(0.3) |> gain(0.5),
        // Center — natural pitch, no bend
        note("e3") |> sf2("guitar") |> sf2_pitch_bend(0.5) |> gain(0.5),
        // Bent up — pitch wheel above center
        note("e3") |> sf2("guitar") |> sf2_pitch_bend(0.7) |> gain(0.5)
    ])
    play_feature_cps(pat, 0.5lf) {
        strudel_load_sf2("{get_das_root()}/examples/daStrudel/midi_sf2_demo/FluidR3_GM.sf2")
    }
}
