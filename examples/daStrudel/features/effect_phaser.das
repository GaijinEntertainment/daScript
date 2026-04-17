options gen2
options indenting = 4
options persistent_heap

// Phaser — single notch biquad with an LFO sweeping the notch frequency.
// Hear: a sustained sawtooth takes on a swooshing, swept-resonance character
// as the notch moves through the mid-frequency band.
/*
n(run(8)).scale("D:pentatonic").s("sawtooth").release(0.5)
  .sustain(1)
  .phaser(2)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- run(8) |> scale("D:pentatonic") |> sound("sawtooth") |> sustain(1.0) |> release(0.5) |> phaser(2.0)
    play_feature_cps(pat, 0.5lf)
}
