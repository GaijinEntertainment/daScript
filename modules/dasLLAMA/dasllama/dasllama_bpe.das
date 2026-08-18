options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_bpe shared public

require dasllama/dasllama_lint public
require daslib/fio
require dasllama/dasllama_gguf
require dasllama/dasllama_unicode
require dasllama/dasllama_pretok
require strings

//! Byte-level BPE tokenizer (Llama-3 / tiktoken family), loaded from a GGUF.
//! Distinct from the SentencePiece path in dasllama_spm: merges are *ranked*
//! (lowest rank merges first, not score-based), pieces live in the GPT-2
//! byte->unicode alphabet, and a pre-tokenizer (dasllama_pretok) splits text before BPE.
struct BpeTokenizer {
    vocab : array<string>             // id -> piece (byte-unicode alphabet)
    lookup : table<string; int64>     // piece -> id
    merge_rank : table<string; int64> // "left right" -> rank (lower merges first; the image serialization source)
    // DERIVED (build_bpe_derived), never serialized: the no-allocation merge engine — packed
    // (left<<32)|right -> (rank<<32)|merged — the per-byte alphabet-piece ids, and (spm_space)
    // codepoint -> single-codepoint-piece id for the gemma4 init loop
    pair_merge : table<int64; int64>
    byte_id : array<int64>
    cpt_id : table<int; int64>
    token_type : array<int64>         // id -> GGUF token type (3 = control/special)
    pre : string                      // pre-tokenizer name, e.g. "llama-bpe"
    bos_id : int64
    eos_id : int64
    // SPM-style BPE (Gemma-4): raw-UTF-8 merges (no GPT-2 byte alphabet), '▁' space escape,
    // newline-run pre-split. No field default — zero-init `var tk : BpeTokenizer` must stay legal.
    spm_space : bool
    // whether encode(add_special) prepends BOS — the GGUF add_bos_token key, else a per-pre default
    // mirroring llama.cpp (gpt-4o adds none). Same no-field-default rule as spm_space.
    add_bos : bool
}

//! Load the BPE vocab / merges / token-types from a llama-arch GGUF's tokenizer metadata.
def load_bpe_tokenizer_gguf(path : string) : BpeTokenizer {
    var tk : BpeTokenizer
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        var inscope toks <- gguf_str_array(m, bytes, "tokenizer.ggml.tokens")
        var inscope merges <- gguf_str_array(m, bytes, "tokenizer.ggml.merges")
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
        // gemma4 GGUFs carry no tokenizer.ggml.pre — the model name IS the pre (llama.cpp does the same)
        let tok_model = gguf_has(m, "tokenizer.ggml.model") ? gguf_str(m, bytes, "tokenizer.ggml.model") : ""
        tk.pre = gguf_has(m, "tokenizer.ggml.pre") ? gguf_str(m, bytes, "tokenizer.ggml.pre") : tok_model
        tk.spm_space = tok_model == "gemma4"
        // Qwen2.5-Omni GGUFs omit bos_token_id entirely (qwen2 never adds BOS)
        tk.bos_id = gguf_has(m, "tokenizer.ggml.bos_token_id") ? gguf_int(m, bytes, "tokenizer.ggml.bos_token_id") : -1l
        tk.eos_id = gguf_int(m, bytes, "tokenizer.ggml.eos_token_id")
        // the metadata key wins; absent, mirror llama.cpp's per-pre defaults (gpt-4o adds no BOS,
        // the llama-bpe family keeps it)
        tk.add_bos = (gguf_has(m, "tokenizer.ggml.add_bos_token")
            ? gguf_int(m, bytes, "tokenizer.ggml.add_bos_token") != 0l
            : tk.pre != "gpt-4o")
        if (tk.pre == "gpt-4o") {
            // llama.cpp force-renders these gpt-oss control tokens (attr -> USER_DEFINED) so channel
            // markers stay visible in decoded text — mirror it (type 4 escapes the type-3 mute rule)
            for (name in ["<|channel|>", "<|message|>", "<|start|>", "<|constrain|>"]) {
                let id = tk.lookup?[name] ?? -1l
                if (id >= 0l) {
                    tk.token_type[id] = 4l
                }
            }
        }
    }
    fclose(f)
    build_bpe_derived(tk)
    return <- tk
}

