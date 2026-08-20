options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vision_embedder shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_gemma4uv public   // re-exported: a requirer that carries the union must see both families to delete it
require dasllama/dasllama_gemma4v public
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
}

struct VisionEmbedder {
    kind : VisionKind = VisionKind.none
    gemma4uv : Gemma4uvEmbedder = Gemma4uvEmbedder()
    gemma4v : Gemma4vTower = Gemma4vTower()
}

//! Per-encode scratch for whichever family the embedder carries (caller-owned, reused).
struct VisionState {
    gemma4uv : Gemma4uvState = Gemma4uvState()
    gemma4v : Gemma4vState = Gemma4vState()
}

// Stated rather than inferred: the family carriers hold borrowed planes, and a generated
// teardown would force every module that deletes a VisionEmbedder to see the plane finalizers too.
def finalize(var e : VisionEmbedder) {
    delete e.gemma4uv
    delete e.gemma4v
}

def finalize(var s : VisionState) {
    delete s.gemma4uv
    delete s.gemma4v
}

//! Soft-token width the embedder projects to (must equal the decoder width); 0 = no vision arm.
def vision_proj_dim(e : VisionEmbedder) : int64 {
    if (e.kind == VisionKind.gemma4uv) return e.gemma4uv.proj_dim
    if (e.kind == VisionKind.gemma4v) return e.gemma4v.proj_dim
    return 0l
}

//! The family's canvas alignment in px (one soft token per align×align): the geometry's unit.
def vision_align(e : VisionEmbedder) : int64 {
    if (e.kind == VisionKind.gemma4uv) return e.gemma4uv.patch
    if (e.kind == VisionKind.gemma4v) return e.gemma4v.patch * e.gemma4v.merge
    return 0l
}

//! The mmproj projector name the carrier holds ("gemma4uv" / "gemma4v"; "" for none) — for logs,
//! bench cells and the family tokens.
def vision_family(e : VisionEmbedder) : string {
    if (e.kind == VisionKind.gemma4uv) return "gemma4uv"
    if (e.kind == VisionKind.gemma4v) return "gemma4v"
    return ""
}

//! The tower's precision half of an image cell's ``<tower>/<decoder>`` exec_fmt stamp (the ASR
//! spelling): "f32" = every plane as the file has it (exact), "q8" = the Q8_0 block-GEMM lane.
def vision_exec_fmt(e : VisionEmbedder) : string {
    if (e.kind == VisionKind.gemma4v && e.gemma4v.q8) return "q8"
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
    panic("dasLLAMA vision: '{path}' is projector '{proj}' - no vision family serves it (gemma4uv, gemma4v)")
    return <- VisionEmbedder()
}

//! The whole image path for the carried family: geometry → letterbox → normalize → the family's
//! encode into ``out`` = nout × proj_dim soft tokens (returns nout). ``tag`` names the
//! ``DASLLAMA_VISION_DUMP`` artifact. Panics on the text-only arm.
def encode_image_(e : VisionEmbedder; var s : VisionState; img : VisionImage; tag : string; var out : array<float>) : int64 {
    if (e.kind == VisionKind.gemma4uv) return encode_image_(e.gemma4uv, s.gemma4uv, img, tag, out)
    if (e.kind == VisionKind.gemma4v) return encode_image_(e.gemma4v, s.gemma4v, img, tag, out)
    panic("dasLLAMA vision: encode_image_ on an embedder with no vision arm")
    return 0l
}
