options gen2

module dasllama_common shared public

require daslib/fio
require daslib/array_boost // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs and false-flags this as unused
require dasllama/dasllama_math
require dasllama/dasllama_math_default  // registers the portable Q8·Q8 backend at [init] (the fallback everywhere)
require dasllama/dasllama_math_aarch64_neon  // registers the arm64 SDOT Q8·Q8 backends at [init]; no-op off-ARM
require dasllama/dasllama_quant
require dasllama/dasllama_gguf
require dasllama/dasllama_tokenizer  // Model owns its tokenizer; load_model populates it
require dasllama/dasllama_par  // maybe_parallel_for (+ get_total_hw_jobs): thread the prefill attention over heads
require math
require daslib/random  // int4 RNG state + random_float for token sampling
require daslib/strings_boost  // join() for the arch-registry diagnostic

// dasLLAMA inference engine. A model is split into two regions by role: a small fp32 aux blob
// (the embedding table + the norm weights, which forward always reads as fp32) and the big 2D
// weight matrices (attn q/k/v/o, ffn gate/down/up, untied classifier). Only the big weights are
// quantized, and only ONE representation is held — so a Q8 model never materializes the fp32
// weights ("load as stored"). All big-weight arrays are reserve()'d to exact size to avoid the
// allocator's power-of-2 rounding.

//! Which weight representation forward routes its 2D matmuls through. fp32 is the default;
//! q8/q4 select the quantized kernels (and the matching big-weight storage).
enum QuantMode {
    fp32
    q8
    q4
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

//! Prefill attention algorithm (A/B selectable; all variants kept). `classic` streams the full causal
//! K/V range from the cache per query — threaded over heads, O(npos²) with K/V re-read every query.
//! `blocked` query-blocks so each K/V tile is loaded once and reused across the whole block, cutting
//! K/V cache traffic ~ATTN_BLOCK_Q×; it is BIT-IDENTICAL to classic (same per-query softmax + same
//! key-order V accumulation). `flash` is tiled flash attention — QKᵀ and P·V as register-tiled fp32
//! GEMMs (gemm_f32) with online softmax, the form that avoids the per-pair horizontal reduce; NOT
//! bit-identical (online-softmax FP reorder), validated token-for-token vs the oracle + logit-diff.
//! Select via set_prefill_attn_mode (default flash).
enum AttnPrefillMode {
    classic
    blocked
    flash
}

//! Model hyperparameters (the 7-int checkpoint header plus derived sizes).
struct Config {
    dim : int64
    hidden_dim : int64
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
    // gpt-oss (all loader-detected from tensor presence, except the norm-name flag which the arch sets)
    attn_sinks : bool = false          // per-head sink logit joins the attention softmax denominator
    attn_out_bias : bool = false       // additive bias on the attention output projection
    moe_router_bias : bool = false     // ffn_gate_inp.bias on the router logits
    moe_exps_bias : bool = false       // per-expert ffn_{gate,up,down}_exps.bias
    post_attn_is_ffn_norm : bool = false // the pre-FFN norm tensor is named post_attention_norm (NOT Gemma2's extra post-norm)
}

// Swappable block kernels — one per (attention|ffn) × (decode|prefill). Decode is the fused
// single-token path (forward); prefill is the batched path (forward_prefill). A new block (MoE,
// QK-norm attention, …) registers a different function here without editing the forward loops.
// Declared before Model (which holds an ArchBlocks by value) so the AOT C++ emitter sees the
// complete type first; the fn-ptr signatures forward-reference Model/Session, which is fine.
typedef AttnDecodeFn  = function<(t : Model; var s : Session; l : int64; pos : int64) : void>
typedef FfnDecodeFn   = function<(t : Model; var s : Session; l : int64) : void>
typedef AttnPrefillFn = function<(t : Model; var s : Session; l : int64; npos : int64; start_pos : int64) : void>
typedef FfnPrefillFn  = function<(t : Model; var s : Session; l : int64; npos : int64) : void>

//! The copyable hot-path block pointers, resolved onto the Model at load and called per layer.
struct ArchBlocks {
    attn_decode : AttnDecodeFn
    ffn_decode : FfnDecodeFn
    attn_prefill : AttnPrefillFn
    ffn_prefill : FfnPrefillFn
}

//! The whole model. `fblob` holds the fp32 embedding + norm weights at the *_off offsets below;
//! the big 2D weights live in exactly one of wblob (fp32) / qblob+qscales (Q8) / q4blob+q4scales
//! (Q4) per `quant`, at the w*_off offsets (parallel layout: the same weight-element offset indexes
//! all three — Q8 scales at off/32, Q4 bytes at off/2). For shared_weights the classifier reads the
//! fp32 embedding from fblob, so wcls_off is unused.
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
    // big 2D weights — exactly one representation populated per `quant`
    wblob : array<float>
    qblob : array<int8>
    qscales : array<float>
    q4blob : array<uint8>
    q4scales : array<float>
    // Attention projections use PER-LAYER offsets — tensor shapes differ per layer class on gemma4
    // (SWA 16Q/8KV heads of 256 vs global 16Q/1KV of 512); uniform arches fill the flat strides.
    // wv_offs[l] = -1 marks a layer with no attn_v tensor (V reuses the pre-norm K projection).
    wq_offs : array<int64>
    wk_offs : array<int64>
    wv_offs : array<int64>
    wo_offs : array<int64>
    w1_off : int64
    w2_off : int64
    w3_off : int64
    // MoE (valid when config.n_expert > 0; the dense w1/w2/w3 regions are not allocated then).
    // Router matrices + shared-expert gate vectors are small fp32 -> fblob; the routed expert
    // stacks (expert-major within each layer) and the shared-expert 2D weights -> big-weight blob
    router_off : int64      // fblob: layers x n_expert x dim
    shexp_gate_off : int64  // fblob: layers x dim (one sigmoid-gate vector per layer)
    // gpt-oss MoE biases (fblob, valid per the config.moe_*_bias flags): router logits bias +
    // per-expert gate/down/up biases (expert-major within each layer, like the we* stacks)
    router_b_off : int64    // fblob: layers x n_expert
    web1_off : int64        // ffn_gate_exps.bias: layers x n_expert x n_ff_exp
    web2_off : int64        // ffn_down_exps.bias: layers x n_expert x dim
    web3_off : int64        // ffn_up_exps.bias:   layers x n_expert x n_ff_exp
    // gpt-oss per-head attention-sink logits (fblob, valid when config.attn_sinks)
    sinks_off : int64       // fblob: layers x n_heads
    we1_off : int64         // ffn_gate_exps: layers x n_expert x (dim x n_ff_exp)
    we2_off : int64         // ffn_down_exps: layers x n_expert x (n_ff_exp x dim)
    we3_off : int64         // ffn_up_exps:   layers x n_expert x (dim x n_ff_exp)
    wsh1_off : int64        // ffn_gate_shexp: layers x (dim x n_ff_shexp)
    wsh2_off : int64        // ffn_down_shexp: layers x (n_ff_shexp x dim)
    wsh3_off : int64        // ffn_up_shexp:   layers x (dim x n_ff_shexp)
    wcls_off : int64
    quant : QuantMode
    rope_freqs : array<float>   // per-pair RoPE scaling (Llama-3.1 NTK-by-parts); empty = none
    // QKV projection biases (Qwen2) — fp32, per layer (q: layers×dim, k/v: layers×kv_dim);
    // empty unless config.attn_qkv_bias
    bq : array<float>
    bk : array<float>
    bv : array<float>
    // attention output-projection bias (gpt-oss) — fp32, layers×dim; empty unless config.attn_out_bias
    bo : array<float>
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
    system : ChatTurn = ChatTurn()
    user : ChatTurn = ChatTurn()
    assistant_open : ChatTurn = ChatTurn()
    stop : array<string>
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
}

struct private LayoutSizes {
    fblob_n : int64   // element count of the fp32 aux region
    wblob_n : int64   // element count of the big-weight region
}

