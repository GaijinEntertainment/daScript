options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_nmt shared public
options _function_length = 0   // the loader and serialize walks mirror the model schema

require dasllama/dasllama_lint public
require math
require daslib/fio
require strings
require daslib/strings_boost
require daslib/array_boost
require daslib/archive
require dasllama/dasllama_env
require dasllama/dasllama_math
require dasllama/dasllama_plane public
require dasllama/dasllama_tower
require dasllama/dasllama_convert
require dasllama/dasllama_common
require dasllama/dasllama_image
require dasllama/dasllama_spm public
require dasllama/dasllama_gemm_schema   // Q8_BLOCK_ELEMS - the q8 lane's block

//! The family a Bergamot student declares: the image tags' stem and the logs' name.
let NMT_ARCH = "bergamot"
let private NMT_WHO = "dasLLAMA {NMT_ARCH}"

//! The block LayerNorms' epsilon, added inside the root.
let NMT_LN_EPS = 1e-9

//! Greedy decoding stops at `NMT_MAX_LEN_FACTOR` times the source's token count, end token included - the length bound the
//! students are served under in Firefox.
let NMT_MAX_LEN_FACTOR = 2l

//! The source tokens a sentence served on a GPU home may carry - every home's cap, so a sentence runs on the device on every
//! box or on none; a longer one runs the CPU chain.
let NMT_DEVICE_SRC_CAP = 256l

//! Whether a GPU home serves the model's geometry: heads of whole float4s, widths whole 64-wide GEMM tiles - every home's
//! gate, the narrowest home's.
[arch(at="../ARCHITECTURE_TRANSLATE.md#nmt-device-homes")]
def nmt_device_shape_ok(m : NmtModel) : bool {
    let hs = m.d / max(m.n_head, 1l)
    return hs * m.n_head == m.d && hs % 4l == 0l && m.d % 64l == 0l && m.ff % 64l == 0l
}

let private MARIAN_FLOAT32 = 0x0404ul
let private MARIAN_INT8 = 0x0101ul
let private MARIAN_INTGEMM8 = 0x4101ul
//! an item's header bytes: four u64 - the name's length, the type, the rank, the blob's bytes
let private MARIAN_ITEM_HEAD = 32l

//! One encoder layer's offsets: the GEMMs in the GEMM plane (``qblob`` on the q8 lane, else ``blob``), the rows in ``blob``.
struct NmtEncOffs {
    wq : int64      //!< wq, wk, wv, wo [d][d] back to back, then w1 [ff][d], w2 [d][ff]
    wk : int64
    wv : int64
    wo : int64
    w1 : int64
    w2 : int64
    bq : int64
    bk : int64
    bv : int64
    bo : int64
    ln1_s : int64
    ln1_b : int64
    b1 : int64
    b2 : int64
    ln2_s : int64
    ln2_b : int64
}

//! One decoder layer's offsets: the SSRU's ``w`` and ``wf`` stacked as one [2d][d] GEMM, the cross-attention, the FFN.
struct NmtDecOffs {
    wrnn : int64    //!< [w ; wf] as [2d][d]: the candidate rows, then the forget gate's
    wq : int64
    wk : int64
    wv : int64
    wo : int64
    w1 : int64
    w2 : int64
    bf : int64
    rnn_ln_s : int64
    rnn_ln_b : int64
    bq : int64
    bk : int64
    bv : int64
    bo : int64
    ctx_ln_s : int64
    ctx_ln_b : int64
    b1 : int64
    b2 : int64
    ffn_ln_s : int64
    ffn_ln_b : int64
}

//! A loaded Bergamot student: hparams, the SentencePiece vocabulary and normalizer, and the weight planes. The tied
//! embedding, the output bias and every bias and norm row are f32 in ``blob``; the block GEMMs serve off ``blob`` on the
//! exact lane and off ``qblob``/``qscales`` as Q8_0 on the q8 lane.
struct NmtModel {
    d : int64
    ff : int64
    n_head : int64
    n_enc : int64
    n_dec : int64
    n_vocab : int64
    eos_id : int64
    unk_id : int64
    byte_fallback : bool
    add_space_prefix : bool
    remove_extra_ws : bool
    pair : string                   //!< the direction the vocabulary's file name carries, "ruen"
    vocab : array<string>
    scores : array<float>
    charsmap : array<uint8>
    emb_off : int64                 //!< [n_vocab][d] in ``blob``: the source and target embedding, and the output projection
    out_b : int64                   //!< [n_vocab] in ``blob``
    aux_off : int64                 //!< the rows: the encoder layers' ``nmt_enc_aux`` each, then the decoder layers' ``nmt_dec_aux``
    gemm_off : int64                //!< the GEMMs: the encoder layers' ``nmt_enc_gemm`` each, then the decoder layers'
    cls_off : int64                 //!< [n_vocab][d] in ``qblob`` on the q8 lane: the output projection's Q8_0 copy; -1 on the exact lane
    q8 : bool
    blob : PlaneF = PlaneF()
    qblob : PlaneI8 = PlaneI8()
    qscales : PlaneF = PlaneF()
    spm : SpmModelFile = SpmModelFile()   //!< rebuilt from the vocabulary fields at load
    image_map : void?
    image_bytes : uint64
    gen : uint64                    //!< this load's generation, unique a process: the device drivers' weight-reuse key
}

