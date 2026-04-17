options gen2
options indenting = 4
options persistent_heap

// compress squeezes the pattern into a time window within each cycle.
// Hear: melody plays only during the middle half of each cycle, silent at start and end.
/*
note("c4 e4 g4 c5").s("sine")
  .sustain(0.3)
  .compress(0.25, 0.75)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 e4 g4 c5", "sine") |> sustain(0.3) |> compress(0.25lf, 0.75lf)
    play_feature_cps(pat, 0.5lf)
}
