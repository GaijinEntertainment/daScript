options gen2
options _comment_hygiene = true

module dasllama_tokenizer shared public

require daslib/fio
require daslib/array_boost // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs
require dasllama/dasllama_gguf
require dasllama/dasllama_bpe
require strings

// llama2.c SentencePiece-BPE tokenizer. tokenizer.bin layout:
//   int32 max_token_length, then per token { float32 score, int32 len, len bytes of text }.

//! Vocabulary loaded from tokenizer.bin: id -> piece text, plus merge scores (for encode).
struct SpmTokenizer {
    vocab : array<string>
    scores : array<float>
    lookup : table<string; int64>  // piece text -> id, for encode
    max_token_length : int
    add_space_prefix : bool = true // SPM add_dummy_prefix: prepend a ▁ to non-empty text (Gemma sets it false)
    bos_id : int64 = 1l            // BOS token id from GGUF metadata (Llama-2/TinyLlama 1, Gemma 2); default = llama2.c
    eos_id : int64 = 2l            // EOS token id from GGUF metadata (Llama-2/TinyLlama 2, Gemma 1)
}

//! Load tokenizer.bin. vocab_size comes from the model Config (it isn't stored in the file).
def load_tokenizer(path : string; vocab_size : int64) : SpmTokenizer {
    var tk = SpmTokenizer()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open tokenizer '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        unsafe {
            tk.max_token_length = temp_array(addr<int?>(bytes[0]), 1, type<int>)[0]
            var cur = 4
            for (i in range64(vocab_size)) {
                let score = temp_array(addr<float?>(bytes[cur]), 1, type<float>)[0]
                cur += 4
                let len = temp_array(addr<int?>(bytes[cur]), 1, type<int>)[0]
                cur += 4
                let piece = string(temp_array(addr<uint8?>(bytes[cur]), len, type<uint8>))
                tk.scores |> push(score)
                tk.lookup |> insert(piece, i)
                tk.vocab |> push(piece)
                cur += len
            }
        }
    }
    fclose(f)
    return <- tk
}

//! Load a SentencePiece tokenizer from a llama-arch GGUF (vocab/scores live in metadata).
//! GGUF stores raw SPM pieces with the '▁' (U+2581) space metasymbol; substituted to space at load
//! so encode/decode work unchanged. BOS=1 / EOS=2 (Llama-2 SPM) hardcoded to match.
def load_tokenizer_gguf(path : string) : SpmTokenizer {
    var tk = SpmTokenizer()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let m <- parse_gguf_meta(bytes)
        let toks <- gguf_str_array(m, bytes, "tokenizer.ggml.tokens")
        let scores <- gguf_f32_array(m, bytes, "tokenizer.ggml.scores")
        let n = length(toks)
        tk.vocab |> reserve(n)
        tk.scores |> reserve(n)
        for (i in range(n)) {
            let piece = replace(toks[i], "▁", " ")
            tk.vocab |> push(piece)
            tk.scores |> push(scores[i])
            tk.lookup |> insert(piece, int64(i))
        }
        tk.max_token_length = 0
        if (gguf_has(m, "tokenizer.ggml.add_space_prefix")) {
            tk.add_space_prefix = gguf_int(m, bytes, "tokenizer.ggml.add_space_prefix") != 0l
        }
        if (gguf_has(m, "tokenizer.ggml.bos_token_id")) {
            tk.bos_id = gguf_int(m, bytes, "tokenizer.ggml.bos_token_id")
        }
        if (gguf_has(m, "tokenizer.ggml.eos_token_id")) {
            tk.eos_id = gguf_int(m, bytes, "tokenizer.ggml.eos_token_id")
        }
    }
    fclose(f)
    return <- tk
}

//! Decode one token to its display text (llama2.c decode): strip a single leading space
//! right after BOS, and turn raw-byte tokens "<0xXX>" into the actual byte (byte_token_val
//! is shared with the BPE backend — same SentencePiece byte-fallback vocabulary).
def decode(tk : SpmTokenizer; prev_token, token : int64) : string {
    let piece = tk.vocab[token]
    let bv = byte_token_val(piece)
    if (bv >= 0) {
        return string([uint8(bv)])
    }
    // sentencepiece strips a single leading space directly after BOS
    if (prev_token == tk.bos_id && !empty(piece) && first_character(piece) == ' ') {
        return slice(piece, 1)
    }
    return piece
}

//! Encode text into token IDs (SentencePiece-BPE, ported from llama2.c encode):
//! optional BOS, a dummy leading-space token, per-codepoint vocab lookup with byte
//! fallback, then greedily merge the highest-scoring adjacent pair until none remain.
def encode(tk : SpmTokenizer; text : string; bos, eos : bool) : array<int64> {
    var tokens : array<int64>
    if (bos) {
        tokens |> push(tk.bos_id)
    }
    // sentencepiece add_dummy_prefix: a leading space token when text is non-empty (Gemma disables it)
    if (tk.add_space_prefix && !empty(text)) {
        tokens |> push(tk.lookup?[" "] ?? -1l)
    }
    // split into UTF-8 codepoints; each -> its vocab token, or byte fallback (byte + 3)
    peek_data(text) $(d : array<uint8> const#) {
        var i = 0
        while (i < length(d)) {
            var j = i + 1
            while (j < length(d) && (int(d[j]) & int(0xC0)) == int(0x80) && j - i < 4) {
                j++
            }
            let id = tk.lookup?[slice(text, i, j)] ?? -1l
            if (id >= 0l) {
                tokens |> push(id)
            } else {
                for (b in range(i, j)) {
                    tokens |> push(int64(int(d[b]) + 3))
                }
            }
            i = j
        }
    }
    // greedy BPE merge: repeatedly fuse the best-scoring adjacent pair present in vocab
    while (true) {
        var best_score = -1.0e10
        var best_id = -1l
        var best_idx = -1
        for (i in range(length(tokens) - 1)) {
            let merged = tk.vocab[tokens[i]] + tk.vocab[tokens[i + 1]]
            let id = tk.lookup?[merged] ?? -1l
            if (id >= 0l && tk.scores[id] > best_score) {
                best_score = tk.scores[id]
                best_id = id
                best_idx = i
            }
        }
        if (best_idx == -1) {
            break
        }
        tokens[best_idx] = best_id
        tokens |> erase(best_idx + 1)
    }
    if (eos) {
        tokens |> push(tk.eos_id)
    }
    return <- tokens
}

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
        let m <- parse_gguf_meta(bytes)
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

//! Resolve a special/control token id by its surface form (e.g. "<end_of_turn>"); -1 if absent.
//! SPM special tokens live in the vocab, so this is a plain lookup — same contract as the BPE overload.
def special_id(tk : SpmTokenizer; name : string) : int64 {
    return tk.lookup?[name] ?? -1l
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
