options gen2
options indenting = 4
options persistent_heap

// Perlin noise modulates gain on a pad — organic, unpredictable swelling.
/*
note("c3").s("sawtooth")
  .sustain(1)
  .gain(perlin.range(0.1, 0.8).slow(4))
  .lpf(800)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- atom("sawtooth") |> set_note(48.0) |> set_sustain(1.0) |> set_lpf(800.0) |> set_gain(signal_perlin() |> signal_range(0.1, 0.8) |> slow(4.0lf))
    play_feature_cps(pat, 0.5lf)
}
