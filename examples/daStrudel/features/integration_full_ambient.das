options gen2
options indenting = 4
options persistent_heap

// Full reproduction of strudel_example.js — ambient, 7-layer piece.
// Style: arrhythmic, choral pads, corrupted acoustic textures,
// delicate bubble arpeggios, large reverb spaces, modal harmony.
// No beat. Slow harmonic drift. Warm and organic.
/*
stack(
  // Layer 1: Deep ocean drone
  note("<c1 g1 f1 eb1 bb0 f1>")
    .s("sine").slow(32)
    .gain(0.3).attack(6).release(8)
    .lpf(180)
    .room(1).roomsize(10).orbit(1),

  // Layer 2: Warm mid pad
  note("<c2 eb2 g2 bb2 f2 ab2 g2 eb2>")
    .s("supersaw").slow(24)
    .gain(0.1).attack(4).decay(2).sustain(0.7).release(6)
    .lpf(sine.range(250, 800).slow(32))
    .hpf(80)
    .room(0.9).roomsize(8)
    .pan(sine.range(0.3, 0.7).slow(20))
    .orbit(2),

  // Layer 3: Choral texture
  note("<g3 c4 eb4 f4 bb3 g3 ab3 c4>")
    .s("sine").fm(2).fmh(3).slow(28)
    .gain(0.08).attack(6).decay(3).sustain(0.5).release(10)
    .vowel("<a e i o u e>".slow(12))
    .lpf(1200)
    .room(1).roomsize(10)
    .delay(0.3).delaytime(0.8).delayfeedback(0.45)
    .orbit(3),

  // Layer 4: Bubble arpeggios
  n("0 ~ ~ 4 ~ 2 ~ ~ 6 ~ 3 ~")
    .scale("C4:pentatonic").s("triangle").slow(6)
    .gain(perlin.range(0.03, 0.12).slow(10))
    .attack(0.01).decay(0.4).sustain(0.05).release(2)
    .lpf(3000)
    .pan(sine.range(0.15, 0.85).slow(7))
    .room(0.8).roomsize(6)
    .delay(0.55).delaytime(0.5).delayfeedback(0.65)
    .orbit(4),

  // Layer 5: Second arpeggio layer (higher, sparser)
  n("~ 5 ~ ~ 1 ~ ~ ~ 3 ~ ~ 7")
    .scale("C5:pentatonic").s("sine").slow(8)
    .gain(perlin.range(0.02, 0.08).slow(14))
    .attack(0.02).decay(0.3).sustain(0.0).release(3)
    .lpf(4000)
    .pan(sine.range(0.1, 0.9).slow(11))
    .room(0.9).roomsize(9)
    .delay(0.4).delaytime(0.75).delayfeedback(0.55)
    .orbit(4),

  // Layer 6: Filtered noise bed
  s("pink")
    .gain(0.03)
    .lpf(sine.range(120, 350).slow(40))
    .hpf(40)
    .room(0.5).roomsize(4).orbit(5),

  // Layer 7: Sub-harmonic pulse
  note("<c1 c1 f0 f0>")
    .s("sine").slow(48)
    .gain(sine.range(0.0, 0.15).slow(16))
    .attack(8).release(10)
    .lpf(100).orbit(1)
).cpm(15)
*/

require feature_common

