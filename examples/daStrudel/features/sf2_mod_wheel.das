options gen2
options indenting = 4
options persistent_heap

// Flute melody with and without mod wheel vibrato — compare clean vs expressive.
// Hear: dry flute plays a simple melody, then vibrato-rich flute repeats it with warmth and motion.
// SF2 mod wheel is a daslang-only extension.

require feature_common

[export]
def main() {
    let pat <- stack([
        // Clean flute — no mod wheel, dry and pure
        note("c4 e4 g4 c5") |> sf2("flute") |> gain(0.5),
        // Vibrato flute — mod wheel adds expressive vibrato
        note("c4 e4 g4 c5") |> sf2("flute") |> sf2_mod_wheel(0.8) |> gain(0.5)
    ])
    play_feature_cps(pat, 0.5lf) {
        strudel_load_sf2("{get_das_root()}/examples/daStrudel/midi_sf2_demo/FluidR3_GM.sf2")
    }
}
