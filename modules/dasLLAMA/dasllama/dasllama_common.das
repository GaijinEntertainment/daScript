options gen2
options _comment_hygiene = true

module dasllama_common shared public

require daslib/fio
require dasllama/dasllama_env  // the shared typed env readers + the knob registry
require dasllama/dasllama_config  // DlimConfiguration — this module registers the cpu/metal sources
require dasllama/dasllama_kqformat public   // KqFmt + kq_sb + the stride truth — re-exported so every requirer keeps seeing the enum
require dasllama/dasllama_rope       // the RoPE angle/table generation (Model-facing wrappers below pass config params in)
require dasllama/dasllama_gpu_tier   // the device-cooperation seam (hooks the loader/dispatch arms call)
require daslib/array_boost // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs and false-flags this as unused
require dasllama/dasllama_math
require dasllama/dasllama_kv_codec  // the KV-cache codec families — kv_store_row/kv_load_row/kv_dot/kv_axpy dispatch into them
require dasllama/dasllama_convert   // cvt_f32_to_f16/cvt_f16_to_f32 — the f16 rows (wscale plane + the f16 KV codec)
require dasllama/dasllama_math_default  // registers the portable Q8·Q8 backend at [init] (the fallback everywhere)
require dasllama/dasllama_math_aarch64_neon  // registers the arm64 SDOT Q8·Q8 backend at [init]; no-op off-ARM
require ?llvm/daslib/llvm_tune dasllama/dasllama_math_gen  // nolint:STYLE030 — [init]-only: registers "arm64-gen" (the generated GEMM family, arm64 load-select tier). PATH guard: the module hard-requires llvm_tune, so it loads exactly when dasLLVM is mounted
require ?llvm/daslib/llvm_tune llvm/daslib/llvm_tune public  // re-export ONLY llvm_tune (not math_gen's whole public surface — its jobque_boost would flood requirers with ambiguities): the auto-policy pass and tune_status reach every app that requires this module
require ?das_metal dasllama/dasllama_metal_gemm  // nolint:STYLE030 — [init]-only: registers "metal" (the GPU prefill-GEMM batch donor). STRICT guard: the C++ das_metal module exists on Apple builds only, so non-Apple lanes never compile the Metal driver
require ?das_accelerate dasllama/dasllama_math_accelerate  // nolint:STYLE030 — [init]-only: arms the "+AMX" float-batch override when DASLLAMA_ACCEL=1. Same Apple-only guard shape as the metal driver
require ?vulkan dasllama/dasllama_math_vulkan  // nolint:STYLE030 — [init]-only: registers "vulkan" (the GPU tier; stage-0 skeleton, kernels land with the offload rail). STRICT guard: the C++ vulkan module is linked only where the dasVulkan package is installed, so CI lanes without it never compile the Vulkan driver
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

//! The GPU-PLE pre-step gate: a prefill override that can ALSO build the E-series PLE side input
//! on its device registers a probe here. On true, forward_prefill stashes the token rows and skips
//! the CPU gather+finish; the override consumes the stash (ple_pre_prefill = late-decline fallback).
typedef PleGpuGateFn = function<(t : Model; s : Session; npos : int64; start_pos : int64) : bool>
var private g_ple_gpu_gate : PleGpuGateFn
var private g_ple_gpu_gate_set = false

def register_ple_gpu_gate(fn : PleGpuGateFn) {
    g_ple_gpu_gate = fn
    g_ple_gpu_gate_set = true
}

// ===== gemma-4 PLE pre-step hooks (registered by dasllama_ple at [init]) =====
// PLE needs Model/Session, so it requires this module back and cannot be required from here — the
// forward path reaches its per-pass pre-step through these hooks, and the umbrella breaks the cycle.
typedef PlePreDecodeFn = function<(t : Model; var s : Session; token : int64) : void>
typedef PlePrePrefillFn = function<(t : Model; var s : Session; tokens : array<int64>; npos : int64) : void>
typedef PlePrePrefillPadFn = function<(t : Model; var s : Session; npos : int64) : void>

var private g_ple_pre_decode : PlePreDecodeFn
var private g_ple_pre_prefill : PlePrePrefillFn
var private g_ple_pre_prefill_pad : PlePrePrefillPadFn

def register_ple_pre_decode(fn : PlePreDecodeFn) {
    g_ple_pre_decode = fn
}
def register_ple_pre_prefill(fn : PlePrePrefillFn) {
    g_ple_pre_prefill = fn
}
def register_ple_pre_prefill_pad(fn : PlePrePrefillPadFn) {
    g_ple_pre_prefill_pad = fn
}

// Every call site is behind has_ple(config), so an unset rail means this program was built without
// dasllama/dasllama_ple yet loaded a model that needs it — say which require is missing, loudly.
def private ple_rail_missing() {
    panic("dasLLAMA: this model needs the PLE rail — the program must require dasllama/dasllama_ple")
}

def private ple_pre_decode(t : Model; var s : Session; token : int64) {
    if (g_ple_pre_decode == null) ple_rail_missing()
    invoke(g_ple_pre_decode, t, s, token)
}

def ple_pre_prefill(t : Model; var s : Session; tokens : array<int64>; npos : int64) {
    if (g_ple_pre_prefill == null) ple_rail_missing()
    invoke(g_ple_pre_prefill, t, s, tokens, npos)
}

