options gen2
module _audio_output_fixture shared public
require audio/audio_boost
require daslib/rtti
require debugapi
require math
require daslib/jobque_boost
require daslib/fio
require dastest/testing_boost

struct OutputProbe { peak, first_nonzero, last_nonzero : float; minimum : float = 1.0
    callbacks, silent_callbacks : int }
var private output_box : SeqBox?
var private output_probe = OutputProbe()

[pinvoke, export]
def capture_setup(var box : SeqBox?) {
    output_box = box; output_box |> add_ref
}
[pinvoke, export]
def capture_reset {
    output_probe = OutputProbe()
    if (output_box != null) { output_box |> publish(output_probe) }
}
[export]
def capture_mixer(var data : array<float>#; channels, rate : int; dt : float) {
    unsafe { invoke_in_context(this_context(), "mixer", addr(data), channels, rate, dt) }
    if (output_box != null) {
        var peak = 0.0
        for (sample in data) {
            peak = max(peak, abs(sample))
            if (sample != 0.0) {
                if (output_probe.first_nonzero == 0.0) { output_probe.first_nonzero = sample }
                output_probe.last_nonzero = sample
            }
            output_probe.peak = max(output_probe.peak, abs(sample))
            output_probe.minimum = min(output_probe.minimum, abs(sample))
        }
        output_probe.silent_callbacks = peak == 0.0 ? output_probe.silent_callbacks + 1 : 0
        output_probe.callbacks++; output_box |> publish(output_probe)
    }
}
[finalize]
def capture_finalize { if (output_box != null) { output_box |> seq_box_release } }

def with_output_probe(body : block<(box : SeqBox?) : void>) {
    sound_set_null_device(true)
    verify(sound_initalize(@@capture_mixer, MA_SAMPLE_RATE, MA_CHANNELS, this_context()))
    var stream = unsafe(stream_create()); var counter = atomic64_create()
    unsafe { set_audio_thread_command_stream(stream); set_sound_sid(counter); invoke_in_context(
        mixer_context(), "setup_command_processor", stream) }
    var box = seq_box_create()
    {
        unsafe(invoke_in_context(mixer_context(), "capture_setup", box))
        invoke(body, box)
    } finally {
        audio_system_finalize(stream, counter); box |> seq_box_release
        sound_set_null_device(false)
    }
}
def observe_output(box : SeqBox?; silent : bool = false) : OutputProbe {
    var output = OutputProbe()
    for (_ in range(100)) {
        box |> read() $(sample : OutputProbe) { output = sample }
        if (output.callbacks >= 8 && (!silent || output.silent_callbacks >= 2)) { break }
        sleep(10u)
    }
    if (output.callbacks < 8 || (silent && output.silent_callbacks < 2)) {
        panic("audio callback did not reach the required observation state")
    }
    return output
}
def constant_pcm(value : float) : array<float> {
    var samples : array<float>; resize(samples, MA_SAMPLE_RATE)
    for (sample in samples) { sample = value }
    return <- samples
}
