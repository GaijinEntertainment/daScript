options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_asr shared public
options _function_length = 0   // the two sniffing loaders mirror the format zoo, arm by arm

require dasllama/dasllama_lint public
require math
require daslib/fio
require strings
require daslib/strings_boost
require dasllama/dasllama_gguf   // rd_u32 for the magic sniff
require dasllama/dasllama_image  // load_model_cached (decoders ride the .dlim rail) + image_peek
require dasllama/dasllama_plane public  // plane finalizers — re-exported so any module that deletes an ASR carrier sees them
require dasllama/dasllama_asr_types public  // AsrCaps/AsrTimestamps/TranscribeSegment — the shared ASR floor
require dasllama/dasllama_math  // guard_interp_inference — the library-seam -jit stop at session creation
require dasllama/dasllama_whisper public
require dasllama/dasllama_qwen3a public
require dasllama/dasllama_parakeet public
require dasllama/dasllama_canary public
require dasllama/dasllama_gemma4a public
require dasllama/dasllama_transformer   // nolint:STYLE029 — the qwen3a decoder is a full LLM; umbrella fires arch [init]s

// The uniform ASR surface: one loader that sniffs the file format, one verb set that
// dispatches — NO family names in the public API. Every verb arm is a one-call forward
// into the family's own module; family behavior (prompts, decode loops, caps) lives THERE.

enum private AsrKind {
    whisper
    qwen3a
    parakeet
    canary
    gemma4a
    qwen3omni
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

// Stated rather than inferred: the family carriers hold borrowed planes, and a generated
// teardown would force every module that deletes an AsrModel to see the plane finalizers too.
// Each field's own finalizer does the real work — this only names them in one place.
def finalize(var m : AsrModel) {
    delete m.whisper
    delete m.dec
    delete m.q3t
    delete m.pk
    delete m.cenc
    delete m.g4a
}

//! One transcription stream over an AsrModel: per-family scratch bundles plus shared options.
//! ``translate``/``no_timestamps`` apply where supported (see caps); ``context`` is qwen3a's
//! context-biasing text; ``detected_lang`` fills after (whisper: resolved code; qwen3a: model's own report).
struct AsrSession {
    w : WhisperSession = WhisperSession()
    q3s : Qwen3aAsrSession = Qwen3aAsrSession()   // qwen3a/qwen3omni: decoder session + tower scratch
    pkst : ParakeetState = ParakeetState()        // parakeet hot-path scratch — zero allocation per transcribe
    cn : CanaryAsrSession = CanaryAsrSession()    // canary: decoder session + encoder scratch
    g4 : Gemma4aAsrSession = Gemma4aAsrSession()  // gemma4a: decoder session + encoder scratch
    translate : bool = false
    no_timestamps : bool = false
    context : string = ""
    detected_lang : string = ""
}

// exact-fp32 knob for oracle rails: flip BEFORE load_asr_model to skip the Q8 fast path
var private g_asr_force_fp32 = false
var private g_asr_tower_fp32 = false   // the TOWER half only — the GPU-tower serving mode
// LLM-decoder ASR sessions' KV dtype. f16 is the DEFAULT (matches llama.cpp's KV — format-to-
// format comparisons; zen2 gb1 -3%, transcripts near-tie-sane); f32 is the oracle/parity rail
// (frozen-id cells pin it); q8_0 measured slower at ASR depths (decode overhead > bandwidth win)
var private g_asr_kv = KVDtype.f16

//! Select the KV-cache dtype for subsequent ASR decoder sessions (f16 default — matches the
//! reference's KV; f32 is the oracle/parity rail). LLM-decoder families only: whisper and
//! parakeet carry no LLM KV and ignore this. Applies at ``create_session``.
def set_asr_kv(dt : KVDtype) {
    g_asr_kv = dt
}

//! Force the fp32 (bit-exact-with-oracle) path for models loaded AFTER this call — the
//! default load quantizes the encoder GEMMs to Q8_0 (~4x on CPU, tolerance-gated).
def set_asr_fp32(on : bool) {
    g_asr_force_fp32 = on
}

//! Force fp32 on the ENCODER/TOWER half only (whisper enc, the parakeet bin, the canary
//! encoder) for models loaded AFTER this call — the GPU-tower serving mode: the decoder half
//! keeps its fast q8 rail. ``set_asr_fp32`` (both halves) wins; GGUF facade towers stay q8.
def set_asr_tower_fp32(on : bool) {
    g_asr_tower_fp32 = on
}

// a prepared image's family, from the baked identity's trailing tag ("" = no family tag —
// a decoder/tower image that needs the two-argument route)
def private image_family_tag(path : string) : string {
    let p = image_peek(path)
    if (!p.valid || empty(p.identity)) {
        panic("dasLLAMA asr: '{path}' is not a readable prepared image (see the log above)")
    }
    var parts <- split(p.identity, "|")
    let tag = parts[length(parts) - 1]
    delete parts
    return tag
}

//! Load an ASR model, sniffing the format: ggml-bin magic = whisper vs parakeet (disambiguated
//! on vocab size: whisper >= 51864, parakeet 1k/8k); GGUF needs the model's mmproj too — use the
//! two-argument overload. A ``.dlim`` prepared image routes on its baked family tag. One loader, no family names.
def load_asr_model(path : string) : AsrModel {
    if (path |> ends_with(".dlim")) {
        let tag = image_family_tag(path)
        if (tag == "whisper-q8" || tag == "whisper-f32" || tag == "whisper-f32-q8") {
            var wm = AsrModel(kind = AsrKind.whisper)
            wm.whisper <- load_whisper_model(path, tag == "whisper-q8", tag != "whisper-f32")
            return <- wm
        }
        if (tag == "parakeet-q8" || tag == "parakeet-f32") {
            var pm = AsrModel(kind = AsrKind.parakeet)
            pm.pk <- load_parakeet_model(path, tag == "parakeet-q8")
            return <- pm
        }
        panic("dasLLAMA asr: '{path}' is baked as '{tag}' - a decoder/tower image needs the pair: load_asr_model(decoder, mmproj)")
    }
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
        panic("dasLLAMA asr: '{path}' is a GGUF decoder - pass its mmproj too: load_asr_model(path, mmproj_path)")
    }
    if (magic != 0x67676d6cu) {
        panic("dasLLAMA asr: '{path}' is neither a GGUF nor a ggml bin")
    }
    if (vocab < 32768) {   // parakeet SPM vocabs: 1024 (v2, English) / 8192 (v3, 25-lang)
        var pm = AsrModel(kind = AsrKind.parakeet)
        pm.pk <- load_parakeet_model(path, !(g_asr_force_fp32 || g_asr_tower_fp32))
        return <- pm
    }
    // whisper vocabs: 51864 (.en) / 51865 (multilingual) / 51866 (large-v3 family); anything
    // else with the ggml magic (e.g. a legacy GPT-2 bin, 50257) fails HERE, not deep in the loader
    if (vocab < 51864 || vocab > 51999) {
        panic("dasLLAMA asr: '{path}' has n_vocab {vocab} - matches neither whisper (51864-51866) nor parakeet (~1k)")
    }
    var m = AsrModel(kind = AsrKind.whisper)
    m.whisper <- load_whisper_model(path, !(g_asr_force_fp32 || g_asr_tower_fp32), !g_asr_force_fp32)
    return <- m
}

