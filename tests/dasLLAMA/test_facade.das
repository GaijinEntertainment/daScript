options gen2

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
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

// Regression (Copilot #3340): SPM encode/decode must use the per-model bos_id/eos_id loaded from
// GGUF, not the llama2.c literals 1/2 — Gemma has bos=2 / eos=1. Empty text isolates the BOS/EOS
// framing (no vocab lookup), so this needs no model file.
[test]
def test_spm_encode_respects_model_bos_eos(t : T?) {
    let tk = SpmTokenizer(bos_id = 2l, eos_id = 1l, add_space_prefix = false)
    let ids <- encode(tk, "", true, true)
    t |> equal(length(ids), 2, "empty text with bos+eos -> [bos, eos]")
    t |> equal(ids[0], 2l, "encode uses per-model bos_id (Gemma=2), not literal 1")
    t |> equal(ids[1], 1l, "encode uses per-model eos_id (Gemma=1), not literal 2")
    let def2 = SpmTokenizer()   // defaults (Llama-2) stay bit-identical
    let ids2 <- encode(def2, "", true, true)
    t |> equal(ids2[0], 1l, "default bos_id=1 unchanged")
    t |> equal(ids2[1], 2l, "default eos_id=2 unchanged")
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
