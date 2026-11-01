options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_gigaam shared public
options _function_length = 0   // the loader and serialize walks mirror the model schema

require dasllama/dasllama_lint public
require math
require daslib/fio
require strings
require daslib/strings_boost
require daslib/apply
require daslib/archive
require daslib/cmp   // cmp_by: a plane's jobs in offset order
require dasllama/dasllama_env
require dasllama/dasllama_math
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gguf
require dasllama/dasllama_plane public
require dasllama/dasllama_asr_types public
require dasllama/dasllama_audio
require dasllama/dasllama_tower
require dasllama/dasllama_load   // kq_tag: the one mapping a plane's disk type takes to the format it serves in
require dasllama/dasllama_convert
require dasllama/dasllama_repack   // repack_conv_out_tap_in: the f16 taps permuted tap-major
require dasllama/dasllama_par
require dasllama/dasllama_common
require dasllama/dasllama_image
require dasllama/dasllama_rope
require dasllama/dasllama_vad public
require dasllama/dasllama_rnnt public   // public: GigaamToken is RnntToken

//! The CTC head's row lattice in ``wblob``: its weight rows padded with zero rows to the device GEMMs' 64-row column tile, so a
//! device GEMM reads the plane in place.
let GIGAAM_CTC_ROW_TILE = 64l

//! The longest clip one encode takes: 25 s; a longer clip is split at silences.
let GIGAAM_MAX_CLIP_S = 25l

//! The encode-total ``asr_prof`` bucket; the stage buckets are ``ga.mel``, ``ga.front``, ``ga.blocks``, ``ga.decode_total``.
let GIGAAM_ENCODE_BUCKET = "ga.encode_total"

//! The log-mel's clamp before the log (`#gigaam-log-mel`): its floor and its ceiling.
let GIGAAM_MEL_FLOOR = 1e-9
let GIGAAM_MEL_CEIL = 1e9

let private GG_MAX_SYMBOLS = 10
//! The GGUF architecture a GigaAM carrier declares (`general.architecture`) - the facade's sniff and the loader's check.
let GIGAAM_ARCH = "gigaam"
let private GG_WHO = "dasLLAMA {GIGAAM_ARCH}"

//! The decode head a carrier ships.
enum GigaamHead {
    ctc
    rnnt
}

//! One decoded token: the shared transducer's record (a CTC head fills its id, frame and log-probability).
typedef GigaamToken = RnntToken

//! The tables derived from the hparams at load: the 320-point DFT twiddles.
struct GigaamMelTabs {
    twiddles : array<float>
}

//! A loaded GigaAM v3 model: hparams, the file's filterbank and window, the vocab, and the weight planes. The block GEMMs
//! serve off ``wblob`` as the carrier's f16 on the exact lane, off ``qblob``/``qscales`` on the q8 lane, and on the kq lane
//! each as the file's own format at its ``gemm_seats`` seat (K-quant off ``kq``/``ks``, Q8_0 off ``qblob``/``qscales``); the rest is f32 in ``blob``.
struct GigaamModel {
    n_vocab : int64
    n_classes : int64
    blank_id : int
    d_model : int64
    n_layer : int64
    n_head : int64
    head_dim : int64
    n_ff : int64
    n_mel : int64
    n_fft : int64
    hop : int64
    n_sub_ch : int64
    subs_kernel : int64
    conv_kernel : int64
    rope_base : float
    frame_cs : int64
    pred_dim : int64
    pred_layers : int64
    joint_dim : int64
    eps : float = 1e-5
    head : GigaamHead
    spm : bool
    vocab : array<string>
    filters : PlaneF = PlaneF()     //!< [n_mel][n_fft/2 + 1]
    window : PlaneF = PlaneF()
    blob : PlaneF = PlaneF()
    wblob : PlaneU16 = PlaneU16()   //!< the carrier's own f16 halfwords: the block GEMMs where ``gemm_f16``, the depthwise taps where ``dw_f16``, an f16 CTC head
    c0_w : int64                    //!< [n_sub_ch][k * n_mel], tap-major columns
    c0_b : int64
    c2_w : int64                    //!< [d][k * n_sub_ch], tap-major columns
    c2_b : int64
    aux_off : int64
    aux_stride : int64
    gemm_off : int64                //!< the block GEMMs' base: 0 in ``wblob`` (``gemm_f16``) or ``qblob`` (``q8``), else in ``blob``
    gemm_stride : int64
    gemm_f16 : bool                 //!< the exact lane over a carrier whose block GEMMs are f16: they serve off ``wblob``
    ctc_w : int64                   //!< [n_classes][d]: in ``wblob`` where ``ctc_f16``, padded to ``GIGAAM_CTC_ROW_TILE`` rows with zero rows; else in ``blob``
    ctc_b : int64
    ctc_f16 : bool                  //!< the CTC head's weight rows serve off ``wblob`` as the carrier's own f16 halfwords, on both lanes
    dw_f16 : bool                   //!< the depthwise taps serve off ``wblob`` as the carrier's own f16 halfwords, on both lanes
    dw_off : int64                  //!< the depthwise taps' base in ``wblob`` where ``dw_f16``: layer l's [k][d] at ``dw_off + l * k * d``
    embed_w : int64                 //!< [n_classes][pred_dim]
    lstm_ih : int64                 //!< [4 pred_dim][pred_dim], torch gate order i, f, g, o
    lstm_hh : int64
    lstm_b_ih : int64
    lstm_b_hh : int64
    jenc_w : int64
    jenc_b : int64
    jpred_w : int64
    jpred_b : int64
    jout_w : int64
    jout_b : int64
    q8 : bool
    qblob : PlaneI8 = PlaneI8()
    qscales : PlaneF = PlaneF()
    kq_lane : bool                  //!< the kq lane: the block GEMMs serve as the file's own quantized planes, each at its ``gemm_seats`` seat
    kq_repacked : bool              //!< the K-quant planes are in the load's backend layout (`matmul_kq_active`), else disk order (`matmul_kq`)
    kq_fmt : int                    //!< the kq lane's one served member (a `KqFmt` slot) where every seat shares it, -1 where the seats mix
    kq : PlaneU8 = PlaneU8()        //!< the kq lane's K-quant quant planes, a GEMM's superblocks at its seat's byte offset
    ks : PlaneU8 = PlaneU8()        //!< ... and their scale planes
    gemm_seats : PlaneI64 = PlaneI64()   //!< the kq lane's seats: `TOWER_SEAT_WORDS` words a block GEMM in layout order (`tower_gemm_seat`)
    tabs : GigaamMelTabs = GigaamMelTabs()
    image_map : void?
    image_bytes : uint64
}

