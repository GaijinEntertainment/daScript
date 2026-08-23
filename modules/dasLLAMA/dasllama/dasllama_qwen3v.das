options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_qwen3v shared public

require dasllama/dasllama_lint public
require daslib/fio
require strings
require daslib/apply
require daslib/archive
require dasllama/dasllama_gguf
require dasllama/dasllama_math
require dasllama/dasllama_common   // apply_box_profile_runtime for the tower-image identity
require dasllama/dasllama_image   // the dlim rail: mapped planes, no allocation or work at load
require dasllama/dasllama_plane public
require dasllama/dasllama_tower   // shared encoder-tower pieces: LN, bias, attention, im2col, the aa resize
require dasllama/dasllama_rope    // build_rope_tabs_vision — the (h, w) ladder table
require dasllama/dasllama_vision   // encode_image_: geometry + letterbox + normalize ahead of the tower
require dasllama/dasllama_env     // g_env_engine.image — the dlim-rail switch
require math

//! The qwen3vl vision tower (Omni AND dense Qwen3-VL): temporal-pair conv folded at load →
//! spatial-merge reorder → +bias +bilinearly-resized 48×48 pos table → pre-LN blocks (fused
//! qkv, vision-mrope, scaled bidirectional attention, GELU-tanh FFN) → post-LN → 2×2 merger
//! MLP to proj_dim soft tokens. Dense carriers add DEEPSTACK taps: after each tap block, a
//! LayerNorm→fc1→GELU→fc2 merger over the ×4-merged rows emits one more proj_dim slice, and
//! the output rows widen to (1+n_deepstack)·proj_dim (slice 0 = the main merger). No clamp —
//! the files carry no ClippableLinear sidecars. Oracle: llama-mtmd-debug -p encode on the
//! f32-widened mmproj (`qwen3vl_plan.md`, dumps in models/qwen3vl-vision-oracle/).
//! CPU serving default: the block GEMMs serve as Q8_0 planes (read-time transcode, the
//! gemma3v recipe); the exact bf16/f32 lane is the parity rail.

let QWEN3V_ROPE_THETA = 10000.0   // models/qwen3vl.cpp hardcodes it — not in the gguf
// the family's dynamic-resolution token budget (mtmd set_limit_image_tokens for qwen)
let QWEN3V_MIN_TOKENS = 8
let QWEN3V_MAX_TOKENS = 4096

// per-block GEMM order — the block GEMM plane stride follows it
let QWEN3V_GEMM_QKV = 0l
let QWEN3V_GEMM_O = 1l
let QWEN3V_GEMM_UP = 2l
let QWEN3V_GEMM_DOWN = 3l
let QWEN3V_GEMMS = 4l

struct Qwen3vTower {
    n_embd : int64       // 1152 — the tower width
    n_layer : int64      // 27
    n_head : int64       // 16 (head width 72)
    n_ff : int64         // 4304
    proj_dim : int64     // the merger's output width — the soft-token width (decoder width)
    patch : int64        // 16 px
    patch_dim : int64    // patch² × 3 — the im2col row (768)
    merge : int64        // spatial merge (2): one soft token per merge×merge patches
    pos_side : int64     // the learned pos table's side (48) — resized to the actual grid
    eps : float
    // per-tensor plane choice follows the file; the folded patch conv is always f32 (a sum
    // of two on-disk tensors is a computed plane, not the file's bytes)
    blk_bf16 : bool
    mm_bf16 : bool
    // q8: the 4·n_layer block GEMMs SERVE as Q8_0 planes (read-time transcode) and layer_w
    // indexes qblob; the FFN pair serves padded to ff_pad (n_ff is not 32-aligned). The exact
    // lane serves the file's widths — only the q8 layout pads.
    q8 : bool
    ff_pad : int64       // n_ff rounded up to 64 — the q8 lane's served FFN width
    blob : PlaneF = PlaneF()
    wblob : PlaneU16 = PlaneU16()
    qblob : PlaneI8 = PlaneI8()
    qscales : PlaneF = PlaneF()
    patch_w : int64      // [n_embd × patch_dim] f32 — W0 + W1 folded (stills eval both convs on one frame)
    patch_b : int64      // [n_embd]
    pos_w : int64        // [pos_side² × n_embd] f32, row = y·side + x
    post_ln_w : int64; post_ln_b : int64   // [n_embd]
    mm0_w : int64        // [4·n_embd → 4·n_embd]
    mm0_b : int64
    mm2_w : int64        // [4·n_embd → proj_dim]
    mm2_b : int64
    layer_f : int64      // per-block f32 region; stride: ln1 w+b, ln2 w+b [d ×4], qkv_b [3d], out_b [d], up_b [ff], down_b [d]
    layer_f_stride : int64
    layer_w : int64      // per-block GEMM region (qblob when q8, wblob when blk_bf16, else blob); stride layer_w_stride
    layer_w_stride : int64
    // deepstack (dense Qwen3-VL): up to three tap mergers — output rows widen to
    // (1+n_deepstack)·proj_dim; 0 taps = the Omni tower, narrow rows
    n_deepstack : int64
    ds_l0 : int64        // tap block indices in encounter order (-1 = unused seat)
    ds_l1 : int64
    ds_l2 : int64
    ds_f : int64         // per-tap f32 region; stride: norm w+b [4d ×2], fc1_b [4d], fc2_b [proj]
    ds_f_stride : int64
    ds_w : int64         // per-tap GEMM region (mm plane type): fc1 [4d×4d], fc2 [4d×proj]
    ds_w_stride : int64
    image_map : void?
    image_bytes : uint64
}

