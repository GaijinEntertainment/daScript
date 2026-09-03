options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

// txt2wav — speak a text into a WAV file with a dasLLAMA TTS model. A sibling of wav2txt and of
// dasllama-server: it requires dasLLAMA and so inherits the shared [tune_scope]. The front end's
// packs (tts_g2p.bin, tts_postag.bin) are read from the model's directory.
//
//   bin/daslang -jit utils/dasllama-server/txt2wav.das -- --tts <kitten-nano.gguf> --text "Hello." --out hello.wav [--voice expr-voice-2-f] [--speed 1.0]

require dasllama/dasllama            // load_tts_model / synthesize / write_wav_pcm16 / the shared [tune_scope]
require llvm/daslib/llvm_tune        // [tune_policy] + log_tune_status
require daslib/jobque_boost
require daslib/clargs
require daslib/defer          // defer() { reset_tts_q8() } — the weight-lane pin never outlives the run
require daslib/fio

[CommandLineArgs]
struct Txt2WavArgs {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Path to the TTS model GGUF (kitten-nano, kitten-mini, kokoro-82m)"
    tts : string

    @clarg_short = "t"
    @clarg_doc = "The text to speak (or --file)"
    text : string

    @clarg_short = "f"
    @clarg_doc = "Read the text from this file instead of --text"
    file : string

    @clarg_required
    @clarg_short = "o"
    @clarg_doc = "The WAV file to write (16-bit PCM, the model's rate)"
    out : string

    @clarg_short = "v"
    @clarg_doc = "Voice name or alias (default: the model's first voice)"
    voice : string

    @clarg_short = "s"
    @clarg_doc = "Speech speed multiplier (default 1.0)"
    speed : float

    @clarg_doc = "Re-tune this box's dasLLAMA kernels, then relaunch"
    tune : bool

    @clarg_doc = "Pin the rows GEMMs to Q8_0 quants (the serving default; its own prepared image beside the GGUF)"
    q8 : bool

    @clarg_doc = "Pin the rows GEMMs to the file's f32 planes (the reference lane)"
    f32 : bool

    @clarg_doc = "Print the generator's per-op profile (PROF rows: bucket, ms, hits, percent)"
    prof : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

[export, tune_policy(missing = "auto")]
def main : int {
    if (!jit_enabled() && !is_standalone_exe()) {
        to_log(LOG_ERROR, "txt2wav: run under the JIT — bin/daslang -jit utils/dasllama-server/txt2wav.das -- ...\n")
        return 1
    }
    var inscope r <- parse_args(type<Txt2WavArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Txt2WavArgs>), "txt2wav")
        return 1
    }
    var inscope cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Txt2WavArgs>), "txt2wav")
        return 0
    }
    var text = cfg.text
    if (!empty(cfg.file)) {
        text = fread(cfg.file)
        if (empty(text)) {
            to_log(LOG_ERROR, "txt2wav: could not read {cfg.file}\n")
            return 1
        }
    }
    if (empty(text)) {
        to_log(LOG_ERROR, "txt2wav: nothing to say — pass --text or --file\n")
        return 1
    }
    log_tune_status("txt2wav")

    if (cfg.q8 && cfg.f32) {
        to_log(LOG_ERROR, "txt2wav: --q8 and --f32 pin the rows GEMMs to opposite lanes - pass one, not both\n")
        return 1
    }
    defer() {   // the pin must not outlive this run, on any path out
        reset_tts_q8()
    }
    if (cfg.q8 || cfg.f32) {
        set_tts_q8(cfg.q8)
    }
    let t_load = ref_time_ticks()
    var inscope m <- load_tts_model(cfg.tts)
    let load_s = double(get_time_usec(t_load)) / 1.0e6lf
    var inscope c <- caps(m)
    let voice = empty(cfg.voice) ? c.voices[0] : cfg.voice
    let speed = cfg.speed > 0.0 ? cfg.speed : 1.0

    var inscope audio = TtsAudio()
    with_job_que() {
        setup_dasllama_jobque()
        set_asr_prof(cfg.prof)
        delete audio
        audio <- synthesize(m, text, voice, speed)
        if (cfg.prof) {
            asr_prof_report()
        }
    }
    if (!write_wav_pcm16(cfg.out, audio.pcm, audio.sample_rate)) {
        to_log(LOG_ERROR, "txt2wav: could not write {cfg.out}\n")
        return 1
    }
    to_log(LOG_INFO, "txt2wav: wrote {cfg.out} - {voice}, load {load_s} s, {timings_line(audio.timings, audio.sample_rate)}\n")
    return 0
}
