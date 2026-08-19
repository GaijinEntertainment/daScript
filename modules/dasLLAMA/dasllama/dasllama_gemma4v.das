options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_gemma4v shared public

require dasllama/dasllama_lint public
require daslib/fio
require strings
require daslib/apply
require daslib/archive
require dasllama/dasllama_gguf
require dasllama/dasllama_math   // matmul_q8q8_batch + the backend repack — the q8 block-GEMM lane
require dasllama/dasllama_convert   // quantize_q8_0_into / requant_rows_q8
require dasllama/dasllama_common   // apply_box_profile_runtime for the tower-image identity
require dasllama/dasllama_image   // the dlim rail: mapped planes, no allocation or work at load
require dasllama/dasllama_plane public   // PlaneF/PlaneU16 — re-exported so requirers can delete a tower
require dasllama/dasllama_audio   // the shared encoder-tower pieces: clamp, row norms, attention, 2-axis rope, im2col, pool
require dasllama/dasllama_rope    // build_rope_tabs_rows — the per-axis rope tables
require dasllama/dasllama_vision   // encode_image_: geometry + letterbox + normalize ahead of the tower
require dasllama/dasllama_env     // g_env_vision: the token-budget knobs the geometry reads
require math

//! The gemma4v vision tower (gemma-4 E-series; E2B is the tested pair): 16-block pre-norm RMS ViT
//! over 16 px patches — x·2−1 → patch conv → x/y position tables → blocks (clamped GEMMs, per-head
//! q/k RMS, two-axis NEOX rope, weightless V RMS, unscaled bidirectional attention, GEGLU-quick)
//! → 3×3 average pool ×√d → weightless RMS → projection. Oracle: llama-mtmd-debug -p encode on the
//! f32-widened mmproj, CPU, -fa off (`vision_gemma4v_plan.md`).

// mtmd hardcodes both in the projector's hparams — neither is in the gguf
let GEMMA4V_ROPE_THETA = 100.0
let private GEMMA4V_MERGE_DEFAULT = 3l

// per-block GEMM order — the clamp table and the block GEMM plane stride follow it (public: the
// Metal tower driver walks the same layout)
let GEMMA4V_GEMM_Q = 0l
let GEMMA4V_GEMM_K = 1l
let GEMMA4V_GEMM_V = 2l
let GEMMA4V_GEMM_O = 3l
let GEMMA4V_GEMM_GATE = 4l
let GEMMA4V_GEMM_UP = 5l
let GEMMA4V_GEMM_DOWN = 6l
let GEMMA4V_GEMMS = 7l
let GEMMA4V_CLAMP_F = 5l   // imin imax omin omax active, per GEMM, in the f32 blob (public: the suite's clamp knockout reads the layout)

struct Gemma4vTower {
    n_embd : int64       // 768 — the tower width
    n_layer : int64      // 16
    n_head : int64       // 12 (head width n_embd / n_head = 64)
    n_ff : int64         // 3072
    proj_dim : int64     // mm.input_projection output width — the soft-token width (decoder width)
    patch : int64        // 16 px
    patch_dim : int64    // patch² × 3 — the im2col row (768)
    merge : int64        // pooling kernel (3): one soft token per merge×merge patches
    pos_entries : int64  // pos-table rows per axis (10240)
    eps : float          // meta layer_norm_epsilon — every RMS norm in the tower
    // Per-GEMM plane choice follows each SOURCE tensor's on-disk type (the shipped "BF16" mmproj keeps
    // the patch conv in F32): a BF16 tensor is the file's own halfwords in wblob, an F32 one lives
    // in blob. The 112 block GEMMs share one flag — a file mixing types across blocks is refused.
    patch_bf16 : bool
    blk_bf16 : bool      // what the FILE holds for the block GEMMs
    proj_bf16 : bool
    has_std : bool       // std_bias/std_scale present (absent on E2B — then the standardize step is skipped)
    // q8: the 112 block GEMMs SERVE as Q8_0 planes (transcoded at read, the gemma4a recipe) and
    // layer_w indexes qblob/qscales; the patch conv and the projection stay on their file planes
    q8 : bool
    blob : PlaneF = PlaneF()
    wblob : PlaneU16 = PlaneU16()
    qblob : PlaneI8 = PlaneI8()
    qscales : PlaneF = PlaneF()
    patch_w : int64      // [n_embd × patch_dim]
    pos_w : int64        // [2][pos_entries][n_embd]: x table then y table
    std_bias : int64; std_scale : int64   // [n_embd] each, when has_std
    proj_w : int64       // [proj_dim × n_embd]
    proj_clamp : int64   // GEMMA4V_CLAMP_F floats in blob
    layer_f : int64      // per-block f32 region base; stride layer_f_stride: ln1 ln2 attn_post ffn_post [n_embd], q_norm k_norm [hd], clamps [7×5]
    layer_f_stride : int64
    layer_w : int64      // per-block GEMM region base (qblob when q8, else wblob when blk_bf16, else blob); stride layer_w_stride
    layer_w_stride : int64
    image_map : void?         // non-null when the planes borrow a mapped .dlim (dasllama_image)
    image_bytes : uint64
}

