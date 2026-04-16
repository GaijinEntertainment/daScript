options gen2
options indenting = 4
options persistent_heap

// Pink noise with breathing LPF, mild reverb — ocean/fluid ambience.
// Tests Phase 4 (pink noise) + Phase 2 (filters) + Phase 5 (reverb).
// Mirrors Layer 6 from strudel_example.js — filtered noise bed.
/*
s("pink")
  .gain(0.3)
  .lpf(sine.range(120, 350).slow(40))
  .hpf(40)
  .room(0.5).roomsize(4)
  .orbit(5)
  .cpm(15)
*/

require feature_common

[export]
def main() {
    let lpfSig <- signal_sine() |> signal_range(120.0, 350.0) |> slow(40.0lf)
    let pat <- atom("pink") |> set_gain(0.3) |> set_sustain(1.0) |> set_lpf(lpfSig) |> set_hpf(40.0) |> set_room(0.5) |> set_roomsize(4.0) |> set_orbit(5)
    play_feature_cpm(pat, 15.0lf)
}
