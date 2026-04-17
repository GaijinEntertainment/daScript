options gen2
options indenting = 4
options persistent_heap

// Sawtooth chord with vowel("<a e i o u>") cycling each cycle.
// Hear: a choir-like texture morphing between vowel shapes.
/*
note("c3 e3 g3").s("sawtooth")
  .vowel("<a e i o u>")
  .gain(0.2)
  .attack(0.05)
  .decay(0.2)
  .sustain(0.7)
  .release(0.5)
  .room(0.3)
  .roomsize(3)
  .delay(0)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c3 e3 g3", "sawtooth") |> vowel("<a e i o u>") |> gain(0.2) |> attack(0.05) |> decay(0.2) |> sustain(0.7) |> release(0.5) |> room(0.3) |> roomsize(3.0)
    play_feature_cps(pat, 0.5lf)
}