//! Per-encode scratch, caller-owned like the audio EncoderState; sized to the largest canvas seen.
struct Gemma4vState {
    @exact_size x0 : array<float>    // [npos × patch_dim] im2col rows (scaled to [-1, 1])
    @exact_size x : array<float>     // residual stream [npos × d]
    @exact_size xb : array<float>    // norm / projection out [npos × d]
    @exact_size xb2 : array<float>   // attention out [npos × d]
    @exact_size xc : array<float>    // clamped GEMM input [npos × max(d, ff)]
    @exact_size qq : array<float>
    @exact_size kk : array<float>
    @exact_size vv : array<float>
    @exact_size hp : array<float>    // attention per-head packs
    @scratch @exact_size att : array<float>   // attention score blocks
    @exact_size hg : array<float>    // ffn gate, then the GEGLU product [npos × ff]
    @exact_size hu : array<float>    // ffn up [npos × ff]
    @exact_size pooled : array<float>  // [nout × d]
    @scratch @exact_size xqi : array<int8>    // q8 lane: the requantized GEMM input [npos × max(d, ff)], grown per site
    @scratch @exact_size xsi : array<float>   // its per-32-block scales
    @exact_size cos_x : array<float>; @exact_size sin_x : array<float>   // rope tables for the current grid
    @exact_size cos_y : array<float>; @exact_size sin_y : array<float>
    @scratch pos_x : array<int64>; @scratch pos_y : array<int64>   // the grid's patch columns / rows
    grid_w : int64       // the grid the tables were built for (0 = none yet)
    grid_h : int64
}

// the tower owns exactly one thing — its image backing. Every plane borrows.
def finalize(var t : Gemma4vTower) {
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

def finalize(var s : Gemma4vState) {
    delete s.x0
    delete s.x
    delete s.xb
    delete s.xb2
    delete s.xc
    delete s.qq
    delete s.kk
    delete s.vv
    delete s.hp
    delete s.att
    delete s.hg
    delete s.hu
    delete s.pooled
    delete s.xqi
    delete s.xsi
    delete s.cos_x
    delete s.sin_x
    delete s.cos_y
    delete s.sin_y
    delete s.pos_x
    delete s.pos_y
}

// the tripwire: serialize_image_meta(Gemma4vTower) covers 25 fields; image_map + image_bytes
// are deliberate skips (the four planes are sections, not meta).
let private GEMMA4V_META_FIELDS = 25 + 2

// every meta-carried Gemma4vTower field, explicitly, both directions (see dasllama_image)
def private serialize_image_meta(var arch : Archive; var t : Gemma4vTower) {
    verify(count_meta_fields(t) == GEMMA4V_META_FIELDS)   // grew Gemma4vTower? extend this list
    arch |> serialize_raw(t.n_embd)
    arch |> serialize_raw(t.n_layer)
    arch |> serialize_raw(t.n_head)
    arch |> serialize_raw(t.n_ff)
    arch |> serialize_raw(t.proj_dim)
    arch |> serialize_raw(t.patch)
    arch |> serialize_raw(t.patch_dim)
    arch |> serialize_raw(t.merge)
    arch |> serialize_raw(t.pos_entries)
    arch |> serialize_raw(t.eps)
    arch |> serialize_raw(t.patch_bf16)
    arch |> serialize_raw(t.blk_bf16)
    arch |> serialize_raw(t.proj_bf16)
    arch |> serialize_raw(t.has_std)
    arch |> serialize_raw(t.q8)
    arch |> serialize_raw(t.patch_w)
    arch |> serialize_raw(t.pos_w)
    arch |> serialize_raw(t.std_bias)
    arch |> serialize_raw(t.std_scale)
    arch |> serialize_raw(t.proj_w)
    arch |> serialize_raw(t.proj_clamp)
    arch |> serialize_raw(t.layer_f)
    arch |> serialize_raw(t.layer_f_stride)
    arch |> serialize_raw(t.layer_w)
    arch |> serialize_raw(t.layer_w_stride)
}

[unused_argument(t)]
def private image_post_load(var t : Gemma4vTower) {}   // no derived state — offsets ARE the meta

//! The mint-side carrier: `t` holds every scalar the meta blob carries, the arrays beside it hold
//! the plane bytes until the writer has them (section names must match the served plane fields —
//! bind-by-name). Public so a suite can check a served tower against a non-image-backed read.
struct Gemma4vStaging {
    t : Gemma4vTower = Gemma4vTower()
    blob : array<float>
    wblob : array<uint16>
    qblob : array<int8>       // q8 lane: the block GEMMs as Q8_0 (repacked for the active backend)
    qscales : array<float>
}

def private serialize_image_meta(var arch : Archive; var st : Gemma4vStaging) {
    serialize_image_meta(arch, st.t)
}

// staging owns its plane bytes outright — every array here is heap, none is borrowed
def finalize(var st : Gemma4vStaging) {
    delete st.blob
    delete st.wblob
    delete st.qblob
    delete st.qscales
    delete st.t
}

[unused_argument(st)]
def private image_post_load(var st : Gemma4vStaging) {}

// two lanes: "gemma4v-q8" serves the block GEMMs as Q8_0 (the CPU serving default, the gemma4a
// recipe); "gemma4v" keeps every plane as the file has it (the parity rail, the Metal leg's source)
let private GEMMA4V_TAG = "gemma4v"
let private GEMMA4V_TAG_Q8 = "gemma4v-q8"

[init]
def private register_gemma4v_image_tags {
    register_image_family_tag(GEMMA4V_TAG)
    register_image_family_tag(GEMMA4V_TAG_Q8)
}

// ===== the GPU blocks hook (registered by dasllama_metal_tower at [init]; requires-back). It takes
// s.x after the stem (rope tables built), runs every block, leaves the result in s.x; false =
// declined, the CPU loop is the fallback. `serves` = would the driver serve (the lane policy asks)
typedef Gemma4vGpuBlocksFn = function<(t : Gemma4vTower; var s : Gemma4vState; npos : int64) : bool>
typedef Gemma4vGpuServesFn = function<() : bool>
var private g_g4v_gpu : Gemma4vGpuBlocksFn
var private g_g4v_gpu_serves : Gemma4vGpuServesFn
var private g_g4v_gpu_set = false

def register_gemma4v_gpu(fn : Gemma4vGpuBlocksFn; serves : Gemma4vGpuServesFn) {
    g_g4v_gpu = fn
    g_g4v_gpu_serves = serves
    g_g4v_gpu_set = true
}

// The lane policy: q8 is the CPU serving default; a GPU blocks driver that will serve wants the
// file's planes (it declines q8), so its presence + knob flips the default to the exact lane.
// A pin (set_gemma4v_q8) overrides either way — the parity rails and the q8 gate pin explicitly.
var private g_g4v_q8_pin = -1   // -1 unset, 0 exact planes, 1 q8

def private g4v_serve_q8() : bool {
    if (g_g4v_q8_pin >= 0) {
        return g_g4v_q8_pin == 1
    }
    return !(g_g4v_gpu_set && invoke(g_g4v_gpu_serves))
}

//! Pin the gemma4v tower's block-GEMM format for subsequent loads: q8 (the CPU serving default)
//! vs the file's own planes (the parity rail, and what the Metal tower serves). Each lane is its
//! own prepared image. ``reset_gemma4v_q8`` returns to the policy default.
def set_gemma4v_q8(on : bool) {
    g_g4v_q8_pin = on ? 1 : 0
}

def reset_gemma4v_q8() {
    g_g4v_q8_pin = -1
}

//! The lane the next load takes under the current pin/policy (true = q8).
def gemma4v_q8_default() : bool => g4v_serve_q8()

def private g4v_tag() : string => g4v_serve_q8() ? GEMMA4V_TAG_Q8 : GEMMA4V_TAG

def private g4v_ensure(@scratch var a : array<float>; n : int64) {
    if (long_length(a) < n) {
        a |> reserve(n)
        a |> resize(n)
    }
}

def private g4v_expect(m : GGUFMeta; name : string; want : int64) {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("dasLLAMA gemma4v: mmproj is missing tensor '{name}'")
    }
    let n = m.tensors[ti].n_elem
    if (n != want) {
        panic("dasLLAMA gemma4v: tensor '{name}' has {n} elements, expected {want}")
    }
}

