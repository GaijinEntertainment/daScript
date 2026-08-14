options gen2
options _comment_hygiene = true

module dasllama_gemma4uv shared public

require daslib/fio
require dasllama/dasllama_gguf
require dasllama/dasllama_audio
require dasllama/dasllama_vision   // encode_image_: geometry + letterbox + normalize ahead of the encoder
require dasllama/dasllama_env     // g_env_vision: the token-budget knobs the geometry reads
require math

//! The gemma4uv "unified" vision embedder (gemma-4 dense 12B/26B): eleven tensors, no
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
    blob : array<float>
    patch_w : int64; patch_b : int64
    ln1_w : int64; ln1_b : int64     // v.patch_norm.1 — over raw im2col rows (patch_dim wide)
    ln2_w : int64; ln2_b : int64     // v.patch_norm.2 — after the patch linear
    ln3_w : int64; ln3_b : int64     // v.patch_norm.3 — after the pos adds
    pos_w : int64                    // [2][pos_entries][n_embd]: x table then y table
    proj_w : int64
}

//! Per-encode scratch, caller-owned like the audio EncoderState.
struct Gemma4uvState {
    x0 : array<float>    // [npos × patch_dim] im2col rows, then their LN
    x1 : array<float>    // [npos × n_embd]
    x2 : array<float>    // [npos × n_embd]
}

def finalize(var t : Gemma4uvEmbedder) {
    delete t.blob
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

//! Load the eleven-tensor gemma4uv embedder from an mmproj gguf into one fp32 blob.
//! Panics on any other projector — the E-series `gemma4v` ViT is a v2 leg.
def load_gemma4uv_embedder(path : string) : Gemma4uvEmbedder {
    var t = Gemma4uvEmbedder()
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
        t.patch = gguf_int(m, bytes, "clip.vision.patch_size") * merge
        t.patch_dim = t.patch * t.patch * 3l
        t.n_embd = gguf_int(m, bytes, "clip.vision.embedding_length")
        t.proj_dim = gguf_int(m, bytes, "clip.vision.projection_dim")
        t.rms_eps = gguf_f32(m, bytes, "clip.vision.attention.layer_norm_epsilon")
        let pos_ti = gguf_find_tensor(m, "v.position_embd.weight")
        if (pos_ti < 0) {
            panic("dasLLAMA gemma4uv: mmproj is missing tensor 'v.position_embd.weight'")
        }
        t.pos_entries = m.tensors[pos_ti].n_elem / (2l * t.n_embd)
        let d = t.n_embd
        let pd = t.patch_dim
        uv_expect(m, "v.patch_embd.weight", pd * d)
        uv_expect(m, "v.patch_embd.bias", d)
        uv_expect(m, "v.patch_norm.1.weight", pd)
        uv_expect(m, "v.patch_norm.1.bias", pd)
        uv_expect(m, "v.patch_norm.2.weight", d)
        uv_expect(m, "v.patch_norm.2.bias", d)
        uv_expect(m, "v.patch_norm.3.weight", d)
        uv_expect(m, "v.patch_norm.3.bias", d)
        uv_expect(m, "mm.input_projection.weight", d * t.proj_dim)
        var off = 0l
        t.patch_w = off; off += pd * d
        t.patch_b = off; off += d
        t.ln1_w = off; off += pd
        t.ln1_b = off; off += pd
        t.ln2_w = off; off += d
        t.ln2_b = off; off += d
        t.ln3_w = off; off += d
        t.ln3_b = off; off += d
        t.pos_w = off; off += 2l * t.pos_entries * d
        t.proj_w = off; off += d * t.proj_dim
        t.blob |> resize(off)
        gguf_read_tensor_f32(m, bytes, "v.patch_embd.weight", t.blob, t.patch_w, pd * d)
        gguf_read_tensor_f32(m, bytes, "v.patch_embd.bias", t.blob, t.patch_b, d)
        gguf_read_tensor_f32(m, bytes, "v.patch_norm.1.weight", t.blob, t.ln1_w, pd)
        gguf_read_tensor_f32(m, bytes, "v.patch_norm.1.bias", t.blob, t.ln1_b, pd)
        gguf_read_tensor_f32(m, bytes, "v.patch_norm.2.weight", t.blob, t.ln2_w, d)
        gguf_read_tensor_f32(m, bytes, "v.patch_norm.2.bias", t.blob, t.ln2_b, d)
        gguf_read_tensor_f32(m, bytes, "v.patch_norm.3.weight", t.blob, t.ln3_w, d)
        gguf_read_tensor_f32(m, bytes, "v.patch_norm.3.bias", t.blob, t.ln3_b, d)
        gguf_read_tensor_f32(m, bytes, "v.position_embd.weight", t.blob, t.pos_w, 2l * t.pos_entries * d)
        gguf_read_tensor_f32(m, bytes, "mm.input_projection.weight", t.blob, t.proj_w, d * t.proj_dim)
    }
    fclose(f)
    return <- t
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

//! Encode a letterboxed, normalized canvas (`vision_normalize` planes, w×h) into
//! `out` = [npos × proj_dim] soft tokens. Returns npos = (w/48)×(h/48).
[hot_path]
def gemma4uv_encode(t : Gemma4uvEmbedder; var s : Gemma4uvState; planes : array<float>; w, h : int64;
                    var out : array<float>) : int64 {
    if (w % t.patch != 0l || h % t.patch != 0l) {
        panic("dasLLAMA gemma4uv: canvas {w}x{h} is not a multiple of the {t.patch}px superpatch - letterbox first")
    }
    let d = t.n_embd
    let n_cols = w / t.patch
    let npos = uv_im2col(t, planes, w, h, s.x0)
    uv_ensure(s.x1, npos * d)
    uv_ensure(s.x2, npos * d)
    // patch_norm.1 over raw rows, then the patch linear + bias
    layernorm_batch(s.x0, s.x0, t.blob, t.ln1_w, t.ln1_b, t.patch_dim, npos, GEMMA4UV_LN_EPS)
    mm_blob_b(s.x1, t.blob, t.patch_w, s.x0, t.patch_dim, d, npos)
    add_bias_rows(s.x1, t.blob, t.patch_b, d, npos)
    layernorm_batch(s.x2, s.x1, t.blob, t.ln2_w, t.ln2_b, d, npos, GEMMA4UV_LN_EPS)
    // learned position lookups: x table first, then y, then pos_norm
    unsafe {
        var xp = addr(s.x2[0])
        let tbl = addr<float const?>(t.blob[0]) + t.pos_w
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
    mm_blob_b(out, t.blob, t.proj_w, s.x1, d, t.proj_dim, npos)
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
    let align = int(t.patch)
    let target = vision_target_size(img.width, img.height, align,
        g_env_vision.image_min_tokens, g_env_vision.image_max_tokens)
    let boxed <- vision_letterbox(img, target)
    maybe_dump_vision(boxed, tag)
    let planes <- vision_normalize(boxed)
    return gemma4uv_encode(t, s, planes, int64(target.x), int64(target.y), out)
}
