options gen2
options indenting = 4
options persistent_heap

// Resonant LPF sweep — acid bass line (TB-303 style).
// Hear: sharp resonant peak sweeping up and down over the sawtooth.
/*
note("c2").s("sawtooth")
  .sustain(1)
  .lpf(sine.range(200, 4000))
  .lpq(8)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var lpf_sig <- signal_sine() |> signal_range(200.0, 4000.0)
    var pat <- atom("sawtooth") |> set_note(36.0) |> set_sustain(1.0) |> set_lpf(lpf_sig) |> set_lpq(8.0)
    play_feature_cps(pat, 0.5lf)
}
