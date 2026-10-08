options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama_core
require daslib/export_c
require daslib/jobque_boost
require daslib/fio
require daslib/strings_boost
require jobque
require math

// dasLLAMA behind a C ABI - text completion, speech to text and speech synthesis. Both backends build
// this one file; README.md says how. A C caller reads each entry point as
// <library name>_<def name>: dasllama_lib_open, and so on.

var g_model = Model()
var g_session = Session()
var g_params = SamplingParams()
var g_reason : string = ""
var g_loaded = false
var g_prefilled = false
var g_owns_queue = false
var g_prefill_us = 0l
var g_prefill_tokens = 0l
var g_gen_us = 0l
var g_gen_tokens = 0l
var g_asr = AsrModel()
var g_asr_session = AsrSession()
var g_asr_loaded = false
var g_asr_us = 0l
var g_audio_seconds = 0.0lf
var g_tts = TtsModel()
var g_tts_loaded = false
var g_tts_voice : string = ""
var g_tts_rate = 1.0
var g_tts_us = 0l
var g_tts_seconds = 0.0lf
var g_tts_pcm : array<float>
var g_tts_pcm_rate = 0
var g_vad = VadModel()
var g_vad_loaded = false
var g_listening = false
var g_vad_iter = VadIter()
var g_take : array<float>       // the live stream at 16 kHz, from stream sample g_take_base on
var g_take_base = 0l
var g_utterance_start = -1l     // the stream sample the open utterance starts at, -1 when none is open
var g_rs_pos = 0.0lf            // the resampler's read position, relative to the next chunk
var g_rs_prev = 0.0             // the previous chunk's last sample, read at position -1

def private refuse(why : string) : bool {
    g_reason = why
    return false
}

//! opened once for both surfaces, and released only when neither model is open
def private queue_acquire() {
    if (!is_job_que_available()) {
        create_job_que()
        g_owns_queue = true
    }
    setup_dasllama_jobque()
    allow_cpu_prefill()
}

def private queue_release() {
    if (g_owns_queue && !g_loaded && !g_asr_loaded && !g_tts_loaded) {
        destroy_job_que()
        g_owns_queue = false
    }
}

[export_c]
def open(model_path : string) : bool {
    //! Load the text model at `model_path` and open one session over it.
    if (g_loaded) {
        return refuse("a model is already open - call close first")
    }
    if (!stat(model_path).is_valid) {
        return refuse("no such file: {model_path}")
    }
    queue_acquire()
    g_model <- load_model(model_path, QuantMode.q8)   // nolint:PERF030 - open refuses while loaded, close deletes it
    g_loaded = true
    if (g_model.config.n_layers <= 0l) {
        close()
        return refuse("{model_path} carries no layers - not a model this engine reads")
    }
    g_session <- create_session(g_model)   // nolint:PERF030 - fresh, or finalized by close
    g_prefilled = false
    g_reason = ""
    return true
}

[export_c]
def close() {
    //! Drop the session and the model; calling it on a closed library is not an error.
    return if (!g_loaded)
    delete g_session
    delete g_model
    g_loaded = false
    g_prefilled = false
    queue_release()
}

[export_c]
def reason() : string {
    //! Why the last call answered false or -1, or "" when none did. A daslang PANIC is a separate
    //! channel: -lib reports it through the generated last_error, -ctx ends the process.
    return g_reason
}

[export_c]
def arch() : string {
    //! The GGUF architecture the loader resolved, or "" while no model is open.
    return g_loaded ? g_model.arch : ""
}

[export_c]
def n_layers() : int => g_loaded ? int(g_model.config.n_layers) : 0

[export_c]
def n_vocab() : int => g_loaded ? int(g_model.config.vocab_size) : 0

[export_c]
def context_size() : int => g_loaded ? int(g_model.config.seq_len) : 0

[export_c]
def set_sampling(temp : float; top_k : int; seed : int) {
    //! Greedy while `temp <= 0`; `top_k <= 0` keeps the whole distribution.
    g_params.temp = temp
    g_params.top_k = int64(top_k)
    if (g_loaded) {
        set_seed(g_session, seed)
    }
}

[export_c]
def prefill(prompt : string) : int {
    //! Encode `prompt` and run it through the model, leaving the session prefilled. Returns the
    //! prompt's token count, or -1 with `reason` set.
    if (!g_loaded) {
        refuse("no model is open")
        return -1
    }
    let tokens <- encode(g_model, prompt, true, false)
    if (empty(tokens)) {
        refuse("the prompt encodes to no tokens")
        return -1
    }
    let started = ref_time_ticks()
    eval(g_model, g_session, tokens)
    g_prefill_us = int64(get_time_usec(started))
    g_prefill_tokens = long_length(tokens)
    g_gen_us = 0l
    g_gen_tokens = 0l
    g_prefilled = true
    g_reason = ""
    return length(tokens)
}

