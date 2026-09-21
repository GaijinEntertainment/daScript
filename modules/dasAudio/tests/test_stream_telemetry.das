options gen2
require audio/audio_boost
require daslib/fio
require daslib/jobque_boost
require dastest/testing_boost public

[test]
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

[test]
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
