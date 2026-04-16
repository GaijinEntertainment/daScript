options gen2
options indenting = 4
options persistent_heap

// Sustained supersaw pad with reverb (room=0.8).
// Hear: lush, diffused pad that fills the space.
/*
note("c3,e3,g3").s("supersaw")
  .attack(0.5)
  .decay(0.1)
  .sustain(0.7)
  .release(1.0)
  .lpf(3000)
  .room(0.8)
  .roomsize(4)
  .cps(0.25)
*/

require feature_common

[export]
def main() {
    var pat <- note_pattern("c3,e3,g3", "supersaw") |> set_attack(0.5) |> set_decay(0.1) |> set_sustain(0.7) |> set_release(1.0) |> set_lpf(3000.0) |> set_room(0.8) |> set_roomsize(4.0)
    play_feature_cps(pat, 0.25lf)
}
