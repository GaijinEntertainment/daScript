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
require dasllama/dasllama_math_default   // dot_q8q8_f16s - the q8 twin's block dot
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_config
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema

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
    v.dense_ffn = gpu_want_dense_ffn()
    v.trim = vulkan_trim()
    v.split = false
    v.dn_native = gpu_want_dn_native()
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
        install_vk_moe_hooks()
    }
    return true
}

//! the tier's whole hook set - the dry bake and the live arm install the same seats
def install_vk_moe_hooks {
    install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
    set_moe_gpu_ffn_xf_hooks(@@vk_moe_ffn_xf, @@vk_moe_ffn_xf_ok)
    set_moe_gpu_ffn_xf_async_hooks(@@vk_moe_ffn_xf_begin, @@vk_moe_ffn_xf_join, @@vk_moe_ffn_xf_async_rows)
    set_moe_gpu_ffn_xf_meet_hooks(@@vk_moe_ffn_xf_open, @@vk_moe_ffn_xf_push, @@vk_moe_ffn_xf_bundle_end, @@vk_moe_ffn_xf_close)
    set_moe_gpu_attn_dec_hooks(@@vk_moe_attn_dec, @@vk_moe_attn_dec_ok, @@vk_moe_attn_dec_claim, @@vk_moe_attn_dec_hydrate)
    set_moe_gpu_ffn_tail_hooks(@@vk_moe_ffn_tail, @@vk_moe_ffn_tail_pending)
    set_moe_gpu_span_dec_hook(@@vk_moe_span_dec)
    set_moe_gpu_ffn_bias_hook(@@vk_moe_upload_ffn_bias)
    install_moe_gpu_os_memory(@@vk_os_memory_now)
    install_rdec_note_weight_bytes(@@vk_rdec_note_weight_bytes)
    install_moe_gpu_resident_bias(@@vk_rdec_upload_bias)
    install_moe_gpu_resident_sinks(@@vk_rdec_upload_sinks)
}

// the model drop: no chain may be in flight, the decode-side counters reset, then the tier's teardown
def vk_drop_model {
    vk_xf_drop_check()
    vk_decode_drop_state()
    vk_drop_model_state()
}

//! the stream group a gate offset belongs to, or -1 for a resident region; the stream groups are asked first, since a slot's
//! pseudo-stack rebased onto a group covers the group's offsets too and would read as a residency
def private stream_group_of(offs1 : int64 const?; f1 : int) : int {
    let w0 = unsafe(offs1[0])
    let g = find_stream_group(w0, f1)
    return g if (g >= 0)
    if (find_stack_idx(w0, f1) < 0) {
        panic("dasLLAMA vulkan tier: dispatch against a region that is neither resident nor streamed (fmt {f1} element offset {w0})")
    }
    return -1
}

