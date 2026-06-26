options gen2
options persistent_heap
options indenting = 4

require strudel/strudel
require strudel/strudel_player
require audio/audio_boost
require jobque

// WASM strudel smoke test — a synth-only arpeggio through the init/update/
// shutdown browser lifecycle, in strudel's MAIN-THREAD playback mode
// (strudel_create_channel + strudel_tick). Single-threaded: strudel_tick renders
// PCM on the main thread and miniaudio's emscripten (ScriptProcessor) device
// callback consumes it on the SAME thread. No samples/SF2 — pure synth voices,
// so nothing to embed. Same .das on desktop via main().

var g_audio_cmd : Stream?
var g_audio_sid : Atomic64?
var g_t0 = ref_time_ticks()   // playback start, reset in init()

let FEED_SECONDS = 6.0lf   // play the arpeggio for ~6 s ...
let END_SECONDS  = 7.0lf   // ... then drain ~1 s and end the loop (wall-clock, fps-independent)

[export]
def init {
    var ch = audio_system_create()
    g_audio_cmd = ch.command
    g_audio_sid = ch.next_sid
    strudel_create_channel()
    strudel_set_cps(0.5lf)
    // 6-note up/down sine arpeggio (C major triad), gentle ADSR.
    let pat <- note("c4 e4 g4 c5 g4 e4", "sine") |> attack(0.01) |> decay(0.1) |> sustain(0.4) |> release(0.2)
    strudel_add_track(pat)
    g_t0 = ref_time_ticks()
    print("strudel: playing synth arpeggio\n")
}

[export]
def update : bool {
    let elapsed = double(get_time_usec(g_t0)) / 1000000.0lf
    if (elapsed < FEED_SECONDS) {
        strudel_tick()   // self-regulating: skips when the buffer is far enough ahead
    }
    return elapsed < END_SECONDS   // false -> loop ends, shutdown() runs
}

[export]
def shutdown {
    // Full clean teardown — no leaks on single-threaded wasm: strudel_shutdown now
    // drains the audio command queue so deferred ref releases run, and the
    // Stream/LockBox joins are no-ops on this build (job_que.cpp).
    strudel_shutdown()
    audio_system_finalize(g_audio_cmd, g_audio_sid)
    print("strudel: stopped\n")
}

// Desktop driver (never called on web). Busy loop — no sleep (not in the wasm build).
[export]
def main {
    init()
    while (update()) {}
    shutdown()
}
