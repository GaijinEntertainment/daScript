options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_gpu_tier shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_boot_restore
require dasllama/dasllama_env      // env-driven route defaults + noisy gate
require dasllama/dasllama_config   // DlimVulkanConfig — the dry-bake seam's config argument
require daslib/fio                 // describe_pending_dynamic_modules — the absent-backend diagnostic
require daslib/algorithm
require math
require dasllama/dasllama_gpu_math public   // sel_lane_val: the lane floor the landing's readers share with the kernels

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
// The f16-fed twin of MoeGpuFfnFn: the window's f32 activation rows (npos x n, one per position)
// in place of the gathered quant image - the tier gathers, converts and combines device-side
typedef MoeGpuFfnXfFn = function<(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n : int64; nfe : int64; nrows : int64; npos : int64; f1 : int; f3 : int; f2 : int; is_gelu : bool) : void>
typedef MoeGpuFfnXfOkFn = function<(npos : int64; n : int64; f1 : int; f3 : int; f2 : int) : bool>
//! One recurrent layer's deltanet block as ONE device chain: qkv + z GEMMs in `fq` / `fz` over one image in their class (Q8_K
//! for a superblock format, else Q8_0), conv + silu + q/k L2-norm, the delta-rule scan, gated out-norm, out projection in `fo`.
//! histp feeds conv history channel-major; zeros = dn_reset's fresh state.
typedef MoeGpuDnFn = function<(var yp : float?; woq : int64; woz : int64; woo : int64; fq : int; fz : int; fo : int;
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
                                 bqp : float const?; bkp : float const?; bvp : float const?;
                                 cosp : float const?; sinp : float const?;
                                 npos : int64; dim : int64; qd : int64; kv_dim : int64;
                                 hs : int64; rot : int64; n_heads : int64;
                                 scale : float; eps : float; gated : bool; qknorm : bool) : void>

[unused_argument(goutp, offs1, offs3, offs2, nregions, xqp, xsp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)]
def private moe_gpu_unset_ffn(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    panic("dasLLAMA: MoE GPU FFN dispatch hit without an installed GPU tier")
}

[unused_argument(wq, ws, woff, n, rows, fmt, stream)]
def private moe_gpu_unset_upload(wq : uint8 const?; ws : uint8 const?; woff : int64; n : int64; rows : int64; fmt : int; stream : bool) : bool =>
    false

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

[unused_argument(yp, woq, woz, woo, fq, fz, fo, xqp, xsp, bp, gp, convw, wnormp, ap, dtp, stp, tailp, histp, npos, dim, cd, kd, di, nvh, nkh, ds, dconv, eps)]
def private moe_gpu_unset_dn(var yp : float?; woq, woz, woo : int64; fq, fz, fo : int;
                             xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
                             convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                             var stp : float?; var tailp : float?; histp : float const?;
                             npos, dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    panic("dasLLAMA: GPU deltanet dispatch hit without an installed GPU tier")
}

