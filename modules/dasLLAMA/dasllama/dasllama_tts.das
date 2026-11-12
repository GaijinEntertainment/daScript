options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tts shared public

require dasllama/dasllama_lint public
require daslib/fio
require math
require strings
require daslib/strings_boost
require daslib/utf8_utils
require dasllama/dasllama_unicode
require dasllama/dasllama_textnorm
require dasllama/dasllama_textnorm_ru
require dasllama/dasllama_postag
require dasllama/dasllama_g2p
require dasllama/dasllama_g2p_ru
require dasllama/dasllama_g2p_ru_stress
require dasllama/dasllama_tts_types public
require dasllama/dasllama_styletts2
require dasllama/dasllama_kitten
require dasllama/dasllama_kokoro
require dasllama/dasllama_pocket
require dasllama/dasllama_gguf   // gguf_file_arch: the family sniff before a load
require dasllama/dasllama_math   // with_dasllama_jobque_: the stress-context pass of a caller outside any job queue
require daslib/jobque_boost   // is_job_que_available

//! The TTS facade: load a model by its GGUF, run text through the front end, synthesize.
//! Returns PCM; writing a file is the caller's job through `dasllama_audio_io`.

enum TtsKind {
    kitten      //! KittenTTS: phonemes in, the StyleTTS2 chain
    kokoro      //! Kokoro and its fine-tunes: phonemes in, the StyleTTS2 chain with voice packs
    pocket      //! Pocket TTS: text in, a continuous-audio language model that clones a voice
    front_end   //! no voice: the packs of a directory alone, which normalize, mark stress and phonemize
}

struct TtsModel {
    //! A loaded TTS model: the shared assembly, the family's driver data, and the front-end packs
    //! read beside the GGUF. Load with ``load_tts_model``; ``delete`` frees every plane and pack.
    kind : TtsKind                          //! the family the GGUF's architecture named
    model : StyleTts2Model = StyleTts2Model()   //! the shared StyleTTS2-lineage assembly, its voices and the family's driver data
    scratch : St2Scratch = St2Scratch()   //! the activation carrier every synthesis reuses
    g2p : G2pModel                          //! ``tts_g2p.bin``: lexicons, rules and the fallback chain; empty where no voice speaks English and the directory has none
    tagger : PosTagger                      //! ``tts_postag.bin``: the tokenizer and part-of-speech tagger
    ru_stress : RuStressPack                //! ``tts_g2p_ru.bin``: the Russian stress tables; empty where no voice speaks Russian
    ru_stress_model : RuStressModel = RuStressModel()   //! ``tts_ctx_ru.gguf``: the Russian stress-context model, read where a voice speaks Russian and the file sits beside the model; empty otherwise
    ru_stress_scratch : RuStressScratch = RuStressScratch()   //! its activation carrier, reused by every synthesis chunk
    pocket : PocketModel = PocketModel()    //! the Pocket TTS family's model, when the GGUF is one; it reads text, not phonemes
    pocket_scratch : PocketScratch = PocketScratch()   //! its activation carrier
    chunk_chars : int = TTS_CHUNK_CHARS     //! the longest chunk a synthesis speaks, in codepoints (``tts_set_chunk_chars``)
}

// Stated rather than inferred: the shared carrier holds borrowed planes, and a generated
// teardown would force every module that deletes a TtsModel to see the plane finalizers too.
def finalize(var m : TtsModel) {
    //! Free the model's planes, the carrier and both packs; ``delete m`` runs it.
    delete m.model
    delete m.scratch
    delete m.g2p
    delete m.tagger
    delete m.ru_stress
    delete m.ru_stress_model
    delete m.ru_stress_scratch
    delete m.pocket
    delete m.pocket_scratch
}

[arch(at = "../ARCHITECTURE_TTS_MEMORY.md#tts-idle-release")]
def tts_release_scratch(var m : TtsModel) {
    //! Free the activation scratch a synthesis or a clone left behind - the carrier of either family (a
    //! StyleTTS2 carrier whose noise the parity rail captured stays) and the block home's scratch globals;
    //! the next run grows them back for a few milliseconds. For a platform where idle memory matters, called when nothing is queued.
    if (!m.scratch.noise.captured) {
        delete m.scratch
    }
    delete m.ru_stress_scratch
    pocket_release_scratch(m.pocket_scratch)   // the block home's scratch globals go with it, whichever family grew them
}

[arch(at = "../ARCHITECTURE_TTS_MEMORY.md#tts-idle-release")]
def tts_set_chunk_chars(var m : TtsModel; chars : int) {
    //! Cap the chunk a synthesis speaks at ``chars`` codepoints (default ``TTS_CHUNK_CHARS``; under one is
    //! refused): the peak memory a say holds is the largest chunk's, so a memory-tight platform lowers it and
    //! the same text speaks in more, shorter pieces, each its own synthesis. Pocket's chunks take no cap.
    if (chars < 1) {
        panic("dasLLAMA tts: tts_set_chunk_chars({chars}) - the chunk cap takes at least one codepoint")
    }
    m.chunk_chars = chars
    if (m.kind == TtsKind.pocket) {
        to_log(LOG_INFO, "dasLLAMA tts: tts_set_chunk_chars({chars}): a Pocket model chunks by its tokenizer's budget - the cap does not reach it\n")
        return
    }
    let which = chars == TTS_CHUNK_CHARS ? "the default" : "off the default {TTS_CHUNK_CHARS}"
    to_log(LOG_INFO, "dasLLAMA tts: tts_set_chunk_chars: a synthesis speaks {chars} codepoints a chunk - {which}\n")
}

