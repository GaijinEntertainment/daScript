options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama
require daslib/export_c
require daslib/jobque_boost
require daslib/fio
require jobque

// dasLLAMA behind a C ABI - text completion and speech to text. Both standalone backends build
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
    if (g_owns_queue && !g_loaded && !g_asr_loaded) {
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
def n_layers() : int {
    return g_loaded ? int(g_model.config.n_layers) : 0
}

[export_c]
def n_vocab() : int {
    return g_loaded ? int(g_model.config.vocab_size) : 0
}

[export_c]
def context_size() : int {
    return g_loaded ? int(g_model.config.seq_len) : 0
}

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

[export_c]
def asr_open(model_path : string) : bool {
    //! Load the speech-to-text model at `model_path` and open one transcription session over it.
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

[export_c]
def asr_close() {
    //! Drop the transcription session and its model.
    return if (!g_asr_loaded)
    delete g_asr_session
    delete g_asr
    g_asr_loaded = false
    queue_release()
}

[export_c]
def asr_transcribe(audio_path : string) : string {
    //! Decode `audio_path` and return its whole transcript, or "" with `reason` set.
    if (!g_asr_loaded) {
        refuse("no ASR model is open")
        return ""
    }
    if (!stat(audio_path).is_valid) {
        refuse("no such file: {audio_path}")
        return ""
    }
    let samples <- load_audio_16k_mono(audio_path)
    if (empty(samples)) {
        refuse("nothing decodable in {audio_path}")
        return ""
    }
    g_audio_seconds = double(length(samples)) / double(AUDIO_SAMPLE_RATE)
    let started = ref_time_ticks()
    let text = transcribe(g_asr, g_asr_session, samples)
    g_asr_us = int64(get_time_usec(started))
    g_reason = ""
    return text
}

[export_c]
def asr_speed() : double {
    //! Seconds of audio transcribed per second of wall clock, 0 before a transcript has run.
    return g_asr_us > 0l ? g_audio_seconds * 1.0e6lf / double(g_asr_us) : 0.0lf
}

//! the engine's own stats counters are filled by the one-call generate, which a pull loop never runs
def private per_second(tokens, elapsed_us : int64) : double {
    return elapsed_us > 0l ? double(tokens) * 1.0e6lf / double(elapsed_us) : 0.0lf
}

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
