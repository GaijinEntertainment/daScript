options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_gemma3v shared public

require dasllama/dasllama_lint public
require daslib/fio
require strings
require daslib/apply
require daslib/archive
require dasllama/dasllama_gguf
require dasllama/dasllama_math   // matmul_q8q8_batch + the backend repack — the q8 block-GEMM lane
require dasllama/dasllama_convert   // quantize_q8_0_into — the q8 lane's read-time transcode
require dasllama/dasllama_common   // apply_box_profile_runtime for the tower-image identity
require dasllama/dasllama_image   // the dlim rail: mapped planes, no allocation or work at load
require dasllama/dasllama_plane public   // PlaneF/PlaneI8 — re-exported so requirers can delete a tower
require dasllama/dasllama_tower   // the shared encoder-tower pieces: layernorm, biases, gelu, attention, pool
require dasllama/dasllama_vision   // encode_image_: letterbox + normalize ahead of the tower
require dasllama/dasllama_env     // g_env_vision: the dump knob the image path reads
require math

//! The gemma3 vision tower (SigLIP-400M; gemma-3-4b is the tested pair): 27-block pre-norm
//! LayerNorm ViT over 14 px patches on a FIXED 896² canvas — x·2−1 → patch conv + bias → learned
//! position add → blocks (biased GEMMs, scaled bidirectional attention, GELU-tanh) → post-LN →
//! 4×4 average pool → weighted RMS → projection. Oracle: llama-mtmd-debug -p encode on the
//! f32-widened mmproj, CPU, -fa off (gemma3-vision-oracle/mint_gemma3.sh).

let private GEMMA3V_MERGE_DEFAULT = 4l
let private GEMMA3V_STAGE_CAP = 1073741824ul   // the staged-mint line: a carrier source at or past 1 GiB streams instead

// per-block GEMM order — the block GEMM plane stride follows it
let GEMMA3V_GEMM_Q = 0l
let GEMMA3V_GEMM_K = 1l
let GEMMA3V_GEMM_V = 2l
let GEMMA3V_GEMM_O = 3l
let GEMMA3V_GEMM_UP = 4l
let GEMMA3V_GEMM_DOWN = 5l
let GEMMA3V_GEMMS = 6l

struct Gemma3vTower {
    n_embd : int64       // 1152 — the tower width
    n_layer : int64      // 27
    n_head : int64       // 16 (head width 72)
    n_ff : int64         // 4304
    proj_dim : int64     // mm.input_projection output width — the soft-token width (decoder width)
    patch : int64        // 14 px
    patch_dim : int64    // patch² × 3 — the im2col row (588)
    merge : int64        // pooling kernel (4): one soft token per merge×merge patches
    image_px : int64     // the FIXED canvas edge (896) — the learned position table covers exactly this grid
    eps : float          // meta layer_norm_epsilon — every norm in the tower
    // q8: the 162 block GEMMs SERVE as Q8_0 planes (read-time transcode) and layer_w indexes
    // qblob/qscales; everything else is f32 in blob — the file's f16 widens exactly at read
    q8 : bool
    blob : PlaneF = PlaneF()
    qblob : PlaneI8 = PlaneI8()
    qscales : PlaneF = PlaneF()
    patch_w : int64      // [n_embd × patch_dim]
    patch_b : int64      // [n_embd]
    pos_w : int64        // [(image_px/patch)² × n_embd] — the learned table, one row per patch
    post_ln_w : int64; post_ln_b : int64   // [n_embd] each
    soft_norm_w : int64  // [n_embd] — the projector's RMS weight
    proj_w : int64       // [proj_dim × n_embd] (the file stores it transposed; the stage read untransposes)
    layer_f : int64      // per-block f32 region base; stride layer_f_stride: ln1 w/b, ln2 w/b, q/k/v/o bias [d], up bias [ff], down bias [d]
    layer_f_stride : int64
    layer_w : int64      // per-block GEMM region base (qblob when q8, else blob); stride layer_w_stride
    layer_w_stride : int64
    // q8 lane only: the ffn_down GEMM stays f32 in blob — its input width is n_ff (4304), not a
    // multiple of the 32-element Q8_0 block, so its rows cannot quantize without straddling
    layer_wd : int64
    layer_wd_stride : int64
    image_map : void?         // non-null when the planes borrow a mapped .dlim (dasllama_image)
    image_bytes : uint64
}

