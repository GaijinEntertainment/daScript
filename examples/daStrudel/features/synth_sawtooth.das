options gen2
options indenting = 4
options persistent_heap

// Sawtooth wave, C3 with a low-pass filter to tame the harmonics.
// Hear: bright, buzzy tone — classic subtractive synth starting point.
/*
note("c3").s("sawtooth")
  .attack(0.01)
  .decay(0.1)
  .sustain(0.7)
  .release(0.3)
  .lpf(2000)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- s("sawtooth") |> note(48.0) |> attack(0.01) |> decay(0.1) |> sustain(0.7) |> release(0.3) |> lpf(2000.0)
    play_feature_cps(pat, 0.5lf)
}
