options gen2
options indenting = 4

// 100% synthesized — no sample files needed.
// Drums (bd/sd/hh/cp) + bass (sine) + lead (sawtooth)

require strudel/strudel
require strudel/strudel_player
require audio/audio_boost
require daslib/fio

def strudel_main(var state : StrudelState?) {
    var pats : array<Pattern>
    // drums
    var drums <- s("bd sd [hh hh] cp")
    pats |> emplace <| drums
    // bass line (sine oscillator, low octave)
    var bass <- set_gain(0.4, note_pattern("c2 c2 f2 g2", "sine"))
    pats |> emplace <| bass
    // lead melody (sawtooth, higher octave)
    var lead <- set_gain(0.3, note_pattern("<e4 d4> <c4 d4> <e4 f4> <g4 e4>", "sawtooth"))
    pats |> emplace <| lead
    var pat <- stack(pats)
    strudel_set_bpm(120.0lf)
    strudel_play_pattern(state, pat)
}

[export]
def main() {
    print("daStrudel synth demo (100%% generated)\n")
    with_audio_system() <| $() {
        strudel_init(@@strudel_main)
        print("playing 10 seconds...\n")
        sleep(10000u)
        strudel_shutdown()
    }
    print("done\n")
}
