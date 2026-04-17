options gen2
options indenting = 4
options persistent_heap

// sometimes(fast(2)) — occasional double-time fills
// Hear: drum pattern with random cycles at double speed.
/*
s("bd sd hh cp")
  .sometimes(x => x.fast(2))
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let media = "{get_das_root()}/examples/media"
    let pat <- sometimes(s("bd sd hh cp"), @(x) => fast(x, 2.0lf))
    play_feature_cps(pat, 0.5lf) {
        strudel_load_sound("{media}/drums/bd", "bd")
        strudel_load_sound("{media}/drums/sd", "sd")
        strudel_load_sound("{media}/drums/hh", "hh")
        strudel_load_sound("{media}/drums/cp", "cp")
    }
}
