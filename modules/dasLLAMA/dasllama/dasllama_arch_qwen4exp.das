options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_arch_qwen4exp shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common
require dasllama/dasllama_blocks       // ffn_moe_decode / ffn_moe_prefill: the routed FFN inside the mixers
require dasllama/dasllama_arch_qwen35  // the hybrid deltanet / gated-attention dispatch + configure_qwen35moe
require dasllama/dasllama_kqformat     // GGML_TYPE_* + iq4nl_lut: the hash table's rows decode from their disk blocks
require dasllama/dasllama_math
require dasllama/dasllama_gpu_math   // sigmoid_f32: the one form the CPU chain and the kernel homes share
require daslib/f16_cvt      // f16_to_f32: the hash table's block scales
require daslib/array_boost  // nolint:STYLE030 — temp_array: the decode token as a one-row token array (a generic instance the lint does not see)
require math

// qwen4exp (Qwen3.8-Flash-Next, "a preview of the Qwen4 architecture"): the qwen35moe hybrid MoE
// (3 DeltaNet layers per gated-attention layer, 512 routed experts + a gated shared expert) inside
// hyper-connections - hc parallel residual streams a low-rank mixer collapses before every block and
// scatters into after it - with an n-gram hash side input before layer 1 and a σ(z) deltanet out-gate.
def private configure_qwen4exp(var c : Config) {
    configure_qwen35moe(c)
    c.hyper_conn = true
    c.dn_z_sigmoid = true
    c.ple_ngram = true
}

[init]
def register_arch_qwen4exp() {
    var d = ArchDesc(name = "qwen4exp", configure = @@configure_qwen4exp)
    d.blocks = qwen4exp_blocks()
    chatml_chat(d.chat)
    hermes_tools(d.chat)
    register_arch("qwen4exp", d)
}

// ===== the n-gram PLE side input =====

//! the mixer site index of layer l: attention first, ffn second
def private hc_site(l : int64; ffn : bool) : int64 => l * 2l + (ffn ? 1l : 0l)

//! Decode one hash-table row (ple_head_dim elements, the disk block form) into dst
def private ngram_row(t : Model; row : int64; var dst : float?) {
    let n = t.config.ple_head_dim
    let bo = row * t.ngram_row_bytes
    let ty = t.ngram_type
    unsafe {
        let tab = addr < uint8 const? >(t.ngram_tab[bo])
        if (ty == GGML_TYPE_IQ4_NL) {   // 18-byte blocks of 32: f16 d + 16 nibble bytes (low = elems 0..15, high = 16..31), value = d * lut[nibble]
            let lut = iq4nl_lut()
            for (blk in range64(n / 32l)) {
                let bp = tab + blk * 18l
                let d = f16_to_f32(uint(bp[0]) | (uint(bp[1]) << 8u))
                for (k in range64(16l)) {
                    let b = int(bp[2l + k])
                    dst[blk * 32l + k] = float(lut[b & 15]) * d
                    dst[blk * 32l + k + 16l] = float(lut[b >> 4]) * d
                }
            }
        } elif (ty == GGML_TYPE_Q8_0) {   // 34-byte blocks of 32: f16 d + 32 int8
            for (blk in range64(n / 32l)) {
                let bp = tab + blk * 34l
                let d = f16_to_f32(uint(bp[0]) | (uint(bp[1]) << 8u))
                for (k in range64(32l)) {
                    let u = int(bp[2l + k])
                    dst[blk * 32l + k] = float(u < 128 ? u : u - 256) * d
                }
            }
        } elif (ty == GGML_TYPE_F16) {
            for (i in range64(n)) {
                dst[i] = f16_to_f32(uint(tab[2l * i]) | (uint(tab[2l * i + 1l]) << 8u))
            }
        } elif (ty == GGML_TYPE_BF16) {
            for (i in range64(n)) {
                dst[i] = reinterpret<float>((uint(tab[2l * i]) | (uint(tab[2l * i + 1l]) << 8u)) << 16u)
            }
        } else {   // F32 (the loader admits no other type)
            let fp = reinterpret<float const?>(tab)
            for (i in range64(n)) {
                dst[i] = fp[i]
            }
        }
    }
}

