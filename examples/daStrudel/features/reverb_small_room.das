options gen2
options indenting = 4
options persistent_heap

// Pluck in a small room (roomsize=0.3 — short decay).
// Hear: tight, contained reverb that fades quickly.
/*
note("c4 e4 g4 c5").s("sine")
  .attack(0.001)
  .decay(0.1)
  .sustain(0)
  .release(0.01)
  .room(0.8)
  .roomsize(0.3)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 e4 g4 c5", "sine") |> attack(0.001) |> decay(0.1) |> sustain(0.0) |> release(0.01) |> room(0.8) |> roomsize(0.3)
    play_feature_cps(pat, 0.5lf)
}
