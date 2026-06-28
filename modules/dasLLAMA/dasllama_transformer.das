options gen2

module dasllama_transformer shared public

require daslib/fio
require daslib/array_boost // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs and false-flags this as unused
require dasllama_math
require dasllama_quant
require dasllama_gguf
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
}

//! The whole model. `fblob` holds the fp32 embedding + norm weights at the *_off offsets below;
//! the big 2D weights live in exactly one of wblob (fp32) / qblob+qscales (Q8) / q4blob+q4scales
//! (Q4) per `quant`, at the w*_off offsets (parallel layout: the same weight-element offset indexes
//! all three — Q8 scales at off/32, Q4 bytes at off/2). For shared_weights the classifier reads the
//! fp32 embedding from fblob, so wcls_off is unused.
struct Transformer {
    config : Config
    // fp32 aux: embedding + norms
    fblob : array<float>
    tok_emb_off : int64
    rms_att_off : int64
    rms_ffn_off : int64
    rms_final_off : int64
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
    var t : Transformer
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
def private load_big(m : GGUFMeta; bytes : array<uint8> | #; name : string; var t : Transformer; woff, n : int64; var scratch : array<float>) {
    if (t.quant == QuantMode.q8) {
        if (gguf_tensor_type(m, name) == GGML_TYPE_Q8_0) {
            gguf_transcode_q8_0(m, bytes, name, t.qblob, t.qscales, woff, woff / 32l, n)
        } else {
            scratch |> resize(n)
            gguf_read_tensor_f32(m, bytes, name, scratch, 0l, n)
            quantize_q8_0_into(scratch, n, t.qblob, t.qscales, woff, woff / 32l)
        }
    } elif (t.quant == QuantMode.q4) {
        if (gguf_tensor_type(m, name) == GGML_TYPE_Q4_0) {
            gguf_transcode_q4_0(m, bytes, name, t.q4blob, t.q4scales, woff / 2l, woff / 32l, n)
        } else {
            scratch |> resize(n)
            gguf_read_tensor_f32(m, bytes, name, scratch, 0l, n)
            quantize_q4_0_into(scratch, n, t.q4blob, t.q4scales, woff / 2l, woff / 32l)
        }
    } else {
        gguf_read_tensor_f32(m, bytes, name, t.wblob, woff, n)
    }
}

//! Load a llama-architecture GGUF into the split layout at the requested precision. The big weights
//! go straight into `mode`'s storage (pre-quantized tensors transcode block-for-block with no fp32
//! round-trip; F16/F32 tensors dequant one at a time through a small reused scratch), so a Q8/Q4
//! load never holds the dense fp32 weights. The embedding + norms stay fp32 in fblob regardless.
//! Config comes from metadata; vocab is derived from token_embd (some files omit llama.vocab_size).
def load_gguf(path : string; mode : QuantMode = QuantMode.fp32) : Transformer {
    var t : Transformer
    t.quant = mode
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let m <- parse_gguf_meta(bytes)
        let arch = gguf_str(m, bytes, "general.architecture")
        if (arch != "llama") {
            panic("dasLLAMA: unsupported architecture '{arch}' (only llama)")
        }
        let dim = gguf_int(m, bytes, "llama.embedding_length")
        let hidden = gguf_int(m, bytes, "llama.feed_forward_length")
        let layers = gguf_int(m, bytes, "llama.block_count")
        let n_heads = gguf_int(m, bytes, "llama.attention.head_count")
        let n_kv_heads = gguf_int(m, bytes, "llama.attention.head_count_kv")
        let seq_len = gguf_int(m, bytes, "llama.context_length")
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
        t.config.head_size = dim / n_heads
        t.config.kv_dim = (dim * n_kv_heads) / n_heads
        // a separate output.weight means the classifier is not tied to the embeddings
        t.config.shared_weights = gguf_find_tensor(m, "output.weight") < 0
        let kv = t.config.kv_dim
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
        }
        gguf_read_tensor_f32(m, bytes, "output_norm.weight", t.fblob, t.rms_final_off, dim)
        // big 2D weights -> active precision. ggml stores them row-major [out x in] = our convention.
        var scratch : array<float>
        for (l in range64(layers)) {
            load_big(m, bytes, "blk.{l}.attn_q.weight", t, t.wq_off + l * dim * dim, dim * dim, scratch)
            load_big(m, bytes, "blk.{l}.attn_k.weight", t, t.wk_off + l * dim * kv, dim * kv, scratch)
            load_big(m, bytes, "blk.{l}.attn_v.weight", t, t.wv_off + l * dim * kv, dim * kv, scratch)
            load_big(m, bytes, "blk.{l}.attn_output.weight", t, t.wo_off + l * dim * dim, dim * dim, scratch)
            load_big(m, bytes, "blk.{l}.ffn_gate.weight", t, t.w1_off + l * dim * hidden, dim * hidden, scratch)
            load_big(m, bytes, "blk.{l}.ffn_down.weight", t, t.w2_off + l * hidden * dim, hidden * dim, scratch)
            load_big(m, bytes, "blk.{l}.ffn_up.weight", t, t.w3_off + l * dim * hidden, dim * hidden, scratch)
        }
        if (!t.config.shared_weights) {
            load_big(m, bytes, "output.weight", t, t.wcls_off, vocab * dim, scratch)
        }
        delete scratch
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
    att : array<float>        // attention scores for one head (seq_len)
    logits : array<float>     // output logits (vocab_size)
    key_cache : array<float>  // (n_layers * seq_len * kv_dim)
    value_cache : array<float>
}

