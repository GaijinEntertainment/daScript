options gen2
options indenting = 4
options persistent_heap

// Sawtooth with lpf=800 and high Q — resonant peak at cutoff, the "wah" sound.
// Hear: sharp, ringing peak. Compare lpq=1 (gentle) vs lpq=10 (peaky).
/*
note("c3").s("sawtooth")
  .sustain(1)
  .lpf(800)
  .lpq(10)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- atom("sawtooth") |> note(48.0) |> sustain(1.0) |> lpf(800.0) |> lpq(10.0)
    play_feature_cps(pat, 0.5lf)
}
