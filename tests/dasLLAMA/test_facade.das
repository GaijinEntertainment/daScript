options gen2

require dastest/testing_boost public
require dasllama/dasllama_transformer
require dasllama/dasllama_tokenizer
require dasllama/dasllama_bpe
require daslib/fio

// The unified tokenizer facade (encode/decode/piece over auto-selected SPM/BPE) must produce
// exactly what a caller writes today against the raw backend — so migrating onto it is a no-op.
// GGUF models are gitignored multi-GB files; each case skips cleanly when its model is absent.

let MODELS_DIR = "/Users/borisbatkin/Work/llama.cpp/models/"

def private same(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i] != b[i]) {
            return false
        }
    }
    return true
}

[test]
def test_facade_spm(t : T?) {
    t |> run("SPM facade auto-dispatch matches raw SentencePiece") @(t : T?) {
        let path = "{MODELS_DIR}tinyllama-1.1b-chat-v1.0.Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        let v <- load_tokenizer_auto(path)
        let raw <- load_tokenizer_gguf(path)
        let text = "Once upon a time"
        let ok = (v.kind == TokKind.spm &&
            same(encode(v, text, true, false), encode(raw, text, true, false)) &&
            same(encode(v, text, false, false), encode(raw, text, false, false)))
        t |> success(ok, "spm dispatch + encode parity (with/without BOS)")
    }
}

[test]
def test_facade_bpe(t : T?) {
    t |> run("BPE facade auto-dispatch matches raw byte-level BPE") @(t : T?) {
        let path = "{MODELS_DIR}Qwen2.5-0.5B-Instruct-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        let v <- load_tokenizer_auto(path)
        let raw <- load_bpe_tokenizer_gguf(path)
        let text = "Once upon a time"
        // add_special=false is exactly bpe_encode; add_special=true prepends bos_id (when present)
        var want : array<int64>
        if (raw.bos_id >= 0l) {
            want |> push(raw.bos_id)
        }
        want |> push_from(bpe_encode(raw, text))
        let ids <- encode(v, text, false, false)
        let ok = (v.kind == TokKind.bpe &&
            same(encode(v, text, false, false), bpe_encode(raw, text)) &&
            same(encode(v, text, true, false), want) &&
            decode(v, ids) == text)   // byte-level BPE round-trips ASCII exactly
        t |> success(ok, "bpe dispatch + encode/decode parity")
    }
}

[test]
def test_facade_model(t : T?) {
    t |> run("load_model owns the tokenizer; model facade delegates") @(t : T?) {
        let path = "{MODELS_DIR}Qwen2.5-0.5B-Instruct-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        let m <- load_model(path, QuantMode.q8)
        let text = "Hello, world"
        let via_model <- encode(m, text, true, false)
        let via_tok <- encode(m.tok, text, true, false)
        let last = via_model[length(via_model) - 1]
        let ok = (m.tok.kind == TokKind.bpe &&
            same(via_model, via_tok) &&
            piece(m, last) == piece(m.tok, last))
        t |> success(ok, "model.tok populated + encode/piece delegate")
    }
}
