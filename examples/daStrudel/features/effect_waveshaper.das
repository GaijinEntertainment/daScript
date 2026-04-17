options gen2
options indenting = 4
options persistent_heap

// Waveshaper — tanh soft-clipping distortion with adjustable drive.
// Hear: a pure sine becomes a gritty overdriven tone as shape sweeps up.
// shape=0 is clean; higher drive adds harmonics and compresses peaks.
/*
note("c3 e3 g3 c4").s("sine")
  .sustain(0.8)
  .shape(sine.range(0.5, 6))
  .slow(2)
  .cps(0.8)
*/

require feature_common

[export]
def main() {
    let pat <- note("c3 e3 g3 c4", "sine") |> sustain(0.8) |> shape(signal_sine() |> signal_range(0.5, 6.0)) |> slow(2.0lf)
    play_feature_cps(pat, 0.8lf)
}
