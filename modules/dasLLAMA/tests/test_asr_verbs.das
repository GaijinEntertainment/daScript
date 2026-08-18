options gen2
require dastest/testing_boost public
require strings
require dasllama/dasllama_whisper
require dasllama/dasllama_parakeet
require dasllama/dasllama_canary
require dasllama/dasllama_gemma4a
require dasllama/dasllama_qwen3a

// The family-owned halves of the ASR facade verbs (asr_exec_fmt / asr_encode_bucket), fed
// constructed structs — the moved-bit test the extraction owes. Plus parakeet's SPM detokenizer
// over a hand-built vocab. (The model-provenance shape invariants live in test_model_specs.das,
// beside the rest of the model-set table's gates.)

// A parakeet model carrying nothing but a toy SPM vocab — parakeet_detokenize reads m.vocab only.
// ▁ is U+2581, the SentencePiece space marker.
def private toy_parakeet() : ParakeetModel {
    var m = ParakeetModel()
    m.vocab <- ["▁hello", "▁▁▁world", "▁▁", "there", "▁tail", "<pad>"]   // nolint:STYLE039 (vocabulary data, not prose)
    return <- m
}

def private detok(m : ParakeetModel; ids : array<int>) : string {
    var toks <- [for (i in ids); ParakeetToken(id = i)]
    let s = parakeet_detokenize(m, toks)
    delete toks
    return s
}

[test]
def test_family_exec_fmt_verbs(t : T?) {
    t |> run("each family verb stamps its own precision") @(t : T?) {
        var w = WhisperModel()
        w.enc.q8 = true
        w.dq8 = false
        t |> equal(whisper_exec_fmt(w), "q8/f32", "whisper mixed rail")
        w.dq8 = true
        t |> equal(whisper_exec_fmt(w), "q8/q8", "whisper q8 rail")
        var p = ParakeetModel()
        p.q8 = true
        t |> equal(parakeet_exec_fmt(p), "q8", "parakeet single carrier")
        var c = CanaryEncoder()
        c.q8 = true
        t |> equal(canary_exec_fmt(c, "q8"), "q8/q8", "canary q8 rail")
        c.q8 = false
        t |> equal(canary_exec_fmt(c, "q8"), "f32/q8", "canary fp32 parity encoder")
        t |> equal(gemma4a_exec_fmt("q8"), "q8/q8", "gemma4a q8 tower (forced)")
        t |> equal(qwen3a_exec_fmt("q8"), "q8/q8", "qwen3a q8 core (forced)")
    }
}

[test]
def test_family_encode_buckets(t : T?) {
    t |> run("encode-total buckets are family facts") @(t : T?) {
        t |> equal(whisper_encode_bucket(), "wh.encode_total")
        t |> equal(canary_encode_bucket(), "cn.encode")
        t |> equal(gemma4a_encode_bucket(), "g4a.encode")
        t |> equal(qwen3a_encode_bucket(false), "q3a.encode")
        t |> equal(qwen3a_encode_bucket(true), "q3o.encode")
    }
}

[test]
def test_parakeet_detokenize(t : T?) {
    t |> run("the FIRST piece drops the spaces its ▁ prefix produced") @(t : T?) {   // nolint:STYLE039
        var inscope m <- toy_parakeet()
        t |> equal(detok(m, [0]), "hello", "one leading mark")
        t |> equal(detok(m, [1]), "world", "three leading marks")
        t |> equal(detok(m, [2]), "", "an all-marks first piece detokenizes to nothing")
    }
    t |> run("a LATER piece keeps the space its ▁ prefix produced") @(t : T?) {   // nolint:STYLE039
        var inscope m <- toy_parakeet()
        t |> equal(detok(m, [0, 4]), "hello tail", "second piece keeps its single space")
        t |> equal(detok(m, [0, 1]), "hello   world", "and keeps all three of them")
        // the all-marks piece emptied out at position 0; at position 1 it is two real spaces
        t |> equal(detok(m, [2, 4]), " tail", "an emptied first piece does not re-arm the strip")
        t |> equal(detok(m, [3, 2]), "there  ", "a later all-marks piece stays two spaces")
    }
    t |> run("pieces with no ▁ prefix pass through; control pieces render as nothing") @(t : T?) {   // nolint:STYLE039
        var inscope m <- toy_parakeet()
        t |> equal(detok(m, [3]), "there", "a bare first piece is untouched")
        t |> equal(detok(m, [0, 5]), "hello", "a <...> control piece drops out")
    }
    t |> run("an empty token list detokenizes to the empty transcript") @(t : T?) {
        var inscope m <- toy_parakeet()
        var none : array<int>
        t |> equal(detok(m, none), "", "no tokens, no transcript")
        delete none
    }
}
