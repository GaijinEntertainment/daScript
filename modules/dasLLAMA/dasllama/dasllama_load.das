options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_load shared public

// THE GGUF LOAD WALK: metadata -> Config, the plane layout, disk-format detection, the eager and
// streamed conversion ladders, and the load entry points every caller reaches the engine through.
// It owns nothing the forward path touches at run time — a loaded Model is the whole handoff.
// It requires dasllama_common back for Model/Session, so the transformer umbrella requires it and
// breaks the cycle, exactly like the GPU residency rail and the Metal drivers.

require dasllama/dasllama_lint public
require dasllama/dasllama_ple   // ple_check_table — the E-series table go-live tripwire
require dasllama/dasllama_common
require dasllama/dasllama_kqformat
require dasllama/dasllama_gguf
require dasllama/dasllama_convert
require dasllama/dasllama_math
require dasllama/dasllama_gemm_schema
require dasllama/dasllama_tune
require dasllama/dasllama_env
require dasllama/dasllama_par
require dasllama/dasllama_gpu_tier
require llvm/daslib/f16_cvt
require daslib/json_boost
require daslib/array_boost
require dasllama/dasllama_tokenizer   // load_tokenizer_auto — load_model_ populates the Model's tokenizer
require daslib/fio
require strings
require math


struct private LayoutSizes {
    fblob_n : int64   // element count of the fp32 aux region
    wblob_n : int64   // element count of the big-weight region
    mx_n : int64      // element count of the native-MXFP4 expert planes (0 unless experts_mx4)
    bf16_n : int64    // halfword count of the native-BF16 region (0 unless ple_model_bf16)
    k4_n : int64      // element counts of the native K-quant planes (0 unless kquant_native)
    k5_n : int64
    k6_n : int64
    q40_n : int64
    q51_n : int64
    pleq8_n : int64   // the dedicated q8 plane holding ONLY the PLE table (fp32/q4 serving)
}

// The big-weight layout cursors: one element cursor per storage plane. kq_take advances the
// cursor a weight's format tag selects, so layout_offsets stays a single linear walk.
struct private KqCursors {
    wo : int64   // qblob / wblob / q4blob (the classic parallel-offset region)
    k4 : int64
    k5 : int64
    k6 : int64
    q40 : int64
    q51 : int64
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
    if (f == KqFmt.q51) {
        let o = cur.q51
        cur.q51 += n
        return o
    }
    let o = cur.wo
    cur.wo += n
    return o
}



var private g_blob_scale_plane : BlobScalePlaneFn

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

def private count_fmt(a : array<KqFmt>; f : KqFmt) : int {
    var n = 0
    for (x in a) {
        if (x == f) {
            n++
        }
    }
    return n
}

var private g_blob_hooks_set = false


// Fill both offset tables from the config; return the element counts of each region. Offsets are
// 32-aligned (every dim/kv/hidden is a multiple of 32), so Q8/Q4 scale/byte indices divide cleanly.
// Attention tensors get PER-LAYER offsets (shapes differ per layer class on gemma4).
def private layout_offsets(var t : Model) : LayoutSizes {   // nolint:STYLE037,STYLE038 — one linear offset walk; every region's offset depends on the running cursor
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
    if (c.recr_mask != 0ul) {  // deltanet big weights (recurrent layers only; kind-major regions; beta/alpha stay q8)
        let cd = dn_conv_dim(c)
        t.dnqkv_offs |> clear(); t.dnqkv_offs |> reserve(layers)
        for (l in range64(layers)) { t.dnqkv_offs |> push(layer_is_recurrent(c, l) ? kq_take(cur, fmt_at(t.dnqkv_fmt, l), dim * cd) : -1l) }
        t.dngate_offs |> clear(); t.dngate_offs |> reserve(layers)
        for (l in range64(layers)) { t.dngate_offs |> push(layer_is_recurrent(c, l) ? kq_take(cur, fmt_at(t.dngate_fmt, l), dim * c.ssm_d_inner) : -1l) }
        t.dnbeta_offs |> clear(); t.dnbeta_offs |> reserve(layers)
        for (l in range64(layers)) { t.dnbeta_offs |> push(layer_is_recurrent(c, l) && !t.dn_ba_f32 ? kq_take(cur, KqFmt.q8, dim * c.ssm_dt_rank) : -1l) }
        t.dnalpha_offs |> clear(); t.dnalpha_offs |> reserve(layers)
        for (l in range64(layers)) { t.dnalpha_offs |> push(layer_is_recurrent(c, l) && !t.dn_ba_f32 ? kq_take(cur, KqFmt.q8, dim * c.ssm_dt_rank) : -1l) }
        t.dnout_offs |> clear(); t.dnout_offs |> reserve(layers)
        for (l in range64(layers)) { t.dnout_offs |> push(layer_is_recurrent(c, l) ? kq_take(cur, fmt_at(t.dnout_fmt, l), c.ssm_d_inner * dim) : -1l) }
    }
    var mx = 0l
    var pleq8 = 0l
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
        // gather-only rows, NOT repacked — pinned to the DISK-format plane in every serving mode (the mode owns the wo region, never the table)
        if (t.ple_emb_fmt == KqFmt.q8 && t.quant != QuantMode.q8) {
            t.ple_emb_off = 0l
            pleq8 = c.vocab_size * ple * layers
        } else {
            t.ple_emb_off = kq_take(cur, t.ple_emb_fmt, c.vocab_size * ple * layers)
        }
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
                       k4_n = cur.k4, k5_n = cur.k5, k6_n = cur.k6, q40_n = cur.q40, q51_n = cur.q51, pleq8_n = pleq8)
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

// ===== conversion-time accounting =====
// load_big's branches ARE the conversion kinds, so bucketing here answers "where does gguf -> image
// time go". DASLLAMA_CONV_PROF arms it: one clock pair per tensor, a dead branch when off.

struct private ConvBucket {
    nsec : int64
    elems : int64
    calls : int64
}

struct private ConvRow {
    name : string
    nsec : int64
    elems : int64
    calls : int64
}

var private g_conv_prof = false
var private g_conv : table<string; ConvBucket>

//! Arm per-conversion-kind timing over the gguf -> image weight walk (env DASLLAMA_CONV_PROF=1
//! arms it at load). Buckets reset on each arm; dump them with [[conv_profile_report]].
def set_conv_profile(on : bool) {
    g_conv_prof = on
    g_conv |> clear()
}

def private conv_note(kind : string; t0, n : int64) {
    if (!g_conv_prof) {
        return
    }
    let ns = get_time_nsec(t0)
    var b = g_conv[kind]
    b.nsec += ns
    b.elems += n
    b.calls++
    g_conv[kind] = b
}

//! Dump the conversion-kind breakdown gathered since the last [[set_conv_profile]] arm, slowest
//! first: wall ms, share of the accounted total, weights moved, and the arm's throughput.
def conv_profile_report() {
    if (!g_conv_prof || empty(g_conv)) {
        return
    }
    var rows : array<ConvRow>
    var total = 0l
    for (k, v in keys(g_conv), values(g_conv)) {
        rows |> emplace(ConvRow(name = clone_string(k), nsec = v.nsec, elems = v.elems, calls = v.calls))
        total += v.nsec
    }
    rows |> sort() $(a, b) => a.nsec > b.nsec
    to_log(LOG_INFO, "dasLLAMA conv: {length(rows)} kinds, {double(total) / 1.0e6lf:.0f} ms accounted\n")
    for (r in rows) {
        let ms = double(r.nsec) / 1.0e6lf
        let pct = total > 0l ? 100.0lf * double(r.nsec) / double(total) : 0.0lf
        let mws = r.nsec > 0l ? double(r.elems) * 1000.0lf / double(r.nsec) : 0.0lf
        to_log(LOG_INFO, "dasLLAMA conv:   {r.name}: {ms:.0f} ms ({pct:.1f}%), "
            + "{double(r.elems) / 1.0e6lf:.0f} Mw over {r.calls} tensors, {mws:.0f} Mw/s\n")
    }
    delete rows
}

