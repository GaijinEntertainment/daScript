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
    let pat <- s("<supersaw sawtooth>") |> note(48.0) |> attack(0.01) |> decay(0.1) |> sustain(0.8) |> release(0.3)
    play_feature_cps(pat, 0.5lf)
}
