options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_transformer
require dasllama/dasllama_math
require dasllama/dasllama_math_gen
require dasllama/dasllama_env

def private probe_model() : string {
    var dir = g_env_harness.models_dir
    if (empty(dir)) {
        dir = "/Users/borisbatkin/Work/llama.cpp/models/"
    }
    return "{dir}gemma-4-E2B-it-Q8_0.gguf"
}

[export]
def main() {
    print("pre-load: active={active_kernel_backend()} family_live={q8q8_family_live()}\n")
    var t <- load_gguf(probe_model(), QuantMode.q8)
    print("post-load: active={active_kernel_backend()} batch={active_batch_backend()}\n")
    delete t
}
