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
//! Family files stay ignorant of each other; a seam that names ``Gemma4uvEmbedder`` or
//! ``Gemma4vTower`` outside its own file and this one is a defect.

enum VisionKind {
    none     // no vision arm
    uv       // gemma4uv — the gemma-4 dense embedder (no attention)
    v        // gemma4v — the gemma-4 E-series ViT tower
}

struct VisionEmbedder {
    kind : VisionKind = VisionKind.none
    uv : Gemma4uvEmbedder = Gemma4uvEmbedder()
    v : Gemma4vTower = Gemma4vTower()
}

//! Per-encode scratch for whichever family the embedder carries (caller-owned, reused).
struct VisionState {
    uv : Gemma4uvState = Gemma4uvState()
    v : Gemma4vState = Gemma4vState()
}

// Stated rather than inferred: the family carriers hold borrowed planes, and a generated
// teardown would force every module that deletes a VisionEmbedder to see the plane finalizers too.
def finalize(var e : VisionEmbedder) {
    delete e.uv
    delete e.v
}

def finalize(var s : VisionState) {
    delete s.uv
    delete s.v
}

//! Soft-token width the embedder projects to (must equal the decoder width); 0 = no vision arm.
def vision_proj_dim(e : VisionEmbedder) : int64 {
    if (e.kind == VisionKind.uv) return e.uv.proj_dim
    if (e.kind == VisionKind.v) return e.v.proj_dim
    return 0l
}

//! The family's canvas alignment in px (one soft token per align×align): the geometry's unit.
def vision_align(e : VisionEmbedder) : int64 {
    if (e.kind == VisionKind.uv) return e.uv.patch
    if (e.kind == VisionKind.v) return e.v.patch * e.v.merge
    return 0l
}

//! The mmproj projector name the carrier holds ("gemma4uv" / "gemma4v"; "" for none) — for logs,
//! bench cells and the family tokens.
def vision_family(e : VisionEmbedder) : string {
    if (e.kind == VisionKind.uv) return "gemma4uv"
    if (e.kind == VisionKind.v) return "gemma4v"
    return ""
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
        var e = VisionEmbedder(kind = VisionKind.uv)
        e.uv <- load_gemma4uv_embedder(path)
        return <- e
    }
    if (proj == "gemma4v") {
        var e = VisionEmbedder(kind = VisionKind.v)
        e.v <- load_gemma4v_tower(path)
        return <- e
    }
    panic("dasLLAMA vision: '{path}' is projector '{proj}' - no vision family serves it (gemma4uv, gemma4v)")
    return <- VisionEmbedder()
}

//! The whole image path for the carried family: geometry → letterbox → normalize → the family's
//! encode into ``out`` = nout × proj_dim soft tokens (returns nout). ``tag`` names the
//! ``DASLLAMA_VISION_DUMP`` artifact. Panics on the text-only arm.
def encode_image_(e : VisionEmbedder; var s : VisionState; img : VisionImage; tag : string; var out : array<float>) : int64 {
    if (e.kind == VisionKind.uv) return encode_image_(e.uv, s.uv, img, tag, out)
    if (e.kind == VisionKind.v) return encode_image_(e.v, s.v, img, tag, out)
    panic("dasLLAMA vision: encode_image_ on an embedder with no vision arm")
    return 0l
}
