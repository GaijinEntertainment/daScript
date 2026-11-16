options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama_core
require dasllama/dasllama_host   // the surfaces' state and their shared steps
require daslib/export_c
require daslib/fio
require daslib/strings_boost

// dasLLAMA behind a C ABI - text completion, speech to text, translation and speech synthesis. Both
// backends build this one file; README.md says how. A C caller reads each entry point as
// <library name>_<def name>: dasllama_lib_open, and so on. A model opens from a path, or - the
// *_memory calls - from bytes the host already holds.

[export_c(comment = "One line naming the CPU kernels this build runs - a fat build's classes and the ones this\nbox picks, or the stamped set and the tune file it came from. For the host's log at start.")]
def kernels() : string => kernel_report()

[export_c(comment = "The boundary, in bytes, a prepared image handed to asr_open_memory, tts_open_memory or translate_open_memory starts on.")]
def image_alignment() : int => int(prepared_image_alignment())

// the job queue every surface shares, and this program's prefill intent: a host with no GPU still prefills
def private engine_acquire() {
    queue_acquire()
    allow_cpu_prefill()
}

[export_c]
def open(model_path : string) : bool {
    //! Load the text model at `model_path` and open one session over it.
    if (g_lib.text.loaded) {
        return refuse("a model is already open - call close first")
    }
    if (!stat(model_path).is_valid) {
        return refuse("no such file: {model_path}")
    }
    engine_acquire()
    g_lib.text.model <- load_model(model_path, QuantMode.q8)
    g_lib.text.loaded = true
    if (g_lib.text.model.config.n_layers <= 0l) {
        close()
        return refuse("{model_path} carries no layers - not a model this engine reads")
    }
    g_lib.text.session <- create_session(g_lib.text.model)
    g_lib.text.prefilled = false
    g_lib.reason = ""
    return true
}

[export_c]
def close() {
    //! Drop the session and the model; calling it on a closed library is not an error.
    close_text()
}

[export_c]
def reason() : string {
    //! Why the last call answered false or -1, or "" when none did. A daslang PANIC is a separate
    //! channel: -lib reports it through the generated last_error, -ctx ends the process.
    return g_lib.reason
}

[export_c]
def arch() : string {
    //! The GGUF architecture the loader resolved, or "" while no model is open.
    return g_lib.text.loaded ? g_lib.text.model.arch : ""
}

[export_c]
def n_layers() : int => g_lib.text.loaded ? int(g_lib.text.model.config.n_layers) : 0

[export_c]
def n_vocab() : int => g_lib.text.loaded ? int(g_lib.text.model.config.vocab_size) : 0

[export_c]
def context_size() : int => g_lib.text.loaded ? int(g_lib.text.model.config.seq_len) : 0

[export_c]
def set_sampling(temp : float; top_k : int; seed : int) {
    //! Greedy while `temp <= 0`; `top_k <= 0` keeps the whole distribution.
    g_lib.text.params.temp = temp
    g_lib.text.params.top_k = int64(top_k)
    if (g_lib.text.loaded) {
        set_seed(g_lib.text.session, seed)
    }
}

[export_c]
def prefill(prompt : string) : int {
    //! Encode `prompt` and run it through the model, leaving the session prefilled. Returns the
    //! prompt's token count, or -1 with `reason` set.
    if (!g_lib.text.loaded) {
        refuse("no model is open")
        return -1
    }
    let tokens <- encode(g_lib.text.model, prompt, true, false)
    if (empty(tokens)) {
        refuse("the prompt encodes to no tokens")
        return -1
    }
    let started = ref_time_ticks()
    eval(g_lib.text.model, g_lib.text.session, tokens)
    g_lib.text.prefill_us = int64(get_time_usec(started))
    g_lib.text.prefill_tokens = long_length(tokens)
    g_lib.text.gen_us = 0l
    g_lib.text.gen_tokens = 0l
    g_lib.text.prefilled = true
    g_lib.reason = ""
    return length(tokens)
}

