options gen2
options indenting = 4
options persistent_heap
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost
require audio/audio_boost
require audio/audio_record
require strings
require daslib/fio
require daslib/defer

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

[test]
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
