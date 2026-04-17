options gen2
options indenting = 4
options persistent_heap

// elongation (_) and weight (@) — uneven rhythms using mini-notation
//
// _ extends the previous element: "c4 _ _ e4 g4" = c4 holds 3/5, e4 and g4 get 1/5 each
// @N sets explicit weight: "c4@3 e4 g4" = same result (c4 gets weight 3, others get 1)
//
// Hear: both tracks play in unison — c4 held for 3/5, then quick e4 and g4.
/*
stack(
  note("c4 _ _ e4 g4").s("sine"),
  note("c4@3 e4 g4").s("sine")
)
  .sustain(1)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- stack([
      note("c4 _ _ e4 g4") |> sustain(1.0), // elongation with _
      note("c4@3 e4 g4") |> sustain(1.0)    // same rhythm via @weight
    ])
    play_feature_cps(pat, 0.5lf)
}