//! Per-encode scratch, caller-owned; sized to the largest canvas seen.
struct Qwen3vState {
    @exact_size x0 : array<float>    // [npos × patch_dim] im2col rows
    @exact_size xn : array<float>    // conv out, natural patch order [npos × d]
    @exact_size x : array<float>     // residual stream, merge-reordered [npos × d]
    @exact_size xb : array<float>    // norm / GEMM out [npos × d]
    @exact_size xb2 : array<float>   // attention out [npos × d]
    @exact_size qkv : array<float>   // fused qkv GEMM out [npos × 3d]
    @exact_size qq : array<float>
    @exact_size kk : array<float>
    @exact_size vv : array<float>
    @exact_size hp : array<float>
    @scratch @exact_size att : array<float>
    @exact_size hu : array<float>    // ffn up, GELU'd in place [npos × ff]
    @exact_size m0 : array<float>    // merger mid [nout × 4d]
    @exact_size ds_mid : array<float>    // deepstack tap: LayerNorm out [nout × 4d] (m0 takes the fc1 out)
    @exact_size ds_slice : array<float>  // deepstack tap / main-merger staging [nout × proj]
    @scratch @exact_size pos_rs : array<float>   // the pos table resized to the grid [npos × d]
    @exact_size cos_t : array<float>; @exact_size sin_t : array<float>
    @scratch @exact_size xqi : array<int8>    // q8 lane: the requantized GEMM input, grown per site
    @scratch @exact_size xsi : array<float>   // its per-32-block scales
    @scratch pos2 : array<int2>      // per reordered row: (patch row, patch column)
    grid_w : int64
    grid_h : int64
}

