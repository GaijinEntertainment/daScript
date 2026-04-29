options gen2
options indenting = 4
options persistent_heap

// HRTF positions; pan widens — pan and HRTF combine, they don't fight.
// Two layers: identical sine notes, both with pan(0.5).
//   Layer 0: pan only — equal-power stereo, source feels "centred-ish" but flat.
//   Layer 1: pan(0.5) plus hrtf(-90, 0) — HRTF places the source far-left in 3D;
//            the pre-pan L/R imbalance is preserved as source width.
// Hear: only layer 1 has the externalised left-side feeling; pan(0.5) tilts the
// width but does not move it from the HRTF-set position.

require feature_common

[export]
def main() {
    let pat <- stack([
        note("c4", "sine") |> pan(0.5)                       |> attack(0.01) |> release(0.4) |> gain(0.4),
        note("e4", "sine") |> pan(0.5) |> hrtf(-90.0, 0.0)   |> attack(0.01) |> release(0.4) |> gain(0.4)
    ])
    play_feature_cps(pat, 0.5lf)
}
