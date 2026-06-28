options gen2

module dasllama_tokenizer shared public

require daslib/fio
require daslib/array_boost // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs
require strings

// llama2.c SentencePiece-BPE tokenizer. tokenizer.bin layout:
//   int32 max_token_length, then per token { float32 score, int32 len, len bytes of text }.

//! Vocabulary loaded from tokenizer.bin: id -> piece text, plus merge scores (for encode).
struct Tokenizer {
    vocab : array<string>
    scores : array<float>
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
            for (_i in range64(vocab_size)) {
                let score = temp_array(reinterpret<float?>(addr(bytes[cur])), 1, type<float>)[0]
                cur += 4
                let len = temp_array(reinterpret<int?>(addr(bytes[cur])), 1, type<int>)[0]
                cur += 4
                let piece = string(temp_array(reinterpret<uint8?>(addr(bytes[cur])), len, type<uint8>))
                tk.scores |> push(score)
                tk.vocab |> push(piece)
                cur += len
            }
        }
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
