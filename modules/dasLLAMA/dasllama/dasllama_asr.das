options gen2
options _comment_hygiene = true

module dasllama_asr shared public

require math
require daslib/fio
require strings
require daslib/strings_boost
require dasllama/dasllama_gguf   // rd_u32 for the magic sniff
require dasllama/dasllama_audio  // EncoderState (the qwen3a tower's scratch)
require dasllama/dasllama_whisper public
require dasllama/dasllama_qwen3a public
require dasllama/dasllama_parakeet public
require dasllama/dasllama_canary public
require dasllama/dasllama_gemma4a public
require dasllama/dasllama_transformer   // nolint:STYLE029 — the qwen3a decoder is a full LLM; umbrella fires arch [init]s

// The uniform ASR surface: one loader that sniffs the file format, one verb set that
// dispatches internally — NO family names in the public API. Backends: whisper (ggml bin)
// and Qwen3-ASR (GGUF decoder + qwen3a mmproj); Parakeet lands behind the same sniff.

//! Timestamp granularity an ASR backend can produce.
enum AsrTimestamps {
    none
    segment
    word
}

enum private AsrKind {
    whisper
    qwen3a
    parakeet
    canary
    gemma4a
    qwen3omni
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

//! A loaded ASR model of whatever family the files turned out to be.
struct AsrModel {
    kind : AsrKind = AsrKind.whisper
    whisper : WhisperModel = WhisperModel()
    dec : Model = Model()              // qwen3a / canary: the qwen3 decoder LLM
    q3t : Qwen3aTower = Qwen3aTower()  // qwen3a: the chunked audio encoder
    pk : ParakeetModel = ParakeetModel()
    cenc : CanaryEncoder = CanaryEncoder()   // canary: the FastConformer-32 encoder + projector
    g4a : Gemma4aEncoder = Gemma4aEncoder()  // gemma4a: the Conformer audio encoder + mm embedder
}

//! One transcription stream over an AsrModel: per-backend scratch plus shared options.
//! ``translate``/``no_timestamps`` apply where supported (see caps); ``context`` is qwen3a's
//! context-biasing text; ``detected_lang`` fills after (whisper: resolved code; qwen3a: model's own report).
struct AsrSession {
    w : WhisperSession = WhisperSession()
    qctx : Session = Session()
    qenc : EncoderState = EncoderState()
    qcs : Qwen3aConvState = Qwen3aConvState()
    pkst : ParakeetState = ParakeetState()   // parakeet hot-path scratch — zero allocation per transcribe
    cst : CanaryState = CanaryState()        // canary encoder scratch (shares qctx for the decoder)
    g4st : Gemma4aState = Gemma4aState()     // gemma4a encoder scratch (shares qctx for the decoder)
    translate : bool = false
    no_timestamps : bool = false
    context : string = ""
    detected_lang : string = ""
}

// exact-fp32 knob for oracle rails: flip BEFORE load_asr_model to skip the Q8 fast path
var private g_asr_force_fp32 = false

//! Force the fp32 (bit-exact-with-oracle) path for models loaded AFTER this call — the
//! default load quantizes the encoder GEMMs to Q8_0 (~4x on CPU, tolerance-gated).
def set_asr_fp32(on : bool) {
    g_asr_force_fp32 = on
}

//! Load an ASR model, sniffing the format: ggml-bin magic = whisper vs parakeet
//! (disambiguated on vocab size: whisper >= 51864, parakeet 1k/8k); GGUF needs the model's
//! mmproj too — use the two-argument overload. One loader, no family names.
def load_asr_model(path : string) : AsrModel {
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA asr: cannot open model '{path}'")
    }
    // stat, not length(bytes): the fmap array's length() wraps int32 on >2GB files (whisper
    // large-v3 / parakeet f32 bins), which would reject valid models here
    if (stat(path).size < 8ul) {
        panic("dasLLAMA asr: '{path}' is too short to be a model file ({stat(path).size} bytes)")
    }
    var magic = 0u
    var vocab = 0
    fmap(f) $(var bytes : array<uint8>#) {
        magic = rd_u32(bytes, 0l)
        vocab = int(rd_u32(bytes, 4l))   // ggml-bin: first hparam after the magic is n_vocab
    }
    fclose(f)
    if (magic == 0x46554747u) {   // "GGUF"
        panic("dasLLAMA asr: '{path}' is a GGUF decoder — pass its mmproj too: load_asr_model(path, mmproj_path)")
    }
    if (magic != 0x67676d6cu) {
        panic("dasLLAMA asr: '{path}' is neither a GGUF nor a ggml bin")
    }
    if (vocab < 32768) {   // parakeet SPM vocabs: 1024 (v2, English) / 8192 (v3, 25-lang)
        var pm = AsrModel(kind = AsrKind.parakeet)
        pm.pk <- load_parakeet_model(path, !g_asr_force_fp32)
        return <- pm
    }
    // whisper vocabs: 51864 (.en) / 51865 (multilingual) / 51866 (large-v3 family); anything
    // else with the ggml magic (e.g. a legacy GPT-2 bin, 50257) fails HERE, not deep in the loader
    if (vocab < 51864 || vocab > 51999) {
        panic("dasLLAMA asr: '{path}' has n_vocab {vocab} — matches neither whisper (51864-51866) nor parakeet (~1k)")
    }
    var m = AsrModel(kind = AsrKind.whisper)
    m.whisper <- load_whisper_model(path, !g_asr_force_fp32)
    return <- m
}

