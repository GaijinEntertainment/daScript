options gen2
options indenting = 4
options persistent_heap

// SF2 expression control on string ensemble — two layers at different dynamic levels.
// Hear: soft strings (expression 0.3) underneath full strings (expression 1.0), showing dynamics without gain change.
// SF2 expression is a daslang-only extension.

require feature_common

[export]
def main() {
    let pat <- stack([
        // Soft strings — low expression for gentle dynamics
        note("<[c3,e3,g3] [f3,a3,c4]>") |> sf2("string_ensemble") |> sf2_expression(0.3) |> gain(0.5),
        // Full strings — high expression for rich dynamics
        note("<[g3,b3,d4] [a3,c4,e4]>") |> sf2("string_ensemble") |> sf2_expression(1.0) |> gain(0.5)
    ])
    play_feature_cps(pat, 0.5lf) {
        strudel_load_sf2("{get_das_root()}/examples/daStrudel/midi_sf2_demo/FluidR3_GM.sf2")
    }
}
