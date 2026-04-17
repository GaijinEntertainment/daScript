options gen2
options indenting = 4
options persistent_heap

// Sawtooth with hpf=2000 — thin, nasal tone with all bass removed.
// Hear: compare with raw sawtooth — fundamentals gone, only upper harmonics.
/*
note("c3").s("sawtooth")
  .sustain(1)
  .hpf(2000)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- atom("sawtooth") |> note(48.0) |> sustain(1.0) |> hpf(2000.0)
    play_feature_cps(pat, 0.5lf)
}