//! Per-encode scratch, caller-owned like the audio EncoderState; the canvas is fixed so every
//! buffer sizes once on the first encode.
struct Gemma3vState {
    @exact_size x0 : array<float>    // [npos × patch_dim] im2col rows (scaled to [-1, 1])
    @exact_size x : array<float>     // residual stream [npos × d]
    @exact_size xb : array<float>    // norm out / GEMM out [npos × d]
    @exact_size xb2 : array<float>   // attention out [npos × d]
    @exact_size qq : array<float>
    @exact_size kk : array<float>
    @exact_size vv : array<float>
    @exact_size hp : array<float>    // attention per-head packs
    @scratch @exact_size att : array<float>   // attention score blocks
    @exact_size h : array<float>     // ffn hidden [npos × ff]
    @exact_size pooled : array<float>  // [nout × d]
    @scratch @exact_size xqi : array<int8>    // q8 lane: the requantized GEMM input, grown per site
    @scratch @exact_size xsi : array<float>   // its per-32-block scales
}

// the tower owns exactly one thing — its image backing. Every plane borrows.
def finalize(var t : Gemma3vTower) {
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

def finalize(var s : Gemma3vState) {
    delete s.x0
    delete s.x
    delete s.xb
    delete s.xb2
    delete s.qq
    delete s.kk
    delete s.vv
    delete s.hp
    delete s.att
    delete s.h
    delete s.pooled
    delete s.xqi
    delete s.xsi
}

// the tripwire: serialize_image_meta(Gemma3vTower) covers 24 fields; image_map + image_bytes
// are deliberate skips (the three planes are sections, not meta).
let private GEMMA3V_META_FIELDS = 24 + 2

// every meta-carried Gemma3vTower field, explicitly, both directions (see dasllama_image)
def private serialize_image_meta(var arch : Archive; var t : Gemma3vTower) {
    verify(count_meta_fields(t) == GEMMA3V_META_FIELDS)   // grew Gemma3vTower? extend this list
    arch |> serialize_raw(t.n_embd)
    arch |> serialize_raw(t.n_layer)
    arch |> serialize_raw(t.n_head)
    arch |> serialize_raw(t.n_ff)
    arch |> serialize_raw(t.proj_dim)
    arch |> serialize_raw(t.patch)
    arch |> serialize_raw(t.patch_dim)
    arch |> serialize_raw(t.merge)
    arch |> serialize_raw(t.image_px)
    arch |> serialize_raw(t.eps)
    arch |> serialize_raw(t.q8)
    arch |> serialize_raw(t.patch_w)
    arch |> serialize_raw(t.patch_b)
    arch |> serialize_raw(t.pos_w)
    arch |> serialize_raw(t.post_ln_w)
    arch |> serialize_raw(t.post_ln_b)
    arch |> serialize_raw(t.soft_norm_w)
    arch |> serialize_raw(t.proj_w)
    arch |> serialize_raw(t.layer_f)
    arch |> serialize_raw(t.layer_f_stride)
    arch |> serialize_raw(t.layer_w)
    arch |> serialize_raw(t.layer_w_stride)
    arch |> serialize_raw(t.layer_wd)
    arch |> serialize_raw(t.layer_wd_stride)
}

[unused_argument(t)]
def private image_post_load(var t : Gemma3vTower) {}   // no derived state — offsets ARE the meta

//! The mint-side carrier: `t` holds every scalar the meta blob carries, the arrays beside it hold
//! the plane bytes until the writer has them (section names must match the served plane fields —
//! bind-by-name). Public so a suite can check a served tower against a non-image-backed read.
struct Gemma3vStaging {
    t : Gemma3vTower = Gemma3vTower()
    blob : array<float>
    qblob : array<int8>       // q8 lane: the block GEMMs as Q8_0 (repacked for the active backend)
    qscales : array<float>
}

def private serialize_image_meta(var arch : Archive; var st : Gemma3vStaging) {
    serialize_image_meta(arch, st.t)
}

// staging owns its plane bytes outright — every array here is heap, none is borrowed
def finalize(var st : Gemma3vStaging) {
    delete st.blob
    delete st.qblob
    delete st.qscales
    delete st.t
}

[unused_argument(st)]
def private image_post_load(var st : Gemma3vStaging) {}

// two lanes: "gemma3v-q8" serves the block GEMMs as Q8_0 (the CPU serving default); "gemma3v"
// keeps every plane f32 as the file dequants (the parity rail)
let private GEMMA3V_TAG = "gemma3v"
let private GEMMA3V_TAG_Q8 = "gemma3v-q8"

[init]
def private register_gemma3v_image_tags {
    register_image_family_tag(GEMMA3V_TAG)
    register_image_family_tag(GEMMA3V_TAG_Q8)
}

// The lane policy follows the fastest GEMM path on the box: the armed accelerate float-batch tier
// wants float planes and flips the default to the exact lane; else q8. A pin overrides. (No Metal
// tower driver serves this family yet — when one does, its serves() joins this policy.)
enum private G3vLane {
    unset    // the policy decides
    exact    // f32 planes
    q8
}
var private g_g3v_q8_pin = G3vLane.unset

def private g3v_serve_q8() : bool {
    if (g_g3v_q8_pin != G3vLane.unset) {
        return g_g3v_q8_pin == G3vLane.q8
    }
    return !float_batch_override_active()
}

//! Pin the gemma3v tower's block-GEMM format for subsequent loads: q8 (the CPU serving default)
//! vs f32 planes (the parity rail). Each lane is its own prepared image. ``reset_gemma3v_q8``
//! returns to the policy default.
def set_gemma3v_q8(on : bool) {
    g_g3v_q8_pin = on ? G3vLane.q8 : G3vLane.exact
}

def reset_gemma3v_q8() {
    g_g3v_q8_pin = G3vLane.unset
}

//! Would the next load serve the block GEMMs as q8 — the pin when set, the policy otherwise.
def gemma3v_serves_q8() : bool => g3v_serve_q8()

def private g3v_tag() : string => g3v_serve_q8() ? GEMMA3V_TAG_Q8 : GEMMA3V_TAG

// the lane changes what a load mints and serves, so the load says which and why, by knob spelling
def private g3v_announce_lane() {
    var why = "the CPU default (no accelerate tier)"
    if (g_g3v_q8_pin != G3vLane.unset) {
        why = "pinned"
    } elif (float_batch_override_active()) {
        why = "DASLLAMA_ACCEL on: the float-batch tier serves f32 planes"
    }
    to_log(LOG_INFO, "dasLLAMA gemma3v: block GEMM lane {g3v_serve_q8() ? "q8" : "f32 planes"} - {why}\n")
}

// the block GEMM tensors in GEMMA3V_GEMM_* order (q k v o up down). The shipped mmproj carries
// ffn up/down SWAPPED (the historic converter defect clip.cpp detects by ne[0] and swaps back);
// the stage read resolves the swap once and these lists hold the RESOLVED source names.
def private g3v_gemm_names(swapped : bool) : array<string> {
    return <- [ "attn_q", "attn_k", "attn_v", "attn_out",
                swapped ? "ffn_down" : "ffn_up", swapped ? "ffn_up" : "ffn_down" ]
}

// the block GEMM element counts in GEMMA3V_GEMM_* order
def private g3v_gemm_sizes(d, ff : int64) : array<int64> {
    return <- [ d * d, d * d, d * d, d * d, d * ff, ff * d ]
}

// every tensor the forward reads, by name and element count, before any layout is committed
def private g3v_verify_tensors(m : GGUFMeta; t : Gemma3vTower; names : array<string>; gemm_sizes : array<int64>) {
    let d = t.n_embd
    let ff = t.n_ff
    let n_side = t.image_px / t.patch
    gguf_expect_elems(m, "v.patch_embd.weight", t.patch_dim * d, "dasLLAMA gemma3v")
    gguf_expect_elems(m, "v.patch_embd.bias", d, "dasLLAMA gemma3v")
    gguf_expect_elems(m, "v.position_embd.weight", n_side * n_side * d, "dasLLAMA gemma3v")
    gguf_expect_elems(m, "v.post_ln.weight", d, "dasLLAMA gemma3v")
    gguf_expect_elems(m, "v.post_ln.bias", d, "dasLLAMA gemma3v")
    gguf_expect_elems(m, "mm.soft_emb_norm.weight", d, "dasLLAMA gemma3v")
    gguf_expect_elems(m, "mm.input_projection.weight", d * t.proj_dim, "dasLLAMA gemma3v")
    let bias_sizes <- [ d, d, d, d, ff, d ]
    for (l in range64(t.n_layer)) {
        let p = "v.blk.{l}"
        gguf_expect_elems(m, "{p}.ln1.weight", d, "dasLLAMA gemma3v")
        gguf_expect_elems(m, "{p}.ln1.bias", d, "dasLLAMA gemma3v")
        gguf_expect_elems(m, "{p}.ln2.weight", d, "dasLLAMA gemma3v")
        gguf_expect_elems(m, "{p}.ln2.bias", d, "dasLLAMA gemma3v")
        for (g in range64(GEMMA3V_GEMMS)) {
            gguf_expect_elems(m, "{p}.{names[g]}.weight", gemm_sizes[g], "dasLLAMA gemma3v")
            gguf_expect_elems(m, "{p}.{names[g]}.bias", bias_sizes[g], "dasLLAMA gemma3v")
        }
    }
}

// the plane layout: fp32 cursor `off` (blob), q8 cursor `qo` (qblob; scales at qo/32)
def private g3v_layout(var t : Gemma3vTower; var off : int64&; var qo : int64&) {
    let d = t.n_embd
    let ff = t.n_ff
    let n_side = t.image_px / t.patch
    t.patch_w = off; off += t.patch_dim * d
    t.patch_b = off; off += d
    t.pos_w = off; off += n_side * n_side * d
    t.post_ln_w = off; off += d
    t.post_ln_b = off; off += d
    t.soft_norm_w = off; off += d
    t.proj_w = off; off += d * t.proj_dim
    t.layer_f_stride = 9l * d + ff   // ln1 w/b, ln2 w/b, q/k/v/o bias, down bias [d each]; up bias [ff]
    t.layer_f = off; off += t.n_layer * t.layer_f_stride
    if (t.q8) {
        // q/k/v/o/up quantize (input width d, 32-divisible); down stays f32 (input width ff is not)
        t.layer_w_stride = 4l * d * d + d * ff
        t.layer_w = qo
        qo += t.n_layer * t.layer_w_stride
        t.layer_wd_stride = ff * d
        t.layer_wd = off
        off += t.n_layer * t.layer_wd_stride
    } else {
        t.layer_w_stride = 4l * d * d + 2l * d * ff
        t.layer_w = off
        off += t.n_layer * t.layer_w_stride
        t.layer_wd = 0l
        t.layer_wd_stride = 0l
    }
}

// q8 GEMM read: the file's f16 tensor widens to f32 scratch, then quantizes to Q8_0 straight
// into qblob/qscales at the 32-aligned region (wo/32 indexes the scales)
def private g3v_read_gemm_q8(m : GGUFMeta; b : array<uint8> | #; name : string; var st : Gemma3vStaging;
                             @scratch var wscratch : array<float>; wo, n_elem : int64) {
    if (n_elem % 32l != 0l || wo % 32l != 0l) {
        panic("dasLLAMA gemma3v q8: GEMM tensor '{name}' ({n_elem} elements at {wo}) is not 32-aligned")
    }
    if (long_length(wscratch) < n_elem) {
        wscratch |> reserve(n_elem)
        wscratch |> resize(n_elem)
    }
    gguf_read_tensor_f32(m, b, name, wscratch, 0l, n_elem)
    quantize_q8_0_into(wscratch, n_elem, st.qblob, st.qscales, wo, wo / 32l)
}

// the reads, in layout order — the per-block f32 region is ln1 w/b ln2 w/b, then the six GEMM
// biases in GEMMA3V_GEMM_* order; the GEMM region follows the same order
def private g3v_read_planes(m : GGUFMeta; bytes : array<uint8> | #; var st : Gemma3vStaging;
                            names : array<string>; gemm_sizes : array<int64>) {
    let d = st.t.n_embd
    let ff = st.t.n_ff
    let n_side = st.t.image_px / st.t.patch
    var wscratch : array<float>   // the q8 transcode's f32 staging, one GEMM at a time
    gguf_read_tensor_f32(m, bytes, "v.patch_embd.weight", st.blob, st.t.patch_w, st.t.patch_dim * d)
    gguf_read_tensor_f32(m, bytes, "v.patch_embd.bias", st.blob, st.t.patch_b, d)
    gguf_read_tensor_f32(m, bytes, "v.position_embd.weight", st.blob, st.t.pos_w, n_side * n_side * d)
    gguf_read_tensor_f32(m, bytes, "v.post_ln.weight", st.blob, st.t.post_ln_w, d)
    gguf_read_tensor_f32(m, bytes, "v.post_ln.bias", st.blob, st.t.post_ln_b, d)
    gguf_read_tensor_f32(m, bytes, "mm.soft_emb_norm.weight", st.blob, st.t.soft_norm_w, d)
    g3v_read_projection(m, bytes, st)
    let bias_sizes <- [ d, d, d, d, ff, d ]
    for (l in range64(st.t.n_layer)) {
        let p = "v.blk.{l}"
        var fo = st.t.layer_f + l * st.t.layer_f_stride
        gguf_read_tensor_f32(m, bytes, "{p}.ln1.weight", st.blob, fo, d); fo += d
        gguf_read_tensor_f32(m, bytes, "{p}.ln1.bias", st.blob, fo, d); fo += d
        gguf_read_tensor_f32(m, bytes, "{p}.ln2.weight", st.blob, fo, d); fo += d
        gguf_read_tensor_f32(m, bytes, "{p}.ln2.bias", st.blob, fo, d); fo += d
        var wo = st.t.layer_w + l * st.t.layer_w_stride
        for (g in range64(GEMMA3V_GEMMS)) {
            if (st.t.q8 && g == GEMMA3V_GEMM_DOWN) {
                gguf_read_tensor_f32(m, bytes, "{p}.{names[g]}.weight", st.blob,
                    st.t.layer_wd + l * st.t.layer_wd_stride, gemm_sizes[g])
            } elif (st.t.q8) {
                g3v_read_gemm_q8(m, bytes, "{p}.{names[g]}.weight", st, wscratch, wo, gemm_sizes[g])
                wo += gemm_sizes[g]
            } else {
                gguf_read_tensor_f32(m, bytes, "{p}.{names[g]}.weight", st.blob, wo, gemm_sizes[g])
                wo += gemm_sizes[g]
            }
            gguf_read_tensor_f32(m, bytes, "{p}.{names[g]}.bias", st.blob, fo, bias_sizes[g])
            fo += bias_sizes[g]
        }
    }
}

// mm.input_projection is stored [d rows × proj_dim] (mtmd transposes it in-graph); the batch
// matmul wants [proj_dim rows × d], so the stage read untransposes once
def private g3v_read_projection(m : GGUFMeta; bytes : array<uint8> | #; var st : Gemma3vStaging) {
    let d = st.t.n_embd
    let pd = st.t.proj_dim
    var pscratch : array<float>
    pscratch |> reserve_resize(d * pd)
    gguf_read_tensor_f32(m, bytes, "mm.input_projection.weight", pscratch, 0l, d * pd)
    unsafe {
        var dst = addr(st.blob[st.t.proj_w])
        let src = addr(pscratch[0])
        for (r in range64(pd)) {
            for (c in range64(d)) {
                dst[r * d + c] = src[c * pd + r]
            }
        }
    }
    delete pscratch
}

// repack every block GEMM region for the active backend's q8q8 kernels — per-row alignment (the
// region's input width) keeps blocks from straddling rows
def private g3v_repack_all(var st : Gemma3vStaging; gemm_sizes : array<int64>) {
    if (!st.t.q8 || !select_matmul_backend_for_load_()) {
        return
    }
    let d = st.t.n_embd
    for (l in range64(st.t.n_layer)) {
        var wo = st.t.layer_w + l * st.t.layer_w_stride
        for (g in range64(GEMMA3V_GEMMS - 1l)) {   // q k v o up: every quantized GEMM's input width is d
            repack_q8q8_weight(st.qblob, st.qscales, wo, d, gemm_sizes[g] / d)
            wo += gemm_sizes[g]
        }
    }
}

//! Read the tower off an mmproj gguf into owned staging arrays — the mint side of the image rail,
//! the only path that touches the source file. Every plane lands f32 in `blob` (the file's f16
//! widens exactly); the q8 lane transcodes the block GEMMs to Q8_0 at read.
def stage_gemma3v_tower(path : string) : Gemma3vStaging {
    var st = Gemma3vStaging()
    st.t.q8 = g3v_serve_q8()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open mmproj gguf '{path}'")
    }
    if (stat(path).size >= GEMMA3V_STAGE_CAP) {
        panic("dasLLAMA gemma3v: '{path}' is {stat(path).size / 1048576ul} MB - at or past the 1 GiB staged-mint line; serve the f16 mmproj (an f32-widened twin is an oracle artifact, not a serving file)")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        let proj = (gguf_has(m, "clip.vision.projector_type") ? gguf_str(m, bytes, "clip.vision.projector_type")
            : gguf_str(m, bytes, "clip.projector_type"))
        if (proj != "gemma3") {
            panic("dasLLAMA: mmproj projector '{proj}' is not gemma3 (the gemma-4 families are dasllama_gemma4uv / dasllama_gemma4v)")
        }
        st.t.merge = GEMMA3V_MERGE_DEFAULT
        if (gguf_has(m, "clip.vision.projector.scale_factor")) {
            st.t.merge = gguf_int(m, bytes, "clip.vision.projector.scale_factor")
        }
        st.t.patch = gguf_int(m, bytes, "clip.vision.patch_size")
        st.t.patch_dim = st.t.patch * st.t.patch * 3l
        st.t.image_px = gguf_int(m, bytes, "clip.vision.image_size")
        st.t.n_embd = gguf_int(m, bytes, "clip.vision.embedding_length")
        st.t.n_layer = gguf_int(m, bytes, "clip.vision.block_count")
        st.t.n_head = gguf_int(m, bytes, "clip.vision.attention.head_count")
        st.t.n_ff = gguf_int(m, bytes, "clip.vision.feed_forward_length")
        st.t.proj_dim = gguf_int(m, bytes, "clip.vision.projection_dim")
        st.t.eps = gguf_f32(m, bytes, "clip.vision.attention.layer_norm_epsilon")
        let d = st.t.n_embd
        if (d <= 0l || st.t.n_head <= 0l || d % st.t.n_head != 0l) {
            panic("dasLLAMA gemma3v: embedding_length {d} / head_count {st.t.n_head} - the head width must divide")
        }
        if (st.t.patch <= 0l || st.t.merge <= 0l || st.t.image_px % (st.t.patch * st.t.merge) != 0l) {
            panic("dasLLAMA gemma3v: image_size {st.t.image_px} is not a multiple of patch {st.t.patch} x merge {st.t.merge}")
        }
        // the historic converter swap: the file's ffn_down maps d->ff when swapped (ne[0] == d)
        let dn = gguf_find_tensor(m, "v.blk.0.ffn_down.weight")
        if (dn < 0) {
            panic("dasLLAMA gemma3v: mmproj is missing tensor 'v.blk.0.ffn_down.weight'")
        }
        let swapped = !empty(m.tensors[dn].dims) && m.tensors[dn].dims[0] == d
        let names <- g3v_gemm_names(swapped)
        let gemm_sizes <- g3v_gemm_sizes(d, st.t.n_ff)
        g3v_verify_tensors(m, st.t, names, gemm_sizes)
        var off = 0l
        var qo = 0l
        g3v_layout(st.t, off, qo)
        st.blob |> reserve_resize(off)
        st.qblob |> reserve_resize(qo)
        st.qscales |> reserve_resize(qo / 32l)
        g3v_read_planes(m, bytes, st, names, gemm_sizes)
        g3v_repack_all(st, gemm_sizes)
    }
    fclose(f)
    return <- st
}