// Fill both offset tables from the config; return the element counts of each region. The big-weight
// offsets are 32-aligned (every dim/kv/hidden is a multiple of 32), so Q8/Q4 scale and byte indices
// divide cleanly. Attention tensors get PER-LAYER offsets (shapes differ per layer class on gemma4);
// each kind's layers stay contiguous, so uniform arches reproduce the old flat strides exactly.
def private layout_offsets(var t : Model) : LayoutSizes {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    let layers = c.n_layers
    var fo = 0l
    t.tok_emb_off = fo
    fo += c.vocab_size * dim
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
    if (c.n_expert > 0l) {  // MoE: per-layer router matrices + shared-expert gate vectors, fp32
        t.router_off = fo
        fo += layers * c.n_expert * dim
        if (c.n_ff_shexp > 0l) {
            t.shexp_gate_off = fo
            fo += layers * dim
        }
        if (c.moe_router_bias) {  // gpt-oss: router logits bias
            t.router_b_off = fo
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
    }
    t.rms_final_off = fo
    fo += dim
    var wo = 0l
    t.wq_offs |> clear()
    t.wq_offs |> reserve(layers)
    for (l in range64(layers)) {
        t.wq_offs |> push(wo)
        wo += dim * layer_qd(c, l)
    }
    t.wk_offs |> clear()
    t.wk_offs |> reserve(layers)
    for (l in range64(layers)) {
        t.wk_offs |> push(wo)
        wo += dim * layer_kv_dim(c, l)
    }
    t.wv_offs |> clear()
    t.wv_offs |> reserve(layers)
    for (l in range64(layers)) {
        if (((c.v_from_k_mask >> uint64(l)) & 1ul) != 0ul) {
            t.wv_offs |> push(-1l)   // no attn_v tensor: V reuses the pre-norm K projection
        } else {
            t.wv_offs |> push(wo)
            wo += dim * layer_kv_dim(c, l)
        }
    }
    t.wo_offs |> clear()
    t.wo_offs |> reserve(layers)
    for (l in range64(layers)) {
        t.wo_offs |> push(wo)
        wo += layer_qd(c, l) * dim
    }
    if (c.n_expert > 0l) {
        // MoE: routed expert stacks — expert e of layer l is a normal 2D matrix at
        // we*_off + (l*n_expert + e) * (dim*n_ff_exp), so the existing kernels apply per expert
        let ege = dim * c.n_ff_exp   // per-expert element count (same for gate/up and down)
        t.we1_off = wo
        wo += layers * c.n_expert * ege
        t.we2_off = wo
        wo += layers * c.n_expert * ege
        t.we3_off = wo
        wo += layers * c.n_expert * ege
        if (c.n_ff_shexp > 0l) {
            let she = dim * c.n_ff_shexp
            t.wsh1_off = wo
            wo += layers * she
            t.wsh2_off = wo
            wo += layers * she
            t.wsh3_off = wo
            wo += layers * she
        }
    } else {
        t.w1_off = wo
        wo += layers * dim * hidden
        t.w2_off = wo
        wo += layers * hidden * dim
        t.w3_off = wo
        wo += layers * dim * hidden
    }
    if (c.shared_weights) {
        t.wcls_off = 0l
    } else {
        t.wcls_off = wo
        wo += c.vocab_size * dim
    }
    return LayoutSizes(fblob_n = fo, wblob_n = wo)
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
            let hdr = temp_array(reinterpret<int?>(addr(bytes[0])), HEADER_INTS, type<int>)
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
            let src = temp_array(reinterpret<float?>(addr(bytes[HEADER_BYTES])), n_floats, type<float>)
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
            copy_floats(src, fc, t.wblob, t.w1_off, layers * dim * hidden)
            fc += layers * dim * hidden
            copy_floats(src, fc, t.wblob, t.w2_off, layers * hidden * dim)
            fc += layers * hidden * dim
            copy_floats(src, fc, t.wblob, t.w3_off, layers * dim * hidden)
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
// and quantize into the shared blob. `woff` is the weight-element offset; `scratch` is reused.
def private load_big(m : GGUFMeta; bytes : array<uint8> | #; name : string; var t : Model; woff, n : int64; var scratch : array<float>; src_off : int64 = 0l) {
    if (t.quant == QuantMode.q8) {
        if (gguf_tensor_type(m, name) == GGML_TYPE_Q8_0) {
            gguf_transcode_q8_0(m, bytes, name, t.qblob, t.qscales, woff, woff / 32l, n, src_off)
        } else {
            scratch |> resize(n)
            gguf_read_tensor_f32(m, bytes, name, scratch, 0l, n, src_off)
            quantize_q8_0_into(scratch, n, t.qblob, t.qscales, woff, woff / 32l)
        }
    } elif (t.quant == QuantMode.q4) {
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

// ===== Arch registry =====
// Each architecture self-registers its config setter, block kernels, and chat template at [init]
// (see the register_arch_* functions after the forward pass). load_gguf looks the arch up by its
// GGUF `general.architecture` string — an arch we can't run panics, better than feeding weights
// through the wrong path.

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

// Look the arch up, apply its config flags, and copy its (copyable) block kernels onto the model.
// The full descriptor stays in the registry; only the hot-path pointers ride on the Model.
def private resolve_arch(var t : Model; arch : string) {
    if (!key_exists(g_arch_registry, arch)) {
        let registered = join(arch_names(), ", ")
        panic("dasLLAMA: unsupported architecture '{arch}' (registered: {registered})")
    }
    t.arch = arch
    g_arch_registry[arch].configure(t.config)
    t.blocks = g_arch_registry[arch].blocks
}

//! Load a llama-architecture GGUF into the split layout at the requested precision. The big weights
// Repack every Q8 2D weight tensor in qblob/qscales into the active backend's interleaved layout, IN
// PLACE, once at load. Mirrors forward()'s per-tensor offsets/shapes exactly; the embedding + norms
// live in fblob and are never touched. Called only after select_matmul_backend_for_load() picks a
// repack backend (arm64 laneq); the
// repack pointer is otherwise identity, so this would be a no-op regardless.
def private repack_q8_weights(var t : Model) {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    for (l in range64(c.n_layers)) {
        let kv = layer_kv_dim(c, l)
        let qd = layer_qd(c, l)
        repack_q8q8_weight(t.qblob, t.qscales, t.wq_offs[l], dim, qd)
        repack_q8q8_weight(t.qblob, t.qscales, t.wk_offs[l], dim, kv)
        if (t.wv_offs[l] >= 0l) {
            repack_q8q8_weight(t.qblob, t.qscales, t.wv_offs[l], dim, kv)
        }
        repack_q8q8_weight(t.qblob, t.qscales, t.wo_offs[l], qd, dim)
        if (c.n_expert > 0l) {   // MoE: each expert slice is its own 2D tensor, repacked per its shape
            let ege = dim * c.n_ff_exp
            for (e in range64(c.n_expert)) {
                let base = (l * c.n_expert + e) * ege
                repack_q8q8_weight(t.qblob, t.qscales, t.we1_off + base, dim, c.n_ff_exp)
                repack_q8q8_weight(t.qblob, t.qscales, t.we2_off + base, c.n_ff_exp, dim)
                repack_q8q8_weight(t.qblob, t.qscales, t.we3_off + base, dim, c.n_ff_exp)
            }
            if (c.n_ff_shexp > 0l) {
                let she = dim * c.n_ff_shexp
                repack_q8q8_weight(t.qblob, t.qscales, t.wsh1_off + l * she, dim, c.n_ff_shexp)
                repack_q8q8_weight(t.qblob, t.qscales, t.wsh2_off + l * she, c.n_ff_shexp, dim)
                repack_q8q8_weight(t.qblob, t.qscales, t.wsh3_off + l * she, dim, c.n_ff_shexp)
            }
        } else {
            repack_q8q8_weight(t.qblob, t.qscales, t.w1_off + l * dim * hidden, dim, hidden)
            repack_q8q8_weight(t.qblob, t.qscales, t.w2_off + l * hidden * dim, hidden, dim)
            repack_q8q8_weight(t.qblob, t.qscales, t.w3_off + l * dim * hidden, dim, hidden)
        }
    }
    if (!c.shared_weights) {
        repack_q8q8_weight(t.qblob, t.qscales, t.wcls_off, dim, c.vocab_size)
    }
}

//! go straight into `mode`'s storage (pre-quantized tensors transcode block-for-block with no fp32
//! round-trip; F16/F32 tensors dequant one at a time through a small reused scratch), so a Q8/Q4
//! load never holds the dense fp32 weights. The embedding + norms stay fp32 in fblob regardless.
//! Config comes from metadata; vocab is derived from token_embd (some files omit llama.vocab_size).
def load_gguf(path : string; mode : QuantMode = QuantMode.fp32) : Model {
    var t = Model()
    t.quant = mode
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let m <- parse_gguf_meta(bytes)
        let arch = gguf_str(m, bytes, "general.architecture")
        resolve_arch(t, arch)   // registry lookup: apply config flags + bind the arch's block kernels
        if (gguf_has(m, "tokenizer.chat_template")) {
            t.chat_template_str = gguf_str(m, bytes, "tokenizer.chat_template")
        }
        // GGUF uses the architecture name as the metadata key prefix (llama.*, qwen2.*, gemma2.*, ...)
        let dim = gguf_int(m, bytes, "{arch}.embedding_length")
        let hidden = gguf_int(m, bytes, "{arch}.feed_forward_length")
        let layers = gguf_int(m, bytes, "{arch}.block_count")
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
        // configure (Gemma2: 4096 / alternating; Gemma3: 512 or 1024 / 5-local-1-global; gemma4
        // stores the pattern as an explicit per-layer bool array -> swa_mask). Resolved BEFORE the
        // kv-head classes below, which key off layer_is_sliding. configure()'s values stay as the
        // fallback for files without the keys.
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
        // KV-head count: scalar (uniform), or gemma4's per-layer array — which must collapse to the
        // TWO layer classes (one value on sliding layers, one on global) the engine models
        if (gguf_is_array(m, "{arch}.attention.head_count_kv")) {
            let kvh <- gguf_int_array(m, bytes, "{arch}.attention.head_count_kv")
            var full_v = -1l
            var swa_v = -1l
            for (l in range64(layers)) {
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
        // the engine ropes over the full head — refuse partial-rotary GGUFs instead of mis-running them
        if (gguf_has(m, "{arch}.rope.dimension_count")
                && gguf_int(m, bytes, "{arch}.rope.dimension_count") != t.config.head_size) {
            panic("dasLLAMA: partial rotary (rope.dimension_count != head_size) is not supported")
        }
        if (gguf_has(m, "{arch}.rope.dimension_count_swa")) {
            let swa_hs = t.config.head_size_swa > 0l ? t.config.head_size_swa : t.config.head_size
            if (gguf_int(m, bytes, "{arch}.rope.dimension_count_swa") != swa_hs) {
                panic("dasLLAMA: partial rotary (rope.dimension_count_swa != the sliding head size) is not supported")
            }
        }
        // honest gates for gemma4 features beyond this wave (E-series shared KV / PLE, the MoE 26B)
        if (gguf_has(m, "{arch}.attention.shared_kv_layers")
                && gguf_int(m, bytes, "{arch}.attention.shared_kv_layers") > 0l) {
            panic("dasLLAMA: shared KV layers (Gemma-4 E-series) are not yet supported")
        }
        if (gguf_has(m, "{arch}.embedding_length_per_layer_input")
                && gguf_int(m, bytes, "{arch}.embedding_length_per_layer_input") > 0l) {
            panic("dasLLAMA: per-layer embeddings (Gemma-4 E-series PLE) are not yet supported")
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
            // shared expert: present iff its tensors are (qwen2moe always ships one)
            if (gguf_find_tensor(m, "blk.0.ffn_up_shexp.weight") >= 0) {
                t.config.n_ff_shexp = (gguf_has(m, "{arch}.expert_shared_feed_forward_length")
                    ? gguf_int(m, bytes, "{arch}.expert_shared_feed_forward_length")
                    : hidden)
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
            // expert-selection bias the core doesn't apply — refuse instead of silently dropping it
            if (gguf_find_tensor(m, "blk.0.exp_probs_b.bias") >= 0) {
                panic("dasLLAMA: expert selection bias (exp_probs_b, DeepSeek-V3) is not yet supported")
            }
            // router / per-expert biases (gpt-oss) — detected from the tensor table; the gate/up
            // pair always ships together, so probing ffn_up_exps.bias covers all three stacks
            t.config.moe_router_bias = gguf_find_tensor(m, "blk.0.ffn_gate_inp.bias") >= 0
            t.config.moe_exps_bias = gguf_find_tensor(m, "blk.0.ffn_up_exps.bias") >= 0
        } elif (gguf_has(m, "{arch}.expert_count") && gguf_int(m, bytes, "{arch}.expert_count") > 0l) {
            panic("dasLLAMA: MoE expert blocks are not supported for arch '{arch}'")
        }
        // attention output-projection bias + per-head sink logits (gpt-oss) — tensor-detected
        t.config.attn_out_bias = gguf_find_tensor(m, "blk.0.attn_output.bias") >= 0
        t.config.attn_sinks = gguf_find_tensor(m, "blk.0.attn_sinks.weight") >= 0
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
        }
        // Phi3 LongRoPE magnitude factor (attn_factor) scales the rope output; absent (=1.0) elsewhere
        let mskey = "{arch}.rope.scaling.attn_factor"
        t.config.rope_mscale = gguf_has(m, mskey) ? gguf_f32(m, bytes, mskey) : 1.0
        // YaRN (gpt-oss: factor 32 over a 4096 original context). The whole scheme folds into the
        // existing per-pair machinery: rope_yarn's theta = extrap·(fscale·(1-mix) + mix) is a per-pair
        // EFFECTIVE position scale, so we synthesize rope_freqs[j] = 1/eff_j (the factors DIVIDE θ),
        // keep rope_freq_scale = 1, and fold the magnitude correction 1 + 0.1·ln(factor) into
        // rope_mscale. ext_factor = 1 and attn_factor = 1 (the llama.cpp runtime defaults for YARN).
        if (gguf_has(m, "{arch}.rope.scaling.type") && gguf_str(m, bytes, "{arch}.rope.scaling.type") == "yarn") {
            if (!empty(t.rope_freqs)) {
                panic("dasLLAMA: YaRN combined with per-pair rope factors is not supported")
            }
            let yfactor = gguf_f32(m, bytes, "{arch}.rope.scaling.factor")
            let yorig = (gguf_has(m, "{arch}.rope.scaling.original_context_length")
                ? gguf_int(m, bytes, "{arch}.rope.scaling.original_context_length") : seq_len)
            let fscale = 1.0 / yfactor
            // NTK-by-parts correction band (ggml_rope_yarn_corr_dims; beta_fast/slow defaults 32/1):
            // pairs below `lo` rotate at full frequency (extrapolate), above `hi` fully interpolate
            let hs = t.config.head_size
            let denom = 2.0 * log(t.config.rope_freq_base)
            let lo = max(0.0, floor(float(hs) * log(float(yorig) / (32.0 * 2.0 * PI)) / denom))
            let hi = min(float(hs - 1l), ceil(float(hs) * log(float(yorig) / (2.0 * PI)) / denom))
            t.rope_freqs |> resize(hs / 2l)
            for (j in range64(hs / 2l)) {
                let ramp = 1.0 - clamp((float(j) - lo) / max(0.001, hi - lo), 0.0, 1.0)  // rope_yarn_ramp(lo, hi, 2j)
                t.rope_freqs[j] = 1.0 / (fscale + ramp * (1.0 - fscale))
            }
            t.config.rope_mscale *= 1.0 + 0.1 * log(1.0 / fscale)
        }
        // a separate output.weight means the classifier is not tied to the embeddings
        t.config.shared_weights = gguf_find_tensor(m, "output.weight") < 0
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
                if (gguf_find_tensor(m, "blk.{l}.attn_v.weight") < 0) {
                    if (l >= 64l) {
                        panic("dasLLAMA: V-from-K layers need n_layers <= 64 (got layer {l})")
                    }
                    t.config.v_from_k_mask |= 1ul << uint64(l)
                }
            }
        }
        // per-layer output scalars (gemma4 layer_output_scale) — presence detected from the tensors
        t.config.layer_out_scale = gguf_find_tensor(m, "blk.0.layer_output_scale.weight") >= 0
        let kv = t.config.kv_dim
        let qd = t.config.n_heads * t.config.head_size   // query width (== dim except Gemma2)
        let sz = layout_offsets(t)
        // fp32 aux (embedding + norms) is always present; the big-weight storage matches `mode`
        reserve_resize(t.fblob, sz.fblob_n)
        if (mode == QuantMode.q8) {
            t.qblob |> reserve(sz.wblob_n)
            t.qblob |> resize(sz.wblob_n)
            t.qscales |> reserve(sz.wblob_n / 32l)
            t.qscales |> resize(sz.wblob_n / 32l)
        } elif (mode == QuantMode.q4) {
            t.q4blob |> reserve(sz.wblob_n / 2l)
            t.q4blob |> resize(sz.wblob_n / 2l)
            t.q4scales |> reserve(sz.wblob_n / 32l)
            t.q4scales |> resize(sz.wblob_n / 32l)
        } else {
            reserve_resize(t.wblob, sz.wblob_n)
        }
        // embedding + norms -> fblob (decoded to fp32 from whatever the disk type is)
        gguf_read_tensor_f32(m, bytes, "token_embd.weight", t.fblob, t.tok_emb_off, vocab * dim)
        // gpt-oss names its pre-FFN norm post_attention_norm (same graph role as ffn_norm)
        let ffn_norm_name = t.config.post_attn_is_ffn_norm ? "post_attention_norm" : "ffn_norm"
        for (l in range64(layers)) {
            gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_norm.weight", t.fblob, t.rms_att_off + l * dim, dim)
            gguf_read_tensor_f32(m, bytes, "blk.{l}.{ffn_norm_name}.weight", t.fblob, t.rms_ffn_off + l * dim, dim)
            if (t.config.pre_post_norm) {  // Gemma2: post-attn + post-FFN norms (already +1-baked in the GGUF)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.post_attention_norm.weight", t.fblob, t.rms_post_att_off + l * dim, dim)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.post_ffw_norm.weight", t.fblob, t.rms_post_ffn_off + l * dim, dim)
            }
            if (t.config.qk_norm) {  // Qwen3/gemma4: per-head Q/K norms (head_size(l) each)
                let hs = layer_head_size(t.config, l)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_q_norm.weight", t.fblob, t.rms_q_offs[l], hs)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_k_norm.weight", t.fblob, t.rms_k_offs[l], hs)
            }
            if (t.config.layer_out_scale) {  // gemma4: one output scalar per layer
                gguf_read_tensor_f32(m, bytes, "blk.{l}.layer_output_scale.weight", t.fblob, t.out_scale_off + l, 1l)
            }
            if (t.config.attn_sinks) {  // gpt-oss: one sink logit per head
                gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_sinks.weight", t.fblob, t.sinks_off + l * n_heads, n_heads)
            }
        }
        if (t.config.v_norm) {  // the weightless V-norm's synthetic all-ones weight vector
            for (i in range64(max_head_size(t.config))) {
                t.fblob[t.rms_ones_off + i] = 1.0
            }
        }
        gguf_read_tensor_f32(m, bytes, "output_norm.weight", t.fblob, t.rms_final_off, dim)
        // big 2D weights -> active precision. ggml stores them row-major [out x in] = our convention.
        var scratch : array<float>
        for (l in range64(layers)) {
            let kv_l = layer_kv_dim(t.config, l)
            let qd_l = layer_qd(t.config, l)
            if (t.config.fused_qkv) {
                // Phi3: attn_qkv rows are [q (qd) ; k (kv) ; v (kv)], ffn_up rows are [gate ; up] (hidden each).
                // src_off slices the fused tensor, preserving its Q8 blocks (no fp32 requantize).
                load_big(m, bytes, "blk.{l}.attn_qkv.weight", t, t.wq_offs[l], dim * qd_l, scratch, 0l)
                load_big(m, bytes, "blk.{l}.attn_qkv.weight", t, t.wk_offs[l], dim * kv_l, scratch, dim * qd_l)
                load_big(m, bytes, "blk.{l}.attn_qkv.weight", t, t.wv_offs[l], dim * kv_l, scratch, dim * qd_l + dim * kv_l)
            } else {
                load_big(m, bytes, "blk.{l}.attn_q.weight", t, t.wq_offs[l], dim * qd_l, scratch)
                load_big(m, bytes, "blk.{l}.attn_k.weight", t, t.wk_offs[l], dim * kv_l, scratch)
                if (t.wv_offs[l] >= 0l) {   // absent on gemma4 global layers (V reuses the K projection)
                    load_big(m, bytes, "blk.{l}.attn_v.weight", t, t.wv_offs[l], dim * kv_l, scratch)
                }
            }
            load_big(m, bytes, "blk.{l}.attn_output.weight", t, t.wo_offs[l], qd_l * dim, scratch)
            if (t.config.n_expert > 0l) {
                // MoE: fp32 router (+ shared-expert gate vector) -> fblob; the 3D expert stacks are
                // expert-major on disk, so ONE contiguous load lands every expert's 2D matrix at its
                // we*_off + (l*n_expert + e) * (dim*n_ff_exp) slot
                let ne = t.config.n_expert
                let ege = dim * t.config.n_ff_exp
                gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_gate_inp.weight", t.fblob, t.router_off + l * ne * dim, ne * dim)
                if (t.config.moe_router_bias) {
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_gate_inp.bias", t.fblob, t.router_b_off + l * ne, ne)
                }
                load_big(m, bytes, "blk.{l}.ffn_gate_exps.weight", t, t.we1_off + l * ne * ege, ne * ege, scratch)
                load_big(m, bytes, "blk.{l}.ffn_down_exps.weight", t, t.we2_off + l * ne * ege, ne * ege, scratch)
                load_big(m, bytes, "blk.{l}.ffn_up_exps.weight", t, t.we3_off + l * ne * ege, ne * ege, scratch)
                if (t.config.moe_exps_bias) {  // per-expert bias stacks, expert-major on disk like the weights
                    let nfe = t.config.n_ff_exp
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_gate_exps.bias", t.fblob, t.web1_off + l * ne * nfe, ne * nfe)
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_down_exps.bias", t.fblob, t.web2_off + l * ne * dim, ne * dim)
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_up_exps.bias", t.fblob, t.web3_off + l * ne * nfe, ne * nfe)
                }
                if (t.config.n_ff_shexp > 0l) {
                    let she = dim * t.config.n_ff_shexp
                    gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_gate_inp_shexp.weight", t.fblob, t.shexp_gate_off + l * dim, dim)
                    load_big(m, bytes, "blk.{l}.ffn_gate_shexp.weight", t, t.wsh1_off + l * she, she, scratch)
                    load_big(m, bytes, "blk.{l}.ffn_down_shexp.weight", t, t.wsh2_off + l * she, she, scratch)
                    load_big(m, bytes, "blk.{l}.ffn_up_shexp.weight", t, t.wsh3_off + l * she, she, scratch)
                }
            } elif (t.config.fused_qkv) {
                load_big(m, bytes, "blk.{l}.ffn_up.weight", t, t.w1_off + l * dim * hidden, dim * hidden, scratch, 0l)
                load_big(m, bytes, "blk.{l}.ffn_up.weight", t, t.w3_off + l * dim * hidden, dim * hidden, scratch, dim * hidden)
                load_big(m, bytes, "blk.{l}.ffn_down.weight", t, t.w2_off + l * hidden * dim, hidden * dim, scratch)
            } else {
                load_big(m, bytes, "blk.{l}.ffn_gate.weight", t, t.w1_off + l * dim * hidden, dim * hidden, scratch)
                load_big(m, bytes, "blk.{l}.ffn_up.weight", t, t.w3_off + l * dim * hidden, dim * hidden, scratch)
                load_big(m, bytes, "blk.{l}.ffn_down.weight", t, t.w2_off + l * hidden * dim, hidden * dim, scratch)
            }
        }
        if (!t.config.shared_weights) {
            load_big(m, bytes, "output.weight", t, t.wcls_off, vocab * dim, scratch)
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
        // arm64: switch to the laneq backend and repack the just-loaded Q8 weights into its interleaved
        // layout. Returns false off-ARM (portable stays, weights stay row-major).
        if (mode == QuantMode.q8 && select_matmul_backend_for_load()) {
            repack_q8_weights(t)
        }
    }
    fclose(f)
    return <- t
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
}

//! Quantize a fp32-loaded model's big weights to Q4_0 in place and switch forward onto the Q4 path.
//! Same fp32-required / wblob-kept contract as quantize_weights.
def quantize_weights_q4(var t : Model) {
    let n = int64(length(t.wblob))
    var qt <- quantize_q4_0(t.wblob, n)
    t.q4blob <- qt.q
    t.q4scales <- qt.scales
    t.quant = QuantMode.q4
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
    total : int64
}

def footprint(t : Model) : MemFootprint {
    let aux = int64(length(t.fblob)) * 4l
    let wf = int64(length(t.wblob)) * 4l
    let q8 = int64(length(t.qblob))               // int8 -> 1 byte each
    let q8s = int64(length(t.qscales)) * 4l
    let q4 = int64(length(t.q4blob))              // uint8 (two nibbles) -> 1 byte each
    let q4s = int64(length(t.q4scales)) * 4l
    return MemFootprint(aux_fp32_bytes = aux, weight_fp32_bytes = wf, q8_bytes = q8, q8_scale_bytes = q8s,
                        q4_bytes = q4, q4_scale_bytes = q4s, total = aux + wf + q8 + q8s + q4 + q4s)
}

//! Per-step scratch buffers and the KV cache (allocated once, reused every token).
struct Session {
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
    att : array<float>        // attention scores for one head (seq_len) — the per-token forward() path
    att_b : array<float>      // batched prefill attention scores, one (start_pos+npos)-wide row PER HEAD
                              // so the head loop threads with no shared scratch (sized in forward_prefill)
    logits : array<float>     // output logits (vocab_size)
    xq : array<int8>          // activation quantized to Q8 (sized max(dim, hidden_dim)) for the Q8·Q8 kernel
    xs : array<float>         // per-block activation scales (sized max(dim, hidden_dim) / 32)
    key_cache : array<float>  // (n_layers * seq_len * kv_dim)
    value_cache : array<float>
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
    // flash-attention per-head packing scratch (empty unless AttnPrefillMode.flash; sized in
    // forward_prefill to n_heads × tile, so each head's tile work is contention-free when threaded)
    fa_q : array<float>       // packed Q tile [n_heads × QT × head_size]
    fa_k : array<float>       // packed Kᵀ tile [n_heads × head_size × KV]
    fa_v : array<float>       // packed V tile  [n_heads × KV × head_size]
    fa_kq : array<float>      // score / prob tile [n_heads × QT × KV]
    fa_vkq : array<float>     // output accumulator [n_heads × QT × head_size]
    // RoPE cos/sin table (empty until forward_prefill builds it once per prefill, sized npos × head_size/2).
    // angle = pos*freq is identical across all heads AND layers, so cos/sin is precomputed once here
    // instead of recomputed per head/layer. Kept LOCAL (run-state, not a module global) so jobque
    // fork-context cloning stays cheap — fork heaps copy module/[init] data, not the run state.
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
}

//! Allocate the run state sized to a model.
def make_run_state(c : Config) : Session {
    var s : Session
    // qd = n_heads * head_size, the query / attention-output width. == dim for the llama family,
    // but Gemma2 has qd (2048) != dim (2304), so q and the attention scratch xb size to max(dim, qd).
    // Heterogeneous arches (gemma4) size to the larger layer class; the KV cache packs each layer's
    // own kv_dim (see kv_cache_off).
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
    s.att |> resize(c.seq_len)
    s.logits |> resize(c.vocab_size)
    if (c.n_expert > 0l) {   // MoE routing scratch
        s.moe_logits |> resize(c.n_expert)
        s.moe_w |> resize(c.n_expert_used)
        s.moe_idx |> resize(c.n_expert_used)
        s.moe_acc |> resize(c.dim)
    }
    let maxn = max(xdim, ffw)   // the Q8·Q8 path quantizes whichever activation feeds a matmul
    s.xq |> resize(maxn)
    s.xs |> resize(maxn / 32l)
    s.key_cache |> resize(c.seq_len * kv_row_total(c))
    s.value_cache |> resize(c.seq_len * kv_row_total(c))
    s.sample_scratch |> resize(c.vocab_size)
    s.rng = random_seed(1234)
    return <- s
}

//! Create a fresh session (KV cache + scratch) sized to `model`. A model serves many sessions,
//! each with its own position and cache — so one model drives many independent conversations.
def create_session(model : Model) : Session {
    return <- make_run_state(model.config)
}

//! THE eval primitive: run `tokens` at the session's current position and advance it. Prefill =
//! eval(prompt); each step = eval([token]) — same call, different batch size. Logits → `s.logits`.
def eval(t : Model; var s : Session; tokens : array<int64>) {
    let npos = int64(length(tokens))
    if (npos <= 0l) return
    forward_batch(t, s, tokens, npos, s.n_past)
    s.n_past += npos
}

//! Load a model AND its tokenizer from a GGUF — the full user-facing entry point. Arch and
//! tokenizer backend are both auto-selected from GGUF metadata; `mode` picks the weight quant.
def load_model(path : string; mode : QuantMode = QuantMode.fp32) : Model {
    var m <- load_gguf(path, mode)
    m.tok <- load_tokenizer_auto(path)
    return <- m
}

//! Encode text to token ids using the model's tokenizer.
def encode(m : Model; text : string; add_special = true; parse_special = false) : array<int64> {
    return <- encode(m.tok, text, add_special, parse_special)
}

//! Decode a full id sequence to text using the model's tokenizer.
def decode(m : Model; ids : array<int64>) : string {
    return decode(m.tok, ids)
}

//! Decode a single token to text (streaming) using the model's tokenizer.
def piece(m : Model; id : int64) : string {
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

// fp32 matmul with the weight as a zero-copy view into the big-weight blob at `woff` (len n*d).
def private mm_at(var y : array<float>; t : Model; woff : int64; x : array<float>; n, d : int64) {
    array_view(t.wblob, woff, n * d) $(w : array<float>#) {
        matmul(y, w, x, n, d)
    }
}

// fp32 matmul with the weight viewed into the fp32 aux blob (used for the tied classifier = embedding).
def private mm_fblob(var y : array<float>; t : Model; woff : int64; x : array<float>; n, d : int64) {
    array_view(t.fblob, woff, n * d) $(w : array<float>#) {
        matmul(y, w, x, n, d)
    }
}

// rmsnorm with the weight as a zero-copy view into the fp32 aux blob at `woff` (len size).
def private rms_at(var o : array<float> | #; t : Model; woff : int64; x : array<float> | #; size : int64) {
    array_view(t.fblob, woff, size) $(w : array<float>#) {
        rmsnorm(o, x, w, size, t.config.norm_eps)
    }
}

// Threaded per-position rmsnorm: out[p*dim..] = rmsnorm(src[p*dim..]) with the layer weight at fblob+woff.
// `out` and `src` may be the same array (Gemma post-norms run in place). Fork workers touch raw addresses.
def private rms_batch(var out : array<float>; src : array<float>; t : Model; woff : int64; dim, npos : int64) {
    let eps = t.config.norm_eps
    unsafe {
        var op = addr(out[0])
        let ip = reinterpret<float const?>(addr(src[0]))
        let wp = reinterpret<float const?>(addr(t.fblob[woff]))
        maybe_parallel_for(dim * npos >= NORM_PAR_THRESHOLD, 0, int(npos), get_total_hw_jobs()) $(rb, re) {
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
// in place for every position, at absolute pos start_pos+p. Fork workers touch raw addresses only —
// q_b/k_b are hoisted to base pointers, the read-only freq-factors to a const pointer. The KV-cache
// copy is a SEPARATE pass in the caller.
//   We call the leaf rope_scaled / rope_scaled_neox DIRECTLY (branching on neox here) rather than the
//   rope_apply dispatch wrapper — same reason rms_batch calls rmsnorm directly. Routing a hoisted
//   pointer THROUGH the rope_apply pass-through (whose param is `float const?`) lets the JIT mark that
//   param readonly and eliminate the leaf's reinterpret-back write — the rope silently no-ops. A
//   call-site reinterpret launders it, but that can't be lint-suppressed inside maybe_parallel_for, so
//   calling the leaf directly is the clean fix.
def private rope_batch(var q_b : array<float>; var k_b : array<float>; t : Model; theta, fscale : float; start_pos, npos, head_size, qd, kv_dim : int64; use_ff : bool = true) {
    let c = t.config
    let neox = c.rope_neox
    let mscale = c.rope_mscale
    let has_ff = use_ff && !empty(t.rope_freqs)
    unsafe {
        var qp = addr(q_b[0])
        var kp = addr(k_b[0])
        let ffp = has_ff ? reinterpret<float const?>(addr(t.rope_freqs[0])) : null
        let rope_par = (qd + kv_dim) * npos >= ROPE_PAR_THRESHOLD
        maybe_parallel_for(rope_par, 0, int(npos), get_total_hw_jobs()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    let ap = start_pos + pp
                    if (neox) {
                        rope_scaled_neox(qp + pp * qd, ap, head_size, qd, theta, fscale, has_ff, ffp, mscale)
                        rope_scaled_neox(kp + pp * kv_dim, ap, head_size, kv_dim, theta, fscale, has_ff, ffp, mscale)
                    } else {
                        rope_scaled(qp + pp * qd, ap, head_size, qd, theta, fscale, has_ff, ffp)
                        rope_scaled(kp + pp * kv_dim, ap, head_size, kv_dim, theta, fscale, has_ff, ffp)
                    }
                }
            }
        }
    }
}

// Precompute the prefill RoPE cos/sin table once: cos_tab[pi*half + j] = cos((start_pos+pi)*freq[j]),
// sin likewise, for every position pi in [0,npos) and frequency-pair j in [0, head_size/2). freq[j] is
// position-independent (one pow per pair, hoisted out of the position loop) and identical for NORM and
// NEOX; the NEOX mscale magnitude factor is folded in here so the tab leaves just look up. Built once
// before the layer loop, then reused by rope_batch_tab across all heads and layers (the redundancy the
// table removes). Bit-identical to the classic leaves — the same float expressions, just deduplicated.
def private build_rope_table(var cos_tab : array<float>; var sin_tab : array<float>; t : Model; theta, fscale : float; start_pos, npos, head_size : int64; use_ff : bool = true) {
    let c = t.config
    let half = head_size / 2l
    let scale = c.rope_neox ? c.rope_mscale : 1.0   // NEOX scales cos/sin by mscale; NORM leaves it 1.0
    let has_ff = use_ff && !empty(t.rope_freqs)
    cos_tab |> resize(npos * half)
    sin_tab |> resize(npos * half)
    unsafe {
        var cp = addr(cos_tab[0])
        var sp = addr(sin_tab[0])
        let ffp = has_ff ? reinterpret<float const?>(addr(t.rope_freqs[0])) : null
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

// Table-driven prefill RoPE: rope q_b[p*qd..] and k_b[p*kv_dim..] in place using the precomputed cos/sin
// table (no transcendentals here — just lookups). Mirrors rope_batch's threading + two-pass contract,
// and like it calls the LEAF rope_scaled_*_tab DIRECTLY (not a dispatch wrapper) to avoid the JIT
// pass-through-const-write-elision bug. crow/srow point at position p's table row (half entries).
def private rope_batch_tab(var q_b : array<float>; var k_b : array<float>; cos_tab : array<float>; sin_tab : array<float>;
                           neox : bool; npos, head_size, qd, kv_dim : int64) {
    let half = head_size / 2l
    unsafe {
        var qp = addr(q_b[0])
        var kp = addr(k_b[0])
        let cp = reinterpret<float const?>(addr(cos_tab[0]))
        let sp = reinterpret<float const?>(addr(sin_tab[0]))
        let rope_par = (qd + kv_dim) * npos >= ROPE_PAR_THRESHOLD
        maybe_parallel_for(rope_par, 0, int(npos), get_total_hw_jobs()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let pp = int64(p)
                    let crow = cp + pp * half
                    let srow = sp + pp * half
                    if (neox) {
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

// Store the roped k / raw v for all npos positions into the layer's KV cache. The destination offset
// loff + (start_pos+p)*kv_dim and the source offset p*kv_dim both advance by kv_dim per position, so
// the whole k_b -> key_cache store is ONE contiguous block of npos*kv_dim floats (v likewise) — copied
// with the vectorized copy_floats (bounds-checked array copy doesn't vectorize), threaded over the
// positions (each worker owns a disjoint cache slice). Self-gates on KV_STORE_PAR_THRESHOLD.
def private kv_store_batch(var key_cache : array<float>; var value_cache : array<float>;
                           k_b : array<float>; v_b : array<float>; loff, start_pos, npos, kv_dim : int64) {
    let dstbase = loff + start_pos * kv_dim
    unsafe {
        var kcp = addr(key_cache[0])
        var vcp = addr(value_cache[0])
        let kbp = reinterpret<float const?>(addr(k_b[0]))
        let vbp = reinterpret<float const?>(addr(v_b[0]))
        maybe_parallel_for(kv_dim * npos >= KV_STORE_PAR_THRESHOLD, 0, int(npos), get_total_hw_jobs()) $(rb, re) {
            let m0 = int64(rb) * kv_dim
            let cnt = (int64(re) - int64(rb)) * kv_dim
            unsafe {
                copy_floats(kcp + dstbase + m0, kbp + m0, cnt)
                copy_floats(vcp + dstbase + m0, vbp + m0, cnt)
            }
        }
    }
}

// Fused activation + gate multiply: hb[p] = act(W1·x[p]) * (W3·x[p]) in place, threaded per position.
// One memory pass (vs silu-then-mul), and the transcendental is vectorized via exp4 unless g_act_vec is
// off. Each element is independent (no cross-position reduce), so threading never changes the result;
// the only non-bit-exactness is exp4's ~2 ulp. Fork workers touch raw base pointers; self-gates on
// ACT_PAR_THRESHOLD so small prompts (and decode, npos=1) run inline.
def private gate_batch(var g : array<float>; u : array<float>; act : FfnAct; hidden, npos : int64) {
    let is_gelu = act == FfnAct.gelu
    let is_oai = act == FfnAct.swiglu_oai
    let vec = g_act_vec
    unsafe {
        var gp = addr(g[0])
        var up = reinterpret<float const?>(addr(u[0]))
        maybe_parallel_for(hidden * npos >= ACT_PAR_THRESHOLD, 0, int(npos), get_total_hw_jobs()) $(rb, re) {
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
    let total = n * d
    quantize_q8_0_into(x, n, xq, xs, 0l, 0l)
    array_view(t.qblob, woff, total) $(wq : array<int8>#) {
        array_view(t.qscales, woff / 32l, total / 32l) $(ws : array<float>#) {
            matmul_q8q8(y, wq, ws, xq, xs, n, d)
        }
    }
}

// Q4 matmul with the weight viewed into the quantized blob (4-bit, two per byte) + its per-block
// scales. Bytes start at woff/2 (woff is even), scales at woff/32.
def private mm_at_q4(var y : array<float>; t : Model; woff : int64; x : array<float>; n, d : int64) {
    let total = n * d
    array_view(t.q4blob, woff / 2l, total / 2l) $(wq : array<uint8>#) {
        array_view(t.q4scales, woff / 32l, total / 32l) $(ws : array<float>#) {
            matmul_q4(y, wq, ws, x, n, d)
        }
    }
}

// Fuse the attention Q/K/V projections (all three read s.xb) into one parallel dispatch when the
// model is Q8 and the combined matmul clears the parallel threshold: the small K/V projections ride
// along on the worker pool instead of running single-thread while it idles, and the activation is
// quantized once. Otherwise fall back to three separate matmuls (each handling its own threshold).
def private mm_qkv(var s : Session; t : Model; l : int64) {
    let c = t.config
    let dim = c.dim
    let kv = layer_kv_dim(c, l)
    let qd = layer_qd(c, l)            // query width; == dim except Gemma2/gemma4
    let qoff = t.wq_offs[l]
    let koff = t.wk_offs[l]
    let voff = t.wv_offs[l]
    if (voff < 0l) {
        // no attn_v tensor (gemma4 global layers): V = the pre-norm K projection
        mm(s.q, t, qoff, s.xb, s.xq, s.xs, dim, qd)
        mm(s.k, t, koff, s.xb, s.xq, s.xs, dim, kv)
        copy_floats(s.k, 0l, s.v, 0l, kv)
    } elif (t.quant == QuantMode.q8 && dim * (qd + 2l * kv) >= MATMUL_PAR_THRESHOLD) {
        quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)
        matmul_q8q8_group3(s.q, s.k, s.v, t.qblob, t.qscales, qoff, koff, voff, s.xq, s.xs, dim, qd, kv, kv)
    } else {
        mm(s.q, t, qoff, s.xb, s.xq, s.xs, dim, qd)
        mm(s.k, t, koff, s.xb, s.xq, s.xs, dim, kv)
        mm(s.v, t, voff, s.xb, s.xq, s.xs, dim, kv)
    }
}

// Route a big-weight matmul through the Q8, Q4, or fp32 kernel depending on the model's quant mode.
// xq/xs are the run-state activation-quant scratch (used only by the Q8 path).
def private mm(var y : array<float>; t : Model; woff : int64; x : array<float>; var xq : array<int8>; var xs : array<float>; n, d : int64) {
    if (t.quant == QuantMode.q8) {
        mm_at_q8(y, t, woff, x, xq, xs, n, d)
    } elif (t.quant == QuantMode.q4) {
        mm_at_q4(y, t, woff, x, n, d)
    } else {
        mm_at(y, t, woff, x, n, d)
    }
}

// Batched big-weight matmul for prefill: Y [npos x d] from X [npos x n], via the slice-1 GEMM
// kernels. Q8 quantizes each of the npos activation columns into the batched xqb/xsb scratch first;
// fp32 goes straight through. Q4 has no batched kernel, so forward_prefill never routes it here.
def private mm_b(var y : array<float>; t : Model; woff : int64; x : array<float>; var xqb : array<int8>; var xsb : array<float>; n, d, npos : int64) {
    let total = n * d
    if (t.quant == QuantMode.q8) {
        let nb = n / 32l
        let ts_rq = ref_time_ticks()
        // quantize each of the npos activation columns into xqb/xsb — independent per position, so
        // thread it (it was the largest serial bucket once attention + GEMM threaded). Fork workers
        // touch raw addresses only; the captured destinations bind straight through as `var int8?`/`var float?`.
        unsafe {
            let xp = reinterpret<float const?>(addr(x[0]))
            var qp = addr(xqb[0])
            var sp = addr(xsb[0])
            let rq_par = n * npos >= REQUANT_PAR_THRESHOLD
            maybe_parallel_for(rq_par, 0, int(npos), get_total_hw_jobs()) $(rb, re) {
                unsafe {
                    for (p in range(rb, re)) {
                        let pp = int64(p)
                        quantize_q8_0_into_ptr(xp + pp * n, n, qp, sp, pp * n, pp * nb)
                    }
                }
            }
        }
        prof_add("mm_requant", ts_rq)
        let ts_gemm = ref_time_ticks()
        array_view(t.qblob, woff, total) $(wq : array<int8>#) {
            array_view(t.qscales, woff / 32l, total / 32l) $(ws : array<float>#) {
                matmul_q8q8_batch(y, wq, ws, xqb, xsb, n, d, npos)
            }
        }
        prof_add("mm_gemm", ts_gemm)
    } else {
        array_view(t.wblob, woff, total) $(w : array<float>#) {
            matmul_batch(y, w, x, n, d, npos)
        }
    }
}

//! Per-layer attention pattern: with SWA on, an explicit per-layer mask (gemma4's GGUF bool array)
//! wins; otherwise every swa_pattern-th layer (l % p == p-1) is global and the rest slide — Gemma2
//! alternates (p=2, even layers slide), Gemma3 runs 5 local : 1 global (p=6).
def layer_is_sliding(c : Config; l : int64) : bool => (c.sliding_window > 0l
    && (c.swa_mask != 0ul ? ((c.swa_mask >> uint64(l)) & 1ul) != 0ul : l % c.swa_pattern != c.swa_pattern - 1l))

// ===== Per-layer attention geometry (gemma4: sliding vs global layer classes differ) =====
// head_size/n_kv_heads/kv_dim in Config describe the GLOBAL class; the *_swa fields (0 = same)
// override for sliding layers. Everything downstream sizes scratch to the class maxima.

//! Layer `l`'s attention head size.
def layer_head_size(c : Config; l : int64) : int64 => (layer_is_sliding(c, l) && c.head_size_swa > 0l
    ? c.head_size_swa : c.head_size)

//! Layer `l`'s KV-head count.
def layer_n_kv_heads(c : Config; l : int64) : int64 => (layer_is_sliding(c, l) && c.n_kv_heads_swa > 0l
    ? c.n_kv_heads_swa : c.n_kv_heads)

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
//! is a function of Config, never baked into the Model at load.
def kv_cache_off(c : Config; l : int64) : int64 {
    var row = 0l
    for (i in range64(l)) {
        row += layer_kv_dim(c, i)
    }
    return row * c.seq_len
}

//! True when sliding layers rope differently than global layers — different θ / position scale
//! (Gemma3: 10k/unscaled local vs 1M/linear-8 global) or a different head size (gemma4: 256 vs 512).
//! The loader resolves the swa fields to the global values when the arch doesn't set them, so this
//! is false for uniform-rope SWA models (Gemma2).
def has_dual_rope(c : Config) : bool => (c.sliding_window > 0l
    && (c.rope_freq_base_swa != c.rope_freq_base || c.rope_freq_scale_swa != c.rope_freq_scale
        || (c.head_size_swa > 0l && c.head_size_swa != c.head_size)))

//! One forward pass: token at position `pos` -> s.logits (over the vocabulary).
//! Faithful port of llama2.c forward(); weights are viewed into the blobs, KV cache grows by pos.
// ===== std/dense block kernels (decode, single-token) =====
// The default llama-family blocks the registry binds for every dense arch. Extracted verbatim from
// the per-layer loop so the seam is a function-pointer swap: a new arch registers a different block
// here without touching forward(). Bit-identical to the inlined form.

// attn-norm → QKV(+bias) → RoPE → KV-store → GQA multi-head attention → out-proj(+post-norm) → residual.
def private attention_std_decode(t : Model; var s : Session; l : int64; pos : int64) {
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size       // query / attention-output width (== dim except Gemma2/gemma4)
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let loff = kv_cache_off(c, l)
    // Sliding-window (local) vs global attention per the arch's layer pattern (Gemma2 alternates,
    // Gemma3 5:1). The window covers all positions when pos < window, so it's a no-op for short contexts.
    let is_sliding = layer_is_sliding(c, l)
    let kstart = (is_sliding && pos + 1l > c.sliding_window) ? (pos + 1l - c.sliding_window) : 0l
    let cnt = pos + 1l - kstart
    // attention rmsnorm
    rms_at(s.xb, t, t.rms_att_off + l * dim, s.x, dim)
    // q, k, v projections (independent — share s.xb; fused into one parallel dispatch when Q8)
    mm_qkv(s, t, l)
    if (c.attn_qkv_bias) {  // Qwen2: + bias before RoPE
        add_bias(s.q, 0l, t.bq, l * qd, qd)
        add_bias(s.k, 0l, t.bk, l * kv_dim, kv_dim)
        add_bias(s.v, 0l, t.bv, l * kv_dim, kv_dim)
    }
    if (c.qk_norm) {  // Qwen3/gemma4: per-head RMSNorm on q/k before RoPE — q/k are n_heads/n_kv_heads rows of head_size
        rms_batch(s.q, s.q, t, t.rms_q_offs[l], head_size, n_heads)
        rms_batch(s.k, s.k, t, t.rms_k_offs[l], head_size, n_kv_heads)
    }
    if (c.v_norm) {  // gemma4: weightless per-head RMSNorm on V (all-ones weight; V is never roped)
        rms_batch(s.v, s.v, t, t.rms_ones_off, head_size, n_kv_heads)
    }
    // RoPE on q (qd) and k (kv_dim) — sliding layers may use their own θ / position scale (Gemma3),
    // and p-RoPE arches (gemma4) apply the rope_freqs factors to GLOBAL layers only
    let theta = is_sliding ? c.rope_freq_base_swa : c.rope_freq_base
    let fscale = is_sliding ? c.rope_freq_scale_swa : c.rope_freq_scale
    let use_ff = !(c.rope_ff_global_only && is_sliding)
    rope_apply(s.q, pos, head_size, qd, theta, fscale, t.rope_freqs, c.rope_neox, c.rope_mscale, use_ff)
    rope_apply(s.k, pos, head_size, kv_dim, theta, fscale, t.rope_freqs, c.rope_neox, c.rope_mscale, use_ff)
    // store k, v for this position into the KV cache
    let kvoff = loff + pos * kv_dim
    copy_floats(unsafe(addr(s.key_cache[kvoff])), unsafe(addr(s.k[0])), kv_dim)
    copy_floats(unsafe(addr(s.value_cache[kvoff])), unsafe(addr(s.v[0])), kv_dim)
    // multi-head attention. att[0..cnt) holds the scores for keys kstart..pos (kstart=0 unless
    // a sliding-window layer past the window), scaled by 1/sqrt(head_size).
    for (h in range64(n_heads)) {
        let qoff = h * head_size
        let kvhoff = (h / kv_mul) * head_size  // GQA: which kv head this query head reads
        for (j in range64(cnt)) {
            let koff = loff + (kstart + j) * kv_dim + kvhoff
            s.att[j] = dot(unsafe(addr(s.q[qoff])), unsafe(addr(s.key_cache[koff])), head_size) * scale
        }
        if (c.attn_logit_softcap > 0.0) {  // Gemma2: soft-cap scores before softmax
            softcap(s.att, cnt, c.attn_logit_softcap)
        }
        if (c.attn_sinks) {  // gpt-oss: this head's sink logit joins the softmax denominator
            softmax_sink(s.att, cnt, t.fblob[t.sinks_off + l * n_heads + h])
        } else {
            softmax(s.att, cnt)
        }
        // weighted sum of values into xb[qoff..]
        for (i in range64(head_size)) {
            s.xb[qoff + i] = 0.0
        }
        for (j in range64(cnt)) {
            let voff = loff + (kstart + j) * kv_dim + kvhoff
            axpy(unsafe(addr(s.xb[qoff])), unsafe(addr(s.value_cache[voff])), s.att[j], head_size)
        }
    }
    // output projection (qd -> dim), optional bias + post-attention norm, residual
    mm(s.xb2, t, t.wo_offs[l], s.xb, s.xq, s.xs, qd, dim)
    if (c.attn_out_bias) {  // gpt-oss: + bias on the projected attention output
        add_bias(s.xb2, 0l, t.bo, l * dim, dim)
    }
    if (c.pre_post_norm) {  // Gemma2: post-attention RMSNorm before the residual add
        rms_at(s.xb2, t, t.rms_post_att_off + l * dim, s.xb2, dim)
    }
    add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb2[0])), dim)
}

// ffn-norm → gated FFN (act(W1·x) * (W3·x)) → down-proj(+post-norm) → residual.
def private ffn_dense_decode(t : Model; var s : Session; l : int64) {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    // ffn rmsnorm
    rms_at(s.xb, t, t.rms_ffn_off + l * dim, s.x, dim)
    // gated FFN: hb = act(W1·xb) * (W3·xb) — silu (SwiGLU) or gelu (GeGLU, Gemma2)
    mm(s.hb, t, t.w1_off + l * dim * hidden, s.xb, s.xq, s.xs, dim, hidden)
    mm(s.hb2, t, t.w3_off + l * dim * hidden, s.xb, s.xq, s.xs, dim, hidden)
    gate_batch(s.hb, s.hb2, c.ffn_act, hidden, 1l)   // fused act*gate; same kernel as prefill (npos=1 → inline)
    // down projection, optional post-FFN norm, residual
    mm(s.xb, t, t.w2_off + l * hidden * dim, s.hb, s.xq, s.xs, hidden, dim)
    if (c.pre_post_norm) {  // Gemma2: post-FFN RMSNorm before the residual add
        rms_at(s.xb, t, t.rms_post_ffn_off + l * dim, s.xb, dim)
    }
    add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb[0])), dim)
    if (c.layer_out_scale) {  // gemma4: scale the whole layer output (after the residual)
        let os = t.fblob[t.out_scale_off + l]
        for (i in range64(dim)) {
            s.x[i] *= os
        }
    }
}

// The MoE FFN core shared by decode and (naive per-token) prefill: reads the rmsnormed activation
// in s.xb, leaves the combined routed + shared-expert output in s.moe_acc. Router probs are taken
// over ALL experts BEFORE top-k selection (llama.cpp build_moe_ffn order); qwen2moe keeps the
// selected probs un-renormalized (norm_topk_prob renormalizes them for the DeepSeek/Qwen3-MoE shape).
def private moe_ffn_core(t : Model; var s : Session; l : int64) {
    let c = t.config
    let dim = c.dim
    let ne = c.n_expert
    let k = c.n_expert_used
    // router logits (fp32 weights + optional bias), then probs — softmax_weight routes on the RAW
    // biased logits and softmaxes only the selected top-k afterwards (gpt-oss)
    mm_fblob(s.moe_logits, t, t.router_off + l * ne * dim, s.xb, dim, ne)
    if (c.moe_router_bias) {
        add_bias(s.moe_logits, 0l, t.fblob, t.router_b_off + l * ne, ne)
    }
    if (c.moe_gate == MoeGate.sigmoid) {
        for (i in range64(ne)) {
            s.moe_logits[i] = 1.0 / (1.0 + exp(-s.moe_logits[i]))
        }
    } elif (c.moe_gate == MoeGate.softmax) {
        var mx = s.moe_logits[0]
        for (i in range64(ne)) {
            mx = max(mx, s.moe_logits[i])
        }
        var sum = 0.0
        for (i in range64(ne)) {
            let e = exp(s.moe_logits[i] - mx)
            s.moe_logits[i] = e
            sum += e
        }
        for (i in range64(ne)) {
            s.moe_logits[i] /= sum
        }
    }
    // top-k by selection score — the winner is knocked out with -FLT_MAX (softmax_weight selects on
    // raw logits, which can be arbitrarily negative); first-index-wins on ties matches argsort's
    // stable descending order
    for (j in range64(k)) {
        var best = 0l
        for (i in range64(ne)) {
            if (s.moe_logits[i] > s.moe_logits[best]) {
                best = i
            }
        }
        s.moe_idx[j] = best
        s.moe_w[j] = s.moe_logits[best]
        s.moe_logits[best] = -3.402823e38
    }
    if (c.moe_gate == MoeGate.softmax_weight) {  // gpt-oss: softmax over just the selected k logits
        softmax(s.moe_w, k)
    }
    if (c.norm_topk_prob) {   // renormalize the selected weights to sum 1 (llama.cpp clamps at f16-min)
        var ws = 0.0
        for (j in range64(k)) {
            ws += s.moe_w[j]
        }
        ws = max(ws, 6.103515625e-5)
        for (j in range64(k)) {
            s.moe_w[j] /= ws
        }
    }
    if (c.expert_weights_scale != 0.0 && c.expert_weights_scale != 1.0) {
        for (j in range64(k)) {
            s.moe_w[j] *= c.expert_weights_scale
        }
    }
    // routed experts: acc = Σ w_j · FFN_e(xb), each expert the standard gated-FFN shape
    for (i in range64(dim)) {
        s.moe_acc[i] = 0.0
    }
    let nfe = c.n_ff_exp
    let ege = dim * nfe
    for (j in range64(k)) {
        let e = l * ne + s.moe_idx[j]
        let base = e * ege
        mm(s.hb, t, t.we1_off + base, s.xb, s.xq, s.xs, dim, nfe)
        mm(s.hb2, t, t.we3_off + base, s.xb, s.xq, s.xs, dim, nfe)
        if (c.moe_exps_bias) {  // gpt-oss: this expert's gate/up biases before the activation
            add_bias(s.hb, 0l, t.fblob, t.web1_off + e * nfe, nfe)
            add_bias(s.hb2, 0l, t.fblob, t.web3_off + e * nfe, nfe)
        }
        gate_batch(s.hb, s.hb2, c.ffn_act, nfe, 1l)
        mm(s.xb2, t, t.we2_off + base, s.hb, s.xq, s.xs, nfe, dim)
        if (c.moe_exps_bias) {  // ... and its down bias, inside the routed weight (ggml_add_id order)
            add_bias(s.xb2, 0l, t.fblob, t.web2_off + e * dim, dim)
        }
        let w = s.moe_w[j]
        for (i in range64(dim)) {
            s.moe_acc[i] += w * s.xb2[i]
        }
    }
    // shared expert, gated by a sigmoid scalar from its per-layer gate vector
    if (c.n_ff_shexp > 0l) {
        var gl = 0.0
        array_view(t.fblob, t.shexp_gate_off + l * dim, dim) $(gv : array<float>#) {
            for (i in range64(dim)) {
                gl += gv[i] * s.xb[i]
            }
        }
        let g = 1.0 / (1.0 + exp(-gl))
        let nsh = c.n_ff_shexp
        let she = dim * nsh
        mm(s.hb, t, t.wsh1_off + l * she, s.xb, s.xq, s.xs, dim, nsh)
        mm(s.hb2, t, t.wsh3_off + l * she, s.xb, s.xq, s.xs, dim, nsh)
        gate_batch(s.hb, s.hb2, c.ffn_act, nsh, 1l)
        mm(s.xb2, t, t.wsh2_off + l * she, s.hb, s.xq, s.xs, nsh, dim)
        for (i in range64(dim)) {
            s.moe_acc[i] += g * s.xb2[i]
        }
    }
}

// MoE FFN, decode: rmsnorm -> routed top-k + shared expert -> residual. Mirrors ffn_dense_decode's
// optional post-norm / layer-output-scale steps so future MoE arches compose with them.
def private ffn_moe_decode(t : Model; var s : Session; l : int64) {
    let c = t.config
    let dim = c.dim
    rms_at(s.xb, t, t.rms_ffn_off + l * dim, s.x, dim)
    moe_ffn_core(t, s, l)
    if (c.pre_post_norm) {
        rms_at(s.moe_acc, t, t.rms_post_ffn_off + l * dim, s.moe_acc, dim)
    }
    add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.moe_acc[0])), dim)
    if (c.layer_out_scale) {
        let os = t.fblob[t.out_scale_off + l]
        for (i in range64(dim)) {
            s.x[i] *= os
        }
    }
}

def forward(t : Model; var s : Session; token, pos : int64) {
    let c = t.config
    let dim = c.dim

    // token embedding: copy row `token` of the fp32 embedding table into the residual stream
    array_view(t.fblob, t.tok_emb_off + token * dim, dim) $(row : array<float>#) {
        for (i in range64(dim)) {
            s.x[i] = row[i]
        }
    }
    if (c.embed_scale != 1.0) {  // Gemma2: scale the embedding by sqrt(dim)
        for (i in range64(dim)) {
            s.x[i] *= c.embed_scale
        }
    }

    // per-layer transformer stack — attention then FFN, each through the arch's registered block
    for (l in range64(c.n_layers)) {
        t.blocks.attn_decode(t, s, l, pos)
        t.blocks.ffn_decode(t, s, l)
    }

    // final rmsnorm (into xb to avoid aliasing x as both in/out), then classifier
    rms_at(s.xb, t, t.rms_final_off, s.x, dim)
    if (c.shared_weights) {
        // tied weights: the classifier IS the fp32 embedding table
        mm_fblob(s.logits, t, t.tok_emb_off, s.xb, dim, c.vocab_size)
    } else {
        mm(s.logits, t, t.wcls_off, s.xb, s.xq, s.xs, dim, c.vocab_size)
    }
    if (c.final_logit_softcap > 0.0) {  // Gemma2: soft-cap the output logits
        softcap(s.logits, c.vocab_size, c.final_logit_softcap)
    }
    for (id in t.suppress) {  // gemma4: pin the checkpoint's suppressed token ids to -inf
        s.logits[id] = -1.0e30
    }
}

// ----- forward_prefill section profiler -----
// Measurement is UNCONDITIONAL: a handful of ref_time_ticks() + table adds per prefill, negligible
// against the matmuls, so it always accumulates. Output is on-demand — reset() to start a window,
// report() to dump it. (We measure a lot; never gate the measurement, only the printing.)
// Thread the prefill attention over heads once the work proxy (npos × max-cnt × n_heads) clears this;
// below it the per-job dispatch overhead isn't worth it and we run the head loop inline.
let ATTN_PAR_THRESHOLD = 100_000l
// Thread the per-position activation requant (mm_b) once n × npos clears this.
let REQUANT_PAR_THRESHOLD = 256_000l
// Thread the per-position rmsnorm once dim × npos clears this.
let NORM_PAR_THRESHOLD = 256_000l
// Thread the per-position rope (q_b + k_b) once (qd + kv_dim) × npos clears this.
let ROPE_PAR_THRESHOLD = 100_000l
// Thread the KV-cache store once kv_dim × npos clears this (it's a memory-bound contiguous copy).
let KV_STORE_PAR_THRESHOLD = 256_000l
// Thread the per-position FFN activation (silu/gelu) once hidden × npos clears this. Lower than the
// memory-bound passes — each element pays a transcendental (exp/tanh), so the compute justifies threading sooner.
let ACT_PAR_THRESHOLD = 100_000l
// Query-block width for the blocked prefill attention (each K/V tile is reused across this many queries).
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

var g_prefill_prof_acc : table<string; int64>

//! Start a fresh measurement window (zero the per-section accumulators).
def prefill_profile_reset() {
    g_prefill_prof_acc |> clear()
}

def private prof_add(name : string; t0 : int64) {
    let cur = g_prefill_prof_acc?[name] ?? 0l
    g_prefill_prof_acc[name] = cur + int64(get_time_usec(t0))
}

//! The accumulated time (us) for one section since the last reset (0 if the section never ran).
def prefill_profile_get(name : string) : int64 {
    return g_prefill_prof_acc?[name] ?? 0l
}

//! Print the accumulated per-section times (us) since the last reset.
def prefill_profile_report() {
    var total = 0l
    for (v in values(g_prefill_prof_acc)) {
        total += v
    }
    to_log(LOG_INFO, "=== forward_prefill profile (us, summed over the window) ===\n")
    for (k, v in keys(g_prefill_prof_acc), values(g_prefill_prof_acc)) {
        to_log(LOG_INFO, "  {k}: {v} ({100l * v / max(total, 1l)}%)\n")
    }
    to_log(LOG_INFO, "  TOTAL: {total}\n")
}

// Prefill attention, threaded over heads (perfect load balance: every head walks the same causal
// triangle into a disjoint qd-strided output slice; no shared scratch). Dispatches on g_attn_prefill_mode.
// Workers run in fork contexts → raw pointers only; scalars are passed by value (never the struct).
//   classic — per query, stream cached K[kstart..ap] then V[kstart..ap] (O(npos²), K/V re-read per query).
//   blocked — query-block of ATTN_BLOCK_Q: load each K[kk]/V[kk] ONCE, reuse across the block's queries.
//     Bit-identical to classic: same per-query score row + softmax, and the V accumulation visits keys in
//     increasing order (== increasing j), so the per-output FMA order matches classic exactly.
// One head of tiled flash attention (online softmax). Q/K/V tiles are packed into this head's
// private scratch, QKᵀ and P·V run through the register-tiled gemm_f32 (no per-pair reduce), and a
// per-query running max/sum (mrun/srun) folds each KV tile in. All pointers are pre-offset to this
// head; qpk/kpk/vpk/kqp/vkqp are this head's scratch bases. NOT bit-exact vs classic (online reorder).
def private attn_head_flash(qp, kcp, vcp : float const?; var xbp : float?;
                            var qpk, kpk, vpk, kqp, vkqp : float?;
                            loff, npos, start_pos, head_size, qd, kv_dim, kvhoff, qoff : int64;
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
                // pack Kᵀ tile [head_size × KV] and V tile [KV × head_size], zero-padding the tail
                for (kt in range64(KV)) {
                    if (kt < kvalid) {
                        let ksrc = loff + (kt0 + kt) * kv_dim + kvhoff
                        for (d in range64(head_size)) {
                            kpk[d * KV + kt] = kcp[ksrc + d]
                            vpk[kt * head_size + d] = vcp[ksrc + d]
                        }
                    } else {
                        for (d in range64(head_size)) {
                            kpk[d * KV + kt] = 0.0
                            vpk[kt * head_size + d] = 0.0
                        }
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

def private prefill_attention(var s : Session; loff, npos, start_pos, head_size, qd, kv_dim,
                              n_heads, kv_mul : int64; scale : float;
                              is_sliding : bool; sliding_window : int64; attn_softcap : float;
                              sinksp : float const?) {   // this layer's per-head sink logits; null = no sinks
    let maxctx = start_pos + npos          // per-head att row width (= max cnt over this call)
    let attn_par = npos * maxctx * n_heads >= ATTN_PAR_THRESHOLD
    unsafe {
        let qp = reinterpret<float const?>(addr(s.q_b[0]))
        let kcp = reinterpret<float const?>(addr(s.key_cache[0]))
        let vcp = reinterpret<float const?>(addr(s.value_cache[0]))
        var xbp = addr(s.xb_b[0])
        var attp = addr(s.att_b[0])
        if (g_attn_prefill_mode == AttnPrefillMode.flash) {
            // per-head packing scratch bases (sized in forward_prefill for this mode)
            var qpk0 = addr(s.fa_q[0]);  var kpk0 = addr(s.fa_k[0]);  var vpk0 = addr(s.fa_v[0])
            var kqp0 = addr(s.fa_kq[0]); var vkqp0 = addr(s.fa_vkq[0])
            maybe_parallel_for(attn_par, 0, int(n_heads), get_total_hw_jobs()) $(rb, re) {
                unsafe {
                    for (h in range(rb, re)) {
                        let hh = int64(h)
                        let qoff = hh * head_size
                        let kvhoff = (hh / kv_mul) * head_size
                        attn_head_flash(qp, kcp, vcp, xbp,
                            qpk0 + hh * ATTN_FLASH_QT * head_size,
                            kpk0 + hh * head_size * ATTN_FLASH_KV,
                            vpk0 + hh * ATTN_FLASH_KV * head_size,
                            kqp0 + hh * ATTN_FLASH_QT * ATTN_FLASH_KV,
                            vkqp0 + hh * ATTN_FLASH_QT * head_size,
                            loff, npos, start_pos, head_size, qd, kv_dim, kvhoff, qoff,
                            scale, is_sliding, sliding_window, attn_softcap,
                            sinksp != null, sinksp != null ? sinksp[hh] : 0.0)
                    }
                }
            }
        } elif (g_attn_prefill_mode == AttnPrefillMode.blocked) {
            maybe_parallel_for(attn_par, 0, int(n_heads), get_total_hw_jobs()) $(rb, re) {
                unsafe {
                    for (h in range(rb, re)) {
                        let hh = int64(h)
                        let qoff = hh * head_size
                        let kvhoff = (hh / kv_mul) * head_size
                        let hblock = hh * ATTN_BLOCK_Q * maxctx   // this head's ATTN_BLOCK_Q score rows
                        var p0 = 0l
                        while (p0 < npos) {
                            let bq = min(ATTN_BLOCK_Q, npos - p0)
                            let ap_first = start_pos + p0
                            let ap_last = ap_first + bq - 1l
                            let kstart_block = (is_sliding && ap_first + 1l > sliding_window) ? (ap_first + 1l - sliding_window) : 0l
                            // PASS 1: scores — each K[kk] loaded once, reused across the block's queries
                            for (kk in range64(kstart_block, ap_last + 1l)) {
                                let koff = loff + kk * kv_dim + kvhoff
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
                                    attp[hblock + bi * maxctx + (kk - kstart_p)] = dot(qp + qbase, kcp + koff, head_size) * scale
                                }
                            }
                            // softcap + softmax per query, then zero its output slice
                            for (bi in range64(bq)) {
                                let p = p0 + bi
                                let ap = start_pos + p
                                let kstart_p = (is_sliding && ap + 1l > sliding_window) ? (ap + 1l - sliding_window) : 0l
                                let cnt = ap + 1l - kstart_p
                                let rbase = hblock + bi * maxctx
                                if (attn_softcap > 0.0) {  // Gemma2: soft-cap scores before softmax
                                    softcap(attp + rbase, cnt, attn_softcap)
                                }
                                if (sinksp != null) {  // gpt-oss: sink joins the denominator
                                    softmax_sink(attp + rbase, cnt, sinksp[hh])
                                } else {
                                    softmax(attp + rbase, cnt)
                                }
                                let qbase = p * qd + qoff
                                for (i in range64(head_size)) {
                                    xbp[qbase + i] = 0.0
                                }
                            }
                            // PASS 2: output += Σ att[j]·V[j] — each V[kk] loaded once, reused across the block
                            for (kk in range64(kstart_block, ap_last + 1l)) {
                                let voff = loff + kk * kv_dim + kvhoff
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
                                    axpy(xbp + qbase, vcp + voff, attp[hblock + bi * maxctx + (kk - kstart_p)], head_size)
                                }
                            }
                            p0 += ATTN_BLOCK_Q
                        }
                    }
                }
            }
        } else {
            maybe_parallel_for(attn_par, 0, int(n_heads), get_total_hw_jobs()) $(rb, re) {
                unsafe {
                    for (h in range(rb, re)) {
                        let hh = int64(h)
                        let qoff = hh * head_size
                        let kvhoff = (hh / kv_mul) * head_size
                        let abase = hh * maxctx                // this head's private score row
                        for (p in range64(npos)) {
                            let ap = start_pos + p
                            let kstart = (is_sliding && ap + 1l > sliding_window) ? (ap + 1l - sliding_window) : 0l
                            let cnt = ap + 1l - kstart
                            let qbase = p * qd + qoff           // q row == output row (both qd-strided)
                            for (j in range64(cnt)) {
                                let koff = loff + (kstart + j) * kv_dim + kvhoff
                                attp[abase + j] = dot(qp + qbase, kcp + koff, head_size) * scale
                            }
                            if (attn_softcap > 0.0) {  // Gemma2: soft-cap scores before softmax
                                softcap(attp + abase, cnt, attn_softcap)
                            }
                            if (sinksp != null) {  // gpt-oss: sink joins the denominator
                                softmax_sink(attp + abase, cnt, sinksp[hh])
                            } else {
                                softmax(attp + abase, cnt)
                            }
                            for (i in range64(head_size)) {
                                xbp[qbase + i] = 0.0
                            }
                            for (j in range64(cnt)) {   // output += Σ att[j]·V[j]
                                let voff = loff + (kstart + j) * kv_dim + kvhoff
                                axpy(xbp + qbase, vcp + voff, attp[abase + j], head_size)
                            }
                        }
                    }
                }
            }
        }
    }
}

//! Prefill: process all `npos` prompt tokens (positions 0..npos-1) in ONE pass per layer — batched
//! GEMM projections (matmul_batch / matmul_q8q8_batch) plus a per-position causal attention loop.
//! Leaves the KV cache filled for every position and s.logits = the logits at the LAST position
//! (what you sample the first generated token from). Bit-for-bit identical to calling forward() for
//! pos 0..npos-1 in sequence. Q4 has no batched kernel, so it falls back to that per-token path.
// ===== std/dense block kernels (prefill, batched) =====
// The batched counterparts to attention_std_decode / ffn_dense_decode; the registry binds these for
// every dense arch. Keep the per-section profiler buckets (prof_add) so prefill_profile_report stays
// intact. Bit-identical to the inlined form.

// attn-norm → batched QKV(+bias) → RoPE → KV-store → prefill attention → out-proj(+post-norm) → residual.
def private attention_std_prefill(t : Model; var s : Session; l : int64; npos : int64; start_pos : int64) {
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let use_rope_tab = g_use_rope_table
    let loff = kv_cache_off(c, l)
    // sliding vs global attention per the arch's layer pattern (see attention_std_decode)
    let is_sliding = layer_is_sliding(c, l)
    // attention rmsnorm, per position
    let ts_norm0 = ref_time_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_att_off + l * dim, dim, npos)
    prof_add("norm", ts_norm0)
    // batched q/k/v projections (each re-quantizes xb_b — cheap vs the matmul)
    let ts_qkv = ref_time_ticks()
    mm_b(s.q_b, t, t.wq_offs[l], s.xb_b, s.xqb, s.xsb, dim, qd, npos)
    mm_b(s.k_b, t, t.wk_offs[l], s.xb_b, s.xqb, s.xsb, dim, kv_dim, npos)
    if (t.wv_offs[l] >= 0l) {
        mm_b(s.v_b, t, t.wv_offs[l], s.xb_b, s.xqb, s.xsb, dim, kv_dim, npos)
    } else {
        // no attn_v tensor (gemma4 global layers): V = the pre-norm K projection
        copy_floats(s.k_b, 0l, s.v_b, 0l, npos * kv_dim)
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
        rms_batch(s.k_b, s.k_b, t, t.rms_k_offs[l], head_size, npos * n_kv_heads)
        prof_add("norm", ts_qknorm)
    }
    if (c.v_norm) {  // gemma4: weightless per-head RMSNorm on V (all-ones weight; V is never roped)
        let ts_vnorm = ref_time_ticks()
        rms_batch(s.v_b, s.v_b, t, t.rms_ones_off, head_size, npos * n_kv_heads)
        prof_add("norm", ts_vnorm)
    }
    // RoPE per position (absolute pos = start_pos + p) in place, then the KV-store pass below.
    // rope_batch* self-gates threading on ROPE_PAR_THRESHOLD, so small prompts run it inline.
    // Sliding layers read the swa table / θ when the arch ropes them differently (Gemma3/gemma4);
    // p-RoPE arches (gemma4) apply the rope_freqs factors to GLOBAL layers only.
    let ts_rope = ref_time_ticks()
    let swa_rope = is_sliding && has_dual_rope(c)
    if (use_rope_tab) {
        if (swa_rope) {
            rope_batch_tab(s.q_b, s.k_b, s.rope_cos_swa, s.rope_sin_swa, c.rope_neox, npos, head_size, qd, kv_dim)
        } else {
            rope_batch_tab(s.q_b, s.k_b, s.rope_cos, s.rope_sin, c.rope_neox, npos, head_size, qd, kv_dim)
        }
    } else {
        let theta = swa_rope ? c.rope_freq_base_swa : c.rope_freq_base
        let fscale = swa_rope ? c.rope_freq_scale_swa : c.rope_freq_scale
        let use_ff = !(c.rope_ff_global_only && is_sliding)
        rope_batch(s.q_b, s.k_b, t, theta, fscale, start_pos, npos, head_size, qd, kv_dim, use_ff)
    }
    prof_add("rope", ts_rope)
    // store the roped k / raw v into the KV cache — a SEPARATE pass (provably identical to
    // interleaving: rope of pos p touches only q_b[p]/k_b[p], the copy reads only k_b[p]/v_b[p]).
    let ts_kv = ref_time_ticks()
    kv_store_batch(s.key_cache, s.value_cache, s.k_b, s.v_b, loff, start_pos, npos, kv_dim)
    prof_add("kv_store", ts_kv)
    // causal attention into xb_b (position ap attends to cached keys kstart..ap; kstart = 0 unless a
    // Gemma2 sliding-window layer past the window). Output for position p lands at xb_b[p*qd..]
    // (qd-strided), feeding the wo projection below. Algorithm (classic vs blocked) is A/B-selectable.
    let ts_attn = ref_time_ticks()
    let sinksp = (c.attn_sinks
        ? unsafe(reinterpret<float const?>(unsafe(addr(t.fblob[t.sinks_off + l * n_heads]))))
        : null)
    prefill_attention(s, loff, npos, start_pos, head_size, qd, kv_dim, n_heads, kv_mul, scale,
                      is_sliding, c.sliding_window, c.attn_logit_softcap, sinksp)
    prof_add("attn", ts_attn)
    // output projection (batched, qd -> dim) + optional bias + post-attn norm + residual
    let ts_wo = ref_time_ticks()
    mm_b(s.xb2_b, t, t.wo_offs[l], s.xb_b, s.xqb, s.xsb, qd, dim, npos)
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

// ffn-norm → batched gated FFN → down-proj(+post-norm) → residual.
def private ffn_dense_prefill(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    // ffn rmsnorm, per position
    let ts_norm1 = ref_time_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_ffn_off + l * dim, dim, npos)
    prof_add("norm", ts_norm1)
    // gated FFN (batched): hb = act(W1·xb) * (W3·xb) — act/mul elementwise, so batch-flat
    let ts_ffn = ref_time_ticks()
    mm_b(s.hb_b, t, t.w1_off + l * dim * hidden, s.xb_b, s.xqb, s.xsb, dim, hidden, npos)
    mm_b(s.hb2_b, t, t.w3_off + l * dim * hidden, s.xb_b, s.xqb, s.xsb, dim, hidden, npos)
    prof_add("mm_ffn", ts_ffn)
    let ts_act = ref_time_ticks()
    gate_batch(s.hb_b, s.hb2_b, c.ffn_act, hidden, npos)   // fused act*gate (exp4 unless act_vec off)
    prof_add("act", ts_act)
    // down projection (batched) + optional post-FFN norm + residual
    let ts_ffn2 = ref_time_ticks()
    mm_b(s.xb_b, t, t.w2_off + l * hidden * dim, s.hb_b, s.xqb, s.xsb, hidden, dim, npos)
    prof_add("mm_ffn", ts_ffn2)
    let ts_resid1 = ref_time_ticks()
    if (c.pre_post_norm) {  // Gemma2: post-FFN RMSNorm per position before residual
        rms_batch(s.xb_b, s.xb_b, t, t.rms_post_ffn_off + l * dim, dim, npos)
    }
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb_b[0])), npos * dim)
    if (c.layer_out_scale) {  // gemma4: scale the whole layer output (after the residual)
        let os = t.fblob[t.out_scale_off + l]
        scale_inplace(unsafe(addr(s.x_b[0])), os, npos * dim)
    }
    prof_add("act_resid", ts_resid1)
}

