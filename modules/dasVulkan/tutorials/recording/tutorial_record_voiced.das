options gen2
options _comment_hygiene = true
options indenting = 4
options rtti                                 // dasOPENAI's SpeechRequest has @optional fields

// Voiced extension to tutorial_record.das (manual, local-only). Three calls on top of capture_apng +
// convert_to_mp4: say() records caption + voice anchors, prepare_voiceover hits Kokoro TTS for each,
// convert_to_mp4_voiced muxes the voice wavs under the music bed and paints captions via ffmpeg
// drawtext.
//
// Split out of tutorial_record.das so that file's compile-check in CI stays clean even when dasHV
// (the dasOPENAI HTTP backend) is not loaded -- the per-folder gate at tutorials/.das_test now also
// checks has_module("dashv"). Only loaded by drivers that actually want voice + captions; the silent
// path (record_triangle.das) keeps working with just tutorial_record.das.

module tutorial_record_voiced

require tutorial_record public               // re-export so a voiced driver requires one file
require daslib/fio
require math
require openai/openai_audio                  // speech / SpeechRequest / openai_client
require audio/audio_wav                      // read_wav (for measuring voiceover durations)
require strings
require daslib/strings_boost

struct public Line {
    frame   : int       // frame index the caption appears on (matches capture_apng's frame model)
    caption : string    // on-screen text (canonical -- dasVulkan / SPIR-V / dasSpirv stay as written)
    voice   : string    // spoken sentence (phonetic respellings -- see skills/recording.md)
    wav     : string    // path to the rendered voiceover wav (filled by prepare_voiceover)
    dur_s   : float     // wav duration in seconds (filled by prepare_voiceover)
}

var private g_lines : array<Line>

//! Register a caption + voice anchor at `frame`. The caption is what shows on screen (terse,
//! for the eye); the voice is what's spoken (natural sentence, ASCII-only, with phonetic
//! respellings for brand names -- see skills/recording.md). Defaults voice to caption.
def public say(frame : int; caption : string) {
    g_lines |> push(Line(frame = frame, caption = caption, voice = caption))
}

def public say(frame : int; caption, voice : string) {
    g_lines |> push(Line(frame = frame, caption = caption, voice = voice))
}

//! For each registered say() line, render a wav via Kokoro TTS (skipped if the wav exists on disk
//! already, so re-runs are cheap) and measure the duration. Wavs land in <apng_dir>/voiceover/.
def public prepare_voiceover(apng_path : string;
                              base_url : string = "http://127.0.0.1:8880/v1";
                              voice_id : string = "bf_emma";
                              model : string = "kokoro") : bool {
    let vo_dir = path_join(dir_name(apng_path), "voiceover")
    mkdir_rec(vo_dir)
    let client = openai_client(base_url, "")
    for (i in range(length(g_lines))) {
        let wav_path = path_join(vo_dir, "line_{i}.wav")
        if (!stat(wav_path).is_valid) {
            let res = speech(client, SpeechRequest(model = model, voice = voice_id,
                input = g_lines[i].voice, response_format = "wav"))
            if (!res.ok) {
                to_log(LOG_ERROR, "[record] TTS failed for line {i}: \"{g_lines[i].voice}\"\n")
                return false
            }
            fopen(wav_path, "wb") $(f) {
                if (f != null) {
                    fwrite(f, res.audio)
                }
            }
        }
        var samples : array<float>
        var rate = 0
        var channels = 0
        if (!read_wav(wav_path, samples, rate, channels) || rate <= 0 || channels <= 0) {
            to_log(LOG_ERROR, "[record] could not measure wav: {wav_path}\n")
            delete samples
            return false
        }
        let dur = float(length(samples) / channels) / float(rate)
        delete samples
        g_lines[i].wav = wav_path
        g_lines[i].dur_s = dur
        to_log(LOG_INFO, "[record] line {i}: {dur}s  \"{g_lines[i].voice}\"\n")
    }
    return true
}

// ffmpeg drawtext: even inside single quotes the filter-arg separator ':' (plus '%' for text
// expansion and '\\' itself) leak into the parser, and a literal apostrophe must close/reopen the
// quote. '=' is fine unescaped (proven by existing captions), so it is left alone.
def private esc_drawtext(s : string) : string {
    var r = replace(s, "\\", "\\\\")     // backslash first, so the escapes below aren't re-escaped
    r = replace(r, ":", "\\:")
    r = replace(r, "%", "\\%")
    r = replace(r, "'", "'\\''")
    return r
}

