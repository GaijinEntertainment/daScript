options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_metal_shapes shared public

require dasllama/dasllama_lint public
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
    graph               //! a block set past the std dense stack (MoE / recurrent / custom attn) this path's kernels do not cover
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
    kq_lane             // the block GEMMs are the carrier's own K-quant planes (GigaAM's kq lane): the FastConformer chain has no GEMM for them
    shape               // a dim violating the GEMM grids (m%32 / ndim%64 / kdim%32) or a misaligned plane offset
    device              // metal init failed (no device / PSO build failure)
    gpu_error           // dispatch failed at execution time
    memory              // a pinned scratch buffer the encode needs came back null: the tower stays on the CPU chain
}

// The whisper ASR-decoder driver's decline set — FIRST failing clause, .none = supported,
// names asserted as strings by the gates. Q8-NATIVE: an f32-forced decoder declines.
// knob/small/step_knob are POLICY (no panic, outside the declines sum); the rest honor required.
enum MetalWdecDecline {
    none                // supported — the decoder-side work runs on the GPU
    knob                // DASLLAMA_METAL_WDEC=0 — the A/B rail (policy: never panics)
    small               // the decode STEP only: n_text_state under the serving floor — the
                        // per-dispatch latency beats small decoders (policy; cross-KV serves)
    step_knob           // the decode STEP only: DASLLAMA_METAL_WDEC_STEP=0 (policy; cross-KV
                        // serves — the note is what separates "off by config" from "broken")
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
    graph               //! a block set past the std dense stack (MoE / recurrent / custom attn) this path's kernels do not cover
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

def metal_missing_needs(t : Model; ok : MetalNeed) : MetalNeed => metal_needs(t) & ~ok

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

//! the dense plane formats the Metal kernels read in their own bytes - any other format (mx4) declines kquant_native
//! instead of reaching a dispatcher, whose default arm reads a superblock layout
def kq_fmt_gpu_supported(f : KqFmt) : bool =>
    (f == KqFmt.q8 || f == KqFmt.k4 || f == KqFmt.k5 || f == KqFmt.k6 || f == KqFmt.iq4xs || f == KqFmt.k3 || f == KqFmt.iq3s || f == KqFmt.iq3xxs
        || f == KqFmt.iq4nl || f == KqFmt.q40 || f == KqFmt.nv4 || f == KqFmt.k2 || f == KqFmt.iq2s || f == KqFmt.iq2xs || f == KqFmt.iq2xxs || f == KqFmt.iq3s4
        || kq_dense_b32(f))

//! the formats the K-quant token-table gather has a stamp for: every
//! served plane format but q8
def emb_kq_gather_fmt(f : KqFmt) : bool => f != KqFmt.q8 && kq_fmt_gpu_supported(f)

//! a table plane in format `fmt` the gather family serves: the q8 linear copy, a per-32 plane or a K-quant plane with a stamp - every
//! plane format the Metal kernels read
def emb_table_gather_fmt(f : KqFmt) : bool => kq_fmt_gpu_supported(f)

//! the E-series pre-step serves on the device for this blob model: its table in a format the gather family serves, bound on
//! the format's own block grid (the table's base and its row of layers x ple each whole blocks of the format)
def metal_ple_dev_ok(t : Model) : bool => has_ple(t.config) && empty(metal_ple_pre_unserved(t))

//! why the device does not serve the E-series pre-step for this model, named by site, `KqFmt` member and shape; "" when it does
//! or the model has no PLE. A planar load or a trimmed image serves no Metal step at all; every other shape the gather reads
//! on its block grid, the finish walks any ple width and the projection any model dim
def metal_ple_pre_unserved(t : Model) : string {
    return "" if (!has_ple(t.config))
    let site = kq_site_words("per_layer_token_embd")
    return "the {site} is not blob-streamed" if (!t.metal_blob)
    return "the {site}'s planes are trimmed" if (t.planes_trimmed)
    return "the {site} plane is {t.ple_emb_fmt}, a format the gather family does not read" if (!emb_table_gather_fmt(t.ple_emb_fmt))
    let all = t.config.n_layers * t.config.n_embd_per_layer
    let unit = kq_unit(t.ple_emb_fmt)
    if (t.ple_emb_fmt != KqFmt.q8 && (all % unit != 0l || t.ple_emb_off % unit != 0l)) {
        return "the {site} plane ({t.ple_emb_fmt}) binds off its {unit}-element blocks: {all} elements a row at offset {t.ple_emb_off}"
    }
    return ""
}

var private g_ple_decline_said : string   //! the last pre-step decline logged: a decline repeats every step, the line once

//! the pre-step's device verdict for the Metal `kind` driver about to serve this model: `metal_ple_dev_ok`, with a CPU verdict's
//! reason logged once while the same one repeats, so the fallback never runs unnamed
[cold_path]
def metal_ple_dev_said(kind : string; t : Model) : bool {
    let why = metal_ple_pre_unserved(t)
    return has_ple(t.config) if (empty(why))
    let line = "dasLLAMA metal {kind}: the E-series pre-step declined - {why}; the CPU serves the pre-step"
    if (line != g_ple_decline_said) {
        g_ple_decline_said = line
        to_log(LOG_WARNING, "{line}\n")
    }
    return false
}

//! a plane format with a one-row GEMV on this home: q8's, a superblock format's row-batched decode form at one row, or a per-32
//! plane's GEMV, whose K (`kdim`) is whole blocks of the format's unit and whose format the Metal kernels serve
def kq_fmt_one_row_gemv(f : KqFmt; kdim : int64) : bool =>
    (f == KqFmt.q8 || ((kq_sb(int(f)) || kq_dense_b32(f)) && kq_fmt_gpu_supported(f)
        && kdim % kq_unit(f) == 0l))

//! the first layer of `a` whose plane format no Metal kernel reads, named by layer, site and `KqFmt` member; "" when every one has a kernel
//! one dense site's planes by layer: the first whose format no Metal kernel reads, or whose weights ride a per-tensor `.scale`,
//! named by layer, site and `KqFmt` member; "" when every plane serves
def private dense_fmts_unserved(t : Model; a : array<KqFmt>; offs : array<int64>; role : string) : string {
    for (f, l in a, count()) {
        return "layer {l}'s {kq_site_words(role)} plane is {f}" if (!kq_fmt_gpu_supported(f))
        let why = plane_scale_unserved(t, f, offs, l, kq_site_words(role))
        return why if (!empty(why))
    }
    return ""
}

//! the words for layer `l`'s plane of one site when its weights ride a per-tensor `.scale` (`Model.site_scale`, keyed by format
//! and plane offset) - the Metal kernels read a plane's own bytes and apply no epilogue factor; "" otherwise
def private plane_scale_unserved(t : Model; f : KqFmt; offs : array<int64>; l : int; words : string) : string {
    return "" if (l >= length(offs))
    return scale_unserved_at(t, f, offs[l], l, words)
}

//! the same words for layer `l`'s plane at `off` - a routed stack's first expert, a shared-expert plane
def private scale_unserved_at(t : Model; f : KqFmt; off : int64; l : int; words : string) : string {
    return "" if (empty(t.site_scale))
    return "layer {l}'s {words} plane is {f} under a per-tensor .scale" if (key_exists(t.site_scale, site_scale_key(f, off)))
    return ""
}

//! the first dense or table plane of a kq load no Metal kernel reads in its own bytes, named by site and `KqFmt` member; "" when
//! every one has a kernel - the decode, batch and prefill gates decline `kquant_native` on it
def kq_load_gpu_unserved(t : Model) : string {
    return "" if (!t.kquant_native)
    var why = dense_fmts_unserved(t, t.wq_fmt, t.wq_offs, "attn_q")
    why = empty(why) ? dense_fmts_unserved(t, t.wk_fmt, t.wk_offs, "attn_k") : why
    why = empty(why) ? dense_fmts_unserved(t, t.wv_fmt, t.wv_offs, "attn_v") : why
    why = empty(why) ? dense_fmts_unserved(t, t.wo_fmt, t.wo_offs, "attn_output") : why
    why = empty(why) ? dense_fmts_unserved(t, t.w1_fmt, t.w1_offs, "ffn_gate") : why
    why = empty(why) ? dense_fmts_unserved(t, t.w3_fmt, t.w3_offs, "ffn_up") : why
    why = empty(why) ? dense_fmts_unserved(t, t.w2_fmt, t.w2_offs, "ffn_down") : why
    return why if (!empty(why))
    return "the {kq_site_words("output")} plane is {t.wcls_fmt}" if (!kq_fmt_gpu_supported(t.wcls_fmt))
    return "the {kq_site_words("token_embd")} plane is {t.emb_fmt}" if (!kq_fmt_gpu_supported(t.emb_fmt))
    return ""
}

def kq_load_gpu_supported(t : Model) : bool => empty(kq_load_gpu_unserved(t))

//! some dense plane - attention, FFN, classifier - is a per-32 format (q51, iq4nl32)
def kq_dense_b32_any(t : Model) : bool {
    return (kq_fmts_b32_any(t.wq_fmt) || kq_fmts_b32_any(t.wk_fmt) || kq_fmts_b32_any(t.wv_fmt) || kq_fmts_b32_any(t.wo_fmt)
        || kq_fmts_b32_any(t.w1_fmt) || kq_fmts_b32_any(t.w2_fmt) || kq_fmts_b32_any(t.w3_fmt)
        || kq_dense_b32(t.wcls_fmt))
}

def private kq_fmts_b32_any(a : array<KqFmt>) : bool {
    for (f in a) {
        return true if (kq_dense_b32(f))
    }
    return false
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

// Wave D serve gate for the deltanet kernel set: fixed d_state 128 (the scan/l2/gate lane math
// bakes 32 lanes x 4), conv taps <= 8 (hist registers). Every projection plane rides its tag's
// site GEMV; f32-on-disk β/α (dn_ba_f32) rides the router-slab f32 GEMV off the fblob offsets.
def dn_metal_ok(t : Model) : bool {
    let c = t.config
    if (c.ssm_d_state != 128l || c.ssm_d_conv < 2l || c.ssm_d_conv > 8l) {
        return false
    }
    return empty(dn_fmts_unserved(t))
}

//! layer `l`'s plane (`words`) in format `f` named by layer, site and `KqFmt` member where this home has no one-row GEMV for it at
//! K `kdim`; "" where it has one
def private one_row_unserved(l : int64; words : string; f : KqFmt; kdim : int64) : string =>
    kq_fmt_one_row_gemv(f, kdim) ? "" : "layer {l}'s {words} plane is {f}"

//! the first recurrent layer's deltanet plane this home has no one-row GEMV for, named by layer, site and `KqFmt` member; "" when
//! every one has its GEMV - `dn_metal_ok` refuses the hybrid on it, and the gates decline `graph`
def dn_fmts_unserved(t : Model) : string {
    let c = t.config
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        continue if (!layer_is_recurrent(c, l))
        var why = one_row_unserved(l, "deltanet qkv", fmt_at(t.dnqkv_fmt, l), c.dim)
        why = empty(why) ? one_row_unserved(l, "deltanet gate", fmt_at(t.dngate_fmt, l), c.dim) : why
        why = empty(why) ? one_row_unserved(l, kq_site_words("ssm_out"), fmt_at(t.dnout_fmt, l), c.ssm_d_inner) : why
        why = empty(why) ? one_row_unserved(l, kq_site_words("ssm_beta"), fmt_at(t.dnbeta_fmt, l), c.dim) : why
        why = empty(why) ? one_row_unserved(l, kq_site_words("ssm_alpha"), fmt_at(t.dnalpha_fmt, l), c.dim) : why
        why = empty(why) ? plane_scale_unserved(t, fmt_at(t.dnqkv_fmt, l), t.dnqkv_offs, int(l), "deltanet qkv") : why
        why = empty(why) ? plane_scale_unserved(t, fmt_at(t.dngate_fmt, l), t.dngate_offs, int(l), "deltanet gate") : why
        why = empty(why) ? plane_scale_unserved(t, fmt_at(t.dnout_fmt, l), t.dnout_offs, int(l), kq_site_words("ssm_out")) : why
        why = empty(why) ? plane_scale_unserved(t, fmt_at(t.dnbeta_fmt, l), t.dnbeta_offs, int(l), kq_site_words("ssm_beta")) : why
        why = empty(why) ? plane_scale_unserved(t, fmt_at(t.dnalpha_fmt, l), t.dnalpha_offs, int(l), kq_site_words("ssm_alpha")) : why
        return why if (!empty(why))
    }
    return ""
}

//! the window's β/α rows: a K-quant plane rides its batched expert-form GEMV (one plane, every row a stream), so its
//! format needs that form (`moe_fmt_metal_served`); q8 planes ride `pf_enc_dn_ba`, f32 ones the slab GEMV
def dn_ba_metal_window_ok(t : Model) : bool => empty(dn_ba_window_fmts_unserved(t))

//! the first recurrent layer's β/α plane whose format has no batched expert-form GEMV, named by layer, site and `KqFmt` member;
//! "" when both have it on every layer - the prefill declines `graph` on it
def dn_ba_window_fmts_unserved(t : Model) : string {
    let c = t.config
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        continue if (!layer_is_recurrent(c, l))
        let fb = fmt_at(t.dnbeta_fmt, l)
        let fa = fmt_at(t.dnalpha_fmt, l)
        return "layer {l}'s {kq_site_words("ssm_beta")} plane is {fb}" if (!moe_fmt_metal_served(fb))
        return "layer {l}'s {kq_site_words("ssm_alpha")} plane is {fa}" if (!moe_fmt_metal_served(fa))
    }
    return ""
}