// GEMM offsets ride the halfword cursor on a bf16 load, the fp32 cursor otherwise — the field then
// indexes whichever plane the mm route reads
def private g4v_gemm_off(bf16 : bool; var o : int64&; var wo : int64&; size : int64) : int64 {
    if (bf16) {
        let at = wo
        wo += size
        return at
    }
    let at = o
    o += size
    return at
}

// GEMM read: a bf16 load copies the file's own halfwords (exact, no conversion); an fp32 load
// reads it like any plain tensor
def private g4v_read_gemm(m : GGUFMeta; b : array<uint8> | #; name : string; var st : Gemma4vStaging;
                          bf16 : bool; off, n_elem : int64) {
    if (bf16) {
        gguf_transcode_bf16(m, b, name, st.wblob, off, n_elem)
    } else {
        gguf_read_tensor_f32(m, b, name, st.blob, off, n_elem)
    }
}

// the clamp's five floats into the f32 blob at `off` (read back by g4v_clamp_at)
def private g4v_put_clamp(var blob : array<float>; off : int64; c : Clamp) {
    blob[off] = c.imin
    blob[off + 1l] = c.imax
    blob[off + 2l] = c.omin
    blob[off + 3l] = c.omax
    blob[off + 4l] = c.active ? 1.0 : 0.0
}

def private g4v_clamp_at(t : Gemma4vTower; off : int64) : Clamp {
    var c = Clamp()
    c.imin = t.blob[off]
    c.imax = t.blob[off + 1l]
    c.omin = t.blob[off + 2l]
    c.omax = t.blob[off + 3l]
    c.active = t.blob[off + 4l] != 0.0
    return c
}

let private G4V_BLK_GEMM_NAMES <- [ "attn_q", "attn_k", "attn_v", "attn_out", "ffn_gate", "ffn_up", "ffn_down" ]

// every tensor the forward reads, by name and element count, before any layout is committed
def private g4v_verify_tensors(m : GGUFMeta; t : Gemma4vTower; gemm_sizes : array<int64>) {
    let d = t.n_embd
    let hd = d / t.n_head
    g4v_expect(m, "v.patch_embd.weight", t.patch_dim * d)
    g4v_expect(m, "mm.input_projection.weight", d * t.proj_dim)
    if (t.has_std) {
        g4v_expect(m, "v.std_bias", d)
        g4v_expect(m, "v.std_scale", d)
    }
    for (l in range64(t.n_layer)) {
        let p = "v.blk.{l}"
        g4v_expect(m, "{p}.ln1.weight", d)
        g4v_expect(m, "{p}.ln2.weight", d)
        g4v_expect(m, "{p}.attn_post_norm.weight", d)
        g4v_expect(m, "{p}.ffn_post_norm.weight", d)
        g4v_expect(m, "{p}.attn_q_norm.weight", hd)
        g4v_expect(m, "{p}.attn_k_norm.weight", hd)
        for (g in range64(GEMMA4V_GEMMS)) {
            let name = "{p}.{G4V_BLK_GEMM_NAMES[g]}.weight"
            g4v_expect(m, name, gemm_sizes[g])
            if ((gguf_tensor_type(m, name) == GGML_TYPE_BF16) != t.blk_bf16) {
                panic("dasLLAMA gemma4v: '{name}' is not the same element type as v.blk.0.attn_q.weight - mixed block GEMM types are not supported")
            }
        }
    }
}

