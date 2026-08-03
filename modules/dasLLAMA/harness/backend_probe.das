options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama_transformer
require dasllama/dasllama_math
require dasllama/dasllama_math_gen
require daslib/fio   // get_env_variable

def private probe_model() : string {
    var dir = get_env_variable("DASLLAMA_MODELS_DIR")
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
