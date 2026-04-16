options gen2
options indenting = 4
options persistent_heap

// Same note cycling through a-e-i-o-u (one per cycle via cat).
// Hear: each vowel has a distinct tonal character — open, nasal, bright, round, dark.
/*
note("c3").s("sawtooth")
  .vowel("<a e i o u>")
  .gain(0.3)
  .attack(0.01)
  .decay(0.1)
  .sustain(0.8)
  .release(0.3)
  .room(0)
  .delay(0)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- atom("sawtooth") |> set_note(48.0) |> set_vowel("<a e i o u>") |> set_gain(0.3) |> set_attack(0.01) |> set_decay(0.1) |> set_sustain(0.8) |> set_release(0.3)
    play_feature_cps(pat, 0.5lf)
}
