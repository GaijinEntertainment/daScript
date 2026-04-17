options gen2
options indenting = 4
options persistent_heap

// Sawtooth with lpf=200 — dark, muffled tone.
// Hear: compare with a raw sawtooth — all brightness removed.
/*
note("c3").s("sawtooth")
  .sustain(1)
  .lpf(200)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- atom("sawtooth") |> note(48.0) |> sustain(1.0) |> lpf(200.0)
    play_feature_cps(pat, 0.5lf)
}
