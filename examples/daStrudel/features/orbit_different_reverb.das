options gen2
options indenting = 4
options persistent_heap

// Two layers on separate orbits with different reverb sizes.
// Orbit 0: tight small-room reverb. Orbit 1: expansive large-room reverb.
// Hear: pluck in a small space vs pad in a cathedral.
/*
stack(
  note("c4 ~ e4 ~").s("sine")
    .attack(0.001).decay(0.15).sustain(0).release(0.01)
    .room(0.8).roomsize(0.5).orbit(0),
  note("c3").s("triangle")
    .attack(0.5).decay(0.2).sustain(0.6).release(0.5)
    .room(0.8).roomsize(8).orbit(1)
).cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- stack([
        note("c4 ~ e4 ~", "sine") |> attack(0.001) |> decay(0.15) |> sustain(0.0) |> release(0.01) |> room(0.8) |> roomsize(0.5) |> orbit(0),
        note("c3", "triangle") |> attack(0.5) |> decay(0.2) |> sustain(0.6) |> release(0.5) |> room(0.8) |> roomsize(8.0) |> orbit(1)
    ])
    play_feature_cps(pat, 0.5lf)
}
