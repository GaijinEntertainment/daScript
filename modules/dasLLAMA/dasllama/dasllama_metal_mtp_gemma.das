options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_metal_mtp_gemma shared private

require dasllama/dasllama_lint public
require math
require das_metal
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_math   // reserve_resize / grow_resize - the scratch sizing
require dasllama/dasllama_batch
require dasllama/dasllama_mtp_gemma
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_lens   // release_handles
require dasllama/dasllama_metal_kernels
require dasllama/dasllama_metal_decode

//! One speculating stream's host-written chain inputs: its carried hidden and its anchor's rope rows for both attention classes.
//! A tick encodes every stream's chain into one command buffer before any runs, so each stream writes its own.
struct GemmaStreamIn {
    bh : MetalBuffer?          // [tdim] the target's post-norm hidden the chain opens on
    bcos : MetalBuffer?        // the anchor's rope row, full class (hs_full / 2)
    bsin : MetalBuffer?
    bcos_sw : MetalBuffer?     // ... sliding class (hs_sw / 2)
    bsin_sw : MetalBuffer?
    rope_pos : int64           // the anchor the rows hold (-1 = none)
}

//! The attached gemma-4 assistant drafter on the GPU: the q8 weights uploaded once, persistent
//! activation buffers, and a stream slot's chain inputs. The drafter has no K/V of its own - its
//! four Q-only layers read the TARGET's K/V mirror at the two capture layers.
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
    btok_out : MetalBuffer?    // [1] argmax out
    bvtok : MetalBuffer?       //! the tick's token rows: a stream's seed then its drafts at its group's first row
    bvtok_rows : int64
    btok1 : MetalBuffer?       //! [1] a step's id at offset 0, for the gather that reads its list from the start
    u_one1 : MetalBuffer?      // uniform 1: the one-token copies
    bpart : MetalBuffer?       // chunked attention partials (grown on demand)
    bpart_bytes : uint64
    u_dim : MetalBuffer?
    u_tdim : MetalBuffer?
    u_cat : MetalBuffer?
    u_hidden : MetalBuffer?
    u_vocab : MetalBuffer?
    u_eps : MetalBuffer?
    u_escale : MetalBuffer?
    u_qd : array<MetalBuffer?>      // per layer 16 * hs
    u_hs : array<MetalBuffer?>      // per layer hs (the tq4 un-rotation's head)
    u_scale : array<MetalBuffer?>   // per layer layer_output_scale
    slots : array<GemmaStreamIn>    // a speculating stream's inputs a slot, grown to the tick's streams
    hs_mx : int64
    cos_tab : array<float>          // host rope rows for an anchor (rebuilt per slot and anchor)
    sin_tab : array<float>
    cos_sw : array<float>
    sin_sw : array<float>
}

var g_gemma_drafter : GemmaDrafter
var g_gemma_gpu : GemmaDrafterGpu
var g_gemma_attached = false
var g_gemma_ws : BatchWorkspace
var g_gemma_ws_ok = false
var g_gemma_stats : MtpAssistantStats   //! the rounds since the attach

def private ubuf_u32(v : uint) : MetalBuffer? => scalar_buffer(metal_new_buffer_untracked(g_dev, 4ul), v)

def private ubuf_f32(v : float) : MetalBuffer? => scalar_buffer(metal_new_buffer_untracked(g_dev, 4ul), v)

def private gemma_slot_release(var sl : GemmaStreamIn) {
    release_handles(sl.bh, sl.bcos, sl.bsin, sl.bcos_sw, sl.bsin_sw)
    sl.rope_pos = -1l
}

def private gemma_gpu_release(var g : GemmaDrafterGpu) {
    release_handles(g.bx, g.bxin, g.bxn, g.bo, g.bq, g.battn)
    release_handles(g.bg, g.bu, g.bhn, g.blog, g.bhnext)
    release_handles(g.btok_out, g.bvtok, g.btok1, g.u_one1, g.bpart)
    release_handles(g.u_dim, g.u_tdim, g.u_cat, g.u_hidden)
    release_handles(g.u_vocab, g.u_eps, g.u_escale)
    for (u in g.u_qd) {
        metal_release(u)
    }
    for (u in g.u_hs) {
        metal_release(u)
    }
    for (u in g.u_scale) {
        metal_release(u)
    }
    for (sl in g.slots) {
        gemma_slot_release(sl)
    }
    g.u_qd |> clear()
    g.u_hs |> clear()
    g.u_scale |> clear()
    g.slots |> clear()
    g.bblob = null   // an upload_region - the region cache owns it
    g.bpart_bytes = 0ul
    g.bvtok_rows = 0l
    g.ready = false
}

