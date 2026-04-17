options gen2
options indenting = 4
options persistent_heap

// Supersaw playing a 3-note chord (C-E-G).
// Hear: thick, wide pad — the classic "trance" sound.
/*
note("c3,e3,g3").s("supersaw")
  .attack(0.05)
  .decay(0.1)
  .sustain(0.7)
  .release(0.5)
  .lpf(4000)
  .cps(0.25)
*/

require feature_common

[export]
def main() {
    let pat <- note("c3,e3,g3", "supersaw") |> attack(0.05) |> decay(0.1) |> sustain(0.7) |> release(0.5) |> lpf(4000.0)
    play_feature_cps(pat, 0.25lf)
}
