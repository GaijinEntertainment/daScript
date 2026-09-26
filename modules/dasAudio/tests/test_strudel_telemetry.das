options gen2
options gc
options persistent_heap
require strudel/strudel_player
require audio/audio_boost
require daslib/fio
require dastest/testing_boost public

def private telemetry_worker(){strudel_play()}

[test]
def threaded_playback_publishes_consumer_and_refill_observations(t : T?){
    sound_set_null_device(true)
    with_audio_system() $ {
        strudel_init(@@telemetry_worker)
        var stats = StrudelDiagnostics()
        for (_i in range(100)){sleep(10u); stats = strudel_get_diagnostics(); if (stats.refills > 2ul && stats.consumed_frames > 0ul){break}}
        t |> success(stats.refills > 0ul && stats.generated_frames > 0ul)
        t |> success(stats.consumed_frames > 0ul)
        t |> success(stats.max_refill_ms >= 0.0 && stats.max_refill_gap_ms >= 0.0)
        strudel_shutdown()
    }
    sound_set_null_device(false)
}

[test]
def main_thread_playback_reports_consumption_and_resets(t : T?){
    sound_set_null_device(true)
    with_audio_system() $ {
        strudel_create_channel()
        var stats = StrudelDiagnostics()
        for (_i in range(100)){
            strudel_tick(); sleep(10u); stats = strudel_get_diagnostics()
            if (stats.refills > 2ul && stats.consumed_frames > 0ul){break}
        }
        t |> success(!stats.threaded)
        t |> success(stats.refills > 0ul && stats.generated_frames > 0ul)
        t |> success(stats.consumed_frames > 0ul)
        strudel_shutdown()
        let reset = strudel_get_diagnostics()
        t |> equal(reset.generated_frames, 0ul)
        t |> equal(reset.consumed_frames, 0ul)
    }
    sound_set_null_device(false)
}

[test]
def paused_consumer_bounds_threaded_generation(t : T?){
    sound_set_null_device(true)
    with_audio_system() $ {
        for (global in fixed_array(false, true)){
            strudel_init(@@telemetry_worker)
            sleep(300u); if (global){set_global_pause(true)}else{strudel_set_pause(true)}; sleep(1000u)
            let before = strudel_get_diagnostics()
            sleep(1000u)
            let after = strudel_get_diagnostics()
            if (global){set_global_pause(false)}else{strudel_set_pause(false)}; strudel_shutdown()
            t |> success(after.generated_frames - before.generated_frames <= 4800ul, "paused stream must stop generation when its bounded queue is full (global={global})")
            t |> success(after.generated_frames - after.consumed_frames <= 14400ul, "at most six 50ms chunks including in-flight output")
        }
    }
    sound_set_null_device(false)
}

require strudel/strudel_pattern
[test]
def track_controls_survive_playback_restart(t : T?){
    strudel_shutdown()
    for (_cycle in range(2)){
        let pat <- silence(); let track = strudel_add_track_immediate(pat, .75)
        t |> equal(strudel_track_gain(track), .75)
        strudel_fade_track(track, .25, 0.0)
        t |> equal(strudel_track_gain(track), .25)
        strudel_shutdown()
        t |> equal(strudel_track_gain(track), 0.0)
    }
}
