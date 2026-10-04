options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // load_model_ - the engine load (a tokenizer rides it; load_gguf has none)
require dasllama/dasllama_layout   // convert_model_to_metal_blob - the blob-flavor transform
require ?das_metal dasllama/dasllama_metal_common   // metal_kernel_coverage_of - the dispatch census
require ?das_metal dasllama/dasllama_metal_decode   // the fused attention's knob
require math

//! one Metal kernel's dispatch count since the census reset - 0 for a compiled kernel that never ran, a panic for a name no
//! compiled kernel carries (`metal_kernel_coverage_of`), and 0 on a build without das_metal
def metal_kernel_count(name : string) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_kernel_coverage_of(name)
    } else {
        return 0l
    }
}

//! `body` under the fused single-pass attention's knob pinned to `on`, the knob put back after: a census that reads the
//! fused form pins the knob, never inherits it. The body may move the knob itself (the knob-off control leg).
def with_metal_attn_d(on : bool; body : block<void>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev = get_metal_attn_d()
        set_metal_attn_d(on)
        invoke(body)
        set_metal_attn_d(prev)
    } else {
        invoke(body)
    }
}

//! the blob-flavor twin of a model file, its context capped: what every Metal parity and serving
//! cell loads as the device arm
def blob_twin(t : T?; path : string; seq_cap : int64) : Model {
    var tg <- load_model_(path, QuantMode.q8)
    t |> success(convert_model_to_metal_blob(tg), "metal-blob transform")
    tg.config.seq_len = min(tg.config.seq_len, seq_cap)
    return <- tg
}
