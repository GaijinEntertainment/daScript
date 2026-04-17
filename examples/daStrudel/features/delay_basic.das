options gen2
options indenting = 4
options persistent_heap

// Pluck with delay echo. Each note repeats with fading echoes.
// Hear: pluck → echo → echo → echo, getting quieter each time.
/*
note("c4 ~ e4 ~ g4 ~ c5 ~").s("sine")
  .attack(0.001)
  .decay(0.15)
  .sustain(0)
  .release(0.01)
  .delay(0.5)
  .delaytime(0.5)
  .delayfeedback(0.4)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 ~ e4 ~ g4 ~ c5 ~", "sine") |> attack(0.001) |> decay(0.15) |> sustain(0.0) |> release(0.01) |> delay(0.5) |> delaytime(0.5) |> delayfeedback(0.4)
    play_feature_cps(pat, 0.5lf)
}
