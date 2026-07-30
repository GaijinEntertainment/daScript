options gen2
options _comment_hygiene = true

module dasllama_gpu_tier shared public

require dasllama/dasllama_env      // env-driven route defaults + noisy gate
require dasllama/dasllama_config   // DlimVulkanConfig — the dry-bake seam's config argument
require math

// The device-COOPERATION seam (SPI): hook types, install/unset slots, route/mark/want/status
// state, and the engine-facing forwarders for GPU tiers that COOPERATE with the CPU forward —
// per-op offload (MoE FFN, attention, deltanet, qkv groups, classifier, the heat cache) and the
// resident whole-stack driver's plumbing (rdec_*). Vulkan implements this today; Metal does NOT —
// on Apple Silicon memory is UMA and Metal integrates as a whole-forward driver through the
// override registries in dasllama_common (which vulkan's resident driver ALSO registers with).
// Direction of travel between the two styles: followup_vulkan.md — vulkan grows toward Metal's
// contract shape; this seam stays as the doesn't-fit-in-VRAM cooperative tier.

// ===== MoE GPU tier (per-layer expert offload) =====
// A GPU tier (dasllama_math_vulkan) installs FFN/attention/dn overrides + a weight-stack upload
// hook; hooks live HERE (not dasllama_common) so the engine never names a GPU symbol directly.
typedef MoeGpuFfnFn = function<(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n : int64; nfe : int64; nrows : int64; npos : int64; f1 : int; f3 : int; f2 : int; is_gelu : bool) : void>
typedef MoeGpuUploadFn = function<(wq : uint8 const?; ws : uint8 const?; woff : int64; n : int64; rows : int64; fmt : int; stream : bool) : bool>
typedef MoeGpuClsFn = function<(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n : int64; d : int64; fmt : int) : void>
typedef MoeGpuDenseFn = function<(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n : int64; d : int64; nrows : int64; fmt : int) : void>
typedef MoeGpuDropFn = function<(n : int; stream : bool) : void>
// The whole deltanet attention block of one recurrent layer as ONE device chain: qkv + z-gate
// GEMMs (Q8_0, always q8 plane), depthwise conv + silu + q/k L2-norm, chunked delta-rule scan,
// gated out-norm, out projection. histp feeds conv history channel-major; zeros = dn_reset's fresh state.
typedef MoeGpuDnFn = function<(var yp : float?; woq : int64; woz : int64; woo : int64;
                               xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
                               convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                               var stp : float?; var tailp : float?; histp : float const?;
                               npos : int64; dim : int64; cd : int64; kd : int64; di : int64;
                               nvh : int64; nkh : int64; ds : int64; dconv : int64; eps : float) : void>
// The whole softmax attention block of one full-attention layer as ONE device chain per window
// (qkv GEMMs, qk-RMSNorm, partial NEOX RoPE, flash-style causal GQA attention, out-gate, wo proj).
// Start-of-sequence ONLY (start_pos == 0) — continuation windows fall back to the CPU path (fail-closed).
typedef MoeGpuAttnFn = function<(var yp : float?; var kp : float?; var vp : float?;
                                 woq : int64; wok : int64; wov : int64; woo : int64;
                                 fq : int; fk : int; fv : int; fo : int;
                                 xqp : int8 const?; xsp : float const?;
                                 rmsqp : float const?; rmskp : float const?;
                                 cosp : float const?; sinp : float const?;
                                 npos : int64; dim : int64; qd : int64; kv_dim : int64;
                                 hs : int64; rot : int64; n_heads : int64; kv_mul : int64;
                                 scale : float; eps : float; gated : bool; qknorm : bool) : void>

[unused_argument(goutp, offs1, offs3, offs2, nregions, xqp, xsp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)]
def private moe_gpu_unset_ffn(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    panic("dasLLAMA: MoE GPU FFN dispatch hit without an installed GPU tier")
}

[unused_argument(wq, ws, woff, n, rows, fmt, stream)]
def private moe_gpu_unset_upload(wq : uint8 const?; ws : uint8 const?; woff : int64; n : int64; rows : int64; fmt : int; stream : bool) : bool {
    return false
}

[unused_argument(yp, woff, xqp, xsp, n, d, fmt)]
def private moe_gpu_unset_cls(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d : int64; fmt : int) {
    panic("dasLLAMA: GPU classifier dispatch hit without an installed GPU tier")
}

[unused_argument(yp, woff, xqp, xsp, n, d, nrows, fmt)]
def private moe_gpu_unset_dense(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d, nrows : int64; fmt : int) {
    panic("dasLLAMA: GPU dense GEMM dispatch hit without an installed GPU tier")
}

[unused_argument(n, stream)]
def private moe_gpu_unset_drop(n : int; stream : bool) {
    panic("dasLLAMA: GPU stack drop hit without an installed GPU tier")
}

[unused_argument(yp, woq, woz, woo, xqp, xsp, bp, gp, convw, wnormp, ap, dtp, stp, tailp, histp, npos, dim, cd, kd, di, nvh, nkh, ds, dconv, eps)]
def private moe_gpu_unset_dn(var yp : float?; woq, woz, woo : int64;
                             xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
                             convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                             var stp : float?; var tailp : float?; histp : float const?;
                             npos, dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    panic("dasLLAMA: GPU deltanet dispatch hit without an installed GPU tier")
}

[unused_argument(yp, kp, vp, woq, wok, wov, woo, fq, fk, fv, fo, xqp, xsp, rmsqp, rmskp, cosp, sinp, npos, dim, qd, kv_dim, hs, rot, n_heads, kv_mul, scale, eps, gated, qknorm)]
def private moe_gpu_unset_attn(var yp : float?; var kp : float?; var vp : float?;
                               woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                               xqp : int8 const?; xsp : float const?;
                               rmsqp : float const?; rmskp : float const?;
                               cosp : float const?; sinp : float const?;
                               npos, dim, qd, kv_dim, hs, rot, n_heads, kv_mul : int64;
                               scale : float; eps : float; gated : bool; qknorm : bool) {
    panic("dasLLAMA: GPU attention dispatch hit without an installed GPU tier")
}

var g_moe_gpu_ffn = @@moe_gpu_unset_ffn
var g_moe_gpu_upload = @@moe_gpu_unset_upload
var g_moe_gpu_cls = @@moe_gpu_unset_cls
var g_moe_gpu_dense = @@moe_gpu_unset_dense
var g_moe_gpu_drop = @@moe_gpu_unset_drop
var g_moe_gpu_dn = @@moe_gpu_unset_dn
var g_moe_gpu_attn = @@moe_gpu_unset_attn
var g_moe_gpu_installed = false
var g_moe_gpu_layer_count = 0l         // K = layers offloaded from the END (env-driven, set by the tier)
var g_moe_gpu_from_layer = 1000000l    // first offloaded layer (n_layers - K, set by the loader)
var g_moe_gpu_cls_off = -1l            // element offset of the resident classifier plane (loader contract)
var g_moe_gpu_cls_route = true         // runtime routing gate over the resident classifier (--cls-ab lever)
var g_moe_gpu_stream_from = 1000000l   // first STREAMED layer (streamed range = [stream_from, from_layer))
var g_moe_gpu_dense_offs : table<int64>   // resident dense planes, keyed woff * 8 + fmt (per-format offset spaces)
var g_moe_gpu_dense_route = true       // runtime routing gate over the marks (in-process A/B lever)
var g_moe_gpu_dn_layers : table<int64; int64>   // recurrent layer -> its qkv plane offset (tier-resident triples)
var g_moe_gpu_dn_route = true          // runtime routing gate over the deltanet marks (--dn-ab lever)
var g_moe_gpu_attn_layers : table<int64>  // full-attention layers whose q/k/v/o quad is tier-resident
var g_moe_gpu_attn_route = true        // runtime routing gate over the attention marks (--attn-ab lever)