// the plane layouts: fp32 cursor `off` (blob), halfword cursor `woff` (wblob), q8 cursor `qo`
// (qblob; scales at qo/32); returns the cursors as the plane sizes
def private g4v_layout(var t : Gemma4vTower; var off : int64&; var woff : int64&; var qo : int64&) {
    let d = t.n_embd
    let ff = t.n_ff
    let hd = d / t.n_head
    t.patch_w = g4v_gemm_off(t.patch_bf16, off, woff, t.patch_dim * d)
    t.pos_w = off; off += 2l * t.pos_entries * d
    if (t.has_std) {
        t.std_bias = off; off += d
        t.std_scale = off; off += d
    }
    t.proj_w = g4v_gemm_off(t.proj_bf16, off, woff, d * t.proj_dim)
    t.proj_clamp = off; off += GEMMA4V_CLAMP_F
    t.layer_f_stride = 4l * d + 2l * hd + GEMMA4V_GEMMS * GEMMA4V_CLAMP_F
    t.layer_f = off; off += t.n_layer * t.layer_f_stride
    t.layer_w_stride = 4l * d * d + 3l * d * ff
    if (t.q8) {
        t.layer_w = qo
        qo += t.n_layer * t.layer_w_stride
    } else {
        t.layer_w = g4v_gemm_off(t.blk_bf16, off, woff, t.n_layer * t.layer_w_stride)
    }
}

// the reads, in layout order — the per-block f32 region is ln1 ln2 attn_post ffn_post q_norm k_norm
// then the seven clamps; the GEMM region follows G4V_BLK_GEMM_NAMES
def private g4v_read_planes(m : GGUFMeta; bytes : array<uint8> | #; var st : Gemma4vStaging; gemm_sizes : array<int64>) {
    let d = st.t.n_embd
    let hd = d / st.t.n_head
    var wscratch : array<float>   // the q8 transcode's f32 staging, one GEMM at a time
    g4v_read_gemm(m, bytes, "v.patch_embd.weight", st, st.t.patch_bf16, st.t.patch_w, st.t.patch_dim * d)
    gguf_read_tensor_f32(m, bytes, "v.position_embd.weight", st.blob, st.t.pos_w, 2l * st.t.pos_entries * d)
    if (st.t.has_std) {
        gguf_read_tensor_f32(m, bytes, "v.std_bias", st.blob, st.t.std_bias, d)
        gguf_read_tensor_f32(m, bytes, "v.std_scale", st.blob, st.t.std_scale, d)
    }
    g4v_read_gemm(m, bytes, "mm.input_projection.weight", st, st.t.proj_bf16, st.t.proj_w, d * st.t.proj_dim)
    g4v_put_clamp(st.blob, st.t.proj_clamp, read_clamp(m, bytes, "mm.input_projection"))
    for (l in range64(st.t.n_layer)) {
        let p = "v.blk.{l}"
        var fo = st.t.layer_f + l * st.t.layer_f_stride
        gguf_read_tensor_f32(m, bytes, "{p}.ln1.weight", st.blob, fo, d); fo += d
        gguf_read_tensor_f32(m, bytes, "{p}.ln2.weight", st.blob, fo, d); fo += d
        gguf_read_tensor_f32(m, bytes, "{p}.attn_post_norm.weight", st.blob, fo, d); fo += d
        gguf_read_tensor_f32(m, bytes, "{p}.ffn_post_norm.weight", st.blob, fo, d); fo += d
        gguf_read_tensor_f32(m, bytes, "{p}.attn_q_norm.weight", st.blob, fo, hd); fo += hd
        gguf_read_tensor_f32(m, bytes, "{p}.attn_k_norm.weight", st.blob, fo, hd); fo += hd
        var wo = st.t.layer_w + l * st.t.layer_w_stride
        for (g in range64(GEMMA4V_GEMMS)) {
            let base = "{p}.{G4V_BLK_GEMM_NAMES[g]}"
            if (st.t.q8) {
                g4v_read_gemm_q8(m, bytes, "{base}.weight", st, wscratch, wo, gemm_sizes[g])
            } else {
                g4v_read_gemm(m, bytes, "{base}.weight", st, st.t.blk_bf16, wo, gemm_sizes[g])
            }
            wo += gemm_sizes[g]
            g4v_put_clamp(st.blob, fo, read_clamp(m, bytes, base))
            fo += GEMMA4V_CLAMP_F
        }
    }
}

// q8 GEMM read: the file's tensor (bf16 or f32) widens to f32 scratch, then quantizes to Q8_0
// straight into qblob/qscales at the 32-aligned region (wo/32 indexes the scales)
def private g4v_read_gemm_q8(m : GGUFMeta; b : array<uint8> | #; name : string; var st : Gemma4vStaging;
                             @scratch var wscratch : array<float>; wo, n_elem : int64) {
    if (n_elem % 32l != 0l || wo % 32l != 0l) {
        panic("dasLLAMA gemma4v q8: GEMM tensor '{name}' ({n_elem} elements at {wo}) is not 32-aligned")
    }
    if (long_length(wscratch) < n_elem) {
        wscratch |> reserve(n_elem)
        wscratch |> resize(n_elem)
    }
    gguf_read_tensor_f32(m, b, name, wscratch, 0l, n_elem)
    quantize_q8_0_into(wscratch, n_elem, st.qblob, st.qscales, wo, wo / 32l)
}

// repack every block GEMM region for the active backend's q8q8 kernels (n = the region's input
// width — per-row alignment, not just n_elem, keeps blocks from straddling rows); without a
// selected backend matmul_q8q8_batch reads the flat Q8_0 image as-is
def private g4v_repack_all(var st : Gemma4vStaging; gemm_sizes : array<int64>) {
    if (!st.t.q8 || !select_matmul_backend_for_load_()) {
        return
    }
    let d = st.t.n_embd
    let ff = st.t.n_ff
    let widths <- [ d, d, d, d, d, d, ff ]   // q k v o gate up down: the input width of each
    for (l in range64(st.t.n_layer)) {
        var wo = st.t.layer_w + l * st.t.layer_w_stride
        for (g in range64(GEMMA4V_GEMMS)) {
            repack_q8q8_weight(st.qblob, st.qscales, wo, widths[g], gemm_sizes[g] / widths[g])
            wo += gemm_sizes[g]
        }
    }
}