def finalize(var t : NmtModel) {
    delete t.vocab
    delete t.scores
    delete t.charsmap
    delete t.spm
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

def private nmt_enc_aux(d, ff : int64) : int64 => 9l * d + ff
def private nmt_dec_aux(d, ff : int64) : int64 => 12l * d + ff
def private nmt_enc_gemm(d, ff : int64) : int64 => 4l * d * d + 2l * ff * d
def private nmt_dec_gemm(d, ff : int64) : int64 => 6l * d * d + 2l * ff * d

//! Encoder layer `l`'s offsets.
def nmt_enc_offsets(m : NmtModel; l : int64) : NmtEncOffs {
    let d = m.d
    let ff = m.ff
    let g = m.gemm_off + l * nmt_enc_gemm(d, ff)
    let a = m.aux_off + l * nmt_enc_aux(d, ff)
    return NmtEncOffs(wq = g, wk = g + d * d, wv = g + 2l * d * d, wo = g + 3l * d * d, w1 = g + 4l * d * d,
                      w2 = g + 4l * d * d + ff * d,
                      bq = a, bk = a + d, bv = a + 2l * d, bo = a + 3l * d, ln1_s = a + 4l * d, ln1_b = a + 5l * d,
                      b1 = a + 6l * d, b2 = a + 6l * d + ff, ln2_s = a + 7l * d + ff, ln2_b = a + 8l * d + ff)
}

//! Decoder layer `l`'s offsets.
def nmt_dec_offsets(m : NmtModel; l : int64) : NmtDecOffs {
    let d = m.d
    let ff = m.ff
    let g = m.gemm_off + m.n_enc * nmt_enc_gemm(d, ff) + l * nmt_dec_gemm(d, ff)
    let a = m.aux_off + m.n_enc * nmt_enc_aux(d, ff) + l * nmt_dec_aux(d, ff)
    return NmtDecOffs(wrnn = g, wq = g + 2l * d * d, wk = g + 3l * d * d, wv = g + 4l * d * d, wo = g + 5l * d * d,
                      w1 = g + 6l * d * d, w2 = g + 6l * d * d + ff * d,
                      bf = a, rnn_ln_s = a + d, rnn_ln_b = a + 2l * d, bq = a + 3l * d, bk = a + 4l * d,
                      bv = a + 5l * d, bo = a + 6l * d, ctx_ln_s = a + 7l * d, ctx_ln_b = a + 8l * d,
                      b1 = a + 9l * d, b2 = a + 9l * d + ff, ffn_ln_s = a + 10l * d + ff, ffn_ln_b = a + 11l * d + ff)
}

//! serialize_image_meta(NmtModel) covers 21 fields, the two POD tables (scores, charsmap) among them uncounted; spm (rebuilt
//! at load), image_map, image_bytes and gen are skipped
let private NMT_META_FIELDS = 20 + 3

def private serialize_image_meta(var arch : Archive; var t : NmtModel) {
    verify(count_meta_fields(t) == NMT_META_FIELDS)
    arch |> serialize_raw(t.d)
    arch |> serialize_raw(t.ff)
    arch |> serialize_raw(t.n_head)
    arch |> serialize_raw(t.n_enc)
    arch |> serialize_raw(t.n_dec)
    arch |> serialize_raw(t.n_vocab)
    arch |> serialize_raw(t.eos_id)
    arch |> serialize_raw(t.unk_id)
    arch |> serialize_raw(t.byte_fallback)
    arch |> serialize_raw(t.add_space_prefix)
    arch |> serialize_raw(t.remove_extra_ws)
    arch |> serialize(t.pair)
    serialize_strings(arch, t.vocab)
    serialize_pod_array(arch, t.scores)
    serialize_pod_array(arch, t.charsmap)
    arch |> serialize_raw(t.emb_off)
    arch |> serialize_raw(t.out_b)
    arch |> serialize_raw(t.aux_off)
    arch |> serialize_raw(t.gemm_off)
    arch |> serialize_raw(t.cls_off)
    arch |> serialize_raw(t.q8)
}

//! The tokenizer the vocabulary fields describe: the lookup and the derived tables rebuilt.
def private nmt_build_spm(var t : NmtModel) {
    delete t.spm
    t.spm.tk.vocab := t.vocab
    t.spm.tk.scores := t.scores
    t.spm.tk.add_space_prefix = t.add_space_prefix
    t.spm.norm.charsmap := t.charsmap
    t.spm.norm.remove_extra_ws = t.remove_extra_ws
    t.spm.unk_id = t.unk_id
    t.spm.eos_id = t.eos_id
    t.spm.byte_fallback = t.byte_fallback
    t.spm.unigram = true
    spm_model_file_finish(t.spm)
}

var private g_nmt_gen = 0ul

def private image_post_load(var t : NmtModel) {
    nmt_build_spm(t)
    g_nmt_gen++
    t.gen = g_nmt_gen
}

[init]
def private register_nmt_image_tags {
    register_image_family_tag("bergamot-q8")
    register_image_family_tag("bergamot-f32", config_free = true)
}

//! The ``.dlim`` family tag of a weight lane: ``bergamot-q8``, or ``bergamot-f32`` for the exact lane.
def nmt_image_tag(q8 : bool) : string => q8 ? "bergamot-q8" : "bergamot-f32"

//! The mint-side carrier: ``e`` holds the scalars the meta blob carries, the arrays beside it the plane bytes.
struct NmtStaging {
    e : NmtModel = NmtModel()
    @exact_size blob : array<float>
    @exact_size qblob : array<int8>
    @exact_size qscales : array<float>
}

def private serialize_image_meta(var arch : Archive; var st : NmtStaging) {
    serialize_image_meta(arch, st.e)
}

def finalize(var st : NmtStaging) {
    delete_staging(st)
}

[unused_argument(st)]
def private image_post_load(var st : NmtStaging) {}

//! One item of a Marian binary model: its name, type, shape and its bytes' place in the file.
struct private MarianItem {
    name : string
    kind : uint64
    shape : array<int>
    at : int64
    bytes : int64
}

def private nmt_expect(ok : bool; what : string) {
    if (!ok) {
        panic("{NMT_WHO}: {what}")
    }
}

//! `n` bytes of `b` at `at` as a string - an item name or the model's yml, their trailing zero left off
def private marian_text(b : array<uint8> | #; at, n : int64) : string {
    return "" if (n <= 0l)
    unsafe {
        return string(temp_array(addr<uint8?>(b[at]), int(n), type<uint8>))
    }
}

//! The item table of a Marian binary file: a version, the item count, a header of four u64 a item,
//! the NUL-terminated names, the i32 shapes, then a pad to 256 and the blobs back to back.
def private marian_items(b : array<uint8> | #) : array<MarianItem> {
    let total = long_length(b)
    nmt_expect(total > 16l && rd_u64(b, 0l) == 1ul, "not a Marian binary model (version {total > 8l ? rd_u64(b, 0l) : 0ul})")
    let n = int64(rd_u64(b, 8l))
    var at = 16l
    var items : array<MarianItem>
    items |> reserve(n)
    var heads : array<int64>
    heads |> reserve(4l * n)
    for (_i in range64(4l * n)) {
        heads |> push(int64(rd_u64(b, at)))
        at += 8l
    }
    for (i in range64(n)) {
        let nl = heads[4l * i]
        nmt_expect(at + nl <= total, "item {i}'s name runs past the file")
        unsafe {
            items |> emplace(MarianItem(name = marian_text(b, at, nl - 1l),
                                        kind = uint64(heads[4l * i + 1l]), bytes = heads[4l * i + 3l]))
        }
        at += nl
    }
    for (i in range64(n)) {
        items[i].shape |> reserve(heads[4l * i + 2l])
        for (_r in range64(heads[4l * i + 2l])) {
            items[i].shape |> push(rd_i32(b, at))
            at += 4l
        }
    }
    at += 8l + int64(rd_u64(b, at))
    for (it in items) {
        it.at = at
        at += it.bytes
    }
    nmt_expect(at == total, "the item blobs end at {at}, the file at {total}")
    delete heads
    return <- items
}

//! Whether `b` is a Marian binary model: version 1, an item table that fits the file, and the embedded
//! `special:model.yml` among its names - the bake tool's sniff, which must not panic on a foreign file.
def is_marian_model(b : array<uint8> | #) : bool {
    let total = long_length(b)
    return false if (total < 16l || rd_u64(b, 0l) != 1ul)
    let n = int64(rd_u64(b, 8l))
    return false if (n <= 0l || n > (total - 16l) / MARIAN_ITEM_HEAD)
    var at = 16l + MARIAN_ITEM_HEAD * n
    for (i in range64(n)) {
        let nl = int64(rd_u64(b, 16l + MARIAN_ITEM_HEAD * i))
        return false if (nl <= 0l || at + nl > total)
        unsafe {
            return true if (marian_text(b, at, nl - 1l) == "special:model.yml")
        }
        at += nl
    }
    return false
}

def private marian_find(items : array<MarianItem>; name : string) : int {
    for (i, it in count(), items) {
        return i if (it.name == name)
    }
    return -1
}

//! A yml scalar of the model's embedded config (`special:model.yml`): the text after `key: ` on its line, "" when absent.
def private yml_value(yml, key : string) : string {
    for (line in split(yml, "\n")) {
        if (starts_with(line, "{key}:")) {
            return strip(slice(line, length(key) + 1))
        }
    }
    return ""
}

//! One weight's f32 values into `dst` at `at`, oriented as the GEMMs read it - [out][in] rows: an intgemm8 item is int8 over
//! one trailing f32 quant multiplier, already stored [out][in] (the embedding alone untransposed, [vocab][d]); a float32
//! matrix is [in][out] and is transposed on the way in.
def private marian_read_f32(b : array<uint8> | #; it : MarianItem; var dst : array<float>; at : int64) {
    let rows = int64(it.shape[0])
    let cols = length(it.shape) > 1 ? int64(it.shape[1]) : 1l
    let n = rows * cols
    unsafe {
        if (it.kind == MARIAN_INTGEMM8) {
            nmt_expect(it.bytes >= n + 4l, "'{it.name}' holds {it.bytes} bytes, its int8 shape {n} and a multiplier")
            let qm = *addr<float?>(b[it.at + n])
            let inv = 1.0 / qm
            let q = addr<int8?>(b[it.at])
            for (i in range64(n)) {
                dst[at + i] = float(q[i]) * inv
            }
        } elif (it.kind == MARIAN_FLOAT32 && (rows == 1l || it.name == "Wemb")) {
            memcpy(addr(dst[at]), addr(b[it.at]), n * 4l)
        } elif (it.kind == MARIAN_FLOAT32) {
            let src = addr<float?>(b[it.at])
            for (r in range64(rows)) {
                for (c in range64(cols)) {
                    dst[at + c * rows + r] = src[r * cols + c]
                }
            }
        } else {
            nmt_expect(false, "'{it.name}' has Marian type {it.kind}, neither float32 nor intgemm8")
        }
    }
}

//! The model's hparams off the embedded yml and the item shapes, the layout into `e`: rows first in ``blob``, then the GEMMs
//! (in ``blob`` on the exact lane, in ``qblob`` on the q8 lane).
def private nmt_read_hparams(items : array<MarianItem>; b : array<uint8> | #; var e : NmtModel) {
    let yi = marian_find(items, "special:model.yml")
    nmt_expect(yi >= 0, "no special:model.yml item")
    var yml = ""
    unsafe {
        yml = marian_text(b, items[yi].at, items[yi].bytes - 1l)
    }
    nmt_expect(yml_value(yml, "type") == "transformer", "model type '{yml_value(yml, "type")}', not transformer")
    nmt_expect(yml_value(yml, "transformer-decoder-autoreg") == "rnn" && yml_value(yml, "dec-cell") == "ssru",
               "the decoder is not an SSRU student")
    nmt_expect(yml_value(yml, "transformer-postprocess") == "dan" && yml_value(yml, "transformer-preprocess") == "\"\"",
               "the layers are not post-norm (`dan`)")
    nmt_expect(yml_value(yml, "tied-embeddings-all") == "true", "the embeddings are not tied")
    nmt_expect(yml_value(yml, "transformer-ffn-activation") == "relu", "the FFN activation is not relu")
    nmt_expect(yml_value(yml, "layer-normalization") == "false", "the SSRU carries layer norms")
    e.d = to_int64(yml_value(yml, "dim-emb"), false)
    e.ff = to_int64(yml_value(yml, "transformer-dim-ffn"), false)
    e.n_head = to_int64(yml_value(yml, "transformer-heads"), false)
    e.n_enc = to_int64(yml_value(yml, "enc-depth"), false)
    e.n_dec = to_int64(yml_value(yml, "dec-depth"), false)
    let wi = marian_find(items, "Wemb")
    nmt_expect(wi >= 0 && length(items[wi].shape) == 2 && int64(items[wi].shape[1]) == e.d, "no [vocab][{e.d}] Wemb")
    e.n_vocab = int64(items[wi].shape[0])
    nmt_expect(e.d > 0l && e.ff > 0l && e.n_head > 0l && e.d % e.n_head == 0l && e.n_enc > 0l && e.n_dec > 0l,
               "hparams d {e.d} ff {e.ff} heads {e.n_head} layers {e.n_enc}+{e.n_dec}")
    nmt_expect(e.d % Q8_BLOCK_ELEMS == 0l && e.ff % Q8_BLOCK_ELEMS == 0l, "d {e.d} / ff {e.ff} off the Q8_0 lattice")
    e.emb_off = 0l
    e.out_b = e.n_vocab * e.d
    e.aux_off = e.out_b + e.n_vocab
    let aux_n = e.n_enc * nmt_enc_aux(e.d, e.ff) + e.n_dec * nmt_dec_aux(e.d, e.ff)
    let rows_end = (e.aux_off + aux_n + Q8_BLOCK_ELEMS - 1l) / Q8_BLOCK_ELEMS * Q8_BLOCK_ELEMS
    e.gemm_off = e.q8 ? 0l : rows_end
    e.cls_off = e.q8 ? e.n_enc * nmt_enc_gemm(e.d, e.ff) + e.n_dec * nmt_dec_gemm(e.d, e.ff) : -1l
}

//! The plane sizes the layout takes: ``blob``'s and the q8 GEMM region's (0 on the exact lane).
def private nmt_plane_sizes(e : NmtModel) : tuple<blob_n : int64; q8_n : int64> {
    let aux_n = e.n_enc * nmt_enc_aux(e.d, e.ff) + e.n_dec * nmt_dec_aux(e.d, e.ff)
    let gemm_n = e.n_enc * nmt_enc_gemm(e.d, e.ff) + e.n_dec * nmt_dec_gemm(e.d, e.ff)
    let rows_end = (e.aux_off + aux_n + Q8_BLOCK_ELEMS - 1l) / Q8_BLOCK_ELEMS * Q8_BLOCK_ELEMS
    return e.q8 ? (blob_n = rows_end, q8_n = gemm_n + e.n_vocab * e.d) : (blob_n = rows_end + gemm_n, q8_n = 0l)
}

//! One named weight of `n` elements into the f32 rows (`blob`) at `at`.
def private nmt_row(items : array<MarianItem>; b : array<uint8> | #; name : string; var blob : array<float>; at, n : int64) {
    let i = marian_find(items, name)
    nmt_expect(i >= 0, "no item '{name}'")
    var cnt = 1l
    for (s in items[i].shape) {
        cnt *= int64(s)
    }
    nmt_expect(cnt == n, "'{name}' holds {cnt} elements, the layout {n}")
    marian_read_f32(b, items[i], blob, at)
}

//! One named [out][in] GEMM into the GEMM plane at `at`: f32 into `blob` on the exact lane, Q8_0 into `qblob`/`qscales`
//! on the q8 lane (through `scratch`).
def private nmt_gemm(items : array<MarianItem>; b : array<uint8> | #; name : string; e : NmtModel; var blob : array<float>;
                     var qblob : array<int8>; var qscales : array<float>; @scratch var scratch : array<float>; at, n : int64) {
    if (!e.q8) {
        nmt_row(items, b, name, blob, at, n)
        return
    }
    ensure_length(scratch, n)
    nmt_row(items, b, name, scratch, 0l, n)
    quantize_q8_0_into(scratch, n, qblob, qscales, at, at / Q8_BLOCK_ELEMS)
}

//! Every weight of the file into the staging's planes per the layout.
def private nmt_fill(items : array<MarianItem>; b : array<uint8> | #; var st : NmtStaging) {
    let d = st.e.d
    let ff = st.e.ff
    let sizes = nmt_plane_sizes(st.e)
    st.blob |> reserve(sizes.blob_n)
    st.blob |> resize(sizes.blob_n)
    st.qblob |> reserve(sizes.q8_n)
    st.qblob |> resize(sizes.q8_n)
    st.qscales |> reserve(sizes.q8_n / Q8_BLOCK_ELEMS)
    st.qscales |> resize(sizes.q8_n / Q8_BLOCK_ELEMS)
    var @scratch @exact_size scratch : array<float>
    nmt_row(items, b, "Wemb", st.blob, st.e.emb_off, st.e.n_vocab * d)
    nmt_row(items, b, "decoder_ff_logit_out_b", st.blob, st.e.out_b, st.e.n_vocab)
    for (l in range64(st.e.n_enc)) {
        let o = nmt_enc_offsets(st.e, l)
        let p = "encoder_l{l + 1l}"
        nmt_gemm(items, b, "{p}_self_Wq", st.e, st.blob, st.qblob, st.qscales, scratch, o.wq, d * d)
        nmt_gemm(items, b, "{p}_self_Wk", st.e, st.blob, st.qblob, st.qscales, scratch, o.wk, d * d)
        nmt_gemm(items, b, "{p}_self_Wv", st.e, st.blob, st.qblob, st.qscales, scratch, o.wv, d * d)
        nmt_gemm(items, b, "{p}_self_Wo", st.e, st.blob, st.qblob, st.qscales, scratch, o.wo, d * d)
        nmt_gemm(items, b, "{p}_ffn_W1", st.e, st.blob, st.qblob, st.qscales, scratch, o.w1, ff * d)
        nmt_gemm(items, b, "{p}_ffn_W2", st.e, st.blob, st.qblob, st.qscales, scratch, o.w2, d * ff)
        nmt_row(items, b, "{p}_self_bq", st.blob, o.bq, d)
        nmt_row(items, b, "{p}_self_bk", st.blob, o.bk, d)
        nmt_row(items, b, "{p}_self_bv", st.blob, o.bv, d)
        nmt_row(items, b, "{p}_self_bo", st.blob, o.bo, d)
        nmt_row(items, b, "{p}_self_Wo_ln_scale", st.blob, o.ln1_s, d)
        nmt_row(items, b, "{p}_self_Wo_ln_bias", st.blob, o.ln1_b, d)
        nmt_row(items, b, "{p}_ffn_b1", st.blob, o.b1, ff)
        nmt_row(items, b, "{p}_ffn_b2", st.blob, o.b2, d)
        nmt_row(items, b, "{p}_ffn_ffn_ln_scale", st.blob, o.ln2_s, d)
        nmt_row(items, b, "{p}_ffn_ffn_ln_bias", st.blob, o.ln2_b, d)
    }
    for (l in range64(st.e.n_dec)) {
        let o = nmt_dec_offsets(st.e, l)
        let p = "decoder_l{l + 1l}"
        nmt_gemm(items, b, "{p}_rnn_W", st.e, st.blob, st.qblob, st.qscales, scratch, o.wrnn, d * d)
        nmt_gemm(items, b, "{p}_rnn_Wf", st.e, st.blob, st.qblob, st.qscales, scratch, o.wrnn + d * d, d * d)
        nmt_gemm(items, b, "{p}_context_Wq", st.e, st.blob, st.qblob, st.qscales, scratch, o.wq, d * d)
        nmt_gemm(items, b, "{p}_context_Wk", st.e, st.blob, st.qblob, st.qscales, scratch, o.wk, d * d)
        nmt_gemm(items, b, "{p}_context_Wv", st.e, st.blob, st.qblob, st.qscales, scratch, o.wv, d * d)
        nmt_gemm(items, b, "{p}_context_Wo", st.e, st.blob, st.qblob, st.qscales, scratch, o.wo, d * d)
        nmt_gemm(items, b, "{p}_ffn_W1", st.e, st.blob, st.qblob, st.qscales, scratch, o.w1, ff * d)
        nmt_gemm(items, b, "{p}_ffn_W2", st.e, st.blob, st.qblob, st.qscales, scratch, o.w2, d * ff)
        nmt_row(items, b, "{p}_rnn_bf", st.blob, o.bf, d)
        nmt_row(items, b, "{p}_rnn_ffn_ln_scale", st.blob, o.rnn_ln_s, d)
        nmt_row(items, b, "{p}_rnn_ffn_ln_bias", st.blob, o.rnn_ln_b, d)
        nmt_row(items, b, "{p}_context_bq", st.blob, o.bq, d)
        nmt_row(items, b, "{p}_context_bk", st.blob, o.bk, d)
        nmt_row(items, b, "{p}_context_bv", st.blob, o.bv, d)
        nmt_row(items, b, "{p}_context_bo", st.blob, o.bo, d)
        nmt_row(items, b, "{p}_context_Wo_ln_scale", st.blob, o.ctx_ln_s, d)
        nmt_row(items, b, "{p}_context_Wo_ln_bias", st.blob, o.ctx_ln_b, d)
        nmt_row(items, b, "{p}_ffn_b1", st.blob, o.b1, ff)
        nmt_row(items, b, "{p}_ffn_b2", st.blob, o.b2, d)
        nmt_row(items, b, "{p}_ffn_ffn_ln_scale", st.blob, o.ffn_ln_s, d)
        nmt_row(items, b, "{p}_ffn_ffn_ln_bias", st.blob, o.ffn_ln_b, d)
    }
    if (st.e.q8) {
        let n = st.e.n_vocab * d
        ensure_length(scratch, n)
        unsafe {
            memcpy(addr(scratch[0]), addr(st.blob[st.e.emb_off]), n * 4l)
        }
        quantize_q8_0_into(scratch, n, st.qblob, st.qscales, st.e.cls_off, st.e.cls_off / Q8_BLOCK_ELEMS)
    }
    delete scratch
    if (st.e.q8 && select_matmul_backend_for_load_()) {
        nmt_repack_q8(st)
    }
}

//! Every q8 GEMM region repacked for the active backend's q8q8 kernels, each at its own input width.
def private nmt_repack_q8(var st : NmtStaging) {
    let d = st.e.d
    let ff = st.e.ff
    for (l in range64(st.e.n_enc)) {
        let o = nmt_enc_offsets(st.e, l)
        for (w in [o.wq, o.wk, o.wv, o.wo]) {
            repack_q8q8_weight(st.qblob, st.qscales, w, d, d)
        }
        repack_q8q8_weight(st.qblob, st.qscales, o.w1, d, ff)
        repack_q8q8_weight(st.qblob, st.qscales, o.w2, ff, d)
    }
    for (l in range64(st.e.n_dec)) {
        let o = nmt_dec_offsets(st.e, l)
        repack_q8q8_weight(st.qblob, st.qscales, o.wrnn, d, 2l * d)
        for (w in [o.wq, o.wk, o.wv, o.wo]) {
            repack_q8q8_weight(st.qblob, st.qscales, w, d, d)
        }
        repack_q8q8_weight(st.qblob, st.qscales, o.w1, d, ff)
        repack_q8q8_weight(st.qblob, st.qscales, o.w2, ff, d)
    }
    repack_q8q8_weight(st.qblob, st.qscales, st.e.cls_off, d, st.e.n_vocab)
}

//! The vocabulary file beside a model: ``vocab.<pair>.spm``, or the source half ``srcvocab.<pair>.spm`` of a split pair.
def nmt_vocab_path(model_path : string) : string {
    let dir = dir_name(model_path)
    let stem = base_name(model_path)
    let parts <- split(stem, ".")
    let pair = length(parts) > 1 ? parts[1] : ""
    let both = path_join(dir, "vocab.{pair}.spm")
    return stat(both).is_valid ? both : path_join(dir, "srcvocab.{pair}.spm")
}

//! The reader proper: the scalars, the vocabulary from the ``.spm`` beside the model, and every weight into the staging.
def stage_nmt_model(path : string; q8 : bool = false) : NmtStaging {
    refuse_staged_source_past_cap(path, NMT_WHO, "the Bergamot students are tens of MB; a file this large is not one")
    var st = NmtStaging()
    st.e.q8 = q8
    let vpath = nmt_vocab_path(path)
    nmt_expect(stat(vpath).is_valid, "no vocabulary beside '{path}' (looked for '{vpath}')")
    nmt_expect(!starts_with(base_name(vpath), "srcvocab"), "'{path}' is a split-vocabulary pair; one shared vocabulary is served")
    var inscope sp <- load_spm_model_file(vpath)
    nmt_expect(sp.unigram, "'{vpath}' is not a unigram SentencePiece model")
    st.e.vocab := sp.tk.vocab
    st.e.scores := sp.tk.scores
    st.e.charsmap := sp.norm.charsmap
    st.e.unk_id = sp.unk_id
    st.e.eos_id = sp.eos_id
    st.e.byte_fallback = sp.byte_fallback
    st.e.add_space_prefix = sp.tk.add_space_prefix
    st.e.remove_extra_ws = sp.norm.remove_extra_ws
    let parts <- split(base_name(path), ".")
    st.e.pair = length(parts) > 1 ? parts[1] : ""
    let f = fopen(path, "rb")
    nmt_expect(f != null, "cannot open model '{path}'")
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope items <- marian_items(bytes)
        nmt_read_hparams(items, bytes, st.e)
        nmt_expect(st.e.n_vocab == long_length(st.e.vocab), "the model's {st.e.n_vocab} embeddings and the vocabulary's {length(st.e.vocab)} pieces")
        nmt_fill(items, bytes, st)
    }
    fclose(f)
    nmt_build_spm(st.e)
    return <- st
}