// ===== the resident whole-stack decode driver hooks =====
// The tier registers these; common reaches the driver through them, so the engine never names a GPU
// symbol. reserve/place fill the arenas; prepare/norms/set_layer/set_cls build it; token runs one forward.
typedef RdecReserveFn = function<(fmt : int; cap_blocks : int64) : bool>
typedef RdecPlaceFn = function<(wq : array<uint8>; ws : array<uint8>; n : int64; rows : int64; fmt : int) : int64>
typedef RdecPrepareFn = function<(n_layers : int64; dim : int64; qd : int64; kv_dim : int64; head_size : int64; n_heads : int64; hidden : int64; vocab : int64; seq_cap : int64; neox : bool; eps : float; scale : float; qk_norm : bool) : bool>
typedef RdecNormsFn = function<(norms : array<float>) : void>
typedef RdecSetLayerFn = function<(l : int64; bq : int64; bk : int64; bv : int64; bo : int64; b1 : int64; b3 : int64; b2 : int64; fq : int; fk : int; fv : int; fo : int; f1 : int; f3 : int; f2 : int) : void>
typedef RdecSetClsFn = function<(cls_block : int64; cls_fmt : int) : void>
typedef RdecTokenFn = function<(x : array<float>; cossin : array<float>; pos : int64; cnt : int64; var logits : array<float>) : void>
typedef RdecPrefillFn = function<(x_batch : array<float>; cos_batch : array<float>; npos : int64; var logits : array<float>) : void>
typedef RdecSyncKvFn = function<(l : int64; kp : float const?; vp : float const?; npos : int64) : void>
typedef RdecReadKvFn = function<(l : int64; pos : int64; kp : float?; vp : float?) : void>
typedef RdecReadKvBulkFn = function<(l : int64; kp : float?; vp : float?; npos : int64) : void>

[unused_argument(fmt, cap_blocks)]
def private rdec_unset_reserve(fmt : int; cap_blocks : int64) : bool => false
[unused_argument(wq, ws, n, rows, fmt)]
def private rdec_unset_place(wq : array<uint8>; ws : array<uint8>; n, rows : int64; fmt : int) : int64 => -1l
[unused_argument(n_layers, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap, neox, eps, scale, qk_norm)]
def private rdec_unset_prepare(n_layers, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64; neox : bool; eps, scale : float; qk_norm : bool) : bool => false
[unused_argument(norms)]
def private rdec_unset_norms(norms : array<float>) {}
[unused_argument(l, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2)]
def private rdec_unset_set_layer(l, bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int) {}
[unused_argument(cls_block, cls_fmt)]
def private rdec_unset_set_cls(cls_block : int64; cls_fmt : int) {}
[unused_argument(x, cossin, pos, cnt, logits)]
def private rdec_unset_token(x : array<float>; cossin : array<float>; pos, cnt : int64; var logits : array<float>) {
    panic("dasLLAMA: resident decode token hit without an installed GPU driver")
}
[unused_argument(x_batch, cos_batch, npos, logits)]
def private rdec_unset_prefill(x_batch : array<float>; cos_batch : array<float>; npos : int64; var logits : array<float>) {
    panic("dasLLAMA: resident prefill hit without an installed GPU driver")
}
[unused_argument(l, kp, vp, npos)]
def private rdec_unset_sync(l : int64; kp : float const?; vp : float const?; npos : int64) {}
[unused_argument(l, pos, kp, vp)]
def private rdec_unset_read(l, pos : int64; kp : float?; vp : float?) {}
[unused_argument(l, kp, vp, npos)]
def private rdec_unset_read_bulk(l : int64; kp : float?; vp : float?; npos : int64) {
    panic("dasLLAMA: resident KV bulk readback hit without an installed GPU driver")
}

var g_rdec_reserve = @@rdec_unset_reserve
var g_rdec_place = @@rdec_unset_place
var g_rdec_prepare = @@rdec_unset_prepare
var g_rdec_norms = @@rdec_unset_norms
var g_rdec_set_layer = @@rdec_unset_set_layer
var g_rdec_set_cls = @@rdec_unset_set_cls
var g_rdec_token = @@rdec_unset_token
var g_rdec_prefill = @@rdec_unset_prefill
var g_rdec_sync_kv = @@rdec_unset_sync
var g_rdec_read_kv = @@rdec_unset_read
var g_rdec_read_kv_bulk = @@rdec_unset_read_bulk
var g_rdec_installed = false

//! Install the resident whole-stack decode driver (a GPU backend calls this at [init]).
def public install_moe_gpu_resident(reserve : RdecReserveFn; place : RdecPlaceFn; prepare : RdecPrepareFn;
                                    norms : RdecNormsFn; set_layer : RdecSetLayerFn; set_cls : RdecSetClsFn;
                                    token : RdecTokenFn; prefill : RdecPrefillFn;
                                    sync_kv : RdecSyncKvFn; read_kv : RdecReadKvFn;
                                    read_kv_bulk : RdecReadKvBulkFn) {
    g_rdec_reserve = reserve
    g_rdec_place = place
    g_rdec_prepare = prepare
    g_rdec_norms = norms
    g_rdec_set_layer = set_layer
    g_rdec_set_cls = set_cls
    g_rdec_token = token
    g_rdec_prefill = prefill
    g_rdec_sync_kv = sync_kv
    g_rdec_read_kv = read_kv
    g_rdec_read_kv_bulk = read_kv_bulk
    g_rdec_installed = true
}

def public moe_gpu_resident_installed() : bool => g_rdec_installed
def public rdec_reserve(fmt : int; cap_blocks : int64) : bool => invoke(g_rdec_reserve, fmt, cap_blocks)
def public rdec_place(wq : array<uint8>; ws : array<uint8>; n, rows : int64; fmt : int) : int64 => invoke(g_rdec_place, wq, ws, n, rows, fmt)
def public rdec_prepare(n_layers, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64; neox : bool; eps, scale : float; qk_norm : bool) : bool => invoke(g_rdec_prepare, n_layers, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap, neox, eps, scale, qk_norm)
def public rdec_upload_norms(norms : array<float>) { invoke(g_rdec_norms, norms) }
def public rdec_set_layer(l, bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int) { invoke(g_rdec_set_layer, l, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2) }
def public rdec_set_cls(cls_block : int64; cls_fmt : int) { invoke(g_rdec_set_cls, cls_block, cls_fmt) }
def public rdec_token(x : array<float>; cossin : array<float>; pos, cnt : int64; var logits : array<float>) { invoke(g_rdec_token, x, cossin, pos, cnt, logits) }
def public rdec_prefill(x_batch : array<float>; cos_batch : array<float>; npos : int64; var logits : array<float>) { invoke(g_rdec_prefill, x_batch, cos_batch, npos, logits) }
def public rdec_sync_kv(l : int64; kp : float const?; vp : float const?; npos : int64) { invoke(g_rdec_sync_kv, l, kp, vp, npos) }
def public rdec_read_kv(l, pos : int64; kp : float?; vp : float?) { invoke(g_rdec_read_kv, l, pos, kp, vp) }
def public rdec_read_kv_bulk(l : int64; kp : float?; vp : float?; npos : int64) { invoke(g_rdec_read_kv_bulk, l, kp, vp, npos) }

