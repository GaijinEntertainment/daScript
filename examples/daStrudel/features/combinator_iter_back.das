options gen2
options indenting = 4
options persistent_heap

// iterBack shifts right instead of left — phrase rotation in reverse direction.
// Hear: 4-note phrase rotating its start point BACKWARD each cycle.
/*
note("c4 e4 g4 a4").s("sine")
  .iterBack(4)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 e4 g4 a4", "sine") |> iterBack(4)
    play_feature_cps(pat, 0.5lf)
}
