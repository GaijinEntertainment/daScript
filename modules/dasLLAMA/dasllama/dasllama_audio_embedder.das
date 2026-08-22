options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_audio_embedder shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_gemma4a public   // re-exported: a requirer that carries the union must see the family to delete it

//! The audio encoder of whatever family the mmproj turned out to be — the VisionEmbedder shape:
//! ONE carrier through every seam (server slot, media worker, bench cell, the facade), the
//! family decided once at load and routed by one-line arms here. ``kind == none`` is the
//! no-audio arm.

enum AudioKind {
    none        // no audio arm
    gemma4a     // the gemma-4 E-series Conformer audio encoder
}

struct AudioEmbedder {
    kind : AudioKind = AudioKind.none
    gemma4a : Gemma4aEncoder = Gemma4aEncoder()
}

//! Per-encode scratch for whichever family the embedder carries (caller-owned, reused).
struct AudioState {
    gemma4a : Gemma4aState = Gemma4aState()
}

// Stated rather than inferred: the family carrier holds borrowed planes, and a generated
// teardown would force every module that deletes an AudioEmbedder to see the plane finalizers too.
def finalize(var e : AudioEmbedder) {
    delete e.gemma4a
}

def finalize(var s : AudioState) {
    delete s.gemma4a
}

//! Soft-token width the encoder projects to (must equal the decoder width); 0 = no audio arm.
def audio_proj_dim(e : AudioEmbedder) : int64 {
    if (e.kind == AudioKind.gemma4a) return e.gemma4a.proj_dim
    return 0l
}

//! The audio family the carrier holds ("gemma4a"; "" for none) — for logs and bench cells.
def audio_family(e : AudioEmbedder) : string {
    if (e.kind == AudioKind.gemma4a) return "gemma4a"
    return ""
}

//! Probe ``path`` for an audio tower ANY family serves: the projection width when one is
//! present, 0 otherwise (missing file, non-GGUF, a vision-only mmproj). Never panics — the
//! seams gate their audio arms on this.
def audio_probe_proj_dim(path : string) : int64 {
    return gemma4a_probe_proj_dim(path)
}

//! Load an audio mmproj as whichever family it is — probed from the file, never from a knob.
//! Panics by name on a file no audio family serves; probe first where absence is an answer.
def load_audio_embedder(path : string) : AudioEmbedder {
    if (audio_probe_proj_dim(path) > 0l) {
        var e = AudioEmbedder(kind = AudioKind.gemma4a)
        e.gemma4a <- load_gemma4a_encoder(path)
        return <- e
    }
    panic("dasLLAMA audio: '{path}' carries no audio tower any family serves (gemma4a)")
    return <- AudioEmbedder()
}

//! The whole audio path for the carried family: 16 kHz mono samples → the family's encode into
//! ``out`` = nout × proj_dim soft tokens (returns nout). Panics on the no-audio arm.
def encode_audio_(e : AudioEmbedder; var s : AudioState; samples : array<float> | #; var out : array<float>) : int64 {
    if (e.kind == AudioKind.gemma4a) return gemma4a_encode_all(e.gemma4a, s.gemma4a, samples, out)
    panic("dasLLAMA audio: encode_audio_ on an embedder with no audio arm")
    return 0l
}
