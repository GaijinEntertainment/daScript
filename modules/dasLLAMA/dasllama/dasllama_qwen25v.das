options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_qwen25v shared public

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
require dasllama/dasllama_tower   // shared encoder-tower pieces: RMS rows, bias, attention, im2col
require dasllama/dasllama_rope    // build_rope_tabs_vision — the (h, w) ladder table
require dasllama/dasllama_vision   // encode_image_: geometry + letterbox + normalize ahead of the tower
require dasllama/dasllama_env     // g_env_engine.image — the dlim-rail switch
require math

//! The qwen2.5o vision tower (Qwen2.5-Omni / Qwen2.5-VL): temporal-pair conv folded at load →
//! WINDOW-sorted merge reorder (112 px windows of 2×2-merged groups) → 32 RMS-normed blocks
//! (separate q/k/v with biases, vision-mrope, gated-silu FFN; attention is block-diagonal per
//! window except every ``n_wa_pattern``-th block, which attends in full) → RMS post-norm →
//! 2×2 merger MLP (mm.0 → GELU → mm.2), then the window UN-sort back to group-row-major
//! order. Rope-only positions — no learned table. No clamp — the files carry no
//! ClippableLinear sidecars. Oracle: llama-mtmd-debug -p encode on the f32-widened mmproj
//! (`qwen3vl_plan.md` slice I, dumps in models/qwen3vl-vision-oracle/).

let QWEN25V_ROPE_THETA = 10000.0   // fixed for the family - not in the gguf
let QWEN25V_MIN_TOKENS = 8
let QWEN25V_MAX_TOKENS = 4096
let QWEN25V_WINDOW_PX_DEFAULT = 112l   // the attn_window_size default when the key is absent

// per-block GEMM order — the block GEMM plane stride follows it
let QWEN25V_GEMM_Q = 0l
let QWEN25V_GEMM_K = 1l
let QWEN25V_GEMM_V = 2l
let QWEN25V_GEMM_O = 3l
let QWEN25V_GEMM_GATE = 4l
let QWEN25V_GEMM_UP = 5l
let QWEN25V_GEMM_DOWN = 6l
let QWEN25V_GEMMS = 7l

struct Qwen25vTower {
    n_embd : int64       // 1280 — the tower width
    n_layer : int64      // 32
    n_head : int64       // 16 (head width 80)
    n_ff : int64         // the gated hidden (3420 on the 3B) — from the TENSOR dims, the metadata lies
    ff_pad : int64       //! the SERVED ffn width: n_ff rounded up to 64 (3456 on the 3B)
    proj_dim : int64     // the merger's output width — the soft-token width (decoder width)
    patch : int64        // 14 px
    patch_dim : int64    // patch² × 3 — the im2col row (588)
    merge : int64        // spatial merge (2)
    n_wa_pattern : int64 // full attention every n-th block ((l+1) % n == 0); 0 = all-full
    win_px : int64       // the attention window side in px (112)
    eps : float
    img_mean : float3    // CLIP per-channel normalize
    img_std : float3
    blk_bf16 : bool      //! the file's GEMM element type - the twin's dispatch family only; every GEMM stages f32-in-blob
    mm_bf16 : bool
    blob : PlaneF = PlaneF()
    wblob : PlaneU16 = PlaneU16()   //! the baked halfword GEMM twin ([[tower_bake_half_twin]]); empty = route-ineligible
    patch_w : int64      // [n_embd × q25v_patch_pad] f32 — W0 + W1 folded, padded rows (no patch bias, no pos table)
    post_ln_w : int64    // [n_embd] RMS weight
    mm0_w : int64        // [4·n_embd → 4·n_embd]
    mm0_b : int64
    mm2_w : int64        // [4·n_embd → proj_dim]
    mm2_b : int64
    layer_f : int64      //! per-block f32 region; q25v_layout names the members of layer_f_stride
    layer_f_stride : int64
    layer_w : int64      //! per-block GEMM region, f32-in-blob; stride layer_w_stride (the FFN trio at ff_pad)
    layer_w_stride : int64
    image_map : void?
    image_bytes : uint64
}

