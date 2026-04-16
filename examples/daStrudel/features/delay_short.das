options gen2
options indenting = 4
options persistent_heap

// Short delay time — slapback echo effect (rockabilly / vintage feel).
// Hear: each pluck has an immediate, tight doubling effect.
/*
note("c4 e4 g4 c5").s("sine")
  .attack(0.001)
  .decay(0.1)
  .sustain(0)
  .release(0.01)
  .delay(0.5)
  .delaytime(0.08)
  .delayfeedback(0.3)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- note_pattern("c4 e4 g4 c5", "sine") |> set_attack(0.001) |> set_decay(0.1) |> set_sustain(0.0) |> set_release(0.01) |> set_delay(0.5) |> set_delaytime(0.08) |> set_delayfeedback(0.3)
    play_feature_cps(pat, 0.5lf)
}
