options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_math_cuda shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema
require dasllama/dasllama_config   // DlimVulkanConfig, dlim_vulkan_tag: the lane's section and tag
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_decode
require dasllama/dasllama_cuda_stream   // the streamed prompt's seats
require dasllama/dasllama_gpu_stream_cache   // the expert cache's seats, shared with the Vulkan home
require dasllama/dasllama_gpu_host_twin   // the host twin both discrete homes register

// The CUDA home's registration: under DASLLAMA_CUDA it takes the GPU tier's seats the Vulkan home otherwise fills - the
// "cuda" kernel backend (no GEMV core: the resident driver is its only dispatch), the arm, the budget and report hooks,
// and the resident seats dasllama_cuda_decode serves. The per-op MoE tier's seats are stubs that decline or panic by
// name: the home serves whole models through the resident driver alone, never a per-op block.

var private g_cud_arm_said = -1

def private cuda_backend_available : bool => cud_init()

//! the arm: the GPU want on, the device up; idempotent, re-run on every re-arm
def cuda_moe_gpu_arm : bool {
    if (!gpu_want_arms_tier()) {
        arm_say(g_cud_arm_said, 0, "GPU tier NOT armed - no rail requested (set DASLLAMA_GPU=1 or call set_gpu_tier_want_)")
        return false
    }
    if (!cud_init()) {
        arm_say(g_cud_arm_said, 2, "GPU tier NOT armed - no CUDA device")
        return false
    }
    set_moe_gpu_layer_count(gpu_want_moe_layers())
    if (!moe_gpu_tier_installed()) {
        install_moe_gpu_tier(@@cud_moe_ffn, @@cud_moe_upload_stack, @@cud_moe_cls, @@cud_moe_dense, @@cud_moe_drop_stacks, @@cud_moe_dn, @@cud_moe_attn)
        install_moe_gpu_os_memory(@@cud_os_memory_now)
        install_cud_stream_seats()
    }
    arm_say(g_cud_arm_said, 1, "GPU tier armed - cuda ({g_cud.name}), the whole-model driver")
    return true
}

[unused_argument(yp, wp, sp, xqp, xsp, n, d)]
def private cuda_no_gemv(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    panic("dasLLAMA cuda backend has no GEMV core - the resident driver is its only dispatch")
}

[unused_argument(yp, wp, sp, xqp, xsp, n, d, ntok)]
def private cuda_no_batch(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    panic("dasLLAMA cuda backend has no batch core - the resident driver is its only dispatch")
}

[unused_argument(y0p, y1p, y2p, wp, sp, woff0, woff1, woff2, xqp, xsp, n, d0, d1, d2)]
def private cuda_no_group3(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?; woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    panic("dasLLAMA cuda backend has no group3 core - the resident driver is its only dispatch")
}

[unused_argument(wp, sp, n, d)]
def private cuda_repack_noop(var wp : int8?; var sp : float?; n, d : int64) {}

[unused_argument(goutp, offs1, offs3, offs2, nregions, xqp, xsp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)]
def private cud_moe_ffn(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    panic("dasLLAMA cuda tier: the per-op MoE FFN is not served - the whole-model driver takes every call of a model it homes")
}

//! the streamed prompt's seats: the tier's word that it streams expert stacks, the f16-fed chain's three hook sets, the pool
//! window, the expert cache's seats (the shared module's), the pool's DMA fill and placement batch, the pool planes the chain reads
def private install_cud_stream_seats {
    set_moe_gpu_streams(true)
    set_moe_gpu_ffn_xf_hooks(@@cud_moe_ffn_xf, @@cud_moe_ffn_xf_ok)
    set_moe_gpu_ffn_xf_async_hooks(@@cud_moe_ffn_xf_begin, @@cud_moe_ffn_xf_join, @@cud_moe_ffn_xf_async_rows)
    set_moe_gpu_ffn_xf_meet_hooks(@@cud_moe_ffn_xf_open, @@cud_moe_ffn_xf_push, @@cud_moe_ffn_xf_bundle_end, @@cud_moe_ffn_xf_close)
    set_moe_gpu_xf_pool_window_fn(@@cud_xf_pool_window)
    set_moe_gpu_stream_cache_fns(@@stream_expert, @@stream_fetch, @@stream_cache_layer, @@stream_cache_ring, @@stream_ring_release)
    set_moe_gpu_stream_view_fn(@@stream_view)
    set_moe_gpu_stream_request_fns(@@stream_request, @@stream_ready)
    set_moe_gpu_stream_cache_stats_fn(@@stream_cache_stats)
    set_moe_gpu_stream_cache_order_fn(@@stream_cache_order)
    install_moe_gpu_resident_hot_dma(@@cud_rdec_hot_dma, @@cud_rdec_hot_dma_flush)
    install_moe_gpu_resident_hot_batch(@@cud_rdec_hot_batch)
    set_cxf_pool_planes_fn(@@crd_xf_pool_planes)
    set_cxf_bias_planes_fn(@@crd_xf_bias_planes)
    install_host_twin()
}

