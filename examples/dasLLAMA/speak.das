options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _jit_fast_math = true   // ggml-parity FP laxity (non-bit-exact, ~+10%); tests stay bit-exact

require dasllama/dasllama   // the facade: the TTS entry points, the WAV writer, the jobque setup
require daslib/clargs
require daslib/jobque_boost
require daslib/strings_boost
require strings

// Speak a line locally: load a TTS GGUF (KittenTTS or Kokoro, with tts_g2p.bin and
// tts_postag.bin beside it), synthesize, write a WAV, and report audio seconds against the
// seconds the synthesis took. The voice defaults to the last one the model offers.
//
//   bin/daslang -jit examples/dasLLAMA/speak.das -- --model <kitten-nano.gguf> --text "Hello." --out hello.wav

[CommandLineArgs]
struct SpeakArgs {
    @clarg_short = "m"
    @clarg_doc = "The TTS model (GGUF or a prepared .dlim)"
    model : string

    @clarg_short = "t"
    @clarg_doc = "The text to speak"
    text : string = "Once upon a time, there was a little girl named Lily. She loved to play outside in the sunshine."

    @clarg_short = "o"
    @clarg_doc = "The WAV file to write"
    out : string = "speech.wav"

    @clarg_short = "v"
    @clarg_doc = "Voice name or alias (default: the model's last voice)"
    voice : string

    @clarg_short = "s"
    @clarg_doc = "Speech speed, 0.25 to 4.0"
    speed : float = 1.0

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

[export]
def main : int {
    var inscope r <- parse_args(type<SpeakArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<SpeakArgs>), "speak")
        return 1
    }
    var inscope cfg <- r |> move_unwrap
    if (cfg.help || empty(cfg.model)) {
        print_help(get_command_info(type<SpeakArgs>), "speak")
        return cfg.help ? 0 : 1
    }
    var inscope m <- load_tts_model(cfg.model)
    allow_cpu_prefill()
    var inscope c <- caps(m)
    let voice = empty(cfg.voice) ? c.voices[length(c.voices) - 1] : cfg.voice
    print("{length(c.voices)} voices: {join(c.voices, ", ")}; speaking as {voice}\n")
    var rc = 0
    with_job_que() {
        setup_dasllama_jobque()
        var inscope a <- synthesize(m, cfg.text, voice, cfg.speed)
        let audio_s = float(length(a.pcm)) / float(a.sample_rate)
        let wall_s = float(a.timings.total_us) / 1000000.0
        if (!write_wav_pcm16(cfg.out, a.pcm, a.sample_rate)) {
            print("cannot write {cfg.out}\n")
            rc = 1
        } else {
            print("{cfg.out}: {audio_s} s of audio at {a.sample_rate} Hz, synthesized in {wall_s} s - {audio_s / wall_s}x real time\n")
        }
    }
    return rc
}
