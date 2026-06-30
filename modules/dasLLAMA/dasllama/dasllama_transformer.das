options gen2

module dasllama_transformer shared public

require daslib/fio
require daslib/array_boost // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs and false-flags this as unused
require dasllama/dasllama_math
require dasllama/dasllama_math_aarch64_neon  // registers the arm64 SDOT Q8·Q8 backend at [init]; no-op off-ARM
require dasllama/dasllama_quant
require dasllama/dasllama_gguf
require dasllama/dasllama_par  // maybe_parallel_for (+ get_total_hw_jobs): thread the prefill attention over heads
require math

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

//! FFN gate activation: silu = SwiGLU (Llama family), gelu = GeGLU (Gemma).
enum FfnAct {
    silu
    gelu
}

//! Prefill attention algorithm (A/B selectable; all variants kept). `classic` streams the full causal
//! K/V range from the cache per query — threaded over heads, O(npos²) with K/V re-read every query.
//! `blocked` query-blocks so each K/V tile is loaded once and reused across the whole block, cutting
//! K/V cache traffic ~ATTN_BLOCK_Q×; it is BIT-IDENTICAL to classic (same per-query softmax + same
//! key-order V accumulation). `flash` is tiled flash attention — QKᵀ and P·V as register-tiled fp32
//! GEMMs (gemm_f32) with online softmax, the form that avoids the per-pair horizontal reduce; NOT
//! bit-identical (online-softmax FP reorder), validated token-for-token vs the oracle + logit-diff.
//! Select via set_prefill_attn_mode (default classic).
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
    pre_post_norm : bool = false       // Gemma2: extra post-attention / post-FFN norms
}

//! The whole model. `fblob` holds the fp32 embedding + norm weights at the *_off offsets below;
//! the big 2D weights live in exactly one of wblob (fp32) / qblob+qscales (Q8) / q4blob+q4scales
//! (Q4) per `quant`, at the w*_off offsets (parallel layout: the same weight-element offset indexes
//! all three — Q8 scales at off/32, Q4 bytes at off/2). For shared_weights the classifier reads the
//! fp32 embedding from fblob, so wcls_off is unused.
struct Transformer {
    config : Config = Config()
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
    // big 2D weights — exactly one representation populated per `quant`
    wblob : array<float>
    qblob : array<int8>
    qscales : array<float>
    q4blob : array<uint8>
    q4scales : array<float>
    wq_off : int64
    wk_off : int64
    wv_off : int64
    wo_off : int64
    w1_off : int64
    w2_off : int64
    w3_off : int64
    wcls_off : int64
    quant : QuantMode
    rope_freqs : array<float>   // per-pair RoPE scaling (Llama-3.1 NTK-by-parts); empty = none
    // QKV projection biases (Qwen2) — fp32, per layer (q: layers×dim, k/v: layers×kv_dim);
    // empty unless config.attn_qkv_bias
    bq : array<float>
    bk : array<float>
    bv : array<float>
}

struct private LayoutSizes {
    fblob_n : int64   // element count of the fp32 aux region
    wblob_n : int64   // element count of the big-weight region
}

