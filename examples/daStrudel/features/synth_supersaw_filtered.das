options gen2
options indenting = 4
options persistent_heap

// Supersaw with LPF sweep via signal — "corrupted acoustic warmth".
// Hear: filter opens and closes, revealing and hiding harmonics.
/*
note("c3").s("supersaw")
  .lpf(sine.range(400,6000).slow(4))
  .attack(0.05)
  .decay(0.1)
  .sustain(0.8)
  .release(0.5)
  .cps(0.25)
*/

require feature_common

[export]
def main() {
    var lpfSig <- signal_sine() |> signal_range(400.0, 6000.0) |> slow(4.0lf)
    var pat <- note_pattern("c3", "supersaw") |> set_lpf(lpfSig) |> set_attack(0.05) |> set_decay(0.1) |> set_sustain(0.8) |> set_release(0.5)
    play_feature_cps(pat, 0.25lf)
}
