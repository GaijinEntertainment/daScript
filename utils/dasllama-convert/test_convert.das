options gen2
options persistent_heap
options _dasllama_internal = true

// dasllama-convert's carrier sniff: the kind a source file routes
// to, off its magic and its GGUF metadata. The model cells skip
// through the suites' gate (`model_available`): a file absent from
// DASLLAMA_MODELS_DIR, a split with a missing shard, or a
// large-tier file without DASLLAMA_PARITY_FULL=1. The absent-file
// cell needs nothing.

require dastest/testing_boost public
require convert_sniff
// models_dir / model_available: the suites' presence and tier gate
require ../../modules/dasLLAMA/tests/_model_tier.das
require daslib/fio

def private sniff_cell(t : T?; name : string; want : AsrCarrierKind) {
    let path = path_join(models_dir(), name)
    return if (!model_available(t, path))
    t |> equal(sniff_asr_kind(path), want, name)
}

[test]
def test_sniff_asr_kind(t : T?) {
    t |> run("the four GigaAM v3 carriers sniff as gigaam") @(t : T?) {
        for (v in ["ctc", "rnnt", "e2e-ctc", "e2e-rnnt"]) {
            sniff_cell(t, "gigaam-v3-{v}-f16.gguf", AsrCarrierKind.gigaam)
        }
    }
    t |> run("a decoder GGUF with no projector sniffs as a decoder") @(t : T?) {
        sniff_cell(t, "SmolLM2-135M-Instruct-Q8_0.gguf", AsrCarrierKind.decoder)
    }
    t |> run("a Qwen3-ASR audio mmproj sniffs as qwen3a") @(t : T?) {
        sniff_cell(t, "mmproj-Qwen3-ASR-0.6B-bf16.gguf", AsrCarrierKind.qwen3a)
    }
    t |> run("no file is a decoder - the Model flow reports the missing file") @(t : T?) {
        let absent = path_join(test_temp_dir(), "absent.gguf")
        t |> equal(sniff_asr_kind(absent), AsrCarrierKind.decoder)
    }
}
