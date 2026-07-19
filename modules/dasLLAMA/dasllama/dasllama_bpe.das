options gen2
options _comment_hygiene = true

module dasllama_bpe shared public

require daslib/fio
require dasllama/dasllama_gguf
require dasllama/dasllama_unicode
require strings

//! Byte-level BPE tokenizer (Llama-3 / tiktoken family), loaded from a GGUF.
//! Distinct from the SentencePiece path in dasllama_tokenizer: merges are *ranked*
//! (lowest rank merges first, not score-based), pieces live in the GPT-2
//! byte->unicode alphabet, and a unicode-aware pre-tokenizer splits text before BPE.
struct BpeTokenizer {
    vocab : array<string>             // id -> piece (byte-unicode alphabet)
    lookup : table<string; int64>     // piece -> id
    merge_rank : table<string; int64> // "left right" -> rank (lower merges first)
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
    return <- tk
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

// ===== BPE pre-tokenizer (port of llama.cpp unicode_regex_split_custom_llama3 / _qwen2 / _qwen35) =====
// llama3 chunks digits \p{N}{1,3}, qwen2 emits one digit per token — `group_digits` selects between
// them; qwen35 = qwen2 digits plus marks absorbed into letter runs ([\p{L}\p{M}]+).

def private tolower_ascii(c : int) : int {
    return (c >= 'A' && c <= 'Z') ? c + 32 : c
}

def private uL(cpts : array<int>; pos : int) : bool {
    return pos >= 0 && pos < length(cpts) && cpt_is_letter(cpts[pos])
}
// letter-or-mark: the qwen35 letter class [\p{L}\p{M}] when marks, plain \p{L} otherwise
def private is_lm(cpt : int; marks : bool) : bool {
    return cpt_is_letter(cpt) || (marks && cpt_is_mark(cpt))
}
def private uLM(cpts : array<int>; pos : int; marks : bool) : bool {
    return pos >= 0 && pos < length(cpts) && is_lm(cpts[pos], marks)
}
def private uN(cpts : array<int>; pos : int) : bool {
    return pos >= 0 && pos < length(cpts) && cpt_is_number(cpts[pos])
}
def private uW(cpts : array<int>; pos : int) : bool {
    return pos >= 0 && pos < length(cpts) && cpt_is_whitespace(cpts[pos])
}
def private uC(cpts : array<int>; pos : int) : int {
    return (pos >= 0 && pos < length(cpts)) ? cpts[pos] : -1
}

def private pretok_split(cpts : array<int>; group_digits : bool; marks : bool = false) : array<int> {
    var offs : array<int>
    let n = length(cpts)
    var prev_end = 0
    var pos = 0
    while (pos < n) {
        let cpt = cpts[pos]

        // (?i:'s|'t|'re|'ve|'m|'ll|'d)
        if (cpt == 39 && pos + 1 < n) {
            let c1 = tolower_ascii(cpts[pos + 1])
            if (c1 == 's' || c1 == 't' || c1 == 'm' || c1 == 'd') {
                pos += 2
                offs |> push(pos - prev_end); prev_end = pos
                continue
            }
            if (pos + 2 < n) {
                let c2 = tolower_ascii(cpts[pos + 2])
                if ((c1 == 'r' && c2 == 'e') || (c1 == 'v' && c2 == 'e') || (c1 == 'l' && c2 == 'l')) {
                    pos += 3
                    offs |> push(pos - prev_end); prev_end = pos
                    continue
                }
            }
        }

        // [^\r\n\p{L}\p{N}]?\p{L}+ (marks widens run class to [\p{L}\p{M}]+)
        if (!(cpt == 13 || cpt == 10 || cpt_is_number(cpt))) {
            if (is_lm(cpt, marks) || uLM(cpts, pos + 1, marks)) {
                pos++
                while (uLM(cpts, pos, marks)) {
                    pos++
                }
                offs |> push(pos - prev_end); prev_end = pos
                continue
            }
        }

        // \p{N}{1,3} (llama3, chunks of up to three) | \p{N} (qwen2, one digit per token)
        if (cpt_is_number(cpt)) {
            if (group_digits) {
                var ini = pos
                while (uN(cpts, pos)) {
                    pos++
                    if (pos - ini >= 3) {
                        offs |> push(pos - prev_end); prev_end = pos
                        ini = pos
                    }
                }
            } else {
                pos++
            }
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // <space>?[^\s\p{L}\p{N}]+[\r\n]* (marks also excludes \p{M} from the run class)
        let probe = (cpt == 32) ? pos + 1 : pos
        if (!(uW(cpts, probe) || uLM(cpts, probe, marks) || uN(cpts, probe))) {
            if (cpt == 32) {
                pos++
            }
            while (!(uW(cpts, pos) || uLM(cpts, pos, marks) || uN(cpts, pos)) && pos < n) {
                pos++
            }
            var c2 = uC(cpts, pos)
            while (c2 == 13 || c2 == 10) {
                pos++
                c2 = uC(cpts, pos)
            }
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // remaining whitespace rules: \s*[\r\n]+ | \s+(?!\S) | \s+
        var num_ws = 0
        var last_rn = 0
        while (uW(cpts, pos + num_ws)) {
            let c2 = uC(cpts, pos + num_ws)
            if (c2 == 13 || c2 == 10) {
                last_rn = pos + num_ws + 1
            }
            num_ws++
        }
        if (last_rn > 0) {
            pos = last_rn
        } elif (num_ws > 1 && uC(cpts, pos + num_ws) != -1) {
            pos += num_ws - 1
        } elif (num_ws > 0) {
            pos += num_ws
        } else {
            pos++  // no match — consume one codepoint
        }
        offs |> push(pos - prev_end); prev_end = pos
    }
    return <- offs
}

// llama.cpp GPT4O letter-case classes: "uppercase" = \p{L} minus ASCII a-z, "lowercase" = \p{L}
// minus ASCII A-Z (its regex spells (?=[\p{L}])[^a-z] / (?=[\p{L}])[^A-Z]), so non-cased scripts
// (CJK, ...) land in BOTH classes. We match llama.cpp — the oracle — not tiktoken's true \p{Lu}/\p{Ll}.
def private uUp(cpts : array<int>; pos : int) : bool {
    if (pos < 0 || pos >= length(cpts)) {
        return false
    }
    let c = cpts[pos]
    return cpt_is_letter(c) && !(c >= 'a' && c <= 'z')
}
def private uLo(cpts : array<int>; pos : int) : bool {
    if (pos < 0 || pos >= length(cpts)) {
        return false
    }
    let c = cpts[pos]
    return cpt_is_letter(c) && !(c >= 'A' && c <= 'Z')
}

// gpt-4o / o200k pre-split (llama.cpp LLAMA_VOCAB_PRE_TYPE_GPT4O — gpt-oss). Differs from llama3:
// contractions suffix letter runs (no standalone arm), letter runs are case-structured
// (upper* lower+ | upper+ lower*), and punctuation runs may also trail '/'.
def private pretok_split_gpt4o(cpts : array<int>) : array<int> {
    var offs : array<int>
    let n = length(cpts)
    var prev_end = 0
    var pos = 0
    while (pos < n) {
        let cpt = cpts[pos]

        // [^\r\n\p{L}\p{N}]?(upper* lower+ | upper+ lower*) then optional contraction suffix
        if (!(cpt == 13 || cpt == 10 || cpt_is_number(cpt))) {
            if (cpt_is_letter(cpt) || uL(cpts, pos + 1)) {
                if (!cpt_is_letter(cpt)) {
                    pos++   // the optional one-char non-letter prefix
                }
                while (uUp(cpts, pos)) {
                    pos++
                }
                while (uLo(cpts, pos)) {
                    pos++
                }
                if (uC(cpts, pos) == 39 && pos + 1 < n) {   // contraction suffix, case-insensitive
                    let c1 = tolower_ascii(cpts[pos + 1])
                    if (c1 == 's' || c1 == 't' || c1 == 'm' || c1 == 'd') {
                        pos += 2
                    } elif (pos + 2 < n) {
                        let c2 = tolower_ascii(cpts[pos + 2])
                        if ((c1 == 'r' && c2 == 'e') || (c1 == 'v' && c2 == 'e') || (c1 == 'l' && c2 == 'l')) {
                            pos += 3
                        }
                    }
                }
                offs |> push(pos - prev_end); prev_end = pos
                continue
            }
        }

        // \p{N}{1,3} (chunks of up to three, same as llama3)
        if (cpt_is_number(cpt)) {
            var ini = pos
            while (uN(cpts, pos)) {
                pos++
                if (pos - ini >= 3) {
                    offs |> push(pos - prev_end); prev_end = pos
                    ini = pos
                }
            }
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // <space>?[^\s\p{L}\p{N}]+[\r\n/]*  (the '/' in the trailing run is gpt-4o's addition)
        let probe = (cpt == 32) ? pos + 1 : pos
        if (!(uW(cpts, probe) || uL(cpts, probe) || uN(cpts, probe))) {
            if (cpt == 32) {
                pos++
            }
            while (!(uW(cpts, pos) || uL(cpts, pos) || uN(cpts, pos)) && pos < n) {
                pos++
            }
            var c2 = uC(cpts, pos)
            while (c2 == 13 || c2 == 10 || c2 == 47) {
                pos++
                c2 = uC(cpts, pos)
            }
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // remaining whitespace rules: \s*[\r\n]+ | \s+(?!\S) | \s+ (same as llama3)
        var num_ws = 0
        var last_rn = 0
        while (uW(cpts, pos + num_ws)) {
            let c2 = uC(cpts, pos + num_ws)
            if (c2 == 13 || c2 == 10) {
                last_rn = pos + num_ws + 1
            }
            num_ws++
        }
        if (last_rn > 0) {
            pos = last_rn
        } elif (num_ws > 1 && uC(cpts, pos + num_ws) != -1) {
            pos += num_ws - 1
        } elif (num_ws > 0) {
            pos += num_ws
        } else {
            pos++  // no match — consume one codepoint
        }
        offs |> push(pos - prev_end); prev_end = pos
    }
    return <- offs
}

// tekken pre-split (Mistral/Voxtral). llama.cpp has no custom splitter — it runs generic std::wregex
// TRUE ECMAScript alternation with greedy backtracking (up*lo+ tried first), so "你A" splits "你"|"A"
// unlike gpt-4o's merged-run reduction. No contraction arm; single-digit \p{N}; '/' in punct tail.
def private pretok_split_tekken(cpts : array<int>) : array<int> {
    var offs : array<int>
    let n = length(cpts)
    var prev_end = 0
    var pos = 0
    while (pos < n) {
        let cpt = cpts[pos]

        // [^\r\n\p{L}\p{N}]?(up* lo+ | up+ lo*)
        if (!(cpt == 13 || cpt == 10 || cpt_is_number(cpt))) {
            if (cpt_is_letter(cpt) || uL(cpts, pos + 1)) {
                var q = pos
                if (!cpt_is_letter(cpt)) {
                    q++   // the optional one-char non-letter prefix
                }
                var up = 0
                while (uUp(cpts, q + up)) {
                    up++
                }
                // alt1 up* lo+: greedy up-run backtracks until a lo char can start lo+
                var split = up
                while (split >= 0 && !uLo(cpts, q + split)) {
                    split--
                }
                var e = q
                if (split >= 0) {
                    e = q + split
                    while (uLo(cpts, e)) {
                        e++
                    }
                } else {   // alt2 up+ lo*: up >= 1 guaranteed (first letter failed uLo => it's up)
                    e = q + up
                    while (uLo(cpts, e)) {
                        e++
                    }
                }
                pos = e
                offs |> push(pos - prev_end); prev_end = pos
                continue
            }
        }

        // \p{N} — one digit per token
        if (cpt_is_number(cpt)) {
            pos++
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // <space>?[^\s\p{L}\p{N}]+[\r\n/]*
        let probe = (cpt == 32) ? pos + 1 : pos
        if (!(uW(cpts, probe) || uL(cpts, probe) || uN(cpts, probe))) {
            if (cpt == 32) {
                pos++
            }
            while (!(uW(cpts, pos) || uL(cpts, pos) || uN(cpts, pos)) && pos < n) {
                pos++
            }
            var c2 = uC(cpts, pos)
            while (c2 == 13 || c2 == 10 || c2 == 47) {
                pos++
                c2 = uC(cpts, pos)
            }
            offs |> push(pos - prev_end); prev_end = pos
            continue
        }

        // remaining whitespace rules: \s*[\r\n]+ | \s+(?!\S) | \s+ (same as llama3)
        var num_ws = 0
        var last_rn = 0
        while (uW(cpts, pos + num_ws)) {
            let c2 = uC(cpts, pos + num_ws)
            if (c2 == 13 || c2 == 10) {
                last_rn = pos + num_ws + 1
            }
            num_ws++
        }
        if (last_rn > 0) {
            pos = last_rn
        } elif (num_ws > 1 && uC(cpts, pos + num_ws) != -1) {
            pos += num_ws - 1
        } elif (num_ws > 0) {
            pos += num_ws
        } else {
            pos++  // no match — consume one codepoint
        }
        offs |> push(pos - prev_end); prev_end = pos
    }
    return <- offs
}

// ===== merge-rank BPE =====

// Greedily merge the lowest-rank adjacent pair (leftmost on ties), in byte-unicode space.
def private bpe_merge(tk : BpeTokenizer; var symbols : array<string>) {
    while (length(symbols) > 1) {
        var best_rank = -1l
        var best_i = -1
        for (i in range(length(symbols) - 1)) {
            let r = tk.merge_rank?[symbols[i] + " " + symbols[i + 1]] ?? -1l
            if (r >= 0l && (best_i == -1 || r < best_rank)) {
                best_rank = r
                best_i = i
            }
        }
        if (best_i == -1) {
            break
        }
        symbols[best_i] = symbols[best_i] + symbols[best_i + 1]
        symbols |> erase(best_i + 1)
    }
}

// Defensive fallback: a symbol that didn't resolve as a token is split back into its
// base byte-unicode chars (each is always in the vocab). Rarely fires for valid merges.
def private emit_chars(tk : BpeTokenizer; s : string; var ids : array<int64>) {
    let scpts <- utf8_to_cpts(s)
    for (c in scpts) {
        let id = tk.lookup?[cpt_to_utf8(c)] ?? -1l
        if (id >= 0l) {
            ids |> push(id)
        }
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
    let cpts <- utf8_to_cpts(replace(text, " ", "▁"))
    let n = length(cpts)
    var pos = 0
    while (pos < n) {
        let is_nl = cpts[pos] == '\n'
        var e = pos
        while (e < n && (cpts[e] == '\n') == is_nl) {
            e++
        }
        var symbols <- [for (i in range(pos, e)); cpt_to_utf8(cpts[i])]
        pos = e
        var done = false
        if (is_nl) {
            let run = build_string() $(var wr) {
                for (s in symbols) {
                    write(wr, s)
                }
            }
            let rid = tk.lookup?[run] ?? -1l
            if (rid >= 0l) {
                ids |> push(rid)
                done = true
            }
        }
        if (!done) {
            bpe_merge(tk, symbols)
            for (s in symbols) {
                let id = tk.lookup?[s] ?? -1l
                if (id >= 0l) {
                    ids |> push(id)
                } else {
                    emit_byte_fallback(tk, s, ids)
                }
            }
        }
        delete symbols
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
        offs <- pretok_split_gpt4o(cpts)
    } elif (tk.pre == "tekken") {
        offs <- pretok_split_tekken(cpts)
    } elif (tk.pre == "qwen35") {
        offs <- pretok_split(cpts, false, true)
    } else {
        offs <- pretok_split(cpts, tk.pre != "qwen2")
    }
    var pos = 0
    for (off in offs) {
        var symbols : array<string>
        for (i in range(pos, pos + off)) {
            peek_data(cpt_to_utf8(cpts[i])) $(d : array<uint8> const#) {
                for (k in range(length(d))) {
                    symbols |> push(BYTE2UNI[int(d[k])])
                }
            }
        }
        pos += off
        // ignore_merges: a word that is already a single token is emitted as-is
        let word = build_string() $(var wr) {
            for (s in symbols) {
                write(wr, s)
            }
        }
        let wid = tk.lookup?[word] ?? -1l
        if (wid >= 0l) {
            ids |> push(wid)
        } else {
            bpe_merge(tk, symbols)
            for (s in symbols) {
                let id = tk.lookup?[s] ?? -1l
                if (id >= 0l) {
                    ids |> push(id)
                } else {
                    emit_chars(tk, s, ids)
                }
            }
        }
        delete symbols
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
    if (id < 0l || id >= int64(length(tk.vocab)) || tk.token_type[id] == 3l) {
        return ""
    }
    if (tk.spm_space) {
        // SPM-style pieces: byte tokens (type 6) -> the raw byte; user-defined markers (type 4)
        // verbatim; normal pieces unescape '▁' -> space (mirrors llama.cpp token_to_piece)
        if (tk.token_type[id] == 6l) {
            let bv = byte_token_val(tk.vocab[id])
            return bv >= 0 ? string([uint8(bv)]) : ""
        }
        return tk.token_type[id] == 4l ? tk.vocab[id] : replace(tk.vocab[id], "▁", " ")
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
