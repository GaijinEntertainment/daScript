options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vision_embedder shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_gemma4uv public   // re-exported: a requirer that carries the union must see both families to delete it
require dasllama/dasllama_gemma4v public
require dasllama/dasllama_gemma3v public
require dasllama/dasllama_qwen3v public
require dasllama/dasllama_qwen25v public
require dasllama/dasllama_vision public     // VisionImage — the decoded-pixel input every family takes
require dasllama/dasllama_gguf
require dasllama/dasllama_image   // image_family_tag — a prepared .dlim routes on its baked tag
require daslib/fio
require strings

//! The vision embedder of whatever family the mmproj turned out to be — the AsrModel shape: ONE
//! carrier through every seam (chat session, server slot, bench cell, the facade), the family
//! decided once at load and routed by one-line arms here. ``kind == none`` is the text-only arm.

enum VisionKind {
    none        // no vision arm
    gemma4uv    // the gemma-4 dense embedder (no attention)
    gemma4v     // the gemma-4 E-series ViT tower
    gemma3v     // the gemma-3 SigLIP tower (fixed 896² canvas)
    qwen3v      // the qwen3vl SigLIP-geometry ViT (Qwen3-Omni; mrope decoder positions)
    qwen25v     // the qwen2.5 window-attention ViT (Qwen2.5-Omni/VL; mrope decoder positions)
}

struct VisionEmbedder {
    kind : VisionKind = VisionKind.none
    gemma4uv : Gemma4uvEmbedder = Gemma4uvEmbedder()
    gemma4v : Gemma4vTower = Gemma4vTower()
    gemma3v : Gemma3vTower = Gemma3vTower()
    qwen3v : Qwen3vTower = Qwen3vTower()
    qwen25v : Qwen25vTower = Qwen25vTower()
}

//! Per-encode scratch for whichever family the embedder carries (caller-owned, reused).
struct VisionState {
    gemma4uv : Gemma4uvState = Gemma4uvState()
    gemma4v : Gemma4vState = Gemma4vState()
    gemma3v : Gemma3vState = Gemma3vState()
    qwen3v : Qwen3vState = Qwen3vState()
    qwen25v : Qwen25vState = Qwen25vState()
}

// Stated rather than inferred: the family carriers hold borrowed planes, and a generated
// teardown would force every module that deletes a VisionEmbedder to see the plane finalizers too.
def finalize(var e : VisionEmbedder) {
    delete e.gemma4uv
    delete e.gemma4v
    delete e.gemma3v
    delete e.qwen3v
    delete e.qwen25v
}

def finalize(var s : VisionState) {
    delete s.gemma4uv
    delete s.gemma4v
    delete s.gemma3v
    delete s.qwen3v
    delete s.qwen25v
}

//! Soft-token width the embedder projects to (must equal the decoder width); 0 = no vision arm.
def vision_proj_dim(e : VisionEmbedder) : int64 {
    if (e.kind == VisionKind.gemma4uv) return e.gemma4uv.proj_dim
    if (e.kind == VisionKind.gemma4v) return e.gemma4v.proj_dim
    if (e.kind == VisionKind.gemma3v) return e.gemma3v.proj_dim
    if (e.kind == VisionKind.qwen3v) return e.qwen3v.proj_dim
    if (e.kind == VisionKind.qwen25v) return e.qwen25v.proj_dim
    return 0l
}

//! Deepstack slice count (dense Qwen3-VL): the encoded rows are (1+n)·proj_dim wide and the
//! decoder must carry the same `n_deepstack` — pair them at arm time. 0 for every other family.
def vision_deepstack(e : VisionEmbedder) : int64 {
    return e.kind == VisionKind.qwen3v ? e.qwen3v.n_deepstack : 0l
}

//! One encoded soft-token row's float count: proj_dim widened by the deepstack slices.
def vision_row_width(e : VisionEmbedder) : int64 {
    return vision_proj_dim(e) * (1l + vision_deepstack(e))
}

//! The family's canvas alignment in px (one soft token per align×align): the geometry's unit.
def vision_align(e : VisionEmbedder) : int64 {
    if (e.kind == VisionKind.gemma4uv) return e.gemma4uv.patch
    if (e.kind == VisionKind.gemma4v) return e.gemma4v.patch * e.gemma4v.merge
    if (e.kind == VisionKind.gemma3v) return e.gemma3v.patch * e.gemma3v.merge
    if (e.kind == VisionKind.qwen3v) return e.qwen3v.patch * e.qwen3v.merge
    if (e.kind == VisionKind.qwen25v) return e.qwen25v.patch * e.qwen25v.merge
    return 0l
}

//! The mmproj projector name the carrier holds ("gemma4uv" / "gemma4v" / "gemma3v"; "" for none)
//! — for logs, bench cells and the family tokens.
def vision_family(e : VisionEmbedder) : string {
    if (e.kind == VisionKind.gemma4uv) return "gemma4uv"
    if (e.kind == VisionKind.gemma4v) return "gemma4v"
    if (e.kind == VisionKind.gemma3v) return "gemma3v"
    if (e.kind == VisionKind.qwen3v) return "qwen3v"
    if (e.kind == VisionKind.qwen25v) return "qwen25v"
    return ""
}

