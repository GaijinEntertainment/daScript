options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_translate shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_nmt
require dasllama/dasllama_common   // Model, Session, create_session_, encode_ - the LLM translator's model
require dasllama/dasllama_transformer   // the arch registry: every [init] registers its family, gemma3 among them
require dasllama/dasllama_image   // load_model_cached
require dasllama/dasllama_sampling   // generate_
require dasllama/dasllama_tokenizer   // eos_id: the end token of either backend
require dasllama/dasllama_chat   // the chat template a chat model's sentence renders through
require strings
require daslib/strings_boost

//! Which model a translator runs: a Bergamot student (one direction, the Marian file) or a translation LLM (a GGUF; every
//! direction among its languages, the direction picked by `set_translator_langs`).
enum TranslatorKind {
    marian
    llm
}

//! A loaded translation model and the buffers its sentences reuse.
struct Translator {
    kind : TranslatorKind
    m : NmtModel = NmtModel()
    st : NmtState = NmtState()
    lm : Model = Model()
    ls : Session = Session()
    src : string   //!< the direction, as language codes (`ru`, `en`)
    trg : string
}

def finalize(var t : Translator) {
    delete t.m
    delete t.st
    delete t.ls
    delete t.lm
}

//! The languages a translation LLM of the MiLMMT-46 (GemmaX) line was trained on, as code and the name its prompt spells.
let private LLM_LANGS <- {
    "ar" => "Arabic", "az" => "Azerbaijani", "bg" => "Bulgarian", "bn" => "Bengali", "ca" => "Catalan", "cs" => "Czech",
    "da" => "Danish", "de" => "German", "el" => "Greek", "en" => "English", "es" => "Spanish", "fa" => "Persian",
    "fi" => "Finnish", "fr" => "French", "he" => "Hebrew", "hi" => "Hindi", "hr" => "Croatian", "hu" => "Hungarian",
    "id" => "Indonesian", "it" => "Italian", "ja" => "Japanese", "kk" => "Kazakh", "km" => "Khmer", "ko" => "Korean",
    "lo" => "Lao", "ms" => "Malay", "my" => "Burmese", "no" => "Norwegian", "nl" => "Dutch", "pl" => "Polish",
    "pt" => "Portuguese", "ro" => "Romanian", "ru" => "Russian", "sk" => "Slovak", "sl" => "Slovenian", "sv" => "Swedish",
    "ta" => "Tamil", "th" => "Thai", "tl" => "Tagalog", "tr" => "Turkish", "ur" => "Urdu", "uz" => "Uzbek",
    "vi" => "Vietnamese", "yue" => "Cantonese", "zh" => "Chinese (Simplified)", "zh-Hant" => "Chinese (Traditional)"
}

//! Whether `path` names a translation LLM: its GGUF, or a prepared image whose family tag is no Bergamot lane's.
def private is_llm_model_path(path : string) : bool {
    return true if (ends_with(path, ".gguf"))
    return false if (!ends_with(path, ".dlim"))
    let tag = image_family_tag(path)
    return tag != nmt_image_tag(true) && tag != nmt_image_tag(false)
}

//! Load a translation model: a Firefox Translations `model.<pair>.intgemm.alphas.bin` (or its `.dlim`) as a Bergamot student
//! of the file's one direction, a `.gguf` (or its `.dlim`) as an LLM - MiLMMT-46, or any instruct model through its chat template - whose
//! direction `set_translator_langs` picks; the weights serve on the q8 lane unless `exact` asks for f32.
def load_translator(path : string; exact : bool = false) : Translator {
    if (is_llm_model_path(path)) {
        var tr = Translator(kind = TranslatorKind.llm, trg = "en")
        tr.lm <- load_model_cached(path, exact ? QuantMode.fp32 : QuantMode.q8)
        tr.ls <- create_session_(tr.lm, KVDtype.f16, KVDtype.f16)
        return <- tr
    }
    var tr = Translator(kind = TranslatorKind.marian)
    tr.m <- load_nmt_model(path, !exact)
    let p = tr.m.pair
    tr.src = length(p) == 4 ? slice(p, 0, 2) : p
    tr.trg = length(p) == 4 ? slice(p, 2, 4) : ""
    return <- tr
}

