options gen2
options persistent_heap
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost
require audio/audio_boost
require audio/audio_record
require strings
require daslib/fio
require daslib/defer
require strudel/strudel
require strudel/strudel_player
require jobque
require audio
require daslib/jobque_boost
require math

// audio_system_create reads the user args through audio_user_args (`--null-audio`,
// `--volume V`). The parse is pure and tested directly; the null-backend EFFECT is
// observable through capture enumeration (miniaudio's null context reports exactly
// its one fake device), so that arm is driven for real under with_argv.

def argv_with(user_args : array<string>) : array<string> {
    var full <- ["daslang", "test_audio_args.das", "--"]
    full |> push_from(user_args)
    return <- full
}

def capture_device_names() : array<string> {
    let devs <- sound_record_list_devices()
    return <- [for (d in devs); d.name]
}

[test(description = "the audio command-line flags --null-audio and --volume parse into defaults, a flag and a clamped volume, ignoring garbage and combining")]
def test_audio_user_args_parse(t : T?) {
    t |> run("no flags = defaults") @(t : T?) {
        let k = audio_user_args([])
        t |> equal(k.null_device, false)
        t |> success(k.volume < 0.0, "volume not given")
    }
    t |> run("--null-audio sets the flag; a near-miss spelling does not") @(t : T?) {
        t |> equal(audio_user_args(["--null-audio"]).null_device, true)
        t |> equal(audio_user_args(["--null-audio-not-quite"]).null_device, false)
    }
    t |> run("--volume V parses and clamps to 0..1") @(t : T?) {
        t |> equal(audio_user_args(["--volume", "0.25"]).volume, 0.25)
        t |> equal(audio_user_args(["--volume", "7"]).volume, 1.0)
        t |> equal(audio_user_args(["--volume", "-3"]).volume, 0.0)
    }
    t |> run("--volume garbage or missing value = not given, never 0") @(t : T?) {
        t |> success(audio_user_args(["--volume", "loud"]).volume < 0.0, "garbage rejected")
        t |> success(audio_user_args(["--volume"]).volume < 0.0, "dangling flag ignored")
    }
    t |> run("flags compose in any order") @(t : T?) {
        let k = audio_user_args(["--volume", "0.5", "--null-audio"])
        t |> equal(k.null_device, true)
        t |> equal(k.volume, 0.5)
    }
}

[test(description = "--null-audio puts audio_system_create on the null backend")]
def test_null_audio_flag_effect(t : T?) {
    t |> run("--null-audio puts audio_system_create on the null backend") @(t : T?) {
        with_argv(argv_with(["--null-audio"])) {
            with_audio_system() {
                let names <- capture_device_names()
                t |> equal(length(names), 1, "null backend exposes exactly one capture device")
                t |> success(length(names) == 1 && find(names[0], "NULL") >= 0, "it is miniaudio's null device: {names}")
            }
        }
        // sticky process-wide flag: hand the next test a clean slate
        sound_set_null_device(false)
    }
}

[test(description = "a child program that never installs audio callbacks finalizes cleanly and exits with code zero", timeout = 1200)]
def test_audio_finalize_without_callbacks(t : T?) {
    let tmp = test_temp_dir()
    let child = path_join(tmp, "audio_finalize.das")
    defer(){remove(child)}
    let source = replace(replace(fread(path_join(get_das_root(), "modules/dasAudio/audio/audio_boost.das")), "module audio_boost shared private", ""), "audio_boost::", "")
    fwrite(child, "{source}\n[export]\ndef main()\{\n    assert(g_mixer_total_time == 0.lf && g_mixer_total_samples == 0ul)\n    if (get_command_line_arguments()[length(get_command_line_arguments()) - 1] == \"sampled\")\{g_mixer_total_samples = 48000ul\}\n    this_context().category.audio = true\n    print(\"finalize armed\\n\")\n\}\n")
    let args <- get_command_line_arguments()
    let exe = args[0]
    let env = get_platform_name() == "windows" ? "set \"DAS_LOG_LEVEL=trace\"&& " : "DAS_LOG_LEVEL=trace "
    var output : string
    let zero_rc = unsafe(popen_timeout("{env}\"{exe}\" -dasroot \"{get_das_root()}\" \"{child}\" 2>&1", 120.0) $(f) {
        if (f != null){output = fread(f)}
    })
    t |> equal(zero_rc, 0, output)
    t |> success(find(output, "finalize armed") >= 0, output)
    t |> success(find(output, "mixer ") < 0, "no utilization exists before the first callback: {output}")
    let sampled_rc = unsafe(popen_timeout("{env}\"{exe}\" -dasroot \"{get_das_root()}\" \"{child}\" -- sampled 2>&1", 120.0) $(f) {
        if (f != null){output = fread(f)}
    })
    t |> equal(sampled_rc, 0, output)
    t |> success(find(output, "mixer 0.0% utilization") >= 0, output)
}