//! Load the gemma3v tower, image-backed: a prepared `.dlim` maps and every plane borrows it, so
//! there is no allocation and no processing at load; missing or stale, one is minted from the
//! source mmproj first. Panics on any other projector.
def load_gemma3v_tower(path : string) : Gemma3vTower {
    var tag = g3v_tag()
    // a .dlim named directly: no mmproj to regenerate from, so a wrong identity panics; the lane is
    // the file's (its baked tag), not the pin's - a served tower reads its q8 flag from the meta
    if (path |> ends_with(".dlim")) {
        let baked = image_family_tag(path)
        if (baked == GEMMA3V_TAG || baked == GEMMA3V_TAG_Q8) {
            tag = baked
        }
        apply_box_profile_runtime()
        let ts_img = ref_time_ticks()
        var t = Gemma3vTower()
        if (!load_image(path, t, tag)) {
            panic("dasLLAMA: '{path}' is not a prepared gemma3v-tower image for this box/knobs (identity {image_identity(tag)}) - regenerate it from the source mmproj gguf")
        }
        to_log(LOG_INFO, "dasLLAMA: prepared gemma3v-tower image mapped in {get_time_usec(ts_img) / 1000} ms - {path}\n")
        g3v_check_planes(t, path)
        return <- t
    }
    g3v_announce_lane()
    if (g_env_engine.image) {
        apply_box_profile_runtime()   // a backend pin must precede the identity (it names the backend)
        let img = image_path_for(path, tag)
        let ts_img = ref_time_ticks()
        var t = Gemma3vTower()
        if (load_image(img, t, tag)) {
            to_log(LOG_INFO, "dasLLAMA: prepared gemma3v-tower image mapped in {get_time_usec(ts_img) / 1000} ms - {img}\n")
            g3v_check_planes(t, img)
            return <- t
        }
        delete t
        var inscope st <- stage_gemma3v_tower(path)
        var built = Gemma3vTower()
        if (cache_via_image_staged(st, built, img, tag)) {
            dlim_gc_stale(path, tag)   // the fresh bake ON DISK proves its lane's siblings dead
        }
        g3v_check_planes(built, img)
        return <- built
    }
    // no persisted image: mint one in memory and serve THAT — a served tower is always image-backed
    var inscope st <- stage_gemma3v_tower(path)
    var built = Gemma3vTower()
    mint_gemma3v_tower(st, built)
    g3v_check_planes(built, path)
    return <- built
}