[export_c]
def next_piece() : string {
    //! The next token's text, advancing the session past it; "" until a prefill has run. The raw
    //! completion path carries no stop token, so the caller decides when it has read enough.
    if (!g_lib.text.loaded || !g_lib.text.prefilled) {
        return ""
    }
    let started = ref_time_ticks()
    let id = sample(g_lib.text.session, g_lib.text.params)
    var one : array<int64>
    one |> push(id)
    eval(g_lib.text.model, g_lib.text.session, one)
    g_lib.text.gen_us += int64(get_time_usec(started))
    g_lib.text.gen_tokens ++
    return piece(g_lib.text.model, id)
}

// ===== speech to text =====

[export_c(comment = "Load the speech-to-text model at `model_path` and open one transcription session over it.")]
def asr_open(model_path : string) : bool {
    if (g_lib.asr.loaded) {
        return refuse("an ASR model is already open - call asr_close first")
    }
    if (!stat(model_path).is_valid) {
        return refuse("no such file: {model_path}")
    }
    engine_acquire()
    g_lib.asr.model <- load_asr_model(model_path)
    return start_asr_session()
}

[export_c(comment = "asr_open for a prepared image - a .dlim's bytes - the host holds in memory, `bytes` long, on an\nimage_alignment boundary. The bytes are borrowed, never copied: keep them until asr_close.")]
def asr_open_memory(image : void?; bytes : int64) : bool {
    if (g_lib.asr.loaded) {
        return refuse("an ASR model is already open - call asr_close first")
    }
    return false if (!image_ok(image, bytes))
    engine_acquire()
    g_lib.asr.model <- load_asr_model(image, uint64(bytes))
    return start_asr_session()
}

[export_c(comment = "Drop the transcription session and its model.")]
def asr_close() {
    close_asr()
}

[export_c(comment = "Seconds of audio transcribed per second of wall clock, 0 before a transcript has run.")]
def asr_speed() : double => g_lib.asr.us > 0l ? g_lib.asr.audio_seconds * 1.0e6lf / double(g_lib.asr.us) : 0.0lf

// ===== speech to text from a live stream =====

[export_c(comment = "Start a live stream on the open ASR model; `vad_path` is the Silero VAD that cuts it into\nutterances (modules/dasLLAMA/models/silero_vad.bin). Then pass the audio to asr_feed as it arrives.")]
def asr_listen(vad_path : string) : bool {
    if (!g_lib.asr.loaded) {
        return refuse("no ASR model is open")
    }
    if (!g_lib.listen.vad_loaded) {
        if (!stat(vad_path).is_valid) {
            return refuse("no such file: {vad_path}")
        }
        g_lib.listen.vad <- load_vad_model(vad_path)
        g_lib.listen.vad_loaded = true
    }
    return start_listening()
}

[export_c(comment = "asr_listen for a Silero VAD file's bytes the host holds in memory. The bytes are copied: they are\nfree again when the call returns.")]
def asr_listen_memory(vad : void?; bytes : int64) : bool {
    if (!g_lib.asr.loaded) {
        return refuse("no ASR model is open")
    }
    if (!g_lib.listen.vad_loaded) {
        if (vad == null || bytes <= 0l) {
            return refuse("no VAD bytes")
        }
        g_lib.listen.vad <- load_vad_model(vad, uint64(bytes))
        g_lib.listen.vad_loaded = true
    }
    return start_listening()
}