//! Mint a served model from a staging WITHOUT touching disk - the in-memory image rail, the suites' load.
def mint_nmt_model(var st : NmtStaging; var out : NmtModel) {
    cache_via_image_staged(st, out, "", nmt_image_tag(st.e.q8))
}

//! Load a Bergamot student (a Marian ``model.<pair>.intgemm.alphas.bin`` with its ``vocab.<pair>.spm`` beside it, or a
//! prepared ``.dlim`` of one). The exact lane serves the int8 weights widened to f32; ``q8`` serves the block GEMMs as Q8_0.
def load_nmt_model(path : string; q8 : bool = false) : NmtModel {
    var m <- load_staged_image(path, nmt_image_tag(q8), NMT_ARCH, "source model", type<NmtModel>) {
        return <- stage_nmt_model(path, q8)
    }
    to_log(LOG_INFO, "{NMT_WHO}: {m.pair} d {m.d} ff {m.ff} layers {m.n_enc}+{m.n_dec}, {q8 ? "q8" : "f32"} lane\n")
    return <- m
}

//! Every buffer an encode and a decode reuse, so a translation allocates nothing past its first call.
struct NmtState {
    @scratch @exact_size x : array<float>       //!< [rows][d] the stream
    @scratch @exact_size q : array<float>
    @scratch @exact_size k : array<float>
    @scratch @exact_size v : array<float>
    @scratch @exact_size att : array<float>
    @scratch @exact_size o : array<float>
    @scratch @exact_size h : array<float>       //!< [rows][ff], or [rows][2d] for the SSRU's stacked GEMM
    @scratch @exact_size hp : array<float>
    @scratch @exact_size sc : array<float>
    @scratch @exact_size xq : array<int8>
    @scratch @exact_size xs : array<float>
    @scratch @exact_size enc : array<float>     //!< [src][d] the encoder output
    @scratch @exact_size ck : array<float>      //!< [layer][src][d] the cross-attention keys
    @scratch @exact_size cv : array<float>      //!< [layer][src][d] the cross-attention values
    @scratch @exact_size cell : array<float>    //!< [layer][d] the SSRU cells
    @scratch @exact_size logits : array<float>
    @scratch @exact_size pos : array<float>     //!< [n_pos][d] the sinusoid rows of the positions reached so far
    n_pos : int64
    @scratch @exact_size stage_names : array<string>   //!< the taps' layer stage names, built once: enc_l1.., then dec_l1_rnn/_ctx/_ffn..
    n_src : int64
}