def finalize(var t : Qwen3vTower) {
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

def finalize(var s : Qwen3vState) {
    delete s.x0
    delete s.xn
    delete s.x
    delete s.xb
    delete s.xb2
    delete s.qkv
    delete s.qq
    delete s.kk
    delete s.vv
    delete s.hp
    delete s.att
    delete s.hu
    delete s.m0
    delete s.ds_mid
    delete s.ds_slice
    delete s.pos_rs
    delete s.cos_t
    delete s.sin_t
    delete s.xqi
    delete s.xsi
    delete s.pos2
}

// serialize_image_meta(Qwen3vTower) covers 35 fields; image_map + image_bytes are deliberate
// skips (the two planes are sections, not meta)
let private QWEN3V_META_FIELDS = 35 + 2

def private serialize_image_meta(var arch : Archive; var t : Qwen3vTower) {
    verify(count_meta_fields(t) == QWEN3V_META_FIELDS)   // grew Qwen3vTower? extend this list
    arch |> serialize_raw(t.n_embd)
    arch |> serialize_raw(t.n_layer)
    arch |> serialize_raw(t.n_head)
    arch |> serialize_raw(t.n_ff)
    arch |> serialize_raw(t.proj_dim)
    arch |> serialize_raw(t.patch)
    arch |> serialize_raw(t.patch_dim)
    arch |> serialize_raw(t.merge)
    arch |> serialize_raw(t.pos_side)
    arch |> serialize_raw(t.eps)
    arch |> serialize_raw(t.blk_bf16)
    arch |> serialize_raw(t.mm_bf16)
    arch |> serialize_raw(t.q8)
    arch |> serialize_raw(t.ff_pad)
    arch |> serialize_raw(t.patch_w)
    arch |> serialize_raw(t.patch_b)
    arch |> serialize_raw(t.pos_w)
    arch |> serialize_raw(t.post_ln_w)
    arch |> serialize_raw(t.post_ln_b)
    arch |> serialize_raw(t.mm0_w)
    arch |> serialize_raw(t.mm0_b)
    arch |> serialize_raw(t.mm2_w)
    arch |> serialize_raw(t.mm2_b)
    arch |> serialize_raw(t.layer_f)
    arch |> serialize_raw(t.layer_f_stride)
    arch |> serialize_raw(t.layer_w)
    arch |> serialize_raw(t.layer_w_stride)
    arch |> serialize_raw(t.n_deepstack)
    arch |> serialize_raw(t.ds_l0)
    arch |> serialize_raw(t.ds_l1)
    arch |> serialize_raw(t.ds_l2)
    arch |> serialize_raw(t.ds_f)
    arch |> serialize_raw(t.ds_f_stride)
    arch |> serialize_raw(t.ds_w)
    arch |> serialize_raw(t.ds_w_stride)
}

[unused_argument(t)]
def private image_post_load(var t : Qwen3vTower) {}

//! The mint-side carrier (see dasllama_gemma4v's staging notes — section names bind by name).
struct Qwen3vStaging {
    t : Qwen3vTower = Qwen3vTower()
    blob : array<float>
    wblob : array<uint16>
    qblob : array<int8>       // q8 lane: the block GEMMs as Q8_0 (repacked for the active backend)
    qscales : array<float>
}

def private serialize_image_meta(var arch : Archive; var st : Qwen3vStaging) {
    serialize_image_meta(arch, st.t)
}

def finalize(var st : Qwen3vStaging) {
    delete st.blob
    delete st.wblob
    delete st.qblob
    delete st.qscales
    delete st.t
}

[unused_argument(st)]
def private image_post_load(var st : Qwen3vStaging) {}

// two lanes: "qwen3v-q8" serves the block GEMMs as Q8_0 (the CPU serving default); "qwen3v"
// keeps every plane as the file has it (the parity rail)
let private QWEN3V_TAG = "qwen3v"
let private QWEN3V_TAG_Q8 = "qwen3v-q8"

[init]
def private register_qwen3v_image_tags {
    register_image_family_tag(QWEN3V_TAG)
    register_image_family_tag(QWEN3V_TAG_Q8)
}

// the lane policy follows the fastest GEMM path on the box: the armed accelerate float-batch
// tier wants the file's planes; else q8. No Metal tower serves this family yet, so there is
// no driver clause. A pin overrides.
enum private Q3vLane {
    unset    // the policy decides
    exact    // the file's planes
    q8
}
var private g_q3v_q8_pin = Q3vLane.unset

def private q3v_serve_q8() : bool {
    if (g_q3v_q8_pin != Q3vLane.unset) {
        return g_q3v_q8_pin == Q3vLane.q8
    }
    return !float_batch_override_active()
}

//! Pin the qwen3v tower's block-GEMM format for subsequent loads: q8 (the CPU serving default)
//! vs the file's own planes (the parity rail). Each lane is its own prepared image.
//! ``reset_qwen3v_q8`` returns to the policy default.
def set_qwen3v_q8(on : bool) {
    g_q3v_q8_pin = on ? Q3vLane.q8 : Q3vLane.exact
}

def reset_qwen3v_q8() {
    g_q3v_q8_pin = Q3vLane.unset
}

//! Would the next load serve the block GEMMs as q8 — the pin when set, the policy otherwise.
def qwen3v_serves_q8() : bool => q3v_serve_q8()

def private q3v_tag() : string => q3v_serve_q8() ? QWEN3V_TAG_Q8 : QWEN3V_TAG

// the lane changes what a load mints and serves, so the load says which and why
def private q3v_announce_lane() {
    var why = "the CPU default (no accelerate tier)"
    if (g_q3v_q8_pin != Q3vLane.unset) {
        why = "pinned"
    } elif (float_batch_override_active()) {
        why = "DASLLAMA_ACCEL on: the float-batch tier serves the file's planes"
    }
    to_log(LOG_INFO, "dasLLAMA qwen3v: block GEMM lane {q3v_serve_q8() ? "q8" : "exact planes"} - {why}\n")
}

// the block GEMM tensors in QWEN3V_GEMM_* order; every per-GEMM list follows it
let private Q3V_BLK_GEMM_NAMES <- [ "attn_qkv", "attn_out", "ffn_up", "ffn_down" ]

def private q3v_gemm_sizes(d, ff : int64) : array<int64> {
    return <- [ 3l * d * d, d * d, d * ff, ff * d ]
}

def private q3v_verify_tensors(m : GGUFMeta; t : Qwen3vTower; gemm_sizes : array<int64>) {
    let d = t.n_embd
    gguf_expect_elems(m, "v.patch_embd.weight", t.patch_dim * d, "dasLLAMA qwen3v")
    gguf_expect_elems(m, "v.patch_embd.weight.1", t.patch_dim * d, "dasLLAMA qwen3v")
    gguf_expect_elems(m, "v.patch_embd.bias", d, "dasLLAMA qwen3v")
    gguf_expect_elems(m, "v.post_ln.weight", d, "dasLLAMA qwen3v")
    gguf_expect_elems(m, "v.post_ln.bias", d, "dasLLAMA qwen3v")
    gguf_expect_elems(m, "mm.0.weight", 16l * d * d, "dasLLAMA qwen3v")
    gguf_expect_elems(m, "mm.0.bias", 4l * d, "dasLLAMA qwen3v")
    gguf_expect_elems(m, "mm.2.weight", 4l * d * t.proj_dim, "dasLLAMA qwen3v")
    gguf_expect_elems(m, "mm.2.bias", t.proj_dim, "dasLLAMA qwen3v")
    for (k in range64(t.n_deepstack)) {
        let dl = k == 0l ? t.ds_l0 : k == 1l ? t.ds_l1 : t.ds_l2
        let p = "v.deepstack.{dl}"
        gguf_expect_elems(m, "{p}.norm.weight", 4l * d, "dasLLAMA qwen3v")
        gguf_expect_elems(m, "{p}.norm.bias", 4l * d, "dasLLAMA qwen3v")
        gguf_expect_elems(m, "{p}.fc1.weight", 16l * d * d, "dasLLAMA qwen3v")
        gguf_expect_elems(m, "{p}.fc1.bias", 4l * d, "dasLLAMA qwen3v")
        gguf_expect_elems(m, "{p}.fc2.weight", 4l * d * t.proj_dim, "dasLLAMA qwen3v")
        gguf_expect_elems(m, "{p}.fc2.bias", t.proj_dim, "dasLLAMA qwen3v")
        let fc1_type = gguf_tensor_type(m, "{p}.fc1.weight")
        if ((fc1_type == GGML_TYPE_BF16) != t.mm_bf16) {
            panic("dasLLAMA qwen3v: '{p}.fc1.weight' ({fc1_type}) is not the element type of mm.0.weight ({t.mm_bf16 ? GGML_TYPE_BF16 : GGML_TYPE_F32}) - mixed merger types are not supported")
        }
    }
    let biases <- [ 3l * d, d, t.n_ff, d ]
    for (l in range64(t.n_layer)) {
        let p = "v.blk.{l}"
        gguf_expect_elems(m, "{p}.ln1.weight", d, "dasLLAMA qwen3v")
        gguf_expect_elems(m, "{p}.ln1.bias", d, "dasLLAMA qwen3v")
        gguf_expect_elems(m, "{p}.ln2.weight", d, "dasLLAMA qwen3v")
        gguf_expect_elems(m, "{p}.ln2.bias", d, "dasLLAMA qwen3v")
        for (g in range64(QWEN3V_GEMMS)) {
            let name = "{p}.{Q3V_BLK_GEMM_NAMES[g]}.weight"
            gguf_expect_elems(m, name, gemm_sizes[g], "dasLLAMA qwen3v")
            gguf_expect_elems(m, "{p}.{Q3V_BLK_GEMM_NAMES[g]}.bias", biases[g], "dasLLAMA qwen3v")
            if ((gguf_tensor_type(m, name) == GGML_TYPE_BF16) != t.blk_bf16) {
                panic("dasLLAMA qwen3v: '{name}' ({gguf_tensor_type(m, name)}) is not the element type of v.blk.0.attn_qkv.weight ({t.blk_bf16 ? GGML_TYPE_BF16 : GGML_TYPE_F32}) - mixed block GEMM types are not supported")
            }
        }
    }
}

def private q3v_layout(var t : Qwen3vTower; var off : int64&; var woff : int64&; var qo : int64&) {
    let d = t.n_embd
    let ff = t.n_ff
    let ffs = t.q8 ? t.ff_pad : ff   // the served FFN width — only the q8 layout pads
    t.patch_w = off; off += t.patch_dim * d
    t.patch_b = off; off += d
    t.pos_w = off; off += t.pos_side * t.pos_side * d
    t.post_ln_w = off; off += d
    t.post_ln_b = off; off += d
    t.mm0_w = take_cursor(t.mm_bf16, off, woff, 16l * d * d)
    t.mm0_b = off; off += 4l * d
    t.mm2_w = take_cursor(t.mm_bf16, off, woff, 4l * d * t.proj_dim)
    t.mm2_b = off; off += t.proj_dim
    t.layer_f_stride = 4l * d + 3l * d + d + t.ff_pad + d   // the up-bias slot is ff_pad both lanes (zero tail)
    t.layer_f = off; off += t.n_layer * t.layer_f_stride
    t.layer_w_stride = 4l * d * d + 2l * d * ffs
    if (t.q8) {
        t.layer_w = qo
        qo += t.n_layer * t.layer_w_stride
    } else {
        t.layer_w = take_cursor(t.blk_bf16, off, woff, t.n_layer * t.layer_w_stride)
    }
    if (t.n_deepstack > 0l) {
        t.ds_f_stride = 12l * d + t.proj_dim
        t.ds_f = off; off += t.n_deepstack * t.ds_f_stride
        t.ds_w_stride = 16l * d * d + 4l * d * t.proj_dim
        t.ds_w = take_cursor(t.mm_bf16, off, woff, t.n_deepstack * t.ds_w_stride)
    }
}

// gemm_sizes here are the SERVED sizes (the q8 lane's FFN pair at ff_pad); the file expectations
// were checked with the file's own sizes in q3v_verify_tensors
def private q3v_read_planes(m : GGUFMeta; bytes : array<uint8> | #; var st : Qwen3vStaging; gemm_sizes : array<int64>) {
    let d = st.t.n_embd
    let ff = st.t.n_ff
    var wscratch : array<float>   // the q8 transcode's f32 staging, one GEMM at a time
    var rowscratch : array<float>   // the down GEMM's unpadded file rows ahead of the expand
    tower_read_conv_pair_folded(m, bytes, "v.patch_embd.weight", st.blob, st.t.patch_w, st.t.patch_dim * d)
    gguf_read_tensor_f32(m, bytes, "v.patch_embd.bias", st.blob, st.t.patch_b, d)
    gguf_read_tensor_f32(m, bytes, "v.position_embd.weight", st.blob, st.t.pos_w, st.t.pos_side * st.t.pos_side * d)
    gguf_read_tensor_f32(m, bytes, "v.post_ln.weight", st.blob, st.t.post_ln_w, d)
    gguf_read_tensor_f32(m, bytes, "v.post_ln.bias", st.blob, st.t.post_ln_b, d)
    tower_read_gemm(m, bytes, "mm.0.weight", st.wblob, st.blob, st.t.mm_bf16, st.t.mm0_w, 16l * d * d)
    gguf_read_tensor_f32(m, bytes, "mm.0.bias", st.blob, st.t.mm0_b, 4l * d)
    tower_read_gemm(m, bytes, "mm.2.weight", st.wblob, st.blob, st.t.mm_bf16, st.t.mm2_w, 4l * d * st.t.proj_dim)
    gguf_read_tensor_f32(m, bytes, "mm.2.bias", st.blob, st.t.mm2_b, st.t.proj_dim)
    for (k in range64(st.t.n_deepstack)) {
        let dl = k == 0l ? st.t.ds_l0 : k == 1l ? st.t.ds_l1 : st.t.ds_l2
        let p = "v.deepstack.{dl}"
        var dfo = st.t.ds_f + k * st.t.ds_f_stride
        gguf_read_tensor_f32(m, bytes, "{p}.norm.weight", st.blob, dfo, 4l * d); dfo += 4l * d
        gguf_read_tensor_f32(m, bytes, "{p}.norm.bias", st.blob, dfo, 4l * d); dfo += 4l * d
        gguf_read_tensor_f32(m, bytes, "{p}.fc1.bias", st.blob, dfo, 4l * d); dfo += 4l * d
        gguf_read_tensor_f32(m, bytes, "{p}.fc2.bias", st.blob, dfo, st.t.proj_dim)
        let dwo = st.t.ds_w + k * st.t.ds_w_stride
        tower_read_gemm(m, bytes, "{p}.fc1.weight", st.wblob, st.blob, st.t.mm_bf16, dwo, 16l * d * d)
        tower_read_gemm(m, bytes, "{p}.fc2.weight", st.wblob, st.blob, st.t.mm_bf16, dwo + 16l * d * d, 4l * d * st.t.proj_dim)
    }
    for (l in range64(st.t.n_layer)) {
        let p = "v.blk.{l}"
        var fo = st.t.layer_f + l * st.t.layer_f_stride
        gguf_read_tensor_f32(m, bytes, "{p}.ln1.weight", st.blob, fo, d); fo += d
        gguf_read_tensor_f32(m, bytes, "{p}.ln1.bias", st.blob, fo, d); fo += d
        gguf_read_tensor_f32(m, bytes, "{p}.ln2.weight", st.blob, fo, d); fo += d
        gguf_read_tensor_f32(m, bytes, "{p}.ln2.bias", st.blob, fo, d); fo += d
        gguf_read_tensor_f32(m, bytes, "{p}.attn_qkv.bias", st.blob, fo, 3l * d); fo += 3l * d
        gguf_read_tensor_f32(m, bytes, "{p}.attn_out.bias", st.blob, fo, d); fo += d
        gguf_read_tensor_f32(m, bytes, "{p}.ffn_up.bias", st.blob, fo, ff)
        tower_zero_span(st.blob, fo + ff, st.t.ff_pad - ff)   // the padded up bias: zero tail
        fo += st.t.ff_pad
        gguf_read_tensor_f32(m, bytes, "{p}.ffn_down.bias", st.blob, fo, d)
        var wo = st.t.layer_w + l * st.t.layer_w_stride
        for (g in range64(QWEN3V_GEMMS)) {
            let wname = "{p}.{Q3V_BLK_GEMM_NAMES[g]}.weight"
            if (!st.t.q8) {
                tower_read_gemm(m, bytes, wname, st.wblob, st.blob, st.t.blk_bf16, wo, gemm_sizes[g])
            } elif (g == QWEN3V_GEMM_UP) {
                tower_stage_q8_zero_rows(m, bytes, wname, st.qblob, st.qscales, wscratch, wo, ff, st.t.ff_pad, d, "dasLLAMA qwen3v q8")
            } elif (g == QWEN3V_GEMM_DOWN) {
                tower_stage_q8_pad_cols(m, bytes, wname, st.qblob, st.qscales, wscratch, rowscratch, wo, d, ff, st.t.ff_pad, "dasLLAMA qwen3v q8")
            } else {
                tower_read_gemm_q8(m, bytes, wname, st.qblob, st.qscales, wscratch, wo, gemm_sizes[g], "dasLLAMA qwen3v q8")
            }
            wo += gemm_sizes[g]
        }
    }
    delete wscratch
    delete rowscratch
}

// repack every block GEMM region for the active backend's q8q8 kernels — per-row alignment (the
// region's input width) keeps blocks from straddling rows
def private q3v_repack_all(var st : Qwen3vStaging; gemm_sizes : array<int64>) {
    if (!st.t.q8 || !select_matmul_backend_for_load_()) {
        return
    }
    let d = st.t.n_embd
    let widths <- [ d, d, d, st.t.ff_pad ]   // qkv o up down: the input width of each
    for (l in range64(st.t.n_layer)) {
        var wo = st.t.layer_w + l * st.t.layer_w_stride
        for (g in range64(QWEN3V_GEMMS)) {
            repack_q8q8_weight(st.qblob, st.qscales, wo, widths[g], gemm_sizes[g] / widths[g])
            wo += gemm_sizes[g]
        }
    }
}

// collect the deepstack tap layers from the TENSOR list, never the metadata (is_deepstack_layers
// flags are converter boilerplate on the Omni file, which carries zero merger tensors)
def private q3v_scan_deepstack(m : GGUFMeta; var t : Qwen3vTower; path : string) {
    t.ds_l0 = -1l
    t.ds_l1 = -1l
    t.ds_l2 = -1l
    for (l in range64(t.n_layer)) {
        if (gguf_find_tensor(m, "v.deepstack.{l}.fc1.weight") >= 0) {
            if (t.n_deepstack >= 3l) {
                panic("dasLLAMA qwen3v: '{path}' carries more than three deepstack taps - not served")
            }
            if (t.n_deepstack == 0l) {
                t.ds_l0 = l
            } elif (t.n_deepstack == 1l) {
                t.ds_l1 = l
            } else {
                t.ds_l2 = l
            }
            t.n_deepstack ++
        }
    }
}

//! Read the tower off an mmproj gguf into owned staging arrays — the mint side of the image
//! rail. No staged-size cap: the Omni mmproj (2.1 GB, both towers in one file) already stages
//! its audio half the same way; the streaming mint is followup_general #24's item.
def stage_qwen3v_tower(path : string) : Qwen3vStaging {
    var st = Qwen3vStaging()
    st.t.q8 = q3v_serve_q8()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open mmproj gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        // the Omni converter scopes the key under vision; the dense Qwen3-VL one does not
        let proj_key = gguf_has(m, "clip.vision.projector_type") ? "clip.vision.projector_type" : "clip.projector_type"
        let proj = gguf_str(m, bytes, proj_key)
        if (proj != "qwen3vl_merger") {
            panic("dasLLAMA: mmproj projector '{proj}' is not qwen3vl_merger")
        }
        st.t.patch = gguf_int(m, bytes, "clip.vision.patch_size")
        st.t.patch_dim = st.t.patch * st.t.patch * 3l
        st.t.n_embd = gguf_int(m, bytes, "clip.vision.embedding_length")
        st.t.n_layer = gguf_int(m, bytes, "clip.vision.block_count")
        st.t.n_head = gguf_int(m, bytes, "clip.vision.attention.head_count")
        st.t.n_ff = gguf_int(m, bytes, "clip.vision.feed_forward_length")
        st.t.proj_dim = gguf_int(m, bytes, "clip.vision.projection_dim")
        st.t.eps = gguf_f32(m, bytes, "clip.vision.attention.layer_norm_epsilon")
        st.t.merge = 2l
        if (gguf_has(m, "clip.vision.spatial_merge_size")) {
            st.t.merge = gguf_int(m, bytes, "clip.vision.spatial_merge_size")
        }
        let d = st.t.n_embd
        if (d <= 0l || st.t.n_head <= 0l || d % st.t.n_head != 0l || (d / st.t.n_head) % 4l != 0l) {
            panic("dasLLAMA qwen3v: embedding_length {d} / head_count {st.t.n_head} - the head width must be a positive multiple of 4 (two vision-rope ladders of pairs)")
        }
        if (st.t.merge != 2l) {
            panic("dasLLAMA qwen3v: spatial_merge_size {st.t.merge} - only the 2x2 merger is implemented")
        }
        q3v_scan_deepstack(m, st.t, path)
        let pos_ti = gguf_find_tensor(m, "v.position_embd.weight")
        if (pos_ti < 0) {
            panic("dasLLAMA qwen3v: mmproj is missing tensor 'v.position_embd.weight'")
        }
        let pos_rows = m.tensors[pos_ti].n_elem / d
        st.t.pos_side = 0l
        for (side in range64(1l, 257l)) {
            if (side * side == pos_rows) {
                st.t.pos_side = side
            }
        }
        if (st.t.pos_side == 0l) {
            panic("dasLLAMA qwen3v: 'v.position_embd.weight' has {pos_rows} rows - not a square grid")
        }
        st.t.blk_bf16 = gguf_tensor_type(m, "v.blk.0.attn_qkv.weight") == GGML_TYPE_BF16
        st.t.mm_bf16 = gguf_tensor_type(m, "mm.0.weight") == GGML_TYPE_BF16
        st.t.ff_pad = (st.t.n_ff + 63l) / 64l * 64l   // %64 fits future GPU tiles, %32 quantizes
        let file_sizes <- q3v_gemm_sizes(d, st.t.n_ff)
        q3v_verify_tensors(m, st.t, file_sizes)
        let serve_sizes <- q3v_gemm_sizes(d, st.t.q8 ? st.t.ff_pad : st.t.n_ff)
        var off = 0l
        var woff = 0l
        var qo = 0l
        q3v_layout(st.t, off, woff, qo)
        st.blob |> reserve_resize(off)
        st.wblob |> reserve_resize(woff)
        st.qblob |> reserve_resize(qo)
        st.qscales |> reserve_resize(qo / 32l)
        q3v_read_planes(m, bytes, st, serve_sizes)
        q3v_repack_all(st, serve_sizes)
    }
    fclose(f)
    return <- st
}