//! Per-encode scratch, caller-owned; sized to the largest canvas seen.
struct Qwen25vState {
    @exact_size x0 : array<float>    // [npos × q25v_patch_pad] im2col rows (zero pad lanes)
    @exact_size xn : array<float>    // conv out, natural patch order [npos × d]
    @exact_size x : array<float>     // residual stream, WINDOW-sorted [npos × d]
    @exact_size xb : array<float>    // norm / GEMM out [npos × d]
    @exact_size xb2 : array<float>   // attention out [npos × d]
    @exact_size qq : array<float>
    @exact_size kk : array<float>
    @exact_size vv : array<float>
    @exact_size hp : array<float>
    @scratch @exact_size att : array<float>
    @exact_size hg : array<float>    // ffn gate [npos × ff]
    @exact_size hu : array<float>    // ffn up [npos × ff]
    @exact_size m0 : array<float>    // merger mid [nout × 4d]
    @exact_size mout : array<float>  // merger out, window-sorted [nout × proj] — un-sorted into the caller's out
    @exact_size cos_t : array<float>; @exact_size sin_t : array<float>
    @scratch pos2 : array<int2>      // per sorted row: (patch row, patch column)
    @scratch src_of : array<int64>   // per sorted row: source patch index in conv order
    @scratch win_lo : array<int64>   // per window: first sorted PATCH row (win_lo[i]..win_lo[i+1])
    @scratch unsort : array<int64>   // per group-row-major merged row: its window-sorted row
    grid_w : int64
    grid_h : int64
}