//! Mint a served tower from a staged read WITHOUT touching disk — the off-rail control for suites.
def mint_gemma3v_tower(var st : Gemma3vStaging; var out : Gemma3vTower) {
    cache_via_image_staged(st, out, "", st.t.q8 ? GEMMA3V_TAG_Q8 : GEMMA3V_TAG)
}

// the bind walk skips a missing section silently, so the flag=>plane invariant must be checked
// once the carrier is live, or the kernel derefs null
def private g3v_check_planes(t : Gemma3vTower; src : string) {
    if (t.q8 && (empty(t.qblob) || empty(t.qscales))) {
        panic("dasLLAMA gemma3v: '{src}' declares q8 block GEMMs but carries no qblob/qscales sections - the image is corrupt or hand-edited")
    }
}

// one biased GEMM: y = W · x + b; `q8site` routes through the Q8_0 planes (the down GEMM never
// does — its rows are not 32-divisible, so it stays f32 on both lanes)
def private g3v_gemm_biased(var y : array<float>; t : Gemma3vTower; var s : Gemma3vState; woff, boff : int64;
                            x : array<float>; n, d, npos : int64; q8site : bool) {
    if (q8site) {
        requant_rows_q8_sized(x, n, npos, s.xqi, s.xsi)
        matmul_q8q8_batch(y, t.qblob, t.qscales, woff, s.xqi, s.xsi, n, d, npos)
    } else {
        mm_blob_b(y, t.blob, woff, x, n, d, npos)
    }
    add_bias_rows(y, t.blob, boff, d, npos)
}