// MoE FFN, prefill — naive per-token routing: each position routes to its own expert set, so there
// is no batched GEMM here; positions run serially through the decode scratch with the per-expert
// matmuls threading internally. The expert-bucketed grouped GEMM is a perf-ledger item.
def private ffn_moe_prefill(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let dim = c.dim
    let ts_norm = ref_time_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_ffn_off + l * dim, dim, npos)
    prof_add("norm", ts_norm)
    let ts_moe = ref_time_ticks()
    for (p in range64(npos)) {
        copy_floats(s.xb_b, p * dim, s.xb, 0l, dim)
        moe_ffn_core(t, s, l)
        if (c.pre_post_norm) {
            rms_at(s.moe_acc, t, t.rms_post_ffn_off + l * dim, s.moe_acc, dim)
        }
        for (i in range64(dim)) {   // residual add into this position's x_b row
            s.x_b[p * dim + i] += s.moe_acc[i]
        }
    }
    prof_add("mm_ffn", ts_moe)
    if (c.layer_out_scale) {
        let os = t.fblob[t.out_scale_off + l]
        scale_inplace(unsafe(addr(s.x_b[0])), os, npos * dim)
    }
}

def forward_prefill(t : Model; var s : Session; tokens : array<int64>; npos, start_pos : int64) {
    let c = t.config
    if (t.quant == QuantMode.q4 || npos <= 0l) {
        for (p in range64(npos)) {
            forward(t, s, tokens[p], start_pos + p)
        }
        return
    }
    let dim = c.dim
    let hidden = c.hidden_dim
    let kv_dim = max_kv_dim(c)         // scratch sizes to the larger layer class (gemma4: swa vs global)
    let head_size = max_head_size(c)
    let n_heads = c.n_heads
    let qd = n_heads * head_size       // query / attention-output width (== dim except Gemma2/gemma4)
    let xdim = max(dim, qd)            // xb_b holds the norm output (dim) then the attn output (qd)
    let maxn = max(xdim, hidden)
    // size the batched scratch to this prompt
    s.x_b |> resize(npos * dim)
    s.xb_b |> resize(npos * xdim)
    s.xb2_b |> resize(npos * dim)
    s.q_b |> resize(npos * qd)
    s.k_b |> resize(npos * kv_dim)
    s.v_b |> resize(npos * kv_dim)
    s.hb_b |> resize(npos * hidden)
    s.hb2_b |> resize(npos * hidden)
    s.xqb |> resize(npos * maxn)
    s.xsb |> resize(npos * maxn / 32l)
    // per-head attention scratch. classic uses one (start_pos+npos)-wide score row per head; blocked
    // needs ATTN_BLOCK_Q such rows per head (one per query in the block). Size for blocked — classic
    // indexes the first row of each head's block, so the wider allocation is a harmless superset.
    s.att_b |> resize(n_heads * ATTN_BLOCK_Q * (start_pos + npos))
    if (g_attn_prefill_mode == AttnPrefillMode.flash) {  // per-head flash packing scratch (see Session)
        s.fa_q |> resize(n_heads * ATTN_FLASH_QT * head_size)
        s.fa_k |> resize(n_heads * head_size * ATTN_FLASH_KV)
        s.fa_v |> resize(n_heads * ATTN_FLASH_KV * head_size)
        s.fa_kq |> resize(n_heads * ATTN_FLASH_QT * ATTN_FLASH_KV)
        s.fa_vkq |> resize(n_heads * ATTN_FLASH_QT * head_size)
    }

    // token embeddings -> residual stream x_b [npos x dim]
    let ts_embed = ref_time_ticks()
    for (p in range64(npos)) {
        let tok = tokens[p]
        array_view(t.fblob, t.tok_emb_off + tok * dim, dim) $(row : array<float>#) {
            for (i in range64(dim)) {
                s.x_b[p * dim + i] = row[i]
            }
        }
    }
    if (c.embed_scale != 1.0) {  // Gemma2: scale the embedding by sqrt(dim)
        for (k in range64(npos * dim)) {
            s.x_b[k] *= c.embed_scale
        }
    }
    prof_add("embed", ts_embed)

    // Precompute the RoPE cos/sin table ONCE for this prefill (angle = pos*freq is identical across
    // every head and layer, so this replaces the per-head/per-layer pow/cos/sin recomputation with a
    // single npos × head_size/2 table that rope_batch_tab looks up). Built outside the layer loop.
    // Dual-rope arches (Gemma3) get a second table with the sliding layers' θ / position scale.
    let use_rope_tab = g_use_rope_table
    let ts_rope_build = ref_time_ticks()
    if (use_rope_tab) {
        // global-class table (c.head_size, factors on); the swa table gets the sliding class's head
        // size and drops the factors on p-RoPE arches (gemma4: factors are global-layers-only)
        build_rope_table(s.rope_cos, s.rope_sin, t, c.rope_freq_base, c.rope_freq_scale, start_pos, npos, c.head_size, true)
        if (has_dual_rope(c)) {
            let swa_hs = c.head_size_swa > 0l ? c.head_size_swa : c.head_size
            build_rope_table(s.rope_cos_swa, s.rope_sin_swa, t, c.rope_freq_base_swa, c.rope_freq_scale_swa,
                             start_pos, npos, swa_hs, !c.rope_ff_global_only)
        }
    }
    prof_add("rope_build", ts_rope_build)   // one-time cos/sin table build (separate bucket)

    // per-layer transformer stack — batched attention then FFN, each through the arch's registered block
    for (l in range64(c.n_layers)) {
        t.blocks.attn_prefill(t, s, l, npos, start_pos)
        t.blocks.ffn_prefill(t, s, l, npos)
    }

    // final rmsnorm + classifier on the LAST position only (that's what we sample next)
    let ts_final = ref_time_ticks()
    let last = npos - 1l
    array_view(s.x_b, last * dim, dim) $(var xv : array<float>#) {
        rms_at(s.xb, t, t.rms_final_off, xv, dim)
    }
    if (c.shared_weights) {
        mm_fblob(s.logits, t, t.tok_emb_off, s.xb, dim, c.vocab_size)
    } else {
        mm(s.logits, t, t.wcls_off, s.xb, s.xq, s.xs, dim, c.vocab_size)
    }
    if (c.final_logit_softcap > 0.0) {  // Gemma2: soft-cap the output logits
        softcap(s.logits, c.vocab_size, c.final_logit_softcap)
    }
    for (id in t.suppress) {  // gemma4: pin the checkpoint's suppressed token ids to -inf
        s.logits[id] = -1.0e30
    }
    prof_add("final", ts_final)
}

