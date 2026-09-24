options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_tower shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_env
require dasllama/dasllama_gemma4v   // Gemma4vTower / Gemma4vState - the ViT block loop
require dasllama/dasllama_gemma3v   // Gemma3vTower / Gemma3vState - the SigLIP block loop
require dasllama/dasllama_qwen3v    // Qwen3vTower / Qwen3vState - the qwen3vl ViT block loop
require dasllama/dasllama_qwen25v   // Qwen25vTower / Qwen25vState - the window-attention ViT chain

//! The vision tower Vulkan driver: one-shot encoder chains - no session, no KV, no mirror - that
//! serve a family's block loop dispatch for dispatch as the family's CPU loop spells it, over the
//! q8 image the CPU chain reads (qwen25v over its baked f16 twin). Every hook is best-effort: it
//! answers false on any shape, knob or device decline and the CPU chain serves that encode. Engage
//! is read from the counter deltas (`vulkan_tower_stats`), never from "the model ran". The Metal
//! driver fills the same slots on an Apple build, so this driver registers only where it is absent.

var private g_vt_env_tower = true
var private g_vt_encodes = 0l
var private g_vt_rows = 0l
var private g_vt_blocks = 0l
var private g_vt_declines : table<string; int64>
var private g_vt_declines_total = 0l

//! why a chain declined, by name in the declines table
enum private VulkanTowerDecline {
    knob         //! DASLLAMA_VK_TOWER=0, or set_vulkan_tower(false)
    unserved     //! a family whose chain the driver does not carry yet
    quant_mode   //! the tower's planes are not the lane the driver serves
    shape        //! a width, head size or row count the stamps do not serve
    device       //! the tier has no device, or a class failed to build
    gpu_error    //! a submit failed
}

//! Engage / decline counters; `blocks` = transformer blocks the driver ran. Model-level gates assert DELTAS on these.
def vulkan_tower_stats : tuple<encodes : int64; rows : int64; blocks : int64; declines : int64> {
    return (encodes = g_vt_encodes, rows = g_vt_rows, blocks = g_vt_blocks, declines = g_vt_declines_total)
}

//! The declines-by-reason table (a copy), keyed by `VulkanTowerDecline` value names.
def vulkan_tower_declines : table<string; int64> {
    return <- clone_to_move(g_vt_declines)
}

//! Test A/B seat: pin the tower driver on / off in-process (the env knob's setter twin).
def set_vulkan_tower(on : bool) {
    g_vt_env_tower = on
}

[cold_path]
def private vt_decline(why : VulkanTowerDecline) : bool {
    let key = "{why}"
    g_vt_declines[key] = (g_vt_declines?[key] ?? 0l) + 1l
    g_vt_declines_total++
    return false
}

//! the lane policy's question - would the driver serve the file's exact planes: no, it serves the q8 image, so a box with
//! this driver keeps the q8 lane the CPU chain reads
def private vulkan_tower_serves_exact() : bool => false

[unused_argument(t, s, npos)]
def private vulkan_gemma4v_blocks(t : Gemma4vTower; var s : Gemma4vState; npos : int64) : bool {
    if (!g_vt_env_tower) {
        return vt_decline(VulkanTowerDecline.knob)
    }
    return vt_decline(VulkanTowerDecline.unserved)
}

[unused_argument(t, s, npos)]
def private vulkan_gemma3v_blocks(t : Gemma3vTower; var s : Gemma3vState; npos : int64) : bool {
    if (!g_vt_env_tower) {
        return vt_decline(VulkanTowerDecline.knob)
    }
    return vt_decline(VulkanTowerDecline.unserved)
}

[unused_argument(t, s, npos)]
def private vulkan_qwen3v_blocks(t : Qwen3vTower; var s : Qwen3vState; npos : int64) : bool {
    if (!g_vt_env_tower) {
        return vt_decline(VulkanTowerDecline.knob)
    }
    return vt_decline(VulkanTowerDecline.unserved)
}

[unused_argument(t, s, npos)]
def private vulkan_qwen25v_blocks(t : Qwen25vTower; var s : Qwen25vState; npos : int64) : bool {
    if (!g_vt_env_tower) {
        return vt_decline(VulkanTowerDecline.knob)
    }
    return vt_decline(VulkanTowerDecline.unserved)
}

[init]
def dasllama_vulkan_tower_register {
    g_vt_env_tower = g_env_vulkan.vk_tower
    static_if (!typeinfo builtin_module_exists(das_metal)) {   //! the Metal driver owns the slots on an Apple build
        register_gemma4v_gpu(@@vulkan_gemma4v_blocks, @@vulkan_tower_serves_exact)
        register_gemma3v_gpu(@@vulkan_gemma3v_blocks, @@vulkan_tower_serves_exact)
        register_qwen3v_gpu(@@vulkan_qwen3v_blocks, @@vulkan_tower_serves_exact)
        register_qwen25v_gpu(@@vulkan_qwen25v_blocks)
    }
}
