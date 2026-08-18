options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_math_vulkan shared public
options _cyclomatic_complexity = 0   // the capability-probe/arm ladders are the domain shape
options _function_length = 0   // same

require dasllama/dasllama_lint public
require dasllama/dasllama_vulkan_classes public   // the facade: consumers reach the whole backend through this module
require dasllama/dasllama_vulkan_common public
require dasllama/dasllama_vulkan_decode public
require dasllama/dasllama_vulkan_prefill public
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_env
require dasllama/dasllama_config

// dasLLAMA Vulkan GPU tier — the backend ENTRY: capability probe + arm, the dry-bake arm, the
// .dlim identity source, the FFN router, and the [init] that installs every hook. The backend
// body lives in the family this module re-exports: dasllama_vulkan_classes (the kernel home),
// dasllama_vulkan_common (device state + rails), dasllama_vulkan_decode / _prefill (drivers).
//
// Per-layer MoE expert-stack offload: the loader gathers offloaded layers' PREPARED planes into
// row-major device planes and hands them to vk_moe_upload_stack (moe_gpu_upload_resident — every
// load rail, gguf and mapped image alike); the MoE dispatches then route those layers' whole
// expert FFN chain (gate+up GEMMs -> act -> requant -> down) through vk_moe_ffn — decode row
// counts on the GEMV kernels, prefill batches on the per-format tiled GEMMs. Needs 8/16-bit SSBO
// storage + compute subgroups + shaderIntegerDotProduct (probed; any absent = tier off, CPU
// fallback). The registered "vulkan" KernelBackend stays a named-only stub (priority -1, GEMV
// slots panic) — the MoE tier hooks are the real integration surface.

// fill the env-else-programmatic wants half of a config section (the caps half is the
// device's / the dry config's) — shared by the live source and the dry arm's verify
def private vk_dlim_wants_fill(var v : DlimVulkanConfig) {
    v.moe_layers = gpu_want_moe_layers()
    v.moe_stream = gpu_want_moe_stream()
    v.dn = gpu_want_dn()
    v.attn = gpu_want_attn()
    v.dnd = gpu_want_dnd()
    v.shexp = gpu_want_shexp()
    v.qkv = gpu_want_qkv()
    v.cls = gpu_want_cls_upload()   // the WALK's gate, not the arm-intent (they had split defaults)
    v.dense = gpu_want_dense()
    v.dense_attn = gpu_want_dense_attn()
    v.dense_shexp = gpu_want_dense_shexp()
    v.trim = g_env_vulkan.trim
}

//! Arm the tier for an OFFLINE bake: the load walk runs its full accept/refuse arithmetic
//! against the config's numbers, gathers collect into the bake blob, and no Vulkan call is
//! made — no instance, no device, no VRAM. The converter's `-f vulkan` rail.
def vulkan_dry_bake_arm(v : DlimVulkanConfig) : bool {
    // the walk's gates read env-else-programmatic wants — a bake whose config disagrees with
    // them would write an image whose identity lies about its own plan. Fail closed, name both.
    var w = v
    vk_dlim_wants_fill(w)
    w.trim = v.trim   // trim shapes the SAVE, not the walk — the converter's flag decides it
    if (dlim_vulkan_tag(w) != dlim_vulkan_tag(v)) {
        to_log(LOG_ERROR, "dasLLAMA vulkan tier: dry-bake config disagrees with this process' wants (set the DASLLAMA_GPU_* knobs to match)\n  config {dlim_vulkan_tag(v)}\n  wants  {dlim_vulkan_tag(w)}\n")
        return false
    }
    g_vk_dry_want = true
    g_vk_dry_cfg = v
    set_moe_gpu_layer_count(v.moe_layers)
    if (!moe_gpu_tier_installed()) {
        install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
    }
    return true
}

