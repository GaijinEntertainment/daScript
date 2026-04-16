options gen2
options indenting = 4
options persistent_heap

// FM index modulated by a sine signal — timbre morphs from pure to complex.
// Hear: the tone smoothly transitions between clean sine and rich FM harmonics.
/*
note("c4").s("sine")
  .fm(sine.range(0,5).slow(4))
  .fmh(3)
  .attack(0.01)
  .decay(0.1)
  .sustain(0.8)
  .release(0.3)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var fmSig <- signal_sine() |> signal_range(0.0, 5.0) |> slow(4.0lf)
    var pat <- atom("sine") |> set_note(60.0) |> set_fm(fmSig) |> set_fmh(3.0) |> set_attack(0.01) |> set_decay(0.1) |> set_sustain(0.8) |> set_release(0.3)
    play_feature_cps(pat, 0.5lf)
}
