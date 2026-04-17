options gen2
options indenting = 4
options persistent_heap

// Signal modulates lpf on a 3-note chord (stack).
// Hear: chord breathing together — all notes filtered in unison.
/*
stack(
  note("c3").s("sawtooth"),
  note("e3").s("sawtooth"),
  note("g3").s("sawtooth")
)
  .sustain(1)
  .lpf(sine.range(200, 3000).slow(4))
  .gain(0.3)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c3,e3,g3", "sawtooth") |> sustain(1.0) |> lpf(sine() |> range(200.0, 3000.0) |> slow(4.0lf)) |> gain(0.3)
    play_feature_cps(pat, 0.5lf)
}
