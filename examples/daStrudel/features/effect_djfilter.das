options gen2
options indenting = 4
options persistent_heap

// DJ filter — LPF/HPF on one knob, bypassed at 0.5.
// Hear: a slow sweep from bass-only (djf=0) through wide-open (0.5)
// to high-pass telephone (1.0), like a club filter knob.
/*
note("c3 e3 g3 c4").s("sawtooth")
  .sustain(0.8)
  .djf(saw.slow(4))
  .cps(0.8)
*/

require feature_common

[export]
def main() {
    let pat <- note("c3 e3 g3 c4", "sawtooth") |> sustain(0.8) |> djf(signal_saw() |> slow(4.0lf))
    play_feature_cps(pat, 0.8lf)
}
