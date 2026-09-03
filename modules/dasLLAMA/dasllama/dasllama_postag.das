options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_postag shared public

require dasllama/dasllama_lint public
require math
require strings
require daslib/fio
require daslib/utf8_utils
require dasllama/dasllama_unicode

//! Part-of-speech tagging for the TTS front end: an English tokenizer shaped like the
//! reference pipeline's over normalized text (contractions and abbreviations from the packed
//! exception table, prosodic punctuation peeled off, infix hyphens split) and a greedy
//! averaged-perceptron PTB tagger whose weights, tag set and exception table load from
//! tts_postag.bin (harness/train_postag.py). Tags drive the lexicon's part-of-speech entries
//! and the punctuation phonemes downstream.

//! One token of normalized text: its surface, its PTB tag once tagged, and whether whitespace
//! followed it in the source.
struct PosToken {
    text : string
    tag : string
    space_after : bool
}

//! The packed tagger: tag names, the unambiguous-word table, the feature table (sorted 64-bit
//! feature hashes with a run of (tag, weight) pairs each), and the tokenizer's exception table.
struct PosTagger {
    tags : array<string>
    scale : float
    test_accuracy : float
    dev_accuracy : float
    source : string
    tagdict : table<string; int>
    feat_hash : array<uint64>
    feat_off : array<int>
    feat_len : array<int>
    w_tag : array<int>
    w_val : array<int>
    exceptions : table<string; array<string>>
}

let private TAGGER_MAGIC = 0x53504F54u
let private START1 = "-START-"
let private START2 = "-START2-"
let private END1 = "-END-"
let private END2 = "-END2-"

// ---------------------------------------------------------------- the packed file

