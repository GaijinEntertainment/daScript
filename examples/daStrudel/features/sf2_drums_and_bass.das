options gen2
options indenting = 4
options persistent_heap

// Drums and bass layered — GM percussion with acoustic bass.
// Hear: punchy kick-snare-hihat groove anchored by a walking bass line.
// SF2 playback is a daslang-only extension.

require feature_common

[export]
def main() {
    let pat <- stack([
        // Drums — bank 128 GM percussion
        note("c2 ~ d2 ~, [fs2 fs2] [fs2 fs2] [fs2 fs2] [fs2 fs2]") |> sf2(0) |> sf2_bank(128) |> gain(0.7),
        // Bass — root notes following chord changes
        note("c2 ~ c2 ~, ~ ~ e2 ~, g2 ~ ~ g2, ~ ~ f2 ~") |> sf2("acoustic_bass") |> gain(0.6)
    ])
    play_feature_cps(pat, 0.5lf) {
        strudel_load_sf2("{get_das_root()}/examples/daStrudel/midi_sf2_demo/FluidR3_GM.sf2")
    }
}
