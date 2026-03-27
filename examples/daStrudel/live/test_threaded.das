options gen2
options indenting = 4

// Test threaded strudel playback under plain daslang.exe.
// Plays for 5 seconds, changes BPM, plays 2 more, then shuts down.

require strudel/strudel
require strudel/strudel_player
require audio/audio_boost
require daslib/fio

let MEDIA = "{get_das_root()}/examples/media"

def strudel_main(var state : StrudelState?) {
    var pat <- s("bd sd [hh hh] cp")
    strudel_set_bpm(120.0lf)
    strudel_play_pattern(state, pat)
}

[export]
def main() {
    print("threaded strudel test\n")
    with_audio_system() <| $() {
        print("loading samples...\n")
        strudel_load_sound("{MEDIA}/drums/bd", "bd")
        strudel_load_sound("{MEDIA}/drums/sd", "sd")
        strudel_load_sound("{MEDIA}/drums/hh", "hh")
        strudel_load_sound("{MEDIA}/drums/cp", "cp")
        print("starting threaded playback...\n")
        strudel_init(@@strudel_main)
        print("playing 5s @ 120 BPM...\n")
        sleep(5000u)
        print("changing to 140 BPM...\n")
        strudel_set_bpm(140.0lf)
        sleep(2000u)
        print("shutting down strudel...\n")
        strudel_shutdown()
    }
    print("done\n")
}
