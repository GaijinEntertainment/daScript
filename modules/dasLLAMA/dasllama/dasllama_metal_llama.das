options gen2
options _comment_hygiene = true

module dasllama_metal_llama shared public

require math
require daslib/fio
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_metal_prefill
require dasllama/dasllama_metal_common public
require dasllama/dasllama_metal_kernels public

// The dasMetal GPU-resident DECODE path (the emission arc's P2 driver + P3 chase shape) — the
// mirror image of dasllama_metal_prefill: one command buffer per token step runs the whole llama
// layer stack on the GPU. Decode is GEMV-bandwidth-bound (one token streams every weight once)
// AND launch-gap-bound (~4.3us per serial dispatch), so the layer runs 9 fused dispatches:
//   fused QKV / W1|W3 GEMVs   the P1-lab-proven single-simdgroup-per-row Q8 GEMV
//                             (metal_q8_gemv_msl, f32 activations) over CONCATENATED weight
//                             regions (upload_region_cat3) into one output buffer each —
//                             downstream dispatches bind at byte offsets
//   metal_sq_attn16/32        single-query attention over the KV mirror, one threadgroup per
//                             (stream, head), simdgroup-cooperative coalesced K scoring; depth
//                             > 128 rows takes metal_sq_attn_part16/32 + metal_sq_attn_comb
//                             (chunked split-K: occupancy scales with depth)
//   metal_rope_store16/32     fused rope + KV-row store: q ropes in place, k ropes straight
//                             into the mirror with cvt_f32_to_f16's exact ±65504 clamp (CPU
//                             fallback steps read byte-identical KV), v copies alongside
//   metal_add_rms             fused residual add + rmsnorm (the trailing one feeds the NEXT
//                             layer's attention norm; layer 0 gets a prologue rms)
// rmsnorm / add / swiglu / the classifier GEMV reuse the prefill module's MSL sources verbatim
// (own PSOs, zero copy drift).
//
// GPU-resident KV = a lazy LOGICAL mirror per session, keyed by Session.uid (address keying is
// unsound: scheduler churn ABAs same-geometry sessions, paged sessions have no flat base,
// kv_pool_grow reallocs blobs in place, Stream erase memmoves Sessions). The CPU session cache
// stays the source of truth: uploads gather logical rows [watermark, pos) through kv_runs,
// pos < watermark = rewind detected (the ASR release_kv_pages + n_past = 0 pattern) → re-upload
// from 0, and after each step the GPU's row bytes memcpy VERBATIM back into the CPU cache (the
// CPU never re-encodes — mirror dtype == session dtype by the f16/f32 gate). Flat and paged
// sessions are the same driver code: the GPU kernels only ever see a dense logical slab.


//! Release every GPU object the driver family holds — mirrors, weight regions, pools, PSOs,
//! queue, device (the tests' leak-gate rail, same contract as metal_prefill_shutdown). Lazily
//! re-initializes on the next served step; all mirrors rebuild from the CPU caches on demand.
def public metal_decode_shutdown {
    discard_pre()
    finish_pending_step()   // land any in-flight pipelined step before tearing down
    g_lp_err = false
    metal_kernels_release()
    metal_common_release()
}

def private decode_shape_decline(t : Model) : MetalDecodeDecline {
    if (t.quant != QuantMode.q8) {
        return MetalDecodeDecline.quant_mode
    }
    if (!kq_load_gpu_supported(t)) {   // a fmt without a GPU kernel (the q4_0 QAT tier, ...) must not dispatch
        return MetalDecodeDecline.kquant_native
    }
    if (t.arch != "llama") {
        return MetalDecodeDecline.arch
    }
    if (kernel_backend_needs_repack(active_kernel_backend())) {
        return MetalDecodeDecline.backend_repack
    }
    if (!get_rope_table_mode()) {
        return MetalDecodeDecline.rope_mode
    }
    let c = t.config
    if ((c.rope_neox || c.attn_qkv_bias || c.qk_norm || c.v_norm || c.pre_post_norm) ||
            (c.layer_out_scale || c.attn_sinks || c.attn_logit_softcap > 0.0 || c.sliding_window > 0l) ||
            (c.ffn_act != FfnAct.silu || c.attn_scale > 0.0)) {
        return MetalDecodeDecline.config
    }
    let head_size = c.head_size
    let qd = c.n_heads * head_size
    let kv_dim = layer_kv_dim(c, 0l)
    let hidden = layer_hidden(t, 0l)
    // head_size == the attention threadgroup width: whole simdgroups only, red[] holds <= 4
    if ((qd != c.dim || (head_size % 32l) != 0l || head_size > 128l) ||
            ((c.dim % 32l) != 0l || (kv_dim % 32l) != 0l || (hidden % 32l) != 0l)) {
        return MetalDecodeDecline.shape
    }
    for (l in range64(c.n_layers)) {
        if ((t.kv_src[l] != l || layer_kv_dim(c, l) != kv_dim || layer_head_size(c, l) != head_size) ||
                (layer_hidden(t, l) != hidden || layer_is_sliding(c, l) || t.wv_offs[l] < 0l)) {
            return MetalDecodeDecline.layers
        }
    }
    return MetalDecodeDecline.none
}

// ... plus the decode-specific gates (uniform f16/f32 KV, attention depth, a real session uid)
[unused_argument(pos)]   // depth retired (M2.4) — pos stays in the gate signature for future clauses
def private decode_decline(t : Model; s : Session; pos : int64) : MetalDecodeDecline {
    // mirror + writeback move raw row bytes, so the session codec must equal the mirror dtype; depth is capacity-bounded by the mirror arena, not gated here
    if (s.kv_dtype_k != s.kv_dtype_v ||
            (s.kv_dtype_k != KVDtype.f16 && s.kv_dtype_k != KVDtype.f32 &&
             s.kv_dtype_k != KVDtype.q8_0 && s.kv_dtype_k != KVDtype.tq4)) {
        return MetalDecodeDecline.kv_dtype
    }
    if (s.uid == 0ul) {
        return MetalDecodeDecline.no_uid
    }
    let shape = decode_shape_decline(t)
    if (shape != MetalDecodeDecline.none) {
        return shape
    }
    // forward() built this position's rope row right before the hook
    if (int64(length(s.rope_cos)) < t.config.head_size / 2l) {
        return MetalDecodeDecline.rope_rows
    }
    return MetalDecodeDecline.none
}

def private kq_fmt_at(a : array<KqFmt>; l : int64) : KqFmt => empty(a) ? KqFmt.q8 : a[l]

// One weight site's GEMV — a q8-tagged tensor rides the planar q8 kernels, a kq tensor the
// plane GEMVs (kq_upload_quants/scales + enc_kq_gemv). yoff places the k/v/w3 output segments.
def private enc_site_gemv(enc : MetalComputeEncoder?; t : Model; s16 : bool; fmt : KqFmt; woff, rows, n : int64; bx, by, bn, bd : MetalBuffer?; yoff : uint64 = 0ul) {
    if (fmt == KqFmt.q8) {
        unsafe {
            var bw = upload_region(addr < void? >(t.qblob[woff]), uint64(rows * n))
            var bws = upload_scales(t, woff, rows, n)
            enc_gemv(enc, s16, bw, bws, bx, by, bn, bd, rows, yoff)
        }
    } else {
        var bq = kq_upload_quants(g_dev, t, fmt, woff, rows, n)
        var bs = kq_upload_scales(g_dev, t, fmt, woff, rows, n)
        enc_kq_gemv(enc, fmt, bq, bs, bx, by, bn, bd, rows, n, yoff)
    }
}

// One kq weight site in a batched step. B <= 8: nrows separate single-stream plane GEMVs (the
// kq GEMV is ALU-bound, so this matches the lcpp kq batch wall). B >= 9: the kq mul_mm twin over
// the M-padded x panel. Disjoint y rows/columns: the caller's one barrier covers the whole group.
def private enc_kq_site_b(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows, n : int64;
                          bx : MetalBuffer?; xstride : int64; by : MetalBuffer?; ystride, ycol : int64;
                          bn, bd, bys, bnr : MetalBuffer?; nrows, mp : int64) {
    var bq = kq_upload_quants(g_dev, t, fmt, woff, rows, n)
    var bs = kq_upload_scales(g_dev, t, fmt, woff, rows, n)
    if (nrows > 8l) {
        enc_kq_gemm_mm_b(enc, fmt, bq, bs, bx, by, uint64(ycol * 4l), bn, bys, mp, rows, n)
        return
    }
    // B = 2..8: the ext small-batch twins — one weight pass per R-column group instead of B per-stream passes (-25..-40% at the 3B sites)
    if (nrows >= 2l && bnr != null) {
        assert(xstride == n)
        enc_kq_mvb(enc, fmt, bq, bs, bx, by, uint64(ycol * 4l), bn, bd, bys, bnr, rows, n, nrows)
        return
    }
    for (b in range64(nrows)) {
        enc_kq_gemv(enc, fmt, bq, bs, bx, by, bn, bd, rows, n,
            uint64((b * ystride + ycol) * 4l), uint64(b * xstride * 4l))
    }
}

// Evict least-recently-used mirrors (never `keep`, never a pinned batch row) until the resident
// bytes fit the cap.

// ===== the per-step resource bundle + encode-ahead pipelining =====

// Everything one step's command buffer owns. The encode-ahead rail (g_pre) keeps ONE pre-encoded
// for the predicted next position; forward() pokes the token-dependent bytes (embed row + rope
// row) and COMMITS — encode cost overlaps the PREVIOUS step's GPU run instead of the per-token wall.
struct private StepRes {
    valid : bool
    pos : int64
    uid : uint64
    kdt : KVDtype         // the session/mirror KV codec (kdt == vdt gated)
    s16 : bool
    gpu_logits : bool
    chunked : bool
    spec : bool           // opened with GPU argmax + embed gather — no CPU poke, runs immediately
    committed : bool      // spec pres commit at encode; poked pres commit at poke time (a cb
                          // parked committed across an idle gap trips the GPU watchdog)
    cb : MetalCommandBuffer?
    btok : MetalBuffer?   // the argmax winner (spec steps; read back to verify the prediction)
    koff : uint64         // mirror arena offsets captured at encode time; arena_epoch pins the
    voff : uint64         // BUFFER those offsets index (a grow-rebuild frees it — stale pre-steps discard)
    arena_epoch : int64
    cap : int64
    bx : MetalBuffer?
    bxb : MetalBuffer?
    bxb2 : MetalBuffer?
    bqkv : MetalBuffer?
    bh12 : MetalBuffer?
    bcos : MetalBuffer?
    bsin : MetalBuffer?
    bpart : MetalBuffer?
    bxf : MetalBuffer?
    blog : MetalBuffer?   // pooled staging — only when bnc is unavailable
    bnc : MetalBuffer?    // zero-copy logits: the session's wrapped storage (registry-owned, never released here)
    u_dim : MetalBuffer?
    u_qd : MetalBuffer?
    u_kvd : MetalBuffer?
    u_qkvd : MetalBuffer?
    u_hidden : MetalBuffer?
    u_h2 : MetalBuffer?
    u_hs : MetalBuffer?
    u_kvm : MetalBuffer?
    u_nh : MetalBuffer?
    u_cnt : MetalBuffer?
    u_npq : MetalBuffer?
    u_npall : MetalBuffer?
    u_scale : MetalBuffer?
    u_eps : MetalBuffer?
    u_vocab : MetalBuffer?
    u_nch : MetalBuffer?
    u_nblk : MetalBuffer?  // kv_dim/32 — the block-codec kernels' row-width uniform (q8_0 only)
    bytes_x : uint64
    bytes_q : uint64
    bytes_qkv : uint64
    bytes_h2 : uint64
    bytes_tab : uint64
    bytes_part : uint64
    bytes_log : uint64
}

var private g_pre : StepRes                    // the pre-encoded next step, when valid
// the driver-built rope row for the PREDICTED next position (bit-identical to forward()'s)
var private g_rope_cos_next : array<float>
var private g_rope_sin_next : array<float>

