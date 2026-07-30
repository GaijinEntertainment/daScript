options gen2
options _comment_hygiene = true
options indenting = 4
options rtti

// convert_recording — turn a recorded .apng + its voiceover sidecar into a soundtracked .mp4.
//
// Reads voiceover/<stem>.sidecar.json (written by with_recording_app at record time), renders
// the daStrudel music bed to exactly the clip length, then builds + runs an ffmpeg command that
// muxes: APNG -> h264 video, a faded low-volume music bed, and each voiceover wav delayed to the
// frame where its caption appeared. The exact ffmpeg command is also saved to <out>.ffmpeg.txt.

require imgui/imgui_playwright              // VoSidecar / VoSidecarHit / dasimgui_module_root
require audio/audio_wav                     // read_wav (music sanity check)
require daslib/fio
require daslib/json
require daslib/json_boost
require daslib/clargs
require strings
require daslib/strings_boost
require math

[CommandLineArgs]
struct Config {
    @clarg_doc = "Path to the recorded .apng"
    apng : string

    @clarg_doc = "daStrudel music feature to render as the bed (default: sf2_integration_full_ambient)"
    music : string

    @clarg_doc = "Output .mp4 (default: the apng path with a .mp4 extension)"
    out : string

    @clarg_doc = "Music bed level in dB (negative = quieter under the voice)"
    bed_db : int = -13

    @clarg_doc = "Fade in/out length, milliseconds"
    fade_ms : int = 100

    @clarg_doc = "libx264 quality (lower = better/larger). Default 23 - keep it here for excellent quality; shrink full-motion clips with --preset, not by raising this"
    crf : int = 23

    @clarg_doc = "libx264 preset (slower = smaller at the SAME quality). Default medium; use veryslow for full-motion clips (e.g. realtime scroll) to cut size without touching crf"
    preset : string = "medium"

    @clarg_doc = "daScript root (find_sf2 + default daslang exe). Default: this exe's das-root"
    das_root : string

    @clarg_doc = "daslang executable (default: <das-root>/bin/Release/daslang.exe)"
    daslang : string

    @clarg_short = "?"
    @clarg_doc = "Show help and exit"
    help : bool
}

def shell_quote(a : string) : string {
    //! Quote an argv element for the saved .ffmpeg.txt so it can be pasted into a shell.
    let needs = find(a, " ") >= 0 || find(a, ";") >= 0 || find(a, "[") >= 0 || find(a, ",") >= 0
    return needs ? "\"{a}\"" : a
}

def join_command(args : array<string>) : string {
    return build_string() $(var w) {
        for (i in range(length(args))) {
            if (i > 0) {
                w |> write(" ")
            }
            w |> write(shell_quote(args[i]))
        }
    }
}

