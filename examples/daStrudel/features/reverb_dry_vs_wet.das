options gen2
options indenting = 4
options persistent_heap

// Same pluck played twice per cycle. Room=0.8 adds a reverb tail.
// Hear: each pluck rings out into space instead of cutting off sharply.
/*
note("c4 ~ c4 ~").s("sine")
  .attack(0.001)
  .decay(0.15)
  .sustain(0)
  .release(0.01)
  .room(0.8)
  .roomsize(2)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- note_pattern("c4 ~ c4 ~", "sine") |> set_attack(0.001) |> set_decay(0.15) |> set_sustain(0.0) |> set_release(0.01) |> set_room(0.8) |> set_roomsize(2.0)
    play_feature_cps(pat, 0.5lf)
}