// ===== streaming plan/fill (phase D of the .dlim arc) =====

//! One recorded big-tensor conversion: load_big's call args, replayable — the fill pass
//! re-derives the branch from (fmt, quant, disk type), so plan and fill cannot drift.
struct FillJob {
    name : string
    woff : int64
    n : int64
    src_off : int64
    fmt : KqFmt
}

//! One repack region over a plane (element offset + 2D shape). fmt ids follow the repack
//! walkers: 0 = q8, 1 = mx4, 2 = q51, 4/5/6/40 = the kq plane pairs.
struct StreamReg {
    off : int64
    n : int64
    d : int64
    fmt : int
}

typedef StreamCollectFn = function<(t : Model; var regs : array<StreamReg>) : void>
typedef StreamRepackFn = function<(fmt : int; qp, sp : void?; n, d : int64) : void>
typedef BlobEligibleFn = function<(t : Model) : bool>
typedef BlobScalePlaneFn = function<(var t : Model; name : string) : bool>

[unused_argument(t, regs)]
def private stream_no_collect(t : Model; var regs : array<StreamReg>) {
    panic("dasLLAMA: no stream-layout hooks registered (dasllama_layout's [init] provides them)")
}

[unused_argument(fmt, qp, sp, n, d)]
def private stream_no_repack(fmt : int; qp, sp : void?; n, d : int64) {
    panic("dasLLAMA: no stream-repack hook registered (dasllama_layout's [init] provides it)")
}

var private g_stream_collect_q8 = @@stream_no_collect
var private g_stream_collect_kq = @@stream_no_collect
var private g_stream_collect_q51 = @@stream_no_collect
var private g_stream_repack = @@stream_no_repack
var private g_blob_eligible : BlobEligibleFn

//! dasllama_layout registers the streaming-fill collaborators at [init]: the three region
//! collectors (the repack walkers' build halves), the single-region repack dispatcher, and the
//! metal-blob pair the streamed build consults to mint the blob flavor straight off the gguf.
def register_stream_layout(cq8, ckq, cq51 : StreamCollectFn; rr : StreamRepackFn;
                           be : BlobEligibleFn; bsp : BlobScalePlaneFn) {
    g_stream_collect_q8 = cq8
    g_stream_collect_kq = ckq
    g_stream_collect_q51 = cq51
    g_stream_repack = rr
    g_blob_eligible = be
    g_blob_scale_plane = bsp
    g_blob_hooks_set = true
}

//! Can this model's streamed build mint the metal blob flavor? False on an unregistered layout
//! module too — a caller without the hooks takes the planar rail rather than a wrong image.
def blob_stream_eligible(t : Model) : bool {
    return g_blob_hooks_set && invoke(g_blob_eligible, t)
}

//! Put one kq scale plane into its device form during a blob build (see metal_blob_scale_plane).
def blob_stream_scale_plane(var t : Model; name : string) : bool {
    return g_blob_hooks_set && invoke(g_blob_scale_plane, t, name)
}

var private g_fill_plan_mode = false
// the streamed build should mint the METAL BLOB flavor, not the planar one. Set for the length of
// one load_gguf_streaming call; the sizing pass turns it into t.metal_blob once offsets exist.
var private g_stream_want_blob = false
var private g_fill_plan : array<FillJob>
// field -> the plane's LAYOUT byte total, recorded at sizing: a layout can hold allocated-but-
// never-written holes (glm4moe's dense-lead shexp slices), so the streamed plane must be sized
// and gap-filled from the layout — a job-sum concatenation SHIFTS everything past the first hole
var private g_stream_plane_total : table<string; int64>

//! The regions the eager repack stage WOULD build for `t` — same gates, same walkers (the
//! collector halves), so the streamed fill's per-job repack cannot drift from the eager one.
def stream_collect_regions(t : Model; var regs : array<StreamReg>) {
    // metal_blob first, and short-circuiting: the blob flavor is device-layout bytes, so a CPU
    // repack has no place in it and the backend must not even be selected for one
    if (t.metal_blob || t.quant != QuantMode.q8 || !select_matmul_backend_for_load_()) {
        return
    }
    invoke(g_stream_collect_q8, t, regs)
    if (active_q51_layout_mr() > 1l) {
        invoke(g_stream_collect_q51, t, regs)
    }
    if (t.kquant_native && kernel_backend_has_kq()) {
        invoke(g_stream_collect_kq, t, regs)
    }
}

// the streamed destination field of a job (Model plane field name), "" = not streamed
def private stream_field_of(t : Model; fmt : KqFmt) : string {
    if (fmt == KqFmt.k4) {
        return "k4q"
    } elif (fmt == KqFmt.k5) {
        return "k5q"
    } elif (fmt == KqFmt.k6) {
        return "k6q"
    } elif (fmt == KqFmt.q40) {
        return "q40q"
    } elif (fmt == KqFmt.q51) {
        return "q51q"
    } elif (t.quant == QuantMode.q8) {
        // the blob flavor carries q8 as gguf-native 34B blocks in ONE plane, never the split pair
        return t.metal_blob ? "mblob" : "qblob"
    }
    return ""
}

// (q plane bytes, scale plane bytes, region fmt id) for one streamed job — pure taxonomy reads
def private stream_job_shape(t : Model; j : FillJob) : tuple<qb : int64; sb : int64; rfmt : int> {
    if (kq_sb(j.fmt) || j.fmt == KqFmt.q51) {
        let e = kq_elems(j.fmt)
        return (qb = (j.n / e) * kq_qsb(j.fmt), sb = (j.n / e) * kq_ssb(j.fmt), rfmt = kq_stream_code(j.fmt))
    }
    if (t.metal_blob) {   // scale and quants land interleaved, so there is no separate scale half
        return (qb = (j.n / Q8_BLOCK_ELEMS) * (Q8_QPB + Q8_SPB), sb = 0l, rfmt = 0)
    }
    return (qb = j.n, sb = 0l, rfmt = 0)   // q8: 1 byte/elem quants; scales handled as floats
}

// a job's byte offset within its plane (woff is elems; kq/q51 planes stride per superblock/block)
def private stream_job_qoff(t : Model; j : FillJob) : int64 {
    if (kq_sb(j.fmt) || j.fmt == KqFmt.q51) {
        return (j.woff / kq_elems(j.fmt)) * kq_qsb(j.fmt)
    }
    if (t.metal_blob) {
        return (j.woff / Q8_BLOCK_ELEMS) * (Q8_QPB + Q8_SPB)
    }
    return j.woff
}

//! Total bytes the streamed fill will append for `field` (page padding NOT included) — the
//! LAYOUT size when the sizing pass recorded one (holes included), else the job sum.
def stream_plane_bytes(t : Model; jobs : array<FillJob>; field : string) : int64 {
    if (empty(jobs)) return 0l   // no plan: the save must degrade to the eager walk EXACTLY (its contract)
    let lt = g_stream_plane_total?[field] ?? 0l
    if (lt > 0l) {
        return lt
    }
    var total = 0l
    for (j in jobs) {
        if (stream_field_of(t, j.fmt) == field) {
            total += stream_job_shape(t, j).qb
        }
    }
    return total
}

// append `gap` zero bytes — a layout hole the jobs never cover (never-written plane space)
def private stream_zero_fill(gap : int64; var zeros : array<uint8>;
                             append : block<(p : void?; nbytes : uint64) : void>) {
    var left = gap
    if (left > 0l && empty(zeros)) {
        zeros |> resize(1048576)
    }
    while (left > 0l) {
        let chunk = min(left, long_length(zeros))
        unsafe {
            invoke(append, addr<void?>(zeros[0]), uint64(chunk))
        }
        left -= chunk
    }
}