def private voice_lang(m : TtsModel; vname : string) : string {
    //! The language a canonical voice was trained on: Kitten's voices are all American
    //! English; a Kokoro pack's name carries its language; a Pocket voice speaks its model's.
    if (m.kind == TtsKind.pocket) {
        return m.pocket.lang
    }
    return m.kind == TtsKind.kitten ? "en-us" : kokoro_voice_lang(m.model.kokoro, vname)
}

def load_tts_model(path : string) : TtsModel {
    //! Load a TTS model: a converted GGUF (KittenTTS, Kokoro or Pocket TTS, picked from its architecture) or its prepared ``.dlim`` image, on the lane ``tts_serves_q8`` names (an image: the lane it was baked on);
    //! a phoneme family reads the packs its voices' languages ask for from the same directory (``tts_pack_names``; Pocket needs none).
    //! A DIRECTORY loads the front end alone, with no voice - its packs and the Russian stress-context model where that sits there (``tts_front_end_only``).
    var inscope m = TtsModel()
    if (stat(path).is_dir) {
        m.kind = TtsKind.front_end
        if (english_pack_present(path)) {
            load_english_packs(m, path)
        }
        if (stat(path_join(path, RU_PACK_NAME)).is_valid) {
            load_russian_packs(m, path)
        }
        if (!english_packs_loaded(m) && !ru_pack_loaded(m.ru_stress)) {
            panic("dasLLAMA tts: the directory '{path}' holds no front-end pack - {RU_PACK_NAME} for Russian, tts_g2p.bin with tts_postag.bin for English")
        }
        return <- m
    }
    if (!(path |> ends_with(".dlim")) && gguf_file_arch(path) == "pocket-tts") {
        m.kind = TtsKind.pocket
        m.pocket <- load_pocket(path)
        return <- m
    }
    m.model <- load_styletts2(path)
    tts_finish_styletts2(m, path)
    return <- m
}

def tts_finish_styletts2(var m : TtsModel; path : string) {
    //! A StyleTTS2 carrier in ``m.model``, loaded or minted from ``path``, made a facade: its family's kind, the
    //! packs its voices' languages ask for from the same directory (the English pair also where it merely sits
    //! there, for the Latin words inside another language's text), the voices it cannot speak said once.
    if (m.model.family == "kitten-tts") {
        m.kind = TtsKind.kitten
    } elif (m.model.family == "kokoro") {
        m.kind = TtsKind.kokoro
    } else {
        panic("dasLLAMA tts: '{path}' has architecture '{m.model.family}', which is not a TTS family")
    }
    let dir = dir_name(path)
    var speaks_english = m.kind == TtsKind.kitten
    var speaks_russian = false
    for (v in m.model.voice_names) {
        let lang = voice_lang(m, v)
        speaks_english = speaks_english || (lang |> starts_with("en"))
        speaks_russian = speaks_russian || lang == "ru"
    }
    if (speaks_english || english_pack_present(dir)) {
        load_english_packs(m, dir)
    }
    if (speaks_russian) {
        load_russian_packs(m, dir)
    }
    log_dropped_voices(m)
}

//! the English phoneme pack and the tagger of `dir` into `m`
def private load_english_packs(var m : TtsModel; dir : string) {
    m.g2p <- load_g2p(g2p_pack_path(dir))
    m.tagger <- load_pos_tagger(path_join(dir, "tts_postag.bin"))
}

//! the Russian stress pack of `dir` into `m`, and the stress-context model where its file sits there
def private load_russian_packs(var m : TtsModel; dir : string) {
    m.ru_stress <- load_ru_stress_pack(path_join(dir, RU_PACK_NAME))
    let stress_model = tts_ru_stress_model_path(dir)
    if (!empty(stress_model)) {
        m.ru_stress_model <- load_ru_stress(stress_model, tts_serves_q8())
        to_log(LOG_INFO, "dasLLAMA tts: the Russian stress-context model {RU_STRESS_MODEL_NAME} ({m.ru_stress_model.encoder}, {length(m.ru_stress_model.layers)} layers) picks the stress of the words with two readings\n")
    }
}

def private english_packs_loaded(m : TtsModel) : bool => !empty(m.g2p.data)

let private RU_PACK_NAME = "tts_g2p_ru.bin"
let private RU_STRESS_MODEL_NAME = "tts_ctx_ru.gguf"