[export]
def main : int {
    var args_r <- parse_args(type<Config>)
    if (args_r |> is_err) {
        to_log(LOG_ERROR, "[convert] {args_r |> unwrap_err}\n")
        print_help(get_command_info(type<Config>), "convert_recording")
        return 1
    }
    let cfg <- args_r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "convert_recording")
        return 0
    }
    if (empty(cfg.apng)) {
        to_log(LOG_ERROR, "[convert] --apng is required\n")
        return 1
    }

    let das_root = empty(cfg.das_root) ? get_das_root() : cfg.das_root
    let music = empty(cfg.music) ? path_join(das_root, "examples/daStrudel/features/sf2_integration_full_ambient.das") : cfg.music
    let daslang_exe = empty(cfg.daslang) ? path_join(das_root, "bin/Release/daslang.exe") : cfg.daslang
    let out_mp4 = empty(cfg.out) ? replace_extension(cfg.apng, ".mp4") : cfg.out

    // --- read the voiceover sidecar (clip length + per-line offsets) ---
    let vo_dir = path_join(dir_name(cfg.apng), "voiceover")
    let sidecar_path = path_join(vo_dir, "{stem(cfg.apng)}.sidecar.json")
    let sc_txt = fread(sidecar_path)
    if (empty(sc_txt)) {
        to_log(LOG_ERROR, "[convert] no sidecar {sidecar_path}\n  (record the tutorial after prepare_recording so with_recording_app writes it)\n")
        return 1
    }
    var jerr : string
    let sc_jv = read_json(sc_txt, jerr)
    if (sc_jv == null) {
        to_log(LOG_ERROR, "[convert] bad sidecar {sidecar_path}: {jerr}\n")
        return 1
    }
    let sc = from_JV(sc_jv, type<VoSidecar>)
    let dur = sc.duration_s
    if (dur <= 0.0f) {
        to_log(LOG_ERROR, "[convert] sidecar duration_s is {dur}; nothing to render\n")
        return 1
    }
    to_log(LOG_INFO, "[convert] {cfg.apng}: {dur}s, {length(sc.voiceovers)} voiceover(s)\n")

    // --- render the music bed to exactly the clip length ---
    let music_wav = path_join(dir_name(out_mp4), "{stem(cfg.apng)}_music.wav")
    let margs <- [daslang_exe, music, "--", "--wav", music_wav, "--duration", "{dur}"]
    var mout : string
    let mrc = run_and_capture(margs, mout)
    if (mrc != 0) {
        to_log(LOG_ERROR, "[convert] music render failed ({mrc}):\n{mout}\n")
        return 1
    }
    var msamples : array<float>
    var mrate = 0
    var mchannels = 0
    if (!read_wav(music_wav, msamples, mrate, mchannels) || empty(msamples)) {
        to_log(LOG_ERROR, "[convert] music render produced no audio: {music_wav}\n")
        return 1
    }
    to_log(LOG_INFO, "[convert] music: {music_wav} ({length(msamples) / max(mchannels, 1)} frames @ {mrate}Hz)\n")

    // "skip-missed" scheduling drops stalled time, so the APNG's per-frame delays sum SHORTER than
    // wall-clock (e.g. 53.9s vs 74.9s) — captions would race ahead of their wall-clock voices. Force
    // the video onto a constant grid (`-r fps_eff`) and anchor each voice to its frame index on it.
    let fps_eff = sc.frames > 0 ? float(sc.frames) / dur : float(sc.fps)

    // --- build the ffmpeg filtergraph: faded low music bed + each voiceover delayed to its frame ---
    let n_vo = length(sc.voiceovers)
    let fade = float(cfg.fade_ms) / 1000.0f
    let fade_out_st = max(dur - fade, 0.0f)
    let bed = "[1:a]afade=t=in:st=0:d={fade},afade=t=out:st={fade_out_st}:d={fade},volume={cfg.bed_db}dB"
    let fg = empty(sc.voiceovers) ? "{bed}[aout]" : build_string() $(var w) {
        w |> write("{bed}[bed]")
        for (i in range(n_vo)) {
            let t_ms = int(float(sc.voiceovers[i].frame) / fps_eff * 1000.0f)
            w |> write(";[{2 + i}:a]adelay={t_ms}:all=1[vo{i}]")
        }
        w |> write(";[bed]")
        for (i in range(n_vo)) {
            w |> write("[vo{i}]")
        }
        w |> write("amix=inputs={1 + n_vo}:duration=longest:normalize=0[aout]")
    }

    // inputs: 0 = video (forced CFR via -r), 1 = music bed, 2+ = voiceover wavs ("-i" <path>, order)
    let vo_inputs <- [for (k in range(n_vo * 2)); k % 2 == 0 ? "-i" : path_join(vo_dir, sc.voiceovers[k / 2].wav)]
    var fargs <- ["ffmpeg", "-y", "-r", "{fps_eff}", "-i", cfg.apng, "-i", music_wav]
    fargs |> push_from(vo_inputs, ["-filter_complex", fg, "-map", "0:v", "-map", "[aout]",
        "-c:v", "libx264", "-preset", cfg.preset, "-crf", "{cfg.crf}", "-pix_fmt", "yuv420p",
        "-movflags", "+faststart", "-shortest", out_mp4])

    // --- save the command, then run it ---
    let cmd_text = join_command(fargs)
    let cmd_path = "{out_mp4}.ffmpeg.txt"
    fwrite(cmd_path, "{cmd_text}\n")
    to_log(LOG_INFO, "[convert] ffmpeg command saved to {cmd_path}\n")

    var fout : string
    let frc = run_and_capture(fargs, fout)
    if (frc != 0) {
        to_log(LOG_ERROR, "[convert] ffmpeg failed ({frc}):\n{fout}\n")
        return 1
    }
    to_log(LOG_INFO, "[convert] wrote {out_mp4}\n")
    return 0
}