//! Read the tower off an mmproj gguf into owned staging arrays — the mint side of the image rail,
//! the only path that touches the source file. GEMMs go to `wblob` verbatim when the file holds
//! them as BF16, to `blob` when fp32; conv, pos tables, norms and clamp sidecars are fp32 in `blob`.
def stage_gemma4v_tower(path : string) : Gemma4vStaging {
    var st = Gemma4vStaging()
    st.t.q8 = g4v_serve_q8()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open mmproj gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        let proj = gguf_str(m, bytes, "clip.vision.projector_type")
        if (proj != "gemma4v") {
            panic("dasLLAMA: mmproj projector '{proj}' is not gemma4v (the dense-family gemma4uv embedder is dasllama_gemma4uv)")
        }
        st.t.merge = GEMMA4V_MERGE_DEFAULT
        if (gguf_has(m, "clip.vision.projector.scale_factor")) {
            st.t.merge = gguf_int(m, bytes, "clip.vision.projector.scale_factor")
        }
        st.t.patch = gguf_int(m, bytes, "clip.vision.patch_size")
        st.t.patch_dim = st.t.patch * st.t.patch * 3l
        st.t.n_embd = gguf_int(m, bytes, "clip.vision.embedding_length")
        st.t.n_layer = gguf_int(m, bytes, "clip.vision.block_count")
        st.t.n_head = gguf_int(m, bytes, "clip.vision.attention.head_count")
        st.t.n_ff = gguf_int(m, bytes, "clip.vision.feed_forward_length")
        st.t.proj_dim = gguf_int(m, bytes, "clip.vision.projection_dim")
        st.t.eps = gguf_f32(m, bytes, "clip.vision.attention.layer_norm_epsilon")
        let d = st.t.n_embd
        let ff = st.t.n_ff
        if (d <= 0l || st.t.n_head <= 0l || d % st.t.n_head != 0l || (d / st.t.n_head) % 4l != 0l) {
            panic("dasLLAMA gemma4v: embedding_length {d} / head_count {st.t.n_head} - the head width must be a positive multiple of 4 (two rope axes of pairs)")
        }
        let pos_ti = gguf_find_tensor(m, "v.position_embd.weight")
        if (pos_ti < 0) {
            panic("dasLLAMA gemma4v: mmproj is missing tensor 'v.position_embd.weight'")
        }
        if (m.tensors[pos_ti].n_elem % (2l * d) != 0l) {
            panic("dasLLAMA gemma4v: 'v.position_embd.weight' has {m.tensors[pos_ti].n_elem} elements - not 2 x k x embedding_length ({d}); a truncated row would silently misindex")
        }
        st.t.pos_entries = m.tensors[pos_ti].n_elem / (2l * d)
        st.t.patch_bf16 = gguf_tensor_type(m, "v.patch_embd.weight") == GGML_TYPE_BF16
        st.t.proj_bf16 = gguf_tensor_type(m, "mm.input_projection.weight") == GGML_TYPE_BF16
        st.t.blk_bf16 = gguf_tensor_type(m, "v.blk.0.attn_q.weight") == GGML_TYPE_BF16
        st.t.has_std = gguf_find_tensor(m, "v.std_bias") >= 0 && gguf_find_tensor(m, "v.std_scale") >= 0
        let gemm_sizes <- [ d * d, d * d, d * d, d * d, d * ff, d * ff, ff * d ]
        g4v_verify_tensors(m, st.t, gemm_sizes)
        var off = 0l
        var woff = 0l
        var qo = 0l
        g4v_layout(st.t, off, woff, qo)
        st.blob |> reserve_resize(off)
        st.wblob |> reserve_resize(woff)
        st.qblob |> reserve_resize(qo)
        st.qscales |> reserve_resize(qo / 32l)
        g4v_read_planes(m, bytes, st, gemm_sizes)
        g4v_repack_all(st, gemm_sizes)
    }
    fclose(f)
    return <- st
}

//! Load the gemma4v tower, image-backed: a prepared `.dlim` maps and every plane borrows it, so
//! there is no allocation and no processing at load; missing or stale, one is minted from the
//! source mmproj first. Panics on any other projector.
def load_gemma4v_tower(path : string) : Gemma4vTower {
    var tag = g4v_tag()
    if (path |> ends_with(".dlim")) {
        // named directly — no mmproj to regenerate from, so a wrong identity panics; the lane is
        // the file's (its baked tag), not the knob's: a served tower reads its q8 flag from the meta
        let baked = image_family_tag(path)
        if (baked == GEMMA4V_TAG || baked == GEMMA4V_TAG_Q8) {
            tag = baked
        }
        apply_box_profile_runtime()
        let ts_img = ref_time_ticks()
        var t = Gemma4vTower()
        if (!load_image(path, t, tag)) {
            panic("dasLLAMA: '{path}' is not a prepared gemma4v-tower image for this box/knobs (identity {image_identity(tag)}) - regenerate it from the source mmproj gguf")
        }
        to_log(LOG_INFO, "dasLLAMA: prepared gemma4v-tower image mapped in {get_time_usec(ts_img) / 1000} ms - {path}\n")
        g4v_check_planes(t, path)
        return <- t
    }
    if (g_env_engine.image) {
        apply_box_profile_runtime()   // a backend pin must precede the identity (it names the backend)
        let img = image_path_for(path, tag)
        let ts_img = ref_time_ticks()
        var t = Gemma4vTower()
        if (load_image(img, t, tag)) {
            to_log(LOG_INFO, "dasLLAMA: prepared gemma4v-tower image mapped in {get_time_usec(ts_img) / 1000} ms - {img}\n")
            g4v_check_planes(t, img)
            return <- t
        }
        delete t
        var inscope st <- stage_gemma4v_tower(path)
        var built = Gemma4vTower()
        if (cache_via_image_staged(st, built, img, tag)) {
            dlim_gc_stale(path, tag)   // the fresh bake ON DISK proves its lane's siblings dead; an in-memory build reaps nothing
        }
        g4v_check_planes(built, img)
        return <- built
    }
    // no persisted image: mint one in memory and serve THAT — a served tower is always
    // image-backed, so there is no second, array-owning shape of this struct to maintain
    var inscope st <- stage_gemma4v_tower(path)
    var built = Gemma4vTower()
    mint_gemma4v_tower(st, built)
    g4v_check_planes(built, path)
    return <- built
}

