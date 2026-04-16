options gen2
options indenting = 4
options persistent_heap

// Supersaw chord, slow LPF sweep, big reverb, pan drift.
// Tests Phase 4 (supersaw) + Phase 2 (filters) + Phase 5 (reverb) + Phase 1 (signals).
// Mirrors Layer 2 from strudel_example.js — warm mid pad.
/*
note("<c2 eb2 g2 bb2 f2 ab2 g2 eb2>")
  .s("supersaw")
  .slow(24)
  .gain(0.1)
  .attack(4).decay(2).sustain(0.7).release(6)
  .lpf(sine.range(250, 800).slow(32))
  .hpf(80)
  .room(0.9).roomsize(8)
  .pan(sine.range(0.3, 0.7).slow(20))
  .orbit(2)
  .cpm(15)
*/

require feature_common

[export]
def main() {
    let lpfSig <- signal_sine() |> signal_range(250.0, 800.0) |> slow(32.0lf)
    let panSig <- signal_sine() |> signal_range(0.3, 0.7) |> slow(20.0lf)
    let pat <- note_pattern("<c2 eb2 g2 bb2 f2 ab2 g2 eb2>", "supersaw") |> slow(24.0lf) |> set_gain(0.1) |> set_attack(4.0) |> set_decay(2.0) |> set_sustain(0.7) |> set_release(6.0) |> set_lpf(lpfSig) |> set_hpf(80.0) |> set_room(0.9) |> set_roomsize(8.0) |> set_pan(panSig) |> set_orbit(2)
    play_feature_cpm(pat, 15.0lf)
}
