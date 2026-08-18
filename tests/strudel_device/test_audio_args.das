options gen2
options indenting = 4
options persistent_heap
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost
require audio/audio_boost
require audio/audio_record
require strings

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

[test]
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

[test]
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
