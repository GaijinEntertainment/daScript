options gen2
options _comment_hygiene = true

module dasllama_gemma4uv shared public

require daslib/fio
require strings
require daslib/apply
require daslib/archive
require dasllama/dasllama_gguf
require dasllama/dasllama_common   // apply_box_profile_runtime for the embedder-image identity
require dasllama/dasllama_image   // the dlim rail: mapped planes, no allocation or work at load
require dasllama/dasllama_plane public   // PlaneF/PlaneU16 — re-exported so requirers can delete an embedder
require dasllama/dasllama_audio
require dasllama/dasllama_vision   // encode_image_: geometry + letterbox + normalize ahead of the encoder
require dasllama/dasllama_env     // g_env_vision: the token-budget knobs the geometry reads
require math

//! The gemma4uv "unified" vision embedder (gemma-4 dense; 12B is the tested pair): TEN tensors read (the mmproj
//! file's eleventh, mm.a.input_projection.weight, is the audio projection and never loads), no
//! attention — im2col 48px superpatches → LN → linear+bias → LN → x/y pos-table adds → LN →
//! weightless rms → square projection. The DECODER does the visual reasoning through the
//! non-causal image span; this file only lifts pixels into embedding space
//! (`vision_plan.md` §pipeline; oracle = llama-mtmd-debug -p encode).

// pytorch LayerNorm default, HARDCODED in mtmd's graph builder — NOT the meta's 1e-6,
// which feeds only the pre-projection rms_norm (the two-eps parity trap)
let GEMMA4UV_LN_EPS = 1e-5

struct Gemma4uvEmbedder {
    n_embd : int64       // embedding width out of the patch linear (= decoder width on 12B)
    proj_dim : int64     // mm.input_projection output width — the soft-token width
    patch : int64        // folded superpatch side: patch_size × scale_factor (48)
    patch_dim : int64    // patch² × 3 — the im2col row (6912)
    pos_entries : int64  // pos-table rows per axis (1120)
    rms_eps : float      // meta layer_norm_epsilon — the weightless rms before the projection
    // Per-GEMM plane choice, following each SOURCE tensor's on-disk type — gemma-4's shipped
    // "BF16" mmproj is MIXED (F32 patch embedder, BF16 projection). A BF16 tensor becomes the
    // file's own halfwords in wblob; an F32 one stays in blob, since rounding it down would lose.
    patch_bf16 : bool
    proj_bf16 : bool
    blob : PlaneF = PlaneF()
    wblob : PlaneU16 = PlaneU16()
    patch_w : int64; patch_b : int64
    ln1_w : int64; ln1_b : int64     // v.patch_norm.1 — over raw im2col rows (patch_dim wide)
    ln2_w : int64; ln2_b : int64     // v.patch_norm.2 — after the patch linear
    ln3_w : int64; ln3_b : int64     // v.patch_norm.3 — after the pos adds
    pos_w : int64                    // [2][pos_entries][n_embd]: x table then y table
    proj_w : int64
    image_map : void?         // non-null when the planes borrow a mapped .dlim (dasllama_image)
    image_bytes : uint64
}

//! Per-encode scratch, caller-owned like the audio EncoderState.
struct Gemma4uvState {
    x0 : array<float>    // [npos × patch_dim] im2col rows, then their LN
    x1 : array<float>    // [npos × n_embd]
    x2 : array<float>    // [npos × n_embd]
}

