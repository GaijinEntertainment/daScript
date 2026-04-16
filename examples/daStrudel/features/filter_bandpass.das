options gen2
options indenting = 4
options persistent_heap

// Sawtooth with lpf=1500 + hpf=500 — band-pass, mid-focused telephone sound.
// Hear: both bass and treble removed, only midrange survives.
/*
note("c3").s("sawtooth")
  .sustain(1)
  .lpf(1500)
  .hpf(500)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- atom("sawtooth") |> set_note(48.0) |> set_sustain(1.0) |> set_lpf(1500.0) |> set_hpf(500.0)
    play_feature_cps(pat, 0.5lf)
}