// geometry + pooled buffers + uniforms for one step at `pos` — no input bytes yet (poke_step)
def private acquire_step(t : Model; s : Session; pos : int64; koff, voff : uint64; cap : int64) : StepRes {
    let c = t.config
    let dim = c.dim
    let head_size = c.head_size
    let n_heads = c.n_heads
    let qd = n_heads * head_size
    let kv_dim = layer_kv_dim(c, 0l)
    let hidden = layer_hidden(t, 0l)
    let qkvd = qd + 2l * kv_dim
    let h2 = 2l * hidden
    var r = StepRes(valid = true, pos = pos, uid = s.uid,
        kdt = s.kv_dtype_k, s16 = t.wscale_f16,
        koff = koff, voff = voff, cap = cap, arena_epoch = g_arena_epoch,
        bytes_x = uint64(dim * 4l), bytes_q = uint64(qd * 4l), bytes_qkv = uint64(qkvd * 4l),
        bytes_h2 = uint64(h2 * 4l), bytes_tab = uint64((head_size / 2l) * 4l),
        bytes_log = uint64(c.vocab_size * 4l))
    r.bx = pool_acquire(g_pool, g_dev, r.bytes_x)
    r.bxb = pool_acquire(g_pool, g_dev, r.bytes_q)   // rms out AND attention out (qd == dim gated)
    r.bxb2 = pool_acquire(g_pool, g_dev, r.bytes_x)
    r.bqkv = pool_acquire(g_pool, g_dev, r.bytes_qkv)   // q | roped-in-place q, k, raw v
    r.bh12 = pool_acquire(g_pool, g_dev, r.bytes_h2)    // w1 gate | swiglu in place, w3 up
    r.bcos = pool_acquire_untracked(g_upool, g_dev, r.bytes_tab)
    r.bsin = pool_acquire_untracked(g_upool, g_dev, r.bytes_tab)
    r.u_dim = uniform_u32(uint(dim))
    r.u_qd = uniform_u32(uint(qd))
    r.u_kvd = uniform_u32(uint(kv_dim))
    r.u_qkvd = uniform_u32(uint(qkvd))
    r.u_hidden = uniform_u32(uint(hidden))
    r.u_h2 = uniform_u32(uint(h2))
    r.u_hs = uniform_u32(uint(head_size))
    r.u_kvm = uniform_u32(uint(n_heads / layer_n_kv_heads(c, 0l)))
    r.u_nh = uniform_u32(uint(n_heads))
    r.u_cnt = uniform_u32(uint(pos + 1l))
    r.u_npq = uniform_u32(uint(qd / 2l))
    r.u_npall = uniform_u32(uint((qd + kv_dim) / 2l))
    r.u_scale = uniform_f32(1.0 / sqrt(float(head_size)))
    r.u_eps = uniform_f32(c.norm_eps)
    if (r.kdt == KVDtype.q8_0 || r.kdt == KVDtype.tq4) {
        r.u_nblk = uniform_u32(uint(kv_dim / 32l))
    }
    // logits-on-GPU (default ON, DASLLAMA_METAL_LOGITS=0 pins the CPU classifier) mirrors the prefill gates
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    let tied_f32 = c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8
    // q8 AND K-quant classifiers serve (the kq plane GEMVs — W4); only the tied-fp32 table stays CPU
    r.gpu_logits = (!g_logits_cpu && get_env_variable("DASLLAMA_METAL_LOGITS") != "0" && empty(g_skip) &&
        !tied_f32 && c.final_logit_softcap <= 0.0 &&
        empty(t.suppress) && t.wcls_off >= 0l)
    if (r.gpu_logits) {
        r.bxf = pool_acquire(g_pool, g_dev, r.bytes_x)
        // zero-copy logits write straight into s.logits; the pooled staging is fallback (spec-capable models keep staging too — the speculative chain must only ever touch pooled memory)
        r.bnc = nc_logits_for(g_dev, s)
        if (r.bnc == null || t.cls_q8) {
            r.blog = pool_acquire(g_pool, g_dev, r.bytes_log)
        }
        r.u_vocab = uniform_u32(uint(c.vocab_size))
    }
    // chunked split-K attention above the single-tg depth (128-row chunks, cross-chunk combine)
    r.chunked = pos + 1l > g_attn_single_max
    let nchunks = (pos + 1l + ATTN_CHUNK_ROWS - 1l) / ATTN_CHUNK_ROWS
    r.bytes_part = uint64(n_heads * nchunks * (head_size + 2l) * 4l)
    if (r.chunked) {
        r.bpart = pool_acquire(g_pool, g_dev, r.bytes_part)
        r.u_nch = uniform_u32(uint(nchunks))
    }
    return r
}

// the token-dependent input bytes — the ONLY step state not known at encode time
def private poke_step(s : Session; r : StepRes) {
    unsafe {
        memcpy(metal_buffer_contents(r.bx), addr < void? >(s.x[0]), int(r.bytes_x))
        memcpy(metal_buffer_contents(r.bcos), addr < void? >(s.rope_cos[0]), int(r.bytes_tab))
        memcpy(metal_buffer_contents(r.bsin), addr < void? >(s.rope_sin[0]), int(r.bytes_tab))
    }
}

// encode the whole step (prologue rms, n_layers fused blocks, optional GPU classifier) into a
// fresh command buffer; commit_now submits it, otherwise the caller commits after poking the
// inputs (the encode-ahead rail). A spec step OPENS with argmax + embed gather — no CPU poke.
def private encode_step(t : Model; var r : StepRes; commit_now : bool; var prev_blog : MetalBuffer? = null) {
    let c = t.config
    let dim = c.dim
    let head_size = c.head_size
    let n_heads = c.n_heads
    let qd = n_heads * head_size
    let kv_dim = layer_kv_dim(c, 0l)
    let hidden = layer_hidden(t, 0l)
    let qkvd = qd + 2l * kv_dim
    let h2 = 2l * hidden
    let pos = r.pos
    let cap = r.cap
    let rowb = kv_row_bytes(r.kdt, kv_dim)   // kdt == vdt gated
    let f16v = r.kdt == KVDtype.f16
    let scalar_kv = r.kdt == KVDtype.f16 || r.kdt == KVDtype.f32   // block codecs take their own kernels
    let nchunks = (pos + 1l + ATTN_CHUNK_ROWS - 1l) / ATTN_CHUNK_ROWS
    // UNRETAINED: skips per-dispatch retain/release of ~2000 resource refs — everything referenced outlives the cb here
    var cb = metal_new_command_buffer_unretained(g_queue)
    var enc = metal_new_compute_encoder(cb)
    if (r.spec) {
        // the speculative head: greedy-pick the previous step's logits, gather the winner's embedding row into r.bx
        unsafe {
            var bembq = upload_region(addr < void? >(t.qblob[t.emb_q8_off]), uint64(c.vocab_size * dim))
            var bembs = upload_scales(t, t.emb_q8_off, c.vocab_size, dim)
            enc_argmax(enc, prev_blog, r.btok, r.u_vocab)
            enc_embed(enc, r.s16, bembq, bembs, r.btok, r.bx, r.u_dim, dim)
        }
    }
    // layer-0 attention norm — the per-layer trailing add+rms is fused and feeds the NEXT layer
    if (g_skip != "ew") {
        unsafe {
            var bw_att0 = upload_region(addr < void? >(t.fblob[t.rms_att_off]), uint64(dim * 4l))
            enc_rms(enc, r.bx, bw_att0, r.bxb, r.u_dim, r.u_eps)
        }
    }
    for (l in range64(c.n_layers)) {
        unsafe {
            // attention: fused QKV GEMV -> single-query attention -> out-proj -> fused add+norm; K-quant sites (mixed PER TENSOR formats) split into per-tensor plane GEMVs
            let fq = kq_fmt_at(t.wq_fmt, l)
            let fk = kq_fmt_at(t.wk_fmt, l)
            let fv = kq_fmt_at(t.wv_fmt, l)
            let qkv_q8 = fq == KqFmt.q8 && fk == KqFmt.q8 && fv == KqFmt.q8
            let fused_qkv = r.s16 && scalar_kv && qkv_q8
            if (g_skip != "gemv") {
                if (qkv_q8) {
                    var bwqkv = upload_region_cat3(
                        addr < void? >(t.qblob[t.wq_offs[l]]), uint64(qd * dim),
                        addr < void? >(t.qblob[t.wk_offs[l]]), uint64(kv_dim * dim),
                        addr < void? >(t.qblob[t.wv_offs[l]]), uint64(kv_dim * dim))
                    var bwqkv_s = upload_scales_cat3(t, t.wq_offs[l], qd, t.wk_offs[l], kv_dim, t.wv_offs[l], kv_dim, dim)
                    if (fused_qkv) {
                        enc_gemv_qkv_rs(enc, f16v, bwqkv, bwqkv_s, r.bxb, r.bqkv, g_arena, g_arena,
                            r.koff + uint64((l * cap + pos) * rowb), r.voff + uint64((l * cap + pos) * rowb),
                            r.bcos, r.bsin, r.u_dim, r.u_qkvd, r.u_qd, r.u_kvd, r.u_hs, qkvd)
                    } else {
                        enc_gemv(enc, r.s16, bwqkv, bwqkv_s, r.bxb, r.bqkv, r.u_dim, r.u_qkvd, qkvd)
                    }
                } else {
                    enc_site_gemv(enc, t, r.s16, fq, t.wq_offs[l], qd, dim, r.bxb, r.bqkv, r.u_dim, r.u_qd)
                    enc_site_gemv(enc, t, r.s16, fk, t.wk_offs[l], kv_dim, dim, r.bxb, r.bqkv, r.u_dim, r.u_kvd, uint64(qd * 4l))
                    enc_site_gemv(enc, t, r.s16, fv, t.wv_offs[l], kv_dim, dim, r.bxb, r.bqkv, r.u_dim, r.u_kvd, uint64((qd + kv_dim) * 4l))
                }
            }
            if (g_skip != "ew" && !fused_qkv) {
                if (r.kdt == KVDtype.q8_0) {
                    enc_rope_store_q8(enc, r.bqkv, uint64((qd + kv_dim) * 4l), g_arena, g_arena,
                        r.koff + uint64((l * cap + pos) * rowb), r.voff + uint64((l * cap + pos) * rowb),
                        r.bcos, r.bsin, r.u_qd, r.u_hs, r.u_npq, r.u_nblk, qd / 2l + 2l * (kv_dim / 32l))
                } elif (r.kdt == KVDtype.tq4) {
                    enc_rope_store_tq4(enc, r.bqkv, uint64((qd + kv_dim) * 4l), g_arena, g_arena,
                        r.koff + uint64((l * cap + pos) * rowb), r.voff + uint64((l * cap + pos) * rowb),
                        r.bcos, r.bsin, r.u_qd, r.u_hs, r.u_nh, r.u_kvm,
                        n_heads + 2l * (kv_dim / head_size), head_size)
                } else {
                    enc_rope_store(enc, f16v, r.bqkv, uint64((qd + kv_dim) * 4l), g_arena, g_arena,
                        r.koff + uint64((l * cap + pos) * rowb), r.voff + uint64((l * cap + pos) * rowb),
                        r.bcos, r.bsin, r.u_qd, r.u_hs, r.u_npq, r.u_npall, (qd + kv_dim) / 2l)
                }
            }
            if (g_skip != "attn") {
                if (r.chunked) {
                    if (r.kdt == KVDtype.q8_0) {
                        enc_attn_part_q8(enc, g_arena, g_arena, r.koff + uint64(l * cap * rowb), r.voff + uint64(l * cap * rowb),
                            r.bqkv, r.bpart, r.u_cnt, r.u_nblk, r.u_nh, r.u_kvm, r.u_hs, r.u_scale, r.u_nch, n_heads * nchunks, head_size)
                    } elif (r.kdt == KVDtype.tq4) {
                        enc_attn_part_tq4(enc, g_arena, g_arena, r.koff + uint64(l * cap * rowb), r.voff + uint64(l * cap * rowb),
                            r.bqkv, r.bpart, r.u_cnt, r.u_nblk, r.u_nh, r.u_kvm, r.u_hs, r.u_scale, r.u_nch, n_heads * nchunks, head_size)
                    } else {
                        enc_attn_part(enc, f16v, g_arena, g_arena, r.koff + uint64(l * cap * rowb), r.voff + uint64(l * cap * rowb),
                            r.bqkv, r.bpart, r.u_cnt, r.u_kvd, r.u_nh, r.u_kvm, r.u_hs, r.u_scale, r.u_nch, n_heads * nchunks, head_size)
                    }
                    enc_attn_comb(enc, r.bpart, r.bxb, r.u_nch, r.u_nh, r.u_hs, n_heads, head_size)
                } else {
                    if (r.kdt == KVDtype.q8_0) {
                        enc_attn_q8(enc, g_arena, g_arena, r.koff + uint64(l * cap * rowb), r.voff + uint64(l * cap * rowb),
                            r.bqkv, r.bxb, r.u_cnt, r.u_nblk, r.u_nh, r.u_kvm, r.u_hs, r.u_scale, n_heads, head_size)
                    } elif (r.kdt == KVDtype.tq4) {
                        enc_attn_tq4(enc, g_arena, g_arena, r.koff + uint64(l * cap * rowb), r.voff + uint64(l * cap * rowb),
                            r.bqkv, r.bxb, r.u_cnt, r.u_nblk, r.u_nh, r.u_kvm, r.u_hs, r.u_scale, n_heads, head_size)
                    } else {
                        enc_attn(enc, f16v, g_arena, g_arena, r.koff + uint64(l * cap * rowb), r.voff + uint64(l * cap * rowb),
                            r.bqkv, r.bxb, r.u_cnt, r.u_kvd, r.u_nh, r.u_kvm, r.u_hs, r.u_scale, n_heads, head_size)
                    }
                }
                if (r.kdt == KVDtype.tq4) {
                    // the attention out is rotated-basis per head — un-rotate before the wo projection
                    enc_fwht_unsign(enc, r.bxb, r.u_hs, r.u_qd, n_heads, 1l, head_size)
                }
            }
            if (g_skip != "gemv") {
                enc_site_gemv(enc, t, r.s16, kq_fmt_at(t.wo_fmt, l), t.wo_offs[l], dim, qd, r.bxb, r.bxb2, r.u_qd, r.u_dim)
            }
            // ffn: fused add+norm -> fused W1|W3 GEMV -> swiglu -> W2 GEMV -> fused add + next layer's attention norm
            if (g_skip != "ew") {
                var bw_ffn = upload_region(addr < void? >(t.fblob[t.rms_ffn_off + l * dim]), uint64(dim * 4l))
                enc_add_rms_site(enc, r.bx, r.bxb2, bw_ffn, r.bxb, r.u_dim, r.u_eps, dim)
            }
            let f1 = kq_fmt_at(t.w1_fmt, l)
            let f3 = kq_fmt_at(t.w3_fmt, l)
            let w13_q8 = f1 == KqFmt.q8 && f3 == KqFmt.q8
            if (g_skip != "gemv") {
                if (w13_q8) {
                    var bw13 = upload_region_cat3(
                        addr < void? >(t.qblob[t.w1_offs[l]]), uint64(hidden * dim),
                        addr < void? >(t.qblob[t.w3_offs[l]]), uint64(hidden * dim), null, 0ul)
                    var bw13_s = upload_scales_cat3(t, t.w1_offs[l], hidden, t.w3_offs[l], hidden, 0l, 0l, dim)
                    if (r.s16) {   // fused gate+up dot with the swiglu epilogue — one dispatch
                        enc_gemv_w13sw(enc, bw13, bw13_s, r.bxb, r.bh12, r.u_dim, r.u_hidden, hidden)
                    } else {
                        enc_gemv(enc, false, bw13, bw13_s, r.bxb, r.bh12, r.u_dim, r.u_h2, h2)
                    }
                } else {
                    enc_site_gemv(enc, t, r.s16, f1, t.w1_offs[l], hidden, dim, r.bxb, r.bh12, r.u_dim, r.u_hidden)
                    enc_site_gemv(enc, t, r.s16, f3, t.w3_offs[l], hidden, dim, r.bxb, r.bh12, r.u_dim, r.u_hidden, uint64(hidden * 4l))
                }
            }
            if (g_skip != "ew" && !(r.s16 && w13_q8)) {
                enc_ew2(enc, g_pso_swiglu, r.bh12, 0ul, r.bh12, uint64(hidden * 4l), r.u_hidden, hidden)
            }
            if (g_skip != "gemv") {
                enc_site_gemv(enc, t, r.s16, kq_fmt_at(t.w2_fmt, l), t.w2_offs[l], dim, hidden, r.bh12, r.bxb, r.u_hidden, r.u_dim)
            }
            if (g_skip != "ew") {
                if (l + 1l < c.n_layers) {
                    var bw_attn = upload_region(addr < void? >(t.fblob[t.rms_att_off + (l + 1l) * dim]), uint64(dim * 4l))
                    enc_add_rms_site(enc, r.bx, r.bxb, bw_attn, r.bxb, r.u_dim, r.u_eps, dim)
                } else {
                    enc_ew2(enc, g_pso_add, r.bx, 0ul, r.bxb, 0ul, r.u_dim, dim)
                }
            }
        }
    }
    if (r.gpu_logits) {
        // final rmsnorm (one row) + classifier GEMV — the caller samples straight from s.logits
        unsafe {
            var bwfin = upload_region(addr < void? >(t.fblob[t.rms_final_off]), uint64(dim * 4l))
            enc_rms(enc, r.bx, bwfin, r.bxf, r.u_dim, r.u_eps)
        }
        let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
        var bcls = r.bnc != null ? r.bnc : r.blog
        enc_site_gemv(enc, t, r.s16, cls_fmt, t.wcls_off, c.vocab_size, dim, r.bxf, bcls, r.u_dim, r.u_vocab)
        if (r.bnc != null && r.blog != null) {
            // spec-capable step: mirror zero-copy logits into pooled staging — the next step's speculative argmax reads that, never session memory
            enc_copy_row(enc, r.bnc, 0ul, r.blog, r.u_vocab, c.vocab_size)
        }
    }
    metal_end_encoding(enc)
    metal_release(enc)
    if (commit_now) {
        metal_commit(cb)
        r.committed = true
    }
    r.cb = cb
}

