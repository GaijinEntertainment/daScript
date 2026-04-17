options gen2
options indenting = 4
options persistent_heap

// Basic drum pattern using GM percussion bank 128.
// GM drum note mapping: c2=kick(36), d2=snare(38), eb2=clap(39),
//   fs2=closed_hh(42), as2=open_hh(46), cs2=side_stick(37).
// Hear: four-on-the-floor kick, snare on 2 and 4, steady closed hi-hats.
// SF2 playback is a daslang-only extension.

require feature_common

[export]
def main() {
    let pat <- note("c2 ~ d2 ~, [fs2 fs2] [fs2 fs2] [fs2 fs2] [fs2 fs2]") |> sf2(0) |> sf2_bank(128) |> gain(0.7)
    play_feature_cps(pat, 0.5lf) {
        strudel_load_sf2("{get_das_root()}/examples/daStrudel/midi_sf2_demo/FluidR3_GM.sf2")
    }
}