//! The predecessor s positions back from batch row p: an earlier row of the batch, else the
//! session's window (oldest first), else -1 (none)
def private ngram_pred(s : Session; tokens : array<int64>; p, back : int64) : int64 {
    let idx = p - back
    if (idx >= 0l) {
        return tokens[idx]
    }
    let nprev = long_length(s.ple_prev)
    let wi = nprev + idx   // idx < 0: -1 = the newest window slot
    return wi >= 0l ? s.ple_prev[wi] : -1l
}

//! Gather one token's heads into dst: ctx[0] = the token, ctx[s] = the predecessor s back (an EOS or a missing one
//! reads as EOS and cuts everything older; the token's own EOS cuts nothing). Bigram heads hash ctx[0..1], trigram
//! heads ctx[0..2] - mixed = XOR of token x multiplier - and head h lands on row mixed % vocab[h] + offset[h]
def ngram_gather(t : Model; s : Session; tokens : array<int64>; p : int64; var dst : float?) {
    let c = t.config
    let n = c.ple_ngram_size
    let eos = c.ple_eos_token
    var ctx : int64[8]
    ctx[0] = tokens[p]
    var cut = false
    for (k in range64(1l, n)) {
        let raw = cut ? -1l : ngram_pred(s, tokens, p, k)
        cut = cut || raw < 0l || raw == eos
        ctx[k] = cut ? eos : raw
    }
    let per = c.ple_heads_per_ngram
    let hd = c.ple_head_dim
    for (ng in range64(2l, n + 1l)) {
        var mixed = uint64(ctx[0]) * t.ple_mult[0]
        for (j in range64(1l, ng)) {
            mixed ^= uint64(ctx[j]) * t.ple_mult[j]
        }
        let base = (ng - 2l) * per
        for (g in range64(per)) {
            let h = base + g
            let row = int64(mixed % uint64(t.ple_head_vocab[h])) + t.ple_head_off[h]
            ngram_row(t, row, unsafe(dst + h * hd))
        }
    }
}

//! Slide the window over the batch's last rows (a batch shorter than the window keeps its older slots)
def private ngram_advance(var s : Session; tokens : array<int64>; npos : int64) {
    let nprev = long_length(s.ple_prev)
    let keep = max(0l, nprev - npos)
    for (i in range64(keep)) {
        s.ple_prev[i] = s.ple_prev[i + npos]
    }
    for (i in range64(keep, nprev)) {
        s.ple_prev[i] = tokens[npos - (nprev - i)]
    }
}

def private ple_reset(var s : Session) {
    for (i in range64(long_length(s.ple_prev))) {
        s.ple_prev[i] = -1l
    }
    for (i in range64(long_length(s.ple_conv_hist))) {
        s.ple_conv_hist[i] = 0.0
    }
}

//! The side input of one row into the wide residual res[roff..], key/value already projected from the gathered heads
//! (key: hc x dim, val: dim): a per-stream gate (signed sqrt of the normed key·query dot) scales the value into every
//! stream; a dilated depthwise causal conv over the normed gated rows (ring history, zero before the sequence) adds its silu
def private ple_apply_row(t : Model; var s : Session; var res : array<float>; roff : int64; var key : float?; val : float const?; pos : int64) {
    let c = t.config
    let dim = c.dim
    let nh = c.hc_count
    let hcd = hc_dim(c)
    let kern = c.ple_conv_kernel
    let dil = c.ple_ngram_size
    let nrows = ple_hist_rows(c)
    let inv_sq = 1.0 / sqrt(float(dim))
    unsafe {
        // the normed key in place, the normed query (the residual) in the mixer scratch
        var kn = key
        var qn = addr(s.hc_xn[0])
        for (ch in range64(nh)) {
            rmsnorm(kn + ch * dim, reinterpret<float const?>(kn + ch * dim), addr < float const? >(t.fblob[t.ple_nkey_off + ch * dim]), dim, c.norm_eps)
            rmsnorm(qn + ch * dim, addr < float const? >(res[roff + ch * dim]), addr < float const? >(t.fblob[t.ple_nquery_off + ch * dim]), dim, c.norm_eps)
        }
        var gated = addr(s.ple_gated[0])
        for (ch in range64(nh)) {
            let sv = dot(reinterpret<float const?>(kn + ch * dim), reinterpret<float const?>(qn + ch * dim), dim) * inv_sq
            let mag = sqrt(max(abs(sv), 1.0e-6))
            let sg = sv > 0.0 ? mag : (sv < 0.0 ? -mag : 0.0)
            let g = sigmoid_f32(sg)
            for (i in range64(dim)) {
                gated[ch * dim + i] = val[i] * g
            }
        }
        // the conv input row is the grouped-normed gated row; it lands in the ring at this position's slot
        var slot = addr(s.ple_conv_hist[(pos % nrows) * hcd])
        for (ch in range64(nh)) {
            rmsnorm(slot + ch * dim, reinterpret<float const?>(gated + ch * dim), addr < float const? >(t.fblob[t.ple_nconv_off + ch * dim]), dim, c.norm_eps)
        }
        var co = addr(s.ple_conv_out[0])
        let w = addr < float const? >(t.fblob[t.ple_conv_off])
        for (chn in range64(hcd)) {
            co[chn] = 0.0
        }
        for (k in range64(kern)) {   // tap k reads (kern - 1 - k) x dilation positions back; before the sequence = zero
            let src_pos = pos - (kern - 1l - k) * dil
            if (src_pos < 0l) {
                continue
            }
            let hrow = addr < float const? >(s.ple_conv_hist[(src_pos % nrows) * hcd])
            for (chn in range64(hcd)) {
                co[chn] += w[chn * kern + k] * hrow[chn]
            }
        }
        silu(co, hcd)
        add_inplace(addr(res[roff]), reinterpret<float const?>(gated), hcd)
        add_inplace(addr(res[roff]), reinterpret<float const?>(co), hcd)
    }
}