//! Build the DERIVED merge engine from the primal fields (vocab/lookup/merge_rank) — called by
//! the GGUF loader and the image reader, never serialized. A pair probe replaces the
//! concat-and-hash rank lookup byte-identically.
def build_bpe_derived(var tk : BpeTokenizer) {
    tk.byte_id |> resize(256)
    for (b in range(256)) {
        tk.byte_id[b] = tk.lookup?[BYTE2UNI[b]] ?? -1l
    }
    tk.cpt_id |> clear()
    if (tk.spm_space) {
        // gemma4 init probes per codepoint — int-keyed, so the hot loop allocates nothing
        for (m, piece in count(), tk.vocab) {
            var inscope pc <- utf8_to_cpts(piece)
            if (length(pc) == 1) {
                tk.cpt_id[pc[0]] = int64(m)
            }
        }
    }
    tk.pair_merge |> clear()
    for (k, v in keys(tk.merge_rank), values(tk.merge_rank)) {
        let sp = find(k, " ")
        continue if (sp < 0)
        let l = tk.lookup?[slice(k, 0, sp)] ?? -1l
        continue if (l < 0l)
        let r = tk.lookup?[slice(k, sp + 1)] ?? -1l
        let mid = tk.lookup?[slice(k, 0, sp) + slice(k, sp + 1)] ?? -1l
        if (r >= 0l && mid >= 0l) {
            tk.pair_merge[(l << 32l) | r] = (v << 32l) | mid
        }
    }
}

// ===== GPT-2 byte <-> unicode alphabet =====

// byte -> its visible-unicode piece (utf8). Printable bytes map to themselves;
// the rest map to codepoints 256+n (in byte order) so every byte is a printable token.
var private BYTE2UNI : array<string>
// reverse map: visible-unicode codepoint -> original byte (for decode)
var private UNI2BYTE : table<int; int>

[init]
def init_byte2uni() {
    BYTE2UNI |> resize(256)
    var n = 0
    for (ch in range(256)) {
        let printable = (ch >= 33 && ch <= 126) || (ch >= 161 && ch <= 172) || (ch >= 174 && ch <= 255)
        var cpt = ch
        if (!printable) {
            cpt = 256 + n
            n++
        }
        BYTE2UNI[ch] = cpt_to_utf8(cpt)
        UNI2BYTE |> insert(cpt, ch)
    }
}

// ===== merge-rank BPE =====

// past this many symbols the heap merge wins; below it the rescan's constant is smaller
let private BPE_HEAP_MIN_SYMS = 32

struct private BpeBigram {
    rank : int64
    li : int      // original left position — doubles as the leftmost tie-break (order is stable under merges)
    ri : int
    l_id : int64
    r_id : int64
    mid : int64
}

// min-heap priority: lowest rank, then leftmost — exactly the rescan's selection order.
// The sg_* heap in dasllama_spm is a deliberate twin: different key struct and comparator,
// and the two backends share no module to host a generic heap.
def private bg_before(a, b : BpeBigram) : bool => a.rank < b.rank || (a.rank == b.rank && a.li < b.li)

def private bg_push(var h : array<BpeBigram>; bg : BpeBigram) {
    h |> push(bg)
    var i = length(h) - 1
    while (i > 0) {
        let p = (i - 1) / 2
        break if (!bg_before(h[i], h[p]))
        let t = h[i]
        h[i] = h[p]
        h[p] = t
        i = p
    }
}

