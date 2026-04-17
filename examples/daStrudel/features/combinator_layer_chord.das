options gen2
options indenting = 4
options persistent_heap

// layer — stack transforms on one pattern to build chords
// Hear: each note plays as a triad (root + third + fifth).
/*
note("c3 e3 g3 c4")
  .s("sine")
  .sustain(1)
  .layer(x => x, x => x.transpose(4), x => x.transpose(7))
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- layer([
        note("c3 e3 g3 c4") |> sustain(1.0) |> transpose(4.0),
        note("c3 e3 g3 c4") |> sustain(1.0) |> transpose(7.0)],
      note("c3 e3 g3 c4") |> sustain(1.0))
    play_feature_cps(pat, 0.5lf)
}
