options gen2

require dasllama/dasllama_transformer

// Memory detector for the memory-honest refactor. Replays the chat path (load a GGUF, then
// quantize to Q8) and reports both the logical footprint (array bytes we hold) and the real
// context-heap high-water mark (heap_bytes_allocated) at each step. Run BEFORE the refactor to
// capture the baseline, then again after to prove the win.
//
//   bin/daslang -jit modules/dasLLAMA/dasllama_mem.das -- <model.gguf>

let DEFAULT_MODEL = "/Users/borisbatkin/Work/llama.cpp/models/tinyllama-1.1b-chat-v1.0.Q8_0.gguf"

def mb(b : int64) : double {
    return double(b) / 1048576.0lf
}

def heap_mb() : double {
    return double(int64(heap_bytes_allocated())) / 1048576.0lf
}

def report(tag : string; t : Model) {
    let f = footprint(t)
    print("  {tag}: heap {heap_mb()} MB | footprint {mb(f.total)} MB"
        + " (aux-fp32 {mb(f.aux_fp32_bytes)} | weight-fp32 {mb(f.weight_fp32_bytes)}"
        + " | q8 {mb(f.q8_bytes + f.q8_scale_bytes)} | q4 {mb(f.q4_bytes + f.q4_scale_bytes)})\n")
}

[export]
def main {
    var path = DEFAULT_MODEL
    let args = get_command_line_arguments()
    if (!empty(args)) {
        path = args[length(args) - 1]
    }
    print("model: {path}\n")
    print("heap at start: {heap_mb()} MB\n")

    // The old chat behavior: dequant everything to fp32, then re-quantize to Q8 in place (holds both).
    print("--- fp32 load + in-place quantize (A/B harness path) ---\n")
    var t <- load_gguf(path, QuantMode.fp32)
    let c = t.config
    print("config: dim={c.dim} layers={c.n_layers} vocab={c.vocab_size} shared={c.shared_weights}\n")
    report("after fp32 load", t)
    quantize_weights(t)
    report("after quantize_weights (fp32 + Q8)", t)
    delete t

    // The memory-honest path: load straight into Q8 storage, never materialize the fp32 weights.
    print("--- memory-honest: load_gguf(.., QuantMode.q8) ---\n")
    var tq <- load_gguf(path, QuantMode.q8)
    report("loaded as Q8", tq)
    delete tq
}
