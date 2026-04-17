options gen2
options indenting = 4
options persistent_heap

// FM sine + cycling vowel + large reverb — ghostly choir effect.
// Tests Phase 4 (FM) + Phase 9 (vowel) + Phase 5 (reverb).
// Mirrors Layer 3 from strudel_example.js.
/*
note("<g3 c4 eb4 f4 bb3 g3 ab3 c4>")
  .s("sine")
  .fm(2).fmh(3)
  .slow(28)
  .gain(0.08)
  .attack(6).decay(3).sustain(0.5).release(10)
  .vowel("<a e i o u e>".slow(12))
  .lpf(1200)
  .room(1).roomsize(10)
  .delay(0.3).delaytime(0.8).delayfeedback(0.45)
  .cpm(15)
*/

require feature_common

[export]
def main() {
    let vowelPat <- s("<a e i o u e>") |> slow(12.0lf)
    let pat <- note("<g3 c4 eb4 f4 bb3 g3 ab3 c4>", "sine") |> fm(2.0) |> fmh(3.0) |> slow(28.0lf) |> gain(0.08) |> attack(6.0) |> decay(3.0) |> sustain(0.5) |> release(10.0) |> vowel(vowelPat) |> lpf(1200.0) |> room(1.0) |> roomsize(10.0) |> delay(0.3) |> delaytime(0.8) |> delayfeedback(0.45)
    play_feature_cpm(pat, 15.0lf)
}