//! Execute every recorded job targeting `field`: transcode into a job-sized temp (load_big's
//! own arms at temp offset 0), land scales in the RAM plane, apply the job's repack regions
//! (temp-biased q, in-place s), hand the temp to `append`; qblob's tail runs the wscale epilogue.
def fill_stream_plane(m : GGUFMeta; bytes : array<uint8> | #; var t : Model; field : string;   // nolint:STYLE037,STYLE038 — per-format transcode arms sharing one temp buffer and the append block
                      jobs : array<FillJob>; regs : array<StreamReg>;
                      append : block<(p : void?; nbytes : uint64) : void>) : bool {
    // jobs for this field, ascending woff — plane order IS ascending source order
    var mine : array<FillJob>
    for (j in jobs) {
        if (stream_field_of(t, j.fmt) == field) {
            mine |> push(j)
        }
    }
    if (empty(mine)) {
        return true
    }
    mine |> sort() $(a, b : FillJob) => a.woff < b.woff
    var temp_q : array<uint8>
    var temp_q8 : array<int8>   // the q8 family's quant temp (qblob is int8)
    var temp_s : array<uint8>
    var temp_sf : array<float>
    var temp_m : array<uint8>   // the blob flavor's interleaved 34B blocks
    var temp_s16 : array<uint16>
    var scratch : array<float>
    var zeros : array<uint8>
    var cursor = 0l   // plane bytes appended so far — layout holes between jobs zero-fill
    for (j in mine) {
        let sh = stream_job_shape(t, j)
        stream_zero_fill(stream_job_qoff(t, j) - cursor, zeros, append)
        cursor = stream_job_qoff(t, j) + sh.qb
        temp_q |> grow_resize(int(sh.qb))
        let t0 = g_conv_prof ? ref_time_ticks() : 0l
        var kind = ""
        if (kq_sb(j.fmt) || j.fmt == KqFmt.q51) {
            temp_s |> grow_resize(int(sh.sb))
            kind = transcode_kq_tensor(m, bytes, j.name, j.fmt, temp_q, temp_s, 0l, j.n, j.src_off, scratch)
        } else {
            // q8 family: scales are FLOATS; the epilogue converts the whole plane at the end
            temp_q8 |> grow_resize(int(j.n))
            temp_sf |> grow_resize(int(j.n / 32l))
            kind = transcode_q8_tensor(m, bytes, j.name, temp_q8, temp_sf, 0l, 0l, j.n, j.src_off, scratch)
        }
        conv_note(kind, t0, j.n)
        // land the scale half at its real offset (RAM-resident plane)
        unsafe {
            if (j.fmt == KqFmt.k4) {
                memcpy(addr<void?>(t.k4s[(j.woff / 256l) * K4_SSB]), addr<void?>(temp_s[0]), sh.sb)
            } elif (j.fmt == KqFmt.k5) {
                memcpy(addr<void?>(t.k5s[(j.woff / 256l) * K5_SSB]), addr<void?>(temp_s[0]), sh.sb)
            } elif (j.fmt == KqFmt.k6) {
                memcpy(addr<void?>(t.k6s[(j.woff / 256l) * K6_SSB]), addr<void?>(temp_s[0]), sh.sb)
            } elif (j.fmt == KqFmt.q40) {
                memcpy(addr<void?>(t.q40s[(j.woff / 256l) * Q40_SSB]), addr<void?>(temp_s[0]), sh.sb)
            } elif (j.fmt == KqFmt.q51) {
                memcpy(addr<void?>(t.q51s[(j.woff / 32l) * Q51_SB]), addr<void?>(temp_s[0]), sh.sb)
            } elif (sh.sb == 0l && j.n > 0l && !t.metal_blob) {
                memcpy(addr<void?>(t.qscales[j.woff / 32l]), addr<void?>(temp_sf[0]), (j.n / 32l) * 4l)
            }
        }
        // per-job repack: this job's regions applied on the temp (q half, pointer biased to
        // woff 0) and the real scale plane (in place) — the same backend fns, same regions
        unsafe {
            for (r in regs) {
                if (r.fmt != sh.rfmt || r.off < j.woff || r.off >= j.woff + j.n) {
                    continue
                }
                if (r.fmt == 0) {
                    invoke(g_stream_repack, 0, addr<void?>(temp_q8[r.off - j.woff]),
                        addr<void?>(t.qscales[r.off / 32l]), r.n, r.d)
                } elif (r.fmt == 2) {
                    invoke(g_stream_repack, 2, addr<void?>(temp_q[((r.off - j.woff) / 32l) * Q51_QPB]),
                        addr<void?>(t.q51s[(r.off / 32l) * Q51_SPB]), r.n, r.d)
                } elif (r.fmt == 4) {
                    invoke(g_stream_repack, 4, addr<void?>(temp_q[((r.off - j.woff) / 256l) * K4_QSB]),
                        addr<void?>(t.k4s[(r.off / 256l) * K4_SSB]), r.n, r.d)
                } elif (r.fmt == 5) {
                    invoke(g_stream_repack, 5, addr<void?>(temp_q[((r.off - j.woff) / 256l) * K5_QSB]),
                        addr<void?>(t.k5s[(r.off / 256l) * K5_SSB]), r.n, r.d)
                } elif (r.fmt == 6) {
                    invoke(g_stream_repack, 6, addr<void?>(temp_q[((r.off - j.woff) / 256l) * K6_QSB]),
                        addr<void?>(t.k6s[(r.off / 256l) * K6_SSB]), r.n, r.d)
                } elif (r.fmt == 40) {
                    invoke(g_stream_repack, 40, addr<void?>(temp_q[((r.off - j.woff) / 256l) * Q40_QSB]),
                        addr<void?>(t.q40s[(r.off / 256l) * Q40_SSB]), r.n, r.d)
                }
            }
        }
        unsafe {
            if (sh.rfmt != 0) {
                invoke(append, addr<void?>(temp_q[0]), uint64(sh.qb))
            } elif (t.metal_blob) {
                blob_pack_job(temp_q8, temp_sf, j.n / Q8_BLOCK_ELEMS, temp_m, temp_s16)
                invoke(append, addr<void?>(temp_m[0]), uint64(sh.qb))
            } else {
                invoke(append, addr<void?>(temp_q8[0]), uint64(sh.qb))
            }
        }
    }
    // the layout's tail past the last job (never-written space) completes the plane
    stream_zero_fill(stream_plane_bytes(t, jobs, field) - cursor, zeros, append)
    if (field == "qblob" && g_wscale_f16) {
        // the epilogue: scales are complete and repacked; qscales16 lands BEFORE its field
        // writes (qblob precedes qscales/qscales16 in Model field order)
        wscale_convert_f16(t)
    }
    delete mine
    delete temp_q
    delete temp_q8
    delete temp_s
    delete temp_sf
    delete temp_m
    delete temp_s16
    delete scratch
    delete zeros
    return true
}