def private rd_u8(b : array<uint8> | #; o : int64) : int => int(b[o])
def private rd_u16(b : array<uint8> | #; o : int64) : int => int(b[o]) | (int(b[o + 1l]) << 8)
def private rd_u32(b : array<uint8> | #; o : int64) : uint => uint(b[o]) | (uint(b[o + 1l]) << 8u) | (uint(b[o + 2l]) << 16u) | (uint(b[o + 3l]) << 24u)
def private rd_i32(b : array<uint8> | #; o : int64) : int => int(rd_u32(b, o))
def private rd_f32(b : array<uint8> | #; o : int64) : float {
    let u = rd_u32(b, o)
    return unsafe(reinterpret<float>(u))
}
def private rd_str(b : array<uint8> | #; o : int64; n : int) : string {
    var inscope bytes : array<uint8>
    bytes |> resize(n)
    for (i in range(n)) {
        bytes[i] = b[o + int64(i)]
    }
    return string(bytes)
}

//! the header: magic, version, the three floats, the source note, the tag names and the
//! unambiguous-word table; returns the offset of the feature table
def private read_tagger_header(var tg : PosTagger; b : array<uint8>#; path : string) : int64 {
    var o = 0l
    if (rd_u32(b, o) != TAGGER_MAGIC) {
        panic("dasLLAMA tts: bad magic in '{path}' (not a tts_postag.bin)")
    }
    o += 4l
    let version = rd_i32(b, o)
    o += 4l
    if (version != 1) {
        panic("dasLLAMA tts: unsupported tts_postag.bin version {version}")
    }
    tg.test_accuracy = rd_f32(b, o)
    tg.dev_accuracy = rd_f32(b, o + 4l)
    tg.scale = rd_f32(b, o + 8l)
    o += 12l
    let src_len = rd_u16(b, o)
    o += 2l
    tg.source = rd_str(b, o, src_len)
    o += int64(src_len)
    let n_tags = rd_i32(b, o)
    o += 4l
    tg.tags |> reserve(n_tags)
    for (_i in range(n_tags)) {
        let l = rd_u8(b, o)
        o++
        tg.tags |> push(rd_str(b, o, l))
        o += int64(l)
    }
    let n_td = rd_i32(b, o)
    o += 4l
    for (_i in range(n_td)) {
        let l = rd_u16(b, o)
        o += 2l
        let w = rd_str(b, o, l)
        o += int64(l)
        tg.tagdict[w] = rd_u8(b, o)
        o++
    }
    return o
}

//! Load tts_postag.bin. Panics on a missing file, a wrong magic or an unsupported version.
def load_pos_tagger(path : string) : PosTagger {
    var tg = PosTagger()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA tts: cannot open tagger '{path}'")
    }
    fmap(f) $(var b : array<uint8>#) {
        var o = read_tagger_header(tg, b, path)
        let n_feat = rd_i32(b, o)
        o += 4l
        tg.feat_hash |> reserve(n_feat)
        tg.feat_hash |> resize(n_feat)
        tg.feat_off |> reserve(n_feat)
        tg.feat_off |> resize(n_feat)
        tg.feat_len |> reserve(n_feat)
        tg.feat_len |> resize(n_feat)
        if (n_feat > 0) {
            unsafe {
                memcpy(addr(tg.feat_hash[0]), addr(b[o]), n_feat * 8)
                o += int64(n_feat) * 8l
                memcpy(addr(tg.feat_off[0]), addr(b[o]), n_feat * 4)
                o += int64(n_feat) * 4l
            }
        }
        for (i in range(n_feat)) {
            tg.feat_len[i] = rd_u16(b, o)
            o += 2l
        }
        let n_w = rd_i32(b, o)
        o += 4l
        tg.w_tag |> reserve(n_w)
        tg.w_tag |> resize(n_w)
        tg.w_val |> reserve(n_w)
        tg.w_val |> resize(n_w)
        for (i in range(n_w)) {
            tg.w_tag[i] = rd_u8(b, o)
            o++
        }
        for (i in range(n_w)) {
            let v = rd_u16(b, o)
            tg.w_val[i] = v >= 32768 ? v - 65536 : v
            o += 2l
        }
        let n_exc = rd_i32(b, o)
        o += 4l
        for (_i in range(n_exc)) {
            let l = rd_u16(b, o)
            o += 2l
            let chunk = rd_str(b, o, l)
            o += int64(l)
            let n_parts = rd_u8(b, o)
            o++
            var parts : array<string>
            for (_p in range(n_parts)) {
                let pl = rd_u16(b, o)
                o += 2l
                parts |> push(rd_str(b, o, pl))
                o += int64(pl)
            }
            tg.exceptions[chunk] <- parts
        }
    }
    fclose(f)
    return <- tg
}

// ---------------------------------------------------------------- features

let private FNV_OFFSET = 0xcbf29ce484222325ul
let private FNV_PRIME = 0x100000001b3ul

def private fnv_str(h0 : uint64; s : string) : uint64 {
    var h = h0
    peek_data(s) $(d : array<uint8> const#) {
        for (b in d) {
            h ^= uint64(b)
            h *= FNV_PRIME
        }
    }
    return h
}

//! the field separator between a feature's name and its arguments
def private fnv_sep(h : uint64) : uint64 => (h ^ 31ul) * FNV_PRIME

def private feat0(name : string) : uint64 => fnv_sep(fnv_str(FNV_OFFSET, name))
def private feat1(name, a : string) : uint64 => fnv_str(fnv_sep(fnv_str(FNV_OFFSET, name)), a)
def private feat2(name, a, b : string) : uint64 => fnv_str(fnv_sep(fnv_str(fnv_sep(fnv_str(FNV_OFFSET, name)), a)), b)

def private is_digit(cp : int) : bool => is_number(cp)
def private is_letter(cp : int) : bool => cp >= 0 && cpt_is_letter(cp)
def private is_lower(cp : int) : bool => is_letter(cp) && int(utf32_to_lower(uint(cp))) == cp
def private is_upper(cp : int) : bool => is_letter(cp) && int(utf32_to_lower(uint(cp))) != cp
def private cp_at(c : array<int>; i : int) : int => (i >= 0 && i < length(c)) ? c[i] : -1

def private suffix_str(c : array<int>; n : int) : string => cpts_to_utf8(c, max(0, length(c) - n), length(c))
def private prefix_str(c : array<int>; n : int) : string => cpts_to_utf8(c, 0, min(n, length(c)))

//! capitalization and character-class silhouette: X caps, Xx title, x lower; d = has a digit,
//! h = inner hyphen, q = apostrophe
def private shape(c : array<int>) : string {
    var has_letter = false
    var has_lower = false
    var has_digit = false
    var has_quote = false
    var inner_hyphen = false
    for (i in range(length(c))) {
        let cp = c[i]
        has_letter = has_letter || is_letter(cp)
        has_lower = has_lower || is_lower(cp)
        has_digit = has_digit || is_digit(cp)
        has_quote = has_quote || cp == '\''
        inner_hyphen = inner_hyphen || (i > 0 && cp == '-')
    }
    var s = (has_letter && !has_lower) ? "X" : (is_upper(cp_at(c, 0)) ? "Xx" : "x")
    if (has_digit) {
        s = "{s}d"
    }
    if (inner_hyphen) {
        s = "{s}h"
    }
    if (has_quote) {
        s = "{s}q"
    }
    return s
}

//! the tagger's word class: hyphenated, four-digit year, digit-led, else lowercased. The
//! hyphen test is the trainer's (`harness/train_postag.py`) - a hyphen anywhere in a token
//! that does not START with one, so "--" and "-well-known" are ordinary words
def private norm_word(c : array<int>) : string {
    let n = length(c)
    var hyphen = false
    var all_digits = n > 0
    for (i in range(n)) {
        hyphen = hyphen || c[i] == '-'
        all_digits = all_digits && is_digit(c[i])
    }
    return "!HYPHEN" if (hyphen && cp_at(c, 0) != '-')
    return "!YEAR" if (all_digits && n == 4)
    return "!DIGITS" if (n > 0 && is_digit(c[0]))
    var inscope lowered <- [for (cp in c); int(utf32_to_lower(uint(cp)))]
    return cpts_to_utf8(lowered)
}

//! The tagger's word class for one token - what the perceptron's word features key on:
//! `!HYPHEN` for a hyphen in a token that does not start with one, `!YEAR` for a four-digit
//! run, `!DIGITS` for a digit-led token, else the lowercased word.
def pos_norm_word(word : string) : string {
    var inscope c <- utf8_to_cpts(word)
    return norm_word(c)
}

//! The tagger's shape feature for one token: `X` all caps, `Xx` title, `x` lower, then `d`
//! when it carries a digit, `h` an interior hyphen, `q` an apostrophe.
def pos_word_shape(word : string) : string {
    var inscope c <- utf8_to_cpts(word)
    return shape(c)
}

def private add_feature(tg : PosTagger; var scores : array<int>; h : uint64) {
    var lo = 0
    var hi = length(tg.feat_hash) - 1
    while (lo <= hi) {
        let mid = (lo + hi) >> 1
        let v = tg.feat_hash[mid]
        if (v == h) {
            let off = tg.feat_off[mid]
            for (k in range(off, off + tg.feat_len[mid])) {
                scores[tg.w_tag[k]] += tg.w_val[k]
            }
            return
        } elif (v < h) {
            lo = mid + 1
        } else {
            hi = mid - 1
        }
    }
}

//! Tag a token sequence: the unambiguous-word table first, else the perceptron over the
//! classic template widened with shape, more affixes and word bigrams; ties fall to the
//! lexicographically largest tag, as the trainer's own predictor does.
def pos_tag_words(tg : PosTagger; words : array<string>) : array<string> {
    let n = length(words)
    var inscope ctx : array<string>
    ctx |> reserve(n + 4)
    ctx |> push(START1)
    ctx |> push(START2)
    var inscope cps : array<array<int>>
    cps |> reserve(n)
    for (w in words) {
        var c <- utf8_to_cpts(w)
        ctx |> push(norm_word(c))
        cps |> emplace(c)
    }
    ctx |> push(END1)
    ctx |> push(END2)
    var tags : array<string>
    tags |> reserve(n)
    var inscope scores : array<int>
    scores |> reserve(length(tg.tags))
    scores |> resize(length(tg.tags))
    var prev = START1
    var prev2 = START2
    for (i in range(n)) {
        var tag_i = tg.tagdict?[words[i]] ?? -1
        if (tag_i < 0) {
            for (s in scores) {
                s = 0
            }
            assume c = cps[i]
            let k = i + 2
            let suf3 = suffix_str(c, 3)
            add_feature(tg, scores, feat0("bias"))
            add_feature(tg, scores, feat1("i suffix", suf3))
            add_feature(tg, scores, feat1("i suffix2", suffix_str(c, 2)))
            add_feature(tg, scores, feat1("i suffix4", suffix_str(c, 4)))
            add_feature(tg, scores, feat1("i pref1", prefix_str(c, 1)))
            add_feature(tg, scores, feat1("i pref2", prefix_str(c, 2)))
            add_feature(tg, scores, feat1("i pref3", prefix_str(c, 3)))
            add_feature(tg, scores, feat1("i shape", shape(c)))
            add_feature(tg, scores, feat1("i-1 tag", prev))
            add_feature(tg, scores, feat1("i-2 tag", prev2))
            add_feature(tg, scores, feat2("i tag+i-2 tag", prev, prev2))
            add_feature(tg, scores, feat1("i word", ctx[k]))
            add_feature(tg, scores, feat2("i-1 tag+i word", prev, ctx[k]))
            add_feature(tg, scores, feat2("i-1 tag+i suffix", prev, suf3))
            add_feature(tg, scores, feat1("i-1 word", ctx[k - 1]))
            add_feature(tg, scores, feat1("i-1 suffix", ctx_suffix(ctx[k - 1])))
            add_feature(tg, scores, feat2("i-1 word+i word", ctx[k - 1], ctx[k]))
            add_feature(tg, scores, feat1("i-2 word", ctx[k - 2]))
            add_feature(tg, scores, feat1("i+1 word", ctx[k + 1]))
            add_feature(tg, scores, feat1("i+1 suffix", ctx_suffix(ctx[k + 1])))
            add_feature(tg, scores, feat2("i word+i+1 word", ctx[k], ctx[k + 1]))
            add_feature(tg, scores, feat1("i+2 word", ctx[k + 2]))
            tag_i = 0
            for (t in range(1, length(scores))) {
                if (scores[t] >= scores[tag_i]) {
                    tag_i = t
                }
            }
        }
        let tag = tg.tags[tag_i]
        tags |> push(tag)
        prev2 = prev
        prev = tag
    }
    return <- tags
}

//! the last three characters of a context word (the pads are ASCII, real words may not be)
def private ctx_suffix(w : string) : string {
    var inscope c <- utf8_to_cpts(w)
    return suffix_str(c, 3)
}

// ---------------------------------------------------------------- tokenizer

def private is_space(cp : int) : bool => cp >= 0 && cpt_is_whitespace(cp)
def private is_hyphen(cp : int) : bool => cp == '-' || cp == 8211 || cp == 8212
def private is_ellipsis(cp : int) : bool => cp == 8230
//! punctuation a token starts with: dashes, ellipsis, clause marks, the quote
def private is_prefix_punct(cp : int) : bool => cp == 8212 || cp == 8211 || cp == 8230 || cp == ',' || cp == ':' || cp == ';' || cp == '!' || cp == '?' || cp == '\''
//! punctuation a token ends with (the period has its own rule)
def private is_suffix_punct(cp : int) : bool => is_prefix_punct(cp)
def private is_letter_or_digit(cp : int) : bool => is_letter(cp) || is_digit(cp)

//! the run of dots starting at `i`, when it is at least two long
def private dots_run(c : array<int>; i, e : int) : int {
    var j = i
    while (j < e && c[j] == '.') {
        j++
    }
    return j - i >= 2 ? j - i : 0
}

def private prefix_len(c : array<int>; s, e : int) : int {
    return 0 if (s >= e)
    let run = dots_run(c, s, e)
    return run if (run > 0)
    return is_prefix_punct(c[s]) ? 1 : 0
}

//! the period comes off after a digit, a lowercase letter, punctuation, or two capitals; a lone
//! capital keeps its period (an initial)
def private period_splits(c : array<int>; s, e : int) : bool {
    let p = cp_at(c, e - 2)
    return false if (e - 2 < s)
    return true if (is_digit(p) || is_lower(p) || !is_letter_or_digit(p))
    return is_upper(p) && e - 3 >= s && is_upper(cp_at(c, e - 3))
}

def private suffix_len(c : array<int>; s, e : int) : int {
    return 0 if (s >= e)
    let last = c[e - 1]
    if (last == '.') {
        var j = e - 1
        while (j > s && c[j - 1] == '.') {
            j--
        }
        return e - j if (e - j >= 2)
        return period_splits(c, s, e) ? 1 : 0
    }
    if ((last == 's' || last == 'S') && e - 2 >= s && c[e - 2] == '\'') {
        return 2
    }
    return is_suffix_punct(last) ? 1 : 0
}

def private push_token(var out : array<PosToken>; text : string) {
    out |> push(PosToken(text = text, tag = "", space_after = false))
}

def private push_exception(tg : PosTagger; var out : array<PosToken>; chunk : string) : bool {
    let parts = unsafe(tg.exceptions?[chunk])
    return false if (parts == null)
    for (p in *parts) {
        push_token(out, p)
    }
    return true
}

//! the infix token starting at `i` inside the core, or 0: an ellipsis, a dash run between a
//! letter or digit and a letter, a period between lower and upper case, a comma or colon
//! between letters
def private infix_len(c : array<int>; s, i, e : int) : int {
    let cp = c[i]
    return 1 if (is_ellipsis(cp))
    let run = dots_run(c, i, e)
    return run if (run > 0)
    let prev = cp_at(c, i - 1)
    let has_prev = i > s
    if (is_hyphen(cp)) {
        var j = i
        while (j < e && is_hyphen(c[j])) {
            j++
        }
        return (has_prev && is_letter_or_digit(prev) && j < e && is_letter(c[j])) ? j - i : 0
    }
    let next = cp_at(c, i + 1)
    return 0 if (!has_prev || i + 1 >= e)
    if (cp == '.') {
        return ((is_lower(prev) || prev == '\'') && (is_upper(next) || next == '\'')) ? 1 : 0
    }
    if (cp == ',') {
        return (is_letter(prev) && is_letter(next)) ? 1 : 0
    }
    if (cp == ':') {
        return (is_letter_or_digit(prev) && is_letter(next)) ? 1 : 0
    }
    return 0
}

def private emit_core(tg : PosTagger; var out : array<PosToken>; c : array<int>; s, e : int) {
    return if (s >= e || push_exception(tg, out, cpts_to_utf8(c, s, e)))
    var seg = s
    var i = s
    while (i < e) {
        let l = infix_len(c, s, i, e)
        if (l > 0) {
            if (i > seg) {
                push_token(out, cpts_to_utf8(c, seg, i))
            }
            push_token(out, cpts_to_utf8(c, i, i + l))
            i += l
            seg = i
            continue
        }
        i++
    }
    if (seg < e) {
        push_token(out, cpts_to_utf8(c, seg, e))
    }
}

def private tokenize_chunk(tg : PosTagger; var out : array<PosToken>; c : array<int>; a, b : int) {
    var s = a
    var e = b
    var inscope suffixes : array<string>
    while (s < e) {
        if (push_exception(tg, out, cpts_to_utf8(c, s, e))) {
            s = e
            break
        }
        let pl = prefix_len(c, s, e)
        if (pl > 0) {
            push_token(out, cpts_to_utf8(c, s, s + pl))
            s += pl
            continue
        }
        let sl = suffix_len(c, s, e)
        if (sl > 0) {
            suffixes |> push(cpts_to_utf8(c, e - sl, e))
            e -= sl
            continue
        }
        break
    }
    emit_core(tg, out, c, s, e)
    var k = length(suffixes) - 1
    while (k >= 0) {
        push_token(out, suffixes[k])
        k--
    }
}

//! Tokenize normalized text: whitespace-separated chunks, each split by the exception table,
//! the prefix and suffix punctuation rules and the infix rules; `space_after` records the
//! whitespace the source had after each token.
def pos_tokenize(tg : PosTagger; text : string) : array<PosToken> {
    var inscope c <- utf8_to_cpts(text)
    var out : array<PosToken>
    let n = length(c)
    var i = 0
    while (i < n) {
        if (is_space(c[i])) {
            i++
            continue
        }
        var j = i
        while (j < n && !is_space(c[j])) {
            j++
        }
        let before = length(out)
        tokenize_chunk(tg, out, c, i, j)
        if (length(out) > before) {
            out[length(out) - 1].space_after = j < n
        }
        i = j
    }
    return <- out
}

//! Fill every token's tag in place.
def pos_tag(tg : PosTagger; var toks : array<PosToken>) {
    var inscope words <- [for (t in toks); t.text]
    var inscope tags <- pos_tag_words(tg, words)
    for (t, tag in toks, tags) {
        t.tag = tag
    }
}

//! Tokenize and tag normalized text in one call.
def pos_tokenize_tag(tg : PosTagger; text : string) : array<PosToken> {
    var toks <- pos_tokenize(tg, text)
    pos_tag(tg, toks)
    return <- toks
}