// wait for the step's cb, then hand results back: s.x, the mirrored KV row VERBATIM into the CPU
// cache (the CPU never re-encodes), s.logits when the GPU classifier ran, watermark advance
def private finish_step(t : Model; var s : Session; r : StepRes) : bool {
    let ts_wait = ref_time_ticks()
    metal_wait_until_completed(r.cb)
    let err = metal_command_buffer_error(r.cb)
    g_gpu_ms += (metal_command_buffer_gpu_end_time(r.cb) - metal_command_buffer_gpu_start_time(r.cb)) * 1000.0lf
    g_us_wait += int64(get_time_usec(ts_wait))
    let ok = empty(err)
    if (ok) {
        let ts_rb = ref_time_ticks()
        let c = t.config
        let kv_dim = layer_kv_dim(c, 0l)
        let rowb = kv_row_bytes(s.kv_dtype_k, kv_dim)
        let pos = r.pos
        let cap = r.cap
        unsafe {
            memcpy(addr < void? >(s.x[0]), metal_buffer_contents(r.bx), int(r.bytes_x))
            var kmp = reinterpret<uint8?>(metal_buffer_contents(g_arena)) + r.koff
            var vmp = reinterpret<uint8?>(metal_buffer_contents(g_arena)) + r.voff
            let prow = kv_phys_row(s, pos)
            for (l in range64(c.n_layers)) {
                memcpy(reinterpret<void?>(kv_layer_kbase(s, l, c.seq_len) + prow * rowb),
                    reinterpret<void?>(kmp + (l * cap + pos) * rowb), int(rowb))
                memcpy(reinterpret<void?>(kv_layer_vbase(s, l, c.seq_len) + prow * rowb),
                    reinterpret<void?>(vmp + (l * cap + pos) * rowb), int(rowb))
            }
            if (r.gpu_logits && r.bnc == null) {   // zero-copy steps: the GPU already wrote s.logits
                memcpy(addr < void? >(s.logits[0]), metal_buffer_contents(r.blog), int(r.bytes_log))
            }
        }
        if (r.gpu_logits) {
            decode_override_logits_done()
        }
        g_mirrors[s.uid].watermark = r.pos + 1l
        g_us_readback += int64(get_time_usec(ts_rb))
        g_decodes++
    } else {
        to_log(LOG_ERROR, "dasLLAMA metal decode: dispatch failed ({err}) — falling back to the CPU layer loop\n")
        decline(MetalDecodeDecline.gpu_error)
    }
    return ok
}

def private release_step(var r : StepRes) {
    if (r.cb != null) {
        metal_release(r.cb)
        r.cb = null
    }
    if (r.spec) {
        pool_release(g_pool, r.btok, 64ul)
    }
    pool_release(g_pool, r.bx, r.bytes_x)
    pool_release(g_pool, r.bxb, r.bytes_q)
    pool_release(g_pool, r.bxb2, r.bytes_x)
    pool_release(g_pool, r.bqkv, r.bytes_qkv)
    pool_release(g_pool, r.bh12, r.bytes_h2)
    pool_release(g_upool, r.bcos, r.bytes_tab)
    pool_release(g_upool, r.bsin, r.bytes_tab)
    if (r.gpu_logits) {
        pool_release(g_pool, r.bxf, r.bytes_x)
        if (r.blog != null) {   // bnc steps have no staging; bnc itself is registry-owned
            pool_release(g_pool, r.blog, r.bytes_log)
        }
        pool_release(g_upool, r.u_vocab, 4ul)
    }
    if (r.chunked) {
        pool_release(g_pool, r.bpart, r.bytes_part)
        pool_release(g_upool, r.u_nch, 4ul)
    }
    if (r.u_nblk != null) {
        pool_release(g_upool, r.u_nblk, 4ul)
    }
    pool_release(g_upool, r.u_dim, 4ul)
    pool_release(g_upool, r.u_qd, 4ul)
    pool_release(g_upool, r.u_kvd, 4ul)
    pool_release(g_upool, r.u_qkvd, 4ul)
    pool_release(g_upool, r.u_hidden, 4ul)
    pool_release(g_upool, r.u_h2, 4ul)
    pool_release(g_upool, r.u_hs, 4ul)
    pool_release(g_upool, r.u_kvm, 4ul)
    pool_release(g_upool, r.u_nh, 4ul)
    pool_release(g_upool, r.u_cnt, 4ul)
    pool_release(g_upool, r.u_npq, 4ul)
    pool_release(g_upool, r.u_npall, 4ul)
    pool_release(g_upool, r.u_scale, 4ul)
    pool_release(g_upool, r.u_eps, 4ul)
    r.valid = false
}

// Retire the pre-encoded step. A poked-mode pre was never committed — releasing it drops the work.
// A spec pre is already running: wait it out and SURFACE any execution error — an eaten fault here
// would poison every later submission ("prior/excessive GPU errors") with no trace of the true site.
def private discard_pre {
    if (!g_pre.valid) {
        return
    }
    var r = g_pre
    g_pre.valid = false
    if (r.committed) {
        metal_wait_until_completed(r.cb)
        let err = metal_command_buffer_error(r.cb)
        if (!empty(err)) {
            to_log(LOG_ERROR, "dasLLAMA metal decode: discarded pre-step failed ({err})\n")
        }
    }
    release_step(r)
}

