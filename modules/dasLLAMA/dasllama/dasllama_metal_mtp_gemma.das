options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_metal_mtp_gemma shared private

require dasllama/dasllama_lint public
require math
require strings   // build_string - the trace line
require das_metal
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_batch
require dasllama/dasllama_mtp_gemma
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_kernels
require dasllama/dasllama_metal_decode

//! The attached gemma-4 assistant drafter on the GPU: the q8 weights uploaded once, persistent
//! activation buffers, and the round's rope rows for both attention classes. The drafter has no
//! K/V of its own - its four Q-only layers read the TARGET's K/V mirror at the two capture layers.
struct GemmaDrafterGpu {
    ready : bool
    bblob : MetalBuffer?
    bx : MetalBuffer?          // [2 x tdim] concat(embed(tok) * sqrt(tdim), h)
    bxin : MetalBuffer?        // [dim] residual stream
    bxn : MetalBuffer?         // [dim] norm out
    bo : MetalBuffer?          // [dim] branch out (attention / FFN)
    bq : MetalBuffer?          // [16 x hs_max] the layer's Q
    battn : MetalBuffer?       // [16 x hs_max] attention out
    bg : MetalBuffer?          // [hidden] gate -> gelu(gate) * up
    bu : MetalBuffer?          // [hidden] up
    bhn : MetalBuffer?         // [dim] output-norm hidden
    blog : MetalBuffer?        // [vocab] logits
    bhnext : MetalBuffer?      // [tdim] post_projection hidden = the next step's h
    btok : MetalBuffer?        // [1] token in (the oracle's single step)
    btok_out : MetalBuffer?    // [1] argmax out
    bvtok : MetalBuffer?       //! [MTP_MAX_ROWS] the round's token list
    u_one1 : MetalBuffer?      // uniform 1: the one-token copies
    bpart : MetalBuffer?       // chunked attention partials (grown on demand)
    bpart_bytes : uint64
    bcos : MetalBuffer?        // the anchor's rope row, full class (hs_full / 2)
    bsin : MetalBuffer?
    bcos_sw : MetalBuffer?     // ... sliding class (hs_sw / 2)
    bsin_sw : MetalBuffer?
    u_dim : MetalBuffer?
    u_tdim : MetalBuffer?
    u_cat : MetalBuffer?
    u_hidden : MetalBuffer?
    u_vocab : MetalBuffer?
    u_eps : MetalBuffer?
    u_escale : MetalBuffer?
    u_qd : array<MetalBuffer?>      // per layer 16 * hs
    u_scale : array<MetalBuffer?>   // per layer layer_output_scale
    cos_tab : array<float>          // host rope rows for the anchor (rebuilt per round)
    sin_tab : array<float>
    cos_sw : array<float>
    sin_sw : array<float>
    rope_pos : int64                // the anchor the rows hold (-1 = none)
}

var g_gemma_drafter : GemmaDrafter
var g_gemma_gpu : GemmaDrafterGpu
var g_gemma_attached = false
var g_gemma_ws : BatchWorkspace
var g_gemma_ws_ok = false
var g_gemma_drafted = 0l
var g_gemma_accepted = 0l
var g_gemma_rounds = 0l

