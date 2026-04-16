options gen2
options indenting = 4
options persistent_heap

// FM synthesis + cycling vowel filter — ghostly choir effect.
// Hear: FM harmonics shaped by vocal formants, vowel changes every cycle.
/*
note("c3 eb3 g3 bb3").s("sine")
  .fm(2)
  .fmh(3)
  .vowel("<a e i o u>")
  .gain(0.3)
  .attack(0.05)
  .decay(0.2)
  .sustain(0.6)
  .release(0.5)
  .room(0.5)
  .roomsize(4)
  .delay(0)
  .cps(0.25)
*/

require feature_common

[export]
def main() {
    var pat <- note_pattern("c3 eb3 g3 bb3", "sine") |> set_fm(2.0) |> set_fmh(3.0) |> set_vowel("<a e i o u>") |> set_gain(0.3) |> set_attack(0.05) |> set_decay(0.2) |> set_sustain(0.6) |> set_release(0.5) |> set_room(0.5) |> set_roomsize(4.0)
    play_feature_cps(pat, 0.25lf)
}