def load_asr_model(path : string; mmproj : string) : AsrModel {
    //! The two-file route: ``path`` is the decoder GGUF, ``mmproj`` its audio front end — sniffed:
    //! ``qwen3a`` mmproj = Qwen3-ASR/Qwen3-Omni AuT tower, ``gemma4a`` = Gemma-4 Conformer, "CNRY"
    //! bin = Canary-Qwen FastConformer. Canary decoder loads fp32 (parity gate); others load Q8.
    let ef = fopen(mmproj, "rb")
    if (ef == null) {
        panic("dasLLAMA asr: cannot open mmproj '{mmproj}'")
    }
    // stat, not length(bytes): the fmap array's length() wraps int32 on >2GB files (the fp32
    // canary encoder is ~3GB), which would misroute the sniff
    if (stat(mmproj).size < 4ul) {
        panic("dasLLAMA asr: mmproj '{mmproj}' is too short")
    }
    var emagic = 0u
    var eproj = ""
    fmap(ef) $(var bytes : array<uint8>#) {
        emagic = rd_u32(bytes, 0l)
        if (emagic == 0x46554747u) {   // "GGUF" — sniff the audio projector type
            let mm <- parse_gguf_meta(bytes)
            if (gguf_has(mm, "clip.audio.projector_type")) {
                eproj = gguf_str(mm, bytes, "clip.audio.projector_type")
            } elif (gguf_has(mm, "clip.projector_type")) {
                eproj = gguf_str(mm, bytes, "clip.projector_type")
            }
        }
    }
    fclose(ef)
    if (eproj == "gemma4a") {
        var m = AsrModel(kind = AsrKind.gemma4a)
        m.dec <- load_model_(path, QuantMode.q8)
        if (m.dec.arch != "gemma4") {
            panic("dasLLAMA asr: '{path}' is arch '{m.dec.arch}' — the gemma4a audio route expects a Gemma-4 decoder")
        }
        m.dec.config.seq_len = min(m.dec.config.seq_len, 8192l)
        m.g4a <- load_gemma4a_encoder(mmproj)
        if (m.g4a.proj_dim != m.dec.config.dim) {
            panic("dasLLAMA asr: gemma4a encoder projects to {m.g4a.proj_dim} but the decoder is {m.dec.config.dim}-wide — mismatched pair")
        }
        return <- m
    }
    if (emagic == CANARY_MAGIC) {
        var m = AsrModel(kind = AsrKind.canary)
        m.dec <- load_model_(path, QuantMode.fp32)
        if (m.dec.arch != "qwen3") {
            panic("dasLLAMA asr: '{path}' is arch '{m.dec.arch}' — the Canary route expects a Qwen3-1.7B decoder")
        }
        m.dec.config.seq_len = min(m.dec.config.seq_len, 8192l)
        m.cenc <- load_canary_encoder(mmproj)
        if (m.cenc.proj_dim != m.dec.config.dim) {
            panic("dasLLAMA asr: canary encoder projects to {m.cenc.proj_dim} but the decoder is {m.dec.config.dim}-wide — mismatched pair")
        }
        return <- m
    }
    var m = AsrModel(kind = AsrKind.qwen3a)
    m.dec <- load_model_(path, QuantMode.q8)
    // Same qwen3a AuT tower, two thinkers: Qwen3-ASR is a dense qwen3 (arch `qwen3`/`qwen3vl`);
    // Qwen3-Omni-30B is the MoE thinker (arch `qwen3vlmoe`), driven as a chat/transcription turn.
    if (m.dec.arch == "qwen3vlmoe") {
        m.kind = AsrKind.qwen3omni
    } elif (m.dec.arch != "qwen3vl" && m.dec.arch != "qwen3") {
        panic("dasLLAMA asr: '{path}' is arch '{m.dec.arch}' — the GGUF ASR route expects a Qwen3-ASR (qwen3/qwen3vl) or Qwen3-Omni (qwen3vlmoe) decoder")
    }
    m.dec.config.seq_len = min(m.dec.config.seq_len, 8192l)
    m.q3t <- load_qwen3a_tower(mmproj)
    if (m.q3t.proj_dim != m.dec.config.dim) {
        panic("dasLLAMA asr: mmproj projects to {m.q3t.proj_dim} but the decoder is {m.dec.config.dim}-wide — mismatched pair")
    }
    return <- m
}

