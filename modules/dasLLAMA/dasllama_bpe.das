options gen2

module dasllama_bpe shared public

require daslib/fio
require dasllama_gguf

//! Byte-level BPE tokenizer (Llama-3 / tiktoken family), loaded from a GGUF.
//! Distinct from the SentencePiece path in dasllama_tokenizer: merges are *ranked*
//! (lowest rank merges first, not score-based), pieces live in the GPT-2
//! byte->unicode alphabet, and a unicode-aware pre-tokenizer splits text before BPE.
struct BPETokenizer {
    vocab : array<string>             // id -> piece (byte-unicode alphabet)
    lookup : table<string; int64>     // piece -> id
    merge_rank : table<string; int64> // "left right" -> rank (lower merges first)
    token_type : array<int64>         // id -> GGUF token type (3 = control/special)
    pre : string                      // pre-tokenizer name, e.g. "llama-bpe"
    bos_id : int64
    eos_id : int64
}

//! Load the BPE vocab / merges / token-types from a llama-arch GGUF's tokenizer metadata.
def load_bpe_tokenizer_gguf(path : string) : BPETokenizer {
    var tk : BPETokenizer
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let m <- parse_gguf_meta(bytes)
        let toks <- gguf_str_array(m, bytes, "tokenizer.ggml.tokens")
        let merges <- gguf_str_array(m, bytes, "tokenizer.ggml.merges")
        let n = length(toks)
        tk.vocab |> reserve(n)
        for (i in range(n)) {
            tk.vocab |> push(toks[i])
            tk.lookup |> insert(toks[i], int64(i))
        }
        tk.token_type <- gguf_i32_array(m, bytes, "tokenizer.ggml.token_type")
        for (i in range(length(merges))) {
            tk.merge_rank |> insert(merges[i], int64(i))
        }
        tk.pre = gguf_str(m, bytes, "tokenizer.ggml.pre")
        tk.bos_id = gguf_int(m, bytes, "tokenizer.ggml.bos_token_id")
        tk.eos_id = gguf_int(m, bytes, "tokenizer.ggml.eos_token_id")
    }
    fclose(f)
    return <- tk
}
