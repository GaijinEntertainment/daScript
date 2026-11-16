options gen2

module dasllama_host public

require dasllama/dasllama_core
require daslib/fio
require daslib/jobque_boost
require jobque
require strings
require math

//! The state and the shared steps of a host's C surface (examples/dasLLAMA/library): text completion, speech to
//! text, live listening, translation and speech synthesis. Not a shared module: every context a host creates
//! keeps its own `g_lib`.

struct TextSurface {
    model : Model = Model()
    session : Session = Session()
    params : SamplingParams = SamplingParams()
    loaded : bool
    prefilled : bool
    prefill_us : int64
    prefill_tokens : int64
    gen_us : int64
    gen_tokens : int64
}

struct AsrSurface {
    model : AsrModel = AsrModel()
    session : AsrSession = AsrSession()
    loaded : bool
    us : int64
    audio_seconds : double
}

struct ListenSurface {
    vad : VadModel = VadModel()
    vad_loaded : bool
    listening : bool
    iter : VadIter = VadIter()
    take : array<float>        //! the live stream at 16 kHz, from stream sample `take_base` on
    take_base : int64
    utterance_start : int64 = -1l   //! the stream sample the open utterance starts at, -1 when none is open
    rs_pos : double            //! the resampler's read position, relative to the next chunk
    rs_prev : float            //! the previous chunk's last sample, read at position -1
}

struct TranslateSurface {
    translator : Translator = Translator()
    loaded : bool
}

struct TtsSurface {
    model : TtsModel = TtsModel()
    loaded : bool
    voice : string
    rate : float = 1.0
    us : int64
    seconds : double
    pcm : array<float>
    pcm_rate : int
}

struct LibHost {
    text : TextSurface = TextSurface()
    asr : AsrSurface = AsrSurface()
    listen : ListenSurface = ListenSurface()
    translate : TranslateSurface = TranslateSurface()
    tts : TtsSurface = TtsSurface()
    reason : string    //! why the last call answered false or -1
    owns_queue : bool
}

var g_lib = LibHost()

def refuse(why : string) : bool {
    g_lib.reason = why
    return false
}

//! opened once for every surface, and released only when no model is open
def queue_acquire() {
    if (!is_job_que_available()) {
        create_job_que()
        g_lib.owns_queue = true
    }
    setup_dasllama_jobque()
}

def queue_release() {
    if (g_lib.owns_queue && !g_lib.text.loaded && !g_lib.asr.loaded && !g_lib.tts.loaded && !g_lib.translate.loaded) {
        destroy_job_que()
        g_lib.owns_queue = false
    }
}

//! refuses bytes no *_memory call can take; the engine itself panics on an image off its boundary
def image_ok(image : void?; bytes : int64) : bool {
    if (image == null || bytes <= 0l) {
        return refuse("no image bytes")
    }
    if (intptr(image) % uint64(prepared_image_alignment()) != 0ul) {
        return refuse("a prepared image starts on a {prepared_image_alignment()}-byte boundary")
    }
    return true
}

def close_text() {
    return if (!g_lib.text.loaded)
    delete g_lib.text.session
    delete g_lib.text.model
    g_lib.text.loaded = false
    g_lib.text.prefilled = false
    queue_release()
}

def start_asr_session() : bool {
    // fresh, or finalized by close_asr
    g_lib.asr.session <- create_session(g_lib.asr.model, "")
    g_lib.asr.loaded = true
    g_lib.reason = ""
    return true
}

def close_asr() {
    return if (!g_lib.asr.loaded)
    delete g_lib.asr.session
    delete g_lib.asr.model
    if (g_lib.listen.vad_loaded) {
        delete g_lib.listen.vad
        g_lib.listen.vad_loaded = false
    }
    g_lib.listen.listening = false
    delete g_lib.listen.take
    g_lib.asr.loaded = false
    queue_release()
}

def start_listening() : bool {
    var opts = default_vad_opts()
    opts.min_silence_ms = 500   // an utterance ends at a half-second pause, not at every breath in a sentence
    opts.speech_pad_ms = 200    // keep the quiet ends of words: ASR reads a cut-off syllable badly
    assume l = g_lib.listen
    vad_iter_reset(l.iter, opts)
    l.take |> clear()
    l.take_base = 0l
    l.utterance_start = -1l
    l.rs_pos = 0.0lf
    l.rs_prev = 0.0
    l.listening = true
    g_lib.reason = ""
    return true
}

//! `count` samples at `rate` Hz as 16 kHz samples, linearly interpolated across chunks
def resample_to_16k(pcm : float const?; count, rate : int) : array<float> {
    var out : array<float>
    let step = double(rate) / double(AUDIO_SAMPLE_RATE)
    out |> reserve(int(double(count) / step) + 2)
    assume l = g_lib.listen
    var pos = l.rs_pos
    while (pos < double(count - 1)) {
        let i = int(floor(pos))
        let a = i < 0 ? l.rs_prev : unsafe(pcm[i])
        out |> push(lerp(a, unsafe(pcm[i + 1]), float(pos - double(i))))
        pos += step
    }
    l.rs_pos = pos - double(count)
    l.rs_prev = unsafe(pcm[count - 1])
    return <- out
}

//! transcribes the open utterance up to stream sample `end`, and drops the audio before `end`
def close_utterance(end : int64) : string {
    assume l = g_lib.listen
    let s = clamp(l.utterance_start - l.take_base, 0l, long_length(l.take))
    let e = clamp(end - l.take_base, s, long_length(l.take))
    var utterance : array<float>
    utterance |> resize(e - s)
    for (i in range64(e - s)) {
        utterance[i] = l.take[s + i]
    }
    l.take |> erase(0l, e)
    l.take_base += e
    l.utterance_start = -1l
    if (empty(utterance)) {
        return ""
    }
    g_lib.asr.audio_seconds = double(length(utterance)) / double(AUDIO_SAMPLE_RATE)
    let started = ref_time_ticks()
    let text = strip(transcribe(g_lib.asr.model, g_lib.asr.session, utterance))
    g_lib.asr.us = int64(get_time_usec(started))
    return text
}

def start_translating() : bool {
    g_lib.translate.loaded = true
    g_lib.reason = ""
    return true
}

def close_translate() {
    return if (!g_lib.translate.loaded)
    delete g_lib.translate.translator
    g_lib.translate.loaded = false
    queue_release()
}

def start_tts(what : string) : bool {
    g_lib.tts.loaded = true
    let c <- caps(g_lib.tts.model)
    if (empty(c.voices)) {
        close_tts()
        return refuse("{what} offers no voice")
    }
    g_lib.tts.voice = c.voices[length(c.voices) - 1]
    g_lib.reason = ""
    return true
}

def close_tts() {
    return if (!g_lib.tts.loaded)
    delete g_lib.tts.model
    delete g_lib.tts.pcm
    g_lib.tts.pcm_rate = 0
    g_lib.tts.loaded = false
    queue_release()
}

//! the engine's own stats counters are filled by the one-call generate, which a pull loop never runs
def per_second(tokens, elapsed_us : int64) : double =>
    elapsed_us > 0l ? double(tokens) * 1.0e6lf / double(elapsed_us) : 0.0lf