//! Load the qwen3v tower, image-backed (`dasllama_gemma4v`'s load shape: a prepared `.dlim`
//! maps and every plane borrows; missing or stale, one is minted from the source mmproj).
def load_qwen3v_tower(path : string) : Qwen3vTower {
    var tag = q3v_tag()
    // a .dlim named directly: no mmproj to regenerate from, so a wrong identity panics; the lane is
    // the file's (its baked tag), not the pin's - a served tower reads its q8 flag from the meta
    if (path |> ends_with(".dlim")) {
        let baked = image_family_tag(path)
        if (baked == QWEN3V_TAG || baked == QWEN3V_TAG_Q8) {
            tag = baked
        }
        apply_box_profile_runtime()
        let ts_img = ref_time_ticks()
        var t = Qwen3vTower()
        if (!load_image(path, t, tag)) {
            panic("dasLLAMA: '{path}' is not a prepared qwen3v-tower image for this box/knobs (identity {image_identity(tag)}) - regenerate it from the source mmproj gguf")
        }
        to_log(LOG_INFO, "dasLLAMA: prepared qwen3v-tower image mapped in {get_time_usec(ts_img) / 1000} ms - {path}\n")
        q3v_check_planes(t, path)
        return <- t
    }
    q3v_announce_lane()
    if (g_env_engine.image) {
        apply_box_profile_runtime()   // a backend pin must precede the identity (it names the backend)
        let img = image_path_for(path, tag)
        let ts_img = ref_time_ticks()
        var t = Qwen3vTower()
        if (load_image(img, t, tag)) {
            to_log(LOG_INFO, "dasLLAMA: prepared qwen3v-tower image mapped in {get_time_usec(ts_img) / 1000} ms - {img}\n")
            q3v_check_planes(t, img)
            return <- t
        }
        delete t
        var inscope st <- stage_qwen3v_tower(path)
        var built = Qwen3vTower()
        if (cache_via_image_staged(st, built, img, tag)) {
            dlim_gc_stale(path, tag)   // the fresh bake ON DISK proves its lane's siblings dead
        }
        q3v_check_planes(built, img)
        return <- built
    }
    var inscope st <- stage_qwen3v_tower(path)
    var built = Qwen3vTower()
    mint_qwen3v_tower(st, built)
    q3v_check_planes(built, path)
    return <- built
}

