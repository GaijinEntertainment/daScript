options gen2
options indenting = 4

// "Ode to Joy" (Beethoven) on Rhodes piano samples + sample drums.
// Piano samples are pitched to target notes via the speed parameter.

require strudel/strudel
require strudel/strudel_player
require audio/audio_boost
require daslib/fio
require math

let MEDIA = "{get_das_root()}/examples/media"

// Sampled MIDI notes available in examples/media/piano/
let PIANO_NOTES = fixed_array(40, 45, 50, 55, 59, 62, 65, 71, 76)

// Find the closest sampled note and return (sound_name, speed)
def piano_note(midi : int) : tuple<snd : string; speed : float> {
    var best = 62
    var bestDist = 100
    for (n in PIANO_NOTES) {
        let dist = abs(midi - n)
        if (dist < bestDist) {
            bestDist = dist
            best = n
        }
    }
    let semitones = float(midi - best)
    return (snd = "note_{best}", speed = pow(2.0, semitones / 12.0))
}

def strudel_main(var state : StrudelState?) {
    // Ode to Joy — 8 bars (2 lines of 4 bars each)
    // line 1: E4 E4 F4 G4 | G4 F4 E4 D4 | C4 C4 D4 E4 | E4. D4 D4
    // line 2: E4 E4 F4 G4 | G4 F4 E4 D4 | C4 C4 D4 E4 | D4. C4 C4
    let melody = fixed_array(
        64, 64, 65, 67,  67, 65, 64, 62,  60, 60, 62, 64,  64, 62, 62, 62,
        64, 64, 65, 67,  67, 65, 64, 62,  60, 60, 62, 64,  62, 60, 60, 60
    )
    // build mini-notation string with sound names and speed per note
    var pats : array<Pattern>
    pats |> reserve(length(melody))
    for (midi in melody) {
        let pn = piano_note(midi)
        var p <- set_speed(pn.speed, set_gain(0.6, atom(pn.snd)))
        pats |> emplace <| p
    }
    // 32 notes over 8 cycles (4 notes per cycle = 4/4 time)
    var melody_pat <- slow(8.0lf, fastcat(pats))
    // gentle drums — 2 beat pattern, repeated
    var drums <- set_gain(0.5, s("bd ~ sd ~"))
    var layers : array<Pattern>
    layers |> emplace <| melody_pat
    layers |> emplace <| drums
    var pat <- stack(layers)
    strudel_set_bpm(100.0lf)
    strudel_play_pattern(state, pat)
}

def load_piano() {
    for (n in PIANO_NOTES) {
        strudel_load_sound("{MEDIA}/piano/note_{n}", "note_{n}")
    }
}

[export]
def main() {
    print("daStrudel piano demo — Ode to Joy (Rhodes samples)\n")
    with_audio_system() <| $() {
        strudel_load_sound("{MEDIA}/drums/bd", "bd")
        strudel_load_sound("{MEDIA}/drums/sd", "sd")
        strudel_load_sound("{MEDIA}/drums/hh", "hh")
        load_piano()
        strudel_init(@@strudel_main)
        print("playing 20 seconds...\n")
        sleep(20000u)
        strudel_shutdown()
    }
    print("done\n")
}
