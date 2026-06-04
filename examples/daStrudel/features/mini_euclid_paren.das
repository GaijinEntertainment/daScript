options gen2
options indenting = 4
options persistent_heap

// Euclidean rhythms straight from mini-notation: "name(k,n)" expands to
// euclid(k,n), and "name(k,n,rot)" to euclidRot(k,n,rot). Composes with the
// rest of the mini-notation grammar inside one string.
// Hear: a tresillo kick under a 5-in-8 hat, both written inline.
/*
s("bd(3,8), hh(5,8,1)")
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let media = "{get_das_root()}/examples/media"
    let pat <- stack([
        s("bd(3,8)") |> gain(0.8),
        s("hh(5,8,1)") |> gain(0.4)
    ])
    play_feature_cps(pat, 0.5lf) {
        strudel_load_sound("{media}/drums/bd", "bd")
        strudel_load_sound("{media}/drums/hh", "hh")
    }
}