//! Mint a served tower from a staged read WITHOUT touching disk — the off-rail control for suites.
def mint_qwen3v_tower(var st : Qwen3vStaging; var out : Qwen3vTower) {
    cache_via_image_staged(st, out, "", st.t.q8 ? QWEN3V_TAG_Q8 : QWEN3V_TAG)
}

// the bind walk skips a missing section silently, so the flag=>plane invariant must be checked
// once the carrier is live, or the kernel derefs null
def private q3v_check_planes(t : Qwen3vTower; src : string) {
    if (((t.blk_bf16 && !t.q8) || t.mm_bf16) && empty(t.wblob)) {
        panic("dasLLAMA qwen3v: '{src}' declares bf16 planes but carries no wblob section - the image is corrupt or hand-edited")
    }
    if (t.q8 && (empty(t.qblob) || empty(t.qscales))) {
        panic("dasLLAMA qwen3v: '{src}' declares q8 block GEMMs but carries no qblob/qscales sections - the image is corrupt or hand-edited")
    }
}

// the (h, w) walk order after the spatial merge: 2×2 partners adjacent, groups row-major
// (clip.cpp's positions fill and the graph's merge permute follow the same walk)
def private q3v_reorder_src(r, pw : int64) : int64 {
    let g = r / 4l
    let sub = r % 4l
    let gcols = pw / 2l
    let sy = (g / gcols) * 2l + sub / 2l
    let sx = (g % gcols) * 2l + sub % 2l
    return sy * pw + sx
}

