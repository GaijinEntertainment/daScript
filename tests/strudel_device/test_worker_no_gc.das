options gen2
options indenting = 4
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost
require strudel/strudel_mini
require strudel/strudel_player
require audio/audio_boost
require daslib/fio

// A program without `options gc` and `options persistent_heap` cannot collect its worker's
// heap: the probe collect at the worker's start throws, the arm logs one warning and keeps the
// heap for the run, and the bed still plays and shuts down clean. Null backend, headless.

def worker_setup() {
    strudel_set_cps(2.0lf)
    strudel_add_track(s("<bd [hh hh] sd [hh hh]>"), 0.5)
    strudel_add_track(note_pattern("<[a2 a2] [f2 f2]> <[c3 c3] [e3 e3]>", "square"), 0.3)
    strudel_play()
}

[test]
def test_worker_without_gc_keeps_playing(t : T?) {
    t |> run("a worker whose program cannot collect plays on with its heap uncollected") @(t : T?) {
        sound_set_null_device(true)
        with_audio_system() {
            strudel_init(@@worker_setup)
            sleep(2500u)
            let a = strudel_get_worker_heap_bytes()
            sleep(1500u)
            let b = strudel_get_worker_heap_bytes()
            t |> success(a > 0ul, "the worker publishes its heap")
            t |> success(b >= a, "with no collect the heap only grows: {int64(a) / 1024l} KB then {int64(b) / 1024l} KB")
            strudel_shutdown()
        }
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}