//! MoeGpuFfnFn: routed experts' whole FFN chain (gate+up GEMMs, activation, requant, down GEMM)
//! as device-side submit chains over resident stacks — hidden activations, and (npos > 0) the
//! combined output, never cross PCIe. Streamed layers route through the slot rail; else panics.
def vk_moe_ffn(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    ensure_ffn_state()
    if (find_stack_idx(unsafe(offs1[0]), f1) < 0) {
        let w0 = unsafe(offs1[0])
        let g = find_stream_group(w0, f1)
        if (g < 0) {
            panic("dasLLAMA vulkan tier: dispatch against a region that is neither resident nor streamed (fmt {f1} element offset {w0})")
        }
        vk_moe_ffn_streamed(g, goutp, offs1, offs3, offs2, nregions, xqp, xsp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
        return
    }
    if (nrows < BATCH_TILE) {
        assert(npos == 0l, "dasLLAMA vulkan tier: combined output needs the batch arm")
        vk_moe_ffn_gemv(goutp, offs1, offs3, offs2, nregions, xqp, xsp, n, nfe, nrows, f1, f3, f2, is_gelu)
    } else {
        vk_moe_ffn_batch(goutp, offs1, offs3, offs2, nregions, xqp, xsp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    }
}

// ===== registration =====


// MoeGpuUnmapNotifyFn: a model's mapping is about to unmap — release imported device memory
// over it FIRST (reading through a freed import faults the device). Whole-model teardown: the
// single-owner tier's stream mirrors all belong to the unmapping model when any hit the range.
def private vk_unmap_notify(base : void?; bytes : uint64) {
    if (g_gpu == null || g_gpu.dry || base == null) {
        return
    }
    var hit = false
    let b0 = intptr(base)
    for (ss in g_gpu.stream_stacks) {
        let p = intptr(ss.hwq.mapped)
        if (p >= b0 && p < b0 + bytes) {
            hit = true
            break
        }
    }
    if (hit) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: model unmapping - dropping imported stream mirrors first\n")
        vk_drop_model_state()
    }
}

var private g_vk_probed = false
var private g_vk_available = false
var private g_vk_why = ""   // why the device probe said no — discarded before, which made it silent
var private g_vk_arm_said = -1   // last logged arm outcome; -1 = nothing said yet

def private vulkan_backend_available : bool {
    if (g_vk_probed) {
        return g_vk_available
    }
    g_vk_probed = true
    vk_arm_moltenvk_env()   // must precede the FIRST volkInitialize — MVK config latches at ICD load
    // raw calls, not boost creators — those panic on failure instead of reporting absence
    if (volkInitialize() != 0) {
        g_vk_why = "no vulkan loader (volkInitialize failed)"
        return false
    }
    var ci = VkInstanceCreateInfo()
    // portability drivers (MoltenVK) reject a bare vkCreateInstance — same opt-in as boost's create_instance
    var exts : array<string>
    if (instance_extension_available("VK_KHR_portability_enumeration")) {
        exts |> push("VK_KHR_portability_enumeration")
        ci.flags.enumerate_portability_khr = true
        ci.enabledExtensionCount = 1u
        unsafe {
            ci.ppEnabledExtensionNames = addr<void?>(exts[0])
        }
    }
    var inst : VkInstance
    let cr = vkCreateInstance(ci, null, inst)
    if (cr != VkResult.SUCCESS) {
        g_vk_why = "vkCreateInstance refused ({cr})"
        return false
    }
    volkLoadInstance(inst)
    var count = 0u
    vkEnumeratePhysicalDevices(inst, count, null)
    vkDestroyInstance(inst, null)
    g_vk_available = count > 0u
    g_vk_why = g_vk_available ? "" : "vulkan loader present but 0 physical devices"
    return g_vk_available
}

[unused_argument(yp, wp, sp, xqp, xsp, n, d)]
def private vulkan_no_gemv(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    panic("dasLLAMA vulkan backend has no GEMV core - the MoE tier (DASLLAMA_GPU_MOE_LAYERS) is its only dispatch")
}

[unused_argument(yp, wp, sp, xqp, xsp, n, d, ntok)]
def private vulkan_no_batch(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    panic("dasLLAMA vulkan backend has no batch core - the MoE tier (DASLLAMA_GPU_MOE_LAYERS) is its only dispatch")
}

[unused_argument(y0p, y1p, y2p, wp, sp, woff0, woff1, woff2, xqp, xsp, n, d0, d1, d2)]
def private vulkan_no_group3(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?; woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    panic("dasLLAMA vulkan backend has no group3 core - the MoE tier (DASLLAMA_GPU_MOE_LAYERS) is its only dispatch")
}

[unused_argument(wp, sp, n, d)]
def private vulkan_repack_noop(var wp : int8?; var sp : float?; n, d : int64) {}

def private vk_resident_bytes : int64 => g_gpu != null ? g_gpu.resident_bytes : 0l

// The resident driver sizes a whole model against these BEFORE uploading anything. Probing the
// device here (rather than reporting 0 until something else arms it) is what lets the gate answer
// "would this fit?" on a cold tier.
def private vk_weight_budget : int64 => vk_moe_init() ? carved_budget() : 0l

// the largest legal single-SSBO binding range (maxStorageBufferRange) — the KV mirror's ceiling
def private vk_binding_cap : int64 => vk_moe_init() ? vk_max_storage_range() : 0l

def private vk_plane_bytes(n, rows : int64; fmt : int) : int64 {
    if (!arena_rows_ok(n, fmt)) {
        return 0l   // unplaceable row length — the plane can never upload, it costs nothing
    }
    let pb = stack_plane_bytes(n, rows, fmt)
    return pb.wq + pb.ws
}

def private vk_device_name : string {
    if (g_gpu == null) {
        return ""
    }
    var props : VkPhysicalDeviceProperties
    vkGetPhysicalDeviceProperties(g_gpu.phys, props)
    unsafe {
        return "{reinterpret<string>(addr(props.deviceName[0]))}"
    }
}

// vulkan_moe_gpu_arm is idempotent and re-runs on every re-arm, so log only on OUTCOME CHANGE.
// An unconditional line here floods the log (14k lines in one GLM run) and gets tuned out.
def private arm_say(outcome : int; msg : string) {
    if (!dasllama_noisy() || g_vk_arm_said == outcome) {
        return
    }
    g_vk_arm_said = outcome
    to_log(LOG_INFO, "dasLLAMA noisy: {msg}\n")
}

//! Arm the vulkan MoE tier from the effective GPU want (env-else-programmatic). Registered as
//! the ``moe_gpu_tier_arm`` probe; ``[init]`` also runs it directly for the env-only path.
//! Idempotent — re-arming after ``set_gpu_tier_want`` just refreshes the layer count.
def vulkan_moe_gpu_arm : bool {
    let layers = gpu_want_moe_layers()
    // the want truth is gpu_want_arms_tier — ONE definition shared with the loud-failure
    // checks, so "would arm" and "should have armed" can never drift apart
    if (!gpu_want_arms_tier()) {   // checked first — the device probe is not free
        arm_say(0, "GPU tier NOT armed - no rail requested (set DASLLAMA_GPU=1 or call set_gpu_tier_want)")
        return false
    }
    if (!vulkan_backend_available()) {
        arm_say(2, "GPU tier NOT armed - {g_vk_why}")
        return false
    }
    set_moe_gpu_layer_count(layers)
    if (!moe_gpu_tier_installed()) {
        install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
    }
    arm_say(1, "GPU tier armed - vulkan, moe layers {layers}{layers < 0l ? " (auto)" : ""}, stream {gpu_want_moe_stream()}")
    return true
}

// Fill the caps-derived half of the vulkan config section from a physical device. Phys-level
// queries only — vk_moe_init stays untouched, so the stream-reserve carve still happens AFTER
// the loader reports the model's slot need (518ae164a).
def private vk_caps_fill(phys : VkPhysicalDevice; var v : DlimVulkanConfig) {
    let has_cm = cooperative_matrix_supported(phys)
    let has_cm2 = has_cm && cooperative_matrix2_supported(phys)
    v.coopmat_mode = resolve_coopmat_mode(has_cm, has_cm2)
    v.subgroup_size = int(subgroup_properties(phys).subgroupSize)
    var props : VkPhysicalDeviceProperties
    vkGetPhysicalDeviceProperties(phys, props)
    v.max_storage_range = int64(props.limits.maxStorageBufferRange)
    v.vram_mb = resolve_weight_cap(query_heap_info(phys)) / 1_000_000l
}

// The DlimConfiguration vulkan source — RESOLVED values (the old tag folded raw env strings,
// so "auto" hid device-derived byte differences and a moving heap budget).
def private vk_dlim_source(var v : DlimVulkanConfig) : bool {
    if (g_vk_dry_want) {
        // an offline bake IS its config — echo it, never probe (there may be no device at all)
        v = g_vk_dry_cfg
        return true
    }
    if (!vulkan_moe_gpu_arm()) {
        return false
    }
    vk_dlim_wants_fill(v)
    if (g_gpu != null) {
        vk_caps_fill(g_gpu.phys, v)
    } else {
        var inscope instance <- create_instance("dasLLAMA dlim config", make_api_version(1u, 3u, 0u))
        volkLoadInstance(boost_value_to_vk(instance))
        vk_caps_fill(select_physical_device(instance), v)
    }
    return true
}

def private vk_bake_tag : string {
    var v : DlimVulkanConfig
    if (!vk_dlim_source(v)) {
        return ""
    }
    return dlim_vulkan_tag(v)
}

[init]
def dasllama_math_vulkan_register() {
    register_kernel_backend(KernelBackend(name = "vulkan", mm = @@vulkan_no_gemv, batch = @@vulkan_no_batch,
        group3 = @@vulkan_no_group3, repack = @@vulkan_repack_noop,
        needs_repack = false, priority = -1, available = @@vulkan_backend_available))
    set_moe_gpu_arm("vulkan", @@vulkan_moe_gpu_arm)
    set_moe_gpu_bake_tag(@@vk_bake_tag)
    set_dlim_vulkan_source(@@vk_dlim_source)
    set_moe_gpu_dry_bake_arm(@@vulkan_dry_bake_arm)
    set_moe_gpu_unmap_notify(@@vk_unmap_notify)
    set_moe_gpu_would_accept_fn(@@vk_moe_would_accept)
    set_moe_gpu_vram_report(@@vk_resident_bytes)
    set_moe_gpu_device_report(@@vk_device_name)
    set_moe_gpu_budget_hooks(@@vk_weight_budget, @@vk_plane_bytes)
    set_moe_gpu_binding_cap(@@vk_binding_cap)
    install_moe_gpu_resident(@@vk_arena_reserve, @@vk_arena_place, @@vk_rdec_prepare,
        @@vk_rdec_upload_norms, @@vk_rdec_set_layer, @@vk_rdec_set_cls, @@vk_rdec_token, @@vk_rdec_prefill,
        @@vk_rdec_sync_kv, @@vk_rdec_read_kv, @@vk_rdec_read_kv_bulk)
    set_moe_gpu_dn_state_hooks(@@vk_dn_step_flush, @@vk_dn_step_invalidate)
    set_moe_gpu_heat_hooks(@@vk_moe_heat_query, @@vk_moe_heat_advise, @@vk_moe_ffn_begin, @@vk_moe_ffn_join)
    set_moe_gpu_qkv_hook(@@vk_moe_qkv)
    set_moe_gpu_drop_model_hook(@@vk_drop_model_state)
    vulkan_moe_gpu_arm()
}