def private gemma_gpu_init(dr : GemmaDrafter; escale : float; var g : GemmaDrafterGpu) {
    var hs_mx = 0l
    for (h in dr.hs) {
        hs_mx = max(hs_mx, h)
    }
    let f4 = 4ul
    g.hs_mx = hs_mx
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
    g.btok_out = metal_new_buffer(g_dev, 64ul)
    g.btok1 = metal_new_buffer(g_dev, 4ul)
    g.u_one1 = ubuf_u32(1u)
    g.u_dim = ubuf_u32(uint(dr.dim))
    g.u_tdim = ubuf_u32(uint(dr.tdim))
    g.u_cat = ubuf_u32(uint(2l * dr.tdim))
    g.u_hidden = ubuf_u32(uint(dr.hidden))
    g.u_vocab = ubuf_u32(uint(dr.vocab))
    g.u_eps = ubuf_f32(dr.eps)
    g.u_escale = ubuf_f32(escale)
    g.u_qd |> reserve(dr.n_layers)
    g.u_hs |> reserve(dr.n_layers)
    g.u_scale |> reserve(dr.n_layers)
    for (l in range64(dr.n_layers)) {
        g.u_qd |> push(ubuf_u32(uint(dr.n_heads * dr.hs[l])))
        g.u_hs |> push(ubuf_u32(uint(dr.hs[l])))
        g.u_scale |> push(ubuf_f32(dr.layer_scale[l]))
    }
    g.bblob = upload_region(unsafe(addr < void? >(dr.blob[0])), uint64(long_length(dr.blob)))
    gemma_token_rows(g, MTP_MAX_ROWS)
    g.ready = true
}

//! the tick's token rows grown to `rows`: a stream's group of seed and drafts each
def private gemma_token_rows(var g : GemmaDrafterGpu; rows : int64) {
    return if (g.bvtok_rows >= rows)
    release_handle(g.bvtok)
    g.bvtok_rows = max(rows, 2l * g.bvtok_rows)
    g.bvtok = metal_new_buffer(g_dev, uint64(g.bvtok_rows * 4l))
}

//! stream slot `si`'s chain inputs, made on first use
[cold_path]
def private gemma_slot(var g : GemmaDrafterGpu; dr : GemmaDrafter; si : int64) {
    while (long_length(g.slots) <= si) {
        let rope_b = uint64(g.hs_mx / 2l) * 4ul
        g.slots |> push(GemmaStreamIn(bh = metal_new_buffer(g_dev, uint64(dr.tdim) * 4ul),
            bcos = metal_new_buffer(g_dev, rope_b), bsin = metal_new_buffer(g_dev, rope_b),
            bcos_sw = metal_new_buffer(g_dev, rope_b), bsin_sw = metal_new_buffer(g_dev, rope_b), rope_pos = -1l))
    }
}

