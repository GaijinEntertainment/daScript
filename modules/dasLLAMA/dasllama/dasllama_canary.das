options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_canary shared public
options _cyclomatic_complexity = 0   // model forward/serialize walks mirror the model schema
options _function_length = 0   // same

require dasllama/dasllama_lint public
require math
require daslib/fio
require strings
require daslib/strings_boost
require daslib/array_boost
require daslib/apply
require daslib/archive
require dasllama/dasllama_env
require dasllama/dasllama_math
require dasllama/dasllama_gguf
require dasllama/dasllama_plane public   // PlaneF — the borrowed weight planes; re-exported so requirers can delete an encoder
require dasllama/dasllama_asr_types public   // TranscribeSegment/AsrCaps — the shared ASR floor
require dasllama/dasllama_audio
require dasllama/dasllama_tower
require dasllama/dasllama_tower_image
require dasllama/dasllama_blocks   // embed_text_rows/eval_embd_ — the ASR turn drives the Qwen3 decoder
require dasllama/dasllama_common   // apply_box_profile_runtime for the canary-image identity
require dasllama/dasllama_convert   // quantize_q8_0_into — the q8 load's read-time transcode
require dasllama/dasllama_par   // maybe_parallel_for/lanes_for_work — the slab attention dispatch
require dasllama/dasllama_image

// Canary-Qwen 2.5B audio front end: the FastConformer-32 encoder + the perception.proj
// linear (audio d_model 1024 -> Qwen3 hidden 2048), read from a self-describing named-tensor
// f32 bin (harness/convert_canary_to_ggml.py output). Architecture = NVIDIA FastConformer
// (parakeet's conformer neighborhood) PLUS the qkv/ffn/conv biases parakeet lacks; the TDT
// predictor/joint are dropped. Default load transcodes the GEMM regions to Q8_0 at read time
// (parakeet's recipe over canary's name-keyed layout); fp32 stays the oracle arm
// (set_asr_fp32); oracle = NeMo SALM greedy (harness/canary_qwen_expected.tsv).

let CANARY_MAGIC = 0x59524e43u   // "CNRY"
// Decode budget, mirroring the NeMo reference arm (canary_qwen_bench.py --max-new-tokens 256).
// Uncapped, a missed EOS on out-of-envelope audio (trained: 40 s / 1024 tokens) decodes to
// ctx-full — the cell then times runaway length, not ASR speed.
let private CN_MAX_NEW = 256l
let private CN_PREEMPH = 0.97
let private CN_HOP = 160l
let private CN_MEL_FLOOR = 5.960464477539063e-08lf   // 2^-24 (the log zero guard)

//! Loaded canary encoder: hparams, mel filterbank + STFT window from the bin, the FastConformer
//! weights (all fp32 in one ``blob``), and the projector into the decoder embedding space.
struct CanaryEncoder {
    d_model : int64
    n_layer : int64
    n_head : int64
    n_ff : int64
    n_mel : int64
    n_fft : int64
    subsampling : int64
    n_sub_ch : int64
    conv_kernel : int64
    proj_dim : int64
    n_fb : int64
    eps : float = 1e-5
    filters : PlaneF = PlaneF()    // [n_mel × n_fb]
    window : PlaneF = PlaneF()
    twiddles : PlaneF = PlaneF()   // DFT twiddle matrix [n_fft × (n_fb·2)]
    blob : PlaneF = PlaneF()
    // pre-encode subsampling stack
    c0_w : int64; c0_b : int64
    c2_w : int64; c2_b : int64
    c3_w : int64; c3_b : int64
    c5_w : int64; c5_b : int64
    c6_w : int64; c6_b : int64
    pre_w : int64; pre_b : int64
    layers : CanaryLayerPlane = CanaryLayerPlane()
    proj_w : int64; proj_b : int64
    // Q8 fast path (default load): Q8_0 image of the GEMM regions, transcoded straight from
    // the bin and repacked for the active backend. GEMM offset fields index qblob on a q8
    // load, blob on fp32 — tower_mm is the only sanctioned route through them.
    q8 : bool
    qblob : PlaneI8 = PlaneI8()
    qscales : PlaneF = PlaneF()
    dev_backend : TowerDev    //! the GPU flavor whose device block the image carries (`#tower-dev-block`); `none` on the CPU image
    dev : PlaneU8 = PlaneU8()
    devplan : PlaneI64 = PlaneI64()
    image_map : void?         // non-null when the planes borrow a mapped .dlim (dasllama_image)
    image_bytes : uint64
}

// the per-layer offset table is a plane like any other; PlaneF/PlaneI8 cover the scalar
// element types, a POD-struct plane needs its own two accessors
struct CanaryLayerPlane {
    borrowed_plane : bool = true
    p : CanaryLayerOffs const? = null
    n : int64 = 0l
}

def length(pl : CanaryLayerPlane) : int64 => pl.n

def operator [] (pl : CanaryLayerPlane; i : int64) : CanaryLayerOffs {
    if (uint64(i) >= uint64(pl.n)) {
        panic("dasLLAMA canary: layer {i} outside [0,{pl.n})")
    }
    return unsafe(pl.p[i])
}

def finalize(var pl : CanaryLayerPlane) {
    pl.p = null
    pl.n = 0l
}

def plane_bind(var pl : CanaryLayerPlane; base : uint8 const?; off, nbytes : int64) : bool {
    let esz = int64(typeinfo sizeof(type<CanaryLayerOffs>))
    if (nbytes % esz != 0l) {
        return false
    }
    unsafe {
        pl.p = reinterpret<CanaryLayerOffs const?>(base + off)
    }
    pl.n = nbytes / esz
    return true
}

