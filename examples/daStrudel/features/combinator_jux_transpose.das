options gen2
options indenting = 4
options persistent_heap

// jux(transpose(7)) — stereo harmony at the fifth
// Hear: original melody in left ear, transposed up a fifth in right ear.
/*
note("c4 e4 g4 c5")
  .s("sine")
  .sustain(1)
  .jux(x => x.transpose(7))
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- jux(note_pattern("c4 e4 g4 c5") |> set_sustain(1.0), @(x) => transpose(x, 7.0))
    play_feature_cps(pat, 0.5lf)
}