//! One block's offsets (elements): the f32 rows in ``blob``, the six GEMM regions in the block
//! GEMM plane (``qblob`` when q8, else ``blob``), in GEMMA3V_GEMM_* order.
struct Gemma3vBlockOffs {
    ln1_w : int64; ln1_b : int64
    ln2_w : int64; ln2_b : int64
    w : int64[6]
    b : int64[6]
}

def gemma3v_block_offs(t : Gemma3vTower; l : int64) : Gemma3vBlockOffs {
    let d = t.n_embd
    let ff = t.n_ff
    let fo = t.layer_f + l * t.layer_f_stride
    var o = Gemma3vBlockOffs(ln1_w = fo, ln1_b = fo + d, ln2_w = fo + 2l * d, ln2_b = fo + 3l * d)
    let bias_sizes <- [ d, d, d, d, ff, d ]
    let sizes <- g3v_gemm_sizes(d, ff)
    var bo = fo + 4l * d
    var at = t.layer_w + l * t.layer_w_stride
    for (g in range64(GEMMA3V_GEMMS)) {
        if (t.q8 && g == GEMMA3V_GEMM_DOWN) {
            o.w[g] = t.layer_wd + l * t.layer_wd_stride
        } else {
            o.w[g] = at
            at += sizes[g]
        }
        o.b[g] = bo
        bo += bias_sizes[g]
    }
    return o
}

