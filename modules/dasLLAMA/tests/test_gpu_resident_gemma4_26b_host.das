options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gpu_tier
require ?vulkan dasllama/dasllama_vulkan_common   // query_heap_info - the device room the pinned cap must fit
require _gemma_resident

// gemma-4-26B-A4B UD-IQ3_XXS under a plan room of a 16 GB card less its desktop, the rig's context asked whole: the plan
// moves the first routed layers' experts to the host and keeps the dense branch, the norms and the router on the device
// (`../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-gemma4-moe-block`). Large tier: arm `host26`. Its own file, since the cap binds the
// device's weight budget at the tier's first init. Skips as `test_gpu_resident_gemma4_26b.das` does, and where the pinned
// cap does not decide the plan: `DASLLAMA_GPU_VRAM_MB` set to another room, or a device whose room is under the cap.
let private GEMMA4_26B_HOST_FILE = GemmaFile(file = "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf", arm = "host26", host_experts = true)
let private HOST_CAP_MB = 8000l   //! about half the 8.9 GB of expert stacks fits beside the dense planes and the mirror
let private GEMMA4_26B_HITS_SLACK = 12l   //! the device-expert file's slack: a router near-tie flips whole positions

[init]
def private host_cap() {
    var want = g_gpu_want
    want.vram_mb = HOST_CAP_MB
    set_gpu_tier_want_(want)
}

//! true after the skip that says why, where the pinned cap does not decide the plan: the environment's room overrides the
//! pin, or the armed device holds less than the cap (a pinned cap reads no device room, so the plan would size past it)
def private cap_not_in_force(t : T?) : bool {
    if (gpu_want_vram_mb() != HOST_CAP_MB) {
        t |> skip("DASLLAMA_GPU_VRAM_MB={gpu_want_vram_mb()} overrides the {HOST_CAP_MB} MB plan room this file pins")
        return true
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (moe_gpu_tier_arm_() && g_gpu != null) {
            let heap = query_heap_info(g_gpu.phys)
            let room = heap.budget > 0l ? heap.budget : heap.size
            if (room < HOST_CAP_MB * 1_000_000l) {
                t |> skip("the device's room {room / 1_000_000l} MB is under the {HOST_CAP_MB} MB plan room this file pins")
                return true
            }
        }
    }
    return false
}

[test]
def test_gpu_resident_gemma4_26b_host_experts(t : T?) {
    t |> run("gemma-4-26B-A4B IQ3_XXS under an {HOST_CAP_MB} MB plan room: the first routed layers' experts on the host, 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        return if (!arm_on(t, GEMMA4_26B_HOST_FILE.arm) || cap_not_in_force(t))
        ppl_compare(t, 150l, 150l, GEMMA4_26B_HOST_FILE, GEMMA4_26B_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
    t |> run("gemma-4-26B-A4B IQ3_XXS under the same plan room: the load log says the host experts get no hot pool, and why") @(t : T?) {
        return if (!arm_on(t, GEMMA4_26B_HOST_FILE.arm) || cap_not_in_force(t))
        let log = resident_load_log(t, GEMMA4_26B_HOST_FILE, GEMMA_RIG_CTX, [(name = "DASLLAMA_GPU_VRAM_MB", value = "{HOST_CAP_MB}"),
            (name = "DAS_LOG_LEVEL", value = "info")])
        return if (empty(log))
        t |> success(find(log, "RESIDENT_LOAD active=true host_layers=0") < 0, "the child's plan moved routed layers to the host:\n{log}")
        t |> success(find(log, "dasLLAMA: resident driver - no hot expert pool") >= 0
            && find(log, "a gemma-4 MoE: the pool's device sums carry no per-expert down scale") >= 0, "the pool's decline names the gemma-4 MoE:\n{log}")
    }
}