def private bg_pop(var h : array<BpeBigram>) : BpeBigram {
    let top = h[0]
    let last = length(h) - 1
    h[0] = h[last]
    h |> resize(last)
    var i = 0
    while (true) {
        var c = i * 2 + 1
        break if (c >= length(h))
        let r = c + 1
        if (r < length(h) && bg_before(h[r], h[c])) {
            c = r
        }
        break if (!bg_before(h[c], h[i]))
        let t = h[i]
        h[i] = h[c]
        h[c] = t
        i = c
    }
    return top
}

def private bg_try_push(tk : BpeTokenizer; var h : array<BpeBigram>; syms : array<int64>; li, ri : int) {
    let pm = tk.pair_merge?[(syms[li] << 32l) | syms[ri]] ?? -1l
    return if (pm < 0l)
    h |> bg_push(BpeBigram(rank = pm >> 32l, li = li, ri = ri,
        l_id = syms[li], r_id = syms[ri], mid = pm & ((1l << 32l) - 1l)))
}

// Heap merge for long runs: O(n log n) where the rescan is O(n^2). Stale heap entries (an
// endpoint merged since push) are skipped by the adjacency+id guard; ids match the rescan exactly.
def private bpe_merge_ids_heap(tk : BpeTokenizer; var syms : array<int64>) {
    let n = length(syms)
    var nxt : array<int>
    var prv : array<int>
    nxt |> resize(n)
    prv |> resize(n)
    for (i in range(n)) {
        nxt[i] = i + 1 < n ? i + 1 : -1
        prv[i] = i - 1
    }
    var h : array<BpeBigram>
    h |> reserve(n)
    for (i in range(n - 1)) {
        bg_try_push(tk, h, syms, i, i + 1)
    }
    while (!empty(h)) {
        let bg = bg_pop(h)
        continue if (nxt[bg.li] != bg.ri || syms[bg.li] != bg.l_id || syms[bg.ri] != bg.r_id)
        syms[bg.li] = bg.mid
        syms[bg.ri] = -2l   // dead slot — compacted out below
        nxt[bg.li] = nxt[bg.ri]
        if (nxt[bg.ri] >= 0) {
            prv[nxt[bg.ri]] = bg.li
        }
        if (prv[bg.li] >= 0) {
            bg_try_push(tk, h, syms, prv[bg.li], bg.li)
        }
        if (nxt[bg.li] >= 0) {
            bg_try_push(tk, h, syms, bg.li, nxt[bg.li])
        }
    }
    var w = 0
    for (i in range(n)) {
        if (syms[i] != -2l) {
            syms[w] = syms[i]
            w++
        }
    }
    syms |> resize(w)
    delete nxt
    delete prv
    delete h
}

// Greedily merge the lowest-rank adjacent pair (leftmost on ties), over symbol IDS — one packed
// pair_merge probe per candidate, no allocation (the concat-per-probe was the dominant cost).
def private bpe_merge_ids(tk : BpeTokenizer; var syms : array<int64>) {
    if (length(syms) > BPE_HEAP_MIN_SYMS) {
        bpe_merge_ids_heap(tk, syms)
        return
    }
    while (length(syms) > 1) {
        var best_rank = -1l
        var best_i = -1
        var best_id = -1l
        for (i in range(length(syms) - 1)) {
            let pm = tk.pair_merge?[(syms[i] << 32l) | syms[i + 1]] ?? -1l
            if (pm >= 0l) {
                let r = pm >> 32l
                if (best_i == -1 || r < best_rank) {
                    best_rank = r
                    best_i = i
                    best_id = pm & ((1l << 32l) - 1l)
                }
            }
        }
        if (best_i == -1) {
            break
        }
        syms[best_i] = best_id
        syms |> erase(best_i + 1)
    }
}

// ===== SPM-style BPE (Gemma-4) =====

var private BYTE_TOKEN_NAME : array<string>   // byte -> its "<0xXX>" fallback-token spelling

