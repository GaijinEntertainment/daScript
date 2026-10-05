options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_audio_tower shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_audio public   // re-exported: a requirer that carries the encoder must see the tower to delete it
require dasllama/dasllama_tower   // asr_prof_add: the stage clock
require dasllama/dasllama_image   // image_family_tag — a prepared .dlim routes on its baked tag
require strings

//! The whisper-class tower with its projector as an audio carrier family: Qwen2-Audio, Qwen2.5-Omni,
//! Ultravox, Voxtral. The tower, its mel and its projector are `dasllama_audio`'s; this file is what
//! the carrier calls.

struct AudioTowerEncoder {
    tower : AudioTower = AudioTower()
}

//! Per-encode scratch (caller-owned, reused)
struct AudioTowerState {
    enc : EncoderState = EncoderState()
    made : bool   //! `enc` is sized for the encoder's tower: made at the first encode
}

def finalize(var e : AudioTowerEncoder) {
    delete e.tower
}

def finalize(var s : AudioTowerState) {
    delete s.enc
}

//! The projection width of the mmproj at `path` where the tower serves it, 0 otherwise. Never panics.
def audio_tower_encoder_probe_proj_dim(path : string) : int64 => audio_tower_probe_proj_dim(path)

//! Whether the mmproj at `path` splices its soft tokens with no span marker; false for a file the tower does not serve
def audio_tower_encoder_probe_span_bare(path : string) : bool => audio_tower_probe_span_bare(path)

//! Whether `path` is a prepared image of this family
def audio_tower_encoder_image(path : string) : bool
    => path |> ends_with(".dlim") && image_family_tag(path) |> starts_with("tower-")

def load_audio_tower_encoder(path : string) : AudioTowerEncoder {
    var e = AudioTowerEncoder()
    e.tower <- load_audio_tower(path)
    return <- e
}

def audio_tower_encoder_proj_dim(e : AudioTowerEncoder) : int64 => e.tower.proj_dim

//! the projector the file carries, as the log names it
def audio_tower_encoder_family(e : AudioTowerEncoder) : string => "{e.tower.proj_kind}"

//! Whether the encoder's soft tokens splice with no span marker around them
def audio_tower_encoder_span_bare(e : AudioTowerEncoder) : bool => audio_span_bare(e.tower.proj_kind)

//! The tower over a whole clip: a 30 s mel chunk at a time, each chunk's soft tokens appended to `out`; answers the rows
def audio_tower_encode_all(e : AudioTowerEncoder; var s : AudioTowerState; samples : array<float> | #; var out : array<float>) : int64 {
    if (!s.made) {
        s.enc <- make_encoder_state(e.tower)
        s.made = true
    }
    let t_mel = ref_time_ticks()
    var inscope chunks <- log_mel_chunks(samples, e.tower.n_mel)
    asr_prof_add("enc.mel", t_mel)
    let rows = audio_tokens_per_chunk(e.tower)
    out |> reserve(length(out) + length(chunks) * int(rows * e.tower.proj_dim))
    for (chunk in chunks) {
        audio_encode(e.tower, s.enc, chunk)
        for (i in range64(rows * e.tower.proj_dim)) {
            out |> push(s.enc.out[i])
        }
    }
    return long_length(chunks) * rows
}
