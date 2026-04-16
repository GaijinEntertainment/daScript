options gen2
options indenting = 4
options persistent_heap

// Sine signal modulates lpf on a sawtooth — classic filter sweep.
// Hear: muffled → bright → muffled, repeating.
/*
note("c3").s("sawtooth")
  .sustain(1)
  .lpf(sine.range(200, 4000).slow(4))
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- atom("sawtooth") |> set_note(48.0) |> set_sustain(1.0) |> set_lpf(signal_sine() |> signal_range(200.0, 4000.0) |> slow(4.0lf))
    play_feature_cps(pat, 0.5lf)
}