//! The tower's precision half of an image cell's ``<tower>/<decoder>`` exec_fmt stamp (the ASR
//! spelling): "f32" = every plane as the file has it (exact), "q8" = the Q8_0 block-GEMM lane.
def vision_exec_fmt(e : VisionEmbedder) : string {
    if ((e.kind == VisionKind.gemma4v && e.gemma4v.q8) || (e.kind == VisionKind.gemma3v && e.gemma3v.q8)
            || (e.kind == VisionKind.qwen3v && e.qwen3v.q8)) {
        return "q8"
    }
    return "f32"
}

// what the file says it is: the gguf's clip.vision.projector_type, or a .dlim's baked family tag
def private vision_sniff(path : string) : string {
    if (path |> ends_with(".dlim")) {
        return image_family_tag(path)
    }
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA vision: cannot open mmproj '{path}'")
    }
    var proj = ""
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        if (gguf_has(m, "clip.vision.projector_type")) {
            proj = gguf_str(m, bytes, "clip.vision.projector_type")
        } elif (gguf_has(m, "clip.projector_type")) {
            proj = gguf_str(m, bytes, "clip.projector_type")
        }
    }
    fclose(f)
    return proj
}

//! Load a vision mmproj (gguf, or a prepared ``.dlim``) as whichever family it is — sniffed from
//! the file, never from a knob. Panics by name on a projector no vision family serves.
def load_vision_embedder(path : string) : VisionEmbedder {
    let proj = vision_sniff(path)
    if (proj == "gemma4uv") {
        var e = VisionEmbedder(kind = VisionKind.gemma4uv)
        e.gemma4uv <- load_gemma4uv_embedder(path)
        return <- e
    }
    if (proj == "gemma4v" || proj == "gemma4v-q8") {   // the two gemma4v image lanes share the family
        var e = VisionEmbedder(kind = VisionKind.gemma4v)
        e.gemma4v <- load_gemma4v_tower(path)
        return <- e
    }
    if (proj == "gemma3" || proj == "gemma3v" || proj == "gemma3v-q8") {   // the gguf spells "gemma3"; the image lanes bake gemma3v*
        var e = VisionEmbedder(kind = VisionKind.gemma3v)
        e.gemma3v <- load_gemma3v_tower(path)
        return <- e
    }
    if (proj == "qwen3vl_merger" || proj == "qwen3v" || proj == "qwen3v-q8") {   // the gguf spells qwen3vl_merger; the two image lanes bake qwen3v*
        var e = VisionEmbedder(kind = VisionKind.qwen3v)
        e.qwen3v <- load_qwen3v_tower(path)
        return <- e
    }
    if (proj == "qwen2.5o" || proj == "qwen2.5vl_merger" || proj == "qwen25v") {   // dual-tower Omni files spell qwen2.5o; the image lane bakes qwen25v
        var e = VisionEmbedder(kind = VisionKind.qwen25v)
        e.qwen25v <- load_qwen25v_tower(path)
        return <- e
    }
    panic("dasLLAMA vision: '{path}' is projector '{proj}' - no vision family serves it (gemma4uv, gemma4v, gemma3, qwen3vl_merger, qwen2.5o)")
    return <- VisionEmbedder()
}

//! The MERGED image grid of the last encode when the carried family ropes the decoder with
//! grid positions (qwen mrope): (nx, ny) soft-token columns/rows; (0, 0) = sequential
//! positions (every gemma family). Valid right after ``encode_image_``.
def vision_mrope_grid(e : VisionEmbedder; s : VisionState) : int2 {
    if (e.kind == VisionKind.qwen3v) {
        return int2(int(s.qwen3v.grid_w / e.qwen3v.merge), int(s.qwen3v.grid_h / e.qwen3v.merge))
    }
    if (e.kind == VisionKind.qwen25v) {
        return int2(int(s.qwen25v.grid_w / e.qwen25v.merge), int(s.qwen25v.grid_h / e.qwen25v.merge))
    }
    return int2(0)
}

//! The whole image path for the carried family: geometry → letterbox → normalize → the family's
//! encode into ``out`` = nout × proj_dim soft tokens (returns nout). ``tag`` names the
//! ``DASLLAMA_VISION_DUMP`` artifact. Panics on the text-only arm.
def encode_image_(e : VisionEmbedder; var s : VisionState; img : VisionImage; tag : string; var out : array<float>) : int64 {
    if (e.kind == VisionKind.gemma4uv) return encode_image_(e.gemma4uv, s.gemma4uv, img, tag, out)
    if (e.kind == VisionKind.gemma4v) return encode_image_(e.gemma4v, s.gemma4v, img, tag, out)
    if (e.kind == VisionKind.gemma3v) return encode_image_(e.gemma3v, s.gemma3v, img, tag, out)
    if (e.kind == VisionKind.qwen3v) return encode_image_(e.qwen3v, s.qwen3v, img, tag, out)
    if (e.kind == VisionKind.qwen25v) return encode_image_(e.qwen25v, s.qwen25v, img, tag, out)
    panic("dasLLAMA vision: encode_image_ on an embedder with no vision arm")
    return 0l
}