//! Install the GPU MoE tier (a GPU backend calls this at [init] when its device probe passes).
def public install_moe_gpu_tier(ffn : MoeGpuFfnFn; upload : MoeGpuUploadFn; cls : MoeGpuClsFn; dense : MoeGpuDenseFn; drop : MoeGpuDropFn; dn : MoeGpuDnFn; attn : MoeGpuAttnFn) {
    g_moe_gpu_ffn = ffn
    g_moe_gpu_upload = upload
    g_moe_gpu_cls = cls
    g_moe_gpu_dense = dense
    g_moe_gpu_drop = drop
    g_moe_gpu_dn = dn
    g_moe_gpu_attn = attn
    g_moe_gpu_installed = true
}

def public set_moe_gpu_layer_count(k : int64) {
    g_moe_gpu_layer_count = k
}

def public moe_gpu_layer_count : int64 {
    return g_moe_gpu_installed ? g_moe_gpu_layer_count : 0l
}

//! Loader contract: called once per load after the resident set is known (layers - K .. layers).
def public set_moe_gpu_from_layer(from_layer : int64) {
    g_moe_gpu_from_layer = from_layer
}

def public moe_gpu_layer_on_gpu(l : int64) : bool {
    // upper bound is load-bearing: the MTP draft block sits at index n_layers, above every uploaded
    // range, so an unbounded `l >= from_layer` dispatches against a region never uploaded.
    // total 0 = loader never reported it; keep the old unbounded answer
    return (g_moe_gpu_installed && l >= g_moe_gpu_from_layer
        && (g_moe_gpu_total_layers <= 0l || l < g_moe_gpu_total_layers))
}

//! Loader contract: first streamed layer (streamed range = [from, the resident set); reset per
//! load). Streamed layers keep their CPU planes — only PREFILL batches dispatch on the GPU,
//! the expert stacks DMA-streamed from pinned host mirrors; decode stays on the CPU.
def public set_moe_gpu_stream_from(from_layer : int64) {
    g_moe_gpu_stream_from = from_layer
}

def public moe_gpu_layer_streamed(l : int64) : bool {
    return g_moe_gpu_installed && l >= g_moe_gpu_stream_from && l < g_moe_gpu_from_layer
}

//! Loader contract: the resident classifier plane's element offset (-1 = none; reset per load).
def public set_moe_gpu_cls_off(woff : int64) {
    g_moe_gpu_cls_off = woff
}

def public moe_gpu_cls_on_gpu(woff : int64) : bool {
    return g_moe_gpu_installed && g_moe_gpu_cls_route && woff == g_moe_gpu_cls_off
}

//! Runtime routing gate over the resident classifier plane (the --cls-ab in-process lever).
//! Route flip only — the upload and its VRAM residency stay, matching the other tier gates.
def public set_moe_gpu_cls_route(on : bool) {
    g_moe_gpu_cls_route = on
}

//! Loader contract: mark one dense (attention-side) plane resident on the tier. Keyed by
//! (fmt, element offset) — the same identity the tier resolves stacks by, so the batch GEMM
//! funnels route any marked weight without knowing layers or arches. Reset per load.
def public set_moe_gpu_dense_off(fmt : int; woff : int64) {
    g_moe_gpu_dense_offs |> insert(woff * 8l + int64(fmt))
}

def public reset_moe_gpu_dense_offs {
    g_moe_gpu_dense_offs |> clear()
}

def public moe_gpu_dense_on_gpu(fmt : int; woff : int64) : bool {
    return g_moe_gpu_installed && g_moe_gpu_dense_route && key_exists(g_moe_gpu_dense_offs, woff * 8l + int64(fmt))
}

//! Runtime routing gate over the dense marks: uploads stay resident, only the funnels' routing
//! flips. The in-process A/B lever — both arms share one process, one residency, one budget
//! knee, so the measured delta is the dense rail alone (cross-run pp drift measured ±12).
def public set_moe_gpu_dense_route(on : bool) {
    g_moe_gpu_dense_route = on
}

//! Loader contract: mark one recurrent layer's deltanet triple resident on the tier (reset per
//! load). woq = the qkv plane's element offset — the decode-step state hooks key tier state by it.
def public set_moe_gpu_dn_layer(l : int64; woq : int64) {
    g_moe_gpu_dn_layers |> insert(l, woq)
}

//! The marked layer's qkv plane offset (-1 when the layer carries no resident triple).
def public moe_gpu_dn_layer_key(l : int64) : int64 {
    return g_moe_gpu_dn_layers?[l] ?? -1l
}

def public reset_moe_gpu_dn_layers {
    g_moe_gpu_dn_layers |> clear()
}

def public moe_gpu_dn_on_gpu(l : int64) : bool {
    return g_moe_gpu_installed && g_moe_gpu_dn_route && key_exists(g_moe_gpu_dn_layers, l)
}

//! Runtime routing gate over the deltanet marks (the --dn-ab in-process lever; uploads stay).
def public set_moe_gpu_dn_route(on : bool) {
    g_moe_gpu_dn_route = on
}

// ===== deltanet DECODE step =====
// Same MoeGpuDnFn hook at npos == 1, same dn marks/uploads; separate routing gate (--dnd-ab).
// State lives device-side across steps — flush/invalidate seams guard every CPU state consumer.

typedef MoeGpuDnFlushFn = function<(l : int64; var stp : float?; var histp : float?) : void>
typedef MoeGpuDnInvalidateFn = function<() : void>

[unused_argument(l, stp, histp)]
def private moe_gpu_noop_dn_flush(l : int64; var stp : float?; var histp : float?) {}
def private moe_gpu_noop_dn_invalidate() {}

var g_moe_gpu_dnd_route = true
var g_moe_gpu_dn_flush = @@moe_gpu_noop_dn_flush
var g_moe_gpu_dn_invalidate = @@moe_gpu_noop_dn_invalidate

def public moe_gpu_dnd_on_gpu(l : int64) : bool {
    return g_moe_gpu_installed && g_moe_gpu_dnd_route && key_exists(g_moe_gpu_dn_layers, l)
}

//! Runtime routing gate over the deltanet DECODE step (the --dnd-ab in-process lever).
def public set_moe_gpu_dnd_route(on : bool) {
    g_moe_gpu_dnd_route = on
}

//! Tier registration of the state-residency seams (beside install_moe_gpu_tier).
def public set_moe_gpu_dn_state_hooks(flush : MoeGpuDnFlushFn; inval : MoeGpuDnInvalidateFn) {
    g_moe_gpu_dn_flush = flush
    g_moe_gpu_dn_invalidate = inval
}

//! Sync one layer's device-resident deltanet state back into the engine's buffers (no-op when
//! the device copy is clean or absent). Call before ANY CPU read of dn_state/conv history.
def public moe_gpu_dn_flush(l : int64; var stp : float?; var histp : float?) {
    invoke(g_moe_gpu_dn_flush, l, stp, histp)
}

//! Drop all device-resident deltanet state (the engine rebuilt/zeroed its CPU copies).
def public moe_gpu_dn_invalidate() {
    invoke(g_moe_gpu_dn_invalidate)
}

//! Loader contract: mark one full-attention layer's q/k/v/o plane quad resident on the tier —
//! the prefill attention block then runs as one device chain. Reset per load.
def public set_moe_gpu_attn_layer(l : int64) {
    g_moe_gpu_attn_layers |> insert(l)
}

