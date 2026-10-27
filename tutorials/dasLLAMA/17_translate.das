options gen2
options stack = 524288   // every dasLLAMA program root takes this budget

require dasllama/dasllama
require daslib/jobque_boost
require strings
require daslib/strings_boost   // join

// Tutorial dasLLAMA-17: Translation
//
// This tutorial covers:
//   - load_translator — one call loads a Firefox Translations model
//     (the Marian `.bin` and its `.spm` vocabulary)
//   - translator_langs — the language the model reads and the one
//     it writes
//   - translate — one sentence in, one sentence out
//   - load_translator(path, exact = true) — the f32 lane beside the
//     default Q8_0 lane
//   - set_translator_langs / translator_serves /
//     translator_languages — a translation LLM (MiLMMT-46, a GGUF):
//     any direction among 46 languages, picked per call
//
// The models are the small students Firefox ships for offline
// translation: a transformer encoder reads the sentence, and a
// decoder of recurrent blocks writes the translation one token at a
// time, always the most likely next token. One direction a model:
// ru-en reads Russian and writes English, en-ru the other way.
//
// A translation LLM is the other kind: MiLMMT-46 is Gemma 3 1B
// trained further on 46 languages. It is thirty times the size and
// slower, and it translates noticeably better; one file covers
// every direction.
//
// Prerequisites: a Bergamot model directory -
// `model.<pair>.intgemm.alphas.bin` with `vocab.<pair>.spm` beside
// it, as Firefox Translations publishes them (gunzipped) - or a
// MiLMMT-46 GGUF.
//
// Run: daslang.exe -jit tutorials/dasLLAMA/17_translate.das --
//      <model.bin | model.gguf> [--from ru] [--to en] [--exact]
//      ["a sentence"]

let RU_LINES = [
    "Привет! Как дела?",
    "Сегодня мы идём в горы, если не будет дождя.",
    "Где ближайшая аптека?"]
let EN_LINES = [
    "Hello! How are you?",
    "Today we are going to the mountains if it does not rain.",
    "Where is the nearest pharmacy?"]

struct Args {
    model_path : string
    exact : bool
    text : string
    from : string = "ru"
    to : string = "en"
}

def parse_args : Args {
    let cmd <- get_command_line_arguments()
    var a = Args()
    var i = 0
    while (i < length(cmd)) {
        let arg = cmd[i]
        if (arg == "--exact") {
            a.exact = true
        } elif (arg == "--from" && i + 1 < length(cmd)) {
            a.from = cmd[i + 1]
            i++
        } elif (arg == "--to" && i + 1 < length(cmd)) {
            a.to = cmd[i + 1]
            i++
        } elif (ends_with(arg, ".bin") || ends_with(arg, ".dlim") || ends_with(arg, ".gguf")) {
            a.model_path = arg
        } elif (!empty(a.model_path) && !starts_with(arg, "-")) {
            a.text = arg
        }
        i++
    }
    return a
}

[export]
def main {
    let args = parse_args()
    let model_path = args.model_path
    let exact = args.exact
    let text = args.text
    let from = args.from
    let to = args.to
    if (empty(model_path)) {
        print("usage: 17_translate.das -- <model.bin> [--exact] [\"a sentence\"]\n")
        return
    }
    with_dasllama_jobque() {
        // ------------------------------------------------------------------
        // Section 1 - Load a model
        // ------------------------------------------------------------------
        //
        // The first load reads the Marian file, converts its weights and
        // bakes a `.dlim` beside it; every later load maps that file and
        // starts at once. The int8 weights serve as Q8_0 blocks;
        // `exact = true` widens them to f32 instead - the reference lane,
        // a little slower and closer to the float math the model was
        // trained in.
        var inscope tr <- exact ? load_translator(model_path, true) : load_translator(model_path)

        // ------------------------------------------------------------------
        // Section 2 - Pick the direction
        // ------------------------------------------------------------------
        //
        // A Bergamot model has one direction, its file's pair. A
        // translation LLM knows a list of languages and takes any two;
        // set_translator_langs picks them, and answers false for a pair
        // the model does not cover - a Bergamot model accepts only its
        // own pair.
        if (tr.kind == TranslatorKind.llm) {
            let codes <- translator_languages(tr)
            print("the model knows {length(codes)} languages: {join(codes, " ")}\n")
            if (!translator_serves(tr, from, to)) {
                print("it does not translate '{from}' to '{to}'\n")
                return
            }
            set_translator_langs(tr, from, to)
        }
        let langs = translator_langs(tr)
        let reads = translator_language_name(langs.src)
        let writes = translator_language_name(langs.trg)
        print("model reads {reads} ('{langs.src}'), writes {writes} ('{langs.trg}')")
        print(" ({exact ? "f32" : "q8"} lane)\n")

        // ------------------------------------------------------------------
        // Section 3 - Translate
        // ------------------------------------------------------------------
        //
        // One call a sentence. Split a paragraph into sentences first: the
        // model was trained on sentences, and each call reuses the buffers
        // the translator holds, so a long text is a loop of short calls.
        var lines : array<string>
        if (!empty(text)) {
            lines |> push(text)
        } else {
            lines := langs.src == "ru" ? RU_LINES : EN_LINES
        }
        for (ln in lines) {
            let t0 = ref_time_ticks()
            let out = translate(tr, ln)
            print("{ln}\n  -> {out}   ({double(get_time_usec(t0)) / 1000.0lf} ms)\n")
        }
    }
}
