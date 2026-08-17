options gen2
options _comment_hygiene = true

module dasllama_metal_shapes shared public

require dasllama/dasllama_common
require dasllama/dasllama_math   // kernel_backend_needs_repack / active_kernel_backend — the repack clause

// The Metal drivers' MODEL-SHAPE gates, extracted PORTABLE (no das_metal require): the decline
// enum, the feature-bit derivation, and every pure-Model serve gate the mint-time servable
// predicate needs. This is what lets a PC bake an M1's metal-flavor .dlim — the blob transform
// (dasllama_layout) was always pure bytes; only these gates were build-locked to Apple.
// The RUNTIME gates (device, PSO, planar, mirrors) stay in the metal modules.

// why a step declined to the CPU path — every gate reports its FIRST failing clause, and the
// declines-by-reason tables make silent coverage regressions observable (the support-matrix test
// asserts these names as strings; renaming a value is a test-visible change)
enum MetalDecodeDecline {
    none                // supported — the step runs on the GPU
    quant_mode          // weights not loaded under QuantMode.q8
    kquant_native       // per-tensor K-quant planes: not readable as qblob offsets (kernels = W4)
    arch                // RETIRED (family waves A): the arch-name check is gone — non-std block
                        // sets decline `graph`, feature-flag configs decline `feature` per bit
    backend_repack      // repack backend: interleaved weight layout unreadable by the GPU kernels
    rope_mode           // rope tables off
    config              // RETIRED (family waves A): split into `feature` + the per-need
                        // `need_*` keys in the declines-by-reason table
    graph               // block set is not the std dense stack (MoE / recurrent / custom attn)
    feature             // the model needs layer-stack features this path's kernels don't
                        // implement — each missing bit records a `need_*` key alongside
    shape               // head/dim alignment outside the kernel grids
    layers              // per-layer non-uniformity (kv_dim/head_size/hidden/sliding/missing wv)
    kv_dtype            // session KV codec not uniform f16/f32 (the mirror moves raw row bytes)
    depth               // RETIRED (M2.4): the chunked/partD attention scans any depth — capacity
                        // limits surface as `mirror` (the arena cannot hold the session's slice)
    no_uid              // session without a real uid (mirror keying)
    rope_rows           // the position's rope row not built
    batch_dim_cap       // RETIRED (M2.6): the batch add+rms falls back to the unfused pair past
                        // the fused slab, and the GEMM's fixed K-tiles serve any dim at B > 4
    device              // metal_decode_init failed (no device / PSO build failure)
    mirror              // the mirror arena cannot hold the KV slice
    gpu_error           // dispatch failed at execution time
    planar              // weights not in the blob-image form (the drivers are blob-only —
                        // load_model under metal mode transforms/maps the metal flavor)
    dn_state            // deltanet recurrent state not GPU-resident for this position (state is
                        // forward-only and NOT re-derivable — no watermark repair exists)
}

// The tower/embedder driver's decline set — same contract as the prefill/decode enums: FIRST
// failing clause, .none = supported, names asserted as strings by the image suite's mtower
// cells. Conformer families never decline — no seam; their evidence is frozen counters.
enum MetalTowerDecline {
    none                // supported — the tower encode runs on the GPU
    knob                // DASLLAMA_METAL_TOWER=0 — the A/B rail
    quant_mode          // a q8-loaded tower: the f32/bf16 lane only (the q8 tower lane is its own slice)
    shape               // a dim violating the GEMM grids (m%32 / ndim%64 / kdim%32) or a misaligned plane offset
    device              // metal init failed (no device / PSO build failure)
    gpu_error           // dispatch failed at execution time
}

// The whisper ASR-decoder driver's decline set (cross-KV now, the decode step as it lands) —
// same contract as the tower enum: FIRST failing clause, .none = supported, names asserted
// as strings by the decoder gates. The driver is Q8-NATIVE: an f32-forced decoder declines.
enum MetalWdecDecline {
    none                // supported — the decoder-side work runs on the GPU
    knob                // DASLLAMA_METAL_WDEC=0 — the A/B rail
    quant_mode          // an f32-loaded decoder: the q8 lane only (f32 is the CPU oracle rail)
    shape               // a dim violating the GEMM grids or a misaligned plane offset
    device              // metal init failed (no device / PSO build failure)
    gpu_error           // dispatch failed at execution time
}