// One job's q8 temps -> the gguf-native 34B blocks the blob plane carries. Scales convert through
// cvt_f32_to_f16, the same kernel wscale_convert_f16 uses, so a streamed blob plane is bit-equal
// to one the in-RAM transform builds from an already-converted qscales16.
def private blob_pack_job(q : array<int8>; s : array<float>; nblk : int64;
                          var dst : array<uint8>; var s16 : array<uint16>) {
    if (nblk <= 0l) {
        return
    }
    let mbb = Q8_QPB + Q8_SPB
    dst |> ensure_capacity(nblk * mbb)
    dst |> resize_no_init(nblk * mbb)
    s16 |> ensure_capacity(nblk)
    s16 |> resize_no_init(nblk)
    unsafe {
        cvt_f32_to_f16(addr(s16[0]), addr < float const? >(s[0]), nblk)
        var dp = addr(dst[0])
        let qp = addr < int8 const? >(q[0])
        let sp = addr < uint16 const? >(s16[0])
        let nch = int(clamp(nblk / 262144l, 1l, 64l))
        maybe_parallel_for(0, int(nblk / 32l) + (nblk % 32l != 0l ? 1 : 0), is_job_que_available() ? nch : 1) $(cb, ce) {
            unsafe {
                for (b in range64(int64(cb) * 32l, min(int64(ce) * 32l, nblk))) {
                    var ph = reinterpret<uint16?>(dp + b * mbb)
                    ph[0] = sp[b]
                    memcpy(reinterpret<void?>(dp + b * mbb + Q8_SPB), reinterpret<void const?>(qp + b * Q8_QPB), 32)
                }
            }
        }
    }
}

//! Arm plan recording: subsequent load_big calls append FillJobs instead of transcoding
//! (smalls still load eagerly — fblob is small and every flavor keeps it).
def fill_plan_begin {
    g_fill_plan_mode = true
    g_fill_plan |> clear()
    // plane totals are THIS plan's sizing — a load that disarms must not save under the previous one's
    g_stream_plane_total |> clear()
}

//! Disarm and move the recorded plan out (empty = nothing recorded).
def fill_plan_take(var out : array<FillJob>) {
    g_fill_plan_mode = false
    delete out   // a take replaces whatever the caller brought, so release it rather than drop it
    out <- g_fill_plan   // nolint:PERF030,LINT019 — released on the line above; the rule has no delete tracking, and fires only in downstream compiles
}

// ONE source for the (fmt, disk type) -> kq transcode decision. load_big and fill_stream_plane
// used to carry this tree as two independent copies that could drift; both now dispatch here.
// Destinations are caller-picked (Model planes at eloff=woff, or stream temps at eloff=0).
def private transcode_kq_tensor(m : GGUFMeta; bytes : array<uint8> | #; name : string; fmt : KqFmt;
                                var kq : array<uint8>; var ks : array<uint8>; eloff, n, src_off : int64;
                                var scratch : array<float>) : string {
    if (fmt == KqFmt.k4) {
        gguf_transcode_q4k(m, bytes, name, kq, ks, eloff, n, src_off)
        return "k4 transcode (Q4_K)"
    } elif (fmt == KqFmt.k5) {
        if (gguf_tensor_type(m, name) == GGML_TYPE_Q5_K) {
            gguf_transcode_q5k(m, bytes, name, kq, ks, eloff, n, src_off)
            return "k5 transcode (Q5_K)"
        }
        // Q5_0 source: no exact embedding exists, so dequant and re-encode (lossy)
        scratch |> grow_resize(n)
        gguf_read_tensor_f32(m, bytes, name, scratch, 0l, n, src_off)
        quantize_k5_plane(scratch, n, kq, ks, eloff)
        return "k5 RE-ENCODE (Q5_0 dequant+quant)"
    } elif (fmt == KqFmt.k6) {
        gguf_transcode_q6k(m, bytes, name, kq, ks, eloff, n, src_off)
        return "k6 transcode (Q6_K)"
    } elif (fmt == KqFmt.q40) {
        gguf_transcode_q40(m, bytes, name, kq, ks, eloff, n, src_off)
        return "q40 transcode (Q4_0)"
    }
    gguf_transcode_q51(m, bytes, name, kq, ks, eloff, n, src_off)
    return "q51 transcode (Q5_1)"
}

// The q8-family half of the same tree: verbatim Q8_0, exact Q5_0->Q8, or dequant+requant.
def private transcode_q8_tensor(m : GGUFMeta; bytes : array<uint8> | #; name : string;
                                var q : array<int8>; var s : array<float>; qoff, soff, n, src_off : int64;
                                var scratch : array<float>) : string {
    let gt = gguf_tensor_type(m, name)
    if (gt == GGML_TYPE_Q8_0) {
        gguf_transcode_q8_0(m, bytes, name, q, s, qoff, soff, n, src_off)
        return "q8 transcode (Q8_0 verbatim)"
    } elif (gt == GGML_TYPE_Q5_0) {
        gguf_transcode_q5_0_to_q8(m, bytes, name, q, s, qoff, soff, n, src_off)
        return "q8 transcode (Q5_0 exact)"
    }
    scratch |> grow_resize(n)
    gguf_read_tensor_f32(m, bytes, name, scratch, 0l, n, src_off)
    quantize_q8_0_into(scratch, n, q, s, qoff, soff)
    return "q8 REQUANT (dequant+quant)"
}