[export_c(comment = "Feed `count` mono samples at `rate` Hz, in chunks of any size - straight from a capture callback.\nReturns the text of every utterance the VAD closed in this call, \"\" while speech goes on; the\ntranscription runs inside the call, so feed it from a worker thread.")]
def asr_feed(pcm : float const?; count : int; rate : int) : string {
    if (!g_lib.listen.listening) {
        refuse("not listening - call asr_listen first")
        return ""
    }
    if (pcm == null || count <= 0 || rate <= 0) {
        return ""
    }
    let fresh <- resample_to_16k(pcm, count, rate)
    g_lib.listen.take |> push_from(fresh)
    var heard = ""
    vad_iter_feed(g_lib.listen.vad, g_lib.listen.iter, fresh) $(ev : VadEvent) {
        if (ev.kind == VadEventKind.speech_start) {
            g_lib.listen.utterance_start = ev.sample
        } else {
            let text = close_utterance(ev.sample)
            heard = empty(heard) ? text : "{heard} {text}"
        }
    }
    // between utterances keep one second, enough for the padded start of the next one
    if (g_lib.listen.utterance_start < 0l && long_length(g_lib.listen.take) > 2l * AUDIO_SAMPLE_RATE) {
        let drop = long_length(g_lib.listen.take) - AUDIO_SAMPLE_RATE
        g_lib.listen.take |> erase(0l, drop)
        g_lib.listen.take_base += drop
    }
    g_lib.reason = ""
    return heard
}

[export_c(comment = "End the live stream: returns the text of the utterance still open, or \"\".")]
def asr_listen_end() : string {
    let text = g_lib.listen.listening && g_lib.listen.utterance_start >= 0l ? close_utterance(g_lib.listen.take_base + long_length(g_lib.listen.take)) : ""
    g_lib.listen.listening = false
    return text
}

// ===== translation =====

[export_c(comment = "Load the translation model at `model_path`: a Bergamot student - model.<pair>.intgemm.alphas.bin with\nits vocab.<pair>.spm beside it, or its prepared .dlim - or a translation LLM's GGUF.")]
def translate_open(model_path : string) : bool {
    if (g_lib.translate.loaded) {
        return refuse("a translation model is already open - call translate_close first")
    }
    if (!stat(model_path).is_valid) {
        return refuse("no such file: {model_path}")
    }
    engine_acquire()
    g_lib.translate.translator <- load_translator(model_path)
    return start_translating()
}

[export_c(comment = "translate_open for a Bergamot student's prepared image the host holds in memory, on an\nimage_alignment boundary. The bytes are borrowed, never copied: keep them until translate_close.")]
def translate_open_memory(image : void?; bytes : int64) : bool {
    if (g_lib.translate.loaded) {
        return refuse("a translation model is already open - call translate_close first")
    }
    return false if (!image_ok(image, bytes))
    engine_acquire()
    g_lib.translate.translator <- load_translator(image, uint64(bytes))
    return start_translating()
}

[export_c(comment = "The open model's direction as language codes, \"ru-en\"; \"\" while no model is open.")]
def translate_direction() : string {
    return "" if (!g_lib.translate.loaded)
    let dir = translator_langs(g_lib.translate.translator)
    return "{dir.src}-{dir.trg}"
}

[export_c(comment = "Translate `text`; \"\" with `reason` set while no model is open.")]
def translate_text(text : string) : string {
    if (!g_lib.translate.loaded) {
        refuse("no translation model is open")
        return ""
    }
    g_lib.reason = ""
    return translate(g_lib.translate.translator, text)
}

[export_c(comment = "Drop the translation model.")]
def translate_close() {
    close_translate()
}

// ===== speech synthesis =====

[export_c(comment = "Load the text-to-speech model at `model_path` - a Pocket TTS, KittenTTS or Kokoro file -\nand take its last voice as the default.")]
def tts_open(model_path : string) : bool {
    if (g_lib.tts.loaded) {
        return refuse("a TTS model is already open - call tts_close first")
    }
    if (!stat(model_path).is_valid) {
        return refuse("no such file: {model_path}")
    }
    engine_acquire()
    g_lib.tts.model <- load_tts_model(model_path)
    return start_tts(model_path)
}