//! What this model supports: Whisper's own language set (translation, segment timestamps);
//! Qwen3-ASR self-detects language (no timestamps, context biasing via ``context``).
// v3 (25-lang) has an 8192-piece SPM vocab; v2 (English) 1024 — pk_is_multilingual's threshold
def private pk_is_multilingual(m : AsrModel) : bool => m.pk.n_vocab >= 8192l

def caps(m : AsrModel) : AsrCaps {
    if (m.kind == AsrKind.gemma4a) {
        // Gemma-4 E-series audio: multimodal chat model; the ASR rail drives a greedy
        // transcription turn. No timestamps; the model reasons then transcribes.
        var gc = AsrCaps(translate = false, timestamps = AsrTimestamps.none,
                         streaming = false, prompt = false)
        gc.languages <- ["en"]
        return <- gc
    }
    if (m.kind == AsrKind.canary) {
        var cc = AsrCaps(translate = false, timestamps = AsrTimestamps.none,
                         streaming = false, prompt = false)
        cc.languages <- ["en"]
        return <- cc
    }
    if (m.kind == AsrKind.qwen3omni) {
        // Qwen3-Omni-30B audio understanding: a chat model driven as a transcription turn. The
        // model handles language internally (languages empty); no timestamps, no streaming.
        return <- AsrCaps(translate = false, timestamps = AsrTimestamps.none,
                          streaming = false, prompt = false)
    }
    if (m.kind == AsrKind.qwen3a) {
        return <- AsrCaps(translate = false, timestamps = AsrTimestamps.none,
                          streaming = false, prompt = true)
    }
    if (m.kind == AsrKind.parakeet) {
        var pc = AsrCaps(translate = false, timestamps = AsrTimestamps.segment,
                         streaming = false, prompt = false)
        if (pk_is_multilingual(m)) {
            pc.languages <- ["bg", "hr", "cs", "da", "nl", "en", "et", "fi", "fr", "de",
                             "el", "hu", "it", "lv", "lt", "mt", "pl", "pt", "ro", "sk",
                             "sl", "es", "sv", "ru", "uk"]
        } else {
            pc.languages <- ["en"]
        }
        return <- pc
    }
    var c = AsrCaps(translate = m.whisper.multilingual,
                    timestamps = AsrTimestamps.segment,
                    streaming = false,
                    prompt = false)
    if (m.whisper.multilingual) {
        c.languages <- whisper_lang_list()
        if (int64(length(c.languages)) > m.whisper.n_lang) {   // 99-lang models predate "yue"
            c.languages |> resize(int(m.whisper.n_lang))
        }
    } else {
        c.languages <- ["en"]
    }
    return <- c
}

