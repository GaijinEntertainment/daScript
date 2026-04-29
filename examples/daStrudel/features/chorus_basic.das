options gen2
options indenting = 4
options persistent_heap

// Sawtooth chord with chorus effect — warm detuned pad with gentle stereo widening.
// Hear: rich sawtooth chords shimmer and spread as chorus adds subtle detuning.
// Note: chorus is a per-orbit effect send (independent per orbit, like reverb / delay).

require feature_common

[export]
def main() {
    let pat <- note("<[c3,e3,g3] [f3,a3,c4] [g3,b3,d4] [a3,c4,e4]>", "sawtooth") |> chorus(0.5) |> attack(0.05) |> decay(0.1) |> sustain(0.6) |> release(0.3) |> gain(0.4)
    play_feature_cps(pat, 0.5lf)
}