def public reset_moe_gpu_attn_layers {
    g_moe_gpu_attn_layers |> clear()
}

def public moe_gpu_attn_on_gpu(l : int64) : bool {
    return g_moe_gpu_installed && g_moe_gpu_attn_route && key_exists(g_moe_gpu_attn_layers, l)
}

//! Runtime routing gate over the attention marks (the --attn-ab in-process lever; uploads stay).
def public set_moe_gpu_attn_route(on : bool) {
    g_moe_gpu_attn_route = on
}

// ===== heat-pinned expert cache (decode split-FFN) =====
// The engine partitions each decode token's top-k against the tier's per-layer slot cache: hits
// dispatch the GPU FFN chain ASYNC (begin/join around the concurrent CPU miss work). Engine owns heat policy (counters, re-pin advise); tier owns slots, uploads, the offset space.

typedef MoeGpuHeatQueryFn = function<(l : int64; idx : int64 const?; k : int64;
                                      var hit : int8?; var o1 : int64?; var o3 : int64?; var o2 : int64?) : int64>
typedef MoeGpuHeatAdviseFn = function<(l : int64; b1 : int64; b3 : int64; b2 : int64; f1 : int; f3 : int; f2 : int;
                                       n : int64; nfe : int64; ids : int64 const?; nids : int64) : void>
typedef MoeGpuFfnBeginFn = function<(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64;
                                     xqp : int8 const?; xsp : float const?; n : int64; nfe : int64; nrows : int64;
                                     f1 : int; f3 : int; f2 : int; is_gelu : bool) : void>
typedef MoeGpuFfnJoinFn = function<(var goutp : float?) : void>

[unused_argument(l, idx, k, hit, o1, o3, o2)]
def private moe_gpu_unset_heat_query(l : int64; idx : int64 const?; k : int64;
                                     var hit : int8?; var o1 : int64?; var o3 : int64?; var o2 : int64?) : int64 {
    return 0l
}

[unused_argument(l, b1, b3, b2, f1, f3, f2, n, nfe, ids, nids)]
def private moe_gpu_unset_heat_advise(l : int64; b1, b3, b2 : int64; f1, f3, f2 : int;
                                      n, nfe : int64; ids : int64 const?; nids : int64) {}

[unused_argument(offs1, offs3, offs2, nregions, xqp, xsp, n, nfe, nrows, f1, f3, f2, is_gelu)]
def private moe_gpu_unset_ffn_begin(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64;
                                    xqp : int8 const?; xsp : float const?; n, nfe, nrows : int64;
                                    f1, f3, f2 : int; is_gelu : bool) {
    panic("dasLLAMA: async GPU FFN begin hit without an installed GPU tier")
}

[unused_argument(goutp)]
def private moe_gpu_unset_ffn_join(var goutp : float?) {
    panic("dasLLAMA: async GPU FFN join hit without an installed GPU tier")
}

var g_moe_gpu_heat_query = @@moe_gpu_unset_heat_query
var g_moe_gpu_heat_advise = @@moe_gpu_unset_heat_advise
var g_moe_gpu_ffn_begin = @@moe_gpu_unset_ffn_begin
var g_moe_gpu_ffn_join = @@moe_gpu_unset_ffn_join
var g_moe_gpu_heat_installed = false
var g_moe_gpu_heat_route = true

//! Tier registration of the heat-cache surface (beside install_moe_gpu_tier).
def public set_moe_gpu_heat_hooks(query : MoeGpuHeatQueryFn; advise : MoeGpuHeatAdviseFn;
                                  begin : MoeGpuFfnBeginFn; join : MoeGpuFfnJoinFn) {
    g_moe_gpu_heat_query = query
    g_moe_gpu_heat_advise = advise
    g_moe_gpu_ffn_begin = begin
    g_moe_gpu_ffn_join = join
    g_moe_gpu_heat_installed = true
}

def public moe_gpu_heat_on : bool {
    return g_moe_gpu_heat_installed && g_moe_gpu_heat_route
}

//! Runtime routing gate over the heat cache (the --heat-ab in-process lever; pins/uploads stay).
def public set_moe_gpu_heat_route(on : bool) {
    g_moe_gpu_heat_route = on
}

// ===== shared-expert residency (decode, async behind the routed experts) =====

var g_moe_gpu_shexp_layers : table<int64>   // layers whose shexp gate/up/down triple is resident
var g_moe_gpu_shexp_route = true

//! Loader contract: mark one layer's shared-expert triple resident on the tier. Reset per load.
def public set_moe_gpu_shexp_layer(l : int64) {
    g_moe_gpu_shexp_layers |> insert(l)
}

def public reset_moe_gpu_shexp_layers {
    g_moe_gpu_shexp_layers |> clear()
}

def public moe_gpu_shexp_on_gpu(l : int64) : bool {
    return g_moe_gpu_installed && g_moe_gpu_shexp_route && key_exists(g_moe_gpu_shexp_layers, l)
}

//! Runtime routing gate over the shared-expert marks (the --shexp-ab in-process lever).
def public set_moe_gpu_shexp_route(on : bool) {
    g_moe_gpu_shexp_route = on
}

// ===== decode q/k/v group (attention layers, one submit) =====
// Serial-chain work — nothing hides behind it, so it pays only where the GPU reads the planes
// faster than the CPU. That crossover moves with core count and card, hence the gate.

typedef MoeGpuQkvFn = function<(var yq : float?; var yk : float?; var yv : float?;
                                woq : int64; wok : int64; wov : int64; fq : int; fk : int; fv : int;
                                xqp : int8 const?; xsp : float const?;
                                n : int64; dq : int64; dk : int64; dv : int64) : void>

[unused_argument(yq, yk, yv, woq, wok, wov, fq, fk, fv, xqp, xsp, n, dq, dk, dv)]
def private moe_gpu_unset_qkv(var yq : float?; var yk : float?; var yv : float?;
                              woq, wok, wov : int64; fq, fk, fv : int;
                              xqp : int8 const?; xsp : float const?; n, dq, dk, dv : int64) {
    panic("dasLLAMA: GPU qkv dispatch hit without an installed GPU tier")
}

var g_moe_gpu_qkv = @@moe_gpu_unset_qkv
var g_moe_gpu_qkv_layers : table<int64>
var g_moe_gpu_qkv_route = true

//! Tier registration of the decode q/k/v group (beside install_moe_gpu_tier).
def public set_moe_gpu_qkv_hook(f : MoeGpuQkvFn) {
    g_moe_gpu_qkv = f
}

//! Loader contract: mark one attention layer's q/k/v triple resident. Reset per load.
def public set_moe_gpu_qkv_layer(l : int64) {
    g_moe_gpu_qkv_layers |> insert(l)
}

def public reset_moe_gpu_qkv_layers {
    g_moe_gpu_qkv_layers |> clear()
}

def public moe_gpu_qkv_on_gpu(l : int64) : bool {
    return g_moe_gpu_installed && g_moe_gpu_qkv_route && key_exists(g_moe_gpu_qkv_layers, l)
}

//! Runtime routing gate over the qkv marks (the --qkv-ab in-process lever; uploads stay).
def public set_moe_gpu_qkv_route(on : bool) {
    g_moe_gpu_qkv_route = on
}

