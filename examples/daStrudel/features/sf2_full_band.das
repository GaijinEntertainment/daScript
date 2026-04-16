options gen2
options indenting = 4
options persistent_heap

// Full band arrangement: drums, bass, piano chords, flute melody.
// Am-F-C-G progression with four SF2 layers via stack.
// Hear: complete lo-fi chill track — kick-snare groove, walking bass, jazzy piano, soaring flute.
// SF2 is daslang-only — no strudel.cc equivalent.

require feature_common

[export]
def main() {
    let pat <- stack([
        // Drums — GM bank 128: kick, clap, closed hi-hats
        note_pattern("c2 ~ d2 ~, ~ eb2 ~ eb2, [fs2 fs2] [fs2 fs2] [fs2 fs2] [fs2 fs2]") |> set_sf2(0) |> set_sf2_bank(128) |> set_gain(0.7),
        // Bass — acoustic bass following Am-F-C-G roots
        note_pattern("<[a1 ~ ~ a1 ~ a1 ~ ~] [f1 ~ ~ f1 ~ f1 ~ ~] [c2 ~ ~ c2 ~ c2 ~ ~] [g1 ~ ~ g1 ~ g1 ~ ~]>") |> set_sf2("acoustic_bass") |> set_gain(0.6),
        // Piano — block chords, Am-F-C-G
        note_pattern("<[a3,c4,e4] [f3,a3,c4] [c3,e3,g3] [g3,b3,d4]>") |> set_sf2("piano") |> set_gain(0.4),
        // Flute — arpeggiated melody over the progression
        note_pattern("<[a4 c5 e5 a5] [f4 a4 c5 f5] [c4 e4 g4 c5] [g4 b4 d5 g5]>") |> set_sf2("flute") |> set_gain(0.45)
    ])
    play_feature_cps(pat, 0.5lf) <| $ {
        strudel_load_sf2("{get_das_root()}/examples/daStrudel/midi_sf2_demo/FluidR3_GM.sf2")
    }
}