// Exercises the AUDIO DEVICE teardown — with_audio_system + strudel main-thread
// playback (strudel_create_channel + strudel_tick) + strudel_shutdown — on
// miniaudio's NULL backend, so it runs headless / in CI (no real hardware).
//
// This whole directory is INTERPRETER-ONLY (gated off under --use-aot in
// tests/.das_test): driving the real device + its callback thread under AOT's
// tighter timing surfaces the pre-existing main-thread drain in
// audio_system_finalize (audio_boost.das:1284) racing the audio thread on the
// non-thread-safe mixer context (verySafeContext=false) -> heap corruption. No
// device test is AOT'd anywhere. The single-thread drain path itself is verified
// in the browser (site/tests/playground/audio.spec.js).
//
// What this guards: that the create/tick/shutdown cycle leaves NO net
// JobStatus/Channel/LockBox/Stream behind. On a threaded build (here, native +
// null backend) strudel_shutdown takes the original join path — the audio thread
// processes the deferred releases — so this is the regression check that the
// teardown ref-counting still balances.

[test(description = "creating, ticking and shutting down a strudel device leaves no JobQue leak")]
def test_device_teardown_no_leak(t : T?) {
    t |> run("strudel device create/tick/shutdown leaves no JobQue leak") @(t : T?) {
        sound_set_null_device(true)
        let before = count_jobque_leaks()
        with_audio_system() {
            strudel_create_channel()
            strudel_set_cps(4.0lf)
            let pat <- note("c4 e4 g4 c5", "sine") |> attack(0.01) |> decay(0.05) |> sustain(0.4) |> release(0.1)
            strudel_add_track(pat)
            for (_i in range(20)) {
                strudel_tick()
            }
            strudel_shutdown()
        }
        let after = count_jobque_leaks()
        t |> equal(after, before)
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}

// Run the full cycle twice — re-creating the audio system after a clean teardown must
// work (the wasm playground re-runs the same program; this is the native mirror).
[test(description = "a second device create, tick and shutdown cycle also leaves no JobQue leak")]
def test_device_teardown_rerun(t : T?) {
    t |> run("strudel device cycle is repeatable, still no JobQue leak") @(t : T?) {
        sound_set_null_device(true)
        let before = count_jobque_leaks()
        for (_cycle in range(2)) {
            with_audio_system() {
                strudel_create_channel()
                strudel_set_cps(4.0lf)
                let pat <- note("c4 e4 g4", "sine") |> release(0.1)
                strudel_add_track(pat)
                for (_i in range(10)) {
                    strudel_tick()
                }
                strudel_shutdown()
            }
        }
        let after = count_jobque_leaks()
        t |> equal(after, before)
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}

// Headless microphone-capture smoke test on miniaudio's NULL backend: the null device
// feeds timer-paced silent frames, so start -> drain -> stop runs in CI with no real
// hardware. Lives in this interpreter-only directory (gated off under --use-aot in
// tests/.das_test) alongside the playback device tests — no capture device is AOT-driven,
// matching the established "no device test is AOT'd" rule. Unlike playback, capture never
// touches the mixer context (the RT callback only writes a lock-free C++ ring), so there is
// no context race here; it is kept interp-only for consistency, not because of a known bug.

[test(description = "a null-backend capture starts, drains silent frames and stops cleanly")]
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

[test(description = "the null backend enumerates its capture devices")]
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

[test(description = "record_to_wav on the null backend writes a readable WAV of exactly the requested length")]
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

// The playback twin of sound_record_overflow_frames. Only the threaded WebAssembly build mixes on
// a worker thread and can underrun its ring; every other target mixes on the device callback and
// answers 0. This cell is what the native arm and the binding itself answer to - a counter never
// registered, or one whose native arm returned something other than 0, fails here.
[test(description = "the playback underrun counter reads zero on a target that mixes on the device callback")]
def test_playback_underrun_counter(t : T?) {
    t |> run("the playback underrun counter reads zero on a target that mixes on the device callback") @(t : T?) {
        t |> equal(sound_playback_underrun_frames(), 0l)
    }
}

// Playback status over a SeqBox, on miniaudio's NULL backend so it runs headless. This directory
// is INTERPRETER-ONLY (gated off under --use-aot in tests/.das_test) — see test_device_teardown.das.
//
// What this guards: the box reads sensibly the instant set_status_update returns, it tracks the
// sound to completion, and releasing it needs no ordering with the audio thread — the property
// that replaced the old join().

def private short_tone(seconds : float) : array<float> {
    let n = int(float(MA_SAMPLE_RATE) * seconds)
    return <- [for (x in range(n)); sin(2.0 * PI * 440.0 * float(x) / float(MA_SAMPLE_RATE))]
}

[test(description = "the sound status box reads the new sound's state the instant set_status_update returns")]
def test_status_seeded_on_the_calling_thread(t : T?) {
    t |> run("the box reads the new sound's state the instant set_status_update returns") @(t : T?) {
        sound_set_null_device(true)
        with_audio_system() {
            var tone <- short_tone(10.0)
            let sid = play_sound_from_pcm(MA_SAMPLE_RATE, 1, tone)
            var box = seq_box_create()
            set_status_update(sid, box)
            var state = AudioChannelState.stopped
            let got = box |> read() $(s : AudioChannelStatus) {
                state = s.state
            }
            t |> equal(got, true, "the box must not read empty right after registration")
            t |> success(state == AudioChannelState.starting || state == AudioChannelState.playing,
                "the seeded `starting`, or `playing` once the audio thread has picked the sound up: {state}")
            stop(sid)
            unset_status_update(sid)
            box |> seq_box_release
        }
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}

[test(description = "a one-shot tone advances to the stopped state")]
def test_status_reaches_stopped(t : T?) {
    t |> run("a one-shot tone advances to stopped") @(t : T?) {
        sound_set_null_device(true)
        with_audio_system() {
            var tone <- short_tone(0.2)
            let sid = play_sound_from_pcm(MA_SAMPLE_RATE, 1, tone)
            var box = seq_box_create()
            set_status_update(sid, box)
            var reached_stopped = false
            for (_i in range(200)) {
                box |> read() $(s : AudioChannelStatus) {
                    reached_stopped = s.state == AudioChannelState.stopped
                }
                break if (reached_stopped)
                sleep(20u)
            }
            t |> success(reached_stopped, "0.2s tone never reported stopped within a 4s window")
            unset_status_update(sid)
            box |> seq_box_release
        }
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}

[test(description = "releasing the status box while the sound still plays leaks nothing")]
def test_release_needs_no_ordering(t : T?) {
    t |> run("releasing the box while the sound still plays leaks nothing") @(t : T?) {
        sound_set_null_device(true)
        let before = count_jobque_leaks()
        with_audio_system() {
            for (_i in range(8)) {
                var inscope tone <- short_tone(30.0)
                let sid = play_sound_from_pcm(MA_SAMPLE_RATE, 1, tone)
                var box = seq_box_create()
                set_status_update(sid, box)
                // Drop our reference immediately, with the sound still playing and the audio
                // thread still holding its own. Under the old LockBox this needed unset + join
                // first, or it trapped on "being deleted while being used".
                box |> seq_box_release
            }
            sleep(60u)
        }
        t |> equal(count_jobque_leaks(), before)
        sound_set_null_device(false)   // sticky process-wide flag: leave a clean slate
    }
}