def load_asr_model(path : string; mmproj : string) : AsrModel {
    //! The two-file route: ``path`` is the decoder GGUF, ``mmproj`` its audio front end — sniffed:
    //! ``qwen3a`` mmproj = Qwen3-ASR/Qwen3-Omni AuT tower, ``gemma4a`` = Gemma-4 Conformer, "CNRY"
    //! bin = Canary-Qwen FastConformer. Every decoder loads Q8 (the tuned rail); the fp32 parity arm rides ``QuantMode.fp32 + image_fp32`` off-facade. Either file may be a prepared ``.dlim`` (the mmproj routes on its baked family tag).
    var emagic = 0u
    var eproj = ""
    var canary_q8 = !(g_asr_force_fp32 || g_asr_tower_fp32)   // raw-bin default; a .dlim's baked tag overrides
    if (mmproj |> ends_with(".dlim")) {
        let tag = image_family_tag(mmproj)
        if (tag == "qwen3a-f32") {
            eproj = "qwen3a"
        } elif (tag == "gemma4a-f32") {
            eproj = "gemma4a"
        } elif (tag == "canary-q8") {
            emagic = CANARY_MAGIC
            canary_q8 = true
        } elif (tag == "canary-f32") {
            emagic = CANARY_MAGIC
            canary_q8 = false
        } else {
            panic("dasLLAMA asr: mmproj image '{mmproj}' is baked as '{tag}' - not an audio front-end image")
        }
    } else {
        let ef = fopen(mmproj, "rb")
        if (ef == null) {
            panic("dasLLAMA asr: cannot open mmproj '{mmproj}'")
        }
        // stat, not length(bytes): the fmap array's length() wraps int32 on >2GB files (the fp32
        // canary encoder is ~3GB), which would misroute the sniff
        if (stat(mmproj).size < 4ul) {
            panic("dasLLAMA asr: mmproj '{mmproj}' is too short")
        }
        fmap(ef) $(var bytes : array<uint8>#) {
            emagic = rd_u32(bytes, 0l)
            if (emagic == 0x46554747u) {   // "GGUF" — sniff the audio projector type
                var inscope mm <- parse_gguf_meta(bytes)
                if (gguf_has(mm, "clip.audio.projector_type")) {
                    eproj = gguf_str(mm, bytes, "clip.audio.projector_type")
                } elif (gguf_has(mm, "clip.projector_type")) {
                    eproj = gguf_str(mm, bytes, "clip.projector_type")
                }
            }
        }
        fclose(ef)
    }
    if (eproj == "gemma4a") {
        var m = AsrModel(kind = AsrKind.gemma4a)
        m.dec <- load_model_cached(path, QuantMode.q8, planar_only = true)
        if (m.dec.arch != "gemma4") {
            panic("dasLLAMA asr: '{path}' is arch '{m.dec.arch}' - the gemma4a audio route expects a Gemma-4 decoder")
        }
        m.dec.config.seq_len = min(m.dec.config.seq_len, 8192l)
        m.g4a <- load_gemma4a_encoder(mmproj)
        if (m.g4a.proj_dim != m.dec.config.dim) {
            panic("dasLLAMA asr: gemma4a encoder projects to {m.g4a.proj_dim} but the decoder is {m.dec.config.dim}-wide - mismatched pair")
        }
        return <- m
    }
    if (emagic == CANARY_MAGIC) {
        var m = AsrModel(kind = AsrKind.canary)
        m.dec <- load_model_cached(path, QuantMode.q8, planar_only = true)
        if (m.dec.arch != "qwen3") {
            panic("dasLLAMA asr: '{path}' is arch '{m.dec.arch}' - the Canary route expects a Qwen3-1.7B decoder")
        }
        m.dec.config.seq_len = min(m.dec.config.seq_len, 8192l)
        m.cenc <- load_canary_encoder(mmproj, canary_q8)
        if (m.cenc.proj_dim != m.dec.config.dim) {
            panic("dasLLAMA asr: canary encoder projects to {m.cenc.proj_dim} but the decoder is {m.dec.config.dim}-wide - mismatched pair")
        }
        return <- m
    }
    var m = AsrModel(kind = AsrKind.qwen3a)
    m.dec <- load_model_cached(path, QuantMode.q8, planar_only = true)
    // Same qwen3a AuT tower, two thinkers: Qwen3-ASR is a dense qwen3 (arch `qwen3`/`qwen3vl`);
    // Qwen3-Omni-30B is the MoE thinker (arch `qwen3vlmoe`), driven as a chat/transcription turn.
    if (m.dec.arch == "qwen3vlmoe") {
        m.kind = AsrKind.qwen3omni
    } elif (m.dec.arch != "qwen3vl" && m.dec.arch != "qwen3") {
        panic("dasLLAMA asr: '{path}' is arch '{m.dec.arch}' - the GGUF ASR route expects a Qwen3-ASR (qwen3/qwen3vl) or Qwen3-Omni (qwen3vlmoe) decoder")
    }
    m.dec.config.seq_len = min(m.dec.config.seq_len, 8192l)
    m.q3t <- load_qwen3a_tower(mmproj)
    if (m.q3t.proj_dim != m.dec.config.dim) {
        panic("dasLLAMA asr: mmproj projects to {m.q3t.proj_dim} but the decoder is {m.dec.config.dim}-wide - mismatched pair")
    }
    return <- m
}

