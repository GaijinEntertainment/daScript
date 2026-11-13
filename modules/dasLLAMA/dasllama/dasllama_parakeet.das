options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_parakeet shared public
options _function_length = 0   // model forward/serialize walks mirror the model schema

require dasllama/dasllama_lint public
require math
require daslib/fio
require strings
require daslib/strings_boost
require daslib/apply
require daslib/archive
require dasllama/dasllama_env
require dasllama/dasllama_math
require dasllama/dasllama_gguf
require dasllama/dasllama_unicode   // bytes_str: a vocabulary piece, a tensor record's name
require dasllama/dasllama_plane public   // PlaneF/PlaneI8/PlaneI32/PlaneI64 — the borrowed weight planes
require dasllama/dasllama_asr_types public   // TranscribeSegment/AsrCaps — the shared ASR floor
require dasllama/dasllama_audio
require dasllama/dasllama_tower
require dasllama/dasllama_tower_image
require dasllama/dasllama_tower_flavor   // [tower_flavor]: the family's GPU flavor seat
require dasllama/dasllama_load   // kq_fmt_of: the format a plane's disk type serves in
require dasllama/dasllama_convert
require dasllama/dasllama_par
require dasllama/dasllama_common   // apply_box_profile_runtime for the parakeet-image identity
require dasllama/dasllama_image
require dasllama/dasllama_rnnt public   // public: ParakeetToken is RnntToken

// Parakeet-TDT (NVIDIA FastConformer + LSTM predictor + Token-and-Duration Transducer),
// read from the whisper.cpp-converted ggml bin. Oracle = whisper.cpp's in-tree parakeet-cli
// (CPU, -ng); see parakeet_plan.md for the container/graph/decode spec.

let PARAKEET_HOP = 160l
let private LOCAL_ATTN_THRESHOLD = 8192l   // beyond this the oracle switches to local attention

// interleaved (re, im) slots of one 512-pt FFT frame — the mel worker's stack buffer
let private FFT_FRAME = 1024

//! One decoded TDT token: the shared transducer's record.
typedef ParakeetToken = RnntToken

//! One conformer block's blob offsets: GEMM regions index qblob on a q8 load, blob on fp32
//! (identical walks on fp32); the d-vectors, u/v biases and the depthwise taps always index blob.
struct PkLayerOffs {
    nf1_w : int64
    nf1_b : int64
    ff1a_w : int64   // 1024 -> 4096
    ff1b_w : int64   // 4096 -> 1024
    nat_w : int64
    nat_b : int64
    bias_u : int64   // [d_head × n_head]
    bias_v : int64
    q_w : int64
    k_w : int64
    v_w : int64
    o_w : int64
    pos_w : int64
    ncv_w : int64
    ncv_b : int64
    pw1_w : int64    // 1024 -> 2048
    dw_w : int64     // [9 × 1024]
    bn_w : int64
    bn_b : int64
    bn_mean : int64
    bn_var : int64
    pw2_w : int64
    nf2_w : int64
    nf2_b : int64
    ff2a_w : int64
    ff2b_w : int64
    nout_w : int64
    nout_b : int64
}

//! A loaded Parakeet-TDT model: hparams, mel filterbank + STFT window, TDT durations, SPM vocab,
//! and weights. An fp32 load keeps one blob; the default q8 load transcodes GEMM regions into
//! qblob/qscales, leaving blob with only the fp32-side regions.
struct ParakeetModel {
    n_vocab : int64
    n_state : int64
    n_head : int64
    n_layer : int64
    n_mel : int64
    n_fft : int64
    subsampling : int64
    n_sub_ch : int64
    n_conv_kernel : int64
    n_pred_dim : int64
    n_pred_layers : int64
    n_durations : int64
    n_max_tokens : int64
    eps : float = 1e-5
    blank_id : int
    durations : PlaneI32 = PlaneI32()
    filters : PlaneF = PlaneF()     // [n_mel][n_fb]
    window : PlaneF = PlaneF()      // STFT window from the file
    fft : FftPlan = FftPlan()  // radix-2 plan (n_fft is 512 on every parakeet); built at load
    vocab : array<string>
    blob : PlaneF = PlaneF()
    // pre-encode conv stack
    c0_w : int64
    c0_b : int64
    c2_w : int64
    c2_b : int64
    c3_w : int64
    c3_b : int64
    c5_w : int64
    c5_b : int64
    c6_w : int64
    c6_b : int64
    pre_w : int64
    pre_b : int64
    layers_off : int64
    layer_stride : int64
    aux_layers_off : int64     // fp32 walk of the layer span (== layers_off/layer_stride on an
    aux_layer_stride : int64   // fp32 load; on a q8 load the GEMM regions collapse out of it)
    // prediction network + joint
    dec_slab : int64           //! the transducer decoder's f32 slab in ``blob``: the token table, the joint biases, the summed gate biases (`rnnt_dev_layout`)
    rnnt_wh : PlaneU16 = PlaneU16()   //! the predictor stack's and joint.pred's weights as the halves the decode step runs on (`rnnt_bake`)
    rnnt_jw : PlaneU16 = PlaneU16()   //! the joint net's weight as half columns in logit blocks
    joint_enc_w : int64
    joint_enc_b : int64
    // Q8 fast path (default load): Q8_0 image of the GEMM regions at their full-layout
    // offsets, repacked for the active backend; transcoded straight from the mapped file.
    q8 : bool
    jenc_exact : bool         //! the joint's encoder projection f32 in `blob` on the q8 lane: a GPU flavor's, which the device's exact tile projects
    qblob : PlaneI8 = PlaneI8()
    qscales : PlaneF = PlaneF()
    gemm_f16 : bool                  //! a GPU home's exact lane (`tower_dev_halfword`): the encoder GEMMs in ``wblob`` at their full-layout offsets
    wblob : PlaneU16 = PlaneU16()    //! the encoder GEMM regions as halfwords, the file's f32 saturated to f16 at staging; the joint stays f32
    kq_lane : bool                   //! a quantized GGUF's own planes (`#parakeet-kq-seats`): every GEMM offset is its seat's number in ``gemm_seats``
    kq_repacked : bool               //! the K-quant planes are in the load's backend layout, else disk order
    kq_fmt : int                     //! the lane's one K-quant member (a `KqFmt` slot), `q8` where every seat is Q8_0, TOWER_KQ_MIXED where they mix
    kq : PlaneU8 = PlaneU8()         //! the K-quant seats' quant bytes
    ks : PlaneU8 = PlaneU8()         //! ... and their scale bytes
    gemm_seats : PlaneI64 = PlaneI64()   //! `TOWER_SEAT_WORDS` a seat: the format slot, the quant offset, the scale offset
    dev_backend : TowerDev    //! the GPU flavor whose device block the image carries (`#tower-dev-block`); `none` on the CPU image
    dev : PlaneU8 = PlaneU8()
    devplan : PlaneI64 = PlaneI64()
    image_map : void?         // non-null when the planes borrow a .dlim image (dasllama_image)
    image_bytes : uint64
}