def finalize(var t : Qwen25vTower) {
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

def finalize(var s : Qwen25vState) {
    delete s.x0
    delete s.xn
    delete s.x
    delete s.xb
    delete s.xb2
    delete s.qq
    delete s.kk
    delete s.vv
    delete s.hp
    delete s.att
    delete s.hg
    delete s.hu
    delete s.m0
    delete s.mout
    delete s.cos_t
    delete s.sin_t
    delete s.pos2
    delete s.src_of
    delete s.win_lo
    delete s.unsort
}

// serialize_image_meta(Qwen25vTower) covers 26 fields; image_map + image_bytes are deliberate
// skips (the two planes are sections, not meta)
let private QWEN25V_META_FIELDS = 26 + 2

def private serialize_image_meta(var arch : Archive; var t : Qwen25vTower) {
    verify(count_meta_fields(t) == QWEN25V_META_FIELDS)   // grew Qwen25vTower? extend this list
    arch |> serialize_raw(t.n_embd)
    arch |> serialize_raw(t.n_layer)
    arch |> serialize_raw(t.n_head)
    arch |> serialize_raw(t.n_ff)
    arch |> serialize_raw(t.ff_pad)
    arch |> serialize_raw(t.proj_dim)
    arch |> serialize_raw(t.patch)
    arch |> serialize_raw(t.patch_dim)
    arch |> serialize_raw(t.merge)
    arch |> serialize_raw(t.n_wa_pattern)
    arch |> serialize_raw(t.win_px)
    arch |> serialize_raw(t.eps)
    arch |> serialize_raw(t.img_mean)
    arch |> serialize_raw(t.img_std)
    arch |> serialize_raw(t.blk_bf16)
    arch |> serialize_raw(t.mm_bf16)
    arch |> serialize_raw(t.patch_w)
    arch |> serialize_raw(t.post_ln_w)
    arch |> serialize_raw(t.mm0_w)
    arch |> serialize_raw(t.mm0_b)
    arch |> serialize_raw(t.mm2_w)
    arch |> serialize_raw(t.mm2_b)
    arch |> serialize_raw(t.layer_f)
    arch |> serialize_raw(t.layer_f_stride)
    arch |> serialize_raw(t.layer_w)
    arch |> serialize_raw(t.layer_w_stride)
}

[unused_argument(t)]
def private image_post_load(var t : Qwen25vTower) {}

//! The mint-side carrier (see dasllama_gemma4v's staging notes — section names bind by name).
struct Qwen25vStaging {
    t : Qwen25vTower = Qwen25vTower()
    blob : array<float>
    wblob : array<uint16>
}

def private serialize_image_meta(var arch : Archive; var st : Qwen25vStaging) {
    serialize_image_meta(arch, st.t)
}

def finalize(var st : Qwen25vStaging) {
    delete st.blob
    delete st.wblob
    delete st.t
}

[unused_argument(st)]
def private image_post_load(var st : Qwen25vStaging) {}

let private QWEN25V_TAG = "qwen25v"

[init]
def private register_qwen25v_image_tags {
    register_image_family_tag(QWEN25V_TAG)
}

// the block GEMM tensors in QWEN25V_GEMM_* order; every per-GEMM list follows it
let private Q25V_BLK_GEMM_NAMES <- [ "attn_q", "attn_k", "attn_v", "attn_out", "ffn_gate", "ffn_up", "ffn_down" ]

// every block/merger GEMM tensor the halfword twin route serves. A sample of two would let a
// mixed file bake a twin the route reads as one type.
[arch(at="../ARCHITECTURE_IMAGE.md#image-tower-twin-plane")]
def private q25v_gemm_types_uniform(m : GGUFMeta; n_layer : int64; gt : int) : bool {
    if (gguf_tensor_type(m, "mm.0.weight") != gt || gguf_tensor_type(m, "mm.2.weight") != gt) {
        return false
    }
    for (l in range64(n_layer)) {
        for (nm in Q25V_BLK_GEMM_NAMES) {
            if (gguf_tensor_type(m, "v.blk.{l}.{nm}.weight") != gt) {
                return false
            }
        }
    }
    return true
}

//! The conv/attention GEMM K width served padded — %64 fits the Metal tiles; zero pads, exact.
[arch(at="../ARCHITECTURE_MEDIA.md#tower-padded-widths")]
def q25v_patch_pad(t : Qwen25vTower) : int64 => (t.patch_dim + 63l) / 64l * 64l

def private q25v_gemm_sizes(d, ffp : int64) : array<int64> {
    return <- [ d * d, d * d, d * d, d * d, d * ffp, d * ffp, ffp * d ]
}

// the SERVED bias element counts (gate/up biases at ff_pad, zero tails)
def private q25v_bias_sizes(d, ffp : int64) : array<int64> {
    return <- [ d, d, d, d, ffp, ffp, d ]
}

def private q25v_verify_tensors(m : GGUFMeta; t : Qwen25vTower) {
    let d = t.n_embd
    let ff = t.n_ff
    gguf_expect_elems(m, "v.patch_embd.weight", t.patch_dim * d, "dasLLAMA qwen25v")
    gguf_expect_elems(m, "v.patch_embd.weight.1", t.patch_dim * d, "dasLLAMA qwen25v")
    gguf_expect_elems(m, "v.post_ln.weight", d, "dasLLAMA qwen25v")
    gguf_expect_elems(m, "mm.0.weight", 16l * d * d, "dasLLAMA qwen25v")
    gguf_expect_elems(m, "mm.0.bias", 4l * d, "dasLLAMA qwen25v")
    gguf_expect_elems(m, "mm.2.weight", 4l * d * t.proj_dim, "dasLLAMA qwen25v")
    gguf_expect_elems(m, "mm.2.bias", t.proj_dim, "dasLLAMA qwen25v")
    let file_gemms <- [ d * d, d * d, d * d, d * d, d * ff, d * ff, ff * d ]
    let file_biases <- [ d, d, d, d, ff, ff, d ]
    for (l in range64(t.n_layer)) {
        let p = "v.blk.{l}"
        gguf_expect_elems(m, "{p}.ln1.weight", d, "dasLLAMA qwen25v")
        gguf_expect_elems(m, "{p}.ln2.weight", d, "dasLLAMA qwen25v")
        for (g in range64(QWEN25V_GEMMS)) {
            let name = "{p}.{Q25V_BLK_GEMM_NAMES[g]}.weight"
            gguf_expect_elems(m, name, file_gemms[g], "dasLLAMA qwen25v")
            gguf_expect_elems(m, "{p}.{Q25V_BLK_GEMM_NAMES[g]}.bias", file_biases[g], "dasLLAMA qwen25v")
            if ((gguf_tensor_type(m, name) == GGML_TYPE_BF16) != t.blk_bf16) {
                panic("dasLLAMA qwen25v: '{name}' ({gguf_tensor_type(m, name)}) is not the element type of v.blk.0.attn_q.weight ({t.blk_bf16 ? GGML_TYPE_BF16 : GGML_TYPE_F32}) - mixed block GEMM types are not supported")
            }
        }
    }
}

[unused_argument(woff), arch(at="../ARCHITECTURE_MEDIA.md#tower-padded-widths")]   // no wblob cursor: every GEMM stages f32-in-blob; woff keeps the shared layout signature
def private q25v_layout(var t : Qwen25vTower; var off : int64&; var woff : int64&) {
    let d = t.n_embd
    let ffp = t.ff_pad
    t.patch_w = off; off += q25v_patch_pad(t) * d
    t.post_ln_w = off; off += d
    t.mm0_w = off; off += 16l * d * d
    t.mm0_b = off; off += 4l * d
    t.mm2_w = off; off += 4l * d * t.proj_dim
    t.mm2_b = off; off += t.proj_dim
    t.layer_f_stride = 7l * d + 2l * ffp   // ln1_w, ln2_w, q_b, k_b, v_b, out_b, down_b [d ×7]; gate_b, up_b [ffp ×2]
    t.layer_f = off; off += t.n_layer * t.layer_f_stride
    t.layer_w_stride = 4l * d * d + 3l * d * ffp
    t.layer_w = off
    off += t.n_layer * t.layer_w_stride
}

def private q25v_read_planes(m : GGUFMeta; bytes : array<uint8> | #; var st : Qwen25vStaging; gemm_sizes : array<int64>) {
    let d = st.t.n_embd
    let ff = st.t.n_ff
    let fp = st.t.ff_pad
    var wscratch : array<float>     // the folded patch-conv W; block GEMMs stage through tower_stage_f32_*
    var rowscratch : array<float>   // the down GEMM's unpadded file rows ahead of the expand
    wscratch |> reserve_resize(st.t.patch_dim * d)
    tower_read_conv_pair_folded(m, bytes, "v.patch_embd.weight", wscratch, 0l, st.t.patch_dim * d)
    tower_stage_f32_pad_row_stride(st.blob, wscratch, st.t.patch_w, d, st.t.patch_dim, q25v_patch_pad(st.t))
    gguf_read_tensor_f32(m, bytes, "v.post_ln.weight", st.blob, st.t.post_ln_w, d)
    gguf_read_tensor_f32(m, bytes, "mm.0.weight", st.blob, st.t.mm0_w, 16l * d * d)
    gguf_read_tensor_f32(m, bytes, "mm.0.bias", st.blob, st.t.mm0_b, 4l * d)
    gguf_read_tensor_f32(m, bytes, "mm.2.weight", st.blob, st.t.mm2_w, 4l * d * st.t.proj_dim)
    gguf_read_tensor_f32(m, bytes, "mm.2.bias", st.blob, st.t.mm2_b, st.t.proj_dim)
    let file_biases <- [ d, d, d, d, ff, ff, d ]
    for (l in range64(st.t.n_layer)) {
        let p = "v.blk.{l}"
        var fo = st.t.layer_f + l * st.t.layer_f_stride
        gguf_read_tensor_f32(m, bytes, "{p}.ln1.weight", st.blob, fo, d); fo += d
        gguf_read_tensor_f32(m, bytes, "{p}.ln2.weight", st.blob, fo, d); fo += d
        for (g in range64(QWEN25V_GEMMS)) {
            gguf_read_tensor_f32(m, bytes, "{p}.{Q25V_BLK_GEMM_NAMES[g]}.bias", st.blob, fo, file_biases[g])
            if (g == QWEN25V_GEMM_GATE || g == QWEN25V_GEMM_UP) {
                tower_zero_span(st.blob, fo + ff, fp - ff)
                fo += fp
            } else {
                fo += file_biases[g]
            }
        }
        var wo = st.t.layer_w + l * st.t.layer_w_stride
        for (g in range64(QWEN25V_GEMMS)) {
            let wname = "{p}.{Q25V_BLK_GEMM_NAMES[g]}.weight"
            if (g == QWEN25V_GEMM_GATE || g == QWEN25V_GEMM_UP) {
                tower_stage_f32_zero_rows(m, bytes, wname, st.blob, wo, ff, fp, d)
            } elif (g == QWEN25V_GEMM_DOWN) {
                tower_stage_f32_pad_cols(m, bytes, wname, st.blob, rowscratch, wo, d, ff, fp)
            } else {
                gguf_read_tensor_f32(m, bytes, wname, st.blob, wo, gemm_sizes[g])
            }
            wo += gemm_sizes[g]
        }
    }
}

//! Read the tower off an mmproj gguf into owned staging arrays — the mint side of the image rail.
[arch(at="../ARCHITECTURE_IMAGE.md#image-tower-twin-plane")]
def stage_qwen25v_tower(path : string) : Qwen25vStaging {
    var st = Qwen25vStaging()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open mmproj gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        let proj_key = gguf_has(m, "clip.vision.projector_type") ? "clip.vision.projector_type" : "clip.projector_type"
        let proj = gguf_str(m, bytes, proj_key)
        if (proj != "qwen2.5o" && proj != "qwen2.5vl_merger" && proj != "qwen25v") {
            panic("dasLLAMA: mmproj projector '{proj}' is not qwen2.5o / qwen2.5vl_merger")
        }
        st.t.patch = gguf_int(m, bytes, "clip.vision.patch_size")
        st.t.patch_dim = st.t.patch * st.t.patch * 3l
        st.t.n_embd = gguf_int(m, bytes, "clip.vision.embedding_length")
        st.t.n_layer = gguf_int(m, bytes, "clip.vision.block_count")
        st.t.n_head = gguf_int(m, bytes, "clip.vision.attention.head_count")
        st.t.proj_dim = gguf_int(m, bytes, "clip.vision.projection_dim")
        st.t.eps = gguf_f32(m, bytes, "clip.vision.attention.layer_norm_epsilon")
        st.t.n_wa_pattern = gguf_int(m, bytes, "clip.vision.n_wa_pattern")
        st.t.win_px = QWEN25V_WINDOW_PX_DEFAULT
        if (gguf_has(m, "clip.vision.attn_window_size")) {
            st.t.win_px = gguf_int(m, bytes, "clip.vision.attn_window_size")
        }
        var mean <- gguf_f32_array(m, bytes, "clip.vision.image_mean")
        var stdv <- gguf_f32_array(m, bytes, "clip.vision.image_std")
        if (length(mean) != 3 || length(stdv) != 3) {
            panic("dasLLAMA qwen25v: image_mean/image_std must carry 3 channels")
        }
        st.t.img_mean = float3(mean[0], mean[1], mean[2])
        st.t.img_std = float3(stdv[0], stdv[1], stdv[2])
        delete mean
        delete stdv
        st.t.merge = 2l
        if (gguf_has(m, "clip.vision.spatial_merge_size")) {
            st.t.merge = gguf_int(m, bytes, "clip.vision.spatial_merge_size")
        }
        let d = st.t.n_embd
        if (d <= 0l || st.t.n_head <= 0l || d % st.t.n_head != 0l || (d / st.t.n_head) % 4l != 0l) {
            panic("dasLLAMA qwen25v: embedding_length {d} / head_count {st.t.n_head} - the head width must be a positive multiple of 4 (two vision-rope ladders of pairs)")
        }
        if (st.t.merge != 2l) {
            panic("dasLLAMA qwen25v: spatial_merge_size {st.t.merge} - only the 2x2 merger is implemented")
        }
        if (st.t.win_px % (st.t.patch * st.t.merge) != 0l) {
            panic("dasLLAMA qwen25v: attention window {st.t.win_px}px is not a multiple of the {st.t.patch * st.t.merge}px merged patch")
        }
        // the gated hidden comes from the TENSOR dims — the metadata's feed_forward_length lies
        let gi = gguf_find_tensor(m, "v.blk.0.ffn_gate.weight")
        if (gi < 0) {
            panic("dasLLAMA qwen25v: mmproj is missing tensor 'v.blk.0.ffn_gate.weight' (the gated-silu FFN)")
        }
        st.t.n_ff = m.tensors[gi].n_elem / d
        st.t.ff_pad = (st.t.n_ff + 63l) / 64l * 64l
        st.t.blk_bf16 = gguf_tensor_type(m, "v.blk.0.attn_q.weight") == GGML_TYPE_BF16
        st.t.mm_bf16 = gguf_tensor_type(m, "mm.0.weight") == GGML_TYPE_BF16
        let gemm_sizes <- q25v_gemm_sizes(d, st.t.ff_pad)
        q25v_verify_tensors(m, st.t)
        var off = 0l
        var woff = 0l
        q25v_layout(st.t, off, woff)
        st.blob |> reserve_resize(off)
        st.wblob |> reserve_resize(woff)
        q25v_read_planes(m, bytes, st, gemm_sizes)
        if (q25v_gemm_types_uniform(m, st.t.n_layer, st.t.blk_bf16 ? GGML_TYPE_BF16 : GGML_TYPE_F16)) {
            tower_bake_half_twin(st.blob, st.wblob, st.t.blk_bf16)
        }
    }
    fclose(f)
    return <- st
}

