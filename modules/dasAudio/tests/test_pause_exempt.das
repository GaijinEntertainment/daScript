options gen2
require _audio_output_fixture
require audio/audio_boost
require daslib/fio
require daslib/jobque_boost
require debugapi
require math
require dastest/testing_boost

[test]
def global_pause_keeps_ordinary_pcm_silent_but_exempt_pcm_is_audible(t : T?) {
    with_output_probe() $(box) {
        set_global_pause(true)
        var ordinary <- constant_pcm(0.7)
        let normal = play_sound_from_pcm(MA_SAMPLE_RATE, 1, ordinary)
        let quiet = observe_output(box)
        t |> success(quiet.callbacks >= 8); t |> equal(quiet.peak, 0.0)
        unsafe(invoke_in_context(mixer_context(), "capture_reset"))
        var alert <- constant_pcm(0.2)
        let exempt = play_sound_from_pcm(MA_SAMPLE_RATE, 1, alert, INVALID_SID, true)
        sleep(100u)
        let audible = observe_output(box)
        t |> success(audible.peak > 0.1 && audible.peak < 0.3,
                     "exempt samples render while ordinary samples stay paused")
        stop(exempt, 0.0); set_global_pause(false)
        unsafe(invoke_in_context(mixer_context(), "capture_reset"))
        sleep(100u)
        let resumed = observe_output(box)
        t |> success(resumed.peak > 0.6, "ordinary playback resumes from its retained source")
        stop(normal, 0.0)
    }
}

[test]
def exempt_pcm_keeps_individual_pause_stop_and_master_volume_controls(t : T?) {
    with_output_probe() $(box) {
        set_global_pause(true)
        var samples <- constant_pcm(0.4)
        let sid = play_sound_from_pcm(MA_SAMPLE_RATE, 1, samples, INVALID_SID, true)
        sleep(100u)
        t |> success(observe_output(box).peak > 0.3)
        set_pause(sid, true, 0.0); sleep(50u)
        unsafe(invoke_in_context(mixer_context(), "capture_reset"))
        sleep(100u); t |> equal(observe_output(box).peak, 0.0)
        set_pause(sid, false, 0.0); sleep(50u)
        unsafe(invoke_in_context(mixer_context(), "capture_reset"))
        sleep(100u); t |> success(observe_output(box).peak > 0.3)
        set_global_volume(0.0); sleep(50u)
        unsafe(invoke_in_context(mixer_context(), "capture_reset"))
        sleep(100u); t |> equal(observe_output(box).peak, 0.0)
        set_global_volume(1.0); stop(sid, 0.0); sleep(50u)
        unsafe(invoke_in_context(mixer_context(), "capture_reset"))
        sleep(100u); t |> equal(observe_output(box).peak, 0.0)
    }
}
[test]
def combined_buses_stay_bounded_and_finished_exempt_pcm_is_removed(t : T?) {
    with_output_probe() $(box) {
        var ordinary <- constant_pcm(0.8); var exempt <- constant_pcm(0.8)
        let normal = play_sound_from_pcm(MA_SAMPLE_RATE, 1, ordinary)
        let alert = play_sound_from_pcm(MA_SAMPLE_RATE, 1, exempt, INVALID_SID, true)
        sleep(100u)
        let together = observe_output(box)
        t |> success(together.peak > 0.9 && together.peak <= 0.950001)
        stop(normal, 0.0)
        sleep(1100u)
        unsafe(invoke_in_context(mixer_context(), "capture_reset"))
        sleep(100u); t |> equal(observe_output(box).peak, 0.0)
    }
}

