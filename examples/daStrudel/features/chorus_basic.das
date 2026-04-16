options gen2
options indenting = 4
options persistent_heap

// Sawtooth chord with chorus effect — warm detuned pad with gentle stereo widening.
// Hear: rich sawtooth chords shimmer and spread as chorus adds subtle detuning.
// Note: strudel.cc .chorus() is audio chopping, not an effect — our chorus send is daslang-only.

require feature_common

[export]
def main() {
    let pat <- note_pattern("<[c3,e3,g3] [f3,a3,c4] [g3,b3,d4] [a3,c4,e4]>", "sawtooth") |> set_chorus(0.5) |> set_attack(0.05) |> set_decay(0.1) |> set_sustain(0.6) |> set_release(0.3) |> set_gain(0.4)
    play_feature_cps(pat, 0.5lf)
}
