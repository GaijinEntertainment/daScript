options gen2
options indenting = 4
options persistent_heap

// chunk divides the cycle into n chunks and applies a transform to a rotating chunk.
// Hear: 8-note pattern where one chunk (out of 4) is played faster each cycle.
/*
note("c4 d4 e4 f4 g4 a4 b4 c5").s("sine")
  .chunk(4, x => x.fast(2))
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 d4 e4 f4 g4 a4 b4 c5", "sine") |> chunk(4, @(var x : Pattern) => fast(x, 2.0lf))
    play_feature_cps(pat, 0.5lf)
}
