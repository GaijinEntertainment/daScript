options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_spm shared public

require dasllama/dasllama_lint public
require daslib/fio
require daslib/array_boost // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs
require dasllama/dasllama_gguf
require dasllama/dasllama_bpe
require strings
require math

// SentencePiece-BPE tokenizer over the llama2.c tokenizer.bin format:
//   int32 max_token_length, then per token { float32 score, int32 len, len bytes of text }.

//! Vocabulary loaded from tokenizer.bin: id -> piece text, plus merge scores (for encode).
struct SpmTokenizer {
    vocab : array<string>
    scores : array<float>
    lookup : table<string; int64>  // piece text -> id, for encode
    // DERIVED (build_spm_derived), never serialized: packed (left<<32)|right -> merged id for
    // every vocab piece that splits into two vocab pieces — the no-allocation merge probe
    pair_merge : table<int64; int64>
    // DERIVED: a short (1-4 byte) vocab piece's raw bytes packed little-endian into a uint -> id.
    // The encode init loop's no-allocation probe: byte-packed keys equal string equality exactly
    // (per-cpt slice + string-keyed lookup strlens the whole source per call — O(n^2) loops)
    bkey_id : table<uint; int64>
    max_token_length : int
    add_space_prefix : bool = true // SPM add_dummy_prefix: prepend a ▁ to non-empty text (Gemma sets it false)
    bos_id : int64 = 1l            // BOS token id from GGUF metadata (Llama-2/TinyLlama 1, Gemma 2); default = llama2.c
    eos_id : int64 = 2l            // EOS token id from GGUF metadata (Llama-2/TinyLlama 2, Gemma 1)
}

//! Build the DERIVED merge-pair table from vocab/lookup — called by both loaders and the image
//! reader. A pair probe replaces the concat-and-hash byte-identically: pack(left, right)
//! maps to exactly the id the concat-string lookup would find (last duplicate wins in both).
def build_spm_derived(var tk : SpmTokenizer) {
    tk.pair_merge |> clear()
    tk.bkey_id |> clear()
    for (m, piece in count(), tk.vocab) {
        for (k in range(1, length(piece))) {
            let l = tk.lookup?[slice(piece, 0, k)] ?? -1l
            continue if (l < 0l)
            let r = tk.lookup?[slice(piece, k)] ?? -1l
            if (r >= 0l) {
                tk.pair_merge[(l << 32l) | r] = int64(m)
            }
        }
        if (!empty(piece) && length(piece) <= 4) {
            tk.bkey_id[pack_piece_key(piece)] = int64(m)   // last duplicate wins, same as lookup
        }
    }
}

// the 1-4 raw bytes of a short piece packed little-endian; strings carry no NUL, so
// zero-padding the high bytes cannot collide across lengths
def private pack_piece_key(piece : string) : uint {
    var k = 0u
    peek_data(piece) $(d : array<uint8> const#) {
        for (b in range(length(d))) {
            k |= uint(d[b]) << uint(b * 8)
        }
    }
    return k
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
    build_spm_derived(tk)
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
        var inscope m <- parse_gguf_meta(bytes)
        var inscope toks <- gguf_str_array(m, bytes, "tokenizer.ggml.tokens")
        var inscope scores <- gguf_f32_array(m, bytes, "tokenizer.ggml.scores")
        let n = length(toks)
        tk.vocab |> reserve(n)
        tk.scores |> reserve(n)
        for (i in range(n)) {
            let piece = replace(toks[i], "▁", " ")   // nolint:STYLE039 (U+2581 is the SentencePiece space marker - vocabulary data, not prose)
            tk.vocab |> push(piece)
            tk.scores |> push(scores[i])
            tk.lookup |> insert(piece, int64(i))
        }
        tk.max_token_length = 0
        for (piece in tk.vocab) {
            tk.max_token_length = max(tk.max_token_length, length(piece))   // bytes: the unigram lattice's longest probe
        }
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
    build_spm_derived(tk)
    return <- tk
}

struct private SpmBigram {
    score : float
    li : int      // original left position — doubles as the leftmost tie-break (order is stable under merges)
    ri : int
    l_id : int64
    r_id : int64
    mid : int64
}

// heap priority: highest score, then leftmost — exactly the sequential scan's selection order.
// The bg_* heap in dasllama_bpe is a deliberate twin: different key struct and comparator,
// and the two backends share no module to host a generic heap.
def private sg_before(a, b : SpmBigram) : bool => a.score > b.score || (a.score == b.score && a.li < b.li)

def private sg_push(var h : array<SpmBigram>; bg : SpmBigram) {
    h |> push(bg)
    var i = length(h) - 1
    while (i > 0) {
        let p = (i - 1) / 2
        break if (!sg_before(h[i], h[p]))
        let t = h[i]
        h[i] = h[p]
        h[p] = t
        i = p
    }
}

