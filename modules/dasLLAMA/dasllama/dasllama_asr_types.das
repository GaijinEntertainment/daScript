options gen2
options _comment_hygiene = true

module dasllama_asr_types shared public

// The ASR floor: the types every family file and the facade share. Family modules require
// this instead of each other — capability/segment shapes move up here, never sideways.

//! Timestamp granularity an ASR backend can produce.
enum AsrTimestamps {
    none
    segment
    word
}

//! What a loaded ASR model honestly supports — advisory (invalid options still panic loudly
//! at the call site, never silently ignore). Query via ``caps``.
struct AsrCaps {
    languages : array<string>    // codes accepted by create_session (sans "auto"); empty = the model detects and reports the language itself
    translate : bool             // speech-to-English translation task
    timestamps : AsrTimestamps
    streaming : bool             // native incremental decode (whisper: false — 30 s windows)
    prompt : bool                // context/prompt conditioning (qwen3a: AsrSession.context)
}

//! One transcription segment: [t0, t1] centiseconds, text, raw token ids (whisper.cpp's
//! result_all shape, timestamps+eot included), and mean per-token log-probability — a
//! confidence signal, more negative = less certain.
struct TranscribeSegment {
    t0 : int64
    t1 : int64
    text : string
    tokens : array<int>
    avg_logprob : float = 0.0
}

//! Fail fast when prompt + audio soft tokens would overrun the decoder's KV cache — the
//! deep alternative is an opaque panic from eval_embd_. Family drivers call this before eval.
def asr_ctx_guard(n_audio, npos, seq_len : int64) {
    if (npos >= seq_len) {
        panic("dasLLAMA asr: {n_audio} audio + prompt tokens = {npos} positions exceed the decoder's {seq_len}-token context - clip too long for a single window")
    }
}