def create_session(m : AsrModel; lang : string = "en") : AsrSession {
    //! Create a transcription session. ``lang`` is a language code from ``caps().languages``,
    //! or "auto" to detect (whisper multilingual models; Qwen3-ASR always detects — it only
    //! accepts "auto").
    var s = AsrSession()
    if (m.kind == AsrKind.parakeet) {
        // parakeet has no language conditioning — the TDT decode transcribes whatever it
        // hears; lang is validated against the model's set but otherwise unused
        if (lang != "auto" && !has_value(caps(m).languages, lang)) {
            panic(pk_is_multilingual(m) ?
                "dasLLAMA asr: '{lang}' is not one of this Parakeet model's 25 languages" :
                "dasLLAMA asr: this Parakeet model is English-only")
        }
        return <- s
    }
    if (m.kind == AsrKind.canary) {
        if (lang != "auto" && lang != "en") {
            panic("dasLLAMA asr: this Canary-Qwen model is English-only — create the session with lang=\"en\" or \"auto\"")
        }
        s.qctx <- create_session_(m.dec)
        return <- s
    }
    if (m.kind == AsrKind.gemma4a) {
        s.qctx <- create_session_(m.dec)
        return <- s
    }
    if (m.kind == AsrKind.qwen3omni) {
        s.qctx <- create_session_(m.dec)
        s.qenc <- make_encoder_state(m.q3t.core)
        return <- s
    }
    if (m.kind == AsrKind.qwen3a) {
        if (lang != "auto") {
            panic("dasLLAMA asr: Qwen3-ASR detects the language itself — create the session with lang=\"auto\"")
        }
        s.qctx <- create_session_(m.dec)
        s.qenc <- make_encoder_state(m.q3t.core)
    } else {
        s.w <- create_session(m.whisper, lang)
    }
    return <- s
}

// The qwen3a transcription turn: chatml with the audio span in <|audio_start|>/<|audio_end|>,
// the context text after it, greedy decode, and the model's `language {L}<asr_text>{text}`
// response parsed into (detected_lang, text).
def private transcribe_qwen3a(m : AsrModel; var s : AsrSession; samples : array<float> | #;
                              blk : block<(seg : TranscribeSegment)>) : int {
    let mel <- log_mel_qwen3a(samples, m.q3t.core.n_mel)
    var rows : array<float>
    let n_audio = qwen3a_encode_all(m.q3t, s.qenc, s.qcs, mel, rows)

    let dim = m.dec.config.dim
    let im_start = special_id(m.dec, "<|im_start|>")
    let im_end = special_id(m.dec, "<|im_end|>")
    var head : array<int64>
    head |> push(im_start)
    head |> push_from(encode_(m.dec, "user\n", false, false))
    head |> push(special_id(m.dec, "<|audio_start|>"))
    var tail : array<int64>
    tail |> push(special_id(m.dec, "<|audio_end|>"))
    if (!empty(s.context)) {
        tail |> push_from(encode_(m.dec, s.context, false, false))
    }
    tail |> push(im_end)
    tail |> push_from(encode_(m.dec, "\n", false, false))
    tail |> push(im_start)
    tail |> push_from(encode_(m.dec, "assistant\n", false, false))

    let n_head = int64(length(head))
    let n_tail = int64(length(tail))
    let npos = n_head + n_audio + n_tail
    var embd : array<float>
    embd |> resize(int(npos * dim))
    embed_text_rows(m.dec, head, embd, 0l)
    for (i in range64(n_audio * dim)) {
        embd[n_head * dim + i] = rows[i]
    }
    embed_text_rows(m.dec, tail, embd, n_head + n_audio)

    s.qctx.n_past = 0l   // each transcribe is a fresh sequence; the KV rewinds to position 0
    eval_embd_(m.dec, s.qctx, embd, npos)

    let eot = special_id(m.dec, "<|endoftext|>")
    let max_new = m.dec.config.seq_len - npos - 1l
    var one : array<int64>
    one |> resize(1)
    var toks : array<int>
    var plog_sum = 0.0lf
    var raw : array<string>
    for (_i in range64(max_new)) {
        // greedy argmax + its logprob (logit − logsumexp) for the confidence signal
        var best = 0
        for (i in range(1, length(s.qctx.logits))) {
            if (s.qctx.logits[i] > s.qctx.logits[best]) {
                best = i
            }
        }
        let mx = s.qctx.logits[best]
        var lse = 0.0lf
        for (v in s.qctx.logits) {
            lse += double(exp(v - mx))
        }
        plog_sum += double(s.qctx.logits[best]) - (log(lse) + double(mx))
        if (int64(best) == im_end || int64(best) == eot) {
            break
        }
        toks |> push(best)
        raw |> push(piece_(m.dec, int64(best)))
        one[0] = int64(best)
        eval_(m.dec, s.qctx, one)
    }

    // parse `language {L}<asr_text>{text}` (the marker may detokenize as text or be muted)
    let full = join(raw, "")
    var text = full
    s.detected_lang = ""
    let mark = find(full, "<asr_text>")
    if (mark >= 0) {
        let pre = slice(full, 0, mark)
        if (starts_with(pre, "language ")) {
            s.detected_lang = strip(slice(pre, length("language "), length(pre)))
        }
        text = slice(full, mark + length("<asr_text>"), length(full))
    }
    var seg = TranscribeSegment(t0 = 0l, t1 = int64(length(samples)) * 100l / AUDIO_SAMPLE_RATE, text = text)
    seg.tokens := toks
    if (!empty(toks)) {
        seg.avg_logprob = float(plog_sum / double(length(toks) + 1))   // + the stop token's plog
    }
    blk |> invoke(seg)
    return 1
}

