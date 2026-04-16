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
    let pat <- note_pattern("<g3 c4 eb4 f4 bb3 g3 ab3 c4>", "sine") |> set_fm(2.0) |> set_fmh(3.0) |> slow(28.0lf) |> set_gain(0.08) |> set_attack(6.0) |> set_decay(3.0) |> set_sustain(0.5) |> set_release(10.0) |> set_vowel(vowelPat) |> set_lpf(1200.0) |> set_room(1.0) |> set_roomsize(10.0) |> set_delay(0.3) |> set_delaytime(0.8) |> set_delayfeedback(0.45)
    play_feature_cpm(pat, 15.0lf)
}