// one pre-norm block over the residual stream s.x (npos rows)
def private g3v_block(t : Gemma3vTower; var s : Gemma3vState; l, npos : int64) {
    let d = t.n_embd
    let ff = t.n_ff
    let hd = d / t.n_head
    let o = gemma3v_block_offs(t, l)
    // attention (the g3v.* buckets feed asr_prof_report - the encode split)
    var tp = ref_time_ticks()
    layernorm_batch(s.xb, s.x, t.blob, o.ln1_w, o.ln1_b, d, npos, t.eps)
    asr_prof_add("g3v.norm", tp)
    tp = ref_time_ticks()
    g3v_gemm_biased(s.qq, t, s, o.w[GEMMA3V_GEMM_Q], o.b[GEMMA3V_GEMM_Q], s.xb, d, d, npos, t.q8)
    g3v_gemm_biased(s.kk, t, s, o.w[GEMMA3V_GEMM_K], o.b[GEMMA3V_GEMM_K], s.xb, d, d, npos, t.q8)
    g3v_gemm_biased(s.vv, t, s, o.w[GEMMA3V_GEMM_V], o.b[GEMMA3V_GEMM_V], s.xb, d, d, npos, t.q8)
    asr_prof_add("g3v.attn_mm", tp)
    tp = ref_time_ticks()
    attention_bidir(s.xb2, s.qq, s.kk, s.vv, s.hp, s.att, npos, d, t.n_head, 1.0 / sqrt(float(hd)))
    asr_prof_add("g3v.attn_core", tp)
    tp = ref_time_ticks()
    g3v_gemm_biased(s.xb, t, s, o.w[GEMMA3V_GEMM_O], o.b[GEMMA3V_GEMM_O], s.xb2, d, d, npos, t.q8)
    add_inplace_rows(s.x, s.xb, npos * d)
    asr_prof_add("g3v.attn_mm", tp)
    // ffn
    tp = ref_time_ticks()
    layernorm_batch(s.xb, s.x, t.blob, o.ln2_w, o.ln2_b, d, npos, t.eps)
    asr_prof_add("g3v.norm", tp)
    tp = ref_time_ticks()
    g3v_gemm_biased(s.h, t, s, o.w[GEMMA3V_GEMM_UP], o.b[GEMMA3V_GEMM_UP], s.xb, d, ff, npos, t.q8)
    asr_prof_add("g3v.ffn_mm", tp)
    tp = ref_time_ticks()
    gelu_tanh_lut_batch(s.h, ff, npos)
    asr_prof_add("g3v.gelu", tp)
    tp = ref_time_ticks()
    g3v_gemm_biased(s.xb, t, s, o.w[GEMMA3V_GEMM_DOWN], o.b[GEMMA3V_GEMM_DOWN], s.h, ff, d, npos, false)
    add_inplace_rows(s.x, s.xb, npos * d)
    asr_prof_add("g3v.ffn_mm", tp)
}

