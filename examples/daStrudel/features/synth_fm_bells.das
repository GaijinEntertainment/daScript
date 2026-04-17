options gen2
options indenting = 4
options persistent_heap

// Sine with fm=1, fmh=7 — metallic bell-like tone.
// High harmonicity ratio creates inharmonic partials characteristic of bells.
/*
note("c5").s("sine")
  .fm(1)
  .fmh(7)
  .decay(0.3)
  .release(0.5)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c5", "sine") |> fm(1.0) |> fmh(7.0) |> decay(0.3) |> release(0.5)
    play_feature_cps(pat, 0.5lf)
}
