options gen2
options indenting = 4
options persistent_heap

// run generates a discrete ramp 0..n-1 across each cycle.
// Hear: 8-step rising chromatic sequence C3..G3 — each step 1/8 of a cycle.
/*
note(run(8).add(48)).s("sine").sustain(0.3).cps(0.5)
*/

require feature_common

[export]
def main() {
    // run(8) produces notes 0..7 across a cycle → add 48 for chromatic C3..G3 (MIDI 48..55)
    let pat <- run(8) |> add(48.0) |> sound("sine") |> sustain(0.3)
    play_feature_cps(pat, 0.5lf)
}