//! Load the qwen25v tower, image-backed (the qwen3v load shape: a prepared `.dlim` maps and
//! every plane borrows; missing or stale, one is minted from the source mmproj).
def load_qwen25v_tower(path : string) : Qwen25vTower {
    // the lane census every family tower load prints; this tower has a single tag, so the
    // line says so rather than leaving qwen25v the one family whose lane never appears
    to_log(LOG_INFO, "dasLLAMA qwen25v: tower GEMM lane f32 planes - exact-only, no q8 lane exists\n")
    if (path |> ends_with(".dlim")) {
        apply_box_profile_runtime()
        let ts_img = ref_time_ticks()
        var t = Qwen25vTower()
        if (!load_image(path, t, QWEN25V_TAG)) {
            panic("dasLLAMA: '{path}' is not a prepared qwen25v-tower image for this box/knobs (identity {image_identity(QWEN25V_TAG)}) - regenerate it from the source mmproj gguf")
        }
        to_log(LOG_INFO, "dasLLAMA: prepared qwen25v-tower image mapped in {get_time_usec(ts_img) / 1000} ms - {path}\n")
        return <- t
    }
    if (g_env_engine.image) {
        apply_box_profile_runtime()
        let img = image_path_for(path, QWEN25V_TAG)
        let ts_img = ref_time_ticks()
        var t = Qwen25vTower()
        if (load_image(img, t, QWEN25V_TAG)) {
            to_log(LOG_INFO, "dasLLAMA: prepared qwen25v-tower image mapped in {get_time_usec(ts_img) / 1000} ms - {img}\n")
            return <- t
        }
        delete t
        var inscope st <- stage_qwen25v_tower(path)
        var built = Qwen25vTower()
        if (cache_via_image_staged(st, built, img, QWEN25V_TAG)) {
            dlim_gc_stale(path, QWEN25V_TAG)
        }
        return <- built
    }
    var inscope st <- stage_qwen25v_tower(path)
    var built = Qwen25vTower()
    mint_qwen25v_tower(st, built)
    return <- built
}