def ple_pre_prefill_pad(t : Model; var s : Session; npos : int64) {
    if (g_ple_pre_prefill_pad == null) ple_rail_missing()
    invoke(g_ple_pre_prefill_pad, t, s, npos)
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

// The MTP spec-step override: a backend's complete mtp_spec_eval twin (cold gates + accept/
// reject bookkeeping inside). Keyed by the DECODE override name — selecting "metal" activates
// both; consulted only for blob models (planar models keep the CPU spec rail).
typedef MtpSpecOverrideFn = function<(t : Model; var s : Session; tok : int64; var accepted : int64&) : bool>
var private g_mtp_spec_overrides : table<string; MtpSpecOverrideFn>

def register_mtp_spec_override(name : string; fn : MtpSpecOverrideFn) {
    g_mtp_spec_overrides[name] = fn
}

// The MTP seam-row override: writes the draft head's row pos-1 on a blob model BEFORE a
// continuation window's trunk body (forward_mtp(..., false)'s GPU twin) — the body's own
// tail overwrites mtp_h, so the seam must consume the pre-window handoff first
typedef MtpSeamOverrideFn = function<(t : Model; var s : Session; token, pos : int64) : bool>
var private g_mtp_seam_overrides : table<string; MtpSeamOverrideFn>

def register_mtp_seam_override(name : string; fn : MtpSeamOverrideFn) {
    g_mtp_seam_overrides[name] = fn
}

// "would the metal drivers serve this loaded model?" — the model-level gate, registered by the
// metal driver module at [init] (it owns the path masks). load_model consults it to pick the
// image flavor: servable -> the blob-only metal flavor, else the planar CPU flavor.
typedef MetalServableFn = function<(t : Model) : bool>

var private g_metal_servable : MetalServableFn
var private g_metal_servable_set = false

def register_metal_servable(fn : MetalServableFn) {
    g_metal_servable = fn
    g_metal_servable_set = true
}

def metal_model_servable(t : Model) : bool {
    return g_metal_servable_set && invoke(g_metal_servable, t)
}

// ===== disk->compute layout hooks (registered by dasllama_layout at [init]) =====

// The layout transforms live in dasllama_layout (which requires this module back for Model/KqFmt);
// the loader reaches them through these hooks. Unset hooks PANIC at first use — a program that
// loads models must require dasllama/dasllama_layout (umbrella + transformer facade both do).

typedef ModelRepackFn = function<(var t : Model) : void>
typedef GatherStackFn = function<(t : Model; woff : int64; n, rows, slice_rows : int64;
                                  mr, kgroup, wbias : int64;
                                  var wq : array<uint8>; var ws : array<uint8>) : void>
typedef GatherStackKqFn = function<(t : Model; fmt : KqFmt; woff : int64; n, rows, slice_rows : int64;
                                    repacked : bool; mr : int64;
                                    var wq : array<uint8>; var ws : array<uint8>) : void>
typedef GatherStackQ8nFn = function<(t : Model; woff : int64; n, rows, slice_rows : int64;
                                     mr, kgroup, wbias : int64; var wq : array<uint8>) : void>

var private g_layout_repack_q8 : ModelRepackFn
var private g_layout_repack_kq : ModelRepackFn
var private g_layout_repack_mx4 : ModelRepackFn
var private g_layout_repack_q51 : ModelRepackFn
var private g_layout_gather : GatherStackFn
var private g_layout_gather_kq : GatherStackKqFn
var private g_layout_gather_q8n : GatherStackQ8nFn
var private g_layout_hooks_set = false

def register_model_layout(rq8, rkq, rmx4, rq51 : ModelRepackFn; gs : GatherStackFn; gskq : GatherStackKqFn;
                          gsq8n : GatherStackQ8nFn) {
    g_layout_repack_q8 = rq8
    g_layout_repack_kq = rkq
    g_layout_repack_mx4 = rmx4
    g_layout_repack_q51 = rq51
    g_layout_gather = gs
    g_layout_gather_kq = gskq
    g_layout_gather_q8n = gsq8n
    g_layout_hooks_set = true
}

def private layout_hooks_check {
    if (!g_layout_hooks_set) {
        panic("dasLLAMA: layout hooks unset — the program must require dasllama/dasllama_layout")
    }
}

def layout_repack_q8(var t : Model) {
    layout_hooks_check()
    invoke(g_layout_repack_q8, t)
}

def layout_repack_kq(var t : Model) {
    layout_hooks_check()
    invoke(g_layout_repack_kq, t)
}

def layout_repack_mx4(var t : Model) {
    layout_hooks_check()
    invoke(g_layout_repack_mx4, t)
}

def layout_repack_q51(var t : Model) {
    layout_hooks_check()
    invoke(g_layout_repack_q51, t)
}

// The GPU walk's gather half of the same hook contract. The residency rail intercepts each gather
// for the flavor bake before calling through, so these stay thin and the hook globals stay private.

def layout_gather_stack(t : Model; woff : int64; n, rows, slice_rows : int64; mr, kgroup, wbias : int64;
                        var wq : array<uint8>; var ws : array<uint8>) {
    layout_hooks_check()
    invoke(g_layout_gather, t, woff, n, rows, slice_rows, mr, kgroup, wbias, wq, ws)
}

def layout_gather_stack_kq(t : Model; fmt : KqFmt; woff : int64; n, rows, slice_rows : int64;
                           repacked : bool; mr : int64; var wq : array<uint8>; var ws : array<uint8>) {
    layout_hooks_check()
    invoke(g_layout_gather_kq, t, fmt, woff, n, rows, slice_rows, repacked, mr, wq, ws)
}

def layout_gather_stack_q8n(t : Model; woff : int64; n, rows, slice_rows : int64;
                            mr, kgroup, wbias : int64; var wq : array<uint8>) {
    layout_hooks_check()
    invoke(g_layout_gather_q8n, t, woff, n, rows, slice_rows, mr, kgroup, wbias, wq)
}

// ===== whole-model GPU residency hook (registered by dasllama_gpu_resident at [init]) =====

// The residency rail requires this module back for Model, so the loader reaches its upload walk
// through this slot. Unset is NORMAL and means no-op: a program that never requires the rail has
// no GPU residency to arm, unlike the layout hooks, which are mandatory and panic.

typedef GpuUploadResidentFn = function<(t : Model) : void>

var private g_gpu_upload_resident : GpuUploadResidentFn

def register_gpu_upload_resident(fn : GpuUploadResidentFn) {
    g_gpu_upload_resident = fn
}

def gpu_upload_resident(t : Model) {
    if (g_gpu_upload_resident == null) {
        // A tier CAN arm without the rail — this module requires ?vulkan itself, the rail only from
        // the transformer umbrella. Silently skipping the upload there would leave a model the tier
        // believes it serves with nothing on the device, so say so instead.
        if (moe_gpu_tier_installed()) {
            panic("dasLLAMA: a GPU tier is installed but no residency rail registered — the program must require dasllama/dasllama_gpu_resident")
        }
        return
    }
    invoke(g_gpu_upload_resident, t)
}

// ===== the GPU-flavor bake plan record (Model.vkplan's element; built by dasllama_gpu_resident) =====

//! One baked gather of the vulkan flavor's plan — walk order, offsets into Model.vkblob.
//! `role` records WHICH walk stage produced it (VkBakeRole) — the trim pass (P3) and the
//! slice verifier both key on it.
struct VkPlanEntry {
    fmt : int
    role : int
    woff : int64
    n : int64
    rows : int64
    wq_off : int64
    wq_bytes : int64
    ws_off : int64
    ws_bytes : int64
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

//! The armed batch-decode override, or none — eval_batch_ (dasllama_batch) dispatches through these
//! two rather than the registry going public.
def batch_decode_override_armed() : bool => !empty(g_batch_decode_override_name)

def batch_decode_override_invoke(t : Model; var ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool {
    return invoke(g_batch_decode_override, t, ws, sessions, nrows)
}

def active_decode_override : string {
    return g_decode_override_name
}

//! Is a whole-prefill/decode override registered under `name`? (Apple builds register "metal"
//! at [init]; other platforms have none.)
def has_prefill_override(name : string) : bool {
    return key_exists(g_prefill_overrides, name)
}

// CPU batched prefill above this length on a Metal-capable build is a measurement hazard, not a
// serving path — days of profiling time went to tools silently CPU-prefilling. forward_prefill_body
// PANICS there unless the caller declares CPU intent (the API below or DASLLAMA_CPU_PREFILL=1).
let CPU_PREFILL_PANIC_NPOS = 32l

var private g_cpu_prefill_allowed = -1   // -1 = read DASLLAMA_CPU_PREFILL on first check

//! Declare that this process runs CPU prefill ON PURPOSE — disarms the Metal-capable-build panic
//! on batched prefills longer than ``CPU_PREFILL_PANIC_NPOS`` tokens. ``DASLLAMA_CPU_PREFILL=1``
//! is the no-recompile equivalent (the test rail exports it).
def allow_cpu_prefill(on : bool = true) {
    g_cpu_prefill_allowed = on ? 1 : 0
}

[cold_path]   // one-time env resolution; the accessor is a global test after the first call
def private load_cpu_prefill_allowed() {
    g_cpu_prefill_allowed = env_flag("DASLLAMA_CPU_PREFILL", false) ? 1 : 0
}

def private cpu_prefill_allowed : bool {
    if (g_cpu_prefill_allowed < 0) {
        load_cpu_prefill_allowed()
    }
    return g_cpu_prefill_allowed == 1
}

// The blob-only tripwire: a metal-flavor (blob-only) model carries no planar CPU planes — any
// CPU inference against it is a wiring bug, never a fallback. (The gather paths — embed_row,
// PLE — read the blob directly and stay legal.)
def blob_only_panic(t : Model; what : string) {
    if (t.metal_blob) {
        panic("dasLLAMA: CPU {what} on a blob-only metal-flavor model — serve it on the GPU (set_metal_mode / fix the declined gate) or load the CPU-flavor image (metal mode off at load_model)")
    }
    if (t.planes_trimmed) {
        // the resident overrides need an f32-KV session — the commonest decline on a trimmed image
        panic("dasLLAMA: CPU {what} on a planes-trimmed vulkan-flavor image — the resident driver must serve it (create the session with KVDtype.f32, keep the tier armed) or rebake without --trim")
    }
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

//! eval_batch_ arms the flag before dispatching and reads it after — the driver lives in
//! dasllama_batch, the registry here, so the flag is reached through these two instead of going public.
def batch_decode_logits_reset {
    g_batch_decode_logits_done = false
}

def batch_decode_logits_done() : bool => g_batch_decode_logits_done

//! Build one position's RoPE cos/sin row into caller arrays — bit-identical to the row forward()
//! hands the decode override (an encode-ahead driver pre-builds the NEXT position's row; the
//! global-rope class only — dual-rope configs pre-build the swa row with the twin below).
def public build_decode_rope_row(t : Model; pos : int64; var cos_tab, sin_tab : array<float>) {
    let c = t.config
    build_rope_table(cos_tab, sin_tab, t, c.rope_freq_base, c.rope_freq_scale, pos, 1l,
                     c.rope_dim > 0l ? c.rope_dim : c.head_size, true)
}

//! The sliding class's row (dual-rope configs) — bit-identical to forward()'s swa row build.
def public build_decode_rope_row_swa(t : Model; pos : int64; var cos_tab, sin_tab : array<float>) {
    let c = t.config
    let swa_hs = c.head_size_swa > 0l ? c.head_size_swa : c.head_size
    build_rope_table(cos_tab, sin_tab, t, c.rope_freq_base_swa, c.rope_freq_scale_swa, pos, 1l,
                     swa_hs, !c.rope_ff_global_only)
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
    // Metal gate: the FFN is the plain dense gated pair (ffn_dense_*). Block sets that route
    // experts (or MAY, per file — gemma4) set this false and the GPU drivers decline `graph`.
    ffn_is_dense : bool = true
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
    // Metal-flavor blob rail (blob-only .dlim): q8 weights as 34-byte block_q8_0 blocks populated
    // INSTEAD of qblob + qscales/qscales16 (blob byte = (off/32)*34); k4s/k5s hold 16B compact
    // scale strips, k6s the GPU split form. CPU inference on a blob-only model PANICS.
    mblob : array<uint8>
    metal_blob : bool = false
    // P3-trimmed image: the big CPU weight families are EMPTY (GPU-resident consumers only);
    // embed reads embq/embs, re-gathers must slice from vkblob, CPU fallbacks fail closed
    planes_trimmed : bool = false
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
    // the q51 tier's plane pair, per 32-BLOCK: 20B nibbles+qh / 4B f16 d+m (never repacked)
    q51q : array<uint8>
    q51s : array<uint8>
    kquant_native : bool = false
    // kq stage 4: the kq planes were repacked at load into the active backend's grp<mr> layout.
    // From then on every kq matmul MUST run the backend slots — disk-order portable kernels would
    // read garbage — and embed_row gathers rows through the grp layout too.
    kq_repacked : bool = false
    mx4_repacked : bool = false   // the CPU repack pass ran (set by repack_q8_weights; mx4 stacks laneq when present — no GPU carry)
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
    // deltanet projection tags (recurrent layers; empty = all q8). CPU rail only: detect keeps
    // these q8 whenever Metal or the GPU dn/dense tier could claim the planes (both bind dn q8)
    dnqkv_fmt : array<KqFmt>
    dngate_fmt : array<KqFmt>
    dnout_fmt : array<KqFmt>
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
    // vulkan-flavor bake (dasllama_image): the GPU walk's gathered device-layout planes + the
    // walk-order plan. Empty on planar/metal flavors (empty planes never save); borrowed
    // views on a flavor load — the generic finalizer walk covers both.
    vkblob : array<uint8>
    vkplan : array<VkPlanEntry>
    // trimmed-flavor embedding planes (P3): the emb/cls tensor's rows kept CPU-readable after
    // the big weight families drop. q8: mblob-style 34B interleaved blocks (embs empty);
    // kq: the region's quant + scale planes copied verbatim (repack layout preserved, rebased to 0).
    embq : array<uint8>
    embs : array<uint8>
    // prepared-model image backing (dasllama_image): when set, every locked array field above
    // is a borrowed VIEW into this image (zero-copy) and the finalizer below releases it.
    // Both fields are skipped by the image save/load field walk.
    image_map : void?
    image_bytes : uint64
}

// ===== prepared-image backing =====

// A carrier's planes borrow either a mapped .dlim or a chunk this process built. Owned chunks
// live here, keyed by the ALIGNED base image_map holds, so telling the two apart is one lookup
// and no carrier needs a second field: a miss means "mapping".
var private g_image_chunks : table<uint64; array<uint8>>

def private ptr_key(p : void?) : uint64 {
    return unsafe(reinterpret<uint64>(p))
}

//! Allocate an image chunk of `bytes`, its base aligned to `align` — the in-memory twin of
//! ``fmap_open``. Uninitialized by design: a multi-GB image is fully overwritten by the build
//! walk (padding gaps zero-fill explicitly), and pre-touching it would commit every page twice.
def image_chunk_alloc(bytes : uint64; align : uint64) : void? {
    var raw : array<uint8>
    let total = int64(bytes + align)
    raw |> resize_no_init(total)
    if (long_length(raw) != total) {
        delete raw
        return null
    }
    var base : void?
    unsafe {
        let a = reinterpret<uint64>(addr(raw[0]))
        base = reinterpret<void?>((a + align - 1ul) / align * align)
    }
    g_image_chunks[ptr_key(base)] <- raw
    return base
}

//! Release an owned image chunk by base; false = not one of ours, i.e. the caller holds a mapping
//! and unmaps instead. This is how every carrier's finalizer tells the two backings apart.
def image_chunk_free(base : void?) : bool {
    let key = ptr_key(base)
    if (base == null || !key_exists(g_image_chunks, key)) {
        return false
    }
    var raw <- g_image_chunks[key]
    g_image_chunks |> erase(key)
    delete raw
    return true
}

//! Let go of a carrier's image backing, chunk or mapping — the ONE release. Every weight
//! carrier's finalizer routes here; nothing else may call ``fmap_close`` on an image.
def image_backing_release(map : void?; bytes : uint64) {
    if (map == null || image_chunk_free(map)) {
        return
    }
    unsafe {
        fmap_close(map, bytes)
    }
}

// Model's finalizer (every `delete model` lowers to _::finalize and lands here): image-backed
// models hold locked BORROWED views over the image — those must be forgotten (reset without
// freeing) before the backing goes; everything owned deletes as usual.
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
        // imported device memory over this image (P2 mirrors) must release BEFORE the backing does
        moe_gpu_unmap_notify(t.image_map, t.image_bytes)
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

//! Resident weight bytes across every plane family (only the loaded quant's planes are
//! non-empty; a mapped .dlim's borrowed views count their mapped extent).
def model_weights_bytes(t : Model) : int64 {
    // long_length throughout: big-model planes exceed INT_MAX elements (a 35B Q4_K plane is > 2 GB)
    return (long_length(t.fblob) * 4l + long_length(t.wblob) * 4l
        + long_length(t.qblob) + long_length(t.qscales) * 4l + long_length(t.qscales16) * 2l
        + long_length(t.q4blob) + long_length(t.q4scales) * 4l
        + long_length(t.mxq) + long_length(t.mxs)
        + long_length(t.bf16blob) * 2l
        + long_length(t.k4q) + long_length(t.k4s) + long_length(t.k5q) + long_length(t.k5s)
        + long_length(t.k6q) + long_length(t.k6s) + long_length(t.q40q) + long_length(t.q40s)
        + long_length(t.q51q) + long_length(t.q51s)
        + long_length(t.mblob) + long_length(t.vkblob)
        + long_length(t.embq) + long_length(t.embs))
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
//! Tied K-quant classifier: the embedding table lives in a native K-quant plane at wcls_off —
//! no fp32 table, no qblob copy (the cls_q8 analog for kquant_native loads).
def cls_kq(t : Model) : bool => t.kquant_native && t.config.shared_weights && t.emb_fmt != KqFmt.q8

// Format tag for layer l of a per-kind tag array; empty array (non-kq load) = q8.
def fmt_at(a : array<KqFmt>; l : int64) : KqFmt => empty(a) ? KqFmt.q8 : a[l]

// Reserve exact capacity then size — avoids the array allocator's power-of-2 rounding on big arrays.
def reserve_resize(var a : array<float>; n : int64) {
    a |> reserve(n)
    a |> resize(n)
}

def copy_floats(src : array<float> | #; soff : int64; var dst : array<float>; doff, n : int64) {
    for (i in range64(n)) {
        dst[doff + i] = src[soff + i]
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
def resolve_arch(var t : Model; arch : string) {
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
    if (t.config.moe_optional && !t.config.moe) {
        // the file-resolved dense flavor of a moe-optional arch (gemma4 12B/31B/E-series) — mirror
        // the load_gguf patch so the GPU drivers' graph gate serves image loads too. The E-series
        // PLE block is a dense-stack epilogue, expressed as MetalNeed.ple, not a MoE graph.
        t.blocks.ffn_is_dense = true
    }
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

// Two error rails for unsupported model/quant combos at load: API-facing tools (probes,
// harnesses, benchmarks) fail loud — panic with the exact reason. A user-facing server opts
// into resilient loads: log the problem, pick the nearest working rail, keep serving.
var private g_resilient_load = false

//! Resilient load mode: unsupported combos log + degrade to a working mode instead of panicking.
def public set_resilient_load(on : bool) {
    g_resilient_load = on
}
def public get_resilient_load() : bool {
    return g_resilient_load
}

// Native K-quant weight planes: Q4_K/Q5_K/Q6_K tensors stay packed in per-format planes instead of
// requantizing to Q8. Default ON since the kernel tier landed; OFF loads through the q8-requant
// fallback exactly as before — the A/B opt-out.
var private g_kquant_native = true

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
    apply_i64_knob(rt, "kq_q40_native") $(v) { set_kq_q40_native(v != 0l) }
    apply_i64_knob(rt, "kq_q50_native") $(v) { set_kq_q50_native(v != 0l) }
    apply_i64_knob(rt, "noisy") $(v) { set_dasllama_noisy(v != 0l) }
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
    let mtc = read_json_field(rt, "metal_tensor", "")
    if (mtc != "") {
        set_metal_tensor_crowns(mtc)
        to_log(LOG_INFO, "dasLLAMA: box profile runtime: metal_tensor crowns = {mtc}\n")
    }
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

// Q5_0 -> k5 re-encode knob. Unlike the other tags this one is NOT a relabel: Q5_0 has no exact
// Q5_K embedding, so the load re-quantizes (measured +0.031 rel-rmse on GLM's ffn_down) to buy
// 9.0 -> 5.625 bits. OFF keeps the bit-exact Q5_0 -> Q8 path — the A/B rail, and the quality out.
var private g_kq_q50_native = false

//! Enable/disable re-encoding Q5_0 tensors into the k5 plane for subsequent loads. LOSSY by
//! construction (see g_kq_q50_native); default OFF. Requires kquant_native.
def public set_kq_q50_native(on : bool) {
    g_kq_q50_native = on
}
def public get_kq_q50_native() : bool {
    return g_kq_q50_native
}

// The q51 tier's bring-up knob: OFF keeps Q5_1 tensors on the q8-requant fallback (~1.4x expert
// memory, exact) — the zero-risk A/B rail while the tier proves out.
var private g_kq_q51_native = true

//! Enable/disable the native Q5_1 plane tier for subsequent loads (A/B rail; default ON;
//! requires kquant_native). Expert stacks only — dense positions always requant to q8.
def public set_kq_q51_native(on : bool) {
    g_kq_q51_native = on
}
def public get_kq_q51_native() : bool {
    return g_kq_q51_native
}

// ===== DlimConfiguration sources (dasllama_config) =====

def private dlim_cpu_source_impl(var c : DlimCpuConfig) {
    // the one sanctioned backend-select side effect — the identity itself is a pure formatter
    select_matmul_backend_for_load()
    c.backend = dlim_backend_name()   // the cross-box bake's target alias when one is armed
    c.q8_mr = active_q8_layout_mr()
    c.q8_wbias = active_q8_wbias()
    c.q8_kgroup = active_q8_kgroup()
    c.kq_mr4 = active_kq_layout_mr(4)
    c.kq_mr5 = active_kq_layout_mr(5)
    c.kq_mr6 = active_kq_layout_mr(6)
    c.kq_mr40 = active_kq_layout_mr(40)
    c.q51_mr = active_q51_layout_mr()
    // OUTCOME, not request: a backend without s16 twins keeps f32 scale planes (wscale_convert_f16)
    c.wscale_f16 = g_wscale_f16 && kernel_backend_has_wscale16()
    c.kquant_native = g_kquant_native
    c.kq_q40_native = g_kq_q40_native
    c.kq_q50_native = g_kq_q50_native
    c.kq_q51_native = g_kq_q51_native
}

def private dlim_metal_source_impl(var c : DlimMetalConfig) : bool {
    if (get_metal_mode() == MetalMode.off || !has_decode_override("metal")) {
        return false
    }
    c.mode = "{get_metal_mode()}"
    return true
}

[init]
def dlim_config_sources_register() {
    set_dlim_cpu_source(@@dlim_cpu_source_impl)
    set_dlim_metal_source(@@dlim_metal_source_impl)
}

// Dense-position demotion: q40/k5-from-Q5_0 need % 256 rows (superblock walkers); q51 always
// demotes — dense positions have no q51 dispatch (v1 is expert stacks — kq_fmt_expert_ok).
def kq_fmt_row_ok(f : KqFmt; n : int64) : KqFmt {
    if (f == KqFmt.q51) return KqFmt.q8
    return (f == KqFmt.q40 || f == KqFmt.k5) && n % 256l != 0l ? KqFmt.q8 : f
}

// Expert-stack tag rule: q51 serves natively off per-32 planes (its whole reason to exist — the
// 26B-A4B down stacks are 704-wide); the superblock formats keep the % 256 rule.
def kq_fmt_expert_ok(f : KqFmt; n : int64) : KqFmt {
    if (f == KqFmt.q51) return n % 32l == 0l ? f : KqFmt.q8
    return kq_fmt_row_ok(f, n)
}

// Any K-quant-tagged expert stack in layer `l`? (The MoE dispatch and prefill gates branch on this.)
def moe_layer_kq(t : Model; l : int64) : bool {
    return fmt_at(t.we1_fmt, l) != KqFmt.q8 || fmt_at(t.we2_fmt, l) != KqFmt.q8 || fmt_at(t.we3_fmt, l) != KqFmt.q8
}

// Are layer l's deltanet projection planes all q8? (Metal and the GPU tiers bind/upload dn q8 only.)
def dn_layer_q8(t : Model; l : int64) : bool {
    return fmt_at(t.dnqkv_fmt, l) == KqFmt.q8 && fmt_at(t.dngate_fmt, l) == KqFmt.q8 && fmt_at(t.dnout_fmt, l) == KqFmt.q8
}

// Any expert stack on the SUPERBLOCK kq lattice? (q51 excluded — its grouped-prefill kernels are
// always available, so it never gates on kq_repacked / the kq batch tile.)
def moe_layer_sb_kq(t : Model; l : int64) : bool {
    return kq_sb(fmt_at(t.we1_fmt, l)) || kq_sb(fmt_at(t.we2_fmt, l)) || kq_sb(fmt_at(t.we3_fmt, l))
}

// Convert the (already repacked/permuted) f32 scale plane to raw binary16 halfwords and free the
// f32 plane. f16->f32->f16 round-trips exactly for gguf-Q8 tensors (bit-identical dots);
// runtime-quantized tensors take one f16 rounding. Gated on the backend carrying f16-scale kernels.
def wscale_convert_f16(var t : Model) {
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

// The device format of an EXPERT stack: q8 rides the cm2 native-block layout (q8n, fmt 6) when
// the armed tier asks for it; every other format ships as-is. Upload and dispatch must agree on
// the answer — both go through this one helper.
def moe_gpu_expert_fmt(f : KqFmt) : KqFmt => f == KqFmt.q8 && moe_gpu_expert_q8n() ? KqFmt.q8n : f

// the active grp<mr> interleave of a repacked kq plane (1 = disk order / not repacked)
def kq_active_mr(t : Model; f : KqFmt) : int64 {
    if (!t.kq_repacked) {
        return 1l
    }
    return f == KqFmt.k4 ? t.kq_repack_mr4 : (f == KqFmt.k5 ? t.kq_repack_mr5 : (f == KqFmt.k6 ? t.kq_repack_mr6 : t.kq_repack_mr40))
}

//! Load a llama-architecture GGUF into the split layout at the requested precision — straight into
//! `mode`'s storage (pre-quantized tensors transcode block-for-block; a Q8/Q4 load never holds the
//! dense fp32 weights). Norms stay fp32 in fblob; vocab derives from token_embd.
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
    let kq = long_length(t.k4q) + long_length(t.k5q) + long_length(t.k6q) + long_length(t.q40q) + long_length(t.q51q)
    let kqs = long_length(t.k4s) + long_length(t.k5s) + long_length(t.k6s) + long_length(t.q40s) + long_length(t.q51s)
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

//! Allocated KV pool bytes (capacity, not just referenced pages) — the sum of every blob's
//! K and V backing arrays. Empty (0) for flat serving, where KV lives inside each session.
def kv_pool_bytes(pool : KVPool) : int64 {
    var total = 0l
    for (kb in pool.kblobs) {
        total += long_length(kb)   // a grown blob can exceed INT_MAX bytes on big-ctx models
    }
    for (vb in pool.vblobs) {
        total += long_length(vb)
    }
    return total
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
    @scratch att_b : array<float>      // batched prefill attention scores, one (start_pos+npos)-wide row PER HEAD
                              // so the head loop threads with no shared scratch (sized in forward_prefill)
    logits : array<float>     // output logits (vocab_size)
    xq : array<int8>          // activation quantized to Q8 (sized max(dim, hidden_dim)) for the Q8·Q8 kernel
    xs : array<float>         // per-block activation scales (sized max(dim, hidden_dim) / 32)
    // q8_0-K decode query, quantized ONCE per token after RoPE (empty unless kv_dtype_k == q8_0;
    // batched decode resizes to nrows × qd). Dedicated planes: xq/xs hold the LIVE input quants
    // while the fused chain's stage-0 q epilogues fill these
    @scratch attq : array<int8>        // quantized query rows (qd)
    @scratch atts : array<float>       // ... and their per-32-block scales (qd / 32)
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
    @scratch chain_stages : array<int3>  // stage defs, int3(range_begin, range_end, num_chunks)
    // flash-decode attention partials (fused chain, ctx > FLASH_DECODE_SLICE): per (head, slice)
    // running max / exp-sum / unnormalized AV accumulator, combined per head in the chain's
    // combine stage. Sized lazily on first deep-ctx use (n_heads x nsl [x head_size])
    @scratch fl_m : array<float>
    @scratch fl_l : array<float>
    @scratch fl_o : array<float>
    // KV cache — codec-encoded byte blobs (dtype fixed per session at create). Layout is ragged
    // per layer: layer l's slab starts at byte kv_bprefix_*[l] * seq_len. Shared-KV layers alias
    // their source layer's slab (same prefix value).
    kv_dtype_k : KVDtype      // key codec, snapshotted from the module knob in make_run_state
    kv_dtype_v : KVDtype      // value codec (independent of K by design)
    // resident-driver mirror ownership: gen stamps which session's rows the device KV mirror
    // holds (0 = never resident-prefilled); hydrated = host KV has been read back since
    rdec_gen : int64
    rdec_hydrated : bool
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
    @scratch xbsb : array<int>         // batched per-16 activation sums (npos x maxn / 16) — the kq batch tile's offset term
    @scratch kxqb : array<int8>        // batched Q8_K-form quants (kq v2 MoE gather source; lazily sized npos x dim)
    @scratch kxsb : array<float>       // batched Q8_K per-256 scales (npos x dim / 256)
    @scratch kxbsb : array<int>        // batched Q8_K per-16 sums (npos x dim / 16)
    // flash-attention per-head packing scratch (empty unless AttnPrefillMode.flash; sized in
    // forward_prefill to n_heads × tile, so each head's tile work is contention-free when threaded)
    @scratch fa_q : array<float>       // packed Q tile [n_heads × QT × head_size]
    @scratch fa_k : array<float>       // packed Kᵀ tile [n_heads × head_size × KV]
    @scratch fa_v : array<float>       // packed V tile  [n_heads × KV × head_size]
    @scratch fa_kq : array<float>      // score / prob tile [n_heads × QT × KV]
    @scratch fa_vkq : array<float>     // output accumulator [n_heads × QT × head_size]
    @scratch fa_tmp : array<float>     // per-head one-row dequant staging for the Kᵀ pack [n_heads × head_size]
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
    // heat-cache split-FFN scratch (decode: hits ride the GPU chain async, misses stay CPU)
    @scratch moe_hdn : array<float>      // GPU hit-expert down outputs, compacted (nhits x dim)
    @scratch heat_hit : array<int8>      // per routed position: 1 = served by the heat cache
    @scratch heat_o1 : array<int64>      // compacted pool-coordinate offsets for the hit dispatch
    @scratch heat_o3 : array<int64>
    @scratch heat_o2 : array<int64>
    // the layer's rmsnormed activation quantized ONCE and reused across every expert's gate/up
    // matmul — dedicated buffers because the down-projections quantize s.hb into the shared xq/xs
    moe_xq : array<int8>        // xb's Q8 quants (dim)
    moe_xs : array<float>       // xb's per-block scales (dim / 32)
    // K-quant activation block sums for the expert dispatch (the moe twins of xbs/xbs2, filled
    // by the bs quantizers only on layers with kq-tagged expert stacks; sized always — ~KB)
    moe_dbs : array<int>        // per-16 sums of moe_dq (k x n_ff_exp / 16)
    // grouped MoE prefill scratch — sized per prefill by ffn_moe_prefill_grouped; empty for dense
    // arches, decode, and the per-position reference path
    @scratch moe_rlog : array<float>     // batched router logits (npos x n_expert)
    @scratch moe_idx_b : array<int64>    // per-position selected expert ids (npos x k)
    @scratch moe_w_b : array<float>      // per-position selected weights (npos x k)
    @scratch moe_gl_b : array<float>     // per-position shared-expert sigmoid gates (npos; shexp arches only)
    @scratch moe_cnt : array<int64>      // per-expert bucket cursor, run END after the CSR fill (n_expert)
    @scratch moe_bkt : array<int64>      // (position*k + slot) entries grouped by expert (npos x k)
    @scratch moe_inv : array<uint>       // grid -> bucket row map (bkt's inverse; GPU combine arm only)
    @scratch moe_gxq : array<int8>       // one bucket's gathered activation quants (npos x dim)
    @scratch moe_gxs : array<float>      // one bucket's gathered activation scales (npos x dim/32)
    @scratch moe_gxbs : array<int>       // ... and their per-16 sums (npos x dim/16; kq-expert layers only)
    @scratch moe_hbs : array<int>        // bucket hidden requant per-16 sums (npos x nfx/16; kq-expert layers only)
    @scratch moe_ord : array<int2>       // fused-prefill LPT region order (x = expert, y = bucket cnt; capacity ne)
    // fused-prefill region triples (woff, row0, cnt) + per-expert bias offsets, one set per
    // projection — cleared and refilled per layer, capacity persists (no per-layer alloc)
    @scratch moe_offs1 : array<int64>    // gate regions (3 per touched expert)
    @scratch moe_offs3 : array<int64>    // up regions
    @scratch moe_offs2 : array<int64>    // down regions
    @scratch moe_boffs1 : array<int64>   // gate bias offsets (moe_exps_bias arches)
    @scratch moe_boffs3 : array<int64>   // up bias offsets
    @scratch moe_boffs2 : array<int64>   // down bias offsets
    @scratch moe_ghb : array<float>      // bucket gate outputs (npos x max(n_ff_exp, n_ff_shexp))
    @scratch moe_ghb2 : array<float>     // bucket up outputs (same shape)
    @scratch moe_hq : array<int8>        // bucket hidden requant quants (npos x max(n_ff_exp, n_ff_shexp))
    @scratch moe_hs : array<float>       // bucket hidden requant scales (same / 32)
    @scratch moe_gout : array<float>     // bucket (then shared-expert) down-projection output (npos x dim)
    @scratch moe_eout : array<float>     // routed expert outputs parked by (position, slot) (npos x k x dim)
    ffn_offs : array<int64>     // dense/shexp gate+up fuse: 2 region (weight, activation) offset pairs
    ffn_gu : array<float>       // dense/shexp gate+up fuse output — gate half then up half (2 x max_ffn_width)
    mx4_wq : array<int8>        // grouped-prefill expansion of ONE native-MXFP4 expert to exact Q8 (dim x n_ff_exp)
    mx4_ws : array<float>       // ... and its per-block scales (same / 32); see expand_mx4_region_q8
    // gemma-4 E-series per-layer embeddings (PLE) scratch — empty unless config.n_embd_per_layer > 0.
    // ple_inp holds one (position, layer) side vector per row, POSITION-major: (p, l, i) at
    // p*(n_layers*ple) + l*ple + i. Sized to 1 position in make_run_state, npos in forward_prefill.
    @scratch ple_inp : array<float>      // the built per-(position, layer) side input (npos x n_layers x ple)
    @scratch ple_proj : array<float>     // model-proj scratch for the pre-step (npos x n_layers x ple)
    @scratch ple_gate : array<float>     // per-(position) gate vector for the layer block (npos x ple)
    @scratch ple_gpu_tokens : array<int64>  // stashed token rows for a driver-owned pre-step (see register_ple_gpu_gate)
    ple_gpu_pending : bool      // forward_prefill skipped the CPU pre-step — the engaged driver builds it
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
    @scratch dn_qkv_b : array<float>
    @scratch dn_z_b : array<float>
    @scratch dn_beta_b : array<float>
    @scratch dn_g_b : array<float>
    @scratch dn_o_b : array<float>
    @scratch dn_qg_b : array<float>
    @scratch dn_qgate_b : array<float>
    // chunked deltanet prefill scratch (DnPrefillMode.chunked only)
    @scratch dn_conv_b : array<float>       // batched conv + SiLU output (npos x conv_dim)
    @scratch dn_wtap : array<float>         // conv weights transposed tap-major for the batched form (d_conv x conv_dim)
    @scratch dn_cws : array<float>          // per-v-head chunk workspace slabs (dt_rank x dn_cws_head_size)
    @scratch dn_tail : array<float>         // GPU deltanet chain: last taps qkv rows back (conv-history store)
    // MTP/NextN self-spec state (empty unless config.n_layer_nextn > 0). mtp_h is the trunk's
    // post-output-norm hidden of the LAST evaluated position (llama.cpp's t_h_nextn handoff);
    // mtp_h_pos1 is that position PLUS ONE (0 = cold, keeps Session zero-init-safe).
    mtp_h : array<float>           // h_nextn (dim)
    mtp_h0 : array<float>          // the verify batch's ROW-0 normed hidden (= h_pos). forward_prefill's
                                   // tail only stashes the LAST row, which a reject discards — this is
                                   // what re-seeds mtp_h so a rejected step needs no re-forward
    mtp_h_pos1 : int64
    mtp_cat : array<float>         // eh_proj input [enorm(embed(tok)) ; hnorm(h)] (2*dim)
    mtp_cat_b : array<float>       // prompt-warm eh_proj concat image ((npos-1) x 2*dim, scratch-sized per warm)
    @scratch mtp_xb_save : array<float>     // prompt-warm x_b preservation (the warm's eh_proj output rides x_b for the arch blocks; consumers like mtp_verify_row0 / embed_forward need the trunk hiddens back)
    mtp_logits : array<float>      // verify-batch row-0 logits (vocab; the draft check reads its argmax)
    mtp_vbatch : array<int64>      // the 2-row verify batch [tok, draft] (persistent — no per-step alloc)
    mtp_norm_b : array<float>      // verify-batch final-normed rows (npos x dim), input to the batched classifier
    mtp_logits_b : array<float>    // verify-batch logits (npos x vocab) — ONE classifier pass covers every row
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
        s.mtp_h0 |> resize(c.dim)
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
    if (long_length(pool.blob_of) != kv_layers) {
        panic("make_run_state_paged: pool was built for a different model ({long_length(pool.blob_of)} layers vs {kv_layers})")
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
    let npos = long_length(tokens)
    if (npos <= 0l) return
    forward_batch(t, s, tokens, npos, s.n_past)
    s.n_past += npos
}

// ===== Per-app runtime profile (the tune sidecar's "runtime" section) =====

// ===== Metal-4 tensor-lane crowns (the sidecar's "runtime"."metal_tensor" knob) =====
// Comma-list of families whose TENSOR twin won this box's tune race; consulted once per
// family at pso build. Default empty = simdgroup; a DAS_TUNE_MANIFEST copy force-crowns.
var private g_tensor_crowns : table<string>

def public set_metal_tensor_crowns(list : string) {
    unsafe {
        delete g_tensor_crowns
    }
    for (f in split(list, ",")) {
        if (!empty(f)) {
            g_tensor_crowns |> insert(f)
        }
    }
}

def public metal_tensor_crowned(family : string) : bool {
    return key_exists(g_tensor_crowns, family)
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
def mm_fblob(var y : array<float>; t : Model; woff : int64; x : array<float>; n, d : int64) {
    matmul(y, t.fblob, woff, x, n, d)
}

// Batched fp32 matmul against the aux blob: Y [npos x d] token-major from X [npos x n] — the grouped
// MoE prefill's router. matmul_batch is bit-for-bit npos x matmul, so routing matches decode exactly.
def mm_fblob_b(var y : array<float>; t : Model; woff : int64; x : array<float>; n, d, npos : int64) {
    matmul_batch(y, t.fblob, woff, x, n, d, npos)
}

// rmsnorm with the weight at `woff` in the fp32 aux blob — straight to the tuned pointer core.
def rms_at(var o : array<float> | #; t : Model; woff : int64; x : array<float> | #; size : int64) {
    unsafe {
        rmsnorm(addr < float? >(o[0]), addr < float const? >(x[0]),
            addr < float const? >(t.fblob[woff]), size, t.config.norm_eps)
    }
}

// Same, with the source at element offset `xoff` (per-position rows of a batched activation).
def rms_at(var o : array<float> | #; t : Model; woff : int64; x : array<float>; xoff : int64; size : int64) {
    unsafe {
        rmsnorm(addr < float? >(o[0]), addr < float const? >(x[xoff]),
            addr < float const? >(t.fblob[woff]), size, t.config.norm_eps)
    }
}

// Threaded per-position rmsnorm: out[p*dim..] = rmsnorm(src[p*dim..]) with the layer weight at fblob+woff.
// `out` and `src` may be the same array (Gemma post-norms run in place). Fork workers touch raw addresses.
def rms_batch(var out : array<float>; src : array<float>; t : Model; woff : int64; dim, npos : int64) {
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
def build_rope_table(@scratch var cos_tab : array<float>; @scratch var sin_tab : array<float>; t : Model; theta, fscale : float; start_pos, npos, head_size : int64; use_ff : bool = true) {
    build_rope_tabs(cos_tab, sin_tab, theta, fscale, t.config.rope_mscale, t.rope_freqs, use_ff,
                    start_pos, npos, head_size)
}

// build_rope_table's batched-decode twin: row pi's angle uses positions[pi] instead of start_pos+pi
// (batch rows come from different sequences at unrelated positions). Bit-identical per row to the
// npos=1 decode table at that position, so rope_batch_tab consumes it unchanged.
def build_rope_table_rows(@scratch var cos_tab : array<float>; @scratch var sin_tab : array<float>; t : Model;
                                  theta, fscale : float; positions : array<int64>; nrows, head_size : int64;
                                  use_ff : bool = true) {
    build_rope_tabs_rows(cos_tab, sin_tab, theta, fscale, t.config.rope_mscale, t.rope_freqs, use_ff,
                         positions, nrows, head_size)
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
[cold_path]   // doubling growth: amortized O(1) per page, and never on a flat session
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
[cold_path]   // only on a page crossing — once every kv_pool.page_rows positions
def private kv_grow_block_table(var s : Session; np : int64) {
    while (long_length(s.block_table) < np) {
        s.block_table |> push(kv_pool_alloc_group(*s.kv_pool))
    }
}

def kv_ensure(var s : Session; upto : int64) {
    if (s.kv_pool == null) {
        return
    }
    let np = (upto + s.kv_pool.page_rows - 1l) / s.kv_pool.page_rows
    if (long_length(s.block_table) < np) {
        kv_grow_block_table(s, np)
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
def kv_axpy(var d : float?; s : float const?; scale : float; n : int64) {
    axpy(d, s, scale, n)
}
def kv_load(p : float const?; base, i : int64) : float => unsafe(p[base + i])
def private kv_dot(a : float const?; b : uint16 const?; n : int64) : float => dot_f16(a, b, n)
def kv_axpy(var d : float?; s : uint16 const?; scale : float; n : int64) {
    axpy_f16(d, s, scale, n)
}
def kv_load(p : uint16 const?; base, i : int64) : float => f16_to_f32(uint(unsafe(p[base + i])))
// q8_0: the cache pointer stays uint8 (byte units — the _d wrappers prescale stride/offset via
// kv_side_off), so `base` here is a whole-block BYTE offset and `i` an element index within it
def private kv_dot(a : float const?; b : uint8 const?; n : int64) : float => dot_q8kv(a, b, n)
def kv_axpy(var d : float?; s : uint8 const?; scale : float; n : int64) {
    axpy_q8kv(d, s, scale, n)
}
def kv_load(p : uint8 const?; base, i : int64) : float {
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
def kv_axpy(var d : float?; s : TQ4B const?; scale : float; n : int64) {
    axpy_tq4kv(d, unsafe(reinterpret<uint8 const?>(s)), scale, n)
}
def kv_load(p : TQ4B const?; base, i : int64) : float {
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
def kv_head_off(p : float const?; elems : int64) : int64 => elems
[unused_argument(p)]
def kv_head_off(p : uint16 const?; elems : int64) : int64 => elems
[unused_argument(p)]
def kv_head_off(p : uint8 const?; elems : int64) : int64 => elems / 32l * 34l
[unused_argument(p)]
def kv_head_off(p : TQ4B const?; elems : int64) : int64 => elems / 32l * 18l

// Bulk one-row dequant into f32 scratch — the flash pack's per-row primitive (and any future
// codec's bulk-dequant seam: q8_0 adds a cvt overload here, never a new pack shape).
def kv_row_to_f32(var d : float?; p : float const?; base, n : int64) {
    unsafe {
        copy_floats(d, p + base, n)
    }
}
def kv_row_to_f32(var d : float?; p : uint16 const?; base, n : int64) {
    unsafe {
        cvt_f16_to_f32(d, p + base, n)
    }
}
def kv_row_to_f32(var d : float?; p : uint8 const?; base, n : int64) {
    unsafe {
        cvt_q8kv_to_f32(d, p + base, n)
    }
}
def kv_row_to_f32(var d : float?; p : TQ4B const?; base, n : int64) {
    unsafe {
        cvt_tq4kv_to_f32(d, reinterpret<uint8 const?>(p) + base, n)
    }
}

// Decode-read score dot, resolved on the K-cache pointer type: scalar codecs dot the f32 query;
// q8_0 dots the PRE-QUANTIZED query planes (qq/qs — filled once per token after RoPE) through the
// int8×int8 kernel. Lets the flash-decode witness recompute scores exactly as the sliced path did.
[unused_argument(qq, qs)]
def kv_score(qp : float const?; qq : int8 const?; qs : float const?; kcp : float const?;
                     koff, qoff, head_size : int64) : float {
    return kv_dot(unsafe(qp + qoff), unsafe(kcp + koff), head_size)
}
[unused_argument(qq, qs)]
def kv_score(qp : float const?; qq : int8 const?; qs : float const?; kcp : uint16 const?;
                     koff, qoff, head_size : int64) : float {
    return kv_dot(unsafe(qp + qoff), unsafe(kcp + koff), head_size)
}
[unused_argument(qp)]
def kv_score(qp : float const?; qq : int8 const?; qs : float const?; kcp : uint8 const?;
                     koff, qoff, head_size : int64) : float {
    return dot_q8q8kv(unsafe(qq + qoff), unsafe(qs + qoff / 32l), unsafe(kcp + koff), head_size)
}
// tq4 K: same pre-quantized-query planes as q8_0 — the drivers fill them from the ROTATED query,
// so the int8×int4 dot runs entirely in the rotated basis
[unused_argument(qp)]
def kv_score(qp : float const?; qq : int8 const?; qs : float const?; kcp : TQ4B const?;
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

def attn_head_decode_d(var arow : float?; var xbp : float?; qp : float const?;
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

def kv_slice_scores_d(var attp : float?; qp : float const?; qq : int8 const?; qsp : float const?;
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

def kv_slice_av_d(var flop : float?; attp : float const?; vcl : uint8 const?; vdt : KVDtype;
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
[cold_path]   // reference-check path, armed by g_flash_decode_check
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
[cold_path]   // reference-check path, armed by g_flash_decode_check
def flash_decode_check_d(qp : float const?; qq : int8 const?; qsp : float const?;
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
def tq4_unrotate_batch(var buf : array<float>; nrows, stride, head_size : int64; signs : float const?) {
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
def gate_batch(var g : array<float>; u : array<float>; act : FfnAct; hidden, npos : int64) {
    unsafe {
        gate_batch(addr(g[0]), addr < float const? >(u[0]), act, hidden, npos)
    }
}

// pointer-level core — the fused gate+up dispatch acts on the halves of one groupn output buffer
def gate_batch(var gp0 : float?; up0 : float const?; act : FfnAct; hidden, npos : int64) {
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
def add_bias(var y : array<float>; yoff : int64; b : array<float>; boff, n : int64) {
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
def mm_qkv(var s : Session; t : Model; l : int64) {
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
    if (moe_gpu_qkv_on_gpu(l)) {   // resident triple: all three projections in one device submit
        let qrows = c.q_gated ? 2l * qd : qd
        if (fq == KqFmt.q8) {
            quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
            matmul_gpu_qkv(s.q, s.k, s.v, qoff, koff, voff, int(fq), int(fk), int(fv),
                s.xq, s.xs, dim, qrows, kv, kv)
        } else {
            quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
            matmul_gpu_qkv(s.q, s.k, s.v, qoff, koff, voff, int(fq), int(fk), int(fv),
                s.kxq, s.kxs, dim, qrows, kv, kv)
        }
        return
    }
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
def mm_at_q8_pre(var y : array<float>; t : Model; woff : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
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
def mm_at_q8_groupn(var y : array<float>; t : Model; offs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    if (t.wscale_f16) {
        matmul_q8q8_groupn(y, t.qblob, t.qscales16, offs, nregions, xq, xs, n, d)
    } else {
        matmul_q8q8_groupn(y, t.qblob, t.qscales, offs, nregions, xq, xs, n, d)
    }
}

def mm_at_q8_groupn(var y : array<float>; t : Model; offs, boffs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    if (t.wscale_f16) {
        matmul_q8q8_groupn(y, t.qblob, t.qscales16, offs, nregions, xq, xs, t.fblob, boffs, n, d)
    } else {
        matmul_q8q8_groupn(y, t.qblob, t.qscales, offs, nregions, xq, xs, t.fblob, boffs, n, d)
    }
}

def mm_at_mx4_groupn(var y : array<float>; t : Model; offs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    matmul_mx4q8_groupn(y, t.mxq, t.mxs, offs, nregions, xq, xs, n, d)
}

def mm_at_mx4_groupn(var y : array<float>; t : Model; offs, boffs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    matmul_mx4q8_groupn(y, t.mxq, t.mxs, offs, nregions, xq, xs, t.fblob, boffs, n, d)
}

// ===== native q51 dispatch (Q8_0-form activations — NOT the kq Q8_K image) =====

def mm_at_q51_groupn(var y : array<float>; t : Model; offs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    matmul_q51q8_groupn(y, t.q51q, t.q51s, offs, nregions, xq, xs, n, d)
}

def mm_b_q51_pre(var y : array<float>; t : Model; woff : int64; xqb : array<int8>; xsb : array<float>; n, d, npos : int64) {
    matmul_q51q8_batch(y, t.q51q, t.q51s, woff, xqb, xsb, n, d, npos)
}

def mm_b_q51_groupn(var y : array<float>; t : Model; offs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; n, d : int64) {
    matmul_q51q8_batch_groupn(y, t.q51q, t.q51s, offs, nregions, xq, xs, n, d)
}

// ===== native K-quant dispatch (kquant_native; fmt != q8 tags) =====

// KqFmt -> the kernel-layer format id (matmul_kq*/kq_rows_fn/kq_qsb take the int form). q8 maps
// to 0, a poison id no kq kernel accepts — kq_rows_for calls this for EVERY tag, so it must stay
// total even though q8 branches never read the result.
def private kq_fi(fmt : KqFmt) : int => fmt == KqFmt.k4 ? 4 : (fmt == KqFmt.k5 ? 5 : (fmt == KqFmt.k6 ? 6 : (fmt == KqFmt.q40 ? 40 : 0)))

// K-quant GEMV against a PRE-quantized activation (xq/xs/xbs already filled by the bs quantizer).
// kq_repacked loads run the backend's stamped cores; disk-order loads run the portable kernels.
def mm_at_kq_pre(var y : array<float>; t : Model; fmt : KqFmt; woff : int64; xq : array<int8>; xs : array<float>; xbs : array<int>; n, d : int64; yoff : int64 = 0l) {
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
def mm_b_kq(var s : Session; var y : array<float>; t : Model; fmt : KqFmt; woff : int64; x : array<float>; n, d, npos : int64) {
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
def mm_at_kq_groupn(var y : array<float>; t : Model; fmt : KqFmt; offs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; xbs : array<int>; n, d : int64) {
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
def mm_b_kq_groupn(var y : array<float>; t : Model; fmt : KqFmt; offs : array<int64>; nregions : int64; xq : array<int8>; xs : array<float>; xbs : array<int>; n, d : int64) {
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
def mm_b_kq_pre(var y : array<float>; t : Model; fmt : KqFmt; woff : int64; xq : array<int8>; xs : array<float>; xbs : array<int>; n, d, ntok : int64) {
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

def kq_plane_q(t : Model; fmt : KqFmt; woff : int64) : uint8 const? {
    let sb = woff / 256l
    if (fmt == KqFmt.k4) return unsafe(addr<uint8 const?>(t.k4q[sb * K4_QSB]))
    if (fmt == KqFmt.k5) return unsafe(addr<uint8 const?>(t.k5q[sb * K5_QSB]))
    if (fmt == KqFmt.k6) return unsafe(addr<uint8 const?>(t.k6q[sb * K6_QSB]))
    if (fmt == KqFmt.q40) return unsafe(addr<uint8 const?>(t.q40q[sb * Q40_QSB]))
    return null
}

def kq_plane_s(t : Model; fmt : KqFmt; woff : int64) : uint8 const? {
    let sb = woff / 256l
    if (fmt == KqFmt.k4) return unsafe(addr<uint8 const?>(t.k4s[sb * K4_SSB]))
    if (fmt == KqFmt.k5) return unsafe(addr<uint8 const?>(t.k5s[sb * K5_SSB]))
    if (fmt == KqFmt.k6) return unsafe(addr<uint8 const?>(t.k6s[sb * K6_SSB]))
    if (fmt == KqFmt.q40) return unsafe(addr<uint8 const?>(t.q40s[sb * Q40_SSB]))
    return null
}

def kq_rows_for(fmt : KqFmt) : MatmulKqRowsFn {
    return kq_rows_fn(kq_fi(fmt))
}

// Can the fused chains serve a weight of this format? q8 always; superblock kq only off a
// repacked load (the backend rows cores read the grp layout). q51 NEVER — no chain/rows arms.
def kq_servable(t : Model; fmt : KqFmt) : bool => fmt == KqFmt.q8 || (kq_sb(fmt) && t.kq_repacked)

// Do the gate/up stacks read the Q8_K-form scratch? Superblock formats only — q8 AND q51 read
// the Q8_0-form image. The pair shares ONE gathered activation plane, so both stacks must
// agree on the form (mixed sb/non-sb panics).
def moe_pair_kform(f1, f3 : KqFmt) : bool {
    let k1 = kq_sb(f1)
    let k3 = kq_sb(f3)
    if (k1 != k3) {
        panic("dasLLAMA: MoE gate/up expert stacks mix Q8_K-form and Q8_0-form weights — unsupported until per-stack activation planes land")
    }
    return k1
}

// Format router: q8-tagged weights take the classic mm; kq tags take the plane GEMV.
def mm_f(var y : array<float>; t : Model; fmt : KqFmt; woff : int64; x : array<float>; var xq : array<int8>; var xs : array<float>; var xbs : array<int>; n, d : int64) {
    if (fmt == KqFmt.q8) {
        mm(y, t, woff, x, xq, xs, n, d)
    } else {
        mm_kq(y, t, fmt, woff, x, xq, xs, xbs, n, d)
    }
}

// Pre-quantized format router: q8-tagged weights consume the Q8_0-form scratch (xq/xs);
// kq-tagged weights the Q8_K-form scratch (kxq/kxs/kxbs — kq v2, all three formats).
def mm_pre_f(var y : array<float>; t : Model; fmt : KqFmt; woff : int64; xq : array<int8>; xs : array<float>; kxq : array<int8>; kxs : array<float>; kxbs : array<int>; n, d : int64) {
    if (fmt == KqFmt.q8) {
        mm_at_q8_pre(y, t, woff, xq, xs, n, d)
    } else {
        mm_at_kq_pre(y, t, fmt, woff, kxq, kxs, kxbs, n, d)
    }
}

// Batched format router: q8 weights ride the batch tile; kq weights take the native kq tile
// (repacked loads) or the serial-per-position fallback (see mm_b_kq).
def mm_b_f(var s : Session; var y : array<float>; t : Model; fmt : KqFmt; woff : int64; x : array<float>; n, d, npos : int64) {
    if (fmt == KqFmt.q8) {
        mm_b(y, t, woff, x, s.xqb, s.xsb, n, d, npos)
    } else {
        mm_b_kq(s, y, t, fmt, woff, x, n, d, npos)
    }
}

// Route a big-weight matmul through the Q8, Q4, or fp32 kernel depending on the model's quant mode.
// xq/xs are the run-state activation-quant scratch (used only by the Q8 path).
def mm(var y : array<float>; t : Model; woff : int64; x : array<float>; var xq : array<int8>; var xs : array<float>; n, d : int64) {
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
def requant_rows_q8(x : array<float>; n, npos : int64; var xqb : array<int8>; var xsb : array<float>) {
    let ts_rq = ref_time_ticks()
    requant_rows_q8(x, n, npos, xqb, xsb, n * npos >= g_requant_par_threshold)
    prof_add("mm_requant", ts_rq)
}

// Q8_K-form batch requant (kq v2): per-256 scales + per-16 sums of the per-256-scaled
// quants. k4-weight batch matmuls and MoE gathers consume these rows.
def requant_rows_q8k_bs(x : array<float>; n, npos : int64; var xqb : array<int8>; var xsb : array<float>; var xbsb : array<int>) {
    let ts_rq = ref_time_ticks()
    requant_rows_q8k_bs(x, n, npos, xqb, xsb, xbsb, n * npos >= g_requant_par_threshold)
    prof_add("mm_requant", ts_rq)
}

// Batched native-MXFP4 GEMM against a PRE-quantized activation image (the batch analog of
// mm_at_mx4_pre): grouped MoE prefill dispatches here — no Q8 expansion — when the active backend
// carries a native mx4 batch kernel (g_active_mx4_native_batch).
def mm_b_mx4_pre(var y : array<float>; t : Model; woff : int64; xqb : array<int8>; xsb : array<float>; n, d, npos : int64) {
    let ts_gemm = ref_time_ticks()
    matmul_mx4q8_batch(y, t.mxq, t.mxs, woff, xqb, xsb, n, d, npos)
    prof_add("mm_gemm", ts_gemm)
}

// Batched Q8 GEMM against a PRE-quantized activation image (the batch analog of mm_at_q8_pre) —
// the grouped MoE prefill runs many GEMMs off one requant (the whole-batch image or a gathered
// expert bucket).
def mm_b_q8_pre(var y : array<float>; t : Model; woff : int64; xqb : array<int8>; xsb : array<float>; n, d, npos : int64) {
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
def mm_b(var y : array<float>; t : Model; woff : int64; x : array<float>; var xqb : array<int8>; var xsb : array<float>; n, d, npos : int64) {
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
def dn_cws_head_size(c : Config) : int64 => (8l * DN_CHUNK * c.ssm_d_state
    + 2l * DN_CHUNK * DN_CHUNK + 3l * DN_CHUNK)

def dn_reset(var s : Session) {
    for (i in range64(long_length(s.dn_conv_state))) {
        s.dn_conv_state[i] = 0.0
    }
    for (i in range64(long_length(s.dn_state))) {
        s.dn_state[i] = 0.0
    }
    moe_gpu_dn_invalidate()   // CPU zeros are now authoritative — drop any device-resident copies
}

// Sync one layer's device-resident deltanet state down before a CPU read (no-op when clean).
def dn_flush_layer(var s : Session; l : int64) {
    if (l < long_length(s.dn_state_prefix) && s.dn_state_prefix[l] >= 0l) {
        unsafe {
            moe_gpu_dn_flush(l, addr(s.dn_state[s.dn_state_prefix[l]]),
                addr(s.dn_conv_state[s.dn_conv_prefix[l]]))
        }
    }
}

def dn_flush_all(var s : Session) {
    for (l in range64(long_length(s.dn_state_prefix))) {
        dn_flush_layer(s, l)
    }
}

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

// Q8_0 row dequant from the linear embedding copy: dst[i] = q[i] * scale[i/32] — the same
// arithmetic gguf_read_tensor_f32 used to fill fblob, so the values are bit-identical to the
// fp32 table this replaces.
def dequant_q8_row(t : Model; off, n : int64; var dst : float?) {
    if (t.metal_blob) {
        // blob layout: f16 scale + 32 int8 per 34-byte block at (off/32)*34
        unsafe {
            let bp = addr < uint8 const? >(t.mblob[(off / 32l) * 34l])
            for (b in range64(n / 32l)) {
                let sc = f16_to_f32(uint(bp[b * 34l]) | (uint(bp[b * 34l + 1l]) << 8u))
                let qp = reinterpret<int8 const?>(bp + b * 34l + 2l)
                let base = b * 32l
                for (k in range64(32l)) {
                    dst[base + k] = float(qp[k]) * sc
                }
            }
        }
        return
    }
    // Fail closed loudly (a bare bounds trip burned two sessions); guards the qblob path — metal returned above.
    if (off < 0l || off + n > long_length(t.qblob)) {
        panic("dequant_q8_row OOB: off={off} n={n} qblob={long_length(t.qblob)} ple_emb_off={t.ple_emb_off} emb_q8_off={t.emb_q8_off} quant={t.quant}")
    }
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

// trimmed-flavor q8 embedding read: embq holds the emb region as mblob-style 34B interleaved
// blocks (f16 scale + 32 int8), region-rebased — same loop as dequant_q8_row's mblob arm
def private dequant_embq_row(t : Model; off, n : int64; var dst : float?) {
    unsafe {
        let bp = addr < uint8 const? >(t.embq[(off / 32l) * 34l])
        for (b in range64(n / 32l)) {
            let sc = f16_to_f32(uint(bp[b * 34l]) | (uint(bp[b * 34l + 1l]) << 8u))
            let qp = reinterpret<int8 const?>(bp + b * 34l + 2l)
            let base = b * 32l
            for (k in range64(32l)) {
                dst[base + k] = float(qp[k]) * sc
            }
        }
    }
}

// Copy token `token`'s embedding row into dst — from fp32 fblob, or (cls_q8) dequanted from the
// linear Q8 copy at emb_q8_off (embq when trimmed), or (cls_kq) from the K-quant plane at
// wcls_off (embq/embs when trimmed). Rows are whole superblocks (a ggml K-quant invariant).
def embed_row(t : Model; token : int64; var dst : float?) {
    let dim = t.config.dim
    if (t.cls_q8) {
        if (t.planes_trimmed) {
            dequant_embq_row(t, token * dim, dim, dst)
        } else {
            dequant_q8_row(t, t.emb_q8_off + token * dim, dim, dst)
        }
    } elif (cls_kq(t)) {
        let mr = kq_active_mr(t, t.emb_fmt)   // 1 when not repacked — unread past the guard then
        if (t.kq_repacked && token < (t.config.vocab_size / mr) * mr) {
            // repacked plane: gather via the grp<mr> layout (tail rows below stay disk-order, verbatim)
            unsafe {
                let nsb = dim / 256l
                let g = token / mr
                let fmt = int64(kq_fi(t.emb_fmt))
                // trimmed: embq/embs hold the region's plane pair verbatim, rebased to 0
                let sb0t = t.planes_trimmed ? 0l : t.wcls_off / 256l
                let sbg = (sb0t + g * mr * nsb)
                if (t.planes_trimmed) {
                    if (t.emb_fmt == KqFmt.k4) {
                        dequant_kq_row_grp(fmt, addr(t.embq[sbg * K4_QSB]), addr(t.embs[sbg * K4_SSB]), token % mr, mr, dim, dst)
                    } elif (t.emb_fmt == KqFmt.k5) {
                        dequant_kq_row_grp(fmt, addr(t.embq[sbg * K5_QSB]), addr(t.embs[sbg * K5_SSB]), token % mr, mr, dim, dst)
                    } elif (t.emb_fmt == KqFmt.k6) {
                        dequant_kq_row_grp(fmt, addr(t.embq[sbg * K6_QSB]), addr(t.embs[sbg * K6_SSB]), token % mr, mr, dim, dst)
                    } elif (t.emb_fmt == KqFmt.q40) {
                        dequant_kq_row_grp(fmt, addr(t.embq[sbg * Q40_QSB]), addr(t.embs[sbg * Q40_SSB]), token % mr, mr, dim, dst)
                    } else {
                        panic("embed_row: '{t.emb_fmt}' has no kq plane pair")
                    }
                } elif (t.emb_fmt == KqFmt.k4) {
                    dequant_kq_row_grp(fmt, addr(t.k4q[sbg * K4_QSB]), addr(t.k4s[sbg * K4_SSB]), token % mr, mr, dim, dst)
                } elif (t.emb_fmt == KqFmt.k5) {
                    dequant_kq_row_grp(fmt, addr(t.k5q[sbg * K5_QSB]), addr(t.k5s[sbg * K5_SSB]), token % mr, mr, dim, dst)
                } elif (t.emb_fmt == KqFmt.k6) {
                    dequant_kq_row_grp(fmt, addr(t.k6q[sbg * K6_QSB]), addr(t.k6s[sbg * K6_SSB]), token % mr, mr, dim, dst)
                } elif (t.emb_fmt == KqFmt.q40) {
                    dequant_kq_row_grp(fmt, addr(t.q40q[sbg * Q40_QSB]), addr(t.q40s[sbg * Q40_SSB]), token % mr, mr, dim, dst)
                } else {
                    panic("embed_row: '{t.emb_fmt}' has no kq plane pair")
                }
            }
        } else {
            unsafe {
                var row = temp_array(dst, dim, type<float>)
                // trimmed: embq/embs hold the region's plane pair verbatim, rebased to 0
                let sb0 = t.planes_trimmed ? (token * dim) / 256l : (t.wcls_off + token * dim) / 256l
                // metal-blob planes: k4/k5 strips are 16B, k6 is the split GPU form (d in the tail)
                let ssb45 = t.metal_blob ? 16l : K4_SSB
                let k6d0 = t.metal_blob ? (long_length(t.k6s) / K6_SSB) * 16l : 0l
                for (s in range64(dim / 256l)) {
                    if (t.planes_trimmed) {
                        if (t.emb_fmt == KqFmt.k4) {
                            dequant_k4_plane_superblock(t.embq, (sb0 + s) * K4_QSB, t.embs, (sb0 + s) * K4_SSB, row, s * 256l)
                        } elif (t.emb_fmt == KqFmt.k5) {
                            dequant_k5_plane_superblock(t.embq, (sb0 + s) * K5_QSB, t.embs, (sb0 + s) * K5_SSB, row, s * 256l)
                        } elif (t.emb_fmt == KqFmt.k6) {
                            dequant_k6_plane_superblock(t.embq, (sb0 + s) * K6_QSB, t.embs, (sb0 + s) * K6_SSB, row, s * 256l)
                        } elif (t.emb_fmt == KqFmt.q40) {
                            dequant_q40_plane_superblock(t.embq, (sb0 + s) * Q40_QSB, t.embs, (sb0 + s) * Q40_SSB, row, s * 256l)
                        } else {
                            panic("embed_row: '{t.emb_fmt}' has no kq plane pair")
                        }
                    } elif (t.emb_fmt == KqFmt.k4) {
                        dequant_k4_plane_superblock(t.k4q, (sb0 + s) * K4_QSB, t.k4s, (sb0 + s) * ssb45, row, s * 256l)
                    } elif (t.emb_fmt == KqFmt.k5) {
                        dequant_k5_plane_superblock(t.k5q, (sb0 + s) * K5_QSB, t.k5s, (sb0 + s) * ssb45, row, s * 256l)
                    } elif (t.emb_fmt == KqFmt.k6) {
                        if (t.metal_blob) {
                            dequant_k6_plane_superblock_at(t.k6q, (sb0 + s) * K6_QSB, t.k6s, (sb0 + s) * 16l, k6d0 + (sb0 + s) * 2l, row, s * 256l)
                        } else {
                            dequant_k6_plane_superblock(t.k6q, (sb0 + s) * K6_QSB, t.k6s, (sb0 + s) * K6_SSB, row, s * 256l)
                        }
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
    if (t.planes_trimmed && !moe_gpu_cls_on_gpu(t.wcls_off)) {
        // fail closed LOUDLY: the CPU classifier path reads the trimmed families
        panic("dasLLAMA: planes_trimmed image — the classifier must serve on the GPU (rebake without trim, or arm the tier as the image's config says)")
    }
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
    s.chain_stages |> resize(1)   // nolint:PERF026 — session scratch, fixed size after the first classifier call
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
        team_parallel_stages(s.chain_stages) <| @(stage : int; jb : int; je : int) {   // nolint:PERF026 — dispatch-API lambda capture, see attention_chain_decode
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

//! One forward pass: token at position `pos` -> s.logits. Faithful port of llama2.c forward().
[no_alloc, no_env, no_io]
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
        blob_only_panic(t, "decode stack")
        for (l in range64(c.n_layers)) {
            let ts_blk_attn = ref_time_ticks()
            t.blocks.attn_decode(t, s, l, pos)
            prof_add("blk_attn", ts_blk_attn)
            let ts_blk_ffn = ref_time_ticks()
            t.blocks.ffn_decode(t, s, l)
            prof_add("blk_ffn", ts_blk_ffn)
        }
    }

    return if (stack_done && g_decode_logits_done)   // the override already produced s.logits

    // final rmsnorm (into xb to avoid aliasing x as both in/out), then classifier
    blob_only_panic(t, "final-norm/classifier step")
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

var private g_mtp_batch_cls = true

//! One batched classifier pass over the whole MTP verify window instead of a per-row GEMV each —
//! the plane (vocab x dim) is a step's largest weight read. Pure perf lever; A/B only.
def set_mtp_batch_cls(on : bool) {
    g_mtp_batch_cls = on
}

//! Current state of the [[set_mtp_batch_cls]] gate.
def get_mtp_batch_cls() : bool => g_mtp_batch_cls

[no_alloc, no_env, no_io]
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

//! CPU-side input prep for a GPU draft step: the raw embed row into s.x (+ embed_scale) and the
//! draft row's rope table — forward()'s prologue at the draft position, for the metal MTP arm
def mtp_draft_prep(t : Model; var s : Session; token, pos : int64) {
    let c = t.config
    embed_row(t, token, unsafe(addr(s.x[0])))
    if (c.embed_scale != 1.0) {
        scale_inplace(unsafe(addr(s.x[0])), c.embed_scale, c.dim)
    }
    build_rope_table(s.rope_cos, s.rope_sin, t, c.rope_freq_base, c.rope_freq_scale, pos, 1l,
                     c.rope_dim > 0l ? c.rope_dim : c.head_size, true)
}

//! CPU-side input prep for a GPU 2-row verify: embed rows for [tok@pos, d@pos+1] into s.mtp_cat
//! (2*dim scratch — row-major) and the 2-row rope table, for the metal MTP arm
def mtp_verify_prep(t : Model; var s : Session; tok, d, pos : int64) {
    let c = t.config
    embed_row(t, tok, unsafe(addr(s.mtp_cat[0])))
    embed_row(t, d, unsafe(addr(s.mtp_cat[c.dim])))
    if (c.embed_scale != 1.0) {
        scale_inplace(unsafe(addr(s.mtp_cat[0])), c.embed_scale, 2l * c.dim)
    }
    build_rope_table(s.rope_cos, s.rope_sin, t, c.rope_freq_base, c.rope_freq_scale, pos, 2l,
                     c.rope_dim > 0l ? c.rope_dim : c.head_size, true)
}

//! Snapshot/restore the deltanet recurrent state (conv history + S + dn_pos) around a speculative
//! verify batch: a rejected draft has already advanced the state past itself. KV rows stay correct;
//! only the recurrent state rolls back, then one plain forward re-advances it.
def mtp_state_snapshot(var s : Session) {
    dn_flush_all(s)   // the snapshot must capture device-resident decode-step state
    let nc = long_length(s.dn_conv_state)
    let ns = long_length(s.dn_state)
    s.mtp_snap_conv |> resize(nc)   // no-op after the first call
    s.mtp_snap_state |> resize(ns)
    copy_floats(s.dn_conv_state, 0l, s.mtp_snap_conv, 0l, nc)
    copy_floats(s.dn_state, 0l, s.mtp_snap_state, 0l, ns)
    s.mtp_snap_pos = s.dn_pos
}

def mtp_state_restore(var s : Session) {
    copy_floats(s.mtp_snap_conv, 0l, s.dn_conv_state, 0l, long_length(s.mtp_snap_conv))
    copy_floats(s.mtp_snap_state, 0l, s.dn_state, 0l, long_length(s.mtp_snap_state))
    s.dn_pos = s.mtp_snap_pos
    moe_gpu_dn_invalidate()   // restored CPU state is authoritative — device copies are stale
}

// After a 2-row verify forward_prefill([tok, draft], 2, pos): row 0's post-norm hidden and
// logits (forward_prefill's own tail only produced row 1's). Reads s.x_b row 0 — still intact,
// the prefill tail never overwrites the batch residual stream.
def private mtp_verify_row0(t : Model; var s : Session) {
    let c = t.config
    let dim = c.dim
    rms_at(s.xb, t, t.rms_final_off, s.x_b, 0l, dim)
    copy_floats(s.xb, 0l, s.mtp_h0, 0l, dim)   // = h_pos; a reject re-seeds the draft head from it
    mm_cls(s.mtp_logits, t, s.xb, s.xq, s.xs, s.xbs, dim, c.vocab_size)
    // mirror the plain classifier tail exactly — `truth` must match plain decode BY CONSTRUCTION
    if (c.final_logit_softcap > 0.0) {
        softcap4(s.mtp_logits, c.vocab_size, c.final_logit_softcap)
    }
    for (id in t.suppress) {
        s.mtp_logits[id] = -1.0e30
    }
}

// The tied-fp32 classifier (mm_fblob) and the GPU-resident plane have no batched form — those keep
// the per-row path.
def private mtp_cls_batch_ok(t : Model) : bool {
    let c = t.config
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    let tied_f32 = c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8
    return g_mtp_batch_cls && !tied_f32 && !moe_gpu_cls_on_gpu(t.wcls_off)
}

// One classifier pass for the whole verify window: normalize every row, then a single batched GEMM
// over the plane. Row 0 feeds the draft check, the last row feeds the next step — the same values
// the per-row path produces, from one weight stream instead of npos of them.
def private mtp_verify_logits(t : Model; var s : Session; npos : int64) {
    let c = t.config
    let dim = c.dim
    let vocab = c.vocab_size
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    s.mtp_norm_b |> scratch_resize(npos * dim)
    s.mtp_logits_b |> scratch_resize(npos * vocab)
    rms_batch(s.mtp_norm_b, s.x_b, t, t.rms_final_off, dim, npos)
    copy_floats(s.mtp_norm_b, 0l, s.mtp_h0, 0l, dim)   // = h_pos; a reject re-seeds the draft head from it
    mm_b_f(s, s.mtp_logits_b, t, cls_fmt, t.wcls_off, s.mtp_norm_b, dim, vocab, npos)
    copy_floats(s.mtp_logits_b, 0l, s.mtp_logits, 0l, vocab)
    copy_floats(s.mtp_logits_b, (npos - 1l) * vocab, s.logits, 0l, vocab)
    // mirror the plain classifier tail exactly — `truth` must match plain decode BY CONSTRUCTION
    if (c.final_logit_softcap > 0.0) {
        softcap4(s.mtp_logits, vocab, c.final_logit_softcap)
        softcap4(s.logits, vocab, c.final_logit_softcap)
    }
    for (id in t.suppress) {
        s.mtp_logits[id] = -1.0e30
        s.logits[id] = -1.0e30
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
    if (t.metal_blob && !empty(g_decode_override_name) && key_exists(g_mtp_spec_overrides, g_decode_override_name)) {
        // a blob model never runs the CPU spec rail — the active override's GPU twin owns the step
        return invoke(g_mtp_spec_overrides[g_decode_override_name], t, s, tok, accepted)
    }
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
    let batch_cls = mtp_cls_batch_ok(t)
    forward_prefill(t, s, s.mtp_vbatch, 2l, pos, batch_cls)
    if (batch_cls) {
        mtp_verify_logits(t, s, 2l)   // both rows from one classifier stream
    } else {
        mtp_verify_row0(t, s)
    }
    let truth = parallel_argmax(s.mtp_logits, c.vocab_size)
    if (truth == d) {
        s.mtp_accepted++
        accepted = d
        s.n_past = pos + 2l
        return true
    }
    // Reject — but the verify already produced what a plain step would: row 0's logits and KV are
    // correct, so re-forwarding discards real work. The re-forward is only needed to roll back
    // recurrent state the verify advanced past the draft; an arch without any skips it.
    if (c.recr_mask == 0ul) {
        copy_floats(s.mtp_logits, 0l, s.logits, 0l, c.vocab_size)
        // forward_prefill's tail stashed the LAST row's h — the draft's, now discarded — so re-seed
        // the draft head from row 0's normed hidden the verify's own classifier computed
        copy_floats(s.mtp_h0, 0l, s.mtp_h, 0l, c.dim)
        s.mtp_h_pos1 = pos + 1l
        s.n_past = pos + 1l
        return false
    }
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
    let np = long_length(prompt)
    var ids : array<int64>
    if (total <= 0l || np <= 0l) {
        return <- ids
    }
    ids |> reserve(total)
    for (i in range64(1l, np)) {   // generate()'s echo convention (incl. budget + BOS delimiter)
        if (long_length(ids) >= total || prompt[i] == 1l) {
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
    while (long_length(ids) < total) {
        break if (tok == 1l)   // BOS delimits sequences (generate()'s contract)
        ids |> push(tok)
        break if (long_length(ids) >= total)
        var acc = 0l
        if (mtp_spec_eval(t, s, tok, acc)) {
            break if (acc == 1l)   // BOS: generate() stops before emitting it — mirror that
            ids |> push(acc)
            break if (long_length(ids) >= total)
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

// Batch widths at or below this are speculative-verify shaped, so they follow the DECODE dispatch
// policy, not the prefill work threshold — see prefill_attention (lcpp runs n=1..3, vLLM's GLM 1).
let ATTN_NARROW_NPOS = 8l

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
var private g_forward_prof_on = false   // armed by forward_profile_reset; see prof_add
var g_prof_mark_disp = 0l
var g_prof_mark_chunks = 0l
var g_prof_mark_inline = 0l

def forward_profile_reset() {
    g_forward_prof_on = true
    g_forward_prof_acc |> clear()
    g_forward_prof_disp |> clear()
    g_forward_prof_chunks |> clear()
    g_forward_prof_inline |> clear()
    g_prof_mark_disp = get_disp_count()
    g_prof_mark_chunks = get_disp_chunks()
    g_prof_mark_inline = get_disp_inline()
}

// Sections are sequential and each span ends in exactly one prof_add, so the dispatch-counter
// deltas since the previous belong to `name`. Armed by forward_profile_reset — it used to run
// unconditionally: six string-keyed table ops per section, per layer, per token, always.
[cold_path]   // only reached once profiling is armed; the accumulate IS the cost of measuring
def private prof_add_armed(name : string; t0 : int64) {
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

def prof_add(name : string; t0 : int64) {
    return if (!g_forward_prof_on)
    prof_add_armed(name, t0)
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

//! Greedy argmax over the current logits; returns the winning token id.
def argmax(s : Session; vocab : int64) : int64 {
    // team-scanned (bit-identical to serial first-wins): 151-262k vocab scan was 210-284us/token idle
    return parallel_argmax(s.logits, vocab)
}

//! Greedy decode. While still inside `prompt_tokens`, the next token is forced from the prompt;
//! otherwise it's argmax. Returns emitted IDs (prompt echo minus BOS, then continuation; stops on BOS).
def generate(t : Model; var s : Session; prompt_tokens : array<int64>; steps : int) : array<int64> {
    var ids : array<int64>
    let n_prompt = long_length(prompt_tokens)
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
        if (long_length(ids) >= nsteps) {
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
    if (long_length(ids) >= nsteps) {
        return <- ids
    }
    var token = argmax(s, t.config.vocab_size)
    var apos = n_prompt
    var one : array<int64>
    one |> resize(1)
    while (long_length(ids) < nsteps) {
        if (token == 1l) {  // BOS delimits sequences
            break
        }
        ids |> push(token)
        if (long_length(ids) >= nsteps) {
            break
        }
        one[0] = token
        forward_batch(t, s, one, 1l, apos)
        apos ++
        token = argmax(s, t.config.vocab_size)
    }
    return <- ids
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

// ===== std/dense block kernels (prefill, batched) =====
// Batched counterparts to attention_std_decode / ffn_dense_decode; bit-for-bit identical to
// calling forward() per position in sequence. Q4 has no batched kernel, falls back to per-token.

// attn-norm → batched QKV(+bias) → qk/v-norm → RoPE, for npos rows of x_b into q_b/k_b/v_b — the
// position-free head of the std attention block, shared by prefill and (later) batched decode.
def attn_qkv_rope_prefix(t : Model; var s : Session; l : int64; npos : int64; start_pos : int64) {
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
def attn_out_suffix(t : Model; var s : Session; l : int64; npos : int64) {
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
[no_alloc, no_env, no_io]
def forward_prefill(t : Model; var s : Session; tokens : array<int64>; npos, start_pos : int64; skip_logits : bool = false) {
    let c = t.config
    if (npos <= 0l) {
        return
    }
    if (t.quant == QuantMode.q4_0) {   // legacy per-row tier, no batched kernels: its prefill IS the decode loop
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
        if (g_ple_gpu_gate_set && invoke(g_ple_gpu_gate, t, s, npos, start_pos)) {
            s.ple_gpu_tokens |> resize(int(npos))
            for (p in range64(npos)) {
                s.ple_gpu_tokens[p] = tokens[p]
            }
            s.ple_gpu_pending = true
        } else {
            ple_pre_prefill(t, s, tokens, npos)
        }
        prof_add("ple", ts_ple)
    }

    // spec on: the draft head's SEAM row (start_pos-1) must be written BEFORE the body — the body's
    // own tail overwrites mtp_h with THIS window's last hidden. Cold/foreign handoff just skips it.
    if (g_mtp_spec && c.n_layer_nextn > 0l && start_pos > 0l && s.mtp_h_pos1 == start_pos) {
        if (t.metal_blob) {
            if (!empty(g_decode_override_name) && key_exists(g_mtp_seam_overrides, g_decode_override_name)) {
                if (!invoke(g_mtp_seam_overrides[g_decode_override_name], t, s, tokens[0], start_pos)) {   // nolint:PERF026 — key_exists guards the line above, so the index cannot insert
                    // quality-only miss: the stale seam row dents draft acceptance until the next
                    // draft rewrites it (the verify keeps outputs exact) — loud so a dip is traceable
                    to_log(LOG_WARNING, "dasLLAMA: MTP seam row declined at pos {start_pos} — draft acceptance may dip this window\n")   // nolint:PERF028,PERF026 — fires only on a seam decline; loud by design so a quality dip is traceable
                }
            }
        } else {
            forward_mtp(t, s, tokens[0], start_pos - 1l, false)
        }
    }
    forward_prefill_body(t, s, npos, start_pos, skip_logits)
    // spec on: maintain the draft head's KV alongside the trunk's — CPU rail only (on a blob
    // model the GPU spec loop owns the draft slab; these hooks would CPU-touch blob planes)
    if (g_mtp_spec && c.n_layer_nextn > 0l && !t.metal_blob) {
        mtp_prefill_warm(t, s, tokens, npos, start_pos)
    }
}

// batched-prefill scratch sizing, shared by token and embedding entries. grow-only no-init:
// contents are stage output (fully overwritten); `delete` on grow skips the realloc's old-block
// copy + zero-fill (~5ms saved on a first 512-row prefill).
def scratch_resize(@scratch var a : array<auto(numT)>; need : int64) {
    if (long_length(a) < need) {
        delete a
    }
    a |> resize_no_init(need)
}

//! Unified batch entry: process `npos` tokens at start_pos..start_pos+npos-1. npos==1 dispatches to
//! forward() (decode); npos>1 to forward_prefill. Leaves s.logits at the last token, extends KV by npos.
[no_alloc, no_env, no_io]
def forward_batch(t : Model; var s : Session; tokens : array<int64>; npos, start_pos : int64) {
    if (npos == 1l) {
        forward(t, s, tokens[0], start_pos)
    } else {
        forward_prefill(t, s, tokens, npos, start_pos)
    }
}

// the size threshold. pre_post_norm/layer_out_scale are handled: the chain defers the residual add
// and the caller's serial tail runs post-norm and/or the fused add+scale after the rendezvous.
def fused_ffn_ok(t : Model; l : int64) : bool {
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

[no_alloc, no_env, no_io]
def forward_prefill_alloc(t : Model; var s : Session; npos, start_pos : int64) {
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
def forward_prefill_body(t : Model; var s : Session; npos, start_pos : int64; skip_logits : bool = false) {
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
        blob_only_panic(t, "prefill stack")
        // Metal-capable build about to chew a real prompt on the CPU stack — the silent profiling-time sink, not a serving path; fail loud unless CPU intent was declared
        if (npos > CPU_PREFILL_PANIC_NPOS && has_prefill_override("metal") && !cpu_prefill_allowed()) {
            panic("dasLLAMA: CPU batched prefill of {npos} tokens on a Metal-capable build (override `{g_prefill_override_name}` {empty(g_prefill_override_name) ? "not selected" : "declined"}) — select the metal prefill (set_metal_mode / DASLLAMA_PIN_PREFILL=metal), or declare CPU intent: allow_cpu_prefill() / DASLLAMA_CPU_PREFILL=1")
        }
        for (l in range64(c.n_layers)) {
            trace_marker("layer", int(l))   // nolint:PERF026 — gated on g_trace_tags; the name->id table lookup is the trace's own cost
            trace_tag(TRACE_TAG_ATTN_STD)   // per-lane trace: color the prefill attention section
            t.blocks.attn_prefill(t, s, l, npos, start_pos)
            trace_tag(TRACE_TAG_FFN_STD)
            t.blocks.ffn_prefill(t, s, l, npos)
        }
    }

    // final rmsnorm + classifier on the LAST position only (that's what we sample next)
    return if (stack_done && g_prefill_logits_done)   // the override already produced s.logits
    blob_only_panic(t, "prefill final-norm/classifier step")
    let ts_final = ref_time_ticks()
    let last = npos - 1l
    rms_at(s.xb, t, t.rms_final_off, s.x_b, last * dim, dim)
    if (c.n_layer_nextn > 0l) {   // stash the last position's h_nextn for the MTP draft head
        copy_floats(s.xb, 0l, s.mtp_h, 0l, dim)
        s.mtp_h_pos1 = start_pos + last + 1l
    }
    if (skip_logits) {   // MTP verify: the caller runs ONE batched classifier covering every row
        prof_add("final", ts_final)
        return
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
