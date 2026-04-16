options gen2
options indenting = 4
options persistent_heap

// Drums and bass layered — GM percussion with acoustic bass.
// Hear: punchy kick-snare-hihat groove anchored by a walking bass line.
// SF2 is daslang-only — no strudel.cc equivalent.

require feature_common

[export]
def main() {
    let pat <- stack([
        // Drums — bank 128 GM percussion
        note_pattern("c2 ~ d2 ~, [fs2 fs2] [fs2 fs2] [fs2 fs2] [fs2 fs2]") |> set_sf2(0) |> set_sf2_bank(128) |> set_gain(0.7),
        // Bass — root notes following chord changes
        note_pattern("c2 ~ c2 ~, ~ ~ e2 ~, g2 ~ ~ g2, ~ ~ f2 ~") |> set_sf2("acoustic_bass") |> set_gain(0.6)
    ])
    play_feature_cps(pat, 0.5lf) <| $ {
        strudel_load_sf2("{get_das_root()}/examples/daStrudel/midi_sf2_demo/FluidR3_GM.sf2")
    }
}
