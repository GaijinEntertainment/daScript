options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_g2p_ru_stress shared public

require dasllama/dasllama_lint public
require daslib/fio
require strings
require dasllama/dasllama_gguf
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema
require dasllama/dasllama_convert
require dasllama/dasllama_common
require dasllama/dasllama_load
require dasllama/dasllama_math
require dasllama/dasllama_unicode
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_wordpiece public
require dasllama/dasllama_g2p_ru

//! The Russian stress-context model: a BERT or ELECTRA encoder reads the lowercased sentence
//! through its own WordPiece tokenizer, the pieces covering one written word are mean-pooled,
//! and a linear head scores the word's readings: class k of the first `RU_STRESS_ORDINALS` is the
//! stress on the word's k-th vowel, and a head of `RU_STRESS_CLASSES` adds as many classes for that
//! vowel read as ё. The pick is masked to the classes the word's form allows. The GGUF is
//! `harness/convert_ru_stress.py`'s: the encoder, the head, the tokenizer and the catalog of
//! forms with their allowed classes. CPU only; a layer the file stores in a K-quant format serves
//! its own planes, an f32 layer its planes or, on the q8 lane, Q8_0 quants minted at load.

let RU_STRESS_ORDINALS = 8   //! the stress classes: the stressed vowel's ordinal 1..8
let RU_STRESS_CLASSES = 16   //! the widest head: the stress classes, then each one's vowel read as ё

struct RuStressLayer {
    q : TtsLinear = TtsLinear()
    k : TtsLinear = TtsLinear()
    v : TtsLinear = TtsLinear()
    dense : TtsLinear = TtsLinear()
    attn_ln : TtsNorm = TtsNorm()
    ffn : TtsLinear = TtsLinear()
    ffn_out : TtsLinear = TtsLinear()
    full_ln : TtsNorm = TtsNorm()
}

//! How the word-embedding table is stored: the file's f32 rows, its Q8_0 blocks, or a K-quant or per-32 plane pair.
enum RuStressTableForm {
    f32
    q8_0
    kq
}

//! The word-embedding table in the form the file stores it.
struct RuStressWordTable {
    form : RuStressTableForm = RuStressTableForm.f32
    f32 : array<float>
    @exact_size q8_0 : array<uint8>
    @exact_size kq : array<uint8>
    @exact_size ks : array<uint8>
    fmt : KqFmt = KqFmt.q8
    rows : int64
}

struct RuStressModel {
    words : RuStressWordTable = RuStressWordTable()
    pos_emb : array<float>
    type_emb : array<float>
    emb_ln : TtsNorm = TtsNorm()
    map_in : TtsLinear = TtsLinear()   //! ELECTRA's embedding projection; `nout == 0` where the embedding is the hidden width
    layers : array<RuStressLayer>
    head : TtsLinear = TtsLinear()
    tok : WordPieceTokenizer = WordPieceTokenizer()
    catalog : table<string; uint>   //! written form -> its allowed classes, bit i = class i + 1
    classes : int                   //! the head's width: `RU_STRESS_ORDINALS` or `RU_STRESS_CLASSES`
    encoder : string
    emb_dim : int64
    hidden_dim : int64
    heads : int64
    max_pos : int64
    max_pieces : int
    eps : float
    gelu_form : TtsGelu = TtsGelu.erf
    q8 : bool
}

//! The activations one sentence's pass sizes and every pass reuses.
struct RuStressScratch {
    @scratch @exact_size emb : array<float>
    @scratch @exact_size word_rows : array<float>   //! the pass's pieces' word rows decoded from a quantized table
    @scratch @exact_size row_ids : array<int>       //! 0 .. n - 1: the pieces' rows of `word_rows`
    @scratch @exact_size hidden : array<float>   //! the last pass's last hidden state [n_pieces][hidden_dim]
    @scratch @exact_size pooled : array<float>
    @scratch @exact_size logits : array<float>
    pieces : array<WordPiece>                    //! the last pass's pieces, `[CLS]` and `[SEP]` included
    layer : TtsBertScratch = TtsBertScratch()
}

