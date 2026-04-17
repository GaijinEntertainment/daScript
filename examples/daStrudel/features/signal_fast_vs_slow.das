options gen2
options indenting = 4
options persistent_heap

// Same sine signal at slow(2) vs slow(16) modulating lpf.
// Hear: fast wobble vs glacial drift, alternating every cycle.
/*
note("c3").s("sawtooth")
  .sustain(1)
  .lpf(sine.range(200, 4000).slow("<2 16>"))
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let lpf_sig <- sine() |> range(200.0, 4000.0) |> slow(n("<2 16>"))
    let pat <- atom("sawtooth") |> note(48.0) |> sustain(1.0) |> lpf(lpf_sig)
    play_feature_cps(pat, 0.5lf)
}
