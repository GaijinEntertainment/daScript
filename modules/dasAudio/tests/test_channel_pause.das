options gen2
require _audio_output_fixture
require audio/audio_boost
require daslib/fio
require debugapi
require dastest/testing_boost

[test]
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