def private ubuf_u32(v : uint) : MetalBuffer? {
    var buf = metal_new_buffer_untracked(g_dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def private ubuf_f32(v : float) : MetalBuffer? {
    var buf = metal_new_buffer_untracked(g_dev, 4ul)
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def private rel(var b : MetalBuffer?&) {
    if (b != null) {
        metal_release(b)
        b = null
    }
}

def private gemma_gpu_release(var g : GemmaDrafterGpu) {
    rel(g.bx)
    rel(g.bxin)
    rel(g.bxn)
    rel(g.bo)
    rel(g.bq)
    rel(g.battn)
    rel(g.bg)
    rel(g.bu)
    rel(g.bhn)
    rel(g.blog)
    rel(g.bhnext)
    rel(g.btok)
    rel(g.btok_out)
    rel(g.bvtok)
    rel(g.u_one1)
    rel(g.bpart)
    rel(g.bcos)
    rel(g.bsin)
    rel(g.bcos_sw)
    rel(g.bsin_sw)
    rel(g.u_dim)
    rel(g.u_tdim)
    rel(g.u_cat)
    rel(g.u_hidden)
    rel(g.u_vocab)
    rel(g.u_eps)
    rel(g.u_escale)
    for (u in g.u_qd) {
        metal_release(u)
    }
    for (u in g.u_scale) {
        metal_release(u)
    }
    g.u_qd |> clear()
    g.u_scale |> clear()
    g.bblob = null   // an upload_region - the region cache owns it
    g.bpart_bytes = 0ul
    g.rope_pos = -1l
    g.ready = false
}

def private gemma_gpu_init(dr : GemmaDrafter; escale : float; var g : GemmaDrafterGpu) {
    var hs_mx = 0l
    for (h in dr.hs) {
        hs_mx = max(hs_mx, h)
    }
    let f4 = 4ul
    g.bx = metal_new_buffer(g_dev, uint64(2l * dr.tdim) * f4)
    g.bxin = metal_new_buffer(g_dev, uint64(dr.dim) * f4)
    g.bxn = metal_new_buffer(g_dev, uint64(dr.dim) * f4)
    g.bo = metal_new_buffer(g_dev, uint64(dr.dim) * f4)
    g.bq = metal_new_buffer(g_dev, uint64(dr.n_heads * hs_mx) * f4)
    g.battn = metal_new_buffer(g_dev, uint64(dr.n_heads * hs_mx) * f4)
    g.bg = metal_new_buffer(g_dev, uint64(dr.hidden) * f4)
    g.bu = metal_new_buffer(g_dev, uint64(dr.hidden) * f4)
    g.bhn = metal_new_buffer(g_dev, uint64(dr.dim) * f4)
    g.blog = metal_new_buffer(g_dev, uint64(dr.vocab) * f4)
    g.bhnext = metal_new_buffer(g_dev, uint64(dr.tdim) * f4)
    g.btok = metal_new_buffer(g_dev, 64ul)
    g.btok_out = metal_new_buffer(g_dev, 64ul)
    g.bvtok = metal_new_buffer(g_dev, 64ul)
    g.u_one1 = ubuf_u32(1u)
    g.bcos = metal_new_buffer(g_dev, uint64(hs_mx / 2l) * f4)
    g.bsin = metal_new_buffer(g_dev, uint64(hs_mx / 2l) * f4)
    g.bcos_sw = metal_new_buffer(g_dev, uint64(hs_mx / 2l) * f4)
    g.bsin_sw = metal_new_buffer(g_dev, uint64(hs_mx / 2l) * f4)
    g.u_dim = ubuf_u32(uint(dr.dim))
    g.u_tdim = ubuf_u32(uint(dr.tdim))
    g.u_cat = ubuf_u32(uint(2l * dr.tdim))
    g.u_hidden = ubuf_u32(uint(dr.hidden))
    g.u_vocab = ubuf_u32(uint(dr.vocab))
    g.u_eps = ubuf_f32(dr.eps)
    g.u_escale = ubuf_f32(escale)
    g.u_qd |> reserve(dr.n_layers)
    g.u_scale |> reserve(dr.n_layers)
    for (l in range64(dr.n_layers)) {
        g.u_qd |> push(ubuf_u32(uint(dr.n_heads * dr.hs[l])))
        g.u_scale |> push(ubuf_f32(dr.layer_scale[l]))
    }
    unsafe {
        g.bblob = upload_region(addr < void? >(dr.blob[0]), uint64(long_length(dr.blob)))
    }
    g.rope_pos = -1l
    g.ready = true
}

//! Attach the drafter GGUF at `path` for the served gemma-4 target `t` (its dim must equal the
//! drafter's tdim, the target must be the metal-blob flavor with a Q8 token table). One drafter at
//! a time; the target's decode then carries the post-norm hidden every step (the drafter's h input).
def public attach_gemma_drafter(t : Model; path : string) : bool {
    detach_gemma_drafter()
    var dr <- load_gemma_drafter(path)
    if (!dr.ok) {
        to_log(LOG_WARNING, "dasLLAMA gemma drafter: {path}: {dr.why}\n")
        delete dr
        return false
    }
    let c = t.config
    let dev_ok = metal_common_init()   // the device comes up lazily with the first GPU step; attach may run before that
    if (!dev_ok || g_dev == null || !t.metal_blob || dr.tdim != c.dim || c.n_layers < 2l || !t.cls_q8) {
        to_log(LOG_WARNING, "dasLLAMA gemma drafter: {path} refused - needs a served metal-blob gemma-4 target with dim {dr.tdim} and a q8 token table (dim {c.dim}, blob {t.metal_blob}, cls_q8 {t.cls_q8}, device {dev_ok})\n")
        delete dr
        return false
    }
    delete g_gemma_drafter
    g_gemma_drafter <- dr
    gemma_drafter_bind_target(g_gemma_drafter, c.n_layers)
    gemma_gpu_init(g_gemma_drafter, c.embed_scale, g_gemma_gpu)   // the target's own token scale (gemma: sqrt(dim))
    delete g_gemma_ws
    g_gemma_ws <- create_batch_workspace_(t)
    g_gemma_ws_ok = true
    set_metal_mtp_carry_hidden(true)
    set_mtp_round_kind(MtpRoundKind.assistant)
    g_gemma_attached = true
    g_gemma_drafted = 0l
    g_gemma_accepted = 0l
    g_gemma_rounds = 0l
    g_gemma_us_draft = 0l
    g_gemma_us_verify = 0l
    g_gemma_us_cold = 0l
    to_log(LOG_INFO, "dasLLAMA gemma drafter attached: {path} ({g_gemma_drafter.n_layers} Q-only layers over target layers {g_gemma_drafter.kv_layer})\n")
    return true
}

def public detach_gemma_drafter {
    if (!g_gemma_attached) {
        return
    }
    finish_pending_step()
    gemma_gpu_release(g_gemma_gpu)
    if (g_gemma_ws_ok) {
        delete g_gemma_ws
        g_gemma_ws_ok = false
    }
    delete g_gemma_drafter
    set_metal_mtp_carry_hidden(false)
    set_mtp_round_kind(MtpRoundKind.nextn)
    g_gemma_attached = false
}

def public gemma_drafter_attached() : bool => g_gemma_attached

//! drafted / accepted / rounds since attach - the acceptance lens
def public gemma_drafter_stats() : tuple<drafted : int64; accepted : int64; rounds : int64> => (drafted = g_gemma_drafted, accepted = g_gemma_accepted, rounds = g_gemma_rounds)

[arch(at="../ARCHITECTURE_GPU_MTP.md#assistant-drafter-step")]
def private gemma_rope_rows(t : Model; var g : GemmaDrafterGpu; anchor : int64) {
    if (g.rope_pos == anchor) {
        return
    }
    let c = t.config
    let dr & = g_gemma_drafter
    var hs_full = 0l
    var hs_sw = 0l
    for (l in range64(dr.n_layers)) {
        if (dr.sliding[l]) {
            hs_sw = dr.hs[l]
        } else {
            hs_full = dr.hs[l]
        }
    }
    if (hs_full > 0l) {
        build_rope_table(g.cos_tab, g.sin_tab, t, c.rope_freq_base, c.rope_freq_scale, anchor, 1l, hs_full)
    }
    if (hs_sw > 0l) {
        build_rope_table(g.cos_sw, g.sin_sw, t, c.rope_freq_base_swa, c.rope_freq_scale_swa, anchor, 1l, hs_sw, false)
    }
    unsafe {
        if (hs_full > 0l) {
            memcpy(metal_buffer_contents(g.bcos), addr < void? >(g.cos_tab[0]), uint64(hs_full / 2l) * 4ul)
            memcpy(metal_buffer_contents(g.bsin), addr < void? >(g.sin_tab[0]), uint64(hs_full / 2l) * 4ul)
        }
        if (hs_sw > 0l) {
            memcpy(metal_buffer_contents(g.bcos_sw), addr < void? >(g.cos_sw[0]), uint64(hs_sw / 2l) * 4ul)
            memcpy(metal_buffer_contents(g.bsin_sw), addr < void? >(g.sin_sw[0]), uint64(hs_sw / 2l) * 4ul)
        }
    }
    g.rope_pos = anchor
}

[arch(at="../ARCHITECTURE_GPU_MTP.md#assistant-drafter-step")]
def private encode_gemma_layer(t : Model; s : Session; enc : MetalComputeEncoder?; var g : GemmaDrafterGpu; l, cnt : int64; mr : KVMirror) {
    let c = t.config
    let dr & = g_gemma_drafter
    let dim = dr.dim
    let hs = dr.hs[l]
    let qd = dr.n_heads * hs
    let kvl = dr.kv_layer[l]
    let kdt = s.kv_dtype_k
    let sl = dr.sliding[l]
    unsafe {
        var bw_an = upload_region(addr < void? >(dr.fblob[dr.attn_norm_offs[l]]), uint64(dim) * 4ul)
        enc_rms(enc, g.bxin, 0ul, bw_an, g.bxn, g.u_dim, g.u_eps)
        enc_gemv(enc, g.bblob, uint64(dr.wq_offs[l]), g.bxn, g.bq, g.u_dim, g.u_qd[l], qd)
        var bw_qn = upload_region(addr < void? >(dr.fblob[dr.q_norm_offs[l]]), uint64(hs) * 4ul)
        var qka = QkNormArgs(nq = uint(dr.n_heads), koffe = 0u, rstride = uint(qd), hs = uint(hs), woffe = 0u, eps = dr.eps, soffe = 0u)
        enc_qk_norm(enc, g.bq, 0ul, bw_qn, qka, dr.n_heads, 1l, hs)
        // rope on Q only: every pair is a Q pair, so the fused store's K/V lanes never run
        var rka = RopeStoreArgs(qd = uint(qd), head_size = uint(hs), npairs_q = uint(qd / 2l), npairs = uint(qd / 2l),
            neox = c.rope_neox ? 1u : 0u, has_bias = 0u, rot = uint(hs))
        enc_rope_store(enc, true, g.bq, 0ul, g_arena, g_arena, 0ul, 0ul, sl ? g.bcos_sw : g.bcos, sl ? g.bsin_sw : g.bsin, g_one, rka, qd / 2l)
        let lb = uint64(mirror_lbase(t, kdt, mr.cap_rows, kvl))
        let lkoff = mr.koff + lb
        let lvoff = mr.voff + lb
        let kstride = uint(kdt == KVDtype.f16 || kdt == KVDtype.f32 ? layer_kv_dim(c, kvl) : layer_kv_dim(c, kvl) / 32l)
        let kv_mul = uint(dr.n_heads / dr.kvh[l])
        let window = sl && c.sliding_window > 0l ? uint(c.sliding_window) : 0u
        if (cnt > g_attn_single_max) {
            let nchunks = (cnt + ATTN_CHUNK_ROWS - 1l) / ATTN_CHUNK_ROWS
            let need = uint64(dr.n_heads * nchunks * (hs + 2l)) * 4ul
            if (g.bpart_bytes < need) {
                rel(g.bpart)
                g.bpart = metal_new_buffer(g_dev, need)
                g.bpart_bytes = need
            }
            var pka = SqAttnPartArgs(cnt = uint(cnt), kstride = kstride, nheads = uint(dr.n_heads), kv_mul = kv_mul, hs = uint(hs),
                scale = 1.0, nchunks = uint(nchunks), softcap = 0.0, window = window)
            enc_attn_part(enc, kdt, g_arena, g_arena, lkoff, lvoff, g.bq, g.bpart, pka, dr.n_heads * nchunks, hs)
            let chlo = window > 0u && cnt > c.sliding_window ? uint((cnt - c.sliding_window) / ATTN_CHUNK_ROWS) : 0u
            var cka = SqAttnCombArgs(nch = uint(nchunks), nheads = uint(dr.n_heads), hs = uint(hs), chlo = chlo, window = window, hass = 0u)
            enc_attn_comb(enc, g.bpart, g.battn, cka, dr.n_heads, hs)
        } else {
            var ska = SqAttnArgs(cnt = uint(cnt), kstride = kstride, nheads = uint(dr.n_heads), kv_mul = kv_mul, hs = uint(hs),
                scale = 1.0, softcap = 0.0, hass = 0u)
            enc_attn(enc, kdt, g_arena, g_arena, lkoff, lvoff, g.bq, g.battn, ska, dr.n_heads, hs)
        }
        enc_gemv(enc, g.bblob, uint64(dr.wo_offs[l]), g.battn, g.bo, g.u_qd[l], g.u_dim, dim)
        var bw_pan = upload_region(addr < void? >(dr.fblob[dr.post_attn_norm_offs[l]]), uint64(dim) * 4ul)
        enc_rms(enc, g.bo, 0ul, bw_pan, g.bo, g.u_dim, g.u_eps)
        enc_add(enc, g.bxin, 0ul, g.bo, 0ul, g.u_dim, dim)
        var bw_fn = upload_region(addr < void? >(dr.fblob[dr.ffn_norm_offs[l]]), uint64(dim) * 4ul)
        enc_rms(enc, g.bxin, 0ul, bw_fn, g.bxn, g.u_dim, g.u_eps)
        enc_gemv(enc, g.bblob, uint64(dr.w1_offs[l]), g.bxn, g.bg, g.u_dim, g.u_hidden, dr.hidden)
        enc_gemv(enc, g.bblob, uint64(dr.w3_offs[l]), g.bxn, g.bu, g.u_dim, g.u_hidden, dr.hidden)
        enc_geglu(enc, g.bg, 0ul, g.bu, 0ul, g.u_hidden, dr.hidden)
        enc_gemv(enc, g.bblob, uint64(dr.w2_offs[l]), g.bg, g.bo, g.u_hidden, g.u_dim, dim)
        var bw_pfn = upload_region(addr < void? >(dr.fblob[dr.post_ffn_norm_offs[l]]), uint64(dim) * 4ul)
        enc_rms(enc, g.bo, 0ul, bw_pfn, g.bo, g.u_dim, g.u_eps)
        enc_add(enc, g.bxin, 0ul, g.bo, 0ul, g.u_dim, dim, g.u_scale[l])
    }
}

//! One drafter step on the GPU: (tok, h) at the constant anchor -> the draft token and the next
//! step's h (post_projection of the output-normed hidden). Synchronous - one command buffer.
def public gemma_draft_step_gpu(t : Model; s : Session; tok, anchor : int64; h : array<float>; var out_tok : int64&; var h_next : array<float>) : bool {
    let dr & = g_gemma_drafter
    var g & = g_gemma_gpu
    let mr = g_mirrors[s.uid]
    gemma_rope_rows(t, g, anchor)
    var cb = metal_new_command_buffer_unretained(g_queue)
    var enc = metal_new_compute_encoder(cb)
    gr_arm(false)
    unsafe {
        var tp = reinterpret<uint?>(metal_buffer_contents(g.btok))
        tp[0] = uint(tok)
        var bxp = reinterpret<uint8?>(metal_buffer_contents(g.bx))
        memcpy(reinterpret<void?>(bxp + dr.tdim * 4l), addr < void? >(h[0]), uint64(dr.tdim) * 4ul)
        let temb = blob_of(g_dev, t, t.emb_q8_off)   // the target's q8 token table (the decode's own embed source)
        enc_embed(enc, temb.buf, temb.boff, g.btok, g.bx, g.u_tdim, g.u_escale, dr.tdim)
        encode_gemma_draft(t, s, enc, g, anchor, mr)
    }
    metal_end_encoding(enc)
    metal_release(enc)
    metal_commit(cb)
    metal_wait_until_completed(cb)
    let ok = !metal_command_buffer_failed(cb)
    if (ok) {
        unsafe {
            let tp = reinterpret<uint?>(metal_buffer_contents(g.btok_out))
            out_tok = int64(tp[0])
            memcpy(addr < void? >(h_next[0]), metal_buffer_contents(g.bhnext), uint64(dr.tdim) * 4ul)
        }
    } else {
        gemma_dispatch_failed(cb)
    }
    metal_release(cb)
    return ok
}

[cold_path]
def private gemma_dispatch_failed(cb : MetalCommandBuffer?) {
    to_log(LOG_ERROR, "dasLLAMA gemma drafter: dispatch failed ({cb_error_text(cb)})\n")
}

//! The last GPU draft step's full-vocab logits (the oracle comparison reads them; the round never does).
def public gemma_drafter_last_logits(var dst : array<float>) {
    let dr & = g_gemma_drafter
    dst |> reserve_resize(dr.vocab)
    unsafe {
        memcpy(addr < void? >(dst[0]), metal_buffer_contents(g_gemma_gpu.blog), uint64(dr.vocab) * 4ul)
    }
}

[arch(at="../ARCHITECTURE_GPU_MTP.md#assistant-drafter-step")]
def private encode_gemma_draft(t : Model; s : Session; enc : MetalComputeEncoder?; var g : GemmaDrafterGpu; anchor : int64; mr : KVMirror) {
    let dr & = g_gemma_drafter
    let dim = dr.dim
    unsafe {
        enc_gemv(enc, g.bblob, uint64(dr.pre_proj_off), g.bx, g.bxin, g.u_cat, g.u_dim, dim)
        for (l in range64(dr.n_layers)) {
            encode_gemma_layer(t, s, enc, g, l, anchor, mr)
        }
        var bw_on = upload_region(addr < void? >(dr.fblob[dr.out_norm_off]), uint64(dim) * 4ul)
        enc_rms(enc, g.bxin, 0ul, bw_on, g.bhn, g.u_dim, g.u_eps)
        enc_gemv(enc, g.bblob, uint64(dr.tok_emb_off), g.bhn, g.blog, g.u_dim, g.u_vocab, dr.vocab)
        enc_argmax(enc, g.blog, g.btok_out, g.u_vocab)
        enc_gemv(enc, g.bblob, uint64(dr.post_proj_off), g.bhn, g.bhnext, g.u_dim, g.u_tdim, dr.tdim)
    }
}

[arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-round-one-join")]
def private gemma_draft_chain_gpu(t : Model; s : Session; seed, anchor, k : int64) : bool {
    let dr & = g_gemma_drafter
    var g & = g_gemma_gpu
    let mr = g_mirrors[s.uid]
    gemma_rope_rows(t, g, anchor)
    var cb = metal_new_command_buffer_unretained(g_queue)
    var enc = metal_new_compute_encoder(cb)
    gr_arm(false)
    unsafe {
        var tp = reinterpret<uint?>(metal_buffer_contents(g.bvtok))
        tp[0] = uint(seed)
        var bxp = reinterpret<uint8?>(metal_buffer_contents(g.bx))
        memcpy(reinterpret<void?>(bxp + dr.tdim * 4l), addr < void? >(s.mtp_h[0]), uint64(dr.tdim) * 4ul)
        let temb = blob_of(g_dev, t, t.emb_q8_off)
        for (i in range64(k)) {
            if (i > 0l) {
                enc_copy_row(enc, g.bhnext, 0ul, g.bx, uint64(dr.tdim * 4l), g.u_tdim, dr.tdim)
            }
            enc_embed(enc, temb.buf, temb.boff, g.bvtok, g.bx, g.u_tdim, g.u_escale, dr.tdim, uint64(i * 4l), 0ul)
            encode_gemma_draft(t, s, enc, g, anchor, mr)
            enc_copy_row(enc, g.btok_out, 0ul, g.bvtok, uint64((i + 1l) * 4l), g.u_one1, 1l)
        }
    }
    metal_end_encoding(enc)
    metal_release(enc)
    metal_commit(cb)
    let ok = !metal_command_buffer_failed(cb)
    metal_release(cb)
    return ok
}

def private gemma_draft_chain_cpu(t : Model; var s : Session; pos, k : int64) : int64 {
    let dr & = g_gemma_drafter
    var drafted = 0l
    unsafe {
        memcpy(addr < void? >(g_gemma_h[0]), addr < void? >(s.mtp_h[0]), uint64(dr.tdim) * 4ul)
    }
    for (i in range64(k)) {
        var d = 0l
        if (!gemma_draft_step(t, s, s.mtp_vbatch[i], pos, g_gemma_h, d, g_gemma_hn)) {
            break
        }
        s.mtp_vbatch[i + 1l] = d
        drafted++
        unsafe {
            memcpy(addr < void? >(g_gemma_h[0]), addr < void? >(g_gemma_hn[0]), uint64(dr.tdim) * 4ul)
        }
    }
    return drafted
}

var private @scratch g_gemma_h : array<float>      // the chain's h input (tdim)
var private @scratch g_gemma_hn : array<float>     // ... and the drafter's h_next
var private g_gemma_draft_cpu = false
var private g_gemma_trace = false
var private g_gemma_cpu_out : GemmaDraftOut
var private g_gemma_us_draft = 0l   // round time split since attach: the draft chain ...
var private g_gemma_us_verify = 0l  // ... the same-slab verify + walk + commit
var private g_gemma_us_cold = 0l    // ... cold rounds (one plain step)

//! Round time split since attach, microseconds: draft chain / verify+commit / cold rounds.
def public gemma_drafter_time() : tuple<draft_us : int64; verify_us : int64; cold_us : int64> => (draft_us = g_gemma_us_draft, verify_us = g_gemma_us_verify, cold_us = g_gemma_us_cold)

//! Bring-up levers: `set_gemma_draft_on_cpu(true)` routes every draft step through the CPU oracle
//! (the target's live K/V, the same inputs), `set_gemma_trace(true)` logs drafts vs truth per round.
def public set_gemma_draft_on_cpu(on : bool) {
    g_gemma_draft_cpu = on
    if (on) {
        to_log(LOG_INFO, "dasLLAMA: gemma drafts route through the CPU oracle (set_gemma_draft_on_cpu)\n")
    }
}

def public set_gemma_trace(on : bool) {
    g_gemma_trace = on
}

//! The drafter's h input: the target's post-output_norm hidden (default) or the pre-norm residual.
//! Takes effect from the next decode step's landing.
def public set_gemma_carry_pre_norm(on : bool) {
    if (g_gemma_attached) {
        set_metal_mtp_carry_hidden(true, on)
        if (on) {
            to_log(LOG_INFO, "dasLLAMA: gemma drafter reads the pre-norm residual (set_gemma_carry_pre_norm)\n")
        }
    }
}

def private gemma_draft_step(t : Model; var s : Session; tok, anchor : int64; h : array<float>; var out_tok : int64&; var h_next : array<float>) : bool {
    if (!g_gemma_draft_cpu) {
        return gemma_draft_step_gpu(t, s, tok, anchor, h, out_tok, h_next)
    }
    gemma_draft_step_cpu(t, s, g_gemma_drafter, tok, h, anchor, g_gemma_cpu_out)
    out_tok = g_gemma_cpu_out.token
    unsafe {
        memcpy(addr < void? >(h_next[0]), addr < void? >(g_gemma_cpu_out.h_next[0]), uint64(long_length(h_next)) * 4ul)
    }
    return true
}

[cold_path]
def private gemma_trace_round(s : Session; pos, drafted, a, vocab : int64) {
    let line = build_string() $(var w) {
        w |> write("gemma round pos={pos} drafts=[")
        for (i in range64(drafted)) {
            w |> write(" {s.mtp_vbatch[i + 1l]}")
        }
        w |> write(" ] truth=[")
        for (i in range64(drafted)) {
            w |> write(" {argmax_row(s.mtp_logits_b, i, vocab)}")
        }
        w |> write(" ] a={a}")
    }
    to_log(LOG_INFO, "{line}\n")
}

def private gemma_accept_walk(var s : Session; drafted, vocab : int64) : int64 {
    if (mtp_walk_samples(s)) {
        return mtp_walk_sampled(s, drafted, vocab)
    }
    var a = 0l
    while (a < drafted && sameslab_argmax(a) == s.mtp_vbatch[a + 1l]) {
        a++
    }
    return a
}

// a round that cannot speculate takes one plain GPU step (output-invariant either way)
def private gemma_plain_step(t : Model; var s : Session; tok, pos : int64) : int64 {
    let tc = ref_time_ticks()
    forward(t, s, tok, pos)
    s.n_past = pos + 1l
    g_gemma_us_cold += int64(get_time_usec(tc))
    return 0l
}

//! The gemma speculative ROUND: k chained drafts at the constant anchor `pos` from (tok, the
//! target's post-norm hidden of the position before), ONE same-slab batch verify of
//! [tok, d_1 .. d_k], the accept walk, the commit. Same contract as metal_mtp_spec_round: returns a,
//! the accepted drafts are s.mtp_vbatch[1..a], tok is always committed; a cold round takes one
//! plain GPU step and returns 0. A model without a drafter (the NextN carriers) takes the metal round.
[hot_path, arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-round-one-join")]
def public gemma_mtp_spec_round(t : Model; var s : Session; tok : int64; k_req : int64) : int64 {
    if (!g_gemma_attached) {
        return metal_mtp_spec_round(t, s, tok, k_req)
    }
    let c = t.config
    let pos = s.n_past
    let k = clamp(k_req, 1l, MTP_MAX_ROWS - 1l)
    let dr & = g_gemma_drafter
    if (long_length(s.mtp_vbatch) < MTP_MAX_ROWS) {   // sized for the NextN carriers only at make_run_state
        s.mtp_vbatch |> reserve_resize(MTP_MAX_ROWS)   // nolint:PERF026 — once per session
    }
    g_gemma_h |> grow_resize(dr.tdim)
    g_gemma_hn |> grow_resize(dr.tdim)
    var cold = (g_dev == null || !t.metal_blob || pos < 1l || s.mtp_h_pos1 != pos || long_length(s.mtp_h) < dr.tdim ||
        pos + k + 1l > c.seq_len || s.kv_dtype_k != KVDtype.f16 || s.kv_dtype_v != KVDtype.f16 || !key_exists(g_mirrors, s.uid))
    if (!cold) {
        let mr = g_mirrors[s.uid]
        cold = mr.watermark < pos || mr.cap_rows < pos + k + 1l
    }
    if (cold) {
        return gemma_plain_step(t, s, tok, pos)
    }
    finish_pending_step()
    let td = ref_time_ticks()
    let ts_draft = prof_ticks()
    s.mtp_vbatch[0] = tok
    s.mtp_pre_drawn = false
    let gpu_chain = !g_gemma_draft_cpu
    let drafted = gpu_chain ? (gemma_draft_chain_gpu(t, s, tok, pos, k) ? k : 0l) : gemma_draft_chain_cpu(t, s, pos, k)
    if (drafted == 0l) {
        return gemma_plain_step(t, s, tok, pos)
    }
    g_gemma_us_draft += int64(get_time_usec(td))
    prof_add("mtp.draft", ts_draft)
    let tv = ref_time_ticks()
    let ts_verify = prof_ticks()
    let nr = drafted + 1l
    if (gpu_chain) {
        set_batch_same_slab_tokens(g_gemma_gpu.bvtok)
    }
    let vok = verify_batch_step(t, g_gemma_ws, s, s.mtp_vbatch, pos, nr)
    set_batch_same_slab_tokens(null)
    if (!vok) {
        return gemma_plain_step(t, s, tok, pos)
    }
    prof_add("mtp.verify", ts_verify)
    let ts_walk = prof_ticks()
    if (gpu_chain) {
        unsafe {
            let tp = reinterpret<uint?>(metal_buffer_contents(g_gemma_gpu.bvtok))
            for (i in range64(drafted)) {
                s.mtp_vbatch[i + 1l] = int64(tp[i + 1l])
            }
        }
    }
    g_gemma_drafted += drafted
    g_gemma_rounds++
    let a = gemma_accept_walk(s, drafted, c.vocab_size)
    if (g_gemma_trace) {
        gemma_trace_round(s, pos, drafted, a, c.vocab_size)
    }
    unsafe {
        memcpy(addr < void? >(s.logits[0]), addr < void? >(s.mtp_logits_b[a * c.vocab_size]), uint64(c.vocab_size) * 4ul)
        memcpy(addr < void? >(s.mtp_h[0]), addr < void? >(s.mtp_hrows[a * c.dim]), uint64(c.dim) * 4ul)
    }
    s.mtp_h_pos1 = pos + a + 1l
    g_mirrors[s.uid].watermark = pos + a + 1l
    s.mtp_accepted += a
    s.mtp_drafted += drafted
    mtp_pos_note(drafted, a)
    g_gemma_accepted += a
    s.n_past = pos + a + 1l
    g_gemma_us_verify += int64(get_time_usec(tv))
    prof_add("mtp.walk", ts_walk)
    return a
}

[init]
def gemma_mtp_init {
    register_mtp_round_override("metal", @@gemma_mtp_spec_round)
    register_reload_prep(@@detach_gemma_drafter)
}
