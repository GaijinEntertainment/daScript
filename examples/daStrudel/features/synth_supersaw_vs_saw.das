options gen2
options indenting = 4
options persistent_heap

// Same note, alternating supersaw and single sawtooth per cycle.
// Hear: supersaw is much fatter and wider than the thin single saw.
/*
note("c3").s("<supersaw sawtooth>")
  .attack(0.01)
  .decay(0.1)
  .sustain(0.8)
  .release(0.3)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- s("<supersaw sawtooth>") |> set_note(48.0) |> set_attack(0.01) |> set_decay(0.1) |> set_sustain(0.8) |> set_release(0.3)
    play_feature_cps(pat, 0.5lf)
}