// why a prefill declined to the CPU path — the FIRST failing gate clause (the decode enum's
// twin; the support-matrix test asserts these names as strings)
enum MetalPrefillDecline {
    none                // supported — the prefill runs on the GPU
    start_pos           // RETIRED (M2.5): a continuation chunk gathers the session's existing
                        // rows into the K/V panels and shifts the causal diagonal by start_pos
    npos_window         // npos below g_min_npos or above g_max_npos
    quant_mode          // weights not loaded under QuantMode.q8
    kquant_native       // per-tensor K-quant planes: not readable as qblob offsets (kernels = W4)
    wscale_f16          // f16 scale plane (the load DEFAULT) — GPU prefill needs the s16 path (W2)
    arch                // RETIRED (family waves A): the arch-name check is gone — non-std block
                        // sets decline `graph`, feature-flag configs decline `feature` per bit
    backend_repack      // repack backend: interleaved weight layout unreadable by the GEMM kernel
    config              // RETIRED (family waves A): split into `feature` + the per-need
                        // `need_*` keys in the declines-by-reason table
    graph               // block set is not the std dense stack (MoE / recurrent / custom attn)
    feature             // the model needs layer-stack features this path's kernels don't
                        // implement — each missing bit records a `need_*` key alongside
    shape               // head/dim alignment outside the kernel grids
    layers              // per-layer non-uniformity (kv_dim/head_size/hidden/sliding/missing wv)
    rope_rows           // the window's rope table not built
    non_causal_span     // the A/B decline only (DASLLAMA_METAL_SPAN=0): the trio takes the
                        // uniform bound as AttnArgs.uend, so a span serves by default
    device              // metal_prefill_init failed (no device / PSO build failure)
    gpu_error           // dispatch failed at execution time
    planar              // weights not in the blob-image form (the drivers are blob-only)
    dn_state            // recurrent state not GPU-resident and no CPU prefix to sync from
}

//! Layer-stack features a model's graph needs beyond the base llama dense block — ONE derivation
//! (`metal_needs`) feeds every GPU gate; each path holds its own implemented-set mask and declines
//! `feature` on the difference, recording a `need_*` key per missing bit.
bitfield MetalNeed {
    neox            // NEOX rope pairing (j, j+hs/2) — qwen2/qwen3/phi3/gemma/gptoss
    qkv_bias        // additive bias on the Q/K/V projections — qwen2/glm4moe/gptoss
    qk_norm         // per-head RMSNorm on Q/K before rope — qwen3/gemma3+/qwen35
    v_norm          // weightless per-head RMSNorm on V — gemma4
    partial_rope    // rope_dim < head_size rotates only the first rope_dim dims — qwen35/glm4moe
    q_gated         // 2x-wide Q, out ⊙ σ(gate) before wo — qwen35
    attn_scale      // attention-score scale override (not 1/sqrt(hs)) — gemma4
    softcap         // attention-logit soft cap — gemma2
    sliding         // sliding-window attention span — gemma2/3/4, gptoss
    sinks           // per-head sink logit in the softmax denominator — gptoss
    out_bias        // additive bias on the attention output projection — gptoss
    pre_post_norm   // extra post-attention / post-FFN norms — gemma2/3/4
    out_scale       // per-layer scalar on the layer output — gemma4
    geglu           // GeGLU FFN activation (gelu, not silu) — gemma2/3/4
    ple             // per-layer embeddings — gemma4 E-series
    qd_neq_dim      // n_heads*head_size != dim (query width differs from the residual) — qwen3
}

