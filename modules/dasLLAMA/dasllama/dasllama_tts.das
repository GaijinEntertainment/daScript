options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tts shared public

require dasllama/dasllama_lint public
require strings
require daslib/fio
require dasllama/dasllama_gguf
require dasllama/dasllama_textnorm
require dasllama/dasllama_postag
require dasllama/dasllama_g2p
require dasllama/dasllama_tts_types public
require dasllama/dasllama_kitten

//! The TTS facade: load a model by its GGUF, run text through the front end, synthesize.
//! Returns PCM; writing a file is the caller's job through `dasllama_audio_io`.

enum TtsKind {
    kitten
}

//! A loaded TTS model: the family carrier and the front-end data it reads beside the GGUF.
struct TtsModel {
    kind : TtsKind
    kitten : KittenModel = KittenModel()
    g2p : G2pModel
    tagger : PosTagger
}

//! Load a TTS GGUF; `tts_g2p.bin` and `tts_postag.bin` come from the same directory.
def load_tts_model(path : string) : TtsModel {
    var m = TtsModel()
    var arch = ""
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA tts: cannot open model '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope meta <- parse_gguf_meta(bytes)
        arch = gguf_str(meta, bytes, "general.architecture")
    }
    fclose(f)
    if (arch == "kitten-tts") {
        m.kind = TtsKind.kitten
        m.kitten <- load_kitten_model(path)
    } else {
        panic("dasLLAMA tts: '{path}' has architecture '{arch}', which is not a TTS family")
    }
    let dir = dir_name(path)
    m.g2p <- load_g2p(path_join(dir, "tts_g2p.bin"))
    m.tagger <- load_pos_tagger(path_join(dir, "tts_postag.bin"))
    return <- m
}

def caps(m : TtsModel) : TtsCaps {
    var c = TtsCaps()
    c.voices := m.kitten.voice_names
    c.sample_rate = m.kitten.sample_rate
    c.langs |> push("en")
    c.cloning = false
    return <- c
}

//! Text -> speech: normalize, phonemize, map into the family's symbols, synthesize.
def synthesize(m : TtsModel; text : string; voice : string; speed : float = 1.0) : TtsAudio {
    var out = TtsAudio()
    out.sample_rate = m.kitten.sample_rate
    let vname = kitten_voice(m.kitten, voice)
    if (empty(vname)) {
        panic("dasLLAMA tts: unknown voice '{voice}'")
    }
    let clock = ref_time_ticks()
    let norm = normalize_text(text)
    let ps = kitten_phonemes_from_misaki(g2p_phonemize(m.g2p, m.tagger, norm))
    let ids <- kitten_token_ids(ps)
    let row = kitten_style_row(norm, m.kitten.style_rows)
    var style : array<float>
    style |> resize(m.kitten.style_dim)
    let sd = int(m.kitten.style_dim)
    m.kitten.voices |> get(vname) $(pack) {
        for (i in range(sd)) {
            style[i] = pack[row * sd + i]
        }
    }
    let sp = speed * (m.kitten.speed_priors?[vname] ?? 1.0)
    var noise = TtsNoise()
    out.timings.front_end_us = int64(get_time_usec(clock))
    out.pcm <- kitten_synthesize_ids(m.kitten, ids, style, sp, noise, 0x5eed1ul, null, out.timings)
    kitten_trim(out.pcm)
    to_log(LOG_INFO, "dasLLAMA tts: kitten {m.kitten.size}: {timings_line(out.timings, out.sample_rate)}\n")
    return <- out
}
