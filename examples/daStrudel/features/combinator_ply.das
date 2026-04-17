options gen2
options indenting = 4
options persistent_heap

// ply repeats each event n times within its timespan.
// Hear: 4-note melody where each note is played 3 times in rapid succession.
/*
note("c4 e4 g4 c5").s("sine")
  .sustain(0.3)
  .ply(3)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 e4 g4 c5", "sine") |> sustain(0.3) |> ply(3)
    play_feature_cps(pat, 0.5lf)
}