// ===== per-model mark sets + whole-model drop (multi-model hosts) =====
// The loader-contract marks are keyed by WEIGHT ELEMENT OFFSETS and two models' offset spaces
// overlap — a multi-model host must swap the whole set per switch or funnels read wrong planes.

//! One loaded model's tier routing marks — everything the loader contract sets per load.
//! A multi-model host saves these after each load and restores them before stepping that model.
struct MoeGpuMarks {
    layer_count : int64
    from_layer : int64 = 1000000l
    stream_from : int64 = 1000000l
    total_layers : int64
    cls_off : int64 = -1l
    dense_offs : table<int64>
    dn_layers : table<int64; int64>
    attn_layers : table<int64>
    shexp_layers : table<int64>
    qkv_layers : table<int64>
    unsupported : string
}

//! Move the installed marks out into `m`; the globals read as no-marks after (routing gates —
//! the process-wide A/B levers — stay put).
def public moe_gpu_marks_save(var m : MoeGpuMarks) {
    m.layer_count = g_moe_gpu_layer_count
    m.from_layer = g_moe_gpu_from_layer
    m.stream_from = g_moe_gpu_stream_from
    m.total_layers = g_moe_gpu_total_layers
    m.cls_off = g_moe_gpu_cls_off
    m.dense_offs <- g_moe_gpu_dense_offs
    m.dn_layers <- g_moe_gpu_dn_layers
    m.attn_layers <- g_moe_gpu_attn_layers
    m.shexp_layers <- g_moe_gpu_shexp_layers
    m.qkv_layers <- g_moe_gpu_qkv_layers
    m.unsupported = g_moe_gpu_unsupported
    g_moe_gpu_layer_count = 0l
    g_moe_gpu_from_layer = 1000000l
    g_moe_gpu_stream_from = 1000000l
    g_moe_gpu_total_layers = 0l
    g_moe_gpu_cls_off = -1l
    g_moe_gpu_unsupported = ""
}

//! Does a saved mark set carry any armed state? A multi-model host uses this to attribute a
//! load's arm outcome to the slot that captured it — the process-global VRAM counter cannot
//! tell WHOSE planes are resident.
def public moe_gpu_marks_armed(m : MoeGpuMarks) : bool {
    return (!empty(m.dense_offs) || !empty(m.dn_layers) || !empty(m.attn_layers)
        || !empty(m.shexp_layers) || !empty(m.qkv_layers) || m.cls_off >= 0l
        || (m.total_layers > 0l && (m.from_layer < m.total_layers || m.stream_from < m.total_layers)))
}

//! Install `m` as the global marks (moves the tables in; `m` reads as no-marks after).
def public moe_gpu_marks_restore(var m : MoeGpuMarks) {
    g_moe_gpu_layer_count = m.layer_count
    g_moe_gpu_from_layer = m.from_layer
    g_moe_gpu_stream_from = m.stream_from
    g_moe_gpu_total_layers = m.total_layers
    g_moe_gpu_cls_off = m.cls_off
    g_moe_gpu_dense_offs <- m.dense_offs
    g_moe_gpu_dn_layers <- m.dn_layers
    g_moe_gpu_attn_layers <- m.attn_layers
    g_moe_gpu_shexp_layers <- m.shexp_layers
    g_moe_gpu_qkv_layers <- m.qkv_layers
    g_moe_gpu_unsupported = m.unsupported
    m.layer_count = 0l
    m.from_layer = 1000000l
    m.stream_from = 1000000l
    m.total_layers = 0l
    m.cls_off = -1l
    m.unsupported = ""
}

typedef MoeGpuDropModelFn = function<() : void>

def private moe_gpu_noop_drop_model() {}

var g_moe_gpu_drop_model = @@moe_gpu_noop_drop_model

//! Tier registration of the whole-model VRAM drop (beside install_moe_gpu_tier).
def public set_moe_gpu_drop_model_hook(f : MoeGpuDropModelFn) {
    g_moe_gpu_drop_model = f
}

//! Destroy every model-owned device object the tier holds (stacks, arenas, the resident
//! driver, all lazy dispatch state); device-lifetime state survives and rebuilds lazily.
//! Callers use dasllama_common's ``moe_gpu_drop_model``, which also resets the marks.
def public moe_gpu_drop_model_state() {
    invoke(g_moe_gpu_drop_model)
}

//! One attention layer's q/k/v decode projections on the tier — one submit, one shared
//! activation image (the three planes must share a quant class), results copied back per plane.
def public matmul_gpu_qkv(var yq : array<float>; var yk : array<float>; var yv : array<float>;
                          woq, wok, wov : int64; fq, fk, fv : int;
                          xq : array<int8>; xs : array<float>; n, dq, dk, dv : int64) {
    unsafe {
        invoke(g_moe_gpu_qkv, addr(yq[0]), addr(yk[0]), addr(yv[0]), woq, wok, wov, fq, fk, fv,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, dq, dk, dv)
    }
}

//! Partition the routed top-k against layer l's slot cache: hit[j] per original position,
//! compacted synthetic offset triples for the hits (pool coordinates), returns the hit count.
def public matmul_moe_gpu_heat_query(l : int64; idx : array<int64>; k : int64;
                                     var hit : array<int8>; var o1, o3, o2 : array<int64>) : int64 {
    unsafe {
        return invoke(g_moe_gpu_heat_query, l, addr<int64 const?>(idx[0]), k,
            addr(hit[0]), addr(o1[0]), addr(o3[0]), addr(o2[0]))
    }
}

//! Re-pin advice for layer l: the desired hot set (ids, heat-descending), the layer's real plane
//! bases + formats. The tier reconciles against its slots under its own upload budget.
def public moe_gpu_heat_advise(l : int64; b1, b3, b2 : int64; f1, f3, f2 : int;
                               n, nfe : int64; ids : array<int64>; nids : int64) {
    unsafe {
        invoke(g_moe_gpu_heat_advise, l, b1, b3, b2, f1, f3, f2, n, nfe, addr<int64 const?>(ids[0]), nids)
    }
}

