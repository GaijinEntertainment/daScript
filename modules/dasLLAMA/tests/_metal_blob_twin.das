options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // load_model_ - the engine load (a tokenizer rides it; load_gguf has none)
require dasllama/dasllama_layout   // convert_model_to_metal_blob - the blob-flavor transform
require math

//! the blob-flavor twin of a model file, its context capped: what every Metal parity and serving
//! cell loads as the device arm
def blob_twin(t : T?; path : string; seq_cap : int64) : Model {
    var tg <- load_model_(path, QuantMode.q8)
    t |> success(convert_model_to_metal_blob(tg), "metal-blob transform")
    tg.config.seq_len = min(tg.config.seq_len, seq_cap)
    return <- tg
}
