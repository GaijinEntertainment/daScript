options gen2
options indenting = 4
options persistent_heap

// Sine with fm=5, fmh=1.4 — harsh, inharmonic, industrial.
// Non-integer harmonicity ratio + high modulation depth = rich, noisy timbre.
/*
note("c3").s("sine")
  .fm(5)
  .fmh(1.4)
  .attack(0.01)
  .decay(0.2)
  .release(0.3)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- note_pattern("c3", "sine") |> set_fm(5.0) |> set_fmh(1.4) |> set_attack(0.01) |> set_decay(0.2) |> set_release(0.3)
    play_feature_cps(pat, 0.5lf)
}
