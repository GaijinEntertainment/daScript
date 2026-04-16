options gen2
options indenting = 4
options persistent_heap

// Signal modulates gain on a sustained tone — smooth volume pulsing.
/*
note("c3").s("sine")
  .sustain(1)
  .gain(sine.slow(2))
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- atom("sine") |> set_note(48.0) |> set_sustain(1.0) |> set_gain(signal_sine() |> slow(2.0lf))
    play_feature_cps(pat, 0.5lf)
}
