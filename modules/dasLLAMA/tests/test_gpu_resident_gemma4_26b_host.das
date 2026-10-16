options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gpu_tier
require _gemma_resident

// gemma-4-26B-A4B UD-IQ3_XXS under a plan room of a 16 GB card less its desktop, the rig's context asked whole: the plan
// moves the first routed layers' experts to the host and keeps the dense branch, the norms and the router on the device
// (`../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-gemma4-moe-block`). Large tier: arm `host26`. Its own file, since the cap binds the
// device's weight budget at the tier's first init. Skips as `test_gpu_resident_gemma4_26b.das` does.
let private GEMMA4_26B_HOST_FILE = GemmaFile(file = "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf", arm = "host26", host_experts = true)
let private HOST_CAP_MB = 8000l   //! about half the 8.9 GB of expert stacks fits beside the dense planes and the mirror
let private GEMMA4_26B_HITS_SLACK = 12l   //! the device-expert file's slack: a router near-tie flips whole positions

[init]
def private host_cap() {
    var want = g_gpu_want
    want.vram_mb = HOST_CAP_MB
    set_gpu_tier_want_(want)
}

[test]
def test_gpu_resident_gemma4_26b_host_experts(t : T?) {
    t |> run("gemma-4-26B-A4B IQ3_XXS under an {HOST_CAP_MB} MB plan room: the first routed layers' experts on the host, 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA4_26B_HOST_FILE, GEMMA4_26B_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
}