//! The feature set model `t` needs from a resident GPU driver — derived from the loaded config
//! (per-file truth: loader-detected flags included). Graph-shape needs are NOT bits here — the
//! block set declares those (`attn_is_std` / `ffn_is_dense`) and the gates decline `graph` first.
def metal_needs(t : Model) : MetalNeed {
    let c = t.config
    var n : MetalNeed
    n.neox = c.rope_neox
    n.qkv_bias = c.attn_qkv_bias
    n.qk_norm = c.qk_norm
    n.v_norm = c.v_norm
    n.partial_rope = c.rope_dim > 0l && c.rope_dim != c.head_size
    n.q_gated = c.q_gated
    n.attn_scale = c.attn_scale > 0.0
    n.softcap = c.attn_logit_softcap > 0.0
    n.sliding = c.sliding_window > 0l
    n.sinks = c.attn_sinks
    n.out_bias = c.attn_out_bias
    n.pre_post_norm = c.pre_post_norm
    n.out_scale = c.layer_out_scale
    n.geglu = c.ffn_act != FfnAct.silu
    n.ple = c.n_embd_per_layer > 0l
    n.qd_neq_dim = c.n_heads * c.head_size != c.dim
    return n
}

def metal_missing_needs(t : Model; ok : MetalNeed) : MetalNeed {
    return metal_needs(t) & ~ok
}

// one `need_*` key per missing bit — the support-matrix cells assert these names as strings
def record_needs(var tab : table<string; int64>; missing : MetalNeed) {
    if (missing.neox) { tab["need_neox"]++ }
    if (missing.qkv_bias) { tab["need_qkv_bias"]++ }
    if (missing.qk_norm) { tab["need_qk_norm"]++ }
    if (missing.v_norm) { tab["need_v_norm"]++ }
    if (missing.partial_rope) { tab["need_partial_rope"]++ }
    if (missing.q_gated) { tab["need_q_gated"]++ }
    if (missing.attn_scale) { tab["need_attn_scale"]++ }
    if (missing.softcap) { tab["need_softcap"]++ }
    if (missing.sliding) { tab["need_sliding"]++ }
    if (missing.sinks) { tab["need_sinks"]++ }
    if (missing.out_bias) { tab["need_out_bias"]++ }
    if (missing.pre_post_norm) { tab["need_pre_post_norm"]++ }
    if (missing.out_scale) { tab["need_out_scale"]++ }
    if (missing.geglu) { tab["need_geglu"]++ }
    if (missing.ple) { tab["need_ple"]++ }
    if (missing.qd_neq_dim) { tab["need_qd_neq_dim"]++ }
}

// the kq formats the Metal kernels implement — anything else (the q4_0 QAT tier, future
// formats) must DECLINE kquant_native instead of falling into a wrong-layout kernel branch
// (the dispatchers below treat "not k4/k6" as k5)
def kq_fmt_gpu_supported(f : KqFmt) : bool {
    return f == KqFmt.q8 || f == KqFmt.k4 || f == KqFmt.k5 || f == KqFmt.k6
}

def private kq_fmts_gpu_supported(a : array<KqFmt>) : bool {
    for (f in a) {
        if (!kq_fmt_gpu_supported(f)) {
            return false
        }
    }
    return true
}

// every tensor fmt a kq load can route to a GPU kernel — the decode/batch/prefill gates all
// clause on this (the MoE expert planes never reach the llama-family GPU graph, so we1/2/3
// stay out of scope)
def kq_load_gpu_supported(t : Model) : bool {
    if (!t.kquant_native) {
        return true
    }
    return ((kq_fmts_gpu_supported(t.wq_fmt) && kq_fmts_gpu_supported(t.wk_fmt) &&
             kq_fmts_gpu_supported(t.wv_fmt) && kq_fmts_gpu_supported(t.wo_fmt)) &&
        (kq_fmts_gpu_supported(t.w1_fmt) && kq_fmts_gpu_supported(t.w2_fmt) &&
             kq_fmts_gpu_supported(t.w3_fmt)) &&
        (kq_fmt_gpu_supported(t.wcls_fmt) && kq_fmt_gpu_supported(t.emb_fmt)))
}

