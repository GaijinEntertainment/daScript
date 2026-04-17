options gen2
options indenting = 4
options persistent_heap

// Tremolo — amplitude modulation by a sine LFO.
// Hear: a sustained pad pulses between full volume and near silence as the
// LFO sweeps the gain; rate in Hz controls how fast it wobbles.
/*
note("d3 d3 d#3 d3").s("supersaw")
  .sustain(1).release(0.5)
  .tremolo(8).tremolodepth(0.8)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("d3 d3 d#3 d3", "supersaw") |> sustain(1.0) |> release(0.5) |> tremolo(8.0) |> tremolodepth(0.8)
    play_feature_cps(pat, 0.5lf)
}
