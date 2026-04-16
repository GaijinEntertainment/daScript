options gen2
options indenting = 4
options persistent_heap

// Drum pattern with reverb (room=0.5).
// Hear: drums in a room — kick gets body, snare develops a tail.
/*
s("bd sd bd [sd cp]")
  .room(0.5)
  .roomsize(3)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- s("bd sd bd [sd cp]") |> set_room(0.5) |> set_roomsize(3.0)
    play_feature_cps(pat, 0.5lf)
}