// (q8 32-blocks, kq 256-superblocks — the expert shift is index math, no base multiple)
[arch(at = "../ARCHITECTURE_GPU_QUANT_PLANES.md#metal-kq-split-scale-plane")]
def moe_site_ok(fmt : KqFmt; off, ege : int64) : bool {
    if (fmt == KqFmt.q8) {
        return off >= 0l && (off % 256l) == 0l && (ege % 32l) == 0l
    }
    if (fmt == KqFmt.k4 || fmt == KqFmt.k5 || fmt == KqFmt.q40 || fmt == KqFmt.iq4nl || fmt == KqFmt.nv4 || fmt == KqFmt.k2) {   // compact 16B scale rows, or k2's 4B d tail: every bind is aligned at a whole superblock
        return off >= 0l && (off % 256l) == 0l && (ege % 256l) == 0l
    }
    if (fmt == KqFmt.k6 || fmt == KqFmt.iq4xs || fmt == KqFmt.iq3s4 || fmt == KqFmt.k3 || fmt == KqFmt.iq3s || fmt == KqFmt.iq3xxs || fmt == KqFmt.iq2s || fmt == KqFmt.iq2xs || fmt == KqFmt.iq2xxs) {
        return off >= 0l && (off % 512l) == 0l && (ege % 256l) == 0l
    }
    if (fmt == KqFmt.q51) {   // per-32 planes: off % 128 keeps the 20B/4B binds 16B-aligned
        return off >= 0l && (off % 128l) == 0l && (ege % 32l) == 0l
    }
    if (fmt == KqFmt.iq4nl32 || fmt == KqFmt.q4032 || fmt == KqFmt.q5032) {   // per-32 planes: off % 256 keeps the 16B (20B on q5032) / 2B binds 16B-aligned
        return off >= 0l && (off % 256l) == 0l && (ege % 32l) == 0l
    }
    return false
}

