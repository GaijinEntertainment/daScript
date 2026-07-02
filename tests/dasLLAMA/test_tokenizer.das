options gen2

require dastest/testing_boost public
require dasllama/dasllama_bpe
require daslib/fio
require strings
require daslib/strings_boost

// Tokenizer regression gates against llama.cpp's own test-tokenizer-0 corpora (the .inp/.out files
// shipped next to each ggml-vocab-*.gguf). The vocab fixtures are machine-local (gitignored model
// dir), so each case skips cleanly when absent — same contract as the model parity tests.
// gemma4 is the SPM-style BPE backend (metaspace escape + newline-only pre-split + raw-UTF-8 ranked
// merges + <0xXX> byte fallback) added in wave 3; llama-bpe guards the byte-alphabet path.

// override the machine-local fixture root with DASLLAMA_MODELS_DIR (e.g. on the x64 handoff box)
def private models_dir() : string {
    let env = get_env_variable("DASLLAMA_MODELS_DIR")
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
    for (i in range(length(a))) {
        return false if (a[i] != b[i])
    }
    return true
}

// Run the .inp/.out corpus for one vocab: every input must encode to the reference ids, and
// decode(encode(x)) must round-trip losslessly.
def private corpus_gate(t : T?; vocab_path : string) {
    let tk = load_bpe_tokenizer_gguf(vocab_path)
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
    for (i in range(length(inputs))) {
        let got <- bpe_encode(tk, inputs[i])
        if (ids_equal(got, parse_out_ids(outlines[i]))) {
            npass++
        }
        if (bpe_decode_ids(tk, got) == inputs[i]) {
            rt_ok++
        }
    }
    t |> equal(npass, length(inputs))
    t |> equal(rt_ok, length(inputs))
}

[test]
def test_tokenizer_gemma4(t : T?) {
    t |> run("gemma4 SPM-style BPE vs the llama.cpp corpus") @(t : T?) {
        let path = path_join(models_dir(), "ggml-vocab-gemma-4.gguf")
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        corpus_gate(t, path)
    }
}

[test]
def test_tokenizer_llama_bpe(t : T?) {
    t |> run("llama3 byte-level BPE vs the llama.cpp corpus") @(t : T?) {
        let path = path_join(models_dir(), "ggml-vocab-llama-bpe.gguf")
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        corpus_gate(t, path)
    }
}