//! Mint a served tower from a staged read WITHOUT touching disk — the off-rail control for suites.
def mint_qwen25v_tower(var st : Qwen25vStaging; var out : Qwen25vTower) {
    cache_via_image_staged(st, out, "", QWEN25V_TAG)
}

//! One block's offsets (elements): the f32 rows in ``blob``, the seven GEMM regions in the
//! block GEMM plane (QWEN25V_GEMM_* order).
struct Qwen25vBlockOffs {
    ln1_w : int64
    ln2_w : int64
    b : int64[7]
    w : int64[7]
}

def qwen25v_block_offs(t : Qwen25vTower; l : int64) : Qwen25vBlockOffs {
    let d = t.n_embd
    let fo = t.layer_f + l * t.layer_f_stride
    var o = Qwen25vBlockOffs(ln1_w = fo, ln2_w = fo + d)
    let bsizes <- q25v_bias_sizes(d, t.ff_pad)
    var bat = fo + 2l * d
    for (g in range64(QWEN25V_GEMMS)) {
        o.b[g] = bat
        bat += bsizes[g]
    }
    let sizes <- q25v_gemm_sizes(d, t.ff_pad)
    var at = t.layer_w + l * t.layer_w_stride
    for (g in range64(QWEN25V_GEMMS)) {
        o.w[g] = at
        at += sizes[g]
    }
    return o
}