//! Async decode FFN: submit the hit chain, return before it completes (join collects gout).
//! Exactly one begin may be in flight; the tier asserts the discipline.
def public matmul_moe_gpu_ffn_begin(offs1 : array<int64>; offs3 : array<int64>; offs2 : array<int64>; nregions : int64;
                                    xq : array<int8>; xs : array<float>; n, nfe, nrows : int64;
                                    f1, f3, f2 : int; is_gelu : bool) {
    unsafe {
        invoke(g_moe_gpu_ffn_begin,
            addr<int64 const?>(offs1[0]), addr<int64 const?>(offs3[0]), addr<int64 const?>(offs2[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, nfe, nrows, f1, f3, f2, is_gelu)
    }
}

//! Collect the pending async FFN's output rows (spin the fence, copy back).
def public matmul_moe_gpu_ffn_join(var gout : array<float>) {
    unsafe {
        invoke(g_moe_gpu_ffn_join, addr(gout[0]))
    }
}

// ===== GPU tier programmatic config + status =====
// The setter mirror of the DASLLAMA_GPU_* env vars (env stays the override) plus the status
// surface dashboards read — what is actually resident after a load, not what was requested.

//! Programmatic GPU tier request — the config-file mirror of the ``DASLLAMA_GPU_*`` env vars
//! (an env var present in the environment overrides its field). Set BEFORE ``load_model``,
//! then call ``moe_gpu_tier_arm`` so a backend can install its hooks.
struct GpuTierWant {
    auto_tier : bool     //!< request the measured-best rail set; individual knobs below still override
    moe_layers : int64   //!< resident expert-stack layers K, offloaded from the end (auto: -1 = fill the budget)
    moe_stream : int64   //!< streamed prefill layers S below the resident set (auto: -1 = all of them)
    vram_mb : int64      //!< resident-weight VRAM cap override in MB (0 = query the device)
    dn : bool            //!< recurrent (deltanet) layers through the device chain
    attn : bool          //!< full-attention layers through the device chain
    dense : bool         //!< dense attention-side planes resident
    dnd : bool           //!< deltanet DECODE step chain (implies the dn uploads)
    heat : int64         //!< heat-pinned expert cache slots per layer C (0 = off; needs stream mirrors)
    shexp : bool         //!< shared-expert triples resident; decode runs them async behind the routed experts
    qkv : bool           //!< attention q/k/v triples resident; decode projects them in one submit
}

var g_gpu_want = GpuTierWant()
var g_moe_gpu_backend = ""            // name registered with the arm hook ("vulkan")
var g_moe_gpu_unsupported = ""        // loader contract: why the loaded model can't ride the armed tier
var g_moe_gpu_total_layers = 0l       // loader contract: n_layers of the loaded model (status math)

def private moe_gpu_no_arm : bool => false
def private moe_gpu_no_vram : int64 => 0l
def private moe_gpu_no_device : string => ""
def private moe_gpu_no_budget : int64 => 0l
[unused_argument(n, rows, fmt)]
def private moe_gpu_no_plane_bytes(n, rows : int64; fmt : int) : int64 => 0l
var g_moe_gpu_arm = @@moe_gpu_no_arm
var g_moe_gpu_vram_report = @@moe_gpu_no_vram
var g_moe_gpu_device_report = @@moe_gpu_no_device
var g_moe_gpu_budget_report = @@moe_gpu_no_budget
var g_moe_gpu_plane_bytes_fn = @@moe_gpu_no_plane_bytes

def public set_gpu_tier_want(want : GpuTierWant) {
    g_gpu_want = want
}

// Noisy mode: the engine says what it did instead of leaving it to be inferred from timings.
// Lives here (not dasllama_common) so the GPU drivers can report too.
var private g_noisy = -1

//! Enable/disable noisy diagnostics. Also DASLLAMA_NOISY=1.
def public set_dasllama_noisy(on : bool) {
    g_noisy = on ? 1 : 0
}
def public dasllama_noisy() : bool {
    if (g_noisy < 0) {
        g_noisy = env_flag("DASLLAMA_NOISY", false) ? 1 : 0
    }
    return g_noisy == 1
}

//! One switch for the measured-best rail set: expert stacks sized to the VRAM budget, plus the
//! deltanet/attention/dense/shared-expert/classifier rails. qkv and heat stay off (measured
//! negative and a wash). Any DASLLAMA_GPU_* knob still overrides individually.
def public gpu_want_auto : bool
    => env_flag("DASLLAMA_GPU", g_gpu_want.auto_tier)

//! Effective tier request, env-else-programmatic — the load walk and the GPU backends read
//! these instead of the raw env vars.

// -1 = auto: the upload walk resolves it against n_layers and lets its budget stops place the split
def public gpu_want_moe_layers : int64
    => env_int64("DASLLAMA_GPU_MOE_LAYERS",
                 g_gpu_want.moe_layers != 0l ? g_gpu_want.moe_layers : (gpu_want_auto() ? -1l : 0l))

def public gpu_want_moe_stream : int64
    => env_int64("DASLLAMA_GPU_MOE_STREAM",
                 g_gpu_want.moe_stream != 0l ? g_gpu_want.moe_stream : (gpu_want_auto() ? -1l : 0l))

def public gpu_want_vram_mb : int64
    => env_int64("DASLLAMA_GPU_VRAM_MB", g_gpu_want.vram_mb)

def public gpu_want_dn : bool
    => env_flag("DASLLAMA_GPU_DN", g_gpu_want.dn || gpu_want_auto())

def public gpu_want_attn : bool
    => env_flag("DASLLAMA_GPU_ATTN", g_gpu_want.attn || gpu_want_auto())

def public gpu_want_dense : bool
    => env_flag("DASLLAMA_GPU_DENSE", g_gpu_want.dense || gpu_want_auto())

//! Dense-rail sub-arm for gemma-4's parallel dense shared expert (moe_dense_shexp) w1/w3/w2 triple.
//! Opt-IN: MEASURED a wash-to-loss (3 dispatches each paying acts-up + y-down around a CPU gate)
//! and the uploads displace 1-2 expert layers. Kept as the upload half of a future FUSED chain.
def public gpu_want_dense_shexp : bool
    => env_flag("DASLLAMA_GPU_DENSE_SHEXP", false)

//! The dense rail's attention half (per-layer q/o pairs). Opt-OUT (``DASLLAMA_GPU_DENSE_ATTN=0``)
//! — measured a LOSS on gemma-4 where the shexp triple is a large win, so the two halves need
//! independent control rather than one DENSE gate.
def public gpu_want_dense_attn : bool
    => env_flag("DASLLAMA_GPU_DENSE_ATTN", true)

def public gpu_want_dnd : bool
    => env_flag("DASLLAMA_GPU_DND", g_gpu_want.dnd || gpu_want_auto())

def public gpu_want_heat : int64
    => env_int64("DASLLAMA_GPU_HEAT", g_gpu_want.heat)

def public gpu_want_shexp : bool
    => env_flag("DASLLAMA_GPU_SHEXP", g_gpu_want.shexp || gpu_want_auto())

def public gpu_want_qkv : bool
    => env_flag("DASLLAMA_GPU_QKV", g_gpu_want.qkv)

//! Arm-intent for the classifier plane. The UPLOAD is opt-OUT (DASLLAMA_GPU_CLS=0 declines it), so
//! this is only the stronger "=1 arms the tier by itself" signal — letting the classifier offload be
//! measured without also offloading expert layers. Unset keeps today's behavior exactly.
def public gpu_want_cls : bool
    => env_flag("DASLLAMA_GPU_CLS", false)

//! The classifier UPLOAD gate (opt-OUT, default on when the tier is armed) — distinct from
//! [[gpu_want_cls]], the opt-IN arm signal. One definition, shared by the upload walk and the
//! DlimConfiguration identity, so the two can never disagree on the default again.
def public gpu_want_cls_upload : bool
    => env_flag("DASLLAMA_GPU_CLS", true)

//! A GPU backend registers its arm probe here at ``[init]`` so config-driven arming works
//! after module init (the env path arms at ``[init]`` directly).
def public set_moe_gpu_arm(name : string; f : function<() : bool>) {
    g_moe_gpu_backend = name
    g_moe_gpu_arm = f
}

//! (Re-)arm the GPU tier from the current want. False = no GPU backend in this build,
//! nothing requested, or the device probe failed.
def public moe_gpu_tier_arm : bool {
    return invoke(g_moe_gpu_arm)
}

//! The tier reports its resident VRAM bytes through this hook (registered beside the arm probe).
def public set_moe_gpu_vram_report(f : function<() : int64>) {
    g_moe_gpu_vram_report = f
}

//! The tier reports its device's marketing name through this hook ("" until the device exists).
def public set_moe_gpu_device_report(f : function<() : string>) {
    g_moe_gpu_device_report = f
}

//! The tier reports its weight budget + its device-layout plane sizing through these. The
//! resident driver needs both BEFORE any upload: it sums a whole model and either fits or declines,
//! so the sizing must come from the backend that owns the layout, not a second copy of the strides.
def public set_moe_gpu_budget_hooks(budget : function<() : int64>; plane : function<(n, rows : int64; fmt : int) : int64>) {
    g_moe_gpu_budget_report = budget
    g_moe_gpu_plane_bytes_fn = plane
}

//! Device-local bytes the armed tier may spend on resident weights (0 = no tier).
def public moe_gpu_weight_budget() : int64 => invoke(g_moe_gpu_budget_report)

def private moe_gpu_no_q8n : bool => false
var private g_moe_gpu_expert_q8n = @@moe_gpu_no_q8n

//! The tier asks the loader to gather q8 EXPERT stacks as gguf-native q8n blocks (fmt 6, the
//! cm2 decode-in-load layout, no scale plane). Registered beside the budget hooks; the probe
//! may create the device — it must answer before the first gather.
def public set_moe_gpu_expert_q8n_probe(f : function<() : bool>) {
    g_moe_gpu_expert_q8n = f
}

[unused_argument(v)]
def private moe_gpu_no_dry_bake(v : DlimVulkanConfig) : bool => false

var private g_moe_gpu_dry_bake = @@moe_gpu_no_dry_bake

//! A GPU tier registers its OFFLINE-bake arm here: the load walk runs its full accept/refuse
//! arithmetic against the config's numbers, gathers collect, and no device call is made.
def public set_moe_gpu_dry_bake_arm(f : function<(v : DlimVulkanConfig) : bool>) {
    g_moe_gpu_dry_bake = f
}

//! Arm the offline bake (false = no GPU backend in this build, or the config disagrees with
//! this process' env wants — the tier logs the exact mismatch).
def public moe_gpu_dry_bake_arm(v : DlimVulkanConfig) : bool => invoke(g_moe_gpu_dry_bake, v)

//! A baked-plan gather's mapped-file window (P2 imported mirrors): pointers into the flavor
//! image's mapping + plane byte sizes. wq == null = no window recorded for (woff, fmt).
typedef MoeGpuSliceWindow = tuple<wq : void?; ws : void?; wqb : int64; wsb : int64>

[unused_argument(woff, fmt)]
def private moe_gpu_no_slice_window(woff : int64; fmt : int) : MoeGpuSliceWindow {
    return default<MoeGpuSliceWindow>
}

var private g_moe_gpu_slice_window = @@moe_gpu_no_slice_window

//! The loader registers its slice-window provider (populated while slicing a flavor image);
//! a GPU tier asks through [[moe_gpu_slice_window]] to import instead of copying.
def public set_moe_gpu_slice_window_fn(f : function<(woff : int64; fmt : int) : MoeGpuSliceWindow>) {
    g_moe_gpu_slice_window = f
}

def public moe_gpu_slice_window(woff : int64; fmt : int) : MoeGpuSliceWindow => invoke(g_moe_gpu_slice_window, woff, fmt)

[unused_argument(base, bytes)]
def private moe_gpu_no_unmap_notify(base : void?; bytes : uint64) {}

var private g_moe_gpu_unmap_notify = @@moe_gpu_no_unmap_notify

//! A GPU tier registers this to learn a model's mapping is about to unmap — imported device
//! memory over that mapping must be released FIRST (the Model finalizer invokes it).
def public set_moe_gpu_unmap_notify(f : function<(base : void?; bytes : uint64) : void>) {
    g_moe_gpu_unmap_notify = f
}

def public moe_gpu_unmap_notify(base : void?; bytes : uint64) {
    invoke(g_moe_gpu_unmap_notify, base, bytes)
}

[unused_argument(n, rows, fmt, stream)]
def private moe_gpu_always_accept(n : int64; rows : int64; fmt : int; stream : bool) : bool => true

var private g_moe_gpu_would_accept = @@moe_gpu_always_accept

//! A GPU tier registers its accept arithmetic (no side effects). The slice replay asks through
//! [[moe_gpu_would_accept]] whether a gather the plan lacks is a refusal boundary — the bake
//! never records refused gathers, so the walk's boundary attempt must re-refuse, not diverge.
def public set_moe_gpu_would_accept_fn(f : function<(n : int64; rows : int64; fmt : int; stream : bool) : bool>) {
    g_moe_gpu_would_accept = f
}

def public moe_gpu_would_accept(n : int64; rows : int64; fmt : int; stream : bool) : bool {
    return invoke(g_moe_gpu_would_accept, n, rows, fmt, stream)
}

def private moe_gpu_no_bake_tag : string => ""

var private g_moe_gpu_bake_tag = @@moe_gpu_no_bake_tag

//! The GPU tier's config-identity tag for the vulkan-flavor image ("" = tier absent/unarmed —
//! no flavor). Everything that shapes the load-time GPU walk must fold in: a config switch
//! must miss the flavor's identity and rebake.
def public set_moe_gpu_bake_tag(f : function<() : string>) {
    g_moe_gpu_bake_tag = f
}

def public moe_gpu_bake_tag : string => invoke(g_moe_gpu_bake_tag)

def public moe_gpu_expert_q8n() : bool => invoke(g_moe_gpu_expert_q8n)

//! Device-layout bytes one [rows x n] plane of `fmt` occupies (quants + scales). 0 = no tier.
def public moe_gpu_plane_bytes(n, rows : int64; fmt : int) : int64 => invoke(g_moe_gpu_plane_bytes_fn, n, rows, fmt)

def public moe_gpu_tier_installed : bool {
    return g_moe_gpu_installed
}

//! Loader contract: total transformer layers of the loaded model (resident/streamed counts).
def public set_moe_gpu_total_layers(n : int64) {
    g_moe_gpu_total_layers = n
}

var private g_moe_gpu_stream_need = 0l

//! Loader contract: worst-case streamed-slot bytes of THIS model (2x gate/up + down device
//! planes of the largest expert layer, via moe_gpu_plane_bytes). The tier carves its stream
//! reserve from this instead of a fixed guess; 0 = unknown (tier falls back to its constant).
def public set_moe_gpu_stream_need(bytes : int64) {
    g_moe_gpu_stream_need = bytes
}

def public moe_gpu_stream_need() : int64 => g_moe_gpu_stream_need

//! Loader contract: "" = the loaded model rides the armed tier; otherwise the human reason it
//! cannot (reset per load; meaningless while the tier is unarmed).
def public set_moe_gpu_model_support(reason : string) {
    g_moe_gpu_unsupported = reason
}

//! Live GPU-tier status (the control-page badge): actual post-load residency and routing.
struct GpuTierStatus {
    backend : string          //!< tier backend name ("vulkan"); "" = no GPU tier installed
    device : string           //!< GPU marketing name ("" until the device is created at first upload)
    supported : bool          //!< tier installed AND the loaded model rides it
    reason : string           //!< why not, when the tier is installed but ``supported`` is false
    resident_layers : int64   //!< expert-stack layers resident (from the end)
    streamed_layers : int64   //!< prefill-streamed layers below the resident set
    dn_layers : int64         //!< recurrent layers running the device deltanet chain
    attn_layers : int64       //!< full-attention layers running the device attention chain
    dense_planes : int64      //!< dense attention-side planes resident
    cls_resident : bool       //!< classifier plane resident
    vram_bytes : int64        //!< resident weight bytes on the device
}

def public gpu_tier_status : GpuTierStatus {
    if (!g_moe_gpu_installed) {
        return GpuTierStatus(reason = "no GPU tier installed")
    }
    let total = g_moe_gpu_total_layers
    let resident = total > 0l ? max(total - g_moe_gpu_from_layer, 0l) : 0l
    let streamed = total > 0l ? max(min(g_moe_gpu_from_layer, total) - min(g_moe_gpu_stream_from, total), 0l) : 0l
    return GpuTierStatus(
        backend = g_moe_gpu_backend,
        device = invoke(g_moe_gpu_device_report),
        supported = empty(g_moe_gpu_unsupported),
        reason = g_moe_gpu_unsupported,
        resident_layers = resident,
        streamed_layers = streamed,
        dn_layers = long_length(g_moe_gpu_dn_layers),
        attn_layers = long_length(g_moe_gpu_attn_layers),
        dense_planes = long_length(g_moe_gpu_dense_offs),
        cls_resident = g_moe_gpu_cls_off >= 0l,
        vram_bytes = invoke(g_moe_gpu_vram_report))
}

//! Upload one weight stack ([rows x n] at woff) as the GPU tier's row-major planes — q8 (fmt 0):
//! int8 quants + f16 scales; native K-quant (k4/k5/k6): superblock payloads + decoded scale rows.
//! stream = pinned host mirror for DMA streaming vs resident VRAM; false = tier absent/budget hit.
def public moe_gpu_upload_stack(wq : array<uint8> | #; ws : array<uint8> | #; woff : int64; n, rows : int64; fmt : int; stream : bool = false) : bool {
    if (!g_moe_gpu_installed) {
        return false
    }
    // empty planes hand the tier null: q8n stacks carry no scale plane, and the slice replay's
    // refusal-boundary serve is empty on purpose (the tier refuses on arithmetic, no bytes read)
    var wqp : uint8 const?
    if (!empty(wq)) {
        wqp = unsafe(addr<uint8 const?>(wq[0]))
    }
    var wsp : uint8 const?
    if (!empty(ws)) {
        wsp = unsafe(addr<uint8 const?>(ws[0]))
    }
    return invoke(g_moe_gpu_upload, wqp, wsp, woff, n, rows, fmt, stream)
}

//! Fused MoE FFN on the GPU tier — the whole gate+up -> act -> requant -> down chain of the
//! routed experts in one device-side dispatch (decode and prefill alike; the tier chunks
//! internally). gout is nrows x n floats; xq/xs is the gathered activation in the layer's own quant form.
def public matmul_moe_gpu_ffn(var gout : array<float>; offs1 : array<int64>; offs3 : array<int64>; offs2 : array<int64>;
                              nregions : int64; xq : array<int8>; xs : array<float>; n, nfe, nrows : int64;
                              f1, f3, f2 : int; is_gelu : bool) {
    unsafe {
        invoke(g_moe_gpu_ffn, addr(gout[0]),
            addr<int64 const?>(offs1[0]), addr<int64 const?>(offs3[0]), addr<int64 const?>(offs2[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), null, null, n, nfe, nrows, 0l, f1, f3, f2, is_gelu)
    }
}

//! The combined prefill form: the routed weighted-sum reduce ALSO runs on the device, so gout
//! returns npos x n combined rows instead of nrows x n bucket rows (down rows never cross PCIe —
//! readback shrinks k-fold). w is the routing weight plane (npos x k), inv the grid->bucket row map.
def public matmul_moe_gpu_ffn_combined(var gout : array<float>; offs1 : array<int64>; offs3 : array<int64>; offs2 : array<int64>;
                                       nregions : int64; xq : array<int8>; xs : array<float>; w : array<float>; inv : array<uint>;
                                       n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    unsafe {
        invoke(g_moe_gpu_ffn, addr(gout[0]),
            addr<int64 const?>(offs1[0]), addr<int64 const?>(offs3[0]), addr<int64 const?>(offs2[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), addr<float const?>(w[0]), addr<uint const?>(inv[0]),
            n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    }
}

//! Dense batched GEMM on the GPU tier — y [nrows x d] = X·Wᵀ over the resident plane ([d x n]
//! at woff). xq/xs is the row-major pre-quantized activation image in the plane's own quant form
//! (q8_0 per-32 for q8, Q8_K per-256 for kq); per-16 block sums are recomputed in-kernel.
def public matmul_gpu_dense(var y : array<float>; woff : int64; xq : array<int8>; xs : array<float>; n, d, nrows : int64; fmt : int) {
    unsafe {
        invoke(g_moe_gpu_dense, addr(y[0]), woff, addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, nrows, fmt)
    }
}

//! One recurrent layer's whole deltanet attention block on the GPU tier — see MoeGpuDnFn for
//! the contract. y gets npos x dim out-proj rows, state/tail update in place; beta/alpha arrive
//! RAW (transforms run in-kernel), conv taps in the fblob disk layout (cd x dconv).
def public matmul_moe_gpu_dn(var y : array<float>; woq, woz, woo : int64;
                             xq : array<int8>; xs : array<float>; b : array<float>; g : array<float>;
                             convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                             var stp : float?; var tail : array<float>; histp : float const?;
                             npos, dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    unsafe {
        invoke(g_moe_gpu_dn, addr(y[0]), woq, woz, woo,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), addr<float const?>(b[0]), addr<float const?>(g[0]),
            convw, wnormp, ap, dtp, stp, addr(tail[0]), histp,
            npos, dim, cd, kd, di, nvh, nkh, ds, dconv, eps)
    }
}

//! One full-attention layer's whole softmax attention block on the GPU tier — see MoeGpuAttnFn
//! for the contract. y gets npos x dim wo rows; k/v get npos x kv_dim roped-k / raw-v rows.
//! cos/sin are npos x rot/2 NEOX rope rows; rms weight rows may be null when qknorm is off.
def public matmul_moe_gpu_attn(var y : array<float>; var k : array<float>; var v : array<float>;
                               woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                               xq : array<int8>; xs : array<float>;
                               rmsqp : float const?; rmskp : float const?;
                               cosp : float const?; sinp : float const?;
                               npos, dim, qd, kv_dim, hs, rot, n_heads, kv_mul : int64;
                               scale : float; eps : float; gated : bool; qknorm : bool) {
    unsafe {
        invoke(g_moe_gpu_attn, addr(y[0]), addr(k[0]), addr(v[0]),
            woq, wok, wov, woo, fq, fk, fv, fo,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            rmsqp, rmskp, cosp, sinp,
            npos, dim, qd, kv_dim, hs, rot, n_heads, kv_mul, scale, eps, gated, qknorm)
    }
}

//! Roll back the last n weight-stack uploads (a partial layer triple the VRAM budget cut
//! mid-way) — a dangling plane would hijack the residency check when its layer re-enters as
//! streamed or CPU. stream selects the pinned-mirror registry instead of the resident one.
def public moe_gpu_drop_stacks(n : int; stream : bool) {
    invoke(g_moe_gpu_drop, n, stream)
}

//! Classifier GEMV on the GPU tier — y[d] = W·x over the resident plane at woff (one region
//! covering the whole vocab). xq/xs is the activation vector in the plane's own quant form (q8_0
//! per-32 for q8, Q8_K per-256 for kq — the tier resolves which from the uploaded stack format).
def public matmul_moe_gpu_cls(var y : array<float>; woff : int64; xq : array<int8>; xs : array<float>; n, d : int64; fmt : int) {
    unsafe {
        invoke(g_moe_gpu_cls, addr(y[0]), woff, addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, fmt)
    }
}

