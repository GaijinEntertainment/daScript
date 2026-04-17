options gen2
options indenting = 4
options persistent_heap

// echo — event repetition with gain decay. Each repeat is quieter.
// Hear: pluck plays, then 3 delayed echoes fading out.
/*
note("c4 ~ ~ ~ g3 ~ ~ ~").s("sine")
  .attack(0.001).decay(0.1).sustain(0).release(0.05)
  .echo(4, 1/8, 0.5)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let pat <- note("c4 ~ ~ ~ g3 ~ ~ ~", "sine") |> attack(0.001) |> decay(0.1) |> sustain(0.0) |> release(0.05) |> echo(4, 0.125lf, 0.5)
    play_feature_cps(pat, 0.5lf)
}