// the embedder owns exactly one thing — its image backing. Every plane borrows.
def finalize(var t : Gemma4uvEmbedder) {
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

// the tripwire: serialize_image_meta(Gemma4uvEmbedder) covers 18 fields; image_map + image_bytes
// are deliberate skips (the two planes are sections, not meta).
let private GEMMA4UV_META_FIELDS = 18 + 2

// every meta-carried Gemma4uvEmbedder field, explicitly, both directions (see dasllama_image)
def private serialize_image_meta(var arch : Archive; var t : Gemma4uvEmbedder) {
    verify(count_meta_fields(t) == GEMMA4UV_META_FIELDS)   // grew Gemma4uvEmbedder? extend this list
    arch |> serialize_raw(t.n_embd)
    arch |> serialize_raw(t.proj_dim)
    arch |> serialize_raw(t.patch)
    arch |> serialize_raw(t.patch_dim)
    arch |> serialize_raw(t.pos_entries)
    arch |> serialize_raw(t.rms_eps)
    arch |> serialize_raw(t.patch_bf16)
    arch |> serialize_raw(t.proj_bf16)
    arch |> serialize_raw(t.patch_w)
    arch |> serialize_raw(t.patch_b)
    arch |> serialize_raw(t.ln1_w)
    arch |> serialize_raw(t.ln1_b)
    arch |> serialize_raw(t.ln2_w)
    arch |> serialize_raw(t.ln2_b)
    arch |> serialize_raw(t.ln3_w)
    arch |> serialize_raw(t.ln3_b)
    arch |> serialize_raw(t.pos_w)
    arch |> serialize_raw(t.proj_w)
}

[unused_argument(t)]
def private image_post_load(var t : Gemma4uvEmbedder) {}   // no derived state — offsets ARE the meta

//! The mint-side carrier: `e` holds every scalar the meta blob carries, the arrays beside it hold
//! the plane bytes until the writer has them (section names must match the served plane fields —
//! bind-by-name). Public so a suite can check a served embedder against a non-image-backed read.
struct Gemma4uvStaging {
    e : Gemma4uvEmbedder = Gemma4uvEmbedder()
    blob : array<float>
    wblob : array<uint16>
}

def private serialize_image_meta(var arch : Archive; var st : Gemma4uvStaging) {
    serialize_image_meta(arch, st.e)
}

// staging owns its plane bytes outright — every array here is heap, none is borrowed
def finalize(var st : Gemma4uvStaging) {
    delete st.blob
    delete st.wblob
    delete st.e
}

[unused_argument(st)]
def private image_post_load(var st : Gemma4uvStaging) {}

// ONE tag: the plane split follows the source file's tensor types, not a knob, and a different
// mmproj already keys a different image path — so there is no second flavor of the same source
// to separate. The meta's two bf16 flags describe the layout completely.
let private GEMMA4UV_TAG = "gemma4uv"

[init]
def private register_gemma4uv_image_tags {
    register_image_family_tag(GEMMA4UV_TAG)
}

def finalize(var s : Gemma4uvState) {
    delete s.x0
    delete s.x1
    delete s.x2
}

def private uv_ensure(@scratch var a : array<float>; n : int64) {
    if (long_length(a) < n) {
        a |> reserve(n)
        a |> resize(n)
    }
}

def private uv_expect(m : GGUFMeta; name : string; want : int64) {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        panic("dasLLAMA gemma4uv: mmproj is missing tensor '{name}'")
    }
    let n = m.tensors[ti].n_elem
    if (n != want) {
        panic("dasLLAMA gemma4uv: tensor '{name}' has {n} elements, expected {want}")
    }
}