//! Mint a served tower from a staged read WITHOUT touching disk — the off-rail control for suites
//! (a knockout edits the staging planes, then serves the result through the same image walk).
def mint_gemma4v_tower(var st : Gemma4vStaging; var out : Gemma4vTower) {
    cache_via_image_staged(st, out, "", st.t.q8 ? GEMMA4V_TAG_Q8 : GEMMA4V_TAG)
}

// the bind walk skips a missing section silently (a pure-fp32 image legitimately has no wblob),
// so the flag=>plane invariant must be checked once the carrier is live, or the kernel derefs null
def private g4v_check_planes(t : Gemma4vTower; src : string) {
    if ((t.patch_bf16 || (t.blk_bf16 && !t.q8) || t.proj_bf16) && empty(t.wblob)) {
        panic("dasLLAMA gemma4v: '{src}' declares bf16 planes but carries no wblob section - the image is corrupt or hand-edited")
    }
    if (t.q8 && (empty(t.qblob) || empty(t.qscales))) {
        panic("dasLLAMA gemma4v: '{src}' declares q8 block GEMMs but carries no qblob/qscales sections - the image is corrupt or hand-edited")
    }
}

// One GEMM, on whichever plane the tower's offsets index. Both arms are the same arithmetic:
// dot_bf16's widen is an exact high-half shift, so the bf16 arm reads half the bytes for the
// identical result.
def private g4v_gemm(var y : array<float>; t : Gemma4vTower; bf16 : bool; off : int64;
                     x : array<float>; n, d, npos : int64) {
    if (bf16) {
        mm_bf16_b(y, t.wblob, off, x, n, d, npos)
    } else {
        mm_blob_b(y, t.blob, off, x, n, d, npos)
    }
}

// the q8 route: per-site requant of the (clamped) input, then the q8q8 batch kernel
def private g4v_gemm_q8(var y : array<float>; t : Gemma4vTower; var s : Gemma4vState; off : int64;
                        x : array<float>; n, d, npos : int64) {
    let need = npos * n
    if (long_length(s.xqi) < need) {
        s.xqi |> reserve(need)
        s.xqi |> resize(need)
        s.xsi |> reserve(need / 32l)
        s.xsi |> resize(need / 32l)
    }
    requant_rows_q8(x, n, npos, s.xqi, s.xsi)
    matmul_q8q8_batch(y, t.qblob, t.qscales, off, s.xqi, s.xsi, n, d, npos)
}

// mtmd's ClippableLinear: y = W · clamp(x); then clamp(y) — the identity GEMM when the file has
// no sidecar scalars for this weight. `q8` routes a BLOCK region through the Q8_0 planes.
def private g4v_gemm_clamped(var y : array<float>; t : Gemma4vTower; var s : Gemma4vState; bf16 : bool; off : int64;
                             x : array<float>; n, d, npos : int64; c : Clamp; q8 : bool = false) {
    if (!c.active) {
        if (q8) {
            g4v_gemm_q8(y, t, s, off, x, n, d, npos)
        } else {
            g4v_gemm(y, t, bf16, off, x, n, d, npos)
        }
        return
    }
    g4v_ensure(s.xc, n * npos)
    clamp_rows(s.xc, x, n * npos, c.imin, c.imax)
    if (q8) {
        g4v_gemm_q8(y, t, s, off, s.xc, n, d, npos)
    } else {
        g4v_gemm(y, t, bf16, off, s.xc, n, d, npos)
    }
    clamp_rows(y, y, d * npos, c.omin, c.omax)
}

// the per-axis rope tables for an n_cols × n_rows patch grid, rebuilt only when the grid changes:
// row i's x-table row is its column, its y-table row its row (pos_x = i % n_cols, pos_y = i / n_cols)
def private g4v_rope_tables(t : Gemma4vTower; var s : Gemma4vState; n_cols, n_rows : int64) {
    if (s.grid_w == n_cols && s.grid_h == n_rows) {
        return
    }
    let npos = n_cols * n_rows
    let hd = t.n_embd / t.n_head
    if (long_length(s.pos_x) < npos) {   // inline: PERF026 can't see @scratch through reserve_resize
        s.pos_x |> reserve(npos)
        s.pos_x |> resize(npos)
        s.pos_y |> reserve(npos)
        s.pos_y |> resize(npos)
    }
    for (i in range64(npos)) {
        s.pos_x[i] = i % n_cols
        s.pos_y[i] = i / n_cols
    }
    var no_ff : array<float>
    build_rope_tabs_rows(s.cos_x, s.sin_x, GEMMA4V_ROPE_THETA, 1.0, 1.0, no_ff, false, s.pos_x, npos, hd / 2l)
    build_rope_tabs_rows(s.cos_y, s.sin_y, GEMMA4V_ROPE_THETA, 1.0, 1.0, no_ff, false, s.pos_y, npos, hd / 2l)
    s.grid_w = n_cols
    s.grid_h = n_rows
}

//! One block's offsets (elements): the f32 rows in ``blob``, the seven GEMM regions in the block
//! GEMM plane (``qblob`` when q8, else ``wblob``/``blob``), the seven clamp records in ``blob``
//! (GEMMA4V_GEMM_* order). The CPU loop and the Metal driver both walk the layout through this.
struct Gemma4vBlockOffs {
    ln1 : int64
    ln2 : int64
    attn_post : int64
    ffn_post : int64
    q_norm : int64
    k_norm : int64
    w : int64[7]
    clamp : int64[7]
}

