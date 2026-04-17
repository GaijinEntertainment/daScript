options gen2
options indenting = 4
options persistent_heap

// Square wave signal modulates gain — hard on/off pulsing (50% duty cycle).
// Hear: sustained pad that cuts out completely every half-cycle.
/*
note("c3").s("sawtooth")
  .sustain(1)
  .gain(square.range(0.0, 0.6).slow(2))
  .lpf(800)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- atom("sawtooth") |> note(48.0) |> sustain(1.0) |> lpf(800.0) |> gain(square() |> range(0.0, 0.6) |> slow(2.0lf))
    play_feature_cps(pat, 0.5lf)
}