//! the expert plane formats the Metal routed block has kernels for at decode and prefill (the mx4 planes key off
//! `experts_mx4`, a stack property, and ride their own arm); any other tag declines `graph`
def moe_fmt_metal_served(f : KqFmt) : bool {
    return (f == KqFmt.q8 || f == KqFmt.q51 || f == KqFmt.iq4nl32 || f == KqFmt.q4032 || f == KqFmt.q5032 || f == KqFmt.k4 || f == KqFmt.k5 || f == KqFmt.k6 || f == KqFmt.k3 || f == KqFmt.q40
        || f == KqFmt.nv4 || f == KqFmt.iq4xs || f == KqFmt.iq3s || f == KqFmt.iq3xxs || f == KqFmt.iq2s || f == KqFmt.iq2xs || f == KqFmt.iq2xxs
        || f == KqFmt.k2 || f == KqFmt.iq4nl || f == KqFmt.iq3s4)
}

//! layer `l`'s first routed stack (gate `f1`, up `f3`, down `f2`) whose format the Metal routed block has no kernel for, named by
//! layer, site and `KqFmt` member; "" when it has one for all three
def private moe_layer_fmt_unserved(l : int64; f1, f3, f2 : KqFmt) : string {
    return "layer {l}'s {kq_site_words("ffn_gate_exps")} plane is {f1}" if (!moe_fmt_metal_served(f1))
    return "layer {l}'s {kq_site_words("ffn_up_exps")} plane is {f3}" if (!moe_fmt_metal_served(f3))
    return "layer {l}'s {kq_site_words("ffn_down_exps")} plane is {f2}" if (!moe_fmt_metal_served(f2))
    return ""
}

