options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama
require dasllama/dasllama_image
require daslib/fio
require strings

// The child test_model_image's mx4-stream arm runs under the OS's peak-footprint accounting: one cold planar mint of the
// gguf it is handed, the image removed first, and the expert planes' bytes of the model it maps back.

[export]
def main {
    let cmd <- get_command_line_arguments()
    let path = empty(cmd) ? "" : cmd[length(cmd) - 1]
    if (!ends_with(path, ".gguf")) {
        print("MX4_MINT_SKIP no model\n")
        return
    }
    allow_cpu_prefill()   //! no prefill runs; the root mints and maps
    remove(image_path_for(path, ""))
    with_dasllama_jobque() {
        var m <- load_model_cached(path, QuantMode.q8, [planar_only = true])
        print("MX4_MINT mapped={m.image_map != null} mx4={m.experts_mx4} planes={long_length(m.mxq) + long_length(m.mxs)}\n")
        delete m
    }
}