// the encoder owns exactly one thing — its image backing. Every plane borrows.
def finalize(var t : CanaryEncoder) {
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

// the tripwire: serialize_image_meta(CanaryEncoder) covers 27 fields; image_map + image_bytes
// are deliberate skips (the seven planes are sections, not meta).
let private CANARY_META_FIELDS = 28 + 2

// every meta-carried CanaryEncoder field, explicitly, both directions (see dasllama_image)
def private serialize_image_meta(var arch : Archive; var t : CanaryEncoder) {
    verify(count_meta_fields(t) == CANARY_META_FIELDS)   // grew CanaryEncoder? extend this list
    arch |> serialize_raw(t.d_model)
    arch |> serialize_raw(t.n_layer)
    arch |> serialize_raw(t.n_head)
    arch |> serialize_raw(t.n_ff)
    arch |> serialize_raw(t.n_mel)
    arch |> serialize_raw(t.n_fft)
    arch |> serialize_raw(t.subsampling)
    arch |> serialize_raw(t.n_sub_ch)
    arch |> serialize_raw(t.conv_kernel)
    arch |> serialize_raw(t.proj_dim)
    arch |> serialize_raw(t.n_fb)
    arch |> serialize_raw(t.eps)
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
    arch |> serialize_raw(t.proj_w)
    arch |> serialize_raw(t.proj_b)
    arch |> serialize_raw(t.q8)
    arch |> serialize_raw(t.dev_backend)
}

[unused_argument(t)]
def private image_post_load(var t : CanaryEncoder) {}   // no derived state — offsets ARE the meta

//! The mint-side carrier: ``e`` holds every scalar the meta blob carries, the arrays beside it
//! hold the plane bytes until the writer has them. Public because reading the source bin off-rail
//! is the only way a suite can check a served encoder against something not itself image-backed.
struct CanaryStaging {
    e : CanaryEncoder = CanaryEncoder()
    filters : array<float>
    window : array<float>
    twiddles : array<float>
    blob : array<float>
    layers : array<CanaryLayerOffs>
    qblob : array<int8>       // section names must match the served plane fields (bind-by-name)
    qscales : array<float>
    @exact_size dev : array<uint8>       //!< a GPU-flavored mint's device block (`TowerDevBuild`)
    @exact_size devplan : array<int64>   //!< ... and its plan
}

def private serialize_image_meta(var arch : Archive; var st : CanaryStaging) {
    serialize_image_meta(arch, st.e)
}

def finalize(var st : CanaryStaging) {
    delete_staging(st)
}

[unused_argument(st)]
def private image_post_load(var st : CanaryStaging) {}

[init]
def private register_canary_image_tags {
    register_image_family_tag("canary-q8")
    register_image_family_tag("canary-f32", true)   // config-free: the exact lane's planes are the file's own, no box property shapes them
    register_tower_dev_tags("canary-q8")
    register_tower_dev_tags("canary-f32")
}

//! Every hot-path buffer of the canary encode, so a transcribe allocates nothing per call.
struct CanaryState {
    mel : array<float>        // frame-major [n_len × n_mel]
    n_len : int64
    n_valid : int64           // real mel frame count (n_in / hop) — drives the conv length mask
    mel_ready : bool          //! the spectrum half ran over the windowed frames (the device front computes its own)
    // mel scratch
    pre : array<float>
    padded : array<float>
    xw : array<float>
    reim : array<float>
    // conv scratch
    col : array<float>
    y0 : array<float>
    y2 : array<float>
    y3 : array<float>
    y5 : array<float>
    y6 : array<float>
    xf : array<float>
    // encoder
    x : array<float>
    xb : array<float>
    h : array<float>
    ao : array<float>
    qq : array<float>
    kk : array<float>
    vv : array<float>
    pos_emb : array<float>
    posl : array<float>
    qu : array<float>
    qv : array<float>
    kt : array<float>
    vh : array<float>
    posh : array<float>
    sc : array<float>
    rl : array<float>
    oh : array<float>
    out : array<float>        // projected soft tokens [tt × proj_dim]
    xqi : array<int8>         // q8 activation image — one requant shared by q/k/v, per-site otherwise
    xsi : array<float>
    xbsi : array<int>         //!< the Q8_K image's per-16 sums (a kq-lane tower's; the FC chain passes it, canary never fills it)
    posq : array<int8>        // pos_emb rows requantized once per encode (layer-invariant input)
    poss : array<float>
}

// ===== bin container =====

struct private CnRec {
    name : string
    file_off : int64
    n_elem : int64
}

def private cn_bytes_str(b : array<uint8> | #; o, n : int64) : string {
    if (n <= 0l) {
        return ""
    }
    var s = ""
    unsafe {
        s = string(temp_array(addr<uint8?>(b[o]), n, type<uint8>))
    }
    return s
}

def private cn_rd_i64(b : array<uint8> | #; o : int64) : int64 =>
    int64(rd_u32(b, o)) | (int64(rd_u32(b, o + 4l)) << 32l)

def private cn_read_rec(b : array<uint8> | #; rec : CnRec; var blob : array<float>; o : int64) {
    for (i in range64(rec.n_elem)) {
        blob[o + i] = rd_f32(b, rec.file_off + i * 4l)
    }
}

// pull one record out of the table — leftovers after the schema walk are converter drift
def private cn_take(var recs : table<string; CnRec>; name : string) : CnRec {
    if (!key_exists(recs, name)) {
        panic("dasLLAMA canary: missing tensor '{name}' in encoder bin")
    }
    let r = recs?[name] ?? CnRec()
    recs |> erase(name)
    return r
}

// fp32-side read: appends at the blob cursor, returns the offset the field records
def private cn_read_plain(var recs : table<string; CnRec>; b : array<uint8> | #; name : string;
                          var blob : array<float>; var po : int64&) : int64 {
    let r = cn_take(recs, name)
    cn_read_rec(b, r, blob, po)
    let at = po
    po += r.n_elem
    return at
}

// GEMM read: a q8 load transcodes it from the file straight into qblob/qscales (the fp32 blob
// never holds it); an fp32 load reads it like any plain tensor
def private cn_read_gemm(var recs : table<string; CnRec>; b : array<uint8> | #; name : string;
                         var st : CanaryStaging; @scratch var wscratch : array<float>;
                         var po : int64&; var qo : int64&) : int64 {
    if (!st.e.q8) {
        return cn_read_plain(recs, b, name, st.blob, po)
    }
    let r = cn_take(recs, name)
    if (r.n_elem % 32l != 0l) {
        panic("dasLLAMA canary q8: GEMM tensor '{name}' has {r.n_elem} elements - not 32-aligned")
    }
    if (long_length(wscratch) < r.n_elem) {
        wscratch |> reserve(r.n_elem)
        wscratch |> resize(r.n_elem)
    }
    cn_read_rec(b, r, wscratch, 0l)
    quantize_q8_0_into(wscratch, r.n_elem, st.qblob, st.qscales, qo, qo / 32l)
    let at = qo
    qo += r.n_elem
    return at
}

// the 15-name GEMM set the q8 image carries; everything else stays fp32 (c0's im2col n=9
// cannot quantize — the whisper/tower conv1 exception, here by name instead of by width)
let private CN_GEMM_SUFFIXES <- [".ff1.linear1.weight", ".ff1.linear2.weight", ".attn.q.weight",
    ".attn.k.weight", ".attn.v.weight", ".attn.out.weight", ".attn.pos.weight",
    ".conv.pw1.weight", ".conv.pw2.weight", ".ff2.linear1.weight", ".ff2.linear2.weight"]

def private cn_is_gemm_name(name : string) : bool {
    if (name == "pre_encode.conv.3.weight" || name == "pre_encode.conv.6.weight"
            || name == "pre_encode.out.weight" || name == "proj.weight") {
        return true
    }
    for (sfx in CN_GEMM_SUFFIXES) {
        if (name |> ends_with(sfx)) {
            return true
        }
    }
    return false
}

// repack one GEMM region for the active backend (scales index at woff/32)
def private cn_repack(var st : CanaryStaging; woff, n, d : int64) {
    if (woff % 32l != 0l) {
        panic("dasLLAMA canary q8: GEMM weight at {woff} is not 32-aligned")
    }
    if (n % 32l != 0l) {
        // n_elem%32==0 does not imply per-row alignment — straddling blocks serve wrong scales
        panic("dasLLAMA canary q8: GEMM row width {n} is not 32-aligned - Q8_0 blocks would straddle rows")
    }
    repack_q8q8_weight(st.qblob, st.qscales, woff, n, d)
}

// repack every GEMM region for the active backend's q8q8 kernels; without a selected
// backend matmul_q8q8_batch reads the flat Q8_0 image as-is
def private cn_repack_all(var st : CanaryStaging) {
    if (!tower_repack_backend()) {
        return
    }
    let d = st.e.d_model
    let ff = st.e.n_ff
    let nch = st.e.n_sub_ch
    let w3 = conv_s2_out(conv_s2_out(conv_s2_out(st.e.n_mel)))
    let feat = w3 * nch
    cn_repack(st, st.e.c3_w, nch, nch)
    cn_repack(st, st.e.c6_w, nch, nch)
    cn_repack(st, st.e.pre_w, feat, d)
    for (l in range64(st.e.n_layer)) {
        let lo = st.layers[l]
        cn_repack(st, lo.ff1a_w, d, ff)
        cn_repack(st, lo.ff1b_w, ff, d)
        cn_repack(st, lo.q_w, d, d)
        cn_repack(st, lo.k_w, d, d)
        cn_repack(st, lo.v_w, d, d)
        cn_repack(st, lo.o_w, d, d)
        cn_repack(st, lo.pos_w, d, d)
        cn_repack(st, lo.pw1_w, d, 2l * d)
        cn_repack(st, lo.pw2_w, d, d)
        cn_repack(st, lo.ff2a_w, d, ff)
        cn_repack(st, lo.ff2b_w, ff, d)
    }
    cn_repack(st, st.e.proj_w, d, st.e.proj_dim)
}

// BN inference fold, with the depthwise conv bias absorbed: scale = γ/sqrt(σ²+eps),
// shift = β + (b_dw − μ)·scale. bn_w/bn_b slots take scale/shift; mean/var stay as read.
def private cn_fold_bn(var blob : array<float>; w, b, mean, vr, dwb, d : int64; eps : float) {
    for (j in range64(d)) {
        let s = blob[w + j] / sqrt(blob[vr + j] + eps)
        blob[w + j] = s
        blob[b + j] += (blob[dwb + j] - blob[mean + j]) * s
    }
}

//! Load a canary encoder bin (convert_canary_to_ggml.py output; magic "CNRY", all-f32 on disk).
//! With ``q8`` the GEMM weights transcode to Q8_0 straight from the mapped file (conv im2col, depthwise, biases, norms stay fp32).
//! Caches a box/knob-specific PREPARED IMAGE like ``load_model``; ``DASLLAMA_IMAGE=0`` disables it, a ``.dlim`` path loads one directly.
def load_canary_encoder(path : string; q8 : bool = false) : CanaryEncoder {
    let tag = q8 ? "canary-q8" : "canary-f32"
    announce_lane("canary encoder", q8, "f32 planes", "set_canary_enc_q8", g_cn_pin != GemmLane.unset, canary_gpu_serves())
    let dev = path |> ends_with(".dlim") ? TowerDev.none : canary_dev_flavor(q8)
    return <- load_staged_image_dev(path, tag, "canary-encoder", "source bin", type<CanaryEncoder>, dev, g_cn_flavor.bake, CANARY_DEV_CONSUMED) {
        return <- stage_canary_encoder(path, q8)
    }
}

//! A GPU driver's Canary encoder flavor (`#tower-dev-block`): the backend whose image it mints, whether its chain serves a lane,
//! and the bake that converts a CPU-form encoder into the driver's device block
typedef CanaryServesLaneFn = function<(q8 : bool) : bool>
typedef CanaryBakeFn = function<(m : CanaryEncoder; var b : TowerDevBuild) : bool>

struct CanaryFlavor {
    backend : TowerDev
    serves : CanaryServesLaneFn
    bake : CanaryBakeFn
}

var private g_cn_flavor = CanaryFlavor()
let private CANARY_DEV_CONSUMED <- ["qblob", "qscales"]   //! the planes a Canary bake converts: a GPU flavor carries their device forms instead

def register_canary_flavor(backend : TowerDev; serves : CanaryServesLaneFn; bake : CanaryBakeFn) {
    g_cn_flavor = CanaryFlavor(backend = backend, serves = serves, bake = bake)
}

//! The device flavor a load on the `q8` lane takes: the registered driver's where its chain serves the lane, else `none` - the
//! CPU image
def canary_dev_flavor(q8 : bool) : TowerDev {
    return TowerDev.none if (g_cn_flavor.backend == TowerDev.none || !invoke(g_cn_flavor.serves, q8))
    return g_cn_flavor.backend
}

//! A GPU-flavored encoder reached a CPU path at `stage`: the device seat declined what the load handed it, and the CPU never runs
//! that image (`#tower-dev-block`)
def private cn_cpu_refuse(m : CanaryEncoder; stage : string) {
    if (m.dev_backend != TowerDev.none) {
        panic("dasLLAMA canary: the {stage} declined on the {m.dev_backend} device - a {m.dev_backend}-flavored image never runs on the CPU")
    }
}

//! The loaded precision as ``<encoder>/<decoder>`` (the decoder tag comes from the facade,
//! which owns the shared LLM): the facade's [[asr_exec_fmt]] arm.
def canary_exec_fmt(e : CanaryEncoder; dec_fmt : string) : string =>
    "{e.q8 ? "q8" : "f32"}/{dec_fmt}"

//! Canary's encode-total ``asr_prof`` bucket (the facade's [[asr_encode_bucket]] arm).
def canary_encode_bucket : string => "cn.encode"

//! Mint a served encoder from a staged read WITHOUT touching disk — the in-memory image rail.
//! The off-rail control for suites: a live encoder that came from no `.dlim` file.
def mint_canary_encoder(var st : CanaryStaging; var out : CanaryEncoder) {
    // the tag is the staging's own precision — a caller-supplied flag could bake a lying tag
    cache_via_image_staged(st, out, "", st.e.q8 ? "canary-q8" : "canary-f32")
}

// The CNRY-bin reader proper: scalars land in the served encoder, plane bytes in the staging
// arrays beside it. Nothing here is servable — the image build is what produces a live encoder.
def stage_canary_encoder(path : string; q8 : bool = false) : CanaryStaging {
    var st = CanaryStaging()
    st.e.q8 = q8
    let n_bytes = int64(stat(path).size)
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA canary: cannot open encoder '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var o = 0l
        if (rd_u32(bytes, o) != CANARY_MAGIC) {
            panic("dasLLAMA canary: bad magic in '{path}'")
        }
        o += 4l
        o += 4l   // version
        st.e.d_model = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_layer = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_head = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_ff = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_mel = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_fft = int64(rd_i32(bytes, o)); o += 4l
        st.e.subsampling = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_sub_ch = int64(rd_i32(bytes, o)); o += 4l
        st.e.conv_kernel = int64(rd_i32(bytes, o)); o += 4l
        st.e.proj_dim = int64(rd_i32(bytes, o)); o += 4l
        st.e.eps = rd_f32(bytes, o); o += 4l
        st.e.n_fb = int64(rd_i32(bytes, o)); o += 4l
        // mel filterbank [n_mel × n_fb]
        let nfilt = st.e.n_mel * st.e.n_fb
        st.filters |> resize(nfilt)
        for (i in range64(nfilt)) {
            st.filters[i] = rd_f32(bytes, o + i * 4l)
        }
        o += nfilt * 4l
        // STFT window
        let n_window = int64(rd_i32(bytes, o)); o += 4l
        st.window |> resize(n_window)
        for (i in range64(n_window)) {
            st.window[i] = rd_f32(bytes, o + i * 4l)
        }
        o += n_window * 4l

        // named tensor records (name_len i32, name, n_elem i64, f32 data) — indexed by name;
        // the schema walk below reads by file_off, so blob layout is SCHEMA order, not file order
        var recs : table<string; CnRec>
        while (o + 4l <= n_bytes) {
            let name_len = int64(rd_i32(bytes, o)); o += 4l
            let name = cn_bytes_str(bytes, o, name_len); o += name_len
            let n_elem = cn_rd_i64(bytes, o); o += 8l
            recs[name] = CnRec(name = name, file_off = o, n_elem = n_elem)
            o += n_elem * 4l
        }

        // size the planes exactly (reserve-exact — array growth rounds to pow2 otherwise)
        var total_plain = 0l
        var total_gemm = 0l
        for (r in values(recs)) {
            if (q8 && cn_is_gemm_name(r.name)) {
                total_gemm += r.n_elem
            } else {
                total_plain += r.n_elem
            }
        }
        st.blob |> reserve(total_plain)
        st.blob |> resize(total_plain)
        if (q8) {
            st.qblob |> reserve(total_gemm)
            st.qblob |> resize(total_gemm)
            st.qscales |> reserve(total_gemm / 32l)
            st.qscales |> resize(total_gemm / 32l)
        }

        // schema walk: each read consumes its record and returns the offset its field records
        // (GEMM offsets index qblob on a q8 load — see tower_mm)
        var wscratch : array<float>
        var po = 0l
        var qo = 0l
        st.e.c0_w = cn_read_plain(recs, bytes, "pre_encode.conv.0.weight", st.blob, po)
        st.e.c0_b = cn_read_plain(recs, bytes, "pre_encode.conv.0.bias", st.blob, po)
        st.e.c2_w = cn_read_plain(recs, bytes, "pre_encode.conv.2.weight", st.blob, po)
        st.e.c2_b = cn_read_plain(recs, bytes, "pre_encode.conv.2.bias", st.blob, po)
        st.e.c3_w = cn_read_gemm(recs, bytes, "pre_encode.conv.3.weight", st, wscratch, po, qo)
        st.e.c3_b = cn_read_plain(recs, bytes, "pre_encode.conv.3.bias", st.blob, po)
        st.e.c5_w = cn_read_plain(recs, bytes, "pre_encode.conv.5.weight", st.blob, po)
        st.e.c5_b = cn_read_plain(recs, bytes, "pre_encode.conv.5.bias", st.blob, po)
        st.e.c6_w = cn_read_gemm(recs, bytes, "pre_encode.conv.6.weight", st, wscratch, po, qo)
        st.e.c6_b = cn_read_plain(recs, bytes, "pre_encode.conv.6.bias", st.blob, po)
        st.e.pre_w = cn_read_gemm(recs, bytes, "pre_encode.out.weight", st, wscratch, po, qo)
        st.e.pre_b = cn_read_plain(recs, bytes, "pre_encode.out.bias", st.blob, po)

        st.layers |> resize(st.e.n_layer)
        for (l in range64(st.e.n_layer)) {
            let lo = CanaryLayerOffs(
                nf1_w = cn_read_plain(recs, bytes, "L{l}.norm_ff1.weight", st.blob, po),
                nf1_b = cn_read_plain(recs, bytes, "L{l}.norm_ff1.bias", st.blob, po),
                ff1a_w = cn_read_gemm(recs, bytes, "L{l}.ff1.linear1.weight", st, wscratch, po, qo),
                ff1a_b = cn_read_plain(recs, bytes, "L{l}.ff1.linear1.bias", st.blob, po),
                ff1b_w = cn_read_gemm(recs, bytes, "L{l}.ff1.linear2.weight", st, wscratch, po, qo),
                ff1b_b = cn_read_plain(recs, bytes, "L{l}.ff1.linear2.bias", st.blob, po),
                nat_w = cn_read_plain(recs, bytes, "L{l}.norm_att.weight", st.blob, po),
                nat_b = cn_read_plain(recs, bytes, "L{l}.norm_att.bias", st.blob, po),
                q_w = cn_read_gemm(recs, bytes, "L{l}.attn.q.weight", st, wscratch, po, qo),
                q_b = cn_read_plain(recs, bytes, "L{l}.attn.q.bias", st.blob, po),
                k_w = cn_read_gemm(recs, bytes, "L{l}.attn.k.weight", st, wscratch, po, qo),
                k_b = cn_read_plain(recs, bytes, "L{l}.attn.k.bias", st.blob, po),
                v_w = cn_read_gemm(recs, bytes, "L{l}.attn.v.weight", st, wscratch, po, qo),
                v_b = cn_read_plain(recs, bytes, "L{l}.attn.v.bias", st.blob, po),
                o_w = cn_read_gemm(recs, bytes, "L{l}.attn.out.weight", st, wscratch, po, qo),
                o_b = cn_read_plain(recs, bytes, "L{l}.attn.out.bias", st.blob, po),
                pos_w = cn_read_gemm(recs, bytes, "L{l}.attn.pos.weight", st, wscratch, po, qo),
                bias_u = cn_read_plain(recs, bytes, "L{l}.attn.pos_bias_u", st.blob, po),
                bias_v = cn_read_plain(recs, bytes, "L{l}.attn.pos_bias_v", st.blob, po),
                ncv_w = cn_read_plain(recs, bytes, "L{l}.norm_conv.weight", st.blob, po),
                ncv_b = cn_read_plain(recs, bytes, "L{l}.norm_conv.bias", st.blob, po),
                pw1_w = cn_read_gemm(recs, bytes, "L{l}.conv.pw1.weight", st, wscratch, po, qo),
                pw1_b = cn_read_plain(recs, bytes, "L{l}.conv.pw1.bias", st.blob, po),
                dw_w = cn_read_plain(recs, bytes, "L{l}.conv.dw.weight", st.blob, po),
                dw_b = cn_read_plain(recs, bytes, "L{l}.conv.dw.bias", st.blob, po),
                bn_w = cn_read_plain(recs, bytes, "L{l}.conv.bn.weight", st.blob, po),
                bn_b = cn_read_plain(recs, bytes, "L{l}.conv.bn.bias", st.blob, po),
                bn_mean = cn_read_plain(recs, bytes, "L{l}.conv.bn.running_mean", st.blob, po),
                bn_var = cn_read_plain(recs, bytes, "L{l}.conv.bn.running_var", st.blob, po),
                pw2_w = cn_read_gemm(recs, bytes, "L{l}.conv.pw2.weight", st, wscratch, po, qo),
                pw2_b = cn_read_plain(recs, bytes, "L{l}.conv.pw2.bias", st.blob, po),
                nf2_w = cn_read_plain(recs, bytes, "L{l}.norm_ff2.weight", st.blob, po),
                nf2_b = cn_read_plain(recs, bytes, "L{l}.norm_ff2.bias", st.blob, po),
                ff2a_w = cn_read_gemm(recs, bytes, "L{l}.ff2.linear1.weight", st, wscratch, po, qo),
                ff2a_b = cn_read_plain(recs, bytes, "L{l}.ff2.linear1.bias", st.blob, po),
                ff2b_w = cn_read_gemm(recs, bytes, "L{l}.ff2.linear2.weight", st, wscratch, po, qo),
                ff2b_b = cn_read_plain(recs, bytes, "L{l}.ff2.linear2.bias", st.blob, po),
                nout_w = cn_read_plain(recs, bytes, "L{l}.norm_out.weight", st.blob, po),
                nout_b = cn_read_plain(recs, bytes, "L{l}.norm_out.bias", st.blob, po))
            // fold BN (with the depthwise bias) into bn_w/bn_b at load
            cn_fold_bn(st.blob, lo.bn_w, lo.bn_b, lo.bn_mean, lo.bn_var, lo.dw_b, st.e.d_model, st.e.eps)
            st.layers[l] = lo
        }
        st.e.proj_w = cn_read_gemm(recs, bytes, "proj.weight", st, wscratch, po, qo)
        st.e.proj_b = cn_read_plain(recs, bytes, "proj.bias", st.blob, po)

        if (!empty(recs)) {
            let leftover <- [for (r in values(recs)); r.name]
            panic("dasLLAMA canary: {length(recs)} unconsumed tensor(s) in encoder bin - converter drift: {join(leftover, ", ")}")
        }
        delete recs
        delete wscratch
    }
    fclose(f)
    if (q8) {
        cn_repack_all(st)
    }
    st.twiddles <- build_dft_twiddles(st.e.n_fft)
    return <- st
}

// ===== mel (frame-major, parakeet flavor) =====

//! Canary's log-mel: preemphasis 0.97, zero center-pad n_fft/2, STFT window, power spectra, mel dot,
//! log(sum + 2⁻²⁴), then per-feature mean/std norm over valid frames (Bessel n−1, std + 1e-5);
//! frames past ``valid`` are zeroed (the seq-len mask). FRAME-major ``[n_len × n_mel]``.
[hot_path]
def canary_log_mel(m : CanaryEncoder; samples : array<float> | #; var st : CanaryState) {
    canary_window_frames(m, samples, st)
    canary_mel_spectrum(m, st)
}

//! the mel's CPU half: preemphasis, the reflection-free zero pad, the windowed frames into ``st.xw`` (``st.n_len``,
//! ``st.n_valid`` filled); the spectrum half follows - [[canary_mel_spectrum]] on the CPU, or the device front's head
def canary_window_frames(m : CanaryEncoder; samples : array<float> | #; var st : CanaryState) {
    let n_in = long_length(samples)
    st.mel_ready = false
    ensure_length(st.pre, n_in)
    for (i in range(int(n_in))) {
        st.pre[i] = samples[i]
    }
    var pi = n_in - 1l
    while (pi > 0l) {   // preemph backward in place: x[i] -= 0.97·x[i−1]
        st.pre[pi] -= CN_PREEMPH * st.pre[pi - 1l]
        pi--
    }
    let pad = m.n_fft / 2l
    let n_padded = n_in + 2l * pad
    ensure_length(st.padded, n_padded)
    for (i in range(int(pad))) {
        st.padded[i] = 0.0
    }
    for (i in range(int(n_in))) {
        st.padded[int(pad) + i] = st.pre[i]
    }
    for (i in range64(n_in + pad, n_padded)) {
        st.padded[i] = 0.0
    }

    let n_fft = m.n_fft
    let n_mel = m.n_mel
    st.n_len = (n_padded - n_fft) / CN_HOP + 1l
    let n_len = st.n_len
    ensure_length(st.mel, n_len * n_mel)
    let wsz = length(m.window)
    let wpad = (n_fft - wsz) / 2l

    // windowed frames -> DFT via twiddle gemm -> power
    ensure_length(st.xw, n_len * n_fft)
    for (i in range64(n_len * n_fft)) {
        st.xw[i] = 0.0
    }
    for (i in range64(n_len)) {
        let off = i * CN_HOP
        let n_do = min(wsz, n_padded - off)
        for (j in range64(n_do)) {
            if (off + wpad + j < n_padded) {
                st.xw[i * n_fft + wpad + j] = m.window[j] * st.padded[off + wpad + j]
            }
        }
    }
    st.n_valid = n_in / CN_HOP
}

//! the mel's spectrum half off ``st.xw``: DFT (twiddle gemm), power, the mel dot and log, the per-feature normalization
//! over the valid frames with the tail masked; frame-major ``st.mel`` [n_len x n_mel]
def canary_mel_spectrum(m : CanaryEncoder; var st : CanaryState) {
    mel_spectrum_rows(st.xw, plane_at(m.twiddles, 0l), plane_at(m.filters, 0l), st.n_len, m.n_fft, m.n_fb, m.n_mel,
                      MelForm(log = MelLog.ln_add, floor = CN_MEL_FLOOR, feature_norm = true, n_valid = st.n_valid),
                      st.reim, st.mel)
    st.mel_ready = true
}

// ===== conv subsampling =====

// Time-rows past the valid length are zeroed before each conv; here
// that lands as: zero rows [from_row, n_rows) of a [n_rows × row_stride] image.
def private cn_mask_rows(var a : array<float>; from_row, n_rows, row_stride : int64) {
    for (i in range64(from_row * row_stride, n_rows * row_stride)) {
        a[i] = 0.0
    }
}

// regular conv2d k3 s2 p1, cin=1: im2col (9 taps, guarded) then mm + bias + relu
def private cn_conv0(m : CanaryEncoder; var st : CanaryState; ih, iw : int64) {
    let cout = m.n_sub_ch
    let ow = conv_s2_out(iw)
    let oh = conv_s2_out(ih)
    let rows = oh * ow
    ensure_length(st.col, rows * 9l)
    unsafe {
        im2col_k3s2p1(addr(st.col[0]), addr<float const?>(st.mel[0]), iw, ih, 1l)
    }
    ensure_length(st.y0, rows * cout)
    mm_blob_b(st.y0, m.blob, m.c0_w, st.col, 9l, cout, rows)
    bias_relu_rows(st.y0, m.blob, m.c0_b, cout, rows)
}

// depthwise conv2d k3 s2 p1: per-channel 3x3, + bias, NO relu. w [ch × 9] (kh·3+kw).
def private cn_conv_dw(m : CanaryEncoder; var out : array<float>; x : array<float>;
                       w_off, b_off : int64; iw, ih : int64) {
    let ch = m.n_sub_ch
    let ow = conv_s2_out(iw)
    let oh = conv_s2_out(ih)
    ensure_length(out, oh * ow * ch)
    for (oy in range64(oh)) {
        for (ox in range64(ow)) {
            let obase = (oy * ow + ox) * ch
            for (c in range64(ch)) {
                var acc = m.blob[b_off + c]
                for (kh in range64(3l)) {
                    let iy = oy * 2l - 1l + kh
                    if (iy >= 0l && iy < ih) {
                        for (kw in range64(3l)) {
                            let ix = ox * 2l - 1l + kw
                            if (ix >= 0l && ix < iw) {
                                acc += m.blob[w_off + c * 9l + kh * 3l + kw] * x[(iy * iw + ix) * ch + c]
                            }
                        }
                    }
                }
                out[obase + c] = acc
            }
        }
    }
}

// pointwise 1x1 conv (channel mix) + bias + relu, over rows
def private cn_conv_pw(m : CanaryEncoder; var out : array<float>; x : array<float>;
                       var xq : array<int8>; var xs : array<float>; w_off, b_off, rows : int64) {
    let ch = m.n_sub_ch
    ensure_length(out, rows * ch)
    tower_mm_rq(m, out, w_off, x, xq, xs, ch, ch, rows)
    bias_relu_rows(out, m.blob, b_off, ch, rows)
}

//! the CPU subsample front: the stride-2 convs, the feature rows and the input projection into ``st.x``; returns the encoder rows
def private cn_front_cpu(m : CanaryEncoder; var st : CanaryState) : int64 {
    let d = m.d_model
    let nch = m.n_sub_ch
    let n_mel = m.n_mel
    // conv subsampling: mel [n_len × n_mel] as image [ih=n_len][iw=n_mel][c=1]
    let t1 = conv_s2_out(st.n_len)
    let w1 = conv_s2_out(n_mel)
    // valid time lengths propagate through the stride-2 convs (the length mask)
    let l0 = conv_s2_out(st.n_valid)
    let l2 = conv_s2_out(l0)
    cn_conv0(m, st, st.n_len, n_mel)                              // -> y0 [t1 × w1 × nch]
    cn_mask_rows(st.y0, l0, t1, w1 * nch)                         // mask before conv2 (depthwise)
    cn_conv_dw(m, st.y2, st.y0, m.c2_w, m.c2_b, w1, t1)          // -> y2 [t2 × w2 × nch]
    let t2 = conv_s2_out(t1)
    let w2 = conv_s2_out(w1)
    cn_conv_pw(m, st.y3, st.y2, st.xqi, st.xsi, m.c3_w, m.c3_b, t2 * w2)   // -> y3
    cn_mask_rows(st.y3, l2, t2, w2 * nch)                        // mask before conv5 (depthwise)
    cn_conv_dw(m, st.y5, st.y3, m.c5_w, m.c5_b, w2, t2)         // -> y5 [t3 × w3 × nch]
    let t3 = conv_s2_out(t2)
    let w3 = conv_s2_out(w2)
    cn_conv_pw(m, st.y6, st.y5, st.xqi, st.xsi, m.c6_w, m.c6_b, t3 * w3)   // -> y6

    let tt = t3
    let feat = w3 * nch
    ensure_length(st.xf, tt * feat)                                  // xf[t][f + w3·c] = y6[t][f][c]
    for (t in range64(tt)) {
        for (fr in range64(w3)) {
            for (c in range64(nch)) {
                st.xf[t * feat + fr + w3 * c] = st.y6[(t * w3 + fr) * nch + c]
            }
        }
    }
    ensure_length(st.x, tt * d)
    tower_mm_rq(m, st.x, m.pre_w, st.xf, st.xqi, st.xsi, feat, d, tt)
    add_bias_rows(st.x, m.blob, m.pre_b, d, tt)
    return tt
}

// ===== encoder =====

//! the Transformer-XL rel-position table [ww x d] into `st.pos_emb`, (sin, cos) interleaved, row w = tt - 1 - w; the q8
//! lane's requant beside it - the CPU chain's per-encode input, and the Metal chain's (the Vulkan chain builds its own on
//! the device)
def canary_pos_table(m : CanaryEncoder; var st : CanaryState; tt, ww : int64) {
    let d = m.d_model
    rel_pos_table(st.pos_emb, d, tt, ww)
    if (m.q8) {
        requant_rows_q8_sized(st.pos_emb, d, ww, st.posq, st.poss)
    }
}

//! the GPU block hook: after the subsample front + inproj, the 32 FastConformer blocks on st.x; the pos table is the hook's
//! own (`canary_pos_table` on the host, or the device's)
typedef CanaryGpuBlocksFn = function<(m : CanaryEncoder; var st : CanaryState; tt, ww : int64) : bool>
typedef CanaryServesFn = function<() : bool>
var private g_cn_gpu : CanaryGpuBlocksFn
var private g_cn_serves : CanaryServesFn
var private g_cn_gpu_set = false

//! the GPU front hook: off the windowed frames ``st.xw``, the mel spectrum, the subsample stack and the input projection
//! into ``st.x``; returns the encoder rows tt, or -1 declined (the CPU front runs whole)
typedef CanaryFrontGpuFn = function<(m : CanaryEncoder; var st : CanaryState) : int64>
var private g_cn_front : CanaryFrontGpuFn
var private g_cn_front_set = false

[arch(at="../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_canary_front_gpu(fn : CanaryFrontGpuFn) {
    g_cn_front = fn
    g_cn_front_set = true
}
var private g_cn_serves_set = false

[arch(at="../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_canary_gpu(fn : CanaryGpuBlocksFn; serves : CanaryServesFn) {
    g_cn_gpu = fn
    g_cn_serves = serves
    g_cn_gpu_set = true
    g_cn_serves_set = true
}

//! the lane policy's question - would a GPU driver serve the exact block loop: false with no driver, with the knob off, or under the Vulkan driver, which serves q8
def canary_gpu_serves() : bool => g_cn_serves_set && invoke(g_cn_serves)

var private g_cn_pin = GemmLane.unset

//! Pin the canary encoder's weight format for subsequent loads: q8 (the CPU serving lane) vs
//! the fp32 lane the parity legs and the GPU driver read. ``reset_canary_enc_q8`` returns to
//! the policy default (exact when the Metal driver serves).
def set_canary_enc_q8(on : bool) {
    g_cn_pin = lane_pin(on)
}

def reset_canary_enc_q8() {
    g_cn_pin = GemmLane.unset
}

//! The lane subsequent loads would take — the facade's default and the tests read it.
[arch(at="../ARCHITECTURE_MEDIA.md#tower-weight-lane")]
def canary_enc_serves_q8() : bool => lane_serves_q8(g_cn_pin, !canary_gpu_serves())

def private cn_blocks_cpu(m : CanaryEncoder; var st : CanaryState; tt, ww : int64) {
    let form = FcCpuForm(d = m.d_model, ff = m.n_ff, n_head = m.n_head, nk = m.conv_kernel, tap_major = false, vec4 = false)
    for (l in range64(m.n_layer)) {
        fc_block_cpu(m, st, form, FcRelPos(ww = ww, n_real = tt), m.layers[l], l, tt)
    }
}

//! FastConformer-32 encode over ``st.mel`` (frame-major) + the perception.proj projector.
//! Leaves the projected soft tokens (``proj_dim`` wide) in ``st.out``; returns the frame count.
[hot_path]
def canary_encode(m : CanaryEncoder; var st : CanaryState) : int64 {
    let d = m.d_model
    // the conformer block loop walks the thrice-subsampled frame count
    dispatch_phase_encode(conv_s2_out(conv_s2_out(conv_s2_out(st.n_len))))

    var tt = -1l
    var ts = ref_time_ticks()
    if (g_cn_front_set) {
        tt = invoke(g_cn_front, m, st)
        if (tt >= 0l) {
            asr_prof_add("cn.front.gpu", ts)
        }
    }
    if (tt < 0l) {
        cn_cpu_refuse(m, "front")
        if (!st.mel_ready) {
            canary_mel_spectrum(m, st)
            asr_prof_add("cn.mel", ts)
            ts = ref_time_ticks()
        }
        tt = cn_front_cpu(m, st)
        asr_prof_add("cn.front", ts)
    }

    let ww = 2l * tt - 1l
    var gpu_served = false
    ts = ref_time_ticks()
    if (g_cn_gpu_set) {
        gpu_served = invoke(g_cn_gpu, m, st, tt, ww)
    }
    if (!gpu_served) {
        cn_cpu_refuse(m, "block loop")
        canary_pos_table(m, st, tt, ww)
        cn_blocks_cpu(m, st, tt, ww)
    }
    asr_prof_add(gpu_served ? "cn.blocks.gpu" : "cn.blocks", ts)
    return tt if (m.dev_backend != TowerDev.none)   // the device projected the rows into `st.out` behind its blocks

    // perception.proj: encoder rows (d) -> decoder embedding (proj_dim)
    ts = ref_time_ticks()
    ensure_length(st.out, tt * m.proj_dim)
    tower_mm_rq(m, st.out, m.proj_w, st.x, st.xqi, st.xsi, d, m.proj_dim, tt)
    add_bias_rows(st.out, m.blob, m.proj_b, m.proj_dim, tt)
    asr_prof_add("cn.proj", ts)
    return tt
}

//! One canary transcription stream: the Qwen3 decoder session + the FastConformer encoder scratch.
struct CanaryAsrSession {
    qctx : Session = Session()
    cst : CanaryState = CanaryState()
}

//! Canary-Qwen's capability sheet: English, no timestamps, whole-clip.
def canary_asr_caps() : AsrCaps {
    var c = AsrCaps(translate = false, timestamps = AsrTimestamps.none,
                    streaming = false, prompt = false, default_lang = "en")
    c.languages <- ["en"]
    return <- c
}

//! The facade's session arm: English-only validation, then a fresh decoder session.
def create_canary_asr_session(dec : Model; lang : string; kv : KVDtype = KVDtype.f16) : CanaryAsrSession {
    if (lang != "auto" && lang != "en") {
        panic("dasLLAMA asr: this Canary-Qwen model is English-only - create the session with lang=\"en\" or \"auto\"")
    }
    return <- CanaryAsrSession(qctx <- create_session_(dec, kv, kv))
}

//! The transcription's prompt: FastConformer-32 encode + projector, the SALM/Qwen prompt with the audio soft tokens spliced
//! at the placeholder, prefilled on the decoder session from position 0 - its logits row the first reply token's; the
//! prompt's row count back
def canary_prefill(dec : Model; enc : CanaryEncoder; var s : CanaryAsrSession; samples : array<float> | #) : int64 {
    let tp = ref_time_ticks()
    canary_window_frames(enc, samples, s.cst)
    let n_audio = canary_encode(enc, s.cst)   // s.cst.out holds [n_audio × proj_dim] soft tokens
    asr_prof_add("cn.encode", tp)

    let dim = dec.config.dim
    let im_start = special_id(dec, "<|im_start|>")
    let im_end = special_id(dec, "<|im_end|>")   // = the text EOS (151645)
    // SALM "qwen" prompt: <|im_start|>user\nTranscribe the following: <AUDIO><|im_end|>\n<|im_start|>assistant\n
    var inscope head : array<int64>
    head |> push(im_start)
    head |> push_from(encode_(dec, "user\n", false, false))
    head |> push_from(encode_(dec, "Transcribe the following: ", false, false))
    var inscope tail : array<int64>
    tail |> push(im_end)
    tail |> push_from(encode_(dec, "\n", false, false))
    tail |> push(im_start)
    tail |> push_from(encode_(dec, "assistant\n", false, false))

    let n_head = long_length(head)
    let npos = n_head + n_audio + long_length(tail)
    asr_ctx_guard(n_audio, npos, dec.config.seq_len)
    var inscope embd : array<float>
    media_body_rows_(dec, s.qctx, head, s.cst.out, n_audio, dim, tail, embd)

    let tpf = ref_time_ticks()
    s.qctx.n_past = 0l   // fresh sequence per transcribe
    eval_embd_body_(dec, s.qctx, embd, npos, 0l, 0l, int2(0))
    asr_prof_add("cn.prefill", tpf)
    return npos
}

// Canary-Qwen transcription: the prompt's prefill, then greedy decode over the Qwen3-1.7B decoder until
// the text EOS (151645, which is included in the emitted ids by convention).
def canary_transcribe(dec : Model; enc : CanaryEncoder; var s : CanaryAsrSession;
                      samples : array<float> | #; blk : block<(seg : TranscribeSegment)>;
                      var detected_lang : string&) : int {
    let npos = canary_prefill(dec, enc, s, samples)
    let im_end = special_id(dec, "<|im_end|>")
    let max_new = min(dec.config.seq_len - npos - 1l, CN_MAX_NEW)
    var inscope one : array<int64>
    one |> resize(1)
    var inscope toks : array<int>
    var inscope raw : array<string>
    toks |> reserve(128)   // typical transcription length; grows if a clip runs longer
    raw |> reserve(128)
    var plog_sum = 0.0lf
    var hit_eos = false
    let tdec = ref_time_ticks()
    for (_i in range64(max_new)) {
        let best = int(parallel_argmax(s.qctx.logits, long_length(s.qctx.logits)))
        let mx = s.qctx.logits[best]
        plog_sum += double(s.qctx.logits[best]) - (log(hlse(s.qctx.logits, mx)) + double(mx))
        toks |> push(best)
        if (int64(best) == im_end) {   // EOS: emitted into the ids, then stop
            hit_eos = true
            break
        }
        raw |> push(piece_(dec, int64(best)))
        one[0] = int64(best)
        eval_(dec, s.qctx, one)
    }
    asr_prof_add("cn.decode", tdec)
    if (!hit_eos) {
        // a budget exit truncates SILENTLY otherwise — the transcript and its timing would
        // publish indistinguishable from a complete decode
        to_log(LOG_WARNING, "dasLLAMA canary: decode budget exhausted at {max_new} tokens - transcript is TRUNCATED\n")
    }

    var inscope seg = TranscribeSegment(t0 = 0l, t1 = long_length(samples) * 100l / AUDIO_SAMPLE_RATE,
        text = strip(join(raw, "")))
    seg.tokens := toks
    if (!empty(toks)) {
        seg.avg_logprob = float(plog_sum / double(length(toks)))
    }
    detected_lang = "en"
    blk |> invoke(seg)
    return 1
}
