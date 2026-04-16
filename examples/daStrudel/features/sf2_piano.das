options gen2
options indenting = 4
options persistent_heap

// Simple ascending C major scale on acoustic piano via SF2.
// Hear: clean piano notes stepping up from C3 to C4.
// SF2 is daslang-only — no strudel.cc equivalent.

require feature_common

[export]
def main() {
    let pat <- note_pattern("c3 d3 e3 f3 g3 a3 b3 c4") |> set_sf2("piano") |> set_gain(0.6)
    play_feature_cps(pat, 0.5lf) <| $ {
        strudel_load_sf2("{get_das_root()}/examples/daStrudel/midi_sf2_demo/FluidR3_GM.sf2")
    }
}
