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
    var pat <- stack([
        note_pattern("c4 e4 g4 c5", "sine") |> set_attack(0.001) |> set_decay(0.1) |> set_sustain(0.0) |> set_release(0.01) |> set_room(0.8) |> set_roomsize(3.0) |> set_orbit(0),
        note_pattern("e5 ~ g5 ~ c6 ~ e6 ~", "sine") |> set_attack(0.001) |> set_decay(0.08) |> set_sustain(0.0) |> set_release(0.01) |> set_delay(0.6) |> set_delaytime(0.375) |> set_delayfeedback(0.5) |> set_orbit(1)
    ])
    play_feature_cps(pat, 0.5lf)
}
