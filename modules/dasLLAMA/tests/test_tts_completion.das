options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true
require dasllama/dasllama
require _model_tier
require dasllama/dasllama_math
require dastest/testing_boost
require daslib/fio

[test]
def pocket_completion_reports_budget_exhaustion_without_discarding_pcm(t : T?){
 let path = path_join(models_dir(), "pocket-tts-en-stuart-kq.gguf")
 if (!stat(path).is_valid){t |> skip("Pocket model not installed"); return }
 with_dasllama_jobque_() {
 var inscope model <- load_tts_model(path)
 // Exercise both EOS paths deterministically, independent of pronunciation.
 let threshold = -1.0e30
 model.pocket.eos_threshold = threshold
 var inscope good <- synthesize(model, "Hello there.", "stuart_bell")
 t |> success(good.generation_complete); t |> success(!empty(good.pcm))
 let original = hash(good.pcm)
 let text = build_string() $(var w){for (_ in range(8)){w |> write("This is a simple sentence for a completion test. ")}}
 var chunks = 0
 synthesize_stream(model, text, "stuart_bell", 1.0) $(chunk){
  chunks++; t |> equal(chunk.generation_complete, chunks == 1); t |> success(!empty(chunk.pcm), "partial audio remains inspectable")
  model.pocket.eos_threshold = 1.0e30
 }
 t |> success(chunks >= 2, "exercise a successful prefix and failed later chunk")
 var inscope failed <- synthesize(model, text, "stuart_bell")
 t |> success(!failed.generation_complete); t |> success(!empty(failed.pcm))
 model.pocket.eos_threshold = threshold
 var inscope again <- synthesize(model, "Hello there.", "stuart_bell")
 t |> success(again.generation_complete, "a prior failure must not taint the next result")
 t |> equal(hash(again.pcm), original, "completion metadata does not change the waveform")
 }
}