//! the first routed stack of a MoE whose format the Metal routed block has no kernel for, named by layer, site and `KqFmt` member;
//! "" when every stack has one (an mx4 stack rides its own arm) - `moe_metal_ok` declines `graph` on it
def moe_fmts_unserved(t : Model) : string {
    return "" if (!t.config.moe || t.experts_mx4)
    for (l in range64(t.config.n_layers + t.config.n_layer_nextn)) {
        let why = moe_layer_unserved(t, l)
        return why if (!empty(why))
    }
    return ""
}

//! layer `l`'s routed stacks: the first whose format the routed block has no kernel for, or whose weights ride a per-expert
//! `.scale` no Metal kernel folds, named by site and `KqFmt` member; "" when all three serve
def private moe_layer_unserved(t : Model; l : int64) : string {
    let f1 = fmt_at(t.we1_fmt, l)
    let f3 = fmt_at(t.we3_fmt, l)
    let f2 = fmt_at(t.we2_fmt, l)
    var why = moe_layer_fmt_unserved(l, f1, f3, f2)
    why = empty(why) ? plane_scale_unserved(t, f1, t.we1_offs, int(l), kq_site_words("ffn_gate_exps")) : why
    why = empty(why) ? plane_scale_unserved(t, f3, t.we3_offs, int(l), kq_site_words("ffn_up_exps")) : why
    return empty(why) ? plane_scale_unserved(t, f2, t.we2_offs, int(l), kq_site_words("ffn_down_exps")) : why
}

