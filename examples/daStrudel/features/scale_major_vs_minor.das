options gen2
options indenting = 4
options persistent_heap

// Alternating major and minor arpeggios on different cycles.
// Same degrees, different scales — hear how mood shifts.
/*
stack(
  n("0 2 4 6").scale("C4:major").s("sine")
    .attack(0.001).decay(0.1).sustain(0).release(0.2).gain(0.5),
  n("0 2 4 6").scale("C4:minor").s("sawtooth")
    .attack(0.01).decay(0.1).sustain(0.3).release(0.2).gain(0.3).lpf(2000)
).cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- stack([
        scale_pattern("0 2 4 6", "C4:major") |> attack(0.001) |> decay(0.1) |> sustain(0.0) |> release(0.2) |> gain(0.5),
        scale_pattern("0 2 4 6", "C4:minor", "sawtooth") |> attack(0.01) |> decay(0.1) |> sustain(0.3) |> release(0.2) |> gain(0.3) |> lpf(2000.0)
    ])
    play_feature_cps(pat, 0.5lf)
}