[test]
def populated_ordinary_buffer_stays_frozen_during_exempt_playback(t : T?) {
    with_output_probe() $(box) {
        var ordinary <- constant_pcm(0.7)
        let normal = play_sound_from_pcm(MA_SAMPLE_RATE, 1, ordinary)
        sleep(100u); t |> success(observe_output(box).peak > 0.6)
        set_global_pause(true); sleep(50u)
        unsafe(invoke_in_context(mixer_context(), "capture_reset"))
        var alert <- constant_pcm(0.2)
        let exempt = play_sound_from_pcm(MA_SAMPLE_RATE, 1, alert, INVALID_SID, true)
        sleep(100u)
        let isolated = observe_output(box)
        t |> success(isolated.peak > 0.19 && isolated.peak < 0.21)
        set_global_pause(false); sleep(50u)
        unsafe(invoke_in_context(mixer_context(), "capture_reset"))
        sleep(100u)
        let combined = observe_output(box)
        t |> success(combined.peak > 0.89 && combined.peak < 0.91)
        stop(normal, 0.0); stop(exempt, 0.0)
    }
}
[test]
def zero_exempt_output_preserves_existing_ordinary_transients(t : T?) {
    with_output_probe() $(box) {
        var impulse <- constant_pcm(0.0); impulse[64] = 1.0
        let normal = play_sound_from_pcm(MA_SAMPLE_RATE, 1, impulse, 77ul, false)
        sleep(100u)
        let peak = observe_output(box).peak
        t |> success(peak > 0.95,
            "ordinary-only impulses keep the existing smoothed limiter behavior: {peak}")
        stop(normal, 0.0)
    }
}
[test]
def explicit_sid_batched_controls_and_natural_completion_publish_status(t : T?) {
    with_output_probe() $(box) {
        var status = seq_box_create()
        set_global_pause(true)
        batch() $ {
            var samples <- constant_pcm(0.4)
            let sid = play_sound_from_pcm(MA_SAMPLE_RATE, 1, samples, 88ul, true)
            t |> equal(sid, 88ul)
            set_volume(sid, 0.5, 0.0); set_pitch(sid, 2.0)
            set_status_update(sid, status)
        }
        sleep(100u)
        let output = observe_output(box)
        t |> success(output.peak > 0.19 && output.peak < 0.21)
        sleep(600u)
        var final_status = AudioChannelStatus()
        status |> read() $(sample : AudioChannelStatus) { final_status = sample }
        t |> equal(final_status.state, AudioChannelState.stopped,
            "natural EOF removes the channel and publishes stopped")
        status |> seq_box_release
    }
}
[test]
def looping_pcm_and_repeated_context_teardown_release_handles(t : T?) {
    let baseline = count_jobque_leaks()
    for (_ in range(2)) {
        with_output_probe() $(box) {
            var loop : array<float>; resize(loop, 16)
            for (sample in loop) { sample = 0.15 }
            let ordinary = play_sound_loop_from_pcm(MA_SAMPLE_RATE, 1, loop)
            var alert <- constant_pcm(0.2)
            let exempt = play_sound_from_pcm(MA_SAMPLE_RATE, 1, alert, 99ul, true)
            sleep(100u)
            unsafe(invoke_in_context(mixer_context(), "capture_reset"))
            sleep(100u)
            let output = observe_output(box)
            t |> success(output.peak > 0.34 && output.peak < 0.36)
        }
        t |> equal(count_jobque_leaks(), baseline)
    }
}

[test]
def resume_preserves_exact_ordinary_sequence_without_replaying_exempt_pcm(t : T?) {
    with_output_probe() $(box) {
        var sequence : array<float>; resize(sequence, MA_SAMPLE_RATE)
        let step = 0.2 / float(MA_SAMPLE_RATE)
        for (sample, index in sequence, count()) { sample = 0.1 + float(index) * step }
        let normal = play_sound_from_pcm(MA_SAMPLE_RATE, 1, sequence)
        sleep(100u)
        set_global_pause(true)
        let frozen = observe_output(box, true)
        t |> success(frozen.last_nonzero > 0.1)
        var marker : array<float>; resize(marker, 960)
        for (sample in marker) { sample = -0.2 }
        let exempt = play_sound_from_pcm(MA_SAMPLE_RATE, 1, marker, INVALID_SID, true)
        sleep(100u)
        let finished = observe_output(box, true)
        t |> success(finished.last_nonzero < -0.19)
        unsafe(invoke_in_context(mixer_context(), "capture_reset"))
        set_global_pause(false)
        let resumed = observe_output(box)
        t |> success(abs(resumed.first_nonzero - frozen.last_nonzero - step) < 0.00001,
            "the next ordinary sample resumes; the negative exempt marker never replays")
        stop(normal, 0.0)
    }
}