[export]
def main() {
    // Layer 1: Deep ocean drone — low sine, very slow root movement
    var layer1 <- note_pattern("<c1 g1 f1 eb1 bb0 f1>", "sine") |> slow(32.0lf) |> set_gain(0.3) |> set_attack(6.0) |> set_sustain(1.0) |> set_release(8.0) |> set_lpf(180.0) |> set_room(1.0) |> set_roomsize(10.0) |> set_orbit(1)

    // Layer 2: Warm mid pad — detuned supersaw, slow filter sweep
    let lpf2 <- signal_sine() |> signal_range(250.0, 800.0) |> slow(32.0lf)
    let pan2 <- signal_sine() |> signal_range(0.3, 0.7) |> slow(20.0lf)
    var layer2 <- note_pattern("<c2 eb2 g2 bb2 f2 ab2 g2 eb2>", "supersaw") |> slow(24.0lf) |> set_gain(0.1) |> set_attack(4.0) |> set_decay(2.0) |> set_sustain(0.7) |> set_release(6.0) |> set_lpf(lpf2) |> set_hpf(80.0) |> set_room(0.9) |> set_roomsize(8.0) |> set_pan(pan2) |> set_orbit(2)

    // Layer 3: Choral texture — FM sine + vowel filter
    let vowel3 <- s("<a e i o u e>") |> slow(12.0lf)
    var layer3 <- note_pattern("<g3 c4 eb4 f4 bb3 g3 ab3 c4>", "sine") |> set_fm(2.0) |> set_fmh(3.0) |> slow(28.0lf) |> set_gain(0.08) |> set_attack(6.0) |> set_decay(3.0) |> set_sustain(0.5) |> set_release(10.0) |> set_vowel(vowel3) |> set_lpf(1200.0) |> set_room(1.0) |> set_roomsize(10.0) |> set_delay(0.3) |> set_delaytime(0.8) |> set_delayfeedback(0.45) |> set_orbit(3)

    // Layer 4: Bubble arpeggios — pentatonic triangle, sparse
    let gain4 <- signal_perlin() |> signal_range(0.03, 0.12) |> slow(10.0lf)
    let pan4 <- signal_sine() |> signal_range(0.15, 0.85) |> slow(7.0lf)
    var layer4 <- scale_pattern("0 ~ ~ 4 ~ 2 ~ ~ 6 ~ 3 ~", "C4:pentatonic", "triangle") |> slow(6.0lf) |> set_gain(gain4) |> set_attack(0.01) |> set_decay(0.4) |> set_sustain(0.05) |> set_release(2.0) |> set_lpf(3000.0) |> set_pan(pan4) |> set_room(0.8) |> set_roomsize(6.0) |> set_delay(0.55) |> set_delaytime(0.5) |> set_delayfeedback(0.65) |> set_orbit(4)

    // Layer 5: Second arpeggio layer — higher, sparser
    let gain5 <- signal_perlin() |> signal_range(0.02, 0.08) |> slow(14.0lf)
    let pan5 <- signal_sine() |> signal_range(0.1, 0.9) |> slow(11.0lf)
    var layer5 <- scale_pattern("~ 5 ~ ~ 1 ~ ~ ~ 3 ~ ~ 7", "C5:pentatonic", "sine") |> slow(8.0lf) |> set_gain(gain5) |> set_attack(0.02) |> set_decay(0.3) |> set_sustain(0.0) |> set_release(3.0) |> set_lpf(4000.0) |> set_pan(pan5) |> set_room(0.9) |> set_roomsize(9.0) |> set_delay(0.4) |> set_delaytime(0.75) |> set_delayfeedback(0.55) |> set_orbit(4)

    // Layer 6: Filtered noise bed — gentle ocean ambience
    let lpf6 <- signal_sine() |> signal_range(120.0, 350.0) |> slow(40.0lf)
    var layer6 <- atom("pink") |> set_gain(0.03) |> set_sustain(1.0) |> set_lpf(lpf6) |> set_hpf(40.0) |> set_room(0.5) |> set_roomsize(4.0) |> set_orbit(5)

    // Layer 7: Sub-harmonic pulse — very slow, barely audible low throb
    let gain7 <- signal_sine() |> signal_range(0.0, 0.15) |> slow(16.0lf)
    var layer7 <- note_pattern("<c1 c1 f0 f0>", "sine") |> slow(48.0lf) |> set_gain(gain7) |> set_attack(8.0) |> set_sustain(1.0) |> set_release(10.0) |> set_lpf(100.0) |> set_orbit(1)

    let pat <- stack([layer1, layer2, layer3, layer4, layer5, layer6, layer7])
    play_feature_cpm(pat, 15.0lf)
}