// the rope tables + positions for a pw × ph patch grid, rebuilt only when the grid changes
def private q3v_rope_tables(t : Qwen3vTower; var s : Qwen3vState; pw, ph : int64) {
    if (s.grid_w == pw && s.grid_h == ph) {
        return
    }
    let npos = pw * ph
    let hd = t.n_embd / t.n_head
    if (long_length(s.pos2) < npos) {   // inline: PERF026 can't see @scratch through reserve_resize
        s.pos2 |> reserve(npos)
        s.pos2 |> resize(npos)
    }
    let pwi = int(pw)
    for (r in range64(npos)) {
        let src = int(q3v_reorder_src(r, pw))
        s.pos2[r] = int2(src / pwi, src % pwi)
    }
    build_rope_tabs_vision(s.cos_t, s.sin_t, QWEN3V_ROPE_THETA, s.pos2, npos, hd)
    s.grid_w = pw
    s.grid_h = ph
}

//! One block's offsets (elements): the f32 rows in ``blob``, the four GEMM regions in the
//! block GEMM plane (QWEN3V_GEMM_* order).
struct Qwen3vBlockOffs {
    ln1_w : int64; ln1_b : int64
    ln2_w : int64; ln2_b : int64
    qkv_b : int64
    out_b : int64
    up_b : int64
    down_b : int64
    w : int64[4]
}

def qwen3v_block_offs(t : Qwen3vTower; l : int64) : Qwen3vBlockOffs {
    let d = t.n_embd
    let fo = t.layer_f + l * t.layer_f_stride
    var o = Qwen3vBlockOffs(ln1_w = fo, ln1_b = fo + d, ln2_w = fo + 2l * d, ln2_b = fo + 3l * d,
        qkv_b = fo + 4l * d, out_b = fo + 7l * d, up_b = fo + 8l * d, down_b = fo + 8l * d + t.ff_pad)
    let sizes <- q3v_gemm_sizes(d, t.q8 ? t.ff_pad : t.n_ff)   // the SERVED sizes — the q8 FFN pair pads
    var at = t.layer_w + l * t.layer_w_stride
    for (g in range64(QWEN3V_GEMMS)) {
        o.w[g] = at
        at += sizes[g]
    }
    return o
}