def private sg_pop(var h : array<SpmBigram>) : SpmBigram {
    let top = h[0]
    let last = length(h) - 1
    h[0] = h[last]
    h |> resize(last)
    var i = 0
    while (true) {
        var c = i * 2 + 1
        break if (c >= length(h))
        let r = c + 1
        if (r < length(h) && sg_before(h[r], h[c])) {
            c = r
        }
        break if (!sg_before(h[c], h[i]))
        let t = h[i]
        h[i] = h[c]
        h[c] = t
        i = c
    }
    return top
}

def private sg_try_push(tk : SpmTokenizer; var h : array<SpmBigram>; toks : array<int64>; li, ri : int) {
    let mid = tk.pair_merge?[(toks[li] << 32l) | toks[ri]] ?? -1l
    return if (mid < 0l)
    h |> sg_push(SpmBigram(score = tk.scores[mid], li = li, ri = ri,
        l_id = toks[li], r_id = toks[ri], mid = mid))
}

// Heap merge over the WHOLE token sequence: O(n log n) where the sequential rescan is O(n^2) in
// text length. Stale entries (an endpoint merged since push) are skipped by the adjacency+id
// guard; selection order matches the scan exactly, so ids are identical.
def private spm_merge_heap(tk : SpmTokenizer; var toks : array<int64>) {
    let n = length(toks)
    return if (n < 2)
    var nxt : array<int>
    var prv : array<int>
    nxt |> resize(n)
    prv |> resize(n)
    for (i in range(n)) {
        nxt[i] = i + 1 < n ? i + 1 : -1
        prv[i] = i - 1
    }
    var h : array<SpmBigram>
    h |> reserve(n)
    for (i in range(n - 1)) {
        sg_try_push(tk, h, toks, i, i + 1)
    }
    while (!empty(h)) {
        let bg = sg_pop(h)
        continue if (nxt[bg.li] != bg.ri || toks[bg.li] != bg.l_id || toks[bg.ri] != bg.r_id)
        toks[bg.li] = bg.mid
        toks[bg.ri] = -2l   // dead slot — compacted out below (-1, the missing-piece marker, survives as before)
        nxt[bg.li] = nxt[bg.ri]
        if (nxt[bg.ri] >= 0) {
            prv[nxt[bg.ri]] = bg.li
        }
        if (prv[bg.li] >= 0) {
            sg_try_push(tk, h, toks, prv[bg.li], bg.li)
        }
        if (nxt[bg.li] >= 0) {
            sg_try_push(tk, h, toks, bg.li, nxt[bg.li])
        }
    }
    var w = 0
    for (i in range(n)) {
        if (toks[i] != -2l) {
            toks[w] = toks[i]
            w++
        }
    }
    toks |> resize(w)
    delete nxt
    delete prv
    delete h
}

//! Decode one token to its display text: strip a single leading space
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

