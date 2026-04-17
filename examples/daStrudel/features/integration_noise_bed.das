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
    let lpfSig <- sine() |> range(120.0, 350.0) |> slow(40.0lf)
    let pat <- atom("pink") |> gain(0.3) |> sustain(1.0) |> lpf(lpfSig) |> hpf(40.0) |> room(0.5) |> roomsize(4.0) |> orbit(5)
    play_feature_cpm(pat, 15.0lf)
}