def finalize(var t : GigaamModel) {
    delete t.vocab
    delete t.tabs
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

//! serialize_image_meta(GigaamModel) covers 52 fields; tabs (rebuilt at load), image_map and image_bytes are skipped
let private GIGAAM_META_FIELDS = 52 + 3

def private serialize_image_meta(var arch : Archive; var t : GigaamModel) {
    verify(count_meta_fields(t) == GIGAAM_META_FIELDS)
    arch |> serialize_raw(t.n_vocab)
    arch |> serialize_raw(t.n_classes)
    arch |> serialize_raw(t.blank_id)
    arch |> serialize_raw(t.d_model)
    arch |> serialize_raw(t.n_layer)
    arch |> serialize_raw(t.n_head)
    arch |> serialize_raw(t.head_dim)
    arch |> serialize_raw(t.n_ff)
    arch |> serialize_raw(t.n_mel)
    arch |> serialize_raw(t.n_fft)
    arch |> serialize_raw(t.hop)
    arch |> serialize_raw(t.n_sub_ch)
    arch |> serialize_raw(t.subs_kernel)
    arch |> serialize_raw(t.conv_kernel)
    arch |> serialize_raw(t.rope_base)
    arch |> serialize_raw(t.frame_cs)
    arch |> serialize_raw(t.pred_dim)
    arch |> serialize_raw(t.pred_layers)
    arch |> serialize_raw(t.joint_dim)
    arch |> serialize_raw(t.eps)
    arch |> serialize_raw(t.head)
    arch |> serialize_raw(t.spm)
    serialize_strings(arch, t.vocab)
    arch |> serialize_raw(t.c0_w)
    arch |> serialize_raw(t.c0_b)
    arch |> serialize_raw(t.c2_w)
    arch |> serialize_raw(t.c2_b)
    arch |> serialize_raw(t.aux_off)
    arch |> serialize_raw(t.aux_stride)
    arch |> serialize_raw(t.gemm_off)
    arch |> serialize_raw(t.gemm_stride)
    arch |> serialize_raw(t.gemm_f16)
    arch |> serialize_raw(t.ctc_w)
    arch |> serialize_raw(t.ctc_b)
    arch |> serialize_raw(t.ctc_f16)
    arch |> serialize_raw(t.dw_f16)
    arch |> serialize_raw(t.dw_off)
    arch |> serialize_raw(t.embed_w)
    arch |> serialize_raw(t.lstm_ih)
    arch |> serialize_raw(t.lstm_hh)
    arch |> serialize_raw(t.lstm_b_ih)
    arch |> serialize_raw(t.lstm_b_hh)
    arch |> serialize_raw(t.jenc_w)
    arch |> serialize_raw(t.jenc_b)
    arch |> serialize_raw(t.jpred_w)
    arch |> serialize_raw(t.jpred_b)
    arch |> serialize_raw(t.jout_w)
    arch |> serialize_raw(t.jout_b)
    arch |> serialize_raw(t.q8)
    arch |> serialize_raw(t.kq_lane)
    arch |> serialize_raw(t.kq_repacked)
    arch |> serialize_raw(t.kq_fmt)
}

def private image_post_load(var t : GigaamModel) {
    t.tabs.twiddles <- build_dft_twiddles(t.n_fft)
}

let GIGAAM_KQ_TAG = "gigaam-kq"   //! the kq lane's image tag: its planes are repacked for the load's backend, so the box keys it like the q8 lane's

[init]
def private register_gigaam_image_tags {
    register_image_family_tag("gigaam-q8")
    register_image_family_tag("gigaam-f16", config_free = true)
    register_image_family_tag(GIGAAM_KQ_TAG)
}

//! The plane a GigaAM fill job lands in.
enum GigaamPlane {
    blob    //!< f32: the front convs, the norms and biases, the heads, and the block GEMMs and depthwise taps of a carrier that holds them in no f16
    wblob   //!< the carrier's f16 halfwords: the block GEMMs, the depthwise taps (permuted tap-major), an f16 CTC head
    q8      //!< the block GEMMs quantized to Q8_0 into ``qblob``, their scales at ``off / 32`` in ``qscales``; on the kq lane a Q8_0 GEMM's own blocks, moved exactly
    kq      //!< the kq lane's K-quant GEMMs: a tensor's superblocks transcoded into ``kq`` at the byte offset ``off``, its scales into ``ks`` at ``soff``
}

//! One tensor's place in a GigaAM plane - the plan the held staging and the streamed mint both replay.
struct GigaamFillJob {
    name : string
    plane : GigaamPlane
    off : int64           //!< the element offset in its plane; a ``kq`` job's byte offset in ``kq``
    n : int64
    conv_out : int64      //!< a conv weight's [out][in][k], permuted tap-major on the way in; 0 for none
    conv_in : int64
    conv_k : int64
    width : int64         //!< a GEMM's input width: the repack's row length
    fmt : int = -1        //!< a kq-lane GEMM's format (a `KqFmt` slot): a ``kq`` job's member, or `q8` for a Q8_0 GEMM moved exactly; -1 on the other lanes
    soff : int64          //!< a ``kq`` job's byte offset in ``ks``
}

//! One carrier's fill plan on one lane: every tensor's job and each plane's size - elements, or bytes for the kq planes - and
//! on the kq lane the block GEMMs' seats (`TOWER_SEAT_WORDS` words each, in layout order).
struct GigaamPlan {
    jobs : array<GigaamFillJob>
    blob_n : int64
    wblob_n : int64
    q8_n : int64
    kq_n : int64
    ks_n : int64
    seats : array<int64>
}

//! The mint-side carrier: ``e`` holds the scalars the meta blob carries, the arrays beside it the plane bytes. The mel planes
//! are always held; a weight plane is held only where `stage_gigaam_model` was asked to hold them, and the mint streams each
//! weight plane its array leaves empty off ``src``, one tensor at a time, per ``plan``.
struct GigaamStaging {
    e : GigaamModel = GigaamModel()
    @exact_size filters : array<float>
    @exact_size window : array<float>
    @exact_size blob : array<float>
    @exact_size wblob : array<uint16>
    @exact_size qblob : array<int8>
    @exact_size qscales : array<float>
    @exact_size kq : array<uint8>
    @exact_size ks : array<uint8>
    @exact_size gemm_seats : array<int64>   //!< the kq lane's seats, held through the mint: the plan writes them, the image carries them
    src : string
    plan : GigaamPlan = GigaamPlan()
}

def private serialize_image_meta(var arch : Archive; var st : GigaamStaging) {
    serialize_image_meta(arch, st.e)
}

def finalize(var st : GigaamStaging) {
    delete_staging(st)
}

[unused_argument(st)]
def private image_post_load(var st : GigaamStaging) {}

//! A block's ten GEMMs in their layout order, each as (input width, output rows).
def private gg_gemm_shapes(d, ff : int64) : array<int2> {
    return <- [int2(int(d), int(ff)), int2(int(ff), int(d)), int2(int(d), int(d)), int2(int(d), int(d)),
               int2(int(d), int(d)), int2(int(d), int(d)), int2(int(d), int(2l * d)), int2(int(d), int(d)),
               int2(int(d), int(ff)), int2(int(ff), int(d))]
}

//! a block's GEMM elements - the chains' stride a layer over the block GEMMs' record (`gigaam_layer_offsets`)
def gigaam_gemm_stride(d, ff : int64) : int64 => 4l * ff * d + 7l * d * d

//! a block's f32 rows: the norms and biases, and the depthwise taps where they are no f16 (`dw_in_aux`)
def private gg_aux_stride(d, ff, nk : int64; dw_in_aux : bool) : int64 => 22l * d + 2l * ff + (dw_in_aux ? nk * d : 0l)

//! Block `l`'s offsets on the FastConformer chains' record: the ten GEMMs index ``qblob`` on a q8 load, ``wblob`` where ``gemm_f16``,
//! ``blob`` otherwise, and on the kq lane a virtual layout `fc_gemm_seat_index` maps to the seats; the depthwise taps ``wblob``
//! where ``dw_f16`` and ``blob`` otherwise, everything else ``blob``; the conv LayerNorm rows sit in the BatchNorm slots, GigaAM's absent slots at -1.
def gigaam_layer_offsets(m : GigaamModel; l : int64) : CanaryLayerOffs {
    let d = m.d_model
    let ff = m.n_ff
    var a = m.aux_off + l * m.aux_stride
    var g = m.gemm_off + l * m.gemm_stride
    var r = CanaryLayerOffs(pos_w = -1l, bias_u = -1l, bias_v = -1l, bn_mean = -1l, bn_var = -1l)
    r.nf1_w = a; a += d
    r.nf1_b = a; a += d
    r.nat_w = a; a += d
    r.nat_b = a; a += d
    r.ncv_w = a; a += d
    r.ncv_b = a; a += d
    r.nf2_w = a; a += d
    r.nf2_b = a; a += d
    r.nout_w = a; a += d
    r.nout_b = a; a += d
    r.ff1a_b = a; a += ff
    r.ff1b_b = a; a += d
    r.q_b = a; a += d
    r.k_b = a; a += d
    r.v_b = a; a += d
    r.o_b = a; a += d
    r.pw1_b = a; a += 2l * d
    if (m.dw_f16) {
        r.dw_w = m.dw_off + l * m.conv_kernel * d
    } else {
        r.dw_w = a
        a += m.conv_kernel * d
    }
    r.dw_b = a; a += d
    r.bn_w = a; a += d
    r.bn_b = a; a += d
    r.pw2_b = a; a += d
    r.ff2a_b = a; a += ff
    r.ff2b_b = a
    r.ff1a_w = g; g += ff * d
    r.ff1b_w = g; g += d * ff
    r.q_w = g; g += d * d
    r.k_w = g; g += d * d
    r.v_w = g; g += d * d
    r.o_w = g; g += d * d
    r.pw1_w = g; g += 2l * d * d
    r.pw2_w = g; g += d * d
    r.ff2a_w = g; g += ff * d
    r.ff2b_w = g
    return r
}

def private gg_take(var cursor : int64&; n : int64) : int64 {
    let at = cursor
    cursor += n
    return at
}

//! a tensor's job in `plane` at `off`, its element count checked against the file
def private gg_job(var p : GigaamPlan; m : GGUFMeta; name : string; plane : GigaamPlane; off, n : int64) {
    gguf_expect_elems(m, name, n, GG_WHO)
    p.jobs |> push(GigaamFillJob(name = name, plane = plane, off = off, n = n))
}

//! a conv weight's job: [nout][cin][k] in the file, tap-major [nout][k][cin] in `plane` - ``blob`` widened, or ``wblob`` as the
//! carrier's halfwords
def private gg_conv_job(var p : GigaamPlan; m : GGUFMeta; name : string; plane : GigaamPlane; off, nout, cin, k : int64) {
    gguf_expect_elems(m, name, nout * cin * k, GG_WHO)
    p.jobs |> push(GigaamFillJob(name = name, plane = plane, off = off, n = nout * cin * k, conv_out = nout, conv_in = cin, conv_k = k))
}

//! Whether every block's depthwise taps are f16 - they then serve off ``wblob`` as the carrier's halfwords, else widened in
//! ``blob``. Refuses a carrier whose taps mix f16 with another element type, naming both tensors.
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def private gg_dw_f16(m : GGUFMeta; n_layer : int64) : bool {
    let first = "encoder.layers.0.conv.dw.weight"
    let f16 = gguf_tensor_type(m, first) == GGML_TYPE_F16
    for (l in range64(1l, n_layer)) {
        let name = "encoder.layers.{l}.conv.dw.weight"
        if ((gguf_tensor_type(m, name) == GGML_TYPE_F16) != f16) {
            panic("{GG_WHO}: the depthwise taps mix element types - '{name}' is {ggml_type_name(gguf_tensor_type(m, name))}, '{first}' is {ggml_type_name(gguf_tensor_type(m, first))}; one plane serves them")
        }
    }
    return f16
}

[arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def gg_kq_carrier(m : GGUFMeta; n_layer : int64) : bool {
    var quant = ""
    var flt = ""
    for (l in range64(n_layer)) {
        for (g in GG_GEMM_NAMES) {
            let name = "encoder.layers.{l}.{g}.weight"
            let gt = gguf_tensor_type(m, name)
            if (gt == GGML_TYPE_F32 || gt == GGML_TYPE_F16 || gt == GGML_TYPE_BF16) {
                flt = name
            } else {
                quant = name
            }
        }
    }
    if (!empty(quant) && !empty(flt)) {
        panic("{GG_WHO}: the block GEMMs mix quantized and float tensors - '{quant}' is {ggml_type_name(gguf_tensor_type(m, quant))}, '{flt}' is {ggml_type_name(gguf_tensor_type(m, flt))}; one lane serves them - requantize the file")
    }
    return !empty(quant)
}

//! the block layers the carrier holds, counted by their first GEMM - the lane peek's layer count, read with no hparams
def private gg_count_layers(m : GGUFMeta) : int64 {
    var n = 0l
    while (gguf_find_tensor(m, "encoder.layers.{n}.{GG_GEMM_NAMES[0]}.weight") >= 0) {
        n++
    }
    return n
}

[arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def private gg_gemm_plane(m : GGUFMeta; n_layer : int64; q8, kq : bool) : GigaamPlane {
    return GigaamPlane.kq if (kq)
    return GigaamPlane.q8 if (q8)
    var first = ""
    var first_f16 = false
    for (l in range64(n_layer)) {
        for (g in GG_GEMM_NAMES) {
            let name = "encoder.layers.{l}.{g}.weight"
            let f16 = gguf_tensor_type(m, name) == GGML_TYPE_F16
            if (empty(first)) {
                first = name
                first_f16 = f16
            } elif (f16 != first_f16) {
                panic("{GG_WHO}: the block GEMMs mix element types - '{name}' is {ggml_type_name(gguf_tensor_type(m, name))}, '{first}' is {ggml_type_name(gguf_tensor_type(m, first))}; one plane serves them")
            }
        }
    }
    return first_f16 ? GigaamPlane.wblob : GigaamPlane.blob
}

//! A block's ten GEMM tensors in their layout order (`gg_gemm_shapes`), by name under ``encoder.layers.<l>.``.
let private GG_GEMM_NAMES = fixed_array("ff1.linear1", "ff1.linear2", "attn.q", "attn.k", "attn.v", "attn.out", "conv.pw1", "conv.pw2",
                                        "ff2.linear1", "ff2.linear2")

def private gg_expect(ok : bool; what : string) {
    if (!ok) {
        panic("{GG_WHO}: {what}")
    }
}

//! The header fields the loader refuses a carrier on, as the GGUF spells them; `n_tokens` is the token table's length.
struct GigaamHparams {
    sample_rate : int64
    mel_center : bool
    attention : string
    conv_norm : string
    n_fft : int64
    win_length : int64
    d_model : int64
    n_head : int64
    head_dim : int64
    n_ff : int64
    subsampling : int64
    n_sub_ch : int64
    subs_kernel : int64
    conv_kernel : int64
    n_classes : int64
    n_vocab : int64
    blank_id : int64
    head : string
    tokenizer : string
    n_tokens : int64
    pred_layers : int64
}

//! Refuses a GGUF whose architecture is not gigaam.
def gigaam_check_arch(arch, path : string) {
    gg_expect(arch == GIGAAM_ARCH, "'{path}' is a '{arch}' GGUF, not {GIGAAM_ARCH}")
}

//! Refuses a header the served encoder and decoders do not take; `pred_layers` is read on an rnnt head alone.
def gigaam_check_hparams(h : GigaamHparams) {
    gg_expect(h.sample_rate == AUDIO_SAMPLE_RATE, "the sample rate is not 16 kHz")
    gg_expect(!h.mel_center, "a centered STFT is not the v3 front")
    gg_expect(h.attention == "rotary", "only rotary attention is served")
    gg_expect(h.conv_norm == "layer_norm", "only the layer_norm conv module is served")
    gg_expect(h.win_length == h.n_fft, "the window length differs from n_fft")
    gg_expect(h.n_head * h.head_dim == h.d_model, "n_heads x head_dim is not d_model")
    gg_expect(h.d_model % 32l == 0l, "d_model {h.d_model} is not a multiple of 32 - the Q8_0 blocks tile it")
    gg_expect(h.head_dim % 2l == 0l, "head_dim {h.head_dim} is odd - the rotary pairs need an even head")
    gg_expect(h.n_ff % 32l == 0l, "ff_dim {h.n_ff} is not a multiple of 32 - the Q8_0 blocks tile it")
    gg_expect(h.subsampling == 4l, "the front is two stride-2 convs: subsampling 4 only")
    gg_expect(h.n_sub_ch == h.d_model, "the front has no projection: its channels must be d_model")
    gg_expect(h.subs_kernel % 2l == 1l && h.conv_kernel % 2l == 1l, "even conv kernels have no centered pad")
    gg_expect(h.n_classes == h.n_vocab + 1l && h.blank_id == h.n_vocab, "the blank is not the class past the vocab")
    gg_expect(h.head == "ctc" || h.head == "rnnt", "head type '{h.head}' is neither ctc nor rnnt")
    gg_expect(h.tokenizer == "spm" || h.tokenizer == "charwise", "tokenizer type '{h.tokenizer}' is neither spm nor charwise")
    gg_expect(h.n_tokens == h.n_vocab, "the token table has {h.n_tokens} entries, the vocab {h.n_vocab}")
    gg_expect(h.head != "rnnt" || h.pred_layers == 1l, "the predictor has {h.pred_layers} layers - the v3 carriers ship one")
}

def private gg_read_hparams(m : GGUFMeta; b : array<uint8> | #; var e : GigaamModel) {
    e.vocab <- gguf_str_array(m, b, "tokenizer.ggml.tokens")
    let head = gguf_str(m, b, "gigaam.head_type")
    let h = GigaamHparams(
        sample_rate = gguf_int(m, b, "gigaam.sample_rate"), mel_center = gguf_bool_or(m, b, "gigaam.mel_center", false),
        attention = gguf_str(m, b, "gigaam.self_attention_model"), conv_norm = gguf_str(m, b, "gigaam.conv_norm_type"),
        n_fft = gguf_int(m, b, "gigaam.n_fft"), win_length = gguf_int(m, b, "gigaam.win_length"),
        d_model = gguf_int(m, b, "gigaam.d_model"), n_head = gguf_int(m, b, "gigaam.n_heads"), head_dim = gguf_int(m, b, "gigaam.head_dim"),
        n_ff = gguf_int(m, b, "gigaam.ff_dim"), subsampling = gguf_int(m, b, "gigaam.subsampling_factor"),
        n_sub_ch = gguf_int(m, b, "gigaam.subsampling_channels"), subs_kernel = gguf_int(m, b, "gigaam.subs_kernel"),
        conv_kernel = gguf_int(m, b, "gigaam.conv_kernel"), n_classes = gguf_int(m, b, "gigaam.num_classes"),
        n_vocab = gguf_int(m, b, "gigaam.vocab_size"), blank_id = gguf_int(m, b, "gigaam.blank_id"),
        head = head, tokenizer = gguf_str(m, b, "gigaam.tokenizer_type"), n_tokens = long_length(e.vocab),
        pred_layers = head == "rnnt" ? gguf_int(m, b, "gigaam.pred_layers") : 1l)
    gigaam_check_hparams(h)
    e.n_mel = gguf_int(m, b, "gigaam.n_mels")
    e.n_fft = h.n_fft
    e.hop = gguf_int(m, b, "gigaam.hop_length")
    e.d_model = h.d_model
    e.n_layer = gguf_int(m, b, "gigaam.n_layers")
    e.n_head = h.n_head
    e.head_dim = h.head_dim
    e.n_ff = h.n_ff
    e.n_sub_ch = h.n_sub_ch
    e.subs_kernel = h.subs_kernel
    e.conv_kernel = h.conv_kernel
    e.rope_base = gguf_f32(m, b, "gigaam.rope_base")
    e.frame_cs = gguf_int(m, b, "gigaam.frame_dur_cs")
    e.n_classes = h.n_classes
    e.n_vocab = h.n_vocab
    e.blank_id = int(h.blank_id)
    e.head = head == "ctc" ? GigaamHead.ctc : GigaamHead.rnnt
    e.spm = h.tokenizer == "spm"
    if (e.head == GigaamHead.rnnt) {
        e.pred_dim = gguf_int(m, b, "gigaam.pred_hidden")
        e.pred_layers = h.pred_layers
        e.joint_dim = gguf_int(m, b, "gigaam.joint_hidden")
    }
}

//! Every weight tensor's job on the lane, its offset set in the served model ``e``: the f32 plane laid out front convs,
//! then the blocks' aux rows, then (where no other plane takes them) the block GEMMs, then the head; the halfword plane the
//! block GEMMs where they are f16, then the depthwise taps where they are f16, then an f16 CTC head's padded rows.
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def private gg_plan(m : GGUFMeta; var e : GigaamModel; q8, kq : bool) : GigaamPlan {
    var p = GigaamPlan()
    let d = e.d_model
    let ff = e.n_ff
    let nk = e.conv_kernel
    let ks = e.subs_kernel
    let nch = e.n_sub_ch
    let gplane = gg_gemm_plane(m, e.n_layer, q8, kq)
    e.gemm_f16 = gplane == GigaamPlane.wblob
    e.dw_f16 = gg_dw_f16(m, e.n_layer)
    var ao = 0l
    e.c0_w = gg_take(ao, nch * ks * e.n_mel)
    e.c0_b = gg_take(ao, nch)
    e.c2_w = gg_take(ao, d * ks * nch)
    e.c2_b = gg_take(ao, d)
    e.aux_stride = gg_aux_stride(d, ff, nk, !e.dw_f16)
    e.aux_off = gg_take(ao, e.n_layer * e.aux_stride)
    e.gemm_stride = gigaam_gemm_stride(d, ff)
    let gemm_n = e.n_layer * e.gemm_stride
    e.gemm_off = gplane == GigaamPlane.blob ? gg_take(ao, gemm_n) : 0l
    let nc = e.n_classes
    let pd = e.pred_dim
    let jd = e.joint_dim
    var hw = gplane == GigaamPlane.wblob ? gemm_n : 0l
    e.dw_off = e.dw_f16 ? gg_take(hw, e.n_layer * nk * d) : 0l
    if (e.head == GigaamHead.ctc) {
        e.ctc_f16 = gguf_tensor_type(m, "head.ctc.weight") == GGML_TYPE_F16
        e.ctc_w = e.ctc_f16 ? gg_take(hw, round_up(nc, GIGAAM_CTC_ROW_TILE) * d) : gg_take(ao, nc * d)
        e.ctc_b = gg_take(ao, nc)
    } else {
        e.embed_w = gg_take(ao, nc * pd)
        e.lstm_ih = gg_take(ao, 4l * pd * pd)
        e.lstm_hh = gg_take(ao, 4l * pd * pd)
        e.lstm_b_ih = gg_take(ao, 4l * pd)
        e.lstm_b_hh = gg_take(ao, 4l * pd)
        e.jenc_w = gg_take(ao, jd * d)
        e.jenc_b = gg_take(ao, jd)
        e.jpred_w = gg_take(ao, jd * pd)
        e.jpred_b = gg_take(ao, jd)
        e.jout_w = gg_take(ao, nc * jd)
        e.jout_b = gg_take(ao, nc)
    }
    p.blob_n = ao
    p.wblob_n = hw
    p.q8_n = gplane == GigaamPlane.q8 ? gemm_n : 0l
    let fb = GigaamPlane.blob
    gg_conv_job(p, m, "encoder.pre.conv.0.weight", fb, e.c0_w, nch, e.n_mel, ks)
    gg_job(p, m, "encoder.pre.conv.0.bias", fb, e.c0_b, nch)
    gg_conv_job(p, m, "encoder.pre.conv.2.weight", fb, e.c2_w, d, nch, ks)
    gg_job(p, m, "encoder.pre.conv.2.bias", fb, e.c2_b, d)
    let shapes <- gg_gemm_shapes(d, ff)
    var q8_elem_cur = 0l
    var kq_byte_cur = 0l
    var ks_byte_cur = 0l
    for (l in range64(e.n_layer)) {
        let lo = gigaam_layer_offsets(e, l)
        let lp = "encoder.layers.{l}."
        gg_job(p, m, "{lp}norm_ff1.weight", fb, lo.nf1_w, d)
        gg_job(p, m, "{lp}norm_ff1.bias", fb, lo.nf1_b, d)
        gg_job(p, m, "{lp}norm_attn.weight", fb, lo.nat_w, d)
        gg_job(p, m, "{lp}norm_attn.bias", fb, lo.nat_b, d)
        gg_job(p, m, "{lp}norm_conv.weight", fb, lo.ncv_w, d)
        gg_job(p, m, "{lp}norm_conv.bias", fb, lo.ncv_b, d)
        gg_job(p, m, "{lp}norm_ff2.weight", fb, lo.nf2_w, d)
        gg_job(p, m, "{lp}norm_ff2.bias", fb, lo.nf2_b, d)
        gg_job(p, m, "{lp}norm_out.weight", fb, lo.nout_w, d)
        gg_job(p, m, "{lp}norm_out.bias", fb, lo.nout_b, d)
        gg_job(p, m, "{lp}ff1.linear1.bias", fb, lo.ff1a_b, ff)
        gg_job(p, m, "{lp}ff1.linear2.bias", fb, lo.ff1b_b, d)
        gg_job(p, m, "{lp}attn.q.bias", fb, lo.q_b, d)
        gg_job(p, m, "{lp}attn.k.bias", fb, lo.k_b, d)
        gg_job(p, m, "{lp}attn.v.bias", fb, lo.v_b, d)
        gg_job(p, m, "{lp}attn.out.bias", fb, lo.o_b, d)
        gg_job(p, m, "{lp}conv.pw1.bias", fb, lo.pw1_b, 2l * d)
        gg_conv_job(p, m, "{lp}conv.dw.weight", e.dw_f16 ? GigaamPlane.wblob : fb, lo.dw_w, 1l, d, nk)
        gg_job(p, m, "{lp}conv.dw.bias", fb, lo.dw_b, d)
        gg_job(p, m, "{lp}conv.ln.weight", fb, lo.bn_w, d)
        gg_job(p, m, "{lp}conv.ln.bias", fb, lo.bn_b, d)
        gg_job(p, m, "{lp}conv.pw2.bias", fb, lo.pw2_b, d)
        gg_job(p, m, "{lp}ff2.linear1.bias", fb, lo.ff2a_b, ff)
        gg_job(p, m, "{lp}ff2.linear2.bias", fb, lo.ff2b_b, d)
        let woffs = fixed_array(lo.ff1a_w, lo.ff1b_w, lo.q_w, lo.k_w, lo.v_w, lo.o_w, lo.pw1_w, lo.pw2_w, lo.ff2a_w, lo.ff2b_w)
        if (gplane == GigaamPlane.kq) {
            gg_kq_gemm_jobs(p, m, lp, shapes, q8_elem_cur, kq_byte_cur, ks_byte_cur)
            continue
        }
        for (g, s, wo in GG_GEMM_NAMES, shapes, woffs) {
            gg_job(p, m, "{lp}{g}.weight", gplane, wo, int64(s.x) * int64(s.y))
            p.jobs[length(p.jobs) - 1].width = int64(s.x)
        }
    }
    if (gplane == GigaamPlane.kq) {
        p.q8_n = q8_elem_cur
        p.kq_n = kq_byte_cur
        p.ks_n = ks_byte_cur
        e.kq_fmt = gg_kq_fmt(p.seats)
    }
    if (e.head == GigaamHead.ctc) {
        gg_job(p, m, "head.ctc.weight", e.ctc_f16 ? GigaamPlane.wblob : fb, e.ctc_w, nc * d)
        gg_job(p, m, "head.ctc.bias", fb, e.ctc_b, nc)
    } else {
        gg_job(p, m, "decoder.embed.weight", fb, e.embed_w, nc * pd)
        gg_job(p, m, "decoder.lstm.0.w_ih", fb, e.lstm_ih, 4l * pd * pd)
        gg_job(p, m, "decoder.lstm.0.w_hh", fb, e.lstm_hh, 4l * pd * pd)
        gg_job(p, m, "decoder.lstm.0.b_ih", fb, e.lstm_b_ih, 4l * pd)
        gg_job(p, m, "decoder.lstm.0.b_hh", fb, e.lstm_b_hh, 4l * pd)
        gg_job(p, m, "joint.enc.weight", fb, e.jenc_w, jd * d)
        gg_job(p, m, "joint.enc.bias", fb, e.jenc_b, jd)
        gg_job(p, m, "joint.pred.weight", fb, e.jpred_w, jd * pd)
        gg_job(p, m, "joint.pred.bias", fb, e.jpred_b, jd)
        gg_job(p, m, "joint.out.weight", fb, e.jout_w, nc * jd)
        gg_job(p, m, "joint.out.bias", fb, e.jout_b, nc)
    }
    return <- p
}

[arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def private gg_kq_seat_fmt(m : GGUFMeta; name : string; width : int64) : KqFmt {
    let gt = gguf_tensor_type(m, name)
    let f = kq_tag(m, name, width)
    let served = f == KqFmt.q8 ? gt == GGML_TYPE_Q8_0 : (kq_sb(f) && f != KqFmt.nv4)
    if (!served) {
        panic("{GG_WHO}: '{name}' is {ggml_type_name(gt)} on a {width}-wide row, which the block GEMMs' kq lane has no seat for - the superblock formats and Q8_0 serve as their own planes; pin a lane (set_gigaam_q8) to dequantize it")
    }
    return f
}

//! a block's ten kq-lane GEMM jobs and their seat words, each GEMM at the cursor its format takes (Q8_0 elements, or K-quant quant and scale bytes), advancing it
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def private gg_kq_gemm_jobs(var p : GigaamPlan; m : GGUFMeta; lp : string; shapes : array<int2>; var q8_elem_cur, kq_byte_cur, ks_byte_cur : int64&) {
    var qkv_q8 = fixed_array(false, false, false)
    for (g, s, i in GG_GEMM_NAMES, shapes, count()) {
        let name = "{lp}{g}.weight"
        let width = int64(s.x)
        let n = width * int64(s.y)
        gguf_expect_elems(m, name, n, GG_WHO)
        let f = gg_kq_seat_fmt(m, name, width)
        if (i >= 2 && i <= 4) {
            qkv_q8[i - 2] = f == KqFmt.q8
        }
        if (f == KqFmt.q8) {
            p.jobs |> push(GigaamFillJob(name = name, plane = GigaamPlane.q8, off = q8_elem_cur, n = n, width = width, fmt = int(f), soff = q8_elem_cur / KQ_B32_ELEMS))
            p.seats |> push(int64(int(f)))   // nolint:STYLE012 - one seat's three words appended behind the GEMMs before it
            p.seats |> push(q8_elem_cur)
            p.seats |> push(q8_elem_cur / KQ_B32_ELEMS)
            q8_elem_cur += n
        } else {
            let nsb = n / kq_elems(f)
            p.jobs |> push(GigaamFillJob(name = name, plane = GigaamPlane.kq, off = kq_byte_cur, n = n, width = width, fmt = int(f), soff = ks_byte_cur))
            p.seats |> push(int64(int(f)))   // nolint:STYLE012 - one seat's three words appended behind the GEMMs before it
            p.seats |> push(kq_byte_cur)
            p.seats |> push(ks_byte_cur)
            kq_byte_cur += nsb * kq_qsb(f)
            ks_byte_cur += nsb * kq_ssb(f)
        }
    }
    if (qkv_q8[0] != qkv_q8[1] || qkv_q8[0] != qkv_q8[2]) {
        panic("{GG_WHO}: '{lp}attn.q', '{lp}attn.k' and '{lp}attn.v' mix Q8_0 with a K-quant format - the three read one activation image, so one class serves them; pin a lane (set_gigaam_q8) to dequantize them")
    }
}

//! the seats' one K-quant member (a `KqFmt` slot), `q8` where every seat is Q8_0, -1 where the K-quant seats mix
def private gg_kq_fmt(seats : array<int64>) : int {
    var fmt = -2
    for (s in range64(long_length(seats) / TOWER_SEAT_WORDS)) {
        let f = int(seats[s * TOWER_SEAT_WORDS])
        continue if (f == int(KqFmt.q8))
        if (fmt == -2) {
            fmt = f
        } elif (fmt != f) {
            return -1
        }
    }
    return fmt == -2 ? int(KqFmt.q8) : fmt
}

//! one job's tensor into its plane's array at `at` (a K-quant job's scales at `sat`), every quantized region repacked where `rep`
def private gg_fill(m : GGUFMeta; b : array<uint8> | #; j : GigaamFillJob; at, sat : int64; rep : bool; var blob : array<float>;
                    var wblob : array<uint16>; var qblob : array<int8>; var qscales : array<float>; var kq, ks : array<uint8>;
                    @scratch var wscratch : array<float>) {
    if (j.plane == GigaamPlane.wblob && j.conv_out != 0l) {
        gg_conv_f16_tap_major(m, b, j, wblob, at)
    } elif (j.plane == GigaamPlane.wblob) {
        gg_stream_f16(m, b, j.name) $(p, nbytes) {
            unsafe {
                memcpy(addr(wblob[at]), p, int64(nbytes))
            }
        }
    } elif (j.plane == GigaamPlane.kq) {
        let f = kq_fmt_of_slot(j.fmt)
        gguf_transcode_kq_at(m, b, j.name, f, kq, ks, at, sat, j.n)
        if (rep && kernel_backend_has_kq()) {
            unsafe {
                repack_kq_weight_at(kq_schema_id(f), addr(kq[at]), addr(ks[sat]), j.width, j.n / j.width)
            }
        }
    } elif (j.plane == GigaamPlane.q8 && j.fmt >= 0) {
        gguf_transcode_q8_0(m, b, j.name, qblob, qscales, at, at / KQ_B32_ELEMS, j.n)
        if (rep) {
            repack_q8q8_weight(qblob, qscales, at, j.width, j.n / j.width)
        }
    } elif (j.plane == GigaamPlane.q8) {
        tower_read_gemm_q8(m, b, j.name, qblob, qscales, wscratch, at, j.n, GG_WHO)
        if (rep) {
            repack_q8q8_weight(qblob, qscales, at, j.width, j.n / j.width)
        }
    } elif (j.conv_out != 0l) {
        tower_read_conv_tap_major(m, b, j.name, blob, at, j.conv_out, j.conv_in, j.conv_k, GIGAAM_ARCH)
    } else {
        gguf_read_tensor_f32(m, b, j.name, blob, at, j.n)
    }
}

//! an f16 conv weight's halfwords off the mapping into `dst` at `at`, permuted tap-major ([nout][k][cin] from the file's
//! [nout][cin][k]): the bits moved, none converted
def private gg_conv_f16_tap_major(m : GGUFMeta; b : array<uint8> | #; j : GigaamFillJob; var dst : array<uint16>; at : int64) {
    var inscope raw : array<uint16>
    gg_stream_f16(m, b, j.name) $(p, nbytes) {
        raw |> resize(int64(nbytes / 2ul))
        unsafe {
            memcpy(addr(raw[0]), p, int64(nbytes))
        }
    }
    gg_expect(long_length(raw) == j.n, "'{j.name}' holds {length(raw)} halfwords, the plan {j.n}")
    repack_conv_out_tap_in(raw, dst, at, j.conv_out, j.conv_in, j.conv_k)
}

//! an f16 tensor's bytes handed to `sink` straight off the mapping
def private gg_stream_f16(m : GGUFMeta; b : array<uint8> | #; name : string; sink : block<(p : void?; nbytes : uint64) : void>) {
    let ti = gguf_find_tensor(m, name)
    gg_expect(ti >= 0 && m.tensors[ti].ggml_type == GGML_TYPE_F16, "'{name}' is not an f16 tensor of the carrier")
    gguf_stream_tensor_raw(m, b, ti, sink)
}

//! a job's span in its plane's image section: the byte offset and byte count of its quants, or with `scales` of its scales
def private gg_job_span(j : GigaamFillJob; scales : bool) : tuple<off : int64; bytes : int64> {
    return (off = j.off * 4l, bytes = j.n * 4l) if (j.plane == GigaamPlane.blob)
    return (off = j.off * 2l, bytes = j.n * 2l) if (j.plane == GigaamPlane.wblob)
    if (j.plane == GigaamPlane.kq) {
        let f = kq_fmt_of_slot(j.fmt)
        let nsb = j.n / kq_elems(f)
        return scales ? (off = j.soff, bytes = nsb * kq_ssb(f)) : (off = j.off, bytes = nsb * kq_qsb(f))
    }
    return scales ? (off = (j.off / KQ_B32_ELEMS) * 4l, bytes = (j.n / KQ_B32_ELEMS) * 4l) : (off = j.off, bytes = j.n)
}

//! a plane's image section's byte count - the quants', or with `scales` the scales'
def private gg_plane_total(plan : GigaamPlan; plane : GigaamPlane; scales : bool) : int64 {
    return plan.blob_n * 4l if (plane == GigaamPlane.blob)
    return plan.wblob_n * 2l if (plane == GigaamPlane.wblob)
    return (scales ? plan.ks_n : plan.kq_n) if (plane == GigaamPlane.kq)
    return scales ? (plan.q8_n / KQ_B32_ELEMS) * 4l : plan.q8_n
}

//! One weight plane of the streamed mint as section `name`: its jobs' tensors in offset order, each read off the mapping into
//! scratch and appended, a span no job fills - a gap or the plane's tail - written as zeros; `scales` writes a quantized
//! plane's scales in place of its quants.
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def private gg_stream_plane(m : GGUFMeta; b : array<uint8> | #; plan : GigaamPlan; plane : GigaamPlane; scales, rep : bool;
                            var w : ImgWriter; var sections : array<ImgSection>; name : string) : bool {
    let at = w_page(w)
    var mine <- [for (j in plan.jobs); j; where j.plane == plane]
    mine |> sort() <| cmp_by(_.off)   // the scale offsets grow with the quant offsets, so one order serves both sections
    var @scratch @exact_size f32s : array<float>
    var @scratch @exact_size halves : array<uint16>
    var @scratch @exact_size q8s : array<int8>
    var @scratch @exact_size qss : array<float>
    var @scratch @exact_size kqs : array<uint8>
    var @scratch @exact_size kss : array<uint8>
    var @scratch @exact_size wscratch : array<float>
    var cur = 0l
    for (j in mine) {
        let span = gg_job_span(j, scales)
        if (span.off > cur) {
            w_zeros(w, uint64(span.off - cur))
        }
        cur = span.off + span.bytes
        if (plane == GigaamPlane.wblob && j.conv_out == 0l) {
            gg_stream_f16(m, b, j.name) $(p, nbytes) {
                w_append(w, p, nbytes)
            }
            continue
        }
        if (plane == GigaamPlane.wblob) {
            ensure_length(halves, j.n)
        } elif (plane == GigaamPlane.blob) {
            ensure_length(f32s, j.n)
        } elif (plane == GigaamPlane.kq) {
            ensure_length(kqs, gg_job_span(j, false).bytes)
            ensure_length(kss, gg_job_span(j, true).bytes)
        } else {
            ensure_length(q8s, j.n)
            ensure_length(qss, j.n / KQ_B32_ELEMS)
        }
        gg_fill(m, b, j, 0l, 0l, rep, f32s, halves, q8s, qss, kqs, kss, wscratch)
        unsafe {
            if (plane == GigaamPlane.wblob) {
                w_append(w, addr<void?>(halves[0]), uint64(span.bytes))
            } elif (plane == GigaamPlane.blob) {
                w_append(w, addr<void?>(f32s[0]), uint64(span.bytes))
            } elif (plane == GigaamPlane.kq && scales) {
                w_append(w, addr<void?>(kss[0]), uint64(span.bytes))
            } elif (plane == GigaamPlane.kq) {
                w_append(w, addr<void?>(kqs[0]), uint64(span.bytes))
            } elif (scales) {
                w_append(w, addr<void?>(qss[0]), uint64(span.bytes))
            } else {
                w_append(w, addr<void?>(q8s[0]), uint64(span.bytes))
            }
        }
    }
    let total = gg_plane_total(plan, plane, scales)
    if (total > cur) {
        w_zeros(w, uint64(total - cur))
    }
    delete mine
    delete f32s
    delete halves
    delete q8s
    delete qss
    delete kqs
    delete kss
    delete wscratch
    sections |> push(ImgSection(name = name, off = at, bytes = w.cur - at))
    return w.ok
}

//! The GGUF reader proper: the scalars into the served model, the mel planes and the fill plan into the staging; a quantized
//! carrier takes the kq lane unless a lane is `pinned`, where `q8` names it. With `hold` every weight plane is read into the
//! staging's arrays too (a control that edits a plane before the mint; the carrier widened in RAM), else the mint streams them one tensor at a time.
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def stage_gigaam_model(path : string; q8 : bool = false; hold : bool = false; pinned : bool = false) : GigaamStaging {
    var st = GigaamStaging(src = path)
    let f = fopen(path, "rb")
    if (f == null) {
        panic("{GG_WHO}: cannot open model '{path}'")
    }
    if (hold) {
        refuse_staged_source_past_cap(path, GG_WHO, "stage it without `hold` - the mint streams its planes")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        gigaam_check_arch(gguf_str_or(m, bytes, "general.architecture", ""), path)
        gg_read_hparams(m, bytes, st.e)
        st.e.kq_lane = !pinned && gg_kq_carrier(m, st.e.n_layer)
        st.e.q8 = q8 && !st.e.kq_lane
        let n_bins = st.e.n_fft / 2l + 1l
        st.filters |> reserve(st.e.n_mel * n_bins)
        st.filters |> resize(st.e.n_mel * n_bins)
        gguf_expect_elems(m, "preprocessor.fb", st.e.n_mel * n_bins, GG_WHO)
        gguf_read_tensor_f32(m, bytes, "preprocessor.fb", st.filters, 0l, st.e.n_mel * n_bins)
        st.window |> reserve(st.e.n_fft)
        st.window |> resize(st.e.n_fft)
        gguf_expect_elems(m, "preprocessor.window", st.e.n_fft, GG_WHO)
        gguf_read_tensor_f32(m, bytes, "preprocessor.window", st.window, 0l, st.e.n_fft)
        st.plan <- gg_plan(m, st.e, st.e.q8, st.e.kq_lane)
        st.gemm_seats := st.plan.seats
        if (hold) {
            gg_hold_planes(m, bytes, st)
        }
    }
    fclose(f)
    st.e.tabs.twiddles <- build_dft_twiddles(st.e.n_fft)
    return <- st
}

//! every weight plane of the plan read whole into the staging's arrays
def private gg_hold_planes(m : GGUFMeta; b : array<uint8> | #; var st : GigaamStaging) {
    let rep = gg_repack(st.e)
    st.blob |> reserve(st.plan.blob_n)
    st.blob |> resize(st.plan.blob_n)
    st.wblob |> reserve(st.plan.wblob_n)
    st.wblob |> resize(st.plan.wblob_n)
    st.qblob |> reserve(st.plan.q8_n)
    st.qblob |> resize(st.plan.q8_n)
    st.qscales |> reserve(st.plan.q8_n / KQ_B32_ELEMS)
    st.qscales |> resize(st.plan.q8_n / KQ_B32_ELEMS)
    st.kq |> reserve(st.plan.kq_n)
    st.kq |> resize(st.plan.kq_n)
    st.ks |> reserve(st.plan.ks_n)
    st.ks |> resize(st.plan.ks_n)
    var @scratch @exact_size wscratch : array<float>
    for (j in st.plan.jobs) {
        gg_fill(m, b, j, j.off, j.soff, rep, st.blob, st.wblob, st.qblob, st.qscales, st.kq, st.ks, wscratch)
    }
    delete wscratch
}

//! whether the quantizing lanes repack for the load's backend - selected here, as the fills need it - with `e.kq_repacked` set beside
def private gg_repack(var e : GigaamModel) : bool {
    let rep = (e.q8 || e.kq_lane) && select_matmul_backend_for_load_()
    e.kq_repacked = e.kq_lane && rep && kernel_backend_has_kq()
    return rep
}

//! The ``.dlim`` family tag of a weight lane: ``gigaam-kq`` for the kq lane, ``gigaam-q8``, or ``gigaam-f16`` for the exact lane.
def gigaam_image_tag(q8 : bool; kq : bool = false) : string => kq ? GIGAAM_KQ_TAG : (q8 ? "gigaam-q8" : "gigaam-f16")

//! The tag the load of `path` serves: a prepared image's own; a carrier whose block GEMMs are quantized planes the kq lane's
//! unless a lane is `pinned`; else the lane `q8` names.
def gigaam_lane_tag(path : string; q8, pinned : bool) : string {
    return image_family_tag(path) if (path |> ends_with(".dlim"))
    var kq = false
    if (!pinned) {
        let read = gguf_header_read(path) $(m; bytes) {
            gigaam_check_arch(gguf_str_or(m, bytes, "general.architecture", ""), path)
            kq = gg_kq_carrier(m, gg_count_layers(m))
        }
        if (!read) {
            panic("{GG_WHO}: cannot read model '{path}'")
        }
    }
    return gigaam_image_tag(q8, kq)
}

//! The kq lane's members, Q8_0's `q8` among them, each named once in seat order and joined by `+` - the lane announce's words.
def gigaam_kq_members(m : GigaamModel) : string {
    var seen : array<string>
    for (s in range64(length(m.gemm_seats) / TOWER_SEAT_WORDS)) {
        let name = "{kq_fmt_of_slot(int(m.gemm_seats[s * TOWER_SEAT_WORDS]))}"
        if (find_index(seen, name) < 0) {
            seen |> push(name)
        }
    }
    return join(seen, "+")
}

//! The mint of a staging into `out` through `img` ("" builds the image in memory): a weight plane the staging holds is written
//! as held, one it leaves empty streams off ``st.src`` one tensor at a time. True only when the file was written.
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def private gg_mint(var st : GigaamStaging; var out : GigaamModel; img : string) : bool {
    let tag = gigaam_image_tag(st.e.q8, st.e.kq_lane)
    let rep = gg_repack(st.e)
    var plan : GigaamPlan
    plan := st.plan
    let s_blob = empty(st.blob) && plan.blob_n > 0l
    let s_wblob = empty(st.wblob) && plan.wblob_n > 0l
    let s_q8 = empty(st.qblob) && plan.q8_n > 0l
    let s_kq = empty(st.kq) && plan.kq_n > 0l
    let page = uint64(IMAGE_PAGE)
    var extra = 0ul
    extra += s_blob ? uint64(gg_plane_total(plan, GigaamPlane.blob, false)) + page : 0ul
    extra += s_wblob ? uint64(gg_plane_total(plan, GigaamPlane.wblob, false)) + page : 0ul
    extra += s_q8 ? uint64(gg_plane_total(plan, GigaamPlane.q8, false) + gg_plane_total(plan, GigaamPlane.q8, true)) + 2ul * page : 0ul
    extra += s_kq ? uint64(plan.kq_n + plan.ks_n) + 2ul * page : 0ul
    let f = fopen(st.src, "rb")
    if (f == null) {
        panic("{GG_WHO}: cannot open model '{st.src}'")
    }
    var wrote = false
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        wrote = cache_via_image_staged_hooked(st, out, img, extra, tag, "q8") $(name : string; var w : ImgWriter; var sections : array<ImgSection>) {
            return gg_stream_plane(m, bytes, plan, GigaamPlane.blob, false, rep, w, sections, name) if (s_blob && name == "blob")
            return gg_stream_plane(m, bytes, plan, GigaamPlane.wblob, false, rep, w, sections, name) if (s_wblob && name == "wblob")
            return gg_stream_plane(m, bytes, plan, GigaamPlane.q8, false, rep, w, sections, name) if (s_q8 && name == "qblob")
            return gg_stream_plane(m, bytes, plan, GigaamPlane.q8, true, rep, w, sections, name) if (s_q8 && name == "qscales")
            return gg_stream_plane(m, bytes, plan, GigaamPlane.kq, false, rep, w, sections, name) if (s_kq && name == "kq")
            return gg_stream_plane(m, bytes, plan, GigaamPlane.kq, true, rep, w, sections, name) if (s_kq && name == "ks")
            return false
        }
    }
    fclose(f)
    delete plan
    return wrote
}

//! Mint a served model from a staged read WITHOUT touching disk - the in-memory image rail, the suites' load.
def mint_gigaam_model(var st : GigaamStaging; var out : GigaamModel; q8 : bool = false) {
    if (!st.e.kq_lane && q8 != st.e.q8) {
        panic("{GG_WHO}: the staging was read on the {gigaam_image_tag(st.e.q8)} lane, the mint asks {gigaam_image_tag(q8)}")
    }
    gg_mint(st, out, "")
}

//! Load a GigaAM v3 GGUF (or a prepared ``.dlim`` of one). A quantized carrier serves its block GEMMs as its own planes - the kq
//! lane - unless a lane is `pinned`; then ``q8`` transcodes them to Q8_0, and the exact lane, f16 taps and an f16 CTC head serve
//! the carrier's f16 (`#gigaam-planes`). A missing image is minted streaming the carrier one tensor at a time; cached like ``load_model``.
def load_gigaam_model(path : string; q8 : bool = false; pinned : bool = false) : GigaamModel {
    let tag = gigaam_lane_tag(path, q8, pinned)
    var m <- load_through_image(path, tag, GIGAAM_ARCH, "source model", type<GigaamModel>, $(img : string; var out : GigaamModel) {
        var inscope st <- stage_gigaam_model(path, q8, false, pinned)
        return gg_mint(st, out, img)
    }, $ [unused_argument(t)] (t : GigaamModel) => true, $ [unused_argument(t, src)] (t : GigaamModel; src : string) {})
    if (m.kq_lane) {
        let layout = m.kq_repacked ? "repacked for {active_kernel_backend()}" : "in disk order (the portable batch GEMM)"
        to_log(LOG_INFO, "dasLLAMA {GIGAAM_ARCH}: GEMM lane kq ({gigaam_kq_members(m)}) - the block GEMMs as the carrier's own planes, {layout}\n")
    } else {
        announce_lane(GIGAAM_ARCH, m.q8, m.gemm_f16 ? "f16 GEMM planes" : "f32 planes", "set_gigaam_q8", pinned, gigaam_gpu_exact_serves())
    }
    return <- m
}

//! The loaded lane's name: the facade's exec-format arm - the kq lane's one K-quant member (`k4`), `kq` where its seats mix.
def gigaam_exec_fmt(m : GigaamModel) : string {
    if (m.kq_lane) {
        return m.kq_fmt >= 0 ? "{kq_fmt_of_slot(m.kq_fmt)}" : "kq"
    }
    return m.q8 ? "q8" : (m.gemm_f16 ? "f16" : "f32")
}

//! A lane is pinned for the next loads (`set_gigaam_q8`); un-pinned, a quantized carrier takes the kq lane.
def gigaam_lane_pinned() : bool => g_ga_pin != GemmLane.unset

//! Every buffer an encode and a decode reuse, so a transcribe allocates nothing past its first call.
struct GigaamState {
    @scratch @exact_size mel : array<float>      //!< frame-major [n_len][n_mel]
    n_len : int64
    @scratch @exact_size xw : array<float>
    @scratch @exact_size reim : array<float>
    @scratch @exact_size col : array<float>
    @scratch @exact_size y1 : array<float>
    @scratch @exact_size x : array<float>        //!< the residual stream; the encoder rows after the last block
    @scratch @exact_size xb : array<float>
    @scratch @exact_size xr : array<float>
    @scratch @exact_size h : array<float>
    @scratch @exact_size qq : array<float>
    @scratch @exact_size kk : array<float>
    @scratch @exact_size vv : array<float>
    @scratch @exact_size ao : array<float>
    @scratch @exact_size hp : array<float>
    @scratch @exact_size att : array<float>
    @scratch @exact_size rope_cos : array<float>
    @scratch @exact_size rope_sin : array<float>
    rope_tt : int64                  //!< the frame count the rope tables hold (0 = none)
    @scratch @exact_size xqi : array<int8>
    @scratch @exact_size xsi : array<float>
    @scratch @exact_size xbsi : array<int>   //!< the kq lane's Q8_K image: its per-16 sums beside `xsi`'s per-superblock scales
    @scratch @exact_size logits : array<float>
    @scratch @exact_size picks : array<int>   //!< the CTC head's per-frame argmax
    @scratch @exact_size plogs : array<float>  //!< ... and its plog, where the device head served
    @scratch @exact_size toks : array<GigaamToken>
    @scratch @exact_size encp : array<float>      //!< the joint's encoder projection [n_frames][joint_dim]
    @scratch @exact_size dw_wide : array<float>   //!< a layer's f16 depthwise taps widened for the CPU conv (`fc_dw_bias_rows_f16`)
    rnnt : RnntState = RnntState()
    enc_gen : uint64                 //!< the device rows' generation the last encode left (`tower_rows_gen`), 0 for none
    vad : VadModel = VadModel()
    vad_ready : bool
}

[hot_path, arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-log-mel")]
def private gg_log_mel_span(samples : array<float> | #; s0, s1 : int64; m : GigaamModel; var st : GigaamState) {
    let n_in = s1 - s0
    let n_fft = m.n_fft
    let hop = m.hop
    let n_bins = n_fft / 2l + 1l
    let n_mel = m.n_mel
    st.n_len = gigaam_mel_frames(m, n_in)
    let n_len = st.n_len
    return if (n_len == 0l)
    ensure_length(st.xw, n_len * n_fft)
    unsafe {
        let sp = addr<float const?>(samples[s0])
        let wp = plane_at(m.window, 0l)
        var xwp = addr(st.xw[0])
        maybe_parallel_for(0, int(n_len), lanes_for_work(n_len * n_fft, 0)) $(rb, re) {
            unsafe {
                for (i in range64(int64(rb), int64(re))) {
                    for (j in range64(n_fft)) {
                        xwp[i * n_fft + j] = wp[j] * sp[i * hop + j]
                    }
                }
            }
        }
    }
    mel_spectrum_rows(st.xw, unsafe(addr<float const?>(m.tabs.twiddles[0])), plane_at(m.filters, 0l), n_len, n_fft, n_bins, n_mel,
                      MelForm(sum = MelSum.float_dot, log = MelLog.ln_clamp_f, floor = double(GIGAAM_MEL_FLOOR), ceil = double(GIGAAM_MEL_CEIL)),
                      st.reim, st.mel)
}

//! The log-mel of the whole clip into ``st.mel`` (``st.n_len`` frames).
def gigaam_log_mel(samples : array<float> | #; m : GigaamModel; var st : GigaamState) {
    gg_log_mel_span(samples, 0l, long_length(samples), m, st)
}

//! The output length of a stride-2 conv of odd kernel `k` with centered pad over `l` frames; no frames from none.
def gigaam_conv_out(l, k : int64) : int64 => l <= 0l ? 0l : (l + 2l * ((k - 1l) / 2l) - k) / 2l + 1l

//! The CPU front off ``st.mel``: the two stride-2 convs with their ReLUs into ``st.x``; returns the encoder frames (0 off an
//! empty mel).
def gigaam_encode_front(m : GigaamModel; var st : GigaamState) : int64 {
    return 0l if (st.n_len <= 0l)
    let k = m.subs_kernel
    let nch = m.n_sub_ch
    let d = m.d_model
    let t1 = gigaam_conv_out(st.n_len, k)
    let t2 = gigaam_conv_out(t1, k)
    ensure_length(st.col, max(t1 * k * m.n_mel, t2 * k * nch))
    ensure_length(st.y1, t1 * nch)
    ensure_length(st.x, t2 * d)
    unsafe {
        im2col_conv1d_rows(addr(st.col[0]), addr<float const?>(st.mel[0]), st.n_len, m.n_mel, t1, k, 2l)
    }
    mm_blob_b(st.y1, m.blob, m.c0_w, st.col, k * m.n_mel, nch, t1)
    bias_relu_rows(st.y1, m.blob, m.c0_b, nch, t1)
    unsafe {
        im2col_conv1d_rows(addr(st.col[0]), addr<float const?>(st.y1[0]), t1, nch, t2, k, 2l)
    }
    mm_blob_b(st.x, m.blob, m.c2_w, st.col, k * nch, d, t2)
    bias_relu_rows(st.x, m.blob, m.c2_b, d, t2)
    return t2
}

//! The rotary tables of `tt` frames into ``st.rope_cos``/``st.rope_sin`` ([tt][head_dim / 2], positions from 0); a
//! session keeps the tables of the frame count it last built.
def gigaam_rope_tables(m : GigaamModel; var st : GigaamState; tt : int64) {
    return if (st.rope_tt == tt)
    let none : array<float>
    build_rope_tabs(st.rope_cos, st.rope_sin, m.rope_base, 1.0, 1.0, none, false, 0l, tt, m.head_dim)
    st.rope_tt = tt
}

//! gigaam's CPU block form: biased GEMMs, tap-major taps - the carrier's halfwords where ``dw_f16`` - the biased depthwise conv
//! then a LayerNorm
def private gg_cpu_form(m : GigaamModel) : FcCpuForm => FcCpuForm(d = m.d_model, ff = m.n_ff, n_head = m.n_head, nk = m.conv_kernel, conv_ln = true,
    dw_taps = m.dw_f16 ? m.wblob : PlaneU16(),
    prof = FcCpuProf(ffn1 = FcFfnProf(ln = "ga.ffn.ln", a = "ga.ffn.a", silu = "ga.ffn.silu", b = "ga.ffn.b", res = "ga.ffn.res"),
        ffn = "ga.ffn", proj_ln = "ga.attn_proj.ln", proj_rq = "ga.attn_proj.rq", proj_q = "ga.attn_proj.q",
        proj_k = "ga.attn_proj.k", proj_v = "ga.attn_proj.v", proj_pos = "ga.attn_proj.rope", proj = "ga.attn_proj",
        heads = "ga.attn_heads", attn_out = "ga.attn_out", conv = "ga.conv_module"))

//! Conformer block `l` over the `tt` rows of ``st.x``, in place: macaron FFN halves around rotary attention and the
//! conv module, then the block's output LayerNorm. The rope tables must hold `tt` frames (``gigaam_rope_tables``).
def gigaam_encode_block(m : GigaamModel; var st : GigaamState; l, tt : int64) {
    fc_block_cpu(m, st, gg_cpu_form(m), FcRotary(), gigaam_layer_offsets(m, l), l, tt)
}

//! the GPU block hook: after the CPU front and the rope tables, the conformer blocks on ``st.x``
typedef GigaamGpuBlocksFn = function<(m : GigaamModel; var st : GigaamState; tt : int64) : bool>
typedef GigaamServesFn = function<() : bool>
//! the GPU encode hook: the whole encode into ``st.x`` - off the `n_in` raw samples at `samples`, the mel on the device, or off
//! ``st.mel`` where `samples` is null; ``st.n_len`` holds the frame count either way. Answers the encoder frames, or -1 to decline
typedef GigaamGpuEncodeFn = function<(m : GigaamModel; var st : GigaamState; samples : float const?; n_in : int64) : int64>
var private g_ga_gpu : GigaamGpuBlocksFn
var private g_ga_seat = GpuLaneSeat()
var private g_ga_gpu_encode : GigaamGpuEncodeFn
var private g_ga_gpu_encode_set = false

//! `exact_lane`: the driver's halfword GEMMs are its faster lane, so the policy prefers the exact planes over the q8 image
[arch(at="../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_gigaam_gpu(fn : GigaamGpuBlocksFn; serves : GigaamServesFn; exact_lane : bool = true; kq_lane : bool = false; kq_disk : bool = false) {
    g_ga_gpu = fn
    g_ga_seat = GpuLaneSeat(set = true, f32_lane = exact_lane, serves = serves, kq_lane = kq_lane, kq_disk = kq_disk)
}

[arch(at="../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_gigaam_gpu_encode(fn : GigaamGpuEncodeFn) {
    g_ga_gpu_encode = fn
    g_ga_gpu_encode_set = true
}

//! Would a GPU driver serve the block loop; false with no driver or the knob off.
def gigaam_gpu_serves() : bool => g_ga_seat.set && invoke(g_ga_seat.serves)

//! Would a GPU driver serve the block loop with the exact planes its faster lane - the lane policy's question.
def gigaam_gpu_exact_serves() : bool => gpu_exact_serves(g_ga_seat)

//! Would a GPU driver serve the loaded model's encode: the q8 lane, the halfword planes, or the kq lane where the driver's seat
//! takes it (in disk order alone where it reads them so), with a driver whose knob is on - the facade's device-lane arm.
def gigaam_device_gpu(m : GigaamModel) : bool
    => (m.q8 || m.gemm_f16 || (m.kq_lane && g_ga_seat.kq_lane && !(m.kq_repacked && g_ga_seat.kq_disk))) && gigaam_gpu_serves()

var private g_ga_pin = GemmLane.unset

//! Pin the weight lane for subsequent loads: q8 (the CPU serving lane) or the exact f16 planes.
def set_gigaam_q8(on : bool) {
    g_ga_pin = lane_pin(on)
}

def reset_gigaam_q8() {
    g_ga_pin = GemmLane.unset
}

[arch(at="../ARCHITECTURE_MEDIA.md#tower-weight-lane")]
def gigaam_serves_q8() : bool => lane_serves_q8(g_ga_pin, !gigaam_gpu_exact_serves())

//! The encode over ``st.mel``: the GPU encode hook first, else the CPU front, the rope tables, then the block hook or
//! the CPU blocks. Leaves the encoder rows in ``st.x``; returns their count, 0 off an empty mel.
def gigaam_encode(m : GigaamModel; var st : GigaamState) : int64 {
    st.enc_gen = 0ul
    return 0l if (st.n_len <= 0l)
    dispatch_phase_encode(gigaam_conv_out(gigaam_conv_out(st.n_len, m.subs_kernel), m.subs_kernel))
    var tt = g_ga_gpu_encode_set ? invoke(g_ga_gpu_encode, m, st, null, 0l) : -1l
    if (tt < 0l) {
        tt = gg_encode_cpu_front(m, st)
    }
    st.enc_gen = tower_rows_gen()
    return tt
}

//! The frames a span of `n_in` samples cuts (`#gigaam-log-mel`): none under one window.
def gigaam_mel_frames(m : GigaamModel; n_in : int64) : int64 => n_in >= m.n_fft ? (n_in - m.n_fft) / m.hop + 1l : 0l

//! The log-mel and the encode of the samples [s0, s1): the GPU encode hook off the raw samples first - the mel on the device,
//! ``st.mel`` left as it was - else the CPU mel into ``st.mel`` and the CPU front with the block hook behind it. Sets
//! ``st.n_len``; leaves the encoder rows in ``st.x`` and returns their count, 0 off a span under one window.
[hot_path]
def gigaam_encode_span(samples : array<float> | #; s0, s1 : int64; m : GigaamModel; var st : GigaamState) : int64 {
    st.enc_gen = 0ul
    st.n_len = gigaam_mel_frames(m, s1 - s0)
    return 0l if (st.n_len <= 0l)
    dispatch_phase_encode(gigaam_conv_out(gigaam_conv_out(st.n_len, m.subs_kernel), m.subs_kernel))
    if (g_ga_gpu_encode_set) {
        let tp = ref_time_ticks()
        let served = invoke(g_ga_gpu_encode, m, st, unsafe(addr<float const?>(samples[s0])), s1 - s0)
        if (served >= 0l) {
            asr_prof_add(GIGAAM_ENCODE_BUCKET, tp)
            st.enc_gen = tower_rows_gen()
            return served
        }
    }
    var tp = ref_time_ticks()
    gg_log_mel_span(samples, s0, s1, m, st)
    asr_prof_add("ga.mel", tp)
    tp = ref_time_ticks()
    let tt = gg_encode_cpu_front(m, st)
    asr_prof_add(GIGAAM_ENCODE_BUCKET, tp)
    st.enc_gen = tower_rows_gen()
    return tt
}

//! the encode past a declined encode hook: the CPU front off ``st.mel``, the rope tables, then the block hook or the CPU blocks
[hot_path]
def private gg_encode_cpu_front(m : GigaamModel; var st : GigaamState) : int64 {
    var tp = ref_time_ticks()
    let tt = gigaam_encode_front(m, st)
    gigaam_rope_tables(m, st, tt)
    asr_prof_add("ga.front", tp)
    tp = ref_time_ticks()
    let served = g_ga_seat.set && invoke(g_ga_gpu, m, st, tt)
    if (!served) {
        for (l in range64(m.n_layer)) {
            gigaam_encode_block(m, st, l, tt)
        }
    }
    asr_prof_add("ga.blocks", tp)
    return tt
}

//! the GPU CTC-head hook: over the `n_frames` encoder rows in ``st.x``, each frame's biased argmax - the lowest class of a
//! tie - into ``st.picks`` and its -log of sum exp(logit - max), the emitted token's plog, into ``st.plogs``; false to decline
typedef GigaamGpuCtcFn = function<(m : GigaamModel; var st : GigaamState; n_frames : int64) : bool>
var private g_ga_gpu_ctc : GigaamGpuCtcFn
var private g_ga_gpu_ctc_set = false

[arch(at="../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_gigaam_gpu_ctc(fn : GigaamGpuCtcFn) {
    g_ga_gpu_ctc = fn
    g_ga_gpu_ctc_set = true
}

//! the CTC head on the CPU: the logits GEMM, its bias, and each frame's argmax (the lowest class of a tie) into ``st.picks``
def private gg_ctc_logits_cpu(m : GigaamModel; var st : GigaamState; n_frames : int64) {
    let nc = m.n_classes
    if (m.ctc_f16) {
        mm_f16_b(st.logits, m.wblob, m.ctc_w, st.x, m.d_model, nc, n_frames)
    } else {
        mm_blob_b(st.logits, m.blob, m.ctc_w, st.x, m.d_model, nc, n_frames)
    }
    add_bias_rows(st.logits, m.blob, m.ctc_b, nc, n_frames)
    for (t in range64(n_frames)) {
        let row = t * nc
        var best = 0
        var mx = st.logits[row]
        for (i in range64(1l, nc)) {
            let v = st.logits[row + i]
            if (v > mx) {
                mx = v
                best = int(i)
            }
        }
        st.picks[t] = best
    }
}

//! -log of the sum of exp(logit - max) over the `nc` logits at `row`, the max at class `best`: the emitted token's plog
def private gg_ctc_plog(logits : array<float>; row, nc : int64; best : int) : float {
    let mx = logits[row + int64(best)]
    var lse = 0.0lf
    for (i in range64(nc)) {
        lse += double(exp(logits[row + i] - mx))
    }
    return -float(log(lse))
}

def private gg_decode_ctc(m : GigaamModel; var st : GigaamState; n_frames : int64) {
    let nc = m.n_classes
    ensure_length(st.picks, n_frames)
    ensure_length(st.plogs, n_frames)
    let dev = g_ga_gpu_ctc_set && invoke(g_ga_gpu_ctc, m, st, n_frames)
    if (!dev) {
        ensure_length(st.logits, n_frames * nc)
        gg_ctc_logits_cpu(m, st, n_frames)
    }
    var prev = -1
    for (t in range64(n_frames)) {
        let best = st.picks[t]
        if (best != m.blank_id && best != prev) {
            let plog = dev ? st.plogs[t] : gg_ctc_plog(st.logits, t * nc, nc, best)
            st.toks |> push(GigaamToken(id = best, frame = t, plog = plog))
        }
        prev = best
    }
}

def private gg_rnnt_net(m : GigaamModel) : RnntNet {
    var net = RnntNet(blob = m.blob, n_layers = m.pred_layers, pred_dim = m.pred_dim, joint_dim = m.joint_dim,
        n_tokens = m.n_classes, blank_id = m.blank_id, max_symbols = GG_MAX_SYMBOLS, zero_start = true, torch_gates = true, two_biases = true,
        embed = m.embed_w, joint_pred_w = m.jpred_w, joint_pred_b = m.jpred_b, joint_net_w = m.jout_w, joint_net_b = m.jout_b,
        prof_step = "ga.dec.step", prof_pick = "ga.dec.pick", prof_joint = "ga.dec.joint")
    net.lstm_ih[0] = m.lstm_ih
    net.lstm_hh[0] = m.lstm_hh
    net.lstm_b[0] = m.lstm_b_ih
    net.lstm_b2[0] = m.lstm_b_hh
    return net
}

def private gg_decode_rnnt(m : GigaamModel; var st : GigaamState; n_frames : int64) {
    let jd = m.joint_dim
    rnnt_decode(gg_rnnt_net(m), st.rnnt, RnntEnc(d = m.d_model, w = m.jenc_w, b = m.jenc_b, gen = st.enc_gen), st.x, st.encp, n_frames, st.toks) $(var encp) {
        ensure_length(encp, n_frames * jd)
        mm_blob_b(encp, m.blob, m.jenc_w, st.x, m.d_model, jd, n_frames)
        add_bias_rows(encp, m.blob, m.jenc_b, jd, n_frames)
    }
}

//! Greedy decode of the encoder rows in ``st.x`` into ``st.toks``: the CTC or the RNNT head, as the carrier ships.
[hot_path]
def gigaam_decode(m : GigaamModel; var st : GigaamState; n_frames : int64) {
    st.toks |> clear()
    if (m.head == GigaamHead.ctc) {
        gg_decode_ctc(m, st, n_frames)
    } else {
        gg_decode_rnnt(m, st, n_frames)
    }
}

//! The transcript of `toks`: the shared SentencePiece rule on an spm vocab, the pieces as they are on a charwise one.
def gigaam_detokenize(m : GigaamModel; toks : array<GigaamToken> | #) : string {
    return asr_spm_detokenize(m.vocab, toks) if (m.spm)
    return build_string() $(var w) {
        for (t in toks) {
            w |> write(m.vocab[t.id])
        }
    }
}

//! The capability sheet: Russian only, segment timestamps, whole-clip.
[unused_argument(m)]
def gigaam_asr_caps(m : GigaamModel) : AsrCaps {
    var c = AsrCaps(translate = false, timestamps = AsrTimestamps.segment, streaming = false, prompt = false, default_lang = "ru")
    c.languages <- ["ru"]
    return <- c
}

//! Session-create validation: GigaAM v3 transcribes Russian; "ru", "" and "auto" are accepted.
[unused_argument(m)]
def gigaam_check_lang(m : GigaamModel; lang : string) {
    if (lang != "ru" && lang != "" && lang != "auto") {
        panic("dasLLAMA asr: '{lang}' is not this GigaAM model's language - it transcribes Russian (ru)")
    }
}

//! The clip spans one encode each: the whole clip up to ``GIGAAM_MAX_CLIP_S``; past it, the silero VAD's speech regions
//! merged greedily into spans no longer than the cap, a region longer than the cap cut at the cap.
def gigaam_chunks(var st : GigaamState; samples : array<float> | #) : array<SpeechSegment> {
    let n = long_length(samples)
    let cap = GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE
    var out : array<SpeechSegment>
    if (n <= cap) {
        out |> push(SpeechSegment(start = 0l, end = n))
        return <- out
    }
    if (!st.vad_ready) {
        let vad_path = vad_model_path()
        if (!stat(vad_path).is_valid) {
            panic("{GG_WHO}: a clip past {GIGAAM_MAX_CLIP_S} s splits at silences, and the silero VAD weights are not at '{vad_path}'")
        }
        st.vad <- load_vad_model(vad_path)
        st.vad_ready = true
    }
    let segs <- find_speech_timestamps(st.vad, samples, default_vad_opts())
    if (empty(segs)) {
        to_log(LOG_WARNING, "{GG_WHO}: the VAD found no speech in the {n / AUDIO_SAMPLE_RATE} s clip - no segment transcribed\n")
    }
    return <- gigaam_spans(segs, cap)
}

//! Speech regions (ascending sample ranges) merged greedily into spans no longer than `cap` samples, a region longer
//! than the cap cut at the cap; no regions, no spans.
def gigaam_spans(segs : array<SpeechSegment>; cap : int64) : array<SpeechSegment> {
    var out : array<SpeechSegment>
    out |> reserve(length(segs))
    var cur = SpeechSegment(start = -1l, end = -1l)
    for (s in segs) {
        var s0 = s.start
        while (s.end - s0 > cap) {
            if (cur.start >= 0l) {
                out |> push(cur)
            }
            out |> push(SpeechSegment(start = s0, end = s0 + cap))
            cur = SpeechSegment(start = -1l, end = -1l)
            s0 += cap
        }
        if (cur.start >= 0l && s.end - cur.start <= cap) {
            cur.end = s.end
        } else {
            if (cur.start >= 0l) {
                out |> push(cur)
            }
            cur = SpeechSegment(start = s0, end = s.end)
        }
    }
    if (cur.start >= 0l) {
        out |> push(cur)
    }
    return <- out
}

//! The facade's transcribe arm: each span of [[gigaam_chunks]] encoded and decoded alone, one segment a span with
//! its bounds in centiseconds from the clip start. Answers the segment count.
def gigaam_transcribe(m : GigaamModel; var st : GigaamState; samples : array<float> | #;
                      blk : block<(seg : TranscribeSegment)>; var detected_lang : string&) : int {
    detected_lang = "ru"
    let spans <- gigaam_chunks(st, samples)
    var n_seg = 0
    for (sp in spans) {
        let tt = gigaam_encode_span(samples, sp.start, sp.end, m, st)
        continue if (st.n_len == 0l)
        let tp = ref_time_ticks()
        gigaam_decode(m, st, tt)
        asr_prof_add("ga.decode_total", tp)
        let seg <- asr_segment(sp.start * 100l / AUDIO_SAMPLE_RATE, sp.end * 100l / AUDIO_SAMPLE_RATE,
            gigaam_detokenize(m, st.toks), st.toks)
        blk |> invoke(seg)
        n_seg++
    }
    return n_seg
}