//! The Metal attach: `""` with the drafter attached to the served gemma-4 target `t` (the trunk `gemma_drafter_target_why`
//! admits, the metal-blob flavor, a token table the device gathers), else why it refused. One drafter at a time; the target's
//! decode then carries the post-norm hidden every step (the drafter's h input).
[arch(at="../ARCHITECTURE_GPU_MTP_ASSISTANT.md#assistant-drafter-attach")]
def public attach_gemma_drafter(t : Model; path : string) : string {
    detach_gemma_drafter()
    if (!t.metal_blob) {   //! before the sidecar's read: a trunk the Metal drivers do not serve takes no drafter here
        return "the trunk is not the metal-blob flavor the Metal drivers serve"
    }
    var dr <- load_gemma_drafter(path)
    if (!dr.ok) {
        let why = clone_string(dr.why)
        delete dr
        return why
    }
    let c = t.config
    let dev_ok = metal_common_init()   // the device comes up lazily with the first GPU step; attach may run before that
    var why = gemma_drafter_target_why(t, dr)
    if (empty(why) && (!dev_ok || g_dev == null)) {
        why = "no Metal device"
    } elif (empty(why) && !emb_dev_gather_ok(t)) {
        why = "the device gathers no row of the trunk's token table"
    }
    if (!empty(why)) {
        delete dr
        return why
    }
    delete g_gemma_drafter
    g_gemma_drafter <- dr
    gemma_drafter_bind_target(g_gemma_drafter, c.n_layers)
    gemma_gpu_init(g_gemma_drafter, c.embed_scale, g_gemma_gpu)   // the target's own token scale (gemma: sqrt(dim))
    delete g_gemma_ws
    g_gemma_ws <- create_batch_workspace_(t)
    g_gemma_ws_ok = true
    set_metal_mtp_carry_hidden(true, g_gemma_carry_pre)
    set_mtp_round_kind(MtpRoundKind.assistant)
    g_gemma_attached = true
    g_gemma_stats = MtpAssistantStats()
    to_log(LOG_INFO, "dasLLAMA gemma drafter attached: {path} ({g_gemma_drafter.n_layers} Q-only layers over target layers {g_gemma_drafter.kv_layer})\n")
    return ""
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


[arch(at="../ARCHITECTURE_GPU_MTP_ASSISTANT.md#assistant-drafter-step")]
def private gemma_rope_rows(t : Model; var g : GemmaDrafterGpu; si, anchor : int64) {
    var sl & = unsafe(g.slots[si])
    if (sl.rope_pos == anchor) {
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
            memcpy(metal_buffer_contents(sl.bcos), addr < void? >(g.cos_tab[0]), uint64(hs_full / 2l) * 4ul)
            memcpy(metal_buffer_contents(sl.bsin), addr < void? >(g.sin_tab[0]), uint64(hs_full / 2l) * 4ul)
        }
        if (hs_sw > 0l) {
            memcpy(metal_buffer_contents(sl.bcos_sw), addr < void? >(g.cos_sw[0]), uint64(hs_sw / 2l) * 4ul)
            memcpy(metal_buffer_contents(sl.bsin_sw), addr < void? >(g.sin_sw[0]), uint64(hs_sw / 2l) * 4ul)
        }
    }
    sl.rope_pos = anchor
}

//! the round's K/V codecs: the decode's every mirror codec on both sides alike (the attention stamps read each; tq4's query rotates
//! into the stored rows' basis and its output rotates back)
def private gemma_kv_ok(s : Session) : bool {
    let k = s.kv_dtype_k
    return k == s.kv_dtype_v && (k == KVDtype.f16 || k == KVDtype.f32 || k == KVDtype.q8_0 || k == KVDtype.tq4)
}

