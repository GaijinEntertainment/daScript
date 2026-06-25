options gen2
options persistent_heap
options indenting = 4

require audio/audio_boost
require jobque
require math

// WASM audio smoke test — a continuous 440 Hz sine through the init/update/
// shutdown browser lifecycle. miniaudio's emscripten backend (Web Audio /
// ScriptProcessorNode) runs its device callback on the MAIN thread, so the
// whole audio path is single-threaded here: audio_system_create() spawns no
// threads and strudel-style blocking is avoided. update() re-triggers the tone
// once per second so there's continuous sound for as long as the page runs.
//
// Same .das on desktop via main(); on the web the run path drives
// init/update/shutdown and persists the Context across browser frames.

var g_audio_cmd : Stream?
var g_audio_sid : Atomic64?
var g_frame : int = 0

def play_beep() {
    // exactly one second of a 0.3-amplitude 440 Hz sine (mono); the audio system
    // upmixes mono -> MA_CHANNELS. play_sound_from_pcm takes ownership (move).
    var samples <- [for (x in range(MA_SAMPLE_RATE)); 0.3 * sin(2.0 * PI * 440.0 * float(x) / float(MA_SAMPLE_RATE))]
    play_sound_from_pcm(MA_SAMPLE_RATE, 1, samples)
}

[export]
def init {
    var ch = audio_system_create()
    g_audio_cmd = ch.command
    g_audio_sid = ch.next_sid
    play_beep()
    print("audio: system up, playing 440 Hz sine\n")
}

[export]
def update : bool {
    g_frame ++
    // ~60 fps -> re-trigger every second for a continuous tone
    if (g_frame % 60 == 0) {
        play_beep()
    }
    return true
}

[export]
def shutdown {
    audio_system_finalize(g_audio_cmd, g_audio_sid)
    print("audio: system down\n")
}

// Desktop driver (never called on web; the web run path drives init/update/
// shutdown directly). Busy loop — no sleep (sleep isn't in the wasm build).
[export]
def main {
    init()
    for (_i in range(180)) {
        update()
    }
    shutdown()
}
