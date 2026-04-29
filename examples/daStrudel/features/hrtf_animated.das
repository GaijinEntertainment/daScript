options gen2
options indenting = 4
options persistent_heap

// Animated HRTF azimuth — pluck sweeps left-to-right via 3D positioning.
// Pattern-valued setter: each event samples the sine signal for its azimuth in degrees.
// Hear: the source orbits the listener, with HRTF cues (not just panning) doing the work.
/*
note("c4 e4 g4 c5").s("triangle").hrtf_azimuth(sine.range(-90, 90).slow(2)).hrtf_elevation(0)
*/

require feature_common

[export]
def main() {
    let pat <- (
        note("c4 e4 g4 c5", "triangle")
        |> hrtf_azimuth(sine() |> range(-90.0, 90.0) |> slow(2.0lf))
        |> hrtf_elevation(0.0)
        |> attack(0.005)
        |> release(0.25)
        |> gain(0.5)
    )
    play_feature_cps(pat, 0.5lf)
}