// ===== the block set =====

//! the wide residual starts as hc copies of the embedding row
def private hc_init_row(t : Model; x : array<float>; xoff : int64; var res : array<float>; roff : int64) {
    let c = t.config
    unsafe {
        for (ch in range64(c.hc_count)) {
            copy_floats(addr(res[roff + ch * c.dim]), addr < float const? >(x[xoff]), c.dim)
        }
    }
}

def private pre_stack_decode_qwen4exp(t : Model; var s : Session; token, pos : int64) {
    if (pos == 0l) {
        ple_reset(s)
    }
    hc_init_row(t, s.x, 0l, s.hc_res, 0l)
    var tok : int64[1]
    tok[0] = token
    let ts_gather = prof_ticks()
    unsafe {
        let toks = temp_array(addr(tok[0]), 1l, type<int64>)
        ngram_gather(t, s, toks, 0l, addr(s.ple_emb[0]))
        ngram_advance(s, toks, 1l)
    }
    prof_add("ple_gather", ts_gather)
}

def private pre_stack_prefill_qwen4exp(t : Model; var s : Session; tokens : array<int64>; npos, start_pos : int64) {
    let c = t.config
    if (start_pos == 0l) {
        ple_reset(s)
    }
    let hcd = hc_dim(c)
    let ind = ple_in_dim(c)
    for (p in range64(npos)) {
        hc_init_row(t, s.x_b, p * c.dim, s.hc_res_b, p * hcd)
    }
    let ts_gather = prof_ticks()
    for (p in range64(npos)) {
        ngram_gather(t, s, tokens, p, unsafe(addr(s.ple_emb_b[p * ind])))
    }
    ngram_advance(s, tokens, npos)
    prof_add("ple_gather", ts_gather)
}

//! the side input of one decode token, before layer ple_layer's attention mixer
def private ple_step_decode(t : Model; var s : Session; pos : int64) {
    let c = t.config
    let ind = ple_in_dim(c)
    mm_f(s.ple_key, t, KqFmt.q8, t.ple_key_off, s.ple_emb, s.xq, s.xs, s.xbs, ind, hc_dim(c))
    mm_f(s.ple_val, t, KqFmt.q8, t.ple_val_off, s.ple_emb, s.xq, s.xs, s.xbs, ind, c.dim)
    unsafe {
        ple_apply_row(t, s, s.hc_res, 0l, addr(s.ple_key[0]), addr < float const? >(s.ple_val[0]), pos)
    }
}

