options gen2

module dasllama_transformer shared public

require daslib/fio
require daslib/array_boost // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs and false-flags this as unused

// dasLLAMA inference engine. Loads a llama2.c checkpoint into one fp32 blob and
// exposes weight tensors as zero-copy views (array_view) for the forward pass.

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