// one block GEMM on the lane the load picked: requant + the q8q8 batch kernel when q8, the
// file's bf16/f32 plane otherwise
def private q3v_gemm_blk(var y : array<float>; t : Qwen3vTower; var s : Qwen3vState; off : int64;
                         x : array<float>; n, d_out, npos : int64) {
    if (t.q8) {
        requant_rows_q8_sized(x, n, npos, s.xqi, s.xsi)
        matmul_q8q8_batch(y, t.qblob, t.qscales, off, s.xqi, s.xsi, n, d_out, npos)
    } else {
        mm_plane_b(y, t.blk_bf16, t.wblob, t.blob, off, x, n, d_out, npos)
    }
}

// split the fused qkv rows [npos × 3d] into contiguous q/k/v [npos × d]
def private q3v_split_qkv(var s : Qwen3vState; d, npos : int64) {
    unsafe {
        let src = addr<float const?>(s.qkv[0])
        var qp = addr(s.qq[0])
        var kp = addr(s.kk[0])
        var vp = addr(s.vv[0])
        for (r in range64(npos)) {
            let row = src + r * 3l * d
            for (i in range64(d)) {
                qp[r * d + i] = row[i]
                kp[r * d + i] = row[d + i]
                vp[r * d + i] = row[2l * d + i]
            }
        }
    }
}

// one pre-LN block over the residual stream s.x (npos rows)
def private q3v_block(t : Qwen3vTower; var s : Qwen3vState; l, npos : int64) {
    let d = t.n_embd
    let ff = t.q8 ? t.ff_pad : t.n_ff   // the served FFN width — the q8 pad lanes are zero by construction
    let hd = d / t.n_head
    let o = qwen3v_block_offs(t, l)
    var tp = ref_time_ticks()
    layernorm_batch(s.xb, s.x, t.blob, o.ln1_w, o.ln1_b, d, npos, t.eps)
    asr_prof_add("q3v.norm", tp)
    tp = ref_time_ticks()
    q3v_gemm_blk(s.qkv, t, s, o.w[QWEN3V_GEMM_QKV], s.xb, d, 3l * d, npos)
    add_bias_rows(s.qkv, t.blob, o.qkv_b, 3l * d, npos)
    asr_prof_add("q3v.attn_mm", tp)
    tp = ref_time_ticks()
    q3v_split_qkv(s, d, npos)
    rope_neox_tab_rows(s.qq, npos, t.n_head, hd, s.cos_t, s.sin_t)
    rope_neox_tab_rows(s.kk, npos, t.n_head, hd, s.cos_t, s.sin_t)
    asr_prof_add("q3v.qkv_rope", tp)
    tp = ref_time_ticks()
    attention_bidir(s.xb2, s.qq, s.kk, s.vv, s.hp, s.att, npos, d, t.n_head, 1.0 / sqrt(float(hd)))
    asr_prof_add("q3v.attn_core", tp)
    tp = ref_time_ticks()
    q3v_gemm_blk(s.xb, t, s, o.w[QWEN3V_GEMM_O], s.xb2, d, d, npos)
    add_bias_rows(s.xb, t.blob, o.out_b, d, npos)
    add_inplace_rows(s.x, s.xb, npos * d)
    asr_prof_add("q3v.attn_mm", tp)
    tp = ref_time_ticks()
    layernorm_batch(s.xb, s.x, t.blob, o.ln2_w, o.ln2_b, d, npos, t.eps)
    asr_prof_add("q3v.norm", tp)
    tp = ref_time_ticks()
    q3v_gemm_blk(s.hu, t, s, o.w[QWEN3V_GEMM_UP], s.xb, d, ff, npos)
    add_bias_rows(s.hu, t.blob, o.up_b, ff, npos)
    gelu_tanh_lut_batch(s.hu, ff, npos)
    q3v_gemm_blk(s.xb, t, s, o.w[QWEN3V_GEMM_DOWN], s.hu, ff, d, npos)
    add_bias_rows(s.xb, t.blob, o.down_b, d, npos)
    add_inplace_rows(s.x, s.xb, npos * d)
    asr_prof_add("q3v.ffn", tp)
}

// the stem: im2col → conv (folded W) → merge-reorder + patch bias + resized pos table, leaving
// the reordered residual stream in s.x; sizes every block buffer for this canvas
def private q3v_stem(t : Qwen3vTower; var s : Qwen3vState; planes : array<float>; w, h, pw, ph : int64) : int64 {
    let d = t.n_embd
    let npos = im2col_rgb_patches(planes, w, h, t.patch, s.x0)
    ensure_length(s.xn, npos * d)
    ensure_length(s.x, npos * d)
    ensure_length(s.xb, npos * d)
    ensure_length(s.xb2, npos * d)
    ensure_length(s.qkv, npos * 3l * d)
    ensure_length(s.qq, npos * d)
    ensure_length(s.kk, npos * d)
    ensure_length(s.vv, npos * d)
    ensure_length(s.hu, npos * (t.q8 ? t.ff_pad : t.n_ff))
    ensure_length(s.m0, (npos / 4l) * 4l * d)
    mm_plane_b(s.xn, false, t.wblob, t.blob, t.patch_w, s.x0, t.patch_dim, d, npos)
    var pos_base = t.pos_w
    var from_plane = true
    if (pw != t.pos_side || ph != t.pos_side) {
        interpolate_grid_bilinear_aa(s.pos_rs, t.blob, t.pos_w, t.pos_side, t.pos_side, pw, ph, d)
        from_plane = false
        pos_base = 0l
    }
    unsafe {
        var xp = addr(s.x[0])
        let np = addr<float const?>(s.xn[0])
        let bp = plane_at(t.blob, t.patch_b)
        let pp = from_plane ? plane_at(t.blob, pos_base) : addr<float const?>(s.pos_rs[0])
        for (r in range64(npos)) {
            let src = q3v_reorder_src(r, pw)
            var row = xp + r * d
            let crow = np + src * d
            let prow = pp + src * d
            for (i in range64(d)) {
                row[i] = (crow[i] + bp[i]) + prow[i]
            }
        }
    }
    return npos
}

//! The soft-token row width this tower emits: proj_dim, widened by the deepstack slices.
def qwen3v_row_width(t : Qwen3vTower) : int64 => t.proj_dim * (1l + t.n_deepstack)