// the stem: im2col → patch conv + bias → learned position add, leaving the residual stream in
// s.x (npos rows). The mean/std normalize is PREPROCESSING for this family (encode_image_ owns
// it), not graph — the mtmd encode fixtures feed the graph raw, and so does this entry.
def private g3v_stem(t : Gemma3vTower; var s : Gemma3vState; planes : array<float>; w, h : int64) : int64 {
    let d = t.n_embd
    let npos = im2col_rgb_patches(planes, w, h, t.patch, s.x0)
    ensure_length(s.x, npos * d)
    ensure_length(s.xb, npos * d)
    ensure_length(s.xb2, npos * d)
    ensure_length(s.qq, npos * d)
    ensure_length(s.kk, npos * d)
    ensure_length(s.vv, npos * d)
    ensure_length(s.h, npos * t.n_ff)
    mm_blob_b(s.x, t.blob, t.patch_w, s.x0, t.patch_dim, d, npos)
    add_bias_rows(s.x, t.blob, t.patch_b, d, npos)
    unsafe {
        // the learned table is one row per patch in the same row-major walk im2col took
        var xp = addr(s.x[0])
        let tbl = plane_at(t.blob, t.pos_w)
        for (pi in range64(npos)) {
            var row = xp + pi * d
            let ep = tbl + pi * d
            for (i in range64(d)) {
                row[i] += ep[i]
            }
        }
    }
    return npos
}

