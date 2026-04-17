options gen2
options indenting = 4
options persistent_heap

// Filtered white noise through vowel — whispering formants.
// Hear: noise shaped into recognizable vowel sounds, like a breathy whisper.
/*
s("white")
  .vowel("<a e i o u>")
  .gain(0.4)
  .attack(0.01)
  .decay(0.1)
  .sustain(0.6)
  .release(0.3)
  .room(0)
  .delay(0)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- atom("white") |> vowel("<a e i o u>") |> gain(0.4) |> attack(0.01) |> decay(0.1) |> sustain(0.6) |> release(0.3)
    play_feature_cps(pat, 0.5lf)
}
