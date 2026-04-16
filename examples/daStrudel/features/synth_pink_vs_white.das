options gen2
options indenting = 4
options persistent_heap

// Pink noise vs white noise (alternating each half-cycle).
// Hear: pink is softer/darker, white is bright and hissy.
/*
s("pink white")
  .sustain(1)
  .gain(0.6)
  .cps(0.25)
*/

require feature_common

[export]
def main() {
    var pat <- s("pink white") |> set_sustain(1.0) |> set_gain(0.6)
    play_feature_cps(pat, 0.25lf)
}
