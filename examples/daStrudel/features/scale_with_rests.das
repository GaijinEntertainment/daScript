options gen2
options indenting = 4
options persistent_heap

// Sparse pentatonic with rests — bubble-like texture.
// Rests (~) create rhythmic gaps; degrees pick notes from the scale.
/*
n("0 ~ ~ 4 ~ 2 ~ ~ 6 ~ 3 ~").scale("C4:major:pentatonic").s("sine")
  .attack(0.001).decay(0.05).sustain(0).release(0.3)
  .room(0.4).roomsize(3)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- scale_pattern("0 ~ ~ 4 ~ 2 ~ ~ 6 ~ 3 ~", "C4:major:pentatonic") |> set_attack(0.001) |> set_decay(0.05) |> set_sustain(0.0) |> set_release(0.3) |> set_room(0.4) |> set_roomsize(3.0)
    play_feature_cps(pat, 0.5lf)
}