//! Read the embedder's ten tensors off an mmproj gguf into owned staging arrays — the
//! mint side of the image rail, and the only path that touches the source file. The two GEMM
//! weights go to `wblob` verbatim when the file holds them as BF16, to `blob` when it holds fp32.
def stage_gemma4uv_embedder(path : string) : Gemma4uvStaging {
    var st = Gemma4uvStaging()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open mmproj gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        let proj = gguf_str(m, bytes, "clip.vision.projector_type")
        if (proj != "gemma4uv") {
            panic("dasLLAMA: mmproj projector '{proj}' is not gemma4uv (the E-series gemma4v ViT tower is a v2 leg)")
        }
        var merge = 3l   // mtmd's pooling_kernel_size default for the gemma-4 family
        if (gguf_has(m, "clip.vision.projector.scale_factor")) {
            merge = gguf_int(m, bytes, "clip.vision.projector.scale_factor")
        }
        st.e.patch = gguf_int(m, bytes, "clip.vision.patch_size") * merge
        st.e.patch_dim = st.e.patch * st.e.patch * 3l
        st.e.n_embd = gguf_int(m, bytes, "clip.vision.embedding_length")
        st.e.proj_dim = gguf_int(m, bytes, "clip.vision.projection_dim")
        st.e.rms_eps = gguf_f32(m, bytes, "clip.vision.attention.layer_norm_epsilon")
        let pos_ti = gguf_find_tensor(m, "v.position_embd.weight")
        if (pos_ti < 0) {
            panic("dasLLAMA gemma4uv: mmproj is missing tensor 'v.position_embd.weight'")
        }
        if (st.e.n_embd <= 0l || m.tensors[pos_ti].n_elem % (2l * st.e.n_embd) != 0l) {
            panic("dasLLAMA gemma4uv: 'v.position_embd.weight' has {m.tensors[pos_ti].n_elem} elements - not 2 x k x embedding_length ({st.e.n_embd}); a truncated row would silently misindex")
        }
        st.e.pos_entries = m.tensors[pos_ti].n_elem / (2l * st.e.n_embd)
        let d = st.e.n_embd
        let pd = st.e.patch_dim
        uv_expect(m, "v.patch_embd.weight", pd * d)
        uv_expect(m, "v.patch_embd.bias", d)
        uv_expect(m, "v.patch_norm.1.weight", pd)
        uv_expect(m, "v.patch_norm.1.bias", pd)
        uv_expect(m, "v.patch_norm.2.weight", d)
        uv_expect(m, "v.patch_norm.2.bias", d)
        uv_expect(m, "v.patch_norm.3.weight", d)
        uv_expect(m, "v.patch_norm.3.bias", d)
        uv_expect(m, "mm.input_projection.weight", d * st.e.proj_dim)
        st.e.patch_bf16 = gguf_tensor_type(m, "v.patch_embd.weight") == GGML_TYPE_BF16
        st.e.proj_bf16 = gguf_tensor_type(m, "mm.input_projection.weight") == GGML_TYPE_BF16
        var off = 0l
        var woff = 0l
        st.e.patch_w = uv_gemm_off(st.e.patch_bf16, off, woff, pd * d)
        st.e.patch_b = off; off += d
        st.e.ln1_w = off; off += pd
        st.e.ln1_b = off; off += pd
        st.e.ln2_w = off; off += d
        st.e.ln2_b = off; off += d
        st.e.ln3_w = off; off += d
        st.e.ln3_b = off; off += d
        st.e.pos_w = off; off += 2l * st.e.pos_entries * d
        st.e.proj_w = uv_gemm_off(st.e.proj_bf16, off, woff, d * st.e.proj_dim)
        st.blob |> reserve_resize(off)
        st.wblob |> reserve_resize(woff)
        uv_read_gemm(m, bytes, "v.patch_embd.weight", st, st.e.patch_bf16, st.e.patch_w, pd * d)
        gguf_read_tensor_f32(m, bytes, "v.patch_embd.bias", st.blob, st.e.patch_b, d)
        gguf_read_tensor_f32(m, bytes, "v.patch_norm.1.weight", st.blob, st.e.ln1_w, pd)
        gguf_read_tensor_f32(m, bytes, "v.patch_norm.1.bias", st.blob, st.e.ln1_b, pd)
        gguf_read_tensor_f32(m, bytes, "v.patch_norm.2.weight", st.blob, st.e.ln2_w, d)
        gguf_read_tensor_f32(m, bytes, "v.patch_norm.2.bias", st.blob, st.e.ln2_b, d)
        gguf_read_tensor_f32(m, bytes, "v.patch_norm.3.weight", st.blob, st.e.ln3_w, d)
        gguf_read_tensor_f32(m, bytes, "v.patch_norm.3.bias", st.blob, st.e.ln3_b, d)
        gguf_read_tensor_f32(m, bytes, "v.position_embd.weight", st.blob, st.e.pos_w, 2l * st.e.pos_entries * d)
        uv_read_gemm(m, bytes, "mm.input_projection.weight", st, st.e.proj_bf16, st.e.proj_w, d * st.e.proj_dim)
    }
    fclose(f)
    return <- st
}