[arch(at="../ARCHITECTURE_GPU_MTP_ASSISTANT.md#assistant-drafter-step")]
def private encode_gemma_layer(t : Model; s : Session; enc : MetalComputeEncoder?; var g : GemmaDrafterGpu; si, l, cnt : int64; mr : KVMirror) {
    let c = t.config
    let dr & = g_gemma_drafter
    let dim = dr.dim
    let hs = dr.hs[l]
    let qd = dr.n_heads * hs
    let kvl = dr.kv_layer[l]
    let kdt = s.kv_dtype_k
    let sl = dr.sliding[l]
    let bcos = sl ? g.slots[si].bcos_sw : g.slots[si].bcos
    let bsin = sl ? g.slots[si].bsin_sw : g.slots[si].bsin
    unsafe {
        var bw_an = upload_region(addr < void? >(dr.fblob[dr.attn_norm_offs[l]]), uint64(dim) * 4ul)
        enc_rms(enc, g.bxin, 0ul, bw_an, g.bxn, g.u_dim, g.u_eps)
        enc_gemv(enc, g.bblob, uint64(dr.wq_offs[l]), g.bxn, g.bq, g.u_dim, g.u_qd[l], qd)
        var bw_qn = upload_region(addr < void? >(dr.fblob[dr.q_norm_offs[l]]), uint64(hs) * 4ul)
        var qka = qk_norm_args(uint(dr.n_heads), 0u, uint(hs), uint(qd), 0u, 0u, 0u, dr.eps)
        enc_qk_norm(enc, g.bq, g.bq, bw_qn, qka, dr.n_heads, 1l)
        if (kdt == KVDtype.tq4) {   // rope on Q only, rotated into the tq4 rows' basis: the q heads' threadgroups alone, so nothing stores
            var tka = RopeStoreTq4Args(qd = uint(qd), head_size = uint(hs), nheads = uint(dr.n_heads), kv_mul = uint(dr.n_heads / dr.kvh[l]),
                neox = c.rope_neox ? 1u : 0u, has_bias = 0u, rot = uint(hs), qhs = 0u)
            enc_rope_store_tq4(enc, g.bq, 0ul, g_arena, g_arena, 0ul, 0ul, bcos, bsin, g.bq, tka, dr.n_heads, hs)
        } else {   // rope on Q only: every pair is a Q pair, so the fused store's K/V lanes never run
            var rka = gk_rope_kv_args(qd, 0l, hs, qd / 2l, qd / 2l, c.rope_neox, false, hs)
            enc_rope_store(enc, true, g.bq, 0ul, g_arena, g_arena, 0ul, 0ul, bcos, bsin, 0ul, g_one, rka, qd / 2l)
        }
        let lb = uint64(mirror_lbase(t, kdt, mr.cap_rows, kvl))
        let lkoff = mr.koff + lb
        let lvoff = mr.voff + lb
        let kstride = uint(kdt == KVDtype.f16 || kdt == KVDtype.f32 ? layer_kv_dim(c, kvl) : layer_kv_dim(c, kvl) / 32l)
        let kv_mul = uint(dr.n_heads / dr.kvh[l])
        let window = sl && c.sliding_window > 0l ? uint(c.sliding_window - 1l) : 0u   //! the pass's count is the anchor, its query a row past the last key: one key short of the window
        if (cnt > g_attn_single_max) {
            let nchunks = (cnt + ATTN_CHUNK_ROWS - 1l) / ATTN_CHUNK_ROWS
            let need = uint64(dr.n_heads * nchunks * (hs + 2l)) * 4ul
            if (g.bpart_bytes < need) {
                release_handle(g.bpart)
                g.bpart = metal_new_buffer(g_dev, need)
                g.bpart_bytes = need
            }
            var pka = SqAttnPartArgs(cnt = uint(cnt), kstride = kstride, nheads = uint(dr.n_heads), kv_mul = kv_mul, hs = uint(hs),
                scale = 1.0, nchunks = uint(nchunks), softcap = 0.0, window = window, ring = uint(mirror_ring(t, kvl)))
            enc_attn_part(enc, kdt, g_arena, g_arena, lkoff, lvoff, g.bq, g.bpart, pka, dr.n_heads * nchunks, hs)
            let chlo = window > 0u && cnt > int64(window) ? uint((cnt - int64(window)) / ATTN_CHUNK_ROWS) : 0u
            var cka = SqAttnCombArgs(nch = uint(nchunks), nheads = uint(dr.n_heads), hs = uint(hs), chlo = chlo, window = window, hass = 0u)
            enc_attn_comb(enc, g.bpart, g.battn, cka, dr.n_heads, hs)
        } else {
            var ska = SqAttnArgs(cnt = uint(cnt), kstride = kstride, nheads = uint(dr.n_heads), kv_mul = kv_mul, hs = uint(hs),
                scale = 1.0, softcap = 0.0, hass = 0u)
            enc_attn(enc, kdt, g_arena, g_arena, lkoff, lvoff, g.bq, g.battn, ska, dr.n_heads, hs)
        }
        if (kdt == KVDtype.tq4) {
            enc_fwht_unsign(enc, g.battn, g.u_hs[l], g.u_qd[l], dr.n_heads, 1l, hs)
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
//! step's h (post_projection of the output-normed hidden). Synchronous - one command buffer, the chain of one step.
def public gemma_draft_step_gpu(t : Model; s : Session; tok, anchor : int64; h : array<float>; var out_tok : int64&; var h_next : array<float>) : bool {
    let dr & = g_gemma_drafter
    var g & = g_gemma_gpu
    var cb = metal_new_command_buffer_unretained(g_queue)
    var enc = metal_new_compute_encoder(cb)
    encode_gemma_chain(t, s, g, enc, 0l, 0l, tok, h, anchor, 1l)
    metal_end_encoding(enc)
    metal_release(enc)
    metal_commit(cb)
    metal_wait_until_completed(cb)
    let ok = !metal_command_buffer_failed(cb)
    if (ok) {
        unsafe {
            let tp = reinterpret<uint?>(metal_buffer_contents(g.bvtok))
            out_tok = int64(tp[1])
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

[arch(at="../ARCHITECTURE_GPU_MTP_ASSISTANT.md#assistant-drafter-step")]
def private encode_gemma_draft(t : Model; s : Session; enc : MetalComputeEncoder?; var g : GemmaDrafterGpu; si, anchor : int64; mr : KVMirror) {
    let dr & = g_gemma_drafter
    let dim = dr.dim
    enc_gemv(enc, g.bblob, uint64(dr.pre_proj_off), g.bx, g.bxin, g.u_cat, g.u_dim, dim)
    for (l in range64(dr.n_layers)) {
        encode_gemma_layer(t, s, enc, g, si, l, anchor, mr)
    }
    var bw_on = upload_region(unsafe(addr < void? >(dr.fblob[dr.out_norm_off])), uint64(dim) * 4ul)
    enc_rms(enc, g.bxin, 0ul, bw_on, g.bhn, g.u_dim, g.u_eps)
    enc_gemv(enc, g.bblob, uint64(dr.tok_emb_off), g.bhn, g.blog, g.u_dim, g.u_vocab, dr.vocab)
    enc_argmax(enc, g.blog, g.btok_out, g.u_vocab)
    enc_gemv(enc, g.bblob, uint64(dr.post_proj_off), g.bhn, g.bhnext, g.u_dim, g.u_tdim, dr.tdim)
}

//! Stream slot `si`'s `k` chained drafter steps at the constant anchor onto `enc`: step i embeds token row `row0 + i` (`seed` at
//! `row0`) beside `h` at step 0 and the step before's post-projection after it, its argmax landing in row `row0 + i + 1`. The
//! slot's inputs are written here, so a tick's streams encode into one command buffer before any runs
[arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-round-one-join")]
def private encode_gemma_chain(t : Model; s : Session; var g : GemmaDrafterGpu; enc : MetalComputeEncoder?; si, row0, seed : int64; h : array<float>; anchor, k : int64) {
    let dr & = g_gemma_drafter
    let mr = g_mirrors[s.uid]
    if (long_length(g.slots) <= si) {
        gemma_slot(g, dr, si)
    }
    gemma_rope_rows(t, g, si, anchor)
    gr_arm(false)
    unsafe {
        var tp = reinterpret<uint?>(metal_buffer_contents(g.bvtok))
        tp[row0] = uint(seed)
        memcpy(metal_buffer_contents(g.slots[si].bh), addr < void? >(h[0]), uint64(dr.tdim) * 4ul)
    }
    enc_copy_row(enc, g.slots[si].bh, 0ul, g.bx, uint64(dr.tdim * 4l), g.u_tdim, dr.tdim)
    let at = emb_dev_gather_at_ok(t)   //! the fp32 table's gather reads its id at the list's start: the step's id moves there first
    for (i in range64(k)) {
        if (i > 0l) {
            enc_copy_row(enc, g.bhnext, 0ul, g.bx, uint64(dr.tdim * 4l), g.u_tdim, dr.tdim)
        }
        let trow = uint64((row0 + i) * 4l)
        if (at) {
            enc_embed_dev(enc, t, g.bvtok, trow, g.bx, 0ul, 1l, g.u_tdim, g.u_escale)
        } else {
            enc_copy_row(enc, g.bvtok, trow, g.btok1, 0ul, g.u_one1, 1l)
            enc_embed_dev(enc, t, g.btok1, 0ul, g.bx, 0ul, 1l, g.u_tdim, g.u_escale)
        }
        encode_gemma_draft(t, s, enc, g, si, anchor, mr)
        enc_copy_row(enc, g.btok_out, 0ul, g.bvtok, uint64((row0 + i + 1l) * 4l), g.u_one1, 1l)
    }
}

//! every warm stream's chain onto the verify's first command buffer (`set_batch_same_slab_chain`): stream gi's token rows open
//! at the sum of the groups before it
[arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-round-one-join")]
def private gemma_encode_chains(t : Model; enc : MetalComputeEncoder?) {
    var row0 = 0l
    for (gi in range64(long_length(g_gw.sessions))) {
        let s & = unsafe(*g_gw.sessions[gi])
        encode_gemma_chain(t, s, g_gemma_gpu, enc, gi, row0, s.mtp_vbatch[0], s.mtp_h, g_gw.pos[gi], g_gw.nr[gi] - 1l)
        row0 += g_gw.nr[gi]
    }
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
var private g_gemma_carry_pre = false
var private g_gemma_cpu_out : GemmaDraftOut

//! Bring-up levers: `set_gemma_draft_on_cpu(true)` routes every draft step through the CPU oracle
//! (the target's live K/V, the same inputs), `set_gemma_trace(true)` logs drafts vs truth per round.
def public set_gemma_draft_on_cpu(on : bool) {
    g_gemma_draft_cpu = on
    if (on) {
        to_log(LOG_INFO, "dasLLAMA: gemma drafts route through the CPU oracle (set_gemma_draft_on_cpu)\n")
    }
}

def public gemma_draft_on_cpu() : bool => g_gemma_draft_cpu

def public set_gemma_trace(on : bool) {
    g_gemma_trace = on
}

def public gemma_trace() : bool => g_gemma_trace

//! The drafter's h input: the target's post-output_norm hidden (default) or the pre-norm residual.
//! Takes effect from the next decode step's landing; a choice made before an attach applies at it.
def public set_gemma_carry_pre_norm(on : bool) {
    g_gemma_carry_pre = on
    if (g_gemma_attached) {
        set_metal_mtp_carry_hidden(true, on)
    }
    if (on) {
        to_log(LOG_INFO, "dasLLAMA: gemma drafter reads the pre-norm residual (set_gemma_carry_pre_norm)\n")
    }
}

def public gemma_carry_pre_norm() : bool => g_gemma_carry_pre

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

// a round that cannot speculate takes one plain GPU step (output-invariant either way)
def private gemma_plain_step(t : Model; var s : Session; tok, pos : int64) {
    let tc = ref_time_ticks()
    forward(t, s, tok, pos)
    s.n_past = pos + 1l
    g_gemma_stats.cold_us += int64(get_time_usec(tc))
}

//! a stream that cannot speculate this round: no Metal blob or device, no carried hidden at its position, a round past the
//! context, a K/V codec the round does not serve, or a mirror short of the position or of the round's rows
def private gemma_round_cold(t : Model; s : Session; pos, k : int64) : bool {
    if (g_dev == null || !t.metal_blob || mtp_round_cold_host(t, s, pos, k, g_gemma_drafter.tdim) || !gemma_kv_ok(s)
            || !key_exists(g_mirrors, s.uid)) {
        return true
    }
    let mr = g_mirrors[s.uid]
    return mr.watermark < pos || mr.cap_rows < pos + k + 1l
}

var private @scratch g_gw : MtpRoundGather   //! the tick's warm streams, a verify group each
var private @scratch g_gw_toks : array<int64>

//! The gemma speculative ROUNDS of a tick's streams at depth `k_req` (each stream's under its `mtp_depth_cap`): every warm
//! stream's chain of drafts at its constant anchor `n_past` from (its token, the target's post-norm hidden of the position
//! before), all encoded into ONE command buffer; ONE same-slab batch verify of every stream's [tok, d_1 .. d_k] as a group
//! (`verify_batch_groups`); each stream's accept walk and commit. `acc[i]` is stream i's accepted count, the drafts in
//! `mtp_vbatch[1..a]`, its token always committed; a cold stream takes one plain GPU step. Returns the warm streams the verify carried
[hot_path, arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-round-one-join"), arch(at="../ARCHITECTURE_GPU_MTP_ASSISTANT.md#assistant-drafter-rounds")]
def private gemma_mtp_spec_rounds(t : Model; var sessions : array<Session?>; toks : array<int64>; k_req : int64; var acc : array<int64>) : int64 {   // nolint:STYLE038 — draft chains / one verify / accept walks / commits is one sequential protocol on shared locals
    let c = t.config
    let k = clamp(k_req, 1l, MTP_MAX_ROWS - 1l)
    let dr & = g_gemma_drafter
    g_gemma_h |> grow_resize(dr.tdim)
    g_gemma_hn |> grow_resize(dr.tdim)
    let ng = mtp_round_gather(g_gw, sessions, toks, k, acc) $(var s : Session; i, pos, ki : int64) : int64 {
        return 0l if (!gemma_round_cold(t, s, pos, ki))
        gemma_plain_step(t, s, toks[i], pos)
        return -1l
    }
    return 0l if (ng == 0l)
    finish_pending_step()
    let td = ref_time_ticks()
    let ts_draft = prof_ticks()
    let gpu_chain = !g_gemma_draft_cpu
    var rows = 0l
    for (nr in g_gw.nr) {
        rows += nr
    }
    var chain_ok = true
    if (gpu_chain) {   //! the chains encode inside the verify's build (`gemma_encode_chains`)
        gemma_token_rows(g_gemma_gpu, rows)
    } else {
        for (gi in range64(ng)) {
            var s & = unsafe(*g_gw.sessions[gi])
            chain_ok = chain_ok && gemma_draft_chain_cpu(t, s, g_gw.pos[gi], g_gw.nr[gi] - 1l) == g_gw.nr[gi] - 1l
        }
    }
    g_gemma_stats.draft_us += int64(get_time_usec(td))
    prof_add("mtp.draft", ts_draft)
    let tv = ref_time_ticks()
    let ts_verify = prof_ticks()
    g_gw_toks |> resize(rows)
    var row = 0l
    for (gi in range64(ng)) {
        for (i in range64(g_gw.nr[gi])) {
            g_gw_toks[row] = g_gw.sessions[gi].mtp_vbatch[i]   //! the device rows gather their own ids; the host rows read these where it does not
            row++
        }
    }
    if (gpu_chain) {
        set_batch_same_slab_chain(g_gemma_gpu.bvtok, @@gemma_encode_chains)
    }
    let vok = chain_ok && verify_batch_groups(t, g_gemma_ws, g_gw.sessions, g_gw_toks, g_gw.pos, g_gw.nr)
    set_batch_same_slab_tokens(null)
    finish_pending_step()
    if (!vok) {
        for (gi in range64(ng)) {
            var s & = unsafe(*g_gw.sessions[gi])
            gemma_plain_step(t, s, s.mtp_vbatch[0], g_gw.pos[gi])
        }
        g_gw.sessions |> clear()
        return 0l
    }
    prof_add("mtp.verify", ts_verify)
    let ts_walk = prof_ticks()
    row = 0l
    for (gi in range64(ng)) {
        var s & = unsafe(*g_gw.sessions[gi])
        let pos = g_gw.pos[gi]
        let kd = g_gw.nr[gi] - 1l
        let r0 = row
        if (gpu_chain) {
            unsafe {
                let tp = reinterpret<uint?>(metal_buffer_contents(g_gemma_gpu.bvtok))
                for (i in range64(kd)) {
                    s.mtp_vbatch[i + 1l] = int64(tp[r0 + i + 1l])
                }
            }
        }
        let a = mtp_accept_walk(s, kd, c.vocab_size) $(vr) => sameslab_argmax(r0 + vr)
        if (g_gemma_trace) {
            mtp_round_trace("gemma", s, pos, kd, kd + 1l, a, c.vocab_size)
        }
        mtp_round_take(s, a, kd, c.dim)
        mtp_round_commit(s, pos, kd, a)
        g_gemma_stats.drafted += kd
        g_gemma_stats.accepted += a
        g_gemma_stats.rounds++
        acc[g_gw.idx[gi]] = a
        row += g_gw.nr[gi]
    }
    if (ng > 1l) {
        g_gemma_stats.joint++
    }
    g_gemma_stats.verify_us += int64(get_time_usec(tv))
    prof_add("mtp.walk", ts_walk)
    g_gw.sessions |> clear()   // borrowed handles
    return ng
}

[unused_argument(t), hot_path]   //! one drafter a process: the attach checked it against its target; the batch tick asks every step
def private gemma_drafter_attached_to(t : Model) : bool => g_gemma_attached

def private gemma_drafter_seam_stats() : MtpAssistantStats => g_gemma_stats

[init]
def gemma_mtp_init {
    register_reload_prep(@@detach_gemma_drafter)
    register_mtp_assistant("metal", MtpAssistantSeams(attach = @@attach_gemma_drafter, attached = @@gemma_drafter_attached_to,
        rounds = @@gemma_mtp_spec_rounds, detach = @@detach_gemma_drafter, stats = @@gemma_drafter_seam_stats))
}