[export_c(comment = "tts_open for a Pocket TTS prepared image - a .dlim's bytes - the host holds in memory, `bytes` long, on an\nimage_alignment boundary. The bytes are borrowed, never copied: keep them until tts_close. Another family's\nimage answers false with `reason` set, and nothing is borrowed: open that model with tts_open.")]
def tts_open_memory(image : void?; bytes : int64) : bool {
    if (g_lib.tts.loaded) {
        return refuse("a TTS model is already open - call tts_close first")
    }
    return false if (!image_ok(image, bytes))
    if (!tts_loads_from_memory(image, uint64(bytes))) {
        return refuse("the bytes are not a Pocket TTS image - a KittenTTS or Kokoro model reads its packs from its directory, so open it with tts_open")
    }
    engine_acquire()
    g_lib.tts.model <- load_tts_model(image, uint64(bytes))
    return start_tts("the image in memory")
}

[export_c(comment = "Drop the synthesis model.")]
def tts_close() {
    close_tts()
}

[export_c(comment = "The voice `tts_speak` speaks in, or \"\" while no model is open.")]
def tts_voice() : string => g_lib.tts.loaded ? g_lib.tts.voice : ""

[export_c(comment = "Pick a voice the model offers - \"\" keeps the current one - and a speech rate from 0.25 to 4.")]
def tts_set_voice(name : string; rate : float) : bool {
    if (!g_lib.tts.loaded) {
        return refuse("no TTS model is open")
    }
    g_lib.tts.rate = rate > 0.0 ? rate : 1.0
    if (empty(name)) {
        return true
    }
    let c <- caps(g_lib.tts.model)
    if (find_index(c.voices, name) < 0) {
        return refuse("{name} is not a voice this model offers: {join(c.voices, ", ")}")
    }
    g_lib.tts.voice = name
    g_lib.reason = ""
    return true
}

[export_c(comment = "Synthesize `text` into memory: returns the sample count, or 0 with `reason` set.\nRead the samples with tts_pcm.")]
def tts_speak(text : string) : int {
    if (!g_lib.tts.loaded) {
        refuse("no TTS model is open")
        return 0
    }
    let started = ref_time_ticks()
    var audio <- synthesize(g_lib.tts.model, text, g_lib.tts.voice, g_lib.tts.rate)
    g_lib.tts.us = int64(get_time_usec(started))
    if (empty(audio.pcm)) {
        refuse("{text} synthesized to no audio")
        return 0
    }
    g_lib.tts.seconds = double(length(audio.pcm)) / double(audio.sample_rate)
    delete g_lib.tts.pcm
    g_lib.tts.pcm <- audio.pcm
    g_lib.tts.pcm_rate = audio.sample_rate
    g_lib.reason = ""
    return length(g_lib.tts.pcm)
}

[export_c(comment = "The mono samples of the last tts_speak, in [-1, 1]; valid until the next tts_speak or tts_close.")]
def tts_pcm() : float const? {
    return null if (empty(g_lib.tts.pcm))
    return unsafe(addr<float const?>(g_lib.tts.pcm[0]))
}

[export_c(comment = "The sample rate of tts_pcm, 0 before a tts_speak.")]
def tts_pcm_rate() : int => g_lib.tts.pcm_rate

[export_c(comment = "Seconds of speech synthesized per second of wall clock, 0 before a tts_speak has run.")]
def tts_speed() : double => g_lib.tts.us > 0l ? g_lib.tts.seconds * 1.0e6lf / double(g_lib.tts.us) : 0.0lf

[export_c]
def prefill_tps() : double {
    //! Prompt tokens per second of the last prefill, 0 before one has run.
    return per_second(g_lib.text.prefill_tokens, g_lib.text.prefill_us)
}

[export_c]
def gen_tps() : double {
    //! Generated tokens per second since the last prefill, 0 before one has run.
    return per_second(g_lib.text.gen_tokens, g_lib.text.gen_us)
}