// one deepstack tap merger over the CURRENT residual stream (rows merge-adjacent, so the ×4
// reshape is flat): LayerNorm → fc1 → GELU → fc2 into the wide out at slice k+1
def private q3v_deepstack_tap(t : Qwen3vTower; var s : Qwen3vState; k, npos : int64; var out : array<float>) {
    let d = t.n_embd
    let nout = npos / 4l
    let fo = t.ds_f + k * t.ds_f_stride
    let wo = t.ds_w + k * t.ds_w_stride
    layernorm_batch(s.ds_mid, s.x, t.blob, fo, fo + 4l * d, 4l * d, nout, t.eps)
    mm_plane_b(s.m0, t.mm_bf16, t.wblob, t.blob, wo, s.ds_mid, 4l * d, 4l * d, nout)
    add_bias_rows(s.m0, t.blob, fo + 8l * d, 4l * d, nout)
    gelu_tanh_lut_batch(s.m0, 4l * d, nout)
    mm_plane_b(s.ds_slice, t.mm_bf16, t.wblob, t.blob, wo + 16l * d * d, s.m0, 4l * d, t.proj_dim, nout)
    add_bias_rows(s.ds_slice, t.blob, fo + 12l * d, t.proj_dim, nout)
    q3v_scatter_slice(t, s.ds_slice, out, k + 1l, nout)
}

// copy a [nout × proj] slice into the wide out rows at slice index `sl`
def private q3v_scatter_slice(t : Qwen3vTower; src : array<float>; var out : array<float>; sl, nout : int64) {
    let w = qwen3v_row_width(t)
    unsafe {
        let sp = addr<float const?>(src[0])
        var op = addr(out[0])
        for (r in range64(nout)) {
            copy_floats(op + r * w + sl * t.proj_dim, sp + r * t.proj_dim, t.proj_dim)
        }
    }
}

// the tail: post-LN, then the 2×2 merger MLP (rows are already merge-adjacent, so the ×4
// reshape is a flat reinterpretation) into out; returns nout
def private q3v_tail(t : Qwen3vTower; var s : Qwen3vState; npos : int64; var out : array<float>) : int64 {
    let d = t.n_embd
    let nout = npos / 4l
    layernorm_batch(s.xb, s.x, t.blob, t.post_ln_w, t.post_ln_b, d, npos, t.eps)
    mm_plane_b(s.m0, t.mm_bf16, t.wblob, t.blob, t.mm0_w, s.xb, 4l * d, 4l * d, nout)
    add_bias_rows(s.m0, t.blob, t.mm0_b, 4l * d, nout)
    gelu_tanh_lut_batch(s.m0, 4l * d, nout)
    ensure_length(out, nout * t.proj_dim)
    mm_plane_b(out, t.mm_bf16, t.wblob, t.blob, t.mm2_w, s.m0, 4l * d, t.proj_dim, nout)
    add_bias_rows(out, t.blob, t.mm2_b, t.proj_dim, nout)
    return nout
}

//! Encode a letterboxed, normalized canvas (`vision_normalize(0.5, 0.5)` planes, w×h) into
//! `out` = [nout × qwen3v_row_width] soft tokens (deepstack: slice 0 = the main merger,
//! slice k+1 = tap k); returns nout = (w/32)×(h/32). Oracle fixtures skip the normalize.
[hot_path]
def qwen3v_encode(t : Qwen3vTower; var s : Qwen3vState; planes : array<float>; w, h : int64;
                  var out : array<float>) : int64 {
    let span = t.patch * t.merge
    if (w % span != 0l || h % span != 0l) {
        panic("dasLLAMA qwen3v: canvas {w}x{h} is not a multiple of the {span}px merged patch - letterbox first")
    }
    let pw = w / t.patch
    let ph = h / t.patch
    var tp = ref_time_ticks()
    q3v_rope_tables(t, s, pw, ph)
    let npos = q3v_stem(t, s, planes, w, h, pw, ph)
    asr_prof_add("q3v.stem", tp)
    let nds = t.n_deepstack
    if (nds > 0l) {
        ensure_length(out, (npos / 4l) * qwen3v_row_width(t))
        ensure_length(s.ds_mid, (npos / 4l) * 4l * t.n_embd)
        ensure_length(s.ds_slice, (npos / 4l) * t.proj_dim)
    }
    for (l in range64(t.n_layer)) {
        q3v_block(t, s, l, npos)
        if (nds > 0l) {
            let k = l == t.ds_l0 ? 0l : l == t.ds_l1 ? 1l : l == t.ds_l2 ? 2l : -1l
            if (k >= 0l) {
                q3v_deepstack_tap(t, s, k, npos, out)
            }
        }
    }
    tp = ref_time_ticks()
    var nout = 0l
    if (nds > 0l) {
        nout = q3v_tail(t, s, npos, s.ds_slice)
        q3v_scatter_slice(t, s.ds_slice, out, 0l, nout)
    } else {
        nout = q3v_tail(t, s, npos, out)
    }
    asr_prof_add("q3v.tail", tp)
    return nout
}

//! The whole image path in one call: geometry → letterbox → normalize(0.5, 0.5) →
//! `qwen3v_encode` into `out` (returns nout). The token budget is the FAMILY's [8, 4096]
//! (mtmd's qwen limits), not the gemma-scoped DASLLAMA_VISION_* knobs.
def encode_image_(t : Qwen3vTower; var s : Qwen3vState; img : VisionImage; tag : string;
                  var out : array<float>) : int64 {
    if (img.width <= 0 || img.height <= 0) {
        panic("dasLLAMA qwen3v: encode_image_ got an empty image (decode failed?)")
    }
    let align = int(t.patch * t.merge)
    let target = vision_target_size(img.width, img.height, align, QWEN3V_MIN_TOKENS, QWEN3V_MAX_TOKENS)
    var inscope boxed <- vision_letterbox(img, target)
    maybe_dump_vision(boxed, tag)
    var inscope planes <- vision_normalize(boxed, 0.5, 0.5)
    return qwen3v_encode(t, s, planes, int64(target.x), int64(target.y), out)
}