//! The sinusoid position rows, through position `upto - 1`, into ``st.pos``: sines over the first half of a row,
//! cosines over the second; a row once computed stays for every later sentence.
def nmt_positions(m : NmtModel; var st : NmtState; upto : int64) {
    return if (upto <= st.n_pos)
    let d = m.d
    let half = d / 2l
    let inc = log(10000.0) / float(half - 1l)
    st.pos |> reserve(upto * d)
    st.pos |> resize(upto * d)
    for (p in range64(st.n_pos, upto)) {
        for (i in range64(half)) {
            let v = float(p) * exp(float(i) * -inc)
            st.pos[p * d + i] = sin(v)
            st.pos[p * d + half + i] = cos(v)
        }
    }
    st.n_pos = upto
}

//! Token `id`'s embedding row (a zero row for -1), scaled by sqrt(d), plus position `p`'s sinusoid, into row `r` of `x`.
def private nmt_embed_row(m : NmtModel; var st : NmtState; id : int64; var x : array<float>; r, p : int64) {
    let d = m.d
    let scale = sqrt(float(d))
    let emb = plane_at(m.blob, m.emb_off)
    nmt_positions(m, st, p + 1l)
    unsafe {
        var xp = addr(x[r * d])
        let pp = addr<float const?>(st.pos[p * d])
        for (i in range64(d)) {
            xp[i] = (id >= 0l ? emb[id * d + i] * scale : 0.0) + pp[i]
        }
    }
}