[unused_argument(yp, kp, vp, woq, wok, wov, woo, fq, fk, fv, fo, xqp, xsp, rmsqp, rmskp, bqp, bkp, bvp, cosp, sinp, npos, dim, qd, kv_dim, hs, rot, n_heads, scale, eps, gated, qknorm)]
def private moe_gpu_unset_attn(var yp : float?; var kp : float?; var vp : float?;
                               woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                               xqp : int8 const?; xsp : float const?;
                               rmsqp : float const?; rmskp : float const?;
                               bqp : float const?; bkp : float const?; bvp : float const?;
                               cosp : float const?; sinp : float const?;
                               npos, dim, qd, kv_dim, hs, rot, n_heads : int64;
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
var g_moe_gpu_cls_route = true         // runtime routing gate over the resident classifier (--cls-ab lever)
var g_moe_gpu_dense_route = true       // runtime routing gate over the marks (in-process A/B lever)
var g_moe_gpu_dn_route = true          // runtime routing gate over the deltanet marks (--dn-ab lever)
var g_moe_gpu_attn_route = true        // runtime routing gate over the attention marks (--attn-ab lever)

// ===== the resident whole-stack decode driver hooks =====
// The tier registers these; common reaches the driver through them, so the engine never names a GPU
// symbol. reserve/place fill the arenas; prepare/norms/set_layer/set_cls build it; token runs one forward.
typedef RdecReserveFn = function<(fmt : int; cap_blocks : int64) : bool>
typedef RdecPlaceFn = function<(wq : array<uint8>; ws : array<uint8>; n : int64; rows : int64; fmt : int) : int64>
//! The deltanet geometry a hybrid model hands the resident driver at prepare; `cd == 0` = no
//! recurrent layers. `ba_f32`: the beta/alpha rows are f32 (uploaded whole through
//! `rdec_upload_dn_ba`, read as f32) rather than arena planes in their tags' formats.
struct RdecDnGeom {
    cd, kd, di, nvh, nkh, ds, dconv : int64
    ba_f32 : bool
    zsig : bool    //! the out-gate is sigmoid(z) (qwen4exp) instead of silu(z): the step and scan kernels' `ZSIG` stamps serve
}
let public RDEC_NROW_MAX = 16l   //! the rows the N-row token command steps at once, the per-row planes' height: two of the N-column GEMV leaves' eight-column tiles, the depth-8 verify's nine rows and the short window's peel
let public RDEC_CODEC_BYTE_SPAN = 4_294_967_296l   //! one side of a block-codec K/V mirror, at most: its kernels address a block by a 32-bit byte offset
let public RDEC_EXPERTS_MAX = 512l   //! the experts a routed layer's select and bucket schedule reach on the device: a workgroup of 256 threads two logits deep, or of 512
let public RDEC_ROUTED_SLOTS = 128l   //! the routed slots a decode step stages: every row's k picks fit in them (the depth-8 verify's nine rows at eight picks), and the hot expert pool takes no more picks a step
let public RDEC_NORM_ROWS = 10l   //! rms_att, rms_ffn, post-attention, post-FFN, the per-layer-embedding post norm, then the gemma-4 MoE layer's router scale, routed pre norm, dense post norm and routed post norm, then the attention output bias row (gpt-oss) - zero rows where a model lacks them
//! the per-layer row indices in the norms plane; a zero row where a model lacks the row
let public RDEC_NORM_ATT = 0l
let public RDEC_NORM_FFN = 1l
let public RDEC_NORM_POST_ATT = 2l
let public RDEC_NORM_POST_FFN = 3l
let public RDEC_NORM_PLE_POST = 4l   //! the per-layer-embedding branch's post norm (gemma-4 E-series)
let public RDEC_NORM_ROUTER = 5l     //! gemma-4: the router's input scale over sqrt(dim)
let public RDEC_NORM_PRE_FFN2 = 6l   //! gemma-4: the routed pre norm
let public RDEC_NORM_POST_FFN1 = 7l  //! gemma-4: the dense branch's post norm
let public RDEC_NORM_POST_FFN2 = 8l  //! gemma-4: the routed sum's post norm
let public RDEC_NORM_OBIAS = 9l      //! the attention output bias row (gpt-oss), added to the wo GEMV's row at the residual step
let public RDEC_HEAD_NORM_ROWS = 5l   //! the NextN draft head's block: its attention norm, its FFN norm, the embed norm, the carry norm, the head norm (the final norm's row where the file ships none)
let public RDEC_HEAD_ATT = 0l
let public RDEC_HEAD_FFN = 1l
let public RDEC_HEAD_ENORM = 2l
let public RDEC_HEAD_HNORM = 3l
let public RDEC_HEAD_OUT = 4l

//! `qk_hs` = 0 on a model without q/k norms
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head")]
def public rdec_head_norm_base(n_layers, dim, qk_hs : int64) : int64 => rdec_norms_qk_base(n_layers, dim) + (n_layers + 1l) * 2l * qk_hs

//! the norms plane's final-norm row, past every layer's `RDEC_NORM_ROWS` rows
def public rdec_norms_final_base(n_layers, dim : int64) : int64 => n_layers * RDEC_NORM_ROWS * dim

//! layer `l`'s norm row of `kind` (`RDEC_NORM_*`) in the norms plane, in floats
def public rdec_norm_off(l, kind, dim : int64) : int64 => (l * RDEC_NORM_ROWS + kind) * dim

let public RDEC_AR_MAX_DIM = 8_192l   //! the residual step's row stash: 32 KB of the 48 KiB workgroup memory the tier requires of a device
let public RDEC_MIR_SLACK_ROWS = 16l   //! the K/V rows a flash fragment may read past a window's end
let public RDEC_MIR_SLACK = RDEC_MIR_SLACK_ROWS * RDEC_AR_MAX_DIM   //! slack elements past each K/V mirror's last plane and past a block codec's shadow row

//! a layer's K/V mirror base in region `region`: the layer's base and the region's stride of `mir_units`
def public rdec_mir_region_base(layer_base, region, mir_units : int64) : int64 => layer_base + region * mir_units

//! a tier entry's arm line under the noisy gate, said once an outcome: `said` holds the last outcome logged (-1 = none)
def public arm_say(var said : int&; outcome : int; msg : string) {
    return if (!dasllama_noisy() || said == outcome)
    said = outcome
    to_log(LOG_INFO, "dasLLAMA noisy: {msg}\n")
}

//! the norms plane's q/k rows, past the final-norm row
def public rdec_norms_qk_base(n_layers, dim : int64) : int64 => rdec_norms_final_base(n_layers, dim) + dim

//! the norms plane's floats before the eps/ascale pair: the layers' rows, the final norm and the q/k rows, then under `head` the head's q/k slot and block
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head")]
def public rdec_norms_len(n_layers, dim, qk_hs : int64; head : bool) : int64
    => head ? rdec_head_norm_base(n_layers, dim, qk_hs) + RDEC_HEAD_NORM_ROWS * dim : rdec_norms_qk_base(n_layers, dim) + n_layers * 2l * qk_hs

//! One attention layer's geometry for the resident driver - its attention class (gemma-4's sliding and
//! global layers differ in head size, kv width and rope), its dense FFN width, and where its rows sit.
struct RdecLayerGeom {
    hs, qd, kvd, hid, rot : int64
    window : int64                  //! positions a sliding layer attends over; 0 = every position
    ring : int64                    //! a sliding layer's ring rows in the mirror (`kv_ring_row`); 0 = its rows at every position
    mir_base : int64                //! its first unit in its K/V mirror slab (one region's layout; the region stride is the slab's)
    mir_slab : int64                //! the K/V mirror slab its rows sit in (a contiguous run of attention layers under one binding's range)
    cbase : int64                   //! its rope class's row inside a position's cos/sin plane
    boff : int64                    //! its row base in the q/k/v bias plane
    pre_post : bool                 //! the residual steps norm their add partner first (a gemma's post-attention and post-FFN norms)
    gelu : bool                     //! the dense FFN's activation is the tanh gelu, else silu
    vfk : bool                      //! the layer has no v plane: V is the raw k row scaled by its own rms (gemma-4's global layers)
    vnorm : bool                    //! V is scaled by its own rms, no weight (gemma-4's v-norm on a layer with its own v plane)
    out_scale : float               //! the layer's output scale on its FFN residual step; 1.0 = none
    kv_shared : bool                //! the layer projects Q only and attends over a donor layer's K/V rows (`mir_base` is the donor's; gemma-4 E-series)
    attn_cap : float                //! the attention logit softcap on the scaled scores (gemma-2: 50); 0 = none
    moe_g4 : bool                   //! the gemma-4 MoE layer: a parallel dense shared expert, the routed branch and the router each on their own norm of x
}
//! qd / kv_dim / head_size / hidden / rot are the layers' maxima; `mir_units` one K/V mirror slab's units a region (elements, or 32-element blocks under a block codec; the widest slab's, every slab the same stride), `n_slabs` the slabs a side, `cos_elems` a position's cos/sin
//! plane; `carry`: every step lands its rows' post-final-norm hidden (`rdec_land_carry`); `head`: the NextN draft head rides the driver,
//! its norm rows in the plane and its slot in `mir_units`; `verify_rows`: one stream's verify rows the per-row planes hold too
typedef RdecPrepareFn = function<(n_layers : int64; n_attn : int64; dim : int64; qd : int64; kv_dim : int64; head_size : int64; n_heads : int64; hidden : int64; vocab : int64; seq_cap : int64; neox : bool; eps : float; scale : float; qk_norm : bool; kv_dtype : int; q_gated : bool; rot : int64; dn : RdecDnGeom; mir_units : int64; n_slabs : int64; cos_elems : int64; regions : int64; carry : bool; head : bool; verify_rows : int64) : bool>
//! `obias`: the plane's `RDEC_NORM_ROWS`-th row a layer is the attention output bias the residual step adds (gpt-oss); zero rows otherwise
typedef RdecNormsFn = function<(norms : array<float>; obias : bool) : void>
typedef RdecSetLayerFn = function<(l : int64; mir_idx : int64; bq : int64; bk : int64; bv : int64; bo : int64; b1 : int64; b3 : int64; b2 : int64; fq : int; fk : int; fv : int; fo : int; f1 : int; f3 : int; f2 : int; g : RdecLayerGeom) : void>
typedef RdecSetDnLayerFn = function<(l : int64; bqkv : int64; bz : int64; bout : int64; bbeta : int64; balpha : int64; b1 : int64; b3 : int64; b2 : int64; ba_off : int64; fqkv : int; fz : int; fout : int; fbeta : int; falpha : int; f1 : int; f3 : int; f2 : int; convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?) : void>
typedef RdecUploadDnBaFn = function<(rows : array<float>) : void>
typedef RdecUploadBiasFn = function<(rows : array<float>) : void>
typedef RdecDnOwnFn = function<(l : int64; stp : float?; histp : float?) : void>
typedef RdecDnPrefillOwnFn = function<(l : int64; stp : float?; histp : float?) : void>
//! the classifier plane, and the epilogue its logits take: the final softcap (0 = none) and the checkpoint's suppressed token ids
typedef RdecSetClsFn = function<(cls_block : int64; cls_fmt : int; cap : float; supp : array<int64>) : void>
//! `ple_row` / `ple_rows`: the per-layer-embedding side input the CPU pre-step built ([layers x ple] a token; position-major over a prompt); empty on a model without one
//! `pick_only`: land the row's argmax id alone (the answer) and leave `logits` unwritten; false lands the logits and answers -1;
//! `token` >= 0: the row's token id, which a driver that holds the token table gathers on the device (`x` then unread); -1 = `x` is the row
typedef RdecTokenFn = function<(x : array<float>; cossin : array<float>; ple_row : array<float>; token : int64; pos : int64; cnt : int64; pick_only : bool; var logits : array<float>) : int64>
//! `ple_raw`: the rows are the gather half alone (sqrt(ple)-scaled table rows) and the driver finishes the pre-step on device; false = the finished side input;
//! `experts` the host's routed sums of a plain MoE's host layers, asked at each one's cut of the window (unreached on a model without them)
typedef RdecPrefillFn = function<(x_batch : array<float>; cos_batch : array<float>; ple_rows : array<float>; ple_raw : bool; npos : int64; pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) : void>
//! layer `l`'s K/V rows [row0, row0 + nrows) of the selected region, `l == n_layers` the NextN head: `kp` / `vp` the host cache from
//! row 0 - the sync takes the rows up from it, the read brings them back into it
typedef RdecKvSyncFn = function<(l : int64; row0 : int64; nrows : int64; kp : uint8 const?; vp : uint8 const?) : void>
typedef RdecKvReadFn = function<(l : int64; row0 : int64; nrows : int64; var kp : uint8?; var vp : uint8?) : void>

[unused_argument(fmt, cap_blocks)]
def private rdec_unset_reserve(fmt : int; cap_blocks : int64) : bool => false
[unused_argument(wq, ws, n, rows, fmt)]
def private rdec_unset_place(wq : array<uint8>; ws : array<uint8>; n, rows : int64; fmt : int) : int64 => -1l
[unused_argument(n_layers, n_attn, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap, neox, eps, scale, qk_norm, kv_dtype, q_gated, rot, dn, mir_units, n_slabs, cos_elems, regions, carry, head, verify_rows)]
def private rdec_unset_prepare(n_layers, n_attn, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64; neox : bool; eps, scale : float; qk_norm : bool; kv_dtype : int; q_gated : bool; rot : int64; dn : RdecDnGeom; mir_units, n_slabs, cos_elems, regions : int64; carry, head : bool; verify_rows : int64) : bool => false
[unused_argument(norms, obias)]
def private rdec_unset_norms(norms : array<float>; obias : bool) {}
[unused_argument(l, mir_idx, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2, g)]
def private rdec_unset_set_layer(l, mir_idx, bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int; g : RdecLayerGeom) {}
[unused_argument(l, bqkv, bz, bout, bbeta, balpha, b1, b3, b2, ba_off, fqkv, fz, fout, fbeta, falpha, f1, f3, f2, convw, wnormp, ap, dtp)]
def private rdec_unset_set_dn_layer(l, bqkv, bz, bout, bbeta, balpha, b1, b3, b2, ba_off : int64; fqkv, fz, fout, fbeta, falpha, f1, f3, f2 : int; convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?) {}
[unused_argument(rows)]
def private rdec_unset_upload_dn_ba(rows : array<float>) {}
[unused_argument(l, stp, histp)]
def private rdec_unset_dn_own(l : int64; stp : float?; histp : float?) {}
[unused_argument(l, stp, histp)]
def private rdec_unset_dn_prefill_own(l : int64; stp : float?; histp : float?) {}
[unused_argument(cls_block, cls_fmt, cap, supp)]
def private rdec_unset_set_cls(cls_block : int64; cls_fmt : int; cap : float; supp : array<int64>) {}
[unused_argument(x, cossin, ple_row, token, pos, cnt, pick_only, logits)]
def private rdec_unset_token(x : array<float>; cossin : array<float>; ple_row : array<float>; token, pos, cnt : int64; pick_only : bool; var logits : array<float>) : int64 {
    panic("dasLLAMA: resident decode token hit without an installed GPU driver")
    return -1l
}
[unused_argument(x_batch, cos_batch, ple_rows, ple_raw, npos, pos0, logits, experts)]
def private rdec_unset_prefill(x_batch : array<float>; cos_batch : array<float>; ple_rows : array<float>; ple_raw : bool; npos : int64; pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    panic("dasLLAMA: resident prefill hit without an installed GPU driver")
}
[unused_argument(l, row0, nrows, kp, vp)]
def private rdec_unset_kv_sync(l, row0, nrows : int64; kp : uint8 const?; vp : uint8 const?) {
    panic("dasLLAMA: resident K/V row sync hit without an installed GPU driver")
}
[unused_argument(l, row0, nrows, kp, vp)]
def private rdec_unset_kv_read(l, row0, nrows : int64; var kp : uint8?; var vp : uint8?) {
    panic("dasLLAMA: resident K/V row readback hit without an installed GPU driver")
}

var g_rdec_reserve = @@rdec_unset_reserve
var g_rdec_place = @@rdec_unset_place
var g_rdec_prepare = @@rdec_unset_prepare
var g_rdec_norms = @@rdec_unset_norms
var g_rdec_set_layer = @@rdec_unset_set_layer
var g_rdec_set_cls = @@rdec_unset_set_cls
var g_rdec_token = @@rdec_unset_token
var g_rdec_prefill = @@rdec_unset_prefill
var g_rdec_kv_sync = @@rdec_unset_kv_sync
var g_rdec_kv_read = @@rdec_unset_kv_read
var g_rdec_installed = false
var g_rdec_set_dn_layer = @@rdec_unset_set_dn_layer
var g_rdec_upload_dn_ba = @@rdec_unset_upload_dn_ba
var g_rdec_dn_own = @@rdec_unset_dn_own
var g_rdec_dn_prefill_own = @@rdec_unset_dn_prefill_own
var g_rdec_dn_installed = false
var g_rdec_site_feeds = false

//! Declare that the installed home feeds every site in its plane's own activation form (the CUDA chain); off, a layer whose q/k/v, gate/up or deltanet qkv/z planes mix forms declines
def public install_moe_gpu_resident_site_feeds(on : bool) {
    g_rdec_site_feeds = on
}

def public rdec_can_mix_site_forms() : bool => g_rdec_site_feeds

var private g_rdec_kv_ring = true

//! Declare whether the installed home rings a sliding layer's mirror rows; off, the plan seats every position's row whatever `DASLLAMA_GPU_KV_RING` asks
def public install_moe_gpu_resident_kv_ring(on : bool) {
    g_rdec_kv_ring = on
}

def public rdec_can_ring_kv() : bool => g_rdec_kv_ring

var private g_rdec_kv_share = true

//! Declare whether the installed home serves a shared-KV layer (Q alone projected, the donor's mirror rows read); off, the plan declines such a model by the layer
def public install_moe_gpu_resident_kv_share(on : bool) {
    g_rdec_kv_share = on
}

def public rdec_can_share_kv() : bool => g_rdec_kv_share

[unused_argument(rows)]
def private rdec_unset_upload_bias(rows : array<float>) {
    panic("dasLLAMA: resident q/k/v bias upload hit without an installed bias seat")
}
var g_rdec_upload_bias = @@rdec_unset_upload_bias
var g_rdec_bias_installed = false

//! Install the resident driver's q/k/v projection-bias seat (qwen2) - separately, so a tier without
//! it declines a biased model by name instead of serving it unbiased.
def public install_moe_gpu_resident_bias(upload_bias : RdecUploadBiasFn) {
    g_rdec_upload_bias = upload_bias
    g_rdec_bias_installed = true
}

def public rdec_bias_installed() : bool => g_rdec_bias_installed
def public rdec_upload_bias(rows : array<float>) { invoke(g_rdec_upload_bias, rows) }

//! The per-head attention sink logits (gpt-oss), every layer's [n_heads] row in layer order; false = the device has no arm for them
typedef RdecUploadSinksFn = function<(rows : array<float>) : bool>

[unused_argument(rows)]
def private rdec_unset_upload_sinks(rows : array<float>) : bool => false
var g_rdec_upload_sinks = @@rdec_unset_upload_sinks
var g_rdec_sinks_installed = false

//! Install the resident driver's attention-sink seat (gpt-oss) - separately, so a tier without it declines a sink model by name.
def public install_moe_gpu_resident_sinks(upload_sinks : RdecUploadSinksFn) {
    g_rdec_upload_sinks = upload_sinks
    g_rdec_sinks_installed = true
}

def public rdec_sinks_installed() : bool => g_rdec_sinks_installed
def public rdec_upload_sinks(rows : array<float>) : bool => invoke(g_rdec_upload_sinks, rows)

// ===== the resident driver's MoE seats =====
//! Does the resident MoE block admit an expert triple of these formats - the tile family it
//! rides is the device's (the f16-fed cm2 tiles today).
typedef RdecMoeOkFn = function<(f1 : int; f3 : int; f2 : int) : bool>
//! The MoE geometry and the router plane (every MoE layer's [ne x dim] rows, then its gate row when the shared expert is gated) after the prepare; false = the class rails declined.
//! `dscale`: every MoE layer's per-expert down scales [n_moe x ne] the top-k folds into its weights (gemma-4); `act_oai`: the experts' activation is the clamped swiglu (gpt-oss).
//! `rbias`: every MoE layer's router bias [n_moe x ne]; `gbias` / `ubias` / `dbias`: the expert stacks' bias rows ([n_moe x ne x nfe] twice, [n_moe x ne x dim]) - each plane empty on a model without it.
typedef RdecPrepareMoeFn = function<(ne : int64; k : int64; nfe : int64; nsh : int64; n_moe : int64; sh_gated : bool; norm_topk : bool; is_gelu : bool; act_oai : bool; wscale : float; router : array<float>; dscale : array<float>; rbias : array<float>; gbias : array<float>; ubias : array<float>; dbias : array<float>) : bool>
//! Register an MoE attention layer: the attention quad, the dense triple (the shared expert's, or -1 blocks for none) and the expert triple.
typedef RdecSetMoeLayerFn = function<(l : int64; mir_idx : int64; router_slot : int64; bq : int64; bk : int64; bv : int64; bo : int64; b1 : int64; b3 : int64; b2 : int64; be1 : int64; be3 : int64; be2 : int64; fq : int; fk : int; fv : int; fo : int; f1 : int; f3 : int; f2 : int; fe1 : int; fe3 : int; fe2 : int; g : RdecLayerGeom) : void>
//! Register the routed block on a layer already registered by its own seat (a recurrent MoE layer after its deltanet registration): the expert triple and the layer's router-plane slot.
typedef RdecSetMoeExpertsFn = function<(l : int64; router_slot : int64; be1 : int64; be3 : int64; be2 : int64; fe1 : int; fe3 : int; fe2 : int) : void>

[unused_argument(f1, f3, f2)]
def private rdec_unset_moe_ok(f1, f3, f2 : int) : bool => false
[unused_argument(ne, k, nfe, nsh, n_moe, sh_gated, norm_topk, is_gelu, act_oai, wscale, router, dscale, rbias, gbias, ubias, dbias)]
def private rdec_unset_prepare_moe(ne, k, nfe, nsh, n_moe : int64; sh_gated, norm_topk, is_gelu, act_oai : bool; wscale : float; router : array<float>; dscale : array<float>;
                                   rbias : array<float>; gbias : array<float>; ubias : array<float>; dbias : array<float>) : bool => false
[unused_argument(l, mir_idx, router_slot, bq, bk, bv, bo, b1, b3, b2, be1, be3, be2, fq, fk, fv, fo, f1, f3, f2, fe1, fe3, fe2, g)]
def private rdec_unset_set_moe_layer(l, mir_idx, router_slot, bq, bk, bv, bo, b1, b3, b2, be1, be3, be2 : int64; fq, fk, fv, fo, f1, f3, f2, fe1, fe3, fe2 : int; g : RdecLayerGeom) {}
[unused_argument(l, router_slot, be1, be3, be2, fe1, fe3, fe2)]
def private rdec_unset_set_moe_experts(l, router_slot, be1, be3, be2 : int64; fe1, fe3, fe2 : int) {}

var g_rdec_moe_ok = @@rdec_unset_moe_ok
var g_rdec_prepare_moe = @@rdec_unset_prepare_moe
var g_rdec_set_moe_layer = @@rdec_unset_set_moe_layer
var g_rdec_set_moe_experts = @@rdec_unset_set_moe_experts
var g_rdec_moe_installed = false

//! Install the resident driver's MoE seats - separately, so a tier without them declines a MoE by name.
def public install_moe_gpu_resident_moe(moe_ok : RdecMoeOkFn; prepare_moe : RdecPrepareMoeFn; set_moe_layer : RdecSetMoeLayerFn; set_moe_experts : RdecSetMoeExpertsFn) {
    g_rdec_moe_ok = moe_ok
    g_rdec_prepare_moe = prepare_moe
    g_rdec_set_moe_layer = set_moe_layer
    g_rdec_set_moe_experts = set_moe_experts
    g_rdec_moe_installed = true
}

def public rdec_moe_installed() : bool => g_rdec_moe_installed
def public rdec_moe_ok(f1, f3, f2 : int) : bool => invoke(g_rdec_moe_ok, f1, f3, f2)
def public rdec_prepare_moe(ne, k, nfe, nsh, n_moe : int64; sh_gated, norm_topk, is_gelu, act_oai : bool; wscale : float; router : array<float>; dscale : array<float>;
                            rbias : array<float>; gbias : array<float>; ubias : array<float>; dbias : array<float>) : bool
    => invoke(g_rdec_prepare_moe, ne, k, nfe, nsh, n_moe, sh_gated, norm_topk, is_gelu, act_oai, wscale, router, dscale, rbias, gbias, ubias, dbias)
def public rdec_set_moe_layer(l, mir_idx, router_slot, bq, bk, bv, bo, b1, b3, b2, be1, be3, be2 : int64; fq, fk, fv, fo, f1, f3, f2, fe1, fe3, fe2 : int; g : RdecLayerGeom) { invoke(g_rdec_set_moe_layer, l, mir_idx, router_slot, bq, bk, bv, bo, b1, b3, b2, be1, be3, be2, fq, fk, fv, fo, f1, f3, f2, fe1, fe3, fe2, g) }
def public rdec_set_moe_experts(l, router_slot, be1, be3, be2 : int64; fe1, fe3, fe2 : int) { invoke(g_rdec_set_moe_experts, l, router_slot, be1, be3, be2, fe1, fe3, fe2) }

//! The mixer sites' planes after the layers register (`hc` streams, low-rank `lr`, `n_sites` = 2 x the routed layers, a NextN head on the chain among them): the norm rows
//! [sites x hc x dim] and inject rows [sites x hc x hc_dim] as f32, the q8 down/up arena blocks a site, the head mixer's; false = the rails declined
typedef RdecSetHcFn = function<(hc : int64; lr : int64; n_sites : int64; norm_rows : array<float>; inject_rows : array<float>; down_blks : array<int64>; up_blks : array<int64>; head_norm : array<float>; head_down : int64; head_up : int64;
                                 mtp_head_norm : array<float>; mtp_head_down : int64; mtp_head_up : int64; hnorm_rows : array<float>) : bool>
//! The n-gram side input before layer `l`'s attention mixer: the key [hc_dim x ind] and value [dim x ind] q8 planes' blocks, the
//! conv ring's rows, taps and dilation, the three norm rows [3 x hc x dim] (key, query, conv input) and the taps [hc_dim x kern].
typedef RdecSetNgramFn = function<(l : int64; ind : int64; nrows : int64; kern : int64; dil : int64; bkey : int64; bval : int64; norm_rows : array<float>; conv_taps : array<float>) : bool>
//! Some routed layers' experts serve on the host (every one on a hyper-connection model, the first in layer order on a plain MoE): the token
//! command splits at each such block, landing the mixed row and the router logits and taking the routed sum back (`RdecTokenSplitFn`);
//! set after the prepare, before the MoE prepare and the layers' registration.
typedef RdecSetHostExpertsFn = function<(on : bool) : void>
//! The host's routed sum for layer `l`: the FFN-mixed row `xb` [dim] and the router logits [ner] in, the weighted expert sum into `acc` [dim].
typedef RdecHostExpertsFn = function<(l : int64; xb : float const?; logits : float const?; var acc : float?) : void>
//! The split token: as `RdecTokenFn`, with `side` the n-gram side input's gathered row where the model has one, and the host's
//! routed sum called between the command's segments.
typedef RdecTokenSplitFn = function<(x : array<float>; cossin : array<float>; side : array<float>; pos : int64; cnt : int64; pick_only : bool; var logits : array<float>; experts : RdecHostExpertsFn) : int64>
//! The n-gram conv ring of the selected region against the session's host ring (`rows x hc x dim` floats): up after a CPU prefill
//! or pass, down before the CPU rails read the session again.
typedef RdecNgramRingFn = function<(var host : float?; up : bool) : void>
//! The host's routed sums of layer `l`'s rows `row0 .. row0 + nrows` of a window: the FFN-mixed rows `xb` [rows x dim] and the
//! router logits [rows x ner] in, the weighted expert sums into `acc` [rows x dim] - the rows before `row0` untouched.
typedef RdecHostExpertsRowsFn = function<(l : int64; xb : float const?; logits : float const?; var acc : float?; row0 : int64; nrows : int64) : void>
//! The hyper-connection window chain: the embedded rows, the rope rows and the n-gram side rows [npos x ind] of a prompt from
//! `pos0`, the host's routed sums called between every window's segments, the last row's logits out.
typedef RdecPrefillHcFn = function<(x_batch : array<float>; cos_batch : array<float>; side : array<float>; npos : int64; pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) : void>

[unused_argument(x_batch, cos_batch, side, npos, pos0, logits, experts)]
def private rdec_unset_prefill_hc(x_batch : array<float>; cos_batch : array<float>; side : array<float>; npos, pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    panic("dasLLAMA: resident hyper-connection prefill hit without an installed GPU driver")
}

[unused_argument(host, up)]
def private rdec_unset_ngram_ring(var host : float?; up : bool) {
    panic("dasLLAMA: resident n-gram ring sync hit without an installed GPU driver")
}

[unused_argument(hc, lr, n_sites, norm_rows, inject_rows, down_blks, up_blks, head_norm, head_down, head_up, mtp_head_norm, mtp_head_down, mtp_head_up, hnorm_rows)]
def private rdec_unset_set_hc(hc, lr, n_sites : int64; norm_rows : array<float>; inject_rows : array<float>; down_blks, up_blks : array<int64>; head_norm : array<float>; head_down, head_up : int64;
                              mtp_head_norm : array<float>; mtp_head_down, mtp_head_up : int64; hnorm_rows : array<float>) : bool => false
[unused_argument(l, ind, nrows, kern, dil, bkey, bval, norm_rows, conv_taps)]
def private rdec_unset_set_ngram(l, ind, nrows, kern, dil, bkey, bval : int64; norm_rows : array<float>; conv_taps : array<float>) : bool => false
[unused_argument(on)]
def private rdec_unset_set_host_experts(on : bool) {}
[unused_argument(x, cossin, side, pos, cnt, pick_only, logits, experts)]
def private rdec_unset_token_split(x : array<float>; cossin : array<float>; side : array<float>; pos, cnt : int64; pick_only : bool; var logits : array<float>; experts : RdecHostExpertsFn) : int64 {
    panic("dasLLAMA: resident split token hit without an installed GPU driver")
    return -1l
}

var g_rdec_set_hc = @@rdec_unset_set_hc
var g_rdec_set_ngram = @@rdec_unset_set_ngram
var g_rdec_set_host_experts = @@rdec_unset_set_host_experts
var g_rdec_token_split = @@rdec_unset_token_split
var g_rdec_ngram_ring = @@rdec_unset_ngram_ring
var g_rdec_prefill_hc = @@rdec_unset_prefill_hc
var g_rdec_hc_installed = false

//! Install the resident driver's split seats - separately, so a tier without them declines a hyper-connection model, and a plain MoE's
//! host-layer form, by name.
def public install_moe_gpu_resident_hc(set_hc : RdecSetHcFn; set_ngram : RdecSetNgramFn; set_host_experts : RdecSetHostExpertsFn; token_split : RdecTokenSplitFn;
                                       ngram_ring : RdecNgramRingFn; prefill_hc : RdecPrefillHcFn) {
    g_rdec_set_hc = set_hc
    g_rdec_set_ngram = set_ngram
    g_rdec_set_host_experts = set_host_experts
    g_rdec_token_split = token_split
    g_rdec_ngram_ring = ngram_ring
    g_rdec_prefill_hc = prefill_hc
    g_rdec_hc_installed = true
}

def public rdec_ngram_ring(var host : float?; up : bool) { invoke(g_rdec_ngram_ring, host, up) }
def public rdec_prefill_hc(x_batch : array<float>; cos_batch : array<float>; side : array<float>; npos, pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    invoke(g_rdec_prefill_hc, x_batch, cos_batch, side, npos, pos0, logits, experts)
}

def public rdec_hc_installed() : bool => g_rdec_hc_installed
//! `mtp_head_*` the NextN head's own head mixer and `hnorm_rows` the carry's per-stream norm gammas where the head rides the chain (empty rows and -1 blocks otherwise)
def public rdec_set_hc(hc, lr, n_sites : int64; norm_rows : array<float>; inject_rows : array<float>; down_blks, up_blks : array<int64>; head_norm : array<float>; head_down, head_up : int64;
                       mtp_head_norm : array<float>; mtp_head_down, mtp_head_up : int64; hnorm_rows : array<float>) : bool
    => invoke(g_rdec_set_hc, hc, lr, n_sites, norm_rows, inject_rows, down_blks, up_blks, head_norm, head_down, head_up, mtp_head_norm, mtp_head_down, mtp_head_up, hnorm_rows)
def public rdec_set_ngram(l, ind, nrows, kern, dil, bkey, bval : int64; norm_rows : array<float>; conv_taps : array<float>) : bool
    => invoke(g_rdec_set_ngram, l, ind, nrows, kern, dil, bkey, bval, norm_rows, conv_taps)
def public rdec_set_host_experts(on : bool) { invoke(g_rdec_set_host_experts, on) }
def public rdec_token_split(x : array<float>; cossin : array<float>; side : array<float>; pos, cnt : int64; pick_only : bool; var logits : array<float>; experts : RdecHostExpertsFn) : int64
    => invoke(g_rdec_token_split, x, cossin, side, pos, cnt, pick_only, logits, experts)

//! Routed layer `l`'s hot expert pool: `slots` device slots of one expert's gate, up and down planes each, in the planes' formats.
//! Returns the offset the gate plane's slot 0 answers to in the tier's expert chains, the up and down planes a `stride` and two
//! past it, a slot an expert's elements wide; `base` under 0 = no pool on this layer.
typedef RdecHotLayerFn = function<(l : int64; slots : int64; f1 : int; f3 : int; f2 : int) : tuple<base : int64; stride : int64>>
//! One expert's planes into slot `slot` of layer `l`'s pool, in the gather's device layout: the gate, up and down quants and scales.
typedef RdecHotUploadFn = function<(l : int64; slot : int64; wq1 : array<uint8>; ws1 : array<uint8>; wq3 : array<uint8>; ws3 : array<uint8>; wq2 : array<uint8>; ws2 : array<uint8>) : void>
//! The hits of layer `l`'s routed step: `nhit` pool slots and their routing weights. The device runs their chain while the host
//! sums the misses, and the next segment adds the hit rows to the host's sum.
typedef RdecHotSubmitFn = function<(l : int64; nhit : int64; slots : int64 const?; w : float const?) : void>
//! `open` brackets a run of pool uploads the tier may land in one submit; `false` lands what the bracket holds.
typedef RdecHotBatchFn = function<(open : bool) : void>
//! the verify rows' hits of layer `l`: k slot and weight entries a row, a row's hits first, its misses padded at weight 0
typedef RdecHotSubmitRowsFn = function<(l : int64; nrows : int64; slots : int64 const?; w : float const?) : void>

//! a routed layer's hot expert pool as the hit chain's region records read it, the geometry both discrete homes' pool records
//! derive from: the three planes' formats, an expert's weight blocks a plane (a slot's stride in the records) and the feed's
//! blocks a row (the gate and up's over the model dim, the down's over the expert width)
struct HotLayerGeom {
    on : bool
    f1, f3, f2 : int
    blk1, blk3, blk2 : int64
    xnb1, xnb2 : int64
}

//! the hit chain's records and weights staged in the home's mapped blocks (`m1` / `m3` / `m2` two words a region, `wp` a float a
//! region): region j reads slot `slots[j]`'s blocks and its row's feed (k regions a row), the weights past `nreg` zero to `wlen` so
//! the combine skips them
def hot_stage(H : HotLayerGeom; var m1, m3, m2 : uint?; var wp : float?; k, nreg, wlen : int64; slots : int64 const?; w : float const?) {
    unsafe {
        for (j in range64(wlen)) {
            wp[j] = j < nreg ? w[j] : 0.0
        }
        for (j in range64(nreg)) {
            let xrow = uint((j / k) * H.xnb1)
            m1[j * 2l] = uint(slots[j] * H.blk1)
            m1[j * 2l + 1l] = xrow
            m3[j * 2l] = uint(slots[j] * H.blk3)
            m3[j * 2l + 1l] = xrow
            m2[j * 2l] = uint(slots[j] * H.blk2)
            m2[j * 2l + 1l] = uint(j * H.xnb2)
        }
    }
}

[unused_argument(l, slots, f1, f3, f2)]
def private rdec_unset_hot_layer(l, slots : int64; f1, f3, f2 : int) : tuple<base : int64; stride : int64> => (base = -1l, stride = 0l)
[unused_argument(l, slot, wq1, ws1, wq3, ws3, wq2, ws2)]
def private rdec_unset_hot_upload(l, slot : int64; wq1 : array<uint8>; ws1 : array<uint8>; wq3 : array<uint8>; ws3 : array<uint8>; wq2 : array<uint8>; ws2 : array<uint8>) {
    panic("dasLLAMA: resident hot expert upload hit without an installed GPU driver")
}
[unused_argument(l, nrows, slots, w)]
def private rdec_unset_hot_submit_rows(l, nrows : int64; slots : int64 const?; w : float const?) {
    panic("dasLLAMA: resident hot expert rows submit hit without an installed GPU driver")
}

[unused_argument(l, nhit, slots, w)]
def private rdec_unset_hot_submit(l, nhit : int64; slots : int64 const?; w : float const?) {
    panic("dasLLAMA: resident hot expert submit hit without an installed GPU driver")
}

var g_rdec_hot_layer = @@rdec_unset_hot_layer
var g_rdec_hot_upload = @@rdec_unset_hot_upload
var g_rdec_hot_submit = @@rdec_unset_hot_submit
var g_rdec_hot_submit_rows = @@rdec_unset_hot_submit_rows
var g_rdec_hot_installed = false

//! Install the resident driver's hot expert pool seats - separately, so a tier without them serves every routed expert on the host.
def public install_moe_gpu_resident_hot(hot_layer : RdecHotLayerFn; hot_upload : RdecHotUploadFn; hot_submit : RdecHotSubmitFn; hot_submit_rows : RdecHotSubmitRowsFn) {
    g_rdec_hot_submit_rows = hot_submit_rows
    g_rdec_hot_layer = hot_layer
    g_rdec_hot_upload = hot_upload
    g_rdec_hot_submit = hot_submit
    g_rdec_hot_installed = true
}

def public rdec_hot_installed() : bool => g_rdec_hot_installed

[unused_argument(open)]
def private rdec_unset_hot_batch(open : bool) {}   //! a tier without a batch lands every upload as it comes

var g_rdec_hot_batch = @@rdec_unset_hot_batch

//! Install the tier's upload batch for the pool's placement runs - separately, so a tier without one still serves the pool.
def public install_moe_gpu_resident_hot_batch(hot_batch : RdecHotBatchFn) {
    g_rdec_hot_batch = hot_batch
}

def public rdec_hot_batch(open : bool) {
    invoke(g_rdec_hot_batch, open)
}
def public rdec_hot_layer(l, slots : int64; f1, f3, f2 : int) : tuple<base : int64; stride : int64> => invoke(g_rdec_hot_layer, l, slots, f1, f3, f2)
def public rdec_hot_upload(l, slot : int64; wq1 : array<uint8>; ws1 : array<uint8>; wq3 : array<uint8>; ws3 : array<uint8>; wq2 : array<uint8>; ws2 : array<uint8>) {
    invoke(g_rdec_hot_upload, l, slot, wq1, ws1, wq3, ws3, wq2, ws2)
}
def public rdec_hot_submit(l, nhit : int64; slots : int64 const?; w : float const?) { invoke(g_rdec_hot_submit, l, nhit, slots, w) }

//! the DMA fill: expert `e` off the pinned stream stacks of the layer at `woff1` into slot `slot` of layer `l`'s pool, on the transfer
//! queue into the layer's open batch; false = the tier cannot (no transfer queue, a dry tier, the stacks do not hold it)
typedef RdecHotDmaFn = function<(l, slot, e, woff1 : int64; f1 : int) : bool>
//! layer l's open DMA batch submitted on the transfer queue; the layer's next hit chain waits its landing
typedef RdecHotDmaFlushFn = function<(l : int64) : void>

[unused_argument(l, slot, e, woff1, f1)]
def private rdec_unset_hot_dma(l, slot, e, woff1 : int64; f1 : int) : bool => false

[unused_argument(l)]
def private rdec_unset_hot_dma_flush(l : int64) {}

var private g_rdec_hot_dma = @@rdec_unset_hot_dma
var private g_rdec_hot_dma_flush = @@rdec_unset_hot_dma_flush

def public install_moe_gpu_resident_hot_dma(dma : RdecHotDmaFn; flush : RdecHotDmaFlushFn) {
    g_rdec_hot_dma = dma
    g_rdec_hot_dma_flush = flush
}

var private g_moe_gpu_streams = false   //! the tier streams expert stacks through pinned mirrors (`moe_gpu_upload_stack` under `stream`): the Vulkan tier's word

//! Install the tier's word that it streams expert stacks (the per-op tier's pinned mirrors): the resident driver's prompt stream arm reads it
def public set_moe_gpu_streams(on : bool) {
    g_moe_gpu_streams = on
}

def public moe_gpu_streams() : bool => g_moe_gpu_streams

def public rdec_hot_dma(l, slot, e, woff1 : int64; f1 : int) : bool => invoke(g_rdec_hot_dma, l, slot, e, woff1, f1)

def public rdec_hot_dma_flush(l : int64) => invoke(g_rdec_hot_dma_flush, l)
def public rdec_hot_submit_rows(l, nrows : int64; slots : int64 const?; w : float const?) { invoke(g_rdec_hot_submit_rows, l, nrows, slots, w) }

var private g_gpu_resident_route = true

//! The in-process twin of DASLLAMA_GPU_RESIDENT: a test pins the whole-model driver off before a
//! load to hold the per-op rails' claims on a model the driver would take.
def public set_gpu_resident_route(on : bool) {
    g_gpu_resident_route = on
}

var private g_vulkan_trim_set = false
var private g_vulkan_trim = false

//! the trimmed GPU lane in process (`DASLLAMA_TRIM`'s twin): the next discrete-home cold load mints its lane without the
//! planar families, and the lane's identity folds the answer, so a flip re-bakes the lane
def public set_vulkan_trim(on : bool) {
    g_vulkan_trim_set = true
    g_vulkan_trim = on
}

//! whether a vulkan-armed cold load mints the trimmed lane: the in-process lever where one was set, else the knob
def public vulkan_trim() : bool => g_vulkan_trim_set ? g_vulkan_trim : g_env_vulkan.trim

//! whether an in-process lever shadows `DASLLAMA_TRIM`: a fixture reads it beside `vulkan_trim()` and puts both back through `restore_vulkan_trim`
def public vulkan_trim_lever_set() : bool => g_vulkan_trim_set

//! the trim lever put back as a fixture found it: no lever where none was set, else the value it read
def public restore_vulkan_trim(was_set : bool; on : bool) {
    g_vulkan_trim_set = was_set
    g_vulkan_trim = on
}

//! The in-process lever's own value (`set_gpu_resident_route`), apart from the knob: a fixture restores what it found.
def public gpu_resident_route_lever() : bool => g_gpu_resident_route

//! The whole-model driver is wanted on this load: the knob and the in-process lever both on.
def public gpu_want_resident : bool => g_env_gpu.gpu_resident && g_gpu_resident_route

var private g_cuda_want_warned = false
var private g_gpu_home = ""   //! the discrete home whose entry fills the tier's seats ("vulkan" | "cuda"); "" until first read
var private g_gpu_home_installers : table<string; array<function<() : void>>>   //! a home's seat installers, by its name
var private g_gpu_home_shared_installers : array<function<() : void>>   //! the installers of seats every home holds alike, run on every switch

//! the environment's spelling of the home: "cuda" where DASLLAMA_CUDA asks for it in a build that carries the home, else "vulkan"
def private gpu_home_default : string {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return g_env_gpu.cuda ? "cuda" : "vulkan"
    } else {
        if (g_env_gpu.cuda && !g_cuda_want_warned) {
            g_cuda_want_warned = true
            to_log(LOG_WARNING, "dasLLAMA: DASLLAMA_CUDA asks for the CUDA home, which this build does not carry (DAS_CUDA_DISABLED, or a 32-bit host) - the Vulkan home keeps the GPU tier\n")
        }
        return "vulkan"
    }
}