// Qwen3-Omni-30B transcription: the SHARED qwen3a AuT tower encodes the clip; soft tokens splice
// into a plain Qwen chatml turn, greedy-decoded until <|im_end|> (emitted into the ids — mtmd
// convention). Token-for-token vs llama-mtmd-cli --temp 0 --jinja on the same Q8 thinker + bf16 mmproj.
def private transcribe_qwen3omni(m : AsrModel; var s : AsrSession; samples : array<float> | #;
                                 blk : block<(seg : TranscribeSegment)>) : int {
    let tp = ref_time_ticks()
    let mel <- log_mel_qwen3a(samples, m.q3t.core.n_mel)
    var arows : array<float>
    let n_audio = qwen3a_encode_all(m.q3t, s.qenc, s.qcs, mel, arows)
    asr_prof_add("q3o.encode", tp)

    let dim = m.dec.config.dim
    let im_start = special_id(m.dec, "<|im_start|>")
    let im_end = special_id(m.dec, "<|im_end|>")   // = the EOG (151645)
    var head : array<int64>
    head |> push(im_start)
    head |> push_from(encode_(m.dec, "user\n", false, false))
    head |> push(special_id(m.dec, "<|audio_start|>"))
    var tail : array<int64>
    tail |> push(special_id(m.dec, "<|audio_end|>"))
    tail |> push_from(encode_(m.dec, "Transcribe the audio.", false, false))
    tail |> push(im_end)
    tail |> push_from(encode_(m.dec, "\n", false, false))
    tail |> push(im_start)
    tail |> push_from(encode_(m.dec, "assistant\n", false, false))

    let n_head = int64(length(head))
    let npos = n_head + n_audio + int64(length(tail))
    asr_ctx_guard(n_audio, npos, m.dec.config.seq_len)
    var embd : array<float>
    embd |> resize(int(npos * dim))
    embed_text_rows(m.dec, head, embd, 0l)
    for (i in range64(n_audio * dim)) {
        embd[n_head * dim + i] = arows[i]
    }
    embed_text_rows(m.dec, tail, embd, n_head + n_audio)

    s.qctx.n_past = 0l   // fresh sequence per transcribe
    let td = ref_time_ticks()
    eval_embd_(m.dec, s.qctx, embd, npos)

    let eot = special_id(m.dec, "<|endoftext|>")
    let max_new = m.dec.config.seq_len - npos - 1l
    var one : array<int64>
    one |> resize(1)
    var toks : array<int>
    var raw : array<string>
    toks |> reserve(256)
    raw |> reserve(256)
    for (_i in range64(max_new)) {
        var best = 0
        for (i in range(1, length(s.qctx.logits))) {
            if (s.qctx.logits[i] > s.qctx.logits[best]) {
                best = i
            }
        }
        toks |> push(best)
        if (int64(best) == im_end || int64(best) == eot) {   // EOG: emitted into the ids, then stop
            break
        }
        raw |> push(piece_(m.dec, int64(best)))
        one[0] = int64(best)
        eval_(m.dec, s.qctx, one)
    }
    asr_prof_add("q3o.decode", td)

    var seg = TranscribeSegment(t0 = 0l, t1 = int64(length(samples)) * 100l / AUDIO_SAMPLE_RATE,
        text = strip(join(raw, "")))
    seg.tokens := toks
    blk |> invoke(seg)
    return 1
}