//! A GEMM of the model's block weights: Y [rows][dout] = W[dout][din] X; the q8 lane requantizes `x` first.
def private nmt_mm(m : NmtModel; var st : NmtState; var y : array<float>; woff : int64; x : array<float>; din, dout, rows : int64) {
    tower_mm_rq(m, y, woff, x, st.xq, st.xs, din, dout, rows)
}

def private nmt_size(var st : NmtState; rows, d, ff : int64) {
    ensure_length(st.x, rows * d)
    ensure_length(st.q, rows * d)
    ensure_length(st.k, rows * d)
    ensure_length(st.v, rows * d)
    ensure_length(st.att, rows * d)
    ensure_length(st.o, rows * d)
    ensure_length(st.h, rows * max(ff, 2l * d))
}

//! The post-norm residual: x = LN(x + y) over `rows` rows, the norm's scale and bias at `ln_s` / `ln_b`.
def private nmt_add_norm(m : NmtModel; var x : array<float>; y : array<float>; ln_s, ln_b, rows : int64) {
    add_inplace_rows(x, y, rows * m.d)
    layernorm_batch(x, x, m.blob, ln_s, ln_b, m.d, rows, NMT_LN_EPS)
}

//! The ReLU FFN with its residual and norm: x = LN(x + W2 relu(W1 x + b1) + b2).
def private nmt_ffn(m : NmtModel; var st : NmtState; w1, b1, w2, b2, ln_s, ln_b, rows : int64) {
    nmt_mm(m, st, st.h, w1, st.x, m.d, m.ff, rows)
    bias_relu_rows(st.h, m.blob, b1, m.ff, rows)
    nmt_mm(m, st, st.o, w2, st.h, m.ff, m.d, rows)
    add_bias_rows(st.o, m.blob, b2, m.d, rows)
    nmt_add_norm(m, st.x, st.o, ln_s, ln_b, rows)
}

