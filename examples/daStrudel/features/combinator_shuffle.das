options gen2
options indenting = 4
options persistent_heap

// shuffle randomly permutes n subdivisions of the pattern per cycle.
// Hear: 8-note sequence with its order scrambled differently each cycle.
/*
note("c4 d4 e4 f4 g4 a4 b4 c5").s("sine")
  .shuffle(8)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 d4 e4 f4 g4 a4 b4 c5", "sine") |> shuffle(8)
    play_feature_cps(pat, 0.5lf)
}