def tts_ru_stress_model_path(dir : string) : string {
    //! The Russian stress-context model a voice in ``dir`` reads - ``tts_ctx_ru.gguf`` there - or "" where the
    //! directory has none: the file is optional, and without it the pack alone decides the stress.
    let path = path_join(dir, RU_STRESS_MODEL_NAME)
    return stat(path).is_valid ? path : ""
}

//! the phoneme pack's two tiers in a directory, the full pack first
def private english_pack_tiers(dir : string) : array<string> => [path_join(dir, "tts_g2p.bin"), path_join(dir, "tts_g2p_en_us.bin")]

def private english_pack_present(dir : string) : bool {
    for (p in english_pack_tiers(dir)) {
        return true if (stat(p).is_valid)
    }
    return false
}

//! the English packs as a program checks for them in `dir`: the phoneme pack tier the directory holds (the full pack
//! unless only the American-only twin is there, `g2p_pack_path`'s own order) and the tagger
def private english_pack_names(dir : string) : array<string> {
    var inscope tiers <- english_pack_tiers(dir)
    let twin_alone = !stat(tiers[0]).is_valid && stat(tiers[1]).is_valid
    var inscope names : array<string>
    names |> push(base_name(tiers[twin_alone ? 1 : 0]))
    names |> push("tts_postag.bin")
    return <- names
}

//! the GGUF a prepared `.dlim` was minted from, by its name beside it (`<gguf>.<lane>.dlim`); "" where none sits there
def private image_source_gguf(path : string) : string {
    let image = base_name(path)
    let stem = slice(image, 0, length(image) - 5)
    let lane = rfind(stem, ".")
    return "" if (lane <= 0)
    let gguf = path_join(dir_name(path), slice(stem, 0, lane))
    return stat(gguf).is_valid ? gguf : ""
}

