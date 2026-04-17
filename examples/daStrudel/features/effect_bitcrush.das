options gen2
options indenting = 4
options persistent_heap

// Bitcrusher — amplitude quantisation and sample-rate reduction.
// Hear: a smooth sawtooth turns into a crunchy, lo-fi, chiptune-style tone.
// Bits=4 collapses amplitude to 16 levels; coarse=4 holds every 4th sample.
/*
note("c3 e3 g3 c4").s("sawtooth")
  .sustain(0.8)
  .crush(4)
  .coarse(4)
  .cps(0.8)
*/

require feature_common

[export]
def main() {
    let pat <- note("c3 e3 g3 c4", "sawtooth") |> sustain(0.8) |> crush(4.0) |> coarse(4)
    play_feature_cps(pat, 0.8lf)
}