// The decoder's KV cache is sized at seq_len; if prompt + audio soft tokens reach it, eval_embd_
// would write past the cache and panic opaquely. Fail fast with a clear "clip too long" message.
def private asr_ctx_guard(n_audio, npos, seq_len : int64) {
    if (npos >= seq_len) {
        panic("dasLLAMA asr: {n_audio} audio + prompt tokens = {npos} positions exceed the decoder's {seq_len}-token context — clip too long for a single window")
    }
}

// Canary-Qwen transcription: FastConformer-32 encode + projector, the SALM/Qwen prompt with the
// audio soft tokens spliced at the placeholder, greedy decode over the Qwen3-1.7B decoder until
// the text EOS (151645, which is included in the emitted ids — the NeMo generate convention).
def private transcribe_canary(m : AsrModel; var s : AsrSession; samples : array<float> | #;
                              blk : block<(seg : TranscribeSegment)>) : int {
    let tp = ref_time_ticks()
    canary_log_mel(m.cenc, samples, s.cst)
    let n_audio = canary_encode(m.cenc, s.cst)   // s.cst.out holds [n_audio × proj_dim] soft tokens
    asr_prof_add("cn.encode", tp)

    let dim = m.dec.config.dim
    let im_start = special_id(m.dec, "<|im_start|>")
    let im_end = special_id(m.dec, "<|im_end|>")   // = the text EOS (151645)
    // SALM "qwen" prompt: <|im_start|>user\nTranscribe the following: <AUDIO><|im_end|>\n<|im_start|>assistant\n
    var head : array<int64>
    head |> push(im_start)
    head |> push_from(encode_(m.dec, "user\n", false, false))
    head |> push_from(encode_(m.dec, "Transcribe the following: ", false, false))
    var tail : array<int64>
    tail |> push(im_end)
    tail |> push_from(encode_(m.dec, "\n", false, false))
    tail |> push(im_start)
    tail |> push_from(encode_(m.dec, "assistant\n", false, false))

    let n_head = int64(length(head))
    let npos = n_head + n_audio + int64(length(tail))
    asr_ctx_guard(n_audio, npos, m.dec.config.seq_len)
    var embd : array<float>
    embd |> resize(int(npos * dim))
    embed_text_rows(m.dec, head, embd, 0l)
    for (i in range64(n_audio * dim)) {
        embd[n_head * dim + i] = s.cst.out[i]
    }
    embed_text_rows(m.dec, tail, embd, n_head + n_audio)

    s.qctx.n_past = 0l   // fresh sequence per transcribe
    eval_embd_(m.dec, s.qctx, embd, npos)

    let max_new = m.dec.config.seq_len - npos - 1l
    var one : array<int64>
    one |> resize(1)
    var toks : array<int>
    var raw : array<string>
    toks |> reserve(128)   // typical transcription length; grows if a clip runs longer
    raw |> reserve(128)
    var plog_sum = 0.0lf
    for (_i in range64(max_new)) {
        var best = 0
        for (i in range(1, length(s.qctx.logits))) {
            if (s.qctx.logits[i] > s.qctx.logits[best]) {
                best = i
            }
        }
        let mx = s.qctx.logits[best]
        var lse = 0.0lf
        for (v in s.qctx.logits) {
            lse += double(exp(v - mx))
        }
        plog_sum += double(s.qctx.logits[best]) - (log(lse) + double(mx))
        toks |> push(best)
        if (int64(best) == im_end) {   // EOS: emitted into the ids, then stop
            break
        }
        raw |> push(piece_(m.dec, int64(best)))
        one[0] = int64(best)
        eval_(m.dec, s.qctx, one)
    }

    var seg = TranscribeSegment(t0 = 0l, t1 = int64(length(samples)) * 100l / AUDIO_SAMPLE_RATE,
        text = strip(join(raw, "")))
    seg.tokens := toks
    if (!empty(toks)) {
        seg.avg_logprob = float(plog_sum / double(length(toks)))
    }
    s.detected_lang = "en"
    blk |> invoke(seg)
    return 1
}

