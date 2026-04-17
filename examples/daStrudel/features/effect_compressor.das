options gen2
options indenting = 4
options persistent_heap

// Compressor — feed-forward dynamics with peak detection.
// Hear: a hard-hitting drum pattern evens out; peaks past the threshold get
// squashed, making softer hits relatively louder and the groove more punchy.
/*
s("bd sd [~ bd] sd,hh*8")
  .compressor(-20).compressorRatio(10)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    // Punchy drum line: the compressor tames the kick+snare peaks so the hats sit up.
    let beat <- s("bd sd [~ bd] sd, hh*8")
    let pat <- beat |> compressor(-20.0) |> compressorRatio(10.0) |> compressorAttack(0.002) |> compressorRelease(0.02)
    play_feature_cps(pat, 0.5lf)
}
