options gen2
options indenting = 4

// MIDI player with SF2 SoundFont — plays a .mid file using a GM SoundFont.
// Place FluidR3_GM.sf2 or GeneralUser GS v1.471.sf2 next to this file (see README.md).
// Usage: daslang.exe main.das [-- path_to.mid]
// Default: examples/media/midi/fur_elise.mid

require strudel/strudel_midi_player
require audio/audio_boost
require daslib/fio
require daslib/strings_boost

def find_sf2() : string {
    let dir = "{get_das_root()}/examples/daStrudel/midi_sf2_demo"
    let big = "{dir}/FluidR3_GM.sf2"
    var st : FStat
    if (stat(big, st)) {
        return big
    }
    let small = "{dir}/GeneralUser GS v1.471.sf2"
    if (stat(small, st)) {
        return small
    }
    return ""
}

[export]
def main() {
    let media = "{get_das_root()}/examples/media"
    var path = "{media}/midi/take_five.mid"
    let args <- get_command_line_arguments()
    for (arg in args) {
        if (ends_with(arg, ".mid") || ends_with(arg, ".MID")) {
            path = arg
        }
    }
    let sf2 = find_sf2()
    print("MIDI SF2 player — {path}\n")
    with_audio_system() {
        if (empty(sf2)) {
            print("No SF2 found — falling back to oscillators (see README.md)\n")
        } elif (!midi_load_sf2(sf2)) {
            print("Failed to load {sf2} — falling back to oscillators\n")
        } else {
            print("Loaded {sf2}\n")
        }
        midi_init()
        if (midi_play("music", path, [gain = 1.0, looping = true])) {
            print("playing 60 seconds (looping)...\n")
            sleep(60000u)
            midi_shutdown()
            print("done\n")
        } else {
            print("failed to load MIDI file\n")
        }
    }
}