// the window-sorted walk + per-window ranges for a pw × ph patch grid, rebuilt on grid change:
// windows (row-major over merged groups, ragged edges kept) → groups local row-major → the
// 2×2 (sy, sx) sub-order; positions and the merger un-sort derive from the same walk
def private q25v_grid_tables(t : Qwen25vTower; var s : Qwen25vState; pw, ph : int64) {
    if (s.grid_w == pw && s.grid_h == ph) {
        return
    }
    let npos = pw * ph
    let mw = pw / 2l
    let mh = ph / 2l
    let gw = t.win_px / t.patch / t.merge
    s.pos2 |> resize(npos)
    s.src_of |> resize(npos)
    s.unsort |> resize(mw * mh)
    s.win_lo |> clear()
    var r = 0l
    var g_sorted = 0l
    var wy = 0l
    while (wy < mh) {
        var wx = 0l
        while (wx < mw) {
            s.win_lo |> push(r)
            let wh = min(gw, mh - wy)
            let ww = min(gw, mw - wx)
            for (dy in range64(wh)) {
                for (dx in range64(ww)) {
                    let gy = wy + dy
                    let gx = wx + dx
                    s.unsort[gy * mw + gx] = g_sorted
                    g_sorted ++
                    for (sub in range64(4l)) {
                        let sy = gy * 2l + sub / 2l
                        let sx = gx * 2l + sub % 2l
                        s.pos2[r] = int2(int(sy), int(sx))
                        s.src_of[r] = sy * pw + sx
                        r ++
                    }
                }
            }
            wx += gw
        }
        wy += gw
    }
    s.win_lo |> push(npos)
    let hd = t.n_embd / t.n_head
    build_rope_tabs_vision(s.cos_t, s.sin_t, QWEN25V_ROPE_THETA, s.pos2, npos, hd)
    s.grid_w = pw
    s.grid_h = ph
}

