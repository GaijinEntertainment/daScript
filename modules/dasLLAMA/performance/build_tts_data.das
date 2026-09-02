options gen2
options _dasllama_internal = true
options stack = 524288

require strings
require daslib/fio
require daslib/clargs
require dasllama/dasllama_env

// Regenerate the TTS model-store data from the experiment tree: the G2P pack (which also emits
// the tag-keyed lexicon words), the silver tagging prose sampled around those words, the tagger
// pack, then the KittenTTS and Kokoro GGUFs and their oracle dumps - each step a harness
// script, each output beside its license file. This is the provenance recipe for tts_postag.bin,
// tts_g2p.bin, kitten-<size>.gguf, kokoro-82m.gguf and tts_oracle/.
//   daslang -jit performance/build_tts_data.das -- --root ~/Work/tts-ab/g2p [--models <dir>] [--silver-sents 100000] [--no-kitten]
// `root` holds the fidelity experiment (its .venv-g2p with misaki, g2p_en, spacy + en_core_web_sm,
// onnx + onnxruntime + kittentts; .nltk with CMUdict; work/ud-ewt with the treebank; work/gutenberg
// with the prose); the pinned Kitten snapshots live in its hub cache (harness/kitten_graph.py).

[CommandLineArgs]
struct BuildArgs {
    @clarg_doc = "the G2P fidelity experiment root (venv, NLTK data, treebank and prose under work/)"
    @clarg_required
    root : string = ""
    @clarg_doc = "the model store to write into (default: the models dir)"
    models : string = ""
    @clarg_doc = "sentences of silver tagging prose to sample (0 = keep an existing work/silver_focus); every sentence carrying a tag-keyed word is kept on top"
    silver_sents : int = 100000
    @clarg_doc = "skip the KittenTTS and Kokoro GGUF conversions and oracle dumps"
    no_kitten : bool = false
}

def private step(name : string; args : array<string>) : bool {
    to_log(LOG_INFO, "tts data: {name}\n")
    var out = ""
    let rc = run_and_capture(args, out, 3600.0)
    to_log(LOG_INFO, out)
    if (rc != 0) {
        to_log(LOG_ERROR, "tts data: {name} failed with {rc}\n")
        return false
    }
    return true
}

[export]
def main() : int {
    var args = BuildArgs()
    let rc = parse_args_with_help(args, "modules/dasLLAMA/performance/build_tts_data.das")
    return rc if (rc >= 0)
    let store = empty(args.models) ? (empty(g_env_harness.models_dir) ? "/Users/borisbatkin/Work/llama.cpp/models/" : g_env_harness.models_dir) : args.models
    let py = path_join(args.root, ".venv-g2p/bin/python")
    let harness = path_join(get_das_root(), "modules/dasLLAMA/harness")
    let fixture = path_join(get_das_root(), "modules/dasLLAMA/tests/_tts_fixtures/g2p_corpus.json")
    let silver = path_join(args.root, "work/silver_focus")
    let focus = path_join(args.root, "work/focus_words.txt")
    return 1 if (!step("pack the G2P data", [py, path_join(harness, "build_g2p_data.py"), args.root, "--out", path_join(store, "tts_g2p.bin"),
        "--focus-words", focus]))
    if (args.silver_sents > 0) {
        var inscope prose : array<string>
        dir(path_join(args.root, "work/gutenberg")) $(name) {
            if (ends_with(name, ".txt")) {
                prose |> push(path_join(args.root, "work/gutenberg/{name}"))
            }
        }
        var inscope mint <- [py, path_join(harness, "mint_postag_silver.py"), args.root, silver]
        mint |> push_from(prose)
        mint |> push("--max-sents")
        mint |> push("{args.silver_sents}")
        mint |> push("--focus-words")
        mint |> push(focus)
        return 1 if (!step("mint silver tagging prose", mint))
    }
    return 1 if (!step("train the tagger", [py, path_join(harness, "train_postag.py"), path_join(args.root, "work/ud-ewt"), silver,
        "--out", path_join(store, "tts_postag.bin"), "--fixture", fixture]))
    if (!args.no_kitten) {
        for (size in ["nano", "mini"]) {
            return 1 if (!step("convert kitten {size}", [py, path_join(harness, "convert_kitten.py"), "--size", size, "--out", store]))
        }
        return 1 if (!step("mint the kitten oracle dumps", [py, path_join(harness, "kitten_oracle.py"), "--out", path_join(store, "tts_oracle")]) ||
            !step("convert kokoro", [py, path_join(harness, "convert_kokoro.py"), "--out", store]) ||
            !step("mint the kokoro oracle dumps", [py, path_join(harness, "kokoro_oracle.py"), "--out", path_join(store, "tts_oracle")]))
    }
    to_log(LOG_INFO, "tts data: done - {path_join(store, "tts_postag.bin")}, {path_join(store, "tts_g2p.bin")}, kitten-nano/mini.gguf, kokoro-82m.gguf, tts_oracle/\n")
    return 0
}
