options gen2
options indenting = 4
options persistent_heap

// daStrudel HRTF demo — three sources rendered together, ambient flavour.
//
//   Source A (orbit 0) — "the bumblebee": a fast chromatic 16th-note line on
//     sawtooth, HRTF azimuth following sine(-180..180) over 4s, elevation
//     gently rising/falling on cos(-30..30). Sounds like the source is
//     circling the listener while moving up and down.
//   Source B (orbit 1) — "left pad": sustained triangle pad fixed at HRTF
//     azimuth = -90, elevation = 0 (full left).
//   Source C (orbit 1) — "right-front pluck": arpeggiated sine pluck fixed
//     at azimuth = +30, elevation = 0 (right-front).
//
// Orbit 0 (bumblebee) has a long, lush reverb. Orbit 1 (the steady sources)
// shares a smaller, drier reverb. Demonstrates per-orbit reverb still works
// alongside HRTF positioning.

require strudel/strudel
require strudel/strudel_player
require audio/audio_boost
require daslib/fio

def strudel_main() {
    strudel_set_bpm(140.0lf)
    // Source A — bumblebee: rapid chromatic line, animated HRTF position
    var bumblebee <- (
        note("c5 c#5 d5 d#5 e5 f5 f#5 g5 g#5 a5 a#5 b5 b5 a#5 a5 g#5 g5 f#5 f5 e5 d#5 d5 c#5 c5", "sawtooth")
        |> hrtf_azimuth(sine() |> range(-180.0, 180.0) |> slow(4.0lf))
        |> hrtf_elevation(cosine() |> range(-30.0, 30.0) |> slow(3.0lf))
        |> orbit(0)
        |> attack(0.005)
        |> release(0.05)
        |> room(0.6)
        |> roomsize(6.0)
        |> gain(0.45)
    )
    // Source B — left pad
    var left_pad <- (
        note("<c3 g2>", "triangle")
        |> hrtf(-90.0, 0.0)
        |> orbit(1)
        |> attack(0.4)
        |> release(0.6)
        |> sustain(0.6)
        |> room(0.4)
        |> roomsize(2.0)
        |> gain(0.3)
    )
    // Source C — right-front pluck
    var right_pluck <- (
        note("c4 e4 g4 e4", "sine")
        |> hrtf(30.0, 0.0)
        |> orbit(1)
        |> attack(0.005)
        |> release(0.25)
        |> room(0.4)
        |> roomsize(2.0)
        |> gain(0.35)
    )
    strudel_add_track(stack([bumblebee, left_pad, right_pluck]))
    strudel_play()
}

[export]
def main() {
    print("daStrudel HRTF demo — bumblebee circling + left pad + right-front pluck\n")
    with_audio_system() {
        strudel_init(@@strudel_main)
        print("playing 30 seconds...\n")
        sleep(30000u)
        strudel_shutdown()
    }
    print("done\n")
}
