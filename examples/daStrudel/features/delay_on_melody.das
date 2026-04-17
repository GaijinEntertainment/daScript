options gen2
options indenting = 4
options persistent_heap

// Arpeggiated melody with delay — interlocking echoes create counterpoint.
// Hear: rising arpeggios where echoes of earlier notes overlap with new notes.
/*
note("c4 e4 g4 b4 c5 b4 g4 e4").s("triangle")
  .attack(0.001)
  .decay(0.15)
  .sustain(0)
  .release(0.01)
  .delay(0.3)
  .delaytime(0.75)
  .delayfeedback(0.4)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 e4 g4 b4 c5 b4 g4 e4", "triangle") |> attack(0.001) |> decay(0.15) |> sustain(0.0) |> release(0.01) |> delay(0.3) |> delaytime(0.75) |> delayfeedback(0.4)
    play_feature_cps(pat, 0.5lf)
}
