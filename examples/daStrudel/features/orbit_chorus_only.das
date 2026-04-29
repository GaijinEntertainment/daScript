options gen2
options indenting = 4
options persistent_heap

// Two orbits, only one with chorus — proves chorus is per-orbit (not shared across orbits).
// Orbit 0: sawtooth chord with chorus shimmer.
// Orbit 1: same chord shape on triangle, dry (no chorus).
// Hear: orbit 0 detunes/widens; orbit 1 stays clean even though both play simultaneously.
/*
stack(
  note("<[c3,e3,g3] [f3,a3,c4]>").s("sawtooth").chorus(0.7).orbit(0),
  note("<[c4,e4,g4] [f4,a4,c5]>").s("triangle").orbit(1)
).cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- stack([
        note("<[c3,e3,g3] [f3,a3,c4]>", "sawtooth") |> chorus(0.7) |> orbit(0) |> attack(0.05) |> release(0.3) |> gain(0.4),
        note("<[c4,e4,g4] [f4,a4,c5]>", "triangle") |> orbit(1) |> attack(0.05) |> release(0.3) |> gain(0.4)
    ])
    play_feature_cps(pat, 0.5lf)
}