//! Encode the source ids (end token included) into ``st.enc``, then lay every decoder layer's cross-attention keys and
//! values over it. A stage tap `tap` sees the embedding and each layer's output.
//! The tap's per-layer stage names, built at the state's first sentence so no step formats one.
[cold_path]
def private nmt_stage_names(m : NmtModel; var st : NmtState) {
    return if (long_length(st.stage_names) == m.n_enc + 3l * m.n_dec)
    st.stage_names <- [for (l in range64(m.n_enc)); "enc_l{l + 1l}"]
    st.stage_names |> reserve(m.n_enc + 3l * m.n_dec)
    for (l in range64(m.n_dec)) {
        st.stage_names |> push("dec_l{l + 1l}_rnn")   // nolint:STYLE012 - appends a layer's three names after the encoder's
        st.stage_names |> push("dec_l{l + 1l}_ctx")
        st.stage_names |> push("dec_l{l + 1l}_ffn")
    }
}

def nmt_encode(m : NmtModel; var st : NmtState; src : array<int64>; tap : block<(stage : string; x : array<float>; rows : int64) : void>) {
    nmt_stage_names(m, st)
    let d = m.d
    let s = long_length(src)
    st.n_src = s
    nmt_size(st, s, d, m.ff)
    for (r, id in count(), src) {
        nmt_embed_row(m, st, id, st.x, int64(r), int64(r))
    }
    invoke(tap, "enc_emb", st.x, s)
    let scale = 1.0 / sqrt(float(d / m.n_head))
    for (l in range64(m.n_enc)) {
        let o = nmt_enc_offsets(m, l)
        nmt_mm(m, st, st.q, o.wq, st.x, d, d, s)
        add_bias_rows(st.q, m.blob, o.bq, d, s)
        nmt_mm(m, st, st.k, o.wk, st.x, d, d, s)
        add_bias_rows(st.k, m.blob, o.bk, d, s)
        nmt_mm(m, st, st.v, o.wv, st.x, d, d, s)
        add_bias_rows(st.v, m.blob, o.bv, d, s)
        attention_bidir(st.att, st.q, st.k, st.v, st.hp, st.sc, s, d, m.n_head, scale)
        nmt_mm(m, st, st.o, o.wo, st.att, d, d, s)
        add_bias_rows(st.o, m.blob, o.bo, d, s)
        nmt_add_norm(m, st.x, st.o, o.ln1_s, o.ln1_b, s)
        nmt_ffn(m, st, o.w1, o.b1, o.w2, o.b2, o.ln2_s, o.ln2_b, s)
        invoke(tap, st.stage_names[l], st.x, s)
    }
    ensure_length(st.enc, s * d)
    unsafe {
        memcpy(addr(st.enc[0]), addr(st.x[0]), s * d * 4l)
    }
    ensure_length(st.ck, m.n_dec * s * d)
    ensure_length(st.cv, m.n_dec * s * d)
    for (l in range64(m.n_dec)) {
        let o = nmt_dec_offsets(m, l)
        nmt_mm(m, st, st.k, o.wk, st.enc, d, d, s)
        add_bias_rows(st.k, m.blob, o.bk, d, s)
        nmt_mm(m, st, st.v, o.wv, st.enc, d, d, s)
        add_bias_rows(st.v, m.blob, o.bv, d, s)
        unsafe {
            memcpy(addr(st.ck[l * s * d]), addr(st.k[0]), s * d * 4l)
            memcpy(addr(st.cv[l * s * d]), addr(st.v[0]), s * d * 4l)
        }
    }
    ensure_length(st.cell, m.n_dec * d)
    for (i in range64(m.n_dec * d)) {
        st.cell[i] = 0.0
    }
}

