options gen2
options _comment_hygiene = true

module dasllama_common shared public

require daslib/fio
require daslib/array_boost // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs and false-flags this as unused
require dasllama/dasllama_math
require dasllama/dasllama_math_default  // registers the portable Q8·Q8 backend at [init] (the fallback everywhere)
require dasllama/dasllama_math_aarch64_neon  // registers the arm64 SDOT Q8·Q8 backend at [init]; no-op off-ARM
require ?llvm/daslib/llvm_tune dasllama/dasllama_math_gen  // nolint:STYLE030 — [init]-only: registers "arm64-gen" (the generated GEMM family, arm64 load-select tier). PATH guard: the module hard-requires llvm_tune, so it loads exactly when dasLLVM is mounted
require ?llvm/daslib/llvm_tune llvm/daslib/llvm_tune public  // re-export ONLY llvm_tune (not math_gen's whole public surface — its jobque_boost would flood requirers with ambiguities): the auto-policy pass and tune_status reach every app that requires this module
require ?das_metal dasllama/dasllama_math_metal  // nolint:STYLE030 — [init]-only: registers "metal" (the GPU prefill-GEMM batch donor). STRICT guard: the C++ das_metal module exists on Apple builds only, so non-Apple lanes never compile the Metal driver
require ?vulkan dasllama/dasllama_math_vulkan  // nolint:STYLE030 — [init]-only: registers "vulkan" (the GPU tier; stage-0 skeleton, kernels land with the offload rail). STRICT guard: the C++ vulkan module is linked only where the dasVulkan package is installed, so CI lanes without it never compile the Vulkan driver
require dasllama/dasllama_quant
require dasllama/dasllama_gguf
require dasllama/dasllama_gemm_schema  // kq_qsb/kq_ssb — the fmt-id plane strides
require dasllama/dasllama_tokenizer  // Model owns its tokenizer; load_model populates it
require dasllama/dasllama_par  // maybe_parallel_for (+ get_total_hw_jobs): thread the prefill attention over heads
require dasllama/dasllama_tune  // box_profile_path: the runtime half of the app tune sidecar applies at load_model
require daslib/json_boost  // read_json / read_json_field for the box profile's "runtime" section
require math
require daslib/random  // int4 RNG state + random_float for token sampling
require daslib/strings_boost  // join() for the arch-registry diagnostic
require daslib/apply  // the Model finalizer + the image save/load walk both iterate Model fields

// dasLLAMA inference engine. A model is split into two regions by role: a small fp32 aux blob
// (the embedding table + the norm weights, which forward always reads as fp32) and the big 2D
// weight matrices (attn q/k/v/o, ffn gate/down/up, untied classifier). Only the big weights are
// quantized, and only ONE representation is held — so a Q8 model never materializes the fp32
// weights ("load as stored"). All big-weight arrays are reserve()'d to exact size to avoid the
// allocator's power-of-2 rounding.

//! Which weight representation forward routes its 2D matmuls through. fp32 is the default;
//! q8 selects the fast integer rails (K-quant / mxfp4 / Q4_0 files keep their native planes);
//! q4_0 is the legacy requant tier — weights held as q4_0 blocks, per-row kernels only.
enum QuantMode {
    fp32
    q8
    q4_0
}

//! Per-weight storage format under a q8-mode load with native K-quant planes (kquant_native):
//! q8 = qblob/qscales; k4/k5/k6 = the tensor's element offset indexes that format's plane pair. A
//! Q4_K_M file mixes formats PER TENSOR, so the tag rides per layer next to each offset array.
enum KqFmt : uint8 {
    q8
    k4
    k5
    k6
    q40
}

//! FFN gate activation: silu = SwiGLU (Llama family), gelu = GeGLU (Gemma), swiglu_oai = the
//! clamped SwiGLU with +1 on the up branch (gpt-oss).
enum FfnAct {
    silu
    gelu
    swiglu_oai
}

//! MoE router probability function: softmax over all experts (Qwen-MoE), per-expert sigmoid
//! (the DeepSeek/GLM shape), or softmax over just the selected top-k weights AFTER selection on
//! raw logits (gpt-oss / bailingmoe2). GGUF `{arch}.expert_gating_func` overrides the arch default.
enum MoeGate {
    softmax
    sigmoid
    softmax_weight
}

//! Prefill attention algorithm (A/B selectable; all variants kept). `classic` streams the full
//! causal K/V range per query, O(npos²). `blocked` reuses each K/V tile across a query block —
//! BIT-IDENTICAL to classic. `flash` (default) tiles with online softmax — NOT bit-identical.
enum AttnPrefillMode {
    classic
    blocked
    flash
}

//! Deltanet prefill algorithm (qwen35 hybrid). `recurrent` runs the per-token delta rule — the
//! bit-exact reference the parity fixtures pin. `chunked` (default) is the 64-token blocked form
//! (llama.cpp's default); NOT bit-identical (chunk-parallel FP reassociation), tolerance-validated.
enum DnPrefillMode {
    recurrent
    chunked
}

//! KV-cache storage codec, chosen per session at create time (llama.cpp -ctk/-ctv analog; K and V
//! carry independent dtypes). f16 = half the KV bytes, near-lossless; f32 = bit-exact reference;
//! q8_0 = block codec (34B/32 elements, needs kv_dim/head_size % 32); tq4 = rotated 4-bit (18B/32, needs head_size a power of two).
enum KVDtype {
    f32
    f16
    q8_0
    tq4
}

// tq4 cache-pointer dispatch tag: the read seam overloads on the cache pointer's element type, and
// q8_0 already owns `uint8 const?` — a distinct newtype over uint8 gives tq4 its own overload
// identity at zero runtime cost (byte-strided arithmetic, ABI identical to uint8).
typedef distinct TQ4B = uint8

//! Model hyperparameters (the 7-int checkpoint header plus derived sizes).
struct Config {
    dim : int64
    hidden_dim : int64          // dense FFN width; the MAX across layers when it varies per layer (scratch sizing)
    n_layers : int64
    n_heads : int64
    n_kv_heads : int64
    vocab_size : int64
    seq_len : int64
    head_size : int64       // dim / n_heads
    kv_dim : int64          // (dim * n_kv_heads) / n_heads
    shared_weights : bool   // classifier aliases the token-embedding table
    rope_freq_base : float  // RoPE θ (set by the loader); Llama-2/TinyLlama 10000, Llama-3 500000
    // per-architecture capability flags — defaults encode the Llama family; the loader overrides per arch
    attn_qkv_bias : bool = false       // Qwen2: additive bias on the Q/K/V projections
    rope_neox : bool = false           // NEOX rope (pairs offset by head_size/2): Qwen2 / Phi3 / Gemma2
    rope_mscale : float = 1.0          // LongRoPE magnitude factor on cos/sin (Phi3 attn_factor)
    fused_qkv : bool = false           // Phi3: attn_qkv + ffn_up (gate_up) stored fused, split at load
    ffn_act : FfnAct = FfnAct.silu     // SwiGLU (silu) vs GeGLU (gelu, Gemma2)
    embed_scale : float = 1.0          // token-embedding multiplier (Gemma2: sqrt(dim))
    norm_eps : float = 1.0e-5          // RMSNorm epsilon (Gemma2: 1e-6)
    attn_logit_softcap : float = 0.0   // attention-score soft cap, 0 = off (Gemma2: 50)
    final_logit_softcap : float = 0.0  // output-logit soft cap, 0 = off (Gemma2: 30)
    sliding_window : int64 = 0         // sliding-window attention span, 0 = off (Gemma2: 4096, alt layers)
    swa_pattern : int64 = 2            // every swa_pattern-th layer is global, the rest sliding (Gemma2: 2, Gemma3: 6)
    rope_freq_scale : float = 1.0      // linear RoPE position scale, angle = pos*scale*freq (Gemma3-4B: 1/8)
    rope_freq_base_swa : float = 0.0   // sliding layers' θ; 0 = same as rope_freq_base (Gemma3: 10000)
    rope_freq_scale_swa : float = 1.0  // sliding layers' position scale (Gemma3: 1.0 — unscaled)
    pre_post_norm : bool = false       // Gemma2: extra post-attention / post-FFN norms
    qk_norm : bool = false             // Qwen3: per-head RMSNorm on Q/K before RoPE
    embed_scale_sqrt_dim : bool = false // Gemma: scale token embeddings by sqrt(dim) — resolved by the loader (dim unknown at configure time)
    // Gemma-4 (heterogeneous per-layer attention geometry — sliding vs global layers differ in
    // head size AND kv-head count, so head_size/kv_dim above describe the GLOBAL class and the
    // layer_* accessors below pick per layer)
    swa_mask : uint64 = 0ul            // explicit per-layer sliding mask (bit l = layer l slides); 0 = derive from swa_pattern
    head_size_swa : int64 = 0          // sliding layers' head size; 0 = same as head_size (gemma4-12B: 256 vs 512)
    n_kv_heads_swa : int64 = 0         // sliding layers' KV-head count; 0 = same as n_kv_heads (gemma4-12B: 8 vs 1)
    attn_scale : float = 0.0           // attention-score scale; 0 = the default 1/sqrt(head_size) (gemma4: 1.0 — QK-norm replaces it)
    v_norm : bool = false              // weightless per-head RMSNorm on V after projection (gemma4)
    rope_ff_global_only : bool = false // rope_freqs factors apply only to GLOBAL layers (gemma4 p-RoPE)
    layer_out_scale : bool = false     // per-layer scalar on the layer output (gemma4 layer_output_scale; loader-detected)
    v_from_k_mask : uint64 = 0ul       // bit l: layer l has no attn_v tensor — V reuses the pre-norm K projection (gemma4 global layers)
    // gemma-4 E-series (E2B/E4B): per-layer embeddings (PLE) + cross-layer KV sharing (both loader-detected)
    n_embd_per_layer : int64 = 0       // PLE input dim (E2B/E4B: 256); 0 = no PLE
    n_layer_kv_from_start : int64 = 0  // first N layers own their KV; later layers reuse a source layer's KV (0 = all own)
    // MoE FFN (qwen2moe shape: routed top-k + shared expert). `moe` is the arch opt-in that lets the
    // loader read the {arch}.expert_* keys; the geometry comes from metadata, 0 = dense FFN
    moe : bool = false                 // arch opt-in: this arch's FFN is router + experts
    n_expert : int64 = 0               // routed expert count (qwen2moe A2.7B: 60)
    n_expert_used : int64 = 0          // experts routed per token, the top-k (qwen2moe: 4)
    n_ff_exp : int64 = 0               // per-expert FFN width (qwen2moe: 1408)
    n_ff_shexp : int64 = 0             // shared-expert FFN width; 0 = no shared expert (qwen2moe: 5632)
    moe_gate : MoeGate = MoeGate.softmax  // router probability function
    norm_topk_prob : bool = false      // renormalize the selected top-k weights to sum 1 ({arch}.expert_weights_norm)
    expert_weights_scale : float = 0.0 // scale on the routed weights; 0/1 = off (DeepSeek {arch}.expert_weights_scale)
    // glm4moe (DeepSeek-V3 lineage): dense lead + selection bias + ungated shared expert
    n_layer_dense_lead : int64 = 0     // leading DENSE FFN layers before MoE starts ({arch}.leading_dense_block_count; glm4moe-Air: 1)
    moe_exp_probs : bool = false       // exp_probs_b per-expert SELECTION bias — biased top-k pick, unbiased mixing (loader-detected)
    moe_shexp_gated : bool = true      // shared expert scaled by σ(gate_vec·x) (qwen2moe); glm4moe's shexp is UNGATED (loader-detected)
    // gemma4 MoE (a dense/routed hybrid): the arch is MoE only when the file ships experts (12B/31B
    // dense, 26B-A4B MoE) and its shape differs from qwen2moe — a parallel DENSE shared expert,
    // fused gate_up stacks, three extra sandwich norms, a custom-normalized router.
    moe_optional : bool = false        // arch MAY be MoE — read expert keys only if the file carries them (gemma4)
    moe_dense_shexp : bool = false     // shared expert is a DENSE parallel FFN + gemma4 sandwich norms + custom router
    // gpt-oss (all loader-detected from tensor presence, except the norm-name flag which the arch sets)
    attn_sinks : bool = false          // per-head sink logit joins the attention softmax denominator
    attn_out_bias : bool = false       // additive bias on the attention output projection
    moe_router_bias : bool = false     // ffn_gate_inp.bias on the router logits
    moe_exps_bias : bool = false       // per-expert ffn_{gate,up,down}_exps.bias
    post_attn_is_ffn_norm : bool = false // the pre-FFN norm tensor is named post_attention_norm (NOT Gemma2's extra post-norm)
    // qwen35 hybrid (Qwen 3.5/3.6): Gated-DeltaNet recurrent layers between gated-attention layers.
    // The three bools are arch opt-ins (configure); the rest is loader-filled from {arch}.ssm.* keys.
    hybrid_deltanet : bool = false     // arch opt-in: read the ssm keys + the recurrent-layer mask
    partial_rope : bool = false        // arch opt-in: rope.dimension_count < head_size rotates rope_dim dims
    q_gated : bool = false             // attn_q is 2x-wide — per head [q | gate]; out ⊙ σ(gate) before wo
    recr_mask : uint64 = 0ul           // bit l: layer l is recurrent (deltanet) — no KV, no attn tensors
    rope_dim : int64 = 0               // partial rotary width (qwen35: 64 of 256); 0 = rope the full head
    ssm_d_conv : int64 = 0             // deltanet conv kernel width (4)
    ssm_d_state : int64 = 0            // deltanet q/k/v head dim (128)
    ssm_n_group : int64 = 0            // deltanet k-head count (16)
    ssm_dt_rank : int64 = 0            // deltanet v-head count (0.8B: 16, 35B: 32)
    ssm_d_inner : int64 = 0            // deltanet value width = ssm_dt_rank * ssm_d_state
    // MTP/NextN (qwen35 family -MTP files): block_count includes the appended draft block(s).
    // n_layers stays the TRUNK count everywhere; the MTP block lives at layer index n_layers.
    n_layer_nextn : int64 = 0          // 0 = no MTP block; 1 = single NextN head (only value supported)
}

// Swappable block kernels — one per (attention|ffn) x (decode|prefill). Decode is the fused
// single-token path (forward); prefill is the batched path (forward_prefill). Declared before
// Model (which holds an ArchBlocks by value) so the AOT C++ emitter sees the complete type first.
typedef AttnDecodeFn  = function<(t : Model; var s : Session; l : int64; pos : int64) : void>
typedef FfnDecodeFn   = function<(t : Model; var s : Session; l : int64) : void>
typedef AttnPrefillFn = function<(t : Model; var s : Session; l : int64; npos : int64; start_pos : int64) : void>
typedef FfnPrefillFn  = function<(t : Model; var s : Session; l : int64; npos : int64) : void>

//! A whole-prefill override: claims the ENTIRE per-layer stack of one prefill. On true the KV
//! cache is filled for every position and x_b holds the final residual stream; false = declined.
//! Registered by name at [init], activated via select_prefill_override (env rail: DASLLAMA_PIN_PREFILL).
typedef PrefillOverrideFn = function<(t : Model; var s : Session; npos : int64; start_pos : int64) : bool>

// The prefill-override registry (see PrefillOverrideFn). Name-keyed like the kernel backends;
// nothing activates by default — an empty active name keeps the CPU layer loop.
var private g_prefill_overrides : table<string; PrefillOverrideFn>
var private g_prefill_override : PrefillOverrideFn
var private g_prefill_override_name = ""

def register_prefill_override(name : string; fn : PrefillOverrideFn) {
    g_prefill_overrides[name] = fn
}

//! Activate a registered whole-prefill override by name ("" deactivates). false = unknown name.
def select_prefill_override(name : string) : bool {
    if (empty(name)) {
        g_prefill_override_name = ""
        return true
    }
    if (!key_exists(g_prefill_overrides, name)) {
        to_log(LOG_ERROR, "dasLLAMA: unknown prefill override `{name}`\n")
        return false
    }
    g_prefill_override = g_prefill_overrides[name]
    g_prefill_override_name = name
    return true
}

def active_prefill_override : string {
    return g_prefill_override_name
}

// set by an override that ALSO produced the last position's logits; reset before every invocation
var private g_prefill_logits_done = false

//! A whole-prefill override that computed s.logits for the LAST position itself (final norm +
//! classifier on its device, softcap/suppression semantics preserved or declined) calls this
//! from inside its forward — forward_prefill_body then skips the CPU final-norm/classifier step.
def prefill_override_logits_done {
    g_prefill_logits_done = true
}

//! The decode twin of PrefillOverrideFn: claims the ENTIRE per-layer stack of one single-token
//! forward. On true the KV row is stored for `pos` and s.x holds the final residual; false =
//! declined. NOTE: forward() also serves 1-token prefill chunks, so `pos` is arbitrary.
typedef DecodeOverrideFn = function<(t : Model; var s : Session; token, pos : int64) : bool>

//! The batched-decode twin, invoked in eval_batch_ after the embed + per-row rope tables: on true
//! every row's KV is stored and ws.scr.x_b holds the final residuals. The CPU tail (final rms,
//! classifier, suppress, n_past++) stays with the caller — the override NEVER touches n_past.
typedef BatchDecodeOverrideFn = function<(t : Model; var ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool>

// The decode-override registries (see DecodeOverrideFn / BatchDecodeOverrideFn). Name-keyed like
// the prefill registry; nothing activates by default — an empty active name keeps the CPU loops.
var private g_decode_overrides : table<string; DecodeOverrideFn>
var private g_decode_override : DecodeOverrideFn
var private g_decode_override_name = ""
var private g_batch_decode_overrides : table<string; BatchDecodeOverrideFn>
var private g_batch_decode_override : BatchDecodeOverrideFn
var private g_batch_decode_override_name = ""

def register_decode_override(name : string; fn : DecodeOverrideFn) {
    g_decode_overrides[name] = fn
}

def register_batch_decode_override(name : string; fn : BatchDecodeOverrideFn) {
    g_batch_decode_overrides[name] = fn
}

//! Activate a registered whole-decode override by name ("" deactivates). The batch twin
//! registered under the same name (if any) activates alongside — a name with only the
//! single-stream form keeps eval_batch_ on the CPU path. false = unknown name.
def select_decode_override(name : string) : bool {
    if (empty(name)) {
        g_decode_override_name = ""
        g_batch_decode_override_name = ""
        return true
    }
    if (!key_exists(g_decode_overrides, name)) {
        to_log(LOG_ERROR, "dasLLAMA: unknown decode override `{name}`\n")
        return false
    }
    g_decode_override = g_decode_overrides[name]
    g_decode_override_name = name
    if (key_exists(g_batch_decode_overrides, name)) {
        g_batch_decode_override = g_batch_decode_overrides[name]
        g_batch_decode_override_name = name
    } else {
        g_batch_decode_override_name = ""
    }
    return true
}

def active_decode_override : string {
    return g_decode_override_name
}

//! Is a whole-prefill/decode override registered under `name`? (Apple builds register "metal"
//! at [init]; other platforms have none.)
def has_prefill_override(name : string) : bool {
    return key_exists(g_prefill_overrides, name)
}

def has_decode_override(name : string) : bool {
    return key_exists(g_decode_overrides, name)
}

//! How the Metal pipeline engages — ONE knob for all three GPU paths (prefill, single-stream
//! decode, batched decode).
enum MetalMode {
    off             //!< CPU loops only (deselects the "metal" overrides)
    when_available  //!< GPU when a path's gates pass, silent CPU fallback otherwise
    required        //!< GPU or fail: a capability decline (the GPU cannot serve) panics
}

var private g_metal_mode = MetalMode.off

//! Select how the Metal pipeline engages (see `MetalMode`). Any mode but `off` pins the row-major
//! "portable" kernel backend (call this BEFORE `load_model`, or the ARM repack backend interleaves
//! weights at load) and activates the "metal" overrides. `required` panics on any capability decline.
def set_metal_mode(mode : MetalMode) {
    if (mode == MetalMode.off) {
        select_prefill_override("")
        select_decode_override("")
        g_metal_mode = mode
        return
    }
    if (!has_prefill_override("metal") || !has_decode_override("metal")) {
        if (mode == MetalMode.required) {
            panic("dasLLAMA: metal mode `required`, but the metal overrides are not registered (non-Apple build or dasMetal missing)")
        }
        to_log(LOG_WARNING, "dasLLAMA: metal mode requested, but the metal overrides are not registered — staying on the CPU paths\n")
        g_metal_mode = MetalMode.off
        return
    }
    pin_kernel_backend("portable")
    select_prefill_override("metal")
    select_decode_override("metal")
    g_metal_mode = mode
}

def get_metal_mode : MetalMode {
    return g_metal_mode
}

// set by an override that ALSO produced this position's logits; reset before every invocation
var private g_decode_logits_done = false

//! A whole-decode override that computed s.logits itself (final norm + classifier on its device)
//! calls this from inside its forward — forward() then skips the CPU final-norm/classifier step.
def decode_override_logits_done {
    g_decode_logits_done = true
}

// the batch twin — reset before every eval_batch_ invocation
var private g_batch_decode_logits_done = false

//! A batch-decode override that filled EVERY sessions[i].logits itself calls this from inside its
//! forward — eval_batch_ then skips the CPU final-norm/classifier/scatter step (suppress pins and
//! the n_past advance still run there).
def batch_decode_override_logits_done {
    g_batch_decode_logits_done = true
}

//! Build one position's RoPE cos/sin row into caller arrays — bit-identical to the row forward()
//! hands the decode override (an encode-ahead driver pre-builds the NEXT position's row; the
//! global-rope class only — dual-rope configs carry a second row this does not cover).
def public build_decode_rope_row(t : Model; pos : int64; var cos_tab, sin_tab : array<float>) {
    let c = t.config
    build_rope_table(cos_tab, sin_tab, t, c.rope_freq_base, c.rope_freq_scale, pos, 1l,
                     c.rope_dim > 0l ? c.rope_dim : c.head_size, true)
}

//! The copyable hot-path block pointers, resolved onto the Model at load and called per layer.
struct ArchBlocks {
    attn_decode : AttnDecodeFn
    ffn_decode : FfnDecodeFn
    attn_prefill : AttnPrefillFn
    ffn_prefill : FfnPrefillFn
    // eval_batch gate: attention is the std QKV/RoPE/causal shape (attention_std_prefill), so the
    // batched-decode worker applies. An arch registering custom attention sets this false and
    // eval_batch falls back to per-row forward().
    attn_is_std : bool = true
}

//! The whole model. `fblob` holds the fp32 embedding + norm weights; the big 2D weights live in
//! exactly one of wblob (fp32) / qblob+qscales (Q8) / q4blob+q4scales (Q4) per `quant`. For
//! shared_weights, cls_q8 (tied Q8_0) drops the fp32 table: classifier + embedding both read qblob.
struct Model {
    config : Config = Config()
    arch : string = ""              // GGUF general.architecture, resolved at load — drives the chat template
    chat_template_str : string = "" // GGUF tokenizer.chat_template (verbatim Jinja source, "" if absent) — sniffed, never executed
    tok : Tokenizer = Tokenizer()   // tokenizer loaded alongside the weights (unset unless loaded via load_model)
    blocks : ArchBlocks = ArchBlocks()   // per-arch attention/ffn kernels, resolved from the registry at load
    // fp32 aux: embedding + norms
    fblob : array<float>
    tok_emb_off : int64
    rms_att_off : int64
    rms_ffn_off : int64
    rms_final_off : int64
    // Gemma2 post-norms (extra RMSNorm after attn-out and after FFN-out); offsets valid only
    // when config.pre_post_norm, otherwise unused
    rms_post_att_off : int64
    rms_post_ffn_off : int64
    // Qwen3 QK-norm weights (one head_size(l) vector per layer); per-layer offsets valid only when
    // config.qk_norm (sizes differ per layer class on gemma4, so these are arrays not strides)
    rms_q_offs : array<int64>
    rms_k_offs : array<int64>
    // gemma4: a max-head-size vector of 1.0s (the weightless V-norm reuses rms_batch), and the
    // per-layer layer_output_scale scalars — both in fblob, valid per the config flags
    rms_ones_off : int64
    out_scale_off : int64
    // token ids whose logits pin to -inf after the classifier (gemma4 suppress_tokens); empty = none
    suppress : array<int64>
    // gemma-4 E-series cross-layer KV sharing: per-layer KV SOURCE (== l for own-KV layers; an
    // earlier layer for shared layers, which carry no attn_k/attn_v of their own). Filled by
    // layout_offsets; kv_row_prefix aliases a shared layer's slot onto its source's.
    kv_src : array<int64>
    // gemma-4 E-series per-layer embeddings (PLE) — offsets valid only when config.n_embd_per_layer > 0.
    // The Q8 token table (linear, gathered per token — NOT repacked) + the per-layer Q8 gate/proj live
    // in the big-weight blob; the fp32 model-proj + the two norm families live in fblob.
    ple_emb_off : int64     // qblob: Q8 per_layer_token_embd, linear [vocab x (ple*layers)]
    ple_model_off : int64   // fblob: fp32 per_layer_model_proj [dim x (ple*layers)]
    ple_pnorm_off : int64   // fblob: fp32 per_layer_proj_norm [ple]
    ple_gate_off : int64    // qblob: per_layer_inp_gate, layers x (dim x ple)
    ple_proj_off : int64    // qblob: per_layer_proj,     layers x (ple x dim)
    ple_post_off : int64    // fblob: fp32 per_layer_post_norm, layers x dim
    // per-layer KV-ROW prefix (Σ layer_kv_dim of prior layers), filled by layout_offsets — makes
    // kv_cache_off O(1). seq_len-INDEPENDENT on purpose: tests cap seq_len after load, so the cache
    // offset multiplies by the live value at call time, never a baked one
    kv_row_prefix : array<int64>
    // big 2D weights — exactly one representation populated per `quant`
    wblob : array<float>
    qblob : array<int8>
    qscales : array<float>
    // wscale_f16 rail: the Q8 weight-scale plane as raw binary16 halfwords — llama.cpp byte
    // parity (34 vs 36 B per 32 weights). Built by wscale_convert_f16 post-repack (qscales is
    // then freed); exactly one of qscales/qscales16 is populated for a q8 model.
    qscales16 : array<uint16>
    wscale_f16 : bool = false
    q4blob : array<uint8>
    q4scales : array<float>
    // Native-MXFP4 expert stacks (valid when experts_mx4): the routed we1/we2/we3 stacks live here
    // as RAW disk bits (packed nibbles + E8M0 scale bytes) instead of qblob, halving resident size
    // and decode traffic vs the old MXFP4->Q8 transcode (and dropping its requant error).
    mxq : array<uint8>
    mxs : array<uint8>
    experts_mx4 : bool = false
    // gemma-4 E-series: per_layer_model_proj kept in its native BF16 halfwords — half the
    // per-token read; dot_bf16 widens by the exact bit shift, so the math matches the fp32
    // expand bit-for-bit. ple_model_off indexes bf16blob when set, fblob otherwise.
    bf16blob : array<uint16>
    ple_model_bf16 : bool = false
    // Native K-quant planes (valid when kquant_native): big weights whose disk type is
    // Q4_K/Q5_K/Q6_K live in per-format plane pairs instead of qblob, packed per 256-weight
    // superblock. The matching *_fmt tag says which plane a weight's element offset indexes.
    k4q : array<uint8>
    k4s : array<uint8>
    k5q : array<uint8>
    k5s : array<uint8>
    k6q : array<uint8>
    k6s : array<uint8>
    // the q40 tier's plane pair (128B nibbles — the k4 pairing — + 16B of f16 d per superblock)
    q40q : array<uint8>
    q40s : array<uint8>
    kquant_native : bool = false
    // kq stage 4: the kq planes were repacked at load into the active backend's grp<mr> layout.
    // From then on every kq matmul MUST run the backend slots — disk-order portable kernels would
    // read garbage — and embed_row gathers rows through the grp layout too.
    kq_repacked : bool = false
    kq_repack_mr4 : int64 = 4l   // per-FORMAT repack interleaves (kq v2: the families tune separately)
    kq_repack_mr5 : int64 = 4l
    kq_repack_mr6 : int64 = 4l
    kq_repack_mr40 : int64 = 4l
    // per-layer weight format tags (KqFmt; empty = all q8). Filled by detect_kq_formats before
    // layout_offsets so the layout walks each tensor into its format's cursor.
    wq_fmt : array<KqFmt>
    wk_fmt : array<KqFmt>
    wv_fmt : array<KqFmt>
    wo_fmt : array<KqFmt>
    w1_fmt : array<KqFmt>
    w2_fmt : array<KqFmt>
    w3_fmt : array<KqFmt>
    // MoE routed expert stacks — one tag per (layer, kind); the whole 3D stack is one tensor,
    // so every expert slice of a layer shares its tag (empty = all q8, like the arrays above)
    we1_fmt : array<KqFmt>
    we2_fmt : array<KqFmt>
    we3_fmt : array<KqFmt>
    wcls_fmt : KqFmt = KqFmt.q8
    emb_fmt : KqFmt = KqFmt.q8
    // gemma4 E-series PLE token table (per_layer_token_embd) — gather-only (whole superblock-
    // aligned rows dequant per token), never repacked, never matmul'd; kq tag = native planes
    ple_emb_fmt : KqFmt = KqFmt.q8
    // Attention projections use PER-LAYER offsets — tensor shapes differ per layer class on gemma4
    // (SWA 16Q/8KV heads of 256 vs global 16Q/1KV of 512); uniform arches fill the flat strides.
    // wv_offs[l] = -1 marks a layer with no attn_v tensor (V reuses the pre-norm K projection).
    wq_offs : array<int64>
    wk_offs : array<int64>
    wv_offs : array<int64>
    wo_offs : array<int64>
    // dense FFN weights — PER-LAYER offsets (gemma-4 E2B varies the FFN width by layer: 6144 for
    // layers 0-14, 12288 for 15-34, the elastic MatFormer sub-model). Uniform arches fill these with
    // a flat stride. ffn_w[l] = layer l's FFN width (empty = uniform config.hidden_dim); layer_hidden(t, l).
    w1_offs : array<int64>
    w2_offs : array<int64>
    w3_offs : array<int64>
    ffn_w : array<int64>
    // MoE (valid when config.n_expert > 0; the dense w1/w2/w3 regions are not allocated then).
    // Router matrices + shared-expert gate vectors are small fp32 -> fblob; the routed expert
    // stacks (expert-major within each layer) and the shared-expert 2D weights -> big-weight blob
    router_off : int64      // fblob: layers x n_expert x dim
    shexp_gate_off : int64  // fblob: layers x dim (one sigmoid-gate vector per layer)
    // gemma4 MoE (valid when config.moe_dense_shexp): the custom router's learned per-channel input
    // scale + the per-expert down-output scale, plus the routed branch's sandwich norms. The dense
    // parallel shared expert reuses the w1/w2/w3 offsets (allocated even though n_expert > 0). All fblob.
    moe_router_scale_off : int64  // ffn_gate_inp.scale:  layers x dim
    moe_down_scale_off : int64    // ffn_down_exps.scale: layers x n_expert
    rms_pre_ffn2_off : int64      // pre_ffw_norm_2  (routed pre-norm):  layers x dim
    rms_post_ffn1_off : int64     // post_ffw_norm_1 (dense post-norm):  layers x dim
    rms_post_ffn2_off : int64     // post_ffw_norm_2 (routed post-norm): layers x dim
    // gpt-oss MoE biases (fblob, valid per the config.moe_*_bias flags): router logits bias +
    // per-expert gate/down/up biases (expert-major within each layer, like the we* stacks)
    router_b_off : int64    // fblob: layers x n_expert
    // DeepSeek-V3/glm4moe expert-selection bias (fblob, valid when config.moe_exp_probs; dense-lead slots unused)
    exp_probs_off : int64   // exp_probs_b.bias: layers x n_expert
    web1_off : int64        // ffn_gate_exps.bias: layers x n_expert x n_ff_exp
    web2_off : int64        // ffn_down_exps.bias: layers x n_expert x dim
    web3_off : int64        // ffn_up_exps.bias:   layers x n_expert x n_ff_exp
    // gpt-oss per-head attention-sink logits (fblob, valid when config.attn_sinks)
    sinks_off : int64       // fblob: layers x n_heads
    // PER-LAYER offsets (expert e of layer l at offs[l] + e*ege): a Q4_K_M file mixes expert-stack
    // formats across layers (ffn_down_exps Q6_K on some), so each layer's stack takes its own
    // format's plane cursor. Kind-major regions preserved (all we1 layers, then we2, then we3).
    we1_offs : array<int64>   // ffn_gate_exps: per layer, n_expert x (dim x n_ff_exp)
    we2_offs : array<int64>   // ffn_down_exps: per layer, n_expert x (n_ff_exp x dim)
    we3_offs : array<int64>   // ffn_up_exps:   per layer, n_expert x (dim x n_ff_exp)
    wsh1_off : int64        // ffn_gate_shexp: layers x (dim x n_ff_shexp)
    wsh2_off : int64        // ffn_down_shexp: layers x (n_ff_shexp x dim)
    wsh3_off : int64        // ffn_up_shexp:   layers x (dim x n_ff_shexp)
    wcls_off : int64
    // Tied-classifier Q8: the classifier lives in qblob at wcls_off (repacked) and the fp32
    // embedding table is NOT in fblob — token rows dequant from the second, LINEAR transcode at
    // emb_q8_off. Set only by load_gguf for shared_weights + QuantMode.q8 + Q8_0 token_embd on disk.
    cls_q8 : bool = false
    emb_q8_off : int64
    quant : QuantMode
    rope_freqs : array<float>   // per-pair RoPE scaling (Llama-3.1 NTK-by-parts); empty = none
    // QKV projection biases (Qwen2) — fp32, per layer (q: layers×dim, k/v: layers×kv_dim);
    // empty unless config.attn_qkv_bias
    bq : array<float>
    bk : array<float>
    bv : array<float>
    // attention output-projection bias (gpt-oss) — fp32, layers×dim; empty unless config.attn_out_bias
    bo : array<float>
    // qwen35 deltanet per-layer weights: big-blob offsets on recurrent layers, -1 on attention layers
    dnqkv_offs : array<int64>   // attn_qkv (fused [q ; k ; v] rows): dim x dn_conv_dim
    dngate_offs : array<int64>  // attn_gate (the z gate): dim x ssm_d_inner
    dnbeta_offs : array<int64>  // ssm_beta: dim x ssm_dt_rank
    dnalpha_offs : array<int64> // ssm_alpha: dim x ssm_dt_rank
    dnout_offs : array<int64>   // ssm_out: ssm_d_inner x dim
    // ... and the fp32 smalls (fblob, flat per-layer strides; attention layers' slots stay zero)
    dn_conv_off : int64         // ssm_conv1d: layers x (d_conv x conv_dim), taps contiguous per channel
    dn_dt_off : int64           // ssm_dt.bias: layers x dt_rank
    dn_a_off : int64            // ssm_a (= -exp(A_log), precomputed on disk): layers x dt_rank
    dn_norm_off : int64         // ssm_norm (gated out-norm weight, shared across heads): layers x d_state
    // ssm_beta/ssm_alpha ride the DISK type like llama.cpp: quantized files matmul them q8
    // (dnbeta/dnalpha_offs above); F32 files (35B-A3B) keep them fp32 in fblob and matmul fp32
    dn_ba_f32 : bool = false
    dn_betaf_off : int64        // fblob: layers x (dim x dt_rank), valid when dn_ba_f32
    dn_alphaf_off : int64
    // qwen3next-layout file (detected via fused ssm_ba): v-indexed tensors ship in HF GROUPED
    // v-head order; the loader permutes them to the TILED order the kernels (and qwen35 files) use
    dn_grouped : bool = false
    // MTP/NextN extras (config.n_layer_nextn == 1). The draft block's standard attention/FFN
    // tensors ride the per-layer arrays above at index n_layers; only the nextn-specific ones
    // need their own slots. -1 = absent; mtp_headnorm_off falls back to rms_final_off.
    mtp_ehproj_off : int64 = -1     // nextn.eh_proj (quant plane): 2*dim -> dim
    mtp_ehproj_fmt : KqFmt = KqFmt.q8
    mtp_enorm_off : int64 = -1      // fblob: nextn.enorm (token-embedding RMS weight), dim
    mtp_hnorm_off : int64 = -1      // fblob: nextn.hnorm (trunk-hidden RMS weight), dim
    mtp_headnorm_off : int64 = -1   // fblob: nextn.shared_head_norm, dim (-1 = output_norm)
    // prepared-model image backing (dasllama_image): when set, every locked array field above
    // is a borrowed VIEW into this mapping (zero-copy) and the finalizer below unmaps it.
    // Both fields are skipped by the image save/load field walk.
    image_map : void?
    image_bytes : uint64
}

// Model's finalizer (every `delete model` lowers to _::finalize and lands here): image-backed
// models hold locked BORROWED views over the mapping — those must be forgotten (reset without
// freeing) before the mapping unmaps; everything owned deletes as usual.
def finalize(var t : Model) {
    apply(t) $ [unused_argument(name)] (name : string; var field) {
        static_if (typeinfo is_array(field)) {
            if (lock_count(field) != 0) {
                unsafe {
                    _builtin_forget_temp_array(field)
                }
            } else {
                delete field
            }
        } static_elif (typeinfo is_struct(field)) {
            delete field
        }
    }
    if (t.image_map != null) {
        unsafe {
            fmap_close(t.image_map, t.image_bytes)
        }
        t.image_map = null
        t.image_bytes = 0ul
    }
}

// ===== Arch registry: per-architecture blocks + chat template =====

//! A chat-template part: literal `text`, a `special`-token spelling (the chat layer resolves it to an
//! id), or the `content` slot the message text fills. Data only in this phase — the chat engine that
//! renders these lands in phase 5.
enum ChatPartKind {
    text
    special
    content
}

struct ChatPart {
    kind : ChatPartKind = ChatPartKind.text
    s : string = ""   // literal for `text`; special-token spelling for `special`; unused for `content`
}

//! One templated turn as an ordered part list.
struct ChatTurn {
    parts : array<ChatPart>
}

//! Per-arch chat template. `user` frames a user message; `assistant_open` is the generation prompt;
//! `stop` lists the special-token spellings that end a turn. `system` is empty for arches with no
//! system role (folded into the first user turn, e.g. Gemma).
struct ChatTemplate {
    add_bos : bool = false                    // prepend the tokenizer BOS at conversation start
    prelude : ChatTurn = ChatTurn()           // conversation-start parts after BOS (glm4moe: [gMASK]<sop>)
    system : ChatTurn = ChatTurn()
    user : ChatTurn = ChatTurn()
    assistant_open : ChatTurn = ChatTurn()
    assistant_close : ChatTurn = ChatTurn()   // empty = derive from the user turn's tail parts
    think_suppress : ChatTurn = ChatTurn()    // appended to the generation prompt when thinking is off
                                              // (Qwen3-style empty think block; empty = no suppress form)
    audio_pre : ChatTurn = ChatTurn()
    audio_post : ChatTurn = ChatTurn()
    stop : array<string>
    // tool-calling format (empty tool_call_open = the family declares none; see hermes_tools)
    tool_sys_prelude : string = ""    // opens the tool-definitions block in the system turn
    tool_sys_epilogue : string = ""   // closes the block + the call-format instructions
    tool_call_open : string = ""      // marker opening one call in assistant output
    tool_call_close : string = ""
    tool_resp_open : string = ""      // wraps one tool result in the reply turn
    tool_resp_close : string = ""
}

//! What a decoder honestly supports at the chat layer — declared by each arch registration,
//! queried via `caps(model)`. First honest entry: gemma has no system role, so the chat layer
//! folds the system prompt into the first user turn; `system_prompt = false` surfaces that.
struct LlmCaps {
    system_prompt : bool = true
}

// Sets the Config flags for one architecture. Runs before dim/vocab are known, so the loader
// finalizes dim-dependent fields (Gemma's embed_scale) after calling it.
typedef ConfigureFn = function<(var c : Config) : void>

//! Full architecture descriptor. Lives only in the registry — non-copyable, because it holds a
//! ChatTemplate. `blocks` (copyable) is copied onto the Model at load; `chat` is read by the chat layer.
struct ArchDesc {
    name : string = ""
    configure : ConfigureFn
    blocks : ArchBlocks = ArchBlocks()
    chat : ChatTemplate = ChatTemplate()
    lcaps : LlmCaps = LlmCaps()
}

struct private LayoutSizes {
    fblob_n : int64   // element count of the fp32 aux region
    wblob_n : int64   // element count of the big-weight region
    mx_n : int64      // element count of the native-MXFP4 expert planes (0 unless experts_mx4)
    bf16_n : int64    // halfword count of the native-BF16 region (0 unless ple_model_bf16)
    k4_n : int64      // element counts of the native K-quant planes (0 unless kquant_native)
    k5_n : int64
    k6_n : int64
    q40_n : int64
}

// The big-weight layout cursors: one element cursor per storage plane. kq_take advances the
// cursor a weight's format tag selects, so layout_offsets stays a single linear walk.
struct private KqCursors {
    wo : int64   // qblob / wblob / q4blob (the classic parallel-offset region)
    k4 : int64
    k5 : int64
    k6 : int64
    q40 : int64
}

def private kq_take(var cur : KqCursors; f : KqFmt; n : int64) : int64 {
    if (f == KqFmt.k4) {
        let o = cur.k4
        cur.k4 += n
        return o
    }
    if (f == KqFmt.k5) {
        let o = cur.k5
        cur.k5 += n
        return o
    }
    if (f == KqFmt.k6) {
        let o = cur.k6
        cur.k6 += n
        return o
    }
    if (f == KqFmt.q40) {
        let o = cur.q40
        cur.q40 += n
        return o
    }
    let o = cur.wo
    cur.wo += n
    return o
}

// Format tag for layer l of a per-kind tag array; empty array (non-kq load) = q8.
def private fmt_at(a : array<KqFmt>; l : int64) : KqFmt => empty(a) ? KqFmt.q8 : a[l]

//! Tied K-quant classifier: the embedding table lives in a native K-quant plane at wcls_off —
//! no fp32 table, no qblob copy (the cls_q8 analog for kquant_native loads).
def cls_kq(t : Model) : bool => t.kquant_native && t.config.shared_weights && t.emb_fmt != KqFmt.q8

// Fill the dense FFN offset tables (w1/w2/w3), each a contiguous per-layer region. Per-layer FFN
// width (gemma-4 E2B) reads t.ffn_w; empty = uniform `hidden`. dense_below >= 0 limits the dense
// slots to layers below it (glm4moe leading dense; routed layers get -1).
def private fill_dense_ffn_offsets(var t : Model; var cur : KqCursors; layers, dim, hidden : int64; dense_below : int64 = -1l) {
    t.w1_offs |> clear(); t.w1_offs |> reserve(layers)
    for (l in range64(layers)) { t.w1_offs |> push(dense_below >= 0l && l >= dense_below ? -1l : kq_take(cur, fmt_at(t.w1_fmt, l), dim * (empty(t.ffn_w) ? hidden : t.ffn_w[l]))) }
    t.w2_offs |> clear(); t.w2_offs |> reserve(layers)
    for (l in range64(layers)) { t.w2_offs |> push(dense_below >= 0l && l >= dense_below ? -1l : kq_take(cur, fmt_at(t.w2_fmt, l), (empty(t.ffn_w) ? hidden : t.ffn_w[l]) * dim)) }
    t.w3_offs |> clear(); t.w3_offs |> reserve(layers)
    for (l in range64(layers)) { t.w3_offs |> push(dense_below >= 0l && l >= dense_below ? -1l : kq_take(cur, fmt_at(t.w3_fmt, l), dim * (empty(t.ffn_w) ? hidden : t.ffn_w[l]))) }
}

// Fill both offset tables from the config; return the element counts of each region. Offsets are
// 32-aligned (every dim/kv/hidden is a multiple of 32), so Q8/Q4 scale/byte indices divide cleanly.
// Attention tensors get PER-LAYER offsets (shapes differ per layer class on gemma4).
def private layout_offsets(var t : Model) : LayoutSizes {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    // The MTP/NextN draft block is one extra trunk-shaped attention layer at index n_layers; every per-layer walk here covers it
    let layers = c.n_layers + c.n_layer_nextn
    var fo = 0l
    var bf16 = 0l
    if (t.cls_q8 || cls_kq(t)) {
        t.tok_emb_off = -1l   // no fp32 embedding table — rows dequant from emb_q8_off / the kq plane
    } else {
        t.tok_emb_off = fo
        fo += c.vocab_size * dim
    }
    t.rms_att_off = fo
    fo += layers * dim
    t.rms_ffn_off = fo
    fo += layers * dim
    if (c.pre_post_norm) {  // Gemma2: extra post-attention + post-FFN norms
        t.rms_post_att_off = fo
        fo += layers * dim
        t.rms_post_ffn_off = fo
        fo += layers * dim
    }
    if (c.qk_norm) {  // Qwen3/gemma4: per-head Q/K RMSNorm weights, one head_size(l) vector per layer
        t.rms_q_offs |> clear()
        t.rms_q_offs |> reserve(layers)
        for (l in range64(layers)) {
            t.rms_q_offs |> push(fo)
            fo += layer_head_size(c, l)
        }
        t.rms_k_offs |> clear()
        t.rms_k_offs |> reserve(layers)
        for (l in range64(layers)) {
            t.rms_k_offs |> push(fo)
            fo += layer_head_size(c, l)
        }
    }
    if (c.v_norm) {  // gemma4: a max-head-size vector of 1.0s so the weightless V-norm reuses rms_batch
        t.rms_ones_off = fo
        fo += max_head_size(c)
    }
    if (c.layer_out_scale) {  // gemma4: one output scalar per layer
        t.out_scale_off = fo
        fo += layers
    }
    if (c.attn_sinks) {  // gpt-oss: one sink logit per head per layer
        t.sinks_off = fo
        fo += layers * c.n_heads
    }
    if (c.recr_mask != 0ul) {  // qwen35 deltanet fp32 smalls (flat strides; attention layers' slots unused)
        t.dn_conv_off = fo
        fo += layers * c.ssm_d_conv * dn_conv_dim(c)
        t.dn_dt_off = fo
        fo += layers * c.ssm_dt_rank
        t.dn_a_off = fo
        fo += layers * c.ssm_dt_rank
        t.dn_norm_off = fo
        fo += layers * c.ssm_d_state
        if (t.dn_ba_f32) {  // F32-on-disk ssm_beta/ssm_alpha stay fp32 (llama.cpp matmuls them f32)
            t.dn_betaf_off = fo
            fo += layers * dim * c.ssm_dt_rank
            t.dn_alphaf_off = fo
            fo += layers * dim * c.ssm_dt_rank
        }
    }
    if (c.n_expert > 0l) {  // MoE: per-layer router matrices + shared-expert gate vectors, fp32
        t.router_off = fo
        fo += layers * c.n_expert * dim
        if (c.n_ff_shexp > 0l && c.moe_shexp_gated) {
            t.shexp_gate_off = fo
            fo += layers * dim
        }
        if (c.moe_router_bias) {  // gpt-oss: router logits bias
            t.router_b_off = fo
            fo += layers * c.n_expert
        }
        if (c.moe_exp_probs) {  // DeepSeek-V3/glm4moe: per-expert selection bias
            t.exp_probs_off = fo
            fo += layers * c.n_expert
        }
        if (c.moe_exps_bias) {  // gpt-oss: per-expert gate/down/up biases (expert-major per layer)
            t.web1_off = fo
            fo += layers * c.n_expert * c.n_ff_exp
            t.web2_off = fo
            fo += layers * c.n_expert * dim
            t.web3_off = fo
            fo += layers * c.n_expert * c.n_ff_exp
        }
        if (c.moe_dense_shexp) {  // gemma4: router input scale + per-expert down scale + routed sandwich norms
            t.moe_router_scale_off = fo
            fo += layers * dim
            t.moe_down_scale_off = fo
            fo += layers * c.n_expert
            t.rms_pre_ffn2_off = fo
            fo += layers * dim
            t.rms_post_ffn1_off = fo
            fo += layers * dim
            t.rms_post_ffn2_off = fo
            fo += layers * dim
        }
    }
    t.rms_final_off = fo
    fo += dim
    // gemma-4 E-series PLE norms (fblob): the single proj-norm vector [ple] + the per-layer post-norm.
    if (has_ple(c)) {
        t.ple_pnorm_off = fo
        fo += c.n_embd_per_layer
        t.ple_post_off = fo
        fo += layers * dim
        if (t.ple_model_bf16) {
            t.ple_model_off = 0l
            bf16 = dim * c.n_embd_per_layer * layers
        } else {
            t.ple_model_off = fo
            fo += dim * c.n_embd_per_layer * layers
        }
    }
    // Per-layer KV source + cache-row prefix. A shared layer (E-series) contributes NO width and aliases its source's slot
    t.kv_src |> clear()
    t.kv_src |> reserve(layers)
    for (l in range64(layers)) {
        t.kv_src |> push(layer_kv_source(c, l))
    }
    t.kv_row_prefix |> clear()
    t.kv_row_prefix |> reserve(layers)
    var kvrow = 0l
    for (l in range64(layers)) {
        if (t.kv_src[l] != l) {
            t.kv_row_prefix |> push(t.kv_row_prefix[t.kv_src[l]])
        } else {
            t.kv_row_prefix |> push(kvrow)
            kvrow += layer_kv_dim(c, l)
        }
    }
    var cur = KqCursors()
    t.wq_offs |> clear()
    t.wq_offs |> reserve(layers)
    for (l in range64(layers)) {
        if (layer_is_recurrent(c, l)) {
            t.wq_offs |> push(-1l)   // deltanet layer: no attention projections at all
        } else {
            // q_gated (qwen35): attn_q is 2x-wide — per head [q | gate]
            t.wq_offs |> push(kq_take(cur, fmt_at(t.wq_fmt, l), dim * layer_qd(c, l) * (c.q_gated ? 2l : 1l)))
        }
    }
    t.wk_offs |> clear()
    t.wk_offs |> reserve(layers)
    for (l in range64(layers)) {
        if (t.kv_src[l] != l || layer_is_recurrent(c, l)) {
            t.wk_offs |> push(-1l)   // shared-KV layer (E-series) or deltanet layer: no attn_k of its own
        } else {
            t.wk_offs |> push(kq_take(cur, fmt_at(t.wk_fmt, l), dim * layer_kv_dim(c, l)))
        }
    }
    t.wv_offs |> clear()
    t.wv_offs |> reserve(layers)
    for (l in range64(layers)) {
        // -1 marks V-from-K/shared-KV/deltanet; the l < n_layers guard kills the shift-by-64 wrap at the MTP index
        if (t.kv_src[l] != l || (l < c.n_layers && ((c.v_from_k_mask >> uint64(l)) & 1ul) != 0ul) || layer_is_recurrent(c, l)) {
            t.wv_offs |> push(-1l)
        } else {
            t.wv_offs |> push(kq_take(cur, fmt_at(t.wv_fmt, l), dim * layer_kv_dim(c, l)))
        }
    }
    t.wo_offs |> clear()
    t.wo_offs |> reserve(layers)
    for (l in range64(layers)) {
        if (layer_is_recurrent(c, l)) {
            t.wo_offs |> push(-1l)
        } else {
            t.wo_offs |> push(kq_take(cur, fmt_at(t.wo_fmt, l), layer_qd(c, l) * dim))
        }
    }
    if (c.recr_mask != 0ul) {  // deltanet big weights (recurrent layers only; kind-major regions, q8 planes)
        let cd = dn_conv_dim(c)
        t.dnqkv_offs |> clear(); t.dnqkv_offs |> reserve(layers)
        for (l in range64(layers)) { t.dnqkv_offs |> push(layer_is_recurrent(c, l) ? kq_take(cur, KqFmt.q8, dim * cd) : -1l) }
        t.dngate_offs |> clear(); t.dngate_offs |> reserve(layers)
        for (l in range64(layers)) { t.dngate_offs |> push(layer_is_recurrent(c, l) ? kq_take(cur, KqFmt.q8, dim * c.ssm_d_inner) : -1l) }
        t.dnbeta_offs |> clear(); t.dnbeta_offs |> reserve(layers)
        for (l in range64(layers)) { t.dnbeta_offs |> push(layer_is_recurrent(c, l) && !t.dn_ba_f32 ? kq_take(cur, KqFmt.q8, dim * c.ssm_dt_rank) : -1l) }
        t.dnalpha_offs |> clear(); t.dnalpha_offs |> reserve(layers)
        for (l in range64(layers)) { t.dnalpha_offs |> push(layer_is_recurrent(c, l) && !t.dn_ba_f32 ? kq_take(cur, KqFmt.q8, dim * c.ssm_dt_rank) : -1l) }
        t.dnout_offs |> clear(); t.dnout_offs |> reserve(layers)
        for (l in range64(layers)) { t.dnout_offs |> push(layer_is_recurrent(c, l) ? kq_take(cur, KqFmt.q8, c.ssm_d_inner * dim) : -1l) }
    }
    var mx = 0l
    if (c.n_expert > 0l) {
        // MoE: routed expert stacks — expert e of layer l is a normal 2D matrix at we*_off + (l*n_expert+e)*(dim*n_ff_exp)
        let ege = dim * c.n_ff_exp   // per-expert element count (same for gate/up and down)
        t.we1_offs |> clear(); t.we1_offs |> reserve(layers)
        t.we2_offs |> clear(); t.we2_offs |> reserve(layers)
        t.we3_offs |> clear(); t.we3_offs |> reserve(layers)
        if (t.experts_mx4) {
            for (_l in range64(layers)) { t.we1_offs |> push(mx); mx += c.n_expert * ege }
            for (_l in range64(layers)) { t.we2_offs |> push(mx); mx += c.n_expert * ege }
            for (_l in range64(layers)) { t.we3_offs |> push(mx); mx += c.n_expert * ege }
        } else {
            // dense-lead layers (glm4moe) carry no expert stacks: -1, no plane space
            for (l in range64(layers)) { t.we1_offs |> push(l < c.n_layer_dense_lead ? -1l : kq_take(cur, fmt_at(t.we1_fmt, l), c.n_expert * ege)) }
            for (l in range64(layers)) { t.we2_offs |> push(l < c.n_layer_dense_lead ? -1l : kq_take(cur, fmt_at(t.we2_fmt, l), c.n_expert * ege)) }
            for (l in range64(layers)) { t.we3_offs |> push(l < c.n_layer_dense_lead ? -1l : kq_take(cur, fmt_at(t.we3_fmt, l), c.n_expert * ege)) }
        }
        if (c.n_ff_shexp > 0l) {
            let she = dim * c.n_ff_shexp
            t.wsh1_off = cur.wo
            cur.wo += layers * she
            t.wsh2_off = cur.wo
            cur.wo += layers * she
            t.wsh3_off = cur.wo
            cur.wo += layers * she
        }
        if (c.moe_dense_shexp) {  // gemma4: the DENSE parallel shared expert (plain ffn gate/up/down)
            fill_dense_ffn_offsets(t, cur, layers, dim, hidden)
        } elif (c.n_layer_dense_lead > 0l) {  // glm4moe: plain dense FFN on the leading layer(s) only
            fill_dense_ffn_offsets(t, cur, layers, dim, hidden, c.n_layer_dense_lead)
        }
    } else {
        fill_dense_ffn_offsets(t, cur, layers, dim, hidden)
    }
    if (has_ple(c)) {  // gemma-4 E-series PLE big weights: the token table + per-layer Q8 gate/proj
        let ple = c.n_embd_per_layer
        // gather-only rows, NOT repacked — kq tag keeps native planes (disk order), else q8 linear
        t.ple_emb_off = kq_take(cur, t.ple_emb_fmt, c.vocab_size * ple * layers)
        t.ple_gate_off = cur.wo
        cur.wo += layers * dim * ple
        t.ple_proj_off = cur.wo
        cur.wo += layers * ple * dim
    }
    if (c.shared_weights) {
        if (t.cls_q8) {
            // tied Q8: the classifier transcode + the linear embedding copy the row reads dequant from
            t.wcls_off = cur.wo
            cur.wo += c.vocab_size * dim
            t.emb_q8_off = cur.wo
            cur.wo += c.vocab_size * dim
        } elif (cls_kq(t)) {
            // tied K-quant: ONE plane copy serves both the classifier matmul and the embedding row reads
            t.wcls_off = kq_take(cur, t.emb_fmt, c.vocab_size * dim)
        } else {
            t.wcls_off = 0l   // unused — the classifier reads the fp32 embedding from fblob
        }
    } else {
        t.wcls_off = kq_take(cur, t.wcls_fmt, c.vocab_size * dim)
    }
    if (c.n_layer_nextn > 0l) {   // MTP/NextN extras (the block's standard tensors rode the walks above)
        t.mtp_ehproj_off = kq_take(cur, t.mtp_ehproj_fmt, 2l * dim * dim)
        t.mtp_enorm_off = fo
        fo += dim
        t.mtp_hnorm_off = fo
        fo += dim
        t.mtp_headnorm_off = fo   // loaded only when the tensor exists; reads fall back to rms_final_off
        fo += dim
    }
    return LayoutSizes(fblob_n = fo, wblob_n = cur.wo, mx_n = mx, bf16_n = bf16,
                       k4_n = cur.k4, k5_n = cur.k5, k6_n = cur.k6, q40_n = cur.q40)
}

// Reserve exact capacity then size — avoids the array allocator's power-of-2 rounding on big arrays.
def private reserve_resize(var a : array<float>; n : int64) {
    a |> reserve(n)
    a |> resize(n)
}

def private copy_floats(src : array<float> | #; soff : int64; var dst : array<float>; doff, n : int64) {
    for (i in range64(n)) {
        dst[doff + i] = src[soff + i]
    }
}

//! Load a llama2.c checkpoint: 7-int32 header, then fp32 weights in fixed file order. Distributes
//! them into the split fp32 layout (embedding + norms -> fblob, the 2D matrices -> wblob).
def load_checkpoint(path : string) : Model {
    let HEADER_INTS = 7
    let HEADER_BYTES = 28
    var t = Model()
    t.quant = QuantMode.fp32
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open checkpoint '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        if (length(bytes) < HEADER_BYTES) {
            panic("dasLLAMA: checkpoint too small")
        }
        unsafe {
            let hdr = temp_array(addr < int? >(bytes[0]), HEADER_INTS, type<int>)
            t.config.dim = int64(hdr[0])
            t.config.hidden_dim = int64(hdr[1])
            t.config.n_layers = int64(hdr[2])
            t.config.n_heads = int64(hdr[3])
            t.config.n_kv_heads = int64(hdr[4])
            let vocab = hdr[5]
            t.config.seq_len = int64(hdr[6])
            // negative vocab is llama2.c's signal for unshared classifier weights
            t.config.shared_weights = vocab > 0
            t.config.vocab_size = int64(vocab > 0 ? vocab : -vocab)
        }
        t.config.head_size = t.config.dim / t.config.n_heads
        t.config.kv_dim = (t.config.dim * t.config.n_kv_heads) / t.config.n_heads
        t.config.rope_freq_base = 10000.0  // llama2.c checkpoints predate configurable θ
        // resolve the swa-θ sentinel like load_gguf does, so code (or a test) that forces
        // sliding_window on afterwards ropes those layers at the real θ, not 0
        t.config.rope_freq_base_swa = t.config.rope_freq_base
        let sz = layout_offsets(t)
        reserve_resize(t.fblob, sz.fblob_n)
        reserve_resize(t.wblob, sz.wblob_n)
        let c = t.config
        let dim = c.dim
        let hidden = c.hidden_dim
        let layers = c.n_layers
        let kv = c.kv_dim
        let qd = c.n_heads * c.head_size
        unsafe {
            let n_floats = (length(bytes) - HEADER_BYTES) / 4
            let src = temp_array(addr < float? >(bytes[HEADER_BYTES]), n_floats, type<float>)
            // file order: emb, rms_att, wq, wk, wv, wo, rms_ffn, w1, w2, w3, rms_final, freq_cis, [wcls]
            // (uniform llama geometry: each kind's region base is its layer-0 offset)
            var fc = 0l
            copy_floats(src, fc, t.fblob, t.tok_emb_off, c.vocab_size * dim)
            fc += c.vocab_size * dim
            copy_floats(src, fc, t.fblob, t.rms_att_off, layers * dim)
            fc += layers * dim
            copy_floats(src, fc, t.wblob, t.wq_offs[0], layers * dim * qd)
            fc += layers * dim * qd
            copy_floats(src, fc, t.wblob, t.wk_offs[0], layers * dim * kv)
            fc += layers * dim * kv
            copy_floats(src, fc, t.wblob, t.wv_offs[0], layers * dim * kv)
            fc += layers * dim * kv
            copy_floats(src, fc, t.wblob, t.wo_offs[0], layers * qd * dim)
            fc += layers * qd * dim
            copy_floats(src, fc, t.fblob, t.rms_ffn_off, layers * dim)
            fc += layers * dim
            copy_floats(src, fc, t.wblob, t.w1_offs[0], layers * dim * hidden)
            fc += layers * dim * hidden
            copy_floats(src, fc, t.wblob, t.w2_offs[0], layers * hidden * dim)
            fc += layers * hidden * dim
            copy_floats(src, fc, t.wblob, t.w3_offs[0], layers * dim * hidden)
            fc += layers * dim * hidden
            copy_floats(src, fc, t.fblob, t.rms_final_off, dim)
            fc += dim
            fc += c.seq_len * c.head_size   // skip old freq_cis (real + imag halves)
            if (!c.shared_weights) {
                copy_floats(src, fc, t.wblob, t.wcls_off, c.vocab_size * dim)
            }
        }
    }
    fclose(f)
    resolve_arch(t, "llama")   // llama2.c checkpoints are always the llama arch — bind its blocks
    return <- t
}

// Read one big 2D weight tensor into the active representation: transcode straight from disk when
// the on-disk type already matches the target precision, otherwise dequant to a reused fp32 scratch
// and quantize into the shared blob. `fmt` != q8 routes into that native K-quant plane pair.
def private load_big(m : GGUFMeta; bytes : array<uint8> | #; name : string; var t : Model; woff, n : int64; var scratch : array<float>; src_off : int64 = 0l; fmt : KqFmt = KqFmt.q8) {
    if (fmt == KqFmt.k4) {
        gguf_transcode_q4k(m, bytes, name, t.k4q, t.k4s, woff, n, src_off)
    } elif (fmt == KqFmt.k5) {
        gguf_transcode_q5k(m, bytes, name, t.k5q, t.k5s, woff, n, src_off)
    } elif (fmt == KqFmt.k6) {
        gguf_transcode_q6k(m, bytes, name, t.k6q, t.k6s, woff, n, src_off)
    } elif (fmt == KqFmt.q40) {
        gguf_transcode_q40(m, bytes, name, t.q40q, t.q40s, woff, n, src_off)
    } elif (t.quant == QuantMode.q8) {
        let gt = gguf_tensor_type(m, name)
        if (gt == GGML_TYPE_Q8_0) {
            gguf_transcode_q8_0(m, bytes, name, t.qblob, t.qscales, woff, woff / 32l, n, src_off)
        } elif (gt == GGML_TYPE_Q5_0) {
            gguf_transcode_q5_0_to_q8(m, bytes, name, t.qblob, t.qscales, woff, woff / 32l, n, src_off)
        } else {
            scratch |> resize(n)
            gguf_read_tensor_f32(m, bytes, name, scratch, 0l, n, src_off)
            quantize_q8_0_into(scratch, n, t.qblob, t.qscales, woff, woff / 32l)
        }
    } elif (t.quant == QuantMode.q4_0) {
        if (gguf_tensor_type(m, name) == GGML_TYPE_Q4_0) {
            gguf_transcode_q4_0(m, bytes, name, t.q4blob, t.q4scales, woff / 2l, woff / 32l, n, src_off)
        } else {
            scratch |> resize(n)
            gguf_read_tensor_f32(m, bytes, name, scratch, 0l, n, src_off)
            quantize_q4_0_into(scratch, n, t.q4blob, t.q4scales, woff / 2l, woff / 32l)
        }
    } else {
        gguf_read_tensor_f32(m, bytes, name, t.wblob, woff, n, src_off)
    }
}

// qwen3next-layout deltanet big weights: same planes as the qwen35 arm, but the file's v-indexed
// rows arrive GROUPED by k-head — sub-slice reads land each v-head chunk at its TILED position
// (tiled j*nk+g <- grouped g*nvpk+j); ssm_out (an INPUT-column permute) round-trips through f32.
def private load_dn_grouped(m : GGUFMeta; bytes : array<uint8> | #; var t : Model; l, dim : int64; var scratch : array<float>) {
    let nk = t.config.ssm_n_group
    let nvh = t.config.ssm_dt_rank
    let nvpk = nvh / nk
    let hv = t.config.ssm_d_inner / nvh
    let qkr = 2l * nk * t.config.ssm_d_state    // fused qkv: the q+k rows stay put, v rows permute
    load_big(m, bytes, "blk.{l}.attn_qkv.weight", t, t.dnqkv_offs[l], dim * qkr, scratch)
    for (g in range64(nk)) {
        for (j in range64(nvpk)) {
            let vg = g * nvpk + j
            let vt = j * nk + g
            load_big(m, bytes, "blk.{l}.attn_qkv.weight", t, t.dnqkv_offs[l] + (qkr + vt * hv) * dim, hv * dim, scratch, (qkr + vg * hv) * dim)
            load_big(m, bytes, "blk.{l}.attn_gate.weight", t, t.dngate_offs[l] + vt * hv * dim, hv * dim, scratch, vg * hv * dim)
            if (!t.dn_ba_f32) {   // fused ba rows per k-head group: [beta j.. ; alpha j..]
                load_big(m, bytes, "blk.{l}.ssm_ba.weight", t, t.dnbeta_offs[l] + vt * dim, dim, scratch, (g * 2l * nvpk + j) * dim)
                load_big(m, bytes, "blk.{l}.ssm_ba.weight", t, t.dnalpha_offs[l] + vt * dim, dim, scratch, ((g * 2l + 1l) * nvpk + j) * dim)
            }
        }
    }
    let n = t.config.ssm_d_inner * dim
    var raw : array<float>
    raw |> resize(n)
    gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_out.weight", raw, 0l, n)
    scratch |> resize(n)
    let din = t.config.ssm_d_inner
    unsafe {   // column-permute threaded over output rows (disjoint writes per row)
        let rawp = addr < float const? >(raw[0])
        var scrp = addr(scratch[0])
        maybe_parallel_for(0, int(dim), is_job_que_available() ? 16 : 1) $(rb, re) {
            unsafe {
                for (r in range64(int64(rb), int64(re))) {
                    for (g in range64(nk)) {
                        for (j in range64(nvpk)) {
                            let src = r * din + (g * nvpk + j) * hv
                            let dst = r * din + (j * nk + g) * hv
                            for (x in range64(hv)) {
                                scrp[dst + x] = rawp[src + x]
                            }
                        }
                    }
                }
            }
        }
    }
    delete raw
    let woff = t.dnout_offs[l]
    if (t.quant == QuantMode.q8) {
        quantize_q8_0_into(scratch, n, t.qblob, t.qscales, woff, woff / 32l)
    } elif (t.quant == QuantMode.q4_0) {
        quantize_q4_0_into(scratch, n, t.q4blob, t.q4scales, woff / 2l, woff / 32l)
    } else {
        for (i in range64(n)) {
            t.wblob[woff + i] = scratch[i]
        }
    }
}

// ===== Arch registry =====
// Each architecture self-registers its config setter, block kernels, and chat template at [init].
// load_gguf looks the arch up by its GGUF `general.architecture` string; an unknown arch panics.

var private g_arch_registry : table<string; ArchDesc>

//! Register an architecture descriptor under `name` (its GGUF architecture string). Called from [init].
def register_arch(name : string; var desc : ArchDesc) {
    g_arch_registry[name] <- desc
}

//! Is `name` a registered architecture?
def has_arch(name : string) : bool {
    return key_exists(g_arch_registry, name)
}

//! The registered architecture names (for diagnostics / tests).
def arch_names() : array<string> {
    return <- [for (k in keys(g_arch_registry)); k]
}

//! A clone of `name`'s chat template (the chat layer / tests consume this).
def arch_chat(name : string) : ChatTemplate {
    var out = ChatTemplate()
    out := g_arch_registry[name].chat
    return <- out
}

//! `name`'s declared LLM capabilities (see LlmCaps).
def arch_lcaps(name : string) : LlmCaps {
    return key_exists(g_arch_registry, name) ? g_arch_registry[name].lcaps : LlmCaps()
}

//! The loaded model's declared LLM capabilities (its arch registration's LlmCaps).
def caps_(m : Model) : LlmCaps {
    return arch_lcaps(m.arch)
}

// Look the arch up, apply its config flags, and copy its (copyable) block kernels onto the model.
// The full descriptor stays in the registry; only the hot-path pointers ride on the Model.
def private resolve_arch(var t : Model; arch : string) {
    rebind_arch_blocks(t, arch)
    g_arch_registry[arch].configure(t.config)
}

// The image loader's post-read half of resolve_arch: block kernels re-bind from the registry,
// but configure must NOT re-run — the serialized config already carries configure's flags PLUS
// the gguf's overrides (re-running clobbered the E2B's real sliding_window with the default).
def rebind_arch_blocks(var t : Model; arch : string) {
    if (!key_exists(g_arch_registry, arch)) {
        let registered = join(arch_names(), ", ")
        panic("dasLLAMA: unsupported architecture '{arch}' (registered: {registered})")
    }
    t.arch = arch
    t.blocks = g_arch_registry[arch].blocks
}

// The wscale_f16 rail's load-time switch: default ON — free at 16 lanes on the 3990X, +5.4% on
// the byte-bound M1. Every backend carries the s16 twins (loader falls back to f32 with a warning
// otherwise). Box-profile runtime knob / benches flip it before load_gguf.
var g_wscale_f16 = true
def public set_wscale_f16(on : bool) {
    g_wscale_f16 = on
}
def public get_wscale_f16() : bool {
    return g_wscale_f16
}

// Native K-quant weight planes: Q4_K/Q5_K/Q6_K tensors stay packed in per-format planes instead of
// requantizing to Q8. Default ON since the kernel tier landed; OFF loads through the q8-requant
// fallback exactly as before — the A/B opt-out.
var private g_kquant_native = true

//! Enable/disable native K-quant weight planes for subsequent loads (A/B rail; default ON).
def public set_kquant_native(on : bool) {
    g_kquant_native = on
}
def public get_kquant_native() : bool {
    return g_kquant_native
}

// The q40 tier's bring-up knob (like kquant_native had): OFF keeps Q4_0 tensors on the
// q8-requant fallback (2x memory, exact) — the zero-risk A/B rail while the tier proves out.
var private g_kq_q40_native = true

//! Enable/disable the native Q4_0 plane tier for subsequent loads (A/B rail; default ON —
//! flipped after the QAT parity gate; requires kquant_native).
def public set_kq_q40_native(on : bool) {
    g_kq_q40_native = on
}
def public get_kq_q40_native() : bool {
    return g_kq_q40_native
}

// GGML disk type -> plane format tag (non-K-quant types ride the classic q8 path)
def private kq_fmt_of(gt : int) : KqFmt {
    if (gt == GGML_TYPE_Q4_K) {
        return KqFmt.k4
    }
    if (gt == GGML_TYPE_Q5_K) {
        return KqFmt.k5
    }
    if (gt == GGML_TYPE_Q6_K) {
        return KqFmt.k6
    }
    if (gt == GGML_TYPE_Q4_0 && g_kq_q40_native) {
        return KqFmt.q40
    }
    return KqFmt.q8
}

// q40 demotion: Q4_0 guarantees rows % 32 only (K-quants are % 256 by construction), and the
// kq tier's superblock walkers/activation image need % 256 — a misaligned tensor falls back to
// the q8 requant path per tensor. `n` = the tensor's ROW length (the matmul's input dim).
def private kq_fmt_row_ok(f : KqFmt; n : int64) : KqFmt => f == KqFmt.q40 && n % 256l != 0l ? KqFmt.q8 : f

// Fill the per-kind weight format tags from the file's tensor types (q8-mode loads with the
// kquant_native knob on). Fused-projection arches (Phi3) split attn_qkv/ffn_up at row-aligned
// slice offsets, so every slice stays superblock-aligned; one disk tensor = one tag for its slices.
def private detect_kq_formats(var t : Model; m : GGUFMeta) {
    if (t.quant != QuantMode.q8 || !g_kquant_native
            || (t.config.fused_qkv && t.config.dim % 256l != 0l)) {   // fused slice offsets are dim-multiples
        return
    }
    let layers = t.config.n_layers + t.config.n_layer_nextn   // + the MTP block's tags at index n_layers
    t.wq_fmt |> resize(layers)
    t.wk_fmt |> resize(layers)
    t.wv_fmt |> resize(layers)
    t.wo_fmt |> resize(layers)
    t.w1_fmt |> resize(layers)
    t.w2_fmt |> resize(layers)
    t.w3_fmt |> resize(layers)
    var any = false
    // q40 tags additionally demote per tensor when the row length isn't % 256 — Q4_0 only guarantees % 32
    for (l in range64(layers)) {
        if (t.config.fused_qkv) {
            let fqkv = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.attn_qkv.weight")), t.config.dim)
            t.wq_fmt[l] = fqkv
            t.wk_fmt[l] = fqkv
            t.wv_fmt[l] = fqkv
            let fgu = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.ffn_up.weight")), t.config.dim)
            t.w1_fmt[l] = fgu
            t.w3_fmt[l] = fgu
        } else {
            t.wq_fmt[l] = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.attn_q.weight")), t.config.dim)
            t.wk_fmt[l] = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.attn_k.weight")), t.config.dim)
            t.wv_fmt[l] = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.attn_v.weight")), t.config.dim)
            t.w1_fmt[l] = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.ffn_gate.weight")), t.config.dim)
            t.w3_fmt[l] = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.ffn_up.weight")), t.config.dim)
        }
        t.wo_fmt[l] = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.attn_output.weight")), layer_qd(t.config, l))
        t.w2_fmt[l] = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.ffn_down.weight")), layer_hidden(t, l))
        any ||= t.wq_fmt[l] != KqFmt.q8 || t.wk_fmt[l] != KqFmt.q8 || t.wv_fmt[l] != KqFmt.q8
        any ||= t.wo_fmt[l] != KqFmt.q8 || t.w1_fmt[l] != KqFmt.q8 || t.w2_fmt[l] != KqFmt.q8
        any ||= t.w3_fmt[l] != KqFmt.q8
    }
    // MoE routed expert stacks: one tag per (layer, kind). Biased-experts arches (gpt-oss) stay q8 — the kq groupn carries no bias form
    if (t.config.n_expert > 0l && !t.config.moe_exps_bias
            && (t.config.dim * t.config.n_ff_exp) % 256l == 0l) {
        t.we1_fmt |> resize(layers)
        t.we2_fmt |> resize(layers)
        t.we3_fmt |> resize(layers)
        let fused_gu = t.config.moe_dense_shexp   // gemma4: experts ship fused as ffn_gate_up_exps
        for (l in range64(layers)) {
            if (fused_gu) {
                let f = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.ffn_gate_up_exps.weight")), t.config.dim)
                t.we1_fmt[l] = f
                t.we3_fmt[l] = f
            } else {
                t.we1_fmt[l] = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.ffn_gate_exps.weight")), t.config.dim)
                t.we3_fmt[l] = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.ffn_up_exps.weight")), t.config.dim)
            }
            t.we2_fmt[l] = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.ffn_down_exps.weight")), t.config.n_ff_exp)
            any ||= t.we1_fmt[l] != KqFmt.q8 || t.we2_fmt[l] != KqFmt.q8 || t.we3_fmt[l] != KqFmt.q8
        }
    }
    if (t.config.shared_weights) {
        t.emb_fmt = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "token_embd.weight")), t.config.dim)
        any ||= t.emb_fmt != KqFmt.q8
    } else {
        t.wcls_fmt = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "output.weight")), t.config.dim)
        any ||= t.wcls_fmt != KqFmt.q8
    }
    if (t.config.n_layer_nextn > 0l) {
        t.mtp_ehproj_fmt = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{t.config.n_layers}.nextn.eh_proj.weight")), 2l * t.config.dim)
        any ||= t.mtp_ehproj_fmt != KqFmt.q8
    }
    // E-series PLE token table: gather-only whole rows; row length must be superblock-aligned
    if (has_ple(t.config) && (t.config.n_layers * t.config.n_embd_per_layer) % 256l == 0l) {
        t.ple_emb_fmt = kq_fmt_of(gguf_tensor_type(m, "per_layer_token_embd.weight"))
        any ||= t.ple_emb_fmt != KqFmt.q8
    }
    if (!any) {   // nothing K-quant in the file: drop the tags so fmt_at short-circuits on empty
        delete t.wq_fmt
        delete t.wk_fmt
        delete t.wv_fmt
        delete t.wo_fmt
        delete t.w1_fmt
        delete t.w2_fmt
        delete t.w3_fmt
        delete t.we1_fmt
        delete t.we2_fmt
        delete t.we3_fmt
    }
    t.kquant_native = any
}

// Any K-quant-tagged expert stack in layer `l`? (The MoE dispatch and prefill gates branch on this.)
def private moe_layer_kq(t : Model; l : int64) : bool {
    return fmt_at(t.we1_fmt, l) != KqFmt.q8 || fmt_at(t.we2_fmt, l) != KqFmt.q8 || fmt_at(t.we3_fmt, l) != KqFmt.q8
}

// Convert the (already repacked/permuted) f32 scale plane to raw binary16 halfwords and free the
// f32 plane. f16->f32->f16 round-trips exactly for gguf-Q8 tensors (bit-identical dots);
// runtime-quantized tensors take one f16 rounding. Gated on the backend carrying f16-scale kernels.
def private wscale_convert_f16(var t : Model) {
    if (!kernel_backend_has_wscale16()) {
        to_log(LOG_WARNING, "dasLLAMA: wscale_f16 requested but backend '{active_kernel_backend()}' has no f16-scale kernels — keeping the f32 scale plane\n")
        return
    }
    let n = long_length(t.qscales)
    if (n == 0l) {   // every big weight lives in a native plane (kq/mx4) — nothing to convert
        return
    }
    t.qscales16 |> resize(n)   // int64 resize — an 80B-weight Q8 model has >2^31 scales
    unsafe {
        var dp = addr(t.qscales16[0])
        let sp = addr < float const? >(t.qscales[0])
        // 32-aligned chunks keep any vectorized cvt kernel on whole groups
        let nch = int(clamp(n / 1048576l, 1l, 64l))
        maybe_parallel_for(0, int(n / 32l), is_job_que_available() ? nch : 1) $(rb, re) {
            unsafe {
                cvt_f32_to_f16(dp + int64(rb) * 32l, sp + int64(rb) * 32l, int64(re - rb) * 32l)
            }
        }
        if (n % 32l != 0l) {
            let t0 = (n / 32l) * 32l
            cvt_f32_to_f16(dp + t0, sp + t0, n - t0)
        }
    }
    delete t.qscales
    t.wscale_f16 = true
    to_log(LOG_INFO, "dasLLAMA: weight-scale plane converted to f16 ({n} scales, -{n * 2l} bytes)\n")
}

// ===== GPU MoE tier upload (post-prepare, loader-agnostic) =====

// Gather one PREPARED Q8 weight stack into the GPU tier's two row-major planes — int8 quants +
// f16 halfword scales. slice_rows is the repack unit (each expert slice interleaves independently).
// Reads whatever the active backend prepared (grp<mr> interleave or plain row-major).
def moe_gpu_gather_stack(t : Model; woff : int64; n, rows, slice_rows : int64; mr, kgroup, wbias : int64;
                         var wq : array<uint8>; var ws : array<uint8>) {
    let nb = n / 32l
    assert(rows * n <= 2147483647l, "GPU MoE stack exceeds the int32 gather-buffer rail")
    wq |> resize(int(rows * n))
    ws |> resize(int(rows * nb * 2l))
    assert(wbias == 0l || wbias == 128l, "GPU MoE gather: unknown q8 plane bias")
    let nslices = int(rows / slice_rows)
    unsafe {
        let wp = addr<int8 const?>(t.qblob[woff])
        var s16p : uint16 const? = null
        var s32p : float const? = null
        if (t.wscale_f16) {
            s16p = addr<uint16 const?>(t.qscales16[woff / 32l])
        } else {
            s32p = addr<float const?>(t.qscales[woff / 32l])
        }
        var wqp = addr(wq[0])
        var wsp = addr<uint16?>(ws[0])
        let njobs = is_job_que_available() ? min(nslices, 4 * (get_total_hw_jobs() + 1)) : 1
        maybe_parallel_for(0, nslices, njobs) $(sb, se) {
            unsafe {
                let ng = slice_rows / mr
                for (s in range(sb, se)) {
                    let swoff = int64(s) * slice_rows * n
                    let ssoff = int64(s) * slice_rows * nb
                    for (rr in range64(slice_rows)) {
                        let grouped = rr < ng * mr
                        let g = rr / mr
                        let r = rr % mr
                        for (bi in range64(nb)) {
                            // scale: group-interleaved [g][bi][r] or the row-major tail; the f32
                            // fallback takes the same clamped rounding wscale_convert_f16 applies
                            let si = grouped ? ssoff + (g * nb + bi) * mr + r : ssoff + rr * nb + bi
                            wsp[ssoff + rr * nb + bi] = (s16p != null ? s16p[si]
                                : uint16(f32_to_f16(clamp(s32p[si], -65504.0, 65504.0))))
                            var dst = wqp + swoff + rr * n + bi * 32l
                            if (grouped) {
                                let gbase = swoff + g * mr * n + r * kgroup
                                var kg = (bi * 32l) / kgroup
                                var o = 0l
                                for (_kk in range64(32l / kgroup)) {
                                    let src = gbase + kg * kgroup * mr
                                    for (j in range64(kgroup)) {
                                        dst[o + j] = uint8(int(wp[src + j]) ^ int(wbias))
                                    }
                                    kg++
                                    o += kgroup
                                }
                            } else {
                                let src = swoff + rr * n + bi * 32l
                                for (j in range64(32l)) {
                                    dst[j] = uint8(wp[src + j])
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Disk-order K-quant plane accessors for the kq gather's un-repacked/tail rows (source of
// truth: the private twins beside repack_k4/k5/k6_grp in dasllama_math_gen.das — replicated
// because math_gen mounts conditionally behind the dasLLVM guard).

// low 4 bits of weight k off a disk-order k4/k5 nibble row (128 bytes per superblock)
def private kq_disk_nib45(rowq : uint8 const?; k : int64) : uint {
    let js = k / 64l
    let rem = k % 64l
    let b = uint(unsafe(rowq[js * 32l + rem % 32l]))
    return rem < 32l ? b & 15u : b >> 4u
}

// the 5th bit of weight k off a disk-order k5 qh row (32 bytes per superblock)
def private kq_disk_hbit5(rowh : uint8 const?; k : int64) : uint {
    let js = k / 64l
    let rem = k % 64l
    return (uint(unsafe(rowh[rem % 32l])) >> uint(2l * js + (rem < 32l ? 0l : 1l))) & 1u
}

// low 4 bits of weight k off a disk-order k6 ql row (128 bytes per superblock)
def private kq_disk_nib6(rowq : uint8 const?; k : int64) : uint {
    let h = k / 128l
    let g = (k % 128l) / 32l
    let b = uint(unsafe(rowq[h * 64l + (g % 2l) * 32l + k % 32l]))
    return g < 2l ? b & 15u : b >> 4u
}

// Gather one PREPARED K-quant weight stack into the GPU tier's device planes — quant payloads
// row-major per superblock in the k/k+16 nibble pairing, plus DECODED 20B scale rows. Reads
// whatever the load prepared (grp<mr> interleave or disk-order); device bytes are identical either way.
def moe_gpu_gather_stack_kq(t : Model; fmt : KqFmt; woff : int64; n, rows, slice_rows : int64;
                            repacked : bool; mr : int64; var wq : array<uint8>; var ws : array<uint8>) {
    let nsb = n / 256l
    let qsb = fmt == KqFmt.k4 ? 128l : (fmt == KqFmt.k5 ? 160l : 192l)
    let dssb = fmt == KqFmt.k6 ? 18l : 20l   // disk/plane scale stride; device rows are 20B for all
    assert(n % 256l == 0l, "GPU MoE kq gather: row length must be a superblock multiple")
    assert(rows * nsb * qsb <= 2147483647l, "GPU MoE kq stack exceeds the int32 gather-buffer rail")
    wq |> resize(int(rows * nsb * qsb))
    ws |> resize(int(rows * nsb * 20l))
    let nslices = int(rows / slice_rows)
    let qrow = nsb * qsb
    let srow = nsb * dssb
    let ng = repacked ? slice_rows / mr : 0l
    unsafe {
        let sb0 = woff / 256l
        let qp = (fmt == KqFmt.k4 ? addr<uint8 const?>(t.k4q[sb0 * 128l])
            : (fmt == KqFmt.k5 ? addr<uint8 const?>(t.k5q[sb0 * 160l]) : addr<uint8 const?>(t.k6q[sb0 * 192l])))
        let sp = (fmt == KqFmt.k4 ? addr<uint8 const?>(t.k4s[sb0 * 20l])
            : (fmt == KqFmt.k5 ? addr<uint8 const?>(t.k5s[sb0 * 20l]) : addr<uint8 const?>(t.k6s[sb0 * 18l])))
        var wqp = addr(wq[0])
        var wsp = addr(ws[0])
        let njobs = is_job_que_available() ? min(nslices, 4 * (get_total_hw_jobs() + 1)) : 1
        maybe_parallel_for(0, nslices, njobs) $(ssb, sse) {
            unsafe {
                for (s in range(ssb, sse)) {
                    let sliceQ = int64(s) * slice_rows * qrow
                    let sliceS = int64(s) * slice_rows * srow
                    for (ri in range64(slice_rows)) {
                        let rr = int64(s) * slice_rows + ri
                        let grouped = repacked && ri < ng * mr
                        let g = ri / mr
                        let r = ri % mr
                        for (sbi in range64(nsb)) {
                            var dq = wqp + rr * qrow + sbi * qsb
                            var dsc = wsp + (rr * nsb + sbi) * 20l
                            if (grouped) {
                                let gq = qp + sliceQ + g * mr * qrow + sbi * qsb * mr
                                let gs = sp + sliceS + g * mr * srow + sbi * dssb * mr
                                for (m in range64(128l)) {   // nibble region: verbatim per (blk, j, t)
                                    dq[m] = gq[((m / 4l) * mr + r) * 4l + m % 4l]
                                }
                                if (fmt == KqFmt.k5) {
                                    for (bj in range64(32l)) {
                                        dq[128l + bj] = gq[128l * mr + bj * mr + r]
                                    }
                                } elif (fmt == KqFmt.k6) {
                                    for (p in range64(64l)) {
                                        dq[128l + p] = gq[128l * mr + ((p / 4l) * mr + r) * 4l + p % 4l]
                                    }
                                }
                                if (fmt == KqFmt.k6) {
                                    for (idx in range64(16l)) {
                                        dsc[idx] = gs[idx * mr + r]
                                    }
                                    dsc[16] = gs[16l * mr + 2l * r]
                                    dsc[17] = gs[16l * mr + 2l * r + 1l]
                                    dsc[18] = uint8(0)
                                    dsc[19] = uint8(0)
                                } else {
                                    dsc[0] = gs[2l * r]
                                    dsc[1] = gs[2l * r + 1l]
                                    dsc[2] = gs[2l * mr + 2l * r]
                                    dsc[3] = gs[2l * mr + 2l * r + 1l]
                                    for (blk in range64(8l)) {
                                        dsc[4l + blk] = gs[4l * mr + blk * mr + r]
                                        dsc[12l + blk] = gs[12l * mr + blk * mr + r]
                                    }
                                }
                            } else {
                                let rq = qp + sliceQ + ri * qrow + sbi * qsb
                                let rs = sp + sliceS + ri * srow + sbi * dssb
                                for (m in range64(128l)) {   // re-pair disk k/k+32 nibbles to k/k+16
                                    let k = (m / 16l) * 32l + m % 16l
                                    let lo = fmt == KqFmt.k6 ? kq_disk_nib6(rq, k) : kq_disk_nib45(rq, k)
                                    let hi = fmt == KqFmt.k6 ? kq_disk_nib6(rq, k + 16l) : kq_disk_nib45(rq, k + 16l)
                                    dq[m] = uint8(lo | (hi << 4u))
                                }
                                if (fmt == KqFmt.k5) {
                                    for (bj in range64(32l)) {
                                        let k = (bj / 4l) * 32l + (bj % 4l) * 4l
                                        var hb = 0u
                                        for (tt in range64(4l)) {
                                            hb |= kq_disk_hbit5(rq + 128l, k + tt) << uint(tt)
                                            hb |= kq_disk_hbit5(rq + 128l, k + 16l + tt) << uint(4l + tt)
                                        }
                                        dq[128l + bj] = uint8(hb)
                                    }
                                } elif (fmt == KqFmt.k6) {
                                    for (p in range64(64l)) {
                                        dq[128l + p] = rq[128l + p]
                                    }
                                }
                                if (fmt == KqFmt.k6) {
                                    for (idx in range64(18l)) {
                                        dsc[idx] = rs[idx]
                                    }
                                    dsc[18] = uint8(0)
                                    dsc[19] = uint8(0)
                                } else {
                                    for (idx in range64(4l)) {   // f16 d + f16 dmin verbatim
                                        dsc[idx] = rs[idx]
                                    }
                                    for (blk in range64(8l)) {   // decode the 6-bit sc/mn packing
                                        let sm = k4_sc_mn(rs + 4l, blk)
                                        dsc[4l + blk] = uint8(sm.x)
                                        dsc[12l + blk] = uint8(sm.y)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

def private moe_gpu_fmt_kq(f : KqFmt) : bool => f == KqFmt.k4 || f == KqFmt.k5 || f == KqFmt.k6

// Gather one stack through its format's reader (q8 planes or native kq planes) and hand it to
// the tier's upload hook with the format tag.
def private moe_gpu_gather_upload(t : Model; f : KqFmt; woff : int64; n, rows, slice_rows : int64;
                                  mr, kgroup, wbias : int64; var wq : array<uint8>; var ws : array<uint8>;
                                  stream : bool = false) : bool {
    if (f == KqFmt.q8) {
        moe_gpu_gather_stack(t, woff, n, rows, slice_rows, mr, kgroup, wbias, wq, ws)
    } else {
        let kmr = t.kq_repacked ? (f == KqFmt.k4 ? t.kq_repack_mr4 : (f == KqFmt.k5 ? t.kq_repack_mr5 : t.kq_repack_mr6)) : 1l
        moe_gpu_gather_stack_kq(t, f, woff, n, rows, slice_rows, t.kq_repacked, kmr, wq, ws)
    }
    return moe_gpu_upload_stack(wq, ws, woff, n, rows, int(f), stream)
}

// Upload the offloaded layers' expert stacks to the GPU tier from the PREPARED planes. Called at
// the end of every load rail so DASLLAMA_GPU_MOE_LAYERS behaves identically however the model
// arrived. Layers go resident from the end while the tier's VRAM budget holds.
def moe_gpu_upload_resident(var t : Model) {
    set_moe_gpu_from_layer(t.config.n_layers)   // nothing resident until an upload lands
    set_moe_gpu_stream_from(t.config.n_layers)
    set_moe_gpu_cls_off(-1l)
    reset_moe_gpu_dense_offs()
    reset_moe_gpu_dn_layers()
    reset_moe_gpu_attn_layers()
    // mx4/biased expert stacks never dispatch on the GPU, so they never upload either
    let nstream = int64(to_int(get_env_variable("DASLLAMA_GPU_MOE_STREAM")))
    let dn_want = get_env_variable("DASLLAMA_GPU_DN") == "1"
    let at_want = get_env_variable("DASLLAMA_GPU_ATTN") == "1"
    if (t.quant != QuantMode.q8 || t.config.n_expert <= 0l
            || (moe_gpu_layer_count() <= 0l && nstream <= 0l && !dn_want && !at_want)
            || t.experts_mx4 || t.config.moe_exps_bias) {
        return
    }
    let ts_gpu = ref_time_ticks()
    let layers = t.config.n_layers
    let dim = t.config.dim
    let ne = t.config.n_expert
    let nfe = t.config.n_ff_exp
    let want = min(moe_gpu_layer_count(), layers)
    let repacked = kernel_backend_needs_repack(active_kernel_backend())
    let mr = repacked ? active_q8_layout_mr() : 1l
    let kgroup = repacked ? active_q8_kgroup() : 4l
    let wbias = repacked ? active_q8_wbias() : 0l
    var from_l = layers
    var wq : array<uint8>
    var ws : array<uint8>
    // classifier plane first — the board's best ms-per-GB region (one whole-vocab GEMV serves
    // every token). One slice: the repack groups the plane globally, tail rows disk-order.
    // DASLLAMA_GPU_CLS=0 keeps it on the CPU (A/B lever).
    let cls_fmt = t.config.shared_weights ? t.emb_fmt : t.wcls_fmt
    let cls_servable = cls_fmt == KqFmt.q8 ? (t.cls_q8 || !t.config.shared_weights) : moe_gpu_fmt_kq(cls_fmt)
    if (cls_servable && get_env_variable("DASLLAMA_GPU_CLS") != "0"
            && moe_gpu_gather_upload(t, cls_fmt, t.wcls_off, dim, t.config.vocab_size, t.config.vocab_size, mr, kgroup, wbias, wq, ws)) {
        set_moe_gpu_cls_off(t.wcls_off)
    }
    // dense (attention-side) planes second — DASLLAMA_GPU_DENSE=1 opt-in. Arch-agnostic: recurrent
    // layers carry the deltanet triple, attention layers the q/o pair (k/v stay CPU — small).
    // Layer-ascending; each successful upload marks its plane, a budget stop ends the walk.
    if (get_env_variable("DASLLAMA_GPU_DENSE") == "1") {
        var dense_marked = 0l
        var dense_stop = false
        for (l in range64(layers)) {
            var uploads : array<tuple<f : KqFmt; woff : int64; n : int64; rows : int64>>
            if (layer_is_recurrent(t.config, l)) {
                if (t.dnqkv_offs[l] >= 0l && t.dngate_offs[l] >= 0l && t.dnout_offs[l] >= 0l) {
                    uploads |> push((f = KqFmt.q8, woff = t.dnqkv_offs[l], n = dim, rows = dn_conv_dim(t.config)))
                    uploads |> push((f = KqFmt.q8, woff = t.dngate_offs[l], n = dim, rows = t.config.ssm_d_inner))
                    uploads |> push((f = KqFmt.q8, woff = t.dnout_offs[l], n = t.config.ssm_d_inner, rows = dim))
                }
            } else {
                let qd = layer_qd(t.config, l)
                let fq = fmt_at(t.wq_fmt, l)
                let fo = fmt_at(t.wo_fmt, l)
                if (t.wq_offs[l] >= 0l && t.wo_offs[l] >= 0l
                        && (fq == KqFmt.q8 || moe_gpu_fmt_kq(fq)) && (fo == KqFmt.q8 || moe_gpu_fmt_kq(fo))
                        && kq_servable(t, fq) && kq_servable(t, fo)) {
                    uploads |> push((f = fq, woff = t.wq_offs[l], n = dim, rows = t.config.q_gated ? 2l * qd : qd))
                    uploads |> push((f = fo, woff = t.wo_offs[l], n = qd, rows = dim))
                }
            }
            for (u in uploads) {
                if (!moe_gpu_gather_upload(t, u.f, u.woff, u.n, u.rows, u.rows, mr, kgroup, wbias, wq, ws)) {
                    dense_stop = true   // VRAM budget hit — later planes keep the CPU dense path
                    break
                }
                set_moe_gpu_dense_off(int(u.f), u.woff)
                dense_marked++
            }
            delete uploads
            break if (dense_stop)
        }
        if (dense_marked > 0l) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: {dense_marked} dense planes resident\n")
        }
    }
    // deltanet triples third — DASLLAMA_GPU_DN=1 opt-in: each recurrent layer's qkv/z-gate/
    // out-proj planes + the layer mark that routes its whole prefill attention through the
    // device chain. Layer-ascending; a budget stop leaves later layers CPU, a partial triple rolls back.
    if (dn_want) {
        var dn_marked = 0l
        var dn_stop = false
        for (l in range64(layers)) {
            if (!layer_is_recurrent(t.config, l)
                    || t.dnqkv_offs[l] < 0l || t.dngate_offs[l] < 0l || t.dnout_offs[l] < 0l) {
                continue
            }
            if (moe_gpu_dense_on_gpu(int(KqFmt.q8), t.dnqkv_offs[l])) {
                set_moe_gpu_dn_layer(l)
                dn_marked++
                continue
            }
            let cd = dn_conv_dim(t.config)
            let di = t.config.ssm_d_inner
            var got = 0
            if (moe_gpu_gather_upload(t, KqFmt.q8, t.dnqkv_offs[l], dim, cd, cd, mr, kgroup, wbias, wq, ws)) {
                got++
            }
            if (got == 1 && moe_gpu_gather_upload(t, KqFmt.q8, t.dngate_offs[l], dim, di, di, mr, kgroup, wbias, wq, ws)) {
                got++
            }
            if (got == 2 && moe_gpu_gather_upload(t, KqFmt.q8, t.dnout_offs[l], di, dim, dim, mr, kgroup, wbias, wq, ws)) {
                got++
            }
            if (got < 3) {
                if (got > 0) {
                    moe_gpu_drop_stacks(got, false)
                }
                dn_stop = true
            }
            break if (dn_stop)
            set_moe_gpu_dn_layer(l)
            dn_marked++
        }
        if (dn_marked > 0l) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: deltanet triples of {dn_marked} layers resident\n")
        }
    }
    // attention quads fourth — DASLLAMA_GPU_ATTN=1 opt-in: each servable full-attention layer's
    // q|gate/k/v/wo planes + the layer mark that routes its whole prefill attention through the
    // device chain. Layer-ascending; a budget stop rolls back the partial quad.
    if (at_want) {
        let c = t.config
        // NEOX rope only (partial rotary implies NEOX per the loader); the chain has no bias /
        // v-norm / sinks / softcap / post-norm arms
        let arch_ok = (!c.attn_qkv_bias && !c.v_norm && !c.attn_sinks && c.attn_logit_softcap == 0.0
            && !c.pre_post_norm && !c.attn_out_bias
            && (c.rope_neox || (c.rope_dim > 0l && c.rope_dim != c.head_size)))
        var at_marked = 0l
        var at_stop = false
        for (l in range64(layers)) {
            break if (!arch_ok)
            continue if (layer_is_recurrent(c, l) || layer_is_sliding(c, l) || t.kv_src[l] != l
                || t.wq_offs[l] < 0l || t.wk_offs[l] < 0l || t.wv_offs[l] < 0l || t.wo_offs[l] < 0l)
            let hs = layer_head_size(c, l)
            let kvd = layer_kv_dim(c, l)
            let qd = c.n_heads * hs
            let rot = c.rope_dim > 0l ? c.rope_dim : hs
            let fq = fmt_at(t.wq_fmt, l)
            let fk = fmt_at(t.wk_fmt, l)
            let fv = fmt_at(t.wv_fmt, l)
            let fo = fmt_at(t.wo_fmt, l)
            // q/k/v share the chain's one activation image — format class must agree; wo is free
            let kq_cls = fq != KqFmt.q8
            var fmts_ok = (fk != KqFmt.q8) == kq_cls && (fv != KqFmt.q8) == kq_cls
            for (f in [fq, fk, fv, fo]) {
                fmts_ok = fmts_ok && (f == KqFmt.q8 || moe_gpu_fmt_kq(f)) && kq_servable(t, f)
            }
            continue if (!fmts_ok || !(hs == 128l || hs == 256l) || qd > 8192l || kvd > 1024l
                || c.n_heads > 64l || rot % 2l != 0l || rot > hs || kvd % 32l != 0l
                || qd % (fo != KqFmt.q8 ? 256l : 32l) != 0l || dim % 32l != 0l)
            var uploads : array<tuple<f : KqFmt; woff : int64; n : int64; rows : int64>>
            if (!moe_gpu_dense_on_gpu(int(fq), t.wq_offs[l])) {
                uploads |> push((f = fq, woff = t.wq_offs[l], n = dim, rows = c.q_gated ? 2l * qd : qd))
            }
            uploads |> push((f = fk, woff = t.wk_offs[l], n = dim, rows = kvd))
            uploads |> push((f = fv, woff = t.wv_offs[l], n = dim, rows = kvd))
            if (!moe_gpu_dense_on_gpu(int(fo), t.wo_offs[l])) {
                uploads |> push((f = fo, woff = t.wo_offs[l], n = qd, rows = dim))
            }
            var got = 0
            for (u in uploads) {
                if (!moe_gpu_gather_upload(t, u.f, u.woff, u.n, u.rows, u.rows, mr, kgroup, wbias, wq, ws)) {
                    at_stop = true
                    break
                }
                got++
            }
            delete uploads
            if (at_stop) {
                if (got > 0) {
                    moe_gpu_drop_stacks(got, false)
                }
                break
            }
            set_moe_gpu_attn_layer(l)
            at_marked++
        }
        if (at_marked > 0l) {
            to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: attention quads of {at_marked} layers resident\n")
        }
    }
    for (i in range64(want)) {
        let l = layers - 1l - i
        let f1 = fmt_at(t.we1_fmt, l)
        let f2 = fmt_at(t.we2_fmt, l)
        let f3 = fmt_at(t.we3_fmt, l)
        let all_q8 = f1 == KqFmt.q8 && f2 == KqFmt.q8 && f3 == KqFmt.q8
        let all_kq = moe_gpu_fmt_kq(f1) && moe_gpu_fmt_kq(f2) && moe_gpu_fmt_kq(f3)
        // dense-lead layers carry no expert stacks; the tier offloads plain-q8 or all-K-quant
        // stack triples (a q8/kq mix would need two activation forms in one chain) — either way
        // the walk stops so the resident set stays contiguous from the end
        if (l < t.config.n_layer_dense_lead || t.we1_offs[l] < 0l || !(all_q8 || all_kq)) {
            break
        }
        var got = 0
        if (moe_gpu_gather_upload(t, f1, t.we1_offs[l], dim, ne * nfe, nfe, mr, kgroup, wbias, wq, ws)) {
            got++
        }
        if (got == 1 && moe_gpu_gather_upload(t, f3, t.we3_offs[l], dim, ne * nfe, nfe, mr, kgroup, wbias, wq, ws)) {
            got++
        }
        if (got == 2 && moe_gpu_gather_upload(t, f2, t.we2_offs[l], nfe, ne * dim, dim, mr, kgroup, wbias, wq, ws)) {
            got++
        }
        if (got < 3) {
            // VRAM budget hit — this and lower layers stay on CPU. A partial triple MUST roll
            // back: a dangling gate/up plane hijacks the tier's residency check when this layer
            // re-enters as streamed (or CPU prefill probes it) — non-resident-region panic
            if (got > 0) {
                moe_gpu_drop_stacks(got, false)
            }
            break
        }
        from_l = l
    }
    // streamed tail: DASLLAMA_GPU_MOE_STREAM=N marks the next N layers below the resident set for
    // prefill streaming — device-layout planes gathered into pinned host mirrors the tier DMAs per
    // prefill. CPU planes stay (decode runs them); the walk is contiguous descending.
    var sfrom = from_l
    for (i in range64(max(nstream, 0l))) {
        let l = from_l - 1l - i
        if (l < t.config.n_layer_dense_lead || t.we1_offs[l] < 0l) {
            break
        }
        let f1 = fmt_at(t.we1_fmt, l)
        let f2 = fmt_at(t.we2_fmt, l)
        let f3 = fmt_at(t.we3_fmt, l)
        let all_q8 = f1 == KqFmt.q8 && f2 == KqFmt.q8 && f3 == KqFmt.q8
        let all_kq = moe_gpu_fmt_kq(f1) && moe_gpu_fmt_kq(f2) && moe_gpu_fmt_kq(f3)
        if (!(all_q8 || all_kq)) {
            break
        }
        var got = 0
        if (moe_gpu_gather_upload(t, f1, t.we1_offs[l], dim, ne * nfe, nfe, mr, kgroup, wbias, wq, ws, [stream = true])) {
            got++
        }
        if (got == 1 && moe_gpu_gather_upload(t, f3, t.we3_offs[l], dim, ne * nfe, nfe, mr, kgroup, wbias, wq, ws, [stream = true])) {
            got++
        }
        if (got == 2 && moe_gpu_gather_upload(t, f2, t.we2_offs[l], nfe, ne * dim, dim, mr, kgroup, wbias, wq, ws, [stream = true])) {
            got++
        }
        if (got < 3) {
            // pinned-mirror budget hit — this and lower layers stay CPU-only. Roll back a
            // partial triple: the stream registry's group arithmetic assumes whole gate/up/down
            // triples, and a phantom gate would match the group scan at dispatch
            if (got > 0) {
                moe_gpu_drop_stacks(got, true)
            }
            break
        }
        sfrom = l
    }
    delete wq
    delete ws
    set_moe_gpu_from_layer(from_l)
    set_moe_gpu_stream_from(sfrom)
    if (from_l < layers || moe_gpu_cls_on_gpu(t.wcls_off)) {
        let cls_note = moe_gpu_cls_on_gpu(t.wcls_off) ? "classifier + " : ""
        let stream_note = sfrom < from_l ? ", [{sfrom}..{from_l}) streamed" : ""
        to_log(LOG_INFO, "dasLLAMA: GPU MoE tier: {cls_note}expert stacks of layers [{from_l}..{layers}) resident{stream_note}, {get_time_usec(ts_gpu) / 1000} ms\n")
    }
}

//! Load a llama-architecture GGUF into the split layout at the requested precision.
// RepackReg: one mr-aligned row-chunk of a weight region. fmt: 0=q8, 1=mx4, 4/5/6=kq planes.
// Chunking splits huge regions (the classifier); 1024 is a multiple of every backend mr.
struct private RepackReg {
    off : int64
    n : int64
    d : int64
    fmt : int
}

def private push_repack(var regs : array<RepackReg>; fmt : int; off, n, d : int64) {
    var r0 = 0l
    while (r0 < d) {
        let rc = min(1024l, d - r0)
        regs |> push(RepackReg(off = off + r0 * n, n = n, d = rc, fmt = fmt))
        r0 += rc
    }
}

// Run every collected repack across the jobque — regions are disjoint, so any lane count is
// bit-exact. Workers touch hoisted raw pointers + the hoisted backend fn values only.
def private repack_regions(var t : Model; regs : array<RepackReg>) {
    if (empty(regs)) {
        return
    }
    let rq8 = g_repack_q8q8
    let rkq = g_repack_kq
    let rmx = g_repack_mx4
    unsafe {
        var qbp : int8? = null
        var qsp : float? = null
        var mxqp : uint8? = null
        var mxsp : uint8? = null
        var k4qp : uint8? = null
        var k4sp : uint8? = null
        var k5qp : uint8? = null
        var k5sp : uint8? = null
        var k6qp : uint8? = null
        var k6sp : uint8? = null
        var q40qp : uint8? = null
        var q40sp : uint8? = null
        if (!empty(t.qblob)) { qbp = addr(t.qblob[0]) }
        if (!empty(t.qscales)) { qsp = addr(t.qscales[0]) }
        if (!empty(t.mxq)) { mxqp = addr(t.mxq[0]) }
        if (!empty(t.mxs)) { mxsp = addr(t.mxs[0]) }
        if (!empty(t.k4q)) { k4qp = addr(t.k4q[0]) }
        if (!empty(t.k4s)) { k4sp = addr(t.k4s[0]) }
        if (!empty(t.k5q)) { k5qp = addr(t.k5q[0]) }
        if (!empty(t.k5s)) { k5sp = addr(t.k5s[0]) }
        if (!empty(t.k6q)) { k6qp = addr(t.k6q[0]) }
        if (!empty(t.k6s)) { k6sp = addr(t.k6s[0]) }
        if (!empty(t.q40q)) { q40qp = addr(t.q40q[0]) }
        if (!empty(t.q40s)) { q40sp = addr(t.q40s[0]) }
        let rp = addr<RepackReg const?>(regs[0])
        let nr = length(regs)
        let njobs = is_job_que_available() ? min(nr, 4 * (get_total_hw_jobs() + 1)) : 1
        maybe_parallel_for(0, nr, njobs) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    let f = rp[i].fmt
                    if (f == 0) {
                        invoke(rq8, qbp + rp[i].off, qsp + rp[i].off / 32l, rp[i].n, rp[i].d)
                    } elif (f == 1) {
                        invoke(rmx, mxqp + rp[i].off / 2l, mxsp + rp[i].off / 32l, rp[i].n, rp[i].d)
                    } else {
                        let sb = rp[i].off / 256l
                        let qsb = kq_qsb(f)
                        let ssb = kq_ssb(f)
                        var kqp = f == 4 ? k4qp : (f == 5 ? k5qp : (f == 6 ? k6qp : q40qp))
                        var ksp = f == 4 ? k4sp : (f == 5 ? k5sp : (f == 6 ? k6sp : q40sp))
                        invoke(rkq, f, kqp + sb * qsb, ksp + sb * ssb, rp[i].n, rp[i].d)
                    }
                }
            }
        }
    }
}

// Repack every Q8 2D weight tensor in qblob/qscales into the active backend's interleaved layout,
// IN PLACE, once at load. Mirrors forward()'s per-tensor offsets/shapes exactly; the embedding +
// norms live in fblob and are never touched.
def private repack_q8_weights(var t : Model) {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    var regs : array<RepackReg>
    for (l in range64(c.n_layers + c.n_layer_nextn)) {   // + the MTP/NextN block at index n_layers
        let kv = layer_kv_dim(c, l)
        let qd = layer_qd(c, l)
        // K-quant-tagged weights live in the kq planes (disk order, never repacked here) — their offsets are NOT qblob offsets
        if (t.wq_offs[l] >= 0l && fmt_at(t.wq_fmt, l) == KqFmt.q8) {
            push_repack(regs, 0, t.wq_offs[l], dim, qd * (c.q_gated ? 2l : 1l))
        }
        if (t.wk_offs[l] >= 0l && fmt_at(t.wk_fmt, l) == KqFmt.q8) {   // absent on E-series shared-KV layers
            push_repack(regs, 0, t.wk_offs[l], dim, kv)
        }
        if (t.wv_offs[l] >= 0l && fmt_at(t.wv_fmt, l) == KqFmt.q8) {
            push_repack(regs, 0, t.wv_offs[l], dim, kv)
        }
        if (t.wo_offs[l] >= 0l && fmt_at(t.wo_fmt, l) == KqFmt.q8) {
            push_repack(regs, 0, t.wo_offs[l], qd, dim)
        }
        if (layer_is_recurrent(c, l)) {  // deltanet projections (all q8 in v1 — no kq tags)
            let cd = dn_conv_dim(c)
            push_repack(regs, 0, t.dnqkv_offs[l], dim, cd)
            push_repack(regs, 0, t.dngate_offs[l], dim, c.ssm_d_inner)
            if (t.dnbeta_offs[l] >= 0l) {   // -1 when beta/alpha ship F32 (fblob, no repack)
                push_repack(regs, 0, t.dnbeta_offs[l], dim, c.ssm_dt_rank)
                push_repack(regs, 0, t.dnalpha_offs[l], dim, c.ssm_dt_rank)
            }
            push_repack(regs, 0, t.dnout_offs[l], c.ssm_d_inner, dim)
        }
        if (has_ple(c)) {  // gemma-4 E-series per-layer PLE gate/proj (the emb table stays linear, unpacked)
            let ple = c.n_embd_per_layer
            push_repack(regs, 0, t.ple_gate_off + l * dim * ple, dim, ple)
            push_repack(regs, 0, t.ple_proj_off + l * ple * dim, ple, dim)
        }
        if (c.n_expert > 0l && l >= c.n_layer_dense_lead) {   // MoE: each expert slice is its own 2D tensor, repacked per its shape
            let ege = dim * c.n_ff_exp
            if (!t.experts_mx4) {   // native-MXFP4 stacks live in mxq/mxs, row-major (no Q8 repack)
                for (e in range64(c.n_expert)) {
                    if (fmt_at(t.we1_fmt, l) == KqFmt.q8) {
                        push_repack(regs, 0, t.we1_offs[l] + e * ege, dim, c.n_ff_exp)
                    }
                    if (fmt_at(t.we2_fmt, l) == KqFmt.q8) {
                        push_repack(regs, 0, t.we2_offs[l] + e * ege, c.n_ff_exp, dim)
                    }
                    if (fmt_at(t.we3_fmt, l) == KqFmt.q8) {
                        push_repack(regs, 0, t.we3_offs[l] + e * ege, dim, c.n_ff_exp)
                    }
                }
            }
            if (c.n_ff_shexp > 0l) {
                let she = dim * c.n_ff_shexp
                push_repack(regs, 0, t.wsh1_off + l * she, dim, c.n_ff_shexp)
                push_repack(regs, 0, t.wsh2_off + l * she, c.n_ff_shexp, dim)
                push_repack(regs, 0, t.wsh3_off + l * she, dim, c.n_ff_shexp)
            }
            if (c.moe_dense_shexp) {  // gemma4: the dense parallel shared expert repacks like a plain FFN
                if (fmt_at(t.w1_fmt, l) == KqFmt.q8) {
                    push_repack(regs, 0, t.w1_offs[l], dim, hidden)
                }
                if (fmt_at(t.w2_fmt, l) == KqFmt.q8) {
                    push_repack(regs, 0, t.w2_offs[l], hidden, dim)
                }
                if (fmt_at(t.w3_fmt, l) == KqFmt.q8) {
                    push_repack(regs, 0, t.w3_offs[l], dim, hidden)
                }
            }
        } else {
            // dense arch layer OR a glm4moe dense-lead layer (a plain FFN, no expert stacks)
            let h = layer_hidden(t, l)
            if (fmt_at(t.w1_fmt, l) == KqFmt.q8) {
                push_repack(regs, 0, t.w1_offs[l], dim, h)
            }
            if (fmt_at(t.w2_fmt, l) == KqFmt.q8) {
                push_repack(regs, 0, t.w2_offs[l], h, dim)
            }
            if (fmt_at(t.w3_fmt, l) == KqFmt.q8) {
                push_repack(regs, 0, t.w3_offs[l], dim, h)
            }
        }
    }
    if ((!c.shared_weights && t.wcls_fmt == KqFmt.q8) || t.cls_q8) {
        // the linear copy at emb_q8_off is deliberately NOT repacked — embedding-row dequant indexes it directly
        push_repack(regs, 0, t.wcls_off, dim, c.vocab_size)
    }
    if (c.n_layer_nextn > 0l && t.mtp_ehproj_fmt == KqFmt.q8) {   // NextN eh_proj: 2*dim -> dim
        push_repack(regs, 0, t.mtp_ehproj_off, 2l * dim, dim)
    }
    repack_regions(t, regs)
    delete regs
}

// route one kq-tagged tensor to its format's plane pair (q8 tags no-op — those live in qblob)
def private push_repack_kq(var regs : array<RepackReg>; fmt : KqFmt; woff, n, d : int64) {
    if (fmt == KqFmt.k4) {
        push_repack(regs, 4, woff, n, d)
    } elif (fmt == KqFmt.k5) {
        push_repack(regs, 5, woff, n, d)
    } elif (fmt == KqFmt.k6) {
        push_repack(regs, 6, woff, n, d)
    } elif (fmt == KqFmt.q40) {
        push_repack(regs, 40, woff, n, d)
    }
}

// Repack every K-quant-tagged weight tensor into the active backend's grp<mr> kq layout, IN
// PLACE — the kq sibling of repack_q8_weights, called only when the backend carries kq slots.
// The tied/untied classifier repacks too: embed_row gathers its rows through the grp layout after.
def private repack_kq_weights(var t : Model) {
    let c = t.config
    let dim = c.dim
    var regs : array<RepackReg>
    for (l in range64(c.n_layers + c.n_layer_nextn)) {   // + the MTP/NextN block at index n_layers
        let kv = layer_kv_dim(c, l)
        let qd = layer_qd(c, l)
        // recurrent (deltanet) layers carry no attention projections; nothing kq to repack on them
        if (t.wq_offs[l] >= 0l) {
            push_repack_kq(regs, fmt_at(t.wq_fmt, l), t.wq_offs[l], dim, qd * (c.q_gated ? 2l : 1l))
        }
        if (t.wk_offs[l] >= 0l) {
            push_repack_kq(regs, fmt_at(t.wk_fmt, l), t.wk_offs[l], dim, kv)
        }
        if (t.wv_offs[l] >= 0l) {
            push_repack_kq(regs, fmt_at(t.wv_fmt, l), t.wv_offs[l], dim, kv)
        }
        if (t.wo_offs[l] >= 0l) {
            push_repack_kq(regs, fmt_at(t.wo_fmt, l), t.wo_offs[l], qd, dim)
        }
        if (c.n_expert > 0l && l >= c.n_layer_dense_lead) {
            if (!t.experts_mx4) {   // each expert slice grp-repacks independently, like the Q8 loop
                let ege = dim * c.n_ff_exp
                for (e in range64(c.n_expert)) {
                    push_repack_kq(regs, fmt_at(t.we1_fmt, l), t.we1_offs[l] + e * ege, dim, c.n_ff_exp)
                    push_repack_kq(regs, fmt_at(t.we2_fmt, l), t.we2_offs[l] + e * ege, c.n_ff_exp, dim)
                    push_repack_kq(regs, fmt_at(t.we3_fmt, l), t.we3_offs[l] + e * ege, dim, c.n_ff_exp)
                }
            }
            if (c.moe_dense_shexp) {
                push_repack_kq(regs, fmt_at(t.w1_fmt, l), t.w1_offs[l], dim, c.hidden_dim)
                push_repack_kq(regs, fmt_at(t.w2_fmt, l), t.w2_offs[l], c.hidden_dim, dim)
                push_repack_kq(regs, fmt_at(t.w3_fmt, l), t.w3_offs[l], dim, c.hidden_dim)
            }
        } else {
            // dense arch layer OR a glm4moe dense-lead layer; pure-MoE arches never reach here
            let h = layer_hidden(t, l)
            push_repack_kq(regs, fmt_at(t.w1_fmt, l), t.w1_offs[l], dim, h)
            push_repack_kq(regs, fmt_at(t.w2_fmt, l), t.w2_offs[l], h, dim)
            push_repack_kq(regs, fmt_at(t.w3_fmt, l), t.w3_offs[l], dim, h)
        }
    }
    if ((!c.shared_weights && t.wcls_fmt != KqFmt.q8) || cls_kq(t)) {
        push_repack_kq(regs, c.shared_weights ? t.emb_fmt : t.wcls_fmt, t.wcls_off, dim, c.vocab_size)
    }
    if (c.n_layer_nextn > 0l) {   // NextN eh_proj (no-op for q8 tags)
        push_repack_kq(regs, t.mtp_ehproj_fmt, t.mtp_ehproj_off, 2l * dim, dim)
    }
    repack_regions(t, regs)
    delete regs
}

// Repack the native-MXFP4 expert stacks into the active backend's interleaved mx layout, IN PLACE
// (each expert slice is its own 2D tensor). Mirrors repack_q8_weights' contract: called only after
// select_matmul_backend_for_load picks a repack backend; the pointer is otherwise identity.
def private repack_mx4_stacks(var t : Model) {
    let c = t.config
    let dim = c.dim
    let ege = dim * c.n_ff_exp
    var regs : array<RepackReg>
    for (l in range64(c.n_layers)) {
        for (e in range64(c.n_expert)) {
            let base = e * ege
            push_repack(regs, 1, t.we1_offs[l] + base, dim, c.n_ff_exp)
            push_repack(regs, 1, t.we2_offs[l] + base, c.n_ff_exp, dim)
            push_repack(regs, 1, t.we3_offs[l] + base, dim, c.n_ff_exp)
        }
    }
    repack_regions(t, regs)
    delete regs
}

//! go straight into `mode`'s storage (pre-quantized tensors transcode block-for-block, no fp32
//! round-trip) so a Q8/Q4 load never holds the dense fp32 weights. Embedding + norms stay fp32 in
//! fblob regardless; vocab is derived from token_embd (some files omit llama.vocab_size).
var private g_weights_epoch = 0l

//! Monotonic count of model loads this process. Weight caches keyed by HOST ADDRESS flush when it
//! moves — a new load can land arrays at a deleted model's recycled addresses, and an address-keyed
//! cache would silently serve the dead model's buffers.
def public weights_epoch() : int64 {
    return g_weights_epoch
}

//! The image loader's half of the same contract: mapping a prepared image lands planes at
//! fresh (possibly recycled) addresses exactly like a gguf load does, so it must move the
//! epoch too — dasllama_image calls this on every successful load_image.
def public bump_weights_epoch() {
    g_weights_epoch++
}

def load_gguf(path : string; mode : QuantMode = QuantMode.fp32) : Model {
    //! The conversion passes (transcode/repack/wscale) thread across the jobque; spin a scoped one
    //! for the load when the caller hasn't (the server/tools start theirs after the model loads).
    if (is_job_que_available()) {
        return <- load_gguf_impl(path, mode)
    }
    var t = Model()
    with_job_que() {
        t <- load_gguf_impl(path, mode)
    }
    return <- t
}

def private load_gguf_impl(path : string; mode : QuantMode) : Model {
    g_weights_epoch++
    var t = Model()
    t.quant = mode
    let ts_load = ref_time_ticks()
    let shards <- gguf_shard_paths(path)
    if (length(shards) > 1) {
        to_log(LOG_INFO, "dasLLAMA: split gguf — {length(shards)} shards\n")
        var bases : array<void?>
        var sizes : array<int64>
        gguf_map_shards(shards, 0, bases, sizes) {
            var m <- parse_gguf_meta_shards(bases, sizes)
            unsafe {
                load_gguf_parsed(t, m, temp_array(reinterpret<uint8?>(bases[0]), sizes[0], type<uint8>), mode, ts_load)
            }
            delete m
        }
        bases |> clear()   // borrowed mapping bases — clear so delete frees only the buffer
        delete bases
        delete sizes
        return <- t
    }
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let m <- parse_gguf_meta(bytes)
        load_gguf_parsed(t, m, bytes, mode, ts_load)
    }
    fclose(f)
    return <- t
}

// The load once metadata is parsed and the byte view carrying the KVs (shard 0 for splits) is
// mapped — config, vocab, plane sizing, tensor reads/transcodes, repack, wscale.
def private load_gguf_parsed(var t : Model; m : GGUFMeta; bytes : array<uint8> | #; mode : QuantMode; ts_load : int64) {
    var ts_stage = ref_time_ticks()
    {
        let arch = gguf_str(m, bytes, "general.architecture")
        resolve_arch(t, arch)   // registry lookup: apply config flags + bind the arch's block kernels
        if (gguf_has(m, "tokenizer.chat_template")) {
            t.chat_template_str = gguf_str(m, bytes, "tokenizer.chat_template")
        }
        // GGUF uses the architecture name as the metadata key prefix (llama.*, qwen2.*, gemma2.*, ...)
        let dim = gguf_int(m, bytes, "{arch}.embedding_length")
        // MTP/NextN: block_count INCLUDES the appended draft block(s) (llama.cpp n_layer_all);
        // `layers` below is the TRUNK count — every per-layer validation/mask walks the trunk only.
        let nextn = gguf_has(m, "{arch}.nextn_predict_layers") ? gguf_int(m, bytes, "{arch}.nextn_predict_layers") : 0l
        if (nextn < 0l || nextn > 1l) {
            panic("dasLLAMA: nextn_predict_layers = {nextn} not supported (single NextN head only)")
        }
        let layers = gguf_int(m, bytes, "{arch}.block_count") - nextn
        if (layers < 1l) {   // fail closed on a malformed file before "blk.-1..." confusion downstream
            panic("dasLLAMA: {arch}.block_count = {layers + nextn} with nextn_predict_layers = {nextn} leaves no trunk layers")
        }
        t.config.n_layer_nextn = nextn
        // FFN width: a scalar for uniform arches, or a per-layer array (gemma-4 E2B: the elastic
        // MatFormer sub-model, 6144 for layers 0-14 then 12288). ffn_w holds the per-layer widths;
        // hidden_dim is the MAX (scratch sizing). Set before layout_offsets so the offsets vary per layer.
        var hidden = 0l
        if (gguf_is_array(m, "{arch}.feed_forward_length")) {
            t.ffn_w <- gguf_int_array(m, bytes, "{arch}.feed_forward_length")
            if (int64(length(t.ffn_w)) != layers) {
                panic("dasLLAMA: feed_forward_length array has {length(t.ffn_w)} entries, expected {layers}")
            }
            for (w in t.ffn_w) { hidden = max(hidden, w) }
        } elif (gguf_has(m, "{arch}.feed_forward_length")) {
            hidden = gguf_int(m, bytes, "{arch}.feed_forward_length")
        } elif (!t.config.moe) {
            // pure-MoE files (qwen35moe) drop the key — every FFN is router + experts; a dense
            // arch without it is a broken file
            panic("dasLLAMA: {arch}.feed_forward_length missing")
        }
        let n_heads = gguf_int(m, bytes, "{arch}.attention.head_count")
        let seq_len = gguf_int(m, bytes, "{arch}.context_length")
        // vocab from the token-embedding tensor: rows = vocab, cols = dim
        let temb = gguf_find_tensor(m, "token_embd.weight")
        if (temb < 0) {
            panic("dasLLAMA: token_embd.weight missing")
        }
        let vocab = m.tensors[temb].n_elem / dim
        t.config.dim = dim
        t.config.hidden_dim = hidden
        t.config.n_layers = layers
        t.config.n_heads = n_heads
        t.config.vocab_size = vocab
        t.config.seq_len = seq_len
        // head_size from metadata key_length when present (Gemma2: 256, != dim/n_heads=288);
        // kv_dim = n_kv_heads * head_size. Both reduce to the llama formulas when head_size = dim/n_heads.
        t.config.head_size = (gguf_has(m, "{arch}.attention.key_length")
            ? gguf_int(m, bytes, "{arch}.attention.key_length")
            : dim / n_heads)
        // Gemma scales the embedding by sqrt(dim); configure can't (dim unknown there)
        if (t.config.embed_scale_sqrt_dim) {
            t.config.embed_scale = sqrt(float(dim))
        }
        // sliding-window span + per-layer pattern from metadata, for arches that opted in via
        // configure (Gemma2/3 alternating; gemma4 an explicit per-layer bool array -> swa_mask).
        // Resolved BEFORE the kv-head classes below, which key off layer_is_sliding.
        if (t.config.sliding_window > 0l) {
            if (gguf_has(m, "{arch}.attention.sliding_window")) {
                t.config.sliding_window = gguf_int(m, bytes, "{arch}.attention.sliding_window")
            }
            if (gguf_is_array(m, "{arch}.attention.sliding_window_pattern")) {
                if (layers > 64l) {
                    panic("dasLLAMA: per-layer sliding_window_pattern needs n_layers <= 64 (got {layers})")
                }
                let pat <- gguf_int_array(m, bytes, "{arch}.attention.sliding_window_pattern")
                for (l in range64(layers)) {
                    if (pat[l] != 0l) {
                        t.config.swa_mask |= 1ul << uint64(l)
                    }
                }
            } elif (gguf_has(m, "{arch}.attention.sliding_window_pattern")) {
                t.config.swa_pattern = gguf_int(m, bytes, "{arch}.attention.sliding_window_pattern")
            }
        }
        // qwen35 hybrid: the per-layer recurrent (Gated-DeltaNet) mask + ssm geometry. Mask from the
        // explicit bool array when present, else every layer except each full_attention_interval-th.
        // Parsed BEFORE the kv-head classes below, whose walk skips recurrent layers.
        if (t.config.hybrid_deltanet) {
            if (layers > 64l) {
                panic("dasLLAMA: the hybrid recurrent-layer mask needs n_layers <= 64 (got {layers})")
            }
            if (gguf_is_array(m, "{arch}.attention.recurrent_layers")) {
                let rl <- gguf_int_array(m, bytes, "{arch}.attention.recurrent_layers")
                if (int64(length(rl)) != layers) {
                    panic("dasLLAMA: recurrent_layers array has {length(rl)} entries, expected {layers}")
                }
                for (l in range64(layers)) {
                    if (rl[l] != 0l) {
                        t.config.recr_mask |= 1ul << uint64(l)
                    }
                }
            } else {
                let interval = (gguf_has(m, "{arch}.full_attention_interval")
                    ? gguf_int(m, bytes, "{arch}.full_attention_interval") : 4l)
                if (interval <= 0l) {
                    panic("dasLLAMA: full_attention_interval must be positive (got {interval})")
                }
                for (l in range64(layers)) {
                    if ((l + 1l) % interval != 0l) {
                        t.config.recr_mask |= 1ul << uint64(l)
                    }
                }
            }
            t.config.ssm_d_conv = gguf_int(m, bytes, "{arch}.ssm.conv_kernel")
            t.config.ssm_d_state = gguf_int(m, bytes, "{arch}.ssm.state_size")
            t.config.ssm_n_group = gguf_int(m, bytes, "{arch}.ssm.group_count")
            t.config.ssm_dt_rank = gguf_int(m, bytes, "{arch}.ssm.time_step_rank")
            t.config.ssm_d_inner = gguf_int(m, bytes, "{arch}.ssm.inner_size")
            if (t.config.ssm_d_inner != t.config.ssm_dt_rank * t.config.ssm_d_state) {
                panic("dasLLAMA: deltanet inner_size {t.config.ssm_d_inner} != time_step_rank {t.config.ssm_dt_rank} * state_size {t.config.ssm_d_state}")
            }
            if (t.config.ssm_d_state <= 0l || t.config.ssm_n_group <= 0l || t.config.ssm_dt_rank <= 0l
                    || t.config.ssm_d_conv < 2l || t.config.ssm_dt_rank % t.config.ssm_n_group != 0l) {
                panic("dasLLAMA: unsupported deltanet geometry (conv_kernel {t.config.ssm_d_conv}, state {t.config.ssm_d_state}, v-heads {t.config.ssm_dt_rank}, k-heads {t.config.ssm_n_group})")
            }
            if (dn_conv_dim(t.config) % 4l != 0l) {   // the batched conv reads float4 rows (validate-at-load, kv_validate_q8 precedent)
                panic("dasLLAMA: deltanet conv_dim {dn_conv_dim(t.config)} must be a multiple of 4")
            }
        }
        // MTP/NextN consistency (arch-agnostic — qwen35 hybrids AND glm4moe carry draft blocks):
        // the metadata and the tensor directory must agree, and an MTP-only sidecar (trunk
        // stripped, llama.cpp mtp_only) is not loadable standalone
        if (t.config.n_layer_nextn > 0l) {
            if (gguf_find_tensor(m, "blk.{layers}.nextn.eh_proj.weight") < 0) {
                panic("dasLLAMA: nextn_predict_layers = {t.config.n_layer_nextn} but blk.{layers}.nextn.eh_proj.weight is missing")
            }
            if (gguf_find_tensor(m, "blk.0.attn_norm.weight") < 0) {
                panic("dasLLAMA: MTP-only sidecar GGUF (no trunk tensors) is not supported — use the full -MTP file")
            }
            // glm4moe ships the draft head with its OWN embed_tokens/shared_head_head; the das MTP
            // head drafts through the TRUNK's embedding/classifier instead, so drafting here would
            // use the wrong weights — disable the head honestly rather than mis-draft.
            if (gguf_find_tensor(m, "blk.{layers}.nextn.embed_tokens.weight") >= 0
                    || gguf_find_tensor(m, "blk.{layers}.nextn.shared_head_head.weight") >= 0) {
                to_log(LOG_WARNING, "dasLLAMA: NextN block carries its own embed_tokens/shared_head_head — MTP draft head disabled\n")
                t.config.n_layer_nextn = 0l
            }
        } elif (gguf_find_tensor(m, "blk.{layers}.nextn.eh_proj.weight") >= 0
                || gguf_find_tensor(m, "blk.{layers - 1l}.nextn.eh_proj.weight") >= 0) {
            // the second probe catches a malformed file whose block_count already counts the
            // draft block — its nextn tensors sit one below the first probe's index
            panic("dasLLAMA: file ships NextN tensors but no nextn_predict_layers key — refusing to guess")
        }
        // KV-head count: scalar (uniform), or gemma4's per-layer array — which must collapse to the
        // TWO layer classes (one value on sliding layers, one on global) the engine models
        if (gguf_is_array(m, "{arch}.attention.head_count_kv")) {
            let kvh <- gguf_int_array(m, bytes, "{arch}.attention.head_count_kv")
            var full_v = -1l
            var swa_v = -1l
            for (l in range64(layers)) {
                if (layer_is_recurrent(t.config, l)) {  // deltanet layers have no KV heads (0 or absent)
                    continue
                }
                if (layer_is_sliding(t.config, l)) {
                    if (swa_v < 0l) {
                        swa_v = kvh[l]
                    } elif (swa_v != kvh[l]) {
                        panic("dasLLAMA: per-layer head_count_kv beyond two sliding/global classes is not supported")
                    }
                } elif (full_v < 0l) {
                    full_v = kvh[l]
                } elif (full_v != kvh[l]) {
                    panic("dasLLAMA: per-layer head_count_kv beyond two sliding/global classes is not supported")
                }
            }
            t.config.n_kv_heads = full_v >= 0l ? full_v : swa_v
            if (swa_v >= 0l && swa_v != t.config.n_kv_heads) {
                t.config.n_kv_heads_swa = swa_v
            }
        } else {
            t.config.n_kv_heads = gguf_int(m, bytes, "{arch}.attention.head_count_kv")
        }
        // sliding layers' head size (gemma4: 256 vs the global 512); K and V widths must agree
        if (gguf_has(m, "{arch}.attention.key_length_swa")) {
            let hs_swa = gguf_int(m, bytes, "{arch}.attention.key_length_swa")
            if (gguf_has(m, "{arch}.attention.value_length_swa")
                    && gguf_int(m, bytes, "{arch}.attention.value_length_swa") != hs_swa) {
                panic("dasLLAMA: key_length_swa != value_length_swa is not supported")
            }
            if (hs_swa != t.config.head_size) {
                t.config.head_size_swa = hs_swa
            }
        }
        t.config.kv_dim = t.config.n_kv_heads * t.config.head_size
        // the engine ropes over the full head unless the arch opted into partial rotary (qwen35:
        // rotate the first rope_dim of head_size dims; text-only M-RoPE sections degenerate to
        // this, see the qwen3vl precedent) — anything else refuses instead of mis-running
        if (gguf_has(m, "{arch}.rope.dimension_count")
                && gguf_int(m, bytes, "{arch}.rope.dimension_count") != t.config.head_size) {
            if (t.config.partial_rope) {
                t.config.rope_dim = gguf_int(m, bytes, "{arch}.rope.dimension_count")
                if (t.config.rope_dim <= 0l || t.config.rope_dim % 2l != 0l || t.config.rope_dim > t.config.head_size) {
                    panic("dasLLAMA: bad partial rotary width {t.config.rope_dim} (head_size {t.config.head_size})")
                }
                if (!t.config.rope_neox) {   // only the NEOX _part leaves exist (qwen35/glm4moe are NEOX)
                    panic("dasLLAMA: partial rotary is only supported with NEOX rope")
                }
            } else {
                panic("dasLLAMA: partial rotary (rope.dimension_count != head_size) is not supported")
            }
        }
        if (gguf_has(m, "{arch}.rope.dimension_count_swa")) {
            let swa_hs = t.config.head_size_swa > 0l ? t.config.head_size_swa : t.config.head_size
            if (gguf_int(m, bytes, "{arch}.rope.dimension_count_swa") != swa_hs) {
                panic("dasLLAMA: partial rotary (rope.dimension_count_swa != the sliding head size) is not supported")
            }
        }
        // gemma-4 E-series: cross-layer KV sharing — the last N layers reuse an earlier layer's KV
        // cache (n_layer_kv_from_start = layers - shared), and their own attn_k/attn_v are dead weights
        // (skipped by layout_offsets). Mirrors llama.cpp gemma4's has_kv()/reuse().
        if (gguf_has(m, "{arch}.attention.shared_kv_layers")) {
            let n_shared = gguf_int(m, bytes, "{arch}.attention.shared_kv_layers")
            if (n_shared > 0l) {
                t.config.n_layer_kv_from_start = layers - n_shared
            }
        }
        // gemma-4 E-series: per-layer embeddings (PLE) — a per-(token, layer) side input mixed into
        // each layer's output. n_embd_per_layer = 0 on the dense 12B/31B (feature off).
        if (gguf_has(m, "{arch}.embedding_length_per_layer_input")) {
            t.config.n_embd_per_layer = gguf_int(m, bytes, "{arch}.embedding_length_per_layer_input")
        }
        // gemma4 is MoE only when the file carries experts (12B/31B dense, 26B-A4B MoE)
        if (t.config.moe_optional
                && gguf_has(m, "{arch}.expert_count") && gguf_int(m, bytes, "{arch}.expert_count") > 0l) {
            t.config.moe = true
        }
        if (t.config.moe) {
            // MoE geometry from metadata (qwen2moe A2.7B: 60 experts, top-4, 1408-wide experts
            // + a 5632-wide shared expert)
            if (!gguf_has(m, "{arch}.expert_count") || gguf_int(m, bytes, "{arch}.expert_count") <= 0l) {
                panic("dasLLAMA: '{arch}' is a MoE arch but the GGUF has no {arch}.expert_count")
            }
            t.config.n_expert = gguf_int(m, bytes, "{arch}.expert_count")
            t.config.n_expert_used = gguf_has(m, "{arch}.expert_used_count") ? gguf_int(m, bytes, "{arch}.expert_used_count") : 0l
            if (t.config.n_expert_used <= 0l || t.config.n_expert_used > t.config.n_expert) {
                panic("dasLLAMA: bad expert_used_count {t.config.n_expert_used} (expert_count {t.config.n_expert})")
            }
            t.config.n_ff_exp = (gguf_has(m, "{arch}.expert_feed_forward_length")
                ? gguf_int(m, bytes, "{arch}.expert_feed_forward_length")
                : hidden / t.config.n_expert_used)   // llama.cpp's fallback for files without the key
            if (t.config.n_ff_exp <= 0l) {   // pure-MoE files may omit feed_forward_length (hidden 0) — then the expert key is mandatory
                panic("dasLLAMA: expert_feed_forward_length missing (and no dense feed_forward_length to derive it)")
            }
            // glm4moe: the first k layers are plain dense FFN — every MoE tensor probe below keys
            // off the FIRST ROUTED layer, not blk.0 (blk.0 is dense there and would misdetect)
            if (gguf_has(m, "{arch}.leading_dense_block_count")) {
                t.config.n_layer_dense_lead = gguf_int(m, bytes, "{arch}.leading_dense_block_count")
                if (t.config.n_layer_dense_lead < 0l || t.config.n_layer_dense_lead >= layers) {
                    panic("dasLLAMA: leading_dense_block_count = {t.config.n_layer_dense_lead} leaves no routed layers ({layers} trunk layers)")
                }
            }
            let lead = t.config.n_layer_dense_lead
            // shared expert: present iff its tensors are (qwen2moe always ships one; glm4moe's is
            // UNGATED — no ffn_gate_inp_shexp — and its width is n_ff_exp x expert_shared_count)
            if (gguf_find_tensor(m, "blk.{lead}.ffn_up_shexp.weight") >= 0) {
                t.config.n_ff_shexp = (gguf_has(m, "{arch}.expert_shared_feed_forward_length")
                    ? gguf_int(m, bytes, "{arch}.expert_shared_feed_forward_length")
                    : (gguf_has(m, "{arch}.expert_shared_count")
                        ? t.config.n_ff_exp * gguf_int(m, bytes, "{arch}.expert_shared_count")
                        : hidden))
                if (t.config.n_ff_shexp <= 0l) {
                    panic("dasLLAMA: expert_shared_feed_forward_length missing (and no dense feed_forward_length to derive it)")
                }
                t.config.moe_shexp_gated = gguf_find_tensor(m, "blk.{lead}.ffn_gate_inp_shexp.weight") >= 0
            }
            if (gguf_has(m, "{arch}.expert_weights_norm")) {
                t.config.norm_topk_prob = gguf_int(m, bytes, "{arch}.expert_weights_norm") != 0l
            }
            if (gguf_has(m, "{arch}.expert_weights_scale")) {
                t.config.expert_weights_scale = gguf_f32(m, bytes, "{arch}.expert_weights_scale")
            }
            if (gguf_has(m, "{arch}.expert_gating_func")) {
                // llama.cpp enum: 1 = softmax over the logits, 2 = sigmoid, 3 = softmax over the
                // selected top-k weights (gpt-oss hardcodes 3 in its graph — no GGUF key there,
                // so its arch configure sets the default; bailingmoe2 files carry the key)
                let gf = gguf_int(m, bytes, "{arch}.expert_gating_func")
                if (gf == 2l) {
                    t.config.moe_gate = MoeGate.sigmoid
                } elif (gf == 3l) {
                    t.config.moe_gate = MoeGate.softmax_weight
                } elif (gf != 1l) {
                    panic("dasLLAMA: unsupported expert_gating_func {gf}")
                }
            }
            // DeepSeek-V3 grouped expert routing is not modeled — refuse instead of mis-routing
            if (gguf_has(m, "{arch}.expert_group_count") && gguf_int(m, bytes, "{arch}.expert_group_count") > 1l) {
                panic("dasLLAMA: grouped expert routing (expert_group_count > 1) is not yet supported")
            }
            // DeepSeek-V3/glm4moe expert-selection bias: added to the (sigmoid) probs for the top-k
            // PICK only; the mixing weights stay unbiased (llama.cpp build_moe_ffn)
            t.config.moe_exp_probs = gguf_find_tensor(m, "blk.{lead}.exp_probs_b.bias") >= 0
            // router / per-expert biases (gpt-oss) — detected from the tensor table; the gate/up
            // pair always ships together, so probing ffn_up_exps.bias covers all three stacks
            t.config.moe_router_bias = gguf_find_tensor(m, "blk.{lead}.ffn_gate_inp.bias") >= 0
            t.config.moe_exps_bias = gguf_find_tensor(m, "blk.{lead}.ffn_up_exps.bias") >= 0
            // gemma4 family: a DENSE parallel shared expert (plain ffn_gate/up/down alongside the routed
            // experts) plus the fused gate_up stacks + sandwich norms + custom router that ride with it
            // (probed at the first ROUTED layer — a glm4moe dense-lead blk.0.ffn_gate must not trip it)
            t.config.moe_dense_shexp = gguf_find_tensor(m, "blk.{lead}.ffn_gate.weight") >= 0
        } elif (gguf_has(m, "{arch}.expert_count") && gguf_int(m, bytes, "{arch}.expert_count") > 0l) {
            panic("dasLLAMA: MoE expert blocks are not supported for arch '{arch}'")
        }
        // attention output-projection bias + per-head sink logits (gpt-oss) — tensor-detected
        t.config.attn_out_bias = gguf_find_tensor(m, "blk.0.attn_output.bias") >= 0
        t.config.attn_sinks = gguf_find_tensor(m, "blk.0.attn_sinks.weight") >= 0
        // QK-norm ships per-FILE within some families (GLM-4.5 355B carries attn_q_norm, Air
        // doesn't) — an arch that didn't opt in would silently skip the norms, so refuse instead
        if (!t.config.qk_norm && gguf_find_tensor(m, "blk.0.attn_q_norm.weight") >= 0) {
            panic("dasLLAMA: file ships attn_q_norm but arch '{arch}' does not configure qk_norm — refusing to mis-run")
        }
        // llama4-style per-position attention temperature tuning (mistral3 carries the key, shipped
        // as 0 = disabled) — no Q-scaling path here, fail closed rather than silently mis-attend
        let tskey = "{arch}.attention.temperature_scale"
        if (gguf_has(m, tskey) && gguf_f32(m, bytes, tskey) != 0.0) {
            panic("dasLLAMA: {arch}.attention.temperature_scale != 0 (attention temperature tuning) is not supported")
        }
        // RoPE base frequency (θ): default 10000, but Llama-3 uses 500000
        t.config.rope_freq_base = gguf_has(m, "{arch}.rope.freq_base") ? gguf_f32(m, bytes, "{arch}.rope.freq_base") : 10000.0
        // linear RoPE position scaling (Gemma3-4B: factor 8 on global layers); YaRN / LongRoPE
        // variants keep their own handling (rope_freqs factors + attn_factor below)
        if (gguf_has(m, "{arch}.rope.scaling.type") && gguf_str(m, bytes, "{arch}.rope.scaling.type") == "linear"
                && gguf_has(m, "{arch}.rope.scaling.factor")) {
            t.config.rope_freq_scale = 1.0 / gguf_f32(m, bytes, "{arch}.rope.scaling.factor")
        }
        // sliding layers' θ: metadata override when present, else configure's value; the 0 sentinel
        // (arch didn't set it) resolves to the global θ so per-layer selection is uniform
        if (gguf_has(m, "{arch}.rope.freq_base_swa")) {
            t.config.rope_freq_base_swa = gguf_f32(m, bytes, "{arch}.rope.freq_base_swa")
        }
        if (t.config.rope_freq_base_swa == 0.0) {
            t.config.rope_freq_base_swa = t.config.rope_freq_base
            t.config.rope_freq_scale_swa = t.config.rope_freq_scale
        }
        // RMSNorm epsilon from metadata (Llama 1e-5; Qwen2 / Gemma2 1e-6); default matches Llama
        let epskey = "{arch}.attention.layer_norm_rms_epsilon"
        t.config.norm_eps = gguf_has(m, epskey) ? gguf_f32(m, bytes, epskey) : 1.0e-5
        // Per-pair RoPE frequency factors: Llama-3.1 NTK-by-parts (rope_freqs.weight) or Phi3 LongRoPE
        // short-context factors (rope_factors_short.weight). Both divide θ per pair (head_size/2 values).
        let rf_name = (gguf_find_tensor(m, "rope_factors_short.weight") >= 0
            ? "rope_factors_short.weight"
            : (gguf_find_tensor(m, "rope_freqs.weight") >= 0 ? "rope_freqs.weight" : ""))
        if (rf_name != "") {
            let nff = t.config.head_size / 2l
            t.rope_freqs |> resize(nff)
            gguf_read_tensor_f32(m, bytes, rf_name, t.rope_freqs, 0l, nff)
            // the _part leaves take a factor pointer, but no partial-rope model ships factors —
            // the untested combination stays fail-closed (std paths pass has_ff=false when partial)
            if (t.config.rope_dim > 0l && t.config.rope_dim != t.config.head_size) {
                panic("dasLLAMA: partial rotary combined with per-pair rope factors is not supported")
            }
        }
        // Phi3 LongRoPE magnitude factor (attn_factor) scales the rope output; absent (=1.0) elsewhere
        let mskey = "{arch}.rope.scaling.attn_factor"
        t.config.rope_mscale = gguf_has(m, mskey) ? gguf_f32(m, bytes, mskey) : 1.0
        // YaRN (gpt-oss: factor 32 over a 4096 original context). Folds into the existing per-pair
        // machinery: synthesize rope_freqs[j] = 1/eff_j (factors DIVIDE θ), keep rope_freq_scale = 1,
        // fold the magnitude correction 1 + 0.1·ln(factor) into rope_mscale.
        if (gguf_has(m, "{arch}.rope.scaling.type") && gguf_str(m, bytes, "{arch}.rope.scaling.type") == "yarn") {
            if (!empty(t.rope_freqs)) {
                panic("dasLLAMA: YaRN combined with per-pair rope factors is not supported")
            }
            if (t.config.rope_dim > 0l && t.config.rope_dim != t.config.head_size) {
                panic("dasLLAMA: partial rotary combined with YaRN is not supported")
            }
            // yarn_log_multiplier replaces the 0.1 in the mscale correction (DeepSeek-2 lineage);
            // mistral3 files carry it as 0, where llama.cpp's cancel-and-reapply dance nets out to
            // the plain 1 + 0.1·ln(factor) below — any other value needs that dance, fail closed
            let lmkey = "{arch}.rope.scaling.yarn_log_multiplier"
            if (gguf_has(m, lmkey) && gguf_f32(m, bytes, lmkey) != 0.0) {
                panic("dasLLAMA: {arch} YaRN yarn_log_multiplier != 0 is not supported")
            }
            let yfactor = gguf_f32(m, bytes, "{arch}.rope.scaling.factor")
            let yorig = (gguf_has(m, "{arch}.rope.scaling.original_context_length")
                ? gguf_int(m, bytes, "{arch}.rope.scaling.original_context_length") : seq_len)
            let fscale = 1.0 / yfactor
            // NTK-by-parts correction band (ggml_rope_yarn_corr_dims; beta_fast/slow from metadata,
            // llama.cpp defaults 32/1): pairs below `lo` rotate at full frequency (extrapolate),
            // above `hi` fully interpolate (mistral3: beta_fast 4 widens the band vs gpt-oss's 32)
            let bfkey = "{arch}.rope.scaling.yarn_beta_fast"
            let bskey = "{arch}.rope.scaling.yarn_beta_slow"
            let beta_fast = gguf_has(m, bfkey) ? gguf_f32(m, bytes, bfkey) : 32.0
            let beta_slow = gguf_has(m, bskey) ? gguf_f32(m, bytes, bskey) : 1.0
            if (beta_fast <= 0.0 || beta_slow <= 0.0) {   // divisors inside log() below — fail closed on a malformed file
                panic("dasLLAMA: YaRN beta_fast/beta_slow must be > 0 (got {beta_fast}/{beta_slow})")
            }
            let hs = t.config.head_size
            let denom = 2.0 * log(t.config.rope_freq_base)
            let lo = max(0.0, floor(float(hs) * log(float(yorig) / (beta_fast * 2.0 * PI)) / denom))
            let hi = min(float(hs - 1l), ceil(float(hs) * log(float(yorig) / (beta_slow * 2.0 * PI)) / denom))
            t.rope_freqs |> resize(hs / 2l)
            for (j in range64(hs / 2l)) {
                let ramp = 1.0 - clamp((float(j) - lo) / max(0.001, hi - lo), 0.0, 1.0)  // = rope_yarn_ramp(lo, hi, i0 = 2j): ggml halves i0, and lo/hi are already pair-unit
                t.rope_freqs[j] = 1.0 / (fscale + ramp * (1.0 - fscale))
            }
            t.config.rope_mscale *= 1.0 + 0.1 * log(1.0 / fscale)
        }
        // a separate output.weight means the classifier is not tied to the embeddings
        t.config.shared_weights = gguf_find_tensor(m, "output.weight") < 0
        // tied Q8 loads of a Q8_0 embedding matmul the classifier on the DISK quants (llama.cpp's
        // arithmetic) and drop the fp32 table — see Model.cls_q8. Other disk types keep the exact
        // fp32 path (a requantized classifier would drift from the oracle for no parity gain).
        t.cls_q8 = (t.config.shared_weights && mode == QuantMode.q8
            && gguf_tensor_type(m, "token_embd.weight") == GGML_TYPE_Q8_0)
        // per-tensor K-quant format tags (kquant_native rail; no-op when the knob is off or the
        // file has no K-quant tensors) — must precede layout_offsets, which walks the tags
        detect_kq_formats(t, m)
        // ssm_beta/ssm_alpha disk type routes their matmul (must precede layout_offsets): F32
        // files keep them fp32 in fblob (llama.cpp matmuls f32); quantized files ride qblob
        if (t.config.recr_mask != 0ul) {
            var frl = 0l
            while (!layer_is_recurrent(t.config, frl)) {
                frl++
            }
            t.dn_grouped = gguf_find_tensor(m, "blk.{frl}.ssm_ba.weight") >= 0
            let ba0 = t.dn_grouped ? "blk.{frl}.ssm_ba.weight" : "blk.{frl}.ssm_beta.weight"
            t.dn_ba_f32 = gguf_tensor_type(m, ba0) == GGML_TYPE_F32
        }
        // logit soft-caps from metadata when present (gemma2: 50/30, gemma4: final 30 only);
        // configure()'s values stay as the fallback for files without the keys
        if (gguf_has(m, "{arch}.attn_logit_softcapping")) {
            t.config.attn_logit_softcap = gguf_f32(m, bytes, "{arch}.attn_logit_softcapping")
        }
        if (gguf_has(m, "{arch}.final_logit_softcapping")) {
            t.config.final_logit_softcap = gguf_f32(m, bytes, "{arch}.final_logit_softcapping")
        }
        // suppressed token ids (gemma4: the checkpoint's known-bad <image|>/<audio|>) — llama.cpp
        // pins their logits to -inf in the graph, so parity requires the same
        if (gguf_has(m, "tokenizer.ggml.suppress_tokens")) {
            t.suppress <- gguf_i32_array(m, bytes, "tokenizer.ggml.suppress_tokens")
        }
        // layers with no attn_v tensor reuse the pre-norm K projection as V (gemma4 global layers);
        // detected from the tensor table so layout_offsets can skip their V slot
        if (!t.config.fused_qkv) {
            for (l in range64(layers)) {
                // recurrent (deltanet) layers legitimately have no attn_v — they are NOT V-from-K
                if (gguf_find_tensor(m, "blk.{l}.attn_v.weight") < 0 && !layer_is_recurrent(t.config, l)) {
                    if (l >= 64l) {
                        panic("dasLLAMA: V-from-K layers need n_layers <= 64 (got layer {l})")
                    }
                    t.config.v_from_k_mask |= 1ul << uint64(l)
                }
            }
        }
        // per-layer output scalars (gemma4 layer_output_scale) — presence detected from the tensors
        t.config.layer_out_scale = gguf_find_tensor(m, "blk.0.layer_output_scale.weight") >= 0
        // Native-MXFP4 expert stacks (gpt-oss): on a q8 load whose routed expert stacks are all
        // MXFP4 on disk, keep them in their native nibble + E8M0 planes (see Model.experts_mx4)
        // instead of transcoding to Q8 — half the resident bytes and decode traffic, exact disk bits.
        t.experts_mx4 = (t.config.n_expert > 0l && mode == QuantMode.q8
            && gguf_tensor_type(m, "blk.0.ffn_gate_exps.weight") == GGML_TYPE_MXFP4
            && gguf_tensor_type(m, "blk.0.ffn_down_exps.weight") == GGML_TYPE_MXFP4
            && gguf_tensor_type(m, "blk.0.ffn_up_exps.weight") == GGML_TYPE_MXFP4)
        // gemma-4 E-series: keep per_layer_model_proj in its native BF16 — half the per-token
        // read; dot_bf16's widen is the exact bit shift, so the values match the fp32 expand
        t.ple_model_bf16 = (has_ple(t.config)
            && gguf_tensor_type(m, "per_layer_model_proj.weight") == GGML_TYPE_BF16)
        let kv = t.config.kv_dim
        let qd = t.config.n_heads * t.config.head_size   // query width (== dim except Gemma2)
        let sz = layout_offsets(t)
        // fp32 aux (embedding + norms) is always present; the big-weight storage matches `mode`
        reserve_resize(t.fblob, sz.fblob_n)
        if (sz.bf16_n > 0l) {
            t.bf16blob |> reserve(sz.bf16_n)
            t.bf16blob |> resize(sz.bf16_n)
        }
        if (sz.mx_n > 0l) {
            t.mxq |> reserve(sz.mx_n / 2l)
            t.mxq |> resize(sz.mx_n / 2l)
            t.mxs |> reserve(sz.mx_n / 32l)
            t.mxs |> resize(sz.mx_n / 32l)
        }
        if (sz.k4_n > 0l) {
            t.k4q |> reserve((sz.k4_n / 256l) * K4_QSB)
            t.k4q |> resize((sz.k4_n / 256l) * K4_QSB)
            t.k4s |> reserve((sz.k4_n / 256l) * K4_SSB)
            t.k4s |> resize((sz.k4_n / 256l) * K4_SSB)
        }
        if (sz.k5_n > 0l) {
            t.k5q |> reserve((sz.k5_n / 256l) * K5_QSB)
            t.k5q |> resize((sz.k5_n / 256l) * K5_QSB)
            t.k5s |> reserve((sz.k5_n / 256l) * K5_SSB)
            t.k5s |> resize((sz.k5_n / 256l) * K5_SSB)
        }
        if (sz.k6_n > 0l) {
            t.k6q |> reserve((sz.k6_n / 256l) * K6_QSB)
            t.k6q |> resize((sz.k6_n / 256l) * K6_QSB)
            t.k6s |> reserve((sz.k6_n / 256l) * K6_SSB)
            t.k6s |> resize((sz.k6_n / 256l) * K6_SSB)
        }
        if (sz.q40_n > 0l) {
            t.q40q |> reserve((sz.q40_n / 256l) * Q40_QSB)
            t.q40q |> resize((sz.q40_n / 256l) * Q40_QSB)
            t.q40s |> reserve((sz.q40_n / 256l) * Q40_SSB)
            t.q40s |> resize((sz.q40_n / 256l) * Q40_SSB)
        }
        if (mode == QuantMode.q8) {
            t.qblob |> reserve(sz.wblob_n)
            t.qblob |> resize(sz.wblob_n)
            t.qscales |> reserve(sz.wblob_n / 32l)
            t.qscales |> resize(sz.wblob_n / 32l)
        } elif (mode == QuantMode.q4_0) {
            t.q4blob |> reserve(sz.wblob_n / 2l)
            t.q4blob |> resize(sz.wblob_n / 2l)
            t.q4scales |> reserve(sz.wblob_n / 32l)
            t.q4scales |> resize(sz.wblob_n / 32l)
        } else {
            reserve_resize(t.wblob, sz.wblob_n)
        }
        // embedding + norms -> fblob (decoded to fp32 from whatever the disk type is); cls_q8 /
        // cls_kq keep the embedding out of fblob — the quantized copies transcode with the big
        // weights below
        if (!t.cls_q8 && !cls_kq(t)) {
            gguf_read_tensor_f32(m, bytes, "token_embd.weight", t.fblob, t.tok_emb_off, vocab * dim)
        }
        // gpt-oss names its pre-FFN norm post_attention_norm (same graph role as ffn_norm)
        let ffn_norm_name = t.config.post_attn_is_ffn_norm ? "post_attention_norm" : "ffn_norm"
        // per-layer walks below include the MTP/NextN block (index n_layers) — a trunk-shaped
        // attention layer whose norms/weights load through the standard arms
        let layers_ld = layers + t.config.n_layer_nextn
        for (l in range64(layers_ld)) {
            gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_norm.weight", t.fblob, t.rms_att_off + l * dim, dim)
            gguf_read_tensor_f32(m, bytes, "blk.{l}.{ffn_norm_name}.weight", t.fblob, t.rms_ffn_off + l * dim, dim)
            if (t.config.pre_post_norm) {  // Gemma2: post-attn + post-FFN norms (already +1-baked in the GGUF)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.post_attention_norm.weight", t.fblob, t.rms_post_att_off + l * dim, dim)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.post_ffw_norm.weight", t.fblob, t.rms_post_ffn_off + l * dim, dim)
            }
            if (t.config.qk_norm && !layer_is_recurrent(t.config, l)) {  // Qwen3/gemma4: per-head Q/K norms (head_size(l) each)
                let hs = layer_head_size(t.config, l)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_q_norm.weight", t.fblob, t.rms_q_offs[l], hs)
                if (t.wk_offs[l] >= 0l) {   // shared-KV layers (E-series) never project/norm their own K
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_k_norm.weight", t.fblob, t.rms_k_offs[l], hs)
                }
            }
            if (layer_is_recurrent(t.config, l)) {  // qwen35 deltanet fp32 smalls
                let cd = dn_conv_dim(t.config)
                let nvh = t.config.ssm_dt_rank
                gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_norm.weight", t.fblob, t.dn_norm_off + l * t.config.ssm_d_state, t.config.ssm_d_state)
                if (t.dn_grouped) {  // permute v-indexed smalls grouped->tiled (see load_dn_grouped)
                    let nk = t.config.ssm_n_group
                    let nvpk = nvh / nk
                    let hv = t.config.ssm_d_inner / nvh
                    let dc = t.config.ssm_d_conv
                    let qkch = 2l * nk * t.config.ssm_d_state   // conv qk channels stay put
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_conv1d.weight", t.fblob, t.dn_conv_off + l * dc * cd, qkch * dc)
                    for (g in range64(nk)) {
                        for (j in range64(nvpk)) {
                            let vg = g * nvpk + j
                            let vt = j * nk + g
                            gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_conv1d.weight", t.fblob, t.dn_conv_off + l * dc * cd + (qkch + vt * hv) * dc, hv * dc, (qkch + vg * hv) * dc)
                            gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_dt.bias", t.fblob, t.dn_dt_off + l * nvh + vt, 1l, vg)
                            gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_a", t.fblob, t.dn_a_off + l * nvh + vt, 1l, vg)
                            if (t.dn_ba_f32) {
                                gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_ba.weight", t.fblob, t.dn_betaf_off + l * dim * nvh + vt * dim, dim, (g * 2l * nvpk + j) * dim)
                                gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_ba.weight", t.fblob, t.dn_alphaf_off + l * dim * nvh + vt * dim, dim, ((g * 2l + 1l) * nvpk + j) * dim)
                            }
                        }
                    }
                } else {
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_conv1d.weight", t.fblob, t.dn_conv_off + l * t.config.ssm_d_conv * cd, t.config.ssm_d_conv * cd)
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_dt.bias", t.fblob, t.dn_dt_off + l * nvh, nvh)
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_a", t.fblob, t.dn_a_off + l * nvh, nvh)
                    if (t.dn_ba_f32) {  // F32-on-disk beta/alpha stay fp32
                        gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_beta.weight", t.fblob, t.dn_betaf_off + l * dim * nvh, dim * nvh)
                        gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_alpha.weight", t.fblob, t.dn_alphaf_off + l * dim * nvh, dim * nvh)
                    }
                }
            }
            if (t.config.layer_out_scale) {  // gemma4: one output scalar per layer
                gguf_read_tensor_f32(m, bytes, "blk.{l}.layer_output_scale.weight", t.fblob, t.out_scale_off + l, 1l)
            }
            if (t.config.moe_dense_shexp) {  // gemma4 MoE: custom-router scale, per-expert down scale, routed sandwich norms
                let ne = t.config.n_expert
                gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_gate_inp.scale", t.fblob, t.moe_router_scale_off + l * dim, dim)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_down_exps.scale", t.fblob, t.moe_down_scale_off + l * ne, ne)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.pre_ffw_norm_2.weight", t.fblob, t.rms_pre_ffn2_off + l * dim, dim)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.post_ffw_norm_1.weight", t.fblob, t.rms_post_ffn1_off + l * dim, dim)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.post_ffw_norm_2.weight", t.fblob, t.rms_post_ffn2_off + l * dim, dim)
            }
            if (t.config.attn_sinks) {  // gpt-oss: one sink logit per head
                gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_sinks.weight", t.fblob, t.sinks_off + l * n_heads, n_heads)
            }
            if (has_ple(t.config)) {  // gemma-4 E-series: per-layer PLE output norm (fblob)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.post_norm.weight", t.fblob, t.ple_post_off + l * dim, dim)
            }
        }
        if (t.config.v_norm) {  // the weightless V-norm's synthetic all-ones weight vector
            for (i in range64(max_head_size(t.config))) {
                t.fblob[t.rms_ones_off + i] = 1.0
            }
        }
        gguf_read_tensor_f32(m, bytes, "output_norm.weight", t.fblob, t.rms_final_off, dim)
        if (has_ple(t.config)) {  // gemma-4 E-series PLE globals: model-proj (native bf16 when on disk) + proj-norm
            let ple = t.config.n_embd_per_layer
            if (t.ple_model_bf16) {
                gguf_transcode_bf16(m, bytes, "per_layer_model_proj.weight", t.bf16blob, t.ple_model_off, dim * ple * layers)
            } else {
                gguf_read_tensor_f32(m, bytes, "per_layer_model_proj.weight", t.fblob, t.ple_model_off, dim * ple * layers)
            }
            gguf_read_tensor_f32(m, bytes, "per_layer_proj_norm.weight", t.fblob, t.ple_pnorm_off, ple)
        }
        to_log(LOG_INFO, "dasLLAMA: load stage meta+config+smalls: {get_time_usec(ts_stage) / 1000} ms\n")
        ts_stage = ref_time_ticks()
        // big 2D weights -> active precision. ggml stores them row-major [out x in] = our convention.
        var scratch : array<float>
        for (l in range64(layers_ld)) {
            let kv_l = layer_kv_dim(t.config, l)
            let qd_l = layer_qd(t.config, l)
            if (layer_is_recurrent(t.config, l)) {
                // qwen35 deltanet projections: fused [q;k;v] + z gate + per-v-head beta/alpha + out
                // (beta/alpha skip the big-weight path when they ship F32 — read into fblob above)
                if (t.dn_grouped) {
                    load_dn_grouped(m, bytes, t, l, dim, scratch)
                } else {
                    let cd = dn_conv_dim(t.config)
                    load_big(m, bytes, "blk.{l}.attn_qkv.weight", t, t.dnqkv_offs[l], dim * cd, scratch)
                    load_big(m, bytes, "blk.{l}.attn_gate.weight", t, t.dngate_offs[l], dim * t.config.ssm_d_inner, scratch)
                    if (!t.dn_ba_f32) {
                        load_big(m, bytes, "blk.{l}.ssm_beta.weight", t, t.dnbeta_offs[l], dim * t.config.ssm_dt_rank, scratch)
                        load_big(m, bytes, "blk.{l}.ssm_alpha.weight", t, t.dnalpha_offs[l], dim * t.config.ssm_dt_rank, scratch)
                    }
                    load_big(m, bytes, "blk.{l}.ssm_out.weight", t, t.dnout_offs[l], t.config.ssm_d_inner * dim, scratch)
                }
            } elif (t.config.fused_qkv) {
                // Phi3: attn_qkv rows are [q (qd) ; k (kv) ; v (kv)], ffn_up rows are [gate ; up] (hidden each).
                // src_off slices the fused tensor, preserving its Q8 blocks (no fp32 requantize); K-quant
                // slices stay superblock-aligned (offsets are dim-multiples, dim % 256 gated in detect).
                load_big(m, bytes, "blk.{l}.attn_qkv.weight", t, t.wq_offs[l], dim * qd_l, scratch, 0l, fmt_at(t.wq_fmt, l))
                load_big(m, bytes, "blk.{l}.attn_qkv.weight", t, t.wk_offs[l], dim * kv_l, scratch, dim * qd_l, fmt_at(t.wk_fmt, l))
                load_big(m, bytes, "blk.{l}.attn_qkv.weight", t, t.wv_offs[l], dim * kv_l, scratch, dim * qd_l + dim * kv_l, fmt_at(t.wv_fmt, l))
            } else {
                // q_gated (qwen35): attn_q ships 2x-wide — per head [q | gate]
                load_big(m, bytes, "blk.{l}.attn_q.weight", t, t.wq_offs[l], dim * qd_l * (t.config.q_gated ? 2l : 1l), scratch, 0l, fmt_at(t.wq_fmt, l))
                if (t.wk_offs[l] >= 0l) {   // absent on E-series shared-KV layers (reuse the source layer's K)
                    load_big(m, bytes, "blk.{l}.attn_k.weight", t, t.wk_offs[l], dim * kv_l, scratch, 0l, fmt_at(t.wk_fmt, l))
                }
                if (t.wv_offs[l] >= 0l) {   // absent on gemma4 global (V-from-K) + E-series shared-KV layers
                    load_big(m, bytes, "blk.{l}.attn_v.weight", t, t.wv_offs[l], dim * kv_l, scratch, 0l, fmt_at(t.wv_fmt, l))
                }
            }
            if (t.wo_offs[l] >= 0l) {   // absent on deltanet layers (ssm_out is their out projection)
                load_big(m, bytes, "blk.{l}.attn_output.weight", t, t.wo_offs[l], qd_l * dim, scratch, 0l, fmt_at(t.wo_fmt, l))
            }
            if (t.config.n_expert > 0l && l >= t.config.n_layer_dense_lead) {
                // MoE: fp32 router (+ shared-expert gate vector) -> fblob; the 3D expert stacks are
                // expert-major on disk, so ONE contiguous load lands every expert's 2D matrix at its
                // we*_off + (l*n_expert + e) * (dim*n_ff_exp) slot
                let ne = t.config.n_expert
                let ege = dim * t.config.n_ff_exp
                gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_gate_inp.weight", t.fblob, t.router_off + l * ne * dim, ne * dim)
                if (t.config.moe_router_bias) {
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_gate_inp.bias", t.fblob, t.router_b_off + l * ne, ne)
                }
                if (t.config.moe_exp_probs) {   // DeepSeek/glm4moe selection bias (f32, one per expert)
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.exp_probs_b.bias", t.fblob, t.exp_probs_off + l * ne, ne)
                }
                if (t.experts_mx4) {
                    // native MXFP4: raw nibble + scale bits straight into the planes (no dequant)
                    let mo1 = t.we1_offs[l]
                    let mo2 = t.we2_offs[l]
                    let mo3 = t.we3_offs[l]
                    gguf_transcode_mx4(m, bytes, "blk.{l}.ffn_gate_exps.weight", t.mxq, t.mxs, mo1 / 2l, mo1 / 32l, ne * ege)
                    gguf_transcode_mx4(m, bytes, "blk.{l}.ffn_down_exps.weight", t.mxq, t.mxs, mo2 / 2l, mo2 / 32l, ne * ege)
                    gguf_transcode_mx4(m, bytes, "blk.{l}.ffn_up_exps.weight", t.mxq, t.mxs, mo3 / 2l, mo3 / 32l, ne * ege)
                } elif (t.config.moe_dense_shexp) {
                    // gemma4: experts ship FUSED as ffn_gate_up_exps {dim, 2*nfe, ne} — expert-major,
                    // each [2*nfe x dim] with rows [0..nfe)=gate, [nfe..2*nfe)=up. Split per expert
                    // into the separate we1(gate)/we3(up) stacks the routed GEMM seam expects.
                    for (e in range64(ne)) {
                        let dst = e * ege
                        let src = e * 2l * ege
                        load_big(m, bytes, "blk.{l}.ffn_gate_up_exps.weight", t, t.we1_offs[l] + dst, ege, scratch, src, fmt_at(t.we1_fmt, l))
                        load_big(m, bytes, "blk.{l}.ffn_gate_up_exps.weight", t, t.we3_offs[l] + dst, ege, scratch, src + ege, fmt_at(t.we3_fmt, l))
                    }
                    load_big(m, bytes, "blk.{l}.ffn_down_exps.weight", t, t.we2_offs[l], ne * ege, scratch, 0l, fmt_at(t.we2_fmt, l))
                } else {
                    load_big(m, bytes, "blk.{l}.ffn_gate_exps.weight", t, t.we1_offs[l], ne * ege, scratch, 0l, fmt_at(t.we1_fmt, l))
                    load_big(m, bytes, "blk.{l}.ffn_down_exps.weight", t, t.we2_offs[l], ne * ege, scratch, 0l, fmt_at(t.we2_fmt, l))
                    load_big(m, bytes, "blk.{l}.ffn_up_exps.weight", t, t.we3_offs[l], ne * ege, scratch, 0l, fmt_at(t.we3_fmt, l))
                }
                if (t.config.moe_exps_bias) {  // per-expert bias stacks, expert-major on disk like the weights
                    let nfe = t.config.n_ff_exp
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_gate_exps.bias", t.fblob, t.web1_off + l * ne * nfe, ne * nfe)
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_down_exps.bias", t.fblob, t.web2_off + l * ne * dim, ne * dim)
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_up_exps.bias", t.fblob, t.web3_off + l * ne * nfe, ne * nfe)
                }
                if (t.config.n_ff_shexp > 0l) {
                    let she = dim * t.config.n_ff_shexp
                    if (t.config.moe_shexp_gated) {   // glm4moe's shared expert is ungated — no gate vector on disk
                        gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_gate_inp_shexp.weight", t.fblob, t.shexp_gate_off + l * dim, dim)
                    }
                    load_big(m, bytes, "blk.{l}.ffn_gate_shexp.weight", t, t.wsh1_off + l * she, she, scratch)
                    load_big(m, bytes, "blk.{l}.ffn_down_shexp.weight", t, t.wsh2_off + l * she, she, scratch)
                    load_big(m, bytes, "blk.{l}.ffn_up_shexp.weight", t, t.wsh3_off + l * she, she, scratch)
                }
                if (t.config.moe_dense_shexp) {  // gemma4: the DENSE parallel shared expert (plain ffn gate/up/down)
                    load_big(m, bytes, "blk.{l}.ffn_gate.weight", t, t.w1_offs[l], dim * hidden, scratch, 0l, fmt_at(t.w1_fmt, l))
                    load_big(m, bytes, "blk.{l}.ffn_up.weight", t, t.w3_offs[l], dim * hidden, scratch, 0l, fmt_at(t.w3_fmt, l))
                    load_big(m, bytes, "blk.{l}.ffn_down.weight", t, t.w2_offs[l], hidden * dim, scratch, 0l, fmt_at(t.w2_fmt, l))
                }
            } elif (t.config.fused_qkv) {
                load_big(m, bytes, "blk.{l}.ffn_up.weight", t, t.w1_offs[l], dim * hidden, scratch, 0l, fmt_at(t.w1_fmt, l))
                load_big(m, bytes, "blk.{l}.ffn_up.weight", t, t.w3_offs[l], dim * hidden, scratch, dim * hidden, fmt_at(t.w3_fmt, l))
                load_big(m, bytes, "blk.{l}.ffn_down.weight", t, t.w2_offs[l], hidden * dim, scratch, 0l, fmt_at(t.w2_fmt, l))
            } else {
                let h = layer_hidden(t, l)   // gemma-4 E2B varies this per layer
                load_big(m, bytes, "blk.{l}.ffn_gate.weight", t, t.w1_offs[l], dim * h, scratch, 0l, fmt_at(t.w1_fmt, l))
                load_big(m, bytes, "blk.{l}.ffn_up.weight", t, t.w3_offs[l], dim * h, scratch, 0l, fmt_at(t.w3_fmt, l))
                load_big(m, bytes, "blk.{l}.ffn_down.weight", t, t.w2_offs[l], h * dim, scratch, 0l, fmt_at(t.w2_fmt, l))
            }
            if (has_ple(t.config)) {  // gemma-4 E-series: per-layer PLE gate (dim->ple) + proj (ple->dim), Q8
                let ple = t.config.n_embd_per_layer
                load_big(m, bytes, "blk.{l}.inp_gate.weight", t, t.ple_gate_off + l * dim * ple, dim * ple, scratch)
                load_big(m, bytes, "blk.{l}.proj.weight", t, t.ple_proj_off + l * ple * dim, ple * dim, scratch)
            }
        }
        if (t.config.n_layer_nextn > 0l) {   // MTP/NextN extras (block index n_layers = `layers`)
            load_big(m, bytes, "blk.{layers}.nextn.eh_proj.weight", t, t.mtp_ehproj_off, 2l * dim * dim, scratch, 0l, t.mtp_ehproj_fmt)
            gguf_read_tensor_f32(m, bytes, "blk.{layers}.nextn.enorm.weight", t.fblob, t.mtp_enorm_off, dim)
            gguf_read_tensor_f32(m, bytes, "blk.{layers}.nextn.hnorm.weight", t.fblob, t.mtp_hnorm_off, dim)
            if (gguf_find_tensor(m, "blk.{layers}.nextn.shared_head_norm.weight") >= 0) {
                gguf_read_tensor_f32(m, bytes, "blk.{layers}.nextn.shared_head_norm.weight", t.fblob, t.mtp_headnorm_off, dim)
            } else {
                t.mtp_headnorm_off = -1l   // reads fall back to output_norm (rms_final_off)
            }
            // optional per-head embedding/classifier: no known qwen35 file ships them — fail closed
            if (gguf_find_tensor(m, "blk.{layers}.nextn.embed_tokens.weight") >= 0
                    || gguf_find_tensor(m, "blk.{layers}.nextn.shared_head_head.weight") >= 0) {
                panic("dasLLAMA: NextN per-head embed_tokens/shared_head_head are not supported yet")
            }
        }
        if (has_ple(t.config)) {  // the per-layer token-embedding table [vocab x (ple*layers)], gathered per token, never repacked
            load_big(m, bytes, "per_layer_token_embd.weight", t, t.ple_emb_off, vocab * t.config.n_embd_per_layer * layers, scratch, 0l, t.ple_emb_fmt)
        }
        if (!t.config.shared_weights) {
            load_big(m, bytes, "output.weight", t, t.wcls_off, vocab * dim, scratch, 0l, t.wcls_fmt)
        } elif (t.cls_q8) {
            // tied Q8: classifier copy (repacked below) + the linear copy embedding rows read
            gguf_transcode_q8_0(m, bytes, "token_embd.weight", t.qblob, t.qscales, t.wcls_off, t.wcls_off / 32l, vocab * dim)
            gguf_transcode_q8_0(m, bytes, "token_embd.weight", t.qblob, t.qscales, t.emb_q8_off, t.emb_q8_off / 32l, vocab * dim)
        } elif (cls_kq(t)) {
            // tied K-quant: ONE plane copy serves the classifier matmul and embedding row reads
            // (the kq planes stay in disk order — no repack, so no second linear copy)
            load_big(m, bytes, "token_embd.weight", t, t.wcls_off, vocab * dim, scratch, 0l, t.emb_fmt)
        }
        // Qwen2 Q/K/V projection biases — small fp32 tensors, kept dense
        if (t.config.attn_qkv_bias) {
            t.bq |> resize(layers * qd)
            t.bk |> resize(layers * kv)
            t.bv |> resize(layers * kv)
            for (l in range64(layers)) {
                gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_q.bias", t.bq, l * qd, qd)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_k.bias", t.bk, l * kv, kv)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_v.bias", t.bv, l * kv, kv)
            }
        }
        // gpt-oss attention output-projection bias (dim per layer)
        if (t.config.attn_out_bias) {
            t.bo |> resize(layers * dim)
            for (l in range64(layers)) {
                gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_output.bias", t.bo, l * dim, dim)
            }
        }
        delete scratch
        to_log(LOG_INFO, "dasLLAMA: load stage weights: {get_time_usec(ts_stage) / 1000} ms\n")
        ts_stage = ref_time_ticks()
        // arm64: switch to the laneq backend and repack the just-loaded Q8 weights into its interleaved
        // layout. Returns false off-ARM (portable stays, weights stay row-major).
        if (mode == QuantMode.q8 && select_matmul_backend_for_load()) {
            repack_q8_weights(t)
            if (t.experts_mx4) {
                repack_mx4_stacks(t)
            }
            if (t.kquant_native && kernel_backend_has_kq()) {
                repack_kq_weights(t)
                t.kq_repacked = true
                t.kq_repack_mr4 = active_kq_layout_mr(4)
                t.kq_repack_mr5 = active_kq_layout_mr(5)
                t.kq_repack_mr6 = active_kq_layout_mr(6)
                t.kq_repack_mr40 = active_kq_layout_mr(40)
                to_log(LOG_INFO, "dasLLAMA: K-quant planes repacked to the '{active_kernel_backend()}' layouts (k4 grp{t.kq_repack_mr4}, k5 grp{t.kq_repack_mr5}, k6 grp{t.kq_repack_mr6}, q40 grp{t.kq_repack_mr40})\n")
            }
        }
        to_log(LOG_INFO, "dasLLAMA: load stage repack: {get_time_usec(ts_stage) / 1000} ms\n")
        ts_stage = ref_time_ticks()
        if (mode == QuantMode.q8 && g_wscale_f16) {
            wscale_convert_f16(t)
        }
        to_log(LOG_INFO, "dasLLAMA: load stage wscale: {get_time_usec(ts_stage) / 1000} ms, total {get_time_usec(ts_load) / 1000} ms\n")
        moe_gpu_upload_resident(t)
    }
}

//! Quantize a fp32-loaded model's big weights to Q8_0 in place and switch forward onto the Q8 path.
//! Requires the fp32 representation (wblob) — i.e. a model loaded with QuantMode.fp32. Keeps wblob
//! so the same model can be re-quantized to another precision for an A/B comparison.
def quantize_weights(var t : Model) {
    let n = int64(length(t.wblob))
    var qt <- quantize_q8_0(t.wblob, n)
    t.qblob <- qt.q
    t.qscales <- qt.scales
    t.quant = QuantMode.q8
    // same backend-for-load select + in-place repack as the load_gguf Q8 path (no-op off-ARM)
    if (select_matmul_backend_for_load()) {
        repack_q8_weights(t)
    }
    if (g_wscale_f16) {
        wscale_convert_f16(t)
    }
    moe_gpu_upload_resident(t)
}

//! Quantize a fp32-loaded model's big weights to Q4_0 in place and switch forward onto the Q4 path.
//! Same fp32-required / wblob-kept contract as quantize_weights.
def quantize_weights_q4(var t : Model) {
    let n = int64(length(t.wblob))
    var qt <- quantize_q4_0(t.wblob, n)
    t.q4blob <- qt.q
    t.q4scales <- qt.scales
    t.quant = QuantMode.q4_0
}

//! Resident memory footprint of a loaded model, in bytes, broken down by region. aux_fp32 is the
//! embedding + norms (always kept); weight_fp32 is the dense big weights (only in fp32 mode — a
//! quantized model should show this as 0). The detector for the memory-honest refactor.
struct MemFootprint {
    aux_fp32_bytes : int64
    weight_fp32_bytes : int64
    q8_bytes : int64
    q8_scale_bytes : int64
    q4_bytes : int64
    q4_scale_bytes : int64
    mx4_bytes : int64
    mx4_scale_bytes : int64
    kq_bytes : int64         // native K-quant planes: quant payloads (k4q+k5q+k6q+q40q)
    kq_scale_bytes : int64   // ... and their scale planes (k4s+k5s+k6s+q40s)
    total : int64
}

def footprint(t : Model) : MemFootprint {
    // long_length throughout: the big blobs exceed INT_MAX elements on 20B-class models (gpt-oss
    // qblob/mxq), where int length() panics interpreted and silently truncates under the JIT
    let aux = long_length(t.fblob) * 4l
    let wf = long_length(t.wblob) * 4l
    let q8 = long_length(t.qblob)                 // int8 -> 1 byte each
    let q8s = long_length(t.qscales) * 4l + long_length(t.qscales16) * 2l
    let q4 = long_length(t.q4blob)                // uint8 (two nibbles) -> 1 byte each
    let q4s = long_length(t.q4scales) * 4l
    let mx4 = long_length(t.mxq)                  // uint8 (two nibbles) -> 1 byte each
    let mx4s = long_length(t.mxs)                 // raw E8M0 -> 1 byte per 32-block
    let kq = long_length(t.k4q) + long_length(t.k5q) + long_length(t.k6q) + long_length(t.q40q)
    let kqs = long_length(t.k4s) + long_length(t.k5s) + long_length(t.k6s) + long_length(t.q40s)
    return MemFootprint(aux_fp32_bytes = aux, weight_fp32_bytes = wf, q8_bytes = q8, q8_scale_bytes = q8s,
                        q4_bytes = q4, q4_scale_bytes = q4s, mx4_bytes = mx4, mx4_scale_bytes = mx4s,
                        kq_bytes = kq, kq_scale_bytes = kqs,
                        total = aux + wf + q8 + q8s + q4 + q4s + mx4 + mx4s + kq + kqs)
}

//! A caller-owned paged KV-cache pool (`create_kv_pool_`). Pages chunk POSITIONS; ONE allocation
//! unit = a page GROUP (the {K,V}-pages of every owning layer for the same `page_rows` span), so a
//! session's block table is one int per span. Keep the pool alive as long as its sessions.
struct KVPool {
    page_rows : int64            // P: positions per page (fixed at create)
    kdt : KVDtype                // KV codec of every session in this pool
    vdt : KVDtype
    nblobs : int64               // owning layers (shared-KV layers alias — see blob_of)
    capacity_groups : int64      // groups each blob currently holds (grows geometrically)
    blob_of : array<int64>       // layer -> owning blob index (shared layers -> their source's)
    krow_bytes : array<int64>    // per blob: one position row's K bytes
    vrow_bytes : array<int64>
    kblobs : array<array<uint8>> // per blob: capacity_groups pages of page_rows rows each
    vblobs : array<array<uint8>>
    free_groups : array<int>     // LIFO free list
    refcnt : array<int>          // per group; 0/1 today (the prefix cache bumps it later)
}

//! Per-step scratch buffers and the KV cache (allocated once, reused every token).
struct Session {
    // Stable identity for device-side mirrors (Metal decode KV mirror keys on it): monotonic per
    // context, assigned once in make_run_state. Address keying is unsound — schedulers churn
    // same-geometry sessions (ABA) and Stream erase memmoves Sessions. 0 = never assigned.
    uid : uint64
    n_past : int64            // absolute position of the next token to eval (0 at session start); advanced by eval()
    rng : int4                // sampling RNG state (seeded in make_run_state; reseed via set_seed)
    recent : array<int64>     // recently generated tokens, for the repetition penalty
    sample_scratch : array<float>  // vocab-sized scratch for top-k selection
    // last-generation timing, filled by the streaming generate() and read by stats()
    stat_n_prefill : int64    // prompt tokens prefilled
    stat_prefill_us : int64   // wall time to prefill the prompt (microseconds)
    stat_n_gen : int64        // tokens generated
    stat_gen_us : int64       // wall time of the generation loop (microseconds)
    stat_ttft_us : int64      // time-to-first-token: prefill + first forward (microseconds)
    x : array<float>          // residual stream (dim)
    xb : array<float>         // scratch (dim): rmsnorm out, attention out
    xb2 : array<float>        // scratch (dim): attention output projection
    hb : array<float>         // FFN hidden (hidden_dim)
    hb2 : array<float>        // FFN gate (hidden_dim)
    q : array<float>          // query (dim)
    k : array<float>          // key, this position (kv_dim)
    v : array<float>          // value, this position (kv_dim)
    att : array<float>        // attention scores, one seq_len row PER HEAD (head h at h*seq_len) —
                              // the per-token forward() path threads over heads past the decode threshold
    att_b : array<float>      // batched prefill attention scores, one (start_pos+npos)-wide row PER HEAD
                              // so the head loop threads with no shared scratch (sized in forward_prefill)
    logits : array<float>     // output logits (vocab_size)
    xq : array<int8>          // activation quantized to Q8 (sized max(dim, hidden_dim)) for the Q8·Q8 kernel
    xs : array<float>         // per-block activation scales (sized max(dim, hidden_dim) / 32)
    // q8_0-K decode query, quantized ONCE per token after RoPE (empty unless kv_dtype_k == q8_0;
    // batched decode resizes to nrows × qd). Dedicated planes: xq/xs hold the LIVE input quants
    // while the fused chain's stage-0 q epilogues fill these
    attq : array<int8>        // quantized query rows (qd)
    atts : array<float>       // ... and their per-32-block scales (qd / 32)
    // fused decode-chain scratch (team_parallel_stages): the FFN chain's stage-0 workers requant
    // their activated-hidden rows while OTHER chunks still read the input quants in xq/xs, so the
    // requant needs its own buffers; chain_stages is the reused stage-def array (no per-layer alloc)
    xq2 : array<int8>         // activated-hidden requant quants (ffn width)
    xs2 : array<float>        // ... and their scales (ffn width / 32)
    // K-quant activation block sums: sum of each 16 quants of xq / xq2 (the K-quant dot's offset
    // term is o * Σq_a per weight sub-block). Filled by quantize_q8_0_bs_into alongside xq/xs
    // on the kq-native paths; sized always (~KB)
    xbs : array<int>          // per-16 sums of xq (sized max(dim, hidden_dim) / 16)
    xbs2 : array<int>         // per-16 sums of xq2 (ffn width / 16)
    kxq : array<int8>         // Q8_K-form activation quants (kq v2 — every kq weight reads these; sized maxn)
    kxs : array<float>        // Q8_K per-256 activation scales (maxn / 256)
    kxbs : array<int>         // Q8_K per-16 activation sums (maxn / 16)
    chain_stages : array<int3>  // stage defs, int3(range_begin, range_end, num_chunks)
    // flash-decode attention partials (fused chain, ctx > FLASH_DECODE_SLICE): per (head, slice)
    // running max / exp-sum / unnormalized AV accumulator, combined per head in the chain's
    // combine stage. Sized lazily on first deep-ctx use (n_heads x nsl [x head_size])
    fl_m : array<float>
    fl_l : array<float>
    fl_o : array<float>
    // KV cache — codec-encoded byte blobs (dtype fixed per session at create). Layout is ragged
    // per layer: layer l's slab starts at byte kv_bprefix_*[l] * seq_len. Shared-KV layers alias
    // their source layer's slab (same prefix value).
    kv_dtype_k : KVDtype      // key codec, snapshotted from the module knob in make_run_state
    kv_dtype_v : KVDtype      // value codec (independent of K by design)
    kv_signs : array<float>   // tq4 rotation signs (±1, max head_size; smaller heads use a prefix)
    key_cache : array<uint8>
    value_cache : array<uint8>
    kv_bprefix_k : array<int64>  // per-layer row-BYTE prefix (seq_len-independent)
    kv_bprefix_v : array<int64>
    kv_runs : array<KVRun>       // per-layer physical-run scratch (kv_runs fills it; flat = 1 entry)
    // paged sessions (make_run_state_paged): the cache lives in a caller-owned pool and the flat
    // slabs above stay EMPTY; block_table[i] = the page group holding positions [i*P, (i+1)*P),
    // grown on demand by kv_ensure. Flat sessions: kv_pool == null, block_table empty.
    @do_not_delete kv_pool : KVPool?   // borrowed — the pool outlives its sessions
    block_table : array<int>
    // batched prefill scratch — empty until forward_prefill sizes them to npos × ... (see forward_prefill)
    x_b : array<float>        // residual stream (npos x dim)
    xb_b : array<float>       // rmsnorm / attention out (npos x dim)
    xb2_b : array<float>      // attention output projection (npos x dim)
    q_b : array<float>        // queries (npos x dim)
    k_b : array<float>        // keys before cache store (npos x kv_dim)
    v_b : array<float>        // values before cache store (npos x kv_dim)
    hb_b : array<float>       // FFN hidden (npos x hidden_dim)
    hb2_b : array<float>      // FFN gate (npos x hidden_dim)
    xqb : array<int8>         // batched activation Q8 quants (npos x max(dim, hidden_dim))
    xsb : array<float>        // batched activation Q8 scales (npos x max(dim, hidden_dim) / 32)
    xbsb : array<int>         // batched per-16 activation sums (npos x maxn / 16) — the kq batch tile's offset term
    kxqb : array<int8>        // batched Q8_K-form quants (kq v2 MoE gather source; lazily sized npos x dim)
    kxsb : array<float>       // batched Q8_K per-256 scales (npos x dim / 256)
    kxbsb : array<int>        // batched Q8_K per-16 sums (npos x dim / 16)
    // flash-attention per-head packing scratch (empty unless AttnPrefillMode.flash; sized in
    // forward_prefill to n_heads × tile, so each head's tile work is contention-free when threaded)
    fa_q : array<float>       // packed Q tile [n_heads × QT × head_size]
    fa_k : array<float>       // packed Kᵀ tile [n_heads × head_size × KV]
    fa_v : array<float>       // packed V tile  [n_heads × KV × head_size]
    fa_kq : array<float>      // score / prob tile [n_heads × QT × KV]
    fa_vkq : array<float>     // output accumulator [n_heads × QT × head_size]
    fa_tmp : array<float>     // per-head one-row dequant staging for the Kᵀ pack [n_heads × head_size]
    // RoPE cos/sin table (empty until forward_prefill builds it, sized npos x head_size/2). angle
    // = pos*freq is identical across heads AND layers, precomputed once. Kept LOCAL (run-state, not
    // a module global) so jobque fork-context cloning stays cheap.
    rope_cos : array<float>
    rope_sin : array<float>
    // second table for sliding layers when the arch ropes them differently (has_dual_rope — Gemma3:
    // θ=10k unscaled local vs θ=1M linear-scaled global); empty otherwise
    rope_cos_swa : array<float>
    rope_sin_swa : array<float>
    // MoE scratch (sized in make_run_state when n_expert > 0; empty for dense arches)
    moe_logits : array<float>   // router logits, then probs (n_expert)
    moe_w : array<float>        // the selected top-k weights (n_expert_used)
    moe_idx : array<int64>      // the selected top-k expert ids (n_expert_used)
    moe_acc : array<float>      // weighted expert-output accumulator (dim)
    moe_ao : array<float>       // gemma4 MoE: attn_out held while its two branches read three norms of it (dim; moe_dense_shexp only)
    // decode dispatch-fuse scratch: all k routed experts' projections run as region-list GEMVs
    moe_offs : array<int64>     // packed (weight, activation) element-offset pairs (2 per region)
    moe_boffs : array<int64>    // per-region fblob bias offsets for the biased groupn forms (k)
    moe_gu : array<float>       // fused gate outputs (k x n_ff_exp)
    moe_gu2 : array<float>      // fused up outputs (k x n_ff_exp)
    moe_dn : array<float>       // fused down outputs (k x dim)
    moe_dq : array<int8>        // the k activated-hidden rows requantized (k x n_ff_exp)
    moe_ds : array<float>       // ... and their scales (k x n_ff_exp / 32)
    // the layer's rmsnormed activation quantized ONCE and reused across every expert's gate/up
    // matmul — dedicated buffers because the down-projections quantize s.hb into the shared xq/xs
    moe_xq : array<int8>        // xb's Q8 quants (dim)
    moe_xs : array<float>       // xb's per-block scales (dim / 32)
    // K-quant activation block sums for the expert dispatch (the moe twins of xbs/xbs2, filled
    // by the bs quantizers only on layers with kq-tagged expert stacks; sized always — ~KB)
    moe_dbs : array<int>        // per-16 sums of moe_dq (k x n_ff_exp / 16)
    // grouped MoE prefill scratch — sized per prefill by ffn_moe_prefill_grouped; empty for dense
    // arches, decode, and the per-position reference path
    moe_rlog : array<float>     // batched router logits (npos x n_expert)
    moe_idx_b : array<int64>    // per-position selected expert ids (npos x k)
    moe_w_b : array<float>      // per-position selected weights (npos x k)
    moe_gl_b : array<float>     // per-position shared-expert sigmoid gates (npos; shexp arches only)
    moe_cnt : array<int64>      // per-expert bucket cursor, run END after the CSR fill (n_expert)
    moe_bkt : array<int64>      // (position*k + slot) entries grouped by expert (npos x k)
    moe_inv : array<uint>       // grid -> bucket row map (bkt's inverse; GPU combine arm only)
    moe_gxq : array<int8>       // one bucket's gathered activation quants (npos x dim)
    moe_gxs : array<float>      // one bucket's gathered activation scales (npos x dim/32)
    moe_gxbs : array<int>       // ... and their per-16 sums (npos x dim/16; kq-expert layers only)
    moe_hbs : array<int>        // bucket hidden requant per-16 sums (npos x nfx/16; kq-expert layers only)
    moe_ord : array<int2>       // fused-prefill LPT region order (x = expert, y = bucket cnt; capacity ne)
    // fused-prefill region triples (woff, row0, cnt) + per-expert bias offsets, one set per
    // projection — cleared and refilled per layer, capacity persists (no per-layer alloc)
    moe_offs1 : array<int64>    // gate regions (3 per touched expert)
    moe_offs3 : array<int64>    // up regions
    moe_offs2 : array<int64>    // down regions
    moe_boffs1 : array<int64>   // gate bias offsets (moe_exps_bias arches)
    moe_boffs3 : array<int64>   // up bias offsets
    moe_boffs2 : array<int64>   // down bias offsets
    moe_ghb : array<float>      // bucket gate outputs (npos x max(n_ff_exp, n_ff_shexp))
    moe_ghb2 : array<float>     // bucket up outputs (same shape)
    moe_hq : array<int8>        // bucket hidden requant quants (npos x max(n_ff_exp, n_ff_shexp))
    moe_hs : array<float>       // bucket hidden requant scales (same / 32)
    moe_gout : array<float>     // bucket (then shared-expert) down-projection output (npos x dim)
    moe_eout : array<float>     // routed expert outputs parked by (position, slot) (npos x k x dim)
    ffn_offs : array<int64>     // dense/shexp gate+up fuse: 2 region (weight, activation) offset pairs
    ffn_gu : array<float>       // dense/shexp gate+up fuse output — gate half then up half (2 x max_ffn_width)
    mx4_wq : array<int8>        // grouped-prefill expansion of ONE native-MXFP4 expert to exact Q8 (dim x n_ff_exp)
    mx4_ws : array<float>       // ... and its per-block scales (same / 32); see expand_mx4_region_q8
    // gemma-4 E-series per-layer embeddings (PLE) scratch — empty unless config.n_embd_per_layer > 0.
    // ple_inp holds one (position, layer) side vector per row, POSITION-major: (p, l, i) at
    // p*(n_layers*ple) + l*ple + i. Sized to 1 position in make_run_state, npos in forward_prefill.
    ple_inp : array<float>      // the built per-(position, layer) side input (npos x n_layers x ple)
    ple_proj : array<float>     // model-proj scratch for the pre-step (npos x n_layers x ple)
    ple_gate : array<float>     // per-(position) gate vector for the layer block (npos x ple)
    // qwen35 deltanet recurrent state (persists across tokens; zeroed at position 0) + scratch.
    // Empty unless config.recr_mask != 0. State is forward-only — dn_pos guards against rewind.
    dn_pos : int64                 // the next position the deltanet stack expects
    dn_conv_state : array<float>   // per recurrent layer: (d_conv-1) history taps per channel, channel-major
    dn_state : array<float>        // per recurrent layer: S — dt_rank heads x (d_state x d_state) f32
    dn_conv_prefix : array<int64>  // per-layer element offsets into the two above; -1 = attention layer
    dn_state_prefix : array<int64>
    dn_qkv : array<float>          // fused [q;k;v] projection out (conv_dim)
    dn_z : array<float>            // z gate projection (d_inner)
    dn_beta : array<float>         // per-v-head β (dt_rank)
    dn_g : array<float>            // per-v-head log-decay g (dt_rank)
    dn_conv_out : array<float>     // conv + SiLU output (conv_dim)
    dn_o : array<float>            // delta-rule output (d_inner)
    dn_sk : array<float>           // per-head kᵀS readout / delta scratch (dt_rank x d_state)
    // gated attention (config.q_gated): fused [q | gate] projection + the deinterleaved gate half
    dn_qg : array<float>           // decode: 2 x qd
    dn_qgate : array<float>        // decode: qd
    // prefill twins (sized per prefill by forward_prefill_alloc)
    dn_qkv_b : array<float>
    dn_z_b : array<float>
    dn_beta_b : array<float>
    dn_g_b : array<float>
    dn_o_b : array<float>
    dn_qg_b : array<float>
    dn_qgate_b : array<float>
    // chunked deltanet prefill scratch (DnPrefillMode.chunked only)
    dn_conv_b : array<float>       // batched conv + SiLU output (npos x conv_dim)
    dn_wtap : array<float>         // conv weights transposed tap-major for the batched form (d_conv x conv_dim)
    dn_cws : array<float>          // per-v-head chunk workspace slabs (dt_rank x dn_cws_head_size)
    dn_tail : array<float>         // GPU deltanet chain: last taps qkv rows back (conv-history store)
    // MTP/NextN self-spec state (empty unless config.n_layer_nextn > 0). mtp_h is the trunk's
    // post-output-norm hidden of the LAST evaluated position (llama.cpp's t_h_nextn handoff);
    // mtp_h_pos1 is that position PLUS ONE (0 = cold, keeps Session zero-init-safe).
    mtp_h : array<float>           // h_nextn (dim)
    mtp_h_pos1 : int64
    mtp_cat : array<float>         // eh_proj input [enorm(embed(tok)) ; hnorm(h)] (2*dim)
    mtp_cat_b : array<float>       // prompt-warm eh_proj concat image ((npos-1) x 2*dim, scratch-sized per warm)
    mtp_xb_save : array<float>     // prompt-warm x_b preservation (the warm's eh_proj output rides x_b for the arch blocks; consumers like mtp_verify_row0 / embed_forward need the trunk hiddens back)
    mtp_logits : array<float>      // verify-batch row-0 logits (vocab; the draft check reads its argmax)
    mtp_vbatch : array<int64>      // the 2-row verify batch [tok, draft] (persistent — no per-step alloc)
    mtp_drafted : int64            // session-lifetime telemetry: drafts attempted / accepted
    mtp_accepted : int64
    // deltanet rollback for a rejected verify batch (sized lazily on first snapshot)
    mtp_snap_conv : array<float>
    mtp_snap_state : array<float>
    mtp_snap_pos : int64
}

// Block-codec KV (q8_0/tq4) needs whole 32-blocks everywhere the cache is sliced: rows (kv_dim)
// and head slots (head_size). Real GGUF targets pass (64/128/256); llama2.c stories15M (48) is
// rejected by design. tq4 additionally rotates whole heads, so head_size must be a power of two.
def private kv_validate_q8(c : Config; kdt, vdt : KVDtype) {
    let tq4 = kdt == KVDtype.tq4 || vdt == KVDtype.tq4
    if (kdt != KVDtype.q8_0 && vdt != KVDtype.q8_0 && !tq4) {
        return
    }
    for (l in range64(c.n_layers + c.n_layer_nextn)) {   // + the MTP/NextN block's KV slab
        let kvd = layer_kv_dim(c, l)
        let hs = layer_head_size(c, l)
        if (kvd % 32l != 0l || hs % 32l != 0l) {
            panic("block-codec KV cache needs layer kv_dim and head_size to be multiples of 32 — layer {l} has kv_dim={kvd}, head_size={hs}")
        }
        if (tq4 && (hs & (hs - 1l)) != 0l) {
            panic("tq4 KV cache needs a power-of-two head_size for the FWHT — layer {l} has head_size={hs}")
        }
    }
}

// Session-uid source (see Session.uid). Per-context counter: fork-worker contexts clone module
// globals, so uniqueness holds within a context — the device mirrors that key on uid live on the
// main context only, which is the only place sessions meet them.
var private g_session_uid : uint64

//! Allocate the run state sized to a model. `kdt`/`vdt` pick the KV-cache codec for THIS session
//! (no process-global state — sessions with different codecs coexist, models run in parallel).
//! `alloc_cache = false` skips the flat cache slabs (paged sessions and cache-less scratch).
def make_run_state(c : Config; kdt : KVDtype = KVDtype.f32; vdt : KVDtype = KVDtype.f32; alloc_cache : bool = true) : Session {
    kv_validate_q8(c, kdt, vdt)
    var s : Session
    g_session_uid++
    s.uid = g_session_uid
    // qd = n_heads * head_size, the query/attention-output width. == dim for the llama family, but
    // Gemma2 has qd (2048) != dim (2304), so q and xb size to max(dim, qd). Heterogeneous arches
    // (gemma4) size to the larger layer class; the KV cache packs each layer's own kv_dim.
    let qd = max_qd(c)
    let kvd = max_kv_dim(c)
    let xdim = max(c.dim, qd)
    let ffw = max_ffn_width(c)   // dense hidden, or the widest expert / shared-expert (MoE)
    s.x |> resize(c.dim)
    s.xb |> resize(xdim)
    s.xb2 |> resize(c.dim)
    s.hb |> resize(ffw)
    s.hb2 |> resize(ffw)
    s.q |> resize(qd)
    s.k |> resize(kvd)
    s.v |> resize(kvd)
    s.att |> resize(c.n_heads * c.seq_len)   // one row per head — decode attention threads over heads
    // logits carry page-rounded CAPACITY (length stays vocab_size): the Metal drivers wrap this
    // storage in a bytesNoCopy MTLBuffer (zero-copy logits), whose length must be a 16KB-page
    // multiple — the reserve keeps that rounded length inside the array's own allocation
    s.logits |> reserve(int(((c.vocab_size * 4l + 16383l) & ~16383l) / 4l))
    s.logits |> resize(c.vocab_size)
    if (c.n_expert > 0l) {   // MoE routing scratch
        s.moe_logits |> resize(c.n_expert)
        s.moe_w |> resize(c.n_expert_used)
        s.moe_idx |> resize(c.n_expert_used)
        s.moe_acc |> resize(c.dim)
        if (c.moe_dense_shexp) {   // gemma4: attn_out survives both branches' three norms
            s.moe_ao |> resize(c.dim)
        }
        s.moe_xq |> resize(c.dim)
        s.moe_xs |> resize(c.dim / 32l)
        s.moe_dbs |> resize(c.n_expert_used * c.n_ff_exp / 16l)
        s.moe_offs |> resize(2l * c.n_expert_used)
        s.moe_boffs |> resize(c.n_expert_used)
        s.moe_gu |> resize(c.n_expert_used * c.n_ff_exp)
        s.moe_gu2 |> resize(c.n_expert_used * c.n_ff_exp)
        s.moe_dn |> resize(c.n_expert_used * c.dim)
        s.moe_dq |> resize(c.n_expert_used * c.n_ff_exp)
        s.moe_ds |> resize(c.n_expert_used * c.n_ff_exp / 32l)
    }
    if (has_ple(c)) {   // gemma-4 E-series PLE decode scratch (one position)
        let all = c.n_layers * c.n_embd_per_layer
        s.ple_inp |> resize(all)
        s.ple_proj |> resize(all)
        s.ple_gate |> resize(c.n_embd_per_layer)
    }
    if (c.recr_mask != 0ul) {   // qwen35 deltanet: per-recurrent-layer conv history + S state (zeroed by resize)
        let cd = dn_conv_dim(c)
        s.dn_conv_prefix |> reserve(int(c.n_layers))
        s.dn_state_prefix |> reserve(int(c.n_layers))
        var co = 0l
        var so = 0l
        for (l in range64(c.n_layers)) {
            if (layer_is_recurrent(c, l)) {
                s.dn_conv_prefix |> push(co)
                s.dn_state_prefix |> push(so)
                co += (c.ssm_d_conv - 1l) * cd
                so += c.ssm_dt_rank * c.ssm_d_state * c.ssm_d_state
            } else {
                s.dn_conv_prefix |> push(-1l)
                s.dn_state_prefix |> push(-1l)
            }
        }
        s.dn_conv_state |> resize(co)
        s.dn_state |> resize(so)
        s.dn_qkv |> resize(cd)
        s.dn_z |> resize(c.ssm_d_inner)
        s.dn_beta |> resize(c.ssm_dt_rank)
        s.dn_g |> resize(c.ssm_dt_rank)
        s.dn_conv_out |> resize(cd)
        s.dn_o |> resize(c.ssm_d_inner)
        s.dn_sk |> resize(c.ssm_dt_rank * c.ssm_d_state)
    }
    if (c.q_gated) {   // gated attention: fused [q | gate] projection scratch
        s.dn_qg |> resize(2l * qd)
        s.dn_qgate |> resize(qd)
    }
    if (c.n_layer_nextn > 0l) {   // MTP/NextN: h_nextn handoff + eh_proj concat input + verify row-0 outputs
        s.mtp_h |> resize(c.dim)
        s.mtp_cat |> resize(2l * c.dim)
        s.mtp_logits |> resize(c.vocab_size)
        s.mtp_vbatch |> resize(2)
    }
    s.ffn_offs |> resize(4)
    s.ffn_gu |> resize(2l * ffw)
    // the Q8·Q8 path quantizes whichever activation feeds a matmul; the MTP eh_proj feeds a
    // 2*dim concat row (don't rely on qd/ffw happening to cover it — 35B-A3B's qd == 2*dim is luck)
    let maxn = max(max(xdim, ffw), c.n_layer_nextn > 0l ? 2l * c.dim : 0l)
    s.xq |> resize(maxn)
    s.xs |> resize(maxn / 32l)
    s.xq2 |> resize(ffw)        // fused-FFN chain requant (see the Session field comment)
    s.xs2 |> resize(ffw / 32l)
    s.xbs |> resize(maxn / 16l)   // K-quant per-16 activation sums (~KB — sized regardless of format)
    s.kxq |> resize(maxn)         // kq v2 Q8_K-form scratch (~KB — sized regardless of format)
    s.kxs |> resize(maxn / 256l)
    s.kxbs |> resize(maxn / 16l)
    s.xbs2 |> resize(ffw / 16l)
    if (kdt == KVDtype.q8_0 || kdt == KVDtype.tq4) {  // block-codec K: the roped (tq4: and
        s.attq |> resize(qd)                          // rotated) query quantizes ONCE per token
        s.atts |> resize(qd / 32l)
    }
    if (kdt == KVDtype.tq4 || vdt == KVDtype.tq4) {
        var maxhs = 0l
        for (l in range64(c.n_layers)) {
            maxhs = max(maxhs, layer_head_size(c, l))
        }
        tq4_fill_signs(s.kv_signs, maxhs)
    }
    s.chain_stages |> resize(3)
    s.kv_runs |> resize(1)      // flat cache: every window is one physical run (paged sessions re-size)
    // KV cache: walk the per-layer row-byte prefixes; shared-KV layers alias their source's slab and contribute no width
    s.kv_dtype_k = kdt
    s.kv_dtype_v = vdt
    // + the MTP/NextN draft block's slab (index n_layers) — one attention layer's rows
    let kv_layers = c.n_layers + c.n_layer_nextn
    s.kv_bprefix_k |> reserve(int(kv_layers))
    s.kv_bprefix_v |> reserve(int(kv_layers))
    var krow = 0l
    var vrow = 0l
    for (l in range64(kv_layers)) {
        let src = layer_kv_source(c, l)
        if (src != l) {
            s.kv_bprefix_k |> push(s.kv_bprefix_k[src])
            s.kv_bprefix_v |> push(s.kv_bprefix_v[src])
        } else {
            s.kv_bprefix_k |> push(krow)
            s.kv_bprefix_v |> push(vrow)
            krow += kv_row_bytes(s.kv_dtype_k, layer_kv_dim(c, l))
            vrow += kv_row_bytes(s.kv_dtype_v, layer_kv_dim(c, l))
        }
    }
    if (alloc_cache) {
        s.key_cache |> resize(c.seq_len * krow)
        s.value_cache |> resize(c.seq_len * vrow)
    }
    s.sample_scratch |> resize(c.vocab_size)
    s.rng = random_seed(1234)
    return <- s
}

//! Create a fresh session (KV cache + scratch) sized to `model`. A model serves many sessions,
//! each with its own position and cache — so one model drives many independent conversations.
//! `kdt`/`vdt` pick this session's KV-cache codec (per-session state, no globals).
def create_session_(model : Model; kdt : KVDtype = KVDtype.f32; vdt : KVDtype = KVDtype.f32) : Session {
    return <- make_run_state(model.config, kdt, vdt)
}

//! Allocate a PAGED run state over a caller-owned pool: the flat slabs stay empty and cache pages
//! come from `pool` on demand (kv_ensure), so KV memory tracks actual context. The pool must
//! outlive the session and stay in place; release the pages (release_kv_pages_) when done.
def make_run_state_paged(c : Config; var pool : KVPool) : Session {
    let kv_layers = c.n_layers + c.n_layer_nextn   // + the MTP/NextN draft block's slab
    if (int64(length(pool.blob_of)) != kv_layers) {
        panic("make_run_state_paged: pool was built for a different model ({length(pool.blob_of)} layers vs {kv_layers})")
    }
    for (l in range64(kv_layers)) {
        if (pool.krow_bytes[pool.blob_of[l]] != kv_row_bytes(pool.kdt, layer_kv_dim(c, l))) {
            panic("make_run_state_paged: pool row bytes mismatch at layer {l} — pool built for a different geometry")
        }
    }
    var s <- make_run_state(c, pool.kdt, pool.vdt, false)
    unsafe {
        s.kv_pool = addr(pool)
    }
    // paged run scratch: a window can span every page of the (post-cap) context
    s.kv_runs |> resize(int((c.seq_len + pool.page_rows - 1l) / pool.page_rows + 1l))
    return <- s
}

//! `create_session_`'s paged twin — see make_run_state_paged / create_kv_pool_.
def create_session_(model : Model; var pool : KVPool) : Session {
    return <- make_run_state_paged(model.config, pool)
}

//! THE eval primitive: run `tokens` at the session's current position and advance it. Prefill =
//! eval(prompt); each step = eval([token]) — same call, different batch size. Logits → `s.logits`.
def eval_(t : Model; var s : Session; tokens : array<int64>) {
    let npos = int64(length(tokens))
    if (npos <= 0l) return
    forward_batch(t, s, tokens, npos, s.n_past)
    s.n_past += npos
}

// ===== Per-app runtime profile (the tune sidecar's "runtime" section) =====

def private apply_i64_knob(rt : JsonValue const?; key : string; blk : block<(v : int64) : void>) {
    let v = read_json_field(rt, key, -1l)
    if (v >= 0l) {
        invoke(blk, v)
        to_log(LOG_INFO, "dasLLAMA: box profile runtime: {key} = {v}\n")
    }
}

//! Apply the runtime knobs from the app tune sidecar's optional "runtime" section (see
//! tune_for_this_box.md): token block, L2 budget, threading thresholds, chunk multipliers, and an
//! optional kernel-backend pin. Missing/STALE file or missing section = no-op; every applied entry logs.
def apply_box_profile_runtime(path : string = "") {
    if (path == "" && box_profile_stale()) {
        to_log(LOG_INFO, "dasLLAMA: tune sidecar is older than the binary — runtime knobs not applied (re-tune)\n")
        return
    }
    let ppath = path != "" ? path : box_profile_path()
    let text = fread(ppath)
    if (empty(text)) return
    var err = ""
    let jv = read_json(text, err)
    if (jv == null) {
        to_log(LOG_WARNING, "dasLLAMA: {ppath} is not valid JSON ({err}) — runtime knobs not applied\n")
        return
    }
    let rt = jv?["runtime"]   // null-sentinel when absent — every read below then keeps its default
    apply_i64_knob(rt, "q8_token_block") $(v) { set_q8_token_block(v) }
    apply_i64_knob(rt, "q8_l2_budget") $(v) { set_q8_l2_budget(v) }
    apply_i64_knob(rt, "target_chunk_work") $(v) { set_target_chunk_work(v) }
    apply_i64_knob(rt, "attn_par_threshold") $(v) { set_attn_par_threshold(v) }
    apply_i64_knob(rt, "requant_par_threshold") $(v) { set_requant_par_threshold(v) }
    apply_i64_knob(rt, "norm_par_threshold") $(v) { set_norm_par_threshold(v) }
    apply_i64_knob(rt, "rope_par_threshold") $(v) { set_rope_par_threshold(v) }
    apply_i64_knob(rt, "kv_store_par_threshold") $(v) { set_kv_store_par_threshold(v) }
    apply_i64_knob(rt, "act_par_threshold") $(v) { set_act_par_threshold(v) }
    // the int-typed knobs clamp into int range before the cast — int() wraps silently on int64
    apply_i64_knob(rt, "q8_chunks_per_job") $(v) { set_q8_chunks_per_job(int(min(v, 2147483647l))) }
    apply_i64_knob(rt, "q4_chunks_per_job") $(v) { set_q4_chunks_per_job(int(min(v, 2147483647l))) }
    apply_i64_knob(rt, "q8_batch_chunks_per_job") $(v) { set_q8_batch_chunks_per_job(int(min(v, 2147483647l))) }
    apply_i64_knob(rt, "matmul_min_chunk_rows") $(v) { set_matmul_min_chunk_rows(int(min(v, 2147483647l))) }
    apply_i64_knob(rt, "matmul_min_chunk_rows_gemv") $(v) { set_matmul_min_chunk_rows_gemv(int(min(v, 2147483647l))) }
    apply_i64_knob(rt, "gemv_chunks_per_lane") $(v) { set_gemv_chunks_per_lane(int(min(v, 2147483647l))) }
    apply_i64_knob(rt, "wscale_f16") $(v) { set_wscale_f16(v != 0l) }
    apply_i64_knob(rt, "kquant_native") $(v) { set_kquant_native(v != 0l) }
    apply_i64_knob(rt, "gemv_lane_cap") $(v) { set_gemv_lane_cap(int(min(v, 2147483647l))) }
    apply_i64_knob(rt, "batch_lane_cap") $(v) { set_batch_lane_cap(int(min(v, 2147483647l))) }
    apply_i64_knob(rt, "batch_grid_2d") $(v) { set_batch_grid_2d(int(min(v, 2l))) }
    apply_i64_knob(rt, "dispatch_worker_limit") $(v) { set_dispatch_worker_limit(int(min(v, 2147483647l))) }
    apply_i64_knob(rt, "gemv_wave_fill") $(v) { set_gemv_wave_fill(v != 0l) }
    apply_i64_knob(rt, "fused_decode") $(v) { set_fused_decode(v != 0l) }
    // stored only — the queue may not exist yet; setup_dasllama_jobque applies these inside with_job_que
    apply_i64_knob(rt, "jobque_spin_us") $(v) { set_jobque_spin_us(v) }
    apply_i64_knob(rt, "jobque_join_poll") $(v) { set_jobque_join_poll(v) }
    apply_i64_knob(rt, "team_rank_gate") $(v) { set_team_rank_gate(int(min(v, 1l))) }
    let be = read_json_field(rt, "backend", "")
    if (be != "") {
        pin_kernel_backend(be)   // warns and keeps the current backend if the name is unknown
        to_log(LOG_INFO, "dasLLAMA: box profile runtime: backend = {active_kernel_backend()} (profile pin '{be}')\n")
    }
    let bbe = read_json_field(rt, "batch_backend", "")
    if (bbe != "") {
        // sticky through the load-time (re)select — activate() re-applies it; warns and keeps the
        // active backend's batch slots if the donor is unknown or reads a different weight layout
        select_batch_backend(bbe)
        to_log(LOG_INFO, "dasLLAMA: box profile runtime: batch_backend = {active_batch_backend()} (profile pin '{bbe}')\n")
    }
    // "threads" is advisory — the worker count is fixed at jobque creation, controlled by the
    // DAS_JOBQUE_THREADS env (see tune_for_this_box.md); a profile can only recommend it. No live
    // comparison here: get_dispatch_lanes() throws outside a job queue, and load_model may run there.
    let adv = read_json_field(rt, "threads", -1l)
    if (adv > 0l) {
        to_log(LOG_INFO, "dasLLAMA: box profile advises threads = {adv} — ensure DAS_JOBQUE_THREADS={adv}\n")
    }
}

//! Load a model AND its tokenizer from a GGUF — the full user-facing entry point. Arch and
//! tokenizer backend are both auto-selected from GGUF metadata; `mode` picks the weight quant.
//! Applies the tune sidecar's "runtime" knobs first (see apply_box_profile_runtime).
def load_model_(path : string; mode : QuantMode = QuantMode.fp32) : Model {
    apply_box_profile_runtime()   // before load: a backend pin must precede the backend-for-load select
    var m <- load_gguf(path, mode)
    m.tok <- load_tokenizer_auto(path)
    return <- m
}

//! Encode text to token ids using the model's tokenizer.
def encode_(m : Model; text : string; add_special = true; parse_special = false) : array<int64> {
    return <- encode(m.tok, text, add_special, parse_special)
}

//! Decode a full id sequence to text using the model's tokenizer.
def decode_(m : Model; ids : array<int64>) : string {
    return decode(m.tok, ids)
}

//! Decode a single token to text (streaming) using the model's tokenizer.
def piece_(m : Model; id : int64) : string {
    return piece(m.tok, id)
}

//! Resolve a special-token id by its surface form (chat templates reference these by spelling).
def special_id(m : Model; name : string) : int64 {
    return special_id(m.tok, name)
}

//! The model's BOS token id (per the tokenizer metadata).
def bos_id(m : Model) : int64 {
    return bos_id(m.tok)
}

// fp32 matmul with the weight at element offset `woff` in the big-weight blob (blob+offset kernel
// form — array_view's block invoke is an LLVM optimization boundary, and the kernel only ever
// wanted the base pointer; same for every mm_* wrapper below).
def private mm_at(var y : array<float>; t : Model; woff : int64; x : array<float>; n, d : int64) {
    matmul(y, t.wblob, woff, x, n, d)
}

// fp32 matmul against the fp32 aux blob (used for the tied classifier = embedding).
def private mm_fblob(var y : array<float>; t : Model; woff : int64; x : array<float>; n, d : int64) {
    matmul(y, t.fblob, woff, x, n, d)
}

// Batched fp32 matmul against the aux blob: Y [npos x d] token-major from X [npos x n] — the grouped
// MoE prefill's router. matmul_batch is bit-for-bit npos x matmul, so routing matches decode exactly.
def private mm_fblob_b(var y : array<float>; t : Model; woff : int64; x : array<float>; n, d, npos : int64) {
    matmul_batch(y, t.fblob, woff, x, n, d, npos)
}

// rmsnorm with the weight at `woff` in the fp32 aux blob — straight to the tuned pointer core.
def private rms_at(var o : array<float> | #; t : Model; woff : int64; x : array<float> | #; size : int64) {
    unsafe {
        rmsnorm(addr < float? >(o[0]), addr < float const? >(x[0]),
            addr < float const? >(t.fblob[woff]), size, t.config.norm_eps)
    }
}

// Same, with the source at element offset `xoff` (per-position rows of a batched activation).
def private rms_at(var o : array<float> | #; t : Model; woff : int64; x : array<float>; xoff : int64; size : int64) {
    unsafe {
        rmsnorm(addr < float? >(o[0]), addr < float const? >(x[xoff]),
            addr < float const? >(t.fblob[woff]), size, t.config.norm_eps)
    }
}

// Threaded per-position rmsnorm: out[p*dim..] = rmsnorm(src[p*dim..]) with the layer weight at fblob+woff.
// `out` and `src` may be the same array (Gemma post-norms run in place). Fork workers touch raw addresses.
def private rms_batch(var out : array<float>; src : array<float>; t : Model; woff : int64; dim, npos : int64) {
    let eps = t.config.norm_eps
    unsafe {
        var op = addr(out[0])
        let ip = addr < float const? >(src[0])
        let wp = addr < float const? >(t.fblob[woff])
        maybe_parallel_for(dim * npos >= g_norm_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let off = int64(p) * dim
                    rmsnorm(op + off, ip + off, wp, dim, eps)
                }
            }
        }
    }
}

// Threaded per-position RoPE (pass 1 of the two-pass rope+store): rope q_b[p*qd..] and k_b[p*kv_dim..]
// in place, at absolute pos start_pos+p. Calls the leaf rope_scaled/rope_scaled_neox DIRECTLY, not
// the rope_apply wrapper — routing through it lets the JIT mark the pointer readonly and silently no-op the write.
def private rope_batch(var q_b : array<float>; var k_b : array<float>; t : Model; theta, fscale : float; start_pos, npos, head_size, qd, kv_dim : int64; use_ff : bool = true; rot : int64 = 0l) {
    let c = t.config
    let neox = c.rope_neox
    let mscale = c.rope_mscale
    let has_ff = use_ff && !empty(t.rope_freqs)
    // partial rotary (glm4moe: 64 of 128) — the loader guarantees partial => NEOX, no rope_freqs
    let part = rot > 0l && rot != head_size
    unsafe {
        var qp = addr(q_b[0])
        var kp = addr(k_b[0])
        let ffp = has_ff ? addr < float const? >(t.rope_freqs[0]) : null
        let rope_par = (qd + kv_dim) * npos >= g_rope_par_threshold
        maybe_parallel_for(rope_par, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    let ap = start_pos + pp
                    if (part) {
                        rope_scaled_neox_part(qp + pp * qd, ap, head_size, rot, qd, theta, fscale, has_ff, ffp, mscale)
                        rope_scaled_neox_part(kp + pp * kv_dim, ap, head_size, rot, kv_dim, theta, fscale, has_ff, ffp, mscale)
                    } elif (neox) {
                        rope_scaled_neox(qp + pp * qd, ap, head_size, qd, theta, fscale, has_ff, ffp, mscale)
                        rope_scaled_neox(kp + pp * kv_dim, ap, head_size, kv_dim, theta, fscale, has_ff, ffp, mscale)
                    } else {
                        rope_scaled(qp + pp * qd, ap, head_size, qd, theta, fscale, has_ff, ffp, mscale)
                        rope_scaled(kp + pp * kv_dim, ap, head_size, kv_dim, theta, fscale, has_ff, ffp, mscale)
                    }
                }
            }
        }
    }
}

// Precompute the prefill RoPE cos/sin table once: cos_tab[pi*half + j] = cos((start_pos+pi)*freq[j]),
// sin likewise. freq[j] is position-independent (hoisted out of the position loop); mscale folds
// in here. Built once before the layer loop, reused across all heads/layers. Bit-identical to classic.
def private build_rope_table(var cos_tab : array<float>; var sin_tab : array<float>; t : Model; theta, fscale : float; start_pos, npos, head_size : int64; use_ff : bool = true) {
    let c = t.config
    let half = head_size / 2l
    let scale = c.rope_mscale   // scales cos/sin — ggml applies mscale to NORM and NEOX alike (mistral3 YaRN)
    let has_ff = use_ff && !empty(t.rope_freqs)
    cos_tab |> resize(npos * half)
    sin_tab |> resize(npos * half)
    unsafe {
        var cp = addr(cos_tab[0])
        var sp = addr(sin_tab[0])
        let ffp = has_ff ? addr < float const? >(t.rope_freqs[0]) : null
        for (j in range64(half)) {
            var freq = 1.0 / pow(theta, float(2l * j) / float(head_size))
            if (has_ff) {
                freq /= ffp[j]
            }
            for (pi in range64(npos)) {
                let angle = float(start_pos + pi) * fscale * freq
                cp[pi * half + j] = cos(angle) * scale
                sp[pi * half + j] = sin(angle) * scale
            }
        }
    }
}

// build_rope_table's batched-decode twin: row pi's angle uses positions[pi] instead of start_pos+pi
// (batch rows come from different sequences at unrelated positions). Bit-identical per row to the
// npos=1 decode table at that position, so rope_batch_tab consumes it unchanged.
def private build_rope_table_rows(var cos_tab : array<float>; var sin_tab : array<float>; t : Model;
                                  theta, fscale : float; positions : array<int64>; nrows, head_size : int64;
                                  use_ff : bool = true) {
    let c = t.config
    let half = head_size / 2l
    let scale = c.rope_mscale
    let has_ff = use_ff && !empty(t.rope_freqs)
    cos_tab |> resize(nrows * half)
    sin_tab |> resize(nrows * half)
    unsafe {
        var cp = addr(cos_tab[0])
        var sp = addr(sin_tab[0])
        let pp = addr < int64 const? >(positions[0])
        let ffp = has_ff ? addr < float const? >(t.rope_freqs[0]) : null
        for (j in range64(half)) {
            var freq = 1.0 / pow(theta, float(2l * j) / float(head_size))
            if (has_ff) {
                freq /= ffp[j]
            }
            for (pi in range64(nrows)) {
                let angle = float(pp[pi]) * fscale * freq
                cp[pi * half + j] = cos(angle) * scale
                sp[pi * half + j] = sin(angle) * scale
            }
        }
    }
}

// Table-driven prefill RoPE: rope q_b[p*qd..] and k_b[p*kv_dim..] in place using the precomputed
// cos/sin table (no transcendentals). Calls the LEAF rope_scaled_*_tab DIRECTLY (not a dispatch
// wrapper) to avoid the JIT pass-through-const-write-elision bug.
def private rope_batch_tab(var q_b : array<float>; var k_b : array<float>; cos_tab : array<float>; sin_tab : array<float>;
                           neox : bool; npos, head_size, qd, kv_dim : int64; rot : int64 = 0l) {
    // partial rotary (glm4moe): a full-head walk would read past each row and rotate dims that must stay put
    let part = rot > 0l && rot != head_size
    let half = part ? rot / 2l : head_size / 2l
    unsafe {
        var qp = addr(q_b[0])
        var kp = addr(k_b[0])
        let cp = addr < float const? >(cos_tab[0])
        let sp = addr < float const? >(sin_tab[0])
        let rope_par = (qd + kv_dim) * npos >= g_rope_par_threshold
        maybe_parallel_for(rope_par, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    let crow = cp + pp * half
                    let srow = sp + pp * half
                    if (part) {
                        rope_scaled_neox_tab_part(qp + pp * qd, head_size, rot, qd, crow, srow)
                        rope_scaled_neox_tab_part(kp + pp * kv_dim, head_size, rot, kv_dim, crow, srow)
                    } elif (neox) {
                        rope_scaled_neox_tab(qp + pp * qd, head_size, qd, crow, srow)
                        rope_scaled_neox_tab(kp + pp * kv_dim, head_size, kv_dim, crow, srow)
                    } else {
                        rope_scaled_tab(qp + pp * qd, head_size, qd, crow, srow)
                        rope_scaled_tab(kp + pp * kv_dim, head_size, kv_dim, crow, srow)
                    }
                }
            }
        }
    }
}

// ===== KV-cache codec seam =====
// Coordinates stay DECOMPOSED (layer-base pointer, row index, sub-row offset), so a block codec
// slots in as a kv_row_bytes case + its own kernel overloads, with zero call-site changes.

//! Bytes per stored element (scalar codecs only; block codecs size via kv_row_bytes directly).
def kv_elem_bytes(dt : KVDtype) : int64 => dt == KVDtype.f16 ? 2l : 4l

//! Byte stride of one position row of width kv_dim under codec dt — THE layout seam.
//! q8_0: kv_dim/32 blocks of {f16 d; int8 qs[32]} = 34 bytes each; tq4: kv_dim/32 blocks of
//! {f16 d; uint8 qs[16]} = 18 bytes each (kv_dim % 32 == 0 by create).
def kv_row_bytes(dt : KVDtype; kv_dim : int64) : int64 {
    if (dt == KVDtype.q8_0) return kv_dim / 32l * 34l
    if (dt == KVDtype.tq4) return kv_dim / 32l * 18l
    return kv_dim * kv_elem_bytes(dt)
}

//! Head-slot offset in the side's own addressing units: elements for scalar codecs, bytes for the
//! block codecs (whole blocks). The _d dispatch wrappers prescale with this so the codec-generic
//! workers' `(rowbase + j) * kdim + khoff` walks stay unit-correct.
def kv_side_off(dt : KVDtype; elems : int64) : int64 {
    if (dt == KVDtype.q8_0) return elems / 32l * 34l
    if (dt == KVDtype.tq4) return elems / 32l * 18l
    return elems
}

//! Layer l's slab base (BYTES) in the key cache; the byte analog of kv_cache_off, reading the
//! LIVE seq_len so post-load seq_len caps keep working.
def kv_base_k(s : Session; l, seq_len : int64) : int64 => s.kv_bprefix_k[l] * seq_len

//! Layer l's slab base (BYTES) in the value cache.
def kv_base_v(s : Session; l, seq_len : int64) : int64 => s.kv_bprefix_v[l] * seq_len

//! One run of positions whose rows are PHYSICALLY consecutive in the cache: absolute positions
//! [j0, j1), with the row of j0 at physical row index row0 (flat cache: row0 == j0 always).
//! Attention workers address rows as (row0 - j0 + j); paged caches produce one run per page span.
struct KVRun {
    j0 : int64
    j1 : int64
    row0 : int64
}

//! Fill `runs` with the physical runs covering absolute positions [lo, hi); returns the run count.
//! THE flat/paged branch lives here: a flat session is always exactly one run; a paged one gets a
//! run per block-table page span (physically-consecutive spans merge).
def kv_runs(s : Session; lo, hi : int64; var runs : array<KVRun>; at : int64 = 0l) : int64 {
    if (hi <= lo) {
        return 0l
    }
    if (s.kv_pool == null) {
        // flat cache: one contiguous slab — one run, physical row == absolute position
        runs[at] = KVRun(j0 = lo, j1 = hi, row0 = lo)
        return 1l
    }
    let pr = s.kv_pool.page_rows
    var n = 0l
    var j = lo
    while (j < hi) {
        let pj1 = min(hi, (j / pr + 1l) * pr)
        let row0 = int64(s.block_table[j / pr]) * pr + j % pr
        if (n > 0l && runs[at + n - 1l].row0 + (j - runs[at + n - 1l].j0) == row0) {
            runs[at + n - 1l].j1 = pj1   // physically consecutive with the previous run: extend
        } else {
            runs[at + n] = KVRun(j0 = j, j1 = pj1, row0 = row0)
            n ++
        }
        j = pj1
    }
    return n
}

//! Physical row index of absolute position `pos` — the single-row-store analog of kv_runs.
//! Flat cache: identity; paged: through the block table (page group * P + in-page offset).
def kv_phys_row(s : Session; pos : int64) : int64 {
    if (s.kv_pool == null) {
        return pos
    }
    let pr = s.kv_pool.page_rows
    return int64(s.block_table[pos / pr]) * pr + pos % pr
}

//! Layer l's base byte pointer in the key cache — THE flat/paged storage branch. Flat: the session
//! slab at its byte prefix; paged: the pool's owning blob for l. Hoist AFTER kv_ensure: pool
//! growth resizes blobs, which would dangle this pointer.
def kv_layer_kbase(var s : Session; l, seq_len : int64) : uint8? {
    unsafe {
        if (s.kv_pool != null) {
            return addr(s.kv_pool.kblobs[s.kv_pool.blob_of[l]][0])
        }
        return addr(s.key_cache[0]) + kv_base_k(s, l, seq_len)
    }
}

//! Layer l's base byte pointer in the value cache — see kv_layer_kbase.
def kv_layer_vbase(var s : Session; l, seq_len : int64) : uint8? {
    unsafe {
        if (s.kv_pool != null) {
            return addr(s.kv_pool.vblobs[s.kv_pool.blob_of[l]][0])
        }
        return addr(s.value_cache[0]) + kv_base_v(s, l, seq_len)
    }
}

// ===== paged KV pool (E2) =====

//! Create a paged KV pool for `c`'s cache geometry: sessions created over it (make_run_state_paged)
//! allocate `page_rows`-position page groups on demand instead of the full seq_len slab, so cache
//! memory tracks actual context and many sessions share one elastic pool.
def create_kv_pool_(c : Config; page_rows : int64; kdt, vdt : KVDtype) : KVPool {
    if (page_rows <= 0l) {
        panic("create_kv_pool: page_rows must be positive (got {page_rows})")
    }
    kv_validate_q8(c, kdt, vdt)
    var p = KVPool(page_rows = page_rows, kdt = kdt, vdt = vdt)
    // + the MTP/NextN draft block's slab (index n_layers) — one attention layer's rows, so the
    // self-spec rails work on paged sessions too (pages only allocate when rows are written)
    let kv_layers = c.n_layers + c.n_layer_nextn
    p.blob_of |> reserve(int(kv_layers))
    for (l in range64(kv_layers)) {
        let src = layer_kv_source(c, l)
        if (src != l) {
            p.blob_of |> push(p.blob_of[src])   // shared-KV layer: alias the source's blob
        } else {
            p.blob_of |> push(p.nblobs)
            p.krow_bytes |> push(kv_row_bytes(kdt, layer_kv_dim(c, l)))
            p.vrow_bytes |> push(kv_row_bytes(vdt, layer_kv_dim(c, l)))
            p.nblobs ++
        }
    }
    p.kblobs |> resize(int(p.nblobs))
    p.vblobs |> resize(int(p.nblobs))
    return <- p
}

// Double the pool's group capacity. Every blob resizes IN PLACE — a hoisted cache pointer would
// dangle, which is why kv_ensure runs at the top of the forward entries, before any hoist.
def private kv_pool_grow(var p : KVPool) {
    let oldcap = p.capacity_groups
    let newcap = oldcap <= 0l ? 4l : oldcap * 2l
    for (b in range64(p.nblobs)) {
        p.kblobs[b] |> resize(int(newcap * p.page_rows * p.krow_bytes[b]))
        p.vblobs[b] |> resize(int(newcap * p.page_rows * p.vrow_bytes[b]))
    }
    p.refcnt |> resize(int(newcap))
    var g = int(newcap)
    while (g > int(oldcap)) {   // push high->low so the LOWEST new group pops first
        g --
        p.free_groups |> push(g)
    }
    p.capacity_groups = newcap
}

//! Allocate one page group (refcount 1): pop the free list, growing the pool when it runs dry.
def kv_pool_alloc_group(var p : KVPool) : int {
    if (empty(p.free_groups)) {
        kv_pool_grow(p)
    }
    let g = p.free_groups[length(p.free_groups) - 1]
    p.free_groups |> pop()
    p.refcnt[g] = 1
    return g
}

//! Drop one reference to group `g`; at zero the group returns to the free list.
def kv_pool_release_group(var p : KVPool; g : int) {
    verify(p.refcnt[g] > 0)
    p.refcnt[g] --
    if (p.refcnt[g] == 0) {
        p.free_groups |> push(g)
    }
}

//! Return every page this session holds to its pool (flat sessions: no-op). Call when the session
//! is done — pages are pool-owned, so deleting the session alone leaks them from the pool. The
//! session itself stays alive but loses its cached context; to reuse it, also reset n_past to 0.
def release_kv_pages_(var s : Session) {
    if (s.kv_pool == null) {
        return
    }
    for (g in s.block_table) {
        kv_pool_release_group(*s.kv_pool, g)
    }
    s.block_table |> clear()
}

//! Grow this session's block table to cover positions [0, upto) — the ONLY place a paged session
//! allocates cache. Runs at the top of forward/forward_prefill/eval_batch, BEFORE any cache-pointer
//! hoist: group allocation may resize the pool's blobs. Flat sessions: no-op.
def kv_ensure(var s : Session; upto : int64) {
    if (s.kv_pool == null) {
        return
    }
    let np = (upto + s.kv_pool.page_rows - 1l) / s.kv_pool.page_rows
    while (int64(length(s.block_table)) < np) {
        s.block_table |> push(kv_pool_alloc_group(*s.kv_pool))
    }
}

//! Store n f32 values into a KV-cache layer slab at (row, sub-row element offset). dst0 = the
//! layer's base byte pointer. Codec dispatch happens HERE, once per row-store, never per element.
//! Contract: one partial row (sub + n <= kv_dim), or whole consecutive rows (sub == 0).
def kv_store_row(var dst0 : uint8?; dt : KVDtype; kv_dim, row, sub, n : int64; src : float const?) {
    if (dt == KVDtype.f32) {
        unsafe {
            // bind the cast first: reinterpret is a PREFIX operator, so an inline
            // `reinterpret<float?>(dst0) + off` would swallow the addition into its operand
            // and scale off in BYTES (uint8 arithmetic), not elements
            var d = reinterpret<float?>(dst0)
            copy_floats(d + row * kv_dim + sub, src, n)
        }
    } elif (dt == KVDtype.f16) {
        unsafe {
            var d = reinterpret<uint16?>(dst0)
            cvt_f32_to_f16(d + row * kv_dim + sub, src, n)   // clamps to ±65504: inf never enters the cache
        }
    } elif (dt == KVDtype.q8_0) {
        unsafe {
            // sub is 0 or a head boundary and head_size/kv_dim are 32-multiples (create-time
            // contract), so the destination is always a whole-block offset
            quantize_q8kv_row(dst0 + (row * kv_dim + sub) / 32l * 34l, src, n)
        }
    } elif (dt == KVDtype.tq4) {
        unsafe {
            // src rows are already in the rotated basis — the drivers rotate at the RoPE seam
            quantize_tq4kv_row(dst0 + (row * kv_dim + sub) / 32l * 18l, src, n)
        }
    } else {
        panic("KV cache codec not yet implemented")
    }
}

// kv_store_row's read twin: dequant row `row`'s [sub, sub + n) elements into f32 `dst` — the
// Metal prefill continuation gathers the session's existing rows into its attention panels
// through this. tq4 values come out in the ROTATED basis; callers un-rotate per head.
def kv_load_row(src0 : uint8 const?; dt : KVDtype; kv_dim, row, sub, n : int64; var dst : float?) {
    if (dt == KVDtype.f32) {
        unsafe {
            var sp = reinterpret<float const?>(src0)
            copy_floats(dst, sp + row * kv_dim + sub, n)
        }
    } elif (dt == KVDtype.f16) {
        unsafe {
            var sp = reinterpret<uint16 const?>(src0)
            cvt_f16_to_f32(dst, sp + row * kv_dim + sub, n)
        }
    } elif (dt == KVDtype.q8_0) {
        unsafe {
            cvt_q8kv_to_f32(dst, src0 + (row * kv_dim + sub) / 32l * 34l, n)
        }
    } elif (dt == KVDtype.tq4) {
        unsafe {
            cvt_tq4kv_to_f32(dst, src0 + (row * kv_dim + sub) / 32l * 18l, n)
        }
    } else {
        panic("KV cache codec not yet implemented")
    }
}

// Read-side glue: overload per cache element type, resolved inside the monomorphized workers.
def private kv_dot(a : float const?; b : float const?; n : int64) : float => dot(a, b, n)
def private kv_axpy(var d : float?; s : float const?; scale : float; n : int64) {
    axpy(d, s, scale, n)
}
def private kv_load(p : float const?; base, i : int64) : float => unsafe(p[base + i])
def private kv_dot(a : float const?; b : uint16 const?; n : int64) : float => dot_f16(a, b, n)
def private kv_axpy(var d : float?; s : uint16 const?; scale : float; n : int64) {
    axpy_f16(d, s, scale, n)
}
def private kv_load(p : uint16 const?; base, i : int64) : float => f16_to_f32(uint(unsafe(p[base + i])))
// q8_0: the cache pointer stays uint8 (byte units — the _d wrappers prescale stride/offset via
// kv_side_off), so `base` here is a whole-block BYTE offset and `i` an element index within it
def private kv_dot(a : float const?; b : uint8 const?; n : int64) : float => dot_q8kv(a, b, n)
def private kv_axpy(var d : float?; s : uint8 const?; scale : float; n : int64) {
    axpy_q8kv(d, s, scale, n)
}
def private kv_load(p : uint8 const?; base, i : int64) : float {
    unsafe {
        let blk = p + base + i / 32l * 34l
        let b16 = reinterpret<uint16 const?>(blk)   // bind first: reinterpret would swallow the index
        let bq = reinterpret<int8 const?>(blk)
        return float(bq[2l + i % 32l]) * f16_to_f32(uint(b16[0]))
    }
}
// tq4: dispatch rides the TQ4B distinct-pointer tag (q8_0 owns the raw uint8 overloads); the
// kernels themselves are byte-typed, so each arm re-tags back to uint8. Values come out in the
// ROTATED basis — kv_axpy accumulates rotated and the drivers un-rotate each head's output once.
def private kv_axpy(var d : float?; s : TQ4B const?; scale : float; n : int64) {
    axpy_tq4kv(d, unsafe(reinterpret<uint8 const?>(s)), scale, n)
}
def private kv_load(p : TQ4B const?; base, i : int64) : float {
    unsafe {
        let blk = reinterpret<uint8 const?>(p) + base + i / 32l * 18l
        let b16 = reinterpret<uint16 const?>(blk)
        let by = int(blk[2l + (i % 32l) / 2l])
        let q = (i & 1l) == 0l ? (by & 15) : (by >> 4)
        return float(q - 8) * f16_to_f32(uint(b16[0]))
    }
}
// kv_side_off's pointer-typed twin for code that is generic over the cache element type (the
// flash-decode witness): element offset -> the side's own addressing units
[unused_argument(p)]
def private kv_head_off(p : float const?; elems : int64) : int64 => elems
[unused_argument(p)]
def private kv_head_off(p : uint16 const?; elems : int64) : int64 => elems
[unused_argument(p)]
def private kv_head_off(p : uint8 const?; elems : int64) : int64 => elems / 32l * 34l
[unused_argument(p)]
def private kv_head_off(p : TQ4B const?; elems : int64) : int64 => elems / 32l * 18l

// Bulk one-row dequant into f32 scratch — the flash pack's per-row primitive (and any future
// codec's bulk-dequant seam: q8_0 adds a cvt overload here, never a new pack shape).
def private kv_row_to_f32(var d : float?; p : float const?; base, n : int64) {
    unsafe {
        copy_floats(d, p + base, n)
    }
}
def private kv_row_to_f32(var d : float?; p : uint16 const?; base, n : int64) {
    unsafe {
        cvt_f16_to_f32(d, p + base, n)
    }
}
def private kv_row_to_f32(var d : float?; p : uint8 const?; base, n : int64) {
    unsafe {
        cvt_q8kv_to_f32(d, p + base, n)
    }
}
def private kv_row_to_f32(var d : float?; p : TQ4B const?; base, n : int64) {
    unsafe {
        cvt_tq4kv_to_f32(d, reinterpret<uint8 const?>(p) + base, n)
    }
}

// Decode-read score dot, resolved on the K-cache pointer type: scalar codecs dot the f32 query;
// q8_0 dots the PRE-QUANTIZED query planes (qq/qs — filled once per token after RoPE) through the
// int8×int8 kernel. Lets the flash-decode witness recompute scores exactly as the sliced path did.
[unused_argument(qq, qs)]
def private kv_score(qp : float const?; qq : int8 const?; qs : float const?; kcp : float const?;
                     koff, qoff, head_size : int64) : float {
    return kv_dot(unsafe(qp + qoff), unsafe(kcp + koff), head_size)
}
[unused_argument(qq, qs)]
def private kv_score(qp : float const?; qq : int8 const?; qs : float const?; kcp : uint16 const?;
                     koff, qoff, head_size : int64) : float {
    return kv_dot(unsafe(qp + qoff), unsafe(kcp + koff), head_size)
}
[unused_argument(qp)]
def private kv_score(qp : float const?; qq : int8 const?; qs : float const?; kcp : uint8 const?;
                     koff, qoff, head_size : int64) : float {
    return dot_q8q8kv(unsafe(qq + qoff), unsafe(qs + qoff / 32l), unsafe(kcp + koff), head_size)
}
// tq4 K: same pre-quantized-query planes as q8_0 — the drivers fill them from the ROTATED query,
// so the int8×int4 dot runs entirely in the rotated basis
[unused_argument(qp)]
def private kv_score(qp : float const?; qq : int8 const?; qs : float const?; kcp : TQ4B const?;
                     koff, qoff, head_size : int64) : float {
    unsafe {
        let kb = reinterpret<uint8 const?>(kcp)
        return dot_q8tq4kv(qq + qoff, qs + qoff / 32l, kb + koff, head_size)
    }
}

// One decode attention head over one layer's cache slab — the attention_std_decode/chain-decode
// stage-1 worker body, generic over the cache element type. kcp/vcp are LAYER-BASE typed pointers;
// arow is this head's score row (window-relative). Cache rows found through `runs` (kv_runs contract).
def private attn_head_decode(var arow : float?; var xbp : float?; qp : float const?;
                             qq : int8 const?; qsp : float const?;
                             kcp, vcp : auto const?; runs : KVRun const?;
                             nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh : int64;
                             scale, attn_softcap : float; sinksp : float const?) {
    unsafe {
        for (r in range64(nrun)) {
            let rj0 = max(runs[r].j0, kstart)
            let rj1 = min(runs[r].j1, kstart + cnt)
            let rowbase = runs[r].row0 - runs[r].j0
            for (j in range64(rj0, rj1)) {
                arow[j - kstart] = kv_score(qp, qq, qsp, kcp, (rowbase + j) * kdim + khoff, qoff, head_size) * scale
            }
        }
        if (attn_softcap > 0.0) {  // Gemma2: soft-cap scores before softmax (softcap4 = the flash-prefill form)
            softcap4(arow, cnt, attn_softcap)
        }
        if (sinksp != null) {  // gpt-oss: this head's sink logit joins the softmax denominator
            softmax_sink(arow, cnt, sinksp[hh])
        } else {
            softmax(arow, cnt)
        }
        // weighted sum of values into xb[qoff..]
        for (i in range64(head_size)) {
            xbp[qoff + i] = 0.0
        }
        for (r in range64(nrun)) {
            let rj0 = max(runs[r].j0, kstart)
            let rj1 = min(runs[r].j1, kstart + cnt)
            let rowbase = runs[r].row0 - runs[r].j0
            for (j in range64(rj0, rj1)) {
                kv_axpy(xbp + qoff, vcp + (rowbase + j) * vdim + vhoff, arow[j - kstart], head_size)
            }
        }
    }
}

// Flash-decode sliced K pass (chain stage 1, nsl > 1): the slice's K rows stream once, dotted
// against every q-head of the GQA group. Codec-generic + run-walking like attn_head_decode; the
// slice sub-window [kstart+j0, kstart+j1) intersects each run.
def private kv_slice_scores(var attp : float?; qp : float const?; qq : int8 const?; qsp : float const?;
                            kcp : auto const?; runs : KVRun const?;
                            nrun, kstart, j0, j1, kdim, khoff, head_size, qh0, kv_mul, seq_len : int64;
                            scale : float) {
    unsafe {
        for (r in range64(nrun)) {
            let rj0 = max(runs[r].j0, kstart + j0)
            let rj1 = min(runs[r].j1, kstart + j1)
            let rowbase = runs[r].row0 - runs[r].j0
            for (aj in range64(rj0, rj1)) {
                let j = aj - kstart
                let koff = (rowbase + aj) * kdim + khoff
                for (g in range64(kv_mul)) {
                    attp[(qh0 + g) * seq_len + j] = kv_score(qp, qq, qsp, kcp, koff, (qh0 + g) * head_size, head_size) * scale
                }
            }
        }
    }
}

// Flash-decode sliced V pass: each V row read once, weighted into every group head's partial.
def private kv_slice_av(var flop : float?; attp : float const?; vcp : auto const?; runs : KVRun const?;
                        nrun, kstart, j0, j1, vdim, vhoff, head_size, qh0, kv_mul, seq_len, nsl, sl : int64) {
    unsafe {
        for (r in range64(nrun)) {
            let rj0 = max(runs[r].j0, kstart + j0)
            let rj1 = min(runs[r].j1, kstart + j1)
            let rowbase = runs[r].row0 - runs[r].j0
            for (aj in range64(rj0, rj1)) {
                let j = aj - kstart
                let voff = (rowbase + aj) * vdim + vhoff
                for (g in range64(kv_mul)) {
                    let hh = qh0 + g
                    kv_axpy(flop + (hh * nsl + sl) * head_size, vcp + voff, attp[hh * seq_len + j], head_size)
                }
            }
        }
    }
}

// ===== codec dispatch wrappers =====
// Byte-base cache pointers + session dtypes in, ONE monomorphized worker call out. The scalar
// K×V matrix is spelled here once; a new codec = new arms here with zero call-site changes.

def private attn_head_decode_d(var arow : float?; var xbp : float?; qp : float const?;
                               qq : int8 const?; qsp : float const?;
                               kcl, vcl : uint8 const?; kdt, vdt : KVDtype; runs : KVRun const?;
                               nrun, qoff, kvhoff, kstart, cnt, kv_dim, head_size, hh : int64;
                               scale, attn_softcap : float; sinksp : float const?) {
    // per-side stride/offset in the side's own units — elements for scalar codecs, BYTES for the q8_0 block codec
    let kdim = kv_side_off(kdt, kv_dim)
    let khoff = kv_side_off(kdt, kvhoff)
    let vdim = kv_side_off(vdt, kv_dim)
    let vhoff = kv_side_off(vdt, kvhoff)
    unsafe {
        if (kdt == KVDtype.q8_0) {
            if (vdt == KVDtype.q8_0) {
                attn_head_decode(arow, xbp, qp, qq, qsp, kcl, vcl,
                    runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_decode(arow, xbp, qp, qq, qsp, kcl, reinterpret<TQ4B const?>(vcl),
                    runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_decode(arow, xbp, qp, qq, qsp, kcl, reinterpret<uint16 const?>(vcl),
                    runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
            } else {
                attn_head_decode(arow, xbp, qp, qq, qsp, kcl, reinterpret<float const?>(vcl),
                    runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
            }
        } elif (kdt == KVDtype.tq4) {
            if (vdt == KVDtype.q8_0) {
                attn_head_decode(arow, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), vcl,
                    runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_decode(arow, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<TQ4B const?>(vcl),
                    runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_decode(arow, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<uint16 const?>(vcl),
                    runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
            } else {
                attn_head_decode(arow, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<float const?>(vcl),
                    runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
            }
        } elif (kdt == KVDtype.f16) {
            if (vdt == KVDtype.q8_0) {
                attn_head_decode(arow, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), vcl,
                    runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_decode(arow, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<TQ4B const?>(vcl),
                    runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_decode(arow, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<uint16 const?>(vcl),
                    runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
            } else {
                attn_head_decode(arow, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<float const?>(vcl),
                    runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
            }
        } elif (vdt == KVDtype.q8_0) {
            attn_head_decode(arow, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), vcl,
                runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
        } elif (vdt == KVDtype.tq4) {
            attn_head_decode(arow, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<TQ4B const?>(vcl),
                runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
        } elif (vdt == KVDtype.f16) {
            attn_head_decode(arow, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<uint16 const?>(vcl),
                runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
        } else {
            attn_head_decode(arow, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<float const?>(vcl),
                runs, nrun, qoff, kdim, khoff, vdim, vhoff, kstart, cnt, head_size, hh, scale, attn_softcap, sinksp)
        }
    }
}

def private kv_slice_scores_d(var attp : float?; qp : float const?; qq : int8 const?; qsp : float const?;
                              kcl : uint8 const?; kdt : KVDtype;
                              runs : KVRun const?;
                              nrun, kstart, j0, j1, kv_dim, kvhoff, head_size, qh0, kv_mul, seq_len : int64;
                              scale : float) {
    let kdim = kv_side_off(kdt, kv_dim)
    let khoff = kv_side_off(kdt, kvhoff)
    unsafe {
        if (kdt == KVDtype.q8_0) {
            kv_slice_scores(attp, qp, qq, qsp, kcl, runs, nrun, kstart, j0, j1, kdim, khoff,
                head_size, qh0, kv_mul, seq_len, scale)
        } elif (kdt == KVDtype.tq4) {
            kv_slice_scores(attp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), runs, nrun, kstart, j0, j1, kdim, khoff,
                head_size, qh0, kv_mul, seq_len, scale)
        } elif (kdt == KVDtype.f16) {
            kv_slice_scores(attp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), runs, nrun, kstart, j0, j1, kdim, khoff,
                head_size, qh0, kv_mul, seq_len, scale)
        } else {
            kv_slice_scores(attp, qp, qq, qsp, reinterpret<float const?>(kcl), runs, nrun, kstart, j0, j1, kdim, khoff,
                head_size, qh0, kv_mul, seq_len, scale)
        }
    }
}

def private kv_slice_av_d(var flop : float?; attp : float const?; vcl : uint8 const?; vdt : KVDtype;
                          runs : KVRun const?;
                          nrun, kstart, j0, j1, kv_dim, kvhoff, head_size, qh0, kv_mul, seq_len, nsl, sl : int64) {
    let vdim = kv_side_off(vdt, kv_dim)
    let vhoff = kv_side_off(vdt, kvhoff)
    unsafe {
        if (vdt == KVDtype.q8_0) {
            kv_slice_av(flop, attp, vcl, runs, nrun, kstart, j0, j1, vdim, vhoff,
                head_size, qh0, kv_mul, seq_len, nsl, sl)
        } elif (vdt == KVDtype.tq4) {
            kv_slice_av(flop, attp, reinterpret<TQ4B const?>(vcl), runs, nrun, kstart, j0, j1, vdim, vhoff,
                head_size, qh0, kv_mul, seq_len, nsl, sl)
        } elif (vdt == KVDtype.f16) {
            kv_slice_av(flop, attp, reinterpret<uint16 const?>(vcl), runs, nrun, kstart, j0, j1, vdim, vhoff,
                head_size, qh0, kv_mul, seq_len, nsl, sl)
        } else {
            kv_slice_av(flop, attp, reinterpret<float const?>(vcl), runs, nrun, kstart, j0, j1, vdim, vhoff,
                head_size, qh0, kv_mul, seq_len, nsl, sl)
        }
    }
}

// The flash-decode witness body (see the call in attention_chain_decode): exact single-pass
// recompute of every head's attention output vs the sliced result in xb. Codec-generic like the
// workers; test-only and serial, so the extraction costs nothing.
def private flash_decode_check_g(qp : float const?; qq : int8 const?; qsp : float const?;
                                 kcp, vcp : auto const?; xbv : float const?;
                                 runs : KVRun const?;
                                 nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl : int64;
                                 scale, attn_softcap : float; sinksp : float const?;
                                 vsignsp : float const?) {   // tq4 V: un-rotate the recomputed head row
    var sref : array<float>
    sref |> resize(int(cnt))
    var vout : array<float>
    vout |> resize(int(head_size))
    unsafe {
        for (hh in range64(n_heads)) {
            let qoff = hh * head_size
            let khoff = kv_head_off(kcp, (hh / kv_mul) * head_size)
            let vhoff = kv_head_off(vcp, (hh / kv_mul) * head_size)
            for (r in range64(nrun)) {
                let rj0 = max(runs[r].j0, kstart)
                let rj1 = min(runs[r].j1, kstart + cnt)
                let rowbase = runs[r].row0 - runs[r].j0
                for (aj in range64(rj0, rj1)) {
                    let koff = (rowbase + aj) * kdim + khoff   // kcp is layer-base (codec seam)
                    let sc = kv_score(qp, qq, qsp, kcp, koff, qoff, head_size) * scale
                    sref[aj - kstart] = attn_softcap > 0.0 ? attn_softcap * tanh(sc / attn_softcap) : sc
                }
            }
            var m2 = sref[0]
            for (j in range64(1l, cnt)) {
                m2 = max(m2, sref[j])
            }
            if (sinksp != null) {
                m2 = max(m2, sinksp[hh])
            }
            // exp4 fold in place: sref becomes the exp weights (one pass, not one exp per (i, j))
            var l2 = 0.0
            {
                let m24 = float4(m2, m2, m2, m2)
                var s4 = float4(0.0)
                var j = 0l
                while (j + 4l <= cnt) {
                    let e = exp4(max(float4(sref[j], sref[j + 1l], sref[j + 2l], sref[j + 3l]) - m24, EXP4_LO))
                    sref[j] = e.x
                    sref[j + 1l] = e.y
                    sref[j + 2l] = e.z
                    sref[j + 3l] = e.w
                    s4 += e
                    j += 4l
                }
                l2 = s4.x + s4.y + s4.z + s4.w
                while (j < cnt) {
                    let e = exp(max(sref[j] - m2, -80.0))
                    sref[j] = e
                    l2 += e
                    j++
                }
            }
            if (sinksp != null) {
                l2 += exp(max(sinksp[hh] - m2, -80.0))
            }
            let invl2 = 1.0 / l2
            for (i in range64(head_size)) {
                var acc = 0.0
                for (r in range64(nrun)) {
                    let rj0 = max(runs[r].j0, kstart)
                    let rj1 = min(runs[r].j1, kstart + cnt)
                    let rowbase = runs[r].row0 - runs[r].j0
                    for (aj in range64(rj0, rj1)) {
                        let voff = (rowbase + aj) * vdim + vhoff   // vcp is layer-base (codec seam)
                        acc += sref[aj - kstart] * invl2 * kv_load(vcp, voff, i)
                    }
                }
                vout[i] = acc
            }
            if (vsignsp != null) {   // tq4 V: the sliced path un-rotated xb, so un-rotate here too
                fwht_unsign_row(addr(vout[0]), vsignsp, head_size)
            }
            var worstd = 0.0
            for (i in range64(head_size)) {
                worstd = max(worstd, abs(vout[i] - xbv[qoff + i]))
            }
            if (worstd > 1e-4) {
                panic("flash-decode check failed: layer {l} head {hh} worst |xb diff| {worstd} (cnt {cnt}, nsl {nsl})")
            }
        }
    }
    delete sref
    delete vout
}

// The witness' codec dispatch — the K×V matrix spelled once, like the worker wrappers.
def private flash_decode_check_d(qp : float const?; qq : int8 const?; qsp : float const?;
                                 kcl, vcl : uint8 const?; kdt, vdt : KVDtype; xbv : float const?;
                                 runs : KVRun const?;
                                 nrun, n_heads, kv_mul, head_size, kv_dim, kstart, cnt, l, nsl : int64;
                                 scale, attn_softcap : float; sinksp : float const?;
                                 vsignsp : float const?) {
    let kdim = kv_side_off(kdt, kv_dim)
    let vdim = kv_side_off(vdt, kv_dim)
    unsafe {
        if (kdt == KVDtype.q8_0) {
            if (vdt == KVDtype.q8_0) {
                flash_decode_check_g(qp, qq, qsp, kcl, vcl, xbv, runs,
                    nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
            } elif (vdt == KVDtype.tq4) {
                flash_decode_check_g(qp, qq, qsp, kcl, reinterpret<TQ4B const?>(vcl), xbv, runs,
                    nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
            } elif (vdt == KVDtype.f16) {
                flash_decode_check_g(qp, qq, qsp, kcl, reinterpret<uint16 const?>(vcl), xbv, runs,
                    nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
            } else {
                flash_decode_check_g(qp, qq, qsp, kcl, reinterpret<float const?>(vcl), xbv, runs,
                    nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
            }
        } elif (kdt == KVDtype.tq4) {
            if (vdt == KVDtype.q8_0) {
                flash_decode_check_g(qp, qq, qsp, reinterpret<TQ4B const?>(kcl), vcl, xbv, runs,
                    nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
            } elif (vdt == KVDtype.tq4) {
                flash_decode_check_g(qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<TQ4B const?>(vcl), xbv, runs,
                    nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
            } elif (vdt == KVDtype.f16) {
                flash_decode_check_g(qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<uint16 const?>(vcl), xbv, runs,
                    nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
            } else {
                flash_decode_check_g(qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<float const?>(vcl), xbv, runs,
                    nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
            }
        } elif (kdt == KVDtype.f16) {
            if (vdt == KVDtype.q8_0) {
                flash_decode_check_g(qp, qq, qsp, reinterpret<uint16 const?>(kcl), vcl, xbv, runs,
                    nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
            } elif (vdt == KVDtype.tq4) {
                flash_decode_check_g(qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<TQ4B const?>(vcl), xbv, runs,
                    nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
            } elif (vdt == KVDtype.f16) {
                flash_decode_check_g(qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<uint16 const?>(vcl), xbv, runs,
                    nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
            } else {
                flash_decode_check_g(qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<float const?>(vcl), xbv, runs,
                    nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
            }
        } elif (vdt == KVDtype.q8_0) {
            flash_decode_check_g(qp, qq, qsp, reinterpret<float const?>(kcl), vcl, xbv, runs,
                nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
        } elif (vdt == KVDtype.tq4) {
            flash_decode_check_g(qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<TQ4B const?>(vcl), xbv, runs,
                nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
        } elif (vdt == KVDtype.f16) {
            flash_decode_check_g(qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<uint16 const?>(vcl), xbv, runs,
                nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
        } else {
            flash_decode_check_g(qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<float const?>(vcl), xbv, runs,
                nrun, n_heads, kv_mul, head_size, kdim, vdim, kstart, cnt, l, nsl, scale, attn_softcap, sinksp, vsignsp)
        }
    }
}

// Store the roped k / raw v for all npos positions into the layer's KV cache. Each worker chunk
// walks the physical runs covering its positions, handing kv_store_row whole-row spans. Threaded
// over positions (workers own disjoint cache slices). Self-gates on g_kv_store_par_threshold.
def kv_store_batch(var kcp : uint8?; var vcp : uint8?;
                   kdt, vdt : KVDtype; k_b : array<float>; v_b : array<float>;
                   runs : KVRun const?; nrun, start_pos, npos, kv_dim : int64) {
    unsafe {
        let kbp = addr < float const? >(k_b[0])
        let vbp = addr < float const? >(v_b[0])
        maybe_parallel_for(kv_dim * npos >= g_kv_store_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (r in range64(nrun)) {
                    let o0 = max(runs[r].j0, start_pos + int64(rb))
                    let o1 = min(runs[r].j1, start_pos + int64(re))
                    if (o1 > o0) {
                        let rowbase = runs[r].row0 - runs[r].j0
                        kv_store_row(kcp, kdt, kv_dim, rowbase + o0, 0l, (o1 - o0) * kv_dim, kbp + (o0 - start_pos) * kv_dim)
                        kv_store_row(vcp, vdt, kv_dim, rowbase + o0, 0l, (o1 - o0) * kv_dim, vbp + (o0 - start_pos) * kv_dim)
                    }
                }
            }
        }
    }
}

// tq4 basis change over a batch buffer: rotate (or un-rotate) every head segment of every row in
// place — q_b rows at stride qd, k_b/v_b rows at stride kv_dim, xb_b rows at stride qd. Segments
// are independent; the cost is O(rows · stride · log head_size), noise next to the GEMMs.
def tq4_rotate_batch(var buf : array<float>; nrows, stride, head_size : int64; signs : float const?) {
    unsafe {
        var p = addr(buf[0])
        for (sg in range64(nrows * (stride / head_size))) {
            fwht_signs_row(p + sg * head_size, signs, head_size)
        }
    }
}
def private tq4_unrotate_batch(var buf : array<float>; nrows, stride, head_size : int64; signs : float const?) {
    unsafe {
        var p = addr(buf[0])
        for (sg in range64(nrows * (stride / head_size))) {
            fwht_unsign_row(p + sg * head_size, signs, head_size)
        }
    }
}

// tq4_unrotate_batch's raw-pointer twin — the Metal prefill continuation un-rotates gathered
// panel rows in place (the GPU attention runs in the raw basis)
def tq4_unrotate_rows(var p : float?; nrows, stride, head_size : int64; signs : float const?) {
    unsafe {
        for (sg in range64(nrows * (stride / head_size))) {
            fwht_unsign_row(p + sg * head_size, signs, head_size)
        }
    }
}

// Fused activation + gate multiply: hb[p] = act(W1·x[p]) * (W3·x[p]) in place, threaded per position.
// One memory pass, transcendental vectorized via exp4 unless g_act_vec is off (only non-bit-exact
// bit: exp4's ~2 ulp). Self-gates on g_act_par_threshold so small prompts/decode run inline.
def private gate_batch(var g : array<float>; u : array<float>; act : FfnAct; hidden, npos : int64) {
    unsafe {
        gate_batch(addr(g[0]), addr < float const? >(u[0]), act, hidden, npos)
    }
}

// pointer-level core — the fused gate+up dispatch acts on the halves of one groupn output buffer
def private gate_batch(var gp0 : float?; up0 : float const?; act : FfnAct; hidden, npos : int64) {
    let is_gelu = act == FfnAct.gelu
    let is_oai = act == FfnAct.swiglu_oai
    let vec = g_act_vec
    unsafe {
        var gp = gp0
        var up = up0
        maybe_parallel_for(hidden * npos >= g_act_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let off = int64(p) * hidden
                    if (vec) {
                        if (is_gelu) {
                            geglu4(gp + off, up + off, hidden)
                        } elif (is_oai) {
                            swiglu_oai4(gp + off, up + off, hidden)
                        } else {
                            swiglu4(gp + off, up + off, hidden)
                        }
                    } else {
                        if (is_gelu) {
                            geglu(gp + off, up + off, hidden)
                        } elif (is_oai) {
                            swiglu_oai(gp + off, up + off, hidden)
                        } else {
                            swiglu(gp + off, up + off, hidden)
                        }
                    }
                }
            }
        }
    }
}

// Add a projection bias (Qwen2): y[yoff + i] += b[boff + i]. yoff is 0 for the single-token forward,
// p*stride for the batched prefill; boff is the per-layer slice into bq/bk/bv.
[hint(unsafe_range_check)]
def private add_bias(var y : array<float>; yoff : int64; b : array<float>; boff, n : int64) {
    for (i in range64(n)) {
        y[yoff + i] += b[boff + i]
    }
}

// Q8 matmul: quantize the activation x to Q8 (into the run-state scratch xq/xs, shared across all d
// rows), then run the int8·int8 SDOT kernel against the int8 weights. woff is the weight-element
// offset; scales start at woff/32 (woff is block-aligned for every 2D tensor).
def private mm_at_q8(var y : array<float>; t : Model; woff : int64; x : array<float>; var xq : array<int8>; var xs : array<float>; n, d : int64) {
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    if (t.wscale_f16) {
        matmul_q8q8(y, t.qblob, t.qscales16, woff, xq, xs, n, d)
    } else {
        matmul_q8q8(y, t.qblob, t.qscales, woff, xq, xs, n, d)
    }
}

// Q4 matmul with the weight in the quantized blob (4-bit, two per byte) + its per-block scales.
// Bytes at woff/2 (woff is even), scales at woff/32 — the kernel's woff form does the math.
def private mm_at_q4(var y : array<float>; t : Model; woff : int64; x : array<float>; n, d : int64) {
    matmul_q4(y, t.q4blob, t.q4scales, woff, x, n, d)
}

// Fuse the attention Q/K/V projections into one parallel dispatch when Q8 and the combined matmul
// clears the parallel threshold: small K/V projections ride along on the worker pool, activation
// quantized once. Otherwise fall back to three separate matmuls.
def private mm_qkv(var s : Session; t : Model; l : int64) {
    let c = t.config
    let dim = c.dim
    let kv = layer_kv_dim(c, l)
    let qd = layer_qd(c, l)            // query width; == dim except Gemma2/gemma4
    let qoff = t.wq_offs[l]
    let koff = t.wk_offs[l]
    let voff = t.wv_offs[l]
    let fq = fmt_at(t.wq_fmt, l)
    let fk = fmt_at(t.wk_fmt, l)
    let fv = fmt_at(t.wv_fmt, l)
    if (voff < 0l) {
        // no attn_v tensor (gemma4 global layers): V = the pre-norm K projection, fused into the weightless V-norm
        mm_f(s.q, t, fq, qoff, s.xb, s.xq, s.xs, s.xbs, dim, qd)
        mm_f(s.k, t, fk, koff, s.xb, s.xq, s.xs, s.xbs, dim, kv)
        if (c.v_norm) {
            rms_batch(s.v, s.k, t, t.rms_ones_off, layer_head_size(c, l), layer_n_kv_heads(c, l))
        } else {
            copy_floats(s.k, 0l, s.v, 0l, kv)
        }
    } elif (fq != KqFmt.q8 || fk != KqFmt.q8 || fv != KqFmt.q8) {
        // K-quant projections (formats can MIX per layer, e.g. Q4_K_M keeps attn_v at Q6_K)
        if (fq == KqFmt.q8 || fk == KqFmt.q8 || fv == KqFmt.q8) {
            quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
        }
        quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)   // kq v2: every kq format
        mm_pre_f(s.q, t, fq, qoff, s.xq, s.xs, s.kxq, s.kxs, s.kxbs, dim, qd)
        mm_pre_f(s.k, t, fk, koff, s.xq, s.xs, s.kxq, s.kxs, s.kxbs, dim, kv)
        mm_pre_f(s.v, t, fv, voff, s.xq, s.xs, s.kxq, s.kxs, s.kxbs, dim, kv)
    } elif (t.quant == QuantMode.q8 && lanes_for_work(dim * (qd + 2l * kv), g_gemv_lane_cap) > 1) {
        quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
        if (t.wscale_f16) {
            matmul_q8q8_group3(s.q, s.k, s.v, t.qblob, t.qscales16, qoff, koff, voff, s.xq, s.xs, dim, qd, kv, kv)
        } else {
            matmul_q8q8_group3(s.q, s.k, s.v, t.qblob, t.qscales, qoff, koff, voff, s.xq, s.xs, dim, qd, kv, kv)
        }
    } else {
        mm(s.q, t, qoff, s.xb, s.xq, s.xs, dim, qd)
        mm(s.k, t, koff, s.xb, s.xq, s.xs, dim, kv)
        mm(s.v, t, voff, s.xb, s.xq, s.xs, dim, kv)
    }
}

// Q8 matmul against a PRE-quantized activation (xq/xs already hold x's Q8 image) — the MoE core
// quantizes the shared rmsnormed activation once per layer and reuses it across every expert's
// gate/up matmul instead of re-quantizing per call.
def private mm_at_q8_pre(var y : array<float>; t : Model; woff : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    if (t.wscale_f16) {
        matmul_q8q8(y, t.qblob, t.qscales16, woff, xq, xs, n, d)
    } else {
        matmul_q8q8(y, t.qblob, t.qscales, woff, xq, xs, n, d)
    }
}

// Native-MXFP4 GEMV against a PRE-quantized activation: the weight region [d x n] lives in the
// nibble + E8M0 planes (woff is the weight-element offset, like mm_at_q8_pre). Expert stacks only.
def private mm_at_mx4_pre(var y : array<float>; t : Model; woff : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    matmul_mx4q8(y, t.mxq, t.mxs, woff, xq, xs, n, d)
}

// Region-list GEMVs (the MoE decode dispatch fuse): all k routed experts' same-shape projections
// in ONE fork/join. offs packs (weight, activation) element-offset pairs; region j -> y[j*d..j*d+d).
// The boffs form folds each region's fblob bias into the workers' stores — bit-identical to add_bias.
def private mm_at_q8_groupn(var y : array<float>; t : Model; offs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    if (t.wscale_f16) {
        matmul_q8q8_groupn(y, t.qblob, t.qscales16, offs, nregions, xq, xs, n, d)
    } else {
        matmul_q8q8_groupn(y, t.qblob, t.qscales, offs, nregions, xq, xs, n, d)
    }
}

def private mm_at_q8_groupn(var y : array<float>; t : Model; offs, boffs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    if (t.wscale_f16) {
        matmul_q8q8_groupn(y, t.qblob, t.qscales16, offs, nregions, xq, xs, t.fblob, boffs, n, d)
    } else {
        matmul_q8q8_groupn(y, t.qblob, t.qscales, offs, nregions, xq, xs, t.fblob, boffs, n, d)
    }
}

def private mm_at_mx4_groupn(var y : array<float>; t : Model; offs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    matmul_mx4q8_groupn(y, t.mxq, t.mxs, offs, nregions, xq, xs, n, d)
}

def private mm_at_mx4_groupn(var y : array<float>; t : Model; offs, boffs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    matmul_mx4q8_groupn(y, t.mxq, t.mxs, offs, nregions, xq, xs, t.fblob, boffs, n, d)
}

// ===== native K-quant dispatch (kquant_native; fmt != q8 tags) =====

// KqFmt -> the kernel-layer format id (matmul_kq*/kq_rows_fn/kq_qsb take the int form). q8 maps
// to 0, a poison id no kq kernel accepts — kq_rows_for calls this for EVERY tag, so it must stay
// total even though q8 branches never read the result.
def private kq_fi(fmt : KqFmt) : int => fmt == KqFmt.k4 ? 4 : (fmt == KqFmt.k5 ? 5 : (fmt == KqFmt.k6 ? 6 : (fmt == KqFmt.q40 ? 40 : 0)))

// K-quant GEMV against a PRE-quantized activation (xq/xs/xbs already filled by the bs quantizer).
// kq_repacked loads run the backend's stamped cores; disk-order loads run the portable kernels.
def private mm_at_kq_pre(var y : array<float>; t : Model; fmt : KqFmt; woff : int64; xq : array<int8>; xs : array<float>; xbs : array<int>; n, d : int64; yoff : int64 = 0l) {
    if (t.kq_repacked) {
        if (fmt == KqFmt.k4) {
            matmul_kq_active(4, y, t.k4q, t.k4s, woff, xq, xs, xbs, n, d, yoff)
        } elif (fmt == KqFmt.k5) {
            matmul_kq_active(5, y, t.k5q, t.k5s, woff, xq, xs, xbs, n, d, yoff)
        } elif (fmt == KqFmt.k6) {
            matmul_kq_active(6, y, t.k6q, t.k6s, woff, xq, xs, xbs, n, d, yoff)
        } elif (fmt == KqFmt.q40) {
            matmul_kq_active(40, y, t.q40q, t.q40s, woff, xq, xs, xbs, n, d, yoff)
        } else {
            panic("mm_at_kq_pre: '{fmt}' has no kq plane pair")   // a new KqFmt must claim its arm, never ride q40's
        }
    } elif (fmt == KqFmt.k4) {
        matmul_kq(4, y, t.k4q, t.k4s, woff, xq, xs, xbs, n, d, yoff)
    } elif (fmt == KqFmt.k5) {
        matmul_kq(5, y, t.k5q, t.k5s, woff, xq, xs, xbs, n, d, yoff)
    } elif (fmt == KqFmt.k6) {
        matmul_kq(6, y, t.k6q, t.k6s, woff, xq, xs, xbs, n, d, yoff)
    } elif (fmt == KqFmt.q40) {
        matmul_kq(40, y, t.q40q, t.q40s, woff, xq, xs, xbs, n, d, yoff)
    } else {
        panic("mm_at_kq_pre: '{fmt}' has no kq plane pair")
    }
}

// K-quant matmul: Q8_K-quantize the activation (quants + per-256 scales + the per-16 sums the
// fold consumes), then the format's plane GEMV. The kq sibling of mm/mm_at_q8.
def private mm_kq(var y : array<float>; t : Model; fmt : KqFmt; woff : int64; x : array<float>; var xq : array<int8>; var xs : array<float>; var xbs : array<int>; n, d : int64) {
    quantize_q8_k_into(x, n, xq, xs, xbs, 0l, 0l, 0l)
    mm_at_kq_pre(y, t, fmt, woff, xq, xs, xbs, n, d)
}

// Batched K-quant matmul. Repacked loads with a kq batch kernel run the native tile: one threaded
// bs-requant, then ONE weight-stationary GEMM (bit-for-bit npos x the per-position GEMV).
// Otherwise positions run serially, each bs-quantized then a full row GEMV.
def private mm_b_kq(var s : Session; var y : array<float>; t : Model; fmt : KqFmt; woff : int64; x : array<float>; n, d, npos : int64) {
    if (t.kq_repacked && kernel_backend_has_kq_batch()) {
        requant_rows_q8k_bs(x, n, npos, s.xqb, s.xsb, s.xbsb)   // kq v2: Q8_K-form image
        if (npos >= 32l && moe_gpu_dense_on_gpu(int(fmt), woff)) {
            matmul_gpu_dense(y, woff, s.xqb, s.xsb, n, d, npos, int(fmt))   // in-kernel bsums, xbsb unused
            return
        }
        let ts_gemm = ref_time_ticks()
        if (fmt == KqFmt.k4) {
            matmul_kq_batch(4, y, t.k4q, t.k4s, woff, s.xqb, s.xsb, s.xbsb, n, d, npos)
        } elif (fmt == KqFmt.k5) {
            matmul_kq_batch(5, y, t.k5q, t.k5s, woff, s.xqb, s.xsb, s.xbsb, n, d, npos)
        } elif (fmt == KqFmt.k6) {
            matmul_kq_batch(6, y, t.k6q, t.k6s, woff, s.xqb, s.xsb, s.xbsb, n, d, npos)
        } elif (fmt == KqFmt.q40) {
            matmul_kq_batch(40, y, t.q40q, t.q40s, woff, s.xqb, s.xsb, s.xbsb, n, d, npos)
        } else {
            panic("mm_b_kq: '{fmt}' has no kq plane pair")
        }
        prof_add("mm_gemm", ts_gemm)
        return
    }
    unsafe {
        for (p in range64(npos)) {
            quantize_q8_k_into_ptr(addr(x[p * n]), n, addr(s.xq[0]), addr(s.xs[0]), addr(s.xbs[0]), 0l, 0l, 0l)
            mm_at_kq_pre(y, t, fmt, woff, s.xq, s.xs, s.xbs, n, d, p * d)
        }
    }
}

// Region-list kq GEMV over the model's planes (the MoE expert dispatch's kq twin of
// mm_at_q8_groupn; offs = (weight, activation) element-offset pairs within fmt's plane).
// Repacked loads run the backend's kq_groupn; disk-order loads run the portable region walk.
def private mm_at_kq_groupn(var y : array<float>; t : Model; fmt : KqFmt; offs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; xbs : array<int>; n, d : int64) {
    if (t.kq_repacked) {
        if (fmt == KqFmt.k4) {
            matmul_kq_groupn_active(4, y, t.k4q, t.k4s, offs, nregions, xq, xs, xbs, n, d)
        } elif (fmt == KqFmt.k5) {
            matmul_kq_groupn_active(5, y, t.k5q, t.k5s, offs, nregions, xq, xs, xbs, n, d)
        } elif (fmt == KqFmt.k6) {
            matmul_kq_groupn_active(6, y, t.k6q, t.k6s, offs, nregions, xq, xs, xbs, n, d)
        } elif (fmt == KqFmt.q40) {
            matmul_kq_groupn_active(40, y, t.q40q, t.q40s, offs, nregions, xq, xs, xbs, n, d)
        } else {
            panic("mm_at_kq_groupn: '{fmt}' has no kq plane pair")
        }
    } elif (fmt == KqFmt.k4) {
        matmul_kq_groupn(4, y, t.k4q, t.k4s, offs, nregions, xq, xs, xbs, n, d)
    } elif (fmt == KqFmt.k5) {
        matmul_kq_groupn(5, y, t.k5q, t.k5s, offs, nregions, xq, xs, xbs, n, d)
    } elif (fmt == KqFmt.k6) {
        matmul_kq_groupn(6, y, t.k6q, t.k6s, offs, nregions, xq, xs, xbs, n, d)
    } elif (fmt == KqFmt.q40) {
        matmul_kq_groupn(40, y, t.q40q, t.q40s, offs, nregions, xq, xs, xbs, n, d)
    } else {
        panic("mm_at_kq_groupn: '{fmt}' has no kq plane pair")
    }
}

// Fused batched region-list kq GEMM (the fused MoE prefill's kq twin of the mx4 batch groupn;
// triple offs). Callers guard with kq_repacked && kernel_backend_has_kq_batch_groupn().
def private mm_b_kq_groupn(var y : array<float>; t : Model; fmt : KqFmt; offs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; xbs : array<int>; n, d : int64) {
    if (fmt == KqFmt.k4) {
        matmul_kq_batch_groupn(4, y, t.k4q, t.k4s, offs, nregions, xq, xs, xbs, n, d)
    } elif (fmt == KqFmt.k5) {
        matmul_kq_batch_groupn(5, y, t.k5q, t.k5s, offs, nregions, xq, xs, xbs, n, d)
    } elif (fmt == KqFmt.k6) {
        matmul_kq_batch_groupn(6, y, t.k6q, t.k6s, offs, nregions, xq, xs, xbs, n, d)
    } elif (fmt == KqFmt.q40) {
        matmul_kq_batch_groupn(40, y, t.q40q, t.q40s, offs, nregions, xq, xs, xbs, n, d)
    } else {
        panic("mm_b_kq_groupn: '{fmt}' has no kq plane pair")
    }
}

// Batched kq GEMM off PRE-quantized activation rows (the grouped MoE prefill's kq twin of
// mm_b_q8_pre). Callers guard with kq_repacked && kernel_backend_has_kq_batch() — the prefill
// gate routes kq-expert layers to the per-position reference path otherwise.
def private mm_b_kq_pre(var y : array<float>; t : Model; fmt : KqFmt; woff : int64; xq : array<int8>; xs : array<float>; xbs : array<int>; n, d, ntok : int64) {
    if (fmt == KqFmt.k4) {
        matmul_kq_batch(4, y, t.k4q, t.k4s, woff, xq, xs, xbs, n, d, ntok)
    } elif (fmt == KqFmt.k5) {
        matmul_kq_batch(5, y, t.k5q, t.k5s, woff, xq, xs, xbs, n, d, ntok)
    } elif (fmt == KqFmt.k6) {
        matmul_kq_batch(6, y, t.k6q, t.k6s, woff, xq, xs, xbs, n, d, ntok)
    } elif (fmt == KqFmt.q40) {
        matmul_kq_batch(40, y, t.q40q, t.q40s, woff, xq, xs, xbs, n, d, ntok)
    } else {
        panic("mm_b_kq_pre: '{fmt}' has no kq plane pair")
    }
}

def private kq_plane_q(t : Model; fmt : KqFmt; woff : int64) : uint8 const? {
    let sb = woff / 256l
    if (fmt == KqFmt.k4) return unsafe(addr<uint8 const?>(t.k4q[sb * K4_QSB]))
    if (fmt == KqFmt.k5) return unsafe(addr<uint8 const?>(t.k5q[sb * K5_QSB]))
    if (fmt == KqFmt.k6) return unsafe(addr<uint8 const?>(t.k6q[sb * K6_QSB]))
    if (fmt == KqFmt.q40) return unsafe(addr<uint8 const?>(t.q40q[sb * Q40_QSB]))
    return null
}

def private kq_plane_s(t : Model; fmt : KqFmt; woff : int64) : uint8 const? {
    let sb = woff / 256l
    if (fmt == KqFmt.k4) return unsafe(addr<uint8 const?>(t.k4s[sb * K4_SSB]))
    if (fmt == KqFmt.k5) return unsafe(addr<uint8 const?>(t.k5s[sb * K5_SSB]))
    if (fmt == KqFmt.k6) return unsafe(addr<uint8 const?>(t.k6s[sb * K6_SSB]))
    if (fmt == KqFmt.q40) return unsafe(addr<uint8 const?>(t.q40s[sb * Q40_SSB]))
    return null
}

def private kq_rows_for(fmt : KqFmt) : MatmulKqRowsFn {
    return kq_rows_fn(kq_fi(fmt))
}

// Can the fused chains serve a weight of this format? q8 always; kq only off a repacked load
// (the backend rows cores read the grp layout the loader wrote).
def private kq_servable(t : Model; fmt : KqFmt) : bool => fmt == KqFmt.q8 || t.kq_repacked

// kq v2 activation form: every kq format reads Q8_K-form activations (per-256 scale + per-16
// bsums). The MoE gate/up pair shares ONE gathered/quantized activation plane, so its two
// stacks must agree on the form: both kq (Q8_K) or both q8 (Q8_0).
def private moe_pair_kform(f1, f3 : KqFmt) : bool {
    let k1 = f1 != KqFmt.q8
    let k3 = f3 != KqFmt.q8
    if (k1 != k3) {
        panic("dasLLAMA: MoE gate/up expert stacks mix q8 and K-quant weights — unsupported until per-stack activation planes land")
    }
    return k1
}

// Format router: q8-tagged weights take the classic mm; kq tags take the plane GEMV.
def private mm_f(var y : array<float>; t : Model; fmt : KqFmt; woff : int64; x : array<float>; var xq : array<int8>; var xs : array<float>; var xbs : array<int>; n, d : int64) {
    if (fmt == KqFmt.q8) {
        mm(y, t, woff, x, xq, xs, n, d)
    } else {
        mm_kq(y, t, fmt, woff, x, xq, xs, xbs, n, d)
    }
}

// Pre-quantized format router: q8-tagged weights consume the Q8_0-form scratch (xq/xs);
// kq-tagged weights the Q8_K-form scratch (kxq/kxs/kxbs — kq v2, all three formats).
def private mm_pre_f(var y : array<float>; t : Model; fmt : KqFmt; woff : int64; xq : array<int8>; xs : array<float>; kxq : array<int8>; kxs : array<float>; kxbs : array<int>; n, d : int64) {
    if (fmt == KqFmt.q8) {
        mm_at_q8_pre(y, t, woff, xq, xs, n, d)
    } else {
        mm_at_kq_pre(y, t, fmt, woff, kxq, kxs, kxbs, n, d)
    }
}

// Batched format router: q8 weights ride the batch tile; kq weights take the native kq tile
// (repacked loads) or the serial-per-position fallback (see mm_b_kq).
def private mm_b_f(var s : Session; var y : array<float>; t : Model; fmt : KqFmt; woff : int64; x : array<float>; n, d, npos : int64) {
    if (fmt == KqFmt.q8) {
        mm_b(y, t, woff, x, s.xqb, s.xsb, n, d, npos)
    } else {
        mm_b_kq(s, y, t, fmt, woff, x, n, d, npos)
    }
}

// Route a big-weight matmul through the Q8, Q4, or fp32 kernel depending on the model's quant mode.
// xq/xs are the run-state activation-quant scratch (used only by the Q8 path).
def private mm(var y : array<float>; t : Model; woff : int64; x : array<float>; var xq : array<int8>; var xs : array<float>; n, d : int64) {
    if (t.quant == QuantMode.q8) {
        mm_at_q8(y, t, woff, x, xq, xs, n, d)
    } elif (t.quant == QuantMode.q4_0) {
        mm_at_q4(y, t, woff, x, n, d)
    } else {
        mm_at(y, t, woff, x, n, d)
    }
}

// Quantize the npos rows of a batched activation to Q8 (row p at x[p*n..] -> xqb[p*n..]) —
// independent per position, so thread it (was the largest serial bucket once attention + GEMM
// threaded). Same per-row quants as the single-row quantize_q8_0_into.
def private requant_rows_q8(x : array<float>; n, npos : int64; var xqb : array<int8>; var xsb : array<float>) {
    let nb = n / 32l
    let ts_rq = ref_time_ticks()
    unsafe {
        let xp = addr < float const? >(x[0])
        var qp = addr(xqb[0])
        var sp = addr(xsb[0])
        let rq_par = n * npos >= g_requant_par_threshold
        maybe_parallel_for(rq_par, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    quantize_q8_0_into_ptr(xp + pp * n, n, qp, sp, pp * n, pp * nb)
                }
            }
        }
    }
    prof_add("mm_requant", ts_rq)
}

// Q8_K-form batch requant (kq v2): per-256 scales + per-16 sums of the per-256-scaled
// quants. k4-weight batch matmuls and MoE gathers consume these rows.
def private requant_rows_q8k_bs(x : array<float>; n, npos : int64; var xqb : array<int8>; var xsb : array<float>; var xbsb : array<int>) {
    let nsb = n / 256l
    let nh = n / 16l
    let ts_rq = ref_time_ticks()
    unsafe {
        let xp = addr < float const? >(x[0])
        var qp = addr(xqb[0])
        var sp = addr(xsb[0])
        var bp = addr(xbsb[0])
        let rq_par = n * npos >= g_requant_par_threshold
        maybe_parallel_for(rq_par, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    quantize_q8_k_into_ptr(xp + pp * n, n, qp, sp, bp, pp * n, pp * nsb, pp * nh)
                }
            }
        }
    }
    prof_add("mm_requant", ts_rq)
}

// Batched native-MXFP4 GEMM against a PRE-quantized activation image (the batch analog of
// mm_at_mx4_pre): grouped MoE prefill dispatches here — no Q8 expansion — when the active backend
// carries a native mx4 batch kernel (g_active_mx4_native_batch).
def private mm_b_mx4_pre(var y : array<float>; t : Model; woff : int64; xqb : array<int8>; xsb : array<float>; n, d, npos : int64) {
    let ts_gemm = ref_time_ticks()
    matmul_mx4q8_batch(y, t.mxq, t.mxs, woff, xqb, xsb, n, d, npos)
    prof_add("mm_gemm", ts_gemm)
}

// Batched Q8 GEMM against a PRE-quantized activation image (the batch analog of mm_at_q8_pre) —
// the grouped MoE prefill runs many GEMMs off one requant (the whole-batch image or a gathered
// expert bucket).
def private mm_b_q8_pre(var y : array<float>; t : Model; woff : int64; xqb : array<int8>; xsb : array<float>; n, d, npos : int64) {
    let ts_gemm = ref_time_ticks()
    if (t.wscale_f16) {
        matmul_q8q8_batch(y, t.qblob, t.qscales16, woff, xqb, xsb, n, d, npos)
    } else {
        matmul_q8q8_batch(y, t.qblob, t.qscales, woff, xqb, xsb, n, d, npos)
    }
    prof_add("mm_gemm", ts_gemm)
}

// Batched big-weight matmul for prefill: Y [npos x d] from X [npos x n], via the slice-1 GEMM
// kernels. Q8 quantizes each of the npos activation columns into the batched xqb/xsb scratch first;
// fp32 goes straight through. Q4 has no batched kernel, so forward_prefill never routes it here.
def private mm_b(var y : array<float>; t : Model; woff : int64; x : array<float>; var xqb : array<int8>; var xsb : array<float>; n, d, npos : int64) {
    if (t.quant == QuantMode.q8) {
        requant_rows_q8(x, n, npos, xqb, xsb)
        if (npos >= 32l && moe_gpu_dense_on_gpu(int(KqFmt.q8), woff)) {
            matmul_gpu_dense(y, woff, xqb, xsb, n, d, npos, int(KqFmt.q8))
        } else {
            mm_b_q8_pre(y, t, woff, xqb, xsb, n, d, npos)
        }
    } else {
        matmul_batch(y, t.wblob, woff, x, n, d, npos)
    }
}

//! Per-layer attention pattern: with SWA on, an explicit per-layer mask (gemma4's GGUF bool array)
//! wins; otherwise every swa_pattern-th layer (l % p == p-1) is global and the rest slide — Gemma2
//! alternates (p=2, even layers slide), Gemma3 runs 5 local : 1 global (p=6).
def layer_is_sliding(c : Config; l : int64) : bool => (c.sliding_window > 0l && l < c.n_layers
    && (c.swa_mask != 0ul ? ((c.swa_mask >> uint64(l)) & 1ul) != 0ul : l % c.swa_pattern != c.swa_pattern - 1l))

// ===== Per-layer attention geometry (gemma4: sliding vs global layer classes differ) =====
// head_size/n_kv_heads/kv_dim in Config describe the GLOBAL class; the *_swa fields (0 = same)
// override for sliding layers. Everything downstream sizes scratch to the class maxima.

//! Does this model carry per-layer embeddings (gemma-4 E-series)?
def has_ple(c : Config) : bool => c.n_embd_per_layer > 0l

//! Layer `l` runs the Gated-DeltaNet recurrent block instead of attention (qwen35 hybrid).
// the l < n_layers guard is load-bearing: the MTP/NextN block sits at index n_layers, which on
// a 64-trunk-layer model (27B) means a shift by 64 — x86 wraps that to bit 0 (a recurrent layer)
def layer_is_recurrent(c : Config; l : int64) : bool => l < c.n_layers && ((c.recr_mask >> uint64(l)) & 1ul) != 0ul

//! deltanet q/k projection width (n_group heads of d_state each).
def dn_key_dim(c : Config) : int64 => c.ssm_n_group * c.ssm_d_state

//! deltanet fused-projection / conv channel count: [q (key_dim) ; k (key_dim) ; v (d_inner)].
def dn_conv_dim(c : Config) : int64 => 2l * dn_key_dim(c) + c.ssm_d_inner

// Chunk length for the blocked (chunked) deltanet prefill — llama.cpp's CS for gated deltanet.
let DN_CHUNK = 64l

// Per-v-head workspace floats for the chunked deltanet prefill: eight CS x d_state panels
// (q / kT / k_beta / v_beta / w / v_new / o / kgT), two CS x CS triangles (T, KQ), and three
// per-chunk scalar rows (g cumsum, e^{gcum}, e^{g_last - gcum}).
def private dn_cws_head_size(c : Config) : int64 => (8l * DN_CHUNK * c.ssm_d_state
    + 2l * DN_CHUNK * DN_CHUNK + 3l * DN_CHUNK)

//! Layer `l`'s dense FFN width. Uniform config.hidden_dim unless the model varies it per layer
//! (gemma-4 E2B: 6144 for layers 0-14, 12288 for 15-34).
def layer_hidden(t : Model; l : int64) : int64 => (empty(t.ffn_w) ? t.config.hidden_dim : t.ffn_w[l])

//! Layer `l`'s KV source (mirrors llama.cpp gemma4 reuse()): own-KV layers map to themselves; a
//! shared layer (l >= n_layer_kv_from_start) reuses the last own-KV layer of its class — sliding
//! reads kv_from_start-2, global reads kv_from_start-1 (those two straddle the boundary by pattern).
def layer_kv_source(c : Config; l : int64) : int64 {
    if (l >= c.n_layers) {   // the MTP/NextN draft block never shares KV (no arch combines kv_from_start with nextn today — fail safe if one ever does)
        return l
    }
    let kvfs = c.n_layer_kv_from_start
    if (kvfs <= 0l || l < kvfs) {
        return l
    }
    return kvfs - (layer_is_sliding(c, l) ? 2l : 1l)
}

//! Layer `l`'s attention head size.
def layer_head_size(c : Config; l : int64) : int64 => (layer_is_sliding(c, l) && c.head_size_swa > 0l
    ? c.head_size_swa : c.head_size)

//! Layer `l`'s KV-head count. Recurrent (deltanet) layers have NO KV — their 0 makes every
//! KV-cache prefix walk (session slabs, pool blobs, q8 validation) allocate zero bytes for them.
def layer_n_kv_heads(c : Config; l : int64) : int64 => (layer_is_recurrent(c, l) ? 0l
    : (layer_is_sliding(c, l) && c.n_kv_heads_swa > 0l ? c.n_kv_heads_swa : c.n_kv_heads))

//! Layer `l`'s KV width (kv heads × head size).
def layer_kv_dim(c : Config; l : int64) : int64 => layer_n_kv_heads(c, l) * layer_head_size(c, l)

//! Layer `l`'s query / attention-output width (n_heads × head size).
def layer_qd(c : Config; l : int64) : int64 => c.n_heads * layer_head_size(c, l)

//! The larger head size across the two layer classes (scratch sizing).
def max_head_size(c : Config) : int64 => max(c.head_size, c.head_size_swa)

//! The larger query width across the two layer classes (scratch sizing).
def max_qd(c : Config) : int64 => c.n_heads * max_head_size(c)

//! The widest FFN intermediate — dense hidden, per-expert, or shared-expert (scratch sizing).
def max_ffn_width(c : Config) : int64 => max(c.hidden_dim, max(c.n_ff_exp, c.n_ff_shexp))

//! The larger KV width across the two layer classes (scratch sizing).
def max_kv_dim(c : Config) : int64 => (max(c.n_kv_heads * c.head_size,
    (c.n_kv_heads_swa > 0l ? c.n_kv_heads_swa : c.n_kv_heads) * (c.head_size_swa > 0l ? c.head_size_swa : c.head_size)))

//! Total KV width of one position across all layers (the KV cache holds seq_len of these).
def kv_row_total(c : Config) : int64 {
    var total = 0l
    for (l in range64(c.n_layers)) {
        total += layer_kv_dim(c, l)
    }
    return total
}

//! Layer `l`'s base offset into the KV cache. Reads seq_len LIVE (tests cap it after load), so this
//! is a function of Config, never baked into the Model at load. The definitional walking form; the
//! hot paths use the O(1) Model overload below (same value via the load-time prefix array).
def kv_cache_off(c : Config; l : int64) : int64 {
    var row = 0l
    for (i in range64(l)) {
        row += layer_kv_dim(c, i)
    }
    return row * c.seq_len
}

//! O(1) kv_cache_off: the per-layer KV-ROW prefix is filled by layout_offsets at load
//! (seq_len-independent), multiplied by the LIVE seq_len here so post-load seq_len caps keep working.
def kv_cache_off(t : Model; l : int64) : int64 => t.kv_row_prefix[l] * t.config.seq_len

//! True when sliding layers rope differently than global layers — different θ/position scale
//! (Gemma3) or head size (gemma4: 256 vs 512). False for uniform-rope SWA models (Gemma2), since
//! the loader resolves swa fields to global values when the arch doesn't set them.
def has_dual_rope(c : Config) : bool => (c.sliding_window > 0l
    && (c.rope_freq_base_swa != c.rope_freq_base || c.rope_freq_scale_swa != c.rope_freq_scale
        || (c.head_size_swa > 0l && c.head_size_swa != c.head_size)))

//! One forward pass: token at position `pos` -> s.logits. Faithful port of llama2.c forward().
// ===== std/dense block kernels (decode, single-token) =====
// The default llama-family blocks; a new arch registers a different block without touching forward().

// Flash-decode slicing (fused attention chain): a context deeper than FLASH_DECODE_SLICE splits
// each head's KV pass into ceil(cnt/SLICE_LEN) slices with a per-head combine stage. Slice count
// derives from CONTEXT ONLY (deterministic); ctx <= 256 stays bit-identical, deeper is tolerance-equal.
let FLASH_DECODE_SLICE = 256l       // single-pass threshold: cnt <= this never slices
let FLASH_DECODE_SLICE_LEN = 32l    // slice granularity once slicing engages
let FLASH_DECODE_MAX_SLICES = 32l   // 4 chunks/lane at 16L: Qwen2.5-1.5B d4k attn wall 393->333us/layer vs cap 16

// attn-norm -> QKV(+bias) -> RoPE -> KV-store -> GQA attention -> out-proj(+post-norm) -> residual.
// Can layer l's attention run as the fused chain? Std shape only: Q8 weights, group3 layout, table
// rope, team mode + a backend row core. pre_post_norm defers the residual add to a serial tail.
def private fused_attn_ok(t : Model; l : int64) : bool {
    let c = t.config
    let fq = fmt_at(t.wq_fmt, l)
    let fk = fmt_at(t.wk_fmt, l)
    let fv = fmt_at(t.wv_fmt, l)
    let fo = fmt_at(t.wo_fmt, l)
    let any_kq = fq != KqFmt.q8 || fk != KqFmt.q8 || fv != KqFmt.q8 || fo != KqFmt.q8
    let head_size = layer_head_size(c, l)
    let qd = layer_qd(c, l)
    // head_size % 32: the wo-input requant needs whole 32-quant groups per head (stories15M's 48 falls back)
    return (g_fused_decode && t.quant == QuantMode.q8 && g_use_rope_table
        && t.wv_offs[l] >= 0l && head_size % 32l == 0l
        // partial rotary (glm4moe): fall back to attention_std_decode, which has the rot-aware leaves
        && (c.rope_dim == 0l || c.rope_dim == head_size)
        && kq_servable(t, fq) && kq_servable(t, fk) && kq_servable(t, fv) && kq_servable(t, fo)
        // kq v2 regrain: Q8_K quantizes whole 256-superblocks, so qd and the head grouping must divide out
        && (!any_kq || c.dim % 256l == 0l)
        && (fo == KqFmt.q8 || (qd % 256l == 0l && (head_size >= 256l ? head_size % 256l == 0l : 256l % head_size == 0l)))
        && get_jobque_team_mode() && kernel_backend_has_rows()
        && (!t.wscale_f16 || kernel_backend_has_rows16())
        && lanes_for_work(c.dim * (qd + 2l * layer_kv_dim(c, l)), g_gemv_lane_cap) > 1)
}

// Fused attention block: ONE team rendezvous runs [qkv rows] -> [head-slot norm/rope/kv-store] ->
// [attn heads + wo-input requant] -> [wo rows + residual add] as four stages. Bit-identical to
// attention_std_decode at ctx <= FLASH_DECODE_SLICE; deeper contexts are tolerance-equal, not bit-equal.
def private attention_chain_decode(t : Model; var s : Session; l : int64; pos : int64) {
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let is_sliding = layer_is_sliding(c, l)
    let kstart = (is_sliding && pos + 1l > c.sliding_window) ? (pos + 1l - c.sliding_window) : 0l
    let cnt = pos + 1l - kstart
    let nrun = kv_runs(s, kstart, pos + 1l, s.kv_runs)   // physical runs of this layer's window (flat: 1)
    let prow = kv_phys_row(s, pos)                       // where THIS position's row lives
    // per-weight formats (kq layers reach here only off a repacked load — fused_attn_ok)
    let fq = fmt_at(t.wq_fmt, l)
    let fk = fmt_at(t.wk_fmt, l)
    let fv = fmt_at(t.wv_fmt, l)
    let fo = fmt_at(t.wo_fmt, l)
    let any_kq = fq != KqFmt.q8 || fk != KqFmt.q8 || fv != KqFmt.q8 || fo != KqFmt.q8
    // attention rmsnorm + input quantize: one image per consumed form — q8 reads Q8_0, kq reads Q8_K (kq v2)
    let ts_norm0 = ref_time_ticks()
    rms_at(s.xb, t, t.rms_att_off + l * dim, s.x, dim)
    if (fq == KqFmt.q8 || fk == KqFmt.q8 || fv == KqFmt.q8) {
        quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
    }
    if (fq != KqFmt.q8 || fk != KqFmt.q8 || fv != KqFmt.q8) {
        quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
    }
    prof_add("norm", ts_norm0)
    let ts_chain = ref_time_ticks()
    trace_tag(TRACE_TAG_ATTN_CHAIN)
    // stage geometry (same grain math as the per-op dispatches; the C++ side clamps chunks <= range)
    let nslots = int(n_heads + 2l * n_kv_heads)
    let ngq = int((qd + 2l * kv_dim) / 32l)   // stage-0 domain: 32-row groups (head_size % 32 == 0 by the fuse gate)
    let n0 = chain_stage_chunks(ngq, int(qd + 2l * kv_dim), dim * (qd + 2l * kv_dim))
    let n1 = get_dispatch_lanes()
    let ngrp = int(dim / 32l)
    let n2 = chain_stage_chunks(ngrp, int(dim), qd * dim)
    // attn-stage unit: a kq wo requants the output per HEAD GROUP spanning whole 256-superblocks
    let hpu = (fo != KqFmt.q8 && head_size < 256l) ? 256l / head_size : 1l
    let nhu = int(n_heads / hpu)
    // flash-decode slicing engages once cnt crosses FLASH_DECODE_SLICE (ctx-derived, deterministic); nsl == 1 keeps the exact single-pass body
    let nsl = cnt <= FLASH_DECODE_SLICE ? 1 : int(min(FLASH_DECODE_MAX_SLICES, (cnt + FLASH_DECODE_SLICE_LEN - 1l) / FLASH_DECODE_SLICE_LEN))
    let slice_len = (cnt + int64(nsl) - 1l) / int64(nsl)
    if (nsl > 1) {
        let need = int(n_heads) * nsl
        if (length(s.fl_m) < need) {
            s.fl_m |> resize(need)
            s.fl_l |> resize(need)
        }
        let needo = int(n_heads * int64(nsl) * head_size)
        if (length(s.fl_o) < needo) {
            s.fl_o |> resize(needo)
        }
        s.chain_stages |> resize(5)
        s.chain_stages[0] = int3(0, ngq, n0)
        s.chain_stages[1] = int3(0, nslots, nslots)
        s.chain_stages[2] = int3(0, int(n_kv_heads) * nsl, int(n_kv_heads) * nsl)
        s.chain_stages[3] = int3(0, nhu, nhu)
        s.chain_stages[4] = int3(0, ngrp, n2)
        count_parallel_dispatch(min(n0, ngq) + nslots + int(n_kv_heads) * nsl + nhu + min(n2, ngrp))
    } else {
        s.chain_stages |> resize(4)
        s.chain_stages[0] = int3(0, ngq, n0)
        s.chain_stages[1] = int3(0, nslots, nslots)
        s.chain_stages[2] = int3(0, nhu, n1)
        s.chain_stages[3] = int3(0, ngrp, n2)
        count_parallel_dispatch(min(n0, ngq) + nslots + min(n1, nhu) + min(n2, ngrp))
    }
    let mmrows = mm_q8q8_rows_fn()
    let neox = c.rope_neox
    let eps = c.norm_eps
    let seq_len = c.seq_len
    let attn_softcap = c.attn_logit_softcap
    let kdt = s.kv_dtype_k
    let vdt = s.kv_dtype_v
    // Gemma2/3 post-attention norm is a whole-vector reduction, so the residual add lands serially after
    let post_norm = c.pre_post_norm
    unsafe {
        // activations / scratch — raw pointers only (the stage lambda runs in fork contexts)
        var qp = addr(s.q[0])
        var kp = addr(s.k[0])
        var vp = addr(s.v[0])
        var xbp = addr(s.xb[0])
        var xb2p = addr(s.xb2[0])
        var xp = addr(s.x[0])
        var attp = addr(s.att[0])
        var xqp = addr(s.xq[0])
        var xsp = addr(s.xs[0])
        // block-codec K: each stage-0 q slot fills its quantized-query plane after rope; tq4 rotates first
        let kq8 = kdt == KVDtype.q8_0 || kdt == KVDtype.tq4
        let ktq4 = kdt == KVDtype.tq4
        let vtq4 = vdt == KVDtype.tq4
        let sgp = (ktq4 || vtq4) ? addr < float const? >(s.kv_signs[0]) : null
        var attqp = kq8 ? addr(s.attq[0]) : null
        var attsp = kq8 ? addr(s.atts[0]) : null
        var kcl = kv_layer_kbase(s, l, c.seq_len)  // layer-base byte pointers (stores + the _d read dispatch)
        var vcl = kv_layer_vbase(s, l, c.seq_len)
        let runsp = addr < KVRun const? >(s.kv_runs[0])  // this layer's physical runs (fork-safe hoist)
        // weight region bases (element offsets into the Q8 blob); all-kq layers skip q8 bases (may be EMPTY)
        let f16w = t.wscale_f16
        let mmrows16 = mm_q8q8_rows16_fn()
        let has_q8w = fq == KqFmt.q8 || fk == KqFmt.q8 || fv == KqFmt.q8 || fo == KqFmt.q8
        let wqb = has_q8w ? addr < int8 const? >(t.qblob[0]) : null
        var wsq : float const? = null
        var wsk : float const? = null
        var wsv : float const? = null
        var wso : float const? = null
        var wsq16 : uint16 const? = null
        var wsk16 : uint16 const? = null
        var wsv16 : uint16 const? = null
        var wso16 : uint16 const? = null
        if (f16w && has_q8w) {
            let wsb16 = addr < uint16 const? >(t.qscales16[0])
            wsq16 = wsb16 + t.wq_offs[l] / 32l
            wsk16 = wsb16 + t.wk_offs[l] / 32l
            wsv16 = wsb16 + t.wv_offs[l] / 32l
            wso16 = wsb16 + t.wo_offs[l] / 32l
        } elif (has_q8w) {
            let wsb = addr < float const? >(t.qscales[0])
            wsq = wsb + t.wq_offs[l] / 32l
            wsk = wsb + t.wk_offs[l] / 32l
            wsv = wsb + t.wv_offs[l] / 32l
            wso = wsb + t.wo_offs[l] / 32l
        }
        let wpq = wqb + t.wq_offs[l]
        let wpk = wqb + t.wk_offs[l]
        let wpv = wqb + t.wv_offs[l]
        let wpo = wqb + t.wo_offs[l]
        // kq-tagged weights: plane bases + stamped rows core per format + the Q8_K activation image (kq v2)
        var kxqp : int8? = any_kq ? addr(s.kxq[0]) : null
        var kxsp : float? = any_kq ? addr(s.kxs[0]) : null
        var kxbsp : int? = any_kq ? addr(s.kxbs[0]) : null
        let kqpq = kq_plane_q(t, fq, t.wq_offs[l])
        let kspq = kq_plane_s(t, fq, t.wq_offs[l])
        let rowsq = kq_rows_for(fq)
        let kqpk = kq_plane_q(t, fk, t.wk_offs[l])
        let kspk = kq_plane_s(t, fk, t.wk_offs[l])
        let rowsk = kq_rows_for(fk)
        let kqpv = kq_plane_q(t, fv, t.wv_offs[l])
        let kspv = kq_plane_s(t, fv, t.wv_offs[l])
        let rowsv = kq_rows_for(fv)
        let kqpo = kq_plane_q(t, fo, t.wo_offs[l])
        let kspo = kq_plane_s(t, fo, t.wo_offs[l])
        let rowso = kq_rows_for(fo)
        // this position's rope table row (dual-rope arches: the sliding class's row)
        let swa_row = is_sliding && has_dual_rope(c)
        let crow = swa_row ? addr < float const? >(s.rope_cos_swa[0]) : addr < float const? >(s.rope_cos[0])
        let srow = swa_row ? addr < float const? >(s.rope_sin_swa[0]) : addr < float const? >(s.rope_sin[0])
        // optional per-arch epilogues (null = off): qkv bias, per-head q/k norm, weightless v norm, sinks, wo bias
        let bqp = c.attn_qkv_bias ? addr < float const? >(t.bq[0]) : null
        let bkp = c.attn_qkv_bias ? addr < float const? >(t.bk[0]) : null
        let bvp = c.attn_qkv_bias ? addr < float const? >(t.bv[0]) : null
        let rqp = c.qk_norm ? addr < float const? >(t.fblob[t.rms_q_offs[l]]) : null
        let rkp = c.qk_norm ? addr < float const? >(t.fblob[t.rms_k_offs[l]]) : null
        let rvp = c.v_norm ? addr < float const? >(t.fblob[t.rms_ones_off]) : null
        let sinksp = c.attn_sinks ? addr < float const? >(t.fblob[t.sinks_off + l * n_heads]) : null
        let bop = c.attn_out_bias ? addr < float const? >(t.bo[0]) : null
        let bqoff = l * qd
        let bkvoff = l * kv_dim
        let booff = l * dim
        // flash-decode partial planes (deep ctx only; the arrays are empty when nsl == 1)
        var flmp = nsl > 1 ? addr(s.fl_m[0]) : null
        var fllp = nsl > 1 ? addr(s.fl_l[0]) : null
        var flop = nsl > 1 ? addr(s.fl_o[0]) : null
        team_parallel_stages(s.chain_stages) <| @(stage : int; jb : int; je : int) {
            if (stage == 0) {
                // qkv rows in 32-row groups (row-local); a group never straddles a q/k/v region boundary
                unsafe {
                    var r = int64(jb) * 32l
                    let rend = int64(je) * 32l
                    while (r < rend) {
                        if (r < qd) {
                            let r1 = min(rend, qd)
                            if (fq != KqFmt.q8) {
                                invoke(rowsq, qp, kqpq, kspq, kxqp, kxsp, kxbsp, dim, r, r1)
                            } elif (f16w) {
                                invoke(mmrows16, qp, wpq, wsq16, xqp, xsp, dim, r, r1)
                            } else {
                                invoke(mmrows, qp, wpq, wsq, xqp, xsp, dim, r, r1)
                            }
                            if (bqp != null) {
                                add_inplace(qp + r, bqp + bqoff + r, r1 - r)
                            }
                            r = r1
                        } elif (r < qd + kv_dim) {
                            let r0 = r - qd
                            let r1 = min(rend, qd + kv_dim) - qd
                            if (fk != KqFmt.q8) {
                                invoke(rowsk, kp, kqpk, kspk, kxqp, kxsp, kxbsp, dim, r0, r1)
                            } elif (f16w) {
                                invoke(mmrows16, kp, wpk, wsk16, xqp, xsp, dim, r0, r1)
                            } else {
                                invoke(mmrows, kp, wpk, wsk, xqp, xsp, dim, r0, r1)
                            }
                            if (bkp != null) {
                                add_inplace(kp + r0, bkp + bkvoff + r0, r1 - r0)
                            }
                            r = r1 + qd
                        } else {
                            let r0 = r - qd - kv_dim
                            let r1 = min(rend, qd + 2l * kv_dim) - qd - kv_dim
                            if (fv != KqFmt.q8) {
                                invoke(rowsv, vp, kqpv, kspv, kxqp, kxsp, kxbsp, dim, r0, r1)
                            } elif (f16w) {
                                invoke(mmrows16, vp, wpv, wsv16, xqp, xsp, dim, r0, r1)
                            } else {
                                invoke(mmrows, vp, wpv, wsv, xqp, xsp, dim, r0, r1)
                            }
                            if (bvp != null) {
                                add_inplace(vp + r0, bvp + bkvoff + r0, r1 - r0)
                            }
                            r = r1 + qd + kv_dim
                        }
                    }
                }
            } elif (stage == 1) {
                // head-local epilogue by head slot: norm -> rope -> cache store -> roped-query quantize
                unsafe {
                    for (slot in range(jb, je)) {
                        let sl = int64(slot)
                        if (sl < n_heads) {
                            let r0 = sl * head_size
                            if (rqp != null) {
                                rmsnorm(qp + r0, qp + r0, rqp, head_size, eps)
                            }
                            if (neox) {
                                rope_scaled_neox_tab(qp + r0, head_size, head_size, crow, srow)
                            } else {
                                rope_scaled_tab(qp + r0, head_size, head_size, crow, srow)
                            }
                            if (ktq4) {   // tq4: rotate the roped query head before the plane fill
                                fwht_signs_row(qp + r0, sgp, head_size)
                            }
                            if (kq8) {   // quantize this head's roped query for the int8 score dot
                                quantize_q8_0_into_ptr(qp + r0, head_size, attqp, attsp, r0, r0 / 32l)
                            }
                        } elif (sl < n_heads + n_kv_heads) {
                            let r0 = (sl - n_heads) * head_size
                            if (rkp != null) {
                                rmsnorm(kp + r0, kp + r0, rkp, head_size, eps)
                            }
                            if (neox) {
                                rope_scaled_neox_tab(kp + r0, head_size, head_size, crow, srow)
                            } else {
                                rope_scaled_tab(kp + r0, head_size, head_size, crow, srow)
                            }
                            if (ktq4) {   // tq4: rotate the roped key head before the codec store
                                fwht_signs_row(kp + r0, sgp, head_size)
                            }
                            kv_store_row(kcl, kdt, kv_dim, prow, r0, head_size, kp + r0)
                        } else {
                            let r0 = (sl - n_heads - n_kv_heads) * head_size
                            if (rvp != null) {
                                rmsnorm(vp + r0, vp + r0, rvp, head_size, eps)
                            }
                            if (vtq4) {   // tq4: rotate the value head before the codec store
                                fwht_signs_row(vp + r0, sgp, head_size)
                            }
                            kv_store_row(vcl, vdt, kv_dim, prow, r0, head_size, vp + r0)
                        }
                    }
                }
            } elif (stage == 2 && nsl == 1) {
                // multi-head attention + requant of the wo input; overwrites input quants (stage 0's only reader is done)
                unsafe {
                    for (u in range(jb, je)) {
                        for (g in range64(hpu)) {
                            let hh = int64(u) * hpu + g
                            let qoff = hh * head_size
                            let kvhoff = (hh / kv_mul) * head_size  // GQA: which kv head this query head reads
                            attn_head_decode_d(attp + hh * seq_len, xbp, qp, attqp, attsp, kcl, vcl, kdt, vdt, runsp,
                                nrun, qoff, kvhoff, kstart, cnt, kv_dim, head_size, hh, scale, attn_softcap, sinksp)
                            if (vtq4) {   // tq4 V: this head's xb slice is rotated-basis — un-rotate before requant
                                fwht_unsign_row(xbp + qoff, sgp, head_size)
                            }
                        }
                        let u0 = int64(u) * hpu * head_size
                        if (fo != KqFmt.q8) {   // kq wo: Q8_K over the unit's whole-superblock span
                            quantize_q8_k_into_ptr(xbp + u0, hpu * head_size, kxqp, kxsp, kxbsp, u0, u0 / 256l, u0 / 16l)
                        } else {
                            quantize_q8_0_into_ptr(xbp + u0, hpu * head_size, xqp, xsp, u0, u0 / 32l)
                        }
                    }
                }
            } elif (stage == 2) {
                // flash-decode partials: unit = (KV-head, slice) — K/V rows stream ONCE, serving all kv_mul q-heads
                unsafe {
                    for (u in range(jb, je)) {
                        let kvh = int64(u / nsl)
                        let sl = int64(u % nsl)
                        let kvhoff = kvh * head_size
                        let qh0 = kvh * kv_mul
                        let j0 = sl * slice_len
                        let j1 = min(j0 + slice_len, cnt)
                        // K pass: each K row read once, dotted against every group q-head
                        kv_slice_scores_d(attp, qp, attqp, attsp, kcl, kdt, runsp, nrun, kstart, j0, j1, kv_dim, kvhoff, head_size,
                            qh0, kv_mul, seq_len, scale)
                        for (g in range64(kv_mul)) {
                            let hh = qh0 + g
                            var arow = attp + hh * seq_len
                            if (attn_softcap > 0.0) {
                                softcap4(arow + j0, j1 - j0, attn_softcap)
                            }
                            // hand-vectorized max reduction — order-independent, bit-identical to the scalar scan
                            let m = hmax(unsafe(arow + j0), j1 - j0)
                            // exp4 fast path, clamped to -80 like the prefill flash fold (underflow -> ~0)
                            var lsum = 0.0
                            {
                                let m4 = float4(m, m, m, m)
                                var sum4 = float4(0.0)
                                var j = j0
                                while (j + 4l <= j1) {
                                    let e = exp4(max(float4(arow[j], arow[j + 1l], arow[j + 2l], arow[j + 3l]) - m4, EXP4_LO))
                                    arow[j] = e.x
                                    arow[j + 1l] = e.y
                                    arow[j + 2l] = e.z
                                    arow[j + 3l] = e.w
                                    sum4 += e
                                    j += 4l
                                }
                                lsum = sum4.x + sum4.y + sum4.z + sum4.w
                                while (j < j1) {
                                    let e = exp(max(arow[j] - m, -80.0))
                                    arow[j] = e
                                    lsum += e
                                    j++
                                }
                            }
                            var op = flop + (hh * int64(nsl) + sl) * head_size
                            for (i in range64(head_size)) {
                                op[i] = 0.0
                            }
                            flmp[hh * int64(nsl) + sl] = m
                            fllp[hh * int64(nsl) + sl] = lsum
                        }
                        // V pass: each V row read once, weighted into every group head's partial
                        kv_slice_av_d(flop, attp, vcl, vdt, runsp, nrun, kstart, j0, j1, kv_dim, kvhoff, head_size,
                            qh0, kv_mul, seq_len, int64(nsl), sl)
                    }
                }
            } elif (stage == 3 && nsl > 1) {
                // per-head combine (flash rescale; sink joins max + denominator) + wo-input requant per unit span
                unsafe {
                    for (u in range(jb, je)) {
                        for (g in range64(hpu)) {
                            let hh = int64(u) * hpu + g
                            let qoff = hh * head_size
                            let fbase = hh * int64(nsl)
                            var mg = hmax(unsafe(flmp + fbase), int64(nsl))
                            if (sinksp != null) {
                                mg = max(mg, sinksp[hh])
                            }
                            for (i in range64(head_size)) {
                                xbp[qoff + i] = 0.0
                            }
                            var lg = 0.0
                            {
                                // exp4 over the slice scale factors (nsl <= 16), flash-fold clamp convention
                                let mg4 = float4(mg, mg, mg, mg)
                                var sl = 0l
                                while (sl + 4l <= int64(nsl)) {
                                    let f = exp4(max(float4(flmp[fbase + sl], flmp[fbase + sl + 1l], flmp[fbase + sl + 2l], flmp[fbase + sl + 3l]) - mg4, EXP4_LO))
                                    lg += (fllp[fbase + sl] * f.x + fllp[fbase + sl + 1l] * f.y
                                        + fllp[fbase + sl + 2l] * f.z + fllp[fbase + sl + 3l] * f.w)
                                    axpy(xbp + qoff, flop + (fbase + sl) * head_size, f.x, head_size)
                                    axpy(xbp + qoff, flop + (fbase + sl + 1l) * head_size, f.y, head_size)
                                    axpy(xbp + qoff, flop + (fbase + sl + 2l) * head_size, f.z, head_size)
                                    axpy(xbp + qoff, flop + (fbase + sl + 3l) * head_size, f.w, head_size)
                                    sl += 4l
                                }
                                while (sl < int64(nsl)) {
                                    let f = exp(max(flmp[fbase + sl] - mg, -80.0))
                                    lg += fllp[fbase + sl] * f
                                    axpy(xbp + qoff, flop + (fbase + sl) * head_size, f, head_size)
                                    sl++
                                }
                            }
                            if (sinksp != null) {
                                lg += exp(max(sinksp[hh] - mg, -80.0))
                            }
                            let inv = 1.0 / lg
                            for (i in range64(head_size)) {
                                xbp[qoff + i] *= inv
                            }
                            if (vtq4) {   // tq4 V: combined partials are rotated-basis — un-rotate before requant
                                fwht_unsign_row(xbp + qoff, sgp, head_size)
                            }
                        }
                        let u0 = int64(u) * hpu * head_size
                        if (fo != KqFmt.q8) {   // kq wo: Q8_K over the unit's whole-superblock span
                            quantize_q8_k_into_ptr(xbp + u0, hpu * head_size, kxqp, kxsp, kxbsp, u0, u0 / 256l, u0 / 16l)
                        } else {
                            quantize_q8_0_into_ptr(xbp + u0, hpu * head_size, xqp, xsp, u0, u0 / 32l)
                        }
                    }
                }
            } else {
                // output projection rows + optional bias + residual add, all chunk-local (post-norm arches defer the add)
                unsafe {
                    let r0 = int64(jb) * 32l
                    let r1 = int64(je) * 32l
                    if (fo != KqFmt.q8) {
                        invoke(rowso, xb2p, kqpo, kspo, kxqp, kxsp, kxbsp, qd, r0, r1)
                    } elif (f16w) {
                        invoke(mmrows16, xb2p, wpo, wso16, xqp, xsp, qd, r0, r1)
                    } else {
                        invoke(mmrows, xb2p, wpo, wso, xqp, xsp, qd, r0, r1)
                    }
                    if (bop != null) {
                        add_inplace(xb2p + r0, bop + booff + r0, r1 - r0)
                    }
                    if (!post_norm) {
                        add_inplace(xp + r0, xb2p + r0, r1 - r0)
                    }
                }
            }
        }
        if (nsl > 1 && g_flash_decode_check) {
            // test-only witness: sliced partials + combine must equal an exact single-pass recompute (fp32 tolerance)
            flash_decode_check_d(qp, attqp, attsp, kcl, vcl, kdt, vdt, xbp, runsp,
                nrun, n_heads, kv_mul, head_size, kv_dim, kstart, cnt, l, int64(nsl), scale, attn_softcap, sinksp,
                vtq4 ? sgp : null)
            g_flash_decode_checked++
        }
    }
    prof_add("attn_chain", ts_chain)
    if (post_norm) {  // Gemma2/3: post-attention RMSNorm before the residual add — same serial
        // tail as the un-fused path (dim-sized, microseconds), bit-identical order
        let ts_resid = ref_time_ticks()
        rms_at(s.xb2, t, t.rms_post_att_off + l * dim, s.xb2, dim)
        add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb2[0])), dim)
        prof_add("act_resid", ts_resid)
    }
}

// Can layer l's dense FFN run as the fused 2-stage chain? Q8, team mode + a backend row core, and
// the size threshold. pre_post_norm/layer_out_scale are handled: the chain defers the residual add
// and the caller's serial tail runs post-norm and/or the fused add+scale after the rendezvous.
def private fused_ffn_ok(t : Model; l : int64) : bool {
    let c = t.config
    let f1 = fmt_at(t.w1_fmt, l)
    let f2 = fmt_at(t.w2_fmt, l)
    let f3 = fmt_at(t.w3_fmt, l)
    let any_kq = f1 != KqFmt.q8 || f2 != KqFmt.q8 || f3 != KqFmt.q8
    return (g_fused_decode && t.quant == QuantMode.q8
        && kq_servable(t, f1) && kq_servable(t, f2) && kq_servable(t, f3)
        // kq v2 regrain: Q8_K quantizes whole 256-superblocks, so dim/hidden must divide out cleanly
        && (!any_kq || c.dim % 256l == 0l)
        && (f2 == KqFmt.q8 || layer_hidden(t, l) % 256l == 0l)
        && get_jobque_team_mode() && kernel_backend_has_rows()
        && (!t.wscale_f16 || kernel_backend_has_rows16())
        && lanes_for_work(c.dim * layer_hidden(t, l), g_gemv_lane_cap) > 1)
}

// Fused dense FFN block: ONE team rendezvous runs [w1+w3 rows + act + requant] -> [w2 rows +
// residual add] as two stages, gate/up rows PAIRED per chunk (requant lands in xq2/xs2, not
// xq/xs — other stage-0 chunks still read the input quants). Bit-identical to ffn_dense_decode.
def private ffn_chain_decode(t : Model; var s : Session; l : int64) {
    let c = t.config
    let dim = c.dim
    let hidden = layer_hidden(t, l)   // gemma-4 E2B varies FFN width per layer
    // per-weight formats (kq layers reach here only off a repacked load — fused_ffn_ok)
    let f1 = fmt_at(t.w1_fmt, l)
    let f2 = fmt_at(t.w2_fmt, l)
    let f3 = fmt_at(t.w3_fmt, l)
    let any_kq = f1 != KqFmt.q8 || f2 != KqFmt.q8 || f3 != KqFmt.q8
    let ts_norm1 = ref_time_ticks()
    rms_at(s.xb, t, t.rms_ffn_off + l * dim, s.x, dim)
    // one input quantize per consumed form: q8 gate/up read Q8_0, kq gate/up read Q8_K (kq v2)
    if (f1 == KqFmt.q8 || f3 == KqFmt.q8) {
        quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
    }
    if (f1 != KqFmt.q8 || f3 != KqFmt.q8) {
        quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
    }
    prof_add("norm", ts_norm1)
    let ts_chain = ref_time_ticks()
    trace_tag(TRACE_TAG_FFN_CHAIN)
    let mmrows = mm_q8q8_rows_fn()
    let is_gelu = c.ffn_act == FfnAct.gelu
    let is_oai = c.ffn_act == FfnAct.swiglu_oai
    let vec = g_act_vec
    // Gemma2/3 post-FFN norm is a whole-vector reduction, so the chain defers the residual add to a serial tail
    let post_norm = c.pre_post_norm
    let out_scale_here = c.layer_out_scale && !has_ple(c)
    let defer_add = post_norm || out_scale_here
    // stage-0 grain: 32 rows for a q8 down-proj; 256 for a kq one (Q8_K quantizes whole superblocks)
    let grain0 = f2 != KqFmt.q8 ? 256l : 32l
    let ngrp0 = int(hidden / grain0)
    let ngrp1 = int(dim / 32l)
    let n0 = chain_stage_chunks(ngrp0, int(hidden), 2l * dim * hidden)
    let n1 = chain_stage_chunks(ngrp1, int(dim), hidden * dim)
    s.chain_stages |> resize(2)
    s.chain_stages[0] = int3(0, ngrp0, n0)
    s.chain_stages[1] = int3(0, ngrp1, n1)
    count_parallel_dispatch(min(n0, ngrp0) + min(n1, ngrp1))
    unsafe {
        var xp = addr(s.x[0])
        var xbp = addr(s.xb[0])
        var gup = addr(s.ffn_gu[0])
        var xqp = addr(s.xq[0])
        var xsp = addr(s.xs[0])
        var xq2p = addr(s.xq2[0])
        var xs2p = addr(s.xs2[0])
        let f16w = t.wscale_f16
        let mmrows16 = mm_q8q8_rows16_fn()
        // all-kq layers skip the q8 bases — qblob/qscales can be EMPTY on an all-K-quant file
        let has_q8w = f1 == KqFmt.q8 || f2 == KqFmt.q8 || f3 == KqFmt.q8
        let wqb = has_q8w ? addr < int8 const? >(t.qblob[0]) : null
        let w1o = t.w1_offs[l]
        let w3o = t.w3_offs[l]
        let w2o = t.w2_offs[l]
        var ws1 : float const? = null
        var ws3 : float const? = null
        var ws2 : float const? = null
        var ws116 : uint16 const? = null
        var ws316 : uint16 const? = null
        var ws216 : uint16 const? = null
        if (f16w && has_q8w) {
            let wsb16 = addr < uint16 const? >(t.qscales16[0])
            ws116 = wsb16 + w1o / 32l
            ws316 = wsb16 + w3o / 32l
            ws216 = wsb16 + w2o / 32l
        } elif (has_q8w) {
            let wsb = addr < float const? >(t.qscales[0])
            ws1 = wsb + w1o / 32l
            ws3 = wsb + w3o / 32l
            ws2 = wsb + w2o / 32l
        }
        let wp1 = wqb + w1o
        let wp3 = wqb + w3o
        let wp2 = wqb + w2o
        // kq-tagged weights: plane bases + rows cores + the Q8_K activation image (kq v2); q8 rows cores keep Q8_0
        var kxqp : int8? = any_kq ? addr(s.kxq[0]) : null
        var kxsp : float? = any_kq ? addr(s.kxs[0]) : null
        var kxbsp : int? = any_kq ? addr(s.kxbs[0]) : null
        var xbs2w : int? = f2 != KqFmt.q8 ? addr(s.xbs2[0]) : null
        let kqp1 = kq_plane_q(t, f1, w1o)
        let ksp1 = kq_plane_s(t, f1, w1o)
        let rows1 = kq_rows_for(f1)
        let kqp3 = kq_plane_q(t, f3, w3o)
        let ksp3 = kq_plane_s(t, f3, w3o)
        let rows3 = kq_rows_for(f3)
        let kqp2 = kq_plane_q(t, f2, w2o)
        let ksp2 = kq_plane_s(t, f2, w2o)
        let rows2 = kq_rows_for(f2)
        team_parallel_stages(s.chain_stages) <| @(stage : int; jb : int; je : int) {
            unsafe {
                let grain = stage == 0 ? grain0 : 32l
                let r0 = int64(jb) * grain
                let r1 = int64(je) * grain
                if (stage == 0) {
                    // paired gate/up rows, then act + requant on exactly these rows
                    if (f1 != KqFmt.q8) {
                        invoke(rows1, gup, kqp1, ksp1, kxqp, kxsp, kxbsp, dim, r0, r1)
                    } elif (f16w) {
                        invoke(mmrows16, gup, wp1, ws116, xqp, xsp, dim, r0, r1)
                    } else {
                        invoke(mmrows, gup, wp1, ws1, xqp, xsp, dim, r0, r1)
                    }
                    if (f3 != KqFmt.q8) {
                        invoke(rows3, gup + hidden, kqp3, ksp3, kxqp, kxsp, kxbsp, dim, r0, r1)
                    } elif (f16w) {
                        invoke(mmrows16, gup + hidden, wp3, ws316, xqp, xsp, dim, r0, r1)
                    } else {
                        invoke(mmrows, gup + hidden, wp3, ws3, xqp, xsp, dim, r0, r1)
                    }
                    if (vec) {
                        if (is_gelu) {
                            geglu4(gup + r0, gup + hidden + r0, r1 - r0)
                        } elif (is_oai) {
                            swiglu_oai4(gup + r0, gup + hidden + r0, r1 - r0)
                        } else {
                            swiglu4(gup + r0, gup + hidden + r0, r1 - r0)
                        }
                    } else {
                        if (is_gelu) {
                            geglu(gup + r0, gup + hidden + r0, r1 - r0)
                        } elif (is_oai) {
                            swiglu_oai(gup + r0, gup + hidden + r0, r1 - r0)
                        } else {
                            swiglu(gup + r0, gup + hidden + r0, r1 - r0)
                        }
                    }
                    if (f2 != KqFmt.q8) {   // kq down-proj: Q8_K requant, whole superblocks (grain 256)
                        quantize_q8_k_into_ptr(gup + r0, r1 - r0, xq2p, xs2p, xbs2w, r0, r0 / 256l, r0 / 16l)
                    } else {
                        quantize_q8_0_into_ptr(gup + r0, r1 - r0, xq2p, xs2p, r0, r0 / 32l)
                    }
                } else {
                    // down-projection rows + residual add (post-norm/layer_out_scale arches defer the add)
                    if (f2 != KqFmt.q8) {
                        invoke(rows2, xbp, kqp2, ksp2, xq2p, xs2p, xbs2w, hidden, r0, r1)
                    } elif (f16w) {
                        invoke(mmrows16, xbp, wp2, ws216, xq2p, xs2p, hidden, r0, r1)
                    } else {
                        invoke(mmrows, xbp, wp2, ws2, xq2p, xs2p, hidden, r0, r1)
                    }
                    if (!defer_add) {
                        add_inplace(xp + r0, xbp + r0, r1 - r0)
                    }
                }
            }
        }
    }
    prof_add("ffn_chain", ts_chain)
    if (defer_add) {  // same serial tail as the un-fused path (dim-sized, microseconds), bit-identical order
        let ts_resid = ref_time_ticks()
        if (post_norm) {  // Gemma2/3/4: post-FFN RMSNorm before the residual add
            rms_at(s.xb, t, t.rms_post_ffn_off + l * dim, s.xb, dim)
        }
        if (out_scale_here) {  // gemma4 12B/31B: residual + per-layer output scale, one pass
            add_scale_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb[0])), t.fblob[t.out_scale_off + l], dim)
        } else {
            add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb[0])), dim)
        }
        prof_add("act_resid", ts_resid)
    }
}

def private attention_std_decode(t : Model; var s : Session; l : int64; pos : int64) {
    if (fused_attn_ok(t, l)) {
        attention_chain_decode(t, s, l, pos)
        return
    }
    trace_tag(TRACE_TAG_ATTN_STD)
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size       // query / attention-output width (== dim except Gemma2/gemma4)
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    var kcl = kv_layer_kbase(s, l, c.seq_len)   // layer-base byte pointers (aliases the source layer when shared)
    var vcl = kv_layer_vbase(s, l, c.seq_len)
    // sliding-window vs global attention per the arch's layer pattern; no-op for short contexts (pos < window)
    let is_sliding = layer_is_sliding(c, l)
    // gemma-4 E-series shared-KV layer: no attn_k/attn_v of its own — project Q only, attend against the SOURCE layer's cache
    let kv_shared = t.kv_src[l] != l
    let kstart = (is_sliding && pos + 1l > c.sliding_window) ? (pos + 1l - c.sliding_window) : 0l
    let cnt = pos + 1l - kstart
    // attention rmsnorm
    let ts_norm0 = ref_time_ticks()
    rms_at(s.xb, t, t.rms_att_off + l * dim, s.x, dim)
    prof_add("norm", ts_norm0)
    // q, k, v projections (independent — share s.xb; fused into one parallel dispatch when Q8)
    let ts_qkv = ref_time_ticks()
    if (kv_shared) {
        mm_f(s.q, t, fmt_at(t.wq_fmt, l), t.wq_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, qd)   // Q only
    } else {
        mm_qkv(s, t, l)
    }
    if (c.attn_qkv_bias) {  // Qwen2: + bias before RoPE
        add_bias(s.q, 0l, t.bq, l * qd, qd)
        add_bias(s.k, 0l, t.bk, l * kv_dim, kv_dim)
        add_bias(s.v, 0l, t.bv, l * kv_dim, kv_dim)
    }
    prof_add("mm_qkv", ts_qkv)
    if (c.qk_norm) {  // Qwen3/gemma4: per-head RMSNorm on q/k before RoPE — q/k are n_heads/n_kv_heads rows of head_size
        let ts_qknorm = ref_time_ticks()
        rms_batch(s.q, s.q, t, t.rms_q_offs[l], head_size, n_heads)
        if (!kv_shared) {
            rms_batch(s.k, s.k, t, t.rms_k_offs[l], head_size, n_kv_heads)
        }
        prof_add("norm", ts_qknorm)
    }
    // gemma4: weightless per-head RMSNorm on V; V-from-K layers already normed V inside mm_qkv
    if (c.v_norm && t.wv_offs[l] >= 0l) {
        let ts_vnorm = ref_time_ticks()
        rms_batch(s.v, s.v, t, t.rms_ones_off, head_size, n_kv_heads)
        prof_add("norm", ts_vnorm)
    }
    // table path: leaves called DIRECTLY, same as rope_batch_tab, to dodge pass-through-const-write-elision
    let ts_rope = ref_time_ticks()
    // partial rotary (glm4moe: 64 of 128) — table rows span rot/2 pairs; loader guarantees NEOX
    let rot = c.rope_dim > 0l ? c.rope_dim : head_size
    if (g_use_rope_table) {
        let swa_row = is_sliding && has_dual_rope(c)
        unsafe {
            let crow = swa_row ? addr<float const?>(s.rope_cos_swa[0]) : addr<float const?>(s.rope_cos[0])
            let srow = swa_row ? addr<float const?>(s.rope_sin_swa[0]) : addr<float const?>(s.rope_sin[0])
            if (rot != head_size) {
                rope_scaled_neox_tab_part(addr(s.q[0]), head_size, rot, qd, crow, srow)
                if (!kv_shared) { rope_scaled_neox_tab_part(addr(s.k[0]), head_size, rot, kv_dim, crow, srow) }
            } elif (c.rope_neox) {
                rope_scaled_neox_tab(addr(s.q[0]), head_size, qd, crow, srow)
                if (!kv_shared) { rope_scaled_neox_tab(addr(s.k[0]), head_size, kv_dim, crow, srow) }
            } else {
                rope_scaled_tab(addr(s.q[0]), head_size, qd, crow, srow)
                if (!kv_shared) { rope_scaled_tab(addr(s.k[0]), head_size, kv_dim, crow, srow) }
            }
        }
    } else {
        let theta = is_sliding ? c.rope_freq_base_swa : c.rope_freq_base
        let fscale = is_sliding ? c.rope_freq_scale_swa : c.rope_freq_scale
        let use_ff = !(c.rope_ff_global_only && is_sliding)
        if (rot != head_size) {
            unsafe {
                rope_scaled_neox_part(addr(s.q[0]), pos, head_size, rot, qd, theta, fscale, false, null, c.rope_mscale)
                if (!kv_shared) {
                    rope_scaled_neox_part(addr(s.k[0]), pos, head_size, rot, kv_dim, theta, fscale, false, null, c.rope_mscale)
                }
            }
        } else {
            rope_apply(s.q, pos, head_size, qd, theta, fscale, t.rope_freqs, c.rope_neox, c.rope_mscale, use_ff)
            if (!kv_shared) {
                rope_apply(s.k, pos, head_size, kv_dim, theta, fscale, t.rope_freqs, c.rope_neox, c.rope_mscale, use_ff)
            }
        }
    }
    prof_add("rope", ts_rope)
    let ktq4 = s.kv_dtype_k == KVDtype.tq4
    let vtq4 = s.kv_dtype_v == KVDtype.tq4
    if (ktq4 || vtq4) {
        // tq4 basis change: q/k/v rotate per head, attention runs in that basis, xb un-rotates after (⟨Hq,Hk⟩ = ⟨q,k⟩)
        let ts_rot = ref_time_ticks()
        unsafe {
            let sgp = addr < float const? >(s.kv_signs[0])
            if (ktq4) {
                var qrp = addr(s.q[0])
                for (hh in range64(qd / head_size)) {
                    fwht_signs_row(qrp + hh * head_size, sgp, head_size)
                }
                if (!kv_shared) {
                    var krp = addr(s.k[0])
                    for (hh in range64(kv_dim / head_size)) {
                        fwht_signs_row(krp + hh * head_size, sgp, head_size)
                    }
                }
            }
            if (vtq4 && !kv_shared) {
                var vrp = addr(s.v[0])
                for (hh in range64(kv_dim / head_size)) {
                    fwht_signs_row(vrp + hh * head_size, sgp, head_size)
                }
            }
        }
        prof_add("tq4_rot", ts_rot)
    }
    if (s.kv_dtype_k == KVDtype.q8_0 || ktq4) {
        // quantize the roped query ONCE for the int8x int8/int4 score dot — O(qd), dwarfed by the reads it feeds
        quantize_q8_0_into(s.q, qd, s.attq, s.atts, 0l, 0l)
    }
    // store k, v for this position (shared-KV layers: nothing to store — source layer already cached it)
    if (!kv_shared) {
        let ts_kv = ref_time_ticks()
        unsafe {
            kv_store_row(kcl, s.kv_dtype_k, kv_dim, kv_phys_row(s, pos), 0l, kv_dim,
                addr < float const? >(s.k[0]))
            kv_store_row(vcl, s.kv_dtype_v, kv_dim, kv_phys_row(s, pos), 0l, kv_dim,
                addr < float const? >(s.v[0]))
        }
        prof_add("kv_store", ts_kv)
    }
    // attention threaded over heads: disjoint att rows per head, result-identical to the inline loop
    let ts_attn = ref_time_ticks()
    let seq_len = c.seq_len
    let attn_softcap = c.attn_logit_softcap
    // work-proportional lanes; tiny-cnt early-decode attention (~2K MACs at cnt=1) inlines instead
    let attn_lanes = min(int(n_heads), lanes_for_work(2l * cnt * head_size * n_heads, g_gemv_lane_cap))
    let sinksp = (c.attn_sinks
        ? unsafe(addr < float const? >(t.fblob[t.sinks_off + l * n_heads]))
        : null)
    let kdt = s.kv_dtype_k
    let vdt = s.kv_dtype_v
    let nrun = kv_runs(s, kstart, pos + 1l, s.kv_runs)   // physical runs of this layer's window (flat: 1)
    unsafe {
        let kplanes = kdt == KVDtype.q8_0 || kdt == KVDtype.tq4
        let qp = addr < float const? >(s.q[0])
        let qqp = kplanes ? addr < int8 const? >(s.attq[0]) : null
        let qsp = kplanes ? addr < float const? >(s.atts[0]) : null
        let runsp = addr < KVRun const? >(s.kv_runs[0])
        var xbp = addr(s.xb[0])
        var attp = addr(s.att[0])
        maybe_parallel_for(0, int(n_heads), attn_lanes) $(rb, re) {
            unsafe {
                for (h in range(rb, re)) {
                    let hh = int64(h)
                    let qoff = hh * head_size
                    let kvhoff = (hh / kv_mul) * head_size  // GQA: which kv head this query head reads
                    attn_head_decode_d(attp + hh * seq_len, xbp, qp, qqp, qsp, kcl, vcl, kdt, vdt, runsp,
                        nrun, qoff, kvhoff, kstart, cnt, kv_dim, head_size, hh, scale, attn_softcap, sinksp)
                }
            }
        }
    }
    prof_add("attn", ts_attn)
    if (vtq4) {   // tq4 V: xb accumulated in the rotated basis — un-rotate each head's slice
        unsafe {
            let sgp = addr < float const? >(s.kv_signs[0])
            var xrp = addr(s.xb[0])
            for (hh in range64(n_heads)) {
                fwht_unsign_row(xrp + hh * head_size, sgp, head_size)
            }
        }
    }
    // output projection (qd -> dim), optional bias + post-attention norm, residual
    let ts_wo = ref_time_ticks()
    mm_f(s.xb2, t, fmt_at(t.wo_fmt, l), t.wo_offs[l], s.xb, s.xq, s.xs, s.xbs, qd, dim)
    if (c.attn_out_bias) {  // gpt-oss: + bias on the projected attention output
        add_bias(s.xb2, 0l, t.bo, l * dim, dim)
    }
    prof_add("mm_wo", ts_wo)
    let ts_resid0 = ref_time_ticks()
    if (c.pre_post_norm) {  // Gemma2: post-attention RMSNorm before the residual add
        rms_at(s.xb2, t, t.rms_post_att_off + l * dim, s.xb2, dim)
    }
    add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb2[0])), dim)
    prof_add("act_resid", ts_resid0)
}

// ===== qwen35 hybrid blocks: Gated DeltaNet (recurrent) + gated attention =====
// Alternates Gated-DeltaNet linear-attention layers (no KV, O(1) f32 state) with full-attention
// layers whose 2x-wide Q carries a sigmoid output gate. Oracle: llama.cpp qwen35.cpp + delta-net-base.cpp.

// Causal depthwise conv1d, one token step: per channel, dot the (taps = d_conv-1)-deep history
// plus the current input against that channel's taps (oldest first, ggml_ssm_conv order), then
// shift the history. Float accumulation matches ggml's sumf.
def private dn_conv_step(var outp : float?; var st : float?; inp : float const?; w : float const?; channels, taps : int64) {
    unsafe {
        for (ch in range64(channels)) {
            let sb = ch * taps
            let wb = ch * (taps + 1l)
            var acc = 0.0
            for (k in range64(taps)) {
                acc += st[sb + k] * w[wb + k]
            }
            let cur = inp[ch]
            acc += cur * w[wb + taps]
            for (k in range64(taps - 1l)) {
                st[sb + k] = st[sb + k + 1l]
            }
            st[sb + taps - 1l] = cur
            outp[ch] = acc
        }
    }
}

// One delta-rule step for one v-head (llama.cpp build_delta_net_autoregressive): S decays by
// exp(g); sk = kᵀS; d = (v - sk)·β; S += k⊗d; o = qᵀS (q pre-scaled by 1/sqrt(d_state)).
// Row-by-row accumulation over the k index reproduces ggml's sum order.
def private dn_delta_head(var st : float?; q, k, v : float const?; var o : float?; var skd : float?; g, beta : float; ds : int64) {
    unsafe {
        let decay = exp(g)
        for (j in range64(ds)) {
            skd[j] = 0.0
        }
        for (i in range64(ds)) {
            var row = st + i * ds
            let ki = k[i]
            for (j in range64(ds)) {
                let sv = row[j] * decay
                row[j] = sv
                skd[j] += ki * sv
            }
        }
        for (j in range64(ds)) {
            skd[j] = (v[j] - skd[j]) * beta
            o[j] = 0.0
        }
        for (i in range64(ds)) {
            var row = st + i * ds
            let ki = k[i]
            let qi = q[i]
            for (j in range64(ds)) {
                let sv = row[j] + ki * skd[j]
                row[j] = sv
                o[j] += qi * sv
            }
        }
    }
}

// The per-token deltanet core, shared by decode and (per position) prefill: β/g transforms, causal
// conv + SiLU, q/k L2-norm + q pre-scale, the per-v-head delta rule, and z-gated out-norm.
// `qkv` is this token's fused [q;k;v] row; z/beta/g its other projections; output lands in `o`.
def private dn_token_core(t : Model; var s : Session; l : int64; qkv : float const?; var z : float?; var beta : float?; var g : float?; var o : float?) {
    let c = t.config
    let cd = dn_conv_dim(c)
    let kd = dn_key_dim(c)
    let nvh = c.ssm_dt_rank
    let nkh = c.ssm_n_group
    let ds = c.ssm_d_state
    let taps = c.ssm_d_conv - 1l
    unsafe {
        // β = σ(raw); g = ssm_a ⊙ softplus(raw + dt_bias) — ssm_a stores −exp(A_log) precomputed
        for (h in range64(nvh)) {
            beta[h] = 1.0 / (1.0 + exp(-beta[h]))
            g[h] = t.fblob[t.dn_a_off + l * nvh + h] * softplus(g[h] + t.fblob[t.dn_dt_off + l * nvh + h])
        }
        // causal conv over the fused [q;k;v] row (persistent per-layer history), then SiLU
        var cop = addr(s.dn_conv_out[0])
        dn_conv_step(cop, addr(s.dn_conv_state[s.dn_conv_prefix[l]]), qkv,
            addr < float const? >(t.fblob[t.dn_conv_off + l * c.ssm_d_conv * cd]), cd, taps)
        silu(cop, cd)
        // L2-normalize the q and k heads (v passes through), then pre-scale q by 1/sqrt(ds)
        l2_norm_rows(cop, nkh, ds, c.norm_eps)
        l2_norm_rows(cop + kd, nkh, ds, c.norm_eps)
        scale_inplace(cop, 1.0 / sqrt(float(ds)), kd)
        // per-v-head delta rule; k-heads repeat cyclically onto v-heads (h % nkh); heads are independent, so lanes over them
        let qp = reinterpret<float const?>(cop)
        let kp = reinterpret<float const?>(cop + kd)
        let vp = reinterpret<float const?>(cop + 2l * kd)
        var stp = addr(s.dn_state[s.dn_state_prefix[l]])
        var skp = addr(s.dn_sk[0])
        let dn_lanes = min(int(nvh), lanes_for_work(2l * nvh * ds * ds, g_gemv_lane_cap))
        maybe_parallel_for(0, int(nvh), dn_lanes) $(rb, re) {
            unsafe {
                for (hh in range(rb, re)) {
                    let h = int64(hh)
                    let kh = h % nkh
                    dn_delta_head(stp + h * ds * ds, qp + kh * ds, kp + kh * ds, vp + h * ds,
                        o + h * ds, skp + h * ds, g[h], beta[h], ds)
                }
            }
        }
        // gated out-norm: per head RMSNorm(o; ssm_norm, shared weights) ⊙ SiLU(z)
        let wnorm = addr < float const? >(t.fblob[t.dn_norm_off + l * ds])
        for (h in range64(nvh)) {
            rmsnorm(o + h * ds, reinterpret<float const?>(o + h * ds), wnorm, ds, c.norm_eps)
        }
        silu(z, c.ssm_d_inner)
        mul_inplace(o, reinterpret<float const?>(z), c.ssm_d_inner)
    }
}

// attn-norm → fused qkv/z/β/α projections → per-token deltanet core → ssm_out projection → residual.
def private deltanet_decode(t : Model; var s : Session; l : int64) {
    let c = t.config
    let dim = c.dim
    let cd = dn_conv_dim(c)
    let di = c.ssm_d_inner
    let nvh = c.ssm_dt_rank
    let ts_norm0 = ref_time_ticks()
    rms_at(s.xb, t, t.rms_att_off + l * dim, s.x, dim)
    prof_add("norm", ts_norm0)
    let ts_qkv = ref_time_ticks()
    mm_f(s.dn_qkv, t, KqFmt.q8, t.dnqkv_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, cd)
    mm_f(s.dn_z, t, KqFmt.q8, t.dngate_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, di)
    if (t.dn_ba_f32) {   // F32-on-disk beta/alpha matmul fp32, like llama.cpp
        mm_fblob(s.dn_beta, t, t.dn_betaf_off + l * dim * nvh, s.xb, dim, nvh)
        mm_fblob(s.dn_g, t, t.dn_alphaf_off + l * dim * nvh, s.xb, dim, nvh)
    } else {
        mm_f(s.dn_beta, t, KqFmt.q8, t.dnbeta_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, nvh)
        mm_f(s.dn_g, t, KqFmt.q8, t.dnalpha_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, nvh)
    }
    prof_add("mm_qkv", ts_qkv)
    let ts_attn = ref_time_ticks()
    unsafe {
        dn_token_core(t, s, l, addr < float const? >(s.dn_qkv[0]), addr(s.dn_z[0]),
            addr(s.dn_beta[0]), addr(s.dn_g[0]), addr(s.dn_o[0]))
    }
    prof_add("attn", ts_attn)
    let ts_wo = ref_time_ticks()
    mm_f(s.xb2, t, KqFmt.q8, t.dnout_offs[l], s.dn_o, s.xq, s.xs, s.xbs, di, dim)
    prof_add("mm_wo", ts_wo)
    let ts_resid = ref_time_ticks()
    add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb2[0])), dim)
    prof_add("act_resid", ts_resid)
}

// attn-norm → 2x-wide [q|gate] + k/v → QK-norm → partial rope → KV-store → GQA attention →
// out ⊙ σ(gate) → out-proj → residual. The gated-attention twin of attention_std_decode.
def private attention_qwen35_gated_decode(t : Model; var s : Session; l : int64; pos : int64) {
    trace_tag(TRACE_TAG_ATTN_STD)
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let rot = c.rope_dim > 0l ? c.rope_dim : head_size
    var kcl = kv_layer_kbase(s, l, c.seq_len)
    var vcl = kv_layer_vbase(s, l, c.seq_len)
    let kstart = 0l
    let cnt = pos + 1l
    let ts_norm0 = ref_time_ticks()
    rms_at(s.xb, t, t.rms_att_off + l * dim, s.x, dim)
    prof_add("norm", ts_norm0)
    let ts_qkv = ref_time_ticks()
    mm_f(s.dn_qg, t, fmt_at(t.wq_fmt, l), t.wq_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, 2l * qd)
    mm_f(s.k, t, fmt_at(t.wk_fmt, l), t.wk_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, kv_dim)
    mm_f(s.v, t, fmt_at(t.wv_fmt, l), t.wv_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, kv_dim)
    // deinterleave: head h's q lives at 2h·hs, its gate at (2h+1)·hs
    unsafe {
        let qgp = addr < float const? >(s.dn_qg[0])
        var qp = addr(s.q[0])
        var gp = addr(s.dn_qgate[0])
        for (h in range64(n_heads)) {
            let sb = h * 2l * head_size
            let db = h * head_size
            for (i in range64(head_size)) {
                qp[db + i] = qgp[sb + i]
                gp[db + i] = qgp[sb + head_size + i]
            }
        }
    }
    prof_add("mm_qkv", ts_qkv)
    let ts_qknorm = ref_time_ticks()
    rms_batch(s.q, s.q, t, t.rms_q_offs[l], head_size, n_heads)
    rms_batch(s.k, s.k, t, t.rms_k_offs[l], head_size, n_kv_heads)
    prof_add("norm", ts_qknorm)
    let ts_rope = ref_time_ticks()
    if (g_use_rope_table) {
        unsafe {
            let crow = addr < float const? >(s.rope_cos[0])
            let srow = addr < float const? >(s.rope_sin[0])
            rope_scaled_neox_tab_part(addr(s.q[0]), head_size, rot, qd, crow, srow)
            rope_scaled_neox_tab_part(addr(s.k[0]), head_size, rot, kv_dim, crow, srow)
        }
    } else {
        unsafe {
            rope_scaled_neox_part(addr(s.q[0]), pos, head_size, rot, qd, c.rope_freq_base, c.rope_freq_scale, false, null, c.rope_mscale)
            rope_scaled_neox_part(addr(s.k[0]), pos, head_size, rot, kv_dim, c.rope_freq_base, c.rope_freq_scale, false, null, c.rope_mscale)
        }
    }
    prof_add("rope", ts_rope)
    let ktq4 = s.kv_dtype_k == KVDtype.tq4
    let vtq4 = s.kv_dtype_v == KVDtype.tq4
    if (ktq4 || vtq4) {
        // tq4 basis change at the RoPE seam: attention runs rotated; xb un-rotates per head after
        let ts_rot = ref_time_ticks()
        unsafe {
            let sgp = addr < float const? >(s.kv_signs[0])
            if (ktq4) {
                var qrp = addr(s.q[0])
                for (hh in range64(qd / head_size)) {
                    fwht_signs_row(qrp + hh * head_size, sgp, head_size)
                }
                var krp = addr(s.k[0])
                for (hh in range64(kv_dim / head_size)) {
                    fwht_signs_row(krp + hh * head_size, sgp, head_size)
                }
            }
            if (vtq4) {
                var vrp = addr(s.v[0])
                for (hh in range64(kv_dim / head_size)) {
                    fwht_signs_row(vrp + hh * head_size, sgp, head_size)
                }
            }
        }
        prof_add("tq4_rot", ts_rot)
    }
    if (s.kv_dtype_k == KVDtype.q8_0 || ktq4) {
        quantize_q8_0_into(s.q, qd, s.attq, s.atts, 0l, 0l)
    }
    let ts_kv = ref_time_ticks()
    unsafe {
        kv_store_row(kcl, s.kv_dtype_k, kv_dim, kv_phys_row(s, pos), 0l, kv_dim,
            addr < float const? >(s.k[0]))
        kv_store_row(vcl, s.kv_dtype_v, kv_dim, kv_phys_row(s, pos), 0l, kv_dim,
            addr < float const? >(s.v[0]))
    }
    prof_add("kv_store", ts_kv)
    let ts_attn = ref_time_ticks()
    let seq_len = c.seq_len
    let attn_softcap = c.attn_logit_softcap
    let attn_lanes = min(int(n_heads), lanes_for_work(2l * cnt * head_size * n_heads, g_gemv_lane_cap))
    let kdt = s.kv_dtype_k
    let vdt = s.kv_dtype_v
    let nrun = kv_runs(s, kstart, pos + 1l, s.kv_runs)
    unsafe {
        let kplanes = kdt == KVDtype.q8_0 || kdt == KVDtype.tq4
        let qp = addr < float const? >(s.q[0])
        let qqp = kplanes ? addr < int8 const? >(s.attq[0]) : null
        let qsp = kplanes ? addr < float const? >(s.atts[0]) : null
        let runsp = addr < KVRun const? >(s.kv_runs[0])
        var xbp = addr(s.xb[0])
        var attp = addr(s.att[0])
        maybe_parallel_for(0, int(n_heads), attn_lanes) $(rb, re) {
            unsafe {
                for (h in range(rb, re)) {
                    let hh = int64(h)
                    let qoff = hh * head_size
                    let kvhoff = (hh / kv_mul) * head_size
                    attn_head_decode_d(attp + hh * seq_len, xbp, qp, qqp, qsp, kcl, vcl, kdt, vdt, runsp,
                        nrun, qoff, kvhoff, kstart, cnt, kv_dim, head_size, hh, scale, attn_softcap, null)
                }
            }
        }
    }
    prof_add("attn", ts_attn)
    if (vtq4) {   // tq4 V: xb accumulated in the rotated basis — un-rotate before the out-gate
        unsafe {
            let sgp = addr < float const? >(s.kv_signs[0])
            var xrp = addr(s.xb[0])
            for (hh in range64(n_heads)) {
                fwht_unsign_row(xrp + hh * head_size, sgp, head_size)
            }
        }
    }
    // per-head sigmoid output gate on the attention output, before the out projection (exp4)
    let ts_wo = ref_time_ticks()
    unsafe {
        var xbp2 = addr(s.xb[0])
        let gp2 = addr < float const? >(s.dn_qgate[0])
        if (g_act_vec) {
            sigmoid_gate4(xbp2, gp2, qd)
        } else {
            sigmoid_gate(xbp2, gp2, qd)
        }
    }
    mm_f(s.xb2, t, fmt_at(t.wo_fmt, l), t.wo_offs[l], s.xb, s.xq, s.xs, s.xbs, qd, dim)
    prof_add("mm_wo", ts_wo)
    let ts_resid = ref_time_ticks()
    add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb2[0])), dim)
    prof_add("act_resid", ts_resid)
}

// Zero the recurrent state for a fresh conversation (position 0).
def private dn_reset(var s : Session) {
    for (i in range64(int64(length(s.dn_conv_state)))) {
        s.dn_conv_state[i] = 0.0
    }
    for (i in range64(int64(length(s.dn_state)))) {
        s.dn_state[i] = 0.0
    }
}

// The qwen35 decode attention block: forward-only state guard, then deltanet or gated attention
// per the layer mask.
def private attention_qwen35_decode(t : Model; var s : Session; l : int64; pos : int64) {
    if (l == 0l) {   // recurrent state is forward-only: full reset at 0, no rewind
        if (pos == 0l) {
            dn_reset(s)
        } elif (pos != s.dn_pos) {
            panic("dasLLAMA: deltanet state is forward-only — cannot eval position {pos} (session is at {s.dn_pos}); rewinding needs a fresh session")
        }
        s.dn_pos = pos + 1l
    }
    if (layer_is_recurrent(t.config, l)) {
        trace_tag(TRACE_TAG_ATTN_STD)
        deltanet_decode(t, s, l)
    } else {
        attention_qwen35_gated_decode(t, s, l, pos)
    }
}

// ffn-norm -> gated FFN (act(W1·x) * (W3·x)) -> down-proj(+post-norm) -> residual.
// Dense gated FFN kernel, in place: reads pre-normed activation in s.xb, writes act(W1·xb)·(W3·xb)
// projected through W2 back into s.xb. Shared by ffn_dense_decode and gemma4's dense shared expert.
def private dense_ffn_inplace(t : Model; var s : Session; l : int64) {
    let c = t.config
    let dim = c.dim
    let hidden = layer_hidden(t, l)   // gemma-4 E2B varies FFN width per layer
    let ts_ffn = ref_time_ticks()
    let f1 = fmt_at(t.w1_fmt, l)
    let f2 = fmt_at(t.w2_fmt, l)
    let f3 = fmt_at(t.w3_fmt, l)
    if (f1 != KqFmt.q8 || f2 != KqFmt.q8 || f3 != KqFmt.q8) {
        // K-quant FFN (formats mix, e.g. Q4_K_M keeps ffn_down at Q6_K): one bs-quantize serves every format
        if (f1 == KqFmt.q8 || f3 == KqFmt.q8) {
            quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
        }
        if (f1 != KqFmt.q8 || f3 != KqFmt.q8) {   // kq v2: every kq format reads the Q8_K form
            quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
        }
        mm_pre_f(s.hb, t, f1, t.w1_offs[l], s.xq, s.xs, s.kxq, s.kxs, s.kxbs, dim, hidden)
        mm_pre_f(s.hb2, t, f3, t.w3_offs[l], s.xq, s.xs, s.kxq, s.kxs, s.kxbs, dim, hidden)
        prof_add("mm_ffn", ts_ffn)
        let ts_act = ref_time_ticks()
        gate_batch(s.hb, s.hb2, c.ffn_act, hidden, 1l)
        prof_add("act", ts_act)
        let ts_down = ref_time_ticks()
        if (f2 == KqFmt.q8) {
            quantize_q8_0_into(s.hb, hidden, s.xq, s.xs, 0l, 0l)
        } else {   // kq v2
            quantize_q8_k_into(s.hb, hidden, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
        }
        mm_pre_f(s.xb, t, f2, t.w2_offs[l], s.xq, s.xs, s.kxq, s.kxs, s.kxbs, hidden, dim)
        prof_add("mm_ffn", ts_down)
    } elif (t.quant == QuantMode.q8) {
        // gate+up share the activation: ONE quantize + ONE region-list dispatch into ffn_gu's halves
        quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
        s.ffn_offs[0] = t.w1_offs[l]
        s.ffn_offs[1] = 0l
        s.ffn_offs[2] = t.w3_offs[l]
        s.ffn_offs[3] = 0l
        mm_at_q8_groupn(s.ffn_gu, t, s.ffn_offs, 2l, s.xq, s.xs, dim, hidden)
        prof_add("mm_ffn", ts_ffn)
        let ts_act = ref_time_ticks()
        unsafe {
            gate_batch(addr(s.ffn_gu[0]), addr < float const? >(s.ffn_gu[hidden]), c.ffn_act, hidden, 1l)
        }
        prof_add("act", ts_act)
        // down projection off the gated half
        let ts_down = ref_time_ticks()
        quantize_q8_0_into(s.ffn_gu, hidden, s.xq, s.xs, 0l, 0l)
        mm_at_q8_pre(s.xb, t, t.w2_offs[l], s.xq, s.xs, hidden, dim)
        prof_add("mm_ffn", ts_down)
    } else {
        mm(s.hb, t, t.w1_offs[l], s.xb, s.xq, s.xs, dim, hidden)
        mm(s.hb2, t, t.w3_offs[l], s.xb, s.xq, s.xs, dim, hidden)
        prof_add("mm_ffn", ts_ffn)
        let ts_act = ref_time_ticks()
        gate_batch(s.hb, s.hb2, c.ffn_act, hidden, 1l)   // fused act*gate; same kernel as prefill (npos=1 → inline)
        prof_add("act", ts_act)
        let ts_ffn2 = ref_time_ticks()
        mm(s.xb, t, t.w2_offs[l], s.hb, s.xq, s.xs, hidden, dim)
        prof_add("mm_ffn", ts_ffn2)
    }
}

def private ffn_dense_decode(t : Model; var s : Session; l : int64) {
    if (fused_ffn_ok(t, l)) {
        ffn_chain_decode(t, s, l)
        return
    }
    trace_tag(TRACE_TAG_FFN_STD)
    let c = t.config
    let dim = c.dim
    // ffn rmsnorm
    let ts_norm1 = ref_time_ticks()
    rms_at(s.xb, t, t.rms_ffn_off + l * dim, s.x, dim)
    prof_add("norm", ts_norm1)
    // gated FFN: hb = act(W1·xb) * (W3·xb) — silu (SwiGLU) or gelu (GeGLU, Gemma2)
    dense_ffn_inplace(t, s, l)
    let ts_resid1 = ref_time_ticks()
    if (c.pre_post_norm) {  // Gemma2: post-FFN RMSNorm before the residual add
        rms_at(s.xb, t, t.rms_post_ffn_off + l * dim, s.xb, dim)
    }
    // gemma4: E-series applies layer-output scale AFTER its PLE block, so skip it here when PLE is on
    if (c.layer_out_scale && !has_ple(c)) {
        add_scale_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb[0])), t.fblob[t.out_scale_off + l], dim)
    } else {
        add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb[0])), dim)
    }
    prof_add("act_resid", ts_resid1)
}

// The MoE FFN core shared by decode and (naive per-token) prefill: reads rmsnormed activation in
// s.xb, leaves combined routed + shared-expert output in s.moe_acc. Router probs taken over ALL
// experts BEFORE top-k (llama.cpp build_moe_ffn order); off is 0 for decode, p*k for grouped prefill.
def private moe_select(c : Config; var lg : float?; var idx : array<int64>; var w : array<float>; off : int64; bias : float const? = null) {
    unsafe {
        moe_select_core(c.moe_gate, c.n_expert, c.n_expert_used, c.norm_topk_prob, c.expert_weights_scale,
            lg, addr(idx[0]), addr(w[0]), off, bias)
    }
}

// pointer core — the threaded prefill router runs this in fork workers (scalars + raw pointers
// only; Config/Session stay on the caller). `bias` (exp_probs_b, DeepSeek-V3/glm4moe) skews the
// top-k PICK only — the stored weights stay the unbiased probs (llama.cpp build_moe_ffn order).
def private moe_select_core(gate : MoeGate; ne, k : int64; norm_topk : bool; wscale : float;
                            var lg : float?; var idx : int64?; var w : float?; off : int64;
                            bias : float const? = null) {
    unsafe {   // the whole body indexes the logit-row pointer (in-place gate + top-k knockout)
    // probs — softmax_weight routes on the RAW biased logits, softmaxes only the selected top-k after
    if (gate == MoeGate.sigmoid) {
        var i = 0l
        while (i + 4l <= ne) {
            let e = exp4(clamp(-float4(lg[i], lg[i + 1l], lg[i + 2l], lg[i + 3l]), EXP4_LO, EXP4_HI))
            lg[i] = 1.0 / (1.0 + e.x)
            lg[i + 1l] = 1.0 / (1.0 + e.y)
            lg[i + 2l] = 1.0 / (1.0 + e.z)
            lg[i + 3l] = 1.0 / (1.0 + e.w)
            i += 4l
        }
        while (i < ne) {
            lg[i] = 1.0 / (1.0 + exp(clamp(-lg[i], -80.0, 80.0)))
            i++
        }
    } elif (gate == MoeGate.softmax) {
        var mx = lg[0]
        for (i in range64(ne)) {
            mx = max(mx, lg[i])
        }
        let m4 = float4(mx, mx, mx, mx)
        var sum4 = float4(0.0)
        var i = 0l
        while (i + 4l <= ne) {
            let e = exp4(max(float4(lg[i], lg[i + 1l], lg[i + 2l], lg[i + 3l]) - m4, EXP4_LO))
            lg[i] = e.x
            lg[i + 1l] = e.y
            lg[i + 2l] = e.z
            lg[i + 3l] = e.w
            sum4 += e
            i += 4l
        }
        var sum = sum4.x + sum4.y + sum4.z + sum4.w
        while (i < ne) {
            let e = exp(max(lg[i] - mx, -80.0))
            lg[i] = e
            sum += e
            i++
        }
        for (j in range64(ne)) {
            lg[j] /= sum
        }
    }
    // top-k by score, winner knocked out with -FLT_MAX; with a bias the pick compares lg[i]+bias[i] but stores UNBIASED lg[best]
    if (bias == null) {
        for (j in range64(k)) {
            var best = 0l
            for (i in range64(ne)) {
                if (lg[i] > lg[best]) {
                    best = i
                }
            }
            idx[off + j] = best
            w[off + j] = lg[best]
            lg[best] = -FLT_MAX
        }
    } else {
        for (j in range64(k)) {
            var best = 0l
            var bs = -FLT_MAX
            for (i in range64(ne)) {
                if (lg[i] == -FLT_MAX) {   // knocked out — a huge/inf file bias must not resurrect it
                    continue
                }
                let sc = lg[i] + bias[i]
                if (sc > bs) {
                    best = i
                    bs = sc
                }
            }
            idx[off + j] = best
            w[off + j] = lg[best]
            lg[best] = -FLT_MAX
        }
    }
    if (gate == MoeGate.softmax_weight) {  // gpt-oss: softmax over just the selected k logits
        softmax(w + off, k)
    }
    if (norm_topk) {   // renormalize the selected weights to sum 1 (llama.cpp clamps at f16-min)
        var ws = 0.0
        for (j in range64(k)) {
            ws += w[off + j]
        }
        ws = max(ws, 6.103515625e-5)
        for (j in range64(k)) {
            w[off + j] /= ws
        }
    }
    if (wscale != 0.0 && wscale != 1.0) {
        for (j in range64(k)) {
            w[off + j] *= wscale
        }
    }
    }
}

// The shared expert's scalar sigmoid gate: σ(gate_vec_l · x). One body for decode and prefill.
// Tuned dot on raw pointers, not an array_view loop — the block invoke + bounds-checked indexing
// was an optimization boundary around a plain dot (reassociates the sum: tolerance-level change).
def private moe_shexp_gate(t : Model; l : int64; xp : float const?) : float {
    let dim = t.config.dim
    unsafe {
        return 1.0 / (1.0 + exp(-dot(addr(t.fblob[t.shexp_gate_off + l * dim]), xp, dim)))
    }
}

// Routed experts, given the pre-normed input in s.xb and the top-k selection in s.moe_idx/s.moe_w:
// s.moe_acc = Σ_j w_j · FFN_e_j(xb). Shared by qwen2moe/gpt-oss (via moe_ffn_core) and gemma4
// (which folds its per-expert down scale into s.moe_w first).
def private moe_experts_apply(t : Model; var s : Session; l : int64) {
    trace_tag(TRACE_TAG_MOE)
    let c = t.config
    let dim = c.dim
    let ne = c.n_expert
    let k = c.n_expert_used
    for (i in range64(dim)) {
        s.moe_acc[i] = 0.0
    }
    // Q8: quantize the shared activation ONCE (dedicated moe_xq/moe_xs so down-proj requant doesn't clobber it)
    let pre_q8 = t.quant == QuantMode.q8
    let layer_kq = pre_q8 && moe_layer_kq(t, l)
    if (pre_q8) {
        let ts_rq = ref_time_ticks()
        if (layer_kq && moe_pair_kform(fmt_at(t.we1_fmt, l), fmt_at(t.we3_fmt, l))) {
            // kq v2: kq gate/up stacks read the Q8_K-form scratch
            quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
            if (c.n_ff_shexp > 0l && !c.moe_dense_shexp) {
                // the q8 shexp downstream reads the hoisted moe_xq image — without this it silently drops out
                quantize_q8_0_into(s.xb, dim, s.moe_xq, s.moe_xs, 0l, 0l)
            }
        } else {
            quantize_q8_0_into(s.xb, dim, s.moe_xq, s.moe_xs, 0l, 0l)
        }
        prof_add("mm_requant", ts_rq)
    }
    let nfe = c.n_ff_exp
    let ege = dim * nfe
    if (pre_q8) {
        // fused-dispatch form: bit-exact vs the per-expert loop, but 3 fork/joins instead of 3k
        let ts_gu = ref_time_ticks()
        let biased = c.moe_exps_bias   // gpt-oss: each expert's biases fold into the workers' stores
        let f1 = fmt_at(t.we1_fmt, l)
        let f2 = fmt_at(t.we2_fmt, l)
        let f3 = fmt_at(t.we3_fmt, l)
        // GPU MoE tier: unbiased plain-q8 or all-K-quant stacks of offloaded layers dispatch on the GPU
        let gpu_moe = !biased && !t.experts_mx4 && c.ffn_act != FfnAct.swiglu_oai && moe_gpu_layer_on_gpu(l)
        if (gpu_moe) {
            // region triples (slot j = row j) + a k-row gathered image; gate+up/act/requant/down runs device-side into moe_dn
            s.moe_offs1 |> clear(); s.moe_offs1 |> reserve(k * 3l)
            s.moe_offs3 |> clear(); s.moe_offs3 |> reserve(k * 3l)
            s.moe_offs2 |> clear(); s.moe_offs2 |> reserve(k * 3l)
            for (j in range64(k)) {
                let base = s.moe_idx[j] * ege
                s.moe_offs1 |> push(t.we1_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
                s.moe_offs1 |> push(j)
                s.moe_offs1 |> push(1l)
                s.moe_offs3 |> push(t.we3_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
                s.moe_offs3 |> push(j)
                s.moe_offs3 |> push(1l)
                s.moe_offs2 |> push(t.we2_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
                s.moe_offs2 |> push(j)
                s.moe_offs2 |> push(1l)
            }
            // kq layers gather the Q8_K image; q8 layers the Q8_0 one — the tier resolves the form per stack
            let nb = layer_kq ? dim / 256l : dim / 32l
            s.moe_gxq |> resize(k * dim)
            s.moe_gxs |> resize(k * nb)
            unsafe {
                var gq = addr(s.moe_gxq[0])
                var gs = addr(s.moe_gxs[0])
                let sxq = layer_kq ? addr<void?>(s.kxq[0]) : addr<void?>(s.moe_xq[0])
                let sxs = layer_kq ? addr<void?>(s.kxs[0]) : addr<void?>(s.moe_xs[0])
                for (j in range64(k)) {
                    memcpy(reinterpret<void?>(gq + j * dim), sxq, int(dim))
                    memcpy(reinterpret<void?>(gs + j * nb), sxs, int(nb * 4l))
                }
            }
            matmul_moe_gpu_ffn(s.moe_dn, s.moe_offs1, s.moe_offs3, s.moe_offs2, k,
                s.moe_gxq, s.moe_gxs, dim, nfe, k, int(f1), int(f3), int(f2), c.ffn_act == FfnAct.gelu)
            prof_add("mm_moe", ts_gu)
        } else {
            for (j in range64(k)) {
                let e = l * ne + s.moe_idx[j]
                s.moe_offs[j * 2l] = t.we1_offs[l] + s.moe_idx[j] * ege
                s.moe_offs[j * 2l + 1l] = 0l   // gates share the single hoisted xb image
                s.moe_boffs[j] = t.web1_off + e * nfe
            }
            if (f1 != KqFmt.q8) {   // native K-quant expert stack reads the Q8_K-form scratch (never biased — detect guards that)
                mm_at_kq_groupn(s.moe_gu, t, f1, s.moe_offs, k, s.kxq, s.kxs, s.kxbs, dim, nfe)
            } elif (t.experts_mx4) {   // native 4-bit expert weights — half the Q8 traffic
                if (biased) {
                    mm_at_mx4_groupn(s.moe_gu, t, s.moe_offs, s.moe_boffs, k, s.moe_xq, s.moe_xs, dim, nfe)
                } else {
                    mm_at_mx4_groupn(s.moe_gu, t, s.moe_offs, k, s.moe_xq, s.moe_xs, dim, nfe)
                }
            } elif (biased) {
                mm_at_q8_groupn(s.moe_gu, t, s.moe_offs, s.moe_boffs, k, s.moe_xq, s.moe_xs, dim, nfe)
            } else {
                mm_at_q8_groupn(s.moe_gu, t, s.moe_offs, k, s.moe_xq, s.moe_xs, dim, nfe)
            }
            for (j in range64(k)) {
                let e = l * ne + s.moe_idx[j]
                s.moe_offs[j * 2l] = t.we3_offs[l] + s.moe_idx[j] * ege
                s.moe_offs[j * 2l + 1l] = 0l   // ups read the same hoisted xb image (explicit, not inherited from the gate pass)
                s.moe_boffs[j] = t.web3_off + e * nfe
            }
            if (f3 != KqFmt.q8) {   // kq v2: Q8_K-form scratch
                mm_at_kq_groupn(s.moe_gu2, t, f3, s.moe_offs, k, s.kxq, s.kxs, s.kxbs, dim, nfe)
            } elif (t.experts_mx4) {
                if (biased) {
                    mm_at_mx4_groupn(s.moe_gu2, t, s.moe_offs, s.moe_boffs, k, s.moe_xq, s.moe_xs, dim, nfe)
                } else {
                    mm_at_mx4_groupn(s.moe_gu2, t, s.moe_offs, k, s.moe_xq, s.moe_xs, dim, nfe)
                }
            } elif (biased) {
                mm_at_q8_groupn(s.moe_gu2, t, s.moe_offs, s.moe_boffs, k, s.moe_xq, s.moe_xs, dim, nfe)
            } else {
                mm_at_q8_groupn(s.moe_gu2, t, s.moe_offs, k, s.moe_xq, s.moe_xs, dim, nfe)
            }
            prof_add("mm_moe", ts_gu)
            let ts_act = ref_time_ticks()
            gate_batch(s.moe_gu, s.moe_gu2, c.ffn_act, nfe, k)
            prof_add("act", ts_act)
            if (f2 != KqFmt.q8) {   // kq v2: the down stack reads Q8_K-form rows
                requant_rows_q8k_bs(s.moe_gu, nfe, k, s.moe_dq, s.moe_ds, s.moe_dbs)
            } else {
                requant_rows_q8(s.moe_gu, nfe, k, s.moe_dq, s.moe_ds)   // books mm_requant
            }
            let ts_down = ref_time_ticks()
            for (j in range64(k)) {
                let e = l * ne + s.moe_idx[j]
                s.moe_offs[j * 2l] = t.we2_offs[l] + s.moe_idx[j] * ege
                s.moe_offs[j * 2l + 1l] = j * nfe   // each down reads its own requantized row
                s.moe_boffs[j] = t.web2_off + e * dim   // down bias inside the routed weight (ggml_add_id order)
            }
            if (f2 != KqFmt.q8) {
                mm_at_kq_groupn(s.moe_dn, t, f2, s.moe_offs, k, s.moe_dq, s.moe_ds, s.moe_dbs, nfe, dim)
            } elif (t.experts_mx4) {
                if (biased) {
                    mm_at_mx4_groupn(s.moe_dn, t, s.moe_offs, s.moe_boffs, k, s.moe_dq, s.moe_ds, nfe, dim)
                } else {
                    mm_at_mx4_groupn(s.moe_dn, t, s.moe_offs, k, s.moe_dq, s.moe_ds, nfe, dim)
                }
            } elif (biased) {
                mm_at_q8_groupn(s.moe_dn, t, s.moe_offs, s.moe_boffs, k, s.moe_dq, s.moe_ds, nfe, dim)
            } else {
                mm_at_q8_groupn(s.moe_dn, t, s.moe_offs, k, s.moe_dq, s.moe_ds, nfe, dim)
            }
            prof_add("mm_moe", ts_down)
        }
        let ts_red = ref_time_ticks()
        for (j in range64(k)) {   // slot order preserved — the sum is bit-identical
            axpy(unsafe(addr(s.moe_acc[0])), unsafe(addr(s.moe_dn[j * dim])), s.moe_w[j], dim)
        }
        prof_add("moe_reduce", ts_red)
    } else {
        // fp32 reference path: the per-expert loop (q4_0 never routes here — MoE loads are q8/fp32)
        for (j in range64(k)) {
            let e = l * ne + s.moe_idx[j]
            let base = s.moe_idx[j] * ege
            let ts_gu = ref_time_ticks()
            mm(s.hb, t, t.we1_offs[l] + base, s.xb, s.xq, s.xs, dim, nfe)
            mm(s.hb2, t, t.we3_offs[l] + base, s.xb, s.xq, s.xs, dim, nfe)
            if (c.moe_exps_bias) {  // gpt-oss: this expert's gate/up biases before the activation
                add_bias(s.hb, 0l, t.fblob, t.web1_off + e * nfe, nfe)
                add_bias(s.hb2, 0l, t.fblob, t.web3_off + e * nfe, nfe)
            }
            prof_add("mm_moe", ts_gu)
            let ts_act = ref_time_ticks()
            gate_batch(s.hb, s.hb2, c.ffn_act, nfe, 1l)
            prof_add("act", ts_act)
            let ts_down = ref_time_ticks()
            mm(s.xb2, t, t.we2_offs[l] + base, s.hb, s.xq, s.xs, nfe, dim)
            if (c.moe_exps_bias) {  // ... and its down bias, inside the routed weight (ggml_add_id order)
                add_bias(s.xb2, 0l, t.fblob, t.web2_off + e * dim, dim)
            }
            prof_add("mm_moe", ts_down)
            let ts_red = ref_time_ticks()
            axpy(unsafe(addr(s.moe_acc[0])), unsafe(addr(s.xb2[0])), s.moe_w[j], dim)
            prof_add("moe_reduce", ts_red)
        }
    }
}

// MoE FFN core (qwen2moe / gpt-oss): router logits on the rmsnormed activation in s.xb -> softmax /
// sigmoid select -> routed experts (moe_experts_apply) -> optional sigmoid-gated shared expert.
// Leaves the combined output in s.moe_acc; callers add the post-norm / residual / layer-out scale.
def private moe_ffn_core(t : Model; var s : Session; l : int64) {
    let c = t.config
    let dim = c.dim
    let ne = c.n_expert
    trace_tag(TRACE_TAG_ROUTE)   // decode router GEMV — without this it rides the stale attn tag in traces
    let ts_route = ref_time_ticks()
    mm_fblob(s.moe_logits, t, t.router_off + l * ne * dim, s.xb, dim, ne)
    if (c.moe_router_bias) {
        add_bias(s.moe_logits, 0l, t.fblob, t.router_b_off + l * ne, ne)
    }
    var ebp : float const?
    if (c.moe_exp_probs) {
        ebp = unsafe(addr<float const?>(t.fblob[t.exp_probs_off + l * ne]))
    }
    moe_select(c, unsafe(addr(s.moe_logits[0])), s.moe_idx, s.moe_w, 0l, ebp)
    prof_add("moe_route", ts_route)
    moe_experts_apply(t, s, l)
    // shared expert, gated by a sigmoid scalar from its per-layer gate vector (glm4moe: ungated, g = 1)
    if (c.n_ff_shexp > 0l) {
        let pre_q8 = t.quant == QuantMode.q8
        let ts_sh = ref_time_ticks()
        trace_tag(TRACE_TAG_SHEXP)
        let g = c.moe_shexp_gated ? moe_shexp_gate(t, l, unsafe(addr(s.xb[0]))) : 1.0
        let nsh = c.n_ff_shexp
        let she = dim * nsh
        if (pre_q8) {
            // shexp gate+up share the hoisted xb quants: one region-list dispatch, act on the halves
            s.ffn_offs[0] = t.wsh1_off + l * she
            s.ffn_offs[1] = 0l
            s.ffn_offs[2] = t.wsh3_off + l * she
            s.ffn_offs[3] = 0l
            mm_at_q8_groupn(s.ffn_gu, t, s.ffn_offs, 2l, s.moe_xq, s.moe_xs, dim, nsh)
            unsafe {
                gate_batch(addr(s.ffn_gu[0]), addr < float const? >(s.ffn_gu[nsh]), c.ffn_act, nsh, 1l)
            }
            quantize_q8_0_into(s.ffn_gu, nsh, s.xq, s.xs, 0l, 0l)
            mm_at_q8_pre(s.xb2, t, t.wsh2_off + l * she, s.xq, s.xs, nsh, dim)
        } else {
            mm(s.hb, t, t.wsh1_off + l * she, s.xb, s.xq, s.xs, dim, nsh)
            mm(s.hb2, t, t.wsh3_off + l * she, s.xb, s.xq, s.xs, dim, nsh)
            gate_batch(s.hb, s.hb2, c.ffn_act, nsh, 1l)
            mm(s.xb2, t, t.wsh2_off + l * she, s.hb, s.xq, s.xs, nsh, dim)
        }
        axpy(unsafe(addr(s.moe_acc[0])), unsafe(addr(s.xb2[0])), g, dim)
        prof_add("mm_shexp", ts_sh)
    }
}

// ===== gemma-4 E-series per-layer embeddings (PLE) =====
// A per-(token, layer) side input mixed into each layer's output (mirrors llama.cpp gemma4's
// build_inp_per_layer + per_layer_embd). ple_inp is POSITION-major: (p, l, i) at p*(layers*ple)+l*ple+i.

// Pre-step finish (shared by decode/prefill): given the gathered+sqrt(ple)-scaled token rows already
// in s.ple_inp[0..npos*all), add the projected embedding branch. proj = rmsnorm((1/√dim)·(model_proj·
// embd)); ple_inp = (gathered + proj)/√2. embd_scaled is the sqrt(dim)-scaled residual (s.x / s.x_b).
def private ple_pre_finish(t : Model; var s : Session; embd_scaled : array<float>; npos : int64) {
    trace_tag(TRACE_TAG_PLE)   // decode: runs between cls and layer 0, would inherit the stale cls tag
    let c = t.config
    let ple = c.n_embd_per_layer
    let all = c.n_layers * ple
    scale_inplace(unsafe(addr(s.ple_inp[0])), sqrt(float(ple)), npos * all)
    // model_proj · embedding, then × 1/√dim — native-BF16 rows when kept (exact widen), else fp32 expand
    if (t.ple_model_bf16) {
        matmul_bf16_batch(s.ple_proj, t.bf16blob, t.ple_model_off, embd_scaled, c.dim, all, npos)
    } else {
        mm_fblob_b(s.ple_proj, t, t.ple_model_off, embd_scaled, c.dim, all, npos)
    }
    scale_inplace(unsafe(addr(s.ple_proj[0])), 1.0 / sqrt(float(c.dim)), npos * all)
    // RMSNorm each (position, layer) ple-slice with the single proj-norm vector
    rms_batch(s.ple_proj, s.ple_proj, t, t.ple_pnorm_off, ple, npos * c.n_layers)
    add_inplace(unsafe(addr(s.ple_inp[0])), unsafe(addr(s.ple_proj[0])), npos * all)
    scale_inplace(unsafe(addr(s.ple_inp[0])), 1.0 / sqrt(2.0), npos * all)
}

// One PLE token row (all = layers*ple elements, superblock-aligned by the detect gate) off the
// gather's storage: native disk-order K-quant planes (the table is excluded from the grp repack)
// or the q8 linear region on fallback loads.
def private ple_gather_row(t : Model; row : int64; var dst : array<float>; doff : int64) {
    let all = t.config.n_layers * t.config.n_embd_per_layer
    let eloff = t.ple_emb_off + row * all
    if (t.ple_emb_fmt == KqFmt.q8) {
        dequant_q8_row(t, eloff, all, unsafe(addr(dst[doff])))
        return
    }
    for (blk in range64(all / 256l)) {
        let sb = (eloff + blk * 256l) / 256l
        if (t.ple_emb_fmt == KqFmt.k4) {
            dequant_k4_plane_superblock(t.k4q, sb * K4_QSB, t.k4s, sb * K4_SSB, dst, doff + blk * 256l)
        } elif (t.ple_emb_fmt == KqFmt.k5) {
            dequant_k5_plane_superblock(t.k5q, sb * K5_QSB, t.k5s, sb * K5_SSB, dst, doff + blk * 256l)
        } elif (t.ple_emb_fmt == KqFmt.k6) {
            dequant_k6_plane_superblock(t.k6q, sb * K6_QSB, t.k6s, sb * K6_SSB, dst, doff + blk * 256l)
        } elif (t.ple_emb_fmt == KqFmt.q40) {
            dequant_q40_plane_superblock(t.q40q, sb * Q40_QSB, t.q40s, sb * Q40_SSB, dst, doff + blk * 256l)
        } else {
            panic("ple_gather_row: '{t.ple_emb_fmt}' has no kq plane pair")
        }
    }
}

// Decode pre-step: gather the single token's per-layer embedding row, then finish.
def private ple_pre_decode(t : Model; var s : Session; token : int64) {
    ple_gather_row(t, token, s.ple_inp, 0l)
    ple_pre_finish(t, s, s.x, 1l)
}

// Prefill pre-step: gather every position's per-layer embedding row, then finish (batched).
def private ple_pre_prefill(t : Model; var s : Session; tokens : array<int64>; npos : int64) {
    let all = t.config.n_layers * t.config.n_embd_per_layer
    for (p in range64(npos)) {
        ple_gather_row(t, tokens[p], s.ple_inp, p * all)
    }
    ple_pre_finish(t, s, s.x_b, npos)
}

// Prefill-from-embeddings (multimodal) pre-step: no token ids, so the gather uses the padding token
// (id 0) broadcast across positions — matching llama.cpp gemma4's build_inp_per_layer media branch.
def private ple_pre_prefill_pad(t : Model; var s : Session; npos : int64) {
    let all = t.config.n_layers * t.config.n_embd_per_layer
    for (p in range64(npos)) {
        ple_gather_row(t, 0l, s.ple_inp, p * all)
    }
    ple_pre_finish(t, s, s.x_b, npos)
}

// Per-layer block (decode): cur += post_norm(proj · (gelu(inp_gate·cur) ⊙ ple_inp[l])). In place on s.x.
def private ple_block_decode(t : Model; var s : Session; l : int64) {
    trace_tag(TRACE_TAG_PLE)
    let c = t.config
    let dim = c.dim
    let ple = c.n_embd_per_layer
    mm(s.ple_gate, t, t.ple_gate_off + l * dim * ple, s.x, s.xq, s.xs, dim, ple)   // dim -> ple
    gelu(s.ple_gate, ple)
    mul_inplace(unsafe(addr(s.ple_gate[0])), unsafe(addr(s.ple_inp[l * ple])), ple)  // ⊙ this layer's side input
    mm(s.xb, t, t.ple_proj_off + l * ple * dim, s.ple_gate, s.xq, s.xs, ple, dim)   // ple -> dim
    rms_at(s.xb, t, t.ple_post_off + l * dim, s.xb, dim)
    add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb[0])), dim)   // residual (pe_in + projected)
}

// Per-layer block (prefill, batched over npos). ple_inp is position-major so the layer-l side vector
// of position p sits at p*(layers*ple) + l*ple.
def private ple_block_prefill(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let dim = c.dim
    let ple = c.n_embd_per_layer
    let all = c.n_layers * ple
    mm_b(s.ple_gate, t, t.ple_gate_off + l * dim * ple, s.x_b, s.xqb, s.xsb, dim, ple, npos)
    gelu(s.ple_gate, npos * ple)
    for (p in range64(npos)) {
        mul_inplace(unsafe(addr(s.ple_gate[p * ple])), unsafe(addr(s.ple_inp[p * all + l * ple])), ple)
    }
    mm_b(s.xb_b, t, t.ple_proj_off + l * ple * dim, s.ple_gate, s.xqb, s.xsb, ple, dim, npos)
    rms_batch(s.xb_b, s.xb_b, t, t.ple_post_off + l * dim, dim, npos)
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb_b[0])), npos * dim)
}

// gemma4 MoE router input: weightless RMSNorm(attn_out) × (1/√dim) × ffn_gate_inp_s, into s.xb2.
// Distinct from the plain-matmul routers: the RMS is over attn_out (not the shared-expert pre-norm),
// weightless, then scaled by 1/√n_embd and a learned per-channel scale before the ffn_gate_inp matmul.
def private gemma4_router_tmp(t : Model; var s : Session; l : int64) {
    let dim = t.config.dim
    // sum of squares in DOUBLE (matches ggml_rms_norm): float drift here flips the discrete top-8 expert pick
    var ss = 0.0lf
    for (i in range64(dim)) {
        ss += double(s.moe_ao[i] * s.moe_ao[i])
    }
    let scale = 1.0 / sqrt(float(ss / double(dim)) + t.config.norm_eps)
    let inv_d = 1.0 / sqrt(float(dim))
    unsafe {
        let scp = addr < float const? >(t.fblob[t.moe_router_scale_off + l * dim])
        var op = addr(s.xb2[0])
        let ip = addr < float const? >(s.moe_ao[0])
        let si = scale * inv_d
        for (i in range64(dim)) {
            op[i] = ip[i] * si * scp[i]
        }
    }
}

// gemma4 MoE layer (26B-A4B): two parallel branches of the residual stream in s.moe_ao, SUMMED.
// routed = post_ffw_norm_2(Σ_j w_j · GeGLU_e_j(pre_ffw_norm_2(attn_out))); dense = a full dense
// shared expert, always on. Leaves post_ffw_norm(routed + dense) in s.moe_acc.
def private gemma4_moe_ffn(t : Model; var s : Session; l : int64) {
    let c = t.config
    let dim = c.dim
    let ne = c.n_expert
    let k = c.n_expert_used
    // routed branch — input, custom router, select, per-expert down scale, experts, post-norm
    rms_at(s.xb, t, t.rms_pre_ffn2_off + l * dim, s.moe_ao, dim)
    trace_tag(TRACE_TAG_ROUTE)   // decode router GEMV — without this it rides the stale attn tag in traces
    let ts_route = ref_time_ticks()
    gemma4_router_tmp(t, s, l)
    mm_fblob(s.moe_logits, t, t.router_off + l * ne * dim, s.xb2, dim, ne)
    moe_select(c, unsafe(addr(s.moe_logits[0])), s.moe_idx, s.moe_w, 0l)
    for (j in range64(k)) {   // ggml scales each expert's down output by ffn_down_exps_s — fold it into w_j
        s.moe_w[j] *= t.fblob[t.moe_down_scale_off + l * ne + s.moe_idx[j]]
    }
    prof_add("moe_route", ts_route)
    moe_experts_apply(t, s, l)
    rms_at(s.moe_acc, t, t.rms_post_ffn2_off + l * dim, s.moe_acc, dim)
    // dense parallel shared expert (its own pre/post norms), into s.xb
    let ts_sh = ref_time_ticks()
    trace_tag(TRACE_TAG_SHEXP)
    rms_at(s.xb, t, t.rms_ffn_off + l * dim, s.moe_ao, dim)
    dense_ffn_inplace(t, s, l)
    rms_at(s.xb, t, t.rms_post_ffn1_off + l * dim, s.xb, dim)
    prof_add("mm_shexp", ts_sh)
    // combine then the shared post_ffw_norm on the sum
    add_inplace(unsafe(addr(s.moe_acc[0])), unsafe(addr(s.xb[0])), dim)
    rms_at(s.moe_acc, t, t.rms_post_ffn_off + l * dim, s.moe_acc, dim)
}

// gemma4 FFN decode: dense (12B/31B) or the parallel dense-shared + routed MoE (26B-A4B) by expert count.
def private ffn_gemma4_decode(t : Model; var s : Session; l : int64) {
    let c = t.config
    if (c.n_expert <= 0l) {
        ffn_dense_decode(t, s, l)   // out_scale deferred to after the PLE block when has_ple
        if (has_ple(c)) {  // E-series: per-layer embedding block sits between the FFN residual and out_scale
            let ts_ple = ref_time_ticks()
            ple_block_decode(t, s, l)
            if (c.layer_out_scale) {
                scale_inplace(unsafe(addr(s.x[0])), t.fblob[t.out_scale_off + l], c.dim)
            }
            prof_add("ple", ts_ple)
        }
        return
    }
    let dim = c.dim
    copy_floats(s.x, 0l, s.moe_ao, 0l, dim)
    gemma4_moe_ffn(t, s, l)
    let ts_resid = ref_time_ticks()
    if (c.layer_out_scale) {
        add_scale_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.moe_acc[0])), t.fblob[t.out_scale_off + l], dim)
    } else {
        add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.moe_acc[0])), dim)
    }
    prof_add("act_resid", ts_resid)
}

// gemma4 FFN prefill: dense delegates; MoE takes the grouped expert-bucketed path (Q8, default) or the
// per-position reference loop (fp32 loads and the bit-identical A/B switch).
def private ffn_gemma4_prefill(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    if (c.n_expert <= 0l) {
        ffn_dense_prefill(t, s, l, npos)   // out_scale deferred to after the PLE block when has_ple
        if (has_ple(c)) {  // E-series per-layer embedding block, then out_scale
            let ts_ple = ref_time_ticks()
            ple_block_prefill(t, s, l, npos)
            if (c.layer_out_scale) {
                scale_inplace(unsafe(addr(s.x_b[0])), t.fblob[t.out_scale_off + l], npos * c.dim)
            }
            prof_add("ple", ts_ple)
        }
        return
    }
    let dim = c.dim
    // kq-expert layers ride grouped buckets via the kq batch tile; portable/no-kq backends take the per-position path
    let kq_grouped_ok = !moe_layer_kq(t, l) || (t.kq_repacked && kernel_backend_has_kq_batch())
    if (t.quant == QuantMode.q8 && g_moe_grouped_prefill && kq_grouped_ok) {
        gemma4_moe_prefill_grouped(t, s, l, npos)
    } else {
        for (p in range64(npos)) {
            copy_floats(s.x_b, p * dim, s.moe_ao, 0l, dim)
            gemma4_moe_ffn(t, s, l)
            add_inplace(unsafe(addr(s.x_b[p * dim])), unsafe(addr(s.moe_acc[0])), dim)
        }
    }
    if (c.layer_out_scale) {
        scale_inplace(unsafe(addr(s.x_b[0])), t.fblob[t.out_scale_off + l], npos * dim)
    }
}

// MoE FFN, decode: rmsnorm -> routed top-k + shared expert -> residual. Mirrors ffn_dense_decode's
// optional post-norm / layer-output-scale steps so future MoE arches compose with them.
def private ffn_moe_decode(t : Model; var s : Session; l : int64) {
    let c = t.config
    if (l < c.n_layer_dense_lead) {   // glm4moe: the leading layer(s) are plain dense FFN
        ffn_dense_decode(t, s, l)
        return
    }
    let dim = c.dim
    let ts_norm = ref_time_ticks()
    rms_at(s.xb, t, t.rms_ffn_off + l * dim, s.x, dim)
    prof_add("norm", ts_norm)
    moe_ffn_core(t, s, l)
    let ts_resid = ref_time_ticks()
    if (c.pre_post_norm) {
        rms_at(s.moe_acc, t, t.rms_post_ffn_off + l * dim, s.moe_acc, dim)
    }
    if (c.layer_out_scale) {
        add_scale_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.moe_acc[0])), t.fblob[t.out_scale_off + l], dim)
    } else {
        add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.moe_acc[0])), dim)
    }
    prof_add("act_resid", ts_resid)
}

// Q8_0 row dequant from the linear embedding copy: dst[i] = q[i] * scale[i/32] — the same
// arithmetic gguf_read_tensor_f32 used to fill fblob, so the values are bit-identical to the
// fp32 table this replaces.
def private dequant_q8_row(t : Model; off, n : int64; var dst : float?) {
    unsafe {
        let qp = addr(t.qblob[off])
        if (t.wscale_f16) {
            let sp = addr(t.qscales16[off / 32l])
            for (b in range64(n / 32l)) {
                let sc = f16_to_f32(uint(sp[b]))
                let base = b * 32l
                for (k in range64(32l)) {
                    dst[base + k] = float(qp[base + k]) * sc
                }
            }
        } else {
            let sp = addr(t.qscales[off / 32l])
            for (b in range64(n / 32l)) {
                let sc = sp[b]
                let base = b * 32l
                for (k in range64(32l)) {
                    dst[base + k] = float(qp[base + k]) * sc
                }
            }
        }
    }
}

// Copy token `token`'s embedding row into dst — from fp32 fblob, or (cls_q8) dequanted from the
// linear Q8 copy at emb_q8_off, or (cls_kq) from the K-quant plane at wcls_off. Rows are whole
// superblocks (dim % 256 == 0, a ggml K-quant invariant).
def private embed_row(t : Model; token : int64; var dst : float?) {
    let dim = t.config.dim
    if (t.cls_q8) {
        dequant_q8_row(t, t.emb_q8_off + token * dim, dim, dst)
    } elif (cls_kq(t)) {
        let mr = (t.emb_fmt == KqFmt.k4 ? t.kq_repack_mr4
            : (t.emb_fmt == KqFmt.k5 ? t.kq_repack_mr5
            : (t.emb_fmt == KqFmt.k6 ? t.kq_repack_mr6 : t.kq_repack_mr40)))
        if (t.kq_repacked && token < (t.config.vocab_size / mr) * mr) {
            // repacked plane: gather via the grp<mr> layout (tail rows below stay disk-order, verbatim)
            unsafe {
                let nsb = dim / 256l
                let g = token / mr
                let fmt = int64(kq_fi(t.emb_fmt))
                let sb0t = t.wcls_off / 256l
                if (t.emb_fmt == KqFmt.k4) {
                    dequant_kq_row_grp(fmt, addr(t.k4q[(sb0t + g * mr * nsb) * K4_QSB]), addr(t.k4s[(sb0t + g * mr * nsb) * K4_SSB]), token % mr, mr, dim, dst)
                } elif (t.emb_fmt == KqFmt.k5) {
                    dequant_kq_row_grp(fmt, addr(t.k5q[(sb0t + g * mr * nsb) * K5_QSB]), addr(t.k5s[(sb0t + g * mr * nsb) * K5_SSB]), token % mr, mr, dim, dst)
                } elif (t.emb_fmt == KqFmt.k6) {
                    dequant_kq_row_grp(fmt, addr(t.k6q[(sb0t + g * mr * nsb) * K6_QSB]), addr(t.k6s[(sb0t + g * mr * nsb) * K6_SSB]), token % mr, mr, dim, dst)
                } elif (t.emb_fmt == KqFmt.q40) {
                    dequant_kq_row_grp(fmt, addr(t.q40q[(sb0t + g * mr * nsb) * Q40_QSB]), addr(t.q40s[(sb0t + g * mr * nsb) * Q40_SSB]), token % mr, mr, dim, dst)
                } else {
                    panic("embed_row: '{t.emb_fmt}' has no kq plane pair")
                }
            }
        } else {
            unsafe {
                var row = temp_array(dst, dim, type<float>)
                let sb0 = (t.wcls_off + token * dim) / 256l
                for (s in range64(dim / 256l)) {
                    if (t.emb_fmt == KqFmt.k4) {
                        dequant_k4_plane_superblock(t.k4q, (sb0 + s) * K4_QSB, t.k4s, (sb0 + s) * K4_SSB, row, s * 256l)
                    } elif (t.emb_fmt == KqFmt.k5) {
                        dequant_k5_plane_superblock(t.k5q, (sb0 + s) * K5_QSB, t.k5s, (sb0 + s) * K5_SSB, row, s * 256l)
                    } elif (t.emb_fmt == KqFmt.k6) {
                        dequant_k6_plane_superblock(t.k6q, (sb0 + s) * K6_QSB, t.k6s, (sb0 + s) * K6_SSB, row, s * 256l)
                    } elif (t.emb_fmt == KqFmt.q40) {
                        dequant_q40_plane_superblock(t.q40q, (sb0 + s) * Q40_QSB, t.q40s, (sb0 + s) * Q40_SSB, row, s * 256l)
                    } else {
                        panic("embed_row: '{t.emb_fmt}' has no kq plane pair")
                    }
                }
            }
        }
    } else {
        copy_floats(dst, unsafe(addr(t.fblob[t.tok_emb_off + token * dim])), dim)
    }
}

// The classifier matmul y = wcls · xb: through the GPU tier's resident plane when the loader
// uploaded it (one whole-vocab GEMV; the activation quantizes to the plane's own form), else
// the CPU path for the classifier's format.
def private mm_cls(var y : array<float>; t : Model; x : array<float>; var xq : array<int8>; var xs : array<float>; var xbs : array<int>; n, d : int64) {
    let cls_fmt = t.config.shared_weights ? t.emb_fmt : t.wcls_fmt
    let tied_f32 = t.config.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8
    if (moe_gpu_cls_on_gpu(t.wcls_off)) {
        if (cls_fmt == KqFmt.q8) {
            quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
        } else {
            quantize_q8_k_into(x, n, xq, xs, xbs, 0l, 0l, 0l)
        }
        matmul_moe_gpu_cls(y, t.wcls_off, xq, xs, n, d, int(cls_fmt))
    } elif (tied_f32) {
        // tied weights (fp32/q4_0 loads): the classifier IS the fp32 embedding table
        mm_fblob(y, t, t.tok_emb_off, x, n, d)
    } elif (cls_fmt != KqFmt.q8) {
        // K-quant classifier (tied plane copy or untied kq output.weight)
        mm_kq(y, t, cls_fmt, t.wcls_off, x, xq, xs, xbs, n, d)
    } else {
        // untied — or the tied classifier's Q8 transcode (the exact quants llama.cpp matmuls)
        mm(y, t, t.wcls_off, x, xq, xs, n, d)
    }
}

// Can the classifier run as the fused matmul+softcap team chain? Q8 or repacked-kq wcls (the
// tied-f32 load takes mm_fblob and can't), team mode + a backend row core, 32-aligned vocab,
// and worth threading.
def private fused_cls_ok(t : Model) : bool {
    let c = t.config
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    return (g_fused_decode && t.quant == QuantMode.q8
        && kq_servable(t, cls_fmt)
        && get_jobque_team_mode() && kernel_backend_has_rows()
        && (!t.wscale_f16 || kernel_backend_has_rows16())
        && c.vocab_size % 32l == 0l
        && lanes_for_work(c.dim * c.vocab_size, g_gemv_lane_cap) > 1)
}

// Fused classifier + final-logit softcap: one team chain over 32-row vocab groups, each chunk
// soft-capping cache-hot rows — serial softcap over a 256k vocab was 2-4ms every-lane-idle/token.
// Bit-identical to the unfused mm + softcap4 pair (same row dots, same element order).
def private cls_softcap_chain(t : Model; var s : Session; cap : float) {
    let c = t.config
    let dim = c.dim
    let vocab = c.vocab_size
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    let is_kq = cls_fmt != KqFmt.q8
    if (is_kq) {
        quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)   // kq v2: the rows cores read Q8_K form
    } else {
        quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
    }
    let mmrows = mm_q8q8_rows_fn()
    let ngrp = int(vocab / 32l)
    let n0 = chain_stage_chunks(ngrp, int(vocab), dim * vocab)
    s.chain_stages |> resize(1)
    s.chain_stages[0] = int3(0, ngrp, n0)
    count_parallel_dispatch(min(n0, ngrp))
    unsafe {
        var lp = addr(s.logits[0])
        var xqp = is_kq ? addr(s.kxq[0]) : addr(s.xq[0])
        var xsp = is_kq ? addr(s.kxs[0]) : addr(s.xs[0])
        let f16w = t.wscale_f16
        let mmrows16 = mm_q8q8_rows16_fn()
        let wp = cls_fmt == KqFmt.q8 ? addr < int8 const? >(t.qblob[0]) + t.wcls_off : null
        var ws : float const? = null
        var ws16 : uint16 const? = null
        if (cls_fmt == KqFmt.q8) {
            if (f16w) {
                ws16 = addr < uint16 const? >(t.qscales16[0]) + t.wcls_off / 32l
            } else {
                ws = addr < float const? >(t.qscales[0]) + t.wcls_off / 32l
            }
        }
        var xbsw : int? = is_kq ? addr(s.kxbs[0]) : null
        let kqpc = kq_plane_q(t, cls_fmt, t.wcls_off)
        let kspc = kq_plane_s(t, cls_fmt, t.wcls_off)
        let rowsc = kq_rows_for(cls_fmt)
        team_parallel_stages(s.chain_stages) <| @(stage : int; jb : int; je : int) {
            unsafe {
                let r0 = int64(jb) * 32l
                let r1 = int64(je) * 32l
                if (is_kq) {
                    invoke(rowsc, lp, kqpc, kspc, xqp, xsp, xbsw, dim, r0, r1)
                } elif (f16w) {
                    invoke(mmrows16, lp, wp, ws16, xqp, xsp, dim, r0, r1)
                } else {
                    invoke(mmrows, lp, wp, ws, xqp, xsp, dim, r0, r1)
                }
                softcap4(lp + r0, r1 - r0, cap)
            }
        }
    }
}

def forward(t : Model; var s : Session; token, pos : int64) {
    let c = t.config
    let dim = c.dim
    kv_ensure(s, pos + 1l)   // paged sessions: grow the block table BEFORE any cache-pointer hoist

    // token embedding: row `token` into the residual stream
    let ts_embed = ref_time_ticks()
    embed_row(t, token, unsafe(addr(s.x[0])))
    if (c.embed_scale != 1.0) {  // Gemma2: scale the embedding by sqrt(dim)
        scale_inplace(unsafe(addr(s.x[0])), c.embed_scale, dim)
    }
    prof_add("embed", ts_embed)
    if (has_ple(c)) {  // gemma-4 E-series: build this token's per-(layer) side input before the stack
        let ts_ple = ref_time_ticks()
        ple_pre_decode(t, s, token)
        prof_add("ple", ts_ple)
    }

    // this position's RoPE cos/sin row(s), shared across all layers and q/k (the decode twin of
    // prefill's table: the classic path recomputes pow/cos/sin per PAIR per layer — bit-identical
    // values, trig deduplicated). Dual-rope arches get the sliding class's row alongside.
    if (g_use_rope_table) {
        let ts_rope_build = ref_time_ticks()
        // partial rotary (qwen35): the table spans rope_dim — frequencies run over the rotated span
        build_rope_table(s.rope_cos, s.rope_sin, t, c.rope_freq_base, c.rope_freq_scale, pos, 1l,
                         c.rope_dim > 0l ? c.rope_dim : c.head_size, true)
        if (has_dual_rope(c)) {
            let swa_hs = c.head_size_swa > 0l ? c.head_size_swa : c.head_size
            build_rope_table(s.rope_cos_swa, s.rope_sin_swa, t, c.rope_freq_base_swa, c.rope_freq_scale_swa,
                             pos, 1l, swa_hs, !c.rope_ff_global_only)
        }
        prof_add("rope_build", ts_rope_build)
    }

    // per-layer transformer stack — attention then FFN, each through the arch's registered block.
    // An active whole-decode override (select_decode_override) may claim the entire stack; it
    // returns false to decline (unsupported shape/config), which falls back to the CPU loop.
    var stack_done = false
    g_decode_logits_done = false
    if (!empty(g_decode_override_name)) {
        stack_done = invoke(g_decode_override, t, s, token, pos)
    }
    if (!stack_done) {
        for (l in range64(c.n_layers)) {
            t.blocks.attn_decode(t, s, l, pos)
            t.blocks.ffn_decode(t, s, l)
        }
    }

    return if (stack_done && g_decode_logits_done)   // the override already produced s.logits

    // final rmsnorm (into xb to avoid aliasing x as both in/out), then classifier
    let ts_final = ref_time_ticks()
    trace_tag(TRACE_TAG_CLS)
    rms_at(s.xb, t, t.rms_final_off, s.x, dim)
    if (c.n_layer_nextn > 0l) {   // stash h_nextn (llama.cpp t_h_nextn) for the MTP draft head
        copy_floats(s.xb, 0l, s.mtp_h, 0l, dim)
        s.mtp_h_pos1 = pos + 1l
    }
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    let tied_f32 = c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8
    if (c.final_logit_softcap > 0.0 && !tied_f32 && !moe_gpu_cls_on_gpu(t.wcls_off) && fused_cls_ok(t)) {
        // Gemma2/E-series: soft-cap fused into the classifier chunks (Q8 or repacked-kq wcls)
        cls_softcap_chain(t, s, c.final_logit_softcap)
    } else {
        mm_cls(s.logits, t, s.xb, s.xq, s.xs, s.xbs, dim, c.vocab_size)
        if (c.final_logit_softcap > 0.0) {  // Gemma2: soft-cap the output logits
            softcap4(s.logits, c.vocab_size, c.final_logit_softcap)
        }
    }
    for (id in t.suppress) {  // gemma4: pin the checkpoint's suppressed token ids to -inf
        s.logits[id] = -1.0e30
    }
    prof_add("final", ts_final)
}

//! One MTP/NextN draft step: from stashed trunk hidden `s.mtp_h` and `token`, produce draft logits
//! in s.logits. `pos` is one BELOW llama.cpp's convention — rotary is position-delta-invariant, so
//! shifting every MTP row down by one is mathematically identical. Overwrites s.mtp_h with h_nextn.
var private g_mtp_spec = false

//! Enable MTP/NextN self-speculative greedy decode: `forward_prefill` maintains the draft head's
//! KV incrementally, and spec-aware drivers draft + verify via [[mtp_spec_eval]]. No effect on
//! models without a NextN head; greedy-only.
def set_mtp_spec(on : bool) {
    g_mtp_spec = on
}

//! Current state of the [[set_mtp_spec]] gate.
def get_mtp_spec() : bool => g_mtp_spec

def forward_mtp(t : Model; var s : Session; token, pos : int64; want_logits : bool = true) {
    let c = t.config
    let dim = c.dim
    assert(c.n_layer_nextn > 0l)
    kv_ensure(s, pos + 1l)
    // eh_proj input: [enorm(embed(token)) ; hnorm(h)] — embedding first (llama.cpp concat order)
    embed_row(t, token, unsafe(addr(s.x[0])))
    rms_at(s.xb, t, t.mtp_enorm_off, s.x, dim)
    copy_floats(s.xb, 0l, s.mtp_cat, 0l, dim)
    rms_at(s.xb, t, t.mtp_hnorm_off, s.mtp_h, dim)
    copy_floats(s.xb, 0l, s.mtp_cat, dim, dim)
    // eh_proj: 2*dim -> dim into the residual stream (xq/xs cover 2*dim — sized max(xdim, ffw))
    if (t.mtp_ehproj_fmt != KqFmt.q8) {
        mm_kq(s.x, t, t.mtp_ehproj_fmt, t.mtp_ehproj_off, s.mtp_cat, s.xq, s.xs, s.xbs, 2l * dim, dim)
    } else {
        mm(s.x, t, t.mtp_ehproj_off, s.mtp_cat, s.xq, s.xs, 2l * dim, dim)
    }
    // this row's RoPE table (partial rotary span, like forward)
    if (g_use_rope_table) {
        build_rope_table(s.rope_cos, s.rope_sin, t, c.rope_freq_base, c.rope_freq_scale, pos, 1l,
                         c.rope_dim > 0l ? c.rope_dim : c.head_size, true)
    }
    // the draft block IS one trunk-shaped attention layer at index n_layers — reuse the arch blocks
    t.blocks.attn_decode(t, s, c.n_layers, pos)
    t.blocks.ffn_decode(t, s, c.n_layers)
    // warm/backfill calls only need the KV row written — the head norm + classifier (a large
    // share of the head's cost on small models: vocab x dim) run only when drafting
    return if (!want_logits)
    // head norm (shared_head_norm when shipped, else output_norm) + the trunk classifier
    rms_at(s.xb, t, t.mtp_headnorm_off >= 0l ? t.mtp_headnorm_off : t.rms_final_off, s.x, dim)
    copy_floats(s.xb, 0l, s.mtp_h, 0l, dim)   // the head's own h_nextn — recursive drafting handoff
    s.mtp_h_pos1 = 0l   // NOT a trunk hidden: the warm's seam row must not consume it
    mm_cls(s.logits, t, s.xb, s.xq, s.xs, s.xbs, dim, c.vocab_size)
}

//! Snapshot/restore the deltanet recurrent state (conv history + S + dn_pos) around a speculative
//! verify batch: a rejected draft has already advanced the state past itself. KV rows stay correct;
//! only the recurrent state rolls back, then one plain forward re-advances it.
def mtp_state_snapshot(var s : Session) {
    let nc = int64(length(s.dn_conv_state))
    let ns = int64(length(s.dn_state))
    s.mtp_snap_conv |> resize(nc)   // no-op after the first call
    s.mtp_snap_state |> resize(ns)
    copy_floats(s.dn_conv_state, 0l, s.mtp_snap_conv, 0l, nc)
    copy_floats(s.dn_state, 0l, s.mtp_snap_state, 0l, ns)
    s.mtp_snap_pos = s.dn_pos
}

def mtp_state_restore(var s : Session) {
    copy_floats(s.mtp_snap_conv, 0l, s.dn_conv_state, 0l, int64(length(s.mtp_snap_conv)))
    copy_floats(s.mtp_snap_state, 0l, s.dn_state, 0l, int64(length(s.mtp_snap_state)))
    s.dn_pos = s.mtp_snap_pos
}

// After a 2-row verify forward_prefill([tok, draft], 2, pos): row 0's post-norm hidden and
// logits (forward_prefill's own tail only produced row 1's). Reads s.x_b row 0 — still intact,
// the prefill tail never overwrites the batch residual stream.
def private mtp_verify_row0(t : Model; var s : Session) {
    let c = t.config
    let dim = c.dim
    rms_at(s.xb, t, t.rms_final_off, s.x_b, 0l, dim)
    mm_cls(s.mtp_logits, t, s.xb, s.xq, s.xs, s.xbs, dim, c.vocab_size)
    // mirror the plain classifier tail exactly — `truth` must match plain decode BY CONSTRUCTION
    if (c.final_logit_softcap > 0.0) {
        softcap4(s.mtp_logits, c.vocab_size, c.final_logit_softcap)
    }
    for (id in t.suppress) {
        s.mtp_logits[id] = -1.0e30
    }
}

// Write the draft head's IN-CHUNK KV rows for a just-prefilled [start_pos, start_pos+npos) window.
// Rows start_pos..start_pos+npos-2 ride one batched eh_proj GEMM + the arch's blocks at l = n_layers.
// x_b is saved/restored around the pass: post-prefill contents are load-bearing for mtp_verify_row0.
def private mtp_prefill_warm(t : Model; var s : Session; tokens : array<int64>; npos, start_pos : int64) {
    let c = t.config
    let dim = c.dim
    if (npos > 1l) {
        s.mtp_xb_save |> scratch_resize(npos * dim)
        copy_floats(s.x_b, 0l, s.mtp_xb_save, 0l, npos * dim)
        s.mtp_cat_b |> scratch_resize((npos - 1l) * 2l * dim)
        for (p in range64(npos - 1l)) {
            embed_row(t, tokens[p + 1l], unsafe(addr(s.x[0])))
            rms_at(s.xb, t, t.mtp_enorm_off, s.x, dim)
            copy_floats(s.xb, 0l, s.mtp_cat_b, p * 2l * dim, dim)
            rms_at(s.xb, t, t.rms_final_off, s.x_b, p * dim, dim)   // h_{start_pos+p}
            copy_floats(s.xb, 0l, s.x, 0l, dim)
            rms_at(s.xb, t, t.mtp_hnorm_off, s.x, dim)
            copy_floats(s.xb, 0l, s.mtp_cat_b, p * 2l * dim + dim, dim)
        }
        mm_b_f(s, s.x_b, t, t.mtp_ehproj_fmt, t.mtp_ehproj_off, s.mtp_cat_b, 2l * dim, dim, npos - 1l)
        t.blocks.attn_prefill(t, s, c.n_layers, npos - 1l, start_pos)
        t.blocks.ffn_prefill(t, s, c.n_layers, npos - 1l)
        copy_floats(s.mtp_xb_save, 0l, s.x_b, 0l, npos * dim)
    }
    // SEAM row start_pos-1 is written by forward_prefill BEFORE the trunk body, so it can't be done here
}

//! One greedy self-speculative eval step: evals `tok`, drafting one token ahead with the NextN
//! head and verifying with a 2-row batched trunk forward. On accept, `accepted` is the draft and
//! `s.logits` holds the second position's; else one position, plain-eval semantics. Requires [[set_mtp_spec]] ON.
def mtp_spec_eval(t : Model; var s : Session; tok : int64; var accepted : int64&) : bool {
    let c = t.config
    let pos = s.n_past
    // pos < 1: nothing evaled yet — the zero-init mtp_h_pos1 sentinel COLLIDES with pos == 0
    // (0 != 0 would pass the cold check and draft at row -1), so gate it explicitly
    if (c.n_layer_nextn <= 0l || t.quant == QuantMode.q4_0 || pos < 1l || s.mtp_h_pos1 != pos
            || pos + 2l > c.seq_len) {
        forward(t, s, tok, pos)   // plain step (forward re-stashes the handoff for the next tick)
        s.n_past = pos + 1l
        return false
    }
    // draft x_{pos+1} from (h_{pos-1}, tok) at row pos-1 (rewrites that row's draft KV in place)
    forward_mtp(t, s, tok, pos - 1l)
    let d = parallel_argmax(s.logits, c.vocab_size)
    s.mtp_drafted++
    // batched verify: trunk over [tok@pos, d@pos+1] — weights stream once for both rows; the
    // warm inside this prefill writes the head's row pos = (h_pos, d), which doubles as the
    // accept-path backfill (a reject leaves it stale; the next draft at that row rewrites it)
    mtp_state_snapshot(s)
    s.mtp_vbatch[0] = tok
    s.mtp_vbatch[1] = d
    forward_prefill(t, s, s.mtp_vbatch, 2l, pos)
    mtp_verify_row0(t, s)
    let truth = parallel_argmax(s.mtp_logits, c.vocab_size)
    if (truth == d) {
        s.mtp_accepted++
        accepted = d
        s.n_past = pos + 2l
        return true
    }
    // reject: the accepted row's KV from the verify batch is already correct; roll the recurrent
    // state back and re-advance it through tok alone (s.logits = plain logits, like forward)
    mtp_state_restore(s)
    forward(t, s, tok, pos)
    s.n_past = pos + 1l
    return false
}

//! Greedy decode with MTP/NextN depth-1 self-speculation. Mirrors generate()'s contract (echo +
//! greedy continuation, `total` ids) and is OUTPUT-INVARIANT vs plain greedy decode — the draft
//! only decides batched-two-token vs plain-one-token stepping. Accept telemetry in the out params.
def generate_mtp_greedy(t : Model; var s : Session; prompt : array<int64>; total : int64;
                        var n_drafted, n_accepted : int64&) : array<int64> {
    let c = t.config
    assert(c.n_layer_nextn > 0l)
    if (t.quant == QuantMode.q4_0) {   // no batched prefill rail: the 2-row verify (and the warm) need it
        panic("dasLLAMA: MTP self-spec decode needs the batched prefill path — load q8 or fp32, not q4_0")
    }
    let np = int64(length(prompt))
    var ids : array<int64>
    if (total <= 0l || np <= 0l) {
        return <- ids
    }
    ids |> reserve(total)
    for (i in range64(1l, np)) {   // generate()'s echo convention (incl. budget + BOS delimiter)
        if (int64(length(ids)) >= total || prompt[i] == 1l) {
            return <- ids
        }
        ids |> push(prompt[i])
    }
    // this API is explicit spec — force the engine gate on (the prompt prefill's warm and the
    // verify batches inside mtp_spec_eval both key off it), restore on the way out
    let prev_spec = g_mtp_spec
    g_mtp_spec = true
    forward_prefill(t, s, prompt, np, 0l)   // trunk prefill + head warm; stashes h_{np-1}
    s.n_past = np
    var tok = parallel_argmax(s.logits, c.vocab_size)
    let d0 = s.mtp_drafted
    let a0 = s.mtp_accepted
    while (int64(length(ids)) < total) {
        break if (tok == 1l)   // BOS delimits sequences (generate()'s contract)
        ids |> push(tok)
        break if (int64(length(ids)) >= total)
        var acc = 0l
        if (mtp_spec_eval(t, s, tok, acc)) {
            break if (acc == 1l)   // BOS: generate() stops before emitting it — mirror that
            ids |> push(acc)
            break if (int64(length(ids)) >= total)
        }
        tok = parallel_argmax(s.logits, c.vocab_size)
    }
    n_drafted = s.mtp_drafted - d0
    n_accepted = s.mtp_accepted - a0
    g_mtp_spec = prev_spec
    return <- ids
}

// ----- forward section profiler (decode AND prefill feed the same buckets) -----
// Measurement is UNCONDITIONAL and negligible vs the matmuls; reset() starts a window, report() dumps it.
// Prefill pass-threading thresholds (M1 Max ~90us dispatch, runtime knobs, 0 = always thread).
var g_attn_par_threshold = 100_000l
// Thread the per-position activation requant (mm_b) once n × npos clears this.
var g_requant_par_threshold = 256_000l
// Thread the per-position rmsnorm once dim × npos clears this.
var g_norm_par_threshold = 256_000l
// Thread the per-position rope (q_b + k_b) once (qd + kv_dim) × npos clears this.
var g_rope_par_threshold = 100_000l
// Thread the KV-cache store once kv_dim × npos clears this (it's a memory-bound contiguous copy).
var g_kv_store_par_threshold = 256_000l
// Thread the per-position FFN activation (silu/gelu) once hidden × npos clears this. Lower than the
// memory-bound passes — each element pays a transcendental (exp/tanh), so the compute justifies threading sooner.
var g_act_par_threshold = 100_000l

def set_attn_par_threshold(v : int64) { g_attn_par_threshold = max(v, 0l) }
def get_attn_par_threshold() : int64 => g_attn_par_threshold
def set_requant_par_threshold(v : int64) { g_requant_par_threshold = max(v, 0l) }
def get_requant_par_threshold() : int64 => g_requant_par_threshold
def set_norm_par_threshold(v : int64) { g_norm_par_threshold = max(v, 0l) }
def get_norm_par_threshold() : int64 => g_norm_par_threshold
def set_rope_par_threshold(v : int64) { g_rope_par_threshold = max(v, 0l) }
def get_rope_par_threshold() : int64 => g_rope_par_threshold
def set_kv_store_par_threshold(v : int64) { g_kv_store_par_threshold = max(v, 0l) }
def get_kv_store_par_threshold() : int64 => g_kv_store_par_threshold
def set_act_par_threshold(v : int64) { g_act_par_threshold = max(v, 0l) }
def get_act_par_threshold() : int64 => g_act_par_threshold

// Decode attention threads work-proportionally; the M1-measured threshold knob is now owned per
// box by target_chunk_work.
// Query-block width for blocked prefill attention (each K/V tile reused across this many queries).
let ATTN_BLOCK_Q = 8l
// Tile dims for the flash prefill attention (Q rows × KV cols processed per GEMM tile). The flash
// per-query running max/sum use fixed arrays sized ATTN_FLASH_QT — keep that a literal 64 in sync.
let ATTN_FLASH_QT = 64l
let ATTN_FLASH_KV = 64l

var g_attn_prefill_mode = AttnPrefillMode.flash   // A/B toggle; see AttnPrefillMode (flash is fastest)

//! Select the prefill attention algorithm (default flash). See AttnPrefillMode.
def set_prefill_attn_mode(m : AttnPrefillMode) {
    g_attn_prefill_mode = m
}

//! The prefill attention algorithm currently selected.
def get_prefill_attn_mode() : AttnPrefillMode {
    return g_attn_prefill_mode
}

var g_dn_prefill_mode = DnPrefillMode.chunked   // A/B toggle; see DnPrefillMode (chunked is fastest)

//! Select the deltanet prefill algorithm (default chunked). See DnPrefillMode.
def set_deltanet_prefill_mode(m : DnPrefillMode) {
    g_dn_prefill_mode = m
}

//! The deltanet prefill algorithm currently selected.
def get_deltanet_prefill_mode() : DnPrefillMode {
    return g_dn_prefill_mode
}

// Fused decode chains: ONE multi-stage team dispatch per attention block and per dense-FFN block
// instead of one per matmul — team_parallel_stages replaces the per-op rendezvous with worker-side
// stage barriers. Bit-identical to per-op at ctx <= FLASH_DECODE_SLICE; deeper contexts slice (tolerance-equal).
var g_fused_decode = true

//! Select the decode dispatch shape: fused per-block stage chains (default true) vs one dispatch
//! per matmul. Bit-identical either way at ctx <= FLASH_DECODE_SLICE; deeper contexts slice the
//! fused attention stage (flash-decode) and are tolerance-equal — A/B benchmarking toggle.
def set_fused_decode(on : bool) {
    g_fused_decode = on
}

//! Whether decode uses the fused per-block stage chains.
def get_fused_decode() : bool {
    return g_fused_decode
}

// test-only witness rail for the flash-decode slicing (see attention_chain_decode): when on,
// every sliced layer recomputes exact single-pass attention from the raw scores and panics past
// fp32 reassociation tolerance; the counter proves the sliced path actually ran
var g_flash_decode_check = false
var g_flash_decode_checked = 0

//! Enable the per-layer flash-decode witness check (tests only — serial recompute per layer).
def set_flash_decode_check(on : bool) {
    g_flash_decode_check = on
    g_flash_decode_checked = 0
}

//! Sliced-attention layers checked since set_flash_decode_check(true).
def get_flash_decode_check_count() : int {
    return g_flash_decode_checked
}

// Prefill RoPE: when true (default) precompute the cos/sin table once per prefill and look it up;
// when false fall back to the classic per-position pow/cos/sin path. A/B toggle for benchmarking
// (the two paths are bit-identical — the table just deduplicates the transcendentals across heads/layers).
var g_use_rope_table = true

//! Select the prefill RoPE path: table lookup (default true) vs classic per-position pow/cos/sin.
def set_rope_table_mode(on : bool) {
    g_use_rope_table = on
}

//! Whether prefill RoPE uses the precomputed cos/sin table.
def get_rope_table_mode() : bool {
    return g_use_rope_table
}

// FFN gate activation: when true (default) use the vectorized exp4 fused kernels (swiglu4/geglu4,
// ~2 ulp, non-bit-exact); when false the scalar libm swiglu/geglu (bit-exact reference). The oracle
// tests flip this off for their token-for-token gate; production leaves it on. Same fusion either way.
var g_act_vec = true

//! Select the FFN gate transcendental: vectorized exp4 (default true, fast, ~2 ulp) vs scalar libm
//! (bit-exact). Off is the reference path the token-for-token oracle tests pin.
def set_act_vec(on : bool) {
    g_act_vec = on
}

//! Whether the FFN gate uses the vectorized exp4 kernels.
def get_act_vec() : bool {
    return g_act_vec
}

// Grouped MoE prefill: when true (default) bucket positions by selected expert and run one batched
// GEMM per touched expert; when false fall back to the per-position reference path. A/B toggle for
// benchmarking (the two paths are bit-identical — see ffn_moe_prefill_grouped).
var g_moe_grouped_prefill = true

//! Select the MoE prefill path: expert-bucketed grouped GEMMs (default true) vs the per-position
//! reference path. Bit-identical either way.
def set_moe_grouped_prefill(on : bool) {
    g_moe_grouped_prefill = on
}

// Fused expert GEMMs in grouped MoE prefill (native mx4-batch backends only): gate/up/down run as
// three region-list dispatches per layer instead of a per-expert call chain. A/B toggle;
// bit-identical either way — only the scheduling changes.
var g_moe_fused_expert_gemms = true

//! Select fused (region-list, default) vs per-expert expert GEMMs in the grouped MoE prefill on a
//! native mx4-batch backend. Bit-identical either way — A/B benchmarking toggle.
def set_moe_fused_expert_gemms(on : bool) {
    g_moe_fused_expert_gemms = on
}

//! Whether the MoE prefill uses the grouped expert-bucketed path.
def get_moe_grouped_prefill() : bool {
    return g_moe_grouped_prefill
}

var g_forward_prof_acc : table<string; int64>

//! Start a fresh measurement window (zero the per-section accumulators).
var g_forward_prof_disp : table<string; int64>    // parallel_for dispatches per section
var g_forward_prof_chunks : table<string; int64>   // chunks (jobs) pushed per section
var g_forward_prof_inline : table<string; int64>   // inline (non-dispatched) runs per section
var g_prof_mark_disp = 0l
var g_prof_mark_chunks = 0l
var g_prof_mark_inline = 0l

def forward_profile_reset() {
    g_forward_prof_acc |> clear()
    g_forward_prof_disp |> clear()
    g_forward_prof_chunks |> clear()
    g_forward_prof_inline |> clear()
    g_prof_mark_disp = get_disp_count()
    g_prof_mark_chunks = get_disp_chunks()
    g_prof_mark_inline = get_disp_inline()
}

// Sections are sequential and each span ends in exactly one prof_add, so the dispatch-counter
// deltas since the previous prof_add belong to `name` — jobs-per-section with no call-site changes.
def private prof_add(name : string; t0 : int64) {
    let cur = g_forward_prof_acc?[name] ?? 0l
    g_forward_prof_acc[name] = cur + int64(get_time_usec(t0))
    let dc = get_disp_count()
    let ch = get_disp_chunks()
    let il = get_disp_inline()
    g_forward_prof_disp[name] = (g_forward_prof_disp?[name] ?? 0l) + (dc - g_prof_mark_disp)
    g_forward_prof_chunks[name] = (g_forward_prof_chunks?[name] ?? 0l) + (ch - g_prof_mark_chunks)
    g_forward_prof_inline[name] = (g_forward_prof_inline?[name] ?? 0l) + (il - g_prof_mark_inline)
    g_prof_mark_disp = dc
    g_prof_mark_chunks = ch
    g_prof_mark_inline = il
}

//! The accumulated time (us) for one section since the last reset (0 if the section never ran).
def forward_profile_get(name : string) : int64 {
    return g_forward_prof_acc?[name] ?? 0l
}

//! Print the accumulated per-section times (us) since the last reset.
def forward_profile_report() {
    var total = 0l
    for (v in values(g_forward_prof_acc)) {
        total += v
    }
    to_log(LOG_INFO, "=== forward profile (us | % | dispatches | chunks | inline-runs; prefill + decode share the buckets) ===\n")
    for (k, v in keys(g_forward_prof_acc), values(g_forward_prof_acc)) {
        let dd = g_forward_prof_disp?[k] ?? 0l
        let cc = g_forward_prof_chunks?[k] ?? 0l
        let ii = g_forward_prof_inline?[k] ?? 0l
        to_log(LOG_INFO, "  {k}: {v} ({100l * v / max(total, 1l)}%) | disp {dd} | chunks {cc} | inline {ii}\n")
    }
    to_log(LOG_INFO, "  TOTAL: {total}\n")
}

// Prefill attention, threaded over heads (perfect load balance, no shared scratch). Dispatches on
// g_attn_prefill_mode: classic/blocked are bit-identical (same score row + FMA order); flash tiles
// with online softmax and is NOT bit-exact (online reorder).
def private attn_head_flash(qp : float const?; kcp, vcp : auto const?; var xbp : float?;
                            var qpk, kpk, vpk, kqp, vkqp, tmpp : float?; runs : KVRun const?;
                            nrun, npos, start_pos, head_size, qd, kdim, khoff, vdim, vhoff, qoff : int64;
                            scale : float; is_sliding : bool; sliding_window : int64; attn_softcap : float;
                            has_sink : bool; sink : float) {
    let QT = ATTN_FLASH_QT
    let KV = ATTN_FLASH_KV
    unsafe {
        var p0 = 0l
        while (p0 < npos) {
            let tile_rows = min(QT, npos - p0)
            let ap0 = start_pos + p0
            let apL = ap0 + tile_rows - 1l                 // max query position in this tile
            let kstart_min = (is_sliding && ap0 + 1l > sliding_window) ? (ap0 + 1l - sliding_window) : 0l
            // pack the Q tile contiguous [tile_rows × head_size] (q rows are qd-strided in q_b)
            for (qt in range64(tile_rows)) {
                let qsrc = (p0 + qt) * qd + qoff
                for (d in range64(head_size)) {
                    qpk[qt * head_size + d] = qp[qsrc + d]
                }
            }
            var mrun : float[64]   // must equal ATTN_FLASH_QT (fixed-array dim needs an int32 literal)
            var srun : float[64]
            for (qt in range64(tile_rows)) {
                // the sink (gpt-oss) is one extra virtual score with no V contribution — seeding the
                // online softmax with max = sink, sum = e^(sink-sink) = 1 is exactly that column;
                // later tiles rescale it along with everything else
                mrun[int(qt)] = has_sink ? sink : -1.0e30
                srun[int(qt)] = has_sink ? 1.0 : 0.0
                for (d in range64(head_size)) {
                    vkqp[qt * head_size + d] = 0.0
                }
            }
            var kt0 = kstart_min
            while (kt0 <= apL) {
                let kvalid = min(KV, apL + 1l - kt0)
                // pack Kᵀ tile [head_size × KV] and V tile [KV × head_size] by physical run: each
                // cached row dequants ONCE via kv_row_to_f32 (the dequant seam — the GEMMs read f32
                // scratch), V straight into its tile row, K through tmpp then transpose-scattered
                for (r in range64(nrun)) {
                    let rj0 = max(runs[r].j0, kt0)
                    let rj1 = min(runs[r].j1, kt0 + kvalid)
                    let rowbase = runs[r].row0 - runs[r].j0
                    for (aj in range64(rj0, rj1)) {
                        let kt = aj - kt0
                        kv_row_to_f32(vpk + kt * head_size, vcp, (rowbase + aj) * vdim + vhoff, head_size)
                        kv_row_to_f32(tmpp, kcp, (rowbase + aj) * kdim + khoff, head_size)
                        for (d in range64(head_size)) {
                            kpk[d * KV + kt] = tmpp[d]
                        }
                    }
                }
                for (kt in range64(kvalid, KV)) {   // zero-pad the tail columns
                    for (d in range64(head_size)) {
                        kpk[d * KV + kt] = 0.0
                        vpk[kt * head_size + d] = 0.0
                    }
                }
                // scores: kq[tile_rows × KV] = Q · Kᵀ  (gemm_f32 accumulates, so zero first)
                for (i in range64(tile_rows * KV)) {
                    kqp[i] = 0.0
                }
                gemm_f32(kqp, qpk, kpk, tile_rows, head_size, KV)
                // ggml-style decomposition (ops.cpp:8771): scale — then soft-cap — the whole score tile
                // in one vectorized sweep each, replacing the scalar per-element multiply in the mask branch.
                scale_inplace(kqp, scale, tile_rows * KV)
                if (attn_softcap > 0.0) {   // Gemma2: soft-cap scores before softmax
                    softcap4(kqp, tile_rows * KV, attn_softcap)
                }
                // online-softmax fold per query row. The causal + sliding mask is a CONTIGUOUS column
                // span [lo, hi] (cached keys kstart_p..ap), so masking is two boundary fills (no
                // per-element branch) and the row max is hand-vectorized (hmax).
                for (qt in range64(tile_rows)) {
                    let ap = ap0 + qt
                    let kstart_p = (is_sliding && ap + 1l > sliding_window) ? (ap + 1l - sliding_window) : 0l
                    let row = qt * KV
                    let lo = max(0l, kstart_p - kt0)              // first valid tile-local column
                    let hi = min(kvalid - 1l, ap - kt0)           // last valid tile-local column (causal)
                    if (hi < lo) {                                // no valid keys here → contributes nothing
                        for (kt in range64(KV)) { kqp[row + kt] = 0.0 }
                        continue
                    }
                    for (kt in range64(0l, lo))      { kqp[row + kt] = -1.0e30 }   // below the sliding window
                    for (kt in range64(hi + 1l, KV)) { kqp[row + kt] = -1.0e30 }   // above causal / zero-padded
                    let tmax = hmax(kqp + row, KV)
                    let mold = mrun[int(qt)]
                    let mnew = max(mold, tmax)
                    if (mnew > mold) {                 // new running max → rescale this row's accumulator
                        let ms = exp(mold - mnew)
                        scale_inplace(vkqp + qt * head_size, ms, head_size)
                        srun[int(qt)] *= ms
                    }
                    mrun[int(qt)] = mnew
                    // kq row becomes the (un-normalized) prob row — vectorized exp4 over KV (mult of 4).
                    // Masked entries (-1e30) clamp to -80 so exp4's fast path stays in range and yields ~0
                    // (1.8e-35, negligible vs the real probs); flash is already non-bit-exact (online reorder).
                    let mnew4 = float4(mnew, mnew, mnew, mnew)
                    let floor4 = float4(-80.0, -80.0, -80.0, -80.0)
                    var sum4 = float4(0.0, 0.0, 0.0, 0.0)
                    var prow = reinterpret<float4?>(kqp + row)
                    for (kc in range64(KV / 4l)) {
                        let pe4 = exp4(max(prow[kc] - mnew4, floor4))
                        prow[kc] = pe4
                        sum4 += pe4
                    }
                    srun[int(qt)] += sum4.x + sum4.y + sum4.z + sum4.w
                }
                // accumulate output: vkq[tile_rows × head_size] += P[tile_rows × KV] · V[KV × head_size]
                gemm_f32(vkqp, kqp, vpk, tile_rows, KV, head_size)
                kt0 += KV
            }
            // normalize by the running sum and write the qd-strided output slice
            for (qt in range64(tile_rows)) {
                let inv = srun[int(qt)] > 0.0 ? 1.0 / srun[int(qt)] : 0.0
                let dst = (p0 + qt) * qd + qoff
                for (d in range64(head_size)) {
                    xbp[dst + d] = vkqp[qt * head_size + d] * inv
                }
            }
            p0 += QT
        }
    }
}

// One head of BLOCKED prefill attention (see prefill_attention) — codec-generic + run-walking.
// attp_h = this head's ATTN_BLOCK_Q score rows (maxctx-strided). Scores go through kv_score
// (q8_0 K: the same dot decode runs, keeping prefill == decode bit-exact under the block codec).
def private attn_head_blocked(var attp_h : float?; var xbp : float?; qp : float const?;
                              qq : int8 const?; qsp : float const?;
                              kcp, vcp : auto const?; runs : KVRun const?;
                              nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh : int64;
                              scale : float; is_sliding : bool; sliding_window : int64;
                              attn_softcap : float; sinksp : float const?) {
    unsafe {
        var p0 = 0l
        while (p0 < npos) {
            let bq = min(ATTN_BLOCK_Q, npos - p0)
            let ap_first = start_pos + p0
            let ap_last = ap_first + bq - 1l
            let kstart_block = (is_sliding && ap_first + 1l > sliding_window) ? (ap_first + 1l - sliding_window) : 0l
            // PASS 1: scores — each K[kk] loaded once, reused across the block's queries
            for (r in range64(nrun)) {
                let rj0 = max(runs[r].j0, kstart_block)
                let rj1 = min(runs[r].j1, ap_last + 1l)
                let rowbase = runs[r].row0 - runs[r].j0
                for (kk in range64(rj0, rj1)) {
                    let koff = (rowbase + kk) * kdim + khoff
                    let bi_lo = max(0l, kk - ap_first)            // causal: keep queries with ap >= kk
                    var bi_hi = bq
                    if (is_sliding) {                            // window: keep queries with kstart_p <= kk
                        bi_hi = min(bq, kk + sliding_window - ap_first)
                    }
                    for (bi in range64(bi_lo, bi_hi)) {
                        let p = p0 + bi
                        let ap = start_pos + p
                        let kstart_p = (is_sliding && ap + 1l > sliding_window) ? (ap + 1l - sliding_window) : 0l
                        let qbase = p * qd + qoff
                        attp_h[bi * maxctx + (kk - kstart_p)] = kv_score(qp, qq, qsp, kcp, koff, qbase, head_size) * scale
                    }
                }
            }
            // softcap + softmax per query, then zero its output slice
            for (bi in range64(bq)) {
                let p = p0 + bi
                let ap = start_pos + p
                let kstart_p = (is_sliding && ap + 1l > sliding_window) ? (ap + 1l - sliding_window) : 0l
                let cnt = ap + 1l - kstart_p
                let rbase = bi * maxctx
                if (attn_softcap > 0.0) {  // Gemma2: soft-cap scores before softmax
                    softcap4(attp_h + rbase, cnt, attn_softcap)
                }
                if (sinksp != null) {  // gpt-oss: sink joins the denominator
                    softmax_sink(attp_h + rbase, cnt, sinksp[hh])
                } else {
                    softmax(attp_h + rbase, cnt)
                }
                let qbase = p * qd + qoff
                for (i in range64(head_size)) {
                    xbp[qbase + i] = 0.0
                }
            }
            // PASS 2: output += Σ att[j]·V[j] — each V[kk] loaded once, reused across the block
            for (r in range64(nrun)) {
                let rj0 = max(runs[r].j0, kstart_block)
                let rj1 = min(runs[r].j1, ap_last + 1l)
                let rowbase = runs[r].row0 - runs[r].j0
                for (kk in range64(rj0, rj1)) {
                    let voff = (rowbase + kk) * vdim + vhoff
                    let bi_lo = max(0l, kk - ap_first)
                    var bi_hi = bq
                    if (is_sliding) {
                        bi_hi = min(bq, kk + sliding_window - ap_first)
                    }
                    for (bi in range64(bi_lo, bi_hi)) {
                        let p = p0 + bi
                        let ap = start_pos + p
                        let kstart_p = (is_sliding && ap + 1l > sliding_window) ? (ap + 1l - sliding_window) : 0l
                        let qbase = p * qd + qoff
                        kv_axpy(xbp + qbase, vcp + voff, attp_h[bi * maxctx + (kk - kstart_p)], head_size)
                    }
                }
            }
            p0 += ATTN_BLOCK_Q
        }
    }
}

// One head of CLASSIC prefill attention (see prefill_attention) — codec-generic + run-walking.
// attp_h = this head's private score row (maxctx wide, window-relative); each query's window
// [kstart, ap] intersects the layer's physical runs. Scores through kv_score (see attn_head_blocked).
def private attn_head_classic(var attp_h : float?; var xbp : float?; qp : float const?;
                              qq : int8 const?; qsp : float const?;
                              kcp, vcp : auto const?; runs : KVRun const?;
                              nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh : int64;
                              scale : float; is_sliding : bool; sliding_window : int64;
                              attn_softcap : float; sinksp : float const?) {
    unsafe {
        for (p in range64(npos)) {
            let ap = start_pos + p
            let kstart = (is_sliding && ap + 1l > sliding_window) ? (ap + 1l - sliding_window) : 0l
            let cnt = ap + 1l - kstart
            let qbase = p * qd + qoff           // q row == output row (both qd-strided)
            for (r in range64(nrun)) {
                let rj0 = max(runs[r].j0, kstart)
                let rj1 = min(runs[r].j1, ap + 1l)
                let rowbase = runs[r].row0 - runs[r].j0
                for (aj in range64(rj0, rj1)) {
                    attp_h[aj - kstart] = kv_score(qp, qq, qsp, kcp, (rowbase + aj) * kdim + khoff, qbase, head_size) * scale
                }
            }
            if (attn_softcap > 0.0) {  // Gemma2: soft-cap scores before softmax
                softcap4(attp_h, cnt, attn_softcap)
            }
            if (sinksp != null) {  // gpt-oss: sink joins the denominator
                softmax_sink(attp_h, cnt, sinksp[hh])
            } else {
                softmax(attp_h, cnt)
            }
            for (i in range64(head_size)) {
                xbp[qbase + i] = 0.0
            }
            for (r in range64(nrun)) {   // output += Σ att[j]·V[j]
                let rj0 = max(runs[r].j0, kstart)
                let rj1 = min(runs[r].j1, ap + 1l)
                let rowbase = runs[r].row0 - runs[r].j0
                for (aj in range64(rj0, rj1)) {
                    kv_axpy(xbp + qbase, vcp + (rowbase + aj) * vdim + vhoff, attp_h[aj - kstart], head_size)
                }
            }
        }
    }
}

// Prefill codec dispatch (same contract as attn_head_decode_d): one wrapper per prefill worker,
// the 3x3 K×V matrix spelled once. Prefill is compute-bound, so dequant-in-dot is fine — no
// query quantize needed.
def private attn_head_flash_d(qp : float const?; kcl, vcl : uint8 const?; kdt, vdt : KVDtype; var xbp : float?;
                              var qpk, kpk, vpk, kqp, vkqp, tmpp : float?; runs : KVRun const?;
                              nrun, npos, start_pos, head_size, qd, kv_dim, kvhoff, qoff : int64;
                              scale : float; is_sliding : bool; sliding_window : int64; attn_softcap : float;
                              has_sink : bool; sink : float) {
    let kdim = kv_side_off(kdt, kv_dim)
    let khoff = kv_side_off(kdt, kvhoff)
    let vdim = kv_side_off(vdt, kv_dim)
    let vhoff = kv_side_off(vdt, kvhoff)
    unsafe {
        if (kdt == KVDtype.q8_0) {
            if (vdt == KVDtype.q8_0) {
                attn_head_flash(qp, kcl, vcl, xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } elif (vdt == KVDtype.tq4) {
                attn_head_flash(qp, kcl, reinterpret<TQ4B const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } elif (vdt == KVDtype.f16) {
                attn_head_flash(qp, kcl, reinterpret<uint16 const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } else {
                attn_head_flash(qp, kcl, reinterpret<float const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            }
        } elif (kdt == KVDtype.tq4) {
            if (vdt == KVDtype.q8_0) {
                attn_head_flash(qp, reinterpret<TQ4B const?>(kcl), vcl, xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } elif (vdt == KVDtype.tq4) {
                attn_head_flash(qp, reinterpret<TQ4B const?>(kcl), reinterpret<TQ4B const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } elif (vdt == KVDtype.f16) {
                attn_head_flash(qp, reinterpret<TQ4B const?>(kcl), reinterpret<uint16 const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } else {
                attn_head_flash(qp, reinterpret<TQ4B const?>(kcl), reinterpret<float const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            }
        } elif (kdt == KVDtype.f16) {
            if (vdt == KVDtype.q8_0) {
                attn_head_flash(qp, reinterpret<uint16 const?>(kcl), vcl, xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } elif (vdt == KVDtype.tq4) {
                attn_head_flash(qp, reinterpret<uint16 const?>(kcl), reinterpret<TQ4B const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } elif (vdt == KVDtype.f16) {
                attn_head_flash(qp, reinterpret<uint16 const?>(kcl), reinterpret<uint16 const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            } else {
                attn_head_flash(qp, reinterpret<uint16 const?>(kcl), reinterpret<float const?>(vcl), xbp,
                    qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                    kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
            }
        } elif (vdt == KVDtype.q8_0) {
            attn_head_flash(qp, reinterpret<float const?>(kcl), vcl, xbp,
                qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
        } elif (vdt == KVDtype.tq4) {
            attn_head_flash(qp, reinterpret<float const?>(kcl), reinterpret<TQ4B const?>(vcl), xbp,
                qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
        } elif (vdt == KVDtype.f16) {
            attn_head_flash(qp, reinterpret<float const?>(kcl), reinterpret<uint16 const?>(vcl), xbp,
                qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
        } else {
            attn_head_flash(qp, reinterpret<float const?>(kcl), reinterpret<float const?>(vcl), xbp,
                qpk, kpk, vpk, kqp, vkqp, tmpp, runs, nrun, npos, start_pos, head_size, qd,
                kdim, khoff, vdim, vhoff, qoff, scale, is_sliding, sliding_window, attn_softcap, has_sink, sink)
        }
    }
}

def private attn_head_blocked_d(var attp_h : float?; var xbp : float?; qp : float const?;
                                qq : int8 const?; qsp : float const?;
                                kcl, vcl : uint8 const?; kdt, vdt : KVDtype; runs : KVRun const?;
                                nrun, npos, start_pos, maxctx, qd, head_size, kv_dim, kvhoff, qoff, hh : int64;
                                scale : float; is_sliding : bool; sliding_window : int64;
                                attn_softcap : float; sinksp : float const?) {
    let kdim = kv_side_off(kdt, kv_dim)
    let khoff = kv_side_off(kdt, kvhoff)
    let vdim = kv_side_off(vdt, kv_dim)
    let vhoff = kv_side_off(vdt, kvhoff)
    unsafe {
        if (kdt == KVDtype.q8_0) {
            if (vdt == KVDtype.q8_0) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, kcl, vcl,
                    runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, kcl, reinterpret<TQ4B const?>(vcl),
                    runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, kcl, reinterpret<uint16 const?>(vcl),
                    runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } else {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, kcl, reinterpret<float const?>(vcl),
                    runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            }
        } elif (kdt == KVDtype.tq4) {
            if (vdt == KVDtype.q8_0) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), vcl,
                    runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<TQ4B const?>(vcl),
                    runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<uint16 const?>(vcl),
                    runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } else {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<float const?>(vcl),
                    runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            }
        } elif (kdt == KVDtype.f16) {
            if (vdt == KVDtype.q8_0) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), vcl,
                    runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<TQ4B const?>(vcl),
                    runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<uint16 const?>(vcl),
                    runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } else {
                attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<float const?>(vcl),
                    runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            }
        } elif (vdt == KVDtype.q8_0) {
            attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), vcl,
                runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        } elif (vdt == KVDtype.tq4) {
            attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<TQ4B const?>(vcl),
                runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        } elif (vdt == KVDtype.f16) {
            attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<uint16 const?>(vcl),
                runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        } else {
            attn_head_blocked(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<float const?>(vcl),
                runs, nrun, npos, start_pos, maxctx, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        }
    }
}

def private attn_head_classic_d(var attp_h : float?; var xbp : float?; qp : float const?;
                                qq : int8 const?; qsp : float const?;
                                kcl, vcl : uint8 const?; kdt, vdt : KVDtype; runs : KVRun const?;
                                nrun, npos, start_pos, qd, head_size, kv_dim, kvhoff, qoff, hh : int64;
                                scale : float; is_sliding : bool; sliding_window : int64;
                                attn_softcap : float; sinksp : float const?) {
    let kdim = kv_side_off(kdt, kv_dim)
    let khoff = kv_side_off(kdt, kvhoff)
    let vdim = kv_side_off(vdt, kv_dim)
    let vhoff = kv_side_off(vdt, kvhoff)
    unsafe {
        if (kdt == KVDtype.q8_0) {
            if (vdt == KVDtype.q8_0) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, kcl, vcl,
                    runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, kcl, reinterpret<TQ4B const?>(vcl),
                    runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, kcl, reinterpret<uint16 const?>(vcl),
                    runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } else {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, kcl, reinterpret<float const?>(vcl),
                    runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            }
        } elif (kdt == KVDtype.tq4) {
            if (vdt == KVDtype.q8_0) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), vcl,
                    runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<TQ4B const?>(vcl),
                    runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<uint16 const?>(vcl),
                    runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } else {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<TQ4B const?>(kcl), reinterpret<float const?>(vcl),
                    runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            }
        } elif (kdt == KVDtype.f16) {
            if (vdt == KVDtype.q8_0) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), vcl,
                    runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.tq4) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<TQ4B const?>(vcl),
                    runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } elif (vdt == KVDtype.f16) {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<uint16 const?>(vcl),
                    runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            } else {
                attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<uint16 const?>(kcl), reinterpret<float const?>(vcl),
                    runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                    scale, is_sliding, sliding_window, attn_softcap, sinksp)
            }
        } elif (vdt == KVDtype.q8_0) {
            attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), vcl,
                runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        } elif (vdt == KVDtype.tq4) {
            attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<TQ4B const?>(vcl),
                runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        } elif (vdt == KVDtype.f16) {
            attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<uint16 const?>(vcl),
                runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        } else {
            attn_head_classic(attp_h, xbp, qp, qq, qsp, reinterpret<float const?>(kcl), reinterpret<float const?>(vcl),
                runs, nrun, npos, start_pos, qd, head_size, kdim, khoff, vdim, vhoff, qoff, hh,
                scale, is_sliding, sliding_window, attn_softcap, sinksp)
        }
    }
}

def private prefill_attention(var s : Session; kcl, vcl : uint8 const?; npos, start_pos, head_size, qd, kv_dim,
                              n_heads, kv_mul : int64; scale : float;
                              is_sliding : bool; sliding_window : int64; attn_softcap : float;
                              sinksp : float const?) {   // this layer's per-head sink logits; null = no sinks
    let maxctx = start_pos + npos          // per-head att row width (= max cnt over this call)
    let attn_par = npos * maxctx * n_heads >= g_attn_par_threshold
    let kdt = s.kv_dtype_k
    let vdt = s.kv_dtype_v
    // ONE run list over the layer's widest window; workers intersect their sub-windows with each run
    let kstart_min = (is_sliding && start_pos + 1l > sliding_window) ? (start_pos + 1l - sliding_window) : 0l
    let nrun = kv_runs(s, kstart_min, start_pos + npos, s.kv_runs)
    // block-codec K: quantize every query row once so scores run the SAME int8 dot as decode; flash skips this (f32 tiles)
    if ((kdt == KVDtype.q8_0 || kdt == KVDtype.tq4) && g_attn_prefill_mode != AttnPrefillMode.flash) {
        if (int64(length(s.attq)) < npos * qd) {
            s.attq |> resize(int(npos * qd))
            s.atts |> resize(int(npos * qd / 32l))
        }
        quantize_q8_0_into(s.q_b, npos * qd, s.attq, s.atts, 0l, 0l)
    }
    unsafe {
        let qp = addr < float const? >(s.q_b[0])
        let kq8bc = (kdt == KVDtype.q8_0 || kdt == KVDtype.tq4) && g_attn_prefill_mode != AttnPrefillMode.flash
        let qqp = kq8bc ? addr < int8 const? >(s.attq[0]) : null
        let qsp = kq8bc ? addr < float const? >(s.atts[0]) : null
        let runsp = addr < KVRun const? >(s.kv_runs[0])
        var xbp = addr(s.xb_b[0])
        // att_b is sized only for blocked/classic (flash packs into fa_* instead) — null under flash, never dereferenced
        var attp = !empty(s.att_b) ? addr(s.att_b[0]) : null
        if (g_attn_prefill_mode == AttnPrefillMode.flash) {
            // per-head packing scratch bases (sized in forward_prefill for this mode)
            var qpk0 = addr(s.fa_q[0]);  var kpk0 = addr(s.fa_k[0]);  var vpk0 = addr(s.fa_v[0])
            var kqp0 = addr(s.fa_kq[0]); var vkqp0 = addr(s.fa_vkq[0]); var tmp0 = addr(s.fa_tmp[0])
            maybe_parallel_for(attn_par, 0, int(n_heads), get_dispatch_lanes()) $(rb, re) {
                unsafe {
                    for (h in range(rb, re)) {
                        let hh = int64(h)
                        let qoff = hh * head_size
                        let kvhoff = (hh / kv_mul) * head_size
                        attn_head_flash_d(qp, kcl, vcl, kdt, vdt, xbp,
                            qpk0 + hh * ATTN_FLASH_QT * head_size,
                            kpk0 + hh * head_size * ATTN_FLASH_KV,
                            vpk0 + hh * ATTN_FLASH_KV * head_size,
                            kqp0 + hh * ATTN_FLASH_QT * ATTN_FLASH_KV,
                            vkqp0 + hh * ATTN_FLASH_QT * head_size,
                            tmp0 + hh * head_size, runsp,
                            nrun, npos, start_pos, head_size, qd, kv_dim, kvhoff, qoff,
                            scale, is_sliding, sliding_window, attn_softcap,
                            sinksp != null, sinksp != null ? sinksp[hh] : 0.0)
                    }
                }
            }
        } elif (g_attn_prefill_mode == AttnPrefillMode.blocked) {
            maybe_parallel_for(attn_par, 0, int(n_heads), get_dispatch_lanes()) $(rb, re) {
                unsafe {
                    for (h in range(rb, re)) {
                        let hh = int64(h)
                        let qoff = hh * head_size
                        let kvhoff = (hh / kv_mul) * head_size
                        attn_head_blocked_d(attp + hh * ATTN_BLOCK_Q * maxctx, xbp, qp, qqp, qsp, kcl, vcl, kdt, vdt, runsp,
                            nrun, npos, start_pos, maxctx, qd, head_size, kv_dim, kvhoff, qoff, hh,
                            scale, is_sliding, sliding_window, attn_softcap, sinksp)
                    }
                }
            }
        } else {
            maybe_parallel_for(attn_par, 0, int(n_heads), get_dispatch_lanes()) $(rb, re) {
                unsafe {
                    for (h in range(rb, re)) {
                        let hh = int64(h)
                        let qoff = hh * head_size
                        let kvhoff = (hh / kv_mul) * head_size
                        attn_head_classic_d(attp + hh * maxctx, xbp, qp, qqp, qsp, kcl, vcl, kdt, vdt, runsp,
                            nrun, npos, start_pos, qd, head_size, kv_dim, kvhoff, qoff, hh,
                            scale, is_sliding, sliding_window, attn_softcap, sinksp)
                    }
                }
            }
        }
    }
}

// ===== std/dense block kernels (prefill, batched) =====
// Batched counterparts to attention_std_decode / ffn_dense_decode; bit-for-bit identical to
// calling forward() per position in sequence. Q4 has no batched kernel, falls back to per-token.

// attn-norm → batched QKV(+bias) → qk/v-norm → RoPE, for npos rows of x_b into q_b/k_b/v_b — the
// position-free head of the std attention block, shared by prefill and (later) batched decode.
def private attn_qkv_rope_prefix(t : Model; var s : Session; l : int64; npos : int64; start_pos : int64) {
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let use_rope_tab = g_use_rope_table
    // sliding vs global attention per the arch's layer pattern (see attention_std_decode)
    let is_sliding = layer_is_sliding(c, l)
    // gemma-4 E-series shared-KV layer: project Q only, attend against the source layer's cache (loff aliases it)
    let kv_shared = t.kv_src[l] != l
    // attention rmsnorm, per position
    let ts_norm0 = ref_time_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_att_off + l * dim, dim, npos)
    prof_add("norm", ts_norm0)
    // batched q/k/v projections (each re-quantizes xb_b — cheap vs the matmul)
    let ts_qkv = ref_time_ticks()
    mm_b_f(s, s.q_b, t, fmt_at(t.wq_fmt, l), t.wq_offs[l], s.xb_b, dim, qd, npos)
    if (kv_shared) {
        // E-series shared-KV layer: no K/V of its own — the source layer already filled the cache
    } else {
        mm_b_f(s, s.k_b, t, fmt_at(t.wk_fmt, l), t.wk_offs[l], s.xb_b, dim, kv_dim, npos)
        if (t.wv_offs[l] >= 0l) {
            mm_b_f(s, s.v_b, t, fmt_at(t.wv_fmt, l), t.wv_offs[l], s.xb_b, dim, kv_dim, npos)
        } elif (c.v_norm) {
            // no attn_v tensor (gemma4 global layers): V = the pre-norm K projection, fused into the weightless V-norm
            rms_batch(s.v_b, s.k_b, t, t.rms_ones_off, head_size, npos * n_kv_heads)
        } else {
            copy_floats(s.k_b, 0l, s.v_b, 0l, npos * kv_dim)
        }
    }
    if (c.attn_qkv_bias) {  // Qwen2: + bias per position before RoPE
        for (p in range64(npos)) {
            add_bias(s.q_b, p * qd, t.bq, l * qd, qd)
            add_bias(s.k_b, p * kv_dim, t.bk, l * kv_dim, kv_dim)
            add_bias(s.v_b, p * kv_dim, t.bv, l * kv_dim, kv_dim)
        }
    }
    prof_add("mm_qkv", ts_qkv)
    if (c.qk_norm) {  // Qwen3/gemma4: per-head RMSNorm on q/k before RoPE — q_b/k_b are npos*n_heads/n_kv_heads contiguous head_size rows
        let ts_qknorm = ref_time_ticks()
        rms_batch(s.q_b, s.q_b, t, t.rms_q_offs[l], head_size, npos * n_heads)
        if (!kv_shared) {
            rms_batch(s.k_b, s.k_b, t, t.rms_k_offs[l], head_size, npos * n_kv_heads)
        }
        prof_add("norm", ts_qknorm)
    }
    // gemma4: weightless per-head RMSNorm on V (all-ones weight); V-from-K layers already normed above
    if (c.v_norm && t.wv_offs[l] >= 0l) {
        let ts_vnorm = ref_time_ticks()
        rms_batch(s.v_b, s.v_b, t, t.rms_ones_off, head_size, npos * n_kv_heads)
        prof_add("norm", ts_vnorm)
    }
    // RoPE per position in place; p-RoPE arches (gemma4) apply rope_freqs factors to GLOBAL layers only
    let ts_rope = ref_time_ticks()
    let swa_rope = is_sliding && has_dual_rope(c)
    // shared-KV layers rope Q only (kv_dim = 0 makes the K rope a no-op; there's no K to store)
    let rope_kv = kv_shared ? 0l : kv_dim
    if (use_rope_tab) {
        if (swa_rope) {
            rope_batch_tab(s.q_b, s.k_b, s.rope_cos_swa, s.rope_sin_swa, c.rope_neox, npos, head_size, qd, rope_kv, c.rope_dim)
        } else {
            rope_batch_tab(s.q_b, s.k_b, s.rope_cos, s.rope_sin, c.rope_neox, npos, head_size, qd, rope_kv, c.rope_dim)
        }
    } else {
        let theta = swa_rope ? c.rope_freq_base_swa : c.rope_freq_base
        let fscale = swa_rope ? c.rope_freq_scale_swa : c.rope_freq_scale
        let use_ff = !(c.rope_ff_global_only && is_sliding)
        rope_batch(s.q_b, s.k_b, t, theta, fscale, start_pos, npos, head_size, qd, rope_kv, use_ff, c.rope_dim)
    }
    prof_add("rope", ts_rope)
}

// out-proj(+bias, +post-norm) → residual, for npos rows of xb_b back into x_b — the position-free
// tail of the std attention block, shared by prefill and (later) batched decode.
def private attn_out_suffix(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let dim = c.dim
    let qd = c.n_heads * layer_head_size(c, l)
    // output projection (batched, qd -> dim) + optional bias + post-attn norm + residual
    let ts_wo = ref_time_ticks()
    mm_b_f(s, s.xb2_b, t, fmt_at(t.wo_fmt, l), t.wo_offs[l], s.xb_b, qd, dim, npos)
    if (c.attn_out_bias) {  // gpt-oss: + bias per position on the projected attention output
        for (p in range64(npos)) {
            add_bias(s.xb2_b, p * dim, t.bo, l * dim, dim)
        }
    }
    prof_add("mm_wo", ts_wo)
    let ts_resid0 = ref_time_ticks()
    if (c.pre_post_norm) {  // Gemma2: post-attention RMSNorm per position before residual
        rms_batch(s.xb2_b, s.xb2_b, t, t.rms_post_att_off + l * dim, dim, npos)
    }
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
    prof_add("act_resid", ts_resid0)
}

// The GPU arm shared by the std and gated prefill blocks: the whole attention block (GEMMs,
// qk-norm, rope, flash-style attention, wo) as one device chain per window; norm, activation
// image, KV-store, and residual stay CPU. Numerics: GPU f32 tile accumulation — drift-class vs CPU flash.
def private attention_gpu_prefill(t : Model; var s : Session; l : int64; npos : int64; gated : bool) {
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let rot = c.rope_dim > 0l ? c.rope_dim : head_size
    let fq = fmt_at(t.wq_fmt, l)
    let kq_cls = fq != KqFmt.q8
    let ts_norm0 = ref_time_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_att_off + l * dim, dim, npos)
    if (kq_cls) {   // the chain's activation image follows the q/k/v stack class
        s.kxqb |> resize(int(npos * dim))
        s.kxsb |> resize(int(npos * dim / 256l))
        s.kxbsb |> resize(int(npos * dim / 16l))
        requant_rows_q8k_bs(s.xb_b, dim, npos, s.kxqb, s.kxsb, s.kxbsb)
    } else {
        requant_rows_q8(s.xb_b, dim, npos, s.xqb, s.xsb)
    }
    if (!g_use_rope_table) {   // the GPU rope consumes table rows — build them (bit-identical values)
        build_rope_table(s.rope_cos, s.rope_sin, t, c.rope_freq_base, c.rope_freq_scale, 0l, npos, rot, true)
    }
    prof_add("at_norm", ts_norm0)
    let ts_gpu = ref_time_ticks()
    unsafe {
        if (kq_cls) {
            matmul_moe_gpu_attn(s.xb2_b, s.k_b, s.v_b,
                t.wq_offs[l], t.wk_offs[l], t.wv_offs[l], t.wo_offs[l],
                int(fq), int(fmt_at(t.wk_fmt, l)), int(fmt_at(t.wv_fmt, l)), int(fmt_at(t.wo_fmt, l)),
                s.kxqb, s.kxsb,
                c.qk_norm ? addr<float const?>(t.fblob[t.rms_q_offs[l]]) : null,
                c.qk_norm ? addr<float const?>(t.fblob[t.rms_k_offs[l]]) : null,
                addr<float const?>(s.rope_cos[0]), addr<float const?>(s.rope_sin[0]),
                npos, dim, qd, kv_dim, head_size, rot, n_heads, kv_mul,
                scale, c.norm_eps, gated, c.qk_norm)
        } else {
            matmul_moe_gpu_attn(s.xb2_b, s.k_b, s.v_b,
                t.wq_offs[l], t.wk_offs[l], t.wv_offs[l], t.wo_offs[l],
                int(fq), int(fmt_at(t.wk_fmt, l)), int(fmt_at(t.wv_fmt, l)), int(fmt_at(t.wo_fmt, l)),
                s.xqb, s.xsb,
                c.qk_norm ? addr<float const?>(t.fblob[t.rms_q_offs[l]]) : null,
                c.qk_norm ? addr<float const?>(t.fblob[t.rms_k_offs[l]]) : null,
                addr<float const?>(s.rope_cos[0]), addr<float const?>(s.rope_sin[0]),
                npos, dim, qd, kv_dim, head_size, rot, n_heads, kv_mul,
                scale, c.norm_eps, gated, c.qk_norm)
        }
    }
    prof_add("at_gpu", ts_gpu)
    // the cache store runs the normal CPU codec off the DMA'd k/v rows
    let ts_kv = ref_time_ticks()
    var kcl = kv_layer_kbase(s, l, c.seq_len)
    var vcl = kv_layer_vbase(s, l, c.seq_len)
    let store_nrun = kv_runs(s, 0l, npos, s.kv_runs)
    unsafe {
        kv_store_batch(kcl, vcl, s.kv_dtype_k, s.kv_dtype_v, s.k_b, s.v_b,
            addr < KVRun const? >(s.kv_runs[0]), store_nrun, 0l, npos, kv_dim)
    }
    prof_add("kv_store", ts_kv)
    let ts_resid = ref_time_ticks()
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
    prof_add("at_resid", ts_resid)
}

// npos >= 32 (a batch worth dispatching), start of sequence only (the chain reads the batch's
// own K/V device-side), tq4 excluded (the basis change lives in the CPU path), npos within the
// chain's context capacity
def private attn_gpu_prefill_ready(s : Session; l : int64; npos : int64; start_pos : int64) : bool {
    return (npos >= 32l && npos <= 4096l && start_pos == 0l && moe_gpu_attn_on_gpu(l)
        && s.kv_dtype_k != KVDtype.tq4 && s.kv_dtype_v != KVDtype.tq4)
}

// attn-norm → batched QKV(+bias) → RoPE → KV-store → prefill attention → out-proj(+post-norm) → residual.
def private attention_std_prefill(t : Model; var s : Session; l : int64; npos : int64; start_pos : int64) {
    if (attn_gpu_prefill_ready(s, l, npos, start_pos)) {
        attention_gpu_prefill(t, s, l, npos, false)   // buckets at_norm / at_gpu / kv_store / at_resid
        return
    }
    let c = t.config
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    var kcl = kv_layer_kbase(s, l, c.seq_len)   // layer-base byte pointers (aliases the source layer when shared)
    var vcl = kv_layer_vbase(s, l, c.seq_len)
    let is_sliding = layer_is_sliding(c, l)
    let kv_shared = t.kv_src[l] != l
    attn_qkv_rope_prefix(t, s, l, npos, start_pos)
    let ktq4 = s.kv_dtype_k == KVDtype.tq4
    let vtq4 = s.kv_dtype_v == KVDtype.tq4
    if (ktq4 || vtq4) {
        // tq4 basis change at the RoPE seam (see attention_std_decode); xb_b un-rotates below
        let ts_rot = ref_time_ticks()
        unsafe {
            let sgp = addr < float const? >(s.kv_signs[0])
            if (ktq4) {
                tq4_rotate_batch(s.q_b, npos, qd, head_size, sgp)
                if (!kv_shared) {
                    tq4_rotate_batch(s.k_b, npos, kv_dim, head_size, sgp)
                }
            }
            if (vtq4 && !kv_shared) {
                tq4_rotate_batch(s.v_b, npos, kv_dim, head_size, sgp)
            }
        }
        prof_add("tq4_rot", ts_rot)
    }
    // separate pass (provably identical to interleaving); shared-KV layers store nothing — already cached
    if (!kv_shared) {
        let ts_kv = ref_time_ticks()
        let store_nrun = kv_runs(s, start_pos, start_pos + npos, s.kv_runs)   // where the new rows land
        unsafe {
            kv_store_batch(kcl, vcl, s.kv_dtype_k, s.kv_dtype_v, s.k_b, s.v_b,
                addr < KVRun const? >(s.kv_runs[0]), store_nrun, start_pos, npos, kv_dim)
        }
        prof_add("kv_store", ts_kv)
    }
    // causal attention into xb_b; output for position p lands at xb_b[p*qd..], feeding wo below
    let ts_attn = ref_time_ticks()
    let sinksp = (c.attn_sinks
        ? unsafe(addr < float const? >(t.fblob[t.sinks_off + l * n_heads]))
        : null)
    prefill_attention(s, kcl, vcl, npos, start_pos, head_size, qd, kv_dim, n_heads, kv_mul, scale,
                      is_sliding, c.sliding_window, c.attn_logit_softcap, sinksp)
    prof_add("attn", ts_attn)
    if (vtq4) {   // tq4 V: xb_b rows accumulated in the rotated basis — un-rotate per (pos, head)
        unsafe {
            tq4_unrotate_batch(s.xb_b, npos, qd, head_size, addr < float const? >(s.kv_signs[0]))
        }
    }
    attn_out_suffix(t, s, l, npos)
}

// Batched causal conv over the whole prefill window + per-row epilogue (SiLU, q/k L2-norm, q
// pre-scale): row p depends only on input rows p-taps..p, so rows parallelize freely. Weights
// transpose tap-major once per layer for vector fmas instead of a per-channel scalar tap walk.
def private dn_conv_batch(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let cd = dn_conv_dim(c)
    let kd = dn_key_dim(c)
    let nkh = c.ssm_n_group
    let ds = c.ssm_d_state
    let dconv = c.ssm_d_conv
    let taps = dconv - 1l
    let qscale = 1.0 / sqrt(float(ds))
    unsafe {
        let wsrc = addr < float const? >(t.fblob[t.dn_conv_off + l * dconv * cd])
        var wtp = addr(s.dn_wtap[0])
        for (ch in range64(cd)) {
            for (k in range64(dconv)) {
                wtp[k * cd + ch] = wsrc[ch * dconv + k]
            }
        }
    }
    var inp0 : float const?
    var wtap0 : float const?
    var hist0 : float const?
    var outp0 : float?
    unsafe {
        inp0 = addr < float const? >(s.dn_qkv_b[0])
        wtap0 = addr < float const? >(s.dn_wtap[0])
        hist0 = addr < float const? >(s.dn_conv_state[s.dn_conv_prefix[l]])
        outp0 = addr(s.dn_conv_b[0])
    }
    maybe_parallel_for(npos * cd >= g_act_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
        unsafe {
            for (pp in range(rb, re)) {
                let p = int64(pp)
                var orow = outp0 + p * cd
                if (p >= taps) {   // all taps in-batch: dconv row fmas over float4 channel blocks
                    let irow = inp0 + (p - taps) * cd
                    var ch = 0l
                    while (ch < cd) {
                        var acc = float4(0.0, 0.0, 0.0, 0.0)
                        for (k in range64(dconv)) {
                            acc += *addr < float4 const? >(irow[k * cd + ch]) * *addr < float4 const? >(wtap0[k * cd + ch])
                        }
                        *addr < float4? >(orow[ch]) = acc
                        ch += 4l
                    }
                } else {           // window straddles the batch start: missing taps from history
                    for (ch in range64(cd)) {
                        var acc = 0.0
                        for (k in range64(dconv)) {
                            let sp = p - taps + k
                            acc += (sp >= 0l ? inp0[sp * cd + ch] : hist0[ch * taps + p + k]) * wtap0[k * cd + ch]
                        }
                        orow[ch] = acc
                    }
                }
                silu(orow, cd)
                l2_norm_rows(orow, nkh, ds, c.norm_eps)
                l2_norm_rows(orow + kd, nkh, ds, c.norm_eps)
                scale_inplace(orow, qscale, kd)
            }
        }
    }
    unsafe {   // advance the conv history to the last taps input rows (channel-major)
        var st = addr(s.dn_conv_state[s.dn_conv_prefix[l]])
        for (k in range64(taps)) {
            let sp = npos - taps + k
            if (sp >= 0l) {
                for (ch in range64(cd)) {
                    st[ch * taps + k] = inp0[sp * cd + ch]
                }
            } else {   // npos < taps: shift surviving old taps left (ascending k reads ahead of writes)
                for (ch in range64(cd)) {
                    st[ch * taps + k] = st[ch * taps + k + npos]
                }
            }
        }
    }
}

// One v-head's chunked delta rule over the whole prefill window (mirrors llama.cpp
// build_delta_net_chunking; NOT bit-identical to the recurrent form — chunk FP reassociation).
// Solves (I + A) d = V_beta - (K_beta ⊙ e^{gcum}) S per chunk; state S advances once per chunk.
def private dn_chunked_head(conv : float const?; qoff, koff, voff, cd : int64;
                            bp, gp : float const?; bg_stride : int64;
                            zp : float const?; var op : float?; o_stride : int64;
                            var stp : float?; var ws : float?; wnorm : float const?;
                            aa, dtb, eps : float; vec : bool; npos, ds : int64) {
    let CS = DN_CHUNK
    unsafe {
        var qc = ws                         // CS x ds: q rows, scaled to q ⊙ e^{gcum} after the KQ GEMM
        var kcT = ws + CS * ds              // ds x cl: raw k, transposed (the B side of A/KQ GEMMs)
        var kb = ws + 2l * CS * ds          // CS x ds: k ⊙ beta, scaled to -(k_beta ⊙ e^{gcum}) after A
        var vb = ws + 3l * CS * ds          // CS x ds: v ⊙ beta
        var w = ws + 4l * CS * ds           // CS x ds: -T @ (k_beta ⊙ e^{gcum})
        var vnew = ws + 5l * CS * ds        // CS x ds: the in-chunk delta vectors d
        var och = ws + 6l * CS * ds         // CS x ds: this chunk's outputs
        var kgT = ws + 7l * CS * ds         // ds x cl: (k ⊙ e^{g_last - gcum})^T for the state update
        var tm = ws + 8l * CS * ds          // cl x cl: A, solved in place into T (unit diag)
        var kq = ws + 8l * CS * ds + CS * CS   // cl x cl: Q K^T ⊙ decay (diagonal kept)
        var gcum = ws + 8l * CS * ds + 2l * CS * CS   // CS: in-chunk cumsum of log-decay
        var egc = gcum + CS                 // CS: e^{gcum}
        var kgc = gcum + 2l * CS            // CS: e^{g_last - gcum}
        var c0 = 0l
        while (c0 < npos) {
            let cl = min(CS, npos - c0)
            // gather + per-token transforms (beta/g raw -> beta = sigmoid, g = a·softplus(raw+dt))
            var gacc = 0.0
            for (i in range64(cl)) {
                let crow = conv + (c0 + i) * cd
                let bv = 1.0 / (1.0 + exp(-bp[(c0 + i) * bg_stride]))
                gacc += aa * softplus(gp[(c0 + i) * bg_stride] + dtb)
                gcum[i] = gacc
                egc[i] = exp(gacc)
                for (j in range64(ds)) {
                    let kv = crow[koff + j]
                    qc[i * ds + j] = crow[qoff + j]
                    kcT[j * cl + i] = kv
                    kb[i * ds + j] = kv * bv
                    vb[i * ds + j] = crow[voff + j] * bv
                }
            }
            // A = (k_beta k^T) ⊙ decay, strictly lower, negated for the in-place solve; KQ keeps its diagonal
            for (i in range64(cl * cl)) {
                tm[i] = 0.0
                kq[i] = 0.0
            }
            gemm_f32(tm, kb, kcT, cl, ds, cl)
            gemm_f32(kq, qc, kcT, cl, ds, cl)
            for (i in range64(cl)) {
                let gi = gcum[i]
                var j = 0l
                if (vec) {   // exp4 over the strictly-lower row (args <= 0; clamp = flash-fold convention)
                    let gi4 = float4(gi, gi, gi, gi)
                    while (j + 4l <= i) {
                        let d = exp4(max(gi4 - float4(gcum[j], gcum[j + 1l], gcum[j + 2l], gcum[j + 3l]), EXP4_LO))
                        tm[i * cl + j] = -tm[i * cl + j] * d.x
                        tm[i * cl + j + 1l] = -tm[i * cl + j + 1l] * d.y
                        tm[i * cl + j + 2l] = -tm[i * cl + j + 2l] * d.z
                        tm[i * cl + j + 3l] = -tm[i * cl + j + 3l] * d.w
                        kq[i * cl + j] *= d.x
                        kq[i * cl + j + 1l] *= d.y
                        kq[i * cl + j + 2l] *= d.z
                        kq[i * cl + j + 3l] *= d.w
                        j += 4l
                    }
                }
                while (j < i) {
                    let d = exp(gi - gcum[j])
                    tm[i * cl + j] = -tm[i * cl + j] * d
                    kq[i * cl + j] *= d
                    j++
                }
                tm[i * cl + i] = 0.0
                for (ju in range64(i + 1l, cl)) {   // upper: tm zeroed, kq zeroed (diagonal kept)
                    tm[i * cl + ju] = 0.0
                    kq[i * cl + ju] = 0.0
                }
            }
            // forward substitution in place: T[i][j] = -A[i][j] - sum_t A[i][t] T[t][j]
            for (i in range64(cl)) {
                for (tt in range64(i)) {
                    let coef = tm[i * cl + tt]
                    for (j in range64(tt)) {
                        tm[i * cl + j] += coef * tm[tt * cl + j]
                    }
                }
                tm[i * cl + i] = 1.0
            }
            // d = T v_beta - (T (k_beta ⊙ e^{gcum})) S — kb scaled in place, minus folded into w
            for (i in range64(cl)) {
                scale_inplace(kb + i * ds, -egc[i], ds)
            }
            for (i in range64(cl * ds)) {
                w[i] = 0.0
                vnew[i] = 0.0
            }
            gemm_f32(w, tm, kb, cl, cl, ds)
            gemm_f32(vnew, tm, vb, cl, cl, ds)
            gemm_f32(vnew, w, stp, cl, ds, ds)
            // o = (q ⊙ e^{gcum}) S + KQ d (qc scaled in place after its raw-q GEMM above)
            for (i in range64(cl)) {
                scale_inplace(qc + i * ds, egc[i], ds)
            }
            for (i in range64(cl * ds)) {
                och[i] = 0.0
            }
            gemm_f32(och, qc, stp, cl, ds, ds)
            gemm_f32(och, kq, vnew, cl, cl, ds)
            // gated out-norm into batched output rows; z-gate exp4 (largest exp population of the whole prefill)
            for (i in range64(cl)) {
                rmsnorm(och + i * ds, reinterpret<float const?>(och + i * ds), wnorm, ds, eps)
                var orow = op + (c0 + i) * o_stride
                let zrow = zp + (c0 + i) * o_stride
                if (vec) {
                    silu_mul4(orow, och + i * ds, zrow, ds)
                } else {
                    for (j in range64(ds)) {
                        let zv = zrow[j]
                        orow[j] = och[i * ds + j] * (zv / (1.0 + exp(-zv)))
                    }
                }
            }
            // state advance, once per chunk: S = e^{g_last} S + (k ⊙ e^{g_last - gcum})^T d
            let glast = gcum[cl - 1l]
            scale_inplace(stp, exp(glast), ds * ds)
            for (i in range64(cl)) {
                kgc[i] = exp(glast - gcum[i])
            }
            for (j in range64(ds)) {
                var kr = kgT + j * cl
                let ks = kcT + j * cl
                for (i in range64(cl)) {
                    kr[i] = ks[i] * kgc[i]
                }
            }
            gemm_f32(stp, kgT, vnew, ds, cl, ds)
            c0 += cl
        }
    }
}

// The chunked deltanet prefill core: batched conv rows, then the per-v-head chunk pass threaded
// over heads (each head's state chain is independent; per-head slabs in dn_cws).
def private deltanet_prefill_chunked(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let cd = dn_conv_dim(c)
    let kd = dn_key_dim(c)
    let di = c.ssm_d_inner
    let nvh = c.ssm_dt_rank
    let nkh = c.ssm_n_group
    let ds = c.ssm_d_state
    let ts_conv = ref_time_ticks()
    dn_conv_batch(t, s, l, npos)
    prof_add("dn_conv", ts_conv)
    var conv0 : float const?
    var bp0 : float const?
    var gp0 : float const?
    var zp0 : float const?
    var op0 : float?
    var st0 : float?
    var ws0 : float?
    var wnorm0 : float const?
    var ab0 : float const?
    var dtb0 : float const?
    unsafe {
        conv0 = addr < float const? >(s.dn_conv_b[0])
        bp0 = addr < float const? >(s.dn_beta_b[0])
        gp0 = addr < float const? >(s.dn_g_b[0])
        zp0 = addr < float const? >(s.dn_z_b[0])
        op0 = addr(s.dn_o_b[0])
        st0 = addr(s.dn_state[s.dn_state_prefix[l]])
        ws0 = addr(s.dn_cws[0])
        wnorm0 = addr < float const? >(t.fblob[t.dn_norm_off + l * ds])
        ab0 = addr < float const? >(t.fblob[t.dn_a_off + l * nvh])
        dtb0 = addr < float const? >(t.fblob[t.dn_dt_off + l * nvh])
    }
    let wssz = dn_cws_head_size(c)
    let vec = g_act_vec   // read on the caller, captured — fork workers must not read globals
    let ts_scan = ref_time_ticks()
    maybe_parallel_for(0, int(nvh), min(int(nvh), get_dispatch_lanes())) $(rb, re) {
        unsafe {
            for (hh in range(rb, re)) {
                let h = int64(hh)
                let kh = h % nkh   // k-heads repeat cyclically onto v-heads (ggml_repeat: h % nkh)
                dn_chunked_head(conv0, kh * ds, kd + kh * ds, 2l * kd + h * ds, cd,
                    bp0 + h, gp0 + h, nvh, zp0 + h * ds, op0 + h * ds, di,
                    st0 + h * ds * ds, ws0 + h * wssz, wnorm0,
                    ab0[h], dtb0[h], c.norm_eps, vec, npos, ds)
            }
        }
    }
    prof_add("dn_scan", ts_scan)
}

// The GPU arm: the whole block (qkv/z GEMMs, conv, chunked scan, gated out-norm, out-proj) as one
// device chain per window; norm, raw beta/alpha GEMMs, residual, conv-history store stay CPU.
// Numerics drift-class vs CPU (GPU exp/f32 accumulation) — the DN parity bar (qwen35 knife-edge).
def private deltanet_prefill_gpu(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let dim = c.dim
    let cd = dn_conv_dim(c)
    let kd = dn_key_dim(c)
    let di = c.ssm_d_inner
    let nvh = c.ssm_dt_rank
    let nkh = c.ssm_n_group
    let ds = c.ssm_d_state
    let dconv = c.ssm_d_conv
    let taps = dconv - 1l
    let ts_norm0 = ref_time_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_att_off + l * dim, dim, npos)
    prof_add("dn_norm", ts_norm0)
    let ts_in = ref_time_ticks()
    if (t.dn_ba_f32) {   // raw beta/alpha rows — the transforms run in-kernel
        mm_fblob_b(s.dn_beta_b, t, t.dn_betaf_off + l * dim * nvh, s.xb_b, dim, nvh, npos)
        mm_fblob_b(s.dn_g_b, t, t.dn_alphaf_off + l * dim * nvh, s.xb_b, dim, nvh, npos)
    } else {
        mm_b_f(s, s.dn_beta_b, t, KqFmt.q8, t.dnbeta_offs[l], s.xb_b, dim, nvh, npos)
        mm_b_f(s, s.dn_g_b, t, KqFmt.q8, t.dnalpha_offs[l], s.xb_b, dim, nvh, npos)
    }
    requant_rows_q8(s.xb_b, dim, npos, s.xqb, s.xsb)   // one Q8_0 image serves the qkv AND z GEMMs
    prof_add("dn_mm_in", ts_in)
    let ts_gpu = ref_time_ticks()
    s.dn_tail |> resize(int(taps * cd))
    unsafe {
        matmul_moe_gpu_dn(s.xb2_b, t.dnqkv_offs[l], t.dngate_offs[l], t.dnout_offs[l],
            s.xqb, s.xsb, s.dn_beta_b, s.dn_g_b,
            addr<float const?>(t.fblob[t.dn_conv_off + l * dconv * cd]),
            addr<float const?>(t.fblob[t.dn_norm_off + l * ds]),
            addr<float const?>(t.fblob[t.dn_a_off + l * nvh]),
            addr<float const?>(t.fblob[t.dn_dt_off + l * nvh]),
            addr(s.dn_state[s.dn_state_prefix[l]]), s.dn_tail,
            addr<float const?>(s.dn_conv_state[s.dn_conv_prefix[l]]),
            npos, dim, cd, kd, di, nvh, nkh, ds, dconv, c.norm_eps)
    }
    prof_add("dn_gpu", ts_gpu)
    let ts_resid = ref_time_ticks()
    unsafe {   // conv-history advance from the tail rows (channel-major — the decode layout)
        var st = addr(s.dn_conv_state[s.dn_conv_prefix[l]])
        let tp = addr < float const? >(s.dn_tail[0])
        for (k in range64(taps)) {
            for (ch in range64(cd)) {
                st[ch * taps + k] = tp[k * cd + ch]
            }
        }
    }
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
    prof_add("dn_resid", ts_resid)
}

// qwen35 deltanet prefill: batched projections, then the inherently sequential per-token core
// (state carried across positions), then one batched ssm_out projection + residual.
def private deltanet_prefill(t : Model; var s : Session; l : int64; npos : int64) {
    trace_tag(TRACE_TAG_DN)   // override the loop's attn_std stamp: dn layers color separately
    if (g_dn_prefill_mode == DnPrefillMode.chunked && npos >= 32l && moe_gpu_dn_on_gpu(l)) {
        deltanet_prefill_gpu(t, s, l, npos)   // buckets dn_norm / dn_mm_in / dn_gpu / dn_resid
        return
    }
    let c = t.config
    let dim = c.dim
    let cd = dn_conv_dim(c)
    let di = c.ssm_d_inner
    let nvh = c.ssm_dt_rank
    let ts_norm0 = ref_time_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_att_off + l * dim, dim, npos)
    prof_add("dn_norm", ts_norm0)
    let ts_qkv = ref_time_ticks()
    mm_b_f(s, s.dn_qkv_b, t, KqFmt.q8, t.dnqkv_offs[l], s.xb_b, dim, cd, npos)
    mm_b_f(s, s.dn_z_b, t, KqFmt.q8, t.dngate_offs[l], s.xb_b, dim, di, npos)
    if (t.dn_ba_f32) {   // F32-on-disk beta/alpha matmul fp32, like llama.cpp
        mm_fblob_b(s.dn_beta_b, t, t.dn_betaf_off + l * dim * nvh, s.xb_b, dim, nvh, npos)
        mm_fblob_b(s.dn_g_b, t, t.dn_alphaf_off + l * dim * nvh, s.xb_b, dim, nvh, npos)
    } else {
        mm_b_f(s, s.dn_beta_b, t, KqFmt.q8, t.dnbeta_offs[l], s.xb_b, dim, nvh, npos)
        mm_b_f(s, s.dn_g_b, t, KqFmt.q8, t.dnalpha_offs[l], s.xb_b, dim, nvh, npos)
    }
    prof_add("dn_mm_in", ts_qkv)
    if (g_dn_prefill_mode == DnPrefillMode.chunked && npos > 1l) {   // single rows: recurrent is cheaper
        deltanet_prefill_chunked(t, s, l, npos)   // buckets dn_conv + dn_scan inside
    } else {
        let ts_core = ref_time_ticks()
        unsafe {
            let qkvp = addr < float const? >(s.dn_qkv_b[0])
            var zp = addr(s.dn_z_b[0])
            var bp = addr(s.dn_beta_b[0])
            var gp = addr(s.dn_g_b[0])
            var op = addr(s.dn_o_b[0])
            for (p in range64(npos)) {
                dn_token_core(t, s, l, qkvp + p * cd, zp + p * di, bp + p * nvh, gp + p * nvh, op + p * di)
            }
        }
        prof_add("dn_core", ts_core)
    }
    let ts_wo = ref_time_ticks()
    mm_b_f(s, s.xb2_b, t, KqFmt.q8, t.dnout_offs[l], s.dn_o_b, di, dim, npos)
    prof_add("dn_mm_out", ts_wo)
    let ts_resid = ref_time_ticks()
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
    prof_add("dn_resid", ts_resid)
}

// The batched gated-attention twin of attention_std_prefill: 2x-wide [q|gate], partial rope,
// sigmoid out-gate before the shared out-proj suffix.
def private attention_qwen35_gated_prefill(t : Model; var s : Session; l : int64; npos : int64; start_pos : int64) {
    if (attn_gpu_prefill_ready(s, l, npos, start_pos)) {
        attention_gpu_prefill(t, s, l, npos, true)   // buckets at_norm / at_gpu / kv_store / at_resid
        return
    }
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let rot = c.rope_dim > 0l ? c.rope_dim : head_size
    var kcl = kv_layer_kbase(s, l, c.seq_len)
    var vcl = kv_layer_vbase(s, l, c.seq_len)
    let ts_norm0 = ref_time_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_att_off + l * dim, dim, npos)
    prof_add("norm", ts_norm0)
    let ts_qkv = ref_time_ticks()
    mm_b_f(s, s.dn_qg_b, t, fmt_at(t.wq_fmt, l), t.wq_offs[l], s.xb_b, dim, 2l * qd, npos)
    mm_b_f(s, s.k_b, t, fmt_at(t.wk_fmt, l), t.wk_offs[l], s.xb_b, dim, kv_dim, npos)
    mm_b_f(s, s.v_b, t, fmt_at(t.wv_fmt, l), t.wv_offs[l], s.xb_b, dim, kv_dim, npos)
    // deinterleave q/gate rows (q at 2h·hs, gate at (2h+1)·hs), threaded: serial copy was a ~3ms all-lane hole per layer
    unsafe {
        let qgp = addr < float const? >(s.dn_qg_b[0])
        var qp = addr(s.q_b[0])
        var gp = addr(s.dn_qgate_b[0])
        maybe_parallel_for(npos * qd >= g_act_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (pp in range(rb, re)) {
                    let p = int64(pp)
                    for (h in range64(n_heads)) {
                        let sb = p * 2l * qd + h * 2l * head_size
                        let db = p * qd + h * head_size
                        copy_floats(qp + db, qgp + sb, head_size)
                        copy_floats(gp + db, qgp + sb + head_size, head_size)
                    }
                }
            }
        }
    }
    prof_add("mm_qkv", ts_qkv)
    let ts_qknorm = ref_time_ticks()
    rms_batch(s.q_b, s.q_b, t, t.rms_q_offs[l], head_size, npos * n_heads)
    rms_batch(s.k_b, s.k_b, t, t.rms_k_offs[l], head_size, npos * n_kv_heads)
    prof_add("norm", ts_qknorm)
    let ts_rope = ref_time_ticks()
    unsafe {
        let half = rot / 2l
        var qbp = addr(s.q_b[0])
        var kbp = addr(s.k_b[0])
        if (g_use_rope_table) {
            let cp = addr < float const? >(s.rope_cos[0])
            let sp2 = addr < float const? >(s.rope_sin[0])
            for (p in range64(npos)) {
                rope_scaled_neox_tab_part(qbp + p * qd, head_size, rot, qd, cp + p * half, sp2 + p * half)
                rope_scaled_neox_tab_part(kbp + p * kv_dim, head_size, rot, kv_dim, cp + p * half, sp2 + p * half)
            }
        } else {
            for (p in range64(npos)) {
                rope_scaled_neox_part(qbp + p * qd, start_pos + p, head_size, rot, qd, c.rope_freq_base, c.rope_freq_scale, false, null, c.rope_mscale)
                rope_scaled_neox_part(kbp + p * kv_dim, start_pos + p, head_size, rot, kv_dim, c.rope_freq_base, c.rope_freq_scale, false, null, c.rope_mscale)
            }
        }
    }
    prof_add("rope", ts_rope)
    let ktq4 = s.kv_dtype_k == KVDtype.tq4
    let vtq4 = s.kv_dtype_v == KVDtype.tq4
    if (ktq4 || vtq4) {   // tq4 basis change (attention_std_prefill's twin); xb_b un-rotates below
        let ts_rot = ref_time_ticks()
        unsafe {
            let sgp = addr < float const? >(s.kv_signs[0])
            if (ktq4) {
                tq4_rotate_batch(s.q_b, npos, qd, head_size, sgp)
                tq4_rotate_batch(s.k_b, npos, kv_dim, head_size, sgp)
            }
            if (vtq4) {
                tq4_rotate_batch(s.v_b, npos, kv_dim, head_size, sgp)
            }
        }
        prof_add("tq4_rot", ts_rot)
    }
    let ts_kv = ref_time_ticks()
    let store_nrun = kv_runs(s, start_pos, start_pos + npos, s.kv_runs)
    unsafe {
        kv_store_batch(kcl, vcl, s.kv_dtype_k, s.kv_dtype_v, s.k_b, s.v_b,
            addr < KVRun const? >(s.kv_runs[0]), store_nrun, start_pos, npos, kv_dim)
    }
    prof_add("kv_store", ts_kv)
    let ts_attn = ref_time_ticks()
    prefill_attention(s, kcl, vcl, npos, start_pos, head_size, qd, kv_dim, n_heads, kv_mul, scale,
                      false, 0l, c.attn_logit_softcap, null)
    prof_add("attn", ts_attn)
    if (vtq4) {   // un-rotate every position's xb before the out-gate (gate is in the original basis)
        unsafe {
            tq4_unrotate_batch(s.xb_b, npos, qd, head_size, addr < float const? >(s.kv_signs[0]))
        }
    }
    // threaded + exp4: serial scalar loop was npos·qd (2.1M @pp512) libm exps, ~10ms all-lane hole/layer
    let ts_gate = ref_time_ticks()
    unsafe {
        var xbp = addr(s.xb_b[0])
        let gbp = addr < float const? >(s.dn_qgate_b[0])
        let vec = g_act_vec
        maybe_parallel_for(npos * qd >= g_act_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                let off = int64(rb) * qd
                let cnt = int64(re - rb) * qd
                if (vec) {
                    sigmoid_gate4(xbp + off, gbp + off, cnt)
                } else {
                    sigmoid_gate(xbp + off, gbp + off, cnt)
                }
            }
        }
    }
    prof_add("act", ts_gate)
    attn_out_suffix(t, s, l, npos)
}

// The qwen35 prefill attention block: forward-only state guard, then deltanet or gated attention.
def private attention_qwen35_prefill(t : Model; var s : Session; l : int64; npos : int64; start_pos : int64) {
    if (l == 0l) {   // recurrent state is forward-only: full reset at 0, no rewind
        if (start_pos == 0l) {
            dn_reset(s)
        } elif (start_pos != s.dn_pos) {
            panic("dasLLAMA: deltanet state is forward-only — cannot prefill from position {start_pos} (session is at {s.dn_pos}); rewinding needs a fresh session")
        }
        s.dn_pos = start_pos + npos
    }
    if (layer_is_recurrent(t.config, l)) {
        deltanet_prefill(t, s, l, npos)
    } else {
        attention_qwen35_gated_prefill(t, s, l, npos, start_pos)
    }
}

// ffn-norm → batched gated FFN → down-proj(+post-norm) → residual.
def private ffn_dense_prefill(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let dim = c.dim
    let hidden = layer_hidden(t, l)   // gemma-4 E2B varies FFN width per layer
    // ffn rmsnorm, per position
    let ts_norm1 = ref_time_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_ffn_off + l * dim, dim, npos)
    prof_add("norm", ts_norm1)
    // gated FFN (batched): hb = act(W1·xb) * (W3·xb) — act/mul elementwise, so batch-flat
    let ts_ffn = ref_time_ticks()
    mm_b_f(s, s.hb_b, t, fmt_at(t.w1_fmt, l), t.w1_offs[l], s.xb_b, dim, hidden, npos)
    mm_b_f(s, s.hb2_b, t, fmt_at(t.w3_fmt, l), t.w3_offs[l], s.xb_b, dim, hidden, npos)
    prof_add("mm_ffn", ts_ffn)
    let ts_act = ref_time_ticks()
    gate_batch(s.hb_b, s.hb2_b, c.ffn_act, hidden, npos)   // fused act*gate (exp4 unless act_vec off)
    prof_add("act", ts_act)
    // down projection (batched) + optional post-FFN norm + residual
    let ts_ffn2 = ref_time_ticks()
    mm_b_f(s, s.xb_b, t, fmt_at(t.w2_fmt, l), t.w2_offs[l], s.hb_b, hidden, dim, npos)
    prof_add("mm_ffn", ts_ffn2)
    let ts_resid1 = ref_time_ticks()
    if (c.pre_post_norm) {  // Gemma2: post-FFN RMSNorm per position before residual
        rms_batch(s.xb_b, s.xb_b, t, t.rms_post_ffn_off + l * dim, dim, npos)
    }
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb_b[0])), npos * dim)
    // E-series applies out_scale AFTER its PLE block (in ffn_gemma4_prefill) — skip it here when PLE is on
    if (c.layer_out_scale && !has_ple(c)) {  // gemma4: scale the whole layer output (after the residual)
        let os = t.fblob[t.out_scale_off + l]
        scale_inplace(unsafe(addr(s.x_b[0])), os, npos * dim)
    }
    prof_add("act_resid", ts_resid1)
}

// Raw byte move as int64 lanes — the Q8 gather's copy. Integer loads/stores are bit-exact by
// construction with no FP semantics (a float-lane copy of quant bytes could in principle quiet
// sNaN bit patterns on some targets). n8 = byte count / 8.
[hint(unsafe_range_check, noalias = d, noalias = s)]
def private copy_int64s(var d : int64?; s : int64 const?; n8 : int64) {
    unsafe {
        for (j in range64(n8)) {
            d[j] = s[j]
        }
    }
}

// Gather one expert bucket's Q8-quantized activation rows (quants + scales) out of the whole-batch
// image: gather row r comes from position moe_bkt[b0+r]/k. The int8 quant rows move as int64 lanes
// (dim % 32 == 0 so dim % 8 == 0); the scales are genuine floats and ride the tuned copy_floats.
def private moe_gather_rows(var s : Session; b0, cnt, dim, k : int64; with_bs : bool = false; kform : bool = false) {
    let nb = kform ? dim / 256l : dim / 32l   // kq v2: Q8_K rows carry per-256 scales
    let d8 = dim / 8l
    unsafe {
        var xqp : int64 const?
        var xsp : float const?
        if (kform) {
            xqp = addr < int64 const? >(s.kxqb[0])
            xsp = addr < float const? >(s.kxsb[0])
        } else {
            xqp = addr < int64 const? >(s.xqb[0])
            xsp = addr < float const? >(s.xsb[0])
        }
        var gqp = addr < int64? >(s.moe_gxq[0])
        var gsp = addr(s.moe_gxs[0])
        for (r in range64(cnt)) {
            let p = s.moe_bkt[b0 + r] / k
            copy_int64s(gqp + r * d8, xqp + p * d8, d8)
            copy_floats(gsp + r * nb, xsp + p * nb, nb)
        }
        if (with_bs) {   // kq-expert layers: the per-16 sum rows ride along (dim/16 ints = dim/32 int64s)
            let nb8 = dim / 32l
            var bp : int64 const?
            if (kform) {
                bp = addr < int64 const? >(s.kxbsb[0])
            } else {
                bp = addr < int64 const? >(s.xbsb[0])
            }
            var gbp = addr < int64? >(s.moe_gxbs[0])
            for (r in range64(cnt)) {
                let p = s.moe_bkt[b0 + r] / k
                copy_int64s(gbp + r * nb8, bp + p * nb8, nb8)
            }
        }
    }
}

// Expand one native-MXFP4 expert region [d x n] into EXACT Q8 blocks: q[j] = doubled-e2m1 LUT
// value, scale = e8m0_half(e) — no requantization error. Interleaved-mr backends write the
// repacked form directly (byte-for-byte map); non-interleaved backends expand row-major then repack.
def private expand_mx4_region_q8(t : Model; woff, n, d : int64; var wq : array<int8>; var ws : array<float>) {
    let total = n * d
    wq |> resize(total)
    ws |> resize(total / 32l)
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    let nblk = total / 32l
    unsafe {
        let lutp = addr(lut[0])   // frame outlives the join, so workers read a live pointer
        let np = addr(t.mxq[woff / 2l])
        let ep = addr(t.mxs[woff / 32l])
        var qp = addr(wq[0])
        var sp = addr(ws[0])
        // scale bytes: 1:1 positional map in both the row-major and the interleaved layout
        maybe_parallel_for(total >= g_requant_par_threshold, 0, int(nblk), get_dispatch_lanes()) $(sb, se) {
            unsafe {
                for (bi in range(sb, se)) {
                    sp[bi] = e8m0_half(uint(ep[bi]))
                }
            }
        }
        if (g_active_mx4_expand_interleaved) {
            // interleaved: mr-row block-groups first, then row-major tail; bias128 stamps need a BIASED LUT copy
            let mr = active_q8_layout_mr()
            let wbias = int(active_q8_wbias())
            let kgv = active_q8_kgroup()
            var blut : int8[16]
            for (i in range(16)) {
                blut[i] = int8(int(lut[i]) + wbias)
            }
            let nbyt = 16l * mr    // nibble bytes per block-group
            let qbyt = 32l * mr    // q8 bytes per block-group
            let ngrp = (d / mr) * (n / 32l)
            let blutp = addr(blut[0])   // frame outlives the join, same as lutp
            if (kgv == 8l) {
                maybe_parallel_for(total >= g_requant_par_threshold, 0, int(ngrp), get_dispatch_lanes()) $(gb, ge) {
                    unsafe {
                        for (bg0 in range(gb, ge)) {
                            let bg = int64(bg0)
                            let nb = bg * nbyt
                            let qb = bg * qbyt
                            for (off in range64(nbyt)) {
                                let j = off / (4l * mr)
                                let rem = off % (4l * mr)
                                let q8o = (j / 2l) * 8l * mr + (rem / 4l) * 8l + (j % 2l) * 4l + rem % 4l
                                let b = int(np[nb + off])
                                qp[qb + q8o] = blutp[b & 15]
                                qp[qb + nbyt + q8o] = blutp[b >> 4]
                            }
                        }
                    }
                }
            } else {
                maybe_parallel_for(total >= g_requant_par_threshold, 0, int(ngrp), get_dispatch_lanes()) $(gb, ge) {
                    unsafe {
                        for (bg0 in range(gb, ge)) {
                            let bg = int64(bg0)
                            let nb = bg * nbyt
                            let qb = bg * qbyt
                            for (off in range64(nbyt)) {
                                let b = int(np[nb + off])
                                qp[qb + off] = blutp[b & 15]
                                qp[qb + nbyt + off] = blutp[b >> 4]
                            }
                        }
                    }
                }
            }
            for (bi in range64(ngrp * mr, nblk)) {   // row tail (d % mr): row-major from here on
                let nb16 = bi * 16l
                let qb = bi * 32l
                for (j in range64(16l)) {
                    let b = int(np[nb16 + j])
                    qp[qb + j] = lutp[b & 15]
                    qp[qb + j + 16l] = lutp[b >> 4]
                }
            }
        } else {
            maybe_parallel_for(total >= g_requant_par_threshold, 0, int(nblk), get_dispatch_lanes()) $(bb, be) {
                unsafe {
                    for (bi0 in range(bb, be)) {
                        let bi = int64(bi0)
                        let nb16 = bi * 16l
                        let qb = bi * 32l
                        for (j in range64(16l)) {
                            let b = int(np[nb16 + j])
                            qp[qb + j] = lutp[b & 15]
                            qp[qb + j + 16l] = lutp[b >> 4]
                        }
                    }
                }
            }
        }
    }
    if (g_active_needs_repack && !g_active_mx4_expand_interleaved) {
        // row-major-expanded scratch on a repack backend: interleave through the backend's own Q8 repack
        repack_q8q8_weight(wq, ws, 0l, n, d)
    }
}

// MoE FFN prefill, grouped: route every position, bucket by expert, run ONE batched GEMM chain per
// touched expert instead of npos*k single-token matmuls. Bit-exact vs the per-position reference
// path (native-MXFP4 experts are the one exception — differ by kernel summation order only).

var private g_gpu_combine = true
var private g_gpu_combine_probed = false

def private gpu_combine_on : bool {
    if (!g_gpu_combine_probed) {
        g_gpu_combine_probed = true
        g_gpu_combine = get_env_variable("DASLLAMA_GPU_COMBINE") != "0"
    }
    return g_gpu_combine
}

//! Runtime routing gate over the device-side routed combine (overrides the
//! ``DASLLAMA_GPU_COMBINE`` env probe) — the in-process A/B lever: both arms share one
//! process, one residency, one paging weather, so the measured delta is the combine alone.
def set_gpu_combine_route(on : bool) {
    g_gpu_combine_probed = true
    g_gpu_combine = on
}

def private ffn_moe_prefill_grouped(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let dim = c.dim
    let nb = dim / 32l
    let ne = c.n_expert
    let k = c.n_expert_used
    let nfe = c.n_ff_exp
    let nsh = c.n_ff_shexp
    let nfx = max(nfe, nsh)
    let nk = npos * k
    // fused (native mx4-batch): gather/hidden/output images span every routed row at once, bucket-ordered
    let kq_layer = moe_layer_kq(t, l)
    let f1 = fmt_at(t.we1_fmt, l)
    let f2 = fmt_at(t.we2_fmt, l)
    let f3 = fmt_at(t.we3_fmt, l)
    // fused single-dispatch GEMMs: native mx4-batch backends, or kq layers whose stacks are ALL K-quant
    let fused_kq = (kq_layer && f1 != KqFmt.q8 && f2 != KqFmt.q8 && f3 != KqFmt.q8
        && t.kq_repacked && kernel_backend_has_kq_batch_groupn() && g_moe_fused_expert_gemms)
    // GPU MoE tier: offloaded layers run the whole expert FFN chain device-side; tiny batches stay CPU (GPU submit cost > the GEMM)
    let fused_gpu = (!c.moe_exps_bias && !t.experts_mx4 && c.ffn_act != FfnAct.swiglu_oai
        && (moe_gpu_layer_on_gpu(l) || moe_gpu_layer_streamed(l)) && g_moe_fused_expert_gemms && npos >= 32l)
    // device-side combine rides fused_gpu; DASLLAMA_GPU_COMBINE=0 falls back to park + CPU reduce
    let gpu_comb = fused_gpu && gpu_combine_on()
    let fused = (t.experts_mx4 && g_active_mx4_native_batch && g_moe_fused_expert_gemms) || fused_kq || fused_gpu
    let grows = fused ? nk : npos
    s.moe_rlog |> resize(npos * ne)
    s.moe_idx_b |> resize(nk)
    s.moe_w_b |> resize(nk)
    s.moe_cnt |> resize(ne)
    s.moe_bkt |> resize(nk)
    s.moe_gxq |> resize(grows * dim)
    s.moe_gxs |> resize(grows * nb)
    s.moe_ghb |> resize(grows * nfx)
    s.moe_ghb2 |> resize(grows * nfx)
    s.moe_hq |> resize(grows * nfx)
    s.moe_hs |> resize(grows * nfx / 32l)
    s.moe_gout |> resize(grows * dim)
    s.moe_eout |> resize(nk * dim)
    if (nsh > 0l && c.moe_shexp_gated) {
        s.moe_gl_b |> resize(npos)
    }
    if (kq_layer) {
        s.moe_gxbs |> resize(grows * dim / 16l)
        s.moe_hbs |> resize(grows * nfx / 16l)
    }

    // 1. route every position: one batched router GEMM, then the shared select per row; shexp gate dots ride along
    trace_tag(TRACE_TAG_ROUTE)
    let ts_route = ref_time_ticks()
    mm_fblob_b(s.moe_rlog, t, t.router_off + l * ne * dim, s.xb_b, dim, ne, npos)
    // per-row bias+select+gate dot, threaded: serial loop was ~2-3ms every-lane-idle/layer here
    unsafe {
        var rlp = addr(s.moe_rlog[0])
        var idxp = addr(s.moe_idx_b[0])
        var wp = addr(s.moe_w_b[0])
        var glp = nsh > 0l && c.moe_shexp_gated ? addr(s.moe_gl_b[0]) : null
        let xbp = addr<float const?>(s.xb_b[0])
        let rbp = c.moe_router_bias ? addr<float const?>(t.fblob[t.router_b_off + l * ne]) : null
        let ebp = c.moe_exp_probs ? addr<float const?>(t.fblob[t.exp_probs_off + l * ne]) : null
        let gvp = nsh > 0l && c.moe_shexp_gated ? addr<float const?>(t.fblob[t.shexp_gate_off + l * dim]) : null
        let gate = c.moe_gate
        let norm_topk = c.norm_topk_prob
        let wscale = c.expert_weights_scale
        maybe_parallel_for(npos > 1l, 0, int(npos), get_dispatch_lanes()) $(pb, pe) {
            unsafe {
                for (pp in range(pb, pe)) {
                    let p = int64(pp)
                    var lgp = rlp + p * ne
                    if (rbp != null) {
                        for (i in range64(ne)) {
                            lgp[i] += rbp[i]
                        }
                    }
                    moe_select_core(gate, ne, k, norm_topk, wscale, lgp, idxp, wp, p * k, ebp)
                    if (glp != null) {
                        glp[p] = 1.0 / (1.0 + exp(-dot(gvp, xbp + p * dim, dim)))
                    }
                }
            }
        }
    }
    // bucket (position, slot) pairs by expert — CSR in-place cursor: moe_cnt[e] ends as e's run END
    for (e in range64(ne)) {
        s.moe_cnt[e] = 0l
    }
    for (r in range64(nk)) {
        s.moe_cnt[s.moe_idx_b[r]]++
    }
    var run = 0l
    for (e in range64(ne)) {
        let cn = s.moe_cnt[e]
        s.moe_cnt[e] = run
        run += cn
    }
    for (r in range64(nk)) {
        let e = s.moe_idx_b[r]
        s.moe_bkt[s.moe_cnt[e]] = r
        s.moe_cnt[e]++
    }
    prof_add("moe_route", ts_route)

    // 2. quantize every normed activation row ONCE; kq v2 gate/up gathers off Q8_K, q8 stacks off Q8_0
    let moe_kform = kq_layer && moe_pair_kform(f1, f3)
    if (moe_kform) {
        s.kxqb |> resize(npos * dim)
        s.kxsb |> resize(npos * dim / 256l)
        s.kxbsb |> resize(npos * dim / 16l)
        requant_rows_q8k_bs(s.xb_b, dim, npos, s.kxqb, s.kxsb, s.kxbsb)
        if (nsh > 0l && !c.moe_dense_shexp) {
            // the q8 shexp batch (step 4) reads the Q8_0 image — without this it silently corrupts
            requant_rows_q8(s.xb_b, dim, npos, s.xqb, s.xsb)
        }
    } else {
        requant_rows_q8(s.xb_b, dim, npos, s.xqb, s.xsb)
    }

    // 3F. fused: gate/up/down each run as ONE region-list dispatch (3 fork/joins/layer, not 3/expert)
    if (fused) {
        trace_tag(TRACE_TAG_MOE)
        let ts_gather = ref_time_ticks()
        // the GPU tile computes activation block sums in-kernel — skip the per-16 sum gather
        moe_gather_rows(s, 0l, nk, dim, k, moe_kform && !fused_gpu, moe_kform)
        prof_add("moe_gather", ts_gather)
        // region triples (woff, row0, cnt) per touched expert; the three planes share row0/cnt
        s.moe_offs1 |> clear(); s.moe_offs1 |> reserve(ne * 3l)
        s.moe_offs3 |> clear(); s.moe_offs3 |> reserve(ne * 3l)
        s.moe_offs2 |> clear(); s.moe_offs2 |> reserve(ne * 3l)
        if (c.moe_exps_bias) {
            s.moe_boffs1 |> clear(); s.moe_boffs1 |> reserve(ne)
            s.moe_boffs3 |> clear(); s.moe_boffs3 |> reserve(ne)
            s.moe_boffs2 |> clear(); s.moe_boffs2 |> reserve(ne)
        }
        // LPT order (biggest buckets first) front-loads expensive spans; expert-order left 32% of GEMM windows idle
        s.moe_ord |> clear()
        s.moe_ord |> reserve(int(ne))   // capacity persists across layers — no per-layer alloc
        for (e in range64(ne)) {
            let b0 = e == 0l ? 0l : s.moe_cnt[e - 1l]
            let cnt = s.moe_cnt[e] - b0
            if (cnt > 0l) {
                s.moe_ord |> push(int2(int(e), int(cnt)))
            }
        }
        lpt_order(s.moe_ord)
        var nreg = 0l
        for (oe in s.moe_ord) {
            let e = int64(oe.x)
            let cnt = int64(oe.y)
            let b0 = e == 0l ? 0l : s.moe_cnt[e - 1l]
            let eg = l * ne + e
            let base = e * dim * nfe
            s.moe_offs1 |> push(t.we1_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
            s.moe_offs1 |> push(b0)
            s.moe_offs1 |> push(cnt)
            s.moe_offs3 |> push(t.we3_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
            s.moe_offs3 |> push(b0)
            s.moe_offs3 |> push(cnt)
            s.moe_offs2 |> push(t.we2_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
            s.moe_offs2 |> push(b0)
            s.moe_offs2 |> push(cnt)
            if (c.moe_exps_bias) {
                s.moe_boffs1 |> push(t.web1_off + eg * nfe)
                s.moe_boffs3 |> push(t.web3_off + eg * nfe)
                s.moe_boffs2 |> push(t.web2_off + eg * dim)
            }
            nreg++
        }
        let ts_g1 = ref_time_ticks()
        if (gpu_comb) {   // GPU tier: gate+up/act/requant/down AND the routed weighted-sum combine
            // gout comes back as npos combined rows; the 33MB-class bucket-row readback + park scatter disappear
            s.moe_inv |> resize(nk)
            for (j in range64(nk)) {
                s.moe_inv[s.moe_bkt[j]] = uint(j)
            }
            matmul_moe_gpu_ffn_combined(s.moe_eout, s.moe_offs1, s.moe_offs3, s.moe_offs2, nreg,
                s.moe_gxq, s.moe_gxs, s.moe_w_b, s.moe_inv, dim, nfe, nk, npos, int(f1), int(f3), int(f2), c.ffn_act == FfnAct.gelu)
            prof_add("moe_gpu", ts_g1)
        } elif (fused_gpu) {   // combine off: per-row gout + the park/CPU-reduce path below
            matmul_moe_gpu_ffn(s.moe_gout, s.moe_offs1, s.moe_offs3, s.moe_offs2, nreg,
                s.moe_gxq, s.moe_gxs, dim, nfe, nk, int(f1), int(f3), int(f2), c.ffn_act == FfnAct.gelu)
            prof_add("moe_gpu", ts_g1)
        } else {
            if (fused_kq) {   // native kq: one batched region-list GEMM per projection (never biased)
                mm_b_kq_groupn(s.moe_ghb, t, f1, s.moe_offs1, nreg, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe)
                mm_b_kq_groupn(s.moe_ghb2, t, f3, s.moe_offs3, nreg, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe)
            } elif (c.moe_exps_bias) {   // gpt-oss: gate/up biases fold into the stores, before the act
                matmul_mx4q8_batch_groupn(s.moe_ghb, t.mxq, t.mxs, s.moe_offs1, nreg, s.moe_gxq, s.moe_gxs, t.fblob, s.moe_boffs1, dim, nfe)
                matmul_mx4q8_batch_groupn(s.moe_ghb2, t.mxq, t.mxs, s.moe_offs3, nreg, s.moe_gxq, s.moe_gxs, t.fblob, s.moe_boffs3, dim, nfe)
            } else {
                matmul_mx4q8_batch_groupn(s.moe_ghb, t.mxq, t.mxs, s.moe_offs1, nreg, s.moe_gxq, s.moe_gxs, dim, nfe)
                matmul_mx4q8_batch_groupn(s.moe_ghb2, t.mxq, t.mxs, s.moe_offs3, nreg, s.moe_gxq, s.moe_gxs, dim, nfe)
            }
            prof_add("mm_gemm", ts_g1)
            let ts_act = ref_time_ticks()
            gate_batch(s.moe_ghb, s.moe_ghb2, c.ffn_act, nfe, nk)
            prof_add("act", ts_act)
            if (fused_kq) {   // kq v2: the down stacks read Q8_K-form rows (+ the per-16 sums the fold consumes)
                requant_rows_q8k_bs(s.moe_ghb, nfe, nk, s.moe_hq, s.moe_hs, s.moe_hbs)
            } else {
                requant_rows_q8(s.moe_ghb, nfe, nk, s.moe_hq, s.moe_hs)
            }
            let ts_g3 = ref_time_ticks()
            if (fused_kq) {
                mm_b_kq_groupn(s.moe_gout, t, f2, s.moe_offs2, nreg, s.moe_hq, s.moe_hs, s.moe_hbs, nfe, dim)
            } elif (c.moe_exps_bias) {   // ... and the down biases, inside the routed weight (ggml_add_id order)
                matmul_mx4q8_batch_groupn(s.moe_gout, t.mxq, t.mxs, s.moe_offs2, nreg, s.moe_hq, s.moe_hs, t.fblob, s.moe_boffs2, nfe, dim)
            } else {
                matmul_mx4q8_batch_groupn(s.moe_gout, t.mxq, t.mxs, s.moe_offs2, nreg, s.moe_hq, s.moe_hs, nfe, dim)
            }
            prof_add("mm_gemm", ts_g3)
        }
        // threaded bucket scatter: serial was an ~8ms ALL-LANE hole here (~67MB traffic at pp512/30B)
        if (!gpu_comb) {
            trace_tag(TRACE_TAG_PARK)
            let ts_park = ref_time_ticks()
            unsafe {
                let gp = addr < float const? >(s.moe_gout[0])
                var ep = addr(s.moe_eout[0])
                let bp = addr < int64 const? >(s.moe_bkt[0])
                maybe_parallel_for(nk * dim >= g_act_par_threshold, 0, int(nk), get_dispatch_lanes()) $(rb, re) {
                    unsafe {
                        for (j in range(rb, re)) {
                            let jj = int64(j)
                            copy_floats(ep + bp[jj] * dim, gp + jj * dim, dim)
                        }
                    }
                }
            }
            prof_add("moe_park", ts_park)
        }
    }

    // 3. one batched GEMM chain per touched expert; raw down rows park for the ordered reduce (skipped when fused ran)
    if (!fused) {
        trace_tag(TRACE_TAG_MOE)
    }
    for (e in range64(fused ? 0l : ne)) {
        let b0 = e == 0l ? 0l : s.moe_cnt[e - 1l]
        let cnt = s.moe_cnt[e] - b0
        if (cnt == 0l) {
            continue
        }
        let ts_gather = ref_time_ticks()
        moe_gather_rows(s, b0, cnt, dim, k, moe_kform, moe_kform)
        prof_add("moe_gather", ts_gather)
        let eg = l * ne + e
        let base = e * dim * nfe
        if (kq_layer) {   // native kq batch tiles off the repacked planes, per-kind fmt (down may differ)
            if (f1 != KqFmt.q8) {
                mm_b_kq_pre(s.moe_ghb, t, f1, t.we1_offs[l] + base, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe, cnt)
            } else {
                mm_b_q8_pre(s.moe_ghb, t, t.we1_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
            }
            if (f3 != KqFmt.q8) {
                mm_b_kq_pre(s.moe_ghb2, t, f3, t.we3_offs[l] + base, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe, cnt)
            } else {
                mm_b_q8_pre(s.moe_ghb2, t, t.we3_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
            }
        } elif (t.experts_mx4 && g_active_mx4_native_batch) {
            // native mx4 batch GEMMs straight off the nibble/E8M0 planes — no expansion
            mm_b_mx4_pre(s.moe_ghb, t, t.we1_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
            mm_b_mx4_pre(s.moe_ghb2, t, t.we3_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
        } elif (t.experts_mx4) {   // exact Q8 expansion of this expert, then the proven batch GEMMs
            let ts_ex = ref_time_ticks()
            expand_mx4_region_q8(t, t.we1_offs[l] + base, dim, nfe, s.mx4_wq, s.mx4_ws)
            prof_add("moe_expand", ts_ex)
            let ts_g1 = ref_time_ticks()
            matmul_q8q8_batch(s.moe_ghb, s.mx4_wq, s.mx4_ws, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
            prof_add("mm_gemm", ts_g1)
            let ts_ex2 = ref_time_ticks()
            expand_mx4_region_q8(t, t.we3_offs[l] + base, dim, nfe, s.mx4_wq, s.mx4_ws)
            prof_add("moe_expand", ts_ex2)
            let ts_g2 = ref_time_ticks()
            matmul_q8q8_batch(s.moe_ghb2, s.mx4_wq, s.mx4_ws, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
            prof_add("mm_gemm", ts_g2)
        } else {
            mm_b_q8_pre(s.moe_ghb, t, t.we1_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
            mm_b_q8_pre(s.moe_ghb2, t, t.we3_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
        }
        if (c.moe_exps_bias) {  // gpt-oss: this expert's gate/up biases before the activation
            for (r in range64(cnt)) {
                add_bias(s.moe_ghb, r * nfe, t.fblob, t.web1_off + eg * nfe, nfe)
                add_bias(s.moe_ghb2, r * nfe, t.fblob, t.web3_off + eg * nfe, nfe)
            }
        }
        let ts_act = ref_time_ticks()
        gate_batch(s.moe_ghb, s.moe_ghb2, c.ffn_act, nfe, cnt)
        prof_add("act", ts_act)
        if (f2 != KqFmt.q8) {   // kq v2: Q8_K-form rows + the per-16 sums the fold consumes
            requant_rows_q8k_bs(s.moe_ghb, nfe, cnt, s.moe_hq, s.moe_hs, s.moe_hbs)
        } else {
            requant_rows_q8(s.moe_ghb, nfe, cnt, s.moe_hq, s.moe_hs)
        }
        if (f2 != KqFmt.q8) {
            mm_b_kq_pre(s.moe_gout, t, f2, t.we2_offs[l] + base, s.moe_hq, s.moe_hs, s.moe_hbs, nfe, dim, cnt)
        } elif (t.experts_mx4 && g_active_mx4_native_batch) {
            mm_b_mx4_pre(s.moe_gout, t, t.we2_offs[l] + base, s.moe_hq, s.moe_hs, nfe, dim, cnt)
        } elif (t.experts_mx4) {
            let ts_ex3 = ref_time_ticks()
            expand_mx4_region_q8(t, t.we2_offs[l] + base, nfe, dim, s.mx4_wq, s.mx4_ws)
            prof_add("moe_expand", ts_ex3)
            let ts_g3 = ref_time_ticks()
            matmul_q8q8_batch(s.moe_gout, s.mx4_wq, s.mx4_ws, s.moe_hq, s.moe_hs, nfe, dim, cnt)
            prof_add("mm_gemm", ts_g3)
        } else {
            mm_b_q8_pre(s.moe_gout, t, t.we2_offs[l] + base, s.moe_hq, s.moe_hs, nfe, dim, cnt)
        }
        if (c.moe_exps_bias) {  // ... and its down bias, inside the routed weight (ggml_add_id order)
            for (r in range64(cnt)) {
                add_bias(s.moe_gout, r * dim, t.fblob, t.web2_off + eg * dim, dim)
            }
        }
        let ts_park = ref_time_ticks()
        for (r in range64(cnt)) {
            copy_floats(s.moe_gout, r * dim, s.moe_eout, s.moe_bkt[b0 + r] * dim, dim)
        }
        prof_add("moe_park", ts_park)
    }

    // 4. shared expert for ALL positions — a plain dense batched FFN off the whole-batch image
    if (nsh > 0l) {
        trace_tag(TRACE_TAG_SHEXP)
        let she = dim * nsh
        mm_b_q8_pre(s.moe_ghb, t, t.wsh1_off + l * she, s.xqb, s.xsb, dim, nsh, npos)
        mm_b_q8_pre(s.moe_ghb2, t, t.wsh3_off + l * she, s.xqb, s.xsb, dim, nsh, npos)
        let ts_act2 = ref_time_ticks()
        gate_batch(s.moe_ghb, s.moe_ghb2, c.ffn_act, nsh, npos)
        prof_add("act", ts_act2)
        requant_rows_q8(s.moe_ghb, nsh, npos, s.moe_hq, s.moe_hs)
        mm_b_q8_pre(s.moe_gout, t, t.wsh2_off + l * she, s.moe_hq, s.moe_hs, nsh, dim, npos)
    }

    // 5. ordered reduce into xb2_b, decode's accumulation order (bit-identical); plain mul+add, not axpy
    trace_tag(TRACE_TAG_PARK)
    let ts_red = ref_time_ticks()
    unsafe {
        var ap = addr(s.xb2_b[0])
        let ep = addr<float const?>(s.moe_eout[0])
        let wp = addr<float const?>(s.moe_w_b[0])
        let sp = nsh > 0l ? addr<float const?>(s.moe_gout[0]) : null
        let gp = nsh > 0l && c.moe_shexp_gated ? addr<float const?>(s.moe_gl_b[0]) : null
        maybe_parallel_for(nk * dim >= g_act_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    var arow = ap + pp * dim
                    if (gpu_comb) {
                        let cr = ep + pp * dim
                        for (i in range64(dim)) {
                            arow[i] = cr[i]
                        }
                    } else {
                        for (i in range64(dim)) {
                            arow[i] = 0.0
                        }
                        for (j in range64(k)) {
                            let w = wp[pp * k + j]
                            let er = ep + (pp * k + j) * dim
                            for (i in range64(dim)) {
                                arow[i] += w * er[i]
                            }
                        }
                    }
                    if (sp != null) {
                        let g = gp != null ? gp[pp] : 1.0   // ungated shexp (glm4moe) adds at weight 1
                        let sr = sp + pp * dim
                        for (i in range64(dim)) {
                            arow[i] += g * sr[i]
                        }
                    }
                }
            }
        }
    }
    prof_add("moe_reduce", ts_red)
    // post-norm + residual (the layer-output scale tail is shared with the reference path)
    let ts_resid = ref_time_ticks()
    if (c.pre_post_norm) {
        rms_batch(s.xb2_b, s.xb2_b, t, t.rms_post_ffn_off + l * dim, dim, npos)
    }
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
    prof_add("act_resid", ts_resid)
}

// gemma4 MoE router input, batched: twin of gemma4_router_tmp over npos positions. Sum-of-squares
// accumulates in DOUBLE to match ggml_float — top-8 select is near-tie-brittle, float SoS would flip
// experts. Bit-identical per row to the decode single-token path.
def private gemma4_router_batch(t : Model; var rin : array<float>; src : array<float>; l, dim, npos : int64) {
    let eps = t.config.norm_eps
    let inv_d = 1.0 / sqrt(float(dim))
    unsafe {
        var op = addr(rin[0])
        let ip = addr < float const? >(src[0])
        let scp = addr < float const? >(t.fblob[t.moe_router_scale_off + l * dim])
        maybe_parallel_for(dim * npos >= g_norm_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let off = int64(p) * dim
                    var ss = 0.0lf
                    for (i in range64(dim)) {
                        let v = ip[off + i]
                        ss += double(v * v)
                    }
                    let scale = 1.0 / sqrt(float(ss / double(dim)) + eps)
                    for (i in range64(dim)) {
                        op[off + i] = ip[off + i] * scale * inv_d * scp[i]
                    }
                }
            }
        }
    }
}

// gemma4 MoE prefill, grouped: batched twin of gemma4_moe_ffn (26B-A4B). Routes, buckets by expert,
// runs one Q8 batch GEMM chain per touched expert, then the dense shared expert over all positions.
// Bit-identical per row to the naive per-position path — g_moe_grouped_prefill is a pure perf lever.
def private gemma4_moe_prefill_grouped(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    let nb = dim / 32l
    let ne = c.n_expert
    let k = c.n_expert_used
    let nfe = c.n_ff_exp
    let nk = npos * k
    // per-layer expert stack formats (kq layers: the gate routed us here only with the batch tile up)
    let kq_layer = moe_layer_kq(t, l)
    let f1 = fmt_at(t.we1_fmt, l)
    let f2 = fmt_at(t.we2_fmt, l)
    let f3 = fmt_at(t.we3_fmt, l)
    s.moe_rlog |> resize(npos * ne)
    s.moe_idx_b |> resize(nk)
    s.moe_w_b |> resize(nk)
    s.moe_cnt |> resize(ne)
    s.moe_bkt |> resize(nk)
    s.moe_gxq |> resize(npos * dim)
    s.moe_gxs |> resize(npos * nb)
    s.moe_ghb |> resize(npos * nfe)
    s.moe_ghb2 |> resize(npos * nfe)
    s.moe_hq |> resize(npos * nfe)
    s.moe_hs |> resize(npos * nfe / 32l)
    s.moe_gout |> resize(npos * dim)
    s.moe_eout |> resize(nk * dim)
    if (kq_layer) {
        s.moe_gxbs |> resize(npos * dim / 16l)
        s.moe_hbs |> resize(npos * nfe / 16l)
    }

    // 1. route every position: double-RMS router input, one batched GEMM, per-row select + down-scale fold
    trace_tag(TRACE_TAG_ROUTE)
    let ts_route = ref_time_ticks()
    gemma4_router_batch(t, s.xb2_b, s.x_b, l, dim, npos)
    mm_fblob_b(s.moe_rlog, t, t.router_off + l * ne * dim, s.xb2_b, dim, ne, npos)
    for (p in range64(npos)) {
        moe_select(c, unsafe(addr(s.moe_rlog[p * ne])), s.moe_idx_b, s.moe_w_b, p * k)
        for (j in range64(k)) {
            s.moe_w_b[p * k + j] *= t.fblob[t.moe_down_scale_off + l * ne + s.moe_idx_b[p * k + j]]
        }
    }
    // bucket (position, slot) pairs by expert — CSR in-place cursor trick (moe_cnt[e] ends as its run END)
    for (e in range64(ne)) {
        s.moe_cnt[e] = 0l
    }
    for (r in range64(nk)) {
        s.moe_cnt[s.moe_idx_b[r]]++
    }
    var run = 0l
    for (e in range64(ne)) {
        let cn = s.moe_cnt[e]
        s.moe_cnt[e] = run
        run += cn
    }
    for (r in range64(nk)) {
        let e = s.moe_idx_b[r]
        s.moe_bkt[s.moe_cnt[e]] = r
        s.moe_cnt[e]++
    }
    prof_add("moe_route", ts_route)

    // 2. expert input = pre_ffw_norm_2(residual) per position, quantized ONCE for every expert GEMM
    rms_batch(s.xb_b, s.x_b, t, t.rms_pre_ffn2_off + l * dim, dim, npos)
    // kq v2: kq gate/up stacks gather off a Q8_K-form image; q8 stacks off the Q8_0 image
    let moe_kform = kq_layer && moe_pair_kform(f1, f3)
    if (moe_kform) {
        s.kxqb |> resize(npos * dim)
        s.kxsb |> resize(npos * dim / 256l)
        s.kxbsb |> resize(npos * dim / 16l)
        requant_rows_q8k_bs(s.xb_b, dim, npos, s.kxqb, s.kxsb, s.kxbsb)
    } else {
        requant_rows_q8(s.xb_b, dim, npos, s.xqb, s.xsb)
    }

    // 3. one batched GEMM chain per touched expert; raw down-projection rows park for the ordered reduce
    trace_tag(TRACE_TAG_MOE)
    for (e in range64(ne)) {
        let b0 = e == 0l ? 0l : s.moe_cnt[e - 1l]
        let cnt = s.moe_cnt[e] - b0
        if (cnt == 0l) {
            continue
        }
        let ts_gather = ref_time_ticks()
        moe_gather_rows(s, b0, cnt, dim, k, moe_kform, moe_kform)
        prof_add("moe_gather", ts_gather)
        let base = e * dim * nfe
        if (f1 != KqFmt.q8) {
            mm_b_kq_pre(s.moe_ghb, t, f1, t.we1_offs[l] + base, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe, cnt)
        } else {
            mm_b_q8_pre(s.moe_ghb, t, t.we1_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
        }
        if (f3 != KqFmt.q8) {
            mm_b_kq_pre(s.moe_ghb2, t, f3, t.we3_offs[l] + base, s.moe_gxq, s.moe_gxs, s.moe_gxbs, dim, nfe, cnt)
        } else {
            mm_b_q8_pre(s.moe_ghb2, t, t.we3_offs[l] + base, s.moe_gxq, s.moe_gxs, dim, nfe, cnt)
        }
        let ts_act = ref_time_ticks()
        gate_batch(s.moe_ghb, s.moe_ghb2, c.ffn_act, nfe, cnt)
        prof_add("act", ts_act)
        if (f2 != KqFmt.q8) {
            // kq v2: Q8_K-form rows + the per-16 sums the fold consumes
            requant_rows_q8k_bs(s.moe_ghb, nfe, cnt, s.moe_hq, s.moe_hs, s.moe_hbs)
            mm_b_kq_pre(s.moe_gout, t, f2, t.we2_offs[l] + base, s.moe_hq, s.moe_hs, s.moe_hbs, nfe, dim, cnt)
        } else {
            requant_rows_q8(s.moe_ghb, nfe, cnt, s.moe_hq, s.moe_hs)
            mm_b_q8_pre(s.moe_gout, t, t.we2_offs[l] + base, s.moe_hq, s.moe_hs, nfe, dim, cnt)
        }
        let ts_park = ref_time_ticks()
        for (r in range64(cnt)) {
            copy_floats(s.moe_gout, r * dim, s.moe_eout, s.moe_bkt[b0 + r] * dim, dim)
        }
        prof_add("moe_park", ts_park)
    }

    // 4. ordered reduce into xb2_b: k weighted routed outputs in slot order, matching decode's accumulation order
    let ts_red = ref_time_ticks()
    unsafe {
        var ap = addr(s.xb2_b[0])
        let ep = addr < float const? >(s.moe_eout[0])
        let wp = addr < float const? >(s.moe_w_b[0])
        maybe_parallel_for(nk * dim >= g_act_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    var arow = ap + pp * dim
                    for (i in range64(dim)) {
                        arow[i] = 0.0
                    }
                    for (j in range64(k)) {
                        let w = wp[pp * k + j]
                        let er = ep + (pp * k + j) * dim
                        for (i in range64(dim)) {
                            arow[i] += w * er[i]
                        }
                    }
                }
            }
        }
    }
    prof_add("moe_reduce", ts_red)
    rms_batch(s.xb2_b, s.xb2_b, t, t.rms_post_ffn2_off + l * dim, dim, npos)

    // 5. dense parallel shared expert over ALL positions: rms_ffn -> GeGLU -> post_ffw_norm_1 into gout
    let ts_sh = ref_time_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_ffn_off + l * dim, dim, npos)
    mm_b_f(s, s.hb_b, t, fmt_at(t.w1_fmt, l), t.w1_offs[l], s.xb_b, dim, hidden, npos)
    mm_b_f(s, s.hb2_b, t, fmt_at(t.w3_fmt, l), t.w3_offs[l], s.xb_b, dim, hidden, npos)
    gate_batch(s.hb_b, s.hb2_b, c.ffn_act, hidden, npos)
    mm_b_f(s, s.moe_gout, t, fmt_at(t.w2_fmt, l), t.w2_offs[l], s.hb_b, hidden, dim, npos)
    rms_batch(s.moe_gout, s.moe_gout, t, t.rms_post_ffn1_off + l * dim, dim, npos)
    prof_add("mm_shexp", ts_sh)

    // 6. combine (routed + dense), shared post_ffw_norm, residual add; layer-output scale is the caller's job
    add_inplace(unsafe(addr(s.xb2_b[0])), unsafe(addr(s.moe_gout[0])), npos * dim)
    rms_batch(s.xb2_b, s.xb2_b, t, t.rms_post_ffn_off + l * dim, dim, npos)
    let ts_resid = ref_time_ticks()
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
    prof_add("act_resid", ts_resid)
}

// MoE FFN, prefill: norm, then either the grouped expert-bucketed path (Q8, default) or the
// per-position reference path through the decode scratch (fp32 models — q4_0 never prefills — and
// the bit-identical A/B switch).
def private ffn_moe_prefill(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    if (l < c.n_layer_dense_lead) {   // glm4moe: the leading layer(s) are plain dense FFN
        ffn_dense_prefill(t, s, l, npos)
        return
    }
    let dim = c.dim
    let ts_norm = ref_time_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_ffn_off + l * dim, dim, npos)
    prof_add("norm", ts_norm)
    // native-MXFP4 grouped path pays a per-touched-expert Q8 expansion (~120MB/expert); under ~8 rows/expert the per-position mx4 GEMVs are cheaper
    let mx4_grouped_pays = !t.experts_mx4 || npos * c.n_expert_used >= 8l * c.n_expert
    // kq-expert layers ride grouped buckets via the kq batch tile; portable/no-kq backends take the per-position path
    let kq_grouped_ok = !moe_layer_kq(t, l) || (t.kq_repacked && kernel_backend_has_kq_batch())
    if (t.quant == QuantMode.q8 && g_moe_grouped_prefill && mx4_grouped_pays && kq_grouped_ok) {
        ffn_moe_prefill_grouped(t, s, l, npos)
    } else {
        // per-position reference path — moe_ffn_core carries its own section buckets
        for (p in range64(npos)) {
            copy_floats(s.xb_b, p * dim, s.xb, 0l, dim)
            moe_ffn_core(t, s, l)
            if (c.pre_post_norm) {
                rms_at(s.moe_acc, t, t.rms_post_ffn_off + l * dim, s.moe_acc, dim)
            }
            add_inplace(unsafe(addr(s.x_b[p * dim])), unsafe(addr(s.moe_acc[0])), dim)   // residual add into this position's x_b row
        }
    }
    if (c.layer_out_scale) {
        let os = t.fblob[t.out_scale_off + l]
        scale_inplace(unsafe(addr(s.x_b[0])), os, npos * dim)
    }
}

def forward_prefill(t : Model; var s : Session; tokens : array<int64>; npos, start_pos : int64) {
    let c = t.config
    if (t.quant == QuantMode.q4_0 || npos <= 0l) {
        for (p in range64(npos)) {
            forward(t, s, tokens[p], start_pos + p)
        }
        return
    }
    let ts_alloc = ref_time_ticks()
    kv_ensure(s, start_pos + npos)   // paged sessions: grow the block table BEFORE any cache-pointer hoist
    forward_prefill_alloc(t, s, npos, start_pos)
    prof_add("alloc", ts_alloc)

    // token embeddings -> residual stream x_b [npos x dim]
    let dim = c.dim
    let ts_embed = ref_time_ticks()
    for (p in range64(npos)) {
        embed_row(t, tokens[p], unsafe(addr(s.x_b[p * dim])))
    }
    if (c.embed_scale != 1.0) {  // Gemma2: scale the embedding by sqrt(dim)
        scale_inplace(unsafe(addr(s.x_b[0])), c.embed_scale, npos * dim)
    }
    prof_add("embed", ts_embed)
    if (has_ple(c)) {  // gemma-4 E-series: build each position's per-(layer) side input before the stack
        let ts_ple = ref_time_ticks()
        ple_pre_prefill(t, s, tokens, npos)
        prof_add("ple", ts_ple)
    }

    // spec on: the draft head's SEAM row (start_pos-1) must be written BEFORE the body — the body's
    // own tail overwrites mtp_h with THIS window's last hidden. Cold/foreign handoff just skips it.
    if (g_mtp_spec && c.n_layer_nextn > 0l && start_pos > 0l && s.mtp_h_pos1 == start_pos) {
        forward_mtp(t, s, tokens[0], start_pos - 1l, false)
    }
    forward_prefill_body(t, s, npos, start_pos)
    if (g_mtp_spec && c.n_layer_nextn > 0l) {   // spec on: maintain the draft head's KV alongside the trunk's
        mtp_prefill_warm(t, s, tokens, npos, start_pos)
    }
}

//! Batched prefill from PRE-BUILT embeddings (npos x dim) instead of token ids — the multimodal
//! splice entry (text rows from `embed_text_rows`, media rows from the audio tower). Same KV/logits
//! behavior as `forward_prefill`; embeddings go in as-is, no embed_scale applied here.
def forward_prefill_embd(t : Model; var s : Session; embd : array<float>; npos, start_pos : int64) {
    if (npos <= 0l) {
        return
    }
    if (t.quant == QuantMode.q4_0) {
        panic("forward_prefill_embd: q4_0 mode has no batched prefill path (use q8 or fp32)")
    }
    if (int64(length(embd)) < npos * t.config.dim) {
        panic("forward_prefill_embd: embd has {length(embd)} floats — npos={npos} rows of dim={t.config.dim} need {npos * t.config.dim}")
    }
    kv_ensure(s, start_pos + npos)   // paged sessions: grow the block table BEFORE any cache-pointer hoist
    forward_prefill_alloc(t, s, npos, start_pos)
    let dim = t.config.dim
    let ts_embed = ref_time_ticks()
    for (i in range64(npos * dim)) {
        s.x_b[i] = embd[i]
    }
    prof_add("embed", ts_embed)
    if (has_ple(t.config)) {  // gemma-4 E-series: PLE side input (padding-token gather for the media path)
        let ts_ple = ref_time_ticks()
        ple_pre_prefill_pad(t, s, npos)
        prof_add("ple", ts_ple)
    }
    forward_prefill_body(t, s, npos, start_pos)
}

//! Fill ``dst[at*dim ..]`` with the embedding rows of ``tokens`` (embed_scale applied) — the
//! text-span half of a multimodal prefill buffer.
def embed_text_rows(t : Model; tokens : array<int64>; var dst : array<float>; at : int64) {
    let dim = t.config.dim
    let n = int64(length(tokens))
    for (p in range64(n)) {
        embed_row(t, tokens[p], unsafe(addr(dst[(at + p) * dim])))
    }
    if (t.config.embed_scale != 1.0) {
        scale_inplace(unsafe(addr(dst[at * dim])), t.config.embed_scale, n * dim)
    }
}

//! ``eval_``'s embedding-input twin: prefill ``npos`` embedding rows at the session cursor and
//! advance it. Leaves ``s.logits`` = the last position's logits.
def eval_embd_(t : Model; var s : Session; embd : array<float>; npos : int64) {
    if (npos <= 0l) {
        return
    }
    forward_prefill_embd(t, s, embd, npos, s.n_past)
    s.n_past += npos
}

//! Mean-pooled, L2-normalized sentence embedding for `tokens` into `out`: last-layer hidden state
//! averaged over positions, unit-normalized. Runs one forward over `s` from position 0 — pass a
//! FRESH session (no KV history assumed; cache is left filled).
def embed_forward(t : Model; var s : Session; tokens : array<int64>; npos : int64; var out : array<float>) {
    let c = t.config
    let dim = c.dim
    out |> resize(dim)
    for (i in range64(dim)) {
        out[i] = 0.0
    }
    if (npos <= 0l) {
        return
    }
    if (t.quant != QuantMode.q4_0 && npos > 1l) {
        // batched prefill fills s.x_b (npos × dim residual) and leaves it intact; the body norms only
        // the last position, so re-apply the final RMSNorm to EVERY position and accumulate.
        forward_prefill(t, s, tokens, npos, 0l)
        for (p in range64(npos)) {
            rms_at(s.xb, t, t.rms_final_off, s.x_b, p * dim, dim)
            add_inplace(unsafe(addr(out[0])), unsafe(addr(s.xb[0])), dim)
        }
    } else {
        // q4_0 (no batched prefill) or a single token: per-token forward leaves s.xb = post-final-norm hidden.
        for (p in range64(npos)) {
            forward(t, s, tokens[p], p)
            add_inplace(unsafe(addr(out[0])), unsafe(addr(s.xb[0])), dim)
        }
    }
    s.n_past = npos   // KV now holds npos positions (start_pos was 0) — keep the session cursor consistent
    // mean over positions, then L2-normalize
    let inv = 1.0 / float(npos)
    var ss = 0.0
    for (i in range64(dim)) {
        out[i] *= inv
        ss += out[i] * out[i]
    }
    let rn = ss > 0.0 ? 1.0 / sqrt(ss) : 0.0
    for (i in range64(dim)) {
        out[i] *= rn
    }
}

//! Mean-pooled, L2-normalized sentence embedding of `text` (`dim` floats): tokenize, run one forward,
//! average the decoder's last-layer hidden state over positions, unit-normalize. Decoder-as-embedder
//! yields RAG-grade vectors, not a substitute for a dedicated embedding model.
def embed_(m : Model; text : string) : array<float> {
    var s <- create_session_(m)
    let tokens = encode_(m, text)
    var out : array<float>
    embed_forward(m, s, tokens, int64(length(tokens)), out)
    delete s
    return <- out
}

// batched-prefill scratch sizing, shared by token and embedding entries. grow-only no-init:
// contents are stage output (fully overwritten); `delete` on grow skips the realloc's old-block
// copy + zero-fill (~5ms saved on a first 512-row prefill).
def private scratch_resize(var a : array<auto(numT)>; need : int64) {
    if (int64(length(a)) < need) {
        delete a
    }
    a |> resize_no_init(need)
}

def private forward_prefill_alloc(t : Model; var s : Session; npos, start_pos : int64) {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    let kv_dim = max_kv_dim(c)         // scratch sizes to the larger layer class (gemma4: swa vs global)
    let head_size = max_head_size(c)
    let n_heads = c.n_heads
    let qd = n_heads * head_size       // query / attention-output width (== dim except Gemma2/gemma4)
    let xdim = max(dim, qd)            // xb_b holds the norm output (dim) then the attn output (qd)
    let maxn = max(xdim, hidden)
    // no-init: every array here is stage output, fully overwritten before read — zero-fill was ~5ms waste
    s.x_b |> scratch_resize(npos * dim)
    s.xb_b |> scratch_resize(npos * xdim)
    s.xb2_b |> scratch_resize(npos * dim)
    s.q_b |> scratch_resize(npos * qd)
    s.k_b |> scratch_resize(npos * kv_dim)
    s.v_b |> scratch_resize(npos * kv_dim)
    s.hb_b |> scratch_resize(npos * hidden)
    s.hb2_b |> scratch_resize(npos * hidden)
    s.xqb |> scratch_resize(npos * maxn)
    s.xsb |> scratch_resize(npos * maxn / 32l)
    if (t.kquant_native) {   // kq batch tile: per-16 sums beside the batch quants
        s.xbsb |> resize(npos * maxn / 16l)
    }
    if (has_ple(c)) {   // gemma-4 E-series PLE prefill scratch (npos positions)
        let all = c.n_layers * c.n_embd_per_layer
        s.ple_inp |> resize(npos * all)
        s.ple_proj |> resize(npos * all)
        s.ple_gate |> resize(npos * c.n_embd_per_layer)
    }
    if (c.recr_mask != 0ul) {   // qwen35 deltanet batched projections (the recurrence itself is per-token)
        let cd = dn_conv_dim(c)
        s.dn_qkv_b |> resize(npos * cd)
        s.dn_z_b |> resize(npos * c.ssm_d_inner)
        s.dn_beta_b |> resize(npos * c.ssm_dt_rank)
        s.dn_g_b |> resize(npos * c.ssm_dt_rank)
        s.dn_o_b |> resize(npos * c.ssm_d_inner)
        if (g_dn_prefill_mode == DnPrefillMode.chunked) {   // batched conv rows + per-head chunk slabs
            s.dn_conv_b |> resize(npos * cd)
            s.dn_wtap |> resize(c.ssm_d_conv * cd)
            s.dn_cws |> resize(c.ssm_dt_rank * dn_cws_head_size(c))
        }
    }
    if (c.q_gated) {   // gated attention: batched fused [q | gate] projection + gate halves
        s.dn_qg_b |> resize(npos * 2l * qd)
        s.dn_qgate_b |> resize(npos * qd)
    }
    // sized for blocked (ATTN_BLOCK_Q rows/head); classic indexes just the first row, a harmless superset
    if (g_attn_prefill_mode == AttnPrefillMode.flash) {  // per-head flash packing scratch (see Session)
        s.fa_q |> resize(n_heads * ATTN_FLASH_QT * head_size)
        s.fa_k |> resize(n_heads * head_size * ATTN_FLASH_KV)
        s.fa_v |> resize(n_heads * ATTN_FLASH_KV * head_size)
        s.fa_kq |> resize(n_heads * ATTN_FLASH_QT * ATTN_FLASH_KV)
        s.fa_vkq |> resize(n_heads * ATTN_FLASH_QT * head_size)
        s.fa_tmp |> resize(n_heads * head_size)
    } else {
        s.att_b |> resize(n_heads * ATTN_BLOCK_Q * (start_pos + npos))
    }
}

// the shared prefill core: rope table + layer stack + final norm/classifier, over a pre-filled
// s.x_b residual stream (token or embedding sourced)
def private forward_prefill_body(t : Model; var s : Session; npos, start_pos : int64) {
    let c = t.config
    let dim = c.dim

    // one npos x head_size/2 cos/sin table replaces per-head/per-layer pow/cos/sin recomputation
    let use_rope_tab = g_use_rope_table
    let ts_rope_build = ref_time_ticks()
    if (use_rope_tab) {
        // swa table uses the sliding class's head size and drops factors on p-RoPE arches (gemma4)
        build_rope_table(s.rope_cos, s.rope_sin, t, c.rope_freq_base, c.rope_freq_scale, start_pos, npos,
                         c.rope_dim > 0l ? c.rope_dim : c.head_size, true)
        if (has_dual_rope(c)) {
            let swa_hs = c.head_size_swa > 0l ? c.head_size_swa : c.head_size
            build_rope_table(s.rope_cos_swa, s.rope_sin_swa, t, c.rope_freq_base_swa, c.rope_freq_scale_swa,
                             start_pos, npos, swa_hs, !c.rope_ff_global_only)
        }
    }
    prof_add("rope_build", ts_rope_build)   // one-time cos/sin table build (separate bucket)

    // an active whole-prefill override may claim the entire stack (false = decline, fall back to the CPU loop)
    var stack_done = false
    g_prefill_logits_done = false
    if (!empty(g_prefill_override_name)) {
        stack_done = invoke(g_prefill_override, t, s, npos, start_pos)
    }
    if (!stack_done) {
        for (l in range64(c.n_layers)) {
            trace_marker("layer", int(l))   // unit boundary for the timeline viewer (no-op untraced)
            trace_tag(TRACE_TAG_ATTN_STD)   // per-lane trace: color the prefill attention section
            t.blocks.attn_prefill(t, s, l, npos, start_pos)
            trace_tag(TRACE_TAG_FFN_STD)
            t.blocks.ffn_prefill(t, s, l, npos)
        }
    }

    // final rmsnorm + classifier on the LAST position only (that's what we sample next)
    return if (stack_done && g_prefill_logits_done)   // the override already produced s.logits
    let ts_final = ref_time_ticks()
    let last = npos - 1l
    rms_at(s.xb, t, t.rms_final_off, s.x_b, last * dim, dim)
    if (c.n_layer_nextn > 0l) {   // stash the last position's h_nextn for the MTP draft head
        copy_floats(s.xb, 0l, s.mtp_h, 0l, dim)
        s.mtp_h_pos1 = start_pos + last + 1l
    }
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    let tied_f32 = c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8
    if (c.final_logit_softcap > 0.0 && !tied_f32 && !moe_gpu_cls_on_gpu(t.wcls_off) && fused_cls_ok(t)) {
        cls_softcap_chain(t, s, c.final_logit_softcap)
    } else {
        mm_cls(s.logits, t, s.xb, s.xq, s.xs, s.xbs, dim, c.vocab_size)
        if (c.final_logit_softcap > 0.0) {  // Gemma2: soft-cap the output logits
            softcap4(s.logits, c.vocab_size, c.final_logit_softcap)
        }
    }
    for (id in t.suppress) {  // gemma4: pin the checkpoint's suppressed token ids to -inf
        s.logits[id] = -1.0e30
    }
    prof_add("final", ts_final)
}

//! Unified batch entry: process `npos` tokens at start_pos..start_pos+npos-1. npos==1 dispatches to
//! forward() (decode); npos>1 to forward_prefill. Leaves s.logits at the last token, extends KV by npos.
def forward_batch(t : Model; var s : Session; tokens : array<int64>; npos, start_pos : int64) {
    if (npos == 1l) {
        forward(t, s, tokens[0], start_pos)
    } else {
        forward_prefill(t, s, tokens, npos, start_pos)
    }
}

// ===== batched decode =====
// One decode step over B sessions through ONE pass of the weights: GEMVs become B-row GEMMs via
// the batched prefill blocks; attention stays per-(row, head) since each reads only ITS session's cache.

//! Caller-owned scratch for eval_batch, reused across calls (buffers grow to the largest B seen).
//! `scr` wraps a cache-less Session so the batched prefill blocks (mm_b / rms_batch / ffn_prefill)
//! apply unchanged; per call it impersonates the batch's cache geometry (dtypes + byte prefixes).
struct BatchWorkspace {
    scr : Session             // *_b scratch, rope tables, ple scratch — no KV cache / sampler state
    positions : array<int64>  // row i's absolute position (sessions[i].n_past at entry)
    logits_b : array<float>   // B x vocab classifier output, scattered per session after softcap
    // row i's cache base pointers (into sessions[i]; valid per call). @do_not_delete is load-bearing:
    // delete of a plain array<T?> CASCADES to the pointees, which would free the sessions' cache blobs
    @do_not_delete kbase : array<uint8?>
    @do_not_delete vbase : array<uint8?>
    @do_not_delete lrow : array<float?>   // row i's sessions[i].logits base (the threaded scatter)
    runs_b : array<KVRun>     // per-row physical runs, flattened rows × max-runs (flat rows: 1 each)
    nrun_b : array<int64>     // per-row run count
}

def create_batch_workspace_(model : Model) : BatchWorkspace {
    var ws : BatchWorkspace
    // the scratch Session never owns cache or sampler state — eval_batch reads/writes the real
    // sessions' caches through kbase/vbase (flat) or the batch's pool (paged)
    ws.scr <- make_run_state(model.config, KVDtype.f32, KVDtype.f32, false)
    delete ws.scr.sample_scratch
    return <- ws
}

// B-row slice of forward_prefill_alloc: exactly the buffers the batched-decode step touches (no
// prefill attention scratch — att_b sizes per layer in attention_batch_decode, fa_* never used).
def private batch_decode_alloc(t : Model; var s : Session; nrows : int64) {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    let kv_dim = max_kv_dim(c)
    let head_size = max_head_size(c)
    let qd = c.n_heads * head_size
    let xdim = max(dim, qd)
    let maxn = max(xdim, hidden)
    s.x_b |> resize(nrows * dim)
    s.xb_b |> resize(nrows * xdim)
    s.xb2_b |> resize(nrows * dim)
    s.q_b |> resize(nrows * qd)
    s.k_b |> resize(nrows * kv_dim)
    s.v_b |> resize(nrows * kv_dim)
    s.hb_b |> resize(nrows * hidden)
    s.hb2_b |> resize(nrows * hidden)
    s.xqb |> resize(nrows * maxn)
    s.xsb |> resize(nrows * maxn / 32l)
    if (t.kquant_native) {   // kq batch tile: per-16 sums beside the batch quants
        s.xbsb |> resize(nrows * maxn / 16l)
    }
    if (s.kv_dtype_k == KVDtype.q8_0 || s.kv_dtype_k == KVDtype.tq4) {   // batch query-quant planes (dtypes snapshotted before this)
        s.attq |> resize(nrows * qd)
        s.atts |> resize(nrows * qd / 32l)
    }
    if (has_ple(c)) {   // gemma-4 E-series PLE scratch (one row per batch slot)
        let all = c.n_layers * c.n_embd_per_layer
        s.ple_inp |> resize(nrows * all)
        s.ple_proj |> resize(nrows * all)
        s.ple_gate |> resize(nrows * c.n_embd_per_layer)
    }
}

// One layer's batched-decode attention: shared prefix (norm → QKV GEMM → qk/v-norm → per-row RoPE
// via the positions table) → scatter-store each row's k/v into ITS session's cache → per-(row, head)
// decode attention through the codec seam worker → shared suffix (wo GEMM → post-norm → residual).
def private attention_batch_decode(t : Model; var ws : BatchWorkspace; sessions : array<Session?>; l, nrows : int64) {
    let c = t.config
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let is_sliding = layer_is_sliding(c, l)
    let swin = c.sliding_window
    let kv_shared = t.kv_src[l] != l
    // flat rows address each session's slab; paged rows all address the batch's ONE pool for layer l
    let paged = ws.scr.kv_pool != null
    let kbb = paged ? 0l : kv_base_k(ws.scr, l, c.seq_len)   // layer slab base BYTES — uniform across the batch
    let vbb = paged ? 0l : kv_base_v(ws.scr, l, c.seq_len)   // (ws.scr carries the batch's dtypes + prefixes)
    var pkcl = paged ? kv_layer_kbase(ws.scr, l, c.seq_len) : null
    var pvcl = paged ? kv_layer_vbase(ws.scr, l, c.seq_len) : null
    let kdt = ws.scr.kv_dtype_k
    let vdt = ws.scr.kv_dtype_v
    // start_pos is only for the classic rope path; eval_batch_ never routes here (rows table bakes position)
    attn_qkv_rope_prefix(t, ws.scr, l, nrows, 0l)
    let ktq4 = kdt == KVDtype.tq4
    let vtq4 = vdt == KVDtype.tq4
    if (ktq4 || vtq4) {
        // tq4 basis change at the RoPE seam (see attention_std_decode); xb_b un-rotates below
        let ts_rot = ref_time_ticks()
        unsafe {
            let sgp = addr < float const? >(ws.scr.kv_signs[0])
            if (ktq4) {
                tq4_rotate_batch(ws.scr.q_b, nrows, qd, head_size, sgp)
                if (!kv_shared) {
                    tq4_rotate_batch(ws.scr.k_b, nrows, kv_dim, head_size, sgp)
                }
            }
            if (vtq4 && !kv_shared) {
                tq4_rotate_batch(ws.scr.v_b, nrows, kv_dim, head_size, sgp)
            }
        }
        prof_add("tq4_rot", ts_rot)
    }
    if (kdt == KVDtype.q8_0 || ktq4) {
        // q_b rows are contiguous and qd % 32 == 0, so per-32-block quants match the per-row form
        quantize_q8_0_into(ws.scr.q_b, nrows * qd, ws.scr.attq, ws.scr.atts, 0l, 0l)
    }
    if (!kv_shared) {
        let ts_kv = ref_time_ticks()
        unsafe {
            let kbp = addr < float const? >(ws.scr.k_b[0])
            let vbp = addr < float const? >(ws.scr.v_b[0])
            for (i in range64(nrows)) {
                let prow = kv_phys_row(*sessions[i], ws.positions[i])   // flat: identity
                kv_store_row(paged ? pkcl : ws.kbase[i] + kbb, kdt, kv_dim, prow, 0l, kv_dim, kbp + i * kv_dim)
                kv_store_row(paged ? pvcl : ws.vbase[i] + vbb, vdt, kv_dim, prow, 0l, kv_dim, vbp + i * kv_dim)
            }
        }
        prof_add("kv_store", ts_kv)
    }
    // per-(row, head): row i attends to ITS cache; lane count is work-proportional to KV traffic.
    let ts_attn = ref_time_ticks()
    var maxctx = 0l
    var work = 0l   // Σ 2·cnt_i·head_size MACs per head
    // runs_b row stride: a flat row is always one run; a paged row can span every page of the context
    let maxruns = paged ? (c.seq_len + ws.scr.kv_pool.page_rows - 1l) / ws.scr.kv_pool.page_rows + 1l : 1l
    ws.runs_b |> resize(nrows * maxruns)
    ws.nrun_b |> resize(nrows)
    for (i in range64(nrows)) {
        let pos = ws.positions[i]
        let kstart = (is_sliding && pos + 1l > swin) ? (pos + 1l - swin) : 0l
        maxctx = max(maxctx, pos + 1l - kstart)
        work += 2l * (pos + 1l - kstart) * head_size
        ws.nrun_b[i] = kv_runs(*sessions[i], kstart, pos + 1l, ws.runs_b, i * maxruns)   // flat: 1 identity run
    }
    ws.scr.att_b |> resize(nrows * n_heads * maxctx)
    let attn_softcap = c.attn_logit_softcap
    let attn_lanes = min(int(nrows * n_heads), lanes_for_work(work * n_heads, g_gemv_lane_cap))
    let sinksp = (c.attn_sinks
        ? unsafe(addr < float const? >(t.fblob[t.sinks_off + l * n_heads]))
        : null)
    unsafe {
        let qp = addr < float const? >(ws.scr.q_b[0])
        let kq8 = kdt == KVDtype.q8_0 || kdt == KVDtype.tq4
        let qqp0 = kq8 ? addr < int8 const? >(ws.scr.attq[0]) : null
        let qsp0 = kq8 ? addr < float const? >(ws.scr.atts[0]) : null
        var xbp = addr(ws.scr.xb_b[0])
        var attp = addr(ws.scr.att_b[0])
        let posp = addr < int64 const? >(ws.positions[0])
        let runsbp = addr < KVRun const? >(ws.runs_b[0])
        let nrunbp = addr < int64 const? >(ws.nrun_b[0])
        var kbpp = addr(ws.kbase[0])
        var vbpp = addr(ws.vbase[0])
        maybe_parallel_for(0, int(nrows * n_heads), attn_lanes) $(rb, re) {
            unsafe {
                for (idx in range(rb, re)) {
                    let ix = int64(idx)
                    let ii = ix / n_heads
                    let hh = ix % n_heads
                    let pos = posp[ii]
                    let kstart = (is_sliding && pos + 1l > swin) ? (pos + 1l - swin) : 0l
                    let cnt = pos + 1l - kstart
                    let qoff = hh * head_size
                    let kvhoff = (hh / kv_mul) * head_size
                    attn_head_decode_d(attp + ix * maxctx, xbp + ii * qd, qp + ii * qd,
                        kq8 ? qqp0 + ii * qd : null, kq8 ? qsp0 + ii * qd / 32l : null,
                        paged ? pkcl : kbpp[ii] + kbb, paged ? pvcl : vbpp[ii] + vbb, kdt, vdt,
                        runsbp + ii * maxruns, nrunbp[ii], qoff, kvhoff, kstart, cnt, kv_dim, head_size,
                        hh, scale, attn_softcap, sinksp)
                }
            }
        }
    }
    prof_add("attn", ts_attn)
    if (vtq4) {   // tq4 V: xb_b rows accumulated in the rotated basis — un-rotate per (row, head)
        unsafe {
            tq4_unrotate_batch(ws.scr.xb_b, nrows, qd, head_size, addr < float const? >(ws.scr.kv_signs[0]))
        }
    }
    attn_out_suffix(t, ws.scr, l, nrows)
}

//! One synchronous batched decode step: row i evals tokens[i] at sessions[i]'s position; each
//! session advances by one. Sessions must be distinct, same-geometry (one model, equal cache
//! sizes, uniform KV dtype); B == 1, q4_0, non-std attention, or classic rope fall back to forward().
def eval_batch_(t : Model; var ws : BatchWorkspace; var sessions : array<Session?>; tokens : array<int64>) {   // nolint:LINT014 — rows mutate THROUGH the pointers; a const array constifies the pointees
    let c = t.config
    let nrows = int64(length(sessions))
    if (nrows <= 0l) {
        return
    }
    if (int64(length(tokens)) != nrows) {
        panic("eval_batch: {length(sessions)} sessions but {length(tokens)} tokens — one token per session")
    }
    for (i in range64(nrows)) {
        if (sessions[i] == null) {
            panic("eval_batch: sessions[{i}] is null")
        }
        if (sessions[i].n_past + 1l > c.seq_len) {
            panic("eval_batch: sessions[{i}] is full (n_past={sessions[i].n_past}, seq_len={c.seq_len})")
        }
        for (j in range64(i)) {
            if (sessions[i] == sessions[j]) {
                panic("eval_batch: sessions[{j}] and sessions[{i}] are the same session — rows must be distinct")
            }
        }
    }
    var s0 = sessions[0]
    for (i in range64(1l, nrows)) {
        let sp = sessions[i]
        if (sp.kv_dtype_k != s0.kv_dtype_k || sp.kv_dtype_v != s0.kv_dtype_v) {
            panic("eval_batch: sessions[{i}] KV dtype differs from sessions[0] — the batch must be uniform")
        }
        if (sp.kv_pool != s0.kv_pool) {
            panic("eval_batch: sessions[{i}] KV pool differs from sessions[0] — one pool (or all-flat) per batch")
        }
        if (s0.kv_pool == null && (length(sp.key_cache) != length(s0.key_cache) || length(sp.value_cache) != length(s0.value_cache))) {
            panic("eval_batch: sessions[{i}] cache size differs from sessions[0] — same model + same seq_len required")
        }
    }
    // per-row fallback keeps every configuration correct: B == 1 keeps the fused decode chain, q4_0
    // has no batched kernels, non-std arches have no batch attention worker, and the classic rope
    // path can't express per-row positions
    if (nrows == 1l || t.quant == QuantMode.q4_0 || !t.blocks.attn_is_std || !g_use_rope_table) {
        for (i in range64(nrows)) {
            var sp = sessions[i]
            forward(t, *sp, tokens[i], sp.n_past)
            sp.n_past++
        }
        return
    }
    // snapshot cache geometry onto the scratch Session; kv_ensure runs for every row FIRST because
    // paged growth resizes pool blobs, so no pool pointer may be hoisted until all rows have pages.
    ws.scr.kv_dtype_k = s0.kv_dtype_k
    ws.scr.kv_dtype_v = s0.kv_dtype_v
    ws.scr.kv_signs := s0.kv_signs   // tq4: the rotation signs travel with the dtypes
    ws.scr.kv_bprefix_k := s0.kv_bprefix_k
    ws.scr.kv_bprefix_v := s0.kv_bprefix_v
    ws.scr.kv_pool = s0.kv_pool
    ws.positions |> resize(nrows)
    ws.kbase |> resize(nrows)
    ws.vbase |> resize(nrows)
    for (i in range64(nrows)) {
        var sp = sessions[i]
        kv_ensure(*sp, sp.n_past + 1l)
        ws.positions[i] = sp.n_past
        if (s0.kv_pool == null) {
            unsafe {
                ws.kbase[i] = addr(sp.key_cache[0])
                ws.vbase[i] = addr(sp.value_cache[0])
            }
        } else {
            ws.kbase[i] = null   // paged rows address the pool blobs, hoisted per layer
            ws.vbase[i] = null
        }
    }
    batch_decode_alloc(t, ws.scr, nrows)
    // embed each row's token into the residual stream
    let dim = c.dim
    let ts_embed = ref_time_ticks()
    for (i in range64(nrows)) {
        embed_row(t, tokens[i], unsafe(addr(ws.scr.x_b[i * dim])))
    }
    if (c.embed_scale != 1.0) {  // Gemma2: scale the embedding by sqrt(dim)
        scale_inplace(unsafe(addr(ws.scr.x_b[0])), c.embed_scale, nrows * dim)
    }
    prof_add("embed", ts_embed)
    if (has_ple(c)) {  // gemma-4 E-series: per-(row, layer) side input (token-indexed, position-free)
        let ts_ple = ref_time_ticks()
        ple_pre_prefill(t, ws.scr, tokens, nrows)
        prof_add("ple", ts_ple)
    }
    // per-row RoPE cos/sin table(s) — row i at sessions[i]'s own position
    let ts_rope_build = ref_time_ticks()
    build_rope_table_rows(ws.scr.rope_cos, ws.scr.rope_sin, t, c.rope_freq_base, c.rope_freq_scale,
        ws.positions, nrows, c.rope_dim > 0l ? c.rope_dim : c.head_size, true)
    if (has_dual_rope(c)) {
        let swa_hs = c.head_size_swa > 0l ? c.head_size_swa : c.head_size
        build_rope_table_rows(ws.scr.rope_cos_swa, ws.scr.rope_sin_swa, t, c.rope_freq_base_swa,
            c.rope_freq_scale_swa, ws.positions, nrows, swa_hs, !c.rope_ff_global_only)
    }
    prof_add("rope_build", ts_rope_build)
    // layer stack: batched attention + the arch's batched FFN. An active batch-decode override may
    // claim the whole stack (false = declined, fall back to the per-layer loop below).
    var stack_done = false
    g_batch_decode_logits_done = false
    if (!empty(g_batch_decode_override_name)) {
        stack_done = invoke(g_batch_decode_override, t, ws, sessions, nrows)
    }
    if (!stack_done) {
        for (l in range64(c.n_layers)) {
            attention_batch_decode(t, ws, sessions, l, nrows)
            t.blocks.ffn_prefill(t, ws.scr, l, nrows)
        }
    }
    // final rmsnorm + classifier over ALL rows, then per-row softcap/pins scattered per session
    let ts_final = ref_time_ticks()
    let vocab = c.vocab_size
    if (!(stack_done && g_batch_decode_logits_done)) {
        rms_batch(ws.scr.xb_b, ws.scr.x_b, t, t.rms_final_off, dim, nrows)
        ws.logits_b |> resize(nrows * vocab)
        let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
        if (c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8) {
            // tied weights (fp32/q4_0 loads): the classifier IS the fp32 embedding table
            mm_fblob_b(ws.logits_b, t, t.tok_emb_off, ws.scr.xb_b, dim, vocab, nrows)
        } elif (cls_fmt != KqFmt.q8) {
            mm_b_kq(ws.scr, ws.logits_b, t, cls_fmt, t.wcls_off, ws.scr.xb_b, dim, vocab, nrows)
        } else {
            mm_b(ws.logits_b, t, t.wcls_off, ws.scr.xb_b, ws.scr.xqb, ws.scr.xsb, dim, vocab, nrows)
        }
        // per-row softcap + scatter into each session's logits + suppress pins, ONE row-parallel
        // dispatch — the serial tail here was most of a ~6ms all-lane hole every step at B=8
        // (B x vocab ≈ 5MB of copy). Rows are disjoint (row i writes only sessions[i].logits).
        ws.lrow |> resize(int(nrows))
        unsafe {
            for (i in range64(nrows)) {
                ws.lrow[i] = addr(sessions[i].logits[0])
            }
            var lbp = addr(ws.logits_b[0])
            var lrp = addr(ws.lrow[0])
            let softcap = c.final_logit_softcap
            maybe_parallel_for(0, int(nrows), min(int(nrows), get_dispatch_lanes())) $(rb, re) {
                unsafe {
                    for (i in range(rb, re)) {
                        var src = lbp + int64(i) * vocab
                        if (softcap > 0.0) {  // Gemma2: soft-cap the output logits per row
                            softcap4(src, vocab, softcap)
                        }
                        copy_floats(lrp[i], src, vocab)
                    }
                }
            }
        }
    }
    for (i in range64(nrows)) {
        var sp = sessions[i]
        for (id in t.suppress) {  // gemma4: pin the checkpoint's suppressed token ids to -inf
            sp.logits[id] = -1.0e30
        }
        sp.n_past++
    }
    prof_add("final", ts_final)
}

//! Greedy argmax over the current logits; returns the winning token id.
def private argmax(s : Session; vocab : int64) : int64 {
    // team-scanned (bit-identical to serial first-wins): 151-262k vocab scan was 210-284us/token idle
    return parallel_argmax(s.logits, vocab)
}

//! Greedy decode. While still inside `prompt_tokens`, the next token is forced from the prompt;
//! otherwise it's argmax. Returns emitted IDs (prompt echo minus BOS, then continuation; stops on BOS).
def generate(t : Model; var s : Session; prompt_tokens : array<int64>; steps : int) : array<int64> {
    var ids : array<int64>
    let n_prompt = int64(length(prompt_tokens))
    let nsteps = int64(steps)
    if (nsteps <= 0l || n_prompt <= 0l) {
        return <- ids
    }
    ids |> reserve(nsteps)
    // prefill the whole prompt in one batched pass (cache 0..n_prompt-1; logits at the last position)
    forward_batch(t, s, prompt_tokens, n_prompt, 0l)
    // echo prompt[1..n_prompt-1] (the forced "next" tokens while consuming the prompt), honoring
    // the step budget and the BOS delimiter
    for (q in range64(n_prompt - 1l)) {
        if (int64(length(ids)) >= nsteps) {
            return <- ids
        }
        let tok = prompt_tokens[q + 1l]
        if (tok == 1l) {
            return <- ids
        }
        ids |> push(tok)
    }
    // greedy continuation: sample from the last prompt position's logits, then decode one token
    // per step at the next absolute position
    if (int64(length(ids)) >= nsteps) {
        return <- ids
    }
    var token = argmax(s, t.config.vocab_size)
    var apos = n_prompt
    var one : array<int64>
    one |> resize(1)
    while (int64(length(ids)) < nsteps) {
        if (token == 1l) {  // BOS delimits sequences
            break
        }
        ids |> push(token)
        if (int64(length(ids)) >= nsteps) {
            break
        }
        one[0] = token
        forward_batch(t, s, one, 1l, apos)
        apos ++
        token = argmax(s, t.config.vocab_size)
    }
    return <- ids
}

// ===== Sampling =====

//! Sampling parameters. Defaults are greedy (temp = 0, all cutoffs/penalties off) — bit-identical
//! to argmax. Cutoff order matches llama.cpp: top-k on logits, then top-p and min-p on the
//! softmaxed probabilities.
struct SamplingParams {
    temp : float = 0.0             // <= 0 => greedy argmax; otherwise softmax temperature
    top_k : int64 = 0l             // <= 0 or >= vocab => no top-k cutoff
    top_p : float = 1.0            // nucleus: keep the smallest prob mass >= top_p (>= 1 = off)
    min_p : float = 0.0            // drop tokens with prob < min_p * max prob (<= 0 = off)
    penalty : float = 1.0          // multiplicative repetition penalty over recent tokens (1 = none; <= 0 treated as none)
    presence_penalty : float = 0.0   // subtracted from the logit of every distinct token in the window
    frequency_penalty : float = 0.0  // subtracted per occurrence in the window (OpenAI semantics)
    penalty_last_n : int64 = 64l   // penalty window (most recent N generated tokens)
}

//! Seed the session's sampling RNG for reproducible generation.
def set_seed_(var s : Session; seed : int) {
    s.rng = random_seed(seed)
}

// Repetition/presence/frequency penalties over the last `penalty_last_n` of `s.recent` (llama.cpp
// semantics): each unique token penalized once, multiplicative then presence+frequency*count.
def private apply_penalties_(var s : Session; params : SamplingParams) {
    // <= 0 treated as off too (not just 1): 0 would inf/NaN the logits, and additive-penalty clients send it
    let rep = params.penalty > 0.0 && params.penalty != 1.0
    if (!rep && params.presence_penalty == 0.0 && params.frequency_penalty == 0.0) {
        return
    }
    let n = length(s.recent)
    let win = int(params.penalty_last_n)
    let pstart = n > win ? n - win : 0
    for (k in range(pstart, n)) {
        let id = s.recent[k]
        var seen = false
        for (j in range(pstart, k)) {
            if (s.recent[j] == id) {
                seen = true
                break
            }
        }
        continue if (seen)
        var cnt = 1
        for (j in range(k + 1, n)) {
            if (s.recent[j] == id) {
                cnt++
            }
        }
        var v = s.logits[id]
        if (rep) {
            v = v > 0.0 ? v / params.penalty : v * params.penalty
        }
        v -= params.presence_penalty + params.frequency_penalty * float(cnt)
        s.logits[id] = v
    }
}

//! Sample the next token from `s.logits` per `params`: penalties, then temp + top-k, softmax,
//! top-p/min-p, and a CDF draw (or greedy argmax when temp <= 0). Consumes `s.logits` in place.
def sample_(var s : Session; params : SamplingParams) : int64 {
    let vocab = int64(length(s.logits))
    apply_penalties_(s, params)
    if (params.temp <= 0.0) {
        return argmax(s, vocab)
    }
    let inv = 1.0 / params.temp
    for (i in range64(vocab)) {
        s.logits[i] *= inv
    }
    if (params.top_k > 0l && params.top_k < vocab) {
        for (i in range64(vocab)) {
            s.sample_scratch[i] = s.logits[i]
        }
        var kth = -1.0e30
        for (_j in range64(params.top_k)) {
            var mx = -1.0e30
            var mi = 0l
            for (i in range64(vocab)) {
                if (s.sample_scratch[i] > mx) {
                    mx = s.sample_scratch[i]
                    mi = i
                }
            }
            kth = mx
            s.sample_scratch[mi] = -1.0e30
        }
        for (i in range64(vocab)) {
            if (s.logits[i] < kth) {
                s.logits[i] = -1.0e30
            }
        }
    }
    softmax(s.logits, vocab)
    if (params.top_p < 1.0) {
        // nucleus cut: sort a prob copy descending-by-walk, find the smallest prefix whose mass
        // reaches top_p, zero everything below that prefix's boundary probability
        for (i in range64(vocab)) {
            s.sample_scratch[i] = s.logits[i]
        }
        sort(s.sample_scratch)
        let target = max(params.top_p, 0.0)
        var mass = 0.0
        var cut = 0.0
        var idx = length(s.sample_scratch) - 1
        while (idx >= 0) {
            cut = s.sample_scratch[idx]
            mass += cut
            break if (mass >= target)
            idx--
        }
        for (i in range64(vocab)) {
            if (s.logits[i] < cut) {
                s.logits[i] = 0.0
            }
        }
    }
    if (params.min_p > 0.0) {
        var maxp = 0.0
        for (i in range64(vocab)) {
            maxp = max(maxp, s.logits[i])
        }
        let thr = min(params.min_p, 1.0) * maxp
        for (i in range64(vocab)) {
            if (s.logits[i] < thr) {
                s.logits[i] = 0.0
            }
        }
    }
    // the cuts above zero probabilities without renormalizing — draw against the surviving mass
    // (cdf accumulates in the same order as total, so the walk always lands on a surviving token)
    var total = 0.0
    for (i in range64(vocab)) {
        total += s.logits[i]
    }
    let r = random_float(s.rng) * total
    var cdf = 0.0
    for (i in range64(vocab)) {
        cdf += s.logits[i]
        if (r < cdf) {
            return i
        }
    }
    return vocab - 1l
}

//! Stream-generate up to `max_tokens` from `prompt`, invoking `blk(id, piece)` per token; return
//! false from the block to stop early. Returns the number of tokens emitted.
def generate_(t : Model; var s : Session; prompt : array<int64>; params : SamplingParams;
              max_tokens : int64; blk : block<(id : int64; piece : string) : bool>) : int64 {
    s.recent |> clear()
    let t_pre = ref_time_ticks()
    eval_(t, s, prompt)
    s.stat_prefill_us = int64(get_time_usec(t_pre))
    s.stat_n_prefill = int64(length(prompt))
    var one : array<int64>
    one |> resize(1)
    var nout = 0l
    var got_first = false
    let t_gen = ref_time_ticks()
    for (_step in range64(max_tokens)) {
        let tok = sample_(s, params)
        if (!got_first) {   // time-to-first-token spans the prefill + the first forward
            s.stat_ttft_us = int64(get_time_usec(t_pre))
            got_first = true
        }
        if (!invoke(blk, tok, piece_(t, tok))) {
            break
        }
        nout++
        s.recent |> push(tok)
        one[0] = tok
        eval_(t, s, one)
    }
    s.stat_gen_us = int64(get_time_usec(t_gen))
    s.stat_n_gen = nout
    return nout
}

//! generate_'s embedding-prefill twin: prefill `npos` pre-built embedding rows (the multimodal
//! splice — see eval_embd_), then stream-sample exactly like generate_. The chat layer's audio
//! turns run through this.
def generate_embd_(t : Model; var s : Session; embd : array<float>; npos : int64; params : SamplingParams;
                   max_tokens : int64; blk : block<(id : int64; piece : string) : bool>) : int64 {
    s.recent |> clear()
    let t_pre = ref_time_ticks()
    eval_embd_(t, s, embd, npos)
    s.stat_prefill_us = int64(get_time_usec(t_pre))
    s.stat_n_prefill = npos
    var one : array<int64>
    one |> resize(1)
    var nout = 0l
    var got_first = false
    let t_gen = ref_time_ticks()
    for (_step in range64(max_tokens)) {
        let tok = sample_(s, params)
        if (!got_first) {   // time-to-first-token spans the prefill + the first forward
            s.stat_ttft_us = int64(get_time_usec(t_pre))
            got_first = true
        }
        if (!invoke(blk, tok, piece_(t, tok))) {
            break
        }
        nout++
        s.recent |> push(tok)
        one[0] = tok
        eval_(t, s, one)
    }
    s.stat_gen_us = int64(get_time_usec(t_gen))
    s.stat_n_gen = nout
    return nout
}

// ===== Generation stats =====

//! A summary of the most recent generate()/respond() call's timing. Times are seconds; rates tok/s.
struct Stats {
    n_prompt : int64 = 0l        // prompt tokens prefilled
    n_gen : int64 = 0l           // tokens generated
    ttft_s : double = 0.0lf      // time to first token (prefill + first forward)
    prefill_tps : double = 0.0lf // prompt tokens / prefill seconds
    gen_tps : double = 0.0lf     // generated tokens / generation seconds
}

//! Summarize the last generate()/respond() call from the session's timing counters.
def stats_(s : Session) : Stats {
    var r = Stats(n_prompt = s.stat_n_prefill, n_gen = s.stat_n_gen,
                  ttft_s = double(s.stat_ttft_us) / 1.0e6lf)
    if (s.stat_prefill_us > 0l) {
        r.prefill_tps = double(s.stat_n_prefill) * 1.0e6lf / double(s.stat_prefill_us)
    }
    if (s.stat_gen_us > 0l) {
        r.gen_tps = double(s.stat_n_gen) * 1.0e6lf / double(s.stat_gen_us)
    }
    return r
}

// ===== Shared arch-registration surface =====
// Chat-template part builders + the default dense block set; each arch self-registers at [init].

//! Chat-template part builders (for arch registration + the chat engine). `chat_text` is literal
//! text; `chat_special` is a special-token spelling the renderer resolves to an id; `chat_content`
//! is the slot the message text fills.
def chat_text(s : string) : ChatPart {
    return ChatPart(kind = ChatPartKind.text, s = s)
}
def chat_special(s : string) : ChatPart {
    return ChatPart(kind = ChatPartKind.special, s = s)
}
def chat_content() : ChatPart {
    return ChatPart(kind = ChatPartKind.content, s = "")
}

//! Install the Hermes-style tool-calling format (the Qwen2.5/Qwen3 chat template's tool block):
//! ``<tools>`` JSON definitions in the system turn, ``<tool_call>`` JSON calls in assistant
//! output, ``<tool_response>`` results riding user turns.
def hermes_tools(var ct : ChatTemplate) {
    ct.tool_sys_prelude = "# Tools\n\nYou may call one or more functions to assist with the user query.\n\nYou are provided with function signatures within <tools></tools> XML tags:\n<tools>"
    ct.tool_sys_epilogue = "</tools>\n\nFor each function call, return a json object with function name and arguments within <tool_call></tool_call> XML tags:\n<tool_call>\n\{\"name\": <function-name>, \"arguments\": <args-json-object>}\n</tool_call>"
    ct.tool_call_open = "<tool_call>"
    ct.tool_call_close = "</tool_call>"
    ct.tool_resp_open = "<tool_response>"
    ct.tool_resp_close = "</tool_response>"
}

//! The default llama-family blocks (std attention, dense gated FFN), shared by every dense arch.
//! Arch files pass this to their ArchDesc unless they swap block dataflow (e.g. MoE).
def std_blocks() : ArchBlocks {
    return ArchBlocks(attn_decode = @@attention_std_decode, ffn_decode = @@ffn_dense_decode,
                      attn_prefill = @@attention_std_prefill, ffn_prefill = @@ffn_dense_prefill)
}

//! The MoE block set: standard attention + the routed-experts FFN (decode and naive-per-token
//! prefill). Arches whose configure sets `moe` register these instead of std_blocks.
def moe_blocks() : ArchBlocks {
    return ArchBlocks(attn_decode = @@attention_std_decode, ffn_decode = @@ffn_moe_decode,
                      attn_prefill = @@attention_std_prefill, ffn_prefill = @@ffn_moe_prefill)
}

//! gemma4's block set: standard attention + a FFN that dispatches per file — dense (12B/31B) or the
//! parallel dense-shared + routed MoE (26B-A4B) by expert count, so ONE gemma4 registration covers both.
def gemma4_blocks() : ArchBlocks {
    return ArchBlocks(attn_decode = @@attention_std_decode, ffn_decode = @@ffn_gemma4_decode,
                      attn_prefill = @@attention_std_prefill, ffn_prefill = @@ffn_gemma4_prefill)
}

//! qwen35's block set: the hybrid Gated-DeltaNet / gated-attention dispatch + the dense FFN.
//! attn_is_std = false — batched decode falls back to per-row forward() (recurrent state is
//! per-session and strictly sequential).
def qwen35_blocks() : ArchBlocks {
    return ArchBlocks(attn_decode = @@attention_qwen35_decode, ffn_decode = @@ffn_dense_decode,
                      attn_prefill = @@attention_qwen35_prefill, ffn_prefill = @@ffn_dense_prefill,
                      attn_is_std = false)
}

//! qwen35moe's block set: the same hybrid attention dispatch + the routed-experts FFN
//! (softmax top-k with renorm + the qwen2moe-style sigmoid-gated shared expert).
def qwen35moe_blocks() : ArchBlocks {
    return ArchBlocks(attn_decode = @@attention_qwen35_decode, ffn_decode = @@ffn_moe_decode,
                      attn_prefill = @@attention_qwen35_prefill, ffn_prefill = @@ffn_moe_prefill,
                      attn_is_std = false)
}