//! Cross-layer KV sharing (gemma-4 E-series): does layer l either own its rows or share a
//! byte-compatible source? A shared layer aliases its source's mirror slab / prefill panel via
//! kv_row_prefix, so the source must own rows and match l's class geometry exactly.
def kv_share_ok(t : Model; l : int64) : bool {
    if (l >= long_length(t.kv_src)) {   // the MTP draft slab owns its rows
        return true
    }
    let src = t.kv_src[l]
    if (src == l) {
        return true
    }
    return (src >= 0l && src < l && t.kv_src[src] == src && !layer_is_recurrent(t.config, src) &&
        layer_kv_dim(t.config, l) == layer_kv_dim(t.config, src) &&
        layer_head_size(t.config, l) == layer_head_size(t.config, src))
}

//! true when layer l reads another layer's KV (no rows of its own — Q-only attention)
def kv_shared_layer(t : Model; l : int64) : bool {
    return l < long_length(t.kv_src) && t.kv_src[l] != l
}

// Wave D serve gate for the deltanet kernel set: fixed d_state 128 (the scan/l2/gate lane math
// bakes 32 lanes x 4), conv taps <= 8 (hist registers). β/α projections: q8 planes ride the
// site GEMVs; f32-on-disk (dn_ba_f32) rides the router-slab f32 GEMV off the fblob offsets.
def dn_metal_ok(t : Model) : bool {
    let c = t.config
    return c.ssm_d_state == 128l && c.ssm_d_conv >= 2l && c.ssm_d_conv <= 8l
}

// (q8 32-blocks, kq 256-superblocks — the expert shift is index math, no base multiple)
def moe_site_ok(fmt : KqFmt; off, ege : int64) : bool {
    if (fmt == KqFmt.q8) {
        return off >= 0l && (off % 256l) == 0l && (ege % 32l) == 0l
    }
    if (fmt == KqFmt.k4 || fmt == KqFmt.k5) {
        return off >= 0l && (off % 256l) == 0l && (ege % 256l) == 0l
    }
    if (fmt == KqFmt.k6) {
        return off >= 0l && (off % 512l) == 0l && (ege % 256l) == 0l
    }
    if (fmt == KqFmt.q51) {   // per-32 planes: off % 128 keeps the 20B/4B binds 16B-aligned
        return off >= 0l && (off % 128l) == 0l && (ege % 32l) == 0l
    }
    return false
}