// Gemma-4 E-series audio transcription: gemma4a Conformer encode + mm embedder, soft tokens spliced
// into the gemma-4 chat template, greedy decode until `<turn|>`/`<eos>` (emitted into the ids). The
// reasoning `<|channel>...<channel|>` prefix is stripped. Token-for-token vs llama-mtmd-cli --temp 0 --jinja.
def private transcribe_gemma4a(m : AsrModel; var s : AsrSession; samples : array<float> | #;
                               blk : block<(seg : TranscribeSegment)>) : int {
    var tp = ref_time_ticks()
    var rows : array<float>
    let n_audio = gemma4a_encode_all(m.g4a, s.g4st, samples, rows)
    asr_prof_add("g4a.encode", tp)

    let dim = m.dec.config.dim
    let nl = encode_(m.dec, "\n", false, false)
    // no-think turn: a bare user turn (no system/`<|think|>`) yields a deterministic transcription
    // — the reasoning channel is near-tie-brittle across float engines, so it is disabled here.
    var head : array<int64>
    head |> push(special_id(m.dec, "<bos>"))
    head |> push(special_id(m.dec, "<|turn>"))
    head |> push_from(encode_(m.dec, "user", false, false))
    head |> push_from(nl)
    head |> push(special_id(m.dec, "<|audio>"))
    var tail : array<int64>
    tail |> push(special_id(m.dec, "<audio|>"))
    tail |> push_from(encode_(m.dec, "Transcribe this audio.", false, false))
    tail |> push(special_id(m.dec, "<turn|>"))
    tail |> push_from(nl)
    tail |> push(special_id(m.dec, "<|turn>"))
    tail |> push_from(encode_(m.dec, "model", false, false))
    tail |> push_from(nl)

    let n_head = int64(length(head))
    let npos = n_head + n_audio + int64(length(tail))
    asr_ctx_guard(n_audio, npos, m.dec.config.seq_len)
    var embd : array<float>
    embd |> resize(int(npos * dim))
    embed_text_rows(m.dec, head, embd, 0l)
    for (i in range64(n_audio * dim)) {
        embd[n_head * dim + i] = rows[i]
    }
    embed_text_rows(m.dec, tail, embd, n_head + n_audio)

    s.qctx.n_past = 0l   // fresh sequence per transcribe
    tp = ref_time_ticks()
    eval_embd_(m.dec, s.qctx, embd, npos)

    let turn_end = special_id(m.dec, "<turn|>")
    let eos = special_id(m.dec, "<eos>")
    let max_new = m.dec.config.seq_len - npos - 1l
    var one : array<int64>
    one |> resize(1)
    var toks : array<int>
    var raw : array<string>
    toks |> reserve(512)
    raw |> reserve(512)
    for (_i in range64(max_new)) {
        var best = 0
        for (i in range(1, length(s.qctx.logits))) {
            if (s.qctx.logits[i] > s.qctx.logits[best]) {
                best = i
            }
        }
        toks |> push(best)
        if (int64(best) == turn_end || int64(best) == eos) {   // EOG: emitted into the ids, then stop
            break
        }
        raw |> push(piece_(m.dec, int64(best)))
        one[0] = int64(best)
        eval_(m.dec, s.qctx, one)
    }
    asr_prof_add("g4a.decode", tp)

    // strip the reasoning channel: keep the text after the last `<channel|>` (the final transcription)
    let full = join(raw, "")
    var text = full
    let mark = rfind(full, "<channel|>")
    if (mark >= 0) {
        text = slice(full, mark + length("<channel|>"), length(full))
    }
    var seg = TranscribeSegment(t0 = 0l, t1 = int64(length(samples)) * 100l / AUDIO_SAMPLE_RATE,
        text = strip(text))
    seg.tokens := toks
    s.detected_lang = "en"
    blk |> invoke(seg)
    return 1
}