//! Unified batch entry (llama.cpp-style): process `npos` tokens at absolute positions
//! start_pos..start_pos+npos-1. npos==1 dispatches to the fused per-token forward() (decode, keeping
//! its mm_qkv group3 + 1N threading); npos>1 to the batched forward_prefill. Leaves s.logits = the
//! last token's logits and extends the KV cache by npos positions.
def forward_batch(t : Model; var s : Session; tokens : array<int64>; npos, start_pos : int64) {
    if (npos == 1l) {
        forward(t, s, tokens[0], start_pos)
    } else {
        forward_prefill(t, s, tokens, npos, start_pos)
    }
}

//! Greedy argmax over the current logits; returns the winning token id.
def private argmax(s : Session; vocab : int64) : int64 {
    var best = 0l
    var bestv = s.logits[0]
    for (i in range64(vocab)) {
        if (s.logits[i] > bestv) {
            bestv = s.logits[i]
            best = i
        }
    }
    return best
}

//! Greedy decode. `prompt_tokens` is the encoded prompt (at least [BOS]); while still
//! inside it, the next token is forced from the prompt, otherwise it's the argmax of the
//! logits. Returns the emitted token IDs (prompt echo minus BOS, then the continuation;
//! stops early on BOS, like llama2.c).
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

//! Sampling parameters. Defaults are greedy (temp = 0, penalty = 1.0) — bit-identical to argmax.
struct SamplingParams {
    temp : float = 0.0             // <= 0 => greedy argmax; otherwise softmax temperature
    top_k : int64 = 0l             // <= 0 or >= vocab => no top-k cutoff
    penalty : float = 1.0          // repetition penalty over recent tokens (1.0 = none)
    penalty_last_n : int64 = 64l   // repetition-penalty window (most recent N generated tokens)
}