//! The direction the next `translate` takes, as language codes; a translation LLM's source is "" until it is set.
def translator_langs(tr : Translator) : tuple<src : string; trg : string> => (src = tr.src, trg = tr.trg)

//! Every language code the model reads or writes: a Bergamot student's two, a translation LLM's whole list.
def translator_languages(tr : Translator) : array<string> {
    if (tr.kind == TranslatorKind.marian) {
        return <- [tr.src, tr.trg]
    }
    var codes <- [for (k in keys(LLM_LANGS)); k]
    sort(codes)
    return <- codes
}

//! A language code's English name, as the translation LLMs' prompts spell it (`ru` reads `Russian`); the code itself for one
//! the table does not know.
def translator_language_name(code : string) : string => LLM_LANGS?[code] ?? code

//! Pick the direction: a translation LLM takes any two of its languages; a Bergamot student only its own pair. False, the
//! direction unchanged, otherwise.
def set_translator_langs(var tr : Translator; src, trg : string) : bool {
    return false if (!translator_serves(tr, src, trg))
    tr.src = src
    tr.trg = trg
    return true
}

//! Whether the model covers `src` -> `trg`: a Bergamot student its one pair, a translation LLM any two different
//! languages of its list.
def translator_serves(tr : Translator; src, trg : string) : bool {
    if (tr.kind == TranslatorKind.marian) {
        return src == tr.src && trg == tr.trg
    }
    return src != trg && key_exists(LLM_LANGS, src) && key_exists(LLM_LANGS, trg)
}

//! A chat model's instruction: the direction, and the translation alone for an answer.
def private llm_system(src, trg : string) : string {
    let s = LLM_LANGS?[src] ?? src
    let t = LLM_LANGS?[trg] ?? trg
    return "You are a translator. Translate the user's message from {s} to {t}. Reply with the translation only - no notes, no quotes, no explanations."
}

//! The LLM's prompt for one sentence, as the MiLMMT-46 card spells it; its reply is the target line.
def private llm_prompt(src, trg, text : string) : string {
    let s = LLM_LANGS?[src] ?? src
    let t = LLM_LANGS?[trg] ?? trg
    return "Translate this from {s} to {t}:\n{s}: {text}\n{t}:"
}

//! One sentence through a translation LLM, greedy from position 0 of the translator's session. A model with no chat template
//! (MiLMMT-46) completes the card's prompt to its end token or the end of the line; a chat model (an instruct GGUF) answers
//! the instruction as its own template renders it, thinking off, to its turn's end.
def private llm_translate(var tr : Translator; text : string) : string {
    if (empty(tr.src)) {
        panic("dasLLAMA translate: pick the direction with set_translator_langs before translating with an LLM")
    }
    let params = SamplingParams()
    tr.ls.n_past = 0l
    if (empty(tr.lm.chat_template_str)) {
        let prompt <- encode_(tr.lm, llm_prompt(tr.src, tr.trg, strip(text)), true, false)
        let eos = eos_id(tr.lm.tok)
        let out = build_string() $(var w) {
            generate_(tr.lm, tr.ls, prompt, params, 4l * long_length(prompt) + 16l) $(id, piece) {
                return false if (id == eos || find(piece, "\n") >= 0)
                w |> write(piece)
                return true
            }
        }
        return strip(out)
    }
    var inscope chat <- create_chat_renderer_(tr.lm, llm_system(tr.src, tr.trg))
    set_thinking_(chat, false)
    add_user_(chat, strip(text))
    var inscope prompt <- render_turn_(tr.lm, chat)
    var inscope stops <- effective_stop_ids_(chat)
    let reply = build_string() $(var w) {
        generate_(tr.lm, tr.ls, prompt, params, 4l * long_length(prompt) + 64l) $(id, piece) {
            return false if (find_index(stops, id) >= 0)
            w |> write(piece)
            return true
        }
    }
    var inscope split <- split_reasoning_(chat, reply)
    return strip(split.content)
}

//! Translate one sentence greedily. A Bergamot student runs on a device driver where one serves the model, else on the CPU;
//! a translation LLM runs where the engine serves the model.
def translate(var tr : Translator; text : string) : string => tr.kind == TranslatorKind.llm ? llm_translate(tr, text) : nmt_translate(tr.m, tr.st, text)