// Parakeet transcription: full-clip FastConformer encode + TDT greedy, one segment.
def private transcribe_parakeet(m : AsrModel; var s : AsrSession; samples : array<float> | #;
                                blk : block<(seg : TranscribeSegment)>) : int {
    var tp = ref_time_ticks()
    parakeet_log_mel(samples, m.pk, s.pkst)
    asr_prof_add("pk.mel", tp)
    tp = ref_time_ticks()
    let tt = parakeet_encode(m.pk, s.pkst)
    asr_prof_add("pk.encode_total", tp)
    tp = ref_time_ticks()
    parakeet_tdt_decode(m.pk, s.pkst, tt)
    asr_prof_add("pk.decode_total", tp)
    var seg = TranscribeSegment(t0 = 0l, t1 = int64(length(samples)) * 100l / AUDIO_SAMPLE_RATE,
        text = parakeet_detokenize(m.pk, s.pkst.toks))
    var plog_sum = 0.0lf
    seg.tokens |> reserve(length(s.pkst.toks))
    for (t in s.pkst.toks) {
        seg.tokens |> push(t.id)
        plog_sum += double(t.plog)
    }
    if (!empty(s.pkst.toks)) {
        seg.avg_logprob = float(plog_sum / double(length(s.pkst.toks)))
    }
    if (!pk_is_multilingual(m)) {
        s.detected_lang = "en"   // v3 transcribes without detecting/reporting a language
    }
    blk |> invoke(seg)
    return 1
}

def transcribe(m : AsrModel; var s : AsrSession; samples : array<float> | #;
               blk : block<(seg : TranscribeSegment)>) : int {
    //! Transcribe ``samples`` (16 kHz mono f32 PCM), one ``TranscribeSegment`` per trailing-block
    //! call. Returns the segment count. Whisper honors ``translate``/``no_timestamps``; qwen3a
    //! emits ONE segment (no timestamps) and fills ``detected_lang``.
    if (m.kind == AsrKind.gemma4a) {
        return transcribe_gemma4a(m, s, samples, blk)
    }
    if (m.kind == AsrKind.canary) {
        return transcribe_canary(m, s, samples, blk)
    }
    if (m.kind == AsrKind.qwen3omni) {
        return transcribe_qwen3omni(m, s, samples, blk)
    }
    if (m.kind == AsrKind.qwen3a) {
        return transcribe_qwen3a(m, s, samples, blk)
    }
    if (m.kind == AsrKind.parakeet) {
        return transcribe_parakeet(m, s, samples, blk)
    }
    s.w.translate = s.translate
    s.w.no_timestamps = s.no_timestamps
    let n = transcribe(m.whisper, s.w, samples, blk)
    if (s.w.lang_id >= 0) {
        s.detected_lang = whisper_lang_list()[s.w.lang_id]
    }
    return n
}

def transcribe(m : AsrModel; var s : AsrSession; samples : array<float> | #) : string {
    //! One-shot: transcribe ``samples`` and return the full text.
    return build_string() $(writer) {
        transcribe(m, s, samples) $(seg) {
            writer |> write(seg.text)
        }
    }
}

def feed(m : AsrModel; var s : AsrSession; samples : array<float> | #) {
    //! Push a chunk of 16 kHz mono PCM onto the session's stream (pair with drain/flush).
    //! Whisper only — qwen3a transcribes whole clips (caps().streaming is false there).
    if (m.kind != AsrKind.whisper) {
        panic("dasLLAMA asr: feed/drain/flush are whisper-only — this model transcribes whole clips")
    }
    feed(s.w, samples)
}

def drain(m : AsrModel; var s : AsrSession; blk : block<(seg : TranscribeSegment)>) : int {
    //! Transcribe every complete 30 s window of the fed stream; sub-window audio stays pending.
    if (m.kind != AsrKind.whisper) {
        panic("dasLLAMA asr: feed/drain/flush are whisper-only — this model transcribes whole clips")
    }
    s.w.translate = s.translate
    s.w.no_timestamps = s.no_timestamps
    return drain(m.whisper, s.w, blk)
}

def flush(m : AsrModel; var s : AsrSession; blk : block<(seg : TranscribeSegment)>) : int {
    //! Transcribe everything left on the fed stream (including the tail) and clear it.
    if (m.kind != AsrKind.whisper) {
        panic("dasLLAMA asr: feed/drain/flush are whisper-only — this model transcribes whole clips")
    }
    s.w.translate = s.translate
    s.w.no_timestamps = s.no_timestamps
    return flush(m.whisper, s.w, blk)
}