//! the home whose seats the tier holds: the environment's spelling until `set_gpu_home_` moves it (the facade's `gpu_home`)
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def gpu_home_() : string {
    if (empty(g_gpu_home)) {
        g_gpu_home = gpu_home_default()
    }
    return g_gpu_home
}

//! the CUDA home takes the tier's seats in place of the Vulkan home (DASLLAMA_CUDA, or `set_gpu_home("cuda")`), read at both entries' [init]
def public gpu_want_cuda : bool => gpu_home_() == "cuda"

//! a home's entry, and every driver with seats of its own (the assistant drafter's), registers its seat installer under the
//! home's name at [init]; where the home is asked for, the registrant runs it there as well
def public register_gpu_home(name : string; install : function<() : void>) {
    g_gpu_home_installers[name] |> push(install)
}

//! a driver whose seats read the same under every home registers its installer here at [init] and runs it there itself:
//! `set_gpu_home_` resets every seat, then runs the shared installers before the named home's
def public register_gpu_home_shared(install : function<() : void>) {
    g_gpu_home_shared_installers |> push(install)
}

//! the shared installer taken back out (a test's, registered for one cell)
def public unregister_gpu_home_shared(install : function<() : void>) {
    let at = find_index(g_gpu_home_shared_installers, install)
    if (at >= 0) {
        g_gpu_home_shared_installers |> erase(at)
    }
}

//! the homes this build carries - the names `set_gpu_home_` takes (the facade's `gpu_homes`)
def gpu_homes_() : array<string> => [for (k in keys(g_gpu_home_installers)); k]

//! Move the tier onto the named home before it arms: every seat back to its declared default, the shared installers and
//! then the home's entry filling them. False, said, for a home this build does not carry or a tier already armed on
//! another home (the environment arms at [init] under DASLLAMA_GPU: spell the home there, DASLLAMA_CUDA, or arm from the program)
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def set_gpu_home_(name : string) : bool {
    return true if (name == gpu_home_())
    if (!key_exists(g_gpu_home_installers, name)) {
        to_log(LOG_WARNING, "dasLLAMA: set_gpu_home(\"{name}\") names a GPU home this build does not carry (it has {gpu_homes_()}) - the tier stays on {gpu_home_()}\n")
        return false
    }
    if (g_moe_gpu_installed) {
        to_log(LOG_WARNING, "dasLLAMA: set_gpu_home(\"{name}\") after the tier armed on {gpu_home_()} - a home switch runs before the first arm (the environment armed it at [init]: spell the home there, DASLLAMA_CUDA=1, or leave DASLLAMA_GPU unset and arm from the program)\n")
        return false
    }
    gpu_home_reset_fn_slots()
    gpu_home_reset_flags()
    g_gpu_home = name
    for (install in g_gpu_home_shared_installers) {
        invoke(install)
    }
    for (install in g_gpu_home_installers[name]) {
        invoke(install)
    }
    return true
}

//! Install the resident driver's hybrid (deltanet) seats - separately, so a tier without them
//! declines recurrent layers by name instead of serving a model it cannot step.
def public install_moe_gpu_resident_dn(set_dn_layer : RdecSetDnLayerFn; upload_dn_ba : RdecUploadDnBaFn; dn_own : RdecDnOwnFn; dn_prefill_own : RdecDnPrefillOwnFn) {
    g_rdec_set_dn_layer = set_dn_layer
    g_rdec_upload_dn_ba = upload_dn_ba
    g_rdec_dn_own = dn_own
    g_rdec_dn_prefill_own = dn_prefill_own
    g_rdec_dn_installed = true
}

def public rdec_dn_installed() : bool => g_rdec_dn_installed

typedef RdecSelectRegionFn = function<(region : int64) : void>

[unused_argument(region)]
def private rdec_unset_select_region(region : int64) {}

var g_rdec_select_region = @@rdec_unset_select_region
var private g_rdec_regions_installed = false

//! Install the resident driver's mirror-region seat - separately, so a tier without it serves one region, one session's history at a time.
def public install_moe_gpu_resident_regions(select_region : RdecSelectRegionFn) {
    g_rdec_select_region = select_region
    g_rdec_regions_installed = true
}

def public rdec_regions_installed() : bool => g_rdec_regions_installed

//! Every mirror address and the next token command resolve against mirror region `region`.
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def public rdec_select_region(region : int64) { invoke(g_rdec_select_region, region) }
def public rdec_set_dn_layer(l, bqkv, bz, bout, bbeta, balpha, b1, b3, b2, ba_off : int64; fqkv, fz, fout, fbeta, falpha, f1, f3, f2 : int; convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?) { invoke(g_rdec_set_dn_layer, l, bqkv, bz, bout, bbeta, balpha, b1, b3, b2, ba_off, fqkv, fz, fout, fbeta, falpha, f1, f3, f2, convw, wnormp, ap, dtp) }
def public rdec_upload_dn_ba(rows : array<float>) { invoke(g_rdec_upload_dn_ba, rows) }
def public rdec_dn_own(l : int64; stp : float?; histp : float?) { invoke(g_rdec_dn_own, l, stp, histp) }
//! After a resident prefill: layer `l`'s device-advanced deltanet state and conv history become the
//! session's (`stp`/`histp` name the owner) - the slot stays valid and dirty, so the first decode's
//! owner bind finds it in place and a host read syncs it down through `dn_flush_layer`.
def public rdec_dn_prefill_own(l : int64; stp : float?; histp : float?) { invoke(g_rdec_dn_prefill_own, l, stp, histp) }

//! Install the resident whole-stack decode driver (a GPU backend calls this at [init]).
def public install_moe_gpu_resident(reserve : RdecReserveFn; place : RdecPlaceFn; prepare : RdecPrepareFn;
                                    norms : RdecNormsFn; set_layer : RdecSetLayerFn; set_cls : RdecSetClsFn;
                                    token : RdecTokenFn; prefill : RdecPrefillFn;
                                    kv_sync : RdecKvSyncFn; kv_read : RdecKvReadFn) {
    g_rdec_reserve = reserve
    g_rdec_place = place
    g_rdec_prepare = prepare
    g_rdec_norms = norms
    g_rdec_set_layer = set_layer
    g_rdec_set_cls = set_cls
    g_rdec_token = token
    g_rdec_prefill = prefill
    g_rdec_kv_sync = kv_sync
    g_rdec_kv_read = kv_read
    g_rdec_installed = true
}

typedef RdecPrefillIdsFn = function<(tokens : array<int64>; emb_scale : float; cos_batch : array<float>; npos : int64; pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) : void>
//! the token table placed in the arena at `emb_block` in format `fmt` (q8 or a superblock format), its rows taking `escale`: the
//! ids prefill and the one-row token command then gather the rows on the device; false = the device has no gather for `fmt`
typedef RdecSetEmbFn = function<(emb_block : int64; fmt : int; escale : float) : bool>
//! an F32/F16/BF16 table's exact fp32 rows (`Model.emb_f32_exact`) uploaded whole as the gather's f32 plane, its rows taking `escale`
typedef RdecUploadEmbF32Fn = function<(fblob : array<float>; off : int64; vocab : int64; dim : int64; escale : float) : bool>

//! The f32 token table rides the residency plan's headroom only up to this size; past it the CPU embed serves.
let public RDEC_EMB_F32_CAP = 512l * 1024l * 1024l

//! The device bytes the f32 gather would upload for a (vocab x dim) table - 0 past its cap.
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]
def public rdec_emb_f32_bytes(vocab, dim : int64) : int64 {
    let bytes = vocab * dim * 4l
    return bytes > RDEC_EMB_F32_CAP ? 0l : bytes
}

//! Rows of read slack past a cm2 region's last row: the m column loads a partial column unclamped, so
//! every plane a tile reads (the residency plan's scratch, the prefill's gathered rows, a probe's) carries them.
let public TILE_READ_SLACK = 128l

[unused_argument(tokens, emb_scale, cos_batch, npos, pos0, logits, experts)]
def private rdec_unset_prefill_ids(tokens : array<int64>; emb_scale : float; cos_batch : array<float>; npos : int64; pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    panic("dasLLAMA: ids-form resident prefill hit without an installed GPU driver")
}
[unused_argument(emb_block, fmt, escale)]
def private rdec_unset_set_emb(emb_block : int64; fmt : int; escale : float) : bool => false
[unused_argument(fblob, off, vocab, dim, escale)]
def private rdec_unset_upload_emb_f32(fblob : array<float>; off, vocab, dim : int64; escale : float) : bool => false

var g_rdec_prefill_ids = @@rdec_unset_prefill_ids
var g_rdec_set_emb = @@rdec_unset_set_emb
var g_rdec_upload_emb_f32 = @@rdec_unset_upload_emb_f32
var g_rdec_prefill_ids_installed = false