[init]
def init_byte_token_names() {
    BYTE_TOKEN_NAME |> resize(256)
    peek_data("0123456789ABCDEF") $(hx : array<uint8> const#) {
        for (b in range(256)) {
            BYTE_TOKEN_NAME[b] = string([uint8('<'), uint8('0'), uint8('x'), hx[b >> 4], hx[b & 15], uint8('>')])
        }
    }
}

// An unresolved symbol falls back to per-byte <0xXX> tokens — raw-UTF-8 BPE has no GPT-2
// byte alphabet to land on (llama.cpp llm_tokenizer_bpe_session, non-byte_encode branch).
def private emit_byte_fallback(tk : BpeTokenizer; s : string; var ids : array<int64>) {
    peek_data(s) $(d : array<uint8> const#) {
        for (k in range(length(d))) {
            let id = tk.lookup?[BYTE_TOKEN_NAME[int(d[k])]] ?? -1l
            ids |> push(id) if (id >= 0l)
        }
    }
}

// Gemma-4 encode: escape spaces to '▁', split newline-runs vs rest (llama4 "[^\n]+|[\n]+"),
// then ranked merges over raw UTF-8 pieces. An all-newline run that's itself a vocab token
// short-circuits the merge loop (llama.cpp gemma-4 newline fix).
def private bpe_encode_spm_space(tk : BpeTokenizer; text : string) : array<int64> {
    var ids : array<int64>
    // '▁' space escape at cpt level — ' ' and U+2581 are one cpt each (exact map); a whole-text string replace() is superlinear
    var cpts <- utf8_to_cpts(text)
    let n = length(cpts)
    for (i in range(n)) {
        if (cpts[i] == ' ') {
            cpts[i] = int(0x2581)   // U+2581, the SentencePiece '▁' space marker
        }
    }
    var pos = 0
    while (pos < n) {
        let is_nl = cpts[pos] == '\n'
        var e = pos
        while (e < n && (cpts[e] == '\n') == is_nl) {
            e++
        }
        var done = false
        if (is_nl) {
            let run = build_string() $(var wr) {
                for (i in range(pos, e)) {
                    write(wr, cpt_to_utf8(cpts[i]))
                }
            }
            let rid = tk.lookup?[run] ?? -1l
            if (rid >= 0l) {
                ids |> push(rid)
                done = true
            }
        }
        if (!done) {
            var syms : array<int64>
            syms |> reserve(e - pos)
            for (i in range(pos, e)) {
                let cid = tk.cpt_id?[cpts[i]] ?? -1l
                if (cid >= 0l) {
                    syms |> push(cid)
                } else {
                    // unknown cpt: eager <0xXX> fallback — byte tokens never merge, so pre-merge expansion is byte-identical
                    emit_byte_fallback(tk, cpt_to_utf8(cpts[i]), syms)
                }
            }
            bpe_merge_ids(tk, syms)
            ids |> push_from(syms)
            delete syms
        }
        pos = e
    }
    return <- ids
}

