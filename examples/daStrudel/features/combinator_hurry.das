options gen2
options indenting = 4
options persistent_heap

// hurry combines fast + speed — pattern plays faster AND samples pitch up (like tape sped up).
// Hear: drums play at 2x speed with each hit pitched up an octave. Note: speed only
// affects sample playback, not oscillators — use a sample source to hear the pitch shift.
/*
s("bd sd hh cp").hurry(2).gain(0.4).cps(0.5)
*/

require feature_common

[export]
def main() {
    let media = "{get_das_root()}/examples/media"
    let pat <- s("bd sd hh cp") |> hurry(2.0lf) |> gain(0.4)
    play_feature_cps(pat, 0.5lf) {
        strudel_load_sound("{media}/drums/bd", "bd")
        strudel_load_sound("{media}/drums/sd", "sd")
        strudel_load_sound("{media}/drums/hh", "hh")
        strudel_load_sound("{media}/drums/cp", "cp")
    }
}
