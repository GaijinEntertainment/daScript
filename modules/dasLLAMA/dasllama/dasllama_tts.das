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

struct TtsModel {
    //! A loaded TTS model: the shared assembly, the family's driver data, and the front-end packs
    //! read beside the GGUF. Load with ``load_tts_model``; ``delete`` frees every plane and pack.
    kind : TtsKind                          //! the family the GGUF's architecture named
    model : StyleTts2Model = StyleTts2Model()   //! the shared StyleTTS2-lineage assembly, its voices and the family's driver data
    scratch : St2Scratch = St2Scratch()   //! the activation carrier every synthesis reuses
    g2p : G2pModel                          //! ``tts_g2p.bin``: lexicons, rules and the fallback chain
    tagger : PosTagger                      //! ``tts_postag.bin``: the tokenizer and part-of-speech tagger
}

// Stated rather than inferred: the shared carrier holds borrowed planes, and a generated
// teardown would force every module that deletes a TtsModel to see the plane finalizers too.
def finalize(var m : TtsModel) {
    //! Free the model's planes, the carrier and both packs; ``delete m`` runs it.
    delete m.model
    delete m.scratch
    delete m.g2p
    delete m.tagger
}

def load_tts_model(path : string) : TtsModel {
    //! Load a TTS model: a converted GGUF (KittenTTS or Kokoro, picked from its architecture) or its prepared
    //! ``.dlim`` image, which serves without the GGUF; ``tts_g2p.bin`` and ``tts_postag.bin`` come from the same directory.
    //! A GGUF serves the rows GEMMs on the lane ``tts_serves_q8`` names at load time; an image serves the lane it was baked on.
    var inscope m = TtsModel()
    m.model <- load_styletts2(path)
    if (m.model.family == "kitten-tts") {
        m.kind = TtsKind.kitten
    } elif (m.model.family == "kokoro") {
        m.kind = TtsKind.kokoro
    } else {
        panic("dasLLAMA tts: '{path}' has architecture '{m.model.family}', which is not a TTS family")
    }
    let dir = dir_name(path)
    m.g2p <- load_g2p(g2p_pack_path(dir))
    m.tagger <- load_pos_tagger(path_join(dir, "tts_postag.bin"))
    log_dropped_voices(m)
    return <- m
}

[arch(at = "../ARCHITECTURE_TTS.md#tts-g2p-pack-tiers")]
def private g2p_pack_path(dir : string) : string {
    let full = path_join(dir, "tts_g2p.bin")
    return full if (stat(full).is_valid)
    let us = path_join(dir, "tts_g2p_en_us.bin")
    if (!stat(us).is_valid) {
        panic("dasLLAMA tts: no phoneme pack beside the model - neither {full} nor {us}")
    }
    return us
}

[cold_path]
def private log_dropped_voices(m : TtsModel) {
    //! Name, once per load, the voice packs ``caps`` leaves out: the front end phonemizes none
    //! of their languages, so a caller asking for one gets a refusal rather than a wrong accent.
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
    //! Pin the rows GEMMs' format for subsequent TTS loads: Q8_0 quants (their own prepared image
    //! beside the GGUF) or the file's f32 planes; ``reset_tts_q8`` returns to the policy default.
    //! The pin is per context: a worker thread pins for itself.
    set_styletts2_q8(on)
}

def reset_tts_q8() {
    //! Drop the ``set_tts_q8`` pin: the next load follows the policy default again.
    reset_styletts2_q8()
}

def tts_serves_q8() : bool {
    //! Would the next TTS load serve its rows GEMMs as q8 - the pin when set, the policy otherwise.
    return styletts2_serves_q8()
}

def private front_end_langs(m : TtsModel) : array<string> {
    //! The languages the front end phonemizes for this model, as the reference pipeline's codes. Both
    //! families ship American English; Kokoro adds British (its ``bf_*`` / ``bm_*`` packs) when the phoneme
    //! pack carries the British tier. A family speaks a language only once its lexicon rung lands here.
    var inscope langs : array<string>
    langs |> push("en-us")
    if (m.kind == TtsKind.kokoro && !m.g2p.us_only) {
        langs |> push("en-gb")
    }
    return <- langs
}

def private voice_lang(m : TtsModel; vname : string) : string {
    //! The language a canonical voice was trained on: Kitten's voices are all American
    //! English; a Kokoro pack's name carries its language.
    return m.kind == TtsKind.kitten ? "en-us" : kokoro_voice_lang(vname)
}

