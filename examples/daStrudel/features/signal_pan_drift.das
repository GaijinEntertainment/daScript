options gen2
options indenting = 4
options persistent_heap

// Sine signal modulates pan — sound drifts left to right slowly.
/*
note("c3").s("sine")
  .sustain(1)
  .pan(sine.slow(4))
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- atom("sine") |> set_note(48.0) |> set_sustain(1.0) |> set_pan(signal_sine() |> slow(4.0lf))
    play_feature_cps(pat, 0.5lf)
}
