options gen2
options indenting = 4
options persistent_heap

// linger loops the first fraction of a pattern.
// Hear: only the first half of the melody plays, but it loops twice as fast to fill each cycle.
/*
note("c4 e4 g4 c5 a4 g4 e4 c4").s("sine")
  .linger(0.5)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 e4 g4 c5 a4 g4 e4 c4", "sine") |> linger(0.5lf)
    play_feature_cps(pat, 0.5lf)
}