//! convert_to_mp4 plus: each prepared voiceover wav mixed under the music bed (delayed to its frame
//! index), each caption painted via ffmpeg drawtext over (line.frame/fps .. + line.dur_s + 0.4s).
//! `font_file` is an ffmpeg-escaped TTF path (the default is Windows-only). prepare_voiceover() first.
def public convert_to_mp4_voiced(apng_path, music_feature, daslang_exe, out_mp4 : string;
                                  dur_s : float; fps, bed_db, fade_ms : int;
                                  font_file : string = "C\\:/Windows/Fonts/arial.ttf") : bool {
    let music_wav = path_join(dir_name(apng_path), "{stem(apng_path)}_bed.wav")
    if (!stat(music_wav).is_valid) {
        let margs <- [daslang_exe, music_feature, "--", "--wav", music_wav, "--duration", "{dur_s}"]
        var mout : string
        if (run_and_capture(margs, mout) != 0) {
            to_log(LOG_ERROR, "[record] music bed render failed:\n{mout}\n")
            return false
        }
    }
    let n_vo = length(g_lines)
    let fade = float(fade_ms) / 1000.0f
    let fade_out_st = max(dur_s - fade, 0.0f)
    let bed = "[1:a]afade=t=in:st=0:d={fade},afade=t=out:st={fade_out_st}:d={fade},volume={bed_db}dB[bed]"
    let fg = build_string() $(var w) {
        // video drawtext chain
        w |> write("[0:v]")
        for (i in range(n_vo)) {
            let t_start = float(g_lines[i].frame) / float(fps)
            let t_end = t_start + g_lines[i].dur_s + 0.4f
            if (i != 0) {
                w |> write(",")
            }
            w |> write("drawtext=fontfile='{font_file}':text='{esc_drawtext(g_lines[i].caption)}'")
            w |> write(":fontsize=28:fontcolor=white:box=1:boxcolor=black@0.55:boxborderw=10")
            w |> write(":x=(w-text_w)/2:y=h-70")
            w |> write(":enable='between(t\\,{t_start}\\,{t_end})'")
        }
        w |> write("[vout]")
        // audio chain: bed + voices delayed to their frames, amix into one stream
        w |> write(";{bed}")
        for (i in range(n_vo)) {
            let t_ms = g_lines[i].frame * 1000 / fps
            w |> write(";[{2 + i}:a]adelay={t_ms}:all=1[vo{i}]")
        }
        w |> write(";[bed]")
        for (i in range(n_vo)) {
            w |> write("[vo{i}]")
        }
        w |> write("amix=inputs={1 + n_vo}:duration=longest:normalize=0[aout]")
    }
    var fargs <- ["ffmpeg", "-y", "-loglevel", "error", "-r", "{fps}", "-i", apng_path, "-i", music_wav]
    fargs |> reserve(length(fargs) + n_vo * 2)
    for (i in range(n_vo)) {
        fargs |> push("-i")
        fargs |> push(g_lines[i].wav)
    }
    fargs |> push_from([
        "-filter_complex", fg,
        "-map", "[vout]", "-map", "[aout]",
        // Tuned for the smooth slow-pan content these recordings produce: `-tune animation` biases x264
        // toward larger blocks + stronger lookahead, `-preset slower` buys motion-search exhaustiveness,
        // and a crf above libx264's default 23 is invisible on shader-pure output but cuts size for real.
        "-c:v", "libx264", "-preset", "slower", "-tune", "animation", "-crf", "28",
        "-pix_fmt", "yuv420p",
        "-movflags", "+faststart", "-shortest", out_mp4])
    fwrite("{out_mp4}.ffmpeg.txt", join(fargs, " "))
    var fout : string
    if (run_and_capture(fargs, fout) != 0) {
        to_log(LOG_ERROR, "[record] ffmpeg voiced mux failed:\n{fout}\n")
        return false
    }
    to_log(LOG_INFO, "[record] wrote {out_mp4}\n")
    return true
}