// Fill both offset tables from the config; return the element counts of each region. The big-weight
// offsets are 32-aligned (every dim/kv/hidden is a multiple of 32), so Q8/Q4 scale and byte indices
// divide cleanly. qd = n_heads * head_size (== dim for the llama family).
def private layout_offsets(var t : Transformer) : LayoutSizes {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    let layers = c.n_layers
    let kv = c.kv_dim
    let qd = c.n_heads * c.head_size
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
    t.rms_final_off = fo
    fo += dim
    var wo = 0l
    t.wq_off = wo
    wo += layers * dim * qd
    t.wk_off = wo
    wo += layers * dim * kv
    t.wv_off = wo
    wo += layers * dim * kv
    t.wo_off = wo
    wo += layers * qd * dim
    t.w1_off = wo
    wo += layers * dim * hidden
    t.w2_off = wo
    wo += layers * hidden * dim
    t.w3_off = wo
    wo += layers * dim * hidden
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
def load_checkpoint(path : string) : Transformer {
    let HEADER_INTS = 7
    let HEADER_BYTES = 28
    var t = Transformer()
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
            var fc = 0l
            copy_floats(src, fc, t.fblob, t.tok_emb_off, c.vocab_size * dim)
            fc += c.vocab_size * dim
            copy_floats(src, fc, t.fblob, t.rms_att_off, layers * dim)
            fc += layers * dim
            copy_floats(src, fc, t.wblob, t.wq_off, layers * dim * qd)
            fc += layers * dim * qd
            copy_floats(src, fc, t.wblob, t.wk_off, layers * dim * kv)
            fc += layers * dim * kv
            copy_floats(src, fc, t.wblob, t.wv_off, layers * dim * kv)
            fc += layers * dim * kv
            copy_floats(src, fc, t.wblob, t.wo_off, layers * qd * dim)
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
    return <- t
}

// Read one big 2D weight tensor into the active representation: transcode straight from disk when
// the on-disk type already matches the target precision, otherwise dequant to a reused fp32 scratch
// and quantize into the shared blob. `woff` is the weight-element offset; `scratch` is reused.
def private load_big(m : GGUFMeta; bytes : array<uint8> | #; name : string; var t : Transformer; woff, n : int64; var scratch : array<float>; src_off : int64 = 0l) {
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

//! Validate the GGUF architecture and apply its per-arch capability flags to the config. The Config
//! defaults already encode the Llama family, so each arch overrides only its own deltas. An arch we
//! can't run correctly panics here — better to fail loudly than feed weights through the wrong path.
def private configure_arch(var c : Config; arch : string) {
    if (arch == "llama") {
        // Llama / Llama-2 / Llama-3 / TinyLlama — every Config default is already correct
    } elif (arch == "qwen2") {
        c.attn_qkv_bias = true   // Qwen2 adds a learned bias on the Q/K/V projections
        c.rope_neox = true       // ...and uses NEOX-style rope
    } elif (arch == "phi3") {
        c.rope_neox = true       // Phi3: NEOX rope + LongRoPE (freq factors + mscale, read in the loader)
        c.fused_qkv = true       // ...with fused attn_qkv + ffn_up (gate_up) tensors, split at load
    } elif (arch == "gemma2") {
        c.rope_neox = true                  // NEOX rope, full-rotary over head_size
        c.ffn_act = FfnAct.gelu             // GeGLU (tanh-approx gelu) instead of SwiGLU
        c.attn_logit_softcap = 50.0         // soft-cap attention scores before softmax
        c.final_logit_softcap = 30.0        // soft-cap the classifier logits
        c.sliding_window = 4096l            // local attention on even layers (swa_period=2)
        c.pre_post_norm = true              // extra post-attn + post-FFN norms (4 norms/layer)
        // embed_scale (sqrt(dim)), head_size (key_length) and the (1+w) norm weights are
        // resolved in the loader: dim isn't known here, and Gemma's GGUF pre-bakes the +1
    } else {
        panic("dasLLAMA: unsupported architecture '{arch}' (supported: llama, qwen2, phi3, gemma2)")
    }
}

//! Load a llama-architecture GGUF into the split layout at the requested precision. The big weights
// Repack every Q8 2D weight tensor in qblob/qscales into the active backend's interleaved layout, IN
// PLACE, once at load. Mirrors forward()'s per-tensor offsets/shapes exactly; the embedding + norms
// live in fblob and are never touched. Called only after enable_q8q8_laneq() succeeds (arm64); the
// repack pointer is otherwise identity, so this would be a no-op regardless.
def private repack_q8_weights(var t : Transformer) {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    let kv = c.kv_dim
    let qd = c.n_heads * c.head_size
    for (l in range64(c.n_layers)) {
        repack_q8q8_weight(t.qblob, t.qscales, t.wq_off + l * dim * qd, dim, qd)
        repack_q8q8_weight(t.qblob, t.qscales, t.wk_off + l * dim * kv, dim, kv)
        repack_q8q8_weight(t.qblob, t.qscales, t.wv_off + l * dim * kv, dim, kv)
        repack_q8q8_weight(t.qblob, t.qscales, t.wo_off + l * qd * dim, qd, dim)
        repack_q8q8_weight(t.qblob, t.qscales, t.w1_off + l * dim * hidden, dim, hidden)
        repack_q8q8_weight(t.qblob, t.qscales, t.w2_off + l * hidden * dim, hidden, dim)
        repack_q8q8_weight(t.qblob, t.qscales, t.w3_off + l * dim * hidden, dim, hidden)
    }
    if (!c.shared_weights) {
        repack_q8q8_weight(t.qblob, t.qscales, t.wcls_off, dim, c.vocab_size)
    }
}

//! go straight into `mode`'s storage (pre-quantized tensors transcode block-for-block with no fp32
//! round-trip; F16/F32 tensors dequant one at a time through a small reused scratch), so a Q8/Q4
//! load never holds the dense fp32 weights. The embedding + norms stay fp32 in fblob regardless.
//! Config comes from metadata; vocab is derived from token_embd (some files omit llama.vocab_size).
def load_gguf(path : string; mode : QuantMode = QuantMode.fp32) : Transformer {
    var t = Transformer()
    t.quant = mode
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let m <- parse_gguf_meta(bytes)
        let arch = gguf_str(m, bytes, "general.architecture")
        configure_arch(t.config, arch)
        // GGUF uses the architecture name as the metadata key prefix (llama.*, qwen2.*, gemma2.*, ...)
        let dim = gguf_int(m, bytes, "{arch}.embedding_length")
        let hidden = gguf_int(m, bytes, "{arch}.feed_forward_length")
        let layers = gguf_int(m, bytes, "{arch}.block_count")
        let n_heads = gguf_int(m, bytes, "{arch}.attention.head_count")
        let n_kv_heads = gguf_int(m, bytes, "{arch}.attention.head_count_kv")
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
        t.config.n_kv_heads = n_kv_heads
        t.config.vocab_size = vocab
        t.config.seq_len = seq_len
        // head_size from metadata key_length when present (Gemma2: 256, != dim/n_heads=288);
        // kv_dim = n_kv_heads * head_size. Both reduce to the llama formulas when head_size = dim/n_heads.
        t.config.head_size = (gguf_has(m, "{arch}.attention.key_length")
            ? gguf_int(m, bytes, "{arch}.attention.key_length")
            : dim / n_heads)
        t.config.kv_dim = n_kv_heads * t.config.head_size
        // Gemma2 scales the embedding by sqrt(dim); configure_arch can't (dim unknown there)
        if (arch == "gemma2") {
            t.config.embed_scale = sqrt(float(dim))
        }
        // RoPE base frequency (θ): default 10000, but Llama-3 uses 500000
        t.config.rope_freq_base = gguf_has(m, "{arch}.rope.freq_base") ? gguf_f32(m, bytes, "{arch}.rope.freq_base") : 10000.0
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
        // a separate output.weight means the classifier is not tied to the embeddings
        t.config.shared_weights = gguf_find_tensor(m, "output.weight") < 0
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
        for (l in range64(layers)) {
            gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_norm.weight", t.fblob, t.rms_att_off + l * dim, dim)
            gguf_read_tensor_f32(m, bytes, "blk.{l}.ffn_norm.weight", t.fblob, t.rms_ffn_off + l * dim, dim)
            if (t.config.pre_post_norm) {  // Gemma2: post-attn + post-FFN norms (already +1-baked in the GGUF)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.post_attention_norm.weight", t.fblob, t.rms_post_att_off + l * dim, dim)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.post_ffw_norm.weight", t.fblob, t.rms_post_ffn_off + l * dim, dim)
            }
        }
        gguf_read_tensor_f32(m, bytes, "output_norm.weight", t.fblob, t.rms_final_off, dim)
        // big 2D weights -> active precision. ggml stores them row-major [out x in] = our convention.
        var scratch : array<float>
        for (l in range64(layers)) {
            if (t.config.fused_qkv) {
                // Phi3: attn_qkv rows are [q (qd) ; k (kv) ; v (kv)], ffn_up rows are [gate ; up] (hidden each).
                // src_off slices the fused tensor, preserving its Q8 blocks (no fp32 requantize).
                load_big(m, bytes, "blk.{l}.attn_qkv.weight", t, t.wq_off + l * dim * qd, dim * qd, scratch, 0l)
                load_big(m, bytes, "blk.{l}.attn_qkv.weight", t, t.wk_off + l * dim * kv, dim * kv, scratch, dim * qd)
                load_big(m, bytes, "blk.{l}.attn_qkv.weight", t, t.wv_off + l * dim * kv, dim * kv, scratch, dim * qd + dim * kv)
                load_big(m, bytes, "blk.{l}.ffn_up.weight", t, t.w1_off + l * dim * hidden, dim * hidden, scratch, 0l)
                load_big(m, bytes, "blk.{l}.ffn_up.weight", t, t.w3_off + l * dim * hidden, dim * hidden, scratch, dim * hidden)
            } else {
                load_big(m, bytes, "blk.{l}.attn_q.weight", t, t.wq_off + l * dim * qd, dim * qd, scratch)
                load_big(m, bytes, "blk.{l}.attn_k.weight", t, t.wk_off + l * dim * kv, dim * kv, scratch)
                load_big(m, bytes, "blk.{l}.attn_v.weight", t, t.wv_off + l * dim * kv, dim * kv, scratch)
                load_big(m, bytes, "blk.{l}.ffn_gate.weight", t, t.w1_off + l * dim * hidden, dim * hidden, scratch)
                load_big(m, bytes, "blk.{l}.ffn_up.weight", t, t.w3_off + l * dim * hidden, dim * hidden, scratch)
            }
            load_big(m, bytes, "blk.{l}.attn_output.weight", t, t.wo_off + l * qd * dim, qd * dim, scratch)
            load_big(m, bytes, "blk.{l}.ffn_down.weight", t, t.w2_off + l * hidden * dim, hidden * dim, scratch)
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
        delete scratch
        // arm64: switch to the laneq backend and repack the just-loaded Q8 weights into its interleaved
        // layout. enable returns false off-ARM (portable stays, weights stay row-major).
        if (mode == QuantMode.q8 && enable_q8q8_laneq()) {
            repack_q8_weights(t)
        }
    }
    fclose(f)
    return <- t
}

//! Quantize a fp32-loaded model's big weights to Q8_0 in place and switch forward onto the Q8 path.
//! Requires the fp32 representation (wblob) — i.e. a model loaded with QuantMode.fp32. Keeps wblob
//! so the same model can be re-quantized to another precision for an A/B comparison.
def quantize_weights(var t : Transformer) {
    let n = int64(length(t.wblob))
    var qt <- quantize_q8_0(t.wblob, n)
    t.qblob <- qt.q
    t.qscales <- qt.scales
    t.quant = QuantMode.q8
    // same laneq enable + in-place repack as the load_gguf Q8 path (no-op off-ARM)
    if (enable_q8q8_laneq()) {
        repack_q8_weights(t)
    }
}

//! Quantize a fp32-loaded model's big weights to Q4_0 in place and switch forward onto the Q4 path.
//! Same fp32-required / wblob-kept contract as quantize_weights.
def quantize_weights_q4(var t : Transformer) {
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

def footprint(t : Transformer) : MemFootprint {
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
struct RunState {
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
}

//! Allocate the run state sized to a model.
def make_run_state(c : Config) : RunState {
    var s : RunState
    // qd = n_heads * head_size, the query / attention-output width. == dim for the llama family,
    // but Gemma2 has qd (2048) != dim (2304), so q and the attention scratch xb size to max(dim, qd).
    let qd = c.n_heads * c.head_size
    let xdim = max(c.dim, qd)
    s.x |> resize(c.dim)
    s.xb |> resize(xdim)
    s.xb2 |> resize(c.dim)
    s.hb |> resize(c.hidden_dim)
    s.hb2 |> resize(c.hidden_dim)
    s.q |> resize(qd)
    s.k |> resize(c.kv_dim)
    s.v |> resize(c.kv_dim)
    s.att |> resize(c.seq_len)
    s.logits |> resize(c.vocab_size)
    let maxn = max(xdim, c.hidden_dim)   // the Q8·Q8 path quantizes whichever activation feeds a matmul
    s.xq |> resize(maxn)
    s.xs |> resize(maxn / 32l)
    s.key_cache |> resize(c.n_layers * c.seq_len * c.kv_dim)
    s.value_cache |> resize(c.n_layers * c.seq_len * c.kv_dim)
    return <- s
}

// fp32 matmul with the weight as a zero-copy view into the big-weight blob at `woff` (len n*d).
def private mm_at(var y : array<float>; t : Transformer; woff : int64; x : array<float>; n, d : int64) {
    array_view(t.wblob, woff, n * d) $(w : array<float>#) {
        matmul(y, w, x, n, d)
    }
}

// fp32 matmul with the weight viewed into the fp32 aux blob (used for the tied classifier = embedding).
def private mm_fblob(var y : array<float>; t : Transformer; woff : int64; x : array<float>; n, d : int64) {
    array_view(t.fblob, woff, n * d) $(w : array<float>#) {
        matmul(y, w, x, n, d)
    }
}

// rmsnorm with the weight as a zero-copy view into the fp32 aux blob at `woff` (len size).
def private rms_at(var o : array<float> | #; t : Transformer; woff : int64; x : array<float> | #; size : int64) {
    array_view(t.fblob, woff, size) $(w : array<float>#) {
        rmsnorm(o, x, w, size, t.config.norm_eps)
    }
}

// Threaded per-position rmsnorm: out[p*dim..] = rmsnorm(src[p*dim..]) with the layer weight at fblob+woff.
// `out` and `src` may be the same array (Gemma post-norms run in place). Fork workers touch raw addresses.
def private rms_batch(var out : array<float>; src : array<float>; t : Transformer; woff : int64; dim, npos : int64) {
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
def private rope_batch(var q_b : array<float>; var k_b : array<float>; t : Transformer; start_pos, npos, head_size, qd, kv_dim : int64) {
    let c = t.config
    let theta = c.rope_freq_base
    let neox = c.rope_neox
    let mscale = c.rope_mscale
    let has_ff = !empty(t.rope_freqs)
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
                        rope_scaled_neox(qp + pp * qd, ap, head_size, qd, theta, has_ff, ffp, mscale)
                        rope_scaled_neox(kp + pp * kv_dim, ap, head_size, kv_dim, theta, has_ff, ffp, mscale)
                    } else {
                        rope_scaled(qp + pp * qd, ap, head_size, qd, theta, has_ff, ffp)
                        rope_scaled(kp + pp * kv_dim, ap, head_size, kv_dim, theta, has_ff, ffp)
                    }
                }
            }
        }
    }
}