[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def public install_rdec_prefill_ids(prefill_ids : RdecPrefillIdsFn; set_emb : RdecSetEmbFn; upload_emb_f32 : RdecUploadEmbF32Fn) {
    g_rdec_prefill_ids = prefill_ids
    g_rdec_set_emb = set_emb
    g_rdec_upload_emb_f32 = upload_emb_f32
    g_rdec_prefill_ids_installed = true
}

[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def public rdec_prefill_ids_installed() : bool => g_rdec_prefill_ids_installed
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def public rdec_prefill_ids(tokens : array<int64>; emb_scale : float; cos_batch : array<float>; npos : int64; pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    invoke(g_rdec_prefill_ids, tokens, emb_scale, cos_batch, npos, pos0, logits, experts)
}
//! One segment of a segmented resident prefill: `npos` rows at `row0` of the call's rows are one session's chunk from
//! absolute position `pos0`, its K/V into mirror `region`
struct public RdecPfSeg {
    row0 : int64
    npos : int64
    pos0 : int64
    region : int64
    seg : int64 = 0l   //! the call's segment this is, or the part of it inside a window, belongs to: the captures name their segment by it
    koff : int64 = 0l   //! the segment's first row in its region, past the segments packed before it (a scratch window's)
}

//! A checkpoint stop inside a resident prefill call: segment `seg`'s state after its first `row` rows lands in checkpoint row
//! `slot` of the segment's region (the state, the conv history as the ring image the row's parity reads, the n-gram ring)
struct public RdecPfCap {
    seg : int64
    row : int64
    slot : int64
}

typedef RdecPrefillCapsFn = function<(caps : array<RdecPfCap>) : void>

[unused_argument(caps)]
def private rdec_unset_prefill_caps(caps : array<RdecPfCap>) {
    panic("dasLLAMA: a resident prefill's checkpoint captures asked without an installed GPU driver")
}

var g_rdec_prefill_caps = @@rdec_unset_prefill_caps

//! Install the driver's capture list seat: the stops the next prefill call leaves checkpoints at inside its windows
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def public install_rdec_prefill_caps(caps_fn : RdecPrefillCapsFn) {
    g_rdec_prefill_caps = caps_fn
}

//! the captures the next resident prefill call (one session's or segmented) leaves; an empty list asks none
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def public rdec_prefill_caps(caps : array<RdecPfCap>) {
    invoke(g_rdec_prefill_caps, caps)
}

def public rdec_window_segs(segs : array<RdecPfSeg>; w0, wlen : int64; var out : array<RdecPfSeg>) {
    //! a part's `row0` is window-local, its `pos0` the segment's position at its first row, its `seg` the call's segment
    out |> clear()
    for (sg, i in segs, count()) {
        let lo = max(sg.row0, w0)
        let hi = min(sg.row0 + sg.npos, w0 + wlen)
        continue if (hi <= lo)
        out |> push(RdecPfSeg(row0 = lo - w0, npos = hi - lo, pos0 = sg.pos0 + (lo - sg.row0), region = sg.region, seg = int64(i), koff = sg.koff))
    }
}

//! the slot of the segment whose last row is window row `row` of the window at `w0`, else -1
def public rdec_seg_ending_at(segs : array<RdecPfSeg>; w0, row : int64) : int64 {
    for (sg, i in segs, count()) {
        return int64(i) if (sg.row0 + sg.npos - 1l == w0 + row)
    }
    return -1l
}

//! The segmented resident prefill: every segment's embedded rows (or ids, where the driver gathers) and rope rows back to
//! back in segment order, each segment's logits at its slot of `logits` (`vocab` floats a slot)
typedef RdecPrefillSegsFn = function<(x_batch : array<float>; tokens : array<int64>; use_ids : bool; emb_scale : float; cos_batch : array<float>; segs : array<RdecPfSeg>; var logits : array<float>; experts : RdecHostExpertsRowsFn) : void>

[unused_argument(x_batch, tokens, use_ids, emb_scale, cos_batch, segs, logits, experts)]
def private rdec_unset_prefill_segs(x_batch : array<float>; tokens : array<int64>; use_ids : bool; emb_scale : float; cos_batch : array<float>; segs : array<RdecPfSeg>; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    panic("dasLLAMA: segmented resident prefill hit without an installed GPU driver - require dasllama/dasllama_math_vulkan or dasllama/dasllama_math_cuda (the Vulkan and CUDA tiers install it)")
}

var g_rdec_prefill_segs = @@rdec_unset_prefill_segs
var g_rdec_prefill_segs_installed = false

//! Install the driver's segmented prefill (a GPU backend whose window chain takes several sessions' chunks)
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def public install_rdec_prefill_segs(prefill_segs : RdecPrefillSegsFn) {
    g_rdec_prefill_segs = prefill_segs
    g_rdec_prefill_segs_installed = true
}

[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def public rdec_prefill_segs_installed() : bool => g_rdec_prefill_segs_installed
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def public rdec_prefill_segs(x_batch : array<float>; tokens : array<int64>; use_ids : bool; emb_scale : float; cos_batch : array<float>; segs : array<RdecPfSeg>; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    invoke(g_rdec_prefill_segs, x_batch, tokens, use_ids, emb_scale, cos_batch, segs, logits, experts)
}

[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def public rdec_set_emb(emb_block : int64; fmt : int; escale : float) : bool => invoke(g_rdec_set_emb, emb_block, fmt, escale)
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def public rdec_upload_emb_f32(fblob : array<float>; off, vocab, dim : int64; escale : float) : bool => invoke(g_rdec_upload_emb_f32, fblob, off, vocab, dim, escale)

//! the speculative carry's landing on a driver prepared with `carry`: row `row` of the post-final-norm rows the last token
//! step landed (the prefill's last row at row 0), `dim` floats into `dst`
typedef RdecLandCarryFn = function<(row : int64; var dst : array<float>) : void>
[unused_argument(row, dst)]
def private rdec_unset_land_carry(row : int64; var dst : array<float>) {
    panic("dasLLAMA: resident carry landing hit without an installed GPU driver")
}
var g_rdec_land_carry = @@rdec_unset_land_carry
var g_rdec_carry_installed = false

[arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#logits-transfer-queue")]
def public install_rdec_carry(land : RdecLandCarryFn) {
    g_rdec_land_carry = land
    g_rdec_carry_installed = true
}

def public rdec_carry_installed() : bool => g_rdec_carry_installed
def public rdec_land_carry(row : int64; var dst : array<float>) { invoke(g_rdec_land_carry, row, dst) }

//! the NextN draft head's arm, after the classifier: its eh_proj plane `beh` in format `feh`, its attention quad and FFN triple
//! at layer index n_layers; false = a class rail declined and the head stays off the device
typedef RdecSetHeadFn = function<(beh : int64; feh : int; bq : int64; bk : int64; bv : int64; bo : int64; b1 : int64; b3 : int64; b2 : int64; fq : int; fk : int; fv : int; fo : int; f1 : int; f3 : int; f2 : int; g : RdecLayerGeom) : bool>
//! one NextN draft at head row `row` of the selected region: the host head rows [hyd0, row) at `kp` / `vp` (from row 0) come up, then the token -
//! its id `token`, gathered on the device, or at -1 `x`, its unscaled row - and the carry `h` run the head; `rdec_land_carry`'s row 0 takes its hidden,
//! `logits` its logits unless `pick_only`, row `row`'s K/V stays on the device (`rdec_kv_read` at layer n_layers on a pass). The argmax id, -1 with no head
typedef RdecDraftFn = function<(x : array<float>; token : int64; h : array<float>; cossin : array<float>; row : int64; hyd0 : int64; var kp : uint8?; var vp : uint8?; pick_only : bool; var logits : array<float>; experts : RdecHostExpertsFn) : int64>
[unused_argument(beh, feh, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2, g)]
def private rdec_unset_set_head(beh : int64; feh : int; bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int; g : RdecLayerGeom) : bool => false
[unused_argument(x, token, h, cossin, row, hyd0, kp, vp, pick_only, logits, experts)]
def private rdec_unset_draft(x : array<float>; token : int64; h : array<float>; cossin : array<float>; row, hyd0 : int64; var kp : uint8?; var vp : uint8?; pick_only : bool; var logits : array<float>; experts : RdecHostExpertsFn) : int64 => -1l
var g_rdec_set_head = @@rdec_unset_set_head
var g_rdec_draft = @@rdec_unset_draft
var g_rdec_draft_installed = false

[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head")]
def public install_rdec_draft(set_head : RdecSetHeadFn; draft : RdecDraftFn) {
    g_rdec_set_head = set_head
    g_rdec_draft = draft
    g_rdec_draft_installed = true
}

//! the draft seat back to its unset form, the CPU draft's control: every NextN draft declines to `forward_mtp`'s CPU form until
//! `reinstall_rdec_draft` takes the answer - the seat installed before - back; a head already in the arena stays there
def public unset_rdec_draft() : RdecDraftFn {
    let seat = g_rdec_draft
    g_rdec_draft = @@rdec_unset_draft
    g_rdec_draft_installed = false
    to_log(LOG_INFO, "dasLLAMA: the resident NextN draft seat is unset (unset_rdec_draft) - the CPU draft serves until reinstall_rdec_draft\n")
    return seat
}

def public reinstall_rdec_draft(draft : RdecDraftFn) {
    g_rdec_draft = draft
    g_rdec_draft_installed = true
}

def public rdec_draft_installed() : bool => g_rdec_draft_installed
def public rdec_set_head(beh : int64; feh : int; bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int; g : RdecLayerGeom) : bool
    => invoke(g_rdec_set_head, beh, feh, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2, g)
//! `experts` sums a routed head's experts on the host between the draft's segments (the hyper-connection chain); a dense head never calls it
def public rdec_draft(x : array<float>; token : int64; h : array<float>; cossin : array<float>; row, hyd0 : int64; var kp : uint8?; var vp : uint8?; pick_only : bool; var logits : array<float>; experts : RdecHostExpertsFn) : int64
    => invoke(g_rdec_draft, x, token, h, cossin, row, hyd0, kp, vp, pick_only, logits, experts)

//! the same-slab verify of `n` rows at `pos` in the selected region: `x` / `cossin` the rows and rope rows, `hemb` / `hcos` the head's unscaled embeds and rope
//! rows a position below (`ids`, where the driver gathers the rows: `x` / `hemb` unread), `h0` the pre-draft carry; picks and hidden land in `picks` / `hid`,
//! logits in `logits` unless `pick_only` (a guard step lands them); the head's K/V rows `pos - 1` .. stay on the device. False where it steps no `n` rows
typedef RdecVerifyFn = function<(x : array<float>; cossin : array<float>; hemb : array<float>; hcos : array<float>; h0 : array<float>; side : array<float>; ids : array<int64>; pos : int64; n : int64; pick_only : bool; var logits : array<float>; var picks : array<int64>; var hid : array<float>; experts : RdecHostExpertsRowsFn) : bool>
[unused_argument(x, cossin, hemb, hcos, h0, side, ids, pos, n, pick_only, logits, picks, hid, experts)]
def private rdec_unset_verify(x, cossin, hemb, hcos, h0, side : array<float>; ids : array<int64>; pos, n : int64; pick_only : bool; var logits : array<float>; var picks : array<int64>; var hid : array<float>; experts : RdecHostExpertsRowsFn) : bool => false
def private rdec_no_verify_rows : int64 => 0l
def public rdec_no_verify_landed_logits : bool => false   //! public: a default argument resolves at the caller's site
var g_rdec_verify = @@rdec_unset_verify
var g_rdec_verify_rows = @@rdec_no_verify_rows
var g_rdec_verify_landed_logits = @@rdec_no_verify_landed_logits
var g_rdec_verify_installed = false

//! Install the same-slab verify beside the draft seat: `rows` answers how many rows of one stream it steps at once (0 = none);
//! `landed_logits` whether the last served verify landed the rows' logits - under a pick ask a guard step lands them anyway
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command")]
def public install_rdec_verify(verify : RdecVerifyFn; rows : function<() : int64>; landed_logits : function<() : bool> = @@rdec_no_verify_landed_logits) {
    g_rdec_verify = verify
    g_rdec_verify_rows = rows
    g_rdec_verify_landed_logits = landed_logits
    g_rdec_verify_installed = true
}

def public rdec_verify_installed() : bool => g_rdec_verify_installed

//! The selection landing a device driver leaves a sampled stream's step in place of the rows' logits: each row's survivors (the ids
//! at or above the row's SEL_K-th largest logit, ties kept), its max and softmax denominator at the stream's temperature, and the
//! logits of the recent-window ids the ask listed - pointers into the driver's mapped landing, good until the driver's next command
struct SelLanding {
    rows : int64             //! 0 = nothing landed
    cap : int64              //! survivor slots a row
    recent_cap : int64       //! recent-window slots a row
    hdr : uint const?        //! SEL_HDR words a row: the survivors' count, the overflow flag, the max's bits, the denominator's bits
    ids : uint const?
    vals : float const?
    rec : float const?
}

let SEL_LANE_LOWEST = -3.4e38   //! the lowest finite lane value: what a NaN or -inf logit reads as on the device and on the CPU alike


let SEL_K = 1024l                 //! the survivors a row lands - the candidate-list sampler's cap (the Vulkan kernel's CLS_SELECT_K)
let SEL_RECENT = 128l             //! the recent-window ids an ask lists a row (CLS_SELECT_RECENT)
let SEL_CAP = 2048l               //! the landing's slots a row - the compaction's cap, the overflow flagged past it (the template's CAP)
let SEL_WORDS = SEL_RECENT + 4l   //! one row's ask words (CLS_SEL_WORDS): the `SEL_ASK_*` words, then the recent ids
let SEL_HDR = 4l                  //! one row's landed header words (CLS_SEL_HDR): the `SEL_HDR_*` words
let SEL_ASK_TEMP = 0l             //! the ask row's temperature, as float bits
let SEL_ASK_COUNT = 1l            //! the recent ids the ask row lists
let SEL_ASK_IDS = 4l              //! the first recent id (two pad words before it)
let SEL_HDR_COUNT = 0l            //! the survivors the row landed
let SEL_HDR_OVERFLOW = 1l         //! 1 where the row's ties overflowed its slots
let SEL_HDR_MAX = 2l              //! the row's max, as float bits
let SEL_HDR_DEN = 3l              //! the row's softmax denominator at its temperature, as float bits

//! the selection ask for the driver's next command of `nrows` rows, `words` SEL_WORDS a row; false where the device has no selection
typedef RdecSelAskFn = function<(nrows : int64; words : array<uint>) : bool>
//! the last command's selection landing (rows = 0 where none was asked)
typedef RdecSelLandingFn = function<() : SelLanding>
//! the late landing of the last command's rows `row0 .. row0 + nrows` into `dst`, for a sampler the selection cannot serve
typedef RdecSelLandRowsFn = function<(row0 : int64; nrows : int64; var dst : array<float>) : void>
[unused_argument(nrows, words)]
def private rdec_unset_sel_ask(nrows : int64; words : array<uint>) : bool => false
def private rdec_unset_sel_landing : SelLanding => SelLanding()
[unused_argument(row0, nrows, dst)]
def private rdec_unset_sel_land_rows(row0, nrows : int64; var dst : array<float>) {
    panic("dasLLAMA: a late landing of the selection's rows with no driver installed - a selection lands only from a driver that asked for one (the program must require dasllama/dasllama_math_vulkan, which installs the Vulkan driver's seats); engine bug")
}
var g_rdec_sel_ask = @@rdec_unset_sel_ask
var g_rdec_sel_landing = @@rdec_unset_sel_landing
var g_rdec_sel_land_rows = @@rdec_unset_sel_land_rows

//! Install the selection landing beside the token, draft and verify seats
def public install_rdec_select(ask : RdecSelAskFn; landing : RdecSelLandingFn; land_rows : RdecSelLandRowsFn) {
    g_rdec_sel_ask = ask
    g_rdec_sel_landing = landing
    g_rdec_sel_land_rows = land_rows
}

//! the three selection seats as installed: a fixture that installs its own puts these back through [[restore_rdec_select]]
struct RdecSelectPin {
    ask : RdecSelAskFn
    landing : RdecSelLandingFn
    land_rows : RdecSelLandRowsFn
}

def public rdec_select_pin() : RdecSelectPin => RdecSelectPin(ask = g_rdec_sel_ask, landing = g_rdec_sel_landing, land_rows = g_rdec_sel_land_rows)

def public restore_rdec_select(pin : RdecSelectPin) {
    install_rdec_select(pin.ask, pin.landing, pin.land_rows)
}

def public rdec_sel_ask(nrows : int64; words : array<uint>) : bool => invoke(g_rdec_sel_ask, nrows, words)
def public rdec_sel_landing() : SelLanding => invoke(g_rdec_sel_landing)
def public rdec_sel_land_rows(row0, nrows : int64; var dst : array<float>) {
    invoke(g_rdec_sel_land_rows, row0, nrows, dst)
}
def public rdec_verify_rows() : int64 => invoke(g_rdec_verify_rows)
def public rdec_verify_landed_logits() : bool => invoke(g_rdec_verify_landed_logits)
//! `side` the rows' n-gram side rows and `experts` the host's routed sums of the rows between a hyper-connection chain's segments; empty and unreached elsewhere
def public rdec_verify(x, cossin, hemb, hcos, h0, side : array<float>; ids : array<int64>; pos, n : int64; pick_only : bool; var logits : array<float>; var picks : array<int64>; var hid : array<float>; experts : RdecHostExpertsRowsFn) : bool
    => invoke(g_rdec_verify, x, cossin, hemb, hcos, h0, side, ids, pos, n, pick_only, logits, picks, hid, experts)

//! the same-slab verify's reject in the selected region: every recurrent slot and ring pair back to the copy the verify command
//! took after its row `a` - the last row the round committed - so the round takes no host snapshot and replays no step; false
//! where the driver holds no such copy (a region the last verify did not step, a row past its copies)
typedef RdecRollbackFn = function<(a : int64) : bool>
[unused_argument(a)]
def private rdec_unset_rollback(a : int64) : bool => false
var g_rdec_rollback = @@rdec_unset_rollback
var g_rdec_rollback_installed = false

[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback")]
def public install_rdec_rollback(rollback : RdecRollbackFn) {
    g_rdec_rollback = rollback
    g_rdec_rollback_installed = true
}

def public rdec_rollback_installed() : bool => g_rdec_rollback_installed
def public rdec_rollback(a : int64) : bool => invoke(g_rdec_rollback, a)

//! the short window's peel on a recurrent model: `nrows` consecutive rows of the selected region from `pos` through the in-order rows
//! command, the last row's logits into `logits`
typedef RdecTokenSeqFn = function<(x : array<float>; cossin : array<float>; pos, nrows : int64; var logits : array<float>) : void>
[unused_argument(x, cossin, pos, nrows, logits)]
def private rdec_unset_token_seq(x : array<float>; cossin : array<float>; pos, nrows : int64; var logits : array<float>) {
    panic("dasLLAMA: the resident in-order rows command hit without an installed seat")
}
var g_rdec_token_seq = @@rdec_unset_token_seq
var g_rdec_token_seq_installed = false

[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-peel")]
def public install_rdec_token_seq(token_seq : RdecTokenSeqFn) {
    g_rdec_token_seq = token_seq
    g_rdec_token_seq_installed = true
}

def public rdec_token_seq_installed() : bool => g_rdec_token_seq_installed
def public rdec_token_seq(x : array<float>; cossin : array<float>; pos, nrows : int64; var logits : array<float>) { invoke(g_rdec_token_seq, x, cossin, pos, nrows, logits) }

//! The one-row command's greedy chain: `wanted` answers whether the armed model's next one-row step may chain, `ask` asks the
//! step about to run to chain the next one, the next position's rope row beside it
typedef RdecChainWantedFn = function<() : bool>
typedef RdecChainAskFn = function<(cos_next : array<float>) : void>
def private rdec_unset_chain_wanted() : bool => false
[unused_argument(cos_next)]
def private rdec_unset_chain_ask(cos_next : array<float>) {
    panic("dasLLAMA: the resident greedy chain asked without an installed seat")
}
var g_rdec_chain_wanted = @@rdec_unset_chain_wanted
var g_rdec_chain_ask = @@rdec_unset_chain_ask

[arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#vk-greedy-chain")]
def public install_rdec_chain(wanted : RdecChainWantedFn; ask : RdecChainAskFn) {
    g_rdec_chain_wanted = wanted
    g_rdec_chain_ask = ask
}

def public rdec_chain_wanted() : bool => invoke(g_rdec_chain_wanted)
def public rdec_chain_ask(cos_next : array<float>) { invoke(g_rdec_chain_ask, cos_next) }

var private g_rdec_chain_charged = false   //! the chain is asked: the plan charges its snapshot

//! the backend's word that the chain is asked, read by the next plan
def public set_rdec_chain_charged(on : bool) {
    g_rdec_chain_charged = on
}

def public rdec_chain_charged() : bool => g_rdec_chain_charged

//! A region's recurrent checkpoints: `save` copies every recurrent layer's state slot and ring pair of the selected region into its
//! checkpoint row `slot` (one of `RDEC_DN_SNAPS`), `load` copies row `slot` back into the slots and the region's ring parity with it;
//! false where the driver holds no such row
let public RDEC_DN_SNAPS = 4   //! the checkpoint rows a mirror region keeps (an LRU), each a whole recurrent state
typedef RdecDnSnapFn = function<(slot : int64) : bool>
[unused_argument(slot)]
def private rdec_unset_dn_snap(slot : int64) : bool => false
var g_rdec_dn_snap_save = @@rdec_unset_dn_snap
var g_rdec_dn_snap_load = @@rdec_unset_dn_snap
var g_rdec_dn_snaps_installed = false

[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#device-home-checkpoints")]
def public install_rdec_dn_snaps(save, load : RdecDnSnapFn) {
    g_rdec_dn_snap_save = save
    g_rdec_dn_snap_load = load
    g_rdec_dn_snaps_installed = true
}

def public rdec_dn_snaps_installed() : bool => g_rdec_dn_snaps_installed

//! Where a recurrent layer's state and ring land on the host
struct public RdecDnDst {
    l : int64
    stp : float?
    histp : float?
}

//! A copy of every listed recurrent layer's state and ring in the selected region - its live slots (`slot` -1), its slots as a parked
//! claim left them (-2), or its checkpoint row `slot` - in one submit, the region left as it stands; false where they hold none
typedef RdecDnPeekFn = function<(slot : int64; dsts : array<RdecDnDst>) : bool>
[unused_argument(slot, dsts)]
def private rdec_unset_dn_peek(slot : int64; dsts : array<RdecDnDst>) : bool => false
var g_rdec_dn_peek = @@rdec_unset_dn_peek

def public install_rdec_dn_peek(peek : RdecDnPeekFn) {
    g_rdec_dn_peek = peek
}

def public rdec_dn_peek(slot : int64; dsts : array<RdecDnDst>) : bool => invoke(g_rdec_dn_peek, slot, dsts)

//! checkpoint row `slot`'s n-gram conv ring (the live region's where `slot` < 0) into `host`: the ring a continuation adopted there reads first
typedef RdecDnSnapRingFn = function<(slot : int64; var host : float?) : bool>

[unused_argument(slot, host)]
def private rdec_unset_dn_snap_ring(slot : int64; var host : float?) : bool => false
var g_rdec_dn_snap_ring = @@rdec_unset_dn_snap_ring

def public install_rdec_dn_snap_ring(ring : RdecDnSnapRingFn) {
    g_rdec_dn_snap_ring = ring
}

def public rdec_dn_snap_ring(slot : int64; var host : float?) : bool => invoke(g_rdec_dn_snap_ring, slot, host)

//! One run of a layer's rows between a host cache and the selected region: row `row0` of the run sits `row0` rows past `kp` / `vp`
struct public RdecKvSpan {
    l : int64
    row0 : int64
    nrows : int64
    kp : uint8?
    vp : uint8?
}

//! Every span moved up (`up`) or down in as few submits as the driver's staging allows; false where no driver serves the batch
typedef RdecKvXferFn = function<(var spans : array<RdecKvSpan>; up : bool) : bool>
[unused_argument(spans, up)]
def private rdec_unset_kv_xfer(var spans : array<RdecKvSpan>; up : bool) : bool => false
var g_rdec_kv_xfer = @@rdec_unset_kv_xfer

def public install_rdec_kv_xfer(xfer : RdecKvXferFn) {
    g_rdec_kv_xfer = xfer
}

def public rdec_kv_xfer(var spans : array<RdecKvSpan>; up : bool) : bool => invoke(g_rdec_kv_xfer, spans, up)
def public rdec_dn_snap_save(slot : int64) : bool => invoke(g_rdec_dn_snap_save, slot)
def public rdec_dn_snap_load(slot : int64) : bool => invoke(g_rdec_dn_snap_load, slot)

//! the NextN head's prompt warm on the window chain: `ask` names whether the next chain warms the head's slab over the prompt rows
//! it serves, `rows` answers how many head rows the last chain wrote (0 = none); the head's rows move through the K/V row seats at layer `n_layers`
typedef RdecHeadWarmAskFn = function<(on : bool) : void>
typedef RdecHeadWarmRowsFn = function<() : int64>
[unused_argument(on)]
def private rdec_unset_head_warm_ask(on : bool) {}
def private rdec_unset_head_warm_rows : int64 => 0l
var g_rdec_head_warm_ask = @@rdec_unset_head_warm_ask
var g_rdec_head_warm_rows = @@rdec_unset_head_warm_rows
var g_rdec_head_warm_installed = false

[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm")]
def public install_rdec_head_warm(ask : RdecHeadWarmAskFn; rows : RdecHeadWarmRowsFn) {
    g_rdec_head_warm_ask = ask
    g_rdec_head_warm_rows = rows
    g_rdec_head_warm_installed = true
}

def public rdec_head_warm_installed() : bool => g_rdec_head_warm_installed
def public rdec_head_warm_ask(on : bool) { invoke(g_rdec_head_warm_ask, on) }
def public rdec_head_warm_rows() : int64 => invoke(g_rdec_head_warm_rows)

//! the window chain's packed segments: `fits` answers whether the armed driver can seat several segments in one region, each from
//! its own first row (`RdecPfSeg.koff`) - no sliding layer's ring, no block-codec store
typedef RdecSegsPackedFn = function<() : bool>
def private rdec_unset_segs_packed : bool => false
var g_rdec_segs_packed = @@rdec_unset_segs_packed

[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-scratch")]
def public install_rdec_segs_packed(fits : RdecSegsPackedFn) {
    g_rdec_segs_packed = fits
}

def public rdec_segs_packed() : bool => invoke(g_rdec_segs_packed)

//! the window chain's final residual rows: `ask` names whether the next chains land every row's final residual (an embedding
//! pools them), `rows` copies `nrows` of the last chain's rows from window row `src_row` into `dst` at element `dst_off`
typedef RdecHiddenAskFn = function<(on : bool) : void>
typedef RdecHiddenRowsFn = function<(var dst : array<float>; src_row, dst_off, nrows : int64) : void>
def private rdec_unset_hidden_ask(on : bool) {
    if (on) {
        panic("dasLLAMA: a window asked for its hidden rows with no hidden-rows seat installed - the program must require dasllama/dasllama_math_vulkan")
    }
}
[unused_argument(dst, src_row, dst_off, nrows)]
def private rdec_unset_hidden_rows(var dst : array<float>; src_row, dst_off, nrows : int64) {
    panic("dasLLAMA: a window's hidden rows were read with no hidden-rows seat installed - the program must require dasllama/dasllama_math_vulkan")
}
var g_rdec_hidden_ask = @@rdec_unset_hidden_ask
var g_rdec_hidden_rows = @@rdec_unset_hidden_rows
var g_rdec_hidden_installed = false

[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-hidden-rows")]
def public install_rdec_hidden(ask : RdecHiddenAskFn; rows : RdecHiddenRowsFn) {
    g_rdec_hidden_ask = ask
    g_rdec_hidden_rows = rows
    g_rdec_hidden_installed = true
}

def public rdec_hidden_installed() : bool => g_rdec_hidden_installed
def public rdec_hidden_ask(on : bool) { invoke(g_rdec_hidden_ask, on) }
def public rdec_hidden_rows(var dst : array<float>; src_row, dst_off, nrows : int64) { invoke(g_rdec_hidden_rows, dst, src_row, dst_off, nrows) }

//! the per-layer-embedding seats (gemma-4 E-series): `set_ple` sizes the side-input rows at the model's ple width
//! (after prepare, before the layers); `set_ple_layer` wires one layer's gate [dim x ple] and proj [ple x dim] planes
//! `gate_f32` / `proj_f32`: every layer's gate / projection rows where the kind ships F32 (empty where it rides the arena)
typedef RdecSetPleFn = function<(ple : int64; gate_f32 : array<float>; proj_f32 : array<float>) : bool>
//! a block of -1 names the kind's f32 rows (`RdecSetPleFn`), else the plane in its format slot (`fgate` / `fproj`)
typedef RdecSetPleLayerFn = function<(l : int64; bgate : int64; bproj : int64; fgate : int; fproj : int) : void>
//! the pre-step's projection on device: the per-layer model projection [layers*ple x dim] in the file's own rows - `w16` its bf16
//! halves, or `w32` its f32 rows - and `pnorm` its [ple] norm row; false = the driver keeps the CPU pre-step
typedef RdecSetPleProjFn = function<(w16 : array<uint16>; w32 : array<float>; pnorm : array<float>; all : int64) : bool>
//! the per-layer-embedding token table placed in the arena (`blk`, a plane [vocab x layers*ple] in format slot `fmt`, the table's
//! own tag): the window chain gathers its side rows on device off the token ids
typedef RdecSetPleTableFn = function<(blk : int64; fmt : int) : bool>
[unused_argument(ple, gate_f32, proj_f32)]
def private rdec_unset_set_ple(ple : int64; gate_f32 : array<float>; proj_f32 : array<float>) : bool => false
[unused_argument(l, bgate, bproj, fgate, fproj)]
def private rdec_unset_set_ple_layer(l, bgate, bproj : int64; fgate, fproj : int) {
    panic("dasLLAMA: resident per-layer-embedding layer planes hit without an installed GPU driver - require dasllama/dasllama_math_vulkan (the Vulkan tier installs it)")
}
[unused_argument(w16, w32, pnorm, all)]
def private rdec_unset_set_ple_proj(w16 : array<uint16>; w32 : array<float>; pnorm : array<float>; all : int64) : bool => false
[unused_argument(blk, fmt)]
def private rdec_unset_set_ple_table(blk : int64; fmt : int) : bool => false

var g_rdec_set_ple = @@rdec_unset_set_ple
var g_rdec_set_ple_layer = @@rdec_unset_set_ple_layer
var g_rdec_set_ple_proj = @@rdec_unset_set_ple_proj
var g_rdec_set_ple_table = @@rdec_unset_set_ple_table
var g_rdec_ple_installed = false
var g_rdec_ple_proj_on = false

[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def public install_rdec_ple(set_ple : RdecSetPleFn; set_ple_layer : RdecSetPleLayerFn; set_ple_proj : RdecSetPleProjFn; set_ple_table : RdecSetPleTableFn) {
    g_rdec_set_ple = set_ple
    g_rdec_set_ple_layer = set_ple_layer
    g_rdec_set_ple_proj = set_ple_proj
    g_rdec_set_ple_table = set_ple_table
    g_rdec_ple_installed = true
}

//! true once the driver holds the token table: the ids prefill then gathers the side rows itself
def public rdec_set_ple_table(blk : int64; fmt : int) : bool => invoke(g_rdec_set_ple_table, blk, fmt)

def public rdec_ple_installed() : bool => g_rdec_ple_installed
def public rdec_set_ple(ple : int64; gate_f32 : array<float>; proj_f32 : array<float>) : bool => invoke(g_rdec_set_ple, ple, gate_f32, proj_f32)
def public rdec_set_ple_layer(l, bgate, bproj : int64; fgate, fproj : int) { invoke(g_rdec_set_ple_layer, l, bgate, bproj, fgate, fproj) }
//! true once the driver took the pre-step's projection: the prefill then wants the gather half alone (`ple_raw`)
def public rdec_set_ple_proj(w16 : array<uint16>; w32 : array<float>; pnorm : array<float>; all : int64) : bool {
    g_rdec_ple_proj_on = invoke(g_rdec_set_ple_proj, w16, w32, pnorm, all)
    return g_rdec_ple_proj_on
}
def public rdec_ple_proj_on() : bool => g_rdec_ple_proj_on
def public rdec_ple_proj_set(on : bool) { g_rdec_ple_proj_on = on }

def public moe_gpu_resident_installed() : bool => g_rdec_installed
def public rdec_reserve(fmt : int; cap_blocks : int64) : bool => invoke(g_rdec_reserve, fmt, cap_blocks)
def public rdec_place(wq : array<uint8>; ws : array<uint8>; n, rows : int64; fmt : int) : int64 => invoke(g_rdec_place, wq, ws, n, rows, fmt)
def public rdec_prepare(n_layers, n_attn, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64; neox : bool; eps, scale : float; qk_norm : bool; kv_dtype : int; q_gated : bool; rot : int64; dn : RdecDnGeom; mir_units, n_slabs, cos_elems, regions : int64; carry, head : bool; verify_rows : int64) : bool => invoke(g_rdec_prepare, n_layers, n_attn, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap, neox, eps, scale, qk_norm, kv_dtype, q_gated, rot, dn, mir_units, n_slabs, cos_elems, regions, carry, head, verify_rows)
def public rdec_upload_norms(norms : array<float>; obias : bool) { invoke(g_rdec_norms, norms, obias) }
def public rdec_set_layer(l, mir_idx, bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int; g : RdecLayerGeom) { invoke(g_rdec_set_layer, l, mir_idx, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2, g) }
def public rdec_set_cls(cls_block : int64; cls_fmt : int; cap : float; supp : array<int64>) { invoke(g_rdec_set_cls, cls_block, cls_fmt, cap, supp) }
def public rdec_token(x : array<float>; cossin : array<float>; ple_row : array<float>; token, pos, cnt : int64; pick_only : bool; var logits : array<float>) : int64 => invoke(g_rdec_token, x, cossin, ple_row, token, pos, cnt, pick_only, logits)
def public rdec_prefill(x_batch : array<float>; cos_batch : array<float>; ple_rows : array<float>; ple_raw : bool; npos : int64; pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    invoke(g_rdec_prefill, x_batch, cos_batch, ple_rows, ple_raw, npos, pos0, logits, experts)
}
def public rdec_kv_sync(l, row0, nrows : int64; kp : uint8 const?; vp : uint8 const?) { invoke(g_rdec_kv_sync, l, row0, nrows, kp, vp) }
def public rdec_kv_read(l, row0, nrows : int64; var kp : uint8?; var vp : uint8?) { invoke(g_rdec_kv_read, l, row0, nrows, kp, vp) }

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
    g_installed_marks.layer_count = k
}

def public moe_gpu_layer_count : int64 => g_moe_gpu_installed ? g_installed_marks.layer_count : 0l

//! Loader contract: called once per load after the resident set is known (layers - K .. layers).
def public set_moe_gpu_from_layer(from_layer : int64) {
    g_installed_marks.from_layer = from_layer
}

def public moe_gpu_layer_on_gpu(l : int64) : bool {
    // upper bound is load-bearing: the MTP draft block sits at index n_layers, above every uploaded
    // range, so an unbounded `l >= from_layer` dispatches against a region never uploaded.
    // total 0 = loader never reported it; keep the old unbounded answer
    return (g_moe_gpu_installed && l >= g_installed_marks.from_layer
        && (g_installed_marks.total_layers <= 0l || l < g_installed_marks.total_layers))
}

//! Loader contract: first streamed layer (streamed range = [from, the resident set); reset per
//! load). Streamed layers keep their CPU planes — only PREFILL batches dispatch on the GPU,
//! the expert stacks DMA-streamed from pinned host mirrors; decode stays on the CPU.
def public set_moe_gpu_stream_from(from_layer : int64) {
    g_installed_marks.stream_from = from_layer
}

def public moe_gpu_layer_streamed(l : int64) : bool =>
    g_moe_gpu_installed && l >= g_installed_marks.stream_from && l < g_installed_marks.from_layer

//! Loader contract: the resident classifier plane's element offset (-1 = none; reset per load).
def public set_moe_gpu_cls_off(woff : int64) {
    g_installed_marks.cls_off = woff
}

def public moe_gpu_cls_on_gpu(woff : int64) : bool =>
    g_moe_gpu_installed && g_moe_gpu_cls_route && woff == g_installed_marks.cls_off

//! Runtime routing gate over the resident classifier plane (the --cls-ab in-process lever).
//! Route flip only — the upload and its VRAM residency stay, matching the other tier gates.
def public set_moe_gpu_cls_route(on : bool) {
    g_moe_gpu_cls_route = on
}

//! Loader contract: mark one dense (attention-side) plane resident on the tier. Keyed by
//! (fmt, element offset) — the same identity the tier resolves stacks by, so the batch GEMM
//! funnels route any marked weight without knowing layers or arches. Reset per load.
def public set_moe_gpu_dense_off(fmt : int; woff : int64) {
    g_installed_marks.dense_offs |> insert(woff * 8l + int64(fmt))
}

def public moe_gpu_dense_on_gpu(fmt : int; woff : int64) : bool =>
    g_moe_gpu_installed && g_moe_gpu_dense_route && key_exists(g_installed_marks.dense_offs, woff * 8l + int64(fmt))

var private g_dense_ffn_rows = 0l   //! the dense FFN rows the per-op FFN rail ran on the device, process-wide

//! the dense FFN rows the per-op FFN rail ran on the device so far (a layer's row a step)
def public moe_gpu_dense_ffn_rows() : int64 => g_dense_ffn_rows

//! count `rows` dense FFN rows the per-op FFN rail ran as three dense GEMMs (a gate and up of two activation forms)
def public moe_gpu_dense_ffn_note(rows : int64) {
    g_dense_ffn_rows += rows
}

//! a dense FFN's gate, up and down planes all sit on the dense rail: its decode row runs on the device (`matmul_gpu_dense_ffn`)
[arch(at="../ARCHITECTURE_GPU_VULKAN_PEROP.md#perop-dense-ffn")]
def public moe_gpu_dense_ffn_on_gpu(f1, f3, f2 : int; w1, w3, w2 : int64) : bool =>
    moe_gpu_dense_on_gpu(f1, w1) && moe_gpu_dense_on_gpu(f3, w3) && moe_gpu_dense_on_gpu(f2, w2)

[arch(at="../ARCHITECTURE_GPU_VULKAN_PEROP.md#perop-dense-ffn")]
def public matmul_gpu_dense_ffn(var y : array<float>; w1, w3, w2 : int64; xq : array<int8>; xs : array<float>; n, nfe : int64; f1, f3, f2 : int; is_gelu : bool) {
    g_dense_ffn_rows++
    let o1 = fixed_array(w1, 0l, 1l)
    let o3 = fixed_array(w3, 0l, 1l)
    let o2 = fixed_array(w2, 0l, 1l)
    unsafe {
        invoke(g_moe_gpu_ffn, addr(y[0]), addr(o1[0]), addr(o3[0]), addr(o2[0]), 1l,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), null, null, n, nfe, 1l, 0l, f1, f3, f2, is_gelu)
    }
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
    g_installed_marks.dn_layers |> insert(l, woq)
}

//! The marked layer's qkv plane offset (-1 when the layer carries no resident triple).
def public moe_gpu_dn_layer_key(l : int64) : int64 => g_installed_marks.dn_layers?[l] ?? -1l

def public reset_moe_gpu_dn_layers {
    g_installed_marks.dn_layers |> clear()
}

def public moe_gpu_dn_on_gpu(l : int64) : bool =>
    g_moe_gpu_installed && g_moe_gpu_dn_route && key_exists(g_installed_marks.dn_layers, l)

//! Runtime routing gate over the deltanet marks (the --dn-ab in-process lever; uploads stay).
def public set_moe_gpu_dn_route(on : bool) {
    g_moe_gpu_dn_route = on
}

//! The deltanet chain's routing gate.
def public moe_gpu_dn_route() : bool => g_moe_gpu_dn_route

// ===== deltanet DECODE step =====
// Same MoeGpuDnFn hook at npos == 1, same dn marks/uploads; separate routing gate (--dnd-ab).
// State lives device-side across steps — flush/invalidate seams guard every CPU state consumer.

typedef MoeGpuDnFlushFn = function<(l : int64; var stp : float?; var histp : float?) : void>
typedef MoeGpuDnInvalidateFn = function<() : void>
typedef MoeGpuDnReleaseFn = function<(state_lo : uint64; state_hi : uint64) : void>

[unused_argument(l, stp, histp)]
def private moe_gpu_noop_dn_flush(l : int64; var stp : float?; var histp : float?) {}
def private moe_gpu_noop_dn_invalidate() {}
[unused_argument(state_lo, state_hi)]
def private moe_gpu_noop_dn_release(state_lo : uint64; state_hi : uint64) {}

var g_moe_gpu_dnd_route = true
var g_moe_gpu_dn_flush = @@moe_gpu_noop_dn_flush
var g_moe_gpu_dn_invalidate = @@moe_gpu_noop_dn_invalidate
var g_moe_gpu_dn_release = @@moe_gpu_noop_dn_release

//! How many sessions' recurrent state a driver keeps device-resident at once: a scheduler names
//! its stream count before the first prefill, so no stream's state evicts under the batch it rides
typedef MoeGpuDnRoomFn = function<(n : int64) : void>
[unused_argument(n)]
def private moe_gpu_noop_dn_room(n : int64) {}
var g_moe_gpu_dn_room = @@moe_gpu_noop_dn_room

def gpu_dn_room_(n : int64) {
    invoke(g_moe_gpu_dn_room, n)
}

//! Tier registration of the room seam (beside set_moe_gpu_dn_state_hooks).
def public set_moe_gpu_dn_room_hook(room : MoeGpuDnRoomFn) {
    g_moe_gpu_dn_room = room
}

//! How many rows of context the armed driver holds a session's K/V for, given the bytes a row takes
//! over every layer's two planes; a driver with no bound answers the largest count
typedef GpuKvRoomFn = function<(row_bytes : int64) : int64>
[unused_argument(row_bytes)]
def private gpu_kv_room_unbounded(row_bytes : int64) : int64 => 9223372036854775807l
var g_gpu_kv_room = @@gpu_kv_room_unbounded

def gpu_kv_room_rows(row_bytes : int64) : int64 => invoke(g_gpu_kv_room, row_bytes)

//! Tier registration of the K/V room seam; the installed one is `gpu_kv_room_hook`, for a cell that doubles it and restores it.
def public set_gpu_kv_room_hook(f : GpuKvRoomFn) {
    g_gpu_kv_room = f
}

def public gpu_kv_room_hook() : GpuKvRoomFn => g_gpu_kv_room

def public moe_gpu_dnd_on_gpu(l : int64) : bool =>
    g_moe_gpu_installed && g_moe_gpu_dnd_route && key_exists(g_installed_marks.dn_layers, l)

//! Runtime routing gate over the deltanet DECODE step (the --dnd-ab in-process lever).
def public set_moe_gpu_dnd_route(on : bool) {
    g_moe_gpu_dnd_route = on
}

//! The deltanet decode step's routing gate.
def public moe_gpu_dnd_route() : bool => g_moe_gpu_dnd_route

//! a recurrent layer's device state slot as a session's: the ring image the row reads (`parity`), `valid` where the slot holds a
//! session's state, `dirty` where the device advanced past the host copy, and the owner's host state and history addresses the
//! flush writes back to - the bookkeeping both discrete homes keep beside a slot's device buffers
struct DnSlotOwner {
    parity : int
    valid, dirty : bool
    state_addr, hist_addr : uint64
}

//! the slot freed: no owner, nothing to write back; the ring image it last advanced onto stays named, since a parked claim's
//! snapshot reads the slot's image after its owner let go
def dn_slot_drop(var o : DnSlotOwner) {
    o = DnSlotOwner(parity = o.parity)
}

//! the slot bound to the session whose state sits at `stp` / `histp`, its device copy on ring image `parity`; `dirty` where the
//! device already holds the newer copy (a prefill landed it there)
def dn_slot_bind(var o : DnSlotOwner; stp, histp : float const?; parity : int; dirty : bool) {
    o = DnSlotOwner(parity = parity, valid = true, dirty = dirty, state_addr = intptr(stp), hist_addr = intptr(histp))
}

//! the slot holds the session whose state sits at `stp`
def dn_slot_holds(o : DnSlotOwner; stp : float const?) : bool => o.valid && o.state_addr == intptr(stp)

//! the slot's owner's state sits in the host range [lo, hi)
def dn_slot_in(o : DnSlotOwner; lo, hi : uint64) : bool => o.state_addr >= lo && o.state_addr < hi

//! the device advanced the slot onto ring image `parity`: the host copy is stale
def dn_slot_advance(var o : DnSlotOwner; parity : int) {
    o.parity = parity
    o.dirty = true
}

//! The installed state-residency seams, for a cell that overrides one and restores the set.
def public moe_gpu_dn_state_hooks() : tuple<flush : MoeGpuDnFlushFn; inval : MoeGpuDnInvalidateFn; release : MoeGpuDnReleaseFn> =>
    (flush = g_moe_gpu_dn_flush, inval = g_moe_gpu_dn_invalidate, release = g_moe_gpu_dn_release)

//! Tier registration of the state-residency seams (beside install_moe_gpu_tier).
def public set_moe_gpu_dn_state_hooks(flush : MoeGpuDnFlushFn; inval : MoeGpuDnInvalidateFn; release : MoeGpuDnReleaseFn) {
    g_moe_gpu_dn_flush = flush
    g_moe_gpu_dn_invalidate = inval
    g_moe_gpu_dn_release = release
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

//! Drop, unwritten, the device-resident deltanet copies one session's state owns: the session
//! zeroed that state, or it is dying.
def public moe_gpu_dn_release(state_lo : uint64; state_hi : uint64) {
    invoke(g_moe_gpu_dn_release, state_lo, state_hi)
}

//! Loader contract: mark one full-attention layer's q/k/v/o plane quad resident on the tier —
//! the prefill attention block then runs as one device chain. Reset per load.
def public set_moe_gpu_attn_layer(l : int64) {
    g_installed_marks.attn_layers |> insert(l)
}

def public moe_gpu_attn_on_gpu(l : int64) : bool =>
    g_moe_gpu_installed && g_moe_gpu_attn_route && key_exists(g_installed_marks.attn_layers, l)

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
                                     var hit : int8?; var o1 : int64?; var o3 : int64?; var o2 : int64?) : int64 =>
                                         0l

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

def public moe_gpu_heat_on : bool => g_moe_gpu_heat_installed && g_moe_gpu_heat_route

//! The form the next routed expert-chain calls take past the plain SwiGLU / GeGLU: gpt-oss's clamped swiglu, and each expert's
//! gate and up bias rows joining before the activation; a region's expert is `(offs1[3r] - e_base) / e_stride`
struct MoeGpuFfnForm {
    oai : bool
    bias_off : int64 = -1l   //! this layer's row base in the tier's gate / up bias planes (`moe_gpu_upload_ffn_bias`); -1 = unbiased
    e_base : int64           //! the layer's gate plane offset
    e_stride : int64         //! the elements of one expert's gate rows
    bias_resident : bool = false   //! the bias planes are the resident driver's (the row base a routed layer's ordinal in them), not the per-op upload's
}

var private g_ffn_form = MoeGpuFfnForm()

//! the routed form the next expert-chain calls serve; `clear_moe_gpu_ffn_form` puts the plain form back
def public set_moe_gpu_ffn_form(f : MoeGpuFfnForm) {
    g_ffn_form = f
}

def public clear_moe_gpu_ffn_form() {
    g_ffn_form = MoeGpuFfnForm()
}

def public moe_gpu_ffn_form() : MoeGpuFfnForm => g_ffn_form

var private @scratch g_ffn_experts : array<int64>

//! the expert of each region of the next biased chain call, where the regions' offsets do not name it (the heat pool's slots); an
//! empty list reads the expert off the region's offset (`MoeGpuFfnForm.e_base` / `e_stride`)
def public set_moe_gpu_ffn_experts(e : array<int64>) {
    g_ffn_experts |> resize(long_length(e))
    for (src, dst in e, g_ffn_experts) {
        dst = src
    }
}

def public clear_moe_gpu_ffn_experts() {
    g_ffn_experts |> clear()
}

//! region r's expert off the list `set_moe_gpu_ffn_experts` set; -1 where the list holds none
def public moe_gpu_ffn_expert(r : int64) : int64 => r < long_length(g_ffn_experts) ? g_ffn_experts[r] : -1l

//! The second activation image of the next expert-chain or deltanet-chain call: the up plane's (the z plane's) own form where its
//! pair with the gate (the qkv) plane takes two activation classes; `on` false = one image feeds both
struct MoeGpuImage2 {
    on : bool
    xq : int8 const?
    xs : float const?
}

var private g_image2 = MoeGpuImage2()

def public set_moe_gpu_image2(xq : array<int8>; xs : array<float>) {
    g_image2 = MoeGpuImage2(on = true, xq = unsafe(addr<int8 const?>(xq[0])), xs = unsafe(addr<float const?>(xs[0])))
}

def public clear_moe_gpu_image2() {
    g_image2 = MoeGpuImage2()
}

def public moe_gpu_image2() : MoeGpuImage2 => g_image2

//! the routed experts' gate and up bias planes, every routed layer's [n_expert x n_ff_exp] rows in layer order; false = refused
typedef MoeGpuFfnBiasFn = function<(gb : array<float>; ub : array<float>) : bool>

[unused_argument(gb, ub)]
def private moe_gpu_unset_ffn_bias(gb : array<float>; ub : array<float>) : bool => false
var g_moe_gpu_ffn_bias = @@moe_gpu_unset_ffn_bias

//! Install the tier's routed bias seat (beside install_moe_gpu_tier): a tier without it keeps a biased model's experts on the CPU
def public set_moe_gpu_ffn_bias_hook(up : MoeGpuFfnBiasFn) {
    g_moe_gpu_ffn_bias = up
}

def public moe_gpu_upload_ffn_bias(gb : array<float>; ub : array<float>) : bool => invoke(g_moe_gpu_ffn_bias, gb, ub)

// ===== the f16-fed (cm2) expert chain: an optional seat beside the FFN hook =====

[unused_argument(goutp, offs1, offs3, offs2, nregions, xp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)]
def private moe_gpu_unset_ffn_xf(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64;
                                 xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64;
                                 f1, f3, f2 : int; is_gelu : bool) {
    panic("dasLLAMA: f16-fed GPU FFN dispatch hit without an installed chain")
}

[unused_argument(npos, n, f1, f3, f2)]
def private moe_gpu_no_ffn_xf(npos, n : int64; f1, f3, f2 : int) : bool => false

var g_moe_gpu_ffn_xf = @@moe_gpu_unset_ffn_xf
var g_moe_gpu_ffn_xf_ok = @@moe_gpu_no_ffn_xf

//! Tier registration of the f16-fed expert chain (beside install_moe_gpu_tier); a tier without
//! one leaves the seat unset and every layer takes the quant batch arm.
def public set_moe_gpu_ffn_xf_hooks(ffn : MoeGpuFfnXfFn; ok : MoeGpuFfnXfOkFn) {
    g_moe_gpu_ffn_xf = ffn
    g_moe_gpu_ffn_xf_ok = ok
}

//! Does the tier serve a layer whose expert triple is (f1, f3, f2) through the f16-fed chain at
//! this window? Asked per layer: the formats decide, and the tier's x plane bounds the window.
def public moe_gpu_ffn_xf_ok(npos, n : int64; f1, f3, f2 : int) : bool =>
    invoke(g_moe_gpu_ffn_xf_ok, npos, n, f1, f3, f2)

// The async twin of the f16-fed chain: begin submits and returns, join waits and reads the
// combined rows. ONE in flight. The streamed-layer split runs its CPU tail between the two.
typedef MoeGpuFfnXfBeginFn = function<(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n : int64; nfe : int64; nrows : int64; npos : int64; f1 : int; f3 : int; f2 : int; is_gelu : bool) : void>
typedef MoeGpuFfnXfJoinFn = function<(var goutp : float?) : void>
typedef MoeGpuFfnXfRowsFn = function<(n : int64; nfe : int64) : int64>

[unused_argument(offs1, offs3, offs2, nregions, xp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)]
def private moe_gpu_unset_ffn_xf_begin(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64;
                                       xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64;
                                       f1, f3, f2 : int; is_gelu : bool) {
    panic("dasLLAMA: async f16-fed GPU FFN begin hit without an installed chain")
}

[unused_argument(goutp)]
def private moe_gpu_unset_ffn_xf_join(var goutp : float?) {
    panic("dasLLAMA: async f16-fed GPU FFN join hit without an installed chain")
}

[unused_argument(n, nfe)]
def private moe_gpu_no_ffn_xf_rows(n, nfe : int64) : int64 => 0l

var g_moe_gpu_ffn_xf_begin = @@moe_gpu_unset_ffn_xf_begin
var g_moe_gpu_ffn_xf_join = @@moe_gpu_unset_ffn_xf_join
var g_moe_gpu_ffn_xf_rows = @@moe_gpu_no_ffn_xf_rows

//! Tier registration of the async f16-fed chain (beside set_moe_gpu_ffn_xf_hooks).
def public set_moe_gpu_ffn_xf_async_hooks(begin : MoeGpuFfnXfBeginFn; join : MoeGpuFfnXfJoinFn; rows : MoeGpuFfnXfRowsFn) {
    g_moe_gpu_ffn_xf_begin = begin
    g_moe_gpu_ffn_xf_join = join
    g_moe_gpu_ffn_xf_rows = rows
}

//! The bucket-row cap of the async arm (one chunk); 0 when no async chain is installed.
def public moe_gpu_ffn_xf_async_rows(n, nfe : int64) : int64 =>
    invoke(g_moe_gpu_ffn_xf_rows, n, nfe)

//! The meet form of the async chain over a streamed layer: open prepares the window and answers whether the layer's
//! experts stream (a resident layer answers false), push queues one region to the device - without `wait` it declines
//! while the ring is full - close ends the chain, and the join above collects; the caller sums the regions it kept.
typedef MoeGpuFfnXfOpenFn = function<(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n : int64; nfe : int64; nrows : int64; npos : int64; f1 : int; f3 : int; f2 : int; is_gelu : bool) : bool>
typedef MoeGpuFfnXfPushFn = function<(r_lo : int64; r_hi : int64; wait : bool) : bool>
typedef MoeGpuFfnXfBundleFn = function<(r_lo : int64; r_max : int64) : int64>
typedef MoeGpuFfnXfCloseFn = function<() : void>

[unused_argument(offs1, offs3, offs2, nregions, xp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)]
def private moe_gpu_unset_ffn_xf_open(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64;
                                      xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64;
                                      f1, f3, f2 : int; is_gelu : bool) : bool => false

[unused_argument(r_lo, r_hi, wait)]
def private moe_gpu_unset_ffn_xf_push(r_lo, r_hi : int64; wait : bool) : bool {
    panic("dasLLAMA: f16-fed GPU FFN push hit without an installed chain")
    return false
}

[unused_argument(r_lo, r_max)]
def private moe_gpu_unset_ffn_xf_bundle(r_lo, r_max : int64) : int64 {
    panic("dasLLAMA: f16-fed GPU FFN bundle hit without an installed chain")
    return 0l
}

def private moe_gpu_unset_ffn_xf_close {
    panic("dasLLAMA: f16-fed GPU FFN close hit without an installed chain")
}

var g_moe_gpu_ffn_xf_open = @@moe_gpu_unset_ffn_xf_open
var g_moe_gpu_ffn_xf_push = @@moe_gpu_unset_ffn_xf_push
var g_moe_gpu_ffn_xf_bundle = @@moe_gpu_unset_ffn_xf_bundle
var g_moe_gpu_ffn_xf_close = @@moe_gpu_unset_ffn_xf_close

def public set_moe_gpu_ffn_xf_meet_hooks(open : MoeGpuFfnXfOpenFn; push : MoeGpuFfnXfPushFn; bundle : MoeGpuFfnXfBundleFn; close : MoeGpuFfnXfCloseFn) {
    g_moe_gpu_ffn_xf_open = open
    g_moe_gpu_ffn_xf_push = push
    g_moe_gpu_ffn_xf_bundle = bundle
    g_moe_gpu_ffn_xf_close = close
}

//! Runtime routing gate over the heat cache (the --heat-ab in-process lever; pins/uploads stay).
def public set_moe_gpu_heat_route(on : bool) {
    g_moe_gpu_heat_route = on
}

var g_vk_rdec_rerecord = false

//! Probe lever (--rerecord-ab): re-encode the resident ladder's recorded-once token cmd every
//! token — measures the re-record cost the carrier class twins would pay for push-borne pos/cnt.
def public set_vk_rdec_rerecord(on : bool) {
    g_vk_rdec_rerecord = on
}

def public vk_rdec_rerecord_on : bool => g_vk_rdec_rerecord

// ===== shared-expert residency (decode, async behind the routed experts) =====

var g_moe_gpu_shexp_route = true
var private g_moe_gpu_shexp_said : table<string>   // the prefill's shared-expert decline reasons said for this model

//! The planes the tier holds for one layer's shared-expert triple: each one's format and its offset in that format's plane
//! space - the file's own planes
struct ShexpGpuPlanes {
    f1 : int
    f3 : int
    f2 : int
    o1 : int64
    o3 : int64
    o2 : int64
}

//! Loader contract: mark one layer's shared-expert triple resident on the tier, its planes `p`. Reset per load.
def public set_moe_gpu_shexp_layer(l : int64; p : ShexpGpuPlanes) {
    g_installed_marks.shexp_layers |> insert(l)
    g_installed_marks.shexp_planes |> insert(l, p)
}

//! The planes the tier holds for layer `l`'s shared-expert triple (`set_moe_gpu_shexp_layer`); a layer with no mark reads none
def public moe_gpu_shexp_planes(l : int64) : ShexpGpuPlanes => g_installed_marks.shexp_planes?[l] ?? ShexpGpuPlanes(o1 = -1l, o3 = -1l, o2 = -1l)

def public moe_gpu_shexp_on_gpu(l : int64) : bool =>
    g_moe_gpu_installed && g_moe_gpu_shexp_route && key_exists(g_installed_marks.shexp_layers, l)

//! The prefill's shared expert stays on the CPU for `why`: said once per reason per model while the tier is armed.
[cold_path]
def public moe_gpu_shexp_declined(why : string) {
    return if (!g_moe_gpu_installed)
    gpu_said_once(g_moe_gpu_shexp_said, LOG_INFO, "the prefill's shared expert takes the CPU rail - ", why)
}

//! `why` logged at `level` once under `said` (a reason set the owner clears when it re-arms), true when this call said it;
//! the check every call pays is the lookup alone
def public gpu_said_once(var said : table<string>; level : int; prefix, why : string) : bool {
    return false if (key_exists(said, why))
    gpu_say_once_now(said, level, prefix, why)
    return true
}

[cold_path]
def private gpu_say_once_now(var said : table<string>; level : int; prefix, why : string) {
    said |> insert(why)
    to_log(level, "dasLLAMA: {prefix}{why} (said once per reason)\n")
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

//! Tier registration of the decode q/k/v group (beside install_moe_gpu_tier).
def public set_moe_gpu_qkv_hook(f : MoeGpuQkvFn) {
    g_moe_gpu_qkv = f
}

//! Loader contract: mark one attention layer's q/k/v triple resident. Reset per load.
def public set_moe_gpu_qkv_layer(l : int64) {
    g_installed_marks.qkv_layers |> insert(l)
}

def public moe_gpu_qkv_on_gpu(l : int64) : bool =>
    g_moe_gpu_installed && key_exists(g_installed_marks.qkv_layers, l)

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
    shexp_planes : table<int64; ShexpGpuPlanes>
    qkv_layers : table<int64>
    unsupported : string
}

var private g_installed_marks = MoeGpuMarks()

//! Move the installed marks out into `m`; the globals read as no-marks after (routing gates —
//! the process-wide A/B levers — stay put).
def public moe_gpu_marks_save(var m : MoeGpuMarks) {
    delete m
    m <- g_installed_marks
    delete g_installed_marks
    g_installed_marks <- MoeGpuMarks()
}

//! Does a mark set carry any armed state? A multi-model host uses this to attribute a
//! load's arm outcome to the slot that captured it — the process-global VRAM counter cannot
//! tell WHOSE planes are resident.
def public moe_gpu_marks_armed(m : MoeGpuMarks) : bool {
    return (!empty(m.dense_offs) || !empty(m.dn_layers) || !empty(m.attn_layers)
        || !empty(m.shexp_layers) || !empty(m.qkv_layers) || m.cls_off >= 0l
        || (m.total_layers > 0l && (m.from_layer < m.total_layers || m.stream_from < m.total_layers)))
}

//! Do the INSTALLED marks carry any armed state? A load asks before installing its model: a
//! model deleted without the drop leaves its marks and its device state behind.
def public moe_gpu_installed_marks_armed() : bool => moe_gpu_marks_armed(g_installed_marks)

//! Install `m` as the global marks (moves the tables in; `m` reads as no-marks after). The
//! installed set is released first: nothing enforces the save/restore pairing.
def public moe_gpu_marks_restore(var m : MoeGpuMarks) {
    delete g_installed_marks
    g_installed_marks <- m
    delete m
    m <- MoeGpuMarks()
}

//! Install a no-marks set for a load of `total` layers whose resident and streamed ranges start at `from`, the asked layer count kept
def public reset_moe_gpu_marks(from, total : int64) {
    let layer_count = g_installed_marks.layer_count
    delete g_installed_marks
    g_installed_marks <- MoeGpuMarks(layer_count = layer_count, from_layer = from, stream_from = from, total_layers = total)
    g_moe_gpu_shexp_said |> clear()
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
//! Callers use dasllama_common's ``moe_gpu_drop_model_``, which also resets the marks.
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
    invoke(g_moe_gpu_heat_advise, l, b1, b3, b2, f1, f3, f2, n, nfe, unsafe(addr<int64 const?>(ids[0])), nids)
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
    invoke(g_moe_gpu_ffn_join, unsafe(addr(gout[0])))
}

// ===== GPU tier programmatic config + status =====
// The setter mirror of the DASLLAMA_GPU_* env vars (env stays the override) plus the status
// surface dashboards read — what is actually resident after a load, not what was requested.

//! Programmatic GPU tier request — the config-file mirror of the ``DASLLAMA_GPU_*`` env vars
//! (an env var present in the environment overrides its field). Set BEFORE ``load_model``,
//! then call ``moe_gpu_tier_arm_`` so a backend can install its hooks.
struct GpuTierWant {
    auto_tier : bool     //!< request the measured-best rail set, the whole-model driver included; individual knobs below still override
    resident : bool      //!< ask for the whole-model driver on a model that fits the card beside rails spelled out one by one (``auto_tier`` implies it; ``DASLLAMA_GPU_RESIDENT=0`` still keeps the rails)
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
var g_moe_gpu_backend = ""

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
def private moe_gpu_no_os_memory() : tuple<budget : int64; usage : int64; adapter_dedicated : int64; adapter_shared : int64> => (budget = 0l, usage = 0l, adapter_dedicated = 0l, adapter_shared = 0l)
[unused_argument(bytes)]
def private rdec_no_note_weight_bytes(bytes : int64) {}
var g_moe_gpu_os_memory = @@moe_gpu_no_os_memory
var g_rdec_note_weight_bytes = @@rdec_no_note_weight_bytes

def set_gpu_tier_want_(want : GpuTierWant) {
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
        g_noisy = g_env_engine.noisy ? 1 : 0
    }
    return g_noisy == 1
}

//! One switch for the measured-best rail set: expert stacks sized to the VRAM budget, plus the
//! deltanet/attention/dense/shared-expert/classifier rails. qkv and heat stay off (measured
//! negative and a wash). Any DASLLAMA_GPU_* knob still overrides individually.
def public gpu_want_auto : bool
    => g_env_gpu.gpu ?? g_gpu_want.auto_tier

[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan")]
def public gpu_resident_requested : bool
    => gpu_want_auto() || g_gpu_want.resident

//! The whole-model driver will be attempted on this box: a bake's walk lays the driver's arena in place of the
//! per-op rails' planes, and the vulkan image identity carries the same bit.
def public gpu_want_dn_native : bool
    => moe_gpu_resident_installed() && gpu_resident_requested()

//! Effective tier request, env-else-programmatic — the load walk and the GPU backends read
//! these instead of the raw env vars.

// -1 = auto: the upload walk resolves it against n_layers and lets its budget stops place the split
def public gpu_want_moe_layers : int64
    => (g_env_gpu.gpu_moe_layers
        ?? (g_gpu_want.moe_layers != 0l ? g_gpu_want.moe_layers : (gpu_want_auto() ? -1l : 0l)))

def public gpu_want_moe_stream : int64
    => (g_env_gpu.gpu_moe_stream
        ?? (g_gpu_want.moe_stream != 0l ? g_gpu_want.moe_stream : (gpu_want_auto() ? -1l : 0l)))

def public gpu_want_vram_mb : int64
    => g_env_gpu.gpu_vram_mb ?? g_gpu_want.vram_mb

def public gpu_want_dn : bool
    => g_env_gpu.gpu_dn ?? (g_gpu_want.dn || gpu_want_auto())

def public gpu_want_attn : bool
    => g_env_gpu.gpu_attn ?? (g_gpu_want.attn || gpu_want_auto())

def public gpu_want_dense : bool
    => g_env_gpu.gpu_dense ?? (g_gpu_want.dense || gpu_want_auto())

//! Dense-rail sub-arm for gemma-4's parallel dense shared expert (moe_dense_shexp) w1/w3/w2 triple.
//! Opt-IN: MEASURED a wash-to-loss (3 dispatches each paying acts-up + y-down around a CPU gate)
//! and the uploads displace 1-2 expert layers. Kept as the upload half of a future FUSED chain.
def public gpu_want_dense_shexp : bool
    => g_env_gpu.gpu_dense_shexp

//! The dense rail's attention half (per-layer q/o pairs). Opt-OUT (``DASLLAMA_GPU_DENSE_ATTN=0``)
//! — measured a LOSS on gemma-4 where the shexp triple is a large win, so the two halves need
//! independent control rather than one DENSE gate.
def public gpu_want_dense_attn : bool
    => g_env_gpu.gpu_dense_attn

//! The dense rail's FFN half (each dense layer's w1/w3/w2 triple, the decode row's FFN on the device). On by
//! default (``DASLLAMA_GPU_DENSE_FFN=0`` keeps it on the CPU): the triples are most of a dense model's bytes, placed
//! ahead of the rails the walk reaches after them
[arch(at="../ARCHITECTURE_GPU_VULKAN_PEROP.md#perop-dense-ffn")]
def public gpu_want_dense_ffn : bool
    => g_dense_ffn_pin >= 0 ? g_dense_ffn_pin == 1 : g_env_gpu.gpu_dense_ffn

var private g_dense_ffn_pin = -1   //! `set_gpu_want_dense_ffn`'s pin: -1 reads the environment

//! the in-process twin of ``DASLLAMA_GPU_DENSE_FFN``, read by the next load's walk; -1 lifts the pin
def public set_gpu_want_dense_ffn(pin : int) {
    g_dense_ffn_pin = pin
    if (pin >= 0) {
        let home = pin == 1 ? "on the device" : "on the CPU"
        let knob = "set_gpu_want_dense_ffn({pin})"
        to_log(LOG_INFO, "dasLLAMA GPU tier: {knob} - the next load's dense FFNs run {home}\n")
    }
}

//! the pin `set_gpu_want_dense_ffn` holds, -1 where none
def public gpu_dense_ffn_pin() : int => g_dense_ffn_pin

//! The streamed-layer split want (``DASLLAMA_GPU_MOE_SPLIT``): it decides the stream slot count
//! the carve reserves, so the image identity carries it.
def public gpu_want_moe_split : bool
    => g_env_gpu.gpu_moe_split

var private g_pf_stream_pin = -1   //! `set_gpu_want_pf_stream`'s pin: -1 reads the environment

//! The streamed prompt want (`DASLLAMA_GPU_PF_STREAM`): a routed layer whose experts serve on the host streams its stack to the device for the prompt; the pin is a fixture's in-process twin set before its load, -1 lifts it
def public gpu_want_pf_stream : bool
    => g_pf_stream_pin >= 0 ? g_pf_stream_pin == 1 : g_env_gpu.gpu_pf_stream

def public set_gpu_want_pf_stream(pin : int) {
    g_pf_stream_pin = pin
}

//! the pin as it was written (-1 = none): a fixture puts back what it found
def public gpu_want_pf_stream_pin : int => g_pf_stream_pin

var private g_hot_share_pin = -1.0   //! `set_gpu_hot_share`'s pin: negative reads the environment

//! the DMA fill's ungated share of a decode step's misses (`DASLLAMA_GPU_HOT_SHARE`); the pin is a fixture's in-process twin
def public gpu_hot_share : float => g_hot_share_pin >= 0.0 ? g_hot_share_pin : g_env_gpu.gpu_hot_share

def public set_gpu_hot_share(pin : float) {
    g_hot_share_pin = pin
    if (pin >= 0.0) {
        to_log(LOG_INFO, "dasLLAMA GPU tier: set_gpu_hot_share({pin}) - the DMA fill admits that share of a decode step's pool misses past the heat gate{gpu_hot_dma() ? "" : " (ignored: the DMA fill is off, DASLLAMA_GPU_HOT_DMA / set_gpu_hot_dma)"}\n")
    }
}

def public gpu_hot_share_pin : float => g_hot_share_pin

var private g_hot_dma_pin = -1    //! `set_gpu_hot_dma`'s pin: -1 reads the environment
var private g_dec_meet_pin = -1   //! `set_gpu_dec_meet`'s pin: -1 reads the environment

//! the pool's DMA fill (`DASLLAMA_GPU_HOT_DMA`): a decode step's misses copied into the pool on the transfer queue, a prompt window's hits served off the pool first; the pin is a fixture's in-process twin, -1 lifts it
def public gpu_hot_dma : bool => g_hot_dma_pin >= 0 ? g_hot_dma_pin == 1 : g_env_gpu.gpu_hot_dma

def public set_gpu_hot_dma(pin : int) {
    g_hot_dma_pin = pin
}

def public gpu_hot_dma_pin : int => g_hot_dma_pin

//! the decode meet (`DASLLAMA_GPU_DEC_MEET`): a decode token's picks on a streamed layer claimed through the prefill's meet; the pin is a fixture's in-process twin, -1 lifts it
def public gpu_dec_meet : bool => g_dec_meet_pin >= 0 ? g_dec_meet_pin == 1 : g_env_gpu.gpu_dec_meet

def public set_gpu_dec_meet(pin : int) {
    g_dec_meet_pin = pin
}

def public gpu_dec_meet_pin : int => g_dec_meet_pin

def public gpu_want_dnd : bool
    => g_env_gpu.gpu_dnd ?? (g_gpu_want.dnd || gpu_want_auto())

def public gpu_want_heat : int64
    => g_env_gpu.gpu_heat ?? g_gpu_want.heat

def public gpu_want_shexp : bool
    => g_env_gpu.gpu_shexp ?? (g_gpu_want.shexp || gpu_want_auto())

def public gpu_want_qkv : bool
    => g_env_gpu.gpu_qkv ?? g_gpu_want.qkv

//! Arm-intent for the classifier plane. The UPLOAD is opt-OUT (DASLLAMA_GPU_CLS=0 declines it), so
//! this is only the stronger "=1 arms the tier by itself" signal — letting the classifier offload be
//! measured without also offloading expert layers. Unset keeps today's behavior exactly.
def public gpu_want_cls : bool
    => g_env_gpu.gpu_cls ?? false

//! The classifier UPLOAD gate (opt-OUT, default on when the tier is armed) — distinct from
//! [[gpu_want_cls]], the opt-IN arm signal. One definition, shared by the upload walk and the
//! DlimConfiguration identity, so the two can never disagree on the default again.
def public gpu_want_cls_upload : bool
    => g_env_gpu.gpu_cls ?? true

//! The ONE truth for "did the caller request rails the vulkan tier arms on" — the arm gate and
//! the loud-failure checks share it, so they can never drift. -1 moe_layers = auto, a REQUEST.
//! Excludes what cannot arm alone: dense + its sub-gates (post-arm routing), the vram budget.
def public gpu_want_arms_tier : bool
    => (gpu_want_moe_layers() != 0l || gpu_want_moe_stream() != 0l
        || gpu_want_dn() || gpu_want_attn() || gpu_want_dnd()
        || gpu_want_heat() > 0l || gpu_want_shexp() || gpu_want_qkv() || gpu_want_cls())

//! A GPU backend registers its arm probe here at ``[init]`` so config-driven arming works
//! after module init (the env path arms at ``[init]`` directly).
def public set_moe_gpu_arm(name : string; f : function<() : bool>) {
    g_moe_gpu_backend = name
    g_moe_gpu_arm = f
}

//! False = no GPU backend module ever registered an arm probe — distinct from registered-but-
//! declined (device probe/VRAM); the loud-failure checks use it to say WHICH failure happened.
def public moe_gpu_backend_registered : bool => !empty(g_moe_gpu_backend)

//! the discrete home that registered the arm ("vulkan" | "cuda"), "" with none: the bench row's backend stamp and the lane's name in the rail's lines
def public moe_gpu_backend_name : string => g_moe_gpu_backend

//! the PCIe class, 40 GB/s: a first decode token slower than the resident weights at that rate ran paged
let RDEC_PAGING_BYTES_PER_US = 40_000l

//! the first decode token's wall `wall_us` read against the plan's `weight_bytes` on the device: under the paging class, one warning
//! naming the serving home - the resident drivers' warm-up check, once an armed driver
[cold_path]
def rdec_paging_check(weight_bytes, wall_us : int64) {
    if (weight_bytes > 0l && wall_us > weight_bytes / RDEC_PAGING_BYTES_PER_US) {
        to_log(LOG_WARNING, "dasLLAMA {moe_gpu_backend_name()} resident: a decode token ran under {RDEC_PAGING_BYTES_PER_US / 1_000l} GB/s over {weight_bytes / 1_000_000l} MB of resident weights - the paging class; close other GPU applications or lower DASLLAMA_GPU_VRAM_MB\n")
    }
}

//! the resident drivers' speculative-verify decline for `why`, under the serving home's name: the CPU verify serves
[cold_path]
def rdec_v_decline(why : string) : bool {
    to_log(LOG_WARNING, "dasLLAMA {moe_gpu_backend_name()} resident: the speculative verify has no same-slab form - {why}; the CPU verify serves\n")
    return false
}

//! the N-row command's 0 rows for `why`, said once per armed model through the driver's `said` latch: a batched step runs a row at
//! a time from here on
def rdec_n_decline(var said : bool&; why : string) : int64 {
    if (!said) {
        said = true
        to_log(LOG_WARNING, "dasLLAMA {moe_gpu_backend_name()} resident: the N-row token command has no form for {why} - a batched step runs a row at a time\n")
    }
    return 0l
}

//! the greedy chain's mode knob both discrete homes keep: a pin at `RDEC_CHAIN_ENV` reads the environment's value (0 off, 1 every
//! greedy step, -1 adaptive), and the adaptive mode's run of unchained steps after a miss doubles to `RDEC_CHAIN_BACKOFF_MAX`
let RDEC_CHAIN_ENV = -2
let RDEC_CHAIN_BACKOFF_MAX = 64

//! the chain's mode: the `pin`, else the environment's `env`
def rdec_chain_mode(pin, env : int) : int => pin != RDEC_CHAIN_ENV ? pin : env

//! the adaptive mode's backoff after a miss: the run doubles from one to the cap, and the steps left take the run
def rdec_chain_backoff(var backoff_len : int&; var backoff : int&) {
    backoff_len = clamp(backoff_len * 2, 1, RDEC_CHAIN_BACKOFF_MAX)
    backoff = backoff_len
}

//! One loud line when GPU rails are requested but no backend module ever registered — the
//! ``require ?vulkan`` guard makes a failed shared_module load VANISH otherwise. Log-only;
//! the model-load rail calls it once per load.
def public warn_gpu_backend_absent {
    if (!gpu_want_arms_tier() || moe_gpu_backend_registered()) {
        return
    }
    let pending = describe_pending_dynamic_modules()
    let detail = (empty(pending) ? "  no GPU backend module is installed (dasVulkan under modules/?)"
        : "  shared modules that FAILED to load:\n{pending}")
    to_log(LOG_ERROR, "dasLLAMA: GPU rails requested (DASLLAMA_GPU*) but NO GPU backend module is registered - the run stays on the CPU\n{detail}\n")
}

//! (Re-)arm the GPU tier from the current want. False = no GPU backend in this build,
//! nothing requested, or the device probe failed.
def moe_gpu_tier_arm_() : bool => invoke(g_moe_gpu_arm)

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
def moe_gpu_weight_budget_() : int64 => invoke(g_moe_gpu_budget_report)

def private moe_gpu_no_carve() : int64 => 0l

var private g_moe_gpu_carve_report = @@moe_gpu_no_carve

//! the bytes the tier carves off its cap before the weight budget: the streamed slot and the decode block's mirrors
def public moe_gpu_weight_carve_() : int64 => invoke(g_moe_gpu_carve_report)

def public set_moe_gpu_carve_fn(f : function<() : int64>) {
    g_moe_gpu_carve_report = f
}

//! The OS's video memory numbers, in bytes (all 0 = no OS answer): this process's dedicated ceiling,
//! what it holds, what every process holds on the adapter, the system memory the adapter maps - the
//! resident plan's auto arm sizes against the room left where a backend installs the query.
def moe_gpu_os_memory() : tuple<budget : int64; usage : int64; adapter_dedicated : int64; adapter_shared : int64> => invoke(g_moe_gpu_os_memory)

def public install_moe_gpu_os_memory(f : function<tuple<budget : int64; usage : int64; adapter_dedicated : int64; adapter_shared : int64>>) {
    g_moe_gpu_os_memory = f
}

//! Tell the resident driver how many weight bytes its plan put on the device (its warm-up
//! self-check reads the decode wall against them).
def rdec_note_weight_bytes(bytes : int64) { invoke(g_rdec_note_weight_bytes, bytes) }

def public install_rdec_note_weight_bytes(f : function<(bytes : int64) : void>) {
    g_rdec_note_weight_bytes = f
}

//! the N-row token command: `nrows` rows of x, cos/sin and the per-layer-embedding side input (empty where the model has none; `ple_raw` says the rows are the gathered table rows the device projects, not the finished pre-step; a row with its token id in `ids` brings no side row - the device gathers it off the id), each row's position, cached count and mirror region, each row's
//! logits landed at its own `lrows` pointer (a session's logits row) - or, where the pointer is null, the row's argmax id alone into `picks` (-1 on a landed row); two rows at least - a one-row step is the one-row command's
typedef RdecTokenNFn = function<(x : array<float>; cossin : array<float>; ple : array<float>; ple_raw : bool; ids : array<int64>; poss : array<int64>; cnts : array<int64>; regs : array<int64>; nrows : int64; lrows : array<float?>; var picks : array<int64>) : void>

[unused_argument(x, cossin, ple, ple_raw, ids, poss, cnts, regs, nrows, lrows, picks)]
def private rdec_unset_token_n(x : array<float>; cossin : array<float>; ple : array<float>; ple_raw : bool; ids : array<int64>; poss : array<int64>; cnts : array<int64>; regs : array<int64>; nrows : int64; lrows : array<float?>; var picks : array<int64>) {
    panic("dasLLAMA: resident N-row token hit without an installed GPU driver - require dasllama/dasllama_math_vulkan (the Vulkan tier installs it)")
}
def private rdec_no_token_n_rows : int64 => 0l
var private g_rdec_token_n = @@rdec_unset_token_n
var private g_rdec_token_n_rows = @@rdec_no_token_n_rows

//! Install the N-row token command beside the resident driver: `rows` answers how many rows it steps at once on the armed model (0 = none)
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def public install_moe_gpu_resident_batch(token_n : RdecTokenNFn; rows : function<() : int64>) {
    g_rdec_token_n = token_n
    g_rdec_token_n_rows = rows
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def public rdec_token_n(x : array<float>; cossin : array<float>; ple : array<float>; ple_raw : bool; ids : array<int64>; poss : array<int64>; cnts : array<int64>; regs : array<int64>; nrows : int64; lrows : array<float?>; var picks : array<int64>) {
    invoke(g_rdec_token_n, x, cossin, ple, ple_raw, ids, poss, cnts, regs, nrows, lrows, picks)
    g_rdec_nrow_steps++
    g_rdec_nrow_rows += nrows
}

//! the rows the armed driver's N-row command steps at once; 0 where the model or the driver has none
def public rdec_token_n_rows() : int64 => invoke(g_rdec_token_n_rows)

var private g_rdec_nrow_steps = 0l
//! the N-row commands served this process - one a group of a batched step, one a peeled window: a cell reads the difference across its steps
def public rdec_nrow_steps() : int64 => g_rdec_nrow_steps

var private g_rdec_nrow_rows = 0l
//! the rows the N-row commands carried this process: its difference over `rdec_nrow_steps`' is a command's row count
def public rdec_nrow_rows() : int64 => g_rdec_nrow_rows

[unused_argument(regions)]
def private moe_gpu_no_binding_cap(regions : int64) : int64 => 0l
var private g_moe_gpu_binding_cap = @@moe_gpu_no_binding_cap

//! The tier's largest legal single-SSBO binding range in bytes for planes shared by `regions` K/V mirror
//! regions (0 = no tier / unknown). The resident KV mirror binds one range per slab a side, every region
//! carrying its own slack past its planes, so the ctx negotiation must cap against it.
def public set_moe_gpu_binding_cap(f : function<(regions : int64) : int64>) {
    g_moe_gpu_binding_cap = f
}

def public moe_gpu_binding_cap(regions : int64) : int64 => invoke(g_moe_gpu_binding_cap, regions)

def private moe_gpu_no_mirror_slack : int64 => 0l
var private g_moe_gpu_mirror_slack = @@moe_gpu_no_mirror_slack

//! The elements a K/V mirror slab allocates past its planes a region (a flash fragment's over-read, the kernels' slack);
//! 0 = no tier. The plan charges it with every slab, as the driver's prepare allocates it.
def public set_moe_gpu_mirror_slack(f : function<() : int64>) {
    g_moe_gpu_mirror_slack = f
}

def public moe_gpu_mirror_slack() : int64 => invoke(g_moe_gpu_mirror_slack)

//! the env-else-programmatic wants half of the discrete tier's config section (the caps half is the device's, or the dry
//! config's): both homes' live sources and the dry arm's verify fill it here, so the tag they fold agrees on every knob
def public gpu_dlim_wants_fill(var v : DlimVulkanConfig) {
    v.moe_layers = gpu_want_moe_layers()
    v.moe_stream = gpu_want_moe_stream()
    v.dn = gpu_want_dn()
    v.attn = gpu_want_attn()
    v.dnd = gpu_want_dnd()
    v.shexp = gpu_want_shexp()
    v.qkv = gpu_want_qkv()
    v.cls = gpu_want_cls_upload()   // the walk's gate, not the arm intent
    v.dense = gpu_want_dense()
    v.dense_attn = gpu_want_dense_attn()
    v.dense_shexp = gpu_want_dense_shexp()
    v.dense_ffn = gpu_want_dense_ffn()
    v.trim = vulkan_trim()
    v.split = false
    v.dn_native = gpu_want_dn_native()
}

//! the dry bake's config against this process' wants, on `home`'s entry: the walk's gates read the env-else-programmatic wants, so
//! a config disagreeing with them would write a lane whose identity lies about its plan - fail closed, both tags named; the trim
//! shapes the save, not the walk, so the config's stands
def dlim_dry_wants_ok(v : DlimVulkanConfig; home : string) : bool {
    var w = v
    gpu_dlim_wants_fill(w)
    w.trim = v.trim
    if (dlim_vulkan_tag(w) != dlim_vulkan_tag(v)) {
        to_log(LOG_ERROR, "dasLLAMA {home} tier: dry-bake config disagrees with this process' wants (set the DASLLAMA_GPU_* knobs to match)\n  config {dlim_vulkan_tag(v)}\n  wants  {dlim_vulkan_tag(w)}\n")
        return false
    }
    return true
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
def private moe_gpu_no_slice_window(woff : int64; fmt : int) : MoeGpuSliceWindow =>
    default<MoeGpuSliceWindow>

var private g_moe_gpu_slice_window = @@moe_gpu_no_slice_window

//! The loader registers its slice-window provider (populated while slicing a flavor image);
//! a GPU tier asks through [[moe_gpu_slice_window]] to import instead of copying.
def public set_moe_gpu_slice_window_fn(f : function<(woff : int64; fmt : int) : MoeGpuSliceWindow>) {
    g_moe_gpu_slice_window = f
}

def public moe_gpu_slice_window(woff : int64; fmt : int) : MoeGpuSliceWindow => invoke(g_moe_gpu_slice_window, woff, fmt)

//! A baked-plan stream gather's place in the image FILE: the path and the absolute byte offsets of its two planes, so a
//! streamed stack is read straight into its pinned memory with no mapping touched. path == "" = no source for (woff, fmt).
typedef MoeGpuStreamSource = tuple<path : string; qoff : int64; qbytes : int64; soff : int64; sbytes : int64>

[unused_argument(woff, fmt)]
def private moe_gpu_no_stream_source(woff : int64; fmt : int) : MoeGpuStreamSource =>
    default<MoeGpuStreamSource>

var private g_moe_gpu_stream_source = @@moe_gpu_no_stream_source

//! The loader registers its stream-source provider (populated while slicing a flavor image), which [[moe_gpu_stream_source]] asks
def public set_moe_gpu_stream_source_fn(f : function<(woff : int64; fmt : int) : MoeGpuStreamSource>) {
    g_moe_gpu_stream_source = f
}

def public moe_gpu_stream_source(woff : int64; fmt : int) : MoeGpuStreamSource => invoke(g_moe_gpu_stream_source, woff, fmt)

//! both loader provider seats hold a registered provider, not their declared default (the home switch's witness)
def public moe_gpu_loader_seats_installed() : bool =>
    !(g_moe_gpu_slice_window == @@moe_gpu_no_slice_window) && !(g_moe_gpu_stream_source == @@moe_gpu_no_stream_source)

//! One expert of a streamed stack as the expert cache holds it: the slot's quant and scale pointers for the host's reads, the
//! slot's buffer handles and offsets for the DMA; `present` false where the expert has no slot (resident or ring)
typedef MoeGpuStreamExpert = tuple<q : uint8 const?; s : uint8 const?; qbuf : uint64; qoff : int64; sbuf : uint64; soff : int64; present : bool>

[unused_argument(woff, fmt, e)]
def private moe_gpu_no_stream_expert(woff : int64; fmt : int; e : int64) : MoeGpuStreamExpert => default<MoeGpuStreamExpert>

[unused_argument(woff, fmt, e)]
def private moe_gpu_no_stream_fetch(woff : int64; fmt : int; e : int64) : bool => false

//! A streamed stack as the expert cache lays it out, for a reader that captures it in the main context and resolves experts
//! on a job lane (a lane holds every global as zero, so no hook is callable there): the resident slots' planes and the slot
//! table (`slot_of[e]`: a resident slot, `STREAM_RING_BASE` + a ring slot, or -1), the ring's planes of the stack's kind
typedef MoeGpuStreamView = tuple<q : uint8 const?; s : uint8 const?; slot_of : int const?; nexp : int64; qlen : int64; slen : int64; qoff : int64; soff : int64; ring_q : uint8 const?; ring_s : uint8 const?; ring_qlen : int64; ring_slen : int64>

//! the page a direct lane read aligns to: a ring slot holds its expert's span widened to it, the expert's bytes `(plane offset + e * plane stride) % STREAM_PAGE` in
let public STREAM_PAGE = 4096l

//! the lane span of expert `e`'s plane at `off` + e * `len`, widened to the page: a direct read lands it whole in one aligned call
//! (an unaligned span goes through the handle's bounce band a piece at a time, three times the cost), and the expert's bytes
//! start `skew` into the slot
def public stream_page_span(off, len, e : int64) : tuple<aoff : int64; alen : int64; skew : int64> {
    let o = off + e * len
    let aoff = o & ~(STREAM_PAGE - 1l)
    return (aoff = aoff, alen = ((o + len + STREAM_PAGE - 1l) & ~(STREAM_PAGE - 1l)) - aoff, skew = o - aoff)
}

let STREAM_RING_BASE = 1 << 30   //! `slot_of` values from here name a ring slot of the expert cache's transient ring

[unused_argument(woff, fmt)]
def private moe_gpu_no_stream_view(woff : int64; fmt : int) : MoeGpuStreamView => default<MoeGpuStreamView>

var private g_moe_gpu_stream_view = @@moe_gpu_no_stream_view

//! the streamed stack at (`woff`, `fmt`) as the cache lays it out; `slot_of` null where none sits there
def public moe_gpu_stream_view(woff : int64; fmt : int) : MoeGpuStreamView => invoke(g_moe_gpu_stream_view, woff, fmt)

def public set_moe_gpu_stream_view_fn(f : function<(woff : int64; fmt : int) : MoeGpuStreamView>) {
    g_moe_gpu_stream_view = f
}

[unused_argument(order)]
def private moe_gpu_no_stream_cache_order(order : array<int>) {}

var private g_moe_gpu_stream_cache_order = @@moe_gpu_no_stream_cache_order

//! the next streamed layers' experts by heat, the hottest first: a partial stack keeps its quota off this list's head; empty puts the id order back
def public moe_gpu_stream_cache_order(order : array<int>) => invoke(g_moe_gpu_stream_cache_order, order)

def public set_moe_gpu_stream_cache_order_fn(f : function<(order : array<int>) : void>) {
    g_moe_gpu_stream_cache_order = f
}

[unused_argument(copies, npool, l, woff1, fmt1)]
def private moe_gpu_no_xf_pool_window(copies : int const?; npool, l, woff1 : int64; fmt1 : int) {}

var private g_moe_gpu_xf_pool_window = @@moe_gpu_no_xf_pool_window

//! the next per-expert window's first `npool` regions sit on layer `l`'s pool: `copies[r]` the expert region `r`'s slot takes off the stack at `woff1` before its bundle, -1 a hit; null puts the stream form back
def public moe_gpu_xf_pool_window(copies : int const?; npool, l, woff1 : int64; fmt1 : int) => invoke(g_moe_gpu_xf_pool_window, copies, npool, l, woff1, fmt1)

def public set_moe_gpu_xf_pool_window_fn(f : function<(copies : int const?; npool, l, woff1 : int64; fmt1 : int) : void>) {
    g_moe_gpu_xf_pool_window = f
}

[unused_argument(on)]
def private moe_gpu_no_mint_dry(on : bool) {}

var private g_moe_gpu_mint_dry = @@moe_gpu_no_mint_dry

//! the live tier's walk run dry for the mint - no pinned stack, no device buffer, no upload; off again before the lane is mapped and the model armed
def public moe_gpu_mint_dry(on : bool) => invoke(g_moe_gpu_mint_dry, on)

def public set_moe_gpu_mint_dry_fn(f : function<(on : bool) : void>) {
    g_moe_gpu_mint_dry = f
}

def private moe_gpu_no_host_bytes : int64 => 0l

var private g_moe_gpu_host_bytes = @@moe_gpu_no_host_bytes

//! the bytes of host-visible buffers the tier holds now (pinned stacks, the ring, staging); 0 with no tier
def public moe_gpu_host_bytes : int64 => invoke(g_moe_gpu_host_bytes)

def public set_moe_gpu_host_bytes_fn(f : function<() : int64>) {
    g_moe_gpu_host_bytes = f
}

def private moe_gpu_no_stream_cache_stats : tuple<fetches : int64; us : int64> => (fetches = 0l, us = 0l)

var private g_moe_gpu_stream_cache_stats = @@moe_gpu_no_stream_cache_stats

//! the expert cache's on-demand reads so far (a plane each) and the time waited on them
def public moe_gpu_stream_cache_stats : tuple<fetches : int64; us : int64> => invoke(g_moe_gpu_stream_cache_stats)

def public set_moe_gpu_stream_cache_stats_fn(f : function<() : tuple<fetches : int64; us : int64>>) {
    g_moe_gpu_stream_cache_stats = f
}

[unused_argument(nexp, quota)]
def private moe_gpu_no_stream_cache_layer(nexp, quota : int64) {}

[unused_argument(slots, path)]
def private moe_gpu_no_stream_cache_ring(slots : int64; path : string) {}

def private moe_gpu_no_stream_ring_release {}

var private g_moe_gpu_stream_expert = @@moe_gpu_no_stream_expert
var private g_moe_gpu_stream_fetch = @@moe_gpu_no_stream_fetch
var private g_moe_gpu_stream_cache_layer = @@moe_gpu_no_stream_cache_layer
var private g_moe_gpu_stream_cache_ring = @@moe_gpu_no_stream_cache_ring
var private g_moe_gpu_stream_ring_release = @@moe_gpu_no_stream_ring_release

//! the expert cache's seats: the expert lookup, the on-demand fetch off the lane, the per-layer quota, the ring and its release
def public set_moe_gpu_stream_cache_fns(expert : function<(woff : int64; fmt : int; e : int64) : MoeGpuStreamExpert>;
                                        fetch : function<(woff : int64; fmt : int; e : int64) : bool>;
                                        layer : function<(nexp, quota : int64) : void>;
                                        ring : function<(slots : int64; path : string) : void>;
                                        release : function<() : void>) {
    g_moe_gpu_stream_expert = expert
    g_moe_gpu_stream_fetch = fetch
    g_moe_gpu_stream_cache_layer = layer
    g_moe_gpu_stream_cache_ring = ring
    g_moe_gpu_stream_ring_release = release
}

//! expert `e` of the streamed stack at (`woff`, `fmt`) as the cache holds it; `present` false with no slot
def public moe_gpu_stream_expert(woff : int64; fmt : int; e : int64) : MoeGpuStreamExpert => invoke(g_moe_gpu_stream_expert, woff, fmt, e)

//! expert `e` of the streamed stack at (`woff`, `fmt`) read off the lane into a ring slot where it has none, the call waiting for the read; true with a slot after
def public moe_gpu_stream_fetch(woff : int64; fmt : int; e : int64) : bool => invoke(g_moe_gpu_stream_fetch, woff, fmt, e)

[unused_argument(woff, fmt, e)]
def private moe_gpu_no_stream_request(woff : int64; fmt : int; e : int64) : bool => false

var private g_moe_gpu_stream_request = @@moe_gpu_no_stream_request
var private g_moe_gpu_stream_ready = @@moe_gpu_no_stream_request

def public set_moe_gpu_stream_request_fns(request : function<(woff : int64; fmt : int; e : int64) : bool>;
                                          ready : function<(woff : int64; fmt : int; e : int64) : bool>) {
    g_moe_gpu_stream_request = request
    g_moe_gpu_stream_ready = ready
}

//! expert `e` of the streamed stack at (`woff`, `fmt`) queued for a reader thread where it has no slot; true where it will have one
def public moe_gpu_stream_request(woff : int64; fmt : int; e : int64) : bool => invoke(g_moe_gpu_stream_request, woff, fmt, e)

//! whether expert `e` of the streamed stack at (`woff`, `fmt`) sits in a slot the window may read now (a queued read landed)
def public moe_gpu_stream_ready(woff : int64; fmt : int; e : int64) : bool => invoke(g_moe_gpu_stream_ready, woff, fmt, e)

//! the next streamed layer's stacks hold `nexp` experts, `quota` resident (0 = every one): set before the layer's upload
def public moe_gpu_stream_cache_layer(nexp, quota : int64) {
    invoke(g_moe_gpu_stream_cache_layer, nexp, quota)
}

//! the transient ring: `slots` experts a plane kind, read off `path` (the lane file)
def public moe_gpu_stream_cache_ring(slots : int64; path : string) {
    invoke(g_moe_gpu_stream_cache_ring, slots, path)
}

//! every ring slot freed: the window or the token that read them is done
def public moe_gpu_stream_ring_release {
    invoke(g_moe_gpu_stream_ring_release)
}

//! The host's rows core over a streamed stack's device-layout planes - the tier's own block decode run on the CPU:
//! y[rb, re) = the rows' dots with one Q8_K activation row (`xq`, a scale a superblock `xs`) over the quant plane `q` and
//! scale plane `s` of `n`-wide rows; `twin` is the per-lane handle `moe_gpu_host_twin` makes for the format in the main context
typedef MoeGpuHostRowsFn = function<(twin : void?; var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re : int64) : void>
typedef MoeGpuHostTwinFn = function<(fmt : int) : void?>
typedef MoeGpuHostServesFn = function<(fmt : int) : bool>

[unused_argument(twin, yp, q, s, xq, xs, n, rb, re)]
def private moe_gpu_no_host_rows(twin : void?; var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re : int64) {
    panic("dasLLAMA: no host rows core over device planes is installed")
}

[unused_argument(fmt)]
def private moe_gpu_no_host_twin(fmt : int) : void? => null

[unused_argument(fmt)]
def private moe_gpu_no_host_serves(fmt : int) : bool => false

var private g_moe_gpu_host_rows = @@moe_gpu_no_host_rows
var private g_moe_gpu_host_twin = @@moe_gpu_no_host_twin
var private g_moe_gpu_host_serves = @@moe_gpu_no_host_serves
var private g_moe_gpu_host_rows_on = false

def public set_moe_gpu_host_twin_fns(twin : MoeGpuHostTwinFn; rows : MoeGpuHostRowsFn; serves : MoeGpuHostServesFn) {
    g_moe_gpu_host_twin = twin
    g_moe_gpu_host_rows = rows
    g_moe_gpu_host_serves = serves
    g_moe_gpu_host_rows_on = true
}

def public moe_gpu_host_rows_on() : bool => g_moe_gpu_host_rows_on
def public moe_gpu_host_rows_fn() : MoeGpuHostRowsFn => g_moe_gpu_host_rows
def public moe_gpu_host_twin(fmt : int) : void? => invoke(g_moe_gpu_host_twin, fmt)
//! whether the host twin serves the tier's `fmt` (allocates nothing)
def public moe_gpu_host_twin_serves(fmt : int) : bool => invoke(g_moe_gpu_host_serves, fmt)
//! the rows core the twin's `fmt` reads the activation in: Q8_K for a superblock format (a scale a superblock), else Q8_0
def public moe_gpu_host_rows(twin : void?; var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re : int64) {
    invoke(g_moe_gpu_host_rows, twin, yp, q, s, xq, xs, n, rb, re)
}

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

def public moe_gpu_would_accept(n : int64; rows : int64; fmt : int; stream : bool) : bool =>
    invoke(g_moe_gpu_would_accept, n, rows, fmt, stream)

def private moe_gpu_no_bake_tag : string => ""

var private g_moe_gpu_bake_tag = @@moe_gpu_no_bake_tag

//! The GPU tier's config-identity tag for the vulkan-flavor image ("" = tier absent/unarmed —
//! no flavor). Everything that shapes the load-time GPU walk must fold in: a config switch
//! must miss the flavor's identity and rebake.
def public set_moe_gpu_bake_tag(f : function<() : string>) {
    g_moe_gpu_bake_tag = f
}

def public moe_gpu_bake_tag : string => invoke(g_moe_gpu_bake_tag)


//! Device-layout bytes one [rows x n] plane of `fmt` occupies (quants + scales). 0 = no tier.
def public moe_gpu_plane_bytes(n, rows : int64; fmt : int) : int64 => invoke(g_moe_gpu_plane_bytes_fn, n, rows, fmt)

def public moe_gpu_tier_installed : bool => g_moe_gpu_installed

//! Loader contract: total transformer layers of the loaded model (resident/streamed counts).
def public set_moe_gpu_total_layers(n : int64) {
    g_installed_marks.total_layers = n
}

let MOE_GPU_LAND_EXPERTS = 128l   //! the streamed slot as a ring: at most this many experts' planes - the chain's bundles in flight, four of its widest

var private g_moe_gpu_stream_ring = false
var private g_moe_gpu_stream_ring_experts = 0l

//! the experts a ring of `n_expert` holds: every expert of a layer under the cap, the cap past it
def public moe_gpu_ring_experts(n_expert : int64) : int64 => clamp(n_expert, 1l, MOE_GPU_LAND_EXPERTS)

//! the streamed slot is the per-expert chain's ring of `experts` experts, not a whole layer's planes: the driver sets it where the
//! chain serves every streamed layer it is about to upload, and clears it where the per-op rails serve instead and at the drop
def public set_moe_gpu_stream_ring(on : bool; experts : int64 = 0l) {
    g_moe_gpu_stream_ring = on
    g_moe_gpu_stream_ring_experts = on ? experts : 0l
}

def public moe_gpu_stream_ring() : bool => g_moe_gpu_stream_ring

//! the ring's experts where the ring form is on, 0 otherwise
def public moe_gpu_stream_ring_experts() : int64 => g_moe_gpu_stream_ring_experts

var private g_moe_gpu_stream_need = 0l

//! Loader contract: worst-case streamed-slot bytes of THIS model (2x gate/up + down device
//! planes of the largest expert layer, via moe_gpu_plane_bytes). The tier carves its stream
//! reserve from this instead of a fixed guess; 0 = unknown (tier falls back to its constant).
def public set_moe_gpu_stream_need(bytes : int64) {
    g_moe_gpu_stream_need = bytes
}

def public moe_gpu_stream_need() : int64 => g_moe_gpu_stream_need

let DAT_MIRROR_ROWS = 2_048l
var private g_moe_gpu_dat_need = 0l

//! Loader contract: the decode attention block's per-layer f16 K/V mirrors for THIS model
//! (layers x DAT_MIRROR_ROWS x kvd x 4 bytes); the tier carves them from the weight budget
//! BEFORE placement - allocated after placement they come out of the desktop reserve and page the resident stacks out.
def public set_moe_gpu_dat_need(bytes : int64) {
    g_moe_gpu_dat_need = bytes
}

def public moe_gpu_dat_need() : int64 => g_moe_gpu_dat_need

typedef MoeGpuFfnTailFn = function<(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64;
    xqp : int8 const?; xsp : float const?; n, nfe, nrows : int64; f1, f3, f2 : int; is_gelu : bool;
    wp : float const?; xp : float const?; var xoutp : float?; woq_next : int64; fq_next : int; anp : float const?; eps : float) : void>
typedef MoeGpuFfnTailPendingFn = function<() : bool>
var private g_moe_gpu_ffn_tail : MoeGpuFfnTailFn
var private g_moe_gpu_ffn_tail_pending : MoeGpuFfnTailPendingFn

//! Tier contract: the decode FFN tail - a resident layer's FFN chain plus combine + residual and
//! the NEXT layer's attention rms/requant on the device, submitted without a fence; that layer's
//! attention submit collects it (its fence covers both) and lands the residual row in x.
def public set_moe_gpu_ffn_tail_hooks(tail : MoeGpuFfnTailFn; pending : MoeGpuFfnTailPendingFn) {
    g_moe_gpu_ffn_tail = tail
    g_moe_gpu_ffn_tail_pending = pending
}

def public moe_gpu_ffn_tail_ok : bool => g_moe_gpu_ffn_tail != null

//! true between a tail submit and the attention submit that collects it: the host's x is stale
//! and the next layer's attention acts already sit on the device
def public moe_gpu_ffn_tail_pending : bool => g_moe_gpu_ffn_tail_pending != null && invoke(g_moe_gpu_ffn_tail_pending)

def public matmul_moe_gpu_ffn_tail(offs1 : array<int64>; offs3 : array<int64>; offs2 : array<int64>; nregions : int64;
                                   xq : array<int8>; xs : array<float>; n, nfe, nrows : int64; f1, f3, f2 : int; is_gelu : bool;
                                   w : array<float>; var x : array<float>; woq_next : int64; fq_next : int; anp : float const?; eps : float) {
    unsafe {
        invoke(g_moe_gpu_ffn_tail,
            addr<int64 const?>(offs1[0]), addr<int64 const?>(offs3[0]), addr<int64 const?>(offs2[0]), nregions,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, nfe, nrows, f1, f3, f2, is_gelu,
            addr<float const?>(w[0]), addr<float const?>(x[0]), addr(x[0]), woq_next, fq_next, anp, eps)
    }
}

//! The whole-token decode span's arguments: the resident layers [l0, l1) as ONE device chain.
//! Per-layer arrays run over the span; the feed is layer l0's attention row in its plane's acts
//! form; x rides in and out; kout/vout take every layer's raw k and v rows (nl x kvd each).
struct MoeSpanDec {
    pos : int64
    dim : int64
    qd : int64
    kvd : int64
    hs : int64
    n_heads : int64
    kv_mul : int64
    ne : int64
    k : int64
    nfe : int64
    scale : float
    eps : float
    wscale : float
    neox : bool
    qk_norm : bool
    norm_topk : bool
    is_gelu : bool
    woq : array<int64>
    wok : array<int64>
    wov : array<int64>
    woo : array<int64>
    fq : array<int>
    fk : array<int>
    fv : array<int>
    fo : array<int>
    we1 : array<int64>       // expert 0's plane element offsets per layer
    we3 : array<int64>
    we2 : array<int64>
    f1 : array<int>
    f3 : array<int>
    f2 : array<int>
    nsh : int64                  //! the shared expert's width (0 = the model has none)
    sh_gated : bool              //! the shared expert's row scales by sigmoid(gate . xb)
    wsh1 : array<int64>          //! the shared expert's gate plane offset per layer, in its format's plane space (`moe_gpu_shexp_planes`)
    wsh3 : array<int64>          //! ... its up plane
    wsh2 : array<int64>          //! ... its down plane
    fsh1 : array<int>            //! the shared expert's gate plane format slot per layer
    fsh3 : array<int>            //! ... its up plane's
    fsh2 : array<int>            //! ... its down plane's
    shgate : float const?        //! [nl x dim] f32 gate rows, layer-major (null when ungated or absent)
    rmsq : array<float const?>   // the qk-norm rows (null when off)
    rmsk : array<float const?>
    bq : array<float const?>     // the q/k/v projection bias rows (null when the model has none)
    bk : array<float const?>
    bv : array<float const?>
    router : float const?        // [nl x ne x dim] f32 rows, layer-major
    att_norm : float const?      // [nl x dim]
    ffn_norm : float const?      // [nl x dim]
    blob : float const?          // the carrier those rows live in - with woq[0] and the suffix length, the engine's refill key
    xq : int8 const?             // layer l0's attention feed
    xs : float const?
    cossin : float const?        // this position's packed rope row
    x : float?                   // the residual row in/out
    kout : float?                // [nl x kvd] raw k rows out
    vout : float?
}

typedef MoeGpuSpanDecFn = function<(var a : MoeSpanDec) : void>
var private g_moe_gpu_span_dec : MoeGpuSpanDecFn

//! Tier contract: the whole-token decode span - every resident layer's attention, router,
//! top-k, FFN and combine as one recorded chain and ONE submit; the host feeds the first
//! layer's attention row and takes back the residual row and every layer's raw k/v rows.
def public set_moe_gpu_span_dec_hook(fn : MoeGpuSpanDecFn) {
    g_moe_gpu_span_dec = fn
}

def public moe_gpu_span_ok : bool => g_moe_gpu_span_dec != null

def public matmul_moe_gpu_span_dec(var a : MoeSpanDec) {
    invoke(g_moe_gpu_span_dec, a)
}

//! Loader contract: "" = the loaded model rides the armed tier; otherwise the human reason it
//! cannot (reset per load; meaningless while the tier is unarmed).
def public set_moe_gpu_model_support(reason : string) {
    g_installed_marks.unsupported = reason
}

//! Live GPU-tier status (the control-page badge): actual post-load residency and routing.
struct GpuTierStatus {
    backend : string          //!< tier backend name ("vulkan" | "cuda"); "" = no GPU tier installed
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

def gpu_tier_status_() : GpuTierStatus {
    if (!g_moe_gpu_installed) {
        return GpuTierStatus(reason = "no GPU tier installed")
    }
    let total = g_installed_marks.total_layers
    let resident = total > 0l ? max(total - g_installed_marks.from_layer, 0l) : 0l
    let streamed = total > 0l ? max(min(g_installed_marks.from_layer, total) - min(g_installed_marks.stream_from, total), 0l) : 0l
    return GpuTierStatus(
        backend = g_moe_gpu_backend,
        device = invoke(g_moe_gpu_device_report),
        supported = empty(g_installed_marks.unsupported),
        reason = g_installed_marks.unsupported,
        resident_layers = resident,
        streamed_layers = streamed,
        dn_layers = long_length(g_installed_marks.dn_layers),
        attn_layers = long_length(g_installed_marks.attn_layers),
        dense_planes = long_length(g_installed_marks.dense_offs),
        cls_resident = g_installed_marks.cls_off >= 0l,
        vram_bytes = invoke(g_moe_gpu_vram_report))
}

var private g_gpu_cpu_pass_reasons : array<string>
var private g_gpu_cpu_pass_words : array<string>
var private g_gpu_cpu_pass_counts : array<int64>

//! Name the reasons a served GPU path hands a call to the CPU rails, in slot order, and zero the counts - the driver that counts them registers as it arms a model.
def public set_gpu_cpu_pass_reasons(reasons, words : array<string>) {
    if (length(reasons) != length(words)) {
        panic("dasLLAMA GPU tier: {length(reasons)} pass reasons registered with {length(words)} texts - one text per reason")
    }
    g_gpu_cpu_pass_reasons := reasons
    g_gpu_cpu_pass_words := words
    g_gpu_cpu_pass_counts |> clear()
    g_gpu_cpu_pass_counts |> resize(length(reasons))
}

//! how many pass reasons the armed driver registered: a test walks them all
def public gpu_cpu_pass_reason_count() : int => length(g_gpu_cpu_pass_reasons)

//! No call has passed yet: the counts are the armed model's, so a drop and every load zero them.
def public reset_gpu_cpu_passes() {
    g_gpu_cpu_pass_counts |> fill(0l)
}

def public note_gpu_cpu_pass(reason_slot : int) {
    if (reason_slot < 0 || reason_slot >= length(g_gpu_cpu_pass_counts)) {
        panic("dasLLAMA GPU tier: pass reason {reason_slot} outside the {length(g_gpu_cpu_pass_counts)} registered - the driver registers its reasons as it arms")
    }
    g_gpu_cpu_pass_counts[reason_slot]++
}

//! Calls the served GPU path handed to the CPU rails since the model armed, one row per reason that fired - an empty answer is the device serving every call.
def gpu_cpu_passes_() : array<tuple<reason : string; words : string; count : int64>> =>
    <- [for (r, w, n in g_gpu_cpu_pass_reasons, g_gpu_cpu_pass_words, g_gpu_cpu_pass_counts); (reason = r, words = w, count = n); where n > 0l]

//! Upload one weight stack ([rows x n] at woff) as the GPU tier's row-major planes — q8 (fmt 0):
//! int8 quants + f16 scales; native K-quant (k4/k5/k6): superblock payloads + decoded scale rows.
//! stream = pinned host mirror for DMA streaming vs resident VRAM; false = tier absent/budget hit.
def public moe_gpu_upload_stack(wq : array<uint8> | #; ws : array<uint8> | #; woff : int64; n, rows : int64; fmt : int; stream : bool = false) : bool {
    if (!g_moe_gpu_installed) {
        return false
    }
    // empty planes hand the tier null: the slice replay's refusal-boundary serve is empty on
    // purpose (the tier refuses on arithmetic, no bytes read)
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

//! The f16-fed twin of matmul_moe_gpu_ffn_combined: `x` is the window's normed activation rows
//! (npos x n, f32) - no quant image, no CPU gather; the tier gathers by `inv` device-side.
//! Ask moe_gpu_ffn_xf_ok first: the seat serves only the formats its chain admits.
def public matmul_moe_gpu_ffn_combined_xf(var gout : array<float>; offs1 : array<int64>; offs3 : array<int64>; offs2 : array<int64>;
                                          nregions : int64; x : array<float>; w : array<float>; inv : array<uint>;
                                          n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    unsafe {
        invoke(g_moe_gpu_ffn_xf, addr(gout[0]),
            addr<int64 const?>(offs1[0]), addr<int64 const?>(offs3[0]), addr<int64 const?>(offs2[0]), nregions,
            addr<float const?>(x[0]), addr<float const?>(w[0]), addr<uint const?>(inv[0]),
            n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    }
}

//! The async form of matmul_moe_gpu_ffn_combined_xf: submit the chain over the FIRST `nregions`
//! triples of the offs arrays and return; the CPU serves whatever it keeps meanwhile, then
//! matmul_moe_gpu_ffn_combined_xf_join collects the combined rows. Bound by moe_gpu_ffn_xf_async_rows.
def public matmul_moe_gpu_ffn_combined_xf_begin(offs1 : array<int64>; offs3 : array<int64>; offs2 : array<int64>;
                                                nregions : int64; x : array<float>; w : array<float>; inv : array<uint>;
                                                n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    unsafe {
        invoke(g_moe_gpu_ffn_xf_begin,
            addr<int64 const?>(offs1[0]), addr<int64 const?>(offs3[0]), addr<int64 const?>(offs2[0]), nregions,
            addr<float const?>(x[0]), addr<float const?>(w[0]), addr<uint const?>(inv[0]),
            n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    }
}

def public matmul_moe_gpu_ffn_combined_xf_join(var gout : array<float>) {
    invoke(g_moe_gpu_ffn_xf_join, unsafe(addr(gout[0])))
}

//! the meet form's open: false where the layer does not stream (the caller takes another form)
def public matmul_moe_gpu_ffn_combined_xf_open(offs1 : array<int64>; offs3 : array<int64>; offs2 : array<int64>;
                                               nregions : int64; x : array<float>; w : array<float>; inv : array<uint>;
                                               n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) : bool {
    unsafe {
        return invoke(g_moe_gpu_ffn_xf_open,
            addr<int64 const?>(offs1[0]), addr<int64 const?>(offs3[0]), addr<int64 const?>(offs2[0]), nregions,
            addr<float const?>(x[0]), addr<float const?>(w[0]), addr<uint const?>(inv[0]),
            n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    }
}

//! regions [r_lo, r_hi) of the opened window to the device as one bundle; without `wait` false means the ring is full - ask again later
def public matmul_moe_gpu_ffn_combined_xf_push(r_lo, r_hi : int64; wait : bool) : bool => invoke(g_moe_gpu_ffn_xf_push, r_lo, r_hi, wait)

//! the end of the bundle starting at region `r_lo`, at most `r_max`: what one command of the chain takes
def public matmul_moe_gpu_ffn_combined_xf_bundle(r_lo, r_max : int64) : int64 => invoke(g_moe_gpu_ffn_xf_bundle, r_lo, r_max)

def public matmul_moe_gpu_ffn_combined_xf_close {
    invoke(g_moe_gpu_ffn_xf_close)
}

// ===== the decode attention block: one submit per layer over the resident q/k/v/o quad =====
// The tier keeps a per-layer f16 K/V mirror the prefill chain fills; a served token returns the
// wo row plus its RAW k and v rows (f32, before norm and rope) so the host re-derives its cache row.
typedef MoeGpuAttnDecFn = function<(var xb2p : float?; var kp : float?; var vp : float?; woq : int64; wok : int64; wov : int64; woo : int64; fq : int; fk : int; fv : int; fo : int; xqp : int8 const?; xsp : float const?; cosp : float const?; rmsqp : float const?; rmskp : float const?; bqp : float const?; bkp : float const?; bvp : float const?; pos : int64; dim : int64; qd : int64; kvd : int64; hs : int64; n_heads : int64; kv_mul : int64; scale : float; eps : float; neox : bool; qk_norm : bool) : void>
typedef MoeGpuAttnDecOkFn = function<(woq : int64; fq : int; pos : int64; gen : int64) : int>
typedef MoeGpuAttnDecClaimFn = function<(fresh : bool) : int64>
typedef MoeGpuAttnDecHydrateFn = function<(woq : int64; fq : int; kp : uint8 const?; vp : uint8 const?; npos : int64; kvd : int64; f16 : bool) : void>
// MoeGpuAttnDecOkFn's answers
let DAT_SERVE = 0        // the layer's mirror holds exactly [0, pos) of the session's generation
let DAT_STALE_GEN = 1    // the session's generation is not the mirrors' - claim fresh at layer 0
let DAT_HYDRATE = 2      // this generation, but the layer's rows are not [0, pos) - refill from the host cache
let DAT_NEVER = 3        // no block, or pos past the mirror cap

[unused_argument(xb2p, kp, vp, woq, wok, wov, woo, fq, fk, fv, fo, xqp, xsp, cosp, rmsqp, rmskp, bqp, bkp, bvp, pos, dim, qd, kvd, hs, n_heads, kv_mul, scale, eps, neox, qk_norm)]
def private moe_gpu_unset_attn_dec(var xb2p : float?; var kp : float?; var vp : float?; woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                                   xqp : int8 const?; xsp : float const?; cosp : float const?; rmsqp : float const?; rmskp : float const?;
                                   bqp : float const?; bkp : float const?; bvp : float const?;
                                   pos, dim, qd, kvd, hs, n_heads, kv_mul : int64; scale, eps : float; neox, qk_norm : bool) {
    panic("dasLLAMA: GPU decode attention hit without an installed block")
}

[unused_argument(woq, fq, pos, gen)]
def private moe_gpu_no_attn_dec(woq : int64; fq : int; pos, gen : int64) : int => DAT_NEVER

[unused_argument(fresh)]
def private moe_gpu_noop_attn_dec_claim(fresh : bool) : int64 => 0l

[unused_argument(woq, fq, kp, vp, npos, kvd, f16)]
def private moe_gpu_unset_attn_dec_hydrate(woq : int64; fq : int; kp : uint8 const?; vp : uint8 const?; npos, kvd : int64; f16 : bool) {
    panic("dasLLAMA: GPU decode attention hydrate hit without an installed block")
}

var g_moe_gpu_attn_dec = @@moe_gpu_unset_attn_dec
var g_moe_gpu_attn_dec_ok = @@moe_gpu_no_attn_dec
var g_moe_gpu_attn_dec_claim = @@moe_gpu_noop_attn_dec_claim
var g_moe_gpu_attn_dec_hydrate = @@moe_gpu_unset_attn_dec_hydrate

//! Tier registration of the decode attention block (beside install_moe_gpu_tier).
def public set_moe_gpu_attn_dec_hooks(dec : MoeGpuAttnDecFn; ok : MoeGpuAttnDecOkFn; claim : MoeGpuAttnDecClaimFn; hydrate : MoeGpuAttnDecHydrateFn) {
    g_moe_gpu_attn_dec = dec
    g_moe_gpu_attn_dec_ok = ok
    g_moe_gpu_attn_dec_claim = claim
    g_moe_gpu_attn_dec_hydrate = hydrate
}

//! Refill one layer's mirror with the host cache's K/V rows [0, npos) - f32 rows (converted) or
//! f16 rows (copied) - for a session reaching the block mid-sequence or after a rewind; npos = 0
//! empties it. The layer then holds exactly those positions.
def public matmul_moe_gpu_attn_dec_hydrate(woq : int64; fq : int; kp : uint8 const?; vp : uint8 const?; npos, kvd : int64; f16 : bool) {
    invoke(g_moe_gpu_attn_dec_hydrate, woq, fq, kp, vp, npos, kvd, f16)
}

//! The mirrors change hands: after a GPU prefill from position 0 they hold the prefilling session's
//! rows (fresh = false keeps them); a session decoding from position 0 starts them EMPTY (fresh =
//! true zeroes every layer's count). The tier mints a generation the session keeps (Session.dat_gen).
def public moe_gpu_attn_dec_claim(fresh : bool) : int64 => invoke(g_moe_gpu_attn_dec_claim, fresh)

//! Does the block serve the token at `pos` of the layer whose q plane sits at `woq` in format `fq` (the pair is the layer's
//! identity: offsets are per format plane) for the session holding generation `gen`?
//! One of DAT_SERVE / DAT_STALE_GEN / DAT_HYDRATE / DAT_NEVER.
def public moe_gpu_attn_dec_ok(woq : int64; fq : int; pos, gen : int64) : int =>
    invoke(g_moe_gpu_attn_dec_ok, woq, fq, pos, gen)

//! One layer's decode attention on the GPU (q/k/v projections, bias, qk-norm+rope into the layer's
//! mirror, attention over it, wo). `xb2` gets the wo row; `k`/`v` the RAW projection rows (before the
//! bias) - the host cache row takes its own bias + norm + rope. Bias rows are this layer's, or null.
def public matmul_moe_gpu_attn_dec(var xb2 : array<float>; var k : array<float>; var v : array<float>;
                                   woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                                   xq : array<int8>; xs : array<float>; cossin : array<float>;
                                   rmsq : float const?; rmsk : float const?;
                                   bqp : float const?; bkp : float const?; bvp : float const?;
                                   pos, dim, qd, kvd, hs, n_heads, kv_mul : int64; scale, eps : float; neox, qk_norm : bool) {
    unsafe {
        invoke(g_moe_gpu_attn_dec, addr(xb2[0]), addr(k[0]), addr(v[0]), woq, wok, wov, woo, fq, fk, fv, fo,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), addr<float const?>(cossin[0]), rmsq, rmsk, bqp, bkp, bvp,
            pos, dim, qd, kvd, hs, n_heads, kv_mul, scale, eps, neox, qk_norm)
    }
}

var private g_dense_gemm_rows = 0l   //! the activation rows the dense rail's batched GEMMs ran on the device, process-wide

//! the activation rows the dense rail's batched GEMMs (`matmul_gpu_dense`) ran on the device so far, every plane summed
def public moe_gpu_dense_gemm_rows() : int64 => g_dense_gemm_rows

//! Dense batched GEMM on the GPU tier — y [nrows x d] = X·Wᵀ over the resident plane ([d x n]
//! at woff). xq/xs is the row-major pre-quantized activation image in the plane's own quant form
//! (q8_0 per-32 for q8, Q8_K per-256 for kq); per-16 block sums are recomputed in-kernel.
def public matmul_gpu_dense(var y : array<float>; woff : int64; xq : array<int8>; xs : array<float>; n, d, nrows : int64; fmt : int) {
    g_dense_gemm_rows += nrows
    unsafe {
        invoke(g_moe_gpu_dense, addr(y[0]), woff, addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), n, d, nrows, fmt)
    }
}

//! One recurrent layer's whole deltanet attention block on the GPU tier — see MoeGpuDnFn for
//! the contract. y gets npos x dim out-proj rows, state/tail update in place; beta/alpha arrive
//! RAW (transforms run in-kernel), conv taps in the fblob disk layout (cd x dconv).
def public matmul_moe_gpu_dn(var y : array<float>; woq, woz, woo : int64; fq, fz, fo : int;
                             xq : array<int8>; xs : array<float>; b : array<float>; g : array<float>;
                             convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
                             var stp : float?; var tail : array<float>; histp : float const?;
                             npos, dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    unsafe {
        invoke(g_moe_gpu_dn, addr(y[0]), woq, woz, woo, fq, fz, fo,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), addr<float const?>(b[0]), addr<float const?>(g[0]),
            convw, wnormp, ap, dtp, stp, addr(tail[0]), histp,
            npos, dim, cd, kd, di, nvh, nkh, ds, dconv, eps)
    }
}

//! One full-attention layer's whole softmax attention block on the GPU tier (MoeGpuAttnFn). y gets
//! npos x dim wo rows; k/v get npos x kv_dim roped-k / biased-v rows; cos/sin are npos x rot/2 NEOX
//! rows; rms rows null when qknorm is off, bias rows null without the projection bias (32-arg cap: no kv_mul).
def public matmul_moe_gpu_attn(var y : array<float>; var k : array<float>; var v : array<float>;
                               woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                               xq : array<int8>; xs : array<float>;
                               rmsqp : float const?; rmskp : float const?;
                               bqp : float const?; bkp : float const?; bvp : float const?;
                               cosp : float const?; sinp : float const?;
                               npos, dim, qd, kv_dim, hs, rot, n_heads : int64;
                               scale : float; eps : float; gated : bool; qknorm : bool) {
    unsafe {
        invoke(g_moe_gpu_attn, addr(y[0]), addr(k[0]), addr(v[0]),
            woq, wok, wov, woo, fq, fk, fv, fo,
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]),
            rmsqp, rmskp, bqp, bkp, bvp, cosp, sinp,
            npos, dim, qd, kv_dim, hs, rot, n_heads, scale, eps, gated, qknorm)
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

//! A kernel's emitted payload - shader text, or SPIR-V words - written as `file` under `dir`, the directory a dump
//! knob names; nothing where the knob is unset. `knob` is the knob's name in the line this logs. False where the
//! file could not be written.
def public kernel_dump_save(dir, file, knob : string; payload) : bool {
    return true if (empty(dir))
    let path = path_join(dir, file)
    var ok = false
    fopen(path, "wb") $(f) {
        if (f != null) {
            f |> fwrite(payload)
            ok = true
        }
    }
    to_log(ok ? LOG_INFO : LOG_WARNING, "dasLLAMA: {knob} {ok ? "wrote" : "could not write"} {path}\n")
    return ok
}

//! log census `cov` (name -> dispatches): the dispatched with counts, a LOUD WARNING per never-dispatched one
def public kernel_coverage_report(backend, population : string; cov : table<string; int64>) {
    var names <- [for (name in keys(cov)); name]
    names |> sort()
    var served = 0
    for (name in names) {
        let n = cov?[name] ?? 0l
        if (n > 0l) {
            served++
            to_log(LOG_INFO, "{backend} kernel coverage: {name} = {n}\n")
        }
    }
    var dead = 0
    for (name in names) {
        if ((cov?[name] ?? 0l) == 0l) {
            dead++
            to_log(LOG_WARNING, "{backend} kernel coverage: {name} NEVER DISPATCHED this run - coverage gap or dead kernel (human call)\n")
        }
    }
    to_log(LOG_INFO, "{backend} kernel coverage: {served} dispatched, {dead} never-dispatched of {length(names)} {population}\n")
    delete names
}


//! the seats a home's entry declares by value, back to their declared defaults (`set_gpu_home`); the function seats are
//! `gpu_home_reset_fn_slots`'s, every `= @@fn` global of this module. `REVIEW.das`'s home-switch gate holds this list to the
//! `*_installed` flags the module declares
def private gpu_home_reset_flags {
    g_moe_gpu_backend = ""
    g_rdec_installed = false
    g_rdec_dn_installed = false
    g_rdec_site_feeds = false
    g_rdec_kv_ring = true
    g_rdec_kv_share = true
    g_rdec_bias_installed = false
    g_rdec_sinks_installed = false
    g_rdec_moe_installed = false
    g_rdec_hc_installed = false
    g_rdec_hot_installed = false
    g_moe_gpu_streams = false
    g_rdec_regions_installed = false
    g_rdec_prefill_ids_installed = false
    g_rdec_prefill_segs_installed = false
    g_rdec_carry_installed = false
    g_rdec_draft_installed = false
    g_rdec_verify_installed = false
    g_rdec_rollback_installed = false
    g_rdec_token_seq_installed = false
    g_rdec_chain_charged = false
    g_rdec_dn_snaps_installed = false
    g_rdec_head_warm_installed = false
    g_rdec_hidden_installed = false
    g_rdec_ple_installed = false
    g_moe_gpu_heat_installed = false
    g_moe_gpu_host_rows_on = false
}

[init, boot_restore]
def private restore_gpu_tier_fn_defaults {}

[seat_reset]
def private gpu_home_reset_fn_slots {}
