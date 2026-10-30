options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_transformer   // the umbrella's [init] registrations: the CUDA home's seats under DASLLAMA_CUDA=1
require dasllama/dasllama_config   // DlimVulkanConfig
require dasllama/dasllama_gpu_tier   // gpu_want_cuda, moe_gpu_dry_bake_arm
require dasllama/dasllama_gpu_resident   // rdec_fallback_words
require ?vulkan dasllama/dasllama_math_vulkan   // vulkan_dry_bake_arm

// The fixture test_gpu_tier.das spawns under each home's ask: the texts that follow the asked home - what serves a model the
// whole-model driver declined, and each discrete home's dry-bake arm refusing a config the other home wrote - print through
// to_log, which no in-process cell reads, and DASLLAMA_CUDA is read at context init. Every case opens with a `CASE <name>`
// record on the same log stream.

def private mark(name : string) {
    to_log(LOG_INFO, "CASE {name}\n")
}

[export]
def main() {
    mark("fallback")
    to_log(LOG_INFO, "FALLBACK {rdec_fallback_words()}\n")
    static_if (typeinfo builtin_module_exists(vulkan)) {
        mark("vulkan_foreign")
        to_log(LOG_INFO, "ARM {vulkan_dry_bake_arm(DlimVulkanConfig(home = "cuda"))}\n")
    }
    if (gpu_want_cuda()) {
        mark("cuda_foreign")
        to_log(LOG_INFO, "ARM {moe_gpu_dry_bake_arm(DlimVulkanConfig(home = "vulkan"))}\n")
    }
    mark("end")
    print("ROOT_DONE\n")
}