[unused_argument(yp, woff, xqp, xsp, n, d, fmt)]
def private cud_moe_cls(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d : int64; fmt : int) {
    panic("dasLLAMA cuda tier: the per-op classifier is not served - the whole-model driver takes every call of a model it homes")
}

[unused_argument(yp, woff, xqp, xsp, n, d, nrows, fmt)]
def private cud_moe_dense(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d, nrows : int64; fmt : int) {
    panic("dasLLAMA cuda tier: the per-op dense GEMM is not served - the whole-model driver takes every call of a model it homes")
}

[unused_argument(yp, woq, woz, woo, fq, fz, fo, xqp, xsp, bp, gp, convw, wnormp, ap, dtp, stp, tailp, histp, npos, dim, cd, kd, di, nvh, nkh, ds, dconv, eps)]
def private cud_moe_dn(var yp : float?; woq, woz, woo : int64; fq, fz, fo : int;
                       xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
                       convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                       var stp : float?; var tailp : float?; histp : float const?;
                       npos, dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    panic("dasLLAMA cuda tier: the per-op deltanet block is not served - the whole-model driver takes every call of a model it homes")
}

[unused_argument(yp, kp, vp, woq, wok, wov, woo, fq, fk, fv, fo, xqp, xsp, rmsqp, rmskp, bqp, bkp, bvp, cosp, sinp, npos, dim, qd, kv_dim, hs, rot, n_heads, scale, eps, gated, qknorm)]
def private cud_moe_attn(var yp : float?; var kp : float?; var vp : float?;
                         woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                         xqp : int8 const?; xsp : float const?;
                         rmsqp : float const?; rmskp : float const?;
                         bqp : float const?; bkp : float const?; bvp : float const?;
                         cosp : float const?; sinp : float const?;
                         npos, dim, qd, kv_dim, hs, rot, n_heads : int64;
                         scale : float; eps : float; gated : bool; qknorm : bool) {
    panic("dasLLAMA cuda tier: the per-op attention block is not served - the whole-model driver takes every call of a model it homes")
}

let private CUD_VRAM_HEADROOM = 512l << 20l   //! the bytes the budget leaves the driver, the kernels' modules and the display

//! the plan's room record in the Vulkan home's terms: this process's ceiling (the card less the headroom), what it holds
//! (the boost's allocation count), what every process holds on the adapter (the card less the driver's free bytes)
def private cud_os_memory_now : tuple<budget : int64; usage : int64; adapter_dedicated : int64; adapter_shared : int64> {
    return (budget = 0l, usage = 0l, adapter_dedicated = 0l, adapter_shared = 0l) if (!cud_init())
    let total = int64(cuda_device_total_memory(g_cud.cc))
    let free_ = int64(cuda_device_free_memory())
    return (budget = total - CUD_VRAM_HEADROOM, usage = cuda_device_bytes_allocated(), adapter_dedicated = total - free_, adapter_shared = 0l)
}

def private cud_device_name : string => g_cud.ready ? g_cud.name : ""

//! what the resident driver may spend on planes: the device's free bytes less the headroom and the streamed slots' carve, probed cold
//! so the gate answers "would it fit?"
def private cud_weight_budget : int64 {
    return 0l if (!cud_init())
    let dry = cud_dry_budget()   // a mint's dry walk plans against the lane identity's cap, not the free memory left beside what this process holds
    return max((dry > 0l ? dry : int64(cuda_device_free_memory()) - CUD_VRAM_HEADROOM) - cud_stream_carve(), 0l)
}

def private cud_plane_bytes(n, rows : int64; fmt : int) : int64 {
    return 0l if (!kq_rows_ok(n, fmt))
    let b = dev_rows_bytes(kq_fmt_of_slot(fmt), n, rows)
    return b.q + b.s
}

