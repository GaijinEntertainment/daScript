options gen2

module dasllama_transformer shared public

require daslib/fio
require daslib/array_boost // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs and false-flags this as unused
require dasllama_math
require dasllama_quant
require math

// dasLLAMA inference engine. Loads a llama2.c checkpoint into one fp32 blob and
// exposes weight tensors as zero-copy views (array_view) for the forward pass.

//! Which weight representation forward routes its 2D matmuls through. fp32 is the default;
//! q8/q4 select the quantized kernels once the matching blob has been built.
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
    shared_weights : bool   // wcls aliases the token-embedding table
}

//! The whole model: one fp32 blob plus base offsets into it (float-element units),
//! mirroring llama2.c's memory_map_weights pointer layout.
struct Transformer {
    config : Config
    blob : array<float>
    tok_emb_off : int64
    rms_att_off : int64
    wq_off : int64
    wk_off : int64
    wv_off : int64
    wo_off : int64
    rms_ffn_off : int64
    w1_off : int64
    w2_off : int64
    w3_off : int64
    rms_final_off : int64
    wcls_off : int64
    // Quantized copies of the whole blob (parallel layout, so the same float offsets index them).
    // Q8: int8 quants + one fp32 scale per 32-block (scale index = offset / 32). Q4: 4-bit quants
    // packed two per byte (byte offset = offset / 2) + one fp32 scale per block. Populated by
    // quantize_weights / quantize_weights_q4; `quant` selects which kernel forward routes the 2D
    // weight matmuls through (norms/embedding always stay fp32, matching the GGUF 1D-norm rule).
    qblob : array<int8>
    qscales : array<float>
    q4blob : array<uint8>
    q4scales : array<float>
    quant : QuantMode
}

//! Load a llama2.c checkpoint: 7-int32 header, then fp32 weights in fixed order.
def load_checkpoint(path : string) : Transformer {
    let HEADER_INTS = 7
    let HEADER_BYTES = 28
    var t : Transformer
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
            // copy the fp32 weight blob (everything past the header) into an owned array
            let n_floats = (length(bytes) - HEADER_BYTES) / 4
            t.blob |> resize(n_floats)
            let src = temp_array(reinterpret<float?>(addr(bytes[HEADER_BYTES])), n_floats, type<float>)
            for (i in range(n_floats)) {
                t.blob[i] = src[i]
            }
        }
    }
    fclose(f)
    // derived sizes
    t.config.head_size = t.config.dim / t.config.n_heads
    t.config.kv_dim = (t.config.dim * t.config.n_kv_heads) / t.config.n_heads
    // weight base offsets, in float-element units (mirrors memory_map_weights)
    let dim = t.config.dim
    let hidden = t.config.hidden_dim
    let layers = t.config.n_layers
    let kv = t.config.kv_dim
    let qd = t.config.n_heads * t.config.head_size
    var off = 0l
    t.tok_emb_off = off
    off += t.config.vocab_size * dim
    t.rms_att_off = off
    off += layers * dim
    t.wq_off = off
    off += layers * dim * qd
    t.wk_off = off
    off += layers * dim * kv
    t.wv_off = off
    off += layers * dim * kv
    t.wo_off = off
    off += layers * qd * dim
    t.rms_ffn_off = off
    off += layers * dim
    t.w1_off = off
    off += layers * dim * hidden
    t.w2_off = off
    off += layers * hidden * dim
    t.w3_off = off
    off += layers * dim * hidden
    t.rms_final_off = off
    off += dim
    off += t.config.seq_len * t.config.head_size / 2l   // skip old freq_cis_real
    off += t.config.seq_len * t.config.head_size / 2l   // skip old freq_cis_imag
    t.wcls_off = t.config.shared_weights ? t.tok_emb_off : off
    return <- t
}

//! Quantize all weights to Q8_0 (the whole blob, parallel layout) and switch forward onto the Q8
//! matmul path. The 1D norm weights live in the blob too but forward never routes them through Q8 —
//! only the 2D weight matrices and the classifier. Set t.quant back to QuantMode.fp32 to A/B against
//! the fp32 path on the same model.
def quantize_weights(var t : Transformer) {
    let n = int64(length(t.blob))
    var qt <- quantize_q8_0(t.blob, n)
    t.qblob <- qt.q
    t.qscales <- qt.scales
    t.quant = QuantMode.q8
}

//! Quantize all weights to Q4_0 (GGML split-half layout) and switch forward onto the Q4 matmul
//! path. Same all-2D-weights scope as quantize_weights; ~8x fewer weight bytes than fp32, lossier.
def quantize_weights_q4(var t : Transformer) {
    let n = int64(length(t.blob))
    var qt <- quantize_q4_0(t.blob, n)
    t.q4blob <- qt.q
    t.q4scales <- qt.scales
    t.quant = QuantMode.q4
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

// matmul with the weight as a zero-copy view into the model blob at `woff` (len n*d).
def private mm_at(var y : array<float>; t : Transformer; woff : int64; x : array<float>; n, d : int64) {
    array_view(t.blob, woff, n * d) $(w : array<float>#) {
        matmul(y, w, x, n, d)
    }
}

// rmsnorm with the weight as a zero-copy view into the model blob at `woff` (len size).
def private rms_at(var o : array<float> | #; t : Transformer; woff : int64; x : array<float> | #; size : int64) {
    array_view(t.blob, woff, size) $(w : array<float>#) {
        rmsnorm(o, x, w, size)
    }
}

// Q8 matmul with the weight viewed into the quantized blob (int8) + its per-block scales.
// woff is the same float-element offset as the fp32 path; scales start at woff/32 (woff is
// block-aligned for every 2D tensor since dims are multiples of 32).
def private mm_at_q8(var y : array<float>; t : Transformer; woff : int64; x : array<float>; n, d : int64) {
    let total = n * d
    array_view(t.qblob, woff, total) $(wq : array<int8>#) {
        array_view(t.qscales, woff / 32l, total / 32l) $(ws : array<float>#) {
            matmul_q8(y, wq, ws, x, n, d)
        }
    }
}

// Q4 matmul with the weight viewed into the quantized blob (4-bit, two per byte) + its per-block
// scales. woff is the same float-element offset as the fp32 path; bytes start at woff/2 (woff is
// even — block-aligned for every 2D tensor), scales at woff/32.
def private mm_at_q4(var y : array<float>; t : Transformer; woff : int64; x : array<float>; n, d : int64) {
    let total = n * d
    array_view(t.q4blob, woff / 2l, total / 2l) $(wq : array<uint8>#) {
        array_view(t.q4scales, woff / 32l, total / 32l) $(ws : array<float>#) {
            matmul_q4(y, wq, ws, x, n, d)
        }
    }
}

// Route a weight matmul through the Q8, Q4, or fp32 kernel depending on the model's quant mode.
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
//! Faithful port of llama2.c forward(); weights are viewed into the blob, KV cache grows by pos.
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

    // token embedding: copy row `token` of the embedding table into the residual stream
    array_view(t.blob, t.tok_emb_off + token * dim, dim) $(row : array<float>#) {
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
    mm(s.logits, t, t.wcls_off, s.xb, dim, c.vocab_size)
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

