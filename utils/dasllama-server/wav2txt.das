options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

// wav2txt — transcribe an audio file to text with a dasLLAMA ASR model. A sibling of
// dasllama-server: it requires dasLLAMA and so inherits the shared [tune_scope], reading the
// same <das_root>/dasllama.tune.json. Its policy is "auto" like the server — the first use on
// an untuned box tunes and relaunches, then transcribes. --tune forces a re-tune;
// DAS_TUNE_POLICY=error skips per-start tuning while developing.
//
//   bin/daslang -jit utils/dasllama-server/wav2txt.das -- --asr <asr.gguf> --file <audio.wav> [--out t.txt]

require dasllama/dasllama            // load_asr_model / transcribe / the shared [tune_scope]
require dasllama/dasllama_audio_io   // load_audio_16k_mono: file -> 16 kHz mono f32
require llvm/daslib/llvm_tune        // [tune_policy] + log_tune_status
require daslib/jobque_boost
require daslib/clargs
require daslib/fio

[CommandLineArgs]
struct Wav2TxtArgs {
    @clarg_required
    @clarg_short = "a"
    @clarg_doc = "Path to the ASR model (GGUF or .bin)"
    asr : string

    @clarg_required
    @clarg_short = "f"
    @clarg_doc = "Path to the audio file to transcribe"
    file : string

    @clarg_short = "o"
    @clarg_doc = "Write the transcript here instead of stdout"
    out : string

    @clarg_short = "l"
    @clarg_doc = "Language hint (default en)"
    lang : string

    @clarg_doc = "mmproj for a GGUF ASR model (paired with --asr)"
    mmproj : string

    @clarg_doc = "Re-tune this box's dasLLAMA kernels, then relaunch"
    tune : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

[export, tune_policy(missing = "auto")]
def main : int {
    // inference on the interpreter is uselessly slow — require the JIT (or a jit -exe build)
    if (!jit_enabled() && !is_standalone_exe()) {
        to_log(LOG_ERROR, "wav2txt: run under the JIT — bin/daslang -jit utils/dasllama-server/wav2txt.das -- ...\n")
        return 1
    }
    var r <- parse_args(type<Wav2TxtArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Wav2TxtArgs>), "wav2txt")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Wav2TxtArgs>), "wav2txt")
        return 0
    }

    log_tune_status("wav2txt")

    let t_decode = ref_time_ticks()
    let samples <- load_audio_16k_mono(cfg.file)
    if (empty(samples)) {
        to_log(LOG_ERROR, "wav2txt: could not decode audio: {cfg.file}\n")
        return 1
    }
    let decode_s = double(get_time_usec(t_decode)) / 1.0e6lf
    let audio_s = double(length(samples)) / 16000.0lf   // 16 kHz mono

    var m <- empty(cfg.mmproj) ? load_asr_model(cfg.asr) : load_asr_model(cfg.asr, cfg.mmproj)
    let lang = empty(cfg.lang) ? "en" : cfg.lang

    var text = ""
    let t_tr = ref_time_ticks()
    with_job_que() {
        setup_dasllama_jobque()
        var s = create_session(m, lang)
        text = transcribe(m, s, samples)
    }
    let transcribe_s = double(get_time_usec(t_tr)) / 1.0e6lf

    if (empty(cfg.out)) {
        print("{text}\n")
    } elif (fwrite(cfg.out, text)) {
        to_log(LOG_INFO, "wav2txt: wrote {cfg.out}\n")
    } else {
        to_log(LOG_ERROR, "wav2txt: could not write {cfg.out}\n")
        return 1
    }

    // rtf = transcription wall-clock / audio duration; < 1.0 is faster than real time
    let rtf = audio_s > 0.0lf ? transcribe_s / audio_s : 0.0lf
    to_log(LOG_INFO, "wav2txt: {audio_s} s audio | decode {decode_s} s | transcribe {transcribe_s} s | rtf {rtf}\n")
    return 0
}
