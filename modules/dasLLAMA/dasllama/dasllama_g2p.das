options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_g2p shared public

require dasllama/dasllama_lint public
require math
require strings
require daslib/strings_boost
require daslib/fio
require daslib/utf8_utils
require llvm/daslib/f16_cvt  // the pack stores the GRU as f16; the reader widens it once at load
require dasllama/dasllama_unicode
require dasllama/dasllama_postag
require dasllama/dasllama_textnorm

//! Grapheme-to-phoneme conversion for the TTS front end, emitting the English inventory the
//! StyleTTS2-lineage models were trained on - American by default, British under the
//! `british` flag. A gold lexicon (part-of-speech conditioned entries first), a silver
//! lexicon, function-word rules keyed on what follows (a/an/the/to weak forms, "used to"),
//! inflection stemming (-s, -ed, -ing, possessives), capitalization and acronym stress, then
//! a fallback chain - CMUdict pre-rendered into the American inventory, then a GRU
//! sequence-to-sequence spelling model - so no word is ever dropped. Data loads from
//! tts_g2p.bin (harness/build_g2p_data.py); tokens and tags come from dasllama_postag.

//! One byte-sorted string table of the pack: offsets into the mapped file
struct LexTable {
    n : int
    key_off : int64
    val_off : int64
    key_blob : int64
    val_blob : int64
}

//! The loaded pack: the file bytes (lexicons are searched in place), the three tables, and
//! the GRU spelling model's weights and alphabets.
struct G2pModel {
    data : array<uint8>
    source : string
    gold : LexTable
    silver : LexTable
    cmu : LexTable
    enc_emb : array<float>
    enc_w_ih : array<float>
    enc_w_hh : array<float>
    enc_b_ih : array<float>
    enc_b_hh : array<float>
    dec_emb : array<float>
    dec_w_ih : array<float>
    dec_w_hh : array<float>
    dec_b_ih : array<float>
    dec_b_hh : array<float>
    fc_w : array<float>
    fc_b : array<float>
    hidden : int
    emb : int
    grapheme_id : array<int>
    phonemes : array<string>
}

//! A token on its way to phonemes: surface, tag and trailing space from the tokenizer, the
//! phonemes once resolved (`has_ps`) with the tier that resolved them (4 gold, 3 silver or
//! rule, 2 fallback); `is_head` opens a glued group, `prespace` spaces a separately resolved piece.
struct G2pToken {
    text : string
    tag : string
    space_after : bool
    phonemes : string
    has_ps : bool
    rating : int
    is_head : bool
    prespace : bool
    alias : string
}

let private G2P_MAGIC = 0x50324754u
let private G2P_VERSION = 2
let private TENSOR_F16 = 1
let private PRIMARY = 712   // ˈ
let private SECONDARY = 716   // ˌ
let private NO_STRESS = -99.0
let private VOWELS <- { 'A', 'I', 'O', 'Q', 'W', 'Y', 'a', 'i', 'u', 230, 593, 594, 596, 601, 603, 604, 618, 650, 652, 7547 }
let private CONSONANTS <- { 'b', 'd', 'f', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 's', 't', 'v', 'w', 'z', 240, 331, 609, 633, 638, 643, 658, 676, 679, 952 }
let private DIPHTHONGS <- { 'A', 'I', 'O', 'Q', 'W', 'Y', 676, 679 }
let private US_TAUS <- { 'A', 'I', 'O', 'W', 'Y', 'i', 'u', 230, 593, 601, 603, 618, 633, 650, 652 }
//! the punctuation a group's last piece keeps as its own phoneme, and what ends a clause for
//! the future-vowel context
let private NON_QUOTE_PUNCTS <- { ';', ':', ',', '.', '!', '?', 8212, 8230 }
let private PUNCTS <- { ';', ':', ',', '.', '!', '?', 8212, 8230, '"', 8220, 8221 }
let private SUBTOKEN_JUNKS <- { '\'', ',', '-', '.', '_', 8216, 8217, '/' }
let private PUNCT_TAGS <- { ".", ",", "-LRB-", "-RRB-", "``", "\"\"", "''", ":", "$", "#", "NFP" }
let private PUNCT_TAG_PHONEMES <- { "-LRB-" => "(", "-RRB-" => ")", "``" => "“", "\"\"" => "”", "''" => "”" }   // nolint:STYLE039
let private ADD_SYMBOLS <- { "." => "dot", "/" => "slash" }
let private SYMBOLS <- { "%" => "percent", "&" => "and", "+" => "plus", "@" => "at" }
let private ORDINAL_SUFFIXES <- { "st", "nd", "rd", "th" }

//! the calibrated ARPAbet -> inventory table (stress digits handled by the caller)
let private ARPA_TO_MISAKI <- {
    "AA" => "ɑ", "AE" => "æ", "AH0" => "ə", "AH" => "ʌ", "AO" => "ɔ", "AW" => "W", "AY" => "I",   // nolint:STYLE039
    "EH" => "ɛ", "ER0" => "əɹ", "ER" => "ɜɹ", "EY" => "A", "IH" => "ɪ", "IY" => "i", "IY0" => "i",   // nolint:STYLE039
    "OW" => "O", "OY" => "Y", "UH" => "ʊ", "UW" => "u",   // nolint:STYLE039
    "B" => "b", "CH" => "ʧ", "D" => "d", "DH" => "ð", "F" => "f", "G" => "ɡ", "HH" => "h", "JH" => "ʤ", "K" => "k",   // nolint:STYLE039
    "L" => "l", "M" => "m", "N" => "n", "NG" => "ŋ", "P" => "p", "R" => "ɹ", "S" => "s", "SH" => "ʃ", "T" => "t", "TH" => "θ",   // nolint:STYLE039
    "V" => "v", "W" => "w", "Y" => "j", "Z" => "z", "ZH" => "ʒ"   // nolint:STYLE039
}
let private ARPA_VOWELS <- { "AA", "AE", "AH", "AO", "AW", "AY", "EH", "ER", "EY", "IH", "IY", "OW", "OY", "UH", "UW" }

let private RHOTIC = 633   // ɹ
let private LENGTH_MARK = 720   // ː
let private SCHWA = 601   // ə

//! A vowel plus the rhotic, before a consonant or a word end: the British inventory drops the
//! ɹ into the vowel it leaves. Keyed by the American vowel's codepoint, each row the majority
//! reading of the pack's own merged entries there; a vowel with no row keeps its ɹ.
let private GB_RHOTIC_PRE_C <- {
    601 => "ə", 604 => "ɜː", 596 => "ɔː", 593 => "ɑː", 603 => "ɛː", 618 => "ɪə", 650 => "ʊə",   // nolint:STYLE039
    'I' => "Iə", 'W' => "Wə", 105 => "ɪə", 117 => "ʊə"   // nolint:STYLE039
}

//! The same pair before a vowel, where the ɹ links and survives
let private GB_RHOTIC_PRE_V <- {
    601 => "əɹ", 604 => "ʌɹ", 596 => "ɔːɹ", 593 => "ɑːɹ", 603 => "ɛːɹ", 618 => "ɪəɹ", 650 => "ʊəɹ",   // nolint:STYLE039
    230 => "aɹ", 105 => "iːɹ", 117 => "ʊəɹ", 'O' => "Qɹ", 7547 => "ɪɹ"   // nolint:STYLE039
}

//! The symbols the British inventory spells differently outside the rhotic pairs - the flap
//! under both its spellings, since the pipeline folds `ɾ` to `T` on its way out. `i` is the
//! exception the table cannot carry: it lengthens only under stress.
let private GB_SYMBOL <- {
    230 => "a", 'O' => "Q", 593 => "ɒ", 596 => "ɔː", 117 => "uː", 7547 => "ɪ", 638 => "t", 660 => "t", 'T' => "t"   // nolint:STYLE039
}

// ---------------------------------------------------------------- the packed file