// The MoE serve gate — covered graph shapes: plain routed FFN (qwen3moe, silu, no biases)
// + optional sigmoid-gated shared expert (qwen35moe), gemma4's dense-shared dual branch, and
// gpt-oss's mx4 experts (softmax_weight, biases, disk-order). No dense-lead / selection bias.
def moe_metal_ok(t : Model) : bool {   // nolint:STYLE038 — flat per-shape refusal ladder; every arm is one reason
    let c = t.config
    if (!c.moe || c.n_layer_dense_lead > 0l || c.moe_exp_probs ||
            (c.expert_weights_scale != 0.0 && c.expert_weights_scale != 1.0)) {
        return false
    }
    if (t.experts_mx4) {
        // gpt-oss: softmax_weight top-k on raw logits, router + per-expert biases, clamped
        // swiglu, disk-order mx4 stacks (the CPU laneq repack has no GPU carry)
        if (c.moe_gate != MoeGate.softmax_weight || c.ffn_act != FfnAct.swiglu_oai ||
                !c.moe_router_bias || !c.moe_exps_bias || c.moe_dense_shexp || c.n_ff_shexp > 0l ||
                t.mx4_repacked ||
                t.router_b_off < 0l || t.web1_off < 0l || t.web2_off < 0l || t.web3_off < 0l) {
            return false
        }
    } elif (c.moe_dense_shexp) {
        if (c.moe_gate != MoeGate.softmax || !c.norm_topk_prob ||
                c.moe_router_bias || c.moe_exps_bias || c.n_ff_shexp > 0l ||
                c.ffn_act != FfnAct.gelu || t.moe_router_scale_off < 0l || t.moe_down_scale_off < 0l ||
                t.rms_pre_ffn2_off < 0l || t.rms_post_ffn1_off < 0l || t.rms_post_ffn2_off < 0l ||
                long_length(t.w1_offs) < c.n_layers) {
            return false
        }
    } elif (c.moe_gate != MoeGate.softmax ||
            c.moe_router_bias || c.moe_exps_bias || c.ffn_act != FfnAct.silu) {
        // norm_topk_prob both ways: renorm = select gmode 0 (qwen3moe/qwen35moe), no-renorm =
        // gmode 2 (qwen2moe)
        return false
    } elif (c.n_ff_shexp > 0l) {
        // sigmoid-gated shared expert (qwen35moe): q8 wsh planes at base + l*she ride the
        // dense site kernels — base + stride %256 (bind), nsh %64 (prefill C tiles)
        let she = c.dim * c.n_ff_shexp
        if (!c.moe_shexp_gated ||
                (c.n_ff_shexp % 64l) != 0l || (she % 256l) != 0l ||
                (t.wsh1_off % 256l) != 0l || (t.wsh2_off % 256l) != 0l || (t.wsh3_off % 256l) != 0l) {
            return false
        }
    }
    let ne = c.n_expert
    let k = c.n_expert_used
    let nfe = c.n_ff_exp
    let ege = c.dim * nfe
    // select holds 8 logits per lane (ne <= 256); %64 dims = the gathered mul_mm C tiles
    if (ne < 1l || ne > 256l || k < 1l || k > 16l || k > ne || nfe <= 0l ||
            (c.dim % 64l) != 0l || (nfe % 64l) != 0l ||
            ne * ege > 3800000000l ||   // uint32 in-kernel indices (q8 byte view = elements * 34/32)
            t.router_off < 0l || empty(t.we1_offs) || empty(t.we2_offs) || empty(t.we3_offs)) {
        return false
    }
    if (t.experts_mx4) {
        // mx4 binds: nibble bytes at off/2 (%4 => off %8), E8M0 at off/32 (%4 => off %128)
        if ((ege % 32l) != 0l) {
            return false
        }
        for (l in range64(c.n_layers)) {
            if (t.we1_offs[l] < 0l || (t.we1_offs[l] % 128l) != 0l ||
                    t.we2_offs[l] < 0l || (t.we2_offs[l] % 128l) != 0l ||
                    t.we3_offs[l] < 0l || (t.we3_offs[l] % 128l) != 0l) {
                return false
            }
        }
        return true
    }
    for (l in range64(c.n_layers)) {
        let f1 = fmt_at(t.we1_fmt, l)
        let f2 = fmt_at(t.we2_fmt, l)
        let f3 = fmt_at(t.we3_fmt, l)
        // per-site alignment + reduction dims: superblock kq kernels iterate whole 256-rows,
        // q51 whole 32-blocks (+ the mul_mm's 64-col output tiles)
        let sb1 = f1 == KqFmt.k4 || f1 == KqFmt.k5 || f1 == KqFmt.k6
        let sb3 = f3 == KqFmt.k4 || f3 == KqFmt.k5 || f3 == KqFmt.k6
        let sb2 = f2 == KqFmt.k4 || f2 == KqFmt.k5 || f2 == KqFmt.k6
        if (!moe_site_ok(f1, t.we1_offs[l], ege) ||
                !moe_site_ok(f2, t.we2_offs[l], ege) ||
                !moe_site_ok(f3, t.we3_offs[l], ege) ||
                ((sb1 || sb3) && (c.dim % 256l) != 0l) ||
                (sb2 && (nfe % 256l) != 0l) ||
                ((f1 == KqFmt.q51 || f3 == KqFmt.q51) && ((c.dim % 32l) != 0l || (nfe % 64l) != 0l)) ||
                (f2 == KqFmt.q51 && ((nfe % 32l) != 0l || (c.dim % 64l) != 0l))) {
            return false
        }
    }
    return true
}

// what each decode path's kernels implement beyond the base llama dense block — grows wave by wave.
let DECODE_NEEDS_OK = (MetalNeed.neox | MetalNeed.qkv_bias | MetalNeed.qk_norm | MetalNeed.qd_neq_dim |
    MetalNeed.softcap | MetalNeed.sliding | MetalNeed.pre_post_norm | MetalNeed.geglu |
    MetalNeed.v_norm | MetalNeed.attn_scale | MetalNeed.out_scale |
    MetalNeed.sinks | MetalNeed.out_bias | MetalNeed.partial_rope | MetalNeed.q_gated |
    MetalNeed.ple)
