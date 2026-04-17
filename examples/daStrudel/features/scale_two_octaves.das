options gen2
options indenting = 4
options persistent_heap

// Two layers: same scale, different octaves — high and low voices in harmony.
// C4 pentatonic bass + C5 pentatonic melody with delay.
/*
stack(
  n("0 2 4 3").scale("C4:major:pentatonic").s("sawtooth")
    .attack(0.01).decay(0.1).sustain(0.3).release(0.1)
    .lpf(500).gain(0.4),
  n("4 2 3 0").scale("C5:major:pentatonic").s("sine")
    .attack(0.001).decay(0.05).sustain(0).release(0.2)
    .delay(0.3).delaytime(0.375).delayfeedback(0.3).gain(0.5)
).cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- stack([
        scale_pattern("0 2 4 3", "C4:major:pentatonic", "sawtooth") |> attack(0.01) |> decay(0.1) |> sustain(0.3) |> release(0.1) |> lpf(500.0) |> gain(0.4),
        scale_pattern("4 2 3 0", "C5:major:pentatonic") |> attack(0.001) |> decay(0.05) |> sustain(0.0) |> release(0.2) |> delay(0.3) |> delaytime(0.375) |> delayfeedback(0.3) |> gain(0.5)
    ])
    play_feature_cps(pat, 0.5lf)
}