def gemma4v_block_offs(t : Gemma4vTower; l : int64) : Gemma4vBlockOffs {
    let d = t.n_embd
    let ff = t.n_ff
    let hd = d / t.n_head
    let fo = t.layer_f + l * t.layer_f_stride
    var o = Gemma4vBlockOffs(ln1 = fo, ln2 = fo + d, attn_post = fo + 2l * d, ffn_post = fo + 3l * d,
        q_norm = fo + 4l * d, k_norm = fo + 4l * d + hd)
    let clamps = fo + 4l * d + 2l * hd
    let wo = t.layer_w + l * t.layer_w_stride
    let sizes = fixed_array<int64>(d * d, d * d, d * d, d * d, d * ff, d * ff, ff * d)
    var at = wo
    for (g in range(7)) {
        o.w[g] = at
        at += sizes[g]
        o.clamp[g] = clamps + int64(g) * GEMMA4V_CLAMP_F
    }
    return o
}

// one pre-norm block over the residual stream s.x (npos rows)
def private g4v_block(t : Gemma4vTower; var s : Gemma4vState; l, npos : int64) {
    let d = t.n_embd
    let ff = t.n_ff
    let hd = d / t.n_head
    let o = gemma4v_block_offs(t, l)
    let ln1 = o.ln1
    let ln2 = o.ln2
    let attn_post = o.attn_post
    let ffn_post = o.ffn_post
    let q_norm = o.q_norm
    let k_norm = o.k_norm
    let w_q = o.w[GEMMA4V_GEMM_Q]
    let w_k = o.w[GEMMA4V_GEMM_K]
    let w_v = o.w[GEMMA4V_GEMM_V]
    let w_o = o.w[GEMMA4V_GEMM_O]
    let w_gate = o.w[GEMMA4V_GEMM_GATE]
    let w_up = o.w[GEMMA4V_GEMM_UP]
    let w_down = o.w[GEMMA4V_GEMM_DOWN]
    // attention (the g4v.* buckets feed asr_prof_report - the encode split, the gemma4a shape)
    var tp = ref_time_ticks()
    rms_rows(s.xb, s.x, t.blob, ln1, d, npos, t.eps)
    asr_prof_add("g4v.norm", tp)
    tp = ref_time_ticks()
    g4v_gemm_clamped(s.qq, t, s, t.blk_bf16, w_q, s.xb, d, d, npos, g4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_Q]), t.q8)
    g4v_gemm_clamped(s.kk, t, s, t.blk_bf16, w_k, s.xb, d, d, npos, g4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_K]), t.q8)
    g4v_gemm_clamped(s.vv, t, s, t.blk_bf16, w_v, s.xb, d, d, npos, g4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_V]), t.q8)
    asr_prof_add("g4v.attn_mm", tp)
    tp = ref_time_ticks()
    rms_rows(s.qq, s.qq, t.blob, q_norm, hd, npos * t.n_head, t.eps)
    rms_rows(s.kk, s.kk, t.blob, k_norm, hd, npos * t.n_head, t.eps)
    rms_rows_weightless(s.vv, hd, npos * t.n_head, t.eps)
    rope_neox_2d_rows(s.qq, npos, t.n_head, hd, s.cos_x, s.sin_x, s.cos_y, s.sin_y)
    rope_neox_2d_rows(s.kk, npos, t.n_head, hd, s.cos_x, s.sin_x, s.cos_y, s.sin_y)
    asr_prof_add("g4v.qkv_norm_rope", tp)
    tp = ref_time_ticks()
    attention_bidir(s.xb2, s.qq, s.kk, s.vv, s.hp, s.att, npos, d, t.n_head, 1.0)   // kq_scale = 1: the ViT attends unscaled
    asr_prof_add("g4v.attn_core", tp)
    tp = ref_time_ticks()
    g4v_gemm_clamped(s.xb, t, s, t.blk_bf16, w_o, s.xb2, d, d, npos, g4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_O]), t.q8)
    asr_prof_add("g4v.attn_mm", tp)
    tp = ref_time_ticks()
    rms_rows(s.xb, s.xb, t.blob, attn_post, d, npos, t.eps)
    add_inplace_rows(s.x, s.xb, npos * d)
    // ffn
    rms_rows(s.xb, s.x, t.blob, ln2, d, npos, t.eps)
    asr_prof_add("g4v.norm", tp)
    tp = ref_time_ticks()
    g4v_gemm_clamped(s.hg, t, s, t.blk_bf16, w_gate, s.xb, d, ff, npos, g4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_GATE]), t.q8)
    g4v_gemm_clamped(s.hu, t, s, t.blk_bf16, w_up, s.xb, d, ff, npos, g4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_UP]), t.q8)
    asr_prof_add("g4v.ffn_mm", tp)
    tp = ref_time_ticks()
    geglu_quick_lut_rows(s.hg, s.hg, s.hu, ff, npos)
    asr_prof_add("g4v.geglu", tp)
    tp = ref_time_ticks()
    g4v_gemm_clamped(s.xb, t, s, t.blk_bf16, w_down, s.hg, ff, d, npos, g4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_DOWN]), t.q8)
    asr_prof_add("g4v.ffn_mm", tp)
    tp = ref_time_ticks()
    rms_rows(s.xb, s.xb, t.blob, ffn_post, d, npos, t.eps)
    add_inplace_rows(s.x, s.xb, npos * d)
    asr_prof_add("g4v.norm", tp)
}

