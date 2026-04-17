options gen2
options indenting = 4
options persistent_heap

// striate cuts each sample into n slices via begin/end.
// Hear: drum pattern sped up by 4 — each event plays a progressive quarter of the sample,
// so successive hits reveal later portions of the sample's natural decay envelope.
/*
s("bd sd hh cp")
  .striate(4)
  .gain(0.4)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let media = "{get_das_root()}/examples/media"
    let pat <- s("bd sd hh cp") |> striate(4) |> gain(0.4)
    play_feature_cps(pat, 0.5lf) {
        strudel_load_sound("{media}/drums/bd", "bd")
        strudel_load_sound("{media}/drums/sd", "sd")
        strudel_load_sound("{media}/drums/hh", "hh")
        strudel_load_sound("{media}/drums/cp", "cp")
    }
}
