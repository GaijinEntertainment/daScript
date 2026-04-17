options gen2
options indenting = 4
options persistent_heap

// Pluck with high feedback — echoes repeat many times, almost self-oscillating.
// Hear: a single pluck that keeps bouncing back, gradually fading over seconds.
/*
note("c4 ~ ~ ~").s("sine")
  .attack(0.001)
  .decay(0.2)
  .sustain(0)
  .release(0.01)
  .delay(0.6)
  .delaytime(0.375)
  .delayfeedback(0.8)
  .cps(0.25)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 ~ ~ ~", "sine") |> attack(0.001) |> decay(0.2) |> sustain(0.0) |> release(0.01) |> delay(0.6) |> delaytime(0.375) |> delayfeedback(0.8)
    play_feature_cps(pat, 0.25lf)
}
