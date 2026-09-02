options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tts shared public

require dasllama/dasllama_lint public
require daslib/fio
require dasllama/dasllama_textnorm
require dasllama/dasllama_postag
require dasllama/dasllama_g2p
require dasllama/dasllama_tts_types public
require dasllama/dasllama_styletts2
require dasllama/dasllama_kitten
require dasllama/dasllama_kokoro

//! The TTS facade: load a model by its GGUF, run text through the front end, synthesize.
//! Returns PCM; writing a file is the caller's job through `dasllama_audio_io`.

enum TtsKind {
    kitten
    kokoro
}

//! A loaded TTS model: the shared assembly, the family's driver data, and the front-end packs
//! read beside the GGUF.
struct TtsModel {
    kind : TtsKind
    model : StyleTts2Model = StyleTts2Model()
    kitten : KittenFamily
    kokoro : KokoroFamily
    g2p : G2pModel
    tagger : PosTagger
}

//! Load a TTS GGUF; `tts_g2p.bin` and `tts_postag.bin` come from the same directory.
def load_tts_model(path : string) : TtsModel {
    var m = TtsModel()
    m.model <- load_styletts2(path)
    if (m.model.family == "kitten-tts") {
        m.kind = TtsKind.kitten
        m.kitten <- load_kitten_family(path)
    } elif (m.model.family == "kokoro") {
        m.kind = TtsKind.kokoro
        m.kokoro <- load_kokoro_family(path)
    } else {
        panic("dasLLAMA tts: '{path}' has architecture '{m.model.family}', which is not a TTS family")
    }
    let dir = dir_name(path)
    m.g2p <- load_g2p(path_join(dir, "tts_g2p.bin"))
    m.tagger <- load_pos_tagger(path_join(dir, "tts_postag.bin"))
    return <- m
}

def caps(m : TtsModel) : TtsCaps {
    var c = TtsCaps()
    c.voices := m.model.voice_names
    c.sample_rate = m.model.sample_rate
    c.langs |> push("en")
    c.cloning = false
    return <- c
}

//! Text -> speech: normalize, phonemize, map into the family's symbols, synthesize.
def synthesize(m : TtsModel; text : string; voice : string; speed : float = 1.0) : TtsAudio {
    var out = TtsAudio()
    out.sample_rate = m.model.sample_rate
    let vname = m.kind == TtsKind.kitten ? kitten_voice(m.kitten, m.model, voice) : (styletts2_has_voice(m.model, voice) ? voice : "")
    if (empty(vname)) {
        panic("dasLLAMA tts: unknown voice '{voice}'")
    }
    let clock = ref_time_ticks()
    let norm = normalize_text(text)
    let ps_misaki = g2p_phonemize(m.g2p, m.tagger, norm)
    let is_kitten = m.kind == TtsKind.kitten
    let ids <- is_kitten ? kitten_token_ids(kitten_phonemes_from_misaki(ps_misaki)) : kokoro_token_ids(m.kokoro, ps_misaki)
    let row = is_kitten ? kitten_style_row(norm, m.model.style_rows) : kokoro_style_row(ps_misaki, m.model.style_rows)
    let sp = is_kitten ? speed * (m.kitten.speed_priors?[vname] ?? 1.0) : speed
    var style : array<float>
    styletts2_style(m.model, vname, row, style)
    var noise = TtsNoise()
    out.timings.front_end_us = int64(get_time_usec(clock))
    out.pcm <- styletts2_synthesize(m.model, ids, style, sp, noise, 0x5eed1ul, null, out.timings)
    if (m.kind == TtsKind.kitten) {
        kitten_trim(out.pcm)
    }
    to_log(LOG_INFO, "dasLLAMA tts: {m.model.family} {m.model.name}: {timings_line(out.timings, out.sample_rate)}\n")
    return <- out
}