//! the cross-attention of one query row `qp` over layer `l`'s cached keys and values, into `op`
def private nmt_cross_row(m : NmtModel; var st : NmtState; l : int64; qp : float const?; var op : float?) {
    let d = m.d
    let s = st.n_src
    let hs = d / m.n_head
    let scale = 1.0 / sqrt(float(hs))
    ensure_length(st.sc, s)
    unsafe {
        let kp = addr<float const?>(st.ck[l * s * d])
        let vp = addr<float const?>(st.cv[l * s * d])
        var sp = addr(st.sc[0])
        for (h in range64(m.n_head)) {
            let ho = h * hs
            for (j in range64(s)) {
                var acc = 0.0
                for (i in range64(hs)) {
                    acc += qp[ho + i] * kp[j * d + ho + i]
                }
                sp[j] = acc * scale
            }
            softmax(sp, s)
            for (i in range64(hs)) {
                op[ho + i] = 0.0
            }
            for (j in range64(s)) {
                let w = sp[j]
                for (i in range64(hs)) {
                    op[ho + i] += w * vp[j * d + ho + i]
                }
            }
        }
    }
}

//! One greedy decode step: the previous token `prev` (-1 at the first step: a zero embedding) at position `pos` through
//! every decoder layer, the logits into ``st.logits``; answers the argmax.
[hot_path]
def nmt_decode_step(m : NmtModel; var st : NmtState; prev, pos : int64;
                    tap : block<(stage : string; x : array<float>; rows : int64) : void>) : int64 {
    let d = m.d
    nmt_size(st, 1l, d, m.ff)
    nmt_embed_row(m, st, prev, st.x, 0l, pos)
    invoke(tap, "dec_emb", st.x, 1l)
    for (l in range64(m.n_dec)) {
        let o = nmt_dec_offsets(m, l)
        nmt_mm(m, st, st.h, o.wrnn, st.x, d, 2l * d, 1l)
        let bf = plane_at(m.blob, o.bf)
        unsafe {
            var cp = addr(st.cell[l * d])
            var op = addr(st.o[0])
            let hp = addr<float const?>(st.h[0])
            for (i in range64(d)) {
                let f = 1.0 / (1.0 + exp(-(hp[d + i] + bf[i])))
                cp[i] = f * cp[i] + (1.0 - f) * hp[i]
                op[i] = max(cp[i], 0.0)
            }
        }
        nmt_add_norm(m, st.x, st.o, o.rnn_ln_s, o.rnn_ln_b, 1l)
        invoke(tap, st.stage_names[m.n_enc + 3l * l], st.x, 1l)
        nmt_mm(m, st, st.q, o.wq, st.x, d, d, 1l)
        add_bias_rows(st.q, m.blob, o.bq, d, 1l)
        unsafe {
            nmt_cross_row(m, st, l, addr<float const?>(st.q[0]), addr(st.att[0]))
        }
        nmt_mm(m, st, st.o, o.wo, st.att, d, d, 1l)
        add_bias_rows(st.o, m.blob, o.bo, d, 1l)
        nmt_add_norm(m, st.x, st.o, o.ctx_ln_s, o.ctx_ln_b, 1l)
        invoke(tap, st.stage_names[m.n_enc + 3l * l + 1l], st.x, 1l)
        nmt_ffn(m, st, o.w1, o.b1, o.w2, o.b2, o.ffn_ln_s, o.ffn_ln_b, 1l)
        invoke(tap, st.stage_names[m.n_enc + 3l * l + 2l], st.x, 1l)
    }
    ensure_length(st.logits, m.n_vocab)
    if (m.q8) {
        nmt_mm(m, st, st.logits, m.cls_off, st.x, d, m.n_vocab, 1l)
    } else {
        mm_blob_b(st.logits, m.blob, m.emb_off, st.x, d, m.n_vocab, 1l)
    }
    add_bias_rows(st.logits, m.blob, m.out_b, m.n_vocab, 1l)
    invoke(tap, "logits", st.logits, 1l)
    return parallel_argmax(st.logits, m.n_vocab)
}

