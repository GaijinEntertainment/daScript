options gen2
options indenting = 4
options persistent_heap

// Triangle wave, C3 with gentle envelope.
// Hear: soft, flute-like tone — fewer harmonics than saw or square.
/*
note("c3").s("triangle")
  .attack(0.03)
  .decay(0.1)
  .sustain(0.8)
  .release(0.4)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- s("triangle") |> set_note(48.0) |> set_attack(0.03) |> set_decay(0.1) |> set_sustain(0.8) |> set_release(0.4)
    play_feature_cps(pat, 0.5lf)
}