[export_c]
def next_piece() : string {
    //! The next token's text, advancing the session past it; "" until a prefill has run. The raw
    //! completion path carries no stop token, so the caller decides when it has read enough.
    if (!g_loaded || !g_prefilled) {
        return ""
    }
    let started = ref_time_ticks()
    let id = sample(g_session, g_params)
    var one : array<int64>
    one |> push(id)
    eval(g_model, g_session, one)
    g_gen_us += int64(get_time_usec(started))
    g_gen_tokens ++
    return piece(g_model, id)
}

// ===== speech to text =====

[export_c(comment = "Load the speech-to-text model at `model_path` and open one transcription session over it.")]
def asr_open(model_path : string) : bool {
    if (g_asr_loaded) {
        return refuse("an ASR model is already open - call asr_close first")
    }
    if (!stat(model_path).is_valid) {
        return refuse("no such file: {model_path}")
    }
    queue_acquire()
    g_asr <- load_asr_model(model_path)   // nolint:PERF030 - asr_open refuses while loaded, asr_close deletes it
    g_asr_loaded = true
    g_asr_session <- create_session(g_asr, "en")   // nolint:PERF030 - fresh, or finalized by asr_close
    g_reason = ""
    return true
}

[export_c(comment = "Drop the transcription session and its model.")]
def asr_close() {
    return if (!g_asr_loaded)
    delete g_asr_session
    delete g_asr
    if (g_vad_loaded) {
        delete g_vad
        g_vad_loaded = false
    }
    g_listening = false
    delete g_take
    g_asr_loaded = false
    queue_release()
}

[export_c(comment = "Seconds of audio transcribed per second of wall clock, 0 before a transcript has run.")]
def asr_speed() : double => g_asr_us > 0l ? g_audio_seconds * 1.0e6lf / double(g_asr_us) : 0.0lf

// ===== speech to text from a live stream =====

[export_c(comment = "Start a live stream on the open ASR model; `vad_path` is the Silero VAD that cuts it into\nutterances (modules/dasLLAMA/models/silero_vad.bin). Then pass the audio to asr_feed as it arrives.")]
def asr_listen(vad_path : string) : bool {
    if (!g_asr_loaded) {
        return refuse("no ASR model is open")
    }
    if (!g_vad_loaded) {
        if (!stat(vad_path).is_valid) {
            return refuse("no such file: {vad_path}")
        }
        g_vad <- load_vad_model(vad_path)   // nolint:PERF030 - loaded once, deleted by asr_close
        g_vad_loaded = true
    }
    var opts = default_vad_opts()
    opts.min_silence_ms = 500   // an utterance ends at a half-second pause, not at every breath in a sentence
    opts.speech_pad_ms = 200    // keep the quiet ends of words: ASR reads a cut-off syllable badly
    vad_iter_reset(g_vad_iter, opts)
    g_take |> clear()
    g_take_base = 0l
    g_utterance_start = -1l
    g_rs_pos = 0.0lf
    g_rs_prev = 0.0
    g_listening = true
    g_reason = ""
    return true
}

//! `count` samples at `rate` Hz as 16 kHz samples, linearly interpolated across chunks
def private resample_to_16k(pcm : float const?; count, rate : int) : array<float> {
    var out : array<float>
    let step = double(rate) / double(AUDIO_SAMPLE_RATE)
    out |> reserve(int(double(count) / step) + 2)
    var pos = g_rs_pos
    while (pos < double(count - 1)) {
        let i = int(floor(pos))
        let a = i < 0 ? g_rs_prev : unsafe(pcm[i])
        out |> push(lerp(a, unsafe(pcm[i + 1]), float(pos - double(i))))
        pos += step
    }
    g_rs_pos = pos - double(count)
    g_rs_prev = unsafe(pcm[count - 1])
    return <- out
}

//! transcribes the open utterance up to stream sample `end`, and drops the audio before `end`
def private close_utterance(end : int64) : string {
    let s = clamp(g_utterance_start - g_take_base, 0l, long_length(g_take))
    let e = clamp(end - g_take_base, s, long_length(g_take))
    var utterance : array<float>
    utterance |> resize(e - s)
    for (i in range64(e - s)) {
        utterance[i] = g_take[s + i]
    }
    g_take |> erase(0l, e)
    g_take_base += e
    g_utterance_start = -1l
    if (empty(utterance)) {
        return ""
    }
    g_audio_seconds = double(length(utterance)) / double(AUDIO_SAMPLE_RATE)
    let started = ref_time_ticks()
    let text = strip(transcribe(g_asr, g_asr_session, utterance))
    g_asr_us = int64(get_time_usec(started))
    return text
}