//! what the carve took off the cap: the streamed slots
def private cud_weight_carve : int64 => cud_init() ? cud_stream_carve() : 0l

//! the discrete tier's config section as this home bakes and maps it (`DlimVulkanConfig`, its `home` "cuda"): the wants both
//! homes fill alike, the card's weight budget, a warp, no binding range; false with no CUDA device armed
def private cud_dlim_source(var v : DlimVulkanConfig) : bool {
    return false if (!cuda_moe_gpu_arm())
    gpu_dlim_wants_fill(v)
    v.home = "cuda"
    v.coopmat_mode = 0
    v.subgroup_size = int(g_cud.subgroup_size)
    v.max_storage_range = cud_binding_cap(1l)
    v.vram_mb = cud_identity_cap() / 1_000_000l
    return true
}

//! the weight cap the lane's identity folds: the pin, else the card's whole memory less the headroom - a number that holds across
//! the mint and the map, where the live budget (`cud_weight_budget`, off the free memory) moves by what the device holds
def private cud_identity_cap : int64 {
    let pin = gpu_want_vram_mb()
    return pin * 1_000_000l if (pin > 0l)
    return max(int64(cuda_device_total_memory(g_cud.cc)) - CUD_VRAM_HEADROOM, 0l)
}

//! the lane's tag: the section's, "" where the home is not armed (no lane then: the CPU lane's load)
def private cud_bake_tag : string {
    var v : DlimVulkanConfig
    return cud_dlim_source(v) ? dlim_vulkan_tag(v) : ""
}

//! MoeGpuDryBakeArmFn: the converter's offline bake on this home - the walk under the dry flag against the config's wants
//! (a config disagreeing with this process' knobs would write a lane whose identity lies about its plan: fail closed, name both);
//! the card's own budget stands in for a dry config's, since the home arms off a device alone
def private cud_dry_bake_arm(v : DlimVulkanConfig) : bool {
    if (v.home != "cuda") {   // the home is refused before the device is asked, as the vulkan arm does, so a foreign home fails by name on any box
        to_log(LOG_ERROR, "dasLLAMA cuda tier: dry-bake config names home '{v.home}' - this process bakes the cuda lane alone (a config another home wrote mints there)\n")
        return false
    }
    return false if (!cuda_moe_gpu_arm() || !dlim_dry_wants_ok(v, "cuda"))
    cud_mint_dry(true)
    cud_mint_dry_budget(v.vram_mb * 1_000_000l)
    return true
}

//! MoeGpuWouldAcceptFn: whether a gather the baked plan lacks is a refusal the walk makes anyway (pure arithmetic: the format
//! and the row lattice; the row count moves no refusal here) - a streamed stack asks the lattice alone
[unused_argument(rows)]
def private cud_moe_would_accept(n : int64; rows : int64; fmt : int; stream : bool) : bool => kq_rows_ok(n, fmt) && (stream || cud_serves_fmt(fmt))

//! no binding range on the driver API: a mirror side is one allocation of any size the device holds
[unused_argument(regions)]
def private cud_binding_cap(regions : int64) : int64 => 1l << 62l

def private cud_mirror_slack : int64 => RDEC_MIR_SLACK

