options gen2
options indenting = 4
options persistent_heap

// Static HRTF position — sawtooth pulse positioned 45° to the right of the listener.
// Hear: a clear "off to the right" feeling that plain pan can't reproduce; HRTF adds
// front/back disambiguation and a sense of depth that changes with elevation.

require feature_common

[export]
def main() {
    let pat <- note("c4 ~ e4 ~ g4 ~ c5 ~", "sawtooth") |> hrtf(45.0, 0.0) |> attack(0.005) |> release(0.2) |> gain(0.5)
    play_feature_cps(pat, 0.5lf)
}