def private rd_u8(b : array<uint8> | #; o : int64) : int => int(b[o])
def private rd_u16(b : array<uint8> | #; o : int64) : int => int(b[o]) | (int(b[o + 1l]) << 8)
def private rd_u32(b : array<uint8> | #; o : int64) : uint => uint(b[o]) | (uint(b[o + 1l]) << 8u) | (uint(b[o + 2l]) << 16u) | (uint(b[o + 3l]) << 24u)
def private rd_i32(b : array<uint8> | #; o : int64) : int => int(rd_u32(b, o))
def private bytes_str(b : array<uint8> | #; o : int64; n : int) : string {
    var inscope bytes : array<uint8>
    bytes |> resize(n)
    for (i in range(n)) {
        bytes[i] = b[o + int64(i)]
    }
    return string(bytes)
}

def private read_lex_table(b : array<uint8>; var o : int64&) : LexTable {
    var t = LexTable()
    t.n = rd_i32(b, o)
    o += 4l
    t.key_off = o
    o += int64(t.n + 1) * 4l
    t.val_off = o
    o += int64(t.n + 1) * 4l
    let key_len = rd_i32(b, o)
    o += 4l
    t.key_blob = o
    o += int64(key_len)
    let val_len = rd_i32(b, o)
    o += 4l
    t.val_blob = o
    o += int64(val_len)
    return t
}

//! One GRU tensor, widened from the f16 the pack stores it as (dtype 0 is f32, kept so a
//! future pack can carry a tensor at full width without a version bump).
def private read_tensor(b : array<uint8>; var o : int64&; var dst : array<float>) : int {
    let name_len = rd_u8(b, o)
    o += 1l + int64(name_len)
    let ndims = rd_u8(b, o)
    o++
    var n = 1
    for (_d in range(ndims)) {
        n *= rd_i32(b, o)
        o += 4l
    }
    let dtype = rd_u8(b, o)
    o++
    dst |> reserve(n)
    dst |> resize(n)
    if (dtype == TENSOR_F16) {
        for (i in range(n)) {
            dst[i] = f16_to_f32(uint(rd_u16(b, o + int64(i) * 2l)))
        }
        o += int64(n) * 2l
    } else {
        unsafe {
            memcpy(addr(dst[0]), addr(b[o]), n * 4)
        }
        o += int64(n) * 4l
    }
    return n
}

def private read_alphabet(b : array<uint8>; var o : int64&) : array<string> {
    let n = rd_i32(b, o)
    o += 4l
    var out : array<string>
    out |> reserve(n)
    for (_i in range(n)) {
        let l = rd_u8(b, o)
        o++
        out |> push(bytes_str(b, o, l))
        o += int64(l)
    }
    return <- out
}

//! Load tts_g2p.bin. Panics on a missing file, a wrong magic or an unsupported version.
def load_g2p(path : string) : G2pModel {
    var g = G2pModel()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA tts: cannot open g2p data '{path}'")
    }
    fmap(f) $(var b : array<uint8>#) {
        g.data |> reserve(long_length(b))
        g.data |> resize(long_length(b))
        unsafe {
            memcpy(addr(g.data[0]), addr(b[0]), long_length(b))
        }
    }
    fclose(f)
    var o = 0l
    if (rd_u32(g.data, o) != G2P_MAGIC) {
        panic("dasLLAMA tts: bad magic in '{path}' (not a tts_g2p.bin)")
    }
    o += 4l
    let version = rd_i32(g.data, o)
    o += 4l
    if (version != G2P_VERSION) {
        panic("dasLLAMA tts: '{path}' is a tts_g2p.bin version {version} pack, and this build reads version {G2P_VERSION} - rebuild it with harness/build_g2p_data.py")
    }
    let src_len = rd_u16(g.data, o)
    o += 2l
    g.source = bytes_str(g.data, o, src_len)
    o += int64(src_len)
    g.gold = read_lex_table(g.data, o)
    g.silver = read_lex_table(g.data, o)
    g.cmu = read_lex_table(g.data, o)
    let n_tensors = rd_i32(g.data, o)
    o += 4l
    if (n_tensors != 12) {
        panic("dasLLAMA tts: '{path}' carries {n_tensors} spelling-model tensors, expected 12")
    }
    read_tensor(g.data, o, g.enc_emb)
    read_tensor(g.data, o, g.enc_w_ih)
    read_tensor(g.data, o, g.enc_w_hh)
    g.hidden = read_tensor(g.data, o, g.enc_b_ih) / 3
    read_tensor(g.data, o, g.enc_b_hh)
    read_tensor(g.data, o, g.dec_emb)
    read_tensor(g.data, o, g.dec_w_ih)
    read_tensor(g.data, o, g.dec_w_hh)
    read_tensor(g.data, o, g.dec_b_ih)
    read_tensor(g.data, o, g.dec_b_hh)
    read_tensor(g.data, o, g.fc_w)
    read_tensor(g.data, o, g.fc_b)
    var inscope graphemes <- read_alphabet(g.data, o)
    g.emb = length(g.enc_emb) / length(graphemes)
    g.grapheme_id |> resize(128)
    for (i in range(128)) {
        g.grapheme_id[i] = 1
    }
    for (i, s in iter_range(graphemes), graphemes) {
        if (length(s) == 1) {
            var inscope c <- utf8_to_cpts(s)
            g.grapheme_id[c[0] & 127] = i
        }
    }
    g.phonemes <- read_alphabet(g.data, o)
    return <- g
}

// ---------------------------------------------------------------- lexicon tables

//! bytewise compare of a key of the table with the query bytes: <0, 0, >0
def private key_compare(g : G2pModel; t : LexTable; i : int; q : array<uint8>) : int {
    let a = t.key_blob + int64(rd_u32(g.data, t.key_off + int64(i) * 4l))
    let klen = int(rd_u32(g.data, t.key_off + int64(i + 1) * 4l) - rd_u32(g.data, t.key_off + int64(i) * 4l))
    let n = min(klen, length(q))
    for (k in range(n)) {
        let d = int(g.data[a + int64(k)]) - int(q[k])
        return d if (d != 0)
    }
    return klen - length(q)
}

//! the entry index of `word` in a table, or -1
def private lex_find(g : G2pModel; t : LexTable; word : string) : int {
    var inscope q <- to_bytes(word)
    var lo = 0
    var hi = t.n - 1
    while (lo <= hi) {
        let mid = (lo + hi) >> 1
        let c = key_compare(g, t, mid, q)
        return mid if (c == 0)
        if (c < 0) {
            lo = mid + 1
        } else {
            hi = mid - 1
        }
    }
    return -1
}

def private lex_value_span(g : G2pModel; t : LexTable; i : int; var a : int64&; var b : int64&) {
    a = t.val_blob + int64(rd_u32(g.data, t.val_off + int64(i) * 4l))
    b = t.val_blob + int64(rd_u32(g.data, t.val_off + int64(i + 1) * 4l))
}

//! One dialect's part of a merged entry as the span [a, b) over the mapped bytes, false when
//! the entry carries no reading for it. A key of the merged table exists for either dialect;
//! only the part says whether THIS one knows the word.
def private lex_part(g : G2pModel; t : LexTable; i : int; british : bool; var a : int64&; var b : int64&) : bool {
    var v0 = 0l
    var v1 = 0l
    lex_value_span(g, t, i, v0, v1)
    let flags = rd_u8(g.data, v0)
    var o = v0 + 1l
    if ((flags & 1) != 0) {
        let us_len = rd_u16(g.data, o)
        o += 2l
        if (!british) {
            a = o
            b = o + int64(us_len)
            return true
        }
        o += int64(us_len)
    } elif (!british) {
        return false
    }
    return false if ((flags & 2) == 0)
    a = o
    b = v1
    return true
}

def private lex_has(g : G2pModel; t : LexTable; i : int; british : bool) : bool {
    var a = 0l
    var b = 0l
    return lex_part(g, t, i, british, a, b)
}

def private lex_is_dict(g : G2pModel; t : LexTable; i : int; british : bool) : bool {
    var a = 0l
    var b = 0l
    return false if (!lex_part(g, t, i, british, a, b))
    return g.data[a] == uint8(1)
}

//! the phonemes of a value span [a, b): the first byte is the entry kind, the rest the reading
def private span_ps(g : G2pModel; a, b : int64) : string => bytes_str(g.data, a + 1l, int(b - a - 1l))

//! a single-dialect table's entry: the CMUdict section, whose values carry no dialect part
def private lex_plain_single(g : G2pModel; t : LexTable; i : int) : string {
    var a = 0l
    var b = 0l
    lex_value_span(g, t, i, a, b)
    return span_ps(g, a, b)
}

//! a plain entry's phonemes
def private lex_plain(g : G2pModel; t : LexTable; i : int; british : bool) : string {
    var a = 0l
    var b = 0l
    return "" if (!lex_part(g, t, i, british, a, b))
    return span_ps(g, a, b)
}

//! a part-of-speech entry's reading for `tag`: `found` says the tag has a row, `present` that
//! the row carries phonemes (a row may deliberately carry none)
def private lex_dict_get(g : G2pModel; t : LexTable; i : int; british : bool; tag : string; var found : bool&; var present : bool&) : string {
    var a = 0l
    var b = 0l
    found = false
    present = false
    return "" if (!lex_part(g, t, i, british, a, b))
    var o = a + 1l
    let n_pairs = rd_u8(g.data, o)
    o++
    var inscope tag_bytes <- to_bytes(tag)
    for (_p in range(n_pairs)) {
        let tl = rd_u8(g.data, o)
        o++
        var same = tl == length(tag_bytes)
        if (same) {
            for (k in range(tl)) {
                same = same && g.data[o + int64(k)] == tag_bytes[k]
            }
        }
        o += int64(tl)
        let has = rd_u8(g.data, o) != 0
        let vl = rd_u16(g.data, o + 1l)
        o += 3l
        if (same) {
            found = true
            present = has
            return has ? bytes_str(g.data, o, vl) : ""
        }
        o += int64(vl)
    }
    return ""
}

// ---------------------------------------------------------------- phoneme string helpers

def private cp_in(s : string; set : table<int>) : bool {
    var inscope c <- utf8_to_cpts(s)
    for (cp in c) {
        return true if (key_exists(set, cp))
    }
    return false
}

def private has_stress(s : string) : bool {
    var inscope c <- utf8_to_cpts(s)
    for (cp in c) {
        return true if (cp == PRIMARY || cp == SECONDARY)
    }
    return false
}

def private has_primary(s : string) : bool => find(s, cpt_to_utf8(PRIMARY)) >= 0
def private has_vowel(s : string) : bool => cp_in(s, VOWELS)

def private drop_cps(s : string; a, b : int) : string {
    var inscope c <- utf8_to_cpts(s)
    var inscope out <- [for (cp in c); cp; where cp != a && cp != b]
    return cpts_to_utf8(out)
}

def private replace_cp(s : string; from, to : int) : string {
    var inscope c <- utf8_to_cpts(s)
    var inscope out <- [for (cp in c); cp == from ? to : cp]
    return cpts_to_utf8(out)
}

//! every stress mark moves to just before the next vowel
def private restress(s : string) : string {
    var inscope c <- utf8_to_cpts(s)
    let n = length(c)
    var inscope out : array<int>
    out |> reserve(n)
    var inscope marks : array<int>
    for (i in range(n)) {
        let cp = c[i]
        if (cp == PRIMARY || cp == SECONDARY) {
            var j = i
            while (j < n && !key_exists(VOWELS, c[j])) {
                j++
            }
            if (j < n) {
                marks |> push(j)
                marks |> push(cp)
                continue
            }
        }
        var k = 0
        while (k < length(marks)) {
            if (marks[k] == i) {
                out |> push(marks[k + 1])
            }
            k += 2
        }
        out |> push(cp)
    }
    return cpts_to_utf8(out)
}

//! The reference pipeline's stress adjustment: -2 strips, -1 (or 0 / -0.5 on a word with a
//! primary) demotes to secondary, 0 / 0.5 / 1 adds a secondary to an unstressed word, >= 1
//! promotes a lone secondary, > 1 adds a primary to an unstressed word.
def apply_stress(ps : string; stress : float) : string {
    return ps if (stress == NO_STRESS)
    let primary = has_primary(ps)
    let stressed = has_stress(ps)
    if (stress < -1.0) {
        return drop_cps(ps, PRIMARY, SECONDARY)
    } elif (stress == -1.0 || ((stress == 0.0 || stress == -0.5) && primary)) {
        return replace_cp(drop_cps(ps, SECONDARY, SECONDARY), PRIMARY, SECONDARY)
    } elif ((stress == 0.0 || stress == 0.5 || stress == 1.0) && !stressed) {
        return ps if (!has_vowel(ps))
        return restress("{cpt_to_utf8(SECONDARY)}{ps}")
    } elif (stress >= 1.0 && !primary && stressed) {
        return replace_cp(ps, SECONDARY, PRIMARY)
    } elif (stress > 1.0 && !stressed) {
        return ps if (!has_vowel(ps))
        return restress("{cpt_to_utf8(PRIMARY)}{ps}")
    }
    return ps
}

def private stress_weight(ps : string) : int {
    var inscope c <- utf8_to_cpts(ps)
    var w = 0
    for (cp in c) {
        w += key_exists(DIPHTHONGS, cp) ? 2 : 1
    }
    return w
}

def private last_cp(s : string) : int {
    var inscope c <- utf8_to_cpts(s)
    return empty(c) ? -1 : c[length(c) - 1]
}

def private cp_count(s : string) : int {
    var inscope c <- utf8_to_cpts(s)
    return length(c)
}

def private drop_last_cp(s : string) : string {
    var inscope c <- utf8_to_cpts(s)
    return cpts_to_utf8(c, 0, max(0, length(c) - 1))
}

def private second_last_cp(s : string) : int {
    var inscope c <- utf8_to_cpts(s)
    return length(c) >= 2 ? c[length(c) - 2] : -1
}

def private next_is_vowel(c : array<int>; from : int) : bool {
    var j = from
    while (j < length(c) && (c[j] == PRIMARY || c[j] == SECONDARY)) {
        j++
    }
    return j < length(c) && key_exists(VOWELS, c[j])
}

//! one American symbol in the British inventory: the table, `i` lengthened under stress. A
//! vowel already carrying the length mark passes through, which makes the rewrite idempotent -
//! the American inventory has no length mark, so the guard is inert on American input.
def private gb_symbol(c : array<int>; i : int) : string {
    let cp = c[i]
    return cpt_to_utf8(cp) if (i + 1 < length(c) && c[i + 1] == LENGTH_MARK)
    if (cp == 'i') {
        return (i > 0 && (c[i - 1] == PRIMARY || c[i - 1] == SECONDARY)) ? "iː" : "i"   // nolint:STYLE039
    }
    return GB_SYMBOL?[cp] ?? cpt_to_utf8(cp)
}

def misaki_us_to_gb(ps : string) : string {
    //! The American inventory's phonemes in the British one, aligned off us_gold/gb_gold: the
    //! post-vocalic ɹ into the vowel it leaves, then the symbols the two spell differently.
    //! AMERICAN INPUT ONLY - it reads every ɛɹ before a vowel as SQUARE, merry as Mary.
    var inscope c <- utf8_to_cpts(ps)
    let n = length(c)
    return build_string() $(var w) {
        var i = 0
        while (i < n) {
            let cp = c[i]
            if (i + 1 < n && c[i + 1] == RHOTIC && key_exists(VOWELS, cp)) {
                let linking = next_is_vowel(c, i + 2)
                let pair = linking ? (GB_RHOTIC_PRE_V?[cp] ?? "") : (GB_RHOTIC_PRE_C?[cp] ?? "")
                if (!empty(pair)) {
                    w |> write(pair)
                } else {
                    w |> write(gb_symbol(c, i))
                    w |> write(cpt_to_utf8(RHOTIC))
                }
                i += 2
                continue
            }
            w |> write(gb_symbol(c, i))
            i++
        }
    }
}

// ---------------------------------------------------------------- word shape

def private is_ascii_alpha(cp : int) : bool => cp < 128 && is_alpha(cp)
def private is_lexicon_cp(cp : int) : bool => cp == '\'' || cp == '-' || is_ascii_alpha(cp)

def private all_lexicon_cps(w : string) : bool {
    var inscope c <- utf8_to_cpts(w)
    for (cp in c) {
        return false if (!is_lexicon_cp(cp))
    }
    return true
}

def private is_alpha_word(w : string) : bool {
    var inscope c <- utf8_to_cpts(w)
    return false if (empty(c))
    for (cp in c) {
        return false if (!(cp >= 0 && cpt_is_letter(cp)))
    }
    return true
}

def private is_digits(w : string) : bool {
    var ok = !empty(w)
    peek_data(w) $(d : array<uint8> const#) {
        for (b in d) {
            ok = ok && is_number(int(b))
        }
    }
    return ok
}

def private lower_word(w : string) : string {
    var inscope c <- utf8_to_cpts(w)
    var inscope out <- [for (cp in c); int(utf32_to_lower(uint(cp)))]
    return cpts_to_utf8(out)
}

def private upper_cp(cp : int) : int {
    return cp - 32 if (cp >= 'a' && cp <= 'z')
    return cp
}

def private upper_word(w : string) : string {
    var inscope c <- utf8_to_cpts(w)
    var inscope out <- [for (cp in c); (cp >= 128 && int(utf32_to_lower(uint(cp))) == cp && cpt_is_letter(cp)) ? cp : upper_cp(cp)]
    return cpts_to_utf8(out)
}

def private is_lower_word(w : string) : bool => lower_word(w) == w
def private is_upper_word(w : string) : bool => upper_word(w) == w

//! first character upper, the rest lower
def private capitalize_word(w : string) : string {
    var inscope c <- utf8_to_cpts(w)
    return "" if (empty(c))
    var inscope out : array<int>
    out |> reserve(length(c))
    out |> push(upper_cp(c[0]))
    for (i in range(1, length(c))) {
        out |> push(int(utf32_to_lower(uint(c[i]))))
    }
    return cpts_to_utf8(out)
}

def private tail_from(w : string; n : int) : string {
    var inscope c <- utf8_to_cpts(w)
    return cpts_to_utf8(c, min(n, length(c)), length(c))
}

def private head_to(w : string; n : int) : string {
    var inscope c <- utf8_to_cpts(w)
    return cpts_to_utf8(c, 0, clamp(n, 0, length(c)))
}

def private chop_end(w : string; n : int) : string => head_to(w, cp_count(w) - n)

// ---------------------------------------------------------------- the lexicon

struct PsResult {
    ok : bool
    ps : string
    rating : int
}

def private none_ps() : PsResult => PsResult(ok = false, ps = "", rating = 0)
def private some_ps(ps : string; rating : int) : PsResult => PsResult(ok = true, ps = ps, rating = rating)

//! what a word sees around it, read right to left: whether the next spoken sound is a vowel
//! (-1 unknown, 0 no, 1 yes), whether the next word is "to", and the words on either side -
//! what settles a heteronym the tag alone cannot
struct TokenContext {
    future_vowel : int
    future_to : bool
    prev_text : string   // the word before, lowercased; "" at the start
    prev_tag : string
    next_text : string   // the word after, lowercased; "" at the end
    next_tag : string
}

//! heteronyms the tags cannot split - the reading a following word settles
let private HETERONYM_NEXT <- {
    "bass" => { "player" => "bˈAs", "players" => "bˈAs", "guitar" => "bˈAs", "guitars" => "bˈAs", "line" => "bˈAs", "lines" => "bˈAs",   // nolint:STYLE039
        "drum" => "bˈAs", "drums" => "bˈAs", "clef" => "bˈAs", "note" => "bˈAs", "notes" => "bˈAs", "voice" => "bˈAs", "voices" => "bˈAs",   // nolint:STYLE039
        "singer" => "bˈAs", "part" => "bˈAs", "parts" => "bˈAs", "amp" => "bˈAs" },   // nolint:STYLE039
    "lead" => { "pipe" => "lˈɛd", "pipes" => "lˈɛd", "paint" => "lˈɛd", "poisoning" => "lˈɛd", "weight" => "lˈɛd", "weights" => "lˈɛd",   // nolint:STYLE039
        "shot" => "lˈɛd", "bullet" => "lˈɛd", "bullets" => "lˈɛd", "out" => "lˈɛd" },   // nolint:STYLE039
    "minute" => { "detail" => "mInˈut", "details" => "mInˈut", "amount" => "mInˈut", "amounts" => "mInˈut", "particle" => "mInˈut",   // nolint:STYLE039
        "particles" => "mInˈut", "quantity" => "mInˈut", "quantities" => "mInˈut", "trace" => "mInˈut", "traces" => "mInˈut",   // nolint:STYLE039
        "difference" => "mInˈut", "differences" => "mInˈut" }   // nolint:STYLE039
}

//! heteronyms the lexicon carries as one reading: the part-of-speech keyed readings it lacks
let private HETERONYM_OVERRIDES <- {
    "does" => { "DEFAULT" => "dˈʌz", "NOUN" => "dˈOz" },   // nolint:STYLE039
    "resume" => { "DEFAULT" => "ɹəzˈum", "NOUN" => "ɹˈɛzəmˌA" }   // nolint:STYLE039
}

[arch(at = "../ARCHITECTURE_TTS.md#tts-heteronym-context")]
def private key_order(tag : string; ctx : TokenContext; has_ctx : bool) : array<string> {
    var keys : array<string>
    if (has_ctx && ctx.prev_text == "to" && ctx.prev_tag == "TO") {
        keys |> push("VERB")
    } elif (has_ctx && ctx.prev_tag == "DT") {
        if (starts_with(tag, "VB")) {
            keys |> push("NOUN")
            keys |> push("DEFAULT")
            return <- keys
        }
        if (tag == "JJ" && !starts_with(ctx.next_tag, "NN")) {
            keys |> push("NOUN")
        }
    }
    keys |> push(tag)
    keys |> push(parent_tag(tag))
    keys |> push("DEFAULT")
    return <- keys
}

//! the entry of `word` this dialect knows: the key alone is not enough, since the merged table
//! holds a key either dialect carries
def private lex_find_part(g : G2pModel; t : LexTable; word : string; british : bool) : int {
    let i = lex_find(g, t, word)
    return (i >= 0 && lex_has(g, t, i, british)) ? i : -1
}

//! the grown lexicon: a word, then its capitalized or lowercased twin the way the reference
//! grows its dictionaries (the twin only for keys of two or more characters)
def private grown_find(g : G2pModel; t : LexTable; word : string; british : bool) : int {
    let i = lex_find_part(g, t, word, british)
    return i if (i >= 0)
    return -1 if (cp_count(word) < 2)
    let lower = lower_word(word)
    let cap = capitalize_word(word)
    if (word == cap && lower != cap) {
        return lex_find_part(g, t, lower, british)
    }
    if (word == lower && cap != lower) {
        return lex_find_part(g, t, cap, british)
    }
    return -1
}

def private in_golds(g : G2pModel; word : string; british : bool) : bool => grown_find(g, g.gold, word, british) >= 0
def private in_silvers(g : G2pModel; word : string; british : bool) : bool => grown_find(g, g.silver, word, british) >= 0

def private parent_tag(tag : string) : string {
    return "VERB" if (starts_with(tag, "VB"))
    return "NOUN" if (starts_with(tag, "NN"))
    return "ADV" if (starts_with(tag, "ADV") || starts_with(tag, "RB"))
    return "ADJ" if (starts_with(tag, "ADJ") || starts_with(tag, "JJ"))
    return tag
}

//! resolve one table entry (plain or part-of-speech keyed) for a tag and context
def private entry_ps(g : G2pModel; t : LexTable; i : int; tag : string; ctx : TokenContext; has_ctx, british : bool; var present : bool&) : string {
    if (!lex_is_dict(g, t, i, british)) {
        var a = 0l
        var b = 0l
        present = lex_part(g, t, i, british, a, b)
        return present ? span_ps(g, a, b) : ""
    }
    var found = false
    if (has_ctx && ctx.future_vowel < 0) {
        let none_ps = lex_dict_get(g, t, i, british, "None", found, present)
        return none_ps if (found)
    }
    var inscope keys <- key_order(tag, ctx, has_ctx)
    var ps = ""
    for (key in keys) {
        ps = lex_dict_get(g, t, i, british, key, found, present)
        return ps if (found)
    }
    return ps
}

//! a name or acronym spelled letter by letter, the last letter carrying the primary stress
def private get_nnp(g : G2pModel; word : string; british : bool) : PsResult {
    var inscope c <- utf8_to_cpts(word)
    var inscope parts : array<string>
    for (cp in c) {
        continue if (!(cp >= 0 && cpt_is_letter(cp)))
        let i = lex_find_part(g, g.gold, cpt_to_utf8(upper_cp(cp)), british)
        return none_ps() if (i < 0 || lex_is_dict(g, g.gold, i, british))
        parts |> push(lex_plain(g, g.gold, i, british))
    }
    let ps = apply_stress(join(parts, ""), 0.0)
    let sec = cpt_to_utf8(SECONDARY)
    let last = rfind(ps, sec)
    return some_ps(ps, 3) if (last < 0)
    return some_ps("{slice(ps, 0, last)}{cpt_to_utf8(PRIMARY)}{slice(ps, last + length(sec))}", 3)
}

def private lookup(g : G2pModel; word_in : string; tag : string; stress : float; ctx : TokenContext; has_ctx, british : bool) : PsResult {
    var word = word_in
    var is_nnp = false
    var nnp_case = false
    if (is_upper_word(word) && !in_golds(g, word, british)) {
        word = lower_word(word)
        nnp_case = true
        is_nnp = tag == "NNP"
    }
    if (has_ctx) {
        let wl = lower_word(word)
        var hit = ""
        // every collocation names a NOUN reading, so a verb tag out-ranks the whole table
        if (!starts_with(tag, "VB")) {
            HETERONYM_NEXT |> get(wl) $(coll) {
                coll |> get(ctx.next_text) $(cps) {
                    hit = cps
                }
            }
        }
        // the collocation and override readings are written in the American inventory
        return some_ps(apply_stress(british ? misaki_us_to_gb(hit) : hit, stress), 4) if (!empty(hit))
        HETERONYM_OVERRIDES |> get(wl) $(ov) {
            var inscope keys <- key_order(tag, ctx, has_ctx)
            for (key in keys) {
                ov |> get(key) $(ops) {
                    hit = ops
                }
                break if (!empty(hit))
            }
        }
        return some_ps(apply_stress(british ? misaki_us_to_gb(hit) : hit, stress), 4) if (!empty(hit))
    }
    var rating = 4
    var i = grown_find(g, g.gold, word, british)
    var t = g.gold
    if (i < 0 && !is_nnp) {
        i = grown_find(g, g.silver, word, british)
        t = g.silver
        rating = 3
    }
    var present = false
    var ps = ""
    if (i >= 0) {
        ps = entry_ps(g, t, i, tag, ctx, has_ctx, british, present)
    }
    if (!present || (is_nnp && !has_primary(ps))) {
        let nnp = get_nnp(g, word, british)
        return nnp if (nnp.ok)
    }
    return none_ps() if (!present)
    return some_ps(apply_stress(ps, stress), rating)
}

def private is_known(g : G2pModel; word : string; _tag : string; british : bool) : bool {
    return true if (in_golds(g, word, british) || key_exists(SYMBOLS, word) || in_silvers(g, word, british))
    return false if (!is_alpha_word(word) || !all_lexicon_cps(word))
    return true if (cp_count(word) == 1 || (is_upper_word(word) && in_golds(g, lower_word(word), british)))
    let rest = tail_from(word, 1)
    return rest == upper_word(rest)
}

def private suffix_s(stem : string; british : bool) : string {
    return "" if (empty(stem))
    let last = last_cp(stem)
    return "{stem}s" if (last == 'p' || last == 't' || last == 'k' || last == 'f' || last == 952)
    if (last == 's' || last == 'z' || last == 643 || last == 658 || last == 679 || last == 676) {
        return british ? "{stem}ɪz" : "{stem}ᵻz"   // nolint:STYLE039
    }
    return "{stem}z"
}

def private suffix_ed(stem : string; british : bool) : string {
    return "" if (empty(stem))
    let last = last_cp(stem)
    return "{stem}t" if (last == 'p' || last == 'k' || last == 'f' || last == 952 || last == 643 || last == 's' || last == 679)
    if (last == 'd') {
        return british ? "{stem}ɪd" : "{stem}ᵻd"   // nolint:STYLE039
    }
    return "{stem}d" if (last != 't')
    return "{stem}ɪd" if (british || cp_count(stem) < 2)   // nolint:STYLE039
    return "{drop_last_cp(stem)}ɾᵻd" if (key_exists(US_TAUS, second_last_cp(stem)))   // nolint:STYLE039
    return "{stem}ᵻd"   // nolint:STYLE039
}

//! British declines a stem already ending in a schwa or a length mark - the reference has no
//! rule for the linking r such a stem wants, and answers unknown rather than guess
def private suffix_ing(stem : string; british : bool) : string {
    return "" if (empty(stem))
    if (british) {
        return "" if (last_cp(stem) == SCHWA || last_cp(stem) == LENGTH_MARK)
        return "{stem}ɪŋ"   // nolint:STYLE039
    }
    if (cp_count(stem) > 1 && last_cp(stem) == 't' && key_exists(US_TAUS, second_last_cp(stem))) {
        return "{drop_last_cp(stem)}ɾɪŋ"   // nolint:STYLE039
    }
    return "{stem}ɪŋ"   // nolint:STYLE039
}

def private suffixed_ps(ps : string; rating : int) : PsResult => empty(ps) ? none_ps() : some_ps(ps, rating)

def private stem_s(g : G2pModel; word : string; tag : string; stress : float; ctx : TokenContext; has_ctx, british : bool) : PsResult {
    return none_ps() if (cp_count(word) < 3 || !ends_with(word, "s"))
    var stem = ""
    if (!ends_with(word, "ss") && is_known(g, chop_end(word, 1), tag, british)) {
        stem = chop_end(word, 1)
    } elif ((ends_with(word, "'s") || (cp_count(word) > 4 && ends_with(word, "es") && !ends_with(word, "ies"))) && is_known(g, chop_end(word, 2), tag, british)) {
        stem = chop_end(word, 2)
    } elif (cp_count(word) > 4 && ends_with(word, "ies") && is_known(g, "{chop_end(word, 3)}y", tag, british)) {
        stem = "{chop_end(word, 3)}y"
    } else {
        return none_ps()
    }
    let r = lookup(g, stem, tag, stress, ctx, has_ctx, british)
    return none_ps() if (!r.ok)
    return suffixed_ps(suffix_s(r.ps, british), r.rating)
}

def private stem_ed(g : G2pModel; word : string; tag : string; stress : float; ctx : TokenContext; has_ctx, british : bool) : PsResult {
    return none_ps() if (cp_count(word) < 4 || !ends_with(word, "d"))
    var stem = ""
    if (!ends_with(word, "dd") && is_known(g, chop_end(word, 1), tag, british)) {
        stem = chop_end(word, 1)
    } elif (cp_count(word) > 4 && ends_with(word, "ed") && !ends_with(word, "eed") && is_known(g, chop_end(word, 2), tag, british)) {
        stem = chop_end(word, 2)
    } else {
        return none_ps()
    }
    let r = lookup(g, stem, tag, stress, ctx, has_ctx, british)
    return none_ps() if (!r.ok)
    return suffixed_ps(suffix_ed(r.ps, british), r.rating)
}

//! "([bcdgklmnprstvxz])\1ing$|cking$" - a doubled consonant before -ing, or "cking"
def private doubled_before_ing(word : string) : bool {
    var inscope c <- utf8_to_cpts(word)
    let n = length(c)
    return false if (n < 5)
    let a = c[n - 5]
    let b = c[n - 4]
    return true if (a == 'c' && b == 'k')
    return a == b && find("bcdgklmnprstvxz", a) >= 0
}

def private stem_ing(g : G2pModel; word : string; tag : string; stress : float; ctx : TokenContext; has_ctx, british : bool) : PsResult {
    return none_ps() if (cp_count(word) < 5 || !ends_with(word, "ing"))
    var stem = ""
    if (cp_count(word) > 5 && is_known(g, chop_end(word, 3), tag, british)) {
        stem = chop_end(word, 3)
    } elif (is_known(g, "{chop_end(word, 3)}e", tag, british)) {
        stem = "{chop_end(word, 3)}e"
    } elif (cp_count(word) > 5 && doubled_before_ing(word) && is_known(g, chop_end(word, 4), tag, british)) {
        stem = chop_end(word, 4)
    } else {
        return none_ps()
    }
    let r = lookup(g, stem, tag, stress, ctx, has_ctx, british)
    return none_ps() if (!r.ok)
    return suffixed_ps(suffix_ing(r.ps, british), r.rating)
}

//! a table's plain reading of `word` in this dialect, "" when the table has no such entry or
//! keys it by part of speech
def private lex_plain_reading(g : G2pModel; t : LexTable; word : string; british : bool) : string {
    let i = lex_find_part(g, t, word, british)
    return i >= 0 && !lex_is_dict(g, t, i, british) ? lex_plain(g, t, i, british) : ""
}

def private gold_plain(g : G2pModel; word : string; british : bool) : string => lex_plain_reading(g, g.gold, word, british)

def private gold_dict(g : G2pModel; word, tag : string; british : bool) : string {
    let i = lex_find_part(g, g.gold, word, british)
    return "" if (i < 0 || !lex_is_dict(g, g.gold, i, british))
    var found = false
    var present = false
    return lex_dict_get(g, g.gold, i, british, tag, found, present)
}

//! the function words and symbols whose reading depends on what follows or on their tag
def private special_case(g : G2pModel; word : string; tag : string; stress : float; ctx : TokenContext; british : bool) : PsResult {   // nolint:STYLE037 - one arm per function word, the reference's flat ladder
    if (tag == "ADD" && key_exists(ADD_SYMBOLS, word)) {
        return lookup(g, ADD_SYMBOLS?[word] ?? "", "", -0.5, ctx, true, british)
    }
    if (key_exists(SYMBOLS, word)) {
        return lookup(g, SYMBOLS?[word] ?? "", "", NO_STRESS, ctx, true, british)
    }
    // an interior dot between short letter runs (U.S., a.m.) spells the letters out
    if (find(trim_dots(word), '.') >= 0 && is_alpha_word(replace(word, ".", "")) && longest_dot_piece(word) < 3) {
        return get_nnp(g, word, british)
    }
    if (word == "a" || word == "A") {
        return tag == "DT" ? some_ps("ɐ", 4) : some_ps("ˈA", 4)   // nolint:STYLE039
    }
    if (word == "am" || word == "Am" || word == "AM") {
        return get_nnp(g, word, british) if (starts_with(tag, "NN"))
        return suffixed_ps(gold_plain(g, "am", british), 4) if (ctx.future_vowel < 0 || word != "am" || (stress != NO_STRESS && stress > 0.0))
        return some_ps("ɐm", 4)   // nolint:STYLE039
    }
    if (word == "an" || word == "An" || word == "AN") {
        return get_nnp(g, word, british) if (word == "AN" && starts_with(tag, "NN"))
        return some_ps("ɐn", 4)   // nolint:STYLE039
    }
    if (word == "I" && tag == "PRP") {
        return some_ps("{cpt_to_utf8(SECONDARY)}I", 4)
    }
    if ((word == "by" || word == "By" || word == "BY") && parent_tag(tag) == "ADV") {
        return some_ps("b{cpt_to_utf8(PRIMARY)}I", 4)
    }
    if (word == "to" || word == "To" || (word == "TO" && (tag == "TO" || tag == "IN"))) {
        return suffixed_ps(gold_plain(g, "to", british), 4) if (ctx.future_vowel < 0)
        return some_ps(ctx.future_vowel == 1 ? "tʊ" : "tə", 4)   // nolint:STYLE039
    }
    if (word == "in" || word == "In" || (word == "IN" && tag != "NNP")) {
        let mark : string = (ctx.future_vowel < 0 || tag != "IN") ? cpt_to_utf8(PRIMARY) : ""
        return some_ps("{mark}ɪn", 4)   // nolint:STYLE039
    }
    if (word == "the" || word == "The" || (word == "THE" && tag == "DT")) {
        return some_ps(ctx.future_vowel == 1 ? "ði" : "ðə", 4)   // nolint:STYLE039
    }
    if (tag == "IN" && (to_lower(word) == "vs" || to_lower(word) == "vs.")) {
        return lookup(g, "versus", "", NO_STRESS, ctx, true, british)
    }
    if (word == "used" || word == "Used" || word == "USED") {
        if ((tag == "VBD" || tag == "JJ") && ctx.future_to) {
            return suffixed_ps(gold_dict(g, "used", "VBD", british), 4)
        }
        return suffixed_ps(gold_dict(g, "used", "DEFAULT", british), 4)
    }
    return none_ps()
}

def private trim_dots(w : string) : string {
    var inscope c <- utf8_to_cpts(w)
    var a = 0
    var b = length(c)
    while (a < b && c[a] == '.') {
        a++
    }
    while (b > a && c[b - 1] == '.') {
        b--
    }
    return cpts_to_utf8(c, a, b)
}

def private longest_dot_piece(w : string) : int {
    var best = 0
    var inscope pieces <- split(w, ".")
    for (p in pieces) {
        best = max(best, cp_count(p))
    }
    return best
}

//! a word through the lexicon: special cases, then the case-folded form when the exact one is
//! unknown, then the entry itself, a possessive, or an inflected stem
def private get_word(g : G2pModel; word_in : string; tag : string; stress : float; ctx : TokenContext; british : bool) : PsResult {
    let sp = special_case(g, word_in, tag, stress, ctx, british)
    return sp if (sp.ok)
    var word = word_in
    let wl = lower_word(word)
    if (cp_count(word) > 1 && is_alpha_word(replace(word, "'", "")) && word != wl && (tag != "NNP" || cp_count(word) > 7)
            && !in_golds(g, word, british) && !in_silvers(g, word, british) && (is_upper_word(word) || is_lower_word(tail_from(word, 1)))) {
        if (in_golds(g, wl, british) || in_silvers(g, wl, british) || stem_s(g, wl, tag, stress, ctx, true, british).ok
                || stem_ed(g, wl, tag, stress, ctx, true, british).ok || stem_ing(g, wl, tag, stress, ctx, true, british).ok) {
            word = wl
        }
    }
    return lookup(g, word, tag, stress, ctx, true, british) if (is_known(g, word, tag, british))
    if (ends_with(word, "s'") && is_known(g, "{chop_end(word, 2)}'s", tag, british)) {
        return lookup(g, "{chop_end(word, 2)}'s", tag, stress, ctx, true, british)
    }
    if (ends_with(word, "'") && is_known(g, chop_end(word, 1), tag, british)) {
        return lookup(g, chop_end(word, 1), tag, stress, ctx, true, british)
    }
    let s = stem_s(g, word, tag, stress, ctx, true, british)
    return s if (s.ok)
    let ed = stem_ed(g, word, tag, stress, ctx, true, british)
    return ed if (ed.ok)
    return stem_ing(g, word, tag, stress == NO_STRESS ? 0.5 : stress, ctx, true, british)
}

//! a digit string the normalizer left behind (a glued alphanumeric's number half): read as
//! words, each through the lexicon, ordinal and inflection suffixes honored
def private get_number(g : G2pModel; word_in : string; british : bool) : PsResult {
    var word = word_in
    var suffix = ""
    var inscope c <- utf8_to_cpts(word)
    var split_at = length(c)
    while (split_at > 0 && (is_ascii_alpha(c[split_at - 1]) || c[split_at - 1] == '\'')) {
        split_at--
    }
    if (split_at < length(c)) {
        suffix = cpts_to_utf8(c, split_at, length(c))
        word = cpts_to_utf8(c, 0, split_at)
    }
    var inscope result : array<string>
    var rating = 4
    if (starts_with(word, "-")) {
        let m = lookup(g, "minus", "", NO_STRESS, TokenContext(future_vowel = -1, future_to = false), false, british)
        return none_ps() if (!m.ok)
        result |> push(m.ps)
        word = slice(word, 1)
    }
    let digits = replace(word, ",", "")
    return none_ps() if (!is_digits(digits))
    // the normalizer's 64-bit reader, never to_int, which answers 0 past 2^31
    let over_wide = digit_count(digits) > MAX_DIGITS_AS_NUMBER
    let value = digit_string_value(digits)
    var spoken = digits_to_words(digits)
    if (!over_wide) {
        spoken = key_exists(ORDINAL_SUFFIXES, suffix) ? ordinal_words(value) : number_to_words(value)
    }
    var inscope words <- split_by_chars(replace(spoken, "-", " "), " ")
    for (w in words) {
        continue if (empty(w))
        let r = lookup(g, w, "", NO_STRESS, TokenContext(future_vowel = -1, future_to = false), false, british)
        return none_ps() if (!r.ok)
        result |> push(r.ps)
        rating = min(rating, r.rating)
    }
    return none_ps() if (empty(result))
    let ps = join(result, " ")
    return suffixed_ps(suffix_s(ps, british), rating) if (suffix == "s" || suffix == "'s")
    return suffixed_ps(suffix_ed(ps, british), rating) if (suffix == "ed" || suffix == "'d")
    return suffixed_ps(suffix_ing(ps, british), rating) if (suffix == "ing")
    return some_ps(ps, rating)
}

def private is_number_word(word_in : string; is_head : bool) : bool {
    var inscope c <- utf8_to_cpts(word_in)
    var any_digit = false
    for (cp in c) {
        any_digit = any_digit || is_number(cp)
    }
    return false if (!any_digit)
    var word = word_in
    for (s in ["ing", "'d", "ed", "'s", "st", "nd", "rd", "th", "s"]) {
        if (ends_with(word, s)) {
            word = chop_end(word, cp_count(s))
            break
        }
    }
    var inscope d <- utf8_to_cpts(word)
    for (i, cp in iter_range(d), d) {
        continue if (is_number(cp) || cp == ',' || cp == '.' || (is_head && i == 0 && cp == '-'))
        return false
    }
    return true
}

//! the lexicon's verdict on one token: word first, then a number, else nothing
def private lexicon(g : G2pModel; tk : G2pToken; ctx : TokenContext; british : bool) : PsResult {
    let word = replace(replace(empty(tk.alias) ? tk.text : tk.alias, "‘", "'"), "’", "'")   // nolint:STYLE039
    var stress = NO_STRESS
    if (word != lower_word(word)) {
        stress = is_upper_word(word) ? 2.0 : 0.5
    }
    let r = get_word(g, word, tk.tag, stress, ctx, british)
    return r if (r.ok)
    if (is_number_word(word, tk.is_head)) {
        return get_number(g, word, british)
    }
    return none_ps()
}

// ---------------------------------------------------------------- the spelling model

def private sigmoid(x : float) : float => 1.0 / (1.0 + exp(-x))

//! one GRU step: h' from the input row x and the previous hidden h
def private gru_cell(x : array<float>; var h : array<float>; w_ih, w_hh, b_ih, b_hh : array<float>; hidden, emb : int; var scratch : array<float>) {
    let g3 = hidden * 3
    scratch |> ensure_capacity(g3 * 2)   // reused across every GRU step: amortized, never per-call exact
    scratch |> resize(g3 * 2)
    for (r in range(g3)) {
        var acc_i = b_ih[r]
        var acc_h = b_hh[r]
        let base_i = r * emb
        let base_h = r * hidden
        for (k in range(emb)) {
            acc_i += w_ih[base_i + k] * x[k]
        }
        for (k in range(hidden)) {
            acc_h += w_hh[base_h + k] * h[k]
        }
        scratch[r] = acc_i
        scratch[g3 + r] = acc_h
    }
    for (j in range(hidden)) {
        let rgate = sigmoid(scratch[j] + scratch[g3 + j])
        let zgate = sigmoid(scratch[hidden + j] + scratch[g3 + hidden + j])
        let n = tanh(scratch[2 * hidden + j] + rgate * scratch[g3 + 2 * hidden + j])
        h[j] = (1.0 - zgate) * n + zgate * h[j]
    }
}

//! the stress digit a phone ends with, or -1
def private phone_stress(p : string) : int {
    var st = -1
    peek_data(p) $(d : array<uint8> const#) {
        let last = int(d[length(d) - 1])
        st = is_number(last) ? last - '0' : -1
    }
    return st
}

//! ARPAbet phones with stress digits -> the inventory string, the calibrated table
def arpabet_to_misaki(phones : array<string>) : string {
    var n_vowels = 0
    for (p in phones) {
        let base = phone_stress(p) >= 0 ? slice(p, 0, length(p) - 1) : p
        n_vowels += key_exists(ARPA_VOWELS, base) ? 1 : 0
    }
    return build_string() $(var w) {
        for (p in phones) {
            var base = p
            let st = phone_stress(p)
            if (st >= 0) {
                base = slice(p, 0, length(p) - 1)
            }
            if (st == 1) {
                w |> write(cpt_to_utf8(PRIMARY))
            } elif (st == 2 && n_vowels > 1) {
                w |> write(cpt_to_utf8(SECONDARY))
            }
            w |> write(ARPA_TO_MISAKI?[p] ?? (ARPA_TO_MISAKI?[base] ?? ""))
        }
    }
}

//! spell an unknown lowercase word with the GRU: encoder over the letters, greedy decoder
def g2p_predict(g : G2pModel; word : string) : array<string> {
    var inscope h : array<float>
    h |> reserve(g.hidden)
    h |> resize(g.hidden)
    var inscope x : array<float>
    x |> reserve(g.emb)
    x |> resize(g.emb)
    var inscope scratch : array<float>
    var inscope ids : array<int>
    peek_data(word) $(d : array<uint8> const#) {
        for (b in d) {
            ids |> push(g.grapheme_id[int(b) & 127])
        }
    }
    ids |> push(2)
    for (id in ids) {
        for (k in range(g.emb)) {
            x[k] = g.enc_emb[id * g.emb + k]
        }
        gru_cell(x, h, g.enc_w_ih, g.enc_w_hh, g.enc_b_ih, g.enc_b_hh, g.hidden, g.emb, scratch)
    }
    var out : array<string>
    var prev = 2
    for (_step in range(20)) {
        for (k in range(g.emb)) {
            x[k] = g.dec_emb[prev * g.emb + k]
        }
        gru_cell(x, h, g.dec_w_ih, g.dec_w_hh, g.dec_b_ih, g.dec_b_hh, g.hidden, g.emb, scratch)
        var best = 0
        var best_v = -3.4e38
        for (c in range(length(g.fc_b))) {
            var v = g.fc_b[c]
            let base = c * g.hidden
            for (k in range(g.hidden)) {
                v += g.fc_w[base + k] * h[k]
            }
            if (v > best_v) {
                best_v = v
                best = c
            }
        }
        break if (best == 3)
        out |> push(g.phonemes[best])
        prev = best
    }
    return <- out
}

//! One letter run of a fallback token, in the dialect the caller asked for. The lexicon answers
//! first - a glued group sends its WHOLE surface here as soon as one piece is unresolvable, and
//! CMUdict is pruned of exactly the words both dialects carry. Only the American sources rewrite.
def private fallback_piece(g : G2pModel; low : string; british : bool) : string {
    var ps = lex_plain_reading(g, g.gold, low, british)
    if (empty(ps)) {
        ps = lex_plain_reading(g, g.silver, low, british)
    }
    return ps if (!empty(ps))
    let k = lex_find(g, g.cmu, low)
    if (k >= 0) {
        ps = lex_plain_single(g, g.cmu, k)
    } else {
        var inscope phones <- g2p_predict(g, low)
        ps = arpabet_to_misaki(phones)
    }
    return british ? misaki_us_to_gb(ps) : ps
}

//! The permissive fallback: each letter run of the token through the lexicon, then CMUdict,
//! then the spelling model; pieces join with no space. Rating 2.
def private fallback(g : G2pModel; text : string; british : bool) : PsResult {
    var inscope c <- utf8_to_cpts(text)
    var inscope parts : array<string>
    var i = 0
    let n = length(c)
    while (i < n) {
        if (!is_ascii_alpha(c[i])) {
            i++
            continue
        }
        var j = i
        while (j < n && (is_ascii_alpha(c[j]) || (c[j] == '\'' && j + 1 < n && is_ascii_alpha(c[j + 1])))) {
            j++
        }
        parts |> push(fallback_piece(g, lower_word(cpts_to_utf8(c, i, j)), british))
        i = j
    }
    return suffixed_ps(join(parts, ""), 2)
}

// ---------------------------------------------------------------- tokens and groups

def private is_upper_cp(cp : int) : bool => cp >= 0 && cpt_is_letter(cp) && int(utf32_to_lower(uint(cp))) != cp
def private is_lower_cp(cp : int) : bool => cp >= 0 && cpt_is_letter(cp) && int(utf32_to_lower(uint(cp))) == cp
def private is_letter_cp(cp : int) : bool => cp >= 0 && cpt_is_letter(cp)
def private is_apos(cp : int) : bool => cp == '\'' || cp == 8216 || cp == 8217

//! the length of the subtoken at `p`, the reference splitter's alternatives in order: leading
//! apostrophes, a capital before Capital+lower, a number, dashes, doubled apostrophes, a camelCase
//! piece, a letter run with inner apostrophes, one other character, trailing apostrophes; 0 = skip
def private subtoken_len(c : array<int>; p, n : int) : int {   // nolint:STYLE037 - the splitter regex's alternatives, tried in order
    let cp = c[p]
    if (p == 0 && is_apos(cp)) {
        var j = p
        while (j < n && is_apos(c[j])) {
            j++
        }
        return j - p
    }
    return 1 if (is_upper_cp(cp) && p + 2 < n && is_upper_cp(c[p + 1]) && is_lower_cp(c[p + 2]))
    let num_start = (p == 0 && cp == '-' && p + 1 < n && (is_number(c[p + 1]) || ((c[p + 1] == ',' || c[p + 1] == '.') && p + 2 < n && is_number(c[p + 2])))) ? p + 1 : p
    if (is_number(c[num_start]) || ((c[num_start] == ',' || c[num_start] == '.') && num_start + 1 < n && is_number(c[num_start + 1]))) {
        var j = num_start
        while (j < n) {
            if (is_number(c[j])) {
                j++
            } elif ((c[j] == ',' || c[j] == '.') && j + 1 < n && is_number(c[j + 1])) {
                j += 2
            } else {
                break
            }
        }
        return j - p if (j > num_start)
    }
    if (cp == '-' || cp == '_') {
        var j = p
        while (j < n && (c[j] == '-' || c[j] == '_')) {
            j++
        }
        return j - p
    }
    if (is_apos(cp) && p + 1 < n && is_apos(c[p + 1])) {
        var j = p
        while (j < n && is_apos(c[j])) {
            j++
        }
        return j - p
    }
    if (is_letter_cp(cp)) {
        var j = p
        while (j < n) {
            if (is_letter_cp(c[j])) {
                return j + 1 - p if (is_lower_cp(c[j]) && j + 1 < n && is_upper_cp(c[j + 1]))
                j++
            } elif (is_apos(c[j]) && j + 1 < n && is_letter_cp(c[j + 1])) {
                j++
            } else {
                break
            }
        }
        j = p
        while (j < n) {
            if (is_letter_cp(c[j])) {
                j++
            } elif (is_apos(c[j]) && j + 1 < n && is_letter_cp(c[j + 1])) {
                j += 2
            } else {
                break
            }
        }
        return j - p
    }
    if (!(cp == '-' || cp == '_' || is_letter_cp(cp) || is_apos(cp) || is_number(cp))) {
        return 1
    }
    if (is_apos(cp)) {
        var j = p
        while (j < n && is_apos(c[j])) {
            j++
        }
        return j == n ? j - p : 0
    }
    return 0
}

def private subtokenize(text : string) : array<string> {
    var inscope c <- utf8_to_cpts(text)
    var out : array<string>
    let n = length(c)
    var p = 0
    while (p < n) {
        let l = subtoken_len(c, p, n)
        if (l <= 0) {
            p++
            continue
        }
        out |> push(cpts_to_utf8(c, p, p + l))
        p += l
    }
    return <- out
}

def private all_junk(text : string) : bool {
    var inscope c <- utf8_to_cpts(text)
    for (cp in c) {
        return false if (!key_exists(SUBTOKEN_JUNKS, cp))
    }
    return true
}

def private all_letters_lower_ascii(text : string) : bool {
    var inscope c <- utf8_to_cpts(text)
    for (cp in c) {
        let l = int(utf32_to_lower(uint(cp)))
        return false if (!(l >= 'a' && l <= 'z'))
    }
    return true
}

def private punct_chars(text : string) : string {
    var inscope c <- utf8_to_cpts(text)
    var inscope out <- [for (cp in c); cp; where key_exists(PUNCTS, cp)]
    return cpts_to_utf8(out)
}

def private make_token(text, tag : string; space_after : bool) : G2pToken {
    return G2pToken(text = text, tag = tag, space_after = space_after, phonemes = "", has_ps = false, rating = 0, is_head = true, prespace = false, alias = "")
}

//! the pieces' surfaces joined with the spaces between them (none after the last)
def private group_text(toks : array<G2pToken>; a, b : int) : string {
    return build_string() $(var w) {
        for (i in range(a, b)) {
            w |> write(toks[i].text)
            if (i + 1 < b && toks[i].space_after) {
                w |> write(" ")
            }
        }
    }
}

//! the reference pipeline's merge of a run of pieces: text joined with their spaces, the tag
//! of the most capitalized piece, phonemes joined honoring `prespace`
def private merge_tokens(toks : array<G2pToken>; a, b : int; with_phonemes : bool) : G2pToken {
    let text = group_text(toks, a, b)
    var best_tag = toks[a].tag
    var best_weight = -1
    var any_missing = false
    var min_rating = 99
    for (i in range(a, b)) {
        var weight = 0
        var inscope c <- utf8_to_cpts(toks[i].text)
        for (cp in c) {
            weight += int(utf32_to_lower(uint(cp))) == cp ? 1 : 2
        }
        if (weight > best_weight) {
            best_weight = weight
            best_tag = toks[i].tag
        }
        any_missing = any_missing || !toks[i].has_ps
        min_rating = min(min_rating, toks[i].rating)
    }
    var ps = ""
    if (with_phonemes) {
        ps = build_string() $(var w) {
            var written = false
            var last_space = false
            for (i in range(a, b)) {
                continue if (!toks[i].has_ps || empty(toks[i].phonemes))
                if (toks[i].prespace && written && !last_space) {
                    w |> write(" ")
                }
                w |> write(toks[i].phonemes)
                written = true
                last_space = ends_with(toks[i].phonemes, " ")
            }
        }
    }
    var m = make_token(text, best_tag, toks[b - 1].space_after)
    m.is_head = toks[a].is_head
    m.prespace = toks[a].prespace
    if (with_phonemes) {
        m.phonemes = ps
        m.has_ps = true
        m.rating = any_missing ? 0 : min_rating
    }
    return m
}

//! split every tagged token into the reference pipeline's pieces, resolve the punctuation
//! pieces on the spot, and mark which pieces open a glued group (`is_head`)
def private retokenize(toks : array<PosToken>) : array<G2pToken> {
    var out : array<G2pToken>
    var last_group_open = false
    for (tk in toks) {
        var inscope pieces <- subtokenize(tk.text)
        if (empty(pieces)) {
            pieces |> push(tk.text)
        }
        let n = length(pieces)
        for (j in range(n)) {
            var sub = make_token(pieces[j], tk.tag, j + 1 == n ? tk.space_after : false)
            if (tk.tag == ":" && (sub.text == "-" || sub.text == "–")) {   // nolint:STYLE039
                sub.phonemes = "—"   // nolint:STYLE039
                sub.has_ps = true
                sub.rating = 3
            } elif (key_exists(PUNCT_TAGS, tk.tag) && !all_letters_lower_ascii(sub.text)) {
                sub.phonemes = PUNCT_TAG_PHONEMES?[tk.tag] ?? punct_chars(sub.text)
                sub.has_ps = true
                sub.rating = 4
            } elif (j > 0 && j + 1 < n && sub.text == "2") {
                let prev_last = last_cp(pieces[j - 1])
                var inscope next_c <- utf8_to_cpts(pieces[j + 1])
                if (is_letter_cp(prev_last) && !empty(next_c) && is_letter_cp(next_c[0])) {
                    sub.alias = "to"
                }
            }
            if (sub.has_ps || !empty(sub.alias)) {
                out |> push(sub)
                last_group_open = false
            } elif (last_group_open) {
                sub.is_head = false
                out |> push(sub)
                last_group_open = !sub.space_after
            } else {
                out |> push(sub)
                last_group_open = !sub.space_after
            }
        }
    }
    return <- out
}

def private token_context(ctx : TokenContext; ps : string; has_ps : bool; tk : G2pToken) : TokenContext {
    var next = ctx
    if (has_ps && !empty(ps)) {
        var inscope c <- utf8_to_cpts(ps)
        for (cp in c) {
            if (key_exists(NON_QUOTE_PUNCTS, cp)) {
                next.future_vowel = -1
                break
            } elif (key_exists(VOWELS, cp)) {
                next.future_vowel = 1
                break
            } elif (key_exists(CONSONANTS, cp)) {
                next.future_vowel = 0
                break
            }
        }
    }
    next.future_to = tk.text == "to" || tk.text == "To" || (tk.text == "TO" && (tk.tag == "TO" || tk.tag == "IN"))
    next.next_text = lower_word(tk.text)
    next.next_tag = tk.tag
    return next
}

//! whether a group's pieces speak with spaces between them: the surface has a space or slash,
//! or mixes letters, digits and other characters
def private group_prespace(text : string) : bool {
    var inscope tc <- utf8_to_cpts(text)
    var has_alpha = false
    var has_digit = false
    var has_other = false
    var has_space_or_slash = false
    for (cp in tc) {
        has_space_or_slash = has_space_or_slash || cp == ' ' || cp == '/'
        continue if (key_exists(SUBTOKEN_JUNKS, cp))
        if (is_letter_cp(cp)) {
            has_alpha = true
        } elif (is_number(cp)) {
            has_digit = true
        } else {
            has_other = true
        }
    }
    let kinds = (has_alpha ? 1 : 0) + (has_digit ? 1 : 0) + (has_other ? 1 : 0)
    return has_space_or_slash || kinds > 1
}

//! the weaker half of a group's stressed pieces (by primary stress, weight, position) demotes
//! to secondary stress when more than half carry a primary
def private demote_weak_half(var toks : array<G2pToken>; a, b : int) {
    var inscope idx : array<int>
    var inscope prim : array<int>
    var inscope weight : array<int>
    for (i in range(a, b)) {
        continue if (!toks[i].has_ps || empty(toks[i].phonemes))
        idx |> push(i)
        prim |> push(has_primary(toks[i].phonemes) ? 1 : 0)
        weight |> push(stress_weight(toks[i].phonemes))
    }
    let n = length(idx)
    if (n == 2 && cp_count(toks[idx[0]].text) == 1) {
        toks[idx[1]].phonemes = apply_stress(toks[idx[1]].phonemes, -0.5)
        return
    }
    var n_primary = 0
    for (p in prim) {
        n_primary += p
    }
    return if (n < 2 || n_primary <= (n + 1) / 2)
    var inscope order <- [for (k in range(n)); k]
    order |> sort() $(x, y) {
        return prim[x] < prim[y] if (prim[x] != prim[y])
        return weight[x] < weight[y] if (weight[x] != weight[y])
        return idx[x] < idx[y]
    }
    for (k in range(n / 2)) {
        let i = idx[order[k]]
        toks[i].phonemes = apply_stress(toks[i].phonemes, -0.5)
    }
}

//! a group whose pieces resolved separately: junk pieces go silent, a trailing punctuation
//! piece speaks itself, pieces get their spacing, then the stress demotion
def private resolve_group(var toks : array<G2pToken>; a, b : int) {
    let prespace = group_prespace(group_text(toks, a, b))
    for (i in range(a, b)) {
        if (!toks[i].has_ps) {
            if (i + 1 == b && cp_count(toks[i].text) == 1 && key_exists(NON_QUOTE_PUNCTS, last_cp(toks[i].text))) {
                toks[i].phonemes = toks[i].text
                toks[i].has_ps = true
                toks[i].rating = 3
            } elif (all_junk(toks[i].text)) {
                toks[i].phonemes = ""
                toks[i].has_ps = true
                toks[i].rating = 3
            }
        } elif (i > a) {
            toks[i].prespace = prespace
        }
    }
    return if (prespace)
    demote_weak_half(toks, a, b)
}

//! resolve one glued group [a, b): the longest prefix the lexicon knows as a whole, repeated
//! on the rest; an unresolvable piece sends the WHOLE group to the fallback
def private resolve_pieces(g : G2pModel; var toks : array<G2pToken>; a, b : int; british : bool; var ctx : TokenContext&) {
    var left = a
    var right = b
    var should_fallback = false
    while (left < right) {
        var taken = false
        for (i in range(left, right)) {
            taken = taken || toks[i].has_ps || !empty(toks[i].alias)
        }
        var r = none_ps()
        var merged = make_token("", "", false)
        if (!taken) {
            merged = merge_tokens(toks, left, right, false)
            r = lexicon(g, merged, ctx, british)
        }
        if (r.ok) {
            toks[left].phonemes = r.ps
            toks[left].has_ps = true
            toks[left].rating = r.rating
            for (i in range(left + 1, right)) {
                toks[i].phonemes = ""
                toks[i].has_ps = true
                toks[i].rating = r.rating
            }
            ctx = token_context(ctx, r.ps, true, merged)
            right = left
            left = a
        } elif (left + 1 < right) {
            left++
        } else {
            right--
            if (!toks[right].has_ps) {
                if (all_junk(toks[right].text)) {
                    toks[right].phonemes = ""
                    toks[right].has_ps = true
                    toks[right].rating = 3
                } else {
                    should_fallback = true
                    break
                }
            }
            left = a
        }
    }
    if (should_fallback) {
        let merged = merge_tokens(toks, a, b, false)
        let r = fallback(g, merged.text, british)
        toks[a].phonemes = r.ps
        toks[a].has_ps = r.ok
        toks[a].rating = r.rating
        for (i in range(a + 1, b)) {
            toks[i].phonemes = ""
            toks[i].has_ps = true
            toks[i].rating = r.rating
        }
    } else {
        resolve_group(toks, a, b)
    }
}

//! Phonemize tagged tokens, one per input token: right to left so every word sees what follows
//! it, glued groups resolved as wholes first, the fallback chain for what the lexicon misses.
//! `british` reads the British half of every entry, its inflection rules and its rewrite.
def g2p_tokens(g : G2pModel; tagged : array<PosToken>; british : bool = false) : array<G2pToken> {
    var inscope pieces <- retokenize(tagged)
    let n = length(pieces)
    var ctx = TokenContext(future_vowel = -1, future_to = false)
    var b = n
    while (b > 0) {
        var a = b - 1
        while (a > 0 && !pieces[a].is_head) {
            a--
        }
        ctx.prev_text = a > 0 ? lower_word(pieces[a - 1].text) : ""
        ctx.prev_tag = a > 0 ? pieces[a - 1].tag : ""
        if (b - a == 1) {
            if (!pieces[a].has_ps) {
                let r = lexicon(g, pieces[a], ctx, british)
                if (r.ok) {
                    pieces[a].phonemes = r.ps
                    pieces[a].has_ps = true
                    pieces[a].rating = r.rating
                } else {
                    let fb = fallback(g, pieces[a].text, british)
                    pieces[a].phonemes = fb.ps
                    pieces[a].has_ps = fb.ok
                    pieces[a].rating = fb.rating
                }
            }
            ctx = token_context(ctx, pieces[a].phonemes, pieces[a].has_ps, pieces[a])
        } else {
            resolve_pieces(g, pieces, a, b, british, ctx)
        }
        b = a
    }
    var out : array<G2pToken>
    var i = 0
    while (i < n) {
        var j = i + 1
        while (j < n && !pieces[j].is_head) {
            j++
        }
        var m = j - i == 1 ? pieces[i] : merge_tokens(pieces, i, j, true)
        if (m.has_ps) {
            m.phonemes = replace(replace(m.phonemes, "ɾ", "T"), "ʔ", "t")   // nolint:STYLE039
        }
        out |> push(m)
        i = j
    }
    return <- out
}

//! Phonemes for normalized text, in the inventory `british` names: tokens joined with the
//! source spacing.
def g2p_phonemize(g : G2pModel; tg : PosTagger; text : string; british : bool = false) : string {
    var inscope tagged <- pos_tokenize_tag(tg, text)
    var inscope toks <- g2p_tokens(g, tagged, british)
    return build_string() $(var w) {
        for (tk in toks) {
            w |> write(tk.has_ps ? tk.phonemes : "")
            if (tk.space_after) {
                w |> write(" ")
            }
        }
    }
}