//! One word's answer: the head's logits over its classes and the allowed class they rank first,
//! counted from 1 - 0 when no piece of the pass covers the word's span. `ru_stress_class_pick`
//! turns the class into the pick the stress pass marks.
struct RuStressPick {
    ordinal : int
    logits : float[RU_STRESS_CLASSES]
}

//! The pick of a head class counted from 1: a stress class is its vowel's ordinal, a class past them that vowel's ё pick.
[arch(at = "../ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-yo-pick")]
def ru_stress_class_pick(cls : int) : int => cls <= RU_STRESS_ORDINALS ? cls : RU_PICK_YO + cls - RU_STRESS_ORDINALS

def private need_tensor(m : GGUFMeta; name : string) : int => gguf_need_tensor(m, name, "ru_stress")

def private read_arr(m : GGUFMeta; bytes : array<uint8> | #; name : string) : array<float> =>
    <- gguf_read_f32_arr(m, bytes, name, "ru_stress")

//! the superblock format a layer weight not stored f32 serves its own planes in on its `width`-wide rows; any other form is refused
def private layer_kq_fmt(m : GGUFMeta; name : string; width : int64) : KqFmt {
    let gt = gguf_tensor_type(m, name)
    let f = kq_fmt_row_ok(kq_fmt_of(gt), width)
    if (!kq_sb(f) || f == KqFmt.nv4 || width % KQ_SB_ELEMS != 0l) {
        panic("dasLLAMA ru_stress: '{name}' is {ggml_type_name(gt)} on a {width}-wide row (format {f}), which the stress encoder's layer GEMMs do not serve - they serve f32 and the 256-superblock formats on rows of whole superblocks; convert the layer again (harness/convert_ru_stress.py --quant)")
    }
    return f
}

def private read_linear(m : GGUFMeta; bytes : array<uint8> | #; prefix : string; q8, repack : bool) : TtsLinear {
    var l = TtsLinear()
    let wname = "{prefix}.weight"
    let ti = need_tensor(m, wname)
    l.nin = m.tensors[ti].dims[0]
    l.nout = m.tensors[ti].dims[1]
    l.b <- read_arr(m, bytes, "{prefix}.bias")
    if (m.tensors[ti].ggml_type == GGML_TYPE_F32) {
        l.w <- read_arr(m, bytes, wname)
        linear_prepare(l, true, q8, repack)
    } else {
        linear_read_kq(l, m, bytes, wname, layer_kq_fmt(m, wname, l.nin))
    }
    return <- l
}

def private read_norm(m : GGUFMeta; bytes : array<uint8> | #; prefix : string) : TtsNorm =>
    TtsNorm(w <- read_arr(m, bytes, "{prefix}.weight"), b <- read_arr(m, bytes, "{prefix}.bias"))

def private read_layer(m : GGUFMeta; bytes : array<uint8> | #; i : int64; q8, repack : bool) : RuStressLayer {
    let p = "bert.layer.{i}"
    return RuStressLayer(
        q <- read_linear(m, bytes, "{p}.attention.query", q8, repack),
        k <- read_linear(m, bytes, "{p}.attention.key", q8, repack),
        v <- read_linear(m, bytes, "{p}.attention.value", q8, repack),
        dense <- read_linear(m, bytes, "{p}.attention.dense", q8, repack),
        attn_ln <- read_norm(m, bytes, "{p}.attention.LayerNorm"),
        ffn <- read_linear(m, bytes, "{p}.ffn", q8, repack),
        ffn_out <- read_linear(m, bytes, "{p}.ffn_output", q8, repack),
        full_ln <- read_norm(m, bytes, "{p}.full_layer_layer_norm"))
}

//! the word-embedding table of `width`-wide rows in the form the row gather reads it: f32 rows, Q8_0 blocks, or the plane pair of a format whose blocks divide the row; any other form is refused
def private read_word_table(m : GGUFMeta; bytes : array<uint8> | #; width : int64) : RuStressWordTable {
    let name = "bert.embeddings.word_embeddings.weight"
    var words = RuStressWordTable(rows = m.tensors[need_tensor(m, name)].dims[1])
    let gt = gguf_tensor_type(m, name)
    if (gt == GGML_TYPE_F32) {
        words.f32 <- read_arr(m, bytes, name)
        return <- words
    }
    let f = kq_fmt_row_ok(kq_fmt_of(gt), width)
    if (gt == GGML_TYPE_Q8_0 && width % Q8_BLOCK_ELEMS == 0l) {
        words.form = RuStressTableForm.q8_0
        words.q8_0 |> reserve_resize(words.rows * width / Q8_BLOCK_ELEMS * Q8_BLOCK_BYTES)
        gguf_read_tensor_raw(m, bytes, name, words.q8_0)
        return <- words
    }
    if (f == KqFmt.q8 || f == KqFmt.nv4 || width % kq_elems(f) != 0l) {
        panic("dasLLAMA ru_stress: '{name}' is {ggml_type_name(gt)} on a {width}-wide row (format {f}), which the stress encoder's word-table row gather does not read - it reads f32, Q8_0 and a format whose blocks divide the row; convert the table again (harness/convert_ru_stress.py --emb-quant)")
    }
    words.form = RuStressTableForm.kq
    words.fmt = f
    let n = words.rows * width
    words.kq |> reserve_resize(n / kq_elems(f) * kq_qsb(f))
    words.ks |> reserve_resize(n / kq_elems(f) * kq_ssb(f))
    gguf_transcode_kq(m, bytes, name, f, words.kq, words.ks, 0l, n)
    return <- words
}

def private read_catalog(m : GGUFMeta; bytes : array<uint8> | #) : table<string; uint> {
    var none : table<string; uint>
    return <- none if (!gguf_has(m, "ru_stress.catalog.forms"))
    var inscope forms <- gguf_str_array(m, bytes, "ru_stress.catalog.forms")
    var inscope masks <- gguf_int_array(m, bytes, "ru_stress.catalog.ordinals")
    if (length(forms) != length(masks)) {
        panic("dasLLAMA ru_stress: the catalog lists {length(forms)} forms and {length(masks)} ordinal masks")
    }
    return <- {for (form, mask in forms, masks); form => uint(mask)}
}

def private read_model(m : GGUFMeta; bytes : array<uint8> | #; q8 : bool) : RuStressModel {
    if (gguf_str(m, bytes, "general.architecture") != "ru_stress") {
        panic("dasLLAMA ru_stress: the GGUF's architecture is '{gguf_str(m, bytes, "general.architecture")}', not ru_stress")
    }
    let repack = q8 && select_matmul_backend_for_load_()
    var out = RuStressModel()
    out.q8 = q8
    out.encoder = gguf_str(m, bytes, "ru_stress.encoder")
    out.emb_dim = gguf_int(m, bytes, "ru_stress.emb_dim")
    out.hidden_dim = gguf_int(m, bytes, "ru_stress.hidden")
    out.heads = gguf_int(m, bytes, "ru_stress.n_heads")
    out.max_pos = gguf_int(m, bytes, "ru_stress.max_positions")
    out.max_pieces = int(min(gguf_int(m, bytes, "ru_stress.max_pieces"), out.max_pos))
    out.eps = gguf_f32(m, bytes, "ru_stress.layer_norm_eps")
    let act = gguf_str(m, bytes, "ru_stress.activation")
    if (act != "gelu" && act != "gelu_new") {
        panic("dasLLAMA ru_stress: activation '{act}' - the encoder runs gelu (erf) or gelu_new (tanh)")
    }
    out.gelu_form = act == "gelu" ? TtsGelu.erf : TtsGelu.tanh
    out.classes = int(gguf_int(m, bytes, "ru_stress.n_ordinals"))
    if (out.classes != RU_STRESS_ORDINALS && out.classes != RU_STRESS_CLASSES) {
        panic("dasLLAMA ru_stress: the head scores {out.classes} classes, the reader {RU_STRESS_ORDINALS} or {RU_STRESS_CLASSES}")
    }
    out.words <- read_word_table(m, bytes, out.emb_dim)
    out.pos_emb <- read_arr(m, bytes, "bert.embeddings.position_embeddings.weight")
    out.type_emb <- read_arr(m, bytes, "bert.embeddings.token_type_embeddings.weight")
    out.emb_ln <- read_norm(m, bytes, "bert.embeddings.LayerNorm")
    if (gguf_find_tensor(m, "bert.encoder.embedding_hidden_mapping_in.weight") >= 0) {
        out.map_in <- read_linear(m, bytes, "bert.encoder.embedding_hidden_mapping_in", q8, repack)
    }
    let n_layers = gguf_int(m, bytes, "ru_stress.n_layers")
    out.layers |> reserve(int(n_layers))
    for (i in range64(n_layers)) {
        out.layers |> emplace(read_layer(m, bytes, i, q8, repack))
    }
    out.head <- TtsLinear(w <- read_arr(m, bytes, "stress_head.weight"), b <- read_arr(m, bytes, "stress_head.bias"),
        nin = out.hidden_dim, nout = int64(out.classes))
    out.tok <- wordpiece_of_gguf(m, bytes, "ru_stress")
    out.catalog <- read_catalog(m, bytes)
    return <- out
}

//! Load a converted stress-context GGUF (`harness/convert_ru_stress.py`). A K-quant layer serves the
//! file's own planes; an f32 layer its planes, or with `q8` Q8_0 quants minted at load.
def load_ru_stress(path : string; q8 : bool = false) : RuStressModel {
    var m = RuStressModel()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA ru_stress: cannot open model '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope meta <- parse_gguf_meta(bytes)
        delete m
        m <- read_model(meta, bytes, q8)
    }
    fclose(f)
    return <- m
}

//! Whether `m` holds a loaded model - the facade's empty one where no context model sits beside the voice.
def ru_stress_loaded(m : RuStressModel) : bool => !empty(m.layers)

//! The head classes the catalog allows a written form (lowercase), bit i = class i + 1; 0 for a form it does not list.
def ru_stress_allowed(m : RuStressModel; form : string) : uint => m.catalog?[form] ?? 0u

//! The embeddings of `ids` into `sc.emb` [n][emb_dim].
[arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#tts-stress-lanes")]
def ru_stress_embed(m : RuStressModel; ids : array<int>; var sc : RuStressScratch) {
    let w & = unsafe(m.words)
    if (w.form == RuStressTableForm.f32) {
        bert_embed_rows(ids, w.f32, m.pos_emb, m.type_emb, m.emb_ln, m.eps, m.emb_dim, sc.emb)
        return
    }
    let t = long_length(ids)
    unsafe(scratch_resize(sc.word_rows, t * m.emb_dim))
    unsafe(scratch_resize(sc.row_ids, t))
    let row_blocks = m.emb_dim / Q8_BLOCK_ELEMS
    for (i in range64(t)) {
        let r = int64(ids[i])
        if (r < 0l || r >= w.rows) {
            panic("dasLLAMA ru_stress: piece id {r} past the {w.rows}-row embedding table")
        }
        sc.row_ids[i] = int(i)
        if (w.form == RuStressTableForm.q8_0) {
            unsafe {
                dequant_q8_blocks(addr<uint8 const?>(w.q8_0[r * row_blocks * Q8_BLOCK_BYTES]), row_blocks, addr(sc.word_rows[i * m.emb_dim]))
            }
        } else {
            kq_plane_row_f32(w.fmt, w.kq, w.ks, m.emb_dim, w.rows, 1l, r, sc.word_rows, i * m.emb_dim)
        }
    }
    bert_embed_rows(sc.row_ids, sc.word_rows, m.pos_emb, m.type_emb, m.emb_ln, m.eps, m.emb_dim, sc.emb)
}

//! The embeddings of `ids` as the first layer's input, `sc.hidden` [n][hidden_dim]: through ELECTRA's projection where the model has one.
def ru_stress_embed_hidden(m : RuStressModel; ids : array<int>; var sc : RuStressScratch) {
    let t = long_length(ids)
    ru_stress_embed(m, ids, sc)
    if (m.map_in.nout > 0l) {
        linear_rows(m.map_in, sc.emb, t, sc.hidden)
        return
    }
    unsafe(scratch_resize(sc.hidden, t * m.hidden_dim))
    for (i in range64(t * m.hidden_dim)) {
        sc.hidden[i] = sc.emb[i]
    }
}

//! the multiply-adds of one layer's GEMMs a lane takes before another lane pays for meeting the team
let RU_STRESS_LANE_WORK = 16777216l
let RU_STRESS_MIN_LANES = 8l

var private g_ru_stress_lane_work = RU_STRESS_LANE_WORK
var private g_ru_stress_min_lanes = RU_STRESS_MIN_LANES

//! Probe seam: the lane work and the lane floor the pass narrows the pool by, in place of the shipped pair (0 restores either), so a race times other values in one process.
def set_ru_stress_lanes_probe_(lane_work, min_lanes : int64) {
    g_ru_stress_lane_work = lane_work > 0l ? lane_work : RU_STRESS_LANE_WORK
    g_ru_stress_min_lanes = min_lanes > 0l ? min_lanes : RU_STRESS_MIN_LANES
}

//! the lanes a pass over `t` pieces narrows the pool to
def ru_stress_lanes(m : RuStressModel; t : int64) : int {
    let inter = empty(m.layers) ? 0l : m.layers[0].ffn.nout
    return step_lanes(t * m.hidden_dim * (4l * m.hidden_dim + 2l * inter), g_ru_stress_lane_work, g_ru_stress_min_lanes)
}

//! The encoder over token ids ([CLS] ... [SEP]) into `sc.hidden` [n][hidden_dim], n = length(ids).
[hot_path, arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#tts-stress-lanes")]
def ru_stress_forward(m : RuStressModel; ids : array<int>; var sc : RuStressScratch) {
    let t = long_length(ids)
    if (t > m.max_pos) {
        panic("dasLLAMA ru_stress: {t} pieces exceed the encoder's {m.max_pos} positions")
    }
    let pool = dispatch_lanes_narrow(ru_stress_lanes(m, t))
    ru_stress_embed_hidden(m, ids, sc)
    for (lw in m.layers) {
        bert_layer_rows(lw, m.heads, m.eps, m.gelu_form, sc.hidden, t, sc.layer)
    }
    dispatch_lanes_restore(pool)
}

//! The model input of `sentence`: lowercased codepoint for codepoint, the form the model reads
//! it, then `wordpiece_encode` - every span in codepoints of `sentence`.
def ru_stress_pieces(tk : WordPieceTokenizer; sentence : string; max_pieces : int) : array<WordPiece> {
    var inscope cpts <- utf8_to_cpts(utf8_to_lower(sentence))
    return <- wordpiece_encode(tk, cpts, max_pieces)
}

//! run the encoder over `pieces`, which become `sc.pieces`; the piece count
def private run_pieces(m : RuStressModel; var pieces : array<WordPiece>; var sc : RuStressScratch) : int {
    delete sc.pieces
    sc.pieces <- pieces
    var inscope ids <- [for (p in sc.pieces); p.id]
    ru_stress_forward(m, ids, sc)
    return length(ids)
}

//! Tokenize `sentence` into `sc.pieces`, cut to the model's piece budget, and run the encoder
//! over them; the piece count.
def ru_stress_encode(m : RuStressModel; sentence : string; var sc : RuStressScratch) : int {
    var inscope pieces <- ru_stress_pieces(m.tok, sentence, m.max_pieces)
    return run_pieces(m, pieces, sc)
}

//! The head over the last pass's pieces that overlap `span` (codepoints [x, y) of the sentence),
//! mean-pooled, its pick among the classes `allowed` sets (every class of the head when 0).
def ru_stress_pick_span(m : RuStressModel; span : int2; allowed : uint; var sc : RuStressScratch) : RuStressPick {
    var pick = RuStressPick()
    unsafe(scratch_resize(sc.pooled, m.hidden_dim))
    for (d in range64(m.hidden_dim)) {
        sc.pooled[d] = 0.0
    }
    var n = 0
    for (p, row in sc.pieces, range64(long_length(sc.pieces))) {
        continue if (p.end <= p.start || p.start >= span.y || p.end <= span.x)
        for (d in range64(m.hidden_dim)) {
            sc.pooled[d] += sc.hidden[row * m.hidden_dim + d]
        }
        n++
    }
    return pick if (n == 0)
    for (d in range64(m.hidden_dim)) {
        sc.pooled[d] /= float(n)
    }
    linear_vec(m.head, sc.pooled, sc.logits)
    let mask = allowed == 0u ? (1u << uint(m.classes)) - 1u : allowed
    for (o in range(m.classes)) {
        pick.logits[o] = sc.logits[o]
        if ((mask & (1u << uint(o))) != 0u && (pick.ordinal == 0 || sc.logits[o] > pick.logits[pick.ordinal - 1])) {
            pick.ordinal = o + 1
        }
    }
    return pick
}

def private need_mask_per_span(spans : array<int2>; allowed : array<uint>) {
    if (length(spans) != length(allowed)) {
        panic("dasLLAMA ru_stress: {length(spans)} word spans and {length(allowed)} ordinal masks")
    }
}

//! Every word of one sentence from ONE encoder pass: `spans[i]` (codepoints [x, y)) picked among
//! the classes `allowed[i]` sets.
def ru_stress_pick_words(m : RuStressModel; sentence : string; spans : array<int2>; allowed : array<uint>; var sc : RuStressScratch) : array<RuStressPick> {
    need_mask_per_span(spans, allowed)
    ru_stress_encode(m, sentence, sc)
    return <- [for (span, mask in spans, allowed); ru_stress_pick_span(m, span, mask, sc)]
}

//! One word of a sentence: `ru_stress_pick_words` for one span.
[cold_path]
def ru_stress_pick(m : RuStressModel; sentence : string; span : int2; allowed : uint; var sc : RuStressScratch) : RuStressPick {
    ru_stress_encode(m, sentence, sc)
    return ru_stress_pick_span(m, span, allowed, sc)
}

//! the pieces of `all` (`[CLS]` at 0, `[SEP]` last) a span overlaps, as [a, b); (0, 0) where none does
def private pieces_of(all : array<WordPiece>; span : int2) : int2 {
    var a = 0
    var b = 0
    for (p, i in all, count()) {
        continue if (p.end <= p.start || p.start >= span.y || p.end <= span.x)
        a = a == 0 ? i : a
        b = i + 1
    }
    return int2(a, b)
}

//! The window of pieces [s, e) around pieces [a, b) inside `body` pieces at most `budget` long, both edges on word starts.
def ru_stress_window(tk : WordPieceTokenizer; all : array<WordPiece>; ab : int2; budget, body : int) : int2 {
    var s = clamp(ab.x - (budget - (ab.y - ab.x)) / 2, 1, max(1, body + 1 - budget))
    var e = min(s + budget, body + 1)
    while (s < ab.x && is_continuation(tk, all[s])) {
        s++
    }
    while (e > ab.y && e <= body && is_continuation(tk, all[e])) {
        e--
    }
    return int2(s, e)
}

//! `ru_stress_pick_words` for a sentence of any length.
[arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#tts-stress-context")]
def ru_stress_pick_long(m : RuStressModel; sentence : string; spans : array<int2>; allowed : array<uint>; var sc : RuStressScratch) : array<RuStressPick> {
    need_mask_per_span(spans, allowed)
    var inscope all <- ru_stress_pieces(m.tok, sentence, WORDPIECE_NO_BUDGET)
    if (length(all) <= m.max_pieces) {
        run_pieces(m, all, sc)
        return <- [for (span, mask in spans, allowed); ru_stress_pick_span(m, span, mask, sc)]
    }
    let body = length(all) - 2
    let budget = m.max_pieces - 2
    var picks : array<RuStressPick>
    picks |> resize(length(spans))
    var inscope done : array<bool>
    done |> resize(length(spans))
    var inscope cover <- [for (span in spans); pieces_of(all, span)]
    var inscope cps <- utf8_to_cpts(sentence)
    for (k in range(length(spans))) {
        continue if (done[k])
        let ab = cover[k]
        if (ab.y == 0) {
            done[k] = true
            continue
        }
        let w = ru_stress_window(m.tok, all, ab, budget, body)
        let at = all[w.x].start
        var inscope members <- [for (j in range(k, length(spans))); j; where !done[j] && cover[j].y > 0 && cover[j].x >= w.x && cover[j].y <= w.y]
        var inscope sub <- [for (j in members); spans[j] - int2(at, at)]
        var inscope masks <- [for (j in members); allowed[j]]
        var inscope got <- ru_stress_pick_words(m, cpts_to_utf8(cps, at, all[w.y - 1].end), sub, masks, sc)
        for (j, g in members, got) {
            picks[j] = g
            done[j] = true
        }
    }
    return <- picks
}

//! What a context pass over a text decided: the ordinal picked per word (its index among `ru_text_words`), and the counts
//! behind it - the words the model was asked about, the picks that are a reading the
//! catalog lists and the pack does not, and the words no piece covered, which keep the pack's default.
struct RuContextPicks {
    picked : table<int; int>
    asked : int
    beyond_pack : int
    fallbacks : int
}

//! The context tier's picks over the words `words` (their `ru_text_words` spans) of normalized Russian text `cps`.
[arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#tts-stress-context")]
def ru_stress_context_picks(m : RuStressModel; p : RuStressPack; cps : array<int>; words : array<int2>; var sc : RuStressScratch) : RuContextPicks {
    var res : RuContextPicks
    var inscope lowers <- [for (w in words); utf8_to_lower(cpts_to_utf8(cps, w.x, w.y))]
    var inscope sentences <- ru_word_sentences(cps, words)
    var sentence_start = 0
    var k = 0
    while (k < length(words)) {
        let sentence = sentences[k]
        var inscope ask : array<int>
        var inscope masks : array<uint>
        while (k < length(words) && sentences[k] == sentence) {
            let mask = ru_stress_allowed(m, lowers[k])
            if (mask != 0u && ru_word_needs_context(p, lowers[k])) {
                ask |> push(k)
                masks |> push(mask)
            }
            k++
        }
        var stop = words[k - 1].y
        while (stop < length(cps) && !ru_is_sentence_end(cps, stop)) {
            stop++
        }
        stop = min(stop + 1, length(cps))
        if (!empty(ask)) {
            var inscope spans <- [for (j in ask); words[j] - int2(sentence_start, sentence_start)]
            var inscope picks <- ru_stress_pick_long(m, cpts_to_utf8(cps, sentence_start, stop), spans, masks, sc)
            for (j, pick in ask, picks) {
                res.asked++
                if (pick.ordinal <= 0) {
                    res.fallbacks++
                    continue
                }
                let picked = ru_stress_class_pick(pick.ordinal)
                res.picked[j] = picked
                res.beyond_pack += picked >= RU_PICK_YO || ru_stress_pack_has(p, lowers[j], picked) ? 0 : 1
            }
        }
        sentence_start = stop
    }
    return <- res
}

//! `ru_stress_context_picks` over a text: its own word split, for `ru_mark_stress`'s `picked`.
def ru_stress_context_picks(m : RuStressModel; p : RuStressPack; text : string; var sc : RuStressScratch) : RuContextPicks {
    var inscope cps <- utf8_to_cpts(text)
    var inscope words <- ru_text_words(cps)
    return <- ru_stress_context_picks(m, p, cps, words, sc)
}

//! Normalized Russian text stress-marked with the context tier's picks: one word split serves the picks and the marking.
[arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#tts-stress-context")]
def ru_mark_stress_in_context(m : RuStressModel; p : RuStressPack; text : string; var sc : RuStressScratch) : string {
    var inscope cps <- utf8_to_cpts(text)
    var inscope words <- ru_text_words(cps)
    var inscope picks <- ru_stress_context_picks(m, p, cps, words, sc)
    return ru_mark_words(p, cps, words, picks.picked)
}

def private is_word_at(s, w : array<int>; i : int) : bool {
    let k = length(w)
    return false if ((i > 0 && cpt_is_cyrillic(s[i - 1])) || (i + k < length(s) && cpt_is_cyrillic(s[i + k])))
    for (j in range(k)) {
        return false if (s[i + j] != w[j])
    }
    return true
}

//! The first occurrence of `form` in `sentence`, both lowercased, not inside a longer run of
//! Russian letters, as codepoints [x, y) - (-1, -1) where it does not occur.
[cold_path]
def ru_stress_find_word(sentence, form : string) : int2 {
    var inscope s <- utf8_to_cpts(utf8_to_lower(sentence))
    var inscope w <- utf8_to_cpts(utf8_to_lower(form))
    let k = length(w)
    if (k > 0) {
        for (i in range(length(s) - k + 1)) {
            return int2(i, i + k) if (is_word_at(s, w, i))
        }
    }
    return int2(-1, -1)
}
