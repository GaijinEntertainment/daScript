options gen2

require dastest/testing_boost public
require dasllama/dasllama_tokenizer
require daslib/fio
require strings
require daslib/strings_boost
require dasllama/dasllama_env

// Tokenizer regression gates against llama.cpp's own test-tokenizer-0 corpora (the .inp/.out files
// shipped next to each ggml-vocab-*.gguf), through the production facade (load_tokenizer_auto ->
// encode/decode) so BOTH backends are covered: byte-level BPE (llama-bpe, qwen2, qwen35, gpt-2),
// SPM-style BPE (gemma-4), and SentencePiece (phi-3, llama-spm). The vocab fixtures are
// machine-local (gitignored model dir), so each case skips cleanly when absent — same contract as
// the model parity tests.

// override the machine-local fixture root with DASLLAMA_MODELS_DIR (e.g. on the x64 handoff box)
def private models_dir() : string {
    let env = g_env_harness.models_dir
    return empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
}

def private parse_out_ids(line : string) : array<int64> {
    return <- [for (tok in split(line, " ")); int64(to_int(tok)); where !empty(tok)]
}

def private drop_trailing_empty(var xs : array<string>) {
    if (!empty(xs) && empty(xs[length(xs) - 1])) {
        xs |> erase(length(xs) - 1)
    }
}

def private ids_equal(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (av, bv in a, b) {
        return false if (av != bv)
    }
    return true
}

// Run the .inp/.out corpus for one vocab: every input must encode to the reference ids, and
// decode(encode(x)) must round-trip losslessly. The first divergence of each kind is carried in
// the assertion message so a red is readable without rerunning.
def private corpus_gate(t : T?; vocab_path : string) {
    var tk <- load_tokenizer_auto(vocab_path)
    var inputs <- split(fread(vocab_path + ".inp"), "\n__ggml_vocab_test__\n")
    var outlines <- split(fread(vocab_path + ".out"), "\n")
    drop_trailing_empty(inputs)
    drop_trailing_empty(outlines)
    // fread returns "" for a missing file — without this check a present .gguf with absent
    // .inp/.out corpora would run 0 vectors and pass trivially
    t |> success(!empty(inputs), "corpus files present and non-empty: {vocab_path}.inp/.out")
    t |> equal(length(inputs), length(outlines))
    var npass = 0
    var rt_ok = 0
    var first_bad = ""
    var first_rt = ""
    for (inp, outl in inputs, outlines) {
        var got <- encode(tk, inp, false)
        if (ids_equal(got, parse_out_ids(outl))) {
            npass++
        } elif (empty(first_bad)) {
            first_bad = "first id mismatch on '{inp}': got {got}, want [{outl}]"
        }
        let dec = decode(tk, got)
        if (dec == inp) {
            rt_ok++
        } elif (empty(first_rt)) {
            first_rt = "first round-trip break on '{inp}': decoded '{dec}'"
        }
        delete got
    }
    t |> equal(npass, length(inputs), first_bad)
    t |> equal(rt_ok, length(inputs), first_rt)
    delete tk
    delete inputs
    delete outlines
}

def private corpus_case(t : T?; title, gguf : string) {
    t |> run(title) @(t : T?) {
        let path = path_join(models_dir(), gguf)
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        corpus_gate(t, path)
    }
}

[test]
def test_tokenizer_gemma4(t : T?) {
    corpus_case(t, "gemma4 SPM-style BPE vs the llama.cpp corpus", "ggml-vocab-gemma-4.gguf")
}

[test]
def test_tokenizer_llama_bpe(t : T?) {
    corpus_case(t, "llama3 byte-level BPE vs the llama.cpp corpus", "ggml-vocab-llama-bpe.gguf")
}

[test]
def test_tokenizer_qwen35(t : T?) {
    corpus_case(t, "qwen35 BPE (mark-absorbing letter runs) vs the llama.cpp corpus", "ggml-vocab-qwen35.gguf")
}

[test]
def test_tokenizer_qwen2(t : T?) {
    corpus_case(t, "qwen2 byte-level BPE vs the llama.cpp corpus", "ggml-vocab-qwen2.gguf")
}

[test]
def test_tokenizer_gpt2(t : T?) {
    corpus_case(t, "gpt-2 byte-level BPE vs the llama.cpp corpus", "ggml-vocab-gpt-2.gguf")
}

[test]
def test_tokenizer_phi3(t : T?) {
    corpus_case(t, "phi-3 SentencePiece vs the llama.cpp corpus", "ggml-vocab-phi-3.gguf")
}

[test]
def test_tokenizer_llama_spm(t : T?) {
    corpus_case(t, "llama-2 SentencePiece vs the llama.cpp corpus", "ggml-vocab-llama-spm.gguf")
}