[init, arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def dasllama_math_cuda_register() {
    register_kernel_backend(KernelBackend(name = "cuda", mm = @@cuda_no_gemv, batch = @@cuda_no_batch,
        group3 = @@cuda_no_group3, repack = @@cuda_repack_noop,
        needs_repack = false, priority = -1, available = @@cuda_backend_available))   //! the witness answers under either home
    register_gpu_home("cuda", @@cuda_install_seats)
    return if (!gpu_want_cuda())
    cuda_install_seats()
}

//! the CUDA home's seats in the tier: at [init] where the home is asked for, or when `set_gpu_home("cuda")` moves the tier here
def cuda_install_seats() {
    set_moe_gpu_arm("cuda", @@cuda_moe_gpu_arm)
    set_moe_gpu_bake_tag(@@cud_bake_tag)
    set_dlim_vulkan_source(@@cud_dlim_source)
    set_moe_gpu_dry_bake_arm(@@cud_dry_bake_arm)
    set_moe_gpu_would_accept_fn(@@cud_moe_would_accept)
    set_moe_gpu_mint_dry_fn(@@cud_mint_dry)
    set_moe_gpu_vram_report(@@cud_resident_bytes)
    set_moe_gpu_device_report(@@cud_device_name)
    set_moe_gpu_budget_hooks(@@cud_weight_budget, @@cud_plane_bytes)
    set_moe_gpu_carve_fn(@@cud_weight_carve)
    set_moe_gpu_binding_cap(@@cud_binding_cap)
    set_moe_gpu_mirror_slack(@@cud_mirror_slack)
    install_rdec_prefill_ids(@@cud_rdec_prefill_ids, @@cud_rdec_set_emb, @@cud_rdec_upload_emb_f32)
    install_moe_gpu_resident(@@cud_rdec_reserve, @@cud_rdec_place, @@cud_rdec_prepare,
        @@cud_rdec_upload_norms, @@cud_rdec_set_layer, @@cud_rdec_set_cls, @@cud_rdec_token, @@cud_rdec_prefill,
        @@cud_rdec_kv_sync, @@cud_rdec_kv_read)
    install_moe_gpu_resident_regions(@@cud_rdec_select_region)
    install_moe_gpu_resident_batch(@@cud_rdec_token_n, @@cud_rdec_token_n_rows)
    install_moe_gpu_resident_dn(@@cud_rdec_set_dn_layer, @@cud_rdec_upload_dn_ba, @@cud_rdec_dn_own, @@cud_rdec_prefill_dn_own)
    install_moe_gpu_resident_site_feeds(true)
    install_moe_gpu_resident_moe(@@cud_rdec_moe_ok, @@cud_rdec_prepare_moe, @@cud_rdec_set_moe_layer, @@cud_rdec_set_moe_experts)
    install_moe_gpu_resident_bias(@@cud_rdec_upload_bias)
    install_moe_gpu_resident_sinks(@@cud_rdec_upload_sinks)
    install_moe_gpu_resident_kv_share(true)
    install_rdec_ple(@@cud_rdec_set_ple, @@cud_rdec_set_ple_layer, @@cud_rdec_set_ple_proj, @@cud_rdec_set_ple_table)
    set_moe_gpu_dn_state_hooks(@@cud_dn_step_flush, @@cud_dn_step_invalidate, @@cud_dn_step_release)
    install_rdec_prefill_segs(@@cud_rdec_prefill_segs)
    install_rdec_prefill_caps(@@cud_rdec_prefill_caps)
    install_rdec_carry(@@cud_rdec_land_carry)
    install_rdec_draft(@@cud_rdec_set_head, @@cud_rdec_draft)
    install_rdec_verify(@@cud_rdec_verify, @@cud_rdec_verify_rows, @@cud_rdec_verify_landed_logits)
    install_rdec_rollback(@@cud_rdec_rollback)
    install_rdec_select(@@cud_rdec_sel_ask, @@cud_rdec_sel_landing, @@cud_rdec_sel_land_rows)
    install_rdec_kv_xfer(@@cud_rdec_kv_xfer)
    install_rdec_token_seq(@@cud_rdec_token_seq)
    install_rdec_chain(@@cud_rdec_chain_wanted, @@cud_rdec_chain_ask)
    set_rdec_chain_charged(cud_rdec_chain_mode() != 0)
    cud_rdec_chain_announce()
    install_rdec_note_weight_bytes(@@cud_rdec_note_weight_bytes)
    install_rdec_dn_snaps(@@cud_rdec_dn_snap_save, @@cud_rdec_dn_snap_load)
    install_rdec_dn_peek(@@cud_rdec_dn_peek)
    install_rdec_head_warm(@@cud_rdec_head_warm_ask, @@cud_rdec_head_warm_rows)
    install_rdec_hidden(@@cud_rdec_hidden_ask, @@cud_rdec_hidden_rows)
    install_rdec_segs_packed(@@cud_rdec_segs_packed)
    install_moe_gpu_resident_hc(@@cud_rdec_set_hc, @@cud_rdec_set_ngram, @@cud_rdec_set_host_experts, @@cud_rdec_token_split, @@cud_rdec_ngram_ring, @@cud_rdec_prefill_hc)
    install_moe_gpu_resident_hot(@@cud_rdec_hot_layer, @@cud_rdec_hot_upload, @@cud_rdec_hot_submit, @@cud_rdec_hot_submit_rows)
    set_moe_gpu_drop_model_hook(@@cud_drop_model)
    cuda_moe_gpu_arm()
}