//! the first layer's sigmoid-gated shared expert plane this home has no one-row GEMV for, named by layer, site and `KqFmt` member;
//! "" when every one has its GEMV, or the MoE carries no such branch - `moe_metal_ok` declines `graph` on it
def shexp_fmts_unserved(t : Model) : string {
    let c = t.config
    return "" if (!c.moe || c.n_ff_shexp <= 0l || c.moe_dense_shexp)
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        let g = shexp_plane(t, l, 1)
        let u = shexp_plane(t, l, 3)
        let d = shexp_plane(t, l, 2)
        var why = one_row_unserved(l, kq_site_words("ffn_gate_shexp"), g.f, c.dim)
        why = empty(why) ? one_row_unserved(l, kq_site_words("ffn_up_shexp"), u.f, c.dim) : why
        why = empty(why) ? one_row_unserved(l, kq_site_words("ffn_down_shexp"), d.f, c.n_ff_shexp) : why
        why = empty(why) ? scale_unserved_at(t, g.f, g.off, int(l), kq_site_words("ffn_gate_shexp")) : why
        why = empty(why) ? scale_unserved_at(t, u.f, u.off, int(l), kq_site_words("ffn_up_shexp")) : why
        why = empty(why) ? scale_unserved_at(t, d.f, d.off, int(l), kq_site_words("ffn_down_shexp")) : why
        return why if (!empty(why))
    }
    return ""
}

