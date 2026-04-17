options gen2
options indenting = 4
options persistent_heap

// Sine with fm=2, fmh=3 — ghostly, choir-like (the ambient example's Layer 3).
// Integer harmonicity with moderate depth adds a few harmonic overtones.
// G3 (55) matches the reference's starting note for the choral texture.
/*
note("g3").s("sine")
  .fm(2)
  .fmh(3)
  .attack(0.05)
  .decay(0.2)
  .sustain(0.7)
  .release(0.5)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("g3", "sine") |> fm(2.0) |> fmh(3.0) |> attack(0.05) |> decay(0.2) |> sustain(0.7) |> release(0.5)
    play_feature_cps(pat, 0.5lf)
}
