options gen2
options indenting = 4
options persistent_heap

// euclidRot(k, n, rot) is a Euclidean rhythm rotated left by `rot` steps.
// Hear: euclid(3,8) is the tresillo (onsets 0,3,6); rotating by 2 shifts the
// accent pattern, giving the same density with a different feel.
/*
s("bd").euclidRot(3, 8, 2)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let media = "{get_das_root()}/examples/media"
    let pat <- s("bd") |> euclidRot(3, 8, 2) |> gain(0.7)
    play_feature_cps(pat, 0.5lf) {
        strudel_load_sound("{media}/drums/bd", "bd")
    }
}