// Add a projection bias (Qwen2): y[yoff + i] += b[boff + i]. yoff is 0 for the single-token forward,
// p*stride for the batched prefill; boff is the per-layer slice into bq/bk/bv.
def private add_bias(var y : array<float>; yoff : int64; b : array<float>; boff, n : int64) {
    for (i in range64(n)) {
        y[yoff + i] += b[boff + i]
    }
}

// Q8 matmul: quantize the activation x to Q8 (into the run-state scratch xq/xs, shared across all d
// rows), then run the int8·int8 SDOT kernel against the int8 weights. woff is the weight-element
// offset; scales start at woff/32 (woff is block-aligned for every 2D tensor).
def private mm_at_q8(var y : array<float>; t : Transformer; woff : int64; x : array<float>; var xq : array<int8>; var xs : array<float>; n, d : int64) {
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
def private mm_at_q4(var y : array<float>; t : Transformer; woff : int64; x : array<float>; n, d : int64) {
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
def private mm_qkv(var s : RunState; t : Transformer; l : int64) {
    let c = t.config
    let dim = c.dim
    let kv = c.kv_dim
    let qd = c.n_heads * c.head_size   // query width; == dim except Gemma2 (qd != dim)
    let qoff = t.wq_off + l * dim * qd
    let koff = t.wk_off + l * dim * kv
    let voff = t.wv_off + l * dim * kv
    if (t.quant == QuantMode.q8 && dim * (qd + 2l * kv) >= MATMUL_PAR_THRESHOLD) {
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
def private mm(var y : array<float>; t : Transformer; woff : int64; x : array<float>; var xq : array<int8>; var xs : array<float>; n, d : int64) {
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
def private mm_b(var y : array<float>; t : Transformer; woff : int64; x : array<float>; var xqb : array<int8>; var xsb : array<float>; n, d, npos : int64) {
    let total = n * d
    if (t.quant == QuantMode.q8) {
        let nb = n / 32l
        let ts_rq = ref_time_ticks()
        // quantize each of the npos activation columns into xqb/xsb — independent per position, so
        // thread it (it was the largest serial bucket once attention + GEMM threaded). Fork workers
        // touch raw addresses only; quantize_q8_0_into_ptr un-consts the captured destinations.
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

//! One forward pass: token at position `pos` -> s.logits (over the vocabulary).
//! Faithful port of llama2.c forward(); weights are viewed into the blobs, KV cache grows by pos.
def forward(t : Transformer; var s : RunState; token, pos : int64) {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    let kv_dim = c.kv_dim
    let head_size = c.head_size
    let n_heads = c.n_heads
    let qd = n_heads * head_size       // query / attention-output width (== dim except Gemma2)
    let kv_mul = n_heads / c.n_kv_heads
    let seq_len = c.seq_len
    let scale = 1.0 / sqrt(float(head_size))

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

    for (l in range64(c.n_layers)) {
        let loff = l * seq_len * kv_dim
        // Gemma2: even layers use sliding-window (local) attention; odd layers are global. The window
        // covers all positions when pos < window, so this is a no-op for short contexts.
        let is_sliding = c.sliding_window > 0l && (l % 2l == 0l)
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
        // RoPE on q (qd) and k (kv_dim)
        rope_apply(s.q, pos, head_size, qd, c.rope_freq_base, t.rope_freqs, c.rope_neox, c.rope_mscale)
        rope_apply(s.k, pos, head_size, kv_dim, c.rope_freq_base, t.rope_freqs, c.rope_neox, c.rope_mscale)
        // store k, v for this position into the KV cache
        let kvoff = loff + pos * kv_dim
        for (i in range64(kv_dim)) {
            s.key_cache[kvoff + i] = s.k[i]
            s.value_cache[kvoff + i] = s.v[i]
        }
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
            softmax(s.att, cnt)
            // weighted sum of values into xb[qoff..]
            for (i in range64(head_size)) {
                s.xb[qoff + i] = 0.0
            }
            for (j in range64(cnt)) {
                let voff = loff + (kstart + j) * kv_dim + kvhoff
                axpy(unsafe(addr(s.xb[qoff])), unsafe(addr(s.value_cache[voff])), s.att[j], head_size)
            }
        }
        // output projection (qd -> dim), optional post-attention norm, residual
        mm(s.xb2, t, t.wo_off + l * qd * dim, s.xb, s.xq, s.xs, qd, dim)
        if (c.pre_post_norm) {  // Gemma2: post-attention RMSNorm before the residual add
            rms_at(s.xb2, t, t.rms_post_att_off + l * dim, s.xb2, dim)
        }
        for (i in range64(dim)) {
            s.x[i] += s.xb2[i]
        }
        // ffn rmsnorm
        rms_at(s.xb, t, t.rms_ffn_off + l * dim, s.x, dim)
        // gated FFN: hb = act(W1·xb) * (W3·xb) — silu (SwiGLU) or gelu (GeGLU, Gemma2)
        mm(s.hb, t, t.w1_off + l * dim * hidden, s.xb, s.xq, s.xs, dim, hidden)
        mm(s.hb2, t, t.w3_off + l * dim * hidden, s.xb, s.xq, s.xs, dim, hidden)
        if (c.ffn_act == FfnAct.gelu) {
            gelu(s.hb, hidden)
        } else {
            silu(s.hb, hidden)
        }
        for (i in range64(hidden)) {
            s.hb[i] *= s.hb2[i]
        }
        // down projection, optional post-FFN norm, residual
        mm(s.xb, t, t.w2_off + l * hidden * dim, s.hb, s.xq, s.xs, hidden, dim)
        if (c.pre_post_norm) {  // Gemma2: post-FFN RMSNorm before the residual add
            rms_at(s.xb, t, t.rms_post_ffn_off + l * dim, s.xb, dim)
        }
        for (i in range64(dim)) {
            s.x[i] += s.xb[i]
        }
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
// Query-block width for the blocked prefill attention (each K/V tile is reused across this many queries).
let ATTN_BLOCK_Q = 8l
// Tile dims for the flash prefill attention (Q rows × KV cols processed per GEMM tile). The flash
// per-query running max/sum use fixed arrays sized ATTN_FLASH_QT — keep that a literal 64 in sync.
let ATTN_FLASH_QT = 64l
let ATTN_FLASH_KV = 64l

var g_attn_prefill_mode = AttnPrefillMode.classic   // A/B toggle; see AttnPrefillMode

//! Select the prefill attention algorithm (default classic). See AttnPrefillMode.
def set_prefill_attn_mode(m : AttnPrefillMode) {
    g_attn_prefill_mode = m
}

//! The prefill attention algorithm currently selected.
def get_prefill_attn_mode() : AttnPrefillMode {
    return g_attn_prefill_mode
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
                            scale : float; is_sliding : bool; sliding_window : int64; attn_softcap : float) {
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
                mrun[int(qt)] = -1.0e30
                srun[int(qt)] = 0.0
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
                // scale + softcap + causal/sliding mask, then online-softmax fold per query row
                for (qt in range64(tile_rows)) {
                    let ap = ap0 + qt
                    let kstart_p = (is_sliding && ap + 1l > sliding_window) ? (ap + 1l - sliding_window) : 0l
                    let row = qt * KV
                    var tmax = -1.0e30
                    for (kt in range64(KV)) {
                        let kabs = kt0 + kt
                        if (kt < kvalid && kabs <= ap && kabs >= kstart_p) {
                            var sc = kqp[row + kt] * scale
                            if (attn_softcap > 0.0) {  // Gemma2: soft-cap scores before softmax
                                sc = attn_softcap * tanh(sc / attn_softcap)
                            }
                            kqp[row + kt] = sc
                            tmax = max(tmax, sc)
                        } else {
                            kqp[row + kt] = -1.0e30   // masked → exp() to 0 below
                        }
                    }
                    let mold = mrun[int(qt)]
                    let mnew = max(mold, tmax)
                    if (mnew > mold) {                 // new running max → rescale this row's accumulator
                        let ms = exp(mold - mnew)
                        for (d in range64(head_size)) {
                            vkqp[qt * head_size + d] *= ms
                        }
                        srun[int(qt)] *= ms
                    }
                    mrun[int(qt)] = mnew
                    for (kt in range64(KV)) {          // kq row becomes the (un-normalized) prob row
                        let pe = kqp[row + kt] <= -1.0e29 ? 0.0 : exp(kqp[row + kt] - mnew)
                        kqp[row + kt] = pe
                        srun[int(qt)] += pe
                    }
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

def private prefill_attention(var s : RunState; loff, npos, start_pos, head_size, qd, kv_dim,
                              n_heads, kv_mul : int64; scale : float;
                              is_sliding : bool; sliding_window : int64; attn_softcap : float) {
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
                            scale, is_sliding, sliding_window, attn_softcap)
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
                                softmax(attp + rbase, cnt)
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
                                    vaxpy(xbp + qbase, vcp + voff, attp[hblock + bi * maxctx + (kk - kstart_p)], head_size)
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
                            softmax(attp + abase, cnt)
                            for (i in range64(head_size)) {
                                xbp[qbase + i] = 0.0
                            }
                            for (j in range64(cnt)) {   // output += Σ att[j]·V[j]
                                let voff = loff + (kstart + j) * kv_dim + kvhoff
                                vaxpy(xbp + qbase, vcp + voff, attp[abase + j], head_size)
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
def forward_prefill(t : Transformer; var s : RunState; tokens : array<int64>; npos, start_pos : int64) {
    let c = t.config
    if (t.quant == QuantMode.q4 || npos <= 0l) {
        for (p in range64(npos)) {
            forward(t, s, tokens[p], start_pos + p)
        }
        return
    }
    let dim = c.dim
    let hidden = c.hidden_dim
    let kv_dim = c.kv_dim
    let head_size = c.head_size
    let n_heads = c.n_heads
    let qd = n_heads * head_size       // query / attention-output width (== dim except Gemma2)
    let xdim = max(dim, qd)            // xb_b holds the norm output (dim) then the attn output (qd)
    let kv_mul = n_heads / c.n_kv_heads
    let seq_len = c.seq_len
    let scale = 1.0 / sqrt(float(head_size))
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
    if (g_attn_prefill_mode == AttnPrefillMode.flash) {  // per-head flash packing scratch (see RunState)
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

    for (l in range64(c.n_layers)) {
        let loff = l * seq_len * kv_dim
        // Gemma2: even layers use sliding-window (local) attention; odd layers global (see forward())
        let is_sliding = c.sliding_window > 0l && (l % 2l == 0l)
        // attention rmsnorm, per position
        let ts_norm0 = ref_time_ticks()
        rms_batch(s.xb_b, s.x_b, t, t.rms_att_off + l * dim, dim, npos)
        prof_add("norm", ts_norm0)
        // batched q/k/v projections (each re-quantizes xb_b — cheap vs the matmul)
        let ts_qkv = ref_time_ticks()
        mm_b(s.q_b, t, t.wq_off + l * dim * qd, s.xb_b, s.xqb, s.xsb, dim, qd, npos)
        mm_b(s.k_b, t, t.wk_off + l * dim * kv_dim, s.xb_b, s.xqb, s.xsb, dim, kv_dim, npos)
        mm_b(s.v_b, t, t.wv_off + l * dim * kv_dim, s.xb_b, s.xqb, s.xsb, dim, kv_dim, npos)
        if (c.attn_qkv_bias) {  // Qwen2: + bias per position before RoPE
            for (p in range64(npos)) {
                add_bias(s.q_b, p * qd, t.bq, l * qd, qd)
                add_bias(s.k_b, p * kv_dim, t.bk, l * kv_dim, kv_dim)
                add_bias(s.v_b, p * kv_dim, t.bv, l * kv_dim, kv_dim)
            }
        }
        prof_add("mm_qkv", ts_qkv)
        // RoPE per position (absolute pos = start_pos + p) in place, THEN store k/v into the cache as a
        // SEPARATE pass. Two passes (rope-all, then copy-all) instead of interleaving per position is
        // provably identical (rope of pos p touches only q_b[p]/k_b[p]; the copy reads only k_b[p]/v_b[p])
        // and keeps the rope-write cleanly separated from the cache-read. rope_batch self-gates the
        // threading on ROPE_PAR_THRESHOLD, so small prompts run it inline.
        let ts_rope = ref_time_ticks()
        rope_batch(s.q_b, s.k_b, t, start_pos, npos, head_size, qd, kv_dim)
        for (p in range64(npos)) {
            let kvoff = loff + (start_pos + p) * kv_dim
            for (i in range64(kv_dim)) {
                s.key_cache[kvoff + i] = s.k_b[p * kv_dim + i]
                s.value_cache[kvoff + i] = s.v_b[p * kv_dim + i]
            }
        }
        prof_add("rope", ts_rope)
        // causal attention into xb_b (position ap attends to cached keys kstart..ap; kstart = 0 unless a
        // Gemma2 sliding-window layer past the window). Output for position p lands at xb_b[p*qd..]
        // (qd-strided), feeding the wo projection below. Algorithm (classic vs blocked) is A/B-selectable.
        let ts_attn = ref_time_ticks()
        prefill_attention(s, loff, npos, start_pos, head_size, qd, kv_dim, n_heads, kv_mul, scale,
                          is_sliding, c.sliding_window, c.attn_logit_softcap)
        prof_add("attn", ts_attn)
        // output projection (batched, qd -> dim) + optional post-attn norm + residual
        let ts_wo = ref_time_ticks()
        mm_b(s.xb2_b, t, t.wo_off + l * qd * dim, s.xb_b, s.xqb, s.xsb, qd, dim, npos)
        prof_add("mm_wo", ts_wo)
        let ts_resid0 = ref_time_ticks()
        if (c.pre_post_norm) {  // Gemma2: post-attention RMSNorm per position before residual
            rms_batch(s.xb2_b, s.xb2_b, t, t.rms_post_att_off + l * dim, dim, npos)
        }
        add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
        prof_add("act_resid", ts_resid0)
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
        if (c.ffn_act == FfnAct.gelu) {
            gelu(s.hb_b, npos * hidden)
        } else {
            silu(s.hb_b, npos * hidden)
        }
        mul_inplace(unsafe(addr(s.hb_b[0])), unsafe(addr(s.hb2_b[0])), npos * hidden)
        prof_add("act_resid", ts_act)
        // down projection (batched) + optional post-FFN norm + residual
        let ts_ffn2 = ref_time_ticks()
        mm_b(s.xb_b, t, t.w2_off + l * hidden * dim, s.hb_b, s.xqb, s.xsb, hidden, dim, npos)
        prof_add("mm_ffn", ts_ffn2)
        let ts_resid1 = ref_time_ticks()
        if (c.pre_post_norm) {  // Gemma2: post-FFN RMSNorm per position before residual
            rms_batch(s.xb_b, s.xb_b, t, t.rms_post_ffn_off + l * dim, dim, npos)
        }
        add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb_b[0])), npos * dim)
        prof_add("act_resid", ts_resid1)
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
    prof_add("final", ts_final)
}

//! Unified batch entry (llama.cpp-style): process `npos` tokens at absolute positions
//! start_pos..start_pos+npos-1. npos==1 dispatches to the fused per-token forward() (decode, keeping
//! its mm_qkv group3 + 1N threading); npos>1 to the batched forward_prefill. Leaves s.logits = the
//! last token's logits and extends the KV cache by npos positions.
def forward_batch(t : Transformer; var s : RunState; tokens : array<int64>; npos, start_pos : int64) {
    if (npos == 1l) {
        forward(t, s, tokens[0], start_pos)
    } else {
        forward_prefill(t, s, tokens, npos, start_pos)
    }
}

//! Greedy argmax over the current logits; returns the winning token id.
def private argmax(s : RunState; vocab : int64) : int64 {
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
def generate(t : Transformer; var s : RunState; prompt_tokens : array<int64>; steps : int) : array<int64> {
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
