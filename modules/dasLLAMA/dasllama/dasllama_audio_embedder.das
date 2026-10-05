options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_audio_embedder shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_gemma4a public   // re-exported: a requirer that carries the union must see the family to delete it
require dasllama/dasllama_audio_tower public   // the same for the whisper-class tower (qwen2a, qwen2.5o, ultravox, voxtral)
require dasllama/dasllama_image   // image_family_tag — a prepared .dlim routes on its baked tag
require strings

//! The audio encoder of whatever family the mmproj turned out to be: ONE carrier through every
//! seam (server slot, media worker, bench cell, the facade), the family decided once at load
//! and routed by one-line arms here. ``kind == none`` is the no-audio arm.

enum AudioKind {
    none
    gemma4a     // the gemma-4 E-series Conformer audio encoder
    audio_tower // the whisper-class tower with its projector: Qwen2-Audio, Qwen2.5-Omni, Ultravox, Voxtral
}

struct AudioEmbedder {
    kind : AudioKind = AudioKind.none
    gemma4a : Gemma4aEncoder = Gemma4aEncoder()
    audio_tower : AudioTowerEncoder = AudioTowerEncoder()
}

//! Per-encode scratch for whichever family the embedder carries (caller-owned, reused).
struct AudioState {
    gemma4a : Gemma4aState = Gemma4aState()
    audio_tower : AudioTowerState = AudioTowerState()
}

// Stated rather than inferred: the family carrier holds borrowed planes, and a generated
// teardown would force every module that deletes an AudioEmbedder to see the plane finalizers too.
def finalize(var e : AudioEmbedder) {
    delete e.gemma4a
    delete e.audio_tower
}

def finalize(var s : AudioState) {
    delete s.gemma4a
    delete s.audio_tower
}

//! Soft-token width the encoder projects to (must equal the decoder width); 0 = no audio arm.
def audio_proj_dim(e : AudioEmbedder) : int64 {
    if (e.kind == AudioKind.gemma4a) return e.gemma4a.proj_dim
    if (e.kind == AudioKind.audio_tower) return audio_tower_encoder_proj_dim(e.audio_tower)
    return 0l
}

//! The audio family the carrier holds ("gemma4a"; "" for none) — for logs and bench cells.
def audio_family(e : AudioEmbedder) : string {
    if (e.kind == AudioKind.gemma4a) return "gemma4a"
    if (e.kind == AudioKind.audio_tower) return audio_tower_encoder_family(e.audio_tower)
    return ""
}

//! Probe ``path`` for an audio tower ANY family serves: the projection width when one is
//! present, 0 otherwise (missing file, non-GGUF, a vision-only mmproj). Never panics — the
//! seams gate their audio arms on this.
def audio_probe_proj_dim(path : string) : int64 {
    let g4a = gemma4a_probe_proj_dim(path)
    return g4a > 0l ? g4a : audio_tower_encoder_probe_proj_dim(path)
}

//! Whether the mmproj at `path` splices its soft tokens with no span marker (see `audio_span_bare`); false for a file
//! no audio family serves. Never panics.
def audio_probe_span_bare(path : string) : bool => audio_tower_encoder_probe_span_bare(path)

//! Whether the carried family splices its soft tokens with no span marker around them
def audio_span_bare(e : AudioEmbedder) : bool => e.kind == AudioKind.audio_tower && audio_tower_encoder_span_bare(e.audio_tower)

//! Load an audio mmproj (gguf, or a prepared ``.dlim`` routed on its baked family tag) as
//! whichever family it is — probed from the file, never from a knob. Panics by name on a file
//! no audio family serves; probe first where absence is an answer.
def load_audio_embedder(path : string) : AudioEmbedder {
    let is_gemma4a_image = path |> ends_with(".dlim") && image_family_tag(path) |> starts_with("gemma4a-")
    if (is_gemma4a_image || gemma4a_probe_proj_dim(path) > 0l) {
        var e = AudioEmbedder(kind = AudioKind.gemma4a)
        e.gemma4a <- load_gemma4a_encoder(path)
        return <- e
    }
    if (audio_tower_encoder_image(path) || audio_tower_encoder_probe_proj_dim(path) > 0l) {
        var e = AudioEmbedder(kind = AudioKind.audio_tower)
        e.audio_tower <- load_audio_tower_encoder(path)
        return <- e
    }
    panic("dasLLAMA audio: '{path}' carries no audio tower any family serves (gemma4a, or a whisper-class tower: qwen2a, qwen2.5o, ultravox, voxtral)")
    return <- AudioEmbedder()
}

//! The whole audio path for the carried family: 16 kHz mono samples → the family's encode into
//! ``out`` = nout × proj_dim soft tokens (returns nout). Panics on the no-audio arm.
def encode_audio_(e : AudioEmbedder; var s : AudioState; samples : array<float> | #; var out : array<float>) : int64 {
    if (e.kind == AudioKind.gemma4a) return gemma4a_encode_all(e.gemma4a, s.gemma4a, samples, out)
    if (e.kind == AudioKind.audio_tower) return audio_tower_encode_all(e.audio_tower, s.audio_tower, samples, out)
    panic("dasLLAMA audio: encode_audio_ on an embedder with no audio arm")
    return 0l
}
