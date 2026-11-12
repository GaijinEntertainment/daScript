options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_wordpiece shared public

require dasllama/dasllama_lint public
require daslib/fio
require dasllama/dasllama_gguf
require dasllama/dasllama_pretok public
require dasllama/dasllama_unicode
require daslib/algorithm
require math
require strings

//! The WordPiece backend: the BERT normalizer as the tables a converter folds the tokenizer's
//! flags into (the codepoint classes, the accent-strip and lowercase rewrite), the BERT
//! pre-tokenizer (`pretok_split_bert`), and the greedy longest-match WordPiece over the
//! vocabulary with the `##` continuation, a word with no split - or longer than the word cap -
//! one `[UNK]`. Every piece carries the codepoint span of the input it came from.

//! One piece: its vocabulary id and the codepoint span [start, end) of the input it covers -
//! (0, 0) for `[CLS]` and `[SEP]`.
struct WordPiece {
    id : int
    start : int
    end : int
}

//! A run of the byte-sorted pieces that share their first `depth` bytes: `sorted[lo .. hi)`.
struct WordPieceRun {
    lo : int
    hi : int
    depth : int
}

let WORDPIECE_DEFAULT_PREFIX = "##"
let WORDPIECE_DEFAULT_WORD_CHARS = 100

struct WordPieceTokenizer {
    bytes : array<uint8>        //! every piece's UTF-8 bytes end to end, in id order
    piece_at : array<int>       //! piece `id` is bytes[piece_at[id] .. piece_at[id + 1]): one entry more than the pieces
    sorted : array<int>         //! the piece ids ascending by their bytes
    prefix_run : WordPieceRun = WordPieceRun()
    class_starts : array<int>   //! the first codepoint of every class run, ascending
    class_kinds : array<BertCharKind>
    rewrite_from : array<int>   //! the codepoints the strip and the lowercasing rewrite, ascending
    rewrite_off : array<int>    //! codepoint i rewrites to rewrite_to[rewrite_off[i] .. rewrite_off[i + 1])
    rewrite_to : array<int>
    strip_accents : bool
    prefix : string = WORDPIECE_DEFAULT_PREFIX
    max_word_chars : int = WORDPIECE_DEFAULT_WORD_CHARS
    unk_id : int
    cls_id : int
    sep_id : int
    pad_id : int
}

let WORDPIECE_NO_BUDGET = 1 << 30   //! a `max_pieces` no input reaches: the pieces uncut

def private gguf_ints(m : GGUFMeta; bytes : array<uint8> | #; key : string) : array<int> {
    var inscope wide <- gguf_int_array(m, bytes, key)
    return <- [for (x in wide); int(x)]
}

//! How many pieces the vocabulary holds.
def wordpiece_count(tk : WordPieceTokenizer) : int => length(tk.sorted)

def private piece_len(tk : WordPieceTokenizer; id : int) : int => tk.piece_at[id + 1] - tk.piece_at[id]

//! pieces `a` and `b` by their bytes: negative, zero or positive as `a` sorts before, at or after `b`
def private piece_compare(tk : WordPieceTokenizer; a, b : int) : int =>
    bytes_compare(tk.bytes, int64(tk.piece_at[a]), piece_len(tk, a), tk.bytes, int64(tk.piece_at[b]), piece_len(tk, b))

