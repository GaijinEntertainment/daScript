options gen2
require audio/audio_boost
require daslib/fio
require daslib/jobque_boost
require daslib/runtime_memory
require dastest/testing_boost public

//! What the mixer publishes about itself on the null device: its
//! context memory, playback diagnostics, PCM stream underruns and
//! callback timing.

def private await_memory(box : SeqBox?) : ContextMemory {
    var sample = ContextMemory()
    for (_i in range(250)){
        box |> read() $(value : ContextMemory){sample = value}
        if (sample.stack > 0ul){break}
        sleep(10u)
    }
    return sample
}

[test(description = "The mixer publishes its context memory into a registered box without a stats box, stops publishing into a box once another replaces it, and every box share is released at the end.", timeout = 1200)]
def memory_box_publishes_without_stats_and_replaces_ownership(t : T?){
    let before = count_jobque_leaks()
    sound_set_null_device(true)
    with_audio_system() $ {
        var first = seq_box_create(); var second = seq_box_create()
        set_audio_memory_box(first)
        let first_value = await_memory(first)
        t |> success(first_value.stack > 0ul && first_value.heap_reserved > 0ul)
        set_audio_memory_box(second)
        let second_value = await_memory(second)
        t |> success(second_value.stack > 0ul)
        first |> publish(ContextMemory(stack = 777ul))
        sleep(1100u)
        first |> read() $(old : ContextMemory){t |> equal(old.stack, 777ul, "replaced box no longer receives worker snapshots")}
        first |> seq_box_release
        var none : SeqBox?; set_audio_memory_box(none)
        second |> seq_box_release
    }
    sound_set_null_device(false)
    t |> equal(count_jobque_leaks(), before, "audio registrations release every box share")
}

[test(description = "On native builds get_playback_diagnostics reports zero for every counter, since only the browser ring keeps them.")]
def native_playback_diagnostics_are_unavailable_zero(t : T?){
    let stats = get_playback_diagnostics()
    t |> equal(stats.callbacks, 0ul); t |> equal(stats.capacity_frames, 0ul)
    t |> equal(stats.underrun_frames, 0ul); t |> equal(stats.recoveries, 0ul)
}

[test(description = "A PCM stream fed fewer frames than the mixer consumes reports its consumed position and a nonzero underrun count with nothing left queued.", timeout = 1200)]
def pcm_stream_reports_missing_frames(t : T?){
    sound_set_null_device(true)
    with_audio_system() $ {
        let sid = play_sound_from_pcm_stream(MA_SAMPLE_RATE, 2)
        var box = seq_box_create(); set_status_update(sid, box)
        var data : array<float>; resize(data, 960)
        append_to_pcm(sid, data)
        sleep(150u)
        var status = AudioChannelStatus()
        t |> success(box |> read() $(s : AudioChannelStatus){status = s})
        t |> success(status.consumed_position == 480ul)
        t |> success(status.stream_underrun_frames > 0ul && status.stream_underruns > 0ul)
        t |> equal(status.stream_queued_frames, 0ul)
        stop(sid); unset_status_update(sid); box |> seq_box_release
        delete data
    }
    sound_set_null_device(false)
}

[test(description = "A registered stats box receives callback timing with nonzero mix time and callback gap, and a new registration starts a fresh measurement session.", timeout = 1200)]
def mixer_publishes_callback_timing(t : T?){
    sound_set_null_device(true)
    with_audio_system() $ {
        var box = seq_box_create(); set_audio_stats_box(box)
        var first = AudioSystemStats(); var first_publication = 0ul
        for (_i in range(400)){
            sleep(10u); box |> read() $(s : AudioSystemStats){first = s}
            if (first_publication == 0ul && first.callbacks > 0ul){first_publication = first.callbacks}
            elif (first.callbacks > first_publication){break}
        }
        t |> success(first.callbacks > first_publication, "first registration publishes two timing windows")
        t |> success(first.max_mix_ms > 0.0lf)
        t |> success(first.max_callback_gap_ms > 0.0lf)
        var no_box : SeqBox?; set_audio_stats_box(no_box)
        box |> seq_box_release

        sleep(100u)
        var second_box = seq_box_create(); set_audio_stats_box(second_box)
        var second = AudioSystemStats()
        for (_i in range(200)){
            sleep(10u); second_box |> read() $(s : AudioSystemStats){second = s}
            if (second.callbacks > 0ul){break}
        }
        t |> success(second.callbacks > 0ul, "second registration publishes callback timing")
        t |> success(second.callbacks < first.callbacks,
            "a new stats registration starts a fresh measurement session")
        set_audio_stats_box(no_box)
        second_box |> seq_box_release
    }
    sound_set_null_device(false)
}
