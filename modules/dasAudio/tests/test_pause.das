options gen2
require _audio_output_fixture
require audio/audio_boost
require daslib/fio
require daslib/jobque_boost
require debugapi
require math
require dastest/testing_boost

//! Pause on the mixer, read back from the captured output: a
//! channel's own pause, the global pause, and the exempt PCM that
//! plays through a global pause.

[test(description = "Pausing one channel fades it to silence at every fade length without attenuating another playing channel, and resuming makes it audible again.", timeout = 1200)]
def individual_pause_settles_at_zero_without_muting_another_channel(t : T?) {
    for (fade in fixed_array(0.0, 0.002, 0.01)) {
        with_output_probe() $(box) {
            var held <- constant_pcm(0.3); var continuing <- constant_pcm(0.2)
            let first = play_sound_from_pcm(MA_SAMPLE_RATE, 1, held)
            let second = play_sound_from_pcm(MA_SAMPLE_RATE, 1, continuing)
            sleep(100u); t |> success(observe_output(box).peak > 0.4)
            unsafe(invoke_in_context(mixer_context(), "capture_reset"))
            set_pause(first, true, fade); sleep(100u)
            t |> success(observe_output(box).minimum > 0.19,
                         "the other channel is never attenuated during the fade")
            unsafe(invoke_in_context(mixer_context(), "capture_reset"))
            sleep(100u)
            let paused = observe_output(box)
            t |> success(paused.peak > 0.19 && paused.peak < 0.21,
                         "only the continuing channel remains audible")
            stop(second, 0.0); sleep(50u)
            unsafe(invoke_in_context(mixer_context(), "capture_reset"))
            sleep(100u); t |> equal(observe_output(box).peak, 0.0,
                                    "the paused channel stays silent")
            set_pause(first, false, fade); sleep(100u)
            unsafe(invoke_in_context(mixer_context(), "capture_reset"))
            sleep(100u); t |> success(observe_output(box).peak > 0.29,
                                      "resumed channel becomes audible")
            stop(first, 0.0)
        }
    }
}

[test(description = "Under the global pause ordinary PCM renders silence while exempt PCM is audible, and ordinary playback resumes from its retained source when the pause lifts.", timeout = 1200)]
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

[test(description = "Exempt PCM playing through the global pause still obeys its own pause, the master volume and stop.", timeout = 1200)]
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

[test(description = "Ordinary and exempt PCM mixed together stay under the 0.95 limiter ceiling, and exempt PCM that runs to its end is removed from the mix.", timeout = 1200)]
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

[test(description = "An ordinary buffer already playing stays frozen while exempt PCM plays alone under the global pause, and both sum when the pause lifts.", timeout = 1200)]
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

[test(description = "With no exempt output, an ordinary-only impulse keeps the smoothed limiter's existing peak above 0.95.", timeout = 1200)]
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

[test(description = "Exempt PCM started with an explicit sid inside a batch keeps that sid and its batched volume and pitch, and its natural end publishes the stopped state.", timeout = 1200)]
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

[test(description = "A looping ordinary sound mixed with exempt PCM sums correctly, and tearing the audio context down twice leaks no job-queue handle.", timeout = 1200)]
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

[test(description = "After the global pause lifts, an ordinary sequence resumes at exactly the next sample it would have played, and exempt PCM that played during the pause is never replayed.", timeout = 1200)]
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

def private status_state(box : SeqBox?) : AudioChannelState {
    var state = AudioChannelState.playing
    box |> read() $(sample : AudioChannelStatus) { state = sample.state }
    return state
}

[test(description = "Stopping a channel that is paused ends it and publishes the stopped state instead of leaving it paused forever.", timeout = 1200)]
def stopping_a_paused_channel_ends_it(t : T?) {
    with_output_probe() $(_box) {
        var status = seq_box_create()
        var samples <- constant_pcm(0.3)
        let sid = play_sound_loop_from_pcm(MA_SAMPLE_RATE, 1, samples)
        set_status_update(sid, status)
        sleep(100u)
        set_pause(sid, true, 0.0); sleep(100u)
        t |> equal(status_state(status), AudioChannelState.paused)
        stop(sid, 0.0); sleep(100u)
        t |> equal(status_state(status), AudioChannelState.stopped,
                   "the stopped channel is removed even though it was paused")
        status |> seq_box_release
    }
}

[test(description = "A volume change or an ignore-global-volume change sent to a stopping channel is dropped, so the channel still fades out and ends.", timeout = 1200)]
def volume_commands_do_not_revive_a_stopping_channel(t : T?) {
    with_output_probe() $(box) {
        var status = seq_box_create()
        var samples <- constant_pcm(0.3)
        let sid = play_sound_loop_from_pcm(MA_SAMPLE_RATE, 1, samples)
        set_status_update(sid, status)
        sleep(100u); t |> success(observe_output(box).peak > 0.2)
        batch() $ {
            stop(sid, 0.05)
            set_volume(sid, 1.0, 0.0)
            set_ignore_global_volume(sid, true)
        }
        sleep(400u)
        t |> equal(status_state(status), AudioChannelState.stopped,
                   "the late volume commands never lift the fade-out")
        unsafe(invoke_in_context(mixer_context(), "capture_reset"))
        sleep(100u); t |> equal(observe_output(box).peak, 0.0)
        status |> seq_box_release
    }
}

def private far_attenuation() : Attenuation =>
    Attenuation(A = 0., B = 0., C = 0.1, D = 0., E = 0., F = 1.)

def private steady_peak_after(box : SeqBox?; command : block<() : void>) : float {
    unsafe(invoke_in_context(mixer_context(), "capture_reset"))
    sleep(100u)
    let steady = observe_output(box).peak
    unsafe(invoke_in_context(mixer_context(), "capture_reset"))
    invoke(command)
    sleep(100u)
    let after = observe_output(box).peak
    return after / steady
}

[test(description = "A volume or ignore-global-volume command sent to a distant 3D channel leaves its distance-attenuated level alone instead of spiking it to full volume for a callback.", timeout = 1200)]
def volume_commands_keep_the_3d_attenuation(t : T?) {
    with_output_probe() $(box) {
        var samples <- constant_pcm(0.5)
        let sid = play_3d_sound_loop_from_pcm(float3(40., 0., 0.), far_attenuation(),
                                              MA_SAMPLE_RATE, 1, samples)
        sleep(200u)
        let by_volume = steady_peak_after(box) $ { set_volume(sid, 1.0, 0.0) }
        t |> success(by_volume < 1.5, "set_volume spiked the attenuated channel: {by_volume}x")
        let by_flag = steady_peak_after(box) $ { set_ignore_global_volume(sid, true) }
        t |> success(by_flag < 1.5, "ignore_global_volume spiked the channel: {by_flag}x")
        stop(sid, 0.0)
    }
}