//! `r` narrowed to its pieces whose next byte is `c`
[arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#wordpiece-pieces")]
def private run_step(tk : WordPieceTokenizer; var r : WordPieceRun&; c : int) {
    let depth = r.depth
    let hi = r.hi
    let lo = (r.lo < hi && piece_len(tk, tk.sorted[r.lo]) == depth) ? r.lo + 1 : r.lo
    let first = lower_bound(tk.sorted, lo, hi, c) $(id : int; next : int) => int(tk.bytes[tk.piece_at[id] + depth]) < next
    r.hi = upper_bound(tk.sorted, first, hi, c) $(next : int; id : int) => next < int(tk.bytes[tk.piece_at[id] + depth])
    r.lo = first
    r.depth++
}

//! the piece that is exactly the bytes `r` has taken, or -1
def private run_id(tk : WordPieceTokenizer; r : WordPieceRun) : int =>
    (r.lo < r.hi && piece_len(tk, tk.sorted[r.lo]) == r.depth) ? tk.sorted[r.lo] : -1

//! The id of the piece spelled `piece` - `continued`, of the piece spelled the `##` prefix then `piece` - or -1; the search allocates nothing.
[arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#wordpiece-pieces")]
def wordpiece_find(tk : WordPieceTokenizer; piece : string; continued : bool = false) : int {
    var r = continued ? tk.prefix_run : WordPieceRun(hi = length(tk.sorted))
    peek_data(piece) $(d) {
        for (c in d) {
            break if (r.lo >= r.hi)
            run_step(tk, r, int(c))
        }
    }
    return run_id(tk, r)
}

//! the byte order `sorted` claims, checked piece against piece; `prefix_run` set from `prefix`
def private wordpiece_index(var tk : WordPieceTokenizer; what : string) {
    let n = length(tk.piece_at) - 1
    if (length(tk.sorted) != n) {
        panic("dasLLAMA wordpiece: {what} lists {length(tk.sorted)} ids for {n} pieces - re-run the converter")
    }
    for (id, rank in tk.sorted, count()) {
        if (id < 0 || id >= n || (rank > 0 && piece_compare(tk, tk.sorted[rank - 1], id) >= 0)) {
            panic("dasLLAMA wordpiece: {what} is not the pieces' byte order at entry {rank} - re-run the converter")
        }
    }
    var behind_prefix = WordPieceRun(hi = n)
    peek_data(tk.prefix) $(d) {
        for (c in d) {
            run_step(tk, behind_prefix, int(c))
        }
    }
    tk.prefix_run = behind_prefix
}

//! A tokenizer over `pieces` in id order, every other field at its default: a hand-built vocabulary for a test or a tool.
[cold_path]
def wordpiece_from_pieces(pieces : array<string>) : WordPieceTokenizer {
    var tk = WordPieceTokenizer()
    tk.piece_at |> reserve(length(pieces) + 1)
    for (piece in pieces) {
        tk.piece_at |> push(length(tk.bytes))
        peek_data(piece) $(d) {
            for (c in d) {
                tk.bytes |> push(c)
            }
        }
    }
    tk.piece_at |> push(length(tk.bytes))
    tk.sorted <- [for (id in range(length(pieces))); id]
    sort(tk.sorted) $(a, b) => piece_compare(tk, a, b) < 0
    wordpiece_index(tk, "the hand-built vocabulary")
    return <- tk
}

//! The tokenizer a converted encoder GGUF carries (`harness/convert_ru_stress.py`): the vocabulary,
//! its special ids, the normalizer tables and the WordPiece settings under `<arch>.tokenizer.*`.
[arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#wordpiece-pieces")]
def wordpiece_of_gguf(m : GGUFMeta; bytes : array<uint8> | #; arch : string) : WordPieceTokenizer {
    if (gguf_str_or(m, bytes, "tokenizer.ggml.model", "") != "bert") {
        panic("dasLLAMA wordpiece: the GGUF's tokenizer.ggml.model is not bert")
    }
    let key = "{arch}.tokenizer"
    if (!gguf_has(m, "{key}.sorted_ids")) {
        panic("dasLLAMA wordpiece: the GGUF has no {key}.sorted_ids, the piece ids in the byte order of their pieces - convert the checkpoint again with harness/convert_ru_stress.py")
    }
    var tk = WordPieceTokenizer()
    var inscope pieces <- gguf_str_array_blob(m, bytes, "tokenizer.ggml.tokens")
    tk.bytes <- pieces.bytes
    tk.piece_at <- pieces.at
    tk.sorted <- gguf_ints(m, bytes, "{key}.sorted_ids")
    tk.prefix = gguf_str_or(m, bytes, "{key}.continuing_prefix", WORDPIECE_DEFAULT_PREFIX)
    wordpiece_index(tk, "{key}.sorted_ids")
    tk.class_starts <- gguf_ints(m, bytes, "{key}.class_starts")
    var inscope kinds <- gguf_ints(m, bytes, "{key}.class_kinds")
    let by_number = fixed_array(BertCharKind.keep, BertCharKind.drop, BertCharKind.space, BertCharKind.punct, BertCharKind.cjk)
    tk.class_kinds |> reserve(length(kinds))
    for (k in kinds) {
        if (k < 0 || k >= length(by_number)) {
            panic("dasLLAMA wordpiece: {key}.class_kinds holds {k}, a class the reader does not know")
        }
        tk.class_kinds |> push(by_number[k])
    }
    tk.rewrite_from <- gguf_ints(m, bytes, "{key}.rewrite_from")
    tk.rewrite_off <- gguf_ints(m, bytes, "{key}.rewrite_off")
    tk.rewrite_to <- gguf_ints(m, bytes, "{key}.rewrite_to")
    if (empty(tk.class_starts) || length(tk.class_starts) != length(tk.class_kinds) || length(tk.rewrite_off) != length(tk.rewrite_from) + 1) {
        panic("dasLLAMA wordpiece: the {key} tables disagree in length - re-run the converter")
    }
    tk.strip_accents = gguf_bool_or(m, bytes, "{key}.strip_accents", false)
    tk.max_word_chars = int(gguf_int_or(m, bytes, "{key}.max_word_chars", int64(WORDPIECE_DEFAULT_WORD_CHARS)))
    tk.unk_id = int(gguf_int(m, bytes, "tokenizer.ggml.unknown_token_id"))
    tk.cls_id = int(gguf_int(m, bytes, "tokenizer.ggml.cls_token_id"))
    tk.sep_id = int(gguf_int(m, bytes, "tokenizer.ggml.seperator_token_id"))
    tk.pad_id = int(gguf_int_or(m, bytes, "tokenizer.ggml.padding_token_id", 0l))
    return <- tk
}

//! The tokenizer of an encoder GGUF alone, its tensors left unread.
[cold_path]
def load_wordpiece_gguf(path : string; arch : string) : WordPieceTokenizer {
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA wordpiece: cannot open gguf '{path}'")
    }
    var tk = WordPieceTokenizer()
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        delete tk
        tk <- wordpiece_of_gguf(m, bytes, arch)
    }
    fclose(f)
    return <- tk
}

def private class_of(tk : WordPieceTokenizer; cpt : int) : BertCharKind => tk.class_kinds[max(upper_bound(tk.class_starts, cpt) - 1, 0)]

def private rewrite_index(tk : WordPieceTokenizer; cpt : int) : int {
    let at = lower_bound(tk.rewrite_from, cpt)
    return (at < length(tk.rewrite_from) && tk.rewrite_from[at] == cpt) ? at : -1
}

let private HANGUL_FIRST : int = 0xAC00
let private HANGUL_LAST : int = 0xD7A3
let private JAMO_L : int = 0x1100
let private JAMO_V : int = 0x1161
let private JAMO_T : int = 0x11A7
let private HANGUL_T_COUNT = 28
let private HANGUL_VT_COUNT = 588

//! The normalized codepoints as their UTF-8 bytes, the input index each came from and the class the pre-tokenizer reads.
struct private WordPieceNorm {
    src : array<int>
    kinds : array<BertCharKind>
    utf8 : array<uint8>
    at : array<int>     //! codepoint `i` is utf8[at[i] .. at[i + 1]): one entry more than the codepoints
}

def private norm_push(var nm : WordPieceNorm; cpt, src : int; kind : BertCharKind) {
    nm.src |> push(src)
    nm.kinds |> push(kind)
    nm.at |> push(length(nm.utf8))
    cpt_utf8_push(nm.utf8, cpt)
}

def private hangul_push(var nm : WordPieceNorm; cpt, src : int) {
    let s = cpt - HANGUL_FIRST
    norm_push(nm, JAMO_L + s / HANGUL_VT_COUNT, src, BertCharKind.keep)
    norm_push(nm, JAMO_V + (s % HANGUL_VT_COUNT) / HANGUL_T_COUNT, src, BertCharKind.keep)
    if (s % HANGUL_T_COUNT != 0) {
        norm_push(nm, JAMO_T + s % HANGUL_T_COUNT, src, BertCharKind.keep)
    }
}

[arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#wordpiece-tables")]
def private wordpiece_normalize(tk : WordPieceTokenizer; cpts : array<int>) : WordPieceNorm {
    var nm : WordPieceNorm
    let n = length(cpts)
    nm.src |> reserve(n)
    nm.kinds |> reserve(n)
    nm.at |> reserve(n + 1)
    nm.utf8 |> reserve(n * 2)
    for (cpt, i in cpts, count()) {
        let kind = class_of(tk, cpt)
        continue if (kind == BertCharKind.drop)
        if (kind == BertCharKind.space) {
            norm_push(nm, ' ', i, kind)
            continue
        }
        if (tk.strip_accents && cpt >= HANGUL_FIRST && cpt <= HANGUL_LAST) {
            hangul_push(nm, cpt, i)
            continue
        }
        let r = rewrite_index(tk, cpt)
        if (r < 0) {
            norm_push(nm, cpt, i, kind)
            continue
        }
        for (j in range(tk.rewrite_off[r], tk.rewrite_off[r + 1])) {
            let out = tk.rewrite_to[j]
            norm_push(nm, out, i, kind == BertCharKind.cjk ? kind : class_of(tk, out))
        }
    }
    nm.at |> push(length(nm.utf8))
    return <- nm
}

//! the longest piece that spells codepoints [start, stop) of `nm` for some stop in (start, limit] - behind the `##` prefix where `continued`; id -1 where none does
[arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#wordpiece-pieces")]
def private longest_piece(tk : WordPieceTokenizer; nm : WordPieceNorm; start, limit : int; continued : bool) : tuple<id : int; stop : int> {
    var r = continued ? tk.prefix_run : WordPieceRun(hi = length(tk.sorted))
    var best = (id = -1, stop = start)
    var o = nm.at[start]
    for (c in range(start, limit)) {
        let e = nm.at[c + 1]
        while (o < e && r.lo < r.hi) {
            run_step(tk, r, int(nm.utf8[o]))
            o++
        }
        break if (r.lo >= r.hi)
        let id = run_id(tk, r)
        if (id >= 0) {
            best = (id = id, stop = c + 1)
        }
    }
    return best
}

def private word_pieces(tk : WordPieceTokenizer; nm : WordPieceNorm; word : int2; var out : array<WordPiece>) {
    let first = length(out)
    if (word.y - word.x <= tk.max_word_chars) {
        var start = word.x
        while (start < word.y) {
            let hit = longest_piece(tk, nm, start, word.y, start > word.x)
            break if (hit.id < 0)
            out |> push(WordPiece(id = hit.id, start = nm.src[start], end = nm.src[hit.stop - 1] + 1))
            start = hit.stop
        }
        return if (start == word.y)
    }
    out |> resize(first)
    out |> push(WordPiece(id = tk.unk_id, start = nm.src[word.x], end = nm.src[word.y - 1] + 1))
}

//! The model input of a codepoint run: `[CLS]`, the pieces - cut so the whole stays within
//! `max_pieces` - and `[SEP]`, each piece's span in codepoints of `cpts`.
def wordpiece_encode(tk : WordPieceTokenizer; cpts : array<int>; max_pieces : int) : array<WordPiece> {
    var inscope nm <- wordpiece_normalize(tk, cpts)
    var inscope words <- pretok_split_bert(nm.kinds)
    var pieces : array<WordPiece>
    pieces |> push(WordPiece(id = tk.cls_id))
    for (w in words) {
        word_pieces(tk, nm, w, pieces)
        break if (length(pieces) >= max_pieces - 1)
    }
    if (length(pieces) > max_pieces - 1) {
        pieces |> resize(max_pieces - 1)
    }
    pieces |> push(WordPiece(id = tk.sep_id))
    return <- pieces
}

//! The piece an id names, `[UNK]`'s spelling past the vocabulary: a string made at every call, for a log or a test.
[cold_path]
def wordpiece_piece(tk : WordPieceTokenizer; id : int) : string {
    let shown_id = (id >= 0 && id < wordpiece_count(tk)) ? id : tk.unk_id
    return bytes_str(tk.bytes, int64(tk.piece_at[shown_id]), piece_len(tk, shown_id))
}

//! Whether `p` continues the word of the piece before it: a piece of the input spelled with the `##` prefix.
def is_continuation(tk : WordPieceTokenizer; p : WordPiece) : bool {
    return false if (p.end <= p.start || p.id < 0 || p.id >= wordpiece_count(tk))
    var continues = false
    peek_data(tk.prefix) $(d) {
        let n = length(d)
        continues = piece_len(tk, p.id) >= n && bytes_compare(tk.bytes, int64(tk.piece_at[p.id]), n, d, 0l, n) == 0
    }
    return continues
}
