options gen2
options indenting = 4
options persistent_heap
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost
require audio/audio_record
require audio
require jobque
require daslib/fio

// Headless microphone-capture smoke test on miniaudio's NULL backend: the null device
// feeds timer-paced silent frames, so start -> drain -> stop runs in CI with no real
// hardware. Lives in this interpreter-only directory (gated off under --use-aot in
// tests/.das_test) alongside the playback device tests — no capture device is AOT-driven,
// matching the established "no device test is AOT'd" rule. Unlike playback, capture never
// touches the mixer context (the RT callback only writes a lock-free C++ ring), so there is
// no context race here; it is kept interp-only for consistency, not because of a known bug.

[test]
def test_record_capture_smoke(t : T?) {
    t |> run("null-backend capture: start, drain silent frames, clean stop") @(t : T?) {
        sound_set_null_device(true)
        let before = count_jobque_leaks()
        t |> success(sound_record_start(16000, 1, 16000, -1), "capture device should start on the null backend")
        t |> success(sound_is_recording(), "should be recording after start")
        var scratch : array<float>
        scratch |> resize(16000)
        var total = 0
        for (_iter in range(30)) {   // ~600ms wall; the null device paces silent frames by run time
            sleep(20u)
            let n = sound_record_read(scratch)
            t |> success(n >= 0, "record_read must never return a negative frame count")
            total += n
        }
        t |> success(total > 0, "null backend should have produced capture frames")
        t |> equal(sound_record_overflow_frames(), 0l)   // 1s ring drained every 20ms -> no overflow
        sound_record_stop()
        t |> success(!sound_is_recording(), "should not be recording after stop")
        t |> equal(sound_record_available(), 0)          // device torn down -> nothing buffered
        let after = count_jobque_leaks()
        t |> equal(after, before)                        // capture allocates no jobque objects
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}

[test]
def test_record_device_enumeration(t : T?) {
    t |> run("null-backend capture device enumeration") @(t : T?) {
        sound_set_null_device(true)
        let devs <- sound_record_list_devices()
        t |> equal(length(devs), sound_record_device_count())
        for (d in devs) {
            t |> success(d.index >= 0, "device index is non-negative")
        }
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}

[test]
def test_record_to_wav_roundtrip(t : T?) {
    t |> run("null-backend record_to_wav writes a readable WAV of exactly the requested length") @(t : T?) {
        sound_set_null_device(true)
        let tmp = create_temp_file_result("das_record_to_wav_test", ".wav")   // unique writable path, no cwd assumptions
        if (!(tmp is value)) {
            t |> failure("could not create a temp file")
            return
        }
        let path = tmp as value
        let rate = 16000
        let seconds = 0.2
        t |> success(record_to_wav(path, seconds, rate, 1), "record_to_wav should succeed on the null backend")
        var samples : array<float>
        var got_rate = 0
        var got_channels = 0
        t |> success(read_wav(path, samples, got_rate, got_channels), "the written WAV should be readable back")
        t |> equal(got_rate, rate)
        t |> equal(got_channels, 1)
        t |> equal(length(samples), int(float(rate) * seconds))   // exact-length capping: 0.2s * 16000 = 3200 frames
        remove(path)
        // a write failure must propagate: an unwritable output path returns false (not a silent success)
        t |> success(!record_to_wav("/nonexistent_dir_xyzzy_das/rec.wav", 0.05, rate, 1),
            "record_to_wav returns false when the WAV cannot be written")
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}
