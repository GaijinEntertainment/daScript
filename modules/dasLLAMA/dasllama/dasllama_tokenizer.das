options gen2
options _comment_hygiene = true

module dasllama_tokenizer shared public

require daslib/fio
require dasllama/dasllama_gguf
require dasllama/dasllama_spm public
require dasllama/dasllama_bpe public
require strings

// ===== Unified tokenizer facade (SentencePiece + byte-level BPE behind one type) =====

enum TokKind {
    unset
    spm
    bpe
}

//! A tokenizer over either backend. A model owns one; `load_tokenizer_auto` picks the backend
//! from the GGUF's tokenizer.ggml.model ("gpt2" -> BPE, else SentencePiece).
struct Tokenizer {
    kind : TokKind = TokKind.unset
    spm : SpmTokenizer = SpmTokenizer()
    bpe : BpeTokenizer = BpeTokenizer()
}

//! Load the tokenizer from a GGUF, auto-selecting the backend: "gpt2" -> byte-level BPE (Llama-3,
//! Qwen2), "gemma4" -> SPM-style BPE (raw-UTF-8 ranked merges over '▁'-escaped text), anything
//! else -> SentencePiece (Llama-2, Phi-3, Gemma).
def load_tokenizer_auto(path : string) : Tokenizer {
    var tk = Tokenizer()
    let ts_sniff = ref_time_ticks()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open gguf '{path}'")
    }
    var model_kind = ""
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        if (gguf_has(m, "tokenizer.ggml.model")) {
            model_kind = gguf_str(m, bytes, "tokenizer.ggml.model")
        }
    }
    fclose(f)
    let sniff_ms = get_time_usec(ts_sniff) / 1000
    let ts_build = ref_time_ticks()
    if (model_kind == "gpt2" || model_kind == "gemma4") {
        tk.kind = TokKind.bpe
        tk.bpe <- load_bpe_tokenizer_gguf(path)
    } else {
        tk.kind = TokKind.spm
        tk.spm <- load_tokenizer_gguf(path)
    }
    to_log(LOG_INFO, "dasLLAMA: tokenizer sniff {sniff_ms} ms, build {get_time_usec(ts_build) / 1000} ms\n")
    return <- tk
}

//! Encode text to token ids. `add_special` prepends the model's BOS (SPM also adds its dummy-space
//! prefix). `parse_special` (interpret control-token spellings in the text) is reserved for the chat
//! layer and not yet honored.
def encode(tk : Tokenizer; text : string; add_special = true; parse_special = false) : array<int64> {
    if (tk.kind == TokKind.bpe) {
        if (!add_special || !tk.bpe.add_bos || tk.bpe.bos_id < 0l) {
            return <- bpe_encode(tk.bpe, text)
        }
        var ids : array<int64>
        ids |> push(tk.bpe.bos_id)
        ids |> push_from(bpe_encode(tk.bpe, text))
        return <- ids
    }
    return <- encode(tk.spm, text, add_special, false)
}

//! Decode a full id sequence back to text.
def decode(tk : Tokenizer; ids : array<int64>) : string {
    if (tk.kind == TokKind.bpe) {
        return bpe_decode_ids(tk.bpe, ids)
    }
    return build_string() $(var wr) {
        var prev = tk.spm.bos_id   // BOS as the initial prev, so a leading space right after BOS is stripped (llama2.c)
        for (id in ids) {
            write(wr, decode(tk.spm, prev, id))
            prev = id
        }
    }
}

//! Decode a single token to text — for streaming. Stateless (SPM keeps the piece's own leading space).
def piece(tk : Tokenizer; id : int64) : string {
    if (tk.kind == TokKind.bpe) {
        return bpe_decode(tk.bpe, id)
    }
    return decode(tk.spm, 0l, id)
}

//! Resolve a special-token id through the facade (SPM vocab lookup / BPE added-token lookup).
def special_id(tk : Tokenizer; name : string) : int64 {
    if (tk.kind == TokKind.bpe) {
        return special_id(tk.bpe, name)
    }
    return special_id(tk.spm, name)
}

//! The tokenizer's BOS id (SPM per-model metadata / BPE bos_token_id) — for the chat-template add_bos.
def bos_id(tk : Tokenizer) : int64 {
    if (tk.kind == TokKind.bpe) {
        return tk.bpe.bos_id
    }
    return tk.spm.bos_id
}