// planes borrow; vocab (meta-carried) and the fft plan (rebuilt at load) are owned
def finalize(var t : ParakeetModel) {
    apply(t) $ [unused_argument(name)] (name : string; var field) {
        static_if (typeinfo is_array(field)) {
            delete field
        } static_elif (typeinfo is_struct(field)) {
            delete field
        }
    }
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

// the tripwire: serialize_image_meta(ParakeetModel) covers 42 fields (vocab rides the meta
// blob — string arrays can't be raw planes); fft (rebuilt at load) + image_map + image_bytes
// are deliberate skips.
let private PARAKEET_META_FIELDS = 42 + 3

// every meta-carried ParakeetModel field, explicitly, both directions (see dasllama_image)
def private serialize_image_meta(var arch : Archive; var t : ParakeetModel) {
    verify(count_meta_fields(t) == PARAKEET_META_FIELDS)   // grew ParakeetModel? extend this list
    arch |> serialize_raw(t.n_vocab)
    arch |> serialize_raw(t.n_state)
    arch |> serialize_raw(t.n_head)
    arch |> serialize_raw(t.n_layer)
    arch |> serialize_raw(t.n_mel)
    arch |> serialize_raw(t.n_fft)
    arch |> serialize_raw(t.subsampling)
    arch |> serialize_raw(t.n_sub_ch)
    arch |> serialize_raw(t.n_conv_kernel)
    arch |> serialize_raw(t.n_pred_dim)
    arch |> serialize_raw(t.n_pred_layers)
    arch |> serialize_raw(t.n_durations)
    arch |> serialize_raw(t.n_max_tokens)
    arch |> serialize_raw(t.eps)
    arch |> serialize_raw(t.blank_id)
    serialize_strings(arch, t.vocab)
    arch |> serialize_raw(t.c0_w)
    arch |> serialize_raw(t.c0_b)
    arch |> serialize_raw(t.c2_w)
    arch |> serialize_raw(t.c2_b)
    arch |> serialize_raw(t.c3_w)
    arch |> serialize_raw(t.c3_b)
    arch |> serialize_raw(t.c5_w)
    arch |> serialize_raw(t.c5_b)
    arch |> serialize_raw(t.c6_w)
    arch |> serialize_raw(t.c6_b)
    arch |> serialize_raw(t.pre_w)
    arch |> serialize_raw(t.pre_b)
    arch |> serialize_raw(t.layers_off)
    arch |> serialize_raw(t.layer_stride)
    arch |> serialize_raw(t.aux_layers_off)
    arch |> serialize_raw(t.aux_layer_stride)
    arch |> serialize_raw(t.dec_slab)
    arch |> serialize_raw(t.joint_enc_w)
    arch |> serialize_raw(t.joint_enc_b)
    arch |> serialize_raw(t.q8)
    arch |> serialize_raw(t.jenc_exact)
    arch |> serialize_raw(t.gemm_f16)
    arch |> serialize_raw(t.dev_backend)
    arch |> serialize_raw(t.kq_lane)
    arch |> serialize_raw(t.kq_repacked)
    arch |> serialize_raw(t.kq_fmt)
}

def private image_post_load(var t : ParakeetModel) {
    t.fft <- build_fft_plan(t.n_fft)   // derived tables — the parse-at-load carve-out
}

[init]
def private register_parakeet_image_tags {
    register_image_family_tag("parakeet-q8")
    register_image_family_tag("parakeet-f32", config_free = true)
    register_image_family_tag("parakeet-f16", config_free = true)
    register_tower_dev_tags("parakeet-q8")
    register_tower_dev_tags("parakeet-f16")
    register_image_family_tag(PARAKEET_KQ_TAG)
    register_tower_dev_tags(PARAKEET_KQ_TAG)
}

let PARAKEET_KQ_TAG = "parakeet-kq"   //! the image tag of the kq lane: a quantized GGUF's own planes
let PK_LAYER_SEATS = 11l              //! a block's kq-lane seats, in `PkSeat` order
let PK_FRONT_GEMMS = 3l               //! the front's seats behind the blocks': its two channel mixes, its output linear
let PK_FRONT_SEATS = PK_FRONT_GEMMS + 1l   //! ... and the joint's encoder linear, on a CPU image

//! a block's kq-lane seats in seat order - the order of the GPU chains' GEMM records (`fc_gemm_records`), so a seat's
//! number in its block is its record's; `pk_layer_offsets` hands the chain `l * PK_LAYER_SEATS` + the member
enum PkSeat {
    ff1a
    ff1b
    q
    k
    v
    pos
    o
    pw1
    pw2
    ff2a
    ff2b
}

//! the image tag of a lane: q8, the exact pick's halfwords (`f16`), or the file's f32
def parakeet_lane_tag(q8, f16 : bool) : string => tower_lane_tag("parakeet", q8, f16)

//! the image tag of the lane a load of `path` on `q8` takes under flavor `dev` (`tower_load_f16`)
def parakeet_image_tag(path : string; q8 : bool; dev : TowerDev) : string => parakeet_lane_tag(q8, tower_load_f16(path, q8, dev))

[arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def pk_layer_offsets(m : ParakeetModel; l : int64) : PkLayerOffs {
    let d = m.n_state
    let ff = 4l * d
    let dh = d / m.n_head
    // dual walk: `o` is the full GEMM layout (qblob on a q8 load, wblob on an f16 one); `a` is the fp32 walk of m.blob (identical on an fp32 load)
    let g = (m.q8 || m.gemm_f16 || m.kq_lane) ? 0l : 1l
    var o = m.layers_off + l * m.layer_stride
    var a = m.aux_layers_off + l * m.aux_layer_stride
    var r = PkLayerOffs()
    r.nf1_w = a; a += d; o += d
    r.nf1_b = a; a += d; o += d
    r.ff1a_w = o; o += ff * d; a += g * ff * d
    r.ff1b_w = o; o += d * ff; a += g * d * ff
    r.nat_w = a; a += d; o += d
    r.nat_b = a; a += d; o += d
    r.bias_u = a; a += dh * m.n_head; o += dh * m.n_head
    r.bias_v = a; a += dh * m.n_head; o += dh * m.n_head
    r.q_w = o; o += d * d; a += g * d * d
    r.k_w = o; o += d * d; a += g * d * d
    r.v_w = o; o += d * d; a += g * d * d
    r.o_w = o; o += d * d; a += g * d * d
    r.pos_w = o; o += d * d; a += g * d * d
    r.ncv_w = a; a += d; o += d
    r.ncv_b = a; a += d; o += d
    r.pw1_w = o; o += 2l * d * d; a += g * 2l * d * d
    r.dw_w = a; a += m.n_conv_kernel * d; o += m.n_conv_kernel * d
    r.bn_w = a; a += d; o += d
    r.bn_b = a; a += d; o += d
    r.bn_mean = a; a += d; o += d
    r.bn_var = a; a += d; o += d
    r.pw2_w = o; o += d * d; a += g * d * d
    r.nf2_w = a; a += d; o += d
    r.nf2_b = a; a += d; o += d
    r.ff2a_w = o; o += ff * d; a += g * ff * d
    r.ff2b_w = o; o += d * ff; a += g * d * ff
    r.nout_w = a; a += d; o += d
    r.nout_b = a; a += d; o += d
    if (m.kq_lane) {   // the GEMMs' offsets are their seats' numbers (`#parakeet-kq-seats`)
        let s = l * PK_LAYER_SEATS
        r.ff1a_w = s + int64(PkSeat.ff1a)
        r.ff1b_w = s + int64(PkSeat.ff1b)
        r.q_w = s + int64(PkSeat.q)
        r.k_w = s + int64(PkSeat.k)
        r.v_w = s + int64(PkSeat.v)
        r.pos_w = s + int64(PkSeat.pos)
        r.o_w = s + int64(PkSeat.o)
        r.pw1_w = s + int64(PkSeat.pw1)
        r.pw2_w = s + int64(PkSeat.pw2)
        r.ff2a_w = s + int64(PkSeat.ff2a)
        r.ff2b_w = s + int64(PkSeat.ff2b)
    }
    return r
}

// per-layer blob span: 8 d×d mats (q/k/v/o/pos + pw1(2) + pw2) + 4 ff×d mats + 16
// d-vectors (norms ×8, u/v, bn ×4) + the depthwise kernel
def private pk_layer_stride(d, ff, nk : int64) : int64 => 8l * d * d + 4l * ff * d + 16l * d + nk * d

// fp32 side of a q8 load: the 11 GEMM mats live in qblob only, leaving the 16 d-vectors
// and the depthwise kernel
def private pk_aux_layer_stride(d, nk : int64) : int64 => 16l * d + nk * d

// repack one GEMM region for the active backend (scales index at woff/32, so woff must be 32-aligned)
def private pk_repack(var st : ParakeetStaging; woff, n, d : int64) {
    if (woff % 32l != 0l) {
        panic("parakeet q8: GEMM weight at {woff} is not 32-aligned")
    }
    repack_q8q8_weight(st.qblob, st.qscales, woff, n, d)
}

// repack every GEMM region for the active backend's q8q8 kernels; without a selected
// backend matmul_q8q8_batch reads the flat Q8_0 image as-is
def private pk_repack_all(var st : ParakeetStaging) {
    if (!tower_repack_backend()) {
        return
    }
    let d = st.e.n_state
    let ff = 4l * d
    let nch = st.e.n_sub_ch
    let w3 = conv_s2_out(conv_s2_out(conv_s2_out(st.e.n_mel)))
    let feat = w3 * nch                       // pre_w input dim (encode's xf row width)
    pk_repack(st, st.e.c3_w, nch, nch)
    pk_repack(st, st.e.c6_w, nch, nch)
    pk_repack(st, st.e.pre_w, feat, d)
    for (l in range64(st.e.n_layer)) {
        let lo = pk_layer_offsets(st.e, l)
        pk_repack(st, lo.ff1a_w, d, ff)
        pk_repack(st, lo.ff1b_w, ff, d)
        pk_repack(st, lo.q_w, d, d)
        pk_repack(st, lo.k_w, d, d)
        pk_repack(st, lo.v_w, d, d)
        pk_repack(st, lo.o_w, d, d)
        pk_repack(st, lo.pos_w, d, d)
        pk_repack(st, lo.pw1_w, d, 2l * d)
        pk_repack(st, lo.pw2_w, d, d)
        pk_repack(st, lo.ff2a_w, d, ff)
        pk_repack(st, lo.ff2b_w, ff, d)
    }
    pk_repack(st, st.e.joint_enc_w, d, st.e.n_pred_dim) if (!st.e.jenc_exact)
}

// ===== per-session scratch =====

//! the quantized image a session holds of its position table: none (the float lanes), Q8_0, or the kq lane's Q8_K
enum PkPosImage {
    none
    q8
    q8k
}

//! Every hot-path buffer of the parakeet pipeline (mel, conv subsampling, encoder, requant,
//! TDT decode) so a transcribe allocates NOTHING per call. Arrays grow reserve-exact and never
//! shrink; a default-constructed value is ready to use.
struct ParakeetState {
    enc_gen : uint64   //!< the device rows' generation the last encode left (`tower_rows_gen`), 0 for none
    // mel
    mel : WhisperMel = WhisperMel()
    pre : array<float>
    padded : array<float>
    // conv subsampling
    y0 : array<float>
    y2 : array<float>
    y3 : array<float>
    y5 : array<float>
    y6 : array<float>
    xf : array<float>
    // encoder residual (holds the final encoder rows after the last layer) + scratch
    x : array<float>
    xb : array<float>
    h : array<float>
    qq : array<float>
    kk : array<float>
    vv : array<float>
    pos_emb : array<float>
    pos_tt : int64              //!< the frame count `pos_emb` holds the table of (0 = none), at width `pos_d`, its quantized image `pos_image`
    pos_d : int64
    pos_image : PkPosImage
    posq : array<int8>
    poss : array<float>
    posbs : array<int>          //!< the kq lane's Q8_K image of the table: its per-16 sums
    posl : array<float>
    qu : array<float>
    qv : array<float>
    vh : array<float>
    kt : array<float>
    sc : array<float>
    rl : array<float>
    posh : array<float>
    oh : array<float>
    ao : array<float>
    // q8 activation image (q/k/v share one requant; decode reuses it)
    xqi : array<int8>
    xsi : array<float>
    xbsi : array<int>   //!< the Q8_K image's per-16 sums (the kq lane's; empty on the others)
    // decode
    encp : array<float>
    @scratch toks : array<ParakeetToken>
    dec : RnntState = RnntState()
}

// conv taps arrive [ch][3][3] (dw, torch) / [cout][9] (c0, im2col order) / [ch][k]
// (conformer depthwise); the f4 kernels want tap-major [ntap][ch]
def private pk_conv_tap_major(var blob : array<float>; off, ch, ntap : int64) {
    var tmp : array<float>
    tmp |> resize(ch * ntap)
    for (c in range64(ch)) {
        for (tap in range64(ntap)) {
            tmp[tap * ch + c] = blob[off + c * ntap + tap]
        }
    }
    for (i, v in count(), tmp) {
        blob[off + int64(i)] = v
    }
    delete tmp
}

// inference BN folded to per-channel scale/shift at load: y = acc·scale + shift with
// scale = γ/sqrt(σ²), shift = β − μ·scale. bn_w/bn_b slots take scale/shift; the
// mean/var slots stay as read (dead after the fold).
def private pk_fold_bn(var blob : array<float>; w, b, mean, vr, d : int64) {
    for (j in range64(d)) {
        let s = blob[w + j] / sqrt(blob[vr + j])
        blob[w + j] = s
        blob[b + j] -= blob[mean + j] * s
    }
}

// ===== bin container =====

// whisper-bin-style tensor records: i32 n_dims, i32 name_len, i32 ftype; i32 dims (ne
// order); name bytes; raw data (f32, f16 or Q8_0 blocks), unaligned.
struct private PkTensorRec {
    ttype : int
    n_elem : int64
    off : int64
}

let PARAKEET_ARCH = "parakeet"   //! `general.architecture` of a Parakeet GGUF carrier
let private PK_WHO = "dasLLAMA parakeet"

//! The tensors of a carrier, either container: a ggml bin's records by name, or a GGUF's own table (`gguf`), whose
//! tensors answer to the bin's names through `pk_gguf_name`
struct private PkSource {
    gguf : bool
    recs : table<string; PkTensorRec>
    meta : GGUFMeta = GGUFMeta()
}

[arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-gguf")]
def private pk_gguf_name(name : string) : string => replace_multiple(name, [
    (text = "encoder.layers.", replacement = "enc.blocks."), (text = "encoder.pre_encode.", replacement = "enc.pre_encode."),
    (text = ".norm_feed_forward1.", replacement = ".norm_ff1."), (text = ".feed_forward1.", replacement = ".ff1."),
    (text = ".norm_feed_forward2.", replacement = ".norm_ff2."), (text = ".feed_forward2.", replacement = ".ff2."),
    (text = ".norm_self_att.", replacement = ".norm_attn."), (text = ".self_attn.", replacement = ".attn."),
    (text = ".pointwise_conv1.", replacement = ".pointwise1."), (text = ".pointwise_conv2.", replacement = ".pointwise2."),
    (text = ".depthwise_conv.", replacement = ".depthwise."), (text = ".batch_norm.", replacement = ".bn."),
    (text = "decoder.prediction.embed.", replacement = "pred.embed."), (text = "joint.joint_net.2.", replacement = "joint.out.")])

def private pk_read(src : PkSource; b : array<uint8> | #; name : string;
                    var dst : array<float>; dst_off, n : int64) {
    if (src.gguf) {
        let gname = pk_gguf_name(name)
        gguf_expect_elems(src.meta, gname, n, PK_WHO)
        gguf_read_tensor_f32(src.meta, b, gname, dst, dst_off, n)
        return
    }
    if (!key_exists(src.recs, name)) {
        panic("{PK_WHO}: missing tensor '{name}' in model file")
    }
    let r = src.recs?[name] ?? default<PkTensorRec>
    if (r.n_elem != n) {
        panic("{PK_WHO}: tensor '{name}' has {r.n_elem} elements, want {n}")
    }
    ggml_bin_read_f32(b, r.ttype, r.off, n, dst, dst_off)
}

//! a GEMM weight read: q8 transcodes into qblob/qscales at its full-layout offset, f16 into wblob's halfwords, fp32 into the blob
def private pk_read_gemm(src : PkSource; b : array<uint8> | #; name : string;
                         var st : ParakeetStaging; var scratch : array<float>; woff, n : int64) {
    return if (st.e.kq_lane)   // the kq lane's GEMMs are its seats: `pk_kq_fill` moves every one of them
    if (!st.e.q8 && !st.e.gemm_f16) {
        pk_read(src, b, name, st.blob, woff, n)
        return
    }
    if (woff % 32l != 0l) {
        panic("parakeet q8: GEMM weight at {woff} is not 32-aligned")
    }
    if (long_length(scratch) < n) {
        scratch |> reserve(n)
        scratch |> resize(n)
    }
    pk_read(src, b, name, scratch, 0l, n)
    if (st.e.gemm_f16) {
        unsafe {
            cvt_f32_to_f16_sat(addr(st.wblob[woff]), addr<float const?>(scratch[0]), n)
        }
    } else {
        quantize_q8_0_into(scratch, n, st.qblob, st.qscales, woff, woff / kq_elems(KqFmt.q8))
    }
}

//! a ggml bin's header into the staging and its tensor records into `src`
def private pk_bin_header(var st : ParakeetStaging; var src : PkSource; bytes : array<uint8>#; n_bytes : int64; path : string) {
    var o = 0l
    if (rd_u32(bytes, o) != ASR_GGML_MAGIC) {
        panic("{PK_WHO}: bad magic in '{path}'")
    }
    o += 4l
    st.e.n_vocab = int64(rd_i32(bytes, o)); o += 4l
    o += 4l   // n_audio_ctx (unused: full-clip encode)
    st.e.n_state = int64(rd_i32(bytes, o)); o += 4l
    st.e.n_head = int64(rd_i32(bytes, o)); o += 4l
    st.e.n_layer = int64(rd_i32(bytes, o)); o += 4l
    st.e.n_mel = int64(rd_i32(bytes, o)); o += 4l
    o += 4l   // ftype (per-record types below)
    st.e.n_fft = int64(rd_i32(bytes, o)); o += 4l
    st.e.subsampling = int64(rd_i32(bytes, o)); o += 4l
    st.e.n_sub_ch = int64(rd_i32(bytes, o)); o += 4l
    st.e.n_conv_kernel = int64(rd_i32(bytes, o)); o += 4l
    st.e.n_pred_dim = int64(rd_i32(bytes, o)); o += 4l
    st.e.n_pred_layers = int64(rd_i32(bytes, o)); o += 4l
    st.e.n_durations = int64(rd_i32(bytes, o)); o += 4l
    st.e.n_max_tokens = int64(rd_i32(bytes, o)); o += 4l

    // mel filterbank
    let fb_mel = int64(rd_i32(bytes, o)); o += 4l
    let fb_n = int64(rd_i32(bytes, o)); o += 4l
    if (fb_mel != st.e.n_mel || fb_n != st.e.n_fft / 2l + 1l) {
        panic("{PK_WHO}: filterbank is {fb_mel}x{fb_n}, want {st.e.n_mel}x{st.e.n_fft / 2l + 1l}")
    }
    st.filters |> resize(fb_mel * fb_n)
    for (i in range64(fb_mel * fb_n)) {
        st.filters[i] = rd_f32(bytes, o + i * 4l)
    }
    o += fb_mel * fb_n * 4l

    // STFT window
    let n_window = int64(rd_i32(bytes, o)); o += 4l
    st.window |> resize(n_window)
    for (i in range64(n_window)) {
        st.window[i] = rd_f32(bytes, o + i * 4l)
    }
    o += n_window * 4l

    // TDT durations
    st.durations |> resize(st.e.n_durations)
    for (i in range64(st.e.n_durations)) {
        st.durations[i] = int(rd_u32(bytes, o + i * 4l))
    }
    o += st.e.n_durations * 4l

    // vocab (SPM pieces; blank = id n_vocab lives outside the table)
    let nv = int64(rd_i32(bytes, o)); o += 4l
    st.e.vocab |> resize(nv)
    for (i in range64(nv)) {
        let len = int64(rd_u32(bytes, o)); o += 4l
        st.e.vocab[i] = bytes_str(bytes, o, len)
        o += len
    }

    // tensor records
    while (o + 12l <= n_bytes) {
        let n_dims = int64(rd_i32(bytes, o)); o += 4l
        let name_len = int64(rd_i32(bytes, o)); o += 4l
        let ttype = rd_i32(bytes, o); o += 4l
        var n_elem = 1l
        for (_d in range64(n_dims)) {
            n_elem *= int64(rd_i32(bytes, o)); o += 4l
        }
        let name = bytes_str(bytes, o, name_len)
        o += name_len
        src.recs[name] = PkTensorRec(ttype = ttype, n_elem = n_elem, off = o)
        o += ggml_bin_tensor_bytes(ttype, n_elem)
    }
}

let private PK_GGUF = "stt.parakeet"

[arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-gguf")]
def private pk_gguf_header(var st : ParakeetStaging; var src : PkSource; bytes : array<uint8>#; path : string) {
    src.gguf = true
    src.meta <- parse_gguf_meta(bytes)
    let arch = gguf_str_or(src.meta, bytes, "general.architecture", "")
    if (arch != PARAKEET_ARCH) {
        panic("{PK_WHO}: '{path}' is a '{arch}' GGUF, not a {PARAKEET_ARCH} carrier")
    }
    let head = gguf_str_or(src.meta, bytes, "{PK_GGUF}.head_kind", "")
    if (head != "tdt") {
        panic("{PK_WHO}: '{path}' carries the '{head}' head - the loader serves the TDT head")
    }
    for (key in fixed_array("{PK_GGUF}.encoder.length_masking", "{PK_GGUF}.vad.hidden")) {
        if (gguf_has(src.meta, key)) {
            panic("{PK_WHO}: '{path}' carries '{key}' - a variant of the family the loader does not serve")
        }
    }
    for (key in fixed_array("{PK_GGUF}.encoder.use_bias", "{PK_GGUF}.encoder.xscaling")) {
        if (gguf_bool_or(src.meta, bytes, key, false)) {
            panic("{PK_WHO}: '{path}' sets '{key}' - the chain runs the family's bias-free, unscaled encoder")
        }
    }
    st.e.n_vocab = gguf_int(src.meta, bytes, "{PK_GGUF}.predictor.vocab") - 1l   // the carrier counts the blank
    st.e.n_state = gguf_int(src.meta, bytes, "{PK_GGUF}.encoder.d_model")
    st.e.n_head = gguf_int(src.meta, bytes, "{PK_GGUF}.encoder.n_heads")
    st.e.n_layer = gguf_int(src.meta, bytes, "{PK_GGUF}.encoder.n_layers")
    st.e.subsampling = gguf_int(src.meta, bytes, "{PK_GGUF}.encoder.subsampling_factor")
    st.e.n_sub_ch = gguf_int(src.meta, bytes, "{PK_GGUF}.encoder.subsampling_channels")
    st.e.n_conv_kernel = gguf_int(src.meta, bytes, "{PK_GGUF}.encoder.conv_kernel")
    st.e.n_pred_dim = gguf_int(src.meta, bytes, "{PK_GGUF}.predictor.hidden")
    st.e.n_pred_layers = gguf_int(src.meta, bytes, "{PK_GGUF}.predictor.n_layers")
    st.e.n_max_tokens = gguf_int(src.meta, bytes, "{PK_GGUF}.tdt.max_symbols")
    st.e.n_mel = gguf_int(src.meta, bytes, "stt.frontend.num_mels")
    st.e.n_fft = gguf_int(src.meta, bytes, "stt.frontend.n_fft")
    let d_ff = gguf_int(src.meta, bytes, "{PK_GGUF}.encoder.d_ff")
    if (d_ff != 4l * st.e.n_state) {
        panic("{PK_WHO}: '{path}' has d_ff {d_ff} over d_model {st.e.n_state} - the chain runs an FF four times the model width")
    }
    var inscope durations <- gguf_int_array(src.meta, bytes, "{PK_GGUF}.tdt.durations")
    st.e.n_durations = long_length(durations)
    st.durations <- [for (d in durations); int(d)]
    let rate = gguf_int(src.meta, bytes, "stt.frontend.sample_rate")
    let hop = gguf_int(src.meta, bytes, "stt.frontend.hop_length")
    if (rate != AUDIO_SAMPLE_RATE || hop != PARAKEET_HOP) {
        panic("{PK_WHO}: '{path}' asks a {rate} Hz front end at a hop of {hop} - the mel path runs {AUDIO_SAMPLE_RATE} Hz at a hop of {PARAKEET_HOP}")
    }
    st.filters <- build_mel_filterbank(st.e.n_mel, st.e.n_fft, rate)
    st.window <- build_hann_window(gguf_int(src.meta, bytes, "stt.frontend.win_length"), false)
    st.e.vocab <- gguf_str_array(src.meta, bytes, "tokenizer.ggml.tokens")
    if (long_length(st.e.vocab) != st.e.n_vocab + 1l) {
        panic("{PK_WHO}: '{path}' lists {long_length(st.e.vocab)} tokens, the predictor {st.e.n_vocab + 1l} - the blank is the last of them")
    }
    st.e.vocab |> resize(st.e.n_vocab)
}

//! one predictor layer's weights and gate bias into `dst` at `ih`, `hh` and `b`, each container's rows as stored
[arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-gguf")]
def private pk_read_lstm(src : PkSource; b : array<uint8>#; var dst : array<float>; pd, l, ih, hh, bias : int64) {
    if (!src.gguf) {
        pk_read(src, b, "decoder.prediction.dec_rnn.lstm.weight_ih_l{l}", dst, ih, 4l * pd * pd)
        pk_read(src, b, "decoder.prediction.dec_rnn.lstm.weight_hh_l{l}", dst, hh, 4l * pd * pd)
        pk_read(src, b, "decoder.prediction.dec_rnn.lstm.bias_h_l{l}", dst, bias, 4l * pd)
        return
    }
    for (name, off, n in fixed_array("Wx", "Wh", "bias"), fixed_array(ih, hh, bias), fixed_array(4l * pd * pd, 4l * pd * pd, 4l * pd)) {
        let gname = "pred.lstm.{l}.{name}"
        gguf_expect_elems(src.meta, gname, n, PK_WHO)
        gguf_read_tensor_f32(src.meta, b, gname, dst, off, n)
    }
}

//! The transducer decoder into the staging in the image's form (`rnnt_bake`): the carrier's f32 tensors read into scratch under a
//! net of their own offsets - a GGUF's predictor in torch gate order - then the halves into ``rnnt_wh`` / ``rnnt_jw`` and the slab
//! into ``blob`` at ``dec_slab``
[arch(at = "../ARCHITECTURE_MEDIA_ASR.md#rnnt-decode-step")]
def private pk_bake_decoder(var st : ParakeetStaging; src : PkSource; b : array<uint8>#) {
    let pd = st.e.n_pred_dim
    let n_tokens = st.e.n_vocab + 1l
    let n_logits = n_tokens + st.e.n_durations
    var net = RnntNet(n_layers = st.e.n_pred_layers, pred_dim = pd, joint_dim = pd, n_tokens = n_tokens, torch_gates = src.gguf)
    var at = 0l
    net.embed = at; at += n_tokens * pd
    for (l in range64(net.n_layers)) {
        net.lstm_ih[l] = at; at += 4l * pd * pd
        net.lstm_hh[l] = at; at += 4l * pd * pd
        net.lstm_b[l] = at; at += 4l * pd
    }
    net.joint_pred_w = at; at += pd * pd
    net.joint_pred_b = at; at += pd
    net.joint_net_w = at; at += n_logits * pd
    net.joint_net_b = at; at += n_logits
    let enc_b = at; at += pd
    var inscope raw : array<float>
    raw |> reserve_resize(at)
    pk_read(src, b, "decoder.prediction.embed.weight", raw, net.embed, n_tokens * pd)
    for (l in range64(net.n_layers)) {
        pk_read_lstm(src, b, raw, pd, l, net.lstm_ih[l], net.lstm_hh[l], net.lstm_b[l])
    }
    pk_read(src, b, "joint.pred.weight", raw, net.joint_pred_w, pd * pd)
    pk_read(src, b, "joint.pred.bias", raw, net.joint_pred_b, pd)
    pk_read(src, b, "joint.joint_net.2.weight", raw, net.joint_net_w, n_logits * pd)
    pk_read(src, b, "joint.joint_net.2.bias", raw, net.joint_net_b, n_logits)
    pk_read(src, b, "joint.enc.bias", raw, enc_b, pd)
    unsafe {
        plane_bind(net.blob, addr<uint8 const?>(raw[0]), 0l, at * 4l)
        plane_bind(net.durations, addr<uint8 const?>(st.durations[0]), 0l, st.e.n_durations * 4l)
    }
    var inscope baked <- rnnt_bake(net, enc_b)
    plane_release(net.blob)
    plane_release(net.durations)
    for (v, i in baked.wf, count()) {
        st.blob[st.e.dec_slab + int64(i)] = v
    }
    st.rnnt_wh <- baked.wh
    st.rnnt_jw <- baked.jw
}

//! one kq-lane seat's source: the GGUF tensor, its row width (the GEMM's input) and its rows (the output)
struct private PkSeatJob {
    name : string
    width : int64
    rows : int64
}

[arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def private pk_kq_jobs(e : ParakeetModel) : array<PkSeatJob> {
    let d = e.n_state
    let ff = 4l * d
    let nch = e.n_sub_ch
    var jobs : array<PkSeatJob>
    jobs |> reserve(e.n_layer * PK_LAYER_SEATS + PK_FRONT_SEATS)
    for (l in range64(e.n_layer)) {
        let b = "enc.blocks.{l}"
        jobs |> push(PkSeatJob(name = "{b}.ff1.linear1.weight", width = d, rows = ff))
        jobs |> push(PkSeatJob(name = "{b}.ff1.linear2.weight", width = ff, rows = d))
        jobs |> push(PkSeatJob(name = "{b}.attn.linear_q.weight", width = d, rows = d))
        jobs |> push(PkSeatJob(name = "{b}.attn.linear_k.weight", width = d, rows = d))
        jobs |> push(PkSeatJob(name = "{b}.attn.linear_v.weight", width = d, rows = d))
        jobs |> push(PkSeatJob(name = "{b}.attn.linear_pos.weight", width = d, rows = d))
        jobs |> push(PkSeatJob(name = "{b}.attn.linear_out.weight", width = d, rows = d))
        jobs |> push(PkSeatJob(name = "{b}.conv.pointwise1.weight", width = d, rows = 2l * d))
        jobs |> push(PkSeatJob(name = "{b}.conv.pointwise2.weight", width = d, rows = d))
        jobs |> push(PkSeatJob(name = "{b}.ff2.linear1.weight", width = d, rows = ff))
        jobs |> push(PkSeatJob(name = "{b}.ff2.linear2.weight", width = ff, rows = d))
    }
    jobs |> push(PkSeatJob(name = "enc.pre_encode.conv.3.weight", width = nch, rows = nch))
    jobs |> push(PkSeatJob(name = "enc.pre_encode.conv.6.weight", width = nch, rows = nch))
    jobs |> push(PkSeatJob(name = "enc.pre_encode.out.weight", width = (e.n_mel / e.subsampling) * nch, rows = d))
    if (!e.jenc_exact) {
        jobs |> push(PkSeatJob(name = "joint.enc.weight", width = d, rows = e.n_pred_dim))
    }
    return <- jobs
}

[arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def private pk_kq_check_classes(e : ParakeetModel; seats : array<int64>) {
    let q8 = int64(int(KqFmt.q8))
    let pos0 = seats[int64(PkSeat.pos) * TOWER_SEAT_WORDS] == q8
    for (l in range64(e.n_layer)) {
        let s = l * PK_LAYER_SEATS
        let q = seats[(s + int64(PkSeat.q)) * TOWER_SEAT_WORDS] == q8
        if (q != (seats[(s + int64(PkSeat.k)) * TOWER_SEAT_WORDS] == q8) || q != (seats[(s + int64(PkSeat.v)) * TOWER_SEAT_WORDS] == q8)) {
            panic("{PK_WHO}: block {l}'s attention q, k and v mix Q8_0 with a K-quant format - they read one activation image; pin a lane (set_parakeet_q8) to dequantize them")
        }
        if (pos0 != (seats[(s + int64(PkSeat.pos)) * TOWER_SEAT_WORDS] == q8)) {
            panic("{PK_WHO}: the blocks' position linears mix Q8_0 with a K-quant format (block {l} against block 0) - they read one image of the position table; pin a lane (set_parakeet_q8) to dequantize them")
        }
    }
}

[arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def private pk_kq_fill(var st : ParakeetStaging; src : PkSource; b : array<uint8>#) {
    var inscope jobs <- pk_kq_jobs(st.e)
    var cur = TowerKqCursor()
    st.gemm_seats |> reserve(long_length(jobs) * TOWER_SEAT_WORDS)
    for (j in jobs) {
        gguf_expect_elems(src.meta, j.name, j.width * j.rows, PK_WHO)
        let f = tower_kq_seat_fmt(src.meta, j.name, j.width, true, PK_WHO, "set_parakeet_q8")
        let seat = tower_kq_seat_take(cur, f, j.width * j.rows)
        st.gemm_seats |> push_from([int64(int(f)), seat.qoff, seat.soff])
    }
    pk_kq_check_classes(st.e, st.gemm_seats)
    st.e.kq_fmt = tower_kq_lane_fmt(st.gemm_seats)
    st.qblob |> reserve(cur.q8)
    st.qblob |> resize(cur.q8)
    st.qscales |> reserve(cur.q8 / KQ_B32_ELEMS)
    st.qscales |> resize(cur.q8 / KQ_B32_ELEMS)
    st.kq |> reserve_resize(cur.kq)
    st.ks |> reserve_resize(cur.ks)
    let repack_q8 = tower_repack_backend()
    st.e.kq_repacked = repack_q8 && kernel_backend_has_kq()
    var inscope scratch : array<float>
    for (j, i in jobs, count()) {
        tower_kq_seat_fill(src.meta, b, j.name, tower_gemm_seat(st, int64(i)), j.width, j.width * j.rows, repack_q8, st.e.kq_repacked,
            st.qblob, st.qscales, st.kq, st.ks, scratch, PK_WHO)
    }
    let front = st.e.n_layer * PK_LAYER_SEATS
    st.e.c3_w = front
    st.e.c6_w = front + 1l
    st.e.pre_w = front + 2l
    if (!st.e.jenc_exact) {
        st.e.joint_enc_w = front + 3l
    }
}

//! Loads a Parakeet-TDT carrier: a converted ggml bin (f32, f16 or Q8_0) or a GGUF of architecture `parakeet` (`ARCHITECTURE_MEDIA_ASR.md#parakeet-gguf`).
//! With ``q8`` the encoder/joint GEMM weights serve quantized (~4x CPU fast path): transcoded to Q8_0 off the mapped file, or - un-pinned, a quantized GGUF - as the file's own planes; conv im2col, depthwise, LSTM, and joint_net stay fp32.
//! Like ``load_model``, caches a box/knob-specific PREPARED IMAGE for fast reload; ``DASLLAMA_IMAGE=0`` disables it, a ``.dlim`` path loads one directly.
[arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def load_parakeet_model(path : string; q8 : bool = false) : ParakeetModel {
    let is_dlim = path |> ends_with(".dlim")
    let kq = is_dlim ? image_tag_lane(image_family_tag(path)) == PARAKEET_KQ_TAG : parakeet_load_tag(path, q8) == PARAKEET_KQ_TAG
    if (kq) {
        let kq_dev = is_dlim ? TowerDev.none : parakeet_dev_flavor(ParakeetModel(kq_lane = true))
        var m <- load_staged_image_dev(path, PARAKEET_KQ_TAG, "parakeet", "source model", type<ParakeetModel>, kq_dev, g_parakeet_flavor.bake, PARAKEET_DEV_CONSUMED) {
            return <- stage_parakeet_kq_model(path)
        }
        var layout = m.kq_repacked ? "repacked for {active_kernel_backend()}" : "in disk order (the portable batch GEMM)"
        if (m.dev_backend != TowerDev.none) {
            layout = "gathered for the {m.dev_backend} tiles"
        }
        let unheard = audio_lane_unheard("a quantized carrier's own planes (set_parakeet_q8 dequantizes them)")
        to_log(LOG_INFO, "{PK_WHO}: GEMM lane kq ({parakeet_exec_fmt(m)}) - the GEMMs as the carrier's own planes, {layout}{unheard}\n")
        return <- m
    }
    let dev = is_dlim ? TowerDev.none : parakeet_dev_flavor(q8)
    let f16 = tower_load_f16(path, q8, dev)
    let tag = parakeet_lane_tag(q8, f16)
    announce_lane("parakeet", q8, f16, "set_parakeet_q8", g_pk_pin != GemmLane.unset, false)
    return <- load_staged_image_dev(path, tag, "parakeet", "source model", type<ParakeetModel>, dev, g_parakeet_flavor.bake, PARAKEET_DEV_CONSUMED) {
        return <- stage_parakeet_model(path, q8, f16)
    }
}

//! The image tag a CPU load of `path` on `q8` takes: the kq lane's for a quantized GGUF asked for q8 with no lane pinned, else the q8 or f32 lane's
def parakeet_load_tag(path : string; q8 : bool) : string
    => q8 && g_pk_pin == GemmLane.unset && pk_is_kq_carrier(path) ? PARAKEET_KQ_TAG : parakeet_lane_tag(q8, false)

[arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def pk_is_kq_carrier(path : string) : bool {
    return false if (!(path |> ends_with(".gguf")))
    var quantized = false
    gguf_header_read(path) $(m; bytes) {
        let ti = gguf_find_tensor(m, "enc.blocks.0.attn.linear_q.weight")
        if (gguf_str_or(m, bytes, "general.architecture", "") == PARAKEET_ARCH && ti >= 0) {
            let gt = m.tensors[ti].ggml_type
            quantized = !tower_float_type(gt)
        }
    }
    return quantized
}

//! A GPU driver's Parakeet flavor (`#tower-dev-block`): the backend whose image it mints, whether its chain serves a lane, and
//! the bake that converts a CPU-form model into the driver's device block
[tower_flavor(family = "parakeet", model = "ParakeetModel", lane = "model")]
struct ParakeetFlavor {}

let private PARAKEET_DEV_CONSUMED <- ["qblob", "qscales", "kq", "ks", "gemm_seats"]   //! the planes a Parakeet bake converts: a GPU flavor carries their device forms instead

//! The device flavor a load of a carrier with lane meta `e` takes: the registered driver's where its chain and the device
//! decode serve the lane, else `none` - the CPU image
def parakeet_dev_flavor(e : ParakeetModel) : TowerDev =>
    rnnt_gpu_projects(RnntEnc()) ? tower_dev_flavor(g_parakeet_flavor, e) : TowerDev.none

//! ... of a load on the `q8` lane, or the exact one
def parakeet_dev_flavor(q8 : bool) : TowerDev => parakeet_dev_flavor(ParakeetModel(q8 = q8))

//! A GPU-flavored model reached a CPU path at `stage`: the device seat declined what the load handed it, and the CPU never runs
//! that image (`#tower-dev-block`)
def private pk_cpu_refuse(m : ParakeetModel; stage : string) => tower_cpu_refuse(m, "dasLLAMA parakeet", stage)

//! The mint-side carrier: ``e`` holds every scalar the meta blob carries (vocab included), the
//! arrays beside it hold the plane bytes until the writer has them. Public because reading the
//! source bin off-rail is the only way a suite can check a served model against a non-image form.
struct ParakeetStaging {
    e : ParakeetModel = ParakeetModel()
    durations : array<int>
    filters : array<float>
    window : array<float>
    blob : array<float>
    @exact_size rnnt_wh : array<uint16>
    @exact_size rnnt_jw : array<uint16>
    qblob : array<int8>
    qscales : array<float>
    @exact_size wblob : array<uint16>
    @exact_size kq : array<uint8>
    @exact_size ks : array<uint8>
    @exact_size gemm_seats : array<int64>
    @exact_size dev : array<uint8>       //!< a GPU-flavored mint's device block (`TowerDevBuild`)
    @exact_size devplan : array<int64>   //!< ... and its plan
}

def private serialize_image_meta(var arch : Archive; var st : ParakeetStaging) {
    serialize_image_meta(arch, st.e)
}

def finalize(var st : ParakeetStaging) {
    delete_staging(st)
}

[unused_argument(st)]
def private image_post_load(var st : ParakeetStaging) {}

//! Mint a served model from a staged read WITHOUT touching disk — the in-memory image rail.
//! The off-rail control for suites: a live model that came from no `.dlim` file; `dev` names the GPU flavor (the staging read
//! under `with_tower_dev_staging(dev)`), `none` the CPU image (`tower_mint_flavor`).
def mint_parakeet_model(var st : ParakeetStaging; var out : ParakeetModel; q8 : bool = false; dev : TowerDev = TowerDev.none) {
    let tag = st.e.kq_lane ? PARAKEET_KQ_TAG : parakeet_lane_tag(q8, st.e.gemm_f16)
    tower_mint_flavor(st, out, tag, "parakeet", dev, g_parakeet_flavor, PARAKEET_DEV_CONSUMED)
}

//! The carrier reader proper (a ggml bin or a GGUF) under the running staging's flavor: its halfword lane is the staging's (`tower_load_f16`).
def stage_parakeet_model(path : string; q8 : bool = false) : ParakeetStaging
    => <- stage_parakeet_model(path, q8, tower_load_f16(path, q8, tower_staging_dev()))

//! The carrier reader proper: scalars land in the served model, plane bytes in the staging
//! arrays beside it. Nothing here is servable — the image build is what produces a live model.
def stage_parakeet_model(path : string; q8, f16 : bool) : ParakeetStaging => <- stage_parakeet_model(path, q8, f16, false)

//! the reader on the kq lane: a quantized GGUF's own planes (`#parakeet-kq-seats`)
def stage_parakeet_kq_model(path : string) : ParakeetStaging => <- stage_parakeet_model(path, false, false, true)

//! ... with `kq` the kq lane (a quantized GGUF's own planes; `q8` and `f16` then both off)
def stage_parakeet_model(path : string; q8, f16, kq : bool) : ParakeetStaging {
    var st = ParakeetStaging()
    st.e.q8 = q8
    st.e.gemm_f16 = f16
    st.e.kq_lane = kq
    let gemms_own_plane = q8 || f16 || kq
    let n_bytes = int64(stat(path).size)
    let f = fopen(path, "rb")
    if (f == null) {
        panic("{PK_WHO}: cannot open model '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope src = PkSource()
        if (gguf_magic_ok(bytes)) {
            pk_gguf_header(st, src, bytes, path)
        } else {
            pk_bin_header(st, src, bytes, n_bytes, path)
        }
        st.e.blank_id = int(st.e.n_vocab)
        if (st.e.n_fft != 512l) {   // FFT_FRAME in the mel worker is sized for 512-pt frames
            panic("{PK_WHO}: n_fft {st.e.n_fft} - the mel path is built for the family's 512-pt STFT")
        }
        st.e.fft <- build_fft_plan(st.e.n_fft)

        // blob layout — dual walk: `bo` is the full layout (qblob on a q8 load, blob on an
        // fp32 load); `ao` is the fp32 walk of blob, which the GEMM regions collapse out of
        // wherever the GEMMs own a plane (g = 0). On an fp32 load the walks are identical (ao ends == bo).
        let d = st.e.n_state
        let ff = 4l * d
        let dh = d / st.e.n_head
        let nch = st.e.n_sub_ch
        let g = gemms_own_plane ? 0l : 1l
        let gj = (q8 || kq) && tower_staging_dev() == TowerDev.none ? 0l : 1l   // a GPU flavor's joint encoder half stays the source's f32
        st.e.jenc_exact = (q8 || kq) && gj == 1l
        var bo = 0l
        var ao = 0l
        st.e.c0_w = ao; ao += nch * 9l; bo += nch * 9l
        st.e.c0_b = ao; ao += nch; bo += nch
        st.e.c2_w = ao; ao += nch * 9l; bo += nch * 9l          // depthwise: per-channel 3x3
        st.e.c2_b = ao; ao += nch; bo += nch
        st.e.c3_w = bo; bo += nch * nch; ao += g * nch * nch
        st.e.c3_b = ao; ao += nch; bo += nch
        st.e.c5_w = ao; ao += nch * 9l; bo += nch * 9l
        st.e.c5_b = ao; ao += nch; bo += nch
        st.e.c6_w = bo; bo += nch * nch; ao += g * nch * nch
        st.e.c6_b = ao; ao += nch; bo += nch
        let feat = (st.e.n_mel / st.e.subsampling) * nch
        st.e.pre_w = bo; bo += d * feat; ao += g * d * feat
        st.e.pre_b = ao; ao += d; bo += d
        st.e.layers_off = bo
        st.e.layer_stride = pk_layer_stride(d, ff, st.e.n_conv_kernel)
        st.e.aux_layers_off = ao
        st.e.aux_layer_stride = gemms_own_plane ? pk_aux_layer_stride(d, st.e.n_conv_kernel) : st.e.layer_stride
        bo += st.e.n_layer * st.e.layer_stride
        ao += st.e.n_layer * st.e.aux_layer_stride
        let pd = st.e.n_pred_dim
        let n_logits = st.e.n_vocab + 1l + st.e.n_durations
        let dec = rnnt_dev_layout(st.e.n_pred_layers, pd, pd, st.e.n_vocab + 1l, n_logits)
        st.e.dec_slab = ao; ao += dec.wf_floats
        st.e.joint_enc_w = gj == 1l ? ao : bo; bo += pd * d; ao += gj * pd * d
        st.e.joint_enc_b = st.e.dec_slab + dec.jeb
        // exact allocations: reserve defeats the array allocator's pow2 rounding, which on the
        // 0.6b f32 bin would back 2.4 GiB of blob floats with a 4 GiB allocation
        st.blob |> reserve(ao)
        st.blob |> resize(ao)
        if (kq) {
            if (!src.gguf) {
                panic("{PK_WHO}: the kq lane serves a GGUF carrier's own planes - '{path}' is a ggml bin")
            }
            pk_kq_fill(st, src, bytes)
        } elif (q8) {
            st.qblob |> reserve(bo)
            st.qblob |> resize(bo)
            st.qscales |> reserve(bo / 32l)
            st.qscales |> resize(bo / 32l)
        } elif (st.e.gemm_f16) {
            st.wblob |> reserve_resize(bo)
        }

        var wscratch : array<float>   // q8 transcode staging, one GEMM tensor at a time
        pk_read(src, bytes, "encoder.pre_encode.conv.0.weight", st.blob, st.e.c0_w, nch * 9l)
        pk_conv_tap_major(st.blob, st.e.c0_w, nch, 9l)   // c0 kernel reads tap-major too
        pk_read(src, bytes, "encoder.pre_encode.conv.0.bias", st.blob, st.e.c0_b, nch)
        pk_read(src, bytes, "encoder.pre_encode.conv.2.weight", st.blob, st.e.c2_w, nch * 9l)
        pk_conv_tap_major(st.blob, st.e.c2_w, nch, 9l)
        pk_read(src, bytes, "encoder.pre_encode.conv.2.bias", st.blob, st.e.c2_b, nch)
        pk_read_gemm(src, bytes, "encoder.pre_encode.conv.3.weight", st, wscratch, st.e.c3_w, nch * nch)
        pk_read(src, bytes, "encoder.pre_encode.conv.3.bias", st.blob, st.e.c3_b, nch)
        pk_read(src, bytes, "encoder.pre_encode.conv.5.weight", st.blob, st.e.c5_w, nch * 9l)
        pk_conv_tap_major(st.blob, st.e.c5_w, nch, 9l)
        pk_read(src, bytes, "encoder.pre_encode.conv.5.bias", st.blob, st.e.c5_b, nch)
        pk_read_gemm(src, bytes, "encoder.pre_encode.conv.6.weight", st, wscratch, st.e.c6_w, nch * nch)
        pk_read(src, bytes, "encoder.pre_encode.conv.6.bias", st.blob, st.e.c6_b, nch)
        pk_read_gemm(src, bytes, "encoder.pre_encode.out.weight", st, wscratch, st.e.pre_w, d * feat)
        pk_read(src, bytes, "encoder.pre_encode.out.bias", st.blob, st.e.pre_b, d)
        for (l in range64(st.e.n_layer)) {
            let lo = pk_layer_offsets(st.e, l)
            pk_read(src, bytes, "encoder.layers.{l}.norm_feed_forward1.weight", st.blob, lo.nf1_w, d)
            pk_read(src, bytes, "encoder.layers.{l}.norm_feed_forward1.bias", st.blob, lo.nf1_b, d)
            pk_read_gemm(src, bytes, "encoder.layers.{l}.feed_forward1.linear1.weight", st, wscratch, lo.ff1a_w, ff * d)
            pk_read_gemm(src, bytes, "encoder.layers.{l}.feed_forward1.linear2.weight", st, wscratch, lo.ff1b_w, d * ff)
            pk_read(src, bytes, "encoder.layers.{l}.norm_self_att.weight", st.blob, lo.nat_w, d)
            pk_read(src, bytes, "encoder.layers.{l}.norm_self_att.bias", st.blob, lo.nat_b, d)
            pk_read(src, bytes, "encoder.layers.{l}.self_attn.pos_bias_u", st.blob, lo.bias_u, dh * st.e.n_head)
            pk_read(src, bytes, "encoder.layers.{l}.self_attn.pos_bias_v", st.blob, lo.bias_v, dh * st.e.n_head)
            pk_read_gemm(src, bytes, "encoder.layers.{l}.self_attn.linear_q.weight", st, wscratch, lo.q_w, d * d)
            pk_read_gemm(src, bytes, "encoder.layers.{l}.self_attn.linear_k.weight", st, wscratch, lo.k_w, d * d)
            pk_read_gemm(src, bytes, "encoder.layers.{l}.self_attn.linear_v.weight", st, wscratch, lo.v_w, d * d)
            pk_read_gemm(src, bytes, "encoder.layers.{l}.self_attn.linear_out.weight", st, wscratch, lo.o_w, d * d)
            pk_read_gemm(src, bytes, "encoder.layers.{l}.self_attn.linear_pos.weight", st, wscratch, lo.pos_w, d * d)
            pk_read(src, bytes, "encoder.layers.{l}.norm_conv.weight", st.blob, lo.ncv_w, d)
            pk_read(src, bytes, "encoder.layers.{l}.norm_conv.bias", st.blob, lo.ncv_b, d)
            pk_read_gemm(src, bytes, "encoder.layers.{l}.conv.pointwise_conv1.weight", st, wscratch, lo.pw1_w, 2l * d * d)
            pk_read(src, bytes, "encoder.layers.{l}.conv.depthwise_conv.weight", st.blob, lo.dw_w, st.e.n_conv_kernel * d)
            pk_conv_tap_major(st.blob, lo.dw_w, d, st.e.n_conv_kernel)   // [d][k] -> [k][d] for the f4 row core
            pk_read(src, bytes, "encoder.layers.{l}.conv.batch_norm.weight", st.blob, lo.bn_w, d)
            pk_read(src, bytes, "encoder.layers.{l}.conv.batch_norm.bias", st.blob, lo.bn_b, d)
            pk_read(src, bytes, "encoder.layers.{l}.conv.batch_norm.running_mean", st.blob, lo.bn_mean, d)
            pk_read(src, bytes, "encoder.layers.{l}.conv.batch_norm.running_var", st.blob, lo.bn_var, d)
            pk_fold_bn(st.blob, lo.bn_w, lo.bn_b, lo.bn_mean, lo.bn_var, d)
            pk_read_gemm(src, bytes, "encoder.layers.{l}.conv.pointwise_conv2.weight", st, wscratch, lo.pw2_w, d * d)
            pk_read(src, bytes, "encoder.layers.{l}.norm_feed_forward2.weight", st.blob, lo.nf2_w, d)
            pk_read(src, bytes, "encoder.layers.{l}.norm_feed_forward2.bias", st.blob, lo.nf2_b, d)
            pk_read_gemm(src, bytes, "encoder.layers.{l}.feed_forward2.linear1.weight", st, wscratch, lo.ff2a_w, ff * d)
            pk_read_gemm(src, bytes, "encoder.layers.{l}.feed_forward2.linear2.weight", st, wscratch, lo.ff2b_w, d * ff)
            pk_read(src, bytes, "encoder.layers.{l}.norm_out.weight", st.blob, lo.nout_w, d)
            pk_read(src, bytes, "encoder.layers.{l}.norm_out.bias", st.blob, lo.nout_b, d)
        }
        if (gj == 1l) {   // the joint's encoder half in the f32 blob: the exact lanes and a GPU flavor's q8 or kq one
            pk_read(src, bytes, "joint.enc.weight", st.blob, st.e.joint_enc_w, st.e.n_pred_dim * d)
        } else {
            pk_read_gemm(src, bytes, "joint.enc.weight", st, wscratch, st.e.joint_enc_w, st.e.n_pred_dim * d)
        }
        pk_bake_decoder(st, src, bytes)
        delete wscratch
    }
    fclose(f)
    if (q8) {
        pk_repack_all(st)
    }
    return <- st
}

// ===== mel (frame-major!) =====

//! The loaded lane's name - on the kq lane its one K-quant member, `kq` where they mix: the facade's
//! [[asr_exec_fmt]] arm.
def parakeet_exec_fmt(p : ParakeetModel) : string {
    return tower_lane_fmt(p.q8, p.gemm_f16) if (!p.kq_lane)
    return tower_kq_lane_name(p.kq_fmt)
}

//! Parakeet's log-mel: preemphasis 0.97, zero center-pad n_fft/2, the file's STFT window, power
//! spectra, mel dot, NATURAL log(sum + 2⁻²⁴), then per-feature mean/std normalization over the
//! valid frames only (Bessel n−1, std + 1e-5), applied to all. FRAME-major ``[n_len][n_mel]``.
[hot_path]
def parakeet_log_mel(samples : array<float> | #; m : ParakeetModel; var st : ParakeetState) {
    let n_in = long_length(samples)
    ensure_length(st.pre, n_in)
    for (i in range(int(n_in))) {
        st.pre[i] = samples[i]
    }
    var pi = n_in - 1l
    while (pi > 0l) {   // backward in place: x[i] -= 0.97·x[i−1]
        st.pre[pi] -= 0.97 * st.pre[pi - 1l]
        pi--
    }
    let pad = m.n_fft / 2l
    let n_padded = n_in + 2l * pad
    ensure_length(st.padded, n_padded)
    for (i in range(int(pad))) {   // zero head pad (scratch is reused, not fresh)
        st.padded[i] = 0.0
    }
    for (i in range(int(n_in))) {
        st.padded[int(pad) + i] = st.pre[i]
    }
    for (i in range64(n_in + pad, n_padded)) {   // zero tail pad
        st.padded[i] = 0.0
    }

    st.mel.n_mel = m.n_mel
    st.mel.n_len = (n_padded - m.n_fft) / PARAKEET_HOP + 1l
    st.mel.n_len_org = st.mel.n_len
    ensure_length(st.mel.data, st.mel.n_len * m.n_mel)

    let n_fft = m.n_fft
    let n_bins = n_fft / 2l + 1l
    let wsz = length(m.window)
    let wpad = (n_fft - wsz) / 2l
    let mel_eps = 5.960464477539063e-08lf
    let n_len = st.mel.n_len
    let n_mel = m.n_mel

    // per-frame windowed 512-pt FFT + mel matvec, threaded over frames (frames are
    // independent — bit-exact at any lane count); scratch is a per-worker stack frame,
    // so a 9-minute clip no longer materializes the whole [frames × n_fft] image
    unsafe {
        let pdd = addr<float const?>(st.padded[0])
        let wnd = plane_at(m.window, 0l)
        let flt = plane_at(m.filters, 0l)
        let rev = addr<int const?>(m.fft.rev[0])
        let tw = addr<float const?>(m.fft.tw[0])
        var md = addr(st.mel.data[0])
        maybe_parallel_for(0, int(n_len), lanes_for_work(n_len * n_fft * 8l, 0)) $(rb, re) {
            var frame : float[FFT_FRAME]   // 512 interleaved (re, im) pairs, stack-resident
            unsafe {
                var fp = addr(frame[0])
                for (i in range64(int64(rb), int64(re))) {
                    let off = i * PARAKEET_HOP
                    for (j in range64(n_fft * 2l)) {
                        fp[j] = 0.0
                    }
                    let n_do = min(wsz, n_padded - off)   // window applied at [wpad, wpad+wsz)
                    for (j in range64(n_do)) {
                        if (off + wpad + j < n_padded) {
                            fp[(wpad + j) * 2l] = wnd[j] * pdd[off + wpad + j]
                        }
                    }
                    fft_pow2_run(n_fft, rev, tw, fp)
                    for (k in range64(n_bins)) {   // power spectrum, packed to the front
                        fp[k] = fp[2l * k] * fp[2l * k] + fp[2l * k + 1l] * fp[2l * k + 1l]
                    }
                    for (jm in range64(n_mel)) {   // tuned dot: 3.5x the 4-acc scalar form (conv_sub_bench --meldot)
                        let sum = double(dot(flt + jm * n_bins, fp, n_bins))
                        md[i * n_mel + jm] = float(log(sum + mel_eps))
                    }
                }
            }
        }
    }

    // per-feature normalization over the valid frames, applied to all
    let valid = n_in / PARAKEET_HOP
    for (j in range64(m.n_mel)) {
        var sum = 0.0lf
        for (i in range64(valid)) {
            sum += double(st.mel.data[i * m.n_mel + j])
        }
        let mean = sum / double(valid)
        var sq = 0.0lf
        for (i in range64(valid)) {
            let dv = double(st.mel.data[i * m.n_mel + j]) - mean
            sq += dv * dv
        }
        let denom = sqrt(sq / (double(valid) - 1.0lf)) + 1e-5lf
        for (i in range64(st.mel.n_len)) {
            st.mel.data[i * m.n_mel + j] = float((double(st.mel.data[i * m.n_mel + j]) - mean) / denom)
        }
    }
}

// ===== conv subsampling =====

// c0: conv2d k3 s2 p1, fused bias+relu. w tap-major [9][cout] (repacked at load, L1-resident);
// bit-exact vs the old im2col+K9-GEMM+bias+relu chain and 6.5x faster even single-lane
// (conv_sub_bench); threads over oy rows (bit-exact at any lane count).
def private pk_conv2d_c1_s2_relu(var out : array<float>; x : array<float>; blob : PlaneF;
                                 w_off, b_off : int64; iw, ih, cout : int64) {
    let ow = conv_s2_out(iw)
    let oh = conv_s2_out(ih)
    ensure_length(out, oh * ow * cout)
    unsafe {
        var op = addr(out[0])
        let xp = addr<float const?>(x[0])
        let wp = plane_at(blob, w_off)
        let bp = plane_at(blob, b_off)
        let zero = float4(0.0)
        maybe_parallel_for(0, int(oh), lanes_for_work(oh * ow * cout * 9l, 0)) $(rb, re) {
            unsafe {
                for (oy in range64(int64(rb), int64(re))) {
                    let iy0 = oy * 2l - 1l
                    for (ox in range64(ow)) {
                        let ix0 = ox * 2l - 1l
                        var xs : float[9]
                        for (kh in range64(3l)) {   // 9 guarded scalar taps, hoisted out of the channel sweep
                            let iy = iy0 + kh
                            for (kw in range64(3l)) {
                                let ix = ix0 + kw
                                xs[kh * 3l + kw] = (iy >= 0l && iy < ih && ix >= 0l && ix < iw ? xp[iy * iw + ix] : 0.0)
                            }
                        }
                        let x0 = float4(xs[0], xs[0], xs[0], xs[0])
                        let x1 = float4(xs[1], xs[1], xs[1], xs[1])
                        let x2 = float4(xs[2], xs[2], xs[2], xs[2])
                        let x3 = float4(xs[3], xs[3], xs[3], xs[3])
                        let x4 = float4(xs[4], xs[4], xs[4], xs[4])
                        let x5 = float4(xs[5], xs[5], xs[5], xs[5])
                        let x6 = float4(xs[6], xs[6], xs[6], xs[6])
                        let x7 = float4(xs[7], xs[7], xs[7], xs[7])
                        let x8 = float4(xs[8], xs[8], xs[8], xs[8])
                        let o = (oy * ow + ox) * cout
                        var c = 0l
                        while (c < cout) {
                            var acc = x0 * (*reinterpret<float4 const?>(wp + c))
                            acc = mad(x1, *reinterpret<float4 const?>(wp + cout + c), acc)
                            acc = mad(x2, *reinterpret<float4 const?>(wp + 2l * cout + c), acc)
                            acc = mad(x3, *reinterpret<float4 const?>(wp + 3l * cout + c), acc)
                            acc = mad(x4, *reinterpret<float4 const?>(wp + 4l * cout + c), acc)
                            acc = mad(x5, *reinterpret<float4 const?>(wp + 5l * cout + c), acc)
                            acc = mad(x6, *reinterpret<float4 const?>(wp + 6l * cout + c), acc)
                            acc = mad(x7, *reinterpret<float4 const?>(wp + 7l * cout + c), acc)
                            acc = mad(x8, *reinterpret<float4 const?>(wp + 8l * cout + c), acc)
                            acc += *reinterpret<float4 const?>(bp + c)
                            *reinterpret<float4?>(op + o + c) = max(acc, zero)
                            c += 4l
                        }
                    }
                }
            }
        }
    }
}

// guarded single position of the depthwise 3x3 (borders where a tap falls off the image)
def private pk_dw_pos(var op : float?; xp, wp, bp : float const?; oy, ox, iw, ih, ch, ow : int64) {
    let o = (oy * ow + ox) * ch
    unsafe {
        for (c in range64(ch)) {
            var acc = 0.0
            for (kh in range64(3l)) {
                let iy = oy * 2l - 1l + kh
                if (iy >= 0l && iy < ih) {
                    for (kw in range64(3l)) {
                        let ix = ox * 2l - 1l + kw
                        if (ix >= 0l && ix < iw) {
                            acc += wp[(kh * 3l + kw) * ch + c] * xp[(iy * iw + ix) * ch + c]
                        }
                    }
                }
            }
            op[o + c] = acc + bp[c]
        }
    }
}

// depthwise conv2d k3 s2 p1, w tap-major [9][ch] (repacked at load): float4 over the
// contiguous channel axis, 9 taps unrolled, guarded borders (isolated bench: 7.2x scalar);
// interior threads over oy rows (independent — bit-exact at any lane count)
def private pk_conv2d_dw_s2(var out : array<float>; x : array<float>; blob : PlaneF;
                            w_off, b_off : int64; iw, ih, ch : int64) {
    let ow = conv_s2_out(iw)
    let oh = conv_s2_out(ih)
    let oy_end = (ih - 2l) / 2l + 1l   // last oy with iy=2*oy+1 in bounds, exclusive
    let ox_end = (iw - 2l) / 2l + 1l
    let rs = iw * ch
    ensure_length(out, oh * ow * ch)
    unsafe {
        var op = addr(out[0])
        let xp = addr<float const?>(x[0])
        let wp = plane_at(blob, w_off)
        let bp = plane_at(blob, b_off)
        maybe_parallel_for(1, int(oy_end), lanes_for_work(oh * ow * ch * 9l, 0)) $(rb, re) {
            unsafe {
                for (oy in range64(int64(rb), int64(re))) {
                    for (ox in range64(1l, ox_end)) {
                        let x0 = ((oy * 2l - 1l) * iw + ox * 2l - 1l) * ch
                        let o = (oy * ow + ox) * ch
                        var c = 0l
                        while (c < ch) {
                            var acc = (*reinterpret<float4 const?>(xp + x0 + c)) * (*reinterpret<float4 const?>(wp + c))
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + ch + c), *reinterpret<float4 const?>(wp + ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + 2l * ch + c), *reinterpret<float4 const?>(wp + 2l * ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + rs + c), *reinterpret<float4 const?>(wp + 3l * ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + rs + ch + c), *reinterpret<float4 const?>(wp + 4l * ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + rs + 2l * ch + c), *reinterpret<float4 const?>(wp + 5l * ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + 2l * rs + c), *reinterpret<float4 const?>(wp + 6l * ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + 2l * rs + ch + c), *reinterpret<float4 const?>(wp + 7l * ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + 2l * rs + 2l * ch + c), *reinterpret<float4 const?>(wp + 8l * ch + c), acc)
                            *reinterpret<float4?>(op + o + c) = acc + *reinterpret<float4 const?>(bp + c)
                            c += 4l
                        }
                    }
                }
            }
        }
        for (oy in range64(oh)) {
            let interior_y = oy >= 1l && oy < oy_end
            for (ox in range64(ow)) {
                if (!interior_y || ox == 0l || ox >= ox_end) {
                    pk_dw_pos(op, xp, wp, bp, oy, ox, iw, ih, ch, ow)
                }
            }
        }
    }
}

// feature permute xf[t][fr + w3*c] = y6[t][fr][c]: float4 over the contiguous c axis,
// 4 strided scalar stores per group (bench: 2.85x the scalar 3-loop); threads over t
def private pk_xf_permute(var xf : array<float>; y6 : array<float>; tt, w3, nch : int64) {
    let feat = w3 * nch
    unsafe {
        var fp = addr(xf[0])
        let yp = addr<float const?>(y6[0])
        maybe_parallel_for(0, int(tt), lanes_for_work(tt * feat, 0)) $(rb, re) {
            unsafe {
                for (t in range64(int64(rb), int64(re))) {
                    let base = t * feat
                    for (fr in range64(w3)) {
                        let yr = base + fr * nch
                        let fo = base + fr
                        var c = 0l
                        while (c < nch) {
                            let v = *reinterpret<float4 const?>(yp + yr + c)
                            fp[fo + w3 * c] = v.x
                            fp[fo + w3 * (c + 1l)] = v.y
                            fp[fo + w3 * (c + 2l)] = v.z
                            fp[fo + w3 * (c + 3l)] = v.w
                            c += 4l
                        }
                    }
                }
            }
        }
    }
}

// ===== encoder =====

//! The most encoder frames an encode takes: past it the oracle switches to local attention, which no chain here runs.
def parakeet_max_frames() : int64 => LOCAL_ATTN_THRESHOLD

//! The rel-pos embedding table of `tt` frames into `st.pos_emb`: [2 tt - 1 x d], interleaved (sin, cos) pairs,
//! and on the q8 lane its requant - one serves every layer's pos projection. A session keeps the table of the
//! frame count it last built.
[arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def parakeet_pos_table(m : ParakeetModel; var st : ParakeetState; tt : int64) {
    let d = m.n_state
    let block0_pos_seat = m.kq_lane ? int64(PkSeat.pos) : 0l
    let image = m.kq_lane ? (tower_gemm_seat(m, block0_pos_seat).fmt == KqFmt.q8 ? PkPosImage.q8 : PkPosImage.q8k) : (m.q8 ? PkPosImage.q8 : PkPosImage.none)
    return if (st.pos_tt == tt && st.pos_d == d && (st.pos_image == image || image == PkPosImage.none))
    let ww = 2l * tt - 1l
    rel_pos_table(st.pos_emb, d, tt, ww)
    if (m.kq_lane) {
        tower_requant(m, block0_pos_seat, st.pos_emb, d, ww, st.posq, st.poss, st.posbs)
    } elif (m.q8) {
        requant_rows_q8_sized(st.pos_emb, d, ww, st.posq, st.poss)
    }
    st.pos_tt = tt
    st.pos_d = d
    st.pos_image = image
}

//! FastConformer encoder over the whole mel: conv subsampling, then 24 conformer blocks - global attention only, panics past
//! the oracle's 8192-frame local-attention switch. Reads ``st.mel``, leaves rows in ``st.x`` and the device rows' generation
//! the encode left in ``st.enc_gen`` (`tower_rows_gen`), the transducer decode's key.
[hot_path]
def parakeet_encode(m : ParakeetModel; var st : ParakeetState) : int64 {
    let tt = pk_encode_rows(m, st)
    st.enc_gen = tower_rows_gen()
    return tt
}

def private pk_encode_rows(m : ParakeetModel; var st : ParakeetState) : int64 {
    let d = m.n_state
    let nch = m.n_sub_ch
    // the conformer block loop walks the subsampled frame count
    dispatch_phase_encode((st.mel.n_len + m.subsampling - 1l) / m.subsampling)

    if (g_pk_gpu_encode_set) {
        let served = invoke(g_pk_gpu_encode, m, st)
        return served if (served >= 0l)
    }
    pk_cpu_refuse(m, "encode")

    // conv subsampling ([time][mel] rows are already the [ih][iw][c=1] layout)
    var tp = ref_time_ticks()
    let t1 = conv_s2_out(st.mel.n_len)
    let w1 = conv_s2_out(m.n_mel)
    pk_conv2d_c1_s2_relu(st.y0, st.mel.data, m.blob, m.c0_w, m.c0_b, m.n_mel, st.mel.n_len, nch)
    pk_conv2d_dw_s2(st.y2, st.y0, m.blob, m.c2_w, m.c2_b, w1, t1, nch)
    let w2 = conv_s2_out(w1)
    let t2 = conv_s2_out(t1)
    ensure_length(st.y3, t2 * w2 * nch)
    pk_scan("y0", st.y0, t1 * w1 * nch)
    pk_scan("y2", st.y2, t2 * w2 * nch)
    tower_mm_rq(m, st.y3, m.c3_w, st.y2, st.xqi, st.xsi, st.xbsi, nch, nch, t2 * w2)   // 1x1 conv = per-position channel mix
    bias_relu_rows(st.y3, m.blob, m.c3_b, nch, t2 * w2)
    pk_conv2d_dw_s2(st.y5, st.y3, m.blob, m.c5_w, m.c5_b, w2, t2, nch)
    let w3 = conv_s2_out(w2)
    let t3 = conv_s2_out(t2)
    ensure_length(st.y6, t3 * w3 * nch)
    tower_mm_rq(m, st.y6, m.c6_w, st.y5, st.xqi, st.xsi, st.xbsi, nch, nch, t3 * w3)
    bias_relu_rows(st.y6, m.blob, m.c6_b, nch, t3 * w3)

    // features per time row: v[f + w3·c] = y6[t][f][c]; then pre_out into the residual stream
    let tt = t3
    if (tt > LOCAL_ATTN_THRESHOLD) {
        panic("dasLLAMA parakeet: {tt} encoder frames crosses the local-attention switch - clip audio to under ~10.9 minutes")
    }
    let feat = w3 * nch
    ensure_length(st.xf, tt * feat)
    pk_xf_permute(st.xf, st.y6, tt, w3, nch)
    ensure_length(st.x, tt * d)
    pk_scan("y6", st.y6, t3 * w3 * nch)
    tower_mm_rq(m, st.x, m.pre_w, st.xf, st.xqi, st.xsi, st.xbsi, feat, d, tt)
    add_bias_rows(st.x, m.blob, m.pre_b, d, tt)
    pk_scan("pre", st.x, tt * d)
    asr_prof_add("pk.conv_subsample", tp)
    tp = ref_time_ticks()

    // the oracle masks keys >= ceil(n_len_org/8): no masked tail on a full-clip encode, where ceil-per-stage == ceil(n/8)
    let n_real = (st.mel.n_len_org + m.subsampling - 1l) / m.subsampling

    let ww = 2l * tt - 1l
    parakeet_pos_table(m, st, tt)
    asr_prof_add("pk.pos_emb", tp)

    pk_blocks_cpu(m, st, tt, ww, n_real)
    return tt
}

//! the GPU encode hook: the whole encode off `st.mel` - the subsample front, the pre-projection and the blocks - into
//! `st.x`; answers the frame count, or -1 where it declined - a CPU image's encode, which the CPU chain then runs whole
typedef ParakeetGpuEncodeFn = function<(m : ParakeetModel; var st : ParakeetState) : int64>
var private g_pk_gpu_encode : ParakeetGpuEncodeFn
var private g_pk_gpu_encode_set = false

[arch(at="../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_parakeet_gpu_encode(fn : ParakeetGpuEncodeFn) {
    g_pk_gpu_encode = fn
    g_pk_gpu_encode_set = true
}

//! would a GPU driver serve the model's encode: a GPU-flavored image, its lane picked at load, never run on the CPU (`#tower-dev-block`) - the facade's arm
def parakeet_device_gpu(m : ParakeetModel) : bool => m.dev_backend != TowerDev.none

var private g_pk_pin = GemmLane.unset

//! Pin the parakeet weight format for subsequent loads: q8 (the default on every home; un-pinned, a quantized GGUF serves its own planes instead) vs the
//! exact lane (the halfword lane on a GPU home, the f32 planes on the CPU; also the parity legs).
//! ``reset_parakeet_q8`` returns to the policy default.
def set_parakeet_q8(on : bool) {
    g_pk_pin = lane_pin(on)
}

def reset_parakeet_q8() {
    g_pk_pin = GemmLane.unset
}

[arch(at="../ARCHITECTURE_MEDIA.md#tower-weight-lane")]
def parakeet_serves_q8() : bool => audio_lane_serves_q8(g_pk_pin, true)

//! a parakeet block's offsets as the FastConformer chains read them: canary's record with no GEMM bias (-1 = none)
def pk_fc_layer(lo : PkLayerOffs) : CanaryLayerOffs {
    return CanaryLayerOffs(nf1_w = lo.nf1_w, nf1_b = lo.nf1_b,
        ff1a_w = lo.ff1a_w, ff1a_b = -1l, ff1b_w = lo.ff1b_w, ff1b_b = -1l,
        nat_w = lo.nat_w, nat_b = lo.nat_b,
        q_w = lo.q_w, q_b = -1l, k_w = lo.k_w, k_b = -1l, v_w = lo.v_w, v_b = -1l, o_w = lo.o_w, o_b = -1l,
        pos_w = lo.pos_w, bias_u = lo.bias_u, bias_v = lo.bias_v,
        ncv_w = lo.ncv_w, ncv_b = lo.ncv_b,
        pw1_w = lo.pw1_w, pw1_b = -1l, dw_w = lo.dw_w, dw_b = -1l,
        bn_w = lo.bn_w, bn_b = lo.bn_b, bn_mean = lo.bn_mean, bn_var = lo.bn_var,
        pw2_w = lo.pw2_w, pw2_b = -1l,
        nf2_w = lo.nf2_w, nf2_b = lo.nf2_b,
        ff2a_w = lo.ff2a_w, ff2a_b = -1l, ff2b_w = lo.ff2b_w, ff2b_b = -1l,
        nout_w = lo.nout_w, nout_b = lo.nout_b)
}

//! The kq lane's record seats into `seats`, which the driver sizes to `parakeet_fc_seat_count`; answers the K-quant members' format-slot bits.
[arch(at = "../ARCHITECTURE_MEDIA_ASR.md#parakeet-kq-seats")]
def parakeet_fc_record_seats(m : ParakeetModel; var seats : array<TowerGemmSeat>) : uint {
    var kq_fmts = 0u
    verify(long_length(seats) == parakeet_fc_seat_count(m), "the driver sizes the record seats to the chain")
    for (s, i in seats, count()) {
        s = tower_gemm_seat(m, int64(i))
        if (s.fmt != KqFmt.q8) {
            kq_fmts |= 1u << uint(int(s.fmt))
        }
    }
    return kq_fmts
}

//! the GEMM records a device chain seats off a CPU-form kq model: none on another lane, or where a GPU image carries its own
def parakeet_fc_seat_count(m : ParakeetModel) : int64
    => m.kq_lane && m.dev_backend == TowerDev.none ? m.n_layer * PK_LAYER_SEATS + PK_FRONT_GEMMS : 0l

//! parakeet's CPU block form: no GEMM bias, tap-major taps, the folded BatchNorm, its stage buckets and the NaN scan
def private pk_cpu_form(m : ParakeetModel) : FcCpuForm {
    return FcCpuForm(d = m.n_state, ff = 4l * m.n_state, n_head = m.n_head, nk = m.n_conv_kernel, biased = false,
        prof = FcCpuProf(ffn1 = FcFfnProf(ln = "pk.ffn.ln", a = "pk.ffn.a", silu = "pk.ffn.silu", b = "pk.ffn.b", res = "pk.ffn.res"),
            ffn = "pk.ffn", proj_ln = "pk.attn_proj.ln", proj_rq = "pk.attn_proj.rq", proj_q = "pk.attn_proj.q",
            proj_k = "pk.attn_proj.k", proj_v = "pk.attn_proj.v", proj_pos = "pk.attn_proj.pos", proj = "pk.attn_proj",
            heads_pack = "pk.attn_heads.pack", heads_pos = "pk.attn_heads.pos", heads_blocks = "pk.attn_heads.blocks",
            heads = "pk.attn_heads", attn_out = "pk.attn_out", conv = "pk.conv_module"),
        scan_on = g_pk_nan_scan, scan = @@pk_scan_layer)
}

def private pk_blocks_cpu(m : ParakeetModel; var st : ParakeetState; tt, ww, n_real : int64) {
    let form = pk_cpu_form(m)
    for (l in range64(m.n_layer)) {
        fc_block_cpu(m, st, form, FcRelPos(ww = ww, n_real = n_real), pk_fc_layer(pk_layer_offsets(m, l)), l, tt)
    }
}

// ===== prediction network + joint + TDT greedy =====

//! parakeet rides RnntNet's defaults: the LSTM gates packed i, f, o, g, one bias, the first step on the blank's embedding row.
def private pk_rnnt_net(m : ParakeetModel) : RnntNet {
    var net = RnntNet(blob = m.blob, n_layers = m.n_pred_layers, pred_dim = m.n_pred_dim, joint_dim = m.n_pred_dim,
        n_tokens = m.n_vocab + 1l, blank_id = m.blank_id, durations = m.durations, max_symbols = int(m.n_max_tokens),
        prof_step = "pk.dec.step", prof_pick = "pk.dec.pick", prof_joint = "pk.dec.joint", dev_only = m.dev_backend != TowerDev.none)
    rnnt_net_baked(net, m.dec_slab, m.rnnt_wh, m.rnnt_jw)
    return net
}

// NaN/Inf stage scan (debug rail, pairs with the fast_math x64 investigation): bit-pattern
// detection — a float `v != v` check folds to false under fast_math's nnan, this survives.
var g_pk_nan_scan = false

def set_pk_nan_scan(on : bool) {
    g_pk_nan_scan = on
}

[cold_path]
def private pk_scan_layer(tag : string; l : int64; x : array<float>; n : int64) {
    // layered spelling: the layer rides separately so the hot loop builds no string when off
    if (!g_pk_nan_scan) {
        return
    }
    pk_scan("L{l}.{tag}", x, n)
}

[cold_path]
def private pk_scan(tag : string; x : array<float>; n : int64) {
    if (!g_pk_nan_scan) {
        return
    }
    var nan = 0
    var inf = 0
    var first = -1l
    for (i in range64(n)) {
        let bits = unsafe(reinterpret<uint>(x[i]))
        if ((bits & 0x7F800000u) == 0x7F800000u) {
            if ((bits & 0x007FFFFFu) != 0u) {
                nan++
            } else {
                inf++
            }
            if (first < 0l) {
                first = i
            }
        }
    }
    if (nan + inf > 0) {
        print("pk_scan {tag}: nan={nan} inf={inf} first={first} of {n}\n")
    } else {
        print("pk_scan {tag}: clean ({n})\n")
    }
}

//! Greedy TDT decode over the encoder rows in ``st.x``: the joint's encoder projection of every frame,
//! then the shared transducer loop (`rnnt_greedy`). Tokens land in ``st.toks``.
[hot_path]
def parakeet_tdt_decode(m : ParakeetModel; var st : ParakeetState; n_frames : int64) {
    let d = m.n_state
    let pd = m.n_pred_dim
    let enc = RnntEnc(d = d, w = m.joint_enc_w, b = m.joint_enc_b, q8 = (m.q8 || m.kq_lane) && !m.jenc_exact, gen = st.enc_gen)
    rnnt_decode(pk_rnnt_net(m), st.dec, enc, st.x, st.encp, n_frames, st.toks) $(var encp) {
        pk_cpu_refuse(m, "joint's encoder projection")
        let ts_encp = ref_time_ticks()
        ensure_length(encp, n_frames * pd)
        tower_mm_rq(m, encp, m.joint_enc_w, st.x, st.xqi, st.xsi, st.xbsi, d, pd, n_frames)
        add_bias_rows(encp, m.blob, m.joint_enc_b, pd, n_frames)
        asr_prof_add("pk.dec.encp", ts_encp)
    }
}

//! The transcript of `toks`: the shared SentencePiece rule over the model's vocab.
def parakeet_detokenize(m : ParakeetModel; toks : array<ParakeetToken> | #) : string => asr_spm_detokenize(m.vocab, toks)

// v3 (25-lang) has an 8192-piece SPM vocab; v2 (English) 1024 — the multilingual threshold
def pk_is_multilingual(m : ParakeetModel) : bool => m.n_vocab >= 8192l

//! Parakeet's capability sheet: v2 English-only, v3 the 25 EU languages; segment timestamps.
def parakeet_asr_caps(m : ParakeetModel) : AsrCaps {
    var c = AsrCaps(translate = false, timestamps = AsrTimestamps.segment,
                    streaming = false, prompt = false, default_lang = "en")
    if (pk_is_multilingual(m)) {
        c.languages <- ["bg", "hr", "cs", "da", "nl", "en", "et", "fi", "fr", "de",
                        "el", "hu", "it", "lv", "lt", "mt", "pl", "pt", "ro", "sk",
                        "sl", "es", "sv", "ru", "uk"]
    } else {
        c.languages <- ["en"]
    }
    return <- c
}

//! Session-create validation: parakeet has no language conditioning — the TDT decode
//! transcribes whatever it hears; ``lang`` is checked against the model's set, then unused.
def parakeet_check_lang(m : ParakeetModel; lang : string) {
    if (lang != "auto" && !has_value(parakeet_asr_caps(m).languages, lang)) {
        panic(pk_is_multilingual(m) ?
            "dasLLAMA asr: '{lang}' is not one of this Parakeet model's 25 languages" :
            "dasLLAMA asr: this Parakeet model is English-only")
    }
}

//! The family's encode-total ``asr_prof`` bucket: the whole FastConformer encode, on whichever home serves it.
let PARAKEET_ENCODE_BUCKET = "pk.encode_total"

//! The facade's transcribe arm: full-clip FastConformer encode + TDT greedy, one segment.
def parakeet_transcribe(m : ParakeetModel; var st : ParakeetState; samples : array<float> | #;
                        blk : block<(seg : TranscribeSegment)>; var detected_lang : string&) : int {
    var tp = ref_time_ticks()
    parakeet_log_mel(samples, m, st)
    asr_prof_add("pk.mel", tp)
    tp = ref_time_ticks()
    let tt = parakeet_encode(m, st)
    asr_prof_add(PARAKEET_ENCODE_BUCKET, tp)
    tp = ref_time_ticks()
    parakeet_tdt_decode(m, st, tt)
    asr_prof_add("pk.decode_total", tp)
    let seg <- asr_segment(0l, long_length(samples) * 100l / AUDIO_SAMPLE_RATE, parakeet_detokenize(m, st.toks), st.toks)
    if (!pk_is_multilingual(m)) {
        detected_lang = "en"   // v3 transcribes without detecting/reporting a language
    }
    blk |> invoke(seg)
    return 1
}
