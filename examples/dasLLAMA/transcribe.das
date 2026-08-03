options gen2
options fast_math = true   // ggml-parity FP laxity (non-bit-exact, ~+10%); tests stay bit-exact
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama         // the facade: load_asr_model / caps / create_session / transcribe
require audio                     // miniaudio decode (any format) resampled to 16 kHz mono
require daslib/jobque_boost
require strings
require math

// Speech-to-text over the uniform ASR surface: any audio format miniaudio decodes goes in,
// segments come out. load_asr_model sniffs the file format — stock ggerganov/whisper.cpp
// ggml-*.bin files (timestamped segments; --lang auto detects the language), or a Qwen3-ASR
// GGUF decoder + its mmproj (one segment, the model reports the language; --context biases).
//
//   bin/daslang -jit examples/dasLLAMA/transcribe.das -- <ggml-model.bin> <audio-file>
//                    [--lang <code>] [--translate] [--no-timestamps]

// Decode any audio file miniaudio understands (WAV/MP3/FLAC/Vorbis) to 16 kHz mono f32 —
// the decoder converts rate/channels on the fly (same rail as audio_chat.das).
def load_audio_16k_mono(path : string) : array<float> {
    var decoder = new ma_decoder
    var config <- ma_decoder_config_init(ma_format.ma_format_f32, 1u, uint(AUDIO_SAMPLE_RATE))
    if (ma_decoder_init_file(path, unsafe(addr(config)), decoder) != ma_result.MA_SUCCESS) {
        panic("cannot open audio file '{path}'")
    }
    var out : array<float>
    let chunk = 65536
    var buf : array<float>
    buf |> resize(chunk)
    while (true) {
        let got = int(ma_decoder_read_pcm_frames(decoder, unsafe(addr(buf[0])), uint64(chunk)))
        if (got <= 0) {
            break
        }
        out |> reserve(length(out) + got)
        for (i in range(got)) {
            out |> push(buf[i])
        }
        if (got < chunk) {
            break
        }
    }
    ma_decoder_uninit(decoder)
    unsafe {
        delete decoder
    }
    return <- out
}

def pad2(v : int64) => v < 10l ? "0{v}" : "{v}"

// centiseconds -> hh:mm:ss.cc (whisper-cli's timestamp shape)
def fmt_ts(cs : int64) : string {
    return "{pad2(cs / 360000l)}:{pad2(cs / 6000l % 60l)}:{pad2(cs / 100l % 60l)}.{pad2(cs % 100l)}"
}

[export]
def main {
    let cmd <- get_command_line_arguments()
    var model_path = ""
    var mmproj_path = ""
    var audio_path = ""
    var lang = ""
    var context = ""
    var translate = false
    var no_timestamps = false
    var i = 1
    while (i < length(cmd)) {
        if (cmd[i] == "--lang" && i + 1 < length(cmd)) {
            lang = cmd[i + 1]
            i++
        } elif (cmd[i] == "--context" && i + 1 < length(cmd)) {
            context = cmd[i + 1]
            i++
        } elif (cmd[i] == "--translate") {
            translate = true
        } elif (cmd[i] == "--no-timestamps") {
            no_timestamps = true
        } elif (ends_with(cmd[i], ".bin")) {
            model_path = cmd[i]
        } elif (ends_with(cmd[i], ".gguf")) {
            if (empty(model_path)) {
                model_path = cmd[i]
            } else {
                mmproj_path = cmd[i]
            }
        } elif (empty(model_path)) {
            // skip interpreter/script args before the model path
        } elif (empty(audio_path)) {
            audio_path = cmd[i]
        }
        i++
    }
    if (empty(model_path) || empty(audio_path) || (ends_with(model_path, ".gguf") && empty(mmproj_path))) {
        print("usage: daslang -jit examples/dasLLAMA/transcribe.das -- <ggml-model.bin | decoder.gguf mmproj.gguf> <audio-file> [--lang <code>] [--context <text>] [--translate] [--no-timestamps]\n")
        return
    }
    if (empty(lang)) {
        lang = ends_with(model_path, ".gguf") ? "auto" : "en"
    }

    print("loading {model_path}\n")
    var w <- (empty(mmproj_path) ? load_asr_model(model_path) : load_asr_model(model_path, mmproj_path))
    let c <- caps(w)
    print("model: {length(c.languages)} language(s), translate {c.translate ? "yes" : "no"}, {c.timestamps} timestamps\n")
    let samples <- load_audio_16k_mono(audio_path)
    print("audio: {length(samples)} samples ({float(length(samples)) / float(int(AUDIO_SAMPLE_RATE))} s)\n")

    with_job_que() {
        setup_dasllama_jobque()
        var s <- create_session(w, lang)
        s.translate = translate
        s.no_timestamps = no_timestamps
        s.context = context
        let t0 = ref_time_ticks()
        var n_seg = 0
        transcribe(w, s, samples) $(seg) {
            n_seg++
            if (no_timestamps) {
                print("{seg.text}\n")
            } else {
                print("[{fmt_ts(seg.t0)} --> {fmt_ts(seg.t1)}]  {seg.text}\n")
            }
        }
        let dt = float(get_time_usec(t0)) / 1e6
        if (!empty(s.detected_lang)) {
            print("language: {s.detected_lang}\n")
        }
        print("\n{n_seg} segment(s) in {dt} s ({float(length(samples)) / float(int(AUDIO_SAMPLE_RATE)) / dt}x realtime)\n")
    }
}