// one RMS-normed block over the window-sorted residual stream s.x (npos rows); non-full
// layers attend per contiguous window range, full layers ((l+1) % n_wa_pattern == 0) in full
def private q25v_block(t : Qwen25vTower; var s : Qwen25vState; l, npos : int64) {
    let d = t.n_embd
    let ffp = t.ff_pad
    let hd = d / t.n_head
    let o = qwen25v_block_offs(t, l)
    var tp = ref_time_ticks()
    rms_rows(s.xb, s.x, t.blob, o.ln1_w, d, npos, t.eps)
    asr_prof_add("q25v.norm", tp)
    tp = ref_time_ticks()
    mm_plane_b(s.qq, false, t.wblob, t.blob, o.w[QWEN25V_GEMM_Q], s.xb, d, d, npos)
    add_bias_rows(s.qq, t.blob, o.b[QWEN25V_GEMM_Q], d, npos)
    mm_plane_b(s.kk, false, t.wblob, t.blob, o.w[QWEN25V_GEMM_K], s.xb, d, d, npos)
    add_bias_rows(s.kk, t.blob, o.b[QWEN25V_GEMM_K], d, npos)
    mm_plane_b(s.vv, false, t.wblob, t.blob, o.w[QWEN25V_GEMM_V], s.xb, d, d, npos)
    add_bias_rows(s.vv, t.blob, o.b[QWEN25V_GEMM_V], d, npos)
    asr_prof_add("q25v.attn_mm", tp)
    tp = ref_time_ticks()
    rope_neox_tab_rows(s.qq, npos, t.n_head, hd, s.cos_t, s.sin_t)
    rope_neox_tab_rows(s.kk, npos, t.n_head, hd, s.cos_t, s.sin_t)
    asr_prof_add("q25v.qkv_rope", tp)
    tp = ref_time_ticks()
    let full = t.n_wa_pattern <= 0l || (l + 1l) % t.n_wa_pattern == 0l
    if (full) {
        attention_bidir(s.xb2, s.qq, s.kk, s.vv, s.hp, s.att, npos, d, t.n_head, 1.0 / sqrt(float(hd)))
    } else {
        attention_bidir_windows(s.xb2, s.qq, s.kk, s.vv, s.hp, s.att, s.win_lo, d, t.n_head, 1.0 / sqrt(float(hd)))
    }
    asr_prof_add("q25v.attn_core", tp)
    tp = ref_time_ticks()
    mm_plane_b(s.xb, false, t.wblob, t.blob, o.w[QWEN25V_GEMM_O], s.xb2, d, d, npos)
    add_bias_rows(s.xb, t.blob, o.b[QWEN25V_GEMM_O], d, npos)
    add_inplace_rows(s.x, s.xb, npos * d)
    asr_prof_add("q25v.attn_mm", tp)
    tp = ref_time_ticks()
    rms_rows(s.xb, s.x, t.blob, o.ln2_w, d, npos, t.eps)
    asr_prof_add("q25v.norm", tp)
    tp = ref_time_ticks()
    mm_plane_b(s.hg, false, t.wblob, t.blob, o.w[QWEN25V_GEMM_GATE], s.xb, d, ffp, npos)
    add_bias_rows(s.hg, t.blob, o.b[QWEN25V_GEMM_GATE], ffp, npos)
    mm_plane_b(s.hu, false, t.wblob, t.blob, o.w[QWEN25V_GEMM_UP], s.xb, d, ffp, npos)
    add_bias_rows(s.hu, t.blob, o.b[QWEN25V_GEMM_UP], ffp, npos)
    silu4_batch(s.hg, ffp, npos)
    unsafe {
        var gp = addr(s.hg[0])
        let up = addr<float const?>(s.hu[0])
        for (i in range64(npos * ffp)) {
            gp[i] *= up[i]
        }
    }
    mm_plane_b(s.xb, false, t.wblob, t.blob, o.w[QWEN25V_GEMM_DOWN], s.hg, ffp, d, npos)
    add_bias_rows(s.xb, t.blob, o.b[QWEN25V_GEMM_DOWN], d, npos)
    add_inplace_rows(s.x, s.xb, npos * d)
    asr_prof_add("q25v.ffn", tp)
}

//! the stem's column half, both routes: im2col at the padded stride, and s.x0/s.mout sized for the driver
def private q25v_stem_cols(t : Qwen25vTower; var s : Qwen25vState; planes : array<float>; w, h : int64) : int64 {
    let d = t.n_embd
    let npos = im2col_rgb_patches(planes, w, h, t.patch, s.x0, q25v_patch_pad(t))
    ensure_length(s.xn, npos * d)
    ensure_length(s.x, npos * d)
    ensure_length(s.xb, npos * d)
    ensure_length(s.xb2, npos * d)
    ensure_length(s.qq, npos * d)
    ensure_length(s.kk, npos * d)
    ensure_length(s.vv, npos * d)
    ensure_length(s.hg, npos * t.ff_pad)
    ensure_length(s.hu, npos * t.ff_pad)
    ensure_length(s.m0, (npos / 4l) * 4l * d)
    ensure_length(s.mout, (npos / 4l) * t.proj_dim)
    return npos
}

//! the stem's compute half, the CPU route: conv at the padded K, then the window-sorted reorder into s.x
def private q25v_stem_finish(t : Qwen25vTower; var s : Qwen25vState; npos : int64) {
    let d = t.n_embd
    mm_plane_b(s.xn, false, t.wblob, t.blob, t.patch_w, s.x0, q25v_patch_pad(t), d, npos)
    unsafe {
        var xp = addr(s.x[0])
        let np = addr<float const?>(s.xn[0])
        for (r in range64(npos)) {
            copy_floats(xp + r * d, np + s.src_of[r] * d, d)
        }
    }
}

