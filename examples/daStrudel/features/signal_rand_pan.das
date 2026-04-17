options gen2
options indenting = 4
options persistent_heap

// Random signal modulates pan — notes bounce around unpredictably in stereo field.
// Hear: melody with each note landing at a random stereo position.
/*
note("c4 e4 g4 a4 g4 e4").s("sine")
  .sustain(0.5)
  .pan(rand)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 e4 g4 a4 g4 e4", "sine") |> sustain(0.5) |> pan(rand())
    play_feature_cps(pat, 0.5lf)
}
