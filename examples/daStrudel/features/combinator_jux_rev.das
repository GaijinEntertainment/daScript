options gen2
options indenting = 4
options persistent_heap

// jux(rev) — classic stereo widening
// Hear: original drum pattern in left ear, reversed in right ear.
/*
s("bd sd hh cp")
  .jux(rev)
  .gain(0.4)
  .cps(0.5)
*/

require feature_common

[export]
def main() {
    let media = "{get_das_root()}/examples/media"
    var pat <- jux(s("bd sd hh cp"), @(x) => rev(x))
    play_feature_cps(pat, 0.5lf) <| $ {
        strudel_load_sound("{media}/drums/bd", "bd")
        strudel_load_sound("{media}/drums/sd", "sd")
        strudel_load_sound("{media}/drums/hh", "hh")
        strudel_load_sound("{media}/drums/cp", "cp")
    }
}
