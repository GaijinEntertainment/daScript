options gen2
options _dasllama_internal = true
options stack = 524288

// The driver's own code for the Vulkan home's tiles: the class pipelines created under the capture flags, each one's
// VK_KHR_pipeline_executable_properties report (NVIDIA: the SASS) written beside the others - what the coopmat2 body
// compiles to, to read against the CUDA emitter's lowering of the same body.
//   DASLLAMA_GPU=1 daslang -jit modules/dasLLAMA/harness/vk_sass_dump.das -- <out dir> [<name substring>]

require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require daslib/fio
require strings

[export]
def main {
    let args <- get_command_line_arguments()
    var out = ""
    var filter = ""
    var seen = 0
    for (i in range(length(args))) {
        if (args[i] == "--") {
            seen = i
        } elif (seen > 0 && i == seen + 1) {
            out = args[i]
        } elif (seen > 0 && i == seen + 2) {
            filter = args[i]
        }
    }
    if (empty(out)) {
        print("usage: -- <out dir> [<name substring>]\n")
        return
    }
    set_pipeline_capture(true)
    let armed = (ensure_q8_batch_cm2l_cls() && ensure_q8_batch_cm2m_cls() && ensure_q8_batch_cm2s_cls()
        && fa_stamp_ensure(false, 64l, false, false, false, true) && fa_stamp_ensure(false, 64l, false, false, false, false))
    if (!armed || g_gpu == null || g_gpu.coopmat_mode != COOPMAT_CM2) {
        print("no cm2 device armed\n")
        return
    }
    if (!pipeline_exec_props_supported(g_gpu.phys)) {
        print("the device has no pipelineExecutableInfo\n")
        return
    }
    mkdir_rec(out)
    to_log(LOG_WARNING, "vk_sass_dump: {length(g_pipe_names)} pipelines known\n")
    var n = 0
    for (pk, name in keys(g_pipe_names), values(g_pipe_names)) {
        continue if (!empty(filter) && find(name, filter) < 0)
        to_log(LOG_WARNING, "vk_sass_dump: {name} at {pk}\n")
        var pipe = vk_value_to_boost(unsafe(reinterpret<VkPipeline>(pk)))
        pipe._needs_delete = false
        let report = dump_pipeline_executables(g_gpu.device, pipe)
        to_log(LOG_WARNING, "vk_sass_dump: {name} reported {length(report)} bytes\n")
        let path = "{out}/{name}.txt"
        if (fwrite(path, report)) {
            print("{name}: {length(report)} bytes -> {path}\n")
            n++
        } else {
            print("{name}: could not write {path}\n")
        }
    }
    print("{n} pipelines dumped\n")
}