def private ple_step_prefill(t : Model; var s : Session; npos, start_pos : int64) {
    let c = t.config
    let ind = ple_in_dim(c)
    let hcd = hc_dim(c)
    mm_b_f(s, s.ple_key_b, t, KqFmt.q8, t.ple_key_off, s.ple_emb_b, ind, hcd, npos)
    mm_b_f(s, s.ple_val_b, t, KqFmt.q8, t.ple_val_off, s.ple_emb_b, ind, c.dim, npos)
    unsafe {
        for (p in range64(npos)) {   // in order: the conv ring reads the rows this loop wrote
            ple_apply_row(t, s, s.hc_res_b, p * hcd, addr(s.ple_key_b[p * hcd]), addr < float const? >(s.ple_val_b[p * c.dim]), start_pos + p)
        }
    }
}

def private mix_decode(t : Model; var s : Session; l : int64; ffn : bool) {
    let site = hc_site(l, ffn)
    let hcd = hc_dim(t.config)
    hc_mix(t, s, t.hc_norm_off + site * hcd, t.hc_down_offs[site], t.hc_up_offs[site], t.hc_inject_off + site * hcd * t.config.hc_count)
}

def private mix_prefill(t : Model; var s : Session; l, npos : int64; ffn : bool) {
    let site = hc_site(l, ffn)
    let hcd = hc_dim(t.config)
    hc_mix_b(t, s, t.hc_norm_off + site * hcd, t.hc_down_offs[site], t.hc_up_offs[site], t.hc_inject_off + site * hcd * t.config.hc_count, npos)
}

[no_alloc, no_env, no_io]
def private attention_qwen4exp_decode(t : Model; var s : Session; l : int64; pos : int64) {
    if (l == t.config.ple_layer) {
        let ts_ple = prof_ticks()
        ple_step_decode(t, s, pos)
        prof_add("ple", ts_ple)
    }
    let ts_mix = prof_ticks()
    mix_decode(t, s, l, false)
    prof_add("hc_mix", ts_mix)
    attention_qwen35_decode(t, s, l, pos)
}

[no_alloc, no_env, no_io]
def private ffn_qwen4exp_decode(t : Model; var s : Session; l : int64) {
    let ts_mix = prof_ticks()
    mix_decode(t, s, l, true)
    prof_add("hc_mix", ts_mix)
    ffn_moe_decode(t, s, l)
}

[no_alloc, no_env, no_io]
def private attention_qwen4exp_prefill(t : Model; var s : Session; l : int64; npos : int64; start_pos : int64) {
    if (l == t.config.ple_layer) {
        let ts_ple = prof_ticks()
        ple_step_prefill(t, s, npos, start_pos)
        prof_add("ple", ts_ple)
    }
    let ts_mix = prof_ticks()
    mix_prefill(t, s, l, npos, false)
    prof_add("hc_mix", ts_mix)
    attention_qwen35_prefill(t, s, l, npos, start_pos)
}

[no_alloc, no_env, no_io]
def private ffn_qwen4exp_prefill(t : Model; var s : Session; l : int64; npos : int64) {
    let ts_mix = prof_ticks()
    mix_prefill(t, s, l, npos, true)
    prof_add("hc_mix", ts_mix)
    ffn_moe_prefill(t, s, l, npos)
}

//! the head mixer stands where output_norm would: the classifier's row into xb
def private final_norm_qwen4exp(t : Model; var s : Session; row : int64; batched : bool) {
    if (batched) {
        hc_mix_row(t, s, s.hc_res_b, row * hc_dim(t.config), t.hc_head_norm_off, t.hc_head_down_off, t.hc_head_up_off, -1l)
    } else {
        hc_mix_row(t, s, s.hc_res, 0l, t.hc_head_norm_off, t.hc_head_down_off, t.hc_head_up_off, -1l)
    }
}

//! qwen4exp's block set: the qwen35moe hybrid inside the hyper-connection mixers, the n-gram side
//! input before layer ple_layer, the head mixer as the final norm. No batched decode form yet
//! (eval_batch falls back to per-row forward) and no dense-FFN gate for the GPU drivers.
def qwen4exp_blocks() : ArchBlocks {
    return ArchBlocks(attn_decode = @@attention_qwen4exp_decode, ffn_decode = @@ffn_qwen4exp_decode,
                      attn_prefill = @@attention_qwen4exp_prefill, ffn_prefill = @@ffn_qwen4exp_prefill,
                      attn_is_std = false, ffn_is_dense = false,
                      pre_stack_decode = @@pre_stack_decode_qwen4exp, pre_stack_prefill = @@pre_stack_prefill_qwen4exp,
                      final_norm = @@final_norm_qwen4exp)
}