//! the words a Metal decline `why` adds beside its enum value: the site and `KqFmt` member of the plane the gate stopped at, under
//! `kquant_native` (a dense or table plane) and `graph` (a routed stack, a shared expert, a deltanet plane, and on the `prefill`
//! window the β/α planes' batched form); "" for a decline no plane format decided
def metal_fmt_decline_words(kind, why : string; t : Model) : string {
    var site = ""
    if (why == "kquant_native") {
        site = kq_load_gpu_unserved(t)
    } elif (why == "graph") {
        site = moe_fmts_unserved(t)
        site = empty(site) ? shexp_fmts_unserved(t) : site
        site = empty(site) ? dn_fmts_unserved(t) : site
        site = empty(site) && kind == "prefill" ? dn_ba_window_fmts_unserved(t) : site
    }
    return empty(site) ? "" : "{why} - {site}, a format the Metal kernels do not read there"
}

//! the line a Metal driver logs for a decline whose `metal_fmt_decline_words` name a plane; "" for one they do not
def metal_fmt_decline_line(kind, why : string; t : Model) : string => fmt_line(kind, metal_fmt_decline_words(kind, why, t))

def private fmt_line(kind, words : string) : string => empty(words) ? "" : "dasLLAMA metal {kind}: declined {words}; the CPU serves the call"

var private g_fmt_decline_said : string   //! the last `metal_fmt_decline_line` logged: a decline repeats every step, the line once

//! log `metal_fmt_decline_line` for a decline the driver is about to count, once while the same line repeats; the words it names,
//! which the required-mode panic carries ("" for a decline no plane format decided)
[cold_path]
def fmt_decline_say(kind, why : string; t : Model) : string {
    let words = metal_fmt_decline_words(kind, why, t)
    let line = fmt_line(kind, words)
    if (!empty(line) && line != g_fmt_decline_said) {
        g_fmt_decline_said = line
        to_log(LOG_WARNING, "{line}\n")
    }
    return words
}

//! `fmt_decline_say` under a driver's decline enum (`MetalDecodeDecline`, `MetalPrefillDecline`), its name spelled off the hot path
[cold_path]
def metal_fmt_decline_say(kind : string; why : auto(E); t : Model) : string => fmt_decline_say(kind, "{why}", t)