def private quant_fmt(q : QuantMode) : string {
    if (q == QuantMode.q8) return "q8"
    if (q == QuantMode.q4_0) return "q4"
    return "f32"
}

def asr_exec_fmt(m : AsrModel) : string {
    //! The loaded precision as ``<encoder>/<decoder>`` (single tag for parakeet's one carrier) —
    //! the store's ``exec_fmt`` stamp, so every benchmark row names the bits it ran on.
    if (m.kind == AsrKind.whisper) return whisper_exec_fmt(m.whisper)
    if (m.kind == AsrKind.parakeet) return parakeet_exec_fmt(m.pk)
    if (m.kind == AsrKind.canary) return canary_exec_fmt(m.cenc, quant_fmt(m.dec.quant))
    if (m.kind == AsrKind.gemma4a) return gemma4a_exec_fmt(quant_fmt(m.dec.quant))
    return qwen3a_exec_fmt(quant_fmt(m.dec.quant))   // qwen3a / qwen3omni share the AuT tower
}

def asr_encode_bucket(m : AsrModel) : string {
    //! The family's encode-total ``asr_prof`` bucket — "" for families that only carry
    //! fine-grained buckets (their encode split stays a probe-side concern).
    if (m.kind == AsrKind.canary) return canary_encode_bucket()
    if (m.kind == AsrKind.whisper) return whisper_encode_bucket()
    if (m.kind == AsrKind.gemma4a) return gemma4a_encode_bucket()
    if (m.kind == AsrKind.qwen3a || m.kind == AsrKind.qwen3omni) {
        return qwen3a_encode_bucket(m.kind == AsrKind.qwen3omni)
    }
    return ""   // parakeet: fine-grained probes only
}

