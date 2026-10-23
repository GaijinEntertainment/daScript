options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_math_cuda shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_decode

// The CUDA home's registration: under DASLLAMA_CUDA it takes the GPU tier's seats the Vulkan home otherwise fills - the
// "cuda" kernel backend (no GEMV core: the resident driver is its only dispatch), the arm, the budget and report hooks,
// and the resident seats dasllama_cuda_decode serves. The per-op MoE tier's seats are stubs that decline or panic by
// name: the dense slice homes whole models, not expert layers.

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
    }
    arm_say(g_cud_arm_said, 1, "GPU tier armed - cuda ({g_cud.name}), the whole-model driver's dense slice")
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
    panic("dasLLAMA cuda tier: the per-op MoE FFN is not served - the dense slice homes whole models")
}

[unused_argument(wq, ws, woff, n, rows, fmt, stream)]
def private cud_moe_upload_stack(wq : uint8 const?; ws : uint8 const?; woff : int64; n, rows : int64; fmt : int; stream : bool) : bool => false

[unused_argument(yp, woff, xqp, xsp, n, d, fmt)]
def private cud_moe_cls(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d : int64; fmt : int) {
    panic("dasLLAMA cuda tier: the per-op classifier is not served - the dense slice homes whole models")
}

[unused_argument(yp, woff, xqp, xsp, n, d, nrows, fmt)]
def private cud_moe_dense(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d, nrows : int64; fmt : int) {
    panic("dasLLAMA cuda tier: the per-op dense GEMM is not served - the dense slice homes whole models")
}

[unused_argument(n, stream)]
def private cud_moe_drop_stacks(n : int; stream : bool) {}

[unused_argument(yp, woq, woz, woo, fq, fz, fo, xqp, xsp, bp, gp, convw, wnormp, ap, dtp, stp, tailp, histp, npos, dim, cd, kd, di, nvh, nkh, ds, dconv, eps)]
def private cud_moe_dn(var yp : float?; woq, woz, woo : int64; fq, fz, fo : int;
                       xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
                       convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                       var stp : float?; var tailp : float?; histp : float const?;
                       npos, dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    panic("dasLLAMA cuda tier: the per-op deltanet block is not served - the dense slice homes whole models")
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
    panic("dasLLAMA cuda tier: the per-op attention block is not served - the dense slice homes whole models")
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

//! what the resident driver may spend on planes: the device's free bytes less the headroom, probed cold so the gate answers "would it fit?"
def private cud_weight_budget : int64 => cud_init() ? max(int64(cuda_device_free_memory()) - CUD_VRAM_HEADROOM, 0l) : 0l

def private cud_plane_bytes(n, rows : int64; fmt : int) : int64 {
    return 0l if (!kq_rows_ok(n, fmt))
    let b = dev_rows_bytes(kq_fmt_of_slot(fmt), n, rows)
    return b.q + b.s
}

def private cud_weight_carve : int64 => 0l

//! no binding range on the driver API: a mirror side is one allocation of any size the device holds
[unused_argument(regions)]
def private cud_binding_cap(regions : int64) : int64 => 1l << 62l

def private cud_mirror_slack : int64 => RDEC_MIR_SLACK

[init, arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def dasllama_math_cuda_register() {
    return if (!gpu_want_cuda())
    register_kernel_backend(KernelBackend(name = "cuda", mm = @@cuda_no_gemv, batch = @@cuda_no_batch,
        group3 = @@cuda_no_group3, repack = @@cuda_repack_noop,
        needs_repack = false, priority = -1, available = @@cuda_backend_available))
    set_moe_gpu_arm("cuda", @@cuda_moe_gpu_arm)
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
    install_rdec_prefill_segs(@@cud_rdec_prefill_segs)
    install_rdec_prefill_caps(@@cud_rdec_prefill_caps)
    set_moe_gpu_drop_model_hook(@@cud_drop_model)
    cuda_moe_gpu_arm()
}
