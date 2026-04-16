options gen2
options indenting = 4
options persistent_heap

// Pentatonic arpeggios with delay and panning signal.
// Tests Phase 8 (scales) + Phase 6 (delay) + Phase 1 (signals).
// Mirrors Layer 4 from strudel_example.js — delicate bubble arpeggios.
/*
n("0 ~ ~ 4 ~ 2 ~ ~ 6 ~ 3 ~")
  .scale("C4:pentatonic")
  .s("triangle")
  .slow(6)
  .gain(perlin.range(0.03, 0.12).slow(10))
  .attack(0.01).decay(0.4).sustain(0.05).release(2)
  .lpf(3000)
  .pan(sine.range(0.15, 0.85).slow(7))
  .room(0.8).roomsize(6)
  .delay(0.55).delaytime(0.5).delayfeedback(0.65)
  .cpm(15)
*/

require feature_common

[export]
def main() {
    let gainSig <- signal_perlin() |> signal_range(0.03, 0.12) |> slow(10.0lf)
    let panSig <- signal_sine() |> signal_range(0.15, 0.85) |> slow(7.0lf)
    let pat <- scale_pattern("0 ~ ~ 4 ~ 2 ~ ~ 6 ~ 3 ~", "C4:pentatonic", "triangle") |> slow(6.0lf) |> set_gain(gainSig) |> set_attack(0.01) |> set_decay(0.4) |> set_sustain(0.05) |> set_release(2.0) |> set_lpf(3000.0) |> set_pan(panSig) |> set_room(0.8) |> set_roomsize(6.0) |> set_delay(0.55) |> set_delaytime(0.5) |> set_delayfeedback(0.65)
    play_feature_cpm(pat, 15.0lf)
}