// Pre-encode the PREDICTED next step (same session, pos + 1) while the current step runs on the
// GPU. When speculation gates pass, the cb commits AND chains itself (argmax + embed gather, no
// CPU poke); otherwise it stays uncommitted. Quietly declines when gates fail — slow path serves those.
def private pre_encode_next(t : Model; s : Session; npos : int64; var prev_blog : MetalBuffer?) {
    // quiet non-decline: skipping the pre-encode just means the slow path serves the next step
    if (decode_decline(t, s, npos) != MetalDecodeDecline.none || !key_exists(g_mirrors, s.uid)) {
        return
    }
    let mr = g_mirrors[s.uid]   // cheap copy, capacity check ONLY — row npos-1 is in flight on the GPU
    if (mr.cap_rows < npos + 1l || mr.kdt != s.kv_dtype_k || mr.vdt != s.kv_dtype_v) {
        return
    }
    let ts_enc = ref_time_ticks()
    var r = acquire_step(t, s, npos, mr.koff, mr.voff, mr.cap_rows)
    if (r.gpu_logits && prev_blog != null && t.cls_q8 && t.config.embed_scale == 1.0 &&
            !has_ple(t.config) && spec_armed()) {
        // speculative chain: the cb runs the moment the previous one drains — CPU tail/sampling/next forward() all overlap GPU execution
        r.spec = true
        // a spec step is IN FLIGHT after forward() returns — must not touch session memory (staging only; acceptance lands it via finish_step)
        r.bnc = null
        r.btok = pool_acquire(g_pool, g_dev, 64ul)
        build_decode_rope_row(t, npos, g_rope_cos_next, g_rope_sin_next)
        unsafe {
            memcpy(metal_buffer_contents(r.bcos), addr < void? >(g_rope_cos_next[0]), int(r.bytes_tab))
            memcpy(metal_buffer_contents(r.bsin), addr < void? >(g_rope_sin_next[0]), int(r.bytes_tab))
        }
        encode_step(t, r, true, prev_blog)
    } else {
        encode_step(t, r, false)
    }
    g_pre = r
    g_us_encode += int64(get_time_usec(ts_enc))
}

// the non-pipelined step: mirror catch-up + encode-now + wait; the NEXT step's pre-encode still
// overlaps this step's GPU run. Serves window starts, prediction misses, and speculation reruns.
def private metal_decode_slow(t : Model; var s : Session; pos : int64) : bool {
    let ts_all = ref_time_ticks()
    let mir = mirror_prepare(t, s, pos)
    if (!mir.ok) {
        decline(MetalDecodeDecline.mirror)
        return false
    }
    var r = acquire_step(t, s, pos, mir.koff, mir.voff, mir.cap)
    poke_step(s, r)
    g_us_setup += int64(get_time_usec(ts_all))
    let ts_enc = ref_time_ticks()
    encode_step(t, r, true)
    g_us_encode += int64(get_time_usec(ts_enc))
    pre_encode_next(t, s, pos + 1l, r.blog)
    let ok = finish_step(t, s, r)
    release_step(r)
    return ok
}

//! The DecodeOverrideFn: runs one token step's whole llama layer stack GPU-resident against the
//! session's KV mirror. true = done (KV row stored CPU-side too, s.x = final residual, s.logits
//! when the GPU classifier gates pass); false = declined, the caller runs the CPU layer loop.
def metal_decode_forward(t : Model; var s : Session; token, pos : int64) : bool {
    // mixed batch/single schedules: a pipelined batch step may still own this session's KV row
    // and logits — land it before reading anything session-side (no-op when nothing pends)
    finish_pending_step()
    if (weight_caches_stale()) {   // a model (re)load may have recycled cached weight addresses
        discard_pre()
        weight_caches_flush_on_reload()
    }
    g_lp_err = false
    var why = decode_decline(t, s, pos)
    if (why == MetalDecodeDecline.none && !metal_decode_init()) {
        why = MetalDecodeDecline.device
    }
    if (why != MetalDecodeDecline.none) {
        discard_pre()   // the sequence diverged from the prediction
        decline(why)
        return false
    }
    if (!(g_pre.valid && g_pre.uid == s.uid && g_pre.pos == pos &&
            g_pre.kdt == s.kv_dtype_k && g_pre.s16 == t.wscale_f16 &&
            g_pre.arena_epoch == g_arena_epoch)) {
        discard_pre()
        return metal_decode_slow(t, s, pos)
    }
    // FAST path: this step's cb was encoded during the previous step's GPU run. A spec cb has
    // been running since the previous step drained (GPU-chained argmax + embed); a poked-mode
    // one needs the embed + rope bytes poked, then commits.
    let ts_all = ref_time_ticks()
    var r = g_pre
    g_pre.valid = false
    if (!r.spec) {
        poke_step(s, r)
        metal_commit(r.cb)
        r.committed = true
    }
    g_tick++
    g_mirrors[s.uid].tick = g_tick   // keep the live session hot for the LRU
    g_us_setup += int64(get_time_usec(ts_all))
    pre_encode_next(t, s, pos + 1l, r.blog)
    let ok = finish_step(t, s, r)
    if (ok && r.spec) {
        // verify the GPU's greedy pick against the caller's actual token — a miss means this whole
        // step ran on the wrong embedding: drop the chained pre-step and rerun (the garbage KV row
        // sits at/above the rewound watermark, so mirror_prepare's re-upload repairs it).
        var tokp = unsafe(reinterpret<uint?>(metal_buffer_contents(r.btok)))
        let picked = int64(unsafe(tokp[0]))
        if (picked != token) {
            g_spec_misses++
            g_spec_cooldown = g_spec_backoff
            g_spec_backoff = min(g_spec_backoff * 2, 256)
            discard_pre()
            release_step(r)
            g_decodes--   // finish_step counted the mispredicted step; the rerun recounts
            return metal_decode_slow(t, s, pos)
        }
        g_spec_hits++
        g_spec_backoff = 1
    }
    release_step(r)
    return ok
}

// ===== the batched-decode driver (P4) =====

// GEMV-vs-GEMM site policy: the fixed-B batched GEMV forms win to B = 4, the M-pad-32 GEMM from
// B ~ 5 on every decode site (P1a lab — all sites are big shapes: the QKV concat, wo, w1/w3, w2).
// DASLLAMA_METAL_BATCH_GEMM_MIN overrides for A/B.
var private g_bgemm_min = -1l
// the step splits into DASLLAMA_METAL_BATCH_NCB command buffers (llama.cpp's n_cb shape): the
// ~3.2ms/step scheduling span pipelines behind cb k's execution instead of idling as one big cb;
// queue order serializes the pieces, every encode is token-independent. 1 = the single-cb A/B rail.
var private g_bncb = -1l
// per-site split-K / 64-wide policy: at mp=32 the batch GEMM is OCCUPANCY-bound (d/32 threadgroups
// starve small sites) — grid.y k-slices fix it (kv sk8 +135%, wo sk4 +21%, qkv sk2 +9%, w2 sk8 +33%).
// DASLLAMA_METAL_BATCH_SK=0 pins every site back to the planar 32-wide v2 (the A/B rail).
var private g_bsk = true
// DASLLAMA_METAL_BATCH_CONCURRENT: encode on a CONCURRENT compute encoder, barrier only at a real
// hazard (RAW/WAR/WAW) — the only exploitable overlap in our FUSED chain is W1 || W3. Default ON:
// measured +1-2% at every B (pipelines launch overhead even on the RAW chain). =0 = serial (A/B).
var private g_bconcurrent = true
// DASLLAMA_METAL_BATCH_FUSE: fold w1 + w3 + swiglu into one fused dispatch on the fixed-B path
// (B <= 4) — cuts 2 dispatches + their barriers per layer where inter-dispatch bubbles hurt most.
// Default ON; =0 pins the unfused triple (the A/B rail).
var private g_bfuse = true

// the mul_mv-shape GEMV (MetalQ8MvB2/B4 — the de-verbatim'd Part-3 winner, in-graph == the lcpp
// reference) at every fixed-B site instead of the x-staged forms (which measured -10.5% in-graph:
// the walkthrough lab win was SLC-inflated). DASLLAMA_METAL_BATCH_MV=0 = the x-staged A/B rail.
var private g_bmv = true

// B = 5..8 rides two 4-column mv tiles (weights stream twice) instead of the GEMM + split-K
// policy — the reference's own shape below B = 9. Measured +12.7% at B=8 (239.4 vs 212.5 t/s).
// DASLLAMA_METAL_BATCH_MV8=0 = the GEMM-policy A/B rail.
var private g_bmv8 = true

// pin the batched unfused add+rms pair (ew2-add + rms_b) that dim > ADD_RMS_MAX_DIM selects —
// the tests' parity rail for that path on models whose dim fits the fused slab
var private g_b_addrms_unfused = false

def public set_metal_batch_addrms_unfused(v : bool) {
    g_b_addrms_unfused = v
}

// one-deep encode-ahead for the batch driver: a step commits BEFORE the previous is waited on, so
// sessions hold the PREVIOUS step's logits until the next call. WRONG for logit-gated sampling
// (eval_batch's contract) — OPT-IN for benches only: DASLLAMA_METAL_BATCH_PIPE=1, gpu-logits only.
var private g_b_pipe = false
// per-step scratch reused across calls (row table image + per-row mirror slices)
var private g_b_rte : array<uint>
var private g_b_koffs : array<uint64>
var private g_b_voffs : array<uint64>
var private g_b_caps : array<int64>
var private g_b_ulays : array<MetalBuffer?>   // per-layer uniforms, pooled per step

// the model half is decode_shape_decline; per batch: uniform f16/f32 KV, every row within the
// attention depth cap with a real uid, per-row rope rows built, and dim within the add+rms row
// slabs (GEMM path caps at 3072, fixed-B GEMV at ADD_RMS_MAX_DIM — B >= 5 on a wider model declines)
def private batch_decode_decline(t : Model; ws : BatchWorkspace; sessions : array<Session?>; nrows : int64) : MetalDecodeDecline {
    // kquant batch: B <= 8 serves via per-stream plane GEMVs; each fused site must be format-uniform in CLASS per layer, B >= 9 declines until the kq GEMM lands
    if (t.kquant_native) {
        if (nrows > 8l) {
            // B >= 9 rides the kq mul_mm twins — needs %64 GEMM shapes (vocab %64 too, for GPU-logits)
            let ck = t.config
            let qdk = ck.n_heads * ck.head_size
            if ((ck.dim % 64l) != 0l || (qdk % 64l) != 0l ||
                    (layer_kv_dim(ck, 0l) % 64l) != 0l || (layer_hidden(t, 0l) % 64l) != 0l ||
                    (ck.vocab_size % 64l) != 0l) {
                return MetalDecodeDecline.kquant_native
            }
        }
        for (l in range64(t.config.n_layers)) {
            let kq_qkv = kq_fmt_at(t.wq_fmt, l) != KqFmt.q8
            if ((kq_fmt_at(t.wk_fmt, l) != KqFmt.q8) != kq_qkv || (kq_fmt_at(t.wv_fmt, l) != KqFmt.q8) != kq_qkv) {
                return MetalDecodeDecline.kquant_native
            }
            let kq_w13 = kq_fmt_at(t.w1_fmt, l) != KqFmt.q8
            if ((kq_fmt_at(t.w3_fmt, l) != KqFmt.q8) != kq_w13) {
                return MetalDecodeDecline.kquant_native
            }
        }
    }
    let shape = decode_shape_decline(t)
    if (shape != MetalDecodeDecline.none) {
        return shape
    }
    let c = t.config
    let kdt = ws.scr.kv_dtype_k
    if (ws.scr.kv_dtype_v != kdt ||
            (kdt != KVDtype.f16 && kdt != KVDtype.f32 &&
             kdt != KVDtype.q8_0 && kdt != KVDtype.tq4)) {
        return MetalDecodeDecline.kv_dtype
    }
    for (i in range64(nrows)) {
        if (sessions[i].uid == 0ul) {
            return MetalDecodeDecline.no_uid
        }
    }
    if (int64(length(ws.scr.rope_cos)) < nrows * (c.head_size / 2l)) {
        return MetalDecodeDecline.rope_rows
    }
    return MetalDecodeDecline.none
}

