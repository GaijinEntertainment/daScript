options gen2
options indenting = 4
options persistent_heap

// True biquad bandpass (bpf/bpq) — narrow resonant peak, unlike the LPF+HPF
// combination used elsewhere. Hear: a vocal-ish "wah" sweeping across the
// spectrum, emphasising only a narrow band around the centre frequency.
/*
note("c3 e3 g3 c4").s("sawtooth")
  .sustain(0.8)
  .bpf(sine.range(300, 3000).slow(4))
  .bpq(6)
  .cps(0.8)
*/

require feature_common

[export]
def main() {
    let pat <- note("c3 e3 g3 c4", "sawtooth") |> sustain(0.8) |> bpf(signal_sine() |> signal_range(300.0, 3000.0) |> slow(4.0lf)) |> bpq(6.0)
    play_feature_cps(pat, 0.8lf)
}
