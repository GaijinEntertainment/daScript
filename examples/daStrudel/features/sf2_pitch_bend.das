options gen2
options indenting = 4
options persistent_heap

// Guitar notes with pitch bend — bent down, center, and bent up.
// Hear: first note bends below pitch, second is natural, third bends above — like a guitarist bending strings.
// SF2 pitch bend is daslang-only — no strudel.cc equivalent.

require feature_common

[export]
def main() {
    let pat <- stack([
        // Bent down — pitch wheel below center
        note_pattern("e3") |> set_sf2("guitar") |> set_sf2_pitch_bend(0.3) |> set_gain(0.5),
        // Center — natural pitch, no bend
        note_pattern("e3") |> set_sf2("guitar") |> set_sf2_pitch_bend(0.5) |> set_gain(0.5),
        // Bent up — pitch wheel above center
        note_pattern("e3") |> set_sf2("guitar") |> set_sf2_pitch_bend(0.7) |> set_gain(0.5)
    ])
    play_feature_cps(pat, 0.5lf) <| $ {
        strudel_load_sf2("{get_das_root()}/examples/daStrudel/midi_sf2_demo/FluidR3_GM.sf2")
    }
}