// the stem: im2col → x·2−1 → patch conv → x/y position-table adds, leaving the residual stream in
// s.x (npos rows); sizes every block buffer for this canvas
def private g4v_stem(t : Gemma4vTower; var s : Gemma4vState; planes : array<float>; w, h, n_cols : int64) : int64 {
    let d = t.n_embd
    let npos = im2col_rgb_patches(planes, w, h, t.patch, s.x0)
    // inp_raw_scaled: pixels [0, 1] -> [-1, 1] ahead of the conv
    unsafe {
        var xp = addr(s.x0[0])
        for (i in range64(npos * t.patch_dim)) {
            xp[i] = xp[i] * 2.0 - 1.0
        }
    }
    g4v_ensure(s.x, npos * d)
    g4v_ensure(s.xb, npos * d)
    g4v_ensure(s.xb2, npos * d)
    g4v_ensure(s.qq, npos * d)
    g4v_ensure(s.kk, npos * d)
    g4v_ensure(s.vv, npos * d)
    g4v_ensure(s.hg, npos * t.n_ff)
    g4v_ensure(s.hu, npos * t.n_ff)
    g4v_gemm(s.x, t, t.patch_bf16, t.patch_w, s.x0, t.patch_dim, d, npos)
    unsafe {
        var xp = addr(s.x[0])
        let tbl = plane_at(t.blob, t.pos_w)
        for (pi in range64(0l, npos)) {
            var row = xp + pi * d
            let ex = tbl + (pi % n_cols) * d
            let ey = tbl + (t.pos_entries + pi / n_cols) * d
            for (i in range64(0l, d)) {
                row[i] = (row[i] + ex[i]) + ey[i]
            }
        }
    }
    return npos
}

// the tail: Gemma4VisionPooler (merge×merge mean, ×√d), the std guard, then the
// Gemma4MultimodalEmbedder (weightless rms → clamped projection) into out; returns nout
def private g4v_tail(t : Gemma4vTower; var s : Gemma4vState; n_cols, n_rows : int64; var out : array<float>) : int64 {
    let d = t.n_embd
    let nout = avg_pool2d_rows(s.pooled, s.x, n_cols, n_rows, d, t.merge)
    unsafe {
        var pp = addr(s.pooled[0])
        let sc = sqrt(float(d))
        for (i in range64(nout * d)) {
            pp[i] *= sc
        }
        if (t.has_std) {   // (x - std_bias) * std_scale per row — absent on E2B
            let bp = plane_at(t.blob, t.std_bias)
            let sp = plane_at(t.blob, t.std_scale)
            for (r in range64(nout)) {
                var row = pp + r * d
                for (i in range64(d)) {
                    row[i] = (row[i] - bp[i]) * sp[i]
                }
            }
        }
    }
    rms_rows_weightless(s.pooled, d, nout, t.eps)
    g4v_ensure(out, nout * t.proj_dim)
    g4v_gemm_clamped(out, t, s, t.proj_bf16, t.proj_w, s.pooled, d, t.proj_dim, nout, g4v_clamp_at(t, t.proj_clamp))
    return nout
}

//! Encode a letterboxed, normalized canvas (`vision_normalize` planes, w×h) into `out` =
//! [nout × proj_dim] soft tokens. Returns nout = (w/48)×(h/48) (patch 16 × merge 3).
[hot_path]
def gemma4v_encode(t : Gemma4vTower; var s : Gemma4vState; planes : array<float>; w, h : int64;
                   var out : array<float>) : int64 {
    let span = t.patch * t.merge
    if (w % span != 0l || h % span != 0l) {
        panic("dasLLAMA gemma4v: canvas {w}x{h} is not a multiple of the {span}px pooled patch - letterbox first")
    }
    // the learned position table holds pos_entries per axis; a wider grid would read past it
    if (w / t.patch > t.pos_entries || h / t.patch > t.pos_entries) {
        panic("dasLLAMA gemma4v: canvas {w}x{h} needs a {w / t.patch}x{h / t.patch} position grid but the mmproj table holds {t.pos_entries} per axis - lower DASLLAMA_VISION_MAX_TOKENS")
    }
    let n_cols = w / t.patch
    let n_rows = h / t.patch
    var tp = ref_time_ticks()
    let npos = g4v_stem(t, s, planes, w, h, n_cols)
    g4v_rope_tables(t, s, n_cols, n_rows)
    asr_prof_add("g4v.stem", tp)
    var gpu_served = false
    if (g_g4v_gpu_set) {
        gpu_served = invoke(g_g4v_gpu, t, s, npos)
    }
    if (!gpu_served) {
        for (l in range64(t.n_layer)) {
            g4v_block(t, s, l, npos)
        }
    }
    tp = ref_time_ticks()
    let nout = g4v_tail(t, s, n_cols, n_rows, out)
    asr_prof_add("g4v.tail", tp)
    return nout
}

//! The whole image path in one call: geometry → letterbox → normalize → `gemma4v_encode` into
//! `out` = nout × proj_dim soft tokens (returns nout); `tag` names the `DASLLAMA_VISION_DUMP`
//! artifact. `add_user_image` is this plus queueing onto a chat turn.
def encode_image_(t : Gemma4vTower; var s : Gemma4vState; img : VisionImage; tag : string;
                  var out : array<float>) : int64 {
    if (img.width <= 0 || img.height <= 0) {
        panic("dasLLAMA gemma4v: encode_image_ got an empty image (decode failed?)")
    }
    let min_tok = g_env_vision.vision_min_tokens
    let max_tok = g_env_vision.vision_max_tokens
    if (min_tok < 1 || max_tok < min_tok) {
        panic("dasLLAMA gemma4v: DASLLAMA_VISION_MIN_TOKENS={min_tok} / DASLLAMA_VISION_MAX_TOKENS={max_tok} - need 1 <= min <= max")
    }
    let align = int(t.patch * t.merge)
    let target = vision_target_size(img.width, img.height, align, min_tok, max_tok)
    var inscope boxed <- vision_letterbox(img, target)
    maybe_dump_vision(boxed, tag)
    var inscope planes <- vision_normalize(boxed)
    return gemma4v_encode(t, s, planes, int64(target.x), int64(target.y), out)
}
