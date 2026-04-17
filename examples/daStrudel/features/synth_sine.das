options gen2
options indenting = 4
options persistent_heap

// Pure sine wave, C3 with slow attack and long release.
// Hear: clean, round tone — the simplest oscillator.
/*
note("c3").s("sine")
  .attack(0.05)
  .decay(0.1)
  .sustain(0.8)
  .release(0.5)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- s("sine") |> note(48.0) |> attack(0.05) |> decay(0.1) |> sustain(0.8) |> release(0.5)
    play_feature_cps(pat, 0.5lf)
}
