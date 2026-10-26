options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_asr_types shared public

// The ASR floor: the types every family file and the facade share. Family modules require
// this instead of each other — capability/segment shapes move up here, never sideways.

require dasllama/dasllama_lint public
require strings

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
    default_lang : string        //!< the code a session takes when none is asked; "" leaves it to the facade's derivation
}

//! Per-layer weight offsets inside a FastConformer family's ``blob`` - canary's record, which parakeet and GigaAM map their
//! layers onto for the shared GPU block chains. ``bn_w``/``bn_b`` become fold scale/shift at load (BN + depthwise bias); a
//! slot a family has none of holds -1.
struct CanaryLayerOffs {
    nf1_w : int64; nf1_b : int64
    ff1a_w : int64; ff1a_b : int64; ff1b_w : int64; ff1b_b : int64
    nat_w : int64; nat_b : int64
    q_w : int64; q_b : int64; k_w : int64; k_b : int64
    v_w : int64; v_b : int64; o_w : int64; o_b : int64
    pos_w : int64; bias_u : int64; bias_v : int64
    ncv_w : int64; ncv_b : int64
    pw1_w : int64; pw1_b : int64
    dw_w : int64; dw_b : int64
    bn_w : int64; bn_b : int64; bn_mean : int64; bn_var : int64
    pw2_w : int64; pw2_b : int64
    nf2_w : int64; nf2_b : int64
    ff2a_w : int64; ff2a_b : int64; ff2b_w : int64; ff2b_b : int64
    nout_w : int64; nout_b : int64
}

//! One transcription segment: [t0, t1] centiseconds, text, raw token ids (timestamps and eot
//! included), and mean per-token log-probability — a confidence signal, more negative = less
//! certain.
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

//! The segment [t0, t1] with `text` and the ids and mean log-probability of `toks` - any records carrying an `id` and a
//! `plog`; an empty `toks` leaves ``avg_logprob`` 0.
def asr_segment(t0, t1 : int64; text : string; toks) : TranscribeSegment {
    var seg = TranscribeSegment(t0 = t0, t1 = t1, text = text)
    var plog_sum = 0.0lf
    seg.tokens |> reserve(length(toks))
    for (t in toks) {
        seg.tokens |> push(t.id)
        plog_sum += double(t.plog)
    }
    if (!empty(toks)) {
        seg.avg_logprob = float(plog_sum / double(length(toks)))
    }
    return <- seg
}

//! SentencePiece detokenize of `toks` - any records carrying an `id` into `vocab`: the space marker becomes a space,
//! the first piece's leading spaces are dropped (the decoder's rule for the prefix marker), and `<...>` control
//! pieces render as nothing - the first piece still counts when it is one.
def asr_spm_detokenize(vocab : array<string>; toks) : string {
    return build_string() $(var w) {
        var first = true
        for (t in toks) {
            let piece = vocab[t.id]
            if (!(length(piece) > 2 && starts_with(piece, "<") && ends_with(piece, ">"))) {
                let s = replace(piece, "▁", " ")   // nolint:STYLE039 (U+2581 is the SentencePiece space marker - vocabulary data, not prose)
                var lead = 0
                if (first) {
                    peek_data(s) $(d) {
                        while (starts_with(d, lead, " ")) {
                            lead++
                        }
                    }
                }
                w |> write(lead > 0 ? slice(s, lead) : s)
            }
            first = false
        }
    }
}