//! The phoneme pack a model directory serves: the full pack when present, else the American-only
//! twin; neither is a panic naming the directory.
[arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#tts-g2p-pack-tiers")]
def g2p_pack_path(dir : string) : string {
    var inscope tiers <- english_pack_tiers(dir)
    for (p in tiers) {
        return p if (stat(p).is_valid)
    }
    panic("dasLLAMA tts: no phoneme pack beside the model - neither {tiers[0]} nor {tiers[1]}")
    return ""
}

def tts_pack_names(path : string) : array<string> {
    //! The front-end packs the model at ``path`` reads from its directory, decided from the file's own metadata before any load:
    //! the phoneme pack tier the directory holds and ``tts_postag.bin`` where a voice speaks English, ``tts_g2p_ru.bin`` where one
    //! speaks Russian, none for a Pocket GGUF; a ``.dlim`` answers for the GGUF beside it, and names the English pair where none is; a directory - a front end with no voice - names the packs it holds.
    var inscope out : array<string>
    if (stat(path).is_dir) {
        if (english_pack_present(path)) {
            var inscope names <- english_pack_names(path)
            out |> push_from(names)
        }
        if (stat(path_join(path, RU_PACK_NAME)).is_valid) {
            out |> push(RU_PACK_NAME)
        }
        return <- out
    }
    let dir = dir_name(path)
    if (path |> ends_with(".dlim")) {
        let source = image_source_gguf(path)
        return <- tts_pack_names(source) if (!empty(source))
        return <- english_pack_names(dir)
    }
    let arch = gguf_file_arch(path)
    return <- out if (arch == "pocket-tts")
    var english = arch != "kokoro"
    var russian = false
    if (arch == "kokoro") {
        gguf_header_read(path) $(m; bytes) {
            var inscope voices <- gguf_str_array(m, bytes, "styletts2.voices")
            var inscope langs <- gguf_has(m, "kokoro.voice_langs") ? gguf_str_array(m, bytes, "kokoro.voice_langs") : array<string>()
            for (i, v in iter_range(voices), voices) {
                let declared = i < length(langs) ? langs[i] : ""
                let lang = empty(declared) ? kokoro_voice_lang(v) : declared
                english = english || (lang |> starts_with("en"))
                russian = russian || lang == "ru"
            }
        }
    }
    if (english) {
        var inscope names <- english_pack_names(dir)
        out |> push_from(names)
    }
    if (russian) {
        out |> push(RU_PACK_NAME)
    }
    return <- out
}

[cold_path]
def private log_dropped_voices(m : TtsModel) {
    //! Name, once per load, the voice packs ``caps`` leaves out: the front end phonemizes none
    //! of their languages, so a caller asking for one gets a refusal rather than a wrong accent.
    return if (m.kind == TtsKind.pocket)   // every roster voice speaks the model's language
    var inscope langs <- front_end_langs(m)
    var inscope dropped : array<string>
    for (v in m.model.voice_names) {
        continue if (find_index(langs, voice_lang(m, v)) >= 0)
        dropped |> push(v)
    }
    return if (empty(dropped))
    to_log(LOG_INFO, "dasLLAMA tts: {length(dropped)} voice pack(s) speak a language this front end does not phonemize, and are not offered: {join(dropped, ", ")}\n")
}

def set_tts_q8(on : bool) {
    //! Pin the GEMM weights' format for subsequent TTS loads of every family: Q8_0 quants (their
    //! own prepared image beside the GGUF) or the file's f32 planes; ``reset_tts_q8`` returns to
    //! the policy default. The pin is per context: a worker thread pins for itself.
    set_styletts2_q8(on)
    set_pocket_q8(on)
}

def reset_tts_q8() {
    //! Drop the ``set_tts_q8`` pin: the next load follows the policy default again.
    reset_styletts2_q8()
    reset_pocket_q8()
}

def tts_serves_q8() : bool {
    //! Would the next TTS load serve its rows GEMMs as q8 - the pin when set, the policy otherwise.
    return styletts2_serves_q8()
}

def private front_end_langs(m : TtsModel) : array<string> {
    //! The languages the front end reads for this model, as the reference pipeline's codes: American
    //! English where the English packs are loaded, British too for Kokoro when the pack carries that
    //! tier, Russian where the Russian pack is, and for a Pocket model the one language it was trained for.
    var inscope langs : array<string>
    if (m.kind == TtsKind.pocket) {
        langs |> push(m.pocket.lang)
        return <- langs
    }
    if (english_packs_loaded(m)) {
        langs |> push("en-us")
        if ((m.kind == TtsKind.kokoro || m.kind == TtsKind.front_end) && !m.g2p.us_only) {
            langs |> push("en-gb")
        }
    }
    if (ru_pack_loaded(m.ru_stress)) {
        langs |> push("ru")
    }
    return <- langs
}

def private family_name(m : TtsModel) : string {
    return "a front end with no voice" if (m.kind == TtsKind.front_end)
    return m.kind == TtsKind.pocket ? "pocket-tts {m.pocket.name}" : "{m.model.family} {m.model.name}"
}

def private sample_rate_of(m : TtsModel) : int =>
    m.kind == TtsKind.pocket ? m.pocket.sample_rate : m.model.sample_rate

def caps(m : TtsModel) : TtsCaps {
    //! What the loaded model offers: the canonical voice names the front end can drive, each with
    //! its language (a voice whose language it does not phonemize is left out; aliases resolve at
    //! synthesis), the PCM rate, the languages, whether it clones a voice, whether a speed applies.
    var c = TtsCaps(sample_rate = sample_rate_of(m), cloning = m.kind == TtsKind.pocket && m.pocket.cloning, speed = m.kind != TtsKind.pocket)
    c.langs <- front_end_langs(m)
    let names & = unsafe(m.kind == TtsKind.pocket ? m.pocket.voice_names : m.model.voice_names)
    for (v in names) {
        let lang = voice_lang(m, v)
        continue if (find_index(c.langs, lang) < 0)
        c.voices |> push(v)
        c.voice_langs |> push(lang)
    }
    return <- c
}

def tts_register_voice(var m : TtsModel; name : string; pcm : array<float>; sample_rate : int) {
    //! Clone a voice: ``pcm`` (mono, at the model's own sample rate - ``caps().sample_rate``) joins
    //! the roster under ``name`` and speaks from the next synthesis on. A model whose ``caps()`` says
    //! it cannot clone panics here.
    if (m.kind != TtsKind.pocket) {
        panic("dasLLAMA tts: {family_name(m)} cannot clone a voice")
    }
    if (sample_rate != m.pocket.sample_rate) {
        panic("dasLLAMA tts: a voice clip at {sample_rate} Hz; this model takes {m.pocket.sample_rate} Hz")
    }
    pocket_register_voice(m.pocket, name, pcm, m.pocket_scratch)
}

struct TtsVoicePrompt {
    //! Portable Pocket codec latents. Prepare with an encoder-bearing model; register with any
    //! model of the same latent space (`latent_space`), whichever config name or size it carries and
    //! whether or not it ships the encoder. Serialization is caller-owned.
    version : int = 2                         //! Serialized prompt format version.
    family : string = "pocket"                //! Model family tag.
    latent_space : uint64                    //! Fingerprint of the latent space the frames were encoded in.
    language : string                        //! Language of the preparing model, informational.
    sample_rate : int                        //! Reference audio sample rate in hertz.
    latent_dim : int64                       //! Number of codec latent values per frame.
    frame_samples : int64                    //! Number of PCM samples represented by each frame.
    latents : array<float>                   //! Flattened codec latent frames.
}

def tts_voice_prompt_valid(m : TtsModel; prompt : TtsVoicePrompt) : bool {
    //! Validate bounded finite latent frames and the model's latent space and codec geometry before registration.
    if (m.kind != TtsKind.pocket || prompt.version != 2 || prompt.family != "pocket" || prompt.latent_space != m.pocket.latent_space_id || prompt.sample_rate != m.pocket.sample_rate || prompt.sample_rate <= 0 || prompt.latent_dim != m.pocket.latent_dim || prompt.latent_dim <= 0l || prompt.frame_samples != m.pocket.frame_samples || prompt.frame_samples <= 0l || empty(prompt.latents)) {
        return false
    }
    let frames = long_length(prompt.latents) / prompt.latent_dim
    if (long_length(prompt.latents) % prompt.latent_dim != 0l || frames > (POCKET_MAX_VOICE_SECONDS * int64(prompt.sample_rate) + prompt.frame_samples - 1l) / prompt.frame_samples) {
        return false
    }
    for (v in prompt.latents) {
        if (!(v >= -1000000.0 && v <= 1000000.0)) { return false }
    }
    return true
}

def tts_prepare_voice_prompt(var m : TtsModel; pcm : array<float>; sample_rate : int) : TtsVoicePrompt {
    //! Encode a reference once for shipping as a voice prompt. The receiving model must read the
    //! same latent space (`latent_space`); incompatible data is rejected on registration.
    if (m.kind != TtsKind.pocket || sample_rate != m.pocket.sample_rate || empty(pcm) || long_length(pcm) > POCKET_MAX_VOICE_SECONDS * int64(sample_rate)) {
        panic("dasLLAMA tts: a voice prompt requires a compatible encoder and a nonempty reference of at most 60 seconds")
    }
    var prompt = TtsVoicePrompt(latent_space = m.pocket.latent_space_id, language = m.pocket.language, sample_rate = sample_rate, latent_dim = m.pocket.latent_dim, frame_samples = m.pocket.frame_samples)
    var inscope clip : array<float>
    pocket_prompt_pcm(m.pocket, pcm, clip)
    pocket_encode_latents(m.pocket, clip, m.pocket_scratch, prompt.latents)
    return <- prompt
}

def tts_register_voice_prompt(var m : TtsModel; name : string; prompt : TtsVoicePrompt) : bool {
    //! Add or replace a named prepared voice without a codec encoder. Failure preserves the roster;
    //! the prompt is copied and its backbone state is built lazily at the first synthesis.
    if (empty(name) || !tts_voice_prompt_valid(m, prompt)) { return false }
    var latents <- clone(prompt.latents)
    pocket_voice_clear(m.pocket, name)
    m.pocket.voice_latents[name] <- latents
    return true
}

def tts_voice_lang(m : TtsModel; voice : string) : string {
    //! The language the front end reads ``voice`` in - a canonical name from ``caps`` or a family
    //! alias, resolved the way ``synthesize`` resolves it. A voice this model cannot drive panics
    //! here with the message a synthesis would give, rather than making the caller speak a line.
    return voice_lang(m, resolve_voice(m, voice))
}

// ===== front end =====

def tts_normalize(text : string) : string {
    //! The spoken form of ``text``: the normalization pass every synthesis runs first (rules
    //! alone - it reads no pack), where "Dr." reads "Doctor", "3.5" reads "three point five" and
    //! "$12" reads "twelve dollars".
    return normalize_text(text)
}

def tts_normalize(text : string; lang : string) : string {
    //! The same in the language ``lang`` names - a code from ``caps``: Russian ("ru") reads "в 2024 году" as
    //! "в две тысячи двадцать четвёртом году", an English code ("en-us", "en-gb", "en") reads English, and any other
    //! language's text comes back as it is - a Pocket model's other languages read their text whole, as their reference does.
    return normalize_text_ru(text) if (lang == "ru")
    return normalize_text(text) if (lang |> starts_with("en"))
    return text
}

//! `text` with every Latin run turned into English phonemes; the stress marks sit inside Cyrillic words, which no run reaches
def private latin_runs_spliced(m : TtsModel; text : string) : string {
    var inscope cps <- utf8_to_cpts(text)
    let n = length(cps)
    return build_string() $(var wr) {
        var i = 0
        while (i < n) {
            let cp = cps[i]
            let latin = is_alpha(cp)
            if (!latin) {
                wr |> write(cpt_to_utf8(cp))
                i++
                continue
            }
            var j = i
            while (j < n && ((is_alpha(cps[j])) || (j > i && (cps[j] == '\'' || cps[j] == '-') && j + 1 < n && (is_alpha(cps[j + 1]))))) {
                j++
            }
            wr |> write(strip(g2p_phonemize(m.g2p, m.tagger, cpts_to_utf8(cps, i, j))))
            i = j
        }
    }
}

//! Normalized Russian text with `+` before every stressed vowel: the context model's reading where its file is loaded, the pack's own tiers where not
[arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#tts-stress-context")]
def private ru_stress_marked(m : TtsModel; norm : string; var sc : RuStressScratch) : string {
    return ru_mark_stress(m.ru_stress, norm) if (!ru_stress_loaded(m.ru_stress_model))
    return ru_mark_stress_in_context(m.ru_stress_model, m.ru_stress, norm, sc) if (is_job_que_available())
    var marked = ""
    with_dasllama_jobque_() {   // a caller outside any job queue: the context pass runs on one of its own
        marked = ru_mark_stress_in_context(m.ru_stress_model, m.ru_stress, norm, sc)
    }
    return marked
}

//! Normalized Russian text to phonemes: the stress marked on the text as written, then a Latin run passed to the Russian reader as English phonemes where the English packs are loaded, as letters where not
[arch(at = "../ARCHITECTURE_TTS_FRONT_END.md#tts-stress-context")]
def private ru_phonemize_latin_spliced(m : TtsModel; norm : string; var sc : RuStressScratch) : string {
    let marked = ru_stress_marked(m, norm, sc)
    return ru_phonemize_marked(english_packs_loaded(m) ? latin_runs_spliced(m, marked) : marked)
}

def private phonemize_for(m : TtsModel; norm : string; lang : string; var sc : RuStressScratch) : string =>
    lang == "ru" ? ru_phonemize_latin_spliced(m, norm, sc) : g2p_phonemize(m.g2p, m.tagger, norm, lang == "en-gb")

def tts_needs_packs(path : string) : bool {
    //! Whether the model at ``path`` reads any front-end pack from its directory: the phoneme families do
    //! (``tts_pack_names`` says which), a Pocket TTS GGUF reads text and needs none - decided before any
    //! load, by the architecture test ``load_tts_model`` makes (a ``.dlim`` is a phoneme family).
    return (path |> ends_with(".dlim")) || stat(path).is_dir || gguf_file_arch(path) != "pocket-tts"
}

def tts_has_phonemes(m : TtsModel) : bool {
    //! Whether ``tts_phonemize`` has an answer for this model: the phoneme families yes, in the languages ``caps().langs``
    //! lists (the two-argument form's English needs the English packs beside the model), a Pocket model no - it reads text.
    return m.kind != TtsKind.pocket
}

def tts_front_end_only(m : TtsModel) : bool {
    //! Whether ``m`` is a front end loaded from a directory: it normalizes, marks stress and phonemizes in the
    //! languages ``caps().langs`` lists, and has no voice - ``caps().voices`` is empty and a synthesis panics.
    return m.kind == TtsKind.front_end
}

def private phoneme_family_only(m : TtsModel) {
    if (m.kind == TtsKind.pocket) {
        panic("dasLLAMA tts: {family_name(m)} reads text, not phonemes - nothing to phonemize")
    }
}

def tts_phonemize(m : TtsModel; text : string) : string {
    //! The phonemes one already-normalized sentence becomes, in the front end's own American
    //! inventory (``tts_postag.bin`` tags the tokens, ``tts_g2p.bin`` reads them); a family wanting other
    //! symbols rewrites them. A Pocket model panics, and so does a model with no English pack beside it.
    return tts_phonemize(m, text, "en-us")
}

def tts_phonemize(m : TtsModel; text : string; lang : string) : string {
    //! The same, in the inventory ``lang`` names - a language code from ``caps``: ``"en-us"``, ``"en-gb"``,
    //! or ``"ru"`` (the Russian front end, a Latin word inside read by the English one where its packs are
    //! loaded). A language this model's front end does not phonemize panics rather than answering in another.
    phoneme_family_only(m)
    var inscope langs <- front_end_langs(m)
    if (find_index(langs, lang) < 0) {
        panic("dasLLAMA tts: this front end does not phonemize '{lang}'")
    }
    var inscope sc = RuStressScratch()
    return phonemize_for(m, text, lang, sc)
}

def tts_mark_stress(m : TtsModel; text : string; lang : string) : string {
    //! One already-normalized sentence with ``+`` before every stressed vowel, as the phonemizer of ``lang`` reads it: Russian ("ru")
    //! marks each stressed word - "в з+амке" - by the stress pack, and by the stress-context model where its file sits beside the voice.
    //! A language whose front end marks no text answers the empty string; a Pocket model panics, as ``tts_phonemize`` does.
    phoneme_family_only(m)
    return "" if (lang != "ru" || !ru_pack_loaded(m.ru_stress))
    var inscope sc = RuStressScratch()
    return ru_stress_marked(m, text, sc)
}

// ===== chunking =====

//! The longest chunk of normalized text one synthesis takes: the reference driver's limit, and
//! well inside the 510-symbol context every family's encoder was trained to.
let TTS_CHUNK_CHARS = 400

// abbreviations whose trailing period is not a sentence end (the reference driver's set)
let private NON_BOUNDARY_ABBREVIATIONS <- {"dr", "prof", "mr", "mrs", "ms", "fig", "figs", "pp", "p", "ch", "sec",
    "jan", "feb", "mar", "apr", "jun", "jul", "aug", "sep", "sept", "oct", "nov", "dec", "al"}

// the letters running up to index `i` (exclusive), lowercased
def private word_before(cps : array<int>; i : int) : string {
    var j = i
    while (j > 0 && is_alpha(cps[j - 1])) {
        j--
    }
    return to_lower(cpts_to_utf8(cps, j, i))
}

// the reference driver's sentence-boundary rule: . ! ? followed by space or the end, except a
// decimal point, a listed abbreviation, "a.m." / "p.m." (unless a capital follows the "m.")
def private is_sentence_boundary(cps : array<int>; i : int) : bool {
    let c = cps[i]
    return false if (c != '.' && c != '!' && c != '?')
    let n = length(cps)
    if (c == '.') {
        return false if (i > 0 && i < n - 1 && cpt_is_number(cps[i - 1]) && cpt_is_number(cps[i + 1]))
        let tok = word_before(cps, i)
        return false if (key_exists(NON_BOUNDARY_ABBREVIATIONS, tok) ||
            ((tok == "a" || tok == "p") && i + 1 < n && (cps[i + 1] == 'm' || cps[i + 1] == 'M')))
        if (tok == "m" && i >= 3 && cps[i - 2] == '.' && (cps[i - 3] == 'a' || cps[i - 3] == 'p' || cps[i - 3] == 'A' || cps[i - 3] == 'P')) {
            var k = i + 1
            while (k < n && is_space_cpt(cps[k])) {
                k++
            }
            return k >= n || int(utf32_to_lower(uint(cps[k]))) != cps[k]
        }
    }
    return i + 1 >= n || is_space_cpt(cps[i + 1])
}

//! the splitter's whitespace: the four the reference driver splits on, not `is_white_space`'s form feed and vertical tab
def private is_space_cpt(c : int) : bool => c == ' ' || c == '\t' || c == '\n' || c == '\r'

// a chunk ends on prosodic punctuation when the family's driver says so (Kitten's appends a comma
// where the split left none; Kokoro's pipeline sends the text as it is)
def private finish_chunk(chunk : string; punct : bool) : string {
    let s = strip(chunk)
    return s if (empty(s) || !punct)
    var inscope cps <- utf8_to_cpts(s)
    let last = cps[length(cps) - 1]
    return (last == '.' || last == '!' || last == '?' || last == ',' || last == ';' || last == ':') ? s : "{s},"
}

// a whitespace-free run longer than the cap has no word boundary to split on, so it splits at
// the cap itself, on a codepoint boundary
def private push_hard_split(word : string; max_len : int; punct : bool; var out : array<string>) {
    var inscope cps <- utf8_to_cpts(word)
    let n = length(cps)
    var a = 0
    while (a < n) {
        let b = min(a + max_len, n)
        out |> push(finish_chunk(cpts_to_utf8(cps, a, b), punct))
        a = b
    }
}

def private push_words(sentence : string; max_len : int; punct : bool; var out : array<string>) {
    var inscope cur : array<string>
    var cur_len = 0
    for (w in split(sentence, " ")) {
        continue if (empty(w))
        let wl = cpt_count(w)
        if (!empty(cur) && cur_len + 1 + wl > max_len) {
            out |> push(finish_chunk(join(cur, " "), punct))
            cur |> clear()
            cur_len = 0
        }
        if (wl > max_len) {
            push_hard_split(w, max_len, punct, out)
            continue
        }
        cur_len += (empty(cur) ? 0 : 1) + wl
        cur |> push(w)
    }
    if (!empty(cur)) {
        out |> push(finish_chunk(join(cur, " "), punct))
    }
}

def tts_chunks(norm : string; max_len : int = TTS_CHUNK_CHARS; ensure_punct : bool = true) : array<string> {
    //! Split normalized text into the pieces one synthesis takes: sentences, each at most
    //! ``max_len`` codepoints (a longer sentence splits on words, a longer word at the cap). With
    //! ``ensure_punct`` a chunk the split left bare gets a comma - KittenTTS's driver rule.
    var inscope out : array<string>
    var inscope cps <- utf8_to_cpts(norm)
    var start = 0
    var inscope sentences : array<string>
    for (i in range(length(cps))) {
        if (is_sentence_boundary(cps, i)) {
            sentences |> push(cpts_to_utf8(cps, start, i + 1))
            start = i + 1
        }
    }
    if (start < length(cps)) {
        sentences |> push(cpts_to_utf8(cps, start, length(cps)))
    }
    for (sent in sentences) {
        let st = strip(sent)
        continue if (empty(st))
        if (cpt_count(st) <= max_len) {
            out |> push(finish_chunk(st, ensure_punct))
        } else {
            push_words(st, max_len, ensure_punct, out)
        }
    }
    return <- out
}

def tts_chunks(m : TtsModel; norm : string) : array<string> {
    //! The chunks a synthesis on this model takes, by the family's own driver rule: Kitten's appends
    //! a comma to a chunk the split left bare, Kokoro's sends the text as it is (its voices render an
    //! added mark as a breath), Pocket's splits on its tokenizer's sentence marks under a token budget.
    if (m.kind == TtsKind.pocket) {
        return <- pocket_chunks(m.pocket, norm)
    }
    return <- tts_chunks(norm, m.chunk_chars, m.kind == TtsKind.kitten)
}

// ===== synthesis =====

[cold_path]
def private add_timings(var acc : TtsTimings; t : TtsTimings) {
    acc.front_end_us += t.front_end_us
    acc.bert_us += t.bert_us
    acc.text_enc_us += t.text_enc_us
    acc.duration_us += t.duration_us
    acc.prosody_us += t.prosody_us
    acc.decoder_us += t.decoder_us
    acc.source_us += t.source_us
    acc.generator_us += t.generator_us
    acc.prompt_us += t.prompt_us
    acc.backbone_us += t.backbone_us
    acc.head_us += t.head_us
    acc.codec_us += t.codec_us
    acc.total_us += t.total_us
    acc.samples += t.samples
}

let private NOISE_SEED = 0x5eed1ul   // the chunk index folds in: consecutive chunks draw different noise

def private synthesize_chunk_pocket(var m : TtsModel; chunk : string; vname : string; speed : float; chunk_index : int) : TtsAudio {
    if (speed != 1.0) {
        panic("dasLLAMA tts: {family_name(m)} has no speed control; asked for {speed}")
    }
    var inscope out = TtsAudio()
    out.sample_rate = m.pocket.sample_rate
    pocket_speak(m.pocket, chunk, vname, m.pocket.default_temperature, NOISE_SEED + uint64(chunk_index), out.timings, m.pocket_scratch)
    out.pcm := m.pocket_scratch.wave
    return <- out
}

def private synthesize_chunk(var m : TtsModel; norm : string; vname, lang : string; speed : float; chunk_index : int) : TtsAudio {
    if (m.kind == TtsKind.pocket) {
        return <- synthesize_chunk_pocket(m, norm, vname, speed, chunk_index)
    }
    var inscope out = TtsAudio()
    out.sample_rate = m.model.sample_rate
    let clock = ref_time_ticks()
    let ps_misaki = phonemize_for(m, norm, lang, m.ru_stress_scratch)
    let is_kitten = m.kind == TtsKind.kitten
    var inscope ids <- is_kitten ? kitten_token_ids(kitten_phonemes_from_misaki(ps_misaki)) : kokoro_token_ids(m.model.kokoro, ps_misaki)
    let row = is_kitten ? kitten_style_row(norm, m.model.style_rows) : kokoro_style_row(ps_misaki, m.model.style_rows)
    let sp = is_kitten ? speed * (m.model.kitten.speed_priors?[vname] ?? 1.0) : speed
    var inscope style : array<float>
    styletts2_style(m.model, vname, row, style)
    out.timings.front_end_us = int64(get_time_usec(clock))
    styletts2_synthesize(m.model, ids, style, sp, m.scratch.noise, NOISE_SEED + uint64(chunk_index), null, out.timings, m.scratch)
    out.pcm := m.scratch.wave
    if (is_kitten) {
        kitten_trim(out.pcm)
    }
    out.timings.samples = long_length(out.pcm)
    return <- out
}

def private resolve_voice(m : TtsModel; voice : string) : string {
    if (m.kind == TtsKind.front_end) {
        panic("dasLLAMA tts: a front end loaded from a directory has no voice to speak - load a voice model to synthesize")
    }
    var vname = ""
    if (m.kind == TtsKind.pocket) {
        vname = pocket_has_voice(m.pocket, voice) ? voice : ""
    } elif (m.kind == TtsKind.kitten) {
        vname = kitten_voice(m.model.kitten, m.model, voice)
    } elif (styletts2_has_voice(m.model, voice)) {
        vname = voice
    }
    if (empty(vname)) {
        panic("dasLLAMA tts: unknown voice '{voice}'")
    }
    let lang = voice_lang(m, vname)
    var inscope langs <- front_end_langs(m)
    if (find_index(langs, lang) < 0) {
        // a pack whose name carries no language of the pipeline's own shape has none to name
        let says = empty(lang) ? "speaks a language the front end does not phonemize" : "speaks {lang}, which it does not phonemize"
        panic("dasLLAMA tts: unknown voice '{voice}' for this front end - the pack {says}")
    }
    return vname
}

def synthesize_stream(var m : TtsModel; text : string; voice : string; speed : float; blk : block<(chunk : TtsAudio)>) {
    //! Text -> speech, one sentence-sized chunk at a time: the block receives each chunk's audio as
    //! soon as it exists, in order. Normalization runs once over the whole text; ``voice`` is a
    //! name from ``caps`` or a family alias, and an unknown one panics.
    let vname = resolve_voice(m, voice)
    let lang = voice_lang(m, vname)
    let norm = tts_normalize(text, lang)
    var inscope chunks <- tts_chunks(m, norm)
    for (i, chunk in iter_range(chunks), chunks) {
        var inscope a <- synthesize_chunk(m, chunk, vname, lang, speed, i)
        to_log(LOG_INFO, "dasLLAMA tts: {family_name(m)}: {timings_line(a.timings, a.sample_rate)}\n")
        invoke(blk, a)
    }
}

def synthesize(var m : TtsModel; text : string; voice : string; speed : float = 1.0) : TtsAudio {
    //! Text -> speech: normalize, chunk by sentence, phonemize, map into the family's symbols,
    //! synthesize; the chunks concatenate, the timings sum. Sample-identical to
    //! ``synthesize_stream`` over the same text.
    var inscope out = TtsAudio()
    out.sample_rate = sample_rate_of(m)
    synthesize_stream(m, text, voice, speed) $(chunk) {
        out.pcm |> push_from(chunk.pcm)
        add_timings(out.timings, chunk.timings)
    }
    return <- out
}