def caps(m : AsrModel) : AsrCaps {
    //! What this model honestly supports — each family declares its own sheet.
    if (m.kind == AsrKind.gemma4a) return <- gemma4a_asr_caps()
    if (m.kind == AsrKind.canary) return <- canary_asr_caps()
    if (m.kind == AsrKind.qwen3omni) return <- qwen3a_asr_caps(true)
    if (m.kind == AsrKind.qwen3a) return <- qwen3a_asr_caps(false)
    if (m.kind == AsrKind.parakeet) return <- parakeet_asr_caps(m.pk)
    return <- whisper_asr_caps(m.whisper)
}

def create_session(m : AsrModel; lang : string = "en") : AsrSession {
    //! Create a transcription session. ``lang`` is a language code from ``caps().languages``,
    //! or "auto" to detect (whisper multilingual models; Qwen3-ASR always detects — it only
    //! accepts "auto"). Validation lives with each family.
    guard_interp_inference("ASR session creation")   // whisper/parakeet never cross make_run_state
    var s = AsrSession()
    if (m.kind == AsrKind.parakeet) {
        parakeet_check_lang(m.pk, lang)   // no language conditioning — validate, then unused
    } elif (m.kind == AsrKind.canary) {
        s.cn <- create_canary_asr_session(m.dec, lang, g_asr_kv)
    } elif (m.kind == AsrKind.gemma4a) {
        s.g4 <- create_gemma4a_asr_session(m.dec, g_asr_kv)
    } elif (m.kind == AsrKind.qwen3omni) {
        s.q3s <- create_qwen3a_asr_session(m.dec, m.q3t, lang, true, g_asr_kv)
    } elif (m.kind == AsrKind.qwen3a) {
        s.q3s <- create_qwen3a_asr_session(m.dec, m.q3t, lang, false, g_asr_kv)
    } else {
        s.w <- create_session(m.whisper, lang)
    }
    return <- s
}

def transcribe(m : AsrModel; var s : AsrSession; samples : array<float> | #;
               blk : block<(seg : TranscribeSegment)>) : int {
    //! Transcribe ``samples`` (16 kHz mono f32 PCM), one ``TranscribeSegment`` per trailing-block
    //! call. Returns the segment count. Whisper honors ``translate``/``no_timestamps``; qwen3a
    //! emits ONE segment (no timestamps) and fills ``detected_lang``.
    if (m.kind == AsrKind.gemma4a) {
        return gemma4a_transcribe(m.dec, m.g4a, s.g4, samples, blk, s.detected_lang)
    }
    if (m.kind == AsrKind.canary) {
        return canary_transcribe(m.dec, m.cenc, s.cn, samples, blk, s.detected_lang)
    }
    if (m.kind == AsrKind.qwen3omni) {
        return qwen3omni_transcribe(m.dec, m.q3t, s.q3s, samples, blk)
    }
    if (m.kind == AsrKind.qwen3a) {
        return qwen3a_transcribe(m.dec, m.q3t, s.q3s, s.context, samples, blk, s.detected_lang)
    }
    if (m.kind == AsrKind.parakeet) {
        return parakeet_transcribe(m.pk, s.pkst, samples, blk, s.detected_lang)
    }
    return whisper_asr_transcribe(m.whisper, s.w, s.translate, s.no_timestamps, samples, blk, s.detected_lang)
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
        panic("dasLLAMA asr: feed/drain/flush are whisper-only - this model transcribes whole clips")
    }
    feed(s.w, samples)
}

def drain(m : AsrModel; var s : AsrSession; blk : block<(seg : TranscribeSegment)>) : int {
    //! Transcribe every complete 30 s window of the fed stream; sub-window audio stays pending.
    if (m.kind != AsrKind.whisper) {
        panic("dasLLAMA asr: feed/drain/flush are whisper-only - this model transcribes whole clips")
    }
    s.w.translate = s.translate
    s.w.no_timestamps = s.no_timestamps
    return drain(m.whisper, s.w, blk)
}

def flush(m : AsrModel; var s : AsrSession; blk : block<(seg : TranscribeSegment)>) : int {
    //! Transcribe everything left on the fed stream (including the tail) and clear it.
    if (m.kind != AsrKind.whisper) {
        panic("dasLLAMA asr: feed/drain/flush are whisper-only - this model transcribes whole clips")
    }
    s.w.translate = s.translate
    s.w.no_timestamps = s.no_timestamps
    return flush(m.whisper, s.w, blk)
}
