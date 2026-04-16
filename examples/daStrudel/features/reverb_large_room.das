options gen2
options indenting = 4
options persistent_heap

// Pluck in a large room (roomsize=8 — long decay).
// Hear: vast, cathedral-like reverb with long tail.
/*
note("c4 ~ ~ e4 ~ ~ g4 ~ ~ c5 ~ ~").s("sine")
  .attack(0.001)
  .decay(0.15)
  .sustain(0)
  .release(0.01)
  .room(0.8)
  .roomsize(8)
  .cps(0.25)
*/

require feature_common

[export]
def main() {
    var pat <- note_pattern("c4 ~ ~ e4 ~ ~ g4 ~ ~ c5 ~ ~", "sine") |> set_attack(0.001) |> set_decay(0.15) |> set_sustain(0.0) |> set_release(0.01) |> set_room(0.8) |> set_roomsize(8.0)
    play_feature_cps(pat, 0.25lf)
}
