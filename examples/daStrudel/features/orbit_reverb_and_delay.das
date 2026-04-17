options gen2
options indenting = 4
options persistent_heap

// Two layers with distinct processing. Orbit 0: reverb only. Orbit 1: delay only.
// Hear: pluck dissolving into reverb vs arpeggiated echo.
/*
stack(
  note("c4 e4 g4 c5").s("sine")
    .attack(0.001).decay(0.1).sustain(0).release(0.01)
    .room(0.8).roomsize(3).orbit(0),
  note("e5 ~ g5 ~ c6 ~ e6 ~").s("sine")
    .attack(0.001).decay(0.08).sustain(0).release(0.01)
    .delay(0.6).delaytime(0.375).delayfeedback(0.5).orbit(1)
).cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- stack([
        note("c4 e4 g4 c5", "sine") |> attack(0.001) |> decay(0.1) |> sustain(0.0) |> release(0.01) |> room(0.8) |> roomsize(3.0) |> orbit(0),
        note("e5 ~ g5 ~ c6 ~ e6 ~", "sine") |> attack(0.001) |> decay(0.08) |> sustain(0.0) |> release(0.01) |> delay(0.6) |> delaytime(0.375) |> delayfeedback(0.5) |> orbit(1)
    ])
    play_feature_cps(pat, 0.5lf)
}
