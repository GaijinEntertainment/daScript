options gen2
options indenting = 4
options persistent_heap

// every(2, rev) — palindrome melody
// Hear: 4-note melody forward, then reversed, alternating.
/*
note("c4 e4 g4 c5")
  .s("sine")
  .sustain(1)
  .every(2, rev)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- every(2, note("c4 e4 g4 c5") |> sustain(1.0) |> rev(), note("c4 e4 g4 c5") |> sustain(1.0))
    play_feature_cps(pat, 0.5lf)
}