//! Allocate the run state sized to a model.
def make_run_state(c : Config) : RunState {
    var s : RunState
    s.x |> resize(c.dim)
    s.xb |> resize(c.dim)
    s.xb2 |> resize(c.dim)
    s.hb |> resize(c.hidden_dim)
    s.hb2 |> resize(c.hidden_dim)
    s.q |> resize(c.dim)
    s.k |> resize(c.kv_dim)
    s.v |> resize(c.kv_dim)
    s.att |> resize(c.seq_len)
    s.logits |> resize(c.vocab_size)
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
        rmsnorm(o, x, w, size)
    }
}

// Q8 matmul with the weight viewed into the quantized blob (int8) + its per-block scales. woff is the
// weight-element offset; scales start at woff/32 (woff is block-aligned for every 2D tensor).
def private mm_at_q8(var y : array<float>; t : Transformer; woff : int64; x : array<float>; n, d : int64) {
    let total = n * d
    array_view(t.qblob, woff, total) $(wq : array<int8>#) {
        array_view(t.qscales, woff / 32l, total / 32l) $(ws : array<float>#) {
            matmul_q8(y, wq, ws, x, n, d)
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

// Route a big-weight matmul through the Q8, Q4, or fp32 kernel depending on the model's quant mode.
def private mm(var y : array<float>; t : Transformer; woff : int64; x : array<float>; n, d : int64) {
    if (t.quant == QuantMode.q8) {
        mm_at_q8(y, t, woff, x, n, d)
    } elif (t.quant == QuantMode.q4) {
        mm_at_q4(y, t, woff, x, n, d)
    } else {
        mm_at(y, t, woff, x, n, d)
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
    let kv_mul = n_heads / c.n_kv_heads
    let seq_len = c.seq_len
    let scale = 1.0 / sqrt(float(head_size))

    // token embedding: copy row `token` of the fp32 embedding table into the residual stream
    array_view(t.fblob, t.tok_emb_off + token * dim, dim) $(row : array<float>#) {
        for (i in range64(dim)) {
            s.x[i] = row[i]
        }
    }

    for (l in range64(c.n_layers)) {
        let loff = l * seq_len * kv_dim
        // attention rmsnorm
        rms_at(s.xb, t, t.rms_att_off + l * dim, s.x, dim)
        // q, k, v projections
        mm(s.q, t, t.wq_off + l * dim * dim, s.xb, dim, dim)
        mm(s.k, t, t.wk_off + l * dim * kv_dim, s.xb, dim, kv_dim)
        mm(s.v, t, t.wv_off + l * dim * kv_dim, s.xb, dim, kv_dim)
        // RoPE on q (dim) and k (kv_dim)
        rope(s.q, pos, head_size, dim)
        rope(s.k, pos, head_size, kv_dim)
        // store k, v for this position into the KV cache
        let kvoff = loff + pos * kv_dim
        for (i in range64(kv_dim)) {
            s.key_cache[kvoff + i] = s.k[i]
            s.value_cache[kvoff + i] = s.v[i]
        }
        // multi-head attention
        for (h in range64(n_heads)) {
            let qoff = h * head_size
            let kvhoff = (h / kv_mul) * head_size  // GQA: which kv head this query head reads
            // scores against all cached keys 0..pos, scaled by 1/sqrt(head_size)
            for (tp in range64(pos + 1l)) {
                let koff = loff + tp * kv_dim + kvhoff
                s.att[tp] = dot(unsafe(addr(s.q[qoff])), unsafe(addr(s.key_cache[koff])), head_size) * scale
            }
            softmax(s.att, pos + 1l)
            // weighted sum of values into xb[qoff..]
            for (i in range64(head_size)) {
                s.xb[qoff + i] = 0.0
            }
            for (tp in range64(pos + 1l)) {
                let voff = loff + tp * kv_dim + kvhoff
                axpy(unsafe(addr(s.xb[qoff])), unsafe(addr(s.value_cache[voff])), s.att[tp], head_size)
            }
        }
        // output projection, residual
        mm(s.xb2, t, t.wo_off + l * dim * dim, s.xb, dim, dim)
        for (i in range64(dim)) {
            s.x[i] += s.xb2[i]
        }
        // ffn rmsnorm
        rms_at(s.xb, t, t.rms_ffn_off + l * dim, s.x, dim)
        // SwiGLU: hb = silu(W1·xb) * (W3·xb)
        mm(s.hb, t, t.w1_off + l * dim * hidden, s.xb, dim, hidden)
        mm(s.hb2, t, t.w3_off + l * dim * hidden, s.xb, dim, hidden)
        silu(s.hb, hidden)
        for (i in range64(hidden)) {
            s.hb[i] *= s.hb2[i]
        }
        // down projection, residual
        mm(s.xb, t, t.w2_off + l * hidden * dim, s.hb, hidden, dim)
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
        mm(s.logits, t, t.wcls_off, s.xb, dim, c.vocab_size)
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
    var token = prompt_tokens[0]
    for (pos in range64(steps)) {
        forward(t, s, token, pos)
        var next = 0l
        if (pos < n_prompt - 1l) {
            next = prompt_tokens[pos + 1l]  // still consuming the prompt
        } else {
            next = argmax(s, t.config.vocab_size)
        }
        if (next == 1l) {  // BOS delimits sequences
            break
        }
        ids |> push(next)
        token = next
    }
    return <- ids
}