//! Encode text into token IDs (SentencePiece-BPE):
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
    // split into UTF-8 codepoints; each window's raw bytes pack into the uint bkey_id probe —
    // byte-equality semantics, zero allocation (a slice + string-keyed lookup here strlens the
    // whole source per call, O(n^2) over the text) — or byte fallback (byte + 3)
    peek_data(text) $(d : array<uint8> const#) {
        var i = 0
        while (i < length(d)) {
            var j = i + 1
            var k = uint(d[i])
            while (j < length(d) && (int(d[j]) & int(0xC0)) == int(0x80) && j - i < 4) {
                k |= uint(d[j]) << uint((j - i) * 8)
                j++
            }
            let id = tk.bkey_id?[k] ?? -1l
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
    // greedy BPE merge: repeatedly fuse the best-scoring adjacent pair present in vocab —
    // heap-driven, one packed pair_merge probe per candidate
    spm_merge_heap(tk, tokens)
    if (eos) {
        tokens |> push(tk.eos_id)
    }
    return <- tokens
}

//! Resolve a special/control token id by its surface form (e.g. "<end_of_turn>"); -1 if absent.
//! SPM special tokens live in the vocab, so this is a plain lookup — same contract as the BPE overload.
def special_id(tk : SpmTokenizer; name : string) : int64 {
    return tk.lookup?[name] ?? -1l
}

// ===== the unigram arm: a SentencePiece model of type UNIGRAM (GGUF `tokenizer.ggml.model =
// "t5"`) over the same vocabulary and scores - a Viterbi lattice instead of the merge heap =====

let private UNIGRAM_UNK_PENALTY = 10.0   // sentencepiece's kUnkPenalty: an unknown character scores the floor less this

//! The unknown character's score in a unigram lattice: the vocabulary's lowest piece score less
//! the penalty (the byte and control pieces score zero and never set the floor).
def unigram_unk_score(tk : SpmTokenizer) : float {
    var lo = 0.0
    for (s in tk.scores) {
        lo = min(lo, s)
    }
    return lo - UNIGRAM_UNK_PENALTY
}

def private spm_piece_of(d : array<uint8> const#; i, n : int) : string {
    unsafe {
        return string(temp_array(addr<uint8?>(d[i]), n, type<uint8>))
    }
}

def private hex_char(v : int) : int => v < 10 ? '0' + v : 'A' + v - 10

// the byte piece's name, "<0xXX>", built without a string walk
def private byte_piece_name(bv : int) : string {
    return string([uint8('<'), uint8('0'), uint8('x'), uint8(hex_char(bv >> 4)), uint8(hex_char(bv & 15)), uint8('>')])
}

// the next codepoint boundary after byte position i
def private cpt_end(d : array<uint8> const#; i, n : int) : int {
    var j = i + 1
    while (j < n && (int(d[j]) & 0xC0) == 0x80) {
        j++
    }
    return j
}

// the Viterbi lattice over byte positions: best[j] the highest score reaching j, back_len /
// back_id the piece that got there; ties go to the earliest candidate (the reference's strict >)
def private unigram_lattice(tk : SpmTokenizer; d : array<uint8> const#; unk_id : int64;
                            var best : array<float>; var back_len : array<int>; var back_id : array<int64>) {
    let n = length(d)
    let unk_score = unigram_unk_score(tk)
    let max_len = max(tk.max_token_length, 1)
    best |> resize(n + 1)
    back_len |> resize(n + 1)
    back_id |> resize(n + 1)
    for (i in range(1, n + 1)) {
        best[i] = -3.0e38
    }
    best[0] = 0.0
    var i = 0
    while (i < n) {
        let ni = cpt_end(d, i, n)
        var has_single = false
        var j = ni
        while (j <= n && j - i <= max_len) {
            let id = tk.lookup?[spm_piece_of(d, i, j - i)] ?? -1l
            if (id >= 0l && byte_token_val(tk.vocab[id]) < 0) {   // a byte piece never matches text
                has_single = has_single || j == ni
                let sc = best[i] + tk.scores[id]
                if (sc > best[j]) {
                    best[j] = sc
                    back_len[j] = j - i
                    back_id[j] = id
                }
            }
            j = j < n ? cpt_end(d, j, n) : n + 1
        }
        if (!has_single) {
            let sc = best[i] + unk_score
            if (sc > best[ni]) {
                best[ni] = sc
                back_len[ni] = ni - i
                back_id[ni] = unk_id
            }
        }
        i = ni
    }
}

//! Encode text as a UNIGRAM SentencePiece model does: the dummy-space prefix, the segmentation of
//! highest total piece score over the codepoint lattice (an unspellable codepoint is the unknown
//! piece at the penalty score), and with `byte_fallback` each unknown as its bytes. No BOS or EOS.
def unigram_encode(tk : SpmTokenizer; text : string; unk_id : int64; byte_fallback : bool) : array<int64> {
    var inscope ids : array<int64>
    return <- ids if (empty(text))
    let input = tk.add_space_prefix ? " {text}" : text
    peek_data(input) $(d : array<uint8> const#) {
        var inscope best : array<float>
        var inscope back_len : array<int>
        var inscope back_id : array<int64>
        unigram_lattice(tk, d, unk_id, best, back_len, back_id)
        // the pieces come off the lattice last first
        var inscope rev : array<int64>
        var inscope rev_at : array<int>
        var inscope rev_len : array<int>
        var p = length(d)
        while (p > 0) {
            rev |> push(back_id[p])
            rev_at |> push(p - back_len[p])
            rev_len |> push(back_len[p])
            p -= back_len[p]
        }
        var k = length(rev) - 1
        while (k >= 0) {
            if (rev[k] == unk_id && byte_fallback) {
                for (b in range(rev_at[k], rev_at[k] + rev_len[k])) {
                    ids |> push(tk.lookup?[byte_piece_name(int(d[b]))] ?? unk_id)
                }
            } else {
                ids |> push(rev[k])
            }
            k--
        }
    }
    return <- ids
}

//! The text a SentencePiece id sequence spells: pieces joined (a byte piece as its byte), the
//! dummy-space prefix dropped - the reference's `decode`, control ids rendered as their names.
def spm_decode_ids(tk : SpmTokenizer; ids : array<int64>) : string {
    let s = build_string() $(var wr) {
        for (id in ids) {
            continue if (id < 0l || id >= long_length(tk.vocab))
            let piece = tk.vocab[id]
            let bv = byte_token_val(piece)
            if (bv >= 0) {
                write(wr, string([uint8(bv)]))
            } else {
                write(wr, piece)
            }
        }
    }
    return (tk.add_space_prefix && !empty(s) && first_character(s) == ' ') ? slice(s, 1) : s
}