//! Seed the session's sampling RNG for reproducible generation.
def set_seed(var s : Session; seed : int) {
    s.rng = random_seed(seed)
}

//! Sample the next token from `s.logits` per `params`: repetition penalty over the last
//! `penalty_last_n` of `s.recent`, then temperature + top-k + softmax + CDF (or greedy argmax when
//! temp <= 0). Consumes `s.logits` in place and advances `s.rng`.
def sample(var s : Session; params : SamplingParams) : int64 {
    let vocab = int64(length(s.logits))
    if (params.penalty != 1.0) {
        let n = length(s.recent)
        let win = int(params.penalty_last_n)
        let pstart = n > win ? n - win : 0
        for (k in range(pstart, n)) {
            let id = s.recent[k]
            let v = s.logits[id]
            s.logits[id] = v > 0.0 ? v / params.penalty : v * params.penalty
        }
    }
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
    let r = random_float(s.rng)
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
//! false from the block to stop early (e.g. on a stop token). Prefills `prompt`, then samples one
//! token at a time, advancing the session. Returns the number of tokens emitted. (The greedy
//! `generate(t, s, prompt, steps)` above stays for the token-exact oracle path.)
def generate(t : Model; var s : Session; prompt : array<int64>; params : SamplingParams;
             max_tokens : int64; blk : block<(id : int64; piece : string) : bool>) : int64 {
    s.recent |> clear()
    let t_pre = ref_time_ticks()
    eval(t, s, prompt)
    s.stat_prefill_us = int64(get_time_usec(t_pre))
    s.stat_n_prefill = int64(length(prompt))
    var one : array<int64>
    one |> resize(1)
    var nout = 0l
    var got_first = false
    let t_gen = ref_time_ticks()
    for (_step in range64(max_tokens)) {
        let tok = sample(s, params)
        if (!got_first) {   // time-to-first-token spans the prefill + the first forward
            s.stat_ttft_us = int64(get_time_usec(t_pre))
            got_first = true
        }
        if (!invoke(blk, tok, piece(t, tok))) {
            break
        }
        nout++
        s.recent |> push(tok)
        one[0] = tok
        eval(t, s, one)
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
def stats(s : Session) : Stats {
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
// Chat-template part builders + the default dense block set. Each arch lives in its own
// dasllama_arch_*.das and self-registers at [init] using these; the four current arches are dense
// llama-likes that share std_blocks() and differ only in config flags, tokenizer backend, and chat
// template. The seam earns its keep when a non-dense block (MoE, …) registers different kernels
// without touching the forward loops.

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