// the tail: RMS post-norm, the 2×2 merger MLP (sorted rows are merge-adjacent, so the ×4
// reshape is flat) into s.mout, window-sorted; the encode owns the un-sort scatter
def private q25v_tail(t : Qwen25vTower; var s : Qwen25vState; npos : int64) {
    let d = t.n_embd
    let nout = npos / 4l
    rms_rows(s.xb, s.x, t.blob, t.post_ln_w, d, npos, t.eps)
    mm_plane_b(s.m0, false, t.wblob, t.blob, t.mm0_w, s.xb, 4l * d, 4l * d, nout)
    add_bias_rows(s.m0, t.blob, t.mm0_b, 4l * d, nout)
    gelu_tanh_lut_batch(s.m0, 4l * d, nout)
    mm_plane_b(s.mout, false, t.wblob, t.blob, t.mm2_w, s.m0, 4l * d, t.proj_dim, nout)
    add_bias_rows(s.mout, t.blob, t.mm2_b, t.proj_dim, nout)
}

//! the GPU hook: conv/gather + every block + the merger tail off s.x0, window-sorted rows into s.mout
typedef Qwen25vGpuBlocksFn = function<(t : Qwen25vTower; var s : Qwen25vState; npos : int64) : bool>
var private g_q25v_gpu : Qwen25vGpuBlocksFn
var private g_q25v_gpu_set = false

[arch(at="../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_qwen25v_gpu(fn : Qwen25vGpuBlocksFn) {
    g_q25v_gpu = fn
    g_q25v_gpu_set = true
}

//! Encode a letterboxed, normalized canvas (``vision_normalize3`` planes, w×h) into ``out`` =
//! [nout × proj_dim] soft tokens; returns nout = (w/28)×(h/28). Oracle fixtures skip the normalize.
[hot_path]
def qwen25v_encode(t : Qwen25vTower; var s : Qwen25vState; planes : array<float>; w, h : int64;
                   var out : array<float>) : int64 {
    let span = t.patch * t.merge
    if (w % span != 0l || h % span != 0l) {
        panic("dasLLAMA qwen25v: canvas {w}x{h} is not a multiple of the {span}px merged patch - letterbox first")
    }
    let pw = w / t.patch
    let ph = h / t.patch
    dispatch_phase_encode(pw * ph)
    var tp = ref_time_ticks()
    q25v_grid_tables(t, s, pw, ph)
    let npos = q25v_stem_cols(t, s, planes, w, h)
    asr_prof_add("q25v.stem", tp)
    var gpu_served = false
    if (g_q25v_gpu_set) {
        tp = ref_time_ticks()
        gpu_served = invoke(g_q25v_gpu, t, s, npos)
        asr_prof_add("q25v.gpu", tp)
    }
    if (!gpu_served) {
        tp = ref_time_ticks()
        q25v_stem_finish(t, s, npos)
        asr_prof_add("q25v.stem", tp)
        for (l in range64(t.n_layer)) {
            q25v_block(t, s, l, npos)
        }
        tp = ref_time_ticks()
        q25v_tail(t, s, npos)
        asr_prof_add("q25v.tail", tp)
    }
    tp = ref_time_ticks()
    let nout = npos / 4l
    ensure_length(out, nout * t.proj_dim)
    unsafe {   //! the window UN-sort: out is group-row-major
        var op = addr(out[0])
        let mp = addr<float const?>(s.mout[0])
        for (g in range64(nout)) {
            copy_floats(op + g * t.proj_dim, mp + s.unsort[g] * t.proj_dim, t.proj_dim)
        }
    }
    asr_prof_add("q25v.tail", tp)
    return nout
}

//! The whole image path in one call: geometry → letterbox → per-channel CLIP normalize →
//! ``qwen25v_encode`` into ``out`` (returns nout). Budget = the family's [8, 4096].
def encode_image_(t : Qwen25vTower; var s : Qwen25vState; img : VisionImage; tag : string;
                  var out : array<float>) : int64 {
    if (img.width <= 0 || img.height <= 0) {
        panic("dasLLAMA qwen25v: encode_image_ got an empty image (decode failed?)")
    }
    let align = int(t.patch * t.merge)
    let target = vision_target_size(img.width, img.height, align, QWEN25V_MIN_TOKENS, QWEN25V_MAX_TOKENS)
    var inscope boxed <- vision_letterbox(img, target)
    maybe_dump_vision(boxed, tag)
    var inscope planes <- vision_normalize3(boxed, t.img_mean, t.img_std)
    return qwen25v_encode(t, s, planes, int64(target.x), int64(target.y), out)
}
