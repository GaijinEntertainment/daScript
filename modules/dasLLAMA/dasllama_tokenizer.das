options gen2

module dasllama_tokenizer shared public

require daslib/fio
require daslib/array_boost // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs
require dasllama_gguf
require strings

// llama2.c SentencePiece-BPE tokenizer. tokenizer.bin layout:
//   int32 max_token_length, then per token { float32 score, int32 len, len bytes of text }.

//! Vocabulary loaded from tokenizer.bin: id -> piece text, plus merge scores (for encode).
struct Tokenizer {
    vocab : array<string>
    scores : array<float>
    lookup : table<string; int64>  // piece text -> id, for encode
    max_token_length : int
}

//! Load tokenizer.bin. vocab_size comes from the model Config (it isn't stored in the file).
def load_tokenizer(path : string; vocab_size : int64) : Tokenizer {
    var tk : Tokenizer
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open tokenizer '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        unsafe {
            tk.max_token_length = temp_array(reinterpret<int?>(addr(bytes[0])), 1, type<int>)[0]
            var cur = 4
            for (i in range64(vocab_size)) {
                let score = temp_array(reinterpret<float?>(addr(bytes[cur])), 1, type<float>)[0]
                cur += 4
                let len = temp_array(reinterpret<int?>(addr(bytes[cur])), 1, type<int>)[0]
                cur += 4
                let piece = string(temp_array(reinterpret<uint8?>(addr(bytes[cur])), len, type<uint8>))
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
//! GGUF stores raw SPM pieces with the '▁' (U+2581) space metasymbol; we substitute
//! '▁' -> space at load so encode/decode below (written for llama2.c's literal-space
//! convention) work unchanged. BOS=1 / EOS=2 (Llama-2 SPM) match the hardcoded ids there.
def load_tokenizer_gguf(path : string) : Tokenizer {
    var tk : Tokenizer
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
    }
    fclose(f)
    return <- tk
}

def private hex_val(c : int) : int {
    if (is_number(c)) {
        return c - '0'
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10
    }
    return -1
}

// A raw-byte token looks like "<0x0A>"; returns its byte value, or -1 if not that shape.
def private parse_byte_token(s : string) : int {
    var result = -1
    peek_data(s) $(d : array<uint8> const#) {
        if (length(d) == 6 && int(d[0]) == '<' && int(d[1]) == '0' && int(d[2]) == 'x' && int(d[5]) == '>') {
            let hi = hex_val(int(d[3]))
            let lo = hex_val(int(d[4]))
            if (hi >= 0 && lo >= 0) {
                result = hi * 16 + lo
            }
        }
    }
    return result
}

//! Decode one token to its display text (llama2.c decode): strip a single leading space
//! right after BOS, and turn raw-byte tokens "<0xXX>" into the actual byte.
def decode(tk : Tokenizer; prev_token, token : int64) : string {
    let piece = tk.vocab[token]
    let bv = parse_byte_token(piece)
    if (bv >= 0) {
        return string([uint8(bv)])
    }
    // sentencepiece strips a single leading space directly after BOS
    if (prev_token == 1l && !empty(piece) && first_character(piece) == ' ') {
        return slice(piece, 1)
    }
    return piece
}

//! Encode text into token IDs (SentencePiece-BPE, ported from llama2.c encode):
//! optional BOS, a dummy leading-space token, per-codepoint vocab lookup with byte
//! fallback, then greedily merge the highest-scoring adjacent pair until none remain.
def encode(tk : Tokenizer; text : string; bos, eos : bool) : array<int64> {
    var tokens : array<int64>
    if (bos) {
        tokens |> push(1l)
    }
    // sentencepiece add_dummy_prefix: a leading space token when text is non-empty
    if (!empty(text)) {
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
        tokens |> push(2l)
    }
    return <- tokens
}