def caps(m : TtsModel) : TtsCaps {
    //! What the loaded model offers: the canonical voice names the front end can drive, each with
    //! its language (a voice whose language it does not phonemize is left out; a family's aliases
    //! resolve at synthesis), the PCM sample rate, the languages, and whether it clones a voice.
    var c = TtsCaps(sample_rate = m.model.sample_rate, cloning = false)
    c.langs <- front_end_langs(m)
    for (v in m.model.voice_names) {
        let lang = voice_lang(m, v)
        continue if (find_index(c.langs, lang) < 0)
        c.voices |> push(v)
        c.voice_langs |> push(lang)
    }
    return <- c
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

def tts_phonemize(m : TtsModel; text : string) : string {
    //! The phonemes one already-normalized sentence becomes, in the front end's own American
    //! inventory: the tagger of ``tts_postag.bin`` labels the tokens, the grapheme-to-phoneme pass
    //! of ``tts_g2p.bin`` reads them. A family wanting other symbols rewrites this string into them.
    return g2p_phonemize(m.g2p, m.tagger, text)
}

def tts_phonemize(m : TtsModel; text : string; lang : string) : string {
    //! The same, in the inventory ``lang`` names - a language code from ``caps``, ``"en-us"`` or
    //! ``"en-gb"``. A language this model's front end does not phonemize panics rather than
    //! answering in the wrong dialect.
    var inscope langs <- front_end_langs(m)
    if (find_index(langs, lang) < 0) {
        panic("dasLLAMA tts: this front end does not phonemize '{lang}'")
    }
    return g2p_phonemize(m.g2p, m.tagger, text, lang == "en-gb")
}

// ===== chunking =====

//! The longest chunk of normalized text one synthesis takes: the reference driver's limit, and
//! well inside the 510-symbol context every family's encoder was trained to.
let TTS_CHUNK_CHARS = 400

// abbreviations whose trailing period is not a sentence end (the reference driver's set)
let private NON_BOUNDARY_ABBREVIATIONS <- {"dr", "prof", "mr", "mrs", "ms", "fig", "figs", "pp", "p", "ch", "sec",
    "jan", "feb", "mar", "apr", "jun", "jul", "aug", "sep", "sept", "oct", "nov", "dec", "al"}

def private is_space_cpt(c : int) : bool {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r'
}

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

// a chunk ends on prosodic punctuation when the family's driver says so (Kitten's appends a comma
// where the split left none; Kokoro's pipeline sends the text as it is)
def private finish_chunk(chunk : string; punct : bool) : string {
    let s = strip(chunk)
    return s if (empty(s) || !punct)
    var inscope cps <- utf8_to_cpts(s)
    let last = cps[length(cps) - 1]
    return (last == '.' || last == '!' || last == '?' || last == ',' || last == ';' || last == ':') ? s : "{s},"
}

// the cap counts what a reader says, so codepoints - `length()` on a string is BYTES, and an
// em dash or a curly quote costs three of them
def private cpt_length(s : string) : int {
    var inscope c <- utf8_to_cpts(s)
    return length(c)
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
        let wl = cpt_length(w)
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
        if (cpt_length(st) <= max_len) {
            out |> push(finish_chunk(st, ensure_punct))
        } else {
            push_words(st, max_len, ensure_punct, out)
        }
    }
    return <- out
}

def tts_chunks(m : TtsModel; norm : string) : array<string> {
    //! The chunks a synthesis on this model takes: the family's own driver rule - Kitten's
    //! appends a comma to a chunk the split left bare, Kokoro's pipeline sends the text as it is,
    //! and its voices render an added mark as an audible breath.
    return <- tts_chunks(norm, TTS_CHUNK_CHARS, m.kind == TtsKind.kitten)
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
    acc.total_us += t.total_us
    acc.samples += t.samples
}

let private NOISE_SEED = 0x5eed1ul   // the chunk index folds in: consecutive chunks draw different noise

def private synthesize_chunk(var m : TtsModel; norm : string; vname : string; speed : float; chunk_index : int) : TtsAudio {
    var inscope out = TtsAudio()
    out.sample_rate = m.model.sample_rate
    let clock = ref_time_ticks()
    let ps_misaki = g2p_phonemize(m.g2p, m.tagger, norm, voice_lang(m, vname) == "en-gb")
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
    let vname = m.kind == TtsKind.kitten ? kitten_voice(m.model.kitten, m.model, voice) : (styletts2_has_voice(m.model, voice) ? voice : "")
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
    let norm = normalize_text(text)
    var inscope chunks <- tts_chunks(m, norm)
    for (i, chunk in iter_range(chunks), chunks) {
        var inscope a <- synthesize_chunk(m, chunk, vname, speed, i)
        to_log(LOG_INFO, "dasLLAMA tts: {m.model.family} {m.model.name}: {timings_line(a.timings, a.sample_rate)}\n")
        invoke(blk, a)
    }
}

def synthesize(var m : TtsModel; text : string; voice : string; speed : float = 1.0) : TtsAudio {
    //! Text -> speech: normalize, chunk by sentence, phonemize, map into the family's symbols,
    //! synthesize; the chunks concatenate, the timings sum. Sample-identical to
    //! ``synthesize_stream`` over the same text.
    var inscope out = TtsAudio()
    out.sample_rate = m.model.sample_rate
    synthesize_stream(m, text, voice, speed) $(chunk) {
        out.pcm |> push_from(chunk.pcm)
        add_timings(out.timings, chunk.timings)
    }
    return <- out
}