//! MoeGpuFfnFn: routed experts' whole FFN chain (gate+up GEMMs, activation, requant, down GEMM)
//! as device-side submit chains over resident stacks — hidden activations, and (npos > 0) the
//! combined output, never cross PCIe. Streamed layers route through the slot rail; else panics.
def vk_moe_ffn(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    ensure_ffn_state()
    let g = stream_group_of(offs1, f1)
    if (g >= 0) {
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

//! MoeGpuFfnXfFn: the f16-fed (cm2) twin of vk_moe_ffn — the combined batch form over the
//! window's f32 rows; resident stacks dispatch directly, a streamed group binds its slot first.
def vk_moe_ffn_xf(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    ensure_ffn_state()
    let g = stream_group_of(offs1, f1)
    if (g >= 0) {
        vk_moe_ffn_streamed_xf(g, goutp, offs1, offs3, offs2, nregions, xp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
        return
    }
    vk_moe_ffn_batch_xf(goutp, offs1, offs3, offs2, nregions, xp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
}

//! MoeGpuFfnXfBeginFn: a streamed group takes the per-expert chain over the given regions' experts, a resident layer dispatches directly; join collects
def vk_moe_ffn_xf_begin(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    ensure_ffn_state()
    let g = xf_pool_on() ? g_xf_pool_g : stream_group_of(offs1, f1)   //! a pool window's offsets name the pool's stacks: its group is the one it copies from
    if (g >= 0) {
        vk_moe_ffn_xf_begin_experts(g, offs1, offs3, offs2, nregions, xp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
        return
    }
    vk_moe_ffn_batch_xf_begin(offs1, offs3, offs2, nregions, xp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
}

def vk_moe_ffn_xf_join(var goutp : float?) {
    vk_moe_ffn_batch_xf_join(goutp)
}

//! MoeGpuFfnXfOpenFn: the meet form opens over a streamed group alone - a resident layer answers false and takes the whole chain
def vk_moe_ffn_xf_open(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) : bool {
    ensure_ffn_state()
    let g = xf_pool_on() ? g_xf_pool_g : stream_group_of(offs1, f1)
    return false if (g < 0)
    vk_moe_ffn_xf_open_experts(g, offs1, offs3, offs2, nregions, xp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    return true
}

def vk_moe_ffn_xf_push(r_lo, r_hi : int64; wait : bool) : bool => vk_moe_ffn_xf_push_experts(r_lo, r_hi, wait)

def vk_moe_ffn_xf_close {
    vk_moe_ffn_xf_close_experts()
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
        vk_drop_model()
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
        ci.ppEnabledExtensionNames = unsafe(addr<void?>(exts[0]))
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

//! what the loaded model holds on the device: the per-op tier's planes, and under the whole-model driver its weights and both sides of every K/V mirror slab
def private vk_resident_bytes : int64 => (g_gpu != null ? g_gpu.resident_bytes : 0l) + (g_rd != null && g_rd.ready ? g_rd.weight_bytes + 2l * rd_mir_bytes() * rd_mir_slabs() : 0l)

// The resident driver sizes a whole model against these BEFORE uploading anything. Probing the
// device here (rather than reporting 0 until something else arms it) is what lets the gate answer
// "would this fit?" on a cold tier.
def private vk_weight_budget : int64 => vk_moe_init() ? carved_budget() : 0l

//! what the carve took off the cap: the streamed slot and the decode block's mirrors
def private vk_weight_carve : int64 => gpu_want_arms_tier() && vk_moe_init() ? g_gpu.weight_budget - carved_budget() : 0l

// the largest legal single-SSBO binding range (maxStorageBufferRange) — the KV mirror's ceiling
//! the range a KV mirror side may bind, less the slack each of its `regions` carries past its planes (at the f32 codec's
//! element size) - the bound `vk_rdec_prepare` holds the mirror to
def private vk_binding_cap(regions : int64) : int64 => vk_moe_init() ? vk_max_storage_range() - RDEC_MIR_SLACK * 4l * regions : 0l

//! the slack elements every K/V mirror slab allocates past its planes a region - what `vk_rdec_prepare` adds to the stride
def private vk_mirror_slack : int64 => RDEC_MIR_SLACK

def private vk_plane_bytes(n, rows : int64; fmt : int) : int64 {
    if (!kq_rows_ok(n, fmt)) {
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
//! the ``moe_gpu_tier_arm_`` probe; ``[init]`` also runs it directly for the env-only path.
//! Idempotent — re-arming after ``set_gpu_tier_want_`` just refreshes the layer count.
def vulkan_moe_gpu_arm : bool {
    let layers = gpu_want_moe_layers()
    // the want truth is gpu_want_arms_tier — ONE definition shared with the loud-failure
    // checks, so "would arm" and "should have armed" can never drift apart
    if (!gpu_want_arms_tier()) {   // checked first — the device probe is not free
        arm_say(0, "GPU tier NOT armed - no rail requested (set DASLLAMA_GPU=1 or call set_gpu_tier_want_)")
        return false
    }
    if (!vulkan_backend_available()) {
        arm_say(2, "GPU tier NOT armed - {g_vk_why}")
        return false
    }
    set_moe_gpu_layer_count(layers)
    if (!moe_gpu_tier_installed()) {
        install_vk_moe_hooks()
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
        // an offline bake IS its config — echo it, never probe (there may be no device at all); the trim is the load's lever on every tier
        v = g_vk_dry_cfg
        v.trim = vulkan_trim()
        return true
    }
    if (!vulkan_moe_gpu_arm()) {
        return false
    }
    vk_dlim_wants_fill(v)
    if (g_gpu != null) {
        vk_caps_fill(g_gpu.phys, v)
    } else {
        if (!g_vk_probe_done) {
            var inscope instance <- create_instance("dasLLAMA dlim config", make_api_version(1u, 3u, 0u))
            volkLoadInstance(boost_value_to_vk(instance))
            vk_caps_fill(select_physical_device(instance), g_vk_probe_caps)
            g_vk_probe_done = true
        }
        v.coopmat_mode = g_vk_probe_caps.coopmat_mode
        v.subgroup_size = g_vk_probe_caps.subgroup_size
        v.max_storage_range = g_vk_probe_caps.max_storage_range
        v.vram_mb = g_vk_probe_caps.vram_mb
    }
    return true
}

//! the caps probe's answer, read once per process ahead of the tier's own device: a probe instance reloads the ICD, and
//! glibc's static TLS pool runs out after about a hundred reloads (libnvidia-tls: "cannot allocate memory in static TLS
//! block"), after which every vkCreateInstance answers ERROR_INCOMPATIBLE_DRIVER for the rest of the process
var private g_vk_probe_caps : DlimVulkanConfig
var private g_vk_probe_done = false

def private vk_bake_tag : string {
    var v : DlimVulkanConfig
    if (!vk_dlim_source(v)) {
        return ""
    }
    return dlim_vulkan_tag(v)
}

//! a kq GEMV class of the tier's `fmt` space on the CPU - its blk_contrib IS the device's block decode, bit for bit; a grid
//! format's staged codebook fills here, since no kernel head runs
def private vk_host_kq_cls(fmt : int) : KqGemvBase? {
    var k : KqGemvBase?
    if (fmt == int(KqFmt.k4)) {
        k = new KqGemvK4()
    } elif (fmt == int(KqFmt.q40)) {
        k = new KqGemvQ40()
    } elif (fmt == int(KqFmt.k5)) {
        k = new KqGemvK5()
    } elif (fmt == int(KqFmt.iq4xs)) {
        k = new KqGemvIq4xs()
    } elif (fmt == int(KqFmt.iq3s4)) {
        k = new KqGemvIq3s4()
    } elif (fmt == int(KqFmt.k3)) {
        k = new KqGemvK3()
    } elif (fmt == int(KqFmt.iq3s)) {
        var ki = new KqGemvIq3s()
        for (i in range(512)) {
            ki.gridw[i] = IQ3S_GRID[i]
        }
        k = ki
    } elif (fmt == int(KqFmt.iq3xxs)) {
        var kx = new KqGemvIq3xxs()
        for (i in range(256)) {
            kx.gridw[i] = IQ3XXS_GRID[i]
        }
        k = kx
    } elif (fmt == int(KqFmt.iq4nl)) {
        k = new KqGemvIq4nl()
    } elif (fmt == int(KqFmt.k2)) {
        k = new KqGemvK2()
    } elif (fmt == int(KqFmt.iq2s)) {
        var k2s = new KqGemvIq2s()
        for (i in range(2048)) {
            k2s.gridw[i] = IQ2S_GRID[i]
        }
        k = k2s
    } elif (fmt == int(KqFmt.iq2xs)) {
        var k2x = new KqGemvIq2xs()
        for (i in range(1024)) {
            k2x.gridw[i] = IQ2XS_GRID[i]
        }
        k = k2x
    } elif (fmt == int(KqFmt.iq2xxs)) {
        var k2xx = new KqGemvIq2xxs()
        for (i in range(512)) {
            k2xx.gridw[i] = IQ2XXS_GRID[i]
        }
        k = k2xx
    } elif (fmt == int(KqFmt.k6)) {
        k = new KqGemvK6()
    }
    return k
}

enum private VkTwinFamily {
    superblock
    block32
    q8
}

//! a lane's host twin: the format's GEMV class - the superblock family through its base, a per-32 or mx4 format its own
//! class, q8 none (its dot is spelled here) - and the device plane strides it reads at
struct private VkHostTwin {
    fam : VkTwinFamily
    k : KqGemvBase?
    q51 : Q51Gemv?
    iq4nl32 : Iq4nl32Gemv?
    q4032 : Q4032Gemv?
    q5032 : Q5032Gemv?
    mx4 : Mx4Gemv?
    qsb : int64   //! device bytes a block (a superblock, or a 32-block)
    ssb : int64
}

//! MoeGpuHostServesFn: the formats the host twin reads - every expert format the tier streams
def vk_host_twin_serves(fmt : int) : bool => expert_chain_fmt_ok(kq_fmt_of_slot(fmt))

//! MoeGpuHostTwinFn: a per-lane host twin for `fmt` - null for a format the twin does not serve
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-arena")]
def vk_host_twin(fmt : int) : void? {
    return null if (!vk_host_twin_serves(fmt))
    let f = unsafe(reinterpret<KqFmt>(uint8(fmt)))   //! the tier's stack-format space is int(KqFmt)
    var tw = new VkHostTwin()
    let b = dev_block_bytes(f)
    tw.qsb = b.qsb
    tw.ssb = b.ssb
    if (kq_sb(f)) {
        tw.fam = VkTwinFamily.superblock
        tw.k = vk_host_kq_cls(fmt)
    } elif (f == KqFmt.q8) {
        tw.fam = VkTwinFamily.q8
    } else {
        tw.fam = VkTwinFamily.block32
        if (f == KqFmt.q51) {
            tw.q51 = new Q51Gemv()
        } elif (f == KqFmt.iq4nl32) {
            tw.iq4nl32 = new Iq4nl32Gemv()
        } elif (f == KqFmt.q4032) {
            tw.q4032 = new Q4032Gemv()
        } elif (f == KqFmt.q5032) {
            tw.q5032 = new Q5032Gemv()
        } else {
            tw.mx4 = new Mx4Gemv()
        }
    }
    return unsafe(reinterpret<void?>(tw))
}

//! the per-32 and mx4 twin: the class's own block decode and dot (`blk_words`, `blk_xdot`), its planes and the Q8_0
//! activation bound as borrowed views, the row's blocks summed in order
def private vk_b32_rows(var k : auto(TK)?; var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re, qsb, ssb : int64) {
    let nb = n / 32l
    unsafe {
        vk_bind_view(k.wq, reinterpret<void?>(q), re * nb * qsb / 4l)
        vk_bind_view(k.wsu, reinterpret<void?>(s), (re * nb * ssb + 3l) / 4l)
        vk_bind_view(k.xqw, reinterpret<void?>(xq), n / 4l)
        vk_bind_view(k.axs, reinterpret<void?>(xs), nb)
        for (r in range64(rb, re)) {
            var acc = 0.0
            let wb0 = uint(r * nb)
            for (b in range64(nb)) {
                var wl = uint4(0u)
                var wh = uint4(0u)
                let sc = k->blk_words(wb0 + uint(b), wl, wh)
                acc += k->blk_xdot(wl, wh, sc, uint(b))
            }
            yp[r] = acc
        }
    }
}

//! the q8 twin: the device's int8 rows and f16 block scales, a block's dot in integers, (dot x d) x the activation scale
//! - the `Q8Gemv` class's fold order
def private vk_q8_rows(var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re : int64) {
    let nb = n / 32l
    unsafe {
        let wq = reinterpret<int8 const?>(q)
        let ws = reinterpret<uint16 const?>(s)
        for (r in range64(rb, re)) {
            yp[r] = dot_q8q8_f16s(wq + r * n, ws + r * nb, xq, xs, n)
        }
    }
}

//! `a` as a borrowed view of `count` elements at `p` (the earlier view forgotten)
def private vk_bind_view(var a : array<auto(TT)>; p : void?; count : int64) {
    unsafe {
        if (lock_count(a) != 0) {
            _builtin_forget_temp_array(a)
        }
        _builtin_make_temp_array_i64(a, p, count)
    }
}

//! MoeGpuHostRowsFn over the kq GEMV classes: the rows' superblocks folded a block at a time through the class's own decode
def vk_host_rows(twin : void?; var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re : int64) {
    var tw = unsafe(reinterpret<VkHostTwin?>(twin))
    if (tw.fam == VkTwinFamily.q8) {
        vk_q8_rows(yp, q, s, xq, xs, n, rb, re)
        return
    }
    if (tw.fam == VkTwinFamily.block32) {
        if (tw.q51 != null) {
            vk_b32_rows(tw.q51, yp, q, s, xq, xs, n, rb, re, tw.qsb, tw.ssb)
        } elif (tw.iq4nl32 != null) {
            vk_b32_rows(tw.iq4nl32, yp, q, s, xq, xs, n, rb, re, tw.qsb, tw.ssb)
        } elif (tw.q4032 != null) {
            vk_b32_rows(tw.q4032, yp, q, s, xq, xs, n, rb, re, tw.qsb, tw.ssb)
        } elif (tw.q5032 != null) {
            vk_b32_rows(tw.q5032, yp, q, s, xq, xs, n, rb, re, tw.qsb, tw.ssb)
        } else {
            vk_b32_rows(tw.mx4, yp, q, s, xq, xs, n, rb, re, tw.qsb, tw.ssb)
        }
        return
    }
    var k = tw.k
    let nsb = n / 256l
    let qsb = tw.qsb
    let ssb = tw.ssb
    unsafe {
        vk_bind_view(k.wq, reinterpret<void?>(q), re * nsb * qsb / 4l)
        vk_bind_view(k.wq4, reinterpret<void?>(q), re * nsb * qsb / 16l)
        vk_bind_view(k.wsu, reinterpret<void?>(s), re * nsb * ssb / 4l)
        vk_bind_view(k.xq4, reinterpret<void?>(xq), n / 16l)
        vk_bind_view(k.axs, reinterpret<void?>(xs), nsb)
        for (r in range64(rb, re)) {
            var acc = 0.0
            let wsb0 = uint(r * nsb)
            for (sb in range64(nsb)) {
                for (blk in range(8)) {
                    acc += k->blk_contrib(wsb0 + uint(sb), uint(blk), uint(sb))
                }
            }
            yp[r] = acc
        }
    }
}

[init, arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
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
    set_moe_gpu_stream_cache_fns(@@vk_stream_expert, @@vk_stream_fetch, @@vk_stream_cache_layer, @@vk_stream_cache_ring, @@vk_stream_ring_release)
    set_moe_gpu_stream_view_fn(@@vk_stream_view)
    set_moe_gpu_stream_request_fns(@@vk_stream_request, @@vk_stream_ready)
    set_moe_gpu_stream_cache_stats_fn(@@vk_stream_cache_stats)
    set_moe_gpu_stream_cache_order_fn(@@vk_stream_cache_order)
    set_moe_gpu_host_bytes_fn(@@vk_host_bytes)
    set_moe_gpu_mint_dry_fn(@@vk_mint_dry)
    set_moe_gpu_xf_pool_window_fn(@@vk_xf_pool_window)
    set_moe_gpu_host_twin_fns(@@vk_host_twin, @@vk_host_rows, @@vk_host_twin_serves)
    set_moe_gpu_vram_report(@@vk_resident_bytes)
    set_moe_gpu_device_report(@@vk_device_name)
    set_moe_gpu_budget_hooks(@@vk_weight_budget, @@vk_plane_bytes)
    set_moe_gpu_carve_fn(@@vk_weight_carve)
    set_moe_gpu_binding_cap(@@vk_binding_cap)
    set_moe_gpu_mirror_slack(@@vk_mirror_slack)
    install_rdec_prefill_ids(@@vk_rdec_prefill_ids, @@vk_rdec_set_emb, @@vk_rdec_upload_emb_f32)
    install_rdec_prefill_segs(@@vk_rdec_prefill_segs)
    install_rdec_prefill_caps(@@vk_rdec_prefill_caps)
    install_rdec_ple(@@vk_rdec_set_ple, @@vk_rdec_set_ple_layer, @@vk_rdec_set_ple_proj, @@vk_rdec_set_ple_table)
    install_moe_gpu_resident(@@vk_arena_reserve, @@vk_arena_place, @@vk_rdec_prepare,
        @@vk_rdec_upload_norms, @@vk_rdec_set_layer, @@vk_rdec_set_cls, @@vk_rdec_token, @@vk_rdec_prefill,
        @@vk_rdec_kv_sync, @@vk_rdec_kv_read)
    install_moe_gpu_resident_regions(@@vk_rdec_select_region)
    install_moe_gpu_resident_batch(@@vk_rdec_token_n, @@vk_rdec_token_n_rows)
    install_rdec_carry(@@vk_rdec_land_carry)
    install_rdec_draft(@@vk_rdec_set_head, @@vk_rdec_draft)
    install_rdec_verify(@@vk_rdec_verify, @@vk_rdec_verify_rows, @@vk_rdec_verify_landed_logits)
    install_rdec_select(@@vk_rdec_sel_ask, @@vk_rdec_sel_landing, @@vk_rdec_sel_land_rows)
    install_rdec_rollback(@@vk_rdec_rollback)
    install_rdec_dn_snaps(@@vk_rdec_dn_snap_save, @@vk_rdec_dn_snap_load)
    install_rdec_dn_peek(@@vk_rdec_dn_peek)
    install_rdec_dn_snap_ring(@@vk_rdec_dn_snap_ring)
    install_rdec_kv_xfer(@@vk_rdec_kv_xfer)
    install_rdec_token_seq(@@vk_rdec_token_seq)
    install_rdec_chain(@@vk_rdec_chain_wanted, @@vk_rdec_chain_ask)
    set_rdec_chain_charged(vk_rdec_chain_mode() != 0)
    vk_rdec_chain_announce()
    install_rdec_head_warm(@@vk_rdec_head_warm_ask, @@vk_rdec_head_warm_rows)
    install_moe_gpu_resident_dn(@@vk_rdec_set_dn_layer, @@vk_rdec_upload_dn_ba, @@vk_rdec_dn_own, @@vk_rdec_prefill_dn_own)
    install_moe_gpu_resident_moe(@@vk_rdec_moe_ok, @@vk_rdec_prepare_moe, @@vk_rdec_set_moe_layer, @@vk_rdec_set_moe_experts)
    install_moe_gpu_resident_hc(@@vk_rdec_set_hc, @@vk_rdec_set_ngram, @@vk_rdec_set_host_experts, @@vk_rdec_token_split, @@vk_rdec_ngram_ring, @@vk_rdec_prefill_hc)
    install_moe_gpu_resident_hot(@@vk_rdec_hot_layer, @@vk_rdec_hot_upload, @@vk_rdec_hot_submit, @@vk_rdec_hot_submit_rows)
    install_moe_gpu_resident_hot_dma(@@vk_rdec_hot_dma, @@vk_rdec_hot_dma_flush)
    install_moe_gpu_resident_hot_batch(@@vk_rdec_hot_batch)
    set_moe_gpu_dn_state_hooks(@@vk_dn_step_flush, @@vk_dn_step_invalidate, @@vk_dn_step_release)
    set_moe_gpu_heat_hooks(@@vk_moe_heat_query, @@vk_moe_heat_advise, @@vk_moe_ffn_begin, @@vk_moe_ffn_join)
    set_moe_gpu_qkv_hook(@@vk_moe_qkv)
    set_moe_gpu_drop_model_hook(@@vk_drop_model)
    vulkan_moe_gpu_arm()
}