//! The BatchDecodeOverrideFn (P4): one eval_batch step's whole layer stack GPU-resident — B rows
//! through ONE weight pass. STACK-ONLY: on true every row's KV is stored (mirror + CPU cache) and
//! ws.scr.x_b holds the final residuals; false = declined, CPU loop recomputes from clean state.
def metal_batch_decode_forward(t : Model; var ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool {
    if (weight_caches_stale()) {   // a model (re)load may have recycled cached weight addresses
        discard_pre()
        weight_caches_flush_on_reload()
    }
    var why = batch_decode_decline(t, ws, sessions, nrows)
    if (why == MetalDecodeDecline.none && !metal_decode_init()) {
        why = MetalDecodeDecline.device
    }
    if (why != MetalDecodeDecline.none) {
        finish_pending_step()   // the CPU fallback needs the in-flight step's KV/logits landed first
        g_lp_err = false
        batch_decline(why)
        return false
    }
    // a parked single-stream pre-step may reference a batch row's mirror slice — retire it first
    discard_pre()
    // encode-ahead readiness: with a step in flight, mirror_prepare must stay on its fast path
    // (no upload, no eviction, no rebuild — those touch arena slices the GPU is still writing).
    // Any miss → complete the pending step first and take this one synchronously.
    if (g_lp_active) {
        var ready = true
        for (i in range64(nrows)) {
            var sp = sessions[i]
            if (!key_exists(g_mirrors, sp.uid)) {
                ready = false
                break
            }
            let mr = g_mirrors[sp.uid]   // cheap copy — fields tested off it
            if (mr.cap_rows < ws.positions[i] + 1l || mr.kdt != ws.scr.kv_dtype_k ||
                    mr.vdt != ws.scr.kv_dtype_v || mr.watermark < ws.positions[i]) {
                ready = false
                break
            }
        }
        if (!ready) {
            finish_pending_step()
        }
    }
    let ts_all = ref_time_ticks()
    let c = t.config
    let dim = c.dim
    let head_size = c.head_size
    let n_heads = c.n_heads
    let qd = n_heads * head_size
    let kv_dim = layer_kv_dim(c, 0l)
    let hidden = layer_hidden(t, 0l)
    let qkvd = qd + 2l * kv_dim
    let half = head_size / 2l
    let s16 = t.wscale_f16
    let kdt = ws.scr.kv_dtype_k
    let f16 = kdt == KVDtype.f16
    let tq4 = kdt == KVDtype.tq4
    let blockc = kdt == KVDtype.q8_0 || tq4  // block codec: byte-offset row table + codec kernel twins
    let rowb = kv_row_bytes(kdt, kv_dim)
    let esz = f16 ? 2l : 4l
    // reserve the whole step's arena demand BEFORE pinning — growth is a release-all rebuild
    // and cannot run once the batch's rows are pinned (the readiness check above already
    // finished any in-flight step when a session needs a new slice)
    var demand = 0ul
    for (i in range64(nrows)) {
        demand += mirror_arena_demand(t, *sessions[i], ws.positions[i])
    }
    if (!mirror_reserve(demand)) {
        finish_pending_step()
        g_lp_err = false
        batch_decline(MetalDecodeDecline.mirror)
        return false
    }
    // pin the batch's uids: a later row's mirror alloc must never evict an earlier row's slice
    for (i in range64(nrows)) {
        g_pin |> insert(sessions[i].uid)
    }
    g_b_rte |> resize(int(nrows * 4l))
    g_b_koffs |> resize(int(nrows))
    g_b_voffs |> resize(int(nrows))
    g_b_caps |> resize(int(nrows))
    var maxcnt = 0l
    var rows_ok = false
    for (attempt in range(2)) {
        rows_ok = true
        maxcnt = 0l
        for (i in range64(nrows)) {
            var sp = sessions[i]
            let mir = mirror_prepare(t, *sp, ws.positions[i])
            if (!mir.ok) {
                rows_ok = false
                break
            }
            g_b_koffs[i] = mir.koff
            g_b_voffs[i] = mir.voff
            g_b_caps[i] = mir.cap
            let cnt = ws.positions[i] + 1l
            maxcnt = max(maxcnt, cnt)
            // block codecs address in BYTES (there is no scalar element size); scalar mirrors keep
            // their element-offset entries
            // uint32 holds any arena offset: the arena clamps at 4096MB (= 2^32), so offsets < 2^32
            g_b_rte[i * 4l] = uint(blockc ? mir.koff : mir.koff / uint64(esz))
            g_b_rte[i * 4l + 1l] = uint(blockc ? mir.voff : mir.voff / uint64(esz))
            g_b_rte[i * 4l + 2l] = uint(mir.cap)
            g_b_rte[i * 4l + 3l] = uint(cnt)
        }
        if (rows_ok) {
            break
        }
        // an exact-fit step can strand contiguity around older residents — retry ONCE on an
        // empty arena (re-uploads ride the watermark contract; sequential packing then holds
        // whenever the demand fits capacity-wise)
        g_pin |> clear()
        if (attempt != 0 || !mirror_defrag(demand)) {
            break
        }
        for (i in range64(nrows)) {
            g_pin |> insert(sessions[i].uid)
        }
    }
    g_pin |> clear()
    if (!rows_ok) {
        finish_pending_step()   // fall back synchronously — the CPU loop needs completed KV state
        g_lp_err = false
        batch_decline(MetalDecodeDecline.mirror)
        return false
    }
    if (g_bgemm_min < 0l) {
        g_bgemm_min = int64(max(env_int("DASLLAMA_METAL_BATCH_GEMM_MIN", 5), 2))
        g_bsk = env_int("DASLLAMA_METAL_BATCH_SK", 1) != 0
        g_bncb = int64(clamp(env_int("DASLLAMA_METAL_BATCH_NCB", 4), 1, 16))
        g_bconcurrent = env_int("DASLLAMA_METAL_BATCH_CONCURRENT", 1) != 0
        g_bfuse = env_int("DASLLAMA_METAL_BATCH_FUSE", 1) != 0
        g_bmv = env_int("DASLLAMA_METAL_BATCH_MV", 1) != 0
        g_bmv8 = env_int("DASLLAMA_METAL_BATCH_MV8", 1) != 0
        g_b_pipe = env_int("DASLLAMA_METAL_BATCH_PIPE", 0) != 0
    }
    // the fixed-B GEMV forms cover 4 rows — past that the GEMM is mandatory (the knob can only
    // pull the crossover DOWN) unless the MV8 probe column-tiles (its tiling is probe-tuned to
    // dim <= 3072; wider models ride the GEMM's fixed K-tiles at any dim)
    let mv8 = g_bmv8 && g_bmv && nrows > 4l && nrows <= 8l && dim <= 3072l
    let use_gemm = (nrows >= g_bgemm_min || nrows > 4l) && !mv8
    // the mul_mm rail (opt-in, DASLLAMA_METAL_BATCH_MM=1): MEASURED NEGATIVE at M-pad-32 (3B d512
    // -n 32: B=8 145.2 vs GemmB 153.3) — mm's 32x64 tile grids only d/64 threadgroups vs GemmB's
    // 32x32 = 2x occupancy. Kept as the A/B rail for bigger-M futures (B > 16 pads to 64).
    let use_mm = (use_gemm && get_env_variable("DASLLAMA_METAL_BATCH_MM") == "1" &&
        (dim % 64l) == 0l && (qd % 64l) == 0l && (kv_dim % 64l) == 0l && (hidden % 64l) == 0l)
    let mp = use_gemm ? ((nrows + 31l) / 32l) * 32l : (nrows <= 2l ? 2l : nrows <= 4l ? 4l : 8l)
    // the per-site GEMM policy (see g_bsk): each split-K site gates on its OWN K's nkb % S == 0
    // (and the partial planes assume mp == 32); an ineligible site stays on the planar v2
    let sk_ok = use_gemm && !use_mm && g_bsk && mp == 32l
    let qkv_sk = (sk_ok && (dim / 32l) % 2l == 0l) ? 2l : 0l
    let wo_sk = (sk_ok && (qd / 32l) % 4l == 0l) ? 4l : 0l
    let w2_sk = (sk_ok && (hidden / 32l) % 8l == 0l) ? 8l : 0l
    let cls64 = use_gemm && !use_mm && g_bsk && (c.vocab_size % 64l) == 0l
    // the mul_mv rail owns the whole fixed-B path (w13 = two mv dispatches + the ew2 swiglu, so
    // the x-staged w13sw fusion stands down with it)
    let mv = g_bmv && !use_gemm && !use_mm
    // fold w1+w3+swiglu into one fused GEMV on the fixed-B path; a knockout run (g_skip) keeps the
    // stages separable so its per-stage attribution stays valid
    let fuse13 = g_bfuse && !mv && !use_gemm && !use_mm && empty(g_skip) && !t.kquant_native
    let chunked = maxcnt > g_attn_single_max
    let nchmax = (maxcnt + ATTN_CHUNK_ROWS - 1l) / ATTN_CHUNK_ROWS
    // partD (walkthrough kernel-2 winner): fused single-pass slice kernel + parallel comb —
    // lane ownership assumes hs == 128 (32 lanes x half4 = one head); other head sizes keep
    // the proven single/chunked pair. DASLLAMA_METAL_ATTN_D=0 = the A/B rail to the old path.
    let use_attn_d = head_size == 128l && get_env_variable("DASLLAMA_METAL_ATTN_D") != "0"
    let d_nsgs = min((maxcnt + 31l) / 32l, 16l)
    // logits-on-GPU (default ON): the batched final rms + classifier ride the same cb and the
    // CPU tail's classifier (one full weight-plane stream per step — ~6ms of the small-B wall)
    // is skipped via batch_decode_override_logits_done. Gates mirror the single-stream driver's.
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    let tied_f32 = c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8
    let gpu_logits = (!g_logits_cpu && get_env_variable("DASLLAMA_METAL_LOGITS") != "0" && empty(g_skip) &&
        !tied_f32 && c.final_logit_softcap <= 0.0 && empty(t.suppress) && t.wcls_off >= 0l &&
        (cls_fmt != KqFmt.q8 || !use_gemm || (c.vocab_size % (use_mm ? 64l : 32l)) == 0l))
    // pooled per-step buffers (mp rows: the fixed-B forms read their full B, the GEMM its M pad —
    // stale pad rows only ever land in pad outputs, which nothing reads back)
    let bytes_x = uint64(mp * dim * 4l)
    let bytes_xb = uint64(mp * qd * 4l)
    let bytes_qkv = uint64(mp * qkvd * 4l)
    let bytes_h = uint64(mp * hidden * 4l)
    let bytes_tab = uint64(nrows * half * 4l)
    let bytes_rt = uint64(nrows * 16l)
    let bytes_part = uint64(use_attn_d ? nrows * n_heads * d_nsgs * (head_size + 4l) * 4l :
        nrows * n_heads * nchmax * (head_size + 2l) * 4l)
    var bx = pool_acquire(g_pool, g_dev, bytes_x)
    var bxb = pool_acquire(g_pool, g_dev, bytes_xb)
    var bxb2 = pool_acquire(g_pool, g_dev, bytes_x)
    var bqkv = pool_acquire(g_pool, g_dev, bytes_qkv)
    var bhb = pool_acquire(g_pool, g_dev, bytes_h)
    var bhb2 = pool_acquire(g_pool, g_dev, bytes_h)
    var bpart = (use_attn_d || chunked) ? pool_acquire(g_pool, g_dev, bytes_part) : null
    // one pooled partial-plane buffer serves every split-K site (sized to the largest enabled —
    // w2's S8 x 32 x dim is 3MB at 3B); sites run serially, so reuse is free
    var bytes_sk = 0ul
    if (qkv_sk > 0l) {
        bytes_sk = uint64(qkv_sk * 32l * qkvd * 4l)
    }
    if (wo_sk > 0l) {
        bytes_sk = max(bytes_sk, uint64(wo_sk * 32l * dim * 4l))
    }
    if (w2_sk > 0l) {
        bytes_sk = max(bytes_sk, uint64(w2_sk * 32l * dim * 4l))
    }
    var bpsk = bytes_sk > 0ul ? pool_acquire(g_pool, g_dev, bytes_sk) : null
    let bytes_log = uint64(mp * c.vocab_size * 4l)
    var bxf = gpu_logits ? pool_acquire(g_pool, g_dev, bytes_x) : null
    var blog = gpu_logits ? pool_acquire(g_pool, g_dev, bytes_log) : null
    var bcos = pool_acquire_untracked(g_upool, g_dev, bytes_tab)
    var bsin = pool_acquire_untracked(g_upool, g_dev, bytes_tab)
    var brt = pool_acquire_untracked(g_upool, g_dev, bytes_rt)
    var u_dim = uniform_u32(uint(dim))
    var u_qd = uniform_u32(uint(qd))
    var u_kvd = uniform_u32(uint(kv_dim))
    var u_qkvd = uniform_u32(uint(qkvd))
    var u_hidden = uniform_u32(uint(hidden))
    var u_hs = uniform_u32(uint(head_size))
    var u_kvm = uniform_u32(uint(n_heads / layer_n_kv_heads(c, 0l)))
    var u_nh = uniform_u32(uint(n_heads))
    var u_scale = uniform_f32(1.0 / sqrt(float(head_size)))
    var u_eps = uniform_f32(c.norm_eps)
    var u_npq = uniform_u32(uint(qd / 2l))
    var u_npall = uniform_u32(uint((qd + kv_dim) / 2l))
    var u_s16 = uniform_u32(s16 ? 1u : 0u)
    var u_total_dim = uniform_u32(uint(nrows * dim))
    var u_total_h = uniform_u32(uint(nrows * hidden))
    var u_xs4_dim = mv ? uniform_u32(uint(dim / 4l)) : null
    var u_xs4_qd = mv ? uniform_u32(uint(qd / 4l)) : null
    var u_xs4_hid = mv ? uniform_u32(uint(hidden / 4l)) : null
    var u_nrows_mv = (mv && nrows > 2l) ? uniform_u32(uint(min(nrows, 4l))) : null
    var u_nrows_mv2 = (mv && nrows > 4l) ? uniform_u32(uint(nrows - 4l)) : null
    // the kq ext twins' live-column uniform (stores gate on it; pad columns compute and discard)
    var u_nrows_kq = (t.kquant_native && nrows >= 2l && nrows <= 8l) ? uniform_u32(uint(nrows)) : null
    var u_nch = (!use_attn_d && chunked) ? uniform_u32(uint(nchmax)) : null
    var u_nsgs = use_attn_d ? uniform_u32(uint(d_nsgs)) : null
    var u_nblk = blockc ? uniform_u32(uint(kv_dim / 32l)) : null
    var u_vocab = gpu_logits ? uniform_u32(uint(c.vocab_size)) : null
    var u_sk_qkv = qkv_sk > 0l ? uniform_u32(uint(qkv_sk)) : null
    var u_sk_wo = wo_sk > 0l ? uniform_u32(uint(wo_sk)) : null
    var u_sk_w2 = w2_sk > 0l ? uniform_u32(uint(w2_sk)) : null
    var u_skt_qkv = qkv_sk > 0l ? uniform_u32(uint(32l * qkvd)) : null
    var u_skt_dim = (wo_sk > 0l || w2_sk > 0l) ? uniform_u32(uint(32l * dim)) : null
    g_b_ulays |> resize(int(c.n_layers))
    for (l in range64(c.n_layers)) {
        g_b_ulays[l] = uniform_u32(uint(l))
    }
    unsafe {
        memcpy(metal_buffer_contents(bx), addr < void? >(ws.scr.x_b[0]), int(nrows * dim * 4l))
        memcpy(metal_buffer_contents(bcos), addr < void? >(ws.scr.rope_cos[0]), int(bytes_tab))
        memcpy(metal_buffer_contents(bsin), addr < void? >(ws.scr.rope_sin[0]), int(bytes_tab))
        memcpy(metal_buffer_contents(brt), addr < void? >(g_b_rte[0]), int(bytes_rt))
    }
    g_us_bsetup += int64(get_time_usec(ts_all))
    let ts_enc = ref_time_ticks()
    let ncb = min(g_bncb, c.n_layers)
    let lay_per_cb = (c.n_layers + ncb - 1l) / ncb
    var cbs : array<MetalCommandBuffer?>
    cbs |> reserve(int(ncb))
    // concurrent-encode rail: a concurrent encoder + explicit barriers ONLY at real buffer hazards
    // (the serial encoder implicit-barriers every dispatch). Gated to a full pipeline — a knockout
    // (g_skip) run stays on the serial encoder so a skipped stage can't leave a dependent unbarriered.
    let conc = g_bconcurrent && empty(g_skip)
    var haz = HazTrack(on = conc, w = 0u, r = 0u)
    var cb = metal_new_command_buffer_unretained(g_queue)
    var enc = conc ? metal_new_compute_encoder_concurrent(cb) : metal_new_compute_encoder(cb)
    // layer-0 attention norm (the per-layer trailing add+rms feeds the NEXT layer's)
    if (g_skip != "ew") {
        unsafe {
            var bw_att0 = upload_region(addr < void? >(t.fblob[t.rms_att_off]), uint64(dim * 4l))
            bar_dep(enc, haz, HZ_BX, HZ_BXB)
            enc_rms_b(enc, bx, bw_att0, bxb, u_dim, u_eps, nrows)
        }
    }
    for (l in range64(c.n_layers)) {
        unsafe {
            var ulay = g_b_ulays[l]
            // attention: QKV (one concat stream) -> per-row rope + KV store -> per-row attention
            // over [0, cnt_b) -> out-proj -> fused residual add + ffn norm. K-quant sites take
            // nrows per-stream plane GEMVs (class-uniformity per site gated at the decline).
            let fq = kq_fmt_at(t.wq_fmt, l)
            let fo = kq_fmt_at(t.wo_fmt, l)
            let f1 = kq_fmt_at(t.w1_fmt, l)
            let f2 = kq_fmt_at(t.w2_fmt, l)
            if (g_skip != "gemv") {
                if (fq != KqFmt.q8) {
                    bar_dep(enc, haz, HZ_BXB, HZ_BQKV)
                    enc_kq_site_b(enc, t, fq, t.wq_offs[l], qd, dim, bxb, dim, bqkv, qkvd, 0l, u_dim, u_qd, u_qkvd, u_nrows_kq, nrows, mp)
                    enc_kq_site_b(enc, t, kq_fmt_at(t.wk_fmt, l), t.wk_offs[l], kv_dim, dim, bxb, dim, bqkv, qkvd, qd, u_dim, u_kvd, u_qkvd, u_nrows_kq, nrows, mp)
                    enc_kq_site_b(enc, t, kq_fmt_at(t.wv_fmt, l), t.wv_offs[l], kv_dim, dim, bxb, dim, bqkv, qkvd, qd + kv_dim, u_dim, u_kvd, u_qkvd, u_nrows_kq, nrows, mp)
                } elif (use_mm) {
                    // three per-tensor blob dispatches into the ONE fused QKV buffer — y binds at
                    // the tensor's column byte offset, the qkvd row stride rides the ndim uniform;
                    // disjoint writes, so one barrier (bxb RAW) covers all three — they overlap
                    bar_dep(enc, haz, HZ_BXB, HZ_BQKV)
                    enc_gemm_mm_b(enc, blob_of(t, t.wq_offs[l], qd, dim), bxb, bqkv, 0ul, u_dim, u_qkvd, mp, qd)
                    enc_gemm_mm_b(enc, blob_of(t, t.wk_offs[l], kv_dim, dim), bxb, bqkv, uint64(qd * 4l), u_dim, u_qkvd, mp, kv_dim)
                    enc_gemm_mm_b(enc, blob_of(t, t.wv_offs[l], kv_dim, dim), bxb, bqkv, uint64((qd + kv_dim) * 4l), u_dim, u_qkvd, mp, kv_dim)
                } elif (mv) {
                    bar_dep(enc, haz, HZ_BXB, HZ_BQKV)
                    enc_mv_site(enc, blob_cat3(t, t.wq_offs[l], qd, t.wk_offs[l], kv_dim,
                        t.wv_offs[l], kv_dim, dim), bxb, bqkv, u_dim, u_qkvd, u_xs4_dim, qkvd,
                        nrows, uint64(dim * 4l), u_nrows_mv, u_nrows_mv2)
                } else {
                    var bwqkv = upload_region_cat3(
                        addr < void? >(t.qblob[t.wq_offs[l]]), uint64(qd * dim),
                        addr < void? >(t.qblob[t.wk_offs[l]]), uint64(kv_dim * dim),
                        addr < void? >(t.qblob[t.wv_offs[l]]), uint64(kv_dim * dim))
                    var bwqkv_s = upload_scales_cat3(t, t.wq_offs[l], qd, t.wk_offs[l], kv_dim, t.wv_offs[l], kv_dim, dim)
                    if (qkv_sk > 0l) {
                        bar_dep(enc, haz, HZ_BXB, HZ_BPSK)
                        enc_gemm_sk_b(enc, bwqkv, 0ul, bwqkv_s, 0ul, bxb, bpsk, u_dim, u_qkvd, u_s16, u_sk_qkv, qkvd, qkv_sk)
                        bar_dep(enc, haz, HZ_BPSK, HZ_BQKV)
                        enc_sk_reduce_b(enc, bpsk, bqkv, u_skt_qkv, u_sk_qkv, 32l * qkvd)
                    } elif (use_gemm) {
                        bar_dep(enc, haz, HZ_BXB, HZ_BQKV)
                        enc_gemm_b(enc, bwqkv, 0ul, bwqkv_s, 0ul, bxb, bqkv, u_dim, u_qkvd, u_s16, mp, qkvd)
                    } else {
                        bar_dep(enc, haz, HZ_BXB, HZ_BQKV)
                        enc_gemv_b(enc, nrows > 2l, bwqkv, 0ul, bwqkv_s, 0ul, bxb, bqkv, u_dim, u_qkvd, u_s16, qkvd)
                    }
                }
            }
            if (g_skip != "ew") {
                // rope reads+writes bqkv in place and writes the current KV row into the arena
                bar_dep(enc, haz, HZ_BQKV, HZ_BQKV | HZ_ARENA)
                if (tq4) {
                    enc_rope_store_b_tq4(enc, bqkv, bcos, bsin, brt, ulay, u_qd, u_hs, u_nh, u_kvm, u_qkvd,
                        n_heads + 2l * (kv_dim / head_size), nrows, head_size)
                } elif (blockc) {
                    enc_rope_store_b_q8(enc, bqkv, bcos, bsin, brt, ulay, u_nblk, u_qd, u_hs, u_npq, u_qkvd,
                        qd / 2l + 2l * (kv_dim / 32l), nrows)
                } else {
                    enc_rope_store_b(enc, f16, bqkv, bcos, bsin, brt, ulay, u_kvd, u_qd, u_hs, u_npq, u_npall, u_qkvd,
                        (qd + kv_dim) / 2l, nrows)
                }
            }
            if (g_skip != "attn") {
                if (use_attn_d) {
                    bar_dep(enc, haz, HZ_BQKV | HZ_ARENA, HZ_BPART)
                    if (tq4) {
                        enc_attn_d_tq4(enc, bqkv, bpart, brt, ulay, u_nblk, u_nh, u_kvm, u_hs, u_scale, u_qkvd, u_nsgs,
                            d_nsgs, nrows, n_heads)
                    } elif (blockc) {
                        enc_attn_d_q8(enc, bqkv, bpart, brt, ulay, u_nblk, u_nh, u_kvm, u_hs, u_scale, u_qkvd, u_nsgs,
                            d_nsgs, nrows, n_heads)
                    } else {
                        enc_attn_d(enc, f16, bqkv, bpart, brt, ulay, u_kvd, u_nh, u_kvm, u_hs, u_scale, u_qkvd, u_nsgs,
                            d_nsgs, nrows, n_heads)
                    }
                    bar_dep(enc, haz, HZ_BPART, HZ_BXB)
                    enc_attn_comb_d(enc, bpart, bxb, brt, u_nsgs, u_nh, u_hs, nrows, n_heads, head_size)
                } elif (chunked) {
                    bar_dep(enc, haz, HZ_BQKV | HZ_ARENA, HZ_BPART)
                    if (tq4) {
                        enc_attn_part_b_tq4(enc, bqkv, bpart, brt, ulay, u_nblk, u_nh, u_kvm, u_hs, u_scale, u_qkvd, u_nch,
                            nrows, nchmax, n_heads, head_size)
                    } elif (blockc) {
                        enc_attn_part_b_q8(enc, bqkv, bpart, brt, ulay, u_nblk, u_nh, u_kvm, u_hs, u_scale, u_qkvd, u_nch,
                            nrows, nchmax, n_heads, head_size)
                    } else {
                        enc_attn_part_b(enc, f16, bqkv, bpart, brt, ulay, u_kvd, u_nh, u_kvm, u_hs, u_scale, u_qkvd, u_nch,
                            nrows, nchmax, n_heads, head_size)
                    }
                    bar_dep(enc, haz, HZ_BPART, HZ_BXB)
                    enc_attn_comb_b(enc, bpart, bxb, brt, u_nch, u_nh, u_hs, nrows, n_heads, head_size)
                } else {
                    bar_dep(enc, haz, HZ_BQKV | HZ_ARENA, HZ_BXB)
                    if (tq4) {
                        enc_attn_b_tq4(enc, bqkv, bxb, brt, ulay, u_nblk, u_nh, u_kvm, u_hs, u_scale, u_qkvd,
                            nrows, n_heads, head_size)
                    } elif (blockc) {
                        enc_attn_b_q8(enc, bqkv, bxb, brt, ulay, u_nblk, u_nh, u_kvm, u_hs, u_scale, u_qkvd,
                            nrows, n_heads, head_size)
                    } else {
                        enc_attn_b(enc, f16, bqkv, bxb, brt, ulay, u_kvd, u_nh, u_kvm, u_hs, u_scale, u_qkvd,
                            nrows, n_heads, head_size)
                    }
                }
                if (tq4) {
                    // the attention out is rotated-basis per head — un-rotate before the wo projection
                    bar_dep(enc, haz, HZ_BXB, HZ_BXB)
                    enc_fwht_unsign(enc, bxb, u_hs, u_qd, n_heads, nrows, head_size)
                }
            }
            if (g_skip != "gemv") {
                if (fo != KqFmt.q8) {
                    bar_dep(enc, haz, HZ_BXB, HZ_BXB2)
                    enc_kq_site_b(enc, t, fo, t.wo_offs[l], dim, qd, bxb, qd, bxb2, dim, 0l, u_qd, u_dim, u_dim, u_nrows_kq, nrows, mp)
                } elif (use_mm) {
                    bar_dep(enc, haz, HZ_BXB, HZ_BXB2)
                    enc_gemm_mm_b(enc, blob_of(t, t.wo_offs[l], dim, qd), bxb, bxb2, 0ul, u_qd, u_dim, mp, dim)
                } elif (mv) {
                    bar_dep(enc, haz, HZ_BXB, HZ_BXB2)
                    enc_mv_site(enc, blob_of(t, t.wo_offs[l], dim, qd), bxb, bxb2,
                        u_qd, u_dim, u_xs4_qd, dim, nrows, uint64(qd * 4l), u_nrows_mv, u_nrows_mv2)
                } else {
                    var bwo = upload_region(addr < void? >(t.qblob[t.wo_offs[l]]), uint64(dim * qd))
                    var bwo_s = upload_scales(t, t.wo_offs[l], dim, qd)
                    if (wo_sk > 0l) {
                        bar_dep(enc, haz, HZ_BXB, HZ_BPSK)
                        enc_gemm_sk_b(enc, bwo, 0ul, bwo_s, 0ul, bxb, bpsk, u_qd, u_dim, u_s16, u_sk_wo, dim, wo_sk)
                        bar_dep(enc, haz, HZ_BPSK, HZ_BXB2)
                        enc_sk_reduce_b(enc, bpsk, bxb2, u_skt_dim, u_sk_wo, 32l * dim)
                    } elif (use_gemm) {
                        bar_dep(enc, haz, HZ_BXB, HZ_BXB2)
                        enc_gemm_b(enc, bwo, 0ul, bwo_s, 0ul, bxb, bxb2, u_qd, u_dim, u_s16, mp, dim)
                    } else {
                        bar_dep(enc, haz, HZ_BXB, HZ_BXB2)
                        enc_gemv_b(enc, nrows > 2l, bwo, 0ul, bwo_s, 0ul, bxb, bxb2, u_qd, u_dim, u_s16, dim)
                    }
                }
            }
            // ffn: fused add+norm -> W1 / W3 (the cat region at byte offsets) -> swiglu
            // -> W2 -> fused add + the NEXT layer's attention norm (plain add on the last)
            if (g_skip != "ew") {
                var bw_ffn = upload_region(addr < void? >(t.fblob[t.rms_ffn_off + l * dim]), uint64(dim * 4l))
                // add_rms updates the residual bx in place AND writes the norm-out bxb; past the
                // fused kernel's row slab the unfused pair does the same in two dispatches
                bar_dep(enc, haz, HZ_BX | HZ_BXB2, HZ_BX | HZ_BXB)
                if (dim <= ADD_RMS_MAX_DIM && !g_b_addrms_unfused) {
                    enc_add_rms_b(enc, bx, bxb2, bw_ffn, bxb, u_dim, u_eps, nrows)
                } else {
                    enc_ew2(enc, g_pso_add, bx, 0ul, bxb2, 0ul, u_total_dim, nrows * dim)
                    bar_dep(enc, haz, HZ_BX, HZ_BXB)
                    enc_rms_b(enc, bx, bw_ffn, bxb, u_dim, u_eps, nrows)
                }
            }
            if (g_skip != "gemv") {
                if (f1 != KqFmt.q8) {
                    // gate + up into DISJOINT bhb/bhb2 — the ew2 swiglu below fuses them
                    // (fuse13 stands down on kq loads)
                    bar_dep(enc, haz, HZ_BXB, HZ_BHB | HZ_BHB2)
                    enc_kq_site_b(enc, t, f1, t.w1_offs[l], hidden, dim, bxb, dim, bhb, hidden, 0l, u_dim, u_hidden, u_hidden, u_nrows_kq, nrows, mp)
                    enc_kq_site_b(enc, t, kq_fmt_at(t.w3_fmt, l), t.w3_offs[l], hidden, dim, bxb, dim, bhb2, hidden, 0l, u_dim, u_hidden, u_hidden, u_nrows_kq, nrows, mp)
                } elif (use_mm) {
                    bar_dep(enc, haz, HZ_BXB, HZ_BHB | HZ_BHB2)
                    enc_gemm_mm_b(enc, blob_of(t, t.w1_offs[l], hidden, dim), bxb, bhb, 0ul, u_dim, u_hidden, mp, hidden)
                    enc_gemm_mm_b(enc, blob_of(t, t.w3_offs[l], hidden, dim), bxb, bhb2, 0ul, u_dim, u_hidden, mp, hidden)
                } elif (mv) {
                    // two mv dispatches read the same norm and write DISJOINT gate/up — one
                    // barrier covers both; the ew2 swiglu below fuses them (fuse13 stands down)
                    bar_dep(enc, haz, HZ_BXB, HZ_BHB | HZ_BHB2)
                    enc_mv_site(enc, blob_of(t, t.w1_offs[l], hidden, dim), bxb, bhb,
                        u_dim, u_hidden, u_xs4_dim, hidden, nrows, uint64(dim * 4l), u_nrows_mv, u_nrows_mv2)
                    enc_mv_site(enc, blob_of(t, t.w3_offs[l], hidden, dim), bxb, bhb2,
                        u_dim, u_hidden, u_xs4_dim, hidden, nrows, uint64(dim * 4l), u_nrows_mv, u_nrows_mv2)
                } else {
                    var bw13 = upload_region_cat3(
                        addr < void? >(t.qblob[t.w1_offs[l]]), uint64(hidden * dim),
                        addr < void? >(t.qblob[t.w3_offs[l]]), uint64(hidden * dim), null, 0ul)
                    var bw13_s = upload_scales_cat3(t, t.w1_offs[l], hidden, t.w3_offs[l], hidden, 0l, 0l, dim)
                    if (fuse13) {
                        // one fused dispatch: gate+up dot + swiglu straight into bhb (w2's input)
                        bar_dep(enc, haz, HZ_BXB, HZ_BHB)
                        enc_gemv_w13sw_b(enc, nrows > 2l, bw13, bw13_s, bxb, bhb, u_dim, u_hidden, u_s16, hidden)
                    } else {
                        // W1 and W3 both read the ffn-norm output and write DISJOINT buffers (bhb / bhb2)
                        // — one barrier (bxb RAW) covers both; they then run concurrently
                        bar_dep(enc, haz, HZ_BXB, HZ_BHB | HZ_BHB2)
                        let w3qoff = uint64(hidden * dim)
                        let w3soff = uint64(hidden * (dim / 32l) * (s16 ? 2l : 4l))
                        if (use_gemm) {
                            enc_gemm_b(enc, bw13, 0ul, bw13_s, 0ul, bxb, bhb, u_dim, u_hidden, u_s16, mp, hidden)
                            enc_gemm_b(enc, bw13, w3qoff, bw13_s, w3soff, bxb, bhb2, u_dim, u_hidden, u_s16, mp, hidden)
                        } else {
                            enc_gemv_b(enc, nrows > 2l, bw13, 0ul, bw13_s, 0ul, bxb, bhb, u_dim, u_hidden, u_s16, hidden)
                            enc_gemv_b(enc, nrows > 2l, bw13, w3qoff, bw13_s, w3soff, bxb, bhb2, u_dim, u_hidden, u_s16, hidden)
                        }
                    }
                }
            }
            if (g_skip != "ew" && !fuse13) {
                // swiglu reads gate (bhb) + up (bhb2), writes silu(gate)*up back into bhb
                bar_dep(enc, haz, HZ_BHB | HZ_BHB2, HZ_BHB)
                enc_ew2(enc, g_pso_swiglu, bhb, 0ul, bhb2, 0ul, u_total_h, nrows * hidden)
            }
            if (g_skip != "gemv") {
                if (f2 != KqFmt.q8) {
                    bar_dep(enc, haz, HZ_BHB, HZ_BXB)
                    enc_kq_site_b(enc, t, f2, t.w2_offs[l], dim, hidden, bhb, hidden, bxb, dim, 0l, u_hidden, u_dim, u_dim, u_nrows_kq, nrows, mp)
                } elif (use_mm) {
                    bar_dep(enc, haz, HZ_BHB, HZ_BXB)
                    enc_gemm_mm_b(enc, blob_of(t, t.w2_offs[l], dim, hidden), bhb, bxb, 0ul, u_hidden, u_dim, mp, dim)
                } elif (mv) {
                    bar_dep(enc, haz, HZ_BHB, HZ_BXB)
                    enc_mv_site(enc, blob_of(t, t.w2_offs[l], dim, hidden), bhb, bxb,
                        u_hidden, u_dim, u_xs4_hid, dim, nrows, uint64(hidden * 4l), u_nrows_mv, u_nrows_mv2)
                } else {
                    var bw2 = upload_region(addr < void? >(t.qblob[t.w2_offs[l]]), uint64(dim * hidden))
                    var bw2_s = upload_scales(t, t.w2_offs[l], dim, hidden)
                    if (w2_sk > 0l) {
                        bar_dep(enc, haz, HZ_BHB, HZ_BPSK)
                        enc_gemm_sk_b(enc, bw2, 0ul, bw2_s, 0ul, bhb, bpsk, u_hidden, u_dim, u_s16, u_sk_w2, dim, w2_sk)
                        bar_dep(enc, haz, HZ_BPSK, HZ_BXB)
                        enc_sk_reduce_b(enc, bpsk, bxb, u_skt_dim, u_sk_w2, 32l * dim)
                    } elif (use_gemm) {
                        bar_dep(enc, haz, HZ_BHB, HZ_BXB)
                        enc_gemm_b(enc, bw2, 0ul, bw2_s, 0ul, bhb, bxb, u_hidden, u_dim, u_s16, mp, dim)
                    } else {
                        bar_dep(enc, haz, HZ_BHB, HZ_BXB)
                        enc_gemv_b(enc, nrows > 2l, bw2, 0ul, bw2_s, 0ul, bhb, bxb, u_hidden, u_dim, u_s16, dim)
                    }
                }
            }
            if (g_skip != "ew") {
                if (l + 1l < c.n_layers) {
                    var bw_attn = upload_region(addr < void? >(t.fblob[t.rms_att_off + (l + 1l) * dim]), uint64(dim * 4l))
                    bar_dep(enc, haz, HZ_BX | HZ_BXB, HZ_BX | HZ_BXB)
                    if (dim <= ADD_RMS_MAX_DIM && !g_b_addrms_unfused) {
                        enc_add_rms_b(enc, bx, bxb, bw_attn, bxb, u_dim, u_eps, nrows)
                    } else {
                        enc_ew2(enc, g_pso_add, bx, 0ul, bxb, 0ul, u_total_dim, nrows * dim)
                        bar_dep(enc, haz, HZ_BX, HZ_BXB)
                        enc_rms_b(enc, bx, bw_attn, bxb, u_dim, u_eps, nrows)
                    }
                } else {
                    // last layer: plain residual add into bx (the final rms reads bx)
                    bar_dep(enc, haz, HZ_BX | HZ_BXB, HZ_BX)
                    enc_ew2(enc, g_pso_add, bx, 0ul, bxb, 0ul, u_total_dim, nrows * dim)
                }
            }
        }
        // group boundary: commit this slice so the driver schedules it under the earlier
        // slices' GPU execution; queue order serializes the pieces
        if ((l + 1l) % lay_per_cb == 0l && l + 1l < c.n_layers) {
            metal_end_encoding(enc)
            metal_release(enc)
            metal_commit(cb)
            cbs |> push(cb)
            cb = metal_new_command_buffer_unretained(g_queue)
            enc = conc ? metal_new_compute_encoder_concurrent(cb) : metal_new_compute_encoder(cb)
            // fresh encoder + in-order queue serialize cross-cb hazards — start the tracker clean
            haz.w = 0u
            haz.r = 0u
        }
    }
    var nc_all = false
    if (gpu_logits) {
        // batched final rmsnorm + classifier — the caller's CPU classifier step is skipped
        unsafe {
            var bwfin = upload_region(addr < void? >(t.fblob[t.rms_final_off]), uint64(dim * 4l))
            bar_dep(enc, haz, HZ_BX, HZ_BXF)
            enc_rms_b(enc, bx, bwfin, bxf, u_dim, u_eps, nrows)
            bar_dep(enc, haz, HZ_BXF, HZ_BLOG)
            if (cls_fmt != KqFmt.q8) {
                enc_kq_site_b(enc, t, cls_fmt, t.wcls_off, c.vocab_size, dim, bxf, dim, blog, c.vocab_size, 0l, u_dim, u_vocab, u_vocab, u_nrows_kq, nrows, mp)
            } elif (use_mm) {
                enc_gemm_mm_b(enc, blob_of(t, t.wcls_off, c.vocab_size, dim), bxf, blog, 0ul, u_dim, u_vocab, mp, c.vocab_size)
            } elif (mv) {
                enc_mv_site(enc, blob_of(t, t.wcls_off, c.vocab_size, dim), bxf, blog,
                    u_dim, u_vocab, u_xs4_dim, c.vocab_size, nrows, uint64(dim * 4l), u_nrows_mv, u_nrows_mv2)
            } else {
                var bwc = upload_region(addr < void? >(t.qblob[t.wcls_off]), uint64(c.vocab_size * dim))
                var bwc_s = upload_scales(t, t.wcls_off, c.vocab_size, dim)
                if (cls64) {
                    enc_gemm64_b(enc, bwc, 0ul, bwc_s, 0ul, bxf, blog, u_dim, u_vocab, u_s16, mp, c.vocab_size)
                } elif (use_gemm) {
                    enc_gemm_b(enc, bwc, 0ul, bwc_s, 0ul, bxf, blog, u_dim, u_vocab, u_s16, mp, c.vocab_size)
                } else {
                    enc_gemv_b(enc, nrows > 2l, bwc, 0ul, bwc_s, 0ul, bxf, blog, u_dim, u_vocab, u_s16, c.vocab_size)
                }
            }
        }
        // zero-copy logits: scatter each staging row straight into its session's wrapped storage —
        // the landing memcpy disappears. All-or-nothing per step (one unwrappable session keeps the
        // whole step on the landing scatter); in pipe mode this writes s.logits ASYNCHRONOUSLY.
        nc_all = true
        for (i in range64(nrows)) {
            nc_all &&= nc_logits_for(g_dev, *sessions[i]) != null
        }
        if (nc_all) {
            bar_dep(enc, haz, HZ_BLOG, 0u)
            for (i in range64(nrows)) {
                enc_copy_row(enc, blog, uint64(i * c.vocab_size * 4l), nc_logits_for(g_dev, *sessions[i]), u_vocab, c.vocab_size)
            }
        }
    }
    metal_end_encoding(enc)
    metal_release(enc)
    metal_commit(cb)
    cbs |> push(cb)
    g_us_bencode += int64(get_time_usec(ts_enc))
    // optimistic watermark at commit: the in-order queue writes row pos before any later-committed
    // work reads the mirror — the pipe's fast-path prepare must not re-upload it from the (not yet
    // written back) CPU cache
    for (i in range64(nrows)) {
        var sp = sessions[i]
        if (key_exists(g_mirrors, sp.uid)) {
            let wm0 = g_mirrors[sp.uid].watermark
            g_mirrors[sp.uid].watermark = max(wm0, ws.positions[i] + 1l)
        }
    }
    // the overlap: the PREVIOUS step completes only now, with this one already committed behind it
    finish_pending_step()
    g_lp_err = false   // a lost pipelined step was logged + counted; this step retries fresh
    // stash this step as the pending one (finish_pending_step just cleared every slot)
    g_lp_active = true
    g_lp_cbs <- cbs
    g_lp_pooled |> push((buf = bx, size = bytes_x))
    g_lp_pooled |> push((buf = bxb, size = bytes_xb))
    g_lp_pooled |> push((buf = bxb2, size = bytes_x))
    g_lp_pooled |> push((buf = bqkv, size = bytes_qkv))
    g_lp_pooled |> push((buf = bhb, size = bytes_h))
    g_lp_pooled |> push((buf = bhb2, size = bytes_h))
    if (use_attn_d || chunked) {
        g_lp_pooled |> push((buf = bpart, size = bytes_part))
    }
    if (bytes_sk > 0ul) {
        g_lp_pooled |> push((buf = bpsk, size = bytes_sk))
    }
    if (use_attn_d) {
        g_lp_upooled |> push((buf = u_nsgs, size = 4ul))
    } elif (chunked) {
        g_lp_upooled |> push((buf = u_nch, size = 4ul))
    }
    if (u_nblk != null) {
        g_lp_upooled |> push((buf = u_nblk, size = 4ul))
    }
    if (gpu_logits) {
        g_lp_pooled |> push((buf = bxf, size = bytes_x))
        g_lp_pooled |> push((buf = blog, size = bytes_log))
        g_lp_upooled |> push((buf = u_vocab, size = 4ul))
    }
    if (qkv_sk > 0l) {
        g_lp_upooled |> push((buf = u_sk_qkv, size = 4ul))
        g_lp_upooled |> push((buf = u_skt_qkv, size = 4ul))
    }
    if (wo_sk > 0l) {
        g_lp_upooled |> push((buf = u_sk_wo, size = 4ul))
    }
    if (w2_sk > 0l) {
        g_lp_upooled |> push((buf = u_sk_w2, size = 4ul))
    }
    if (wo_sk > 0l || w2_sk > 0l) {
        g_lp_upooled |> push((buf = u_skt_dim, size = 4ul))
    }
    if (mv) {
        g_lp_upooled |> push((buf = u_xs4_dim, size = 4ul))
        g_lp_upooled |> push((buf = u_xs4_qd, size = 4ul))
        g_lp_upooled |> push((buf = u_xs4_hid, size = 4ul))
        if (u_nrows_mv != null) {
            g_lp_upooled |> push((buf = u_nrows_mv, size = 4ul))
        }
        if (u_nrows_mv2 != null) {
            g_lp_upooled |> push((buf = u_nrows_mv2, size = 4ul))
        }
        if (u_nrows_kq != null) {
            g_lp_upooled |> push((buf = u_nrows_kq, size = 4ul))
        }
    }
    g_lp_upooled |> push((buf = bcos, size = bytes_tab))
    g_lp_upooled |> push((buf = bsin, size = bytes_tab))
    g_lp_upooled |> push((buf = brt, size = bytes_rt))
    g_lp_upooled |> push((buf = u_dim, size = 4ul))
    g_lp_upooled |> push((buf = u_qd, size = 4ul))
    g_lp_upooled |> push((buf = u_kvd, size = 4ul))
    g_lp_upooled |> push((buf = u_qkvd, size = 4ul))
    g_lp_upooled |> push((buf = u_hidden, size = 4ul))
    g_lp_upooled |> push((buf = u_hs, size = 4ul))
    g_lp_upooled |> push((buf = u_kvm, size = 4ul))
    g_lp_upooled |> push((buf = u_nh, size = 4ul))
    g_lp_upooled |> push((buf = u_scale, size = 4ul))
    g_lp_upooled |> push((buf = u_eps, size = 4ul))
    g_lp_upooled |> push((buf = u_npq, size = 4ul))
    g_lp_upooled |> push((buf = u_npall, size = 4ul))
    g_lp_upooled |> push((buf = u_s16, size = 4ul))
    g_lp_upooled |> push((buf = u_total_dim, size = 4ul))
    g_lp_upooled |> push((buf = u_total_h, size = 4ul))
    g_lp_upooled |> reserve((g_lp_upooled |> length()) + length(g_b_ulays))
    for (ul in g_b_ulays) {
        g_lp_upooled |> push((buf = ul, size = 4ul))
    }
    g_b_ulays |> clear()
    g_lp_blog = gpu_logits && !nc_all ? blog : null   // nc steps: the GPU scatter already fed s.logits
    g_lp_xb = null
    if (!gpu_logits) {
        g_lp_xb = bx   // the CPU-classifier tail reads x_b — sync-only writeback
        unsafe {
            g_lp_xb_dst = addr < void? >(ws.scr.x_b[0])
        }
        g_lp_xb_bytes = nrows * dim * 4l
    }
    g_lp_sessions |> clear()
    g_lp_sessions |> reserve(int(nrows))
    g_lp_positions |> resize(int(nrows))
    g_lp_koffs |> resize(int(nrows))
    g_lp_voffs |> resize(int(nrows))
    g_lp_caps |> resize(int(nrows))
    for (i in range64(nrows)) {
        g_lp_sessions |> push(sessions[i])
        g_lp_positions[i] = ws.positions[i]
        g_lp_koffs[i] = g_b_koffs[i]
        g_lp_voffs[i] = g_b_voffs[i]
        g_lp_caps[i] = g_b_caps[i]
    }
    g_lp_nrows = nrows
    g_lp_layers = c.n_layers
    g_lp_seq = c.seq_len
    g_lp_vocab = c.vocab_size
    g_lp_rowb = rowb
    if (!(g_b_pipe && gpu_logits && empty(g_skip))) {
        finish_pending_step()   // synchronous path: complete + read back immediately
        if (g_lp_err) {
            g_lp_err = false
            return false   // the dispatch failed — the caller reruns this step on the CPU loop
        }
    }
    if (gpu_logits) {
        batch_decode_override_logits_done()
    }
    return true
}



[init]
def dasllama_metal_llama_register() {
    // registered but dormant: only select_decode_override("metal") (env rail DASLLAMA_PIN_DECODE
    // in the harnesses) activates it — the batch twin rides the same name — and unsupported
    // shapes decline per call
    register_decode_override("metal", @@metal_decode_forward)
    register_batch_decode_override("metal", @@metal_batch_decode_forward)
    if (has_env_variable("DASLLAMA_METAL_DECODE_SKIP")) {
        g_skip = get_env_variable("DASLLAMA_METAL_DECODE_SKIP")
        to_log(LOG_WARNING, "dasLLAMA metal decode: SKIP={g_skip} — timing-attribution mode, output is GARBAGE\n")
    }
}
