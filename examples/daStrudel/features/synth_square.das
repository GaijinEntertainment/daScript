options gen2
options indenting = 4
options persistent_heap

// Square wave, C3 with low-pass filter.
// Hear: hollow, woody tone — odd harmonics only.
/*
note("c3").s("square")
  .attack(0.01)
  .decay(0.15)
  .sustain(0.6)
  .release(0.2)
  .lpf(1500)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- s("square") |> note(48.0) |> attack(0.01) |> decay(0.15) |> sustain(0.6) |> release(0.2) |> lpf(1500.0)
    play_feature_cps(pat, 0.5lf)
}