//! The source text's ids as the model's tokenizer spells them, the end token appended.
def nmt_encode_text(m : NmtModel; text : string) : array<int64> {
    var ids <- spm_file_encode(m.spm, text)
    ids |> push(m.eos_id)
    return <- ids
}

//! The lanes a decode step publishes to: `step_lanes` over its widest block GEMM's work.
[arch(at="../ARCHITECTURE_TRANSLATE.md#nmt-step-lanes")]
def nmt_step_lanes(m : NmtModel) : int => step_lanes(m.ff * m.d)

//! The GPU translate hook: the whole sentence - the encode and the greedy loop - on the device, the target ids into `out`
//! (the end token excluded); false declines and the CPU chain runs.
typedef NmtGpuTranslateFn = function<(m : NmtModel; var st : NmtState; src : array<int64>; var out : array<int64>) : bool>
var private g_nmt_gpu : NmtGpuTranslateFn
var private g_nmt_gpu_set = false

def register_nmt_gpu(fn : NmtGpuTranslateFn) {
    g_nmt_gpu = fn
    g_nmt_gpu_set = true
}

//! Translate already-tokenized source ids greedily: the target ids, the end token excluded - on a registered device driver
//! that serves the model, else on the CPU.
def nmt_translate_ids(m : NmtModel; var st : NmtState; src : array<int64>) : array<int64> {
    var out : array<int64>
    if (g_nmt_gpu_set && invoke(g_nmt_gpu, m, st, src, out)) {
        return <- out
    }
    return <- nmt_translate_ids_cpu(m, st, src) $ [unused_argument(stage, x, rows)] (stage : string; x : array<float>; rows : int64) {}
}

//! The CPU chain alone, a stage tap seeing every block's output - the oracle's specification and the device's control.
[hot_path]
def nmt_translate_ids_cpu(m : NmtModel; var st : NmtState; src : array<int64>;
                          tap : block<(stage : string; x : array<float>; rows : int64) : void>) : array<int64> {
    let cap = NMT_MAX_LEN_FACTOR * long_length(src)
    var out : array<int64>
    out |> reserve(cap)   // nolint:PERF026 - once a sentence, ahead of the per-step loop
    nmt_encode(m, st, src, tap)
    let pool = dispatch_lanes_narrow(nmt_step_lanes(m))
    var prev = -1l
    for (t in range64(cap)) {
        let id = nmt_decode_step(m, st, prev, t, tap)
        break if (id == m.eos_id)
        out |> push(id)   // nolint:PERF026 - the result, reserved to the cap ahead of the loop: no step reallocates it
        prev = id
    }
    dispatch_lanes_restore(pool)
    return <- out
}

//! Translate one line of text.
def nmt_translate(m : NmtModel; var st : NmtState; text : string) : string {
    var inscope src <- nmt_encode_text(m, text)
    var inscope ids <- nmt_translate_ids(m, st, src)
    return spm_decode_ids(m.spm.tk, ids)
}
