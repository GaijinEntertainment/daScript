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
    let pat <- scale_pattern("0 ~ ~ 4 ~ 2 ~ ~ 6 ~ 3 ~", "C4:major:pentatonic") |> attack(0.001) |> decay(0.05) |> sustain(0.0) |> release(0.3) |> room(0.4) |> roomsize(3.0)
    play_feature_cps(pat, 0.5lf)
}