//! The MoE serve gate: plain routed FFN (qwen3moe), the sigmoid-gated shared expert (qwen35moe), gemma4's dense-shared branch,
//! gpt-oss's gate (softmax_weight, biases) over mx4 experts; no dense lead or selection bias. The stack format is its own axis:
//! mx4 planes serve under every gate shape, disk-order only (the CPU laneq repack has no GPU carry).
def moe_metal_ok(t : Model) : bool {   // nolint:STYLE038 — flat per-shape refusal ladder; every arm is one reason
    let c = t.config
    if (!c.moe || c.n_layer_dense_lead > 0l || c.moe_exp_probs ||
            (t.experts_mx4 && t.mx4_repacked)) {
        return false
    }
    if (t.experts_mx4 && c.moe_gate == MoeGate.softmax_weight) {
        if (c.ffn_act != FfnAct.swiglu_oai ||
                !c.moe_router_bias || !c.moe_exps_bias || c.moe_dense_shexp || c.n_ff_shexp > 0l ||
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
        // sigmoid-gated shared expert (qwen35moe): each plane rides the dense site kernels by its tag - the q8 transcode at
        // base + l*she (base + stride %256, the bind) or the plane in the file's format; nsh %64 (prefill C tiles)
        let she = c.dim * c.n_ff_shexp
        if (!c.moe_shexp_gated ||
                (c.n_ff_shexp % 64l) != 0l || (she % 256l) != 0l ||
                (t.wsh1_off % 256l) != 0l || (t.wsh2_off % 256l) != 0l || (t.wsh3_off % 256l) != 0l ||
                !empty(shexp_fmts_unserved(t))) {
            return false
        }
    }
    let ne = c.n_expert
    let k = c.n_expert_used
    let nfe = c.n_ff_exp
    let ege = c.dim * nfe
    // the select stamps hold 8 or 16 logits per lane (ne <= 512); %64 dims = the gathered mul_mm C tiles
    if (ne < 1l || ne > 512l || k < 1l || k > 16l || k > ne || nfe <= 0l ||
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
        for (l in range64(c.n_layers + c.n_layer_nextn)) {
            if (t.we1_offs[l] < 0l || (t.we1_offs[l] % 128l) != 0l ||
                    t.we2_offs[l] < 0l || (t.we2_offs[l] % 128l) != 0l ||
                    t.we3_offs[l] < 0l || (t.we3_offs[l] % 128l) != 0l) {
                return false
            }
        }
        return true
    }
    for (l in range64(c.n_layers + c.n_layer_nextn)) {   //! the NextN head's routed stacks dispatch on the draft step like a trunk layer's
        let f1 = fmt_at(t.we1_fmt, l)
        let f2 = fmt_at(t.we2_fmt, l)
        let f3 = fmt_at(t.we3_fmt, l)
        return false if (!empty(moe_layer_unserved(t, l)))
        // per-site alignment + reduction dims: superblock kq kernels iterate whole 256-rows,
        // the per-32 formats whole 32-blocks (+ the mul_mm's 64-col output tiles)
        let sb1 = kq_sb(f1)
        let sb3 = kq_sb(f3)
        let sb2 = kq_sb(f2)
        if (!moe_site_ok(f1, t.we1_offs[l], ege) ||
                !moe_site_ok(f2, t.we2_offs[l], ege) ||
                !moe_site_ok(f3, t.we3_offs[l], ege) ||
                ((sb1 || sb3) && (c.dim % 256l) != 0l) ||
                (sb2 && (nfe % 256l) != 0l) ||
                ((kq_dense_b32(f1) || kq_dense_b32(f3))
                    && ((c.dim % 32l) != 0l || (nfe % 64l) != 0l)) ||
                (kq_dense_b32(f2) && ((nfe % 32l) != 0l || (c.dim % 64l) != 0l))) {
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
//! the batch serves every feature the single row does - the hybrid's gated q and partial rope, the E-series PLE branch, all on the rows form
let BATCH_NEEDS_OK = DECODE_NEEDS_OK

//! a routed model with no dense lead and no dense shared branch dispatches none, so its file's dense width gates nothing
def has_dense_ffn(t : Model) : bool => t.blocks.ffn_is_dense || t.config.n_layer_dense_lead > 0l || t.config.moe_dense_shexp

//! The mul_mm tile gate: every served width on the 64 lattice - the dense FFN width only where
//! a dense FFN dispatches, the second MatFormer width where one exists, both attention widths
//! on both head classes
def mm_tile_widths_64_ok(t : Model) : bool {
    let c = t.config
    let hs_sw = c.head_size_swa > 0l ? c.head_size_swa : c.head_size
    let kvh_sw = c.n_kv_heads_swa > 0l ? c.n_kv_heads_swa : c.n_kv_heads
    let h1 = ffn_second_hidden(t)
    return ((c.dim % 64l) == 0l && (!has_dense_ffn(t) || (layer_hidden(t, 0l) % 64l) == 0l) &&
        h1 >= 0l && (h1 % 64l) == 0l && (!has_ple(c) || (c.n_embd_per_layer % 64l) == 0l) &&
        ((c.n_heads * c.head_size) % 64l) == 0l && ((c.n_kv_heads * c.head_size) % 64l) == 0l &&
        ((c.n_heads * hs_sw) % 64l) == 0l && ((kvh_sw * hs_sw) % 64l) == 0l)
}

//! The second distinct per-layer FFN width (MatFormer E-series: a prefix at one width, the rest
//! at another). 0 = uniform; -1 = more than two widths — no served form.
def ffn_second_hidden(t : Model) : int64 {
    let h0 = layer_hidden(t, 0l)
    var h1 = 0l
    for (l in range64(t.config.n_layers)) {
        let hl = layer_hidden(t, l)
        if (hl != h0) {
            if (h1 != 0l && hl != h1) {
                return -1l
            }
            h1 = hl
        }
    }
    return h1
}

def decode_shape_decline(t : Model; needs_ok : MetalNeed; allow_moe : bool = false; allow_kv_share : bool = false; allow_hetero_hidden : bool = false; mint_time : bool = false; allow_hc : bool = false) : MetalDecodeDecline {
    if (t.quant != QuantMode.q8) {
        return MetalDecodeDecline.quant_mode
    }
    if (!kq_load_gpu_supported(t)) {   // a fmt without a GPU kernel (the q4_0 QAT tier, ...) must not dispatch
        return MetalDecodeDecline.kquant_native
    }
    // non-std graphs decline; MoE (Wave C) + deltanet hybrid (Wave D) serve when their kernels cover it;
    // the hyper-connection residual (qwen4exp) has the single row's form alone (allow_hc)
    if ((t.config.hyper_conn && !allow_hc) ||
            (!t.blocks.attn_is_std && !(t.config.hybrid_deltanet && dn_metal_ok(t))) ||
            (!t.blocks.ffn_is_dense && !(allow_moe && moe_metal_ok(t)))) {
        return MetalDecodeDecline.graph
    }
    if (!mint_time && kernel_backend_needs_repack(active_kernel_backend())) {
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
    // MatFormer E-series: at most TWO per-layer FFN widths serve, and only where the loop binds
    // the width per layer — the single-stream driver, dense trunks, no MTP (verify/warm hoist)
    let h1 = ffn_second_hidden(t)
    if (h1 != 0l && (h1 < 0l || !allow_hetero_hidden || !t.blocks.ffn_is_dense || c.n_layer_nextn > 0l)) {
        return MetalDecodeDecline.layers
    }
    // sliding masking lives on the CHUNKED path only — window >= 128 (the single-form depth ceiling's clamp max) guarantees the single form never sees a masked row; decline tighter
    if (((c.dim % 32l) != 0l || (has_dense_ffn(t) && (hidden % 32l) != 0l) || (h1 > 0l && (h1 % 32l) != 0l) ||
                (has_ple(c) && (c.n_embd_per_layer % 32l) != 0l)) ||   //! the PLE sites' own K and N: the GEMM forms step 32-wide tiles
            (c.sliding_window > 0l && c.sliding_window < 128l)) {
        return MetalDecodeDecline.shape
    }
    // per-CLASS: whole simdgroups; red[16] + the epl <= 16 q-register hoist cap the threadgroup width at 512 (gemma4's global class)
    for (l in range64(c.n_layers)) {
        if (layer_is_recurrent(c, l)) {   // no KV, no attention tensors — the width pair above covers the FFN
            continue
        }
        let hs_l = layer_head_size(c, l)
        if ((hs_l % 32l) != 0l || hs_l > 512l || (layer_kv_dim(c, l) % 32l) != 0l) {
            return MetalDecodeDecline.shape
        }
        // E-series KV sharing: a driver with the Q-only arm passes allow_kv_share (the single-row and the batched step both do)
        if (t.kv_src[l] != l && !(allow_kv_share && kv_share_ok(t, l))) {
            return MetalDecodeDecline.layers
        }
        // a missing attn_v (V = the pre-norm K projection) serves — raw copy without v_norm; both encoded (the CPU mm_qkv shapes)
    }
    return MetalDecodeDecline.none
}

// mint-time servability (register_metal_servable): shape/graph/feature gates only — the planar
// check is a RUNTIME gate. A tied classifier read off the fp32 table alone declines: the blob drivers
// force GPU logits and have no fp32-table GEMV (a Metal-mode load keeps a q8 copy of a table no KqFmt carries).
def private metal_model_servable_impl(t : Model) : bool {
    if (cls_tied_f32(t)) {
        return false
    }
    return decode_shape_decline(t, DECODE_NEEDS_OK, true, true, true, mint_time = true, allow_hc = true) == MetalDecodeDecline.none
}

[init]
def private register_shape_gates {
    // pure model shape, registered on EVERY build — a non-Apple converter can bake the metal flavor
    register_metal_servable(@@metal_model_servable_impl)
}