[export_c(comment = "Feed `count` mono samples at `rate` Hz, in chunks of any size - straight from a capture callback.\nReturns the text of every utterance the VAD closed in this call, \"\" while speech goes on; the\ntranscription runs inside the call, so feed it from a worker thread.")]
def asr_feed(pcm : float const?; count : int; rate : int) : string {
    if (!g_listening) {
        refuse("not listening - call asr_listen first")
        return ""
    }
    if (pcm == null || count <= 0 || rate <= 0) {
        return ""
    }
    let fresh <- resample_to_16k(pcm, count, rate)
    g_take |> push_from(fresh)
    var heard = ""
    vad_iter_feed(g_vad, g_vad_iter, fresh) $(ev : VadEvent) {
        if (ev.kind == VadEventKind.speech_start) {
            g_utterance_start = ev.sample
        } else {
            let text = close_utterance(ev.sample)
            heard = empty(heard) ? text : "{heard} {text}"
        }
    }
    // between utterances keep one second, enough for the padded start of the next one
    if (g_utterance_start < 0l && long_length(g_take) > 2l * AUDIO_SAMPLE_RATE) {
        let drop = long_length(g_take) - AUDIO_SAMPLE_RATE
        g_take |> erase(0l, drop)
        g_take_base += drop
    }
    g_reason = ""
    return heard
}

[export_c(comment = "End the live stream: returns the text of the utterance still open, or \"\".")]
def asr_listen_end() : string {
    let text = g_listening && g_utterance_start >= 0l ? close_utterance(g_take_base + long_length(g_take)) : ""
    g_listening = false
    return text
}

// ===== speech synthesis =====

[export_c(comment = "Load the text-to-speech model at `model_path` - a Pocket TTS, KittenTTS or Kokoro file -\nand take its last voice as the default.")]
def tts_open(model_path : string) : bool {
    if (g_tts_loaded) {
        return refuse("a TTS model is already open - call tts_close first")
    }
    if (!stat(model_path).is_valid) {
        return refuse("no such file: {model_path}")
    }
    queue_acquire()
    g_tts <- load_tts_model(model_path)   // nolint:PERF030 - tts_open refuses while loaded, tts_close deletes it
    g_tts_loaded = true
    let c <- caps(g_tts)
    if (empty(c.voices)) {
        tts_close()
        return refuse("{model_path} offers no voice")
    }
    g_tts_voice = c.voices[length(c.voices) - 1]
    g_reason = ""
    return true
}

[export_c(comment = "Drop the synthesis model.")]
def tts_close() {
    return if (!g_tts_loaded)
    delete g_tts
    delete g_tts_pcm
    g_tts_pcm_rate = 0
    g_tts_loaded = false
    queue_release()
}

[export_c(comment = "The voice `tts_speak` speaks in, or \"\" while no model is open.")]
def tts_voice() : string => g_tts_loaded ? g_tts_voice : ""

[export_c(comment = "Pick a voice the model offers - \"\" keeps the current one - and a speech rate from 0.25 to 4.")]
def tts_set_voice(name : string; rate : float) : bool {
    if (!g_tts_loaded) {
        return refuse("no TTS model is open")
    }
    g_tts_rate = rate > 0.0 ? rate : 1.0
    if (empty(name)) {
        return true
    }
    let c <- caps(g_tts)
    if (find_index(c.voices, name) < 0) {
        return refuse("{name} is not a voice this model offers: {join(c.voices, ", ")}")
    }
    g_tts_voice = name
    g_reason = ""
    return true
}

[export_c(comment = "Synthesize `text` into memory: returns the sample count, or 0 with `reason` set.\nRead the samples with tts_pcm.")]
def tts_speak(text : string) : int {
    if (!g_tts_loaded) {
        refuse("no TTS model is open")
        return 0
    }
    let started = ref_time_ticks()
    var audio <- synthesize(g_tts, text, g_tts_voice, g_tts_rate)
    g_tts_us = int64(get_time_usec(started))
    if (empty(audio.pcm)) {
        refuse("{text} synthesized to no audio")
        return 0
    }
    g_tts_seconds = double(length(audio.pcm)) / double(audio.sample_rate)
    delete g_tts_pcm
    g_tts_pcm <- audio.pcm
    g_tts_pcm_rate = audio.sample_rate
    g_reason = ""
    return length(g_tts_pcm)
}

[export_c(comment = "The mono samples of the last tts_speak, in [-1, 1]; valid until the next tts_speak or tts_close.")]
def tts_pcm() : float const? {
    return null if (empty(g_tts_pcm))
    return unsafe(addr<float const?>(g_tts_pcm[0]))
}

[export_c(comment = "The sample rate of tts_pcm, 0 before a tts_speak.")]
def tts_pcm_rate() : int => g_tts_pcm_rate

[export_c(comment = "Seconds of speech synthesized per second of wall clock, 0 before a tts_speak has run.")]
def tts_speed() : double => g_tts_us > 0l ? g_tts_seconds * 1.0e6lf / double(g_tts_us) : 0.0lf

//! the engine's own stats counters are filled by the one-call generate, which a pull loop never runs
def private per_second(tokens, elapsed_us : int64) : double =>
    elapsed_us > 0l ? double(tokens) * 1.0e6lf / double(elapsed_us) : 0.0lf

[export_c]
def prefill_tps() : double {
    //! Prompt tokens per second of the last prefill, 0 before one has run.
    return per_second(g_prefill_tokens, g_prefill_us)
}

[export_c]
def gen_tps() : double {
    //! Generated tokens per second since the last prefill, 0 before one has run.
    return per_second(g_gen_tokens, g_gen_us)
}
