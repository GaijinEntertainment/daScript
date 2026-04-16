options gen2
options indenting = 4
options persistent_heap

// Delay AND reverb combined. Echoes dissolve into reverb wash.
// Hear: pluck → echo → echo, each echo blurred by reverb tail.
/*
note("c4 ~ ~ e4 ~ ~ g4 ~ ~ c5 ~ ~").s("sine")
  .attack(0.001)
  .decay(0.15)
  .sustain(0)
  .release(0.01)
  .delay(0.4)
  .delaytime(0.375)
  .delayfeedback(0.5)
  .room(0.6)
  .roomsize(3)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- note_pattern("c4 ~ ~ e4 ~ ~ g4 ~ ~ c5 ~ ~", "sine") |> set_attack(0.001) |> set_decay(0.15) |> set_sustain(0.0) |> set_release(0.01) |> set_delay(0.4) |> set_delaytime(0.375) |> set_delayfeedback(0.5) |> set_room(0.6) |> set_roomsize(3.0)
    play_feature_cps(pat, 0.5lf)
}