// q_gated/partial_rope only occur on hybrid models (attn_is_std false), which batch graph-declines;
// ple (E-series) has no batch arm either — batched steps decline `feature` and fall back per-row
let BATCH_NEEDS_OK = DECODE_NEEDS_OK & ~MetalNeed.ple   // clear ple explicitly (XOR would re-add it if DECODE ever drops it)

def decode_shape_decline(t : Model; needs_ok : MetalNeed; allow_moe : bool = false; allow_kv_share : bool = false) : MetalDecodeDecline {
    if (t.quant != QuantMode.q8) {
        return MetalDecodeDecline.quant_mode
    }
    if (!kq_load_gpu_supported(t)) {   // a fmt without a GPU kernel (the q4_0 QAT tier, ...) must not dispatch
        return MetalDecodeDecline.kquant_native
    }
    // non-std graphs decline; MoE (Wave C) + deltanet hybrid (Wave D) serve when their kernels cover it
    if ((!t.blocks.attn_is_std && !(t.config.hybrid_deltanet && dn_metal_ok(t))) ||
            (!t.blocks.ffn_is_dense && !(allow_moe && moe_metal_ok(t)))) {
        return MetalDecodeDecline.graph
    }
    if (kernel_backend_needs_repack(active_kernel_backend())) {
        return MetalDecodeDecline.backend_repack
    }
    if (!get_rope_table_mode()) {
        return MetalDecodeDecline.rope_mode
    }
    if (metal_missing_needs(t, needs_ok) != default<MetalNeed>) {
        return MetalDecodeDecline.feature
    }
    let c = t.config
    let hidden = layer_hidden(t, 0l)
    // sliding masking lives on the CHUNKED path only — window >= 128 (the single-form depth ceiling's clamp max) guarantees the single form never sees a masked row; decline tighter
    if (((c.dim % 32l) != 0l || (hidden % 32l) != 0l) ||
            (c.sliding_window > 0l && c.sliding_window < 128l)) {
        return MetalDecodeDecline.shape
    }
    // per-CLASS: whole simdgroups; red[16] + the epl <= 16 q-register hoist cap the threadgroup width at 512 (gemma4's global class)
    for (l in range64(c.n_layers)) {
        if (layer_is_recurrent(c, l)) {   // no KV, no attention tensors — only the FFN uniformity applies
            if (layer_hidden(t, l) != hidden) {
                return MetalDecodeDecline.layers
            }
            continue
        }
        let hs_l = layer_head_size(c, l)
        if ((hs_l % 32l) != 0l || hs_l > 512l || (layer_kv_dim(c, l) % 32l) != 0l) {
            return MetalDecodeDecline.shape
        }
        // E-series KV sharing: only the single-stream driver has the Q-only arm (allow_kv_share)
        if ((t.kv_src[l] != l && !(allow_kv_share && kv_share_ok(t, l))) || layer_hidden(t, l) != hidden) {
            return MetalDecodeDecline.layers
        }
        // a missing attn_v (V = the pre-norm K projection) serves — raw copy without v_norm; both encoded (the CPU mm_qkv shapes)
    }
    return MetalDecodeDecline.none
}

// mint-time servability (register_metal_servable): shape/graph/feature gates only — the planar
// check is a RUNTIME gate. Tied-fp32 classifiers decline: the blob drivers force GPU logits
// and have no fp32-table GEMV.
def private metal_model_servable_impl(t : Model) : bool {
    let cls_fmt = t.config.shared_weights ? t.emb_fmt : t.wcls_fmt
    // tied-fp32 classifier declines (no fp32-table GEMV); NextN serves — prefill stashes mtp_h + warms the slab
    if (t.config.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8) {
        return false
    }
    return decode_shape_decline(t, DECODE_NEEDS_OK, true, true) == MetalDecodeDecline.none
}

[init]
def private register_shape_gates {
    // pure model shape, registered on EVERY build — a non-Apple converter can bake the metal flavor
    register_metal_servable(@@metal_model_servable_impl)
}