//! Encode text to BPE token IDs (no special tokens / BOS — matches test-tokenizer-0).
def bpe_encode(tk : BpeTokenizer; text : string) : array<int64> {
    if (tk.spm_space) {
        return <- bpe_encode_spm_space(tk, text)
    }
    var ids : array<int64>
    let cpts <- utf8_to_cpts(text)
    // gpt-4o and tekken have their own case-structured splits; qwen2/qwen35 split digits
    // one-at-a-time (\p{N}); llama3 / default groups up to three (\p{N}{1,3}); qwen35
    // additionally absorbs combining marks into letter runs
    var offs : array<int>
    if (tk.pre == "gpt-4o") {
        offs <- pretok_split_gpt4o(cpts)   // nolint:PERF030 - offs is fresh (declared empty just above); exactly one arm runs
    } elif (tk.pre == "gpt-2") {
        offs <- pretok_split_gpt2(cpts)   // nolint:PERF030 - offs is fresh (declared empty just above); exactly one arm runs
    } elif (tk.pre == "tekken") {
        offs <- pretok_split_tekken(cpts)   // nolint:PERF030 - offs is fresh (declared empty just above); exactly one arm runs
    } elif (tk.pre == "qwen35") {
        offs <- pretok_split(cpts, false, true)   // nolint:PERF030 - offs is fresh (declared empty just above); exactly one arm runs
    } else {
        offs <- pretok_split(cpts, tk.pre != "qwen2")   // nolint:PERF030 - offs is fresh (declared empty just above); exactly one arm runs
    }
    var pos = 0
    for (off in offs) {
        var syms : array<int64>
        var has_unk = false
        for (i in range(pos, pos + off)) {
            peek_data(cpt_to_utf8(cpts[i])) $(d : array<uint8> const#) {
                for (k in range(length(d))) {
                    let bid = tk.byte_id[int(d[k])]
                    syms |> push(bid)
                    if (bid < 0l) {
                        has_unk = true
                    }
                }
            }
        }
        pos += off
        // ignore_merges: a word that is already a single token is emitted as-is (the one
        // remaining per-word string build; a word with an unknown byte piece can't be one)
        var wid = -1l
        if (!has_unk) {
            let word = build_string() $(var wr) {
                for (s in syms) {
                    write(wr, tk.vocab[s])
                }
            }
            wid = tk.lookup?[word] ?? -1l
        }
        if (wid >= 0l) {
            ids |> push(wid)
        } else {
            bpe_merge_ids(tk, syms)
            for (s in syms) {
                ids |> push(s) if (s >= 0l)   // a negative survivor is an absent byte piece — dropped, as before
            }
        }
        delete syms
    }
    return <- ids
}

// ===== decode + special tokens =====

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

//! The byte value of a raw-byte token spelling like "<0x0A>", or -1 if not that shape.
//! (Shared with the SPM tokenizer — both backends use the SentencePiece byte-fallback vocabulary.)
def byte_token_val(piece : string) : int {
    var result = -1
    peek_data(piece) $(d : array<uint8> const#) {
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

//! Decode one token id back to its raw UTF-8 text. Control/special tokens (BOS, EOS,
//! <|eot_id|>, header markers) render as empty — callers detect them by id, not text.
def bpe_decode(tk : BpeTokenizer; id : int64) : string {
    // out of range, or a control token (BOS/EOS/<|eot_id|>/headers, GGUF type 3) — render nothing
    if (id < 0l || id >= long_length(tk.vocab) || tk.token_type[id] == 3l) {
        return ""
    }
    if (tk.spm_space) {
        // SPM-style pieces: byte tokens (type 6) -> the raw byte; user-defined markers (type 4)
        // verbatim; normal pieces unescape '▁' -> space (mirrors llama.cpp token_to_piece)
        if (tk.token_type[id] == 6l) {
            let bv = byte_token_val(tk.vocab[id])
            return bv >= 0 ? string([uint8(bv)]) : ""
        }
        return tk.token_type[id] == 4l ? tk.vocab[id] : replace(tk.vocab[id], "▁", " ")   // nolint:STYLE039 (U+2581 is the SentencePiece space marker - vocabulary data, not prose)
    }
    let cpts <- utf8_to_cpts(tk.vocab[id])
    return build_string() $(var wr) {
        for (c in cpts) {
            let b = UNI2BYTE?[c] ?? -1
            write_char(wr, b) if (b >= 0)
        }
    }
}

//! Decode a token id sequence to text (concatenated bpe_decode).
def bpe_decode_ids(tk : BpeTokenizer; ids : array<int64>) : string {
    return build_string() $(var wr) {
        for (id in ids) {
            write(wr, bpe_decode(tk, id))
        }
    }
}

//! Look up a special/added token id by its surface form (e.g. "<|eot_id|>"); -1 if absent.
def special_id(tk : BpeTokenizer; name : string) : int64 {
    return tk.lookup?[name] ?? -1l
}
