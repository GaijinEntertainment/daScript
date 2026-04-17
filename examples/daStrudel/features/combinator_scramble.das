options gen2
options indenting = 4
options persistent_heap

// scramble randomizes pattern with replacement — slots pick randomly, duplicates possible.
// Hear: 8-note sequence where each slot plays a random note from the original.
/*
note("c4 d4 e4 f4 g4 a4 b4 c5").s("sine")
  .scramble(8)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 d4 e4 f4 g4 a4 b4 c5", "sine") |> scramble(8)
    play_feature_cps(pat, 0.5lf)
}
