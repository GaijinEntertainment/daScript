options gen2
options indenting = 4
options persistent_heap

// off(0.25, transpose(12)) — canon at the octave
// Hear: melody with echo one quarter-cycle later, transposed up an octave.
/*
note("c4 e4 g4 b4")
  .s("sine")
  .sustain(1)
  .off(1/4, x => x.transpose(12))
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- off(note("c4 e4 g4 b4") |> sustain(1.0), 0.25lf, @(x) => transpose(x, 12.0))
    play_feature_cps(pat, 0.5lf)
}
