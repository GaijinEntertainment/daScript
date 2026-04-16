options gen2
options indenting = 4
options persistent_heap

// Sawtooth with static vowel "a" — open "aah" formant sound.
// Hear: a warm, open "aah" tonal quality on the sawtooth.
/*
note("c3").s("sawtooth")
  .vowel("a")
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
    var pat <- atom("sawtooth") |> set_note(48.0) |> set_vowel("a") |> set_gain(0.3) |> set_attack(0.01) |> set_decay(0.1) |> set_sustain(0.8) |> set_release(0.3)
    play_feature_cps(pat, 0.5lf)
}
