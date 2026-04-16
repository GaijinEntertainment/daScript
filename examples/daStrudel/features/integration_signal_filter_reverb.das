options gen2
options indenting = 4
options persistent_heap

// Sawtooth with signal-modulated LPF + reverb.
// Tests Phase 1 (signals) + Phase 2 (filters) + Phase 5 (reverb) together.
// Hear: filter slowly opens and closes, tail lingers in reverb.
/*
note("c3 e3 g3").s("sawtooth")
  .lpf(sine.range(200, 2000).slow(4))
  .gain(0.2)
  .attack(0.05)
  .decay(0.2)
  .sustain(0.8)
  .release(0.3)
  .room(0.7)
  .roomsize(5)
  .delay(0)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let lpfSig <- signal_sine() |> signal_range(200.0, 2000.0) |> slow(4.0lf)
    let pat <- note_pattern("c3 e3 g3", "sawtooth") |> set_lpf(lpfSig) |> set_gain(0.2) |> set_attack(0.05) |> set_decay(0.2) |> set_sustain(0.8) |> set_release(0.3) |> set_room(0.7) |> set_roomsize(5.0)
    play_feature_cps(pat, 0.5lf)
}
