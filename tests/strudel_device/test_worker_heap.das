options gen2
options indenting = 4
options persistent_heap
options gc
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost
require strudel/strudel
require strudel/strudel_player
require audio/audio_boost
require daslib/fio

// The playback worker's context is its own: nothing else collects it, and a bed that queries
// four patterns every tick leaves megabytes of garbage a second behind. The scheduler's pools
// grow for the first few seconds and then hold, so the arm samples after that warm-up and pins
// that the heap stays within one collect window from there, over a stretch that would have
// grown many windows' worth uncollected. Null backend, so it runs headless.

def worker_setup() {
    strudel_set_cps(2.0lf)
    strudel_add_track(s("<bd [hh hh] sd [hh hh]> <bd [hh cp?0.5] sd [hh hh]>"), 0.5)
    strudel_add_track(note_pattern("<[a2 a2] [a2 a2] [f2 f2] [g2 g2]> <[a2 a2] [c3 c3] [d3 d3] [e3 e3]>", "square"), 0.3)
    strudel_add_track(note_pattern("<a4 c5 e5 a5> <g5 e5 c5 ~> <[a4 b4] c5 e5 ~>", "sawtooth"), 0.25)
    strudel_add_track(note_pattern("<[a4 c5 e5 a5 c6 a5 e5 c5]> <[e5 a5 c6 e6 c6 a5 e5 a4]>", "triangle"), 0.12)
    strudel_play()
}

[test]
def test_worker_heap_stays_flat(t : T?) {
    t |> run("the playback worker collects its own heap: a four-track bed holds within a few collect windows past its warm-up") @(t : T?) {
        sound_set_null_device(true)
        with_audio_system() {
            strudel_init(@@worker_setup)
            sleep(6000u)
            let base = strudel_get_worker_heap_bytes()
            t |> success(base > 0ul, "the worker publishes its heap")
            var peak = base
            for (_i in range(12)) {
                sleep(500u)
                peak = max(peak, strudel_get_worker_heap_bytes())
            }
            strudel_shutdown()
            // the published number is the heap before the tick's collect, so a peak sits one collect
            // window plus one tick's garbage above a base sampled at a trough - and a starved CI tick
            // leaves five megabytes, where an uncollected worker grows twenty in the same six seconds
            let LIMIT = 10ul * 1024ul * 1024ul
            t |> success(peak < base + LIMIT, "the worker heap grew {int64(peak - base) / 1024l} KB in the six seconds past warm-up")
        }
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}
