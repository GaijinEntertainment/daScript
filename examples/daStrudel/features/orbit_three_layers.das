options gen2
options indenting = 4
options persistent_heap

// Three layers, each on its own orbit with different processing.
// Orbit 0: bass with big reverb. Orbit 1: melody with delay. Orbit 2: drums, dry.
// Hear: each layer occupies its own spatial space.
/*
stack(
  note("c2 ~ c2 ~ g1 ~ g1 ~").s("sawtooth")
    .attack(0.01).decay(0.1).sustain(0.3).release(0.1)
    .lpf(200).room(0.6).roomsize(5).orbit(0),
  note("e4 g4 c5 e5").s("sine")
    .attack(0.001).decay(0.1).sustain(0).release(0.01)
    .delay(0.4).delaytime(0.75).delayfeedback(0.4).orbit(1),
  s("bd ~ sd ~ bd ~ sd hh").orbit(2)
).cps(0.5)
*/

require feature_common

[export]
def main() {
    var pat <- stack([
        note_pattern("c2 ~ c2 ~ g1 ~ g1 ~", "sawtooth") |> set_attack(0.01) |> set_decay(0.1) |> set_sustain(0.3) |> set_release(0.1) |> set_lpf(200.0) |> set_room(0.6) |> set_roomsize(5.0) |> set_orbit(0),
        note_pattern("e4 g4 c5 e5", "sine") |> set_attack(0.001) |> set_decay(0.1) |> set_sustain(0.0) |> set_release(0.01) |> set_delay(0.4) |> set_delaytime(0.75) |> set_delayfeedback(0.4) |> set_orbit(1),
        s("bd ~ sd ~ bd ~ sd hh") |> set_orbit(2)
    ])
    let media = "{get_das_root()}/examples/media"
    play_feature_cps(pat, 0.5lf) <| $ {
        strudel_load_sound("{media}/drums/bd", "bd")
        strudel_load_sound("{media}/drums/sd", "sd")
        strudel_load_sound("{media}/drums/hh", "hh")
    }
}
