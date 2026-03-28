options gen2
options indenting = 4

// 100% synthesized — no sample files needed.
// Drums (bd/sd/hh/cp) + bass (sine) + lead (sawtooth)

require strudel/strudel
require strudel/strudel_player
require audio/audio_boost
require daslib/fio

def strudel_main(var state : StrudelState?) {
    strudel_set_bpm(state, 120.0lf)
    strudel_play_pattern(state, stack([
        s("bd sd [hh hh] cp"),
        set_gain(0.4, note_pattern("c2 c2 f2 g2", "sine")),
        set_gain(0.3, note_pattern("<e4 d4> <c4 d4> <e4 f4> <g4 e4>", "sawtooth"))
    ]))
}

[export]
def main() {
    print("daStrudel synth demo (100%% generated)\n")
    with_audio_system() {
        strudel_init(@@strudel_main)
        print("playing 10 seconds...\n")
        sleep(10000u)
        strudel_shutdown()
    }
    print("done\n")
}
