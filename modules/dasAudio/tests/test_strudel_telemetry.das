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