// GEMM offsets ride the halfword cursor on a bf16 load, the fp32 cursor otherwise — the field then
// indexes whichever plane the mm route reads
def private uv_gemm_off(bf16 : bool; var o : int64&; var wo : int64&; size : int64) : int64 {
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
def private uv_read_gemm(m : GGUFMeta; b : array<uint8> | #; name : string; var st : Gemma4uvStaging;
                         bf16 : bool; off, n_elem : int64) {
    if (bf16) {
        gguf_transcode_bf16(m, b, name, st.wblob, off, n_elem)
    } else {
        gguf_read_tensor_f32(m, b, name, st.blob, off, n_elem)
    }
}

//! Load the eleven-tensor gemma4uv embedder, image-backed: a prepared `.dlim` maps and every plane
//! borrows it, so there is no allocation and no processing at load; missing or stale, one is minted
//! from the source mmproj first. Panics on any other projector (the E-series `gemma4v` is a v2 leg).
def load_gemma4uv_embedder(path : string) : Gemma4uvEmbedder {
    let tag = GEMMA4UV_TAG
    if (path |> ends_with(".dlim")) {
        // named directly — no mmproj to regenerate from, so a wrong identity panics
        apply_box_profile_runtime()
        let ts_img = ref_time_ticks()
        var t = Gemma4uvEmbedder()
        if (!load_image(path, t, tag)) {
            panic("dasLLAMA: '{path}' is not a prepared gemma4uv-embedder image for this box/knobs (identity {image_identity(tag)}) - regenerate it from the source mmproj gguf")
        }
        to_log(LOG_INFO, "dasLLAMA: prepared gemma4uv-embedder image mapped in {get_time_usec(ts_img) / 1000} ms - {path}\n")
        return <- t
    }
    if (g_env_engine.image) {
        apply_box_profile_runtime()   // a backend pin must precede the identity (it names the backend)
        let img = image_path_for(path, tag)
        let ts_img = ref_time_ticks()
        var t = Gemma4uvEmbedder()
        if (load_image(img, t, tag)) {
            to_log(LOG_INFO, "dasLLAMA: prepared gemma4uv-embedder image mapped in {get_time_usec(ts_img) / 1000} ms - {img}\n")
            uv_check_planes(t, img)
            return <- t
        }
        delete t
        var inscope st <- stage_gemma4uv_embedder(path)
        var built = Gemma4uvEmbedder()
        if (cache_via_image_staged(st, built, img, tag)) {
            dlim_gc_stale(path, tag)   // the fresh bake ON DISK proves its lane's siblings dead; an in-memory build reaps nothing
        }
        uv_check_planes(built, img)
        return <- built
    }
    // no persisted image: mint one in memory and serve THAT — a served embedder is always
    // image-backed, so there is no second, array-owning shape of this struct to maintain
    var inscope st <- stage_gemma4uv_embedder(path)
    var built = Gemma4uvEmbedder()
    cache_via_image_staged(st, built, "", tag)
    uv_check_planes(built, path)
    return <- built
}

// the bind walk skips a missing section silently (a pure-fp32 image legitimately has no wblob),
// so the flag=>plane invariant must be checked once the carrier is live, or the kernel derefs null
def private uv_check_planes(t : Gemma4uvEmbedder; src : string) {
    if ((t.patch_bf16 || t.proj_bf16) && empty(t.wblob)) {
        panic("dasLLAMA gemma4uv: '{src}' declares bf16 planes but carries no wblob section - the image is corrupt or hand-edited")
    }
}

// im2col in ggml's [C][KH][KW] row order over the PLANAR view of the interleaved planes;
// superpatches walk the canvas row-major, matching pos_x = i % n_cols, pos_y = i / n_cols
def private uv_im2col(t : Gemma4uvEmbedder; planes : array<float>; w, h : int64; var x0 : array<float>) : int64 {
    let p = t.patch
    let n_cols = w / p
    let n_rows = h / p
    let npos = n_cols * n_rows
    let psq = p * p
    uv_ensure(x0, npos * t.patch_dim)
    unsafe {
        let src = addr<float const?>(planes[0])
        var dst = addr(x0[0])
        for (pi in range64(0l, npos)) {
            let py = (pi / n_cols) * p
            let px = (pi % n_cols) * p
            var row = dst + pi * t.patch_dim
            for (c in range64(0l, 3l)) {
                for (ky in range64(0l, p)) {
                    let sline = src + ((py + ky) * w + px) * 3l + c
                    var dline = row + c * psq + ky * p
                    for (kx in range64(0l, p)) {
                        dline[kx] = sline[kx * 3l]
                    }
                }
            }
        }
    }
    return npos
}

// weightless ggml_rms_norm: x ⊙ 1/√(mean(x²) + eps), per row
def private uv_rms_rows(var x : array<float>; d, npos : int64; eps : float) {
    unsafe {
        var xp = addr(x[0])
        for (r in range64(0l, npos)) {
            var row = xp + r * d
            var s = 0.0lf
            for (i in range64(0l, d)) {
                s += double(row[i]) * double(row[i])
            }
            let scale = float(1.0lf / sqrt(s / double(d) + double(eps)))
            for (i in range64(0l, d)) {
                row[i] *= scale
            }
        }
    }
}

// One GEMM, on whichever plane this embedder's offsets index. Both arms are the same arithmetic:
// dot_bf16's widen is an exact high-half shift, so the bf16 arm reads half the bytes for the
// identical result.
def private uv_gemm(var y : array<float>; t : Gemma4uvEmbedder; bf16 : bool; off : int64;
                    x : array<float>; n, d, npos : int64) {
    if (bf16) {
        mm_bf16_b(y, t.wblob, off, x, n, d, npos)
    } else {
        mm_blob_b(y, t.blob, off, x, n, d, npos)
    }
}

//! Encode a letterboxed, normalized canvas (`vision_normalize` planes, w×h) into
//! `out` = [npos × proj_dim] soft tokens. Returns npos = (w/48)×(h/48).
[hot_path]
def gemma4uv_encode(t : Gemma4uvEmbedder; var s : Gemma4uvState; planes : array<float>; w, h : int64;
                    var out : array<float>) : int64 {
    if (w % t.patch != 0l || h % t.patch != 0l) {
        panic("dasLLAMA gemma4uv: canvas {w}x{h} is not a multiple of the {t.patch}px superpatch - letterbox first")
    }
    // the learned position table holds pos_entries per axis; a wider grid would read past it
    if (w / t.patch > t.pos_entries || h / t.patch > t.pos_entries) {
        panic("dasLLAMA gemma4uv: canvas {w}x{h} needs a {w / t.patch}x{h / t.patch} position grid but the mmproj table holds {t.pos_entries} per axis - lower DASLLAMA_VISION_MAX_TOKENS")
    }
    let d = t.n_embd
    let n_cols = w / t.patch
    let npos = uv_im2col(t, planes, w, h, s.x0)
    uv_ensure(s.x1, npos * d)
    uv_ensure(s.x2, npos * d)
    layernorm_batch(s.x0, s.x0, t.blob, t.ln1_w, t.ln1_b, t.patch_dim, npos, GEMMA4UV_LN_EPS)
    uv_gemm(s.x1, t, t.patch_bf16, t.patch_w, s.x0, t.patch_dim, d, npos)
    add_bias_rows(s.x1, t.blob, t.patch_b, d, npos)
    layernorm_batch(s.x2, s.x1, t.blob, t.ln2_w, t.ln2_b, d, npos, GEMMA4UV_LN_EPS)
    unsafe {
        var xp = addr(s.x2[0])
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
    layernorm_batch(s.x1, s.x2, t.blob, t.ln3_w, t.ln3_b, d, npos, GEMMA4UV_LN_EPS)
    // Gemma4UnifiedMultimodalEmbedder: weightless rms(meta eps) → square projection
    uv_rms_rows(s.x1, d, npos, t.rms_eps)
    uv_ensure(out, npos * t.proj_dim)
    uv_gemm(out, t, t.proj_bf16, t.proj_w, s.x1, d, t.proj_dim, npos)
    return npos
}

//! The whole image path in one call: geometry → letterbox → normalize → `gemma4uv_encode` into
//! `out` = npos × proj_dim soft tokens (returns npos); `tag` names the `DASLLAMA_VISION_DUMP`
//! artifact. `add_user_image` is this plus queueing onto a chat turn.
def encode_image_(t : Gemma4uvEmbedder; var s : Gemma4uvState; img : VisionImage; tag : string;
                  var out : array<float>) : int64 {
    if (img.width <= 0 || img.height <= 0) {
        panic("dasLLAMA gemma4uv: encode_image_ got an empty image (decode failed?)")
    }
    let min_tok = g_env_vision.vision_min_tokens
    let max_tok = g_env_vision.vision_max_tokens
    if (min_tok < 1 || max_tok < min_tok) {
        panic("dasLLAMA gemma4uv: DASLLAMA_VISION_MIN_TOKENS={min_tok} / DASLLAMA_VISION_MAX_TOKENS={max_tok} - need 1 <= min <= max")
    }
    let align = int(t.patch)
    let target = vision_target_size(img.width, img.height, align, min_tok, max_tok)
    var inscope boxed <- vision_letterbox(img, target)
    maybe_dump_vision(boxed, tag)
    var inscope planes <- vision_normalize(boxed)
    return gemma4uv_encode(t, s, planes, int64(target.x), int64(target.y), out)
}