def private load_big(m : GGUFMeta; bytes : array<uint8> | #; name : string; var t : Model; woff, n : int64; var scratch : array<float>; src_off : int64 = 0l; fmt : KqFmt = KqFmt.q8) {
    if (g_fill_plan_mode) {
        g_fill_plan |> push(FillJob(name = name, woff = woff, n = n, src_off = src_off, fmt = fmt))
        return
    }
    let t0 = g_conv_prof ? ref_time_ticks() : 0l
    var kind = "f32 read (no quant)"
    if (fmt == KqFmt.k4) {
        kind = transcode_kq_tensor(m, bytes, name, fmt, t.k4q, t.k4s, woff, n, src_off, scratch)
    } elif (fmt == KqFmt.k5) {
        kind = transcode_kq_tensor(m, bytes, name, fmt, t.k5q, t.k5s, woff, n, src_off, scratch)
    } elif (fmt == KqFmt.k6) {
        kind = transcode_kq_tensor(m, bytes, name, fmt, t.k6q, t.k6s, woff, n, src_off, scratch)
    } elif (fmt == KqFmt.q40) {
        kind = transcode_kq_tensor(m, bytes, name, fmt, t.q40q, t.q40s, woff, n, src_off, scratch)
    } elif (fmt == KqFmt.q51) {
        kind = transcode_kq_tensor(m, bytes, name, fmt, t.q51q, t.q51s, woff, n, src_off, scratch)
    } elif (t.quant == QuantMode.q8) {
        kind = transcode_q8_tensor(m, bytes, name, t.qblob, t.qscales, woff, woff / 32l, n, src_off, scratch)
    } elif (t.quant == QuantMode.q4_0) {
        if (gguf_tensor_type(m, name) == GGML_TYPE_Q4_0) {
            gguf_transcode_q4_0(m, bytes, name, t.q4blob, t.q4scales, woff / 2l, woff / 32l, n, src_off)
            kind = "q4_0 transcode (Q4_0 verbatim)"
        } else {
            scratch |> grow_resize(n)
            gguf_read_tensor_f32(m, bytes, name, scratch, 0l, n, src_off)
            quantize_q4_0_into(scratch, n, t.q4blob, t.q4scales, woff / 2l, woff / 32l)
            kind = "q4_0 REQUANT (dequant+quant)"
        }
    } else {
        gguf_read_tensor_f32(m, bytes, name, t.wblob, woff, n, src_off)
    }
    conv_note(kind, t0, n)
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
    raw |> reserve_resize(n)
    gguf_read_tensor_f32(m, bytes, "blk.{l}.ssm_out.weight", raw, 0l, n)
    scratch |> grow_resize(n)
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



def private tally_fmt(t : Model; f : KqFmt) : int {
    return (count_fmt(t.wq_fmt, f) + count_fmt(t.wk_fmt, f) + count_fmt(t.wv_fmt, f)
        + count_fmt(t.wo_fmt, f) + count_fmt(t.w1_fmt, f) + count_fmt(t.w2_fmt, f)
        + count_fmt(t.w3_fmt, f) + count_fmt(t.we1_fmt, f) + count_fmt(t.we2_fmt, f)
        + count_fmt(t.we3_fmt, f))
}

// The load's self-report: knob states, per-tensor format tags, and plane bytes. Plane bytes are
// the ONLY direct evidence that a format tag actually engaged — a timing delta can never prove it.
def private log_load_report(t : Model) {
    if (!dasllama_noisy()) {
        return
    }
    let mb = 1048576.0
    to_log(LOG_INFO, "dasLLAMA noisy: backend '{active_kernel_backend()}' | kquant_native {get_kquant_native()} q40_native {get_kq_q40_native()} q50_native {get_kq_q50_native()}\n")
    // the tier arms at [init], before a --noisy CLI flag can land — restate it here so the flag alone suffices
    to_log(LOG_INFO, "dasLLAMA noisy: GPU tier installed {moe_gpu_tier_installed()} | want auto {gpu_want_auto()}, moe layers {gpu_want_moe_layers()}, stream {gpu_want_moe_stream()}\n")
    to_log(LOG_INFO, "dasLLAMA noisy: format tags - q8 {tally_fmt(t, KqFmt.q8)}, k4 {tally_fmt(t, KqFmt.k4)}, k5 {tally_fmt(t, KqFmt.k5)}, k6 {tally_fmt(t, KqFmt.k6)}, q40 {tally_fmt(t, KqFmt.q40)}, q51 {tally_fmt(t, KqFmt.q51)}\n")
    to_log(LOG_INFO, "dasLLAMA noisy: plane MB - q8 {float(long_length(t.qblob)) / mb} (+scales {float(long_length(t.qscales) * 4l) / mb}), k4 {float(long_length(t.k4q) + long_length(t.k4s)) / mb}, k5 {float(long_length(t.k5q) + long_length(t.k5s)) / mb}, k6 {float(long_length(t.k6q) + long_length(t.k6s)) / mb}, q40 {float(long_length(t.q40q) + long_length(t.q40s)) / mb}, q51 {float(long_length(t.q51q) + long_length(t.q51s)) / mb}, f32 {float(long_length(t.wblob) * 4l) / mb}\n")
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
    if (gt == GGML_TYPE_Q4_0 && get_kq_q40_native()) {
        return KqFmt.q40
    }
    if (gt == GGML_TYPE_Q5_0 && get_kq_q50_native()) {
        return KqFmt.k5
    }
    if (gt == GGML_TYPE_Q5_1 && get_kq_q51_native()) {
        return KqFmt.q51
    }
    return KqFmt.q8
}


// Fill the per-kind weight format tags from the file's tensor types (q8-mode loads with the
// kquant_native knob on). Fused-projection arches (Phi3) split attn_qkv/ffn_up at row-aligned
// slice offsets, so every slice stays superblock-aligned; one disk tensor = one tag for its slices.
def private detect_kq_formats(var t : Model; m : GGUFMeta) {   // nolint:STYLE038 — a flat tag-assignment run: one weight kind per line
    if (t.quant != QuantMode.q8 || !get_kquant_native()
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
                let f = kq_fmt_expert_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.ffn_gate_up_exps.weight")), t.config.dim)
                t.we1_fmt[l] = f
                t.we3_fmt[l] = f
            } else {
                t.we1_fmt[l] = kq_fmt_expert_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.ffn_gate_exps.weight")), t.config.dim)
                t.we3_fmt[l] = kq_fmt_expert_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.ffn_up_exps.weight")), t.config.dim)
            }
            t.we2_fmt[l] = kq_fmt_expert_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.ffn_down_exps.weight")), t.config.n_ff_exp)
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
    // deltanet dn tags: CPU rail only — Metal/GPU tiers bind dn q8; grouped files have no exact transcode
    if (t.config.recr_mask != 0ul && !t.dn_grouped
            && get_metal_mode() == MetalMode.off
            && !(moe_gpu_tier_installed() && (gpu_want_dense() || gpu_want_dn() || gpu_want_dnd()))) {
        t.dnqkv_fmt |> resize(layers)
        t.dngate_fmt |> resize(layers)
        t.dnout_fmt |> resize(layers)
        for (l in range64(layers)) {
            if (layer_is_recurrent(t.config, l)) {
                t.dnqkv_fmt[l] = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.attn_qkv.weight")), t.config.dim)
                t.dngate_fmt[l] = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.attn_gate.weight")), t.config.dim)
                t.dnout_fmt[l] = kq_fmt_row_ok(kq_fmt_of(gguf_tensor_type(m, "blk.{l}.ssm_out.weight")), t.config.ssm_d_inner)
                any ||= t.dnqkv_fmt[l] != KqFmt.q8 || t.dngate_fmt[l] != KqFmt.q8 || t.dnout_fmt[l] != KqFmt.q8
            }
        }
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
        delete t.dnqkv_fmt
        delete t.dngate_fmt
        delete t.dnout_fmt
    }
    t.kquant_native = any
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

//! Streaming-conversion entry: plan-load `t`, then invoke `blk(m, bytes)` with the source
//! mapping(s) STILL ALIVE — split shards included, every shard stays mapped; the caller streams
//! the save inside. Fails open: unstreamable models materialize + save eagerly through the same blk.
def load_gguf_streaming(path : string; mode : QuantMode; var t : Model; var jobs : array<FillJob>;   // nolint:STYLE038 — one linear plan-then-stream sequence under a live mapping
                        want_blob : bool = false;
                        blk : block<(m : GGUFMeta; bytes : array<uint8>) : bool>) : bool {
    if (!is_job_que_available()) {
        var okj = false
        with_job_que() {
            okj = load_gguf_streaming(path, mode, t, jobs, want_blob, blk)
        }
        return okj
    }
    g_stream_want_blob = want_blob
    bump_weights_epoch()
    t.quant = mode
    if (g_env_engine.conv_prof) {
        set_conv_profile(true)
    }
    let ts_load = ref_time_ticks()
    let lmode0 = mode
    var shards <- gguf_shard_paths(path)
    if (length(shards) > 1) {
        // split gguf: every shard mapping stays alive across the plan-load AND the blk-save
        // (the transcode kernels resolve per-shard bases through the meta)
        to_log(LOG_INFO, "dasLLAMA: split gguf - {length(shards)} shards (streaming)\n")
        var sok = false
        var bases : array<void?>
        var sizes : array<int64>
        gguf_map_shards(shards, 0, bases, sizes) {
            if (g_env_engine.prefetch) {
                var pf_mb = 0l
                for (bi in range(length(bases))) {
                    if (unsafe(prefetch_map(bases[bi], uint64(sizes[bi])))) {
                        pf_mb += sizes[bi] >> 20l
                    }
                }
                if (pf_mb > 0l) {
                    to_log(LOG_INFO, "dasLLAMA: source prefetch armed ({pf_mb} MB across {length(bases)} shards)\n")
                }
            }
            var m <- parse_gguf_meta_shards(bases, sizes)
            fill_plan_begin()
            unsafe {
                load_gguf_parsed(t, m, temp_array(reinterpret<uint8?>(bases[0]), sizes[0], type<uint8>), lmode0, ts_load)
            }
            fill_plan_take(jobs)
            t.tok <- load_tokenizer_auto(path)
            unsafe {
                sok = invoke(blk, m, temp_array(reinterpret<uint8?>(bases[0]), sizes[0], type<uint8>))
            }
            delete m
        }
        bases |> clear()   // borrowed mapping bases — clear so delete frees only the buffer
        delete bases
        delete sizes
        delete shards
        g_stream_want_blob = false
        return sok
    }
    delete shards
    let f = fopen(path, "rb")
    if (f == null) {
        return false
    }
    var ok = false
    let lmode = mode
    fmap(f) $(var bytes : array<uint8>#) {
        if (g_env_engine.prefetch) {
            var armed = false
            unsafe {
                armed = prefetch_map(addr<void?>(bytes[0]), uint64(long_length(bytes)))
            }
            if (armed) {
                to_log(LOG_INFO, "dasLLAMA: source prefetch armed ({long_length(bytes) >> 20l} MB)\n")
            }
        }
        var inscope m <- parse_gguf_meta(bytes)
        fill_plan_begin()
        load_gguf_parsed(t, m, bytes, lmode, ts_load)
        fill_plan_take(jobs)
        t.tok <- load_tokenizer_auto(path)
        unsafe {
            ok = invoke(blk, m, temp_array(addr<uint8?>(bytes[0]), long_length(bytes), type<uint8>))
        }
    }
    fclose(f)
    g_stream_want_blob = false
    return ok
}

def private load_gguf_impl(path : string; var mode : QuantMode) : Model {
    bump_weights_epoch()
    var t = Model()
    t.quant = mode
    if (g_env_engine.conv_prof) {
        set_conv_profile(true)
    }
    let ts_load = ref_time_ticks()
    let shards <- gguf_shard_paths(path)
    if (length(shards) > 1) {
        to_log(LOG_INFO, "dasLLAMA: split gguf - {length(shards)} shards\n")
        var bases : array<void?>
        var sizes : array<int64>
        gguf_map_shards(shards, 0, bases, sizes) {
            // fault the source in AHEAD of the parallel transcode (in-loop faults serialize lanes)
            if (g_env_engine.prefetch) {
                var pf_mb = 0l
                for (bi in range(length(bases))) {
                    if (unsafe(prefetch_map(bases[bi], uint64(sizes[bi])))) {
                        pf_mb += sizes[bi] >> 20l
                    }
                }
                if (pf_mb > 0l) {
                    to_log(LOG_INFO, "dasLLAMA: source prefetch armed ({pf_mb} MB across {length(shards)} shards)\n")
                }
            }
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
        if (g_env_engine.prefetch) {
            var armed = false
            unsafe {
                armed = prefetch_map(addr<void?>(bytes[0]), uint64(long_length(bytes)))
            }
            if (armed) {
                to_log(LOG_INFO, "dasLLAMA: source prefetch armed ({long_length(bytes) >> 20l} MB)\n")
            }
        }
        var inscope m <- parse_gguf_meta(bytes)
        load_gguf_parsed(t, m, bytes, mode, ts_load)
    }
    fclose(f)
    return <- t
}

// The load once metadata is parsed and the byte view carrying the KVs (shard 0 for splits) is
// mapped — config, vocab, plane sizing, tensor reads/transcodes, repack, wscale.
def private load_gguf_parsed(var t : Model; m : GGUFMeta; bytes : array<uint8> | #; var mode : QuantMode; ts_load : int64) {   // nolint:STYLE037,STYLE038 — the load pipeline: ordered phases over one shared cursor; splitting it needs its own pass
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
            if (long_length(t.ffn_w) != layers) {
                panic("dasLLAMA: feed_forward_length array has {long_length(t.ffn_w)} entries, expected {layers}")
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
                if (long_length(rl) != layers) {
                    panic("dasLLAMA: recurrent_layers array has {long_length(rl)} entries, expected {layers}")
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
                panic("dasLLAMA: MTP-only sidecar GGUF (no trunk tensors) is not supported - use the full -MTP file")
            }
            // glm4moe's MTP shares the trunk's embedding and classifier with the draft head, and the
            // GGUF materializes copies of both — so draft through the TRUNK's tables and skip them.
            // A shape disagreement means it is NOT that sharing, so fail closed.
            let nemb = gguf_find_tensor(m, "blk.{layers}.nextn.embed_tokens.weight")
            let nhead = gguf_find_tensor(m, "blk.{layers}.nextn.shared_head_head.weight")
            if ((nemb >= 0 && m.tensors[nemb].n_elem != vocab * dim)
                    || (nhead >= 0 && m.tensors[nhead].n_elem != vocab * dim)) {
                to_log(LOG_WARNING, "dasLLAMA: NextN embed_tokens/shared_head_head are not the trunk's shape - MTP draft head disabled\n")
                t.config.n_layer_nextn = 0l
            }
        } elif (gguf_find_tensor(m, "blk.{layers}.nextn.eh_proj.weight") >= 0
                || gguf_find_tensor(m, "blk.{layers - 1l}.nextn.eh_proj.weight") >= 0) {
            // the second probe catches a malformed file whose block_count already counts the
            // draft block — its nextn tensors sit one below the first probe's index
            panic("dasLLAMA: file ships NextN tensors but no nextn_predict_layers key - refusing to guess")
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
        if (t.config.moe_optional && !t.config.moe) {
            // the moe-optional arch registers ffn_is_dense = false for the shared descriptor, but
            // THIS file resolved dense (12B/31B/E-series) — its ffn IS the std dense stack
            // (+ out_scale; the E-series PLE epilogue is MetalNeed.ple, not a MoE graph)
            t.blocks.ffn_is_dense = true
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
            panic("dasLLAMA: file ships attn_q_norm but arch '{arch}' does not configure qk_norm - refusing to mis-run")
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
        // ssm_beta/ssm_alpha disk type routes their matmul (must precede layout_offsets): F32
        // files keep them fp32 in fblob (llama.cpp matmuls f32); quantized files ride qblob.
        // Also before detect_kq_formats — the dn tags demote on grouped (qwen3next) files.
        if (t.config.recr_mask != 0ul) {
            var frl = 0l
            while (!layer_is_recurrent(t.config, frl)) {
                frl++
            }
            t.dn_grouped = gguf_find_tensor(m, "blk.{frl}.ssm_ba.weight") >= 0
            let ba0 = t.dn_grouped ? "blk.{frl}.ssm_ba.weight" : "blk.{frl}.ssm_beta.weight"
            t.dn_ba_f32 = gguf_tensor_type(m, ba0) == GGML_TYPE_F32
        }
        // per-tensor K-quant format tags (kquant_native rail; no-op when the knob is off or the
        // file has no K-quant tensors) — must precede layout_offsets, which walks the tags
        detect_kq_formats(t, m)
        // fp32/q4 serving: the PLE token table is STILL pinned to its disk format
        // (detect_kq_formats runs only on the q8 rail); same superblock-alignment gate
        if (t.quant != QuantMode.q8 && has_ple(t.config)
                && (t.config.n_layers * t.config.n_embd_per_layer) % 256l == 0l) {
            t.ple_emb_fmt = kq_fmt_of(gguf_tensor_type(m, "per_layer_token_embd.weight"))
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
        // streaming covers the q8-mode load_big rails; models with direct-write loaders
        // (mx4 experts, grouped deltanet, bf16 PLE) disarm HERE — before sizing — so the
        // eager path proceeds with fully materialized planes and an empty job list
        if (g_fill_plan_mode && (mode != QuantMode.q8 || t.experts_mx4 || t.dn_grouped || t.ple_model_bf16)) {
            to_log(LOG_INFO, "dasLLAMA: streaming plan disarmed - unstreamed rail (mode {mode}, mx4 {t.experts_mx4}, dn-grouped {t.dn_grouped}, bf16-ple {t.ple_model_bf16}); converting eagerly\n")
            g_fill_plan_mode = false
        }
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
        // streaming: big q-halves never materialize (jobs stream them to the writer); small
        // scale halves stay in RAM. Decided HERE — offsets exist, nothing is sized yet — and a
        // refusal disarms streaming outright, because a planar image is the wrong metal artifact
        if (g_fill_plan_mode && g_stream_want_blob) {
            // the eager rail's two gates, both pure config/fmt reads: metal serves this shape,
            // and the blob layout holds (bind alignment, no repack)
            t.metal_blob = metal_model_servable(t) && blob_stream_eligible(t)
            if (!t.metal_blob) {
                to_log(LOG_INFO, "dasLLAMA: streaming plan disarmed - the metal blob flavor is unavailable for this model; converting eagerly\n")
                g_fill_plan_mode = false
            }
        }
        let stream_q = g_fill_plan_mode
        if (stream_q) {
            // the streamed save sizes each q-plane from the LAYOUT (holes included), not the job sum
            g_stream_plane_total |> clear()
            g_stream_plane_total["k4q"] = (sz.k4_n / 256l) * K4_QSB
            g_stream_plane_total["k5q"] = (sz.k5_n / 256l) * K5_QSB
            g_stream_plane_total["k6q"] = (sz.k6_n / 256l) * K6_QSB
            g_stream_plane_total["q40q"] = (sz.q40_n / 256l) * Q40_QSB
            g_stream_plane_total["q51q"] = (sz.q51_n / 32l) * Q51_QB
            let wb = mode == QuantMode.q8 ? sz.wblob_n : 0l
            g_stream_plane_total[t.metal_blob ? "mblob" : "qblob"] = (t.metal_blob
                ? (wb / Q8_BLOCK_ELEMS) * (Q8_QPB + Q8_SPB) : wb)
        }
        if (sz.k4_n > 0l) {
            if (!stream_q) {
                t.k4q |> reserve((sz.k4_n / 256l) * K4_QSB)
                t.k4q |> resize((sz.k4_n / 256l) * K4_QSB)
            }
            t.k4s |> reserve((sz.k4_n / 256l) * K4_SSB)
            t.k4s |> resize((sz.k4_n / 256l) * K4_SSB)
        }
        if (sz.k5_n > 0l) {
            if (!stream_q) {
                t.k5q |> reserve((sz.k5_n / 256l) * K5_QSB)
                t.k5q |> resize((sz.k5_n / 256l) * K5_QSB)
            }
            t.k5s |> reserve((sz.k5_n / 256l) * K5_SSB)
            t.k5s |> resize((sz.k5_n / 256l) * K5_SSB)
        }
        if (sz.k6_n > 0l) {
            if (!stream_q) {
                t.k6q |> reserve((sz.k6_n / 256l) * K6_QSB)
                t.k6q |> resize((sz.k6_n / 256l) * K6_QSB)
            }
            t.k6s |> reserve((sz.k6_n / 256l) * K6_SSB)
            t.k6s |> resize((sz.k6_n / 256l) * K6_SSB)
        }
        if (sz.q40_n > 0l) {
            if (!stream_q) {
                t.q40q |> reserve((sz.q40_n / 256l) * Q40_QSB)
                t.q40q |> resize((sz.q40_n / 256l) * Q40_QSB)
            }
            t.q40s |> reserve((sz.q40_n / 256l) * Q40_SSB)
            t.q40s |> resize((sz.q40_n / 256l) * Q40_SSB)
        }
        if (sz.q51_n > 0l) {
            if (!stream_q) {
                t.q51q |> reserve((sz.q51_n / 32l) * Q51_QB)
                t.q51q |> resize((sz.q51_n / 32l) * Q51_QB)
            }
            t.q51s |> reserve((sz.q51_n / 32l) * Q51_SB)
            t.q51s |> resize((sz.q51_n / 32l) * Q51_SB)
        }
        if (mode == QuantMode.q8) {
            if (!stream_q) {
                t.qblob |> reserve(sz.wblob_n)
                t.qblob |> resize(sz.wblob_n)
            }
            if (!t.metal_blob) {   // blob: scales ride inside mblob, so the split plane never exists
                t.qscales |> reserve(sz.wblob_n / 32l)
                t.qscales |> resize(sz.wblob_n / 32l)
            }
        } elif (mode == QuantMode.q4_0) {
            t.q4blob |> reserve(sz.wblob_n / 2l)
            t.q4blob |> resize(sz.wblob_n / 2l)
            t.q4scales |> reserve(sz.wblob_n / 32l)
            t.q4scales |> resize(sz.wblob_n / 32l)
        } else {
            reserve_resize(t.wblob, sz.wblob_n)
        }
        if (sz.pleq8_n > 0l) {   // fp32/q4: the dedicated q8 plane holding ONLY the PLE table
            t.qblob |> reserve(sz.pleq8_n)
            t.qblob |> resize(sz.pleq8_n)
            t.qscales |> reserve(sz.pleq8_n / 32l)
            t.qscales |> resize(sz.pleq8_n / 32l)
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
                    load_big(m, bytes, "blk.{l}.attn_qkv.weight", t, t.dnqkv_offs[l], dim * cd, scratch, 0l, fmt_at(t.dnqkv_fmt, l))
                    load_big(m, bytes, "blk.{l}.attn_gate.weight", t, t.dngate_offs[l], dim * t.config.ssm_d_inner, scratch, 0l, fmt_at(t.dngate_fmt, l))
                    if (!t.dn_ba_f32) {
                        load_big(m, bytes, "blk.{l}.ssm_beta.weight", t, t.dnbeta_offs[l], dim * t.config.ssm_dt_rank, scratch)
                        load_big(m, bytes, "blk.{l}.ssm_alpha.weight", t, t.dnalpha_offs[l], dim * t.config.ssm_dt_rank, scratch)
                    }
                    load_big(m, bytes, "blk.{l}.ssm_out.weight", t, t.dnout_offs[l], t.config.ssm_d_inner * dim, scratch, 0l, fmt_at(t.dnout_fmt, l))
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
            // nextn.embed_tokens / nextn.shared_head_head are the trunk's own tables under the
            // head's prefix (shape-vetted at config parse) — deliberately never loaded
        }
        if (has_ple(t.config)) {  // the per-layer token-embedding table [vocab x (ple*layers)], gathered per token, never repacked
            let pn = vocab * t.config.n_embd_per_layer * layers
            if (t.ple_emb_fmt == KqFmt.q8 && t.quant != QuantMode.q8) {
                // fp32/q4: the table stays on its q8 plane. Exact only from Q8_0/Q5_0 disk bits,
                // and fp32 is the exactness instrument, so a lossy requant there fails closed.
                let gt = gguf_tensor_type(m, "per_layer_token_embd.weight")
                if (t.quant != QuantMode.q4_0 && gt != GGML_TYPE_Q8_0 && gt != GGML_TYPE_Q5_0) {
                    panic("dasLLAMA: fp32 serving keeps the PLE table on its disk plane, and this file's table type ({gt}) has no exact q8 form - serve q8/q4, or use a Q8_0/K-quant-table file")
                }
                let t0 = g_conv_prof ? ref_time_ticks() : 0l
                let kind = transcode_q8_tensor(m, bytes, "per_layer_token_embd.weight", t.qblob, t.qscales, t.ple_emb_off, t.ple_emb_off / 32l, pn, 0l, scratch)
                conv_note(kind, t0, pn)
            } else {
                load_big(m, bytes, "per_layer_token_embd.weight", t, t.ple_emb_off, pn, scratch, 0l, t.ple_emb_fmt)
            }
        }
        if (!t.config.shared_weights) {
            load_big(m, bytes, "output.weight", t, t.wcls_off, vocab * dim, scratch, 0l, t.wcls_fmt)
        } elif (t.cls_q8) {
            // tied Q8: classifier copy (repacked below) + the linear copy embedding rows read.
            // Through load_big (its q8 arm is the same transcode) so the streaming plan records both.
            load_big(m, bytes, "token_embd.weight", t, t.wcls_off, vocab * dim, scratch)
            load_big(m, bytes, "token_embd.weight", t, t.emb_q8_off, vocab * dim, scratch)
        } elif (cls_kq(t)) {
            // tied K-quant: ONE plane copy serves the classifier matmul and embedding row reads
            // (the kq planes stay in disk order — no repack, so no second linear copy)
            load_big(m, bytes, "token_embd.weight", t, t.wcls_off, vocab * dim, scratch, 0l, t.emb_fmt)
        }
        // Qwen2 Q/K/V projection biases — small fp32 tensors, kept dense
        if (t.config.attn_qkv_bias) {
            t.bq |> resize(layers_ld * qd)
            t.bk |> resize(layers_ld * kv)
            t.bv |> resize(layers_ld * kv)
            for (l in range64(layers_ld)) {
                gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_q.bias", t.bq, l * qd, qd)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_k.bias", t.bk, l * kv, kv)
                gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_v.bias", t.bv, l * kv, kv)
            }
        }
        // gpt-oss attention output-projection bias (dim per layer)
        if (t.config.attn_out_bias) {
            t.bo |> resize(layers_ld * dim)
            for (l in range64(layers_ld)) {
                gguf_read_tensor_f32(m, bytes, "blk.{l}.attn_output.bias", t.bo, l * dim, dim)
            }
        }
        delete scratch
        to_log(LOG_INFO, "dasLLAMA: load stage weights: {get_time_usec(ts_stage) / 1000} ms\n")
        conv_profile_report()   // the per-conversion-kind split of the stage just reported
        ts_stage = ref_time_ticks()
        // arm64: switch to the laneq backend and repack the just-loaded Q8 weights into its interleaved
        // layout. Returns false off-ARM (portable stays, weights stay row-major).
        if (mode == QuantMode.q8 && select_matmul_backend_for_load_()) {
            if (g_fill_plan_mode) {
                // streaming: the q-halves are not materialized — the fill applies the SAME
                // regions per job. Set the marks the eager walkers would (the meta serializes
                // BEFORE the streamed planes fill), run nothing.
                t.mx4_repacked = true
                if (t.kquant_native && kernel_backend_has_kq()) {
                    t.kq_repacked = true
                    t.kq_repack_mr4 = active_kq_layout_mr(4)
                    t.kq_repack_mr5 = active_kq_layout_mr(5)
                    t.kq_repack_mr6 = active_kq_layout_mr(6)
                    t.kq_repack_mr40 = active_kq_layout_mr(40)
                }
            } else {
                layout_repack_q8(t)
                if (t.experts_mx4) {
                    layout_repack_mx4(t)
                }
                if (active_q51_layout_mr() > 1l) {
                    layout_repack_q51(t)
                }
                if (t.kquant_native && kernel_backend_has_kq()) {
                    layout_repack_kq(t)
                    t.kq_repacked = true
                    t.kq_repack_mr4 = active_kq_layout_mr(4)
                    t.kq_repack_mr5 = active_kq_layout_mr(5)
                    t.kq_repack_mr6 = active_kq_layout_mr(6)
                    t.kq_repack_mr40 = active_kq_layout_mr(40)
                    to_log(LOG_INFO, "dasLLAMA: K-quant planes repacked to the '{active_kernel_backend()}' layouts (k4 grp{t.kq_repack_mr4}, k5 grp{t.kq_repack_mr5}, k6 grp{t.kq_repack_mr6}, q40 grp{t.kq_repack_mr40})\n")
                }
            }
        }
        to_log(LOG_INFO, "dasLLAMA: load stage repack: {get_time_usec(ts_stage) / 1000} ms\n")
        ts_stage = ref_time_ticks()
        if (mode == QuantMode.q8 && g_wscale_f16) {
            if (g_fill_plan_mode) {
                // the epilogue converts after qblob fills, so pre-mark for the meta, mirroring the
                // convert's own outcome (empty scales = nothing converts, no flag). The drivers read
                // this flag, so the streamed and eager rails must not disagree
                let have_q8 = t.metal_blob ? g_stream_plane_total?["mblob"] ?? 0l : long_length(t.qscales)
                t.wscale_f16 = have_q8 > 0l && kernel_backend_has_wscale16()
            } else {
                wscale_convert_f16(t)
            }
        }
        to_log(LOG_INFO, "dasLLAMA: load stage wscale: {get_time_usec(ts_stage) / 1000} ms, total {get_time_usec(ts_load) / 1000} ms\n")
        log_load_report(t)
        if (!g_fill_plan_mode) {   // streaming converts only — no serving, no gathers over unmade planes
            ple_check_table(t, "gguf load")
            gpu_upload_resident(t)
        }
    }
}

//! Quantize a fp32-loaded model's big weights to Q8_0 in place and switch forward onto the Q8 path.
//! Requires the fp32 representation (wblob) — i.e. a model loaded with QuantMode.fp32. Keeps wblob
//! so the same model can be re-quantized to another precision for an A/B comparison.
def quantize_weights(var t : Model) {
    if (has_ple(t.config)) {   // the PLE table rides its disk plane, which this requant would clobber
        panic("dasLLAMA: quantize_weights cannot requant an E-series (PLE) model in place - load with the target quant mode instead")
    }
    let n = long_length(t.wblob)
    var qt <- quantize_q8_0(t.wblob, n)
    t.qblob <- qt.q
    t.qscales <- qt.scales
    t.quant = QuantMode.q8
    // same backend-for-load select + in-place repack as the load_gguf Q8 path (no-op off-ARM)
    if (select_matmul_backend_for_load_()) {
        layout_repack_q8(t)
    }
    if (g_wscale_f16) {
        wscale_convert_f16(t)
    }
    gpu_upload_resident(t)
}

//! Quantize a fp32-loaded model's big weights to Q4_0 in place and switch forward onto the Q4 path.
//! Same fp32-required / wblob-kept contract as quantize_weights.
def quantize_weights_q4(var t : Model) {
    if (has_ple(t.config)) {   // same clobber as quantize_weights — the table plane is not wblob's to replace
        panic("dasLLAMA: quantize_weights_q4 cannot requant an E-series (PLE) model in place - load with the target quant mode instead")
    }
    let n = long_length(t.wblob)
    var qt <- quantize_q4_0(t.wblob, n)
    t.q4blob <- qt.q
    t.q4scales <- qt.scales
    t.quant = QuantMode.q4_0
}



//! Load a model AND its tokenizer from a GGUF — the full user-facing entry point. Arch and
//! tokenizer backend are both auto-selected from GGUF metadata; `mode` picks the weight quant.
//! Applies the tune sidecar's "runtime" knobs first (see apply_box_profile_runtime).
def load_model_(path : string; mode : QuantMode = QuantMode.fp32) : Model {
    apply_box_profile_runtime()   // before load: a backend pin must precede the backend-for-load select
    // same guard rail as load_model_cached: a GPU-armed env whose backend never registered
    // (tier module not required, shared_module load swallowed) says so instead of resolving CPU
    if (get_metal_mode() == MetalMode.off) {
        warn_gpu_backend_absent()
    }
    var m <- load_gguf(path, mode)
    m.tok <- load_tokenizer_auto(path)
    return <- m
}

// Function values do not survive the -exe bake's global serialization (the
// dasllama_config restore names the defect) — every @@-defaulted seam in this
// module boot-restores here; a real registrar wins via the null guards.
[init]
def private restore_load_fn_defaults {
    if (g_stream_collect_q8 == null) {
        g_stream_collect_q8 = @@stream_no_collect
    }
    if (g_stream_collect_kq == null) {
        g_stream_collect_kq = @@stream_no_collect
    }
    if (g_stream_collect_q51 == null) {
        g_stream_collect_q51 = @@stream_no_collect
    }
    if (g_stream_repack == null) {
        g_stream_repack = @@stream_no_repack
    }
}