// the tail: post-LN → 4×4 mean pool → weighted RMS (soft_emb_norm) → projection into out
def private g3v_tail(t : Gemma3vTower; var s : Gemma3vState; n_side : int64; var out : array<float>) : int64 {
    let d = t.n_embd
    let npos = n_side * n_side
    layernorm_batch(s.xb, s.x, t.blob, t.post_ln_w, t.post_ln_b, d, npos, t.eps)
    let nout = avg_pool2d_rows(s.pooled, s.xb, n_side, n_side, d, t.merge)
    rms_rows(s.pooled, s.pooled, t.blob, t.soft_norm_w, d, nout, t.eps)
    ensure_length(out, nout * t.proj_dim)
    mm_blob_b(out, t.blob, t.proj_w, s.pooled, d, t.proj_dim, nout)
    return nout
}

//! Encode a letterboxed, normalized canvas (`vision_normalize` planes, w×h) into `out` =
//! [nout × proj_dim] soft tokens. The canvas is FIXED at image_px² (the learned position table
//! covers exactly that grid); returns nout = (image_px / patch / merge)².
[hot_path]
def gemma3v_encode(t : Gemma3vTower; var s : Gemma3vState; planes : array<float>; w, h : int64;
                   var out : array<float>) : int64 {
    if (w != t.image_px || h != t.image_px) {
        panic("dasLLAMA gemma3v: canvas {w}x{h} - this tower serves exactly {t.image_px}x{t.image_px} (letterbox first)")
    }
    var tp = ref_time_ticks()
    let npos = g3v_stem(t, s, planes, w, h)
    asr_prof_add("g3v.stem", tp)
    for (l in range64(t.n_layer)) {
        g3v_block(t, s, l, npos)
    }
    tp = ref_time_ticks()
    let nout = g3v_tail(t, s, t.image_px / t.patch, out)
    asr_prof_add("g3v.tail", tp)
    return nout
}

//! The whole image path in one call: letterbox to the fixed 896² canvas → normalize →
//! `gemma3v_encode` into `out` = nout × proj_dim soft tokens (returns nout); `tag` names the
//! `DASLLAMA_VISION_DUMP` artifact.
def encode_image_(t : Gemma3vTower; var s : Gemma3vState; img : VisionImage; tag : string;
                  var out : array<float>) : int64 {
    if (img.width <= 0 || img.height <= 0) {
        panic("dasLLAMA gemma3v: encode_image_ got an empty image (decode failed?)")
    }
    let target = int2(int(t.image_px), int(t.image_px))
    var inscope boxed <- vision_letterbox(img, target)
    maybe_dump_vision(boxed, tag)
    var inscope planes <- vision_normalize(boxed)
    // this family's image_mean/std (0.5/0.5) live in preprocessing: [0,1] -> [-1,1] ahead of the graph
    for (p in planes) {
        p = p * 2.0 - 1.0
    }
    return gemma3v_encode(t, s, planes, t.image_px, t.image_px, out)
}
